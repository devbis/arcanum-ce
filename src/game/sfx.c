#include "game/sfx.h"

#include "game/a_name.h"
#include "game/gsound.h"
#include "game/item.h"
#include "game/material.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/random.h"
#include "game/tile.h"

// 0x5BB928
static int dword_5BB928[MATERIAL_COUNT] = {
    /*  MATERIAL_STONE */ 3,
    /*  MATERIAL_BRICK */ 3,
    /*   MATERIAL_WOOD */ 4,
    /*  MATERIAL_PLANT */ 4,
    /*  MATERIAL_FLESH */ 0,
    /*  MATERIAL_METAL */ 2,
    /*  MATERIAL_GLASS */ 1,
    /*  MATERIAL_CLOTH */ 0,
    /* MATERIAL_LIQUID */ 0,
    /*  MATERIAL_PAPER */ 0,
    /*    MATERIAL_GAS */ 0,
    /*  MATERIAL_FORCE */ 1,
    /*   MATERIAL_FIRE */ 0,
    /* MATERIAL_POWDER */ 0,
};

// 0x5BB960
static int dword_5BB960[MATERIAL_COUNT] = {
    /*  MATERIAL_STONE */ 3,
    /*  MATERIAL_BRICK */ 3,
    /*   MATERIAL_WOOD */ 4,
    /*  MATERIAL_PLANT */ 4,
    /*  MATERIAL_FLESH */ 0,
    /*  MATERIAL_METAL */ 2,
    /*  MATERIAL_GLASS */ 5,
    /*  MATERIAL_CLOTH */ 6,
    /* MATERIAL_LIQUID */ 0,
    /*  MATERIAL_PAPER */ 7,
    /*    MATERIAL_GAS */ 0,
    /*  MATERIAL_FORCE */ 1,
    /*   MATERIAL_FIRE */ 0,
    /* MATERIAL_POWDER */ 0,
};

// 0x5BB998
static int dword_5BB998[6] = {
    2904,
    2912,
    2916,
    2920,
    2928,
    2932,
};

// 0x4F0BF0
int sub_4F0BF0(int64_t item_obj, int64_t parent_obj, int64_t target_obj, int type)
{
    int sound_id = -1;
    int sound_effect;
    int material;
    int item_sound;
    int target_sound;
    char path[TIG_MAX_PATH];

    if (item_obj != OBJ_HANDLE_NULL) {
        sound_effect = obj_field_int32_get(item_obj, OBJ_F_SOUND_EFFECT);
    } else {
        sound_effect = 0;
    }

    switch (type) {
    case 0:
        if (item_obj != OBJ_HANDLE_NULL) {
            if (obj_field_int32_get(item_obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN) == 9056) {
                sound_id = 5958;
            } else {
                material = obj_field_int32_get(item_obj, OBJ_F_MATERIAL);
                item_sound = dword_5BB960[material];
                if (item_sound == 2 && item_weight(item_obj, parent_obj) <= 2000) {
                    item_sound = 1;
                }
                sound_id = 5950 + item_sound;
            }
        }
        break;
    case 1:
        if (item_obj != OBJ_HANDLE_NULL) {
            if (obj_field_int32_get(item_obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN) == 9056) {
                sound_id = 5958;
            } else {
                material = obj_field_int32_get(item_obj, OBJ_F_MATERIAL);
                item_sound = dword_5BB960[material];
                if (item_sound == 2 && item_weight(item_obj, parent_obj) <= 2000) {
                    item_sound = 1;
                }
                sound_id = 5960 + item_sound;
            }
        }
        break;
    case 2:
        if (sound_effect != 0) {
            sound_id = sound_effect;
        }
        break;
    case 3:
        if (sound_effect != 0) {
            sound_id = sound_effect + 7;
        }
        break;
    case 4:
    case 6:
        if (target_obj != OBJ_HANDLE_NULL) {
            if (sound_effect != 0) {
                if (type == 6) {
                    sound_id = sound_effect + 6;
                } else {
                    sound_id = sound_effect + 4;
                    if (random_between(0, 1) == 1) {
                        sound_id++;
                    }
                }

                if (!gsound_resolve_path(sound_id, path)) {
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
            target_sound = dword_5BB928[material];

            if (item_obj != OBJ_HANDLE_NULL) {
                material = obj_field_int32_get(item_obj, OBJ_F_MATERIAL);
                item_sound = dword_5BB928[material];
                if (item_sound == 2 && item_weight(item_obj, parent_obj) <= 2000) {
                    item_sound = 1;
                }
            } else {
                item_sound = 0;
            }

            sound_id = 3 * target_sound + random_between(0, 1) + 20 * item_sound + 7000;
        }
        break;
    case 5:
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
int sub_4F0ED0(int64_t obj, int a2)
{
    int64_t pc_obj;
    tig_art_id_t art_id;
    int armor_type;
    int base;
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

    if (a2 != 7) {
        return obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT) + a2;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    armor_type = tig_art_critter_id_armor_get(art_id);
    if (armor_type == TIG_ART_ARMOR_TYPE_PLATE
        || armor_type == TIG_ART_ARMOR_TYPE_PLATE_CLASSIC) {
        base = 2900;
    } else {
        loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
        tile_art_id = tile_art_id_at(loc);
        tile_sound_type = a_name_tile_sound(tile_art_id);
        base = dword_5BB998[tile_sound_type];
        if (armor_type == TIG_ART_ARMOR_TYPE_CHAIN
            && (tile_sound_type == 5
                || tile_sound_type == 3
                || tile_sound_type == 0)) {
            base += 4;
        }
    }

    return random_between(0, 3) + base;
}

// 0x4F0FD0
int sub_4F0FD0(int64_t obj, int a2)
{
    return obj != OBJ_HANDLE_NULL && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER
        ? obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT) + a2
        : -1;
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
