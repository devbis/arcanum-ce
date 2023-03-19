#include "tig/file.h"

#include <direct.h>
#include <stdarg.h>
#include <stdio.h>

#include "tig/database.h"
#include "tig/find_file.h"
#include "tig/tig.h"

#define TIG_FILE_CACHE_DIR_NAME "TIGCache"
#define COPY_BUFFER_SIZE 0x8000

#define TIG_FILE_DATABASE 0x01
#define TIG_FILE_PLAIN 0x02
#define TIG_FILE_DELETE_ON_CLOSE 0x04

typedef struct TigFile {
    int field_0;
    unsigned int flags;
    union {
        TigDatabaseFileHandle* database;
        FILE* plain_file;
    };
};

// See 0x530BD0.
static_assert(sizeof(TigFile) == 0xC, "wrong size");

typedef struct TigFileRepository {
    int type;
    char* path;
    TigDatabase* database;
    TigFileRepository* next;
};

// See 0x52ED40.
static_assert(sizeof(TigFileRepository) == 0x10, "wrong size");

typedef struct S62B2B0 {
    unsigned int flags;
    char* path;
    S62B2B0* next;
};

// See 0x52F370
static_assert(sizeof(S62B2B0) == 0xC, "wrong size");

static TigFile* tig_file_create();
static void tig_file_destroy(TigFile* fp);
static bool tig_file_copy_internal(TigFile* out, TigFile* in);

// 0x62B2AC
static TigFileRepository* tig_file_repositories;

// 0x62B2B0
static S62B2B0* off_62B2B0;

// 0x52ECA0
int tig_file_init(TigContext* ctx)
{
    TigFindFileData find_file_data;
    if (tig_find_first_file("tig.dat", &find_file_data)) {
        tig_file_repository_add("tig.dat");
    } else {
        tig_file_repository_add(tig_get_executable(false));
    }

    tig_file_repository_add(".");

    return TIG_OK;
}

// 0x52ED00
void tig_file_exit()
{
    while (off_62B2B0 != NULL) {
        S62B2B0* next = off_62B2B0->next;
        if (off_62B2B0->path != NULL) {
            free(off_62B2B0->path);
        }
        free(off_62B2B0);
        off_62B2B0 = next;
    }

    tig_file_repository_remove_all();
}

// 0x52ED40
bool tig_file_repository_add(const char* path)
{
    // TODO: Incomplete.
}

// 0x52EF30
bool tig_file_repository_remove(const char* path)
{
    // TODO: Incomplete.
}

// 0x52F000
bool tig_file_repository_remove_all()
{
    TigFileRepository* curr = tig_file_repositories;
    while (curr != NULL) {
        TigFileRepository* next = curr->next;
        if ((curr->type & 1) != 0) {
            tig_database_close(curr->database);
        } else {
            char path[MAX_PATH];
            sprintf(path, "%s\\%s", curr->path, TIG_FILE_CACHE_DIR_NAME);
            tig_file_rmdir(path);
        }

        free(curr->path);
        free(curr);

        curr = next;
    }
    tig_file_repositories = NULL;

    return true;
}

// 0x52F0A0
bool tig_file_repository_guid(const char* path, GUID* guid)
{
    TigFileRepository* curr = tig_file_repositories;
    while (curr != NULL) {
        if ((curr->type & TIG_FILE_DATABASE) != 0) {
            if (strcmpi(curr->database->path, path) == 0) {
                *guid = curr->database->guid;
                return true;
            }
        }
        curr = curr->next;
    }

    return false;
}

// 0x52F100
int tig_file_mkdir(const char* path)
{
    char temp_path[MAX_PATH];
    temp_path[0] = '\0';

    if (path[0] != '.' && path[0] != '.' && path[1] != ':') {
        TigFileRepository* curr = tig_file_repositories;
        while (curr != NULL) {
            if ((curr->type & 2) != 0) {
                strcpy(temp_path, curr->path);
                strcat(temp_path, "\\");
                break;
            }
            curr = curr->next;
        }
    }

    strcat(temp_path, path);

    size_t len = strlen(temp_path);
    if (temp_path[len] == '\\') {
        temp_path[len] = '\0';
    }

    char* pch = strchr(temp_path, '\\');
    while (pch != NULL) {
        *pch = '\0';
        _mkdir(temp_path);
        *pch = '\\';

        pch = strchr(pch + 1, '\\');
    }

    mkdir(temp_path);

    return 0;
}

// 0x52F230
int sub_52F230(const char* path)
{
    int rc = -1;

    char temp_path[MAX_PATH];
    temp_path[0] = '\0';

    if (path[0] != '.' && path[0] != '.' && path[1] != ':') {
        TigFileRepository* curr = tig_file_repositories;
        while (curr != NULL) {
            if ((curr->type & 2) != 0) {
                strcpy(temp_path, curr->path);
                strcat(temp_path, "\\");
                strcat(temp_path, path);
                if (rmdir(temp_path) == 0) {
                    rc = 0;
                }
            }
            curr = curr->next;
        }
    } else {
        strcat(temp_path, path);
        rc = rmdir(temp_path);
    }

    return rc;
}

// 0x52F370
void sub_52F370(const char* path, unsigned int flags)
{
    if ((flags & (TIG_FILE_DATABASE | TIG_FILE_PLAIN)) != 0) {
        if (sub_53E230(path)) {
            S62B2B0* node = (S62B2B0*)malloc(sizeof(*node));
            node->flags = flags;
            node->path = strdup(path);
            node->next = off_62B2B0;
            off_62B2B0 = node;

            // TODO: Likely wrong, check.
            size_t len = strlen(node->path);
            if (len > 3) {
                char* pch = &(node->path[len - 3]);
                if (strcmp(pch, "*.*") == 0) {
                    pch[1] = '\0';
                }
            }
        }
    }
}

// 0x52F760
void tig_file_list_create(TigFileList* file_list, const char* pattern)
{
    // TODO: Incomplete.
}

// 0x52FB20
void tig_file_list_destroy(TigFileList* file_list)
{
    if (file_list->entries != NULL) {
        free(file_list->entries);
    }

    file_list->count = 0;
    file_list->entries = NULL;
}

// 0x52FB40
int tig_file_filelength(TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_filelength(fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return _filelength(_fileno(fp->plain_file));
    }

    return -1;
}

// 0x52FB80
bool tig_file_info(const char* path, TigFileInfo* info)
{
    // TODO: Incomplete.
}

// 0x5300F0
int tig_file_remove(const char* path)
{
    if (path[0] == '.' || path[0] == '\\' || path[1] == ':') {
        return remove(path);
    }

    // TODO: Incomplete.
}

// 0x5301F0
int tig_file_rename(const char* old_path, const char* new_path)
{
    if (old_path[0] == '.' || old_path[0] == '\\' || old_path[1] == ':') {
        return rename(old_path, new_path);
    }

    // TODO: Incomplete.
}

// 0x530320
char* tig_file_tmpnam(char* buffer)
{
    return tmpnam(buffer);
}

// 0x530330
int tig_file_fclose(TigFile* fp)
{
    int rc = tig_file_close_internal(fp);
    tig_file_destroy(fp);
    return rc == 0 ? true : false;
}

// 0x530360
int tig_file_fflush(TigFile* fp)
{
    if (fp == NULL) {
        // TODO: Check.
        return fflush(stdin);
    }

    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_fflush(fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return fflush(fp->plain_file);
    }

    return -1;
}

// 0x5303A0
TigFile* tig_file_fopen(const char* path, const char* mode)
{
    TigFile* fp = tig_file_create();
    if (!sub_530C70(path, mode, fp)) {
        tig_file_destroy(fp);
        return NULL;
    }

    return fp;
}

// 0x5303D0
TigFile* tig_file_reopen(const char* path, const char* mode, TigFile* fp)
{
    tig_file_close_internal(fp);

    if (!tig_file_open_internal(path, mode, fp)) {
        tig_file_destroy(fp);
        return NULL;
    }

    return fp;
}

// 0x530440
int tig_file_setvbuf(TigFile* fp, char* buffer, int mode, size_t size)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_setvbuf(fp->database, buffer, mode, size);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return setvbuf(fp->plain_file, buffer, mode, size);
    }

    return 1;
}

// 0x530490
int tig_file_fprintf(TigFile* fp, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int rc = tig_file_vfprintf(fp, format, args);
    va_end(args);
    return rc;
}

// 0x5304B0
int sub_5304B0()
{
    return -1;
}

// 0x5304C0
int tig_file_vfprintf(TigFile* fp, const char* format, va_list args)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_vfprintf(fp->database, format, args);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return vfprintf(fp->plain_file, format, args);
    }

    return -1;
}

// 0x530510
int tig_file_fgetc(TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_fgetc(fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return fgetc(fp->plain_file);
    }

    return -1;
}

// 0x530540
char* tig_file_fgets(char* buffer, int max_count, TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_fgets(buffer, max_count, fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return fgets(buffer, max_count, fp->plain_file);
    }

    return NULL;
}

// 0x530580
int tig_file_fputc(int ch, TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_fputc(ch, fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return fputc(ch, fp->plain_file);
    }

    return -1;
}

// 0x5305C0
int tig_file_fputs(const char* buffer, TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_fputs(buffer, fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return fputs(buffer, fp->plain_file);
    }

    return -1;
}

// 0x530640
int tig_file_fread(void* buffer, size_t size, size_t count, TigFile* fp)
{
    if (size == 0 || count == 0) {
        return 0;
    }

    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_fread(buffer, size, count, fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return fread(buffer, size, count, fp->plain_file);
    }

    return count - 1;
}

// 0x5306A0
int tig_file_fwrite(const void* buffer, size_t size, size_t count, TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_fwrite(buffer, size, count, fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return fwrite(buffer, size, count, fp->plain_file);
    }

    return count - 1;
}

// 0x5306F0
int tig_file_fgetpos(TigFile* fp, long* pos)
{
    long v1 = tig_file_ftell(fp);
    if (v1 == -1) {
        return 1;
    }

    *pos = v1;

    return 0;
}

// 0x530720
int tig_file_fseek(TigFile* fp, long offset, int origin)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_fseek(fp->database, offset, origin);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return fseek(fp->plain_file, offset, origin);
    }

    return 1;
}

// 0x530770
int tig_file_fsetpos(TigFile* fp, long* pos)
{
    return tig_file_fseek(fp, *pos, SEEK_SET);
}

// 0x530790
long tig_file_ftell(TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_ftell(fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return ftell(fp->plain_file);
    }

    return -1;
}

// 0x5307C0
void tig_file_rewind(TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        tig_database_rewind(fp->database);
    } else if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        rewind(fp->plain_file);
    }
}

// 0x5307F0
void tig_file_clearerr(TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        tig_database_clearerr(fp->database);
    } else if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        clearerr(fp->plain_file);
    }
}

// 0x530820
int tig_file_feof(TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_feof(fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return feof(fp->plain_file);
    }

    return 0;
}

// 0x530850
int tig_file_ferror(TigFile* fp)
{
    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        return tig_database_ferror(fp->database);
    }

    if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        return ferror(fp->plain_file);
    }

    return 0;
}

// 0x530BD0
TigFile* tig_file_create()
{
    TigFile* fp = (TigFile*)malloc(sizeof(*fp));
    fp->field_0 = 0;
    fp->flags = 0;
    fp->plain_file = 0;
    return fp;
}

// 0x530BF0
void tig_file_destroy(TigFile* fp)
{
    free(fp);
}

// 0x530C00
bool tig_file_close_internal(TigFile* fp)
{
    bool success = 0;

    if ((fp->flags & TIG_FILE_DATABASE) != 0) {
        if (tig_database_ferror(fp->database) == 0) {
            success = true;
        }
    } else if ((fp->flags & TIG_FILE_PLAIN) != 0) {
        if (fclose(fp->plain_file) == 0) {
            success = true;
        }
    }

    if ((fp->flags & TIG_FILE_DELETE_ON_CLOSE) != 0) {
        tig_file_remove(fp->field_0);
        free(fp->field_0);
        fp->field_0 = NULL;
        fp->flags &= ~TIG_FILE_DELETE_ON_CLOSE;
    }

    return success;
}

// 0x530C70
bool tig_file_open_internal(const char* path, const char* mode, TigFile* fp)
{
    fp->flags &= ~(TIG_FILE_DATABASE | TIG_FILE_PLAIN);

    if (path[0] == '.' || path[0] == '\\' || path[1] == ':') {
        fp->plain_file = fopen(path, mode);
        if (fp->plain_file == NULL) {
            return false;
        }

        fp->flags |= TIG_FILE_PLAIN;

        return true;
    }
    // TODO: Incomplete.
}

// 0x5311D0
bool tig_file_copy(const char* src, const char* dst)
{
    if (!tig_file_info(src, NULL)) {
        return false;
    }

    TigFile* in = tig_file_fopen(src, "rb");
    if (in == NULL) {
        return false;
    }

    TigFile* out = tig_file_fopen(dst, "wb");
    if (out == NULL) {
        tig_file_fclose(in);
        return false;
    }

    if (!tig_file_copy_internal(out, in)) {
        tig_file_fclose(out);
        tig_file_fclose(in);
        return false;
    }

    tig_file_fclose(out);
    tig_file_fclose(in);
    return true;
}

// 0x531250
bool tig_file_copy_internal(TigFile* out, TigFile* in)
{
    uint8_t buffer[COPY_BUFFER_SIZE];

    // TODO: Incomplete.
}

// 0x531320
void tig_file_rmdir(const char* path)
{
    char temp_path[MAX_PATH];
    strcpy(temp_path, path);
    strcat(temp_path, "\\*.*");

    TigFindFileData find_file_data;
    if (tig_find_first_file(temp_path, &find_file_data)) {
        do {
            const char* name = tig_find_get_name(&find_file_data);
            bool is_directory = tig_find_is_directory(&find_file_data);

            strcpy(temp_path, path);
            strcat(temp_path, "\\");
            strcat(temp_path, name);

            if (is_directory) {
                if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
                    tig_file_rmdir(path);
                }
            } else {
                remove(temp_path);
            }
        } while (tig_find_next_file(&find_file_data));
    }
    tig_find_close(&find_file_data);

    rmdir(path);
}
