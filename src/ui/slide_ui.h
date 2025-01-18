#ifndef ARCANUM_UI_SLIDE_UI_H_
#define ARCANUM_UI_SLIDE_UI_H_

#include "game/context.h"

typedef enum SlideUiType {
    SLIDE_UI_TYPE_END_GAME,
    SLIDE_UI_TYPE_DEATH,
    SLIDE_UI_TYPE_CREDITS,
} SlideUiType;

bool slide_ui_mod_load();
void slide_ui_mod_unload();
bool slide_ui_is_active();
void slide_ui_run(int type);
void slide_ui_queue_slide(int slide);

#endif /* ARCANUM_UI_SLIDE_UI_H_ */
