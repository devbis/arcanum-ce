#include "game/dialog.h"

#include "game/mes.h"
#include "game/timeevent.h"

#define THIRTY_TWO 32

typedef struct DialogEntry {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
} DialogEntry;

static_assert(sizeof(Dialog) == 0x1C, "wrong size");

typedef struct Dialog {
    /* 0000 */ char path[TIG_MAX_PATH];
    /* 0104 */ int refcount;
    /* 0108 */ DateTime timestamp;
    /* 0110 */ int entries_length;
    /* 0114 */ int entries_capacity;
    /* 0118 */ DialogEntry* entries;
    /* 011C */ int field_11C:
} Dialog;

static_assert(sizeof(Dialog) == 0x120, "wrong size");

// 0x5D19F4
static mes_file_handle_t* dword_5D19F4;

// 0x5D19F8
static int dword_5D19F8;

// 0x5D19FC
static int dword_5D19FC;

// 0x5D1A00
static int dword_5D1A00;

// 0x5D1A04
static int dword_5D1A04;

// 0x5D1A08
static Dialog* dword_5D1A08;

// 0x5D1A0C
static bool dword_5D1A0C;

// 0x412D40
bool dialog_init(GameInitInfo* init_info)
{
    int index;

    (void)init_info;

    dword_5D19F4 = (mes_file_handle_t*)CALLOC(THIRTY_TWO, sizeof(mes_file_handle_t));
    for (index = 0; index < THIRTY_TWO; index++) {
        dword_5D19F4[index] = MES_FILE_HANDLE_INVALID;
    }

    return true;
}

// 0x412D80
void dialog_exit()
{
    int index;

    for (index = 0; index < THIRTY_TWO; index++) {
        if (dword_5D19F4[index] != MES_FILE_HANDLE_INVALID) {
            mes_unload(dword_5D19F4[index]);
        }
    }

    for (index = 0; index < dword_5D1A04; index++) {
        if (dword_5D1A08[index].path[0] != '\0') {
            sub_412F60(index);
        }
    }

    if (dword_5D1A04 > 0) {
        FREE(dword_5D1A08);
        dword_5D1A08 = NULL;
        dword_5D1A04 = 0;
    }
}

// 0x412E10
bool sub_412E10(const char* path, int* a2)
{
    int index;
    Dialog dialog;

    if (sub_4175D0(path, &index)) {
        dword_5D1A08[index].refcount++;
        dword_5D1A08[index].timestamp = sub_45A7C0();
    } else {
        strcpy(dialog.path, path);
        dialog.refcount = 1;
        dialog.timestamp = sub_45A7C0();
        dialog.entries_length = 0;
        dialog.entries_capacity = 0;
        dialog.entries = NULL;
        sub_417720(&dialog);

        if (dialog.entries_length == 0) {
            return false;
        }

        dword_5D1A08[index] = dialog;
        dword_5D1A00++;
    }

    *a2 = index;

    return true;
}

// 0x412F40
void sub_412F40(int index)
{
    dword_5D1A08[index].refcount--;
}

// 0x417D60
TigFile* sub_417D60(const char* fname, const char* mode)
{
    dword_5D19FC = 0;
    dword_5D19F8 = 0;
    return tig_file_fopen(fname, mode);
}

// 0x417D80
int sub_417D80(TigFile* stream)
{
    return tig_file_fclose(stream);
}

// 0x4182C0
void sub_4182C0()
{
    dword_5D1A0C = true;
}

// 0x418460
void sub_418460(int a1, int a2)
{
    int v1 = sub_445090();
    sub_4182D0(a1, a2, v1 + 1700, v1 + 1700);
}

// 0x4189C0
int sub_4189C0(const char* a1, int a2)
{
    if (a1 != NULL && a2 != 0) {
        return ((a2 & 0x7FFF) << 16) | (atoi(a1) & 0xFFFF);
    } else {
        return -1;
    }
}

// 0x418A00
int sub_418A00(int a1, int* a2, int* a3)
{
    if (a1 != -1) {
        *a2 = (a1 >> 16) & 0x7FFF;
        *a3 = a1 & 0xFFFF;
    } else {
        *a2 = 0;
        *a3 = 0;
    }
}
