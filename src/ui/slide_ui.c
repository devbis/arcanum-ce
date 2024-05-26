#include "ui/slide_ui.h"

#include <tig/tig.h>

#include "game/lib/gfade.h"

// 0x67B974
static int slide_ui_slide_mes_file;

// 0x67B978
static int dword_67B978[100];

// 0x67BB08
static bool dword_67BB08;

// 0x5695B0
bool slide_ui_mod_load()
{
    message_load("rules\\slide.mes", &slide_ui_slide_mes_file);
    return true;
}

// 0x5695D0
void slide_ui_mod_unload()
{
    message_unload(slide_ui_slide_mes_file);
    slide_ui_slide_mes_file = -1;
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
            sub_41C340(25);
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
void sub_569720()
{
    // TODO: Incomplete.
}

// 0x569770
void sub_569770()
{
    // TODO: Incomplete.
}

// 0x569930
bool sub_569930(int num, char* slide_path, char* speech_path)
{
    MesFileEntry mes_file_entry;
    char buffer[2000];
    char* tok;

    mes_file_entry.num = num;
    if (message_find(slide_ui_slide_mes_file, &mes_file_entry)) {
        return false;
    }

    strcpy(buffer, mes_file_entry.text);

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
