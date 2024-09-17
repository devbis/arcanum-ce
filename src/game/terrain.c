#include "game/terrain.h"

#include "game/mes.h"
#include "game/random.h"

#define FIRST_TERRAIN_COLOR_ID 100
#define FIRST_TERRAIN_TRANSITION_ID 200

#define TERRAIN_TYPE_MAX 32

typedef struct TerrainHeader {
    /* 0000 */ float version;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t field_8;
    /* 0010 */ int64_t field_10;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
} TerrainHeader;

static_assert(sizeof(TerrainHeader) == 0x20, "wrong size");

static const char* terrain_base_name(int terrain_type);
static int terrain_match_base_name(const char* base_name);
static void terrain_color(int terrain_type, int* red, int* green, int* blue);
static void sub_4E8CE0(int a1, int a2, int a3, int a4, uint16_t* value);
static int64_t sub_4E8E60(int a1);
static bool terrain_init_types();
static bool sub_4E8F40();
static bool terrain_init_transitions();
static bool terrain_parse_transition(char* str, int* from_terrain_type, int* to_terrain_type);
static bool sub_4E92B0(int from, int to, bool* transitions, int* a4);
static uint16_t sub_4E93A0(int from, int to);
static bool sub_4E93E0(int from, int to);

// 0x603748
static int dword_603748[TERRAIN_TYPE_MAX];

// 0x6037C8
static char byte_6037C8[TIG_MAX_PATH];

// 0x6038E0
static bool terrain_editor;

// 0x6038E4
static char byte_6038E4[TIG_MAX_PATH];

// 0x6039E8
static int* dword_6039E8;

// 0x6039EC
static uint16_t* dword_6039EC;

// 0x6039F0
static TerrainHeader terrain_header;

// 0x603A10
static bool dword_603A10;

// 0x603A14
static int dword_603A14;

// 0x603A18
static int dword_603A18;

// 0x603A1C
static mes_file_handle_t terrain_mes_file;

// 0x603A20
static int dword_603A20[TERRAIN_TYPE_MAX];

// 0x4E7B00
bool terrain_init(GameInitInfo* init_info)
{
    if (!mes_load("terrain\\terrain.mes", &terrain_mes_file)) {
        return false;
    }

    if (!terrain_init_types()) {
        return false;
    }

    if (!sub_4E8F40()) {
        return false;
    }

    if (!terrain_init_transitions()) {
        return false;
    }

    terrain_editor = init_info->editor;

    return true;
}

// 0x4E7B50
void terrain_reset()
{
}

// 0x4E7B60
void terrain_exit()
{
    if (dword_6039E8 != NULL) {
        FREE(dword_6039E8);
        dword_6039E8 = NULL;
    }

    mes_unload(terrain_mes_file);
}

// 0x4E7B90
void sub_4E7B90(MapResetInfo* reset_info)
{
    int index;

    terrain_close();

    terrain_header.version = 1.2f;
    terrain_header.field_4 = 0;
    terrain_header.field_8 = reset_info->field_10;
    terrain_header.field_10 = reset_info->field_18;
    terrain_header.field_18 = reset_info->field_8;

    dword_603A18 = sizeof(uint16_t) * (int)terrain_header.field_8 * (int)terrain_header.field_10;
    dword_6039EC = (uint16_t*)MALLOC(dword_603A18);

    for (index = 0; index < terrain_header.field_10 * terrain_header.field_8; index++) {
        dword_6039EC[index] = sub_4E8D60(reset_info->field_8, reset_info->field_8, 15);
    }

    sprintf(byte_6038E4, "%s\\terrain.tdf", reset_info->name);

    if (terrain_editor) {
        sprintf(byte_6037C8, "%s\\terrain.tdf", reset_info->name);
    }

    dword_603A10 = true;

    terrain_flush();
}


// 0x4E7CB0
void terrain_open()
{
    // TODO: Incomplete.
}

// 0x4E7E80
void terrain_close()
{
    // TODO: Incomplete.
}

// 0x4E7EF0
void sub_4E7EF0()
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char path1[TIG_MAX_PATH];
    char path2[TIG_MAX_PATH];

    _splitpath(byte_6038E4, drive, dir, NULL, NULL);
    sprintf(path1, "%s%s", drive, dir);
    _splitpath(byte_6037C8, drive, dir, NULL, NULL);
    sprintf(path2, "%s%s", drive, dir);
    terrain_close();
    terrain_open(path1, path2);
}

// 0x4E7F90
void sub_4E7F90()
{
    // TODO: Incomplete.
}

// 0x4E80C0
void sub_4E80C0()
{
    // TODO: Incomplete.
}

// 0x4E84C0
void terrain_sector_path()
{
    // TODO: Incomplete.
}

// 0x4E86F0
void sub_4E86F0(Sector* sector)
{
    tig_art_id_t art_id;
    int index;

    tig_art_tile_id_create(7, 7, 15, 0, 0, 0, 0, 0, &art_id);

    for (index = 0; index < 4096; index++) {
        sector->tiles.field_0[index] = sub_4D7480(art_id, 7, 0, 15);
    }
}

// 0x4E8740
void terrain_flush()
{
    TigFile* stream;
    unsigned int flags;
    TerrainHeader hdr;

    if (terrain_editor) {
        if (dword_603A10) {
            stream = tig_file_fopen(byte_6037C8, "wb");
            if (stream != NULL) {
                flags = terrain_header.field_4;
                hdr = terrain_header;
                hdr.field_4 = terrain_header.field_4 | 0x1;
                if (tig_file_fwrite(&hdr, sizeof(hdr), 1, stream) == 1) {
                    if (sub_4E9680(stream)) {
                        dword_603A10 = false;
                        tig_file_fclose(stream);
                    }
                }
            }
        }
    }
}

// 0x4E8B20
const char* terrain_base_name(int terrain_type)
{
    MesFileEntry mes_file_entry;

    if (terrain_type >= 0 && terrain_type < dword_603A14) {
        mes_file_entry.num = terrain_type;

        if (mes_search(terrain_mes_file, &mes_file_entry)) {
            return mes_file_entry.str;
        }
    }

    return NULL;
}

// 0x4E8B60
int terrain_match_base_name(const char* base_name)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = 0;
    if (mes_search(terrain_mes_file, &mes_file_entry)) {
        do {
            if (_strcmpi(base_name, mes_file_entry.str) == 0) {
                return mes_file_entry.num;
            }
        } while (mes_find_next(terrain_mes_file, &mes_file_entry));
    }

    return -1;
}

// 0x4E8BE0
void terrain_color(int terrain_type, int* red, int* green, int* blue)
{
    MesFileEntry mes_file_entry;
    char buffer[MAX_STRING];
    char* tok;

    *red = 255;
    *green = 0;
    *blue = 0;

    if (terrain_type >= 0 && terrain_type < dword_603A14) {
        mes_file_entry.num = terrain_type + FIRST_TERRAIN_COLOR_ID;
        if (mes_search(terrain_mes_file, &mes_file_entry)) {
            strcpy(buffer, mes_file_entry.str);

            tok = strtok(buffer, ",");
            if (tok != NULL) {
                *red = atoi(tok);

                tok = strtok(NULL, ",");
                if (tok != NULL) {
                    *green = atoi(tok);

                    tok = strtok(NULL, ",");
                    if (tok != NULL) {
                        *blue = atoi(tok);
                    }
                }
            }
        }
    }
}

// 0x4E8CE0
void sub_4E8CE0(int a1, int a2, int a3, int a4, uint16_t* value)
{
    a1 = __min(__max(a1, 0), 31);
    a2 = __min(__max(a2, 0), 31);
    a3 = __min(__max(a3, 0), 15);
    a4 = __min(__max(a4, 0), 3);

    // TODO: Check.
    *value = (uint16_t)(a4 | (a3 << 2) | (a2 << 4) | (a1 << 11));
}

// 0x4E8DC0
int sub_4E8DC0(uint16_t a1)
{
    return (a1 >> 11) & 0x1F;
}

// 0x4E8DD0
int sub_4E8DD0(uint16_t a1)
{
    return (a1 >> 6) & 0x1F;
}

// 0x4E8DE0
int sub_4E8DE0(uint16_t a1)
{
    return (a1 >> 2) & 0xF;
}

// 0x4E8DF0
int sub_4E8DF0(uint16_t a1)
{
    return a1 & 3;
}

// 0x_4E8E00
bool sub_4E8E00(int64_t a1)
{
    uint16_t v1;

    v1 = sub_4E87F0(a1);
    if (v1 == 0xFFFF) {
        return true;
    }

    if ((dword_603A20[sub_4E8DC0(v1)] & 1) != 0) {
        return true;
    }

    if ((dword_603A20[sub_4E8DD0(v1)] & v1) != 0) {
        return true;
    }

    return false;
}

// 0x4E8E60
int64_t sub_4E8E60(int a1)
{
    // TODO: Check.
    int64_t v1 = a1;
    return (v1 % 2) | ((v1 / 2) << 26);
}

// 0x4E8EA0
bool terrain_init_types()
{
    MesFileEntry mes_file_entry;
    int index;
    char* pch;

    for (index = 0; index < TERRAIN_TYPE_MAX; index++) {
        mes_file_entry.num = index;
        if (!mes_search(terrain_mes_file, &mes_file_entry)) {
            break;
        }

        pch = strstr(mes_file_entry.str, "/b");
        if (pch != NULL) {
            dword_603A20[index] = 1;

            while (isspace(*pch)) {
                pch--;
            }

            pch[1] = '\0';
        }
    }

    return true;
}

// 0x4E8F40
bool sub_4E8F40()
{
    MesFileEntry msg;
    char path[TIG_MAX_PATH];
    int index;
    int k;

    for (index = 0; index < TERRAIN_TYPE_MAX; index++) {
        msg.num = index;
        if (!mes_search(terrain_mes_file, &msg)) {
            break;
        }

        for (k = 0; k <= 3; k++) {
            strcpy(path, "terrain\\");
            strcat(path, msg.str);
            strcat(path, "\\");
            _i64toa(sub_4E8E60(k), path + strlen(path), 10);
            strcat(path, ".sec");

            if (!tig_file_exists(path, NULL)) {
                break;
            }

            dword_603748[index]++;
        }

        if (k == 0 && index == 0) {
            return false;
        }
    }

    if (index == 0) {
        return false;
    }

    dword_603A14 = index;

    return true;
}

// 0x4E90B0
bool terrain_init_transitions()
{
    MesFileEntry mes_file_entry;
    int transition_count;
    bool* transitions;
    int index;
    int from;
    int to;
    int v1;

    mes_file_entry.num = FIRST_TERRAIN_TRANSITION_ID;
    if (!mes_search(terrain_mes_file, &mes_file_entry)) {
        return false;
    }

    transition_count = dword_603A14 * dword_603A14;
    transitions = (bool*)MALLOC(sizeof(*transitions) * transition_count);
    for (index = 0; index < transition_count; index++) {
        transitions[index] = false;
    }

    do {
        if (!terrain_parse_transition(mes_file_entry.str, &from, &to)) {
            FREE(transitions);
            return false;
        }

        transitions[to + from * dword_603A14] = true;
        transitions[from + to * dword_603A14] = true;
    } while (mes_find_next(terrain_mes_file, &mes_file_entry));

    dword_6039E8 = (int*)MALLOC(sizeof(*dword_6039E8) * transition_count);

    for (from = 0; from < dword_603A14; from++) {
        for (to = 0; to < dword_603A14; to++) {
            if (!sub_4E92B0(from, to, transitions, &v1)) {
                FREE(transitions);
                return false;
            }

            dword_6039E8[to + from * dword_603A14] = v1;
        }
    }

    FREE(transitions);
    return true;
}

// 0x4E9240
bool terrain_parse_transition(char* str, int* from_terrain_type, int* to_terrain_type)
{
    char* pch;
    int from;
    int to;

    pch = strstr(str, " to ");
    if (pch == NULL) {
        return false;
    }

    *pch = '\0';

    from = terrain_match_base_name(str);
    if (from == -1) {
        // FIXME: `str` not restored to previous state.
        return false;
    }

    *pch = ' ';

    to = terrain_match_base_name(pch + 4);
    if (to = -1) {
        return false;
    }

    *from_terrain_type = from;
    *to_terrain_type = to;

    return true;
}

// 0x4E92B0
bool sub_4E92B0(int from, int to, bool* transitions, int* a4)
{
    int* v1;
    bool result;

    if (from == to) {
        *a4 = from;
        return true;
    }

    v1 = (int*)MALLOC(sizeof(*v1) * dword_603A14);
    v1[0] = from;

    result = sub_4E9310(v1, 0, to, transitions);

    *a4 = v1[1];
    FREE(v1);

    return result;
}

// 0x4E93A0
uint16_t sub_4E93A0(int from, int to)
{
    uint16_t value;
    int v1;

    v1 = dword_6039E8[to + from * dword_603A14];
    sub_4E8CE0(v1, v1, 15, 0, &value);

    return value;
}

// 0x4E93E0
bool sub_4E93E0(int from, int to)
{
    int terrain_type;

    terrain_type = sub_4E8DC0(sub_4E93A0(from, to));
    return terrain_type == from || terrain_type == to;
}
