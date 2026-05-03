# /perf/perf_main.py

"""
This file initiates the benchmark harness.
"""

import sys
import shutil
import os
import argparse

from datetime import datetime
from pathlib import Path

from .benchmark.benchmark import benchmark_performance
from .profiling.profile_program import profile_program
from .util.compile_program import compile_program
from .util.read_config import Config

def main():
    # Intiailize parser
    parser = argparse.ArgumentParser(description = "EvolutionSim Performance Harness")

    # Add Config Arg
    parser.add_argument(
        "--config", 
        help="Path to config file", 
        default="./perf/config/config.json"
    )
    
    args = parser.parse_args()

    # Read in JSON config
    config = Config(args.config)

    # Generate output folder
    current_datetime = datetime.today().strftime('%Y-%m-%d-%H:%M:%S')
    output_path = Path(f"./website/jekyll_site/perf")
    output_path.mkdir(parents=True, exist_ok=True)


    print("BEGINNING COMPILING PROGRAM")
    # Compile executable
    compile_program(
        config.paths.cmake_path, 
        config.paths.compile_path
    )

    # Perform benchmarking
    print("PERFORMING BENCHMARKING")
    benchmark_performance(
        config.paths.exec_path, 
        config.benchmark,
        output_path,
        current_datetime
    )

    # Perform profiling
    profile_program(
        config.paths.exec_path,
        config.benchmark,
        config.paths.flamegraph_dir,
        output_path,
        current_datetime
    )

    return

if __name__ == "__main__":
    sys.exit(main())