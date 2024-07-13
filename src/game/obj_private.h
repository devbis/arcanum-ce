#ifndef ARCANUM_GAME_OBJ_PRIVATE_H_
#define ARCANUM_GAME_OBJ_PRIVATE_H_

#include "game/obj.h"

void obj_find_init();
void obj_find_exit();
void sub_4E3F80();
void sub_4E3F90();
void sub_4E62A0(ObjectID* object_id);
void objid_id_perm_by_load_order(ObjectID* object_id, object_id_t obj);
bool objid_is_valid(ObjectID a);
bool objid_is_equal(ObjectID a, ObjectID b);
ObjectID sub_4E6540(int a1);
void objid_id_to_str(char* buffer, ObjectID object_id);
bool objid_id_from_str(ObjectID* object_id, const char* str);

#endif /* ARCANUM_GAME_OBJ_PRIVATE_H_ */
