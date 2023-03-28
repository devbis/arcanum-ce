#ifndef ARCANUM_GAMELIB_PLAYER_H_
#define ARCANUM_GAMELIB_PLAYER_H_

#include "game/context.h"
#include "game/lib/object.h"
#include "tig/file.h"

#define PLAYER_SPEC_FLAG_0x1 0x1
#define PLAYER_SPEC_FLAG_0x2 0x2
#define PLAYER_SPEC_FLAG_0x4 0x4

typedef struct PlayerSpec {
    object_id_t field_0;
    ObjectID field_8;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
};

static_assert(sizeof(PlayerSpec) == 0x30, "wrong size");

bool player_init(GameContext* ctx);
void player_reset();
void player_exit();
bool player_save(TigFile* stream);
bool player_load(LoadContext* ctx);
void sub_40D860();
void player_create();
bool player_is_pc_obj(object_id_t object_id);
bool sub_40DA20(object_id_t object_id);
object_id_t sub_40DA50();
bool sub_40DAB0();
bool sub_40DAF0(object_id_t obj);
void sub_40DB50(PlayerSpec* player_spec);
bool player_obj_create_player(PlayerSpec* player_spec)

#endif /* ARCANUM_GAMELIB_PLAYER_H_ */
