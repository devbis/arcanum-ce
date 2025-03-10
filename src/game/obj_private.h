#ifndef ARCANUM_GAME_OBJ_PRIVATE_H_
#define ARCANUM_GAME_OBJ_PRIVATE_H_

#include "game/obj.h"
#include "game/sa.h"
#include "game/script.h"
#include "game/quest.h"

typedef enum SaType {
    SA_TYPE_INVALID = 0,
    SA_TYPE_BEGIN = 1,
    SA_TYPE_END = 2,
    SA_TYPE_INT32 = 3,
    SA_TYPE_INT64 = 4,
    SA_TYPE_INT32_ARRAY = 5,
    SA_TYPE_INT64_ARRAY = 6,
    SA_TYPE_UINT32_ARRAY = 7,
    SA_TYPE_UINT64_ARRAY = 8,
    SA_TYPE_SCRIPT = 9,
    SA_TYPE_QUEST = 10,
    SA_TYPE_STRING = 11,
    SA_TYPE_HANDLE = 12,
    SA_TYPE_HANDLE_ARRAY = 13,
} SaType;

typedef struct S4E4BD0 {
    uint8_t* field_0;
    uint8_t* field_4;
    int field_8;
    int field_C;
} S4E4BD0;

static_assert(sizeof(S4E4BD0) == 0x10, "wrong size");

typedef struct ObjSa {
    /* 0000 */ int type;
    /* 0004 */ void* ptr;
    /* 0008 */ int idx;
    /* 000C */ int field_C;
    union {
        /* 0010 */ int value;
        /* 0010 */ int64_t value64;
        /* 0010 */ char* str;
        /* 0010 */ ObjectID oid;
        Script scr;
        PcQuestState quest;
    } storage;
} ObjSa;

static_assert(sizeof(ObjSa) == 0x28, "wrong size");

typedef struct FindNode FindNode;

void obj_find_init();
void obj_find_exit();
void obj_find_add(int64_t obj);
void obj_find_remove(int64_t obj);
void obj_find_move(int64_t obj);
bool obj_find_walk_first(int64_t sector_id, int64_t* obj_ptr, FindNode** iter_ptr);
bool obj_find_walk_next(int64_t* obj_ptr, FindNode** iter_ptr);
void sub_4E3F80();
void sub_4E3F90();
void sub_4E3FA0(ObjSa* a1);
void sub_4E4000(ObjSa* a1);
void sub_4E4180(ObjSa* a1);
void sub_4E4280(ObjSa* a1, void* value);
bool sub_4E4360(ObjSa* a1, TigFile* stream);
bool sub_4E44F0(ObjSa* a1, TigFile* stream);
void sub_4E4660(ObjSa* a1, uint8_t** data);
bool sub_4E47E0(ObjSa* a1, TigFile* stream);
void sub_4E4990(ObjSa* a1, S4E4BD0* a2);
void sub_4E4B70(ObjSa* a1);
int sub_4E4BA0(ObjSa* a1);
void sub_4E4BD0(S4E4BD0* a1);
void sub_4E4C00(const void* data, int size, S4E4BD0* a3);
void sub_4E4C50(void* buffer, int size, uint8_t** data);
void sub_4E4CD0(int size, bool editor);
void sub_4E4DB0();
Object* sub_4E4E60(int64_t* obj_ptr);
Object* sub_4E4F80(int64_t obj);
void sub_4E4FA0(int64_t obj);
void sub_4E4FB0(int64_t obj);
void sub_4E4FD0(ObjectID a1, int64_t obj);
int64_t objp_perm_lookup(ObjectID oid);
ObjectID sub_4E5280(int64_t obj);
void sub_4E52F0(ObjectID oid);
void sub_4E5300();
bool sub_4E53C0(int64_t* obj_ptr, int* iter_ptr);
bool sub_4E5420(int64_t* obj_ptr, int* iter_ptr);
bool sub_4E5470(int64_t obj);
int sub_4E5530(int64_t obj);
void sub_4E59B0();
void sub_4E5A50();
int sub_4E5AA0();
int sub_4E5B40(int a1);
int sub_4E5BF0(int a1);
void sub_4E5C60(int a1, int a2, bool a3);
int sub_4E5CE0(int a1, int a2);
int sub_4E5D30(int a1, int a2);
bool sub_4E5DB0(int a1, bool(*callback)(int));
bool sub_4E5E20(int a1, TigFile* stream);
bool sub_4E5E80(int* a1, TigFile* stream);
int sub_4E5F10(int a1);
void sub_4E5F30(int a1, void* a2);
void sub_4E5F70(int* a1, uint8_t** data);
int sub_4E5FE0(int a1, int a2);
void sub_4E62A0(ObjectID* oid);
void objid_id_perm_by_load_order(ObjectID* oid, int64_t obj);
bool objid_is_valid(ObjectID a);
bool objid_is_equal(ObjectID a, ObjectID b);
ObjectID sub_4E6540(int a1);
void objid_id_to_str(char* buffer, ObjectID oid);
bool objid_id_from_str(ObjectID* oid, const char* str);
bool objf_solitary_write(int64_t handle, const char* dir, const char* ext);
bool objf_solitary_read(int64_t* handle_ptr, const char* path);
void objf_solitary_delete(int64_t handle, const char* dir, const char* ext);
bool obj_write_raw(void* buffer, size_t size, TigFile* stream);
bool obj_read_raw(void* buffer, size_t size, TigFile* stream);

#endif /* ARCANUM_GAME_OBJ_PRIVATE_H_ */
