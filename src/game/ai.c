#include "game/ai.h"

#include "game/critter.h"
#include "game/map.h"
#include "game/obj.h"
#include "game/object_node.h"
#include "game/stat.h"
#include "game/timeevent.h"

#define CLOCKWORK_DECOY 6719

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

static void sub_4A88D0(Ai* ai, int64_t obj);
static void sub_4A94C0(int64_t obj, int64_t tgt);
static void sub_4A9F10(int64_t a1, int64_t a2, int64_t a3, int a4);
static void sub_4AA420(int64_t obj, int64_t a2);
static bool sub_4AAA30(TimeEvent* timeevent);
static void ai_danger_source(int64_t obj, int* type_ptr, int64_t* danger_source_ptr);
static void sub_4ABC20(Ai* ai);
static bool sub_4ABEB0(int64_t obj, int64_t tgt);
static void sub_4AC180(Ai* ai);
static void sub_4AC320(Ai* ai);
static void sub_4AC350(Ai* ai);
static void sub_4AC620(Ai* ai);
static void sub_4AC660(Ai* ai);
static bool ai_is_day();
static bool ai_get_standpoint(int64_t obj, int64_t* standpoint_ptr);
static void sub_4AD1B0(int64_t a1, int64_t a2, int a3);
static int sub_4AD5D0(int64_t obj);
static bool sub_4AD6B0(TimeEvent* timeevent);
static void sub_4AD700(int64_t obj, int millis);
static void sub_4AD730(int64_t obj, DateTime* datetime);
static int sub_4AF240(int value);
static bool sub_4AF800(int64_t obj, int64_t a2);

// 0x5F8488
static Func5F8488* dword_5F8488;

// 0x5F848C
static Func5F848C* dword_5F848C;

// 0x5F8490
static int64_t qword_5F8490;

// 0x5F84A0
static int64_t qword_5F84A0;

// 0x4A8320
void ai_init()
{
    // TODO: Incomplete.
}

// 0x4A83F0
void ai_exit()
{
}

// 0x4A8400
void ai_mod_load()
{
    // TODO: Incomplete.
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
void sub_4A8570()
{
    // TODO: Incomplete.
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
    ai->field_24 = 0;
    ai->leader_obj = critter_leader_get(obj);
    ai_danger_source(obj, &(ai->danger_type), &(ai->danger_source));
    ai->field_30 = -1;
}

// 0x4A8940
void sub_4A8940()
{
    // TODO: Incomplete.
}

// 0x4A8AA0
void sub_4A8AA0()
{
    // TODO: Incomplete.
}

// 0x4A8E70
void sub_4A8E70()
{
    // TODO: Incomplete.
}

// 0x4A8F90
void sub_4A8F90()
{
    // TODO: Incomplete.
}

// 0x4A92D0
void sub_4A92D0()
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
void sub_4A9530()
{
    // TODO: Incomplete.
}

// 0x4A9560
void sub_4A9560()
{
    // TODO: Incomplete.
}

// 0x4A9650
void sub_4A9650()
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
void sub_4A9B80()
{
    // TODO: Incomplete.
}

// 0x4A9C00
void sub_4A9C00()
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
            if (!sub_4AF260(a1, a3) || !sub_4AF470(a1, a3, a4)) {
                sub_4AA620(a1, a2);
            }
        } else if (sub_4AE3A0(a1, a2)) {
            if (!sub_4AF260(a1, a3) || !sub_4AF470(a1, a3, a4)) {
                sub_4AA620(a1, a3);
            }
        } else if (critter_social_class_get(a1) != SOCIAL_CLASS_GUARD
            && (obj_field_int32_get(a1, OBJ_F_CRITTER_FLAGS) & OCF_NO_FLEE) == 0) {
            ai_danger_source(a1, &danger_type, NULL);
            if (danger_type == 0
                && (!sub_4AF260(a1, a3) || !sub_4AF470(a1, a3, a4))) {
                    sub_4AABE0(a1, 2, a2, 0);
                }
        }
    }
}

// 0x4AA0D0
void sub_4AA0D0()
{
    // TODO: Incomplete.
}

// 0x4AA1B0
void sub_4AA1B0()
{
    // TODO: Incomplete.
}

// 0x4AA300
void sub_4AA300()
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
    ObjectNodeList obj_list;
    ObjectNode* node;

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    switch (type) {
    case OBJ_TYPE_NPC:
        combat_focus_obj = obj_field_handle_get(obj, OBJ_F_NPC_COMBAT_FOCUS);
        if (combat_focus_obj != NULL) {
            combat_focus_obj(obj, combat_focus_obj);
            sub_424070(obj, 3, 0, 0);
        }
        if (sub_45DDA0(obj)) {
            flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
            flags |= ONF_NO_ATTACK;
            obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, flags);
        }
        ai_target_unlock(obj);
        break;
    case OBJ_TYPE_PC:
        sub_441260(obj, &obj_list);

        node = obj_list.head;
        while (node != NULL) {
            sub_4AA4A0(node->obj);
            node = node->next;
        }

        object_list_destroy(&obj_list);
        break;
    }
}

// 0x4AA580
void sub_4AA580(int64_t obj)
{
    int type;
    unsigned int flags;
    ObjectNodeList obj_list;
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
        sub_441260(obj, &obj_list);

        node = obj_list.head;
        while (node != NULL) {
            sub_4AA580(node->obj);
            node = node->next;
        }

        object_list_destroy(&obj_list);
        break;
    }
}

// 0x4AA620
void sub_4AA620()
{
    // TODO: Incomplete.
}

// 0x4AA7A0
void sub_4AA7A0()
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
void sub_4AAA60()
{
    // TODO: Incomplete.
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
void sub_4AABE0()
{
    // TODO: Incomplete.
}

// 0x4AAF50
void sub_4AAF50()
{
    // TODO: Incomplete.
}

// 0x4AB030
void sub_4AB030()
{
    // TODO: Incomplete.
}

// 0x4AB0B0
void sub_4AB0B0()
{
    // TODO: Incomplete.
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
void sub_4AB2F0()
{
    // TODO: Incomplete.
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
void sub_4AB460()
{
    // TODO: Incomplete.
}

// 0x4AB990
void sub_4AB990()
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
void sub_4ABC70()
{
    // TODO: Incomplete.
}

// 0x4ABE20
void sub_4ABE20()
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
void sub_4ABF10()
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
void sub_4AC250()
{
    // TODO: Incomplete.
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
void sub_4AC380()
{
    // TODO: Incomplete.
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
void sub_4AC6E0()
{
    // TODO: Incomplete.
}

// 0x4AC7B0
void sub_4AC7B0()
{
    // TODO: Incomplete.
}

// 0x4AC910
void sub_4AC910()
{
    // TODO: Incomplete.
}

// 0x4ACBB0
void sub_4ACBB0()
{
    // TODO: Incomplete.
}

// 0x4ACD90
bool ai_is_day()
{
    int hour;

    hour = game_time_hour();
    return hour >= 6 && hour < 21;
}

// 0x4ACDB0
void sub_4ACDB0()
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
void sub_4AD0B0()
{
    // TODO: Incomplete.
}

// 0x4AD140
void sub_4AD140()
{
    // TODO: Incomplete.
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
void ai_timeevent_process()
{
    // TODO: Incomplete.
}

// 0x4AD420
void sub_4AD420()
{
    // TODO: Incomplete.
}

// 0x4AD4D0
void sub_4AD4D0()
{
    // TODO: Incomplete.
}

// 0x4AD5D0
int sub_4AD5D0(int64_t obj)
{
    return 4750 * sub_4AD610(obj) / 30 + 250;
}

// 0x4AD610
void sub_4AD610()
{
    // TODO: Incomplete.
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
void sub_4AD790()
{
    // TODO: Incomplete.
}

// 0x4AD7D0
void sub_4AD7D0(int64_t obj)
{
    qword_5F84A0 = obj;
    timeevent_clear_one_ex(TIMEEVENT_TYPE_AI, sub_4AD6B0);
}

// 0x4AD800
void sub_4AD800()
{
    // TODO: Incomplete.
}

// 0x4AD950
void sub_4AD950()
{
    // TODO: Incomplete.
}

// 0x4ADB50
void sub_4ADB50()
{
    // TODO: Incomplete.
}

// 0x4ADCC0
void sub_4ADCC0()
{
    // TODO: Incomplete.
}

// 0x4ADE00
void sub_4ADE00()
{
    // TODO: Incomplete.
}

// 0x4ADFF0
void sub_4ADFF0()
{
    // TODO: Incomplete.
}

// 0x4AE020
void sub_4AE020()
{
    // TODO: Incomplete.
}

// 0x4AE0A0
void sub_4AE0A0()
{
    // TODO: Incomplete.
}

// 0x4AE120
void sub_4AE120()
{
    // TODO: Incomplete.
}

// 0x4AE3A0
void sub_4AE3A0()
{
    // TODO: Incomplete.
}

// 0x4AE450
void sub_4AE450()
{
    // TODO: Incomplete.
}

// 0x4AE4E0
void sub_4AE4E0()
{
    // TODO: Incomplete.
}

// 0x4AE570
void sub_4AE570()
{
    // TODO: Incomplete.
}

// 0x4AE720
void sub_4AE720()
{
    // TODO: Incomplete.
}

// 0x4AE9E0
void sub_4AE9E0()
{
    // TODO: Incomplete.
}

// 0x4AEAB0
void sub_4AEAB0()
{
    // TODO: Incomplete.
}

// 0x4AEB10
void sub_4AEB10()
{
    // TODO: Incomplete.
}

// 0x4AEB70
void sub_4AEB70()
{
    // TODO: Incomplete.
}

// 0x4AECA0
void sub_4AECA0()
{
    // TODO: Incomplete.
}

// 0x4AED80
void sub_4AED80()
{
    // TODO: Incomplete.
}

// 0x4AEE50
void sub_4AEE50()
{
    // TODO: Incomplete.
}

// 0x4AF130
void sub_4AF130()
{
    // TODO: Incomplete.
}

// 0x4AF170
void sub_4AF170()
{
    // TODO: Incomplete.
}

// 0x4AF1D0
void sub_4AF1D0()
{
    // TODO: Incomplete.
}

// 0x4AF210
void sub_4AF210()
{
    // TODO: Incomplete.
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
void sub_4AF260()
{
    // TODO: Incomplete.
}

// 0x4AF470
void sub_4AF470()
{
    // TODO: Incomplete.
}

// 0x4AF640
void sub_4AF640()
{
    // TODO: Incomplete.
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
    // TODO: Incomplete.
}

// 0x4AF8C0
void sub_4AF8C0()
{
    // TODO: Incomplete.
}

// 0x4AF930
void sub_4AF930()
{
    // TODO: Incomplete.
}

// 0x4AF9D0
void sub_4AF9D0()
{
    // TODO: Incomplete.
}

// 0x4AFA90
void sub_4AFA90()
{
    // TODO: Incomplete.
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
void sub_4AFBB0()
{
    // TODO: Incomplete.
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
