#include "ui/scrollbar_ui.h"

#define MAX_CONTROLS 8

typedef enum ScrollbarUiControlFlags {
    SB_IN_USE = 0x1,
    SB_HIDDEN = 0x2,
} ScrollbarUiControlFlags;

typedef struct ScrollbarUiControl {
    /* 0000 */ ScrollbarId id;
    /* 0008 */ ScrollbarUiControlFlags flags;
    /* 000C */ ScrollbarUiControlInfo info;
    /* 0050 */ tig_window_handle_t window_handle;
    /* 0054 */ TigWindowData window_data;
    /* 0074 */ tig_button_handle_t button_up;
    /* 0078 */ tig_button_handle_t button_down;
    /* 007C */ int field_7C;
} ScrollbarUiControl;

static_assert(sizeof(ScrollbarUiControl) == 0x80, "wrong size");

// 0x5CBF48
static TigRect stru_5CBF48 = { 0, 0, 11, 5 };

// 0x5CBF58
static TigRect stru_5CBF58 = { 0, 0, 11, 7 };

// 0x5CBF68
static TigRect stru_5CBF68 = { 0, 0, 11, 1 };

// 0x5CBF78
static int dword_5CBF78 = -1;

// 0x684250
static int scrollbar_ui_button_down_width;

// 0x684254
static tig_art_id_t scrollbar_ui_middle_art_id;

// 0x684258
static int scrollbar_ui_button_up_height;

// 0x68425C
static bool initialized;

// 0x684260
static tig_art_id_t scrollbar_ui_top_art_id;

// 0x684268
static ScrollbarUiControl scrollbar_ui_controls[MAX_CONTROLS];

// 0x684668
static tig_art_id_t scrollbar_ui_bottom_art_id;

// 0x68466C
static int scrollbar_ui_button_up_width;

// 0x684670
static int scrollbar_ui_button_down_height;

// 0x684674
static int dword_684674;

// 0x684678
static int dword_684678;

// 0x684684
static int dword_684684;

// 0x580410
bool scrollbar_ui_init(GameInitInfo* init_info)
{
    (void)init_info;

    scrollbar_reset();

    if (tig_art_interface_id_create(238, 0, 0, 0, &scrollbar_ui_top_art_id) != TIG_OK) {
        return false;
    }

    if (tig_art_interface_id_create(240, 0, 0, 0, &scrollbar_ui_bottom_art_id) != TIG_OK) {
        return false;
    }

    if (tig_art_interface_id_create(787, 0, 0, 0, &scrollbar_ui_middle_art_id) != TIG_OK) {
        return false;
    }

    initialized = true;

    return true;
}

// 0x580480
void scrollbar_ui_exit()
{
    initialized = false;
}

// 0x580490
void scrollbar_ui_reset()
{
    int index;

    for (index = 0; index < MAX_CONTROLS; index++) {
        scrollbar_ui_control_reset(&(scrollbar_ui_controls[index]));
    }

    dword_684678 = random_between(0, 8192);
    dword_5CBF78 = -1;
    dword_684684 = 0;
}

// 0x5804E0
void scrollbar_create()
{
    // TODO: Incomplete.
}

// 0x580690
void scrollbar_ui_control_destroy(ScrollbarId id)
{
    ScrollbarUiControl* ctrl;

    if (!sub_5812E0(&id, &ctrl)) {
        return;
    }

    tig_button_destroy(ctrl->button_up);
    tig_button_destroy(ctrl->button_down);

    if (dword_5CBF78 == ctrl->id.index) {
        dword_5CBF78 = -1;
    }

    scrollbar_ui_control_reset(ctrl);
}

// 0x5806F0
void sub_5806F0(ScrollbarId id)
{
    ScrollbarUiControl* ctrl;

    if (!sub_5812E0(&id, &ctrl)) {
        return;
    }

    sub_5807F0(ctrl->id.index, 0);
}

// 0x580720
bool scrollbar_ui_control_show(ScrollbarId id)
{
    ScrollbarUiControl* ctrl;

    if (!sub_5812E0(&id, &ctrl)) {
        return false;
    }

    if ((ctrl->flags & 0x2) == 0) {
        return false;
    }

    ctrl->flags &= ~0x2;

    tig_button_show(ctrl->button_down);
    tig_button_show(ctrl->button_up);
    sub_5807F0(ctrl->id.index, 0);

    return true;
}

// 0x580780
void scrollbar_ui_control_hide(ScrollbarId id)
{
    ScrollbarUiControl* ctrl;

    if (!sub_5812E0(&id, &ctrl)) {
        return false;
    }

    if ((ctrl->flags & 0x2) != 0) {
        return false;
    }

    tig_button_hide(ctrl->button_down);
    tig_button_hide(ctrl->button_up);

    if (ctrl->info.field_40 != NULL) {
        ctrl->info.field_40(&(ctrl->info.field_4));
    }

    return true;
}

// 0x5807F0
void sub_5807F0()
{
    // TODO: Incomplete.
}

// 0x580B10
void sub_580B10()
{
    // TODO: Incomplete.
}

// 0x5810D0
void sub_5810D0()
{
    // TODO: Incomplete.
}

// 0x581180
void sub_581180()
{
    dword_684684++;
}

// 0x581190
void sub_581190()
{
    dword_684684--;
}

// 0x5811A0
void sub_5811A0()
{
    dword_684684 = 1;
}

// 0x5811B0
void sub_5811B0()
{
    dword_684684 = 0;
}

// 0x5811C0
void scrollbar_ui_control_reset(ScrollbarUiControl* ctrl)
{
    ctrl->id.index = -1;
    ctrl->id.index = -1;
    ctrl->flags = 0;
    memset(&(ctrl->info), 0, sizeof(ctrl->info));
    ctrl->window_handle = TIG_WINDOW_HANDLE_INVALID;
    ctrl->button_up = TIG_BUTTON_HANDLE_INVALID;
    ctrl->button_down = TIG_BUTTON_HANDLE_INVALID;
}

// 0x5811F0
void sub_5811F0(ScrollbarUiControl* ctrl, ScrollbarUiControlInfo* info)
{
    ctrl->info = *info;

    if ((ctrl->info.flags & 0x2) == 0) {
        ctrl->info.rect.x = 0;
        ctrl->info.rect.y = 0;
        ctrl->info.rect.width = 0;
        ctrl->info.rect.height = 0;
    }

    if ((ctrl->info.flags & 0x4) == 0) {
        ctrl->info.field_24 = 10;
    }

    if ((ctrl->info.flags & 0x8) == 0) {
        ctrl->info.field_28 = 0;
    }

    if ((ctrl->info.flags & 0x10) == 0) {
        ctrl->info.field_2C = 1;
    }

    if ((ctrl->info.flags & 0x20) == 0) {
        ctrl->info.field_30 = 5;
    }

    if ((ctrl->info.flags & 0x40) == 0) {
        ctrl->info.field_34 = 3;
    }

    if ((ctrl->info.flags & 0x80) == 0) {
        ctrl->info.field_38 = 0;
    }

    if ((ctrl->info.flags & 0x100) == 0) {
        ctrl->info.field_3C = 0;
    }

    if ((ctrl->info.flags & 0x200) == 0) {
        ctrl->info.field_40 = 0;
    }
}

// 0x581280
bool sub_581280(ScrollbarId* id)
{
    int index;

    for (index = 0; index < MAX_CONTROLS; index++) {
        if ((scrollbar_ui_controls[index].flags & 0x1) == 0) {
            id->index = index;
            id->global_index = dword_684678++;
            scrollbar_ui_controls[index].id = *id;
            scrollbar_ui_controls[index].flags = 0x1;
            return true;
        }
    }

    return false;
}

// 0x5812E0
bool sub_5812E0(const ScrollbarId* id, ScrollbarUiControl** ctrl_ptr)
{
    int index;

    if (id->index >= 0 && id->index < MAX_CONTROLS
        && scrollbar_ui_controls[id->index].id.global_index == id->global_index
        && (scrollbar_ui_controls[id->index].flags & 0x1) != 0) {
        *ctrl_ptr = &(scrollbar_ui_controls[id->index]);
        return true;
    }

    for (index = 0; index < MAX_CONTROLS; index++) {
        if (scrollbar_ui_controls[index].id.global_index == id->global_index
            && (scrollbar_ui_controls[id->index].flags & 0x1) != 0) {
            *ctrl_ptr = &(scrollbar_ui_controls[id->index]);
            return true;
        }
    }

    return false;
}

// 0x581360
bool sub_581360(int id, int x, int y)
{
    ScrollbarUiControl* ctrl;
    TigWindowData window_data;

    ctrl = &(scrollbar_ui_controls[id]);
    tig_window_data(ctrl->window_handle, &window_data);

    return x - window_data.rect.x >= ctrl->info.field_4.x
        && x - window_data.rect.x <= ctrl->info.field_4.x + ctrl->info.field_4.width
        && y - window_data.rect.y >= sub_5815D0(id)
        && y - window_data.rect.y <= sub_581660(id);
}

// 0x5813E0
bool sub_5813E0(int id, int x, int y)
{
    ScrollbarUiControl* ctrl;
    TigWindowData window_data;

    ctrl = &(scrollbar_ui_controls[id]);
    tig_window_data(ctrl->window_handle, &window_data);

    return x - window_data.rect.x >= ctrl->info.field_4.x
        && x - window_data.rect.x <= ctrl->info.field_4.x + ctrl->info.field_4.width
        && y - window_data.rect.y >= ctrl->info.rect.y + scrollbar_ui_button_up_height
        && y - window_data.rect.y <= sub_5815D0(id);
}

// 0x581460
bool sub_581460(int id, int x, int y)
{
    ScrollbarUiControl* ctrl;
    TigWindowData window_data;

    ctrl = &(scrollbar_ui_controls[id]);
    tig_window_data(ctrl->window_handle, &window_data);

    return x - window_data.rect.x >= ctrl->info.field_4.x
        && x - window_data.rect.x <= ctrl->info.field_4.x + ctrl->info.field_4.width
        && y - window_data.rect.y >= sub_581660(id)
        && y - window_data.rect.y <= ctrl->info.rect.y + ctrl->info.rect.height - scrollbar_ui_button_down_height;
}

// 0x5814E0
bool sub_5814E0(int id, int x, int y)
{
    ScrollbarUiControl* ctrl;
    TigWindowData window_data;

    ctrl = &(scrollbar_ui_controls[id]);
    tig_window_data(ctrl->window_handle, &window_data);

    return x - window_data.rect.x >= ctrl->info.rect.x
        && x - window_data.rect.x <= ctrl->info.rect.x + ctrl->info.rect.width
        && y - window_data.rect.y >= ctrl->info.rect.y
        && y - window_data.rect.y <= ctrl->info.rect.y + ctrl->info.rect.height;
}

// 0x581550
float sub_581550(int id)
{
    ScrollbarUiControl* ctrl;

    ctrl = &(scrollbar_ui_controls[id]);
    return (float)(ctrl->info.field_4.height - scrollbar_ui_button_down_height - scrollbar_ui_button_up_height)
        / (float)(ctrl->info.field_2C + ctrl->info.field_24 - ctrl->info.field_28)
        * (float)ctrl->info.field_2C;
}

// 0x5815A0
int sub_5815A0(int id)
{
    float v1;

    v1 = sub_581550(id);
    if (v1 > 12.0f) {
        return (int)v1;
    }
    return 12;
}

// 0x5815D0
int sub_5815D0(int id)
{
    return scrollbar_ui_button_up_height
        + scrollbar_ui_controls[id].info.field_4.y
        + (int)(sub_581550(id)
            * (float)(scrollbar_ui_controls[id].info.field_38 - scrollbar_ui_controls[id].info.field_28)
            / (float)(scrollbar_ui_controls[id].info.field_2C));
}

// 0x581660
int sub_581660(int id)
{
    int v1;

    v1 = (int)sub_581550(id);
    return sub_5815D0(id) + (v1 >= 12 ? v1 : 12);
}

// 0x5816A0
bool sub_5816A0(int id, int a2)
{
    return a2 >= scrollbar_ui_controls[id].info.field_28
        && a2 <= scrollbar_ui_controls[id].info.field_24;
}

// 0x5816D0
void sub_5816D0()
{
    dword_684674++;
}

// 0x5816E0
void sub_5816E0()
{
    dword_684674--;
}

// 0x5816F0
int sub_5816F0()
{
    return dword_684674;
}
