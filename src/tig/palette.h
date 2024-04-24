#ifndef ARCANUM_TIG_PALETTE_H_
#define ARCANUM_TIG_PALETTE_H_

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TigPaletteAdjustFlags {
    TIG_PALETTE_ADJUST_TINT = 0x1,
    TIG_PALETTE_ADJUST_GRAYSCALE = 0x2,
} TigPaletteAdjustFlags;

typedef struct TigPaletteAdjustDesc {
    /* 0000 */ unsigned int flags;
    /* 0004 */ unsigned int tint_color;
    /* 0008 */ TigPalette src_palette;
    /* 000C */ TigPalette dst_palette;
} TigPaletteAdjustDesc;

int tig_palette_init(TigInitializeInfo* init_info);
void tig_palette_exit();
TigPalette tig_palette_create();
void tig_palette_destroy(TigPalette palette);
void tig_palette_set_color(TigPalette palette, unsigned int color);
void tig_palette_copy(TigPalette dst, const TigPalette src);
void tig_palette_adjust(TigPaletteAdjustDesc* desc);
size_t tig_palette_get_size();

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_PALETTE_H_ */
