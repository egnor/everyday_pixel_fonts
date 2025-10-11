#!/usr/bin/env python3
"""Builds 'make_bitmap' against u8g2 source tree and generates proof images."""

import argparse
import io
import logging
import ok_logging_setup
import ok_subprocess_defaults
import PIL.Image
from pathlib import Path

EXAMPLE_TEXT = """
Angel Adept Blind Bodice Clique Coast Dunce Docile Enact Eosin
Furlong Focal Gnome Gondola Human Hoist Inlet Iodine Justin Jocose
Knoll Koala Linden Loads Milliner Modal Number Nodule Onset Oddball
Pneumo Poncho Quanta Qophs Rhone Roman Snout Sodium Tundra Tocsin
Uncle Udder Vulcan Vocal Whale Woman Xmas Xenon Yunnan Young Zloty Zodiac

Angel angel adept for the nuance loads of the arena cocoa and quaalude.
Blind blind bodice for the submit oboe of the club snob and abbot.
Clique clique coast for the pouch loco of the franc assoc and accede.
Dunce dunce docile for the loudness mastodon of the loud statehood and huddle.
Enact enact eosin for the quench coed of the pique canoe and bleep.
Furlong furlong focal for the genuflect profound of the motif aloof and offers.
Gnome gnome gondola for the impugn logos of the unplug analog and smuggle.
Human human hoist for the buddhist alcohol of the riyadh caliph and bathhouse.
Inlet inlet iodine for the quince champion of the ennui scampi and shiite.
Justin justin jocose for the djibouti sojourn of the oranj raj and hajjis.
Knoll knoll koala for the banknote lookout of the dybbuk outlook and trekked.
Linden linden loads for the ulna monolog of the consul menthol and shallot.
Milliner milliner modal for the alumna solomon of the album custom and summon.
Number number nodule for the unmade economic of the shotgun bison and tunnel.
Onset onset oddball for the abandon podium of the antiquo tempo and moonlit.
Pneumo pneumo poncho for the dauphin opossum of the holdup bishop and supplies.
Quanta quanta qophs for the inquest sheqel of the cinq coq and suqqu.
Rhone rhone roman for the burnt porous of the lemur clamor and carrot.
Snout snout sodium for the ensnare bosom of the genus pathos and missing.
Tundra tundra tocsin for the nutmeg isotope of the peasant ingot and ottoman.
Uncle uncle udder for the dunes cloud of the hindu thou and continuum.
Vulcan vulcan vocal for the alluvial ovoid of the yugoslav chekhov and revved.
Whale whale woman for the meanwhile blowout of the forepaw meadow and glowworm.
Xmas xmas xenon for the bauxite doxology of the tableaux equinox and exxon.
Yunnan yunnan young for the dynamo coyote of the obloquy employ and sayyid.
Zloty zloty zodiac for the gizmo ozone of the franz laissez and buzzing.

ABIDE ACORN OF THE HABIT DACRON FOR THE BUDDHA GOUDA QUAALUDE.
BENCH BOGUS OF THE SCRIBE ROBOT FOR THE APLOMB JACOB RIBBON.
CENSUS CORAL OF THE SPICED JOCOSE FOR THE BASIC HAVOC SOCCER.
DEMURE DOCILE OF THE TIDBIT LODGER FOR THE CUSPID PERIOD BIDDER.
EBBING ECHOING OF THE BUSHED DECAL FOR THE APACHE ANODE NEEDS.
FEEDER FOCUS OF THE LIFER BEDFORD FOR THE SERIF PROOF BUFFER.
GENDER GOSPEL OF THE PIGEON DOGCART FOR THE SPRIG QUAHOG DIGGER.
HERALD HONORS OF THE DIHEDRAL MADHOUSE FOR THE PENH RIYADH BATHHOUSE.
IBSEN ICEMAN OF THE APHID NORDIC FOR THE SUSHI SAUDI SHIITE.
JENNIES JOGGER OF THE TIJERA ADJOURN FOR THE ORANJ KOWBOJ HAJJIS.
KEEPER KOSHER OF THE SHRIKE BOOKCASE FOR THE SHEIK LOGBOOK CHUKKAS.
LENDER LOCKER OF THE CHILD GIGOLO FOR THE UNCOIL GAMBOL ENROLLED.
MENACE MCCOY OF THE NIMBLE TOMCAT FOR THE DENIM RANDOM SUMMON.
NEBULA NOSHED OF THE INBRED BRONCO FOR THE COUSIN CARBON KENNEL.
OBSESS OCEAN OF THE PHOBIC DOCKSIDE FOR THE GAUCHO LIBIDO HOODED.
PENNIES PODIUM OF THE SNIPER OPCODE FOR THE SCRIP BISHOP HOPPER.
QUANTA QOPHS OF THE INQUEST OQOS FOR THE CINQ COQ SUQQU.
REDUCE ROGUE OF THE GIRDLE ORCHID FOR THE MEMOIR SENSOR SORREL.
SENIOR SCONCE OF THE DISBAR GODSON FOR THE HUBRIS AMENDS LESSEN.
TENDON TORQUE OF THE UNITED SCOTCH FOR THE NOUGHT FORGOT BITTERS.
UNDER UGLINESS OF THE RHUBARB SEDUCE FOR THE MANCHU HINDU CONTINUUM.
VERSED VOUCH OF THE DIVER OVOID FOR THE TELAVIV KARPOV FLIVVER.
WENCH WORKER OF THE UNWED SNOWCAP FOR THE ANDREW ESCROW GLOWWORM.
XENON XOCHITL OF THE MIXED BOXCAR FOR THE SUFFIX ICEBOX EXXON.
YEOMAN YONDER OF THE HYBRID ARROYO FOR THE DINGHY BRANDY SAYYID.
ZEBRA ZOMBIE OF THE PRIZED OZONE FOR THE FRANZ ARROZ BUZZING.

To my "friends" & "family" (don't worry?): "Hello, world!" [me@example.com]
*Stars*, _underscores_, and `backticks`; 1234567890 < 9876543210 > 0x12345678
https://example.com:8080/index.html?q=Hello%2C%20world%21#top
C:\Program Files (x86)\Steam\steam.exe
ip=192.168.3.47 net=255.255.255.0 gw=192.168.3.1
email_rx = re.compile(r"^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}$")

int main(int argc, char *argv[]) {
  printf("Hello, world!\\n");
  return 0;
}
""".strip("\n")

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
make_bitmap_source = top_dir / "build_tools" / "make_bitmap.c"
make_bitmap = build_dir / "make_bitmap"
logging.info(f"\n🏗️ Building: {make_bitmap}")
subproc.run(
    "gcc", "-O4", "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections",
    *(f"-I{dir}" for dir in lib_dirs), make_bitmap_source, *lib_sources,
    "-o", make_bitmap,
)

png_dir = top_dir / "example_pngs"
png_dir.mkdir(exist_ok=True)
font_names = subproc.stdout_lines(make_bitmap, "list");
for name in font_names:
    png_path = png_dir / f"{name}_example.png"
    logging.info(f"\n🖼️ Making example: {png_path}")
    text = f"Font: {name}\n\n{EXAMPLE_TEXT}"
    pbm_text = subproc.stdout_text(make_bitmap, name, input=text)
    pbm_stream = io.BytesIO(pbm_text.encode())
    image = PIL.Image.open(pbm_stream, formats=["ppm"])
    image.save(png_path)
