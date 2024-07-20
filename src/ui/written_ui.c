#include "ui/written_ui.h"

#include <tig/tig.h>

#include "game/lib/object.h"

#define TWO 2
#define FIVE 5
#define TEN 10

// 0x5CA478
static tig_window_handle_t written_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x5CA480
static TigRect stru_5CA480 = { 0, 41, 800, 399 };

static const char* off_5CA490[FIVE] = {
    "mes\\gamebook.mes",
    "mes\\gamenote.mes",
    "mes\\gamenewspaper.mes",
    "mes\\gametelegram.mes",
    "mes\\gameplaque.mes",
};

// 0x67BE00
static char written_ui_text[20000];

// 0x680DB0
static bool dword_680DB0;

// 0x680DB8
static int dword_680DB8[FIVE];

// 0x680DD8
static bool written_ui_mod_loaded;

// 0x680DDC
static bool written_ui_created;

// 0x56BAA0
bool written_ui_mod_load()
{
    int index;

    if (written_ui_mod_loaded) {
        return false;
    }

    for (index = 0; index < FIVE; index++) {
        message_load(off_5CA490[index], &(dword_680DB8[index]));
    }

    written_ui_mod_loaded = true;

    dword_680DB0 = false;

    if (dword_680DB8[2] != -1) {
        dword_67BC68 = message_count_in_range(dword_680DB8[2], 10, 999);
    } else {
        dword_67BC68 = 0;
    }

    return true;
}

// 0x56BB20
void written_ui_mod_unload()
{
    int index;

    if (!written_ui_mod_loaded) {
        return;
    }

    for (index = 0; index < FIVE; index++) {
        message_unload(dword_680DB8[index]);
        dword_680DB8[index] = -1;
    }

    written_ui_mod_loaded = false;
}

// 0x56BB60
void sub_56BB60(long long a1, long long a2)
{
    int subtype;
    int start;

    if (!written_ui_mod_loaded) {
        return;
    }

    if (sub_45D8D0(a2)) {
        return;
    }

    subtype = obj_f_get_int(a1, OBJ_F_WRITTEN_SUBTYPE);
    start = obj_f_get_int(a1, OBJ_F_WRITTEN_TEXT_START_LINE);

    if (subtype == 5) {
        sub_4B0120(a2, a1);
    } else {
        dword_680DB0 = sub_49B290(a1) == 14072;
        sub_4EE290(a2, subtype, start);
    }
}

// 0x56BC00
void sub_56BC00(int a1, int a2)
{
    if (!written_ui_mod_loaded) {
        return;
    }

    if (written_ui_created) {
        sub_56BC90();
        return;
    }

    if (a1 < 4) {
        dword_680DB4 = dword_680DB8[a1];
    } else if (a1 == 6) {
        dword_680DB4 = dword_680DB8[4];
    }

    if (dword_680DB4 == -1) {
        return;
    }

    if (!sub_551A80(0)) {
        return;
    }

    if (!sub_551A80(15)) {
        return;
    }

    dword_680DCC = a1;
    dword_67BC6C = a2;
    written_ui_text[0] = '\0';
    written_ui_create();
}

// 0x56BC90
void sub_56BC90()
{
    if (!written_ui_mod_loaded) {
        return;
    }

    if (!sub_551A80(0)) {
        return;
    }

    written_ui_destroy();
    dword_680DB0 = 0;
}

// 0x56BCC0
void written_ui_create()
{
    TigWindowData window_data;
    MesFileEntry mes_file_entry;
    Jack v1;
    long long obj;
    long long location;
    int index;

    if (!written_ui_mod_loaded
        || written_ui_created) {
        return;
    }

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.rect = stru_5CA480;
    window_data.message_filter_func = written_ui_message_filter;

    if (tig_window_create(&window_data, &written_ui_window) != TIG_OK) {
        tig_debug_printf("written_ui_create: ERROR: window create failed!\n");
        exit(0);
    }

    sub_56C590(494, 0, 0);

    obj = sub_40DA50();
    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    sub_4B8CE0(location);

    v1.window_handle = written_ui_window;
    v1.rect = &stru_5CA4A8;
    tig_art_interface_id_create(231, 0, 0, 0, &(v1.art_id));
    sub_550DA0(1, &v1);

    switch (dword_680DCC) {
    case 0:
        for (index = 0; index < TWO; index++) {
            sub_54AA60(written_ui_window, &stru_5CA480, &(stru_5CA4D8[index]), 9);
        }
        dword_680DD0 = 0;
        dword_680DD4 = 0;
        dword_67BC70 = dword_67BC6C;
        dword_680C20 = 0;
        break;
    case 1:
        for (index = 0; index < TEN; index++) {
            mes_file_entry.num = index + dword_67BC6C;
            if (!message_find(dword_680DB4, &mes_file_entry)) {
                break;
            }
            strcat(written_ui_text, mes_file_entry.text);
            strcat(written_ui_text, "\n");
        }
        break;
    case 3:
        for (index = 0; index < TEN; index++) {
            mes_file_entry.num = index + dword_67BC6C;
            if (!message_find(dword_680DB4, &mes_file_entry)) {
                break;
            }
            strcat(written_ui_text, mes_file_entry.text);

            mes_file_entry.num = index + dword_67BC6C;
            sub_4D43A0(dword_680DB8[3], &mes_file_entry);
            strcat(written_ui_text, mes_file_entry.text);
        }
    }

    sub_56C050();
    gsound_play_sfx_id(3008, 1);
    written_ui_created = true;
}

// 0x56BF60
void written_ui_destroy()
{
    if (!written_ui_mod_loaded
        || !written_ui_created) {
        return;
    }

    sub_550DA0(0, 0);

    if (tig_window_destroy(written_ui_window) == TIG_OK) {
        written_ui_window = TIG_WINDOW_HANDLE_INVALID;
    }

    gsound_play_sfx_id(3009, 1);
    written_ui_created = false;
}

// 0x56BFC0
bool written_ui_message_filter(TigMessage* msg)
{
    switch (msg->type) {
    case TIG_MESSAGE_MOUSE:
        if (msg->data.mouse.event == TIG_MOUSE_MESSAGE_LEFT_BUTTON_UP
            && sub_551000(msg->data.mouse.x, msg->data.mouse.y)) {
            sub_56BC90();
            return true;
        }
        break;
    case TIG_MESSAGE_BUTTON:
        if (msg->data.button.state == TIG_BUTTON_STATE_RELEASED) {
            if (stru_5CA4D8[0].button_handle == msg->data.button.button_handle) {
                dword_680DD0 -= 2;
                sub_56C050();
                gsound_play_sfx_id(3010, 1);
                return true;
            }

            if (stru_5CA4D8[1].button_handle == msg->data.button.button_handle) {
                dword_680DD0 += 2;
                sub_56C050();
                gsound_play_sfx_id(3010, 1);
                return true;
            }
        }
        break;
    }

    return false;
}

// 0x56C050
void sub_56C050()
{
    // TODO: Incomplete.
}

// 0x56C590
void sub_56C590(int num, int x, int y)
{
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitSpec blit_info;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = stru_5CA480.width;
    src_rect.height = stru_5CA480.height;

    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.width = src_rect.width;
    dst_rect.height = src_rect.height;

    blit_info.flags = 0;
    tig_art_interface_id_create(num, 0, 0, 0, &(blit_info.art_id));
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;

    tig_window_blit_art(written_ui_window, &blit_info);
}

// 0x56C630
void sub_56C630(const char* str, int num, int x, int y, int alignment)
{
    TigFont font_info;
    tig_font_handle_t font_handle;
    TigRect text_rect;

    font_info.flags = 0;
    tig_art_interface_id_create(num, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(0, 0, 0);
    tig_font_create(&font_info, &font_handle);
    tig_font_push(font_handle);

    font_info.str = str;
    font_info.width = 0;
    sub_535390(&font_info);

    switch (alignment) {
    case 0:
        text_rect.x = x;
        break;
    case 1:
        text_rect.x = x - font_info.width / 2;
        break;
    case 2:
        text_rect.x = x - font_info.width;
        break;
    }

    text_rect.y = y;
    text_rect.width = font_info.width;
    text_rect.height = font_info.height;
    tig_window_text_write(written_ui_window, str, &text_rect);
    tig_font_pop();
    tig_font_destroy(font_handle);
}

// 0x56C750
void sub_56C750(const char* str, int num, TigRect* rect)
{
    TigFont font_info;
    tig_font_handle_t font_handle;

    font_info.flags = 0;
    tig_art_interface_id_create(num, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(0, 0, 0);
    tig_font_create(&font_info, &font_handle);
    tig_font_push(font_handle);
    tig_window_text_write(written_ui_window, str, rect);
    tig_font_pop();
    tig_font_destroy(font_handle);
}

// 0x56C800
void sub_56C800()
{
    // TODO: Incomplete.
}

// 0x56C9F0
void sub_56C9F0()
{
    // TODO: Incomplete.
}

// 0x56CAA0
void sub_56CAA0()
{
    // TODO: Incomplete.
}

// 0x56CB60
void sub_56CB60()
{
    // TODO: Incomplete.
}

// 0x56CCA0
void sub_56CCA0()
{
    // TODO: Incomplete.
}
