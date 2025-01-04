#include "game/trap.h"

#include "game/ai.h"
#include "game/animfx.h"
#include "game/combat.h"
#include "game/critter.h"
#include "game/item.h"
#include "game/magictech.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj_private.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/proto.h"
#include "game/random.h"
#include "game/script.h"
#include "game/skill.h"
#include "game/stat.h"
#include "game/ui.h"

#define TRAP_START_SENTINEL 0x81726354
#define TRAP_END_SENTINEL 0x81726354

typedef enum TrapScript {
    TRAP_SCRIPT_MAGICAL = 30000,
    TRAP_SCRIPT_MECHANICAL = 30001,
    TRAP_SCRIPT_ARROW = 30002,
    TRAP_SCRIPT_BULLET = 30003,
    TRAP_SCRIPT_FIRE = 30004,
    TRAP_SCRIPT_ELECTRICAL = 30005,
    TRAP_SCRIPT_POISON = 30006,
    TRAP_SCRIPT_TRAP_SOURCE = 30007,
    // TODO: Bad name, it's not count.
    TRAP_SCRIPT_COUNT,
    TRAP_SCRIPT_FIRST = TRAP_SCRIPT_MAGICAL,
    TRAP_SCRIPT_LAST = TRAP_SCRIPT_POISON,
} TrapScript;

typedef struct TrapListNode {
    /* 0000 */ ObjectID trap_oid;
    /* 0018 */ ObjectID pc_oid;
    /* 0030 */ struct TrapListNode* next;
} TrapListNode;

static_assert(sizeof(TrapListNode) == 0x38, "wrong size");

static int trap_type_from_scr(Script* scr);
static void sub_4BC690(int spl, int64_t loc, int delay, int64_t item_obj);
static bool get_disarm_item_name(int64_t obj, int* name_ptr);
void trigger_trap(int64_t obj, ScriptInvocation* invocation);
static int64_t find_trap_source(int64_t obj, uint8_t id);
static TrapListNode* sub_4BD1E0(int64_t pc_obj, int64_t trap_obj);
static int sub_4BD340(int64_t trap_obj);
static bool sub_4BD430(int64_t pc_obj, int64_t trap_obj);
static TrapListNode* sub_4BD480(int64_t pc_obj, int64_t trap_obj);
static int64_t sub_4BD950(int64_t obj);

// 0x5B5F60
static int dword_5B5F60[8] = {
    26000,
    26001,
    26002,
    26003,
    26004,
    26005,
    26006,
    26007,
};

// 0x5B5F80
static int dword_5B5F80[8] = {
    0,
    6047, // Railroad Spike
    7038, // Arrow
    7039, // Bullet
    7041, // Fuel
    7040, // Battery
    10065, // Poison
};

// 0x5FC458
static mes_file_handle_t trap_mes_file;

// 0x5FC460
static AnimFxList stru_5FC460;

// 0x5FC48C
static TrapListNode* dword_5FC48C;

// 0x4BBD00
bool trap_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (!animfx_list_init(&stru_5FC460)) {
        return false;
    }

    stru_5FC460.path = "Rules\\TrapEyeCandy.mes";
    stru_5FC460.field_18 = 24;
    stru_5FC460.field_C = 3;
    stru_5FC460.field_10 = 10;
    if (!animfx_list_load(&stru_5FC460)) {
        return false;
    }

    if (!mes_load("mes\\trap.mes", &trap_mes_file)) {
        animfx_list_exit(&stru_5FC460);
        return false;
    }

    dword_5FC48C = NULL;

    return true;
}

// 0x4BBD90
void trap_exit()
{
    TrapListNode* node;

    animfx_list_exit(&stru_5FC460);
    mes_unload(trap_mes_file);

    while (dword_5FC48C != NULL) {
        node = dword_5FC48C;
        dword_5FC48C = dword_5FC48C->next;
        FREE(node);
    }
}

// 0x4BBDD0
int trap_type(int64_t obj)
{
    Script scr;

    obj_arrayfield_script_get(obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
    return trap_type_from_scr(&scr);
}

// 0x4BBE00
int trap_type_from_scr(Script* scr)
{
    unsigned int flags;

    if (scr->num == 0) {
        return TRAP_TYPE_INVALID;
    }

    if (!sub_44C310(scr, &flags)) {
        return TRAP_TYPE_INVALID;
    }

    if ((flags & SF_MAGICAL_TRAP) != 0) {
        return TRAP_TYPE_MAGICAL;
    }

    if ((flags & SF_NONMAGICAL_TRAP) != 0) {
        return TRAP_TYPE_NONMAGICAL;
    }

    return TRAP_TYPE_INVALID;
}

// 0x4BBE40
bool sub_4BBE40(int64_t pc_obj, int64_t trap_obj)
{
    int type;
    Script scr;
    SkillInvocation skill_invocation;
    int64_t item_obj;

    if (pc_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    type = obj_field_int32_get(pc_obj, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return false;
    }

    type = obj_field_int32_get(trap_obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_GENERIC
        && (obj_field_int32_get(trap_obj, OBJ_F_GENERIC_FLAGS) & OGF_IS_TRAP_DEVICE) != 0) {
        return false;
    }

    obj_arrayfield_script_get(trap_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
    if (trap_type_from_scr(&scr) == TRAP_TYPE_INVALID) {
        return false;
    }

    if (sub_4BBFE0(pc_obj, trap_obj)) {
        return true;
    }

    if (!sub_458A80(OSF_DETECTING_INVISIBLE)) {
        skill_invocation_init(&skill_invocation);
        sub_4440E0(pc_obj, &(skill_invocation.source));
        sub_4440E0(trap_obj, &(skill_invocation.target));
        skill_invocation.skill = SKILL_SPOT_TRAP;

        item_obj = item_wield_get(pc_obj, 1005);
        if (item_obj != OBJ_HANDLE_NULL
            && obj_field_int32_get(item_obj, OBJ_F_NAME) == 2866
            && item_ammo_quantity_get(pc_obj, 2) > 0) {
            skill_invocation.modifier = stat_level(pc_obj, STAT_MAGICK_TECH_APTITUDE);
            if (skill_invocation.modifier > 0) {
                skill_invocation.modifier = 0;
            }
        }

        if (!skill_invocation_run(&skill_invocation)) {
            return false;
        }

        if ((skill_invocation.flags & SKILL_INVOCATION_SUCCESS) == 0) {
            return false;
        }
    }

    sub_4BC090(pc_obj, trap_obj, 0);
    return true;
}

// 0x4BBFE0
bool sub_4BBFE0(int64_t pc_obj, int64_t trap_obj)
{
    int type;
    unsigned int flags;

    if (pc_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    type = obj_field_int32_get(pc_obj, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return false;
    }

    flags = obj_field_int32_get(trap_obj, OBJ_F_FLAGS);
    if (type != OBJ_TYPE_PC && critter_pc_leader_get(pc_obj) == OBJ_HANDLE_NULL) {
        return (flags & OF_TRAP_PC) == 0;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        return sub_4BD430(pc_obj, trap_obj);
    }

    if ((flags & (OF_TRAP_PC | OF_TRAP_SPOTTED)) == 0) {
        return false;
    }

    return true;
}

// 0x4BC090
void sub_4BC090(int64_t pc_obj, int64_t trap_obj, int a3)
{
    int type;
    Script scr;
    AnimFxNode animfx;
    MesFileEntry mes_file_entry;
    UiMessage ui_message;

    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return;
        }

        sub_4EDC70(pc_obj, trap_obj, a3);
    }

    if (pc_obj != OBJ_HANDLE_NULL) {
        sub_4BD1E0(pc_obj, trap_obj);

        type = obj_field_int32_get(pc_obj, OBJ_F_TYPE);
        switch (type) {
        case OBJ_TYPE_PC:
            if (!player_is_pc_obj(pc_obj)) {
                return;
            }
            break;
        case OBJ_TYPE_NPC:
            if (critter_pc_leader_get(pc_obj) == OBJ_HANDLE_NULL) {
                return;
            }
            break;
        default:
            return;
        }

        object_flags_set(trap_obj, OF_TRAP_SPOTTED);

        if (obj_field_int32_get(trap_obj, OBJ_F_TYPE) == OBJ_TYPE_TRAP) {
            object_flags_unset(trap_obj, OF_DONTDRAW);
        } else {
            obj_arrayfield_script_get(trap_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
            if (scr.num >= TRAP_SCRIPT_FIRST && scr.num < TRAP_SCRIPT_COUNT) {
                sub_4CCD20(&stru_5FC460, &animfx, trap_obj, -1, 3 * scr.num - 90000 - 2);
                animfx.field_1C = 1;
                animfx_add(&animfx);
            }
        }

        if (a3 == 0 && pc_obj == player_get_pc_obj()) {
            // You have spotted a trap!
            mes_file_entry.num = 0;
            mes_get_msg(trap_mes_file, &mes_file_entry);

            ui_message.type = UI_MSG_TYPE_EXCLAMATION;
            ui_message.str = mes_file_entry.str;
            sub_460630(&ui_message);
        }
    }
}

// 0x4BC220
void sub_4BC220(int64_t trap_obj)
{
    Script scr;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        sub_4BD340(trap_obj);
    }

    if (trap_type(trap_obj) != TRAP_TYPE_INVALID) {
        if (obj_field_int32_get(trap_obj, OBJ_F_TYPE) == OBJ_TYPE_TRAP) {
            sub_43CCA0(trap_obj);
        } else {
            obj_arrayfield_script_get(trap_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
            if (scr.num >= TRAP_SCRIPT_FIRST && scr.num < TRAP_SCRIPT_COUNT) {
                animfx_remove(&stru_5FC460, trap_obj, 3 * scr.num - 90000 - 2, -1);
            }
            scr.num = 0;
            obj_arrayfield_script_set(trap_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
        }
    }
}

// 0x4BC2E0
bool sub_4BC2E0(int64_t pc_obj, int64_t item_obj, int64_t target_obj)
{
    Script scr;
    MesFileEntry mes_file_entry;
    UiMessage ui_message;
    int64_t target_loc;
    int target_type;
    int spl;

    target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    target_type = obj_field_int32_get(target_obj, OBJ_F_TYPE);
    if (target_type == OBJ_TYPE_WALL) {
        return sub_4BC480(pc_obj, item_obj, target_loc);
    }

    spl = obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_2);
    if (spl == 10000) {
        // Check if we can trap the target object - it should be either a door
        // or a container with no use script.
        obj_arrayfield_script_get(target_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
        if (scr.num != 0
            || (target_type != OBJ_TYPE_PORTAL && target_type != OBJ_TYPE_CONTAINER)) {
            // You cannot trap that!
            mes_file_entry.num = 2;
            mes_get_msg(trap_mes_file, &mes_file_entry);

            ui_message.type = UI_MSG_TYPE_EXCLAMATION;
            ui_message.str = mes_file_entry.str;
            sub_460630(&ui_message);

            return false;
        }

        obj_arrayfield_script_get(item_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
        obj_arrayfield_script_set(target_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
        sub_43CCA0(item_obj);
        object_flags_set(target_obj, OF_TRAP_PC);
        sub_4BC090(pc_obj, target_obj, 1);

        return true;
    }

    sub_43CCA0(item_obj);
    if (spl == 176) {
        sub_4AA0D0(pc_obj);
    }
    sub_4BC690(spl, target_loc, 88, OBJ_HANDLE_NULL);

    return true;
}

// 0x4BC480
bool sub_4BC480(int64_t pc_obj, int64_t item_obj, int64_t target_loc)
{
    Script scr;
    MesFileEntry mes_file_entry;
    UiMessage ui_message;
    int spl;
    int64_t prototype_handle;
    int64_t trap_obj;
    int name;

    spl = obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_2);
    if (spl != 10000) {
        sub_43CCA0(item_obj);
        switch (spl) {
        case 176:
            sub_4AA0D0(pc_obj);
            prototype_handle = sub_4685A0(4047);
            break;
        case 220:
            prototype_handle = sub_4685A0(4049);
            break;
        default:
            prototype_handle = OBJ_HANDLE_NULL;
            break;
        }

        if (prototype_handle != OBJ_HANDLE_NULL) {
            if (object_create(prototype_handle, target_loc, &trap_obj)) {
                object_flags_set(trap_obj, OF_TRAP_PC);
                sub_4BC090(pc_obj, trap_obj, 1);
            }
        }

        sub_4BC690(spl, target_loc, 88, item_obj);
        return true;
    }

    if (sub_4BCAB0(target_loc) != OBJ_HANDLE_NULL) {
        // There is already a trap there.
        mes_file_entry.num = 3;
        mes_get_msg(trap_mes_file, &mes_file_entry);

        ui_message.type = UI_MSG_TYPE_EXCLAMATION;
        ui_message.str = mes_file_entry.str;
        sub_460630(&ui_message);

        return false;
    }

    obj_arrayfield_script_get(item_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
    if (scr.num >= TRAP_SCRIPT_FIRST && scr.num < TRAP_SCRIPT_COUNT) {
        name = dword_5B5F60[scr.num - TRAP_SCRIPT_FIRST];
    } else {
        name = trap_type_from_scr(&scr) == TRAP_TYPE_MAGICAL ? 26000 : 26001;
    }

    prototype_handle = sub_4685A0(name);
    if (!object_create(prototype_handle, target_loc, &trap_obj)) {
        return false;
    }

    obj_arrayfield_script_set(trap_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
    object_flags_set(trap_obj, OF_TRAP_PC);
    sub_4BC090(pc_obj, trap_obj, 1);
    sub_43CCA0(item_obj);

    return true;
}

// 0x4BC690
void sub_4BC690(int spl, int64_t loc, int delay, int64_t item_obj)
{
    TimeEvent timeevent;
    DateTime datetime;
    MagicTechSerializedData v1;

    if (delay != 0) {
        timeevent.type = TIMEEVENT_TYPE_TRAPS;
        timeevent.params[0].integer_value = spl;
        timeevent.params[1].location_value = loc;
        timeevent.params[2].object_value = item_obj;
        sub_45A950(&datetime, 1000 * delay);
        sub_45B800(&timeevent, &datetime);
    } else {
        sub_455A20(&v1, OBJ_HANDLE_NULL, spl);
        v1.target_loc = loc;
        sub_455AC0(&v1);

        if (item_obj != OBJ_HANDLE_NULL) {
            sub_43CCA0(item_obj);
        }
    }
}

// 0x4BC780
bool trap_timeevent_process(TimeEvent* timeevent)
{
    sub_4BC690(timeevent->params[0].integer_value,
        timeevent->params[1].object_value,
        false,
        timeevent->params[2].object_value);
    return true;
}

// 0x4BC7B0
void sub_4BC7B0(int64_t pc_obj, int64_t trap_obj, bool* is_success_ptr, bool* is_critical_ptr)
{
    int disarm_item_name;
    int64_t prototype_handle;
    int64_t loc;
    int64_t disarm_item_obj;
    Packet70 pkt;

    if (trap_type(trap_obj) == TRAP_TYPE_INVALID) {
        *is_success_ptr = false;
        *is_critical_ptr = false;
        return;
    }

    if (!sub_4BBFE0(pc_obj, trap_obj)) {
        *is_success_ptr = false;
        *is_critical_ptr = false;
        return;
    }

    if (*is_success_ptr) {
        if (*is_critical_ptr
            && tech_skill_get_training(pc_obj, TECH_SKILL_DISARM_TRAPS) >= TRAINING_EXPERT
            && get_disarm_item_name(trap_obj, &disarm_item_name)) {
            prototype_handle = sub_4685A0(disarm_item_name);
            loc = obj_field_int32_get(pc_obj, OBJ_F_LOCATION);
            if (object_create(prototype_handle, loc, &disarm_item_obj)) {
                if ((tig_net_flags & TIG_NET_CONNECTED) != 0
                    && (tig_net_flags & TIG_NET_HOST) != 0) {
                    pkt.type = 70;
                    pkt.subtype = 0;
                    pkt.s0.name = disarm_item_name;
                    pkt.s0.oid = sub_407EF0(pc_obj);
                    pkt.s0.loc = loc;
                    pkt.s0.field_38 = sub_407EF0(trap_obj);
                    pkt.s0.field_30 = 1;
                    tig_net_send_app_all(&pkt, sizeof(pkt));
                }

                sub_4A2BC0();
                item_transfer(disarm_item_obj, pc_obj);
                sub_4A2BD0();
            }
        }
        sub_4BC220(trap_obj);
    } else {
        if (*is_critical_ptr) {
            sub_441980(pc_obj, trap_obj, OBJ_HANDLE_NULL, 1, 0);
        }
    }
}

// 0x4BC9C0
bool get_disarm_item_name(int64_t trap_obj, int* name_ptr)
{
    Script scr;

    obj_arrayfield_script_get(trap_obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
    if (scr.num < TRAP_SCRIPT_FIRST || scr.num >= TRAP_SCRIPT_COUNT) {
        return false;
    }

    if ((obj_field_int32_get(trap_obj, OBJ_F_FLAGS) & OF_TRAP_PC) != 0) {
        *name_ptr = ((scr.hdr.counters >> 24) & 0xFF) != 0 ? 15101 : 6047;
        return true;
    }

    if (dword_5B5F80[scr.num - TRAP_SCRIPT_FIRST] != 0) {
        *name_ptr = dword_5B5F80[scr.num - TRAP_SCRIPT_FIRST];
        return true;
    }

    return false;
}

// 0x4BCA60
bool trap_is_trap_device(int64_t obj)
{
    return obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_GENERIC
        && (obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) & OGF_IS_TRAP_DEVICE) != 0;
}

// 0x4BCAB0
int64_t sub_4BCAB0(int64_t loc)
{
    ObjectList traps;
    int64_t first_trap;

    sub_4407C0(loc, OBJ_TM_TRAP, &traps);
    if (traps.head != NULL) {
        first_trap = traps.head->obj;
    } else {
        first_trap = OBJ_HANDLE_NULL;
    }

    object_list_destroy(&traps);

    return first_trap;
}

// 0x4BCB00
void sub_4BCB00(int64_t a1, int64_t a2)
{
    Script scr;
    ScriptInvocation invocation;

    obj_arrayfield_script_get(a2, OBJ_F_SCRIPTS_IDX, 1, &scr);

    invocation.triggerer_obj = a1;
    invocation.extra_obj = OBJ_HANDLE_NULL;
    invocation.line = 0;
    invocation.attachment_point = SAP_USE;
    invocation.attachee_obj = a2;
    invocation.script = &scr;
    sub_4449B0(&invocation);
}

// 0x4BCB70
bool sub_4BCB70(ScriptInvocation* invocation)
{
    unsigned int flags;
    int radius;
    ObjectList critters;
    ObjectNode* node;
    AnimFxNode animfx;
    int v1;

    if (trap_type_from_scr(invocation->script) == TRAP_TYPE_INVALID) {
        return false;
    }

    sub_44C310(invocation->script, &flags);

    if ((flags & SF_RADIUS_FIVE) != 0) {
        radius = 4;
    } else if ((flags & SF_RADIUS_THREE) != 0) {
        radius = 2;
    } else if ((flags & SF_RADIUS_TWO) != 0) {
        radius = 1;
    } else {
        radius = 0;
    }

    if (invocation->script->num < TRAP_SCRIPT_FIRST) {
        return false;
    }

    if (invocation->script->num > TRAP_SCRIPT_LAST) {
        return invocation->script->num == TRAP_SCRIPT_TRAP_SOURCE;
    }

    // NOTE: Silence compiler warning.
    critters.head = NULL;

    if (radius > 0) {
        sub_4AE4E0(invocation->attachee_obj, radius, &critters, OBJ_TM_PC | OBJ_TM_NPC);
    }

    if (invocation->script->num >= TRAP_SCRIPT_FIRST && invocation->script->num < TRAP_SCRIPT_COUNT) {
        v1 = 3 * invocation->script->num - 90000;

        // TODO: Looks unreachable, check.
        if (3 * invocation->script->num != 89999) {
            sub_4CCD20(&stru_5FC460, &animfx, invocation->triggerer_obj, -1, v1);
            animfx.field_1C = 1;
            animfx_add(&animfx);
            animfx_remove(&stru_5FC460, invocation->attachee_obj, v1 + 2, -1);
        }
    } else {
        v1 = -1;
    }

    if (radius > 0) {
        node = critters.head;
        while (node != NULL) {
            trigger_trap(node->obj, invocation);
            if (v1 != -1) {
                sub_4CCD20(&stru_5FC460, &animfx, node->obj, -1, v1 + 1);
                animfx.field_1C = 1;
                animfx_add(&animfx);
            }

            node = node->next;
        }

        object_list_destroy(&critters);

        if ((flags & SF_AUTO_REMOVING) != 0) {
            invocation->script->num = 0;
        }
    } else {
        if (invocation->triggerer_obj != OBJ_HANDLE_NULL) {
            trigger_trap(invocation->triggerer_obj, invocation);
            if (v1 != -1) {
                sub_4CCD20(&stru_5FC460, &animfx, invocation->triggerer_obj, -1, v1 + 1);
                animfx.field_1C = 1;
                animfx_add(&animfx);
            }

            if ((flags & SF_AUTO_REMOVING) != 0) {
                invocation->script->num = 0;
            }
        }
    }

    return true;
}

// 0x4BCDC0
void trigger_trap(int64_t obj, ScriptInvocation* invocation)
{
    MagicTechSerializedData v1;
    CombatContext combat;
    int64_t trap_source_obj;

    switch (invocation->script->num) {
    case TRAP_SCRIPT_MAGICAL:
        sub_455A20(&v1, OBJ_HANDLE_NULL, (invocation->script->hdr.counters >> 18) & 0xFF);
        sub_4440E0(obj, &(v1.target_obj));
        sub_455AC0(&v1);
        return;
    case TRAP_SCRIPT_MECHANICAL:
        sub_4B2210(invocation->attachee_obj, obj, &combat);
        combat.field_30 = sub_4BD950(invocation->attachee_obj);
        combat.flags |= 0x80300;
        combat.dam[DAMAGE_TYPE_NORMAL] = random_between(invocation->script->hdr.counters & 0xFF, (invocation->script->hdr.counters >> 8) & 0xFF);
        sub_4B4390(&combat);
        break;
    case TRAP_SCRIPT_ARROW:
        trap_source_obj = find_trap_source(invocation->attachee_obj, (invocation->script->hdr.counters >> 16) & 0xFF);
        if (trap_source_obj != OBJ_HANDLE_NULL) {
            sub_4B2210(trap_source_obj, obj, &combat);
            combat.field_30 = sub_4BD950(invocation->attachee_obj);
            combat.flags |= 0x300;
            combat.weapon_obj = OBJ_HANDLE_NULL;
            combat.dam[DAMAGE_TYPE_NORMAL] = random_between(invocation->script->hdr.counters & 0xFF, (invocation->script->hdr.counters >> 8) & 0xFF);
            sub_4B3170(&combat);
        }
        break;
    case TRAP_SCRIPT_BULLET:
        sub_4B2210(invocation->attachee_obj, obj, &combat);
        combat.field_30 = sub_4BD950(invocation->attachee_obj);
        combat.flags |= 0x80300;
        combat.dam[DAMAGE_TYPE_NORMAL] = random_between(invocation->script->hdr.counters & 0xFF, (invocation->script->hdr.counters >> 8) & 0xFF);
        sub_4B4390(&combat);
        break;
    case TRAP_SCRIPT_FIRE:
        sub_4B2210(invocation->attachee_obj, obj, &combat);
        combat.field_30 = sub_4BD950(invocation->attachee_obj);
        combat.flags |= 0x80300;
        combat.dam[DAMAGE_TYPE_FIRE] = random_between(invocation->script->hdr.counters & 0xFF, (invocation->script->hdr.counters >> 8) & 0xFF);
        sub_4B4390(&combat);
        break;
    case TRAP_SCRIPT_ELECTRICAL:
        sub_4B2210(invocation->attachee_obj, obj, &combat);
        combat.field_30 = sub_4BD950(invocation->attachee_obj);
        combat.flags |= 0x80300;
        combat.dam[DAMAGE_TYPE_ELECTRICAL] = random_between(invocation->script->hdr.counters & 0xFF, (invocation->script->hdr.counters >> 8) & 0xFF);
        sub_4B4390(&combat);
        break;
    case TRAP_SCRIPT_POISON:
        sub_4B2210(invocation->attachee_obj, obj, &combat);
        combat.field_30 = sub_4BD950(invocation->attachee_obj);
        combat.flags |= 0x80300;
        combat.dam[DAMAGE_TYPE_POISON] = random_between(invocation->script->hdr.counters & 0xFF, (invocation->script->hdr.counters >> 8) & 0xFF);
        sub_4B4390(&combat);
        break;
    default:
        return;
    }

    if (((invocation->script->hdr.counters >> 18) & 0xFF) != 0) {
        sub_455A20(&v1, OBJ_HANDLE_NULL, (invocation->script->hdr.counters >> 18) & 0xFF);
        sub_4440E0(obj, &(v1.target_obj));
        sub_455AC0(&v1);
    }
}

// 0x4BD150
int64_t find_trap_source(int64_t obj, uint8_t id)
{
    int64_t trap_source_obj;
    Script scr;
    ObjectList objects;
    ObjectNode* node;

    trap_source_obj = OBJ_HANDLE_NULL;
    sub_440FC0(obj, OBJ_TM_TRAP | OBJ_TM_SCENERY | OBJ_TM_PORTAL | OBJ_TM_WALL, &objects);

    node = objects.head;
    while (node != NULL) {
        obj_arrayfield_script_get(node->obj, OBJ_F_SCRIPTS_IDX, 1, &scr);
        if (scr.num == TRAP_SCRIPT_TRAP_SOURCE && ((scr.hdr.counters >> 16) & 0xFF) == id) {
            trap_source_obj = node->obj;
            break;
        }
        node = node->next;
    }

    object_list_destroy(&objects);
    return trap_source_obj;
}

// 0x4BD1E0
TrapListNode* sub_4BD1E0(int64_t pc_obj, int64_t trap_obj)
{
    TrapListNode* node;

    node = (TrapListNode*)MALLOC(sizeof(*node));
    node->pc_oid = sub_407EF0(pc_obj);
    node->trap_oid = sub_407EF0(trap_obj);
    node->next = dword_5FC48C;
    dword_5FC48C = node;

    return node;
}

// 0x4BD340
int sub_4BD340(int64_t trap_obj)
{
    int cnt = 0;
    ObjectID trap_oid;
    TrapListNode* node;
    TrapListNode* tmp;

    trap_oid = sub_407EF0(trap_obj);

    while (dword_5FC48C != NULL) {
        if (!objid_is_equal(dword_5FC48C->trap_oid, trap_oid)) {
            break;
        }

        tmp = dword_5FC48C;
        dword_5FC48C = dword_5FC48C->next;
        FREE(tmp);
        cnt++;
    }

    node = dword_5FC48C;
    while (node != NULL && node->next != NULL) {
        if (objid_is_equal(node->trap_oid, trap_oid)) {
            tmp = node->next;
            node->next = tmp->next;
            FREE(tmp);
            cnt++;
        }
        node = node->next;
    }

    return cnt;
}

// 0x4BD430
bool sub_4BD430(int64_t pc_obj, int64_t trap_obj)
{
    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        pc_obj = critter_pc_leader_get(pc_obj);
    }

    return sub_4BD480(pc_obj, trap_obj) != 0;
}

// 0x4BD480
TrapListNode* sub_4BD480(int64_t pc_obj, int64_t trap_obj)
{
    ObjectID trap_oid;
    ObjectID pc_oid;
    TrapListNode* node;

    trap_oid = sub_407EF0(trap_obj);
    pc_oid = sub_407EF0(pc_obj);

    node = dword_5FC48C;
    while (node != NULL) {
        if (objid_is_equal(pc_oid, node->pc_oid)
            && objid_is_equal(trap_oid, node->trap_oid)) {
            return node;
        }
        node = node->next;
    }

    return NULL;
}

// 0x4BD550
bool mp_load(GameLoadInfo* load_info)
{
    unsigned int sentinel;
    int cnt;
    ObjectID pc_oid;
    ObjectID trap_oid;
    TrapListNode* node;

    if (load_info->stream == NULL) {
        return false;
    }

    if (tig_file_fread(&sentinel, sizeof(sentinel), 1, load_info->stream) != 1) {
        tig_debug_printf("TRAP: mp_load: FAILURE: could not read START sentinel.\n");
        return false;
    }

    if (sentinel != TRAP_START_SENTINEL) {
        tig_debug_printf("TRAP: mp_load: FAILURE: START sentinel read (0x%08X) does not match expected (0x%08X)!\n", sentinel, TRAP_START_SENTINEL);
        return false;
    }

    if (tig_file_fread(&cnt, sizeof(cnt), 1, load_info->stream) != 1) {
        tig_debug_printf("TRAP: mp_load: FAILURE: could not read count.\n");
        return false;
    }

    while (cnt != 0) {
        if (tig_file_fread(&pc_oid, sizeof(pc_oid), 1, load_info->stream) != 1) {
            tig_debug_printf("TRAP: mp_load: FAILURE: could not read pc_id for entry %d.\n", cnt);
            return false;
        }

        if (tig_file_fread(&trap_oid, sizeof(trap_oid), 1, load_info->stream) != 1) {
            tig_debug_printf("TRAP: mp_load: FAILURE: could not read trap_id for entry %d.\n", cnt);
            return false;
        }

        node = (TrapListNode*)MALLOC(sizeof(*node));
        node->pc_oid = pc_oid;
        node->trap_oid = trap_oid;
        node->next = dword_5FC48C;
        dword_5FC48C = node;

        cnt--;
    }

    if (tig_file_fread(&sentinel, sizeof(sentinel), 1, load_info->stream) != 1) {
        tig_debug_printf("TRAP: mp_load: FAILURE: could not read END sentinel.\n");
        return false;
    }

    if (sentinel != TRAP_END_SENTINEL) {
        tig_debug_printf("TRAP: mp_load: FAILURE: END sentinel read (0x%08X) does not match expected (0x%08X)!\n", sentinel, TRAP_END_SENTINEL);
        return false;
    }

    return true;
}

// 0x4BD710
bool mp_save(TigFile* stream)
{
    unsigned int sentinel;
    int cnt;
    TrapListNode* node;

    if (stream == NULL) {
        return false;
    }

    sentinel = TRAP_START_SENTINEL;
    if (!tig_file_fwrite(&sentinel, sizeof(sentinel), 1, stream) != 1) {
        tig_debug_printf("TRAP: mp_save: FAILURE: could not write START sentinel.\n");
        return false;
    }

    cnt = 0;
    node = dword_5FC48C;
    while (node != NULL) {
        cnt++;
        node = node->next;
    }

    if (!tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        tig_debug_printf("TRAP: mp_save: FAILURE: could not write count.\n");
        return false;
    }

    node = dword_5FC48C;
    while (node != NULL) {
        if (tig_file_fwrite(&(node->pc_oid), sizeof(node->pc_oid), 1, stream) != 1) {
            tig_debug_printf("TRAP: mp_save: FAILURE: could not write pc_id for entry %d.\n", cnt);
            return false;
        }

        if (tig_file_fwrite(&(node->trap_oid), sizeof(node->pc_oid), 1, stream) != 1) {
            tig_debug_printf("TRAP: mp_save: FAILURE: could not write trap_id for entry %d.\n", cnt);
            return false;
        }

        cnt--;
        node = node->next;
    }

    sentinel = TRAP_END_SENTINEL;
    if (!tig_file_fwrite(&sentinel, sizeof(sentinel), 1, stream) != 1) {
        tig_debug_printf("TRAP: mp_save: FAILURE: could not write END sentinel.\n");
        return false;
    }

    return true;
}

// 0x4BD850
void sub_4BD850()
{
    // TODO: Incomplete.
}

// 0x4BD950
int64_t sub_4BD950(int64_t obj)
{
    if (obj != OBJ_HANDLE_NULL
        && (obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_TRAP_PC) != 0) {
        return player_get_pc_obj();
    }

    return OBJ_HANDLE_NULL;
}
