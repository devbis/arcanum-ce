#include "tig/bmp.h"

#include "tig/color.h"
#include "tig/file.h"
#include "tig/memory.h"
#include "tig/window.h"

// 0x535D80
int tig_bmp_create(TigBmp* bmp)
{
    TigFile* stream;
    BITMAPFILEHEADER file_hdr;
    BITMAPINFOHEADER info_hdr;
    int num_colors;
    int data_size;
    int x;
    int y;
    uint8_t color;
    RGBQUAD palette[256];
    int index;

    if (bmp == NULL) {
        return TIG_ERR_12;
    }

    bmp->pixels = NULL;

    stream = tig_file_fopen(bmp->name, "rb");
    if (stream == NULL) {
        return TIG_ERR_13;
    }

    static_assert(sizeof(file_hdr) == 0xE, "wrong size");
    if (tig_file_fread(&file_hdr, sizeof(file_hdr), 1, stream) != 1) {
        tig_file_fclose(stream);
        return TIG_ERR_13;
    }

    if (file_hdr.bfType != 0x4D42) {
        tig_file_fclose(stream);
        return TIG_ERR_16;
    }

    static_assert(sizeof(info_hdr) == 0x28, "wrong size");
    if (tig_file_fread(&info_hdr, sizeof(info_hdr), 1, stream) != 1) {
        tig_file_fclose(stream);
        return TIG_ERR_13;
    }

    if (info_hdr.biSize != sizeof(info_hdr)
        || info_hdr.biPlanes != 1
        || (info_hdr.biBitCount != 4
            && info_hdr.biBitCount != 8
            && info_hdr.biBitCount != 24)
        || info_hdr.biCompression != BI_RGB) {
        tig_file_fclose(stream);
        return TIG_ERR_16;
    }

    bmp->bpp = info_hdr.biBitCount;

    num_colors = info_hdr.biClrUsed;
    if (num_colors == 0) {
        switch (info_hdr.biBitCount) {
        case 1:
            num_colors = 2;
            break;
        case 4:
            num_colors = 16;
            break;
        case 8:
            num_colors = 256;
            break;
        case 24:
            num_colors = 0;
            break;
        default:
            tig_file_fclose(stream);
            return TIG_ERR_16;
        }
    }

    if (num_colors > 0) {
        if (info_hdr.biBitCount > 8) {
            if (tig_file_fread(palette, sizeof(RGBQUAD), num_colors, stream) != num_colors) {
                tig_file_fclose(stream);
                return TIG_ERR_13;
            }
        }

        for (index = 0; index < num_colors; ++index) {
            bmp->palette[index] = tig_color_to_24_bpp(palette[index].rgbRed,
                palette[index].rgbGreen,
                palette[index].rgbBlue);
        }
    }

    bmp->width = info_hdr.biWidth;
    bmp->height = info_hdr.biHeight;

    if (info_hdr.biSizeImage != 0) {
        bmp->pitch = info_hdr.biSizeImage / info_hdr.biHeight;
    } else {
        bmp->pitch = info_hdr.biWidth;
        if (bmp->pitch % 4 > 0) {
            bmp->pitch = bmp->pitch - bmp->pitch % 4 + 4;
        }
    }

    data_size = bmp->pitch * bmp->height;
    bmp->pixels = MALLOC(data_size);
    if (bmp->pixels == NULL) {
        tig_file_fclose(stream);
        return TIG_ERR_5;
    }

    if (tig_file_fread(bmp->pixels, data_size, 1, stream) != 1) {
        tig_file_fclose(stream);
        return TIG_ERR_13;
    }

    for (y = 0; y < bmp->height / 2; ++y) {
        for (x = 0; x < bmp->width; ++x) {
            color = bmp->pixels[bmp->pitch * y + x];
            bmp->pixels[bmp->pitch * y + x] = bmp->pixels[bmp->pitch * (bmp->height - y - 1) + x];
            bmp->pixels[bmp->pitch * (bmp->height - y - 1) + x] = color;
        }
    }

    tig_file_fclose(stream);
    return TIG_OK;
}

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

// 0x5377A0
int tig_bmp_copy_to_bmp(TigBmp* src, TigBmp* dst)
{
    float width_ratio;
    float height_ratio;
    int x;
    int y;

    if (src->bpp != 8) {
        return TIG_ERR_12;
    }

    if (dst->width <= 0) {
        dst->width = src->width;
    }

    if (dst->height <= 0) {
        dst->height = src->height;
    }

    dst->pitch = src->width;
    if ((dst->width % 4) > 0) {
        dst->pitch = dst->pitch - dst->pitch % 4 + 4;
    }

    dst->bpp = 8;
    memcpy(dst->palette, src->palette, sizeof(src->palette));

    dst->pixels = MALLOC(dst->pitch * dst->height);

    width_ratio = (float)src->width / (float)dst->width;
    height_ratio = (float)src->height / (float)dst->height;

    for (y = 0; y < dst->height; ++y) {
        for (x = 0; x < dst->width; ++x) {
            dst->pixels[dst->pitch * y + x] = src->pixels[src->pitch * (int)(y * height_ratio) + (int)(x * width_ratio)];
        }
    }

    return TIG_OK;
}

// 0x537900
int tig_bmp_copy_to_window(TigBmp* bmp, const TigRect* src_rect, tig_window_handle_t window_handle, const TigRect* dst_rect)
{
    TigVideoBuffer* video_buffer;

    if (tig_window_vbid_get(window_handle, &video_buffer)) {
        return TIG_ERR_16;
    }

    return tig_bmp_copy_to_video_buffer(bmp, src_rect, video_buffer, dst_rect);
}
