#include "game/portal.h"

#include "game/obj.h"

// 0x4F08C0
bool portal_is_open(int64_t obj)
{
    return tig_art_id_frame_get(obj_field_int32_get(obj, OBJ_F_CURRENT_AID)) != 0;
}

// 0x4F08F0
void portal_toggle(int64_t obj)
{
    if (tig_art_id_frame_get(obj_field_int32_get(obj, OBJ_F_CURRENT_AID)) == 0) {
        while (portal_open(obj)) {
        }
    } else {
        while (portal_close(obj)) {
        }
    }
}

// 0x4F0950
bool portal_open(int64_t door_obj)
{
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;
    int frame;
    int rotation;

    if (obj_field_int32_get(door_obj, OBJ_F_TYPE) != OBJ_TYPE_PORTAL) {
        return false;
    }

    art_id = obj_field_int32_get(door_obj, OBJ_F_CURRENT_AID);
    if (tig_art_anim_data(art_id, &art_anim_data) != TIG_OK) {
        return false;
    }

    if (art_anim_data.num_frames == 1) {
        return false;
    }

    frame = tig_art_id_frame_get(art_id);
    if (tig_art_portal_id_type_get(art_id) == TIG_ART_PORTAL_TYPE_WINDOW) {
        if (frame == 1) {
            return false;
        }

        art_id = tig_art_id_frame_set(art_id, 1);
        object_flags_set(door_obj, OF_SHOOT_THROUGH | OF_NO_BLOCK);
    } else {
        rotation = tig_art_id_rotation_get(art_id);
        if ((rotation & 1) != 0) {
            rotation--;
        }

        if (rotation == 6 || rotation == 0) {
            if (frame >= 4 && frame <= 6) {
                if (frame == 6) {
                    return false;
                }

                art_id = tig_art_id_frame_set(art_id, frame + 1);
            } else {
                art_id = tig_art_id_frame_set(art_id, 4);
                object_flags_set(door_obj, OF_SHOOT_THROUGH | OF_NO_BLOCK | OF_SEE_THROUGH);
            }
        } else {
            if (frame >= 1 && frame <= 3) {
                if (frame == 3) {
                    return false;
                }

                art_id = tig_art_id_frame_set(art_id, frame + 1);
            } else {
                art_id = tig_art_id_frame_set(art_id, 1);
                object_flags_set(door_obj, OF_SHOOT_THROUGH | OF_NO_BLOCK | OF_SEE_THROUGH);
            }
        }
    }

    object_set_current_aid(door_obj, art_id);

    return true;
}

// 0x4F0A90
bool portal_close(int64_t door_obj)
{
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;
    int frame;
    int rotation;

    if (obj_field_int32_get(door_obj, OBJ_F_TYPE) != OBJ_TYPE_PORTAL) {
        return false;
    }

    art_id = obj_field_int32_get(door_obj, OBJ_F_CURRENT_AID);
    if (tig_art_anim_data(art_id, &art_anim_data) != TIG_OK) {
        return false;
    }

    if (art_anim_data.num_frames == 1) {
        return false;
    }

    frame = tig_art_id_frame_get(art_id);
    if (frame == 0) {
        return false;
    }

    if (tig_art_portal_id_type_get(art_id) == TIG_ART_PORTAL_TYPE_WINDOW) {
        art_id = tig_art_id_frame_set(art_id, 0);
        object_flags_unset(door_obj, OF_SHOOT_THROUGH | OF_NO_BLOCK);
    } else {
        rotation = tig_art_id_rotation_get(art_id);
        if ((rotation & 1) != 0) {
            rotation--;
        }

        if (rotation == 6 || rotation == 0) {
            if (frame >= 4 && frame <= 6) {
                if (frame == 4) {
                    object_flags_unset(door_obj, OF_SHOOT_THROUGH | OF_NO_BLOCK | OF_SEE_THROUGH);
                    art_id = tig_art_id_frame_set(art_id, 0);
                } else {
                    art_id = tig_art_id_frame_set(art_id, frame - 1);
                }
            } else {
                art_id = tig_art_id_frame_set(art_id, 6);
            }
        } else {
            if (frame >= 1 && frame <= 3) {
                if (frame == 1) {
                    object_flags_unset(door_obj, OF_SHOOT_THROUGH | OF_NO_BLOCK | OF_SEE_THROUGH);
                    art_id = tig_art_id_frame_set(art_id, 0);
                } else {
                    art_id = tig_art_id_frame_set(art_id, frame - 1);
                }
            } else {
                art_id = tig_art_id_frame_set(art_id, 3);
            }
        }
    }

    object_set_current_aid(door_obj, art_id);

    return true;
}
