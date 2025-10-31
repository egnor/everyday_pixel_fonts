# everyday_pixel_fonts

<p><img width="410" alt="Microcontroller connected to display showing 'Hello, World!'" src="https://github.com/user-attachments/assets/3d664ded-9d3b-4137-9130-8b46de4a8297" /></p>

Conversions of [VEXED's "Everyday" pixel fonts](https://v3x3d.itch.io/everyday-typical) for embedded systems (Arduino, etc) using the [u8g2 display driver library](https://github.com/olikraus/u8g2/wiki).

The original "Everyday" series of pixel fonts were designed by VEXED with support from me (Dan Egnor). They are licensed under [Creative Commons Attribution 4.0 International](https://creativecommons.org/licenses/by/4.0/deed.en), [as is this repository](LICENSE.txt). I highly recommend checking out [VEXED's site](https://v3x3d.itch.io/) which has tons of fun pixel art and a variety of nifty fonts on offer! If you find these fonts useful please [support the artist](https://www.patreon.com/c/V3X3D/posts).

## Using these fonts

Browse [the proof images in the repository](proof_pngs) to see what the fonts look like, then:

1. Use [u8g2](https://github.com/olikraus/u8g2) (PRs welcome to support Adafruit-GFX-Library, etc)
2. Use this library, it's in the Arduino registry as "Everyday Pixel Fonts" (PRs welcome to support platformio, etc)
3. `#include <everyday_pixel_fonts.h>`
4. Reference any of [the font names defined in that header](src/everyday_pixel_fonts.h) (versioned or unversioned) with `.setFont(...)`

### Example code

This is for a [128x64 SSD1306 display](https://www.adafruit.com/product/326) on default I2C pins. [Watch this code run on wokwi!](https://wokwi.com/projects/446370045333538817)

```
#include <everyday_pixel_fonts.h>
#include <U8g2lib.h>
#include <Wire.h>

using DisplayDriver = U8G2_SSD1306_128X64_NONAME_F_HW_I2C;
DisplayDriver* display;

void setup() {
  Serial.begin(115200);
  Serial.println("starting setup...");
  Wire.begin();
  display = new DisplayDriver(U8G2_R0);
  display->begin();
  display->clearDisplay();
  display->setPowerSave(0);
  display->setFont(everyday_ample_black_u8g2);
  display->setDrawColor(1);
  display->drawStr(10, 10, "Hello, World!");
  display->sendBuffer();
}

void loop() {
  Serial.println("looping...");
  delay(500);
}
```

## Rebuilding this data

Everything in this repository derives from the `.ttf` files in `copied_ttfs/`.
If you want to regenerate derived products (`.bdf` files, u8g2 sources, etc):

- install [`mise`](https://mise.jdx.dev/)
- install a C compiler (on Ubuntu: `sudo apt install build-essential`)
- install [FontForge](https://fontforge.org/en-US/)
- clone this repository
- clone the [u8g2 repository](https://github.com/olikraus/u8g2) in `../u8g2`
- run: `mise install` in this repo directory to install tools (Python, uv, etc)
- EITHER
  - run: `build_tools/ttf_to_bdf.fontforge_py` (requires FontForge)
  - run: `build_tools/bdf_to_u8g2.py` (requires the `../u8g2` repo)
  - run: `build_tools/make_proof_pngs.py` (requires the `../u8g2` repo)
- OR
  - run: `mise run rebuild` (does the build steps above)

...then send me a PR or something.
