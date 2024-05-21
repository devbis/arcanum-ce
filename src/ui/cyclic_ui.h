#ifndef ARCANUM_UI_CYCLIC_UI_H_
#define ARCANUM_UI_CYCLIC_UI_H_

#include <tig/tig.h>

typedef void CyclicUiControlValueChanged(int value);

typedef struct CyclicUiControlInfo {
    /* 0000 */ tig_window_handle_t window_handle;
    /* 0004 */ int x;
    /* 0008 */ int y;
    /* 000C */ int type;
    /* 0010 */ int field_10;
    /* 0014 */ const char* text;
    /* 0018 */ const char* mes_file_path;
    /* 001C */ const char** text_array;
    /* 0020 */ int text_array_size;
    /* 0024 */ CyclicUiControlValueChanged* value_changed_callback;
    /* 0028 */ unsigned char visible; // boolean
    /* 0029 */ unsigned char enabled;
} CyclicUiControlInfo;

static_assert(sizeof(CyclicUiControlInfo) == 0x2C, "wrong size");

#endif /* ARCANUM_UI_CYCLIC_UI_H_ */
