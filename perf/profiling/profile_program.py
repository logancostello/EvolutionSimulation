#perf/profiling/profile_program.py
"""
Given an executable path, generates flamegraph SVG
"""

import os
import subprocess
import sys
from pathlib import Path

from ..util.util import run_process, current_os

# Config
FLAMEGRAPH_DIR_PATH = Path("./perf/flamegraph")
PROFILE_DIR         = Path("./profile_graphics")
RUN_SEED            = 9282024
EVAL_TICKS          = 1_000

# Profilers
def profile_linux(exec_path: Path, output_path: Path) -> None:
    data_path           = output_path / "perf.data"
    out_path            = output_path / "out.perf"
    fold_path           = output_path / "out.folded"
    flamegraph_path     = output_path / "flamegraph.svg"
    fold_script         = FLAMEGRAPH_DIR_PATH / "stackcollapse-perf.pl"
    flamegraph_script   = FLAMEGRAPH_DIR_PATH / "flamegraph.pl"

    # Sample
    run_process(["sudo", "perf", "record", "-g", "-o", data_path,
          exec_path, str(RUN_SEED), str(EVAL_TICKS)])

    # Convert binary
    result = run_process(["sudo", "perf", "script", "-i", data_path],
                  capture_output=True, text=True)
    out_path.write_text(result.stdout)

    # Collapse stacks
    result = run_process(["perl", fold_script, out_path],
                  capture_output=True, text=True)
    fold_path.write_text(result.stdout)

    # Render flamegraph
    result = run_process(["perl", flamegraph_script, fold_path],
                  capture_output=True, text=True)
    flamegraph_path.write_text(result.stdout)

    print(f"Flamegraph written to: {flamegraph_path}")

def profile_macos(exec_path: Path, output_path: Path) -> None:
    dtrace_out          = output_path / "out.dtrace"
    fold_path           = output_path / "out.folded"
    flamegraph_path     = output_path / "flamegraph.svg"
    fold_script         = FLAMEGRAPH_DIR_PATH / "stackcollapse-dtrace.pl"
    flamegraph_script   = FLAMEGRAPH_DIR_PATH / "flamegraph.pl"

    # Launch the target process in the background
    target = subprocess.Popen(
        [str(exec_path), str(RUN_SEED), str(EVAL_TICKS)]
    )

    # Sample with dtrace while the target runs, then tear down
    dtrace_cmd = [
        "sudo", "dtrace",
        "-x", "ustackframes=100",
        "-n", (
            f"profile-99 /pid == {target.pid}/ "
            "{ @[ustack()] = count(); } "
            f"tick-{EVAL_TICKS}ms {{ exit(0); }}"
        ),
        "-o", str(dtrace_out),
    ]
    dtrace_proc = subprocess.Popen(dtrace_cmd)
    # wait for exec to finish
    target.wait()
    # wait for dtrace to exit
    dtrace_proc.wait()

    if dtrace_proc.returncode != 0:
        raise RuntimeError("dtrace exited with a non-zero status.")

    # Collapse stacks
    result = run_process(["perl", fold_script, dtrace_out],
                  capture_output=True, text=True)
    fold_path.write_text(result.stdout)

    # Render flamegraph
    result = run_process(["perl", flamegraph_script, fold_path],
                  capture_output=True, text=True)
    flamegraph_path.write_text(result.stdout)

    print(f"Flamegraph written to: {flamegraph_path}")

def profile_program(exec_path: str, output_path: str) -> None:
    """Benchmark an executable and write a flamegraph SVG

    Args:
        exec_path:   Path to the compiled executable.
        output_path: Directory in which to store profiling artefacts.
    """
    exec_path   = Path(exec_path)
    output_path = Path(output_path)

    if not exec_path.exists():
        raise FileNotFoundError(f"Executable not found: {exec_path}")

    output_path.mkdir(parents=True, exist_ok=True)

    os_type = current_os()
    print(f"Detected platform: {os_type}")

    if os_type == "linux":
        profile_linux(exec_path, output_path)
    else:
        profile_macos(exec_path, output_path)