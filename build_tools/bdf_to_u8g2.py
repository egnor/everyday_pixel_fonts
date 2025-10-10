#!/usr/bin/env python3
"""Converts .bdf files (X11 bitmap format) to u8g2-format source files."""

import logging
import ok_logging_setup
import ok_subprocess_defaults
import platform
import re

from pathlib import Path

ok_logging_setup.install()

top_dir = Path(__file__).parent.parent
try: top_dir = top_dir.relative_to(Path.cwd())
except ValueError: pass

conversions_dir = top_dir / "derived_bdfs"
bdf_paths = list(sorted(conversions_dir.glob("**/*.bdf")))
if not bdf_paths:
    ok_logging_setup.exit(f"No .bdf files in {conversions_dir}")

subproc = ok_subprocess_defaults.SubprocessDefaults()
arch = f"{platform.machine().lower()}-{platform.system().lower()}"
bdfconv_path = top_dir / "build_tools" / f"{arch}-bdfconv.exe"
if not bdfconv_path.is_file():
    ok_logging_setup.exit(f"Missing: {bdfconv_path}")

src_dir = top_dir / "src"
src_dir.mkdir(parents=True, exist_ok=True)

delete_paths = set()
delete_paths.update(src_dir.glob("font_*.c"))
for delete_path in sorted(delete_paths):
    logging.info(f"🗑️ Deleting: {delete_paths}")
    delete_path.unlink()

for bdf_path in bdf_paths:
    logging.info(f"\n🔠 Converting: {bdf_path}")
    font_name = re.sub(r"-[0-9]+$", "", bdf_path.stem.lower())
    font_name = re.sub(r"[^a-z0-9]+", "_", font_name)
    font_path = src_dir / f"font_{font_name}.c"
    subproc.run(
        bdfconv_path, "-b0", "-f1",
        f"-n{font_name}", f"-o{font_path}", bdf_path,
    )

logging.info(f"\n🏁 Done converting {len(bdf_paths)} .bdf fonts to u8g2")
