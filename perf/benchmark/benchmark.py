# /perf/benchmark/benchmark.py

"""
Given the executable path, this script will benchmark performance.
"""

import re
import subprocess

from pathlib import Path

RUN_SEED = 9282024
EVAL_TICKS = 1_000

WARMUPS = 2

EVAL_RUNS = 10

def benchmark_performance(exec_path: str) -> None:
    """ Benchmark performance and persist to artifact
    Args:
        exec_path (str): Path to compiled executable
    Returns:
        None, TODO: Writes to supabase or artifact
    """

    exec_path = Path(exec_path)

    benchmark_results = []

    # Run warmups
    for warmup in range(WARMUPS):
        warmup_result = subprocess.run([exec_path, f"{RUN_SEED}", f"{EVAL_TICKS}"])
    
    # Run evals
    for eval_run in range(EVAL_RUNS):
        print(f"Run: {eval_run}")
        eval_result = subprocess.run([exec_path, f"{RUN_SEED}", f"{EVAL_TICKS}"], capture_output = True, text = True)
        match = re.search(r"Avg/tick: (\d+\.?\d*)ms", eval_result.stdout)
        benchmark_results.append(match.group(1))
    
    return