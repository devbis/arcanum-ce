#include "game/combat.h"

#include "game/animfx.h"
#include "game/gamelib.h"
#include "game/mes.h"
#include "game/obj.h"
#include "game/object_node.h"
#include "game/player.h"
#include "game/random.h"

static void turn_based_changed();
static void fast_turn_based_changed();
static int sub_4B2810(int64_t obj);
static void sub_4B54B0(int64_t obj, int a2);
static int sub_4B65A0();
static bool sub_4B7580(ObjectNode* object_node);
static void combat_turn_based_subturn_start();
static void combat_turn_based_subturn_end();
static bool sub_4B7DC0(int64_t obj);

// 0x5FC178
static mes_file_handle_t combat_mes_file;

// 0x5FC1D8
static bool combat_editor;

// 0x5FC1F8
static AnimFxList stru_5FC1F8;

// 0x5FC224
static bool combat_turn_based;

// 0x5FC228
static bool combat_fast_turn_based;

// 0x5FC240
static ObjectNode* dword_5FC240;

// 0x5FC268
static bool in_combat_reset;

// 0x4B1D50
bool combat_init(GameInitInfo* init_info)
{
    combat_editor = init_info->editor;

    if (!mes_load("mes\\combat.mes", &combat_mes_file)) {
        return false;
    }

    if (!combat_editor) {
        if (!animfx_list_init(&stru_5FC1F8)) {
            return false;
        }

        stru_5FC1F8.path = "Rules\\CombatEyeCandy.mes";
        stru_5FC1F8.field_18 = 1;
        if (!animfx_list_load(&stru_5FC1F8)) {
            return false;
        }
    }

    settings_add(&settings, "turn-based", "0", turn_based_changed);
    combat_turn_based = settings_get_value(&settings, "turn-based");

    settings_add(&settings, "fast turn-based", "0", fast_turn_based_changed);
    combat_fast_turn_based = settings_get_value(&settings, "fast turn-based");

    settings_add(&settings, "auto attack", "0", NULL);
    settings_add(&settings, "combat taunts", "0", NULL);

    return true;
}

// 0x4B1E50
void combat_exit()
{
    mes_unload(combat_mes_file);
    sub_4B7330();
    if (!combat_editor) {
        animfx_list_exit(&stru_5FC1F8);
    }
}

// 0x4B1E80
void combat_reset()
{
    in_combat_reset = true;
    sub_4B7330();
    in_combat_reset = false;
}

// 0x4B1EA0
void combat_save()
{
    // TODO: Incomplete.
}

// 0x4B2020
void combat_load()
{
    // TODO: Incomplete.
}

// 0x4B2210
void sub_4B2210()
{
    // TODO: Incomplete.
}

// 0x4B23B0
int64_t sub_4B23B0(int64_t obj)
{
    return item_wield_get(obj, 1004);
}

// 0x4B23D0
void turn_based_changed()
{
    int value;

    value = settings_get_value(&settings, "turn-based");
    if (value && (tig_net_flags & 0x1) != 0) {
        settings_set_value(&settings, "turn-based", 0);
    } else {
        sub_4B6C90(value);
    }
}

// 0x4B2410
void fast_turn_based_changed()
{
    combat_fast_turn_based = settings_get_value(&settings, "fast turn-based");
}

// 0x4B24F0
void sub_4B24F0()
{
    // TODO: Incomplete.
}

// 0x4B2650
void sub_4B2650()
{
    // TODO: Incomplete.
}

// 0x4B2690
void sub_4B2690()
{
    // TODO: Incomplete.
}

// 0x4B2810
int sub_4B2810(int64_t obj)
{
    int flags;

    if (obj == OBJ_HANDLE_NULL
        || obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_WEAPON) {
        return 1;
    }

    flags = obj_field_int32_get(obj, OBJ_F_WEAPON_FLAGS);
    if ((flags & OWF_LOUD) != 0) {
        return 2;
    }

    if ((flags & OWF_SILENT) != 0) {
        return 0;
    }

    return 1;
}

// 0x4B2870
void sub_4B2870()
{
    // TODO: Incomplete.
}

// 0x4B2F60
void sub_4B2F60()
{
    // TODO: Incomplete.
}

// 0x4B3170
void sub_4B3170()
{
    // TODO: Incomplete.
}

// 0x4B3770
void sub_4B3770()
{
    // TODO: Incomplete.
}

// 0x4B39B0
void sub_4B39B0()
{
    // TODO: Incomplete.
}

// 0x4B3BB0
void sub_4B3BB0()
{
    // TODO: Incomplete.
}

// 0x4B3C00
void sub_4B3C00()
{
    // TODO: Incomplete.
}

// 0x4B3D50
bool combat_critter_is_combat_mode_active(int64_t obj)
{
    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_COMBAT_MODE_ACTIVE) != 0;
    } else {
        return false;
    }
}

// 0x4B3D90
void sub_4B3D90()
{
    // TODO: Incomplete.
}

// 0x4B3F20
void combat_critter_deactivate_combat_mode(int64_t obj)
{
    if (combat_critter_is_combat_mode_active(obj)) {
        combat_critter_toggle_combat_mode(obj);
    }
}

// 0x4B3F50
void combat_critter_activate_combat_mode(int64_t obj)
{
    if (!combat_critter_is_combat_mode_active(obj)) {
        combat_critter_toggle_combat_mode(obj);
    }
}

// 0x4B3F80
void combat_critter_toggle_combat_mode()
{
    // TODO: Incomplete.
}

// 0x4B4320
void sub_4B4320()
{
    // TODO: Incomplete.
}

// 0x4B4390
void sub_4B4390()
{
    // TODO: Incomplete.
}

// 0x4B54B0
void sub_4B54B0(int64_t obj, int a2)
{
    switch (a2) {
    case 1:
        return item_wield_get(obj, 1000);
    case 2:
        return item_wield_get(obj, 1007);
    case 3:
        return item_wield_get(obj, 1008);
    default:
        return item_wield_get(obj, 1006);
    }
}

// 0x4B5520
void sub_4B5520()
{
    // TODO: Incomplete.
}

// 0x4B5580
void sub_4B5580()
{
    // TODO: Incomplete.
}

// 0x4B5810
void sub_4B5810()
{
    // TODO: Incomplete.
}

// 0x4B58C0
void sub_4B58C0()
{
    // TODO: Incomplete.
}

// 0x4B5E90
void sub_4B5E90()
{
    // TODO: Incomplete.
}

// 0x4B5F30
void sub_4B5F30()
{
    // TODO: Incomplete.
}

// 0x4B5F40
void sub_4B5F40()
{
    // TODO: Incomplete.
}

// 0x4B6410
void sub_4B6410()
{
    // TODO: Incomplete.
}

// 0x4B65A0
int sub_4B65A0()
{
    int value = random_between(1, 100);
    if (value <= 70) return 0;
    if (value <= 85) return 1;
    if (value <= 95) return 2;
    return 3;
}

// 0x4B65D0
void sub_4B65D0()
{
    // TODO: Incomplete.
}

// 0x4B6680
void sub_4B6680()
{
    // TODO: Incomplete.
}

// 0x4B6860
void sub_4B6860()
{
    // TODO: Incomplete.
}

// 0x4B6930
void sub_4B6930()
{
    // TODO: Incomplete.
}

// 0x4B6A00
void sub_4B6A00()
{
    // TODO: Incomplete.
}

// 0x4B6B10
tig_art_id_t sub_4B6B10(tig_art_id_t aid, int v2)
{
    if (tig_art_type(aid) == TIG_ART_TYPE_ITEM) {
        aid = tig_art_id_rotation_set(aid, v2 / 4);
    } else {
        aid = sub_502780(aid, tig_art_num(aid) + v2 / 8);
        aid = tig_art_id_rotation_set(aid, v2 % 8);
    }

    return aid;
}

// 0x4B6B90
void sub_4B6B90()
{
    // TODO: Incomplete.
}

// 0x4B6C40
void sub_4B6C40()
{
    // TODO: Incomplete.
}

// 0x4B6C80
void combat_is_turn_based()
{
    // TODO: Incomplete.
}

// 0x4B6C90
void sub_4B6C90()
{
    // TODO: Incomplete.
}

// 0x4B6D20
void sub_4B6D20()
{
    // TODO: Incomplete.
}

// 0x4B6D70
void sub_4B6D70()
{
    // TODO: Incomplete.
}

// 0x4B6D80
void sub_4B6D80()
{
    // TODO: Incomplete.
}

// 0x4B6DA0
void combat_debug()
{
    // TODO: Incomplete.
}

// 0x4B6E70
void sub_4B6E70()
{
    // TODO: Incomplete.
}

// 0x4B7010
void sub_4B7010()
{
    // TODO: Incomplete.
}

// 0x4B7080
void sub_4B7080()
{
    // TODO: Incomplete.
}

// 0x4B7150
void combat_tb_timeevent_process()
{
    // TODO: Incomplete.
}

// 0x4B71E0
void sub_4B71E0()
{
    // TODO: Incomplete.
}

// 0x4B7300
void sub_4B7300()
{
    // TODO: Incomplete.
}

// 0x4B7330
void sub_4B7330()
{
    // TODO: Incomplete.
}

// 0x4B73A0
void combat_turn_based_begin_turn()
{
    // TODO: Incomplete.
}

// 0x4B7580
bool sub_4B7580(ObjectNode* object_node)
{
    if (dword_5FC240 == NULL) {
        return true;
    }

    if (!sub_45D790(object_node->obj)) {
        sub_427000(object_node->obj);
        return true;
    }

    if (sub_4B7DC0(object_node->obj)) {
        return true;
    }

    return false;
}

// 0x4B75D0
void combat_turn_based_subturn_start()
{
    if (dword_5FC240 != NULL) {
        combat_debug(dword_5FC240->obj, "SubTurn Start");
        sub_4B6E70(dword_5FC240->obj);
        sub_4B7790(dword_5FC240->obj, 0);
    } else {
        tig_debug_printf("Combat: combat_turn_based_subturn_start: ERROR: Couldn't start TB Combat Turn due to no Active Critters!\n");
        sub_4B7330();
    }
}

// 0x4B7630
void combat_turn_based_subturn_end()
{
    combat_debug(dword_5FC240->obj, "SubTurn End");
    if (player_is_pc_obj(dword_5FC240->obj)) {
        dword_5FC1EC(0);
    } else {
        sub_441980(dword_5FC240->obj, dword_5FC240->obj, OBJ_HANDLE_NULL, 16, 0);
    }
}

// 0x4B7690
void combat_turn_based_next_subturn()
{
    // TODO: Incomplete.
}

// 0x4B7740
void sub_4B7740()
{
    // TODO: Incomplete.
}

// 0x4B7780
void sub_4B7780()
{
    // TODO: Incomplete.
}

// 0x4B7790
void sub_4B7790()
{
    // TODO: Incomplete.
}

// 0x4B7830
void sub_4B7830()
{
    // TODO: Incomplete.
}

// 0x4B78D0
void sub_4B78D0()
{
    // TODO: Incomplete.
}

// 0x4B79A0
void sub_4B79A0()
{
    // TODO: Incomplete.
}

// 0x4B7A20
void sub_4B7A20()
{
    // TODO: Incomplete.
}

// 0x4B7AA0
void sub_4B7AA0()
{
    // TODO: Incomplete.
}

// 0x4B7AE0
void sub_4B7AE0()
{
    // TODO: Incomplete.
}

// 0x4B7BA0
void sub_4B7BA0()
{
    // TODO: Incomplete.
}

// 0x4B7C20
void sub_4B7C20()
{
    // TODO: Incomplete.
}

// 0x4B7C30
void sub_4B7C30()
{
    // TODO: Incomplete.
}

// 0x4B7C90
void sub_4B7C90()
{
    // TODO: Incomplete.
}

// 0x4B7CD0
void sub_4B7CD0()
{
    // TODO: Incomplete.
}

// 0x4B7DC0
bool sub_4B7DC0(int64_t obj)
{
    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_DONTDRAW) != 0) {
        return !critter_is_sleeping(obj);
    } else {
        return false;
    }
}

// 0x4B7E00
void combat_turn_based_add_critter()
{
    // TODO: Incomplete.
}

// 0x4B7EB0
void sub_4B7EB0()
{
    // TODO: Incomplete.
}

// 0x4B8040
void sub_4B8040()
{
    // TODO: Incomplete.
}

// 0x4B80D0
void sub_4B80D0()
{
    // TODO: Incomplete.
}

// 0x4B80E0
void sub_4B80E0()
{
    // TODO: Incomplete.
}

// 0x4B81B0
void sub_4B81B0()
{
    // TODO: Incomplete.
}

// 0x4B8230
bool combat_auto_attack_get(int64_t obj)
{
    int player;

    if (sub_4B6D70()) {
        return false;
    }

    if ((tig_net_flags & 0x1) == 0) {
        return settings_get_value(&settings, "auto attack");
    }

    player = sub_4A2B10(obj);
    if (player == -1) {
        return false;
    }

    return (sub_4A55D0(player) & 0x200) != 0;
}

// 0x4B8280
void combat_auto_attack_set(bool value)
{
    int player;

    settings_set_value(&settings, "auto attack", value);

    if ((tig_net_flags & 0x1) != 0) {
        player = sub_4A2B10(player_get_pc_obj());
        if (player != -1) {
            if (value) {
                sub_4A5510(player, 0x200);
            } else {
                sub_4A5570(player, 0x200);
            }
        }
    }
}

// 0x4B82E0
bool combat_taunts_get()
{
    if ((tig_net_flags & 0x1) == 0) {
        return settings_get_value(&settings, "combat taunts");
    } else {
        return false;
    }
}

// 0x4B8300
void combat_taunts_set(bool value)
{
    if ((tig_net_flags & 0x1) == 0) {
        settings_set_value(&settings, "combat taunts", value);
    }
}

// 0x4B8330
bool combat_auto_switch_weapons_get(int64_t obj)
{
    int player;

    if ((tig_net_flags & 0x1) == 0) {
        return settings_get_value(&settings, "auto switch");
    }

    player = sub_4A2B10(obj);
    if (player == -1) {
        return false;
    }

    return (sub_4A55D0(player) & 0x400) != 0;
}

// 0x4B8380
void combat_auto_switch_weapons_set(bool value)
{
    int player;

    settings_set_value(&settings, "auto switch", value);

    if ((tig_net_flags & 0x1) != 0) {
        player = sub_4A2B10(player_get_pc_obj());
        if (player != -1) {
            if (value) {
                sub_4A5510(player, 0x400);
            } else {
                sub_4A5570(player, 0x400);
            }
        }
    }
}

// 0x4B83E0
void sub_4B83E0()
{
    // TODO: Incomplete.
}
