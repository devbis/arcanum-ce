#ifndef TIG_DRAW_H_
#define TIG_DRAW_H_

// NOTE: This module does not draw anything. It only provide accessors to line
// properties which are used by `tig_video_buffer_line` exclusively. I'm not
// sure why this couple of accessors was put in a separate module with it's
// own init/exit functions. As for the name (`draw`) it is based on location in
// binary which appears between file cache and drop-down (named `cache` and
// `menu` respectively).

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TigDrawLinePatternInfo {
    int field_0;
    int field_4;
} TigDrawLinePatternInfo;

typedef enum TigDrawLineStyle {
    TIG_LINE_STYLE_SOLID = 0,
    TIG_LINE_STYLE_DOTTED = 1,
    TIG_LINE_STYLE_DASHED = 2,
} TigDrawLineStyle;

typedef struct TigDrawLineStrokeInfo {
    TigDrawLineStyle style;
} TigDrawLineStrokeInfo;

int tig_draw_init(TigInitializeInfo* init_info);
void tig_draw_exit();
void tig_draw_set_line_pattern_info(TigDrawLinePatternInfo* pattern_info);
void tig_draw_get_line_pattern_info(TigDrawLinePatternInfo* pattern_info);
void tig_draw_set_line_stroke_info(TigDrawLineStrokeInfo* stroke_info);
void tig_draw_get_line_stroke_info(TigDrawLineStrokeInfo* stroke_info);

#ifdef __cplusplus
}
#endif

#endif /* TIG_DRAW_H_ */
