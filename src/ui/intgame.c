#include "ui/intgame.h"

#include "game/mes.h"

// 0x64C470
static tig_font_handle_t dword_64C470;

// 0x64C498
static tig_font_handle_t dword_64C498;

// 0x64C49C
static tig_font_handle_t dword_64C49C;

// 0x64C4A0
static tig_font_handle_t dword_64C4A0;

// 0x64C500
static tig_font_handle_t dword_64C500;

// 0x64C504
static mes_file_handle_t intgame_mes_file;

// 0x64C538
static tig_font_handle_t dword_64C538;

// 0x739F88
static tig_font_handle_t dword_739F88;

// 0x549B70
void intgame_init()
{
    // TODO: Incomplete.
}

// 0x549F00
void intgame_reset()
{
    // TODO: Incomplete.
}

// 0x549F60
void intgame_resize()
{
    // TODO: Incomplete.
}

// 0x54A130
void intgame_exit()
{
    tig_font_destroy(dword_739F88);
    tig_font_destroy(dword_64C470);
    tig_font_destroy(dword_64C538);
    tig_font_destroy(dword_64C498);
    tig_font_destroy(dword_64C49C);
    tig_font_destroy(dword_64C500);
    tig_font_destroy(dword_64C4A0);
    intgame_big_window_destroy();
    mes_unload(intgame_mes_file);
}

// 0x54A1A0
void intgame_save()
{
    // TODO: Incomplete.
}

// 0x54A220
void intgame_load()
{
    // TODO: Incomplete.
}

// 0x54A330
void iso_interface_create()
{
    // TODO: Incomplete.
}

// 0x54A9A0
void iso_interface_destroy()
{
    // TODO: Incomplete.
}

// 0x54AA30
void sub_54AA30()
{
    // TODO: Incomplete.
}

// 0x54AA60
void sub_54AA60()
{
    // TODO: Incomplete.
}

// 0x54AAE0
void sub_54AAE0()
{
    // TODO: Incomplete.
}

// 0x54AB20
void sub_54AB20()
{
    // TODO: Incomplete.
}

// 0x54ABD0
void sub_54ABD0()
{
    // TODO: Incomplete.
}

// 0x54AC70
void sub_54AC70()
{
    // TODO: Incomplete.
}

// 0x54AD00
void sub_54AD00()
{
    // TODO: Incomplete.
}

// 0x54AEE0
void sub_54AEE0()
{
    // TODO: Incomplete.
}

// 0x54AF10
void sub_54AF10()
{
    // TODO: Incomplete.
}

// 0x54B3A0
void sub_54B3A0()
{
    // TODO: Incomplete.
}

// 0x54B3C0
void sub_54B3C0()
{
    // TODO: Incomplete.
}

// 0x54B500
void intgame_ammo_icon_refresh()
{
    // TODO: Incomplete.
}

// 0x54B5D0
void sub_54B5D0()
{
    // TODO: Incomplete.
}

// 0x54C8E0
void sub_54C8E0()
{
    // TODO: Incomplete.
}

// 0x54DBF0
void sub_54DBF0()
{
    // TODO: Incomplete.
}

// 0x54DC80
void sub_54DC80()
{
    // TODO: Incomplete.
}

// 0x54DE50
void sub_54DE50()
{
    // TODO: Incomplete.
}

// 0x54EA80
void sub_54EA80()
{
    // TODO: Incomplete.
}

// 0x54EB50
void sub_54EB50()
{
    // TODO: Incomplete.
}

// 0x54EB60
void sub_54EB60()
{
    // TODO: Incomplete.
}

// 0x54EBF0
void sub_54EBF0()
{
    // TODO: Incomplete.
}

// 0x54ECD0
void sub_54ECD0()
{
    // TODO: Incomplete.
}

// 0x54ED30
void sub_54ED30()
{
    // TODO: Incomplete.
}

// 0x54FCF0
void sub_54FCF0()
{
    // TODO: Incomplete.
}

// 0x550000
void sub_550000()
{
    // TODO: Incomplete.
}

// 0x550150
void sub_550150()
{
    // TODO: Incomplete.
}

// 0x5501C0
void sub_5501C0()
{
    // TODO: Incomplete.
}

// 0x5503F0
void sub_5503F0()
{
    // TODO: Incomplete.
}

// 0x5504F0
void iso_interface_window_disable()
{
    // TODO: Incomplete.
}

// 0x5506C0
void sub_5506C0()
{
    // TODO: Incomplete.
}

// 0x550720
void sub_550720()
{
    // TODO: Incomplete.
}

// 0x550750
void sub_550750()
{
    // TODO: Incomplete.
}

// 0x550770
void sub_550770()
{
    // TODO: Incomplete.
}

// 0x5507D0
void sub_5507D0()
{
    // TODO: Incomplete.
}

// 0x5507E0
void sub_5507E0()
{
    // TODO: Incomplete.
}

// 0x550860
void sub_550860()
{
    // TODO: Incomplete.
}

// 0x5508C0
void sub_5508C0()
{
    // TODO: Incomplete.
}

// 0x550930
void sub_550930()
{
    // TODO: Incomplete.
}

// 0x5509C0
void sub_5509C0()
{
    // TODO: Incomplete.
}

// 0x550A10
void sub_550A10()
{
    // TODO: Incomplete.
}

// 0x550BD0
void sub_550BD0()
{
    // TODO: Incomplete.
}

// 0x550C60
void sub_550C60()
{
    // TODO: Incomplete.
}

// 0x550CD0
void sub_550CD0()
{
    // TODO: Incomplete.
}

// 0x550D20
void sub_550D20()
{
    // TODO: Incomplete.
}

// 0x550D60
void sub_550D60()
{
    // TODO: Incomplete.
}

// 0x550DA0
void sub_550DA0()
{
    // TODO: Incomplete.
}

// 0x551000
void sub_551000()
{
    // TODO: Incomplete.
}

// 0x551080
void sub_551080()
{
    // TODO: Incomplete.
}

// 0x551160
void sub_551160()
{
    // TODO: Incomplete.
}

// 0x551210
void iso_interface_window_enable()
{
    // TODO: Incomplete.
}

// 0x551660
void sub_551660()
{
    // TODO: Incomplete.
}

// 0x551740
void sub_551740()
{
    // TODO: Incomplete.
}

// 0x5517A0
void sub_5517A0()
{
    // TODO: Incomplete.
}

// 0x5517F0
void sub_5517F0()
{
    // TODO: Incomplete.
}

// 0x551830
void sub_551830()
{
    // TODO: Incomplete.
}

// 0x5518C0
void sub_5518C0()
{
    // TODO: Incomplete.
}

// 0x551910
void sub_551910()
{
    // TODO: Incomplete.
}

// 0x551A00
void sub_551A00()
{
    // TODO: Incomplete.
}

// 0x551A10
void sub_551A10()
{
    // TODO: Incomplete.
}

// 0x551A80
void sub_551A80()
{
    // TODO: Incomplete.
}

// 0x551F20
void sub_551F20()
{
    // TODO: Incomplete.
}

// 0x551F40
void sub_551F40()
{
    // TODO: Incomplete.
}

// 0x551F70
void sub_551F70()
{
    // TODO: Incomplete.
}

// 0x551F80
void sub_551F80()
{
    // TODO: Incomplete.
}

// 0x552050
void sub_552050()
{
    // TODO: Incomplete.
}

// 0x552070
void sub_552070()
{
    // TODO: Incomplete.
}

// 0x552080
void sub_552080()
{
    // TODO: Incomplete.
}

// 0x5520D0
void sub_5520D0()
{
    // TODO: Incomplete.
}

// 0x552130
void sub_552130()
{
    // TODO: Incomplete.
}

// 0x552160
void sub_552160()
{
    // TODO: Incomplete.
}

// 0x5521B0
void intgame_text_edit_refresh_color()
{
    // TODO: Incomplete.
}

// 0x5522F0
void intgame_clock_refresh()
{
    // TODO: Incomplete.
}

// 0x5526F0
void intgame_clock_process_callback()
{
    // TODO: Incomplete.
}

// 0x552740
void sub_552740()
{
    // TODO: Incomplete.
}

// 0x552770
void sub_552770()
{
    // TODO: Incomplete.
}

// 0x5528E0
void sub_5528E0()
{
    // TODO: Incomplete.
}

// 0x552930
void sub_552930()
{
    // TODO: Incomplete.
}

// 0x552960
void sub_552960()
{
    // TODO: Incomplete.
}

// 0x5529C0
void sub_5529C0()
{
    // TODO: Incomplete.
}

// 0x553320
void sub_553320()
{
    // TODO: Incomplete.
}

// 0x553350
void sub_553350()
{
    // TODO: Incomplete.
}

// 0x553370
void sub_553370()
{
    // TODO: Incomplete.
}

// 0x553380
void sub_553380()
{
    // TODO: Incomplete.
}

// 0x5533A0
void sub_5533A0()
{
    // TODO: Incomplete.
}

// 0x5533B0
void sub_5533B0()
{
    // TODO: Incomplete.
}

// 0x5533C0
void sub_5533C0()
{
    // TODO: Incomplete.
}

// 0x553620
void sub_553620()
{
    // TODO: Incomplete.
}

// 0x553670
void intgame_spell_maintain_refresh_func()
{
    // TODO: Incomplete.
}

// 0x553910
void sub_553910()
{
    // TODO: Incomplete.
}

// 0x553960
void sub_553960()
{
    // TODO: Incomplete.
}

// 0x553990
void sub_553990()
{
    // TODO: Incomplete.
}

// 0x553A60
void sub_553A60()
{
    // TODO: Incomplete.
}

// 0x553A70
void sub_553A70()
{
    // TODO: Incomplete.
}

// 0x553BE0
void sub_553BE0()
{
    // TODO: Incomplete.
}

// 0x553D10
void sub_553D10()
{
    // TODO: Incomplete.
}

// 0x553F70
void sub_553F70()
{
    // TODO: Incomplete.
}

// 0x554560
void sub_554560()
{
    // TODO: Incomplete.
}

// 0x554640
void sub_554640()
{
    // TODO: Incomplete.
}

// 0x554830
void sub_554830()
{
    // TODO: Incomplete.
}

// 0x554B00
void sub_554B00()
{
    // TODO: Incomplete.
}

// 0x554BE0
void sub_554BE0()
{
    // TODO: Incomplete.
}

// 0x554C20
void sub_554C20()
{
    // TODO: Incomplete.
}

// 0x554F10
void sub_554F10()
{
    // TODO: Incomplete.
}

// 0x555780
void sub_555780()
{
    // TODO: Incomplete.
}

// 0x555910
void sub_555910()
{
    // TODO: Incomplete.
}

// 0x555B50
void sub_555B50()
{
    // TODO: Incomplete.
}

// 0x555D80
void sub_555D80()
{
    // TODO: Incomplete.
}

// 0x555EC0
void sub_555EC0()
{
    // TODO: Incomplete.
}

// 0x556040
void sub_556040()
{
    // TODO: Incomplete.
}

// 0x5561D0
void sub_5561D0()
{
    // TODO: Incomplete.
}

// 0x556220
void sub_556220()
{
    // TODO: Incomplete.
}

// 0x5566B0
void sub_5566B0()
{
    // TODO: Incomplete.
}

// 0x556A90
void sub_556A90()
{
    // TODO: Incomplete.
}

// 0x556B70
void sub_556B70()
{
    // TODO: Incomplete.
}

// 0x556B90
void sub_556B90()
{
    // TODO: Incomplete.
}

// 0x556C20
void sub_556C20()
{
    // TODO: Incomplete.
}

// 0x556E60
void sub_556E60()
{
    // TODO: Incomplete.
}

// 0x556EA0
void sub_556EA0()
{
    // TODO: Incomplete.
}

// 0x556EF0
void sub_556EF0()
{
    // TODO: Incomplete.
}

// 0x556F80
void intgame_mt_button_disable()
{
    // TODO: Incomplete.
}

// 0x5570A0
void sub_5570A0()
{
    // TODO: Incomplete.
}

// 0x5570D0
void sub_5570D0()
{
    // TODO: Incomplete.
}

// 0x5571C0
void sub_5571C0()
{
    // TODO: Incomplete.
}

// 0x557230
void intgame_big_window_create()
{
    // TODO: Incomplete.
}

// 0x5572B0
void intgame_big_window_destroy()
{
    // TODO: Incomplete.
}

// 0x5572C0
void intgame_big_window_message_filter()
{
    // TODO: Incomplete.
}

// 0x5572D0
void intgame_big_window_lock()
{
    // TODO: Incomplete.
}

// 0x557330
void intgame_big_window_unlock()
{
    // TODO: Incomplete.
}

// 0x557370
void sub_557370()
{
    // TODO: Incomplete.
}

// 0x557670
void sub_557670()
{
    // TODO: Incomplete.
}

// 0x5576B0
void sub_5576B0()
{
    // TODO: Incomplete.
}

// 0x557730
void sub_557730()
{
    // TODO: Incomplete.
}

// 0x557790
void sub_557790()
{
    // TODO: Incomplete.
}

// 0x5577D0
void sub_5577D0()
{
    // TODO: Incomplete.
}

// 0x5577E0
void sub_5577E0()
{
    // TODO: Incomplete.
}

// 0x5577F0
void intgame_set_width()
{
    // TODO: Incomplete.
}

// 0x557800
void intgame_set_height()
{
    // TODO: Incomplete.
}

// 0x557810
void intgame_create_iso_window()
{
    // TODO: Incomplete.
}

// 0x5578C0
void sub_5578C0()
{
    // TODO: Incomplete.
}

// 0x5578D0
void intgame_set_fullscreen()
{
    // TODO: Incomplete.
}

// 0x5578E0
void intgame_toggle_interface()
{
    // TODO: Incomplete.
}

// 0x557AA0
void sub_557AA0()
{
    // TODO: Incomplete.
}

// 0x557AB0
void sub_557AB0()
{
    // TODO: Incomplete.
}

// 0x557AC0
void sub_557AC0()
{
    // TODO: Incomplete.
}

// 0x557B00
void sub_557B00()
{
    // TODO: Incomplete.
}

// 0x557B10
void sub_557B10()
{
    // TODO: Incomplete.
}

// 0x557B20
void sub_557B20()
{
    // TODO: Incomplete.
}

// 0x557B30
void sub_557B30()
{
    // TODO: Incomplete.
}

// 0x557B50
void sub_557B50()
{
    // TODO: Incomplete.
}

// 0x557B60
void sub_557B60()
{
    // TODO: Incomplete.
}

// 0x557C00
void sub_557C00()
{
    // TODO: Incomplete.
}

// 0x557CF0
void sub_557CF0()
{
    // TODO: Incomplete.
}
