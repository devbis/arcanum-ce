#include "game/lib/gmovie.h"

#include <stdio.h>

#include "game/lib/message.h"
#include "tig/movie.h"

// 0x59F02C
static int movies_msg_file = -1;

// 0x40DE20
bool gmovie_mod_load()
{
    if (!message_load("movies\\movies.mes", &movies_msg_file)) {
        movies_msg_file = -1;
    }

    return true;
}

// 0x40DE50
void gmovie_mod_unload()
{
    if (movies_msg_file != -1) {
        message_unload(movies_msg_file);
        movies_msg_file = -1;
    }
}

// 0x40DE70
void gmovie_play(int movie, unsigned int flags, int sound_track)
{
    if (movies_msg_file != -1) {
        MessageListItem msg;
        msg.num = movie;
        if (message_find(movies_msg_file, &msg)) {
            char path[MAX_PATH];
            sprintf(path, "movies\\%s", msg.text);
            gmovie_play_path(path, flags, sound_track);
        }
    }
}

// 0x40DEE0
void gmovie_play_path(const char* path, unsigned int flags, int sound_track)
{
    char temp_path[MAX_PATH];
    if (tig_file_extract(path, temp_path)) {
        sub_41C610();

        unsigned int movie_flags = 0;

        if ((flags & GAME_MOVIE_FADE_IN) != 0) {
            movie_flags |= MOVIE_FADE_IN;
        }

        if ((flags & GAME_MOVIE_FADE_OUT) != 0) {
            movie_flags |= MOVIE_FADE_OUT;
        }

        if ((flags & GAME_MOVIE_FLAG_0x04) != 0) {
            movie_flags |= MOVIE_FLAG_0x01;
        }

        if ((flags & GAME_MOVIE_FLAG_0x08) != 0) {
            movie_flags |= MOVIE_FLAG_0x08;
        }

        tig_movie_play(temp_path, movie_flags, sound_track);

        sub_41C660();
    }
}
