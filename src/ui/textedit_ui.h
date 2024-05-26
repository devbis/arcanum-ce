#ifndef ARCANUM_UI_TEXTEDIT_UI_H_
#define ARCANUM_UI_TEXTEDIT_UI_H_

#include <tig/tig.h>

struct TextEdit;

typedef void(TextEditCallback)(struct TextEdit* textedit);

typedef struct TextEdit {
    /* 0000 */ unsigned int flags;
    /* 0004 */ char* buffer;
    /* 0008 */ int size;
    /* 000C */ TextEditCallback* on_enter;
    /* 0010 */ TextEditCallback* on_change;
    /* 0014 */ TextEditCallback* on_tab;
} TextEdit;

static_assert(sizeof(TextEdit) == 0x14, "wrong size");

bool textedit_ui_init(GameInitInfo* init_info);
void textedit_ui_reset();
void textedit_ui_exit();
void textedit_ui_focus(TextEdit* textedit);
void textedit_ui_unfocus();
bool textedit_ui_is_focused();
bool textedit_ui_process_message(TigMessage* msg);
void textedit_ui_clear();
void textedit_ui_restore();
void textedit_ui_commit();

#endif /* ARCANUM_UI_TEXTEDIT_UI_H_ */
