#include "game/anim.h"

#include <stdio.h>

#include "game/ai.h"
#include "game/animfx.h"
#include "game/critter.h"
#include "game/gamelib.h"
#include "game/gsound.h"
#include "game/item.h"
#include "game/light_scheme.h"
#include "game/magictech.h"
#include "game/map.h"
#include "game/mp_utils.h"
#include "game/mt_item.h"
#include "game/multiplayer.h"
#include "game/name.h"
#include "game/object.h"
#include "game/player.h"
#include "game/proto.h"
#include "game/reaction.h"
#include "game/skill.h"
#include "game/stat.h"
#include "game/teleport.h"
#include "game/tile.h"
#include "game/timeevent.h"

typedef enum AgDataType {
    AGDATATYPE_INT,
    AGDATATYPE_OBJ,
    AGDATATYPE_LOC,
    AGDATATYPE_SOUND,
} AgDataType;

static bool sub_421CE0(AnimID* anim_id, AnimRunInfo* run_info);
static void violence_filter_changed();
static bool sub_4221C0(AnimRunInfo* run_info, TigFile* stream);
static bool sub_422350(AnimGoalData* goal_data, TigFile* stream);
static bool sub_422430(AnimRunInfoParam* param, Ryan* a2, int type, TigFile* stream);
static bool anim_load_internal(GameLoadInfo* load_info);
static bool sub_4227F0(AnimRunInfo* run_info, TigFile* stream);
static bool sub_4229A0(AnimGoalData* goal_data, TigFile* stream);
static bool sub_422A50(void* data, Ryan* a2, int type, TigFile* stream);
static int anim_goal_pending_active_goals_count();
static bool sub_436220(int64_t a1, int64_t a2, int64_t a3);
static bool sub_4363E0(int64_t a1, int64_t a2);
static bool sub_436720(int64_t* source_obj_ptr, int64_t* block_obj_ptr);
static void sub_436CB0(AnimID anim_id);
static void sub_436D20(unsigned int flags1, unsigned int flags2);
static void turn_on_flags(AnimID anim_id, unsigned int flags1, unsigned int flags2);
static void sub_436ED0(AnimID anim_id);
static void notify_speed_recalc(AnimID* anim_id);
static bool sub_4372B0(int64_t a1, int64_t a2);
static int num_goal_subslots_in_use(AnimID* anim_id);
static bool is_anim_forever(AnimID* anim_id);
static void sub_437460(AGModifyData* modify_data);
static bool sub_4246C0(AnimRunInfo* run_info);
static bool sub_4246D0(AnimRunInfo* run_info);
static bool sub_4246E0(AnimRunInfo* run_info);
static bool sub_424820(AnimRunInfo* run_info);
static bool sub_4248A0(int a1, int64_t self_obj, int64_t a3, int64_t a4, int64_t loc, int spell, int64_t* obj_ptr, AnimID anim_id, ObjectID oid);
static bool sub_424BC0(AnimRunInfo* run_info);
static bool sub_424D00(AnimRunInfo* run_info);
static bool sub_424D90(AnimRunInfo* run_info);
static bool sub_424E00(AnimRunInfo* run_info);
static bool sub_425130(AnimRunInfo* run_info);
static bool sub_425270(AnimRunInfo* run_info);
static bool sub_425340(AnimRunInfo* run_info);
static bool sub_425430(AnimRunInfo* run_info);
static bool sub_4254C0(AnimRunInfo* run_info);
static bool sub_425590(AnimRunInfo* run_info);
static bool sub_425740(AnimRunInfo* run_info);
static void sub_4257E0(int64_t obj, unsigned int* flags_ptr);
static bool sub_425840(int64_t a1, int64_t a2, int64_t a3, int a4, int64_t a5);
static bool sub_425930(AnimRunInfo* run_info);
static bool sub_425BF0(PathCreateInfo* path_create_info, bool a2);
static bool sub_425D60(AnimRunInfo* run_info);
static bool sub_426040(AnimRunInfo* run_info);
static bool sub_426840(AnimRunInfo* run_info);
static bool sub_4268F0(AnimRunInfo* run_info);
static bool sub_4269D0(AnimRunInfo* run_info);
static bool sub_426A80(AnimRunInfo* run_info);
static bool sub_426E80(AnimRunInfo* run_info);
static bool sub_426F10(AnimRunInfo* run_info);
static bool sub_426F60(AnimRunInfo* run_info);
static bool sub_4270B0(AnimRunInfo* run_info);
static bool sub_427110(AnimRunInfo* run_info, int64_t obj, int64_t loc);
static bool sub_4272E0(AnimRunInfo* run_info);
static bool sub_4273B0(int64_t obj, int64_t loc, int rotation, int a4, int64_t* obj_ptr);
static bool sub_427640(AnimRunInfo* run_info);
static bool sub_427710(AnimRunInfo* run_info);
static bool sub_427720(AnimRunInfo* run_info);
static bool sub_427730(AnimRunInfo* run_info);
static bool sub_427990(AnimRunInfo* run_info);
static bool sub_4280D0(AnimRunInfo* run_info);
static bool sub_4284A0(AnimRunInfo* run_info);
static bool sub_4284F0(AnimRunInfo* run_info);
static bool sub_428550(AnimRunInfo* run_info);
static bool sub_428620(AnimRunInfo* run_info);
static bool sub_428690(AnimRunInfo* run_info);
static bool sub_428750(AnimRunInfo* run_info);
static bool sub_4287E0(AnimRunInfo* run_info);
static bool sub_428890(AnimRunInfo* run_info);
static bool sub_4288A0(AnimRunInfo* run_info);
static bool sub_428930(AnimRunInfo* run_info);
static bool sub_428A10(AnimRunInfo* run_info);
static bool sub_428CD0(AnimRunInfo* run_info);
static bool sub_428E10(AnimRunInfo* run_info);
static bool sub_429040(AnimRunInfo* run_info);
static bool sub_429160(AnimRunInfo* run_info);
static bool sub_429370(AnimRunInfo* run_info);
static bool sub_429380(AnimRunInfo* run_info);
static bool sub_429390(AnimRunInfo* run_info);
static bool sub_4293A0(AnimRunInfo* run_info);
static bool sub_4293B0(AnimRunInfo* run_info);
static bool sub_4293C0(AnimRunInfo* run_info);
static bool sub_4293D0(AnimRunInfo* run_info);
static bool sub_429420(AnimRunInfo* run_info);
static bool sub_429430(AnimRunInfo* run_info);
static bool sub_429440(AnimRunInfo* run_info);
static bool sub_429450(AnimRunInfo* run_info);
static bool sub_4294A0(AnimRunInfo* run_info);
static bool sub_4294F0(int64_t source_obj, int64_t target_obj);
static bool sub_4296D0(AnimRunInfo* run_info);
static bool sub_429760(AnimRunInfo* run_info);
static bool sub_4298D0(AnimRunInfo* run_info);
static bool sub_429960(AnimRunInfo* run_info);
static bool sub_429AD0(AnimRunInfo* run_info);
static bool sub_429B40(AnimRunInfo* run_info);
static bool sub_429B50(AnimRunInfo* run_info);
static bool sub_429BB0(AnimRunInfo* run_info);
static bool sub_429BC0(AnimRunInfo* run_info);
static bool sub_429C40(AnimRunInfo* run_info);
static bool sub_429C80(AnimRunInfo* run_info);
static bool sub_429CD0(AnimRunInfo* run_info);
static bool sub_429E70(AnimRunInfo* run_info);
static bool sub_429ED0(AnimRunInfo* run_info);
static bool sub_429F00(AnimRunInfo* run_info);
static bool sub_42A010(AnimRunInfo* run_info);
static bool sub_42A180(AnimRunInfo* run_info);
static bool sub_42A200(AnimRunInfo* run_info);
static bool sub_42A260(AnimRunInfo* run_info);
static bool sub_42A280(AnimRunInfo* run_info);
static bool sub_42A2A0(AnimRunInfo* run_info);
static bool sub_42A430(AnimRunInfo* run_info);
static bool sub_42A440(AnimRunInfo* run_info);
static bool sub_42A490(AnimRunInfo* run_info);
static bool sub_42A4E0(AnimRunInfo* run_info);
static bool sub_42A630(AnimRunInfo* run_info);
static bool sub_42A720(AnimRunInfo* run_info);
static bool sub_42A930(AnimRunInfo* run_info);
static bool sub_42A9B0(AnimRunInfo* run_info);
static bool sub_42AA70(int64_t source_obj, int64_t target_obj);
static bool sub_42AB90(AnimRunInfo* run_info);
static bool sub_42ACD0(AnimRunInfo* run_info);
static bool sub_42AE10(AnimRunInfo* run_info);
static bool sub_42AF00(AnimRunInfo* run_info);
static bool sub_42AFB0(AnimRunInfo* run_info);
static bool sub_42B090(AnimRunInfo* run_info);
static bool sub_42B250(AnimRunInfo* run_info);
static bool sub_42B440(AnimRunInfo* run_info);
static bool sub_42B640(AnimRunInfo* run_info);
static bool sub_42B6F0(AnimRunInfo* run_info);
static bool sub_42B790(AnimRunInfo* run_info);
static bool sub_42B7F0(AnimRunInfo* run_info);
static bool sub_42B940(AnimRunInfo* run_info);
static bool sub_42B9C0(AnimRunInfo* run_info);
static bool sub_42BC10(AnimRunInfo* run_info);
static bool sub_42BD40(AnimRunInfo* run_info);
static bool sub_42BE50(AnimRunInfo* run_info);
static bool sub_42BE80(AnimRunInfo* run_info);
static bool sub_42BEA0(AnimRunInfo* run_info);
static bool sub_42BEC0(AnimRunInfo* run_info);
static bool sub_42BF40(AnimRunInfo* run_info);
static bool sub_42BFD0(AnimRunInfo* run_info);
static bool sub_42C0F0(AnimRunInfo* run_info);
static bool sub_42C240(AnimRunInfo* run_info);
static bool sub_42C390(AnimRunInfo* run_info);
static bool sub_42C440(AnimRunInfo* run_info);
static bool sub_42C610(AnimRunInfo* run_info);
static bool sub_42C650(AnimRunInfo* run_info);
static bool sub_42C780(AnimRunInfo* run_info);
static bool sub_42C850(AnimRunInfo* run_info);
static bool sub_42CA90(AnimRunInfo* run_info);
static bool sub_42CAA0(AnimRunInfo* run_info);
static bool sub_42CAC0(AnimRunInfo* run_info);
static bool sub_42CB10(AnimRunInfo* run_info);
static bool sub_42CC80(AnimRunInfo* run_info);
static bool sub_42CDF0(AnimRunInfo* run_info);
static bool sub_42CF40(AnimRunInfo* run_info);
static bool sub_42D080(AnimRunInfo* run_info);
static bool sub_42D160(AnimRunInfo* run_info);
static bool sub_42D1C0(AnimRunInfo* run_info);
static bool sub_42D2A0(AnimRunInfo* run_info);
static bool sub_42D300(AnimRunInfo* run_info);
static bool sub_42D440(AnimRunInfo* run_info);
static bool sub_42D570(AnimRunInfo* run_info);
static bool sub_42D6F0(AnimRunInfo* run_info);
static bool sub_42D7D0(AnimRunInfo* run_info);
static bool sub_42D910(AnimRunInfo* run_info);
static bool sub_42DA50(AnimRunInfo* run_info);
static bool sub_42DCF0(AnimRunInfo* run_info);
static bool sub_42DDE0(AnimRunInfo* run_info);
static bool sub_42DED0(AnimRunInfo* run_info);
static bool AGbeginStunAnim(AnimRunInfo* run_info);
static bool sub_42E070(AnimRunInfo* run_info);
static bool sub_42E1B0(AnimRunInfo* run_info);
static bool sub_42E2D0(AnimRunInfo* run_info);
static bool sub_42E460(AnimRunInfo* run_info);
static bool AGbeginKnockDownAnim(AnimRunInfo* run_info);
static bool sub_42E590(AnimRunInfo* run_info);
static bool sub_42E6B0(AnimRunInfo* run_info);
static bool sub_42E720(AnimRunInfo* run_info);
static bool sub_42E8B0(AnimRunInfo* run_info);
static bool sub_42E9B0(AnimRunInfo* run_info);
static bool sub_42F000(AnimRunInfo* run_info);
static bool sub_42F140(AnimRunInfo* run_info);
static bool sub_42F2D0(AnimRunInfo* run_info);
static bool sub_42F390(AnimRunInfo* run_info);
static bool sub_42F5C0(AnimRunInfo* run_info);
static bool sub_42F6A0(AnimRunInfo* run_info);
static bool sub_42FA50(AnimRunInfo* run_info);
static bool sub_42FEA0(AnimRunInfo* run_info);
static bool sub_42FEB0(AnimRunInfo* run_info);
static bool sub_42FEC0(AnimRunInfo* run_info);
static bool sub_42FED0(AnimRunInfo* run_info);
static bool sub_42FF40(AnimRunInfo* run_info);
static bool sub_42FFE0(AnimRunInfo* run_info);
static int sub_4302D0(LocRect* loc_rect, ObjectList* objects);
static bool sub_4303D0(int64_t obj);
static bool sub_4305D0(AnimRunInfo* run_info);
static bool sub_430F20(AnimRunInfo* run_info);
static int sub_430FC0(AnimRunInfo* run_info);
static bool sub_431130(AnimRunInfo* run_info);
static bool sub_431150(AnimRunInfo* run_info);
static bool sub_4311F0(AnimRunInfo* run_info);
static bool sub_431290(AnimRunInfo* run_info);
static bool sub_431320(AnimRunInfo* run_info);
static bool sub_431340(AnimRunInfo* run_info);
static bool sub_4315B0(AnimRunInfo* run_info);
static void sub_4319F0(AnimRunInfo* run_info, int64_t obj);
static bool sub_431A40(AnimRunInfo* run_info);
static bool sub_431B20(AnimRunInfo* run_info);
static bool sub_431C40(AnimRunInfo* run_info);
static bool sub_431E50(AnimRunInfo* run_info);
static bool sub_4321C0(AnimRunInfo* run_info);
static bool sub_4322A0(AnimRunInfo* run_info);
static bool sub_4324D0(AnimRunInfo* run_info);
static bool sub_432700(AnimRunInfo* run_info);
static bool sub_432990(AnimRunInfo* run_info);
static bool sub_432D50(AnimRunInfo* run_info);
static bool sub_433270(AnimRunInfo* run_info);

// 0x5A1908
static AnimID stru_5A1908 = { -1, -1, 0 };

// 0x5A59D0
static AnimGoalNode stru_5A59D0 = {
    6,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42CC80, 0, -1, -1, 0x30000000, -2, 0x10000000, -2 },
        { sub_426F10, 0, -1, -1, 4, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 5, 0, 0x40000036, 0 },
        { sub_42CB10, 0, 8, -1, 6, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A5BD0
static AnimGoalNode stru_5A5BD0 = {
    4,
    1,
    0,
    1,
    1,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 4, 0 },
        { sub_42DCF0, 0, -1, -1, 0x10000000, 0x320, 3, 0 },
        { sub_42DDE0, 0, -1, -1, 0x10000000, -2, 0x10000000, -2 },
        { sub_42DA50, 0, 8, -1, 0x90000000, 0, 0x10000000, -2 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42DED0, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A5DD0
static AnimGoalNode stru_5A5DD0 = {
    5,
    1,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42CF40, 0, -1, -1, 5, 0, 0x10000000, -2 },
        { sub_42CDF0, 0, 8, -1, 4, 0, 0x10000000, -2 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { sub_4298D0, 0, -1, -1, 0x90000000, 0, 3, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A5FD0
static AnimGoalNode stru_5A5FD0 = {
    7,
    2,
    0,
    0,
    0,
    7,
    4,
    -1,
    {
        { sub_424D90, 0, 5, -1, 2, 0, 0x30000000, 0 },
        { sub_426F10, 0, -1, -1, 3, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 4, 0, 0x40000036, 0 },
        { sub_425740, 0, -1, -1, 5, 0, 0x40000005, 0 },
        { sub_426040, 0, 5, -1, 7, 0, 6, 0 },
        { sub_42C610, -1, -1, -1, 0x90000000, 0, 0x40000035, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A61D0
static AnimGoalNode stru_5A61D0 = {
    8,
    2,
    0,
    0,
    0,
    7,
    3,
    0x37,
    {
        { sub_42B940, -1, -1, -1, 2, 0, 2, 0 },
        { sub_424D90, 0, 5, -1, 3, 0, 0x30000000, 0 },
        { sub_426F10, 0, -1, -1, 4, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 5, 0, 0x40000036, 0 },
        { sub_425740, 0, -1, -1, 6, 0, 0x40000005, 0 },
        { sub_426040, 0, 5, -1, 8, 0, 7, 0 },
        { sub_42C610, -1, -1, -1, 0x90000000, 0, 0x40000035, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A63D0
static AnimGoalNode stru_5A63D0 = {
    0xC,
    2,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 4, 0 },
        { sub_4270B0, 0, -1, -1, 3, 0, 0xA, 0xFFFFFFFC },
        { sub_4305D0, 0, -1, -1, 0x30000000, -2, 0x10000000, -2 },
        { sub_425740, 0, -1, -1, 0xC, 0, 5, 0 },
        { sub_425430, 0, -1, -1, 6, 0, 0x30000000, 0 },
        { sub_427640, 0, -1, -1, 7, 0, 0xB, 0 },
        { sub_4272E0, 0, -1, -1, 9, 0, 8, 0 },
        { sub_4284F0, 3, -1, -1, 9, 0, 0x5200000B, 0x32 },
        { sub_42E9B0, 0, -1, -1, 0xA, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { sub_4288A0, 0, 3, -1, 0x90000000, 0, 0x5000000E, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { sub_430F20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A65D0
static AnimGoalNode stru_5A65D0 = {
    3,
    2,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_424D90, 0, 5, -1, 0x90000000, 0, 0x90000000, 0 },
        { sub_427710, 2, -1, -1, 0x90000000, 0, 0x90000000, 0x3E8 },
        { sub_427720, 2, -1, -1, 0x90000000, 0, 0x90000000, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A67D0
static AnimGoalNode stru_5A67D0 = {
    7,
    2,
    0,
    0,
    0,
    4,
    3,
    0x37,
    {
        { sub_424D90, 0, 5, -1, 2, 0, 0x30000000, 0 },
        { sub_426F10, 0, -1, -1, 3, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 4, 0, 0x40000036, 0 },
        { sub_425740, 0, -1, -1, 5, 0, 0x40000005, 0 },
        { sub_427730, 0, 5, -1, 7, 0, 6, 0 },
        { sub_42C610, -1, -1, -1, 0x90000000, 0, 0x40000035, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A69D0
static AnimGoalNode stru_5A69D0 = {
    6,
    2,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_425130, 0, 1, -1, 2, 0, 0x30000000, 0 },
        { sub_426F10, 0, -1, -1, 3, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 4, 0, 0x40000036, 0 },
        { sub_425740, 0, -1, -1, 5, 0, 0x4000003D, 0 },
        { sub_427990, 0, 1, -1, 6, 0, 0x4000003D, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A6BD0
static AnimGoalNode stru_5A6BD0 = {
    4,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_424D90, 0, 5, -1, 2, 0, 0x30000000, 0 },
        { sub_425740, 0, -1, -1, 3, 0, 0x4000000A, 0 },
        { sub_426840, 0, 5, -1, 4, 0, 0x4000000A, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A6DD0
static AnimGoalNode stru_5A6DD0 = {
    3,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42F390, 0, -1, -1, 0x30000000, -2, 0x10000000, 0xFFFFFFFD },
        { sub_42F2D0, 0, -1, -1, 0x90000000, 0, 0x10000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A6FD0
static AnimGoalNode stru_5A6FD0 = {
    5,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_4284A0, 3, -1, -1, 0x30000000, 0, 2, 0 },
        { sub_4284F0, 3, -1, -1, 0x30000000, 0, 3, 0 },
        { sub_428750, 3, 0, -1, 0x7100000C, 0, 4, 0 },
        { sub_4287E0, 0, 3, -1, 5, 0, 0x4000000D, 0 },
        { sub_428890, 0, 3, -1, 0x90000000, 0, 0x4000000F, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A71D0
static AnimGoalNode stru_5A71D0 = {
    8,
    3,
    1,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_4284A0, 3, -1, -1, 0x30000000, 0, 2, 0 },
        { sub_4284F0, 3, -1, -1, 0x90000000, 0, 3, 0 },
        { sub_428690, 0, 3, -1, 0x90000000, 0, 4, 0 },
        { sub_428750, 3, 0, -1, 6, 0, 5, 0 },
        { sub_4287E0, 0, 3, -1, 0x90000000, 0, 0x7000000D, 0 },
        { sub_428A10, 0, 3, -1, 0x90000000, 0, 7, 0 },
        { sub_428550, 3, 0, -1, 0x90000000, 0, 8, 0 },
        { sub_4246E0, 3, 0, 0x46, 0x90000000, 0, 0x31000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A73D0
static AnimGoalNode stru_5A73D0 = {
    0xB,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42D910, 0, -1, -1, 6, -2, 7, 0 },
        { sub_42BD40, 0, -1, 0xD, 0x90000000, 0, 4, 0 },
        { sub_42C780, 0, -1, -1, 0x90000000, 0, 0xB, 0 },
        { sub_42D7D0, 0, 8, -1, 0xA, 0, 0x10000000, 0 },
        { sub_42BD40, 0, -1, 1, 0x90000000, 0, 9, 0 },
        { sub_42CAA0, 0, 8, -1, 0x10000000, -2, 8, 0 },
        { sub_42F000, 0, 8, -1, 0x10000000, -2, 0x10000000, -2 },
        { sub_42F140, 0, 8, -1, 0x90000000, 0, 0x31000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { sub_4284F0, 3, -1, -1, 5, 0, 0x5200000B, 0x32 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A75D0
static AnimGoalNode stru_5A75D0 = {
    0xA,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_429370, 0, 1, -1, 2, 0, 0x90000000, 0 },
        { sub_429380, 1, -1, -1, 0x90000000, 0, 3, 0 },
        { sub_424D90, 0, 0x20, -1, 4, 0, 0x40000010, 0 },
        { sub_429390, 0, 1, -1, 0x90000000, 0, 5, 0 },
        { sub_4293A0, 1, -1, -1, 6, 0, 0xA, 0 },
        { sub_4293B0, 0x21, -1, -1, 0x40000014, 0, 7, 0 },
        { sub_4293C0, 0x21, -1, -1, 8, 0, 0x40000011, 0 },
        { sub_429420, 0, 0x21, -1, 9, 0, 0x40000014, 0 },
        { sub_429430, 0, 0x21, -1, 0x90000000, 0, 0x40000011, 0 },
        { sub_42BEA0, -1, -1, 0, 0x90000000, 0, 0x40000008, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A77D0
static AnimGoalNode stru_5A77D0 = {
    2,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42A9B0, 0, 1, -1, 2, 0, 2, 0 },
        { sub_42BD40, 0, -1, 0, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A79D0
static AnimGoalNode stru_5A79D0 = {
    1,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42BE80, -1, -1, 5, 0x90000000, 0, 0x40000021, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A7BD0
static AnimGoalNode stru_5A7BD0 = {
    8,
    3,
    0,
    0,
    0,
    0x13,
    -1,
    -1,
    {
        { sub_429450, 1, -1, -1, 0x30000000, 0, 2, 0 },
        { sub_4294A0, 0, 1, -1, 0x90000000, 0, 3, 0 },
        { sub_426F10, 0, -1, -1, 4, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 5, 0, 0x40000036, 0 },
        { sub_429960, 0, 1, -1, 6, 0, 0x74000013, 0 },
        { sub_42BEA0, -1, -1, 1, 0x90000000, 0, 7, 0 },
        { sub_425130, 0, 1, -1, 0x4000004F, 0, 8, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A7DD0
static AnimGoalNode stru_5A7DD0 = {
    0xD,
    3,
    0,
    0,
    0,
    0x12,
    0x14,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_432990, 0, -1, -1, 9, 0, 7, 0 },
        { sub_42C0F0, 0, 1, -1, 0xA, 0, 4, 0 },
        { sub_429960, 0, 1, -1, 0xC, 0, 5, 0 },
        { sub_42B9C0, 0, 1, -1, 0xA, 0, 6, 0 },
        { sub_432700, 0, 8, -1, 0xA, 0, 0x10000000, 0 },
        { sub_42CAA0, 0, 8, -1, 0x10000000, -2, 0xD, 0 },
        { sub_42A630, 0, 1, -1, 0xA, 0, 0x10000000, -2 },
        { sub_42A930, 0, -1, -1, 0xA, 0, 0xA, 0 },
        { sub_42BD40, 0, -1, 0, 0xB, 0, 0xB, 0 },
        { sub_432D50, 0, -1, -1, 0xC, 0, 0xC, 0 },
        { sub_429760, 0, 1, -1, 0x30000000, 0, 0x74000012, 5 },
        { sub_429960, 0, 1, -1, 0xC, 0, 8, 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A7FD0
static AnimGoalNode stru_5A7FD0 = {
    7,
    3,
    0,
    0,
    0,
    0x13,
    -1,
    -1,
    {
        { sub_429450, 1, -1, -1, 0x30000000, 0, 2, 0 },
        { sub_426F10, 0, -1, -1, 3, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 4, 0, 0x40000036, 0 },
        { sub_4294A0, 0, 1, -1, 0x90000000, 0, 5, 0 },
        { sub_429B40, 0, 1, -1, 0x40000016, 0, 6, 0 },
        { sub_429960, 0, 1, -1, 7, 0, 0x40000013, 0 },
        { sub_42BEA0, -1, -1, 1, 0x90000000, 0, 0x40000008, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A81D0
static AnimGoalNode stru_5A81D0 = {
    6,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42ACD0, 0, 1, -1, 0x30000000, 0, 2, 0 },
        { sub_426F10, 0, -1, -1, 3, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 4, 0, 0x40000036, 0 },
        { sub_42AE10, 0, 1, -1, 6, 0, 5, 0 },
        { sub_42AF00, 0, 1, -1, 0x90000000, 0, 0x30000000, 0 },
        { sub_42BEA0, -1, -1, 1, 0x90000000, 0, 0x40000008, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A83D0
static AnimGoalNode stru_5A83D0 = {
    4,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42B940, -1, -1, -1, 2, 0, 2, 0 },
        { sub_42BF40, 0, -1, -1, 0x90000000, 0, 3, 0 },
        { sub_425130, 0, 1, -1, 0x40000008, 0, 4, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x10000000, 0x64 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A85D0
static AnimGoalNode stru_5A85D0 = {
    5,
    2,
    0,
    0,
    0,
    0x38,
    8,
    -1,
    {
        { sub_42BF40, 0, -1, -1, 0x90000000, 0, 2, 0 },
        { sub_42BFD0, 0, 1, -1, 3, 0, 3, 0 },
        { sub_425130, 0, 1, -1, 4, 0, 5, 0 },
        { sub_425340, 0, 1, -1, 0x70000038, 0, 0x70000008, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x10000000, 0x64 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A87D0
static AnimGoalNode stru_5A87D0 = {
    4,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42B940, -1, -1, -1, 2, 0, 2, 0 },
        { sub_42BEA0, -1, -1, 9, 0x90000000, 0, 3, 0 },
        { sub_425130, 0, 1, -1, 4, 0, 0x40000034, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x30000000, -2 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A89D0
static AnimGoalNode stru_5A89D0 = {
    0xA,
    3,
    0,
    0,
    0,
    0x1B,
    -1,
    -1,
    {
        { sub_429440, 1, -1, -1, 0xA, 0, 2, 0 },
        { sub_426F10, 0, -1, -1, 3, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 4, 0, 0x40000036, 0 },
        { sub_429B50, 0, 1, -1, 8, 0, 5, 0 },
        { sub_429BB0, 0, 1, -1, 0x40000016, 0, 6, 0 },
        { sub_429BC0, 0, 1, -1, 0xA, 0, 9, 0 },
        { sub_42BEA0, -1, -1, 8, 0xA, 0, 0x40000008, 0 },
        { sub_431A40, 0, -1, -1, 0xA, 0, 0xA, 0 },
        { sub_42C850, 0, 1, -1, 0xA, 0, 0x7000001B, 0 },
        { sub_429CD0, 0, 0xB, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A8BD0
static AnimGoalNode stru_5A8BD0 = {
    0xC,
    4,
    0,
    0,
    0,
    0x1A,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 0xB, 0 },
        { sub_431340, 0, -1, -1, 8, -2, 4, 0 },
        { sub_4315B0, 0, -1, -1, 0xC, 0, 0x10000000, 0 },
        { sub_42CAA0, 0, 8, -1, 0x10000000, -2, 5, 0 },
        { sub_429C40, 0xB, -1, -1, 9, 0, 6, 0 },
        { sub_424820, 0, 0x1F, 5, 8, 0, 7, 0 },
        { sub_4246E0, 3, 0, 0x1C, 0x90000000, -2, 0x10000000, -2 },
        { sub_431A40, 0, -1, -1, 0x90000000, 0, 0x30000000, 0 },
        { sub_429C80, 0xB, -1, -1, 8, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { sub_429F00, 0xB, -1, -1, 0xC, 0, 3, 0 },
        { sub_429C80, 0xB, -1, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { sub_429CD0, 0, 0xB, 1, 0, 0, 0, 0 },
    },
};

// 0x5A8DD0
static AnimGoalNode stru_5A8DD0 = {
    9,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_424D90, 0, 0x22, -1, 2, 0, 4, 0 },
        { sub_425740, 0, -1, -1, 3, 0, 8, 0 },
        { sub_426840, 0, 0x22, -1, 0x90000000, 0, 8, 0 },
        { sub_42A180, 0, 1, -1, 9, 0, 5, 0 },
        { sub_429C80, 0xB, -1, -1, 0x90000000, 0, 6, 0 },
        { sub_424D00, 0, -1, -1, 0x90000000, 0, 0x30000000, 0 },
        { sub_42A200, 0, 0xB, -1, 6, 0, 6, 0 },
        { sub_429ED0, 0xB, -1, -1, 0x90000000, 0, 0x4000004E, 0 },
        { sub_42BEC0, 1, -1, -1, 0x30000000, 0, 1, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_429E70, 0, 0xB, 1, 0, 0, 0, 0 },
    },
};

// 0x5A8FD0
static AnimGoalNode stru_5A8FD0 = {
    4,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42FEC0, 0, -1, -1, 0x30000000, -2, 0x10000000, -2 },
        { sub_42FEB0, 0, 0xA, -1, 4, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A91D0
static AnimGoalNode stru_5A91D0 = {
    6,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42CC80, 0, -1, -1, 5, -2, 0x10000000, -2 },
        { sub_42BD40, 0, -1, 0x17, 0x90000000, 0, 4, 0 },
        { sub_42CB10, 0, 8, -1, 6, 0, 0x10000000, 0 },
        { sub_42BD40, 0, -1, 0, 0x90000000, 0, 0x90000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A93D0
static AnimGoalNode stru_5A93D0 = {
    9,
    5,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42D6F0, 0, -1, -1, 7, 0, 0x10000000, -2 },
        { sub_426F10, 0, -1, -1, 4, 0, 7, 0 },
        { sub_426E80, 0, -1, -1, 5, 0, 5, 0 },
        { sub_42BC10, 0, -1, -1, 7, 0, 6, 0 },
        { sub_42D570, 0, 8, -1, 7, 0, 0x10000000, 0 },
        { sub_42FED0, 0, -1, -1, 8, 0, 9, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { sub_42FF40, 0, -1, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42FFE0, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A95D0
static AnimGoalNode stru_5A95D0 = {
    1,
    5,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_424D00, 0, -1, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A97D0
static AnimGoalNode stru_5A97D0 = {
    7,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42BEA0, -1, -1, 1, 0x90000000, 0, 2, 0 },
        { sub_425130, 0, 1, 1, 3, 0, 5, 0 },
        { sub_425740, 0, -1, -1, 4, 0, 0x40000005, 0 },
        { sub_427990, 0, 1, -1, 6, 0, 0x40000005, 0 },
        { sub_42A490, 0, 0xC, 1, 7, 0, 0x70000043, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { sub_42A440, 0, 0xC, 1, 0x70000022, 0, 0x70000056, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A99D0
static AnimGoalNode stru_5A99D0 = {
    9,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42CC80, 0, -1, -1, 8, -2, 6, 0 },
        { sub_42C0F0, 0, 1, -1, 0x90000000, 0, 4, 0 },
        { sub_42BD40, 0, -1, 0xC, 0x90000000, 0, 5, 0 },
        { sub_42CB10, 0, 8, -1, 9, 0, 0x10000000, 0 },
        { sub_42CAA0, 0, 8, -1, 0x10000000, -2, 7, 0 },
        { sub_42A2A0, 0, 1, -1, 0x90000000, 0, 0x10000000, -2 },
        { sub_42BD40, 0, -1, 0, 0x90000000, 0, 0x30000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A9BD0
static AnimGoalNode stru_5A9BD0 = {
    6,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42CC80, 0, -1, -1, 5, -2, 0x10000000, -2 },
        { sub_42BD40, 0, -1, 5, 0x90000000, 0, 4, 0 },
        { sub_42CB10, 0, 8, -1, 6, 0, 0x10000000, 0 },
        { sub_42BD40, 0, -1, 5, 0x90000000, 0, 0x90000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A9DD0
static AnimGoalNode stru_5A9DD0 = {
    7,
    5,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42F6A0, 0, 1, -1, 0x30000000, 0, 0x10000000, 0xFFFFFFFD },
        { sub_424D90, 0, 5, -1, 4, 0, 0x30000000, 0 },
        { sub_424BC0, 0, 1, -1, 0x30000000, 0, 5, 0 },
        { sub_425740, 0, -1, -1, 6, 0, 7, 0 },
        { sub_4269D0, 0, 5, -1, 0x30000000, 0, 7, 0 },
        { sub_42F5C0, 0, -1, -1, 0x30000000, 0, 0x10000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42CAC0, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A9FD0
static AnimGoalNode stru_5A9FD0 = {
    9,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42CC80, 0, -1, -1, 8, -2, 6, 0 },
        { sub_42BD40, 0, -1, 0xE, 8, 0, 4, 0 },
        { sub_42C390, 0, 5, -1, 8, 0, 5, 0 },
        { sub_42CB10, 0, 8, -1, 8, 0, 0x10000000, 0 },
        { sub_42CAA0, 0, 8, -1, 0x10000000, -2, 7, 0 },
        { sub_42AB90, 0, 3, 5, 8, 0, 0x10000000, -2 },
        { sub_42BD40, 0, -1, 0, 9, 0, 9, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B440, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AA1D0
static AnimGoalNode stru_5AA1D0 = {
    6,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_428930, 0, 1, -1, 0x90000000, 0, 2, 0 },
        { sub_425130, 0, 1, -1, 0x40000008, 0, 3, 0 },
        { sub_4284A0, 1, -1, -1, 4, 0, 5, 0 },
        { sub_428A10, 0, 1, -1, 0x90000000, 0, 0x90000000, 0 },
        { sub_42BE50, 1, -1, -1, 0x90000000, 0, 6, 0 },
        { sub_4284F0, 1, -1, -1, 0x70000031, 0, 0x7000000B, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AA3D0
static AnimGoalNode stru_5AA3D0 = {
    3,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42BEA0, -1, -1, 1, 0x90000000, 0, 2, 0 },
        { sub_425130, 0, 1, -1, 0x40000008, 0, 3, 0 },
        { sub_428CD0, 0, 1, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AA5D0
static AnimGoalNode stru_5AA5D0 = {
    3,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42BEA0, -1, -1, 1, 0x90000000, 0, 2, 0 },
        { sub_425130, 0, 1, -1, 0x40000008, 0, 3, 0 },
        { sub_428E10, 0, 1, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AA7D0
static AnimGoalNode stru_5AA7D0 = {
    3,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42BEA0, -1, -1, 1, 0x90000000, 0, 2, 0 },
        { sub_425270, 0, 5, -1, 0x40000007, 0, 3, 0 },
        { sub_429040, 0, 5, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AA9D0
static AnimGoalNode stru_5AA9D0 = {
    3,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42BEA0, -1, -1, 1, 0x90000000, 0, 2, 0 },
        { sub_425270, 0, 5, -1, 0x40000007, 0, 3, 0 },
        { sub_429160, 0, 5, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AABD0
static AnimGoalNode stru_5AABD0 = {
    4,
    5,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_424D90, 0, 5, -1, 2, 0, 0x30000000, 0 },
        { sub_425740, 0, -1, -1, 3, 0, 0x40000040, 0 },
        { sub_4268F0, 0, 5, -1, 4, 0, 0x40000040, 0 },
        { sub_42FEA0, 0, -1, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AADD0
static AnimGoalNode stru_5AADD0 = {
    4,
    5,
    1,
    1,
    1,
    -1,
    -1,
    -1,
    {
        { sub_431130, 0, -1, -1, 2, 0, 3, 0 },
        { sub_431150, 0, -1, -1, 4, 0x64, 0x10000000, 0x64 },
        { sub_4311F0, 0, -1, -1, 0x90000000, 0, 0x10000000, 0 },
        { sub_431290, 0, -1, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AAFD0
static AnimGoalNode stru_5AAFD0 = {
    4,
    5,
    1,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 3, 0 },
        { sub_431340, 0, -1, -1, 4, 0, 0x10000000, -2 },
        { sub_4315B0, 0, -1, -1, 0x90000000, 0, 0x10000000, -2 },
        { sub_431A40, 0, -1, -1, 3, 0, 0x30000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_431B20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AB1D0
static AnimGoalNode stru_5AB1D0 = {
    4,
    5,
    1,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 3, 0 },
        { sub_431C40, 0, -1, -1, 4, 0, 0x10000000, -2 },
        { sub_431E50, 0, -1, -1, 0x90000000, 0, 0x10000000, -2 },
        { sub_4321C0, 0, -1, -1, 3, 0, 0x30000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_431B20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AB3D0
static AnimGoalNode stru_5AB3D0 = {
    6,
    5,
    1,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 3, 0 },
        { sub_431340, 0, -1, -1, 4, 0, 5, 0 },
        { sub_4315B0, 0, -1, -1, 0x90000000, 0, 0x10000000, -2 },
        { sub_431A40, 0, -1, -1, 3, 0, 0x30000000, 0 },
        { sub_42CAA0, 0, 8, -1, 0x10000000, -2, 6, 0 },
        { sub_42A260, 0xB, -1, -1, 4, 0, 0x10000000, -2 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_431B20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AB5D0
static AnimGoalNode stru_5AB5D0 = {
    6,
    5,
    1,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 3, 0 },
        { sub_431C40, 0, -1, -1, 4, 0, 5, 0 },
        { sub_431E50, 0, -1, -1, 0x90000000, 0, 0x10000000, -2 },
        { sub_4321C0, 0, -1, -1, 3, 0, 0x30000000, 0 },
        { sub_42CAA0, 0, 8, -1, 0x10000000, -2, 6, 0 },
        { sub_42A260, 0xB, -1, -1, 4, 0, 0x10000000, -2 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_431B20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AB7D0
static AnimGoalNode stru_5AB7D0 = {
    2,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_4284A0, 3, -1, -1, 0x30000000, 0, 2, 0 },
        { sub_4284F0, 3, -1, -1, 0x70000032, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AB9D0
static AnimGoalNode stru_5AB9D0 = {
    4,
    3,
    1,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_4284A0, 3, -1, -1, 0x30000000, 0, 2, 0 },
        { sub_4284F0, 3, -1, -1, 3, 0, 0x90000000, 0 },
        { sub_428620, 3, 0, -1, 0x90000000, 0, 4, 0 },
        { sub_4246E0, 3, 0, 0x47, 0x90000000, 0, 0x30000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5ABBD0
static AnimGoalNode stru_5ABBD0 = {
    4,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42E8B0, 0, -1, -1, 0x30000000, -2, 0x10000000, -2 },
        { sub_42E720, 0, 8, -1, 4, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5ABDD0
static AnimGoalNode stru_5ABDD0 = {
    7,
    2,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_426F10, 0, -1, -1, 2, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 3, 0, 0x40000036, 0 },
        { sub_425130, 0, 1, -1, 0x30000000, 0, 5, 0 },
        { sub_425740, 0, -1, -1, 5, 0, 0x40000005, 0 },
        { sub_426A80, 0, 1, -1, 6, 0, 0x40000005, 0 },
        { sub_42C240, 0, 1, -1, 0x90000000, 0, 7, 0 },
        { sub_4293D0, 0, -1, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5ABFD0
static AnimGoalNode stru_5ABFD0 = {
    1,
    2,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42C440, 0, 0xE, -1, 0x30000000, 0, 0x10000000, 0x1E },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42C650, 0, 0xE, -1, 0, 0, 0, 0 },
    },
};

// 0x5AC1D0
static AnimGoalNode stru_5AC1D0 = {
    5,
    4,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42E8B0, 0, -1, -1, 0x30000000, -2, 0x10000000, -2 },
        { sub_42E6B0, 0, -1, -1, 5, 0, 4, 0 },
        { sub_42E720, 0, 8, -1, 5, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42AFB0, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AC3D0
static AnimGoalNode stru_5AC3D0 = {
    8,
    2,
    0,
    0,
    0,
    7,
    4,
    -1,
    {
        { sub_42B940, -1, -1, -1, 2, 0, 2, 0 },
        { sub_424D90, 0, 5, -1, 3, 0, 0x30000000, 0 },
        { sub_426F10, 0, -1, -1, 4, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 5, 0, 0x40000036, 0 },
        { sub_425740, 0, -1, -1, 6, 0, 0x40000005, 0 },
        { sub_427730, 0, 5, -1, 8, 0, 7, 0 },
        { sub_42C610, -1, -1, -1, 0x90000000, 0, 0x40000035, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AC5D0
static AnimGoalNode stru_5AC5D0 = {
    7,
    2,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42B940, -1, -1, -1, 2, 0, 2, 0 },
        { sub_425130, 0, 1, -1, 3, 0, 0x30000000, 0 },
        { sub_426F10, 0, -1, -1, 4, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 5, 0, 0x40000036, 0 },
        { sub_425740, 0, -1, -1, 6, 0, 0x40000005, 0 },
        { sub_427990, 0, 1, -1, 7, 0, 0x40000005, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AC7D0
static AnimGoalNode stru_5AC7D0 = {
    7,
    1,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42E070, 0, -1, -1, 7, 0, 0x10000000, -2 },
        { sub_426F10, 0, -1, -1, 4, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 5, 0, 0x40000036, 0 },
        { AGbeginStunAnim, 0, 8, -1, 6, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { sub_426F60, 0, -1, -1, 5, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B250, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AC9D0
static AnimGoalNode stru_5AC9D0 = {
    5,
    5,
    1,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 3, 0 },
        { sub_431340, 0, -1, -1, 4, 0, 0x10000000, -2 },
        { sub_4315B0, 0, -1, -1, 0x90000000, 0, 0x10000000, 0 },
        { sub_42A280, 0xB, -1, -1, 5, 0, 5, 0 },
        { sub_431A40, 0, -1, -1, 0x30000000, 0, 0x30000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_431B20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5ACBD0
static AnimGoalNode stru_5ACBD0 = {
    5,
    5,
    1,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 3, 0 },
        { sub_431C40, 0, -1, -1, 4, 0, 0x10000000, -2 },
        { sub_431E50, 0, -1, -1, 0x90000000, 0, 0x10000000, 0 },
        { sub_42A280, 0xB, -1, -1, 5, 0, 5, 0 },
        { sub_4321C0, 0, -1, -1, 0x30000000, 0, 0x30000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_431B20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5ACDD0
static AnimGoalNode stru_5ACDD0 = {
    6,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42E2D0, 0, -1, -1, 0x70000033, 0, 0x10000000, -2 },
        { sub_426F10, 0, -1, -1, 4, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 5, 0, 0x40000036, 0 },
        { sub_42E1B0, 0, -1, -1, 6, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5ACFD0
static AnimGoalNode stru_5ACFD0 = {
    0xC,
    2,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 4, 0 },
        { sub_4270B0, 0, -1, -1, 3, 0, 0xA, 0xFFFFFFFC },
        { sub_4305D0, 0, -1, -1, 0x30000000, -2, 0x10000000, -2 },
        { sub_425740, 0, -1, -1, 0xC, 0, 5, 0 },
        { sub_4254C0, 0, 1, -1, 6, 0, 0x30000000, 0 },
        { sub_427640, 0, -1, -1, 7, 0, 0xB, 0 },
        { sub_4272E0, 0, -1, -1, 9, 0, 8, 0 },
        { sub_4284F0, 3, -1, -1, 9, 0, 0x5200000B, 0x32 },
        { sub_42E9B0, 0, -1, -1, 0xA, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x30000000, 0, 0x30000000, 0 },
        { sub_4288A0, 0, 3, -1, 0x90000000, 0, 0x5000000E, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { sub_430F20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AD1D0
static AnimGoalNode stru_5AD1D0 = {
    4,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42CC80, 0, -1, -1, 4, 0, 0x10000000, -2 },
        { AGbeginKnockDownAnim, 0, -1, -1, 0x90000000, 0, 0x10000000, 0 },
        { sub_42E460, 0, -1, -1, 0x30000000, 0, 0x7000003F, 0xC8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AD3D0
static AnimGoalNode stru_5AD3D0 = {
    4,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42CC80, 0, -1, -1, 0x30000000, 0, 0x10000000, -2 },
        { sub_42E590, 0, -1, -1, 4, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AD5D0
static AnimGoalNode stru_5AD5D0 = {
    4,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42FA50, 0, -1, -1, 0x90000000, 0, 0x10000000, 0xFFFFFFFD },
        { sub_42F2D0, 0, -1, -1, 0x90000000, 0, 0x10000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AD7D0
static AnimGoalNode stru_5AD7D0 = {
    1,
    3,
    0,
    0,
    0,
    7,
    -1,
    -1,
    {
        { sub_425930, 0, -1, -1, 0x10000000, 0x12C, 0x70000007, 0x12C },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AD9D0
static AnimGoalNode stru_5AD9D0 = {
    1,
    3,
    0,
    0,
    0,
    7,
    -1,
    -1,
    {
        { sub_425D60, 0, -1, -1, 0x10000000, 0x12C, 0x70000007, 0x12C },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5ADBD0
static AnimGoalNode stru_5ADBD0 = {
    7,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42D440, 0, -1, -1, 6, -2, 0x10000000, -2 },
        { sub_42C0F0, 0, 1, -1, 0x90000000, 0, 4, 0 },
        { sub_42BD40, 0, -1, 0xC, 0x90000000, 0, 5, 0 },
        { sub_42D300, 0, 8, -1, 7, 0, 0x10000000, -2 },
        { sub_42A4E0, 0, 1, -1, 0x90000000, 0, 0x10000000, -2 },
        { sub_42BD40, 0, -1, 0, 0x90000000, 0, 0x30000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5ADDD0
static AnimGoalNode stru_5ADDD0 = {
    8,
    2,
    0,
    0,
    0,
    7,
    4,
    3,
    {
        { sub_424E00, 0, 5, -1, 0x30000000, 0, 2, 0xFFFFFFFD },
        { sub_424D90, 0, 5, -1, 3, 0, 0x30000000, 0 },
        { sub_426F10, 0, -1, -1, 4, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 5, 0, 0x40000036, 0 },
        { sub_425740, 0, -1, -1, 6, 0, 0x40000005, 0 },
        { sub_426040, 0, 5, -1, 8, 0, 7, 0 },
        { sub_42C610, -1, -1, -1, 0x90000000, 0, 0x40000035, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5ADFD0
static AnimGoalNode stru_5ADFD0 = {
    5,
    2,
    1,
    0,
    0,
    0x38,
    8,
    -1,
    {
        { sub_42BF40, 0, -1, -1, 0x90000000, 0, 2, 0 },
        { sub_42BFD0, 0, 1, -1, 3, 0, 0x40000034, 0 },
        { sub_425130, 0, 1, -1, 4, 0, 5, 0 },
        { sub_425340, 0, 1, -1, 0x70000038, 0, 0x70000008, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x10000000, 0x64 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AE1D0
static AnimGoalNode stru_5AE1D0 = {
    4,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42D160, 0, -1, -1, 4, 0, 0x10000000, -2 },
        { sub_42D080, 0, 8, -1, 0x30000000, 0, 0x10000000, -2 },
        { sub_42B790, 0, -1, -1, 0x30000000, 0, 0x70000048, 0x5DC },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B640, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AE3D0
static AnimGoalNode stru_5AE3D0 = {
    3,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42D2A0, 0, -1, -1, 0x30000000, -2, 0x10000000, -2 },
        { sub_42D1C0, 0, 8, -1, 0x30000000, 0, 0x10000000, -2 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B6F0, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AE5D0
static AnimGoalNode stru_5AE5D0 = {
    1,
    3,
    0,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42B7F0, 0, -1, -1, 0x10000000, 0x5DC, 0x70000047, 0x5DC },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AE7D0
static AnimGoalNode stru_5AE7D0 = {
    9,
    3,
    1,
    0,
    0,
    0x1B,
    -1,
    -1,
    {
        { sub_429440, 1, -1, -1, 0x30000000, 0, 2, 0 },
        { sub_426F10, 0, -1, -1, 3, 0, 3, 0 },
        { sub_426E80, 0, -1, -1, 4, 0, 4, 0 },
        { sub_429B50, 0, 1, -1, 8, 0, 5, 0 },
        { sub_429BB0, 0, 1, -1, 0x40000016, 0, 6, 0 },
        { sub_429BC0, 0, 1, -1, 0x90000000, 0, 9, 0 },
        { sub_42BEA0, -1, -1, 8, 0x90000000, 0, 0x40000008, 0 },
        { sub_431A40, 0, -1, -1, 0x90000000, 0, 0x90000000, 0 },
        { sub_4246C0, -1, -1, -1, 0x90000000, 0, 0x7000001B, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AE9D0
static AnimGoalNode stru_5AE9D0 = {
    0xC,
    4,
    1,
    0,
    0,
    0x1A,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 0xB, 0 },
        { sub_431340, 0, -1, -1, 8, -2, 4, 0 },
        { sub_4315B0, 0, -1, -1, 0xC, 0, 0x10000000, 0 },
        { sub_42CAA0, 0, 8, -1, 0x10000000, -2, 5, 0 },
        { sub_429C40, 0xB, -1, -1, 9, 0, 6, 0 },
        { sub_424820, 0, 0x1F, 5, 8, 0, 7, 0 },
        { sub_4246E0, 3, 0, 0x1C, 0x90000000, -2, 0x10000000, -2 },
        { sub_431A40, 0, -1, -1, 0x90000000, 0, 0x30000000, 0 },
        { sub_429C80, 0xB, -1, -1, 8, 0, 0x10000000, 0 },
        { sub_4246C0, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { sub_429F00, 0xB, -1, -1, 0xC, 0, 3, 0 },
        { sub_429C80, 0xB, -1, -1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { sub_429CD0, 0, 0xB, 1, 0, 0, 0, 0 },
    },
};

// 0x5AEBD0
static AnimGoalNode stru_5AEBD0 = {
    5,
    5,
    1,
    1,
    1,
    -1,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 3, 0 },
        { sub_4322A0, 0, 4, -1, 4, 0, 0x10000000, -2 },
        { sub_4315B0, 0, -1, -1, 0x90000000, 0, 5, 0 },
        { sub_431A40, 0, -1, -1, 3, 0, 0x30000000, 0 },
        { sub_42A720, 0, 4, -1, 0x90000000, 0, 0x10000000, -2 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_431B20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AEDD0
static AnimGoalNode stru_5AEDD0 = {
    5,
    5,
    1,
    1,
    1,
    -1,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 3, 0 },
        { sub_4324D0, 0, 4, -1, 4, 0, 0x10000000, -2 },
        { sub_431E50, 0, -1, -1, 0x90000000, 0, 5, 0 },
        { sub_4321C0, 0, -1, -1, 3, 0, 0x30000000, 0 },
        { sub_42A720, 0, 4, -1, 0x90000000, 0, 0x10000000, -2 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_431B20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AEFD0
static AnimGoalNode stru_5AEFD0 = {
    5,
    1,
    0,
    1,
    1,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 4, 0 },
        { sub_42DCF0, 0, -1, -1, 0x10000000, 0x320, 3, 0 },
        { sub_42DDE0, 0, -1, -1, 5, 0, 0x10000000, -2 },
        { sub_42DA50, 0, 8, -1, 0x90000000, 0, 5, 0 },
        { sub_42A720, 0, 4, -1, 0x90000000, 0, 0x10000000, -2 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42DED0, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AF1D0
static AnimGoalNode stru_5AF1D0 = {
    3,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42F390, 0, -1, -1, 0x30000000, -2, 0x10000000, 0xFFFFFFFD },
        { sub_42F2D0, 0, -1, -1, 0x90000000, 0, 0x10000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AF3D0
static AnimGoalNode stru_5AF3D0 = {
    8,
    2,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_425130, 0, 1, -1, 2, 0, 0x30000000, 0 },
        { sub_426F10, 0, -1, -1, 3, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 4, 0, 0x40000036, 0 },
        { sub_425740, 0, -1, -1, 5, 0, 7, 0 },
        { sub_4280D0, 0, 1, -1, 6, 0, 7, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { sub_4296D0, 0, -1, -1, 8, 0, 8, 0 },
        { sub_429AD0, 0, -1, -1, 0x4000003D, 0, 0x40000050, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AF5D0
static AnimGoalNode stru_5AF5D0 = {
    0xC,
    2,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 4, 0 },
        { sub_4270B0, 0, -1, -1, 3, 0, 0xA, 0xFFFFFFFC },
        { sub_4305D0, 0, -1, -1, 0x30000000, -2, 0x10000000, -2 },
        { sub_425740, 0, -1, -1, 0xC, 0, 5, 0 },
        { sub_425590, 0, 1, -1, 6, 0, 0x38000000, 0 },
        { sub_427640, 0, -1, -1, 7, 0, 0xB, 0 },
        { sub_4272E0, 0, -1, -1, 9, 0, 8, 0 },
        { sub_4284F0, 3, -1, -1, 9, 0, 0x5200000B, 0x32 },
        { sub_42E9B0, 0, -1, -1, 0xA, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { sub_4288A0, 0, 3, -1, 0x90000000, 0, 0x5000000E, 0 },
        { sub_4246D0, -1, -1, -1, 0x90000000, 0, 0x50000006, 0x3E8 },
        { 0 },
        { 0 },
        { sub_430F20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AF7D0
static AnimGoalNode stru_5AF7D0 = {
    6,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_428930, 0, 1, -1, 0x90000000, 0, 2, 0 },
        { sub_425130, 0, 1, -1, 0x40000008, 0, 3, 0 },
        { sub_4284A0, 1, -1, -1, 4, 0, 5, 0 },
        { sub_428A10, 0, 1, -1, 0x90000000, 0, 0x90000000, 0 },
        { sub_42BE50, 1, -1, -1, 0x90000000, 0, 6, 0 },
        { sub_4284F0, 1, -1, -1, 0x70000031, 0, 0x7000000B, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AF9D0
static AnimGoalNode stru_5AF9D0 = {
    0xA,
    3,
    0,
    0,
    0,
    0x1B,
    -1,
    -1,
    {
        { sub_429440, 1, -1, -1, 0xA, 0, 2, 0 },
        { sub_426F10, 0, -1, -1, 3, 0, 0x4000003F, 0 },
        { sub_426E80, 0, -1, -1, 4, 0, 0x40000036, 0 },
        { sub_429B50, 0, 1, -1, 8, 0, 5, 0 },
        { sub_429BB0, 0, 1, -1, 0x40000016, 0, 6, 0 },
        { sub_429BC0, 0, 1, -1, 0xA, 0, 9, 0 },
        { sub_42BEA0, -1, -1, 8, 0xA, 0, 0x40000008, 0 },
        { sub_431A40, 0, -1, -1, 0xA, 0, 0xA, 0 },
        { sub_42C850, 0, 1, -1, 0xA, 0, 0x70000053, 0 },
        { sub_429CD0, 0, 0xB, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AFBD0
static AnimGoalNode stru_5AFBD0 = {
    0xE,
    4,
    0,
    0,
    0,
    0x1A,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 0xB, 0 },
        { sub_42CC80, 0, -1, -1, 8, -2, 4, 0 },
        { sub_4246C0, 0, -1, -1, 0xD, 0, 0x10000000, 0 },
        { sub_42CAA0, 0, 8, -1, 0x10000000, -2, 5, 0 },
        { sub_429C40, 0xB, -1, -1, 9, 0, 6, 0 },
        { sub_424820, 0, 0x1F, 5, 8, 0, 7, 0 },
        { sub_4246E0, 3, 0, 0x1C, 0x90000000, -2, 0x10000000, -2 },
        { sub_4246C0, 0, -1, -1, 0x90000000, 0, 0x30000000, 0 },
        { sub_429C80, 0xB, -1, -1, 8, 0, 0x10000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { sub_4246C0, 0xB, -1, -1, 0xD, 0, 0xC, 0 },
        { sub_42A010, 0, 9, -1, 0xD, 0, 3, 0 },
        { sub_429C80, 0xB, -1, -1, 0x90000000, 0, 0x90000000, 0 },
        { sub_4246C0, 0, -1, -1, 8, -2, 4, 0 },
        { sub_429CD0, 0, 0xB, 1, 0, 0, 0, 0 },
    },
};

// 0x5AFDD0
static AnimGoalNode stru_5AFDD0 = {
    5,
    5,
    1,
    1,
    0,
    -1,
    -1,
    -1,
    {
        { sub_431320, 0, -1, -1, 2, 0, 3, 0 },
        { sub_431340, 0, -1, -1, 5, 0, 0x10000000, -2 },
        { sub_429F00, 0xB, -1, -1, 0x90000000, 0, 4, -2 },
        { sub_4315B0, 0, -1, -1, 0x90000000, 0, 0x10000000, -2 },
        { sub_431A40, 0, -1, -1, 3, 0, 0x30000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_431B20, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5AFFD0
static AnimGoalNode stru_5AFFD0 = {
    4,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42B940, -1, -1, -1, 2, 0, 2, 0 },
        { sub_42BEA0, -1, -1, 9, 0x90000000, 0, 3, 0 },
        { sub_425130, 0, 1, -1, 4, 0, 0x40000034, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x30000000, -2 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { NULL, -1, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5B01D0
static AnimGoalNode stru_5B01D0 = {
    9,
    3,
    0,
    0,
    0,
    -1,
    -1,
    -1,
    {
        { sub_42CA90, 0, -1, -1, 2, 0, 3, 0 },
        { sub_42CC80, 0, -1, -1, 8, -2, 0x10000000, -2 },
        { sub_42C0F0, 0, 1, -1, 0x90000000, 0, 4, 0 },
        { sub_42A2A0, 0, 1, -1, 0x90000000, 0, 5, 0 },
        { sub_42A430, 0, 1, -1, 6, 0, 8, 0 },
        { sub_42BD40, 0, -1, 0xC, 0x90000000, 0, 7, 0 },
        { sub_42CB10, 0, 8, -1, 9, 0, 0x10000000, 0 },
        { sub_42BD40, 0, -1, 0, 0x90000000, 0, 0x30000000, 0 },
        { sub_433270, 0, -1, 1, 0x90000000, 0, 0x90000000, 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { sub_42B090, 0, -1, 1, 0, 0, 0, 0 },
    },
};

// 0x5A597C
static int dword_5A597C[AGDATA_COUNT] = {
    AGDATATYPE_OBJ,
    AGDATATYPE_OBJ,
    AGDATATYPE_OBJ,
    AGDATATYPE_OBJ,
    AGDATATYPE_OBJ,
    AGDATATYPE_LOC,
    AGDATATYPE_LOC,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_INT,
    AGDATATYPE_SOUND,
};

// 0x5B03D0
AnimGoalNode* off_5B03D0[] = {
    &stru_5A59D0, // AG_ANIMATE
    &stru_5A5BD0, // AG_ANIMATE_LOOP
    &stru_5A5DD0, // AG_ANIM_FIDGET
    &stru_5A5FD0, // AG_MOVE_TO_TILE
    &stru_5A61D0, // AG_RUN_TO_TILE
    &stru_5A63D0, // AG_ATTEMPT_MOVE
    &stru_5A65D0, // AG_MOVE_TO_PAUSE
    &stru_5A67D0, // AG_MOVE_NEAR_TILE
    &stru_5A69D0, // AG_MOVE_NEAR_OBJ
    &stru_5A6BD0, // AG_MOVE_STRAIGHT
    &stru_5A6DD0, // AG_ATTEMPT_MOVE_STRAIGHT
    &stru_5A6FD0, // AG_OPEN_DOOR
    &stru_5A71D0, // AG_ATTEMPT_OPEN_DOOR
    NULL, // AG_UNLOCK_DOOR
    &stru_5A73D0, // AG_JUMP_WINDOW
    &stru_5A75D0, // AG_PICKUP_ITEM
    &stru_5A77D0, // AG_ATTEMPT_PICKUP
    &stru_5A79D0, // AG_PICKPOCKET
    &stru_5A7BD0, // AG_ATTACK
    &stru_5A7DD0, // AG_ATTEMPT_ATTACK
    &stru_5A7FD0, // AG_KILL
    &stru_5A81D0, // AG_TALK
    NULL, // AG_PICK_WEAPON
    &stru_5A83D0, // AG_CHASE
    &stru_5A85D0, // AG_FOLLOW
    &stru_5A87D0, // AG_FLEE
    &stru_5A89D0, // AG_THROW_SPELL
    &stru_5A8BD0, // AG_ATTEMPT_SPELL
    &stru_5A8DD0, // AG_SHOOT_SPELL
    &stru_5A8FD0, // AG_HIT_BY_SPELL
    &stru_5A91D0, // AG_HIT_BY_WEAPON
    &stru_5A93D0, // AG_DYING
    &stru_5A95D0, // AG_DESTROY_OBJ
    &stru_5A97D0, // AG_USE_SKILL_ON
    &stru_5A99D0, // AG_ATTEMPT_USE_SKILL_ON
    &stru_5A9BD0, // AG_SKILL_CONCEAL
    &stru_5A9DD0, // AG_PROJECTILE
    &stru_5A9FD0, // AG_THROW_ITEM
    &stru_5AA1D0, // AG_USE_OBJECT
    &stru_5AA3D0, // AG_USE_ITEM_ON_OBJECT
    &stru_5AA5D0, // AG_USE_ITEM_ON_OBJECT_WITH_SKILL
    &stru_5AA7D0, // AG_USE_ITEM_ON_TILE
    &stru_5AA9D0, // AG_USE_ITEM_ON_TILE_WITH_SKILL
    &stru_5AABD0, // AG_KNOCKBACK
    &stru_5AADD0, // AG_FLOATING
    &stru_5AAFD0, // AG_EYE_CANDY
    &stru_5AB1D0, // AG_EYE_CANDY_REVERSE
    &stru_5AB3D0, // AG_EYE_CANDY_CALLBACK
    &stru_5AB5D0, // AG_EYE_CANDY_REVERSE_CALLBACK
    &stru_5AB7D0, // AG_CLOSE_DOOR
    &stru_5AB9D0, // AG_ATTEMPT_CLOSE_DOOR
    &stru_5ABBD0, // AG_ANIMATE_REVERSE
    &stru_5ABDD0, // AG_MOVE_AWAY_FROM_OBJ
    &stru_5ABFD0, // AG_ROTATE
    &stru_5AC1D0, // AG_UNCONCEAL
    &stru_5AC3D0, // AG_RUN_NEAR_TILE
    &stru_5AC5D0, // AG_RUN_NEAR_OBJ
    &stru_5AC7D0, // AG_ANIMATE_STUNNED
    &stru_5AC9D0, // AG_EYE_CANDY_END_CALLBACK
    &stru_5ACBD0, // AG_EYE_CANDY_REVERSE_END_CALLBACK
    &stru_5ACDD0, // AG_ANIMATE_KNEEL_MAGIC_HANDS
    &stru_5ACFD0, // AG_ATTEMPT_MOVE_NEAR
    &stru_5AD1D0, // AG_KNOCK_DOWN
    &stru_5AD3D0, // AG_ANIM_GET_UP
    &stru_5AD5D0, // AG_ATTEMPT_MOVE_STRAIGHT_KNOCKBACK
    &stru_5AD7D0, // AG_WANDER
    &stru_5AD9D0, // AG_WANDER_SEEK_DARKNESS
    &stru_5ADBD0, // AG_USE_PICKLOCK_SKILL_ON
    &stru_5ADDD0, // AG_PLEASE_MOVE
    &stru_5ADFD0, // AG_ATTEMPT_SPREAD_OUT
    &stru_5AE1D0, // AG_ANIMATE_DOOR_OPEN
    &stru_5AE3D0, // AG_ANIMATE_DOOR_CLOSED
    &stru_5AE5D0, // AG_PEND_CLOSING_DOOR
    &stru_5AE7D0, // AG_THROW_SPELL_FRIENDLY
    &stru_5AE9D0, // AG_ATTEMPT_SPELL_FRIENDLY
    &stru_5AEBD0, // AG_EYE_CANDY_FIRE_DMG
    &stru_5AEDD0, // AG_EYE_CANDY_REVERSE_FIRE_DMG
    &stru_5AEFD0, // AG_ANIMATE_LOOP_FIRE_DMG
    &stru_5AF1D0, // AG_ATTEMPT_MOVE_STRAIGHT_SPELL
    &stru_5AF3D0, // AG_MOVE_NEAR_OBJ_COMBAT
    &stru_5AF5D0, // AG_ATTEMPT_MOVE_NEAR_COMBAT
    &stru_5AF7D0, // AG_USE_CONTAINER
    &stru_5AF9D0, // AG_THROW_SPELL_W_CAST_ANIM
    &stru_5AFBD0, // AG_ATTEMPT_SPELL_W_CAST_ANIM
    &stru_5AFDD0, // AG_THROW_SPELL_W_CAST_ANIM_2NDARY
    &stru_5AFFD0, // AG_BACK_OFF_FROM
    &stru_5B01D0, // AG_ATTEMPT_USE_PICKPOCKET_SKILL_ON
};

// 0x5DE608
static int dword_5DE608;

// 0x5DE610
static AnimFxList stru_5DE610;

// 0x5DE640
static int dword_5DE640;

// 0x5DE648
static int dword_5DE648;

// 0x5DE650
static int dword_5DE650;

// 0x5DE658
static int dword_5DE658;

// 0x5DE660
static int dword_5DE660;

// 0x5DE668
static int dword_5DE668;

// 0x5DE670
static AnimFxList stru_5DE670;

// 0x5DE69C
static int dword_5DE69C;

// 0x5DE6A0
static int dword_5DE6A0;

// 0x5DE6A4
static int violence_filter;

// 0x5DE6B0
static int dword_5DE6B0;

// 0x5DE6B8
static int dword_5DE6B8;

// 0x5DE6C0
static int dword_5DE6C0;

// 0x5DE6C4
static int dword_5DE6C4;

// 0x5DE6D0
static bool dword_5DE6D0;

// 0x5DE6D4
static bool anim_editor;

// 0x5DE6D8
static int64_t qword_5DE6D8;

// 0x5DE6E0
static bool dword_5DE6E0;

// 0x5DE6E4
static int dword_5DE6E4;

// 0x421B00
bool anim_init(GameInitInfo* init_info)
{
    anim_editor = init_info->editor;

    if (!anim_editor) {
        if (!animfx_list_init(&stru_5DE670)) {
            return false;
        }

        stru_5DE670.path = "Rules\\AnimEyeCandy.mes";
        stru_5DE670.field_18 = 11;
        if (!animfx_list_load(&stru_5DE670)) {
            return false;
        }

        if (!animfx_list_init(&stru_5DE610)) {
            return false;
        }

        stru_5DE610.path = "Rules\\WeaponEyeCandy.mes";
        stru_5DE610.field_18 = 750;
        stru_5DE610.field_8 = 1;
        stru_5DE610.field_C = 5;
        stru_5DE610.field_10 = 10;
        if (!animfx_list_load(&stru_5DE610)) {
            return false;
        }
    }

    settings_add(&settings, "violence filter", "0", violence_filter_changed);
    violence_filter = settings_get_value(&settings, "violence filter");
    settings_add(&settings, "always run", "0", NULL);

    return true;
}

// 0x421BF0
void anim_exit()
{
    if (!anim_editor) {
        animfx_list_exit(&stru_5DE610);
        animfx_list_exit(&stru_5DE670);
    }
}

// 0x421C20
void anim_reset()
{
    dword_5E3500 = 0;
}

// 0x421C30
bool anim_id_to_run_info(AnimID* anim_id, AnimRunInfo** run_info_ptr)
{
    int index;

    ASSERT(anim_id != NULL); // pAnimID != NULL
    ASSERT(run_info_ptr != NULL); // ppRunInfo != NULL

    if (anim_id->slot_num != -1) {
        for (index = 0; index < 216; index++) {
            if (sub_421CE0(anim_id, &(anim_run_info[index]))) {
                *run_info_ptr = &(anim_run_info[index]);
                return true;
            }
        }
    }

    *run_info_ptr = NULL;
    return false;
}

// 0x421CE0
bool sub_421CE0(AnimID* anim_id, AnimRunInfo* run_info)
{
    ASSERT(anim_id != NULL); // pAnimID != NULL
    ASSERT(run_info != NULL); // pRunInfo != NULL

    if (run_info->id.field_4 != anim_id->field_4) {
        return false;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        if (run_info->id.slot_num != anim_id->slot_num) {
            return false;
        }
    }

    return true;
}

// 0x421D60
bool sub_421D60(AnimID* a, AnimID* b)
{
    ASSERT(a != NULL); // pAnimID1 != NULL
    ASSERT(b != NULL); // pAnimID2 != NULL

    if (a->field_4 != b->field_4) {
        return false;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        if (a->slot_num != b->slot_num) {
            return false;
        }
    }

    return true;
}

// 0x421DE0
void sub_421DE0(AnimID* anim_id)
{
    ASSERT(anim_id != NULL); // pAnimID != NULL

    anim_id->slot_num = -1;
    anim_id->field_4 = -1;
    anim_id->field_8 = 0;
}

// 0x421E20
void sub_421E20(AnimID* anim_id, char* buffer)
{
    ASSERT(anim_id != NULL); // pAnimID != NULL
    ASSERT(buffer != NULL); // str != NULL

    _snprintf(buffer, 36,
        "[%d:%dr%d]",
        anim_id->slot_num,
        anim_id->field_4,
        anim_id->field_8);
}

// 0x421EA0
bool anim_save(TigFile* stream)
{
    // TODO: Incomplete.
}

// 0x4221A0
void violence_filter_changed()
{
    violence_filter = settings_get_value(&settings, "violence filter");
}

// 0x4221C0
bool sub_4221C0(AnimRunInfo* run_info, TigFile* stream)
{
    int idx;

    if (stream == NULL) {
        return false;
    }

    if (tig_file_fwrite(&(run_info->id.slot_num), 4, 1, stream) != 1) return false;
    if (tig_file_fwrite(&(run_info->id.field_4), 4, 1, stream) != 1) return false;
    if (tig_file_fwrite(&(run_info->id.field_8), 4, 1, stream) != 1) return false;
    if (tig_file_fwrite(&(run_info->field_C), 4, 1, stream) != 1) return false;
    if (tig_file_fwrite(&(run_info->field_10), 4, 1, stream) != 1) return false;
    if (tig_file_fwrite(&(run_info->field_14), 4, 1, stream) != 1) return false;
    if (!sub_4439D0(&(run_info->goals[0].params[0].obj), &(run_info->goals[0].field_B0[0]), stream) ) return false;
    if (tig_file_fwrite(&(run_info->field_28), 8, 1, stream) != 1) return false;
    if (tig_file_fwrite(&(run_info->current_goal), 8, 1, stream) != 1) return false;

    for (idx = 0; idx <= run_info->current_goal; idx++) {
        if (!sub_422350(&(run_info->goals[idx]), stream)) {
            return false;
        }
    }

    if (tig_file_fwrite(&(run_info->path), sizeof(run_info->path), 1, stream) != 1) return false;

    // FIXME: Strange case with 0xCF8 / 0xCFC: writing 8 bytes, while 0xCFC is
    // definitely separate field (something FPS related).
    if (tig_file_fwrite(&(run_info->field_CF8), 8, 1, stream) != 1) return false;

    if (tig_file_fwrite(&(run_info->field_18), sizeof(run_info->field_18), 1, stream) != 1) return false;

    return true;
}

// 0x422350
bool sub_422350(AnimGoalData* goal_data, TigFile* stream)
{
    int idx;
    AnimRunInfoParam param;

    if (stream == NULL) {
        return false;
    }

    if (tig_file_fwrite(&(goal_data->type), sizeof(goal_data->type), 1, stream) != 1) {
        return false;
    }

    for (idx = 0; idx < 5; idx++) {
        if (!sub_422430(&(goal_data->params[idx]), &(goal_data->field_B0[idx]), dword_5A597C[idx], stream)) {
            return false;
        }
    }

    for (; idx < 20; idx++) {
        if (!sub_422430(&(goal_data->params[idx]), NULL, dword_5A597C[idx], stream)) {
            return false;
        }
    }

    // Special case - sound handle is volatile, it's not intended to be
    // serialized.
    param.data = -1;
    if (!sub_422430(&param, NULL, dword_5A597C[AGDATA_SOUND_HANDLE], stream)) {
        return false;
    }

    return true;
}

// 0x422430
bool sub_422430(AnimRunInfoParam* param, Ryan* a2, int type, TigFile* stream)
{
    if (stream == NULL) {
        return false;
    }

    switch (type) {
    case AGDATATYPE_INT:
        if (tig_file_fwrite(&(param->data), sizeof(param->data), 1, stream) != 1) return false;
        return true;
    case AGDATATYPE_OBJ:
        // __FILE__: C:\Troika\Code\Game\GameLibX\Anim.c
        // __LINE__: 750
        // pObjSafeData != NULL
        ASSERT(a2 != NULL);

        if (!sub_4439D0(&(param->obj), a2, stream)) return false;
        return true;
    case AGDATATYPE_LOC:
        if (tig_file_fwrite(&(param->loc), sizeof(param->loc), 1, stream) != 1) return false;
        return true;
    case AGDATATYPE_SOUND:
        // Special case - reading 8 bytes even though sound handle is only 4
        // bytes.
        if (tig_file_fwrite(param, 8, 1, stream) != 1) return false;
        return true;
    }

    return false;
}

// 0x4224E0
bool anim_load(GameLoadInfo* load_info)
{
    bool loaded;

    in_anim_load = true;
    loaded = anim_load_internal(load_info);
    in_anim_load = false;

    sub_423E60("Anim Load");

    return loaded;
}

// 0x422520
bool anim_load_internal(GameLoadInfo* load_info)
{
    int cnt;
    int idx;
    int extent_size;

    if (load_info == NULL) {
        return false;
    }

    if (tig_file_fread(&dword_6876E4, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&animNumActiveGoals, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE6D0, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_739E44, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_739E40, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE650, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE658, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE608, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE640, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE648, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE660, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE668, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE6B8, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE6B0, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE6A0, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE69C, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE6C4, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5DE6C0, 4, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&cnt, 4, 1, load_info->stream) != 1 ) return false;

    idx = 0;
    while (idx < cnt) {
        if (tig_file_fread(&extent_size, sizeof(extent_size), 1, load_info->stream) != 1) {
            return false;
        }

        if (extent_size > 0) {
            while (idx < extent_size) {
                if (!sub_4227F0(&(anim_run_info[idx]), load_info->stream)) {
                    return false;
                }
                idx++;
            }
        } else if (extent_size < 0) {
            idx += -extent_size;
        }
    }

    return true;
}

// 0x4227F0
bool sub_4227F0(AnimRunInfo* run_info, TigFile* stream)
{
    int index;

    if (stream == NULL) {
        return false;
    }

    if (tig_file_fread(&(run_info->id.slot_num), 4, 1, stream) != 1) return false;
    if (tig_file_fread(&(run_info->id.field_4), 4, 1, stream) != 1) return false;
    if (tig_file_fread(&(run_info->id.field_8), 4, 1, stream) != 1) return false;
    if (tig_file_fread(&(run_info->field_C), 4, 1, stream) != 1) return false;
    if (tig_file_fread(&(run_info->field_10), 4, 1, stream) != 1) return false;
    if (tig_file_fread(&(run_info->field_14), 4, 1, stream) != 1) return false;
    if (!sub_443AD0(&(run_info->field_20), 0, stream) ) return false;
    if (tig_file_fread(&(run_info->field_28), 8, 1, stream) != 1) return false;
    if (tig_file_fread(&(run_info->current_goal), 8, 1, stream) != 1) return false;

    for (index = 0; index <= run_info->current_goal; index++) {
        if (!sub_4229A0(&(run_info->goals[index]), stream)) {
            return false;
        }
    }

    if (tig_file_fread(&(run_info->path), sizeof(run_info->path), 1, stream) != 1) return false;

    // FIXME: Strange case with 0xCF8 / 0xCFC: writing 8 bytes, while 0xCFC is
    // definitely separate field (something FPS related).
    if (tig_file_fread(&(run_info->field_CF8), 8, 1, stream) != 1) return false;

    if (tig_file_fread(&(run_info->field_18), sizeof(run_info->field_18), 1, stream) != 1) return false;

    if (run_info->current_goal >= 0) {
        run_info->cur_stack_data = &(run_info->goals[run_info->current_goal]);
    }

    return true;
}

// 0x4229A0
bool sub_4229A0(AnimGoalData* goal_data, TigFile* stream)
{
    int idx;

    if (stream == NULL) {
        return false;
    }

    if (tig_file_fread(&(goal_data->type), sizeof(goal_data->type), 1, stream) != 1) {
        return false;
    }

    for (idx = 0; idx < 5; idx++) {
        if (!sub_422A50(&(goal_data->params[idx]), &(goal_data->field_B0[idx]), dword_5A597C[idx], stream)) {
            return false;
        }
    }

    for (; idx < AGDATA_COUNT; idx++) {
        if (!sub_422A50(&(goal_data->params[idx]), NULL, dword_5A597C[idx], stream)) {
            return false;
        }
    }

    return true;
}

// 0x422A50
bool sub_422A50(AnimRunInfoParam* param, Ryan* a2, int type, TigFile* stream)
{
    if (stream == NULL) {
        return false;
    }

    switch (type) {
    case AGDATATYPE_INT:
        if (tig_file_fread(&(param->data), sizeof(param->data), 1, stream) != 1) return false;
        return true;
    case AGDATATYPE_OBJ:
        // __FILE__: C:\Troika\Code\Game\GameLibX\Anim.c
        // __LINE__: 949
        // pObjSafeData != NULL
        ASSERT(a2 != NULL);

        if (!sub_443AD0(&(param->obj), a2, stream)) return false;
        return true;
    case AGDATATYPE_LOC:
        if (tig_file_fread(&(param->loc), sizeof(param->loc), 1, stream) != 1) return false;
        return true;
    case AGDATATYPE_SOUND:
        // Special case - reading 8 bytes even though sound handle is only 4
        // bytes.
        if (tig_file_fread(param, 8, 1, stream) != 1) return false;
        param->data = -1;
        return true;
    }

    return false;
}

// 0x422B10
void anim_break_nodes_to_map(const char* map)
{
    char path[TIG_MAX_PATH];
    TigFile* stream;
    bool exists;
    int cnt;
    int idx;
    AnimRunInfo* run_info;

    sprintf(path, "Save\\Current\\maps\\%s\\Anim.dat", map);

    exists = tig_file_exists(path, NULL);
    if (exists) {
        stream = tig_file_fopen(path, "r+b");
    } else {
        stream = tig_file_fopen(path, "wb");
    }

    if (stream == NULL) {
        tig_debug_printf("Anim: anim_break_nodes_to_map: ERROR: Couldn't create TimeEvent data file for map!\n");
        ASSERT(0); // 1001, "0"
        return;
    }

    cnt = 0;

    if (!exists) {
        if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
            tig_debug_printf("Anim: anim_break_nodes_to_map: ERROR: Writing Header to data file for map!\n");
            tig_file_fclose(stream);
            ASSERT(0); // 1011, "0"
            return;
        }
    } else {
        if (tig_file_fseek(stream, 0, SEEK_SET) != 0) {
            tig_debug_printf("Anim: anim_break_nodes_to_map: ERROR: Seeking to start of data file for map!\n");
            tig_file_fclose(stream);
            ASSERT(0); // 1042, "0"
            return;
        }

        if (tig_file_fread(&cnt, sizeof(cnt), 1, stream) != 1) {
            tig_debug_printf("Anim: anim_break_nodes_to_map: ERROR: Reading Header to data file for map!\n");
            tig_file_fclose(stream);
            ASSERT(0); // 1025, "0"
            return;
        }

        if (tig_file_fseek(stream, 0, SEEK_END) != 0) {
            tig_debug_printf("Anim: anim_break_nodes_to_map: ERROR: Seeking to end of data file for map!\n");
            tig_file_fclose(stream);
            ASSERT(0); // 1034, "0"
            return;
        }
    }

    for (idx = 0; idx < 216; idx++) {
        run_info = &(anim_run_info[idx]);
        if ((run_info->field_C & 0x1) != 0) {
            if (!sub_4D3420(run_info->field_20)
                || !off_5B03D0[run_info->goals[0].type]->field_C) {
                sub_44E2C0(&(run_info->id), 6);
            } else if (sub_4221C0(run_info, stream)) {
                cnt++;
                sub_44E2C0(&(run_info->id), 6);
            } else {
                ASSERT(0); // 1067, "0"
                break;
            }
        }
    }

    if (idx < 216) {
        tig_debug_printf("Anim: anim_break_nodes_to_map: ERROR: Failed to save out nodes!\n");
        ASSERT(0); // 1089, "0"
        tig_file_fclose(stream);
        tig_file_remove(path);
        return;
    }

    if (tig_file_fseek(stream, 0, SEEK_SET) != 0) {
        tig_debug_printf("Anim: anim_break_nodes_to_map: ERROR: Writing Header to data file for map!\n");
        tig_file_fclose(stream);
        ASSERT(0); // 1111, "0"
        return;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        tig_debug_printf("Anim: anim_break_nodes_to_map: ERROR: Writing Header to data file for map!\n");
        tig_file_fclose(stream);
        ASSERT(0); // 1103, "0"
        return;
    }

    tig_file_fclose(stream);
}

// 0x422DF0
void anim_save_nodes_to_map(const char* map)
{
    char path[TIG_MAX_PATH];
    TigFile* stream;
    bool exists;
    int cnt;
    int idx;
    AnimRunInfo* run_info;

    sprintf(path, "Save\\Current\\maps\\%s\\Anim.dat", map);

    exists = tig_file_exists(path, NULL);
    if (exists) {
        stream = tig_file_fopen(path, "r+b");
    } else {
        stream = tig_file_fopen(path, "wb");
    }

    if (stream == NULL) {
        tig_debug_printf("Anim: anim_save_nodes_to_map: ERROR: Couldn't create TimeEvent data file for map!\n");
        ASSERT(0); // 1138, "0"
        return;
    }

    cnt = 0;

    if (!exists) {
        if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
            tig_debug_printf("Anim: anim_save_nodes_to_map: ERROR: Writing Header to data file for map!\n");
            tig_file_fclose(stream);
            ASSERT(0); // 1148, "0"
            return;
        }
    } else {
        if (tig_file_fseek(stream, 0, SEEK_SET) != 0) {
            tig_debug_printf("Anim: anim_save_nodes_to_map: ERROR: Seeking to start of data file for map!\n");
            tig_file_fclose(stream);
            ASSERT(0); // 1179, "0"
            return;
        }

        if (tig_file_fread(&cnt, sizeof(cnt), 1, stream) != 1) {
            tig_debug_printf("Anim: anim_save_nodes_to_map: ERROR: Reading Header to data file for map!\n");
            tig_file_fclose(stream);
            ASSERT(0); // 1162, "0"
            return;
        }

        if (tig_file_fseek(stream, 0, SEEK_END) != 0) {
            tig_debug_printf("Anim: anim_save_nodes_to_map: ERROR: Seeking to end of data file for map!\n");
            tig_file_fclose(stream);
            ASSERT(0); // 1171, "0"
            return;
        }
    }

    for (idx = 0; idx < 216; idx++) {
        run_info = &(anim_run_info[idx]);
        if ((run_info->field_C & 0x1) != 0) {
            if (!sub_4221C0(run_info, stream)) {
                ASSERT(0); // 1199, "0"
                break;
            }

            cnt++;
        }
    }

    if (idx < 216) {
        tig_debug_printf("Anim: anim_save_nodes_to_map: ERROR: Failed to save out nodes!\n");
        ASSERT(0); // 1208, "0"
        tig_file_fclose(stream);
        tig_file_remove(path);
        return;
    }

    if (tig_file_fseek(stream, 0, SEEK_SET) != 0) {
        tig_debug_printf("Anim: anim_save_nodes_to_map: ERROR: Writing Header to data file for map!\n");
        tig_file_fclose(stream);
        ASSERT(0); // 1230, "0"
        return;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        tig_debug_printf("Anim: anim_save_nodes_to_map: ERROR: Writing Header to data file for map!\n");
        tig_file_fclose(stream);
        ASSERT(0); // 1222, "0"
        return;
    }

    tig_file_fclose(stream);
}

// 0x4230A0
void anim_load_nodes_from_map(const char* map)
{
    char path[TIG_MAX_PATH];
    TigFile* stream;
    int cnt;
    int idx;
    AnimID anim_id;
    AnimRunInfo run_info;

    sprintf(path, "Save\\Current\\maps\\%s\\Anim.dat", map);

    if (!tig_file_exists(path, NULL)) {
        return;
    }

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        tig_debug_printf("Anim: anim_load_nodes_from_map: ERROR: Couldn't open TimeEvent data file for map!\n");
        ASSERT(0); // 1255, "0"
        return;
    }

    if (tig_file_fread(&cnt, sizeof(cnt), 1, stream) != 1) {
        tig_debug_printf("Anim: anim_load_nodes_from_map: ERROR: Reading Header to data file for map!\n");
        tig_file_fclose(stream);
        ASSERT(0); // 1264, "0"
        return;
    }

    for (idx = 0; idx < cnt; idx++) {
        if (!sub_4227F0(&run_info, stream)) {
            break;
        }

        if ((anim_run_info[run_info.id.slot_num].field_C & 0x1) != 0) {
            if (!sub_44CCB0(&anim_id)) {
                tig_debug_printf("Anim: anim_load_nodes_from_map: ERROR: Failed to allocate a run slot!\n");
                ASSERT(0); // 1282, "0"
                break;
            }
        } else {
            anim_id = run_info.id;
        }

        anim_run_info[anim_id.slot_num] = run_info;
        anim_run_info[anim_id.slot_num].id = anim_id;
        anim_run_info[anim_id.slot_num].cur_stack_data = &(anim_run_info[anim_id.slot_num].goals[anim_run_info[anim_id.slot_num].current_goal]);
        anim_goal_restart(&anim_id);
    }

    tig_file_fclose(stream);

    if (idx < cnt) {
        tig_debug_printf("Anim: anim_load_nodes_from_map: ERROR: Failed to load all nodes!\n");
        ASSERT(0); // 1307, "0"
    }

    tig_file_remove(path);
}

// 0x4232F0
void sub_4232F0()
{
}

// 0x423300
bool sub_423300(int64_t obj, AnimID* anim_id)
{
    int prev = -1;
    int slot;
    AnimGoalNode *goal_node;

    slot = sub_44D2F0(obj);
    while (slot != -1 && slot != prev) {
        prev = slot;

        goal_node = off_5B03D0[anim_run_info[slot].goals[0].type];
        ASSERT(goal_node != NULL); // 1345, "pGoalNode != NULL"

        if (!goal_node->field_8) {
            if (anim_id != NULL) {
                *anim_id = anim_run_info[slot].id;
            }
            return true;
        }

        slot = sub_44D340(slot, obj);
    }

    if (anim_id != NULL) {
        sub_421DE0(anim_id);
    }

    return false;
}

// 0x4233D0
int sub_4233D0(int64_t obj)
{
    int prev = -1;
    int slot;
    AnimGoalNode *goal_node;

    slot = sub_44D2F0(obj);
    while (slot != -1 && slot != prev) {
        prev = slot;

        goal_node = off_5B03D0[anim_run_info[slot].goals[0].type];
        ASSERT(goal_node != NULL); // 1383, "pGoalNode != NULL"

        if (!goal_node->field_8) {
            return goal_node->priority_level;
        }

        slot = sub_44D340(slot, obj);
    }

    return 0;
}

// 0x423470
bool sub_423470(int64_t obj)
{
    int index;
    AnimRunInfo* run_info;
    tig_art_id_t art_id;

    index = sub_44D2F0(obj);
    run_info = &(anim_run_info[index]);
    if (run_info->current_goal != 0) {
        return false;
    }

    if (run_info->cur_stack_data == NULL) {
        run_info->cur_stack_data = &(run_info->goals[0]);
    }

    if (run_info->cur_stack_data->type > 2) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    if (sub_503E20(art_id)) {
        return false;
    }

    return true;
}

// 0x4234F0
bool sub_4234F0(int64_t obj)
{
    return !sub_44E830(obj, 2, NULL);
}

// 0x423530
void sub_423530(AnimRunInfo* run_info)
{
    (void)run_info;
}

// 0x423540
void sub_423540(AnimRunInfo* run_info)
{
    (void)run_info;
}

// 0x423550
void sub_423550(AnimRunInfo* run_info, int a2)
{
    (void)run_info;
    (void)a2;
}

// 0x423560
bool anim_timeevent_process(TimeEvent* timeevent)
{
    // TODO: Incomplete.
}

// 0x423C80
bool sub_423C80(AnimRunInfo* run_info, DateTime* a2, int delay)
{
    TimeEvent timeevent;
    DateTime datetime;

    sub_45A950(&datetime, delay);
    datetime.milliseconds *= 8;

    timeevent.type = TIMEEVENT_TYPE_ANIM;
    timeevent.params[0].integer_value = run_info->id.slot_num;
    timeevent.params[1].integer_value = run_info->id.field_4;
    timeevent.params[2].integer_value = 1111;

    if (dword_5DE6D0) {
        return sub_45BA30(&timeevent, &datetime, a2, &(run_info->field_18));
    } else {
        return sub_45B880(&timeevent, &datetime, &(run_info->field_18));
    }
}

// 0x423D10
void sub_423D10()
{
    // TODO: Incomplete.
}

// 0x423E60
void sub_423E60(const char* msg)
{
    int cnt;

    cnt = anim_goal_pending_active_goals_count();
    if (cnt != animNumActiveGoals) {
        tig_debug_printf("Anim: Num Active Goals Failure: %s, Expected: %d, Actual: %d!\n",
            msg,
            animNumActiveGoals,
            cnt);

        if (!in_anim_load) {
            ASSERT(0);
        }
    }
}

// 0x423EB0
int anim_goal_pending_active_goals_count()
{
    int index;
    AnimRunInfo* run_info;
    int cnt = 0;
    int stack_index;

    for (index = 0; index < 216; index++) {
        run_info = &(anim_run_info[index]);
        if ((run_info->field_C & 0x1) != 0) {
            for (stack_index = 0; stack_index <= run_info->current_goal; stack_index++) {
                ASSERT(run_info->goals[stack_index].type >= 0); // pRunInfo->goal_stack[j].goal_type >= 0
                ASSERT(run_info->goals[stack_index].type < ANIM_GOAL_MAX); // pRunInfo->goal_stack[j].goal_type < anim_goal_max

                if (run_info->goals[stack_index].type >= 0 && run_info->goals[stack_index].type < ANIM_GOAL_MAX) {
                    if (off_5B03D0[run_info->goals[stack_index].type]->priority_level >= 2
                        && !off_5B03D0[run_info->goals[stack_index].type]->field_8) {
                        cnt++;
                    }
                } else {
                    tig_debug_printf("Anim: anim_goal_pending_active_goals_count: ERROR: goal Type invalid: %d, Slot: %d, Stack Index: %d!\n",
                        run_info->goals[stack_index].type,
                        index,
                        stack_index);
                    mp_deallocate_run_index(&(run_info->id));
                }
            }
        }
    }

    return cnt;
}

// 0x423FB0
void sub_423FB0()
{
    animNumActiveGoals = anim_goal_pending_active_goals_count();
}

// 0x423FC0
void sub_423FC0()
{
    dword_5DE6D0 = true;
}

// 0x423FE0
void sub_423FE0(void(*func)())
{
    dword_5E34F8 = func;
}

// 0x423FF0
bool sub_423FF0(int64_t obj)
{
    int v1 = -1;
    int v2;

    if (light_scheme_is_changing()) {
        return true;
    }

    v2 = sub_44D2F0(obj);
    if (v2 == -1) {
        return true;
    }

    while (v2 != v1) {
        v1 = v2;
        if (!sub_44E160(&(anim_run_info[v2].id))) {
            return false;
        }
        v2 = sub_44D340(v2, obj);
        if (v2 == -1) {
            return true;
        }
    }

    return false;
}

// 0x424070
bool sub_424070(int64_t obj, int priority_level, bool a3, bool a4)
{
    Packet9 pkt;
    int v1 = -1;
    int v2;

    ASSERT(priority_level >= PRIORITY_NONE && priority_level < PRIORITY_HIGHEST); // (priorityLevel >= priorityNone)&&(priorityLevel <= priorityHighest)

    if (!a4) {
        if ((tig_net_flags & TIG_NET_CONNECTED) != 0 && sub_40DA20(obj)) {
            pkt.type = 9;
            pkt.field_4 = 0;

            if ((tig_net_flags & TIG_NET_HOST) != 0) {
                sub_4440E0(obj, &(pkt.field_18));
                pkt.priority_level = priority_level;
                pkt.field_48 = a3;
                pkt.loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
                pkt.art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
                pkt.offset_x = obj_field_int32_get(obj, OBJ_F_OFFSET_X);
                pkt.offset_y = obj_field_int32_get(obj, OBJ_F_OFFSET_Y);
                tig_net_send_app_all(&pkt, sizeof(pkt));
            } else {
                sub_4440E0(obj, &(pkt.field_18));
                pkt.priority_level = priority_level;
                pkt.field_48 = a3;
                pkt.loc = 0;
                pkt.art_id = TIG_ART_ID_INVALID;
                pkt.offset_x = 0;
                pkt.offset_y = 0;
                tig_net_send_app_all(&pkt, sizeof(pkt));
            }
        }
    }

    if (!a3) {
        priority_level = PRIORITY_NONE;
    }

    v2 = sub_44D2F0(obj);
    if (v2 == -1) {
        return true;
    }

    while (v2 != v1) {
        v1 = v2;
        if (!sub_44E2C0(&(anim_run_info[v2].id), priority_level)) {
            return false;
        }
        v2 = sub_44D340(v2, obj);
        if (v2 == -1) {
            return true;
        }
    }

    return false;
}

// 0x424250
bool sub_424250()
{
    int index;

    if (dword_5E3500 > 0) {
        for (index = 0; index < 216; index++) {
            if ((anim_run_info[index].field_C & 0x1) != 0
                && !sub_44E2C0(&(anim_run_info[index].id), PRIORITY_HIGHEST)) {
                return false;
            }
        }
    }

    return true;
}

// 0x424290
bool anim_goal_interrupt_all_goals_of_priority(int priority_level)
{
    int index;

    ASSERT(priority_level >= PRIORITY_NONE && priority_level < PRIORITY_HIGHEST); // (priorityLevel >= priorityNone)&&(priorityLevel <= priorityHighest)

    for (index = 0; index < 216; index++) {
        if ((anim_run_info[index].field_C & 0x1) != 0
            && !sub_44E2C0(&(anim_run_info[index].id), PRIORITY_HIGHEST)) {
            tig_debug_printf("Anim: anim_goal_interrupt_all_goals_of_priority: ERROR: Failed to interrupt slot: %d!\n", index);
        }
    }

    return true;
}

// 0x424310
bool anim_goal_interrupt_all_for_tb_combat()
{
    int index = 0;
    AnimRunInfo* run_info;

    for (index = 0; index < 216; index++) {
        run_info = &(anim_run_info[index]);
        if ((run_info->field_C & 0x1) != 0
            && !sub_44C9A0(run_info)
            && !sub_44E2C0(&(run_info->id), 3)) {
            tig_debug_printf("Anim: anim_goal_interrupt_all_for_tb_combat: ERROR: Failed to interrupt slot: %d!\n", index);
        }
    }

    return true;
}

// 0x4243E0
bool sub_4243E0(int64_t obj, tig_art_id_t eye_candy_id, int a3)
{
    AnimID prev_anim_id;
    AnimID cur_anim_id;
    unsigned int num;
    AnimRunInfo* run_info;

    sub_421DE0(&prev_anim_id);

    if (obj == OBJ_HANDLE_NULL) {
        return true;
    }

    if (!anim_find_first_of_type(obj, AG_EYE_CANDY, &cur_anim_id)) {
        return true;
    }

    num = tig_art_num_get(eye_candy_id);

    do {
        if (sub_421D60(&prev_anim_id, &cur_anim_id)) {
            break;
        }

        run_info = &(anim_run_info[cur_anim_id.slot_num]);

        ASSERT(run_info->current_goal > -1); // 2839, "pRunInfo->current_goal > -1"
        ASSERT(run_info->current_goal < 14); // 2840, "pRunInfo->current_goal < ANIM_GOAL_MAX_SUBNODES"

        if (run_info->current_goal > -1 && run_info->current_goal < 14) {
            if (run_info->cur_stack_data == NULL) {
                run_info->cur_stack_data = &(run_info->goals[run_info->current_goal]);
            }

            if (run_info->cur_stack_data->params[AGDATA_SPELL_DATA].data == a3
                && tig_art_num_get(run_info->cur_stack_data->params[AGDATA_ANIM_ID].data) == num
                && sub_44E2C0(&(run_info->id), PRIORITY_HIGHEST)) {
                break;
            }
        }
    } while (anim_find_next_of_type(obj, AG_EYE_CANDY, &cur_anim_id));

    return true;
}

// 0x424560
bool sub_424560(int64_t obj, tig_art_id_t eye_candy_id, int a3)
{
    AnimID prev_anim_id;
    AnimID cur_anim_id;
    unsigned int num;
    AnimRunInfo* run_info;

    sub_421DE0(&prev_anim_id);

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!anim_find_first_of_type(obj, AG_EYE_CANDY, &cur_anim_id)) {
        return false;
    }

    num = tig_art_num_get(eye_candy_id);

    do {
        if (sub_421D60(&prev_anim_id, &cur_anim_id)) {
            break;
        }

        run_info = &(anim_run_info[cur_anim_id.slot_num]);
        if (run_info->cur_stack_data == NULL) {
            run_info->cur_stack_data = &(run_info->goals[run_info->current_goal]);
        }

        if (run_info->cur_stack_data->params[AGDATA_SPELL_DATA].data == a3
            && tig_art_num_get(run_info->cur_stack_data->params[AGDATA_ANIM_ID].data) == num) {
            return true;
        }

        prev_anim_id = cur_anim_id;
    } while (anim_find_next_of_type(obj, AG_EYE_CANDY, &cur_anim_id));

    return false;
}

// 0x4246C0
bool sub_4246C0(AnimRunInfo* run_info)
{
    (void)run_info;

    return 1;
}

// 0x4246D0
bool sub_4246D0(AnimRunInfo* run_info)
{
    (void)run_info;

    return sub_4B6D70() == 0;
}

// 0x4246E0
bool sub_4246E0(AnimRunInfo* run_info)
{
    int64_t obj;
    int64_t parent_obj;
    AnimGoalData goal_data;
    AnimID anim_id;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0
        && !sub_4A2BA0()) {
        return true;
    }

    obj = run_info->params[0].obj;
    parent_obj = run_info->params[1].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 3001, "obj != OBJ_HANDLE_NULL"
    ASSERT(parent_obj != OBJ_HANDLE_NULL); // 3002, "parentObj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL || parent_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    sub_44D4E0(&goal_data, obj, run_info->params[2].data);

    // TODO: Incomplete.

    if (!sub_44D520(&goal_data, &anim_id)) {
        return false;
    }

    return true;
}

// 0x424820
bool sub_424820(AnimRunInfo* run_info)
{
    ObjectID oid;

    oid.type = OID_TYPE_NULL;
    return sub_4248A0(run_info->params[AGDATA_SCRATCH_VAL4].data,
        run_info->params[AGDATA_SELF_OBJ].obj,
        run_info->params[AGDATA_TARGET_OBJ].obj,
        run_info->params[AGDATA_TARGET_OBJ].obj,
        run_info->params[AGDATA_TARGET_TILE].loc,
        run_info->params[AGDATA_SPELL_DATA].data,
        &(run_info->params[AGDATA_SCRATCH_OBJ].obj),
        run_info->id,
        oid);
}

// 0x4248A0
bool sub_4248A0(int a1, int64_t self_obj, int64_t a3, int64_t a4, int64_t loc, int spell, int64_t* obj_ptr, AnimID anim_id, ObjectID oid)
{
    // TODO: Incomplete.
}

// 0x424BC0
bool sub_424BC0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x424D00
bool sub_424D00(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    int p_piece;

    obj = run_info->params[0].obj;
    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_DESTROYED) != 0) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    p_piece = tig_art_wall_id_p_piece_get(art_id);
    if (tig_art_type(art_id) != TIG_ART_TYPE_WALL
        || p_piece == 0
        || p_piece == 2) {
        sub_43D0E0(obj, OF_OFF);
    }

    sub_43CCA0(obj);

    return true;
}

// 0x424D90
bool sub_424D90(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 3321, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    return obj_field_int64_get(obj, OBJ_F_LOCATION) == obj_field_int64_get(run_info->params[1].obj, OBJ_F_LOCATION);
}

// 0x424E00
bool sub_424E00(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x425130
bool sub_425130(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    int range;
    int64_t source_loc;
    int64_t target_loc;
    int direction;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;
    range = run_info->cur_stack_data->params[AGDATA_RANGE_DATA].data;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 3532, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 3533, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    source_loc = obj_field_int64_get(source_obj, OBJ_F_LOCATION);

    if (sub_4B96F0(source_loc, target_loc) > range) {
        return false;
    }

    direction = sub_4B8D50(source_loc, target_loc);
    if (sub_425840(source_obj, source_loc, target_loc, direction, target_obj)) {
        return false;
    }

    return true;
}

// 0x425270
bool sub_425270(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_loc;
    int range;
    int64_t source_loc;

    source_obj = run_info->params[0].obj;
    target_loc = run_info->params[1].loc;
    range = run_info->cur_stack_data->params[AGDATA_RANGE_DATA].data;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 3584, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_loc != 0); // 3585, "targetLoc != 0"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_loc == 0) {
        return false;
    }

    source_loc = obj_field_int64_get(source_obj, OBJ_F_LOCATION);
    if (sub_4B96F0(source_loc, target_loc) > range) {
        return false;
    }

    return true;
}

// 0x425340
bool sub_425340(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    int64_t range;
    int64_t source_loc;
    int64_t target_loc;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;
    range = run_info->cur_stack_data->params[AGDATA_RANGE_DATA].data + 2;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 3613, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 3614, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    source_loc = obj_field_int64_get(source_obj, OBJ_F_LOCATION);

    if (sub_4B96F0(source_loc, target_loc) > range) {
        return false;
    }

    return true;
}

// 0x425430
bool sub_425430(AnimRunInfo* run_info)
{
    if ((run_info->path.flags & 0xC) == 0) {
        return false;
    }

    sub_430FC0(run_info);

    if (run_info->current_goal > 0) {
        // TODO: Incomplete.
    }

    run_info->path.flags &= ~0x4;
    run_info->path.flags |= 0x1;

    run_info->field_C &= ~0x30;

    if ((run_info->path.flags & 0x8) != 0
        && ((tig_net_flags & TIG_NET_CONNECTED) == 0
            || (tig_net_flags & TIG_NET_HOST) != 0)) {
        run_info->field_C |= 0x2;
    }

    return true;
}

// 0x4254C0
bool sub_4254C0(AnimRunInfo* run_info)
{
    if (run_info->params[1].obj != OBJ_HANDLE_NULL
        && run_info->cur_stack_data->params[AGDATA_TARGET_TILE].loc != obj_field_int64_get(run_info->params[1].obj, OBJ_F_LOCATION)) {
        run_info->path.flags |= 0x4;
    }

    if ((run_info->path.flags & 0xC) == 0) {
        return false;
    }

    sub_430FC0(run_info);

    if (run_info->current_goal > 0) {
        // TODO: Incomplete.
    }

    run_info->path.flags &= ~0x4;
    run_info->path.flags |= 0x1;

    run_info->field_C &= ~0x30;

    if ((run_info->path.flags & 0x8) != 0
        && ((tig_net_flags & TIG_NET_CONNECTED) == 0
            || (tig_net_flags & TIG_NET_HOST) != 0)) {
        run_info->field_C |= 0x2;
    }

    return true;
}

// 0x425590
bool sub_425590(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x425740
bool sub_425740(AnimRunInfo* run_info)
{
    return (run_info->path.flags & 0x1) == 0;
}

// 0x425760
bool sub_425760(int64_t a1, int64_t a2, int64_t a3, int a4)
{
    unsigned int flags = 0;

    sub_4257E0(a1, &flags);

    if (sub_4D7110(a3, false)) {
        return true;
    }

    return sub_43FD70(a1, a2, a4, flags, 0);
}

// 0x4257E0
void sub_4257E0(int64_t obj, unsigned int* flags_ptr)
{
    if ((obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_POLYMORPHED) != 0) {
        *flags_ptr |= 0x3;
    }

    if (!critter_can_open_portals(obj)) {
        *flags_ptr |= 0x1;
    }

    if (!sub_45F570(obj)) {
        *flags_ptr |= 0x2;
    }
}

// 0x425840
bool sub_425840(int64_t a1, int64_t a2, int64_t a3, int a4, int64_t a5)
{
    unsigned int flags = 0;
    bool v1 = true;
    bool v2 = false;
    int64_t v3;
    int v4;

    sub_4257E0(a1, &flags);

    if (sub_4D7110(a3, false)) {
        return true;
    }

    if ((obj_field_int32_get(a5, OBJ_F_FLAGS) & OF_NO_BLOCK) == 0) {
        sub_43D0E0(a5, OF_NO_BLOCK);
        v1 = false;
    }

    if ((sub_43FDC0(a1, a2, a4, flags, &v3, &v4, 0) || v3) && !v4) {
        v2 = true;
    }

    if (!v1) {
        sub_43D280(a5, OF_NO_BLOCK);
    }

    return v2;
}

// 0x425930
bool sub_425930(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x425BF0
bool sub_425BF0(PathCreateInfo* path_create_info, bool a2)
{
    ASSERT(path_create_info != NULL); // 3923, "pPathData != NULL"
    ASSERT(path_create_info->obj != OBJ_HANDLE_NULL); // 3924, "pPathData->movingObj != OBJ_HANDLE_NULL"

    if (obj_type_is_critter(obj_field_int32_get(path_create_info->obj, OBJ_F_TYPE))) {
        if ((obj_field_int32_get(path_create_info->obj, OBJ_F_SPELL_FLAGS) & OSF_ENTANGLED) != 0) {
            return false;
        }

        if (!critter_can_open_portals(path_create_info->obj)) {
            path_create_info->field_20 |= 0x0002;
        }

        if (!sub_45F570(path_create_info->obj)) {
            path_create_info->field_20 |= 0x0004;
        }

        if (critter_is_concealed(path_create_info->obj)
            && basic_skill_get_training(path_create_info->obj, BASIC_SKILL_PROWLING) <= 0) {
            path_create_info->field_20 |= 0x0200;
        }

        if (sub_43D6D0(path_create_info->obj, 1, false) < 45
            && stat_level(path_create_info->obj, STAT_STRENGTH) != 0) {
            path_create_info->field_20 |= 0x0400;
        }

        if (a2) {
            if (!combat_critter_is_combat_mode_active(path_create_info->obj)) {
                path_create_info->field_20 |= 0x0010;
            }
        }
    }

    return true;
}

// 0x425D60
bool sub_425D60(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x426040
bool sub_426040(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x4261E0
int sub_4261E0(int64_t a1, int64_t a2)
{
    AnimPath path;

    path.flags = 0;
    path.curr = 0;
    path.max = 0;
    path.absMaxPathLength = 0;
    path.maxPathLength = 0;
    path.baseRot = 0;
    if (!sub_426500(a1, a2, &path, 0x0)) {
        return 0;
    }

    return path.max;
}

// 0x426250
int sub_426250(int64_t a1, int64_t a2)
{
    AnimPath path;

    path.flags = 1;
    path.curr = 0;
    path.max = 0;
    path.absMaxPathLength = 0;
    path.maxPathLength = 0;
    path.baseRot = 0;
    if (!sub_426500(a1, a2, &path, 0x1)) {
        return 0;
    }

    return path.max;
}

// 0x4262D0
void anim_create_path_max_length()
{
    // TODO: Incomplete.
}

// 0x426320
void sub_426320()
{
    // TODO: Incomplete.
}

// 0x426500
void sub_426500()
{
    // TODO: Incomplete.
}

// 0x426560
void sub_426560()
{
    // TODO: Incomplete.
}

// 0x426840
bool sub_426840(AnimRunInfo* run_info)
{
    int64_t obj;
    int64_t target_loc;
    int64_t source_loc;

    obj = run_info->params[0].obj;
    target_loc = run_info->params[1].loc;

    ASSERT(obj != OBJ_HANDLE_NULL); // 4643, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    run_info->field_14 = run_info->current_goal;

    if (target_loc == 0) {
        return false;
    }

    // TODO: Incomplete.

    source_loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
    run_info->path.max = sub_4201C0(source_loc, target_loc, &(run_info->path.rotations));
    if (run_info->path.max == 0) {
        return false;
    }

    run_info->path.curr = 0;
    run_info->path.flags &= ~0x3;

    return true;
}

// 0x4268F0
bool sub_4268F0(AnimRunInfo* run_info)
{
    int64_t obj;
    int64_t target_loc;
    int64_t source_loc;

    obj = run_info->params[0].obj;
    target_loc = run_info->params[1].loc;

    ASSERT(obj != OBJ_HANDLE_NULL); // 4689, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    run_info->field_14 = run_info->current_goal;

    if (target_loc == 0) {
        return false;
    }

    source_loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
    run_info->path.baseRot = sub_4B8D50(source_loc, target_loc);
    run_info->path.max = sub_4201C0(source_loc, target_loc, &(run_info->path.rotations));
    if (run_info->path.max == 0) {
        return false;
    }

    run_info->path.curr = 0;
    run_info->path.flags &= ~0x3;

    return true;
}

// 0x4269D0
bool sub_4269D0(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 4738, "obj != OBJ_HANDLE_NULL"
    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    run_info->field_14 = run_info->current_goal;

    if (run_info->params[1].loc == 0
        || run_info->params[1].loc == -1) {
        return false;
    }

    run_info->path.max = sub_4201C0(obj_field_int64_get(obj, OBJ_F_LOCATION),
        run_info->params[1].loc,
        run_info->path.rotations);
    if (run_info->path.max == 0) {
        return false;
    }

    run_info->path.curr = 0;
    run_info->path.flags &= ~0x03;

    return true;
}

// 0x426A80
bool sub_426A80(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x426E80
bool sub_426E80(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 4919, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return false;
    }

    if (sub_503E20(obj_field_int32_get(obj, OBJ_F_CURRENT_AID)) != 5) {
        return false;
    }

    if (sub_40DA20(obj)) {
        return false;
    }

    return true;
}

// 0x426F10
bool sub_426F10(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 4946, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    return sub_45D730(obj);
}

// 0x426F60
bool sub_426F60(AnimRunInfo* run_info)
{
    int64_t obj;
    unsigned int flags;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 4961, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        || (tig_net_flags & TIG_NET_HOST) == 0) {
        return false;
    }

    if (!sub_4B6D70()) {
        run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data--;
    }

    if (run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data > 0) {
        return false;
    }

    flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS);
    if ((flags & OCF_STUNNED) != 0) {
        flags &= ~OCF_STUNNED;
        sub_4EFDD0(obj, OBJ_F_CRITTER_FLAGS, flags);
    }

    return true;
}

// 0x427000
bool sub_427000(int64_t obj)
{
    AnimID anim_id;
    AnimRunInfo* run_info;

    if (sub_4B6D70()
        && sub_4B6D80() == obj
        && (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_STUNNED) != 0
        && sub_44E830(obj, AG_ANIMATE_STUNNED, &anim_id)
        && anim_id_to_run_info(&anim_id, &run_info)) {
        run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data -= 3;
        if (run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data <= 0) {
            sub_44E2C0(&anim_id, PRIORITY_HIGHEST);
        }
    }

    return true;
}

// 0x4270B0
bool sub_4270B0(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 4961, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    return sub_427110(run_info,
        obj,
        obj_field_int64_get(obj, OBJ_F_LOCATION));
}

// 0x427110
bool sub_427110(AnimRunInfo* run_info, int64_t obj, int64_t loc)
{
    // TODO: Incomplete.
}

// 0x4272E0
bool sub_4272E0(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    int64_t loc;
    int rotation;
    int64_t v1;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 5202, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    loc = obj_field_int64_get(obj, OBJ_F_LOCATION);

    if ((run_info->path.flags & 0x1) != 0) {
        rotation = tig_art_id_rotation_get(art_id);
    } else {
        rotation = run_info->path.rotations[run_info->path.curr];
    }

    if (!sub_440700(obj, loc, rotation, 0, &v1)) {
        return false;
    }

    run_info->cur_stack_data->params[AGDATA_SCRATCH_OBJ].obj = v1;

    return true;
}

// 0x4273B0
bool sub_4273B0(int64_t obj, int64_t loc, int rotation, int a4, int64_t* obj_ptr)
{
    // TODO: Incomplete.
}

// 0x427640
bool sub_427640(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    int64_t loc;
    int rotation;
    int64_t v1;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 5401, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    loc = obj_field_int64_get(obj, OBJ_F_LOCATION);

    if ((run_info->path.flags & 0x1) != 0) {
        rotation = tig_art_id_rotation_get(art_id);
    } else {
        rotation = run_info->path.rotations[run_info->path.curr];
    }

    if (!sub_4273B0(obj, loc, rotation, 0, &v1)) {
        return false;
    }

    run_info->cur_stack_data->params[AGDATA_SCRATCH_OBJ].obj = v1;

    return true;
}

// 0x427710
bool sub_427710(AnimRunInfo* run_info)
{
    (void)run_info;

    return false;
}

// 0x427720
bool sub_427720(AnimRunInfo* run_info)
{
    (void)run_info;

    return true;
}

// 0x427730
bool sub_427730(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x427990
bool sub_427990(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x4280D0
bool sub_4280D0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x4284A0
bool sub_4284A0(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PORTAL) {
        return false;
    }

    return true;
}

// 0x4284F0
bool sub_4284F0(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if (sub_4F08C0(obj)) {
        return false;
    }

    if (sub_423300(obj, NULL)) {
        return false;
    }

    return true;
}

// 0x428550
bool sub_428550(AnimRunInfo* run_info)
{
    int64_t door_obj;
    int64_t self_obj;
    int sound_id;

    door_obj = run_info->params[0].obj;
    self_obj = run_info->params[1].obj;

    ASSERT(door_obj != OBJ_HANDLE_NULL); // 6093, "doorObj != OBJ_HANDLE_NULL"
    ASSERT(self_obj != OBJ_HANDLE_NULL); // 6093, "selfObj != OBJ_HANDLE_NULL"

    if (door_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (self_obj == OBJ_HANDLE_NULL) {
        // FIXME: Probably wrong.
        return true;
    }

    if ((obj_field_int32_get(door_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if (sub_4AEB70(self_obj, door_obj, sub_441B20(self_obj, door_obj))) {
        sound_id = sub_4F1010(door_obj, 2);
        sub_41B930(sound_id, 1, door_obj);
        return false;
    }

    return true;
}

// 0x428620
bool sub_428620(AnimRunInfo* run_info)
{
    int64_t door_obj;

    door_obj = run_info->params[0].obj;

    ASSERT(door_obj != OBJ_HANDLE_NULL); // 6125, "doorObj != OBJ_HANDLE_NULL"

    if (door_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(door_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if ((obj_field_int32_get(door_obj, OBJ_F_PORTAL_FLAGS) & OPF_MAGICALLY_HELD) != 0) {
        return false;
    }

    return true;
}

// 0x428690
bool sub_428690(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t portal_obj;

    source_obj = run_info->params[0].obj;
    portal_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 6151, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(portal_obj != OBJ_HANDLE_NULL); // 6152, "portalObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (portal_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!critter_can_open_portals(source_obj)) {
        return false;
    }

    if (sub_4AEB70(source_obj, portal_obj, sub_441B20(source_obj, portal_obj)) != 0) {
        return false;
    }

    return true;
}

// 0x428750
bool sub_428750(AnimRunInfo* run_info)
{
    int64_t door_obj;
    int64_t v1;

    door_obj = run_info->params[0].obj;
    v1 = run_info->params[1].obj;

    ASSERT(door_obj != OBJ_HANDLE_NULL); // 6183, "doorObj != OBJ_HANDLE_NULL"

    if ((obj_field_int32_get(door_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if (sub_4AEB70(v1, door_obj, sub_441B20(v1, door_obj)) == 0) {
        return false;
    }

    return true;
}

// 0x4287E0
bool sub_4287E0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t door_obj;

    source_obj = run_info->params[0].obj;
    door_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 6221, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(door_obj != OBJ_HANDLE_NULL); // 6222, "portalObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (door_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(door_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    // FIXME: Probably wrong.
    sub_4AEB70(source_obj, door_obj, sub_441B20(source_obj, door_obj));

    return false;
}

// 0x428890
bool sub_428890(AnimRunInfo* run_info)
{
    (void)run_info;

    return false;
}

// 0x4288A0
bool sub_4288A0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t v1;

    source_obj = run_info->params[0].obj;
    v1 = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 6259, "sourceObj != OBJ_HANDLE_NULL"

    if (!sub_45F570(source_obj)) {
        return false;
    }

    if (v1 == OBJ_HANDLE_NULL) {
        return true;
    }

    if (sub_4AEB70(source_obj, v1, sub_441B20(source_obj, v1)) != 0) {
        return false;
    }

    return true;
}

// 0x428930
bool sub_428930(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    int range = 2;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 6288, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 6289, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    switch (obj_field_int32_get(target_obj, OBJ_F_TYPE)) {
    case OBJ_TYPE_CONTAINER:
    case OBJ_TYPE_PC:
    case OBJ_TYPE_NPC:
        range = 1;
        break;
    }

    run_info->cur_stack_data->params[AGDATA_RANGE_DATA].data = range;

    return true;
}

// 0x428A10
bool sub_428A10(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x428CD0
bool sub_428CD0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x428E10
bool sub_428E10(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x429040
bool sub_429040(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x429160
bool sub_429160(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x429370
bool sub_429370(AnimRunInfo* run_info)
{
    (void)run_info;

    return false;
}

// 0x429380
bool sub_429380(AnimRunInfo* run_info)
{
    (void)run_info;

    return true;
}

// 0x429390
bool sub_429390(AnimRunInfo* run_info)
{
    (void)run_info;

    return true;
}

// 0x4293A0
bool sub_4293A0(AnimRunInfo* run_info)
{
    (void)run_info;

    return true;
}

// 0x4293B0
bool sub_4293B0(AnimRunInfo* run_info)
{
    (void)run_info;

    return false;
}

// 0x4293C0
bool sub_4293C0(AnimRunInfo* run_info)
{
    (void)run_info;

    return false;
}

// 0x4293D0
bool sub_4293D0(AnimRunInfo* run_info)
{
    int64_t critter_obj;

    critter_obj = run_info->params[0].obj;

    ASSERT(critter_obj != OBJ_HANDLE_NULL); // 6633, "critObj != OBJ_HANDLE_NULL"

    if (critter_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    sub_4AF1D0(critter_obj);

    return true;
}

// 0x429420
bool sub_429420(AnimRunInfo* run_info)
{
    (void)run_info;

    return false;
}

// 0x429430
bool sub_429430(AnimRunInfo* run_info)
{
    (void)run_info;

    return false;
}

// 0x429440
bool sub_429440(AnimRunInfo* run_info)
{
    (void)run_info;

    return true;
}

// 0x429450
bool sub_429450(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        && sub_45D8D0(obj)) {
        return false;
    }

    return true;
}

// 0x4294A0
bool sub_4294A0(AnimRunInfo* run_info)
{
    bool rc;

    rc = sub_4294F0(run_info->params[0].obj, run_info->params[1].obj);
    if (!rc) {
        if (sub_4B6D70()) {
            sub_4B7C90(run_info->params[0].obj);
        }
    }

    return rc;
}

// 0x4294F0
bool sub_4294F0(int64_t source_obj, int64_t target_obj)
{
    // TODO: Incomplete.
}

// 0x4296D0
bool sub_4296D0(AnimRunInfo* run_info)
{
    int64_t self_obj;
    int goal;

    self_obj = run_info->params[0].obj;

    ASSERT(self_obj != OBJ_HANDLE_NULL); // 6826, "selfObj != OBJ_HANDLE_NULL"

    if (self_obj == OBJ_HANDLE_NULL) {
        return true;
    }

    if (run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data != -1) {
        sub_41B930(run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data, 1, self_obj);
        for (goal = 0; goal <= run_info->current_goal; goal++) {
            run_info->goals[goal].params[AGDATA_SCRATCH_VAL5].data = -1;
        }
    }

    return true;
}

// 0x429760
bool sub_429760(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 6854, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 6855, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!combat_critter_is_combat_mode_active(source_obj)) {
        return false;
    }

    if (!sub_40DA20(source_obj)) {
        return false;
    }

    if (!combat_auto_attack_get(source_obj)) {
        return false;
    }

    if ((obj_field_int32_get(target_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if (sub_45D8D0(target_obj)) {
        return false;
    }

    if (object_is_destroyed(target_obj)) {
        return false;
    }

    if ((run_info->field_C & 0x800) != 0) {
        return false;
    }

    if (sub_4B6D70()) {
        return false;
    }

    if ((obj_field_int32_get(source_obj, OBJ_F_CRITTER_FLAGS) & OCF_NON_LETHAL_COMBAT) != 0) {
        return false;
    }

    if ((obj_field_int32_get(source_obj, OBJ_F_CRITTER_FLAGS2) & OCF2_USING_BOOMERANG) != 0) {
        return false;
    }

    run_info->path.maxPathLength = 0;
    run_info->field_C &= ~0x83C;

    return true;
}

// 0x4298D0
bool sub_4298D0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t pc_obj;

    source_obj = run_info->params[0].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 6918, "sourceObj != OBJ_HANDLE_NULL"

    if (combat_critter_is_combat_mode_active(source_obj)) {
        return true;
    }

    if ((obj_field_int32_get(source_obj, OBJ_F_CRITTER_FLAGS2) & OCF2_AUTO_ANIMATES) == 0) {
        return false;
    }

    pc_obj = player_get_pc_obj();
    if (sub_441AE0(source_obj, pc_obj) >= 30) {
        return false;
    }

    return true;
}

// 0x429960
bool sub_429960(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x429AD0
bool sub_429AD0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t weapon_obj;

    source_obj = run_info->params[0].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7008, "sourceObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    weapon_obj = sub_4B23B0(source_obj);
    if (weapon_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (item_weapon_range(weapon_obj, source_obj) <= 1) {
        return false;
    }

    return true;
}

// 0x429B40
bool sub_429B40(AnimRunInfo* run_info)
{
    (void)run_info;

    return true;
}

// 0x429B50
bool sub_429B50(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 7039, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_4B7CD0(obj, 4)) {
        return false;
    }

    if (!sub_450940(run_info->cur_stack_data->params[AGDATA_SPELL_DATA].data)) {
        return false;
    }

    return true;
}

// 0x429BB0
bool sub_429BB0(AnimRunInfo* run_info)
{
    (void)run_info;

    return true;
}

// 0x429BC0
bool sub_429BC0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7070, "sourceObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj != OBJ_HANDLE_NULL) {
        if ((obj_field_int32_get(target_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
            return false;
        }

        // FIXME: Unused.
        obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    }

    return true;
}

// 0x429C40
bool sub_429C40(AnimRunInfo* run_info)
{
    tig_art_id_t art_id;

    art_id = sub_458B70(run_info->params[0].data);
    if (art_id == TIG_ART_ID_INVALID) {
        return false;
    }

    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL4].data = art_id;

    sub_456E00(run_info->params[0].data);

    return true;
}

// 0x429C80
bool sub_429C80(AnimRunInfo* run_info)
{
    if (!sub_456A90(run_info->params[0].data)) {
        return false;
    }

    if (sub_458B70(run_info->params[0].data) == TIG_ART_ID_INVALID) {
        sub_456E00(run_info->params[0].data);
    }

    run_info->field_C |= 0xC;

    sub_456F70(run_info->params[0].data);

    return true;
}

// 0x429CD0
bool sub_429CD0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x429E70
bool sub_429E70(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 7243, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_DESTROYED) == 0) {
        sub_43CCA0(obj);
    }

    return true;
}

// 0x429ED0
bool sub_429ED0(AnimRunInfo* run_info)
{
    if (run_info->params[0].data == -1) {
        return false;
    }

    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data = sub_458CA0(run_info->params[0].data);

    return true;
}

// 0x429F00
bool sub_429F00(AnimRunInfo* run_info)
{
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;

    if (!sub_456A90(run_info->params[0].data)) {
        return false;
    }

    if (!sub_456D20(run_info->params[0].data, &v1, &v2, &v3, &v4, &v5, &v6, &v7)) {
        run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL4].data = -1;
        run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data = -1;
        run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL6].data = -1;
        return false;
    }

    if (run_info->cur_stack_data == NULL) {
        run_info->cur_stack_data = &(run_info->goals[run_info->current_goal]);
    }

    run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = v1;
    run_info->cur_stack_data->params[AGDATA_SKILL_DATA].data = v7;
    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data = v2;
    run_info->cur_stack_data->params[AGDATA_RANGE_DATA].data = v3;
    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL1].data = v4;
    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL2].data = v5;
    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL3].data = v6;
    run_info->field_C |= 0x2000;

    return true;
}

// 0x42A010
bool sub_42A010(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42A180
bool sub_42A180(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7393, "sourceObj!= OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj != OBJ_HANDLE_NULL) {
        if (obj_field_int64_get(source_obj, OBJ_F_LOCATION) != obj_field_int64_get(target_obj, OBJ_F_LOCATION)) {
            return false;
        }
    }

    return true;
}

// 0x42A200
bool sub_42A200(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 7416, "obj != OBJ_HANDLE_NULL"

    sub_456FA0(run_info->params[1].data, 0x1);

    return true;
}

// 0x42A260
bool sub_42A260(AnimRunInfo* run_info)
{
    sub_457000(run_info->params[0].data, 3);

    return true;
}

// 0x42A280
bool sub_42A280(AnimRunInfo* run_info)
{
    sub_457000(run_info->params[0].data, 4);

    return true;
}

// 0x42A2A0
bool sub_42A2A0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    Tanya v1;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7461, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 7462, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(target_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if (!sub_4B7CD0(source_obj, 4)) {
        return false;
    }

    sub_4C7090(&v1);

    v1.field_9C = run_info->cur_stack_data->params[AGDATA_SKILL_DATA].data;

    if (target_obj == -1) {
        target_obj = OBJ_HANDLE_NULL;
    }

    sub_4440E0(source_obj, &(v1.field_0));
    sub_4440E0(target_obj, &(v1.field_30));

    if (run_info->cur_stack_data->params[AGDATA_SCRATCH_OBJ].obj != -1) {
        sub_4440E0(run_info->cur_stack_data->params[AGDATA_SCRATCH_OBJ].obj, &(v1.field_68));
    }

    if ((run_info->cur_stack_data->params[AGDATA_FLAGS_DATA].data & 0x1) != 0) {
        v1.field_98 |= 0x2;
    }

    sub_4C7160(&v1);

    if ((v1.field_98 & 0x1) != 0) {
        run_info->field_C |= 0x40000;
    }

    return true;
}

// 0x42A430
bool sub_42A430(AnimRunInfo* run_info)
{
    return (run_info->field_C & 40000) != 0;
}

// 0x42A440
bool sub_42A440(AnimRunInfo* run_info)
{
    int64_t source_obj;

    source_obj = run_info->params[0].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7519, "sourceObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    return run_info->params[1].data == 5;
}

// 0x42A490
bool sub_42A490(AnimRunInfo* run_info)
{
    int64_t source_obj;

    source_obj = run_info->params[0].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7540, "sourceObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    return run_info->params[1].data == 14;
}

// 0x42A4E0
bool sub_42A4E0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    Tanya v1;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7563, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 7564, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    sub_4C7090(&v1);

    v1.field_9C = run_info->cur_stack_data->params[AGDATA_SKILL_DATA].data;

    if (target_obj == -1) {
        target_obj = OBJ_HANDLE_NULL;
    }

    sub_4440E0(source_obj, &(v1.field_0));
    sub_4440E0(target_obj, &(v1.field_30));

    if (run_info->cur_stack_data->params[AGDATA_SCRATCH_OBJ].obj != -1) {
        sub_4440E0(run_info->cur_stack_data->params[AGDATA_SCRATCH_OBJ].obj, &(v1.field_68));
    }

    if ((run_info->cur_stack_data->params[AGDATA_FLAGS_DATA].data & 0x1) != 0) {
        v1.field_98 |= 0x2;
    }

    sub_4C7160(&v1);

    return true;
}

// 0x42A630
bool sub_42A630(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    int hit_loc;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;
    hit_loc = run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL3].data;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7694, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 7605, "targetObj != OBJ_HANDLE_NULL"
    ASSERT(hit_loc != -1); // 7606, "hitLoc != -1"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (hit_loc == -1) {
        return false;
    }

    if ((obj_field_int32_get(target_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    sub_4B3BB0(source_obj, target_obj, hit_loc);

    return true;
}

// 0x42A720
bool sub_42A720(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42A930
bool sub_42A930(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t weapon_obj;

    source_obj = run_info->params[0].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7752, "sourceObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(source_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    weapon_obj = sub_4B23B0(source_obj);
    return item_weapon_range(weapon_obj, source_obj) > 1;
}

// 0x42A9B0
bool sub_42A9B0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7778, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 7779, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(source_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if (!sub_42AA70(source_obj, target_obj)) {
        return false;
    }

    return true;
}

// 0x42AA70
bool sub_42AA70(int64_t source_obj, int64_t target_obj)
{
    int64_t parent_obj;
    int sound_id;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7803, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 7804, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_4681A0()) {
        return false;
    }

    if (item_parent(target_obj, &parent_obj)) {
        if (parent_obj == OBJ_HANDLE_NULL || sub_49B290(parent_obj) != 3023) {
            return false;
        }
    }

    if (!sub_4B7CD0(source_obj, 1)) {
        return false;
    }

    if (player_is_pc_obj(source_obj)) {
        sound_id = sub_4F0BF0(target_obj, source_obj, OBJ_HANDLE_NULL, 0);
        sub_41B930(sound_id, 1, source_obj);
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        return sub_4617F0(target_obj, source_obj);
    }

    return true;
}

// 0x42AB90
bool sub_42AB90(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42ACD0
bool sub_42ACD0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    int v1;
    int64_t source_loc;
    int64_t target_loc;
    int target_type;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7905, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 7906, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(target_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if ((obj_field_int32_get(source_obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    if (obj_type_is_critter(obj_field_int32_get(source_obj, OBJ_F_TYPE))) {
        if ((obj_field_int32_get(source_obj, OBJ_F_CRITTER_FLAGS) & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
            return false;
        }
    }

    if ((obj_field_int32_get(target_obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    target_type = obj_field_int32_get(target_obj, OBJ_F_TYPE);
    if (obj_type_is_critter(target_type)) {
        if ((obj_field_int32_get(target_obj, OBJ_F_CRITTER_FLAGS) & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
            return false;
        }
    }

    if (!obj_type_is_critter(target_type)) {
        return false;
    }

    return true;
}

// 0x42AE10
bool sub_42AE10(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    int v1;
    int64_t source_loc;
    int64_t target_loc;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7963, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 7964, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(target_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    v1 = sub_4AFBB0(source_obj);
    target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    source_loc = obj_field_int64_get(source_obj, OBJ_F_LOCATION);
    return sub_4B96F0(source_loc, target_loc) < v1;
}

// 0x42AF00
bool sub_42AF00(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 7997, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 7998, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(target_obj, OBJ_F_FLAGS) & (OF_DESTROYED | OF_OFF)) != 0) {
        return false;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        sub_460410(source_obj, target_obj);
    }

    return false;
}

// 0x42AFB0
bool sub_42AFB0(AnimRunInfo* run_info)
{
    int64_t obj;
    int type;
    tig_art_id_t art_id;
    int64_t leader_obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 8028, "obj != OBJ_HANDLE_NULL"

    if ((run_info->field_C & 0x8000) != 0) {
        return false;
    }

    if (map_is_clearing_objects()) {
        return false;
    }

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    type = obj_field_int32_get(obj, OBJ_F_TYPE);

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    art_id = sub_503E50(art_id, 0);
    art_id = tig_art_id_frame_set(art_id, 0);
    object_set_current_aid(obj, art_id);

    if (obj_type_is_critter(type)) {
        sub_430490(obj, 0, 0);

        leader_obj = critter_leader_get(obj);
        if (leader_obj == OBJ_HANDLE_NULL
            || !critter_is_concealed(leader_obj)) {
            sub_45EE30(obj, false);
        }
    }

    return true;
}

// 0x42B090
bool sub_42B090(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42B250
bool sub_42B250(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42B440
bool sub_42B440(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42B640
bool sub_42B640(AnimRunInfo* run_info)
{
    int64_t door_obj;

    door_obj = run_info->params[0].obj;

    ASSERT(door_obj != OBJ_HANDLE_NULL); // 8351, "doorObj != OBJ_HANDLE_NULL"

    if ((run_info->field_C & 0x8000) != 0) {
        return false;
    }

    if (map_is_clearing_objects()) {
        return false;
    }

    if (door_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (obj_field_int32_get(door_obj, OBJ_F_TYPE) != OBJ_TYPE_PORTAL) {
        return false;
    }

    if (sub_4F08C0(door_obj)) {
        return true;
    }

    if ((obj_field_int32_get(door_obj, OBJ_F_PORTAL_FLAGS) & OPF_MAGICALLY_HELD) != 0) {
        return false;
    }

    // NOTE: Why twice?
    sub_4F08F0(door_obj);
    sub_4F08F0(door_obj);

    return true;
}

// 0x42B6F0
bool sub_42B6F0(AnimRunInfo* run_info)
{
    int64_t door_obj;

    door_obj = run_info->params[0].obj;

    ASSERT(door_obj != OBJ_HANDLE_NULL); // 8386, "doorObj != OBJ_HANDLE_NULL"

    if ((run_info->field_C & 0x8000) != 0) {
        return false;
    }

    if (map_is_clearing_objects()) {
        return false;
    }

    if (door_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (obj_field_int32_get(door_obj, OBJ_F_TYPE) != OBJ_TYPE_PORTAL) {
        return false;
    }

    if (!sub_4F08C0(door_obj)) {
        return true;
    }

    if ((obj_field_int32_get(door_obj, OBJ_F_PORTAL_FLAGS) & OPF_MAGICALLY_HELD) != 0) {
        return false;
    }

    sub_4F08F0(door_obj);

    return true;
}

// 0x42B790
bool sub_42B790(AnimRunInfo* run_info)
{
    int64_t door_obj;

    door_obj = run_info->params[0].obj;

    ASSERT(door_obj != OBJ_HANDLE_NULL); // 8418, "doorObj != OBJ_HANDLE_NULL"

    if (door_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    return (obj_field_int32_get(door_obj, OBJ_F_PORTAL_FLAGS) & OPF_STICKY) == 0;
}

// 0x42B7F0
bool sub_42B7F0(AnimRunInfo* run_info)
{
    int64_t door_obj;
    ObjectList objects;
    ObjectNode* node;

    door_obj = run_info->params[0].obj;

    ASSERT(door_obj != OBJ_HANDLE_NULL); // 8438, "doorObj != OBJ_HANDLE_NULL"

    if (door_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (sub_4B6D70()) {
        return false;
    }

    sub_4407C0(obj_field_int64_get(door_obj, OBJ_F_LOCATION),
        OBJ_TM_NPC | OBJ_TM_PC,
        &objects);
    node = objects.head;
    while (node != NULL) {
        if (!sub_45D8D0(node->obj)) {
            object_list_destroy(&objects);
            return false;
        }
        node = node->next;
    }

    object_list_destroy(&objects);
    return true;
}

// 0x42B8B0
void sub_42B8B0()
{
    // TODO: Incomplete.
}

// 0x42B940
bool sub_42B940(AnimRunInfo* run_info)
{
    ASSERT(run_info != OBJ_HANDLE_NULL); // 8503, "pRunInfo != NULL"
    ASSERT(run_info->field_20); // 8504, "pRunInfo->animObj != OBJ_HANDLE_NULL"

    if (sub_45F790(run_info->field_20) >= ENCUMBRANCE_LEVEL_SIGNIFICANT) {
        return false;
    }

    run_info->field_C |= 0x40;

    return true;
}

// 0x42B9C0
bool sub_42B9C0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    int64_t leader_obj;
    tig_art_id_t art_id;
    unsigned int flags;
    int64_t weapon_obj;
    int v1;
    int v2;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 8525, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 8526, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    combat_critter_activate_combat_mode(source_obj);

    if (sub_40DA20(target_obj)) {
        combat_critter_activate_combat_mode(target_obj);
    } else {
        leader_obj = sub_45DDA0(target_obj);
        if (leader_obj == OBJ_HANDLE_NULL && !sub_45D8D0(leader_obj)) {
            combat_critter_activate_combat_mode(leader_obj);
        }
    }

    art_id = obj_field_int32_get(target_obj, OBJ_F_CURRENT_AID);
    v2 = sub_503E20(art_id);

    flags = obj_field_int32_get(target_obj, OBJ_F_FLAGS);
    weapon_obj = item_wield_get(source_obj, 1004);
    if (weapon_obj != OBJ_HANDLE_NULL
        && (obj_field_int32_get(weapon_obj, OBJ_F_WEAPON_FLAGS) & OWF_BOOMERANGS) != 0) {
        v1 = 14;
    } else if ((flags & OF_SHRUNK) != 0) {
        v1 = 21;
    } else {
        switch (v2) {
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            v1 = 21;
            break;
        default:
            v1 = sub_503F60(art_id) != 0 ? 20 : 21;
            break;
        }
    }

    if (!combat_critter_is_combat_mode_active(source_obj)) {
        return false;
    }

    art_id = obj_field_int32_get(source_obj, OBJ_F_CURRENT_AID);
    art_id = sub_503E50(art_id, v2);
    art_id = tig_art_id_frame_set(art_id, 0);
    if (tig_art_exists(art_id) != TIG_OK) {
        // FIXME: Unused.
        tig_art_exists(art_id);

        tig_debug_printf("Anim: ERROR: Missing art: %u, List: %d,",
            art_id,
            tig_art_type(art_id));
        tig_debug_printf(" Num: %d, Anim: %d,",
            tig_art_num_get(art_id),
            sub_503E20(art_id));
        tig_debug_printf(" Weapon: %d, Specie: %d!\n",
            sub_5040D0(art_id),
            sub_503F20(art_id));

        return false;
    }

    object_set_current_aid(source_obj, art_id);
    run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = art_id;

    return true;
}

// 0x42BC10
bool sub_42BC10(AnimRunInfo* run_info)
{
    int64_t obj;
    int v1;
    int64_t pc_obj;
    tig_art_id_t art_id;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 8623, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        tig_debug_printf("Anim: AGsetObjArtDeathAnim: Failed to set art to Death art!!!\n");
        return false;
    }

    v1 = run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL1].data;

    // FIXME: Meaningless.
    pc_obj = player_get_pc_obj();
    obj_field_int32_get(pc_obj, OBJ_F_PC_FLAGS);

    if (violence_filter != 0) {
        v1 = 7;
    }

    combat_critter_deactivate_combat_mode(obj);

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    art_id = sub_503E50(art_id, v1);
    art_id = tig_art_id_frame_set(art_id, 0);
    if (tig_art_exists(art_id) != TIG_OK) {
        if (violence_filter != 0) {
            ASSERT(0); // 8647, "0"
            return false;
        }

        art_id = sub_503E50(art_id, 18);
        tig_debug_printf("Anim: Error: Missing Death Art: %d: Using Default\n", v1);
    }

    if (tig_art_exists(art_id) != TIG_OK) {
        tig_debug_printf("Anim: AGsetObjArtDeathAnim: Failed to set art to Death art!!!\n");
        return false;
    }

    object_set_current_aid(obj, art_id);
    run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = art_id;

    return true;
}

// 0x42BD40
bool sub_42BD40(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 8676, "obj != OBJ_HANDLE_NULL"
    ASSERT(run_info->params[2].data != -1); // 8677, "pRunInfo->params[2].dataVal != -1"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    art_id = sub_503E50(art_id, run_info->params[2].data);
    art_id = tig_art_id_frame_set(art_id, 0);
    if (tig_art_exists(art_id) != TIG_OK) {
        return false;
    }

    object_set_current_aid(obj, art_id);

    run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = art_id;
    if (tig_art_anim_data(art_id, &art_anim_data) == TIG_OK) {
        run_info->field_CFC = 1000 / art_anim_data.fps;
    } else {
        tig_debug_printf("Anim: AGsetObjArtAnim: Failed to find Aid: %d, defaulting to 10 fps!", art_id);
        run_info->field_CFC = 100;
    }
}

// 0x42BE50
bool sub_42BE50(AnimRunInfo* run_info)
{
    run_info->cur_stack_data->params[AGDATA_SCRATCH_OBJ].obj = run_info->params[0].obj;

    return true;
}

// 0x42BE80
bool sub_42BE80(AnimRunInfo* run_info)
{
    run_info->cur_stack_data->params[AGDATA_SKILL_DATA].data = run_info->params[2].data;

    return true;
}

// 0x42BEA0
bool sub_42BEA0(AnimRunInfo* run_info)
{
    run_info->cur_stack_data->params[AGDATA_RANGE_DATA].data = run_info->params[2].data;

    return true;
}

// 0x42BEC0
bool sub_42BEC0(AnimRunInfo* run_info)
{
    int64_t target_obj;
    int64_t target_loc;

    target_obj = run_info->params[0].obj;

    ASSERT(target_obj != OBJ_HANDLE_NULL); // 8742, "targetObj != OBJ_HANDLE_NULL"

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);

    if (run_info->cur_stack_data->params[AGDATA_TARGET_TILE].loc == target_loc) {
        return false;
    }

    run_info->cur_stack_data->params[AGDATA_TARGET_TILE].loc = target_loc;

    run_info->path.flags |= 0x1;

    return true;
}

// 0x42BF40
bool sub_42BF40(AnimRunInfo* run_info)
{
    int64_t obj;
    unsigned int flags;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 8765, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    flags = 0;
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        flags = obj_field_int32_get(obj, OBJ_F_NPC_FLAGS);
    }

    run_info->cur_stack_data->params[AGDATA_RANGE_DATA].data = (flags & ONF_AI_SPREAD_OUT) != 0 ? 5 : 2;

    return true;
}

// 0x42BFD0
bool sub_42BFD0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    unsigned int flags;
    int v2;
    int64_t source_loc;
    int64_t target_loc;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 8792, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 8793, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    flags = 0;
    if (obj_field_int32_get(source_obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        flags = obj_field_int32_get(source_obj, OBJ_F_NPC_FLAGS);
    }

    v2 = (flags & ONF_AI_SPREAD_OUT) != 0 ? 3 : 1;

    source_loc = obj_field_int64_get(source_obj, OBJ_F_LOCATION);
    target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    return sub_4B96F0(source_loc, target_loc) < v2;
}

// 0x42C0F0
bool sub_42C0F0(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    unsigned int flags;
    int v2;
    int64_t source_loc;
    int64_t target_loc;
    int rotation;
    tig_art_id_t art_id;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 8832, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 8833, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(source_obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    if (obj_type_is_critter(obj_field_int32_get(source_obj, OBJ_F_TYPE))) {
        if ((obj_field_int32_get(source_obj, OBJ_F_CRITTER_FLAGS) & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
            return false;
        }
    }

    target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    source_loc = obj_field_int64_get(source_obj, OBJ_F_LOCATION);
    rotation = sub_4B8D50(source_loc, target_loc);

    art_id = obj_field_int32_get(source_obj, OBJ_F_CURRENT_AID);
    art_id = tig_art_id_rotation_set(art_id, rotation);
    if (tig_art_exists(art_id) == TIG_OK) {
        object_set_current_aid(source_obj, art_id);
        run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = art_id;
    }

    return true;
}

// 0x42C240
bool sub_42C240(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t target_obj;
    int64_t source_loc;
    int64_t target_loc;
    int rotation;
    tig_art_id_t art_id;

    source_obj = run_info->params[0].obj;
    target_obj = run_info->params[1].obj;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 8832, "sourceObj != OBJ_HANDLE_NULL"
    ASSERT(target_obj != OBJ_HANDLE_NULL); // 8833, "targetObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (target_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(source_obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    if (obj_type_is_critter(obj_field_int32_get(source_obj, OBJ_F_TYPE))) {
        if ((obj_field_int32_get(source_obj, OBJ_F_CRITTER_FLAGS) & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
            return false;
        }
    }

    source_loc = obj_field_int64_get(source_obj, OBJ_F_LOCATION);
    target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    rotation = sub_4B8D50(target_loc, source_loc);
    art_id = obj_field_int32_get(source_obj, OBJ_F_CURRENT_AID);
    art_id = tig_art_id_rotation_set(art_id, rotation);
    if (tig_art_exists(art_id) == TIG_OK) {
        object_set_current_aid(source_obj, art_id);
        run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = art_id;
    }

    return true;
}

// 0x42C390
bool sub_42C390(AnimRunInfo* run_info)
{
    int64_t source_obj;
    int64_t source_loc;
    int64_t target_loc;
    int rotation;
    tig_art_id_t art_id;

    source_obj = run_info->params[0].obj;
    target_loc = run_info->params[1].loc;

    ASSERT(source_obj != OBJ_HANDLE_NULL); // 8927, "sourceObj != OBJ_HANDLE_NULL"

    if (source_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    source_loc = obj_field_int64_get(source_obj, OBJ_F_LOCATION);
    rotation = sub_4B8D50(source_loc, target_loc);
    art_id = obj_field_int32_get(source_obj, OBJ_F_CURRENT_AID);
    art_id = tig_art_id_rotation_set(art_id, rotation);
    if (tig_art_exists(art_id) == TIG_OK) {
        object_set_current_aid(source_obj, art_id);
        run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = art_id;
    }

    return true;
}

// 0x42C440
bool sub_42C440(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42C610
bool sub_42C610(AnimRunInfo* run_info)
{
    if (run_info->cur_stack_data == NULL) {
        run_info->cur_stack_data = &(run_info->goals[run_info->current_goal]);
    }

    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL1].data = run_info->path.rotations[0];

    return true;
}

// 0x42C650
bool sub_42C650(AnimRunInfo* run_info)
{
    int64_t obj;
    int new_rot;
    tig_art_id_t art_id;

    obj = run_info->params[0].obj;
    new_rot = run_info->params[1].data;

    ASSERT(obj != OBJ_HANDLE_NULL); // 9055, "obj != OBJ_HANDLE_NULL"
    ASSERT(new_rot >= 0); // 9056, "newRot >= 0"
    ASSERT(new_rot < TIG_ART_ARMOR_TYPE_PLATE_CLASSIC); // 9057, "newRot < tig_art_rotation_max"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
            return false;
        }
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    if (tig_art_id_rotation_get(art_id) != new_rot) {
        art_id = tig_art_id_rotation_set(art_id, new_rot);
        if (tig_art_exists(art_id) == TIG_OK) {
            object_set_current_aid(obj, art_id);
            run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = art_id;
        }
    }

    return true;
}

// 0x42C780
bool sub_42C780(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 9099, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
            return false;
        }
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    art_id = tig_art_id_rotation_set(art_id, run_info->path.rotations[run_info->path.curr]);
    if (tig_art_exists(art_id) == TIG_OK) {
        object_set_current_aid(obj, art_id);
        run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = art_id;
    }

    return true;
}

// 0x42C850
bool sub_42C850(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42CA90
bool sub_42CA90(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42CAA0
bool sub_42CAA0(AnimRunInfo* run_info)
{
    if ((run_info->field_C & 0x4) == 0) {
        return false;
    }

    if ((run_info->field_C & 0x8) != 0) {
        return false;
    }

    run_info->field_C |= 0x8;

    return true;
}

// 0x42CAC0
bool sub_42CAC0(AnimRunInfo* run_info)
{
    AnimGoalData* goal_data;
    int64_t self_obj;
    int64_t parent_obj;

    goal_data = run_info->cur_stack_data;
    if (goal_data != NULL) {
        self_obj = goal_data->params[AGDATA_SELF_OBJ].obj;
        parent_obj = goal_data->params[AGDATA_PARENT_OBJ].obj;
        if (self_obj != OBJ_HANDLE_NULL
            && (obj_field_int32_get(self_obj, OBJ_F_FLAGS) & OF_DESTROYED) == 0) {
            sub_4B2650(self_obj, parent_obj, NULL);
        }
    }

    return true;
}

// 0x42CB10
bool sub_42CB10(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42CC80
bool sub_42CC80(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42CDF0
bool sub_42CDF0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42CF40
bool sub_42CF40(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42D080
bool sub_42D080(AnimRunInfo* run_info)
{
    int64_t door_obj;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    door_obj = run_info->params[0].obj;
    ASSERT(door_obj != OBJ_HANDLE_NULL); // 9543, doorObj != OBJ_HANDLE_NULL
    if (door_obj == OBJ_HANDLE_NULL) {
        tig_debug_printf("Anim: AGbeginAnimOpenDoor: Warning: Goal Received NULL Object!\n");
        return false;
    }

    art_id = obj_field_int32_get(door_obj, OBJ_F_CURRENT_AID);
    if (!sub_4F0950(door_obj)) {
        return false;
    }

    if (tig_art_anim_data(art_id, &art_anim_data) == TIG_OK) {
        run_info->field_CFC = 1000 / art_anim_data.fps;
    } else {
        tig_debug_printf("Anim: AGbeginAnimOpenDoor: Failed to find Aid: %d, defaulting to 10 fps!\n", art_id);
        run_info->field_CFC = 100;
    }

    run_info->field_C &= ~0xC;
    sub_41B930(sub_4F1010(door_obj, 0), 1, door_obj);
    run_info->field_C |= 0x10;

    return true;
}

// 0x42D160
bool sub_42D160(AnimRunInfo* run_info)
{
    int64_t door_obj;

    door_obj = run_info->params[0].obj;
    ASSERT(door_obj != OBJ_HANDLE_NULL); // 9580, doorObj != OBJ_HANDLE_NULL
    if (door_obj == OBJ_HANDLE_NULL) {
        tig_debug_printf("Anim: AGupdateAnimOpenDoor: Warning: Goal Received NULL Object!\n");
        return false;
    }

    return sub_4F0950(door_obj);
}

// 0x42D1C0
bool sub_42D1C0(AnimRunInfo* run_info)
{
    int64_t door_obj;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    door_obj = run_info->params[0].obj;
    ASSERT(door_obj != OBJ_HANDLE_NULL); // 9603, doorObj != OBJ_HANDLE_NULL
    if (door_obj == OBJ_HANDLE_NULL) {
        tig_debug_printf("Anim: AGbeginAnimOpenDoor: Warning: Goal Received NULL Object!\n");
        return false;
    }

    art_id = obj_field_int32_get(door_obj, OBJ_F_CURRENT_AID);
    if (!sub_4F0A90(door_obj)) {
        return false;
    }

    if (tig_art_anim_data(art_id, &art_anim_data) == TIG_OK) {
        run_info->field_CFC = 1000 / art_anim_data.fps;
    } else {
        tig_debug_printf("Anim: AGbeginAnimOpenDoor: Failed to find Aid: %d, defaulting to 10 fps!\n", art_id);
        run_info->field_CFC = 100;
    }

    run_info->field_C &= ~0xC;
    sub_41B930(sub_4F1010(door_obj, 1), 1, door_obj);
    run_info->field_C |= 0x10;

    return true;
}

// 0x42D2A0
bool sub_42D2A0(AnimRunInfo* run_info)
{
    int64_t door_obj;

    door_obj = run_info->params[0].obj;
    ASSERT(door_obj != OBJ_HANDLE_NULL); // 9640, doorObj != OBJ_HANDLE_NULL
    if (door_obj == OBJ_HANDLE_NULL) {
        tig_debug_printf("Anim: AGupdateAnimOpenDoor: Warning: Goal Received NULL Object!\n");
        return false;
    }

    return sub_4F0A90(door_obj);
}

// 0x42D300
bool sub_42D300(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42D440
bool sub_42D440(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42D570
bool sub_42D570(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42D6F0
bool sub_42D6F0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42D7D0
bool sub_42D7D0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42D910
bool sub_42D910(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42DA50
bool sub_42DA50(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42DCF0
bool sub_42DCF0(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 10145, obj != OBJ_HANDLE_NULL
    if (obj == OBJ_HANDLE_NULL) {
        tig_debug_printf("Anim: Warning: Goal Received NULL Object!\n");
        run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL4].data = 0;
        return false;
    }

    if ((obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data++;
    if (run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data < 3) {
        return run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL4].data;
    }

    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL5].data = 0;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        if (sub_441AE0(obj, player_get_pc_obj()) >= 30) {
            run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL4].data = 0;
            return false;
        }
    }

    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL4].data = 1;
    return true;
}

// 0x42DDE0
bool sub_42DDE0(AnimRunInfo* run_info)
{
    int64_t obj;
    int obj_type;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;
    int frame;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 10193, obj != OBJ_HANDLE_NULL
    if (obj == OBJ_HANDLE_NULL) return false;

    if ((obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    // FIXME: Unused.
    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);

    object_inc_current_aid(obj);

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    if (tig_art_anim_data(art_id, &art_anim_data) == TIG_OK) {
        frame = tig_art_id_frame_get(art_id);
        if (frame == art_anim_data.num_frames - 1) {
            run_info->field_C &= ~0x4;
        }
        if (frame == art_anim_data.action_frame - 1) {
            run_info->field_C |= 0x4;
        }
        return frame % 3 != 1;
    } else {
        run_info->field_C &= ~0x10;
        return false;
    }
}

// 0x42DED0
bool sub_42DED0(AnimRunInfo* run_info)
{
    ASSERT(run_info != NULL); // 10259, pRunInfo != NULL

    if (run_info->cur_stack_data->params[AGDATA_SOUND_HANDLE].data != TIG_SOUND_HANDLE_INVALID
        && (run_info->field_C & 0x20000) == 0) {
        tig_sound_destroy(run_info->cur_stack_data->params[AGDATA_SOUND_HANDLE].data);
        run_info->cur_stack_data->params[AGDATA_SOUND_HANDLE].data = TIG_SOUND_HANDLE_INVALID;
    }

    return true;
}

// 0x42DF40
bool AGbeginStunAnim(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 10292, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    art_id = sub_503E50(art_id, 23);
    art_id = tig_art_id_frame_set(art_id, 0);
    object_set_current_aid(obj, art_id);

    if ((obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0) {
        return false;
    }

    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_PARALYZED) != 0) {
            switch (sub_503E20(art_id)) {
            case 17:
            case 18:
            case 19:
                break;
            default:
                return false;
            }
        }
    }

    // FIXME: Set one more time?
    object_set_current_aid(obj, art_id);

    if (tig_art_anim_data(art_id, &art_anim_data) != TIG_OK) {
        tig_debug_printf("Anim: AGbeginStunAnim: Failed to find Aid: %d, defaulting to 10 fps!", art_id);
        return false;
    }

    run_info->field_CFC = 1000 / art_anim_data.fps;
    run_info->field_C |= 0x10;

    return true;
}

// 0x42E070
bool sub_42E070(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42E1B0
bool sub_42E1B0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42E2D0
bool sub_42E2D0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42E460
bool sub_42E460(AnimRunInfo* run_info)
{
    int64_t self_obj;

    self_obj = run_info->params[0].obj;

    ASSERT(self_obj != OBJ_HANDLE_NULL); // 10534, "selfObj != OBJ_HANDLE_NULL"
    if (self_obj == OBJ_HANDLE_NULL) return false;

    return !sub_45D800(self_obj);
}

// 0x42E4B0
bool AGbeginKnockDownAnim(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 10553, "obj != OBJ_HANDLE_NULL"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    art_id = sub_503E50(art_id, 7);
    art_id = tig_art_id_frame_set(art_id, 0);
    object_set_current_aid(obj, art_id);

    run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = art_id;
    if (tig_art_anim_data(art_id, &art_anim_data) == TIG_OK) {
        run_info->field_CFC = 1000 / art_anim_data.fps;
    } else {
        tig_debug_printf("Anim: AGbeginKnockDownAnim: Failed to find Aid: %d, defaulting to 10 fps!", art_id);
        run_info->field_CFC = 100;
    }

    run_info->field_C |= 0x10;

    return true;
}

// 0x42E590
bool sub_42E590(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42E6B0
bool sub_42E6B0(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 10638, obj != OBJ_HANDLE_NULL
    if (obj == OBJ_HANDLE_NULL) return false;

    if (!sub_45D790(obj)) {
        return false;
    }

    run_info->cur_stack_data->params[AGDATA_ANIM_ID].data = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);

    return true;
}

// 0x42E720
bool sub_42E720(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42E8B0
bool sub_42E8B0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42E9B0
bool sub_42E9B0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42EDC0
void sub_42EDC0()
{
    // TODO: Incomplete.
}

// 0x42EE90
void sub_42EE90()
{
    // TODO: Incomplete.
}

// 0x42EF60
void sub_42EF60()
{
    // TODO: Incomplete.
}

// 0x42F000
bool sub_42F000(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42F140
bool sub_42F140(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42F2D0
bool sub_42F2D0(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 11351, obj != OBJ_HANDLE_NULL
    if (obj == OBJ_HANDLE_NULL) return false;

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    if (tig_art_anim_data(art_id, &art_anim_data) == TIG_OK) {
        run_info->field_CFC = 1000 / art_anim_data.fps;
    } else {
        tig_debug_printf("Anim: AGbeginAnimMoveStraight: Failed to find Aid: %d, defaulting to 10 fps!", art_id);
        run_info->field_CFC = 100;
    }

    run_info->path.flags &= ~0x1;
    run_info->path.curr = 0;
    run_info->field_C |= 0x10;

    return true;
}

// 0x42F390
bool sub_42F390(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42F5C0
bool sub_42F5C0(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 11453, obj != OBJ_HANDLE_NULL
    if (obj == OBJ_HANDLE_NULL) return false;

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    if (tig_art_anim_data(art_id, &art_anim_data) == TIG_OK) {
        run_info->field_CFC = 1000 / art_anim_data.fps;
    } else {
        tig_debug_printf("Anim: AGbeginAnimMoveStraight: Failed to find Aid: %d, defaulting to 10 fps!", art_id);
        run_info->field_CFC = 100;
    }

    run_info->path.flags &= ~0x1;
    run_info->path.curr = 0;
    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL3].data = 0;
    run_info->cur_stack_data->params[AGDATA_ORIGINAL_TILE].loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
    run_info->field_C |= 0x10;

    return true;
}

// 0x42F6A0
bool sub_42F6A0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42FA50
bool sub_42FA50(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x42FD70
void sub_42FD70()
{
    // TODO: Incomplete.
}

// 0x42FEA0
bool sub_42FEA0(AnimRunInfo* run_info)
{
    (void)run_info;

    return true;
}

// 0x42FEB0
bool sub_42FEB0(AnimRunInfo* run_info)
{
    (void)run_info;

    return false;
}

// 0x42FEC0
bool sub_42FEC0(AnimRunInfo* run_info)
{
    (void)run_info;

    return false;
}

// 0x42FED0
bool sub_42FED0(AnimRunInfo* run_info)
{
    int64_t obj;
    int64_t location;

    obj = run_info->params[0].obj;
    if (obj == OBJ_HANDLE_NULL) {
        ASSERT(obj != OBJ_HANDLE_NULL); // obj != OBJ_HANDLE_NULL
        return false;
    }

    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    if (!sub_4D7110(location, false)) {
        sub_432D90(location);
    }

    return true;
}

// 0x42FF40
bool sub_42FF40(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;
    if (obj == OBJ_HANDLE_NULL) {
        ASSERT(obj != OBJ_HANDLE_NULL); // obj != OBJ_HANDLE_NULL
        return false;
    }

    if ((run_info->field_C & 0x200) == 0 && sub_45D8D0(obj)) {
        sub_43D0E0(obj, 0x404);

        if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
            if (player_is_pc_obj(obj)) {
                sub_460520();
            }
        }

        sub_4CBC60(0, obj);
    }

    return true;
}

// 0x42FFE0
bool sub_42FFE0(AnimRunInfo* run_info)
{
    int64_t obj;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;
    int64_t location;

    obj = run_info->params[0].obj;
    ASSERT(obj != OBJ_HANDLE_NULL); // obj != OBJ_HANDLE_NULL

    if ((run_info->field_C & 0x8000) != 0) {
        return false;
    }

    if (map_is_clearing_objects()) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    if (tig_art_anim_data(art_id, &art_anim_data) != TIG_OK) {
        return false;
    }

    if (tig_art_id_frame_get(art_id) != art_anim_data.num_frames - 1) {
        art_id = tig_art_id_frame_set(art_id, art_anim_data.num_frames - 1);
        object_set_current_aid(obj, art_id);
    }

    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    if (sub_4D7110(location, false)) {
        sub_43CCA0(obj);
    }

    return true;
}

// 0x4300D0
bool anim_fidget_timeevent_process(TimeEvent* timeevent)
{
    // TODO: Incomplete.
}

// 0x4302D0
int sub_4302D0(LocRect* loc_rect, ObjectList* objects)
{
    ObjectNode* node;
    int cnt = 0;

    ASSERT(loc_rect != NULL); // 11919, "pLocRect != NULL"
    ASSERT(objects != NULL); // 11920, "pObjList != NULL"

    sub_440B40(loc_rect, OBJ_TM_PC | OBJ_TM_NPC, objects);
    node = objects->head;
    while (node != NULL) {
        if (sub_4303D0(node->obj)) {
            cnt++;
            node = node->next;
            continue;
        }

        if (!sub_441140(objects, node->obj)) {
            ASSERT(0); // 11939, "0"
            object_list_destroy(objects);
            return false;
        }

        node = objects->head;
        cnt = 0;
    }

    return cnt;
}

// 0x4303D0
bool sub_4303D0(int64_t obj)
{
    if (!sub_423300(obj, 0)
        && !combat_critter_is_combat_mode_active(obj)
        && sub_45D790(obj)) {
        if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
            || !player_is_pc_obj(sub_4C1110(obj))) {
            if (!sub_503E20(obj_field_int32_get(obj, OBJ_F_CURRENT_AID))) {
                return true;
            }
        }
    }

    return false;
}

// 0x430460
void sub_430460()
{
    DateTime datetime;
    TimeEvent timeevent;

    sub_45A950(&datetime, 4000);
    timeevent.type = TIMEEVENT_TYPE_FIDGET_ANIM;
    sub_45B800(&timeevent, &datetime);
}

// 0x430490
void sub_430490(int64_t obj, int offset_x, int offset_y)
{
    object_set_offset(obj, offset_x, offset_y + dword_5DE6E4);
}

// 0x4304C0
bool get_always_run(int64_t obj)
{
    int client_id;
    bool always_run;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        client_id = sub_4A2B10(obj);
        if (client_id != -1) {
            return (sub_4A55D0(client_id) & 0x100) != 0;
        }
        return false;
    }

    always_run = settings_get_value(&settings, "always run");

    if (player_is_pc_obj(obj)) {
        if (always_run) {
            if (tig_kb_is_key_pressed(DIK_LCONTROL)
                || tig_kb_is_key_pressed(DIK_RCONTROL)) {
                return false;
            }
        } else {
            if (tig_kb_is_key_pressed(DIK_LCONTROL)
                || tig_kb_is_key_pressed(DIK_RCONTROL)
                || tig_kb_get_modifier(DIK_NUMLOCK)) {
                return true;
            }
        }
    }

    return always_run;
}

// 0x430580
void set_always_run(bool value)
{
    int client_id;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        client_id = sub_4A2B10(player_get_pc_obj());
        if (client_id != -1) {
            if (value) {
                sub_4A5510(client_id, 0x100);
            } else {
                sub_4A5570(client_id, 0x100);
            }
        }
    }

    settings_set_value(&settings, "always run", value);
}

// 0x4305D0
bool sub_4305D0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x430F20
bool sub_430F20(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x430FC0
int sub_430FC0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x431130
bool sub_431130(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x431150
bool sub_431150(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x4311F0
bool sub_4311F0(AnimRunInfo* run_info)
{
    int64_t obj;
    int offset_x;
    int offset_y;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 12872, obj != OBJ_HANDLE_NULL
    if (obj == OBJ_HANDLE_NULL) return false;

    run_info->cur_stack_data->params[AGDATA_FLAGS_DATA].data |= ~0x20;
    run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL1].data = 1;
    run_info->field_CFC = 100;
    offset_x = obj_field_int32_get(obj, OBJ_F_OFFSET_X);
    offset_y = obj_field_int32_get(obj, OBJ_F_OFFSET_Y);
    object_set_offset(obj, offset_x, offset_y - 15);

    return true;
}

// 0x431290
bool sub_431290(AnimRunInfo* run_info)
{
    int64_t obj;
    int offset_x;
    int offset_y;

    obj = run_info->params[0].obj;

    ASSERT(obj != OBJ_HANDLE_NULL); // 12885, obj != OBJ_HANDLE_NULL
    if (obj == OBJ_HANDLE_NULL) return false;

    run_info->cur_stack_data->params[AGDATA_FLAGS_DATA].data &= ~0x20;
    offset_x = obj_field_int32_get(obj, OBJ_F_OFFSET_X);
    offset_y = obj_field_int32_get(obj, OBJ_F_OFFSET_Y);
    object_set_offset(obj, offset_x, 15 - 2 * run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL1].data + offset_y);

    return true;
}

// 0x431320
bool sub_431320(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x431340
bool sub_431340(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x431550
void sub_431550(AnimRunInfo* run_info, int64_t obj)
{
    AnimGoalData* goal_data;
    tig_sound_handle_t sound_handle;

    goal_data = run_info->cur_stack_data;
    if (goal_data->params[AGDATA_SOUND_HANDLE].data != TIG_SOUND_HANDLE_INVALID
        && goal_data->params[AGDATA_ANIM_ID_PREVIOUS].data != -1
        && (goal_data->params[AGDATA_FLAGS_DATA].data & 0x80000000) != 0) {
        sound_handle = sub_41B930(goal_data->params[AGDATA_ANIM_ID_PREVIOUS].data, 0, obj);
        if (sound_handle != TIG_SOUND_HANDLE_INVALID) {
            goal_data->params[AGDATA_SOUND_HANDLE].data = sound_handle;
        } else {
            goal_data->params[AGDATA_ANIM_ID_PREVIOUS].data = -1;
        }
    }
}

// 0x4315B0
bool sub_4315B0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x431960
void sub_431960(AnimRunInfo* run_info, int64_t obj)
{
    AnimGoalData* goal_data;
    tig_sound_handle_t sound_handle;
    int loops = 1;

    goal_data = run_info->cur_stack_data;
    if (goal_data->params[AGDATA_SOUND_HANDLE].data == TIG_SOUND_HANDLE_INVALID) {
        if (goal_data->params[AGDATA_ANIM_ID_PREVIOUS].data != -1) {
            if ((goal_data->params[AGDATA_FLAGS_DATA].data & 0x80000000) != 0) {
                loops = 0;
            }

            sound_handle = sub_41B930(goal_data->params[AGDATA_ANIM_ID_PREVIOUS].data, loops, obj);
            if (sound_handle != TIG_SOUND_HANDLE_INVALID) {
                if (loops != 0) {
                    run_info->field_C |= 0x20000;
                } else {
                    goal_data->params[AGDATA_ANIM_ID_PREVIOUS].data = sound_handle;
                }
            } else {
                goal_data->params[AGDATA_SOUND_HANDLE].data = TIG_SOUND_HANDLE_INVALID;
            }
        }
    } else {
        sub_4319F0(run_info, obj);
    }
}

// 0x4319F0
void sub_4319F0(AnimRunInfo* run_info, int64_t obj)
{
    AnimGoalData* goal_data;

    goal_data = run_info->cur_stack_data;
    if (goal_data->params[AGDATA_SOUND_HANDLE].data != TIG_SOUND_HANDLE_INVALID
        && (run_info->field_C & 0x20000) == 0
        && (goal_data->params[AGDATA_FLAGS_DATA].data & 0x80000000) != 0) {
        sub_41C780(goal_data->params[AGDATA_SOUND_HANDLE].data, obj_field_int64_get(obj, OBJ_F_LOCATION));
    }
}

// 0x431A40
bool sub_431A40(AnimRunInfo* run_info)
{
    int64_t obj;
    int overlay_fore;
    int overlay_back;
    int overlay_light;

    obj = run_info->params[0].obj;
    ASSERT(obj != OBJ_HANDLE_NULL); // 13298, "obj != OBJ_HANDLE_NULL"
    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    run_info->cur_stack_data->params[AGDATA_FLAGS_DATA].data &= ~0x40;
    if ((run_info->cur_stack_data->params[AGDATA_FLAGS_DATA].data & 0x80) != 0) {
        return false;
    }

    overlay_fore = run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL1].data;
    overlay_back = run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL2].data;
    overlay_light = run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL3].data;

    if (overlay_fore != -1 || overlay_back != -1) {
        if (overlay_back != -5) {
            if (overlay_fore != -1) {
                sub_43ECF0(obj, OBJ_F_OVERLAY_FORE, overlay_fore, -1);
            }
            if (overlay_back != -1) {
                sub_43ECF0(obj, OBJ_F_OVERLAY_BACK, overlay_back, -1);
            }
        } else {
            sub_43ECF0(obj, OBJ_F_UNDERLAY, overlay_fore, -1);
        }
        if (overlay_light == -1) {
            return true;
        }
    } else {
        if (overlay_light == -1) {
            return false;
        }
    }

    object_set_overlay_light(obj, overlay_light, 0, TIG_ART_ID_INVALID, 0);

    return true;
}

// 0x431B20
bool sub_431B20(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x431C40
bool sub_431C40(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x431E50
bool sub_431E50(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// NOTE: Binary identical to 0x431A40.
//
// 0x4321C0
bool sub_4321C0(AnimRunInfo* run_info)
{
    int64_t obj;
    int overlay_fore;
    int overlay_back;
    int overlay_light;

    obj = run_info->params[0].obj;
    ASSERT(obj != OBJ_HANDLE_NULL); // 13671, "obj != OBJ_HANDLE_NULL"
    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    run_info->cur_stack_data->params[AGDATA_FLAGS_DATA].data &= ~0x40;
    if ((run_info->cur_stack_data->params[AGDATA_FLAGS_DATA].data & 0x80) != 0) {
        return false;
    }

    overlay_fore = run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL1].data;
    overlay_back = run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL2].data;
    overlay_light = run_info->cur_stack_data->params[AGDATA_SCRATCH_VAL3].data;

    if (overlay_fore != -1 || overlay_back != -1) {
        if (overlay_back != -5) {
            if (overlay_fore != -1) {
                sub_43ECF0(obj, OBJ_F_OVERLAY_FORE, overlay_fore, -1);
            }
            if (overlay_back != -1) {
                sub_43ECF0(obj, OBJ_F_OVERLAY_BACK, overlay_back, -1);
            }
        } else {
            sub_43ECF0(obj, OBJ_F_UNDERLAY, overlay_fore, -1);
        }
        if (overlay_light == -1) {
            return true;
        }
    } else {
        if (overlay_light == -1) {
            return false;
        }
    }

    object_set_overlay_light(obj, overlay_light, 0, TIG_ART_ID_INVALID, 0);

    return true;
}

// 0x4322A0
bool sub_4322A0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x4324D0
bool sub_4324D0(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x432700
bool sub_432700(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x432990
bool sub_432990(AnimRunInfo* run_info)
{
    // TODO: Incomplete.
}

// 0x432CF0
bool sub_432CF0(int64_t critter_obj)
{
    int64_t item_obj;
    int ammo_type;
    int qty;
    int consumption;

    item_obj = item_wield_get(critter_obj, 1004);
    if (item_obj == OBJ_HANDLE_NULL) {
        return true;
    }

    ammo_type = item_weapon_ammo_type(item_obj);
    if (ammo_type == 10000) {
        return true;
    }

    qty = item_ammo_quantity_get(critter_obj, ammo_type);
    consumption = obj_field_int32_get(item_obj, OBJ_F_WEAPON_AMMO_CONSUMPTION);
    if (qty >= consumption) {
        return true;
    }

    return false;
}

// 0x432D50
bool sub_432D50(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;
    if (obj == OBJ_HANDLE_NULL) {
        ASSERT(obj != OBJ_HANDLE_NULL); // obj != OBJ_HANDLE_NULL
        return false;
    }

    return true;
}

// 0x432D90
void sub_432D90(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x433020
void sub_433020(int64_t obj, int a2, int a3, CombatContext* combat)
{
    // TODO: Incomplete.
}

// 0x433170
void sub_433170(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x433220
void sub_433220(int64_t obj)
{
    int idx;
    tig_art_id_t art_id;

    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    for (idx = 0; idx < 7; idx++) {
        art_id = sub_407470(obj, OBJ_F_OVERLAY_FORE, idx);
        if (tig_art_num_get(art_id) == 0) {
            sub_43ECF0(obj, OBJ_F_OVERLAY_FORE, idx, TIG_ART_ID_INVALID);
            break;
        }
    }
}

// 0x433270
bool sub_433270(AnimRunInfo* run_info)
{
    int64_t obj;

    obj = run_info->params[0].obj;
    if (obj == OBJ_HANDLE_NULL) {
        ASSERT(obj != OBJ_HANDLE_NULL); // obj != OBJ_HANDLE_NULL
        return false;
    }

    if (player_is_pc_obj(run_info->field_20)) {
        return true;
    }

    return sub_4B7CD0(obj, 2);
}

// 0x4332E0
bool sub_4332E0(int64_t obj, int which_anim)
{
    int obj_type;
    AnimGoalData goal_data;
    tig_art_id_t art_id;

    ASSERT(obj != OBJ_HANDLE_NULL); // 14570, obj != OBJ_HANDLE_NULL
    ASSERT(which_anim >= 0); // 14571, whichAnim >= 0
    ASSERT(which_anim < 26); // 14572, whichAnim < tig_art_anim_max

    if (obj == OBJ_HANDLE_NULL
        || which_anim < 0
        || which_anim >= 26) {
        return false;
    }

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type_is_critter(obj_type)
        && which_anim == 10
        && sub_44D4E0(&goal_data, obj, AG_ANIMATE_KNEEL_MAGIC_HANDS)) {
        sub_44D520(&goal_data, 0);
        return true;
    }

    if (!sub_44D4E0(&goal_data, obj, AG_ANIMATE)) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    goal_data.params[AGDATA_ANIM_ID].data = sub_503E50(art_id, which_anim);
    if (!sub_44D520(&goal_data, 0)) {
        return false;
    }

    return true;
}

// 0x433440
bool sub_433440(int64_t obj, int rotation)
{
    // TODO: Incomplete.
}

// 0x433580
bool sub_433580(int64_t obj)
{
    tig_art_id_t art_id;
    int goal_type;
    AnimGoalData goal_data;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (sub_423300(obj, NULL)) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    goal_type = (obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) & OSCF_IS_FIRE)
        ? AG_ANIMATE_LOOP_FIRE_DMG
        : AG_ANIMATE_LOOP;
    sub_44D4E0(&goal_data, obj, goal_type);
    goal_data.params[AGDATA_ANIM_ID].data = art_id;
    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    anim_run_info[stru_5A1908.slot_num].goals[0].params[AGDATA_SOUND_HANDLE].data = TIG_SOUND_HANDLE_INVALID;

    return true;
}

// 0x433640
bool sub_433640(int64_t a1, int64_t a2)
{
    // TODO: Incomplete.
}

// 0x4339A0
bool sub_4339A0(int64_t obj)
{
    return obj != OBJ_HANDLE_NULL
        && sub_45D790(obj)
        && (!sub_4B6D70() || sub_4B6D80() == obj)
        && (sub_40DA20(obj) || !sub_4C1110(obj));
}

// 0x433A00
void sub_433A00()
{
    // TODO: Incomplete.
}

// 0x433C80
bool sub_433C80(int64_t obj, int64_t loc)
{
    // TODO: Incomplete.
}

// 0x434030
bool sub_434030(int64_t a1, int64_t a2)
{
    AnimRunInfo* run_info;
    AnimGoalData goal_data;

    if (!sub_4339A0(a1)) {
        return false;
    }

    if (!sub_44E830(a1, 4, &stru_5A1908)) {
        sub_44D500(&goal_data, a1, 4);
        goal_data.params[AGDATA_TARGET_TILE].loc = a2;

        if (!sub_424070(a1, 3, false, false)) {
            return false;
        }

        if (!sub_44D520(&goal_data, &stru_5A1908)) {
            return false;
        }

        return true;
    }

    run_info = &(anim_run_info[stru_5A1908.slot_num]);
    run_info->field_C |= 0x40;
    if (run_info->goals[0].params[AGDATA_TARGET_TILE].loc != a2) {
        sub_44D500(&goal_data, a1, 3);
        goal_data.params[AGDATA_TARGET_TILE].loc = a2;
        sub_44DBE0(stru_5A1908, &goal_data);
    }

    return true;
}

// 0x4341C0
bool sub_4341C0(int64_t a1, int64_t a2, int a3)
{
    // TODO: Incomplete.
}

// 0x434400
void sub_434400()
{
    // TODO: Incomplete.
}

// 0x4346A0
void anim_goal_follow_obj(int64_t a1, int64_t a2)
{
    // TODO: Incomplete.
}

// 0x4348E0
bool sub_4348E0(int64_t obj, int action_points)
{
    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_45D790(obj)) {
        return false;
    }

    if (!sub_4B6D70()) {
        return true;
    }

    if (sub_4B6D80() != obj) {
        return false;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return true;
    }

    if (action_points <= combat_get_action_points()) {
        return true;
    }

    if (100 * sub_45D700(obj) / sub_45D670(obj) > 35) {
        return true;
    }

    return false;
}

// 0x434980
bool sub_434980(int64_t obj, int64_t a2)
{
    AnimGoalData goal_data;

    if (!sub_4348E0(obj, 0)) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, obj, AG_FLEE)) {
        return false;
    }

    goal_data.params[AGDATA_TARGET_OBJ].obj = a2;

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    sub_436CB0(stru_5A1908);

    return true;
}

// 0x434AE0
bool sub_434AE0(int64_t attacker_obj, int64_t target_obj)
{
    return sub_434B00(attacker_obj, target_obj, -1);
}

// 0x434B00
bool sub_434B00(int64_t attacker_obj, int64_t target_obj, int a3)
{
    ASSERT(attacker_obj != target_obj); // attackerObj != targetObj

    if (attacker_obj == target_obj) {
        return false;
    }

    // TODO: Incomplete.
}

// 0x434DE0
bool sub_434DE0(int64_t obj)
{
    AnimGoalData goal_data;
    tig_art_id_t art_id;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    if (sub_503E20(art_id) != 7) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, obj, AG_ANIM_GET_UP)) {
        return false;
    }

    goal_data.params[AGDATA_ANIM_ID].data = sub_503E50(art_id, 8);

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    return true;
}

// 0x434E80
void sub_434E80()
{
    // TODO: Incomplete.
}

// 0x434F80
bool sub_434F80(int64_t obj, int64_t a2, int64_t a3)
{
    AnimGoalData goal_data;

    ASSERT(a2 != OBJ_HANDLE_NULL); // itemObj != OBJ_HANDLE_NULL

    if (!sub_4348E0(obj, 0)) {
        return false;
    }

    if (a2 == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_44D500(&goal_data, obj, AG_THROW_ITEM)) {
        return false;
    }

    goal_data.params[AGDATA_TARGET_TILE].loc = a3;
    goal_data.params[AGDATA_SCRATCH_OBJ].obj = a2;

    if (sub_44E6F0(obj, &goal_data)) {
        return false;
    }

    if (!sub_424070(obj, 3, false, false)) {
        return false;
    }

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    return true;
}

// 0x435080
bool sub_435080(int64_t obj, int a2)
{
    AnimGoalData goal_data;

    if (obj != OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, obj, AG_DYING)) {
        return false;
    }

    goal_data.params[AGDATA_SCRATCH_VAL1].data = a2;

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    return true;
}

// 0x4350F0
bool sub_4350F0(int64_t obj, int64_t a2, int64_t a3, int a6, int a7)
{
    AnimGoalData goal_data;

    if (a6 == SKILL_PICK_LOCKS) {
        return sub_436220(obj, a2, a3);
    }

    if (!sub_4348E0(obj, 4)) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, obj, AG_USE_SKILL_ON)) {
        return false;
    }

    goal_data.params[AGDATA_TARGET_OBJ].obj = a2;
    goal_data.params[AGDATA_SCRATCH_OBJ].obj = a3;
    goal_data.params[AGDATA_SKILL_DATA].data = a6;
    goal_data.params[AGDATA_FLAGS_DATA].data |= a7;

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    sub_436ED0(stru_5A1908);

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        if ((tig_net_flags & TIG_NET_CONNECTED) == 0
            && sub_45D700(obj) > 8) {
            turn_on_running(stru_5A1908);
        }
    } else {
        if (get_always_run(obj)) {
            turn_on_running(stru_5A1908);
        }
    }

    return true;
}

// 0x4352C0
bool sub_4352C0(int64_t obj, int64_t a2, int64_t a4, int a5, int a6)
{
    AnimGoalData goal_data;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_4348E0(obj, 4)) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, obj, AG_USE_ITEM_ON_OBJECT_WITH_SKILL)) {
        return false;
    }

    goal_data.params[AGDATA_SCRATCH_OBJ].obj = a2;
    goal_data.params[AGDATA_TARGET_OBJ].obj = a4;
    goal_data.params[AGDATA_SKILL_DATA].data = a5;
    goal_data.params[AGDATA_SCRATCH_VAL4].data = a6;

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    sub_436ED0(stru_5A1908);

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        if ((tig_net_flags & TIG_NET_CONNECTED) == 0
            && sub_45D700(obj) > 8) {
            turn_on_running(stru_5A1908);
        }
    } else {
        if (get_always_run(obj)) {
            turn_on_running(stru_5A1908);
        }
    }

    return true;
}

// 0x435450
bool sub_435450(int64_t obj, int64_t a2, int64_t a3, unsigned int a4)
{
    AnimGoalData goal_data;

    if (a2 == OBJ_HANDLE_NULL) {
        return false;
    }

    if (a3 == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_4348E0(obj, 4)) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, obj, AG_USE_ITEM_ON_OBJECT)) {
        return false;
    }

    goal_data.params[AGDATA_TARGET_OBJ].obj = a2;
    goal_data.params[AGDATA_SCRATCH_OBJ].obj = a3;
    goal_data.params[AGDATA_FLAGS_DATA].data |= a4;

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    sub_436ED0(stru_5A1908);

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        if ((tig_net_flags & TIG_NET_CONNECTED) == 0
            && sub_45D700(obj) > 8) {
            turn_on_running(stru_5A1908);
        }
    } else {
        if (get_always_run(obj)) {
            turn_on_running(stru_5A1908);
        }
    }

    return true;
}

// 0x4355F0
bool sub_4355F0(int64_t obj, int64_t a2, int64_t a3, unsigned int flags)
{
    AnimGoalData goal_data;

    if (a2 == 0) {
        return false;
    }

    if (a3 == 0) {
        return false;
    }

    if (!sub_4348E0(obj, 4)) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, obj, 41)) {
        return false;
    }

    goal_data.params[AGDATA_TARGET_TILE].loc = a2;
    goal_data.params[AGDATA_SCRATCH_OBJ].obj = a3;
    goal_data.params[AGDATA_FLAGS_DATA].data |= flags;

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    return true;
}

// 0x4356C0
bool sub_4356C0(int64_t a1, int64_t a2)
{
    AnimGoalData goal_data;

    if (a1 == OBJ_HANDLE_NULL || a2 == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_4348E0(a1, 4)) {
        return false;
    }

    if (obj_field_int32_get(a1, OBJ_F_TYPE) == OBJ_TYPE_NPC
        && !sub_4681A0()) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, a1, AG_PICKUP_ITEM)) {
        return false;
    }

    goal_data.params[AGDATA_TARGET_OBJ].obj = a2;

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    sub_436ED0(stru_5A1908);

    return true;
}

// 0x4357B0
bool sub_4357B0(int64_t obj)
{
    AnimGoalData goal_data;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_45D790(obj)) {
        return false;
    }

    if (!sub_424070(obj, 4, false, false)) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, obj, AG_ANIMATE_STUNNED)) {
        return false;
    }

    combat_critter_activate_combat_mode(obj);
    sub_4B7C90(obj);

    goal_data.params[AGDATA_SCRATCH_VAL5].data = (20 - stat_level(obj, STAT_CONSTITUTION)) / 2;

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    return true;
}

// 0x435870
bool sub_435870(int64_t source_obj, int64_t missile_obj, tig_art_id_t missile_art_id, int a4, int a5, int64_t target_obj, int64_t target_loc, int64_t weapon_obj)
{
    AnimGoalData goal_data;
    int64_t loc;
    int rotation;
    AnimFxListEntry* v1;
    int projectile_speed;

    if (missile_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_424070(missile_obj, 4, false, true)) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, missile_obj, AG_PROJECTILE)) {
        return false;
    }

    loc = obj_field_int64_get(source_obj, OBJ_F_LOCATION);
    rotation = sub_4B6A00(loc, target_loc);
    sub_4EDCE0(missile_obj, sub_4B6B10(missile_art_id, rotation));

    // FIXME: Useless.
    tig_art_id_rotation_get(obj_field_int32_get(source_obj, OBJ_F_CURRENT_AID));

    goal_data.params[AGDATA_TARGET_OBJ].obj = target_obj;
    goal_data.params[AGDATA_PARENT_OBJ].obj = source_obj;
    goal_data.params[AGDATA_SCRATCH_OBJ].obj = target_obj;
    goal_data.params[AGDATA_TARGET_TILE].loc = target_loc;
    goal_data.params[AGDATA_TARGET_TILE].loc = target_loc;
    goal_data.params[AGDATA_SCRATCH_VAL4].data = missile_art_id;

    if (weapon_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
        && animfx_id_get(&stru_5DE610, 5 * sub_49B290(weapon_obj) - 30143, &v1)) {
        projectile_speed = v1->projectile_speed;
    } else {
        projectile_speed = 0;
    }
    goal_data.params[AGDATA_SCRATCH_VAL5].data = projectile_speed;

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    return true;
}

// 0x435A00
bool sub_435A00(int64_t obj, int64_t a2, int64_t a3)
{
    AnimID anim_id;
    AnimRunInfo* run_info;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (!sub_44E830(obj, 36, &anim_id)) {
        return false;
    }

    run_info = &(anim_run_info[anim_id.slot_num]);
    run_info->goals[0].params[AGDATA_TARGET_TILE].loc = a2;
    run_info->goals[0].params[AGDATA_SCRATCH_OBJ].obj = a3;
    run_info->goals[0].params[AGDATA_TARGET_OBJ].obj = run_info->goals[0].params[AGDATA_PARENT_OBJ].obj;
    run_info->path.flags = 1;
    run_info->field_C &= ~0x10;

    return true;
}

// 0x435A90
bool sub_435A90(int64_t critter_obj)
{
    tig_art_id_t art_id;
    AnimGoalData goal_data;

    if (critter_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    art_id = obj_field_int32_get(critter_obj, OBJ_F_CURRENT_AID);
    if (sub_503E20(art_id) == 7) {
        return false;
    }

    if (sub_45D8D0(art_id)) {
        return false;
    }

    if (!sub_424070(art_id, 5, false, false)) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, critter_obj, 62)) {
        return false;
    }

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    return true;
}

// 0x435B30
bool anim_goal_make_knockdown(int64_t obj)
{
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    if (sub_424070(obj, 4, false, false)) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    art_id = sub_503E50(art_id, 7);
    if (tig_art_anim_data(art_id, &art_anim_data) == TIG_OK) {
        art_id = tig_art_id_frame_set(art_id, art_anim_data.num_frames - 1);
    } else {
        tig_debug_printf("Anim: anim_goal_make_knockdown: Failed to find Aid: %d, defaulting to frame 0!", art_id);
        art_id = tig_art_id_frame_set(art_id, 0);
    }

    object_set_current_aid(obj, art_id);

    return true;
}

// 0x435BD0
bool sub_435BD0(int64_t critter_obj)
{
    int obj_type;
    tig_art_id_t art_id;
    AnimGoalData goal_data;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        return true;
    }

    if (critter_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    obj_type = obj_field_int32_get(critter_obj, OBJ_F_TYPE);
    ASSERT(obj_type_is_critter(obj_type)); // 16405, obj_type_is_critter(objType)
    if (obj_type != OBJ_TYPE_NPC) return false;

    if (!sub_4348E0(critter_obj, 0)) {
        return false;
    }

    if (sub_4234F0(critter_obj) || sub_423300(critter_obj, NULL)) {
        return false;
    }

    art_id = obj_field_int32_get(critter_obj, OBJ_F_CURRENT_AID);
    if (sub_503E20(art_id)) {
        return false;
    }

    if (!sub_44D4E0(&goal_data, critter_obj, AG_ANIM_FIDGET)) {
        return false;
    }

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    return true;
}

// 0x435CE0
bool sub_435CE0(int64_t critter_obj)
{
    ASSERT(critter_obj != OBJ_HANDLE_NULL); // critterObj != OBJ_HANDLE_NULL
    if (critter_obj == OBJ_HANDLE_NULL) return false;

    if (!obj_type_is_critter(obj_field_int32_get(critter_obj, OBJ_F_TYPE))
        || (obj_field_int32_get(critter_obj, OBJ_F_CRITTER_FLAGS2) & OCF2_AUTO_ANIMATES) != 0
        || sub_4B6D70()) {
        return false;
    }

    sub_435BD0(critter_obj);

    return true;
}

// 0x435D70
bool sub_435D70(int64_t critter_obj)
{
    int obj_type;
    AnimGoalData goal_data;

    ASSERT(critter_obj != OBJ_HANDLE_NULL); // critterObj != OBJ_HANDLE_NULL
    if (critter_obj == OBJ_HANDLE_NULL) return false;

    obj_type = obj_field_int32_get(critter_obj, OBJ_F_TYPE);
    ASSERT(obj_type_is_critter(obj_type)); // obj_type_is_critter(objType)
    if (obj_type != OBJ_TYPE_NPC) return false;

    return sub_45D790(critter_obj)
        && !sub_423300(critter_obj, 0)
        && sub_44D4E0(&goal_data, critter_obj, AG_UNCONCEAL)
        && sub_44D520(&goal_data, &stru_5A1908);
}

// 0x435E60
bool sub_435E60(int64_t obj, int64_t tether_loc, int radius)
{
    int obj_type;
    int64_t source_obj;
    int64_t block_obj = OBJ_HANDLE_NULL;
    AnimGoalData goal_data;

    ASSERT(tether_loc != 0); // tetherLoc != 0
    ASSERT(radius > 0); // radius > 0

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    ASSERT(obj_type_is_critter(obj_type)); // obj_type_is_critter(objType)

    if ((!sub_4B6D70() || sub_4B6D80() == obj)
        && radius > 0
        && obj_type_is_critter(obj_type)
        && sub_45D790(obj)
        && !sub_423300(obj, NULL)) {
        source_obj = obj;
        if (sub_436720(&source_obj, &block_obj)) {
            sub_4363E0(block_obj, source_obj);
        } else if (sub_44D4E0(&goal_data, obj, AG_WANDER)) {
            goal_data.params[AGDATA_RANGE_DATA].data = radius;
            goal_data.params[AGDATA_SCRATCH_VAL1].data = tether_loc & 0xFFFFFFFF;
            goal_data.params[AGDATA_SCRATCH_VAL2].data = (tether_loc >> 32) & 0xFFFFFFFF;
            if (sub_44D520(&goal_data, &stru_5A1908)) {
                return true;
            }
        }
    }

    return false;
}

// 0x436040
bool sub_436040(int64_t obj, int64_t tether_loc, int radius)
{
    int obj_type;
    int64_t source_obj;
    int64_t block_obj = OBJ_HANDLE_NULL;
    AnimGoalData goal_data;

    ASSERT(tether_loc != 0); // tetherLoc != 0
    ASSERT(radius > 0); // radius > 0

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    ASSERT(obj_type_is_critter(obj_type)); // obj_type_is_critter(objType)

    if ((!sub_4B6D70() || sub_4B6D80() == obj)
        && radius > 0
        && obj_type_is_critter(obj_type)
        && sub_45D790(obj)
        && !sub_423300(obj, NULL)) {
        source_obj = obj;
        if (sub_436720(&source_obj, &block_obj)) {
            sub_4363E0(block_obj, source_obj);
        } else if (sub_44D4E0(&goal_data, obj, AG_WANDER_SEEK_DARKNESS)) {
            goal_data.params[AGDATA_RANGE_DATA].data = radius;
            goal_data.params[AGDATA_SCRATCH_VAL1].data = tether_loc & 0xFFFFFFFF;
            goal_data.params[AGDATA_SCRATCH_VAL2].data = (tether_loc >> 32) & 0xFFFFFFFF;
            if (sub_44D520(&goal_data, &stru_5A1908)) {
                return true;
            }
        }
    }

    return false;
}

// 0x436220
bool sub_436220(int64_t a1, int64_t a2, int64_t a3)
{
    // TODO: Incomplete.
}

// 0x4363E0
bool sub_4363E0(int64_t a1, int64_t a2)
{
    // TODO: Incomplete.
}

// 0x4364D0
void sub_4364D0(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x436720
bool sub_436720(int64_t* source_obj_ptr, int64_t* block_obj_ptr)
{
    // TODO: Incomplete.
}

// 0x436960
bool sub_436960(int64_t a1, int64_t a2)
{
    AnimID anim_id;
    AnimGoalData goal_data;
    AnimRunInfo* run_info;

    if (!sub_4348E0(a1, 0)) {
        return false;
    }

    if (!sub_44D500(&goal_data, a1, 69)) {
        return false;
    }

    goal_data.params[AGDATA_TARGET_OBJ].obj = a2;

    if (sub_44E710(a1, &goal_data, &anim_id)) {
        run_info = &(anim_run_info[anim_id.slot_num]);

        // FIXME: Unused.
        obj_field_int32_get(a1, OBJ_F_NPC_FLAGS);

        if (run_info->cur_stack_data->type == 56) {
            return true;
        }

        if (run_info->cur_stack_data->type != 8
            && run_info->cur_stack_data->type != 61) {
            return true;
        }

        if (sub_4B96F0(obj_field_int64_get(a1, OBJ_F_LOCATION), obj_field_int64_get(a2, OBJ_F_LOCATION)) <= 7) {
            return true;
        }
    }

    if (!sub_424070(a1, 3, false, false)) {
        return false;
    }

    if (!sub_44D520(&goal_data, &stru_5A1908)) {
        return false;
    }

    return true;
}

// NOTE: Passes AnimID by value.
//
// 0x436AB0
void turn_on_running(AnimID anim_id)
{
    AnimRunInfo* run_info;
    char str[36];
    int64_t obj;
    Packet8 pkt;

    if (!anim_id_to_run_info(&anim_id, &run_info)) {
        sub_421E20(&anim_id, str);
        tig_debug_printf("Anim: turn_on_running: could not turn animID into a AnimRunInfo %s.\n", str);
        return;
    }

    obj = run_info->goals[0].params[AGDATA_SELF_OBJ].obj;
    if (sub_45F790(obj) < ENCUMBRANCE_LEVEL_SIGNIFICANT) {
        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
            if ((tig_net_flags & TIG_NET_HOST) != 0) {
                run_info->field_C |= 0x40;
                sub_44D0C0(run_info);
            }

            sub_437460(&(pkt.modify_data));
            pkt.modify_data.id = anim_id;
            pkt.type = 8;
            pkt.modify_data.field_C = run_info->field_C;
            pkt.modify_data.field_10 = run_info->path.flags;
            pkt.modify_data.field_14 = -1;
            pkt.modify_data.field_2C = run_info->path.curr;
            pkt.modify_data.location = obj_field_int64_get(obj, OBJ_F_LOCATION);
            pkt.modify_data.current_aid = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
            pkt.offset_x = obj_field_int32_get(obj, OBJ_F_OFFSET_X);
            pkt.offset_y = obj_field_int32_get(obj, OBJ_F_OFFSET_Y);
            pkt.field_40 = run_info->field_28;
            pkt.field_44 = run_info->field_2C;

            if ((tig_net_flags & TIG_NET_HOST) != 0) {
                run_info->id.field_8++;
            }

            tig_net_send_app_all(&pkt, sizeof(pkt));
        } else {
            run_info->field_C |= 0x40;
        }
    }
}

// 0x436C20
void sub_436C20()
{
    turn_on_running(stru_5A1908);
}

// NOTE: Passes AnimID by value.
//
// 0x436C50
void sub_436C50(AnimID anim_id)
{
    turn_on_flags(anim_id, 0x100, 0);
}

// 0x436C80
void sub_436C80()
{
    sub_436C50(stru_5A1908);
}

// NOTE: Passes AnimID by value.
//
// 0x436CB0
void sub_436CB0(AnimID anim_id)
{
    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        turn_on_flags(anim_id, 0x400, 0);
    }
}

// 0x436CF0
void sub_436CF0()
{
    sub_436CB0(stru_5A1908);
}

// 0x436D20
void sub_436D20(unsigned int flags1, unsigned int flags2)
{
    turn_on_flags(stru_5A1908, flags1, flags2);
}

// NOTE: Passes AnimID by value.
//
// 0x436D50
void turn_on_flags(AnimID anim_id, unsigned int flags1, unsigned int flags2)
{
    AnimRunInfo* run_info;
    char str[36];
    int64_t obj;
    Packet8 pkt;

    if (!anim_id_to_run_info(&anim_id, &run_info)) {
        sub_421E20(&anim_id, str);
        tig_debug_printf("Anim: turn_on_flags: could not turn animID into a AnimRunInfo %s.\n", str);
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        obj = run_info->goals[0].params[AGDATA_SELF_OBJ].obj;

        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            run_info->field_C |= flags1;
            sub_44D0C0(run_info);
        }

        sub_437460(&(pkt.modify_data));
        pkt.modify_data.id = anim_id;
        pkt.type = 8;
        pkt.modify_data.field_C = run_info->field_C;
        pkt.modify_data.field_10 = run_info->path.flags;
        pkt.modify_data.field_14 = -1;
        pkt.modify_data.field_2C = run_info->path.curr;
        pkt.modify_data.location = obj_field_int64_get(obj, OBJ_F_LOCATION);
        pkt.modify_data.current_aid = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
        pkt.offset_x = obj_field_int32_get(obj, OBJ_F_OFFSET_X);
        pkt.offset_y = obj_field_int32_get(obj, OBJ_F_OFFSET_Y);
        pkt.field_40 = run_info->field_28;
        pkt.field_44 = run_info->field_2C;

        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            run_info->id.field_8++;
        }

        tig_net_send_app_all(&pkt, sizeof(pkt));
    } else {
        run_info->field_C |= flags1;
        run_info->path.flags |= flags2;
    }
}

// NOTE: Passes AnimID by value.
//
// 0x436ED0
void sub_436ED0(AnimID anim_id)
{
    turn_on_flags(anim_id, 0x4000, 0);
}

// 0x436F30
void notify_speed_recalc(AnimID* anim_id)
{
    AnimRunInfo* run_info;
    char str[36];

    if (!anim_id_to_run_info(anim_id, &run_info)) {
        sub_421E20(anim_id, str);
        tig_debug_printf("Anim: notify_speed_recalc: could not turn animID into a AnimRunInfo %s.\n", str);
        return;
    }

    // FIXME: Should use obtained `run_info`, not lookup from the master table.
    anim_run_info[anim_id->slot_num].field_C |= 0x80;
}

// 0x436FA0
void sub_436FA0(int64_t obj)
{
    AnimID anim_id;

    if (obj != OBJ_HANDLE_NULL
        && sub_423300(obj, &anim_id)) {
        notify_speed_recalc(&anim_id);
    }
}

// 0x4372B0
bool sub_4372B0(int64_t a1, int64_t a2)
{
    int index;
    AnimRunInfo* run_info;
    int goal_index;

    index = sub_44D2F0(a1);
    if (index == -1) {
        return false;
    }

    while (index != -1) {
        run_info = &(anim_run_info[index]);
        goal_index = run_info->current_goal;
        if (goal_index >= 0) {
            // FIXME: Refactor.
            goal_index++;
            do {
                run_info->goals[0].params[AGDATA_PARENT_OBJ].obj = a2;
                goal_index--;
            } while (goal_index != 0);
        }

        index = sub_44D340(index, a1);
    }

    return true;
}

// 0x4373A0
int num_goal_subslots_in_use(AnimID* anim_id)
{
    AnimRunInfo* run_info;
    char str[36];

    if (!anim_id_to_run_info(anim_id, &run_info)) {
        sub_421E20(anim_id, str);
        tig_debug_printf("Anim: num_goal_subslots_in_use: could not turn animID into a AnimRunInfo %s.\n", str);
        return 0;
    }

    return run_info->current_goal;
}

// 0x4373F0
bool is_anim_forever(AnimID* anim_id)
{
    AnimRunInfo* run_info;
    char str[36];

    if (!anim_id_to_run_info(anim_id, &run_info)) {
        sub_421E20(anim_id, str);
        tig_debug_printf("Anim: is_anim_forever: could not turn animID into a AnimRunInfo %s.\n", str);
        return false;
    }

    return run_info->cur_stack_data->type == 2
        || run_info->cur_stack_data->type == 1;
}

// 0x437460
void sub_437460(AGModifyData* modify_data)
{
    ASSERT(modify_data != NULL); // pAGModifyData != NULL

    sub_421DE0(&(modify_data->id));
    modify_data->field_18 = 0;
    modify_data->location = 0;
    modify_data->field_C = 0;
    modify_data->field_10 = 0;
    modify_data->field_1C = 0;
    modify_data->field_14 = 0;
    modify_data->current_aid = TIG_ART_ID_INVALID;
    modify_data->field_2C = -1;
}

// 0x4374C0
void mp_anim_modify()
{
    // TODO: Incomplete.
}

// 0x4377C0
bool sub_4377C0(CombatContext* combat, int64_t obj, int64_t a3, int which)
{
    int obj_type;
    int64_t weapon_obj;
    AnimFxNode node;
    tig_art_id_t art_id;
    int v1;

    ASSERT(which >= ANIM_WEAPON_EYE_CANDY_POWER_GATHER); // 17560, "whichIdx >= ANIM_WEAPON_EYE_CANDY_POWER_GATHER"
    ASSERT(which < ANIMFX_WEAPON_TYPE_COUNT); // 17561, "whichIdx < ANIMFX_WEAPON_TYPE_COUNT"

    if (obj == OBJ_HANDLE_NULL) {
        return false;
    }

    obj_type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type_is_critter(obj_type)) {
        if (combat != NULL
            && (combat->flags & 0x40000) != 0) {
            if (combat->weapon_obj == OBJ_HANDLE_NULL
                || obj_field_int32_get(combat->weapon_obj, OBJ_F_TYPE) != OBJ_TYPE_WEAPON) {
                return false;
            }

            weapon_obj = combat->weapon_obj;
        } else {
            weapon_obj = item_wield_get(obj, 1004);
        }
    } else if (obj_type == OBJ_TYPE_WEAPON) {
        weapon_obj = obj;
    } else {
        return false;
    }

    if (weapon_obj == OBJ_HANDLE_NULL) {
        return false;
    }

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    v1 =  5 * (sub_49B290(weapon_obj) - 6029);
    sub_4CCD20(&stru_5DE610, &node, a3, -1, v1 + which);
    node.rotation = tig_art_id_rotation_get(art_id);
    node.field_1C = 1;
    node.field_20 = 2;
    if (!animfx_add(&node)) {
        return false;
    }

    if (which == 3) {
        sub_4CCD20(&stru_5DE610, &node, a3, -1, v1 + 5);
        node.field_1C = 1;
        node.field_20 = 0;
        node.field_24 |= 2u;
        animfx_add(&node);
    }

    return true;
}

// 0x437980
void sub_437980()
{
}

// 0x437990
void sub_437990()
{
    // TODO: Incomplete.
}

// 0x437C50
void sub_437C50()
{
    // TODO: Incomplete.
}

// 0x437CF0
void sub_437CF0()
{
    // TODO: Incomplete.
}

// 0x437D00
void sub_437D00(AnimRunInfo* run_info)
{
    AnimGoalData* goal_data;
    AnimGoalNode* goal_node;

    goal_data = &(run_info->goals[run_info->current_goal]);
    if (run_info->current_goal == 0) {
        run_info->field_C |= 0x2;
    }

    if (goal_data->type >= 0 && goal_data->type < 87) {
        goal_node = off_5B03D0[goal_data->type];
        if (goal_node->subnodes[14].func != NULL
            && sub_44DD80(run_info, &(goal_node->subnodes[14]))) {
            goal_node->subnodes[14].func(run_info);
        }

        sub_44C8F0(run_info, goal_node);
    }

    run_info->current_goal--;
    if (run_info->current_goal >= 0) {
        run_info->cur_stack_data = &(run_info->goals[run_info->current_goal]);
    }

    run_info->field_10 = 0;
}
