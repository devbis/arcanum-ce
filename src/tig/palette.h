#ifndef ARCANUM_TIG_PALETTE_H_
#define ARCANUM_TIG_PALETTE_H_

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TigPaletteModifyFlags {
    TIG_PALETTE_MODIFY_TINT = 0x1,
    TIG_PALETTE_MODIFY_GRAYSCALE = 0x2,
} TigPaletteModifyFlags;

typedef struct TigPaletteModifyInfo {
    /* 0000 */ TigPaletteModifyFlags flags;
    /* 0004 */ unsigned int tint_color;
    /* 0008 */ TigPalette src_palette;
    /* 000C */ TigPalette dst_palette;
} TigPaletteModifyInfo;

int tig_palette_init(TigInitializeInfo* init_info);
void tig_palette_exit();
TigPalette tig_palette_create();
void tig_palette_destroy(TigPalette palette);
void tig_palette_set_color(TigPalette palette, unsigned int color);
void tig_palette_copy(TigPalette dst, const TigPalette src);
void tig_palette_modify(const TigPaletteModifyInfo* modify_info);
size_t tig_palette_system_memory_size();

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_PALETTE_H_ */
