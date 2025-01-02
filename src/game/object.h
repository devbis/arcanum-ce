#ifndef ARCANUM_GAME_OBJECT_H_
#define ARCANUM_GAME_OBJECT_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/object_node.h"
#include "game/object.h"
#include "game/location.h"
#include "game/timeevent.h"

#define OBJ_TM_WALL 0x00000001
#define OBJ_TM_PORTAL 0x00000002
#define OBJ_TM_CONTAINER 0x00000004
#define OBJ_TM_SCENERY 0x00000008
#define OBJ_TM_PROJECTILE 0x00000010
#define OBJ_TM_WEAPON 0x00000020
#define OBJ_TM_AMMO 0x00000040
#define OBJ_TM_ITEM_ARMOR 0x00000080
#define OBJ_TM_ITEM_GOLD 0x00000100
#define OBJ_TM_ITEM_FOOD 0x00000200
#define OBJ_TM_ITEM_SCROLL 0x00000400
#define OBJ_TM_ITEM_KEY 0x00000800
#define OBJ_TM_ITEM_KEY_RING 0x00001000
#define OBJ_TM_ITEM_WRITTEN 0x00002000
#define OBJ_TM_ITEM_GENERIC 0x00004000
#define OBJ_TM_PC 0x00008000
#define OBJ_TM_NPC 0x00010000
#define OBJ_TM_TRAP 0x00020000

#define OBJ_TM_ITEM (OBJ_TM_WEAPON \
    | OBJ_TM_AMMO \
    | OBJ_TM_ITEM_ARMOR \
    | OBJ_TM_ITEM_GOLD \
    | OBJ_TM_ITEM_FOOD \
    | OBJ_TM_ITEM_SCROLL \
    | OBJ_TM_ITEM_KEY \
    | OBJ_TM_ITEM_KEY_RING \
    | OBJ_TM_ITEM_WRITTEN \
    | OBJ_TM_ITEM_GENERIC)

#define OBJ_TM_CRITTER (OBJ_TM_PC | OBJ_TM_NPC)

#define OBJ_TM_ALL (OBJ_TM_WALL \
    | OBJ_TM_PORTAL \
    | OBJ_TM_CONTAINER \
    | OBJ_TM_SCENERY \
    | OBJ_TM_PROJECTILE \
    | OBJ_TM_ITEM \
    | OBJ_TM_PC \
    | OBJ_TM_NPC \
    | OBJ_TM_TRAP)

// TODO: Better name.
typedef struct Ryan {
    /* 0000 */ ObjectID objid;
    /* 0018 */ int64_t location;
    /* 0020 */ int map;
} Ryan;

static_assert(sizeof(Ryan) == 0x28, "wrong size");

// TODO: Better name.
typedef struct FollowerInfo {
    /* 0000 */ int64_t obj;
    /* 0008 */ Ryan field_8;
} FollowerInfo;

static_assert(sizeof(FollowerInfo) == 0x30, "wrong size");

// TODO: Better name.
typedef struct Tanya {
    /* 0000 */ FollowerInfo field_0;
    /* 0030 */ FollowerInfo field_30;
    /* 0060 */ int64_t field_60;
    /* 0068 */ FollowerInfo field_68;
    /* 0098 */ int field_98;
    /* 009C */ int field_9C;
    /* 00A0 */ int field_A0;
    /* 00A4 */ int field_A4;
} Tanya;

static_assert(sizeof(Tanya) == 0xA8, "wrong size");

typedef struct ObjectList {
    /* 0000 */ int num_sectors;
    /* 0008 */ int64_t sectors[9];
    /* 0050 */ ObjectNode* head;
} ObjectList;

static_assert(sizeof(ObjectList) == 0x58, "wrong size");

extern int dword_5E2E68;
extern int dword_5E2E6C;
extern bool dword_5E2E94;
extern tig_art_id_t object_hover_underlay_art_id;
extern tig_color_t object_hover_color;
extern tig_art_id_t object_hover_overlay_art_id;
extern Ryan stru_5E2F60;
extern int64_t object_hover_obj;

bool object_init(GameInitInfo* init_info);
void object_resize(GameResizeInfo* resize_info);
void object_reset();
void object_exit();
void object_ping(tig_timestamp_t timestamp);
bool sub_43AA40(ViewOptions* view_options);
void object_close();
bool sub_43AA70(int obj_type);
void sub_43AA80(int obj_type);
void object_render(UnknownContext* render_info);
void sub_43C270(int64_t obj);
int64_t sub_43C570();
void sub_43C690(UnknownContext* render_info);
void object_invalidate_rect(TigRect* rect);
void sub_43CB70();
bool object_create(int64_t proto_obj, int64_t loc, int64_t* obj_ptr);
bool sub_43CBF0(int64_t proto_obj, int64_t loc, ObjectID oid, int64_t* obj_ptr);
void sub_43CCA0(int64_t obj);
void sub_43CF70(int64_t obj);
void sub_43CFF0(object_id_t obj);
void sub_43D0E0(int64_t obj, unsigned int flags);
void sub_43D280(int64_t obj, unsigned int flags);
int object_get_hp_pts(object_id_t obj);
int object_hp_pts_set(int64_t obj, int value);
int object_get_hp_adj(object_id_t obj);
int object_set_hp_adj(object_id_t obj, int value);
int object_hp_damage_get(object_id_t obj);
int object_hp_damage_set(object_id_t obj, int value);
int sub_43D5A0(object_id_t obj);
int sub_43D600(object_id_t obj);
int sub_43D6D0(int64_t obj, int resistance_type, bool a2);
int object_get_ac(object_id_t obj, bool a2);
bool sub_43D940(object_id_t obj);
bool sub_43D990(object_id_t obj);
bool sub_43D9F0(int x, int y, int64_t* obj_ptr, unsigned int flags);
void object_get_rect(int64_t obj, unsigned int flags, TigRect* rect);
void sub_43E770(int64_t obj, int64_t loc, int offset_x, int offset_y);
bool sub_43EAC0(TimeEvent* timeevent);
void object_set_offset(object_id_t obj, int offset_x, int offset_y);
void object_set_current_aid(object_id_t obj, tig_art_id_t aid);
void object_set_light(object_id_t obj, unsigned int flags, tig_art_id_t aid, tig_color_t color);
void sub_43ECF0(int64_t obj, int fld, int index, int value);
bool object_scenery_respawn_timeevent_process(TimeEvent* timeevent);
void object_set_overlay_light(object_id_t obj, int index, unsigned int flags, tig_art_id_t aid, tig_color_t color);
void object_set_blit_scale(object_id_t obj, int value);
void object_add_flags(object_id_t obj, unsigned int flags);
void object_remove_flags(object_id_t obj, unsigned int flags);
void sub_43F030(object_id_t obj);
void object_cycle_palette(object_id_t obj);
void sub_43F1C0(int64_t obj, int64_t triggerer_obj);
bool object_is_destroyed(object_id_t obj);
void sub_43F710(object_id_t obj);
void object_inc_current_aid(object_id_t obj);
void object_dec_current_aid(object_id_t obj);
void sub_43F9F0(object_id_t obj, int fld, int index);
void sub_43FAB0(object_id_t obj, int fld, int index);
void sub_43FB80(object_id_t obj, int index);
void sub_43FBF0(object_id_t obj, int index);
void sub_43FC80(object_id_t obj, int index);
void sub_43FCB0(object_id_t obj, int index);
void object_cycle_rotation(object_id_t obj);
bool sub_43FD70(int64_t a1, int64_t a2, int rotation, unsigned int flags, int* a5);
int sub_43FDC0(int64_t a1, int64_t a2, int rotation, unsigned int flags, int64_t* a5, int* a6, int* a7);
bool sub_440700(int64_t a1, int64_t a2, int rotation, unsigned int flags, int64_t* a5);
void sub_4407C0(int64_t loc, unsigned int flags, ObjectList* objects);
void sub_440B40(LocRect* loc_rect, unsigned int flags, ObjectList* objects);
void sub_440FC0(int64_t obj, unsigned int flags, ObjectList* objects);
int object_list_destroy(ObjectList* objects);
bool sub_441140(ObjectList* objects, int64_t obj);
int object_get_followers(int64_t obj, ObjectList* objects);
void sub_441260(int64_t obj, ObjectList* objects);
int sub_441310(int64_t obj, ObjectList* objects);
void sub_4413E0(int64_t obj, ObjectList* objects);
void sub_4414E0(ObjectList* dst, ObjectList* src);
void sub_4415C0(int64_t obj, int64_t loc);
void sub_4417A0(int64_t item_obj, int64_t parent_obj);
bool sub_441980(int64_t triggerer_obj, int64_t attachee_obj, int64_t extra_obj, int a4, int a5);
int64_t sub_441AE0(object_id_t obj1, object_id_t obj2);
int sub_441B20(object_id_t obj1, object_id_t obj2);
void sub_441B60(int64_t a1, int64_t a2, char* buffer);
void sub_441C70(object_id_t obj, int a2, int gender, int race);
bool object_is_lockable(object_id_t obj);
bool object_is_locked(object_id_t obj);
bool sub_441DD0(object_id_t obj, bool a2);
bool object_lock_timeevent_process(TimeEvent* timeevent);
bool sub_441F10(object_id_t obj, bool a2);
void sub_441FC0(object_id_t obj, int a2);
void object_blit_flags_set(unsigned int flags);
int object_blit_flags_get();
void sub_442050(uint8_t** data_ptr, int* size_ptr, int64_t obj);
bool sub_4420D0(uint8_t* data, int64_t* obj_ptr, int64_t loc);
void sub_442520(int64_t obj);
void sub_443770(object_id_t obj);
bool sub_4437E0(TigRect* rect);
bool sub_4439D0(int64_t* obj_ptr, Ryan* a2, TigFile* stream);
bool sub_443AD0(int64_t* obj_ptr, Ryan* a2, TigFile* stream);
void sub_443EB0(int64_t obj, Ryan* a2);
bool sub_443F80(int64_t* obj_ptr, Ryan* a2);
bool sub_444020(int64_t* obj_ptr, Ryan* a2);
void sub_4440E0(int64_t obj, FollowerInfo* a2);
bool sub_444110(FollowerInfo* a1);
bool sub_444130(FollowerInfo* a1);
int64_t sub_444500(int64_t loc);
void sub_4445A0(int64_t a1, int64_t a2);

#endif /* ARCANUM_GAME_OBJECT_H_ */
