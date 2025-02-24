#ifndef ARCANUM_GAME_GSOUND_H_
#define ARCANUM_GAME_GSOUND_H_

#include "game/context.h"

int gsound_resolve_path(int sound_id, char* path);
bool gsound_init(GameInitInfo* init_info);
void gsound_exit();
void gsound_reset();
bool gsound_mod_load();
void gsound_mod_unload();
bool gsound_load(GameLoadInfo* load_info);
bool gsound_save(TigFile* stream);
void gsound_ping(tig_timestamp_t timestamp);
void gsound_flush();
tig_sound_handle_t gsound_play_sfx(const char* path, int loops, int volume, int extra_volume, int id);
tig_sound_handle_t gsound_play_sfx_id_ex(int id, int loops, int volume, int extra_volume);
tig_sound_handle_t gsound_play_sfx_id(int id, int loops);
tig_sound_handle_t sub_41B7A0(int id, int loops, int64_t x, int64_t y);
tig_sound_handle_t sub_41B7D0(int id, int loops, int64_t x, int64_t y, int size);
tig_sound_handle_t sub_41B850(int id, int loops, int64_t location);
tig_sound_handle_t sub_41B870(int id, int loops, int64_t location, int size);
tig_sound_handle_t sub_41B930(int id, int loops, int64_t obj);
TigSoundPositionalSize gsound_size(int64_t obj);
int gsound_range(int64_t obj);
void gsound_play_scheme(int gsound_play_scheme, int ambient_scheme_idx);
void gsound_stop_all(int fade_duration);
tig_sound_handle_t gsound_play_voice(const char* path, int fade_duration);
tig_sound_handle_t gsound_play_music_once(const char* path, int fade_duration);
tig_sound_handle_t gsound_play_music_indefinitely(const char* path, int fade_duration);
void gsound_start_combat_music(int64_t obj);
void gsound_stop_combat_music(int64_t obj);
void gsound_lock();
void gsound_unlock();
void gsound_listener_set(int64_t location);
void gsound_move(tig_sound_handle_t sound_handle, int64_t location);

#endif /* ARCANUM_GAME_GSOUND_H_ */
