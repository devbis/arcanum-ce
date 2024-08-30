#include "game/ai.h"

#include "game/critter.h"
#include "game/obj.h"
#include "game/stat.h"

#define CLOCKWORK_DECOY 6719

static void ai_danger_source(int64_t obj, int* type_ptr, int64_t* danger_source_ptr);
static int sub_4AF240(int value);
static bool sub_4AF800(int64_t obj, int64_t a2);

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
void sub_4A84D0()
{
    // TODO: Incomplete.
}

// 0x4A84F0
void sub_4A84F0()
{
    // TODO: Incomplete.
}

// 0x4A8570
void sub_4A8570()
{
    // TODO: Incomplete.
}

// 0x4A88D0
void sub_4A88D0()
{
    // TODO: Incomplete.
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
void sub_4A94C0()
{
    // TODO: Incomplete.
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
void sub_4A9AD0()
{
    // TODO: Incomplete.
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
void sub_4A9F10()
{
    // TODO: Incomplete.
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
void sub_4AA420()
{
    // TODO: Incomplete.
}

// 0x4AA4A0
void sub_4AA4A0()
{
    // TODO: Incomplete.
}

// 0x4AA580
void sub_4AA580()
{
    // TODO: Incomplete.
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
void sub_4AA8C0()
{
    // TODO: Incomplete.
}

// 0x4AA990
void ai_npc_wait_here_timeevent_process()
{
    // TODO: Incomplete.
}

// 0x4AAA30
void sub_4AAA30()
{
    // TODO: Incomplete.
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
void sub_4AB2A0()
{
    // TODO: Incomplete.
}

// 0x4AB2F0
void sub_4AB2F0()
{
    // TODO: Incomplete.
}

// 0x4AB400
void sub_4AB400()
{
    // TODO: Incomplete.
}

// 0x4AB430
void sub_4AB430()
{
    // TODO: Incomplete.
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
void sub_4ABBC0()
{
    // TODO: Incomplete.
}

// 0x4ABC20
void sub_4ABC20()
{
    // TODO: Incomplete.
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
void sub_4ABEB0()
{
    // TODO: Incomplete.
}

// 0x4ABF10
void sub_4ABF10()
{
    // TODO: Incomplete.
}

// 0x4AC180
void sub_4AC180()
{
    // TODO: Incomplete.
}

// 0x4AC250
void sub_4AC250()
{
    // TODO: Incomplete.
}

// 0x4AC320
void sub_4AC320()
{
    // TODO: Incomplete.
}

// 0x4AC350
void sub_4AC350()
{
    // TODO: Incomplete.
}

// 0x4AC380
void sub_4AC380()
{
    // TODO: Incomplete.
}

// 0x4AC620
void sub_4AC620()
{
    // TODO: Incomplete.
}

// 0x4AC660
void sub_4AC660()
{
    // TODO: Incomplete.
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
void sub_4ACD90()
{
    // TODO: Incomplete.
}

// 0x4ACDB0
void sub_4ACDB0()
{
    // TODO: Incomplete.
}

// 0x4AD060
void sub_4AD060()
{
    // TODO: Incomplete.
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
void sub_4AD1B0()
{
    // TODO: Incomplete.
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
void sub_4AD5D0()
{
    // TODO: Incomplete.
}

// 0x4AD610
void sub_4AD610()
{
    // TODO: Incomplete.
}

// 0x4AD6B0
void sub_4AD6B0()
{
    // TODO: Incomplete.
}

// 0x4AD6E0
void sub_4AD6E0()
{
    // TODO: Incomplete.
}

// 0x4AD700
void sub_4AD700()
{
    // TODO: Incomplete.
}

// 0x4AD730
void sub_4AD730()
{
    // TODO: Incomplete.
}

// 0x4AD790
void sub_4AD790()
{
    // TODO: Incomplete.
}

// 0x4AD7D0
void sub_4AD7D0()
{
    // TODO: Incomplete.
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
