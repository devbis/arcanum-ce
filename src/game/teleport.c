#include "game/teleport.h"

#include <stdio.h>

#include "game/ai.h"
#include "game/anim.h"
#include "game/combat.h"
#include "game/critter.h"
#include "game/gamelib.h"
#include "game/gmovie.h"
#include "game/gsound.h"
#include "game/light.h"
#include "game/magictech.h"
#include "game/map.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/obj_private.h"
#include "game/object.h"
#include "game/player.h"
#include "game/reaction.h"
#include "game/ui.h"
#include "game/wallcheck.h"

typedef struct S6018B8 {
    int64_t loc;
    int64_t obj;
    struct S6018B8* next;
} S6018B8;

// See 0x4D3F00.
static_assert(sizeof(S6018B8) == 0x18, "wrong size");

static bool sub_4D3460(TeleportData* teleport_data);
static bool sub_4D3760(int64_t obj, int64_t loc);
static bool sub_4D39A0(TeleportData* teleport_data);
static void sub_4D3D60(int64_t obj);
static void sub_4D3E20(int64_t obj);
static void sub_4D3E80();
static S6018B8* sub_4D3EB0();
static void sub_4D3EE0(S6018B8* node);
static void sub_4D3F00();
static void sub_4D3F30();

// 0x601840
static S6018B8* dword_601840;

// 0x601844
static bool dword_601844;

// 0x601848
static GameContextF8* dword_601848;

// 0x60184C
static bool dword_60184C;

// 0x601850
static GameContextFC* dword_601850;

// 0x601858
static TeleportData stru_601858;

// 0x6018B8
static S6018B8* off_6018B8;

// 0x6018BC
static bool dword_6018BC;

// 0x4D32D0
bool teleport_init(GameInitInfo* init_info)
{
    dword_601848 = init_info->field_8;
    dword_601850 = init_info->field_C;
    dword_601844 = false;

    return true;
}

// 0x4D3300
bool teleport_reset()
{
    sub_4D3F30();
    dword_601844 = false;
    return true;
}

// 0x4D3320
void teleport_exit()
{
    teleport_reset();
}

// 0x4D3330
void teleport_ping()
{
    if (dword_601844) {
        dword_60184C = true;
        sub_4D3460(&stru_601858);
        dword_60184C = false;

        dword_601844 = false;

        if ((stru_601858.flags & TELEPORT_0x80000000) != 0
            && (stru_601858.flags & TELEPORT_0x0020) != 0) {
            sub_402FA0();
        }
    }
}

// 0x4D3380
bool sub_4D3380(TeleportData* teleport_data)
{
    if (dword_601844) {
        if ((stru_601858.flags & TELEPORT_0x80000000) != 0) {
            return false;
        }
    }

    stru_601858 = *teleport_data;
    dword_601844 = true;

    if (player_is_pc_obj(teleport_data->obj)) {
        stru_601858.flags |= TELEPORT_0x80000000;

        if ((stru_601858.flags & TELEPORT_FADE_OUT) != 0) {
            sub_4BDFA0(&(stru_601858.fade_out));
        }

        if ((stru_601858.flags & TELEPORT_0x0020) != 0) {
            sub_402FC0();
            sub_402F90();
        }
    }

    return true;
}

// 0x4D3410
bool sub_4D3410()
{
    return dword_60184C;
}

// 0x4D3420
bool sub_4D3420(int64_t obj)
{
    S6018B8* node;

    node = dword_601840;
    while (node != NULL) {
        if (node->obj == obj) {
            return true;
        }
        node = node->next;
    }

    return false;
}

// 0x4D3460
bool sub_4D3460(TeleportData* teleport_data)
{
    int map;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        if ((teleport_data->flags & TELEPORT_FADE_OUT) != 0) {
            sub_4BDFA0(&(teleport_data->fade_out));
        }
    }

    if ((teleport_data->flags & TELEPORT_SOUND) != 0) {
        sub_41B930(teleport_data->sound_id, 1, teleport_data->obj);
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        if ((teleport_data->flags & TELEPORT_MOVIE1) != 0) {
            gmovie_play(teleport_data->movie1, teleport_data->movie_flags1, 0);
        }

        if ((teleport_data->flags & TELEPORT_MOVIE2) != 0) {
            gmovie_play(teleport_data->movie2, teleport_data->movie_flags2, 0);
        }

        if ((teleport_data->flags & TELEPORT_TIME) != 0) {
            DateTime datetime;

            sub_45A950(&datetime, 1000 * teleport_data->time);
            sub_45C200(&datetime);
        }
    }

    map = sub_40FF40();
    if (teleport_data->map == -1) {
        teleport_data->map = map;
    }

    if (!sub_4A2BA0()) {
        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return false;
        }

        if (teleport_data->map != map) {
            return false;
        }
    }

    if ((teleport_data->flags & TELEPORT_0x0100) == 0) {
        ObjectList objects;
        ObjectNode* node;

        object_get_followers(teleport_data->obj, &objects);
        node = objects.head;
        while (node != NULL) {
            if (sub_45D800(node->obj)) {
                sub_4AA7A0(node->obj);
            }
            node = node->next;
        }
        object_list_destroy(&objects);
    }

    sub_4D3760(teleport_data->obj, teleport_data->loc);

    if (dword_6018BC) {
        sub_43CB70();
        wallcheck_flush();
    }

    if (teleport_data->map != map && sub_40DA20(teleport_data->obj)) {
        sub_459F50();
        sub_437980();
        sub_45C720(teleport_data->map);
    }

    if (!sub_4D39A0(teleport_data)) {
        return false;
    }

    if (dword_6018BC) {
        ObjectID oid;
        int64_t obj;

        oid.type = 0;
        if (obj_field_int32_get(teleport_data->obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
            oid = sub_407EF0(teleport_data->obj);
        } else if (obj_field_int32_get(teleport_data->obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
            oid = sub_407EF0(sub_45DDA0(teleport_data->obj));
        }

        dword_6018BC = false;

        if (teleport_data->map != map) {
            if (!map_open_in_game(teleport_data->map, false, false)) {
                return false;
            }

            sub_4B8CE0(teleport_data->loc);
            sub_40D860();
        }

        if (oid.type != 0) {
            obj = objp_perm_lookup(oid);
            if (obj != OBJ_HANDLE_NULL) {
                sub_4EF1E0(teleport_data->loc, obj);
            }
        }
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        if ((teleport_data->flags & TELEPORT_FADE_IN) != 0) {
            dword_601848(NULL);
            dword_601850();
            tig_window_display();
            sub_4BDFA0(&(teleport_data->fade_in));
        }
    }

    return true;
}

// 0x4D3760
bool sub_4D3760(int64_t obj, int64_t loc)
{
    int obj_type;
    unsigned int flags;
    int inventory_num_fld;
    int inventory_list_fld;
    S6018B8* node;

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    flags = obj_field_int32_get(obj, OBJ_F_FLAGS);

    if ((flags & OF_TEXT) != 0) {
        sub_4EF630(obj);
    }

    if ((flags & OF_TEXT_FLOATER) != 0) {
        sub_4EF5C0(obj);
    }

    if (obj_type_is_critter(obj_type)) {
        ObjectList objects;
        ObjectNode* obj_node;
        int64_t v1;

        object_get_followers(obj, &objects);
        obj_node = objects.head;
        while (obj_node != NULL) {
            if ((obj_field_int32_get(obj_node->obj, OBJ_F_NPC_FLAGS) & ONF_AI_WAIT_HERE) == 0) {
                if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
                    v1 = sub_4C1110(obj_node->obj);
                    if (v1 != OBJ_HANDLE_NULL) {
                        sub_460A20(v1, 0);
                    }
                }

                if (!sub_4D3760(obj_node->obj, loc)) {
                    object_list_destroy(&objects);
                    return false;
                }
            }
            obj_node = obj_node->next;
        }

        object_list_destroy(&objects);
    }

    if (obj_type_is_critter(obj_type)) {
        inventory_num_fld = OBJ_F_CRITTER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CRITTER_INVENTORY_LIST_IDX;
    } else if (obj_type == OBJ_TYPE_CONTAINER) {
        inventory_num_fld = OBJ_F_CONTAINER_INVENTORY_NUM;
        inventory_list_fld = OBJ_F_CONTAINER_INVENTORY_LIST_IDX;
    } else {
        inventory_num_fld = -1;
    }

    if (inventory_num_fld != -1) {
        int cnt;
        int inv_size;
        int index;
        int64_t item_obj;
        int64_t item_loc;

        cnt = obj_field_int32_get(obj, inventory_num_fld);
        inv_size = obj_arrayfield_length_get(obj, inventory_list_fld);
        if (cnt != inv_size) {
            tig_debug_printf("Inventory array count does not equal associated num field on teleport.  Array: %d, Field: %d\n",
                inv_size,
                cnt);
            return false;
        }

        for (index = 0; index < cnt; index++) {
            item_obj = obj_arrayfield_handle_get(obj, inventory_list_fld, index);
            item_loc = obj_field_int64_get(item_obj, OBJ_F_LOCATION);
            if (!sub_4D3760(item_obj, item_loc)) {
                return false;
            }
        }
    }

    node = sub_4D3EB0();
    node->loc = loc;
    node->obj = obj;
    node->next = dword_601840;
    dword_601840 = node;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        dword_6018BC = true;
    }

    return true;
}

// 0x4D39A0
bool sub_4D39A0(TeleportData* teleport_data)
{
    int curr_map;
    char* curr_map_name;
    char curr_map_path[TIG_MAX_PATH];
    char* dst_map_name;
    char dst_map_path[TIG_MAX_PATH];
    S6018B8* node;
    unsigned int flags;
    char path[TIG_MAX_PATH];
    TigFile* stream;

    curr_map = sub_40FF40();
    if (curr_map != teleport_data->map) {
        if (!map_get_name(curr_map, &curr_map_name)) {
            return false;
        }

        strcpy(curr_map_path, "Save\\Current");
        strcat(curr_map_path, "\\maps\\");
        strcat(curr_map_path, curr_map_name);

        if (!map_get_name(teleport_data->map, &dst_map_name)) {
            return false;
        }

        strcpy(dst_map_path, "Save\\Current");
        strcat(dst_map_path, "\\maps\\");
        strcat(dst_map_path, dst_map_name);
        tig_file_mkdir_ex(dst_map_path);

        node = dword_601840;
        while (node != NULL) {
            sub_4D3E20(node->obj);
            node = node->next;
        }

        node = dword_601840;
        while (node != NULL) {
            sub_4064B0(node->obj);
            node = node->next;
        }

        node = dword_601840;
        while (node != NULL) {
            flags = obj_field_int32_get(node->obj, OBJ_F_FLAGS);
            flags |= OF_DYNAMIC;
            flags |= OF_TELEPORTED;
            obj_field_int32_set(node->obj, OBJ_F_FLAGS, flags);
            obj_field_int64_set(node->obj, OBJ_F_LOCATION, teleport_data->loc);
            obj_field_int32_set(node->obj, OBJ_F_OFFSET_X, 0);
            obj_field_int32_set(node->obj, OBJ_F_OFFSET_Y, 0);

            sprintf(path, "%s\\mobile.mdy", dst_map_path);
            stream = tig_file_fopen(path, "ab");
            if (stream == NULL) {
                return false;
            }

            if (!obj_write(stream, node->obj)) {
                // FIXME: Leaking file.
                return false;
            }

            tig_file_fclose(stream);
            flags &= ~OF_DYNAMIC;
            flags &= ~OF_TELEPORTED;
            flags |= OF_DESTROYED;
            flags |= OF_EXTINCT;
            obj_field_int32_set(node->obj, OBJ_F_FLAGS, flags);

            node = node->next;
        }

        node = dword_601840;
        while (node != NULL) {
            sub_406520(node->obj);
            node = node->next;
        }

        sub_4D3E80();
        return true;
    }
}

// 0x4D3D60
void sub_4D3D60(int64_t obj)
{
    int type;

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    sub_424070(obj, 5, 0, 1);

    // NOTE: Conditions looks odd, check (note fallthrough from npc block).
    switch (type) {
    case OBJ_TYPE_NPC:
        sub_45F710(obj);
        mp_obj_field_obj_set(obj, OBJ_F_NPC_COMBAT_FOCUS, OBJ_HANDLE_NULL);
        mp_obj_field_obj_set(obj, OBJ_F_NPC_WHO_HIT_ME_LAST, OBJ_HANDLE_NULL);
        sub_4F0500(obj, OBJ_F_NPC_SHIT_LIST_IDX);
        mp_obj_field_obj_set(obj, OBJ_F_NPC_SUBSTITUTE_INVENTORY, OBJ_HANDLE_NULL);
    case OBJ_TYPE_PC:
        mp_obj_field_obj_set(obj, OBJ_F_CRITTER_FLEEING_FROM, OBJ_HANDLE_NULL);
        if (player_is_pc_obj(obj)) {
            sub_460B20();
        }
    }

    sub_4D9990(obj);
    sub_4D9A90(obj);
}

// 0x4D3E20
void sub_4D3E20(int64_t obj)
{
    sub_4D3D60(obj);
    combat_critter_deactivate_combat_mode(obj);
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        sub_45F710(obj);
    }
    sub_4AD7D0(obj);
    sub_4601D0(obj);
    sub_43CF70(obj);
}

// 0x4D3E80
void sub_4D3E80()
{
    S6018B8* next;

    while (dword_601840 != NULL) {
        next = dword_601840->next;
        sub_4D3EE0(dword_601840);
        dword_601840 = next;
    }
}

// 0x4D3EB0
S6018B8* sub_4D3EB0()
{
    S6018B8* node;

    node = off_6018B8;
    if (node == NULL) {
        sub_4D3F00();
        node = off_6018B8;
    }

    off_6018B8 = node->next;
    node->next = NULL;

    return node;
}

// 0x4D3EE0
void sub_4D3EE0(S6018B8* node)
{
    node->next = off_6018B8;
    off_6018B8 = node;
}

// 0x4D3F00
void sub_4D3F00()
{
    int index;
    S6018B8* node;

    if (off_6018B8 == NULL) {
        for (index = 0; index < 32; index++) {
            node = (S6018B8*)MALLOC(sizeof(*node));
            node->next = off_6018B8;
            off_6018B8 = node;
        }
    }
}

// 0x4D3F30
void sub_4D3F30()
{
    S6018B8* curr;
    S6018B8* next;

    curr = off_6018B8;
    while (curr != NULL) {
        next = curr->next;
        FREE(curr);
        curr = next;
    }
    off_6018B8 = NULL;
}
