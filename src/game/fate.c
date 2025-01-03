#include "game/fate.h"

#include "game/critter.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj.h"
#include "game/stat.h"

static void sub_4F52D0(int64_t obj, int index);

// 0x5BC48C
static int dword_5BC48C[12] = {
    0x0000,
    0x0001,
    0x0002,
    0x0004,
    0x0008,
    0x0010,
    0x0020,
    0x0040,
    0x0080,
    0x0100,
    0x0200,
    0x0400,
};

// 0x603D48
static void(*dword_603D48)(int64_t obj, int index);

// 0x4F5090
bool sub_4F5090(int64_t obj, int index)
{
    return (obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE) & dword_5BC48C[index]) != 0;
}

// 0x4F50C0
bool sub_4F50C0(int64_t obj, int index)
{
    unsigned int flags;
    Packet36 pkt;
    int fate_points;

    if (!sub_4A2BA0()) {
        pkt.type = 36;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.field_38 = index;
        pkt.field_3C = 1;
        tig_net_send_app_all(&pkt, sizeof(pkt));

        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return false;
        }
    }

    flags = obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE);
    if ((dword_5BC48C[index] & flags) != 0) {
        return false;
    }

    fate_points = stat_get_base(obj, STAT_FATE_POINTS);
    if (fate_points <= 0) {
        return false;
    }

    stat_set_base(obj, STAT_FATE_POINTS, fate_points - 1);

    if (dword_5BC48C[index] != 0) {
        obj_field_int32_set(obj, OBJ_F_PC_FLAGS_FATE, flags | dword_5BC48C[index]);
    } else {
        sub_4F52D0(obj, index);
    }

    return true;
}

// 0x4F51B0
bool sub_4F51B0(int64_t obj, int index)
{
    unsigned int flags;
    Packet36 pkt;
    int fate_points;

    if (!sub_4A2BA0()) {
        pkt.type = 36;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.field_38 = index;
        pkt.field_3C = 0;
        tig_net_send_app_all(&pkt, sizeof(pkt));

        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return false;
        }
    }

    flags = obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE);
    if ((dword_5BC48C[index] & flags) == 0) {
        return false;
    }

    obj_field_int32_set(obj, OBJ_F_PC_FLAGS_FATE, flags & ~dword_5BC48C[index]);

    fate_points = stat_get_base(obj, STAT_FATE_POINTS);
    stat_set_base(obj, STAT_FATE_POINTS, fate_points + 1);

    return true;
}

// 0x4F5270
bool sub_4F5270(int64_t obj, int index)
{
    unsigned int flags;

    flags = obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE);
    if ((flags & dword_5BC48C[index]) == 0) {
        return false;
    }

    flags &= ~dword_5BC48C[index];
    obj_field_int32_set(obj, OBJ_F_PC_FLAGS_FATE, flags);

    if (dword_603D48 != NULL) {
        dword_603D48(obj, index);
    }

    return true;
}

// 0x4F52D0
void sub_4F52D0(int64_t obj, int index)
{
    if (index == 0) {
        object_hp_damage_set(obj, 0);
        stat_set_base(obj, STAT_POISON_LEVEL, 0);
        critter_fatigue_damage_set(obj, 0);
    }

    if (dword_603D48 != NULL) {
        dword_603D48(obj, index);
    }
}

// 0x4F5320
void sub_4F5320(void(*func)(int64_t obj, int index))
{
    dword_603D48 = func;
}
