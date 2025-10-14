#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "u8g2.h"
#include "everyday_u8g2_fonts.h"

u8g2_t u8g2;

char header_0[120] = "";
char header_1[120] = "";

char const* const text_lines[] = {
  header_0,
  header_1,
  "",
  "Angel Adept Blind Bodice Clique Coast Dunce Docile Enact Eosin",
  "Furlong Focal Gnome Gondola Human Hoist Inlet Iodine Justin Jocose",
  "Knoll Koala Linden Loads Milliner Modal Number Nodule Onset Oddball",
  "Pneumo Poncho Quanta Qophs Rhone Roman Snout Sodium Tundra Tocsin",
  "Uncle Udder Vulcan Vocal Whale Woman Xmas Xenon Yunnan Young Zloty Zodiac",
  "",
  "Angel angel adept for the nuance loads of the arena cocoa and quaalude.",
  "Blind blind bodice for the submit oboe of the club snob and abbot.",
  "Clique clique coast for the pouch loco of the franc assoc and accede.",
  "Dunce dunce docile for the loudness mastodon of the loud statehood and huddle.",
  "Enact enact eosin for the quench coed of the pique canoe and bleep.",
  "Furlong furlong focal for the genuflect profound of the motif aloof and offers.",
  "Gnome gnome gondola for the impugn logos of the unplug analog and smuggle.",
  "Human human hoist for the buddhist alcohol of the riyadh caliph and bathhouse.",
  "Inlet inlet iodine for the quince champion of the ennui scampi and shiite.",
  "Justin justin jocose for the djibouti sojourn of the oranj raj and hajjis.",
  "Knoll knoll koala for the banknote lookout of the dybbuk outlook and trekked.",
  "Linden linden loads for the ulna monolog of the consul menthol and shallot.",
  "Milliner milliner modal for the alumna solomon of the album custom and summon.",
  "Number number nodule for the unmade economic of the shotgun bison and tunnel.",
  "Onset onset oddball for the abandon podium of the antiquo tempo and moonlit.",
  "Pneumo pneumo poncho for the dauphin opossum of the holdup bishop and supplies.",
  "Quanta quanta qophs for the inquest sheqel of the cinq coq and suqqu.",
  "Rhone rhone roman for the burnt porous of the lemur clamor and carrot.",
  "Snout snout sodium for the ensnare bosom of the genus pathos and missing.",
  "Tundra tundra tocsin for the nutmeg isotope of the peasant ingot and ottoman.",
  "Uncle uncle udder for the dunes cloud of the hindu thou and continuum.",
  "Vulcan vulcan vocal for the alluvial ovoid of the yugoslav chekhov and revved.",
  "Whale whale woman for the meanwhile blowout of the forepaw meadow and glowworm.",
  "Xmas xmas xenon for the bauxite doxology of the tableaux equinox and exxon.",
  "Yunnan yunnan young for the dynamo coyote of the obloquy employ and sayyid.",
  "Zloty zloty zodiac for the gizmo ozone of the franz laissez and buzzing.",
  "",
  "ABIDE ACORN OF THE HABIT DACRON FOR THE BUDDHA GOUDA QUAALUDE.",
  "BENCH BOGUS OF THE SCRIBE ROBOT FOR THE APLOMB JACOB RIBBON.",
  "CENSUS CORAL OF THE SPICED JOCOSE FOR THE BASIC HAVOC SOCCER.",
  "DEMURE DOCILE OF THE TIDBIT LODGER FOR THE CUSPID PERIOD BIDDER.",
  "EBBING ECHOING OF THE BUSHED DECAL FOR THE APACHE ANODE NEEDS.",
  "FEEDER FOCUS OF THE LIFER BEDFORD FOR THE SERIF PROOF BUFFER.",
  "GENDER GOSPEL OF THE PIGEON DOGCART FOR THE SPRIG QUAHOG DIGGER.",
  "HERALD HONORS OF THE DIHEDRAL MADHOUSE FOR THE PENH RIYADH BATHHOUSE.",
  "IBSEN ICEMAN OF THE APHID NORDIC FOR THE SUSHI SAUDI SHIITE.",
  "JENNIES JOGGER OF THE TIJERA ADJOURN FOR THE ORANJ KOWBOJ HAJJIS.",
  "KEEPER KOSHER OF THE SHRIKE BOOKCASE FOR THE SHEIK LOGBOOK CHUKKAS.",
  "LENDER LOCKER OF THE CHILD GIGOLO FOR THE UNCOIL GAMBOL ENROLLED.",
  "MENACE MCCOY OF THE NIMBLE TOMCAT FOR THE DENIM RANDOM SUMMON.",
  "NEBULA NOSHED OF THE INBRED BRONCO FOR THE COUSIN CARBON KENNEL.",
  "OBSESS OCEAN OF THE PHOBIC DOCKSIDE FOR THE GAUCHO LIBIDO HOODED.",
  "PENNIES PODIUM OF THE SNIPER OPCODE FOR THE SCRIP BISHOP HOPPER.",
  "QUANTA QOPHS OF THE INQUEST OQOS FOR THE CINQ COQ SUQQU.",
  "REDUCE ROGUE OF THE GIRDLE ORCHID FOR THE MEMOIR SENSOR SORREL.",
  "SENIOR SCONCE OF THE DISBAR GODSON FOR THE HUBRIS AMENDS LESSEN.",
  "TENDON TORQUE OF THE UNITED SCOTCH FOR THE NOUGHT FORGOT BITTERS.",
  "UNDER UGLINESS OF THE RHUBARB SEDUCE FOR THE MANCHU HINDU CONTINUUM.",
  "VERSED VOUCH OF THE DIVER OVOID FOR THE TELAVIV KARPOV FLIVVER.",
  "WENCH WORKER OF THE UNWED SNOWCAP FOR THE ANDREW ESCROW GLOWWORM.",
  "XENON XOCHITL OF THE MIXED BOXCAR FOR THE SUFFIX ICEBOX EXXON.",
  "YEOMAN YONDER OF THE HYBRID ARROYO FOR THE DINGHY BRANDY SAYYID.",
  "ZEBRA ZOMBIE OF THE PRIZED OZONE FOR THE FRANZ ARROZ BUZZING.",
  "",
  "DE Victor jagt zwölf Boxkämpfer quer über den großen Sylter Deich.",
  "DK Høj bly gom vandt fræk sexquiz på wc.",
  "ES Queda gazpacho, fibra, látex, jamón, kiwi y viñas.",
  "FI Törkylempijävongahdus.",
  "FR Voyez le brick géant que j'examine près du wharf.",
  "HU Jó foxim és don Quijote húszwattos lámpánál ülve egy pár bűvös cipőt készít.",
  "IE Ċuaiġ bé ṁórṡáċ le dlúṫspád fíorḟinn trí hata mo ḋea-ṗorcáin ḃig.",
  "NO Jeg begynte å fortære en sandwich mens jeg kjørte taxi på vei til quiz.",
  "PT Luís argüia à Júlia que «brações, fé, chá, óxido, pôr, zângão» eram palavras do português.",
  "SE Flygande bäckasiner söka hwila på mjuka tuvor.",
  "",
  "EN  To my \"friends\" & \"family\" (don't worry?): \"Hello, world!\" [me@example.com]",
  "MD  *Stars*, _underscores_, and `backticks`; 12345678 < 87654321 > 0x12345678",
  "URL https://example.com:8080/index.html?q=Hello%2C%20world%21#top",
  "DOS C:\\Program Files (x86)\\Steam\\steam.exe",
  "IP  ip=192.168.3.47 net=255.255.255.0 gw=192.168.3.1",
  "PY  email_rx = re.compile(r\"^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}$\")",
  "C   int main(int argc, char *argv[]) { printf(\"Hello, world!\\n\"); return 0; }",
  NULL
};

static void make_example(struct everyday_entry const* entry) {
  u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, 10, 10);
  u8g2_SetFont(&u8g2, entry->font);
  u8g2_SetFontRefHeightExtendedText(&u8g2);

  snprintf(
      header_0, sizeof(header_0), "Font: \"%s\" (%d glyphs) [%c] %dx%d",
      entry->name, u8g2.font_info.glyph_cnt, "pm8"[u8g2.font_info.bbx_mode],
      u8g2.font_info.max_char_width, u8g2.font_info.max_char_height
  );

  snprintf(
      header_1, sizeof(header_1),
      "asc+desc: cell=%d+%d max=%d+%d A+g=%d+%d (=%d+%d\n",
      u8g2.font_info.max_char_height + u8g2.font_info.y_offset,
      -u8g2.font_info.y_offset,
      u8g2.font_ref_ascent, -u8g2.font_ref_descent,
      u8g2.font_info.ascent_A, -u8g2.font_info.descent_g,
      u8g2.font_info.ascent_para, -u8g2.font_info.descent_para
  );

  int image_w = 1;
  int line_count = 0;
  for (; text_lines[line_count] != NULL; ++line_count) {
    int const len = strlen(text_lines[line_count]);
    int const pixels = u8g2_GetUTF8Width(&u8g2, text_lines[line_count]);
    if (pixels > image_w) image_w = pixels;
  }

  int const line_h = u8g2.font_ref_ascent - u8g2.font_ref_descent + 1;
  if (image_w >= 2040) {
    fprintf(stderr, "💥 Image too wide (max 2040)\n");
    exit(1);
  }

  /* Draw image in stripes to avoid u8g2 64K buffer size limit */
  u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, image_w, line_h);
  u8g2_SetFont(&u8g2, entry->font);
  u8g2_SetFontRefHeightExtendedText(&u8g2);

  char bitmap_filename[128];
  snprintf(bitmap_filename, sizeof(bitmap_filename), "%s.pbm", entry->name);
  fprintf(stderr, "👾 %s\n", bitmap_filename);

  FILE* fp = fopen(bitmap_filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "💥 %s: %s\n", strerror(errno), bitmap_filename);
    exit(1);
  }

  fprintf(fp, "P1 %d %d\n", image_w, line_h * line_count);
  for (int l = 0; text_lines[l] != NULL; ++l) {
    u8g2_SetDrawColor(&u8g2, 1);  // black fill
    u8g2_DrawBox(&u8g2, 0, 0, image_w, line_h);
    u8g2_SetDrawColor(&u8g2, 0);  // white text
    u8g2_DrawUTF8(&u8g2, 0, u8g2.font_ref_ascent, text_lines[l]);
    u8g2_SendBuffer(&u8g2);
    for (int y = 0; y < line_h; ++y) {
      for (int x = 0; x < image_w; ++x) {
        if (x) fputc(' ', fp);
        fputc(u8x8_GetBitmapPixel(&u8g2.u8x8, x, y) ? '1' : '0', fp);
      }
      fputc('\n', fp);
    }
    fputc('\n', fp);
  }
  fclose(fp);
}

int main(int argc, char* argv[]) {
  if (argc != 1) {
    fprintf(stderr, "ℹ️ Usage: cd outdir && make_example_pbms\n");
    return 2;
  }

  for (int i = 0; all_everyday_fonts[i].name != NULL; ++i) {
    make_example(&all_everyday_fonts[i]);
  }

  return 0;
}
