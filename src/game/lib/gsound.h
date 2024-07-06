#ifndef ARCANUM_GAMELIB_GSOUND_H_
#define ARCANUM_GAMELIB_GSOUND_H_

#include "game/context.h"
#include "game/lib/object.h"

int sub_41A940(int sound_id, char* path);
bool gsound_init(GameContext* ctx);
void gsound_exit();
void gsound_reset();
bool gsound_mod_load();
void gsound_mod_unload();
bool gsound_load(LoadContext* ctx);
bool gsound_save(TigFile* stream);
void gsound_ping(int a1);
int gsound_play_sfx(const char* path, int loops, int volume, int extra_volume, int id);
int gsound_play_sfx_id_ex(int id, int loops, int volume, int extra_volume);
int gsound_play_sfx_id(int id, int loops);
int gsound_get_positional_size(object_id_t object_id);

#endif /* ARCANUM_GAMELIB_GSOUND_H_ */
