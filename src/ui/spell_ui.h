#ifndef ARCANUM_UI_SPELL_UI_H_
#define ARCANUM_UI_SPELL_UI_H_

#include "game/context.h"

bool spell_ui_init(GameInitInfo* init_info);
void spell_ui_reset();
void spell_ui_exit();
bool spell_ui_save(TigFile* stream);
bool spell_ui_load(GameLoadInfo* load_info);
void sub_57BC30();
void sub_57BFF0();
void sub_57C040(int64_t obj, int index);
void sub_57C080(int64_t obj, int spl);
void sub_57C0B0();
void sub_57C0E0();
void sub_57C320(int a1);
void sub_57C370(int index);
void sub_57C3F0(int index);
void sub_57C4B0();
bool sub_57C520(int index);
void sub_57C540(int64_t obj, int index);
void sub_57C670(int64_t obj, int index);

#endif /* ARCANUM_UI_SPELL_UI_H_ */
