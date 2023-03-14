#ifndef ARCANUM_TIG_CACHE_H_
#define ARCANUM_TIG_CACHE_H_

#include "tig/types.h"

typedef struct TigCache;

typedef struct TigCacheEntry {
    void* data;
    int size;
    int index;
    char* path;
};

static_assert(sizeof(TigCacheEntry) == 0x10, "wrong size");

int tig_cache_init(TigContext* ctx);
void tig_cache_exit();
void tig_cache_flush(TigCache* cache);
TigCache* tig_cache_create(int capacity, int max_size);
void tig_cache_destroy(TigCache* cache);
TigCacheEntry* tig_cache_ref(TigCache* cache, const char* path);
void tig_cache_unref(TigCache* cache, TigCacheEntry* entry);

#endif /* ARCANUM_TIG_CACHE_H_ */
