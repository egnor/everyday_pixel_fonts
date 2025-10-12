# everyday_pixel_fonts

These are conversions of [VEXED's "Everyday" pixel fonts](https://v3x3d.itch.io/everyday-typical) for embedded systems (Arduino, etc) using the [u8g2 small-display-driver library](https://github.com/olikraus/u8g2/wiki).

The original "Everyday" series of pixel fonts were designed by VEXED with support from me (Dan Egnor). They are licensed under [Creative Commons Attribution 4.0 International](https://creativecommons.org/licenses/by/4.0/deed.en), as is this repository.

## Using these fonts

## Rebuilding this data

Everything in this repository derives from the `.ttf` files in `copied_ttfs/`.
If you want to regenerate derived products (`.bdf` files, u8g2 sources, etc):

- install [`mise`](https://mise.jdx.dev/)
- install a C compiler (on Ubuntu: `sudo apt install build-essential`)
- install [FontForge](https://fontforge.org/en-US/)
- clone this repository
- clone the [`u8g2`](https://github.com/olikraus/u8g2) repository next to it
- run: `mise install` in this repo directory to install tools (Python, uv, etc)
- run: `build_tools/ttf_to_bdf.fontforge_py` (requires FontForge)
- run: `build_tools/bdf_to_u8g2.py` (requires the `../u8g2` repo)
- run: `build_tools/make_example_pngs` (requires the `../u8g2` repo)

...then send me a PR or something.
