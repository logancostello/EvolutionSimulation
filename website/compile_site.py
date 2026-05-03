# /site/compile_site.py

"""
This file compiles the sim for web and compiles the latest jekyll site.
"""

import sys

from .compile.web_compile import web_compile
from .compile.jekyll_update import jekyll_update

def main():
    """ Site and WASM compilation runner
    Args:
        None
    Returns:
        None, creates folder ../web-build and copies WASM to ./site/jekyll_site/scripts
    """

    # Build web compilation
    print("Compiling WASM")
    web_compile()

    # Compile Jekyll site
    print("Updating Jekyll Site")
    jekyll_update()
    return

if __name__ == "__main__":
    sys.exit(main())