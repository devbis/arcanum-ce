#ifndef ARCANUM_TIG_DATABASE_H_
#define ARCANUM_TIG_DATABASE_H_

#include "tig/types.h"

#define TIG_DATABASE_ENTRY_COMPRESSED 0x02
#define TIG_DATABASE_ENTRY_IGNORED 0x800

typedef struct TigDatabaseEntry;
typedef struct TigDatabaseFileHandle;

typedef struct TigDatabase {
    char* path;
    int entries_count;
    TigDatabaseEntry* entries;
    TigDatabaseFileHandle* open_file_handles;
    TigDatabase* next;
    GUID guid;
    int field_24;
    void* field_28;
};

static_assert(sizeof(TigDatabase) == 0x2C, "wrong size");

typedef struct TigDatabaseEntry {
    char* path;
    unsigned int flags;
    int size;
    int field_C;
    int offset;
};

static_assert(sizeof(TigDatabaseEntry) == 0x14, "wrong size");

typedef struct TigDatabaseFindEntryData {
    TigDatabase* database;
    char* pattern;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
};

// See 0x53BFD0.
static_assert(sizeof(TigDatabaseFindEntryData) == 0x1C, "wrong size");

TigDatabase* tig_database_open(const char* path);
bool tig_database_close(TigDatabase* database);
bool tig_database_find_first_entry(TigDatabase* database, const char* pattern, TigDatabaseFindEntryData* find_entry_data);
bool tig_database_find_next_entry(TigDatabaseFindEntryData* find_entry_data);
bool tig_database_find_close(TigDatabaseFindEntryData* find_entry_data);
int tig_database_filelength(TigDatabaseFileHandle* fp);
bool tig_database_get_entry(TigDatabase* database, const char* path, TigDatabaseEntry** entry);
int tig_database_fclose(TigDatabaseFileHandle* fp);
int tig_database_fflush(TigDatabaseFileHandle* fp);
TigDatabaseFileHandle* tig_database_fopen(TigDatabase* database, const char* path, const char* mode);
TigDatabaseFileHandle* tig_database_reopen(TigDatabase* database, const char* path, const char* mode, TigDatabaseFileHandle* fp);
TigDatabaseFileHandle* tig_database_fopen_entry(TigDatabase* database, TigDatabaseEntry* entry, const char* mode);
int tig_database_setvbuf(TigDatabaseFileHandle* fp, char* buffer, int mode, size_t size);
int tig_database_vfprintf(TigDatabaseFileHandle* fp, const char* format, va_list args);
int tig_database_fgetc(TigDatabaseFileHandle* fp);
char* tig_database_fgets(char* buffer, int max_count, TigDatabaseFileHandle* fp);
int tig_database_fputc(int ch, TigDatabaseFileHandle* fp);
int tig_database_fputs(const char* buffer, TigDatabaseFileHandle* fp);
int tig_database_ungetc(int ch, TigDatabaseFileHandle* fp);
int tig_database_fread(void* buffer, size_t size, size_t count, TigDatabaseFileHandle* fp);
int tig_database_fwrite(const void* buffer, size_t size, size_t count, TigDatabaseFileHandle* fp);
int tig_database_fseek(TigDatabaseFileHandle* fp, long offset, int origin);
long tig_database_ftell(TigDatabaseFileHandle* fp);
void tig_database_rewind(TigDatabaseFileHandle* fp);
void tig_database_clearerr(TigDatabaseFileHandle* fp);
int tig_database_feof(TigDatabaseFileHandle* fp);
int tig_database_ferror(TigDatabaseFileHandle* fp);

#endif /* ARCANUM_TIG_DATABASE_H_ */
