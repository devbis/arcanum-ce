#include "tig/art.h"

#include "tig/debug.h"
#include "tig/memory.h"
#include "tig/video.h"

typedef struct TigArtCacheEntry {
    /* 0004 */ char path[MAX_PATH];
    /* 0108 */ int field_108;
    /* 010C */ unsigned int art_id;
    /* 0130 */ int video_buffer_count;
    /* 0194 */ uint8_t field_194[4][8];
    /* 01B4 */ TigVideoBuffer** video_buffers[4][8];
    /* 0254 */ int field_254[4];
    /* 0264 */ size_t system_memory_usage;
    /* 0268 */ size_t video_memory_usage;
};

static_assert(sizeof(TigArtCacheEntry) == 0x26C, "wrong size");

// 0x5BE880
static int dword_5BE880[16] = {
    0,
    1,
    8,
    3,
    4,
    5,
    6,
    7,
    8,
    3,
    10,
    11,
    6,
    7,
    14,
    15,
};

// 0x5BE8C0
static int dword_5BE8C0[16] = {
    0,
    1,
    2,
    9,
    4,
    5,
    12,
    13,
    2,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
};

// 0x5BE980
static int dword_5BE980[5] = {
    1,
    0,
    0,
    2,
    1,
};

// 0x5BE994
static int dword_5BE994[32] = {
    0,
    0,
    1,
    2,
    1,
    1,
    1,
    1,
    2,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    2,
    2,
    2,
    1,
    1,
    1,
};

// 0x604710
static TigArtFilePathResolver* tig_art_file_path_resolver;

// 0x60471C
static int tig_art_cache_entries_length;

// 0x604720
static TigArtCacheEntry* tig_art_cache_entries;

// 0x604724
static size_t tig_art_total_system_memory;

// 0x60472C
static int tig_art_cache_entries_capacity;

// 0x604730
static size_t tig_art_available_video_memory;

// 0x604738
static int tig_art_bytes_per_pixel;

// 0x60473C
static bool tig_art_initialized;

// 0x604740
static size_t tig_art_available_system_memory;

// 0x604748
static size_t tig_art_total_video_memory;

// 0x60474C
static int tig_art_bits_per_pixel;

// 0x604750
static TigContextF20* dword_604750;

// 0x500590
int tig_art_init(TigContext* ctx)
{
    if (tig_art_initialized) {
        return 2;
    }

    tig_art_cache_entries_capacity = 512;
    tig_art_cache_entries_length = 0;
    tig_art_cache_entries = malloc(317440);

    size_t total_memory;
    size_t available_memory;
    tig_memory_get_system_status(&total_memory, &available_memory);
    tig_art_available_system_memory = total_memory >> 2;
    tig_art_total_system_memory = total_memory >> 2;

    if (tig_video_get_video_memory_status(&total_memory, &available_memory) != TIG_OK) {
        available_memory = tig_art_total_system_memory;
    }

    tig_art_available_video_memory = available_memory * 0.5;
    tig_art_total_video_memory = tig_art_available_video_memory;
    tig_debug_printf("Art mem vid avail is %d\n", tig_art_available_video_memory);

    tig_art_bits_per_pixel = ctx->bpp;
    tig_art_bytes_per_pixel = tig_art_bits_per_pixel / 8;
    tig_art_file_path_resolver = ctx->art_file_path_resolver;
    dword_604750 = ctx->field_20;

    tig_art_initialized = true;

    dword_604718 = sub_51FB10() == TIG_OK;

    return TIG_OK;
}

// 0x500690
void tig_art_exit()
{
    if (tig_art_initialized) {
        tig_art_flush();

        if (tig_art_cache_entries != NULL) {
            free(tig_art_cache_entries);
            tig_art_cache_entries = NULL;
            tig_art_cache_entries_length = 0;
            tig_art_cache_entries_capacity = 0;
        }

        tig_art_initialized = false;
    }
}

// 0x5006D0
void tig_art_ping()
{
}

// 0x501DD0
int sub_501DD0(unsigned int a1, unsigned int a2, unsigned int* art_id)
{
    if (a1 >= 512 || a2 >= 4) {
        return TIG_ERR_12;
    }

    // TODO: Check.
    *art_id = (8 << 28) | ((a1 & 0x1FF) << 19) | ((a2 & 3) << 4);

    return TIG_OK;
}

// 0x5021E0
void tig_art_flush()
{
    if (tig_art_initialized) {
        for (int index = 0; index < tig_art_cache_entries_length; index++) {
            sub_51B490(index);
        }
        tig_art_cache_entries_length = 0;
    }
}

// 0x502360
int tig_art_blit(TigArtBlitSpec* blit_spec)
{
    // TODO: Incomplete.
}

// 0x502700
int tig_art_type(unsigned int art_id)
{
    return art_id >> 28;
}

// 0x502710
int sub_502710(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
        return 0;
    case TIG_ART_TYPE_UNIQUE_NPC:
        return (art_id >> 20) & 0xFF;
    case TIG_ART_TYPE_ITEM:
        return (art_id >> 17) & 0x7FF;
    case TIG_ART_TYPE_INTERFACE:
        return (art_id >> 16) & 0xFFF;
    default:
        return (art_id >> 19) & 0x1FF;
    }
}

// 0x502780
unsigned int sub_502780(unsigned int art_id, unsigned int value)
{
    unsigned int max;
    unsigned int mask;
    unsigned int shift;

    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
        return 0;
    case TIG_ART_TYPE_UNIQUE_NPC:
        max = 256;
        mask = 0xFF00000;
        shift = 20;
        break;
    case TIG_ART_TYPE_ITEM:
        max = sub_502830(art_id);
        mask = 0xFFE0000;
        shift = 17;
        break;
    case TIG_ART_TYPE_INTERFACE:
        max = 4096;
        mask = 0xFFF0000;
        shift = 16;
        break;
    default:
        max = 512;
        mask = 0xFF80000;
        shift = 19;
        break;
    }

    if (value < max) {
        return (art_id & ~mask) | (value << shift);
    }

    tig_debug_println("Range exceeded in art set.");

    return art_id;
}

// 0x502830
unsigned int sub_502830(unsigned int art_id)
{
    if (tig_art_type(art_id) != TIG_ART_TYPE_ITEM) {
        return 0;
    }

    unsigned int max = 1000;

    switch (sub_504550(art_id)) {
    case TIG_ART_TYPE_6_SUBTYPE_0:
    case TIG_ART_TYPE_6_SUBTYPE_1:
        max = 20;
        break;
    case TIG_ART_TYPE_6_SUBTYPE_2:
        if (sub_504570(art_id) == 0) {
            max = 20;
        }
        break;
    }

    return max;
}

// 0x502880
int sub_502880(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_INTERFACE:
    case TIG_ART_TYPE_8:
    case TIG_ART_TYPE_ROOF:
    case TIG_ART_TYPE_ITEM:
    case TIG_ART_TYPE_FACADE:
        return 0;
    case TIG_ART_TYPE_LIGHT:
        return (art_id >> 9) & 7;
    case TIG_ART_TYPE_EYE_CANDY:
        return (art_id >> 9) & 7;
    default:
        return (art_id >> 11) & 7;
    }
}

// 0x5028E0
unsigned int sub_5028E0(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_INTERFACE:
    case TIG_ART_TYPE_8:
    case TIG_ART_TYPE_ROOF:
    case TIG_ART_TYPE_ITEM:
    case TIG_ART_TYPE_FACADE:
        return art_id;
    }

    return sub_502930(art_id, sub_502880(art_id) + 1);
}

// 0x502B50
int sub_502B50(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_ITEM:
        return 0;
    case TIG_ART_TYPE_INTERFACE:
    case TIG_ART_TYPE_8:
        return (art_id >> 8) & 0xFF;
    case TIG_ART_TYPE_FACADE:
        return (art_id >> 1) & 0x3FF;
    case TIG_ART_TYPE_LIGHT:
        return (art_id >> 12) & 0x7F;
    case TIG_ART_TYPE_EYE_CANDY:
        return (art_id >> 12) & 0x7F;
    default:
        return (art_id >> 14) & 0x1F;
    }
}

// 0x502BC0
unsigned int sub_502BC0(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_ITEM:
        return art_id;
    }

    int v1 = sub_502B50(art_id);
    return sub_502C40(art_id, v1 + 1);
}

// 0x502C00
unsigned int sub_502C00(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_ITEM:
        return art_id;
    }

    int v1 = sub_502B50(art_id);
    if (v1 > 0) {
        v1--;
    }

    return sub_502C40(art_id, v1);
}

// 0x502D30
unsigned int sub_502D30(unsigned int art_id, int value)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_PORTAL:
    case TIG_ART_TYPE_ROOF:
        if (value >= 16) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0xF) | value;
    default:
        return art_id;
    }
}

// 0x502D80
int tig_art_palette(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_LIGHT:
    case TIG_ART_TYPE_FACADE:
        return 0;
    default:
        return (art_id >> 4) & 3;
    }
}

// 0x502DB0
unsigned int tig_art_set_palette(unsigned int art_id, int value)
{
    if (value >= 4) {
        tig_debug_println("Range exceeded in art set.");
        value = 0;
    }

    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_LIGHT:
    case TIG_ART_TYPE_FACADE:
        return art_id;
    default:
        return (art_id & ~0x30) | (value << 4);
    }
}

// 0x5030B0
int tig_art_data(unsigned int art_id, TigArtData* art_data)
{
    // TODO: Incomplete.
}

// 0x5031C0
int tig_art_frame_data(unsigned int art_id, TigArtFrameData* art_frame_data)
{
    // TODO: Incomplete.
}

// 0x5036B0
int sub_5036B0(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
        return (art_id >> 22) & 0x3F;
    case TIG_ART_TYPE_FACADE:
        return (art_id >> 11) & 0x3F;
    default:
        return 0;
    }
}

// 0x5036E0
int sub_5036E0(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_TILE) {
        return (art_id >> 16) & 0x3F;
    }

    return 0;
}

// 0x503700
int sub_503700(unsigned int art_id)
{
    if (tig_art_type(art_id) != TIG_ART_TYPE_TILE) {
        return 0;
    }

    int v1 = (art_id >> 12) & 0xF;
    if ((sub_504FD0(art_id) & 1) != 0) {
        v1 = dword_5BE8C0[v1];
    }
    return v1;
}

// 0x5037B0
int sub_5037B0(unsigned int art_id)
{
    if (tig_art_type(art_id) != TIG_ART_TYPE_TILE) {
        return 0;
    }

    int v1 = (art_id >> 9) & 7;
    int v2 = sub_503700(art_id);
    if (dword_5BE8C0[v2] == dword_5BE880[v2]) {
        if ((sub_504FD0(art_id) & 1) != 0) {
            v1 += 8;
        }
    }
    return v1;
}

// 0x5038C0
int sub_5038C0(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_TILE) {
        return (art_id >> 8) & 1;
    }

    if (tig_art_type(art_id) == TIG_ART_TYPE_FACADE) {
        return (art_id >> 25) & 1;
    }

    return 0;
}

// 0x503900
int sub_503900(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_FACADE) {
        return (art_id >> 26) & 1;
    }

    if (tig_art_type(art_id) == TIG_ART_TYPE_TILE) {
        if (sub_503950(art_id) != 0 && sub_503990(art_id) != 0) {
            return 1;
        }
    }

    return 0;
}

// 0x503950
int sub_503950(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_TILE) {
        return (art_id >> 7) & 1;
    }

    if (tig_art_type(art_id) == TIG_ART_TYPE_FACADE) {
        return sub_503900(art_id);
    }

    return 0;
}

// 0x503990
int sub_503990(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_TILE) {
        return (art_id >> 6) & 1;
    }

    if (tig_art_type(art_id) == TIG_ART_TYPE_FACADE) {
        return sub_503900(art_id);
    }

    return 0;
}

// 0x503A60
int sub_503A60(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_WALL) {
        return (art_id >> 14) & 0x3F;
    }

    return 0;
}

// 0x503A90
unsigned int sub_503A90(unsigned int art_id, int value)
{
    if (tig_art_type(art_id) != TIG_ART_TYPE_WALL) {
        return art_id;
    }

    if (value >= 64) {
        tig_debug_println("Range exceeded in art set.");
        value = 0;
    }

    return (art_id & 0xFFF03FFF) | (value << 14);
}

// 0x503AD0
int sub_503AD0(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_WALL) {
        return (art_id >> 20) & 0xFF;
    }

    return 0;
}

// 0x503B00
int sub_503B00(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_WALL) {
        return (art_id >> 8) & 3;
    }

    return 0;
}

// 0x503B30
unsigned int sub_503B30(unsigned int art_id, int value)
{
    if (tig_art_type(art_id) != TIG_ART_TYPE_WALL) {
        return art_id;
    }

    if (value >= 5) {
        tig_debug_println("Range exceeded in art set.");
        return art_id;
    }

    return (art_id & 0xFFFFFCFF) | (value << 8);
}

// 0x503B70
int sub_503B70(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_WALL:
        return art_id & 0x480;
    case TIG_ART_TYPE_PORTAL:
        return art_id & 0x200;
    case TIG_ART_TYPE_ITEM:
        return art_id & 0x800;
    default:
        return 0;
    }
}

// 0x503BB0
unsigned int sub_503BB0(unsigned int art_id, int value)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_WALL:
        return (art_id & ~0x480) | value;
    case TIG_ART_TYPE_PORTAL:
        return (art_id & ~0x200) | value;
    case TIG_ART_TYPE_ITEM:
        return (art_id & ~0x800) | value;
    default:
        return art_id;
    }
}

// 0x503E20
int sub_503E20(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
    case TIG_ART_TYPE_UNIQUE_NPC:
        return (art_id >> 6) & 0x1F;
    default:
        return 0;
    }
}

// 0x503E50
unsigned int sub_503E50(unsigned int art_id, int value)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
    case TIG_ART_TYPE_UNIQUE_NPC:
        if (value >= 26) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0x7C0) | (value << 6);
    default:
        return art_id;
    }
}

// 0x503EA0
int sub_503EA0(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_CRITTER) {
        return (art_id >> 24) & 7;
    }

    return 0;
}

// 0x503ED0
unsigned int sub_503ED0(unsigned int art_id, int value)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
    case TIG_ART_TYPE_UNIQUE_NPC:
        if (value >= 5) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0x7000000) | (value << 24);
    default:
        return art_id;
    }
}

// 0x503F20
int sub_503F20(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_MONSTER) {
        return (art_id >> 23) & 0x1F;
    }

    return 0;
}

// 0x503F60
int sub_503F60(unsigned int art_id)
{
    int v1;
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
        v1 = sub_502710(art_id);
        return dword_5BE980[v1];
    case TIG_ART_TYPE_MONSTER:
        v1 = sub_503F20(art_id);
        return dword_5BE994[v1];
    default:
        return 1;
    }
}

// 0x503FB0
int sub_503FB0(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_CRITTER) {
        return (art_id >> 27) & 1;
    } else {
        return 0;
    }
}

// 0x503FE0
unsigned int sub_503FE0(unsigned int art_id, int value)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
    case TIG_ART_TYPE_UNIQUE_NPC:
        if (value >= 2) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0x8000000) | (value << 27);
    default:
        return art_id;
    }
}

// 0x504030
int sub_504030(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
        return (art_id >> 20) & 0xF;
    case TIG_ART_TYPE_MONSTER:
        return (art_id >> 20) & 7;
    default:
        return 0;
    }
}

// 0x504060
unsigned int sub_504060(unsigned int art_id, int value)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
        if (value >= 9) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0xF00000) | (value << 20);
    case TIG_ART_TYPE_MONSTER:
        if (value >= 8) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0x700000) | (value << 20);
    default:
        return art_id;
    }
}

// 0x5040D0
int sub_5040D0(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
    case TIG_ART_TYPE_UNIQUE_NPC:
        return art_id & 0xF;
    default:
        return 0;
    }
}

// 0x504100
unsigned int sub_504100(unsigned int art_id, int value)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
    case TIG_ART_TYPE_UNIQUE_NPC:
        if (value >= 15) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0xF) | value;
    default:
        return art_id;
    }
}

// 0x504150
int sub_504150(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
    case TIG_ART_TYPE_UNIQUE_NPC:
        return (art_id >> 19) & 1;
    default:
        return 0;
    }
}

// 0x504180
unsigned int sub_504180(unsigned int art_id, int value)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
    case TIG_ART_TYPE_MONSTER:
    case TIG_ART_TYPE_UNIQUE_NPC:
        if (value >= 2) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0x80000) | (value << 19);
    default:
        return art_id;
    }
}

// 0x504260
int sub_504260(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_PORTAL) {
        return (art_id >> 10) & 1;
    } else {
        return 0;
    }
}

// 0x504300
int sub_504300(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_SCENERY) {
        return (art_id >> 6) & 0x1F;
    } else {
        return 0;
    }
}

// 0x504330
int tig_art_interface_id_create(unsigned int a1, unsigned int a2, unsigned char a3, unsigned int a4, unsigned int* art_id)
{
    if (a1 >= 0x1000 || a2 >= 0x100 || a4 >= 4) {
        return 12;
    }

    // TODO: Incomplete.

    return TIG_OK;
}

// 0x504390
int sub_504390(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_INTERFACE) {
        return (art_id >> 7) & 1;
    } else {
        return 0;
    }
}

// 0x504490
int sub_504490(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ITEM) {
        return (art_id >> 12) & 3;
    } else {
        return 0;
    }
}

// 0x5044C0
unsigned int sub_5044C0(unsigned int art_id, int value)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ITEM) {
        if (value >= 4) {
            value = 0;
        }
        return (art_id & ~0x3000) | (value << 12);
    } else {
        return 0;
    }
}

// 0x5044F0
int sub_5044F0(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ITEM) {
        return (art_id >> 6) & 0x1F;
    } else {
        return 0;
    }
}

// 0x504550
int sub_504550(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ITEM) {
        return art_id & 0xF;
    } else {
        return 0;
    }
}

// 0x504570
int sub_504570(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ITEM) {
        return (art_id >> 14) & 7;
    } else {
        return 0;
    }
}

// 0x5045A0
int sub_5045A0(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ITEM) {
        return art_id & 0x400;
    } else {
        return 0;
    }
}

// 0x5045C0
unsigned int sub_5045C0(unsigned int art_id, int value)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ITEM) {
        // TODO: Check.
        return value != 0
            ? (art_id & ~0x400) | 0x400
            : art_id & ~0x400;
    } else {
        return 0;
    }
}

// 0x504660
int sub_504660(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_CONTAINER) {
        return (art_id >> 6) & 0x1F;
    } else {
        return 0;
    }
}

// 0x504700
int sub_504700(unsigned int art_id)
{
    if (sub_504790(art_id) != 0) {
        return (art_id >> 4) & 0x1F;
    } else {
        return sub_502880(art_id);
    }
}

// 0x504790
int sub_504790(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_LIGHT) {
        return art_id & 1;
    } else {
        return 0;
    }
}

// 0x504840
int sub_504840(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ROOF) {
        int v1 = sub_502B50(art_id);
        if ((sub_504FD0(art_id) & 1) != 0) {
            v1 += 9;
        }
        return v1;
    } else {
        return -1;
    }
}

// 0x5048D0
int sub_5048D0(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ROOF) {
        return (art_id >> 13) & 1;
    } else {
        return 0;
    }
}

// 0x504900
unsigned int sub_504900(unsigned int art_id, unsigned int value)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ROOF) {
        if (value > 1) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0x2000) | (value << 13);
    } else {
        return art_id;
    }
}

// 0x504940
int sub_504940(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ROOF) {
        return (art_id >> 12) & 1;
    } else {
        return 0;
    }
}

// 0x504970
unsigned int sub_504970(unsigned int art_id, unsigned int value)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_ROOF) {
        if (value > 1) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        return (art_id & ~0x1000) | (value << 12);
    } else {
        return art_id;
    }
}

// 0x504A60
int sub_504A60(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_FACADE) {
        int v1 = (art_id >> 17) & 0xFF;
        if ((art_id & 0x8000000) != 0) {
            v1 += 256;
        }
        return v1;
    } else {
        return 0;
    }
}

// 0x504AC0
int sub_504AC0(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_FACADE) {
        return art_id & 1;
    } else {
        return 0;
    }
}

// 0x504B90
int sub_504B90(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_EYE_CANDY) {
        return (art_id >> 6) & 3;
    } else {
        return 0;
    }
}

// 0x504BC0
unsigned int sub_504BC0(unsigned int art_id, int value)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_EYE_CANDY) {
        if (value >= 3) {
            tig_debug_println("Range exceeded in art set.");
            value = 0;
        }
        // NOTE: Rare case - value is additionally ANDed with max value.
        return (art_id & ~0xC0) | ((value & 3) << 6);
    } else {
        return art_id;
    }
}

// 0x504C00
int sub_504C00(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_EYE_CANDY) {
        return (art_id >> 8) & 1;
    } else {
        return 0;
    }
}

// 0x504C60
int sub_504C60(unsigned int art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_EYE_CANDY) {
        return (art_id >> 1) & 7;
    } else {
        // NOTE: Rare case - default is not 0.
        return 4;
    }
}

// 0x504C90
unsigned int sub_504C90(unsigned int art_id, int value)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_EYE_CANDY) {
        return (art_id & ~0xE) | ((value & 7) << 1);
    } else {
        return art_id;
    }
}

// 0x504FD0
int sub_504FD0(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_PORTAL:
    case TIG_ART_TYPE_ROOF:
        return art_id & 0xF;
    default:
        return 0;
    }
}

// 0x51ADE0
int sub_51ADE0(const void* a1, const void* a2)
{
    TigArtCacheEntry* v1 = (TigArtCacheEntry*)a1;
    TigArtCacheEntry* v2 = (TigArtCacheEntry*)a2;
    return v1->field_108 - v2->field_108;
}

// 0x51AE00
int sub_51AE00(const void* a1, const void* a2)
{
    TigArtCacheEntry* v1 = (TigArtCacheEntry*)a1;
    TigArtCacheEntry* v2 = (TigArtCacheEntry*)a2;
    return strcmp(v1->path, v2->path);
}

// 0x51AE50
int tig_art_build_path(unsigned int art_id, char* path)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_8) {
        switch (sub_502710(art_id)) {
        case 0:
            strcpy(path, "art\\mouse.art");
            return 0;
        case 1:
            strcpy(path, "art\\button.art");
            return 0;
        case 2:
            strcpy(path, "art\\up.art");
            return 0;
        case 3:
            strcpy(path, "art\\down.art");
            return 0;
        case 4:
            strcpy(path, "art\\cancel.art");
            return 0;
        case 5:
            strcpy(path, "art\\lens.art");
            return 0;
        case 6:
            strcpy(path, "art\\x.art");
            return 0;
        case 7:
            strcpy(path, "art\\plus.art");
            return 0;
        case 8:
            strcpy(path, "art\\minus.art");
            return 0;
        case 9:
            strcpy(path, "art\\blank.art");
            return 0;
        case 10:
            strcpy(path, "art\\morph15font.art");
            return 0;
        }
    } else {
        if (tig_art_file_path_resolver != NULL) {
            return tig_art_file_path_resolver(art_id, path);
        }
    }

    return 16;
}

// TODO: Check.
//
// 0x51B0E0
bool tig_art_cache_find(const char* path, int* index)
{
    int l = 0;
    int r = tig_art_cache_entries_length - 1;

    while (l <= r) {
        int mid = (l + r) / 2;
        int cmp = strcmp(tig_art_cache_entries[mid].path, path);
        if (cmp == 0) {
            *index = mid;
            return true;
        }

        if (cmp > 0) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }

    *index = 0;
    return false;
}

// 0x51B610
void sub_51B610(int cache_entry_index)
{
    for (int v1 = 0; v1 < 4; v1++) {
        for (int v2 = 0; v2 < 8; v2++) {
            tig_art_cache_entries[cache_entry_index].field_194[v1][v2] = 1;
        }
    }
}

// 0x51B650
void sub_51B650(int cache_entry_index)
{
    int video_buffer_count;
    if (tig_art_type(tig_art_cache_entries[cache_entry_index].art_id) == TIG_ART_TYPE_ROOF) {
        video_buffer_count = 13;
    } else {
        video_buffer_count = tig_art_cache_entries[cache_entry_index].video_buffer_count;
    }

    for (int v1 = 0; v1 < 4; v1++) {
        for (int v2 = 0; v2 < 8; v2++) {
            if (tig_art_cache_entries[cache_entry_index].video_buffers[v1][v2] != NULL) {
                for (int video_buffer_index = 0; video_buffer_index < video_buffer_count; video_buffer_index++) {
                    tig_video_buffer_destroy(tig_art_cache_entries[cache_entry_index].video_buffers[v1][v2][video_buffer_index]);
                }

                free(tig_art_cache_entries[cache_entry_index].video_buffers[v1][v2]);
                tig_art_cache_entries[cache_entry_index].video_buffers[v1][v2] = NULL;
            }
        }
    }
}
