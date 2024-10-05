#ifndef ARCANUM_GAME_OBJ_PRIVATE_H_
#define ARCANUM_GAME_OBJ_PRIVATE_H_

#include "game/obj.h"

typedef struct S4E4BD0 {
    uint8_t* field_0;
    uint8_t* field_4;
    int field_8;
    int field_C;
} S4E4BD0;

static_assert(sizeof(S4E4BD0) == 0x10, "wrong size");

void obj_find_init();
void obj_find_exit();
void obj_find_add(int64_t obj);
void obj_find_remove(int64_t obj);
void obj_find_move(int64_t obj);
void sub_4E3F80();
void sub_4E3F90();
void sub_4E4BD0(S4E4BD0* a1);
void sub_4E4C00(const void* data, int size, S4E4BD0* a3);
void sub_4E4C50(void* data, int size, S4E4BD0* a3);
void sub_4E4CD0(int size, bool editor);
void sub_4E4DB0();
Object* sub_4E4E60(int64_t* obj_ptr);
Object* sub_4E4F80(int64_t obj);
void sub_4E62A0(ObjectID* object_id);
void objid_id_perm_by_load_order(ObjectID* object_id, object_id_t obj);
bool objid_is_valid(ObjectID a);
bool objid_is_equal(ObjectID a, ObjectID b);
ObjectID sub_4E6540(int a1);
void objid_id_to_str(char* buffer, ObjectID object_id);
bool objid_id_from_str(ObjectID* object_id, const char* str);
bool objf_solitary_write(int64_t handle, const char* dir, const char* ext);
bool objf_solitary_read(int64_t* handle_ptr, const char* path);
void objf_solitary_delete(int64_t handle, const char* dir, const char* ext);
bool obj_write_raw(void* buffer, size_t size, TigFile* stream);
bool obj_read_raw(void* buffer, size_t size, TigFile* stream);

#endif /* ARCANUM_GAME_OBJ_PRIVATE_H_ */
