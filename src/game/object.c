#include "game/object.h"

#include <tig/tig.h>

#include "game/lib/effect.h"
#include "game/lib/object.h"
#include "game/lib/stat.h"

// 0x43D410
int object_get_hp_pts(object_id_t obj)
{
    return obj_f_get_int32(obj, OBJ_F_HP_PTS);
}

// 0x43D4C0
int object_get_hp_adj(object_id_t obj)
{
    return obj_f_get_int32(obj, OBJ_F_HP_ADJ);
}

// 0x43D4E0
int object_set_hp_adj(object_id_t obj, int value)
{
    obj_f_set_int32(obj, OBJ_F_HP_ADJ, value);
    sub_460240(obj);
    return value;
}

// 0x43D510
int object_get_hp_damage(object_id_t obj)
{
    return obj_f_get_int32(obj, OBJ_F_HP_DAMAGE);
}

// 0x43D530
int object_set_hp_damage(object_id_t obj, int value)
{
    int type;

    if ((tig_net_flags & 0x1) != 0 && (tig_net_flags & 0x2) == 0) {
        return 0;
    }

    if (value < 0) {
        value = 0;
    }

    sub_4EFDD0(obj, OBJ_F_HP_DAMAGE, value);

    if (value > 0) {
        type = obj_f_get_int32(obj, OBJ_F_TYPE);
        if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
            sub_45EAB0(obj);
        }
    }

    sub_4EE1D0(obj);

    return value;
}

// 0x43D5A0
int sub_43D5A0(object_id_t obj)
{
    int value;
    int type;

    value = object_get_hp_adj(obj) + sub_43D690(obj);
    type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        value = effect_adjust_max_hit_points(obj, sub_43D630(obj) + value);
    }
    return value;
}

// 0x43D600
int sub_43D600(object_id_t obj)
{
    return sub_43D5A0(obj) - object_get_hp_damage(obj);
}

// 0x43D630
int sub_43D630(object_id_t obj)
{
    int type;

    type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        return stat_level(obj, STAT_WILLPOWER)
            + 2 * (stat_level(obj, STAT_STRENGTH) + stat_level(obj, STAT_LEVEL))
            + 4;
    }

    return 0;
}

// 0x43D690
int sub_43D690(object_id_t obj)
{
    int type;

    type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        return 4 * object_get_hp_pts(obj);
    }

    return object_get_hp_pts(obj);
}

// 0x43D880
int object_get_ac(object_id_t obj, bool a2)
{
    int ac;
    int type;
    int index;
    object_id_t item_obj;

    ac = obj_f_get_int32(obj, OBJ_F_AC);
    type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        for (index = 1000; index <= 1008; index++) {
            item_obj = item_wield_get(obj, index);
            if (item_obj != OBJ_HANDLE_NULL) {
                ac += sub_465BA0(obj, item_obj, a2);
            }
        }
    } else if (type == OBJ_TYPE_ITEM_ARMOR) {
        ac += sub_465BA0(obj, OBJ_HANDLE_NULL, a2);
    }

    if (ac < 0) {
        ac = 0;
    } else if (ac > 95) {
        ac = 95;
    }

    return ac;
}

// 0x43D940
bool sub_43D940(object_id_t obj)
{
    int type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PROJECTILE
        || type == OBJ_TYPE_CONTAINER
        || type == OBJ_TYPE_PC
        || type == OBJ_TYPE_NPC
        || (type >= OBJ_TYPE_WEAPON && type <= OBJ_TYPE_ITEM_GENERIC)) {
        return false;
    }
    return true;
}

// 0x43D990
bool sub_43D990(object_id_t obj)
{
    int type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PROJECTILE
        || type == OBJ_TYPE_CONTAINER
        || type == OBJ_TYPE_PC
        || type == OBJ_TYPE_NPC
        || (type >= OBJ_TYPE_WEAPON && type <= OBJ_TYPE_ITEM_GENERIC)) {
        return false;
    }
    return (obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_DYNAMIC) == 0;
}

// 0x43EAF0
void object_set_offset(object_id_t obj, int offset_x, int offset_y)
{
    if ((obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_DESTROYED) == 0) {
        sub_4423E0(obj, offset_x, offset_y);
    }
}

// 0x43EB30
void object_set_current_aid(object_id_t obj, tig_art_id_t aid)
{
    TigRect dirty_rect;
    TigRect update_rect;

    if ((obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_DESTROYED) == 0) {
        object_get_rect(obj, 0x7, &dirty_rect);
        obj_f_set_int32(obj, OBJ_F_CURRENT_AID, aid);
        obj_f_set_int32(obj, obj_f_get_int32(obj, OBJ_F_RENDER_FLAGS) & ~0x8000000);
        object_get_rect(obj, 0x7, &update_rect);
        tig_rect_union(&dirty_rect, &update_rect, &dirty_rect);
        dword_5E2EB4(&dirty_rect);
        sub_43F710(obj);
    }
}

// 0x43EBD0
void object_set_light(object_id_t obj, unsigned int flags, tig_art_id_t aid, tig_color_t color)
{
    int light_handle;

    if (aid == TIG_ART_ID_INVALID) {
        light_handle = obj_f_get_int32(obj, OBJ_F_LIGHT_HANDLE);
        if (light_handle != 0) {
            sub_4D8620(light_handle);
        }
    }

    obj_f_set_int32(obj, OBJ_F_LIGHT_FLAGS, flags);
    obj_f_set_int32(obj, OBJ_F_LIGHT_AID, aid);
    obj_f_set_int32(obj, OBJ_F_LIGHT_COLOR, color);
    obj_f_set_int32(obj, obj_f_get_int32(obj, OBJ_F_RENDER_FLAGS) & ~0x80000000);
    sub_4D9590(obj, true);
}

// 0x43EE10
void object_set_overlay_light(object_id_t obj, int index, unsigned int flags, tig_art_id_t aid, tig_color_t color)
{
    int light_handle;

    if (aid == TIG_ART_ID_INVALID) {
        light_handle = sub_407470(obj, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light_handle != 0) {
            sub_4D8620(light_handle);
        }
    }

    sub_4074E0(obj, index, OBJ_F_OVERLAY_LIGHT_FLAGS, flags);
    sub_4074E0(obj, index, OBJ_F_OVERLAY_LIGHT_AID, aid);
    sub_4074E0(obj, index, OBJ_F_OVERLAY_LIGHT_COLOR, color);
    obj_f_set_int32(obj, obj_f_get_int32(obj, OBJ_F_RENDER_FLAGS) & ~0x80000000);
    sub_4D9590(obj, true);
}

// 0x43EEB0
void object_set_blit_scale(object_id_t obj, int value)
{
    TigRect dirty_rect;
    TigRect update_rect;

    object_get_rect(obj, 0x7, &dirty_rect);
    obj_f_set_int32(obj, OBJ_F_BLIT_SCALE, value);
    object_get_rect(obj, 0x7, &update_rect);
    tig_rect_union(&dirty_rect, &update_rect, &dirty_rect);
    dword_5E2EB4(&dirty_rect);
}

// 0x43EF10
void object_add_flags(object_id_t obj, unsigned int flags)
{
    TigRect dirty_rect;
    TigRect update_rect;

    if ((obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_OFF) == 0) {
        object_get_rect(obj, 0x7, &dirty_rect);
        sub_43D0E0(obj, flags);
        object_get_rect(obj, 0x7, &update_rect);
        tig_rect_union(&dirty_rect, &update_rect, &dirty_rect);
        dword_5E2EB4(&dirty_rect);
        obj_f_set_int32(obj, obj_f_get_int32(obj, OBJ_F_RENDER_FLAGS) & ~0x5000000);
    }
}

// 0x43EFA0
void object_remove_flags(object_id_t obj, unsigned int flags)
{
    TigRect dirty_rect;
    TigRect update_rect;

    if ((obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_OFF) == 0) {
        object_get_rect(obj, 0x7, &dirty_rect);
        sub_43D280(obj, flags);
        object_get_rect(obj, 0x7, &update_rect);
        tig_rect_union(&dirty_rect, &update_rect, &dirty_rect);
        dword_5E2EB4(&dirty_rect);
        obj_f_set_int32(obj, obj_f_get_int32(obj, OBJ_F_RENDER_FLAGS) & ~0x5000000);
    }
}

// 0x43F030
void sub_43F030(object_id_t obj)
{
    TigRect rect;

    if ((obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_OFF) == 0) {
        object_get_rect(obj, 0x7, &rect);
        dword_5E2EB4(&rect);
        obj_f_set_int32(obj, obj_f_get_int32(obj, OBJ_F_RENDER_FLAGS) & ~0x7000000);
    }
}

// 0x43F090
void sub_43F090(object_id_t obj)
{
    tig_art_id_t aid;
    TigRect rect;

    if ((obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_OFF) == 0) {
        aid = obj_f_get_int32(obj, OBJ_F_CURRENT_AID);
        aid = tig_art_palette_set(tig_art_palette_get(aid) + 1);
        if (sub_502E00(aid) == TIG_OK) {
            aid = tig_art_palette_set(aid, 0);
        }
        obj_f_set_int32(obj, OBJ_F_CURRENT_AID, aid);
        object_get_rect(obj, 0, &rect);
        dword_5E2EB4(&rect);
        obj_f_set_int32(obj, obj_f_get_int32(obj, OBJ_F_RENDER_FLAGS) & ~0x3000000);
    }
}

// 0x43F130
void sub_43F130(object_id_t obj, int palette)
{
    tig_art_id_t aid;
    TigRect rect;

    if ((obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_OFF) == 0) {
        aid = obj_f_get_int32(obj, OBJ_F_CURRENT_AID);
        aid = tig_art_palette_set(aid, palette);
        // NOTE: Looks wrong.
        if (sub_502E00(aid) != TIG_OK) {
            obj_f_set_int32(obj, OBJ_F_CURRENT_AID, aid);
            object_get_rect(obj, 0, &rect);
            dword_5E2EB4(&rect);
            obj_f_set_int32(obj, obj_f_get_int32(obj, OBJ_F_RENDER_FLAGS) & ~0x2000000);
        }
    }
}

// 0x43F630
bool object_is_destroyed(object_id_t obj)
{
    if ((obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_OFF) != 0) {
        return false;
    }

    switch (obj_f_get_int32(obj, OBJ_F_TYPE)) {
    case OBJ_TYPE_PORTAL:
        return (obj_f_get_int32(obj, OBJ_F_PORTAL_FLAGS) & OPF_BUSTED) != 0;
    case OBJ_TYPE_CONTAINER:
        return (obj_f_get_int32(obj, OBJ_F_CONTAINER_FLAGS) & OCOF_BUSTED) != 0;
    case OBJ_TYPE_SCENERY:
        return (obj_f_get_int32(obj, OBJ_F_SCENERY_FLAGS) & OSCF_BUSTED) != 0;
    case OBJ_TYPE_WEAPON:
    case OBJ_TYPE_AMMO:
    case OBJ_TYPE_ITEM_ARMOR:
    case OBJ_TYPE_ITEM_GOLD:
    case OBJ_TYPE_ITEM_FOOD:
    case OBJ_TYPE_ITEM_SCROLL:
    case OBJ_TYPE_ITEM_KEY:
    case OBJ_TYPE_ITEM_KEY_RING:
    case OBJ_TYPE_ITEM_WRITTEN:
    case OBJ_TYPE_ITEM_GENERIC:
        return tig_art_item_id_destroyed_get(obj_f_get_int32(obj)) != 0;
    case OBJ_TYPE_TRAP:
        return (obj_f_get_int32(obj, OBJ_F_TRAP_FLAGS) & OTF_BUSTED) != 0;
    }

    return false;
}
