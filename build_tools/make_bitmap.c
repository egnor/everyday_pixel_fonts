#include <stdio.h>
#include <string.h>

#include "u8g2.h"
#include "u8g2_vexed_everyday_fonts.h"

char text[65536];
char const* lines[512];
int line_count;
u8g2_t u8g2;

static void output_cb(char const* s) { fputs(s, stdout); }

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "ℹ️ Usage: make_example_bitmap font_name < text > bitmap\n");
    return 2;
  }

  if (!strcmp(argv[1], "list")) {
    for (int i = 0; all_everyday_fonts[i].name != NULL; ++i) {
      printf("%s\n", all_everyday_fonts[i].name);
    }
    return 0;
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
  u8g2_SetFontRefHeightExtendedText(&u8g2);
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

  fprintf(stderr, "📥 Reading text from stdin...\n");
  int const len = fread(text, 1, sizeof(text), stdin);
  if (len >= sizeof(text)) {
    fprintf(stderr, "💥 Too much input (>=%lu chars)\n", sizeof(text));
    return 1;
  }
  text[len] = '\0';

  line_count = 0;
  int const max_lines = sizeof(lines) / sizeof(lines[0]);
  for (char const* line = strtok(text, "\n"); line; line = strtok(NULL, "\n")) {
    if (line_count >= max_lines) {
      fprintf(stderr, "💥 Too much input (>=%d lines)\n", max_lines);
      return 1;
    }
    lines[line_count++] = line;
  }

  int max_width = 0;
  int max_strlen = 0;
  for (int l = 0; l < line_count; ++l) {
    int const len = strlen(lines[l]);
    if (len > max_strlen) max_strlen = len;
    int const pixels = u8g2_GetUTF8Width(&u8g2, lines[l]);
    if (pixels > max_width) max_width = pixels;
  }

  int const line_height = u8g2.font_ref_ascent - u8g2.font_ref_descent + 1;
  int const total_height = line_height * line_count;
  fprintf(
      stderr, "📐 Text: chars=%dx%d pixels=%dx%d\n",
      max_strlen, line_count, max_width, total_height
  );

  u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, max_width, total_height);
  u8g2_SetFont(&u8g2, font);
  u8g2_SetFontRefHeightExtendedText(&u8g2);
  u8g2_ClearBuffer(&u8g2);

  for (int l = 0; l < line_count; ++l) {
    const int y = l * line_height + u8g2.font_ref_ascent;
    u8g2_DrawUTF8(&u8g2, 0, y, lines[l]);
  }

  fprintf(stderr, "👾 Writing bitmap (PBM) to stdout...\n");
  u8g2_WriteBufferPBM(&u8g2, output_cb);
  return 0;
}
