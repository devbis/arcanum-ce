#include "tig/message.h"

#include "tig/mouse.h"
#include "tig/movie.h"
#include "tig/tig.h"
#include "tig/timer.h"
#include "tig/video.h"
#include "tig/window.h"

typedef struct TigMessageListNode {
    TigMessage message;
    TigMessageListNode* next;
};

static_assert(sizeof(TigMessageListNode) == 0x20, "wrong size");

// 0x62B1E8
static TigContextMessageHandler* tig_message_handler;

// 0x62B1EC
static bool dword_62B1EC;

// 0x62B278
static CRITICAL_SECTION tig_message_critical_section;

// 0x62B270
static TigContextWindowProc* tig_message_default_window_proc;

// 0x62B274
static bool dword_62B274;

// 0x62B290
static TigMessageListNode* tig_message_empty_node_head;

// 0x62B294
static TigMessageListNode* tig_message_queue_head;

// 0x62B298
static int dword_62B298;

// 0x52B6D0
int tig_message_init(TigContext* ctx)
{
    InitializeCriticalSection(&tig_message_critical_section);

    tig_message_empty_node_head = NULL;
    tig_message_queue_head = NULL;
    dword_62B1EC = true;
    dword_62B298 = 0;
    dword_62B274 = (ctx->flags & TIG_CONTEXT_FLAG_0x20) != 0;

    if (!dword_62B274) {
        tig_message_default_window_proc = NULL;
        tig_message_handler = NULL;
        return TIG_OK;
    }

    if (tig_message_default_window_proc == NULL) {
        return TIG_ERR_16;
    }

    if ((ctx->flags & TIG_CONTEXT_FLAG_0x40) == 0) {
        tig_message_handler = NULL;
        return TIG_OK;
    }

    if (ctx->message_handler == NULL) {
        return TIG_ERR_12;
    }

    tig_message_handler = ctx->message_handler;

    return TIG_OK;
}

// 0x52B750
void tig_message_exit()
{
    while (tig_message_queue_head != NULL) {
        TigMessageListNode* next = tig_message_queue_head->next;
        free(tig_message_queue_head);
        tig_message_queue_head = next;
    }

    while (tig_message_empty_node_head != NULL) {
        TigMessageListNode* next = tig_message_empty_node_head->next;
        free(tig_message_empty_node_head);
        tig_message_empty_node_head = next;
    }

    DeleteCriticalSection(&tig_message_critical_section);
}

// 0x52B7B0
void tig_message_ping()
{
    TigMessage message;
    message.type = TIG_MESSAGE_TYPE_6;
    message.time = tig_ping_time;
    tig_message_enqueue(&message);

    for (int index = 0; index < dword_62B298; index++) {
        // TODO: Incomplete.
    }

    MSG msg;
    if (PeekMessageA(&msg, NULL, 0, 0, TRUE)) {
        if (msg.message == WM_QUIT) {
            tig_message_quit(msg.wParam);
        } else {
            if (tig_message_handler != NULL) {
                if (tig_message_handler(&msg)) {
                    TranslateMessage(&msg);
                    DispatchMessageA(&msg);
                }
            } else {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
    }
}

// 0x52B890
int tig_message_enqueue(TigMessage* message)
{
    EnterCriticalSection(&tig_message_critical_section);

    if (sub_52BF90(message)) {
        TigMessageListNode* node = tig_message_allocate_node();
        node->message = *message;
        node->next = NULL;

        // TODO: Check.
        if (message->type == 8 || tig_message_queue_head == NULL) {
            node->next = tig_message_queue_head;
            tig_message_queue_head = node;
        } else {
            TigMessageListNode* curr = tig_message_queue_head;
            while (curr->next != NULL) {
                curr = curr->next;
            }

            curr->next = node;
        }
    }

    LeaveCriticalSection(&tig_message_critical_section);
    return 0;
}

// 0x52B9D0
LRESULT CALLBACK tig_message_wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    RECT client_rect;
    TigRect rect;
    TigMessage message;

    switch (msg) {
    case WM_CREATE:
        GetClientRect(hWnd, &client_rect);
        ClientToScreen(hWnd, &(client_rect->left));
        ClientToScreen(hWnd, &(client_rect->right));
        tig_video_set_client_rect(&client_rect);
        break;
    case WM_MOVE:
        GetClientRect(hWnd, &client_rect);
        ClientToScreen(hWnd, &(client_rect->left));
        ClientToScreen(hWnd, &(client_rect->right));
        tig_video_set_client_rect(&client_rect);
        sub_51FA40(&rect);
        tig_window_set_needs_display_in_rect(&rect);
        break;
    case WM_PAINT:
        if (GetUpdateRect(hWnd, &client_rect, FALSE)) {
            rect.x = client_rect.left;
            rect.y = client_rect.top;
            rect.width = client_rect.right - client_rect.left;
            rect.height = client_rect.bottom - client_rect.top;
            tig_window_set_needs_display_in_rect(&rect);
            tig_window_display();
        }
        break;
    case WM_CLOSE:
        tig_message_quit(1);
        return FALSE;
    case WM_ERASEBKGND:
        return FALSE;
    case WM_ACTIVATEAPP:
        if (wParam == 1 || wParam == 2) {
            tig_set_active(true);
        } else {
            tig_set_active(false);
        }
        break;
    case WM_CHAR:
        tig_timer_start(&(message.time));
        message.type = TIG_MESSAGE_TYPE_4;
        message.field_8 = wParam & 0xFF;
        tig_message_enqueue(&message);
        break;
    case WM_SYSCOMMAND:
        if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) {
            return FALSE;
        }
        break;
    case WM_MOUSEMOVE:
        if (dword_62B274) {
            // TODO: Check `GET_X_LPARAM` and `GET_Y_LPARAM`.
            tig_mouse_set_position(LOWORD(lParam), HIWORD(lParam), 0);
        }
        break;
    case WM_LBUTTONDOWN:
        if (dword_62B274) {
            tig_mouse_set_button(TIG_MOUSE_BUTTON_LEFT, true);
        }
        break;
    case WM_LBUTTONUP:
        if (dword_62B274) {
            tig_mouse_set_button(TIG_MOUSE_BUTTON_LEFT, false);
        }
        break;
    case WM_RBUTTONDOWN:
        if (dword_62B274) {
            tig_mouse_set_button(TIG_MOUSE_BUTTON_RIGHT, true);
        }
        break;
    case WM_RBUTTONUP:
        if (dword_62B274) {
            tig_mouse_set_button(TIG_MOUSE_BUTTON_RIGHT, false);
        }
        break;
    case WM_MBUTTONDOWN:
        if (dword_62B274) {
            tig_mouse_set_button(TIG_MOUSE_BUTTON_MIDDLE, true);
        }
        break;
    case WM_MBUTTONUP:
        if (dword_62B274) {
            tig_mouse_set_button(TIG_MOUSE_BUTTON_MIDDLE, false);
        }
        break;
    case WM_MOUSEWHEEL:
        if (dword_62B274) {
            tig_mouse_set_position(LOWORD(lParam), HIWORD(lParam), HIWORD(wParam));
        }
        break;
    case TIG_MESSAGE_ID_0x402:
        sub_528E80(hWnd, wParam, lParam);
        return TRUE;
    case TIG_MESSAGE_ID_0x401:
        sub_528FD0(hWnd, wParam, lParam);
        return TRUE;
    }

    if (tig_message_default_window_proc != NULL) {
        return tig_message_default_window_proc(hWnd, msg, wParam, lParam);
    } else {
        return DefWindowProcA(hWnd, msg, wParam, lParam);
    }
}

// 0x52BD90
void tig_message_set_default_window_proc(TigContextWindowProc* func)
{
    tig_message_default_window_proc = func;
}

// 0x52BDA0
int sub_52BDA0(TigMessage* message)
{
    TigMessage temp_message;

    if (tig_message_queue_head == NULL) {
        return TIG_ERR_10;
    }

    EnterCriticalSection(&tig_message_critical_section);

    // TODO: Incomplete.

    memcpy(message, &temp_message, sizeof(TigMessage));
    LeaveCriticalSection(&tig_message_critical_section);

    return TIG_OK;
}

// 0x52BE60
int tig_message_quit(int exit_code)
{
    TigMessage message;
    tig_timer_start(&(message.time));
    message.type = TIG_MESSAGE_TYPE_3;
    message.field_8 = exit_code;
    return tig_message_enqueue(&message);
}

// 0x52BED0
TigMessageListNode* tig_message_allocate_node()
{
    EnterCriticalSection(&tig_message_critical_section);

    tig_message_make_empty_nodes();

    TigMessageListNode* node = tig_message_empty_node_head;
    tig_message_empty_node_head = node->next;
    node->next = NULL;

    LeaveCriticalSection(&tig_message_critical_section);

    return node;
}

// 0x52BF10
void tig_message_make_empty_nodes()
{
    if (tig_message_empty_node_head == NULL) {
        EnterCriticalSection(&tig_message_critical_section);

        for (int index = 0; index < 32; index++) {
            TigMessageListNode* node = (TigMessageListNode*)malloc(sizeof(*node));
            node->next = tig_message_empty_node_head;
            tig_message_empty_node_head = node;
        }

        LeaveCriticalSection(&tig_message_critical_section);
    }
}

// 0x52BF60
void sub_52BF60(TigMessageListNode* node)
{
    EnterCriticalSection(&tig_message_critical_section);

    node->next = tig_message_empty_node_head;
    tig_message_empty_node_head = node;

    LeaveCriticalSection(&tig_message_critical_section);
}

// 0x52BF90
bool sub_52BF90(TigMessage* message)
{
    if (!dword_62B1EC) {
        switch (message->type) {
        case TIG_MESSAGE_TYPE_0:
            if (message->field_14 != 6) {
                return false;
            }
            break;
        case TIG_MESSAGE_TYPE_5:
            return false;
        }
    }

    return true;
}
