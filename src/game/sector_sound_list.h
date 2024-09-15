#ifndef ARCANUM_GAME_SOUND_LIST_H_
#define ARCANUM_GAME_SOUND_LIST_H_

#include <tig/tig.h>

typedef struct SectorSoundList {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
} SectorSoundList;

// See 0x4F815F.
static_assert(sizeof(SectorSoundList) == 0xC, "wrong size");

bool sector_sound_list_init(SectorSoundList* list);
bool sector_sound_list_reset(SectorSoundList* list);
void sub_4F8120(SectorSoundList* list);
bool sector_sound_list_exit(SectorSoundList* list);
bool sector_sound_list_load(SectorSoundList* list, TigFile* stream);
bool sector_sound_list_save(SectorSoundList* list, TigFile* stream);
bool sub_4F81B0(SectorSoundList* list);
bool sector_sound_list_load_with_dif(SectorSoundList* list, TigFile* stream);
bool sector_sound_list_save_with_dif(SectorSoundList* list, TigFile* stream);

#endif /* ARCANUM_GAME_SOUND_LIST_H_ */
