#!/usr/bin/env python3
"""Compiles 'bdfconv' from a u8g2 tree with musl-gcc and copies the binary"""

import argparse
import logging
import ok_logging_setup
import ok_subprocess_defaults
import platform
import shutil

from pathlib import Path

ok_logging_setup.install()

parser = argparse.ArgumentParser()
parser.add_argument("u8g2_repo_dir", type=Path)
args = parser.parse_args()
u8g2_bdfconv_dir = args.u8g2_repo_dir / "tools" / "font" / "bdfconv"
if not u8g2_bdfconv_dir.is_dir():
    ok_logging_setup.exit(f"Not found: {u8g2_bdfconv_dir}")

arch = f"{platform.machine().lower()}-{platform.system().lower()}"
gcc = f"{arch}-musl-gcc"
if not shutil.which(gcc):
    ok_logging_setup.exit(f"Not installed: {gcc} (try: apt install musl-dev)")

subproc = ok_subprocess_defaults.SubprocessDefaults()
make_flags=["CFLAGS=-O4", "LDFLAGS=-static", f"CC={gcc}"]
subproc.run("make", "-C", u8g2_bdfconv_dir, "clean")
subproc.run("make", "-C", u8g2_bdfconv_dir, *make_flags)

built_exe = u8g2_bdfconv_dir / "bdfconv"
output_exe = Path(__file__).parent / f"{arch}-bdfconv.exe"
if not built_exe.is_file():
    ok_logging_setup.exit(f"Not found: {built_exe}")

shutil.copyfile(built_exe, output_exe)
shutil.copymode(built_exe, output_exe)
logging.info(f"\n🏁 Updated: {output_exe}")
