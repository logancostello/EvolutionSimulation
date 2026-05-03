"""
Given an executable path, generates flamegraph SVGs
"""
import os
import subprocess
import sys
import webbrowser
from pathlib import Path

from ..util.util import run_process, current_os, open_in_browser
from ..util.read_config import PathConfig, BenchmarkConfig

# Profilers
def profile_linux(
    exec_path: Path,
    benchmark_config: BenchmarkConfig,
    flamegraph_dir: Path,
    output_path: Path,
    current_time: str
) -> None:
    """Profile an executable on Linux using perf and generate a flamegraph SVG.

    Args:
        exec_path (Path): Path to the compiled executable.
        benchmark_config (BenchmarkConfig): Config containing benchmarking info.
        flamegraph_dir (Path): Path to flamegraph executables.
        output_path (Path): Directory in which to store profiling artefacts.
    """
    data_path = output_path / "perf.data"
    out_path = output_path / "out.perf"
    fold_path = output_path / "out.folded"
    flamegraph_path = output_path / "flamegraphs" / f"{current_time}.svg"
    top_fn_path = output_path / "top_functions.txt"
    fold_script = flamegraph_dir / "stackcollapse-perf.pl"
    flamegraph_script = flamegraph_dir / "flamegraph.pl"

    # Sample
    run_process(["sudo", "perf", "record", "-g", "-o", data_path,
                 exec_path, benchmark_config.run_seed, benchmark_config.eval_ticks])

    # Top 20 hottest functions (flat, no children)
    result = run_process(
        ["sudo", "perf", "report", "-i", data_path,
         "--stdio", "--no-children", "--sort=symbol"],
        capture_output=True, text=True,
    )
    top_lines = [l for l in result.stdout.splitlines() if not l.startswith("#")][:25]
    top_text  = "\n".join(top_lines)
    top_fn_path.write_text(top_text)

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
    open_in_browser(flamegraph_path)


def profile_macos(
    exec_path: Path,
    benchmark_config: BenchmarkConfig,
    flamegraph_dir: Path,
    output_path: Path,
    current_time: str
) -> None:
    """Profile an executable on MacOS using perf and generate a flamegraph SVG.

    Args:
        exec_path (Path): Path to the compiled executable.
        benchmark_config (BenchmarkConfig): Config containing benchmarking info.
        flamegraph_dir (Path): Path to flamegraph executables.
        output_path (Path): Directory in which to store profiling artefacts.
    """

    trace_path = output_path / "capture.trace"
    flamegraph_path   = output_path / "flamegraphs" / f"{current_time}.svg"

    # Remove old trace if it exists (xctrace won't overwrite)
    if trace_path.exists():
        import shutil
        shutil.rmtree(trace_path)

    # Record with Time Profiler
    run_process([
        "xctrace", "record",
        "--template", "Time Profiler",
        "--output", trace_path,
        "--launch", "--",
        exec_path,
        str(benchmark_config.run_seed),
        str(benchmark_config.eval_ticks),
    ], capture_output=True, text=True)

    # Export the trace to XML
    xml_path = output_path / "export.xml"
    run_process([
        "xctrace", "export",
        "--input", trace_path,
        "--xpath", "/trace-toc/run/data/table[@schema=\"time-profile\"]",
        "--output", xml_path,
    ], capture_output=True, text=True)

    # Convert XML to folded stacks for flamegraph
    fold_path = output_path / "out.folded"
    _xctrace_xml_to_folded(xml_path, fold_path)

    # Render flamegraph
    flamegraph_script = flamegraph_dir / "flamegraph.pl"
    result = run_process(["perl", flamegraph_script, fold_path],
                         capture_output=True, text=True)
    flamegraph_path.write_text(result.stdout)

    print(f"Flamegraph written to: {flamegraph_path}")
    open_in_browser(flamegraph_path)


def _xctrace_xml_to_folded(xml_path: Path, fold_path: Path) -> None:
    import xml.etree.ElementTree as ET

    tree = ET.parse(xml_path)
    root = tree.getroot()

    # Build a lookup of id -> name for all frames
    frame_names: dict[str, str] = {}
    for elem in root.iter():
        elem_id = elem.get("id")
        if elem_id and elem.tag == "frame":
            name = elem.get("name")
            if name:
                frame_names[elem_id] = name

    lines = []
    for row in root.iter("row"):
        backtrace = row.find("backtrace")
        if backtrace is None:
            # Check for backtrace ref
            bt_elem = row.find("backtrace[@ref]")
            if bt_elem is None:
                continue
            # Find the referenced backtrace by id elsewhere in doc
            ref_id = bt_elem.get("ref")
            backtrace = root.find(f".//backtrace[@id='{ref_id}']")
        if backtrace is None:
            continue

        frames = []
        for frame in backtrace.iter("frame"):
            frame_id = frame.get("id")
            ref_id = frame.get("ref")
            if frame_id and frame_id in frame_names:
                frames.append(frame_names[frame_id])
            elif ref_id and ref_id in frame_names:
                frames.append(frame_names[ref_id])

        if frames:
            lines.append(";".join(reversed(frames)) + " 1")

    fold_path.write_text("\n".join(lines))


def profile_program(
    exec_path: Path,
    benchmark_config: BenchmarkConfig,
    flamegraph_dir: Path,
    output_path: Path,
    current_time: str
) -> None:
    """Benchmark an executable and write a flamegraph SVG

    Args:
        exec_path (Path): Path to the compiled executable.
        benchmark_config (BenchmarkConfig): Config containing benchmarking info.
        flamegraph_dir (Path): Path to flamegraph executables.
        output_path (Path): Directory in which to store profiling artifacts.
    """
    if not exec_path.exists():
        raise FileNotFoundError(f"Executable not found: {exec_path}")

    os_type = current_os()
    print(f"Detected platform: {os_type}")

    flamegraph_path = output_path / "flamegraphs"
    flamegraph_path.mkdir(parents=True, exist_ok=True)

    if os_type == "linux":
        profile_linux(
            exec_path,
            benchmark_config,
            flamegraph_dir,
            output_path,
            current_time
        )
    else:
        profile_macos(
            exec_path,
            benchmark_config,
            flamegraph_dir,
            output_path,
            current_time
        )
    
    # Folder cleanup
    os.remove(output_path / "out.folded")
    os.remove(output_path / "out.perf")
    os.remove(output_path / "top_functions.txt")
    os.remove(output_path / "perf.data")

    return