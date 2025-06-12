#include "game/monstergen.h"

#include "game/location.h"
#include "game/mp_utils.h"
#include "game/obj.h"
#include "game/random.h"
#include "game/target.h"
#include "game/tile.h"

#define DISABLED 0x80

static int monstergen_concurrent_get(int id);
static void monstergen_update(GeneratorInfo* info);
static int monstergen_concurrent_set(int id, int value);
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
static uint8_t* monstergen_state;

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

    monstergen_state = (uint8_t*)CALLOC(256, 1);

    return true;
}

// 0x4BA400
void monstergen_reset()
{
    int index;

    for (index = 0; index < 256; index++) {
        monstergen_state[index] = 0;
    }
}

// 0x4BA420
void monstergen_exit()
{
    FREE(monstergen_state);
}

// 0x4BA430
void monstergen_resize(GameResizeInfo* resize_info)
{
    monstergen_iso_content_rect = resize_info->content_rect;
}

// 0x4BA460
bool monstergen_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(monstergen_state, 256, 1, load_info->stream) != 1) return false;

    return true;
}

// 0x4BA490
bool monstergen_save(TigFile* stream)
{
    if (tig_file_fwrite(monstergen_state, 256, 1, stream) != 1) return false;

    return true;
}

// 0x4BA500
void monstergen_get(int64_t obj, GeneratorInfo* info)
{
    unsigned int flags;

    flags = obj_field_int32_get(obj, OBJ_F_NPC_GENERATOR_DATA);
    info->flags = (flags >> 27) & 0x1F;
    info->max_concurrent = (flags >> 14) & 0x1F;
    info->obj = obj;
    info->id = (flags >> 19);
    info->max_total = (flags >> 7) & 0x7F;
    info->cur_total = flags & 0x7F;
    info->cur_concurrent = monstergen_concurrent_get(info->id);
    info->rate = (obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) >> 21) & 0x7;
    info->enabled = !monstergen_is_disabled(info->id);
}

// 0x4BA590
int monstergen_concurrent_get(int id)
{
    return id >= 0 && id < 256 ? monstergen_state[id] & 0x1F : 0;
}

// 0x4BA5B0
bool monstergen_set(GeneratorInfo* info)
{
    unsigned int npc_flags;

    if (obj_field_int32_get(info->obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return false;
    }

    npc_flags = obj_field_int32_get(info->obj, OBJ_F_NPC_FLAGS);
    npc_flags |= ONF_GENERATOR;
    mp_obj_field_int32_set(info->obj, OBJ_F_NPC_FLAGS, npc_flags);

    mp_object_flags_set(info->obj, OF_INVULNERABLE | OF_OFF);

    monstergen_update(info);

    return true;
}

// 0x4BA620
void monstergen_update(GeneratorInfo* info)
{
    unsigned int flags;

    flags = info->cur_total;
    flags |= info->max_total << 7;
    flags |= info->max_concurrent << 14;
    flags |= info->id << 19;
    flags |= info->flags << 27;
    obj_field_int32_set(info->obj, OBJ_F_NPC_GENERATOR_DATA, flags);
    monstergen_concurrent_set(info->id, info->cur_concurrent);

    flags = obj_field_int32_get(info->obj, OBJ_F_NPC_FLAGS);
    flags &= ~(ONF_GENERATOR_RATE1 | ONF_GENERATOR_RATE2 | ONF_GENERATOR_RATE3);
    flags |= (info->rate & 7) << 21;
    obj_field_int32_set(info->obj, OBJ_F_NPC_FLAGS, flags);

    if (info->enabled) {
        monstergen_enable(info->id);
    } else {
        monstergen_disable(info->id);
    }
}

// 0x4BA6D0
int monstergen_concurrent_set(int id, int value)
{
    if (id < 0 || id >= 256) {
        return monstergen_concurrent_get(id);
    }

    if (value < 0 || value >= 32) {
        return monstergen_concurrent_get(id);
    }

    monstergen_state[id] &= ~0x1F;
    monstergen_state[id] |= value;

    return value;
}

// 0x4BA720
bool monstergen_remove(int64_t obj)
{
    unsigned int npc_flags;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return false;
    }

    npc_flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
    if ((npc_flags & ONF_GENERATOR) == 0) {
        return false;
    }

    npc_flags &= ~ONF_GENERATOR;
    mp_obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, npc_flags);

    mp_object_flags_unset(obj, OF_INVULNERABLE | OF_OFF);

    return true;
}

// 0x4BA790
bool monstergen_process(int64_t obj, DateTime* datetime)
{
    GeneratorInfo generator_info;
    int num_monsters_created;
    int num_monsters_to_create;
    int remaining;
    TigRect rect;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) & ONF_GENERATOR) == 0) {
        return false;
    }

    monstergen_get(obj, &generator_info);

    num_monsters_to_create = generator_info.max_concurrent - generator_info.cur_concurrent;

    do {
        if (num_monsters_to_create <= 0) {
            break;
        }

        if (monstergen_is_disabled(generator_info.id)) {
            break;
        }

        if (game_time_is_day()) {
            if ((generator_info.flags & GENERATOR_DAY) == 0) {
                break;
            }
        } else {
            if ((generator_info.flags & GENERATOR_NIGHT) == 0) {
                break;
            }
        }

        if ((generator_info.flags & GENERATOR_INACTIVE_ON_SCREEN) != 0) {
            object_get_rect(obj, 0x8, &rect);
            if (rect.x < monstergen_iso_content_rect.width + monstergen_iso_content_rect.x
                && rect.y < monstergen_iso_content_rect.height + monstergen_iso_content_rect.y
                && monstergen_iso_content_rect.x < rect.x + rect.width
                && monstergen_iso_content_rect.y < rect.y + rect.height) {
                break;
            }
        }

        if ((generator_info.flags & GENERATOR_IGNORE_TOTAL) == 0) {
            remaining = generator_info.max_total - generator_info.cur_total;
            if (remaining <= 0) {
                break;
            }

            if (num_monsters_to_create > remaining) {
                num_monsters_to_create = remaining;
            }
        }

        if (num_monsters_to_create <= 0) {
            break;
        }

        if ((generator_info.flags & GENERATOR_SPAWN_ALL) == 0) {
            num_monsters_to_create = 1;
        }

        num_monsters_created = sub_4BA910(&generator_info, num_monsters_to_create);
        generator_info.cur_concurrent += num_monsters_created;

        if ((generator_info.flags & GENERATOR_IGNORE_TOTAL) == 0) {
            generator_info.cur_total += num_monsters_created;
        }

        monstergen_update(&generator_info);
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
    art_id = tile_art_id_at(loc);
    tile_type = tig_art_tile_id_type_get(art_id);
    location_xy(loc, &x, &y);
    object_get_rect(generator_info->obj, 0x8, &rect);

    gen_cnt = 0;
    while (gen_cnt < cnt) {
        if (generator_info->max_concurrent == 1) {
            target_loc = loc;
        } else {
            for (attempt = 0; attempt < 10; attempt++) {
                distance = random_between(1, 5);
                if (sub_4F4E40(generator_info->obj, distance, &target_loc)
                    && tile_type == tig_art_tile_id_type_get(tile_art_id_at(target_loc))) {
                    if ((generator_info->flags & GENERATOR_INACTIVE_ON_SCREEN) == 0) {
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

        monstergen_remove(obj);

        flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
        flags |= ONF_GENERATED;
        mp_obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, flags);

        gen_cnt++;
    }

    return gen_cnt;
}

// 0x4BAB30
void monstergen_notify_killed(int64_t obj)
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

    monstergen_get(obj, &generator_info);
    monstergen_concurrent_set(generator_info.id, monstergen_concurrent_get(generator_info.id) - 1);
}

// 0x4BABA0
bool monstergen_is_disabled(int id)
{
    return id >= 0 && id < 256
        && (monstergen_state[id] & DISABLED) != 0;
}

// 0x4BABD0
void monstergen_enable(int id)
{
    if (id >= 0 && id < 256) {
        monstergen_state[id] &= ~DISABLED;
    }
}

// 0x4BABF0
void monstergen_disable(int id)
{
    if (id >= 0 && id < 256) {
        monstergen_state[id] |= DISABLED;
    }
}
