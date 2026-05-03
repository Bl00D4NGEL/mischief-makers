#!/usr/bin/env python3

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MAP_PATH = ROOT / "build" / "mischiefmakers.map"
DEFAULT_OUTPUT_PATH = ROOT / "versions" / "us1" / "us1_report.json"
README_PATH = ROOT / "README.md"
MODULE_MAIN = ".main"
MODULE_OVERLAY_PREFIX = ".overlay_"


def resolve_path(path: str) -> Path:
    resolved_path = Path(path)
    if resolved_path.is_absolute():
        return resolved_path
    return ROOT / resolved_path


def format_code_size(byte_count: int) -> str:
    if byte_count < 1000:
        return f"{byte_count} B"
    if byte_count < 1000 * 1000:
        return f"{byte_count / 1000.0:.2f} kB"
    return f"{byte_count / 1000.0 / 1000.0:.2f} MB"


def measure_int(measures: dict, key: str) -> int:
    value = measures.get(key, 0)
    if value is None:
        return 0
    return int(value)


def is_matched_function(function: dict) -> bool:
    return float(function.get("fuzzy_match_percent", 0.0)) == 100.0


def update_function_measures(
    measures: dict,
    total_functions: int,
    matched_functions: int,
) -> None:
    measures["total_functions"] = total_functions
    measures["matched_functions"] = matched_functions
    if total_functions != 0:
        measures["matched_functions_percent"] = (
            matched_functions / total_functions * 100.0
        )
    else:
        measures["matched_functions_percent"] = 0.0


def empty_measures() -> dict:
    return {
        "total_code": "0",
        "matched_code": "0",
        "matched_code_percent": 0.0,
        "total_functions": 0,
        "matched_functions": 0,
        "matched_functions_percent": 0.0,
        "total_units": 0,
    }


def merge_measures(destination: dict, source: dict) -> None:
    destination["total_code"] = str(
        measure_int(destination, "total_code") + measure_int(source, "total_code")
    )
    destination["matched_code"] = str(
        measure_int(destination, "matched_code") + measure_int(source, "matched_code")
    )
    destination["total_functions"] = measure_int(
        destination,
        "total_functions",
    ) + measure_int(source, "total_functions")
    destination["matched_functions"] = measure_int(
        destination,
        "matched_functions",
    ) + measure_int(source, "matched_functions")
    destination["total_units"] = measure_int(destination, "total_units") + measure_int(
        source,
        "total_units",
    )


def finalize_measures(measures: dict) -> None:
    total_code = measure_int(measures, "total_code")
    matched_code = measure_int(measures, "matched_code")
    total_functions = measure_int(measures, "total_functions")
    matched_functions = measure_int(measures, "matched_functions")

    if total_code != 0:
        measures["matched_code_percent"] = matched_code / total_code * 100.0
    else:
        measures["matched_code_percent"] = 0.0

    update_function_measures(measures, total_functions, matched_functions)


def category_from_measures(category_id: str, name: str, measures: dict) -> dict:
    return {
        "id": category_id,
        "name": name,
        "measures": dict(measures),
    }


def calculate_module_measures(report: dict) -> tuple[dict, dict]:
    main_measures = empty_measures()
    overlay_measures = empty_measures()

    for unit in report.get("units", []):
        module_name = unit.get("metadata", {}).get("module_name", "")
        measures = unit.get("measures", {})

        if module_name == MODULE_MAIN:
            merge_measures(main_measures, measures)
        elif module_name.startswith(MODULE_OVERLAY_PREFIX):
            merge_measures(overlay_measures, measures)

    finalize_measures(main_measures)
    finalize_measures(overlay_measures)
    return main_measures, overlay_measures


def set_summary_categories(
    report: dict,
    main_measures: dict,
    overlay_measures: dict,
) -> None:
    summary_category_ids = {"total_matching", "main_code_segment", "overlays"}
    categories = [
        category
        for category in report.get("categories", [])
        if category.get("id") not in summary_category_ids
    ]

    categories.insert(
        0,
        category_from_measures("overlays", "Overlays", overlay_measures),
    )
    categories.insert(
        0,
        category_from_measures(
            "main_code_segment",
            "Main Code Segment",
            main_measures,
        ),
    )
    categories.insert(
        0,
        category_from_measures(
            "total_matching",
            "Total Matching",
            report["measures"],
        ),
    )
    report["categories"] = categories


def filter_local_label_functions(output_path: Path) -> None:
    with output_path.open("r", encoding="utf-8") as report_file:
        report = json.load(report_file)

    category_functions: dict[str, list[int]] = {}
    total_functions = 0
    matched_functions = 0

    for unit in report.get("units", []):
        functions = [
            function
            for function in unit.get("functions", [])
            if not str(function.get("name", "")).startswith(".L")
        ]
        unit["functions"] = functions

        unit_total_functions = len(functions)
        unit_matched_functions = sum(
            1 for function in functions if is_matched_function(function)
        )
        total_functions += unit_total_functions
        matched_functions += unit_matched_functions

        update_function_measures(
            unit["measures"],
            unit_total_functions,
            unit_matched_functions,
        )

        for category in unit.get("metadata", {}).get("progress_categories", []) or []:
            if category not in category_functions:
                category_functions[category] = [0, 0]
            category_functions[category][0] += unit_total_functions
            category_functions[category][1] += unit_matched_functions

    for category in report.get("categories", []):
        category_total_functions, category_matched_functions = category_functions.get(
            category.get("id", ""),
            [0, 0],
        )
        update_function_measures(
            category["measures"],
            category_total_functions,
            category_matched_functions,
        )

    update_function_measures(report["measures"], total_functions, matched_functions)
    main_measures, overlay_measures = calculate_module_measures(report)
    set_summary_categories(report, main_measures, overlay_measures)

    with output_path.open("w", encoding="utf-8") as report_file:
        json.dump(report, report_file, indent=2)
        report_file.write("\n")


def run_mapfile_parser(map_path: Path, output_path: Path, quiet: bool) -> int:
    if not map_path.exists():
        print(f"progress: map file not found: {map_path}", file=sys.stderr)
        print(
            "progress: run ninja or configure.py --split --build first",
            file=sys.stderr,
        )
        return 1

    output_path.parent.mkdir(parents=True, exist_ok=True)

    command = [
        sys.executable,
        "-m",
        "mapfile_parser",
        "objdiff_report",
        str(map_path),
        str(output_path),
        "-a",
        "asm",
        "-n",
        "asm/nonmatchings",
        "-t",
        "build/src/",
        "-t",
        "build/asm/",
        "-t",
        "build/",
    ]

    if quiet:
        command.append("--quiet")

    returncode = subprocess.run(command, cwd=ROOT).returncode
    if returncode != 0:
        return returncode

    filter_local_label_functions(output_path)
    return 0


def print_summary(output_path: Path) -> None:
    with output_path.open("r", encoding="utf-8") as report_file:
        report = json.load(report_file)

    measures = report["measures"]
    total_code = measure_int(measures, "total_code")
    matched_code = measure_int(measures, "matched_code")
    total_functions = measure_int(measures, "total_functions")
    matched_functions = measure_int(measures, "matched_functions")
    matched_code_percent = float(measures.get("matched_code_percent", 0.0))
    matched_functions_percent = float(measures.get("matched_functions_percent", 0.0))
    main_measures, overlay_measures = calculate_module_measures(report)

    print(
        "progress: "
        f"{matched_code:,}/{total_code:,} code bytes "
        f"({matched_code_percent:.2f}%), "
        f"{matched_functions:,}/{total_functions:,} functions "
        f"({matched_functions_percent:.2f}%)"
    )
    print(
        "progress: "
        f"{format_code_size(matched_code)} matched / "
        f"{format_code_size(total_code)} total"
    )
    print(
        "progress: "
        f"main {float(main_measures['matched_code_percent']):.2f}%, "
        f"overlays {float(overlay_measures['matched_code_percent']):.2f}%"
    )
    try:
        display_path = output_path.relative_to(ROOT)
    except ValueError:
        display_path = output_path
    print(f"progress: wrote {display_path}")


def update_readme_badges(output_path: Path) -> int:
    with output_path.open("r", encoding="utf-8") as report_file:
        report = json.load(report_file)

    main_measures, overlay_measures = calculate_module_measures(report)
    badge_values = {
        "Total%20Matching": float(report["measures"].get("matched_code_percent", 0.0)),
        "Main%20Code%20Segment": float(
            main_measures.get("matched_code_percent", 0.0)
        ),
        "Overlays": float(overlay_measures.get("matched_code_percent", 0.0)),
    }

    text = README_PATH.read_text(encoding="utf-8")
    replacements = [
        (
            r"(img\.shields\.io/badge/Total%20Matching-)[^-]+(-brightgreen\.svg)",
            rf"\g<1>{badge_values['Total%20Matching']:.2f}%25\2",
        ),
        (
            r"(img\.shields\.io/badge/Main%20Code%20Segment-)[^-]+(-yellow\.svg)",
            rf"\g<1>{badge_values['Main%20Code%20Segment']:.2f}%25\2",
        ),
        (
            r"(img\.shields\.io/badge/Overlays-)[^-]+(-orange\.svg)",
            rf"\g<1>{badge_values['Overlays']:.2f}%25\2",
        ),
    ]

    new_text = text
    for pattern, replacement in replacements:
        new_text, count = re.subn(pattern, replacement, new_text, count=1)
        if count != 1:
            print(
                f"progress: README badge pattern not found: {pattern}",
                file=sys.stderr,
            )
            return 1

    if new_text != text:
        README_PATH.write_text(new_text, encoding="utf-8")
        print(f"progress: updated {README_PATH.relative_to(ROOT)} badges")
    else:
        print(f"progress: {README_PATH.relative_to(ROOT)} badges already current")

    return 0


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Generate a decomp.dev progress report."
    )
    parser.add_argument(
        "-m",
        "--map",
        default=str(DEFAULT_MAP_PATH),
        help="Path to the linker map file.",
    )
    parser.add_argument(
        "-o",
        "--output",
        default=str(DEFAULT_OUTPUT_PATH),
        help="Path to write the Objdiff report JSON.",
    )
    parser.add_argument(
        "--quiet",
        action="store_true",
        help="Suppress mapfile_parser's detailed summary.",
    )
    parser.add_argument(
        "--update-readme",
        action="store_true",
        help="Update README badge percentages from the generated report.",
    )
    args = parser.parse_args()

    map_path = resolve_path(args.map)
    output_path = resolve_path(args.output)

    returncode = run_mapfile_parser(map_path, output_path, args.quiet)
    if returncode != 0:
        return returncode

    print_summary(output_path)
    if args.update_readme:
        return update_readme_badges(output_path)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
