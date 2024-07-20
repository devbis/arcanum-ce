#include "game/reaction.h"

#include "game/mes.h"
#include "game/obj.h"

// 0x5FC88C
static mes_file_handle_t reaction_mes_file;

// 0x5FC890
static char* reaction_names[REACTION_COUNT];

// 0x4C0BD0
bool reaction_init(GameInitInfo* init_info)
{
    MesFileEntry mes_file_entry;
    int index;

    (void)init_info;

    if (!mes_load("mes\\reaction.mes", &reaction_mes_file)) {
        return false;
    }

    for (index = 0; index < REACTION_COUNT; index++) {
        mes_file_entry.num = index;
        mes_get_msg(reaction_mes_file, &mes_file_entry);
        reaction_names[index] = mes_file_entry.str;
    }

    return true;
}

// 0x4C0C30
void reaction_exit()
{
    mes_unload(reaction_mes_file);
}

// 0x4C0C40
bool sub_4C0C40(int64_t pc, int64_t npc)
{
    int v1;

    if (pc == npc) {
        return true;
    }

    if (obj_field_int32_get(pc, OBJ_F_TYPE) != OBJ_TYPE_PC
        || obj_field_int32_get(npc, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return false;
    }

    return sub_4C12F0(pc, npc, false, &v1);
}

// 0x4C0CC0
int sub_4C0CC0(int64_t a1, int64_t a2)
{
    return sub_4C0D00(a1, a2, 0);
}

// 0x4C0CE0
int sub_4C0CE0(int64_t a1, int64_t a2)
{
    return sub_4C0D00(a1, a2, 1);
}

// 0x4C0FC0
int reaction_translate(int value)
{
    if (value <= 0) return REACTION_HATRED;
    if (value <= 20) return REACTION_DISLIKE;
    if (value <= 40) return REACTION_SUSPICIOUS;
    if (value <= 60) return REACTION_NEUTRAL;
    if (value <= 80) return REACTION_COURTEOUS;
    if (value <= 100) return REACTION_AMIABLE;
    return REACTION_LOVE;
}

// 0x4C1010
const char* reaction_get_name(int reaction)
{
    return reaction_names[reaction];
}

// 0x4C1020
void sub_4C1020(int64_t pc, int64_t npc)
{
    int v1;

    if (obj_field_int32_get(pc, OBJ_F_TYPE) != OBJ_TYPE_PC
        || obj_field_int32_get(npc, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return;
    }

    v1 = sub_4C1290(pc, npc);
    if (v1 < 100 && sub_4F5270(npc, 1)) {
        v1 = 100;
    }

    sub_4C1490(pc, npc, v1, 0);
}

// 0x4C10A0
void sub_4C10A0(int64_t pc, int64_t npc)
{
    int v1;

    if (obj_field_int32_get(pc, OBJ_F_TYPE) != OBJ_TYPE_PC
        || obj_field_int32_get(npc, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return;
    }

    v1 = sub_4C1290(pc, npc);
    sub_4C1490(pc, 0, v1, 0);
    sub_4C1360(pc, npc, v1);
}

// 0x4C1110
int64_t sub_4C1110(int64_t npc)
{
    if (obj_field_int32_get(npc, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    return obj_arrayfield_handle_get(npc, OBJ_F_NPC_REACTION_PC_IDX, 0);
}
