#include "tig/draw.h"

// 0x5C2810
static TigDrawLineModeInfo tig_draw_line_mode_info = { TIG_DRAW_LINE_MODE_NORMAL, 1 };

// 0x63650C
static TigDrawLineStyleInfo tig_draw_line_style_info;

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
void tig_draw_set_line_mode(TigDrawLineModeInfo* mode_info)
{
    tig_draw_line_mode_info = *mode_info;
}

// 0x538F20
void tig_draw_get_line_mode(TigDrawLineModeInfo* mode_info)
{
    *mode_info = tig_draw_line_mode_info;
}

// 0x538F40
void tig_draw_set_line_style(TigDrawLineStyleInfo* style_info)
{
    tig_draw_line_style_info = *style_info;
}

// 0x538F50
void tig_draw_get_line_style(TigDrawLineStyleInfo* style_info)
{
    *style_info = tig_draw_line_style_info;
}
