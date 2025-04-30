#include "game/townmap.h"

#include <stdio.h>

#include "game/mes.h"
#include "game/sector.h"

typedef struct TownMapIndexEntry {
    int map;
    bool w;
} TownMapIndexEntry;

static void townmap_kstate_reset();
static void townmap_set_known_internal(int map, bool known);
static void townmap_tile_known_set(int map, int index);
static bool townmap_tile_known_get(int map, int index);
static bool townmap_kstate_load(int map);
static void townmap_kstate_init(int map);
static bool townmap_kstate_flush();

// 0x5B6450
static mes_file_handle_t townmap_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5FC4C8
static TigRect townmap_tile_bounds;

// 0x5FC4D8
static TownMapInfo townmap_info_cache;

// 0x5FC508
static TownMapIndexEntry* townmap_entries;

// 0x5FC50C
static bool townmap_kstate_modified;

// 0x5FC510
static uint8_t* townmap_kstate;

// 0x5FC514
static int townmap_kstate_size;

// 0x5FC518
static int townmap_kstate_map;

// 0x5FC51C
static int townmap_entries_cnt;

// 0x4BE1B0
void townmap_reset()
{
    townmap_kstate_reset();
}

// 0x4BE1C0
bool townmap_mod_load()
{
    MesFileEntry mes_file_entry;
    int index;
    char* pch;
    char* back;

    if (!mes_load("rules\\townmap.mes", &townmap_mes_file)) {
        townmap_mes_file = MES_FILE_HANDLE_INVALID;
        return true;
    }

    townmap_entries_cnt = mes_num_entries(townmap_mes_file);
    townmap_entries = (TownMapIndexEntry*)CALLOC(townmap_entries_cnt, sizeof(*townmap_entries));

    if (mes_find_first(townmap_mes_file, &mes_file_entry)) {
        index = 0;
        do {
            townmap_entries[index].map = mes_file_entry.num;

            pch = strstr(mes_file_entry.str, "[w:");
            if (pch == NULL) {
                pch = strstr(mes_file_entry.str, "[W:");
            }

            if (pch != NULL) {
                back = pch;
                do {
                    *back-- = '\0';
                } while (isspace(*back));

                pch += 3;
                while (isspace(*pch)) {
                    pch++;
                }

                if (*pch == '1') {
                    townmap_entries[index].w = true;
                }
            }
            index++;
        } while (mes_find_next(townmap_mes_file, &mes_file_entry));
    }

    return true;
}

// 0x4BE310
void townmap_mod_unload()
{
    townmap_kstate_reset();

    if (townmap_entries != NULL) {
        FREE(townmap_entries);
        townmap_entries = NULL;
    }

    townmap_entries_cnt = 0;

    if (townmap_mes_file != MES_FILE_HANDLE_INVALID) {
        mes_unload(townmap_mes_file);
        townmap_mes_file = MES_FILE_HANDLE_INVALID;
    }

    memset(&townmap_info_cache, 0, sizeof(townmap_info_cache));
}

// 0x4BE370
void townmap_flush()
{
    townmap_kstate_flush();
    townmap_kstate_reset();
}

// 0x4BE380
int townmap_get(int64_t sec)
{
    Sector* sector;
    int num;

    if (!sector_lock(sec, &sector)) {
        return 0;
    }

    num = sector->townmap_info;
    sector_unlock(sec);

    return num;
}

// 0x4BE3C0
void townmap_set(int64_t sec, int townmap)
{
    Sector* sector;

    if (!sector_lock(sec, &sector)) {
        return;
    }

    sector->townmap_info = townmap;
    sector->flags |= 0x1;

    sector_unlock(sec);
}

// 0x4BE400
int townmap_count()
{
    if (townmap_mes_file == MES_FILE_HANDLE_INVALID) {
        return 0;
    }

    return mes_num_entries(townmap_mes_file);
}

// 0x4BE420
const char* townmap_name(int map)
{
    MesFileEntry mes_file_entry;

    if (townmap_mes_file == MES_FILE_HANDLE_INVALID) {
        return NULL;
    }

    mes_file_entry.num = map;
    if (!mes_search(townmap_mes_file, &mes_file_entry)) {
        return NULL;
    }

    return mes_file_entry.str;
}

// 0x4BE4E0
bool townmap_info(int map, TownMapInfo* tmi)
{
    const char* name;
    char path[TIG_MAX_PATH];
    TigFile* stream;
    int version;

    if (map == 0) {
        return false;
    }

    if (map == townmap_info_cache.map) {
        *tmi = townmap_info_cache;
        return true;
    }

    name = townmap_name(map);
    if (name == NULL) {
        return false;
    }

    sprintf(path, "townmap\\%s\\%s.tmi", name, name);

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        return false;
    }

    if (tig_file_fread(&version, sizeof(version), 1, stream) != 1 || version != 2) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fseek(stream, -(int)(sizeof(version)), SEEK_CUR) != 0
        || tig_file_fread(tmi, sizeof(*tmi), 1, stream) != 1) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);

    townmap_tile_bounds.x = 0;
    townmap_tile_bounds.y = 0;
    townmap_tile_bounds.width = (int)(tmi->width / tmi->num_hor_tiles * tmi->scale);
    townmap_tile_bounds.height = (int)(tmi->height / tmi->num_vert_tiles * tmi->scale);

    townmap_info_cache = *tmi;

    return true;
}

// 0x4BE670
void townmap_loc_to_coords(TownMapInfo* tmi, int64_t loc, int* x_ptr, int* y_ptr)
{
    int64_t center_x;
    int64_t center_y;
    int64_t x;
    int64_t y;

    *x_ptr = 0;
    *y_ptr = 0;

    location_xy(tmi->loc, &center_x, &center_y);
    location_xy(loc, &x, &y);

    x += tmi->width / 2 - center_x;
    y += tmi->height / 2 - center_y;

    if (x >= 0 && x < tmi->width
        && y >= 0 && y < tmi->height) {
        *x_ptr = (int)(x * tmi->scale);
        *y_ptr = (int)(y * tmi->scale);
    }
}

// 0x4BE780
void townmap_coords_to_loc(TownMapInfo* tmi, int x, int y, int64_t* loc_ptr)
{
    int64_t offset_x;
    int64_t offset_y;
    int64_t center_x;
    int64_t center_y;
    int64_t loc;

    *loc_ptr = 0;

    offset_x = (int)(x / tmi->scale) - tmi->width / 2;
    offset_y = (int)(y / tmi->scale) - tmi->height / 2;

    location_xy(tmi->loc, &center_x, &center_y);
    location_at(center_x + offset_x, center_y + offset_y, &loc);
    *loc_ptr = loc;
}

// 0x4BE8F0
bool townmap_mark_visited(int64_t loc)
{
    int64_t sector_id;
    Sector* sector;
    TownMapInfo tmi;
    int64_t x1;
    int64_t y1;
    int64_t x2;
    int64_t y2;
    int64_t x;
    int64_t y;

    sector_id = sector_id_from_loc(loc);

    if (!sector_exists(sector_id)) {
        return false;
    }

    if (!sector_lock(sector_id, &sector)) {
        return false;
    }

    if (sector->townmap_info == 0) {
        sector_unlock(sector_id);
        return false;
    }

    if (townmap_info(sector->townmap_info, &tmi)) {
        location_xy(tmi.loc, &x1, &y1);
        location_xy(loc, &x2, &y2);

        x = x2 + tmi.width / 2 - x1;
        y = y2 + tmi.height / 2 - y1;

        if (x >= 0 && x < tmi.width
            && y >= 0 && y < tmi.height) {
            townmap_tile_known_set(sector->townmap_info,
                (int)(tmi.num_hor_tiles * x / tmi.width + tmi.num_hor_tiles * (tmi.num_vert_tiles * y / tmi.height)));
        }
    }

    sector_unlock(sector_id);
    return true;
}

// 0x4BEAB0
bool townmap_set_known(int map, bool known)
{
    TownMapInfo tmi;

    if (map == 0) {
        return false;
    }

    if (!townmap_info(map, &tmi)) {
        return false;
    }

    townmap_set_known_internal(map, known);

    return true;
}

// 0x4BEAF0
bool townmap_tile_blit_info(int map, int index, TigVideoBufferBlitInfo* vb_blit_info)
{
    uint8_t available_flags[3][3] = {
        { 0x8, 0x8 | 0x4, 0x4 },
        { 0x8 | 0x1, 0, 0x4 | 0x2 },
        { 0x1, 0x2 | 0x1, 0x2 },
    };

    unsigned int flags;
    TownMapInfo tmi;
    tig_color_t color;
    int base_col;
    int base_row;
    int row_offset;
    int col_offset;
    int row;
    int col;

    vb_blit_info->flags = 0;

    if (map == 0) {
        return false;
    }

    if (!townmap_info(map, &tmi)) {
        return false;
    }

    if (townmap_tile_known_get(map, index)) {
        return true;
    }

    flags = 0;

    base_col = index % tmi.num_hor_tiles;
    base_row = index / tmi.num_hor_tiles;

    // TODO: Check.
    for (row_offset = 0; row_offset < 3; row_offset++) {
        row = base_row + row_offset - 1;
        for (col_offset = 0; col_offset < 3; col_offset++) {
            col = base_col + col_offset - 1;
            if (col >= 0 && col < tmi.num_hor_tiles
                && row >= 0 && row < tmi.num_vert_tiles) {
                if (townmap_tile_known_get(map, col + tmi.num_hor_tiles * row)) {
                    flags |= available_flags[row_offset][col_offset];
                }
            }
        }
    }

    if (flags == 0) {
        return false;
    }

    color = tig_color_make(255, 255, 255);

    vb_blit_info->flags = TIG_VIDEO_BUFFER_BLIT_BLEND_COLOR_LERP;
    vb_blit_info->field_20 = &townmap_tile_bounds;
    vb_blit_info->field_10 = (flags & 0x8) != 0 ? color : 0;
    vb_blit_info->field_14 = (flags & 0x4) != 0 ? color : 0;
    vb_blit_info->field_18 = (flags & 0x2) != 0 ? color : 0;
    vb_blit_info->field_1C = (flags & 0x1) != 0 ? color : 0;

    return true;
}

// 0x4BECC0
bool townmap_is_waitable(int map)
{
    int index = 0;

    for (index = 0; index < townmap_entries_cnt; index++) {
        if (townmap_entries[index].map == map) {
            return townmap_entries[index].w;
        }
    }

    return false;
}

// 0x4BED00
void townmap_kstate_reset()
{
    if (townmap_kstate != NULL) {
        FREE(townmap_kstate);
        townmap_kstate = NULL;
    }
    townmap_kstate_size = 0;
    townmap_kstate_modified = false;
    townmap_kstate_map = 0;
}

// 0x4BED30
void townmap_set_known_internal(int map, bool known)
{
    int idx;
    uint8_t value;

    if (townmap_kstate_load(map)) {
        value = known ? 0xFF : 0;
        for (idx = 0; idx < townmap_kstate_size; idx++) {
            townmap_kstate[idx] = value;
        }

        townmap_kstate_modified = true;
    }
}

// 0x4BED90
void townmap_tile_known_set(int map, int index)
{
    if (townmap_kstate_load(map)) {
        townmap_kstate[index / 8] |= 1 << (index % 8);
        townmap_kstate_modified = true;
    }
}

// 0x4BEDD0
bool townmap_tile_known_get(int map, int index)
{
    const char* name;
    char path[TIG_MAX_PATH];

    if (!townmap_kstate_load(map)) {
        return false;
    }

    if ((townmap_kstate[index / 8] & (1 << (index % 8))) == 0) {
        return false;
    }

    name = townmap_name(map);
    sprintf(path, "townmap\\%s\\%s%06d.bmp", name, name, index);
    if (!tig_file_exists(path, NULL)) {
        return false;
    }

    return true;
}

// 0x4BEE60
bool townmap_kstate_load(int map)
{
    char path[TIG_MAX_PATH];
    TigFile* stream;

    if (townmap_kstate_map == map) {
        return true;
    }

    townmap_kstate_flush();
    townmap_kstate_init(map);

    sprintf(path, "%s\\%s.tmf", "Save\\Current", townmap_name(townmap_kstate_map));
    if (!tig_file_exists(path, NULL)) {
        townmap_set_known_internal(map, false);
        return true;
    }

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        return false;
    }

    if (tig_file_fread(townmap_kstate, 1u, townmap_kstate_size, stream) != townmap_kstate_size) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}

// 0x4BEF40
void townmap_kstate_init(int map)
{
    TownMapInfo tmi;

    townmap_kstate_reset();

    if (townmap_info(map, &tmi)) {
        townmap_kstate_size = tmi.num_hor_tiles * tmi.num_vert_tiles / 8;
        if ((tmi.num_hor_tiles * tmi.num_vert_tiles) % 8 != 0) {
            townmap_kstate_size++;
        }

        townmap_kstate = (uint8_t*)MALLOC(townmap_kstate_size);
        townmap_kstate_map = map;
    }
}

// 0x4BEFB0
bool townmap_kstate_flush()
{
    char path[TIG_MAX_PATH];
    TigFile* stream;

    if (townmap_kstate_modified) {
        sprintf(path, "%s\\%s.tmf", "Save\\Current", townmap_name(townmap_kstate_map));
        stream = tig_file_fopen(path, "wb");
        if (stream == NULL) {
            return false;
        }

        if (tig_file_fwrite(townmap_kstate, 1, townmap_kstate_size, stream) != townmap_kstate_size) {
            tig_file_fclose(stream);
            tig_file_remove(path);
            return false;
        }

        tig_file_fclose(stream);
    }

    return true;
}
