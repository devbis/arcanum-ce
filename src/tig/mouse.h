#ifndef ARCANUM_TIG_MOUSE_H_
#define ARCANUM_TIG_MOUSE_H_

#include "tig/types.h"

typedef enum TigMouseButton {
    TIG_MOUSE_BUTTON_LEFT,
    TIG_MOUSE_BUTTON_RIGHT,
    TIG_MOUSE_BUTTON_MIDDLE,
    TIG_MOUSE_BUTTON_COUNT,
};

#define TIG_MOUSE_STATE_HIDDEN 0x01

typedef struct TigMouseState {
    unsigned int flags;
    TigRect rect;
    int offset_x;
    int offset_y;
    int x;
    int y;
    int z;
};

// See 0x4FF9F0.
static_assert(sizeof(TigMouseState) == 0x28, "wrong size");

int tig_mouse_init(TigContext* ctx);
void tig_mouse_exit();
void tig_mouse_set_active(bool is_active);
bool tig_mouse_ping();
void tig_mouse_set_position(int x, int y, int z);
void tig_mouse_set_button(int button, bool pressed);
int tig_mouse_get_state(TigMouseState* mouse_state);
int tig_mouse_hide();
int tig_mouse_show();
void tig_mouse_display();
void sub_4FFB40();
int sub_4FFFE0(unsigned int art_id);
void mouse_set_offset(int a1, int a2);
int sub_500150();
void sub_500160(unsigned int art_id, int a2, int a3);
int sub_500560();
void sub_500570();
void tig_mouse_set_z_axis_enabled(bool enabled);

#endif /* ARCANUM_TIG_MOUSE_H_ */
