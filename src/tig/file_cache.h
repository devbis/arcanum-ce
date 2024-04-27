#ifndef TIG_FILE_CACHE_H_
#define TIG_FILE_CACHE_H_

#include <time.h>

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Represents cached file.
typedef struct TigFileCacheEntry {
    void* data;
    int size;
    int index;
    char* path;
} TigFileCacheEntry;

static_assert(sizeof(TigFileCacheEntry) == 0x10, "wrong size");

// An item in file cache.
typedef struct TigFileCacheItem {
    TigFileCacheEntry entry;
    int refcount;
    time_t timestamp;
} TigFileCacheItem;

static_assert(sizeof(TigFileCacheItem) == 0x18, "wrong size");

// A collection of cached files.
//
// Implements a least-recently-used cache of up to `capacity` files, up to
// `max_size` size (provided during creation).
typedef struct TigFileCache {
    int signature;
    int capacity;
    int max_size;
    int bytes;
    int items_count;
    TigFileCacheItem* items;
} TigFileCache;

static_assert(sizeof(TigFileCache) == 0x18, "wrong size");

// Initializes file cache system.
//
// NOTE: This function does nothing and always returns `TIG_OK`.
int tig_file_cache_init(TigInitializeInfo* init_info);

// Shutdowns file cache system.
//
// NOTE: This function does nothing. Unlike many other systems which track
// allocations, file cache objects are not tracked. Which in turn means it
// does not release existing cache objects. You're responsible for releasing
// of file cache objects with `tig_file_cache_destroy`.
void tig_file_cache_exit();

// Evicts ununsed entries from cache.
void tig_file_cache_flush(TigFileCache* cache);

// Creates a new file cache.
//
// - `capacity`: total nubmer of files this cache object can manage.
// - `max_size`: max size of files this cache object can manage.
TigFileCache* tig_file_cache_create(int capacity, int max_size);

// Destroys the given file cache.
//
// NOTE: It's an error to have acquired but not released entries, which is a
// memory leak, but this is neither checked, nor enforced.
void tig_file_cache_destroy(TigFileCache* cache);

// Fetches file with given path from cache loading it from the file system if
// needed.
TigFileCacheEntry* tig_file_cache_acquire(TigFileCache* cache, const char* path);

// Releases access to given entry.
void tig_file_cache_release(TigFileCache* cache, TigFileCacheEntry* entry);

#ifdef __cplusplus
}
#endif

#endif /* TIG_FILE_CACHE_H_ */
