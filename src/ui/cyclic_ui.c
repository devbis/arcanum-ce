#include "ui/cyclic_ui.h"

#include "game/mes.h"

#define MAX_CONTROLS 32

typedef struct CyclicUiControl {
    /* 0000 */ CyclicUiControlInfo info;
    /* 002C */ tig_button_handle_t left_button_handle;
    /* 0030 */ tig_button_handle_t right_button_handle;
    /* 0034 */ mes_file_handle_t mes_file;
    /* 0038 */ int value;
    /* 003C */ int max_value;
    /* 0040 */ unsigned char in_use; // boolean
} CyclicUiControl;

static_assert(sizeof(CyclicUiControl) == 0x44, "wrong size");

static bool sub_57FA70(CyclicUiControl** ctrl_ptr, int id, const char* function_name);
static bool sub_57FAD0(CyclicUiControl* ctrl);
static bool sub_57FB20(CyclicUiControl* ctrl);
static bool cyclic_ui_draw_base(CyclicUiControl* ctrl);
static bool cyclic_ui_base_aid(CyclicUiControl* ctrl, tig_art_id_t* art_id_ptr);
static bool cyclic_ui_draw_empty_slot(CyclicUiControl* ctrl);
static bool cyclic_ui_draw_title(CyclicUiControl* ctrl);
static bool cyclic_ui_buttons_create(CyclicUiControl* ctrl);
static void cyclic_ui_buttons_destroy(CyclicUiControl* ctrl);
static bool cyclic_ui_refresh_level(CyclicUiControl* ctrl);
static bool cyclic_ui_draw_bar(CyclicUiControl* ctrl);
static bool cyclic_ui_draw_text(CyclicUiControl* ctrl);
static void sub_580390(CyclicUiControl* ctrl);
static void sub_5803D0(CyclicUiControl* ctrl);

// 0x6839B8
static tig_font_handle_t dword_6839B8;

// 0x6839C0
static CyclicUiControl cyclic_ui_controls[MAX_CONTROLS];

// 0x684240
static tig_font_handle_t dword_684240;

// 0x684244
static tig_font_handle_t dword_684244;

// 0x684248
static int cyclic_ui_prev_id;

// 0x68424C
static bool cyclic_ui_initialized;

// 0x57F4D0
bool cyclic_ui_init(GameInitInfo* init_info)
{
    int index;
    TigFont font_desc;

    (void)init_info;

    if (cyclic_ui_initialized) {
        return true;
    }

    for (index = 0; index < MAX_CONTROLS; index++) {
        cyclic_ui_controls[index].in_use = false;
        cyclic_ui_controls[index].left_button_handle = TIG_BUTTON_HANDLE_INVALID;
        cyclic_ui_controls[index].right_button_handle = TIG_BUTTON_HANDLE_INVALID;
        cyclic_ui_controls[index].mes_file = MES_FILE_HANDLE_INVALID;
    }

    cyclic_ui_prev_id = MAX_CONTROLS - 1;

    if (tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id)) != TIG_OK) {
        return false;
    }

    font_desc.flags = TIG_FONT_CENTERED;
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 0, 0);
    tig_font_create(&font_desc, &dword_684244);

    if (tig_art_interface_id_create(27, 0, 0, 0, &(font_desc.art_id)) != TIG_OK) {
        return false;
    }

    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_6839B8);

    font_desc.color = tig_color_make(130, 130, 130);
    tig_font_create(&font_desc, &dword_684240);

    cyclic_ui_initialized = true;

    return true;
}

// 0x57F680
void cyclic_ui_exit()
{
    int index;

    if (!cyclic_ui_initialized) {
        return;
    }

    for (index = 0; index < MAX_CONTROLS; index++) {
        if (cyclic_ui_controls[index].in_use) {
            cyclic_ui_control_destroy(index, true);
        }
    }

    tig_font_destroy(dword_684244);
    tig_font_destroy(dword_6839B8);
}

// 0x57F6D0
bool sub_57F6D0(tig_button_handle_t button_handle)
{
    int index;
    CyclicUiControl* ctrl;

    for (index = 0; index < MAX_CONTROLS; index++) {
        ctrl = &(cyclic_ui_controls[index]);
        if (ctrl->in_use) {
            if (button_handle == ctrl->left_button_handle) {
                sub_580390(ctrl);
                return true;
            } else if (button_handle == ctrl->right_button_handle) {
                sub_5803D0(ctrl);
                return true;
            }
        }
    }

    return false;
}

// 0x57F720
void sub_57F720(CyclicUiControlInfo* info)
{
    info->x = 0;
    info->y = 0;
    info->type = 0;
    info->max_value = 0;
    info->text = NULL;
    info->mes_file_path = NULL;
    info->text_array = NULL;
    info->text_array_size = 0;
    info->value_changed_callback = NULL;
    info->window_handle = TIG_WINDOW_HANDLE_INVALID;
    info->visible = true;
    info->enabled = true;
}

// 0x57F750
bool cyclic_ui_control_create(CyclicUiControlInfo* info, int* id_ptr)
{
    int index;
    CyclicUiControl* ctrl;

    index = cyclic_ui_prev_id + 1;
    if (index >= MAX_CONTROLS) {
        index = 0;
    }

    while (index != cyclic_ui_prev_id && cyclic_ui_controls[index].in_use) {
        index++;
        if (index == MAX_CONTROLS) {
            index = 0;
        }
    }

    if (index == cyclic_ui_prev_id) {
        tig_debug_println("Error, cyclic_ui_control_create:  Too many cyclic ui controls.");
        return false;
    }

    ctrl = &(cyclic_ui_controls[index]);
    ctrl->info = *info;
    ctrl->value = 0;

    switch (ctrl->info.type) {
    case 0:
        ctrl->max_value = 10;
        break;
    case 1:
        if (!mes_load(ctrl->info.mes_file_path, &(ctrl->mes_file))) {
            tig_debug_printf("Error, cyclic_ui_control_create:  Unable to load message file [%s]\n", ctrl->info.mes_file_path);
            return false;
        }
        ctrl->max_value = mes_num_entries(ctrl->mes_file) - 1;
        break;
    case 2:
        if (ctrl->info.text_array == NULL) {
            tig_debug_println("Error, cyclic_ui_control_create:  Control type is cui_text_array, but text_array is NULL");
            return false;
        }
        ctrl->max_value = ctrl->info.text_array_size - 1;
        break;
    default:
        tig_debug_println("Error, cyclic_ui_control_create:  Invalid control type");
        return false;
    }

    if (ctrl->info.max_value != 0) {
        ctrl->max_value = ctrl->info.max_value;
    }

    ctrl->in_use = true;

    if (ctrl->info.visible) {
        if (!sub_57FAD0(ctrl)) {
            ctrl->in_use = false;
            return false;
        }
    }

    cyclic_ui_prev_id = index;
    *id_ptr = index;

    return true;
}

// 0x57F8A0
void cyclic_ui_control_destroy(int id, bool a2)
{
    CyclicUiControl* ctrl;

    if (!sub_57FA70(&ctrl, id, __FUNCTION__)) {
        return;
    }

    if (ctrl->info.type == 1) {
        mes_unload(ctrl->mes_file);
    }

    if (a2) {
        ctrl->left_button_handle = TIG_BUTTON_HANDLE_INVALID;
        ctrl->right_button_handle = TIG_BUTTON_HANDLE_INVALID;
        ctrl->in_use = false;
    } else {
        if (ctrl->info.visible) {
            sub_57FB20(ctrl);
        }
        ctrl->in_use = false;
    }
}

// 0x57F910
void cyclic_ui_control_show(int id, bool visible)
{
    CyclicUiControl* ctrl;

    if (!sub_57FA70(&ctrl, id, __FUNCTION__)) {
        return;
    }

    if (visible != ctrl->info.visible) {
        return;
    }

    if (visible) {
        if (sub_57FAD0(ctrl)) {
            ctrl->info.visible = true;
        }
    } else {
        if (sub_57FB20(ctrl)) {
            ctrl->info.visible = false;
        }
    }
}

// 0x57F970
void cyclic_ui_control_enable(int id, bool enabled)
{
    CyclicUiControl* ctrl;

    if (!sub_57FA70(&ctrl, id, __FUNCTION__)) {
        return;
    }

    if (enabled == ctrl->info.enabled) {
        return;
    }

    ctrl->info.enabled = enabled;

    if (!ctrl->info.visible) {
        return;
    }

    if (!ctrl->info.enabled) {
        cyclic_ui_buttons_destroy(ctrl);
    }

    sub_57FAD0(ctrl);
}

// 0x57F9D0
void cyclic_ui_control_set(int id, int value)
{
    CyclicUiControl* ctrl;

    if (!sub_57FA70(&ctrl, id, __FUNCTION__)) {
        return;
    }

    if (value < 0) {
        tig_debug_println("Error, cyclic_ui_control_set:  value is sub-zero");
        return;
    }

    if (value > ctrl->max_value) {
        tig_debug_printf("Error, cyclic_ui_control_set:  value exceeds limit (v:%d l:%d)\n",
            value,
            ctrl->max_value);
        return;
    }

    ctrl->value = value;

    if (ctrl->info.visible) {
        cyclic_ui_refresh_level(ctrl);
    }
}

// 0x57FA40
int cyclic_ui_control_get(int id)
{
    CyclicUiControl* ctrl;

    if (!sub_57FA70(&ctrl, id, __FUNCTION__)) {
        return 0;
    }

    return ctrl->value;
}

// 0x57FA70
bool sub_57FA70(CyclicUiControl** ctrl_ptr, int id, const char* function_name)
{
    if (id < 0 || id >= MAX_CONTROLS) {
        tig_debug_printf("Error, %s:  id out of range\n", function_name);
        *ctrl_ptr = NULL;
        return false;
    }

    if (!cyclic_ui_controls[id].in_use) {
        tig_debug_printf("Error, %s:  id not in use\n", function_name);
        *ctrl_ptr = NULL;
        return false;
    }

    *ctrl_ptr = &(cyclic_ui_controls[id]);
    return true;
}

// 0x57FAD0
bool sub_57FAD0(CyclicUiControl* ctrl)
{
    if (!cyclic_ui_draw_base(ctrl)) {
        return false;
    }

    if (!cyclic_ui_draw_title(ctrl)) {
        return false;
    }

    if (!cyclic_ui_refresh_level(ctrl)) {
        return false;
    }

    if (ctrl->info.enabled) {
        if (!cyclic_ui_buttons_create(ctrl)) {
            return false;
        }
    }

    return true;
}

// 0x57FB20
bool sub_57FB20(CyclicUiControl* ctrl)
{
    if (ctrl->info.enabled) {
        cyclic_ui_buttons_destroy(ctrl);
    }

    return cyclic_ui_draw_empty_slot(ctrl);
}

// 0x57FB50
bool cyclic_ui_draw_base(CyclicUiControl* ctrl)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    if (!cyclic_ui_base_aid(ctrl, &art_id)) {
        return false;
    }

    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_draw_base:  Unable to get frame data");
        return false;
    }

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = ctrl->info.x;
    dst_rect.y = ctrl->info.y;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    tig_art_interface_id_create(669, 0, 0, 0, &art_id);

    blit_info.flags = 0;
    blit_info.art_id = art_id;
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;

    if (tig_window_blit_art(ctrl->info.window_handle, &blit_info) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_draw_base:  Blit failed");
        return false;
    }

    return true;
}

// 0x57FC20
bool cyclic_ui_base_aid(CyclicUiControl* ctrl, tig_art_id_t* art_id_ptr)
{
    int num;
    tig_art_id_t art_id;

    switch (ctrl->info.type) {
    case 0:
        num = 668;
        break;
    case 1:
    case 2:
        num = 671;
        break;
    default:
        tig_debug_println("Error, cyclic_ui_base_aid:  Invalid control type");
        return false;
    }

    if (tig_art_itnerface_id_create(num, 0, 0, 0, art_id) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_base_aid:  Can't get aid");
        return false;
    }

    *art_id_ptr = art_id;
    return true;
}

// 0x57FC90
bool cyclic_ui_draw_empty_slot(CyclicUiControl* ctrl)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    if (!cyclic_ui_base_aid(ctrl, &art_id)) {
        return false;
    }

    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_draw_empty_slot:  Unable to get frame data");
        return false;
    }

    src_rect.x = 261;
    src_rect.y = 81;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = ctrl->info.x;
    dst_rect.y = ctrl->info.y;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    tig_art_itnerface_id_create(669, 0, 0, 0, &art_id);

    blit_info.flags = 0;
    blit_info.art_id = art_id;
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;

    if (tig_window_blit_art(ctrl->info.window_handle, &blit_info) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_draw_empty_slot:  Blit failed");
        return false;
    }

    return true;
}

// 0x57FD80
bool cyclic_ui_draw_title(CyclicUiControl* ctrl)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigRect text_rect;

    if (!cyclic_ui_base_aid(ctrl, &art_id)) {
        return false;
    }

    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_draw_title:  Unable to get base frame data for size reference");
        return false;
    }

    if (ctrl->info.text != NULL && ctrl->info.text[0] != '\0') {
        text_rect.x = ctrl->info.x;
        text_rect.y = ctrl->info.y + 2;
        text_rect.width = art_frame_data.width;
        text_rect.height = art_frame_data.height - 2;

        if (ctrl->info.enabled) {
            tig_font_push(dword_6839B8);
        } else {
            tig_font_push(dword_684240);
        }

        if (tig_window_text_write(ctrl->info.window_handle, ctrl->info.text, &text_rect) != TIG_OK) {
            tig_debug_println("Error, cyclic_ui_draw_title:  Text write failed");
            tig_font_pop();
            return false;
        }

        tig_font_pop();
    }

    return true;
}

// 0x57FE60
bool cyclic_ui_buttons_create(CyclicUiControl* ctrl)
{
    TigButtonData button_data;

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = ctrl->info.window_handle;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    if (tig_art_interface_id_create(667, 0, 0, 0, &(button_data.art_id)) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_buttons_create:  Can't get aid for left button art");
        return false;
    }

    button_data.x = ctrl->info.x + 6;
    button_data.y = ctrl->info.y + 21;

    if (tig_button_create(&button_data, &(ctrl->left_button_handle)) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_buttons_create:  Failed to create left button");
        return false;
    }

    if (tig_art_interface_id_create(668, 0, 0, 0, &(button_data.art_id)) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_buttons_create:  Can't get aid for right button art");
        return false;
    }

    button_data.x = ctrl->info.x + 6;
    button_data.y = ctrl->info.y + 21;

    if (tig_button_create(&button_data, &(ctrl->right_button_handle)) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_buttons_create:  Failed to create right button");
        return false;
    }

    return true;
}

// 0x57FF80
void cyclic_ui_buttons_destroy(CyclicUiControl* ctrl)
{
    tig_button_destroy(ctrl->left_button_handle);
    tig_button_destroy(ctrl->right_button_handle);
    ctrl->left_button_handle = TIG_BUTTON_HANDLE_INVALID;
    ctrl->right_button_handle = TIG_BUTTON_HANDLE_INVALID;
}

// 0x57FFB0
bool cyclic_ui_refresh_level(CyclicUiControl* ctrl)
{
    switch (ctrl->info.type) {
    case 0:
        return cyclic_ui_draw_bar(ctrl);
    case 1:
    case 2:
        return cyclic_ui_draw_text(ctrl);
    }

    tig_debug_println("Error, cyclic_ui_refresh_level:  Invalid control type");
    return false;
}

// 0x580000
bool cyclic_ui_draw_bar(CyclicUiControl* ctrl)
{
    // TODO: Incomplete.
}

// 0x580190
bool cyclic_ui_draw_text(CyclicUiControl* ctrl)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigRect text_rect;
    MesFileEntry mes_file_entry;
    const char* str;

    if (tig_art_interface_id_create(623, 0, 0, 0, &art_id) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_draw_text:  Unable to get liquid aid for size reference");
        return false;
    }

    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_draw_text:  Unable to get liquid frame data for size reference");
        return false;
    }

    if (!cyclic_ui_base_aid(ctrl, &art_id)) {
        return false;
    }

    src_rect.x = 47;
    src_rect.y = 24;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = ctrl->info.x + 47;
    dst_rect.y = ctrl->info.y + 24;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    if (tig_window_blit_art(ctrl->info.window_handle, &art_blit_info) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_draw_text:  Erase blit failed");
        return false;
    }

    switch (ctrl->info.type) {
    case 1:
        mes_file_entry.num = ctrl->value;
        if (mes_search(ctrl->mes_file, &mes_file_entry)) {
            mes_get_msg(ctrl->mes_file, &mes_file_entry);
            str = mes_file_entry.str;
        } else {
            str = NULL;
        }
        break;
    case 2:
        str = ctrl->info.text_array[ctrl->value];
        if (str != NULL && *str == '\0') {
            str = NULL;
        }
        break;
    }

    if (str == NULL) {
        tig_debug_printf("Warning, cyclic_ui_draw_text:  [%s] is missing line %d\n",
            ctrl->info.mes_file_path,
            ctrl->value);
        return true;
    }

    text_rect.x = ctrl->info.x + 47;
    text_rect.y = ctrl->info.y + 30;
    text_rect.width = art_frame_data.width;
    text_rect.height = art_frame_data.height;

    tig_font_push(dword_684244);
    if (tig_window_text_write(ctrl->info.window_handle, str, &text_rect) != TIG_OK) {
        tig_debug_println("Error, cyclic_ui_draw_text:  Text write failed");
        tig_font_pop();
        return false;
    }

    tig_font_pop();
    return true;
}

// 0x580390
void sub_580390(CyclicUiControl* ctrl)
{
    if (ctrl->value > 0) {
        ctrl->value--;
    } else {
        if (ctrl->info.type == 0) {
            return;
        }

        ctrl->value = ctrl->max_value;
    }

    if (ctrl->info.value_changed_callback != NULL) {
        ctrl->info.value_changed_callback(ctrl->value);
    }

    cyclic_ui_refresh_level(ctrl);
}

// 0x5803D0
void sub_5803D0(CyclicUiControl* ctrl)
{
    if (ctrl->value < ctrl->max_value) {
        ctrl->value++;
    } else {
        if (ctrl->info.type == 0) {
            return;
        }

        ctrl->value = 0;
    }

    if (ctrl->info.value_changed_callback != NULL) {
        ctrl->info.value_changed_callback(ctrl->value);
    }

    cyclic_ui_refresh_level(ctrl);
}
