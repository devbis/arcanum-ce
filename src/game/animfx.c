#include "game/animfx.h"

#include "game/gsound.h"
#include "game/light.h"
#include "game/mes.h"

#define ANIMFX_LIST_CAPACITY 10
#define BLEND_COUNT 7
#define SCALE_COUNT 8

typedef enum EffectType {
    EFFECT_TYPE_TRANSLUCENCY,
    EFFECT_TYPE_TINTING,
    EFFECT_TYPE_COUNT,
} EffectType;

static void sub_4CDCD0(AnimFxList* list);
static bool animfx_list_load_internal(AnimFxList* list);
static void animfx_build_eye_candy_effect(int index, char* str);
static bool sub_4CE1A0(tig_art_id_t art_id, unsigned int flags, int index, const char* str);
static void sub_4CE2A0(int index);

// 0x5B7658
const char* off_5B7658[] = {
    "reverse",
    "stack",
    "destroy",
    "callback",
    "end_callback",
    "random_start",
    "fire_dmg",
    "check_already",
    "no_id",
    "ice_dmg",
};

// 0x5B7680
int dword_5B7680[] = {
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x20,
    0x40,
    0x80,
    0x100,
    0x240,
};

// 0x5B76A8
static const char* off_5B76A8[] = {
    "overlay_f",
    "overlay_b",
    "overlay_fb",
    "underlay",
    "tinting",
    "animates",
    "anim_forever",
    "can_autoscale",
};

// 0x5B76C8
static unsigned int dword_5B76C8[] = {
    ANIMFX_LIST_ENTRY_FOREGROUND_OVERLAY,
    ANIMFX_LIST_ENTRY_BACKGROUND_OVERLAY,
    ANIMFX_LIST_ENTRY_ANY_OVERLAY,
    ANIMFX_LIST_ENTRY_UNDERLAY,
    ANIMFX_LIST_ENTRY_TINTING,
    ANIMFX_LIST_ENTRY_ANIMATES,
    ANIMFX_LIST_ENTRY_ANIMATES | ANIMFX_LIST_ENTRY_LOOPS,
    ANIMFX_LIST_ENTRY_CAN_AUTOSCALE,
};

static_assert(sizeof(off_5B76A8) / sizeof(off_5B76A8[0]) == sizeof(dword_5B76C8) / sizeof(dword_5B76C8[0]), "wrong size");

// 0x5B76E8
static const char* off_5B76E8[] = {
    "none",
    "add",
    "sub",
    "mult",
    "alphC",
    "alphA",
    "alphS",
};

static_assert(sizeof(off_5B76E8) / sizeof(off_5B76E8[0]) == BLEND_COUNT, "wrong size");

// 0x5B7704
static const char* off_5B7704[] = {
    "Translucency",
    "Tinting",
};

static_assert(sizeof(off_5B7704) / sizeof(off_5B7704[0]) == EFFECT_TYPE_COUNT, "wrong size");

// 0x5B770C
static int dword_5B770C[] = {
    50,
    62,
    75,
    87,
    100,
    125,
    150,
    200,
};

static_assert(sizeof(dword_5B770C) / sizeof(dword_5B770C[0]) == SCALE_COUNT, "wrong size");

// 0x5B772C
static int dword_5B772C[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
};

static_assert(sizeof(dword_5B772C) / sizeof(dword_5B772C[0]) == SCALE_COUNT, "wrong size");

// 0x60170C
static AnimFxList* dword_60170C[ANIMFX_LIST_CAPACITY];

// 0x601734
static mes_file_handle_t dword_601734;

// 0x601738
static AnimFxList* dword_601738;

// 0x60173C
static int dword_60173C;

// 0x601740
static bool animfx_initialized;

// 0x4CCCF0
bool animfx_init(GameInitInfo* init_info)
{
    (void)init_info;

    animfx_initialized = true;

    return true;
}

// 0x4CCD00
void animfx_exit()
{
    if (animfx_initialized) {
        animfx_initialized = false;
    }
}

// 0x4CCD20
void sub_4CCD20(AnimFxList* list, AnimFxNode* node, int64_t obj, int a4, int a5)
{
    node->list = list;
    node->obj = obj;
    node->field_18 = a5;
    node->field_10 = 0;
    node->field_28 = a4;
    node->field_1C = 0;
    node->field_20 = -1;
    node->field_14 = 0;
    node->field_2C = 0;
    node->field_30 = 0;
    node->field_34 = 0;
    node->field_24 = 0;
    node->field_44 = -1;
    node->rotation = 0;
    node->field_4C = 4;
}

// 0x4CCD80
void sub_4CCD80(AnimFxNode* node)
{
    tig_art_id_t art_id;

    dword_601738 = node->list;
    art_id = dword_601738->entries[node->field_18].eye_candy_art_id;
    sub_502290(art_id);
    if ((dword_601738->entries[node->field_18].flags & 0x2) != 0) {
        art_id = tig_art_eye_candy_id_type_set(art_id, 1);
        sub_502290(art_id);
    }
}

// 0x4CD940
void animfx_remove(AnimFxList* list, int64_t obj, int index, int a4)
{
    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    dword_601738 = list;

    if (index >= list->field_14) {
        tig_debug_printf("AnimFX: animfx_id_get: Warning: Weapon AnimFXID Out of Range: %d.\n", index);
        return;
    }

    if (list->entries[index].eye_candy_art_id == TIG_ART_ID_INVALID) {
        tig_debug_printf("AnimFX: animfx_remove: Note: Attempting to remove EMPTY Art!\n");
        return;
    }

    if ((list->entries[index].flags & 0x20) != 0) {
        sub_4243E0(obj, list->entries[index].eye_candy_art_id, a4);
    } else {
        sub_4CD9C0(&(list->entries[index]), obj);
    }
}

// 0x4CDB30
bool animfx_list_init(AnimFxList* list)
{
    if (list == NULL) {
        return false;
    }

    list->path = NULL;
    list->flags = 0;
    list->field_8 = 0;
    list->field_C = 1;
    list->field_10 = 1;
    list->field_14 = 0;
    list->field_18 = 0;
    list->field_1C = 0;
    list->entries = NULL;
    list->field_20 = 0;
    list->field_24 = 0;

    return true;
}

// 0x4CDB70
bool animfx_list_load(AnimFxList* list)
{
    bool success;

    if (dword_60173C >= ANIMFX_LIST_CAPACITY) {
        tig_debug_printf("AnimFX: animfx_list_load: ERROR: Attempt to load too many FX lists!\n");
        // FIXME: Should be `EXIT_FAILURE`.
        exit(EXIT_SUCCESS);
    }

    dword_601738 = list;

    if (!mes_load(list->path, &dword_601734)) {
        return false;
    }

    sub_4CDCD0(list);

    if (list->field_18 > 0) {
        list->entries = (AnimFxListEntry*)MALLOC(sizeof(AnimFxListEntry) * list->field_18);
        list->field_1C = list->field_18;
    }

    success = false;
    if (animfx_list_load_internal(list)) {
        success = true;
        list->flags |= ANIMFX_LIST_LOADED;
        dword_60170C[dword_60173C++] = list;
    }

    mes_unload(dword_601734);
    dword_601738 = NULL;

    return success;
}

// 0x4CDC30
int animfx_list_find(AnimFxList* list)
{
    int index;

    for (index = 0; index < ANIMFX_LIST_CAPACITY; index++) {
        if (dword_60170C[index] == list) {
            return index;
        }
    }

    return -1;
}

// 0x4CDC50
AnimFxList* animfx_list_get(int index)
{
    if (index < ANIMFX_LIST_CAPACITY) {
        return dword_60170C[index];
    } else {
        return NULL;
    }
}

// 0x4CDC70
void animfx_list_exit(AnimFxList* list)
{
    int index;

    if ((list->flags & ANIMFX_LIST_LOADED) != 0) {
        for (index = 0; index < ANIMFX_LIST_CAPACITY; index++) {
            if (dword_60170C[index] == list) {
                dword_60170C[index] = NULL;
                dword_60173C--;
                break;
            }
        }

        FREE(list->entries);
        list->entries = NULL;

        list->flags &= ~ANIMFX_LIST_LOADED;
    }
}

// 0x4CDCD0
void sub_4CDCD0(AnimFxList* list)
{
    int count = 0;
    MesFileEntry mes_file_entry;

    mes_file_entry.num = list->field_8;

    while (mes_search(dword_601734, &mes_file_entry)) {
        mes_file_entry.num += list->field_10;
        count += list->field_C;
    }

    list->field_18 = count;
}

// 0x4CDD30
bool animfx_list_load_internal(AnimFxList* list)
{
    MesFileEntry mes_file_entry;
    int index;

    if (list->field_14 != 0) {
        return false;
    }

    mes_file_entry.num = list->field_8;

    while (list->field_14 < list->field_1C) {
        for (index = 0; index < list->field_C; index++) {
            sub_4CE2A0(list->field_14);

            if (mes_search(dword_601734, &mes_file_entry)) {
                mes_get_msg(dword_601734, &mes_file_entry);
                animfx_build_eye_candy_effect(list->field_14, mes_file_entry.str);
            }

            mes_file_entry.num++;
            list->field_14++;
        }

        mes_file_entry.num += list->field_10 - list->field_C;
    }

    return true;
}

// 0x4CDDF0
void animfx_build_eye_candy_effect(int index, char* str)
{
    if (str == NULL) {
        return;
    }

    char* curr = str;
    tig_str_parse_set_separator(',');

    AnimFxListEntry* entry = &(dword_601738->entries[index]);
    entry->eye_candy_art_id = -1;
    entry->sound = -1;
    entry->light_art_id = -1;
    entry->flags = 0;
    entry->light_color = 0;

    if (curr[0] == '\0') {
        entry->eye_candy_art_id = -1;
        return;
    }

    int art;
    int palette;
    int scale;
    int blend;
    int sound;
    int art_effect;
    if (tig_str_parse_named_value(&curr, "Art:", &art)) {
        tig_str_parse_named_value(&curr, "Palette:", &palette);

        int scale_key;
        if (tig_str_parse_named_value(&curr, "Scale:", &scale_key)) {
            int scale_index;
            for (scale_index = 0; scale_index < SCALE_COUNT; scale_index++) {
                if (dword_5B770C[scale_index] == scale_key) {
                    break;
                }
            }

            if (scale_index < SCALE_COUNT) {
                scale = dword_5B772C[scale_index];
            } else {
                tig_debug_printf("animfx_build_eye_candy_effect: Error: scale out of range!\n");
                scale = dword_5B772C[0];
            }

            entry->flags |= ANIMFX_LIST_ENTRY_CAN_AUTOSCALE;
        }

        tig_str_match_named_str_to_list(&curr, "Blend:", off_5B76E8, BLEND_COUNT, &blend);

        if (tig_str_parse_named_value(&curr, "Sound:", &sound)) {
            entry->sound = sound;
        } else {
            entry->sound = -1;

            if (dword_601738->field_20 > 0) {
                int base_index = index % dword_601738->field_C;
                if (base_index != 0) {
                    int base_sound = dword_601738->entries[index - base_index].sound;
                    if (base_sound != -1) {
                        entry->sound = dword_601738->field_24[base_index] + base_sound;

                        char path[MAX_PATH];
                        if (gsound_resolve_path(entry->sound, path) != TIG_OK) {
                            entry->sound = -1;
                        }
                    }
                }
            }
        }

        tig_str_parse_named_flag_list(&curr, "Flags:", off_5B76A8, dword_5B76C8, sizeof(off_5B76A8) / sizeof(off_5B76A8[0]), &(entry->flags));

        // TODO: Find constants.
        int type = ((entry->flags & ANIMFX_LIST_ENTRY_ANY_OVERLAY) != 0)
            ? 0
            : 2;

        if (blend != 0) {
            entry->flags |= ANIMFX_LIST_ENTRY_TRANSLUCENCY;
        }

        tig_art_id_t eye_candy_art_id;
        if (tig_art_eye_candy_id_create(art, 0, 0, (entry->flags & ANIMFX_LIST_ENTRY_TRANSLUCENCY) != 0 ? 1 : 0, type, palette, scale, &eye_candy_art_id) == TIG_OK) {
            entry->eye_candy_art_id = eye_candy_art_id;
            if (!sub_4CE1A0(entry->eye_candy_art_id, entry->flags, index, str)) {
                entry->eye_candy_art_id = -1;
                entry->flags = 0;
            }
        } else {
            tig_debug_printf("AnimFX: ERROR: Build Eye Candy FAILED Due to invalid ART ID: [%s], %u, Art Num: %d: Foreground: Index: %d, Str: [%s]!!!\n",
                dword_601738->path,
                eye_candy_art_id,
                tig_art_num(eye_candy_art_id),
                index,
                curr);
            entry->eye_candy_art_id = -1;
            entry->flags = 0;
        }
    } else if (tig_str_parse_named_value(&curr, "Sound:", &sound)) {
        entry->sound = sound;
    } else if (tig_str_match_named_str_to_list(&curr, "ArtEffect:", off_5B7704, EFFECT_TYPE_COUNT, &art_effect)) {
        switch (art_effect) {
        case EFFECT_TYPE_TRANSLUCENCY:
            entry->flags |= ANIMFX_LIST_ENTRY_TRANSLUCENCY;
            break;
        case EFFECT_TYPE_TINTING:
            entry->flags |= ANIMFX_LIST_ENTRY_TINTING;
            break;
        }
    }

    int light;
    if (tig_str_parse_named_value(&curr, "Light:", &light)) {
        tig_art_id_t light_art_id;
        tig_art_light_id_create(light, 0, 0, 0, &light_art_id);
        entry->light_art_id = light_art_id;

        int red;
        int green;
        int blue;
        if (tig_str_parse_named_complex_value3(&curr, "Light Color:", '@', &red, &green, &blue)) {
            light_build_color(red, green, blue, &(entry->light_color));
        } else {
            light_build_color(255, 255, 255, &(entry->light_color));
        }
    } else {
        entry->light_art_id = -1;
    }

    int projectile_speed;
    if (tig_str_parse_named_value(&curr, "Proj Speed:", &projectile_speed)) {
        entry->projectile_speed = projectile_speed;
    }
}

// 0x4CE1A0
bool sub_4CE1A0(tig_art_id_t art_id, unsigned int flags, int index, const char* str)
{
    if ((flags & ANIMFX_LIST_ENTRY_FOREGROUND_OVERLAY) != 0) {
        art_id = tig_art_eye_candy_id_type_set(art_id, 0);
        if (tig_art_exists(art_id) != TIG_OK) {
            tig_debug_printf("AnimFX: ERROR: Build Eye Candy FAILED Due to invalid ART ID: [%s], %u, Art Num: %d: Foreground: Index: %d, Str: [%s]!!!\n",
                dword_601738->path,
                art_id,
                tig_art_num(art_id),
                index,
                str);
            return false;
        }
    }

    if ((flags & ANIMFX_LIST_ENTRY_BACKGROUND_OVERLAY) != 0) {
        art_id = tig_art_eye_candy_id_type_set(art_id, 1);
        if (tig_art_exists(art_id) != TIG_OK) {
            tig_debug_printf("AnimFX: ERROR: Build Eye Candy FAILED Due to invalid ART ID: [%s], %u, Art Num: %d: Background: Index: %d, Str: [%s]!!!\n",
                dword_601738->path,
                art_id,
                tig_art_num(art_id),
                index,
                str);
            return false;
        }
    }

    if ((flags & ANIMFX_LIST_ENTRY_UNDERLAY) != 0) {
        art_id = tig_art_eye_candy_id_type_set(art_id, 2);
        if (tig_art_exists(art_id) != TIG_OK) {
            tig_debug_printf("AnimFX: ERROR: Build Eye Candy FAILED Due to invalid ART ID: [%s], %u, Art Num: %d: Underlay: Index: %d, Str: [%s]!!!\n",
                dword_601738->path,
                art_id,
                tig_art_num(art_id),
                index,
                str);
            return false;
        }
    }

    return true;
}

// 0x4CE2A0
void sub_4CE2A0(int index)
{
    AnimFxListEntry* entry;

    if (index >= dword_601738->field_1C) {
        dword_601738->field_1C = index;
        dword_601738->entries = (AnimFxListEntry*)REALLOC(dword_601738->entries, sizeof(AnimFxListEntry) * index);
    }

    entry = &(dword_601738->entries[index]);
    entry->flags = 0;
    entry->eye_candy_art_id = -1;
    entry->sound = -1;
    entry->light_art_id = -1;
    entry->projectile_speed = 0;
}

// 0x4CE300
bool animfx_id_get(AnimFxList* list, int index, AnimFxListEntry** entry)
{
    dword_601738 = list;

    if (entry != NULL) {
        if (index < list->field_14) {
            *entry = &(list->entries[index]);
            return true;
        }

        tig_debug_printf("AnimFX: animfx_id_get: Warning: Weapon AnimFXID Out of Range: %d.\n", index);
    }

    return false;
}
