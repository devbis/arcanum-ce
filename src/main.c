#include <tig/tig.h>

#include <stdio.h>

#include "game/ai.h"
#include "game/anim.h"
#include "game/combat.h"
#include "game/critter.h"
#include "game/dialog.h"
#include "game/gamelib.h"
#include "game/gmovie.h"
#include "game/gsound.h"
#include "game/level.h"
#include "game/light_scheme.h"
#include "game/location.h"
#include "game/magictech.h"
#include "game/map.h"
#include "game/multiplayer.h"
#include "game/name.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/roof.h"
#include "game/roof.h"
#include "game/script.h"
#include "game/scroll.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/tech.h"
#include "game/wallcheck.h"
#include "ui/charedit_ui.h"
#include "ui/dialog_ui.h"
#include "ui/gameuilib.h"
#include "ui/intgame.h"
#include "ui/iso.h"
#include "ui/logbook_ui.h"
#include "ui/mainmenu_ui.h"
#include "ui/textedit_ui.h"
#include "ui/wmap_rnd.h"
#include "ui/wmap_ui.h"

static void main_loop();
static void handle_mouse_scroll();
static void handle_keyboard_scroll();

// 0x59A040
static float flt_59A040 = 1.0f;

// 0x5CFF00
static int dword_5CFF00;

// 0x401000
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    TigInitInfo init_info;
    TigVideoScreenshotSettings screenshotter;
    GameInitInfo game_init_info;
    char* pch;
    int value;
    tig_art_id_t cursor_art_id;
    location_t pc_starting_location;
    char msg[80];

    init_info.texture_width = 1024;
    init_info.texture_height = 1024;
    init_info.flags = TIG_INITIALIZE_3D_HARDWARE_DEVICE | TIG_INITIALIZE_3D_SOFTWARE_DEVICE | TIG_INITIALIZE_VIDEO_MEMORY;

    pch = lpCmdLine;
    while (*pch != '\0') {
        *pch = (unsigned char)tolower(*(unsigned char*)pch);
        pch++;
    }

    if (strstr(lpCmdLine, "-fps") != NULL) {
        init_info.flags |= TIG_INITIALIZE_FPS;
    }

    if (strstr(lpCmdLine, "-nosound") != NULL) {
        init_info.flags |= TIG_INITIALIZE_NO_SOUND;
    }

    if (strstr(lpCmdLine, "-no3d") != NULL) {
        init_info.flags &= ~(TIG_INITIALIZE_ANY_3D | TIG_INITIALIZE_VIDEO_MEMORY);
    }

    if (strstr(lpCmdLine, "-3dref") != NULL) {
        init_info.flags &= ~(TIG_INITIALIZE_3D_HARDWARE_DEVICE | TIG_INITIALIZE_3D_SOFTWARE_DEVICE);
        init_info.flags |= TIG_INITIALIZE_3D_REF_DEVICE;
    }

    if (strstr(lpCmdLine, "-doublebuffer") != NULL) {
        init_info.flags |= TIG_INITIALIZE_DOUBLE_BUFFER;
    }

    pch = strstr(lpCmdLine, "-vidfreed");
    if (pch != NULL) {
        value = atoi(pch + 8);
        if (value > 0) {
            tig_art_cache_set_video_memory_fullness(value);
        }
    }

    if (strstr(lpCmdLine, "-animcatchup") != NULL) {
        sub_423FC0();
    }

    if (strstr(lpCmdLine, "-animdebug") != NULL) {
        sub_4232F0();
    }

    if (strstr(lpCmdLine, "-norandom") != NULL) {
        sub_5589D0();
    }

    if (strstr(lpCmdLine, "-nonmsmousez") != NULL) {
        tig_mouse_set_z_axis_enabled(false);
    } else if (strstr(lpCmdLine, "-msmousez") != NULL) {
        tig_mouse_set_z_axis_enabled(true);
    }

    sub_549A70();

    pch = strstr(lpCmdLine, "-pathlimit");
    if (pch != NULL) {
        value = atoi(pch + 10);
        if (value > 0) {
            sub_421AC0(value);
        }
    }

    pch = strstr(lpCmdLine, "-pathtimelimit");
    if (pch != NULL) {
        // FIXME: Length is wrong, should be 14.
        value = atoi(pch + 10);
        if (value > 0) {
            sub_421AE0(value);
        }
    }

    if (strstr(lpCmdLine, "-mpautojoin") != NULL) {
        tig_net_auto_join_enable();
    }

    if (strstr(lpCmdLine, "-mpnobcast") != NULL) {
        tig_net_no_broadcast_enable();
    }

    if (strstr(lpCmdLine, "-fullscreen") != NULL) {
        intgame_set_fullscreen();
        intgame_toggle_interface();
    }

    if (strstr(lpCmdLine, "-patchlvl") != NULL) {
        sub_404640();
    }

    init_info.width = 800;
    init_info.height = 600;
    init_info.bpp = 16;
    init_info.instance = hInstance;
    init_info.art_file_path_resolver = gamelib_art_file_path_resolver;
    init_info.art_id_reset_func = gamelib_art_id_reset;
    init_info.sound_file_path_resolver = gsound_resolve_path;

    if (tig_init(&init_info) != TIG_OK) {
        return EXIT_SUCCESS; // FIXME: Should be `EXIT_FAILURE`.
    }

    screenshotter.key = DIK_F12;
    screenshotter.field_4 = 0;
    tig_video_screenshot_set_settings(&screenshotter);

    if ((init_info.flags & TIG_INITIALIZE_ANY_3D) != 0) {
        intgame_set_iso_window_flags(intgame_get_iso_window_flags() | TIG_WINDOW_HAVE_FLUSH);
    }

    intgame_set_iso_window_width(init_info.width);
    intgame_set_iso_window_height(init_info.height);
    if (!intgame_create_iso_window(&(game_init_info.iso_window_handle))) {
        tig_exit();
        return EXIT_SUCCESS; // FIXME: Should be `EXIT_FAILURE`.
    }

    tig_mouse_hide();

    game_init_info.editor = false;
    game_init_info.invalidate_func = sub_5581A0;
    game_init_info.redraw_func = iso_redraw;

    if (!gamelib_init(&game_init_info)) {
        tig_window_destroy(game_init_info.iso_window_handle);
        tig_exit();
        return EXIT_SUCCESS; // FIXME: Should be `EXIT_FAILURE`.
    }

    if (strstr(lpCmdLine, "-dialogcheck") != NULL) {
        sub_418030();
    }

    if (strstr(lpCmdLine, "-dialognumber") != NULL) {
        sub_4182C0();
    }

    if (strstr(lpCmdLine, "-gendercheck") != NULL) {
        map_enable_gender_check();
    }

    pch = strstr(lpCmdLine, "-scrollfps:");
    if (pch != NULL) {
        value = atoi(pch + 11);
        scroll_set_fps(value);
    } else {
        scroll_set_fps(35);
    }

    pch = strstr(lpCmdLine, "-scrolldist:");
    if (pch != NULL) {
        value = atoi(pch + 12);
        scroll_set_distance(value);
    } else {
        scroll_set_distance(10);
    }

    pch = strstr(lpCmdLine, "-mod:");
    if (pch != NULL) {
        gamelib_default_module_name_set(pch + 5);
    }

    tig_art_interface_id_create(0, 0, 0, 0, &cursor_art_id);
    tig_mouse_cursor_set_art_id(cursor_art_id);

    if (!gameuilib_init(&game_init_info)) {
        gameuilib_exit();
        tig_window_destroy(game_init_info.iso_window_handle);
        tig_exit();
        return EXIT_SUCCESS; // FIXME: Should be `EXIT_FAILURE`.
    }

    if (!gamelib_mod_load(gamelib_default_module_name_get())) {
        tig_debug_printf("Error loading default module %s\n",
            gamelib_default_module_name_get());
        // FIXME: Missing graceful shutdown sequence.
        return EXIT_SUCCESS; // FIXME: Should be `EXIT_FAILURE`.
    }

    if (!gameuilib_mod_load()) {
        tig_debug_printf("Error loading UI module data\n");
        // FIXME: Missing graceful shutdown sequence.
        return EXIT_SUCCESS; // FIXME: Should be `EXIT_FAILURE`.
    }

    gmovie_play(8, GAME_MOVIE_BLACK_OUT, 0);

    if (!mainmenu_ui_handle()) {
        gameuilib_exit();
        gamelib_exit();
        tig_exit();
        tig_memory_print_stats(TIG_MEMORY_STATS_PRINT_ALL_BLOCKS);
        return EXIT_SUCCESS; // FIXME: Should be `EXIT_FAILURE`.
    }

    if (strstr(lpCmdLine, "-logcheck") != NULL) {
        sub_5407F0();
    }

    tig_debug_printf("[Beginning Game]\n");

    pc_starting_location = obj_field_int64_get(player_get_pc_obj(), OBJ_F_LOCATION);
    sprintf(msg, "Player Start Position: x: %d, y: %d",
        (int)LOCATION_GET_X(pc_starting_location),
        (int)LOCATION_GET_Y(pc_starting_location));
    tig_debug_printf("%s\n", msg);

    main_loop();

    gameuilib_mod_unload();
    gamelib_mod_unload();
    gameuilib_exit();
    gamelib_exit();
    tig_exit();
    tig_memory_print_stats(TIG_MEMORY_STATS_PRINT_ALL_BLOCKS);

    return EXIT_SUCCESS;
}

// 0x401560
void main_loop()
{
    location_t location;
    object_id_t pc_obj;
    object_id_t party_member_obj;
    tig_art_id_t art_id;
    bool enable_profiler = false;
    bool disable_profiler = false;
    bool output_profile_data = false;
    TigMessage message;
    int index;
    TigMouseState mouse_state;
    int coords[2];
    char version_str[40];
    char mouse_state_str[20];
    char story_state_str[80];

    if (!sub_4B8730(400, 200, &location)) {
        return;
    }

    pc_obj = player_get_pc_obj();
    sub_43E770(pc_obj, location, 0, 0);
    sub_4B8CE0(location);

    art_id = obj_field_int32_get(pc_obj, OBJ_F_CURRENT_AID);
    art_id = tig_art_id_frame_set(art_id, 0);
    art_id = sub_503E50(art_id, 0);
    object_set_current_aid(pc_obj, art_id);

    sub_43D280(pc_obj, OF_OFF);
    sub_430460();
    sub_551160();
    sub_54AEE0(0);
    sub_54AEE0(1);

    while (1) {
        if (enable_profiler) {
            sub_550770(-1, "Enabling profiler...\n");
            enable_profiler = false;
        }

        if (disable_profiler) {
            sub_550770(-1, "Disabling profiler...\n");
            disable_profiler = false;
        }

        if (output_profile_data) {
            sub_550770(-1, "Outputing profile data...\n");
            output_profile_data = false;
        }

        tig_ping();
        gamelib_ping();
        iso_redraw();
        tig_window_display();

        pc_obj = player_get_pc_obj();

        while (tig_message_dequeue(&message) == TIG_OK) {
            if (message.type == TIG_MESSAGE_QUIT
                && !sub_541690()) {
                sub_541710();
                return;
            }

            if (message.type == TIG_MESSAGE_REDRAW) {
                sub_4045A0();
            }

            sub_54DE50(&message);

            if (sub_53EAD0()) {
                mainmenu_ui_start(0);
                if (!mainmenu_ui_handle()) {
                    return;
                }
            }

            if (sub_551F70(sub_551A00())) {
                handle_keyboard_scroll();
            }

            if (message.type == TIG_MESSAGE_KEYBOARD) {
                if (!message.data.keyboard.pressed) {
                    switch (message.data.keyboard.key) {
                    case DIK_ESCAPE:
                        if (sub_567A10()
                            || sub_560F90()
                            || (tig_net_flags & TIG_NET_CONNECTED) != 0
                            || sub_4B6D70()
                            && player_get_pc_obj() != sub_4B6D80()) {
                            mainmenu_ui_start(3);
                        } else {
                            mainmenu_ui_start(2);
                        }
                        if (!mainmenu_ui_handle()) {
                            return;
                        }
                        break;
                    case DIK_F10:
                        intgame_toggle_interface();
                        tig_debug_printf("iso_redraw...");
                        iso_redraw();
                        tig_debug_printf("tig_window_display...");
                        tig_window_display();
                        tig_debug_printf("completed.\n");
                        break;
                    case DIK_O:
                        if (!textedit_ui_is_focused()) {
                            mainmenu_ui_start(4);
                            if (!mainmenu_ui_handle()) {
                                return;
                            }
                        }
                        break;
                    case DIK_F7:
                        if (!sub_45D8D0(player_get_pc_obj())) {
                            if (sub_560F90()) {
                                sub_560F40();
                                tig_ping();
                                gamelib_ping();
                                iso_redraw();
                                tig_window_display();
                            }

                            if (!sub_4B6D70() || player_get_pc_obj() == sub_4B6D80()) {
                                if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
                                    sub_551A80(0);
                                    sub_551A80(0);
                                    sub_5496E0();
                                    gamelib_save("SlotAuto", "Auto-Save");
                                    sub_549750();
                                }
                            } else {
                                sub_549760();
                            }
                        }
                        break;
                    case DIK_F8:
                        if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
                            sub_549770();
                            sub_543220();
                            sub_549780();
                        }
                        break;
                    case DIK_F11:
                        if (sub_402CB0() >= 3) {
                            for (index = 0; index < 80; index++) {
                                sub_4B1790(pc_obj, index, 1);
                            }

                            stat_set_base(pc_obj, STAT_INTELLIGENCE, 20);
                            stat_set_base(pc_obj, STAT_WILLPOWER, 20);

                            for (index = 0; index < 80; index++) {
                                sub_4B1790(pc_obj, index, 1);
                            }

                            sub_44FE20();
                            critter_fatigue_damage_set(pc_obj, 0);
                            sub_551160();
                            sub_54AEE0(0);
                            sub_54AEE0(1);
                        }
                        break;
                    case DIK_J:
                        for (index = 0; index < 8; index++) {
                            sub_526D20(index);
                        }
                        break;
                    }

                    if (!textedit_ui_is_focused()) {
                        if (sub_402CB0() >= 3) {
                            switch (message.data.keyboard.key) {
                            case DIK_H:
                                sub_45C1C0(3600000);
                                break;
                            case DIK_N:
                                object_set_hp_damage(pc_obj, 0);
                                critter_fatigue_damage_set(pc_obj, 0);
                                sub_43D280(pc_obj, 1028);
                                sub_45EC80(pc_obj);
                                break;
                            case DIK_GRAVE:
                                stat_set_base(pc_obj, STAT_INTELLIGENCE, 20);
                                for (index = 0; index < 8; index++) {
                                    tech_inc_degree(pc_obj, index);
                                }
                                sub_55A230();
                                break;
                            case DIK_P:
                                sub_4AF860();
                                break;
                            }
                        }

                        if (sub_402CB0() >= 2) {
                            switch (message.data.keyboard.key) {
                            case DIK_D:
                                if (light_scheme_get() == LIGHT_SCHEME_DEFAULT_LIGHTING) {
                                    light_scheme_set(dword_5CFF00, light_scheme_get_hour());
                                } else {
                                    dword_5CFF00 = light_scheme_get();
                                    light_scheme_set(LIGHT_SCHEME_DEFAULT_LIGHTING, light_scheme_get_hour());
                                }
                                break;
                            case DIK_Y:
                                if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
                                    sub_45F110(pc_obj, level_get_experience_points_to_next_level(pc_obj));
                                } else if ((tig_net_flags & TIG_NET_HOST) != 0) {
                                    party_member_obj = multiplayer_find_first_player_obj();
                                    while (party_member_obj != OBJ_HANDLE_NULL) {
                                        sub_45F110(party_member_obj, level_get_experience_points_to_next_level(party_member_obj));
                                        party_member_obj = multiplayer_find_next_player_obj();
                                    }
                                }
                                break;
                            case DIK_4:
                                if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                                    // TODO: Incomplete (give $1000).
                                }
                                break;
                            }
                        }

                        if (sub_402CB0() >= 1) {
                            switch (message.data.keyboard.key) {
                            case DIK_V:
                                gamelib_copy_version(version_str, NULL, NULL);
                                if (tig_video_3d_check_hardware() == TIG_OK) {
                                    strcat(version_str, " [hardware renderer");
                                } else {
                                    strcat(version_str, " [software renderer");
                                }

                                if (tig_video_check_gamma_control() == TIG_OK) {
                                    strcat(version_str, " with gamma support]");
                                } else {
                                    strcat(version_str, " without gamma support]");
                                }
                                sub_550770(-1, version_str);
                                break;
                            case DIK_E:
                                tig_mouse_get_state(&mouse_state);
                                sub_4B8730(mouse_state.x, mouse_state.y, coords);
                                sprintf(mouse_state_str, "x: %d, y: %d", coords[0], coords[1]);
                                tig_debug_printf("%s\n", mouse_state_str);
                                sub_550770(-1, mouse_state_str);
                                break;
                            case DIK_U:
                                sprintf(story_state_str, "Current Story State: %d", sub_445090());
                                sub_550770(-1, story_state_str);
                                break;
                            case DIK_LBRACKET:
                                switch (sub_442040()) {
                                case 0:
                                    sub_442010(TIG_ART_BLT_BLEND_ALPHA_CONST);
                                    break;
                                case TIG_ART_BLT_BLEND_ALPHA_CONST:
                                    sub_442010(TIG_ART_BLT_BLEND_ALPHA_STIPPLE_S);
                                    break;
                                case TIG_ART_BLT_BLEND_ALPHA_STIPPLE_S:
                                    sub_442010(0);
                                    break;
                                }
                                break;
                            case DIK_RBRACKET:
                                switch (sub_43A130()) {
                                case 0:
                                    sub_43A110(TIG_ART_BLT_BLEND_ALPHA_CONST);
                                    break;
                                case TIG_ART_BLT_BLEND_ALPHA_CONST:
                                    sub_43A110(TIG_ART_BLT_BLEND_ALPHA_STIPPLE_S);
                                    break;
                                case TIG_ART_BLT_BLEND_ALPHA_STIPPLE_S:
                                    sub_43A110(0);
                                    break;
                                }
                                break;
                            case DIK_APOSTROPHE:
                                settings_set_value(&settings, "shadows", 1 - settings_get_value(&settings, "shadows"));
                                break;
                            case DIK_BACKSLASH:
                                sub_438500(!sub_438520());
                                break;
                            case DIK_NUMPAD7:
                                if (tig_kb_is_key_pressed(DIK_LCONTROL) || tig_kb_is_key_pressed(DIK_RCONTROL)) {
                                    disable_profiler = true;
                                }
                                break;
                            case DIK_NUMPAD8:
                                if (tig_kb_is_key_pressed(DIK_LCONTROL) || tig_kb_is_key_pressed(DIK_RCONTROL)) {
                                    enable_profiler = true;
                                }
                                break;
                            case DIK_NUMPAD9:
                                if (tig_kb_is_key_pressed(DIK_LCONTROL) || tig_kb_is_key_pressed(DIK_RCONTROL)) {
                                    output_profile_data = true;
                                }
                                break;
                            case DIK_G:
                                if (tig_kb_is_key_pressed(DIK_LCONTROL) || tig_kb_is_key_pressed(DIK_RCONTROL)) {
                                    flt_59A040 = 1.0f;
                                    tig_video_set_gamma(flt_59A040);
                                } else {
                                    if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                                        if (flt_59A040 > 0.1f) {
                                            flt_59A040 -= 0.1f;
                                            tig_video_set_gamma(flt_59A040);
                                        }
                                    } else {
                                        if (flt_59A040 < 1.9f) {
                                            flt_59A040 += 0.1f;
                                            tig_video_set_gamma(flt_59A040);
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            } else {
                if (sub_541680()) {
                    if (!mainmenu_ui_handle()) {
                        return;
                    }
                }
            }
        }

        if (sub_551F70(sub_551A00())) {
            handle_mouse_scroll();
        }
    }
}

// 0x401F50
void handle_mouse_scroll()
{
    TigMouseState mouse_state;
    tig_mouse_get_state(&mouse_state);

    if (mouse_state.x == 0) {
        if (mouse_state.y == 0) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP_LEFT);
        } else if (mouse_state.y == 600 - 1) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN_LEFT);
        } else {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_LEFT);
        }
    } else if (mouse_state.x == 800 - 1) {
        if (mouse_state.y == 0) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP_RIGHT);
        } else if (mouse_state.y == 600 - 1) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN_RIGHT);
        } else {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_RIGHT);
        }
    } else {
        if (mouse_state.y == 0) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP);
        } else if (mouse_state.y == 600 - 1) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN);
        } else {
            scroll_stop_scrolling();
        }
    }
}

// 0x402010
void handle_keyboard_scroll()
{
    if (tig_kb_is_key_pressed(DIK_UP)) {
        if (tig_kb_is_key_pressed(DIK_LEFT)) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP_LEFT);
        } else if (tig_kb_is_key_pressed(DIK_RIGHT)) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP_RIGHT);
        } else {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP);
        }
    } else if (tig_kb_is_key_pressed(DIK_DOWN)) {
        if (tig_kb_is_key_pressed(DIK_LEFT)) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN_LEFT);
        } else if (tig_kb_is_key_pressed(DIK_RIGHT)) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN_RIGHT);
        } else {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN);
        }
    } else if (tig_kb_is_key_pressed(DIK_LEFT)) {
        scroll_start_scrolling_in_direction(SCROLL_DIRECTION_LEFT);
    } else if (tig_kb_is_key_pressed(DIK_RIGHT)) {
        scroll_start_scrolling_in_direction(SCROLL_DIRECTION_RIGHT);
    }
}
