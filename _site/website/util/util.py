#site/util/util.py
"""
This file provides various util helpers.
"""

import subprocess

def run_process(cmd: list, **kwargs) -> subprocess.CompletedProcess:
    try:
        return subprocess.run(cmd, check=True, **kwargs)
    except subprocess.CalledProcessError as e:
        if e.stderr:
            print(e.stderr)
        if e.stdout:
            print(e.stdout)
        raise

