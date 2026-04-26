"""
Given an executable path, generates flamegraph SVGs
"""
import os
import subprocess
import sys
from pathlib import Path

from ..util.util import run_process, current_os
from ..util.read_config import PathConfig, BenchmarkConfig


# Profilers
def profile_linux(
    exec_path:        Path,
    benchmark_config: BenchmarkConfig,
    flamegraph_dir:   Path,
    output_path:      Path,
) -> None:
    """Profile an executable on Linux using perf and generate a flamegraph SVG.

    Args:
        exec_path (Path):                   Path to the compiled executable.
        benchmark_config (BenchmarkConfig): Config containing benchmarking info.
        flamegraph_dir (Path):              Path to flamegraph executables.
        output_path (Path):                 Directory in which to store profiling artefacts.
    """
    data_path         = output_path / "perf.data"
    out_path          = output_path / "out.perf"
    fold_path         = output_path / "out.folded"
    flamegraph_path   = output_path / "flamegraph.svg"
    fold_script       = flamegraph_dir / "stackcollapse-perf.pl"
    flamegraph_script = flamegraph_dir / "flamegraph.pl"

    # Sample
    run_process(["sudo", "perf", "record", "-g", "-o", data_path,
                 exec_path, benchmark_config.run_seed, benchmark_config.eval_ticks])

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


def profile_macos(
    exec_path:        Path,
    benchmark_config: BenchmarkConfig,
    flamegraph_dir:   Path,
    output_path:      Path,
) -> None:
    """Profile an executable on macOS using dtrace and generate a flamegraph SVG.

    Args:
        exec_path (Path):                   Path to the compiled executable.
        benchmark_config (BenchmarkConfig): Config containing benchmarking info.
        flamegraph_dir (Path):              Path to flamegraph executables.
        output_path (Path):                 Directory in which to store profiling artefacts.
    """
    dtrace_out        = output_path / "out.dtrace"
    fold_path         = output_path / "out.folded"
    flamegraph_path   = output_path / "flamegraph.svg"
    fold_script       = flamegraph_dir / "stackcollapse-dtrace.pl"
    flamegraph_script = flamegraph_dir / "flamegraph.pl"

    # Launch the target process in the background
    target = subprocess.Popen(
        [exec_path, benchmark_config.run_seed, benchmark_config.eval_ticks]
    )

    # Sample with dtrace while the target runs, then tear down
    dtrace_cmd = [
        "sudo", "dtrace",
        "-x", "ustackframes=100",
        "-n", (
            f"profile-99 /pid == {target.pid}/ "
            "{ @[ustack()] = count(); } "
            f"tick-{benchmark_config.eval_ticks}ms {{ exit(0); }}"
        ),
        "-o", dtrace_out,
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


def profile_program(
    exec_path:        Path,
    benchmark_config: BenchmarkConfig,
    flamegraph_dir:   Path,
    output_path:      Path,
) -> None:
    """Benchmark an executable and write a flamegraph SVG

    Args:
        exec_path (Path):                   Path to the compiled executable.
        benchmark_config (BenchmarkConfig): Config containing benchmarking info.
        flamegraph_dir (Path):              Path to flamegraph executables.
        output_path (Path):                 Directory in which to store profiling artefacts.
    """
    if not exec_path.exists():
        raise FileNotFoundError(f"Executable not found: {exec_path}")

    output_path.mkdir(parents=True, exist_ok=True)

    os_type = current_os()
    print(f"Detected platform: {os_type}")

    if os_type == "linux":
        profile_linux(exec_path, benchmark_config, flamegraph_dir, output_path)
    else:
        profile_macos(exec_path, benchmark_config, flamegraph_dir, output_path)