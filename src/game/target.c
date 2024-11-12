#include "game/target.h"

#include "game/critter.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/name.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/random.h"
#include "game/stat.h"

static bool sub_4F28A0(int x, int y, S4F2810* a3);
static void sub_4F3F10(S603CB8_F50* a1, int64_t obj);
static void sub_4F3FD0(S603CB8_F50* a1, int64_t obj);
static void sub_4F4050(S603CB8_F50* a1, int64_t a2);
static void sub_4F52D0(int64_t obj, int index);

// 0x5BC428
const char* off_5BC428[] = {
    "resist_damage",
    "resist_fire",
    "resist_electrical",
    "resist_poison",
    "resist_magic",
};

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

// 0x603B88
static PathCreateInfo stru_603B88;

// 0x603BB0
static TigRect stru_603BB0;

// 0x603BC4
static int8_t byte_603BC4[200];

// 0x603C94
static int dword_603C94;

// 0x603C98
static int dword_603C98;

// 0x603CA8
static int64_t qword_603CA8;

// 0x603CB0
static int64_t qword_603CB0;

// 0x603CB8
static S603CB8 stru_603CB8;

// 0x603D20
static S603D20 stru_603D20;

// 0x603D38
static bool target_initialized;

// 0x603D40
static int64_t qword_603D40;

// 0x603D48
static void(*dword_603D48)(int64_t obj, int index);

// 0x4F24F0
bool target_init(GameInitInfo* init_info)
{
    TigWindowData window_data;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    stru_603BB0 = window_data.rect;
    sub_4F2600(&stru_603CB8, &stru_603D20, OBJ_HANDLE_NULL);
    target_initialized = true;

    return true;
}

// 0x4F2560
void target_exit()
{
    target_initialized = false;
}

// 0x4F2570
void target_reset()
{
    qword_603D40 = OBJ_HANDLE_NULL;
}

// 0x4F2580
void target_resize(ResizeInfo* resize_info)
{
    stru_603BB0 = resize_info->field_14;
}

// 0x4F25B0
void sub_4F25B0(uint64_t flags)
{
    stru_603D20.field_0 = flags;
}

// 0x4F25D0
uint64_t sub_4F25D0()
{
    return stru_603D20.field_0;
}

// 0x4F25E0
void sub_4F25E0(S603D20* a1)
{
    a1->field_0 = Tgt_None;
    a1->field_14 = 0;
    a1->field_10 = 0;
    a1->field_C = 0;
    a1->field_8 = 0;
}

// 0x4F2600
void sub_4F2600(S603CB8 *a1, S603D20 *a2, int64_t a3)
{
    a1->field_38 = 0;
    a1->field_30 = 0;
    a1->field_40 = 0;
    a1->field_10 = a3;
    a1->field_8 = a3;
    a1->field_0 = a2;
    a1->field_54 = 0;
    a1->field_58 = 0;
    a1->field_50 = 0;
    if (a3 != OBJ_HANDLE_NULL) {
        a1->field_18 = obj_field_int64_get(a3, OBJ_F_LOCATION);
    }
    a1->field_48 = 0;
    a1->field_28 = 0;
    a1->field_20 = 0;
    a1->field_4C = 0;
    a1->field_60 = 0;

    if (a2 != NULL) {
        sub_4F25E0(a2);
    }
}

// 0x4F2680
bool sub_4F2680(S4F2680* a1)
{
    stru_603CB8.field_10 = a1->field_0;

    if (a1->field_10->field_8) {
        stru_603CB8.field_38 = a1->field_10->field_0;
        stru_603CB8.field_28 = a1->field_10->field_0;
        stru_603CB8.field_20 = 0;
        stru_603CB8.field_30 = 0;
    } else {
        if ((obj_field_int32_get(a1->field_10->field_0, OBJ_F_FLAGS) & OF_CLICK_THROUGH) != 0) {
            return false;
        }

        stru_603CB8.field_30 = a1->field_10->field_0;
        stru_603CB8.field_20 = a1->field_10->field_0;
        stru_603CB8.field_28 = OBJ_HANDLE_NULL;
        stru_603CB8.field_38 = OBJ_HANDLE_NULL;
    }

    stru_603CB8.field_8 = a1->field_8;

    if (sub_4F2D20(&stru_603CB8)) {
        if (stru_603D20.field_0) {
            return true;
        }
        return false;
    }

    if (!a1->field_10->field_8) {
        if (a1->field_10->field_0 != OBJ_HANDLE_NULL) {
            stru_603CB8.field_28 = obj_field_int64_get(a1->field_10->field_0, OBJ_F_LOCATION);
            stru_603CB8.field_38 = stru_603CB8.field_28;
        }
        if (sub_4F2D20(&stru_603CB8) && stru_603D20.field_0) {
            sub_4F27F0(a1->field_10, stru_603CB8.field_28);
            return true;
        }
    }

    return false;
}

// 0x4F27F0
void sub_4F27F0(S4F2810* a1, int64_t loc)
{
    a1->field_0 = loc;
    a1->field_8 = 1;
}

// 0x4F2810
void sub_4F2810(S4F2810* a1, int64_t obj)
{
    a1->field_0 = obj;
    a1->field_8 = 0;
}

// 0x4F2830
bool sub_4F2830(TigMouseMessageData* mouse, S4F2810* a2, bool fullscreen)
{
    int x;
    int y;

    if (mouse->x < stru_603BB0.x
        || mouse->x >= stru_603BB0.x + stru_603BB0.width
        || mouse->y < stru_603BB0.y
        || mouse->y >= stru_603BB0.y + stru_603BB0.height) {
        return false;
    }

    x = mouse->x - stru_603BB0.x;
    y = mouse->y;

    // FIXME: Looks odd, why `x` is not treat in the same way?
    if (!fullscreen) {
        y -= stru_603BB0.y;
    }

    return sub_4F28A0(x, y, a2);
}

// 0x4F28A0
bool sub_4F28A0(int x, int y, S4F2810* a3)
{
    int64_t pc_obj;
    S4F2680 v1;
    ObjectList party_members;
    ObjectList mp_party_members;
    ObjectList dead_critters;
    ObjectList objects;
    ObjectNode* node;
    int64_t v2;
    int64_t loc;
    bool ret = false;

    pc_obj = player_get_pc_obj();

    v1.field_0 = pc_obj;
    v1.field_8 = pc_obj;
    v1.field_10 = a3;

    if ((stru_603D20.field_0 & Tgt_No_Self) != 0) {
        sub_43D0E0(pc_obj, OF_CANCEL);
    }

    if ((stru_603D20.field_0 & Tgt_Non_Party) != 0) {
        sub_441260(pc_obj, &party_members);
        node = party_members.head;
        while (node != NULL) {
            sub_43D0E0(node->obj, OF_CANCEL);
            node = node->next;
        }

        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            sub_441310(pc_obj, &mp_party_members);
            node = mp_party_members.head;
            while (node != NULL) {
                sub_43D0E0(node->obj, OF_CANCEL);
                node = node->next;
            }
        }
    }

    if ((stru_603D20.field_0 & Tgt_No_ST_Critter_Dead) != 0) {
        sub_440FC0(pc_obj, OBJ_TM_PC | OBJ_TM_NPC, &dead_critters);
        node = mp_party_members.head;
        while (node != NULL) {
            if (sub_45D8D0(node->obj)) {
                sub_43D0E0(node->obj, OF_CANCEL);
            }
            node = node->next;
        }
    }

    if (sub_43D9F0(x, y, &v2, 0x3)) {
        sub_4F2C60(&v2);
        sub_4F2810(a3, v2);
        if (sub_4F2680(&v1)) {
            ret = true;
        }
    }

    if (!ret) {
        if (sub_4B8730(x, y, &loc)) {
            sub_4407C0(loc, OBJ_TM_ALL & ~OBJ_TM_PROJECTILE, &objects);
            node = objects.head;
            while (node != NULL) {
                v2 = node->obj;
                sub_4F2C60(&v2);
                sub_4F2810(a3, v2);
                if (sub_4F2680(&v1)) {
                    ret = true;
                    break;
                }
                node = node->next;
            }
            object_list_destroy(&objects);

            if (!ret) {
                sub_4F27F0(a3, loc);
                if (sub_4F2680(&v1)) {
                    ret = true;
                }
            }
        }
    }


    if ((stru_603D20.field_0 & Tgt_No_Self) != 0) {
        sub_43D280(pc_obj, OF_CANCEL);
    }

    if ((stru_603D20.field_0 & Tgt_Non_Party) != 0) {
        node = party_members.head;
        while (node != NULL) {
            sub_43D280(node->obj, OF_CANCEL);
            node = node->next;
        }

        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            node = mp_party_members.head;
            while (node != NULL) {
                sub_43D280(node->obj, OF_CANCEL);
                node = node->next;
            }
        }
    }

    if ((stru_603D20.field_0 & Tgt_No_ST_Critter_Dead) != 0) {
        node = mp_party_members.head;
        while (node != NULL) {
            if (sub_45D8D0(node->obj)) {
                sub_43D280(node->obj, OF_CANCEL);
            }
            node = node->next;
        }
    }

    if (!ret) {
        sub_4F2810(a3, OBJ_HANDLE_NULL);
        return false;
    }

    return true;
}

// 0x4F2C60
int sub_4F2C60(int64_t* obj_ptr)
{
    int type;
    int64_t whos_in_me_obj;

    type = obj_field_int32_get(*obj_ptr, OBJ_F_TYPE);
    if (type == OBJ_TYPE_SCENERY) {
        whos_in_me_obj = obj_field_int32_get(*obj_ptr, OBJ_F_SCENERY_WHOS_IN_ME);
        if (whos_in_me_obj != OBJ_HANDLE_NULL) {
            *obj_ptr = whos_in_me_obj;
            return OBJ_TYPE_NPC;
        }
    }

    return type;
}

// 0x4F2CB0
bool sub_4F2CB0(int x, int y, S4F2810* a3, uint64_t tgt, bool fullscreen)
{
    uint64_t old_tgt;
    bool rc;

    x -= stru_603BB0.x;
    if (!fullscreen) {
        y -= stru_603BB0.y;
    }

    old_tgt = sub_4F25D0();
    sub_4F25B0(tgt);
    rc = sub_4F28A0(x, y, a3);
    sub_4F25B0(tgt);
    return rc;
}

// 0x4F2D10
int64_t sub_4F2D10()
{
    return qword_603D40;
}

// 0x4F2D20
bool sub_4F2D20(S603CB8* a1)
{
    // TODO: Incomplete.
}

// 0x4F3F10
void sub_4F3F10(S603CB8_F50* a1, int64_t obj)
{
    int idx;

    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
            return;
        }

        if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2) & OCF2_SAFE_OFF) == 0) {
            return;
        }
    }

    for (idx = 0; idx < a1->cnt; idx++) {
        if (a1->entries[idx].field_0 == obj) {
            return;
        }
    }

    a1->entries[a1->cnt].field_0 = obj;
    a1->entries[a1->cnt].field_8 = 0;
    a1->cnt++;
}

// 0x4F3FD0
void sub_4F3FD0(S603CB8_F50* a1, int64_t obj)
{
    int idx;

    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_DESTROYED) != 0) {
        return;
    }

    for (idx = 0; idx < a1->cnt; idx++) {
        if (a1->entries[idx].field_0 == obj) {
            return;
        }
    }

    a1->entries[a1->cnt].field_0 = obj;
    a1->entries[a1->cnt].field_8 = 0;
    a1->cnt++;
}

// 0x4F4050
void sub_4F4050(S603CB8_F50* a1, int64_t a2)
{
    int idx;

    for (idx = 0; idx < a1->cnt; idx++) {
        if (a1->entries[idx].field_8 == a2) {
            return;
        }
    }

    a1->entries[a1->cnt].field_0 = OBJ_HANDLE_NULL;
    a1->entries[a1->cnt].field_8 = a2;
    a1->cnt++;
}

// 0x4F40B0
void sub_4F40B0(S603CB8* a1)
{
    // TODO: Incomplete.
}

// 0x4F4E40
bool sub_4F4E40(int64_t obj, int distance, int64_t* loc_ptr)
{
    int64_t from;
    int64_t to;
    int obj_type;
    int rotation;
    int idx;
    ObjectList critters;
    ObjectNode* node;

    from = obj_field_int64_get(obj, OBJ_F_LOCATION);
    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    *loc_ptr = 0;
    to = from;
    rotation = random_between(0, 8);

    for (idx = 0; idx < distance; idx++) {
        if (!sub_4B8FF0(to, rotation, &to)) {
            return false;
        }
    }

    stru_603B88.obj = obj;
    stru_603B88.from = from;
    stru_603B88.to = to;
    stru_603B88.max_rotations = sizeof(byte_603BC4);
    stru_603B88.rotations = byte_603BC4;
    stru_603B88.field_20 = 16;

    dword_603C98 = sub_41F3C0(&stru_603B88);
    qword_603CB0 = stru_603B88.to;
    qword_603CA8 = stru_603B88.from;

    if ((dword_603C98 > 0 || stru_603B88.field_24 > 0)
        && dword_603C98 < distance + 2) {
        if (stru_603B88.field_24 == 0) {
            stru_603B88.field_24 = dword_603C98;
        }

        to = from;

        dword_603C94 = 0;
        while (dword_603C94 < stru_603B88.field_24) {
            if (obj_type_is_critter(obj_type)) {
                sub_4407C0(to, OBJ_TM_CRITTER, &critters);
                node = critters.head;
                object_list_destroy(&critters);
                if (node == NULL) {
                    break;
                }
            }

            if (!sub_4B8FF0(to, byte_603BC4[dword_603C94], &to)) {
                return false;
            }
        }
        *loc_ptr = to;
        return true;
    } else {
        *loc_ptr = from;
        return true;
    }
}

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
        object_set_hp_damage(obj, 0);
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

// 0x4F5330
void sub_4F5330(int64_t a1, int64_t a2)
{
    int desc;
    int value;

    desc = critter_description_get(a2, a1);

    value = sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 0);
    sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 0, value + 1);

    value = stat_level(a2, STAT_LEVEL);
    if (value >= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 2)
        || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 1) == 0) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 2, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 1, desc);
    }
    if (value >= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 4)
        || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 3) == 0) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 4, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 3, desc);
    }

    value = stat_level(a2, STAT_ALIGNMENT);
    if (value > 0
        && (value >= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 6)
            || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 5) == 0)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 6, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 5, desc);
    }
    if (value < 0
        && (value <= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 8)
            || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 7) == 0)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 8, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 7, desc);
    }

    value = stat_level(a2, STAT_MAGICK_TECH_APTITUDE);
    if (value > 0
        && (value >= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 10)
            || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 9) == 0)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 10, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 9, desc);
    }
    if (value < 0
        && (value <= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 12)
            || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 11) == 0)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 12, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 11, desc);
    }
}

// 0x4F5590
void sub_4F5590(int64_t obj, int* a2)
{
    a2[0] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 0);
    a2[1] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 1);
    a2[2] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 2);
    a2[3] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 3);
    a2[4] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 4);
    a2[5] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 5);
    a2[6] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 6);
    a2[7] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 7);
    a2[8] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 8);
    a2[9] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 9);
    a2[10] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 10);
    a2[11] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 11);
    a2[12] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 12);
}

// 0x4F5690
void sub_4F5690(int64_t a1, int64_t a2, int a3)
{
    int type;
    int desc;
    int index;

    if (a2 == OBJ_HANDLE_NULL) {
        return;
    }

    if (obj_field_int32_get(a1, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    type = obj_field_int32_get(a2, OBJ_F_TYPE);
    if (type != OBJ_TYPE_PC && type != OBJ_TYPE_NPC) {
        return;
    }

    if (a1 == a2) {
        desc = 30286;
    } else if (type == OBJ_TYPE_PC) {
        desc = 30287;
    } else {
        desc = critter_description_get(a2, a1);
    }

    index = 64;
    while (sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, index)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, index++, desc);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, index++, a3);
    }
}

// 0x4F5770
int sub_4F5770(int64_t obj, int* a2, int* a3)
{
    *a2 = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 64);
    if (*a2 == 0) {
        return 0;
    }

    *a3 = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 65);
    return 64;
}

// 0x4F57C0
int sub_4F57C0(int64_t obj, int a2, int* a3, int* a4)
{
    *a3 = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, a2 + 2);
    if (*a3 == 0) {
        return 0;
    }

    *a4 = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, a2 + 3);
    return a2 + 2;
}
