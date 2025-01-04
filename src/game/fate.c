#include "game/fate.h"

#include "game/critter.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj.h"
#include "game/stat.h"

static void fate_apply(int64_t obj, int index);

// 0x5BC48C
static unsigned int fate_flags[FATE_COUNT] = {
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
static void(*fate_callback)(int64_t obj, int index);

// 0x4F5090
bool fate_is_activated(int64_t obj, int fate)
{
    return (obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE) & fate_flags[fate]) != 0;
}

// 0x4F50C0
bool fate_activate(int64_t obj, int fate)
{
    unsigned int flags;
    int fate_points;

    if (!multiplayer_is_locked()) {
        PacketFateStateSet pkt;

        pkt.type = 36;
        sub_4440E0(obj, &(pkt.oid));
        pkt.fate = fate;
        pkt.action = 1;
        tig_net_send_app_all(&pkt, sizeof(pkt));

        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return false;
        }
    }

    flags = obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE);
    if ((fate_flags[fate] & flags) != 0) {
        return false;
    }

    fate_points = stat_get_base(obj, STAT_FATE_POINTS);
    if (fate_points <= 0) {
        return false;
    }

    stat_set_base(obj, STAT_FATE_POINTS, fate_points - 1);

    if (fate_flags[fate] != 0) {
        obj_field_int32_set(obj, OBJ_F_PC_FLAGS_FATE, flags | fate_flags[fate]);
    } else {
        fate_apply(obj, fate);
    }

    return true;
}

// 0x4F51B0
bool fate_deactivate(int64_t obj, int fate)
{
    unsigned int flags;
    int fate_points;

    if (!multiplayer_is_locked()) {
        PacketFateStateSet pkt;

        pkt.type = 36;
        sub_4440E0(obj, &(pkt.oid));
        pkt.fate = fate;
        pkt.action = 0;
        tig_net_send_app_all(&pkt, sizeof(pkt));

        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return false;
        }
    }

    flags = obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE);
    if ((fate_flags[fate] & flags) == 0) {
        return false;
    }

    obj_field_int32_set(obj, OBJ_F_PC_FLAGS_FATE, flags & ~fate_flags[fate]);

    fate_points = stat_get_base(obj, STAT_FATE_POINTS);
    stat_set_base(obj, STAT_FATE_POINTS, fate_points + 1);

    return true;
}

// 0x4F5270
bool fate_resolve(int64_t obj, int fate)
{
    unsigned int flags;

    flags = obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE);
    if ((flags & fate_flags[fate]) == 0) {
        return false;
    }

    flags &= ~fate_flags[fate];
    obj_field_int32_set(obj, OBJ_F_PC_FLAGS_FATE, flags);

    if (fate_callback != NULL) {
        fate_callback(obj, fate);
    }

    return true;
}

// 0x4F52D0
void fate_apply(int64_t obj, int fate)
{
    if (fate == FATE_FULL_HEAL) {
        object_hp_damage_set(obj, 0);
        stat_set_base(obj, STAT_POISON_LEVEL, 0);
        critter_fatigue_damage_set(obj, 0);
    }

    if (fate_callback != NULL) {
        fate_callback(obj, fate);
    }
}

// 0x4F5320
void fate_set_callback(FateCallback* cb)
{
    fate_callback = cb;
}
