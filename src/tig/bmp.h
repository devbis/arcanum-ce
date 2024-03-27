#ifndef TIG_BMP_H_
#define TIG_BMP_H_

#include "tig/types.h"

typedef struct TigBmp {
    /* 0000 */ char name[TIG_MAX_PATH];
    /* 0104 */ int bpp;
    /* 0108 */ unsigned int palette[256];
    /* 0508 */ unsigned char* pixels;
    /* 050C */ int width;
    /* 0510 */ int height;
    /* 0514 */ int pitch;
} TigBmp;

static_assert(sizeof(TigBmp) == 0x518, "wrong size");

int tig_bmp_copy_to_video_buffer(TigBmp* bmp, const TigRect* src_rect, TigVideoBuffer* video_buffer, const TigRect* dst_rect);

#endif /* TIG_BMP_H_ */
