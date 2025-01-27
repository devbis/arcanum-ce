#include "ui/fate_ui.h"

#include "game/critter.h"
#include "game/fate.h"
#include "game/mes.h"
#include "ui/charedit_ui.h"
#include "ui/intgame.h"
#include "ui/types.h"

static void fate_ui_create();
static void fate_ui_destroy();
static bool fate_ui_message_filter(TigMessage* msg);
static void fate_ui_handle_fate_resolved(int64_t obj, int fate);

// 0x5CAAE0
static tig_window_handle_t fate_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x5CAAE8
static UiButtonInfo fate_ui_buttons[FATE_COUNT] = {
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
static int64_t fate_ui_obj;

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
    fate_set_callback(fate_ui_handle_fate_resolved);

    return true;
}

// 0x56FBB0
void fate_ui_reset()
{
    if (fate_ui_created) {
        fate_ui_close();
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
void fate_ui_open(int64_t obj)
{
    if (fate_ui_created) {
        fate_ui_close();
        return;
    }

    if (obj == OBJ_HANDLE_NULL
        || critter_is_dead(obj)) {
        return;
    }

    fate_ui_obj = obj;
    fate_ui_create();
}

// 0x56FC40
void fate_ui_close()
{
    if (fate_ui_created) {
        fate_ui_destroy();
        fate_ui_obj = OBJ_HANDLE_NULL;
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

    window_data.flags = TIG_WINDOW_MESSAGE_FILTER;
    window_data.rect = window_rect;
    window_data.message_filter = fate_ui_message_filter;
    if (tig_window_create(&window_data, &fate_ui_window) != TIG_OK) {
        tig_debug_printf("fate_ui_create: ERROR: window create failed!\n");
        exit(EXIT_SUCCESS); // FIXME: Should be `EXIT_FAILURE`.
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

    for (int fate = 0; fate < FATE_COUNT; fate++) {
        mes_file_entry.num = fate;
        mes_get_msg(fate_ui_mes, &mes_file_entry);
        tig_window_text_write(fate_ui_window, mes_file_entry.str, &button_rect);
        button_rect.y += button_rect.height;
    }

    tig_font_pop();

    for (int fate = 0; fate < FATE_COUNT; fate++) {
        intgame_button_create_ex(fate_ui_window,
            &window_rect,
            &(fate_ui_buttons[fate]),
            fate_is_activated(fate_ui_obj, fate)
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
    switch (msg->type) {
    case TIG_MESSAGE_BUTTON:
        for (int fate = 0; fate < FATE_COUNT; fate++) {
            if (fate_ui_buttons[fate].button_handle == msg->data.button.button_handle) {
                if (msg->data.button.state == TIG_BUTTON_STATE_PRESSED
                    && !fate_activate(fate_ui_obj, fate)) {
                    tig_button_state_change(fate_ui_buttons[fate].button_handle, TIG_BUTTON_STATE_RELEASED);
                    tig_button_state_change(fate_ui_buttons[fate].button_handle, TIG_BUTTON_STATE_MOUSE_INSIDE);
                    return true;
                }

                if (msg->data.button.state == TIG_BUTTON_STATE_RELEASED
                    && fate_deactivate(fate_ui_obj, fate)) {
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
            fate_ui_close();
            return true;
        }
    }

    return false;
}

// 0x56FF40
void fate_ui_handle_fate_resolved(int64_t obj, int fate)
{
    if (fate_ui_obj == obj) {
        tig_button_state_change(fate_ui_buttons[fate].button_handle, TIG_BUTTON_STATE_RELEASED);
    }

    sub_55A230();
}
