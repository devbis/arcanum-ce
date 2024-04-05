#include "tig/art.h"

#include <stdio.h>

#include "tig/color.h"
#include "tig/debug.h"
#include "tig/file.h"
#include "tig/memory.h"
#include "tig/palette.h"
#include "tig/timer.h"
#include "tig/core.h"
#include "tig/video.h"

#define MAX_PALETTES 4

typedef struct TigArtFileFrameData {
    /* 0000 */ int width;
    /* 0004 */ int height;
    /* 0008 */ int data_size;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
} TigArtFileFrameData;

static_assert(sizeof(TigArtFileFrameData) == 0x1C, "wrong size");

typedef struct TigArtHeader {
    /* 0000 */ int unk_0;
    /* 0004 */ int unk_4;
    /* 0008 */ int field_8;
    /* 000C */ uint32_t* palette_tbl[4];
    /* 001C */ int field_1C;
    /* 0020 */ int num_frames;
    /* 0024 */ TigArtFileFrameData* frames_tbl[8];
    /* 0044 */ int field_44[8];
    /* 0064 */ uint8_t* pixels_tbl[8];
} TigArtHeader;

static_assert(sizeof(TigArtHeader) == 0x84, "wrong size");

typedef struct TigArtCacheEntry {
    /* 0000 */ unsigned int flags;
    /* 0004 */ char path[MAX_PATH];
    /* 0108 */ timer_t time;
    /* 010C */ unsigned int art_id;
    /* 0110 */ TigArtHeader hdr;
    /* 0194 */ uint8_t field_194[4][8];
    /* 01B4 */ TigVideoBuffer** video_buffers[4][8];
    /* 0234 */ uint8_t** pixels_tbl[8];
    /* 0254 */ TigPalette field_254[4];
    /* 0264 */ size_t system_memory_usage;
    /* 0268 */ size_t video_memory_usage;
} TigArtCacheEntry;

static_assert(sizeof(TigArtCacheEntry) == 0x26C, "wrong size");

static int art_get_video_buffer(unsigned int cache_entry_index, tig_art_id_t art_id, TigVideoBuffer** video_buffer_ptr);
static int sub_505940(unsigned int art_blt_flags, unsigned int* vb_blt_flags_ptr);
static int sub_5059F0(int cache_entry_index, TigArtBlitSpec* blt);
static int art_blit(int cache_entry_index, TigArtBlitSpec* blt);
static unsigned int sub_51AA90(tig_art_id_t art_id);
static void sub_51AC20();
static int tig_art_cache_entry_compare_time(const void* a1, const void* a2);
static int tig_art_cache_entry_compare_name(const void* a1, const void* a2);
static int tig_art_build_path(unsigned int art_id, char* path);
static bool tig_art_cache_find(const char* path, int* index);
static bool sub_51B170(tig_art_id_t art_id, const char* path, int index);
static void sub_51B490(int cache_entry_index);
static void sub_51B610(unsigned int cache_entry_index);
static void sub_51B650(int cache_entry_index);
static int sub_51B710(tig_art_id_t art_id, const char* filename, TigArtHeader* hdr, void** palettes, int a5, int* size_ptr);
static int sub_51BE30(TigArtHeader* hdr);
static void sub_51BE50(TigFile* stream, TigArtHeader* hdr, TigPalette* palette_table);
static void sub_51BF20(TigArtHeader* hdr);

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

// 0x5BE900
static int dword_5BE900[32] = {
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
    1,
    1,
    1,
    0,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
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

// 0x5BEA14
static int dword_5BEA14 = 1;

// 0x5BEA38
float flt_5BEA38 = 0.3f;

// 0x604710
static TigArtFilePathResolver* tig_art_file_path_resolver;

// 0x604714
static unsigned int dword_604714;

// 0x604718
static bool dword_604718;

// 0x60471C
static unsigned int tig_art_cache_entries_length;

// 0x604720
static TigArtCacheEntry* tig_art_cache_entries;

// 0x604724
static size_t tig_art_total_system_memory;

// 0x60472C
static unsigned int tig_art_cache_entries_capacity;

// 0x604730
static size_t tig_art_available_video_memory;

// 0x604744
static TigArtBlitPaletteAdjustCallback* dword_604744;

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

// 0x604754
static int dword_604754;

// 0x500590
int tig_art_init(TigContext* ctx)
{
    size_t total_memory;
    size_t available_memory;

    if (tig_art_initialized) {
        return TIG_ALREADY_INITIALIZED;
    }

    tig_art_cache_entries_capacity = 512;
    tig_art_cache_entries_length = 0;
    tig_art_cache_entries = (TigArtCacheEntry*)MALLOC(sizeof(TigArtCacheEntry) * tig_art_cache_entries_capacity);

    tig_memory_get_system_status(&total_memory, &available_memory);
    tig_art_available_system_memory = total_memory >> 2;
    tig_art_total_system_memory = total_memory >> 2;

    if (tig_video_get_video_memory_status(&total_memory, &available_memory) != TIG_OK) {
        available_memory = tig_art_total_system_memory;
    }

    tig_art_available_video_memory = available_memory / 2;
    tig_art_total_video_memory = tig_art_available_video_memory;
    tig_debug_printf("Art mem vid avail is %d\n", tig_art_available_video_memory);

    tig_art_bits_per_pixel = ctx->bpp;
    tig_art_bytes_per_pixel = tig_art_bits_per_pixel / 8;
    tig_art_file_path_resolver = ctx->art_file_path_resolver;
    dword_604750 = ctx->field_20;

    tig_art_initialized = true;

    dword_604718 = tig_video_3d_check_initialized() == TIG_OK;

    return TIG_OK;
}

// 0x500690
void tig_art_exit()
{
    if (tig_art_initialized) {
        tig_art_flush();

        if (tig_art_cache_entries != NULL) {
            FREE(tig_art_cache_entries);
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

// 0x5006E0
int sub_5006E0(tig_art_id_t art_id, TigPalette palette)
{
    TigArtHeader hdr;
    int size;
    TigPalette palette_tbl[4];
    char path[_MAX_PATH];
    int rc;

    rc = tig_art_build_path(art_id, path);
    if (rc != TIG_OK) {
        return rc;
    }

    palette_tbl[0] = palette;
    return sub_51B710(art_id,
        path,
        &hdr,
        palette_tbl,
        1,
        &size);
}

// 0x501DD0
int tig_art_misc_id_create(unsigned int a1, unsigned int palette, unsigned int* art_id)
{
    if (a1 >= 512 || palette >= 4) {
        return TIG_ERR_12;
    }

    // TODO: Check.
    *art_id = (TIG_ART_TYPE_MISC << 28) | ((a1 & 0x1FF) << 19) | ((palette & 3) << 4);

    return TIG_OK;
}

// 0x501E10
int sub_501E10(tig_art_id_t art_id, int a2)
{
    int index;

    index = sub_51AA90(art_id);
    if (index == -1) {
        return TIG_ERR_16;
    }

    tig_art_cache_entries[index].hdr.unk_4 = a2;
    tig_art_cache_entries[index].flags |= 0x2;

    return TIG_OK;
}

// 0x501E60
int sub_501E60(tig_art_id_t art_id, short a2)
{
    int index;

    index = sub_51AA90(art_id);
    if (index == -1) {
        return TIG_ERR_16;
    }

    tig_art_cache_entries[index].hdr.field_1C = a2;
    tig_art_cache_entries[index].flags |= 0x02;

    return TIG_OK;
}

// 0x501EB0
int sub_501EB0(tig_art_id_t art_id, const char* filename)
{
    int index;
    FILE* stream;

    index = sub_51AA90(art_id);
    if (index == -1) {
        return TIG_ERR_16;
    }

    if ((tig_art_cache_entries[index].flags & 0x02) == 0) {
        return TIG_ERR_16;
    }

    tig_art_cache_entries[index].flags &= 0x02;

    stream = fopen(filename, "r+b");
    if (stream == NULL) {
        return TIG_ERR_13;
    }

    rewind(stream);

    if (fwrite(&(tig_art_cache_entries[index].hdr), sizeof(TigArtHeader), 1, stream) != 1) {
        fclose(stream);
        return TIG_ERR_13;
    }

    fclose(stream);
    return TIG_OK;
}

// 0x501F60
int sub_501F60(const char* filename, uint32_t* new_palette_entries, int new_palette_index)
{
    FILE* in;
    FILE* out;
    TigArtHeader hdr;
    char path[_MAX_PATH];
    uint32_t palette_entries[256];
    uint8_t buffer[10000];
    uint32_t* palette_table[4];
    int index;
    size_t bytes_read;
    size_t bytes_written;

    if (tmpnam(path) == NULL) {
        return TIG_ERR_13;
    }

    in = fopen(filename, "rb");
    if (in == NULL) {
        return TIG_ERR_13;
    }

    out = fopen(path, "wb");
    if (out == NULL) {
        fclose(in);
        return TIG_ERR_13;
    }

    if (fread(&hdr, sizeof(hdr), 1, in) != 1) {
        fclose(in);
        fclose(out);
        return TIG_ERR_13;
    }

    palette_table[0] = hdr.palette_tbl[0];
    palette_table[1] = hdr.palette_tbl[1];
    palette_table[2] = hdr.palette_tbl[2];
    palette_table[3] = hdr.palette_tbl[3];
    hdr.palette_tbl[new_palette_index] = new_palette_entries;

    if (fwrite(&hdr, sizeof(hdr), 1, out) != 1) {
        fclose(in);
        fclose(out);
        return TIG_ERR_13;
    }

    for (index = 0; index < 4; index++) {
        if (palette_table[index] != 0) {
            if (fread(palette_entries, sizeof(*palette_entries), 256, in) != 256) {
                fclose(in);
                fclose(out);
                return TIG_ERR_13;
            }
        }

        if (new_palette_index == index) {
            if (fwrite(new_palette_entries, sizeof(*new_palette_entries), 256, out) != 256) {
                fclose(in);
                fclose(out);
                return TIG_ERR_13;
            }
        }

        if (palette_table[index] != 0) {
            if (fwrite(palette_entries, sizeof(*palette_entries), 256, out) != 256) {
                fclose(in);
                fclose(out);
                return TIG_ERR_13;
            }
        }
    }

    for (;;) {
        bytes_read = fread(buffer, 1, sizeof(buffer), in);
        if (bytes_read == 0) {
            break;
        }

        bytes_written = fwrite(buffer, 1, bytes_read, out);
        if (bytes_read != bytes_written) {
            fclose(in);
            fclose(out);
            return TIG_ERR_13;
        }
    }

    fclose(in);
    fclose(out);
    if (!CopyFileA(path, filename, FALSE)) {
        return TIG_ERR_16;
    }

    remove(path);
    return TIG_OK;
}

// 0x5021E0
void tig_art_flush()
{
    unsigned int index;

    if (!tig_art_initialized) {
        return;
    }

    for (index = 0; index < tig_art_cache_entries_length; index++) {
        sub_51B490(index);
    }

    tig_art_cache_entries_length = 0;
}

// 0x502220
int tig_art_exists(tig_art_id_t art_id)
{
    char path[MAX_PATH];
    if (tig_art_build_path(art_id, path) != TIG_OK) {
        return TIG_ERR_16;
    }

    if (!tig_file_exists(path, NULL)) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x502270
int sub_502270(tig_art_id_t art_id, char* path)
{
    return tig_art_build_path(art_id, path);
}

// 0x502290
int sub_502290(tig_art_id_t art_id)
{
    int index;

    index = sub_51AA90(art_id);
    if (index == -1) {
        return TIG_ERR_13;
    }

    return TIG_OK;
}

// 0x5022B0
void sub_5022B0(TigArtBlitPaletteAdjustCallback* callback)
{
    dword_604744 = callback;
}

// 0x5022C0
TigArtBlitPaletteAdjustCallback* sub_5022C0()
{
    return dword_604744;
}

// 0x5022D0
void sub_5022D0()
{
    unsigned int cache_entry_index;
    unsigned int palette;

    for (cache_entry_index = 0; cache_entry_index < tig_art_cache_entries_length; cache_entry_index++) {
        for (palette = 0; palette < 4; palette++) {
            if (tig_art_cache_entries[cache_entry_index].hdr.palette_tbl[palette] != NULL) {
                sub_505000(tig_art_cache_entries[cache_entry_index].art_id,
                    tig_art_cache_entries[cache_entry_index].hdr.palette_tbl[palette],
                    tig_art_cache_entries[cache_entry_index].field_254[palette]);
            }
        }
        sub_51B610(cache_entry_index);
    }
}

// 0x502360
int tig_art_blit(TigArtBlitSpec* blit_spec)
{
    TigArtBlitSpec mut_art_blt;
    TigVideoBuffer* video_buffer;
    TigVideoBufferData video_buffer_data;
    TigVideoBufferBlitSpec video_buffer_blt;
    int rc;
    unsigned int cache_entry_index;
    unsigned int type;

    mut_art_blt = *blit_spec;

    cache_entry_index = sub_51AA90(mut_art_blt.art_id);
    if (cache_entry_index == -1) {
        return TIG_ERR_13;
    }

    type = tig_art_type(mut_art_blt.art_id);
    if (type == TIG_ART_TYPE_TILE) {
        if (sub_503900(mut_art_blt.art_id)) {
            unsigned int v1 = sub_504FD0(mut_art_blt.art_id);
            if ((v1 & 0x1) != 0) {
                if ((mut_art_blt.flags & 0x1) != 0) {
                    mut_art_blt.flags &= ~0x1;
                } else {
                    mut_art_blt.flags |= 0x1;
                }
            }
            mut_art_blt.art_id = sub_502D30(mut_art_blt.art_id, v1 & ~0x1);
        }
    } else {
        if (dword_5BEA14 &&
            (type == TIG_ART_TYPE_CRITTER
                || type == TIG_ART_TYPE_MONSTER
                || type == TIG_ART_TYPE_UNIQUE_NPC)) {
            int rotation = tig_art_id_rotation_get(mut_art_blt.art_id);
            if (rotation > 0 && rotation < 4) {
                mut_art_blt.art_id = tig_art_id_rotation_set(mut_art_blt.art_id, 8 - rotation);
                if ((mut_art_blt.flags & 0x1) != 0) {
                    mut_art_blt.flags &= ~0x1;
                } else {
                    mut_art_blt.flags |= 0x1;
                }
            }
        }
    }

    if (sub_505940(mut_art_blt.flags, &(video_buffer_blt.flags)) == TIG_OK
        && sub_520FB0(mut_art_blt.dst_video_buffer, video_buffer_blt.flags) == TIG_OK
        && art_get_video_buffer(cache_entry_index, mut_art_blt.art_id, &video_buffer) == TIG_OK) {
        if ((mut_art_blt.flags & 0x2000) != 0) {
            video_buffer_blt.field_10 = mut_art_blt.field_10;
        } else if ((mut_art_blt.flags & 0x20000) != 0) {
            video_buffer_blt.field_10 = mut_art_blt.field_14[0];
            video_buffer_blt.field_14 = mut_art_blt.field_14[1];
            video_buffer_blt.field_18 = mut_art_blt.field_14[2];
            video_buffer_blt.field_1C = mut_art_blt.field_14[3];
            video_buffer_blt.field_20 = mut_art_blt.field_18;
        } else if ((mut_art_blt.flags & 0x4000) != 0) {
            video_buffer_blt.field_10 = mut_art_blt.field_14[0];
            video_buffer_blt.field_14 = mut_art_blt.field_14[1];
            video_buffer_blt.field_18 = mut_art_blt.field_14[1];
            video_buffer_blt.field_1C = mut_art_blt.field_14[0];
            video_buffer_blt.field_20 = 0;
        }

        if ((mut_art_blt.flags & 0x100) != 0) {
            video_buffer_blt.field_C = mut_art_blt.field_1C & 0xFF;
        } else if ((mut_art_blt.flags & 0x400) != 0) {
            video_buffer_blt.field_C = mut_art_blt.field_1C & 0xFF;
            video_buffer_blt.field_D = mut_art_blt.field_1C & 0xFF;
            video_buffer_blt.field_E = (mut_art_blt.field_1C >> 8) & 0xFF;
            video_buffer_blt.field_F = (mut_art_blt.field_1C >> 8) & 0xFF;
        } else if ((mut_art_blt.flags & 0x800) != 0) {
            video_buffer_blt.field_C = mut_art_blt.field_1C & 0xFF;
            video_buffer_blt.field_D = mut_art_blt.field_1C & 0xFF;
            video_buffer_blt.field_E = (mut_art_blt.field_1C >> 24) & 0xFF;
            video_buffer_blt.field_F = (mut_art_blt.field_1C >> 24) & 0xFF;
        } else if ((mut_art_blt.flags & 0x1000) != 0) {
            video_buffer_blt.field_C = mut_art_blt.field_1C & 0xFF;
            video_buffer_blt.field_D = (mut_art_blt.field_1C >> 8) & 0xFF;
            video_buffer_blt.field_E = (mut_art_blt.field_1C >> 16) & 0xFF;
            video_buffer_blt.field_F = (mut_art_blt.field_1C >> 24) & 0xFF;
        }

        video_buffer_blt.src_rect = mut_art_blt.src_rect;
        video_buffer_blt.src_video_buffer = video_buffer;
        video_buffer_blt.dst_rect = mut_art_blt.dst_rect;
        video_buffer_blt.dst_video_buffer = mut_art_blt.dst_video_buffer;
        return tig_video_buffer_blit(&video_buffer_blt);
    }

    if ((mut_art_blt.flags & 0x3FFF0) != 0) {
        tig_video_buffer_data(mut_art_blt.dst_video_buffer, &video_buffer_data);

        if ((video_buffer_data.flags & 0x04) != 0
            && (mut_art_blt.flags & 0x1000000) != 0) {
            rc = tig_video_buffer_data(mut_art_blt.scratch_video_buffer, &video_buffer_data);
            if (rc != TIG_OK) {
                return rc;
            }

            if ((mut_art_blt.flags & 0x19FF0) != 0) {
                if ((video_buffer_data.flags & 0x8) == 0) {
                    tig_debug_printf("Warning: using video memory surface as a scratch buffer.  This will cause slower performance.\n");
                }

                video_buffer_blt.flags = 0;
                video_buffer_blt.src_rect = mut_art_blt.dst_rect;
                video_buffer_blt.dst_rect = mut_art_blt.dst_rect;
                video_buffer_blt.src_video_buffer = mut_art_blt.dst_video_buffer;
                video_buffer_blt.dst_video_buffer = mut_art_blt.scratch_video_buffer;
                rc = tig_video_buffer_blit(&video_buffer_blt);
                if (rc != TIG_OK) {
                    return rc;
                }
            } else {
                tig_video_buffer_fill(mut_art_blt.scratch_video_buffer,
                    mut_art_blt.dst_rect,
                    video_buffer_data.color_key);
            }

            video_buffer = mut_art_blt.dst_video_buffer;
            mut_art_blt.dst_video_buffer = mut_art_blt.scratch_video_buffer;

            rc = art_blit(cache_entry_index, &mut_art_blt);
            if (rc != TIG_OK) {
                return rc;
            }

            mut_art_blt.dst_video_buffer = video_buffer;

            video_buffer_blt.flags = 0;
            video_buffer_blt.src_rect = mut_art_blt.dst_rect;
            video_buffer_blt.src_video_buffer = mut_art_blt.scratch_video_buffer;
            video_buffer_blt.dst_rect = mut_art_blt.dst_rect;
            video_buffer_blt.dst_video_buffer = mut_art_blt.dst_video_buffer;
            return tig_video_buffer_blit(&video_buffer_blt);
        }
    }

    return art_blit(cache_entry_index, &mut_art_blt);
}

// 0x502700
int tig_art_type(unsigned int art_id)
{
    return art_id >> 28;
}

// 0x502710
int tig_art_num(unsigned int art_id)
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
int tig_art_id_rotation_get(tig_art_id_t art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_INTERFACE:
    case TIG_ART_TYPE_MISC:
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
tig_art_id_t tig_art_id_rotation_inc(tig_art_id_t art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_INTERFACE:
    case TIG_ART_TYPE_MISC:
    case TIG_ART_TYPE_ROOF:
    case TIG_ART_TYPE_ITEM:
    case TIG_ART_TYPE_FACADE:
        return art_id;
    }

    return tig_art_id_rotation_set(art_id, tig_art_id_rotation_get(art_id) + 1);
}

// 0x502930
tig_art_id_t tig_art_id_rotation_set(tig_art_id_t art_id, int rotation)
{
    // TODO: Incomplete.
    (void)rotation;

    return art_id;
}

// 0x502B50
int tig_art_id_frame_get(unsigned int art_id)
{
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_ITEM:
        return 0;
    case TIG_ART_TYPE_INTERFACE:
    case TIG_ART_TYPE_MISC:
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
unsigned int tig_art_id_frame_inc(unsigned int art_id)
{
    int frame;

    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_ITEM:
        return art_id;
    }

    frame = tig_art_id_frame_get(art_id);
    return tig_art_id_frame_set(art_id, frame + 1);
}

// 0x502C00
unsigned int tig_art_id_frame_dec(unsigned int art_id)
{
    int frame;

    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_TILE:
    case TIG_ART_TYPE_WALL:
    case TIG_ART_TYPE_ITEM:
        return art_id;
    }

    frame = tig_art_id_frame_get(art_id);
    if (frame > 0) {
        --frame;
    }

    return tig_art_id_frame_set(art_id, frame);
}

// 0x502C40
tig_art_id_t tig_art_id_frame_set(tig_art_id_t art_id, int value)
{
    TigArtData art_data;
    int type;

    type = tig_art_type(art_id);

    if (type == TIG_ART_TYPE_TILE
        || type == TIG_ART_TYPE_WALL
        || type == TIG_ART_TYPE_ITEM) {
        return art_id;
    }

    if (value > 0) {
        if (tig_art_data(art_id, &art_data) != TIG_OK) {
            return art_id;
        }

        if (value >= art_data.frames) {
            value = 0;
        }
    } else if (value < 0) {
        if (tig_art_data(art_id, &art_data) != TIG_OK) {
            return art_id;
        }

        value = art_data.frames - 1;
    }

    switch (type) {
    case TIG_ART_TYPE_INTERFACE:
    case TIG_ART_TYPE_MISC:
        return (art_id & ~0xFF00) | (value << 8);
    case TIG_ART_TYPE_FACADE:
        return (art_id & ~0x7FE) | (value << 1);
    case TIG_ART_TYPE_LIGHT:
    case TIG_ART_TYPE_EYE_CANDY:
        return (art_id & ~0x7F000) | (value << 12);
    }

    return (art_id & ~0x7C000) | (value << 14);
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

// 0x502E00
int sub_502E00(tig_art_id_t art_id)
{
    int cache_index;
    int palette;

    cache_index = sub_51AA90(art_id);
    if (cache_index == -1) {
        return TIG_ERR_13;
    }

    palette = tig_art_palette(art_id);
    if (tig_art_cache_entries[cache_index].hdr.palette_tbl[palette] == 0) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x502E50
int sub_502E50(tig_art_id_t art_id, int x, int y, unsigned int* color_ptr)
{
    TigArtCacheEntry* cache_entry;
    int cache_index;
    int rotation;
    int frame;
    int type;
    int palette;
    uint8_t byte;

    cache_index = sub_51AA90(art_id);
    if (cache_index == -1) {
        return TIG_ERR_13;
    }

    cache_entry = &(tig_art_cache_entries[cache_index]);

    rotation = tig_art_id_rotation_get(art_id);
    frame = tig_art_id_frame_get(art_id);

    if (dword_5BEA14) {
        type = tig_art_type(art_id);
        if ((type == TIG_ART_TYPE_CRITTER
                || type == TIG_ART_TYPE_MONSTER
                || type == TIG_ART_TYPE_UNIQUE_NPC)
            && rotation > 0
            && rotation < 4) {
            rotation = 8 - rotation;
            x = cache_entry->hdr.frames_tbl[rotation][frame].width - x - 1;
        }
    }

    byte = cache_entry->pixels_tbl[rotation][frame][y * cache_entry->hdr.frames_tbl[rotation][frame].width + x];

    palette = tig_art_palette(art_id);
    if (cache_entry->hdr.palette_tbl[palette] == 0) {

    }

    if (tig_art_bits_per_pixel != 8) {
        switch (tig_art_bits_per_pixel) {
        case 16:
            *color_ptr = *((uint16_t*)cache_entry->hdr.palette_tbl[palette] + byte);
            break;
        case 24:
            *color_ptr = *((uint32_t*)cache_entry->hdr.palette_tbl[palette] + byte);
            break;
        case 32:
            *color_ptr = *((uint32_t*)cache_entry->hdr.palette_tbl[palette] + byte);
            break;
        }
    }

    return TIG_OK;
}

// 0x5030B0
int tig_art_data(unsigned int art_id, TigArtData* data)
{
    unsigned int cache_index;
    TigArtCacheEntry* cache_entry;
    int palette;

    cache_index = sub_51AA90(art_id);
    if (cache_index == -1) {
        return TIG_ERR_16;
    }

    cache_entry = &(tig_art_cache_entries[cache_index]);

    data->unk_0 = cache_entry->hdr.unk_0;
    data->unk_4 = cache_entry->hdr.unk_4;
    data->bpp = tig_art_bits_per_pixel;
    data->frames = cache_entry->hdr.num_frames;
    data->field_C = cache_entry->hdr.field_1C;

    palette = tig_art_palette(art_id);

    data->palette1 = cache_entry->field_254[palette];
    if (data->palette1 == NULL) {
        data->palette1 = cache_entry->field_254[0];
    }

    data->palette2 = cache_entry->hdr.palette_tbl[palette];
    if (data->palette2 == 0) {
        data->palette2 = cache_entry->hdr.palette_tbl[0];
    }

    switch (tig_art_bits_per_pixel) {
    case 8:
        data->color_key = ((uint8_t*)data->palette1)[0];
        break;
    case 16:
        data->color_key = ((uint16_t*)data->palette1)[0];
        break;
    case 24:
        data->color_key = ((uint32_t*)data->palette1)[0];
        break;
    case 32:
        data->color_key = ((uint32_t*)data->palette1)[0];
        break;
    }

    return TIG_OK;
}

// 0x5031C0
int tig_art_frame_data(unsigned int art_id, TigArtFrameData* data)
{
    unsigned int cache_index;
    TigArtCacheEntry* cache_entry;
    int rotation;
    int frame;
    bool mirrored;
    int type;

    cache_index = sub_51AA90(art_id);
    if (cache_index == -1) {
        return TIG_ERR_16;
    }

    rotation = tig_art_id_rotation_get(art_id);
    frame = tig_art_id_frame_get(art_id);
    type = tig_art_type(art_id);

    mirrored = false;
    if (dword_5BEA14
        && (type == TIG_ART_TYPE_CRITTER
            || type == TIG_ART_TYPE_MONSTER
            || type == TIG_ART_TYPE_UNIQUE_NPC)
        && rotation > 0
        && rotation < 4) {
        mirrored = true;
        rotation = 8 - rotation;
    }

    cache_entry = &(tig_art_cache_entries[cache_index]);

    data->width = cache_entry->hdr.frames_tbl[rotation][frame].width;
    data->height = cache_entry->hdr.frames_tbl[rotation][frame].height;
    data->field_8 = cache_entry->hdr.frames_tbl[rotation][frame].field_C;
    data->field_C = cache_entry->hdr.frames_tbl[rotation][frame].field_10;
    data->field_10 = cache_entry->hdr.frames_tbl[rotation][frame].field_14;
    data->field_14 = cache_entry->hdr.frames_tbl[rotation][frame].field_18;

    if ((type == TIG_ART_TYPE_WALL || type == TIG_ART_TYPE_PORTAL)
        && (rotation < 2 || rotation > 5)) {
        data->field_8 -= 40;
        data->field_C += 20;
    }

    if (mirrored) {
        data->field_8 = data->width - data->field_8 - 1;
        data->field_10 = -data->field_10;
    }

    if ((sub_504FD0(art_id) & 0x1) != 0) {
        if (type == TIG_ART_TYPE_ROOF) {
            data->field_8 = 0;
            data->field_10 = 0;
        } else {
            data->field_8 = cache_entry->hdr.frames_tbl[rotation][frame].width - data->field_8 - 2;
            data->field_10 = -data->field_10;
        }
    }

    return TIG_OK;
}

// 0x503340
int sub_503340(tig_art_id_t art_id, uint8_t* dst, int pitch)
{
    unsigned int cache_index;
    int rotation;
    int v2;
    uint8_t* src;
    int width;
    int height;

    cache_index = sub_51AA90(art_id);
    if (cache_index == -1) {
        return TIG_ERR_13;
    }

    rotation = tig_art_id_rotation_get(art_id);
    v2 = tig_art_id_frame_get(art_id);
    src = tig_art_cache_entries[cache_index].pixels_tbl[rotation][v2];
    width = tig_art_cache_entries[cache_index].hdr.frames_tbl[rotation][v2].width;
    height = tig_art_cache_entries[cache_index].hdr.frames_tbl[rotation][v2].height;

    while (height > 0) {
        memcpy(dst, src, width);
        dst += pitch;
        height--;
    }

    return TIG_OK;
}

// 0x503510
int tig_art_size(tig_art_id_t art_id, int* width_ptr, int* height_ptr)
{
    TigArtCacheEntry* art;
    TigArtFileFrameData* frm;
    unsigned int cache_entry_index;
    int type;
    int rotation_offset;
    int num_rotations;
    int rotation;
    int frame;
    int width = 0;
    int height = 0;

    cache_entry_index = sub_51AA90(art_id);
    if (cache_entry_index == -1) {
        return TIG_ERR_16;
    }

    art = &(tig_art_cache_entries[cache_entry_index]);
    type = tig_art_type(art_id);

    if ((art->hdr.unk_0 & 1) != 0) {
        rotation_offset = 0;
        num_rotations = 1;
    } else if (dword_5BEA14 &&
        (type == TIG_ART_TYPE_CRITTER
            || type == TIG_ART_TYPE_MONSTER
            || type == TIG_ART_TYPE_UNIQUE_NPC)) {
        rotation_offset = 4;
        num_rotations = 5;
    } else {
        rotation_offset = 0;
        num_rotations = 8;
    }

    for (frame = 0; frame < art->hdr.num_frames; ++frame) {
        for (rotation = 0; rotation < num_rotations; ++rotation) {
            frm = &(art->hdr.frames_tbl[(rotation + rotation_offset) % 8][frame]);
            if (width < frm->width) {
                width = frm->width;
            }
            if (height < frm->height) {
                height = frm->height;
            }
        }
    }

    *width_ptr = width;
    *height_ptr = height;

    return TIG_OK;
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

// 0x503F50
int sub_503F50(int a1)
{
    return dword_5BE900[a1];
}

// 0x503F60
int sub_503F60(unsigned int art_id)
{
    int v1;
    switch (tig_art_type(art_id)) {
    case TIG_ART_TYPE_CRITTER:
        v1 = tig_art_num(art_id);
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
int tig_art_interface_id_create(unsigned int num, unsigned int a2, unsigned char a3, unsigned int palette, unsigned int* art_id)
{
    if (num >= 0x1000) {
        return TIG_ERR_12;
    }

    if (a2 >= 0x100) {
        return TIG_ERR_12;
    }

    if (palette >= 4) {
        return TIG_ERR_12;
    }

    *art_id = (TIG_ART_TYPE_INTERFACE << 28) | ((num & 0xFFF) << 16) | ((a2 & 0xFF) << 8) | ((a3 & 1) << 7) | ((palette & 3) << 4);

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

// 0x504520
tig_art_id_t sub_504520(tig_art_id_t art_id, int value)
{
    if (tig_art_type(art_id) != TIG_ART_TYPE_ITEM) {
        return art_id;
    }

    if (value >= 16) {
        return art_id;
    }

    return (art_id & ~0x3C0) | (value << 6);
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
    if (tig_art_type(art_id) != TIG_ART_TYPE_ITEM) {
        return art_id;
    }

    if (value) {
        art_id |= 0x400;
    } else {
        art_id &= ~0x400;
    }

    return art_id;
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

// 0x504690
int tig_art_light_id_create(unsigned int a1, unsigned int a2, unsigned int a3, int a4, tig_art_id_t* art_id)
{
    if (a1 >= 512) {
        return TIG_ERR_12;
    }

    if (a2 >= 128) {
        return TIG_ERR_12;
    }

    if (a4 == 0) {
        if (a3 >= 8) {
            return TIG_ERR_12;
        }
    } else {
        if (a3 >= 32) {
            return TIG_ERR_12;
        }
    }

    // TODO: Check.
    *art_id = (TIG_ART_TYPE_LIGHT << 28) | ((a1 & 0x1FF) << 19) | ((a2 & 0x7F) << 12) | ((a3 & 7) << 9) | ((a3 & 0x1F) << 4) | (a4 & 1);

    return TIG_OK;
}

// 0x504700
int sub_504700(unsigned int art_id)
{
    if (sub_504790(art_id) != 0) {
        return (art_id >> 4) & 0x1F;
    } else {
        return tig_art_id_rotation_get(art_id);
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
        int v1 = tig_art_id_frame_get(art_id);
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

// 0x504AE0
int tig_art_eye_candy_id_create(unsigned int a1, unsigned int a2, int a3, int translucency, int type, unsigned int palette, int scale, tig_art_id_t* art_id)
{
    if (a1 >= 512) {
        return TIG_ERR_12;
    }

    if (a2 >= 128) {
        return TIG_ERR_12;
    }

    if (a3 >= 8) {
        return TIG_ERR_12;
    }

    if (translucency >= 2) {
        return TIG_ERR_12;
    }

    if (type >= 4) {
        return TIG_ERR_12;
    }

    if (palette >= 4) {
        return TIG_ERR_12;
    }

    if (scale < 0 || scale > 7) {
        return TIG_ERR_12;
    }

    // TODO: Check.
    *art_id = (TIG_ART_TYPE_EYE_CANDY << 28) | ((a1 & 0x1FF) << 19) | ((a2 & 0x7F) << 12) | ((a3 & 7) << 9) | ((translucency & 1) << 8) | ((type & 3) << 6) | ((palette & 3) << 4) | ((scale & 7) << 1);

    return TIG_OK;
}

// 0x504B90
int tig_art_eye_candy_id_type_get(tig_art_id_t art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_EYE_CANDY) {
        return (art_id >> 6) & 3;
    } else {
        return 0;
    }
}

// 0x504BC0
tig_art_id_t tig_art_eye_candy_id_type_set(tig_art_id_t art_id, int value)
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
int tig_art_eye_candy_id_translucency_get(tig_art_id_t art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_EYE_CANDY) {
        return (art_id >> 8) & 1;
    } else {
        return 0;
    }
}

// 0x504C30
tig_art_id_t tig_art_eye_candy_id_translucency_set(tig_art_id_t art_id, int value)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_EYE_CANDY) {
        return (art_id & ~0x100) | ((value & 1) << 8);
    } else {
        return art_id;
    }
}

// 0x504C60
int tig_art_eye_candy_id_scale_get(tig_art_id_t art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_EYE_CANDY) {
        return (art_id >> 1) & 7;
    } else {
        // NOTE: Rare case - default is not 0.
        return 4;
    }
}

// 0x504C90
tig_art_id_t tig_art_eye_candy_id_scale_set(tig_art_id_t art_id, int value)
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

// 0x505000
void sub_505000(tig_art_id_t art_id, TigPalette src_palette, TigPalette dst_palette)
{
    TigPaletteAdjustDesc palette_adjust_desc;

    if (dword_604744 != NULL && dword_604744(art_id, &palette_adjust_desc)) {
        palette_adjust_desc.src_palette = src_palette;
        palette_adjust_desc.dst_palette = dst_palette;
        tig_palette_adjust(&palette_adjust_desc);
    } else {
        tig_palette_copy(dst_palette, src_palette);
    }
}

// 0x505060
int art_get_video_buffer(unsigned int cache_entry_index, tig_art_id_t art_id, TigVideoBuffer** video_buffer_ptr)
{
    TigVideoBufferSpec desc;
    TigArtBlitSpec blt;
    TigRect rect;
    int type;
    int palette;
    int rotation;
    int frame;
    int rc;
    size_t system_memory_size = 0;
    size_t video_memory_size = 0;

    if (sub_51F860() != TIG_OK) {
        return TIG_ERR_16;
    }

    type = tig_art_type(art_id);
    switch (type) {
    case TIG_ART_TYPE_ITEM:
        if (sub_504490(art_id) != 0) {
            return TIG_ERR_16;
        }
        break;
    case TIG_ART_TYPE_INTERFACE:
        if (sub_504390(art_id) == 0) {
            return TIG_ERR_16;
        }
        break;
    case TIG_ART_TYPE_MISC:
        return TIG_ERR_16;
    case TIG_ART_TYPE_LIGHT:
        if (sub_504790(art_id) == 0) {
            return TIG_ERR_16;
        }
        break;
    case TIG_ART_TYPE_ROOF:
        // TODO: Incomplete.
        break;
    case TIG_ART_TYPE_TILE:
        art_id = sub_502D30(art_id, sub_504FD0(art_id) & ~1);
        break;
    }

    rotation = tig_art_id_rotation_get(art_id);
    if (dword_5BEA14
        && (type == TIG_ART_TYPE_CRITTER
            || type == TIG_ART_TYPE_MONSTER
            || type == TIG_ART_TYPE_UNIQUE_NPC)
        && rotation > 0 && rotation < 4) {
        tig_debug_printf("Error in art_get_video_buffer() - requested rotation %d for mirrored object\n", rotation);
    }

    if ((type == TIG_ART_TYPE_WALL
            || type == TIG_ART_TYPE_PORTAL)
        && (rotation & 1) != 0) {
        art_id = tig_art_id_rotation_set(art_id, rotation - 1);
    }

    palette = tig_art_palette(art_id);

    if (tig_art_cache_entries[cache_entry_index].field_254[palette] == NULL) {
        palette = 0;
        art_id = tig_art_set_palette(art_id, 0);
    }

    if (tig_art_cache_entries[cache_entry_index].video_buffers[palette][rotation] == NULL) {
        system_memory_size = sizeof(TigVideoBuffer*) * tig_art_cache_entries[cache_entry_index].hdr.num_frames;
        tig_art_cache_entries[cache_entry_index].video_buffers[palette][rotation] = (TigVideoBuffer**)MALLOC(system_memory_size);

        desc.color_key = tig_color_rgb_make(0, 0, 0);
        desc.background_color = desc.color_key;

        if (dword_604718) {
            desc.flags = TIG_VIDEO_BUFFER_SPEC_BACKGROUND_COLOR_ENABLED;
            desc.color_key = 0;
        } else {
            desc.flags = TIG_VIDEO_BUFFER_SPEC_TRANSPARENCY_ENABLED | TIG_VIDEO_BUFFER_SPEC_VIDEO_MEMORY;
        }

        for (frame = 0; frame < tig_art_cache_entries[cache_entry_index].hdr.num_frames; frame++) {
            desc.width = tig_art_cache_entries[cache_entry_index].hdr.frames_tbl[rotation][frame].width;
            desc.height = tig_art_cache_entries[cache_entry_index].hdr.frames_tbl[rotation][frame].height;
            video_memory_size += desc.width * desc.height * tig_art_bytes_per_pixel;
            rc = tig_video_buffer_create(&desc, &(tig_art_cache_entries[cache_entry_index].video_buffers[palette][rotation][frame]));
            if (rc != TIG_OK) {
                while (frame > 0) {
                    tig_video_buffer_destroy(tig_art_cache_entries[cache_entry_index].video_buffers[palette][rotation][frame - 1]);
                    frame--;
                }

                FREE(tig_art_cache_entries[cache_entry_index].video_buffers[palette][rotation]);
                tig_art_cache_entries[cache_entry_index].video_buffers[palette][rotation] = NULL;

                return rc;
            }
        }

        tig_art_cache_entries[cache_entry_index].field_194[palette][rotation] = 1;
    }

    if (tig_art_cache_entries[cache_entry_index].field_194[palette][rotation] == 1) {
        rect.x = 0;
        rect.y = 0;

        blt.flags = 0;
        blt.src_rect = &rect;
        blt.dst_rect = &rect;

        for (frame = 0; frame < tig_art_cache_entries[cache_entry_index].hdr.num_frames; frame++) {
            rect.width = tig_art_cache_entries[cache_entry_index].hdr.frames_tbl[rotation][frame].width;
            rect.height = tig_art_cache_entries[cache_entry_index].hdr.frames_tbl[rotation][frame].height;

            blt.art_id = tig_art_id_frame_set(art_id, frame);
            blt.dst_video_buffer = tig_art_cache_entries[cache_entry_index].video_buffers[palette][rotation][frame];

            if (dword_604718) {
                rc = sub_5059F0(art_id, &blt);
            } else {
                rc = art_blit(art_id, &blt);
            }
        }

        tig_art_cache_entries[cache_entry_index].field_194[palette][rotation] = 0;
    }

    frame = tig_art_id_frame_get(art_id);

    *video_buffer_ptr = tig_art_cache_entries[cache_entry_index].video_buffers[palette][rotation][frame];

    tig_art_cache_entries[cache_entry_index].system_memory_usage += system_memory_size;
    tig_art_cache_entries[cache_entry_index].video_memory_usage += video_memory_size;

    tig_art_available_system_memory -= system_memory_size;
    tig_art_available_video_memory -= video_memory_size;

    return TIG_OK;
}

// 0x505940
int sub_505940(unsigned int art_blt_flags, unsigned int* vb_blt_flags_ptr)
{
    if ((art_blt_flags & 0x1802C) != 0) {
        return TIG_ERR_12;
    }

    *vb_blt_flags_ptr = 0;

    if ((art_blt_flags & 0x1) != 0) {
        *vb_blt_flags_ptr |= 0x1;
    }

    if ((art_blt_flags & 0x2) != 0) {
        *vb_blt_flags_ptr |= 0x2;
    }

    if ((art_blt_flags & 0x10) != 0) {
        *vb_blt_flags_ptr |= 0x4;
    }

    if ((art_blt_flags & 0x40) != 0) {
        *vb_blt_flags_ptr |= 0x10;
    }

    if ((art_blt_flags & 0x80) != 0) {
        *vb_blt_flags_ptr |= 0x20;
    }

    if ((art_blt_flags & 0x100) != 0) {
        *vb_blt_flags_ptr |= 0x40;
    }

    if ((art_blt_flags & 0x200) != 0) {
        *vb_blt_flags_ptr |= 0x80;
    }

    if ((art_blt_flags & 0x400) != 0) {
        *vb_blt_flags_ptr |= 0x100;
    }

    if ((art_blt_flags & 0x800) != 0) {
        *vb_blt_flags_ptr |= 0x100;
    }

    if ((art_blt_flags & 0x1000) != 0) {
        *vb_blt_flags_ptr |= 0x100;
    }

    if ((art_blt_flags & 0x2000) != 0) {
        *vb_blt_flags_ptr |= 0x200;
    }

    if ((art_blt_flags & 0x4000) != 0) {
        *vb_blt_flags_ptr |= 0x400;
    }

    if ((art_blt_flags & 0x20000) != 0) {
        *vb_blt_flags_ptr |= 0x400;
    }

    return TIG_OK;
}

// 0x5059F0
int sub_5059F0(int cache_entry_index, TigArtBlitSpec* blt)
{
    TigVideoBufferData video_buffer_data;
    TigPalette plt;
    int rc;
    int rotation;
    int frame;
    int palette;
    uint8_t* src_pixels;
    int width;
    int height;
    int delta;
    uint8_t* dst_pixels;
    int dst_skip;
    int x;
    int y;
    unsigned int color;

    rc = tig_video_buffer_lock(blt->dst_video_buffer);
    if (rc != TIG_OK) {
        return rc;
    }

    rc = tig_video_buffer_data(blt->dst_video_buffer, &video_buffer_data);
    if (rc != TIG_OK) {
        tig_video_buffer_unlock(blt->dst_video_buffer);
        return rc;
    }

    rotation = tig_art_id_rotation_get(blt->art_id);
    frame = tig_art_id_frame_get(blt->art_id);
    palette = tig_art_palette(blt->art_id);

    src_pixels = tig_art_cache_entries[cache_entry_index].pixels_tbl[rotation][frame];
    width = tig_art_cache_entries[cache_entry_index].hdr.frames_tbl[rotation][frame].width;
    height = tig_art_cache_entries[cache_entry_index].hdr.frames_tbl[rotation][frame].height;

    plt = tig_art_cache_entries[cache_entry_index].hdr.palette_tbl[palette];
    if (plt == NULL) {
        plt = tig_art_cache_entries[cache_entry_index].hdr.palette_tbl[0];
    }

    if ((sub_504FD0(blt->art_id) & 1) != 0) {
        src_pixels = src_pixels + width * height - 1;
        delta = -1;
    } else {
        delta = 1;
    }

    switch (tig_art_bits_per_pixel) {
    case 8:
        break;
    case 16:
        if (delta > 0) {
            dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
            dst_skip = video_buffer_data.pitch / 2 - width;
        } else {
            dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 2 * (height - 1);
            dst_skip = -width - video_buffer_data.pitch / 2;
        }
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                if (*src_pixels != 0) {
                    color = tig_color_16_to_32(*((uint16_t*)plt + *src_pixels));
                } else {
                    color = 0;
                }
                *(uint16_t*)dst_pixels = (uint16_t)color;
                src_pixels += delta;
                dst_pixels += 2;
            }
            dst_pixels += dst_skip;
        }
        break;
    case 24:
        if (delta > 0) {
            dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
            dst_skip = video_buffer_data.pitch / 3 - width;
        } else {
            dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 3 * (height - 1);
            dst_skip = -width - video_buffer_data.pitch / 3;
        }
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                if (*src_pixels != 0) {
                    color = tig_color_16_to_32(*((uint32_t*)plt + *src_pixels));
                } else {
                    color = 0;
                }
                dst_pixels[0] = (uint8_t)color;
                dst_pixels[1] = (uint8_t)(color >> 8);
                dst_pixels[2] = (uint8_t)(color >> 16);
                src_pixels += delta;
                dst_pixels += 3;
            }
            dst_pixels += dst_skip;
        }
        break;
    case 32:
        if (delta > 0) {
            dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
            dst_skip = video_buffer_data.pitch / 4 - width;
        } else {
            dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
            dst_skip = -width - video_buffer_data.pitch / 4;
        }
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                if (*src_pixels != 0) {
                    color = tig_color_16_to_32(*((uint32_t*)plt + *src_pixels));
                } else {
                    color = 0;
                }
                *(uint32_t*)dst_pixels = (uint32_t)color;
                src_pixels += delta;
                dst_pixels += 4;
            }
            dst_pixels += dst_skip;
        }
        break;
    }

    tig_video_buffer_unlock(blt->dst_video_buffer);

    return TIG_OK;
}

static int __ftol(double a1)
{
    return (int)a1;
}

// 0x505EB0
int art_blit(int cache_entry_index, TigArtBlitSpec* blt)
{
    TigVideoBufferData video_buffer_data;
    TigArtCacheEntry* art;
    TigArtFileFrameData* frm;
    TigPalette plt;
    TigRect bounds;
    TigRect src_rect;
    TigRect dst_rect;
    TigRect tmp_rect;
    int rc;
    int rotation;
    int frame;
    int palette;
    uint8_t* src_pixels;
    int width;
    int height;
    int delta;
    uint8_t* dst_pixels;
    int dst_skip;
    int x;
    int y;
    bool stretched;
    float width_ratio;
    float height_ratio;
    // unsigned int color;

    rc = tig_video_buffer_lock(blt->dst_video_buffer);
    if (rc != TIG_OK) {
        return rc;
    }

    rc = tig_video_buffer_data(blt->dst_video_buffer, &video_buffer_data);
    if (rc != TIG_OK) {
        tig_video_buffer_unlock(blt->dst_video_buffer);
        return rc;
    }

    rotation = tig_art_id_rotation_get(blt->art_id);
    frame = tig_art_id_frame_get(blt->art_id);

    art = &(tig_art_cache_entries[cache_entry_index]);
    frm = &(art->hdr.frames_tbl[rotation][frame]);

    if (blt->src_rect->width == blt->dst_rect->width
        && blt->src_rect->height == blt->dst_rect->height) {
        stretched = false;
    } else {
        stretched = true;
        width_ratio = (float)blt->src_rect->width / (float)blt->dst_rect->width;
        height_ratio = (float)blt->dst_rect->height / (float)blt->dst_rect->height;
    }

    src_pixels = art->pixels_tbl[rotation][frame];
    width = frm->width;
    height = frm->height;

    bounds.x = 0;
    bounds.y = 0;
    bounds.width = width;
    bounds.height = height;

    if (tig_rect_intersection(blt->src_rect, &bounds, &src_rect) != TIG_OK) {
        // Specifies source rectangle is out of bounds of the frame, there is
        // nothing to blit.
        tig_video_buffer_unlock(blt->dst_video_buffer);
        return TIG_OK;
    }

    tmp_rect = *blt->dst_rect;

    if (stretched) {
        // TODO: Incomplete.
    } else {
        tmp_rect.x += src_rect.x - blt->src_rect->x;
        tmp_rect.y += src_rect.y - blt->src_rect->y;
        tmp_rect.width += src_rect.width - blt->src_rect->width;
        tmp_rect.height += src_rect.height - blt->src_rect->height;
    }

    bounds.x = 0;
    bounds.y = 0;
    bounds.width = video_buffer_data.width;
    bounds.height = video_buffer_data.height;

    if (tig_rect_intersection(&tmp_rect, &bounds, &dst_rect) != TIG_OK) {
        // Specified destination rectangle is out of bounds of destination
        // video buffer bounds, there is nothing to blit.
        tig_video_buffer_unlock(blt->dst_video_buffer);
        return TIG_OK;
    }

    if (stretched) {
        // TODO: Incomplete.
    } else {
        src_rect.x += dst_rect.x - tmp_rect.x;
        src_rect.y += dst_rect.y - tmp_rect.y;
        src_rect.width += dst_rect.width - tmp_rect.width;
        src_rect.height += dst_rect.width - tmp_rect.height;
    }

    if ((blt->flags & TIG_ART_BLT_PALETTE_CUSTOM) != 0) {
        plt = blt->field_C;
    } else if ((blt->flags & TIG_ART_BLT_PALETTE_NORMAL) != 0) {
        palette = tig_art_palette(blt->art_id);
        plt = art->hdr.palette_tbl[palette];
        if (plt == NULL) {
            plt = art->hdr.palette_tbl[0];
        }
    } else {
        palette = tig_art_palette(blt->art_id);
        plt = art->field_254[palette];
        if (plt == NULL) {
            plt = art->field_254[0];
        }
    }

    // TODO: Incomplete.

    int checkerboard_start_x = src_rect.x;
    int checkerboard_cur_x = src_rect.x;
    int checkerboard_cur_y = src_rect.y;

    if ((sub_504FD0(blt->art_id) & 1) != 0) {
        src_pixels = src_pixels + width * height - 1;
        delta = -1;
    } else {
        delta = 1;
    }

    if (stretched) {
        // TODO: Incomplete.
    } else {
        if ((blt->flags & TIG_ART_BLT_0x2000) != 0) {
            // 0x5084E9
            if ((blt->flags & TIG_ART_BLT_0x10) != 0) {
                // 0x5084F5
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                uint32_t color = tig_color_make_3(*((uint32_t*)plt + *src_pixels), blt->field_10);
                                *(uint32_t*)dst_pixels = tig_color_make_4(*(uint32_t*)dst_pixels, color);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x20) != 0) {
                // 0x508A1C
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                uint32_t color = tig_color_make_3(*((uint32_t*)plt + *src_pixels), blt->field_10);
                                if ((*(uint32_t*)dst_pixels & tig_color_red_mask) < (color & tig_color_red_mask)) {
                                    *(uint32_t*)dst_pixels = (*(uint32_t*)dst_pixels & ~tig_color_red_mask) | (color & tig_color_red_mask);
                                }
                                if ((*(uint32_t*)dst_pixels & tig_color_green_mask) < (color & tig_color_green_mask)) {
                                    *(uint32_t*)dst_pixels = (*(uint32_t*)dst_pixels & ~tig_color_green_mask) | (color & tig_color_green_mask);
                                }
                                if ((*(uint32_t*)dst_pixels & tig_color_blue_mask) < (color & tig_color_blue_mask)) {
                                    *(uint32_t*)dst_pixels = (*(uint32_t*)dst_pixels & ~tig_color_blue_mask) | (color & tig_color_blue_mask);
                                }
                                *(uint32_t*)dst_pixels -= color;
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x40) != 0) {
                // 0x508FB6
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                uint32_t color = tig_color_make_3(*((uint32_t*)plt + *src_pixels), blt->field_10);
                                *(uint32_t*)dst_pixels = tig_color_make_5(color, *(uint32_t*)dst_pixels);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x80) != 0) {
                // 0x50964F
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                uint32_t color = tig_color_make_3(*((uint32_t*)plt + *src_pixels), blt->field_10);
                                *(uint32_t*)dst_pixels = tig_color_make_6(color, *(uint32_t*)dst_pixels);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x100) != 0) {
                // 0x509CD6
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                uint32_t color = tig_color_make_3(*((uint32_t*)plt + *src_pixels), blt->field_10);
                                *(uint32_t*)dst_pixels = tig_color_make_7(color, *(uint32_t*)dst_pixels, blt->field_1C & 0xFF);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x200) != 0) {
                // 0x50ABD1
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                uint32_t color1 = *((uint32_t*)plt + *src_pixels);
                                uint32_t color2 = *(uint32_t*)dst_pixels;
                                uint32_t color3 = blt->field_10;

                                unsigned int red2 = (color2 & tig_color_red_mask);
                                unsigned int green2 = (color2 & tig_color_green_mask);
                                unsigned int blue2 = (color2 & tig_color_blue_mask);
                                unsigned int red3 = (color3 & tig_color_red_mask);
                                unsigned int green3 = (color3 & tig_color_green_mask);
                                unsigned int blue3 = (color3 & tig_color_blue_mask);
                                unsigned int v1 = tig_color_red_value_table[(color1 & tig_color_red_mask) >> tig_color_red_shift];

                                *(uint32_t*)dst_pixels = ((red2 + ((v1 * (red3 - red2)) >> 8)) & tig_color_red_mask)
                                    | ((green2 + ((v1 * (green3 - green2)) >> 8)) & tig_color_green_mask)
                                    | ((blue2 + ((v1 * (blue3 - blue2)) >> 8)) & tig_color_blue_mask);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_GRADIENT_ANY) != 0) {
                // 0x50AF93
            } else if ((blt->flags & TIG_ART_BLT_0x8000) != 0) {
                // 0x50A2A6
            } else if ((blt->flags & TIG_ART_BLT_0x10000) != 0) {
                // 0x50A739
            } else {
                // 0x509647
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                *(uint32_t*)dst_pixels = tig_color_make_3(*((uint32_t*)plt + *src_pixels), blt->field_10);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            }
        } else if ((blt->flags & TIG_ART_BLT_0x4000) != 0) {
            // 0x50BAEF
            if ((blt->flags & TIG_ART_BLT_0x10) != 0) {
                // 0x50BAFB
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x20) != 0) {
                // 0x50C06F
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x40) != 0) {
                // 0x50C646
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x80) != 0) {
                // 0x50CD23
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x100) != 0) {
                // 0x50D3F9
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x200) != 0) {
                // 0x50E3C4
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_GRADIENT_ANY) != 0) {
                // 0x50E7CB
            } else if ((blt->flags & TIG_ART_BLT_0x8000) != 0) {
                // 0x50DA0F
            } else if ((blt->flags & TIG_ART_BLT_0x10000) != 0) {
                // 0x50DEE7
            } else {
                // 0x50CD1B
            }
        } else if ((blt->flags & TIG_ART_BLT_0x20000) != 0) {
            // 0x50F3B4
        } else {
            // 0x50655B
            if ((blt->flags & TIG_ART_BLT_0x10) != 0) {
                // 0x506566
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                *(uint32_t*)dst_pixels = tig_color_make_4(*((uint32_t*)plt + *src_pixels), *(uint32_t*)dst_pixels);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x20) != 0) {
                // 0x506838
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                if ((*(uint32_t*)dst_pixels & tig_color_red_mask) < ((*((uint32_t*)plt + *src_pixels)) & tig_color_red_mask)) {
                                    *(uint32_t*)dst_pixels = (*(uint32_t*)dst_pixels & ~tig_color_red_mask) | ((*((uint32_t*)plt + *src_pixels)) & tig_color_red_mask);
                                }
                                if ((*(uint32_t*)dst_pixels & tig_color_green_mask) < ((*((uint32_t*)plt + *src_pixels)) & tig_color_green_mask)) {
                                    *(uint32_t*)dst_pixels = (*(uint32_t*)dst_pixels & ~tig_color_green_mask) | ((*((uint32_t*)plt + *src_pixels)) & tig_color_green_mask);
                                }
                                if ((*(uint32_t*)dst_pixels & tig_color_blue_mask) < ((*((uint32_t*)plt + *src_pixels)) & tig_color_blue_mask)) {
                                    *(uint32_t*)dst_pixels = (*(uint32_t*)dst_pixels & ~tig_color_blue_mask) | ((*((uint32_t*)plt + *src_pixels)) & tig_color_blue_mask);
                                }
                                *(uint32_t*)dst_pixels -= *((uint32_t*)plt + *src_pixels);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x40) != 0) {
                // 0x506B87
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                *(uint32_t*)dst_pixels = tig_color_make_5(*((uint32_t*)plt + *src_pixels), *(uint32_t*)dst_pixels);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x80) != 0) {
                // 0x506FD0
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                *(uint32_t*)dst_pixels = tig_color_make_6(*((uint32_t*)plt + *src_pixels), *(uint32_t*)dst_pixels);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x100) != 0) {
                // 0x50742A
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                *(uint32_t*)dst_pixels = tig_color_make_7(*((uint32_t*)plt + *src_pixels), *(uint32_t*)dst_pixels, blt->field_1C & 0xFF);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_0x200) != 0) {
                // 0x507B0F
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                *(uint32_t*)dst_pixels = tig_color_make_8(*((uint32_t*)plt + *src_pixels), *(uint32_t*)dst_pixels);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_BLEND_MODE_GRADIENT_ANY) != 0) {
                // 0x507ECB
            } else if ((blt->flags & TIG_ART_BLT_0x8000) != 0) {
                // 0x50779F
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (((checkerboard_cur_x ^ checkerboard_cur_y) & 1) != 0) {
                                if (*src_pixels != 0) {
                                    *(uint32_t*)dst_pixels = *((uint32_t*)plt + *src_pixels);
                                }
                            }
                            src_pixels += delta;
                            dst_pixels += 4;

                            checkerboard_cur_x++;
                        }
                        dst_pixels += dst_skip;

                        checkerboard_cur_x = checkerboard_start_x;
                        checkerboard_cur_y++;
                    }
                    break;
                }
            } else if ((blt->flags & TIG_ART_BLT_0x10000) != 0) {
                // 0x507955
            } else {
                // 0x506FC8
                switch (tig_art_bits_per_pixel) {
                case 32:
                    if (delta > 0) {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels;
                        dst_skip = video_buffer_data.pitch - width * 4;
                    } else {
                        dst_pixels = (uint8_t*)video_buffer_data.surface_data.pixels + video_buffer_data.pitch / 4 * (height - 1);
                        dst_skip = -width * 4 - video_buffer_data.pitch;
                    }
                    dst_pixels += video_buffer_data.pitch * blt->dst_rect->y + blt->dst_rect->x * 4;
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            if (*src_pixels != 0) {
                                *(uint32_t*)dst_pixels = *((uint32_t*)plt + *src_pixels);
                            }
                            src_pixels += delta;
                            dst_pixels += 4;
                        }
                        dst_pixels += dst_skip;
                    }
                    break;
                }
            }
        }
    }

    tig_video_buffer_unlock(blt->dst_video_buffer);

    return TIG_OK;
}

// 0x51AA90
unsigned int sub_51AA90(tig_art_id_t art_id)
{
    char path[_MAX_PATH];
    int cache_entry_index;

    if (tig_art_build_path(art_id, path) != TIG_OK) {
        return (unsigned int)-1;
    }

    if (dword_604714 < tig_art_cache_entries_length
        && strcmp(path, tig_art_cache_entries[dword_604714].path) == 0) {
        tig_art_cache_entries[dword_604714].time = tig_ping_time;
        return dword_604714;
    }

    sub_51AC20();
    sub_51AC20();

    if (!tig_art_cache_find(path, &cache_entry_index)) {
        if (!sub_51B170(art_id, path, cache_entry_index)) {
            tig_debug_printf("ART LOAD FAILURE!!! Trying to load %s\n", path);

            if (!sub_51B170(art_id, "art\\badart.art", cache_entry_index)) {
                tig_debug_printf("ART LOAD FAILURE!!! Trying to load badart.art\n");
                return (unsigned int)-1;
            }
        }
    }

    tig_art_cache_entries[cache_entry_index].time = tig_ping_time;
    tig_art_cache_entries[cache_entry_index].art_id = art_id;
    dword_604714 = cache_entry_index;

    return cache_entry_index;
}

// 0x51AC00
void sub_51AC00(int a1)
{
    // TODO: Check, probably / 100.0 or * 0.01
    flt_5BEA38 = (float)a1 * 0.0099999998f;
}

// 0x51AC20
void sub_51AC20()
{
    // TODO: Incomplete.
}

// 0x51ADE0
int tig_art_cache_entry_compare_time(const void* a1, const void* a2)
{
    TigArtCacheEntry* a = (TigArtCacheEntry*)a1;
    TigArtCacheEntry* b = (TigArtCacheEntry*)a2;
    return a->time - b->time;
}

// 0x51AE00
int tig_art_cache_entry_compare_name(const void* a1, const void* a2)
{
    TigArtCacheEntry* a = (TigArtCacheEntry*)a1;
    TigArtCacheEntry* b = (TigArtCacheEntry*)a2;
    return strcmp(a->path, b->path);
}

// 0x51AE50
int tig_art_build_path(unsigned int art_id, char* path)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_MISC) {
        switch (tig_art_num(art_id)) {
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

// 0x51B0A0
tig_art_id_t sub_51B0A0(tig_art_id_t art_id)
{
    if (tig_art_type(art_id) == TIG_ART_TYPE_MISC) {
        return tig_art_id_frame_set(tig_art_set_palette(art_id, 0), 0);
    }

    if (dword_604750 != NULL) {
        return dword_604750(art_id);
    }

    return art_id;
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

// 0x51B170
bool sub_51B170(tig_art_id_t art_id, const char* path, int cache_entry_index)
{
    TigArtCacheEntry* art;
    int rc;
    int size;
    int type;
    int start;
    int num_rotations;
    int rotation;
    int index;
    int frame;
    int offset;

    if (tig_art_cache_entries_length == tig_art_cache_entries_capacity - 1) {
        tig_art_cache_entries_capacity += 32;
        tig_art_cache_entries = (TigArtCacheEntry*)REALLOC(tig_art_cache_entries,
            sizeof(TigArtCacheEntry) * tig_art_cache_entries_capacity);
    }

    memcpy(&(tig_art_cache_entries[cache_entry_index + 1]),
        &(tig_art_cache_entries[cache_entry_index]),
        sizeof(TigArtCacheEntry) * (tig_art_cache_entries_length - cache_entry_index));

    art = &(tig_art_cache_entries[cache_entry_index]);

    memset(art, 0, sizeof(TigArtCacheEntry));
    strcpy(art->path, path);

    rc = sub_51B710(art_id,
        path,
        &(art->hdr),
        art->field_254,
        0,
        &size);
    if (rc != TIG_OK) {
        memcpy(&(tig_art_cache_entries[cache_entry_index]),
            &(tig_art_cache_entries[cache_entry_index + 1]),
            sizeof(TigArtCacheEntry) * (tig_art_cache_entries_length - cache_entry_index));
        return false;
    }

    art->system_memory_usage += size;

    type = tig_art_type(art_id);
    if ((art->hdr.unk_0 & 0x1) != 0) {
        start = 0;
        num_rotations = 1;
    } else if (dword_5BEA14
        && (type == TIG_ART_TYPE_CRITTER
            || type == TIG_ART_TYPE_MONSTER
            || type == TIG_ART_TYPE_UNIQUE_NPC)) {
        start = 4;
        num_rotations = 5;
    } else {
        start = 0;
        num_rotations = 8;
    }

    for (index = 0; index < num_rotations; index++) {
        rotation = (index + start) % 8;
        art->pixels_tbl[rotation] = (uint8_t**)MALLOC(sizeof(uint8_t*) * art->hdr.num_frames);
        art->system_memory_usage += sizeof(uint8_t*) * art->hdr.num_frames;

        offset = 0;
        for (frame = 0; frame < art->hdr.num_frames; ++frame) {
            art->pixels_tbl[rotation][frame] = art->hdr.pixels_tbl[rotation] + offset;
            offset += art->hdr.frames_tbl[rotation][frame].width * art->hdr.frames_tbl[rotation][frame].height;
        }
    }

    for (index = 8 - num_rotations; index > 0; --index) {
        rotation = (num_rotations + start) % 8;
        art->pixels_tbl[rotation] = art->pixels_tbl[0];
    }

    tig_art_cache_entries_length++;
    tig_art_available_system_memory -= art->system_memory_usage;

    return true;
}

// 0x51B490
void sub_51B490(int cache_entry_index)
{
    // TODO: Incomplete.
    (void)cache_entry_index;
}

// 0x51B610
void sub_51B610(unsigned int cache_entry_index)
{
    int palette;
    int rotation;

    for (palette = 0; palette < 4; palette++) {
        for (rotation = 0; rotation < 8; rotation++) {
            tig_art_cache_entries[cache_entry_index].field_194[palette][rotation] = 1;
        }
    }
}

// 0x51B650
void sub_51B650(int cache_entry_index)
{
    TigArtCacheEntry* art;
    int num_frames;
    int palette;
    int rotation;
    int frame;

    art = &(tig_art_cache_entries[cache_entry_index]);

    if (tig_art_type(art->art_id) == TIG_ART_TYPE_ROOF) {
        num_frames = 13;
    } else {
        num_frames = art->hdr.num_frames;
    }

    for (palette = 0; palette < 4; palette++) {
        for (rotation = 0; rotation < 8; rotation++) {
            if (art->video_buffers[palette][rotation] != NULL) {
                for (frame = 0; frame < num_frames; frame++) {
                    tig_video_buffer_destroy(art->video_buffers[palette][rotation][frame]);
                }

                FREE(art->video_buffers[palette][rotation]);
                art->video_buffers[palette][rotation] = NULL;
            }
        }
    }
}

// 0x51B710
int sub_51B710(tig_art_id_t art_id, const char* filename, TigArtHeader* hdr, void** palette_tbl, int a5, int* size_ptr)
{
    TigFile* stream;
    int rotation;
    int shade;
    uint32_t* saved_palette_table[4];
    uint32_t temp_palette_entries[256];
    int size_tbl[8];
    int index;
    int frame;
    int current_palette_index;
    void* current_palette;
    int num_rotations;

    // NOTE: Keep compiler happy.
    current_palette_index = 0;
    current_palette = NULL;

    *size_ptr = 0;

    for (rotation = 0; rotation < 8; rotation++) {
        hdr->frames_tbl[rotation] = NULL;
        hdr->pixels_tbl[rotation] = NULL;
    }

    stream = tig_file_fopen(filename, "rb");
    if (stream == NULL) {
        return TIG_ERR_16;
    }

    if (tig_file_fread(hdr, sizeof(TigArtHeader), 1, stream) != 1) {
        tig_file_fclose(stream);
        return TIG_ERR_16;
    }

    if (hdr->field_8 != 8) {
        tig_file_fclose(stream);
        return TIG_ERR_16;
    }

    if (a5) {
        current_palette_index = tig_art_palette(art_id);
        if (hdr->palette_tbl[current_palette_index] != NULL) {
            tig_file_fclose(stream);
            return TIG_ERR_16;
        }
        current_palette = palette_tbl[0];
    }

    for (shade = 0; shade < 4; shade++) {
        saved_palette_table[shade] = hdr->palette_tbl[shade];
        hdr->palette_tbl[shade] = NULL;
        palette_tbl[shade] = NULL;
    }

    for (shade = 0; shade < 4; shade++) {
        if (saved_palette_table[shade] != NULL) {
            if (tig_file_fread(temp_palette_entries, sizeof(uint32_t), 256, stream) != 256) {
                sub_51BE50(stream, hdr, palette_tbl);
                return TIG_ERR_16;
            }

            if (a5) {
                if (shade == current_palette_index) {
                    for (index = 0; index < 256; index++) {
                        ((uint32_t*)current_palette)[index] = temp_palette_entries[index];
                    }

                    tig_file_fclose(stream);
                    return TIG_OK;
                }
            } else {
                hdr->palette_tbl[shade] = tig_palette_create();
                palette_tbl[shade] = tig_palette_create();
                *size_ptr += 2 * tig_palette_get_size();

                switch (tig_art_bits_per_pixel) {
                case 8:
                    for (index = 0; index < 256; index++) {
                        ((uint8_t*)hdr->palette_tbl[shade])[index] = (uint8_t)tig_color_index_of(temp_palette_entries[index]);
                    }
                    break;
                case 16:
                    for (index = 0; index < 256; index++) {
                        ((uint16_t*)hdr->palette_tbl[shade])[index] = (uint16_t)tig_color_index_of(temp_palette_entries[index]);
                    }
                    break;
                case 24:
                    for (index = 0; index < 256; index++) {
                        ((uint32_t*)hdr->palette_tbl[shade])[index] = (uint32_t)tig_color_index_of(temp_palette_entries[index]);
                    }
                    break;
                case 32:
                    for (index = 0; index < 256; index++) {
                        ((uint32_t*)hdr->palette_tbl[shade])[index] = (uint32_t)tig_color_index_of(temp_palette_entries[index]);
                    }
                    break;
                }
            }

            sub_505000(art_id, hdr->palette_tbl[shade], palette_tbl[shade]);
        }
    }

    num_rotations = sub_51BE30(hdr);

    for (rotation = 0; rotation < num_rotations; rotation++) {
        hdr->frames_tbl[rotation] = (TigArtFileFrameData*)MALLOC(sizeof(TigArtFileFrameData) * hdr->num_frames);
        *size_ptr += sizeof(TigArtFileFrameData) * hdr->num_frames;

        if (tig_file_fread(hdr->frames_tbl[rotation], sizeof(TigArtFileFrameData), hdr->num_frames, stream) != hdr->num_frames) {
            sub_51BE50(stream, hdr, palette_tbl);
            return TIG_ERR_16;
        }
    }

    for (index = 0; index < num_rotations; index++) {
        uint8_t* bytes;
        int total_size;

        // Calculate total size of pixels data.
        total_size = 0;
        for (frame = 0; frame < hdr->num_frames; ++frame) {
            total_size += hdr->frames_tbl[index][frame].width * hdr->frames_tbl[index][frame].height;
        }

        // Allocate appropriate memory.
        hdr->pixels_tbl[index] = (uint8_t*)MALLOC(total_size);

        // Store size for later use.
        size_tbl[index] = total_size;
        *size_ptr += total_size;

        // Read raw pixel data for each frame.
        bytes = hdr->pixels_tbl[index];
        for (frame = 0; frame < hdr->num_frames; ++frame) {
            if (hdr->frames_tbl[index][frame].data_size == hdr->frames_tbl[index][frame].width * hdr->frames_tbl[index][frame].height) {
                // Pixels are not compressed, read everything in one go.
                if (tig_file_fread(hdr->pixels_tbl[index], 1, hdr->frames_tbl[index][frame].data_size, stream) != hdr->frames_tbl[index][frame].data_size) {
                    sub_51BE50(stream, hdr, palette_tbl);
                    return TIG_ERR_16;
                }
            } else if (hdr->frames_tbl[index][frame].data_size > 0) {
                // Pixels are RLE-encoded.
                uint8_t value;
                uint8_t color;
                int cnt = 0;

                while (cnt < hdr->frames_tbl[index][frame].data_size) {
                    if (tig_file_fread(&value, 1, 1, stream) != 1) {
                        sub_51BE50(stream, hdr, palette_tbl);
                        return TIG_ERR_16;
                    }

                    if ((value & 0x80) != 0) {
                        if (tig_file_fread(bytes, 1, value & 0x7F, stream) != (value & 0x7F)) {
                            sub_51BE50(stream, hdr, palette_tbl);
                            return TIG_ERR_16;
                        }
                        cnt += 1 + (value & 0x7F);
                    } else {
                        if (tig_file_fread(&color, 1, 1, stream) != 1) {
                            sub_51BE50(stream, hdr, palette_tbl);
                            return TIG_ERR_16;
                        }

                        memset(bytes, color, value & 0x7F);
                        cnt += 2;
                    }
                    bytes += value & 0x7F;
                }
            }
        }
    }

    while (index < 8) {
        hdr->frames_tbl[index] = hdr->frames_tbl[0];
        hdr->pixels_tbl[index] = hdr->pixels_tbl[0];
        index++;
    }

    if (num_rotations > 1) {
        int type = tig_art_type(art_id);
        if (dword_5BEA14) {
            if (type == TIG_ART_TYPE_CRITTER
                || type == TIG_ART_TYPE_MONSTER
                || type == TIG_ART_TYPE_UNIQUE_NPC) {
                for (index = 0; index < 3; index++) {
                    *size_ptr -= size_tbl[1 + index];

                    FREE(hdr->pixels_tbl[1 + index]);
                    hdr->pixels_tbl[1 + index] = hdr->pixels_tbl[0];

                    FREE(hdr->frames_tbl[1 + index]);
                    hdr->frames_tbl[1 + index] = hdr->frames_tbl[0];
                }
            }
        }
    }

    tig_file_fclose(stream);
    return TIG_OK;
}

// 0x51BE30
int sub_51BE30(TigArtHeader* hdr)
{
    return (hdr->unk_0 & 1) != 0 ? 1 : 8;
}

// 0x51BE50
void sub_51BE50(TigFile* stream, TigArtHeader* hdr, TigPalette* palette_table)
{
    int rotation;

    if (stream != NULL) {
        tig_file_fclose(stream);
    }

    sub_51BF20(hdr);

    for (rotation = 0; rotation < 4; ++rotation) {
        if (hdr->palette_tbl[rotation] != NULL) {
            tig_palette_destroy(hdr->palette_tbl[rotation]);
        }

        if (palette_table[rotation] != NULL) {
            tig_palette_destroy(palette_table[rotation]);
        }
    }
}

// 0x51BF20
void sub_51BF20(TigArtHeader* hdr)
{
    int num_rotations;
    int rotation;

    num_rotations = sub_51BE30(hdr);
    for (rotation = 0; rotation < num_rotations; rotation++) {
        if (hdr->pixels_tbl[rotation] != NULL) {
            FREE(hdr->pixels_tbl[rotation]);
        }
        if (hdr->frames_tbl[rotation] != NULL) {
            FREE(hdr->frames_tbl[rotation]);
        }
    }
}
