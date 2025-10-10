#include <stdio.h>
#include <string.h>

#include "u8g2.h"
#include "u8g2_vexed_everyday_fonts.h"

char text[65536];
char const* lines[512];
int line_count;
u8g2_t u8g2;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "ℹ️ Usage: make_example_bitmap font_name < text > bitmap\n");
    return 2;
  }

  uint8_t const* font = NULL;
  for (int i = 0; all_everyday_fonts[i].name != NULL; ++i) {
    if (!strcmp(argv[1], all_everyday_fonts[i].name)) {
      font = all_everyday_fonts[i].font;
      break;
    }
  }

  if (font == NULL) {
    fprintf(stderr, "💥 Font not found: \"%s\"\n", argv[1]);
    return 1;
  }

  u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, 10, 10);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_SetFont(&u8g2, font);
  fprintf(
      stderr, "🔠 \"%s\": %d glyphs [%c]:\n"
      "  bits: 0-run=%d 1-run=%d box-w=%d box-h=%d off-x=%d off-y=%d delta=%d\n"
      "  sizes: w=%d h=%d x0=%d y0=%d asc-A=%d desc-g=%d asc-(=%d desc-(=%d\n",
      argv[1], u8g2.font_info.glyph_cnt, "pm8"[u8g2.font_info.bbx_mode],
      u8g2.font_info.bits_per_0, u8g2.font_info.bits_per_1,
      u8g2.font_info.bits_per_char_width, u8g2.font_info.bits_per_char_height,
      u8g2.font_info.bits_per_char_x, u8g2.font_info.bits_per_char_y,
      u8g2.font_info.bits_per_delta_x,
      u8g2.font_info.max_char_width, u8g2.font_info.max_char_height,
      u8g2.font_info.x_offset, u8g2.font_info.y_offset,
      u8g2.font_info.ascent_A, u8g2.font_info.descent_g,
      u8g2.font_info.ascent_para, u8g2.font_info.descent_para
  );

  fprintf(stderr, "Reading text from stdin...\n");
  int const len = fread(text, 1, sizeof(text), stdin);
  if (len >= sizeof(text)) {
    fprintf(stderr, "💥 Too much input (limit %lu chars)\n", sizeof(text));
    return 1;
  }
  text[len] = '\0';

  line_count = 0;
  const int max_lines = sizeof(lines) / sizeof(lines[0]);
  for (const char *line = strtok(text, "\n"); line = strtok(NULL, "\n"); ) {
    if (line_count >= max_lines) {
      fprintf(stderr, "💥 Too much input (limit %d lines)\n", max_lines);
      return 1;
    }
    lines[line_count++] = line;
  }

  fprintf(stderr, "📐 text size...\n");

  return 0;
}
