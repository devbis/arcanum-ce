#ifndef ARCANUM_GAME_LIB_SECTOR_H_
#define ARCANUM_GAME_LIB_SECTOR_H_

#include "game/context.h"
#include "game/sector_block_list.h"
#include "game/sector_light_list.h"
#include "game/sector_object_list.h"
#include "game/sector_roof_list.h"
#include "game/sector_script_list.h"
#include "game/sector_sound_list.h"
#include "game/sector_tile_list.h"
#include "game/tile_script_list.h"
#include "game/timeevent.h"

typedef struct Sector {
    /* 0000 */ int flags;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t id;
    /* 0010 */ DateTime datetime;
    /* 0018 */ SectorLightList lights;
    /* 0020 */ SectorTileList tiles;
    /* 4224 */ SectorRoofList roofs;
    /* 4628 */ TileScriptList tile_scripts;
    /* 4630 */ SectorScriptList sector_scripts;
    /* 4640 */ int townmap_info;
    /* 4644 */ int aptitude_adj;
    /* 4648 */ int light_scheme;
    /* 464C */ SectorSoundList sounds;
    /* 4658 */ SectorBlockList blocks;
    /* 485C */ SectorObjectList objects;
} Sector;

static_assert(sizeof(Sector) == 0x8868, "wrong size");

typedef struct Sector601808 {
    /* 0000 */ int64_t id;
    /* 0008 */ int64_t field_8;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ struct Sector601808* next;
    /* 001C */ int field_1C;
} Sector601808;

typedef bool(SectorEnumerateFunc)(Sector* sector);
typedef bool(SectorLockFunc)(const char* path);

// See 0x4D13D0.
static_assert(sizeof(Sector601808) == 0x20, "wrong size");

bool sector_init(GameInitInfo* init_info);
void sector_reset();
void sector_exit();
void sector_resize(ResizeInfo* resize_info);
void sub_4CF320();
bool sector_update_view(ViewOptions* view_options);
void sub_4CF360(SectorLockFunc* func);
void sub_4CF370();
void sub_4CF390(UnknownContext* info);
bool sub_4CF790(int64_t a1, int64_t a2);
void sub_4CF7E0(int64_t* a1, int64_t* a2);
Sector601808* sub_4D02E0(int64_t* rect);
void sub_4D0400(Sector601808* node);
bool sub_4D0440(const char* a1, const char* a2);
bool sub_4D04A0(uint64_t a1);
bool sector_lock(int64_t id, Sector** sector_ptr);
bool sector_unlock(int64_t id);
void sub_4D0B40();
void sector_flush(unsigned int flags);
void sub_4D0E70(tig_art_id_t art_id);
void sub_4D0F40();
bool sub_4D0F50(const char* a1, const char* a2);
void sub_4D1040();
void sub_4D1050();
void sector_enumerate(SectorEnumerateFunc* func);
Sector601808* sub_4D13A0();
int sub_4D2FC0(int64_t a1);

#define SECTOR_X(a) ((a) & 0x3FFFFFF)
#define SECTOR_Y(a) (((a) >> 32) & 0x3FFFFFF)
#define SECTOR_FROM_XY(a, b) ((a) | ((b) << 32))

#endif /* ARCANUM_GAME_LIB_SECTOR_H_ */
