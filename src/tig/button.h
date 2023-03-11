#ifndef ARCANUM_TIG_BUTTON_H_
#define ARCANUM_TIG_BUTTON_H_

#include "tig/types.h"

#define TIG_BUTTON_FLAG_0x01 0x01
#define TIG_BUTTON_FLAG_0x02 0x02
#define TIG_BUTTON_FLAG_0x04 0x04
#define TIG_BUTTON_FLAG_HIDDEN 0x08
#define TIG_BUTTON_FLAG_0x10 0x10

typedef struct TigButtonData {
    int flags;
    int window_handle;
    int x;
    int y;
    unsigned int art_id;
    int width;
    int height;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
};

static_assert(sizeof(TigButtonData) == 0x2C, "wrong size");

int tig_button_init(TigContext* ctx);
void tig_button_exit();
int tig_button_create(TigButtonData* button_data, int* button_handle);
int tig_button_destroy(int button_handle);
int tig_button_data(int button_handle, TigButtonData* button_data);
int tig_button_refresh_rect(int window_handle, TigRect* rect);
void tig_button_state_change(int button_handle, int state);
int tig_button_state_get(int button_handle, int* state);
int sub_5380F0(int a1, int a2);
int sub_538220(void* a1);
int tig_button_radio_group(int count, int* button_handles, int a3);
int sub_538730(int button_handle);
int sub_5387E0(int button_handle);
int sub_538840(int button_handle);
int tig_button_is_hidden(int button_handle, bool* hidden);
int tig_button_show(int button_handle);
int tig_button_hide(int button_handle);
void tig_button_set_art(int button_handle, int art_id);

#endif /* ARCANUM_TIG_BUTTON_H_ */
