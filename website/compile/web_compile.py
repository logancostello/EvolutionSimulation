# /site/compile/web_compile.py

"""
This file contains command line commands to compile evolution sim to WASM.
"""

import subprocess
import shutil

from pathlib import Path

from ..util.util import run_process

def web_compile():
    """ WASM compilation runner
    Args:
        None
    Returns:
        None, creates folder ../web-build
    """

    # Build EMCMake folder
    build_dir = ["emcmake", "cmake", "-S", ".", "-B", "web-build"]
    build_dir_result = run_process(build_dir)
    
    if build_dir_result.returncode != 0:
            raise RuntimeError(f"EMCMake folder build failed:\n{build_dir_result.stderr}")
    
    # Compile Sim to WASM
    compile_sim = ["cmake", "--build", "./web-build"]
    compile_sim_result = run_process(compile_sim)
    
    if compile_sim_result.returncode != 0:
            raise RuntimeError(f"EMCMake folder build failed:\n{compile_sim_result.stderr}")
    
    # Transfer scripts
    compile_dir = Path("./web-build/bin/")
    destination_dir = Path("./website/jekyll_site/scripts/")
    for item in compile_dir.iterdir():
        print(f"Move {str(item)}, {destination_dir}, {item.name}")
        shutil.move(str(item), destination_dir / item.name)
    return