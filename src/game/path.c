#include "game/path.h"

#include "game/critter.h"
#include "game/object.h"
#include "game/location.h"
#include "game/sector.h"
#include "game/tile.h"
#include "game/townmap.h"
#include "game/trap.h"

typedef struct S420330 {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int64_t field_10;
    /* 0018 */ int64_t field_18;
    /* 0020 */ uint8_t* rotations;
} S420330;

static int sub_41F6C0(PathCreateInfo* path_create_info);
static int sub_41F840(PathCreateInfo* path_create_info);
static int sub_41F9F0(PathCreateInfo* path_create_info);
static void sub_420330(int64_t x, int64_t y, S420330* a5);
static void sub_4203B0(int64_t from_x, int64_t from_y, int64_t to_x, int64_t to_y, S420330* a5, void(*fn)(int64_t, int64_t, S420330*));
static int sub_420660(int64_t from, int64_t to, uint8_t* rotations);
static int sub_420E30(PathCreateInfo* path_create_info, tig_duration_t ms);

// 0x5A15C0
static int path_limit = 10;

// 0x5A15C4
static int path_time_limit = 250;

// 0x41F3C0
int sub_41F3C0(PathCreateInfo* path_create_info)
{
    bool v1 = false;

    if ((path_create_info->field_20 & 0x800) != 0) {
        return sub_41F6C0(path_create_info);
    }

    path_create_info->field_24 = 0;

    if (path_create_info->from == path_create_info->to) {
        return 0;
    }

    if ((path_create_info->field_20 & 0x08) != 0) {
        v1 = true;
    }

    if ((path_create_info->field_20 & 0x80) != 0) {
        return sub_420660(path_create_info->from, path_create_info->to, path_create_info->rotations);
    }

    if ((path_create_info->field_20 & 0x100) != 0) {
        return sub_4201C0(path_create_info->from, path_create_info->to, path_create_info->rotations);
    }

    if ((path_create_info->field_20 & 0x41) == 0) {
        ObjectList objects;
        ObjectNode* node;

        if ((path_create_info->field_20 & 0x08) == 0
            && sub_4D7110(path_create_info->to, v1)) {
            return 0;
        }

        sub_4407C0(path_create_info->to, OBJ_TM_CRITTER | OBJ_TM_SCENERY | OBJ_TM_CONTAINER, &objects);
        node = objects.head;
        while (node != NULL) {
            if ((obj_field_int32_get(node->obj, OBJ_F_FLAGS) & OF_NO_BLOCK) != 0) {
                if (!obj_type_is_critter(obj_field_int32_get(node->obj, OBJ_F_TYPE))
                    || !critter_is_dead(node->obj)) {
                    break;
                }
            }
            node = node->next;
        }
        object_list_destroy(&objects);

        if (node != NULL) {
            return false;
        }
    }

    int v2 = 0;
    if ((path_create_info->field_20 & 0x200) == 0) {
        v2 = sub_41F840(path_create_info);
    }

    if (v2 == 0) {
        if ((path_create_info->field_20 & 0x1000) != 0) {
            v2 = sub_420E30(path_create_info, 300);
        } else {
            if ((path_create_info->field_20 & 0x20) != 0) {
                return 0;
            }

            v2 = sub_41F9F0(path_create_info);
        }
    }

    if (v2 > 8) {
        v2 = 8;
    }

    return v2;
}

// 0x41F570
unsigned int sub_41F570(unsigned int flags)
{
    unsigned int new_flags = 0;

    if ((flags & 0x2) != 0) {
        new_flags |= 0x01;
    }

    if ((flags & 0x04) != 0) {
        new_flags |= 0x02;
    }

    if ((flags & 0x10) != 0) {
        new_flags |= 0x04;
    }

    return new_flags;
}

// 0x41F6C0
int sub_41F6C0(PathCreateInfo* path_create_info)
{
    int64_t dist;
    int rotations[8];
    int range;
    int base_rot;
    int idx;
    int v1;
    int v2;
    int64_t loc;

    rotations[0] = 4;
    rotations[1] = 5;
    rotations[2] = 3;
    rotations[3] = 6;
    rotations[4] = 2;
    rotations[5] = 7;
    rotations[6] = 1;
    rotations[7] = 0;

    path_create_info->field_20 &= ~0x800;

    dist = location_dist(path_create_info->from, path_create_info->to);

    if (dist >= INT_MAX) {
        return 0;
    }

    range = path_create_info->field_24 - (int)dist;
    base_rot = location_rot(path_create_info->from, path_create_info->to);

    v2 = 0;
    for (idx = 0; idx < 8; idx++) {
        sub_4B90D0(path_create_info->from,
            (base_rot + rotations[idx]) % 8,
            range,
            &(path_create_info->to));
        v1 = sub_41F840(path_create_info);
        if (v1 != 0) {
            break;
        }

        if (v2 < path_create_info->field_24) {
            v2 = path_create_info->field_24;
            loc = path_create_info->to;
        }
    }

    path_create_info->field_20 |= 0x800;

    if (v1 == 0 && v2 > 0) {
        path_create_info->to = loc;
        v1 = sub_41F840(path_create_info);
    }

    return v1;
}

// 0x41F840
int sub_41F840(PathCreateInfo* path_create_info)
{
    bool v1 = false;
    unsigned int flags;
    int idx;
    int64_t loc;
    int rot;
    int64_t adjacent_loc;
    int v2;
    int64_t obj;

    if (location_dist(path_create_info->from, path_create_info->to) > 32) {
        return 0;
    }

    if ((path_create_info->field_20 & 0x08) != 0) {
        v1 = true;
    }

    flags = sub_41F570(path_create_info->field_20);

    loc = path_create_info->from;
    for (idx = 0; idx < path_create_info->max_rotations; idx++) {
        if (loc == path_create_info->to) {
            break;
        }

        rot = location_rot(loc, path_create_info->to);
        path_create_info->rotations[idx] = (uint8_t)rot;
        sub_4B8FF0(loc, rot, &adjacent_loc);

        if ((path_create_info->field_20 & 0x40) == 0) {
            if (adjacent_loc != path_create_info->to
                || (path_create_info->field_20 & 0x01) == 0) {
                if ((path_create_info->field_20 & 0x08) == 0
                    && sub_4D7110(adjacent_loc, v1)) {
                    break;
                }

                if (sub_43FD70(path_create_info->obj, loc, rot, flags, &v2)) {
                    break;
                }

                if (v2) {
                    break;
                }

                obj = sub_4BCAB0(adjacent_loc);
                if (obj != OBJ_HANDLE_NULL
                    && sub_4BBFE0(path_create_info->obj, obj)) {
                    break;
                }

                if ((path_create_info->field_20 & 0x400) != 0
                    && sub_444500(adjacent_loc)) {
                    break;
                }
            }
        }

        loc = adjacent_loc;
    }

    if (loc != path_create_info->to) {
        path_create_info->field_24 = idx;
        return 0;
    }

    if ((path_create_info->field_20 & 0x01) != 0) {
        idx--;
    }

    return idx;
}

// 0x41F9F0
int sub_41F9F0(PathCreateInfo* path_create_info)
{
    // TODO: Incomplete.
}

// 0x4200C0
int sub_4200C0(int a1, int a2, int a3)
{
    int v1;
    int v2;

    v1 = a1 % a3 - a2 % a3;
    if (v1 < 0) {
        v1 = a2 % a3 - a1 % a3;
    }

    v2 = a1 / a3 - a2 / a3;
    if (v2 < 0) {
        v2 = a2 / a3 - a1 / a3;
    }

    if (v1 <= v2) {
        v1 = v2;
    }

    return 10 * v1;
}

// 0x420110
int sub_420110(int a1, int a2, int a3)
{
    int v3;
    int v4;
    int rot;

    v3 = a2 % a3 - a1 % a3;
    v4 = a2 / a3 - a1 / a3;

    if (v4 >= 0) {
        if (v3 >= 0) {
            if (v4 <= 0) {
                if (v4 > -1 )
                rot = (v4 < 1) + 4;
                else
                rot = 6;
            } else {
                rot = (v3 >= 1) + 3;
            }
        } else if (v4 > -1) {
            rot = (v4 >= 1) + 1;
        } else {
            rot = 0;
        }
    } else if (v3 > -1) {
        rot = (v3 < 1) + 6;
    } else {
        rot = 0;
    }

    return rot;
}

// 0x4201C0
int sub_4201C0(int64_t from, int64_t to, uint8_t* rotations)
{
    S420330 v1;
    int64_t from_x;
    int64_t from_y;
    int64_t to_x;
    int64_t to_y;

    v1.field_0 = 0;
    v1.field_4 = 10;
    v1.field_8 = 0;
    v1.field_10 = 0;
    v1.field_18 = 0;
    v1.rotations = rotations;

    location_xy(from, &from_x, &from_y);
    from_x += 40;
    from_y += 20;

    location_xy(to, &to_x, &to_y);
    to_x += 40;
    to_y += 20;

    v1.field_10 = from_x;
    v1.field_18 = from_y;

    sub_4203B0(from_x, from_y, to_x, to_y, &v1, sub_420330);

    if (v1.field_8 == -1) {
        return 0;
    }

    while (v1.field_0 != 0) {
        sub_420330(to_x, to_y, &v1);
    }

    if (v1.field_8 == -1) {
        return 0;
    }

    return v1.field_8;
}

// 0x420330
void sub_420330(int64_t x, int64_t y, S420330* a5)
{
    if (a5->field_8 == -1 || a5->field_8 >= 200) {
        a5->field_8 = -1;
    } else {
        a5->field_0++;
        if (a5->field_0 == a5->field_4) {
            a5->rotations[a5->field_8] = (x & 0xFF) - (a5->field_10 & 0xFF);
            a5->rotations[a5->field_8 + 1] = (y & 0xFF) - (a5->field_18 & 0xFF);
            a5->field_10 = x;
            a5->field_18 = y;
            a5->field_8 += 2;
            a5->field_0 = 0;
        }
    }
}

// 0x4203B0
void sub_4203B0(int64_t from_x, int64_t from_y, int64_t to_x, int64_t to_y, S420330* a5, void(*fn)(int64_t, int64_t, S420330*))
{
    int64_t dx;
    int64_t dy;
    int64_t ddx;
    int64_t ddy;
    int64_t err;
    int sx;
    int sy;

    dx = to_x - from_x;
    ddx = 2 * llabs(dx);

    if (dx < 0) {
        sx = -1;
    } else if (dx > 0) {
        sx = 1;
    } else {
        sx = 0;
    }

    dy = to_y - from_y;
    ddy = 2 * llabs(dy);

    if (dy < 0) {
        sy = -1;
    } else if (dy > 0) {
        sy = 1;
    } else {
        sy = 0;
    }

    if (ddx <= ddy) {
        err = ddx - ddy / 2;
        fn(from_x, from_y, a5);
        while (from_y != to_y) {
            if (err >= 0) {
                from_x += sx;
                err -= ddy;
            }

            err += ddx;
            from_y += sy;
            fn(from_x, from_y, a5);
        }
    } else {
        err = ddy - ddx / 2;
        fn(from_x, from_y, a5);
        while (from_x != to_x) {
            if (err >= 0) {
                from_y += sy;
                err -= ddx;
            }

            err += ddy;
            from_x += sx;
            fn(from_x, from_y, a5);
        }
    }
}

// 0x420660
int sub_420660(int64_t from, int64_t to, uint8_t* rotations)
{
    S420330 v1;
    int64_t from_x;
    int64_t from_y;
    int64_t to_x;
    int64_t to_y;

    v1.field_0 = 0;
    v1.field_4 = 10;
    v1.field_8 = 0;
    v1.field_10 = 0;
    v1.field_18 = 0;
    v1.rotations = NULL;

    location_xy(from, &from_x, &from_y);
    from_x += 40;
    from_y += 20;

    location_xy(to, &to_x, &to_y);
    to_x += 40;
    to_y += 20;

    v1.field_10 = from_x;
    v1.field_18 = from_y;
    sub_4203B0(from_x, from_y, to_x, to_y, &v1, sub_420330);

    if (v1.field_8 == -1) {
        return 0;
    }

    while (v1.field_0 != 0) {
        sub_420330(to_x, to_y, &v1);
    }

    if (v1.field_8 == -1) {
        return 0;
    }

    return v1.field_8;
}

// 0x4207D0
void sub_4207D0()
{
    // TODO: Incomplete.
}

// 0x420900
void sub_420900()
{
    // TODO: Incomplete.
}

// 0x4209C0
void sub_4209C0()
{
    // TODO: Incomplete.
}

// 0x420E30
int sub_420E30(PathCreateInfo* path_create_info, tig_duration_t ms)
{
    // TODO: Incomplete.
}

// 0x421AC0
bool path_set_limit(int value)
{
    if (value <= 0 || value >= 35) {
        return false;
    }

    path_limit = value;

    return true;
}

// 0x421AE0
bool path_set_time_limit(int value)
{
    if (value <= 0 || value >= 1000) {
        return false;
    }

    path_time_limit = value;

    return true;
}
