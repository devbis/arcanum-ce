#include "tig/kb.h"

#include "tig/dxinput.h"
#include "tig/message.h"
#include "tig/tig.h"
#include "tig/video.h"

#define KEYBOARD_DEVICE_DATA_CAPACITY 64

static bool tig_kb_device_init();
static void tig_kb_device_exit();

// 0x62AC90
static bool tig_kb_caps_lock_state;

// 0x62AC94
static bool tig_kb_scroll_lock_state;

// 0x62AC98
static DIDEVICEOBJECTDATA tig_kb_keyboard_buffer[KEYBOARD_DEVICE_DATA_CAPACITY];

// 0x62B098
static bool tig_kb_num_lock_state;

// 0x62B09C
static unsigned char tig_kb_pressed_keys[256];

// 0x62B19C
static LPDIRECTINPUTDEVICEA tig_kb_keyboard_device;

// 0x62B1A0
static bool tig_kb_initialized;

// 0x52B2F0
int tig_kb_init(TigContext* ctx)
{
    if (tig_kb_initialized) {
        return TIG_ALREADY_INITIALIZED;
    }

    if (!tig_kb_device_init()) {
        return TIG_ERR_7;
    }

    tig_kb_initialized = true;

    return TIG_OK;
}

// 0x52B320
void tig_kb_exit()
{
    if (tig_kb_initialized) {
        tig_kb_device_exit();
        tig_kb_initialized = false;
    }
}

// 0x52B340
bool tig_kb_is_key_pressed(int key)
{
    return tig_kb_pressed_keys[key] & 0x80;
}

// 0x52B350
bool tig_kb_get_modifier(int key)
{
    switch (key) {
    case DIK_CAPITAL:
        return tig_kb_caps_lock_state;
    case DIK_SCROLL:
        return tig_kb_scroll_lock_state;
    case DIK_NUMLOCK:
        return tig_kb_num_lock_state;
    }

    return false;
}

// 0x52B380
void tig_kb_ping()
{
    if (!tig_kb_initialized) {
        return;
    }

    if (!tig_get_active()) {
        return;
    }

    TigMessage message;
    message.time = tig_ping_time;
    message.type = TIG_MESSAGE_TYPE_5;

    DWORD count = KEYBOARD_DEVICE_DATA_CAPACITY;
    if (FAILED(IDirectInputDevice_GetDeviceData(tig_kb_keyboard_device, sizeof(*tig_kb_keyboard_buffer), tig_kb_keyboard_buffer, &count, 0))) {
        // TODO: Incomplete.
    }

    for (DWORD index = 0; index < count; index++) {
        int key = tig_kb_keyboard_buffer[index].dwOfs;
        unsigned char pressed = (unsigned char)tig_kb_keyboard_buffer[index].dwData;
        if (tig_kb_pressed_keys[key] != pressed) {
            tig_kb_pressed_keys[key] = pressed;

            // TODO: Looks odd, check.
            bool v1 = true;
            if (key != DIK_LMENU && key != DIK_RMENU && (tig_kb_pressed_keys[DIK_LMENU] != 0 || tig_kb_pressed_keys[DIK_RMENU] != 0)) {
                v1 = false;
            }

            if ((key == DIK_LCONTROL || key == DIK_RCONTROL || (tig_kb_pressed_keys[DIK_LCONTROL] == 0 && tig_kb_pressed_keys[DIK_RCONTROL] == 0)) && v1) {
                message.keyboard.key = key;
                message.keyboard.pressed = pressed;
            }

            if (message.keyboard.pressed == 1) {
                // NOTE: Original code is slightly different, uses the
                // following pattern:
                //
                // ```c
                // state = 1 - state
                // ```
                switch (key) {
                case DIK_CAPITAL:
                    tig_kb_caps_lock_state = !tig_kb_caps_lock_state;
                    break;
                case DIK_SCROLL:
                    tig_kb_scroll_lock_state = !tig_kb_scroll_lock_state;
                    break;
                case DIK_NUMLOCK:
                    tig_kb_num_lock_state = !tig_kb_num_lock_state;
                    break;
                }
            }
        }
    }
}

// 0x52B580
bool tig_kb_device_init()
{
    tig_kb_caps_lock_state = GetKeyState(VK_CAPITAL) & 1;
    tig_kb_scroll_lock_state = GetKeyState(VK_SCROLL) & 1;
    tig_kb_num_lock_state = GetKeyState(VK_NUMLOCK) & 1;

    LPDIRECTINPUTA dinput;
    if (tig_dxinput_get_instance(&dinput) != TIG_OK) {
        return false;
    }

    if (FAILED(IDirectInput_CreateDevice(dinput, GUID_SysKeyboard, &tig_kb_keyboard_device, NULL))) {
        return false;
    }

    if (FAILED(IDirectInputDevice_SetDataFormat(tig_kb_keyboard_device, &c_dfDIKeyboard))) {
        tig_kb_device_exit();
        return false;
    }

    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = KEYBOARD_DEVICE_DATA_CAPACITY;
    if (FAILED(IDirectInputDevice_SetProperty(tig_kb_keyboard_device, DIPROP_BUFFERSIZE, &(dipdw.diph)))) {
        tig_kb_device_exit();
        return false;
    }

    HWND wnd;
    if (tig_video_get_hwnd(&wnd) != TIG_OK) {
        tig_kb_device_exit();
        return false;
    }

    if (FAILED(IDirectInputDevice_SetCooperativeLevel(tig_kb_keyboard_device, wnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        tig_kb_device_exit();
        return false;
    }

    return true;
}

// 0x52B6A0
void tig_kb_device_exit()
{
    if (tig_kb_keyboard_device != NULL) {
        IDirectInputDevice_Unacquire(tig_kb_keyboard_device);
        IDirectInputDevice_Release(tig_kb_keyboard_device);
        tig_kb_keyboard_device = NULL;
    }
}
