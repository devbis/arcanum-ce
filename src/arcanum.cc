#include "arcanum.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <timeapi.h>
#include <windows.h>

// 59A---

// 0x59ADD8
int dword_59ADD8 = 1;

// 0x59ADDC
char byte_59ADDC[260] = "Arcanum";

// 0x59AEDC
char arcanum[260] = "Arcanum";

// 59F---

// 0x59F050
int dword_59F050 = 1000;

// 5A3--

// 0x5A3E90
int dword_5A3E90 = 1;

// 5A5--

// 0x5A53A0
int dword_5A53A0 = 256;

// 0x5A54AC
int dword_5A54AC = 256;

// 0x5A5978
int dword_5A5978 = -1;

// 5B2---

// 0x5B278C
int dword_5B278C;

// 0x5B2790
int dword_5B2790;

// 5B3---

// 0x5B306C
int dword_5B306C[7] = {
    15,
    30,
    45,
    60,
    75,
    90,
    100,
};

// 0x5B32C0
int dword_5B32C0[11] = {
    2,
    1,
    2,
    2,
    1,
    1,
    2,
    4,
    2,
    4,
    2,
};

// 0x5B37FC
int dword_5B37FC[20] = {
    1000,
    2008,
    3023,
    4006,
    5028,
    6029,
    7038,
    8042,
    9056,
    10057,
    11059,
    12060,
    13061,
    14062,
    15065,
    16066,
    17067,
    26000,
    27309,
    28310,
};

// 0x5B3FD8
int dword_5B3FD8 = 10;

// 5B5---

// 0x5B5050
int dword_5B5050 = -1;

// 0x5B5054
int dword_5B5054 = 1;

// 0x5B5124
int dword_5B5124[8] = {
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
};

// 0x5B5144
int dword_5B5144[8] = {
    0,
    10,
    20,
    35,
    50,
    65,
    80,
    100,
};

// 0x5B5164
int dword_5B5164[8] = {
    0,
    5,
    8,
    11,
    13,
    15,
    17,
    19,
};

// 0x5B5194
int dword_5B5194[28] = {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    300,
    -50,
    -9,
    1,
    0,
    1,
    -65,
    1,
    -100,
    0,
    0,
    -1000,
    0,
    0,
    0,
    0,
    0,
    20,
    0,
    0,
};

// 0x5B55B0
int dword_5B55B0[16] = {
    11,
    44,
    38,
    92,
    50,
    86,
    56,
    98,
    62,
    68,
    74,
    104,
    110,
    80,
    116,
    122,
};

// #unclear
// 0x5B5798
int dword_5B5798[1] = {
    0,
};

// 5B6---

// 0x5B6F04
int dword_5B6F04[12] = {
    1,
    1,
    1,
    1,
    1,
    1,
    5,
    5,
    4,
    6,
    4,
    7,
};

// #wrong
// 0x5B6F44
int dword_5B6F44[1] = {
    0,
};

// 0x5B6F34
int dword_5B6F34[4] = {
    4,
    5,
    1,
    5,
};

// 5B7---

// 0x5B7CD0
int dword_5B7CD0 = -1;

// 0x5B7CD4
int dword_5B7CD4 = -1;

// 0x5B7CD8
int dword_5B7CD8 = 1;

// 5D0---

// 0x5D0E9C
int game_difficulty_;

// 5D1---

// 0x5D10E8
bool in_gamelib_reset_;

// 0x5D10D4
int dword_5D10D4;

// 0x5D10D8
int dword_5D10D8;

// 0x5D10E4
int dword_5D10E4;

// 0x5D1A18
int voice_volume_;

// 0x5D1A44
int music_volume_;

// 0x5D11C4
int dword_5D11C4;

// 0x5D11C8
int dword_5D11C8;

// 0x5D11FC
int dword_5D11FC;

// 0x5D1208
int dword_5D1208;

// 0x5D1210
int dword_5D1210;

// 5D5---

// 0x5D559C
int effects_volume_;

// 5DE---

// 0x5DE6D0
int dword_5DE6D0;

// 0x5DE6E0
int dword_5DE6E0;

// 5E0---

// 0x5E0A00
int dword_5E0A00;

// 5E2---

// 0x5E2E44
int dword_5E2E44;

// 0x5E2FA0
int* dword_5E2FA0;

// 0x5E2FA4
int dword_5E2FA4;

// 5E3---

// 0x5E34F8
int dword_5E34F8;

// 0x5E3500
int dword_5E3500;

// 0x5E3504
int dword_5E3504;

// 5E7---

// 0x5E75FC
int dword_5E75FC;

// 5E8---

// 0x5E8600
int dword_5E8600;

// 0x5E8614
int dword_5E8614;

// 0x5E8618
int dword_5E8618;

// 0x5E8638
int dword_5E8638;

// 0x5E8658
void (*off_5E8658)();

// 0x5E86A8
void (*off_5E86A8)(int a1);

// 0x5E86AC
void (*off_5E86AC)();

// 0x5E86B4
void (*off_5E86B4)();

// 0x5E86B8
void (*off_5E86B8)();

// 0x5E86BC
void (*off_5E86BC)(int a1);

// 0x5E86C8
void (*off_5E86C8)();

// 0x5E86CC
void (*off_5E86CC)();

// 0x5E86D4
void (*off_5E86D4)(int a1);

// 0x5E86D8
void (*off_5E86D8)(int a1);

// 0x5E86DC
void (*off_5E86DC)(int a1);

// 0x5E86E0
void (*off_5E86E0)();

// 0x5E86F0
void (*off_5E86F0)();

// 0x5E86F4
void (*off_5E86F4)();

// 0x5E86FC
void (*off_5E86FC)(int a1);

// 0x5E8710
void (*off_5E8710)();

// 0x5E8714
void (*off_5E8714)(int a1);

// 0x5E8718
void (*off_5E8718)(int a1);

// 0x5E872C
void (*off_5E872C)(int a1);

// 0x5E8754
void (*off_5E8754)(int a1);

// 0x5E8758
void (*off_5E8758)();

// 0x5E8760
void (*off_5E8760)();

// 0x5E876C
void (*off_5E876C)(int a1);

// 0x5E8774
int dword_5E8774;

// 0x5E8778
int (*off_5E8778)();

// 0x5E8784
int (*off_5E8784)();

// 0x5E879C
void (*off_5E879C)(int a1);

// 0x5E87A0
void (*off_5E87A0)(int a1);

// 0x5E87B4
void (*off_5E87B4)();

// 0x5E87BC
void (*off_5E87BC)(int a1);

// 0x5E87C8
int dword_5E87C8;

// 0x5E87E0
int* off_5E87E0;

// 0x5E87E8
int dword_5E87E8;

// 0x5E8800
int dword_5E8800;

// 0x5E8820
int dword_5E8820;

// 5F0---

// 0x5F0BC8
void* off_5F0BC8[8];

// 0x5F0DE0
int dword_5F0DE0;

// 0x5F0E04
void (*off_5F0E04)();

// 0x5F0E08
int dword_5F0E08;

// 0x5F0E0C
int dword_5F0E0C;

// 0x5F0E10
int dword_5F0E10;

// 5F5---

// 0x5F5CA0
void* off_5F5CA0;

// 0x5F5CA4
int dword_5F5CA4;

// 0x5F5CA
int dword_5F5CAC;

// 5F8---

// 0x5F85C8
int dword_5F85C8;

// 0x5F84A8
int dword_5F84A8[8];

// 0x5F84C8
int dword_5F84C8[64];

// 0x5F85CC
int dword_5F85CC[8];

// 0x5F85EC
int dword_5F85EC[8];

// 0x5F8610
int dword_5F8610[2];

// 0x5F8618
int dword_5F8618[11];

// 0x5F8644
int dword_5F8644;

// 0x5F8648
int dword_5F8648[28];

// 0x5F86B8
int dword_5F86B8[28];

// 0x5F8774
int dword_5F8774[16];

// 5FC---

// #maybe-bool
// 0x5FC224
int turn_based_;

// #maybe-bool
// 0x5FC228
int fast_turn_based_;

// 0x5FC22C
int dword_5FC22C;

// 0x5FC230
int dword_5FC230;

// 0x5FC234
int dword_5FC234;

// 0x5FC244
int dword_5FC244;

// 0x5FC2F8
int dword_5FC2F8;

// 0x5FC328
int dword_5FC328;

// 0x5FC32C
int dword_5FC32C[8];

// 0x5FC350
void* off_5FC350;

// 0x5FC368
void* off_5FC368;

// 0x5FC390
int dword_5FC390;

// 0x5FC3DC
int dword_5FC3DC;

// 0x5FC43C
int dword_5FC43C;

// 0x5FC4A8
int dword_5FC4A8;

// 0x5FC660
int dword_5FC660;

// 0x5FC874
int dword_5FC874;

// 0x5FC87C
int dword_5FC87C;

// 0x5FC88C
int dword_5FC88C;

// 0x5FC890
int dword_5FC890[7];

// 5FD---

// 0x5FDC88
int dword_5FDC88;

// 5FF---

// 0x5FF418
int* off_5FF418;

// 0x5FF484
int dword_5FF484;

// 0x5FF424
int dword_5FF424[12];

// 0x5FF454
int dword_5FF454[12];

// 0x5FF4E8
int dword_5FF4E8[4];

// 0x5FF488
int dword_5FF488[4];

// 0x5FF498
int dword_5FF498[4];

// 0x5FF58C
int dword_5FF58C;

// 0x5FF590
int dword_5FF590;

// 0x5FF5A8
int dword_5FF5A8;

// 0x5FF610
int dword_5FF610;

// 601---

// 0x601740
int dword_601740;

// 0x601778
int dword_601778;

// 0x601834
int dword_601834;

// 0x6017F8
int dword_6017F8;

// 0x6017D8
int dword_6017D8;

// 0x60184C
int dword_60184C;

// 0x6018B8
void* off_6018B8;

// 0x6018C8
int dword_6018C8;

// 0x6018D4
int description_mes_;

// 602---

// 0x6028EC
int dword_6028EC;

// 603---

// 0x603DD0
void* off_603DD0;

// 638---

// 687---

// 0x6876D0
int* off_6876D0;

// 0x5BE87C
int dword_5BE87C = 1;

// 0x5BEA38
float flt_5BEA38 = 0.3;

// 0x5C0648
int dword_5C0648 = 127;

// 0x5C064C
int dword_5C064C = -1;

// 0x5C22AC
char byte_5C22AC = ',';

// 0x5C26F8
int dword_5C26F8 = -1;

// 0x5C26FC
int dword_5C26FC = -1;

// 0x60289C
void* off_60289C;

// 0x6028F0
void* off_6028F0;

// 0x602E60
void* off_602E60;

// 0x602EA8
int dword_602EA8;

// 0x603408
int dword_603408;

// 0x603674
int dword_603674;

// 0x603680
int dword_603680;

// 0x6036A8
int dword_6036A8;

// 0x603AC0
void* off_603AC0;

// 0x603B4C
int dword_603B4C;

// 0x603B60
int* off_603B60;

// 0x603D38
int dword_603D38;

// 0x603D40
int dword_603D40;

// 0x603D44
int dword_603D44;

// 0x603D48
int dword_603D48;

// 0x603DE4
void* off_603DE4;

// 0x603E00
int dword_603E00;

// 0x604670
int dword_604670;

// 0x604744
int dword_604744;

// 0x60F244
int dword_60F244;

// 0x60F254
HWND hWnd;

// 0x62AC4C
int dword_62AC4C;

// 0x62AC50
int dword_62AC50;

// 0x62AC54
int dword_62AC54;

// 0x62AC58
int dword_62AC58;

// 0x62AC60
int dword_62AC60;

// 0x62AC64
int dword_62AC64;

// 0x62AC68
int dword_62AC68;

// 0x62AC6C
unsigned int dword_62AC6C;

// 0x62AC88
int dword_62AC88;

// 0x62B09C
char byte_62B09C[29];

// 0x62B270
int dword_62B270;

// 0x62B2A4
void* off_62B2A4;

// 0x62B2BC
int dword_62B2BC;

// 0x62B2C0
int dword_62B2C0;

// 0x6301F4
int dword_6301F4;

// 0x630200
int dword_630200;

// 0x630CFC
int dword_630CFC;

// 0x630D54
void* off_630D54;

// 0x63650C
int dword_63650C;

// 0x638BB0
int dword_638BB0;

// 0x638BB4
int dword_638BB4;

// ------

// 0x4027B0
char* sub_4027B0()
{
    return arcanum;
}

// 0x402CA0
bool gamelib_in_reset_()
{
    return in_gamelib_reset_;
}

// #maybe-bool
// 0x402CB0
int sub_402CB0()
{
    return 0;
}

// 0x402F90
void sub_402F90()
{
    dword_59ADD8--;
}

// 0x402FA0
void sub_402FA0()
{
    dword_59ADD8++;
}

// 0x402FE0
char* sub_402FE0()
{
    return byte_59ADDC;
}

// 0x4038D0
int sub_4038D0()
{
    return dword_5D10E4;
}

// 0x4038E0
void sub_4038E0(int a1)
{
    dword_5D10D4 = a1;
}

// 0x4038F0
void sub_4038F0(int a1)
{
    dword_5D10D8 = a1;
}

// 0x404590
int get_game_difficulty_()
{
    return game_difficulty_;
}

// 0x404640
void nullsub_1()
{
}

// 0x408710
int sub_408710(int a1)
{
    return sub_4E4E60(a1);
}

// 0x408720
int sub_408720(int a1, int a2)
{
    return sub_4E4F80(a1, a2);
}

// 0x408740
int sub_408740(int a1, int a2)
{
    return nullsub_13(a1, a2);
}

// 0x40C5B0
void sub_40C5B0(void* a1)
{
    free(a1->field_48);
}

// 0x40C640
void sub_40C640(void* a1)
{
    free(a1->field_4C);
}

// 0x40E000
void scroll_exit_()
{
}

// 0x40E010
void scroll_reset_()
{
    dword_5D11C8 = 0;
}

// 0x40E890
void sub_40E890(int a1)
{
    dword_59F050 = 1000 / a1;
}

// 0x40E8A0
void sub_40E8A0(int a1)
{
    dword_5D11C4 = a1;
}

// 0x40E900
void sub_40E900(int a1)
{
    dword_5D11C8 = a1;
}

// 0x40FED0
void nullsub_2()
{
}

// 0x40FF40
int sub_40FF40()
{
    return dword_5D1210;
}

// 0x410060
int sub_410060()
{
    return dword_5D1208;
}

// 0x410270
int sub_410270()
{
    return dword_5D11FC;
}

// 0x417D80
int sub_417D80(void* fp)
{
    return tio_fclose(fp);
}

// #maybe-void
// 0x41B2D0
int j_sound_game_reset_()
{
    return sound_game_reset_();
}

// 0x41B3A0
int sub_41B3A0()
{
    return sub_533BF0(sub_41B3B0);
}

// 0x41C8A0
int get_effects_volume_()
{
    return effects_volume_;
}

// 0x41C920
int get_voice_volume_()
{
    return voice_volume_;
}

// 0x41C970
int get_music_volume_()
{
    return music_volume_;
}

// 0x421C20
void anim_reset_()
{
    dword_5E3500 = 0;
}

// 0x4232F0
void sub_4232F0()
{
}

// 0x423530
void nullsub_29()
{
}

// 0x423540
void nullsub_30()
{
}

// 0x423550
void nullsub_31()
{
}

// 0x423FB0
void sub_423FB0()
{
    dword_5E3504 = anim_goal_pending_active_goals_count();
}

// 0x423FC0
void sub_423FC0()
{
    dword_5DE6D0 = 1;
}

// 0x423FE0
void sub_423FE0(int a1)
{
    dword_5E34F8 = a1;
}

// 0x4246C0
int sub_4246C0()
{
    return 1;
}

// 0x4246D0
bool sub_4246D0()
{
    return sub_4B6D70() == 0;
}

// 0x425740
bool sub_425740(void* a1)
{
    return (a1->field_C00 & 1) == 0;
}

// 0x427710
int sub_427710()
{
    return 0;
}

// 0x427720
int sub_427720()
{
    return 1;
}

// 0x428890
int sub_428890()
{
    return 0;
}

// 0x429370
int sub_429370()
{
    return 0;
}

// 0x429380
int sub_429380()
{
    return 1;
}

// 0x429390
int sub_429390()
{
    return 1;
}

// 0x4293A0
int sub_4293A0()
{
    return 1;
}

// 0x4293B0
int sub_4293B0()
{
    return 0;
}

// 0x4293C0
int sub_4293C0()
{
    return 0;
}

// 0x429420
int sub_429420()
{
    return 0;
}

// 0x429430
int sub_429430()
{
    return 0;
}

// 0x429440
int sub_429440()
{
    return 1;
}

// 0x429B40
int sub_429B40()
{
    return 1;
}

// 0x429BB0
int sub_429BB0()
{
    return 1;
}

// 0x42A430
int sub_42A430(void* a1)
{
    return (a1->field_C >> 18) & 1;
}

// #not-sure
// 0x42CA90
int sub_42CA90(void* a1)
{
    return ((~a1->field_12) >> 4) & 1;
}

// 0x42FEA0
int sub_42FEA0()
{
    return 1;
}

// 0x42FEB0
int sub_42FEB0()
{
    return 0;
}

// 0x42FEC0
int sub_42FEC0()
{
    return 0;
}

// 0x437980
void nullsub_32()
{
}

// 0x437CF0
int sub_437CF0()
{
    return 1;
}

// 0x437E00
void wallcheck_exit_()
{
    wallcheck_reset_();
    dword_5E0A00 = 0;
}

// 0x4384F0
void j_wallcheck_reset_()
{
    wallcheck_reset_();
}

// 0x438520
int sub_438520()
{
    return dword_5A3E90;
}

// 0x438AE0
void config_init_(void* a1, int a2)
{
    a1->field_0 = a2;
    a1->field_4 = 0;
}

// 0x4390F0
void roof_resize_(void* a1)
{
    dword_5E2E44 = a1->field_0;
}

// 0x43A130
int sub_43A130()
{
    return dword_5A53A0;
}

// 0x43AA60
void object_close_()
{
    object_reset_();
}

// 0x441780
int sub_441780(void* a1)
{
    free(a1);
    return 1;
}

// 0x441960
int sub_441960(void* a1)
{
    free(a1);
    return 1;
}

// 0x442040
int sub_442040()
{
    return dword_5A54AC;
}

// 0x4437C0
int sub_4437C0(int a1, int a2)
{
    return sub_4DA310(a1, a2);
}

// 0x444C80
int sub_444C80(int a1)
{
    return dword_5E2FA0[a1];
}

// 0x445090
int sub_445090()
{
    return dword_5E2FA4;
}

// 0x44C130
void sub_44C130(void* a1)
{
    free(a1);
}

// 0x44CB60
bool sub_44CB60()
{
    return dword_5A5978 != -1;
}

// 0x44D0C0
void nullsub_4()
{
}

// 0x44D0D0
void nullsub_5()
{
}

// 0x44EBD0
void nullsub_6()
{
}

// 0x44EBE0
void nullsub_7()
{
}

// 0x44EEB0
void anim_stat_()
{
}

// 0x44F150
void magictech_reset_()
{
    dword_5E75FC = 0;
}

// 0x44FE20
void sub_44FE20()
{
    dword_5E75FC = 1;
}

// 0x4502E0
int sub_4502E0()
{
    return 5;
}

// 0x452F00
void nullsub_9()
{
}

// 0x452F10
void sub_452F10()
{
    tig_debug_printf_("MagicTech: Process: Component: Error:  Invalid Component Type!\n");
}

// 0x457100
void sub_457100()
{
    sub_455710();
}

// 0x458B60
int sub_458B60(int a1)
{
    return sub_4B1600(a1);
}

// 0x459A20
void nullsub_10()
{
}

// 0x459F50
void j_mt_ai_reset_()
{
    mt_ai_reset_();
}

// 0x45A8D0
int sub_45A8D0()
{
    return dword_5E8600 + 1;
}

// #maybe-unsigned
// 0x45A8E0
int sub_45A8E0(void* a1)
{
    return a1->field_0 % 30 + 1;
}

// 0x45AD10
void sub_45AD10(int a1)
{
    if (a1 >= 0) {
        dword_5B278C = a1;
    }
}

// 0x45AD70
int sub_45AD70()
{
    return dword_5B2790;
}

// 0x45AD80
int sub_45AD80()
{
    return 1;
}

// 0x45AEC0
void timeevent_exit_()
{
    dword_5E8614 = 0;
    sub_45BC20();
}

// 0x45B360
void sub_45B360()
{
    dword_5E8618 = 0;
    dword_5DE6E0 = 0;
}

// 0x45B600
void sub_45B600(void* a1)
{
    free(a1);
}

// 0x45BA20
void* sub_45BA20()
{
    return malloc(216);
}

// 0x45F550
int sub_45F550(int a1, int a2)
{
    return sub_4AEB10(a1, a2);
}

// 0x45F710
int sub_45F710(int a1, int a2)
{
    return sub_45F6D0(a1, a2);
}

// 0x45F920
void sub_45F920()
{
    dword_5E8638--;
}

// 0x45F960
int sub_45F960(int a1)
{
    return dword_5B306C[a1];
}

// 0x460150
void sub_460150()
{
    sub_534370(dword_5E87C8);
}

// 0x4601C0
void sub_4601C0()
{
    if (off_5E8658 != NULL) {
        off_5E8658();
    }
}

// 0x4604C0
void sub_4604C0(int a1)
{
    if (off_5E86A8 != NULL) {
        off_5E86A8(a1);
    }
}

// 0x4604E0
void sub_4604E0()
{
    if (off_5E86AC != NULL) {
        off_5E86AC();
    }
}

// 0x460520
void sub_460520()
{
    if (off_5E86B4 != NULL) {
        off_5E86B4();
    }
}

// 0x460530
void sub_460530()
{
    if (off_5E86B8 != NULL) {
        off_5E86B8();
    }
}

// 0x460540
void sub_460540(int a1)
{
    if (off_5E86BC != NULL) {
        off_5E86BC(a1);
    }
}

// 0x4605C0
void sub_4605C0()
{
    if (off_5E86C8 != NULL) {
        off_5E86C8();
    }
}

// 0x4605D0
void sub_4605D0()
{
    if (off_5E86CC != NULL) {
        off_5E86CC();
    }
}

// 0x460610
void sub_460610(int a1)
{
    if (off_5E86D4 != NULL) {
        off_5E86D4(a1);
    }
}

// 0x460630
void sub_460630(int a1)
{
    if (off_5E86D8 != NULL) {
        off_5E86D8(a1);
    }
}

// 0x4606C0
void sub_4606C0(int a1)
{
    if (off_5E86DC != NULL) {
        off_5E86DC(a1);
    }
}

// 0x4606E0
void sub_4606E0()
{
    if (off_5E86E0 != NULL) {
        off_5E86E0();
    }
}

// 0x460770
void sub_460770()
{
    if (off_5E86F0 != NULL) {
        off_5E86F0();
    }
}

// 0x460780
void sub_460780()
{
    if (off_5E86F4 != NULL) {
        off_5E86F4();
    }
}

// 0x4607B0
void sub_4607B0(int a1)
{
    if (off_5E86FC != NULL) {
        off_5E86FC(a1);
    }
}

// 0x460860
void sub_460860()
{
    if (off_5E8710 != NULL) {
        off_5E8710();
    }
}

// 0x460870
void sub_460870(int a1)
{
    if (off_5E8714 != NULL) {
        off_5E8714(a1);
    }
}

// 0x460890
void sub_460890(int a1)
{
    if (off_5E8718 != NULL) {
        off_5E8718(a1);
    }
}

// 0x460960
void sub_460960(int a1)
{
    if (off_5E872C != NULL) {
        off_5E872C(a1);
    }
}

// 0x460AD0
void sub_460AD0(int a1)
{
    if (off_5E8754 != NULL) {
        off_5E8754(a1);
    }
}

// 0x460AF0
void sub_460AF0()
{
    if (off_5E8758 != NULL) {
        off_5E8758();
    }
}

// 0x460B20
void sub_460B20()
{
    if (off_5E8760 != NULL) {
        off_5E8760();
    }
}

// 0x460B80
void sub_460B80(int a1)
{
    if (off_5E876C != NULL) {
        off_5E876C(a1);
    }
}

// 0x460BC0
int sub_460BC0()
{
    if (dword_5E8774 != 0) {
        return off_5E8778();
    } else {
        return 0;
    }
}

// 0x460C20
int sub_460C20()
{
    if (off_5E8784 != NULL) {
        return off_5E8784();
    } else {
        return 0;
    }
}

// 0x460D10
void sub_460D10(int a1)
{
    if (off_5E879C != NULL) {
        off_5E879C(a1);
    }
}

// 0x460D30
void sub_460D30(int a1)
{
    if (off_5E87A0 != NULL) {
        off_5E87A0(a1);
    }
}

// 0x460DD0
void sub_460DD0()
{
    if (off_5E87B4 != NULL) {
        off_5E87B4();
    }
}

// 0x460E10
void sub_460E10(int a1)
{
    if (off_5E87BC != NULL) {
        off_5E87BC(a1);
    }
}

// 0x462920
void sub_462920(void* a1)
{
    if (a1 != NULL) {
        free(a1);
    }
}

// 0x465010
int sub_465010()
{
    return 0;
}

// 0x4655C0
int sub_4655C0()
{
    return dword_5E87E8;
}

// 0x4657D0
int sub_4657D0(int a1)
{
    return off_5E87E0[a1];
}

// 0x465C90
int sub_465C90(int a1)
{
    return dword_5B32C0[a1];
}

// 0x467E70
int sub_467E70()
{
    int v1;

    v1 = dword_5E8820;
    dword_5E8820 = 0;

    return v1;
}

// 0x468180
void sub_468180()
{
    dword_5E8800++;
}

// 0x468190
void sub_468190()
{
    dword_5E8800--;
}

// 0x4681A0
int sub_4681A0()
{
    return dword_5E8800;
}

// 0x468600
int sub_468600(int a1)
{
    return dword_5B37FC[a1];
}

// 0x49CB50
int multiplayer_mod_load_()
{
    return 1;
}

// 0x49CC50
int sub_49CC50()
{
    return sub_527E80();
}

// 0x4A2A30
void sub_4A2A30()
{
    if (off_5F0E04 != NULL) {
        off_5F0E04();
    } else {
        sub_4606E0();
    }
}

// 0x4A2B00
void sub_4A2B00(int a1)
{
    dword_5F0E08 = a1;
}

// 0x4A2BD0
void sub_4A2BD0()
{
    if (dword_5F0E0C > 0) {
        dword_5F0E0C--;
    }
}

// 0x4A3890
void nullsub_39()
{
}

// 0x4A38A0
int sub_4A38A0()
{
    return dword_5F0DE0;
}

// 0x4A40D0
bool sub_4A40D0(int a1)
{
    return off_5F0BC8[a1] != NULL;
}

// 0x4A4230
void* sub_4A4230(int a1)
{
    return off_5F0BC8[a1];
}

// 0x4A4270
void sub_4A4270()
{
    dword_5F0E10++;
}

// 0x4A4280
void sub_4A4280()
{
    dword_5F0E10--;
}

// 0x4A4EB0
int sub_4A4EB0(int a1)
{
    return sub_4F5820(a1);
}

// 0x4A4EC0
int sub_4A4EC0()
{
    return sub_4F5910();
}

// 0x4A4ED0
int sub_4A4ED0()
{
    return sub_4F59E0();
}

// 0x4A4EE0
int sub_4A4EE0()
{
    return sub_4F6130();
}

// 0x4A4F10
int sub_4A4F10(int a1, int a2)
{
    return sub_4F5A90(a1, a2);
}

// 0x4A4F50
int sub_4A4F50(int a1)
{
    return sub_4F5B70(a1);
}

// 0x4A4F80
int sub_4A4F80()
{
    return sub_4F5C10();
}

// 0x4A4F90
int sub_4A4F90()
{
    return sub_4F5C50();
}

// 0x4A4FA0
int sub_4A4FA0(int a1)
{
    return sub_4F60E0(a1);
}

// 0x4A4FB0
int sub_4A4FB0(int a1, int a2)
{
    return sub_4F5C90(a1, a2);
}

// 0x4A4FD0
int sub_4A4FD0(int a1)
{
    return sub_4F5CE0(a1);
}

// 0x4A4FE0
int sub_4A4FE0(int a1, int a2)
{
    return sub_4F5D30(a1, a2);
}

// 0x4A5000
int sub_4A5000(int a1)
{
    return sub_4F5D80(a1);
}

// 0x4A5010
int sub_4A5010(int a1)
{
    return sub_4F5DD0(a1);
}

// 0x4A5020
int sub_4A5020(int a1, int a2)
{
    return sub_4F5E20(a1, a2);
}

// 0x4A5040
int sub_4A5040(int a1)
{
    return sub_4F5E70(a1);
}

// 0x4A5050
int sub_4A5050(int a1, int a2)
{
    return sub_4F5EC0(a1, a2);
}

// 0x4A5070
int sub_4A5070(int a1, int a2)
{
    return sub_4F5FA0(a1, a2);
}

// 0x4A5090
int sub_4A5090(int a1)
{
    return sub_4F5FF0(a1);
}

// 0x4A50A0
int sub_4A50A0(int a1, int a2)
{
    return sub_4F6040(a1, a2);
}

// 0x4A50C0
int sub_4A50C0(int a1)
{
    return sub_4F6090(a1);
}

// 0x4A5490
void multiplayer_ping_()
{
    if (sub_4A4ED0()) {
        sub_4A4EE0();
    }
}

// 0x4A55F0
int sub_4A55F0()
{
    return dword_5B3FD8;
}

// 0x4A5600
void sub_4A5600(int a1)
{
    dword_5B3FD8 = a1;
}

// 0x4A7C80
int light_scheme_reset_()
{
    return sub_4A7DD0(0, 12);
}

// 0x4A7CF0
void light_scheme_exit_()
{
    free(off_5F5CA0);
}

// 0x4A7DC0
int sub_4A7DC0()
{
    return dword_5B5054;
}

// 0x4A7EA0
int sub_4A7EA0()
{
    return dword_5B5050;
}

// 0x4A7FA0
int sub_4A7FA0()
{
    return dword_5F5CA4;
}

// 0x4A7FB0
int sub_4A7FB0()
{
    return dword_5F5CAC;
}

// 0x4A83F0
void ai_exit_()
{
}

// 0x4A84C0
void ai_mod_unload_()
{
}

// 0x4AFDD0
int tech_exit_()
{
    return message_unload_(dword_5F85C8);
}

// 0x4AFE10
int sub_4AFE10(int a1)
{
    return dword_5F85EC[a1];
}

// 0x4AFE20
int sub_4AFE20(int a1)
{
    return dword_5F85CC[a1];
}

// 0x4AFE30
int sub_4AFE30(int a1)
{
    return dword_5F84A8[a1];
}

// 0x4AFE40
int sub_4AFE40(int a1, int a2)
{
    return dword_5F84C8[8 * a2 + a1];
}

// 0x4B00A0
int sub_4B00A0(int a1)
{
    return dword_5B5124[a1];
}

// 0x4B0110
int sub_4B0110(int a1)
{
    return dword_5B5164[a1];
}

// 0x4B0440
int stat_exit_()
{
    return message_unload_(dword_5F8644);
}

// 0x4B0F80
int sub_4B0F80(int a1)
{
    return dword_5F8648[a1];
}

// 0x4B0F90
int sub_4B0F90(int a1)
{
    return dword_5F86B8[a1];
}

// 0x4B0FA0
int sub_4B0FA0(int a1)
{
    return dword_5F8610[a1];
}

// 0x4B0FB0
int sub_4B0FB0(int a1)
{
    return dword_5F8618[a1];
}

// 0x4B0FC0
int sub_4B0FC0(int a1, int a2, int a3)
{
    return dword_5B5194[a3];
}

// 0x4B1520
void spell_exit_()
{
}

// 0x4B1650
int sub_4B1650()
{
    return 1;
}

// 0x4B1740
int sub_4B1740()
{
    return 100;
}

// 0x4B1750
int sub_4B1750()
{
    return sub_4502E0();
}

// 0x4B1760
int sub_4B1760(int a1)
{
    return sub_4502F0(a1);
}

// 0x4B1A40
int sub_4B1A40(int a1)
{
    return dword_5F8774[a1];
}

// 0x4B1A80
int sub_4B1A80(int a1)
{
    return dword_5B55B0[a1];
}

// 0x4B5F30
int sub_4B5F30(int a1)
{
    return dword_5B5798[a1];
}

// 0x4B6C80
int get_turn_based()
{
    return turn_based_;
}

// 0x4B6D70
int sub_4B6D70()
{
    return dword_5FC22C;
}

// 0x4B7780
int sub_4B7780()
{
    return dword_5FC234;
}

// 0x4B7C20
int sub_4B7C20()
{
    return dword_5FC244;
}

// 0x4B80D0
int sub_4B80D0()
{
    return dword_5FC230;
}

// 0x4B8530
void location_exit_()
{
}

// 0x4B8D40
void sub_4B8D40(int a1)
{
    dword_5FC2F8 = a1;
}

// 0x4B9C20
void gameinit_exit_()
{
    dword_5FC328 = 0;
}

// #maybe-bool
// 0x4B9E10
int party_init_()
{
    party_reset_();
    return 1;
}

// 0x4B9E20
void party_exit_()
{
    party_reset_();
}

// 0x4B9E30
void party_reset_()
{
    // #unwrapped-memset
    for (int index = 0; index < 8; index++) {
        dword_5FC32C[index] = -1;
    }
}

// 0x4BA420
void monstergen_exit_()
{
    free(off_5FC350);
}

// 0x4BAC10
int mt_obj_node_init_()
{
    return 1;
}

// 0x4BAC20
void mt_obj_node_exit_()
{
    sub_4BACD0();
}

// 0x4BAC80
void sub_4BAC80(void* a1)
{
    a1->field_40 = off_5FC368;
    off_5FC368 = a1;
}

// 0x4BB050
void tileblock_exit_()
{
}

// 0x4BB080
void tileblock_resize_(void* a1)
{
    dword_5FC390 = a1->field_0;
}

// 0x4BB8E0
void sub_4BB8E0()
{
    dword_5FC3DC++;
}

// 0x4BB8F0
void sub_4BB8F0()
{
    dword_5FC3DC--;
}

// 0x4BB900
int sub_4BB900()
{
    return dword_5FC3DC;
}

// 0x4BB910
void nullsub_11()
{
}

// 0x4BBC00
void sub_4BBC00()
{
    dword_5FC43C++;
}

// 0x4BBC10
void sub_4BBC10()
{
    dword_5FC43C--;
}

// 0x4BDAB0
void anti_teleport_mod_unload_()
{
    sub_4BDAC0();
    dword_5FC4A8 = 0;
}

// 0x4BDB10
void sub_4BDB10(void* a1)
{
    a1->field_0 = 0;
    a1->field_4 = 0;
}

// 0x4BDC40
void sub_4BDC40(void* a1)
{
    a1->field_0 = 0;
    a1->field_4 = 0;
}

// 0x4BE0E0
void brightness_exit_()
{
}

// 0x4BE1B0
void townmap_init_()
{
    sub_4BED00();
}

// 0x4BF0B0
void newspaper_exit_()
{
    free(off_6876D0);
}

// 0x4BF200
int sub_4BF200(int a1)
{
    return off_6876D0[a1];
}

// 0x4BFDA0
void wp_resize_(void* a1)
{
    dword_5FC660 = a1->field_0;
}

// 0x4C0410
int sector_script_init_()
{
    return 1;
}

// 0x4C0420
void sector_script_reset_()
{
}

// 0x4C0430
void sector_script_exit_()
{
}

// 0x4C05B0
void tile_script_reset_()
{
}

// 0x4C05C0
void tile_script_exit_()
{
    dword_5FC874 = 0;

    void* curr = off_603DD0;
    while (curr != NULL) {
        void* next = curr->field_14;
        free(curr);
        curr = next;
    }

    off_603DD0 = NULL;
}

// 0x4C05D0
void tile_script_resize_(void* a1)
{
    dword_5FC87C = a1->field_0;
}

// 0x4C0C30
void reaction_exit_()
{
    message_unload_(dword_5FC88C);
}

// 0x4C1010
int sub_4C1010(int a1)
{
    return dword_5FC890[a1];
}

// 0x4C2EA0
void sub_4C2EA0(int a1)
{
    dword_5FDC88 = a1;
}

// 0x4C51A0
int sub_4C51A0(int a1)
{
    return off_5FF418[a1 - 1000];
}

// 0x4C5DB0
void skill_exit_()
{
    message_unload_(dword_5FF484);
}

// 0x4C62B0
int sub_4C62B0(int a1)
{
    return dword_5FF454[a1];
}

// 0x4C62C0
int sub_4C62C0(int a1)
{
    return dword_5FF424[a1];
}

// 0x4C62D0
int sub_4C62D0(int a1, int a2, int a3)
{
    return (a3 + 1) * (a2 + 2);
}

// 0x4C64B0
int sub_4C64B0()
{
    return 1;
}

// 0x4C64C0
int sub_4C64C0()
{
    return 1;
}

// 0x4C64D0
int sub_4C64D0(int a1)
{
    return dword_5B6F04[a1];
}

// 0x4C6510
int sub_4C6510()
{
    return 100;
}

// 0x4C69A0
int sub_4C69A0(int a1)
{
    return dword_5FF4E8[a1];
}

// 0x4C69B0
int sub_4C69B0(int a1)
{
    return dword_5FF488[a1];
}

// 0x4C69C0
int sub_4C69C0(int a1)
{
    return dword_5B6F44[a1];
}

// 0x4C69D0
int sub_4C69D0(int a1)
{
    return dword_5FF498[a1];
}

// 0x4C69E0
int sub_4C69E0(int a1, int a2, int a3)
{
    return (a3 + 1) * (a2 + 2);
}

// 0x4C6AF0
int sub_4C6AF0()
{
    return 1;
}

// 0x4C6B00
int sub_4C6B00()
{
    return 1;
}

// 0x4C6B10
int sub_4C6B10(int a1)
{
    return dword_5B6F34[a1];
}

// 0x4C9DE0
void facade_exit_()
{
    sub_4CA240();
    dword_5FF590 = 0;
}

// 0x4C9DF0
void facade_resize_(void* a1)
{
    dword_5FF58C = a1->field_0;
}

// 0x4CA940
int area_init_()
{
    dword_5FF5A8 = 0;
    return 1;
}

// 0x4CA9A0
void area_exit_()
{
}

// #maybe-size_t
// 0x4CAE80
int sub_4CAE80()
{
    return dword_5FF5A8;
}

// 0x4CB720
int mt_item_init_()
{
    dword_5FF610 = 1;
    return 1;
}

// #signness
// 0x4CB790
int sub_4CB790(int a1)
{
    return a1 & 0xFF;
}

// 0x4CC210
void mt_ai_reset_()
{
    sub_4CC4C0();
}

// 0x4CCBF0
void nullsub_12()
{
}

// 0x4CCCF0
int animfx_init_()
{
    dword_601740 = 1;
    return 1;
}

// 0x4CEEB0
int random_init_()
{
    sub_4CEEE0();
    return 1;
}

// 0x4CEEC0
void random_exit_()
{
}

// 0x4CEED0
void sub_4CEED0(int a1)
{
    sub_4CEF40(a1);
}

// 0x4CEF40
void sub_4CEF40(int a1)
{
    dword_601778 = a1;
}

// 0x4CF0D0
void sector_reset_()
{
    sub_4D0B40();
    dword_601834 = 0;
}

// 0x4CF2C0
void sector_resize_(void* a1)
{
    dword_6017F8 = a1->field_0;
}

// 0x4D0F40
void sub_4D0F40()
{
    sub_4D0F70();
}

// 0x4D1040
void sub_4D1040()
{
    dword_5B7CD8++;
}

// 0x4D1050
void sub_4D1050()
{
    dword_5B7CD8--;
}

// 0x4D10C0
int gl_sector_init_()
{
    sub_4D1100();
    return 1;
}

// 0x4D10D0
void gl_sector_reset_()
{
    dword_601834 = 0;
}

// 0x4D2F80
void sub_4D2F80()
{
    dword_6017D8 = 0;
}

// 0x4D2FB0
int sub_4D2FB0(const void* a1, const void* a2)
{
    return a1->field_0 - a2->field_0;
}

// 0x4D3320
void teleport_exit_()
{
    teleport_reset_();
}

// 0x4D3410
int sub_4D3410()
{
    return dword_60184C;
}

// 0x4D3EE0
void sub_4D3EE0(void* a1)
{
    a1->field_10 = off_6018B8;
    off_6018B8 = a1;
}

// 0x4D3FE0
void item_effect_exit_()
{
    message_unload_(dword_6018C8);
}

// 0x4D4150
void description_exit_()
{
    message_unload_(description_mes_);
}

// 0x4D4C90
int message_file_close_(void* a1)
{
    return tio_fclose(a1);
}

// 0x4D5090
void tf_reset_()
{
    sub_4D5780();
}

// 0x4D5150
void tf_close_()
{
    sub_4D5780();
}

// 0x4D5160
int sub_4D5160(int a1)
{
    sub_4D5B10(a1);
    return dword_6028EC;
}

// 0x4D5180
int sub_4D5180()
{
    return dword_6028EC;
}

// 0x4D5850
void sub_4D5850(void* a1)
{
    a1->field_C = off_60289C;
    off_60289C = a1;
}

// 0x4D59D0
void sub_4D59D0(void* a1)
{
    a1->field_1C = off_6028F0;
    off_6028F0 = a1;
}

// 0x4D5DB0
void tb_reset_()
{
    sub_4D6320();
}

// 0x4D5E80
void tb_close_()
{
    sub_4D6320();
}

// 0x4D8500
int sub_4D8500()
{
    return dword_602EA8;
}

// 0x4D8530
int sub_4D8530()
{
    return dword_603408;
}

// 0x4D94B0
int sub_4D94B0(void* a1)
{
    return a1->field_18 & 0x80;
}

// 0x4D94C0
void sub_4D94C0(void* a1)
{
    a1->field_18 &= ~0x80;
}

// 0x4D94D0
int sub_4D94D0(int a1, int a2)
{
    return sub_4DDD20(a1, a2);
}

// 0x4D94F0
int sub_4D94F0(int a1, int a2)
{
    return sub_4DDD70(a1, a2);
}

// #maybe-void
// 0x4DD130
int sub_4DD130(int a1, int a2)
{
    return sub_4DE290(a1, a2);
}

// 0x4DD310
int sub_4DD310(void* a1)
{
    return a1->field_18;
}

// 0x4DDA60
int64_t sub_4DDA60(void* a1)
{
    return a1->field_8;
}

// 0x4DDB70
int sub_4DDB70(void* a1)
{
    return a1->field_1C;
}

// 0x4DE7A0
void sub_4DE7A0(void* a1)
{
    a1->field_C = off_602E60;
    off_602E60 = a1;
}

// 0x4E3020
void jumppoint_reset_()
{
    jumppoint_close_();
}

// 0x4E3030
void jumppoint_exit_()
{
    jumppoint_close_();
    dword_603680 = 0;
}

// 0x4E3040
void jumppoint_resize_(void* a1)
{
    dword_603674 = a1->field_0;
}

// 0x4E3F90
void sub_4E3F90()
{
    dword_6036A8 = 0;
}

// 0x4E4FA0
void nullsub_13()
{
}

// 0x4E52F0
void nullsub_14()
{
}

// 0x4E5900
int64_t sub_4E5900(int64_t a1)
{
    return a1 >> 29;
}

// #unclear
// 0x4E59A0
int sub_4E59A0(int* a1)
{
    return *a1 >> 8;
}

// 0x4E61E0
int sub_4E61E0(int a1)
{
    return a1 / 32;
}

// 0x4E77A0
int sub_4E77A0(void* a1)
{
    return a1->field_0->field_4;
}

// 0x4E7A50
int sub_4E7A50(int a1)
{
    return a1 + 12;
}

// 0x4E7A60
int sub_4E7A60(void* a1)
{
    return a1->field_0 * a1->field_4 + 12;
}

// 0x4E7B50
void terrain_reset_()
{
}

// 0x4E8DC0
int sub_4E8DC0(int a1)
{
    return (a1 >> 11) & 0x1F;
}

// 0x4E8DD0
int sub_4E8DD0(int a1)
{
    return (a1 >> 6) & 0x1F;
}

// 0x4E8DE0
int sub_4E8DE0(int a1)
{
    return (a1 >> 2) & 0xF;
}

// 0x4E8DF0
int sub_4E8DF0(char a1)
{
    return a1 & 3;
}

// 0x4E9920
int object_node_init_()
{
    return 1;
}

// 0x4E9930
void object_node_exit_()
{
    sub_4E99C0();
}

// 0x4E9970
void sub_4E9970(void* a1)
{
    a1->field_8 = off_603AC0;
    off_603AC0 = a1;
}

// 0x4EC610
void sub_4EC610()
{
    message_unload_(dword_603B4C);
}

// 0x4ECC00
int sub_4ECC00(int a1)
{
    return off_603B60[a1];
}

// 0x4EF1D0
int sub_4EF1D0(void* a1)
{
    return sub_502290(a1->field_4);
}

// 0x4F1200
int sub_4F1200(int a1)
{
    return sub_4F1170(a1);
}

// 0x4F2560
void target_exit_()
{
    dword_603D38 = 0;
}

// 0x4F2570
void target_reset_()
{
    dword_603D40 = 0;
    dword_603D44 = 0;
}

// 0x4F5320
void sub_4F5320(int a1)
{
    dword_603D48 = a1;
}

// 0x4F6170
int sub_4F6170(int a1)
{
    return sub_4F6180(a1);
}

// 0x4F61B0
int sub_4F61B0(int a1)
{
    return sub_4F6180(a1);
}

// 0x4F62A0
int sub_4F62A0(void* a1)
{
    return a1->field_0 & 1;
}

// 0x4F6310
void nullsub_26()
{
}

// 0x4F6360
void sub_4F6360(void* a1)
{
    a1->field_14 = off_603DD0;
    off_603DD0 = a1;
}

// 0x4F6460
int sub_4F6460(int a1)
{
    return sub_4F6400(a1);
}

// 0x4F66C0
int sub_4F66C0(void* a1)
{
    return a1->field_0 & 1;
}

// 0x4F71B0
int sub_4F71B0(int a1)
{
    return sub_4F7130(a1);
}

// 0x4F7B40
void sub_4F7B40(void* a1, void* a2)
{
    a2->field_4 = a1->field_0;
    a1->field_0 = a2;
}

// 0x4F7C20
int sub_4F7C20()
{
    return 1;
}

// 0x4F7C80
int sub_4F7C80(int a1, int a2)
{
    return sub_4F7D40(a1, a2);
}

// 0x4F7CA0
int sub_4F7CA0(int a1, int a2)
{
    return sub_4F7DF0(a1, a2);
}

// 0x4F7F70
int sub_4F7F70()
{
    return 1;
}

// 0x4F7FC0
int sub_4F7FC0()
{
    return 0;
}

// 0x4F7FD0
int sub_4F7FD0()
{
    return 1;
}

// 0x4F7FE0
int sub_4F7FE0()
{
    return 1;
}

// 0x4F80E0
int sub_4F80E0(void* a1)
{
    sub_4F8120(a1);
    return 1;
}

// 0x4F8100
int sub_4F8100(void* a1)
{
    sub_4F8120(a1);
    return 1;
}

// 0x4F8120
void sub_4F8120(void* a1)
{
    a1->field_0 = 0;
    a1->field_4 = 0;
    a1->field_8 = 0;
}

// 0x4F8130
int sub_4F8130(void* a1)
{
    sub_4F8120(a1);
    return 1;
}

// 0x4F81B0
int sub_4F81B0(void* a1)
{
    return a1->field_0 & 0x8000000;
}

// 0x4F81C0
int sub_4F81C0()
{
    return 1;
}

// 0x4F81D0
int sub_4F81D0()
{
    return 1;
}

// 0x4F8330
void nullsub_33()
{
}

// 0x4F8340
void sub_4F8340()
{
    sub_4F83D0();
}

// 0x4F8380
void sub_4F8380(void* a1)
{
    a1->field_4 = off_603DE4;
    off_603DE4 = a1;
}

// 0x4FB100
bool sub_4FB100(void* a1)
{
    return a1->field_0 == 1;
}

// 0x4FB270
void* sub_4FB270(int a1, int a2, int a3)
{
    return malloc(a3 * a2);
}

// 0x4FB290
void sub_4FB290(void* a1)
{
    free(a1);
}

// 0x4FB320
void nullsub_28()
{
}

// 0x4FE790
void sub_4FE790(int a1)
{
    dword_603E00 = a1;
}

// 0x4FEB60
void tig_debug_exit_()
{
}

// 0x4FEE50
void sub_4FEE50(const char* string)
{
    OutputDebugStringA(string);
}

// 0x4FF340
void nullsub_34()
{
}

// 0x500150
int sub_500150()
{
    return dword_604670;
}

// 0x500560
int sub_500560()
{
    return 0;
}

// 0x500570
void nullsub_15()
{
}

// 0x500580
void sub_500580(int a1)
{
    dword_5BE87C = a1;
}

// 0x5006D0
void nullsub_24()
{
}

// 0x5022B0
void sub_5022B0(int a1)
{
    dword_604744 = a1;
}

// 0x502700
unsigned int sub_502700(unsigned int a1)
{
    return a1 >> 28;
}

// #check
// 0x51AC00
void sub_51AC00(int a1)
{
    // #check: probably / 100.0 or * 0.01
    flt_5BEA38 = (double)a1 * 0.0099999998;
}

// 0x51E2C0
int sub_51E2C0(int a1)
{
    return a1;
}

// 0x51E2D0
int sub_51E2D0(int a1)
{
    return a1;
}

// 0x51F320
int sub_51F320()
{
    return dword_60F244;
}

// 0x5242F0
void sub_5242F0()
{
    hWnd = NULL;
}

// 0x524EF0
void nullsub_35()
{
}

// 0x526AB0
int sub_526AB0(int a1, int a2)
{
    return _WSAFDIsSet(a1, a2);
}

// 0x526AD0
void nullsub_16()
{
}

// 0x526B00
bool sub_526B00(int a1)
{
    return a1 == stru_614848.field_0;
}

// 0x526E00
void sub_526E00()
{
    // #check
    dword_62AC6C &= 0xF4;
}

// 0x527170
void nullsub_36()
{
}

// 0x527280
void sub_527280()
{
    // #check
    dword_62AC6C |= 1;
}

// 0x5280E0
void sub_5280E0()
{
    // #check
    dword_62AC6C |= 2;
}

// 0x528710
void sub_528710(int a1)
{
    dword_62AC54 = a1;
}

// 0x528720
void sub_528720(int a1)
{
    dword_62AC58 = a1;
}

// 0x528740
void sub_528740(int a1)
{
    dword_62AC60 = a1;
}

// 0x528750
void sub_528750(int a1)
{
    dword_62AC64 = a1;
}

// 0x528760
void sub_528760()
{
    dword_62AC6C |= 0x10;
}

// 0x528770
void sub_528770()
{
    dword_62AC6C &= ~0x10;
}

// 0x528780
int sub_528780()
{
    return dword_62AC6C & 0x10;
}

// 0x528790
void sub_528790()
{
    dword_62AC4C++;
}

// 0x5287A0
void sub_5287A0()
{
    dword_62AC4C--;
}

// 0x529330
int sub_529330()
{
    return stru_6123C0.field_3C;
}

// 0x529430
void sub_529430()
{
    sub_5293C0(0);
}

// 0x5294A0
void sub_5294A0(int* a1)
{
    if (a1 != NULL) {
        *a1 = stru_6123C0.field_30;
    }
}

// 0x5294C0
void sub_5294C0(int a1)
{
    stru_6123C0.field_30 = a1;
}

// 0x5294D0
int sub_5294D0()
{
    return stru_6123C0.field_5C;
}

// 0x5294E0
void sub_5294E0(int a1)
{
    stru_6123C0.field_5C |= a1;
}

// 0x52A4C0
void sub_52A4C0()
{
    // #check
    dword_62AC6C |= 8;
}

// 0x52A530
int sub_52A530()
{
    return dword_5C064C;
}

// 0x52A7A0
void sub_52A7A0(int a1)
{
    dword_5C0648 |= a1;
}

// 0x52A940
void sub_52A940()
{
    dword_62AC88++;
}

// 0x52A950
void sub_52A950()
{
    dword_62AC88--;
}

// 0x52AD80
void sub_52AD80()
{
    dword_62AC6C |= 0x20;
}

// 0x52B1E0
void sub_52B1E0(int a1)
{
    dword_62AC68 = a1;
}

// 0x52B1F0
u_long sub_52B1F0(u_long a1)
{
    return ntohl(a1);
}

// 0x52B210
void sub_52B210()
{
    tig_timer_start_(&stru_6123C0.field_60);
}

// 0x52B2C0
void sub_52B2C0()
{
    dword_62AC50++;
}

// 0x52B2D0
void sub_52B2D0()
{
    dword_62AC50--;
}

// 0x52B340
int sub_52B340(int a1)
{
    return byte_62B09C[a1] & 0x80;
}

// 0x52BD90
void sub_52BD90(int a1)
{
    dword_62B270 = a1;
}

// 0x52C330
int sub_52C330(int a1)
{
    return sub_52C760(a1, 1, 0);
}

// 0x52C350
int sub_52C350(int a1)
{
    return sub_52C760(a1, 2, 0);
}

// 0x52C500
int sub_52C500(int a1)
{
    return sub_52C760(a1, 0, 3);
}

// 0x52C520
int sub_52C520(int a1)
{
    return sub_52C760(a1, 1, 3);
}

// 0x52C540
int sub_52C540(int a1)
{
    return sub_52C760(a1, 2, 3);
}

// 0x52D0E0
int tig_init_3_()
{
    return 0;
}

// 0x52D180
void sub_52D180(void* a1)
{
    a1->field_10 = off_62B2A4;
    off_62B2A4 = a1;
}

// 0x52DF80
int tig_timer_init_()
{
    if (timeBeginPeriod(1u) != 0) {
        return 16;
    }

    return 0;
}

// 0x52DF90
void tig_timer_exit_()
{
    timeEndPeriod(1u);
}

// 0x52DFA0
int tig_timer_start_(unsigned int* start)
{
    *start = timeGetTime();
    return 0;
}

// 0x52DFB0
int tig_timer_elapsed_(unsigned int start)
{
    return timeGetTime() - start;
}

// 0x52DFC0
unsigned int tig_timer_between_(unsigned int start, unsigned int end)
{
    unsigned int diff = end - start;
    if (diff > INT_MAX) {
        diff = INT_MAX;
    }
    return diff;
}

// 0x52DFE0
bool sub_52DFE0(int a1)
{
    return sub_52F100(a1) == 0;
}

// 0x530320
char* tio_tmpnam(char* dest)
{
    return tmpnam(dest);
}

// 0x5304B0
int sub_5304B0()
{
    return -1;
}

// 0x5304FD
int sub_5304FD()
{
    return -1;
}

// 0x530BF0
void tio_destroy_(void* fp)
{
    free(fp);
}

// 0x531520
void tig_movie_exit_()
{
}

// 0x5317C0
int sub_5317C0()
{
    return dword_62B2BC;
}

// 0x5318C0
int tig_init_19_()
{
    byte_5C22AC = 44;
    return 0;
}

// 0x5318D0
void tig_exit_19_()
{
}

// 0x5318E0
void sub_5318E0(char a1)
{
    byte_5C22AC = a1;
}

// 0x533000
void sub_533000(int a1)
{
    dword_62B2C0 = a1;
}

// 0x533080
int sub_533080(int a1, int a2)
{
    return sub_533020(a1, a2);
}

// 0x533C30
void sub_533C30(int a1)
{
    dword_6301F4 = a1;
}

// 0x534290
int sub_534290()
{
    return dword_630200 + 4;
}

// 0x534990
int sub_534990(void* a1)
{
    return a1->field_C;
}

// 0x534A40
void nullsub_17()
{
}

// 0x535130
void sub_535130(void* a1)
{
    memset(a1, 0, 28);
}

// 0x5352C0
void tig_exit_17_()
{
    sub_535380();
    sub_535330(off_630D54);
}

// 0x535330
void sub_535330(void* a1)
{
    free(a1);
}

// 0x535380
void sub_535380()
{
    if (dword_630CFC > 0) {
        dword_630CFC--;
    }
}

// 0x537FC0
int sub_537FC0(int a1)
{
    return a1;
}

// 0x537FD0
int sub_537FD0(int a1)
{
    return a1;
}

// 0x5387D0
void sub_5387D0()
{
    dword_5C26F8 = -1;
    dword_5C26FC = -1;
}

// 0x538A80
int tig_init_21_()
{
    return 0;
}

// 0x538A90
void tig_exit_21_()
{
}

// 0x538D80
int sub_538D80(int a1)
{
    return sub_538D90(a1);
}

// 0x538E90
void sub_538E90(int a1, void* a2)
{
    a2->field_10++;
}

// 0x538EE0
int tig_init_18_()
{
    return 0;
}

// 0x538EF0
void tig_exit_18_()
{
}

// 0x538F40
void sub_538F40(void* a1)
{
    dword_63650C = a1->field_0;
}

// 0x538F50
void sub_538F50(void* a1)
{
    *a1 = dword_63650C;
}

// 0x539EC0
int sub_539EC0(int a1)
{
    return a1;
}

// 0x539ED0
int sub_539ED0(int a1)
{
    return a1;
}

// 0x53A020
int sub_53A020(void* a1)
{
    return tig_window_destroy(a1->field_10);
}

// 0x53AB00
int sub_53AB00()
{
    dword_638BB0 = 0;
    dword_638BB4 = 0;
    return 1;
}

// 0x53AB20
int sub_53AB20(void)
{
    return sub_53AB30();
}

// 0x53C1C0
int sub_53C1C0(void* a1)
{
    return a1->field_8->field_8;
}

// 0x53C370
int sub_53C370()
{
    return 1;
}

// 0x53C520
int sub_53C520(int a1, int a2, int a3)
{
    return a3 - 1;
}

// 0x53C710
void sub_53C710(int* a1)
{
    *a1 &= ~6u;
}

// 0x53C720
int sub_53C720(int* a1)
{
    return *a1 & 2;
}

// 0x53C730
int sub_53C730(int* a1)
{
    return *a1 & 4;
}

// 0x53EB00
void sub_53EB00()
{
    dword_63CBE0 = 0;
}

// 0x53F0D0
int sub_53F0D0()
{
    return dword_64B86C;
}

// 0x5412D0
void sub_5412D0()
{
    sub_5412E0(0);
}

// 0x541680
int sub_541680()
{
    return dword_64C384;
}

// 0x541690
int sub_541690()
{
    return sub_5416A0(5100);
}

// 0x541D70
void sub_541D70()
{
    sub_5894C0();
    sub_550DA0(0, 0);
}

// 0x542280
void sub_542280(int a1)
{
    off_5C3A40[dword_64C414]->field_84 = a1;
}

// 0x5422A0
void sub_5422A0()
{
    off_5C3A40[dword_64C414]->field_54(0);
}

// 0x544690
void nullsub_68()
{
}

// 0x545690
int sub_545690()
{
    return 1;
}

// 0x5456A0
int sub_5456A0()
{
    return 1;
}

// 0x545C50
void sub_545C50()
{
    dword_64C414 = 14;
    sub_546330();
}

// 0x545C60
void sub_545C60()
{
    if (sub_55A220()) {
        sub_55A150();
    }
}

// 0x545E20
void sub_545E20()
{
    dword_64C414 = 15;
    sub_546330();
}

// 0x545E30
void sub_545E30()
{
    if (sub_5735F0()) {
        sub_573440();
    }
}

// 0x546330
void sub_546330()
{
    mainmenu_ui_create_window_func(1);
}

// 0x549520
int sub_549520()
{
    // #wrong
    return dword_64C428 != 0 ? off_5C400C : 0;
}

// 0x5496C0
int sub_5496C0(int a1)
{
    return off_5C3A40[a1];
}

// 0x5496D0
int sub_5496D0()
{
    return dword_64C414;
}

// 0x5496E0
int sub_5496E0()
{
    return sub_5496F0(5060);
}

// 0x549750
int sub_549750()
{
    return sub_5496F0(5062);
}

// 0x549760
int sub_549760()
{
    return sub_5496F0(5065);
}

// 0x549770
int sub_549770()
{
    return sub_5496F0(5061);
}

// 0x549780
int sub_549780()
{
    return sub_5496F0(5063);
}

// 0x549820
int sub_549820()
{
    return dword_5C3624;
}

// 0x549830
void sub_549830(int a1)
{
    dword_64C414 = a1;
}

// 0x549840
int sub_549840()
{
    return dword_5C3FB4;
}

// 0x549A40
void sub_549A40()
{
    dword_64C468++;
}

// 0x549A50
void sub_549A50()
{
    dword_64C468--;
}

// 0x549A60
int sub_549A60()
{
    return dword_64C468;
}

// 0x549A70
void sub_549A70()
{
    dword_5C3620 = 0;
}

// 0x54B3A0
void sub_54B3A0()
{
    sub_54AEE0(0);
    sub_54AEE0(1);
}

// 0x54EB50
int sub_54EB50()
{
    return sub_57DE00();
}

// 0x5507D0
void sub_5507D0(int a1)
{
    dword_64C6D4 = a1;
}

// 0x551A00
int sub_551A00()
{
    return dword_64C634[dword_64C6B8];
}

// 0x551F70
int sub_551F70(int a1)
{
    return dword_5C72B4[a1];
}

// 0x552070
int sub_552070()
{
    return dword_64C6A8;
}

// 0x553370
int sub_553370(void)
{
    return sub_4C9720();
}

// 0x553380
int sub_553380(int a1, int a2)
{
    return sub_4C9810(a1, a2);
}

// 0x5533A0
int sub_5533A0(int a1)
{
    return sub_4C9A10(a1);
}

// 0x5533B0
int sub_5533B0()
{
    return dword_64C6A8;
}

// 0x553A60
void sub_553A60(int a1)
{
    dword_5C71B8 = a1;
}

// 0x556B70
int sub_556B70(int a1)
{
    dword_64C534 = a1;
    sub_556B90(a1);
}

// 0x5572B0
int sub_5572B0()
{
    return tig_window_destroy(dword_64C508);
}

// 0x5572C0
int sub_5572C0()
{
    return 0;
}

// 0x5577D0
int sub_5577D0()
{
    return dword_64C6F0;
}

// 0x5577E0
void sub_5577E0(int a1)
{
    dword_64C6F0 = a1;
}

// 0x5577F0
void intgame_set_width_(int width)
{
    intgame_width_ = width;
}

// 0x557800
void intgame_set_height_(int height)
{
    intgame_height_ = height;
}

// 0x5578C0
int sub_5578C0()
{
    return dword_64C67C;
}

// 0x5578D0
void intgame_set_fullscreen_()
{
    intgame_fullscreen_ = 1;
}

// 0x557AA0
int sub_557AA0()
{
    return dword_64C6A8;
}

// 0x557AB0
int sub_557AB0()
{
    return dword_64C530;
}

// 0x557B10
int sub_557B10()
{
    return intgame_mes_;
}

// 0x557B20
int sub_557B20(int a1)
{
    // #wrong
    return dword_5C6F40[a1];
}

// 0x557B50
int sub_557B50(int a1)
{
    return dword_5C6F60[a1];
}

// 0x557EA0
void iso_exit_()
{
    if (dword_64C708) {
        sub_54A9A0();
    }
    sub_535330(dword_64C70C);
}

// 0x557EC0
void iso_reset_()
{
    sub_54AA30();
    sub_557FD0(0);
}

// 0x558110
int iso_redraw()
{
    int v1 = sub_402E50();
    sub_551080();
    return v1;
}

// 0x5581A0
void* sub_5581A0(void* a1)
{
    return sub_402D30(a1);
}

// 0x5581B0
int wmap_rnd_init_()
{
    return 1;
}

// 0x5581C0
void wmap_rnd_exit_()
{
}

// 0x5581D0
void wmap_rnd_reset_()
{
}

// 0x5589D0
void sub_5589D0()
{
    dword_64C794 = 1;
}

// 0x558AC0
void sub_558AC0(void* a1)
{
    *a1 = 0;
}

// 0x5597B0
void charedit_reset_()
{
    if (sub_55A220()) {
        sub_55A150();
    }
}

// 0x55A220
int sub_55A220()
{
    return dword_64E018;
}

// 0x55A230
int sub_55A230()
{
    if (dword_64E018) {
        sub_55B150();
    }
}

// 0x55EFE0
int sub_55EFE0()
{
    return sub_55EFF0();
}

// 0x55F0D0
int sub_55F0D0()
{
    return sub_55EFB0();
}

// 0x560740
int sub_560740()
{
    return dword_66D9C4;
}

// 0x560750
void sub_560750()
{
    dword_66D9C4 = 0;
}

// 0x560EE0
void nullsub_18()
{
}

// 0x560F90
int sub_560F90()
{
    return dword_66D864;
}

// 0x5627F0
int sub_5627F0()
{
    return 1;
}

// 0x562F90
void sub_562F90(void* a1)
{
    a1->field_0 = 0;
    a1->field_18 = 0;
    a1->field_1C = 0;
}

// 0x563200
int sub_563200()
{
    return 1;
}

// 0x563750
int sub_563750(int a1)
{
    return sub_563790(a1, 4);
}

// 0x563D80
void nullsub_19()
{
}

// 0x5642E0
void nullsub_37()
{
}

// 0x564830
void nullsub_20()
{
}

// 0x5649C0
void nullsub_21()
{
}

// 0x5649D0
void nullsub_22()
{
}

// 0x565130
void nullsub_23()
{
}

// 0x565CF0
int sub_565CF0()
{
    return 1;
}

// 0x566E20
int textedit_ui_init_()
{
    return 1;
}

// 0x566E30
void textedit_ui_reset_()
{
}

// 0x566E40
void textedit_ui_exit_()
{
}

// 0x566ED0
int sub_566ED0()
{
    dword_66DAA8 = 0;
    dword_66DAAC = 0;
    dword_66DAA4 = 0;
}

// 0x566EF0
int sub_566EF0()
{
    return dword_66DAA4;
}

// 0x567320
void sub_567320()
{
    // #todo: dword_66DAA8 is probably some DD object, this is a Release.
    if (dword_66DAA8 != NULL) {
        dword_66DAA8->field_12(dword_66DAA8);
    }
}

// 0x5673A0
void dialog_ui_exit_()
{
    sub_5689B0();
}

// 0x567A10
int sub_567A10()
{
    return dword_67B964;
}

// 0x5681B0
int sub_5681B0()
{
    return 0;
}

// 0x568A30
void compact_ui_reset_()
{
}

// 0x568F20
int sub_568F20()
{
    if (dword_5CA198 != -1) {
        sub_51E950(dword_5CA198);
    }
}

// 0x569040
int sub_569040()
{
    return 1;
}

// 0x569050
void nullsub_72()
{
}

// 0x569060
int sub_569060(int a1)
{
    return sub_54B5D0(a1);
}

// 0x569570
int sub_569570()
{
    return 1;
}

// 0x569580
void nullsub_73()
{
}

// 0x569590
int sub_569590()
{
    return 0;
}

// 0x5695A0
void nullsub_74()
{
}

// 0x5695F0
int sub_5695F0()
{
    return dword_67BB08;
}

// 0x569A90
void sub_569A90()
{
    dword_67B970 = 0;
}

// 0x569AC0
int matchmaker_ui_init_()
{
    dword_67BB0C = 1;
    return 1;
}

// 0x569AD0
void matchmaker_ui_exit_()
{
    dword_67BB0C = 0;
}

// 0x569AE0
void matchmaker_ui_reset_()
{
}

// 0x569B50
int sub_569B50()
{
    sub_569B60();
    return 1;
}

// 0x569C30
int sub_569C30()
{
    return 1;
}

// 0x569C50
int mp_ctrl_ui_init_()
{
    dword_67BB14 = 1;
    return 1;
}

// 0x569C60
void mp_ctrl_ui_exit_()
{
    dword_67BB14 = 0;
}

// 0x569DC0
bool sub_569DC0(void* a1)
{
    return a1->field_10 == 2;
}

// 0x569F20
int sub_569F20(int a1)
{
    dword_5CA350 = a1;
    return 1;
}

// 0x569F30
int sub_569F30()
{
    return sub_569F40(809);
}

// 0x56B280
int sub_56B280()
{
    return sub_551A80(0);
}

// 0x56EBE0
void combat_ui_exit_()
{
    dword_680EB8 = 0;
}

// 0x56EBF0
void combat_ui_reset_()
{
    sub_56EF40();
}

// 0x56ECE0
int sub_56ECE0(int a1)
{
    sub_553990();
    if (a1) {
        sub_560F40();
    }
}

// 0x56EF40
int sub_56EF40()
{
    return sub_56EF50();
}

// 0x56F990
void sub_56F990(int64_t a1)
{
    // #check
    if (a1 != 0) {
        sub_40DA50();
    }
}

// 0x56FBB0
void fate_ui_reset_()
{
    if (dword_680EE8) {
        sub_56FC40();
    }
}

// 0x5700B0
void mutliplayer_ui_reset_()
{
}

// 0x5708B0
int sub_5708B0(int a1)
{
    return sub_570E10(a1);
}

// 0x571880
void broadcast_ui_exit_()
{
    sub_535330(dword_6810C8);
}

// 0x571890
void broadcast_ui_reset_()
{
    byte_681048 = 0;
}

// 0x5719D0
int item_ui_init_()
{
    return 1;
}

// 0x5719E0
void item_ui_exit_()
{
}

// 0x5735F0
int sub_5735F0()
{
    return dword_68139C;
}

// 0x573730
void sub_573730()
{
    dword_68346C = 0;
}

// 0x573840
void sub_573840()
{
    dword_6810E0 = 0;
    dword_6810E4 = 0;
}

// 0x575580
int sub_575580(void* a1)
{
    free(a1);
    return 1;
}

// 0x579F30
void skill_ui_reset_()
{
    dword_683490 = 0;
    dword_683494 = 0;
}

// 0x579F40
void skill_ui_exit_()
{
    message_unload_(skill_ui_mes_);
}

// 0x57A6A0
int sub_57A6A0(int a1)
{
    return dword_5CB240[a1];
}

// 0x57A6B0
int sub_57A6B0(int a1)
{
    return sub_4C7160(a1);
}

// 0x57A700
int sub_57A700(int a1)
{
    return dword_683478[a1];
}

// 0x57B170
void sleep_ui_reset_()
{
    if (dword_6834F0) {
        sub_57B450();
    }
}

// 0x57B4E0
int sub_57B4E0()
{
    return dword_6834F0;
}

// 0x57BB30
int spell_ui_init_()
{
    return 1;
}

// 0x57BB40
void spell_ui_reset_()
{
    sub_57C470();
}

// 0x57BB50
void spell_ui_exit_()
{
}

// 0x57C520
bool sub_57C520(int a1)
{
    // #check
    return dword_5CB3AC[2 * a1] != 0;
}

// 0x57CA90
void tb_ui_reset_()
{
}

// 0x57CAA0
void tb_ui_exit_()
{
    sub_460150();
}

// 0x57CDC0
int sub_57CDC0()
{
    return sub_57D370(10, -1, 5000);
}

// 0x57CDE0
int sub_57CDE0()
{
    return sub_57D370(11, -1, 50);
}

// 0x57CDF0
int sub_57CDF0(int a1)
{
    return sub_57EF90(a1);
}

// 0x57CE00
int sub_57CE00()
{
    return sub_54B3C0();
}

// 0x57D2C0
void anim_ui_exit_()
{
}

// 0x57D300
int anim_ui_save_()
{
    return 1;
}

// 0x57D640
void sub_57D640()
{
    dword_5CB408 = 0;
}

// 0x57D650
int sub_57D650()
{
    return dword_5CB408;
}

// 0x57D800
void nullsub_56()
{
}

// 0x57DE00
int sub_57DE00()
{
    return dword_6839B0;
}

// 0x57EF90
int sub_57EF90(int a1)
{
    return sub_57F210(a1);
}

// 0x57F240
void sub_57F240(int a1)
{
    // #wrong: part of array of structs
    return dword_6835E0[22 * a1];
}

// 0x580480
void scrollbar_exit_()
{
    dword_68425C = 0;
}

// 0x5811B0
void sub_5811B0()
{
    dword_684684 = 0;
}

// 0x5816D0
void sub_5816D0()
{
    dword_684674++;
}

// 0x5816E0
void sub_5816E0()
{
    dword_684674--;
}

// 0x5816F0
int sub_5816F0()
{
    return dword_684674;
}

// 0x581A80
void nullsub_71()
{
}

// 0x581F80
void nullsub_70()
{
}

// 0x581F90
int sub_581F90(int a1)
{
    dword_5CBF80 = a1;
    sub_582AD0(0);
}

// 0x581FB0
int sub_581FB0()
{
    return sub_582D60(0);
}

// 0x5826B0
void nullsub_38()
{
}

// 0x5826C0
int sub_5826C0()
{
    return sub_5830F0();
}

// 0x582D10
bool sub_582D10(int a1)
{
    return a1 == 0;
}

// 0x582D40
int sub_582D40(int a1)
{
    return a1;
}

// 0x582D50
int sub_582D50(int a1)
{
    return a1;
}

// 0x583A80
void sub_583A80()
{
    message_unload_(dword_686538);
}

// 0x583B10
void sub_583B10()
{
    message_unload_(dword_686538);
}

// 0x584A80
char* sub_584A80()
{
    return byte_68653C;
}

// 0x584A90
char* sub_584A90()
{
    return byte_6865BC;
}

// 0x585BA0
int sub_585BA0()
{
    return sub_585A20();
}

// 0x585BB0
int sub_585BB0()
{
    return sub_584CB0(0);
}

// 0x588EF0
void sub_588EF0()
{
    if (sub_541680()) {
        sub_5412D0();
    }
}

// 0x588F00
void nullsub_69()
{
}

// 0x589530
int sub_589530(int a1)
{
    return sub_57F6D0(a1);
}

// 0x589740
int sub_589740(int a1)
{
    return sub_4B8280(a1);
}

// 0x589780
int sub_589780(int a1)
{
    return sub_4B8380(a1);
}

// 0x5897C0
int sub_5897C0(int a1)
{
    return sub_430580(a1);
}

// 0x589800
int sub_589800(int a1)
{
    return sub_4C8FF0(a1);
}

// 0x5898A0
void sub_5898A0(int* a1, int* a2)
{
    *a1 = sub_4D5180();
    *a2 = 1;
}
