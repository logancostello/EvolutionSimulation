# perf/util/compile_program.py

"""
Compiles EvolutionSim To Certain Location.
"""

import subprocess

from pathlib import Path

from ..util.util import run_process

def compile_program(cmake_path: Path, compile_path: Path) -> None:
    """ Compiles EvolutionSim To Certain Location.
    Args:
        cmake_path (Path): Initial path to CMakeLists.txt
        compile_path (Path): Build folder directory for CMake
    """
    # Create build dir for compilation
    print("CREATING BUILD DIR")
    if not compile_path.exists():
        build_dir = run_process(
            [
            "cmake", "-S", ".", "-B", compile_path,
            "-DCMAKE_BUILD_TYPE=RelWithDebInfo",
            "-DCMAKE_CXX_FLAGS=-O2 -g -fno-omit-frame-pointer",
            "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
            ], 
            capture_output=True, text=True)

        if build_dir.returncode != 0:
            raise RuntimeError(f"CMake configure failed:\n{build_dir.stderr}")

    # Compile program
    print("COMPILING PROGRAM")
    compile_result = run_process(
        ["cmake", "--build", compile_path], 
        capture_output=True, text=True)

    if compile_result.returncode != 0:
        raise RuntimeError(f"CMake build failed:\n{compile_result.stderr}")
