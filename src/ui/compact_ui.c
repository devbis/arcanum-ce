#include "ui/compact_ui.h"

#include "game/player.h"
#include "ui/anim_ui.h"
#include "ui/dialog_ui.h"
#include "ui/intgame.h"

#define MAX_COMPONENTS 2

typedef struct CompactUiComponent CompactUiComponent;

typedef bool(CompactUiComponentInit)(CompactUiComponent* comp);
typedef void(CompactUiComponentDraw)(int a1);

typedef struct CompactUiComponent {
    /* 0000 */ const char* name;
    /* 0004 */ tig_window_handle_t window_handle;
    /* 0008 */ CompactUiComponentInit* init;
    /* 000C */ void* field_C;
    /* 0010 */ TigWindowMessageFilterFunc* message_filter;
    /* 0014 */ CompactUiComponentDraw* draw;
    /* 0018 */ TigRect rect;
    /* 0028 */ int r;
    /* 002C */ int g;
    /* 0030 */ int b;
    /* 0034 */ int field_34;
} CompactUiComponent;

static_assert(sizeof(CompactUiComponent) == 0x38, "wrong size");

static bool compact_ui_health_bar_init(CompactUiComponent* comp);
static void sub_569050();
static bool compact_ui_health_bar_message_filter(TigMessage* msg);
static void compact_ui_health_bar_draw(int a1);
static void sub_569550(tig_window_handle_t window_handle, TigRect* rect, int color);
static bool compact_ui_hotkey_bar_init(CompactUiComponent* comp);
static void sub_569580();
static bool compact_ui_hotkey_bar_message_filter(TigMessage* msg);
static void compact_ui_hotkey_bar_draw(int a1);

// 0x5CA198
static tig_window_handle_t dword_5CA198 = TIG_WINDOW_HANDLE_INVALID;

// 0x5CA1A0
static TigRect stru_5CA1A0 = { 210, 479, 385, 84 };

// 0x5CA1B0
static CompactUiComponent compact_ui_components[MAX_COMPONENTS] = {
    {
        "Health/Fatigue Bar",
        TIG_WINDOW_HANDLE_INVALID,
        compact_ui_health_bar_init,
        sub_569050,
        compact_ui_health_bar_message_filter,
        compact_ui_health_bar_draw,
        { 751, 498, 49, 102 },
        0,
        155,
        0,
        0,
    },
    {
        "Hotkey Bar",
        TIG_WINDOW_HANDLE_INVALID,
        compact_ui_hotkey_bar_init,
        sub_569580,
        compact_ui_hotkey_bar_message_filter,
        compact_ui_hotkey_bar_draw,
        { 25, 25, 25, 25 },
        0,
        0,
        0,
        0,
    },
};

// 0x67B968
static bool compact_ui_initialized;

// 0x67B96C
static bool compact_ui_created;

// 0x5689D0
bool compact_ui_init(GameInitInfo* init_info)
{
    int index;

    (void)init_info;

    for (index = 0; index < MAX_COMPONENTS; index++) {
        compact_ui_components[index].window_handle = TIG_WINDOW_HANDLE_INVALID;
    }

    compact_ui_created = false;
    compact_ui_initialized = true;

    return true;
}

// 0x568A00
void compact_ui_exit()
{
    int index;

    for (index = 0; index < MAX_COMPONENTS; index++) {
        if (compact_ui_components[index].window_handle != TIG_WINDOW_HANDLE_INVALID) {
            tig_window_destroy(compact_ui_components[index].window_handle);
        }
    }

    compact_ui_initialized = false;
}

// 0x568A30
void compact_ui_reset()
{
}

// 0x568A40
bool compact_ui_create()
{
    int index;
    CompactUiComponent* comp;
    TigWindowData window_data;

    if (compact_ui_initialized) {
        // FIXME: Should be false?
        return true;
    }

    if (compact_ui_created) {
        compact_ui_destroy();
        return true;
    }

    for (index = 0; index < MAX_COMPONENTS; index++) {
        comp = &(compact_ui_components[index]);

        window_data.flags = TIG_WINDOW_FLAG_0x02 | TIG_WINDOW_HAVE_TRANSPARENCY;
        window_data.rect = comp->rect;
        window_data.background_color = tig_color_make(0, 0, 0);
        window_data.color_key = tig_color_make(comp->r, comp->g, comp->b);
        window_data.message_filter = comp->message_filter;

        if (tig_window_create(&window_data, &(comp->window_handle)) != TIG_OK) {
            tig_debug_printf("Compact_UI: could not create window component: %s\n", comp->name);
            return false;
        }

        if (!comp->init(comp)) {
            tig_debug_printf("Compact_UI: could not initialize window component: %s\n", comp->name);
            return false;
        }

        comp->draw(0);
    }

    window_data.rect = stru_5CA1A0;
    window_data.flags = 0;
    window_data.color_key = tig_color_make(5, 5, 5);

    if (tig_window_create(&window_data, &dword_5CA198) != TIG_OK) {
        return false;
    }

    tig_window_hide(dword_5CA198);

    compact_ui_created = true;

    return true;
}

// 0x568C90
bool compact_ui_destroy()
{
    int index;

    for (index = 0; index < MAX_COMPONENTS; index++) {
        if (compact_ui_components[index].window_handle != TIG_WINDOW_HANDLE_INVALID) {
            tig_window_destroy(compact_ui_components[index].window_handle);
        }
    }

    if (dword_5CA198 != TIG_WINDOW_HANDLE_INVALID) {
        tig_window_destroy(dword_5CA198);
        dword_5CA198 = TIG_WINDOW_HANDLE_INVALID;
    }

    compact_ui_created = false;

    return true;
}

// 0x568CF0
void compact_ui_draw()
{
    int index;

    for (index = 0; index < MAX_COMPONENTS; index++) {
        if (compact_ui_components[index].draw != NULL) {
            compact_ui_components[index].draw(0);
        }
    }
}

// 0x568D20
tig_window_handle_t sub_568D20()
{
    TigRect rect;

    if (dword_5CA198 == TIG_WINDOW_HANDLE_INVALID) {
        return TIG_WINDOW_HANDLE_INVALID;
    }

    if (tig_window_is_hidden(dword_5CA198)) {
        rect.x = 0;
        rect.y = 0;
        rect.width = stru_5CA1A0.width;
        rect.height = stru_5CA1A0.height;

        tig_window_fill(dword_5CA198, &rect, tig_color_make(5, 5, 5));
        tig_window_box(dword_5CA198, &rect, tig_color_make(9, 9, 9));

        if (!sub_567400(player_get_pc_obj())) {
            if (tig_window_show(dword_5CA198) != TIG_OK) {
                return TIG_WINDOW_HANDLE_INVALID;
            }
        }
    }

    sub_57D3E0(14, -1);
    sub_57D370(14, -1, 3000);

    return dword_5CA198;
}

// 0x568E70
void sub_568E70()
{
    TigRect rect;

    if (dword_5CA198 == TIG_WINDOW_HANDLE_INVALID) {
        return;
    }

    if (tig_window_is_hidden(dword_5CA198)) {
        return;
    }

    rect.x = 0;
    rect.y = 0;
    rect.width = stru_5CA1A0.width;
    rect.height = stru_5CA1A0.height;
    tig_window_box(dword_5CA198, &rect, tig_color_make(9, 9, 9));
}

// 0x568F20
void sub_568F20()
{
    if (dword_5CA198 != TIG_WINDOW_HANDLE_INVALID) {
        tig_window_hide(dword_5CA198);
    }
}

// 0x568F40
void sub_568F40()
{
    TigRect rect;

    if (dword_5CA198 != TIG_WINDOW_HANDLE_INVALID) {
        rect.x = 0;
        rect.y = 0;
        rect.width = stru_5CA1A0.width;
        rect.height = stru_5CA1A0.height;

        tig_window_fill(dword_5CA198, &rect, tig_color_make(5, 5, 5));
        tig_window_box(dword_5CA198, &rect, tig_color_make(9, 9, 9));
    }

    sub_57D3E0(14, -1);
    sub_568F20();
}

// 0x569040
bool compact_ui_health_bar_init(CompactUiComponent* comp)
{
    (void)comp;

    return true;
}

// 0x569050
void sub_569050()
{
}

// 0x569060
bool compact_ui_health_bar_message_filter(TigMessage* msg)
{
    return sub_54B5D0(msg);
}

// 0x569070
void compact_ui_health_bar_draw(int a1)
{
    // TODO: Incomplete.
}

// 0x569550
void sub_569550(tig_window_handle_t window_handle, TigRect* rect, int color)
{
    tig_window_fill(window_handle, rect, color);
}

// 0x569570
bool compact_ui_hotkey_bar_init(CompactUiComponent* comp)
{
    (void)comp;

    return true;
}

// 0x569580
void sub_569580()
{
}

// 0x569590
bool compact_ui_hotkey_bar_message_filter(TigMessage* msg)
{
    return false;
}

// 0x5695A0
void compact_ui_hotkey_bar_draw(int a1)
{
    (void)a1;
}
