#ifndef ARCANUM_UI_INVEN_UI_H_
#define ARCANUM_UI_INVEN_UI_H_

#include "game/context.h"
#include "game/mp_utils.h"

bool inven_ui_init(GameInitInfo* init_info);
void inven_ui_exit();
void inven_ui_reset();
bool sub_572370(int64_t a1, int64_t a2, int a3);
bool sub_572510(int64_t a1, int64_t a2, int a3);
bool sub_572240(int64_t a1, int64_t a2, int a3);
void sub_572640(int64_t a1, int64_t a2, int a3);
bool inven_ui_create(int64_t a1, int64_t a2, int a3);
void inven_ui_destroy();
void sub_573590(int64_t obj);
int inven_ui_is_created();
int64_t sub_573600();
int64_t sub_573620();
void sub_573630(int64_t obj);
void sub_5736E0();
void sub_573730();
void sub_573740(int64_t a1, bool a2);
bool sub_575080(int64_t a1, int64_t a2);
void sub_575770();
void sub_575930();
void sub_575C50(int64_t obj);
void sub_576100(int64_t obj);
void sub_578B80(int a1);
bool sub_578EA0(Packet81* pkt);
int64_t sub_579760();

#endif /* ARCANUM_UI_INVEN_UI_H_ */
