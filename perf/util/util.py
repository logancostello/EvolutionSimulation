#perf/util/util.py
"""
This file provides various util helpers.
"""

import subprocess
import platform

def run_process(cmd: list, **kwargs) -> subprocess.CompletedProcess:
    """Run a command
    Args:
        None
    Returns:
        None
    """
    return subprocess.run(cmd, check=True, **kwargs)


def current_os() -> str:
    """ Checks current OS 
    Args:
        None
    Returns:
        str: Either raises error or returns string describing OS   
    """
    system = platform.system()
    if system == "Linux":
        return "linux"
    if system == "Darwin":
        return "macos"
    raise OSError(f"Unsupported platform: {system!r}. Only Linux and macOS are supported.")
