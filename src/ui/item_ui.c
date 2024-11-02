#include "ui/item_ui.h"

#include "game/magictech.h"
#include "game/mt_item.h"
#include "game/obj.h"
#include "game/target.h"
#include "game/trap.h"
#include "ui/intgame.h"

static void sub_5719F0(int64_t item_obj, uint64_t** tgt_ptr);

// 0x5CAC40
static uint64_t qword_5CAC40 = Tgt_Object;

// 0x5CAC48
static uint64_t qword_5CAC48 = Tgt_Obj_Inven;

// 0x5CAC50
static uint64_t qword_5CAC50 = Tgt_Tile | Tgt_Object;

// 0x6810D8
static int64_t qword_6810D8;

// 0x5719D0
bool item_ui_init(GameInitInfo* init_info)
{
    (void)init_info;

    return true;
}

// 0x5719E0
void item_ui_exit()
{
}

// 0x5719F0
void sub_5719F0(int64_t item_obj, uint64_t** tgt_ptr)
{
    int mana_store;
    unsigned int flags;
    int spl;

    *tgt_ptr = 0;

    if (item_obj != OBJ_HANDLE_NULL) {
        mana_store = obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_MANA_STORE);
        flags = obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS);
        spl = mt_item_spell(item_obj, 0);
        if (mana_store != 0 || (flags & OIF_IS_MAGICAL) != 0) {
            sub_459F20(spl, tgt_ptr);
        } else if (trap_is_trap_device(item_obj)) {
            *tgt_ptr = &qword_5CAC50;
        } else if (spl == -1) {
            *tgt_ptr = &qword_5CAC40;
        } else {
            sub_459F20(spl, tgt_ptr);
        }
    }
}

// 0x571AA0
void sub_571AA0(int64_t a1, int64_t a2)
{
    // TODO: Incomplete.
}

// 0x571C80
void sub_571C80()
{
    sub_551A80(0);
    qword_6810D8 = OBJ_HANDLE_NULL;
    sub_553A60(21);
}

// 0x571CB0
void sub_571CB0(S4F2810* a1)
{
    // TODO: Incomplete.
}
