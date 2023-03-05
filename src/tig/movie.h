#ifndef ARCANUM_TIG_MOVIE_H_
#define ARCANUM_TIG_MOVIE_H_

#include "tig/types.h"

int tig_movie_init(TigContext* ctx);
void tig_movie_exit();
int tig_movie_play(const char* path, unsigned int flags, int sound_track);
BINK* tig_movie_get_bink();

#endif /* ARCANUM_TIG_MOVIE_H_ */
