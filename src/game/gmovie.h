#ifndef ARCANUM_GAMELIB_GMOVIE_H_
#define ARCANUM_GAMELIB_GMOVIE_H_

#include "game/context.h"

typedef enum GameMovieFlags {
    GAME_MOVIE_FADE_IN = 0x1,
    GAME_MOVIE_FADE_OUT = 0x2,
    GAME_MOVIE_FLAG_0x04 = 0x4,
    GAME_MOVIE_FLAG_0x08 = 0x8,
};

bool gmovie_mod_load();
void gmovie_mod_unload();
void gmovie_play(int movie, unsigned int flags, int sound_track);
void gmovie_play_path(const char* path, unsigned int flags, int sound_track);

#endif /* ARCANUM_GAMELIB_GMOVIE_H_ */
