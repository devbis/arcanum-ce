#include "game/player.h"

#include "game/item.h"
#include "game/level.h"
#include "game/location.h"
#include "game/map.h"
#include "game/obj_private.h"
#include "game/object.h"
#include "game/proto.h"
#include "game/scroll.h"
#include "game/ui.h"

// 0x5D1138
static ObjectID stru_5D1138;

// 0x5D1150
static object_id_t qword_5D1150;

// 0x5D1158
static object_id_t pcObj;

// 0x5D1160
static int64_t qword_5D1160;

// 0x739E5C
bool player_editor;

// 0x40D6C0
bool player_init(GameInitInfo* init_info)
{
    stru_5D1138.type = OID_TYPE_NULL;
    player_editor = init_info->editor;

    return true;
}

// 0x40D6E0
void player_reset()
{
    if (pcObj != OBJ_HANDLE_NULL) {
        pcObj = OBJ_HANDLE_NULL;
        stru_5D1138.type = OID_TYPE_NULL;
    }

    qword_5D1150 = OBJ_HANDLE_NULL;
}

// 0x40D720
void player_exit()
{
    if (pcObj != OBJ_HANDLE_NULL) {
        pcObj = OBJ_HANDLE_NULL;
    }

    qword_5D1150 = OBJ_HANDLE_NULL;
}

// 0x40D760
bool player_save(TigFile* stream)
{
    ObjectID temp_object_id;

    if (stream == NULL) {
        return false;
    }

    if (pcObj == OBJ_HANDLE_NULL) {
        return false;
    }

    temp_object_id = sub_407EF0(pcObj);
    if (tig_file_fwrite(&temp_object_id, sizeof(temp_object_id), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x40D7C0
bool player_load(GameLoadInfo* load_info)
{
    ObjectID temp_object_id;
    long long location;

    if (load_info->stream == NULL) {
        return false;
    }

    if (tig_file_fread(&temp_object_id, sizeof(temp_object_id), 1, load_info->stream) != 1) {
        return false;
    }

    stru_5D1138 = temp_object_id;
    pcObj = objp_perm_lookup(temp_object_id);
    if (pcObj == OBJ_HANDLE_NULL) {
        return false;
    }

    location = obj_field_int64_get(pcObj, OBJ_F_LOCATION);
    scroll_set_center(location);
    sub_4B8CE0(location);
    sub_4604E0();

    return true;
}

// 0x40D860
void sub_40D860()
{
    if (stru_5D1138.type != OID_TYPE_NULL) {
        pcObj = objp_perm_lookup(stru_5D1138);
    }
}

// 0x40D8A0
void player_create()
{
    tig_art_id_t art_id;
    int map;
    int64_t x;
    int64_t y;
    int64_t loc;

    qword_5D1150 = sub_468570(15);

    map = sub_40FF40();
    if (map == 0
        || !map_get_starting_location(map, &x, &y)
        || (loc = LOCATION_MAKE(x, y)) == 0) {
        loc = sub_4B9810();
        if (!player_editor) {
            tig_debug_printf("Player: ERROR: Map loaded that is NOT in map list!\n");
        }
    }

    tig_debug_printf("player_create: loc: X: %d, Y: %d\n", LOCATION_GET_X(loc), LOCATION_GET_Y(loc));
    if (!object_create(qword_5D1150, loc, &pcObj)) {
        tig_debug_printf("player_create: Error: failed to create player!\n");
        exit(EXIT_FAILURE);
    }

    if (tig_art_critter_id_create(1, 0, 0, 0, 0, 4, 0, 0, 0, &art_id) != TIG_OK) {
        tig_debug_printf("player_create: Error: failed to create player ART!\n");
        exit(EXIT_FAILURE);
    }

    obj_field_int32_set(pcObj, OBJ_F_AID, art_id);
    obj_field_int32_set(pcObj, OBJ_F_CURRENT_AID, art_id);

    stru_5D1138 = sub_407EF0(pcObj);
}

// 0x40D9F0
bool player_is_pc_obj(object_id_t object_id)
{
    return object_id != OBJ_HANDLE_NULL && object_id == pcObj;
}

// 0x40DA20
bool sub_40DA20(object_id_t object_id)
{
    if (object_id != OBJ_HANDLE_NULL) {
        return obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_PC;
    } else {
        return false;
    }
}

// 0x40DA50
object_id_t player_get_pc_obj()
{
    object_id_t obj = pcObj;

    if (obj != OBJ_HANDLE_NULL) {
        if (sub_4E5470(pcObj) || stru_5D1138.type == OID_TYPE_NULL) {
            obj = pcObj;
        } else {
            pcObj = objp_perm_lookup(stru_5D1138);
            obj = pcObj;
        }
    }

    return obj;
}

// 0x40DAB0
bool sub_40DAB0()
{
    if (pcObj == OBJ_HANDLE_NULL) {
        return false;
    }

    sub_43CCA0(pcObj);
    pcObj = OBJ_HANDLE_NULL;
    stru_5D1138.type = OID_TYPE_NULL;

    return true;
}

// 0x40DAF0
bool sub_40DAF0(object_id_t obj)
{
    pcObj = obj;
    if (obj != OBJ_HANDLE_NULL) {
        stru_5D1138 = sub_407EF0(obj);
        sub_4604E0();
    } else {
        stru_5D1138.type = OID_TYPE_NULL;
    }

    return true;
}

// 0x40DB50
void sub_40DB50(PlayerSpec* player_spec)
{
    player_spec->field_0 = OBJ_HANDLE_NULL;
    player_spec->loc = 0;
    player_spec->field_28 = 0;
    player_spec->field_2C = -1;
}

// 0x40DB70
bool player_obj_create_player(PlayerSpec* player_spec)
{
    object_id_t old_handle = OBJ_HANDLE_NULL;
    int64_t loc;
    char str[80];

    if (player_spec->field_2C == -1) {
        qword_5D1160 = sub_468570(15);
    } else {
        qword_5D1160 = objp_perm_lookup(sub_407EF0(sub_4685A0(player_spec->field_2C)));
    }

    if ((player_spec->field_28 & PLAYER_SPEC_FLAG_0x2) != 0) {
        loc = player_spec->loc;
    } else {
        // TODO: Looks that same as above, check.
        loc = player_spec->loc;
    }

    if ((player_spec->field_28 & PLAYER_SPEC_FLAG_0x1) != 0) {
        old_handle = player_spec->field_0;
    }

    if ((player_spec->field_28 & PLAYER_SPEC_FLAG_0x4) != 0) {
        if (pcObj != OBJ_HANDLE_NULL) {
            sub_40DAB0();
        }
    }

    if ((player_spec->field_28 & PLAYER_SPEC_FLAG_0x1) != 0) {
        objid_id_to_str(str, player_spec->field_8);
        tig_debug_printf("player_obj_create_player: Player ID: %s\n", str);
        if (!sub_43CBF0(qword_5D1160, loc, player_spec->field_8, &(player_spec->field_0))) {
            tig_debug_printf("player_obj_create_player: Error: failed to create player!\n");
            exit(EXIT_FAILURE);
        }
    } else {
        if (!object_create(qword_5D1160, loc, &(player_spec->field_0))) {
            tig_debug_printf("player_obj_create_player: Error: failed to create player!\n");
            exit(EXIT_FAILURE);
        }

        player_spec->field_8 = sub_407EF0(player_spec->field_0);
        objid_id_to_str(str, player_spec->field_8);
        tig_debug_printf("player_obj_create_player: Player ID: %s\n", str);
    }

    if ((player_spec->field_28 & PLAYER_SPEC_FLAG_0x1) != 0) {
        if (old_handle != player_spec->field_0) {
            // FIXME: Using println with newline character.
            tig_debug_println("ERROR: : object_create created new handle instead of using current one!\n");
        }
    }

    tig_debug_printf("player_obj_create_player: StartLoc: X: %d, Y: %d\n", LOCATION_GET_X(loc), LOCATION_GET_Y(loc));
    obj_field_int64_set(player_spec->field_0, OBJ_F_CRITTER_TELEPORT_DEST, loc);
    obj_field_int32_set(player_spec->field_0, OBJ_F_CRITTER_TELEPORT_MAP, sub_40FF40());

    if ((player_spec->field_28 & PLAYER_SPEC_FLAG_0x4) == 0) {
        pcObj = player_spec->field_0;
        stru_5D1138 = sub_407EF0(pcObj);
        tig_debug_printf("pcObj == %I64u\n", pcObj);
    }

    level_set_level(player_spec->field_0, 1);
    sub_463E20(player_spec->field_0);
    obj_field_int32_set(player_spec->field_0, OBJ_F_CRITTER_INVENTORY_SOURCE, 0);
    sub_4604E0();

    return true;
}
