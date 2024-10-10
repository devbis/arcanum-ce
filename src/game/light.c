#include "game/light.h"

#include "game/gamelib.h"
#include "game/object.h"
#include "game/sector.h"
#include "game/sector_block_list.h"

#define SHADOWS_KEY "shadows"

typedef struct Light602E60 {
    int field_0;
    int field_4;
    int field_8;
    struct Light602E60* next;
} Light602E60;

// See 4DE7C0.
static_assert(sizeof(Light602E60) == 0x10, "wrong size");

typedef struct LightCreateInfo {
    /* 0000 */ int64_t obj;
    /* 0008 */ int64_t loc;
    /* 0010 */ int offset_x;
    /* 0014 */ int offset_y;
    /* 0018 */ tig_art_id_t art_id;
    /* 001C */ unsigned int flags;
    /* 0020 */ uint8_t r;
    /* 0021 */ uint8_t g;
    /* 0022 */ uint8_t b;
} LightCreateInfo;

static void sub_4D9310(LightCreateInfo* create_info, Light30** light_ptr);
static void sub_4D93B0(Light30* light);
static void sub_4DD150(light_handle_t light_handle, int a2);
static void sub_4DD230(light_handle_t light_handle, int a2);
static void sub_4DD320(light_handle_t light_handle, int a2, int a3, int a4, int a5);
static void sub_4DD500(Light30* light, int offset_x, int offset_y);
static void sub_4DDAB0(Light30* light, tig_art_id_t art_id);
static tig_art_id_t sub_4DDB70(Light30* light);
static void sub_4DDB80(Light30* light, uint8_t r, uint8_t g, uint8_t b);
static bool sub_4DDD90(Sector* sector);
static void shadows_changed();
static bool sub_4DDF50();
static void sub_4DE060();
static bool sub_4DE0B0(tig_art_id_t art_id, TigPaletteModifyInfo* modify_info);
static void sub_4DE200();
static void sub_4DE250();
static void light_get_rect_internal(Light30* light, TigRect* rect);
static void sub_4DE390(Light30* light);
static void sub_4DE4D0(Light30* light);
static void sub_4DE4F0(Light30* light, int offset_x, int offset_y);
static bool sub_4DE5D0();
static void sub_4DE730();
static Light602E60* sub_4DE770();
static void sub_4DE7A0(Light602E60* node);
static void sub_4DE7C0();
static void sub_4DE7F0();
static bool sub_4DE820(TimeEvent* timeevent);
static void sub_4DE870(LightCreateInfo* create_info, Light30** light_ptr);
static void sub_4DE900(UnknownContext* ctx);

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
static TigPalette* dword_602E58;

// 0x602E5C
static TigVideoBuffer* dword_602E5C;

// 0x602E60
static Light602E60* off_602E60;

// 0x602E68
static TigVideoBufferData stru_602E68;

// 0x602E88
static TigPalette dword_602E88;

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
static tig_color_t light_outdoor_color;

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
static bool light_editor;

// 0x602EC4
static tig_window_handle_t light_iso_window_handle;

// 0x602EC8
static int dword_602EC8;

// 0x602ECC
static bool dword_602ECC;

// 0x602ED0
static int dword_602ED0;

// 0x602ED4
static int dword_602ED4;

// 0x602ED8
static TigRect stru_602ED8;

// 0x602EE8
static TigBmp stru_602EE8;

// 0x603400
static Light30* dword_603400;

// 0x603404
static int dword_603404;

// 0x603408
static tig_color_t light_indoor_color;

// 0x60340C
static int dword_60340C;

// 0x603418
static int dword_603418;

// 0x60341C
static int dword_60341C;

// 0x4D7F30
bool light_init(GameInitInfo* init_info)
{
    TigWindowData window_data;

    dword_602E58 = (TigPalette**)CALLOC(7, sizeof(*dword_602E58));
    sub_4F8330();
    light_iso_window_handle = init_info->iso_window_handle;
    dword_602E8C = init_info->field_8;
    light_editor = init_info->editor;
    light_view_options.type = VIEW_TYPE_ISOMETRIC;
    dword_602ECC = true;
    dword_602EB8 = true;

    if (tig_video_get_bpp(&light_bpp) != TIG_OK) {
        FREE(dword_602E58);
        return false;
    }

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        FREE(dword_602E58);
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
        FREE(dword_602E58);
        return false;
    }

    if (!sub_4DE5D0()) {
        FREE(dword_602E58);
        return false;
    }

    light_outdoor_color = tig_color_make(255, 255, 255);
    light_indoor_color = tig_color_make(255, 255, 255);

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
    light_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
    dword_602E8C = NULL;
    sub_4F8340();
    FREE(dword_602E58);
}

// 0x4D8160
void light_resize(ResizeContext* resize_info)
{
    light_iso_window_handle = resize_info->iso_window_handle;
    stru_602E48 = resize_info->field_14;
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
    dword_602ECC = !dword_602ECC;

    if (!dword_60340C) {
        sub_5022D0();
    }

    sector_enumerate(sub_4DDD90);
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
            dword_602EB4 = stru_602E68.surface_data.pixels;
            dword_602EBC = stru_602E18.pitch / 2;
            dword_602E9C = stru_602E18.surface_data.pixels;
            break;
        case 24:
            dword_602EC8 = stru_602E68.pitch;
            dword_602EB0 = stru_602E68.surface_data.pixels;
            dword_602EBC = stru_602E18.pitch;
            dword_602EA0 = stru_602E18.surface_data.pixels;
            break;
        case 32:
            dword_602EC8 = stru_602E68.pitch / 4;
            dword_602EAC = stru_602E68.surface_data.pixels;
            dword_602EBC = stru_602E18.pitch / 4;
            dword_602E98 = stru_602E18.surface_data.pixels;
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
    long long location;
    long long x;
    long long y;

    if (dword_602EB8) {
        if (light_view_options.type == VIEW_TYPE_ISOMETRIC) {
            sub_4B8730(stru_602E48.width / 2, stru_602E48.height / 2, &location);
            sub_4B8680(location, &x, &y);
            x += 40;
            y += 20;

            dword_602ED0 = (int)x - dword_602E44 / 2;
            dword_602ED4 = (int)y - dword_602EA4 / 2;
            sub_4DE900(ctx);
        }
    }
}

// 0x4D84B0
void light_build_color(uint8_t red, uint8_t green, uint8_t blue, unsigned int* color)
{
    *color = (red << 16) | (green << 8) | blue;
}

// 0x4D84D0
void light_get_color_components(unsigned int color, uint8_t* red, uint8_t* green, uint8_t* blue)
{
    *red = (color >> 16) & 0xFF;
    *green = (color >> 8) & 0xFF;
    *blue = color & 0xFF;
}

// 0x4D8500
tig_color_t light_get_outdoor_color()
{
    return light_outdoor_color;
}

// 0x4D8530
tig_color_t light_get_indoor_color()
{
    return light_indoor_color;
}

// 0x4D8560
void light_set_colors(tig_color_t indoor_color, tig_color_t outdoor_color)
{
    light_indoor_color = indoor_color;
    light_outdoor_color = outdoor_color;
    sub_4DE200();

    if (dword_602ECC) {
        if (!dword_60340C) {
            sub_5022D0();
        }

        sector_enumerate(sub_4DDD90);
        sub_4DF310(NULL, false);
    }
}

// 0x4D8590
void sub_4D8590(Light30* light)
{
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;
    TimeEvent timeevent;
    DateTime datetime;

    if ((sub_4DD310(light) & 0x24) == 0) {
        art_id = sub_4DDB70(light);
        if (tig_art_anim_data(art_id, &art_anim_data) == TIG_OK
            && art_anim_data.num_frames > 1) {
            timeevent.type = TIMEEVENT_TYPE_LIGHT;
            timeevent.params[0].integer_value = light;
            timeevent.params[1].integer_value = 1000 / art_anim_data.fps;

            sub_45A950(&datetime, 1000 / art_anim_data.fps);
            if (sub_45B800(&timeevent, &datetime)) {
                sub_4DD150(light, 0x4);
            }
        }
    }
}

// 0x4D8620
void sub_4D8620(Light30* light)
{
    if ((sub_4DD310(light) & 0x4) != 0) {
        dword_603400 = light;
        timeevent_clear_all_ex(TIMEEVENT_TYPE_LIGHT, sub_4DE820);
        sub_4DD230(light, 0x4);
    }
}

// 0x4D8660
bool light_timeevent_process(TimeEvent* timeevent)
{
    DateTime datetime;
    TimeEvent next_timeevent;

    sub_4DD720(timeevent->params[0].integer_value);

    next_timeevent = *timeevent;
    sub_45A950(&datetime, next_timeevent.params[1].integer_value);
    sub_45B800(&timeevent, &datetime);

    return true;
}

// 0x4D86B0
void sub_4D86B0()
{
    // TODO: Incomplete.
}

// 0x4D87F0
void sub_4D87F0()
{
    // TODO: Incomplete.
}

// 0x4D89E0
void sub_4D89E0()
{
    // TODO: Incomplete.
}

// 0x4D9240
void sub_4D9240()
{
    // TODO: Incomplete.
}

// 0x4D9310
void sub_4D9310(LightCreateInfo* create_info, Light30** light_ptr)
{
    int64_t sector_id;
    Sector* sector;
    TigRect rect;

    sub_4DE870(create_info, light_ptr);
    if (*light_ptr != NULL) {
        sector_id = sub_4CFC50(create_info->loc);
        if (sub_4DD110(*light_ptr) || sub_4D04E0(sector_id)) {
            sector_lock(sector_id, &sector);
            sub_4F71C0(&(sector->lights), *light_ptr);
            sector_unlock(sector_id);
        }

        light_get_rect(*light_ptr, &rect);
        sub_4DF310(&rect, true);
    }
}

// 0x4D93B0
void sub_4D93B0(Light30* light)
{
    TigRect rect;
    int index;

    sub_4D8620(light);

    light_get_rect_internal(light, &rect);
    sub_4DF310(&rect, true);

    if (light->obj != OBJ_HANDLE_NULL) {
        if (obj_field_int32_get(light->obj, OBJ_F_LIGHT_HANDLE) == light) {
            obj_field_int32_set(light->obj, OBJ_F_LIGHT_HANDLE, LIGHT_HANDLE_INVALID);
            sub_4D9510(light);
            sub_4D9570(light);
        } else {
            for (index = 0; index < 4; index++) {
                if (sub_407470(light->obj, OBJ_F_OVERLAY_LIGHT_HANDLES, index) == light) {
                    sub_4074E0(light->obj, OBJ_F_OVERLAY_LIGHT_HANDLES, index, LIGHT_HANDLE_INVALID);
                    sub_4D9510(light);
                    sub_4D9570(light);
                    break;
                }
            }
        }
    } else {
        if (light_editor) {
            sub_4D9510(light);
            sub_4D9570(light);
        } else {
            sub_4DD150(light, 0x100);
        }
    }
}

// 0x4D94B0
int sub_4D94B0(Light30* a1)
{
    return a1->flags & 0x80;
}

// 0x4D94C0
void sub_4D94C0(Light30* a1)
{
    a1->flags &= ~0x80;
}

// 0x4D94D0
bool sub_4D94D0(TigFile* stream, Light30** a2)
{
    return sub_4DDD20(stream, a2);
}

// 0x4D94F0
bool sub_4D94F0(TigFile* stream, Light30* a2)
{
    return sub_4DDD70(stream, a2);
}

// 0x4D9510
void sub_4D9510()
{
    // TODO: Incomplete.
}

// 0x4D9570
void sub_4D9570(Light30* light)
{
    sub_4D8620(light);
    sub_4DE4D0(light);
    FREE(light);
}

// 0x4D9590
void sub_4D9590()
{
    // TODO: Incomplete.
}

// 0x4D9990
void sub_4D9990(int64_t obj)
{
    Light30* light;
    unsigned int color;
    int index;

    light = (Light30*)obj_field_int32_get(obj, OBJ_F_LIGHT_HANDLE);
    if (light != LIGHT_HANDLE_INVALID) {
        obj_field_int32_set(obj, OBJ_F_LIGHT_FLAGS, light->flags);
        obj_field_int32_set(obj, OBJ_F_LIGHT_AID, light->art_id);
        light_build_color(light->r, light->g, light->b, &color);
        obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, color);
    } else {
        obj_field_int23_set(obj, OBJ_F_LIGHT_AID, TIG_ART_ID_INVALID);
    }

    for (index = 0; index < 4; index++) {
        light = (Light30*)sub_407470(obj, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light != LIGHT_HANDLE_INVALID) {
            sub_4074E0(obj, OBJ_F_OVERLAY_LIGHT_FLAGS, index, light->flags);
            sub_4074E0(obj, OBJ_F_OVERLAY_LIGHT_AID, index, light->art_id);
            light_build_color(light->r, light->g, light->b, &color);
            sub_4074E0(obj, OBJ_F_OVERLAY_LIGHT_COLOR, index, color);
        } else {
            sub_4074E0(obj, OBJ_F_OVERLAY_LIGHT_AID, index, TIG_ART_ID_INVALID);
        }
    }
}

// 0x4D9A90
void sub_4D9A90(object_id_t object_id)
{
    unsigned int render_flags;
    light_handle_t light_handle;
    int index;

    render_flags = obj_field_int32_get(object_id, OBJ_F_RENDER_FLAGS);
    if ((render_flags & OBJECT_RENDER_FLAG_0x80000000) != 0) {
        light_handle = obj_field_int32_get(object_id, OBJ_F_LIGHT_HANDLE);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4D93B0(light_handle);
        }

        for (index = 0; index < 4; index++) {
            light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
            if (light_handle != LIGHT_HANDLE_INVALID) {
                sub_4D93B0(light_handle);
            }
        }

        obj_field_int32_set(object_id, OBJ_F_RENDER_FLAGS, render_flags & ~OBJECT_RENDER_FLAG_0x80000000);
    }
}

// 0x4D9B20
void sub_4D9B20()
{
    // TODO: Incomplete.
}

// 0x4DA310
void sub_4DA310(object_id_t object_id)
{
    int index;
    shadow_handle_t shadow_handle;

    for (index = 0; index < 5; index++) {
        shadow_handle = obj_arrayfield_int32_get(object_id, OBJ_F_SHADOW_HANDLES, index);
        if (shadow_handle == SHADOW_HANDLE_INVALID) {
            break;
        }

        sub_4DE7A0(shadow_handle);
        obj_arrayfield_int32_set(object_id, OBJ_F_SHADOW_HANDLES, index, SHADOW_HANDLE_INVALID);
    }
}

// 0x4DA360
void sub_4DA360()
{
    // TODO: Incomplete.
}

// 0x4DC210
void sub_4DC210()
{
    // TODO: Incomplete.
}

// 0x4DCE10
uint8_t sub_4DCE10(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x4DCEA0
void sub_4DCEA0(object_id_t object_id, int a2)
{
    light_handle_t light_handle;
    int index;
    unsigned int flags;

    light_handle = obj_field_int32_get(object_id, OBJ_F_LIGHT_HANDLE);
    if (light_handle != LIGHT_HANDLE_INVALID) {
        sub_4DD150(light_handle, a2);
    } else {
        if (obj_field_int32_get(object_id, OBJ_F_LIGHT_AID) != -1) {
            flags = obj_field_int32_get(object_id, OBJ_F_LIGHT_FLAGS);
            flags |= a2;
            obj_field_int32_set(object_id, OBJ_F_LIGHT_FLAGS, flags);
        }
    }

    for (index = 0; index < 4; index++) {
        light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4DD150(light_handle, a2);
        } else {
            if (sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_AID, index) != -1) {
                flags = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_FLAGS, index);
                flags |= a2;
                sub_4074E0(object_id, OBJ_F_OVERLAY_LIGHT_FLAGS, index, flags);
            }
        }
    }
}

// 0x4DCF60
void sub_4DCF60(object_id_t object_id, int a2)
{
    light_handle_t light_handle;
    int index;
    unsigned int flags;

    light_handle = obj_field_int32_get(object_id, OBJ_F_LIGHT_HANDLE);
    if (light_handle != LIGHT_HANDLE_INVALID) {
        sub_4DD230(light_handle, a2);
    } else {
        if (obj_field_int32_get(object_id, OBJ_F_LIGHT_AID) != -1) {
            flags = obj_field_int32_get(object_id, OBJ_F_LIGHT_FLAGS);
            flags &= ~a2;
            obj_field_int32_set(object_id, OBJ_F_LIGHT_FLAGS, flags);
        }
    }

    for (index = 0; index < 4; index++) {
        light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4DD230(light_handle, a2);
        } else {
            if (sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_AID, index) != -1) {
                flags = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_FLAGS, index);
                flags &= ~a2;
                sub_4074E0(object_id, OBJ_F_OVERLAY_LIGHT_FLAGS, index, flags);
            }
        }
    }
}

// 0x4DD020
void sub_4DD020(object_id_t object_id, int a2, int a3, int a4, int a5)
{
    light_handle_t light_handle;
    int index;

    light_handle = obj_field_int32_get(object_id, OBJ_F_LIGHT_HANDLE);
    if (light_handle != LIGHT_HANDLE_INVALID) {
        sub_4DD320(light_handle, a2, a3, a4, a5);
    }

    for (index = 0; index < 4; index++) {
        light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4DD320(light_handle, a2, a3, a4, a5);
        }
    }
}

// 0x4DD0A0
void sub_4DD0A0(object_id_t object_id, int a2, int a3)
{
    light_handle_t light_handle;
    int index;

    light_handle = obj_field_int32_get(object_id, OBJ_F_LIGHT_HANDLE);
    if (light_handle != LIGHT_HANDLE_INVALID) {
        sub_4DD500(light_handle, a2, a3);
    }

    for (index = 0; index < 4; index++) {
        light_handle = sub_407470(object_id, OBJ_F_OVERLAY_LIGHT_HANDLES, index);
        if (light_handle != LIGHT_HANDLE_INVALID) {
            sub_4DD500(light_handle, a2, a3);
        }
    }
}

// 0x4DD110
bool sub_4DD110()
{
    // TODO: Incomplete.
}

// 0x4DD130
void light_get_rect(Light30* light, TigRect* rect)
{
    light_get_rect_internal(light, rect);
}

// 0x4DD150
void sub_4DD150(light_handle_t light_handle, int a2)
{
    // TODO: Incomplete.
}

// 0x4DD230
void sub_4DD230(light_handle_t light_handle, int a2)
{
    // TODO: Incomplete.
}

// 0x4DD310
unsigned int sub_4DD310(Light30* a1)
{
    return a1->flags;
}

// 0x4DD320
void sub_4DD320(light_handle_t light_handle, int a2, int a3, int a4, int a5)
{
    // TODO: Incomplete.
}

// 0x4DD500
void sub_4DD500(Light30* light, int offset_x, int offset_y)
{
    sub_4DE4F0(light, offset_x, offset_y);
}

// 0x4DD720
void sub_4DD720()
{
    // TODO: Incomplete.
}

// 0x4DD830
void sub_4DD830()
{
    // TODO: Incomplete.
}

// 0x4DD940
void sub_4DD940()
{
    // TODO: Incomplete.
}

// 0x4DDA40
void light_set_location(Light30* light, int64_t loc)
{
    light->loc = loc;
    light->flags |= 0x80;
}

// 0x4DDA60
int64_t light_get_location(Light30* light)
{
    return light->loc;
}

// 0x4DDA70
void sub_4DDA70(Light30* light, int offset_x, int offset_y)
{
    light->offset_x = offset_x;
    light->offset_y = offset_y;
    light->flags |= 0x80;
}

// 0x4DDAB0
void sub_4DDAB0(Light30* light, tig_art_id_t art_id)
{
    light->art_id = art_id;
    light->flags |= 0x80;

    if (light->obj != OBJ_HANDLE_NULL) {
        if ((light->flags & 0x8000000) != 0) {
            obj_field_int32_set(light->obj, OBJ_F_LIGHT_AID, art_id);
        } else if ((light->flags & 0x10000000) != 0) {
            obj_field_int32_set(light->obj, OBJ_F_OVERLAY_LIGHT_AID, 0, art_id);
        } else if ((light->flags & 0x20000000) != 0) {
            obj_field_int32_set(light->obj, OBJ_F_OVERLAY_LIGHT_AID, 1, art_id);
        } else if ((light->flags & 0x40000000) != 0) {
            obj_field_int32_set(light->obj, OBJ_F_OVERLAY_LIGHT_AID, 2, art_id);
        } else if ((light->flags & 0x80000000) != 0) {
            obj_field_int32_set(light->obj, OBJ_F_OVERLAY_LIGHT_AID, 3, art_id);
        }
    }
}

// 0x4DDB70
tig_art_id_t sub_4DDB70(Light30* light)
{
    return light->art_id;
}

// 0x4DDB80
void sub_4DDB80(Light30* light, uint8_t r, uint8_t g, uint8_t b)
{
    unsigned int color;
    int index;

    if ((light->flags & 0x08) != 0) {
        sub_4DD230(light, 0x08);
    }

    if ((light->flags & 0x10) != 0) {
        sub_4DD230(light, 0x10);
    }

    light->flags |= 0x80;

    if (light->obj != OBJ_HANDLE_NULL) {
        light_build_color(r, g, b, &color);

        if ((light->flags & 0x8000000) != 0) {
            obj_field_int32_set(light->obj, OBJ_F_LIGHT_COLOR, color);
        } else {
            if ((light->flags & 0x10000000) != 0) {
                index = 0;
            } else if ((light->flags & 0x20000000) != 0) {
                index = 1;
            } else if ((light->flags & 0x40000000) != 0) {
                index = 2;
            } else if ((light->flags & 0x80000000) != 0) {
                index = 3;
            } else {
                // Unreachable.
                __assume(0);
            }

            sub_4074E0(light->obj, OBJ_F_OVERLAY_LIGHT_COLOR, index, color);
        }
    }

    light->r = r;
    light->g = g;
    light->b = b;
    sub_4DE390(light);
}

// 0x4DDD20
bool sub_4DDD20(TigFile* stream, Light30** light_ptr)
{
    Light30* light;

    light = (Light30*)MALLOC(sizeof(*light));
    if (tig_file_fread(light, sizeof(*light), 1, stream) != 1) {
        return false;
    }

    light->palette = NULL;
    sub_4DE390(light);
    *light_ptr = light;

    return true;
}

// 0x4DDD70
bool sub_4DDD70(TigFile* stream, Light30* a2)
{
    return tig_file_fread(a2, sizeof(*a2), 1, stream) == 1;
}

// 0x4DDD90
bool sub_4DDD90(Sector* sector)
{
    // TODO: Incomplete.
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

    TigVideoBufferCreateInfo vb_create_info;
    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY | TIG_VIDEO_BUFFER_CREATE_COLOR_KEY;
    vb_create_info.width = dword_603418;
    vb_create_info.height = dword_60341C;
    vb_create_info.color_key = tig_color_make(0, 255, 0);
    vb_create_info.background_color = vb_create_info.color_key;

    if (tig_video_buffer_create(&vb_create_info, &dword_602E5C) != TIG_OK) {
        return false;
    }

    if (tig_video_buffer_create(&vb_create_info, &dword_602E94) != TIG_OK) {
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

// 0x4DE0B0
bool sub_4DE0B0(tig_art_id_t art_id, TigPaletteModifyInfo* modify_info)
{
    if (!dword_602ECC) {
        return false;
    }

    modify_info->flags = TIG_PALETTE_MODIFY_TINT;
    switch (tig_art_type(art_id )) {
    case TIG_ART_TYPE_TILE:
        if (tig_art_tile_id_type_get(art_id) == 0) {
            modify_info->tint_color = light_indoor_color;
        } else {
            modify_info->tint_color = light_outdoor_color;
        }
        return true;
    case TIG_ART_TYPE_WALL:
        modify_info->tint_color = light_outdoor_color;
        return true;
    case TIG_ART_TYPE_CRITTER:
        modify_info->tint_color = light_outdoor_color;
        return true;
    case TIG_ART_TYPE_PORTAL:
        modify_info->tint_color = light_outdoor_color;
        return true;
    case TIG_ART_TYPE_SCENERY:
        modify_info->tint_color = light_outdoor_color;
        return true;
    case TIG_ART_TYPE_INTERFACE:
        return false;
    case TIG_ART_TYPE_ITEM:
        if (tig_art_item_id_disposition_get(art_id) == TIG_ART_ITEM_DISPOSITION_GROUND) {
            modify_info->tint_color = light_outdoor_color;
            return true;
        }
        return false;
    case TIG_ART_TYPE_CONTAINER:
        modify_info->tint_color = light_outdoor_color;
        return true;
    case TIG_ART_TYPE_MISC:
        return false;
    case TIG_ART_TYPE_LIGHT:
        return false;
    case TIG_ART_TYPE_ROOF:
        modify_info->tint_color = light_outdoor_color;
        return true;
    case TIG_ART_TYPE_FACADE:
        if (tig_art_tile_id_type_get(art_id) == 0) {
            modify_info->tint_color = light_indoor_color;
        } else {
            modify_info->tint_color = light_outdoor_color;
        }
        return true;
    case TIG_ART_TYPE_MONSTER:
        modify_info->tint_color = light_outdoor_color;
        return true;
    case TIG_ART_TYPE_UNIQUE_NPC:
        modify_info->tint_color = light_outdoor_color;
        return true;
    default:
        return false;
    }
}

// 0x4DE200
void sub_4DE200()
{
    if (!dword_60340C) {
        if (dword_602E40 == NULL) {
            dword_602E40 = tig_palette_create();
        }
        tig_palette_fill(dword_602E40, light_indoor_color);

        if (dword_602E88 == NULL) {
            dword_602E88 = tig_palette_create();
        }
        tig_palette_fill(dword_602E88, light_outdoor_color);
    }
}

// 0x4DE250
void sub_4DE250()
{
    if (!dword_60340C) {
        tig_palette_destroy(dword_602E40);
        dword_602E40 = NULL;

        tig_palette_destroy(dword_602E88);
        dword_602E88 = NULL;
    }
}

// 0x4DE290
void light_get_rect_internal(Light30* light, TigRect* rect)
{
    int64_t x;
    int64_t y;
    TigArtFrameData art_frame_data;

    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;

    sub_4B8680(light->loc, &x, &y);
    if (x >= INT_MIN && x < INT_MAX
        && y >= INT_MIN && y < INT_MAX) {
        if (tig_art_frame_data(light->art_id, &art_frame_data) == TIG_OK) {
            rect->x = light->offset_x + (int)x + 40;
            rect->y = light->offset_y + (int)y + 20;
            rect->width = art_frame_data.width;
            rect->height = art_frame_data.height;
            rect->x -= art_frame_data.hot_x;
            rect->y -= art_frame_data.hot_y;
        }
    }
}

// 0x4DE390
void sub_4DE390(Light30* light)
{
    tig_color_t color;
    bool have_color;
    TigPaletteModifyInfo palette_modify_info;
    TigArtAnimData art_anim_data;

    if ((light->flags & 0x8) != 0) {
        color = light_get_indoor_color();
        have_color = true;
    } else if ((light->flags & 0x10) != 0) {
        color = light_get_outdoor_color();
        have_color = true;
    }

    if (have_color) {
        light->r = (uint8_t)tig_color_get_red(color);
        light->g = (uint8_t)tig_color_get_green(color);
        light->b = (uint8_t)tig_color_get_blue(color);
    }

    light->tint_color = tig_color_make(light->r, light->g, light->b);
    if (light->tint_color == tig_color_make(255, 255, 255)) {
        sub_4DE4D0(light);
    } else {
        if (light->palette == NULL) {
            light->palette = tig_palette_create();
        }

        tig_art_anim_data(light->art_id, &art_anim_data);
        palette_modify_info.flags = TIG_PALETTE_MODIFY_TINT;
        palette_modify_info.tint_color = light->tint_color;
        palette_modify_info.src_palette = art_anim_data.palette1;
        palette_modify_info.dst_palette = light->palette;
        tig_palette_modify(&palette_modify_info);
    }
}

// 0x4DE4D0
void sub_4DE4D0(Light30* light)
{
    if (light->palette != NULL) {
        tig_palette_destroy(light->palette);
        light->palette = NULL;
    }
}

// 0x4DE4F0
void sub_4DE4F0(Light30* light, int offset_x, int offset_y)
{
    TigRect dirty_rect;
    TigRect updated_rect;
    int64_t sector_id;
    Sector* sector;

    if (light->offset_x != offset_x || light->offset_y != offset_y) {
        light_get_rect_internal(light, &dirty_rect);
        sector_id = sub_4CFC50(light->loc);
        if (sub_4DD110(light) || sub_4D04E0(sector_id)) {
            sector_lock(sector_id, &sector);
            sub_4F7250(&(sector->lights), light, light->loc, offset_x, offset_y);
            sector_unlock(sector_id);
        } else {
            light->offset_x = offset_x;
            light->offset_y = offset_y;
        }

        light->flags |= 0x80;
        light_get_rect_internal(light, &updated_rect);
        tig_rect_union(&dirty_rect, &updated_rect, &dirty_rect);
        sub_4DF310(&dirty_rect, true);
    }
}

// 0x4DE5D0
bool sub_4DE5D0()
{
    int index;
    tig_art_id_t art_id;
    TigPaletteModifyInfo palette_modify_info;
    TigArtAnimData art_anim_data;

    for (index = 0; index < 1000; index++) {
        if (tig_art_light_id_create(index, 0, 0, 1, &art_id) == TIG_OK
            && tig_art_exists(art_id) == TIG_OK) {
            break;
        }
    }

    if (index == 1000) {
        return false;
    }

    if (tig_art_anim_data(art_id, &art_anim_data) != TIG_OK) {
        return false;
    }

    strcpy(stru_602EE8.name, "art\\light\\shadowmap.bmp");
    if (tig_bmp_create(&stru_602EE8) != TIG_OK) {
        return false;
    }

    if (stru_602EE8.bpp != 8) {
        return false;
    }

    stru_602ED8.width = stru_602EE8.width;
    stru_602ED8.height = stru_602EE8.height;

    palette_modify_info.flags = TIG_PALETTE_MODIFY_TINT;
    palette_modify_info.src_palette = art_anim_data.palette2;

    for (index = 0; index < 7; index++) {
        dword_602E58[index] = tig_palette_create();
        palette_modify_info.dst_palette = dword_602E58[index];
        palette_modify_info.tint_color = tig_color_make(225 - 13 * index, 225 - 13 * index, 225 - 13 * index);
        tig_palette_modify(&palette_modify_info);
    }

    return true;
}

// 0x4DE730
void sub_4DE730()
{
    int index;

    sub_4DE7F0();

    for (index = 0; index < 7; index++) {
        tig_palette_destroy(dword_602E58[index]);
    }

    tig_bmp_destroy(&stru_602EE8);
}

// 0x4DE770
Light602E60* sub_4DE770()
{
    Light602E60* node;

    node = off_602E60;
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
    int index;
    Light602E60* node;

    for (index = 0; index < 20; index++) {
        node = (Light602E60*)MALLOC(sizeof(*node));
        node->next = off_602E60;
        off_602E60 = node;
    }
}

// 0x4DE7F0
void sub_4DE7F0()
{
    Light602E60* curr;
    Light602E60* next;

    curr = off_602E60;
    while (curr != NULL) {
        next = curr->next;
        FREE(curr);
        curr = next;
    }
    off_602E60 = NULL;
}

// 0x4DE820
bool sub_4DE820(TimeEvent* timeevent)
{
    return timeevent->params[0].integer_value == dword_603400;
}

// 0x4DE870
void sub_4DE870(LightCreateInfo* create_info, Light30** light_ptr)
{
    Light30* light;

    if (light_editor || create_info->obj != OBJ_HANDLE_NULL) {
        light = (Light30*)MALLOC(sizeof(*light));
        light->obj = create_info->obj;
        light->loc = create_info->loc;
        light->offset_x = create_info->offset_x;
        light->offset_y = create_info->offset_y;
        light->art_id = create_info->art_id;
        light->flags = create_info->flags;
        light->r = create_info->r;
        light->g = create_info->g;
        light->b = create_info->b;
        light->palette = NULL;
        sub_4DE390(&light);
        *light_ptr = light;
    } else {
        *light_ptr = NULL;
    }
}

// 0x4DE900
void sub_4DE900(UnknownContext* ctx)
{
    // TODO: Incomplete.
}

// 0x4DF1D0
void sub_4DF1D0()
{
    // TODO: Incomplete.
}

// 0x4DF310
void sub_4DF310(TigRect* rect, bool invalidate)
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

    if (invalidate) {
        object_invalidate_rect(&dirty_rect);
    }
}
