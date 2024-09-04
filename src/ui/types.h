#ifndef ARCANUM_UI_TYPES_H_
#define ARCANUM_UI_TYPES_H_

#include <tig/tig.h>

typedef struct UiButtonInfo {
    int x;
    int y;
    int art_num;
    tig_button_handle_t button_handle;
} UiButtonInfo;

#endif /* ARCANUM_UI_TYPES_H_ */
