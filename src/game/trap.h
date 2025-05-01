#ifndef ARCANUM_GAME_TRAP_H_
#define ARCANUM_GAME_TRAP_H_

#include "game/context.h"
#include "game/object.h"
#include "game/timeevent.h"
#include "game/script.h"

typedef enum TrapType {
    TRAP_TYPE_INVALID,
    TRAP_TYPE_NONMAGICAL,
    TRAP_TYPE_MAGICAL,
} TrapType;

bool trap_init(GameInitInfo* init_info);
void trap_exit();
bool trap_timeevent_process(TimeEvent* timeevent);
int trap_type(int64_t obj);
bool sub_4BBE40(int64_t pc_obj, int64_t trap_obj);
bool trap_is_spotted(int64_t pc_obj, int64_t trap_obj);
void sub_4BC090(int64_t pc_obj, int64_t trap_obj, int a3);
void sub_4BC220(int64_t trap_obj);
bool sub_4BC2E0(int64_t pc_obj, int64_t item_obj, int64_t target_obj);
bool sub_4BC480(int64_t pc_obj, int64_t item_obj, int64_t target_loc);
void sub_4BC7B0(int64_t pc_obj, int64_t trap_obj, bool* is_success_ptr, bool* is_critical_ptr);
bool trap_is_trap_device(int64_t obj);
int64_t get_trap_at_location(int64_t loc);
void trap_invoke(int64_t triggerer_obj, int64_t attachee_obj);
bool sub_4BCB70(ScriptInvocation* invocation);
bool mp_load(GameLoadInfo* load_info);
bool mp_save(TigFile* stream);
void sub_4BD850(int64_t obj);

#endif /* ARCANUM_GAME_TRAP_H_ */
