#ifndef ARCANUM_TIG_TYPES_H_
#define ARCANUM_TIG_TYPES_H_

#include <windows.h>

#define TIG_OK 0

typedef void(TigFilePathResolver)(int a1, char* path);

typedef struct TigContext {
    /* 0000 */ unsigned int flags;
    /* 0018 */ HINSTANCE instance;
    /* 002C */ TigFilePathResolver* sound_file_path_resolver;
};

#endif /* ARCANUM_TIG_TYPES_H_ */
