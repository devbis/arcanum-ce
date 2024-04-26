#include "tig/idxtable.h"

#include <string.h>

#include "tig/file.h"
#include "tig/memory.h"

/// Number of buckets allocated for each `TigMap` object.
#define MAX_BUCKETS 63

/// Magic 32 bit number in the file stream denoting start of serialized `TigMap`
/// representation.
#define START_SENTINEL 0xAB1EE1BA

/// Magic 32 bit number in the file stream denoting end of serialized `TigMap`
/// representation.
#define END_SENTINEL 0xE1BAAB1E

static void tig_map_entry_destroy(TigMapEntry* entry);
static TigMapEntry* tig_map_entry_create(int size);
static TigMapEntry* tig_map_entry_copy(TigMapEntry* src, int size);
static int tig_map_entry_count(TigMapEntry* entry);
static bool tig_map_entry_read(TigMapEntry** entry_ptr, int size, int count, TigFile* stream);

// 0x534320
void tig_map_init(TigMap* map, int value_size)
{
    int index;

    map->buckets_count = MAX_BUCKETS;
    map->buckets = (TigMapEntry**)MALLOC(sizeof(map->buckets) * MAX_BUCKETS);

    for (index = 0; index < map->buckets_count; index++) {
        map->buckets[index] = NULL;
    }

    map->count = 0;
    map->value_size = value_size;
}

// 0x534370
void tig_map_exit(TigMap* map)
{
    int index;
    TigMapEntry* entry;
    TigMapEntry* next;

    for (index = 0; index < map->buckets_count; index++) {
        // NOTE: Original code is slightly different.
        entry = map->buckets[index];
        while (entry != NULL) {
            next = entry->next;
            tig_map_entry_destroy(entry);
            entry = next;
        }
    }

    FREE(map->buckets);
}

// 0x5343C0
void tig_map_copy(TigMap* dst, TigMap* src)
{
    int index;

    dst->value_size = src->value_size;
    dst->buckets_count = src->buckets_count;
    dst->buckets = (TigMapEntry**)MALLOC(sizeof(TigMapEntry*) * src->buckets_count);

    for (index = 0; index < src->buckets_count; index++) {
        dst->buckets[index] = tig_map_entry_copy(src->buckets[index], dst->value_size);
    }

    dst->count = src->count;
}

// 0x534430
int tig_map_write(TigMap* map, TigFile* stream)
{
    int grd;
    int index;
    int count;
    TigMapEntry* entry;
    int bytes_written = 0;

    grd = START_SENTINEL;
    bytes_written += tig_file_fwrite(&grd, sizeof(grd), 1, stream);

    bytes_written += tig_file_fwrite(&(map->buckets_count), sizeof(map->buckets_count), 1, stream);
    bytes_written += tig_file_fwrite(&(map->value_size), sizeof(map->value_size), 1, stream);

    for (index = 0; index < map->buckets_count; index++) {
        count = tig_map_entry_count(map->buckets[index]);
        bytes_written += tig_file_fwrite(&count, sizeof(count), 1, stream);

        entry = map->buckets[index];
        while (entry != NULL) {
            bytes_written += tig_file_fwrite(&(entry->key), sizeof(entry->key), 1, stream);
            bytes_written += tig_file_fwrite(entry->value, map->value_size, 1, stream);
            entry = entry->next;
        }
    }

    grd = END_SENTINEL;
    bytes_written += tig_file_fwrite(&grd, sizeof(grd), 1, stream);

    return bytes_written;
}

// 0x534520
bool tig_map_read(TigMap* map, TigFile* stream)
{
    int grd;
    int index;
    int count;
    TigMapEntry* entry;
    TigMapEntry* next;

    if (tig_file_fread(&grd, sizeof(grd), 1, stream) != sizeof(grd)) {
        return false;
    }

    if (grd != START_SENTINEL) {
        return false;
    }

    if (tig_file_fread(&(map->buckets_count), sizeof(map->buckets_count), 1, stream) != sizeof(map->buckets_count)) {
        return false;
    }

    if (tig_file_fread(&(map->value_size), sizeof(map->value_size), 1, stream) != sizeof(map->value_size)) {
        return false;
    }

    map->count = 0;

    for (index = 0; index < map->buckets_count; index++) {
        entry = map->buckets[index];
        while (entry != NULL) {
            next = entry->next;
            tig_map_entry_destroy(entry);
            entry = next;
        }

        if (tig_file_fread(&count, sizeof(count), 1, stream) != sizeof(count)) {
            return false;
        }

        if (!tig_map_entry_read(&(map->buckets[index]), map->value_size, count, stream)) {
            return false;
        }

        map->count += count;
    }

    if (tig_file_fread(&grd, sizeof(grd), 1, stream) != sizeof(grd)) {
        return false;
    }

    if (grd != END_SENTINEL) {
        return false;
    }

    return true;
}

// 0x534650
bool tig_map_enumerate(TigMap* map, TigMapEnumerateCallback* callback, void* context)
{
    int index;
    TigMapEntry* entry;

    for (index = 0; index < map->buckets_count; index++) {
        entry = map->buckets[index];
        while (entry != NULL) {
            if (!callback(entry->value, entry->key, context)) {
                return false;
            }
            entry = entry->next;
        }
    }

    return true;
}

// 0x5346A0
void tig_map_set(TigMap* map, int key, void* value)
{
    int bucket;
    TigMapEntry* entry;

    if (key < 0) {
        key = -key;
    }

    bucket = key % map->buckets_count;
    entry = map->buckets[bucket];
    while (entry != NULL) {
        if (entry->key == key) {
            memcpy(entry->value, value, map->value_size);
            return;
        }
        entry = entry->next;
    }

    entry = tig_map_entry_create(map->value_size);
    memcpy(entry->value, value, map->value_size);
    entry->key = key;
    entry->next = map->buckets[bucket];
    map->buckets[bucket] = entry;

    map->count++;
}

// 0x534730
bool tig_map_get(TigMap* map, int key, void* value)
{
    int bucket;
    TigMapEntry* entry;

    if (key < 0) {
        key = -key;
    }

    bucket = key % map->buckets_count;
    entry = map->buckets[bucket];
    while (entry != NULL) {
        if (entry->key == key) {
            memcpy(value, entry->value, map->value_size);
            return true;
        }
        entry = entry->next;
    }

    return false;
}

// 0x534780
bool tig_map_contains(TigMap* map, int key)
{
    int bucket;
    TigMapEntry* entry;

    if (key < 0) {
        key = -key;
    }

    bucket = key % map->buckets_count;
    entry = map->buckets[bucket];
    while (entry != NULL) {
        if (entry->key == key) {
            return true;
        }
        entry = entry->next;
    }

    return false;
}

// 0x5347C0
void tig_map_remove(TigMap* map, int key)
{
    int bucket;
    TigMapEntry** entry_ptr;
    TigMapEntry* entry;

    if (key < 0) {
        key = -key;
    }

    // NOTE: Original implementation is different.
    bucket = key % map->buckets_count;
    entry_ptr = &(map->buckets[bucket]);

    while (*entry_ptr != NULL) {
        entry = *entry_ptr;
        if (entry->key == key) {
            *entry_ptr = entry->next;
            tig_map_entry_destroy(entry);
            map->count--;
            break;
        }
        entry_ptr = &(entry->next);
    }
}

// 0x534840
void tig_map_entry_destroy(TigMapEntry* entry)
{
    FREE(entry->value);
    FREE(entry);
}

// 0x534860
TigMapEntry* tig_map_entry_create(int size)
{
    TigMapEntry* entry;

    entry = (TigMapEntry*)MALLOC(sizeof(*entry));
    entry->value = MALLOC(size);

    return entry;
}

// 0x534880
TigMapEntry* tig_map_entry_copy(TigMapEntry* entry, int size)
{
    TigMapEntry* copy;

    if (entry == NULL) {
        return NULL;
    }

    copy = tig_map_entry_create(size);
    memcpy(copy->value, entry->value, size);
    copy->next = tig_map_entry_copy(entry->next, size);

    // FIXME: Key is not copied (see failing unit test).

    return copy;
}

// 0x5348D0
int tig_map_entry_count(TigMapEntry* entry)
{
    return entry != NULL ? tig_map_entry_count(entry->next) + 1 : 0;
}

// 0x5348F0
bool tig_map_entry_read(TigMapEntry** entry_ptr, int size, int count, TigFile* stream)
{
    int index;

    for (index = 0; index < count; index++) {
        *entry_ptr = tig_map_entry_create(size);
        if (tig_file_fread(&((*entry_ptr)->key), sizeof((*entry_ptr)->key), 1, stream) != sizeof((*entry_ptr)->key)) {
            tig_map_entry_destroy(*entry_ptr);
            return false;
        }

        if (tig_file_fread(&((*entry_ptr)->value), size, 1, stream) != size) {
            tig_map_entry_destroy(*entry_ptr);
            return false;
        }
        entry_ptr++;
    }

    *entry_ptr = NULL;
    return true;
}

// 0x534990
int tig_map_count(TigMap* map)
{
    return map->count;
}
