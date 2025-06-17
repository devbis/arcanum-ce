#include "game/walkmask.h"

#include <stdio.h>

static void walkmask_build_path(int facade, char* buffer);
static bool walkmask_read_hdr(TigFile* stream, int* cnt, int* width, int* height, int* tile, int* outdoor, int* flippable);
static bool walkmask_read_signature(TigFile* stream);
static tig_art_id_t* walkmask_allocate(int width, int height);
static bool walkmask_read_data(TigFile* stream, tig_art_id_t* ids, int facade, int cnt, int width, int height, int tile, int outdoor, int flippable);

// 0x5BCFCC
static const char* walkmask_signature = "FacWalk V101  ";

// 0x4F6CF0
bool walkmask_load(int facade, tig_art_id_t** ids_ptr, int* width_ptr, int* height_ptr)
{
    char path[100];
    TigFile* stream;
    tig_art_id_t* ids;
    int cnt;
    int width;
    int height;
    int tile;
    int outdoor;
    int flippable;

    walkmask_build_path(facade, path);

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        return false;
    }

    if (!walkmask_read_hdr(stream, &cnt, &width, &height, &tile, &outdoor, &flippable)) {
        tig_file_fclose(stream);
        return false;
    }

    ids = walkmask_allocate(width, height);
    if (!walkmask_read_data(stream, ids, facade, cnt, width, height, tile, outdoor, flippable)) {
        FREE(ids);
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);

    *ids_ptr = ids;
    *width_ptr = width;
    *height_ptr = height;

    return true;
}

// 0x4F6E60
void walkmask_build_path(int facade, char* buffer)
{
    if (facade >= 0 && facade < 512) {
        sprintf(buffer, "art\\Facade\\facwalk.%02X", facade);
    } else {
        buffer[0] = '\0';
    }
}

// 0x4F6E90
bool walkmask_read_hdr(TigFile* stream, int* cnt, int* width, int* height, int* tile, int* outdoor, int* flippable)
{
    if (!walkmask_read_signature(stream)) return false;
    if (tig_file_fread(tile, sizeof(*tile), 1, stream) != 1) return false;
    if (tig_file_fread(outdoor, sizeof(*outdoor), 1, stream) != 1) return false;
    if (tig_file_fread(flippable, sizeof(*flippable), 1, stream) != 1) return false;
    if (tig_file_fread(width, sizeof(*width), 1, stream) != 1) return false;
    if (tig_file_fread(height, sizeof(*height), 1, stream) != 1) return false;
    if (tig_file_fread(cnt, sizeof(*cnt), 1, stream) != 1) return false;
    if (*cnt > 1024) return false;

    return true;
}

// 0x4F6F60
bool walkmask_read_signature(TigFile* stream)
{
    char signature[100];

    if (tig_file_fread(signature, strlen(walkmask_signature), 1, stream) != 1) return false;
    if (strncmp(signature, walkmask_signature, strlen(walkmask_signature)) != 0) return false;

    return true;
}

// 0x4F6FC0
tig_art_id_t* walkmask_allocate(int width, int height)
{
    int x;
    int y;
    tig_art_id_t* ids;
    int index;

    ids = (tig_art_id_t*)MALLOC(sizeof(*ids) * width * height);

    index = 0;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            ids[index++] = TIG_ART_ID_INVALID;
        }
    }

    return ids;
}

// 0x4F7020
bool walkmask_read_data(TigFile* stream, tig_art_id_t* ids, int facade, int cnt, int width, int height, int tile, int outdoor, int flippable)
{
    tig_art_id_t art_id;
    int frame;
    int x;
    int y;
    int walkable;

    for (frame = 0; frame < cnt; frame++) {
        if (tig_file_fread(&x, sizeof(x), 1, stream) != 1) {
            return false;
        }

        if (x < 0 || x >= width) {
            return false;
        }

        if (tig_file_fread(&y, sizeof(y), 1, stream) != 1) {
            return false;
        }

        if (y < 0 || y >= height) {
            return false;
        }

        if (tig_file_fread(&walkable, sizeof(walkable), 1, stream) != 1) {
            return false;
        }

        if (tig_art_facade_id_create(facade, tile, outdoor, flippable, frame, walkable, &art_id) != TIG_OK) {
            return false;
        }

        ids[x + y * width] = art_id;
    }

    return true;
}
