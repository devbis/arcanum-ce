#include "tig/movie.h"

#define DIRECTDRAW_VERSION 0x300
#include <ddraw.h>

static bool tig_movie_do_frame();

// 0x62B2B4
static LPDIRECTDRAWSURFACE tig_movie_surface;

// 0x62B2B8
static int tig_movie_flags;

// 0x62B2BC
static BINK* tig_movie_bink;

// 0x5314F0
int tig_movie_init(TigContext* ctx)
{
    HDIGDRIVER dig;
    AIL_quick_handles(&dig, NULL, NULL);
    BinkSetSoundSystem(BinkOpenMiles, dig);

    return TIG_OK;
}

// 0x531520
void tig_movie_exit()
{
}

// 0x531530
int tig_movie_play(const char* path, unsigned int flags, int sound_track)
{
    if (path == NULL) {
        return 12;
    }

    // TODO
}

// 0x5317C0
BINK* tig_movie_get_bink()
{
    return tig_movie_bink;
}

// 0x5317D0
bool tig_movie_do_frame()
{
    if (!BinkWait(tig_movie_bink)) {
        BinkDoFrame(tig_movie_bink);

        DDSURFACEDESC surface_desc;
        surface_desc.dwSize = sizeof(surface_desc);

        // TODO: Check.
        do {
            if (IDirectDrawSurface_Lock(tig_movie_surface, NULL, &surface_desc, DDLOCK_WAIT, NULL) == DDERR_SURFACELOST) {
                if (IDirectDrawSurface_Restore(tig_movie_surface) != DD_OK) {
                    continue;
                }
            }

            BinkCopyToBuffer(tig_movie_bink,
                surface_desc.lpSurface,
                surface_desc.lPitch,
                tig_movie_bink->Height,
                (surface_desc.dwWidth - tig_movie_bink->Width) / 2,
                (surface_desc.dwHeight - tig_movie_bink->Height) / 2,
                tig_movie_flags);
        } while (0);

        if (tig_movie_bink->FrameNum == tig_movie_bink->Frames) {
            return false;
        }

        BinkNextFrame(tig_movie_bink);
    }

    return true;
}
