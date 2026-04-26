# perf/util/compile_program.py

"""
Compiles EvolutionSim To Certain Location.
"""

import subprocess

from pathlib import Path

def compile_program(cmake_path: str, compile_path: str) -> str:
    """ Compiles EvolutionSim To Certain Location.
    Args:
        cmake_path (str): Initial path to CMakeLists.txt
        compile_path (str): Build folder directory for CMake
    Returns:
        exec_path (str): Resulting path to executable
    """
    
    cmake_path = Path(cmake_path)
    compile_path = Path(compile_path)

    # Create build dir for compilation
    print("CREATING BUILD DIR")
    build_dir = subprocess.run(["cmake", "-S", ".", "-B", f"{compile_path}"], capture_output = True, text = True)

    # Print error message
    if build_dir.returncode != 0:
        print(build_dir.stderr)
        return

    # Compile program
    print("COMPILING PROGRAM")
    compile_program = subprocess.run(["cmake", "--build", "build"], capture_output = True, text = True)
    if compile_program.returncode != 0:
        print(compile_program.stderr)
        return

    return