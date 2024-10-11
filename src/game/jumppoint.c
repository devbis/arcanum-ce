#include "game/jumppoint.h"

#include <stdio.h>

#include "game/location.h"

static void jumppoint_get_rect(int jumppoint, TigRect* rect);
static bool jumppoint_read_all(TigFile* stream);
static bool jumppoint_write_all(TigFile* stream);

// 0x603450
static char byte_603450[TIG_MAX_PATH];

static JumpPoint* jumppoints;

// 0x603558
static tig_art_id_t dword_603558;

// 0x60355C
static GameContextF8* dword_60355C;

// 0x603560
static char byte_603560[TIG_MAX_PATH];

// 0x603668
static ViewOptions jumppoint_view_options;

// 0x603670
static bool dword_603670;

// 0x603674
static int jumppoint_iso_window_handle;

// 0x603678
static tig_art_id_t dword_603678;

// 0x60367C
static bool dword_60367C;

// 0x603680
static bool jumppoint_initialized;

// 0x603684
static int jumppoints_count;

// 0x603688
static bool jumppoint_editor;

// 0x4E2FB0
bool jumppoint_init(GameInitInfo* init_info)
{
    jumppoint_iso_window_handle = init_info->iso_window_handle;
    dword_60355C = init_info->field_8;
    jumppoint_editor = init_info->editor;
    jumppoint_view_options.type = VIEW_TYPE_ISOMETRIC;

    tig_art_interface_id_create(350, 0, 0, 0, &dword_603678);
    tig_art_interface_id_create(351, 0, 0, 0, &dword_603558);

    jumppoint_initialized = true;
    dword_60367C = false;
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
    jumppoint_initialized = false;
}

// 0x4E3040
void jumppoint_resize(ResizeInfo* resize_info)
{
    jumppoint_iso_window_handle = resize_info->iso_window_handle;
}

// 0x4E3050
void sub_4E3050(MapResetInfo* reset_info)
{
    jumppoint_close();

    sprintf(byte_603560, "%s\\map.jmp", reset_info->name);
    sprintf(byte_603450, "%s\\map.jmp", reset_info->folder);

    dword_60367C = true;
    jumppoint_flush();
}

// 0x4E30A0
bool jumppoint_open(const char* a1, const char* a2)
{
    TigFile* stream;

    jumppoint_close();

    sprintf(byte_603560, "%s\\map.jmp", a1);
    sprintf(byte_603450, "%s\\map.jmp", a2);

    stream = tig_file_fopen(byte_603450, "rb");
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

    dword_60367C = false;
    tig_file_fclose(stream);

    return true;
}

// 0x4E3140
void jumppoint_close()
{
    if (jumppoints != NULL) {
        FREE(jumppoints);
        jumppoints = NULL;
    }

    jumppoints_count = 0;

    if (jumppoint_editor == 1) {
        dword_60355C(0);
    }

    dword_60367C = false;
}

// 0x4E3270
bool jumppoint_flush()
{
    TigFile* stream;

    if (dword_60367C) {
        stream = tig_file_fopen(byte_603450, "wb");
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
    int index;

    if (tig_file_fread(&jumppoints_count, sizeof(jumppoints_count), 1, stream) != 1) {
        return false;
    }

    if (jumppoints_count != 0) {
        jumppoints = (JumpPoint*)MALLOC(sizeof(*jumppoints) * jumppoints_count);
        for (index = 0; index < jumppoints_count; index++) {
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
    int index;

    if (tig_file_fwrite(&jumppoints_count, sizeof(jumppoints_count), 1, stream) != 1) {
        return false;
    }

    for (index = 0; index < jumppoints_count; index++) {
        if (tig_file_fwrite(&(jumppoints[index]), sizeof(*jumppoints), 1, stream) != 1) {
            return false;
        }
    }

    return true;
}
