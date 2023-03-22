#include "tig/font.h"

#include "tig/art.h"
#include "tig/color.h"

#define FONT_STACK_SIZE 20

// 0x630CFC
static int tig_font_stack_index;

// 0x630D00
static TigFont* tig_font_stack[FONT_STACK_SIZE];

// 0x630D50
static unsigned int dword_630D50;

// 0x630D54
static TigFont* off_630D54;

// 0x5351D0
int tig_font_init(TigContext* ctx)
{
    tig_font_stack_index = -1;

    TigFont font;
    font.flags = 8;
    sub_501DD0(10, 0, &(font.art_id));
    dword_630D50 = tig_color_rgb_make(10, 10, 10);
    font.field_18 = 0x3FC00000;
    font.field_1C = 0;
    font.field_8 = tig_color_rgb_make(255, 255, 255);
    font.field_C = font.field_8;
    font.field_14 = font.field_8;
    tig_font_create(&font, &off_630D54);
    tig_font_push(off_630D54);

    return TIG_OK;
}

// 0x5352C0
void tig_font_exit()
{
    tig_font_pop();
    tig_font_destroy(off_630D54);
}

// 0x5352E0
void tig_font_create(TigFont* src, TigFont** dst)
{
    TigFont* copy = (TigFont*)malloc(sizeof(*copy));
    memcpy(copy, src, sizeof(TigFont));

    TigArtData art_data;
    if (tig_art_data(src->art_id, &art_data) != TIG_OK) {
        exit(EXIT_FAILURE);
    }

    *dst = copy;
}

// 0x535330
void tig_font_destroy(TigFont* font)
{
    free(font);
}

// 0x535340
int tig_font_push(TigFont* font)
{
    tig_font_stack_index++;
    if (tig_font_stack_index < FONT_STACK_SIZE) {
        tig_font_stack[tig_font_stack_index] = font;
    } else {
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
