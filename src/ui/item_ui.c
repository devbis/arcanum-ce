#include "ui/item_ui.h"

#include "game/combat.h"
#include "game/critter.h"
#include "game/magictech.h"
#include "game/mt_item.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/target.h"
#include "game/trap.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"
#include "ui/spell_ui.h"

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
    uint64_t* tgt_ptr;
    S4F2810 v1;

    if (a2 == OBJ_HANDLE_NULL) {
        return;
    }

    if ((obj_field_int32_get(a1, OBJ_F_FLAGS) & OF_OFF) != 0
        && !sub_45D8D0(a1)
        && !sub_45D800(a1)) {
        return;
    }

    if (sub_551A00() == 16) {
        sub_571C80();
    }

    qword_6810D8 = a2;

    sub_5719F0(a2, &tgt_ptr);

    if ((*tgt_ptr & (Tgt_Obj_Inven & ~Tgt_Object)) == 0) {
        inven_ui_destroy();

        if (!sub_551A80(0)) {
            return;
        }
    }

    // TODO: Check, probably wrong.
    if (*tgt_ptr == Tgt_None
        || ((*tgt_ptr & Tgt_Self) != 0
            && (*tgt_ptr & Tgt_Tile) == 0)
        || *tgt_ptr == Tgt_Obj_Radius) {
        sub_4F27F0(&v1, obj_field_int64_get(a1, OBJ_F_LOCATION));
        sub_57C110(&v1);
        return;
    }

    if (sub_4B7790(a1, 4)) {
        sub_4F25B0(*tgt_ptr);
        if (sub_551A80(16)) {
            sub_4F25B0(*tgt_ptr);
            if (obj_field_int32_get(qword_6810D8, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) < 0) {
                sub_553A60(620);
                sub_553990();
            }

            if ((*tgt_ptr & Tgt_Object) != 0
                && (tig_kb_is_key_pressed(DIK_LSHIFT)
                    || tig_kb_is_key_pressed(DIK_RSHIFT))) {
                sub_4F2810(&v1, player_get_pc_obj());
                sub_571CB0(&v1);
            }
        }
    }
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
