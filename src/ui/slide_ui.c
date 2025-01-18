#include "ui/slide_ui.h"

#include <stdio.h>

#include "game/gamelib.h"
#include "game/gfade.h"
#include "game/gsound.h"
#include "game/mes.h"
#include "game/script.h"

#define MAX_QUEUE_SIZE 100

static void slide_ui_prepare(int type);
static bool slide_ui_do_slide(tig_window_handle_t window_handle, int slide);
static bool slide_ui_get_assets(int slide, char* bmp_path, char* speech_path);
static void slide_ui_fade_out();
static void slide_ui_fade_in();
static void slide_ui_queue_clear();

// 0x67B970
static int slide_ui_queue_size;

// 0x67B974
static mes_file_handle_t slide_ui_slide_mes_file;

// 0x67B978
static int slide_ui_queue[MAX_QUEUE_SIZE];

// 0x67BB08
static bool slide_ui_active;

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
bool slide_ui_is_active()
{
    return slide_ui_active;
}

// 0x569600
void slide_ui_run(int type)
{
    TigWindowData window_data;
    tig_window_handle_t window_handle;
    tig_sound_handle_t sound_handle;
    int index;

    slide_ui_active = true;

    slide_ui_queue_clear();
    slide_ui_prepare(type);

    if (type == SLIDE_UI_TYPE_DEATH
        && slide_ui_queue_size == 0
        && slide_ui_slide_mes_file != MES_FILE_HANDLE_INVALID) {
        slide_ui_queue_slide(0);
    }

    slide_ui_fade_out();

    if (slide_ui_queue_size > 0) {
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

            for (index = 0; index < slide_ui_queue_size; index++) {
                if (!slide_ui_do_slide(window_handle, slide_ui_queue[index])) {
                    break;
                }
            }

            tig_sound_destroy(sound_handle);
            tig_window_destroy(window_handle);
            tig_mouse_show();
            tig_window_display();
        }
    }

    if (type == SLIDE_UI_TYPE_CREDITS) {
        slide_ui_fade_in();
    }

    slide_ui_active = false;
}

// 0x569720
void slide_ui_prepare(int type)
{
    Script scr;
    ScriptInvocation invocation;

    scr.num = dword_5A5700[type];

    invocation.script = &scr;
    invocation.attachment_point = SAP_USE;
    invocation.triggerer_obj = OBJ_HANDLE_NULL;
    invocation.attachee_obj = OBJ_HANDLE_NULL;
    invocation.extra_obj = OBJ_HANDLE_NULL;
    invocation.line = 0;
    sub_4449B0(&invocation);
}

// 0x569770
bool slide_ui_do_slide(tig_window_handle_t window_handle, int slide)
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

    if (slide_ui_get_assets(slide, bmp.name, speech_path)) {
        if (tig_bmp_create(&bmp) == TIG_OK) {
            tig_bmp_copy_to_window(&bmp, &rect, window_handle, &rect);
            tig_bmp_destroy(&bmp);

            sub_51E850(window_handle);
            tig_window_display();

            slide_ui_fade_in();

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

        slide_ui_fade_out();
    }

    return success;
}

// 0x569930
bool slide_ui_get_assets(int slide, char* bmp_path, char* speech_path)
{
    MesFileEntry mes_file_entry;
    char buffer[2000];
    char* tok;

    mes_file_entry.num = slide;
    if (!mes_search(slide_ui_slide_mes_file, &mes_file_entry)) {
        return false;
    }

    strcpy(buffer, mes_file_entry.str);

    tok = strtok(buffer, " \t\n");
    if (tok == NULL) {
        return false;
    }

    sprintf(bmp_path, "slide\\%s", tok);

    tok = strtok(NULL, " \t\n");
    if (tok == NULL) {
        return false;
    }

    sprintf(speech_path, "sound\\speech\\slide\\%s", tok);

    return true;
}

// 0x5699F0
void slide_ui_fade_out()
{
    FadeData fade_data;

    fade_data.flags = 0;
    fade_data.field_10 = 0;
    fade_data.duration = 2.0f;
    fade_data.steps = 48;
    fade_data.color = tig_color_make(0, 0, 0);
    gfade_run(&fade_data);
}

// 0x569A60
void slide_ui_fade_in()
{
    FadeData fade_data;

    fade_data.flags = FADE_IN;
    fade_data.duration = 2.0f;
    fade_data.steps = 48;
    gfade_run(&fade_data);
}

// 0x569A90
void slide_ui_queue_clear()
{
    slide_ui_queue_size = 0;
}

// 0x569AA0
void slide_ui_queue_slide(int slide)
{
    if (slide_ui_queue_size < MAX_QUEUE_SIZE) {
        slide_ui_queue[slide_ui_queue_size++] = slide;
    }
}
