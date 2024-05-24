#include "ui/dialog_ui.h"

#include <tig/tig.h>

#include "game/lib/stat.h"

#define MAX_ENTRIES 8

// 0x567330
bool dialog_ui_init(GameInitInfo* init_info)
{
    int index;

    (void)init_info;

    dword_679DB8 = 0;
    dword_679DBC = 0;

    for (index = 0; index < MAX_ENTRIES; index++) {
        stru_66DAB8[index].field_1850 = false;
        stru_66DAB8[index].field_0 = index;
    }

    sub_444990(sub_567460, sub_568430);
    sub_4A84D0(sub_5678D0, sub_568430);
    sub_4C2EA0(sub_568430);
    dword_67B960 = -1;

    return true;
}

// 0x5673A0
void dialog_ui_exit()
{
    sub_5689B0();
}

// 0x5673B0
void dialog_ui_reset()
{
    int index;

    if (dword_67B964) {
        sub_553350();
        dword_67B964 = false;
    }

    for (index = 0; index < MAX_ENTRIES; index++) {
        if (stru_66DAB8[index].field_1850) {
            sub_5679C0(&(stru_66DAB8[index]));
        }
    }
}

// 0x567400
bool sub_567400(long long obj)
{
    return sub_567420(obj)->field_1850;
}

// 0x567420
DialogUiEntry* sub_567420(long long obj)
{
    int index = 0;

    if ((tig_net_flags & 0x1) != 0) {
        index = sub_4A2B10(obj);
    }

    return &(stru_66DAB8[index]);
}

// 0x567460
void sub_567460()
{
    // TODO: Incomplete.
}

// 0x5678D0
void sub_5678D0(long long obj)
{
    DialogUiEntry* entry;
    unsigned char packet[1072];

    entry = sub_567420(obj);
    if (!entry->field_1850) {
        return;
    }

    entry->field_1850 = false;

    if (player_is_pc_obj(obj)) {
        sub_553350();
        dword_67B964 = 0;
    }

    if ((tig_net_flags & 0x1) == 0 || (tig_net_flags & 0x2) != 0) {
        sub_412F40(entry->field_4);
    }

    sub_4D6160(entry->field_40, -1);

    if ((tig_net_flags & 0x1) == 0 || (tig_net_flags & 0x2) != 0) {
        sub_413280(&(entry->field_8));
    }

    if ((tig_net_flags & 0x1) == 0 && (tig_net_flags & 0x2) != 0) {
        *(int*)(&(packet[0])) = 44;
        *(int*)(&(packet[4])) = 1;
        memcpy(&(packet[8]), sub_407EF0(obj), sizeof(ObjectId));
        sub_5295F0(packet, sizeof(packet));
    }
}

// 0x5679C0
void sub_5679C0(DialogUiEntry* entry)
{
    if (!entry->field_1850) {
        return;
    }

    entry->field_1850 = false;

    if ((tig_net_flags & 0x1) == 0 || (tig_net_flags & 0x2) != 0) {
        sub_412F40(entry->field_4);
    }

    if ((tig_net_flags & 0x1) == 0 || (tig_net_flags & 0x2) != 0) {
        sub_413280(&(entry->field_8));
    }
}

// 0x567A10
int sub_567A10()
{
    return dword_67B964;
}

// 0x567A20
void sub_567A20(long long obj)
{
    sub_567420(obj)->field_1850 = false;
    if (player_is_pc_obj(obj)) {
        sub_553350();
        dword_67B964 = false;
    }
}

// 0x567A60
void sub_567A60(long long obj)
{
    sub_567420(obj)->field_1850 = true;
    if (player_is_pc_obj(obj)) {
        if (sub_553320(sub_5680A0)) {
            dword_67B964 = true;
        }
    }
}

// 0x567AB0
void sub_567AB0()
{
    // TODO: Incomplete.
}

// 0x567C30
void sub_567C30()
{
    // TODO: Incomplete.
}

// 0x567D60
void sub_567D60()
{
    // TODO: Incomplete.
}

// 0x567E00
void sub_567E00(int index, int a2)
{
    sub_567E30(&stru_66DAB8[index], a2);
}

// 0x567E30
bool sub_567E30(DialogUiEntry* entry, int a2)
{
    bool is_pc;

    is_pc = player_is_pc_obj(entry->field_10);
    sub_5686C0(entry->field_10, entry->field_40, 2, -1, &(entry->field_468[a2]));
    sub_4EF630(entry->field_40);
    sub_5689B0();
    sub_413130(&(entry->field_8), a2);
    sub_567D60(entry);

    switch (entry->field_17F0) {
    case 0:
        sub_5684C0(entry);
        break;
    case 1:
        sub_5678D0(entry->field_10);
        sub_568480(entry, 0);
        break;
    case 2:
        sub_5678D0(entry->field_10);
        sub_568480(entry, entry->field_17F4);
        break;
    case 3:
        if (is_pc) {
            sub_553370();
            sub_572240(entry->field_10, entry->field_40, 1);
        }
        if ((tig_net_flags & 0x1) != 0) {
            sub_5678D0(entry->field_10);
        }
        break;
    case 4:
        sub_568540(entry->field_40, entry->field_10, 0, -1, entry->field_78, entry->field_460);
        sub_5678D0(entry->field_10);
        break;
    case 5:
        if (is_pc) {
            sub_553370();
            sub_5597C0(entry->field_40, 2);
        }
        break;
    case 6:
        if (is_pc) {
            sub_553370();
            sub_560760();
        }
        break;
    case 7:
        if (is_pc) {
            sub_553370();
            sub_56D130(entry->field_40, entry->field_10);
        }
        break;
    case 8:
        if (is_pc) {
            sub_553370();
            sub_4EE550(entry->field_10, entry->field_40);
        }
        if ((tig_net_flags & 0x1) != 0) {
            sub_5678D0(entry->field_10);
        }
        break;
    case 9:
        if (is_pc) {
            sub_553370();
            sub_572240(entry->field_10, entry->field_40, 6);
        }
        if ((tig_net_flags & 0x1) != 0) {
            sub_5678D0(entry->field_10);
        }
        break;
    }

    return true;
}

// 0x5680A0
void sub_5680A0()
{
    // TODO: Incomplete.
}

// 0x5681B0
int sub_5681B0()
{
    return 0;
}

// 0x5681C0
void sub_5681C0(long long a1, long long a2)
{
    char text[1000];

    sub_4132A0(a2, a1, text);
    sub_568540(a2, a1, 0, -1, text, -1);
}

// 0x568220
void sub_568220()
{
    // TODO: Incomplete.
}

// 0x568280
bool sub_568280(DialogUiEntry *a1)
{
    bool is_pc;

    is_pc = player_is_pc_obj(a1->field_10);

    if ((tig_net_flags & 0x1) != 0 && (tig_net_flags & 0x2) == 0) {
        byte_679DB8[sub_4A2B10(sub_40DA50())] = 0;
    }

    switch (a1->field_17F0) {
    case 0:
        sub_5684C0(a1);
        break;
    case 1:
    case 2:
        sub_5678D0(a1->field_10);
        break;
    case 3:
        if (is_pc) {
            sub_553370();
            sub_572240(a1->field_10, a1->field_40, 1);
        }
        break;
    case 5:
        if (is_pc) {
            sub_553370();
            sub_5597C0(a1->field_40, 2);
        }
        break;
    case 6:
        if (is_pc) {
            sub_553370();
            sub_560760();
        }
        break;
    case 7:
        if (is_pc) {
            sub_553370();
            sub_56D130(a1->field_40, a1->field_10);
        }
        break;
    case 8:
        if (is_pc) {
            sub_553370();
            sub_4EE550(a1->field_10, a1->field_40);
        }
        break;
    case 9:
        if (is_pc) {
            sub_553370();
            sub_572240(a1->field_10, a1->field_40, 6);
        }
        break;
    }

    return true;
}

// 0x568430
void sub_568430()
{
    // TODO: Incomplete.
}

// 0x568480
void sub_568480(DialogUiEntry* entry, int a2)
{
    if (a2 == 0) {
        a2 = entry->field_1858 + 1;
    }

    sub_441980(entry->field_10, entry->field_40, 0, 0, 9, a2);
}

// 0x5684C0
void sub_5684C0(DialogUiEntry* entry)
{
    int index;

    sub_568540(entry->field_40, entry->field_10, 0, -2, entry->field_78, entry->field_460);

    if (player_is_pc_obj(entry->field_10)) {
        sub_553370();

        for (index = 0; index < entry->field_464; index++) {
            sub_553380(index, entry->field_468[index]);
        }
    }
}

// 0x568540
void sub_568540()
{
    // TODO: Incomplete.
}

// 0x5686C0
void sub_5686C0()
{
    // TODO: Incomplete.
}

// 0x568830
void sub_568830()
{
    // TODO: Incomplete.
}

// 0x568880
void sub_568880(long long obj, int a2, int a3, int a4, int a5, int a6, const char* str)
{
    if ((a6 & 1) != 0) {
        if (player_is_pc_obj(obj)) {
            return;
        }
    }

    sub_4D62B0(obj);
    sub_4D5FE0(obj, a4, str);
    sub_4D6160(obj, a5);
}

// 0x5688D0
void sub_5688D0(int a1, int a2, long long obj, int a4)
{
    int v1;
    int v2;
    char gender;
    char path[TIG_MAX_PATH];

    if (a4 == -1) {
        return;
    }

    sub_5689B0();
    sub_418A00(a4, &v1, &v2);

    gender = stat_level(obj, STAT_GENDER) != 0 ? 'm' : 'f';
    sprintf(path, "sound\\speech\\%.5d\\v%d_%c.mp3", v1, v2, gender);
    if (gender == 'f' && !tig_file_exists(path, NULL)) {
        sprintf(path, "sound\\speech\\%.5d\\v%d_%c.mp3", v1, v2, 'm');
    }
    if (tig_file_exists(path, NULL)) {
        dword_67B960 = gsound_play_voice(path, 0);
    }
}

// 0x5689B0
void sub_5689B0()
{
    if (dword_67B960 != -1) {
        tig_sound_destroy(dword_67B960);
        dword_67B960 = -1;
    }
}
