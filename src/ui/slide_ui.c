#include "ui/slide_ui.h"

#include <stdio.h>

#include "game/gamelib.h"
#include "game/gfade.h"
#include "game/gsound.h"
#include "game/mes.h"
#include "game/script.h"

static void sub_569720(int type);
static bool sub_569770(tig_window_handle_t window_handle, int num);
static bool sub_569930(int num, char* slide_path, char* speech_path);
static void sub_5699F0();
static void sub_569A60();
static void sub_569A90();

// 0x67B970
static int dword_67B970;

// 0x67B974
static mes_file_handle_t slide_ui_slide_mes_file;

// 0x67B978
static int dword_67B978[100];

// 0x67BB08
static bool dword_67BB08;

// 0x5695B0
bool slide_ui_mod_load()
{
    mes_load("rules\\slide.mes", &slide_ui_slide_mes_file);
    return true;
}

// 0x5695D0
void slide_ui_mod_unload()
{
    mes_unload(slide_ui_slide_mes_file);
    slide_ui_slide_mes_file = MES_FILE_HANDLE_INVALID;
}

// 0x5695F0
bool sub_5695F0()
{
    return dword_67BB08;
}

// 0x569600
void sub_569600(int a1)
{
    TigWindowData window_data;
    tig_window_handle_t window_handle;
    tig_sound_handle_t sound_handle;
    int index;

    dword_67BB08 = true;
    sub_569A90();
    sub_569720(a1);

    if (a1 == 1) {
        if (dword_67B970 == 0 && slide_ui_slide_mes_file != -1) {
            sub_569AA0(0);
        }
    }

    sub_5699F0();

    if (dword_67B970 > 0) {
        window_data.flags = TIG_WINDOW_FLAG_0x08;
        window_data.rect.x = 0;
        window_data.rect.y = 0;
        window_data.rect.width = 800;
        window_data.rect.height = 600;
        window_data.background_color = 0;
        if (tig_window_create(&window_data, &window_handle) == TIG_OK) {
            gsound_stop_all(25);
            tig_mouse_hide();
            sound_handle = gsound_play_music_indefinitely("sound\\music\\DwarvenMusic.mp3", 25);

            for (index = 0; index < dword_67B970; index++) {
                if (!sub_569770(window_handle, dword_67B978[index])) {
                    break;
                }
            }

            tig_sound_destroy(sound_handle);
            tig_window_destroy(window_handle);
            tig_mouse_show();
            tig_window_display();
        }
    }

    if (a1 == 2) {
        sub_569A60();
    }

    dword_67BB08 = false;
}

// 0x569720
void sub_569720(int type)
{
    Script scr;
    ScriptInvocation invocation;

    scr.num = dword_5A5700[type];

    invocation.script = &scr;
    invocation.field_20 = 1;
    invocation.triggerer_obj = OBJ_HANDLE_NULL;
    invocation.attachee_obj = OBJ_HANDLE_NULL;
    invocation.extra_obj = OBJ_HANDLE_NULL;
    invocation.line = 0;
    sub_4449B0(&invocation);
}

// 0x569770
bool sub_569770(tig_window_handle_t window_handle, int num)
{
    TigMessage msg;
    TigRect rect;
    TigBmp bmp;
    char speech_path[TIG_MAX_PATH];
    tig_sound_handle_t speech_handle;
    bool success = true;
    bool stop;

    rect.x = 0;
    rect.y = 0;
    rect.width = 800;
    rect.height = 600;

    if (sub_569930(num, bmp.name, speech_path)) {
        if (tig_bmp_create(&bmp) == TIG_OK) {
            tig_bmp_copy_to_window(&bmp, &rect, window_handle, &rect);
            tig_bmp_destroy(&bmp);

            sub_51E850(window_handle);
            tig_window_display();

            sub_569A60();

            speech_handle = gsound_play_voice(speech_path, 0);
            while (tig_message_dequeue(&msg) == TIG_OK) {
                if (msg.type == TIG_MESSAGE_REDRAW) {
                    sub_4045A0();
                }
            }

            stop = false;
            do {
                tig_ping();

                while (tig_message_dequeue(&msg) == TIG_OK) {
                    if (msg.type == TIG_MESSAGE_KEYBOARD) {
                        if (msg.data.keyboard.pressed == false) {
                            stop = true;
                            if (msg.data.keyboard.key == DIK_ESCAPE) {
                                success = false;
                            }
                        }
                    } else if (msg.type == TIG_MESSAGE_MOUSE) {
                        if (msg.data.mouse.event == TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP
                            || msg.data.mouse.event == TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP
                            || msg.data.mouse.event == TIG_MESSAGE_MOUSE_MIDDLE_BUTTON_UP) {
                            stop = true;
                        }
                    } else if (msg.type == TIG_MESSAGE_REDRAW) {
                        sub_4045A0();
                    }
                }

                if (stop) {
                    break;
                }
            } while (speech_handle != TIG_SOUND_HANDLE_INVALID && tig_sound_is_playing(speech_handle));

            tig_sound_destroy(speech_handle);
        }

        while (tig_message_dequeue(&msg) == TIG_OK) {
            if (msg.type == TIG_MESSAGE_REDRAW) {
                sub_4045A0();
            }
        }

        sub_5699F0();
    }

    return success;
}

// 0x569930
bool sub_569930(int num, char* slide_path, char* speech_path)
{
    MesFileEntry mes_file_entry;
    char buffer[2000];
    char* tok;

    mes_file_entry.num = num;
    if (!mes_search(slide_ui_slide_mes_file, &mes_file_entry)) {
        return false;
    }

    strcpy(buffer, mes_file_entry.str);

    tok = strtok(buffer, " \t\n");
    if (tok == NULL) {
        return false;
    }

    sprintf(slide_path, "slide\\%s", tok);

    tok = strtok(NULL, " \t\n");
    if (tok == NULL) {
        return false;
    }

    sprintf(speech_path, "sound\\speech\\slide\\%s", tok);

    return true;
}

// 0x5699F0
void sub_5699F0()
{
    FadeData fade_data;

    fade_data.field_0 = 0;
    fade_data.field_10 = 0;
    fade_data.duration = 2.0f;
    fade_data.steps = 48;
    fade_data.color = tig_color_make(0, 0, 0);
    sub_4BDFA0(&fade_data);
}

// 0x569A60
void sub_569A60()
{
    FadeData fade_data;

    fade_data.field_0 = 1;
    fade_data.duration = 2.0f;
    fade_data.steps = 48;
    sub_4BDFA0(&fade_data);
}

// 0x569A90
void sub_569A90()
{
    dword_67B970 = 0;
}

// 0x569AA0
void sub_569AA0(int a1)
{
    if (dword_67B970 < 100) {
        dword_67B978[dword_67B970++] = a1;
    }
}
