#include "game/lib/sector.h"

#include <stdio.h>

#include "tig/color.h"
#include "tig/debug.h"
#include "tig/message.h"

typedef void(SectorFunc601788)();
typedef void(SectorFunc6017B4)();

typedef struct Sector601808 {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    Sector601808* next;
    int field_1C;
};

// See 0x4D13D0.
static_assert(sizeof(Sector601808) == 0x20, "wrong size");

typedef struct SectorCacheEntry {
    bool used;
};

static_assert(sizeof(SectorCacheEntry) == 0x8878, "wrong size");

// 0x5B7CD8
int dword_5B7CD8 = 1;

// 0x601780
static unsigned int dword_601780;

// 0x601788
static SectorFunc601788* dword_601788;

// 0x601790
static int dword_601790;

// TODO: Type.
// 0x601794
static void* dword_601794;

// 0x601798
static bool dword_601798;

// 0x60179C
static char* dword_60179C;

// 0x6017A0
static int64_t qword_6017A0;

// 0x6017A8
static bool sector_is_in_editor;

// 0x6017AC
static int dword_6017AC;

// 0x6017B0
static SectorCacheEntry* dword_6017B0;

// 0x6017B4
static SectorFunc6017B4* dword_6017B4;

// 0x6017B8
static GameContextF8* dword_6017B8;

// 0x6017C4
static int dword_6017C4;

// 0x6017C8
static int64_t qword_6017C8;

// 0x6017D0
static int64_t qword_6017D0;

// 0x6017D8
static int dword_6017D8;

// 0x6017E0
static int64_t qword_6017E0;

// TODO: Type.
// 0x6017E8
static void* dword_6017E8;

// TODO: Type.
// 0x6017EC
static void* dword_6017EC;

// TODO: Type.
// 0x6017F0
static void* dword_6017F0;

// 0x6017F8
static int sector_iso_window_handle;

// 0x6017FC
static char* dword_6017FC;

// 0x601800
static ViewOptions sector_view_options;

// 0x601808
static Sector601808* dword_601808;

// 0x601810
static int64_t qword_601810;

// 0x601818
static int64_t qword_601818;

// 0x601820
static int dword_601820;

// 0x601824
static bool dword_601824;

// 0x601828
static int dword_601828;

// 0x601830
static int dword_601830;

// 0x601834
static int dword_601834;

// 0x4CEF70
bool sector_init(GameContext* ctx)
{
    dword_601820 = 0;
    sector_iso_window_handle = ctx->iso_window_handle;
    dword_6017B8 = ctx->field_8;
    sector_view_options.type = VIEW_TYPE_ISOMETRIC;
    dword_601830 = 0;
    dword_6017AC = tig_color_make(255, 255, 0);
    dword_601828 = tig_color_make(255, 255, 0);
    sector_is_in_editor = ctx->editor;

    if (sector_is_in_editor) {
        dword_6017B4 = sub_4D1530;
        dword_601788 = sub_4D22E0;
    } else {
        dword_6017B4 = sub_4D1A30;
        dword_601788 = sub_4D2750;
    }

    dword_60179C = (char*)calloc(MAX_PATH, sizeof(*dword_60179C));
    dword_6017FC = (char*)calloc(MAX_PATH, sizeof(*dword_6017FC));

    // TODO: Type.
    dword_6017E8 = calloc(151, 8);
    dword_6017EC = calloc(151, 8);

    sub_4CF790(0x4000000, 0x4000000);

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

// 0x4CF2C0
void sector_resize(ResizeContext* ctx)
{
    sector_iso_window_handle = ctx->iso_window_handle;
}

// 0x4CF320
void sub_4CF320()
{
    sub_4D0B40();
    if (!sub_4038D0()) {
        dword_601834 = 0;
    }
}

// 0x4CF340
bool sector_update_view(ViewOptions* view_options)
{
    sector_view_options = *view_options;
    return true;
}

// 0x4CF790
bool sub_4CF790(int64_t a1, int64_t a2)
{
    if (a1 > 0x4000000 || a2 > 0x4000000) {
        return false;
    }

    qword_6017A0 = a1;
    qword_6017D0 = a2;

    return true;
}

// 0x4CF7E0
void sub_4CF7E0(int64_t* a1, int64_t* a2)
{
    *a1 = qword_6017A0;
    *a2 = qword_6017D0;
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
    return tig_file_info(path, NULL);
}

// 0x4D0F50
bool sub_4D0F50(int a1, int a2)
{
    sub_4D0F70();
    return sub_4D31C0(a1, a2);
}

// 0x4D0F70
void sub_4D0F70()
{
    if (dword_601794 != NULL) {
        free(dword_601794);
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

// 0x4D13A0
Sector601808* sub_4D13A0()
{
    Sector601808* node = dword_601808;
    if (node == NULL) {
        sub_4D13D0();
        node = dword_601808;
    }

    dword_601808 = node->next;
    node->next = NULL;

    return node;
}

// 0x4D13D0
void sub_4D13D0()
{
    for (int index = 0; index < 4; index++) {
        Sector601808* node = (Sector601808*)malloc(sizeof(*node));
        node->next = dword_601808;
        dword_601808 = node;
    }
}

// 0x4D2C00
void sub_4D2C00(const char* a1)
{
    if (!obj_validate_system()) {
        tig_debug_printf("Object system validate failed in %s\n", a1);
        tig_message_quit(0);
    }
}

// 0x4D2C30
void sub_4D2C30(const char* a1)
{
    if (!obj_validate_system()) {
        char text[300];
        _snprintf(text, sizeof(text) - 1, "Object system validate failed in %s", a1);
        *(text + sizeof(text) - 1) = '\0';
        MessageBoxA(NULL, text, "Execution Halting", MB_TASKMODAL | MB_ICONHAND);
        tig_message_quit(0);
    }
}

// 0x4D2D30
bool sector_cache_find_unused(unsigned int* slot)
{
    // NOTE: Unsigned comparison.
    for (unsigned int index = 0; index < dword_601780; index++) {
        if (!dword_6017B0->used) {
            *slot = index;
        }
    }

    return false;
}

// 0x4D2D70
void sub_4D2D70()
{
    char buffer[MAX_PATH];

    dword_601824 = false;
    sprintf(buffer, "%s\\134217730001.sec", dword_60179C);

    if (!tig_file_info(buffer, NULL)) {
        return;
    }

    TigFile* stream = tig_file_fopen(buffer, "rb");
    if (stream == NULL) {
        return;
    }

    do {
        if (!tig_file_fgets(buffer, sizeof(buffer), stream)) break;
        qword_601818 = _atoi64(buffer);

        if (!tig_file_fgets(buffer, sizeof(buffer), stream)) break;
        qword_6017E0 = _atoi64(buffer);

        if (!tig_file_fgets(buffer, sizeof(buffer), stream)) break;
        qword_601810 = _atoi64(buffer);

        if (!tig_file_fgets(buffer, sizeof(buffer), stream)) break;
        qword_6017C8 = _atoi64(buffer);

        dword_601824 = true;
    } while (0);

    tig_file_fclose(stream);
}

// 0x4D2EA0
void sub_4D2EA0()
{
    if (dword_6017F0 != NULL) {
        free(dword_6017F0);
    }

    dword_6017D8 = 0;
    dword_6017C4 = 0;
}

// 0x4D2F90
void sub_4D2F90()
{
    qsort(dword_6017F0, dword_6017D8, sizeof(*dword_6017F0), sub_4D2FB0);
}

// 0x4D2FB0
int sub_4D2FB0(const void* a1, const void* a2)
{
    return *(int*)a1 - *(int*)a2;
}

// 0x4D30A0
bool sub_4D30A0()
{
    char path[MAX_PATH];

    if (dword_601798) {
        if (dword_601790 != 0) {
            sprintf(path, "%s\\map.sbf", dword_6017FC);

            TigFile* stream = tig_file_fopen(path, "wb");
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

            TigFile* stream = tig_file_fopen(path, "wb");
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
    sprintf(path, "%s\\map.sbf", a2);

    if (!tig_file_info(path, NULL)) {
        sprintf(path, "%s\\map.sbf", a1);
        if (!tig_file_info(path, NULL)) {
            return false;
        }
    }

    TigFile* stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        return false;
    }

    if (tig_file_fread(&dword_601790, sizeof(dword_601790), 1, stream) != 1) {
        tig_file_fclose(stream);
        return false;
    }

    dword_601794 = malloc(sizeof(*dword_601794) * dword_601790);
    if (tig_file_fread(dword_601794, sizeof(*dword_601794), dword_601790, stream) != dword_601790) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}
