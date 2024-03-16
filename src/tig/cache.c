#include "tig/cache.h"

#include "tig/file.h"
#include "tig/memory.h"

static void tig_cache_entry_remove(TigCache* cache, TigCacheItem* entry);
static bool tig_cache_read_contents_into(const char* path, void** data, int* size);
static bool tig_cache_prepare_item(TigCache* cache, TigCacheItem* item, const char* path);
static TigCacheItem* sub_538D80(TigCache* cache);
static TigCacheItem* sub_538D90(TigCache* cache);
static TigCacheItem* sub_538E00(TigCache* cache);
static void tig_cache_shrink(TigCache* cache, int size);
static TigCacheEntry* tig_cache_acquire_internal(TigCache* cache, TigCacheItem* item);
static void tig_cache_release_internal(TigCache* cache, TigCacheItem* entry);

// 0x6364F8
static int tig_cache_hit_count;

// 0x6364FC
static int tig_cache_miss_count;

// 0x636500
static int tig_cache_hit_bytes;

// 0x636504
static int tig_cache_miss_bytes;

// 0x636508
static int tig_cache_removed_bytes;

// 0x538A80
int tig_cache_init(TigContext* ctx)
{
    (void)ctx;

    return TIG_OK;
}

// 0x538A90
void tig_cache_exit()
{
}

// 0x538AA0
void tig_cache_flush(TigCache* cache)
{
    int index;

    for (index = 0; index < cache->capacity; index++) {
        if (cache->items[index].refcount == 0) {
            tig_cache_entry_remove(cache, &(cache->items[index]));
        }
    }
}

// 0x538AE0
void tig_cache_entry_remove(TigCache* cache, TigCacheItem* item)
{
    if (item->entry.data != NULL) {
        cache->items_count--;
        cache->bytes -= item->entry.size;
        tig_cache_removed_bytes += item->entry.size;

        if (item->entry.path) {
            FREE(item->entry.path);
            item->entry.path = NULL;
        }

        FREE(item->entry.data);
        item->entry.data = NULL;

        memset(item, 0, sizeof(*item));
    }
}

// 0x538B50
TigCache* tig_cache_create(int capacity, int max_size)
{
    TigCache* cache;

    cache = (TigCache*)MALLOC(sizeof(*cache));
    cache->signature = 'FILC';
    cache->capacity = capacity;
    cache->max_size = max_size;
    cache->items = (TigCacheItem*)CALLOC(sizeof(*cache->items), capacity);
    cache->items_count = 0;
    cache->bytes = 0;
    return cache;
}

// 0x538BA0
void tig_cache_destroy(TigCache* cache)
{
    tig_cache_flush(cache);
    FREE(cache->items);
    FREE(cache);
}

// 0x538BC0
bool tig_cache_read_contents_into(const char* path, void** data, int* size)
{
    TigFile* stream;

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        return false;
    }

    *size = tig_file_filelength(stream);
    *data = MALLOC(*size);
    tig_file_fread(*data, *size, 1, stream);
    tig_file_fclose(stream);

    return true;
}

// 0x538C20
bool tig_cache_prepare_item(TigCache* cache, TigCacheItem* item, const char* path)
{
    if (!tig_cache_read_contents_into(path, &(item->entry.data), &(item->entry.size))) {
        return false;
    }

    item->entry.path = STRDUP(path);

    cache->items_count++;
    cache->bytes += item->entry.size;
    item->entry.index = cache->items - item;

    return true;
}

// 0x538C90
TigCacheEntry* tig_cache_acquire(TigCache* cache, const char* path)
{
    // 0x6364E8
    static TigCacheEntry null_entry;

    int index;
    TigCacheItem* item;
    TigCacheItem* new_item = NULL;
    TigCacheEntry* entry;

    for (index = 0; index < cache->capacity; index++) {
        item = &(cache->items[index]);
        if (item->entry.data != NULL) {
            if (_strcmpi(item->entry.path, path) == 0) {
                tig_cache_hit_count++;
                tig_cache_hit_bytes += item->entry.size;
                return tig_cache_acquire_internal(cache, item);
            }
        } else {
            if (new_item == NULL) {
                new_item = item;
            }
        }
    }

    if (new_item == NULL) {
        new_item = sub_538D80(cache);
        if (new_item == NULL) {
            return &null_entry;
        }
    }

    if (!tig_cache_prepare_item(cache, new_item, path)) {
        return &null_entry;
    }

    tig_cache_miss_count++;
    tig_cache_miss_bytes += new_item->entry.size;

    entry = tig_cache_acquire_internal(cache, new_item);

    if (cache->bytes > cache->max_size) {
        tig_cache_shrink(cache, cache->bytes - cache->max_size);
    }

    return entry;
}

// 0x538D80
TigCacheItem* sub_538D80(TigCache* cache)
{
    return sub_538D90(cache);
}

// 0x538D90
TigCacheItem* sub_538D90(TigCache* cache)
{
    int attempt;
    int index;
    TigCacheItem* entry;

    for (attempt = 0; attempt < 4 * cache->capacity; attempt++) {
        index = rand() % cache->capacity;
        entry = &(cache->items[index]);
        if (entry->entry.data == NULL) {
            return entry;
        }

        if (entry->refcount == 0) {
            tig_cache_entry_remove(cache, entry);
            return entry;
        }
    }

    return sub_538E00(cache);
}

// 0x538E00
TigCacheItem* sub_538E00(TigCache* cache)
{
    int index;
    TigCacheItem* entry;

    for (index = 0; index < cache->capacity; index++) {
        entry = &(cache->items[index]);
        if (entry->entry.data == NULL) {
            return entry;
        }

        if (entry->refcount == 0) {
            tig_cache_entry_remove(cache, entry);
            return entry;
        }
    }

    return NULL;
}

// 0x538E40
void tig_cache_shrink(TigCache* cache, int size)
{
    int attempt;

    if (size > cache->bytes) {
        tig_cache_flush(cache);
    } else {
        tig_cache_removed_bytes = 0;

        for (attempt = 0; attempt < 4 * cache->capacity; attempt++) {
            sub_538D90(cache);
            if (tig_cache_removed_bytes >= size) {
                break;
            }
        }
    }
}

// 0x538E90
TigCacheEntry* tig_cache_acquire_internal(TigCache* cache, TigCacheItem* item)
{
    (void)cache;

    item->refcount++;
    return &(item->entry);
}

// 0x538EA0
void tig_cache_release(TigCache* cache, TigCacheEntry* entry)
{
    tig_cache_release_internal(cache, &(cache->items[entry->index]));
}

// 0x538EC0
void tig_cache_release_internal(TigCache* cache, TigCacheItem* item)
{
    (void)cache;

    time(&(item->timestamp));
    item->refcount--;
}
