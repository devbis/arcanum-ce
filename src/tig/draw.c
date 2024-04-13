#include "tig/draw.h"

// 0x5C2810
static TigDrawLinePatternInfo tig_draw_line_pattern_info = { 0, 1 };

// 0x63650C
static TigDrawLineStrokeInfo tig_draw_lin_stroke_info;

// 0x538EE0
int tig_draw_init(TigInitializeInfo* init_info)
{
    (void)init_info;

    return TIG_OK;
}

// 0x538EF0
void tig_draw_exit()
{
}

// 0x538F00
void tig_draw_set_line_pattern_info(TigDrawLinePatternInfo* pattern_info)
{
    tig_draw_line_pattern_info = *pattern_info;
}

// 0x538F20
void tig_draw_get_line_pattern_info(TigDrawLinePatternInfo* pattern_info)
{
    *pattern_info = tig_draw_line_pattern_info;
}

// 0x538F40
void tig_draw_set_line_stroke_info(TigDrawLineStrokeInfo* stroke_info)
{
    tig_draw_lin_stroke_info = *stroke_info;
}

// 0x538F50
void tig_draw_get_line_stroke_info(TigDrawLineStrokeInfo* stroke_info)
{
    *stroke_info = tig_draw_lin_stroke_info;
}
