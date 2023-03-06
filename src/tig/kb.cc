#include "tig/kb.h"

#include "tig/dxinput.h"
#include "tig/tig.h"
#include "tig/video.h"

#define KEYBOARD_DEVICE_DATA_CAPACITY 64

static bool tig_kb_device_init();
static void tig_kb_device_exit();

// 0x62AC90
static bool caps_lock_state;

// 0x62AC94
static bool scroll_lock_state;

// 0x62AC98
static DIDEVICEOBJECTDATA keyboard_buffer[KEYBOARD_DEVICE_DATA_CAPACITY];

// 0x62B098
static bool num_lock_state;

// 0x62B09C
static unsigned char keys[256];

// 0x62B19C
static LPDIRECTINPUTDEVICEA keyboard_device;

// 0x62B1A0
static bool initialized;

// 0x52B2F0
int tig_kb_init(TigContext* ctx)
{
    if (initialized) {
        return 2;
    }

    if (!tig_kb_device_init()) {
        return 7;
    }

    initialized = true;

    return TIG_OK;
}

// 0x52B320
void tig_kb_exit()
{
    if (initialized) {
        tig_kb_device_exit();
        initialized = false;
    }
}

// 0x52B340
bool tig_kb_is_key_pressed(int key)
{
    return keys[key] & 0x80;
}

// 0x52B350
bool tig_kb_get_modifier(int key)
{
    switch (key) {
    case DIK_CAPITAL:
        return caps_lock_state;
    case DIK_SCROLL:
        return scroll_lock_state;
    case DIK_NUMLOCK:
        return num_lock_state;
    }

    return false;
}

// 0x52B380
void tig_kb_ping()
{
    if (initialized) {
        if (tig_get_active()) {
            // TODO: Incomplete.
        }
    }
}

// 0x52B580
bool tig_kb_device_init()
{
    caps_lock_state = GetKeyState(VK_CAPITAL) & 1;
    scroll_lock_state = GetKeyState(VK_SCROLL) & 1;
    num_lock_state = GetKeyState(VK_NUMLOCK) & 1;

    LPDIRECTINPUTA direct_input;
    if (tig_dxinput_get_instance(&direct_input) != TIG_OK) {
        return false;
    }

    if (FAILED(IDirectInput_CreateDevice(direct_input, GUID_SysKeyboard, &keyboard_device, NULL))) {
        return false;
    }

    if (FAILED(IDirectInputDevice_SetDataFormat(keyboard_device, &c_dfDIKeyboard))) {
        tig_kb_device_exit();
        return false;
    }

    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = KEYBOARD_DEVICE_DATA_CAPACITY;
    if (FAILED(IDirectInputDevice_SetProperty(keyboard_device, DIPROP_BUFFERSIZE, &(dipdw.diph)))) {
        tig_kb_device_exit();
        return false;
    }

    HWND wnd;
    if (tig_video_get_hwnd(&wnd) != TIG_OK) {
        tig_kb_device_exit();
        return false;
    }

    if (FAILED(IDirectInputDevice_SetCooperativeLevel(keyboard_device, wnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        tig_kb_device_exit();
        return false;
    }

    return true;
}

// 0x52B6A0
void tig_kb_device_exit()
{
    if (keyboard_device != NULL) {
        IDirectInputDevice_Unacquire(keyboard_device);
        IDirectInputDevice_Release(keyboard_device);
        keyboard_device = NULL;
    }
}
