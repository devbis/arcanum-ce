#include "game/monstergen.h"

#include "game/location.h"
#include "game/mp_utils.h"
#include "game/obj.h"
#include "game/random.h"
#include "game/target.h"
#include "game/tile.h"

#define DISABLED 0x80

typedef struct GeneratorInfo {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t obj;
    /* 0010 */ int index;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int rate;
    /* 0020 */ bool enabled;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
} GeneratorInfo;

static void sub_4BA500(int64_t obj, GeneratorInfo* info);
static int sub_4BA590(int index);
static void sub_4BA620(GeneratorInfo* info);
static void sub_4BA6D0(int index, int value);
static bool sub_4BA720(int64_t obj);
static int sub_4BA910(GeneratorInfo* generator_info, int cnt);

// 0x5B5EC0
static DateTime stru_5B5EC0[8] = {
    { 0, 1000 },
    { 0, 30000 },
    { 0, 60000 },
    { 0, 3600000 },
    { 1, 0 },
    { 7, 0 },
    { 30, 0 },
    { 360, 0 },
};

// 0x5FC350
static uint8_t* off_5FC350;

// 0x5FC358
static TigRect monstergen_iso_content_rect;

// 0x4BA380
bool monstergen_init(GameInitInfo* init_info)
{
    TigWindowData window_data;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    monstergen_iso_content_rect.x = 0;
    monstergen_iso_content_rect.y = 0;
    monstergen_iso_content_rect.width = window_data.rect.width;
    monstergen_iso_content_rect.height = window_data.rect.height;

    off_5FC350 = (uint8_t*)CALLOC(256, 1);

    return true;
}

// 0x4BA400
void monstergen_reset()
{
    int index;

    for (index = 0; index < 256; index++) {
        off_5FC350[index] = 0;
    }
}

// 0x4BA420
void monstergen_exit()
{
    FREE(off_5FC350);
}

// 0x4BA430
void monstergen_resize(GameResizeInfo* resize_info)
{
    monstergen_iso_content_rect = resize_info->content_rect;
}

// 0x4BA460
bool monstergen_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(off_5FC350, 256, 1, load_info->stream) != 1) return false;

    return true;
}

// 0x4BA490
bool monstergen_save(TigFile* stream)
{
    if (tig_file_fwrite(off_5FC350, 256, 1, stream) != 1) return false;

    return true;
}

// 0x4BA500
void sub_4BA500(int64_t obj, GeneratorInfo* info)
{
    unsigned int flags;

    flags = obj_field_int32_get(obj, OBJ_F_NPC_GENERATOR_DATA);
    info->field_0 = (flags >> 27) & 0x1F;
    info->field_14 = (flags >> 14) & 0x1F;
    info->obj = obj;
    info->index = (flags >> 19);
    info->field_18 = (flags >> 7) & 0x7F;
    info->field_28 = flags & 0x7F;
    info->field_24 = sub_4BA590(info->index);
    info->rate = (obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) >> 21) & 0x7;
    info->enabled = !monstergen_is_disabled(info->index);
}

// 0x4BA590
int sub_4BA590(int index)
{
    return index >= 0 && index < 256 ? off_5FC350[index] & 0x1F : 0;
}

// 0x4BA620
void sub_4BA620(GeneratorInfo* info)
{
    unsigned int flags;

    flags = info->field_28;
    flags |= info->field_18 << 7;
    flags |= info->field_14 << 14;
    flags |= info->index << 19;
    flags |= info->field_0 << 27;
    obj_field_int32_set(info->obj, OBJ_F_NPC_GENERATOR_DATA, flags);
    sub_4BA6D0(info->index, info->field_24);

    flags = obj_field_int32_get(info->obj, OBJ_F_NPC_FLAGS);
    flags &= ~(ONF_GENERATOR_RATE1 | ONF_GENERATOR_RATE2 | ONF_GENERATOR_RATE3);
    flags |= (info->rate & 7) << 21;
    obj_field_int32_set(info->obj, OBJ_F_NPC_FLAGS, flags);

    if (info->enabled) {
        monstergen_enable(info->index);
    } else {
        monstergen_disable(info->index);
    }
}

// 0x4BA6D0
void sub_4BA6D0(int index, int value)
{
    if (index < 0 || index >= 256) {
        sub_4BA590(index);
        return;
    }

    if (value < 0 || value >= 32) {
        sub_4BA590(index);
        return;
    }

    off_5FC350[index] &= ~0x1F;
    off_5FC350[index] |= value;
}

// 0x4BA720
bool sub_4BA720(int64_t obj)
{
    unsigned int flags;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return false;
    }

    flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
    if ((flags & ONF_GENERATOR) == 0) {
        return false;
    }

    flags &= ~ONF_GENERATOR;
    sub_4EFDD0(obj, OBJ_F_NPC_FLAGS, flags);
    sub_4EFEE0(obj, 0x400002);

    return true;
}

// 0x4BA790
bool sub_4BA790(int64_t obj, DateTime* datetime)
{
    GeneratorInfo generator_info;
    int v3;
    int v2;
    int v1;
    TigRect rect;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) & ONF_GENERATOR) == 0) {
        return false;
    }

    sub_4BA500(obj, &generator_info);

    v2 = generator_info.field_14 - generator_info.field_24;

    do {
        if (v2 <= 0) {
            break;
        }

        if (monstergen_is_disabled(generator_info.index)) {
            break;
        }

        if (game_time_is_day()) {
            if ((generator_info.field_0 & 1) == 0) {
                break;
            }
        } else {
            if ((generator_info.field_0 & 2) == 0) {
                break;
            }
        }

        if ((generator_info.field_0 & 0x4) != 0) {
            object_get_rect(obj, 0x8, &rect);
            if (rect.x < monstergen_iso_content_rect.width + monstergen_iso_content_rect.x
                && rect.y < monstergen_iso_content_rect.height + monstergen_iso_content_rect.y
                && monstergen_iso_content_rect.x < rect.x + rect.width
                && monstergen_iso_content_rect.y < rect.y + rect.height) {
                break;
            }
        }

        if ((generator_info.field_0 & 0x10) == 0) {
            v1 = generator_info.field_18 - generator_info.field_28;
            if (v1 <= 0) {
                break;
            }

            if (v2 > v1) {
                v2 = v1;
            }
        }

        if (v2 <= 0) {
            break;
        }

        if ((generator_info.field_0 & 0x08) == 0) {
            v2 = 1;
        }

        v3 = sub_4BA910(&generator_info, v2);
        generator_info.field_24 += v3;

        if ((generator_info.field_0 & 0x10) == 0) {
            generator_info.field_28 += v3;
        }
        sub_4BA620(&generator_info);
    } while (0);

    *datetime = stru_5B5EC0[generator_info.rate];
    return true;
}

// 0x4BA910
int sub_4BA910(GeneratorInfo* generator_info, int cnt)
{
    int64_t loc;
    int64_t x;
    int64_t y;
    int64_t target_loc;
    int64_t target_x;
    int64_t target_y;
    tig_art_id_t art_id;
    int tile_type;
    int gen_cnt;
    int attempt;
    int distance;
    TigRect rect;
    int dx;
    int dy;
    int64_t obj;
    unsigned int flags;

    loc = obj_field_int64_get(generator_info->obj, OBJ_F_LOCATION);
    art_id = sub_4D70B0(loc);
    tile_type = tig_art_tile_id_type_get(art_id);
    location_xy(loc, &x, &y);
    object_get_rect(generator_info->obj, 0x8, &rect);

    gen_cnt = 0;
    while (gen_cnt < cnt) {
        if (generator_info->field_14 == 1) {
            target_loc = loc;
        } else {
            for (attempt = 0; attempt < 10; attempt++) {
                distance = random_between(1, 5);
                if (sub_4F4E40(generator_info->obj, distance, &target_loc)
                    && tile_type == tig_art_tile_id_type_get(sub_4D70B0(target_loc))) {
                    if ((generator_info->field_0 & 0x4) == 0) {
                        break;
                    }

                    location_xy(target_loc, &target_x, &target_y);

                    dx = (int)(target_x - x + rect.x);
                    dy = (int)(target_y - y + rect.y);
                    if (dx >= monstergen_iso_content_rect.x + monstergen_iso_content_rect.width
                        || dy >= monstergen_iso_content_rect.y + monstergen_iso_content_rect.height
                        || monstergen_iso_content_rect.x >= dx + rect.width
                        || monstergen_iso_content_rect.y >= dy + rect.height) {
                        break;
                    }
                }
            }

            if (attempt == 10) {
                target_loc = loc;
            }
        }

        if (!sub_4EF920(generator_info->obj, target_loc, &obj)) {
            break;
        }

        sub_4BA720(obj);

        flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
        flags |= ONF_GENERATED;
        sub_4EFDD0(obj, OBJ_F_NPC_FLAGS, flags);

        gen_cnt++;
    }

    return gen_cnt;
}

// 0x4BAB30
void sub_4BAB30(int64_t obj)
{
    unsigned int flags;
    GeneratorInfo generator_info;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return;
    }

    flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
    if ((flags & ONF_GENERATED) == 0) {
        return;
    }

    sub_4BA500(obj, &generator_info);
    sub_4BA6D0(generator_info.index, sub_4BA590(generator_info.index) - 1);
}

// 0x4BABA0
bool monstergen_is_disabled(int index)
{
    return index >= 0 && index < 256
        && (off_5FC350[index] & DISABLED) != 0;
}

// 0x4BABD0
void monstergen_enable(int index)
{
    if (index >= 0 && index < 256) {
        off_5FC350[index] &= ~DISABLED;
    }
}

// 0x4BABF0
void monstergen_disable(int index)
{
    if (index >= 0 && index < 256) {
        off_5FC350[index] |= DISABLED;
    }
}
