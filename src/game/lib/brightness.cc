#include "game/lib/brightness.h"

#include "tig/debug.h"
#include "tig/video.h"

#define BRIGHTNESS "brightness"

// 0x4BE0B0
bool brightness_init(GameContext* ctx)
{
    settings_register(&settings, BRIGHTNESS, "0", brightness_changed);
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

    if (!changing) {
        changing = true;

        int brightness = settings_get(&settings, BRIGHTNESS);
        if (sub_51FFE0(brightness * 0.1 + 1.0) != TIG_OK) {
            tig_debug_printf("brightness_changed: ERROR: Gamma out of Range!  Attempting to Correct bounds!\n");

            int corrected_brightness;
            if (brightness > 9) {
                corrected_brightness = 9;
            } else {
                corrected_brightness = 1;
            }
            settings_set_value(&settings, BRIGHTNESS, corrected_brightness);

            if (sub_51FFE0(corrected_brightness * 0.1 + 1.0) != TIG_OK) {
                tig_debug_printf("brightness_changed: ERROR: CORRECTION FAILED: Gamma STILL out of Range!\n");
            }
        }

        changing = false;
    }
}
