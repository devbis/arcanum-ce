#ifndef ARCANUM_GAME_ANIM_H_
#define ARCANUM_GAME_ANIM_H_

#include "game/context.h"
#include "game/anim_private.h"
#include "game/combat.h"
#include "game/timeevent.h"

#define PRIORITY_NONE 0
#define PRIORITY_HIGHEST 6

extern AnimGoalNode* off_5B03D0[];

bool anim_init(GameInitInfo* init_info);
void anim_exit();
void anim_reset();
bool anim_id_to_run_info(AnimID* anim_id, AnimRunInfo** run_info_ptr);
void anim_break_nodes_to_map(const char* map);
void anim_save_nodes_to_map(const char* map);
void anim_load_nodes_from_map(const char* map);
bool sub_421D60(AnimID* a, AnimID* b);
void sub_421DE0(AnimID* anim_id);
void sub_421E20(AnimID* anim_id, char* buffer);
bool anim_save(TigFile* stream);
bool anim_load(GameLoadInfo* load_info);
void sub_4232F0();
bool sub_423300(int64_t obj, AnimID* anim_id);
int sub_4233D0(int64_t obj);
bool sub_423470(int64_t obj);
bool anim_timeevent_process(TimeEvent* timeevent);
void sub_423E60(const char* msg);
void sub_423FC0();
void sub_423FE0(void(*func)());
bool sub_423FF0(int64_t obj);
bool sub_424070(int64_t obj, int priority_level, bool a3, bool a4);
bool sub_424250();
bool anim_goal_interrupt_all_goals_of_priority(int priority_level);
bool anim_goal_interrupt_all_for_tb_combat();
bool sub_4243E0(int64_t obj, tig_art_id_t eye_candy_id, int a3);
bool sub_424560(int64_t obj, tig_art_id_t eye_candy_id, int a3);
int sub_4261E0(int64_t a1, int64_t a2);
int sub_426250(int64_t a1, int64_t a2);
bool sub_426560(int64_t obj, int64_t from, int64_t to, AnimPath* path, unsigned int flags);
bool sub_427000(int64_t obj);
bool anim_fidget_timeevent_process(TimeEvent* timeevent);
void sub_430460();
void sub_430490(int64_t obj, int offset_x, int offset_y);
bool get_always_run(int64_t obj);
void set_always_run(bool value);
void sub_432D90(int64_t obj);
void sub_433020(int64_t obj, int a2, int a3, CombatContext* combat);
void sub_433170(int64_t obj);
void sub_433220(int64_t obj);
bool sub_433440(int64_t obj, int rotation);
bool sub_433580(int64_t obj);
bool sub_433640(int64_t a1, int64_t a2);
bool sub_433C80(int64_t obj, int64_t loc);
bool sub_4341C0(int64_t a1, int64_t a2, int a3);
bool anim_goal_follow_obj(int64_t source_obj, int64_t target_obj);
bool sub_4348E0(int64_t obj, int action_points);
bool sub_434980(int64_t obj, int64_t a2);
bool sub_434AE0(int64_t attacker_obj, int64_t target_obj);
bool sub_434B00(int64_t attacker_obj, int64_t target_obj, int a3);
bool sub_434DE0(int64_t obj);
bool sub_434F80(int64_t obj, int64_t a2, int64_t a3);
bool sub_435080(int64_t obj, int a2);
bool sub_4350F0(int64_t obj, int64_t a2, int64_t a3, int a6, int a7);
bool sub_4352C0(int64_t obj, int64_t a2, int64_t a4, int a5, int a6);
bool sub_435450(int64_t obj, int64_t a2, int64_t a3, unsigned int a4);
bool sub_4355F0(int64_t obj, int64_t a2, int64_t a3, unsigned int flags);
bool sub_435870(int64_t source_obj, int64_t missile_obj, tig_art_id_t missile_art_id, int a4, int a5, int64_t target_obj, int64_t target_loc, int64_t weapon_obj);
bool sub_435A90(int64_t obj);
bool sub_435BD0(int64_t critter_obj);
bool sub_435CE0(int64_t obj);
bool sub_435E60(int64_t obj, int64_t tether_loc, int radius);
bool sub_436040(int64_t obj, int64_t tether_loc, int radius);
void sub_4364D0(int64_t obj);
bool sub_436960(int64_t a1, int64_t a2);
void turn_on_running(AnimID anim_id);
void sub_436C20();
void sub_436C80();
void sub_436CF0();
void sub_436FA0(int64_t obj);
bool sub_4377C0(CombatContext* combat, int64_t obj, int64_t a3, int which);
void sub_437980();
bool sub_437CF0(int a1, int a2, int a3);

#endif /* ARCANUM_GAME_ANIM_H_ */
