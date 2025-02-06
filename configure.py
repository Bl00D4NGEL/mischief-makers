#! /usr/bin/env python3

import argparse
import os
import shutil
import re
import sys
import subprocess
from pathlib import Path
from typing import Dict, List, Set, Union

import ninja_syntax
import requests
import splat
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry

ROOT = Path(__file__).parent.relative_to(os.getcwd())
TOOLS_DIR = ROOT / "tools"

BASENAME = "mischiefmakers"
GAME_VERSION = "us1"

def get_version_num():
    if GAME_VERSION == "jp":
        return 0
    elif GAME_VERSION == "us0":
        return 1
    elif GAME_VERSION == "us1":
        return 2
    elif GAME_VERSION == "eu":
        return 3

YAML_FILE = f"versions/{GAME_VERSION}/{BASENAME}.yaml"
LD_PATH = f"versions/{GAME_VERSION}/{BASENAME}.ld"
MAP_PATH = f"build/{BASENAME}.map"
ELF_PATH = f"build/{BASENAME}.elf"
Z64_PATH = f"build/{BASENAME}.z64"
OK_PATH = f"build/{BASENAME}.ok"

COMMON_INCLUDES = "-I include -I src -I ultralib/include -I ultralib/include/ido -I ultralib/include/PR -I ultralib/src"
IDO_DEFS = f"-D_LANGUAGE_C -D_DEBUG -DF3DEX_GBI -DGAME_VERSION={get_version_num()} -DBUILD_VERSION=VERSION_H"
IDO_DEFS_ULTRA = "-D_DEBUG -DBUILD_VERSION=VERSION_H"

CROSS = "mips-linux-gnu-"
CROSS_AS = f"{CROSS}as"
CROSS_CPP = f"{CROSS}cpp"
CROSS_LD = f"{CROSS}ld"
CROSS_STRIP = f"{CROSS}strip"
CROSS_OBJCOPY = f"{CROSS}objcopy"
AS_FLAGS = f"-G 0 {COMMON_INCLUDES} -EB -mtune=vr4300 -march=vr4300 -mabi=32"
OPT_FLAGS = "-O2"
MIPS_FLAGS = "-mips1 -32"
MIPS_FLAGS_ULTRA = "-mips2 -g1 -32"

IDO_53_CC = TOOLS_DIR / "ido5.3" / "cc"

O32_TOOL = ROOT / "ultralib/tools/set_o32abi_bit.py"

GAME_CC_CMD = f"python3 tools/asm_processor/build.py --input-enc=utf-8 --output-enc=EUC-JP {IDO_53_CC} -- {CROSS_AS} {AS_FLAGS} -- -G 0 -non_shared -fullwarn -verbose -Xcpluscomm -nostdinc -Wab,-r4300_mul $flags {MIPS_FLAGS} {COMMON_INCLUDES} {IDO_DEFS} -c -o $out $in"

LIBULTRA_CC_CMD = f"$ido -G 0 -non_shared -fullwarn -verbose -Wab,-r4300_mul -woff 513,516,649,838,712 -Xcpluscomm -nostdinc $flags {COMMON_INCLUDES} {IDO_DEFS} -DBUILD_VERSION=$libultra -c -o $out $in && {O32_TOOL} $out"

LIBULTRA_AS_CMD = f"{IDO_53_CC} -G 0 -non_shared -fullwarn -verbose -Wab,-r4300_mul -woff 513,516,649,838,712 $flags {MIPS_FLAGS_ULTRA} {COMMON_INCLUDES} {IDO_DEFS_ULTRA} -c -o $out $in && {O32_TOOL} $out && {CROSS_STRIP} $out -N asdasdasdasd"

def clean():
    if os.path.exists(f"versions/{GAME_VERSION}/.splache"):
        os.remove(f"versions/{GAME_VERSION}/.splache")

    shutil.rmtree("asm", ignore_errors=True)
    shutil.rmtree("assets", ignore_errors=True)
    shutil.rmtree("build", ignore_errors=True)

def fullclean():
    if os.path.exists(".ninja.log"):
        os.remove(".ninja.log")
    if os.path.exists("build.ninja"):
        os.remove("build.ninja")
    if os.path.exists("permuter_settings.toml"):
        os.remove("permuter_settings.toml")

    if os.path.exists(f"versions/{GAME_VERSION}/{BASENAME}.d"):
        os.remove(f"versions/{GAME_VERSION}/{BASENAME}.d")
    if os.path.exists(f"versions/{GAME_VERSION}/{BASENAME}.ld"):
        os.remove(f"versions/{GAME_VERSION}/{BASENAME}.ld")
    if os.path.exists(f"versions/{GAME_VERSION}/undefined_funcs_auto.txt"):
        os.remove(f"versions/{GAME_VERSION}/undefined_funcs_auto.txt")
    if os.path.exists(f"versions/{GAME_VERSION}/undefined_syms_auto.txt"):
        os.remove(f"versions/{GAME_VERSION}/undefined_syms_auto.txt")
        
    clean()

def obtain_ido_recomp(version: str):
    download_dir = TOOLS_DIR / f"ido{version}"

    if download_dir.exists():
        print(
            f"IDO {version} already exists at {download_dir}, removing and re-downloading"
        )
        shutil.rmtree(download_dir)

    IDO_RECOMP_VERSION = "v1.1"

    if sys.platform == "darwin":
        ido_os = "macos"
    elif sys.platform == "linux":
        ido_os = "linux"
    elif sys.platform == "win32":
        ido_os = "windows"
    else:
        print(f"Unsupported platform {sys.platform}")
        sys.exit(1)

    ido_tar_name = f"ido-{version}-recomp-{ido_os}.tar.gz"
    url = f"https://github.com/decompals/ido-static-recomp/releases/download/{IDO_RECOMP_VERSION}/{ido_tar_name}"
    target_path = TOOLS_DIR / ido_tar_name

    print(f"Downloading IDO {version}: {url}")
    response = requests.get(url)
    if response.status_code != 200:
        print(f"Failed to download IDO tarball from {url}")
        sys.exit(1)
    with open(target_path, "wb") as f:
        f.write(response.content)

    shutil.unpack_archive(target_path, download_dir)
    os.remove(target_path)


def setup():
    obtain_ido_recomp("5.3")
    print("Setup complete!")


def write_permuter_settings():
    with open("permuter_settings.toml", "w") as f:
        f.write(
            f"""build_system = "ninja"
compiler_type = "ido"

[preserve_macros]
"g[DS]P.*" = "void"
"gs[DS]P.*" = "void"
"G_IM_SIZ_.*" = "int"
"G_[AC]C.*" = "int"
ABS = "int"
ABS_ALT = "int"
SQ = "int"
ARRAY_COUNT = "int"
ARRAY_COUNTU = "int"
CLAMP = "int"
NULL = "int"

[decompme.compilers]
"{IDO_53_CC}" = "ido5.3"
"""
        )


def build_c_segment(entry: any, build):
    opt_level = OPT_FLAGS
    mips = MIPS_FLAGS
    ido = "5.3"
    libultra = "VERSION_H"

    c_path = entry.src_paths[0]
    if "libc" in str(c_path):
        opt_level = "-O3"
        mips = "-mips2 -32"

        if c_path.stem in ["ll", "llbit", "llcvt"]:
            opt_level = "-O1 -g2"
            mips = "-mips3 -32"
        build(
            entry.object_path,
            entry.src_paths,
            "cc_libultra",
            variables={
                "flags": f"{opt_level} {mips}",
                "libultra": libultra,
                "ido": TOOLS_DIR / ("ido" + ido) / "cc",
            },
        )
    elif "ultralib" in str(c_path):
        opt_level = "-O2 -g2"
        mips = "-mips2"

        build(
                entry.object_path,
                entry.src_paths,
                "cc_libultra",
                variables={
                    "flags": f"{opt_level} {mips}",
                    "ido": TOOLS_DIR / ("ido" + ido) / "cc",
                    "libultra": libultra,
                },
            )
    else:
        build(
                entry.object_path,
                entry.src_paths,
                "cc",
                variables={"flags": opt_level},
            )

def create_build_script(linker_entries: List[LinkerEntry]):
    built_objects: Set[Path] = set()

    def build(
        object_paths: Union[Path, List[Path]],
        src_paths: List[Path],
        task: str,
        variables: Dict[str, str] = {},
        implicit_outputs: List[str] = [],
    ):
        if not isinstance(object_paths, list):
            object_paths = [object_paths]

        object_strs = [str(obj) for obj in object_paths]

        for object_path in object_paths:
            if object_path.suffix == ".o":
                built_objects.add(object_path)
            ninja.build(
                outputs=object_strs,
                rule=task,
                inputs=[str(s) for s in src_paths],
                variables=variables,
                implicit_outputs=implicit_outputs,
            )

    ninja = ninja_syntax.Writer(open(str(ROOT / "build.ninja"), "w"), width=96)

    # Rules
    ninja.rule(
        "bin",
        description="bin $in",
        command=f"{CROSS_LD} -r -b binary $in -o $out",
    )

    ninja.rule(
        "as",
        description="as $in",
        command=f"{CROSS_CPP} {COMMON_INCLUDES} -DGAME_VERSION={get_version_num()} $in -o - | iconv -t EUC-JP | {CROSS_AS} -G0 {COMMON_INCLUDES} -EB -mtune=vr4300 -march=vr4300 -o $out",
    )

    ninja.rule(
        "as_libultra",
        description="as $in",
        command=f"{LIBULTRA_AS_CMD}",
    )

    ninja.rule(
        "cc",
        description="cc $in",
        command=f"{GAME_CC_CMD}",
    )

    ninja.rule(
        "cc_libultra",
        description="cc $in",
        command=f"{LIBULTRA_CC_CMD}",
    )

    ninja.rule(
        "ld",
        description="link $out",
        command=f"{CROSS_LD} -T versions/{GAME_VERSION}/undefined_syms_auto.txt -T versions/{GAME_VERSION}/undefined_funcs_auto.txt -T versions/{GAME_VERSION}/undefined_funcs.txt -T versions/{GAME_VERSION}/undefined_syms.txt -Map $mapfile -T $in -o $out",
    )

    ninja.rule(
        "sha1sum",
        description=f"python3 {TOOLS_DIR}/checksum.py $in",
        command=f"python3 {TOOLS_DIR}/checksum.py $in && touch $out",
    )

    ninja.rule(
        "elf",
        description="elf $out",
        command=f"{CROSS_OBJCOPY} $in $out -O binary",
    )

    ninja.rule(
        "z64",
        description="rom $out",
        command=f"{CROSS_OBJCOPY} $in $out -O binary",
    )

    for entry in linker_entries:
        seg = entry.segment

        if seg.type[0] == ".":
            continue

        if entry.object_path is None:
            continue

        if isinstance(seg, splat.segtypes.n64.header.N64SegHeader):
            build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.asm.CommonSegAsm) or isinstance(
            seg, splat.segtypes.common.data.CommonSegData
        ):
            build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.c.CommonSegC):
            build_c_segment(entry, build)
        elif isinstance(seg, splat.segtypes.common.textbin.CommonSegTextbin):
            if seg.sibling is None:
                build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.bin.CommonSegBin):
            build(entry.object_path, entry.src_paths, "bin")
        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)

    ninja.build(
        ELF_PATH,
        "ld",
        LD_PATH,
        implicit=[str(obj) for obj in built_objects],
        variables={"mapfile": MAP_PATH},
    )

    ninja.build(
        Z64_PATH,
        "z64",
        ELF_PATH,
    )

    ninja.build(
        OK_PATH,
        "sha1sum",
        [Z64_PATH, f"checksum.{GAME_VERSION}.sha1"],
    )


def graph_segments():
    import pandas as pd
    import numpy as np
    import matplotlib.pyplot as plt
    from splat.segtypes.segment import Segment

    graph_items = []
    vram_start = 0x80000400

    for item in split.segment_rams.items():
        segment: Segment = item[2]
        graph_items.append(
            dict(
                Segment=segment.name,
                VRAMStart=segment.vram_start,
                VRAMEnd=segment.vram_end,
                VRAMLength=segment.vram_end - segment.vram_start,
                PosFromStart=segment.vram_start - vram_start,
            )
        )

    # Sort graph_items by VRAMStart
    graph_items = sorted(graph_items, key=lambda x: x["VRAMStart"])

    df = pd.DataFrame(graph_items)
    plt.barh(y=df["Segment"], width=df["VRAMLength"], left=df["PosFromStart"])
    plt.grid(axis="x")

    axes = plt.gca()
    xlabels = map(lambda t: "0x%08X" % (int(t) + vram_start), axes.get_xticks())
    axes.set_xticklabels(xlabels)

    plt.show()


def remove_multiline_comments(input_file):
    with open(input_file, 'r') as file:
        text = file.read()

    pattern = r'/\*[\s\S]*?\*/'
    result = re.sub(pattern, '', text)

    # Remove empty lines from the result
    lines = result.splitlines()
    non_empty_lines = [line for line in lines if line.strip()]

    with open(input_file, 'w') as file:
        file.write('\n'.join(non_empty_lines))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "-c",
        "--clean",
        help="Clean extraction and build artifacts",
        action="store_true",
    )
    parser.add_argument(
        "-f",
        "--fullclean",
        help="Fully clean extraction and build artifacts",
        action="store_true",
    )
    parser.add_argument(
        "-i",
        "--setup",
        help="Download and extract IDO compiler",
        action="store_true",
    )
    parser.add_argument(
        "-d",
        "--disassemble-all",
        help="Disassemble all",
        action="store_true",
    )
    parser.add_argument(
        "-b",
        "--build",
        help="Build",
        action="store_true",
    )
    parser.add_argument(
        "-s",
        "--split",
        help="Split",
        action="store_true",
    )
    args = parser.parse_args()

    if args.fullclean:
        fullclean()

    if args.clean:
        clean()

    if args.setup:
        setup()
        sys.exit(0)

    if not Path(f"baserom.{GAME_VERSION}.z64").exists():
        print(f"baserom.{GAME_VERSION}.z64 is missing!")
        sys.exit(1)

    if args.split:
        split.main([YAML_FILE], modes="all", verbose=False, disassemble_all=args.disassemble_all)
        linker_entries = split.linker_writer.entries
        # graph_segments()
        create_build_script(linker_entries)
        write_permuter_settings()

    if args.build:
        subprocess.run(["ninja"])
