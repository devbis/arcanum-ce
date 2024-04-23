#include "tig/font.h"

#include "tig/art.h"
#include "tig/color.h"
#include "tig/memory.h"
#include "tig/rect.h"
#include "tig/video.h"

#define FONT_STACK_SIZE 20

static int sub_535850(TigVideoBuffer* video_buffer, const char* str, int length, TigArtBlitSpec* blt, bool shadow);
static int sub_535C40(tig_art_id_t font_art_id, const char* str, int max_width, int* width_ptr);
static bool tig_font_glyph_data(tig_art_id_t font_art_id, int ch, int* width_ptr, int* height_ptr, int* dx_ptr, int* dy_ptr);

// 0x630CFC
static int tig_font_stack_index;

// 0x630D00
static TigFont* tig_font_stack[FONT_STACK_SIZE];

// 0x630D50
static unsigned int tig_font_shadow_color;

// 0x630D54
static TigFont* tig_font_default_font;

// 0x5351D0
int tig_font_init(TigInitializeInfo* init_info)
{
    (void)init_info;

    TigFont font;

    tig_font_stack_index = -1;
    tig_font_shadow_color = tig_color_make(10, 10, 10);

    font.flags = TIG_FONT_SHADOW;
    tig_art_misc_id_create(TIG_ART_SYSTEM_FONT, 0, &(font.art_id));
    font.scale = 1.5f;
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    font.underline_color = font.color;
    font.strike_through_color = font.color;
    tig_font_create(&font, &tig_font_default_font);
    tig_font_push(tig_font_default_font);

    return TIG_OK;
}

// 0x5352C0
void tig_font_exit()
{
    tig_font_pop();
    tig_font_destroy(tig_font_default_font);
}

// 0x5352E0
void tig_font_create(TigFont* desc, TigFont** font_ptr)
{
    TigFont* font;
    TigArtData art_data;

    font = (TigFont*)MALLOC(sizeof(TigFont));
    memcpy(font, desc, sizeof(TigFont));

    if (tig_art_data(desc->art_id, &art_data) != TIG_OK) {
        // Fatal error - missing font art.
        exit(EXIT_FAILURE);
    }

    *font_ptr = font;
}

// 0x535330
void tig_font_destroy(TigFont* font)
{
    FREE(font);
}

// 0x535340
int tig_font_push(TigFont* font)
{
    tig_font_stack_index++;
    if (tig_font_stack_index < FONT_STACK_SIZE) {
        tig_font_stack[tig_font_stack_index] = font;
    } else {
        // Font #0 is the default and therefore should be skipped.
        tig_font_stack_index = 1;
        tig_font_stack[tig_font_stack_index] = font;
    }

    return tig_font_stack_index;
}

// 0x535380
void tig_font_pop()
{
    if (tig_font_stack_index > 0) {
        tig_font_stack_index--;
    }
}

// 0x535390
void sub_535390(TigFont* font)
{
    const char* str;
    int width;
    int height;
    int glyph_width;
    int glyph_height;
    int glyph_dx;
    int glyph_dy;
    int pos;
    int lines;
    int line_length;
    int line_width;

    str = font->str;
    width = font->width;
    height = font->height;
    memcpy(font, tig_font_stack[tig_font_stack_index], sizeof(TigFont));
    font->str = str;
    font->width = width;
    font->height = height;

    width = 0;
    height = 0;

    if (str != NULL) {
        glyph_height = 0;
        lines = 1;
        if (font->width != 0) {
            if (!tig_font_glyph_data(tig_font_stack[tig_font_stack_index]->art_id, str[0], &glyph_width, &glyph_height, &glyph_dx, &glyph_dy)) {
                font->height = 0;
                return;
            }

            pos = 0;
            do {
                line_length = sub_535C40(tig_font_stack[tig_font_stack_index]->art_id, &(str[pos]), font->width, &line_width);
                if (line_length == -1) {
                    font->height = 0;
                    return;
                }

                pos += line_length;
                lines++;
            } while (str[pos] != '\0');
        } else {
            pos = 0;
            while (str[pos] != '\0') {
                if (!tig_font_glyph_data(tig_font_stack[tig_font_stack_index]->art_id, str[pos], &glyph_width, &glyph_height, &glyph_dx, &glyph_dy)) {
                    font->height = 0;
                    return;
                }

                if (str[pos + 1] != '\0') {
                    width += glyph_dx;
                } else {
                    width += glyph_width;
                }

                pos++;
            }
        }

        height = glyph_height * lines;

        if ((font->flags & TIG_FONT_SHADOW) != 0) {
            ++height;
            ++width;
        }

        if ((font->flags & TIG_FONT_SCALE) != 0) {
            width = (int)((float)width * font->scale);
            height = (int)((float)height * font->scale);
        }
    }

    if (font->width == 0 || str == NULL) {
        font->width = width;
    }

    font->height = height;
}

// 0x535570
int tig_font_write(TigVideoBuffer* video_buffer, const char* str, const TigRect* rect, TigRect* dirty_rect)
{
    TigArtBlitSpec blt;
    TigRect dst_rect;
    TigRect src_rect;
    int glyph_dx;
    int glyph_dy;
    int glyph_height;
    int glyph_width;
    int max_width;
    int max_y;
    int min_dx;
    int num_passes;
    int pass;
    bool shadow;
    int dst_rect_x;

    // NOTE: Initialize to keep compiler happy.
    max_y = 0;

    if (str == NULL) {
        return TIG_OK;
    }

    if ((tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_SHADOW) != 0) {
        num_passes = 2;
        shadow = true;
    } else {
        num_passes = 1;
        shadow = false;
    }

    blt.dst_rect = &dst_rect;
    blt.src_rect = &src_rect;
    blt.dst_video_buffer = video_buffer;

    src_rect.x = 0;
    src_rect.y = 0;

    min_dx = rect->width;
    max_width = 0;

    for (pass = 0; pass < num_passes; pass++) {
        blt.flags = 0;

        if (!shadow) {
            blt.flags |= 0x2000;
            if ((tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_0x100) != 0) {
                blt.flags |= 0x10;
            }

            if ((tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_0x80) != 0) {
                blt.flags |= 0x200;
            }

            blt.field_10 = tig_font_stack[tig_font_stack_index]->color;
        }

        dst_rect.x = rect->x;
        dst_rect.y = rect->y;

        if (shadow) {
            blt.flags |= 0x2000;
            ++dst_rect.x;
            ++dst_rect.y;
            blt.field_10 = tig_font_shadow_color;
        }

        if (!tig_font_glyph_data(tig_font_stack[tig_font_stack_index]->art_id, ' ', &glyph_width, &glyph_height, &glyph_dx, &glyph_dy)) {
            return TIG_ERR_16;
        }

        max_y = rect->y;
        dst_rect_x = dst_rect.x;

        if (glyph_height <= rect->height) {
            int line_length;
            int line_width;
            int dx;
            int rc;

            while (1) {
                line_length = sub_535C40(tig_font_stack[tig_font_stack_index]->art_id, str, rect->width, &line_width);
                if (line_length == -1) {
                    break;
                }

                if ((tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_CENTERED) != 0) {
                    dx = (rect->width - line_width) / 2;
                } else {
                    dx = 0;
                }

                if (dx < min_dx) {
                    min_dx = dx;
                }

                if (line_width > max_width) {
                    max_width = line_width;
                }

                dst_rect.x += dx;

                if (dx != 0 && !shadow) {
                    dst_rect.x += dst_rect_x;
                }

                rc = sub_535850(video_buffer, str, line_length + 1, &blt, shadow);
                if (rc != TIG_OK) {
                    return rc;
                }

                dst_rect.y += glyph_height;
                dst_rect.x = dst_rect_x;

                max_y += glyph_height;

                if (str[line_length] == '\0') {
                    break;
                }

                str += line_length + 1;
                if (rect->y + rect->height - max_y < glyph_height) {
                    break;
                }
            }
        }

        shadow = false;
    }

    if (dirty_rect != NULL) {
        dirty_rect->x = rect->x + min_dx;
        dirty_rect->y = rect->y;
        dirty_rect->width = max_width;
        dirty_rect->height = max_y - rect->y + 1;

        if ((tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_SHADOW) != 0) {
            ++dirty_rect->width;
            ++dirty_rect->height;
        }
    }

    return TIG_OK;
}

// 0x535850
int sub_535850(TigVideoBuffer* video_buffer, const char* str, int length, TigArtBlitSpec* blt, bool shadow)
{
    tig_art_id_t glyph_art_id;
    int blt_dst_rect_x;
    int blt_dst_rect_y;
    int glyph_dx;
    int glyph_dy;
    int glyph_height;
    int glyph_width;
    int pos;
    int rc;

    // NOTE: Initialize to make compiler happy.
    glyph_dy = 0;
    rc = TIG_OK;

    blt_dst_rect_x = blt->dst_rect->x;
    blt_dst_rect_y = blt->dst_rect->y;

    for (pos = 0; pos < length; pos++) {
        if (str[pos] == '\0') {
            break;
        }

        if (str[pos] != '\n') {
            glyph_art_id = tig_art_id_frame_set(tig_font_stack[tig_font_stack_index]->art_id,
                str[pos] - 31);

            if (!tig_font_glyph_data(glyph_art_id, str[pos], &glyph_width, &glyph_height, &glyph_dx, &glyph_dy)) {
                return TIG_ERR_16;
            }

            if (str[pos] != '\t') {
                blt->art_id = glyph_art_id;
                blt->src_rect->width = glyph_width;
                blt->src_rect->height = glyph_height;
                blt->dst_rect->width = glyph_width;
                blt->dst_rect->height = glyph_height;

                if ((tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_SCALE) != 0) {
                    blt->dst_rect->width = (int)((float)blt->dst_rect->width * tig_font_stack[tig_font_stack_index]->scale);
                    blt->dst_rect->height = (int)((float)blt->dst_rect->height * tig_font_stack[tig_font_stack_index]->scale);
                }

                rc = tig_art_blit(blt);
                if (rc != TIG_OK) {
                    return rc;
                }
            }

            if ((tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_SCALE) != 0) {
                blt->dst_rect->x += (int)((float)glyph_dx * tig_font_stack[tig_font_stack_index]->scale);
            } else {
                blt->dst_rect->x += glyph_dx;
            }
        }
    }

    if (*str != '\0'
        && (tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_UNDERLINE) != 0) {
        int y = blt->dst_rect->y;

        blt->dst_rect->width = blt->dst_rect->x - blt_dst_rect_x;
        blt->dst_rect->height = 1;
        blt->dst_rect->x = blt_dst_rect_x;
        blt->dst_rect->y = blt_dst_rect_y;

        if ((tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_SCALE) != 0) {
            blt->dst_rect->y += (int)((float)glyph_dy * tig_font_stack[tig_font_stack_index]->scale);
        } else {
            blt->dst_rect->y += glyph_dy;
        }

        if (shadow) {
            ++blt->dst_rect->x;
            ++blt->dst_rect->y;
            --blt->dst_rect->width;
            tig_video_buffer_fill(video_buffer,
                blt->dst_rect,
                tig_color_make(10, 10, 10));
            --blt->dst_rect->x;
            --blt->dst_rect->y;
            ++blt->dst_rect->width;
        }

        rc = tig_video_buffer_fill(video_buffer,
            blt->dst_rect,
            tig_font_stack[tig_font_stack_index]->underline_color);

        blt->dst_rect->y = y;
    }

    if (*str != '\0'
        && (tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_STRIKE_THROUGH) != 0) {
        int y = blt->dst_rect->y;

        blt->dst_rect->width = blt->dst_rect->x - blt_dst_rect_x;
        blt->dst_rect->height = 1;
        blt->dst_rect->x = blt_dst_rect_x;
        blt->dst_rect->y = blt_dst_rect_y;

        if ((tig_font_stack[tig_font_stack_index]->flags & TIG_FONT_SCALE) != 0) {
            blt->dst_rect->y += (int)((float)glyph_dy * tig_font_stack[tig_font_stack_index]->scale / 2.0);
        } else {
            blt->dst_rect->y += glyph_dy / 2;
        }

        if (shadow) {
            ++blt->dst_rect->x;
            ++blt->dst_rect->y;
            --blt->dst_rect->width;
            rc = tig_video_buffer_fill(video_buffer,
                blt->dst_rect,
                tig_color_make(10, 10, 10));
            --blt->dst_rect->x;
            --blt->dst_rect->y;
            ++blt->dst_rect->width;
        }

        tig_video_buffer_fill(video_buffer,
            blt->dst_rect,
            tig_font_stack[tig_font_stack_index]->strike_through_color);

        blt->dst_rect->y = y;
    }

    return rc;
}

// 0x535C40
int sub_535C40(tig_art_id_t font_art_id, const char* str, int max_width, int* width_ptr)
{
    int glyph_width;
    int glyph_height;
    int glyph_dx;
    int glyph_dy;
    int pos;
    int prev;
    int width;
    int total_width;

    total_width = 0;
    prev = -1;
    pos = 0;
    while (str[pos] != '\0') {
        if (str[pos] == '\n') {
            // Found NL, that's what we were looking for.
            prev = pos;
            *width_ptr = total_width;
            break;
        }

        if (!tig_font_glyph_data(font_art_id, str[pos], &glyph_width, &glyph_height, &glyph_dx, &glyph_dy)) {
            return -1;
        }

        if (str[pos] == ' ') {
            // Found SPACE, save it as a candidate to be a line break.
            prev = pos;
            *width_ptr = total_width;
        }

        width = str[pos + 1] != '\0' ? glyph_dx : glyph_width;

        if (total_width + width > max_width) {
            break;
        }

        total_width += glyph_dx;

        if (str[pos] == '-') {
            // Found HYPHEN, save it a candidate to be a line break.
            prev = pos;
            *width_ptr = total_width;
        }

        pos++;
    }

    if (str[pos] != '\0') {
        if (prev != pos) {
            pos = prev;
        }
    } else {
        *width_ptr = total_width;
    }

    return pos;
}

// 0x535D10
bool tig_font_glyph_data(tig_art_id_t font_art_id, int ch, int* width_ptr, int* height_ptr, int* dx_ptr, int* dy_ptr)
{
    TigArtFrameData glyph_frame_data;
    tig_art_id_t glyph_art_id;

    glyph_art_id = tig_art_id_frame_set(font_art_id, (unsigned char)ch - 31);
    if (tig_art_frame_data(glyph_art_id, &glyph_frame_data) != TIG_OK) {
        return false;
    }

    *width_ptr = glyph_frame_data.width;
    *height_ptr = glyph_frame_data.height;
    *dx_ptr = glyph_frame_data.hot_x;
    *dy_ptr = glyph_frame_data.hot_y;

    if ((unsigned char)ch == '\t') {
        *dx_ptr += 20;
    }

    return true;
}
