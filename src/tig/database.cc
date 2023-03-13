#include "tig/database.h"

#define TIG_DATABASE_FILE_EOF 0x02
#define TIG_DATABASE_FILE_ERROR 0x04
#define TIG_DATABASE_FILE_TEXT_MODE 0x08

#define DECOMPRESSION_BUFFER_SIZE 0x4000

typedef struct DecompressionContext {
    z_stream zstrm;
    uint8_t buffer[DECOMPRESSION_BUFFER_SIZE];
};

// See 0x53CF50.
static_assert(sizeof(DecompressionContext) == 0x4038, "wrong size");

typedef struct TigDatabaseFileHandle {
    unsigned int flags;
    TigDatabase* database;
    TigDatabaseEntry* entry;
    HANDLE hFile;
    int field_10;
    int field_14;
    int field_18;
    DecompressionContext* decompression_context;
    TigDatabaseFileHandle* next;
};

// See 0x53C300.
static_assert(sizeof(TigDatabaseFileHandle) == 0x24, "wrong size");

// 0x63CBC0
static TigDatabase* tig_database_open_databases;

static void sub_53C030(TigDatabaseFindEntryData* find_entry_data);
static void sub_53C740(int a1, int a2);
static int num_path_segments(const char* path);
static int tig_database_find_entry_by_path(const void* a1, const void* a2);
static bool tig_database_fclose_internal(TigDatabaseFileHandle* fp);
static bool tig_database_fopen_internal(TigDatabase* database, TigDatabaseEntry* entry, const char* mode, TigDatabaseFileHandle* fp);

// 0x53BC50
TigDatabase* tig_database_open(const char* path)
{
    // TODO: Incomplete.
}

// 0x53BF40
bool tig_database_close(TigDatabase* database)
{
    TigDatabase* prev_db = NULL;
    TigDatabase* curr_db = tig_database_open_databases;
    while (curr_db != NULL && curr_db != database) {
        prev_db = curr_db;
        curr_db = curr_db->next;
    }

    if (curr_db == NULL) {
        return false;
    }

    if (prev_db != NULL) {
        prev_db->next = curr_db->next;
    } else {
        tig_database_open_databases = curr_db->next;
    }

    TigDatabaseFileHandle* curr_fp = database->open_file_handles;
    while (curr_fp != NULL) {
        TigDatabaseFileHandle* next_fp = curr_fp->next;
        tig_database_fclose(next_fp);
        curr_fp = next_fp;
    }

    free(database->field_28);
    free(database->entries);
    free(database->path);
    free(database);

    return true;
}

// 0x53BFD0
bool tig_database_find_first_entry(TigDatabase* database, const char* pattern, TigDatabaseFindEntryData* find_entry_data)
{
    memset(find_entry_data, 0, sizeof(*find_entry_data));

    find_entry_data->database = database;
    find_entry_data->pattern = strdup(pattern);
    find_entry_data->field_8 = num_path_segments(pattern);

    if (!sub_53E230(pattern)) {
        find_entry_data->field_10 = database->entries_count;
        return false;
    }

    sub_53C030(find_entry_data);
    return sub_53C0D0(find_entry_data);
}

// 0x53C030
void sub_53C030(TigDatabaseFindEntryData* find_entry_data)
{
    // TODO: Incomplete.
}

// 0x53C0D0
bool tig_database_find_next_entry(TigDatabaseFindEntryData* find_entry_data)
{
    // TODO: Incomplete.
}

// 0x53C190
bool tig_database_find_close(TigDatabaseFindEntryData* find_entry_data)
{
    free(find_entry_data->pattern);
    memset(find_entry_data, 0, sizeof(*find_entry_data));
    return true;
}

// 0x53C1C0
int tig_database_filelength(TigDatabaseFileHandle* fp)
{
    return fp->entry->size;
}

// 0x53C1D0
bool tig_database_get_entry(TigDatabase* database, const char* path, TigDatabaseEntry** entry)
{
    *entry = (TigDatabaseEntry*)bsearch(path, database->entries, database->entries_count, sizeof(*database->entries), tig_database_find_entry_by_path);
    if (*entry == NULL) {
        return false;
    }

    if (((*entry)->flags & TIG_DATABASE_ENTRY_IGNORED) != 0) {
        *entry = NULL;
        return false;
    }

    return true;
}

// 0x53C220
int tig_database_fclose(TigDatabaseFileHandle* fp)
{
    int rc = tig_database_fclose_internal(fp) ? 0 : -1;
    free(fp);
    return rc;
}

// 0x53C250
int tig_database_fflush(TigDatabaseFileHandle* fp)
{
    return -1;
}

// 0x53C260
TigDatabaseFileHandle* tig_database_fopen(TigDatabase* database, const char* path, const char* mode)
{
    TigDatabaseEntry* entry;
    if (!tig_database_get_entry(database, path, &entry)) {
        return false;
    }

    return tig_database_fopen_entry(database, entry, mode);
}

// 0x53C2A0
TigDatabaseFileHandle* tig_database_reopen(TigDatabase* database, const char* path, const char* mode, TigDatabaseFileHandle* fp)
{
    tig_database_fclose_internal(fp);

    TigDatabaseEntry* entry;
    if (!tig_database_get_entry(database, path, &entry)) {
        free(fp);
        return NULL;
    }

    if (!tig_database_fopen_internal(database, entry, mode, fp)) {
        free(fp);
        return NULL;
    }

    return fp;
}

// 0x53C300
TigDatabaseFileHandle* tig_database_fopen_entry(TigDatabase* database, TigDatabaseEntry* entry, const char* mode)
{
    TigDatabaseFileHandle* fp = (TigDatabaseFileHandle*)malloc(sizeof(*fp));
    if (!tig_database_fopen_internal(database, entry, mode, fp)) {
        free(fp);
        return NULL;
    }

    return fp;
}

// 0x53C370
int tig_database_setvbuf(TigDatabaseFileHandle* fp, char* buffer, int mode, size_t size)
{
    return 1;
}

// 0x53C3A0
int tig_database_vfprintf(TigDatabaseFileHandle* fp, const char* format, va_list args)
{
    return -1;
}

// 0x53C3B0
int tig_database_fgetc(TigDatabaseFileHandle* fp)
{
    // TODO
}

// 0x53C3F0
char* tig_database_fgets(char* buffer, int max_count, TigDatabaseFileHandle* fp)
{
    int count = 0;
    int ch = '\0';

    while (count < max_count - 1 && ch != '\n') {
        ch = tig_database_fgetc(fp);
        if (ch == -1) {
            if (count == 0) {
                return NULL;
            }
        }

        buffer[count++] = ch;
    }

    buffer[count] = '\0';

    return buffer;
}

// 0x53C440
int tig_database_fputc(int ch, TigDatabaseFileHandle* fp)
{
    return -1;
}

// 0x53C450
int tig_database_fputs(const char* buffer, TigDatabaseFileHandle* fp)
{
    return -1;
}

// 0x53C460
int tig_database_ungetc(int ch, TigDatabaseFileHandle* fp)
{
    if (ch == -1) {
        return -1;
    }

    // TODO: Incomplete.
}

// 0x53C4A0
int tig_database_fread(void* buffer, size_t size, size_t count, TigDatabaseFileHandle* fp)
{
    // TODO: Incomplete.
}

// 0x53C520
int tig_database_fwrite(const void* buffer, size_t size, size_t count, TigDatabaseFileHandle* fp)
{
    return count - 1;
}

// 0x53C550
int tig_database_fseek(TigDatabaseFileHandle* fp, long offset, int origin)
{
    // TODO: Incomplete.
}

// 0x53C6E0
long tig_database_ftell(TigDatabaseFileHandle* fp)
{
    return fp->field_10;
}

// 0x53C6F0
void tig_database_rewind(TigDatabaseFileHandle* fp)
{
    tig_database_clearerr(fp);
    tig_database_fseek(fp, 0, SEEK_SET);
}

// 0x53C710
void tig_database_clearerr(TigDatabaseFileHandle* fp)
{
    fp->flags &= ~(TIG_DATABASE_FILE_EOF | TIG_DATABASE_FILE_ERROR);
}

// 0x53C720
int tig_database_feof(TigDatabaseFileHandle* fp)
{
    return fp->flags & TIG_DATABASE_FILE_EOF;
}

// 0x53C730
int tig_database_ferror(TigDatabaseFileHandle* fp)
{
    return fp->flags & TIG_DATABASE_FILE_ERROR;
}

// 0x53C740
void sub_53C740(int a1, int a2)
{
    dword_63CBC4 = a1;
    dword_63CBBC = a2;
}

// 0x53CE80
int num_path_segments(const char* path)
{
    int count = 0;
    const char* pch = strchr(path, '\\');
    while (pch != NULL) {
        count++;
        pch = strchr(pch + 1, '\\');
    }
    return count;
}

// 0x53CEB0
int tig_database_find_entry_by_path(const void* a1, const void* a2)
{
    const char* path = (const char*)a1;
    const TigDatabaseEntry* entry = (const TigDatabaseEntry*)a2;
    return strcmpi(path, entry->path);
}

// 0x53CED0
bool tig_database_fclose_internal(TigDatabaseFileHandle* fp)
{
    TigDatabaseFileHandle* prev = NULL;
    TigDatabaseFileHandle* curr = fp->database->open_file_handles;
    if (curr == NULL) {
        return false;
    }

    while (curr != fp) {
        prev = curr;
        curr = curr->next;
        if (curr == NULL) {
            return false;
        }
    }

    if (prev != NULL) {
        prev->next = curr->next;
    } else {
        fp->database->open_file_handles = curr->next;
    }

    CloseHandle(fp->hFile);

    if ((fp->entry->flags & TIG_DATABASE_ENTRY_COMPRESSED) != 0) {
        inflateEnd(fp->decompression_context.zstrm);
        free(fp->decompression_context);
    }

    memset(fp, 0, sizeof(*fp));

    return true;
}

// 0x53CF50
bool tig_database_fopen_internal(TigDatabase* database, TigDatabaseEntry* entry, const char* mode, TigDatabaseFileHandle* fp)
{
    if (mode[0] == 'w') {
        return false;
    }

    memset(fp, 0, sizeof(*fp));

    fp->hFile = CreateFileA(database->path, GENERIC_READ, TRUE, NULL, CREATE_NEW | CREATE_ALWAYS, FILE_FLAG_RANDOM_ACCESS, NULL);
    if (fp->hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    if (SetFilePointer(fp->hFile, entry->offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
        return false;
    }

    fp->entry = entry;

    if (mode[1] == 't') {
        fp->flags |= TIG_DATABASE_FILE_TEXT_MODE;
    }

    if ((entry->flags & TIG_DATABASE_ENTRY_COMPRESSED) != 0) {
        fp->decompression_context = (DecompressionContext*)malloc(sizeof(fp->decompression_context));
        fp->decompression_context.zstrm.next_in = fp->decompression_context->buffer;
        fp->decompression_context.zstrm.avail_in = 0;
        fp->decompression_context.zstrm.zalloc = Z_NULL;
        fp->decompression_context.zstrm.zfree = Z_NULL;
        fp->decompression_context.zstrm.opaque = Z_NULL;

        if (inflateInit(&(fp->decompression_context.zstrm), ZLIB_VERSION, sizeof(fp->decompression_context.zstrm)) != Z_OK) {
            free(fp->decompression_context);
            CloseHandle(fp->hFile);
            return false;
        }
    }

    fp->database = database;
    fp->next = database->open_file_handles;
    database->open_file_handles = fp;

    return true;
}
