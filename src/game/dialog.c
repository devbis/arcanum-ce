#include "game/dialog.h"

#include <tig/tig.h>

// 0x5D19F4
static int* dword_5D19F4;

// 0x5D19F8
static int dword_5D19F8;

// 0x5D19FC
static int dword_5D19FC;

// 0x5D1A0C
static bool dword_5D1A0C;

// 0x412D40
bool dialog_init()
{
    int index;

    dword_5D19F4 = CALLOC(32, sizeof(int));
    for (index = 0; index < 32; index++) {
        dword_5D19F4[index] = -1;
    }

    return true;
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
