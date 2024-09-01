#include "ui/schematic_ui.h"

#include <tig/tig.h>

#include "game/lib/object.h"
#include "game/lib/tech.h"

#define SCHEMATIC_F_NAME 0
#define SCHEMATIC_F_DESCRIPTION 1
#define SCHEMATIC_F_ART_NUM 2
#define SCHEMATIC_F_ITEM_1 3
#define SCHEMATIC_F_ITEM_2 4
#define SCHEMATIC_F_PROD 5
#define SCHEMATIC_F_QTY 6

typedef struct SchematicInfo {
    int name;
    int description;
    int art_num;
    int item1[3];
    int item2[3];
    int prod[3];
    int qty;
} SchematicInfo;

static_assert(sizeof(SchematicInfo) == 0x34, "wrong size");

static void sub_56D0D0();
static void schematic_ui_create();
static void schematic_ui_destroy();
static bool schematic_ui_message_filter(TigMessage* msg);
static int sub_56DB00(int schematic);
static int sub_56DB60();
static void sub_56DD20(const char* str, int* items);
static void sub_56DDC0();
static void sub_56E190();

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
static TigRect stru_5CA920 = { 579, 181, 99, 67 };

// 0x5CA930
static TigRect stru_5CA930 = { 579, 277, 99, 67 };

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
static TigFont* schematic_ui_name_font;

// 0x680E38
static TigFont* schematic_ui_description_font;

// FIXME: Should be initialized with `TIG_BUTTON_HANDLE_INVALID`.
//
// 0x680E3C
static tig_button_handle_t dword_680E3C;

// 0x680E40
static bool dword_680E40;

// 0x680E44
static int schematic_ui_rules_mes_file;

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
static TigFont* dword_680E88;

// 0x680E8C
static TigFont* dword_680E8C;

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
    int index;
    MesFileEntry mes_file_entry;
    int tech;
    TigFont font;

    (void)init_info;

    if (!message_load("rules\\schematic.mes", &schematic_ui_rules_mes_file)) {
        return false;
    }

    if (!message_load("mes\\schematic_text.mes", &schematic_ui_text_mes_file)) {
        message_unload(schematic_ui_rules_mes_file);
        return false;
    }

    for (tech = 0; tech < TECH_COUNT; tech++) {
        dword_680DE4[tech] = 0;
    }

    dword_680E54 = message_count_in_range(schematic_ui_rules_mes_file, 2000, 3999) / 7;
    if (dword_680E54 > 0) {
        dword_680E98 = (int*)MALLOC(sizeof(int) * dword_680E54);
        for (index = 2000; index < 3999; index += 10) {
            mes_file_entry.num = index;
            if (message_find(schematic_ui_rules_mes_file, &mes_file_entry)) {
                dword_680E98[index] = index;

                tech = sub_56DB00(dword_680E98[index]);
                dword_680DE4[tech]++;
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
    message_unload(schematic_ui_rules_mes_file);
    message_unload(schematic_ui_text_mes_file);
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
        || sub_45D8D0(a1)
        || sub_45D8D0(a2)
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
        schematic_ui_num_found_schematics = obj_f_idx_size(a1, OBJ_F_PC_SCHEMATICS_FOUND_IDX);
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
    Jack v1;

    if (schematic_ui_created) {
        return;
    }

    if (intgame_big_window_lock(schematic_ui_message_filter, &schematic_ui_window)) {
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
    button_data.x = stru_5CA920.x;
    button_data.y = stru_5CA920.y;
    button_data.width = stru_5CA920.width;
    button_data.height = stru_5CA920.height;
    tig_button_create(&button_data, &dword_680DE0);

    button_data.x = stru_5CA930.x;
    button_data.y = stru_5CA930.y;
    button_data.width = stru_5CA930.width;
    button_data.height = stru_5CA930.height;
    tig_button_create(&button_data, &dword_680E3C);

    sub_56DDC0();

    sub_4B8CE0(obj_field_int64_get(qword_680E70, OBJ_F_LOCATION));

    v1.window_handle = schematic_ui_window;
    v1.dst_rect = &stru_5CA840;
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
        sub_557330();
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
    John v1;
    int tech;

    switch (msg->type) {
    case TIG_MESSAGE_MOUSE:
        if (msg->data.mouse.event == TIG_MOUSE_MESSAGE_LEFT_BUTTON_UP
            && sub_551000(msg->data.mouse.x, msg->data.mouse.y)) {
                sub_56D2D0();
            return true;
        }

        break;
    case TIG_MESSAGE_BUTTON:
        switch (msg->data.button.state) {
        case 2:
            if (msg->data.button.button_handle == dword_680DE0) {
                if (qword_680E90 != 0) {
                    sub_57CCF0(qword_680E60, qword_680E90);
                }
                return true;
            }

            if (msg->data.button.button_handle == dword_680E3C) {
                if (qword_680E28 != 0) {
                    sub_57CCF0(qword_680E60, qword_680E28);
                }
                return true;
            }

            if (msg->data.button.button_handle == stru_5CA850[2].button_handle) {
                mes_file_entry.num = 3;
                sub_4D43A0(schematic_ui_text_mes_file, &mes_file_entry);
                v1.field_0 = 6;
                v1.field_4 = mes_file_entry.text;
                sub_550750(&v1);
                return true;
            }

            if (msg->data.button.button_handle == stru_5CA850[3].button_handle) {
                mes_file_entry.num = 4;
                sub_4D43A0(schematic_ui_text_mes_file, &mes_file_entry);
                v1.field_0 = 6;
                v1.field_4 = mes_file_entry.text;
                sub_550750(&v1);
                return true;
            }

            if (msg->data.button.button_handle == stru_5CA850[4].button_handle) {
                mes_file_entry.num = 5;
                sub_4D43A0(schematic_ui_text_mes_file, &mes_file_entry);
                v1.field_0 = 6;
                v1.field_4 = mes_file_entry.text;
                sub_550750(&v1);
                return true;
            }

            for (tech = 0; tech < TECH_COUNT; tech++) {
                if (msg->data.button.button_handle == stru_5CA8A0[tech].button_handle) {
                    v1.field_0 = 10;
                    v1.field_8 = tech;
                    sub_550750(&v1);
                    return true;
                }
            }

            break;
        case 3:
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
        case 0:
            for (tech = 0; tech < TECH_COUNT; tech++) {
                if (msg->data.button.button_handle == stru_5CA8A0[tech].button_handle) {
                    dword_680E6C = tech;
                    dword_680E58 = tech;
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
        case 1:
            if (msg->data.button.button_handle == stru_5CA850[2].button_handle) {
                switch (dword_680E7C) {
                case 0:
                    if ((tig_net_flags & 0x1) != 0 && (tig_net_flags & 0x2) == 0) {
                        struct {
                            int field_0;
                            int field_4;
                            ObjectId field_8;
                            ObjectId field_20;
                        } packet;

                        static_assert(sizeof(packet) == 0x40, "wrong size");

                        packet.field_0 = 79;
                        packet.field_4 = sub_56DB60();
                        packet.field_8 = sub_407EF0(qword_680E70);
                        packet.field_20 = sub_407EF0(qword_680E60);
                        sub_5295F0(packet, sizeof(packet));
                        sub_56DDC0();
                    } else {
                        sub_56E720(sub_56DB60(), qword_680E70, qword_680E60);
                        sub_56DDC0();
                    }
                    break;
                case 1:
                    mes_file_entry.num = 0;
                    sub_4D43A0(schematic_ui_text_mes_file, &mes_file_entry);
                    v1.field_0 = 4;
                    v1.field_4 = mes_file_entry.text;
                    sub_550750(&v1);
                    break;
                case 2:
                    mes_file_entry.num = 1;
                    sub_4D43A0(schematic_ui_text_mes_file, &mes_file_entry);
                    v1.field_0 = 4;
                    v1.field_4 = mes_file_entry.text;
                    sub_550750(&v1);
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
    // TODO: Incomplete.
}

// 0x56DBD0
void sub_56DBD0(int schematic, SchematicInfo* schematic_info)
{
    MesFileEntry mes_file_entry;

    if (schematic == -1) {
        schematic = 2 * (5 * dword_680E6C + 3000);
    }

    mes_file_entry.num = schematic + SCHEMATIC_F_NAME;
    sub_4D43A0(schematic_ui_rules_mes_file, &mes_file_entry);
    schematic_info->name = atoi(mes_file_entry.text);

    mes_file_entry.num = schematic + SCHEMATIC_F_DESCRIPTION;
    sub_4D43A0(schematic_ui_rules_mes_file, &mes_file_entry);
    schematic_info->description = atoi(mes_file_entry.text);

    mes_file_entry.num = schematic + SCHEMATIC_F_ART_NUM;
    sub_4D43A0(schematic_ui_rules_mes_file, &mes_file_entry);
    schematic_info->art_num = atoi(mes_file_entry.text);

    mes_file_entry.num = schematic + SCHEMATIC_F_ITEM_1;
    sub_4D43A0(schematic_ui_rules_mes_file, &mes_file_entry);
    sub_56DD20(mes_file_entry.text, schematic_info->item1);

    tig_debug_printf("Read line schematic.mes line %d: %d %d %d\n",
        mes_file_entry.num,
        schematic_info->item1[0],
        schematic_info->item1[1],
        schematic_info->item1[2]);

    mes_file_entry.num = schematic + SCHEMATIC_F_ITEM_2;
    sub_4D43A0(schematic_ui_rules_mes_file, &mes_file_entry);
    sub_56DD20(mes_file_entry.text, schematic_info->item2);

    mes_file_entry.num = schematic + SCHEMATIC_F_PROD;
    sub_4D43A0(schematic_ui_rules_mes_file, &mes_file_entry);
    sub_56DD20(mes_file_entry.text, schematic_info->prod);

    mes_file_entry.num = schematic + SCHEMATIC_F_QTY;
    sub_4D43A0(schematic_ui_rules_mes_file, &mes_file_entry);
    schematic_info->qty = atoi(mes_file_entry.text);
}

// 0x56DD20
void sub_56DD20(const char* str, int* items)
{
    char mut_str[2000];
    char* pch;
    int cnt;

    strcpy(mut_str, str);
    cnt = 0;

    pch = strok(mut_str, " ");
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
    TigArtBlitSpec blit_info;
    TigArtFrameData art_frame_data;
    TigFont font;
    MesFileEntry mes_file_entry;
    int schematic;
    SchematicInfo schematic_info;
    long long obj;
    char discipline[2];
    int v1;
    int v2;
    int v3;
    int v4;

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

    tig_window_blit(schematic_ui_window, &blit_info);

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

    dst_rect.x = 146;
    dst_rect.y = 240;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;
    blit_info.dst_rect = &dst_rect;

    tig_window_blit_art(schematic_ui_window, &blit_info);

    // Render name.
    mes_file_entry.num = schematic_info.name;
    sub_4D43A0(schematic_ui_text_mes_file, &mes_file_entry);

    tig_font_push(schematic_ui_name_font);
    font.str = mes_file_entry.text;
    font.width = 0;
    sub_535390(&font);

    src_rect.x = 219;
    src_rect.y = 71;
    src_rect.width = font.width;
    src_rect.height = font.height;

    tig_window_text_write(schematic_ui_window, mes_file_entry.text, &src_rect);
    tig_font_pop();

    // Render description.
    mes_file_entry.num = schematic_info.description;
    sub_4D43A0(schematic_ui_text_mes_file, &mes_file_entry);

    tig_font_push(schematic_ui_description_font);
    font.str = mes_file_entry.text;
    font.width = 0;
    sub_535390(&font);

    src_rect.x = 219;
    src_rect.y = 71;
    src_rect.width = 466;
    src_rect.height = 75;

    tig_window_text_write(schematic_ui_window, mes_file_entry.text, &src_rect);
    tig_font_pop();

    //
    obj = sub_4685A0(schematic_info.prod[0]);
    discipline[0] = obj_field_int32_get(obj, OBJ_F_ITEM_DISCIPLINE) + '0';
    discipline[1] = '\0';

    tig_font_push(dword_680E8C);

    src_rect.x = 222;
    src_rect.y = 32;
    src_rect.width = 100;
    src_rect.height = 100;

    tig_window_text_write(schematic_ui_window, mes_file_entry.text, &src_rect);
    tig_font_pop();

    //
    sub_56E190(0, schematic_info, &v1, &v2);
    sub_56E190(1, schematic_info, &v3, &v4);

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
void sub_56E190()
{
    // TODO: Incomplete.
}

// 0x56E720
void sub_56E720()
{
    // TODO: Incomplete.
}

// 0x56E950
bool sub_56E950(int a1, int a2, int a3, long long obj)
{
    MesFileEntry mes_file_entry;
    John v1;

    if (a1 && player_is_pc_obj(obj)) {
        mes_file_entry.num = 2;
        sub_4D43A0(schematic_ui_text_mes_file, &mes_file_entry);
        v1.field_0 = 4;
        v1.field_4 = mes_file_entry.text;
        sub_550750(&v1);
        sub_56DDC0();
        gsound_play_sfx_id(dword_680E6C + 3018, 1);
    }
    return true;
}

// 0x56E9D0
void sub_56E9D0(int schematic)
{
    SchematicInfo schematic_info;
    MesFileEntry mes_file_entry;

    sub_56DBD0(schematic, &schematic_info);

    mes_file_entry.num = schematic_info.name;
    sub_4D43A0(schematic_ui_text_mes_file, &mes_file_entry);
    return mes_file_entry.text;
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
