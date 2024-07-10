#include "game/brightness.h"

#include <tig/tig.h>

#include "game/gamelib.h"

#define BRIGHTNESS "brightness"

static void brightness_changed();

// 0x4BE0B0
bool brightness_init(GameInitInfo* init_info)
{
    (void)init_info;

    settings_add(&settings, BRIGHTNESS, "0", brightness_changed);
    brightness_changed();

    return true;
}

// 0x4BE0E0
void brightness_exit()
{
}

// 0x4BE0F0
void brightness_changed()
{
    // 0x5FC4C4
    static bool changing;

    int brightness;

    if (!changing) {
        changing = true;

        brightness = settings_get_value(&settings, BRIGHTNESS);
        if (tig_video_set_gamma(brightness * 0.1 + 1.0f) == TIG_OK) {
            tig_debug_printf("brightness_changed: ERROR: Gamma out of Range!  Attempting to Correct bounds!\n");

            brightness = brightness > 9 ? 9 : 1;
            settings_set_value(&settings, BRIGHTNESS, brightness);

            if (tig_video_set_gamma(brightness * 0.1f + 1.0f) != TIG_OK) {
                tig_debug_printf("brightness_changed: ERROR: CORRECTION FAILED: Gamma STILL out of Range!\n");
            }
        }

        changing = false;
    }
}
