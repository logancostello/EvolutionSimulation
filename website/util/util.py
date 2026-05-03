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

def dt_str_to_slug(dt_str: str) -> str:
    """ Converts a datetime string to a URL-safe slug
    Args:
        dt_str: Datetime string e.g. '2026-05-02-17:27:04'
    Returns:
        Slug string e.g. '2026-05-02-17-27-04'
    """
    return dt_str.replace(":", "-")


def slug_to_url(slug: str) -> str:
    """ Converts a slug to a flamegraph post URL
    Args:
        slug: Slug string e.g. '2026-05-02-17-27-04'
    Returns:
        URL string e.g. '/perf/flamegraphs/2026-05-02-17-27-04/'
    """
    return f"/perf/flamegraphs/{slug}/"
