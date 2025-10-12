#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "u8g2.h"
#include "everyday_u8g2_fonts.h"

char buffer[65536];
char const* lines[512];
int line_count;
u8g2_t u8g2;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "ℹ️ Usage: make_text_bitmap font_name < text > bitmap\n");
    fprintf(stderr, "  OR: make_text_bitmap list\n");
    fprintf(stderr, "  OR: make_text_bitmap box (width) (height)\n");
    return 2;
  }

  if (!strcmp(argv[1], "list")) {
    for (int i = 0; all_everyday_fonts[i].name != NULL; ++i) {
      printf("%s\n", all_everyday_fonts[i].name);
    }
    return 0;
  }

  uint8_t const* font = NULL;
  for (int i = 0; font == NULL && all_everyday_fonts[i].name != NULL; ++i) {
    if (!strcmp(argv[1], all_everyday_fonts[i].name))
      font = all_everyday_fonts[i].font;
  }
  if (font == NULL) {
    fprintf(stderr, "💥 Font not found: \"%s\"\n", argv[1]);
    return 1;
  }

  u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, 10, 10);
  u8g2_SetFont(&u8g2, font);
  u8g2_SetFontRefHeightExtendedText(&u8g2);
  fprintf(
      stderr, "🇫 \"%s\": %d glyphs [%c]:\n"
      "  bits: 0-run=%d 1-run=%d box-w=%d box-h=%d off-x=%d off-y=%d dx=%d\n"
      "  size: w=%d h=%d x0=%d y0=%d asc-A=%d desc-g=%d asc-(=%d desc-(=%d\n",
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
  int const len = fread(buffer, 1, sizeof(buffer), stdin);
  if (len >= sizeof(buffer)) {
    fprintf(stderr, "💥 Too much input (%d bytes)\n", len);
    return 1;
  }
  buffer[len] = '\0';

  line_count = 0;
  for (char* next = buffer; *next != '\0'; ) {
    if (line_count >= sizeof(lines) / sizeof(lines[0])) {
      fprintf(stderr, "💥 Too many lines (%d)\n", line_count);
      return 1;
    }
    lines[line_count++] = next;
    while (*next != '\0' && *next != '\n') ++next;
    if (*next != '\0') *next++ = '\0';
  }
  if (line_count <= 0) {
    fprintf(stderr, "💥 No text to draw\n");
    return 1;
  }

  int image_w = 1;
  int max_strlen = 0;
  for (int l = 0; l < line_count; ++l) {
    int const len = strlen(lines[l]);
    if (len > max_strlen) max_strlen = len;
    int const pixels = u8g2_GetUTF8Width(&u8g2, lines[l]);
    if (pixels > image_w) image_w = pixels;
  }

  int const line_h = u8g2.font_ref_ascent - u8g2.font_ref_descent + 1;
  fprintf(
      stderr, "🔠 Text: chars=%dx%d pixels=%dx%d\n",
      max_strlen, line_count, image_w, line_h * line_count
  );
  if (image_w >= 2040) {
    fprintf(stderr, "💥 Image too wide (max 2040)\n");
    return 1;
  }

  /* Draw image in stripes to avoid u8g2 64K buffer size limit */
  u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, image_w, line_h);
  u8g2_SetFont(&u8g2, font);
  u8g2_SetFontRefHeightExtendedText(&u8g2);

  fprintf(stderr, "👾 Writing bitmap (PBM) to stdout...\n");
  printf("P1 %d %d\n", image_w, line_h * line_count);
  for (int l = 0; l < line_count; ++l) {
    u8g2_SetDrawColor(&u8g2, 1);  // black fill
    u8g2_DrawBox(&u8g2, 0, 0, image_w, line_h);
    u8g2_SetDrawColor(&u8g2, 0);  // white text
    u8g2_DrawUTF8(&u8g2, 0, u8g2.font_ref_ascent, lines[l]);
    u8g2_SendBuffer(&u8g2);
    for (int y = 0; y < line_h; ++y) {
      for (int x = 0; x < image_w; ++x) {
        printf("%s%d", x ? " " : "", u8x8_GetBitmapPixel(&u8g2.u8x8, x, y));
      }
      printf("\n");
    }
    printf("\n");
  }

  return 0;
}
