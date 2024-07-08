#include "game/lib/newspaper.h"

#define TWENTY_FIVE 25

// 0x6876CC
static int dword_6876CC;

// TODO: Type.
// 0x6876D0
static int* off_6876D0;

// 0x4BF060
bool newspaper_init(GameContext* ctx)
{
    off_6876D0 = (int*)calloc(TWENTY_FIVE, sizeof(*off_6876D0));
    newspaper_reset();
    return true;
}

// 0x4BF080
void newspaper_reset()
{
    for (int index = 0; index < 4; index++) {
        off_6876D0[index] = -1;
    }

    off_6876D0[4] = 5000;
    dword_6876CC = 5;
}

// 0x4BF0B0
void newspaper_exit()
{
    free(off_6876D0);
}

// 0x4BF0C0
bool newspaper_load(LoadContext* ctx)
{
    // TODO: Use equation.
    if (tig_file_fread(off_6876D0, 100, 1, ctx->stream) != 1) return false;
    if (tig_file_fread(&dword_6876CC, sizeof(dword_6876CC), 1, ctx->stream) != 1) return false;

    return true;
}

// 0x4BF100
bool newspaper_save(TigFile* stream)
{
    // TODO: Use equation.
    if (tig_file_fwrite(off_6876D0, 100, 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_6876CC, sizeof(dword_6876CC), 1, stream) != 1) return false;

    return true;
}

// 0x4BF1D0
bool sub_4BF1D0(int a1)
{
    for (int index = 0; index < TWENTY_FIVE; index++) {
        if (off_6876D0[index] == a1) {
            return true;
        }
    }

    return false;
}

// 0x4BF200
int sub_4BF200(int index)
{
    return off_6876D0[index];
}
