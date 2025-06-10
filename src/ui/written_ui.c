#include "ui/written_ui.h"

#include "game/critter.h"
#include "game/gsound.h"
#include "game/hrp.h"
#include "game/location.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/proto.h"
#include "game/tech.h"
#include "ui/intgame.h"
#include "ui/types.h"

#define TEN 10

typedef enum WrittenMes {
    WRITTEN_MES_BOOK,
    WRITTEN_MES_NOTE,
    WRITTEN_MES_NEWSPAPER,
    WRITTEN_MES_TELEGRAM,
    WRITTEN_MES_PLAQUE,
    WRITTEN_MES_COUNT,
} WrittenMes;

typedef enum WrittenTextAlignment {
    WRITTEN_TEXT_ALIGNMENT_LEFT,
    WRITTEN_TEXT_ALIGNMENT_CENTER,
    WRITTEN_TEXT_ALIGNMENT_RIGHT,
} WrittenTextAlignment;

typedef enum WrittenUiBookButton {
    WRITTEN_UI_BOOK_BUTTON_PREV,
    WRITTEN_UI_BOOK_BUTTON_NEXT,
    WRITTEN_UI_BOOK_BUTTON_COUNT,
} WrittenUiBookButton;

typedef struct WrittenUiElement {
    /* 0000 */ int font_num;
    /* 0004 */ int message_num;
    /* 0008 */ int x;
    /* 000C */ int y;
    /* 0010 */ WrittenTextAlignment alignment;
} WrittenUiElement;

static void written_ui_create();
static void written_ui_destroy();
static bool written_ui_message_filter(TigMessage* msg);
static void written_ui_refresh();
static void written_ui_draw_background(int num, int x, int y);
static void written_ui_draw_element(const char* str, int font_num, int x, int y, WrittenTextAlignment alignment);
static void written_ui_draw_paragraph(const char* str, int font_num, TigRect* rect);
static bool sub_56C800(char* str, int font_num, int centered, TigRect* rect, int* height_ptr);
static void written_ui_parse(char* str, int* font_num_ptr, int* centered_ptr, char** str_ptr);
static void sub_56CAA0(int a1, int* a2, int* a3);
static int sub_56CB60(TigRect* rect, int* num_ptr, int* height_ptr);

// 0x5CA478
static tig_window_handle_t written_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x5CA480
static TigRect written_ui_background_frame = { 0, 41, 800, 399 };

static const char* off_5CA490[WRITTEN_MES_COUNT] = {
    "mes\\gamebook.mes",
    "mes\\gamenote.mes",
    "mes\\gamenewspaper.mes",
    "mes\\gametelegram.mes",
    "mes\\gameplaque.mes",
};

// 0x5CA4A8
static TigRect stru_5CA4A8 = { 25, 24, 89, 89 };

// 0x5CA4B8
static int dword_5CA4B8[WRITTEN_TYPE_COUNT] = {
    /*      WRITTEN_TYPE_BOOK */ 477, // "bookbackground.art"
    /*      WRITTEN_TYPE_NOTE */ 489, // "notebackground.art"
    /* WRITTEN_TYPE_NEWSPAPER */ 487, // "newsbackground.art"
    /*  WRITTEN_TYPE_TELEGRAM */ 491, // "telebackground.art"
    /*     WRITTEN_TYPE_IMAGE */ 0,
    /* WRITTEN_TYPE_SCHEMATIC */ 0,
    /*    WRITTEN_TYPE_PLAQUE */ 633, // "stone_plaque.art"
};

// 0x5CA4D8
static UiButtonInfo written_ui_book_buttons[WRITTEN_UI_BOOK_BUTTON_COUNT] = {
    /* WRITTEN_UI_BOOK_BUTTON_PREV */ { 213, 77, 495, TIG_BUTTON_HANDLE_INVALID },
    /* WRITTEN_UI_BOOK_BUTTON_NEXT */ { 675, 77, 496, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5CA4F8
static TigRect written_ui_note_content_rect = { 349, 49, 215, 300 };

// 0x5CA508
static TigRect written_ui_telegram_content_rect = { 327, 126, 250, 240 };

// 0x5CA518
static TigRect written_ui_telegram_disclaimer_rect = { 335, 90, 234, 20 };

// 0x5CA528
static TigRect written_ui_plaque_content_rect = { 269, 40, 428, 327 };

// 0x5CA538
static WrittenUiElement written_ui_telegram_elements[] = {
    { 481, 1000, 342, 35, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 481, 1001, 342, 41, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 481, 1002, 321, 50, WRITTEN_TEXT_ALIGNMENT_LEFT },
    { 481, 1003, 321, 59, WRITTEN_TEXT_ALIGNMENT_LEFT },
    { 481, 1004, 321, 68, WRITTEN_TEXT_ALIGNMENT_LEFT },
    { 479, 1005, 450, 36, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 479, 1006, 450, 61, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 481, 1007, 558, 35, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 481, 1008, 539, 50, WRITTEN_TEXT_ALIGNMENT_LEFT },
    { 481, 1009, 539, 60, WRITTEN_TEXT_ALIGNMENT_LEFT },
    { 481, 1010, 539, 69, WRITTEN_TEXT_ALIGNMENT_LEFT },
};

// 0x5CA618
static TigRect stru_5CA618[2] = {
    { 227, 57, 200, 290 },
    { 476, 57, 200, 290 },
};

// 0x5CA638
static WrittenUiElement written_ui_newspaper_tarantian_elements[] = {
    { 485, 0, 216, 44, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 484, 1, 453, 41, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 485, 0, 689, 44, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 482, 2, 194, 76, WRITTEN_TEXT_ALIGNMENT_LEFT },
    { 482, 3, 449, 76, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 482, 2, 712, 76, WRITTEN_TEXT_ALIGNMENT_RIGHT },
};

// 0x5CA6B0
static WrittenUiElement written_ui_vendigroth_times_elements[] = {
    { 485, 0, 216, 44, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 815, 4, 453, 41, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 485, 0, 689, 44, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 482, 2, 194, 76, WRITTEN_TEXT_ALIGNMENT_LEFT },
    { 482, 5, 449, 76, WRITTEN_TEXT_ALIGNMENT_CENTER },
    { 482, 2, 712, 76, WRITTEN_TEXT_ALIGNMENT_RIGHT },
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
static int written_ui_num;

// 0x67BC70
static int dword_67BC70[100];

// 0x67BE00
static char written_ui_text[20000];

// 0x680C20
static int dword_680C20[100];

// 0x680DB0
static bool written_ui_is_vendigroth_times;

// 0x680DB4
static mes_file_handle_t dword_680DB4;

// 0x680DB8
static mes_file_handle_t written_ui_mes_files[WRITTEN_MES_COUNT];

// 0x680DCC
static WrittenType written_ui_type;

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

    for (index = 0; index < WRITTEN_MES_COUNT; index++) {
        mes_load(off_5CA490[index], &(written_ui_mes_files[index]));
    }

    written_ui_mod_loaded = true;

    written_ui_is_vendigroth_times = false;

    if (written_ui_mes_files[WRITTEN_MES_NEWSPAPER] != MES_FILE_HANDLE_INVALID) {
        dword_67BC68 = mes_entries_count_in_range(written_ui_mes_files[WRITTEN_MES_NEWSPAPER], 10, 999);
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

    for (index = 0; index < WRITTEN_MES_COUNT; index++) {
        mes_unload(written_ui_mes_files[index]);
        written_ui_mes_files[index] = MES_FILE_HANDLE_INVALID;
    }

    written_ui_mod_loaded = false;
}

// 0x56BB60
void written_ui_start_obj(int64_t written_obj, int64_t pc_obj)
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

    if (subtype == WRITTEN_TYPE_SCHEMATIC) {
        tech_learn_schematic(pc_obj, written_obj);
    } else {
        written_ui_is_vendigroth_times = sub_49B290(written_obj) == 14072;
        mp_ui_written_start_type(pc_obj, subtype, start);
    }
}

// 0x56BC00
void written_ui_start_type(WrittenType written_type, int num)
{
    if (!written_ui_mod_loaded) {
        return;
    }

    if (written_ui_created) {
        written_ui_close();
        return;
    }

    if (written_type < 4) {
        dword_680DB4 = written_ui_mes_files[written_type];
    } else if (written_type == WRITTEN_TYPE_PLAQUE) {
        dword_680DB4 = written_ui_mes_files[WRITTEN_MES_PLAQUE];
    }

    if (dword_680DB4 == MES_FILE_HANDLE_INVALID) {
        return;
    }

    if (!sub_551A80(0)) {
        return;
    }

    if (!sub_551A80(15)) {
        return;
    }

    written_ui_type = written_type;
    written_ui_num = num;
    written_ui_text[0] = '\0';
    written_ui_create();
}

// 0x56BC90
void written_ui_close()
{
    if (!written_ui_mod_loaded) {
        return;
    }

    if (!sub_551A80(0)) {
        return;
    }

    written_ui_destroy();
    written_ui_is_vendigroth_times = false;
}

// 0x56BCC0
void written_ui_create()
{
    TigWindowData window_data;
    MesFileEntry mes_file_entry;
    PcLens pc_lens;
    int64_t obj;
    int64_t location;
    int index;

    if (!written_ui_mod_loaded
        || written_ui_created) {
        return;
    }

    window_data.flags = TIG_WINDOW_MESSAGE_FILTER;
    window_data.rect = written_ui_background_frame;
    window_data.message_filter = written_ui_message_filter;
    hrp_apply(&(window_data.rect), GRAVITY_CENTER_HORIZONTAL | GRAVITY_CENTER_VERTICAL);

    if (tig_window_create(&window_data, &written_ui_window) != TIG_OK) {
        tig_debug_printf("written_ui_create: ERROR: window create failed!\n");
        exit(0);
    }

    written_ui_draw_background(494, 0, 0);

    obj = player_get_local_pc_obj();
    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    location_origin_set(location);

    pc_lens.window_handle = written_ui_window;
    pc_lens.rect = &stru_5CA4A8;
    tig_art_interface_id_create(231, 0, 0, 0, &(pc_lens.art_id));
    intgame_pc_lens_do(PC_LENS_MODE_PASSTHROUGH, &pc_lens);

    switch (written_ui_type) {
    case WRITTEN_TYPE_BOOK:
        for (index = 0; index < WRITTEN_UI_BOOK_BUTTON_COUNT; index++) {
            intgame_button_create_ex(written_ui_window,
                &written_ui_background_frame,
                &(written_ui_book_buttons[index]),
                TIG_BUTTON_FLAG_HIDDEN | TIG_BUTTON_FLAG_0x01);
        }
        dword_680DD0 = 0;
        dword_680DD4 = 0;
        dword_67BC70[0] = written_ui_num;
        dword_680C20[0] = 0;
        break;
    case WRITTEN_TYPE_NOTE:
        for (index = 0; index < TEN; index++) {
            mes_file_entry.num = index + written_ui_num;
            if (!mes_search(dword_680DB4, &mes_file_entry)) {
                break;
            }
            strcat(written_ui_text, mes_file_entry.str);
            strcat(written_ui_text, "\n");
        }
        break;
    case WRITTEN_TYPE_TELEGRAM:
        for (index = 0; index < TEN; index++) {
            mes_file_entry.num = index + written_ui_num;
            if (!mes_search(dword_680DB4, &mes_file_entry)) {
                break;
            }
            strcat(written_ui_text, mes_file_entry.str);

            mes_file_entry.num = index + written_ui_num;
            mes_get_msg(written_ui_mes_files[WRITTEN_MES_TELEGRAM], &mes_file_entry);
            strcat(written_ui_text, mes_file_entry.str);
        }
        break;
    }

    written_ui_refresh();
    gsound_play_sfx(3008, 1);
    written_ui_created = true;
}

// 0x56BF60
void written_ui_destroy()
{
    if (!written_ui_mod_loaded
        || !written_ui_created) {
        return;
    }

    intgame_pc_lens_do(PC_LENS_MODE_NONE, NULL);

    if (tig_window_destroy(written_ui_window) == TIG_OK) {
        written_ui_window = TIG_WINDOW_HANDLE_INVALID;
    }

    gsound_play_sfx(3009, 1);
    written_ui_created = false;
}

// 0x56BFC0
bool written_ui_message_filter(TigMessage* msg)
{
    switch (msg->type) {
    case TIG_MESSAGE_MOUSE:
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP
            && intgame_pc_lens_check_pt(msg->data.mouse.x, msg->data.mouse.y)) {
            written_ui_close();
            return true;
        }
        break;
    case TIG_MESSAGE_BUTTON:
        if (msg->data.button.state == TIG_BUTTON_STATE_RELEASED) {
            if (written_ui_book_buttons[WRITTEN_UI_BOOK_BUTTON_PREV].button_handle == msg->data.button.button_handle) {
                dword_680DD0 -= 2;
                written_ui_refresh();
                gsound_play_sfx(3010, 1);
                return true;
            }

            if (written_ui_book_buttons[WRITTEN_UI_BOOK_BUTTON_NEXT].button_handle == msg->data.button.button_handle) {
                dword_680DD0 += 2;
                written_ui_refresh();
                gsound_play_sfx(3010, 1);
                return true;
            }
        }
        break;
    }

    return false;
}

// 0x56C050
void written_ui_refresh()
{
    int art_num;
    int v2;
    int v3;
    WrittenUiElement* elements;
    int cnt;
    MesFileEntry mes_file_entry;
    int index;
    int width;
    int height;
    int font_num;
    int centered;
    char* str;
    TigRect rect;

    if (written_ui_type == WRITTEN_TYPE_BOOK) {
        tig_button_hide(written_ui_book_buttons[WRITTEN_UI_BOOK_BUTTON_PREV].button_handle);
        tig_button_hide(written_ui_book_buttons[WRITTEN_UI_BOOK_BUTTON_NEXT].button_handle);
    }

    if (written_ui_type == WRITTEN_TYPE_IMAGE) {
        art_num = written_ui_num;
    } else if (written_ui_is_vendigroth_times) {
        art_num = 817; // "vendnewsback.art"
    } else {
        art_num = dword_5CA4B8[written_ui_type];
    }

    written_ui_draw_background(art_num, 132, 0);

    switch (written_ui_type) {
    case WRITTEN_TYPE_BOOK:
        v2 = dword_67BC70[dword_680DD0];
        v3 = dword_680C20[dword_680DD0];
        sub_56CAA0(0, &v2, &v3);
        sub_56CAA0(1, &v2, &v3);
        if (dword_680DD0 > 0) {
            tig_button_show(written_ui_book_buttons[WRITTEN_UI_BOOK_BUTTON_PREV].button_handle);
        }
        if (dword_680DD0 < dword_680DD4 - 1 && dword_680DD4 < 100) {
            tig_button_show(written_ui_book_buttons[WRITTEN_UI_BOOK_BUTTON_NEXT].button_handle);
        }
        break;
    case WRITTEN_TYPE_NOTE:
        written_ui_draw_paragraph(written_ui_text, 497, &written_ui_note_content_rect);
        break;
    case WRITTEN_TYPE_NEWSPAPER:
        if (written_ui_is_vendigroth_times) {
            elements = written_ui_vendigroth_times_elements;
            cnt = SDL_arraysize(written_ui_vendigroth_times_elements);
        } else {
            elements = written_ui_newspaper_tarantian_elements;
            cnt = SDL_arraysize(written_ui_newspaper_tarantian_elements);
        }
        for (index = 0; index < cnt; index++) {
            mes_file_entry.num = elements[index].message_num;
            mes_get_msg(written_ui_mes_files[WRITTEN_MES_NEWSPAPER], &mes_file_entry);
            written_ui_draw_element(mes_file_entry.str,
                elements[index].font_num,
                elements[index].x,
                elements[index].y,
                elements[index].alignment);
        }

        mes_file_entry.num = written_ui_num;
        if (mes_search(dword_680DB4, &mes_file_entry)) {
            written_ui_parse(mes_file_entry.str, &font_num, &centered, &str);
            sub_56C800(str, font_num, centered, &(stru_5CA728[0]), &height);
        }

        mes_file_entry.num++;
        if (mes_search(dword_680DB4, &mes_file_entry)) {
            written_ui_parse(mes_file_entry.str, &font_num, &centered, &str);

            index = 1;
            rect = stru_5CA728[index];
            while (true) {
                while (!sub_56C800(str, font_num, centered, &rect, &height)) {
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
                if (mes_file_entry.num >= written_ui_num + 10) {
                    break;
                }

                if (!mes_search(dword_680DB4, &mes_file_entry)) {
                    break;
                }

                written_ui_parse(mes_file_entry.str, &font_num, &centered, &str);
            }

            if (index < 5) {
                mes_file_entry.num = written_ui_num / 10 + 10;
                if (mes_file_entry.num >= dword_67BC68 + 10) {
                    mes_file_entry.num = 10;
                }

                if (mes_search(written_ui_mes_files[WRITTEN_MES_NEWSPAPER], &mes_file_entry)) {
                    written_ui_parse(mes_file_entry.str, &font_num, &centered, &str);

                    while (true) {
                        while (!sub_56C800(str, font_num, centered, &rect, &height)) {
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

                        if (!mes_search(written_ui_mes_files[WRITTEN_MES_NEWSPAPER], &mes_file_entry)) {
                            break;
                        }

                        written_ui_parse(mes_file_entry.str, &font_num, &centered, &str);
                        rect.height -= height;
                        rect.y += height;
                    }
                }
            }
        }
        break;
    case WRITTEN_TYPE_TELEGRAM:
        cnt = SDL_arraysize(written_ui_telegram_elements);
        for (index = 0; index < cnt; index++) {
            mes_file_entry.num = written_ui_telegram_elements[index].message_num;
            mes_get_msg(written_ui_mes_files[WRITTEN_MES_TELEGRAM], &mes_file_entry);
            written_ui_draw_element(mes_file_entry.str,
                written_ui_telegram_elements[index].font_num,
                written_ui_telegram_elements[index].x,
                written_ui_telegram_elements[index].y,
                written_ui_telegram_elements[index].alignment);
        }

        mes_file_entry.num = 1011;
        mes_get_msg(written_ui_mes_files[WRITTEN_MES_TELEGRAM], &mes_file_entry);
        written_ui_draw_paragraph(mes_file_entry.str, 481, &written_ui_telegram_disclaimer_rect);
        written_ui_draw_paragraph(written_ui_text, 480, &written_ui_telegram_content_rect);
        break;
    case WRITTEN_TYPE_PLAQUE:
        width = written_ui_num;
        height = 0;
        sub_56CB60(&written_ui_plaque_content_rect, &width, &height);
        break;
    }
}

// 0x56C590
void written_ui_draw_background(int num, int x, int y)
{
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitInfo blit_info;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = written_ui_background_frame.width;
    src_rect.height = written_ui_background_frame.height;

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
void written_ui_draw_element(const char* str, int font_num, int x, int y, WrittenTextAlignment alignment)
{
    TigFont font_info;
    tig_font_handle_t font_handle;
    TigRect text_rect;

    font_info.flags = 0;
    tig_art_interface_id_create(font_num, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(0, 0, 0);
    tig_font_create(&font_info, &font_handle);
    tig_font_push(font_handle);

    font_info.str = str;
    font_info.width = 0;
    tig_font_measure(&font_info);

    switch (alignment) {
    case WRITTEN_TEXT_ALIGNMENT_LEFT:
        text_rect.x = x;
        break;
    case WRITTEN_TEXT_ALIGNMENT_CENTER:
        text_rect.x = x - font_info.width / 2;
        break;
    case WRITTEN_TEXT_ALIGNMENT_RIGHT:
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
void written_ui_draw_paragraph(const char* str, int font_num, TigRect* rect)
{
    TigFont font_info;
    tig_font_handle_t font_handle;

    font_info.flags = 0;
    tig_art_interface_id_create(font_num, 0, 0, 0, &(font_info.art_id));
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

    if (written_ui_type == WRITTEN_TYPE_PLAQUE) {
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
void written_ui_parse(char* str, int* font_num_ptr, int* centered_ptr, char** str_ptr)
{
    *font_num_ptr = atoi(str);

    *str_ptr = str;
    while (SDL_isdigit(**str_ptr)) {
        (*str_ptr)++;
    }

    *centered_ptr = **str_ptr == 'c' || **str_ptr == 'C';

    if (**str_ptr != '\0') {
        (*str_ptr)++;
    }

    if (SDL_isspace(**str_ptr)) {
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
        if (mes_file_entry.num < written_ui_num + 10
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
    int font_num;
    int centered;
    char* str;
    TigRect v4;
    int v1;

    mes_file_entry.num = *num_ptr;
    if (mes_file_entry.num >= written_ui_num + 10) {
        return -1;
    }

    if (mes_search(dword_680DB4, &mes_file_entry)) {
        return -1;
    }

    written_ui_parse(mes_file_entry.str, &font_num, &centered, &str);

    v1 = *height_ptr;
    str += *height_ptr;
    v4 = *rect;
    if (sub_56C800(str, font_num, centered, &v4, height_ptr)) {
        v1 = 0;
        do {
            v4.y += *height_ptr;
            v4.height -= *height_ptr;
            mes_file_entry.num++;
            if (mes_file_entry.num >= written_ui_num + 10) {
                break;
            }

            if (!mes_search(dword_680DB4, &mes_file_entry)) {
                break;
            }

            written_ui_parse(mes_file_entry.str, &font_num, &centered, &str);
        } while (sub_56C800(str, font_num, centered, &v4, height_ptr));
    }

    *num_ptr = mes_file_entry.num;
    return v1;
}

// 0x56CCA0
void written_ui_newspaper_headline(int num, char* str)
{
    MesFileEntry mes_file_entry;
    int font_num;
    int centered;
    char* tmp;
    char* pch;

    if (!written_ui_mod_loaded) {
        return;
    }

    mes_file_entry.num = num;
    str[0] = '\0';

    if (written_ui_mes_files[WRITTEN_MES_NEWSPAPER] == MES_FILE_HANDLE_INVALID) {
        return;
    }

    if (!mes_search(written_ui_mes_files[WRITTEN_MES_NEWSPAPER], &mes_file_entry)) {
        return;
    }

    written_ui_parse(mes_file_entry.str, &font_num, &centered, &tmp);

    strcpy(str, tmp);

    pch = str;
    while (*pch != '\0') {
        if (*pch == '\n') {
            *pch = ' ';
        }
        pch++;
    }
}
