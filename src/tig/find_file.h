#ifndef ARCANUM_TIG_FIND_FILE_H_
#define ARCANUM_TIG_FIND_FILE_H_

#include <io.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TigFindFileData {
    struct _finddata_t find_data;
    intptr_t handle;
} TigFindFileData;

// See 0x533D20.
static_assert(sizeof(TigFindFileData) == 0x11C, "wrong size");

bool tig_find_first_file(const char* pattern, TigFindFileData* find_file_data);
bool tig_find_next_file(TigFindFileData* find_file_data);
bool tig_find_close(TigFindFileData* find_file_data);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_FIND_FILE_H_ */
