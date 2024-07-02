#include "game/dialog.h"

#include <tig/tig.h>

// 0x5D19F4
static int* dword_5D19F4;

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
