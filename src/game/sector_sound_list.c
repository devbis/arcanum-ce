#include "game/sector_sound_list.h"

#define SECTOR_SOUND_LIST_MODIFIED 0x8000000

static void sub_4F8120(SectorSoundList* list);

// 0x4F80E0
bool sector_sound_list_init(SectorSoundList* list)
{
    sub_4F8120(list);

    return true;
}

// 0x4F8100
bool sector_sound_list_reset(SectorSoundList* list)
{
    sub_4F8120(list);

    return true;
}

// 0x4F8120
void sub_4F8120(SectorSoundList* list)
{
    list->flags = 0;
    list->music_scheme_idx = 0;
    list->ambient_scheme_idx = 0;
}

// 0x4F8130
bool sector_sound_list_exit(SectorSoundList* list)
{
    sub_4F8120(list);

    return true;
}

// 0x4F8150
bool sector_sound_list_load(SectorSoundList* list, TigFile* stream)
{
    if (tig_file_fread(list, sizeof(*list), 1, stream) != 1) {
        return false;
    }

    list->flags &= ~SECTOR_SOUND_LIST_MODIFIED;

    return true;
}

// 0x4F8180
bool sector_sound_list_save(SectorSoundList* list, TigFile* stream)
{
    list->flags &= ~SECTOR_SOUND_LIST_MODIFIED;

    if (tig_file_fwrite(list, sizeof(*list), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4F81B0
bool sector_sound_list_is_modified(SectorSoundList* list)
{
    return (list->flags & SECTOR_SOUND_LIST_MODIFIED) != 0;
}

// 0x4F81C0
bool sector_sound_list_load_with_dif(SectorSoundList* list, TigFile* stream)
{
    (void)list;
    (void)stream;

    return true;
}

// 0x4F81D0
bool sector_sound_list_save_with_dif(SectorSoundList* list, TigFile* stream)
{
    (void)list;
    (void)stream;

    return true;
}
