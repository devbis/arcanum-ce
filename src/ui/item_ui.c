#include "ui/item_ui.h"

#include "game/obj.h"
#include "ui/intgame.h"

// 0x6810D8
static int64_t qword_6810D8;

// 0x5719D0
bool item_ui_init(GameInitInfo* init_info)
{
    (void)init_info;

    return true;
}

// 0x5719E0
void item_ui_exit()
{
}

// 0x5719F0
void sub_5719F0()
{
    // TODO: Incomplete.
}

// 0x571AA0
void sub_571AA0(int64_t a1, int64_t a2)
{
    // TODO: Incomplete.
}

// 0x571C80
void sub_571C80()
{
    sub_551A80(0);
    qword_6810D8 = OBJ_HANDLE_NULL;
    sub_553A60(21);
}

// 0x571CB0
void sub_571CB0(S4F2810* a1)
{
    // TODO: Incomplete.
}
