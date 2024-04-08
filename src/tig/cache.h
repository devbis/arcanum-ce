#ifndef ARCANUM_TIG_CACHE_H_
#define ARCANUM_TIG_CACHE_H_

#include <time.h>

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Represents cached file.
typedef struct TigCacheEntry {
    void* data;
    int size;
    int index;
    char* path;
} TigCacheEntry;

static_assert(sizeof(TigCacheEntry) == 0x10, "wrong size");

/// An item in file cache.
typedef struct TigCacheItem {
    TigCacheEntry entry;
    int refcount;
    time_t timestamp;
} TigCacheItem;

static_assert(sizeof(TigCacheItem) == 0x18, "wrong size");

/// A collection of cached files.
///
/// Implements a least-recently-used cache of up to `capacity` files, up to
/// `max_size` size (provided during creation).
typedef struct TigCache {
    int signature;
    int capacity;
    int max_size;
    int bytes;
    int items_count;
    TigCacheItem* items;
} TigCache;

static_assert(sizeof(TigCache) == 0x18, "wrong size");

/// Initializes file cache system.
///
/// NOTE: This function does nothing and always returns `TIG_OK`.
int tig_cache_init(TigInitializeInfo* init_info);

/// Shutdowns file cache system.
///
/// NOTE: This function does nothing. Unlike many other systems which track
/// allocations, file cache objects are not tracked. Which in turn means it
/// does not release existing cache objects. You're responsible for releasing
/// of file cache objects with `tig_cache_destroy`.
void tig_cache_exit();

/// Evicts ununsed entries from cache.
void tig_cache_flush(TigCache* cache);

/// Creates a new file cache.
///
/// - `capacity`: total nubmer of files this cache object can manage.
/// - `max_size`: max size of files this cache object can manage.
TigCache* tig_cache_create(int capacity, int max_size);

/// Destroys the given file cache.
///
/// NOTE: It's an error to have acquired but not released entries, which is a
/// memory leak, but this is neither checked, nor enforced.
void tig_cache_destroy(TigCache* cache);

/// Fetches file with given path from cache loading it from the file system if
/// needed.
TigCacheEntry* tig_cache_acquire(TigCache* cache, const char* path);

/// Releases access to given entry.
void tig_cache_release(TigCache* cache, TigCacheEntry* entry);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_CACHE_H_ */
