#ifndef ARCANUM_GAME_SA_H_
#define ARCANUM_GAME_SA_H_

#include <tig/tig.h>

typedef struct SizeableArray {
    int size;
    int count;
    int field_8;
} SizeableArray;

typedef bool(SizeableArrayEnumerateCallback)(void* entry, int index);

void sa_allocate(SizeableArray** sa_ptr, int size);
void sa_deallocate(SizeableArray** sa_ptr);
void sub_4E74A0(SizeableArray** dst, SizeableArray** src);
void sa_array_copy_to_flat(void* dst, SizeableArray** sa_ptr, int cnt, int size);
bool sa_write(SizeableArray** sa_ptr, TigFile* stream);
bool sa_read(SizeableArray** sa_ptr, TigFile* stream);
bool sa_enumerate(SizeableArray** sa_ptr, SizeableArrayEnumerateCallback* callback);
void sa_set(SizeableArray** sa_ptr, int a2, const void* value);
void sa_get(SizeableArray** sa_ptr, int a2, void* value);
bool sa_has(SizeableArray** sa_ptr, int a2);
void sub_4E7760(SizeableArray** sa_ptr, int a2);
int sa_count(SizeableArray** sa_ptr);
int sub_4E77B0(SizeableArray** sa_ptr);
void sub_4E77E0(SizeableArray** src, SizeableArray* dst);
bool sa_read_no_dealloc(SizeableArray** sa_ptr, TigFile* stream);

#endif /* ARCANUM_GAME_SA_H_ */
