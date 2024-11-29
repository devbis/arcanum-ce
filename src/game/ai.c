#include "game/ai.h"

#include "game/anim_private.h"
#include "game/anim.h"
#include "game/combat.h"
#include "game/critter.h"
#include "game/dialog.h"
#include "game/item.h"
#include "game/magictech.h"
#include "game/map.h"
#include "game/mt_ai.h"
#include "game/multiplayer.h"
#include "game/name.h"
#include "game/obj.h"
#include "game/object_node.h"
#include "game/object.h"
#include "game/player.h"
#include "game/proto.h"
#include "game/random.h"
#include "game/reaction.h"
#include "game/script.h"
#include "game/skill.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/tile.h"
#include "game/timeevent.h"
#include "game/ui.h"

#define CLOCKWORK_DECOY 6719

typedef union AiParams {
    struct {
        /* 0000 */ int field_0; // Percentage of NPC hit points below which NPC will flee.
        /* 0004 */ int field_4; // Number of people besides PC beyond which NPC will flee.
        /* 0008 */ int field_8; // Number of levels above NPC beyond which NPC will flee.
        /* 000C */ int field_C; // Percentage of PC hit points below which NPC will never flee.
        /* 0010 */ int field_10; // How far to flee, in tiles.
        /* 0014 */ int field_14; // The reaction level at which the NPC will not follow the PC.
        /* 0018 */ int field_18; // How far PC align is above NPC align before NPC wont follow.
        /* 001C */ int field_1C; // How far PC align is below NPC align before NPC wont follow.
        /* 0020 */ int field_20; // How many levels the NPC can be above the PC and still join.
        /* 0024 */ int field_24; // How much a non-accidental hit will lower a follower's reaction.
        /* 0028 */ int field_28; // NPC will attack if his reaction is below this.
        /* 002C */ int field_2C; // How different alignments can be before non-follower NPC attacks.
        /* 0030 */ int field_30; // Alignment of target at (or above) which the good-aligned follower NPC will not attack.
        /* 0034 */ int field_34; // Chance of throwing defensive spell (as opposed to offensive).
        /* 0038 */ int field_38; // Chance of throwing a healing spell in combat.
        /* 003C */ int field_3C; // Minimum distance in combat.
        /* 0040 */ int field_40; // The NPC can open portals if this is nonzero and cannot if it is zero.
    };
    int values[17];
} AiParams;

static_assert(sizeof(AiParams) == 0x44, "wrong size");

typedef struct Ai {
    /* 0000 */ int64_t obj;
    /* 0008 */ int64_t danger_source;
    /* 0010 */ int danger_type;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
    /* 0020 */ int64_t field_20;
    /* 0028 */ int64_t leader_obj;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
} Ai;

static_assert(sizeof(Ai) == 0x38, "wrong size");

typedef struct S4ABF10 {
    /* 0000 */ unsigned int flags;
    /* 0004 */ S600A20_Entry* entries;
    /* 0008 */ int cnt;
    /* 0010 */ int64_t obj;
} S4ABF10;

static_assert(sizeof(S4ABF10) == 0x18, "wrong size");

static bool sub_4A8570(Ai* ai);
static void sub_4A88D0(Ai* ai, int64_t obj);
static bool sub_4A8940(Ai* ai);
static bool sub_4A8AA0(Ai* ai, int64_t obj, bool a3);
static bool sub_4A8E70(Ai* ai);
static void sub_4A92D0(Ai* ai);
static void sub_4A94C0(int64_t obj, int64_t tgt);
static void sub_4A9B80(int64_t a1, int64_t a2, int a3, int a4);
static void sub_4A9C00(int64_t a1, int64_t a2, int64_t a3, int a4, int a5, int a6);
static void sub_4A9F10(int64_t a1, int64_t a2, int64_t a3, int a4);
static void sub_4AA420(int64_t obj, int64_t a2);
static void sub_4AA620(int64_t a1, int64_t a2);
static bool sub_4AAA30(TimeEvent* timeevent);
static void sub_4AAA60(int64_t obj, AiParams* params);
static void ai_danger_source(int64_t obj, int* type_ptr, int64_t* danger_source_ptr);
static int sub_4AABE0(int64_t a1, int a2, int64_t a3, int* a4);
static bool sub_4AAF50(Ai* ai);
static bool sub_4AB030(int64_t a1, int64_t a2);
static int64_t sub_4AB0B0(int64_t a1, int64_t a2, int64_t a3);
static void sub_4AB2A0(int64_t a1, int64_t a2);
static bool sub_4AB2F0(int64_t a1, int64_t a2);
static int64_t sub_4AB460(int64_t a1);
static bool sub_4AB990(int64_t a1, int64_t a2);
static void sub_4ABC20(Ai* ai);
static bool sub_4ABC70(Ai* ai);
static int sub_4ABE20(Ai* ai);
static bool sub_4ABEB0(int64_t obj, int64_t tgt);
static bool sub_4ABF10(Ai* ai, S4ABF10* a2);
static void sub_4AC180(Ai* ai);
static void sub_4AC250(Ai* ai);
static void sub_4AC320(Ai* ai);
static void sub_4AC350(Ai* ai);
static void sub_4AC380(Ai* ai);
static void sub_4AC620(Ai* ai);
static void sub_4AC660(Ai* ai);
static void sub_4AC6E0(Ai* ai);
static void sub_4AC7B0(Ai* ai);
static bool sub_4AC910(Ai* ai, int64_t a2);
static bool sub_4ACBB0(int64_t obj, bool a2);
static bool ai_is_day();
static bool sub_4ACDB0(int64_t obj, bool a2);
static bool ai_get_standpoint(int64_t obj, int64_t* standpoint_ptr);
static void sub_4AD0B0(int64_t npc_obj);
static int64_t ai_find_nearest_bed(int64_t obj);
static void sub_4AD1B0(int64_t a1, int64_t a2, int a3);
static bool sub_4AD420(int64_t obj);
static bool sub_4AD4D0(int64_t obj);
static int sub_4AD5D0(int64_t obj);
static int sub_4AD610(int64_t obj);
static bool sub_4AD6B0(TimeEvent* timeevent);
static void sub_4AD700(int64_t obj, int millis);
static void sub_4AD730(int64_t obj, DateTime* datetime);
static int sub_4ADB50(int64_t npc_obj, int64_t pc_obj);
static int sub_4ADCC0(int64_t a1, int64_t a2, int64_t a3);
static void sub_4AE0A0(int64_t obj, int* cnt_ptr, int* lvl_ptr);
static int sub_4AE3A0(int64_t a1, int64_t a2);
static int64_t sub_4AE450(int64_t a1, int64_t a2);
static int sub_4AE720(int64_t a1, int64_t item_obj, int64_t a3, int magictech);
static bool sub_4AECA0(int64_t a1, int a2);
static int sub_4AF240(int value);
static int sub_4AF640(int64_t a1, int64_t a2);
static bool sub_4AF800(int64_t obj, int64_t a2);
static void sub_4AF8C0(int64_t a1, int64_t a2);
static void sub_4AF930(int64_t a1, int64_t a2);
static void sub_4AF9D0(int64_t a1, int64_t a2);
static int64_t sub_4AFA90(int64_t obj);

// 0x5B50C0
static int dword_5B50C0[] = {
    2,
    8,
};

// 0x5B50C8
static int dword_5B50C8 = 15;

// 0x5B50CC
static bool dword_5B50CC = true;

// NOTE: Original code likely defined it as `int[150][17]`.
//
// 0x5F5CB0
static AiParams dword_5F5CB0[150];

// 0x5F8488
static Func5F8488* dword_5F8488;

// 0x5F848C
static Func5F848C* dword_5F848C;

// 0x5F8490
static int64_t qword_5F8490;

// 0x5F84A0
static int64_t qword_5F84A0;

// 0x4A8320
bool ai_init(GameInitInfo* init_info)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    int param;
    int idx;
    char* str;
    char* tok;

    (void)init_info;

    dword_5F848C = NULL;
    dword_5F8488 = NULL;

    if (!mes_load("rules\\ai_params.mes", &mes_file)) {
        return false;
    }

    mes_file_entry.num = 0;
    if (mes_search(mes_file, &mes_file_entry)) {
        idx = mes_file_entry.num;
        do {
            str = mes_file_entry.str;
            for (param = 0; param < 17; param++) {
                tok = strtok(str, " \t\n");
                if (tok == NULL) {
                    break;
                }

                dword_5F5CB0[idx].values[param] = atoi(tok);

                str = NULL;
            }
        } while (idx < 100 && mes_find_next(mes_file, &mes_file_entry));
    }

    mes_unload(mes_file);

    return true;
}

// 0x4A83F0
void ai_exit()
{
}

// 0x4A8400
bool ai_mod_load()
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    int param;
    int idx;
    char* str;
    char* tok;

    if (mes_load("rules\\gameai.mes", &mes_file)) {
        mes_file_entry.num = 100;
        if (mes_search(mes_file, &mes_file_entry)) {
            idx = mes_file_entry.num;
            do {
                str = mes_file_entry.str;
                for (param = 0; param < 17; param++) {
                    tok = strtok(str, " \t\n");
                    if (tok == NULL) {
                        break;
                    }

                    dword_5F5CB0[idx].values[param] = atoi(tok);

                    str = NULL;
                }
            } while (idx < 150 && mes_find_next(mes_file, &mes_file_entry));
        }

        mes_unload(mes_file);
    }

    return true;
}

// 0x4A84C0
void ai_mod_unload()
{
}

// 0x4A84D0
void sub_4A84D0(Func5F848C* a1, Func5F8488* a2)
{
    dword_5F848C = a1;
    dword_5F8488 = a2;
}

// 0x4A84F0
void sub_4A84F0(int64_t obj)
{
    Ai ai;

    if (!map_is_clearing_objects()
        && ((tig_net_flags & TIG_NET_CONNECTED) == 0
            || (tig_net_flags & TIG_NET_HOST) != 0)) {
        sub_4A88D0(&ai, obj);
        if (sub_4A8570(&ai)) {
            if (!sub_4A8940(&ai) && !sub_4A8E70(&ai)) {
                sub_4A92D0(&ai);
            }
            sub_4AC180(&ai);
        }
    }
}

// 0x4A8570
bool sub_4A8570(Ai* ai)
{
    int64_t pc_obj;
    int64_t v1;
    unsigned int critter_flags;
    unsigned int npc_flags;
    int64_t leader_obj;

    if (sub_45D8D0(ai->obj)) {
        return false;
    }

    if (!critter_is_sleeping(ai->obj)
        && (obj_field_int32_get(ai->obj, OBJ_F_FLAGS) & (OF_OFF | OF_DONTDRAW)) != 0) {
        return false;
    }

    pc_obj = player_get_pc_obj();

    if ((obj_field_int32_get(pc_obj, OBJ_F_FLAGS) & OF_OFF) != 0) {
        return false;
    }

    if ((obj_field_int32_get(ai->obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    critter_flags = obj_field_int32_get(ai->obj, OBJ_F_CRITTER_FLAGS);

    v1 = sub_4C1110(ai->obj);
    if (v1) {
        if (pc_obj == v1
            && dword_5F848C != NULL
            && ai->danger_type != 0
            && ai->danger_type != 3) {
            dword_5F848C(pc_obj, 0);
        }
        return false;
    }

    if (combat_critter_is_combat_mode_active(ai->obj)) {
        if (!sub_4B6D70()) {
            sub_441980(ai->danger_source, ai->obj, OBJ_HANDLE_NULL, SAP_END_COMBAT, 0);
        }
        sub_441980(ai->danger_source, ai->obj, OBJ_HANDLE_NULL, SAP_START_COMBAT, 0);
    }

    if (sub_4B6D70() && sub_4B6D80() != ai->obj) {
        return false;
    }

    if (sub_4233D0(ai->obj) > 2) {
        return false;
    }

    if ((critter_flags & OCF_BLINDED) != 0
        && random_between(1, 160) <= 1) {
        critter_flags &= ~OCF_BLINDED;
        obj_field_int32_set(ai->obj, OBJ_F_CRITTER_FLAGS, critter_flags);
    }

    if ((critter_flags & OCF_CRIPPLED) != 0
        && random_between(1, 8000) <= 1) {
        critter_flags &= ~OCF_CRIPPLED;
        obj_field_int32_set(ai->obj, OBJ_F_CRITTER_FLAGS, critter_flags);
    }

    if ((critter_flags & OCF_STUNNED) != 0) {
        if (!sub_44E830(ai->obj, AG_ANIMATE_STUNNED, NULL)) {
            critter_flags &= ~OCF_STUNNED;
            obj_field_int32_set(ai->obj, OBJ_F_CRITTER_FLAGS, critter_flags);
            return false;
        }
    }

    if (sub_45D800(ai->obj)) {
        return false;
    }

    npc_flags = obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS);
    if ((npc_flags & ONF_GENERATOR) != 0) {
        return false;
    }

    if ((npc_flags & ONF_CHECK_WIELD) != 0) {
        sub_4654F0(ai->obj, ai->danger_source);

        npc_flags = obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS);
        npc_flags &= ~ONF_CHECK_WIELD;
        obj_field_int32_set(ai->obj, OBJ_F_NPC_FLAGS, npc_flags);
    } else if ((npc_flags & ONF_CHECK_WEAPON) != 0) {
        sub_465170(ai->obj, 1004, ai->danger_source);

        npc_flags = obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS);
        npc_flags &= ~ONF_CHECK_WEAPON;
        obj_field_int32_set(ai->obj, OBJ_F_NPC_FLAGS, npc_flags);
    }

    if ((npc_flags & ONF_DEMAINTAIN_SPELLS) != 0) {
        leader_obj = critter_leader_get(ai->obj);
        if (leader_obj == OBJ_HANDLE_NULL
            || sub_45D8D0(leader_obj)
            || !combat_critter_is_combat_mode_active(leader_obj)) {
            npc_flags &= ~ONF_DEMAINTAIN_SPELLS;
            obj_field_int32_set(ai->obj, OBJ_F_NPC_FLAGS, npc_flags);
            sub_457450(ai->obj);
        }
    }

    return true;
}

// 0x4A88D0
void sub_4A88D0(Ai* ai, int64_t obj)
{
    ai->obj = obj;
    ai->danger_source = OBJ_HANDLE_NULL;
    ai->danger_type = 0;
    ai->field_14 = 0;
    ai->field_18 = 10000;
    ai->field_1C = -1;
    ai->field_20 = 0;
    ai->leader_obj = critter_leader_get(obj);
    ai_danger_source(obj, &(ai->danger_type), &(ai->danger_source));
    ai->field_30 = -1;
}

// 0x4A8940
bool sub_4A8940(Ai* ai)
{
    bool v1;
    AiParams ai_params;
    int64_t v2;
    ObjectList objects;
    ObjectNode* node;

    if (critter_is_sleeping(ai->obj)) {
        return false;
    }

    if (ai->danger_type != 2) {
        return false;
    }

    v1 = true;

    if (combat_critter_is_combat_mode_active(ai->obj)) {
        sub_4AAA60(ai->obj, &ai_params);
        if (random_between(1, 100) > ai_params.field_38) {
            v1 = false;
        }
    }

    if (sub_4A8AA0(ai, ai->obj, v1)) {
        return true;
    }

    v2 = sub_45DDA0(ai->obj);
    if (v2 == OBJ_HANDLE_NULL) {
        v2 = ai->leader_obj;
    }

    if (v2 != OBJ_HANDLE_NULL) {
        if (sub_4A8AA0(ai, v2, v1)) {
            return true;
        }
    } else {
        v2 = ai->obj;
    }

    sub_441260(v2, &objects);
    node = objects.head;
    while (node != NULL) {
        if (node->obj != ai->obj
            && sub_4A8AA0(ai, node->obj, v1)) {
            break;
        }
        node = node->next;
    }
    object_list_destroy(&objects);

    if (node == NULL) {
        return false;
    }

    return true;
}

// 0x4A8AA0
bool sub_4A8AA0(Ai* ai, int64_t obj, bool a3)
{
    S4ABF10 v1;
    S5FF620 v2;
    int v3;
    int64_t item_obj;

    if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & ONF_NO_ATTACK) != 0) {
        return false;
    }

    if (sub_45D8D0(obj)) {
        sub_4CCA90(&v2, ai->obj, 9);
        v1.flags = 0x8;
        v1.entries = v2.field_30[9].entries;
        v1.cnt = v2.field_30[9].cnt;
        v1.obj = obj;
        if (sub_4ABF10(ai, &v1)) {
            sub_4CCBF0(&v2);
            return true;
        } else {
            sub_4CCBF0(&v2);
            return false;
        }
    }

    v3 = sub_4AB400(obj);
    if (v3 > 30 && !a3) {
        return false;
    }

    if (ai->danger_type != 0) {
        if (sub_4AB430(ai->obj) < 20) {
            sub_4CCA90(&v2, ai->obj, 8);
            v1.flags = 0x8;
            v1.entries = v2.field_30[8].entries;
            v1.cnt = v2.field_30[8].cnt;
            v1.obj = obj;
            if (sub_4ABF10(ai, &v1)) {
                sub_4CCBF0(&v2);
                return true;
            }

            sub_4CCBF0(&v2);
        }
    } else {
        if (v3 < 90) {
            item_obj = sub_4C91F0(ai->obj, BASIC_SKILL_HEAL);
            if (!sub_4AE570(ai->obj, obj, item_obj, BASIC_SKILL_HEAL)) {
                ai->danger_source = obj;
                ai->field_20 = item_obj;
                ai->field_14 = 3;
                ai->field_1C = 10;
                return true;
            }
        }
    }

    if (v3 < 40) {
        sub_4CCA90(&v2, ai->obj, 6);
        v1.flags = 0x8;
        v1.entries = v2.field_30[6].entries;
        v1.cnt = v2.field_30[6].cnt;
        v1.obj = obj;
        if (sub_4ABF10(ai, &v1)) {
            sub_4CCBF0(&v2);
            return true;
        }

        sub_4CCBF0(&v2);
    }

    if (v3 < 55) {
        sub_4CCA90(&v2, ai->obj, 5);
        v1.flags = 0x8;
        v1.entries = v2.field_30[5].entries;
        v1.cnt = v2.field_30[5].cnt;
        v1.obj = obj;
        if (sub_4ABF10(ai, &v1)) {
            sub_4CCBF0(&v2);
            return true;
        }

        sub_4CCBF0(&v2);
    }

    if (stat_level(obj, STAT_POISON_LEVEL) > 0) {
        sub_4CCA90(&v2, ai->obj, 7);
        v1.flags = 0x8;
        v1.entries = v2.field_30[7].entries;
        v1.cnt = v2.field_30[7].cnt;
        v1.obj = obj;
        if (sub_4ABF10(ai, &v1)) {
            sub_4CCBF0(&v2);
            return true;
        }

        sub_4CCBF0(&v2);
    }

    if (v3 < 70
        || (ai->danger_type == 0 && v3 < 90)) {
        sub_4CCA90(&v2, ai->obj, 4);
        v1.flags = 0x8;
        v1.entries = v2.field_30[4].entries;
        v1.cnt = v2.field_30[4].cnt;
        v1.obj = obj;
        if (sub_4ABF10(ai, &v1)) {
            sub_4CCBF0(&v2);
            return true;
        }

        sub_4CCBF0(&v2);
    }

    return false;
}

// 0x4A8E70
bool sub_4A8E70(Ai* ai)
{
    // TODO: Incomplete.
}

// 0x4A8F90
void sub_4A8F90()
{
    // TODO: Incomplete.
}

// 0x4A92D0
void sub_4A92D0(Ai* ai)
{
    // TODO: Incomplete.
}

// 0x4A94C0
void sub_4A94C0(int64_t obj, int64_t tgt)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC && obj != tgt) {
        sub_4A9650(obj, tgt, 0, 0);
        obj_field_handle_set(tgt, OBJ_F_NPC_COMBAT_FOCUS, obj);
        obj_field_handle_set(tgt, OBJ_F_NPC_WHO_HIT_ME_LAST, obj);
    }
}

// 0x4A9530
void sub_4A9530(AiRedirect* ai_redirect, int64_t a2, int64_t a3)
{
    ai_redirect->field_0 = a2;
    ai_redirect->field_8 = a3;
    ai_redirect->min_iq = -1;
    ai_redirect->critter_flags = 0;
}

// 0x4A9560
void sub_4A9560(AiRedirect* ai_redirect)
{
    ObjectList objects;
    ObjectNode* node;

    if (ai_redirect->field_0 == OBJ_HANDLE_NULL
        || ai_redirect->field_8 == OBJ_HANDLE_NULL) {
        return;
    }

    sub_440FC0(ai_redirect->field_8, OBJ_TM_PC | OBJ_TM_NPC, &objects);
    node = objects.head;
    while (node != NULL) {
        if (!sub_45D8D0(node->obj)
            && node->obj != ai_redirect->field_0
            && node->obj != ai_redirect->field_8
            && (ai_redirect->min_iq <= 0
                || stat_level(ai_redirect->min_iq, STAT_INTELLIGENCE) <= ai_redirect->min_iq)
            && (ai_redirect->critter_flags == 0
                || (obj_field_int32_get(node->obj, OBJ_F_CRITTER_FLAGS) & ai_redirect->critter_flags) != 0)) {
            sub_4A94C0(ai_redirect->field_0, node->obj);
        }
        node = node->next;
    }
    object_list_destroy(&objects);
}

// 0x4A9650
void sub_4A9650(int64_t obj, int64_t tgt, int a3, unsigned int flags)
{
    // TODO: Incomplete.
}

// 0x4A9AD0
void sub_4A9AD0(int64_t a1, int64_t a2)
{
    if (a1 != a2) {
        if (dword_5F848C != NULL) {
            dword_5F848C(a1, 0);
        }

        if (!combat_critter_is_combat_mode_active(a1)) {
            combat_critter_activate_combat_mode(a1);
            if (sub_4B6D70()) {
                sub_4B6E70(a2);
            }
        }

        if (!sub_423300(a1, 0) || sub_423470(a1)) {
            if (combat_auto_attack_get(a1)) {
                if (!sub_44E830(a1, 19, 0)) {
                    sub_434AE0(a1, a2);
                }
            }
        }
    }
}

// 0x4A9B80
void sub_4A9B80(int64_t a1, int64_t a2, int a3, int a4)
{
    ObjectList followers;
    ObjectNode* node;

    if (a1 != a2) {
        object_get_followers(a1, &followers);
        node = followers.head;
        while (node != NULL) {
            sub_4A9C00(node->obj, a1, a2, a3, a4, 0);
            node = node->next;
        }
        object_list_destroy(&followers);
    }
}

// 0x4A9C00
void sub_4A9C00(int64_t a1, int64_t a2, int64_t a3, int a4, int a5, int a6)
{
    // TODO: Incomplete.
}

// 0x4A9E10
void sub_4A9E10()
{
    // TODO: Incomplete.
}

// 0x4A9F10
void sub_4A9F10(int64_t a1, int64_t a2, int64_t a3, int a4)
{
    int64_t leader_obj;
    int danger_type;

    if ((obj_field_int32_get(a1, OBJ_F_NPC_FLAGS) & ONF_AI_WAIT_HERE) != 0) {
        leader_obj = OBJ_HANDLE_NULL;
    } else {
        leader_obj = critter_leader_get(a1);
    }

    if (a1 != a3 && a1 != a2 && leader_obj != a2 && leader_obj != a3) {
        if (critter_is_sleeping(a1)) {
            sub_4AD0B0(a1);
        }

        if (sub_4AE3A0(a1, a3)) {
            if (sub_4AF260(a1, a3) == 0 || sub_4AF470(a1, a3, a4) == 0) {
                sub_4AA620(a1, a2);
            }
        } else if (sub_4AE3A0(a1, a2)) {
            if (sub_4AF260(a1, a3) == 0 || sub_4AF470(a1, a3, a4) == 0) {
                sub_4AA620(a1, a3);
            }
        } else if (critter_social_class_get(a1) != SOCIAL_CLASS_GUARD
            && (obj_field_int32_get(a1, OBJ_F_CRITTER_FLAGS) & OCF_NO_FLEE) == 0) {
            ai_danger_source(a1, &danger_type, NULL);
            if (danger_type == 0
                && (sub_4AF260(a1, a3) == 0 || sub_4AF470(a1, a3, a4) == 0)) {
                    sub_4AABE0(a1, 2, a2, 0);
                }
        }
    }
}

// 0x4AA0D0
void sub_4AA0D0(int64_t obj)
{
    ObjectList npcs;
    ObjectNode* node;

    sub_4AE4E0(obj, 10, &npcs, OBJ_TM_NPC);

    node = npcs.head;
    while (node != NULL) {
        if (!sub_45D8D0(node->obj)
            && (obj_field_int32_get(node->obj, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) == 0
            && sub_45DDA0(node->obj) != obj
            && (sub_4AF260(node->obj, obj) == 0 || !sub_4AF470(node->obj, obj, 0))) {
            sub_4A9650(obj, node->obj, 0, 0);
        }
        node = node->next;
    }
}

// 0x4AA1B0
void sub_4AA1B0(int64_t a1, int64_t a2)
{
    sub_4AF170(a1);
    sub_4AA8C0(a1, true);

    if (a2 != OBJ_HANDLE_NULL
        && obj_field_int32_get(a2, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        ObjectList objects;
        ObjectNode* node;
        int danger_type;
        int64_t danger_obj;
        unsigned int flags;

        sub_4AE4E0(a2, 3, &objects, OBJ_TM_NPC);
        node = objects.head;
        while (node != NULL) {
            if (!sub_45D8D0(node->obj)
                && (obj_field_int32_get(node->obj, OBJ_F_CRITTER_FLAGS) & OCF_NO_FLEE) == 0
                && critter_faction_same(a1, node->obj)) {
                ai_danger_source(node->obj, &danger_type, &danger_obj);
                if (danger_type == 1
                    && danger_obj == a2) {
                    flags = obj_field_int32_get(node->obj, OBJ_F_NPC_FLAGS);
                    flags |= ONF_BACKING_OFF;
                    obj_field_int32_set(node->obj, OBJ_F_NPC_FLAGS, flags);
                }
            }
            node = node->next;
        }
        object_list_destroy(&objects);
    }
}

// 0x4AA300
void sub_4AA300(int64_t a1, int64_t a2)
{
    // TODO: Incomplete.
}

// 0x4AA420
void sub_4AA420(int64_t obj, int64_t a2)
{
    if (obj_field_handle_get(obj, OBJ_F_NPC_COMBAT_FOCUS) == a2) {
        obj_field_handle_set(obj, OBJ_F_NPC_COMBAT_FOCUS, OBJ_HANDLE_NULL);
    }
    if (obj_field_handle_get(obj, OBJ_F_NPC_WHO_HIT_ME_LAST) == a2) {
        obj_field_handle_set(obj, OBJ_F_NPC_WHO_HIT_ME_LAST, OBJ_HANDLE_NULL);
    }
    sub_4AF9D0(obj, a2);
}

// 0x4AA4A0
void sub_4AA4A0(int64_t obj)
{
    int type;
    int64_t combat_focus_obj;
    unsigned int flags;
    ObjectList objects;
    ObjectNode* node;

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    switch (type) {
    case OBJ_TYPE_NPC:
        combat_focus_obj = obj_field_handle_get(obj, OBJ_F_NPC_COMBAT_FOCUS);
        if (combat_focus_obj != OBJ_HANDLE_NULL) {
            sub_4AA300(obj, combat_focus_obj);
            sub_424070(obj, 3, 0, 0);
            combat_critter_deactivate_combat_mode(obj);
        }
        if (sub_45DDA0(obj)) {
            flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
            flags |= ONF_NO_ATTACK;
            obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, flags);
        }
        ai_target_unlock(obj);
        break;
    case OBJ_TYPE_PC:
        sub_441260(obj, &objects);

        node = objects.head;
        while (node != NULL) {
            sub_4AA4A0(node->obj);
            node = node->next;
        }

        object_list_destroy(&objects);
        break;
    }
}

// 0x4AA580
void sub_4AA580(int64_t obj)
{
    int type;
    unsigned int flags;
    ObjectList objects;
    ObjectNode* node;

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    switch (type) {
    case OBJ_TYPE_NPC:
        if (sub_45DDA0(obj)) {
            flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
            flags &= ~ONF_NO_ATTACK;
            obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, flags);
        }
        break;
    case OBJ_TYPE_PC:
        sub_441260(obj, &objects);

        node = objects.head;
        while (node != NULL) {
            sub_4AA580(node->obj);
            node = node->next;
        }

        object_list_destroy(&objects);
        break;
    }
}

// 0x4AA620
void sub_4AA620(int64_t a1, int64_t a2)
{
    // TODO: Incomplete.
}

// 0x4AA7A0
void sub_4AA7A0(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x4AA8C0
void sub_4AA8C0(int64_t obj, bool force)
{
    int64_t leader_obj;
    unsigned int flags;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC
        && ((tig_net_flags & TIG_NET_CONNECTED) == 0
            || (tig_net_flags & TIG_NET_HOST) != 0)) {
        leader_obj = critter_leader_get(obj);
        if (leader_obj != OBJ_HANDLE_NULL
            && (obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) & ONF_AI_WAIT_HERE) != 0
            && (force || !sub_4AD950(obj, leader_obj, false))) {
            flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
            flags &= ~ONF_AI_WAIT_HERE;
            obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, flags);

            critter_leader_set(obj, OBJ_HANDLE_NULL);

            qword_5F8490 = obj;
            timeevent_clear_one_ex(TIMEEVENT_TYPE_NPC_WAIT_HERE, sub_4AAA30);

            critter_follow(obj, leader_obj, force);
        }
    }
}

// 0x4AA990
bool ai_npc_wait_here_timeevent_process(TimeEvent* timeevent)
{
    int64_t obj;
    int64_t leader_obj;
    unsigned int flags;
    int max_charisma;
    int charisma;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        obj = timeevent->params[0].object_value;
        leader_obj = critter_leader_get(obj);
        if (leader_obj != OBJ_HANDLE_NULL) {
            flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
            flags &= ~ONF_AI_WAIT_HERE;
            flags |= ONF_JILTED;
            obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, flags);

            max_charisma = stat_get_max_value(leader_obj, STAT_CHARISMA);
            charisma = stat_level(leader_obj, STAT_CHARISMA);
            sub_4C0DE0(obj, leader_obj, 2 * (charisma - max_charisma));
            critter_leader_set(obj, OBJ_HANDLE_NULL);
        }
    }

    return true;
}

// 0x4AAA30
bool sub_4AAA30(TimeEvent* timeevent)
{
    return timeevent->params[0].object_value == qword_5F8490;
}

// 0x4AAA60
void sub_4AAA60(int64_t obj, AiParams* params)
{
    int index;

    index = obj_field_int32_get(obj, OBJ_F_NPC_AI_DATA);

    // NOTE: Original code is different. It treats `params` as an array of 17
    // integers and copies them one by one in the loop.
    *params = dword_5F5CB0[index];
}

// 0x4AAAA0
bool ai_is_fighting(int64_t obj)
{
    int type;

    ai_danger_source(obj, &type, NULL);

    return type == 1;
}

// 0x4AAB00
void ai_danger_source(int64_t obj, int* type_ptr, int64_t* danger_source_ptr)
{
    if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_FLEEING) != 0) {
        if (danger_source_ptr != NULL) {
            obj_field_obj_get(obj, OBJ_F_CRITTER_FLEEING_FROM, danger_source_ptr);
        }
        *type_ptr = 2;
    } else if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_SURRENDERED) != 0) {
        if (danger_source_ptr != NULL) {
            obj_field_obj_get(obj, OBJ_F_CRITTER_FLEEING_FROM, danger_source_ptr);
        }
        *type_ptr = 3;
    } else if ((obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) & ONF_FIGHTING) != 0) {
        if (danger_source_ptr != NULL) {
            obj_field_obj_get(obj, OBJ_F_NPC_COMBAT_FOCUS, danger_source_ptr);
        }
        *type_ptr = 1;
    } else {
        if (danger_source_ptr != NULL) {
            *danger_source_ptr = OBJ_HANDLE_NULL;
        }
        *type_ptr = 0;
    }
}

// 0x4AABE0
int sub_4AABE0(int64_t a1, int a2, int64_t a3, int* a4)
{
    // TODO: Incomplete.
}

// 0x4AAF50
bool sub_4AAF50(Ai* ai)
{
    unsigned int critter_flags;
    S4ABF10 v1;
    S5FF620 v2;

    critter_flags = obj_field_int32_get(ai->obj, OBJ_F_CRITTER_FLAGS);
    if ((critter_flags & OCF_SPELL_FLEE) != 0) {
        obj_field_int32_set(ai->obj, OBJ_F_CRITTER_FLAGS, critter_flags & ~OCF_SPELL_FLEE);
        return false;
    }

    sub_4CCA90(&v2, ai->obj, 0);
    v1.flags = 0x1;
    v1.entries = v2.field_30[0].entries;
    v1.cnt = v2.field_30[0].cnt;
    if (!sub_4ABF10(ai, &v1)) {
        sub_4CCBF0(&v2);
        return false;
    }

    obj_field_int32_set(ai->obj, OBJ_F_CRITTER_FLAGS, critter_flags | OCF_SPELL_FLEE);
    sub_4CCBF0(&v2);

    return true;
}

// 0x4AB030
bool sub_4AB030(int64_t a1, int64_t a2)
{
    AiParams params;

    if ((obj_field_int32_get(a1, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) != 0) {
        return true;
    }

    sub_4AAA60(a1, &params);

    return sub_441AE0(a1, a2) > params.field_10;
}

// 0x4AB0B0
int64_t sub_4AB0B0(int64_t a1, int64_t a2, int64_t a3)
{
    int64_t distance1;
    int64_t distance2;
    int score1;
    int score2;

    if (a2 == OBJ_HANDLE_NULL) {
        return a3;
    }

    if (a3 == OBJ_HANDLE_NULL) {
        return a2;
    }

    if (!obj_type_is_critter(obj_field_int32_get(a3, OBJ_F_TYPE))) {
        return a2;
    }

    if (!obj_type_is_critter(obj_field_int32_get(a2, OBJ_F_TYPE))) {
        return a3;
    }

    if (sub_45D800(a3)) {
        return a2;
    }

    if (sub_45D800(a2)) {
        return a3;
    }

    distance1 = sub_441AE0(a1, a3);
    if (distance1 > 20) {
        return a2;
    }

    distance2 = sub_441AE0(a1, a2);
    if (distance2 > 20) {
        return a3;
    }

    if (sub_4AF800(a1, a2)) {
        return a2;
    }

    if (sub_4AF800(a1, a3)) {
        return a3;
    }

    if (random_between(1, 100) > 50) {
        score1 = -((int)distance1);
        score2 = -((int)distance2);
    } else {
        score1 = stat_level(a3, STAT_LEVEL) - (int)distance1;
        score2 = stat_level(a3, STAT_LEVEL) - (int)distance2;
    }

    if (score1 > score2) {
        return a3;
    } else {
        return a2;
    }
}

// 0x4AB2A0
void sub_4AB2A0(int64_t a1, int64_t a2)
{
    if (sub_4AB2F0(a1, a2)) {
        sub_4AABE0(a1, 2, a2, 0);
    } else {
        sub_4AABE0(a1, 1, a2, 0);
    }
}

// 0x4AB2F0
bool sub_4AB2F0(int64_t a1, int64_t a2)
{
    AiParams ai_params;
    int v1;
    int v2;
    int v3;
    int v4;

    if ((obj_field_int32_get(a1, OBJ_F_CRITTER_FLAGS) & OCF_NO_FLEE) != 0) {
        return false;
    }

    if ((obj_field_int32_get(a1, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) != 0) {
        return false;
    }

    sub_4AAA60(a1, &ai_params);

    if (sub_4AB400(a2) < ai_params.field_C) {
        return false;
    }

    if (sub_4AB400(a2) <= ai_params.field_0) {
        return true;
    }

    sub_4AE020(a2, &v1, &v2);
    sub_4AE020(a1, &v3, &v4);

    if (v1 - v3 >= ai_params.field_4) {
        return true;
    }

    if (v2 - v4 >= ai_params.field_8) {
        return true;
    }

    return false;
}

// 0x4AB400
int sub_4AB400(int64_t obj)
{
    return 100 * sub_43D600(obj) / sub_43D5A0(obj);
}

// 0x4AB430
int sub_4AB430(int64_t obj)
{
    return 100 * sub_45D700(obj) / sub_45D670(obj);
}

// 0x4AB460
int64_t sub_4AB460(int64_t a1)
{
    // TODO: Incomplete.
}

// 0x4AB990
bool sub_4AB990(int64_t a1, int64_t a2)
{
    // TODO: Incomplete.
}

// 0x4ABBC0
int64_t sub_4ABBC0(int64_t obj)
{
    int64_t combat_focus_obj;

    obj_field_obj_get(obj, OBJ_F_NPC_COMBAT_FOCUS, &combat_focus_obj);
    if (sub_4AB990(obj, combat_focus_obj)) {
        return combat_focus_obj;
    }

    ai_target_unlock(obj);

    return sub_4AB460(obj);
}

// 0x4ABC20
void sub_4ABC20(Ai* ai)
{
    if ((obj_field_int32_get(ai->obj, OBJ_F_FLAGS) & OF_INVULNERABLE) != 0
        || (obj_field_int32_get(ai->obj, OBJ_F_CRITTER_FLAGS2) & OCF2_NIGH_INVULNERABLE) != 0
        || !sub_4ABC70(ai)) {
        ai->field_14 = 0;
    }
}

// 0x4ABC70
bool sub_4ABC70(Ai* ai)
{
    S5FF620 v1;
    S4ABF10 v3;
    AiParams ai_params;

    if (!dword_5B50CC) {
        return false;
    }

    if (random_between(1, 100) > sub_4ABE20(ai)) {
        return false;
    }

    if (!sub_45E3F0(ai->obj, false) == 0) {
        sub_4CCA90(&v1, ai->obj, 1);
        v3.flags = 0x1;
        v3.entries = v1.field_30[1].entries;
        v3.cnt = v1.field_30[1].cnt;
        if (sub_4ABF10(ai, &v3)) {
            sub_4CCBF0(&v1);
            return true;
        }

        sub_4CCBF0(&v1);
    }

    sub_4AAA60(ai->obj, &ai_params);

    if (ai_params.field_34 < random_between(1, 100)) {
        sub_4CCA90(&v1, ai->obj, 2);
        v3.flags = 0x4;
        v3.entries = v1.field_30[2].entries;
        v3.cnt = v1.field_30[2].cnt;
        if (sub_4ABF10(ai, &v3)) {
            sub_4CCBF0(&v1);
            return true;
        }

        sub_4CCBF0(&v1);
    }

    sub_4CCA90(&v1, ai->obj, 3);
    v3.flags = 0x2;
    v3.entries = v1.field_30[3].entries;
    v3.cnt = v1.field_30[3].cnt;
    if (sub_4ABF10(ai, &v3)) {
        sub_4CCBF0(&v1);
        return true;
    }

    sub_4CCBF0(&v1);
    return false;
}

// 0x4ABE20
int sub_4ABE20(Ai* ai)
{
    // TODO: Incomplete.
}

// 0x4ABEB0
bool sub_4ABEB0(int64_t obj, int64_t tgt)
{
    int obj_level;
    int tgt_level;

    tgt_level = stat_level(tgt, STAT_LEVEL);
    obj_level = stat_level(obj, STAT_LEVEL);

    if (tgt_level < 20 && tgt_level >= obj_level + 10) {
        return true;
    } else if (tgt_level < 40 && tgt_level >= obj_level + 5) {
        return true;
    } else if (tgt_level >= obj_level) {
        return true;
    }

    return false;
}

// 0x4ABF10
bool sub_4ABF10(Ai* ai, S4ABF10* a2)
{
    // TODO: Incomplete.
}

// 0x4AC180
void sub_4AC180(Ai* ai)
{
    if (!sub_4B6D70() || sub_4B6D80() == ai->obj) {
        if ((obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS) & ONF_BACKING_OFF) != 0) {
             sub_4AC7B0(ai);
             return;
        }

        switch (ai->field_14) {
        case 1:
            sub_4AC250(ai);
            return;
        case 2:
            sub_4AC320(ai);
            return;
        case 3:
            sub_4AC350(ai);
            return;
        }

        switch (ai->danger_type) {
        case 0:
            sub_4AC380(ai);
            return;
        case 1:
            sub_4AC6E0(ai);
            return;
        case 2:
            sub_4AC620(ai);
            return;
        case 3:
            sub_4AC660(ai);
            return;
        }
    }
}

// 0x4AC250
void sub_4AC250(Ai* ai)
{
    MagicTechSerializedData v1;

    if (ai->field_20 != OBJ_HANDLE_NULL) {
        sub_455A20(&v1, ai->field_20, ai->field_18);
    } else {
        sub_455A20(&v1, ai->obj, ai->field_18);
    }

    sub_4440E0(ai->danger_source, &(v1.field_70));

    if (sub_4564E0(&v1)) {
        sub_455AC0(&v1);

        if (ai->field_20 != OBJ_HANDLE_NULL) {
            sub_4574D0(ai->field_20);

            switch (obj_field_int32_get(ai->field_20, OBJ_F_TYPE)) {
            case OBJ_TYPE_ITEM_FOOD:
            case OBJ_TYPE_ITEM_SCROLL:
                sub_43CCA0(ai->field_20);
                break;
            }
        }
    }
}

// 0x4AC320
void sub_4AC320(Ai* ai)
{
    sub_435450(ai->obj,
        ai->danger_source,
        ai->field_20,
        0);
}

// 0x4AC350
void sub_4AC350(Ai* ai)
{
    sub_4350F0(ai->obj,
        ai->danger_source,
        ai->field_20,
        ai->field_1C,
        0);
}

// 0x4AC380
void sub_4AC380(Ai* ai)
{
    unsigned int npc_flags;
    int v1;
    int v2;
    char str[1000];
    int v3;

    combat_critter_deactivate_combat_mode(ai->obj);

    if (ai->leader_obj != OBJ_HANDLE_NULL) {
        npc_flags = obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS);
        if ((npc_flags & ONF_AI_WAIT_HERE) != 0) {
            return;
        }

        anim_goal_follow_obj(ai->obj, ai->leader_obj);

        if (!sub_423300(ai->obj, NULL)) {
            v1 = sub_4AD950(ai->obj, ai->leader_obj, true);
            if (v1 != 0 && critter_disband(ai->obj, false)) {
                npc_flags = obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS);
                npc_flags |= ONF_JILTED;
                obj_field_int32_set(ai->obj, OBJ_F_NPC_FLAGS, npc_flags);

                if (dword_5F8488 != NULL && sub_45D790(ai->obj)) {
                    sub_414290(ai->obj, ai->leader_obj, v1, str, &v3);
                    dword_5F8488(ai->obj, ai->leader_obj, str, v3);
                }
            } else if ((npc_flags & ONF_CHECK_LEADER) != 0) {
                npc_flags = obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS);
                npc_flags &= ~ONF_CHECK_LEADER;
                obj_field_int32_set(ai->obj, OBJ_F_NPC_FLAGS, npc_flags);

                v2 = sub_4ADB50(ai->obj, ai->leader_obj);
                if (v2 != 0) {
                    if (dword_5F8488 != NULL && sub_45D790(ai->obj)) {
                        sub_414290(ai->obj, ai->leader_obj, v2, str, &v3);
                        dword_5F8488(ai->obj, ai->leader_obj, str, v3);
                    }
                }
            }
        }
    } else if (!sub_4ACBB0(ai->obj, false) && !sub_4ACDB0(ai->obj, false)) {
        sub_435CE0(ai->obj);
        sub_4364D0(ai->obj);
    }

    if (!critter_is_sleeping(ai->obj)) {
        if (sub_460C20() == OBJ_HANDLE_NULL && random_between(1, 100) == 1) {
            sub_4A8F90(ai->obj, 32736);
        }
    }
}

// 0x4AC620
void sub_4AC620(Ai* ai)
{
    sub_4AABE0(ai->obj, 3, ai->danger_source, 0);
    sub_434980(ai->obj, ai->danger_source);
}

// 0x4AC660
void sub_4AC660(Ai* ai)
{
    int64_t fleeing_from_obj;

    obj_field_obj_get(ai->obj, OBJ_F_CRITTER_FLEEING_FROM, &fleeing_from_obj);

    if (fleeing_from_obj == OBJ_HANDLE_NULL
        || sub_45D8D0(fleeing_from_obj)
        || sub_45D800(fleeing_from_obj)) {
        ai->danger_type = sub_4AABE0(ai->obj, 0, OBJ_HANDLE_NULL, 0);
    }
}

// 0x4AC6E0
void sub_4AC6E0(Ai* ai)
{
    AiParams params;
    unsigned int npc_flags;
    int64_t distance;

    sub_4AAA60(ai->obj, &params);
    npc_flags = obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS);
    distance = sub_441AE0(ai->obj, ai->danger_source);
    if (params.field_3C > 1 && distance < params.field_3C && random_between(1, 20) == 1) {
        npc_flags |= ONF_BACKING_OFF;
        npc_flags |= 0x40000000;
        obj_field_int32_set(ai->obj, OBJ_F_NPC_FLAGS, npc_flags);
    } else if (!sub_4AC910(ai, distance)) {
        sub_434B00(ai->obj, ai->danger_source, ai->field_30);
    }
}

// 0x4AC7B0
void sub_4AC7B0(Ai* ai)
{
    AiParams ai_params;
    unsigned int npc_flags;
    int combat_min_distance;
    PathCreateInfo path_create_info;
    int8_t rotations[100];

    sub_4AAA60(ai->obj, &ai_params);

    npc_flags = obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS);
    if ((npc_flags & ONF_BACKING_OFF) != 0) {
            combat_min_distance = ai_params.field_3C;
            if (combat_min_distance <= 1) {
                combat_min_distance = 5;
            }

            if (sub_441AE0(ai->obj, ai->danger_source) < combat_min_distance) {
                path_create_info.obj = ai->obj;
                path_create_info.from = obj_field_int64_get(ai->obj, OBJ_F_LOCATION);
                path_create_info.to = obj_field_int64_get(ai->danger_source, OBJ_F_LOCATION);
                path_create_info.max_rotations = 100;
                path_create_info.rotations = rotations;
                path_create_info.field_20 = 0x800;
                path_create_info.field_24 = combat_min_distance;
                if (sub_41F3C0(&path_create_info)) {
                    sub_433C80(ai->obj, path_create_info.to);
                } else {
                    obj_field_int32_set(ai->obj, OBJ_F_NPC_FLAGS, npc_flags & ~(ONF_BACKING_OFF));
                }
            } else {
                obj_field_int32_set(ai->obj, OBJ_F_NPC_FLAGS, npc_flags & ~(ONF_BACKING_OFF));
            }
    }
}

// 0x4AC910
bool sub_4AC910(Ai* ai, int64_t a2)
{
    unsigned int flags;
    int64_t item_obj;
    int v1;
    int64_t loc;
    int64_t v2;
    ObjectList objects;
    ObjectNode* node;

    flags = obj_field_int32_get(ai->obj, OBJ_F_NPC_FLAGS);
    if ((flags & 0x40000000) == 0) {
        return false;
    }

    if ((flags & 0x10000000) == 0) {
        return false;
    }

    flags &= ~0x40000000;
    obj_field_int32_set(ai->obj, OBJ_F_NPC_FLAGS, flags);

    if (basic_skill_get_base(ai->obj, BASIC_SKILL_THROWING) == 0) {
        return false;
    }

    item_obj = item_find_first_generic(ai->obj, 0x10);
    if (item_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    v1 = sub_466230(item_obj);
    if (a2 <= v1) {
        return false;
    }

    loc = obj_field_int64_get(ai->danger_source, OBJ_F_LOCATION);
    sub_4ADE00(ai->obj, loc, &v2);
    if (v2 != 0) {
        return false;
    }

    sub_4AE4E0(ai->danger_source, v1, &objects, OBJ_TM_NPC);
    node = objects.head;
    while (node != NULL) {
        if (!sub_45D8D0(node->obj)
            && sub_4AE3A0(ai->obj, node->obj)) {
            break;
        }
        node = node->next;
    }
    object_list_destroy(&objects);

    if (node != NULL) {
        return false;
    }

    loc = obj_field_int64_get(ai->danger_source, OBJ_F_LOCATION);
    sub_434F80(ai->obj, item_obj, loc);

    return true;
}

// 0x4ACBB0
bool sub_4ACBB0(int64_t obj, bool a2)
{
    // TODO: Incomplete.
}

// 0x4ACD90
bool ai_is_day()
{
    int hour;

    hour = datetime_current_hour();
    return hour >= 6 && hour < 21;
}

// 0x4ACDB0
bool sub_4ACDB0(int64_t obj, bool a2)
{
    // TODO: Incomplete.
}

// 0x4AD060
bool ai_get_standpoint(int64_t obj, int64_t* standpoint_ptr)
{
    if (critter_leader_get(obj) != OBJ_HANDLE_NULL) {
        return false;
    }

    if (ai_is_day()) {
        *standpoint_ptr = obj_field_int64_get(obj, OBJ_F_NPC_STANDPOINT_DAY);
    } else {
        *standpoint_ptr = obj_field_int64_get(obj, OBJ_F_NPC_STANDPOINT_NIGHT);
    }

    return true;
}

// 0x4AD0B0
void sub_4AD0B0(int64_t npc_obj)
{
    int64_t loc;
    int64_t bed_obj;

    loc = obj_field_int64_get(npc_obj, OBJ_F_NPC_STANDPOINT_NIGHT);
    bed_obj = ai_find_nearest_bed(loc);
    if (bed_obj != OBJ_HANDLE_NULL
        && obj_field_handle_get(bed_obj, OBJ_F_SCENERY_WHOS_IN_ME) == npc_obj) {
        critter_leave_bed(npc_obj, bed_obj);
        return;
    }

    loc = obj_field_int64_get(npc_obj, OBJ_F_NPC_STANDPOINT_DAY);
    bed_obj = ai_find_nearest_bed(loc);
    if (bed_obj != OBJ_HANDLE_NULL
        && obj_field_handle_get(bed_obj, OBJ_F_SCENERY_WHOS_IN_ME) == npc_obj) {
        critter_leave_bed(npc_obj, bed_obj);
        return;
    }
}

// 0x4AD140
int64_t ai_find_nearest_bed(int64_t loc)
{
    int64_t bed_obj = OBJ_HANDLE_NULL;
    ObjectList objects;
    ObjectNode* node;
    tig_art_id_t art_id;

    sub_4407C0(loc, OBJ_TM_SCENERY, &objects);

    node = objects.head;
    while (node != NULL) {
        art_id = obj_field_int32_get(node->obj, OBJ_F_CURRENT_AID);
        if (tig_art_scenery_id_type_get(art_id) == TIG_ART_SCENERY_TYPE_BEDS) {
            bed_obj = node->obj;
            break;
        }

        node = node->next;
    }

    object_list_destroy(&objects);

    return bed_obj;
}

// 0x4AD1B0
void sub_4AD1B0(int64_t a1, int64_t a2, int a3)
{
    if (a3 != 0) {
        sub_4341C0(a1, a2, a3);
    } else {
        sub_433640(a1, a2);
    }
}

// 0x4AD200
bool ai_timeevent_process(TimeEvent* timeevent)
{
    // TODO: Incomplete.
}

// 0x4AD420
bool sub_4AD420(int64_t obj)
{
    int64_t pc_obj;

    if (sub_45D8D0(obj)) {
        return false;
    }

    if (sub_4B6D70()) {
        return false;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pc_obj = multiplayer_find_first_player_obj();
        while (pc_obj != OBJ_HANDLE_NULL) {
            if (sub_441AE0(pc_obj, obj) <= 30) {
                return true;
            }
            pc_obj = multiplayer_find_next_player_obj();
        }

        return false;
    }

    if (sub_441AE0(player_get_pc_obj(), obj) <= 30) {
        return true;
    }

    return false;
}

// 0x4AD4D0
bool sub_4AD4D0(int64_t obj)
{
    int64_t pc_leader_obj;
    int64_t v1;

    if (sub_45D8D0(obj)) {
        return false;
    }

    if (sub_4B6D70()) {
        return false;
    }

    pc_leader_obj = sub_45DDA0(obj);
    if (pc_leader_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (sub_441AE0(pc_leader_obj, obj) <= 30) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) & ONF_AI_WAIT_HERE) != 0) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_PARALYZED) != 0) {
        return false;
    }

    if (!sub_45D790(obj)) {
        return false;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        v1 = sub_4C1110(obj);
        if (v1 != OBJ_HANDLE_NULL) {
            sub_460A20(v1, 0);
        }
    }

    sub_424070(obj, 4, false, true);
    sub_43E770(obj, obj_field_int64_get(pc_leader_obj, OBJ_F_LOCATION), 0, 0);

    return true;
}

// 0x4AD5D0
int sub_4AD5D0(int64_t obj)
{
    return 4750 * sub_4AD610(obj) / 30 + 250;
}

// 0x4AD610
int sub_4AD610(int64_t obj)
{
    int64_t pc_obj;
    int64_t distance;
    int64_t max_distance;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        max_distance = -1;
        pc_obj = multiplayer_find_first_player_obj();
        while (pc_obj != OBJ_HANDLE_NULL) {
            distance = sub_441AE0(pc_obj, obj);
            if (max_distance == -1 || distance < max_distance) {
                max_distance = distance;
            }
            pc_obj = multiplayer_find_next_player_obj();
        }
    } else {
        max_distance = sub_441AE0(player_get_pc_obj(), obj);
    }

    return max_distance >= 0 && max_distance <= 30 ? (int)max_distance : 30;
}

// 0x4AD6B0
bool sub_4AD6B0(TimeEvent* timeevent)
{
    return timeevent->params[0].object_value == qword_5F84A0;
}

// 0x4AD6E0
void sub_4AD6E0(int64_t obj)
{
    sub_4AD700(obj, sub_4AD5D0(obj));
}

// 0x4AD700
void sub_4AD700(int64_t obj, int millis)
{
    DateTime datetime;

    sub_45A950(&datetime, millis);
    sub_4AD730(obj, &datetime);
}

// 0x4AD730
void sub_4AD730(int64_t obj, DateTime* datetime)
{
    TimeEvent timeevent;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        sub_4AD7D0(obj);

        timeevent.type = TIMEEVENT_TYPE_AI;
        timeevent.params[0].object_value = obj;
        timeevent.params[1].integer_value = 0;
        sub_45B800(&timeevent, datetime);
    }
}

// 0x4AD790
void sub_4AD790(int64_t obj, int a2)
{
    TimeEvent timeevent;

    sub_4AD7D0(obj);

    timeevent.type = TIMEEVENT_TYPE_AI;
    timeevent.params[0].object_value = obj;
    timeevent.params[1].integer_value = a2;
    sub_45B820(&timeevent);
}

// 0x4AD7D0
void sub_4AD7D0(int64_t obj)
{
    qword_5F84A0 = obj;
    timeevent_clear_one_ex(TIMEEVENT_TYPE_AI, sub_4AD6B0);
}

// 0x4AD800
int sub_4AD800(int64_t npc_obj, int64_t pc_obj, bool a3)
{
    unsigned int critter_flags;
    unsigned int spell_flags;
    int danger_type;
    int64_t reaction_pc_obj;

    if ((obj_field_int32_get(npc_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return 5;
    }

    if (obj_field_int32_get(npc_obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        critter_flags = obj_field_int32_get(npc_obj, OBJ_F_CRITTER_FLAGS);
        if ((critter_flags & (OCF_MUTE | OCF_PARALYZED | OCF_STUNNED)) != 0) {
            return 5;
        }
        if ((critter_flags & OCF_SLEEPING) != 0) {
            return 6;
        }

        if (sub_459380(npc_obj, 58)) {
            return 5;
        }

        spell_flags = obj_field_int32_get(npc_obj, OBJ_F_SPELL_FLAGS);
        if ((spell_flags & OSF_STONED) != 0) {
            return 5;
        }

        if (sub_45D8D0(npc_obj)) {
            return (spell_flags & OSF_SPOKEN_WITH_DEAD) == 0 ? 1 : 0;
        }

        if (sub_45D800(npc_obj)) {
            return 6;
        }

        if (!a3) {
            ai_danger_source(npc_obj, &danger_type, NULL);

            if (danger_type == 1 || danger_type == 2) {
                return 2;
            }

            if (!sub_424070(npc_obj, 2, false, false)) {
                return 3;
            }
        }

        reaction_pc_obj = sub_4C1110(npc_obj);
        if (reaction_pc_obj != OBJ_HANDLE_NULL
            && reaction_pc_obj != pc_obj) {
            return 4;
        }
    }

    return 0;
}

// 0x4AD950
int sub_4AD950(int64_t npc_obj, int64_t pc_obj, bool a3)
{
    int64_t mind_controlled_by_obj;
    int64_t leader_obj;
    AiParams params;
    int npc_alignment;
    int pc_alignment;
    int pc_max_followers;

    if ((obj_field_int32_get(npc_obj, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) != 0) {
        if (sub_459040(npc_obj, OSF_MIND_CONTROLLED, &mind_controlled_by_obj)) {
            if (mind_controlled_by_obj == pc_obj) {
                return 0;
            }
        } else {
            if (sub_45DDA0(npc_obj)) {
                return 0;
            }
        }
    }

    if (basic_skill_get_training(pc_obj, BASIC_SKILL_PERSUATION) >= TRAINING_MASTER) {
        return 0;
    }

    leader_obj = critter_leader_get(npc_obj);
    if (leader_obj != OBJ_HANDLE_NULL && leader_obj != pc_obj) {
        if (stat_level(pc_obj, STAT_CHARISMA) <= stat_level(leader_obj, STAT_CHARISMA)) {
            return 4;
        }
    }

    if ((obj_field_int32_get(npc_obj, OBJ_F_NPC_FLAGS) & ONF_FORCED_FOLLOWER) != 0) {
        return 0;
    }

    sub_4AAA60(npc_obj, &params);

    if (sub_4C0CC0(npc_obj, pc_obj) <= params.field_14) {
        return 3;
    }

    npc_alignment = stat_level(npc_obj, STAT_ALIGNMENT);
    pc_alignment = stat_level(pc_obj, STAT_ALIGNMENT);
    if (pc_alignment > npc_alignment) {
        if (pc_alignment - npc_alignment > params.field_18) {
            return 1;
        }
    } else {
        if (npc_alignment - pc_alignment > params.field_1C) {
            return 2;
        }
    }

    if (stat_level(npc_obj, STAT_LEVEL) >= stat_level(pc_obj, STAT_LEVEL) + params.field_20) {
        return 7;
    }

    if (!a3) {
        pc_max_followers = stat_level(pc_obj, STAT_MAX_FOLLOWERS);
        if (pc_max_followers == 0) {
            return 6;
        }
        if (sub_45E3F0(pc_obj, true) >= pc_max_followers) {
            return 5;
        }
    }

    return 0;
}

// 0x4ADB50
int sub_4ADB50(int64_t npc_obj, int64_t pc_obj)
{
    unsigned int critter_flags2;
    int64_t mind_controlled_by_obj;
    AiParams params;
    int npc_alignment;
    int pc_alignment;

    critter_flags2 = obj_field_int32_get(npc_obj, OBJ_F_CRITTER_FLAGS2);
    obj_field_int32_set(npc_obj, OBJ_F_CRITTER_FLAGS2, critter_flags2 & ~(OCF2_CHECK_ALIGN_BAD | OCF2_CHECK_ALIGN_GOOD | OCF2_CHECK_REACTION_BAD));

    if ((obj_field_int32_get(npc_obj, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) != 0) {
        if (sub_459040(npc_obj, OSF_MIND_CONTROLLED, &mind_controlled_by_obj)) {
            if (mind_controlled_by_obj == pc_obj) {
                return 0;
            }
        } else {
            if (sub_45DDA0(npc_obj)) {
                return 0;
            }
        }
    }

    if (basic_skill_get_training(pc_obj, BASIC_SKILL_PERSUATION) >= TRAINING_MASTER) {
        return 0;
    }

    if ((obj_field_int32_get(npc_obj, OBJ_F_NPC_FLAGS) & ONF_FORCED_FOLLOWER) != 0) {
        return 0;
    }

    sub_4AAA60(npc_obj, &params);

    if ((critter_flags2 & OCF2_CHECK_REACTION_BAD) != 0
        && sub_4C0CC0(npc_obj, pc_obj) <= params.field_14 + 20) {
        return 3;
    }

    npc_alignment = stat_level(npc_obj, STAT_ALIGNMENT);
    pc_alignment = stat_level(pc_obj, STAT_ALIGNMENT);
    if (pc_alignment > npc_alignment) {
        if ((critter_flags2 & OCF2_CHECK_ALIGN_GOOD) != 0
            && pc_alignment - npc_alignment <= params.field_18 - 100) {
            return 1;
        }
    } else {
        if ((critter_flags2 & OCF2_CHECK_ALIGN_BAD) != 0
            && npc_alignment - pc_alignment <= params.field_1C - 100) {
            return 2;
        }
    }

    return 0;
}

// 0x4ADCC0
int sub_4ADCC0(int64_t a1, int64_t a2, int64_t a3)
{
    int64_t mind_controlled_by_obj;
    AiParams params;

    if ((obj_field_int32_get(a1, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) != 0) {
        if (sub_459040(a1, OSF_MIND_CONTROLLED, &mind_controlled_by_obj)) {
            if (mind_controlled_by_obj == a1) {
                return 0;
            }
        } else {
            if (sub_45DDA0(a1)) {
                return 0;
            }
        }
    }

    if (!obj_type_is_critter(obj_field_int32_get(a2, OBJ_F_TYPE))) {
        return 0;
    }

    if (critter_leader_get(a2) == a3) {
        return 4;
    }

    if (a3 != OBJ_HANDLE_NULL) {
        if (stat_level(a1, STAT_ALIGNMENT) > 0) {
            sub_4AAA60(a1, &params);
            if (stat_level(a2, STAT_ALIGNMENT) >= params.field_30) {
                return 1;
            }
        }
    }

    if (critter_origin_same(a1, a2)) {
        return 2;
    }

    if (critter_faction_same(a1, a2)) {
        return 3;
    }

    return 0;
}

// 0x4ADE00
int sub_4ADE00(int64_t a1, int64_t a2, int64_t* a3)
{
    // TODO: Incomplete.
}

// 0x4ADFF0
void sub_4ADFF0(int64_t obj)
{
    unsigned int flags;

    flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
    flags |= ONF_CHECK_WEAPON;
    flags |= ONF_LOOK_FOR_WEAPON;
    flags |= ONF_LOOK_FOR_AMMO;
    obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, flags);
}

// 0x4AE020
void sub_4AE020(int64_t obj, int* cnt_ptr, int* lvl_ptr)
{
    int64_t leader_obj;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        sub_4AE0A0(obj, cnt_ptr, lvl_ptr);
        return;
    }

    leader_obj = critter_leader_get(obj);
    if (leader_obj != OBJ_HANDLE_NULL) {
        sub_4AE0A0(leader_obj, cnt_ptr, lvl_ptr);
        return;
    }

    *cnt_ptr = 1;
    *lvl_ptr = stat_level(obj, STAT_LEVEL);
}

// 0x4AE0A0
void sub_4AE0A0(int64_t obj, int* cnt_ptr, int* lvl_ptr)
{
    ObjectList objects;
    ObjectNode* node;

    *cnt_ptr = 1;
    *lvl_ptr = stat_level(obj, STAT_LEVEL);

    sub_441260(obj, &objects);
    node = objects.head;
    while (node != NULL) {
        *cnt_ptr += 1;
        *lvl_ptr += stat_level(obj, STAT_LEVEL);
        node = node->next;
    }

    object_list_destroy(&objects);
}

// 0x4AE120
int sub_4AE120(int64_t a1, int64_t a2)
{
    int64_t pc_leader_obj;
    int obj_type;
    int danger_source_type;
    int64_t danger_source_obj;
    unsigned int npc_flags;
    unsigned int critter_flags;
    AiParams ai_params;

    if (sub_4AB990(a1, a2)) {
        pc_leader_obj = sub_45DDA0(a1);
        obj_type = obj_field_int32_get(a2, OBJ_F_TYPE);

        if (sub_441980(a2, a1, OBJ_HANDLE_NULL, SAP_WILL_KOS, 0) != 0) {
            ai_danger_source(a1, &danger_source_type, &danger_source_obj);

            if (danger_source_type != 3 || danger_source_obj != a2) {
                if (pc_leader_obj == OBJ_HANDLE_NULL) {
                    npc_flags = obj_field_int32_get(a1, OBJ_F_NPC_FLAGS);
                    if ((npc_flags & ONF_KOS) != 0) {
                        critter_flags = obj_field_int32_get(a1, OBJ_F_CRITTER_FLAGS);
                        if (((obj_field_int32_get(a2, OBJ_F_SPELL_FLAGS) & OSF_ENSHROUDED) != 0
                                && (critter_flags & OCF_UNDEAD) != 0)
                            || (obj_field_int32_get(a2, OBJ_F_CRITTER_FLAGS) & OSF_FAMILIAR) != 0
                                && (critter_flags & OCF_ANIMAL) != 0) {
                            return 0;
                        }

                        if ((npc_flags & ONF_KOS_OVERRIDE) == 0
                            && !critter_faction_same(a1, a2)) {
                            return 1;
                        }
                    }

                    sub_4AAA60(a1, &ai_params);

                    if (obj_type == OBJ_TYPE_PC
                        && sub_4C0CE0(a1, a2) <= ai_params.field_28) {
                        return 2;
                    }

                    if (abs(stat_level(a1, STAT_ALIGNMENT) - stat_level(a2, STAT_ALIGNMENT)) >= ai_params.field_2C) {
                        return 3;
                    }

                    if (sub_4AF800(a1, a2)) {
                        return 5;
                    }
                }

                if (obj_type == OBJ_TYPE_NPC) {
                    ai_danger_source(a2, &danger_source_type, &danger_source_obj);

                    if (danger_source_type == 1
                        && danger_source_obj != OBJ_HANDLE_NULL
                        && (sub_4AE3A0(a1, danger_source_obj)
                            || (critter_social_class_get(a1) == SOCIAL_CLASS_GUARD)
                                && sub_45F730(a2)
                                && critter_leader_get(a2) == OBJ_HANDLE_NULL)
                        && !sub_4ADCC0(a1, a2, critter_leader_get(a1))) {
                        return 4;
                    }
                }
            }
        }
    }

    return 0;
}

// 0x4AE3A0
int sub_4AE3A0(int64_t a1, int64_t a2)
{
    int64_t leader_obj;

    if (a1 == a2) {
        return 4;
    }

    leader_obj = critter_leader_get(a1);
    if (leader_obj != OBJ_HANDLE_NULL && leader_obj == a2) {
        return 3;
    }

    if ((obj_field_int32_get(a1, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) == 0) {
        if (critter_faction_same(a1, a2)) {
            return 1;
        }

        if (critter_social_class_get(a1) == SOCIAL_CLASS_GUARD
            && critter_origin_same(a1, a2)) {
            return 2;
        }
    }

    return 0;
}

// 0x4AE450
int64_t sub_4AE450(int64_t a1, int64_t a2)
{
    int64_t combat_focus_obj;

    if (sub_45D8D0(a2)
        && obj_field_int32_get(a2, OBJ_F_TYPE) == OBJ_TYPE_NPC
        && obj_field_obj_get(a2, OBJ_F_NPC_COMBAT_FOCUS, &combat_focus_obj)
        && combat_focus_obj != OBJ_HANDLE_NULL
        && sub_4AB990(a1, combat_focus_obj)
        && sub_4AE3A0(a1, a2)) {
        return combat_focus_obj;
    }

    return OBJ_HANDLE_NULL;
}

// 0x4AE4E0
void sub_4AE4E0(int64_t obj, int radius, ObjectList* objects, unsigned int flags)
{
    int64_t loc;
    LocRect loc_rect;

    loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
    loc_rect.x1 = loc - radius;
    loc_rect.y1 = loc - radius;
    loc_rect.x2 = loc + radius;
    loc_rect.y2 = loc + radius;
    sub_440B40(&loc_rect, flags, objects);
}

// 0x4AE570
int sub_4AE570(int64_t a1, int64_t a2, int64_t a3, int skill)
{
    switch (skill) {
    case SKILL_HEAL:
        if (a3 == OBJ_HANDLE_NULL
            || obj_field_int32_get(a3, OBJ_F_TYPE) != OBJ_TYPE_ITEM_GENERIC
            || (obj_field_int32_get(a3, OBJ_F_GENERIC_FLAGS) & 0x8) == 0) {
            return 1;
        }

        if (a2 == OBJ_HANDLE_NULL
            || !obj_type_is_critter(obj_field_int32_get(a2, OBJ_F_TYPE))
            || (obj_field_int32_get(a2, OBJ_F_CRITTER_FLAGS) & (OCF_UNDEAD | OCF_MECHANICAL)) != 0) {
            return 2;
        }

        return 0;
    case SKILL_REPAIR:
        if (a2 == OBJ_HANDLE_NULL
            || !obj_type_is_item(obj_field_int32_get(a2, OBJ_F_TYPE))) {
            return 2;
        }

        return 0;
    case SKILL_DISARM_TRAPS:
        if (a3 != OBJ_HANDLE_NULL) {
            if (obj_field_int32_get(a3, OBJ_F_TYPE) != OBJ_TYPE_ITEM_GENERIC
                || (obj_field_int32_get(a3, OBJ_F_GENERIC_FLAGS) & OGF_IS_LOCKPICK) == 0) {
                return 1;
            }
        } else {
            if (a1 == OBJ_HANDLE_NULL || !sub_45DDA0(a1)) {
                return 1;
            }
        }

        if (a2 == OBJ_HANDLE_NULL
            || !object_is_lockable(a2)) {
            return 2;
        }

        return 0;
    }

    return 0;
}

// 0x4AE720
int sub_4AE720(int64_t a1, int64_t item_obj, int64_t a3, int magictech)
{
    int obj_type;
    int64_t v1;

    obj_type = obj_field_int32_get(a1, OBJ_F_TYPE);

    if (item_obj != OBJ_HANDLE_NULL) {
        if (obj_type == OBJ_TYPE_NPC
            && sub_4503A0(magictech)
            && !sub_450B40(a1)) {
            return 6;
        }

        if (obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_MANA_STORE) == 0) {
            return 2;
        }

        if (a3 != OBJ_HANDLE_NULL
            && (COLLEGE_FROM_SPELL(magictech) != COLLEGE_NECROMANTIC_WHITE
                || (sub_4AB400(a3) > 30
                    && sub_4AB430(a1) < 80))
            && sub_453CC0(a1, item_obj, a3) >= 38) {
            return 8;
        }
    } else {
        if (!sub_4B1950(a1, magictech)) {
            return 1;
        }

        if (obj_type == OBJ_TYPE_NPC) {
            if (!sub_45DDA0(a1) && sub_4AB430(a1) < 20) {
                return 2;
            }

            if (sub_4503A0(magictech) && !sub_450B40(a1)) {
                return 6;
            }
        }

        if (sub_4B1660(magictech, a1) >= sub_45D700(a1)) {
            return 2;
        }

        if (sub_4B1750(magictech) > stat_level(a1, STAT_INTELLIGENCE)) {
            return 3;
        }

        if (spell_get_iq(magictech) > stat_level(a1, STAT_WILLPOWER)) {
            return 7;
        }

        if ((obj_field_int32_get(a1, OBJ_F_NPC_FLAGS) & ONF_CAST_HIGHEST) != 0
            && sub_4B1AB0(a1, COLLEGE_FROM_SPELL(magictech)) + 5 * COLLEGE_FROM_SPELL(magictech) - 1 != magictech) {
            return 4;
        }

        if (a3 != OBJ_HANDLE_NULL
            && (COLLEGE_FROM_SPELL(magictech) != COLLEGE_NECROMANTIC_WHITE
                || sub_4AB400(a3) > 30)
            && sub_453B20(a1, a3, magictech) >= 38) {
            return 8;
        }
    }

    if (a1 != a3 && a3 != OBJ_HANDLE_NULL) {
        if (sub_4ADE00(a1, obj_field_int64_get(a3, OBJ_F_LOCATION), &v1) >= 100
            || v1 == OBJ_HANDLE_NULL
            || v1 == a3) {
            return 5;
        }
    }

    return 0;
}

// 0x4AE9E0
void sub_4AE9E0(int64_t a1, bool a2)
{
    int cnt;
    int rnd;
    int64_t follower_obj;
    char str[1000];
    int v1;

    if (((tig_net_flags & TIG_NET_CONNECTED) == 0
            || (tig_net_flags & TIG_NET_HOST) != 0)
        && random_between(1, 2) != 1) {
        if (dword_5F8488 != NULL) {
            cnt = sub_45E3F0(a1, false);
            if (cnt != 0) {
                rnd = cnt > 1 ? random_between(0, cnt - 1) : 0;
                follower_obj = obj_arrayfield_handle_get(a1, OBJ_F_CRITTER_FOLLOWER_IDX, rnd);
                if (sub_45D790(follower_obj)) {
                    sub_413F50(follower_obj, a1, a2, str, &v1);
                    dword_5F8488(follower_obj, a1, str, v1);
                }
            }
        }
    }
}

// 0x4AEAB0
void sub_4AEAB0(int64_t a1, int64_t a2)
{
    char str[1000];
    int v1;

    sub_414490(a1, a2, str, &v1);
    dword_5F8488(a1, a2, str, v1);
}

// 0x4AEB10
bool ai_critter_can_open_portals(int64_t obj)
{
    AiParams params;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    switch (obj_field_int32_get(obj, OBJ_F_TYPE)) {
    case OBJ_TYPE_PC:
        return true;
    case OBJ_TYPE_NPC:
        sub_4AAA60(obj, &params);
        return params.field_40;
    }

    return false;
}

// 0x4AEB70
int sub_4AEB70(int64_t obj, int64_t portal, int a3)
{
    int type;
    unsigned int flags;

    if (object_is_destroyed(portal)) {
        return 0;
    }

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (type != OBJ_TYPE_PC && type != OBJ_TYPE_NPC) {
        return 5;
    }

    if (!sub_441980(obj, portal, OBJ_HANDLE_NULL, 3, 0)) {
        return 4;
    }

    if (obj_field_int32_get(portal, OBJ_F_TYPE) != OBJ_TYPE_PORTAL) {
        return 0;
    }

    flags = obj_field_int32_get(portal, OBJ_F_PORTAL_FLAGS);
    if ((flags & OPF_JAMMED) != 0) {
        return 2;
    }
    if ((flags & OPF_MAGICALLY_HELD) != 0) {
        return 3;
    }
    if ((flags & OPF_ALWAYS_LOCKED) == 0) {
        if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
            if (!sub_45DDA0(obj) || sub_4AECA0(portal, a3)) {
                return 0;
            }
        } else if (sub_4AECA0(portal, a3)) {
            return 0;
        }
    }

    if (object_is_locked(portal)) {
        // TODO: Review (convert from bool to int).
        return !sub_463370(obj, obj_field_int32_get(portal, OBJ_F_PORTAL_KEY_ID));
    }

    return 0;
}

// 0x4AECA0
bool sub_4AECA0(int64_t obj, int a2)
{
    int64_t loc;
    tig_art_id_t aid;
    int rotation;
    int64_t new_loc;
    int64_t tmp_loc;

    loc = obj_field_int32_get(obj, OBJ_F_LOCATION);
    aid = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    rotation = tig_art_id_rotation_get(aid);
    if (!sub_4B8FF0(loc, rotation, &new_loc)) {
        return false;
    }

    if ((a2 + 4) % 8 == rotation
        || (a2 + 5) % 8 == rotation
        || (a2 + 3) % 8 == rotation) {
        tmp_loc = new_loc;
        loc = new_loc;
        new_loc = tmp_loc;
    }

    aid = sub_4D70B0(loc);
    if (tig_art_tile_id_type_get(aid) != 0) {
        return false;
    }

    aid = sub_4D70B0(new_loc);
    if (tig_art_tile_id_type_get(aid) == 0) {
        return false;
    }

    return true;
}

// 0x4AED80
int sub_4AED80(int64_t a1, int64_t a2)
{
    unsigned int container_flags;
    int key_id;

    if (sub_49B290(a2) == 3023 || object_is_destroyed(a2)) {
        return 0;
    }

    if (!sub_441980(a1, a2, OBJ_HANDLE_NULL, SAP_UNLOCK, 0)) {
        return 4;
    }

    if (obj_field_int32_get(a2, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER) {
        container_flags = obj_field_int32_get(a2, OBJ_F_CONTAINER_FLAGS);
        if ((container_flags & OCOF_JAMMED) != 0) {
            return 2;
        }
        if ((container_flags & OCOF_MAGICALLY_HELD) != 0) {
            return 3;
        }

        if (object_is_locked(a2)) {
            key_id = obj_field_int32_get(a2, OBJ_F_CONTAINER_KEY_ID);
            if (!sub_463370(a1, key_id)) {
                return 1;
            }
        }
    }

    return 0;
}

// 0x4AEE50
void sub_4AEE50(int64_t critter_obj, int64_t target_obj, int a3, int a4)
{
    int64_t pc_obj;
    int notify_npc;
    ObjectList objects;
    ObjectNode* node;
    AiParams ai_params;
    char str[1000];
    int v2;

    if (critter_obj == OBJ_HANDLE_NULL) {
        return;
    }

    switch (obj_field_int32_get(critter_obj, OBJ_F_TYPE)) {
    case OBJ_TYPE_PC:
        pc_obj = critter_obj;
        break;
    case OBJ_TYPE_NPC:
        pc_obj = sub_45DDA0(critter_obj);
        break;
    default:
        pc_obj = OBJ_HANDLE_NULL;
        break;
    }

    if (pc_obj == OBJ_HANDLE_NULL) {
        return;
    }

    switch (obj_field_int32_get(target_obj, OBJ_F_TYPE)) {
    case OBJ_TYPE_PORTAL:
        notify_npc = obj_field_int32_get(target_obj, OBJ_F_PORTAL_NOTIFY_NPC);
        break;
    case OBJ_TYPE_CONTAINER:
        notify_npc = obj_field_int32_get(target_obj, OBJ_F_CONTAINER_NOTIFY_NPC);
        break;
    case OBJ_TYPE_SCENERY:
        notify_npc = -1;
        break;
    default:
        notify_npc = 0;
        break;
    }

    if (notify_npc == 0) {
        return;
    }

    sub_4AE4E0(target_obj, 10, &objects, OBJ_TM_NPC);
    node = objects.head;
    while (node != NULL) {
        if ((obj_field_int32_get(node->obj, OBJ_F_NAME) == notify_npc
                || critter_social_class_get(node->obj) == SOCIAL_CLASS_GUARD)
            && !sub_45D8D0(node->obj)
            && (obj_field_int32_get(node->obj, OBJ_F_SPELL_FLAGS) & OSF_MIND_CONTROLLED) == 0
            && (!sub_4AF260(node->obj, critter_obj)
                || !sub_4AF470(node->obj, critter_obj, a4))) {
            if (sub_441980(critter_obj, node->obj, target_obj, SAP_CATCHING_THIEF_PC, 0) == 1) {
                if (a3 && !critter_is_sleeping(node->obj)) {
                    sub_4C0DE0(node->obj, pc_obj, -20);
                    sub_4AAA60(node->obj, &ai_params);
                    if (sub_4C0CC0(node->obj, pc_obj) <= ai_params.field_28) {
                        sub_4A9650(critter_obj, node->obj, a4, 0);
                    }
                    if (sub_45D790(node->obj)) {
                        if (dword_5F8488 != NULL && sub_45D790(node->obj)) {
                            sub_414130(node->obj, critter_obj, str, &v2);
                            dword_5F8488(node->obj, critter_obj, str, v2);
                        }
                    }
                } else {
                    sub_4A9650(critter_obj, node->obj, a4, 0);
                }
            }
        }
        node = node->next;
    }
    object_list_destroy(&objects);
}

// 0x4AF130
void sub_4AF130(int64_t a1, int64_t a2)
{
    if (obj_field_int32_get(a1, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        sub_4AABE0(a1, 2, a2, 0);
    }
}

// 0x4AF170
void sub_4AF170(int64_t obj)
{
    int danger_type;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        ai_danger_source(obj, &danger_type, NULL);
        if (danger_type == 2) {
            sub_4AABE0(obj, 0, OBJ_HANDLE_NULL, 0);
        }
        sub_44E4D0(obj, 25, -1);
    }
}

// 0x4AF1D0
void sub_4AF1D0(int64_t obj)
{
    unsigned int flags;

    sub_4AF170(obj);

    flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS);
    flags |= OCF_NO_FLEE;
    obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS, flags);
}

// 0x4AF210
bool sub_4AF210(int64_t obj, int64_t* danger_source_ptr)
{
    int danger_type;

    ai_danger_source(obj, &danger_type, danger_source_ptr);

    return danger_type == 3;
}

// 0x4AF240
int sub_4AF240(int value)
{
    if (value < 1) {
        return 1;
    }
    if (value > 10) {
        return 10;
    }
    return value;
}

// 0x4AF260
int sub_4AF260(int64_t a1, int64_t a2)
{
    int perception;
    Tanya v1;
    int64_t v2;
    int v3;
    int64_t v4;

    if ((obj_field_int32_get(a1, OBJ_F_CRITTER_FLAGS) & (OCF_SLEEPING | OCF_BLINDED | OCF_STUNNED)) != 0) {
        return 1000;
    }

    if (sub_45D800(a1)) {
        return 1000;
    }

    if (((obj_field_int32_get(a2, OBJ_F_FLAGS) & OF_INVISIBLE) != 0
            || (obj_field_int32_get(a2, OBJ_F_SPELL_FLAGS) & OSF_INVISIBLE) != 0)
        && !stat_is_maximized(a1, STAT_PERCEPTION)
        && (obj_field_int32_get(a1, OBJ_F_SPELL_FLAGS) & OSF_DETECTING_INVISIBLE) == 0) {
        return 1000;
    }

    if (!sub_45EFF0(a1, a2)) {
        return 1000;
    }

    perception = stat_level(a1, STAT_PERCEPTION);

    if (critter_is_concealed(a2)) {
        int prowling;
        int diff;

        prowling = sub_4C62E0(a2, BASIC_SKILL_PROWLING, a1);
        sub_4C7090(&v1);
        sub_4440E0(a2, &(v1.field_0));
        sub_4440E0(a1, &(v1.field_30));
        v1.field_98 |= 0x4000;
        v1.field_9C = BASIC_SKILL_PROWLING;

        diff = prowling - sub_4C8430(&v1);
        if (diff < 0) {
            diff = 0;
        } else if (diff > 100) {
            diff = 100;
        }

        perception += perception * diff / -100;
    }

    perception = sub_4AF240(perception);

    v2 = sub_441AE0(a1, a2);
    if (v2 > 1000) {
        return 1000;
    }

    v3 = (int)v2 - perception;
    if (v3 < 0) {
        v3 = 0;
    }

    sub_4ADE00(a1, obj_field_int64_get(a2, OBJ_F_LOCATION), &v4);
    if (v4 != 0) {
        v3++;
    }

    return v3;
}

// 0x4AF470
int sub_4AF470(int64_t a1, int64_t a2, int a3)
{
    unsigned int critter_flags;
    int64_t distance;
    int perception;
    Tanya v1;
    int v2;

    critter_flags = obj_field_int32_get(a1, OBJ_F_CRITTER_FLAGS);
    if ((critter_flags & OCF_STUNNED) != 0) {
        return 1000;
    }

    if (sub_45D800(a1)) {
        return 1000;
    }

    distance = sub_441AE0(a1, a2);
    if (distance > 1000) {
        return 1000;
    }

    perception = stat_level(a1, STAT_PERCEPTION);
    if ((critter_flags & OCF_SLEEPING) != 0) {
        perception /= 2;
    }

    if ((obj_field_int32_get(a2, OBJ_F_CRITTER_FLAGS) & OCF_MOVING_SILENTLY) != 0) {
        int prowling;
        int diff;

        prowling = sub_4C62E0(a2, BASIC_SKILL_PROWLING, a1);
        sub_4C7090(&v1);
        sub_4440E0(a2, &(v1.field_0));
        sub_4440E0(a1, &(v1.field_30));
        v1.field_98 |= 0x2000;
        v1.field_9C = BASIC_SKILL_PROWLING;

        diff = prowling - sub_4C8430(&v1);
        if (diff < 0) {
            diff = 0;
        } else if (diff > 100) {
            diff = 100;
        }

        perception += perception * diff / -100;
    }

    v2 = (dword_5B50C0[a3] - dword_5B50C0[0] + sub_4AF240(perception - 4)) / 2 - sub_4AF640(a1, a2);
    if ((int)distance > v2) {
        return (int)distance - v2;
    }

    return 0;
}

// 0x4AF640
int sub_4AF640(int64_t a1, int64_t a2)
{
    int cnt = 0;
    int64_t loc1;
    int64_t loc2;
    int v1;
    int8_t offsets[200];
    unsigned int flags;
    int offset_x;
    int offset_y;
    int64_t loc_x;
    int64_t loc_y;
    int idx;
    int64_t new_loc;
    int rotation;
    int64_t v2;
    int v3;

    loc1 = obj_field_int64_get(a1, OBJ_F_LOCATION);
    loc2 = obj_field_int64_get(a2, OBJ_F_LOCATION);
    if (loc1 == loc2) {
        return 0;
    }

    v1 = sub_4201C0(loc1, loc2, offsets);
    if (v1 == 0) {
        return 100;
    }

    flags = 0x20;

    offset_x = obj_field_int32_get(a1, OBJ_F_OFFSET_X);
    offset_y = obj_field_int32_get(a1, OBJ_F_OFFSET_Y);
    sub_4B8680(loc1, &loc_x, &loc_y);

    for (idx = 0; idx < v1; idx += 2) {
        if (loc1 == loc2) {
            break;
        }

        offset_x += offsets[idx];
        offset_y += offsets[idx + 1];
        sub_4B8730(loc_x + offset_x + 40, loc_y + offset_y + 20, &new_loc);
        if (new_loc != loc1) {
            rotation = sub_4B8D50(loc1, new_loc);
            if (new_loc == loc2) {
                flags |= 0x10;
            }

            cnt += sub_43FDC0(OBJ_HANDLE_NULL, loc1, rotation, flags, &v2, &v3, 0);
            loc1 = new_loc;
        }
    }

    return cnt;
}

// 0x4AF800
bool sub_4AF800(int64_t obj, int64_t a2)
{
    return !sub_45D8D0(a2)
        && stat_level(obj, STAT_INTELLIGENCE) < 5
        && obj_field_int32_get(a2, OBJ_F_NAME) == CLOCKWORK_DECOY;
}

// 0x4AF860
void sub_4AF860()
{
    John v1;

    dword_5B50CC = !dword_5B50CC;

    v1.type = 4;
    if (dword_5B50CC) {
        v1.str = "NPC fighting is ON";
    } else {
        v1.str = "NPC fighting is OFF";
    }
    sub_460630(&v1);
}

// 0x4AF8C0
void sub_4AF8C0(int64_t a1, int64_t a2)
{
    int64_t v1;
    ObjectList objects;
    ObjectNode* node;

    v1 = sub_45DDA0(a2);
    if (v1 == OBJ_HANDLE_NULL) {
        v1 = a2;
    }

    sub_4413E0(v1, &objects);

    node = objects.head;
    while (node != NULL) {
        sub_4AF930(a1, node->obj);
        node = node->next;
    }

    object_list_destroy(&objects);
}

// 0x4AF930
void sub_4AF930(int64_t a1, int64_t a2)
{
    int64_t v1;
    int index;

    if (obj_field_int32_get(a2, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        v1 = sub_45DDA0(a2);
    } else {
        v1 = a2;
    }

    if ((v1 == OBJ_HANDLE_NULL || v1 != sub_45DDA0(a1))
        && !sub_4AFB30(a1, a2)) {
        index = obj_arrayfield_length_get(a1, OBJ_F_NPC_SHIT_LIST_IDX);
        obj_arrayfield_obj_set(a1, OBJ_F_NPC_SHIT_LIST_IDX, index, a2);
        sub_4B80E0(a1);
    }
}

// 0x4AF9D0
void sub_4AF9D0(int64_t a1, int64_t a2)
{
    int cnt;
    int index;
    int64_t other_obj;

    cnt = obj_arrayfield_length_get(a1, OBJ_F_NPC_SHIT_LIST_IDX);
    for (index = 0; index < cnt; index++) {
        obj_arrayfield_obj_get(a1, OBJ_F_NPC_SHIT_LIST_IDX, index, &other_obj);
        if (other_obj == a2 || other_obj == OBJ_HANDLE_NULL) {
            if (index < cnt - 1) {
                obj_arrayfield_obj_get(a1, OBJ_F_NPC_SHIT_LIST_IDX, cnt - 1, &other_obj);
                obj_arrayfield_obj_set(a1, OBJ_F_NPC_SHIT_LIST_IDX, index, other_obj);
            }
            obj_arrayfield_length_set(a1, OBJ_F_NPC_SHIT_LIST_IDX, cnt - 1);
            cnt--;
            index--;
        }
    }
}

// 0x4AFA90
int64_t sub_4AFA90(int64_t obj)
{
    int cnt;
    int start;
    int index;
    int64_t shit_obj;

    sub_4AF9D0(obj, OBJ_HANDLE_NULL);

    cnt = obj_arrayfield_length_get(obj, OBJ_F_NPC_SHIT_LIST_IDX);
    start = random_between(0, cnt - 1);

    for (index = 0; index < cnt; index++) {
        obj_arrayfield_obj_get(obj, OBJ_F_NPC_SHIT_LIST_IDX, (start + index) % cnt, &shit_obj);
        if (sub_4AB990(obj, shit_obj)) {
            return shit_obj;
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x4AFB30
bool sub_4AFB30(int64_t obj, int64_t a2)
{
    int cnt;
    int index;
    int64_t v1;

    if (obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        cnt = obj_arrayfield_length_get(obj, OBJ_F_NPC_SHIT_LIST_IDX);
        for (index = 0; index < cnt; index++) {
            obj_arrayfield_obj_get(obj, OBJ_F_NPC_SHIT_LIST_IDX, index, &v1);
            if (v1 == a2) {
                return true;
            }
        }
    }

    return false;
}

// 0x4AFBB0
int sub_4AFBB0(int64_t obj)
{
    return sub_40DA20(obj) ? 5 : 10;
}

// 0x4AFBD0
void ai_target_lock(int64_t obj, int64_t tgt)
{
    int danger_type;
    int64_t danger_source;
    unsigned int flags;

    if (obj != OBJ_HANDLE_NULL
        && tgt != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        ai_target_unlock(obj);
        sub_4AABE0(obj, 1, tgt, 0);
        ai_danger_source(obj, &danger_type, &danger_source);
        if (danger_type == 1 && danger_source == tgt) {
            flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2);
            obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS2, flags | OCF2_TARGET_LOCK);
        }
    }
}

// 0x4AFC70
void ai_target_unlock(int64_t obj)
{
    unsigned int flags;

    if (obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2);
        if ((flags & OCF2_TARGET_LOCK) != 0) {
            obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS2, flags & ~OCF2_TARGET_LOCK);
        }
    }
}
