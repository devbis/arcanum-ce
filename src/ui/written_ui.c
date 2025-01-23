#include "ui/written_ui.h"

#include "game/critter.h"
#include "game/gsound.h"
#include "game/location.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/proto.h"
#include "game/tech.h"
#include "ui/intgame.h"
#include "ui/types.h"

#define TWO 2
#define FIVE 5
#define TEN 10

typedef struct S5CA538 {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int x;
    /* 000C */ int y;
    /* 0010 */ int alignment;
} S5CA538;

static void written_ui_create();
static void written_ui_destroy();
static bool written_ui_message_filter(TigMessage* msg);
static void sub_56C050();
static void sub_56C590(int num, int x, int y);
static void sub_56C630(const char* str, int num, int x, int y, int alignment);
static void sub_56C750(const char* str, int num, TigRect* rect);
static bool sub_56C800(char* str, int font_num, int centered, TigRect* rect, int* height_ptr);
static void sub_56C9F0(char* str, int* font_ptr, int* centered_ptr, char** str_ptr);
static void sub_56CAA0(int a1, int* a2, int* a3);
static int sub_56CB60(TigRect* rect, int* num_ptr, int* height_ptr);

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

// 0x5CA4A8
static TigRect stru_5CA4A8 = { 25, 24, 89, 89 };

// 0x5CA4B8
static int dword_5CA4B8[8] = {
    477,
    489,
    487,
    491,
    0,
    0,
    633,
    0,
};

// 0x5CA4D8
static UiButtonInfo stru_5CA4D8[2] = {
    { 213, 77, 495, TIG_BUTTON_HANDLE_INVALID },
    { 675, 77, 496, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5CA4F8
static TigRect stru_5CA4F8 = { 349, 49, 215, 300 };

// 0x5CA508
static TigRect stru_5CA508 = { 327, 126, 250, 240 };

// 0x5CA518
static TigRect stru_5CA518 = { 335, 90, 234, 20 };

// 0x5CA528
static TigRect stru_5CA528 = { 269, 40, 428, 327 };

// 0x5CA538
static S5CA538 stru_5CA538[11] = {
    { 481, 1000, 342, 35, 1 },
    { 481, 1001, 342, 41, 1 },
    { 481, 1002, 321, 50, 0 },
    { 481, 1003, 321, 59, 0 },
    { 481, 1004, 321, 68, 0 },
    { 479, 1005, 450, 36, 1 },
    { 479, 1006, 450, 61, 1 },
    { 481, 1007, 558, 35, 1 },
    { 481, 1008, 539, 50, 0 },
    { 481, 1009, 539, 60, 0 },
    { 481, 1010, 539, 69, 0 },
};

// 0x5CA618
static TigRect stru_5CA618[2] = {
    { 227, 57, 200, 290 },
    { 476, 57, 200, 290 },
};

// 0x5CA638
static S5CA538 stru_5CA638[6] = {
    { 485, 0, 216, 44, 1 },
    { 484, 1, 453, 41, 1 },
    { 485, 0, 689, 44, 1 },
    { 482, 2, 194, 76, 0 },
    { 482, 3, 449, 76, 1 },
    { 482, 2, 712, 76, 2 },
};

// 0x5CA6B0
static S5CA538 stru_5CA6B0[6] = {
    { 485, 0, 216, 44, 1 },
    { 815, 4, 453, 41, 1 },
    { 485, 0, 689, 44, 1 },
    { 482, 2, 194, 76, 0 },
    { 482, 5, 449, 76, 1 },
    { 482, 2, 712, 76, 2 },
};

// 0x5CA728
static TigRect stru_5CA728[5] = {
    { 186, 97, 255, 90 },
    { 188, 196, 120, 175 },
    { 328, 196, 120, 175 },
    { 460, 103, 120, 270 },
    { 592, 103, 120, 270 },
};

// 0x67BC68
static int dword_67BC68;

// 0x67BC6C
static int dword_67BC6C;

// 0x67BC70
static int dword_67BC70[100];

// 0x67BE00
static char written_ui_text[20000];

// 0x680C20
static int dword_680C20[100];

// 0x680DB0
static bool dword_680DB0;

// 0x680DB4
static int dword_680DB4;

// 0x680DB8
static mes_file_handle_t dword_680DB8[FIVE];

// 0x680DCC
static int dword_680DCC;

// 0x680DD0
static int dword_680DD0;

// 0x680DD4
static int dword_680DD4;

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
        mes_load(off_5CA490[index], &(dword_680DB8[index]));
    }

    written_ui_mod_loaded = true;

    dword_680DB0 = false;

    if (dword_680DB8[2] != -1) {
        dword_67BC68 = mes_entries_count_in_range(dword_680DB8[2], 10, 999);
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
        mes_unload(dword_680DB8[index]);
        dword_680DB8[index] = MES_FILE_HANDLE_INVALID;
    }

    written_ui_mod_loaded = false;
}

// 0x56BB60
void sub_56BB60(int64_t written_obj, int64_t pc_obj)
{
    int subtype;
    int start;

    if (!written_ui_mod_loaded) {
        return;
    }

    if (critter_is_dead(pc_obj)) {
        return;
    }

    subtype = obj_field_int32_get(written_obj, OBJ_F_WRITTEN_SUBTYPE);
    start = obj_field_int32_get(written_obj, OBJ_F_WRITTEN_TEXT_START_LINE);

    if (subtype == 5) {
        tech_learn_schematic(pc_obj, written_obj);
    } else {
        dword_680DB0 = sub_49B290(written_obj) == 14072;
        sub_4EE290(pc_obj, subtype, start);
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
    S550DA0 v1;
    long long obj;
    long long location;
    int index;

    if (!written_ui_mod_loaded
        || written_ui_created) {
        return;
    }

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.rect = stru_5CA480;
    window_data.message_filter = written_ui_message_filter;

    if (tig_window_create(&window_data, &written_ui_window) != TIG_OK) {
        tig_debug_printf("written_ui_create: ERROR: window create failed!\n");
        exit(0);
    }

    sub_56C590(494, 0, 0);

    obj = player_get_pc_obj();
    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    location_origin_set(location);

    v1.window_handle = written_ui_window;
    v1.rect = &stru_5CA4A8;
    tig_art_interface_id_create(231, 0, 0, 0, &(v1.art_id));
    sub_550DA0(1, &v1);

    switch (dword_680DCC) {
    case 0:
        for (index = 0; index < TWO; index++) {
            intgame_button_create_ex(written_ui_window, &stru_5CA480, &(stru_5CA4D8[index]), 9);
        }
        dword_680DD0 = 0;
        dword_680DD4 = 0;
        dword_67BC70[0] = dword_67BC6C;
        dword_680C20[0] = 0;
        break;
    case 1:
        for (index = 0; index < TEN; index++) {
            mes_file_entry.num = index + dword_67BC6C;
            if (!mes_search(dword_680DB4, &mes_file_entry)) {
                break;
            }
            strcat(written_ui_text, mes_file_entry.str);
            strcat(written_ui_text, "\n");
        }
        break;
    case 3:
        for (index = 0; index < TEN; index++) {
            mes_file_entry.num = index + dword_67BC6C;
            if (!mes_search(dword_680DB4, &mes_file_entry)) {
                break;
            }
            strcat(written_ui_text, mes_file_entry.str);

            mes_file_entry.num = index + dword_67BC6C;
            mes_get_msg(dword_680DB8[3], &mes_file_entry);
            strcat(written_ui_text, mes_file_entry.str);
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
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP
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
    int v1;
    int v2;
    int v3;
    S5CA538* v4;
    MesFileEntry mes_file_entry;
    int index;
    int width;
    int height;
    int font;
    int centered;
    char* str;
    TigRect rect;

    if (dword_680DCC == 0) {
        tig_button_hide(stru_5CA4D8[0].button_handle);
        tig_button_hide(stru_5CA4D8[1].button_handle);
    }

    if (dword_680DCC == 4) {
        v1 = dword_67BC6C;
    } else if (dword_680DB0) {
        v1 = 817;
    } else {
        v1 = dword_5CA4B8[dword_680DCC];
    }

    sub_56C590(v1, 132, 0);

    switch (dword_680DCC) {
    case 0:
        v2 = dword_67BC70[dword_680DD0];
        v3 = dword_680C20[dword_680DD0];
        sub_56CAA0(0, &v2, &v3);
        sub_56CAA0(1, &v2, &v3);
        if (dword_680DD0 > 0) {
            tig_button_show(stru_5CA4D8[0].button_handle);
        }
        if (dword_680DD0 < dword_680DD4 - 1 && dword_680DD4 < 100) {
            tig_button_show(stru_5CA4D8[1].button_handle);
        }
        break;
    case 1:
        sub_56C750(written_ui_text, 497, &stru_5CA4F8);
        break;
    case 2:
        v4 = !dword_680DB0 ? stru_5CA638 : stru_5CA6B0;
        for (index = 0; index < 6; index++) {
            mes_file_entry.num = v4[index].field_4;
            mes_get_msg(dword_680DB8[2], &mes_file_entry);
            sub_56C630(mes_file_entry.str,
                v4[index].field_0,
                v4[index].x,
                v4[index].y,
                v4[index].alignment);
        }

        mes_file_entry.num = dword_67BC6C;
        if (mes_search(dword_680DB4, &mes_file_entry)) {
            sub_56C9F0(mes_file_entry.str, &font, &centered, &str);
            sub_56C800(str, font, centered, &(stru_5CA728[0]), &height);
        }

        mes_file_entry.num++;
        if (mes_search(dword_680DB4, &mes_file_entry)) {
            sub_56C9F0(mes_file_entry.str, &font, &centered, &str);

            index = 1;
            rect = stru_5CA728[index];
            while (true) {
                while (!sub_56C800(str, font, centered, &rect, &height)) {
                    index++;
                    if (index >= 5) {
                        return;
                    }

                    rect = stru_5CA728[index];
                    str += height;
                }

                rect.height -= height;
                rect.y += height;

                mes_file_entry.num++;
                if (mes_file_entry.num >= dword_67BC6C + 10) {
                    break;
                }

                if (!mes_search(dword_680DB4, &mes_file_entry)) {
                    break;
                }

                sub_56C9F0(mes_file_entry.str, &font, &centered, &str);
            }

            if (index < 5) {
                mes_file_entry.num = dword_67BC6C / 10 + 10;
                if (mes_file_entry.num >= dword_67BC68 + 10) {
                    mes_file_entry.num = 10;
                }

                if (mes_search(dword_680DB8[2], &mes_file_entry)) {
                    sub_56C9F0(mes_file_entry.str, &font, &centered, &str);

                    while (true) {
                        while (!sub_56C800(str, font, centered, &rect, &height)) {
                            index++;
                            if (index >= 5) {
                                return;
                            }

                            rect = stru_5CA728[index];
                            str += height;
                        }

                        mes_file_entry.num++;
                        if (mes_file_entry.num >= dword_67BC68 + 10) {
                            mes_file_entry.num = 10;
                        }

                        if (!mes_search(dword_680DB8[2], &mes_file_entry)) {
                            break;
                        }

                        sub_56C9F0(mes_file_entry.str, &font, &centered, &str);
                        rect.height -= height;
                        rect.y += height;
                    }
                }
            }
        }
        break;
    case 3:
        for (index = 0; index < 11; index++) {
            mes_file_entry.num = stru_5CA538[index].field_4;
            mes_get_msg(dword_680DB8[3], &mes_file_entry);
            sub_56C630(mes_file_entry.str,
                stru_5CA538[index].field_0,
                stru_5CA538[index].x,
                stru_5CA538[index].y,
                stru_5CA538[index].alignment);
        }

        mes_file_entry.num = 1011;
        mes_get_msg(dword_680DB8[3], &mes_file_entry);
        sub_56C750(mes_file_entry.str, 481, &stru_5CA518);
        sub_56C750(written_ui_text, 480, &stru_5CA508);
        break;
    case 6:
        width = dword_67BC6C;
        height = 0;
        sub_56CB60(&stru_5CA528, &width, &height);
        break;
    }
}

// 0x56C590
void sub_56C590(int num, int x, int y)
{
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitInfo blit_info;

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
    tig_font_measure(&font_info);

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
bool sub_56C800(char* str, int font_num, int centered, TigRect* rect, int* height_ptr)
{
    TigFont font_desc;
    tig_font_handle_t font;
    bool rc;

    font_desc.flags = centered == 1 ? TIG_FONT_CENTERED : 0;
    tig_art_interface_id_create(font_num, 0, 0, 0, &(font_desc.art_id));

    if (dword_680DCC == 6) {
        font_desc.color = tig_color_make(255, 255, 255);
    } else {
        font_desc.color = tig_color_make(0, 0, 0);
    }

    font_desc.str = NULL;

    tig_font_create(&font_desc, &font);
    tig_font_push(font);

    font_desc.str = str;
    font_desc.width = rect->width;
    tig_font_measure(&font_desc);

    if (font_desc.height > rect->height) {
        size_t truncate_pos = 0;
        size_t end = strlen(str);
        size_t pos;
        char ch;

        for (pos = 0; pos < end; pos++) {
            ch = str[pos];
            if (ch == ' ' || ch == '\n') {
                str[pos] = '\0';
                tig_font_measure(&font_desc);
                str[pos] = ch;

                if (font_desc.height > rect->height) {
                    break;
                }

                truncate_pos = pos;
            }
        }

        if (truncate_pos > 0) {
            ch = str[truncate_pos];
            str[truncate_pos] = '\0';
            tig_window_text_write(written_ui_window, str, rect);
            str[truncate_pos] = ch;
        }

        *height_ptr = (int)truncate_pos;
        rc = false;
    } else {
        tig_window_text_write(written_ui_window, str, rect);
        *height_ptr = font_desc.height;
        rc = true;
    }

    tig_font_pop();
    tig_font_destroy(font);

    return rc;
}

// 0x56C9F0
void sub_56C9F0(char* str, int* font_ptr, int* centered_ptr, char** str_ptr)
{
    *font_ptr = atoi(str);

    *str_ptr = str;
    while (isdigit(**str_ptr)) {
        (*str_ptr)++;
    }

    *centered_ptr = **str_ptr == 'c' || **str_ptr == 'C';

    if (**str_ptr != '\0') {
        (*str_ptr)++;
    }

    if (isspace(**str_ptr)) {
        (*str_ptr)++;
    }
}

// 0x56CAA0
void sub_56CAA0(int a1, int* a2, int* a3)
{
    TigRect rect;
    int v1;
    MesFileEntry mes_file_entry;

    rect = stru_5CA618[a1];
    v1 = sub_56CB60(&rect, a2, a3);
    if (v1 == -1) {
        *a3 += v1;
        mes_file_entry.num = *a2;
        if (mes_file_entry.num < dword_67BC6C + 10
            && mes_search(dword_680DB4, &mes_file_entry)) {
            dword_67BC70[dword_680DD0 + a1 + 1] = mes_file_entry.num;
            dword_680C20[dword_680DD0 + a1 + 1] = *a3;
            if (dword_680DD4 == dword_680DD0 + a1) {
                dword_680DD4++;
            }
        }
    }
}

// 0x56CB60
int sub_56CB60(TigRect* rect, int* num_ptr, int* height_ptr)
{
    MesFileEntry mes_file_entry;
    int font;
    int centered;
    char* str;
    TigRect v4;
    int v1;

    mes_file_entry.num = *num_ptr;
    if (mes_file_entry.num >= dword_67BC6C + 10) {
        return -1;
    }

    if (mes_search(dword_680DB4, &mes_file_entry)) {
        return -1;
    }

    sub_56C9F0(mes_file_entry.str, &font, &centered, &str);

    v1 = *height_ptr;
    str += *height_ptr;
    v4 = *rect;
    if (sub_56C800(str, font, centered, &v4, height_ptr)) {
        v1 = 0;
        do {
            v4.y += *height_ptr;
            v4.height -= *height_ptr;
            mes_file_entry.num++;
            if (mes_file_entry.num >= dword_67BC6C + 10) {
                break;
            }

            if (!mes_search(dword_680DB4, &mes_file_entry)) {
                break;
            }

            sub_56C9F0(mes_file_entry.str, &font, &centered, &str);
        } while (sub_56C800(str, font, centered, &v4, height_ptr));
    }

    *num_ptr = mes_file_entry.num;
    return v1;
}

// 0x56CCA0
void sub_56CCA0(int num, char* str)
{
    MesFileEntry mes_file_entry;
    int font;
    int centered;
    char* tmp;
    char* pch;

    if (!written_ui_mod_loaded) {
        return;
    }

    mes_file_entry.num = num;
    str[0] = '\0';

    if (dword_680DB8[2] == MES_FILE_HANDLE_INVALID) {
        return;
    }

    if (!mes_search(dword_680DB8[2], &mes_file_entry)) {
        return;
    }

    sub_56C9F0(mes_file_entry.str, &font, &centered, &tmp);

    strcpy(str, tmp);

    pch = str;
    while (*pch != '\0') {
        if (*pch == '\n') {
            *pch = ' ';
        }
        pch++;
    }
}
