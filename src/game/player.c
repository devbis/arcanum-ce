#include "game/lib/player.h"

#include "game/lib/level.h"
#include "tig/art.h"
#include "tig/debug.h"

// 0x5D1138
static ObjectID stru_5D1138;

// 0x5D1150
static object_id_t qword_5D1150;

// #original-name
// 0x5D1158
static object_id_t pcObj;

// 0x739E5C
bool dword_739E5C;

// 0x40D6C0
bool player_init(GameContext* ctx)
{
    stru_5D1138.field_0 = 0;
    dword_739E5C = ctx->editor;
    return true;
}

// 0x40D6E0
void player_reset()
{
    if (pcObj != OBJ_HANDLE_NULL) {
        pcObj = OBJ_HANDLE_NULL;
        stru_5D1138.field_0 = 0;
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
    if (stream == NULL) {
        return false;
    }

    if (pcObj == OBJ_HANDLE_NULL) {
        return false;
    }

    ObjectID temp_object_id = sub_407EF0(pcObj);
    if (tig_file_fwrite(&temp_object_id, sizeof(temp_object_id), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x40D7C0
bool player_load(LoadContext* ctx)
{
    if (ctx->stream == NULL) {
        return false;
    }

    ObjectID temp_object_id;
    if (tig_file_fread(&temp_object_id, sizeof(temp_object_id), 1, ctx->stream) != 1) {
        return false;
    }

    stru_5D1138 = temp_object_id;
    pcObj = objp_perm_lookup(temp_object_id);
    if (pcObj == OBJ_HANDLE_NULL) {
        return false;
    }

    long long location = obj_field_int64_get(pcObj, OBJ_F_LOCATION);
    scroll_set_center(location);
    sub_4B8CE0(location);
    sub_4604E0();

    return true;
}

// 0x40D860
void sub_40D860()
{
    if (stru_5D1138.field_0 != 0) {
        pcObj = objp_perm_lookup(stru_5D1138);
    }
}

// 0x40D8A0
void player_create()
{
    qword_5D1150 = sub_468570(15);

    // TODO: Incomplete.

    tig_debug_printf("player_create: loc: X: %d, Y: %d\n", x, y);
    if (!object_create(qword_5D1150), x, y, pcObj)) {
        tig_debug_printf("player_create: Error: failed to create player!\n");
        exit(EXIT_FAILURE);
    }

    art_id_t art_id;
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
object_id_t sub_40DA50()
{
    object_id_t obj = pcObj;

    if (obj != OBJ_HANDLE_NULL) {
        if (sub_4E5470(pcObj) || stru_5D1138.field_0 == 0) {
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
    stru_5D1138.field_0 = 0;

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
        stru_5D1138.field_0 = 0;
    }

    return true;
}

// 0x40DB50
void sub_40DB50(PlayerSpec* player_spec)
{
    player_spec->field_0 = OBJ_HANDLE_NULL;
    player_spec->field_20 = 0;
    player_spec->field_24 = 0;
    player_spec->field_28 = 0;
    player_spec->field_2C = -1;
}

// 0x40DB70
bool player_obj_create_player(PlayerSpec* player_spec)
{
    object_id_t current_handle = OBJ_HANDLE_NULL;

    if (player_spec->field_2C == -1) {
        qword_5D1160 = sub_468570(15);
    } else {
        qword_5D1160 = objp_perm_lookup(sub_407EF0(sub_4685A0(player_spec->field_2C)));
    }

    if ((player_spec->field_28 & PLAYER_SPEC_FLAG_0x2) != 0) {

    }

    // TODO: Incomplete.

    if ((player_spec->field_28 & PLAYER_SPEC_FLAG_0x1) != 0) {
        if (current_handle != player_spec->field_0) {
            // FIXME: Using println with newline character.
            tig_debug_println("ERROR: : object_create created new handle instead of using current one!\n");
        }
    }

    tig_debug_printf("player_obj_create_player: StartLoc: X: %d, Y: %d\n", x, y);
    obj_field_int64_set(player_spec->field_0, OBJ_F_CRITTER_TELEPORT_DEST, x, y);
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
