#!/usr/bin/env python3
"""Builds 'make_example' against u8g2 source tree and generates proof images."""

import argparse
import logging
import ok_logging_setup
import ok_subprocess_defaults
from pathlib import Path

ok_logging_setup.install()

top_dir = Path(__file__).parent.parent
try: top_dir = top_dir.relative_to(Path.cwd())
except ValueError: pass

parser = argparse.ArgumentParser()
parser.add_argument("--u8g2_repo", type=Path, default=top_dir / ".." / "u8g2")
args = parser.parse_args()
subproc = ok_subprocess_defaults.SubprocessDefaults()

lib_dirs = [
    top_dir / "src",
    args.u8g2_repo / "csrc",
    args.u8g2_repo / "sys" / "bitmap" / "common",
]
lib_sources = [fn for dir in lib_dirs for fn in sorted(dir.glob("*.c"))]
if not lib_sources:
    ok_logging_setup.exit("No *.c:" + "".join(f"\n  {d}" for d in lib_dirs))

build_dir = top_dir / "build.tmp"
build_dir.mkdir(exist_ok=True)
make_example_source = top_dir / "build_tools" / "make_example_bitmap.c"
make_example = build_dir / "make_example"
logging.info(f"\n🏗️ Building: {make_example}")
subproc.run(
    "gcc", "-O4", *(f"-I{dir}" for dir in lib_dirs),
    make_example_source, *lib_sources, "-o", make_example,
)
