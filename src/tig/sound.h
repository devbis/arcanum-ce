#ifndef ARCANUM_TIG_SOUND_H_
#define ARCANUM_TIG_SOUND_H_

#include "tig/types.h"

typedef enum TigSoundType {
    TIG_SOUND_TYPE_EFFECT,
    TIG_SOUND_TYPE_MUSIC,
    TIG_SOUND_TYPE_VOICE,
    TIG_SOUND_TYPE_COUNT,
};

typedef enum TigSoundFlags {
    TIG_SOUND_STREAMED = 0x01,
    TIG_SOUND_MEMORY = 0x02,
    TIG_SOUND_FADE_OUT = 0x04,
    TIG_SOUND_WAIT = 0x08,
    TIG_SOUND_FADE_IN = 0x10,
    TIG_SOUND_STOP = 0x20,
};

typedef void(TigSoundFileNameFunc)(int a1, char* path);
typedef void(TigSoundEnumerateFunc)(int sound_handle);

int tig_sound_init(TigContext* ctx);
void tig_sound_exit();
void tig_sound_ping();
void tig_sound_set_path_func(TigSoundFileNameFunc* func);
void tig_sound_fade_out(int sound_handle, int fade_duration);
void tig_sound_fade_out_from_stop(int sound_handle, int fade_duration);
void tig_sound_fade_out_all(int fade_duration);
int tig_sound_allocate(int* sound_handle, int type);
void tig_sound_stop(int sound_handle);
int tig_sound_play_by_path(int sound_handle, const char* path, int id);
int tig_sound_play_by_id(int sound_handle, int id);
int tig_sound_play_streamed(int sound_handle, const char* name, int loops, int fade_duration, int prev_sound_handle);
int tig_sound_play_streamed_indefinitely(int sound_handle, const char* name, int fade_duration, int prev_sound_handle);
int tig_sound_play_streamed_once(int sound_handle, const char* name, int fade_duration, int prev_sound_handle);
void tig_sound_set_loops(int sound_handle, int loops);
void tig_sound_set_volume(int sound_handle, int volume);
void tig_sound_set_volume_by_type(int type, int volume);
int tig_sound_get_type(int sound_handle);
int sub_5338D0(int sound_handle);
void sub_533910(int sound_handle, int value);
void tig_sound_set_extra_volume(int sound_handle, int extra_volume);
bool tig_sound_is_active_id(int id);
bool tig_sound_is_playing(int sound_handle);
bool tig_sound_is_active(int sound_handle);
void sub_533B10(int sound_handle, int64_t a2, int64_t a3);
void sub_533B60(int sound_handle, int64_t* a2, int64_t* a3);
void sub_533BF0(TigSoundEnumerateFunc* func);
void tig_sound_set_effects_volume(int volume);
void tig_sound_quick_play(int id);
void tig_sound_set_active(bool is_active);

#endif /* ARCANUM_TIG_SOUND_H_ */
