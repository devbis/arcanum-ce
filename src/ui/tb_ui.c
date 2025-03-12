#include "ui/tb_ui.h"

#include <stdio.h>

#include "game/ai.h"
#include "game/critter.h"
#include "game/gamelib.h"
#include "game/magictech.h"
#include "game/mes.h"
#include "game/multiplayer.h"
#include "game/object.h"
#include "game/player.h"
#include "game/script.h"
#include "game/ui.h"
#include "ui/anim_ui.h"
#include "ui/charedit_ui.h"
#include "ui/dialog_ui.h"
#include "ui/follower_ui.h"
#include "ui/gameuilib.h"
#include "ui/hotkey_ui.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"
#include "ui/item_ui.h"
#include "ui/mainmenu_ui.h"
#include "ui/mp_ctrl_ui.h"
#include "ui/multiplayer_hub_ui.h"
#include "ui/multiplayer_ui.h"
#include "ui/schematic_ui.h"
#include "ui/skill_ui.h"
#include "ui/sleep_ui.h"
#include "ui/slide_ui.h"
#include "ui/spell_ui.h"
#include "ui/tech_ui.h"
#include "ui/wmap_rnd.h"
#include "ui/wmap_ui.h"
#include "ui/written_ui.h"

static void sub_57CAE0();
static void sub_57CAB0(int64_t obj);
static void sub_57CAF0(int64_t obj);
static void show_inven_loot(int64_t pc_obj, int64_t target_obj);
static void show_inven_identify(int64_t pc_obj, int64_t target_obj);
static void show_inven_npc_identify(int64_t pc_obj, int64_t target_obj);
static void sub_57CBE0(char* str);
static void sub_57CC10(int64_t obj);
static void sub_57CC70(int64_t a1, int64_t a2);
static void sub_57CDC0();
static void sub_57CDE0();
static void sub_57CDF0(int a1);
static void sub_57CE00();
static void sub_57CE10();
static void sub_57CE30(int64_t obj, void* a2, int a3);
static void sub_57CE70(int64_t a1, int64_t a2);
static void ui_charedit_error_msg(int type, int a2);
static void sub_57CF70(int64_t a1, int64_t a2);
static void sub_57CFA0();
static bool sub_57CFB0();
static bool sub_57D080();
static bool sub_57D150(const char* name, const char* a2);

// 0x57C6E0
bool tb_ui_init(GameInitInfo* init_info)
{
    UiCallbacks callbacks;

    (void)init_info;

    callbacks.field_7C = sub_57CBE0;
    callbacks.field_80 = sub_550750;
    callbacks.field_58 = sub_57CCF0;
    callbacks.field_48 = spell_ui_add;
    callbacks.field_4C = sub_57C290;
    callbacks.field_50 = sub_57C320;
    callbacks.field_54 = sub_57C4B0;
    callbacks.field_0 = sub_57CAE0;
    callbacks.field_4 = sub_57CAF0;
    callbacks.field_8 = sub_57CAB0;
    callbacks.field_C = sub_5704E0;
    callbacks.field_14 = sub_57CC10;
    callbacks.field_10 = sub_5571C0;
    callbacks.field_18 = sub_5570A0;
    callbacks.field_1C = sub_5570D0;
    callbacks.show_inven_loot = show_inven_loot;
    callbacks.show_inven_identify = show_inven_identify;
    callbacks.field_28 = tech_ui_adjust_degree;
    callbacks.field_2C = skill_ui_preprocess;
    callbacks.field_30 = sub_57A320;
    callbacks.adjust_skill = skill_ui_adjust_skill;
    callbacks.set_skill_training = skill_ui_set_training;
    callbacks.field_3C = sub_57CC70;
    callbacks.field_40 = sub_567E00;
    callbacks.field_44 = sub_567400;
    callbacks.field_5C = sub_57CDC0;
    callbacks.field_60 = sub_57CDE0;
    callbacks.field_64 = sub_57CDF0;
    callbacks.field_68 = sub_57EDF0;
    callbacks.field_6C = sub_57EED0;
    callbacks.field_70 = sub_57CE10;
    callbacks.field_74 = sub_57CE00;
    callbacks.field_78 = sub_57CE30;
    callbacks.field_88 = sub_53EAF0;
    callbacks.field_84 = sub_557730;
    callbacks.written_start_obj = written_ui_start_obj;
    callbacks.written_start_type = written_ui_start_type;
    callbacks.field_94 = sub_571AA0;
    callbacks.follower_refresh = follower_ui_refresh;
    callbacks.field_9C = sub_56B290;
    callbacks.toggle_primary_button = intgame_toggle_primary_button;
    callbacks.set_map_button = intgame_set_map_button;
    callbacks.field_A8 = sub_57CE70;
    callbacks.field_AC = written_ui_newspaper_headline;
    callbacks.field_B0 = sub_57B180;
    callbacks.field_B4 = ui_charedit_error_msg;
    callbacks.field_B8 = sub_55A230;
    callbacks.progressbar_init = mainmenu_ui_progressbar_init;
    callbacks.progressbar_update = mainmenu_ui_progressbar_update;
    callbacks.field_C4 = sub_560790;
    callbacks.field_C8 = sub_56E720;
    callbacks.field_CC = sub_56E950;
    callbacks.field_D0 = sub_57CF70;
    callbacks.field_D4 = sub_578EA0;
    callbacks.field_D8 = sub_573630;
    callbacks.field_DC = sub_57CFA0;
    callbacks.field_E0 = sub_575C50;
    callbacks.field_E4 = gameuilib_reset;
    callbacks.field_E8 = dialog_ui_start_dialog;
    callbacks.field_EC = sub_5678D0;
    callbacks.field_F0 = sub_568880;
    callbacks.field_F4 = sub_568220;
    callbacks.field_F8 = sub_569C70;
    callbacks.field_FC = sub_549310;
    callbacks.refresh_cursor = intgame_refresh_cursor;
    callbacks.field_104 = sub_57A6C0;
    callbacks.field_108 = sub_575770;
    callbacks.wmap_rnd_timeevent_process = wmap_rnd_timeevent_process;
    callbacks.queue_slide = slide_ui_queue_slide;
    callbacks.field_118 = gameuilib_mod_load;
    callbacks.field_11C = sub_57CFB0;
    callbacks.field_120 = sub_57D080;
    callbacks.field_124 = sub_57D150;
    callbacks.field_128 = sub_56DBD0;
    callbacks.field_12C = sub_573600;
    callbacks.field_130 = sub_557790;
    callbacks.field_134 = sub_572370;
    callbacks.field_138 = sub_572510;
    callbacks.field_13C = sub_572640;
    callbacks.field_140 = inven_ui_create;
    callbacks.field_150 = sub_582670;
    callbacks.field_144 = sub_582510;
    callbacks.field_148 = sub_5825B0;
    callbacks.field_14C = sub_582650;
    callbacks.field_154 = sub_5826B0;
    callbacks.field_158 = sub_582690;
    callbacks.field_15C = sub_5826C0;
    callbacks.show_inven_npc_identify = show_inven_npc_identify;
    callbacks.field_164 = sub_55F360;
    callbacks.field_168 = sub_55F450;
    callbacks.field_16C = sub_55F5F0;
    ui_init(&callbacks);

    return true;
}

// 0x57CA90
void tb_ui_reset()
{
}

// 0x57CAA0
void tb_ui_exit()
{
    ui_exit();
}

// 0x57CAB0
void sub_57CAB0(int64_t obj)
{
    sub_576100(obj);
    if (player_is_pc_obj(obj)) {
        sub_556E60();
        sub_54B3C0();
    }
}

// 0x57CAE0
void sub_57CAE0()
{
    sub_551160();
    sub_55A230();
}

// 0x57CAF0
void sub_57CAF0(int64_t obj)
{
    sub_573590(obj);
    sub_568830(obj);
}

// 0x57CB10
void show_inven_loot(int64_t pc_obj, int64_t target_obj)
{
    if ((obj_field_int32_get(target_obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return;
    }

    if (obj_type_is_critter(obj_field_int32_get(target_obj, OBJ_F_TYPE))
        && obj_field_int32_get(target_obj, OBJ_F_CRITTER_INVENTORY_NUM) == 0) {
        intgame_there_is_nothing_to_loot();
        return;
    }

    inven_ui_open(pc_obj, target_obj, INVEN_UI_MODE_LOOT);
}

// 0x57CB80
void show_inven_identify(int64_t pc_obj, int64_t target_obj)
{
    if (player_is_pc_obj(pc_obj)) {
        inven_ui_open(pc_obj, target_obj, INVEN_UI_MODE_IDENTIFY);
    }
}

// 0x57CBC0
void show_inven_npc_identify(int64_t pc_obj, int64_t target_obj)
{
    inven_ui_open(pc_obj, target_obj, INVEN_UI_MODE_NPC_IDENTIFY);
}

// 0x57CBE0
void sub_57CBE0(char* str)
{
    UiMessage ui_message;

    ui_message.type = UI_MSG_TYPE_EXCLAMATION;
    ui_message.str = str;
    sub_550750(&ui_message);
}

// 0x57CC10
void sub_57CC10(int64_t obj)
{
    if (player_is_pc_obj(obj)) {
        sub_57D350(1, -1);
        if (!critter_is_active(obj)) {
            sub_575770();
        }
    }
    if (sub_43C570() == obj) {
        sub_57CCF0(player_get_pc_obj(), obj);
    }
    sub_56B4D0(obj);
}

// 0x57CC70
void sub_57CC70(int64_t a1, int64_t a2)
{
    int64_t v1;

    if (sub_441980(a1, a2, OBJ_HANDLE_NULL, SAP_DIALOG, 0) == 1) {
        if (sub_4AF210(a2, &v1) && sub_45E2E0(v1, a1)) {
            sub_4AF130(a2, a1);
        } else {
            sub_5681C0(a1, a2);
        }
    }
}

// 0x57CCF0
void sub_57CCF0(int64_t a1, int64_t a2)
{
    char buffer[2000];

    if (sub_57CD60(a1, a2, buffer)) {
        sub_553BE0(a1, a2, buffer);
        sub_557370(a1, a2);
    }
}

// 0x57CD60
bool sub_57CD60(int64_t a1, int64_t a2, char* buffer)
{
    if (!player_is_pc_obj(a1)) {
        return false;
    }

    if (!sub_441980(a1, a2, OBJ_HANDLE_NULL, SAP_EXAMINE, 0)) {
        return false;
    }

    sub_441B60(a2, a1, buffer);

    return true;
}

// 0x57CDC0
void sub_57CDC0()
{
    sub_57D370(10, -1, 5000);
}

// 0x57CDE0
void sub_57CDE0()
{
    sub_57D370(11, -1, 50);
}

// 0x57CDF0
void sub_57CDF0(int a1)
{
    sub_57EF90(a1);
}

// 0x57CE00
void sub_57CE00()
{
    sub_54B3C0();
}

// 0x57CE10
void sub_57CE10()
{
    intgame_hotkeys_recover();
    sub_551A80(0);
    sub_551A80(0);

    if (object_hover_obj != OBJ_HANDLE_NULL) {
        dword_5E2E94 = false;
        object_hover_obj = OBJ_HANDLE_NULL;

        object_hover_color = tig_color_make(255, 255, 255);
        tig_art_interface_id_create(467, dword_5E2E6C, 1, 0, &object_hover_underlay_art_id);
        tig_art_interface_id_create(468, dword_5E2E68, 1, 0, &object_hover_overlay_art_id);
        sub_443EB0(OBJ_HANDLE_NULL, &stru_5E2F60);
    }
}

// 0x57CE30
void sub_57CE30(int64_t obj, void* a2, int a3)
{
    if (!sub_45A030(a3)) {
        if (!sub_57BC70(obj, a3)) {
            sub_57C110(a2);
        }
        sub_57BFF0();
    }
}

// 0x57CE70
void sub_57CE70(int64_t a1, int64_t a2)
{
    if (a2 != OBJ_HANDLE_NULL) {
        if (tig_net_is_active()) {
            if (sub_4A2B10(a2) != -1) {
                sub_564F60(a2, a1);
            }
        } else {
            if (player_is_pc_obj(a2)) {
                sub_564F60(a2, a1);
            }
        }
    }
}

// 0x57CEE0
void ui_charedit_error_msg(int type, int a2)
{
    switch (type) {
    case 0:
        charedit_error_not_enough_character_points();
        break;
    case 1:
        charedit_error_not_enough_level();
        break;
    case 2:
        charedit_error_not_enough_intelligence();
        break;
    case 3:
        charedit_error_not_enough_willpower();
        break;
    case 4:
        charedit_error_skill_at_max();
        break;
    case 5:
        charedit_error_not_enough_stat(a2);
        break;
    case 6:
        charedit_error_skill_is_zero();
        break;
    case 7:
        charedit_error_skill_at_min();
        break;
    case 8:
        if (charedit_is_created()) {
            sub_55A230();
        }
        break;
    default:
        tig_debug_printf("UI: ui_charedit_error_msg() unknown error type %d\n", type);
        break;
    }
}

// 0x57CF70
void sub_57CF70(int64_t a1, int64_t a2)
{
    if (player_is_pc_obj(a1)) {
        charedit_create(a2, 2);
    }
}

// 0x57CFA0
void sub_57CFA0()
{
    sub_575930();
    intgame_refresh_cursor();
}

// 0x57CFB0
bool sub_57CFB0()
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    TigWindowModalDialogInfo modal_info;
    TigWindowModalDialogChoice choice;

    mes_load("mes\\MultiPlayer.mes", &mes_file);

    mes_file_entry.num = 1900; // "y"
    mes_get_msg(mes_file, &mes_file_entry);
    modal_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_OK] = mes_file_entry.str[0];

    mes_file_entry.num = 1901; // "n"
    mes_get_msg(mes_file, &mes_file_entry);
    modal_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_CANCEL] = mes_file_entry.str[0];

    mes_file_entry.num = 1902; // "Do you wish to save your character?"
    mes_get_msg(mes_file, &mes_file_entry);
    modal_info.text = mes_file_entry.str;

    modal_info.process = NULL;
    modal_info.type = TIG_WINDOW_MODAL_DIALOG_TYPE_OK_CANCEL;
    modal_info.x = 237;
    modal_info.y = 232;
    modal_info.redraw = sub_4045A0;
    tig_window_modal_dialog(&modal_info, &choice);

    mes_unload(mes_file);

    return choice == TIG_WINDOW_MODAL_DIALOG_CHOICE_OK;
}

// 0x57D080
bool sub_57D080()
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    TigWindowModalDialogInfo modal_info;
    TigWindowModalDialogChoice choice;

    mes_load("mes\\MultiPlayer.mes", &mes_file);

    mes_file_entry.num = 1900; // "y"
    mes_get_msg(mes_file, &mes_file_entry);
    modal_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_OK] = mes_file_entry.str[0];

    mes_file_entry.num = 1901; // "n"
    mes_get_msg(mes_file, &mes_file_entry);
    modal_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_CANCEL] = mes_file_entry.str[0];

    mes_file_entry.num = 1903; // "Do you wish to export your character to Multiplayer?"
    mes_get_msg(mes_file, &mes_file_entry);
    modal_info.text = mes_file_entry.str;

    modal_info.process = NULL;
    modal_info.type = TIG_WINDOW_MODAL_DIALOG_TYPE_OK_CANCEL;
    modal_info.x = 237;
    modal_info.y = 232;
    modal_info.redraw = sub_4045A0;
    tig_window_modal_dialog(&modal_info, &choice);

    mes_unload(mes_file);

    return choice == TIG_WINDOW_MODAL_DIALOG_CHOICE_OK;
}

// 0x57D150
bool sub_57D150(const char* name, const char* a2)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    char buffer[260];
    TigWindowModalDialogInfo modal_info;
    TigWindowModalDialogChoice choice;

    (void)a2;

    mes_load("mes\\MultiPlayer.mes", &mes_file);

    mes_file_entry.num = 1900; // "y"
    mes_get_msg(mes_file, &mes_file_entry);
    modal_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_OK] = mes_file_entry.str[0];

    mes_file_entry.num = 1901; // "n"
    mes_get_msg(mes_file, &mes_file_entry);
    modal_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_CANCEL] = mes_file_entry.str[0];

    mes_file_entry.num = 1902; // "The character %s already exists. Do you wish to overwrite the file?""
    mes_get_msg(mes_file, &mes_file_entry);
    snprintf(buffer, sizeof(buffer), mes_file_entry.str, name);
    modal_info.text = buffer;

    modal_info.process = NULL;
    modal_info.type = TIG_WINDOW_MODAL_DIALOG_TYPE_OK_CANCEL;
    modal_info.x = 237;
    modal_info.y = 232;
    modal_info.redraw = sub_4045A0;
    tig_window_modal_dialog(&modal_info, &choice);

    mes_unload(mes_file);

    return choice == TIG_WINDOW_MODAL_DIALOG_CHOICE_OK;
}
