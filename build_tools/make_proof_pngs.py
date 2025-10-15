#!/usr/bin/env python3
"""Builds 'make_proof_pbms' against u8g2 tree, converts output to png."""

import argparse
import io
import logging
import ok_logging_setup
import ok_subprocess_defaults
import os.path
import PIL.Image
from natsort import natsorted, ns
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
lib_sources = natsorted(
    (path for dir in lib_dirs for path in dir.glob("*.c")),
    alg=ns.I | ns.P
)
if not lib_sources:
    ok_logging_setup.exit("No *.c:" + "".join(f"\n  {d}" for d in lib_dirs))

build_dir = top_dir / "build.tmp"
build_dir.mkdir(exist_ok=True)
make_pbms_source = top_dir / "build_tools" / "make_proof_pbms.c"
make_pbms = build_dir / "make_proof_pbms"
logging.info(f"\n🏗️ Building: {make_pbms}")
subproc.run(
    "gcc", "-O4", "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections",
    *(f"-I{dir}" for dir in lib_dirs), make_pbms_source, *lib_sources,
    "-o", make_pbms,
)

logging.info(f"\n🗑️ Cleaning: {build_dir}/*.pbm")
[old_path.unlink() for old_path in build_dir.glob("*.pbm")]

logging.info(f"🖼️ Generating examples: {build_dir}/")
subproc.run(os.path.join(".", make_pbms.relative_to(build_dir)), cwd=build_dir)

png_dir = top_dir / "proof_pngs"
png_dir.mkdir(exist_ok=True)
logging.info(f"\n🗑️ Cleaning: {png_dir}/*.png")
[old_path.unlink() for old_path in png_dir.glob("*.png")]

for pbm_path in natsorted(build_dir.glob("*.pbm"), alg=ns.I | ns.P):
    png_path = png_dir / f"{pbm_path.stem}-proof.png"
    logging.info(f"🖼️ Converting: {png_path}")
    image = PIL.Image.open(pbm_path, formats=["ppm"])
    image.resize((image.width * 2, image.height * 2)).save(png_path)
