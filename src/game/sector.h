#ifndef ARCANUM_GAME_SECTOR_H_
#define ARCANUM_GAME_SECTOR_H_

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

typedef bool(SectorEnumerateFunc)(Sector* sector);
typedef bool(SectorLockFunc)(const char* path);

bool sector_init(GameInitInfo* init_info);
void sector_reset();
void sector_exit();
void sector_resize(GameResizeInfo* resize_info);
void sub_4CF320();
void sector_update_view(ViewOptions* view_options);
void sub_4CF360(SectorLockFunc* func);
void sub_4CF370();
void sector_draw(GameDrawInfo* draw_info);
bool sector_limits_set(int64_t x, int64_t y);
void sector_limits_get(int64_t* x, int64_t* y);
int64_t sector_id_from_loc(int64_t loc);
int64_t sector_loc_from_id(int64_t sector_id);
int sector_rot(int64_t a, int64_t b);
bool sector_in_dir(int64_t sec, int dir, int64_t* new_sec_ptr);
bool sub_4D0090(LocRect* rect, SomeSectorStuff* a2);
Sector601808* sub_4D02E0(LocRect* loc_rect);
void sub_4D0400(Sector601808* node);
bool sector_map_name_set(const char* base_map_name, const char* current_map_name);
bool sector_exists(uint64_t id);
bool sub_4D04E0(int64_t id);
bool sector_lock(int64_t id, Sector** sector_ptr);
bool sector_unlock(int64_t id);
void sub_4D0B40();
void sector_flush(unsigned int flags);
bool sub_4D0DE0(int64_t id);
void sub_4D0E70(tig_art_id_t art_id);
bool sector_is_blocked(int64_t sec);
void sector_block_set(int64_t sec, bool blocked);
void sector_block_init();
bool sector_block_load(const char* base_map_name, const char* current_map_name);
void sub_4D1040();
void sub_4D1050();
void sector_enumerate(SectorEnumerateFunc* func);
bool sub_4D10C0(GameInitInfo* init_info);
void sub_4D10D0();
void sub_4D10E0();
bool sub_4D1150(TigFile* stream);
bool sub_4D12B0(GameLoadInfo* load_info);
Sector601808* sub_4D13A0();

#define SECTOR_X(a) ((a) & 0x3FFFFFF)
#define SECTOR_Y(a) (((a) >> 26) & 0x3FFFFFF)
#define SECTOR_MAKE(a, b) ((a) | ((b) << 26))

#endif /* ARCANUM_GAME_SECTOR_H_ */
