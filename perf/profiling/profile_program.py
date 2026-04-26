# /perf/profiling/profile_program.py

"""
Given the executable path, this program will generate flamegraphs and profile it
"""

import subprocess

from pathlib import Path

FLAMEGRAPH_DIR_PATH = Path("./perf/flamegraph")

RUN_SEED = 9282024
EVAL_TICKS = 1_000

def profile_program(exec_path: str, output_path: str) -> None:
    """ Benchmark performance and persist to artifact
    Args:
        exec_path (str): Path to compiled executable
        output_path (str): Store flamegraph SVG
    Returns:
        None, writes flamegraph
    """

    exec_path = Path(exec_path)
    output_path = Path(output_path)

    data_path = output_path / "perf.data"
    out_path = output_path / "out.perf"
    fold_path = output_path / "out.folded"
    flamegraph_path = output_path / "flamegraph.svg"

    fold_script_path = FLAMEGRAPH_DIR_PATH / "stackcollapse-perf.pl"
    flamegraph_script_path = FLAMEGRAPH_DIR_PATH / "flamegraph.pl"

    # Generate raw binary sample data
    subprocess.run(["sudo", "perf", "record", "-g", "-o", data_path, exec_path, f"{RUN_SEED}", f"{EVAL_TICKS}"])

    # Convert into human readable text
    result = subprocess.run(["sudo", "perf", "script", "-i", data_path], capture_output = True, text = True)
    with open(out_path, "w") as f:
        f.write(result.stdout)

    # Fold stacks
    result = subprocess.run(["sudo", "perl", fold_script_path, out_path], capture_output = True, text = True)
    with open(fold_path, "w") as f:
        f.write(result.stdout)

    # Generate flamegraph
    result = subprocess.run(["sudo", "perl", flamegraph_script_path, fold_path], capture_output = True, text = True)
    with open(flamegraph_path, "w") as f:
        f.write(result.stdout)
    
    return