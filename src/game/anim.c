#include "game/anim.h"

#include "game/animfx.h"
#include "game/gamelib.h"

static void violence_filter_changed();

// 0x5E3500
static int dword_5E3500;

// 0x5DE610
static AnimFxList stru_5DE610;

// 0x5DE670
static AnimFxList stru_5DE670;

// 0x5DE6A4
static int violence_filter;

// 0x5DE6D4
static bool anim_editor;

// 0x421B00
void anim_init()
{
    // TODO: Incomplete.
}

// 0x421BF0
void anim_exit()
{
    if (!anim_editor) {
        animfx_list_exit_(&stru_5DE610);
        animfx_list_exit_(&stru_5DE670);
    }
}

// 0x421C20
void anim_reset()
{
    dword_5E3500 = 0;
}

// 0x421C30
void anim_id_to_run_info()
{
    // TODO: Incomplete.
}

// 0x421CE0
void sub_421CE0()
{
    // TODO: Incomplete.
}

// 0x421D60
void sub_421D60()
{
    // TODO: Incomplete.
}

// 0x421DE0
void sub_421DE0()
{
    // TODO: Incomplete.
}

// 0x421E20
void sub_421E20()
{
    // TODO: Incomplete.
}

// 0x421EA0
void anim_save()
{
    // TODO: Incomplete.
}

// 0x4221A0
void violence_filter_changed()
{
    violence_filter = settings_get_value(&settings, "violence filter");
}

// 0x4221C0
void sub_4221C0()
{
    // TODO: Incomplete.
}

// 0x422350
void sub_422350()
{
    // TODO: Incomplete.
}

// 0x422430
void sub_422430()
{
    // TODO: Incomplete.
}

// 0x4224E0
void anim_load()
{
    // TODO: Incomplete.
}

// 0x422520
void sub_422520()
{
    // TODO: Incomplete.
}

// 0x4227F0
void sub_4227F0()
{
    // TODO: Incomplete.
}

// 0x4229A0
void sub_4229A0()
{
    // TODO: Incomplete.
}

// 0x422A50
void sub_422A50()
{
    // TODO: Incomplete.
}

// 0x422B10
void anim_break_nodes_to_map()
{
    // TODO: Incomplete.
}

// 0x422DF0
void anim_save_nodes_to_map()
{
    // TODO: Incomplete.
}

// 0x4230A0
void anim_load_nodes_from_map()
{
    // TODO: Incomplete.
}

// 0x4232F0
void sub_4232F0()
{
    // TODO: Incomplete.
}

// 0x423300
void sub_423300()
{
    // TODO: Incomplete.
}

// 0x4233D0
void sub_4233D0()
{
    // TODO: Incomplete.
}

// 0x423470
void sub_423470()
{
    // TODO: Incomplete.
}

// 0x4234F0
void sub_4234F0()
{
    // TODO: Incomplete.
}

// 0x423530
void nullsub_29()
{
    // TODO: Incomplete.
}

// 0x423540
void nullsub_30()
{
    // TODO: Incomplete.
}

// 0x423550
void nullsub_31()
{
    // TODO: Incomplete.
}

// 0x423560
void anim_timeevent_process()
{
    // TODO: Incomplete.
}

// 0x423C80
void sub_423C80()
{
    // TODO: Incomplete.
}

// 0x423D10
void sub_423D10()
{
    // TODO: Incomplete.
}

// 0x423E60
void sub_423E60()
{
    // TODO: Incomplete.
}

// 0x423EB0
void anim_goal_pending_active_goals_count()
{
    // TODO: Incomplete.
}

// 0x423FB0
void sub_423FB0()
{
    // TODO: Incomplete.
}

// 0x423FC0
void sub_423FC0()
{
    // TODO: Incomplete.
}

// 0x423FE0
void sub_423FE0()
{
    // TODO: Incomplete.
}

// 0x423FF0
void sub_423FF0()
{
    // TODO: Incomplete.
}

// 0x424070
void sub_424070()
{
    // TODO: Incomplete.
}

// 0x424250
void sub_424250()
{
    // TODO: Incomplete.
}

// 0x424290
void anim_goal_interrupt_all_goals_of_priority()
{
    // TODO: Incomplete.
}

// 0x424310
void anim_goal_interrupt_all_for_tb_combat()
{
    // TODO: Incomplete.
}

// 0x4243E0
void sub_4243E0()
{
    // TODO: Incomplete.
}

// 0x424560
void sub_424560()
{
    // TODO: Incomplete.
}

// 0x4246C0
void sub_4246C0()
{
    // TODO: Incomplete.
}

// 0x4246D0
void sub_4246D0()
{
    // TODO: Incomplete.
}

// 0x4246E0
void sub_4246E0()
{
    // TODO: Incomplete.
}

// 0x424820
void sub_424820()
{
    // TODO: Incomplete.
}

// 0x4248A0
void sub_4248A0()
{
    // TODO: Incomplete.
}

// 0x424BC0
void sub_424BC0()
{
    // TODO: Incomplete.
}

// 0x424D00
void sub_424D00()
{
    // TODO: Incomplete.
}

// 0x424D90
void sub_424D90()
{
    // TODO: Incomplete.
}

// 0x424E00
void sub_424E00()
{
    // TODO: Incomplete.
}

// 0x425130
void sub_425130()
{
    // TODO: Incomplete.
}

// 0x425270
void sub_425270()
{
    // TODO: Incomplete.
}

// 0x425340
void sub_425340()
{
    // TODO: Incomplete.
}

// 0x425430
void sub_425430()
{
    // TODO: Incomplete.
}

// 0x4254C0
void sub_4254C0()
{
    // TODO: Incomplete.
}

// 0x425590
void sub_425590()
{
    // TODO: Incomplete.
}

// 0x425740
void sub_425740()
{
    // TODO: Incomplete.
}

// 0x425760
void sub_425760()
{
    // TODO: Incomplete.
}

// 0x4257E0
void sub_4257E0()
{
    // TODO: Incomplete.
}

// 0x425840
void sub_425840()
{
    // TODO: Incomplete.
}

// 0x425930
void sub_425930()
{
    // TODO: Incomplete.
}

// 0x425BF0
void sub_425BF0()
{
    // TODO: Incomplete.
}

// 0x425D60
void sub_425D60()
{
    // TODO: Incomplete.
}

// 0x426040
void sub_426040()
{
    // TODO: Incomplete.
}

// 0x4261E0
void sub_4261E0()
{
    // TODO: Incomplete.
}

// 0x426250
void sub_426250()
{
    // TODO: Incomplete.
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
void sub_426840()
{
    // TODO: Incomplete.
}

// 0x4268F0
void sub_4268F0()
{
    // TODO: Incomplete.
}

// 0x4269D0
void sub_4269D0()
{
    // TODO: Incomplete.
}

// 0x426A80
void sub_426A80()
{
    // TODO: Incomplete.
}

// 0x426E80
void sub_426E80()
{
    // TODO: Incomplete.
}

// 0x426F10
void sub_426F10()
{
    // TODO: Incomplete.
}

// 0x426F60
void sub_426F60()
{
    // TODO: Incomplete.
}

// 0x427000
void sub_427000()
{
    // TODO: Incomplete.
}

// 0x4270B0
void sub_4270B0()
{
    // TODO: Incomplete.
}

// 0x427110
void sub_427110()
{
    // TODO: Incomplete.
}

// 0x4272E0
void sub_4272E0()
{
    // TODO: Incomplete.
}

// 0x4273B0
void sub_4273B0()
{
    // TODO: Incomplete.
}

// 0x427640
void sub_427640()
{
    // TODO: Incomplete.
}

// 0x427710
void sub_427710()
{
    // TODO: Incomplete.
}

// 0x427720
void sub_427720()
{
    // TODO: Incomplete.
}

// 0x427730
void sub_427730()
{
    // TODO: Incomplete.
}

// 0x427990
void sub_427990()
{
    // TODO: Incomplete.
}

// 0x4280D0
void sub_4280D0()
{
    // TODO: Incomplete.
}

// 0x4284A0
void sub_4284A0()
{
    // TODO: Incomplete.
}

// 0x4284F0
void sub_4284F0()
{
    // TODO: Incomplete.
}

// 0x428550
void sub_428550()
{
    // TODO: Incomplete.
}

// 0x428620
void sub_428620()
{
    // TODO: Incomplete.
}

// 0x428690
void sub_428690()
{
    // TODO: Incomplete.
}

// 0x428750
void sub_428750()
{
    // TODO: Incomplete.
}

// 0x4287E0
void sub_4287E0()
{
    // TODO: Incomplete.
}

// 0x428890
void sub_428890()
{
    // TODO: Incomplete.
}

// 0x4288A0
void sub_4288A0()
{
    // TODO: Incomplete.
}

// 0x428930
void sub_428930()
{
    // TODO: Incomplete.
}

// 0x428A10
void sub_428A10()
{
    // TODO: Incomplete.
}

// 0x428CD0
void sub_428CD0()
{
    // TODO: Incomplete.
}

// 0x428E10
void sub_428E10()
{
    // TODO: Incomplete.
}

// 0x429040
void sub_429040()
{
    // TODO: Incomplete.
}

// 0x429160
void sub_429160()
{
    // TODO: Incomplete.
}

// 0x429370
void sub_429370()
{
    // TODO: Incomplete.
}

// 0x429380
void sub_429380()
{
    // TODO: Incomplete.
}

// 0x429390
void sub_429390()
{
    // TODO: Incomplete.
}

// 0x4293A0
void sub_4293A0()
{
    // TODO: Incomplete.
}

// 0x4293B0
void sub_4293B0()
{
    // TODO: Incomplete.
}

// 0x4293C0
void sub_4293C0()
{
    // TODO: Incomplete.
}

// 0x4293D0
void sub_4293D0()
{
    // TODO: Incomplete.
}

// 0x429420
void sub_429420()
{
    // TODO: Incomplete.
}

// 0x429430
void sub_429430()
{
    // TODO: Incomplete.
}

// 0x429440
void sub_429440()
{
    // TODO: Incomplete.
}

// 0x429450
void sub_429450()
{
    // TODO: Incomplete.
}

// 0x4294A0
void sub_4294A0()
{
    // TODO: Incomplete.
}

// 0x4294F0
void sub_4294F0()
{
    // TODO: Incomplete.
}

// 0x4296D0
void sub_4296D0()
{
    // TODO: Incomplete.
}

// 0x429760
void sub_429760()
{
    // TODO: Incomplete.
}

// 0x4298D0
void sub_4298D0()
{
    // TODO: Incomplete.
}

// 0x429960
void sub_429960()
{
    // TODO: Incomplete.
}

// 0x429AD0
void sub_429AD0()
{
    // TODO: Incomplete.
}

// 0x429B40
void sub_429B40()
{
    // TODO: Incomplete.
}

// 0x429B50
void sub_429B50()
{
    // TODO: Incomplete.
}

// 0x429BB0
void sub_429BB0()
{
    // TODO: Incomplete.
}

// 0x429BC0
void sub_429BC0()
{
    // TODO: Incomplete.
}

// 0x429C40
void sub_429C40()
{
    // TODO: Incomplete.
}

// 0x429C80
void sub_429C80()
{
    // TODO: Incomplete.
}

// 0x429CD0
void sub_429CD0()
{
    // TODO: Incomplete.
}

// 0x429E70
void sub_429E70()
{
    // TODO: Incomplete.
}

// 0x429ED0
void sub_429ED0()
{
    // TODO: Incomplete.
}

// 0x429F00
void sub_429F00()
{
    // TODO: Incomplete.
}

// 0x42A010
void sub_42A010()
{
    // TODO: Incomplete.
}

// 0x42A180
void sub_42A180()
{
    // TODO: Incomplete.
}

// 0x42A200
void sub_42A200()
{
    // TODO: Incomplete.
}

// 0x42A260
void sub_42A260()
{
    // TODO: Incomplete.
}

// 0x42A280
void sub_42A280()
{
    // TODO: Incomplete.
}

// 0x42A2A0
void sub_42A2A0()
{
    // TODO: Incomplete.
}

// 0x42A430
void sub_42A430()
{
    // TODO: Incomplete.
}

// 0x42A440
void sub_42A440()
{
    // TODO: Incomplete.
}

// 0x42A490
void sub_42A490()
{
    // TODO: Incomplete.
}

// 0x42A4E0
void sub_42A4E0()
{
    // TODO: Incomplete.
}

// 0x42A630
void sub_42A630()
{
    // TODO: Incomplete.
}

// 0x42A720
void sub_42A720()
{
    // TODO: Incomplete.
}

// 0x42A930
void sub_42A930()
{
    // TODO: Incomplete.
}

// 0x42A9B0
void sub_42A9B0()
{
    // TODO: Incomplete.
}

// 0x42AA70
void sub_42AA70()
{
    // TODO: Incomplete.
}

// 0x42AB90
void sub_42AB90()
{
    // TODO: Incomplete.
}

// 0x42ACD0
void sub_42ACD0()
{
    // TODO: Incomplete.
}

// 0x42AE10
void sub_42AE10()
{
    // TODO: Incomplete.
}

// 0x42AF00
void sub_42AF00()
{
    // TODO: Incomplete.
}

// 0x42AFB0
void sub_42AFB0()
{
    // TODO: Incomplete.
}

// 0x42B090
void sub_42B090()
{
    // TODO: Incomplete.
}

// 0x42B250
void sub_42B250()
{
    // TODO: Incomplete.
}

// 0x42B440
void sub_42B440()
{
    // TODO: Incomplete.
}

// 0x42B640
void sub_42B640()
{
    // TODO: Incomplete.
}

// 0x42B6F0
void sub_42B6F0()
{
    // TODO: Incomplete.
}

// 0x42B790
void sub_42B790()
{
    // TODO: Incomplete.
}

// 0x42B7F0
void sub_42B7F0()
{
    // TODO: Incomplete.
}

// 0x42B8B0
void sub_42B8B0()
{
    // TODO: Incomplete.
}

// 0x42B940
void sub_42B940()
{
    // TODO: Incomplete.
}

// 0x42B9C0
void sub_42B9C0()
{
    // TODO: Incomplete.
}

// 0x42BC10
void sub_42BC10()
{
    // TODO: Incomplete.
}

// 0x42BD40
void sub_42BD40()
{
    // TODO: Incomplete.
}

// 0x42BE50
void sub_42BE50()
{
    // TODO: Incomplete.
}

// 0x42BE80
void sub_42BE80()
{
    // TODO: Incomplete.
}

// 0x42BEA0
void sub_42BEA0()
{
    // TODO: Incomplete.
}

// 0x42BEC0
void sub_42BEC0()
{
    // TODO: Incomplete.
}

// 0x42BF40
void sub_42BF40()
{
    // TODO: Incomplete.
}

// 0x42BFD0
void sub_42BFD0()
{
    // TODO: Incomplete.
}

// 0x42C0F0
void sub_42C0F0()
{
    // TODO: Incomplete.
}

// 0x42C240
void sub_42C240()
{
    // TODO: Incomplete.
}

// 0x42C390
void sub_42C390()
{
    // TODO: Incomplete.
}

// 0x42C440
void sub_42C440()
{
    // TODO: Incomplete.
}

// 0x42C610
void sub_42C610()
{
    // TODO: Incomplete.
}

// 0x42C650
void sub_42C650()
{
    // TODO: Incomplete.
}

// 0x42C780
void sub_42C780()
{
    // TODO: Incomplete.
}

// 0x42C850
void sub_42C850()
{
    // TODO: Incomplete.
}

// 0x42CA90
void sub_42CA90()
{
    // TODO: Incomplete.
}

// 0x42CAA0
void sub_42CAA0()
{
    // TODO: Incomplete.
}

// 0x42CAC0
void sub_42CAC0()
{
    // TODO: Incomplete.
}

// 0x42CB10
void sub_42CB10()
{
    // TODO: Incomplete.
}

// 0x42CC80
void sub_42CC80()
{
    // TODO: Incomplete.
}

// 0x42CDF0
void sub_42CDF0()
{
    // TODO: Incomplete.
}

// 0x42CF40
void sub_42CF40()
{
    // TODO: Incomplete.
}

// 0x42D080
void sub_42D080()
{
    // TODO: Incomplete.
}

// 0x42D160
void sub_42D160()
{
    // TODO: Incomplete.
}

// 0x42D1C0
void sub_42D1C0()
{
    // TODO: Incomplete.
}

// 0x42D2A0
void sub_42D2A0()
{
    // TODO: Incomplete.
}

// 0x42D300
void sub_42D300()
{
    // TODO: Incomplete.
}

// 0x42D440
void sub_42D440()
{
    // TODO: Incomplete.
}

// 0x42D570
void sub_42D570()
{
    // TODO: Incomplete.
}

// 0x42D6F0
void sub_42D6F0()
{
    // TODO: Incomplete.
}

// 0x42D7D0
void sub_42D7D0()
{
    // TODO: Incomplete.
}

// 0x42D910
void sub_42D910()
{
    // TODO: Incomplete.
}

// 0x42DA50
void sub_42DA50()
{
    // TODO: Incomplete.
}

// 0x42DCF0
void sub_42DCF0()
{
    // TODO: Incomplete.
}

// 0x42DDE0
void sub_42DDE0()
{
    // TODO: Incomplete.
}

// 0x42DED0
void sub_42DED0()
{
    // TODO: Incomplete.
}

// 0x42DF40
void sub_42DF40()
{
    // TODO: Incomplete.
}

// 0x42E070
void sub_42E070()
{
    // TODO: Incomplete.
}

// 0x42E1B0
void sub_42E1B0()
{
    // TODO: Incomplete.
}

// 0x42E2D0
void sub_42E2D0()
{
    // TODO: Incomplete.
}

// 0x42E460
void sub_42E460()
{
    // TODO: Incomplete.
}

// 0x42E4B0
void sub_42E4B0()
{
    // TODO: Incomplete.
}

// 0x42E590
void sub_42E590()
{
    // TODO: Incomplete.
}

// 0x42E6B0
void sub_42E6B0()
{
    // TODO: Incomplete.
}

// 0x42E720
void sub_42E720()
{
    // TODO: Incomplete.
}

// 0x42E8B0
void sub_42E8B0()
{
    // TODO: Incomplete.
}

// 0x42E9B0
void sub_42E9B0()
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
void sub_42F000()
{
    // TODO: Incomplete.
}

// 0x42F140
void sub_42F140()
{
    // TODO: Incomplete.
}

// 0x42F2D0
void sub_42F2D0()
{
    // TODO: Incomplete.
}

// 0x42F390
void sub_42F390()
{
    // TODO: Incomplete.
}

// 0x42F5C0
void sub_42F5C0()
{
    // TODO: Incomplete.
}

// 0x42F6A0
void sub_42F6A0()
{
    // TODO: Incomplete.
}

// 0x42FA50
void sub_42FA50()
{
    // TODO: Incomplete.
}

// 0x42FD70
void sub_42FD70()
{
    // TODO: Incomplete.
}

// 0x42FEA0
void sub_42FEA0()
{
    // TODO: Incomplete.
}

// 0x42FEB0
void sub_42FEB0()
{
    // TODO: Incomplete.
}

// 0x42FEC0
void sub_42FEC0()
{
    // TODO: Incomplete.
}

// 0x42FED0
void sub_42FED0()
{
    // TODO: Incomplete.
}

// 0x42FF40
void sub_42FF40()
{
    // TODO: Incomplete.
}

// 0x42FFE0
void sub_42FFE0()
{
    // TODO: Incomplete.
}

// 0x4300D0
void anim_fidget_timeevent_process()
{
    // TODO: Incomplete.
}

// 0x4302D0
void sub_4302D0()
{
    // TODO: Incomplete.
}

// 0x4303D0
void sub_4303D0()
{
    // TODO: Incomplete.
}

// 0x430460
void sub_430460()
{
    // TODO: Incomplete.
}

// 0x430490
void sub_430490()
{
    // TODO: Incomplete.
}

// 0x4304C0
void sub_4304C0()
{
    // TODO: Incomplete.
}

// 0x430580
void set_always_run()
{
    // TODO: Incomplete.
}

// 0x4305D0
void sub_4305D0()
{
    // TODO: Incomplete.
}

// 0x430F20
void sub_430F20()
{
    // TODO: Incomplete.
}

// 0x430FC0
void sub_430FC0()
{
    // TODO: Incomplete.
}

// 0x431130
void sub_431130()
{
    // TODO: Incomplete.
}

// 0x431150
void sub_431150()
{
    // TODO: Incomplete.
}

// 0x4311F0
void sub_4311F0()
{
    // TODO: Incomplete.
}

// 0x431290
void sub_431290()
{
    // TODO: Incomplete.
}

// 0x431320
void sub_431320()
{
    // TODO: Incomplete.
}

// 0x431340
void sub_431340()
{
    // TODO: Incomplete.
}

// 0x431550
void sub_431550()
{
    // TODO: Incomplete.
}

// 0x4315B0
void sub_4315B0()
{
    // TODO: Incomplete.
}

// 0x431960
void sub_431960()
{
    // TODO: Incomplete.
}

// 0x4319F0
void sub_4319F0()
{
    // TODO: Incomplete.
}

// 0x431A40
void sub_431A40()
{
    // TODO: Incomplete.
}

// 0x431B20
void sub_431B20()
{
    // TODO: Incomplete.
}

// 0x431C40
void sub_431C40()
{
    // TODO: Incomplete.
}

// 0x431E50
void sub_431E50()
{
    // TODO: Incomplete.
}

// 0x4321C0
void sub_4321C0()
{
    // TODO: Incomplete.
}

// 0x4322A0
void sub_4322A0()
{
    // TODO: Incomplete.
}

// 0x4324D0
void sub_4324D0()
{
    // TODO: Incomplete.
}

// 0x432700
void sub_432700()
{
    // TODO: Incomplete.
}

// 0x432990
void sub_432990()
{
    // TODO: Incomplete.
}

// 0x432CF0
void sub_432CF0()
{
    // TODO: Incomplete.
}

// 0x432D50
void sub_432D50()
{
    // TODO: Incomplete.
}

// 0x432D90
void sub_432D90()
{
    // TODO: Incomplete.
}

// 0x433020
void sub_433020()
{
    // TODO: Incomplete.
}

// 0x433170
void sub_433170()
{
    // TODO: Incomplete.
}

// 0x433220
void sub_433220()
{
    // TODO: Incomplete.
}

// 0x433270
void sub_433270()
{
    // TODO: Incomplete.
}

// 0x4332E0
void sub_4332E0()
{
    // TODO: Incomplete.
}

// 0x433440
void sub_433440()
{
    // TODO: Incomplete.
}

// 0x433580
void sub_433580()
{
    // TODO: Incomplete.
}

// 0x433640
void sub_433640()
{
    // TODO: Incomplete.
}

// 0x4339A0
void sub_4339A0()
{
    // TODO: Incomplete.
}

// 0x433A00
void sub_433A00()
{
    // TODO: Incomplete.
}

// 0x433C80
void sub_433C80()
{
    // TODO: Incomplete.
}

// 0x434030
void sub_434030()
{
    // TODO: Incomplete.
}

// 0x4341C0
void sub_4341C0()
{
    // TODO: Incomplete.
}

// 0x434400
void sub_434400()
{
    // TODO: Incomplete.
}

// 0x4346A0
void sub_4346A0()
{
    // TODO: Incomplete.
}

// 0x4348E0
void sub_4348E0()
{
    // TODO: Incomplete.
}

// 0x434980
void sub_434980()
{
    // TODO: Incomplete.
}

// 0x434AE0
void sub_434AE0()
{
    // TODO: Incomplete.
}

// 0x434B00
void sub_434B00()
{
    // TODO: Incomplete.
}

// 0x434DE0
void sub_434DE0()
{
    // TODO: Incomplete.
}

// 0x434E80
void sub_434E80()
{
    // TODO: Incomplete.
}

// 0x434F80
void sub_434F80()
{
    // TODO: Incomplete.
}

// 0x435080
void sub_435080()
{
    // TODO: Incomplete.
}

// 0x4350F0
void sub_4350F0()
{
    // TODO: Incomplete.
}

// 0x4352C0
void sub_4352C0()
{
    // TODO: Incomplete.
}

// 0x435450
void sub_435450()
{
    // TODO: Incomplete.
}

// 0x4355F0
void sub_4355F0()
{
    // TODO: Incomplete.
}

// 0x4356C0
void sub_4356C0()
{
    // TODO: Incomplete.
}

// 0x4357B0
void sub_4357B0()
{
    // TODO: Incomplete.
}

// 0x435870
void sub_435870()
{
    // TODO: Incomplete.
}

// 0x435A00
void sub_435A00()
{
    // TODO: Incomplete.
}

// 0x435A90
void sub_435A90()
{
    // TODO: Incomplete.
}

// 0x435B30
void sub_435B30()
{
    // TODO: Incomplete.
}

// 0x435BD0
void sub_435BD0()
{
    // TODO: Incomplete.
}

// 0x435CE0
void sub_435CE0()
{
    // TODO: Incomplete.
}

// 0x435D70
void sub_435D70()
{
    // TODO: Incomplete.
}

// 0x435E60
void sub_435E60()
{
    // TODO: Incomplete.
}

// 0x436040
void sub_436040()
{
    // TODO: Incomplete.
}

// 0x436220
void sub_436220()
{
    // TODO: Incomplete.
}

// 0x4363E0
void sub_4363E0()
{
    // TODO: Incomplete.
}

// 0x4364D0
void sub_4364D0()
{
    // TODO: Incomplete.
}

// 0x436720
void sub_436720()
{
    // TODO: Incomplete.
}

// 0x436960
void sub_436960()
{
    // TODO: Incomplete.
}

// 0x436AB0
void turn_on_running()
{
    // TODO: Incomplete.
}

// 0x436C20
void sub_436C20()
{
    // TODO: Incomplete.
}

// 0x436C50
void sub_436C50()
{
    // TODO: Incomplete.
}

// 0x436C80
void sub_436C80()
{
    // TODO: Incomplete.
}

// 0x436CB0
void sub_436CB0()
{
    // TODO: Incomplete.
}

// 0x436CF0
void sub_436CF0()
{
    // TODO: Incomplete.
}

// 0x436D20
void sub_436D20()
{
    // TODO: Incomplete.
}

// 0x436D50
void turn_on_flags()
{
    // TODO: Incomplete.
}

// 0x436ED0
void sub_436ED0()
{
    // TODO: Incomplete.
}

// 0x436F30
void notify_speed_recalc()
{
    // TODO: Incomplete.
}

// 0x436FA0
void sub_436FA0()
{
    // TODO: Incomplete.
}

// 0x4372B0
void sub_4372B0()
{
    // TODO: Incomplete.
}

// 0x4373A0
void num_goal_subslots_in_use()
{
    // TODO: Incomplete.
}

// 0x4373F0
void is_anim_forever()
{
    // TODO: Incomplete.
}

// 0x437460
void sub_437460()
{
    // TODO: Incomplete.
}

// 0x4374C0
void mp_anim_modify()
{
    // TODO: Incomplete.
}

// 0x4377C0
void sub_4377C0()
{
    // TODO: Incomplete.
}

// 0x437980
void nullsub_32()
{
    // TODO: Incomplete.
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
void sub_437D00()
{
    // TODO: Incomplete.
}
