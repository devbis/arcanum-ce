#include "game/lib/jumppoint.h"

#include <stdio.h>

#include "tig/art.h"
#include "tig/debug.h"

typedef struct JumpPoint {
    int field_0;
    int field_4;
    int64_t location;
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
static ViewOptions jumppoint_view_options;

// 0x603670
static bool dword_603670;

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
static bool jumppoint_is_editor;

// 0x4E2FB0
bool jumppoint_init(GameContext* ctx)
{
    jumppoint_iso_window_handle = ctx->iso_window_handle;
    dword_60355C = ctx->field_8;
    jumppoint_is_editor = ctx->editor;
    jumppoint_view_options.type = VIEW_TYPE_ISOMETRIC;

    tig_art_interface_id_create(350, 0, 0, 0, &dword_603678);
    tig_art_interface_id_create(351, 0, 0, 0, &dword_603558);

    dword_603680 = 1;
    dword_60367C = 0;
    dword_603670 = true;

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

    if (jumppoint_is_editor == 1) {
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

// 0x4E32D0
bool jumppoint_update_view(ViewOptions* view_options)
{
    jumppoint_view_options = *view_options;
    return true;
}

// 0x4E32F0
bool sub_4E32F0()
{
    return dword_603670;
}

// 0x4E3300
void sub_4E3300()
{
    dword_603670 = !dword_603670;
}

// 0x4E3450
bool jumppoint_find_by_location(int64_t location, JumpPoint* jumppoint)
{
    int index;

    for (index = 0; index < jumppoints_count; index++) {
        if (jumppoints[index].location == location) {
            if (jumppoint != NULL) {
                *jumppoint = jumppoints[index];
            }
            return true;
        }
    }

    return false;
}

// 0x4E36A0
void sub_4E36A0(int jumppoint)
{
    TigRect rect;

    jumppoint_get_rect(jumppoint, &rect);
    dword_60355C(&rect);
}

// 0x4E36D0
void jumppoint_get_rect(int jumppoint, TigRect* rect)
{
    int64_t x;
    int64_t y;
    TigArtFrameData art_frame_data;

    sub_4B8680(jumppoints[jumppoint].location, &x, &y);
    if (x >= INT_MIN && x < INT_MAX
        && y >= INT_MIN && y < INT_MAX) {
        if (jumppoint_view_options.type == 1) {
            rect->x = (int)x;
            rect->y = (int)y;
            rect->width = jumppoint_view_options.zoom;
            rect->height = jumppoint_view_options.zoom;
            return;
        }

        if (tig_art_frame_data(dword_603678, &art_frame_data) == TIG_OK) {
            rect->x = (int)x - art_frame_data.hot_x + 40;
            rect->y = (int)y - (40 - art_frame_data.height) / 2 - art_frame_data.hot_y + 40;
            rect->width = art_frame_data.width;
            rect->height = art_frame_data.height;
            return;
        }
    }

    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;
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
