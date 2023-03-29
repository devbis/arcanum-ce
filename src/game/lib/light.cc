#include "game/lib/light.h"

#include "game/lib/gamelib.h"
#include "game/lib/settings.h"
#include "tig/art.h"
#include "tig/color.h"
#include "tig/debug.h"
#include "tig/palette.h"
#include "tig/rect.h"
#include "tig/video.h"
#include "tig/window.h"

#define SHADOWS_KEY "shadows"

typedef struct Light30 {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
};

// See 0x4DDD20.
static_assert(sizeof(Light30) == 0x30, "wrong size");

typedef struct Light602E60 {
    int field_0;
    int field_4;
    int field_8;
    Light602E60* next;
};

// See 4DE7C0.
static_assert(sizeof(Light602E60) == 0x10, "wrong size");

// 0x602E10
static int light_bpp;

// 0x602E18
static TigVideoBufferData stru_602E18;

// 0x602E38
static ViewOptions light_view_options;

// 0x602E40
static void* dword_602E40;

// 0x602E44
static int dword_602E44;

// 0x602E48
static TigRect stru_602E48;

// 0x602E58
static void** dword_602E58;

// 0x602E5C
static TigVideoBuffer* dword_602E5C;

// 0x602E60
static Light602E60* off_602E60;

// 0x602E68
static TigVideoBufferData stru_602E68;

// 0x602E88
static void* dword_602E88;

// 0x602E8C
static GameContextF8* dword_602E8C;

// 0x602E90
static bool light_shadows_enabled;

// 0x602E94
static TigVideoBuffer* dword_602E94;

// 0x602E98
static void* dword_602E98;

// 0x602E9C
static void* dword_602E9C;

// 0x602EA0
static void* dword_602EA0;

// 0x602EA4
static int dword_602EA4;

// 0x602EA8
static int dword_602EA8;

// 0x602EAC
static void* dword_602EAC;

// 0x602EB0
static void* dword_602EB0;

// 0x602EB4
static void* dword_602EB4;

// 0x602EB8
static bool dword_602EB8;

// 0x602EBC
static int dword_602EBC;

// 0x602EC0
static bool dword_602EC0;

// 0x602EC4
static int dword_602EC4;

// 0x602EC8
static int dword_602EC8;

// 0x602ECC
static int dword_602ECC;

// 0x602ED0
static int dword_602ED0;

// 0x602ED4
static int dword_602ED4;

// 0x603404
static int dword_603404;

// 0x603408
static int dword_603408;

// 0x60340C
static int dword_60340C;

// 0x603418
static int dword_603418;

// 0x60341C
static int dword_60341C;

// 0x4D7F30
bool light_init(GameContext* ctx)
{
    dword_602E58 = (void**)calloc(7, sizeof(*dword_602E58));
    nullsub_33();
    dword_602EC4 = ctx->iso_window_handle;
    dword_602E8C = ctx->field_8;
    dword_602EC0 = ctx->editor;
    light_view_options.type = VIEW_TYPE_ISOMETRIC;
    dword_602ECC = 1;
    dword_602EB8 = 1;

    if (sub_51FA80(&light_bpp) != TIG_OK) {
        free(dword_602E58);
        return false;
    }

    TigWindowData window_data;
    if (tig_window_data(ctx->iso_window_handle, &window_data) != TIG_OK) {
        free(dword_602E58);
        return false;
    }

    stru_602E48.x = 0;
    stru_602E48.y = 0;
    stru_602E48.width = window_data.rect.width;
    stru_602E48.height = window_data.rect.height;

    dword_60340C = tig_video_3d_check_initialized() == TIG_OK;

    settings_add(&settings, SHADOWS_KEY, "0", shadows_changed);
    light_shadows_enabled = settings_get_value(&settings, SHADOWS_KEY);

    if (!sub_4DDF50()) {
        free(dword_602E58);
        return false;
    }

    if (!sub_4DE5D0()) {
        free(dword_602E58);
        return false;
    }

    dword_602EA8 = tig_color_rgb_make(255, 255, 255);
    dword_603408 = tig_color_rgb_make(255, 255, 255);

    sub_4DE200();
    sub_5022B0(sub_4DE0B0);
    sub_5022D0();

    return true;
}

// 0x4D8120
void light_exit()
{
    sub_4DE730();
    sub_4DE250();
    sub_4DE060();
    dword_602EC4 = -1;
    dword_602E8C = NULL;
    sub_4F8340();
    free(dword_602E58);
}

// 0x4D8160
void light_resize(ResizeContext* ctx)
{
    dword_602EC4 = ctx->iso_window_handle;
    stru_602E48 = ctx->field_14;
    sub_4DE060();

    if (!sub_4DDF50()) {
        tig_debug_printf("light_resize: ERROR: Failed to rebuild the vbuffer!\n");
    }
}

// 0x4D81B0
bool light_update_view(ViewOptions* view_options)
{
    light_view_options = *view_options;
    return true;
}

// 0x4D81F0
void sub_4D81F0()
{
    dword_602ECC = 1 - dword_602ECC;

    if (!dword_60340C) {
        sub_5022D0();
    }

    sub_4D1060(sub_4DDD90);
    sub_4DF310(NULL, false);
}

// 0x4D8210
void sub_4D8210()
{
    dword_603404++;
    if (dword_603404 == 1) {
        tig_video_buffer_lock(dword_602E94);
        tig_video_buffer_data(dword_602E94, &stru_602E68);

        tig_video_buffer_lock(dword_602E5C);
        tig_video_buffer_data(dword_602E5C, &stru_602E18);

        switch (light_bpp) {
        case 8:
            break;
        case 16:
            dword_602EC8 = stru_602E68.pitch / 2;
            dword_602EB4 = stru_602E68.surface_data;
            dword_602EBC = stru_602E18.pitch / 2;
            dword_602E9C = stru_602E18.surface_data;
            break;
        case 24:
            dword_602EC8 = stru_602E68.pitch;
            dword_602EB0 = stru_602E68.surface_data;
            dword_602EBC = stru_602E18.pitch;
            dword_602EA0 = stru_602E18.surface_data;
            break;
        case 32:
            dword_602EC8 = stru_602E68.pitch / 4;
            dword_602EAC = stru_602E68.surface_data;
            dword_602EBC = stru_602E18.pitch / 4;
            dword_602E98 = stru_602E18.surface_data;
            break;
        }
    }
}

// 0x4D8320
void sub_4D8320()
{
    dword_603404--;
    if (dword_603404 == 0) {
        tig_video_buffer_unlock(dword_602E94);
        tig_video_buffer_unlock(dword_602E5C);
    }
}

// 0x4D8350
void sub_4D8350(UnknownContext* ctx)
{
    if (dword_602EB8) {
        if (light_view_options.type == VIEW_TYPE_ISOMETRIC) {
            long long location;
            sub_4B8730(stru_602E48.width / 2, stru_602E48.height / 2, &location);

            long long x;
            long long y;
            sub_4B8680(location, &x, &y);
            x += 40;
            y += 20;

            dword_602ED0 = x - dword_602E44 / 2;
            dword_602ED4 = y - dword_602EA4 / 2;
            sub_4DE900(ctx);
        }
    }
}

// 0x4D84B0
void light_build_color(uint8_t red, uint8_t green, uint8_t blue, int* color)
{
    *color = (red << 16) | (green << 8) | blue;
}

// 0x4D84D0
void light_get_color_components(int color, uint8_t* red, uint8_t* green, uint8_t* blue)
{
    *red = (color >> 16) & 0xFF;
    *green = (color >> 8) & 0xFF;
    *blue = color & 0xFF;
}

// 0x4D8500
int sub_4D8500()
{
    return dword_602EA8;
}

// 0x4D8530
int sub_4D8530()
{
    return dword_603408;
}

// 0x4D8560
void sub_4D8560(int a1, int a2)
{
    dword_603408 = a1;
    dword_602EA8 = a2;
    sub_4DE200();

    if (dword_602ECC) {
        if (!dword_60340C) {
            sub_5022D0();
        }

        sub_4D1060(sub_4DDD90);
        sub_4DF310(NULL, false);
    }
}

// 0x4D9A90
void sub_4D9A90(object_id_t object_id)
{
    unsigned int render_flags = object_field_get(object_id, OBJ_F_RENDER_FLAGS);
    if ((render_flags & OBJECT_RENDER_FLAG_0x80000000) != 0) {
        light_handle_t light_handle = object_field_get(object_id, OBJ_F_LIGHT_HANDLE);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4D93B0(light_handle);
        }

        for (int index = 0; index < 4; index++) {
            light_handle_t light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
            if (light_handle != LIGHT_HANDLE_INVALID) {
                sub_4D93B0(light_handle);
            }
        }

        object_field_set(object_id, OBJ_F_RENDER_FLAGS, render_flags & ~OBJECT_RENDER_FLAG_0x80000000);
    }
}

// 0x4DA310
void sub_4DA310(object_id_t object_id)
{
    for (int index = 0; index < 5; index++) {
        shadow_handle_t shadow_handle = sub_4072D0(object_id, OBJ_F_SHADOW_HANDLES, index);
        if (shadow_handle == SHADOW_HANDLE_INVALID) {
            break;
        }

        sub_4DE7A0(shadow_handle);
        sub_407340(object_id, OBJ_F_SHADOW_HANDLES, index, SHADOW_HANDLE_INVALID);
    }
}

// 0x4DCEA0
void sub_4DCEA0(object_id_t object_id, int a2)
{
    light_handle_t light_handle = object_field_get(object_id, OBJ_F_LIGHT_HANDLE);
    if (light_handle != LIGHT_HANDLE_INVALID) {
        sub_4DD150(light_handle, a2);
    } else {
        if (object_field_get(object_id, OBJ_F_LIGHT_AID) != -1) {
            unsigned int light_flags = object_field_get(object_id, OBJ_F_LIGHT_FLAGS);
            object_field_set(object_id, OBJ_F_LIGHT_FLAGS, light_flags | a2);
        }
    }

    for (int index = 0; index < 4; index++) {
        light_handle_t light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4DD150(light_handle, a2);
        } else {
            if (sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_AID, index) != -1) {
                unsigned int light_flags = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_FLAGS, index);
                sub_4074E0(object_id, OBJ_F_OVERLAY_LIGHT_FLAGS, index, light_flags | a2);
            }
        }
    }
}

// 0x4DCF60
void sub_4DCF60(object_id_t object_id, int a2)
{
    light_handle_t light_handle = object_field_get(object_id, OBJ_F_LIGHT_HANDLE);
    if (light_handle != LIGHT_HANDLE_INVALID) {
        sub_4DD230(light_handle, a2);
    } else {
        if (object_field_get(object_id, OBJ_F_LIGHT_AID) != -1) {
            unsigned int light_flags = object_field_get(object_id, OBJ_F_LIGHT_FLAGS);
            object_field_set(object_id, OBJ_F_LIGHT_FLAGS, light_flags & ~a2);
        }
    }

    for (int index = 0; index < 4; index++) {
        light_handle_t light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4DD230(light_handle, a2);
        } else {
            if (sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_AID, index) != -1) {
                unsigned int light_flags = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_FLAGS, index);
                sub_4074E0(object_id, OBJ_F_OVERLAY_LIGHT_FLAGS, index, light_flags & ~a2);
            }
        }
    }
}

// 0x4DD020
void sub_4DD020(object_id_t object_id, int a2, int a3, int a4, int a5)
{
    light_handle_t light_handle = object_field_get(object_id, OBJ_F_LIGHT_HANDLE);
    if (light_handle != LIGHT_HANDLE_INVALID) {
        sub_4DD320(light_handle, a2, a3, a4, a5);
    }

    for (int index = 0; index < 4; index++) {
        light_handle_t light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4DD320(light_handle, a2, a3, a4, a5);
        }
    }
}

// 0x4DD0A0
void sub_4DD0A0(object_id_t object_id, int a2, int a3)
{
    light_handle_t light_handle = object_field_get(object_id, OBJ_F_LIGHT_HANDLE);
    if (light_handle != LIGHT_HANDLE_INVALID) {
        sub_4DD500(light_handle, a2, a3);
    }

    for (int index = 0; index < 4; index++) {
        light_handle_t light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4DD500(light_handle, a2, a3);
        }
    }
}

// 0x4DDD20
bool sub_4DDD20(TigFile* stream, Light30** a2)
{
    Light30* v1 = (Light30*)malloc(sizeof(*v1));
    if (tig_file_fread(v1, sizeof(*v1), 1, stream) != 1) {
        return false;
    }

    v1->field_28 = 0;
    sub_4DE390(v1);
    *a2 = v1;
    return true;
}

// 0x4DDD70
bool sub_4DDD70(TigFile* stream, Light30* a2)
{
    return tig_file_fread(a2, sizeof(*a2), 1, stream) == 1;
}

// 0x4DDF20
void shadows_changed()
{
    light_shadows_enabled = settings_get_value(&settings, SHADOWS_KEY);

    if (dword_602E8C != NULL) {
        dword_602E8C(NULL);
    }
}

// 0x4DDF50
bool sub_4DDF50()
{
    sub_4DE060();

    dword_602E44 = stru_602E48.width + 320;
    dword_603418 = (int)((double)dword_602E44 * 0.0125 + (double)dword_602E44 * 0.0125 + 1.0);

    dword_602EA4 = stru_602E48.height + 160;
    dword_60341C = (int)((double)dword_602EA4 * 0.025 + (double)dword_602EA4 * 0.025 + 1.0);

    TigVideoBufferSpec video_buffer_spec;
    video_buffer_spec.flags = TIG_VIDEO_BUFFER_SPEC_SYSTEM_MEMORY | TIG_VIDEO_BUFFER_SPEC_TRANSPARENCY_ENABLED;
    video_buffer_spec.width = dword_603418;
    video_buffer_spec.height = dword_60341C;
    video_buffer_spec.color_key = tig_color_rgb_make(0, 255, 0);
    video_buffer_spec.background_color = video_buffer_spec.color_key;

    if (tig_video_buffer_create(&video_buffer_spec, &dword_602E5C) != TIG_OK) {
        return false;
    }

    if (tig_video_buffer_create(&video_buffer_spec, &dword_602E94) != TIG_OK) {
        return false;
    }

    return true;
}

// 0x4DE060
void sub_4DE060()
{
    if (dword_602E94 != NULL) {
        tig_video_buffer_destroy(dword_602E94);
        dword_60341C = 0;
        dword_603418 = 0;
        dword_602E94 = NULL;
    }

    if (dword_602E5C != NULL) {
        tig_video_buffer_destroy(dword_602E5C);
        dword_60341C = 0;
        dword_603418 = 0;
        dword_602E5C = NULL;
    }
}

// 0x4DE200
void sub_4DE200()
{
    if (!dword_60340C) {
        if (dword_602E40 == NULL) {
            dword_602E40 = sub_533DF0();
        }
        sub_533E40(dword_602E40, dword_603408);

        if (dword_602E88 == NULL) {
            dword_602E88 = sub_533DF0();
        }
        sub_533E40(dword_602E88, dword_602EA8);
    }
}

// 0x4DE250
void sub_4DE250()
{
    if (!dword_60340C) {
        sub_533E20(dword_602E40);
        dword_602E40 = NULL;

        sub_533E20(dword_602E88);
        dword_602E88 = NULL;
    }
}

// 0x4DE770
Light602E60* sub_4DE770()
{
    Light602E60* node = off_602E60;
    if (node == NULL) {
        sub_4DE7C0();
        node = off_602E60;
    }

    off_602E60 = node->next;
    node->next = NULL;

    return node;
}

// 0x4DE7A0
void sub_4DE7A0(Light602E60* node)
{
    node->next = off_602E60;
    off_602E60 = node;
}

// 0x4DE7C0
void sub_4DE7C0()
{
    for (int index = 0; index < 20; index++) {
        Light602E60* node = (Light602E60*)malloc(sizeof(*node));
        node->next = off_602E60;
        off_602E60 = node;
    }
}

// 0x4DE7F0
void sub_4DE7F0()
{
    Light602E60* curr = off_602E60;
    while (curr != NULL) {
        Light602E60* next = curr->next;
        free(curr);
        curr = next;
    }
    off_602E60 = NULL;
}

// 0x4DF310
void sub_4DF310(TigRect* rect, bool a2)
{
    TigRect dirty_rect;

    if (rect != NULL) {
        dirty_rect = *rect;
    } else {
        dirty_rect = stru_602E48;
    }

    dirty_rect.x -= 40;
    dirty_rect.y -= 20;
    dirty_rect.width += 80;
    dirty_rect.height += 40;
    dword_602E8C(&dirty_rect);

    if (a2) {
        sub_43CB10(&dirty_rect);
    }
}
