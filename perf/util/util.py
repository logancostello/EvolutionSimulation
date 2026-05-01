#perf/util/util.py
"""
This file provides various util helpers.
"""

import subprocess
import platform
import webbrowser

from pathlib import Path

def run_process(cmd: list, **kwargs) -> subprocess.CompletedProcess:
    try:
        return subprocess.run(cmd, check=True, **kwargs)
    except subprocess.CalledProcessError as e:
        if e.stderr:
            print(e.stderr)
        if e.stdout:
            print(e.stdout)
        raise

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


def open_in_browser(path: Path) -> None:
    """Open a file in a real browser, falling back to webbrowser.open()."""
    abs_uri = path.resolve().as_uri()
    candidates = {
        "linux": ("firefox", "google-chrome", "chromium", "chromium-browser"),
        "darwin": ("firefox", "google-chrome", "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"),
    }
    os_type = current_os()
    for browser in candidates.get(os_type, ()):
        try:
            subprocess.Popen([browser, abs_uri],
                             stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            return
        except FileNotFoundError:
            continue
    webbrowser.open(abs_uri)

