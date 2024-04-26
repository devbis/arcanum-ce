#ifndef TIG_IDXTABLE_H_
#define TIG_IDXTABLE_H_

// IDXTABLE
// ---
//
// The IDXTABLE module provides `IdxTable` object which resembles
// `std::unordered_map<int, T>` from C++ STL.
//
// NOTES
//
// - The `IdxTable` uses integers as keys. Negative integer keys are the same as
// positive, that is:
//
// ```
// idxtable[100] == idxtable[-100];
// ```
//
// - Internally `IdxTable` uses a number of buckets to make lookup a little bit
// more efficient in large collections. The number of buckets is considered
// implementation detail and should not be changed. FYI it's 63 in Arcanum and
// 256 in ToEE.

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TigFile TigFile;

typedef struct TigIdxTable TigIdxTable;
typedef struct TigIdxTableEntry TigIdxTableEntry;

// A key/value pair representing entry in a `TigIdxTable`.
//
// This type is private (visible for testing).
typedef struct TigIdxTableEntry {
    /* 0000 */ void* value;
    /* 0004 */ TigIdxTableEntry* next;
    /* 0008 */ int key;
} TigIdxTableEntry;

static_assert(sizeof(TigIdxTableEntry) == 0xC, "wrong size");

// A collection whose elements are key/value pairs.
typedef struct TigIdxTable {
    /* 0000 */ int buckets_count;
    /* 0004 */ TigIdxTableEntry** buckets;
    /* 0008 */ int value_size;
    /* 000C */ int count;
} TigIdxTable;

static_assert(sizeof(TigIdxTable) == 0x10, "wrong size");

// Signature of a callback used by `tig_idxtable_enumerate`.
//
// The callback should return `true` to continue enumeration, or `false` to
// stop it.
typedef bool(TigIdxTableEnumerateCallback)(void* value, int key, void* context);

void tig_idxtable_init(TigIdxTable* idxtable, int value_size);
void tig_idxtable_exit(TigIdxTable* idxtable);
void tig_idxtable_copy(TigIdxTable* dst, TigIdxTable* src);
int tig_idxtable_write(TigIdxTable* idxtable, TigFile* stream);
bool tig_idxtable_read(TigIdxTable* idxtable, TigFile* stream);
bool tig_idxtable_enumerate(TigIdxTable* idxtable, TigIdxTableEnumerateCallback* callback, void* context);
void tig_idxtable_set(TigIdxTable* idxtable, int key, void* value);
bool tig_idxtable_get(TigIdxTable* idxtable, int key, void* value);
bool tig_idxtable_contains(TigIdxTable* idxtable, int key);
void tig_idxtable_remove(TigIdxTable* idxtable, int key);
int tig_idxtable_count(TigIdxTable* idxtable);

#ifdef __cplusplus
}
#endif

#endif /* TIG_IDXTABLE_H_ */
