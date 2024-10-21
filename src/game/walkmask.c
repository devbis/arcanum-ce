#include "game/walkmask.h"

#include <stdio.h>

static void sub_4F6E60(int a1, char* buffer);
static bool sub_4F6E90(TigFile* stream, int* a2, int* a3, int* a4, int* a5, int* a6, int* a7);
static bool sub_4F6F60(TigFile* stream);
static tig_art_id_t* sub_4F6FC0(int width, int height);
static bool sub_4F7020(TigFile* stream, tig_art_id_t* ids, int a3, int a4, int width, int height, int a7, int a8, int a9);

// 0x5BCFCC
static const char* off_5BCFCC = "FacWalk V101  ";

// 0x4F6CF0
bool sub_4F6CF0(int a1, tig_art_id_t** ids_ptr, int* width_ptr, int* height_ptr)
{
    char path[100];
    TigFile* stream;
    tig_art_id_t* ids;
    int v1;
    int width;
    int height;
    int v2;
    int v3;
    int v4;

    sub_4F6E60(a1, path);

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        return false;
    }

    if (!sub_4F6E90(stream, &v1, &width, &height, &v2, &v3, &v4)) {
        tig_file_fclose(stream);
        return false;
    }

    ids = sub_4F6FC0(width, height);
    if (!sub_4F7020(stream, ids, a1, v1, width, height, v2, v3, v4)) {
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
void sub_4F6E60(int a1, char* buffer)
{
    if (a1 >= 0 && a1 < 512) {
        sprintf(buffer, "art\\Facade\\facwalk.%02X", a1);
    } else {
        buffer[0] = '\0';
    }
}

// 0x4F6E90
bool sub_4F6E90(TigFile* stream, int* a2, int* a3, int* a4, int* a5, int* a6, int* a7)
{
    if (!sub_4F6F60(stream)) return false;
    if (tig_file_fread(a5, sizeof(*a5), 1, stream) != 1) return false;
    if (tig_file_fread(a6, sizeof(*a6), 1, stream) != 1) return false;
    if (tig_file_fread(a7, sizeof(*a7), 1, stream) != 1) return false;
    if (tig_file_fread(a3, sizeof(*a3), 1, stream) != 1) return false;
    if (tig_file_fread(a4, sizeof(*a4), 1, stream) != 1) return false;
    if (tig_file_fread(a2, sizeof(*a2), 1, stream) != 1) return false;
    if (*a2 > 1024) return false;

    return true;
}

// 0x4F6F60
bool sub_4F6F60(TigFile* stream)
{
    char str[100];

    if (tig_file_fread(str, strlen(off_5BCFCC), 1, stream) != 1) return false;
    if (strncmp(str, off_5BCFCC, sizeof(off_5BCFCC)) != 0) return false;

    return true;
}

// 0x4F6FC0
tig_art_id_t* sub_4F6FC0(int width, int height)
{
    int x;
    int y;
    tig_art_id_t* ids;
    int index;

    ids = (tig_art_id_t*)MALLOC(sizeof(*ids) * width * height);

    index = 0;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            ids[index] = TIG_ART_ID_INVALID;
        }
    }

    return ids;
}

// 0x4F7020
bool sub_4F7020(TigFile* stream, tig_art_id_t* ids, int a3, int a4, int width, int height, int a7, int a8, int a9)
{
    tig_art_id_t art_id;
    int x;
    int y;
    int v3;

    for (x = 0; x < a4; x++) {
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

        if (tig_file_fread(&v3, sizeof(v3), 1, stream) != 1) {
            return false;
        }

        if (tig_art_facade_id_create(a3, a7, a8, a9, x, v3, &art_id) != TIG_OK) {
            return false;
        }

        ids[x + y * width] = art_id;
    }

    return true;
}
