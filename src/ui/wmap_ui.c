#include "ui/wmap_ui.h"

#include <stdio.h>

#include "game/ai.h"
#include "game/anim.h"
#include "game/area.h"
#include "game/critter.h"
#include "game/location.h"
#include "game/map.h"
#include "game/magictech.h"
#include "game/mes.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/sector.h"
#include "game/teleport.h"
#include "game/townmap.h"
#include "game/ui.h"
#include "ui/anim_ui.h"
#include "ui/intgame.h"
#include "ui/textedit_ui.h"
#include "ui/types.h"

typedef struct S5C9160 {
    // NOTE: Rare case - during initialization the value at offset 0 is art num,
    // which is replaced with interface art id (see `wmap_ui_init`).
    union {
        int num;
        tig_art_id_t art_id;
    } data;
    /* 0004 */ int width;
    /* 0008 */ int height;
    /* 000C */ tig_color_t color;
    /* 0010 */ tig_font_handle_t font;
    /* 0014 */ int field_14;
} S5C9160;

static_assert(sizeof(S5C9160) == 0x18, "wrong size");

typedef struct WmapNote {
    /* 0000 */ int id;
    /* 0004 */ int field_4;
    /* 0008 */ int x;
    /* 000C */ int y;
    /* 0010 */ int64_t field_10;
    /* 0018 */ TigRect field_18;
    /* 0028 */ int field_28;
    /* 002C */ char str[250];
    /* 0128 */ TigVideoBuffer* video_buffer;
    /* 012C */ int field_12C;
} WmapNote;

static_assert(sizeof(WmapNote) == 0x130, "wrong size");

typedef struct S5C9228_F180 {
    /* 0000 */ unsigned int flags;
    /* 0004 */ TigVideoBuffer* video_buffer;
    /* 0008 */ TigRect rect;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
} S5C9228_F180;

static_assert(sizeof(S5C9228_F180) == 0x20, "wrong size");

typedef struct S5C9228 {
    /* 0000 */ unsigned int flags;
    /* 0004 */ TigRect rect;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
    /* 0040 */ int field_40;
    /* 0044 */ int field_44;
    /* 0048 */ void(*field_48)();
    /* 004C */ void(*field_4C)(TigRect* rect);
    /* 0050 */ void(*field_50)(int direction);
    /* 0054 */ int field_54;
    /* 0058 */ int field_58;
    /* 005C */ int field_5C;
    /* 0060 */ int field_60;
    /* 0064 */ int field_64;
    /* 0068 */ char field_68[TIG_MAX_PATH];
    /* 016C */ int field_16C;
    /* 0170 */ int field_170;
    /* 0174 */ int field_174;
    /* 0178 */ int field_178;
    /* 017C */ int field_17C;
    /* 0180 */ S5C9228_F180* field_180;
    /* 0184 */ int field_184;
    /* 0188 */ WmapNote* notes;
    /* 018C */ int field_18C;
    /* 0190 */ int* num_notes;
    /* 0194 */ int field_194;
    /* 0198 */ int field_198;
    /* 019C */ int field_19C;
    /* 01A0 */ int field_1A0;
    /* 01A4 */ char str[260];
    /* 02A8 */ TigRect field_2A8;
    /* 02B8 */ TigVideoBuffer* video_buffer;
    /* 02BC */ void(*field_2BC)(int x, int y, int* coords);
} S5C9228;

static_assert(sizeof(S5C9228) == 0x2C0, "wrong size");

typedef struct S64E048 {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int64_t field_10;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
} S64E048;

static_assert(sizeof(S64E048) == 0x20, "wrong size");

typedef struct S64E408 {
    /* 0000 */ S64E048 field_0[30];
    /* 03C0 */ int field_3C0;
    /* 03C4 */ int field_3C4;
    /* 03C8 */ int field_3C8;
    /* 03CC */ int field_3CC;
} S64E408;

static_assert(sizeof(S64E408) == 0x3D0, "wrong size");

static void sub_560150();
static void sub_562FE0(int a1);

// 0x5C9160
static S5C9160 stru_5C9160[8] = {
    { 142, 0, 0, 0, 0, 1 },
    { 144, 0, 0, 0, 0, 1 },
    { 141, 0, 0, 0, 0, 1 },
    { 143, 0, 0, 0, 0, 1 },
    { 204, 0, 0, 0, 0, 1 },
    { 205, 0, 0, 0, 0, 1 },
    { 206, 0, 0, 0, 0, 1 },
    { 814, 0, 0, 0, 0, 1 },
};

static void sub_560010();
static void wmap_ui_town_notes_save();
static bool wmap_ui_town_note_save(WmapNote* note, TigFile* stream);
static bool sub_560440(TigRect* rect, TigFile* stream);
static bool wmap_ui_town_note_load(WmapNote* note, TigFile* stream);
static bool sub_5606B0(TigRect* rect, TigFile* stream);
static void sub_5607E0();
static void sub_561430(long long location);
static void sub_561490(long long location, int* coords);
static void sub_5614C0(int a1, int a2);
static bool sub_5615D0(int a1);
static bool sub_5627F0(long long a1);
static bool wmTileArtLockMode(int a1, int a2);
static bool sub_5630F0(const char* path, TigVideoBuffer** video_buffer_ptr, TigRect* rect);
static bool sub_563200(int a1, int a2);
static void sub_563210(int a1, int a2);
static void sub_563270();
static void sub_5632A0(int direction);
static void sub_563300(int direction);
static void sub_563790(int a1, int a2);
static void sub_563AC0(int x, int y, int* coords);
static void sub_563B10(int x, int y, int* coords);
static void sub_563C00(int x, int y, int* coords);
static bool sub_563DE0(int* coords, int* id);
static bool sub_563E00(int* coords, int* id, int a3);
static bool sub_563F00(int* coords, int64_t* a2);
static void sub_563F90(int* coords);
static void sub_564030(WmapNote* note);
static void sub_564070(bool a1);
static void sub_5640C0(TextEdit* textedit);
static bool sub_564140(WmapNote* note);
static bool sub_564160(WmapNote* note, int a2);
static bool sub_564210(WmapNote* note);
static void sub_564320(TextEdit* textedit);
static void sub_564360(int id);
static bool sub_5643C0(const char* str);
static void sub_564940();
static void sub_564970(S64E408* a1);
static int64_t sub_564EE0(int* a1, int* a2, DateTime* datetime);
static void wmap_ui_town_notes_load();
static void sub_5650C0();
static void sub_565230();
static void sub_5657A0(TigRect* rect);
static void sub_565D00(WmapNote* note, TigRect* a2, TigRect* a3);
static void wmap_note_vbid_lock(WmapNote* note);
static void sub_565F00(TigVideoBuffer* video_buffer, TigRect* rect);
static void wmap_town_refresh_rect(TigRect* rect);
static void sub_566A80(S5C9228 *a1, TigRect *a2, TigRect *a3);
static void sub_566D10(int a1, int* a2, TigRect* a3, TigRect* a4, S5C9228* a5);

// 0x5C9220
static int dword_5C9220 = -1;

// 0x5C9A68
static TigRect stru_5C9A68 = { 150, 52, 501, 365 };

// 0x5C9A78
static TigRect stru_5C9A78 = { 662, 248, 130, 130 };

// 0x5C9A88
static TigRect stru_5C9A88 = { 391, 403, 10, 12 };

// 0x5C9A98
static TigRect stru_5C9A98[] = {
    { 707, 200, 60, 18 },
    { 707, 232, 60, 18 },
};

// 0x5C9AB8
static TigRect stru_5C9AB8 = { 25, 65, 89, 89 };

// 0x5C9AC8
static TigRect stru_5C9AC8 = { 0, 0, 200, 50 };

// 0x5C9AD8
static int dword_5C9AD8 = -1;

// 0x5C9B00
static tig_window_handle_t wmap_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x5C9B08
static TigRect stru_5C9B08 = { 0, 41, 800, 400 };

// 0x5C9B20
static TextEdit stru_5C9B20 = {
    0,
    "",
    50,
    sub_5640C0,
    sub_564320,
    NULL,
};

// 0x5C9B38
static TigRect stru_5C9B38 = { 294, 0, 0, 0 };

// 0x5C9B48
static tig_art_id_t wmap_ui_nav_cvr_art_id = TIG_ART_ID_INVALID;

// 0x5C9B50
static UiButtonInfo stru_5C9B50[2] = {
    { 20, 206, 811, TIG_BUTTON_HANDLE_INVALID },
    { 19, 308, 812, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C9B70
static UiButtonInfo stru_5C9B70 = { 375, 388, 139, TIG_BUTTON_HANDLE_INVALID };

// 0x5C9B80
static UiButtonInfo stru_5C9B80 = { 698, 353, 813, TIG_BUTTON_HANDLE_INVALID };

// 0x5C9B90
static int dword_5C9B90 = 1;

// 0x5C9B94
static int dword_5C9B94 = 5;

// 0x5C9B98
static int dword_5C9B98 = 5;

// 0x64E030
static tig_color_t dword_64E030;

// 0x64E034
static tig_color_t dword_64E034;

// 0x64E03C
static tig_color_t dword_64E03C;

// 0x64E048
static S64E408 stru_64E048[2];

// 0x64E7F0
static mes_file_handle_t wmap_ui_worldmap_mes_file;

// 0x64E7F4
static TigVideoBuffer* dword_64E7F4;

// 0x64FBB0
static TigRect stru_64FBB0;

// 0x64FBD8
static WmapNote stru_64FBD8[200];

// 0x65E968
static int dword_65E968;

// 0x65E970
static tig_color_t dword_65E970;

// 0x65E974
static tig_color_t dword_65E974;

// 0x65E978
static WmapNote wmap_ui_town_notes[200];

// 0x66D6F8
static int dword_66D6F8;

// 0x66D6FC
static mes_file_handle_t dword_66D6FC;

// 0x66D70C
static int dword_66D70C;

// 0x66D710
static int dword_66D710;

// 0x66D714
static int dword_66D714;

// 0x66D718
static WmapNote stru_66D718;

// 0x66D850
static int64_t qword_66D850;

// 0x66D860
static bool wmap_ui_initialized;

// 0x66D864
static bool wmap_ui_created;

// 0x66D868
static int dword_66D868;

// 0x66D86C
static int dword_66D86C;

// 0x66D870
static int wmap_ui_town_notes_cnt;

// 0x66D874
static int dword_66D874;

// 0x66D878
static int dword_66D878;

// 0x66D87C
static int dword_66D87C;

// 0x66D880
static int dword_66D880;

// 0x66D888
static int64_t qword_66D888;

// 0x66D890
static int dword_66D890;

// 0x66D894
static int dword_66D894;

// 0x66D898
static int dword_66D898;

// 0x66D89C
static int dword_66D89C;

// 0x66D8AC
static int dword_66D8AC;

// 0x66D8B0
static int dword_66D8B0;

// 0x66D8B4
static int dword_66D8B4;

// 0x66D8B8
static int dword_66D8B8;

// 0x66D8BC
static char byte_66D8BC[256];

// 0x66D9BC
static bool dword_66D9BC;

// 0x66D9C0
static const char* wmap_ui_action;

// 0x66D9C4
static bool dword_66D9C4;

// 0x66D9C8
static bool dword_66D9C8;

// 0x66D9D4
static WmapNote* dword_66D9D4;

static S5C9228 s = {
    6,
    { 150, 52, 501, 365 },
    0x96,
    0x34,
    0x1F5,
    0x16D,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    sub_563270,
    sub_5657A0,
    sub_5632A0,
    -1,
    0,
    0,
    0,
    0,
    { 0 },
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    stru_64FBD8,
    200,
    &dword_66D86C,
    200,
    -1,
    0,
    0,
    { 0 },
    { 0 },
    NULL,
    sub_563AC0,
};

// NOTE: Rare case - this array is put behind uninitialized stuff because it
// obtains references to many internal state variables.
//
// 0x5C9228
static S5C9228 stru_5C9228[3] = {
    {
        6,
        { 150, 52, 501, 365 },
        0x96,
        0x34,
        0x1F5,
        0x16D,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        sub_563270,
        sub_5657A0,
        sub_5632A0,
        -1,
        0,
        0,
        0,
        0,
        { 0 },
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        stru_64FBD8,
        200,
        &dword_66D86C,
        200,
        -1,
        0,
        0,
        { 0 },
        { 0 },
        NULL,
        sub_563AC0,
    },
    {
        0,
        { 218, 52, 365, 365 },
        0xDA,
        0x34,
        0x16D,
        0x16D,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        sub_565230,
        NULL,
        NULL,
        -1,
        0,
        0,
        0,
        0,
        { 0 },
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        stru_64FBD8,
        200,
        &dword_66D86C,
        200,
        -1,
        0,
        0,
        { 0 },
        { 0 },
        NULL,
        sub_563B10,
    },
    {
        4,
        { 150, 52, 501, 365 },
        0x96,
        0x34,
        0x1F5,
        0x16D,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        sub_563270,
        wmap_town_refresh_rect,
        sub_563300,
        -1,
        0,
        0,
        0,
        0,
        { 0 },
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        wmap_ui_town_notes,
        200,
        &wmap_ui_town_notes_cnt,
        200,
        -1,
        0,
        0,
        { 0 },
        { 0 },
        NULL,
        sub_563C00,
    }
};

// 0x55F8D0
bool wmap_ui_init(GameInitInfo* init_info)
{
    MesFileEntry mes_file_entry;
    TigArtFrameData art_frame_data;
    TigArtAnimData art_anim_data;
    tig_art_id_t art_id;
    int index;

    (void)init_info;

    if (!mes_load("mes\\WorldMap.mes", &wmap_ui_worldmap_mes_file)) {
        tig_debug_printf("wmap_ui_init: ERROR: failed to load msg file: mes\\WorldMap.mes!");
        exit(EXIT_FAILURE);
    }

    mes_file_entry.num = 499;
    if (mes_search(wmap_ui_worldmap_mes_file, &mes_file_entry)) {
        mes_get_msg(wmap_ui_worldmap_mes_file, &mes_file_entry);
        wmap_ui_action = mes_file_entry.str;
    }

    dword_66D868 = 0;
    dword_66D8AC = 0;

    if (tig_art_interface_id_create(217, 0, 0, 0, &wmap_ui_nav_cvr_art_id) != TIG_OK
        || tig_art_frame_data(wmap_ui_nav_cvr_art_id, &art_frame_data) != TIG_OK) {
        tig_debug_printf("WMapUI: ERROR: Can't build button cover art info!\n");
        return false;
    }

    dword_66D8B4 = art_frame_data.width;
    dword_66D8B8 = art_frame_data.height;
    dword_65E970 = tig_color_make(50, 160, 50);
    dword_65E974 = tig_color_make(160, 50, 50);
    dword_64E030 = tig_color_make(50, 50, 235);
    dword_64E034 = tig_color_make(50, 50, 110);

    for (index = 0; index < 3; index++) {
        if (stru_5C9228[index].rect.y > stru_5C9B08.y) {
            stru_5C9228[index].rect.y -= stru_5C9B08.y;
        }
    }

    stru_5C9A68.y -= stru_5C9B08.y;
    stru_5C9A98[0].y -= stru_5C9B08.y;
    stru_5C9A98[1].y -= stru_5C9B08.y;
    stru_5C9A78.y -= stru_5C9B08.y;
    stru_5C9AB8.y -= stru_5C9B08.y;
    dword_64E03C = tig_color_make(0, 0, 255);

    for (index = 0; index < 8; index++) {
        stru_5C9160[index].field_14 = 1;
        tig_art_interface_id_create(stru_5C9160[index].data.num, 0, 0, 0, &art_id);
        stru_5C9160[index].data.art_id = art_id;

        if (tig_art_anim_data(art_id, &art_anim_data) != TIG_OK) {
            return false;
        }

        if (tig_art_frame_data(art_id, &art_frame_data) == TIG_OK) {
            stru_5C9160[index].width = art_frame_data.width;
            stru_5C9160[index].height = art_frame_data.height;

            if (art_frame_data.width > dword_66D890) {
                dword_66D890 = art_frame_data.width;
            }

            if (art_frame_data.height > dword_66D894) {
                dword_66D894 = art_frame_data.height;
            }
        }
    }

    stru_5C9160[0].color = tig_color_make(255, 255, 255);
    stru_5C9160[1].color = tig_color_make(255, 0, 0);
    stru_5C9160[2].color = tig_color_make(255, 255, 0);
    stru_5C9160[3].color = tig_color_make(0, 255, 0);
    stru_5C9160[4].color = tig_color_make(150, 150, 150);
    stru_5C9160[5].color = tig_color_make(250, 250, 250);

    mes_file_entry.num = 655;
    if (mes_search(wmap_ui_worldmap_mes_file, &mes_file_entry)) {
        char* str;
        int r, g, b;

        mes_get_msg(wmap_ui_worldmap_mes_file, &mes_file_entry);
        tig_str_parse_set_separator(',');
        str = mes_file_entry.str;
        tig_str_parse_value(&str, &r);
        tig_str_parse_value(&str, &g);
        tig_str_parse_value(&str, &b);
        stru_5C9160[5].color = tig_color_make(r, g, b);
    }

    stru_5C9160[6].color = tig_color_make(150, 150, 150);

    dword_66D70C = stru_5C9AC8.y;
    stru_5C9AC8.x = dword_66D890 + 3;
    dword_66D710 = stru_5C9AC8.width + dword_66D890 + 3;
    dword_66D714 = stru_5C9AC8.height;

    if (tig_art_interface_id_create(217, 0, 0, 0, &art_id) != TIG_OK
        || tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return false;
    }

    stru_5C9B38.y = 382 - stru_5C9B08.y;
    stru_5C9B38.width = art_frame_data.width;
    stru_5C9B38.height = art_frame_data.height;

    if (tig_art_interface_id_create(196, 0, 0, 0, &art_id) != TIG_OK
        || tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return false;
    }

    stru_64FBB0.x = 0;
    stru_64FBB0.y = 0;
    stru_64FBB0.width = art_frame_data.width;
    stru_64FBB0.height = art_frame_data.height;

    if (tig_art_interface_id_create(197, 0, 0, 0, &art_id) != TIG_OK
        || tig_art_anim_data(art_id, &art_anim_data) != TIG_OK) {
        return false;
    }

    dword_66D8B0 = art_anim_data.num_frames;

    for (index = 0; index < 2; index++) {
        stru_64E048[index].field_3C0 = 0;
        stru_64E048[index].field_3C4 = 1;
        stru_64E048[index].field_3C8 = 1;
    }

    wmap_ui_initialized = true;
    return true;
}

// 0x55FF80
void wmap_ui_exit()
{
    int index;

    dword_66D868 = 0;
    dword_66D8AC = 0;

    for (index = 0; index < 3; index++) {
        // TODO: Incomplete.
    }

    sub_560150();
    sub_560010();
    mes_unload(wmap_ui_worldmap_mes_file);
    wmap_ui_initialized = false;
}

// 0x560010
void sub_560010()
{
    wmap_ui_town_notes_save();
    stru_5C9228[2].field_174 = 0;
    if (stru_5C9228[2].field_180 != NULL) {
        FREE(stru_5C9228[2].field_180);
        stru_5C9228[2].field_180 = NULL;
    }
}

// 0x560040
void wmap_ui_town_notes_save()
{
    int index;
    bool success = true;
    char path[TIG_MAX_PATH];
    TigFile* stream;

    if (dword_66D878 == 0) {
        return;
    }

    if (townmap_count() <= dword_66D878) {
        dword_66D878 = 0;
        return;
    }

    sprintf(path, "%s\\%s.tmn", "Save\\Current", townmap_name(dword_66D878));

    stream = tig_file_fopen(path, "wb");
    if (stream == NULL) {
        tig_debug_printf("wmap_ui_town_notes_save(): Error creating file %s\n", path);
        return;
    }

    if (tig_file_fwrite(&wmap_ui_town_notes_cnt, sizeof(wmap_ui_town_notes_cnt), 1, stream) == 1) {
        if (tig_file_fwrite(&(stru_5C9228[2].field_194), sizeof(stru_5C9228[2].field_194), 1, stream) == 1) {
            for (index = 0; index < wmap_ui_town_notes_cnt; index++) {
                if (!wmap_ui_town_note_save(&(wmap_ui_town_notes[index]), stream)) {
                    success = false;
                    break;
                }
            }
        } else {
            // FIXME: Should set error?
        }
    } else {
        success = false;
    }

    tig_file_fclose(stream);

    if (!success) {
        tig_debug_printf("wmap_ui_town_notes_save(): Error writing data!\n");
    }
}

// 0x560150
void sub_560150()
{
    int index;
    int j;

    for (index = 0; index < 3; index++) {
        dword_66D868 = index;

        if ((stru_5C9228[index].flags & 0x1) != 0) {
            if (tig_video_buffer_destroy(stru_5C9228[index].video_buffer) == TIG_OK) {
                stru_5C9228[index].flags &= ~0x1;
            } else {
                tig_debug_printf("WMapUI: Destroy: ERROR: Video Buffer Destroy FAILED!\n");
            }
        }

        for (j = 0; j < stru_5C9228[index].field_174; j++) {
            sub_562FE0(j);
        }
    }

    dword_66D868 = 0;
    dword_66D87C = 0;
    byte_66D8BC[0] = '\0';

    if (dword_66D9BC) {
        mes_unload(dword_66D6FC);
    }
    dword_66D9BC = false;
}

// 0x560200
void wmap_ui_reset()
{
    int index;

    if (wmap_ui_initialized) {
        sub_560F40();

        for (index = 0; index < 3; index++) {
            if (stru_5C9228[index].num_notes != NULL) {
                *stru_5C9228[index].num_notes = 0;
            }
            stru_5C9228[index].field_198 = -1;
        }

        for (index = 0; index < 2; index++) {
            stru_64E048[index].field_3C0 = 0;
            stru_64E048[index].field_3C4 = 1;
            stru_64E048[index].field_3C8 = 1;
        }

        dword_5C9AD8 = -1;
        dword_66D868 = 0;
        dword_66D8AC = 0;
        dword_65E968 = 0;
        stru_64E048[0].field_3C0 = 0;
        dword_66D9C4 = 0;
    }
}

// 0x560280
void wmap_ui_save()
{
    // TODO: Incomplete.
}

// 0x560350
bool wmap_ui_town_note_save(WmapNote* note, TigFile* stream)
{
    int size;

    if (stream == NULL) return false;

    if (tig_file_fwrite(&(note->id), sizeof(note->id), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(note->field_4), sizeof(note->field_4), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(note->x), sizeof(note->x), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(note->y), sizeof(note->y), 1, stream) != 1) return false;
    if (!sub_560440(&(note->field_18), stream)) return false;
    if (tig_file_fwrite(&(note->field_28), sizeof(note->field_28), 1, stream) != 1) return false;

    size = (int)strlen(note->str);
    if (tig_file_fwrite(&size, sizeof(size), 1, stream) != 1) return false;
    if (tig_file_fwrite(note->str, 1, size, stream) != size) return false;

    return true;
}

// 0x560440
bool sub_560440(TigRect* rect, TigFile* stream)
{
    if (tig_file_fwrite(&(rect->x), sizeof(rect->x), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(rect->y), sizeof(rect->y), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(rect->width), sizeof(rect->width), 1, stream) != 1) return false;
    if (tig_file_fwrite(&(rect->height), sizeof(rect->height), 1, stream) != 1) return false;

    return true;
}

// 0x5604B0
void wmap_ui_load()
{
    // TODO: Incomplete.
}

// 0x5605C0
bool wmap_ui_town_note_load(WmapNote* note, TigFile* stream)
{
    int size;

    if (stream == NULL) return false;

    if (tig_file_fread(&(note->id), sizeof(note->id), 1, stream) != 1) return false;
    if (tig_file_fread(&(note->field_4), sizeof(note->field_4), 1, stream) != 1) return false;
    if (tig_file_fread(&(note->x), sizeof(note->x), 1, stream) != 1) return false;
    if (tig_file_fread(&(note->y), sizeof(note->y), 1, stream) != 1) return false;
    if (!sub_5606B0(&(note->field_18), stream)) return false;
    if (tig_file_fread(&(note->field_28), sizeof(note->field_28), 1, stream) != 1) return false;
    if (tig_file_fread(&size, sizeof(size), 1, stream) != 1) return false;

    memset(note->str, 0, 248);
    note->str[248] = '\0';
    note->str[249] = '\0';
    if (tig_file_fwrite(note->str, 1, size, stream) != size) return false;

    return true;
}

// 0x5606B0
bool sub_5606B0(TigRect* rect, TigFile* stream)
{
    if (tig_file_fread(&(rect->x), sizeof(rect->x), 1, stream) != 1) return false;
    if (tig_file_fread(&(rect->y), sizeof(rect->y), 1, stream) != 1) return false;
    if (tig_file_fread(&(rect->width), sizeof(rect->width), 1, stream) != 1) return false;
    if (tig_file_fread(&(rect->height), sizeof(rect->height), 1, stream) != 1) return false;

    return true;
}

// 0x560720
void sub_560720()
{
    dword_66D9C4 = 1;
    sub_57D370(13, 0, 4000);
}

// 0x560740
int sub_560740()
{
    return dword_66D9C4;
}

// 0x560750
void sub_560750()
{
    dword_66D9C4 = 0;
}

// 0x560760
void sub_560760()
{
    dword_5C9220 = -1;
    dword_66D9C8 = 0;
    qword_66D888 = 0;
    sub_5607E0();
}

// 0x560790
void sub_560790(int64_t a1, int a2)
{
    qword_66D888 = a1;
    dword_65E968 = 0;
    stru_64E048[0].field_3C0 = 0;
    dword_66D9C8 = 1;
    dword_66D880 = 0;
    dword_5C9220 = a2;
    sub_5607E0();
}

// 0x5607E0
void sub_5607E0()
{
    // TODO: Incomplete.
}

// 0x560AA0
void wmap_load_worldmap_info()
{
    // TODO: Incomplete.
}

// 0x560EE0
void sub_560EE0()
{
}

// 0x560EF0
void sub_560EF0()
{
    dword_66D880 = 0;
    dword_66D874 = sub_4BE380(sub_4CFC50(obj_field_int64_get(player_get_pc_obj(), OBJ_F_LOCATION)));
    if (sub_40FF40() == dword_66D87C) {
        if (dword_66D874 == 0) {
            dword_66D880 = 1;
        }
    }
}

// 0x560F40
void sub_560F40()
{
    int64_t pc_obj;
    int index;

    if (!wmap_ui_created) {
        return;
    }

    pc_obj = player_get_pc_obj();
    if (pc_obj != OBJ_HANDLE_NULL
        && (obj_field_int32_get(pc_obj, OBJ_F_FLAGS) & OF_OFF) != 0) {
        sub_45E1E0(pc_obj);
        sub_4AA580(pc_obj);
    }

    if (sub_551A80(false) && wmap_ui_created) {
        sub_564070(false);
        sub_5615D0(0);
        sub_550DA0(0, NULL);
        sub_57D620();
        sub_54AC70(&stru_5C9B70);
        intgame_big_window_unlock();

        wmap_ui_window = TIG_WINDOW_HANDLE_INVALID;
        for (index = 0; index < 8; index++) {
            tig_font_destroy(stru_5C9160[index].font);
        }
        tig_video_buffer_destroy(dword_64E7F4);
        wmap_ui_created = 0;
        qword_66D888 = OBJ_HANDLE_NULL;
        dword_5C9220 = -1;
        sub_560150();
        sub_560010();
    }
}

// 0x560F90
int wmap_ui_is_created()
{
    return wmap_ui_created;
}

// 0x560FA0
void wmap_ui_create()
{
    // TODO: Incomplete.
}

// 0x561430
void sub_561430(long long location)
{
    int coords[2];
    int v1;
    int v2;

    sub_561490(location, coords);

    if (coords[0] > 99999 || coords[0] == 0) {
        v1 = 200;
    }

    if (coords[1] > 99999 || coords[1] == 0) {
        v2 = 180;
    }

    sub_5614C0(v1, v2);
}

// 0x561490
void sub_561490(long long location, int* coords)
{
    coords[0] = dword_66D6F8 - ((location >> 6) & 0x3FFFFFF);
    coords[1] = (location >> 32) >> 6;
}

// 0x5614C0
void sub_5614C0(int a1, int a2)
{
    stru_5C9228[dword_66D868].field_3C = a1;
    stru_5C9228[dword_66D868].field_40 = a2;
}

// 0x5614F0
bool sub_5614F0()
{
    if (dword_66D868) {
        return false;
    }

    if (sub_40FF40() == sub_40FF50(MAP_TYPE_START_MAP)) {
        return false;
    }

    return true;
}

// 0x5615D0
bool sub_5615D0(int a1)
{
    // TODO: Incomplete.
}

// 0x561800
void sub_561800()
{
    // TODO: Incomplete.
}

// 0x561860
void sub_561860(long long a1)
{
    TeleportData teleport_data;

    sector_flush(0);

    teleport_data.flags = 0x20;
    teleport_data.obj = player_get_pc_obj();
    teleport_data.loc = a1;
    teleport_data.map = sub_40FF50(MAP_TYPE_START_MAP);
    sub_4D3380(&teleport_data);
}

// 0x5618B0
void sub_5618B0()
{
    // TODO: Incomplete.
}

// 0x5627B0
bool sub_5627B0(int a1)
{
    long long v1;

    if (dword_66D868 == 2) {
        return 1;
    }

    sub_561800(a1, &v1);
    return sub_5627F0(v1);
}

// 0x5627F0
bool sub_5627F0(long long a1)
{
    return true;
}

// 0x562800
void sub_562800()
{
    // TODO: Incomplete.
}

// 0x562880
void sub_562880()
{
    // TODO: Incomplete.
}

// 0x562A20
void sub_562A20()
{
    // TODO: Incomplete.
}

// 0x562AF0
void sub_562AF0()
{
    // TODO: Incomplete.
}

// 0x562B70
void sub_562B70()
{
    // TODO: Incomplete.
}

// 0x562D80
void wmap_load_townmap_info()
{
    // TODO: Incomplete.
}

// 0x562F90
void sub_562F90()
{
    // TODO: Incomplete.
}

// 0x562FA0
void sub_562FA0(int a1)
{
    wmTileArtLockMode(dword_66D868, a1);
}

// 0x562FC0
void sub_562FC0(int a1)
{
    sub_563200(dword_66D868, a1);
}

// 0x562FE0
void sub_562FE0(int a1)
{
    sub_563210(dword_66D868, a1);
}

// 0x563000
bool wmTileArtLockMode(int a1, int a2)
{
    char path[TIG_MAX_PATH];

    if ((stru_5C9228[a1].field_180[a2].flags & 0x1) == 0) {
        if (a1 == 2) {
            snprintf(path, sizeof(path),
                "townmap\\%s\\%s%06d.bmp",
                stru_5C9228[a1].field_68,
                stru_5C9228[a1].field_68,
                a2);
            if (!tig_file_exists(path, NULL)) {
                snprintf(path, sizeof(path),
                    "townmap\\%s\\%s%06d.bmp",
                    stru_5C9228[a1].field_68,
                    stru_5C9228[a1].field_68,
                    0);
            }
        } else {
            snprintf(path, sizeof(path),
                "%s%03d",
                stru_5C9228[a1].field_68,
                a2 + 1);
        }

        if (sub_5630F0(path, &(stru_5C9228[a1].field_180[a2].video_buffer), &(stru_5C9228[a1].field_180[a2].rect))) {
            stru_5C9228[a1].field_180[a2].flags |= 0x1;
            stru_5C9228[a1].field_184++;
        } else {
            tig_debug_printf("WMapUI: Blit: ERROR: Bmp Load Failed!\n");
        }
    }

    return true;
}

// 0x5630F0
bool sub_5630F0(const char* path, TigVideoBuffer** video_buffer_ptr, TigRect* rect)
{
    TigBmp bmp;
    TigVideoBufferData video_buffer_data;

    if (dword_66D868 != 2) {
        snprintf(bmp.name, sizeof(bmp.name), "WorldMap\\%s.bmp", path);
        path = bmp.name;
    }

    if (tig_video_buffer_load_from_bmp(path, video_buffer_ptr, 0x1) != TIG_OK) {
        tig_debug_printf("WMapUI: Pic [%s] FAILED to load!\n", path);
        return false;
    }

    if (tig_video_buffer_data(*video_buffer_ptr, &video_buffer_data) != TIG_OK) {
        return false;
    }

    rect->x = 0;
    rect->y = 0;
    rect->width = video_buffer_data.width;
    rect->height = video_buffer_data.height;

    return true;
}

// 0x563200
bool sub_563200(int a1, int a2)
{
    return true;
}

// 0x563210
void sub_563210(int a1, int a2)
{
    if ((stru_5C9228[a1].field_180[a2].flags & 0x1) != 0) {
        if (tig_video_buffer_destroy(stru_5C9228[a1].field_180[a2].video_buffer) != TIG_OK) {
            tig_debug_printf("WMapUI: Destroy: ERROR: Video Buffer Destroy FAILED!\n");
            return;
        }

        stru_5C9228[a1].field_180[a2].flags &= ~0x1;
        stru_5C9228[a1].field_184--;
    }
}

// 0x563270
void sub_563270()
{
    if (stru_5C9228[dword_66D868].field_4C != NULL) {
        stru_5C9228[dword_66D868].field_4C(&(stru_5C9228[dword_66D868].rect));
    }
}

// 0x5632A0
void sub_5632A0(int direction)
{
    switch (direction) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if (stru_5C9228[dword_66D868].field_4C != NULL) {
            stru_5C9228[dword_66D868].field_4C(&(stru_5C9228[dword_66D868].rect));
        }
        break;
    default:
        tig_debug_printf("WMapUI: Error: Refresh Scroll Direction out of bounds!\n");
        break;
    }
}

// 0x563300
void sub_563300(int direction)
{
    // TODO: Incomplete.
}

// 0x563590
void sub_563590()
{
    // TODO: Incomplete.
}

// 0x563610
void sub_563610()
{
    // TODO: Incomplete.
}

// 0x563750
void sub_563750(int a1)
{
    sub_563790(a1, 4);
}

// 0x563770
void sub_563770(int a1)
{
    if (dword_66D8AC != 2) {
        sub_563790(a1, 8);
    }
}

// 0x563790
void sub_563790(int a1, int a2)
{
    // TODO: Incomplete.
}

// 0x563AC0
void sub_563AC0(int x, int y, int* coords)
{
    coords[0] = x + stru_5C9228[dword_66D868].field_34 - stru_5C9228[dword_66D868].rect.x;
    coords[1] = y + stru_5C9228[dword_66D868].field_38 - stru_5C9228[dword_66D868].rect.y - stru_5C9B08.y;
}

// 0x563B10
void sub_563B10(int x, int y, int* coords)
{
    // TODO: Incomplete.
}

// 0x563C00
void sub_563C00(int x, int y, int* coords)
{
    coords[0] = x + stru_5C9228[dword_66D868].field_34 - stru_5C9228[dword_66D868].rect.x;
    coords[1] = y + stru_5C9228[dword_66D868].field_38 - stru_5C9228[dword_66D868].rect.y - stru_5C9B08.y;
}

// 0x563C60
void sub_563C60()
{
    // TODO: Incomplete.
}

// 0x563D50
void sub_563D50(WmapNote* note)
{
    note->field_18.x = note->x;
    note->field_18.y = note->y;
    note->field_18.width = dword_66D890 + 203;
    note->field_18.height = 50;
}

// 0x563D80
void sub_563D80(int a1, int a2)
{
}

// 0x563D90
WmapNote* sub_563D90(int id)
{
    int index;

    for (index = 0; index < *stru_5C9228[dword_66D868].num_notes; index++) {
        if (stru_5C9228[dword_66D868].notes[index].id == id) {
            return &(stru_5C9228[dword_66D868].notes[index]);
        }
    }

    return NULL;
}

// 0x563DE0
bool sub_563DE0(int* coords, int* id)
{
    return sub_563E00(coords, id, dword_66D868);
}

// 0x563E00
bool sub_563E00(int* coords, int* id, int a3)
{
    // TODO: Incomplete.
}

// 0x563F00
bool sub_563F00(int* coords, int64_t* a2)
{
    int64_t v1;
    int64_t pc_location;

    if (a2 == NULL) {
        return false;
    }

    sub_561800(coords, &v1);
    pc_location = obj_field_int64_get(player_get_pc_obj(), OBJ_F_LOCATION);
    if (sub_4B96F0(v1, pc_location) < qword_66D850) {
        sub_561490(pc_location, coords);
        *a2 = pc_location;
        return true;
    }

    *a2 = 0;
    return false;
}

// 0x563F90
void sub_563F90(int* coords)
{
    stru_66D718.str[0] = '\0';
    stru_66D718.field_28 = dword_66D89C;
    stru_66D718.x = coords[0];
    stru_66D718.y = coords[1];
    sub_563D50(&stru_66D718);
    stru_66D718.field_10 = 0;
    sub_563F00(&(stru_66D718.x), &(stru_66D718.field_10));
    sub_564030(&stru_66D718);
}

// 0x564000
void sub_564000(int a1)
{
    int v1[3];

    v1[0] = 0;
    v1[1] = 1;
    v1[2] = 2;
    dword_66D89C = v1[a1];
}

// 0x564030
void sub_564030(WmapNote* note)
{
    sub_5506C0(6);
    dword_66D9D4 = note;
    stru_5C9B20.buffer = note->str;
    textedit_ui_focus(&stru_5C9B20);
    dword_66D898 = 1;
    sub_552160(note->str, stru_5C9160[0].font);
}

// 0x564070
void sub_564070(bool a1)
{
    dword_66D9D4 = 0;
    stru_66D718.str[0] = '\0';
    stru_66D718.id = -1;
    dword_66D898 = 0;
    textedit_ui_unfocus(&stru_5C9B20);
    stru_5C9B20.buffer = NULL;
    if (a1) {
        sub_552160(" ", stru_5C9160[0].font);
    }
}

// 0x5640C0
void sub_5640C0(TextEdit* textedit)
{
    bool v1;

    if (*textedit->buffer != '\0' && sub_5643C0(textedit->buffer)) {
        if (textedit->buffer == stru_66D718.str) {
            dword_66D9D4->field_4 = 0;
            v1 = sub_564140(dword_66D9D4);
        } else {
            v1 = true;
        }
    } else {
        v1 = sub_564210(dword_66D9D4);
    }

    sub_564070(1);

    if (v1) {
        stru_5C9228[dword_66D868].field_48();
    }
}

// 0x564140
bool sub_564140(WmapNote* note)
{
    return sub_564160(note, dword_66D868);
}

// 0x564160
bool sub_564160(WmapNote* note, int a2)
{
    int note_index;

    if (stru_5C9228[a2].notes == NULL
        || *stru_5C9228[a2].num_notes >= 199) {
        return false;
    }

    note_index = *stru_5C9228[a2].num_notes;

    note->id = stru_5C9228[a2].field_194++;
    note->field_4 &= ~0x4;
    stru_5C9228[a2].notes[note_index] = *note;
    sub_563D50(&(stru_5C9228[a2].notes[note_index]));
    (*stru_5C9228[a2].num_notes)++;
    sub_563D80(stru_5C9228[a2].notes[note_index].id, a2);

    return true;
}

// 0x564210
bool sub_564210(WmapNote* note)
{
    // TODO: Incomplete.
}

// 0x5642E0
void sub_5642E0()
{
}

// 0x5642F0
void sub_5642F0(WmapNote* note)
{
    if ((note->field_4 & 0x4) != 0) {
        tig_video_buffer_destroy(note->video_buffer);
        note->field_4 &= ~0x4;
    }
}

// 0x564320
void sub_564320(TextEdit* textedit)
{
    if (*textedit->buffer != '\0') {
        sub_552160(textedit->buffer, stru_5C9160[0].font);
    } else {
        sub_552160(" ", stru_5C9160[0].font);
    }
}

// 0x564360
void sub_564360(int id)
{
    S5C9228* v1;
    int index;
    int cnt;

    v1 = &(stru_5C9228[dword_66D868]);
    cnt = *v1->num_notes;
    if (v1->notes != NULL) {
        for (index = 0; index < cnt; index++) {
            if (v1->notes[index].id == id) {
                break;
            }
        }

        if (index < cnt && (v1->notes[index].field_4 & 0x2) == 0) {
            sub_564030(&(v1->notes[index]));
        }
    }
}

// 0x5643C0
bool sub_5643C0(const char* str)
{
    while (*str != '\0') {
        if (*str != ' ') {
            return true;
        }
        str++;
    }

    return false;
}

// 0x5643E0
void sub_5643E0()
{
    // TODO: Incomplete.
}

// 0x564780
void sub_564780()
{
    // TODO: Incomplete.
}

// 0x564830
void sub_564830()
{
}

// 0x564840
void sub_564840()
{
    // TODO: Incomplete.
}

// 0x5648E0
void sub_5648E0()
{
    // TODO: Incomplete.
}

// 0x564940
void sub_564940()
{
    if (dword_5C9AD8 != -1 && dword_66D868 != 2) {
        sub_564970(&(stru_64E048[dword_5C9AD8]));
    }
}

// 0x564970
void sub_564970(S64E408* a1)
{
    // TODO: Incomplete.
}

// 0x5649C0
void sub_5649C0()
{
}

// 0x5649D0
void sub_5649D0()
{
}

// 0x5649F0
void sub_5649F0()
{
    // TODO: Incomplete.
}

// 0x564A70
void sub_564A70(long long a1, long long a2)
{
    int v1;

    v1 = sub_4CB4D0(a2, 0);
    if (v1 > 0) {
        if (!sub_4CAF50(a1, v1)) {
            if (sub_4CB100(a1) == v1) {
                sub_4CB160(a1);
            }
            sub_4CAFD0(a1, v1);
            sub_562800(v1);
        }

        if (sub_4CB100(a1) == v1) {
            sub_4CB160(a1);
        }
    }
}

// 0x564AF0
void sub_564AF0(int64_t a1)
{
    // TODO: Incomplete.
}

// 0x564C20
bool wmap_ui_bkg_process_callback(TimeEvent* timeevent)
{
    // TODO: Incomplete.
}

// 0x564E30
void sub_564E30()
{
    // TODO: Incomplete.
}

// 0x564EE0
int64_t sub_564EE0(int* a1, int* a2, DateTime* datetime)
{
    int64_t v1;
    int64_t v2;
    int v3;

    sub_561800(a1, &v1);
    sub_561800(a2, &v2);
    v3 = (int)(sub_4B96F0(v1, v2) / 64);

    if (datetime != NULL) {
        sub_45A950(datetime, 3600000 * v3);
    }

    return v3;
}

// 0x564F60
void sub_564F60(long long a1, long long a2)
{
    dword_66D874 = sub_4BE380(a2);
    if (dword_66D874) {
        sub_564A70(a1, sub_4CFC90(a2));
        wmap_ui_town_notes_load();
        sub_4607B0(2);
    } else {
        sub_5650C0();
        sub_4607B0(3);
    }
    sub_560EF0();
}

// 0x564FD0
void wmap_ui_town_notes_load()
{
    char path[TIG_MAX_PATH];
    TigFile* stream;
    int index;
    bool success = true;

    if (dword_66D878 != dword_66D874) {
        sub_5650C0();
    }
    dword_66D878 = dword_66D874;

    if (dword_66D878 != 0) {
        sprintf(path,
            "%s\\%s.tmn",
            "Save\\Current",
            townmap_name(dword_66D878));

        stream = tig_file_fopen(path, "rb");
        if (stream == NULL) {
            return;
        }

        if (tig_file_fread(&wmap_ui_town_notes_cnt, sizeof(wmap_ui_town_notes_cnt), 1, stream) == 1
            && tig_file_fread(&(stru_5C9228[2].field_194), sizeof(stru_5C9228[2].field_194), 1, stream) == 1) {
            for (index = 0; index < wmap_ui_town_notes_cnt; index++) {
                if (!wmap_ui_town_note_load(&(wmap_ui_town_notes[index]), stream)) {
                    success = false;
                    break;
                }
            }
        } else {
            success = false;
        }

        tig_file_fclose(stream);

        if (!success) {
            tig_debug_printf("wmap_ui_town_notes_load(): Warning reading data!\n");
        }
    }
}

// 0x5650C0
void sub_5650C0()
{
    if (dword_66D878) {
        wmap_ui_town_notes_save();
    }
    dword_66D878 = 0;
    wmap_ui_town_notes_cnt = 0;
}

// 0x5650E0
void sub_5650E0()
{
    // TODO: Incomplete.
}

// 0x565130
void sub_565130()
{
}

// 0x565140
bool sub_565140()
{
    if (!sub_424070(player_get_pc_obj(), 3, 0, 1)) {
        return false;
    }

    sub_457450(player_get_pc_obj());

    return true;
}

// 0x565170
void sub_565170()
{
    // TODO: Incomplete.
}

// 0x565230
void sub_565230()
{
    // TODO: Incomplete.
}

// 0x5656B0
void sub_5656B0()
{
    // TODO: Incomplete.
}

// 0x5657A0
void sub_5657A0(TigRect* rect)
{
    // TODO: Incomplete.
}

// 0x565CF0
bool sub_565CF0(WmapNote* note)
{
    return true;
}

// 0x565D00
void sub_565D00(WmapNote* note, TigRect* a2, TigRect* a3)
{
    TigWindowBlitInfo win_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    int x;
    int y;
    int dx;
    int dy;

    wmap_note_vbid_lock(note);

    if ((note->field_4 & 0x4) != 0) {
        x = 0;
        y = 0;
        dx = a2->x + note->x - stru_5C9228[dword_66D868].field_34;
        dy = a2->y + note->y - stru_5C9228[dword_66D868].field_38;

        if (dword_66D868 == 0 && note->id <= sub_4CAE80()) {
            sub_4CAF00(note->id, &x, &y);
        }

        x += dx - 10;
        y += dy - 10;

        dst_rect.x = x;
        dst_rect.y = y;
        dst_rect.width = note->field_18.width;
        dst_rect.height = note->field_18.height;

        if (tig_rect_intersection(&dst_rect, a3, &dst_rect) == TIG_OK) {
            src_rect.x = dst_rect.x - x;
            src_rect.y = dst_rect.y - y;
            src_rect.width = dst_rect.width;
            src_rect.height = dst_rect.height;

            win_blit_info.type = TIG_WINDOW_BLIT_VIDEO_BUFFER_TO_WINDOW;
            win_blit_info.vb_blit_flags = 0;
            win_blit_info.src_video_buffer = note->video_buffer;
            win_blit_info.src_rect = &src_rect;
            win_blit_info.dst_window_handle = wmap_ui_window;
            win_blit_info.dst_rect = &dst_rect;
            tig_window_blit(&win_blit_info);
        }
    }
}

// 0x565E40
void wmap_note_vbid_lock(WmapNote* note)
{
    TigVideoBufferCreateInfo vb_create_info;
    TigRect dirty_rect;

    if ((note->field_4 & 0x4) == 0) {
        vb_create_info.flags = TIG_VIDEO_BUFFER_LOCKED | TIG_VIDEO_BUFFER_VIDEO_MEMORY;
        vb_create_info.width = dword_66D890 + 203;
        vb_create_info.height = 50;
        vb_create_info.background_color = dword_64E03C;
        vb_create_info.color_key = dword_64E03C;
        if (tig_video_buffer_create(&vb_create_info, &(note->video_buffer)) == TIG_OK) {
            tig_debug_printf("wmap_note_vbid_lock: ERROR: tig_video_buffer_create failed!\n");
            exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
        }

        note->field_4 |= 0x4;

        sub_565F00(note->video_buffer, &(note->field_18));
        tig_font_push(stru_5C9160[note->field_28].font);
        tig_font_write(note->video_buffer, note->str, &stru_5C9AC8, &dirty_rect);
        tig_font_pop();
    }
}

// 0x565F00
void sub_565F00(TigVideoBuffer* video_buffer, TigRect* rect)
{
    // TODO: Incomplete.
}

// 0x566130
void wmap_town_refresh_rect(TigRect* rect)
{
    // TODO: Incomplete.
}

// 0x566A80
void sub_566A80(S5C9228 *a1, TigRect *a2, TigRect *a3)
{
    int v1;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigLine line;
    TigDrawLineModeInfo line_mode_info;
    TigDrawLineStyleInfo line_style_info;
    int index;
    int x1;
    int y1;
    int x2;
    int y2;

    v1 = dword_66D868 == 2 ? 1 : 0;
    if (stru_64E048[v1].field_3C0 <= 0) {
        return;
    }

    line_mode_info.mode = TIG_DRAW_LINE_MODE_NORMAL;
    line_mode_info.thickness = 1;
    tig_draw_set_line_mode(&line_mode_info);

    line_style_info.style = TIG_LINE_STYLE_DASHED;
    tig_draw_set_line_style(&line_style_info);

    x1 = a2->x + a1->field_3C - a1->field_34;
    y1 = a2->y + a1->field_40 - a1->field_38;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = stru_5C9160[0].width;
    src_rect.height = stru_5C9160[0].height;

    art_blit_info.flags = 0;
    art_blit_info.art_id = stru_5C9160[0].data.art_id;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_video_buffer = dword_64E7F4;

    for (index = 0; index < stru_64E048[v1].field_3C0; index++) {
        x2 = a2->x + stru_64E048[v1].field_0[index].field_8 - a1->field_34;
        y2 = a2->y + stru_64E048[v1].field_0[index].field_C - a1->field_38;

        line.x1 = x1;
        line.y1 = y1;
        line.x2 = x2;
        line.y2 = y2;

        if (tig_line_intersection(a3, &line) == TIG_OK) {
            tig_window_line(wmap_ui_window,
                &line,
                index == dword_5C9AD8 ? dword_64E034 : dword_65E974);
        }

        src_rect.x = x2 - stru_5C9160[6].width / 2;
        src_rect.y = y2 - stru_5C9160[6].height / 2;
        src_rect.width = stru_5C9160[6].width;
        src_rect.height = stru_5C9160[6].height;
        art_blit_info.art_id = stru_5C9160[6].data.art_id;

        if (tig_rect_intersection(&src_rect, a3, &src_rect) == TIG_OK) {
            dst_rect = src_rect;

            src_rect.x -= x2;
            src_rect.y -= y2;

            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(wmap_ui_window, &art_blit_info);
        }

        x1 = x2;
        y1 = y2;
    }
}

// 0x566CC0
void sub_566CC0(int64_t* location_ptr)
{
    if (wmap_ui_created && dword_66D8AC == 2) {
        sub_561800(&stru_5C9228[dword_66D868].field_3C, location_ptr);
    } else {
        *location_ptr = obj_field_int64_get(player_get_pc_obj(), OBJ_F_LOCATION);
    }
}

// 0x566D10
void sub_566D10(int a1, int* a2, TigRect* a3, TigRect* a4, S5C9228* a5)
{
    int dx;
    int dy;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    dx = a2[0] - src_rect.width / 2 - a5->field_34;
    dy = a2[1] - src_rect.height / 2 - a5->field_38;

    src_rect.x = a3->x + dx;
    src_rect.y = a3->y + dy;
    src_rect.width = stru_5C9160[a1].width;
    src_rect.height = stru_5C9160[a1].height;
    if (tig_rect_intersection(&src_rect, a4, &src_rect) == TIG_OK) {
        dst_rect = src_rect;

        src_rect.x -= a3->x + dx;
        src_rect.y -= a3->y + dy;

        art_blit_info.flags = 0;
        art_blit_info.art_id = stru_5C9160[a1].data.art_id;
        art_blit_info.dst_video_buffer = dword_64E7F4;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(wmap_ui_window, &art_blit_info);
    }
}
