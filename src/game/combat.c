#include "game/combat.h"

#include "game/ai.h"
#include "game/anim.h"
#include "game/animfx.h"
#include "game/critter.h"
#include "game/gamelib.h"
#include "game/item.h"
#include "game/mes.h"
#include "game/multiplayer.h"
#include "game/obj.h"
#include "game/object_node.h"
#include "game/object.h"
#include "game/player.h"
#include "game/random.h"
#include "game/reaction.h"
#include "game/skill.h"
#include "game/stat.h"
#include "game/text_floater.h"

static void turn_based_changed();
static void fast_turn_based_changed();
static int sub_4B2810(int64_t obj);
static void sub_4B2F60(CombatContext* combat);
static void sub_4B3770(CombatContext* combat);
static void sub_4B39B0(CombatContext* combat);
static void combat_critter_toggle_combat_mode(int64_t obj);
static int64_t sub_4B54B0(int64_t obj, int a2);
static void sub_4B5E90(int64_t loc);
static int sub_4B65A0();
static void sub_4B6B90(CombatContext* combat);
static void sub_4B7080();
static bool combat_turn_based_start();
static void combat_turn_based_end();
static bool combat_turn_based_begin_turn();
static void sub_4B7300();
static bool sub_4B7580(ObjectNode* object_node);
static void combat_turn_based_subturn_start();
static void combat_turn_based_subturn_end();
static void combat_turn_based_end_turn();
static int sub_4B7BA0(int64_t obj, int64_t a2, bool a3);
static bool sub_4B7DC0(int64_t obj);
static void sub_4B7EB0();
static void sub_4B83E0(int64_t obj, int64_t a2);

// 0x5B57B8
static int dword_5B57B8 = 15;

// 0x5B57FC
static int dword_5B57FC[2] = {
    0,
    10,
};

// 0x5FC178
static mes_file_handle_t combat_mes_file;

// 0x5FC180
static ObjectList stru_5FC180;

// 0x5FC1D0
static ObjectNode* dword_5FC1D0;

// 0x5FC1D8
static bool combat_editor;

// 0x5FC1F8
static AnimFxList stru_5FC1F8;

// 0x5FC224
static bool combat_turn_based;

// 0x5FC228
static bool combat_fast_turn_based;

// 0x5FC22C
static bool dword_5FC22C;

// 0x5FC230
static int dword_5FC230;

// 0x5FC234
static int combat_action_points;

// 0x5FC238
static int64_t qword_5FC238;

// 0x5FC240
static ObjectNode* dword_5FC240;

// 0x5FC244
static int dword_5FC244;

// 0x5FC248
static int64_t qword_5FC248;

// 0x5FC250
static int dword_5FC250;

// 0x5FC264
static bool dword_5FC264;

// 0x5FC268
static bool in_combat_reset;

// 0x5FC1E0
static CombatCallbacks combat_callbacks;

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
    combat_turn_based_end();
    if (!combat_editor) {
        animfx_list_exit(&stru_5FC1F8);
    }
}

// 0x4B1E80
void combat_reset()
{
    in_combat_reset = true;
    combat_turn_based_end();
    in_combat_reset = false;
}

// 0x4B1EA0
bool combat_save(TigFile* stream)
{
    ObjectID oid;

    if (stream == NULL) return false;
    if (tig_file_fwrite(&combat_turn_based, sizeof(combat_turn_based), 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_5FC22C, sizeof(dword_5FC22C), 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_5FC230, sizeof(dword_5FC230), 1, stream) != 1) return false;

    if (!dword_5FC22C) {
        return true;
    }

    if (tig_file_fwrite(&combat_action_points, sizeof(combat_action_points), 1, stream) != 1) return false;

    if (qword_5FC238 == OBJ_HANDLE_NULL) {
        oid.type = 0;
    } else {
        oid = sub_407EF0(qword_5FC238);
    }
    if (tig_file_fwrite(&oid, sizeof(oid), 1, stream) != 1) return false;

    oid = sub_407EF0(dword_5FC240->obj);
    if (tig_file_fwrite(&oid, sizeof(oid), 1, stream) != 1) return false;

    if (tig_file_fwrite(&dword_5FC244, sizeof(dword_5FC244), 1, stream) != 1) return false;

    oid = sub_407EF0(qword_5FC248);
    if (tig_file_fwrite(&oid, sizeof(oid), 1, stream) != 1) return false;

    return true;
}

// 0x4B2020
bool combat_load(GameLoadInfo* load_info)
{
    int saved_action_points;
    ObjectID oid;
    int64_t obj;
    ObjectNode* node;

    if (load_info->stream == NULL) return false;
    if (tig_file_fread(&combat_turn_based, sizeof(combat_turn_based), 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5FC22C, sizeof(dword_5FC22C), 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5FC230, sizeof(dword_5FC230), 1, load_info->stream) != 1) return false;

    if (!dword_5FC22C) {
        return true;
    }

    combat_callbacks.field_4();

    if (tig_file_fread(&combat_action_points, sizeof(combat_action_points), 1, load_info->stream) != 1) return false;
    saved_action_points = combat_action_points;
    dword_5FC22C = false;

    if (!combat_turn_based_start()) {
        return false;
    }

    combat_action_points = saved_action_points;

    if (tig_file_fread(&oid, sizeof(oid), 1, load_info->stream) != 1) return false;
    qword_5FC238 = objp_perm_lookup(oid);

    if (tig_file_fread(&oid, sizeof(oid), 1, load_info->stream) != 1) return false;
    obj = objp_perm_lookup(oid);
    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    node = stru_5FC180.head;
    while (node != NULL) {
        if (node->obj == obj) {
            break;
        }
        node = node->next;
    }

    if (node == NULL) {
        return false;
    }

    dword_5FC240 = node;

    if (tig_file_fread(&dword_5FC244, sizeof(dword_5FC244), 1, load_info->stream) != 1) return false;

    dword_5FC244 = 0;

    if (tig_file_fread(&oid, sizeof(oid), 1, load_info->stream) != 1) return false;
    qword_5FC248 = objp_perm_lookup(oid);
    if (qword_5FC248 == OBJ_HANDLE_NULL) {
        return false;
    }

    combat_callbacks.field_C(combat_action_points);

    return true;
}

// 0x4B2210
void sub_4B2210(int64_t attacker_obj, int64_t target_obj, CombatContext* combat)
{
    int type;

    combat->field_8 = attacker_obj;
    if (attacker_obj != OBJ_HANDLE_NULL) {
        type = obj_field_int32_get(attacker_obj, OBJ_F_TYPE);
        if (obj_type_is_critter(type)) {
            combat->weapon_obj = sub_4B23B0(attacker_obj);
        } else {
            combat->weapon_obj = OBJ_HANDLE_NULL;
        }
    }

    combat->skill = item_weapon_skill(combat->weapon_obj);

    if (attacker_obj != OBJ_HANDLE_NULL
        && obj_type_is_critter(type)
        && target_obj != OBJ_HANDLE_NULL
        && obj_type_is_critter(obj_field_int32_get(target_obj, OBJ_F_TYPE))
        && combat->skill == BASIC_SKILL_MELEE
        && critter_can_backstab(attacker_obj, target_obj)) {
        combat->flags |= 0x4000;
        if (sub_4AF260(target_obj, attacker_obj)
            && sub_4AF470(target_obj, attacker_obj, 0)) {
            combat->flags |= 0x8000;
        }
    }

    combat->field_20 = target_obj;
    combat->field_28 = target_obj;

    if (target_obj != OBJ_HANDLE_NULL) {
        combat->target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    }

    combat->field_30 = attacker_obj;
    combat->field_40 = 0;
    combat->field_58 = 0;
    combat->field_5C = 0;
    combat->field_44[0] = 0;
    combat->field_44[1] = 0;
    combat->field_44[2] = 0;
    combat->field_44[3] = 0;
    combat->field_44[4] = 0;

    if (combat->skill == BASIC_SKILL_THROWING
        || item_weapon_range(combat->weapon_obj, combat->field_8) > 1) {
        combat->flags |= 0x200;
    }
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
void sub_4B2650(int64_t a1, int64_t a2, CombatContext* combat)
{
    sub_4B2690(a1, a2, combat != NULL ? combat->field_28 : OBJ_HANDLE_NULL, false);
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
void sub_4B2F60(CombatContext* combat)
{
    int sound_id;

    if ((combat->flags & 0x2) != 0) {
        if (combat->weapon_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->field_8, combat->weapon_obj, combat->field_20, 7, 0);

            if ((combat->flags & 0x4) != 0
                && obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
                sub_441980(combat->field_8, combat->weapon_obj, combat->field_20, 34, 0);
            }
        }

        if (combat->field_8 != OBJ_HANDLE_NULL
            && combat->field_20 != OBJ_HANDLE_NULL) {
            sub_441980(combat->field_20, combat->field_8, OBJ_HANDLE_NULL, 34, 0);
        }

        sub_4B6680(combat);
        sub_4B4390(combat);

        if ((combat->flags & 0x4) != 0) {
            sound_id = sub_4F0ED0(combat->field_20, 0);
            sub_41B930(sound_id, 1, combat->field_20);

            sound_id = sub_4F0BF0(combat->weapon_obj, combat->field_8, combat->field_20, 6);
            sub_41B930(sound_id, 1, combat->field_8);
        } else {
            sound_id = sub_4F0BF0(combat->weapon_obj, combat->field_8, combat->field_20, 4);
            sub_41B930(sound_id, 1, combat->field_8);
        }
    } else {
        sound_id = sub_4F0BF0(combat->weapon_obj, combat->field_8, combat->field_20, 5);
        sub_41B930(sound_id, 1, combat->field_8);

        if (combat->field_20 != OBJ_HANDLE_NULL
            && obj_field_int32_get(combat->field_20, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
            sub_4A84F0(combat->field_20);
        }

        if (combat->weapon_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->field_8, combat->weapon_obj, combat->field_20, 8, 0);

            if ((combat->flags & 0x04) != 0
                && obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
                sub_441980(combat->field_8, combat->weapon_obj, combat->field_20, 35, 0);
            }
        }
    }
}

// 0x4B3170
void sub_4B3170()
{
    // TODO: Incomplete.
}

// 0x4B3770
void sub_4B3770(CombatContext* combat)
{
    int sound_id;
    CombatContext body_of_fire;

    if ((combat->flags & 0x2) != 0) {
        if (combat->weapon_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->field_8, combat->weapon_obj, combat->field_20, 7, 0);

            if ((combat->flags & 0x4) != 0) {
                sub_441980(combat->field_8, combat->weapon_obj, combat->field_20, 34, 0);
            }
        }

        if (combat->field_8 != OBJ_HANDLE_NULL
            && combat->field_20 != OBJ_HANDLE_NULL) {
            sub_441980(combat->field_20, combat->field_8, OBJ_HANDLE_NULL, 26, 0);
        }

        sub_4B6680(combat);
        sub_4B4390(combat);

        if ((combat->flags & 0x4) != 0) {
            sound_id = sub_4F0ED0(combat->field_20, 0);
            sub_41B930(sound_id, 1, combat->field_20);

            sound_id = sub_4F0BF0(combat->weapon_obj, combat->field_8, combat->field_20, 6);
            sub_41B930(sound_id, 1, combat->field_8);
        } else {
            sound_id = sub_4F0BF0(combat->weapon_obj, combat->field_8, combat->field_20, 4);
            sub_41B930(sound_id, 1, combat->field_8);
        }

        if ((obj_field_int32_get(combat->field_8, OBJ_F_SPELL_FLAGS) & OSF_BODY_OF_FIRE) == 0
            && (obj_field_int32_get(combat->field_20, OBJ_F_SPELL_FLAGS) & OSF_BODY_OF_FIRE) != 0) {
            sub_4B2210(combat->field_20, combat->field_8, &body_of_fire);
            body_of_fire.field_44[3] = 5;
            sub_4B4390(&body_of_fire);
        }
    } else {
        sound_id = sub_4F0BF0(combat->weapon_obj, combat->field_8, combat->field_20, 5);
        sub_41B930(sound_id, 1, combat->field_8);

        if (obj_field_int32_get(combat->field_20, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
            sub_4A84F0(combat->field_20);
        }

        if (combat->weapon_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->field_8, combat->weapon_obj, combat->field_20, 8, 0);

            if ((combat->flags & 0x4) != 0) {
                sub_441980(combat->field_8, combat->weapon_obj, combat->field_20, 35, 0);
            }
        }
    }
}

// 0x4B39B0
void sub_4B39B0(CombatContext* combat)
{
    bool is_boomerangs = false;
    tig_art_id_t missile_art_id = TIG_ART_ID_INVALID;
    int64_t loc;
    int range;
    int max_range;
    int rotation;
    int num_arrows;
    int arrow;

    if ((combat->flags & 0x100) == 0
        && combat->weapon_obj != OBJ_HANDLE_NULL) {
        if (obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
            missile_art_id = obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_MISSILE_AID);
            is_boomerangs = (obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_FLAGS) & OWF_BOOMERANGS) != 0;
        }
    } else {
        tig_art_scenery_id_create(0, 0, 0, 0, 0, &missile_art_id);
    }

    if (missile_art_id == TIG_ART_ID_INVALID) {
        if (combat->skill == BASIC_SKILL_THROWING) {
            missile_art_id = obj_field_int32_get(combat->weapon_obj, OBJ_F_CURRENT_AID);
        }
    }

    if (missile_art_id == TIG_ART_ID_INVALID) {
        sub_4B2F60(combat);
        return;
    }

    loc = obj_field_int64_get(combat->field_8, OBJ_F_LOCATION);
    if (combat->skill == BASIC_SKILL_THROWING
        && !is_boomerangs
        && (combat->flags & 0x02) == 0) {
        range = (20 - basic_skill_level(combat->field_8, BASIC_SKILL_THROWING)) / 5 + 1;
        max_range = sub_4B96F0(loc, combat->target_loc);
        if (range > max_range) {
            range = max_range;
        }

        sub_4B90D0(combat->target_loc, random_between(0, 7), &(combat->target_loc));
        combat->field_20 = OBJ_HANDLE_NULL;
        combat->flags |= 0x20;
    }

    if (sub_4A2BA0() || (tig_net_flags & TIG_NET_HOST) != 0) {
        // FIXME: Unused.
        rotation = tig_art_id_rotation_get(obj_field_int32_get(combat->field_8, OBJ_F_CURRENT_AID));

        num_arrows = 1;
        if (combat->skill == BASIC_SKILL_BOW
            && basic_skill_get_training(combat->field_8, BASIC_SKILL_BOW) >= TRAINING_EXPERT) {
            num_arrows = 2;
        }

        for (arrow = 0; arrow < num_arrows; arrow++) {
            sub_4B24F0(combat, loc, dword_5B57FC[arrow], dword_5B57FC[arrow], missile_art_id);
        }
    }
}

// 0x4B3BB0
void sub_4B3BB0(int64_t attacker_obj, int64_t target_obj, int a3)
{
    CombatContext combat;

    sub_4B2210(attacker_obj, target_obj, &combat);
    combat.field_40 = a3;
    combat.flags |= 0x50000;
    sub_4B3170(&combat);
}

// 0x4B3C00
void sub_4B3C00(int64_t attacker_obj, int64_t weapon_obj, int64_t target_obj, int64_t target_loc, int a5)
{
    int64_t attacker_loc;
    CombatContext combat;

    attacker_loc = obj_field_int64_get(attacker_obj, OBJ_F_LOCATION);
    if (obj_field_int32_get(attacker_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        item_remove(weapon_obj);
    }

    if (sub_441980(attacker_obj, weapon_obj, target_obj, 6, 0)) {
        sub_4B2210(attacker_obj, target_obj, &combat);
        combat.field_40 = a5;
        combat.weapon_obj = weapon_obj;
        if (target_obj == OBJ_HANDLE_NULL) {
            combat.target_loc = target_loc;
        }
        combat.flags &= ~0xC000;
        combat.flags |= 0x40240;
        sub_4EFF50(weapon_obj, 2);
        sub_4415C0(weapon_obj, attacker_loc);
        sub_4B3170(&combat);
    } else {
        sub_4415C0(weapon_obj, attacker_loc);
        sub_4617F0(weapon_obj, attacker_obj);
    }
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
bool sub_4B3D90(int64_t obj)
{
    bool ret = true;
    ObjectList pcs;
    ObjectNode* pc_node;
    ObjectList npcs;
    ObjectNode* npc_node;
    int64_t combat_focus_obj;

    if (!combat_critter_is_combat_mode_active(obj)) {
        return true;
    }

    if (!sub_45D790(obj)) {
        return true;
    }

    if (sub_44E830(obj, 19, NULL)) {
        return false;
    }

    sub_441260(obj, &pcs);
    sub_440FC0(obj, OBJ_TM_NPC, &npcs);

    npc_node = npcs.head;
    while (npc_node != NULL && ret) {
        combat_focus_obj = obj_field_handle_get(npc_node->obj, OBJ_F_NPC_COMBAT_FOCUS);
        if (combat_focus_obj == obj) {
            if (sub_45D790(npc_node->obj) && ai_is_fighting(npc_node->obj)) {
                ret = false;
                break;
            }
        } else {
            pc_node = pcs.head;
            while (pc_node != NULL) {
                if (combat_focus_obj == obj
                    && sub_45D790(npc_node->obj)
                    && ai_is_fighting(npc_node->obj)) {
                    ret = false;
                    break;
                }
                pc_node = pc_node->next;
            }
        }
        npc_node = npc_node->next;
    }

    object_list_destroy(&npcs);
    object_list_destroy(&pcs);

    return ret;
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
void combat_critter_toggle_combat_mode(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x4B4320
void sub_4B4320(int64_t obj)
{
    if (obj != OBJ_HANDLE_NULL
        && (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
            || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC)
        && ((!dword_5FC22C && combat_critter_is_combat_mode_active(obj))
            || (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2) & OCF2_AUTO_ANIMATES) != 0)) {
        sub_435BD0(obj);
    }
}

// 0x4B4390
void sub_4B4390()
{
    // TODO: Incomplete.
}

// 0x4B54B0
int64_t sub_4B54B0(int64_t obj, int a2)
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
void sub_4B58C0(CombatContext* combat)
{
    Packet21 pkt;
    int type;
    int index;
    unsigned int critter_flags;
    bool v1 = false;
    bool v2 = false;
    tig_art_id_t art_id;

    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        pkt.type = 21;
        sub_4F0640(combat->field_8, &(pkt.field_70));
        sub_4F0640(combat->weapon_obj, &(pkt.field_88));
        sub_4F0640(combat->field_20, &(pkt.field_A0));
        sub_4F0640(combat->field_28, &(pkt.field_B8));
        sub_4F0640(combat->field_30, &(pkt.field_D0));
        pkt.combat = *combat;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    type = obj_field_int32_get(combat->field_20, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    if ((combat->field_58 & 0x1000) != 0
        && (combat->field_58 & 0x1) == 0) {
        for (index = 0; index < 5; index++) {
            sub_4EA2E0(combat->field_20, 7);
        }
    }

    if (sub_45D8D0(combat->field_20)) {
        if ((combat->field_58 & 0x2) == 0) {
            return;
        }
    }

    critter_flags = obj_field_int32_get(combat->field_20, OBJ_F_CRITTER_FLAGS);
    if ((critter_flags & OCF_UNREVIVIFIABLE) != 0
        && (combat->field_58 & 0x400000) != 0) {
        v1 = true;
    }
    if ((critter_flags & OCF_UNRESSURECTABLE) != 0
        && (combat->field_58 & 0x400000) == 0) {
        v1 = true;
    }

    if (!sub_441980(combat->field_8, combat->field_20, combat->weapon_obj, 18, 0) || v1) {
        sub_45A540(combat->field_20);
        return;
    }

    art_id = obj_field_int32_get(combat->field_20, OBJ_F_CURRENT_AID);
    art_id = sub_503E50(art_id, 0);
    art_id = tig_art_id_frame_set(art_id, 0);
    object_set_current_aid(combat->field_20, art_id);

    v2 = true;

    sub_4B5E90(combat->target_loc);
    sub_4AF170(combat->field_20);
    sub_43D280(combat->field_20, OF_FLAT | OF_NO_BLOCK);

    critter_flags = obj_field_int32_get(combat->field_20, OBJ_F_CRITTER_FLAGS);
    critter_flags &= ~OCF_STUNNED;
    obj_field_int32_set(combat->field_20, OBJ_F_CRITTER_FLAGS, critter_flags);

    sub_45EC80(combat->field_20);

    if (critter_fatigue_damage_get(combat->field_20) != 10) {
        critter_fatigue_damage_set(combat->field_20, 10);
    }

    sub_459740(combat->field_20);

    if (combat->field_8 != OBJ_HANDLE_NULL
        && obj_field_int32_get(combat->field_8, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        int reaction_level = sub_4C0CC0(combat->field_20, combat->field_8);
        if (reaction_level > 0 && reaction_level < 70) {
            sub_4C0DE0(combat->field_20, combat->field_8, 70 - reaction_level);
        }
    }

    if (type == OBJ_TYPE_NPC) {
        int64_t leader_obj = critter_leader_get(combat->field_20);
        if (leader_obj != OBJ_HANDLE_NULL) {
            if (!critter_follow(combat->field_20, leader_obj, false)) {
                critter_leader_set(combat->field_20, OBJ_HANDLE_NULL);
            }
        }
    }

    if ((combat->field_58 & 0x1) != 0) {
        combat->field_44[0] = object_get_hp_damage(combat->field_20);
        if (combat->field_44[4] > 0) {
            combat->field_44[4] = critter_fatigue_damage_get(combat->field_20);
        }

        for (index = 0; index < 5; index++) {
            sub_4EA2E0(combat->field_20, 7);
        }

        critter_flags = obj_field_int32_get(combat->field_20, OBJ_F_CRITTER_FLAGS);
        if ((critter_flags & OCF_BLINDED) != 0) {
            critter_flags &= ~OCF_BLINDED;
            obj_field_int32_set(combat->field_20, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((critter_flags & OCF_CRIPPLED_ARMS_ONE) != 0) {
            critter_flags &= ~OCF_CRIPPLED_ARMS_ONE;
            obj_field_int32_set(combat->field_20, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((critter_flags & OCF_CRIPPLED_ARMS_BOTH) != 0) {
            critter_flags &= ~OCF_CRIPPLED_ARMS_BOTH;
            obj_field_int32_set(combat->field_20, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((critter_flags & OCF_CRIPPLED_LEGS_BOTH) != 0) {
            critter_flags &= ~OCF_CRIPPLED_LEGS_BOTH;
            obj_field_int32_set(combat->field_20, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
    } else {
        critter_flags = obj_field_int32_get(combat->field_20, OBJ_F_CRITTER_FLAGS);
        if ((combat->field_58 & 0x200) != 0
            && (critter_flags & OCF_BLINDED) != 0) {
            critter_flags &= ~OCF_BLINDED;
            obj_field_int32_set(combat->field_20, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((combat->field_58 & 0x400) != 0
            && (critter_flags & (OCF_CRIPPLED_ARMS_ONE | OCF_CRIPPLED_ARMS_BOTH)) != 0) {
            critter_flags &= ~(OCF_CRIPPLED_ARMS_ONE | OCF_CRIPPLED_ARMS_BOTH);
            obj_field_int32_set(combat->field_20, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((combat->field_58 & 0x800) != 0
            && (critter_flags & OCF_CRIPPLED_LEGS_BOTH) != 0) {
            critter_flags &= ~OCF_BLINDED;
            obj_field_int32_set(combat->field_20, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
    }

    if (combat->field_44[0] > 0) {
        int new_dam;
        int cur_dam;

        cur_dam = object_get_hp_damage(combat->field_20);
        if (cur_dam != 0) {
            new_dam = cur_dam - combat->field_44[0];
            if (new_dam < 0) {
                new_dam = 0;
            }
            object_set_hp_damage(combat->field_20, new_dam);
        }
    }

    if (combat->field_44[4] > 0) {
        int new_dam;
        int cur_dam;
        int v1;

        cur_dam = critter_fatigue_damage_get(combat->field_20);
        if (cur_dam != 0) {
            v1 = sub_45D700(combat->field_20);
            new_dam = cur_dam - combat->field_44[4];
            if (new_dam < 0) {
                new_dam = 0;
            }
            critter_fatigue_damage_set(combat->field_20, new_dam);

            if (v1 <= 0 && sub_45D700(combat->field_20) > 0) {
                sub_434DE0(combat->field_20);
            }
        }
    }

    if (combat->field_44[1] > 0) {
        int new_dam;
        int cur_dam;

        cur_dam = stat_get_base(combat->field_20, STAT_POISON_LEVEL);
        if (cur_dam != 0) {
            new_dam = cur_dam - combat->field_44[1];
            if (new_dam < 0) {
                new_dam = 0;
            }
            stat_set_base(combat->field_20, STAT_POISON_LEVEL, new_dam);
        }
    }

    if (v2 && type != OBJ_TYPE_PC) {
        sub_4AD6E0(combat->field_20);
    }

    if (type == OBJ_TYPE_NPC) {
        if (sub_45DDA0(combat->field_20) == player_get_pc_obj()) {
            sub_460780();
        }
    }

    sub_4B80E0(combat->field_20);
}

// 0x4B5E90
void sub_4B5E90(int64_t loc)
{
    ObjectList objects;
    ObjectNode* node;
    tig_art_id_t art_id;
    int64_t obj;

    sub_4407C0(loc, OBJ_TM_SCENERY, &objects);

    node = objects.head;
    while (node != NULL) {
        art_id = obj_field_int32_get(node->obj, OBJ_F_CURRENT_AID);
        if (tig_art_scenery_id_type_get(art_id) == TIG_ART_SCENERY_TYPE_MISC_SMALL
            && tig_art_num_get(art_id) == 0) {
            obj = node->obj;
            object_list_destroy(&objects);
            sub_45EC80(obj);
            sub_43CCA0(obj);
            return;
        }
        node = node->next;
    }

    object_list_destroy(&objects);
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
        aid = tig_art_num_set(aid, tig_art_num_get(aid) + v2 / 8);
        aid = tig_art_id_rotation_set(aid, v2 % 8);
    }

    return aid;
}

// 0x4B6B90
void sub_4B6B90(CombatContext* combat)
{
    int64_t pc_obj;

    // FIXME: Unused.
    pc_obj = player_get_pc_obj();

    if (!combat_taunts_get()) {
        return;
    }

    if (obj_field_int32_get(combat->field_8, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (combat->field_20 == OBJ_HANDLE_NULL) {
        return;
    }

    if (combat->field_20 == combat->field_8) {
        return;
    }

    if ((combat->flags & 0x2) == 0) {
        return;
    }

    if ((combat->field_20 == combat->field_28)) {
        if ((combat->flags & 0x4) != 0) {
            sub_4AE9E0(combat->field_8, false);
        }
    } else {
        if ((combat->flags & 0x08) != 0) {
            sub_4AE9E0(combat->field_8, true);
        }
    }
}

// 0x4B6C40
bool combat_set_callbacks(CombatCallbacks* callbacks)
{
    combat_callbacks = *callbacks;

    return true;
}

// 0x4B6C80
bool combat_is_turn_based()
{
    return combat_turn_based;
}

// 0x4B6C90
bool sub_4B6C90(bool turn_based)
{
    int64_t pc;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0 && turn_based) {
        return false;
    }

    if (combat_turn_based == turn_based) {
        return true;
    }

    pc = player_get_pc_obj();
    if (!sub_424070(pc, 3, 0, 1)) {
        return false;
    }

    if (!combat_critter_is_combat_mode_active(pc)) {
        combat_turn_based = turn_based;
        return true;
    }

    if (!turn_based) {
        combat_callbacks.field_8();
        combat_turn_based_end();
        combat_turn_based = turn_based;
        return true;
    }

    combat_callbacks.field_4();
    if (combat_turn_based_start()) {
        combat_turn_based = turn_based;
        return true;
    }

    combat_callbacks.field_8();
    return 1;
}

// 0x4B6D20
void sub_4B6D20()
{
    if (combat_is_turn_based()) {
        settings_set_value(&settings, "turn-based", 0);
    } else {
        if (sub_45D790(player_get_pc_obj())) {
            settings_set_value(&settings, "turn-based", 1);
        }
    }
}

// 0x4B6D70
bool sub_4B6D70()
{
    return dword_5FC22C;
}

// 0x4B6D80
int64_t sub_4B6D80()
{
    if (dword_5FC240 != NULL) {
        return dword_5FC240->obj;
    } else {
        return OBJ_HANDLE_NULL;
    }
}

// 0x4B6DA0
void combat_debug(int64_t obj, const char* msg)
{
    // TODO: Incomplete.
}

// 0x4B6E70
void sub_4B6E70(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x4B7010
void sub_4B7010(int64_t obj)
{
    if (obj == dword_5FC240->obj
        && !sub_40DA20(obj)
        && !gamelib_in_load()) {
        if (sub_441980(obj, obj, OBJ_HANDLE_NULL, 19, 0) == 1) {
            sub_4A84F0(obj);
        }

        if (!sub_423300(obj, NULL)) {
            combat_action_points = 0;
            sub_4B7080();
        }
    }
}

// 0x4B7080
void sub_4B7080()
{
    // TODO: Incomplete.
}

// 0x4B7150
bool combat_tb_timeevent_process(TimeEvent* timeevent)
{
    combat_debug(OBJ_HANDLE_NULL, "TimeEvent Process");

    if (dword_5FC22C) {
        if (dword_5FC240 != NULL
            && sub_40DA20(dword_5FC240->obj)
            && combat_action_points > 0) {
            return true;
        }

        if (dword_5FC264) {
            dword_5FC264 = false;
            sub_4B7CD0(dword_5FC240->obj, combat_action_points);
        }

        if (combat_get_action_points() <= 0) {
            combat_turn_based_next_subturn();
        }
    }

    return true;
}

// 0x4B71E0
bool combat_turn_based_start()
{
    int64_t loc;
    LocRect loc_rect;
    ObjectNode* node;

    combat_debug(OBJ_HANDLE_NULL, "TB Start");
    if (dword_5FC22C) {
        return true;
    }

    dword_5FC230 = 0;

    if (!anim_goal_interrupt_all_for_tb_combat()) {
        tig_debug_printf("Combat: TB_Start: Anim-Goal-Interrupt FAILED!\n");
    }

    dword_5FC250 = 0;

    sub_423FE0(sub_4B7080);

    loc = obj_field_int64_get(player_get_pc_obj(), OBJ_F_LOCATION);
    sub_4B7300();

    loc_rect.x1 = LOCATION_GET_X(loc) - dword_5B57B8;
    loc_rect.x2 = LOCATION_GET_X(loc) + dword_5B57B8;
    loc_rect.y1 = LOCATION_GET_Y(loc) - dword_5B57B8;
    loc_rect.y2 = LOCATION_GET_Y(loc) + dword_5B57B8;
    sub_440B40(&loc_rect, 0x18000, &stru_5FC180);
    sub_4B7EB0();

    node = stru_5FC180.head;
    while (node != NULL) {
        sub_424070(node->obj, 3, 0, 1);
        node = node->next;
    }

    dword_5FC22C = true;
    return combat_turn_based_begin_turn();
}

// 0x4B7300
void sub_4B7300()
{
    dword_5B57B8 = stat_level(player_get_pc_obj(), STAT_PERCEPTION) / 2 + 5;
    if (dword_5B57B8 < 10) {
        dword_5B57B8 = 10;
    }
}

// 0x4B7330
void combat_turn_based_end()
{
    ObjectNode* node;

    combat_debug(OBJ_HANDLE_NULL, "TB End");

    if (dword_5FC22C) {
        dword_5FC22C = false;
        sub_423FE0(0);

        if (!in_combat_reset) {
            node = stru_5FC180.head;
            while (node != NULL) {
                animfx_remove(&stru_5FC1F8, node->obj, 0, -1);
                node = node->next;
            }
        }

        object_list_destroy(&stru_5FC180);
    }
}

// 0x4B73A0
bool combat_turn_based_begin_turn()
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
        combat_turn_based_end();
    }
}

// 0x4B7630
void combat_turn_based_subturn_end()
{
    combat_debug(dword_5FC240->obj, "SubTurn End");
    if (player_is_pc_obj(dword_5FC240->obj)) {
        combat_callbacks.field_C(0);
    } else {
        sub_441980(dword_5FC240->obj, dword_5FC240->obj, OBJ_HANDLE_NULL, 16, 0);
    }
}

// 0x4B7690
void combat_turn_based_next_subturn()
{
    combat_debug(dword_5FC240->obj, "Next SubTurn");

    dword_5FC250++;
    combat_turn_based_subturn_end();

    dword_5FC240 = dword_5FC240->next;
    if (dword_5FC240 == NULL) {
        combat_turn_based_end_turn();
        return;
    }

    if (sub_4B7580(dword_5FC240)) {
        while (dword_5FC240 != NULL) {
            dword_5FC240 = dword_5FC240->next;
            dword_5FC250++;
            if (dword_5FC240 == NULL) {
                break;
            }
            if (!sub_4B7580(dword_5FC240)) {
                break;
            }
        }
        if (dword_5FC240 == NULL) {
            combat_turn_based_end_turn();
        }
    }

    if (dword_5FC240 != NULL) {
        combat_turn_based_subturn_start();
        return;
    }

    tig_debug_printf("Combat: combat_turn_based_next_subturn: ERROR: Couldn't start TB Combat Turn due to no Active Critters!\n");
    combat_turn_based_end();
}

// 0x4B7740
void combat_turn_based_end_turn()
{
    DateTime datetime;

    combat_debug(OBJ_HANDLE_NULL, "TB End Turn");
    sub_45A950(&datetime, 1000);
    sub_45C200(&datetime);
    combat_turn_based_begin_turn();
}

// 0x4B7780
int combat_get_action_points()
{
    return combat_action_points;
}

// 0x4B7790
bool sub_4B7790(int64_t obj, int a2)
{
    bool is_pc;

    if (!dword_5FC22C) {
        return true;
    }

    if (dword_5FC240->obj != obj) {
        return true;
    }

    dword_5FC244 = a2;

    is_pc = player_is_pc_obj(obj);
    if (is_pc) {
        combat_callbacks.field_C(combat_action_points);
    }

    if (combat_action_points >= a2) {
        return true;
    }

    if (combat_action_points > 0 && is_pc && sub_45D700(obj) > 1) {
        return true;
    }

    return false;
}

// 0x4B7830
bool sub_4B7830(int64_t a1, int64_t a2)
{
    int type;

    if (!dword_5FC22C) {
        return sub_4B7790(a1, 0);
    }

    type = obj_field_int32_get(a1, OBJ_F_TYPE);
    if (obj_type_is_item(type)
        && (obj_field_int32_get(a1, OBJ_F_FLAGS) & OF_INVENTORY) != 0
        && obj_field_handle_get(a1, OBJ_F_ITEM_PARENT) != OBJ_HANDLE_NULL) {
        return sub_4B7790(a1, 0);
    }

    return sub_4B7790(a1, sub_4B7BA0(a1, a2, 0));
}

// 0x4B78D0
bool sub_4B78D0(int64_t a1, int64_t a2)
{
    int v1;
    int64_t weapon_obj;
    int64_t loc1;
    int64_t loc2;

    if (!dword_5FC22C) {
        return sub_4B7790(a1, 0);
    }

    v1 = sub_4B7C30(a1);
    weapon_obj = item_wield_get(a1, 1004);
    if (weapon_obj == OBJ_HANDLE_NULL || item_weapon_range(weapon_obj, a1) <= 1) {
        loc1 = obj_field_int64_get(a1, OBJ_F_LOCATION);
        loc2 = obj_field_int64_get(a2, OBJ_F_LOCATION);
        if (sub_4B96F0(loc1, loc2) > 1) {
            v1 += sub_4B7BA0(a1, loc2, 1);
        }
    }

    return v1;
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
bool sub_4B7AA0(int64_t obj)
{
    if (dword_5FC22C) {
        return sub_4B7790(obj, 4);
    } else {
        return sub_4B7790(obj, 0);
    }
}

// 0x4B7AE0
bool sub_4B7AE0(int64_t obj)
{
    if (dword_5FC22C) {
        return sub_4B7790(obj, 4);
    } else {
        return sub_4B7790(obj, 0);
    }
}

// 0x4B7BA0
int sub_4B7BA0(int64_t obj, int64_t a2, bool a3)
{
    int v1;

    v1 = 2;
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        && get_always_run(obj)) {
        v1 = 1;
    }

    if (a3) {
        return v1 * sub_426250(obj, a2);
    } else {
        return v1 * sub_4261E0(obj, a2);
    }
}

// 0x4B7C20
int sub_4B7C20()
{
    return dword_5FC244;
}

// 0x4B7C30
int sub_4B7C30(int64_t obj)
{
    int64_t weapon_obj;
    int speed;
    int v1;

    weapon_obj = item_wield_get(obj, 1004);
    speed = item_weapon_magic_speed(weapon_obj, obj);
    if (speed > 24) {
        v1 = 1;
    } else if (speed > 20) {
        v1 = 2;
    } else {
        v1 = 8 - speed / 3;
        if (v1 < 1) {
            v1 = 1;
        }
    }

    return v1;
}

// 0x4B7C90
void sub_4B7C90(int64_t obj)
{
    if (dword_5FC22C
        && dword_5FC240->obj == obj) {
        combat_action_points = 0;
        combat_turn_based_next_subturn();
    }
}

// 0x4B7CD0
void sub_4B7CD0(int64_t obj, int action_points)
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
void combat_turn_based_add_critter(int64_t obj)
{
    ObjectNode* prev = NULL;
    ObjectNode* curr;

    if (!dword_5FC22C) {
        return;
    }

    curr = dword_5FC1D0;
    while (curr != NULL && curr->obj != obj) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != NULL) {
        // Already added.
        return;
    }

    if (sub_4B7DC0(obj)) {
        tig_debug_printf("Combat: combat_turn_based_add_critter: WARNING: Attempt to add critter that is OF_DONTDRAW!\n");
    }

    curr = object_node_create();
    curr->obj = obj;
    curr->next = NULL;
    combat_debug(obj, "Adding Critter to List");

    if (prev != NULL) {
        prev->next = curr;
    } else {
        tig_debug_printf("Combat: combat_turn_based_add_critter: ERROR: Base list is EMPTY!\n");
        dword_5FC1D0 = curr;
    }

    sub_4B7EB0();
}

// 0x4B7EB0
void sub_4B7EB0()
{
    // TODO: Incomplete.
}

// 0x4B8040
bool sub_4B8040(int64_t obj)
{
    if (!dword_5FC22C) {
        return false;
    }

    if (sub_4B6D80() != obj) {
        return false;
    }

    if (combat_fast_turn_based) {
        return true;
    }

    if (sub_40DA20(obj)) {
        return false;
    }

    if (combat_critter_is_combat_mode_active(obj)) {
        return false;
    }

    if (sub_44E830(obj, 18, 0)) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) & ONF_BACKING_OFF) != 0) {
        return false;
    }

    return true;
}

// 0x4B80D0
void sub_4B80D0()
{
    // TODO: Incomplete.
}

// 0x4B80E0
void sub_4B80E0(int64_t obj)
{
    // 0x5B5818
    static unsigned int dword_5B5818[7] = {
        OCF2_REACTION_0,
        OCF2_REACTION_1,
        OCF2_REACTION_2,
        OCF2_REACTION_3,
        OCF2_REACTION_4,
        OCF2_REACTION_5,
        OCF2_REACTION_6,
    };

    int64_t pc_obj;
    int reaction_level;
    int reaction_type;
    unsigned int flags;
    AnimFxNode node;

    if (dword_5FC22C) {
        pc_obj = player_get_pc_obj();
        reaction_level = sub_4C0CC0(obj, pc_obj);
        reaction_type = reaction_translate(reaction_level);

        flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2);
        flags &= ~(OCF2_REACTION_0 | OCF2_REACTION_1 | OCF2_REACTION_2 | OCF2_REACTION_3 | OCF2_REACTION_4 | OCF2_REACTION_5 | OCF2_REACTION_6);
        flags |= dword_5B5818[reaction_type];
        obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS2, flags);

        if (sub_45D8D0(obj) || sub_4B7DC0(obj)) {
            animfx_remove(&stru_5FC1F8, obj, 0, -1);
        } else {
            sub_4CCD20(&stru_5FC1F8, &node, obj, -1, 0);
            if (!sub_4CCDD0(&node)) {
                animfx_add(&node);
            }
        }
    }
}

// 0x4B81B0
bool combat_set_blinded(int64_t obj)
{
    unsigned int flags;
    int color;
    MesFileEntry mes_file_entry;

    flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS);
    if ((flags & OCF_BLINDED) != 0) {
        return false;
    }

    flags |= OCF_BLINDED;
    obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS, flags);

    color = obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC ? 1 : 0;

    mes_file_entry.num = 4; // "Blinded"
    mes_get_msg(combat_mes_file, &mes_file_entry);
    sub_4D5450(obj, color, mes_file_entry.str);

    return true;
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
void sub_4B83E0(int64_t obj, int64_t a2)
{
    if (obj != OBJ_HANDLE_NULL
        && ((tig_net_flags & TIG_NET_CONNECTED) == 0
            || (tig_net_flags & TIG_NET_HOST) != 0)
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        && combat_auto_switch_weapons_get(obj)) {
        sub_465170(obj, 1004, a2);
    }
}
