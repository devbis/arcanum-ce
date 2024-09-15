#include "game/sector_sound_list.h"

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
    list->field_0 = 0;
    list->field_4 = 0;
    list->field_8 = 0;
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

    list->field_0 &= ~0x8000000;

    return true;
}

// 0x4F8180
bool sector_sound_list_save(SectorSoundList* list, TigFile* stream)
{
    list->field_0 &= ~0x8000000;

    if (tig_file_fwrite(list, sizeof(*list), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4F81B0
bool sub_4F81B0(SectorSoundList* list)
{
    return (list->field_0 & 0x8000000) != 0;
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
