# /perf/perf_main.py

"""
This file initiates the benchmark harness.
"""

import sys
import shutil
import argparse

from .benchmark.benchmark import benchmark_performance
from .profiling.profile_program import profile_program
from .util.compile_program import compile_program
from .util.read_config import Config

def main():
    # Intiailize parser
    parser = argparse.ArgumentParser(description = "EvolutionSim Performance Harness")

    # Add Config Arg
    parser.add_argument("--config", help="Path to config file", default="./perf/config/config.json")
    
    args = parser.parse_args()

    # Read in JSON config
    config = Config(args.config)


    print("BEGINNING COMPILING PROGRAM")
    # Compile executable
    compile_program(config.paths.cmake_path, config.paths.compile_path)

    # Perform benchmarking
    print("PERFORMING BENCHMARKING")
    benchmark_performance(config.paths.exec_path, config.benchmark)

    # Perform profiling
    profile_program(config.paths.exec_path, config.benchmark, config.paths.flamegraph_dir, config.paths.output_path)

    return

if __name__ == "__main__":
    sys.exit(main())