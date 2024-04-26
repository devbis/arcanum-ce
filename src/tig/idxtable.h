#ifndef ARCANUM_TIG_MAP_H_
#define ARCANUM_TIG_MAP_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TigFile TigFile;

typedef struct TigMap TigMap;
typedef struct TigMapEntry TigMapEntry;

/// A key/value pair representing entry in a `TigMap`.
///
/// This type is private (visible for testing).
typedef struct TigMapEntry {
    /* 0000 */ void* value;
    /* 0004 */ TigMapEntry* next;
    /* 0008 */ int key;
} TigMapEntry;

static_assert(sizeof(TigMapEntry) == 0xC, "wrong size");

/// A collection whose elements are key/value pairs.
typedef struct TigMap {
    /* 0000 */ int buckets_count;
    /* 0004 */ TigMapEntry** buckets;
    /* 0008 */ int value_size;
    /* 000C */ int count;
} TigMap;

static_assert(sizeof(TigMap) == 0x10, "wrong size");

/// Signature of a callback used by `tig_map_enumerate`.
///
/// The callback should return `true` to continue enumeration, or `false` to
/// stop it.
typedef bool(TigMapEnumerateCallback)(void* value, int key, void* context);

void tig_map_init(TigMap* map, int value_size);
void tig_map_exit(TigMap* map);
void tig_map_copy(TigMap* dst, TigMap* src);
int tig_map_write(TigMap* map, TigFile* stream);
bool tig_map_read(TigMap* map, TigFile* stream);
bool tig_map_enumerate(TigMap* map, TigMapEnumerateCallback* callback, void* context);
void tig_map_set(TigMap* map, int key, void* value);
bool tig_map_get(TigMap* map, int key, void* value);
bool tig_map_contains(TigMap* map, int key);
void tig_map_remove(TigMap* map, int key);
int tig_map_count(TigMap* map);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_MAP_H_ */
