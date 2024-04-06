#include "tig/bmp.h"

#include "tig/memory.h"

// 0x5360D0
int tig_bmp_destroy(TigBmp* bmp)
{
    if (bmp->pixels != NULL) {
        FREE(bmp->pixels);
    }

    return TIG_OK;
}

// 0x5368B0
int tig_bmp_copy_to_video_buffer(TigBmp* bmp, const TigRect* src_rect, TigVideoBuffer* video_buffer, const TigRect* dst_rect)
{
    // TODO: Incomplete.
    (void)bmp;
    (void)src_rect;
    (void)video_buffer;
    (void)dst_rect;

    return TIG_OK;
}
