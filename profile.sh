#!/usr/bin/env bash
# Usage:
#   ./profile.sh                  # seed=42, ticks=1000
#   ./profile.sh 99               # seed=99, ticks=1000
#   ./profile.sh 99 5000          # seed=99, ticks=5000
#   ./profile.sh 42 1000 --no-flamegraph   # skip flamegraph (if perf/perl not available)
set -euo pipefail

# -------------------------------------------------------------------------
# Config
# -------------------------------------------------------------------------
SEED=${1:-42}
TICKS=${2:-1000}
SKIP_FLAMEGRAPH=${3:-""}

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$REPO_ROOT/build_profile"
BINARY="$BUILD_DIR/bin/main"
PROFILES_DIR="$REPO_ROOT/profiles"
RUN_DIR="$PROFILES_DIR/seed${SEED}_t${TICKS}_$(date +%Y%m%d_%H%M%S)"
BASELINE_FILE="$PROFILES_DIR/baseline_s${SEED}_t${TICKS}.txt"

# -------------------------------------------------------------------------
# Helpers
# -------------------------------------------------------------------------
bold()  { printf '\033[1m%s\033[0m\n' "$*"; }
info()  { printf '  \033[34m=>\033[0m %s\n' "$*"; }
ok()    { printf '  \033[32m✓\033[0m  %s\n' "$*"; }
fail()  { printf '  \033[31m✗\033[0m  %s\n' "$*"; }
warn()  { printf '  \033[33m!\033[0m  %s\n' "$*"; }

# -------------------------------------------------------------------------
# Detect OS and pick profiler
# -------------------------------------------------------------------------
OS="$(uname -s)"
HAS_PERF=false
HAS_INSTRUMENTS=false
HAS_SAMPLY=false

if [[ "$OS" == "Linux" ]]; then
    command -v perf &>/dev/null && HAS_PERF=true
elif [[ "$OS" == "Darwin" ]]; then
    command -v instruments &>/dev/null && HAS_INSTRUMENTS=true
    command -v samply     &>/dev/null && HAS_SAMPLY=true
fi

HAS_FLAMEGRAPH_TOOLS=false
if command -v stackcollapse-perf.pl &>/dev/null && command -v flamegraph.pl &>/dev/null; then
    HAS_FLAMEGRAPH_TOOLS=true
fi

# -------------------------------------------------------------------------
# Step 1: Build
# -------------------------------------------------------------------------
bold "==> Building (RelWithDebInfo)..."

cmake -S "$REPO_ROOT" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_CXX_FLAGS="-O2 -g -fno-omit-frame-pointer" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    > /dev/null 2>&1

cmake --build "$BUILD_DIR" --parallel "$(nproc 2>/dev/null || sysctl -n hw.logicalcpu 2>/dev/null || echo 4)" \
    > /dev/null 2>&1

ok "Build complete: $BINARY"

# -------------------------------------------------------------------------
# Step 2: Run headless, capture output
# -------------------------------------------------------------------------
bold "==> Running headless (seed=$SEED, ticks=$TICKS)..."

mkdir -p "$RUN_DIR"

RAW_OUTPUT=$("$BINARY" "$SEED" "$TICKS" 2>&1)
echo "$RAW_OUTPUT"
echo "$RAW_OUTPUT" > "$RUN_DIR/run_output.txt"

CHECKSUM=$(echo "$RAW_OUTPUT" | grep '^CHECKSUM' | awk '{print $2}')

if [[ -z "$CHECKSUM" ]]; then
    fail "Could not extract checksum from output — did the binary crash?"
    exit 1
fi

# -------------------------------------------------------------------------
# Step 3: Determinism check
# -------------------------------------------------------------------------
bold "==> Checking determinism..."

if [[ -f "$BASELINE_FILE" ]]; then
    EXPECTED=$(cat "$BASELINE_FILE")
    if [[ "$CHECKSUM" == "$EXPECTED" ]]; then
        ok "Determinism verified (checksum=$CHECKSUM)"
    else
        fail "CHECKSUM MISMATCH"
        fail "  Expected: $EXPECTED"
        fail "  Got:      $CHECKSUM"
        warn "This means a refactor broke deterministic reproducibility."
        warn "Delete $BASELINE_FILE to reset the baseline."
        exit 1
    fi
else
    echo "$CHECKSUM" > "$BASELINE_FILE"
    ok "Baseline saved: $BASELINE_FILE (checksum=$CHECKSUM)"
fi

# -------------------------------------------------------------------------
# Step 4: Profile
# -------------------------------------------------------------------------
bold "==> Profiling..."

if [[ "$OS" == "Linux" ]] && $HAS_PERF; then

    info "Using perf..."
    perf record \
        -g \
        --call-graph dwarf \
        --output="$RUN_DIR/perf.data" \
        -- "$BINARY" "$SEED" "$TICKS" \
        > /dev/null 2>&1

    # Top functions (flat profile, no children)
    info "Top 20 hottest functions:"
    perf report \
        -i "$RUN_DIR/perf.data" \
        --stdio \
        --no-children \
        --sort=symbol \
        2>/dev/null \
        | grep -v '^#' \
        | head -25 \
        | tee "$RUN_DIR/top_functions.txt"

    # Flamegraph
    if $HAS_FLAMEGRAPH_TOOLS && [[ "$SKIP_FLAMEGRAPH" != "--no-flamegraph" ]]; then
        info "Generating flamegraph..."
        perf script -i "$RUN_DIR/perf.data" 2>/dev/null \
            | stackcollapse-perf.pl \
            | flamegraph.pl \
            > "$RUN_DIR/flame.svg"
        ok "Flamegraph: $RUN_DIR/flame.svg"
    else
        warn "Skipping flamegraph (install FlameGraph: https://github.com/brendangregg/FlameGraph)"
        warn "  Then: perf script -i $RUN_DIR/perf.data | stackcollapse-perf.pl | flamegraph.pl > flame.svg"
    fi

    ok "Full interactive report: perf report -i $RUN_DIR/perf.data"

elif [[ "$OS" == "Darwin" ]] && $HAS_SAMPLY; then

    info "Using samply..."
    samply record --output "$RUN_DIR/profile.json" \
        -- "$BINARY" "$SEED" "$TICKS" \
        > /dev/null 2>&1
    ok "Open with: samply load $RUN_DIR/profile.json"

elif [[ "$OS" == "Darwin" ]] && $HAS_INSTRUMENTS; then

    info "Using Instruments (xctrace)..."
    xctrace record \
        --template 'Time Profiler' \
        --output "$RUN_DIR/trace.xtrace" \
        --launch -- "$BINARY" "$SEED" "$TICKS" \
        > /dev/null 2>&1
    ok "Open with: open $RUN_DIR/trace.xtrace"

else

    warn "No profiler found. Install one of:"
    if [[ "$OS" == "Linux" ]]; then
        warn "  perf:     sudo apt install linux-perf   (or linux-tools-generic)"
        warn "  flamegraph: https://github.com/brendangregg/FlameGraph"
    else
        warn "  samply:   cargo install samply"
        warn "  Instruments: included with Xcode"
    fi
    warn ""
    warn "Timing data was still captured in: $RUN_DIR/run_output.txt"

fi

# -------------------------------------------------------------------------
# Step 5: Compare against previous run (if one exists)
# -------------------------------------------------------------------------
bold "==> Comparing to previous runs..."

PREV_RUNS=( $(ls -dt "$PROFILES_DIR"/seed${SEED}_t${TICKS}_* 2>/dev/null | grep -v "$RUN_DIR" | head -3) )

if [[ ${#PREV_RUNS[@]} -eq 0 ]]; then
    info "No previous runs to compare against for seed=$SEED ticks=$TICKS"
else
    CURRENT_ELAPSED=$(grep '^Elapsed:' "$RUN_DIR/run_output.txt" | awk '{print $2}' | tr -d 's')
    CURRENT_AVG=$(grep '^Avg/tick:' "$RUN_DIR/run_output.txt" | awk '{print $2}' | tr -d 'ms')

    printf "  %-40s %10s %12s\n" "Run" "Elapsed" "Avg/tick"
    printf "  %-40s %10s %12s\n" "---" "-------" "--------"

    for prev in "${PREV_RUNS[@]}"; do
        if [[ -f "$prev/run_output.txt" ]]; then
            P_ELAPSED=$(grep '^Elapsed:' "$prev/run_output.txt" | awk '{print $2}')
            P_AVG=$(grep '^Avg/tick:' "$prev/run_output.txt" | awk '{print $2}')
            printf "  %-40s %10s %12s\n" "$(basename "$prev")" "$P_ELAPSED" "$P_AVG"
        fi
    done

    printf "  %-40s %10s %12s  <-- current\n" "$(basename "$RUN_DIR")" "${CURRENT_ELAPSED}s" "${CURRENT_AVG}ms"
fi

# -------------------------------------------------------------------------
# Done
# -------------------------------------------------------------------------
bold ""
bold "==> Done. Results in: $RUN_DIR"