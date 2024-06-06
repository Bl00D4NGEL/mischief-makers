#!/usr/bin/env python3

import argparse
import git
import os
import subprocess
import sys

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.join(script_dir, "..")
asm_dir = os.path.join(root_dir, "asm")
matchings_dir = os.path.join(asm_dir, "matchings")
data_dir = os.path.join(asm_dir, "data")
src_dir = os.path.join(root_dir, "src")
build_dir = os.path.join(root_dir, "build")
elf_path = os.path.join(build_dir, "mischiefmakers.elf")

def get_func_sizes():
    try:
        result = subprocess.run(["objdump", "-x", elf_path], stdout=subprocess.PIPE)
        nm_lines = result.stdout.decode().split("\n")
    except:
        print(
            f"Error: Could not run objdump on {elf_path} - make sure that the project is built"
        )
        sys.exit(1)

    sizes = {}
    total = 0

    for line in nm_lines:
        if " F " in line:
            components = line.split()
            size = int(components[4], 16)
            name = components[5]
            if not name.startswith(".L") and not name.startswith("D_"):
                total += size
                sizes[name] = size

    return sizes, total

def get_funcs_from_asm(asm_path):
    funcs = set()
    with open(asm_path, 'r') as file:
        for line in file:
            if line.startswith('glabel '):
                func_name = line.split()[1]
                funcs.add(func_name)
    return funcs

def get_all_funcs(asm_root, exclude_dirs=None):
    if exclude_dirs is None:
        exclude_dirs = []
    all_funcs = set()
    for root, dirs, files in os.walk(asm_root):
        dirs[:] = [d for d in dirs if d not in exclude_dirs]
        for file in files:
            if file.endswith('.s'):
                asm_path = os.path.join(root, file)
                funcs = get_funcs_from_asm(asm_path)
                all_funcs.update(funcs)
    return all_funcs

def main(frogress_key=None):
    func_sizes, total_size = get_func_sizes() # get the size of every function, and the total size of all functions within the ELF
    all_funcs = set(func_sizes.keys()) # get a list of every function

    nonmatching_funcs = get_all_funcs(asm_dir, exclude_dirs=['matchings', 'data']) # read every *.s file within `asm`, excluding the trees `asm/matchings` and `asm/data` to get a complete list of nonmatching functions
    matching_funcs = get_all_funcs(matchings_dir) # read every *.s file within `asm/matchings`
    matching_funcs.update(get_all_funcs(src_dir)) # and `src` to get a complete list of matching functions

    # determine total size of matching functions
    matching_size = 0
    for func in matching_funcs:
        if func in func_sizes:
            matching_size += func_sizes[func]

    # determine total size of nonmatching functions
    nonmatching_size = 0
    for func in nonmatching_funcs:
        if func in func_sizes:
            nonmatching_size += func_sizes[func]

    if len(all_funcs) == 0:
        funcs_matching_ratio = 0.0
        matching_ratio = 0.0
    else:
        funcs_matching_ratio = (len(matching_funcs) / len(all_funcs)) * 100
        matching_ratio = (matching_size / total_size) * 100

    if matching_size + nonmatching_size != total_size:
        print(f"Warning: category/total size mismatch!\n")
        print(f"Function sizes: matching {matching_size:X} + nonmatching {nonmatching_size:X} == {(matching_size + nonmatching_size):X}, total {total_size:X}")

    print(
        f"{len(matching_funcs)} matched functions / {len(all_funcs)} total ({funcs_matching_ratio:.2f}%)"
    )
    print(
        f"0x{matching_size:X} matching bytes / 0x{total_size:X} total ({matching_ratio:.2f}%)"
    )

    if frogress_key is not None:
        import requests

        frogress_url = "https://progress.deco.mp/data/mischief-makers/us1/"

        repo = git.Repo(root_dir)

        frogress_data = {
            "api_key": frogress_key,
            "entries": [
                {
                    "timestamp": repo.head.commit.committed_date,
                    "git_hash": repo.head.commit.hexsha,
                    "categories": {
                        "all": {
                            "bytes": matching_size,
                            "bytes/total": total_size,
                            "funcs": len(matching_funcs),
                            "funcs/total": len(all_funcs),
                        }
                    },
                }
            ],
        }
        response = requests.post(frogress_url, json=frogress_data)
        if response.status_code != 201:
            print("Error: Could not push progress data to frogress:" + response.text)
            sys.exit(1)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Calculate the progress of the project"
    )
    parser.add_argument(
        "--frogress",
        help="Push progress data to progress.deco.mp with the provided key",
    )
    args = parser.parse_args()
    main()
