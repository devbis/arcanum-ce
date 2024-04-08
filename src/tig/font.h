#ifndef ARCANUM_TIG_FELICITY_H_
#define ARCANUM_TIG_FELICITY_H_

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TigFontFlags {
    TIG_FONT_SCALE = 0x01,
    TIG_FONT_UNDERLINE = 0x02,
    TIG_FONT_SHADOW = 0x08,
    TIG_FONT_CENTERED = 0x10,
    TIG_FONT_STRIKE_THROUGH = 0x20,
    TIG_FONT_0x80 = 0x80,
    TIG_FONT_0x100 = 0x100,
} TigFontFlags;

typedef struct TigFont {
    /* 0000 */ unsigned int flags;
    /* 0004 */ unsigned int art_id;
    /* 0008 */ tig_color_t color;
    /* 000C */ tig_color_t underline_color;
    /* 0010 */ int field_10;
    /* 0014 */ tig_color_t strike_through_color;
    /* 0018 */ float scale;
    /* 001C */ const char* str;
    /* 0020 */ int width;
    /* 0024 */ int height;
    /* 0028 */ int field_28;
} TigFont;

// See 0x5352E0.
static_assert(sizeof(TigFont) == 0x2C, "wrong size");

int tig_font_init(TigInitializeInfo* init_info);
void tig_font_exit();
void tig_font_create(TigFont* desc, TigFont** font_ptr);
void tig_font_destroy(TigFont* font);
int tig_font_push(TigFont* font);
void tig_font_pop();
void sub_535390(TigFont* font);
int tig_font_write(TigVideoBuffer* video_buffer, const char* str, const TigRect* rect, TigRect* dirty_rect);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_FELICITY_H_ */
