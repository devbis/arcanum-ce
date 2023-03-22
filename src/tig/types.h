#ifndef ARCANUM_TIG_TYPES_H_
#define ARCANUM_TIG_TYPES_H_

#include <stdint.h>

#include <windows.h>

#define TIG_CONTEXT_FLAG_FPS 0x0001
#define TIG_CONTEXT_FLAG_DOUBLE_BUFFER 0x2
#define TIG_CONTEXT_FLAG_0x04 0x04
#define TIG_CONTEXT_FLAG_0x10 0x10
#define TIG_CONTEXT_FLAG_0x20 0x20
#define TIG_CONTEXT_FLAG_0x40 0x40
#define TIG_CONTEXT_FLAG_3D_SOFTWARE 0x200
#define TIG_CONTEXT_FLAG_3D_HARDWARE 0x400
#define TIG_CONTEXT_FLAG_3D_REF_DEVICE 0x800
#define TIG_CONTEXT_FLAG_NO_SOUND 0x2000

#define TIG_CONTEXT_FLAG_ANY_3D (TIG_CONTEXT_FLAG_3D_SOFTWARE | TIG_CONTEXT_FLAG_3D_HARDWARE | TIG_CONTEXT_FLAG_3D_REF_DEVICE)

#define TIG_OK 0
#define TIG_NOT_INITIALIZED 1
#define TIG_ALREADY_INITIALIZED 2
#define TIG_ERR_4 4
#define TIG_ERR_5 5
#define TIG_ERR_6 6
#define TIG_ERR_7 7
#define TIG_ERR_10 10
#define TIG_ERR_11 11
#define TIG_ERR_12 12
#define TIG_ERR_13 13
#define TIG_ERR_14 14
#define TIG_ERR_16 16

typedef int(TigArtFilePathResolver)(unsigned int art_id, char* path);
typedef void(TigFilePathResolver)(int a1, char* path);
typedef int(TigContextF20)(unsigned int art_id);
typedef LRESULT(TigContextWindowProc)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef bool(TigContextMessageHandler)(LPMSG msg);

typedef struct TigContext {
    /* 0000 */ unsigned int flags;
    /* 0004 */ int x;
    /* 0008 */ int y;
    /* 000C */ int width;
    /* 0010 */ int height;
    /* 0014 */ int bpp;
    /* 0018 */ HINSTANCE instance;
    /* 001C */ TigArtFilePathResolver* art_file_path_resolver;
    /* 0020 */ TigContextF20* field_20;
    /* 0024 */ TigContextWindowProc* default_window_proc;
    /* 0028 */ TigContextMessageHandler* message_handler;
    /* 002C */ TigFilePathResolver* sound_file_path_resolver;
    /* 0030 */ int field_30;
    /* 0034 */ unsigned int texture_width;
    /* 0038 */ unsigned int texture_height;
    /* 003C */ int field_3C;
};

static_assert(sizeof(TigContext) == 0x40, "wrong size");

typedef struct TigRect {
    int x;
    int y;
    int width;
    int height;
};

#endif /* ARCANUM_TIG_TYPES_H_ */
