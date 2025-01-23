#include "game/gmovie.h"

#include <stdio.h>

#include "game/gsound.h"
#include "game/mes.h"

// 0x59F02C
static mes_file_handle_t movies_mes_file = MES_FILE_HANDLE_INVALID;

// 0x40DE20
bool gmovie_mod_load()
{
    if (!mes_load("movies\\movies.mes", &movies_mes_file)) {
        movies_mes_file = MES_FILE_HANDLE_INVALID;
    }

    return true;
}

// 0x40DE50
void gmovie_mod_unload()
{
    if (movies_mes_file != MES_FILE_HANDLE_INVALID) {
        mes_unload(movies_mes_file);
        movies_mes_file = MES_FILE_HANDLE_INVALID;
    }
}

// 0x40DE70
void gmovie_play(int movie, unsigned int flags, int sound_track)
{
    MesFileEntry mes_file_entry;
    char path[TIG_MAX_PATH];

    if (movies_mes_file != MES_FILE_HANDLE_INVALID) {
        mes_file_entry.num = movie;
        if (mes_search(movies_mes_file, &mes_file_entry)) {
            sprintf(path, "movies\\%s", mes_file_entry.str);
            gmovie_play_path(path, flags, sound_track);
        }
    }
}

// 0x40DEE0
void gmovie_play_path(const char* path, unsigned int flags, int sound_track)
{
    char temp_path[TIG_MAX_PATH];
    unsigned int movie_flags;

    if (tig_file_extract(path, temp_path)) {
        sub_41C610();

        movie_flags = 0;

        if ((flags & GAME_MOVIE_FADE_IN) != 0) {
            movie_flags |= TIG_MOVIE_FADE_IN;
        }

        if ((flags & GAME_MOVIE_FADE_OUT) != 0) {
            movie_flags |= TIG_MOVIE_FADE_OUT;
        }

        if ((flags & GAME_MOVIE_IGNORE_KEYBOARD) != 0) {
            movie_flags |= TIG_MOVIE_IGNORE_KEYBOARD;
        }

        if ((flags & GAME_MOVIE_BLACK_OUT) != 0) {
            movie_flags |= TIG_MOVIE_BLACK_OUT;
        }

        tig_movie_play(temp_path, movie_flags, sound_track);

        sub_41C660();
    }
}
