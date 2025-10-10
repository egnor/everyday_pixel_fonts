#!/usr/bin/env python3
"""Compiles u8g2 library sources and 'bdfconv' with musl-gcc"""

import argparse
import logging
import ok_logging_setup
import ok_subprocess_defaults
import platform
import shutil
from pathlib import Path

ok_logging_setup.install()
subproc = ok_subprocess_defaults.SubprocessDefaults()

parser = argparse.ArgumentParser()
parser.add_argument("u8g2_repo_dir", type=Path)
args = parser.parse_args()

tools_dir = Path(__file__).parent
try: tools_dir = tools_dir.relative_to(Path.cwd())
except ValueError: pass

arch = f"{platform.machine().lower()}-{platform.system().lower()}"
compiler = f"{arch}-musl-gcc"
if not shutil.which(compiler):
    ok_logging_setup.exit(f"Not found: {compiler} (try: apt install musl-dev)")

lib_dirs = [
    args.u8g2_repo_dir / "csrc",
    args.u8g2_repo_dir / "sys" / "bitmap" / "common",
    args.u8g2_repo_dir / "sys" / "utf8" / "common",
]

lib_sources = [fn for dir in lib_dirs for fn in dir.glob("*.c")]
lib_headers = [fn for dir in lib_dirs for fn in dir.glob("*.h")]

lib_list = "".join(f"\n  {d}/" for d in lib_dirs)
if not lib_headers: ok_logging_setup.exit("No *.h: " + lib_list)
if not lib_sources: ok_logging_setup.exit("No *.c: " + lib_list)

include_dir = tools_dir / f"{arch}-u8g2-include"
include_dir.mkdir(parents=True, exist_ok=True)
logging.info(f"🗑️ Cleaning: {include_dir}")
for old_path in include_dir.glob("*.h"): old_path.unlink()

for header_path in lib_headers:
    out_path = include_dir / header_path.name
    logging.info(f"👥 Copying: {out_path}")
    shutil.copyfile(header_path, out_path)

lib_obj_dir = tools_dir / f"{arch}-u8g2-obj.tmp"
lib_obj_dir.mkdir(parents=True, exist_ok=True)
[p.unlink() for p in lib_obj_dir.glob("*.o")]
logging.info(f"\n🏗️ Compiling: {lib_obj_dir} ({len(lib_sources)} objects)")
for lib_source_path in lib_sources:
    subproc.run(
        compiler, "-O4", f"-I{include_dir}", "-c", lib_source_path,
        "-o", lib_obj_dir / (lib_source_path.stem + ".o"),
    )

lib_out = tools_dir / f"{arch}-u8g2.a"
lib_out.unlink(missing_ok=True)
logging.info(f"\n📚 Building: {lib_out}")
subproc.run("ar", "rs", lib_out, *lib_obj_dir.glob("*.o"))

bdfconv_dir = args.u8g2_repo_dir / "tools" / "font" / "bdfconv"
bdfconv_sources = bdfconv_dir.glob("*.c")
if not bdfconv_sources: ok_logging_setup.exit("No *.c: " + bdfconv_dir)

bdfconv_out = tools_dir / f"{arch}-bdfconv.exe"
logging.info(f"\n🏗️ Building: {bdfconv_out}")
subproc.run(
    compiler, "-static", "-O4", f"-I{bdfconv_dir}",
    *bdfconv_sources, "-o", bdfconv_out,
)
logging.info(f"\n🏁 Updated:\n  {include_dir}\n  {lib_out}\n  {bdfconv_out}")
