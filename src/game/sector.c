#include "game/sector.h"

#include <stdio.h>

#include "game/gamelib.h"
#include "game/li.h"
#include "game/map.h"
#include "game/obj_private.h"
#include "game/terrain.h"
#include "game/tile.h"
#include "game/timeevent.h"
#include "game/townmap.h"

typedef bool(SectorSaveFunc)(Sector* sector);
typedef bool(SectorLoadFunc)(int64_t a1, Sector* sector);

typedef struct SectorCacheEntry {
    /* 0000 */ bool used;
    /* 0004 */ int refcount;
    /* 0008 */ unsigned int timestamp;
    /* 000C */ int field_C;
    /* 0010 */ Sector sector;
} SectorCacheEntry;

static_assert(sizeof(SectorCacheEntry) == 0x8878, "wrong size");

typedef struct S60180C {
    int64_t id;
    DateTime datetime;
} S60180C;

static bool sub_4CF810(unsigned int size);
static void sub_4D0F70();
static void sub_4D1100();
static int sub_4D1310(int64_t a1, int64_t a2, int a3, int64_t* a4);
static void sub_4D13D0();
static void sub_4D1400(Sector* sector);
static bool sector_load_editor(int64_t id, Sector* sector);
static bool sector_load_game(int64_t id, Sector* sector);
static bool sector_save_editor(Sector* sector);
static bool sub_4D2460(Sector* sector, const char* base_path);
static bool sector_save_game(Sector* sector);
static void sub_4D2C00(const char* section);
static void sub_4D2C30(const char* section);
static bool sub_4D2CA0(int64_t id, int* index_ptr);
static bool sector_cache_find_unused(unsigned int* index_ptr);
static void sub_4D2D70();
static void sub_4D2EA0();
static void sub_4D2ED0(Sector* sector);
static void sub_4D2F80();
static void sub_4D2F90();
static int sub_4D2FB0(const tig_art_id_t* a, const tig_art_id_t* b);
static void sub_4D3000(int64_t a1);
static void sub_4D3050(int idx);
static bool sub_4D30A0();
static bool sub_4D31C0(const char* a1, const char* a2);

// 0x5B7CD0
static DateTime qword_5B7CD0 = { -1, -1 };

// 0x5B7CD8
static int dword_5B7CD8 = 1;

// 0x601780
static unsigned int sector_cache_size;

// 0x601784
static unsigned int dword_601784;

// 0x601788
static SectorSaveFunc* sector_save_func;

// 0x60178C
static bool in_sector_lock;

// 0x601790
static int dword_601790;

// 0x601794
static int64_t* dword_601794;

// 0x601798
static bool dword_601798;

// 0x60179C
static char* dword_60179C;

// 0x6017A0
static int64_t sector_limit_x;

// 0x6017A8
static bool sector_editor;

// 0x6017AC
static tig_color_t dword_6017AC;

// 0x6017B0
static SectorCacheEntry* sector_cache_entries;

// 0x6017B4
static SectorLoadFunc* sector_load_func;

// 0x6017B8
static IsoInvalidateRectFunc* sector_iso_window_invalidate_rect;

// 0x6017BC
static int dword_6017BC;

// 0x6017C0
static bool in_sector_enumerate;

// 0x6017C4
static int dword_6017C4;

// 0x6017C8
static int64_t qword_6017C8;

// 0x6017D0
static int64_t sector_limit_y;

// 0x6017D8
static int dword_6017D8;

// 0x6017E0
static int64_t qword_6017E0;

// 0x6017E8
static int64_t* dword_6017E8;

// 0x6017EC
static int64_t* dword_6017EC;

// 0x6017F0
static tig_art_id_t* dword_6017F0;

// 0x6017F4
static int* sector_cache_indexes;

// 0x6017F8
static int sector_iso_window_handle;

// 0x6017FC
static char* dword_6017FC;

// 0x601800
static ViewOptions sector_view_options;

// 0x601808
static Sector601808* dword_601808;

// 0x60180C
static S60180C* dword_60180C;

// 0x601810
static int64_t qword_601810;

// 0x601818
static int64_t qword_601818;

// 0x601820
static SectorLockFunc* dword_601820;

// 0x601824
static bool dword_601824;

// 0x601828
static tig_color_t dword_601828;

// 0x60182C
static int dword_60182C;

// 0x601830
static bool dword_601830;

// 0x601834
static unsigned int dword_601834;

// 0x601838
static int sector_refcount;

// 0x4CEF70
bool sector_init(GameInitInfo* init_info)
{
    dword_601820 = NULL;
    sector_iso_window_handle = init_info->iso_window_handle;
    sector_iso_window_invalidate_rect = init_info->invalidate_rect_func;
    sector_view_options.type = VIEW_TYPE_ISOMETRIC;
    dword_601830 = false;
    dword_6017AC = tig_color_make(255, 255, 0);
    dword_601828 = tig_color_make(255, 255, 0);
    sector_editor = init_info->editor;

    if (sector_editor) {
        sector_load_func = sector_load_editor;
        sector_save_func = sector_save_editor;
    } else {
        sector_load_func = sector_load_game;
        sector_save_func = sector_save_game;
    }

    dword_60179C = (char*)CALLOC(TIG_MAX_PATH, sizeof(*dword_60179C));
    dword_6017FC = (char*)CALLOC(TIG_MAX_PATH, sizeof(*dword_6017FC));

    dword_6017E8 = (int64_t*)CALLOC(151, sizeof(*dword_6017E8));
    dword_6017EC = (int64_t*)CALLOC(151, sizeof(*dword_6017EC));

    sector_limits_set(0x4000000, 0x4000000);

    if (!sub_4CF810(16)) {
        return false;
    }

    return true;
}

// 0x4CF0D0
void sector_reset()
{
    sub_4D0B40();
    dword_601834 = 0;
}

// 0x4CF0E0
void sector_exit()
{
    Sector601808* node;
    unsigned int index;
    Sector* sector;

    sub_4D0B40();

    while (dword_601808 != NULL) {
        node = dword_601808->next;
        FREE(dword_601808);
        dword_601808 = node;
    }

    for (index = 0; index < sector_cache_size; index++) {
        sector = &(sector_cache_entries[index].sector);
        if (!sector_object_list_exit(&(sector->objects))) {
            tig_debug_printf("Error destroying object list in sector_exit()\n");
        }

        if (!sector_block_list_exit(&(sector->blocks))) {
            tig_debug_printf("Error destroying tile block list in sector_exit()\n");
        }

        if (!sector_sound_list_exit(&(sector->sounds))) {
            tig_debug_printf("Error destroying sound list in sector_exit()\n");
        }

        if (!sector_script_list_exit(&(sector->sector_scripts))) {
            tig_debug_printf("Error destroying sector script list in sector_exit()\n");
        }

        if (!tile_script_list_exit(&(sector->tile_scripts))) {
            tig_debug_printf("Error destroying tile script list in sector_exit()\n");
        }

        if (!sector_roof_list_exit(&(sector->roofs))) {
            tig_debug_printf("Error destroying roof list in sector_exit()\n");
        }

        if (!sector_tile_list_exit(&(sector->tiles))) {
            tig_debug_printf("Error destroying tile list in sector_exit()\n");
        }

        if (!sector_light_list_exit(&(sector->lights))) {
            tig_debug_printf("Error destroying light list in sector_exit()\n");
        }
    }

    dword_601784 = 0;
    FREE(sector_cache_indexes);
    FREE(sector_cache_entries);
    FREE(dword_60179C);
    FREE(dword_6017FC);
    FREE(dword_6017E8);
    FREE(dword_6017EC);
    sub_4D2EA0();
}

// 0x4CF2C0
void sector_resize(GameResizeInfo* resize_info)
{
    sector_iso_window_handle = resize_info->window_handle;
}

// 0x4CF320
void sub_4CF320()
{
    sub_4D0B40();
    if (!gamelib_in_load()) {
        dword_601834 = 0;
    }
}

// 0x4CF340
bool sector_update_view(ViewOptions* view_options)
{
    sector_view_options = *view_options;
    return true;
}

// 0x4CF360
void sub_4CF360(SectorLockFunc* func)
{
    dword_601820 = func;
}

// 0x4CF370
void sub_4CF370()
{
    dword_601830 = !dword_601830;
    sector_iso_window_invalidate_rect(NULL);
}

// 0x4CF390
void sub_4CF390(UnknownContext* info)
{
    Sector601808* node;
    int sector_x;
    int sector_y;
    int townmap;
    tig_color_t color;
    int64_t location;
    int64_t location_x;
    int64_t location_y;
    int x;
    int y;
    int size;
    TigRect rect;
    TigRect dirty_rect;
    TigRectListNode* rect_node;

    if (dword_601830) {
        if (sector_view_options.type == VIEW_TYPE_TOP_DOWN) {
            node = info->field_C;
            while (node != NULL) {
                sector_x = SECTOR_X(node->id);
                sector_y = SECTOR_Y(node->id);

                townmap = sub_4BE380(node->id);
                if (townmap != 0) {
                    int red = townmap * (255 / townmap_count());
                    if ((townmap & 1) != 0) {
                        color = tig_color_make(red, 255 - red, 0);
                    } else {
                        color = tig_color_make(red, 0, 255 - red);
                    }
                } else {
                    color = dword_6017AC;
                }

                location = location_make(sector_x + 63, sector_y);
                sub_4B8680(location, &location_x, &location_y);

                x = location_x & 0xFFFFFFFF;
                y = location_y & 0xFFFFFFFF;
                size = sector_view_options.zoom * 64;

                rect.x = x;
                rect.y = y;
                rect.width = size;
                rect.height = 1;

                rect_node = *info->rects;
                while (rect_node != NULL) {
                    if (tig_rect_intersection(&rect, &(rect_node->rect), &dirty_rect) == TIG_OK) {
                        tig_window_box(sector_iso_window_handle, &dirty_rect, color);
                    }
                    rect_node = rect_node->next;
                }

                rect.x = x;
                rect.y = y;
                rect.width = 1;
                rect.height = size;

                rect_node = *info->rects;
                while (rect_node != NULL) {
                    if (tig_rect_intersection(&rect, &(rect_node->rect), &dirty_rect) == TIG_OK) {
                        tig_window_box(sector_iso_window_handle, &dirty_rect, color);
                    }
                    rect_node = rect_node->next;
                }

                rect.x = size + x - 1;
                rect.y = y;
                rect.width = 1;
                rect.height = size;

                rect_node = *info->rects;
                while (rect_node != NULL) {
                    if (tig_rect_intersection(&rect, &(rect_node->rect), &dirty_rect) == TIG_OK) {
                        tig_window_box(sector_iso_window_handle, &dirty_rect, color);
                    }
                    rect_node = rect_node->next;
                }

                rect.x = x;
                rect.y = size + y - 1;
                rect.width = size;
                rect.height = 1;

                rect_node = *info->rects;
                while (rect_node != NULL) {
                    if (tig_rect_intersection(&rect, &(rect_node->rect), &dirty_rect) == TIG_OK) {
                        tig_window_box(sector_iso_window_handle, &dirty_rect, color);
                    }
                    rect_node = rect_node->next;
                }

                rect.x = size / 2 - sector_view_options.zoom / 2 + x - 1;
                rect.y = y - sector_view_options.zoom;
                rect.width = 2;
                rect.height = 2 * sector_view_options.zoom;

                rect_node = *info->rects;
                while (rect_node != NULL) {
                    if (tig_rect_intersection(&rect, &(rect_node->rect), &dirty_rect) == TIG_OK) {
                        tig_window_box(sector_iso_window_handle, &dirty_rect, dword_601828);
                    }
                    rect_node = rect_node->next;
                }

                rect.x = x - sector_view_options.zoom;
                rect.y = size / 2 + y - 1;
                rect.width = 2 * sector_view_options.zoom;
                rect.height = 2;

                rect_node = *info->rects;
                while (rect_node != NULL) {
                    if (tig_rect_intersection(&rect, &(rect_node->rect), &dirty_rect) == TIG_OK) {
                        tig_window_box(sector_iso_window_handle, &dirty_rect, dword_601828);
                    }
                    rect_node = rect_node->next;
                }

                node = node->next;
            }
        }
    }
}

// 0x4CF790
bool sector_limits_set(int64_t x, int64_t y)
{
    if (x > 0x4000000 || y > 0x4000000) {
        return false;
    }

    sector_limit_x = x;
    sector_limit_y = y;

    return true;
}

// 0x4CF7E0
void sector_limits_get(int64_t* x, int64_t* y)
{
    *x = sector_limit_x;
    *y = sector_limit_y;
}

// 0x4CF810
bool sub_4CF810(unsigned int size)
{
    unsigned int index;
    Sector* sector;

    if (sector_refcount > 0) {
        return false;
    }

    sector_flush(0);

    if (size < 8) {
        size = 8;
    } else if (size > 128) {
        size = 128;
    }

    if (size < sector_cache_size) {
        // Requested size is smaller - shrink current cache.
        for (index = sector_cache_size - 1; index >= size; index--) {
            sector = &(sector_cache_entries[index].sector);
            sector_object_list_exit(&(sector->objects));
            sector_block_list_exit(&(sector->blocks));
            sector_sound_list_exit(&(sector->sounds));
            sector_script_list_exit(&(sector->sector_scripts));
            tile_script_list_exit(&(sector->tile_scripts));
            sector_roof_list_exit(&(sector->roofs));
            sector_light_list_exit(&(sector->lights));
        }

        sector_cache_entries = (SectorCacheEntry*)REALLOC(sector_cache_entries, sizeof(*sector_cache_entries) * size);
        sector_cache_indexes = (int*)REALLOC(sector_cache_indexes, sizeof(*sector_cache_indexes) * size);
    } else if (size > sector_cache_size) {
        sector_cache_entries = (SectorCacheEntry*)REALLOC(sector_cache_entries, sizeof(*sector_cache_entries) * size);
        sector_cache_indexes = (int*)REALLOC(sector_cache_indexes, sizeof(*sector_cache_indexes) * size);

        for (index = sector_cache_size; index < size; index++) {
            memset(&(sector_cache_entries[index]), 0, sizeof(*sector_cache_entries));
            sector_cache_indexes[index] = 0;

            sector = &(sector_cache_entries[index].sector);
            if (!sector_light_list_init(&(sector->lights))) {
                return false;
            }

            if (!sector_roof_list_init(&(sector->roofs))) {
                sector_light_list_init(&(sector->lights)); // FIXME: Should be sector_light_list_exit.
                return false;
            }

            if (!tile_script_list_init(&(sector->tile_scripts))) {
                sector_roof_list_init(&(sector->roofs)); // FIXME: Should be sector_roof_list_exit.
                sector_light_list_init(&(sector->lights)); // FIXME: Should be sector_light_list_exit.
                return false;
            }

            if (!sector_script_list_init(&(sector->sector_scripts))) {
                tile_script_list_init(&(sector->tile_scripts)); // FIXME: Should be tile_script_list_exit.
                sector_roof_list_init(&(sector->roofs)); // FIXME: Should be sector_roof_list_exit.
                sector_light_list_init(&(sector->lights)); // FIXME: Should be sector_light_list_exit.
                return false;
            }

            if (!sector_sound_list_init(&(sector->sounds))) {
                sector_script_list_init(&(sector->sector_scripts)); // FIXME: Should be sector_script_list_exit.
                tile_script_list_init(&(sector->tile_scripts)); // FIXME: Should be tile_script_list_exit.
                sector_roof_list_init(&(sector->roofs)); // FIXME: Should be sector_roof_list_exit.
                sector_light_list_init(&(sector->lights)); // FIXME: Should be sector_light_list_exit.
                return false;
            }

            if (!sector_block_list_init(&(sector->blocks))) {
                sector_sound_list_exit(&(sector->sounds));
                sector_script_list_init(&(sector->sector_scripts)); // FIXME: Should be sector_script_list_exit.
                tile_script_list_init(&(sector->tile_scripts)); // FIXME: Should be tile_script_list_exit.
                sector_roof_list_init(&(sector->roofs)); // FIXME: Should be sector_roof_list_exit.
                sector_light_list_init(&(sector->lights)); // FIXME: Should be sector_light_list_exit.
                return false;
            }

            if (!sector_object_list_init(&(sector->objects))) {
                sector_block_list_exit(&(sector->blocks));
                sector_sound_list_exit(&(sector->sounds));
                sector_script_list_init(&(sector->sector_scripts)); // FIXME: Should be sector_script_list_exit.
                tile_script_list_init(&(sector->tile_scripts)); // FIXME: Should be tile_script_list_exit.
                sector_roof_list_init(&(sector->roofs)); // FIXME: Should be sector_roof_list_exit.
                sector_light_list_init(&(sector->lights)); // FIXME: Should be sector_light_list_exit.
                return false;
            }

            sector->townmap_info = 0;
            sector->aptitude_adj = 0;
            sector->light_scheme = 0;
        }
    }

    sector_cache_size = size;
    sub_4D1100();

    return true;
}

// 0x4CFC50
int64_t sub_4CFC50(int64_t loc)
{
    int64_t x = LOCATION_GET_X(loc);
    int64_t y = LOCATION_GET_Y(loc);

    return (((y >> 6) & 0x3FFFFFF) << 26) | ((x >> 6) & 0x3FFFFFF);
}

// 0x4CFC90
int64_t sub_4CFC90(int64_t sector_id)
{
    int64_t x = SECTOR_X(sector_id) << 6;
    int64_t y = SECTOR_Y(sector_id) << 6;

    return LOCATION_MAKE(x, y);
}

// 0x4CFCD0
void sub_4CFCD0()
{
    // TODO: Incomplete.
}

// 0x4CFFA0
bool sub_4CFFA0(int64_t sec, int rot, int64_t* new_sec_ptr)
{
    int64_t x;
    int64_t y;

    // TODO: Check, probably wrong.
    x = SECTOR_X(sec);
    y = SECTOR_Y(sec);
    switch (rot) {
    case 0:
        x--;
        y--;
        break;
    case 1:
        x--;
        break;
    case 2:
        x--;
        y++;
        break;
    case 3:
        y++;
        break;
    case 4:
        x++;
        y++;
        break;
    case 5:
        x++;
        break;
    case 6:
        x++;
        y--;
        break;
    case 7:
        y--;
        break;
    }

    if (x < 0
        || x >= sector_limit_x
        || y < 0
        || y >= sector_limit_y) {
        return false;
    }

    *new_sec_ptr = SECTOR_FROM_XY(x, y);

    return true;
}

// 0x4D0090
bool sub_4D0090(LocRect* rect, SomeSectorStuff* a2)
{
    int x;
    int y;
    int width;
    int height;
    int64_t horizontal[4];
    int64_t vertical[4];

    width = sub_4D1310(rect->x1, rect->x2 + 1, 64, horizontal) - 1;
    if (width == 0) {
        return false;
    }

    height = sub_4D1310(rect->y1, rect->y2 + 1, 64, vertical) - 1;
    if (height == 0) {
        return false;
    }

    for (y = 0; y < height; y++) {
        a2->field_8[y].width = 0;
        a2->field_8[y].field_50 = (int)(vertical[y + 1] - vertical[y]);

        for (x = 0; x < width; x++) {
            a2->field_8[y].field_8[x] = LOCATION_MAKE(horizontal[x], vertical[y]);
            a2->field_8[y].field_20[x] = sub_4CFC50(a2->field_8[y].field_8[x]);
            a2->field_8[y].field_38[x] = sub_4D7090(a2->field_8[y].field_8[x]);
            a2->field_8[y].field_44[x] = (int)(horizontal[x + 1] - horizontal[x]);
        }

        a2->field_8[y].width = width;
    }

    a2->height = height;

    return true;
}

// 0x4D02E0
Sector601808* sub_4D02E0(LocRect* loc_rect)
{
    int x;
    int y;
    int width;
    int height;
    Sector601808* prev;
    Sector601808* node;

    width = sub_4D1310(loc_rect->x1, loc_rect->x2 + 1, 64, dword_6017E8) - 1;
    if (width == 0) {
        return NULL;
    }

    height = sub_4D1310(loc_rect->y1, loc_rect->y2 + 1, 64, dword_6017EC) - 1;
    if (height == 0) {
        return NULL;
    }

    prev = NULL;
    node = NULL;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            node = sub_4D13A0();
            node->next = prev;
            node->field_8 = LOCATION_MAKE(dword_6017E8[x], dword_6017EC[y]);
            node->id = sub_4CFC50(node->field_8);
            node->field_10 = (dword_6017E8[x + 1] & 0xFFFFFFFF) - (dword_6017E8[x] & 0xFFFFFFFF);
            node->field_14 = (dword_6017EC[y + 1] & 0xFFFFFFFF) - (dword_6017EC[y] & 0xFFFFFFFF);
            prev = node;
        }
    }

    return node;
}

// 0x4D0400
void sub_4D0400(Sector601808* node)
{
    while (node->next != NULL) {
        node = node->next;
    }

    node->next = dword_601808;
    dword_601808 = node;
}

// 0x4D0440
bool sub_4D0440(const char* a1, const char* a2)
{
    strcpy(dword_60179C, a1);
    strcpy(dword_6017FC, a2);
    sub_4D2D70();
    return true;
}

// 0x4D04A0
bool sub_4D04A0(uint64_t a1)
{
    char path[MAX_PATH];
    sprintf(path, "%s\\%I64u.sec", dword_60179C, a1);
    return tig_file_exists(path, NULL);
}

// 0x4D04E0
bool sub_4D04E0(int64_t id)
{
    int index;

    if (!sub_4D2CA0(id, &index)) {
        return false;
    }

    if ((sector_cache_entries[sector_cache_indexes[index]].sector.flags & 0x80000000) != 0) {
        return false;
    }

    return true;
}

// 0x4D0540
bool sector_lock(int64_t id, Sector** sector_ptr)
{
    SectorCacheEntry* cache_entry;
    unsigned int index;
    unsigned int oldest = -1;
    DateTime datetime;

    if (in_sector_lock) {
        tig_debug_printf("Warning: recursive sector lock detected\n");
        return false;
    }

    if (in_sector_enumerate) {
        return false;
    }

    if (!sub_410270()) {
        tig_debug_printf("ERROR: Attempt to lock a sector when the map is not valid!!!\n");
    }

    if (SECTOR_X(id) < 0
        || SECTOR_X(id) >= sector_limit_x
        || SECTOR_Y(id) < 0
        || SECTOR_Y(id) >= sector_limit_y) {
        return false;
    }

    in_sector_lock = true;
    dword_6017BC++;

    cache_entry = &(sector_cache_entries[sector_cache_indexes[dword_60182C]]);
    if (cache_entry->used && cache_entry->sector.id == id) {
        cache_entry->refcount++;
        cache_entry->timestamp = dword_6017BC;
    } else if (sub_4D2CA0(id, &dword_60182C)) {
        cache_entry = &(sector_cache_entries[sector_cache_indexes[dword_60182C]]);
        cache_entry->refcount++;
        cache_entry->timestamp = dword_6017BC;
    } else {
        if (dword_601784 >= sector_cache_size) {
            for (index = 0; index < sector_cache_size; index++) {
                if (sector_cache_entries[sector_cache_indexes[index]].refcount == 0) {
                    if (oldest == -1
                        || sector_cache_entries[sector_cache_indexes[index]].timestamp < sector_cache_entries[sector_cache_indexes[oldest]].timestamp) {
                        oldest = index;
                    }
                }
            }

            if (oldest == -1) {
                tig_debug_println("Warning: attempt to lock sector in cache failed due to lack of unlocked slots available.  This is bad.  Help.\n");
                in_sector_lock = false;
                return false;
            }

            tig_debug_printf("Sector cache full, removing oldest (%I64u)...\n",
                sector_cache_entries[sector_cache_indexes[oldest]].sector.id);

            sector_save_func(&(sector_cache_entries[sector_cache_indexes[oldest]].sector));
            sub_4D1400(&(sector_cache_entries[sector_cache_indexes[oldest]].sector));
            sector_cache_entries[sector_cache_indexes[oldest]].used = false;

            memcpy(&(sector_cache_indexes[oldest]),
                &(sector_cache_indexes[oldest + 1]),
                sizeof(*sector_cache_indexes) * (dword_601784 - oldest - 1));
            dword_601784--;

            sub_4D2CA0(id, &dword_60182C);
        }

        if (!sector_cache_find_unused(&index)) {
            tig_debug_printf("Error: sector_cache_find_unused() failed to find an available slot in the art cache!\n");
            in_sector_lock = false;
            return false;
        }

        datetime = qword_5B7CD0;
        for (unsigned int j = 0; j < dword_601834; j++) {
            if (dword_60180C[j].id == id) {
                datetime = sub_45A7D0(&(dword_60180C[j].datetime));
                memcpy(&(dword_60180C[j]),
                    &(dword_60180C[j + 1]),
                    sizeof(*dword_60180C) * (dword_601834 - j - 1));
                dword_601834--;
                break;
            }
        }

        cache_entry = &(sector_cache_entries[index]);
        cache_entry->sector.datetime = datetime;

        if (!sector_load_func(id, &(cache_entry->sector))) {
            tig_debug_printf("Error: attempt to lock sector %I64u in cache failed due to error in load.  This is bad.  Help.\n", id);
            in_sector_lock = false;
            return false;
        }

        memcpy(&(sector_cache_indexes[dword_60182C + 1]),
            &(sector_cache_indexes[dword_60182C]),
            sizeof(*sector_cache_indexes) * (dword_601784 - dword_60182C));
        sector_cache_indexes[dword_60182C] = index;
        dword_601784++;

        cache_entry->used = true;
        cache_entry->refcount = 1;
        cache_entry->timestamp = dword_6017BC;
        cache_entry->sector.id = id;
    }

    *sector_ptr = &(cache_entry->sector);

    in_sector_lock = false;
    sector_refcount++;

    return true;
}

// 0x4D0AE0
bool sector_unlock(int64_t id)
{
    int index;

    if (!sub_4D2CA0(id, &index)) {
        return false;
    }

    sector_cache_entries[sector_cache_indexes[index]].refcount--;
    sector_refcount--;

    return true;
}

// 0x4D0B40
void sub_4D0B40()
{
    unsigned int index;

    for (index = 0; index < dword_601784; index++) {
        sub_4D1400(&(sector_cache_entries[sector_cache_indexes[index]].sector));
        sector_cache_entries[sector_cache_indexes[index]].used = false;
    }

    dword_601784 = 0;
}

// 0x4D0BC0
void sector_flush(unsigned int flags)
{
    unsigned int index;
    unsigned int v1 = 0x3FFFFFFF;
    SectorCacheEntry* cache_entry;

    for (index = 0; index < dword_601784; index++) {
        cache_entry = &(sector_cache_entries[sector_cache_indexes[index]]);
        if (cache_entry->refcount == 0) {
            sector_save_func(&(cache_entry->sector));
            if ((flags & 0x1) == 0) {
                sub_4D1400(&(cache_entry->sector));
                cache_entry->used = false;
                memcpy(&(sector_cache_indexes[index]),
                    &(sector_cache_indexes[index + 1]),
                    sizeof(*sector_cache_indexes) * (v1 + dword_601784));
                index--;
                dword_601784--;
                v1 -= 0x3FFFFFFF;
            }
        }
        v1 += 0x3FFFFFFF;
    }

    sub_4D30A0();
}

// 0x4D0DE0
bool sub_4D0DE0(int64_t id)
{
    if (!dword_601824) {
        return true;
    }

    if ((id & 0x3FFFFFF) > qword_6017E0) {
        return false;
    }

    if ((id & 0x3FFFFFF) < qword_601818) {
        return false;
    }

    if ((id >> 26) > qword_6017C8) {
        return false;
    }

    if ((id >> 26) < qword_601810) {
        return false;
    }

    return true;
}

// 0x4D0E70
void sub_4D0E70(tig_art_id_t art_id)
{
    if (dword_6017D8 == dword_6017C4) {
        dword_6017C4 += 128;
        dword_6017F0 = (tig_art_id_t*)REALLOC(dword_6017F0, sizeof(*dword_6017F0) * dword_6017C4);
    }

    dword_6017F0[dword_6017D8++] = art_id;
}

// 0x4D0EE0
bool sub_4D0EE0(int64_t a1)
{
    return sub_4D2FC0(a1) != -1;
}

// 0x4D0F00
void sub_4D0F00(int64_t a1, bool a2)
{
    int v1;

    v1 = sub_4D2FC0(a1);
    if (a2) {
        if (v1 == -1) {
            sub_4D3000(a1);
        }
    } else {
        if (v1 != -1) {
            sub_4D3050(v1);
        }
    }
}

// 0x4D0F40
void sub_4D0F40()
{
    sub_4D0F70();
}

// 0x4D0F50
bool sub_4D0F50(const char* a1, const char* a2)
{
    sub_4D0F70();
    return sub_4D31C0(a1, a2);
}

// 0x4D0F70
void sub_4D0F70()
{
    if (dword_601794 != NULL) {
        FREE(dword_601794);
        dword_601794 = NULL;
    }

    dword_601790 = 0;
    dword_601798 = 0;
}

// 0x4D1040
void sub_4D1040()
{
    dword_5B7CD8++;
}

// 0x4D1050
void sub_4D1050()
{
    dword_5B7CD8--;
}

// 0x4D1060
void sector_enumerate(SectorEnumerateFunc* func)
{
    unsigned int index;

    in_sector_enumerate = true;

    for (index = 0; index < dword_601784; index++) {
        if (!func(&(sector_cache_entries[sector_cache_indexes[index]].sector))) {
            break;
        }
    }

    in_sector_enumerate = false;
}

// 0x4D10C0
bool sub_4D10C0(GameInitInfo* init_info)
{
    (void)init_info;

    sub_4D1100();

    return true;
}

// 0x4D10D0
void sub_4D10D0()
{
    dword_601834 = 0;
}

// 0x4D10E0
void sub_4D10E0()
{
    FREE(dword_60180C);
    dword_601834 = 0;
}

// 0x4D1100
void sub_4D1100()
{
    dword_60180C = (S60180C*)REALLOC(dword_60180C, sizeof(*dword_60180C) * sector_cache_size * 2);
    memset(dword_60180C, 0, sizeof(*dword_60180C) * sector_cache_size * 2);
    dword_601834 = 0;
}

// 0x4D1150
bool sub_4D1150(TigFile* stream)
{
    unsigned int index;
    unsigned int k;
    SectorCacheEntry* cache_entry;

    for (index = 0; index < sector_cache_size; index++) {
        cache_entry = &(sector_cache_entries[index]);
        if (cache_entry->used) {
            for (k = 0; k < dword_601834; k++) {
                if (dword_60180C[k].id == cache_entry->sector.id) {
                    break;
                }
            }

            if (k >= dword_601834) {
                if (dword_601834 == 2 * sector_cache_size) {
                    memcpy(&(dword_60180C[0]),
                        &(dword_60180C[1]),
                        sizeof(*dword_60180C) * (dword_601834 - 1));
                    dword_601834--;
                }

                dword_60180C[dword_601834].id = cache_entry->sector.id;
                dword_60180C[dword_601834].datetime = sub_45A7C0();
                dword_601834++;
            }
        }
    }

    if (tig_file_fwrite(&dword_601834, sizeof(dword_601834), 1, stream) != 1) {
        return false;
    }

    if (tig_file_fwrite(dword_60180C, sizeof(*dword_60180C), dword_601834, stream) != dword_601834) {
        return false;
    }

    return true;
}

// 0x4D12B0
bool sub_4D12B0(GameLoadInfo* load_info)
{
    if (tig_file_fread(&dword_601834, sizeof(dword_601834), 1, load_info->stream) != 1) {
        return false;
    }

    if (dword_601834 > 2 * sector_cache_size) {
        return false;
    }

    if (tig_file_fread(dword_60180C, sizeof(*dword_60180C), dword_601834, load_info->stream) != dword_601834) {
        return false;
    }

    return true;
}

// 0x4D1310
int sub_4D1310(int64_t a1, int64_t a2, int a3, int64_t* a4)
{
    int64_t v1;
    int cnt = 0;

    a4[cnt++] = a1;

    v1 = (a1 / a3 + 1) * a3;
    while (v1 < a2) {
        a4[cnt++] = v1;
        v1 += a3;
    }

    a4[cnt++] = a2;

    return cnt;
}

// 0x4D13A0
Sector601808* sub_4D13A0()
{
    Sector601808* node;

    if (dword_601808 == NULL) {
        sub_4D13D0();
    }

    node = dword_601808;
    dword_601808 = node->next;

    node->next = NULL;

    return node;
}

// 0x4D13D0
void sub_4D13D0()
{
    int index;
    Sector601808* node;

    for (index = 0; index < 4; index++) {
        node = (Sector601808*)MALLOC(sizeof(*node));
        node->next = dword_601808;
        dword_601808 = node;
    }
}

// 0x4D1400
void sub_4D1400(Sector* sector)
{
    if (sector->id != -1) {
        if (dword_601834 == 2 * sector_cache_size) {
            memcpy(dword_60180C, &(dword_60180C[1]), sizeof(*dword_60180C) * (dword_601834 - 1));
            dword_601834--;
        }

        dword_60180C[dword_601834].id = sector->id;
        dword_60180C[dword_601834].datetime = sub_45A7C0();
        dword_601834++;
    }

    sector->flags = 0x80000000;
    sector_light_list_reset(&(sector->lights));
    sector_tile_list_reset(&(sector->tiles));
    sector_roof_list_reset(&(sector->roofs));
    tile_script_list_reset(&(sector->tile_scripts));
    sector->townmap_info = 0;
    sector->aptitude_adj = 0;
    sector->light_scheme = 0;
    sector_script_list_reset(&(sector->sector_scripts));
    sector_sound_list_reset(&(sector->sounds));
    sector_block_list_reset(&(sector->blocks));
    sector_object_list_reset(&(sector->objects));
    sector->flags = 0;
    sector->id = -1;
}

// 0x4D1530
bool sector_load_editor(int64_t id, Sector* sector)
{
    bool v1 = false;
    char path[TIG_MAX_PATH];
    TigFile* stream;
    int placeholder;

    if (sub_4D0DE0(id)) {
        strcpy(path, dword_6017FC);
        strcat(path, "\\");
        _ui64toa(id, &path[strlen(path)], 10);
        strcat(path, ".sec");

        if (!tig_file_exists(path, NULL)) {
            strcpy(path, dword_60179C);
            strcat(path, "\\");
            _ui64toa(id, &path[strlen(path)], 10);
            strcat(path, ".sec");

            if (!tig_file_exists(path, NULL)) {
                terrain_sector_path(id, path);
                if (!tig_file_exists(path, NULL)) {
                    sub_4E86F0(sector);
                    v1 = true;
                }
            }
        }
    } else {
        terrain_sector_path(id, path);
        if (!tig_file_exists(path, NULL)) {
            sub_4E86F0(sector);
            v1 = true;
        }
    }

    sub_4D2C30("sector pre-load");

    if (!v1) {
        stream = tig_file_fopen(path, "rb");
        if (stream == NULL) {
            tig_debug_printf("Error opening sector file %s\n", path);
            return false;
        }

        if (!sector_light_list_load(&(sector->lights), stream, id)) {
            tig_debug_printf("Error loading lights from sector file %s\n", path);
            tig_file_fclose(stream);
            return false;
        }

        if (!sector_tile_list_load(&(sector->tiles), stream)) {
            tig_debug_printf("Error loading tiles from sector file %s\n", path);
            tig_file_fclose(stream);
            return false;
        }

        if (!sector_roof_list_load(&(sector->roofs), stream)) {
            tig_debug_printf("Error loading roofs from sector file %s\n", path);
            tig_file_fclose(stream);
            return false;
        }

        if (!obj_read_raw(&placeholder, sizeof(placeholder), stream)
            || placeholder < 0xAA0000 || placeholder > 0xAA0004) {
            tig_debug_printf("Error loading placeholder from sector file %s\n", path);
            tig_file_fclose(stream);
            return false;
        }

        if (placeholder != 0xAA0000) {
            if (!tile_script_list_load(&(sector->tile_scripts), stream)) {
                tig_debug_printf("Error loading tile scripts from sector file %s\n", path);
                tig_file_fclose(stream);
                return false;
            }

            if (placeholder >= 0xAA0002) {
                if (!sector_script_list_load(&(sector->sector_scripts), stream)) {
                    tig_debug_printf("Error loading sector scripts from sector file %s\n", path);
                    tig_file_fclose(stream);
                    return false;
                }
            }

            if (placeholder >= 0xAA0003) {
                if (tig_file_fread(&(sector->townmap_info), sizeof(sector->townmap_info), 1, stream) != 1) {
                    tig_debug_printf("Error loading townmap info from sector file %s\n", path);
                    tig_file_fclose(stream);
                    return false;
                }

                if (tig_file_fread(&(sector->aptitude_adj), sizeof(sector->aptitude_adj), 1, stream) != 1) {
                    tig_debug_printf("Error loading aptitude adjustment from sector file %s\n", path);
                    tig_file_fclose(stream);
                    return false;
                }

                if (tig_file_fread(&(sector->light_scheme), sizeof(sector->light_scheme), 1, stream) != 1) {
                    tig_debug_printf("Error loading light scheme from sector file %s\n", path);
                    tig_file_fclose(stream);
                    return false;
                }

                sector->flags &= ~0x0007;

                if (!sector_sound_list_load(&(sector->sounds), stream)) {
                    tig_debug_printf("Error loading sound list from sector file %s\n", path);
                    tig_file_fclose(stream);
                    return false;
                }
            }

            if (placeholder >= 0xAA0004) {
                if (!sector_block_list_load(&(sector->blocks), stream)) {
                    tig_debug_printf("Error loading tile blocking info from sector file %s\n", path);
                    tig_file_fclose(stream);
                    return false;
                }
            }
        }

        if (!objlist_load(&(sector->objects), stream, id)) {
            tig_debug_printf("Error loading objects from sector file %s\n", path);
            tig_file_fclose(stream);
            return false;
        }

        tig_file_fclose(stream);
    }

    sub_4D2C30("sector post-load (pre-fold)");
    objlist_fold(&(sector->objects), id, sub_45A9B0(&(sector->datetime), 3000));
    sector_light_list_fold(&(sector->lights), id, &(sector->tiles));
    sub_4D2C30("sector post-fold");

    return true;
}

// FIXME: There are many cleanup errors (mosty leaking `dif_stream`).
//
// 0x4D1A30
bool sector_load_game(int64_t id, Sector* sector)
{
    bool v1 = false;
    unsigned int flags = 0;
    int64_t v2 = -1;
    char sec_path[TIG_MAX_PATH];
    char dif_path[TIG_MAX_PATH];
    TigFile* sec_stream;
    TigFile* dif_stream;
    int placeholder;

    if (sub_4D0DE0(id)) {
        strcpy(sec_path, dword_60179C);
        strcat(sec_path, "\\");
        _ui64toa(id, &sec_path[strlen(sec_path)], 10);
        strcat(sec_path, ".sec");

        if (!tig_file_exists(sec_path, NULL)) {
            terrain_sector_path(id, sec_path);
            v2 = id;

            if (!tig_file_exists(sec_path, NULL)) {
                tig_debug_printf("Sector: ERROR loading %s terrain sector while attempting to load sector %I64d\n",
                    sec_path,
                    id);
                sub_4E86F0(sector);
                v1 = true;
            }
        }
    } else {
        tig_debug_printf("Sector %I64u is beyond the demo limits\n", id);
        terrain_sector_path(id, sec_path);
        v2 = id;

        if (!tig_file_exists(sec_path, NULL)) {
            sub_4E86F0(sector);
            v1 = true;
        }
    }

    sub_4D2C00("sector pre-load");
    if (!v1) {
        li_update();
        sec_stream = tig_file_fopen(sec_path, "rb");
        if (sec_stream == NULL) {
            tig_debug_printf("Error opening sector file %s\n", sec_path);
        }

        strcpy(dif_path, dword_6017FC);
        strcat(dif_path, "\\");
        _ui64toa(id, &dif_path[strlen(dif_path)], 10);
        strcat(dif_path, ".dif");

        if (tig_file_exists(dif_path, NULL)) {
            dif_stream = tig_file_fopen(dif_path, "rb");
            if (dif_stream != NULL) {
                if (tig_file_fread(&flags, sizeof(flags), 1, dif_stream) != 1) {
                    tig_debug_printf("Error reading flags from sector differences file %s\n", dif_path);
                    tig_file_fclose(dif_stream);
                    dif_stream = NULL;
                }
            } else {
                tig_debug_printf("Error opening sector differences file %s\n", dif_path);
            }
        } else {
            dif_stream = NULL;
        }

        li_update();
        if ((flags & 0x0001) != 0) {
            if (!sector_light_list_load_with_dif(&(sector->lights), sec_stream, dif_stream, v2)) {
                tig_debug_printf("Error loading lights with differences from files %s and %s\n", sec_path, dif_path);
                tig_file_fclose(sec_stream);
                tig_file_fclose(dif_stream);
                return false;
            }
        } else {
            if (!sector_light_list_load(&(sector->lights), sec_stream, v2)) {
                tig_debug_printf("Error loading lights from sector file %s\n", sec_path);
                tig_file_fclose(sec_stream);
                return false;
            }
        }

        li_update();
        if (!sector_tile_list_load(&(sector->tiles), sec_stream)) {
            tig_debug_printf("Error loading tiles from sector file %s\n", sec_path);
            tig_file_fclose(sec_stream);
            if (dif_stream != NULL) {
                tig_file_fclose(dif_stream);
            }
            return false;
        }
        if ((flags & 0x0002) != 0) {
            if (!sector_tile_list_load_with_dif(&(sector->tiles), dif_stream)) {
                tig_debug_printf("Error loading tiles from differences file %s\n", dif_path);
                tig_file_fclose(sec_stream);
                tig_file_fclose(dif_stream);
                return false;
            }
        }

        li_update();
        if (!sector_roof_list_load(&(sector->roofs), sec_stream)) {
            tig_debug_printf("Error loading roofs from sector file %s\n", sec_path);
            tig_file_fclose(sec_stream);
            if (dif_stream != NULL) {
                tig_file_fclose(dif_stream);
            }
            return false;
        }
        if ((flags & 0x0004) != 0) {
            if (!sector_roof_list_load_with_dif(&(sector->roofs), dif_stream)) {
                tig_debug_printf("Error loading roofs from differences file %s\n", dif_path);
                tig_file_fclose(sec_stream);
                tig_file_fclose(dif_stream);
                return false;
            }
        }

        li_update();
        if (!obj_read_raw(&placeholder, sizeof(placeholder), sec_stream)
            || placeholder < 0xAA0000 || placeholder > 0xAA0004) {
            tig_debug_printf("Error loading placeholder from sector file %s\n", sec_path);
            tig_file_fclose(sec_stream);
            if (dif_stream != NULL) {
                tig_file_fclose(dif_stream);
            }
            return false;
        }

        if (placeholder != 0xAA0000) {
            li_update();
            if (!tile_script_list_load(&(sector->tile_scripts), sec_stream)) {
                tig_debug_printf("Error loading tile scripts from sector file %s\n", sec_path);
                tig_file_fclose(sec_stream);
                return false;
            }
            if ((flags & 0x0010) != 0) {
                if (!tile_script_list_load_with_dif(&(sector->tile_scripts), dif_stream)) {
                    tig_debug_printf("Error loading tile scripts from differences file %s\n", dif_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }
            }

            if (placeholder >= 0xAA0002) {
                li_update();
                if (!sector_script_list_load(&(sector->sector_scripts), sec_stream)) {
                    tig_debug_printf("Error loading sector scripts from sector file %s\n", sec_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }
                if ((flags & 0x0020) != 0) {
                    if (!sector_script_list_load_with_dif(&(sector->sector_scripts), dif_stream)) {
                        tig_debug_printf("Error loading sector scripts from differences file %s\n", dif_path);
                        tig_file_fclose(sec_stream);
                        return false;
                    }
                }
            }

            if (placeholder >= 0xAA0003) {
                li_update();
                if (tig_file_fread(&(sector->townmap_info), sizeof(sector->townmap_info), 1, sec_stream) != 1) {
                    tig_debug_printf("Error loading townmap info from sector file %s\n", sec_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }
                if ((flags & 0x0040) != 0) {
                    // FIXME: Wrong message.
                    tig_debug_printf("Error loading sound list from differences file %s\n", dif_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }

                li_update();
                if (tig_file_fread(&(sector->aptitude_adj), sizeof(sector->aptitude_adj), 1, sec_stream) != 1) {
                    tig_debug_printf("Error loading aptitude adjustment from sector file %s\n", sec_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }
                if ((flags & 0x0080) != 0) {
                    tig_debug_printf("Error loading aptitude adjustment from differences file %s\n", dif_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }

                li_update();
                if (tig_file_fread(&(sector->light_scheme), sizeof(sector->light_scheme), 1, sec_stream) != 1) {
                    tig_debug_printf("Error loading light scheme from sector file %s\n", sec_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }
                if ((flags & 0x0100) != 0) {
                    tig_debug_printf("Error loading light scheme from differences file %s\n", dif_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }

                li_update();
                if (!sector_sound_list_load(&(sector->sounds), sec_stream)) {
                    tig_debug_printf("Error loading sound list from sector file %s\n", sec_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }
                if ((flags & 0x0200) != 0) {
                    if (!sector_sound_list_load_with_dif(&(sector->sounds), dif_stream)) {
                        tig_debug_printf("Error loading sound list from differences file %s\n", dif_path);
                        tig_file_fclose(sec_stream);
                        return false;
                    }
                }

                sector->flags &= ~0x0007;
            }

            if (placeholder >= 0xAA0004) {
                li_update();
                if (!sector_block_list_load(&(sector->blocks), sec_stream)) {
                    tig_debug_printf("Error loading tile blocking info from sector file %s\n", sec_path);
                    tig_file_fclose(sec_stream);
                    return false;
                }
                if ((flags & 0x0400) != 0) {
                    if (!sector_block_list_load_with_dif(&(sector->blocks), dif_stream)) {
                        tig_debug_printf("Error loading tile blocking info from differences file %s\n", dif_path);
                        tig_file_fclose(sec_stream);
                        return false;
                    }
                }
            }
        }

        li_update();
        if ((flags & 0x0008) != 0) {
            if (!objlist_load_with_difs(&(sector->objects), sec_stream, dif_stream, v2)) {
                tig_debug_printf("Error loading objects with differences from files %s and %s\n", sec_path, dif_path);
                tig_file_fclose(sec_stream);
                tig_file_fclose(dif_stream);
                return false;
            }
        } else {
            if (!objlist_load(&(sector->objects), sec_stream, v2)) {
                tig_debug_printf("Error loading objects from sector file %s\n", sec_path);
                tig_file_fclose(sec_stream);
                return false;
            }
        }

        tig_file_fclose(sec_stream);
        if (flags != 0) {
            tig_file_fclose(dif_stream);
        }
    }

    sub_4D2C00("sector post-load (pre-fold)");
    li_update();
    objlist_fold(&(sector->objects), id, sub_45A9B0(&(sector->datetime), 3000));
    li_update();
    sector_light_list_fold(&(sector->lights), id, &(sector->tiles));
    sub_4D2C00("sector post-fold");

    if (sector->townmap_info != 0) {
        sub_4D2ED0(sector);
    }

    return true;
}

// 0x4D22E0
bool sector_save_editor(Sector* sector)
{
    unsigned int flags = 0;
    char path[TIG_MAX_PATH];

    if (sector_light_list_is_modified(&(sector->lights))) {
        flags |= 0x0001;
    }

    if (sector->tiles.dif) {
        flags |= 0x0002;
    }

    if (sector_roof_list_is_modified(&(sector->roofs))) {
        flags |= 0x0004;
    }

    if (tile_script_list_is_modified(&(sector->tile_scripts))) {
        flags |= 0x0010;
    }

    if (sector_script_list_is_modified(&(sector->sector_scripts))) {
        flags |= 0x0020;
    }

    if ((sector->flags & 0x01) != 0) {
        flags |= 0x0040;
    }

    if ((sector->flags & 0x02) != 0) {
        flags |= 0x0080;
    }

    if ((sector->flags & 0x04) != 0) {
        flags |= 0x0100;
    }

    if (sub_4F81B0(&(sector->sounds))) {
        flags |= 0x0200;
    }

    if (sector->blocks.modified) {
        flags |= 0x0400;
    }

    if (sub_4F1A20(&(sector->objects))) {
        flags |= 0x0008;
    }

    if (flags == 0) {
        return true;
    }

    strcpy(path, dword_6017FC);
    strcat(path, "\\");
    _ui64toa(sector->id, &(path[strlen(path)]), 10);

    return sub_4D2460(sector, path);
}

// 0x4D2460
bool sub_4D2460(Sector* sector, const char* base_path)
{
    char fname[TIG_MAX_PATH];
    char path[TIG_MAX_PATH];
    TigFile* stream;
    int placeholder;

    if (!sub_4D0DE0(sector->id)) {
        return true;
    }

    if (dword_601820 != NULL) {
        _splitpath(base_path, NULL, NULL, fname, NULL);
        sprintf(path, "%s_sec.loc", fname);
        if (!dword_601820(path)) {
            tig_debug_println("Trying to save a sector that we don't have write locked.");
            return false;
        }
    }

    sprintf(path, "%s.sec", base_path);

    stream = tig_file_fopen(path, "wb");
    if (stream == NULL) {
        tig_debug_printf("Error creating sector data file %s\n", path);
        tig_file_fclose(stream); // FIXME: Crash! (dereferencing NULL).
        return false;
    }

    sub_4D2C30("sector pre-save");

    if (!sector_light_list_save(&(sector->lights), stream)) {
        tig_debug_printf("Error saving lights to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if (!sector_tile_list_save(&(sector->tiles), stream)) {
        tig_debug_printf("Error saving tiles to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if (!sector_roof_list_save(&(sector->roofs), stream)) {
        tig_debug_printf("Error saving roofs to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    placeholder = 0xAA0004;
    if (!obj_write_raw(&placeholder, sizeof(placeholder), stream)) {
        tig_debug_printf("Error saving placeholder to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if (!tile_script_list_save(&(sector->tile_scripts), stream)) {
        tig_debug_printf("Error saving tile scripts to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if (!sector_script_list_save(&(sector->sector_scripts), stream)) {
        tig_debug_printf("Error saving sector scripts to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if (tig_file_fwrite(&(sector->townmap_info), sizeof(sector->townmap_info), 1, stream) != 1) {
        tig_debug_printf("Error saving townmap info to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if (tig_file_fwrite(&(sector->aptitude_adj), sizeof(sector->townmap_info), 1, stream) != 1) {
        tig_debug_printf("Error saving aptitude adjustment to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if (tig_file_fwrite(&(sector->light_scheme), sizeof(sector->townmap_info), 1, stream) != 1) {
        tig_debug_printf("Error saving light scheme to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    sector->flags &= ~0x0007;

    if (!sector_sound_list_save(&(sector->sounds), stream)) {
        tig_debug_printf("Error saving sound list to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if (!sector_block_list_save(&(sector->blocks), stream)) {
        tig_debug_printf("Error saving tile blocking info to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if (!objlist_save(&(sector->objects), stream)) {
        tig_debug_printf("Error saving objects to sector data file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    sub_4D2C30("sector post-save");
    tig_file_fclose(stream);

    return true;
}

// 0x4D2750
bool sector_save_game(Sector* sector)
{
    unsigned int flags;
    char path[TIG_MAX_PATH];
    TigFile* stream;

    if (!sub_4D0DE0(sector->id)) {
        return true;
    }

    flags = 0;

    if (sector_light_list_is_modified(&(sector->lights))) {
        flags |= 0x0001;
    }

    if (sector->tiles.dif) {
        flags |= 0x0002;
    }

    if (sector_roof_list_is_modified(&(sector->roofs))) {
        flags |= 0x0004;
    }

    if (tile_script_list_is_modified(&(sector->tile_scripts))) {
        flags |= 0x0010;
    }

    if (sector_script_list_is_modified(&(sector->sector_scripts))) {
        flags |= 0x0020;
    }

    if ((sector->flags & 0x1) != 0) {
        flags |= 0x0040;
    }

    if ((sector->flags & 0x2) != 0) {
        flags |= 0x0080;
    }

    if ((sector->flags & 0x4) != 0) {
        flags |= 0x0100;
    }

    if (sub_4F81B0(&(sector->sounds))) {
        flags |= 0x0200;
    }

    if (sector->blocks.modified) {
        flags |= 0x0400;
    }

    if (sub_4F1A20(&(sector->objects))) {
        flags |= 0x0008;
    }

    if (flags == 0) {
        return true;
    }

    strcpy(path, dword_6017FC);
    strcat(dword_6017FC, "\\");
    _ui64toa(sector->id, &(path[strlen(path)]), 10);
    strcat(path, ".dif");

    stream = tig_file_fopen(path, "wb");
    if (stream == NULL) {
        tig_debug_printf("Error creating differences file %s\n", path);
        tig_file_fclose(stream); // FIXME: Crash!
        return false;
    }

    if (tig_file_fwrite(&flags, sizeof(flags), 1, stream) != 1) {
        tig_debug_printf("Error writing flags to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    sub_4D2C00("sector pre-save");

    if ((flags & 0x0001) != 0
        && !sector_light_list_save_with_dif(&(sector->lights), stream)) {
        tig_debug_printf("Error saving lights to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0002) != 0
        && !sector_tile_list_save_with_dif(&(sector->tiles), stream)) {
        tig_debug_printf("Error saving tiles to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0004) != 0
        && !sector_roof_list_save_with_dif(&(sector->roofs), stream)) {
        tig_debug_printf("Error saving roofs to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0010) != 0
        && !tile_script_list_save_with_dif(&(sector->tile_scripts), stream)) {
        tig_debug_printf("Error saving tile scripts to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0020) != 0
        && !sector_script_list_save_with_dif(&(sector->sector_scripts), stream)) {
        tig_debug_printf("Error saving sector scripts to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0040) != 0
        && tig_file_fwrite(&(sector->townmap_info), sizeof(sector->townmap_info), 1, stream) != 1) {
        tig_debug_printf("Error saving townmap info to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0080) != 0
        && tig_file_fwrite(&(sector->townmap_info), sizeof(sector->townmap_info), 1, stream) != 1) {
        tig_debug_printf("Error saving aptitude adjustment to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0100) != 0
        && tig_file_fwrite(&(sector->townmap_info), sizeof(sector->townmap_info), 1, stream) != 1) {
        tig_debug_printf("Error saving light scheme to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0200) != 0
        && !sector_sound_list_save_with_dif(&(sector->sounds), stream)) {
        tig_debug_printf("Error saving sound list to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0400) != 0
        && !sector_block_list_save_with_dif(&(sector->blocks), stream)) {
        tig_debug_printf("Error saving sound list to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    if ((flags & 0x0008) != 0
        && !objlist_save_with_dif(&(sector->objects), stream)) {
        tig_debug_printf("Error saving objects to differences file %s\n", path);
        tig_file_fclose(stream);
        tig_file_remove(path);
        return false;
    }

    tig_file_fclose(stream);
    sub_4D2C00("sector post-save");

    return true;
}

// 0x4D2C00
void sub_4D2C00(const char* section)
{
    if (!obj_validate_system(1)) {
        tig_debug_printf("Object system validate failed in %s\n", section);
        tig_message_post_quit(0);
    }
}

// 0x4D2C30
void sub_4D2C30(const char* section)
{
    char str[300];

    if (!obj_validate_system(1)) {
        _snprintf(str, sizeof(str) - 1, "Object system validate failed in %s", section);
        str[sizeof(str) - 1] = '\0';
        MessageBoxA(NULL, str, "Execution Halting", MB_TASKMODAL | MB_ICONHAND);
        tig_message_post_quit(0);
    }
}

// 0x4D2CA0
bool sub_4D2CA0(int64_t id, int* index_ptr)
{
    int l;
    int r;
    int m;
    int64_t curr;

    l = 0;
    r = dword_601784 - 1;
    while (l <= r) {
        m = (l + r) / 2;
        curr = sector_cache_entries[sector_cache_indexes[m]].sector.id;

        if (curr < id) {
            l = m + 1;
        } else if (curr > id) {
            r = m - 1;
        } else {
            *index_ptr = m;
            return true;
        }
    }

    *index_ptr = l;
    return false;
}

// 0x4D2D30
bool sector_cache_find_unused(unsigned int* index_ptr)
{
    unsigned int index;

    // NOTE: Unsigned comparison.
    for (index = 0; index < sector_cache_size; index++) {
        if (!sector_cache_entries[index].used) {
            *index_ptr = index;
            return true;
        }
    }

    return false;
}

// 0x4D2D70
void sub_4D2D70()
{
    char buffer[TIG_MAX_PATH];
    TigFile* stream;

    dword_601824 = false;
    sprintf(buffer, "%s\\134217730001.sec", dword_60179C);

    if (!tig_file_exists(buffer, NULL)) {
        return;
    }

    stream = tig_file_fopen(buffer, "rb");
    if (stream == NULL) {
        return;
    }

    do {
        if (tig_file_fgets(buffer, sizeof(buffer), stream) == NULL) break;
        qword_601818 = _atoi64(buffer);

        if (tig_file_fgets(buffer, sizeof(buffer), stream) == NULL) break;
        qword_6017E0 = _atoi64(buffer);

        if (tig_file_fgets(buffer, sizeof(buffer), stream) == NULL) break;
        qword_601810 = _atoi64(buffer);

        if (tig_file_fgets(buffer, sizeof(buffer), stream) == NULL) break;
        qword_6017C8 = _atoi64(buffer);

        dword_601824 = true;
    } while (0);

    tig_file_fclose(stream);
}

// 0x4D2EA0
void sub_4D2EA0()
{
    if (dword_6017F0 != NULL) {
        FREE(dword_6017F0);
    }

    dword_6017D8 = 0;
    dword_6017C4 = 0;
}

// 0x4D2ED0
void sub_4D2ED0(Sector* sector)
{
    int index;

    if (dword_5B7CD8 > 0) {
        li_update();
        sub_4F7CC0(&(sector->tiles));
        li_update();
        sub_4F7FF0(&(sector->roofs));
        li_update();
        sub_4F1F00(&(sector->objects));
        li_update();
        sub_4D2F90();
        li_update();

        for (index = 0; index < dword_6017D8; index++) {
            while (index + 1 < dword_6017D8 && dword_6017F0[index] == dword_6017F0[index + 1]) {
                index++;
            }
            sub_502290(dword_6017F0[index]);
            li_update();
        }

        sub_4D2F80();
    }
}

// 0x4D2F80
void sub_4D2F80()
{
    dword_6017D8 = 0;
}

// 0x4D2F90
void sub_4D2F90()
{
    qsort(dword_6017F0, dword_6017D8, sizeof(*dword_6017F0), sub_4D2FB0);
}

// 0x4D2FB0
int sub_4D2FB0(const tig_art_id_t* a, const tig_art_id_t* b)
{
    return *a - *b;
}

// 0x4D2FC0
int sub_4D2FC0(int64_t a1)
{
    int index;

    for (index = 0; index < dword_601790; index++) {
        if (dword_601794[index] == a1) {
            return index;
        }
    }

    return -1;
}

// 0x4D3000
void sub_4D3000(int64_t a1)
{
    dword_601794 = (int64_t*)REALLOC(dword_601794, sizeof(*dword_601794) * (dword_601790 + 1));
    dword_601794[dword_601790++] = a1;
    dword_601798 = true;
}

// 0x4D3050
void sub_4D3050(int idx)
{
    memcpy(&(dword_601794[idx]),
        &(dword_601794[idx + 1]),
        sizeof(*dword_601794) * (dword_601790 - idx - 1));
    dword_601790--;
    dword_601798 = true;
}

// 0x4D30A0
bool sub_4D30A0()
{
    char path[MAX_PATH];
    TigFile* stream;

    if (dword_601798) {
        if (dword_601790 != 0) {
            sprintf(path, "%s\\map.sbf", dword_6017FC);

            stream = tig_file_fopen(path, "wb");
            if (stream == NULL) {
                return false;
            }

            if (tig_file_fwrite(&dword_601790, sizeof(dword_601790), 1, stream) != 1) {
                tig_file_fclose(stream);
                return false;
            }

            if (tig_file_fwrite(dword_601794, sizeof(*dword_601794), dword_601790, stream) != dword_601790) {
                tig_file_fclose(stream);
                return false;
            }

            tig_file_fclose(stream);
        } else {
            sprintf(path, "%s\\null.sbf", dword_6017FC);

            stream = tig_file_fopen(path, "wb");
            if (stream == NULL) {
                return false;
            }

            tig_file_fputc(1, stream);
            tig_file_fclose(stream);
        }

        dword_601798 = false;
    }

    return true;
}

// 0x4D31C0
bool sub_4D31C0(const char* a1, const char* a2)
{
    char path[MAX_PATH];
    TigFile* stream;

    sprintf(path, "%s\\map.sbf", a2);

    if (!tig_file_exists(path, NULL)) {
        sprintf(path, "%s\\map.sbf", a1);
        if (!tig_file_exists(path, NULL)) {
            return false;
        }
    }

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        return false;
    }

    if (tig_file_fread(&dword_601790, sizeof(dword_601790), 1, stream) != 1) {
        tig_file_fclose(stream);
        return false;
    }

    dword_601794 = (int64_t*)MALLOC(sizeof(*dword_601794) * dword_601790);
    if (tig_file_fread(dword_601794, sizeof(*dword_601794), dword_601790, stream) != dword_601790) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}
