#include "game/sfx.h"

#include "game/a_name.h"
#include "game/gsound.h"
#include "game/item.h"
#include "game/material.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/random.h"
#include "game/tile.h"

typedef enum MeleeMaterialSound {
    MELEE_MAT_SOUND_FLESH,
    MELEE_MAT_SOUND_LIGHT_METAL,
    MELEE_MAT_SOUND_HEAVY_METAL,
    MELEE_MAT_SOUND_STONE,
    MELEE_MAT_SOUND_WOOD,
} MeleeMaterialSound;

typedef enum ItemMaterialSound {
    ITEM_MAT_SOUND_FLESH,
    ITEM_MAT_SOUND_LIGHT_METAL,
    ITEM_MAT_SOUND_HEAVY_METAL,
    ITEM_MAT_SOUND_STONE,
    ITEM_MAT_SOUND_WOOD,
    ITEM_MAT_SOUND_GLASS,
    ITEM_MAT_SOUND_CLOTH,
    ITEM_MAT_SOUND_PAPER,
} ItemMaterialSound;

// 0x5BB928
static MeleeMaterialSound sfx_melee_sounds[MATERIAL_COUNT] = {
    /*  MATERIAL_STONE */ MELEE_MAT_SOUND_STONE,
    /*  MATERIAL_BRICK */ MELEE_MAT_SOUND_STONE,
    /*   MATERIAL_WOOD */ MELEE_MAT_SOUND_WOOD,
    /*  MATERIAL_PLANT */ MELEE_MAT_SOUND_WOOD,
    /*  MATERIAL_FLESH */ MELEE_MAT_SOUND_FLESH,
    /*  MATERIAL_METAL */ MELEE_MAT_SOUND_HEAVY_METAL,
    /*  MATERIAL_GLASS */ MELEE_MAT_SOUND_LIGHT_METAL,
    /*  MATERIAL_CLOTH */ MELEE_MAT_SOUND_FLESH,
    /* MATERIAL_LIQUID */ MELEE_MAT_SOUND_FLESH,
    /*  MATERIAL_PAPER */ MELEE_MAT_SOUND_FLESH,
    /*    MATERIAL_GAS */ MELEE_MAT_SOUND_FLESH,
    /*  MATERIAL_FORCE */ MELEE_MAT_SOUND_LIGHT_METAL,
    /*   MATERIAL_FIRE */ MELEE_MAT_SOUND_FLESH,
    /* MATERIAL_POWDER */ MELEE_MAT_SOUND_FLESH,
};

// 0x5BB960
static ItemMaterialSound sfx_item_sounds[MATERIAL_COUNT] = {
    /*  MATERIAL_STONE */ ITEM_MAT_SOUND_STONE,
    /*  MATERIAL_BRICK */ ITEM_MAT_SOUND_STONE,
    /*   MATERIAL_WOOD */ ITEM_MAT_SOUND_WOOD,
    /*  MATERIAL_PLANT */ ITEM_MAT_SOUND_WOOD,
    /*  MATERIAL_FLESH */ ITEM_MAT_SOUND_FLESH,
    /*  MATERIAL_METAL */ ITEM_MAT_SOUND_HEAVY_METAL,
    /*  MATERIAL_GLASS */ ITEM_MAT_SOUND_GLASS,
    /*  MATERIAL_CLOTH */ ITEM_MAT_SOUND_CLOTH,
    /* MATERIAL_LIQUID */ ITEM_MAT_SOUND_FLESH,
    /*  MATERIAL_PAPER */ ITEM_MAT_SOUND_PAPER,
    /*    MATERIAL_GAS */ ITEM_MAT_SOUND_FLESH,
    /*  MATERIAL_FORCE */ ITEM_MAT_SOUND_LIGHT_METAL,
    /*   MATERIAL_FIRE */ ITEM_MAT_SOUND_FLESH,
    /* MATERIAL_POWDER */ ITEM_MAT_SOUND_FLESH,
};

// 0x5BB998
static int sfx_base_footstep_sound_ids[TILE_SOUND_COUNT] = {
    /*  TILE_SOUND_DIRT */ 2904,
    /*  TILE_SOUND_SAND */ 2912,
    /*  TILE_SOUND_SNOW */ 2916,
    /* TILE_SOUND_STONE */ 2920,
    /* TILE_SOUND_WATER */ 2928,
    /*  TILE_SOUND_WOOD */ 2932,
};

// 0x4F0BF0
int sfx_item_sound(int64_t item_obj, int64_t parent_obj, int64_t target_obj, ItemSoundType type)
{
    int sound_id = -1;
    int sound_effect;
    int material;
    ItemMaterialSound item_mat_sound;
    MeleeMaterialSound target_mat_sound;
    MeleeMaterialSound weapon_mat_sound;
    char path[TIG_MAX_PATH];

    if (item_obj != OBJ_HANDLE_NULL) {
        sound_effect = obj_field_int32_get(item_obj, OBJ_F_SOUND_EFFECT);
    } else {
        sound_effect = 0;
    }

    switch (type) {
    case ITEM_SOUND_PICKUP:
        if (item_obj != OBJ_HANDLE_NULL) {
            if (obj_field_int32_get(item_obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN) == 9056) {
                sound_id = 5958;
            } else {
                material = obj_field_int32_get(item_obj, OBJ_F_MATERIAL);
                item_mat_sound = sfx_item_sounds[material];
                if (item_mat_sound == ITEM_MAT_SOUND_HEAVY_METAL
                    && item_weight(item_obj, parent_obj) <= 2000) {
                    item_mat_sound = ITEM_MAT_SOUND_LIGHT_METAL;
                }
                sound_id = 5950 + item_mat_sound;
            }
        }
        break;
    case ITEM_SOUND_DROP:
        if (item_obj != OBJ_HANDLE_NULL) {
            if (obj_field_int32_get(item_obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN) == 9056) {
                sound_id = 5968;
            } else {
                material = obj_field_int32_get(item_obj, OBJ_F_MATERIAL);
                item_mat_sound = sfx_item_sounds[material];
                if (item_mat_sound == ITEM_MAT_SOUND_HEAVY_METAL
                    && item_weight(item_obj, parent_obj) <= 2000) {
                    item_mat_sound = ITEM_MAT_SOUND_LIGHT_METAL;
                }
                sound_id = 5960 + item_mat_sound;
            }
        }
        break;
    case WEAPON_SOUND_USE:
        if (sound_effect != 0) {
            sound_id = sound_effect;
        }
        break;
    case WEAPON_SOUND_OUT_OF_AMMO:
        if (sound_effect != 0) {
            sound_id = sound_effect + 7;
        }
        break;
    case WEAPON_SOUND_HIT:
    case WEAPON_SOUND_CRITICAL_HIT:
        if (target_obj != OBJ_HANDLE_NULL) {
            if (sound_effect != 0) {
                if (type == WEAPON_SOUND_CRITICAL_HIT) {
                    sound_id = sound_effect + 6;
                } else {
                    sound_id = sound_effect + 4;
                    if (random_between(0, 1) == 1) {
                        sound_id++;
                    }
                }

                if (gsound_resolve_path(sound_id, path) == TIG_OK) {
                    break;
                }
            }

            if (obj_type_is_critter(obj_field_int32_get(target_obj, OBJ_F_TYPE))) {
                int64_t armor_obj = item_wield_get(target_obj, ITEM_INV_LOC_ARMOR);
                if (armor_obj != OBJ_HANDLE_NULL) {
                    target_obj = armor_obj;
                }
            }

            material = obj_field_int32_get(target_obj, OBJ_F_MATERIAL);
            target_mat_sound = sfx_melee_sounds[material];

            if (item_obj != OBJ_HANDLE_NULL) {
                material = obj_field_int32_get(item_obj, OBJ_F_MATERIAL);
                weapon_mat_sound = sfx_melee_sounds[material];
                if (weapon_mat_sound == MELEE_MAT_SOUND_HEAVY_METAL
                    && item_weight(item_obj, parent_obj) <= 2000) {
                    weapon_mat_sound = MELEE_MAT_SOUND_LIGHT_METAL;
                }
            } else {
                weapon_mat_sound = MELEE_MAT_SOUND_FLESH;
            }

            sound_id = 7000 + weapon_mat_sound * 20 + target_mat_sound * 3 + random_between(0, 1);
        }
        break;
    case WEAPON_SOUND_MISS:
        if (item_obj != OBJ_HANDLE_NULL && sound_effect != 0) {
            sound_id = sound_effect + 3;
        } else {
            sound_id = 4010;
        }
        break;
    }

    return sound_id;
}

// 0x4F0ED0
int sfx_critter_sound(int64_t obj, CritterSoundType type)
{
    int64_t pc_obj;
    tig_art_id_t art_id;
    int armor_type;
    int sound_id;
    int64_t loc;
    tig_art_id_t tile_art_id;
    int tile_sound_type;

    if (obj != OBJ_HANDLE_NULL) {
        return -1;
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return -1;
    }

    pc_obj = player_get_local_pc_obj();
    if (pc_obj == OBJ_HANDLE_NULL) {
        return -1;
    }

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_OFF) != 0) {
        return -1;
    }

    if (type != CRITTER_SOUND_FOOTSTEPS) {
        return obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT) + type;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    armor_type = tig_art_critter_id_armor_get(art_id);
    if (armor_type == TIG_ART_ARMOR_TYPE_PLATE
        || armor_type == TIG_ART_ARMOR_TYPE_PLATE_CLASSIC) {
        sound_id = 2900;
    } else {
        loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
        tile_art_id = tile_art_id_at(loc);
        tile_sound_type = a_name_tile_sound(tile_art_id);
        sound_id = sfx_base_footstep_sound_ids[tile_sound_type];
        if (armor_type == TIG_ART_ARMOR_TYPE_CHAIN
            && (tile_sound_type == TILE_SOUND_WOOD
                || tile_sound_type == TILE_SOUND_STONE
                || tile_sound_type == TILE_SOUND_DIRT)) {
            sound_id += 4;
        }
    }

    return sound_id + random_between(0, 3);
}

// 0x4F0FD0
int sfx_container_sound(int64_t obj, ContainerSoundType type)
{
    int sound_effect;

    if (obj == OBJ_HANDLE_NULL) {
        return -1;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_CONTAINER) {
        return -1;
    }

    sound_effect = obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT);

    return sound_effect + type;
}

// 0x4F1010
int sfx_portal_sound(int64_t obj, PortalSoundType type)
{
    int sound_effect;

    if (obj == OBJ_HANDLE_NULL) {
        return -1;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PORTAL) {
        return -1;
    }

    sound_effect = obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT);

    return sound_effect + type;
}

// 0x4F1050
int sfx_misc_sound(int64_t obj, MiscSoundType type)
{
    int obj_type;
    int sound_effect;
    int sound_id = -1;
    char path[TIG_MAX_PATH];

    if (obj == OBJ_HANDLE_NULL) {
        return -1;
    }

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type_is_critter(obj_type)
        || obj_type == OBJ_TYPE_CONTAINER
        || obj_type == OBJ_TYPE_PORTAL
        || (obj_type_is_item(obj_type) && obj_type != OBJ_TYPE_WEAPON)) {
        sound_effect = 0;
    } else {
        sound_effect = obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT);
    }

    switch (type) {
    case MISC_SOUND_BUSTING:
        if (sound_effect == 0) {
            return -1;
        }

        sound_id = obj_type == OBJ_TYPE_WEAPON ? sound_effect + 1 : sound_effect;
        if (gsound_resolve_path(sound_id, path) != TIG_OK) {
            return -1;
        }
        break;
    case MISC_SOUND_DESTROYING:
        if (sound_effect == 0) {
            return -1;
        }

        sound_id = obj_type == OBJ_TYPE_WEAPON ? sound_effect + 2 : sound_effect + 1;
        if (gsound_resolve_path(sound_id, path) != TIG_OK) {
            return -1;
        }
        break;
    case MISC_SOUND_ANIMATING:
        if (sound_effect != 0) {
            sound_id = sound_effect + 2;
        }
        break;
    }

    return sound_id;
}
