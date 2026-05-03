#!/usr/bin/env python3
"""
第 3 题排序实验计时脚本。

功能：
1. 自动生成随机、正序、逆序测试数据。
2. 自动编译第 3 题 0~7 的排序 cpp 文件。
3. 并行运行“算法 × 数据集”任务。
4. 每个任务开始和结束都会输出一行，方便观察卡在哪个算法/数据集。
5. 将结果写入 CSV，方便放进实验报告。
"""

from __future__ import annotations

import argparse
import csv
import os
import random
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass
from pathlib import Path
from threading import Lock


ROOT_DIR = Path(__file__).resolve().parent
WORK_DIR = ROOT_DIR / ".sort-benchmark"
INPUT_DIR = WORK_DIR / "inputs"
BUILD_DIR = WORK_DIR / "bin"
RESULT_PATH = WORK_DIR / "results.csv"


@dataclass(frozen=True)
class Algorithm:
    name: str
    source: str
    executable: str


@dataclass(frozen=True)
class Dataset:
    name: str
    size: int
    kind: str


ALGORITHMS = [
    Algorithm("0_std_sort", "3. 排序_0_std::sort copy.cpp", "sort_0_std_sort"),
    Algorithm("1_quick_sort", "3. 排序_1_快速排序.cpp", "sort_1_quick_sort"),
    Algorithm("2_merge_sort", "3. 排序_2_归并排序.cpp", "sort_2_merge_sort"),
    Algorithm("3_heap_sort", "3. 排序_3_堆排序.cpp", "sort_3_heap_sort"),
    Algorithm("4_selection_sort", "3. 排序_4_选择排序.cpp", "sort_4_selection_sort"),
    Algorithm("5_bubble_sort", "3. 排序_5_冒泡排序.cpp", "sort_5_bubble_sort"),
    Algorithm("6_insertion_sort", "3. 排序_6_直接插入排序.cpp", "sort_6_insertion_sort"),
    Algorithm("7_shell_sort", "3. 排序_7_希尔排序.cpp", "sort_7_shell_sort"),
]


DATASETS = [
    Dataset("random_10", 10, "random"),
    Dataset("random_100", 100, "random"),
    Dataset("random_1k", 1_000, "random"),
    Dataset("random_10k", 10_000, "random"),
    Dataset("random_100k", 100_000, "random"),
    Dataset("random_1m", 1_000_000, "random"),
    Dataset("sorted_10k", 10_000, "sorted"),
    Dataset("reversed_10k", 10_000, "reversed"),
]


print_lock = Lock()


def log(message: str) -> None:
    with print_lock:
        print(message, flush=True)


def format_seconds(seconds: float | None) -> str:
    if seconds is None:
        return ""
    return f"{seconds:.6f}"


def prepare_dirs() -> None:
    INPUT_DIR.mkdir(parents=True, exist_ok=True)
    BUILD_DIR.mkdir(parents=True, exist_ok=True)


def make_values(dataset: Dataset, seed: int) -> list[int]:
    if dataset.kind == "random":
        rng = random.Random(seed + dataset.size)
        return [rng.randint(0, 1_000_000_000) for _ in range(dataset.size)]
    if dataset.kind == "sorted":
        return list(range(dataset.size))
    if dataset.kind == "reversed":
        return list(range(dataset.size, 0, -1))
    raise ValueError(f"unknown dataset kind: {dataset.kind}")


def write_dataset(dataset: Dataset, seed: int) -> Path:
    input_path = INPUT_DIR / f"{dataset.name}.txt"
    if input_path.exists():
        return input_path

    log(f"GENERATE | {dataset.name:12s} | n={dataset.size}")
    values = make_values(dataset, seed)
    with input_path.open("w", encoding="utf-8") as file:
        file.write(str(dataset.size))
        file.write("\n")
        file.write(" ".join(map(str, values)))
        file.write("\n")
    return input_path


def compile_algorithm(algorithm: Algorithm) -> Path:
    source_path = ROOT_DIR / algorithm.source
    executable_path = BUILD_DIR / algorithm.executable

    if not source_path.exists():
        raise FileNotFoundError(f"找不到源文件：{source_path}")

    command = [
        "g++",
        "-std=c++17",
        "-O2",
        str(source_path),
        "-o",
        str(executable_path),
    ]
    log(f"COMPILE  | {algorithm.name}")
    result = subprocess.run(command, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(
            f"编译失败：{algorithm.name}\n{result.stderr.strip()}"
        )
    return executable_path


def check_executable(executable_path: Path, algorithm: Algorithm) -> None:
    sample_input = "5\n5 4 3 2 1\n"
    result = subprocess.run(
        [str(executable_path)],
        input=sample_input,
        capture_output=True,
        text=True,
    )
    expected = "1 2 3 4 5"
    actual = result.stdout.strip()
    if result.returncode != 0 or actual != expected:
        raise RuntimeError(
            f"自测失败：{algorithm.name}\n"
            f"expected: {expected}\n"
            f"actual:   {actual}\n"
            f"stderr:   {result.stderr.strip()}"
        )


def run_task(
    algorithm: Algorithm,
    executable_path: Path,
    dataset: Dataset,
    input_path: Path,
    timeout: float | None,
) -> dict[str, str]:
    log(f"START    | {dataset.name:12s} | {algorithm.name}")
    start = time.perf_counter()
    status = "OK"
    error = ""

    try:
        with input_path.open("rb") as input_file:
            result = subprocess.run(
                [str(executable_path)],
                stdin=input_file,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.PIPE,
                timeout=timeout,
            )
        if result.returncode != 0:
            status = "ERROR"
            error = result.stderr.decode("utf-8", errors="replace").strip()
    except subprocess.TimeoutExpired:
        status = "TIMEOUT"
        error = f"超过 timeout={timeout} 秒"

    elapsed = time.perf_counter() - start
    log(
        f"{status:<8s} | {dataset.name:12s} | "
        f"{algorithm.name:16s} | {elapsed:.6f}s"
    )

    return {
        "dataset": dataset.name,
        "size": str(dataset.size),
        "kind": dataset.kind,
        "algorithm": algorithm.name,
        "status": status,
        "seconds": format_seconds(elapsed),
        "error": error,
    }


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="并行测试第 3 题不同排序算法的运行时间。"
    )
    parser.add_argument(
        "--workers",
        type=int,
        default=max(1, min(4, os.cpu_count() or 1)),
        help="并行任务数量，默认最多 4 个。",
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=20260503,
        help="随机数据种子。",
    )
    parser.add_argument(
        "--timeout",
        type=float,
        default=None,
        help="单个任务超时时间（秒）。默认不设置超时，会一直跑到结束。",
    )
    parser.add_argument(
        "--datasets",
        nargs="*",
        default=None,
        help="只运行指定数据集，例如 random_10 random_100。默认运行全部。",
    )
    parser.add_argument(
        "--algorithms",
        nargs="*",
        default=None,
        help="只运行指定算法，例如 1_quick_sort 2_merge_sort。默认运行全部。",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    prepare_dirs()

    selected_datasets = DATASETS
    if args.datasets:
        dataset_names = set(args.datasets)
        selected_datasets = [item for item in DATASETS if item.name in dataset_names]

    selected_algorithms = ALGORITHMS
    if args.algorithms:
        algorithm_names = set(args.algorithms)
        selected_algorithms = [
            item for item in ALGORITHMS if item.name in algorithm_names
        ]

    if not selected_datasets:
        raise ValueError("没有匹配到任何数据集。")
    if not selected_algorithms:
        raise ValueError("没有匹配到任何算法。")

    input_paths = {
        dataset.name: write_dataset(dataset, args.seed)
        for dataset in selected_datasets
    }

    executable_paths = {}
    for algorithm in selected_algorithms:
        executable_path = compile_algorithm(algorithm)
        check_executable(executable_path, algorithm)
        executable_paths[algorithm.name] = executable_path

    tasks = []
    with ThreadPoolExecutor(max_workers=args.workers) as executor:
        for dataset in selected_datasets:
            for algorithm in selected_algorithms:
                tasks.append(
                    executor.submit(
                        run_task,
                        algorithm,
                        executable_paths[algorithm.name],
                        dataset,
                        input_paths[dataset.name],
                        args.timeout,
                    )
                )

        results = []
        for future in as_completed(tasks):
            results.append(future.result())

    results.sort(key=lambda row: (row["dataset"], row["algorithm"]))

    with RESULT_PATH.open("w", encoding="utf-8", newline="") as file:
        writer = csv.DictWriter(
            file,
            fieldnames=[
                "dataset",
                "size",
                "kind",
                "algorithm",
                "status",
                "seconds",
                "error",
            ],
        )
        writer.writeheader()
        writer.writerows(results)

    log(f"RESULT   | {RESULT_PATH}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except KeyboardInterrupt:
        print("\n用户中断。", file=sys.stderr)
        raise SystemExit(130)
