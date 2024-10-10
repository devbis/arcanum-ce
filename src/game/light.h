#ifndef ARCANUM_GAME_LIGHT_H_
#define ARCANUM_GAME_LIGHT_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/timeevent.h"

#define LIGHT_HANDLE_INVALID 0
#define SHADOW_HANDLE_INVALID 0

typedef unsigned int light_handle_t;
typedef unsigned int shadow_handle_t;

typedef struct Light30 {
    /* 0000 */ int64_t obj;
    /* 0008 */ int64_t loc;
    /* 0010 */ int offset_x;
    /* 0014 */ int offset_y;
    /* 0018 */ unsigned int flags;
    /* 001C */ tig_art_id_t art_id;
    /* 0020 */ uint8_t r;
    /* 0021 */ uint8_t b;
    /* 0022 */ uint8_t g;
    /* 0024 */ tig_color_t tint_color;
    /* 0028 */ TigPalette palette;
    /* 002C */ int field_2C;
} Light30;

// See 0x4DDD20.
static_assert(sizeof(Light30) == 0x30, "wrong size");

bool light_init(GameInitInfo* init_info);
void light_exit();
void light_resize(ResizeContext* resize_info);
bool light_update_view(ViewOptions* view_options);
void sub_4D81F0();
void sub_4D8350(UnknownContext* ctx);
void light_build_color(uint8_t red, uint8_t green, uint8_t blue, unsigned int* color);
void light_get_color_components(unsigned int color, uint8_t* red, uint8_t* green, uint8_t* blue);
tig_color_t light_get_outdoor_color();
tig_color_t light_get_indoor_color();
void light_set_colors(tig_color_t indoor_color, tig_color_t outdoor_color);
void sub_4D8590(Light30* light);
void sub_4D8620(Light30* light);
bool light_timeevent_process(TimeEvent* timeevent);
bool sub_4D94D0(TigFile* stream, Light30** a2);
bool sub_4D94F0(TigFile* stream, Light30* a2);
void sub_4D9570(Light30* light);
void sub_4D9990(int64_t obj);
void sub_4D9A90(object_id_t object_id);
void sub_4DA310(object_id_t object_id);
uint8_t sub_4DCE10(int64_t obj);
void light_get_rect(Light30* light, TigRect* rect);
void sub_4DDA70(Light30* light, int offset_x, int offset_y);
bool sub_4DDD20(TigFile* stream, Light30** a2);
bool sub_4DDD70(TigFile* stream, Light30* a2);
void sub_4DF310(TigRect* rect, bool invalidate);

#endif /* ARCANUM_GAME_LIGHT_H_ */
