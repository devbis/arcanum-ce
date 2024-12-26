#include "game/wallcheck.h"

#include "game/object.h"
#include "game/player.h"
#include "game/roof.h"
#include "game/sector.h"

typedef struct S5E0A10 {
    /* 0000 */ int64_t field_0;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
} S5E0A10;

static_assert(sizeof(S5E0A10) == 0x10, "wrong size");

typedef struct S5E0E20 {
    /* 0000 */ unsigned int flags;
    /* 0008 */ int64_t sector_id;
    /* 0010 */ int64_t obj;
    /* 0018 */ int64_t field_18;
} S5E0E20;

static_assert(sizeof(S5E0E20) == 0x20, "wrong size");

static void sub_438570(int64_t obj, int64_t sector_id, int type);
static bool sub_4386B0(int64_t obj, int* index_ptr);
static void sub_438720(int64_t a1);
static bool sub_4387C0(int64_t a1, int* index_ptr);
static void sub_438830();
static void wallcheck_roof_faded_clear(int64_t a1);

// 0x5A3E90
static bool dword_5A3E90 = true;

// 0x5E0A00
static GameContextF8* dword_5E0A00;

// 0x5E0A08
static int64_t qword_5E0A08;

// 0x5E0A10
static S5E0A10 stru_5E0A10[64];

// 0x5E0E10
static int dword_5E0E10;

// 0x5E0E18
static int64_t qword_5E0E18;

// 0x5E0E20
static S5E0E20 stru_5E0E20[256];

// 0x5E2E24
static int dword_5E2E24;

// 0x5E2E28
static int dword_5E2E28;

// 0x5DE6E8
static bool dword_5DE6E8;

// 0x437D90
bool wallcheck_init(GameInitInfo* init_info)
{
    dword_5E2E24 = 0;
    dword_5E2E28 = 0;
    dword_5E0A00 = init_info->field_8;

    return true;
}

// 0x437DB0
void wallcheck_reset()
{
    int index;

    if (dword_5E2E24 > 0) {
        for (index = 0; index < dword_5E2E24; index++) {
            stru_5E0E20[index].flags &= ~0x2;
            stru_5E0E20[index].flags |= 0x1;
        }

        sub_438830();
    }

    qword_5E0E18 = 0;
    dword_5E0E10 = 0;
    dword_5DE6E8 = 0;
}

// 0x437E00
void wallcheck_exit()
{
    wallcheck_reset();
    dword_5E0A00 = 0;
}

// 0x437E10
void sub_437E10()
{
    if (dword_5DE6E8) {
        sub_439D30(qword_5E0A08);
        sub_437E50(qword_5E0A08);
        dword_5DE6E8 = false;
    }
}

// 0x437E50
void sub_437E50(int64_t a1)
{
    // TODO: Incomplete.
}

// 0x4384F0
void wallcheck_flush()
{
    wallcheck_reset();
}

// 0x438500
void sub_438500(bool enabled)
{
    if (dword_5A3E90 != enabled) {
        dword_5A3E90 = enabled;
    }
}

// 0x438520
bool sub_438520()
{
    return dword_5A3E90;
}

// 0x438530
void sub_438530(int64_t obj)
{
    if (player_is_pc_obj(obj)) {
        qword_5E0A08 = obj_field_int64_get(obj, OBJ_F_LOCATION);
        dword_5DE6E8 = true;
    }
}

// 0x438570
void sub_438570(int64_t obj, int64_t sector_id, int type)
{
    int index;
    Sector* sector;

    if (!sub_4386B0(obj, &index)) {
        memcpy(&(stru_5E0E20[index + 1]),
            &(stru_5E0E20[index]),
            sizeof(*stru_5E0E20) * (dword_5E2E24 - index));
        stru_5E0E20[index].flags = 0;
        stru_5E0E20[index].sector_id = sector_id;
        stru_5E0E20[index].obj = obj;
        stru_5E0E20[index].field_18 = sub_4395E0(obj_field_int64_get(obj, OBJ_F_LOCATION));

        sector_lock(sector_id, &sector);
        sub_438720(stru_5E0E20[index].field_18);
        dword_5E2E24++;
    }

    if (type == 1 || type == 16) {
        stru_5E0E20[index].flags = 0x04 | 0x02;
    } else if (type == 2 || type == 32) {
        stru_5E0E20[index].flags = 0x08 | 0x02;
    } else {
        stru_5E0E20[index].flags = 0x08 | 0x04 | 0x02;
    }
}

// 0x4386B0
bool sub_4386B0(int64_t obj, int* index_ptr)
{
    int l;
    int r;
    int m;
    int64_t curr;

    l = 0;
    r = dword_5E2E24 - 1;
    while (l <= r) {
        m = (l + r) / 2;
        curr = stru_5E0E20[m].obj;

        if (curr < obj) {
            l = m + 1;
        } else if (curr > obj) {
            r = m - 1;
        } else {
            *index_ptr = m;
            return true;
        }
    }

    *index_ptr = l;
    return false;
}

// 0x438720
void sub_438720(int64_t a1)
{
    int index;

    if (!sub_4387C0(a1, &index)) {
        memcpy(&(stru_5E0A10[index + 1]),
            &(stru_5E0A10[index]),
            sizeof(*stru_5E0A10) * (dword_5E2E28 - index));
        stru_5E0A10[index].field_0 = a1;
        stru_5E0A10[index].field_8 = 0;
        dword_5E2E28++;
    }

    if (++stru_5E0A10[index].field_8 == 1) {
        sub_439EE0(a1);
    }
}

// 0x4387C0
bool sub_4387C0(int64_t a1, int* index_ptr)
{
    int l;
    int r;
    int m;
    int64_t curr;

    l = 0;
    r = dword_5E2E28 - 1;
    while (l <= r) {
        m = (l + r) / 2;
        curr = stru_5E0A10[m].field_0;

        if (curr < a1) {
            l = m + 1;
        } else if (curr > a1) {
            r = m - 1;
        } else {
            *index_ptr = m;
            return true;
        }
    }

    *index_ptr = l;
    return false;
}

// 0x438830
void sub_438830()
{
    int idx;
    unsigned int wall_flags;
    unsigned int render_flags;
    TigRect obj_rect;
    unsigned int new_wall_flags;

    for (idx = 0; idx < dword_5E2E24; idx++) {
        if ((stru_5E0E20[idx].flags & 0x01) != 0) {
            wall_flags = obj_field_int32_get(stru_5E0E20[idx].obj, OBJ_F_WALL_FLAGS);
            if ((wall_flags & 0x01) == 0) {
                wall_flags &= ~(0x04 | 0x02);
                obj_field_int32_set(stru_5E0E20[idx].obj, OBJ_F_WALL_FLAGS, wall_flags);

                render_flags = obj_field_int32_get(stru_5E0E20[idx].obj, OBJ_F_RENDER_FLAGS);
                render_flags &= ~ORF_01000000;
                obj_field_int32_set(stru_5E0E20[idx].obj, OBJ_F_RENDER_FLAGS, render_flags);

                object_get_rect(stru_5E0E20[idx].obj, 0, &obj_rect);
                dword_5E0A00(&obj_rect);
            }

            wallcheck_roof_faded_clear(stru_5E0E20[idx].field_18);

            sector_unlock(stru_5E0E20[idx].sector_id);

            memcpy(&(stru_5E0E20[idx]),
                &(stru_5E0E20[idx + 1]),
                sizeof(stru_5E0E20[0]) * (dword_5E2E24 - idx - 1));
            dword_5E2E24--;
            idx--;
        } else if ((stru_5E0E20[idx].flags & 0x02) != 0) {
            wall_flags = obj_field_int32_get(stru_5E0E20[idx].obj, OBJ_F_WALL_FLAGS);
            if ((wall_flags & 0x01) == 0) {
                wall_flags &= ~(0x04 | 0x02);

                if (((stru_5E0E20[idx].flags) & (0x08 | 0x04)) == (0x08 | 0x04)) {
                    new_wall_flags = 0x04 | 0x02;
                } else if ((stru_5E0E20[idx].flags & (0x08 | 0x04)) == 0x04) {
                    new_wall_flags = 0x02;
                } else {
                    new_wall_flags = 0x04;
                }

                if (new_wall_flags != wall_flags) {
                    obj_field_int32_set(stru_5E0E20[idx].obj, OBJ_F_WALL_FLAGS, wall_flags | new_wall_flags);

                    render_flags = obj_field_int32_get(stru_5E0E20[idx].obj, OBJ_F_RENDER_FLAGS);
                    render_flags &= ~ORF_01000000;
                    obj_field_int32_set(stru_5E0E20[idx].obj, OBJ_F_RENDER_FLAGS, render_flags);

                    object_get_rect(stru_5E0E20[idx].obj, 0, &obj_rect);
                    dword_5E0A00(&obj_rect);
                }
            }

            stru_5E0E20[idx].flags &= ~0x02;
        }
    }
}

// 0x438A50
void wallcheck_roof_faded_clear(int64_t a1)
{
    int index;

    if (!sub_4387C0(a1, &index)) {
        tig_debug_printf("Warning: unable to find roof to clear in wallcheck_roof_faded_clear()\n");
        return;
    }

    if (stru_5E0A10[index].field_8-- == 1) {
        sub_439F20(a1);
        memcpy(&(stru_5E0A10[index]),
            &(stru_5E0A10[index + 1]),
            sizeof(*stru_5E0A10) * (dword_5E2E28 - index - 1));
        dword_5E2E28--;
    }
}
