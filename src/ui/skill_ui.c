#include "ui/skill_ui.h"

#include "game/lib/object.h"

#define FOUR 4

// 0x5CB220
static int dword_5CB220[] = { 279, 280, 282, 278 };

// 0x5CB230
static int dword_5CB230[] = { 279, 280, 282, 278 };

// 0x5CB240
static int dword_5CB240[] = { 6, 5, 15, 12 };

// 0x5CB250
static int dword_5CB250[4][2] = {
    { 0x1, 0x0 },
    { 0x40000044, 0x0 },
    { 0x4, 0x8000 },
    { 0x2000004, 0x0 },
};

// 0x5CB270
static int dword_5CB270 = -1;

// 0x683478
static const char* dword_683478[FOUR];

// 0x683488
static int skill_ui_mes_file;

// 0x683490
static long long qword_683490;

// 0x579E80
bool skill_ui_init(GameInitInfo* init_info)
{
    int index;
    SkillCallbacks callbacks;
    MesFileEntry mes_file_entry;

    (void)init_info;

    callbacks.field_0 = sub_57A770;
    callbacks.field_4 = sub_57A7F0;
    callbacks.field_8 = sub_57A8C0;
    callbacks.field_C = sub_57A990;
    callbacks.field_10 = skill_ui_trap;
    callbacks.field_14 = sub_57AA90;
    callbacks.field_18 = skill_ui_no_repair;
    callbacks.field_1C = skill_ui_lock_pick;
    callbacks.field_20 = skill_ui_no_lock;
    skill_set_callbacks(&callbacks);

    if (!message_load("mes\\skill_ui.mes", &skill_ui_mes_file)) {
        return false;
    }

    for (index = 0; index < FOUR; index++) {
        mes_file_entry.num = 100 + index;
        sub_4D43A0(skill_ui_mes_file, &mes_file_entry);
        dword_683478[index] = mes_file_entry.text;
    }

    return true;
}

// 0x579F30
void skill_ui_reset()
{
    qword_683490 = 0;
}

// 0x579F40
void skill_ui_exit()
{
    message_unload(skill_ui_mes_file);
}

// 0x579F50
int sub_579F50(int index)
{
    return index >= 0 && index < FOUR ? dword_5CB220[index] : dword_5CB220[0];
}

// 0x579F70
int sub_579F70(int index)
{
    return index >= 0 && index < FOUR ? dword_5CB230[index] : dword_5CB230[0];
}

// 0x579F90
int sub_579F90()
{
  return dword_5CB270;
}

// 0x579FA0
void sub_579FA0()
{
    // TODO: Incomplete.
}

// 0x57A0A0
void skill_ui_preprocess()
{
    // TODO: Incomplete.
}

// 0x57A1A0
void sub_57A1A0()
{
    sub_551A80(0);
    qword_683490 = 0;
}

// 0x57A1F0
void sub_57A1F0()
{
    // TODO: Incomplete.
}

// 0x57A320
void sub_57A320()
{
    // TODO: Incomplete.
}

// 0x57A5E0
bool sub_57A5E0(long long obj)
{
    return (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2) & OCF2_NO_PICKPOCKET) != 0
        || (obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_MECHANICAL) != 0;
}

// 0x57A620
void sub_57A620()
{
    // TODO: Incomplete.
}

// 0x57A6A0
int sub_57A6A0(int index)
{
    return dword_5CB240[index];
}

// 0x57A6B0
void sub_57A6B0()
{
    // TODO: Incomplete.
}

// 0x57A6C0
void sub_57A6C0()
{
    // TODO: Incomplete.
}

// 0x57A700
const char* sub_57A700(int index)
{
    return dword_683478[index];
}

// 0x57A710
bool sub_57A710(long long a1, long long a2)
{
    int type;

    if (a1 != a2) {
        type = obj_field_int32_get(a2, OBJ_F_TYPE);
        sub_572240(a1, a2, type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC ? 3 : 2);
    }

    return true;
}

// 0x57A770
bool sub_57A770(long long obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // You succeed.
        mes_file_entry.num = 500;
        font = 0;
    } else {
        // You fail.
        mes_file_entry.num = 501;
        font = 1;
    }

    sub_4D43A0(skill_ui_mes_file, &mes_file_entry);

    v1.field_0 = 6;
    v1.field_4 = mes_file_entry.text;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.text);

    return true;
}

// 0x57A7F0
void sub_57A7F0()
{
    // TODO: Incomplete.
}

// 0x57A8C0
void sub_57A8C0()
{
    // TODO: Incomplete.
}

// 0x57A990
bool sub_57A990(long long obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // You succeed.
        mes_file_entry.num = 500;
        font = 0;
    } else {
        // You fail.
        mes_file_entry.num = 501;
        font = 1;
    }

    sub_4D43A0(skill_ui_mes_file, &mes_file_entry);

    v1.field_0 = 6;
    v1.field_4 = mes_file_entry.text;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.text);

    return true;
}

// 0x57AA10
bool skill_ui_trap(long long obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // Trap successfully disarmed.
        mes_file_entry.num = 560;
        font = 0;
    } else {
        // Trap disarm failed.
        mes_file_entry.num = 561;
        font = 1;
    }

    sub_4D43A0(skill_ui_mes_file, &mes_file_entry);

    v1.field_0 = 6;
    v1.field_4 = mes_file_entry.text;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.text);

    return true;
}

// 0x57AA90
bool sub_57AA90(long long obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // You succeed.
        mes_file_entry.num = 500;
        font = 0;
    } else {
        // You fail.
        mes_file_entry.num = 501;
        font = 1;
    }

    sub_4D43A0(skill_ui_mes_file, &mes_file_entry);

    v1.field_0 = 6;
    v1.field_4 = mes_file_entry.text;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.text);

    return true;
}

// 0x57AB10
bool skill_ui_no_repair(long long obj)
{
    MesFileEntry mes_file_entry;
    John v1;

    // That is not in need of repair.
    mes_file_entry.num = 505;
    sub_4D43A0(skill_ui_mes_file, &mes_file_entry);

    v1.field_0 = 6;
    v1.field_4 = mes_file_entry.text;
    sub_550750(&v1);

    sub_4D5450(obj, 1, mes_file_entry.text);

    return true;
}

// 0x57AB70
void skill_ui_lock_pick(long long obj, int a2, int a3, bool success)
{
    MesFileEntry mes_file_entry;
    John v1;
    int font;

    if (success) {
        // Lock successfully picked.
        mes_file_entry.num = 550;
        font = 0;
    } else {
        // Lock picking failed.
        mes_file_entry.num = 551;
        font = 1;
    }

    sub_4D43A0(skill_ui_mes_file, &mes_file_entry);

    v1.field_0 = 6;
    v1.field_4 = mes_file_entry.text;
    sub_550750(&v1);

    sub_4D5450(obj, font, mes_file_entry.text);

    return true;
}

// 0x57ABF0
bool skill_ui_no_lock(long long obj)
{
    MesFileEntry mes_file_entry;
    John v1;

    // That has no lock.
    mes_file_entry.num = 512;
    sub_4D43A0(skill_ui_mes_file, &mes_file_entry);

    v1.field_0 = 6;
    v1.field_4 = mes_file_entry.text;
    sub_550750(&v1);

    sub_4D5450(obj, 1, mes_file_entry.text);

    return true;
}

// 0x57AC50
void sub_57AC50(long long obj, int a2, int a3)
{
    switch (a3) {
    case 1:
        sub_57ACD0(obj, a2);
        break;
    case -1:
        sub_57AEB0(obj, a2);
        break;
    }
}

// 0x57AC90
void sub_57AC90(long long obj, int a2, int a3)
{
    if (a2 > 12) {
        sub_4C6850(obj, a2 - 12, a3);
    } else {
        sub_4C6170(obj, a2, a3);
    }
}

// 0x57ACD0
void sub_57ACD0()
{
    // TODO: Incomplete.
}

// 0x57AEB0
void sub_57AEB0()
{
    // TODO: Incomplete.
}
