"""
Given a config path, parses it for future use.
"""
import json
from dataclasses import dataclass
from pathlib import Path


@dataclass
class PathConfig:
    cmake_path: Path
    compile_path: Path
    exec_path: Path
    output_path: Path
    flamegraph_dir: Path
    profile_dir: Path


@dataclass
class BenchmarkConfig:
    run_seed: int
    eval_ticks: int
    warmups: int
    eval_runs: int


class Config:
    def __init__(self, config_path: str | Path):
        config_path = Path(config_path)

        if not config_path.exists():
            raise FileNotFoundError(f"Config file not found: {config_path}")

        with open(config_path, "r") as f:
            data = json.load(f)

        raw_paths = data["path"]
        self.paths = PathConfig(
            cmake_path = Path(raw_paths["CMAKE_PATH"]),
            compile_path = Path(raw_paths["COMPILE_PATH"]),
            exec_path = Path(raw_paths["EXEC_PATH"]),
            output_path = Path(raw_paths["OUTPUT_PATH"]),
            flamegraph_dir= Path(raw_paths["FLAMEGRAPH_DIR"]),
            profile_dir = Path(raw_paths["PROFILE_DIR"]),
        )

        raw_bench = data["benchmark"]
        self.benchmark = BenchmarkConfig(
            run_seed = str(raw_bench["RUN_SEED"]),
            eval_ticks= str(raw_bench["EVAL_TICKS"]),
            warmups = str(raw_bench["WARMUPS"]),
            eval_runs = str(raw_bench["EVAL_RUNS"]),
        )