#include "ui/schematic_ui.h"

#include <stdio.h>

#include "game/critter.h"
#include "game/gsound.h"
#include "game/item.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/proto.h"
#include "game/tech.h"
#include "ui/intgame.h"
#include "ui/tb_ui.h"

#define SCHEMATIC_F_NAME 0
#define SCHEMATIC_F_DESCRIPTION 1
#define SCHEMATIC_F_ART_NUM 2
#define SCHEMATIC_F_ITEM_1 3
#define SCHEMATIC_F_ITEM_2 4
#define SCHEMATIC_F_PROD 5
#define SCHEMATIC_F_QTY 6

static void sub_56D0D0();
static void schematic_ui_create();
static void schematic_ui_destroy();
static bool schematic_ui_message_filter(TigMessage* msg);
static int sub_56DB00(int schematic);
static int sub_56DB60();
static void sub_56DD20(const char* str, int* items);
static void sub_56DDC0();
static void sub_56E190(int ingr, SchematicInfo* schematic_info, bool* a3, bool* a4);

// 0x5CA818
static tig_window_handle_t schematic_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x5CA820
static TigRect stru_5CA820 = { 0, 41, 800, 400 };

// 0x5CA830
static int dword_5CA830[3] = {
    360,
    359,
    358,
};

// 0x5CA840
static TigRect stru_5CA840 = { 50, 26, 89, 89 };

// 0x5CA850
static UiButtonInfo stru_5CA850[5] = {
    { 204, 57, 372, TIG_BUTTON_HANDLE_INVALID },
    { 687, 57, 373, TIG_BUTTON_HANDLE_INVALID },
    { 76, 365, 357, TIG_BUTTON_HANDLE_INVALID },
    { 29, 201, 6, TIG_BUTTON_HANDLE_INVALID },
    { 29, 289, 6, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5CA8A0
static UiButtonInfo stru_5CA8A0[TECH_COUNT] = {
    { 707, 58, 355, TIG_BUTTON_HANDLE_INVALID },
    { 707, 103, 356, TIG_BUTTON_HANDLE_INVALID },
    { 707, 148, 361, TIG_BUTTON_HANDLE_INVALID },
    { 707, 193, 362, TIG_BUTTON_HANDLE_INVALID },
    { 707, 238, 363, TIG_BUTTON_HANDLE_INVALID },
    { 707, 283, 364, TIG_BUTTON_HANDLE_INVALID },
    { 707, 328, 368, TIG_BUTTON_HANDLE_INVALID },
    { 707, 373, 369, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5CA920
static TigRect stru_5CA920[] = {
    { 579, 181, 99, 67 },
    { 579, 277, 99, 67 },
};

// 0x5CA940
static TigRect stru_5CA940[] = {
    { 583, 182, 50, 50 },
    { 583, 278, 50, 50 },
};

// 0x5CA960
static TigRect stru_5CA960[] = {
    { 658, 232, 50, 50 },
    { 658, 328, 50, 50 },
};

// FIXME: Should be initialized with `TIG_BUTTON_HANDLE_INVALID`.
//
// 0x680DE0
static tig_button_handle_t dword_680DE0;

// 0x680DE4
static int dword_680DE4[TECH_COUNT];

// 0x680E04
static int schematic_ui_num_found_schematics_by_tech[8];

// 0x680E24
static int dword_680E24;

// 0x680E28
static long long qword_680E28;

// 0x680E30
static int dword_680E30;

// 0x680E34
static tig_font_handle_t schematic_ui_name_font;

// 0x680E38
static tig_font_handle_t schematic_ui_description_font;

// 0x680E3C
static tig_button_handle_t dword_680E3C;

// 0x680E40
static bool dword_680E40;

// 0x680E44
static mes_file_handle_t schematic_ui_rules_mes_file;

// 0x680E48
static int dword_680E48;

// 0x680E4C
static int* dword_680E4C;

// 0x680E50
static int* schematic_ui_found_schematics;

// 0x680E54
static int dword_680E54;

// 0x680E58
static int dword_680E58;

// 0x680E5C
static int* dword_680E5C;

// 0x680E60
static long long qword_680E60;

// 0x680E68
static int dword_680E68;

// 0x680E6C
static int dword_680E6C;

// 0x680E70
static long long qword_680E70;

// 0x680E78
static int schematic_ui_text_mes_file;

// 0x680E7C
static int dword_680E7C;

// 0x680E80
static int dword_680E80;

// 0x680E84
static int schematic_ui_num_found_schematics;

// 0x680E88
static tig_font_handle_t dword_680E88;

// 0x680E8C
static tig_font_handle_t dword_680E8C;

// 0x680E90
static long long qword_680E90;

// 0x680E98
static int* dword_680E98;

// 0x680E9C
static bool schematic_ui_initialized;

// 0x680EA0
static bool schematic_ui_created;

// 0x56CD60
bool schematic_ui_init(GameInitInfo* init_info)
{
    int num;
    MesFileEntry mes_file_entry;
    int tech;
    TigFont font;
    int idx;

    (void)init_info;

    if (!mes_load("rules\\schematic.mes", &schematic_ui_rules_mes_file)) {
        return false;
    }

    if (!mes_load("mes\\schematic_text.mes", &schematic_ui_text_mes_file)) {
        mes_unload(schematic_ui_rules_mes_file);
        return false;
    }

    for (tech = 0; tech < TECH_COUNT; tech++) {
        dword_680DE4[tech] = 0;
    }

    dword_680E54 = mes_entries_count_in_range(schematic_ui_rules_mes_file, 2000, 3999) / 7;
    if (dword_680E54 > 0) {
        dword_680E98 = (int*)MALLOC(sizeof(int) * dword_680E54);
        idx = 0;
        for (num = 2000; num < 3999; num += 10) {
            mes_file_entry.num = num;
            if (mes_search(schematic_ui_rules_mes_file, &mes_file_entry)) {
                dword_680E98[idx] = num;

                tech = sub_56DB00(num);
                dword_680DE4[tech]++;

                idx++;
            }
        }
    }

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(0, 0, 0);
    tig_font_create(&font, &schematic_ui_description_font);

    font.flags = 0;
    tig_art_interface_id_create(300, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(0, 0, 0);
    tig_font_create(&font, &dword_680E88);

    font.flags = 0;
    tig_art_interface_id_create(370, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(0, 0, 0);
    tig_font_create(&font, &dword_680E8C);

    font.flags = 0;
    tig_art_interface_id_create(371, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(0, 0, 0);
    tig_font_create(&font, &schematic_ui_name_font);

    schematic_ui_initialized = true;
    qword_680E70 = 0;
    qword_680E60 = 0;
    sub_56D0D0();

    return true;
}

// 0x56D040
void schematic_ui_exit()
{
    mes_unload(schematic_ui_rules_mes_file);
    mes_unload(schematic_ui_text_mes_file);
    tig_font_destroy(schematic_ui_description_font);
    tig_font_destroy(dword_680E88);
    tig_font_destroy(dword_680E8C);
    tig_font_destroy(schematic_ui_name_font);
    if (dword_680E54 > 0) {
        FREE(dword_680E98);
    }
    schematic_ui_initialized = false;
}

// 0x56D0B0
void schematic_ui_reset()
{
    qword_680E70 = 0;
    qword_680E60 = 0;
    sub_56D0D0();
}

// 0x56D0D0
void sub_56D0D0()
{
    dword_680E7C = 1;
    dword_680E6C = 0;
    dword_680E80 = 0;
    dword_680E30 = 0;
    dword_680E58 = 0;
    dword_680E68 = 0;
    dword_680E24 = 0;
    dword_680E48 = dword_680E54;
    dword_680E4C = dword_680DE4;
    dword_680E5C = dword_680E98;
    dword_680E40 = 0;
}

// 0x56D130
void sub_56D130(long long a1, long long a2)
{
    int tech;
    int index;

    if (schematic_ui_created) {
        sub_56D2D0();
        return;
    }

    if (a1 == OBJ_HANDLE_NULL
        || a2 == OBJ_HANDLE_NULL
        || critter_is_dead(a1)
        || critter_is_dead(a2)
        || !sub_551A80(0)
        || !sub_551A80(14)) {
        return;
    }

    qword_680E70 = a1;
    qword_680E60 = a2;

    if (obj_field_int32_get(a1, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        sub_56D0D0();
    }

    for (tech = 0; tech < TECH_COUNT; tech++) {
        dword_680DE4[tech] = tech_get_degree(a1, tech);
        schematic_ui_num_found_schematics_by_tech[tech] = 0;
    }

    if (obj_field_int32_get(a1, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        schematic_ui_num_found_schematics = obj_arrayfield_length_get(a1, OBJ_F_PC_SCHEMATICS_FOUND_IDX);
        if (schematic_ui_num_found_schematics > 0) {
            schematic_ui_found_schematics = (int*)MALLOC(sizeof(int) * schematic_ui_num_found_schematics);
            for (index = 0; index < schematic_ui_num_found_schematics; index++) {
                schematic_ui_found_schematics[index] = sub_407470(a1, OBJ_F_PC_SCHEMATICS_FOUND_IDX, index);

                tech = sub_56DB00(schematic_ui_found_schematics[index]);
                schematic_ui_num_found_schematics_by_tech[tech]++;
            }
        }
    } else {
        schematic_ui_num_found_schematics = 0;
    }

    if (dword_680E40 == 1) {
        dword_680E4C = schematic_ui_num_found_schematics_by_tech;
        dword_680E5C = schematic_ui_found_schematics;
        dword_680E48 = schematic_ui_num_found_schematics;
    }

    schematic_ui_create();
}

// 0x56D2D0
void sub_56D2D0()
{
    if (schematic_ui_created && sub_551A80(0)) {
        schematic_ui_destroy();
        qword_680E70 = 0;
        qword_680E60 = 0;
    }
}

// 0x56D310
void schematic_ui_create()
{
    int index;
    tig_button_handle_t buttons[TECH_COUNT];
    TigButtonData button_data;
    S550DA0 v1;

    if (schematic_ui_created) {
        return;
    }

    if (!intgame_big_window_lock(schematic_ui_message_filter, &schematic_ui_window)) {
        tig_debug_printf("schematic_ui_create: ERROR: intgame_big_window_lock failed!\n");
        exit(0);
    }

    for (index = 0; index < 5; index++) {
        sub_54AA60(schematic_ui_window, &stru_5CA820, &(stru_5CA850[index]), 1);
    }

    for (index = 0; index < TECH_COUNT; index++) {
        sub_54AA60(schematic_ui_window, &stru_5CA820, &(stru_5CA8A0[index]), 1);
        buttons[index] = stru_5CA8A0[index].button_handle;
    }

    tig_button_radio_group_create(TECH_COUNT, buttons, dword_680E6C);

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = schematic_ui_window;
    button_data.art_id = TIG_ART_ID_INVALID;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    button_data.x = stru_5CA920[0].x;
    button_data.y = stru_5CA920[0].y;
    button_data.width = stru_5CA920[0].width;
    button_data.height = stru_5CA920[0].height;
    tig_button_create(&button_data, &dword_680DE0);

    button_data.x = stru_5CA920[1].x;
    button_data.y = stru_5CA920[1].y;
    button_data.width = stru_5CA920[1].width;
    button_data.height = stru_5CA920[1].height;
    tig_button_create(&button_data, &dword_680E3C);

    sub_56DDC0();

    sub_4B8CE0(obj_field_int64_get(qword_680E70, OBJ_F_LOCATION));

    v1.window_handle = schematic_ui_window;
    v1.rect = &stru_5CA840;
    tig_art_interface_id_create(231, 0, 0, 0, &(v1.art_id));
    sub_550DA0(1, &v1);

    gsound_play_sfx_id(3008, 1);
    schematic_ui_created = true;
}

// 0x56D4D0
void schematic_ui_destroy()
{
    if (schematic_ui_created) {
        sub_550DA0(0, NULL);
        intgame_big_window_unlock();
        schematic_ui_window = TIG_WINDOW_HANDLE_INVALID;
        if (schematic_ui_num_found_schematics > 0) {
            free(schematic_ui_found_schematics);
        }
        gsound_play_sfx_id(3009, 1);
        schematic_ui_created = false;
    }
}

// 0x56D530
bool schematic_ui_message_filter(TigMessage* msg)
{
    MesFileEntry mes_file_entry;
    UiMessage ui_message;
    int tech;
    Packet79 pkt;

    switch (msg->type) {
    case TIG_MESSAGE_MOUSE:
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP
            && sub_551000(msg->data.mouse.x, msg->data.mouse.y)) {
                sub_56D2D0();
            return true;
        }

        break;
    case TIG_MESSAGE_BUTTON:
        switch (msg->data.button.state) {
        case TIG_BUTTON_STATE_MOUSE_INSIDE:
            if (msg->data.button.button_handle == dword_680DE0) {
                if (qword_680E90 != 0) {
                    sub_57CCF0(qword_680E60, qword_680E90);
                }
                return true;
            }

            if (msg->data.button.button_handle == dword_680E3C) {
                if (qword_680E28 != OBJ_HANDLE_NULL) {
                    sub_57CCF0(qword_680E60, qword_680E28);
                }
                return true;
            }

            if (msg->data.button.button_handle == stru_5CA850[2].button_handle) {
                mes_file_entry.num = 3;
                mes_get_msg(schematic_ui_text_mes_file, &mes_file_entry);
                ui_message.type = UI_MSG_TYPE_FEEDBACK;
                ui_message.str = mes_file_entry.str;
                sub_550750(&ui_message);
                return true;
            }

            if (msg->data.button.button_handle == stru_5CA850[3].button_handle) {
                mes_file_entry.num = 4;
                mes_get_msg(schematic_ui_text_mes_file, &mes_file_entry);
                ui_message.type = UI_MSG_TYPE_FEEDBACK;
                ui_message.str = mes_file_entry.str;
                sub_550750(&ui_message);
                return true;
            }

            if (msg->data.button.button_handle == stru_5CA850[4].button_handle) {
                mes_file_entry.num = 5;
                mes_get_msg(schematic_ui_text_mes_file, &mes_file_entry);
                ui_message.type = UI_MSG_TYPE_FEEDBACK;
                ui_message.str = mes_file_entry.str;
                sub_550750(&ui_message);
                return true;
            }

            for (tech = 0; tech < TECH_COUNT; tech++) {
                if (msg->data.button.button_handle == stru_5CA8A0[tech].button_handle) {
                    ui_message.type = UI_MSG_TYPE_TECH;
                    ui_message.field_8 = tech;
                    sub_550750(&ui_message);
                    return true;
                }
            }

            break;
        case TIG_BUTTON_STATE_MOUSE_OUTSIDE:
            if (msg->data.button.button_handle == dword_680DE0
                || msg->data.button.button_handle == dword_680E3C
                || msg->data.button.button_handle == stru_5CA850[2].button_handle
                || msg->data.button.button_handle == stru_5CA850[3].button_handle
                || msg->data.button.button_handle == stru_5CA850[4].button_handle) {
                sub_550720();
                return true;
            }

            for (tech = 0; tech < TECH_COUNT; tech++) {
                if (msg->data.button.button_handle == stru_5CA8A0[tech].button_handle) {
                    sub_550720();
                    return true;
                }
            }

            break;
        case TIG_BUTTON_STATE_PRESSED:
            for (tech = 0; tech < TECH_COUNT; tech++) {
                if (msg->data.button.button_handle == stru_5CA8A0[tech].button_handle) {
                    dword_680E6C = tech;
                    dword_680E58 = 0;
                    sub_56DDC0();
                    gsound_play_sfx_id(3010, 1);
                    return true;
                }
            }

            if (msg->data.button.button_handle == stru_5CA850[0].button_handle) {
                dword_680E58--;
                sub_56DDC0();
                gsound_play_sfx_id(3010, 1);
                return true;
            }

            if (msg->data.button.button_handle == stru_5CA850[1].button_handle) {
                dword_680E58++;
                sub_56DDC0();
                gsound_play_sfx_id(3010, 1);
                return true;
            }

            if (msg->data.button.button_handle == stru_5CA850[3].button_handle) {
                if (dword_680E40 == 1) {
                    tig_button_state_change(stru_5CA8A0[dword_680E6C].button_handle, 1);
                    dword_680E68 = dword_680E58;
                    dword_680E80 = dword_680E6C;
                    dword_680E58 = dword_680E24;
                    dword_680E6C = dword_680E30;
                    dword_680E48 = dword_680E54;
                    dword_680E4C = dword_680DE4;
                    dword_680E5C = dword_680E98;
                    dword_680E40 = 0;
                    tig_button_state_change(stru_5CA8A0[dword_680E30].button_handle, 0);
                    sub_56DDC0();
                    gsound_play_sfx_id(3011, 1);
                }
                return true;
            }

            if (msg->data.button.button_handle == stru_5CA850[4].button_handle) {
                if (dword_680E40 == 0) {
                    tig_button_state_change(stru_5CA8A0[dword_680E6C].button_handle, 1);
                    dword_680E24 = dword_680E58;
                    dword_680E30 = dword_680E6C;
                    dword_680E58 = dword_680E68;
                    dword_680E6C = dword_680E80;
                    dword_680E48 = schematic_ui_num_found_schematics;
                    dword_680E4C = schematic_ui_num_found_schematics_by_tech;
                    dword_680E5C = schematic_ui_found_schematics;
                    dword_680E40 = 1;
                    tig_button_state_change(stru_5CA8A0[dword_680E80].button_handle, 0);
                    sub_56DDC0();
                    gsound_play_sfx_id(3011, 1);
                }
                return true;
            }

            break;
        case TIG_BUTTON_STATE_RELEASED:
            if (msg->data.button.button_handle == stru_5CA850[2].button_handle) {
                switch (dword_680E7C) {
                case 0:
                    if ((tig_net_flags & 0x1) != 0 && (tig_net_flags & 0x2) == 0) {
                        pkt.type = 79;
                        pkt.field_4 = sub_56DB60();
                        pkt.field_8 = sub_407EF0(qword_680E70);
                        pkt.field_20 = sub_407EF0(qword_680E60);
                        tig_net_send_app_all(&pkt, sizeof(pkt));
                        sub_56DDC0();
                    } else {
                        sub_56E720(sub_56DB60(), qword_680E70, qword_680E60);
                        sub_56DDC0();
                    }
                    break;
                case 1:
                    mes_file_entry.num = 0;
                    mes_get_msg(schematic_ui_text_mes_file, &mes_file_entry);
                    ui_message.type = UI_MSG_TYPE_EXCLAMATION;
                    ui_message.str = mes_file_entry.str;
                    sub_550750(&ui_message);
                    break;
                case 2:
                    mes_file_entry.num = 1;
                    mes_get_msg(schematic_ui_text_mes_file, &mes_file_entry);
                    ui_message.type = UI_MSG_TYPE_EXCLAMATION;
                    ui_message.str = mes_file_entry.str;
                    sub_550750(&ui_message);
                    break;
                }
                return true;
            }
            break;
        }
        break;
    }

    return false;
}

// 0x56DB00
int sub_56DB00(int schematic)
{
    SchematicInfo schematic_info;
    long long obj;

    if (schematic >= 4000) {
        sub_56DBD0(schematic, &schematic_info);
        obj = sub_4685A0(schematic_info.prod[0]);
        return obj_field_int32_get(obj, OBJ_F_ITEM_DISCIPLINE);
    }

    if (schematic >= 2000) {
        return (schematic - 2000) / 200;
    }

    return 0;
}

// 0x56DB60
int sub_56DB60()
{
    int index;
    int v1;

    if (dword_680E4C[dword_680E6C] == 0) {
        return -1;
    }

    v1 = 0;
    for (index = 0; index < dword_680E48; index++) {
        if (sub_56DB00(dword_680E5C[index]) == dword_680E6C) {
            if (v1 == dword_680E58) {
                return dword_680E5C[index];
            }
            v1++;
        }
    }

    return -1;
}

// 0x56DBD0
void sub_56DBD0(int schematic, SchematicInfo* schematic_info)
{
    MesFileEntry mes_file_entry;

    if (schematic == -1) {
        schematic = 2 * (5 * dword_680E6C + 3000);
    }

    mes_file_entry.num = schematic + SCHEMATIC_F_NAME;
    mes_get_msg(schematic_ui_rules_mes_file, &mes_file_entry);
    schematic_info->name = atoi(mes_file_entry.str);

    mes_file_entry.num = schematic + SCHEMATIC_F_DESCRIPTION;
    mes_get_msg(schematic_ui_rules_mes_file, &mes_file_entry);
    schematic_info->description = atoi(mes_file_entry.str);

    mes_file_entry.num = schematic + SCHEMATIC_F_ART_NUM;
    mes_get_msg(schematic_ui_rules_mes_file, &mes_file_entry);
    schematic_info->art_num = atoi(mes_file_entry.str);

    mes_file_entry.num = schematic + SCHEMATIC_F_ITEM_1;
    mes_get_msg(schematic_ui_rules_mes_file, &mes_file_entry);
    sub_56DD20(mes_file_entry.str, schematic_info->item1);

    tig_debug_printf("Read line schematic.mes line %d: %d %d %d\n",
        mes_file_entry.num,
        schematic_info->item1[0],
        schematic_info->item1[1],
        schematic_info->item1[2]);

    mes_file_entry.num = schematic + SCHEMATIC_F_ITEM_2;
    mes_get_msg(schematic_ui_rules_mes_file, &mes_file_entry);
    sub_56DD20(mes_file_entry.str, schematic_info->item2);

    mes_file_entry.num = schematic + SCHEMATIC_F_PROD;
    mes_get_msg(schematic_ui_rules_mes_file, &mes_file_entry);
    sub_56DD20(mes_file_entry.str, schematic_info->prod);

    mes_file_entry.num = schematic + SCHEMATIC_F_QTY;
    mes_get_msg(schematic_ui_rules_mes_file, &mes_file_entry);
    schematic_info->qty = atoi(mes_file_entry.str);
}

// 0x56DD20
void sub_56DD20(const char* str, int* items)
{
    char mut_str[2000];
    char* pch;
    int cnt;

    strcpy(mut_str, str);
    cnt = 0;

    pch = strtok(mut_str, " ");
    if (pch != NULL) {
        do {
            items[cnt++] = atoi(pch);
            pch = strtok(NULL, " ");
        } while (pch != NULL);

        if (cnt > 0) {
            while (cnt < 3) {
                items[cnt] = items[cnt - 1];
                cnt++;
            }
        }
    }
}

// 0x56DDC0
void sub_56DDC0()
{
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitInfo blit_info;
    TigArtFrameData art_frame_data;
    TigFont font;
    MesFileEntry mes_file_entry;
    int schematic;
    SchematicInfo schematic_info;
    long long obj;
    char icon[2];
    bool v1;
    bool v2;
    bool v3;
    bool v4;

    if (dword_680E58 != 0) {
        tig_button_show(stru_5CA850[0].button_handle);
    } else {
        tig_button_hide(stru_5CA850[0].button_handle);
    }

    if (dword_680E58 < dword_680E4C[dword_680E6C] - 1) {
        tig_button_show(stru_5CA850[1].button_handle);
    } else {
        tig_button_hide(stru_5CA850[1].button_handle);
    }

    // Render background.
    blit_info.flags = 0;
    tig_art_interface_id_create(365, 0, 0, 0, &(blit_info.art_id));

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = stru_5CA820.width;
    src_rect.height = stru_5CA820.height;
    blit_info.src_rect = &src_rect;

    dst_rect = src_rect;
    blit_info.dst_rect = &dst_rect;

    tig_window_blit_art(schematic_ui_window, &blit_info);

    // Obtain current schematic info.
    schematic = sub_56DB60();
    sub_56DBD0(schematic, &schematic_info);

    // Render schematic image.
    blit_info.flags = 0;
    tig_art_interface_id_create(schematic_info.art_num, 0, 0, 0, &(blit_info.art_id));
    tig_art_frame_data(blit_info.art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;
    blit_info.src_rect = &src_rect;

    dst_rect.x = 240;
    dst_rect.y = 146;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;
    blit_info.dst_rect = &dst_rect;

    tig_window_blit_art(schematic_ui_window, &blit_info);

    // Render name.
    mes_file_entry.num = schematic_info.name;
    mes_get_msg(schematic_ui_text_mes_file, &mes_file_entry);

    tig_font_push(schematic_ui_name_font);
    font.str = mes_file_entry.str;
    font.width = 0;
    sub_535390(&font);

    src_rect.x = 261;
    src_rect.y = 30;
    src_rect.width = font.width;
    src_rect.height = font.height;

    tig_window_text_write(schematic_ui_window, mes_file_entry.str, &src_rect);
    tig_font_pop();

    // Render description.
    mes_file_entry.num = schematic_info.description;
    mes_get_msg(schematic_ui_text_mes_file, &mes_file_entry);

    tig_font_push(schematic_ui_description_font);
    font.str = mes_file_entry.str;
    font.width = 0;
    sub_535390(&font);

    src_rect.x = 219;
    src_rect.y = 71;
    src_rect.width = 466;
    src_rect.height = 75;

    tig_window_text_write(schematic_ui_window, mes_file_entry.str, &src_rect);
    tig_font_pop();

    // Render icon.
    obj = sub_4685A0(schematic_info.prod[0]);
    icon[0] = (char)obj_field_int32_get(obj, OBJ_F_ITEM_DISCIPLINE) + '0';
    icon[1] = '\0';

    tig_font_push(dword_680E8C);

    src_rect.x = 222;
    src_rect.y = 32;
    src_rect.width = 100;
    src_rect.height = 100;

    tig_window_text_write(schematic_ui_window, icon, &src_rect);
    tig_font_pop();

    //
    sub_56E190(0, &schematic_info, &v1, &v2);
    sub_56E190(1, &schematic_info, &v3, &v4);

    if (!v2 || !v4) {
        dword_680E7C = 2;
    } else if (!v1 || !v3) {
        dword_680E7C = 1;
    } else {
        dword_680E7C = 0;
    }

    //
    blit_info.flags = 0;
    tig_art_interface_id_create(dword_5CA830[dword_680E7C], 0, 0, 0, &(blit_info.art_id));
    tig_art_frame_data(blit_info.art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;
    blit_info.src_rect = &src_rect;

    dst_rect.x = 43;
    dst_rect.y = 338;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;
    blit_info.dst_rect = &dst_rect;

    tig_window_blit_art(schematic_ui_window, &blit_info);
}

// 0x56E190
void sub_56E190(int ingr, SchematicInfo* schematic_info, bool* a3, bool* a4)
{
    int index;
    TigArtAnimData art_anim_data;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect dst_rect;
    TigRect src_rect;
    int64_t item_obj;
    int discipline;
    int complexity;
    TigPalette palette;
    TigPaletteModifyInfo palette_modify_info;
    char str[80];
    float width_ratio;
    float height_ratio;

    if (ingr == 0) {
        for (index = 0; index < 3; index++) {
            item_obj = sub_4685A0(schematic_info->item1[index]);
            if (sub_462540(qword_680E70, item_obj, 0x7) != OBJ_HANDLE_NULL) {
                break;
            }
        }

        if (index == 3) {
            item_obj = sub_4685A0(schematic_info->item1[0]);
            *a3 = false;
        } else {
            *a3 = true;
        }

        qword_680E90 = item_obj;
    } else {
        for (index = 0; index < 3; index++) {
            item_obj = sub_4685A0(schematic_info->item2[index]);
            if (sub_462540(qword_680E70, item_obj, 0x7) != OBJ_HANDLE_NULL) {
                break;
            }
        }

        if (index == 3) {
            item_obj = sub_4685A0(schematic_info->item2[0]);
            *a3 = false;
        } else {
            *a3 = true;
        }

        qword_680E28 = item_obj;
    }

    discipline = obj_field_int32_get(item_obj, OBJ_F_ITEM_DISCIPLINE);
    complexity = -obj_field_int32_get(item_obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);
    *a4 = sub_4B00B0(qword_680E70, discipline) >= complexity;

    if (*a4) {
        if (*a3) {
            tig_art_interface_id_create(831, 0, 0, 0, &(art_blit_info.art_id));
        } else {
            tig_art_interface_id_create(830, 0, 0, 0, &(art_blit_info.art_id));
        }

        art_blit_info.flags = 0;
        if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
            return;
        }

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.height;

        dst_rect.y = stru_5CA920[ingr].y;
        dst_rect.x = stru_5CA920[ingr].x;
        dst_rect.width = art_frame_data.width;
        dst_rect.height = art_frame_data.height;

        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(schematic_ui_window, &art_blit_info);
    }

    art_blit_info.flags = 0;

    switch (obj_field_int32_get(item_obj, OBJ_F_TYPE)) {
    case OBJ_TYPE_WEAPON:
        art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_WEAPON_PAPER_DOLL_AID);
        break;
    case OBJ_TYPE_ARMOR:
        art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_ARMOR_PAPER_DOLL_AID);
        break;
    default:
        art_blit_info.art_id = TIG_ART_ID_INVALID;
        break;
    }

    if (art_blit_info.art_id != TIG_ART_ID_INVALID) {
        art_blit_info.art_id = tig_art_item_id_disposition_set(art_blit_info.art_id, TIG_ART_ITEM_DISPOSITION_SCHEMATIC);
        if (tig_art_exists(art_blit_info.art_id) != TIG_OK) {
            art_blit_info.art_id = TIG_ART_ID_INVALID;
        }
    }

    if (art_blit_info.art_id == TIG_ART_ID_INVALID) {
        art_blit_info.art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_INV_AID);
    }

    if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect = stru_5CA920[ingr];

    width_ratio = (float)art_frame_data.width / (float)stru_5CA920[ingr].width;
    height_ratio = (float)art_frame_data.height / (float)stru_5CA920[ingr].height;

    if (width_ratio > height_ratio && width_ratio > 1.0f) {
        dst_rect.height = (int)(art_frame_data.height / width_ratio);
        dst_rect.y += (stru_5CA920[ingr].height - dst_rect.height) / 2;
    } else if (height_ratio > width_ratio && height_ratio > 1.0f) {
        dst_rect.width = (int)(art_frame_data.width / height_ratio);
        dst_rect.x += (stru_5CA920[ingr].width - dst_rect.width) / 2;
    } else {
        dst_rect.width = art_frame_data.width;
        dst_rect.x += (stru_5CA920[ingr].width - art_frame_data.width) / 2;
        dst_rect.height = art_frame_data.height;
        dst_rect.y += (stru_5CA920[ingr].height - art_frame_data.height) / 2;
    }

    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;

    if (!*a4 && tig_art_anim_data(art_blit_info.art_id, &art_anim_data) == TIG_OK) {
        palette = tig_palette_create();

        palette_modify_info.flags = TIG_PALETTE_MODIFY_GRAYSCALE;
        palette_modify_info.src_palette = art_anim_data.palette1;
        palette_modify_info.dst_palette = palette;
        tig_palette_modify(&palette_modify_info);

        palette_modify_info.flags = TIG_PALETTE_MODIFY_TINT;
        palette_modify_info.tint_color = tig_color_make(255, 204, 102);
        palette_modify_info.src_palette = palette;
        tig_palette_modify(&palette_modify_info);

        art_blit_info.flags |= TIG_ART_BLT_PALETTE_OVERRIDE;
        art_blit_info.palette = palette;
    } else {
        palette = NULL;
    }

    tig_window_blit_art(schematic_ui_window, &art_blit_info);

    if (palette != NULL) {
        tig_palette_destroy(palette);
    }

    if (complexity > 0) {
        str[0] = (char)(discipline + '1');
        str[1] = '\0';
        tig_font_push(dword_680E88);
        tig_window_text_write(schematic_ui_window, str, &(stru_5CA940[ingr]));
        tig_font_pop();

        sprintf(str, "%d", complexity);
        tig_font_push(schematic_ui_description_font);
        tig_window_text_write(schematic_ui_window, str, &stru_5CA960[ingr]);
        tig_font_pop();
    }
}

// 0x56E720
bool sub_56E720(int schematic, int64_t a2, int64_t a3)
{
    SchematicInfo info;
    int64_t obj;
    int ingridient1;
    int ingridient2;
    int64_t ingridient1_obj;
    int64_t ingridient2_obj;
    int prod;
    int64_t loc;
    int qty;
    int64_t prod_obj;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0
        && !sub_4A2BA0()) {
        return true;
    }

    if (schematic == -1) {
        return false;
    }

    sub_56DBD0(schematic, &info);

    for (ingridient1 = 0; ingridient1 < 3; ingridient1++) {
        obj = sub_4685A0(info.item1[ingridient1]);
        ingridient1_obj = sub_462540(a2, obj, 0x7);
        if (ingridient1_obj != OBJ_HANDLE_NULL) {
            break;
        }
    }

    if (ingridient1 == 3) {
        return false;
    }

    for (ingridient2 = 0; ingridient2 < 3; ingridient2++) {
        obj = sub_4685A0(info.item2[ingridient2]);
        ingridient2_obj = sub_462540(a2, obj, 0x7);
        if (ingridient2_obj != OBJ_HANDLE_NULL) {
            break;
        }
    }

    if (ingridient2 == 3) {
        return false;
    }

    if (ingridient1 != 0) {
        prod = info.prod[ingridient1];
    } else {
        prod = info.prod[ingridient2];
    }

    loc = obj_field_int64_get(a2, OBJ_F_LOCATION);

    if (obj_field_int32_get(ingridient1_obj, OBJ_F_TYPE) == OBJ_TYPE_AMMO) {
        item_ammo_move(a2,
            OBJ_HANDLE_NULL,
            1,
            obj_field_int32_get(ingridient1_obj, OBJ_F_AMMO_QUANTITY),
            ingridient1_obj);
    } else {
        sub_43CCA0(ingridient1_obj);
    }

    if (obj_field_int32_get(ingridient2_obj, OBJ_F_TYPE) == OBJ_TYPE_AMMO) {
        item_ammo_move(a2,
            OBJ_HANDLE_NULL,
            1,
            obj_field_int32_get(ingridient2_obj, OBJ_F_AMMO_QUANTITY),
            ingridient2_obj);
    } else {
        sub_43CCA0(ingridient2_obj);
    }

    for (qty = 0; qty < info.qty; qty++) {
        if (!mp_object_create(prod, loc, &prod_obj)) {
            return false;
        }

        sub_4617F0(prod_obj, a2);
    }

    sub_4EE0F0(1, a2, a3);

    return true;
}

// 0x56E950
bool sub_56E950(int a1, int64_t a2, long long obj)
{
    MesFileEntry mes_file_entry;
    UiMessage ui_message;

    (void)a2;

    if (a1 && player_is_pc_obj(obj)) {
        mes_file_entry.num = 2;
        mes_get_msg(schematic_ui_text_mes_file, &mes_file_entry);
        ui_message.type = UI_MSG_TYPE_EXCLAMATION;
        ui_message.str = mes_file_entry.str;
        sub_550750(&ui_message);
        sub_56DDC0();
        gsound_play_sfx_id(dword_680E6C + 3018, 1);
    }
    return true;
}

// 0x56E9D0
char* sub_56E9D0(int schematic)
{
    SchematicInfo schematic_info;
    MesFileEntry mes_file_entry;

    sub_56DBD0(schematic, &schematic_info);

    mes_file_entry.num = schematic_info.name;
    mes_get_msg(schematic_ui_text_mes_file, &mes_file_entry);
    return mes_file_entry.str;
}

// 0x56EA10
char* sub_56EA10(int a1, int a2)
{
    return sub_56E9D0(sub_4B0320(a1, a2));
}

// 0x56EA30
void sub_56EA30(int a1, int a2, char* a3, char* a4)
{
    SchematicInfo schematic_info;
    long long obj;

    sub_56DBD0(sub_4B0320(a1, a2), &schematic_info);

    obj = sub_4685A0(schematic_info.item1[0]);
    sub_441B60(obj, obj, a3);

    obj = sub_4685A0(schematic_info.item2[0]);
    sub_441B60(obj, obj, a4);
}
