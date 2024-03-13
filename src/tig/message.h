#ifndef ARCANUM_TIG_MESSAGE_H_
#define ARCANUM_TIG_MESSAGE_H_

#include "tig/timer.h"
#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TIG_MESSAGE_ID_0x401 (WM_USER + 1)
#define TIG_MESSAGE_ID_0x402 (WM_USER + 2)

typedef int(TigMessageKeyboardCallback)(int);

typedef enum TigMessageType {
    TIG_MESSAGE_MOUSE,
    TIG_MESSAGE_TYPE_1,
    TIG_MESSAGE_TYPE_2,
    TIG_MESSAGE_QUIT,
    TIG_MESSAGE_CHAR,
    TIG_MESSAGE_KEYBOARD,
    TIG_MESSAGE_TYPE_6,
    TIG_MESSAGE_TYPE_7,
    TIG_MESSAGE_TYPE_8,
    TIG_MESSAGE_TYPE_9,
} TigMessageType;

typedef struct TigMouseMessageData {
    int x;
    int y;
    int z;
} TigMouseMessageData;

typedef struct TigQuitMessageData {
    int exit_code;
} TigQuitMessageData;

typedef struct TigKeyboardMessageData {
    int key;
    unsigned char pressed;
} TigKeyboardMessageData;

typedef struct TigCharacterMessageData {
    unsigned char ch;
} TigCharacterMessageData;

typedef struct TigMessage {
    timer_t time;
    int type;
    union {
        struct {
            int field_8;
            int field_C;
            int field_10;
            int field_14;
            int field_18;
        } unknown;
        TigMouseMessageData mouse;
        TigQuitMessageData quit;
        TigCharacterMessageData character;
        TigKeyboardMessageData keyboard;
    } data;
} TigMessage;

static_assert(sizeof(TigMessage) == 0x1C, "wrong size");

int tig_message_init(TigContext* ctx);
void tig_message_exit();
void tig_message_ping();
int tig_message_enqueue(TigMessage* message);
int tig_message_set_key_handler(TigMessageKeyboardCallback* callback, int key);
LRESULT CALLBACK tig_message_wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void tig_message_set_default_window_proc(WNDPROC func);

/// Pulls next message from the game's message queue and returns `TIG_OK`.
///
/// If the queue is empty returns `TIG_ERR_10`.
int tig_message_dequeue(TigMessage* message);

/// Adds `TIG_MESSAGE_QUIT` message to the game's message queue and
/// returns `TIG_OK`.
int tig_message_post_quit(int exit_code);

int sub_52BE90();
int sub_52BEA0();
int sub_52BEB0(bool a1);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_MESSAGE_H_ */
