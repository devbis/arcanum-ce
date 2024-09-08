#include "game/townmap.h"

#include <stdio.h>

#include "game/mes.h"

typedef struct TownMapIndexEntry {
    int map;
    bool w;
} TownMapIndexEntry;

static void sub_4BED00();
static void sub_4BED90(int map, int index);
static bool sub_4BEDD0(int map, int index);
static bool sub_4BEE60(int map);
static void sub_4BEF40(int map);
static void sub_4BEFB0();

// 0x5B6450
static mes_file_handle_t townmap_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5FC4C8
static TigRect stru_5FC4C8;

// 0x5FC4D8
static TownMapInfo townmap_info_cache;

// 0x5FC508
static TownMapIndexEntry* townmap_entries;

// 0x5FC50C
static bool dword_5FC50C;

// 0x5FC510
static uint8_t* dword_5FC510;

// 0x5FC514
static int dword_5FC514;

// 0x5FC518
static int dword_5FC518;

// 0x5FC51C
static int townmap_entries_cnt;

// 0x4BE1B0
void townmap_init()
{
    sub_4BED00();
}

// 0x4BE1C0
void townmap_reset()
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
                while (isspace(pch)) {
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
void townmap_mod_load()
{
    sub_4BED00();

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
    sub_4BEFB0();
    sub_4BED00();
}

// 0x4BE380
void sub_4BE380()
{
    // TODO: Incomplete.
}

// 0x4BE400
void townmap_count()
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
    char* name;
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

    if (tig_file_fseek(stream, -sizeof(version), SEEK_CUR) != 0
        || tig_file_fread(tmi, sizeof(*tmi), 1, stream) != 1) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);

    stru_5FC4C8.x = 0;
    stru_5FC4C8.y = 0;
    stru_5FC4C8.width = tmi->field_C / tmi->width * tmi->field_28;
    stru_5FC4C8.height = tmi->field_10 / tmi->height * tmi->field_28;

    townmap_info_cache = *tmi;

    return true;
}

// 0x4BE670
void sub_4BE670()
{
    // TODO: Incomplete.
}

// 0x4BE780
void sub_4BE780()
{
    // TODO: Incomplete.
}

// 0x4BE8F0
void sub_4BE8F0()
{
    // TODO: Incomplete.
}

// 0x4BEAB0
bool sub_4BEAB0(int map, int a2)
{
    TownMapInfo tmi;

    if (map == 0) {
        return false;
    }

    if (!townmap_info(map, &tmi)) {
        return false;
    }

    sub_4BED30(map, a2);

    return true;
}

// 0x4BEAF0
void sub_4BEAF0()
{
    // TODO: Incomplete.
}

// 0x4BECC0
bool sub_4BECC0(int map)
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
void sub_4BED00()
{
    if (dword_5FC510 != NULL) {
        FREE(dword_5FC510);
        dword_5FC510 = NULL;
    }
    dword_5FC514 = 0;
    dword_5FC50C = false;
    dword_5FC518 = 0;
}

// 0x4BED30
void sub_4BED30()
{
    // TODO: Incomplete.
}

// 0x4BED90
void sub_4BED90(int map, int index)
{
    if (sub_4BEE60(map)) {
        dword_5FC510[index / 8] |= 1 << (index & 7);
        dword_5FC50C = true;
    }
}

// 0x4BEDD0
bool sub_4BEDD0(int map, int index)
{
    char* name;
    char path[TIG_MAX_PATH];

    if (!sub_4BEE60(map)) {
        return false;
    }

    if ((dword_5FC510[index / 8] & (1 << (index & 7))) == 0) {
        return false;
    }

    name = townmap_name(map);
    sprintf(path, "townmap\\%s\\%s%06d.bmp", name, name, index);
    return tig_file_exists(path, NULL);
}

// 0x4BEE60
bool sub_4BEE60(int map)
{
    char path[TIG_MAX_PATH];
    TigFile* stream;

    if (dword_5FC518 == map) {
        return true;
    }

    sub_4BEFB0();
    sub_4BEF40(map);

    sprintf(path, "%s\\%s.tmf", "Save\\Current", townmap_name(dword_5FC518));
    if (!tig_file_exists(path, NULL)) {
        sub_4BED30(map, 0);
        return true;
    }

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        return false;
    }

    if (tig_file_fread(dword_5FC510, 1u, dword_5FC514, stream) != dword_5FC514) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}

// 0x4BEF40
void sub_4BEF40(int map)
{
    TownMapInfo tmi;

    sub_4BED00();

    if (townmap_info(map, &tmi)) {
        dword_5FC514 = tmi.width * tmi.height / 8;
        if (tmi.width * tmi.height % 8 != 0) {
            dword_5FC514++;
        }

        dword_5FC510 = (uint8_t*)MALLOC(dword_5FC514);
        dword_5FC518 = map;
    }
}

// 0x4BEFB0
void sub_4BEFB0()
{
    char path[TIG_MAX_PATH];
    TigFile* stream;

    if (!dword_5FC50C) {
        return;
    }

    sprintf(path, "%s\\%s.tmf", "Save\\Current", townmap_name(dword_5FC518));
    stream = tig_file_fopen(path, "wb");
    if (stream == NULL) {
        return;
    }

    if (tig_file_fwrite(dword_5FC510, 1, dword_5FC514, stream) != dword_5FC514) {
        tig_file_fclose(stream);
        tig_file_remove(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}
