# /perf/perf_main.py

"""
This file initiates the benchmark harness.
"""

import sys
import shutil

from .util.compile_program import compile_program
from .benchmark.benchmark import benchmark_performance
from .profiling.profile_program import profile_program

CMAKE_PATH = "./CMakeLists.txt"
COMPILE_PATH = "./build"
EXEC_PATH = "./build/bin/main"
OUTPUT_PATH = "./profile_graphics"

def main():
    # Delete artifact folders
    shutil.rmtree(COMPILE_PATH, ignore_errors=True)
    shutil.rmtree(OUTPUT_PATH, ignore_errors=True)

    print("BEGINNING COMPILING PROGRAM")
    # Compile executable
    compile_program(CMAKE_PATH, COMPILE_PATH)

    # Perform benchmarking
    print("PERFORMING BENCHMARKING")
    benchmark_performance(EXEC_PATH)

    # Perform profiling
    profile_program(EXEC_PATH, OUTPUT_PATH)

    return

if __name__ == "__main__":
    sys.exit(main())