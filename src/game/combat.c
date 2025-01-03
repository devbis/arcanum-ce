#include "game/combat.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include "game/ai.h"
#include "game/anim.h"
#include "game/animfx.h"
#include "game/ci.h"
#include "game/critter.h"
#include "game/effect.h"
#include "game/fate.h"
#include "game/gamelib.h"
#include "game/gsound.h"
#include "game/item.h"
#include "game/logbook.h"
#include "game/magictech.h"
#include "game/map.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/mt_item.h"
#include "game/multiplayer.h"
#include "game/obj_private.h"
#include "game/obj.h"
#include "game/object_node.h"
#include "game/object.h"
#include "game/player.h"
#include "game/random.h"
#include "game/reaction.h"
#include "game/script.h"
#include "game/skill.h"
#include "game/stat.h"
#include "game/tf.h"
#include "game/trap.h"

static void turn_based_changed();
static void fast_turn_based_changed();
static void sub_4B24F0(CombatContext* combat, int64_t loc, int a3, int a4, tig_art_id_t missile_art_id);
static void sub_4B2690(int64_t proj_obj, int64_t a2, int64_t a3, CombatContext* combat, bool a5);
static int sub_4B2810(int64_t obj);
static void sub_4B2F60(CombatContext* combat);
static void sub_4B3770(CombatContext* combat);
static void sub_4B39B0(CombatContext* combat);
static void combat_critter_toggle_combat_mode(int64_t obj);
static int64_t sub_4B54B0(int64_t obj, int a2);
static bool sub_4B5520(CombatContext* combat);
static void sub_4B5580(CombatContext* combat);
static void sub_4B5E90(int64_t loc);
static void sub_4B5F40(CombatContext* combat);
static void sub_4B6410(CombatContext* combat);
static int sub_4B65A0();
static bool sub_4B65D0(int64_t weapon_obj, int64_t critter_obj, int a3, bool a4);
static void sub_4B6680(CombatContext* combat);
static void sub_4B6860(CombatContext* combat);
static int sub_4B6930(CombatContext* combat);
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

// 0x5B5790
static struct {
    const char* name;
    bool(*func)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
} stru_5B5790[] = {
    { "Multiplayer", sub_4A6190 },
};

// 0x5B5798
static int dword_5B5798[] = {
    0,
    -50,
    -30,
    -30,
};

// 0x5B57A8
static int dword_5B57A8[TRAINING_COUNT] = {
    0,
    10,
    50,
    100,
};

// 0x5B57B8
static int dword_5B57B8 = 15;

// 0x5B57BC
static int dword_5B57BC[14] = {
    10,
    10,
    5,
    0,
    0,
    10,
    0,
    0,
    0,
    0,
    0,
    10,
    0,
    0,
};

// 0x5B57FC
static int dword_5B57FC[2] = {
    0,
    10,
};

// 0x5B5804
static int dword_5B5804[5] = {
    RESISTANCE_TYPE_NORMAL,
    RESISTANCE_TYPE_POISON,
    RESISTANCE_TYPE_ELECTRICAL,
    RESISTANCE_TYPE_FIRE,
    RESISTANCE_TYPE_NORMAL,
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

// NOTE: It's `bool`, but needs to be 4 byte integer because of saving/reading
// compatibility.
//
// 0x5FC224
static int combat_turn_based;

// 0x5FC228
static bool combat_fast_turn_based;

// NOTE: It's `bool`, but needs to be 4 byte integer because of saving/reading
// compatibility.
//
// 0x5FC22C
static int dword_5FC22C;

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

// 0x5FC258
static int64_t qword_5FC258;

// 0x5FC260
static int dword_5FC260;

// 0x5FC264
static bool dword_5FC264;

// 0x5FC268
static bool in_combat_reset;

// 0x5FC26C
static bool dword_5FC26C;

// 0x5FC270
static int64_t qword_5FC270;

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
        oid.type = OID_TYPE_NULL;
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

    combat->attacker_obj = attacker_obj;
    if (attacker_obj != OBJ_HANDLE_NULL) {
        type = obj_field_int32_get(attacker_obj, OBJ_F_TYPE);
        if (obj_type_is_critter(type)) {
            combat->weapon_obj = sub_4B23B0(attacker_obj);
        } else {
            combat->weapon_obj = OBJ_HANDLE_NULL;
        }
    } else {
        combat->weapon_obj = OBJ_HANDLE_NULL;
    }

    combat->skill = item_weapon_skill(combat->weapon_obj);

    if (attacker_obj != OBJ_HANDLE_NULL
        && obj_type_is_critter(type)
        && target_obj != OBJ_HANDLE_NULL
        && obj_type_is_critter(obj_field_int32_get(target_obj, OBJ_F_TYPE))
        && combat->skill == BASIC_SKILL_MELEE
        && critter_can_backstab(attacker_obj, target_obj)) {
        combat->flags |= 0x4000;
        if (sub_4AF260(target_obj, attacker_obj) != 0
            && sub_4AF470(target_obj, attacker_obj, 0) != 0) {
            combat->flags |= 0x8000;
        }
    }

    combat->target_obj = target_obj;
    combat->field_28 = target_obj;

    if (target_obj != OBJ_HANDLE_NULL) {
        combat->target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    }

    combat->field_30 = attacker_obj;
    combat->hit_loc = 0;
    combat->dam_flags = 0;
    combat->field_5C = 0;
    combat->dam[DAMAGE_TYPE_NORMAL] = 0;
    combat->dam[DAMAGE_TYPE_POISON] = 0;
    combat->dam[DAMAGE_TYPE_ELECTRICAL] = 0;
    combat->dam[DAMAGE_TYPE_FIRE] = 0;
    combat->dam[DAMAGE_TYPE_FATIGUE] = 0;

    if (combat->skill == BASIC_SKILL_THROWING
        || item_weapon_range(combat->weapon_obj, combat->attacker_obj) > 1) {
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
void sub_4B24F0(CombatContext* combat, int64_t loc, int a3, int a4, tig_art_id_t missile_art_id)
{
    int64_t missile_obj;
    int hit_loc;
    unsigned int weapon_flags;
    unsigned int critter_flags2;

    if (mp_object_create(5028, loc, &missile_obj)) {
        sub_4EFDD0(missile_obj, OBJ_F_PROJECTILE_FLAGS_COMBAT_DAMAGE, combat->dam_flags);
        if ((combat->flags & 0x100) != 0) {
            hit_loc = combat->dam[DAMAGE_TYPE_NORMAL];
        } else {
            hit_loc = combat->hit_loc;
        }
        sub_4EFDD0(missile_obj, OBJ_F_PROJECTILE_HIT_LOC, hit_loc);

        mp_obj_field_obj_set(missile_obj, OBJ_F_PROJECTILE_PARENT_WEAPON, combat->weapon_obj);

        if (combat->weapon_obj != OBJ_HANDLE_NULL
            && obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
            weapon_flags = obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_FLAGS);
            if ((weapon_flags & OWF_TRANS_PROJECTILE) != 0) {
                sub_4EFDD0(missile_obj, OBJ_F_BLIT_FLAGS, TIG_ART_BLT_BLEND_ADD);
            }
            if ((weapon_flags & OWF_BOOMERANGS) != 0) {
                critter_flags2 = obj_field_int32_get(combat->attacker_obj, OBJ_F_CRITTER_FLAGS2);
                critter_flags2 |= OCF2_USING_BOOMERANG;
                sub_4EFDD0(combat->attacker_obj, OBJ_F_CRITTER_FLAGS2, critter_flags2);

                combat->flags |= 0x1000;
            }
        }

        sub_4EFDD0(missile_obj, OBJ_F_PROJECTILE_FLAGS_COMBAT, combat->flags);
        sub_435870(combat->attacker_obj,
            missile_obj,
            missile_art_id,
            a3,
            a4,
            combat->target_obj,
            combat->target_loc,
            combat->weapon_obj);
    }
}

// 0x4B2650
void sub_4B2650(int64_t a1, int64_t a2, CombatContext* combat)
{
    sub_4B2690(a1, a2, combat != NULL ? combat->field_28 : OBJ_HANDLE_NULL, combat, false);
}

// 0x4B2690
void sub_4B2690(int64_t proj_obj, int64_t a2, int64_t a3, CombatContext* combat, bool a5)
{
    unsigned int proj_flags;
    int64_t weapon_obj;
    int64_t loc;
    int v1;
    unsigned int critter_flags2;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0) {
        return;
    }

    proj_flags = obj_field_int32_get(proj_obj, OBJ_F_PROJECTILE_FLAGS_COMBAT);
    if ((proj_flags & 0x40) != 0) {
        weapon_obj = obj_field_handle_get(proj_obj, OBJ_F_PROJECTILE_PARENT_WEAPON);
        loc = obj_field_int64_get(proj_obj, OBJ_F_LOCATION);
        sub_4EDF20(weapon_obj, loc, 0, 0, false);
        sub_4EFEE0(weapon_obj, OF_OFF);
        sub_43CCA0(proj_obj);
    } else if ((proj_flags & 0x1000) != 0) {
        if (a5 && (proj_flags && 0x2000) == 0) {
            proj_flags |= 0x2000;
            sub_4EFDD0(proj_obj, OBJ_F_PROJECTILE_FLAGS_COMBAT, proj_flags);
            if (!sub_435A00(proj_obj, obj_field_int64_get(a2, OBJ_F_LOCATION), a3)) {
                sub_4B2690(proj_obj, a2, a3, combat, true);
                return;
            }
        } else {
            critter_flags2 = obj_field_int32_get(a2, OBJ_F_CRITTER_FLAGS2);
            critter_flags2 &= ~OCF2_USING_BOOMERANG;
            sub_4EFDD0(a2, OBJ_F_CRITTER_FLAGS2, critter_flags2);

            sub_43CCA0(proj_obj);
            sub_4A9AD0(a2, a3);
        }
    } else {
        sub_43CCA0(proj_obj);
    }

    if (combat != NULL
        && combat->target_obj != OBJ_HANDLE_NULL) {
        v1 = sub_4B2810(combat->weapon_obj);
        if ((combat->flags & 0x800) != 0) {
            sub_4A9650(combat->attacker_obj, combat->target_obj, v1, 0x01);
        } else {
            sub_4A9650(combat->attacker_obj, combat->target_obj, v1, 0);
        }
    }
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
bool sub_4B2870(int64_t attacker_obj, int64_t target_obj, int64_t target_loc, int64_t proj_obj, int range, int64_t cur_loc, int64_t a7)
{
    unsigned int proj_flags;
    unsigned int proj_dam_flags;
    int64_t weapon_obj;
    int64_t proj_loc;
    int hit_loc;
    int weapon_obj_type;
    int64_t attacker_loc;
    int64_t attacker_to_target_dist;
    int64_t proj_to_target_dist;
    int scale;
    int64_t block_obj = OBJ_HANDLE_NULL;
    int block_obj_type;
    int dam = 0;

    proj_flags = obj_field_int32_get(proj_obj, OBJ_F_PROJECTILE_FLAGS_COMBAT);
    proj_dam_flags = obj_field_int32_get(proj_obj, OBJ_F_PROJECTILE_FLAGS_COMBAT_DAMAGE);
    weapon_obj = obj_field_handle_get(proj_obj, OBJ_F_PROJECTILE_PARENT_WEAPON);
    proj_loc = obj_field_int64_get(proj_obj, OBJ_F_LOCATION);
    hit_loc = obj_field_int32_get(proj_obj, OBJ_F_PROJECTILE_HIT_LOC);

    if (weapon_obj != OBJ_HANDLE_NULL) {
        weapon_obj_type = obj_field_int32_get(weapon_obj, OBJ_F_TYPE);
    } else {
        weapon_obj_type = -1;
    }

    if (weapon_obj_type != 5) {
        attacker_loc = obj_field_int64_get(attacker_obj, OBJ_F_LOCATION);
        attacker_to_target_dist = location_dist(attacker_loc, target_loc);
        proj_to_target_dist = location_dist(proj_loc, target_loc);
        if (attacker_to_target_dist > 0
            && proj_to_target_dist > 0
            && proj_to_target_dist < attacker_to_target_dist) {
            if (attacker_to_target_dist - proj_to_target_dist * 2 > 0
                && attacker_to_target_dist - proj_to_target_dist != 0) {
                scale = (int)(200 - 150 * (attacker_to_target_dist - proj_to_target_dist) / attacker_to_target_dist);
            } else {
                range = (int)(200 + 150 * (attacker_to_target_dist - proj_to_target_dist) / attacker_to_target_dist);
            }
        } else {
            scale = 50;
        }

        tig_debug_printf("Scaling to %d\n", scale);
        object_set_blit_scale(proj_obj, scale);
    }

    if (proj_loc == target_loc) {
        if (target_obj == OBJ_HANDLE_NULL
            || (proj_flags & 0x2000) != 0) {
            sub_4B2690(proj_obj, attacker_obj, a7, NULL, true);

            if (weapon_obj != OBJ_HANDLE_NULL) {
                sub_441980(attacker_obj, weapon_obj, target_obj, SAP_HIT, 0);

                if ((proj_flags & 0x04) != 0
                    && weapon_obj_type == OBJ_TYPE_WEAPON) {
                    sub_441980(attacker_obj, weapon_obj, target_obj, SAP_CRITICAL_HIT, 0);
                }

                sub_4CBDB0(attacker_obj, weapon_obj, target_loc);
            }

            return false;
        }

        unsigned int new_proj_flags = proj_flags;
        new_proj_flags &= ~0x02;
        new_proj_flags |= 0x800 | 0x20;
        obj_field_int32_set(proj_obj, OBJ_F_PROJECTILE_FLAGS_COMBAT, new_proj_flags);
    }

    if ((proj_flags & 0x2000) != 0) {
        return false;
    }

    if ((proj_flags & 0x02) != 0) {
        if (target_obj != OBJ_HANDLE_NULL
            && proj_loc == obj_field_int64_get(target_obj, OBJ_F_LOCATION)) {
            block_obj = target_obj;

            if ((proj_flags & 0x100) != 0) {
                dam = hit_loc;
                hit_loc = 0;
            }
        }
    } else {
        if ((proj_flags & 0x20) != 0) {
            sub_43FDC0(proj_obj,
                proj_obj,
                location_rot(proj_loc, cur_loc),
                0x08 | 0x04 | 0x01,
                &block_obj,
                &block_obj_type,
                NULL);
        }

        if (block_obj == OBJ_HANDLE_NULL) {
            ObjectList objects;
            ObjectNode* node;

            sub_4407C0(proj_loc, OBJ_TM_CRITTER, &objects);
            node = objects.head;
            while (node != NULL) {
                if (!critter_is_dead(node->obj)
                    && node->obj != target_obj
                    && node->obj != attacker_obj
                    && random_between(1, 3) == 1) {
                    block_obj = node->obj;
                    break;
                }
                node = node->next;
            }
            object_list_destroy(&objects);
        }

        if (block_obj != OBJ_HANDLE_NULL) {
            proj_dam_flags = 0;

            proj_flags &= ~0x04;
            proj_flags |= 0x800 | 0x02;

            dam = (proj_flags & 0x100) != 0 ? hit_loc : 0;
            hit_loc = 0;
        }
    }

    if (block_obj != OBJ_HANDLE_NULL) {
        CombatContext combat;

        sub_4B2210(attacker_obj, block_obj, &combat);

        if (target_obj != OBJ_HANDLE_NULL) {
            combat.field_28 = target_obj;
        }

        combat.flags = proj_flags | 0x40000 | 0x200;
        combat.dam_flags = proj_dam_flags;
        combat.dam[DAMAGE_TYPE_NORMAL] = dam;
        combat.hit_loc = hit_loc;
        combat.weapon_obj = weapon_obj;
        combat.skill = item_weapon_skill(weapon_obj);
        combat.flags &= ~0xC000;
        sub_4B2690(proj_obj, attacker_obj, target_obj, &combat, 1);
        sub_4B2F60(&combat);
        sub_4B6B90(&combat);
        sub_4CBDB0(attacker_obj,
            weapon_obj,
            obj_field_int64_get(block_obj, OBJ_F_LOCATION));
        return sub_4B6930(&combat);
    }

    int weapon_range;
    if ((proj_flags & 0x100) != 0) {
        weapon_range = 20;
    } else if ((proj_flags & 0x40) != 0) {
        weapon_range = item_throwing_distance(weapon_obj, attacker_obj);
    } else {
        weapon_range = item_weapon_range(weapon_obj, attacker_obj);
    }

    if (range > weapon_range) {
        if (proj_obj != OBJ_HANDLE_NULL) {
            proj_loc = obj_field_int64_get(proj_obj, OBJ_F_LOCATION);
        }

        sub_4B2690(proj_obj, attacker_obj, target_obj, NULL, true);

        if (weapon_obj != OBJ_HANDLE_NULL) {
            sub_441980(attacker_obj, weapon_obj, target_obj, SAP_MISS, 0);

            if ((proj_flags & 0x04) != 0
                && weapon_obj_type == OBJ_TYPE_WEAPON) {
                sub_441980(attacker_obj, weapon_obj, target_obj, SAP_CRITICAL_MISS, 0);
            }

            sub_4CBDB0(attacker_obj, weapon_obj, proj_loc);
        }
    }

    return false;
}

// 0x4B2F60
void sub_4B2F60(CombatContext* combat)
{
    int sound_id;

    if ((combat->flags & 0x2) != 0) {
        if (combat->weapon_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->attacker_obj, combat->weapon_obj, combat->target_obj, SAP_HIT, 0);

            if ((combat->flags & 0x4) != 0
                && obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
                sub_441980(combat->attacker_obj, combat->weapon_obj, combat->target_obj, SAP_CRITICAL_HIT, 0);
            }
        }

        if (combat->attacker_obj != OBJ_HANDLE_NULL
            && combat->target_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->target_obj, combat->attacker_obj, OBJ_HANDLE_NULL, SAP_CRITTER_HITS, 0);
        }

        sub_4B6680(combat);
        sub_4B4390(combat);

        if ((combat->flags & 0x4) != 0) {
            sound_id = sub_4F0ED0(combat->target_obj, 0);
            sub_41B930(sound_id, 1, combat->target_obj);

            sound_id = sub_4F0BF0(combat->weapon_obj, combat->attacker_obj, combat->target_obj, 6);
            sub_41B930(sound_id, 1, combat->attacker_obj);
        } else {
            sound_id = sub_4F0BF0(combat->weapon_obj, combat->attacker_obj, combat->target_obj, 4);
            sub_41B930(sound_id, 1, combat->attacker_obj);
        }
    } else {
        sound_id = sub_4F0BF0(combat->weapon_obj, combat->attacker_obj, combat->target_obj, 5);
        sub_41B930(sound_id, 1, combat->attacker_obj);

        if (combat->target_obj != OBJ_HANDLE_NULL
            && obj_field_int32_get(combat->target_obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
            sub_4A84F0(combat->target_obj);
        }

        if (combat->weapon_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->attacker_obj, combat->weapon_obj, combat->target_obj, SAP_MISS, 0);

            if ((combat->flags & 0x04) != 0
                && obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
                sub_441980(combat->attacker_obj, combat->weapon_obj, combat->target_obj, SAP_CRITICAL_MISS, 0);
            }
        }
    }
}

// 0x4B3170
int sub_4B3170(CombatContext* combat)
{
    bool was_in_bed = false;
    int sound_id;

    if (combat->target_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(combat->target_obj, OBJ_F_TYPE) == OBJ_TYPE_SCENERY) {
        int64_t critter_obj = obj_field_handle_get(combat->target_obj, OBJ_F_SCENERY_WHOS_IN_ME);
        if (critter_obj != OBJ_HANDLE_NULL) {
            critter_leave_bed(critter_obj, combat->target_obj);

            combat->target_obj = critter_obj;
            combat->field_28 = critter_obj;

            was_in_bed = true;
        }
    }

    if (combat->hit_loc != 0) {
        combat->flags |= 0x01;
    } else {
        combat->hit_loc = sub_4B65A0();
    }

    if (!sub_4B65D0(combat->weapon_obj, combat->attacker_obj, 1, 0)) {
        combat->flags |= 0x400;

        sound_id = sub_4F0BF0(combat->weapon_obj, combat->attacker_obj, combat->target_obj, 3);
        sub_41B930(sound_id, 1, combat->attacker_obj);

        if (obj_field_int32_get(combat->attacker_obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
            sub_4ADFF0(combat->attacker_obj);
        } else {
            sub_4B83E0(combat->attacker_obj, combat->target_obj);
        }

        return 0;
    }

    sound_id = sub_4F0BF0(combat->weapon_obj, combat->attacker_obj, combat->target_obj, 2);
    sub_41B930(sound_id, 1, combat->attacker_obj);

    bool is_melee = true;
    if ((combat->flags & 0x200) != 0
        && (combat->skill == SKILL_THROWING
            || obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_MISSILE_AID) != -1)) {
        is_melee = false;
    }

    if (combat->target_obj != combat->field_28) {
        combat->flags |= 0x800;
    }

    if (is_melee && combat->target_obj != OBJ_HANDLE_NULL) {
        int v1 = sub_4B2810(combat->weapon_obj);
        if ((combat->flags & 0x800) != 0) {
            sub_4A9650(combat->attacker_obj, combat->target_obj, v1, 1);
        } else {
            sub_4A9650(combat->attacker_obj, combat->target_obj, v1, 0);
        }
    }

    int v2 = 0;
    if (combat->weapon_obj != OBJ_HANDLE_NULL && combat->skill != SKILL_THROWING) {
        v2 = sub_461700(combat->weapon_obj, combat->attacker_obj);
    }

    if (random_between(1, 100) <= v2) {
        combat->flags |= 0x04;
    } else if ((combat->flags & 0x100) != 0) {
        combat->flags |= 0x02;
    } else {
        if ((combat->flags & 0x200) != 0) {
            int64_t blocking_obj;

            sub_4ADE00(combat->attacker_obj, combat->target_loc, &blocking_obj);
            if (blocking_obj != OBJ_HANDLE_NULL) {
                combat->target_obj = blocking_obj;
            }
        }

        int v3 = 0;
        if (combat->weapon_obj != OBJ_HANDLE_NULL && combat->skill != SKILL_THROWING) {
            v3 = sub_461620(combat->weapon_obj, combat->attacker_obj, combat->target_obj);
        }

        bool cont = true;
        if (random_between(1, 100) <= v3) {
            if (combat->skill == SKILL_MELEE
                && combat->weapon_obj != OBJ_HANDLE_NULL
                && obj_field_int32_get(combat->weapon_obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) > 0) {
                combat->flags |= 0x20000;
            } else {
                cont = false;
            }
        }

        if (cont) {
            if (combat->target_obj != OBJ_HANDLE_NULL
                && !obj_type_is_critter(obj_field_int32_get(combat->target_obj, OBJ_F_TYPE))) {
                combat->flags |= 0x02;
                cont = false;
            }
        }

        if (cont) {
            SkillInvocation skill_invocation;

            skill_invocation_init(&skill_invocation);
            sub_4440E0(combat->attacker_obj, &(skill_invocation.source));
            sub_4440E0(combat->target_obj, &(skill_invocation.target));
            skill_invocation.target_loc = combat->target_loc;
            sub_4440E0(combat->weapon_obj, &(skill_invocation.item));
            skill_invocation.hit_loc = combat->hit_loc;
            skill_invocation.skill = combat->skill;

            if ((combat->flags & 0x01) != 0) {
                skill_invocation.flags |= 0x08;
            }

            if ((combat->flags & 0x8000) != 0) {
                skill_invocation.flags |= 0x8000;
            }

            if ((combat->flags & 0x20000) != 0) {
                skill_invocation.flags |= 0x10000;
            }

            skill_invocation.modifier = 0;

            if (was_in_bed) {
                skill_invocation.modifier -= 30;
            }

            if (!skill_invocation_run(&skill_invocation)) {
                return 0;
            }

            if ((skill_invocation.flags & SKILL_INVOCATION_SUCCESS) != 0) {
                combat->flags |= 0x02;
            }

            if ((skill_invocation.flags & SKILL_INVOCATION_CRITICAL) != 0) {
                combat->flags |= 0x04;
            }

            if (combat->target_obj != OBJ_HANDLE_NULL
                && (combat->flags & 0x02) != 0) {
                skill_invocation_init(&skill_invocation);
                sub_4440E0(combat->target_obj, &(skill_invocation.source));
                skill_invocation.skill = SKILL_DODGE;
                if (!skill_invocation_run(&skill_invocation)) {
                    return 0;
                }

                if ((skill_invocation.flags & 0x01) != 0) {
                    MesFileEntry mes_file_entry;

                    mes_file_entry.num = 11; // "Dodge!"
                    mes_get_msg(combat_mes_file, &mes_file_entry);
                    tf_add(combat->target_obj, 0, mes_file_entry.str);

                    combat->flags &= ~0x06;

                    if ((skill_invocation.flags & 0x10) != 0) {
                        int training = basic_skill_get_training(combat->target_obj, BASIC_SKILL_DODGE);
                        if (random_between(1, 100) <= dword_5B57A8[training]) {
                            combat->flags |= 0x04;
                        }
                    }
                }
            }
        }
    }

    if (combat->attacker_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(combat->attacker_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if (fate_resolve(combat->attacker_obj, FATE_CRIT_HIT)) {
            combat->flags |= 0x06;
        }
    } else if (combat->target_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(combat->target_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if (fate_resolve(combat->target_obj, FATE_CRIT_MISS)) {
            combat->flags &= ~0x02;
            combat->flags |= 0x04;
        }
    }

    if ((combat->flags & 0x02) == 0 && (combat->flags & 0x04) != 0) {
        sub_4B6410(combat);
    }

    if ((combat->flags & 0x200) != 0) {
        sub_4B39B0(combat);
    } else {
        sub_4B3770(combat);
    }

    sub_4377C0(combat, combat->attacker_obj, combat->attacker_obj, 1);

    if (is_melee) {
        sub_4B6B90(combat);
        return sub_4B6930(combat);
    }

    return 0;
}

// 0x4B3770
void sub_4B3770(CombatContext* combat)
{
    int sound_id;
    CombatContext body_of_fire;

    if ((combat->flags & 0x2) != 0) {
        if (combat->weapon_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->attacker_obj, combat->weapon_obj, combat->target_obj, SAP_HIT, 0);

            if ((combat->flags & 0x4) != 0) {
                sub_441980(combat->attacker_obj, combat->weapon_obj, combat->target_obj, SAP_CRITICAL_HIT, 0);
            }
        }

        if (combat->attacker_obj != OBJ_HANDLE_NULL
            && combat->target_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->target_obj, combat->attacker_obj, OBJ_HANDLE_NULL, SAP_CRITTER_HITS, 0);
        }

        sub_4B6680(combat);
        sub_4B4390(combat);

        if ((combat->flags & 0x4) != 0) {
            sound_id = sub_4F0ED0(combat->target_obj, 0);
            sub_41B930(sound_id, 1, combat->target_obj);

            sound_id = sub_4F0BF0(combat->weapon_obj, combat->attacker_obj, combat->target_obj, 6);
            sub_41B930(sound_id, 1, combat->attacker_obj);
        } else {
            sound_id = sub_4F0BF0(combat->weapon_obj, combat->attacker_obj, combat->target_obj, 4);
            sub_41B930(sound_id, 1, combat->attacker_obj);
        }

        if ((obj_field_int32_get(combat->attacker_obj, OBJ_F_SPELL_FLAGS) & OSF_BODY_OF_FIRE) == 0
            && (obj_field_int32_get(combat->target_obj, OBJ_F_SPELL_FLAGS) & OSF_BODY_OF_FIRE) != 0) {
            sub_4B2210(combat->target_obj, combat->attacker_obj, &body_of_fire);
            body_of_fire.dam[DAMAGE_TYPE_FIRE] = 5;
            sub_4B4390(&body_of_fire);
        }
    } else {
        sound_id = sub_4F0BF0(combat->weapon_obj, combat->attacker_obj, combat->target_obj, 5);
        sub_41B930(sound_id, 1, combat->attacker_obj);

        if (obj_field_int32_get(combat->target_obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
            sub_4A84F0(combat->target_obj);
        }

        if (combat->weapon_obj != OBJ_HANDLE_NULL) {
            sub_441980(combat->attacker_obj, combat->weapon_obj, combat->target_obj, SAP_MISS, 0);

            if ((combat->flags & 0x4) != 0) {
                sub_441980(combat->attacker_obj, combat->weapon_obj, combat->target_obj, SAP_CRITICAL_MISS, 0);
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
    int64_t max_range;
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

    loc = obj_field_int64_get(combat->attacker_obj, OBJ_F_LOCATION);
    if (combat->skill == BASIC_SKILL_THROWING
        && !is_boomerangs
        && (combat->flags & 0x02) == 0) {
        range = (20 - basic_skill_level(combat->attacker_obj, BASIC_SKILL_THROWING)) / 5 + 1;
        max_range = location_dist(loc, combat->target_loc);
        if (range > max_range) {
            range = (int)max_range;
        }

        sub_4B90D0(combat->target_loc, random_between(0, 7), range, &(combat->target_loc));
        combat->target_obj = OBJ_HANDLE_NULL;
        combat->flags |= 0x20;
    }

    if (sub_4A2BA0() || (tig_net_flags & TIG_NET_HOST) != 0) {
        // FIXME: Unused.
        rotation = tig_art_id_rotation_get(obj_field_int32_get(combat->attacker_obj, OBJ_F_CURRENT_AID));

        num_arrows = 1;
        if (combat->skill == BASIC_SKILL_BOW
            && basic_skill_get_training(combat->attacker_obj, BASIC_SKILL_BOW) >= TRAINING_EXPERT) {
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
    combat.hit_loc = a3;
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

    if (sub_441980(attacker_obj, weapon_obj, target_obj, SAP_THROW, 0)) {
        sub_4B2210(attacker_obj, target_obj, &combat);
        combat.hit_loc = a5;
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
        item_transfer(weapon_obj, attacker_obj);
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
    bool v1 = false;
    bool combat_mode_is_active;
    bool is_pc;
    int obj_type;
    bool is_tb;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    is_pc = player_is_pc_obj(obj);
    combat_mode_is_active = combat_critter_is_combat_mode_active(obj);
    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);

    if (!obj_type_is_critter(obj_type) || !critter_is_dead(obj)) {
        if ((obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
            return;
        }

        if (obj_type_is_critter(obj_type)) {
            unsigned int critter_flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS);
            if ((critter_flags & OCF_PARALYZED) != 0) {
                return;
            }

            if (combat_mode_is_active) {
                if ((critter_flags & OCF_STUNNED) != 0) {
                    return;
                }
            } else {
                if (!sub_45D790(obj)) {
                    return;
                }
            }
        }
    }

    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) != 0 || is_pc) {
            Packet19 pkt;

            pkt.type = 19;
            pkt.oid = sub_407EF0(obj);
            pkt.active = combat_mode_is_active;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }

        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }
    }

    is_tb = combat_is_turn_based();

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);

    if (sub_503E20(art_id) == 0) {
        v1 = true;
        if (!is_pc && !critter_is_dead(obj)) {
            return;
        }
    }

    unsigned int critter_flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS);
    unsigned int critter_flags2 = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2);

    if (combat_mode_is_active) {
        obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS, critter_flags & ~OCF_COMBAT_MODE_ACTIVE);
        ai_target_unlock(obj);

        int v2 = sub_503E20(art_id);
        if (v2 == 20 || v2 == 21) {
            art_id = sub_503E50(art_id, 0);
        }

        art_id = sub_504100(art_id, 0);
        art_id = sub_504180(art_id, 0);

        if (is_pc) {
            if (is_tb) {
                combat_callbacks.field_8();
                combat_turn_based_end();
            }
            gsound_stop_combat_music(obj);
        }

        if ((obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_INVISIBLE) != 0
            && !player_is_pc_obj(obj)) {
            sub_43D0E0(obj, OF_INVISIBLE);
        }
    } else {
        obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS, critter_flags | OCF_COMBAT_MODE_ACTIVE);

        art_id = sub_504100(art_id, 1);

        if (is_pc && is_tb) {
            sub_40FED0();

            if (!combat_turn_based_start()) {
                return;
            }

            combat_callbacks.field_4();

            if (dword_5FC22C) {
                sub_4B6E70(obj);
            }
        }

        gsound_start_combat_music(obj);
    }

    if (tig_art_anim_data(art_id, &art_anim_data) != TIG_OK) {
        tig_debug_printf("Combat: combat_critter_toggle_combat_mode: Failed to grab art: %u!\n", art_id);
        obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS, critter_flags);
        return;
    }

    if (tig_art_id_frame_get(art_id) >= art_anim_data.num_frames) {
        art_id = tig_art_id_frame_set(art_id, 0);
    }

    object_set_current_aid(obj, art_id);
    object_set_current_aid(obj, sub_465020(obj));

    if (combat_mode_is_active) {
        if (is_pc) {
            combat_callbacks.field_0(0);
        }

        if (!v1 && sub_4234F0(obj)) {
            sub_424070(obj, 3, 0, 0);
        }
    } else {
        if (is_pc) {
            combat_callbacks.field_0(1);
            sub_4AA580(obj);
        }

        if (!v1 && !is_tb) {
            sub_435BD0(obj);
        }
    }

    if ((critter_flags2 & OCF2_COMBAT_TOGGLE_FX) != 0) {
        sub_456E60(obj, 223);
    }
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
void sub_4B4390(CombatContext* combat)
{
    int obj_type;
    unsigned int dam_flags;
    int dam;
    MesFileEntry mes_file_entry;
    char str[80];
    unsigned int spell_flags = 0;
    bool weapon_dropped = false;

    if (!sub_4A2BA0()) {
        if (sub_4B5520(combat) || (tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }
    }

    if (combat->target_obj == OBJ_HANDLE_NULL) {
        return;
    }

    obj_type = obj_field_int32_get(combat->target_obj, OBJ_F_TYPE);
    if (obj_type_is_critter(obj_type)) {
        unsigned int critter_flags = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS);
        spell_flags = obj_field_int32_get(combat->target_obj, OBJ_F_SPELL_FLAGS);

        if ((critter_flags & (OCF_UNDEAD | OCF_MECHANICAL)) != 0) {
            combat->dam[DAMAGE_TYPE_POISON] = 0;
        } else if ((spell_flags & OSF_STONED) != 0) {
            combat->dam[DAMAGE_TYPE_NORMAL] = 0;
            combat->dam[DAMAGE_TYPE_POISON] = 0;
            combat->dam[DAMAGE_TYPE_ELECTRICAL] = 0;
            combat->dam[DAMAGE_TYPE_FIRE] = 0;
            combat->dam[DAMAGE_TYPE_FATIGUE] = 0;
        }
    }

    if (!dword_5FC26C) {
        bool def;

        dword_5FC26C = true;
        def = sub_441980(combat->attacker_obj, combat->target_obj, OBJ_HANDLE_NULL, SAP_TAKING_DAMAGE, 0);
        dword_5FC26C = false;

        if (!def) {
            return;
        }
    }

    if ((obj_field_int32_get(combat->target_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_INVULNERABLE)) != 0) {
        return;
    }

    if (obj_type_is_critter(obj_type)
        && (obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS2) & OCF2_NIGH_INVULNERABLE) != 0
        && (combat->weapon_obj == OBJ_HANDLE_NULL
            || (obj_field_int32_get(combat->weapon_obj, OBJ_F_ITEM_FLAGS) & OIF_UBER) == 0)) {
        combat->dam[DAMAGE_TYPE_NORMAL] = 0;
        combat->dam[DAMAGE_TYPE_POISON] = 0;
        combat->dam[DAMAGE_TYPE_ELECTRICAL] = 0;
        combat->dam[DAMAGE_TYPE_FIRE] = 0;
        combat->dam[DAMAGE_TYPE_FATIGUE] = 0;
        return;
    }

    if (obj_type == OBJ_TYPE_WALL) {
        return;
    }

    if ((combat->flags & 0x04) != 0) {
        sub_4B5F40(combat);
    }

    sub_4B6860(combat);

    dam_flags = combat->dam_flags;
    if ((dam_flags & (CDF_FULL | CDF_DEATH)) != 0) {
        dam = object_hp_current(combat->target_obj);
    } else {
        dam = combat->dam[DAMAGE_TYPE_NORMAL]
            + combat->dam[DAMAGE_TYPE_ELECTRICAL]
            + combat->dam[DAMAGE_TYPE_FIRE];
    }

    if (combat->dam[DAMAGE_TYPE_FIRE] > 0) {
        dam_flags |= CDF_DAMAGE_ARMOR;
    }

    if (combat->weapon_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
        && (obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_FLAGS) & OWF_DAMAGE_ARMOR) != 0) {
        dam_flags |= CDF_DAMAGE_ARMOR;
    }

    if (sub_40DA20(combat->attacker_obj)) {
        if ((tig_net_flags & TIG_NET_CONNECTED) == 0
            || (tig_net_flags & TIG_NET_HOST) != 0) {
            combat->game_difficulty = gamelib_get_game_difficulty();
        }

        switch (combat->game_difficulty) {
        case 0:
            dam += dam / 2;
            break;
        case 2:
            dam -= dam / 4;
            break;
        }
    }

    if ((dam_flags & CDF_BONUS_DAM_200) != 0) {
        dam *= 3;
    } else if ((dam_flags & CDF_BONUS_DAM_100) != 0) {
        dam *= 2;
    } else if ((dam_flags & CDF_BONUS_DAM_50) != 0) {
        dam += dam / 2;
    }

    if (obj_type_is_critter(obj_type)) {
        // 0x4B475F
        if ((combat->flags & 0x80000) != 0) {
            combat->flags |= 0x02;
            if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                || (tig_net_flags & TIG_NET_HOST) != 0) {
                sub_4B6930(combat);
            }
        }

        if (critter_is_dead(combat->target_obj)) {
            return;
        }

        if ((combat->flags & 0x04) != 0) {
            int inventory_location;
            int64_t item_obj;

            for (inventory_location = 1000; inventory_location <= 1008; inventory_location++) {
                item_obj = item_wield_get(combat->target_obj, inventory_location);
                if (item_obj != OBJ_HANDLE_NULL
                    && obj_field_int32_get(item_obj, OBJ_F_TYPE) == OBJ_TYPE_ARMOR) {
                    sub_441980(combat->attacker_obj, item_obj, combat->target_obj, SAP_TAKING_DAMAGE, 0);
                }
            }
        }

        int tf_type;
        if (obj_type == OBJ_TYPE_PC) {
            tf_type = TF_TYPE_RED;
        } else if (obj_type == OBJ_TYPE_NPC
            && sub_45DDA0(combat->target_obj) == player_get_pc_obj()) {
            tf_type = TF_TYPE_YELLOW;
        } else {
            tf_type = TF_TYPE_WHITE;
        }

        if ((combat->flags & 0x18) != 0) {
            mes_file_entry.num = 2; // "Critical miss"
            mes_get_msg(combat_mes_file, &mes_file_entry);
            tf_add(combat->attacker_obj, tf_type, mes_file_entry.str);
        } else if ((combat->flags & 0x04) != 0) {
            mes_file_entry.num = 12; // "Critical hit"
            mes_get_msg(combat_mes_file, &mes_file_entry);
            tf_add(combat->target_obj, tf_type, mes_file_entry.str);
        }

        if ((dam_flags & CDF_BONUS_DAM_200) != 0) {
            mes_file_entry.num = 15; // "Damage +200%"
            mes_get_msg(combat_mes_file, &mes_file_entry);
            tf_add(combat->target_obj, tf_type, mes_file_entry.str);
        } else if ((dam_flags & CDF_BONUS_DAM_100) != 0) {
            mes_file_entry.num = 14; // "Damage +100%"
            mes_get_msg(combat_mes_file, &mes_file_entry);
            tf_add(combat->target_obj, tf_type, mes_file_entry.str);
        } else if ((dam_flags & CDF_BONUS_DAM_50) != 0) {
            mes_file_entry.num = 13; // "Damage +50%"
            mes_get_msg(combat_mes_file, &mes_file_entry);
            tf_add(combat->target_obj, tf_type, mes_file_entry.str);
        }

        unsigned int critter_flags = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS);
        if ((dam_flags & CDF_STUN) != 0
            && (critter_flags & OCF_STUNNED) == 0
            && (dam_flags & (CDF_KNOCKDOWN | CDF_KNOCKOUT)) == 0) {
            sub_4CBB80(combat->attacker_obj, combat->target_obj);
            critter_flags |= OCF_STUNNED;

            if (((tig_net_flags & TIG_NET_CONNECTED) == 0
                    || (tig_net_flags & TIG_NET_HOST) != 0)
                && !sub_4357B0(combat->target_obj)) {
                critter_flags &= ~OCF_STUNNED;
            }

            obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);

            mes_file_entry.num = 16; // "Stunned"
            mes_get_msg(combat_mes_file, &mes_file_entry);
            tf_add(combat->target_obj, tf_type, mes_file_entry.str);
        }

        if ((dam_flags & (CDF_KNOCKDOWN | CDF_KNOCKOUT)) != 0) {
            sub_4CBBF0(combat->attacker_obj, combat->target_obj);
            sub_4CBE00(combat->attacker_obj, combat->target_obj);
            sub_435A90(combat->target_obj);

            if ((dam_flags & CDF_KNOCKOUT) != 0) {
                if (!sub_45D800(combat->target_obj)) {
                    int max_fatigue = critter_fatigue_max(combat->target_obj);

                    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                        || (tig_net_flags & TIG_NET_HOST) != 0) {
                        combat->field_64 = random_between(10, 20);
                    }

                    critter_fatigue_damage_set(combat->target_obj, combat->field_64 + max_fatigue);

                    mes_file_entry.num = 17; // "Unconscious"
                    mes_get_msg(combat_mes_file, &mes_file_entry);
                    tf_add(combat->target_obj, tf_type, mes_file_entry.str);
                }
            } else {
                mes_file_entry.num = 18; // "Knocked down"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                tf_add(combat->target_obj, tf_type, mes_file_entry.str);
            }
        }

        if ((dam_flags & CDF_SCAR) != 0) {
            effect_add(combat->target_obj, 50, EFFECT_CAUSE_INJURY);
            logbook_add_injury(combat->target_obj, combat->attacker_obj, LBI_SCARRED);

            mes_file_entry.num = 3; // "Scarred"
            mes_get_msg(combat_mes_file, &mes_file_entry);
            tf_add(combat->target_obj, tf_type, mes_file_entry.str);
        }

        if ((dam_flags & CDF_BLIND) != 0) {
            if ((critter_flags & OCF_BLINDED) == 0) {
                critter_flags |= OCF_BLINDED;
                obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
                logbook_add_injury(combat->target_obj, combat->attacker_obj, LBI_BLINDED);

                mes_file_entry.num = 4; // "Blinded"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                tf_add(combat->target_obj, tf_type, mes_file_entry.str);
            }
        }

        if ((dam_flags & CDF_CRIPPLE_ARM) != 0) {
            if ((critter_flags & OCF_CRIPPLED_ARMS_ONE) == 0) {
                critter_flags |= OCF_CRIPPLED_ARMS_ONE;
                obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
                logbook_add_injury(combat->target_obj, combat->attacker_obj, LBI_CRIPPLED_ARM);

                mes_file_entry.num = 5; // "Crippled arm"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                tf_add(combat->target_obj, tf_type, mes_file_entry.str);
            } else if ((critter_flags & OCF_CRIPPLED_ARMS_BOTH) == 0
                && random_between(1, 100) <= 50) {
                critter_flags |= OCF_CRIPPLED_ARMS_BOTH;
                obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
                logbook_add_injury(combat->target_obj, combat->attacker_obj, LBI_CRIPPLED_ARM);

                mes_file_entry.num = 5; // "Crippled arm"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                tf_add(combat->target_obj, tf_type, mes_file_entry.str);
            }

            int64_t weapon_obj = item_wield_get(combat->target_obj, 1004);
            if (weapon_obj != OBJ_HANDLE_NULL
                && sub_464D20(weapon_obj, 1004, combat->target_obj)) {
                if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                    || (tig_net_flags & TIG_NET_HOST) != 0) {
                    item_drop_nearby(weapon_obj);
                }

                mes_file_entry.num = 19; // "Weapon dropped"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                tf_add(combat->target_obj, tf_type, mes_file_entry.str);

                weapon_dropped = true;
            }

            int64_t item_obj = item_wield_get(combat->target_obj, 1005);
            if (item_obj != OBJ_HANDLE_NULL
                && sub_464D20(item_obj, 1005, combat->target_obj)) {
                if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                    || (tig_net_flags & TIG_NET_HOST) != 0) {
                    item_drop_nearby(weapon_obj);
                }

                mes_file_entry.num = 20; // "Item dropped"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                tf_add(combat->target_obj, tf_type, mes_file_entry.str);
            }
        }

        if ((dam_flags & CDF_CRIPPLE_LEG) != 0) {
            if ((critter_flags & OCF_CRIPPLED_LEGS_BOTH) == 0) {
                critter_flags |= OCF_CRIPPLED_LEGS_BOTH;
                obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
                logbook_add_injury(combat->target_obj, combat->attacker_obj, LBI_CRIPPLED_LEG);

                mes_file_entry.num = 6; // "Crippled leg"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                tf_add(combat->target_obj, tf_type, mes_file_entry.str);
            }
        }

        if ((dam_flags & CDF_DAMAGE_ARMOR) != 0) {
            int64_t armor_obj = sub_4B54B0(combat->target_obj, combat->hit_loc);
            if (armor_obj != OBJ_HANDLE_NULL) {
                object_hp_damage_set(armor_obj, object_hp_damage_get(armor_obj) + 10);

                if (object_hp_current(armor_obj) > 0) {
                    mes_file_entry.num = 7; // "Armor damaged"
                    mes_get_msg(combat_mes_file, &mes_file_entry);
                    tf_add(combat->target_obj, tf_type, mes_file_entry.str);
                } else {
                    sub_43F1C0(armor_obj, combat->attacker_obj);

                    mes_file_entry.num = 24; // "Armor broken"
                    mes_get_msg(combat_mes_file, &mes_file_entry);
                    tf_add(combat->target_obj, tf_type, mes_file_entry.str);
                }
            }
        }

        if ((dam_flags & CDF_DROP_ITEM) != 0) {
            int64_t item_obj = item_wield_get(combat->target_obj, 1000);
            if (item_obj != OBJ_HANDLE_NULL) {
                item_drop_nearby(item_obj);

                mes_file_entry.num = 20; // "Item dropped"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                tf_add(combat->target_obj, tf_type, mes_file_entry.str);
            }
        }

        int64_t weapon_obj = sub_4B23B0(combat->target_obj);
        if (weapon_obj != OBJ_HANDLE_NULL) {
            if ((obj_field_int32_get(weapon_obj, OBJ_F_FLAGS) & OF_INVULNERABLE) != 0) {
                dam_flags &= ~0x1C000;
            }

            if ((dam_flags & CDF_DAMAGE_WEAPON) != 0) {
                object_hp_damage_set(weapon_obj, object_hp_damage_get(weapon_obj) + 10);

                if (object_hp_current(weapon_obj) > 0) {
                    mes_file_entry.num = 8; // "Weapon damaged"
                    mes_get_msg(combat_mes_file, &mes_file_entry);
                    tf_add(combat->target_obj, tf_type, mes_file_entry.str);
                } else {
                    sub_43F1C0(weapon_obj, combat->attacker_obj);

                    mes_file_entry.num = 23; // "Weapon broken"
                    mes_get_msg(combat_mes_file, &mes_file_entry);
                    tf_add(combat->target_obj, tf_type, mes_file_entry.str);

                    weapon_dropped = true;
                }
            }

            if ((dam_flags & 0x10000) != 0) {
                dam_flags |= CDF_DESTROY_WEAPON;
                sub_44C820(combat->target_obj);
            }

            if ((dam_flags & CDF_DESTROY_WEAPON) != 0) {
                sub_43CCA0(weapon_obj);

                mes_file_entry.num = 22; // "Weapon destroyed"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                tf_add(combat->target_obj, tf_type, mes_file_entry.str);

                weapon_dropped = true;
            } else {
                if ((dam_flags & CDF_DESTROY_AMMO) != 0) {
                    sub_4B65D0(weapon_obj, combat->target_obj, 0, true);

                    mes_file_entry.num = 9; // "Ammo lost"
                    mes_get_msg(combat_mes_file, &mes_file_entry);
                    tf_add(combat->target_obj, tf_type, mes_file_entry.str);
                } else if ((dam_flags & CDF_LOST_AMMO) != 0) {
                    sub_4B65D0(weapon_obj, combat->target_obj, 5, false);

                    mes_file_entry.num = 9; // "Ammo lost"
                    mes_get_msg(combat_mes_file, &mes_file_entry);
                    tf_add(combat->target_obj, tf_type, mes_file_entry.str);
                }

                if ((dam_flags & CDF_DROP_WEAPON) != 0) {
                    item_drop_nearby(weapon_obj);

                    mes_file_entry.num = 19; // "Weapon dropped"
                    mes_get_msg(combat_mes_file, &mes_file_entry);
                    tf_add(combat->target_obj, tf_type, mes_file_entry.str);

                    weapon_dropped = true;
                }
            }
        }

        if (dam > 0
            && (obj_field_int32_get(combat->target_obj, OBJ_F_SPELL_FLAGS) & OSF_MIRRORED) != 0) {
            sub_459A20(combat->target_obj);
        }

        int hp_ratio_before;

        if (dam > 0) {
            combat->dam_flags |= 0x200000;
            hp_ratio_before = ai_object_hp_ratio(combat->target_obj);
        }

        mes_file_entry.num = 1; // "HT"
        mes_get_msg(combat_mes_file, &mes_file_entry);
        sprintf(str, "%s %d", mes_file_entry.str, dam);

        combat->field_5C = dam;

        int hp_dam = object_hp_damage_get(combat->target_obj) + dam;
        if (hp_dam < 0) {
            hp_dam = 0;
        }
        object_hp_damage_set(combat->target_obj, hp_dam);

        if (dam > 0) {
            int64_t leader_obj = sub_45DDA0(combat->target_obj);
            if (leader_obj != OBJ_HANDLE_NULL
                && hp_ratio_before >= 20
                && ai_object_hp_ratio(combat->target_obj) < 20) {
                sub_4AEAB0(combat->target_obj, leader_obj);
            }
        }

        if ((critter_flags & OCF_FATIGUE_IMMUNE) == 0) {
            if (combat->dam[DAMAGE_TYPE_FATIGUE] > 0) {
                mes_file_entry.num = 10;
                mes_get_msg(combat_mes_file, &mes_file_entry);

                sprintf(&(str[strlen(str)]),
                    " %d %s",
                    combat->dam[DAMAGE_TYPE_FATIGUE],
                    mes_file_entry.str);

                critter_fatigue_damage_set(combat->target_obj,
                    critter_fatigue_damage_get(combat->target_obj) + combat->dam[DAMAGE_TYPE_FATIGUE]);
            }
        }

        if (tf_level_get() == TF_LEVEL_VERBOSE && str[0] != '\0') {
            tf_add(combat->target_obj, tf_type, str);
        }

        int poison = stat_get_base(combat->target_obj, STAT_POISON_LEVEL) + combat->dam[DAMAGE_TYPE_POISON];
        if (poison < 0) {
            poison = 0;
        }
        stat_set_base(combat->target_obj, STAT_POISON_LEVEL, poison);


        if (combat->dam[DAMAGE_TYPE_POISON] > 0 && tf_level_get() == TF_LEVEL_VERBOSE) {
            mes_file_entry.num = 0;
            mes_get_msg(combat_mes_file, &mes_file_entry);
            sprintf(str, "%s %+d", mes_file_entry.str, combat->dam[DAMAGE_TYPE_POISON]);
            tf_add(combat->target_obj, TF_TYPE_GREEN, str);
        }

        if (combat->field_30 != OBJ_HANDLE_NULL
            && obj_type == OBJ_TYPE_NPC
            && (spell_flags & OSF_STONED) == 0) {
            int remaining_experience = obj_field_int32_get(combat->target_obj, OBJ_F_NPC_EXPERIENCE_POOL);
            if (remaining_experience > 0) {
                int dam_ratio = 100 * dam / object_hp_max(combat->target_obj);
                int awarded_experience = obj_field_int32_get(combat->target_obj, OBJ_F_NPC_EXPERIENCE_WORTH) * dam_ratio / 100;
                if (awarded_experience > remaining_experience) {
                    awarded_experience = remaining_experience;
                }
                obj_field_int32_set(combat->target_obj, OBJ_F_NPC_EXPERIENCE_POOL, remaining_experience - awarded_experience);

                if (obj_field_int32_get(combat->field_30, OBJ_F_TYPE) == OBJ_TYPE_PC) {
                    sub_45F110(combat->field_30, awarded_experience);
                }
            }
        }

        if (critter_is_dead(combat->target_obj)) {
            int v2;

            if ((dam_flags & CDF_DEATH) != 0 || dam <= 20) {
                v2 = 7;
            } else if (combat->hit_loc == 1) {
                v2 = 17;
            } else if (combat->hit_loc == 3) {
                v2 = 19;
            } else {
                v2 = 18;
            }

            sub_45DA20(combat->target_obj, combat->field_30, v2);

            if (obj_type == OBJ_TYPE_NPC
                && sub_45DDA0(combat->target_obj) == player_get_pc_obj()) {
                sub_460780();
            }
        } else if (obj_type == OBJ_TYPE_NPC) {
            sub_4A84F0(combat->target_obj);

            if (sub_45DDA0(combat->target_obj) == player_get_pc_obj()) {
                sub_460780();
            }
        }

        if (combat->attacker_obj != OBJ_HANDLE_NULL) {
            sub_4CBAD0(combat->attacker_obj, combat, combat->target_obj);

            if ((combat->flags & 0x40000) != 0) {
                sub_4CBE70(combat->attacker_obj, combat->weapon_obj, combat->target_obj);
            } else {
                sub_4CBE70(combat->attacker_obj, OBJ_HANDLE_NULL, combat->target_obj);
            }
        }

        sub_4B5580(combat);

        if (weapon_dropped) {
            sub_4B83E0(combat->target_obj, combat->attacker_obj);
        }
    } else {
        // 0x4B462E
        if (obj_type == OBJ_TYPE_PORTAL
            || obj_type == OBJ_TYPE_CONTAINER) {
            int material = obj_field_int32_get(combat->target_obj, OBJ_F_MATERIAL);
            dam -= dword_5B57BC[material];
        }

        if (dam < 0) {
            dam = 0;
        } else if (dam > 0) {
            combat->dam_flags |= 0x200000;
        }

        int hp_dam = object_hp_damage_get(combat->target_obj) + dam;
        if (hp_dam < 0) {
            hp_dam = 0;
        }
        object_hp_damage_set(combat->target_obj, hp_dam);

        if (tf_level_get() == TF_LEVEL_VERBOSE) {
            mes_file_entry.num = 1; // "HT"
            mes_get_msg(combat_mes_file, &mes_file_entry);

            sprintf(str, "%s %d", mes_file_entry.str, dam);
            tf_add(combat->target_obj, TF_TYPE_WHITE, str);
        }

        if (object_hp_current(combat->target_obj) > 0) {
            if (combat->field_30 != OBJ_HANDLE_NULL) {
                sub_4AEE50(combat->field_30, combat->target_obj, 1, 2);
                sub_4B5580(combat);
            }
        } else {
            if (trap_type(combat->target_obj) != 0) {
                int64_t triggerer_obj = (combat->flags & 0x200) == 0
                    ? combat->attacker_obj
                    : OBJ_HANDLE_NULL;
                sub_4BCB00(triggerer_obj, combat->target_obj);
            }

            sub_43F1C0(combat->target_obj, combat->attacker_obj);

            if (combat->field_30 != OBJ_HANDLE_NULL) {
                sub_4AEE50(combat->field_30, combat->target_obj, 0, 2);
            }
        }
    }

    if (!sub_4A2BA0() && (tig_net_flags & TIG_NET_HOST) != 0) {
        Packet20 pkt;

        pkt.type = 20;
        sub_4F0640(combat->attacker_obj, &(pkt.field_70));
        sub_4F0640(combat->weapon_obj, &(pkt.field_88));
        sub_4F0640(combat->target_obj, &(pkt.field_A0));
        sub_4F0640(combat->field_28, &(pkt.field_B8));
        sub_4F0640(combat->field_30, &(pkt.field_D0));
        pkt.combat = *combat;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
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
bool sub_4B5520(CombatContext* combat)
{
    int index;

    for (index = 0; index < sizeof(stru_5B5790) / sizeof(stru_5B5790[0]); index++) {
        if (stru_5B5790[index].func != NULL
            && stru_5B5790[index].func(combat->attacker_obj, combat->target_obj, combat->field_28, combat->field_30)) {
            return true;
        }
    }

    return false;
}

// 0x4B5580
void sub_4B5580(CombatContext* combat)
{
    int dam;
    int complexity;
    char str[80];
    MesFileEntry mes_file_entry;

    if (combat->attacker_obj == OBJ_HANDLE_NULL) {
        return;
    }

    if (combat->target_obj == OBJ_HANDLE_NULL) {
        return;
    }

    if ((combat->flags & 0x10000) == 0) {
        return;
    }

    if ((combat->dam_flags & 0x200000) == 0) {
        return;
    }

    if (combat->skill != SKILL_MELEE) {
        return;
    }

    switch (obj_field_int32_get(combat->target_obj, OBJ_F_MATERIAL)) {
    case 0:
        dam = 4;
        break;
    case 1:
        dam = 3;
        break;
    case 2:
        dam = 2;
        break;
    case 3:
        dam = 1;
        break;
    case 5:
        dam = 5;
        break;
    case 6:
        dam = combat->weapon_obj != OBJ_HANDLE_NULL ? 0 : 5;
        break;
    case 12:
        dam = 5;
        break;
    default:
        dam = 0;
        break;
    }

    if (combat->weapon_obj != OBJ_HANDLE_NULL) {
        complexity = obj_field_int32_get(combat->weapon_obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);
        if (complexity < 0) {
            complexity = -complexity;
        }

        if (complexity > 100) {
            complexity = 100;
        }

        dam += dam * complexity / -100;

        switch (tig_art_item_id_subtype_get(obj_field_int32_get(combat->weapon_obj, OBJ_F_ITEM_USE_AID_FRAGMENT))) {
        case TIG_ART_WEAPON_TYPE_AXE:
            dam -= 2;
            break;
        case TIG_ART_WEAPON_TYPE_MACE:
        case TIG_ART_WEAPON_TYPE_STAFF:
            dam -= 1;
            break;
        }
    } else {
        int hp = object_hp_current(combat->attacker_obj);
        if (dam >= hp) {
            dam = hp - 1;
        }
    }

    str[0] = '\0';

    if (dam > 0) {
        if (combat->weapon_obj != OBJ_HANDLE_NULL) {
            // Apply damage to the weapon.
            object_hp_damage_set(combat->weapon_obj,
                object_hp_damage_get(combat->weapon_obj) + dam);

            mes_file_entry.num = 8; // "Weapon damaged"
            if (object_hp_current(combat->weapon_obj) <= 0) {
                sub_43F1C0(combat->weapon_obj, combat->attacker_obj);
                mes_file_entry.num = 23; // "Weapon broken"
                sub_4B83E0(combat->attacker_obj, combat->target_obj);
            }

            mes_get_msg(combat_mes_file, &mes_file_entry);
            strcpy(str, mes_file_entry.str);
        } else {
            if (tf_level_get() == TF_LEVEL_VERBOSE) {
                mes_file_entry.num = 1; // "HT"
                mes_get_msg(combat_mes_file, &mes_file_entry);
                sprintf(str, "%d %s", dam, mes_file_entry.str);
            }

            // Apply damage to the target.
            object_hp_damage_set(combat->attacker_obj,
                object_hp_damage_get(combat->attacker_obj) + dam);
        }

        if (str[0] != '\0') {
            tf_add(combat->attacker_obj, TF_TYPE_RED, str);
        }
    }
}

// 0x4B5810
void sub_4B5810(CombatContext* combat)
{
    int64_t obj;
    int index;
    int64_t item_obj;

    obj = combat->target_obj;
    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        for (index = 0; index < 9; index++) {
            item_obj = item_wield_get(obj, 1000 + index);
            if (item_obj != OBJ_HANDLE_NULL) {
                combat->target_obj = item_obj;
                sub_4B4390(combat);
            }
        }
    }
    combat->target_obj = obj;
    combat->dam[DAMAGE_TYPE_NORMAL] /= 3;
    sub_4B4390(combat);
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
        sub_4F0640(combat->attacker_obj, &(pkt.field_70));
        sub_4F0640(combat->weapon_obj, &(pkt.field_88));
        sub_4F0640(combat->target_obj, &(pkt.field_A0));
        sub_4F0640(combat->field_28, &(pkt.field_B8));
        sub_4F0640(combat->field_30, &(pkt.field_D0));
        pkt.combat = *combat;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    type = obj_field_int32_get(combat->target_obj, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    if ((combat->dam_flags & CDF_SCAR) != 0
        && (combat->dam_flags & CDF_FULL) == 0) {
        for (index = 0; index < 5; index++) {
            effect_remove_one_caused_by(combat->target_obj, EFFECT_CAUSE_INJURY);
        }
    }

    if (critter_is_dead(combat->target_obj)) {
        if ((combat->dam_flags & CDF_RESURRECT) == 0) {
            return;
        }
    }

    critter_flags = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS);
    if ((critter_flags & OCF_UNREVIVIFIABLE) != 0
        && (combat->dam_flags & 0x400000) != 0) {
        v1 = true;
    }
    if ((critter_flags & OCF_UNRESSURECTABLE) != 0
        && (combat->dam_flags & 0x400000) == 0) {
        v1 = true;
    }

    if (!sub_441980(combat->attacker_obj, combat->target_obj, combat->weapon_obj, SAP_RESURRECT, 0) || v1) {
        sub_45A540(combat->target_obj);
        return;
    }

    art_id = obj_field_int32_get(combat->target_obj, OBJ_F_CURRENT_AID);
    art_id = sub_503E50(art_id, 0);
    art_id = tig_art_id_frame_set(art_id, 0);
    object_set_current_aid(combat->target_obj, art_id);

    v2 = true;

    sub_4B5E90(combat->target_loc);
    sub_4AF170(combat->target_obj);
    sub_43D280(combat->target_obj, OF_FLAT | OF_NO_BLOCK);

    critter_flags = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS);
    critter_flags &= ~OCF_STUNNED;
    obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);

    sub_45EC80(combat->target_obj);

    if (critter_fatigue_damage_get(combat->target_obj) != 10) {
        critter_fatigue_damage_set(combat->target_obj, 10);
    }

    sub_459740(combat->target_obj);

    if (combat->attacker_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(combat->attacker_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        int reaction_level = sub_4C0CC0(combat->target_obj, combat->attacker_obj);
        if (reaction_level > 0 && reaction_level < 70) {
            sub_4C0DE0(combat->target_obj, combat->attacker_obj, 70 - reaction_level);
        }
    }

    if (type == OBJ_TYPE_NPC) {
        int64_t leader_obj = critter_leader_get(combat->target_obj);
        if (leader_obj != OBJ_HANDLE_NULL) {
            if (!critter_follow(combat->target_obj, leader_obj, false)) {
                critter_leader_set(combat->target_obj, OBJ_HANDLE_NULL);
            }
        }
    }

    if ((combat->dam_flags & CDF_FULL) != 0) {
        combat->dam[DAMAGE_TYPE_NORMAL] = object_hp_damage_get(combat->target_obj);
        if (combat->dam[DAMAGE_TYPE_FATIGUE] > 0) {
            combat->dam[DAMAGE_TYPE_FATIGUE] = critter_fatigue_damage_get(combat->target_obj);
        }

        for (index = 0; index < 5; index++) {
            effect_remove_one_caused_by(combat->target_obj, EFFECT_CAUSE_INJURY);
        }

        critter_flags = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS);
        if ((critter_flags & OCF_BLINDED) != 0) {
            critter_flags &= ~OCF_BLINDED;
            obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((critter_flags & OCF_CRIPPLED_ARMS_ONE) != 0) {
            critter_flags &= ~OCF_CRIPPLED_ARMS_ONE;
            obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((critter_flags & OCF_CRIPPLED_ARMS_BOTH) != 0) {
            critter_flags &= ~OCF_CRIPPLED_ARMS_BOTH;
            obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((critter_flags & OCF_CRIPPLED_LEGS_BOTH) != 0) {
            critter_flags &= ~OCF_CRIPPLED_LEGS_BOTH;
            obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
    } else {
        critter_flags = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS);
        if ((combat->dam_flags & CDF_BLIND) != 0
            && (critter_flags & OCF_BLINDED) != 0) {
            critter_flags &= ~OCF_BLINDED;
            obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((combat->dam_flags & CDF_CRIPPLE_ARM) != 0
            && (critter_flags & (OCF_CRIPPLED_ARMS_ONE | OCF_CRIPPLED_ARMS_BOTH)) != 0) {
            critter_flags &= ~(OCF_CRIPPLED_ARMS_ONE | OCF_CRIPPLED_ARMS_BOTH);
            obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
        if ((combat->dam_flags & CDF_CRIPPLE_LEG) != 0
            && (critter_flags & OCF_CRIPPLED_LEGS_BOTH) != 0) {
            critter_flags &= ~OCF_BLINDED;
            obj_field_int32_set(combat->target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);
        }
    }

    if (combat->dam[DAMAGE_TYPE_NORMAL] > 0) {
        int new_dam;
        int cur_dam;

        cur_dam = object_hp_damage_get(combat->target_obj);
        if (cur_dam != 0) {
            new_dam = cur_dam - combat->dam[DAMAGE_TYPE_NORMAL];
            if (new_dam < 0) {
                new_dam = 0;
            }
            object_hp_damage_set(combat->target_obj, new_dam);
        }
    }

    if (combat->dam[DAMAGE_TYPE_FATIGUE] > 0) {
        int new_dam;
        int cur_dam;
        int v3;

        cur_dam = critter_fatigue_damage_get(combat->target_obj);
        if (cur_dam != 0) {
            v3 = critter_fatigue_current(combat->target_obj);
            new_dam = cur_dam - combat->dam[DAMAGE_TYPE_FATIGUE];
            if (new_dam < 0) {
                new_dam = 0;
            }
            critter_fatigue_damage_set(combat->target_obj, new_dam);

            if (v3 <= 0 && critter_fatigue_current(combat->target_obj) > 0) {
                sub_434DE0(combat->target_obj);
            }
        }
    }

    if (combat->dam[DAMAGE_TYPE_POISON] > 0) {
        int new_dam;
        int cur_dam;

        cur_dam = stat_get_base(combat->target_obj, STAT_POISON_LEVEL);
        if (cur_dam != 0) {
            new_dam = cur_dam - combat->dam[DAMAGE_TYPE_POISON];
            if (new_dam < 0) {
                new_dam = 0;
            }
            stat_set_base(combat->target_obj, STAT_POISON_LEVEL, new_dam);
        }
    }

    if (v2 && type != OBJ_TYPE_PC) {
        sub_4AD6E0(combat->target_obj);
    }

    if (type == OBJ_TYPE_NPC) {
        if (sub_45DDA0(combat->target_obj) == player_get_pc_obj()) {
            sub_460780();
        }
    }

    sub_4B80E0(combat->target_obj);
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
int sub_4B5F30(int a1)
{
    return dword_5B5798[a1];
}

// 0x4B5F40
void sub_4B5F40(CombatContext* combat)
{
    int target_obj_type;
    int attacker_obj_type;
    bool npc_attacks_pc;
    int crit_hit_chart;
    int chance;
    int critter_crit_hit_chart;
    unsigned int critter_flags;
    int64_t item_obj;
    int difficulty;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0) {
        return;
    }

    if (combat->target_obj == OBJ_HANDLE_NULL) {
        return;
    }

    if (combat->attacker_obj == OBJ_HANDLE_NULL) {
        return;
    }

    target_obj_type = obj_field_int32_get(combat->target_obj, OBJ_F_TYPE);
    attacker_obj_type = obj_field_int32_get(combat->attacker_obj, OBJ_F_TYPE);

    // Make sure it's a clash between critters.
    if (!obj_type_is_critter(target_obj_type)
        || !obj_type_is_critter(attacker_obj_type)) {
        return;
    }

    npc_attacks_pc = target_obj_type == OBJ_TYPE_PC
        && attacker_obj_type == OBJ_TYPE_NPC;

    critter_crit_hit_chart = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_CRIT_HIT_CHART);

    if (combat->weapon_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
        crit_hit_chart = obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_CRIT_HIT_CHART);
        if ((combat->flags & 0x20000) == 0) {
            chance = sub_461590(combat->weapon_obj,
                combat->attacker_obj,
                obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_EFFECT));
        }
    } else {
        crit_hit_chart = 1;
        chance = 0;
    }

    chance = effect_adjust_crit_hit_effect(combat->attacker_obj, chance);

    if ((combat->flags & 0x1) != 0) {
        chance += 10;
    }

    if (!npc_attacks_pc) {
        if (random_between(1, 100) <= chance + 10) {
            combat->dam_flags |= CDF_BONUS_DAM_200;
        } else if (random_between(1, 100) <= chance + 30) {
            combat->dam_flags |= CDF_BONUS_DAM_100;
        } else if (random_between(1, 100) <= chance + 60) {
            combat->dam_flags |= CDF_BONUS_DAM_50;
        }
    }

    if (critter_crit_hit_chart != 2) {
        critter_flags = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS);
        if ((critter_flags & OCF_UNDEAD) == 0) {
            item_obj = item_wield_get(combat->target_obj, 1000);

            difficulty = chance + 5;

            if (crit_hit_chart == 1 || crit_hit_chart == 3) {
                difficulty += 10;
            }

            if (combat->hit_loc == 1 && item_obj == OBJ_HANDLE_NULL) {
                difficulty += 10;
            }

            if (random_between(1, 100) <= difficulty
                && !sub_45F060(combat->target_obj, STAT_CONSTITUTION, -5)) {
                combat->dam_flags |= CDF_STUN;

                if (target_obj_type != OBJ_TYPE_PC
                    && (critter_flags & OCF_FATIGUE_IMMUNE) == 0
                    && random_between(1, 100) < difficulty
                    && !sub_45F060(combat->target_obj, STAT_CONSTITUTION, -5)) {
                    combat->dam_flags |= CDF_KNOCKOUT;
                }
            }

            // NOTE: Not sure how to reimplement it without goto.
            do {
                if (critter_crit_hit_chart != 4) {
                    if (critter_crit_hit_chart != 5 && combat->hit_loc == 2) {
                        if (random_between(1, 100) <= chance + 1
                            && !sub_45F060(combat->target_obj, STAT_CONSTITUTION, -5)) {
                            combat->dam_flags |= CDF_CRIPPLE_ARM;
                        }
                        break;
                    }

                    if (combat->hit_loc == 3) {
                        if (random_between(1, 100) <= chance + 1
                            && !sub_45F060(combat->target_obj, STAT_CONSTITUTION, -5)) {
                            combat->dam_flags |= CDF_CRIPPLE_LEG;
                        }
                        break;
                    }
                }

                if (combat->hit_loc == 1) {
                    if (crit_hit_chart == 1) {
                        if (item_obj != OBJ_HANDLE_NULL
                            && random_between(1, 100) <= chance + 5) {
                            combat->dam_flags |= CDF_DROP_ITEM;
                            break;
                        }
                    }

                    if (!npc_attacks_pc
                        && random_between(1, 100) <= chance + (item_obj == OBJ_HANDLE_NULL ? 1 : 0)
                        && !sub_45F060(combat->target_obj, STAT_CONSTITUTION, 0)) {
                        combat->dam_flags |= CDF_BLIND;
                        break;
                    }
                }
            } while (0);

            if (target_obj_type == OBJ_TYPE_PC
                && ((combat->flags & 0x8) == 0
                    || sub_4EA4A0(combat->target_obj, 50) <= 0)
                && random_between(1, 100) <= chance + 5) {
                combat->dam_flags |= CDF_SCAR;
            }
        }

        if (critter_crit_hit_chart == 0) {
            difficulty = chance + 5;

            if (crit_hit_chart == 1) {
                difficulty += 5;
            }

            if (combat->hit_loc == 3) {
                difficulty += 10;
            }

            if (random_between(1, 100) <= difficulty) {
                combat->dam_flags |= CDF_KNOCKDOWN;
            }
        }
    }

    if (sub_4B23B0(combat->target_obj) != OBJ_HANDLE_NULL) {
        if (random_between(1, 100) <= chance + 10) {
            combat->dam_flags |= CDF_DAMAGE_WEAPON;
        }

        if (!npc_attacks_pc
            && random_between(1, 100) <= chance + 10) {
            combat->dam_flags |= CDF_DROP_WEAPON;
        }
    }

    if (sub_4B54B0(combat->target_obj, combat->hit_loc) != OBJ_HANDLE_NULL
        && random_between(1, 100) <= chance + 10) {
        combat->dam_flags |= CDF_DAMAGE_ARMOR;
    }

    if (combat->dam_flags == 0) {
        combat->dam_flags = CDF_BONUS_DAM_50;
    }
}

// 0x4B6410
void sub_4B6410(CombatContext* combat)
{
    int crit_miss_chart;
    int chance;

    if (combat->weapon_obj != OBJ_HANDLE_NULL) {
        if (obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
            crit_miss_chart = obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_CRIT_MISS_CHART);
            if ((combat->flags & 0x20000) == 0) {
                chance = sub_461590(combat->weapon_obj,
                    combat->attacker_obj,
                    obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_MAGIC_CRIT_MISS_EFFECT));
            } else {
                chance = 0;
            }
        } else {
            crit_miss_chart = 1;
            chance = 0;
        }
    } else {
        crit_miss_chart = 5;
        chance = 0;
    }

    chance = effect_adjust_crit_fail_effect(combat->attacker_obj, chance);

    combat->flags &= ~0x1;
    combat->flags &= ~0x4;
    combat->flags |= 0x8;
    combat->flags |= 0x2;

    combat->target_obj = combat->attacker_obj;
    combat->target_loc = obj_field_int64_get(combat->target_obj, OBJ_F_LOCATION);

    if (random_between(1, 100) <= 50) {
        combat->flags |= 0x4;
        if (crit_miss_chart != 5 && random_between(1, 100) <= chance + 1) {
            combat->dam_flags |= CDF_DESTROY_WEAPON;
        }

        if ((crit_miss_chart == 6
                || crit_miss_chart == 7
                || crit_miss_chart == 8)
            && random_between(1, 100) <= chance + 1) {
            combat->dam_flags |= 0x10000;
        }

        if (item_weapon_ammo_type(combat->weapon_obj) != 10000) {
            if (random_between(1, 100) <= chance + 1) {
                combat->dam_flags |= CDF_DESTROY_AMMO;
            } else if (random_between(1, 100) <= chance + 11) {
                combat->dam_flags |= CDF_LOST_AMMO;
            }
        }

        sub_4B5F40(combat);
    }
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
bool sub_4B65D0(int64_t weapon_obj, int64_t critter_obj, int a3, bool a4)
{
    int ammo_type;
    int qty;
    int consumption;

    if (obj_field_int32_get(critter_obj, OBJ_F_TYPE) == OBJ_TYPE_NPC
        && !sub_45DDA0(critter_obj)) {
        return true;
    }

    ammo_type = item_weapon_ammo_type(weapon_obj);
    if (ammo_type == 10000) {
        return true;
    }

    qty = item_ammo_quantity_get(critter_obj, ammo_type);

    if (a4) {
        item_ammo_transfer(critter_obj, OBJ_HANDLE_NULL, qty, ammo_type, OBJ_HANDLE_NULL);
        return true;
    }

    consumption = a3 * obj_field_int32_get(weapon_obj, OBJ_F_WEAPON_AMMO_CONSUMPTION);
    if (qty >= consumption) {
        item_ammo_transfer(critter_obj, OBJ_HANDLE_NULL, consumption, ammo_type, OBJ_HANDLE_NULL);
        return true;
    }

    return false;
}

// 0x4B6680
void sub_4B6680(CombatContext* combat)
{
    unsigned int critter_flags;
    unsigned int critter_flags2;
    unsigned int spell_flags;
    int damage_type;
    int min_damage;
    int max_damage;
    int damage;

    if ((combat->flags & 0x100) != 0) {
        return;
    }

    if (obj_type_is_critter(obj_field_int32_get(combat->target_obj, OBJ_F_TYPE))) {
        critter_flags = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS);
        critter_flags2 = obj_field_int32_get(combat->target_obj, OBJ_F_CRITTER_FLAGS2);
        spell_flags = obj_field_int32_get(combat->target_obj, OBJ_F_SPELL_FLAGS);
    }

    for (damage_type = 0; damage_type < 5; damage_type++) {
        item_weapon_damage(combat->weapon_obj,
            combat->attacker_obj,
            damage_type,
            combat->skill,
            (combat->flags & 0x20000) != 0,
            &min_damage,
            &max_damage);

        if (damage_type == 1
            && ((critter_flags & (OCF_UNDEAD | OCF_MECHANICAL)) != 0
                || (spell_flags & OSF_STONED) != 0)) {
            damage = 0;
        } else {
            damage = random_between(min_damage, max_damage);

            switch (damage_type) {
            case DAMAGE_TYPE_NORMAL:
                if ((combat->flags & 0x8000) != 0) {
                    damage += basic_skill_level(combat->attacker_obj, BASIC_SKILL_BACKSTAB) * 5;
                } else if ((combat->flags & 0x4000) != 0) {
                    damage += basic_skill_level(combat->attacker_obj, BASIC_SKILL_BACKSTAB);
                }
                break;
            case DAMAGE_TYPE_FATIGUE:
                if ((critter_flags2 & OCF2_FATIGUE_DRAINING) != 0 && damage > 0) {
                    damage *= 4;
                }
                if ((critter_flags & OCF_FATIGUE_LIMITING) != 0 && damage > 0) {
                    damage /= 4;
                }
                break;
            }
        }

        combat->dam[damage_type] = damage;
    }

    spell_flags = obj_field_int32_get(combat->target_obj, OBJ_F_SPELL_FLAGS);
    if (combat->weapon_obj == OBJ_HANDLE_NULL) {
        if ((spell_flags & OSF_BODY_OF_EARTH) != 0) {
            combat->dam[DAMAGE_TYPE_NORMAL] += 5;
        } else if ((spell_flags & OSF_BODY_OF_FIRE) != 0) {
            combat->dam[DAMAGE_TYPE_FIRE] += 15;
        } else if ((spell_flags & OSF_BODY_OF_WATER) != 0) {
            combat->dam[DAMAGE_TYPE_FATIGUE] += 15;
        } else if ((spell_flags & OSF_HARDENED_HANDS) != 0) {
            combat->dam[DAMAGE_TYPE_NORMAL] += 2;
        }
    }
}

// 0x4B6860
void sub_4B6860(CombatContext* combat)
{
    int idx;
    int resistance;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0) {
        return;
    }

    if (combat->target_obj == OBJ_HANDLE_NULL) {
        return;
    }

    if ((combat->dam_flags & CDF_IGNORE_RESISTANCE) != 0) {
        return;
    }

    if (combat->weapon_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
        && (obj_field_int32_get(combat->weapon_obj, OBJ_F_WEAPON_FLAGS) & OWF_IGNORE_RESISTANCE) != 0) {
        return;
    }

    for (idx = 0; idx < 5; idx++) {
        resistance = sub_43D6D0(combat->target_obj, dword_5B5804[idx], false);
        if (idx == DAMAGE_TYPE_FATIGUE) {
            resistance = 3 * resistance / 4;
        }
        if (resistance > 0) {
            combat->dam[idx] -= resistance * combat->dam[idx] / 100;
        }
    }
}

// 0x4B6930
int sub_4B6930(CombatContext* combat)
{
    int v1 = 0;
    int max_dam = 0;
    int max_dam_index = 0;
    int index;

    if (combat->target_obj != OBJ_HANDLE_NULL
        && obj_type_is_critter(obj_field_int32_get(combat->target_obj, OBJ_F_TYPE))
        && (combat->flags & 0x02) != 0) {
        if ((combat->flags & 0x04) != 0) {
            v1 = 6;
        } else {
            for (index = 0; index < 4; index++) {
                if (combat->dam[index] > max_dam) {
                    max_dam = combat->dam[index];
                    max_dam_index = index;
                }
            }

            v1 = max_dam_index + 1;

            // FIXME: Unreachable.
            if (max_dam_index == -1) {
                return v1;
            }
        }

        if (!sub_4377C0(combat, combat->weapon_obj, combat->target_obj, 3)) {
            if (max_dam > 0) {
                if ((combat->flags & 0x80) == 0) {
                    sub_433020(combat->target_obj, v1, 0, combat);
                }
            } else {
                magictech_anim_play_hit_fx(combat->target_obj, combat);
            }
        }
    }

    return v1;
}

// 0x4B6A00
int sub_4B6A00(int64_t loc1, int64_t loc2)
{
    int64_t x1;
    int64_t y1;
    int64_t x2;
    int64_t y2;
    double v1;
    int extended_rotation;

    if (loc1 == loc2) {
        return 0;
    }

    location_xy(loc1, &x1, &y1);
    location_xy(loc2, &x2, &y2);

    v1 = atan2((double)(y2 - y1), (double)(x2 - x1));
    if (v1 < 0.0) {
        v1 += M_PI * 2.0;
    }

    extended_rotation = (int)(v1 * 32.0 / (M_PI * 2.0));
    if (extended_rotation < 0 || extended_rotation >= 32) {
        extended_rotation = 0;
    }

    extended_rotation += 8;
    if (extended_rotation >= 32) {
        extended_rotation -= 32;
    }

    return extended_rotation;
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

    if (obj_field_int32_get(combat->attacker_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (combat->target_obj == OBJ_HANDLE_NULL) {
        return;
    }

    if (combat->target_obj == combat->attacker_obj) {
        return;
    }

    if ((combat->flags & 0x2) == 0) {
        return;
    }

    if ((combat->target_obj == combat->field_28)) {
        if ((combat->flags & 0x4) != 0) {
            sub_4AE9E0(combat->attacker_obj, false);
        }
    } else {
        if ((combat->flags & 0x08) != 0) {
            sub_4AE9E0(combat->attacker_obj, true);
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
    char* name = NULL;

    if (obj == OBJ_HANDLE_NULL) {
        obj = qword_5FC270;
    }

    if (obj != OBJ_HANDLE_NULL) {
        if (sub_4E5470(obj)) {
            if (sub_40DA20(obj)) {
                obj_field_string_get(obj, OBJ_F_PC_PLAYER_NAME, &name);
            } else {
                obj_field_string_get(obj, OBJ_F_NAME, &name);
            }
        } else {
            qword_5FC270 = OBJ_HANDLE_NULL;
        }
    }

    tig_debug_printf("Combat: TB: DBG: %s: %s, Idx: %d, APs Left: %d\n",
        msg,
        name != NULL ? name : " ",
        dword_5FC250,
        combat_action_points);

    if (name != NULL) {
        FREE(name);
    }
}

// 0x4B6E70
void sub_4B6E70(int64_t obj)
{
    if (!dword_5FC22C) {
        return;
    }

    combat_debug(obj, "Whos Turn Set");

    if (!player_is_pc_obj(qword_5FC248)) {
        sub_424070(qword_5FC248, 3, 0, 1);
    }

    if (dword_5FC240->obj != obj) {
        dword_5FC240 = stru_5FC180.head;
        while (dword_5FC240 != NULL) {
            if (dword_5FC240->obj == obj) {
                break;
            }
            dword_5FC240 = dword_5FC240->next;
        }

        if (dword_5FC240 == NULL) {
            tig_debug_printf("Combat: TB: Note: Couldn't change to 'Who's' Turn, inserting them in list.\n");
            combat_turn_based_add_critter(obj);
            dword_5FC240 = stru_5FC180.head;
            while (dword_5FC240 != NULL) {
                if (dword_5FC240->obj == obj) {
                    break;
                }
                dword_5FC240 = dword_5FC240->next;
            }

            if (dword_5FC240 == NULL) {
                tig_debug_printf("Combat: TB: ERROR: Couldn't change to 'Who's' Turn AFTER inserting them in list...DISABLING TB-COMBAT!\n");
                settings_set_value(&settings, "turn-based", 0);
                return;
            }
        }
    }

    combat_action_points = stat_level(obj, STAT_SPEED);
    if (combat_action_points < 5) {
        combat_action_points = 5;
    }

    tig_debug_printf("Combat: TB: Action Points Available: %d.\n", combat_action_points);

    if (player_is_pc_obj(obj)) {
        combat_callbacks.field_C(combat_action_points);

        if (!sub_4BB900()) {
            sub_4BB8E0();
        }
    } else {
        combat_callbacks.field_C(0);
        sub_4BB910();
        sub_4B7010(obj);

        if (sub_4BB900()) {
            sub_4BB8F0();
        }
    }

    qword_5FC248 = obj;

    if (combat_callbacks.field_10 != NULL) {
        combat_callbacks.field_10(obj);
    }
}

// 0x4B7010
void sub_4B7010(int64_t obj)
{
    if (obj == dword_5FC240->obj
        && !sub_40DA20(obj)
        && !gamelib_in_load()) {
        if (sub_441980(obj, obj, OBJ_HANDLE_NULL, SAP_HEARTBEAT, 0) == 1) {
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
    DateTime datetime;
    TimeEvent timeevent;

    if (!dword_5FC22C) {
        return;
    }

    combat_debug(OBJ_HANDLE_NULL, "TB Anims Complete");

    if (gamelib_in_reset()) {
        return;
    }

    if (dword_5FC240 != NULL) {
        if (sub_40DA20(dword_5FC240->obj) && combat_action_points > 0) {
            return;
        }

        if (dword_5FC240->obj == qword_5FC258) {
            if (combat_action_points == dword_5FC260) {
                dword_5FC264 = true;
            }
            dword_5FC260 = combat_action_points;
        } else {
            qword_5FC258 = dword_5FC240->obj;
        }
    }

    if (!sub_45C0E0(TIMEEVENT_TYPE_TB_COMBAT)) {
        timeevent.type = TIMEEVENT_TYPE_TB_COMBAT;
        sub_45A950(&datetime, 2);
        sub_45B800(&timeevent, &datetime);
    }
}

// 0x4B7150
bool combat_tb_timeevent_process(TimeEvent* timeevent)
{
    (void)timeevent;

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
    sub_440B40(&loc_rect, OBJ_TM_NPC | OBJ_TM_PC, &stru_5FC180);
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
    int64_t pc_obj;
    int64_t pc_loc;
    LocRect loc_rect;
    ObjectList objects;
    ObjectNode* node;

    dword_5FC250 = 0;
    pc_obj = player_get_pc_obj();
    dword_5FC230++;
    pc_loc = obj_field_int64_get(pc_obj, OBJ_F_LOCATION);
    sub_4B7300();

    loc_rect.x1 = LOCATION_GET_X(pc_loc) - dword_5B57B8;
    loc_rect.y1 = LOCATION_GET_Y(pc_loc) - dword_5B57B8;
    loc_rect.x2 = LOCATION_GET_X(pc_loc) + dword_5B57B8;
    loc_rect.y2 = LOCATION_GET_Y(pc_loc) + dword_5B57B8;
    sub_440B40(&loc_rect, OBJ_TM_PC | OBJ_TM_NPC, &objects);
    sub_4414E0(&stru_5FC180, &objects);
    object_list_destroy(&objects);

    sub_4B7EB0();

    dword_5FC250 = 0;
    dword_5FC240 = stru_5FC180.head;
    while (sub_4B7580(dword_5FC240) && dword_5FC240 != NULL) {
        dword_5FC250++;
        dword_5FC240 = dword_5FC240->next;
    }

    if (dword_5FC240 == NULL) {
        if (sub_45D790(pc_obj)) {
            node = stru_5FC180.head;
            dword_5FC250 = 0;
            dword_5FC240 = node;
            while (node != NULL && node->obj != pc_obj) {
                node = node->next;
                dword_5FC240 = node;
                dword_5FC250++;
            }

            if (node == OBJ_HANDLE_NULL) {
                tig_debug_printf("Combat: combat_turn_based_begin_turn: ERROR: Couldn't start TB Combat Turn due to no Active Critters!\n");
                combat_turn_based_end();
                return false;
            }
        }
    } else {
        node = dword_5FC240;
    }

    combat_debug(node != NULL ? node->obj : OBJ_HANDLE_NULL, "TB Begin Turn");
    qword_5FC258 = OBJ_HANDLE_NULL;
    dword_5FC260 = 0;
    dword_5FC264 = 0;
    combat_turn_based_subturn_start();

    return true;
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
        sub_441980(dword_5FC240->obj, dword_5FC240->obj, OBJ_HANDLE_NULL, SAP_END_COMBAT, 0);
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

    if (combat_action_points > 0 && is_pc && critter_fatigue_current(obj) > 1) {
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
        if (location_dist(loc1, loc2) > 1) {
            v1 += sub_4B7BA0(a1, loc2, 1);
        }
    }

    return v1;
}

// 0x4B79A0
bool sub_4B79A0(int64_t a1, int64_t a2)
{
    int64_t loc1;
    int64_t loc2;
    int v1;

    if (!dword_5FC22C) {
        return sub_4B7790(a1, 0);
    }

    v1 = 2;
    loc1 = obj_field_int64_get(a1, OBJ_F_LOCATION);
    loc2 = obj_field_int64_get(a2, OBJ_F_LOCATION);
    if (loc1 != loc2) {
        v1 += sub_4B7BA0(a1, loc2, true);
    }
    return sub_4B7790(a1, v1);
}

// 0x4B7A20
bool sub_4B7A20(int64_t a1, int64_t a2)
{
    int64_t loc1;
    int64_t loc2;
    int v1;

    if (!dword_5FC22C) {
        return sub_4B7790(a1, 0);
    }

    v1 = 1;
    loc1 = obj_field_int64_get(a1, OBJ_F_LOCATION);
    loc2 = obj_field_int64_get(a2, OBJ_F_LOCATION);
    if (loc1 != loc2) {
        v1 += sub_4B7BA0(a1, loc2, false);
    }
    return sub_4B7790(a1, v1);
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
bool sub_4B7CD0(int64_t obj, int action_points)
{
    bool is_pc;
    CombatContext combat;

    if (!dword_5FC22C) {
        return true;
    }

    if (dword_5FC240->obj != obj) {
        return true;
    }

    is_pc = player_is_pc_obj(obj);
    if (combat_action_points >= action_points) {
        combat_action_points -= action_points;

        if (is_pc) {
            combat_callbacks.field_C(combat_action_points);
        }

        return true;
    }

    if (combat_action_points > 0 && is_pc && critter_fatigue_current(obj) > 1) {
        sub_4B2210(OBJ_HANDLE_NULL, obj, &combat);
        combat.flags |= 0x80;
        combat.dam[DAMAGE_TYPE_FATIGUE] = 2;
        sub_4B4390(&combat);

        combat_action_points = 0;
        combat_callbacks.field_C(0);

        return true;
    }

    combat_action_points = 0;
    combat_turn_based_next_subturn();

    return false;
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
    ObjectNode* node;
    ObjectNode* tail;
    ObjectNode* prev;
    ObjectNode* tmp;
    int64_t leader_obj;
    bool process;
    char* name;
    int index;

    combat_debug(OBJ_HANDLE_NULL, "Sorting List");

    tail = NULL;
    prev = NULL;
    node = stru_5FC180.head;
    if (node != NULL) {
        do {
            process = true;
            if (obj_field_int32_get(node->obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
                leader_obj = sub_45DDA0(node->obj);
                if (leader_obj == OBJ_HANDLE_NULL
                    || (obj_field_int32_get(node->obj, OBJ_F_TYPE) != OBJ_TYPE_PC)) {
                    process = false;
                }
            }

            if (process) {
                if (prev != NULL) {
                    prev->next = node->next;
                } else {
                    stru_5FC180.head = node->next;
                }

                if (tail != NULL) {
                    tmp = tail;
                    while (tmp->next != NULL) {
                        tmp = tmp->next;
                    }
                    tmp->next = node;
                } else {
                    tail = node;
                }

                node->next = NULL;
                if (prev != NULL) {
                    node = prev->next;
                } else {
                    node = stru_5FC180.head;
                }
            } else {
                prev = node;
                node = node->next;
            }
        } while (node != NULL);

        if (tail != NULL) {
            if (prev != NULL) {
                prev->next = tail;
            } else {
                stru_5FC180.head = tail;
            }
        }
    }

    node = stru_5FC180.head;
    while (node != NULL) {
        sub_4B80E0(node->obj);
        node = node->next;
    }

    index = 0;
    node = stru_5FC180.head;
    while (node != NULL) {
        name = NULL;
        if (sub_4E5470(node->obj)) {
            if (sub_40DA20(node->obj)) {
                obj_field_string_get(node->obj, OBJ_F_PC_PLAYER_NAME, &name);
            } else {
                obj_field_string_get(node->obj, OBJ_F_NAME, &name);
            }
        }

        tig_debug_printf("Combat: TB: DBG: List[%d]: %s\n",
            index,
            name != NULL ? name : " ");

        // NOTE: Original code is slightly different and buggy - it attempts
        // to release `name` depending on a flag which is not reset on every
        // iteration.
        if (name != NULL) {
            FREE(name);
        }

        node = node->next;
        index++;
    }


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
int sub_4B80D0()
{
    return dword_5FC230;
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

        if (critter_is_dead(obj) || sub_4B7DC0(obj)) {
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
    int tf_type;
    MesFileEntry mes_file_entry;

    flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS);
    if ((flags & OCF_BLINDED) != 0) {
        return false;
    }

    flags |= OCF_BLINDED;
    obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS, flags);

    tf_type = obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        ? TF_TYPE_RED
        : TF_TYPE_WHITE;

    mes_file_entry.num = 4; // "Blinded"
    mes_get_msg(combat_mes_file, &mes_file_entry);
    tf_add(obj, tf_type, mes_file_entry.str);

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
