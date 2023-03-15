#include "game/lib/jumppoint.h"

#include <stdio.h>

#include "tig/art.h"
#include "tig/debug.h"

typedef struct JumpPoint {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
};

// See 0x4E3800.
static_assert(sizeof(JumpPoint) == 0x20, "wrong size");

static bool jumppoint_read_all(TigFile* stream);
static bool jumppoint_write_all(TigFile* stream);

// 0x603450
static char byte_603450[260];

static JumpPoint* jumppoints;

// 0x603558
static unsigned int dword_603558;

// 0x60355C
static GameContextF8* dword_60355C;

// 0x603560
static char byte_603560[MAX_PATH];

// 0x603668
static int dword_603668;

// 0x603670
static int dword_603670;

// 0x603674
static int jumppoint_iso_window_handle;

// 0x603678
static unsigned int dword_603678;

// 0x60367C
static int dword_60367C;

// 0x603680
static int dword_603680;

// 0x603684
static int jumppoints_count;

// 0x603688
static bool dword_603688;

// 0x4E2FB0
bool jumppoint_init(GameContext* ctx)
{
    jumppoint_iso_window_handle = ctx->iso_window_handle;
    dword_60355C = ctx->field_8;
    dword_603688 = ctx->editor;
    dword_603668 = 0;

    tig_art_interface_id_create(350, 0, 0, 0, &dword_603678);
    tig_art_interface_id_create(351, 0, 0, 0, &dword_603558);

    dword_603680 = 1;
    dword_60367C = 0;
    dword_603670 = 1;

    return true;
}

// 0x4E3020
void jumppoint_reset()
{
    jumppoint_close();
}

// 0x4E3030
void jumppoint_exit()
{
    jumppoint_close();
    dword_603680 = 0;
}

// 0x4E3040
void jumppoint_resize(ResizeContext* ctx)
{
    jumppoint_iso_window_handle = ctx->iso_window_handle;
}

// TODO: Type.
// 0x4E3050
bool sub_4E3050(void* a1)
{
    jumppoint_close();

    sprintf(byte_603560, "%s\\map.jmp", a1->field_0);
    sprintf(byte_603450, "%s\\map.jmp", a1->field_4);
    dword_60367C = 1;

    return sub_4E3270();
}

// 0x4E30A0
bool jumppoint_open(const char* a1, const char* a2)
{
    jumppoint_close();

    sprintf(byte_603560, "%s\\map.jmp", a1);
    sprintf(byte_603450, "%s\\map.jmp", a2);

    TigFile* stream = tig_file_fopen(byte_603450, "rb");
    if (stream == NULL) {
        stream = tig_file_fopen(byte_603560, "rb");
        if (stream == NULL) {
            tig_debug_printf("Jumppoint file doesn't exist [%s]\n", byte_603450);
            return false;
        }
    }

    if (!jumppoint_read_all(stream)) {
        // FIXME: Leaking open file stream.
        return false;
    }

    dword_60367C = 0;
    tig_file_fclose(stream);

    return true;
}

// 0x4E3140
void jumppoint_close()
{
    if (jumppoints != NULL) {
        free(jumppoints);
        jumppoints = NULL;
    }

    jumppoints_count = 0;

    if (dword_603688 == 1) {
        dword_60355C(0);
    }

    dword_60367C = 0;
}

// 0x4E3270
bool sub_4E3270()
{
    if (dword_60367C) {
        TigFile* stream = tig_file_fopen(byte_603450, "wb");
        if (stream == NULL) {
            return false;
        }

        if (!jumppoint_write_all(stream)) {
            tig_file_fclose(stream);
            return false;
        }

        tig_file_fclose(stream);
        dword_60367C = false;
    }

    return true;
}

// TODO: Type.
// 0x4E32D0
bool sub_4E32D0(void* a1)
{
    dword_603668 = a1->field_0;
    dword_60366C = a1->field_4;
    return true;
}

// 0x4E3800
bool jumppoint_read_all(TigFile* stream)
{
    if (tig_file_fread(&jumppoints_count, sizeof(jumppoints_count), 1, stream) != 1) {
        return false;
    }

    if (jumppoints_count != 0) {
        jumppoints = (JumpPoint*)malloc(sizeof(*jumppoints) * jumppoints_count);
        for (int index = 0; index < jumppoints_count; index++) {
            if (tig_file_fread(&(jumppoints[index]), sizeof(*jumppoints), 1, stream) != 1) {
                jumppoint_close();
                return false;
            }
        }
    }

    return true;
}

// 0x4E3890
bool jumppoint_write_all(TigFile* stream)
{
    if (tig_file_fwrite(&jumppoints_count, sizeof(jumppoints_count), 1, stream) != 1) {
        return false;
    }

    for (int index = 0; index < jumppoints_count; index++) {
        if (tig_file_fwrite(&(jumppoints[index]), sizeof(*jumppoints), 1, stream) != 1) {
            return false;
        }
    }

    return true;
}
