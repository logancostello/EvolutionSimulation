# /perf/benchmark/benchmark.py

"""
Given the executable path, this script will benchmark performance.
"""

import re
import subprocess

from pathlib import Path

from ..util.util import run_process
from ..util.read_config import BenchmarkConfig

def benchmark_performance(
    exec_path: Path, 
    benchmark_config: BenchmarkConfig
    ) -> None:
    """ Benchmark performance and persist to artifact
    Args:
        exec_path (path): Path to compiled executable
        benchmark_config (BenchmarkConfig): Config args for benchmark passed in
    """
    benchmark_results = []

    # Run warmups
    for warmup in range(int(benchmark_config.warmups)):
        warmup_result = run_process(
            [exec_path, benchmark_config.run_seed, benchmark_config.eval_ticks]
            )
    
    # Run evals
    for eval_run in range(int(benchmark_config.eval_runs)):
        print(f"Run: {eval_run}")
        eval_result = run_process(
            [exec_path, benchmark_config.run_seed, benchmark_config.eval_ticks], 
            capture_output = True, text = True)

        match = re.search(r"Avg/tick: (\d+\.?\d*)ms", eval_result.stdout)
        benchmark_results.append(match.group(1))
    
    return