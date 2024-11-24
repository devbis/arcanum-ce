#include "ui/fate_ui.h"

#include "game/critter.h"
#include "game/mes.h"
#include "game/target.h"
#include "ui/charedit_ui.h"
#include "ui/intgame.h"
#include "ui/types.h"

#define NUM_BUTTONS 12

static void fate_ui_create();
static void fate_ui_destroy();
static bool fate_ui_message_filter(TigMessage* msg);
static void sub_56FF40(long long obj, int btn);

// 0x5CAAE0
static tig_window_handle_t fate_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x5CAAE8
static UiButtonInfo stru_5CAAE8[NUM_BUTTONS] = {
    { 223, 4, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 22, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 40, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 58, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 76, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 94, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 112, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 130, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 148, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 166, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 184, 293, TIG_BUTTON_HANDLE_INVALID },
    { 223, 202, 293, TIG_BUTTON_HANDLE_INVALID },
};

// 0x680ED0
static tig_font_handle_t fate_ui_font;

// 0x680ED8
static long long qword_680ED8;

// 0x680EE0
static int fate_ui_mes;

// 0x680EE4
static bool fate_ui_initialized;

// 0x680EE8
static bool fate_ui_created;

// 0x56FAE0
bool fate_ui_init(GameInitInfo* init_info)
{
    TigFont font_info;

    (void)init_info;

    if (!mes_load("mes\\fate_ui.mes", &fate_ui_mes)) {
        return false;
    }

    font_info.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_info, &fate_ui_font);

    fate_ui_initialized = true;
    sub_4F5320(sub_56FF40);

    return true;
}

// 0x56FBB0
void fate_ui_reset()
{
    if (fate_ui_created) {
        sub_56FC40();
    }
}

// 0x56FBC0
void fate_ui_exit()
{
    mes_unload(fate_ui_mes);
    tig_font_destroy(fate_ui_font);
    fate_ui_initialized = false;
}

// 0x56FBF0
void sub_56FBF0(long long obj)
{
    if (fate_ui_created) {
        sub_56FC40();
        return;
    }

    if (obj != 0) {
        if (!sub_45D8D0(obj)) {
            qword_680ED8 = obj;
            fate_ui_create();
        }
    }
}

// 0x56FC40
void sub_56FC40()
{
    if (fate_ui_created) {
        fate_ui_destroy();
        qword_680ED8 = 0;
    }
}

// 0x56FC70
void fate_ui_create()
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigRect window_rect;
    TigRect button_rect;
    TigWindowData window_data;
    TigArtBlitInfo blit_info;
    int index;
    MesFileEntry mes_file_entry;

    if (fate_ui_created) {
        return;
    }

    if (tig_art_interface_id_create(292, 0, 0, 0, &art_id) != TIG_OK) {
        return;
    }

    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    window_rect.x = 0;
    window_rect.y = 41;
    window_rect.width = art_frame_data.width;
    window_rect.height = art_frame_data.height;

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.rect = window_rect;
    window_data.message_filter = fate_ui_message_filter;
    if (tig_window_create(&window_data, &fate_ui_window) != TIG_OK) {
        tig_debug_printf("fate_ui_create: ERROR: window create failed!\n");
        exit(0);
    }

    window_rect.x = 0;
    window_rect.y = 0;

    blit_info.src_rect = &window_rect;
    blit_info.art_id = art_id;
    blit_info.flags = 0;
    blit_info.dst_rect = &window_rect;
    tig_window_blit_art(fate_ui_window, &blit_info);

    tig_font_push(fate_ui_font);

    button_rect.x = 11;
    button_rect.y = 3;
    button_rect.width = window_rect.width - 11;
    button_rect.height = 18;

    for (index = 0; index < NUM_BUTTONS; index++) {
        mes_file_entry.num = index;
        mes_get_msg(fate_ui_mes, &mes_file_entry);
        tig_window_text_write(fate_ui_window, mes_file_entry.str, &button_rect);
        button_rect.y += button_rect.height;
    }

    tig_font_pop();

    for (index = 0; index < NUM_BUTTONS; index++) {
        sub_54AA60(fate_ui_window,
            &window_rect,
            &(stru_5CAAE8[index]),
            sub_4F5090(qword_680ED8, index)
                ? TIG_BUTTON_FLAG_0x10
                : TIG_BUTTON_FLAG_0x02);
    }

    fate_ui_created = true;
}

// 0x56FE40
void fate_ui_destroy()
{
    if (fate_ui_created) {
        if (tig_window_destroy(fate_ui_window) == TIG_OK) {
            fate_ui_window = TIG_WINDOW_HANDLE_INVALID;
        }
        fate_ui_created = false;
    }
}

// 0x56FE70
bool fate_ui_message_filter(TigMessage* msg)
{
    int index;

    switch (msg->type) {
    case TIG_MESSAGE_BUTTON:
        for (index = 0; index < NUM_BUTTONS; index++) {
            if (stru_5CAAE8[index].button_handle == msg->data.button.button_handle) {
                if (msg->data.button.state == TIG_BUTTON_STATE_PRESSED
                    && !sub_4F50C0(qword_680ED8, index)) {
                    tig_button_state_change(stru_5CAAE8[index].button_handle, TIG_BUTTON_STATE_RELEASED);
                    tig_button_state_change(stru_5CAAE8[index].button_handle, TIG_BUTTON_STATE_MOUSE_INSIDE);
                    return true;
                }

                if (msg->data.button.state == TIG_BUTTON_STATE_RELEASED
                    && sub_4F51B0(qword_680ED8, index)) {
                    return true;
                }

                sub_551160();
                return true;
            }
        }
        break;
    case TIG_MESSAGE_KEYBOARD:
        if (msg->data.keyboard.key == DIK_SPACE
            && msg->data.keyboard.pressed == 1) {
            sub_56FC40();
            return true;
        }
    }

    return false;
}

// 0x56FF40
void sub_56FF40(long long obj, int btn)
{
    if (qword_680ED8 == obj) {
        tig_button_state_change(stru_5CAAE8[btn].button_handle, TIG_BUTTON_STATE_RELEASED);
    }
    sub_55A230();
}
