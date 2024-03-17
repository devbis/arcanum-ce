#ifndef ARCANUM_TIG_TYPES_H_
#define ARCANUM_TIG_TYPES_H_

#include <stdbool.h>
#include <stdint.h>

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIG_OK 0
#define TIG_NOT_INITIALIZED 1
#define TIG_ALREADY_INITIALIZED 2
#define TIG_ERR_3 3
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

#define TIG_CONTEXT_FPS 0x0001
#define TIG_CONTEXT_DOUBLE_BUFFER 0x0002
#define TIG_CONTEXT_0x0004 0x0004
#define TIG_CONTEXT_0x0008 0x0008
#define TIG_CONTEXT_0x0010 0x0010
#define TIG_CONTEXT_0x0020 0x0020
#define TIG_CONTEXT_0x0040 0x0040
#define TIG_CONTEXT_0x0080 0x0080
#define TIG_CONTEXT_0x0100 0x0100
#define TIG_CONTEXT_3D_SOFTWARE 0x0200
#define TIG_CONTEXT_3D_HARDWARE 0x0400
#define TIG_CONTEXT_3D_REF_DEVICE 0x0800
#define TIG_CONTEXT_HAVE_MSS_REDIST_PATH 0x1000
#define TIG_CONTEXT_NO_SOUND 0x2000
#define TIG_CONTEXT_HAVE_WINDOW_NAME 0x4000

#define TIG_CONTEXT_ANY_3D (TIG_CONTEXT_3D_SOFTWARE | TIG_CONTEXT_3D_HARDWARE | TIG_CONTEXT_3D_REF_DEVICE)

typedef int(TigArtFilePathResolver)(unsigned int art_id, char* path);
typedef void(TigFilePathResolver)(int a1, char* path);
typedef int(TigContextF20)(unsigned int art_id);
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
    /* 0024 */ WNDPROC default_window_proc;
    /* 0028 */ TigContextMessageHandler* message_handler;
    /* 002C */ TigFilePathResolver* sound_file_path_resolver;
    /* 0030 */ const char* mss_redist_path;
    /* 0034 */ unsigned int texture_width;
    /* 0038 */ unsigned int texture_height;
    /* 003C */ const char* window_name;
} TigContext;

static_assert(sizeof(TigContext) == 0x40, "wrong size");

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_TYPES_H_ */
