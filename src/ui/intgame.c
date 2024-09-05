#include "ui/intgame.h"

#include "game/gamelib.h"
#include "game/mes.h"
#include "game/obj.h"
#include "game/stat.h"
#include "ui/compact_ui.h"
#include "ui/gameuilib.h"

static bool sub_54AB20(UiButtonInfo* button_info, unsigned int flags);
static bool sub_54ABD0(UiButtonInfo* button_info, int width, int height);
static void intgame_ammo_icon_refresh(tig_art_id_t art_id);
static void intgame_mt_button_enable();
static bool intgame_big_window_create();
static void intgame_big_window_destroy();
static bool intgame_big_window_message_filter(TigMessage* msg);

// 0x5C6378
static tig_window_handle_t dword_5C6378[5] = {
    TIG_WINDOW_HANDLE_INVALID,
    TIG_WINDOW_HANDLE_INVALID,
    TIG_WINDOW_HANDLE_INVALID,
    TIG_WINDOW_HANDLE_INVALID,
    TIG_WINDOW_HANDLE_INVALID,
};

// 0x5C6390
static TigRect stru_5C6390[2] = {
    { 0, 0, 800, 41 },
    { 0, 441, 800, 159 },
};

// 0x5C63B0
static TigRect stru_5C63B0 = { 311, 96, 178, 178 };

// 0x5C63C0
static TigRect stru_5C63C0 = { 311, 196, 178, 178 };

// 0x5C63D0
static int dword_5C63D0 = -1;

// 0x5C63D8
static TigRect stru_5C63D8 = { 14, 472, 28, 88 };

// 0x5C63E8
static TigRect stru_5C63E8 = { 754, 473, 28, 88 };

// 0x5C6470
static TigRect stru_5C6470 = { 61, 509, 251, -1 };

// 0x5C6524
static int dword_5C6524[5] = {
    169,
    187,
    193,
    194,
    186,
};

// 0x5C6F74
static tig_button_handle_t intgame_mt_button_handle;

// 0x5C6F78
static int dword_5C6F78 = 6;

// 0x5C739C
static int intgame_iso_window_width = 800;

// 0x5C73A0
static int intgame_iso_window_height = 600;

// 0x5C73A4
static tig_window_handle_t intgame_iso_window = TIG_WINDOW_HANDLE_INVALID;

// 0x64C470
static tig_font_handle_t dword_64C470;

// 0x64C474
static TigVideoBuffer* dword_64C474;

// 0x64C484
static int dword_64C484[5];

// 0x64C498
static tig_font_handle_t dword_64C498;

// 0x64C49C
static tig_font_handle_t dword_64C49C;

// 0x64C4A0
static tig_font_handle_t dword_64C4A0;

// 0x64C4F8
static tig_window_handle_t dword_64C4F8[2];

// 0x64C500
static tig_font_handle_t dword_64C500;

// 0x64C504
static mes_file_handle_t intgame_mes_file;

// 0x64C508
static tig_window_handle_t intgame_big_window_handle;

// 0x64C510
static TigRect stru_64C510;

// 0x64C52C
static tig_window_handle_t dword_64C52C;

// 0x64C530
static int dword_64C530;

// 0x64C534
static int dword_64C534;

// 0x64C538
static tig_font_handle_t dword_64C538;

// 0x64C630
static bool intgame_big_window_locked;

// 0x64C670
static tig_font_handle_t dword_64C670;

// 0x64C674
static int dword_64C674;

// 0x64C67C
static bool intgame_compact_interface;

// 0x64C680
static bool intgame_fullscreen;

// 0x64C6A8
static int dword_64C6A8;

// 0x64C6AC
static int dword_64C6AC;

// 0x64C6B4
static bool dword_64C6B4;

// 0x64C6D4
static void(*dword_64C6D4)(int);

// 0x64C6D8
static int dword_64C6D8;

// 0x64C6F0
static unsigned int intgame_iso_window_flags;

// 0x739F88
static tig_font_handle_t dword_739F88;

// 0x549B70
bool intgame_init(GameInitInfo* init_info)
{
    TigFont font;

    (void)init_info;

    if (!mes_load("mes\\intgame.mes", &intgame_mes_file)) {
        return false;
    }

    if (!intgame_big_window_create()) {
        mes_unload(intgame_mes_file);
        return false;
    }

    font.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create_(&font, &dword_739F88);

    font.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(100, 100, 255);
    tig_font_create_(&font, &dword_64C470);

    font.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 114, 0);
    tig_font_create_(&font, &dword_64C538);

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create_(&font, &dword_64C498);

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 0, 0);
    tig_font_create_(&font, &dword_64C49C);

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(0, 0, 255);
    tig_font_create_(&font, &dword_64C500);

    font.flags = 0;
    tig_art_interface_id_create(230, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create_(&font, &dword_64C4A0);

    memcpy(dword_64C484, dword_5C6524, sizeof(dword_64C484));
    dword_64C534 = 2;
    dword_64C674 = -1;

    return true;
}

// 0x549F00
void intgame_reset()
{
    int index;

    dword_64C6D8 = 0;
    sub_553990();
    sub_57DAB0();
    intgame_clock_process_callback();
    sub_552130(0);

    for (index = 0; index < 10; index++) {
        sub_57F210(index);
    }

    dword_64C534 = 2;
    memcpy(dword_64C484, dword_5C6524, sizeof(dword_64C484));
    sub_54AA30();
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
bool intgame_save(TigFile* stream)
{
    if (stream == NULL) return false;
    if (tig_file_fwrite(&dword_64C6A8, sizeof(dword_64C6A8), 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_64C530, sizeof(dword_64C530), 1, stream) != 1) return false;
    if (!sub_57DB40(stream)) return false;
    if (tig_file_fwrite(dword_64C484, sizeof(*dword_64C484), 5, stream) != 5) return false;
    if (tig_file_fwrite(&dword_64C534, sizeof(dword_64C534), 1, stream) != 1) return false;

    return true;
}

// 0x54A220
bool intgame_load(GameLoadInfo* load_info)
{
    int v1;
    int index;
    int64_t obj;

    if (load_info->stream == NULL) return false;
    if (tig_file_fread(&v1, sizeof(v1), 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_64C530, sizeof(dword_64C530), 1, load_info->stream) != 1) return false;

    if (dword_64C6A8 == 1) {
        tig_button_state_change(stru_5C6618[dword_64C530].button_handle, TIG_BUTTON_STATE_PRESSED);
    }

    if (!sub_57DBA0(load_info)) return false;
    if (tig_file_fread(dword_64C484, sizeof(*dword_64C484), 5, load_info->stream) != 5) return false;
    if (tig_file_fread(&dword_64C534, sizeof(dword_64C534), 1, load_info->stream) != 1) return false;

    for (index = 0; index < 5; index++) {
        sub_556B90(index);
    }

    sub_54B3C0();
    sub_553990();
    intgame_mt_button_enable();

    obj = player_get_pc_obj();
    if (obj != OBJ_HANDLE_NULL) {
        sub_556C20(obj);
        sub_54AD00(2, stat_level(obj, STAT_FATE_POINTS), 2);
    }

    return true;
}

// 0x54A330
bool iso_interface_create(tig_window_handle_t window_handle)
{
    // TODO: Incomplete.
}

// 0x54A9A0
void iso_interface_destroy()
{
    int index;

    if (dword_64C6B4) {
        for (index = 0; index < 2; index++) {
            tig_window_destroy(dword_64C4F8[index]);
        }

        tig_font_destroy(dword_64C670);

        for (index = 0; index < 10; index++) {
            FREE(stru_64C540[index].field_4);
        }

        for (index = 0; index < 5; index++) {
            if (dword_5C6378[index] != TIG_WINDOW_HANDLE_INVALID) {
                tig_window_destroy(dword_5C6378[index]);
                dword_5C6378[index] = TIG_WINDOW_HANDLE_INVALID;
            }
        }
    }

    tig_video_buffer_destroy(dword_64C474);
}

// 0x54AA30
void sub_54AA30()
{
    dword_64C6B8 = 0;
    dword_64C634[0] = 0;
    dword_64C6C0 = 0;
    dword_64C6C4 = 0;
    dword_64C6C8 = 0;
}

// 0x54AA60
bool sub_54AA60(tig_window_handle_t window_handle, TigRect* rect, UiButtonInfo* button_info, unsigned int flags)
{
    TigButtonData button_data;

    button_data.flags = flags;
    button_data.window_handle = window_handle;
    button_data.x = button_info->x - rect->x;
    button_data.y = button_info->y - rect->y;
    tig_art_interface_id_create(button_info->art_num, 0, 0, 0, &(button_data.art_id));
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_exit_snd_id = TIG_SOUND_HANDLE_INVALID;
    return tig_button_create(&button_data, &(button_info->button_handle)) == TIG_OK;
}

// 0x54AAE0
bool sub_54AAE0(UiButtonInfo* button_info)
{
    int index;

    index = sub_551740(button_info->x, button_info->y);
    if (index == -1) {
        return false;
    }

    return sub_54AA60(dword_64C4F8[index], &(stru_5C6390[index]), button_info, TIG_BUTTON_FLAG_0x01);
}

// 0x54AB20
bool sub_54AB20(UiButtonInfo* button_info, unsigned int flags)
{
    int index;

    index = sub_551740(button_info->x, button_info->y);
    if (index == -1) {
        return false;
    }

    return sub_54AA60(dword_64C4F8[index], &(stru_5C6390[index]), button_info, flags);
}

// 0x54ABD0
bool sub_54ABD0(UiButtonInfo* button_info, int width, int height)
{
    int index;
    TigButtonData button_data;

    index = sub_551740(button_info->x, button_info->y);
    if (index == -1) {
        return false;
    }

    button_data.window_handle = dword_64C4F8[index];
    button_data.x = button_info->x - stru_5C6390[index].x;
    button_data.y = button_info->y - stru_5C6390[index].y;
    button_data.width = width;
    button_data.height = height;
    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.art_id = TIG_ART_ID_INVALID;
    button_data.mouse_down_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_up_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_enter_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_exit_snd_id = TIG_SOUND_HANDLE_INVALID;
    return tig_button_create(&button_data, &(button_info->button_handle)) == TIG_OK;
}

// 0x54AC70
void sub_54AC70(UiButtonInfo* button_info)
{
    tig_button_destroy(button_info->button_handle);
    button_info->button_handle = TIG_BUTTON_HANDLE_INVALID;
}

// 0x54AD00
void sub_54AD00()
{
    // TODO: Incomplete.
}

// 0x54AEE0
void sub_54AEE0(int a1)
{
    switch (a1) {
    case 0:
        sub_54AF10(&stru_5C63D8);
        break;
    case 1:
        sub_54AF10(&stru_5C63E8);
        break;
    }
}

// 0x54AF10
void sub_54AF10()
{
    // TODO: Incomplete.
}

// 0x54B3A0
void sub_54B3A0()
{
    sub_54AEE0(0);
    sub_54AEE0(1);
}

// 0x54B3C0
void sub_54B3C0()
{
    // TODO: Incomplete.
}

// 0x54B500
void intgame_ammo_icon_refresh(tig_art_id_t art_id)
{
    int index;
    TigRect src_rect;
    TigRect dst_rect;
    TigArtFrameData art_frame_data;
    TigArtBlitSpec blt;

    index = sub_551740(stru_5C6470.x, stru_5C6470.y);
    if (index == -1) {
        tig_debug_printf("intgame_ammo_icon_refresh: ERROR: couldn't find iwid match!\n");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
    }

    blt.art_id = art_id;
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = stru_5C6470.x - stru_5C6390[index].x;
    dst_rect.y = stru_5C6470.y - stru_5C6390[index].y;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    blt.flags = 0;
    blt.src_rect = &src_rect;
    blt.dst_rect = &dst_rect;
    tig_window_blit_art(dword_64C4F8[index], &blt);
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
void sub_5506C0(int a1)
{
    sub_571910();
    if (dword_64C6A8 == 9) {
        sub_551A80(0);
    }

    dword_5C6F78 = 6;
    if (dword_64C6A8 == a1) {
        dword_64C6AC = 0;
        sub_552130(0);
    } else {
        dword_64C6AC = a1;
        sub_552130(a1);
    }
}

// 0x550720
void sub_550720()
{
    if (dword_64C6B4 && dword_64C6A8 == 0) {
        if (dword_64C6D4 != NULL) {
            dword_64C6D4(0);
        } else {
            sub_550930();
        }
    }
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
void intgame_mt_button_enable()
{
    bool hidden;
    int64_t obj;
    int64_t item_obj;
    int mana_store;
    unsigned int flags;

    if (tig_button_is_hidden(intgame_mt_button_handle, &hidden) == TIG_OK) {
        obj = player_get_pc_obj();
        item_obj = item_wield_get(obj, 1004);
        if (item_obj != OBJ_HANDLE_NULL) {
            mana_store = obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_MANA_STORE);
            flags = obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS);
            if ((mana_store != 0 || (flags & OIF_IS_MAGICAL) != 0)
                && obj_field_int32_get(item_obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) > 0) {
                tig_button_show(intgame_mt_button_handle);
            }
        }
    }
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
bool intgame_big_window_create()
{
    TigWindowData window_data;

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.rect.x = 0;
    window_data.rect.y = 0;
    window_data.rect.width = 0;
    window_data.rect.height = 0;
    window_data.background_color = 0;
    window_data.message_filter = intgame_big_window_message_filter;
    if (tig_window_create(&window_data, &intgame_big_window_handle) != TIG_OK) {
        return false;
    }

    tig_window_hide(intgame_big_window_handle);
    intgame_big_window_locked = false;

    return true;
}

// 0x5572B0
void intgame_big_window_destroy()
{
    tig_window_destroy(intgame_big_window_handle);
}

// 0x5572C0
bool intgame_big_window_message_filter(TigMessage* msg)
{
    return false;
}

// 0x5572D0
bool intgame_big_window_lock(TigWindowMessageFilterFunc* func, tig_window_handle_t* window_handle_ptr)
{
    if (intgame_big_window_locked) {
        return false;
    }

    intgame_big_window_locked = true;
    tig_window_message_filter_set(intgame_big_window_handle, func);
    tig_window_show(intgame_big_window_handle);
    sub_51E850(intgame_big_window_handle);
    *window_handle_ptr = intgame_big_window_handle;

    return true;
}

// 0x557330
void intgame_big_window_unlock()
{
    intgame_big_window_locked = false;
    tig_window_button_destroy(intgame_big_window_handle);
    tig_window_message_filter_set(intgame_big_window_handle, intgame_big_window_message_filter);
    tig_window_hide(intgame_big_window_handle);
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
    TigRect rect;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &rect;
    art_blit_info.dst_rect = &rect;
    tig_art_interface_id_create(185, 0, 0, 0, &(art_blit_info.art_id));

    tig_art_frame_data(art_blit_info.art_id, &art_frame_data);

    rect.x = 0;
    rect.y = 0;
    rect.width = art_frame_data.width;
    rect.height = art_frame_data.height;
    tig_window_blit_art(dword_64C4F8[0], &art_blit_info);
}

// 0x557730
void sub_557730()
{
    // TODO: Incomplete.
}

// 0x557790
void sub_557790(int64_t obj)
{
    if (obj != OBJ_HANDLE_NULL
        && obj == sub_43C570()) {
        sub_57CCF0(player_get_pc_obj(), obj);
    }
}

// 0x5577D0
unsigned int intgame_get_iso_window_flags()
{
    return intgame_iso_window_flags;
}

// 0x5577E0
void intgame_set_iso_window_flags(unsigned int flags)
{
    intgame_iso_window_flags = flags;
}

// 0x5577F0
void intgame_set_iso_window_width(int width)
{
    intgame_iso_window_width = width;
}

// 0x557800
void intgame_set_iso_window_height(int height)
{
    intgame_iso_window_height = height;
}

// 0x557810
bool intgame_create_iso_window(tig_window_handle_t* window_handle_ptr)
{
    TigWindowData window_data;

    window_data.flags = intgame_iso_window_flags | TIG_WINDOW_FLAG_0x80 | TIG_WINDOW_VIDEO_MEMORY;
    window_data.rect.x = 0;
    window_data.rect.width = intgame_iso_window_width;

    if (intgame_is_compact_interface()) {
        window_data.rect.y = 0;
        window_data.rect.height = intgame_iso_window_height;
    } else {
        window_data.rect.y = 41;
        window_data.rect.height = 400;
    }

    window_data.background_color = 0;

    if (tig_window_create(&window_data, window_handle_ptr) != TIG_OK) {
        tig_debug_printf("intgame_create_iso_window: ERROR: window create failed!\n");
        tig_exit();
        return false;
    }

    intgame_iso_window = *window_handle_ptr;
    return true;
}

// 0x5578C0
bool intgame_is_compact_interface()
{
    return intgame_compact_interface;
}

// 0x5578D0
void intgame_set_fullscreen()
{
    intgame_fullscreen = true;
}

// 0x5578E0
void intgame_toggle_interface()
{
    TigWindowData window_data;
    ResizeInfo resize_info;
    int index;

    if (!intgame_fullscreen) {
        return;
    }

    tig_debug_printf("Resizing Iso View...");

    resize_info.iso_window_handle = dword_64C52C;
    intgame_compact_interface = !intgame_compact_interface;

    if (intgame_iso_window != TIG_WINDOW_HANDLE_INVALID) {
        if (tig_window_data(dword_64C52C, &window_data) != TIG_OK) {
            tig_debug_printf("intgame_toggle_interface: ERROR: tig_window_data failed!\n");
            exit(EXIT_FAILURE);
        }

        resize_info.field_4 = window_data.rect;
        resize_info.field_14 = window_data.rect;

        if (!intgame_is_compact_interface()) {
            resize_info.field_14.x = 0;
            resize_info.field_14.y = 41;
        }

        if (intgame_compact_interface) {
            stru_64C510 = stru_5C63C0;

            for (index = 0; index < 2; index++) {
                tig_window_hide(dword_64C4F8[index]);
            }

            gamelib_resize(&resize_info);
            gameuilib_resize(&resize_info);

            compact_ui_create();
        } else {
            stru_64C510 = stru_5C63B0;

            resize_info.field_4.height = 400;
            resize_info.field_14.height = 400;

            gamelib_resize(&resize_info);
            gameuilib_resize(&resize_info);

            for (index = 0; index < 2; index++) {
                tig_window_show(dword_64C4F8[index]);
            }

            compat_ui_destroy();
        }
    }

    tig_debug_printf("completed.\n");
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
int sub_557B60()
{
    TigMouseState mouse_state;
    TigButtonData button_data;
    int x;
    int y;
    int index;

    if (dword_64C6A8 == 2) {
        tig_mouse_get_state(&mouse_state);
        x = mouse_state.x - stru_5C6390[1].x;
        y = mouse_state.y - stru_5C6390[1].y;

        for (index = 0; index < 4; index++) {
            if (tig_button_data(stru_5C6C68[index].button_handle, &button_data) != TIG_OK) {
                break;
            }

            if (x >= button_data.x
                && y >= button_data.y
                && x < button_data.x + button_data.width
                && y < button_data.y + button_data.height) {
                return index;
            }
        }
    }

    return 4;
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
