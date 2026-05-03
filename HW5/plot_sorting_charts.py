#!/usr/bin/env python3
"""
根据 benchmark_sorting.py 生成的 results.csv 绘制排序实验图表。

运行：
    python3 "HW5/plot_sorting_charts.py"

输出：
    HW5/.sort-benchmark/charts/*.png
"""

from __future__ import annotations

import csv
import math
from pathlib import Path

import matplotlib.pyplot as plt


ROOT_DIR = Path(__file__).resolve().parent
RESULT_PATH = ROOT_DIR / ".sort-benchmark" / "results.csv"
CHART_DIR = ROOT_DIR / ".sort-benchmark" / "charts"


ALGORITHM_LABELS = {
    "0_std_sort": "std::sort",
    "1_quick_sort": "Quick",
    "2_merge_sort": "Merge",
    "3_heap_sort": "Heap",
    "4_selection_sort": "Selection",
    "5_bubble_sort": "Bubble",
    "6_insertion_sort": "Insertion",
    "7_shell_sort": "Shell",
}

ALGORITHM_ORDER = list(ALGORITHM_LABELS.keys())

DATASET_ORDER = [
    "random_10",
    "random_100",
    "random_1k",
    "random_10k",
    "random_100k",
    "random_1m",
    "sorted_10k",
    "reversed_10k",
]

FAST_ALGORITHMS = [
    "0_std_sort",
    "1_quick_sort",
    "2_merge_sort",
    "3_heap_sort",
    "7_shell_sort",
]

SLOW_ALGORITHMS = [
    "4_selection_sort",
    "5_bubble_sort",
    "6_insertion_sort",
]


def load_rows() -> list[dict[str, str]]:
    if not RESULT_PATH.exists():
        raise FileNotFoundError(f"找不到结果文件：{RESULT_PATH}")

    with RESULT_PATH.open("r", encoding="utf-8", newline="") as file:
        rows = list(csv.DictReader(file))

    ok_rows = [row for row in rows if row["status"] == "OK" and row["seconds"]]
    for row in ok_rows:
        row["size_value"] = int(row["size"])
        row["seconds_value"] = float(row["seconds"])
    return ok_rows


def index_rows(rows: list[dict[str, str]]) -> dict[tuple[str, str], dict[str, str]]:
    return {(row["dataset"], row["algorithm"]): row for row in rows}


def setup_style() -> None:
    plt.rcParams["figure.dpi"] = 140
    plt.rcParams["savefig.dpi"] = 180
    plt.rcParams["axes.grid"] = True
    plt.rcParams["grid.alpha"] = 0.25
    plt.rcParams["axes.spines.top"] = False
    plt.rcParams["axes.spines.right"] = False


def save_current(name: str) -> None:
    CHART_DIR.mkdir(parents=True, exist_ok=True)
    path = CHART_DIR / name
    plt.tight_layout()
    plt.savefig(path)
    plt.close()
    print(f"生成图表：{path}")


def random_rows_for_algorithm(
    rows: list[dict[str, str]],
    algorithm: str,
) -> list[dict[str, str]]:
    result = [
        row
        for row in rows
        if row["algorithm"] == algorithm and row["kind"] == "random"
    ]
    return sorted(result, key=lambda row: row["size_value"])


def plot_random_growth_all(rows: list[dict[str, str]]) -> None:
    plt.figure(figsize=(10, 6))
    for algorithm in ALGORITHM_ORDER:
        points = random_rows_for_algorithm(rows, algorithm)
        sizes = [row["size_value"] for row in points]
        seconds = [row["seconds_value"] for row in points]
        plt.plot(
            sizes,
            seconds,
            marker="o",
            linewidth=1.8,
            label=ALGORITHM_LABELS[algorithm],
        )

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Input size n (log scale)")
    plt.ylabel("Runtime seconds (log scale)")
    plt.title("Random data: all sorting algorithms")
    plt.legend(ncol=2)
    save_current("01_random_all_algorithms_log_log.png")


def plot_random_growth_fast(rows: list[dict[str, str]]) -> None:
    plt.figure(figsize=(10, 6))
    for algorithm in FAST_ALGORITHMS:
        points = random_rows_for_algorithm(rows, algorithm)
        sizes = [row["size_value"] for row in points]
        seconds = [row["seconds_value"] for row in points]
        plt.plot(
            sizes,
            seconds,
            marker="o",
            linewidth=2,
            label=ALGORITHM_LABELS[algorithm],
        )

    plt.xscale("log")
    plt.xlabel("Input size n (log scale)")
    plt.ylabel("Runtime seconds")
    plt.title("Random data: O(n log n) and Shell sort")
    plt.legend()
    save_current("02_random_fast_algorithms.png")


def plot_random_growth_slow(rows: list[dict[str, str]]) -> None:
    plt.figure(figsize=(10, 6))
    for algorithm in SLOW_ALGORITHMS:
        points = random_rows_for_algorithm(rows, algorithm)
        sizes = [row["size_value"] for row in points]
        seconds = [row["seconds_value"] for row in points]
        plt.plot(
            sizes,
            seconds,
            marker="o",
            linewidth=2,
            label=ALGORITHM_LABELS[algorithm],
        )

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Input size n (log scale)")
    plt.ylabel("Runtime seconds (log scale)")
    plt.title("Random data: O(n^2) algorithms")
    plt.legend()
    save_current("03_random_slow_algorithms_log_log.png")


def plot_random_1m_bar(index: dict[tuple[str, str], dict[str, str]]) -> None:
    dataset = "random_1m"
    labels = [ALGORITHM_LABELS[algorithm] for algorithm in ALGORITHM_ORDER]
    values = [
        index[(dataset, algorithm)]["seconds_value"]
        for algorithm in ALGORITHM_ORDER
    ]

    plt.figure(figsize=(10, 6))
    bars = plt.bar(labels, values)
    plt.yscale("log")
    plt.ylabel("Runtime seconds (log scale)")
    plt.title("Random 1M: runtime comparison")
    plt.xticks(rotation=35, ha="right")

    for bar, value in zip(bars, values):
        plt.text(
            bar.get_x() + bar.get_width() / 2,
            value,
            f"{value:.2f}s",
            ha="center",
            va="bottom",
            fontsize=8,
        )

    save_current("04_random_1m_bar_log.png")


def plot_10k_distribution_bar(index: dict[tuple[str, str], dict[str, str]]) -> None:
    datasets = ["random_10k", "sorted_10k", "reversed_10k"]
    dataset_labels = ["Random 10K", "Sorted 10K", "Reversed 10K"]
    bar_width = 0.1
    x_positions = list(range(len(datasets)))

    plt.figure(figsize=(12, 6))
    for offset, algorithm in enumerate(ALGORITHM_ORDER):
        values = [
            index[(dataset, algorithm)]["seconds_value"]
            for dataset in datasets
        ]
        positions = [
            x + (offset - (len(ALGORITHM_ORDER) - 1) / 2) * bar_width
            for x in x_positions
        ]
        plt.bar(
            positions,
            values,
            width=bar_width,
            label=ALGORITHM_LABELS[algorithm],
        )

    plt.yscale("log")
    plt.xticks(x_positions, dataset_labels)
    plt.ylabel("Runtime seconds (log scale)")
    plt.title("10K data distribution comparison")
    plt.legend(ncol=4, fontsize=8)
    save_current("05_10k_distribution_grouped_bar.png")


def plot_heatmap(index: dict[tuple[str, str], dict[str, str]]) -> None:
    matrix = []
    for algorithm in ALGORITHM_ORDER:
        row_values = []
        for dataset in DATASET_ORDER:
            seconds = index[(dataset, algorithm)]["seconds_value"]
            row_values.append(math.log10(seconds))
        matrix.append(row_values)

    plt.figure(figsize=(11, 6))
    image = plt.imshow(matrix, aspect="auto", cmap="viridis")
    plt.colorbar(image, label="log10(runtime seconds)")
    plt.xticks(
        range(len(DATASET_ORDER)),
        DATASET_ORDER,
        rotation=35,
        ha="right",
    )
    plt.yticks(
        range(len(ALGORITHM_ORDER)),
        [ALGORITHM_LABELS[item] for item in ALGORITHM_ORDER],
    )
    plt.title("Runtime heatmap")

    for y, algorithm in enumerate(ALGORITHM_ORDER):
        for x, dataset in enumerate(DATASET_ORDER):
            seconds = index[(dataset, algorithm)]["seconds_value"]
            text_color = "white" if math.log10(seconds) > 0 else "black"
            plt.text(
                x,
                y,
                f"{seconds:.2g}",
                ha="center",
                va="center",
                color=text_color,
                fontsize=7,
            )

    save_current("06_runtime_heatmap.png")


def plot_speedup_vs_std(index: dict[tuple[str, str], dict[str, str]]) -> None:
    dataset = "random_1m"
    baseline = index[(dataset, "0_std_sort")]["seconds_value"]
    algorithms = [item for item in ALGORITHM_ORDER if item != "0_std_sort"]
    labels = [ALGORITHM_LABELS[item] for item in algorithms]
    ratios = [
        index[(dataset, algorithm)]["seconds_value"] / baseline
        for algorithm in algorithms
    ]

    plt.figure(figsize=(10, 6))
    bars = plt.bar(labels, ratios)
    plt.yscale("log")
    plt.ylabel("Runtime / std::sort (log scale)")
    plt.title("Random 1M: slowdown compared with std::sort")
    plt.xticks(rotation=35, ha="right")

    for bar, ratio in zip(bars, ratios):
        plt.text(
            bar.get_x() + bar.get_width() / 2,
            ratio,
            f"{ratio:.1f}x",
            ha="center",
            va="bottom",
            fontsize=8,
        )

    save_current("07_random_1m_slowdown_vs_std_sort.png")


def main() -> int:
    setup_style()
    rows = load_rows()
    index = index_rows(rows)

    plot_random_growth_all(rows)
    plot_random_growth_fast(rows)
    plot_random_growth_slow(rows)
    plot_random_1m_bar(index)
    plot_10k_distribution_bar(index)
    plot_heatmap(index)
    plot_speedup_vs_std(index)

    print(f"全部图表已输出到：{CHART_DIR}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
