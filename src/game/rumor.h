#ifndef ARCANUM_GAME_RUMOR_H_
#define ARCANUM_GAME_RUMOR_H_

#include "game/context.h"
#include "game/obj.h"

// FIXME: Waste memory due to alignment (current size is 24 bytes, but could
// be reduced to 16 just by rearranging `timestamp`).
typedef struct RumorInfo {
    /* 0000 */ int num;
    /* 0004 */ int64_t timestamp;
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
int sub_4C5A40(int64_t obj, RumorInfo* rumors);
bool sub_4C5B10(int64_t a1, int64_t a2);

#endif /* ARCANUM_GAME_RUMOR_H_ */
