#include "ui/server_list_ui.h"

static void sub_588EF0();

// 0x585BC0
void serverlist_ui_init()
{
    // TODO: Incomplete.
}

// 0x585CB0
void serverlist_ui_exit()
{
    // TODO: Incomplete.
}

// 0x585D50
void sub_585D50()
{
    // TODO: Incomplete.
}

// 0x585E20
void sub_585E20()
{
    // TODO: Incomplete.
}

// 0x585E40
void sub_585E40()
{
    // TODO: Incomplete.
}

// 0x585F00
void sub_585F00()
{
    // TODO: Incomplete.
}

// 0x585FF0
void sub_585FF0()
{
    // TODO: Incomplete.
}

// 0x5860D0
void sub_5860D0()
{
    // TODO: Incomplete.
}

// 0x586150
void sub_586150()
{
    // TODO: Incomplete.
}

// 0x5862D0
void sub_5862D0()
{
    // TODO: Incomplete.
}

// 0x586AB0
void sub_586AB0()
{
    // TODO: Incomplete.
}

// 0x586AD0
void sub_586AD0()
{
    // TODO: Incomplete.
}

// 0x586D20
void sub_586D20()
{
    // TODO: Incomplete.
}

// 0x586ED0
void sub_586ED0()
{
    // TODO: Incomplete.
}

// 0x587570
void sub_587570()
{
    // TODO: Incomplete.
}

// 0x587960
void sub_587960()
{
    // TODO: Incomplete.
}

// 0x587D60
void sub_587D60()
{
    // TODO: Incomplete.
}

// 0x588180
void sub_588180()
{
    // TODO: Incomplete.
}

// 0x5883E0
void sub_5883E0()
{
    // TODO: Incomplete.
}

// 0x588650
void sub_588650()
{
    // TODO: Incomplete.
}

// 0x5887F0
void sub_5887F0()
{
    // TODO: Incomplete.
}

// 0x588990
void sub_588990()
{
    // TODO: Incomplete.
}

// 0x588B10
void sub_588B10()
{
    // TODO: Incomplete.
}

// 0x588B70
void sub_588B70()
{
    // TODO: Incomplete.
}

// 0x588C80
void sub_588C80()
{
    // TODO: Incomplete.
}

// 0x588CC0
void sub_588CC0()
{
    // TODO: Incomplete.
}

// 0x588D00
void sub_588D00()
{
    // TODO: Incomplete.
}

// 0x588D70
void sub_588D70()
{
    // TODO: Incomplete.
}

// 0x588D90
void sub_588D90()
{
    // TODO: Incomplete.
}

// 0x588E10
void sub_588E10()
{
    // TODO: Incomplete.
}

// 0x588EC0
void sub_588EC0()
{
    // TODO: Incomplete.
}

// 0x588EF0
void sub_588EF0()
{
    if (sub_541680()) {
        sub_5412D0();
    }
}

// 0x588F00
void sub_588F00()
{
}

// 0x588F10
void sub_588F10(int a1)
{
    switch (a1) {
    case 0:
        tig_net_reset_connection();
        sub_5417A0(0);
        sub_583200();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        break;
    case 1:
        tig_net_reset_connection();
        sub_5417A0(0);
        multiplayer_hub_ui_init();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        break;
    case 2:
        tig_net_reset_connection();
        sub_5417A0(0);
        sub_584AE0();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        break;
    case 3:
        sub_5883E0(dword_5CCB94, sub_549820());
        break;
    case 4:
        sub_588650();
        break;
    case 5:
        sub_5887F0();
        break;
    case 6:
        sub_588990();
        break;
    }

    return 0;
}

// 0x588FF0
void sub_588FF0()
{
    // TODO: Incomplete.
}
