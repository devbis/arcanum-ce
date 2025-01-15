#ifndef ARCANUM_GAME_RUMOR_H_
#define ARCANUM_GAME_RUMOR_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/timeevent.h"

// FIXME: Waste memory due to alignment (current size is 24 bytes, but could
// be reduced to 16 just by rearranging `timestamp`).
typedef struct RumorInfo {
    /* 0000 */ int num;
    /* 0004 */ DateTime datetime;
    /* 0010 */ int known;
} RumorInfo;

static_assert(sizeof(RumorInfo) == 0x18, "wrong size");

bool rumor_init(GameInitInfo* init_info);
void rumor_reset();
void rumor_exit();
bool rumor_mod_load();
void rumor_mod_unload();
bool rumor_load(GameLoadInfo* load_info);
bool rumor_save(TigFile* stream);
bool rumor_is_known(int rumor);
void rumor_set_known(int rumor);
void sub_4C5700(object_id_t pc_object_id, object_id_t npc_object_id, int rumor, char* buffer);
void sub_4C57E0(int64_t obj, int rumor);
void sub_4C58A0(object_id_t obj, int rumor, int64_t timestamp);
bool sub_4C58D0(object_id_t object_id, int rumor);
void sub_4C5920(object_id_t object_id, int rumor, char* buffer);
void sub_4C5960(int rumor, char* buffer);
void sub_4C59D0(int rumor, char* buffer);
int rumor_copy_state(int64_t obj, RumorInfo* rumors);
bool sub_4C5B10(int64_t a1, int64_t a2);

#endif /* ARCANUM_GAME_RUMOR_H_ */
