#include "game/trap.h"

#include "game/animfx.h"
#include "game/critter.h"
#include "game/mes.h"
#include "game/obj.h"
#include "game/player.h"

typedef struct TrapListNode {
    ObjectID field_0;
    ObjectID field_18;
    struct TrapListNode* next;
} TrapListNode;

static_assert(sizeof(TrapListNode) == 0x38, "wrong size");

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

    if (animfx_list_init(&stru_5FC460)) {
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
bool sub_4BBDD0(int64_t obj)
{
    int scr[3];

    sub_407840(obj, OBJ_F_SCRIPTS_IDX, 1, scr);
    return sub_4BBE00(scr);
}

// 0x4BBE00
void sub_4BBE00()
{
    // TODO: Incomplete.
}

// 0x4BBE40
void sub_4BBE40()
{
    // TODO: Incomplete.
}

// 0x4BBFE0
bool sub_4BBFE0(int64_t a1, int64_t a2)
{
    int type;
    unsigned int flags;

    if (a1 == OBJ_HANDLE_NULL) {
        return false;
    }

    type = obj_field_int32_get(a1, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return false;
    }

    flags = obj_field_int32_get(a2, OBJ_F_FLAGS);
    if (type != OBJ_TYPE_PC && !sub_45DDA0(a1)) {
        return (flags & OF_TRAP_PC) == 0;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        return sub_4BD430(a1, a2);
    }

    if ((flags & (OF_TRAP_PC | OF_TRAP_SPOTTED)) == 0) {
        return false;
    }

    return true;
}

// 0x4BC090
void sub_4BC090()
{
    // TODO: Incomplete.
}

// 0x4BC220
void sub_4BC220()
{
    // TODO: Incomplete.
}

// 0x4BC2E0
void sub_4BC2E0()
{
    // TODO: Incomplete.
}

// 0x4BC480
void sub_4BC480()
{
    // TODO: Incomplete.
}

// 0x4BC690
void sub_4BC690(int a1, int64_t a2, int delay, int64_t a4)
{
    TimeEvent timeevent;
    DateTime datetime;

    if (delay != 0) {
        timeevent.type = TIMEEVENT_TYPE_TRAPS;
        timeevent.params[0].integer_value = a1;
        timeevent.params[1].object_value = a2;
        timeevent.params[2].object_value = a4;
        sub_45A950(&datetime, 1000 * delay);
        sub_45B800(&timeevent, &datetime);
    } else {
        sub_455A20(v1, OBJ_HANDLE_NULL, a1);
        sub_455AC0(v1);

        if (a4 != OBJ_HANDLE_NULL) {
            sub_43CCA0(a4);
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
void sub_4BC7B0()
{
    // TODO: Incomplete.
}

// 0x4BC9C0
void sub_4BC9C0()
{
    // TODO: Incomplete.
}

// 0x4BCA60
bool trap_is_trap_device(int64_t obj)
{
    return obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_ITEM_GENERIC
        && (obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) & OGF_IS_TRAP_DEVICE) != 0;
}

// 0x4BCAB0
void sub_4BCAB0(int64_t obj)
{
    ObjectNodeList ol;
    int64_t first_obj;

    sub_4407C0(obj, 0x20000, &ol);
    if (ol.head != NULL) {
        first_obj = ol.head->obj;
    } else {
        first_obj = OBJ_HANDLE_NULL;
    }

    object_list_destroy(&ol);

    return first_obj;
}

// 0x4BCB00
void sub_4BCB00(int64_t a1, int64_t a2)
{
    int scr[3];
    ScriptInvocation invocation;

    sub_407840(a2, OBJ_F_SCRIPTS_IDX, 1, scr);

    invocation.triggerer_obj = a1;
    invocation.extra_obj = OBJ_HANDLE_NULL;
    invocation.field_4 = 0;
    invocation.field_20 = 1;
    invocation.attachee_obj = a2;
    invocation.field_0 = scr;
    sub_4449B0(&invocation);
}

// 0x4BCB70
void sub_4BCB70()
{
    // TODO: Incomplete.
}

// 0x4BCDC0
void sub_4BCDC0()
{
    // TODO: Incomplete.
}

// 0x4BD150
void sub_4BD150()
{
    // TODO: Incomplete.
}

// 0x4BD1E0
TrapListNode* sub_4BD1E0(int64_t a1, int64_t a2)
{
    TrapListNode* node;

    node = (TrapListNode*)MALLOC(sizeof(*node));
    node->field_18 = sub_407EF0(a1);
    node->field_0 = sub_407EF0(a2);
    node->next = dword_5FC48C;
    dword_5FC48C = node;

    return node;
}

// 0x4BD340
void sub_4BD340()
{
    // TODO: Incomplete.
}

// 0x4BD430
void sub_4BD430()
{
    // TODO: Incomplete.
}

// 0x4BD480
void sub_4BD480()
{
    // TODO: Incomplete.
}

// 0x4BD550
void mp_load()
{
    // TODO: Incomplete.
}

// 0x4BD710
void mp_save()
{
    // TODO: Incomplete.
}

// 0x4BD850
void sub_4BD850()
{
    // TODO: Incomplete.
}

// 0x4BD950
void sub_4BD950(int64_t obj)
{
    if (obj != OBJ_HANDLE_NULL
        && (obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_TRAP_PC) != 0) {
        return player_get_pc_obj();
    }

    return OBJ_HANDLE_NULL;
}
