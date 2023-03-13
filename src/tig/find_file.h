#ifndef ARCANUM_TIG_FIND_FILE_H_
#define ARCANUM_TIG_FIND_FILE_H_

#include <io.h>

typedef struct TigFindFileData {
    _finddata32_t find_data;
    intptr_t handle;
};

// See 0x533D20.
static_assert(sizeof(TigFindFileData) == 0x11C, "wrong size");

bool tig_find_first_file(const char* pattern, TigFindFileData* find_file_data);
bool tig_find_next_file(TigFindFileData* find_file_data);
bool tig_find_close(TigFindFileData* find_file_data);

static inline const char* tig_find_get_name(TigFindFileData* find_file_data)
{
    return find_file_data->find_data.name;
}

static inline bool tig_find_is_directory(TigFindFileData* find_file_data)
{
    return (find_file_data->find_data.attrib & _A_SUBDIR) != 0;
}

#endif /* ARCANUM_TIG_FIND_FILE_H_ */
