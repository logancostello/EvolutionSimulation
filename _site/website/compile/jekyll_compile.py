# /site/compile/jekyll_compile.py

"""
This file contains command line commands to build updated Jekyll _site
"""

import subprocess

from ..util.util import run_process

def jekyll_compile():
    """ Jekyll compilation runner
    Args:
        None
    Returns:
        None, updates jekyll_site/_site
    """

    # Execute Jekyll build command
    jekyll_build = ["jekyll", "build"]
    jekyll_result = run_process(jekyll_build)

    if jekyll_result.returncode != 0:
        raise RuntimeError(f"EMCMake folder build failed:\n{jekyll_result.stderr}")

    return