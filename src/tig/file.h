#ifndef ARCANUM_TIG_FILE_H_
#define ARCANUM_TIG_FILE_H_

#include "tig/types.h"

typedef struct TigFile;

typedef struct TigFileInfo {
    char path[MAX_PATH];
    unsigned int attributes;
    size_t size;
    time_t modify_time;
};

static_assert(sizeof(TigFileInfo) == 0x110, "wrong size");

int tig_file_init(TigContext* ctx);
void tig_file_exit();
bool tig_file_repository_add(const char* path);
bool tig_file_repository_remove(const char* path);
bool tig_file_repository_remove_all();
bool tig_file_repository_guid(const char* path, GUID* guid);
int tig_file_mkdir(const char* path);
int tig_file_filelength(TigFile* fp);
bool tig_file_info(const char* path, TigFileInfo* info);
int tig_file_remove(const char* path);
int tig_file_rename(const char* old_path, const char* new_path);
char* tig_file_tmpnam(char* buffer);
int tig_file_fclose(TigFile* fp);
int tig_file_fflush(TigFile* fp);
TigFile* tig_file_fopen(const char* path, const char* mode);
TigFile* tig_file_reopen(const char* path, const char* mode, TigFile* fp);
int tig_file_setvbuf(TigFile* fp, char* buffer, int mode, size_t size);
int tig_file_fprintf(TigFile* fp, const char* format, ...);
int tig_file_vfprintf(TigFile* fp, const char* format, va_list args);
int tig_file_fgetc(TigFile* fp);
char* tig_file_fgets(char* buffer, int max_count, TigFile* fp);
int tig_file_fputc(int ch, TigFile* fp);
int tig_file_fputs(const char* buffer, TigFile* fp);
int tig_file_fread(void* buffer, size_t size, size_t count, TigFile* fp);
int tig_file_fwrite(const void* buffer, size_t size, size_t count, TigFile* fp);
int tig_file_fgetpos(TigFile* fp, long* pos);
int tig_file_fseek(TigFile* fp, long offset, int origin);
int tig_file_fsetpos(TigFile* fp, long* pos);
long tig_file_ftell(TigFile* fp);
void tig_file_rewind(TigFile* fp);
void tig_file_clearerr(TigFile* fp);
int tig_file_feof(TigFile* fp);
int tig_file_ferror(TigFile* fp);
bool tig_file_copy(const char* src, const char* dst);

#endif /* ARCANUM_TIG_FILE_H_ */
