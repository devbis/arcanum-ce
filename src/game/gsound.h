#ifndef ARCANUM_GAME_GSOUND_H_
#define ARCANUM_GAME_GSOUND_H_

#include "game/context.h"
#include "game/obj.h"

int gsound_resolve_path(int sound_id, char* path);
bool gsound_init(GameInitInfo* init_info);
void gsound_exit();
void gsound_reset();
bool gsound_mod_load();
void gsound_mod_unload();
bool gsound_load(GameLoadInfo* load_info);
bool gsound_save(TigFile* stream);
void gsound_ping(int a1);
void gsound_flush();
tig_sound_handle_t gsound_play_sfx(const char* path, int loops, int volume, int extra_volume, int id);
tig_sound_handle_t gsound_play_sfx_id_ex(int id, int loops, int volume, int extra_volume);
tig_sound_handle_t gsound_play_sfx_id(int id, int loops);
tig_sound_handle_t sub_41B7A0(int id, int loops, int64_t x, int64_t y);
tig_sound_handle_t sub_41B7D0(int id, int loops, int64_t x, int64_t y, int size);
tig_sound_handle_t sub_41B850(int id, int loops, int64_t location);
tig_sound_handle_t sub_41B870(int id, int loops, int64_t location, int size);
tig_sound_handle_t sub_41B930(int id, int loops, int64_t obj);
int gsound_get_positional_size(object_id_t object_id);
void sub_41C610();
void sub_41C660();
void sub_41C6D0(int64_t location);

#endif /* ARCANUM_GAME_GSOUND_H_ */
