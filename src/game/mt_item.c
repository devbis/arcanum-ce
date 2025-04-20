#include "game/mt_item.h"

#include "game/item.h"
#include "game/magictech.h"
#include "game/obj.h"
#include "game/random.h"

static void sub_4CB800(int64_t a1, int64_t a2, int64_t a3, unsigned int flags);
static void sub_4CB830(int64_t a1, int64_t a2, int64_t a3, int64_t a4, unsigned int flags);
static void sub_4CBFF0(int64_t a1, int64_t a2, unsigned int flags);

// 0x5B72AC
const char* mt_item_trig_keys[MT_ITEM_TRIG_COUNT] = {
    /*                         MT_ITEM_NONE */ "None",
    /*                MT_ITEM_USER_ACTIVATE */ "User_Activate",
    /*                         MT_ITEM_WEAR */ "Wear",
    /*                       MT_ITEM_UNWEAR */ "Unwear",
    /*                       MT_ITEM_PICKUP */ "Pickup",
    /*                         MT_ITEM_DROP */ "Drop",
    /*                   MT_ITEM_PARENT_HIT */ "Parent_Hit",
    /*               MT_ITEM_PARENT_STUNNED */ "Parent_Stunned",
    /*     MT_ITEM_PARENT_GOING_UNCONSCIOUS */ "Parent_Going_Unconscious",
    /*                 MT_ITEM_PARENT_DYING */ "Parent_Dying",
    /*      MT_ITEM_PARENT_HIT_BY_ATK_SPELL */ "Parent_Hit_By_Atk_Spell",
    /*         MT_ITEM_PARENT_ATKS_OPPONENT */ "Parent_Atks_Opponent",
    /*         MT_ITEM_PARENT_ATKS_LOCATION */ "Parent_Atks_Location",
    /*         MT_ITEM_PARENT_DMGS_OPPONENT */ "Parent_Dmgs_Opponent",
    /*  MT_ITEM_PARENT_DMGS_OPPONENT_W_ITEM */ "Parent_Dmgs_Opponent_W_Item",
    /*                   MT_ITEM_TARGET_HIT */ "Target_Hit",
    /*     MT_ITEM_TARGET_GOING_UNCONSCIOUS */ "Target_Going_Unconscious",
    /*                    MT_ITEM_ITEM_USED */ "Item_Used",
    /*              MT_ITEM_TARGET_ATTACKER */ "Target_Attacker",
    /*       MT_ITEM_TARGET_ATTACKER_WEAPON */ "Target_Attacker_Weapon",
    /*        MT_ITEM_TARGET_ATTACKER_ARMOR */ "Target_Attacker_Armor",
    /* MT_ITEM_TARGET_ATTACKER_WEAPON_MELEE */ "Target_Attacker_Weapon_Melee",
    /*           MT_ITEM_RANDOM_CHANCE_RARE */ "Random_Chance_Rare",
    /*       MT_ITEM_RANDOM_CHANCE_UNCOMMON */ "Random_Chance_Uncommon",
    /*         MT_ITEM_RANDOM_CHANCE_COMMON */ "Random_Chance_Common",
    /*       MT_ITEM_RANDOM_CHANCE_FREQUENT */ "Random_Chance_Frequent",
};

// 0x5B7314
unsigned int mt_item_trig_values[MT_ITEM_TRIG_COUNT] = {
    MT_ITEM_TRIG_NONE,
    MT_ITEM_TRIG_USER_ACTIVATE,
    MT_ITEM_TRIG_WEAR,
    MT_ITEM_TRIG_UNWEAR,
    MT_ITEM_TRIG_PICKUP,
    MT_ITEM_TRIG_DROP,
    MT_ITEM_TRIG_PARENT_HIT,
    MT_ITEM_TRIG_PARENT_STUNNED,
    MT_ITEM_TRIG_PARENT_GOING_UNCONSCIOUS,
    MT_ITEM_TRIG_PARENT_DYING,
    MT_ITEM_TRIG_PARENT_HIT_BY_ATK_SPELL,
    MT_ITEM_TRIG_PARENT_ATKS_OPPONENT,
    MT_ITEM_TRIG_PARENT_ATKS_LOCATION,
    MT_ITEM_TRIG_PARENT_DMGS_OPPONENT,
    MT_ITEM_TRIG_PARENT_DMGS_OPPONENT_W_ITEM,
    MT_ITEM_TRIG_TARGET_HIT,
    MT_ITEM_TRIG_TARGET_GOING_UNCONSCIOUS,
    MT_ITEM_TRIG_ITEM_USED,
    MT_ITEM_TRIG_TARGET_ATTACKER,
    MT_ITEM_TRIG_TARGET_ATTACKER_WEAPON,
    MT_ITEM_TRIG_TARGET_ATTACKER_ARMOR,
    MT_ITEM_TRIG_TARGET_ATTACKER_WEAPON_MELEE,
    MT_ITEM_TRIG_RANDOM_CHANCE_RARE,
    MT_ITEM_TRIG_RANDOM_CHANCE_UNCOMMON,
    MT_ITEM_TRIG_RANDOM_CHANCE_COMMON,
    MT_ITEM_TRIG_RANDOM_CHANCE_FREQUENT,
};

// 0x5FF610
static bool mt_item_initialized;

// 0x5FF618
static int64_t qword_5FF618;

// 0x4CB720
bool mt_item_init(GameInitInfo* init_info)
{
    (void)init_info;

    mt_item_initialized = true;

    return true;
}

// 0x4CB730
void mt_item_exit()
{
    if (mt_item_initialized) {
        mt_item_initialized = false;
    }
}

// 0x4CB760
unsigned int mt_item_triggers(int index)
{
    return index != 10000 ? magictech_spells[index].item_triggers : 0;
}

// 0x4CB790
int sub_4CB790(int a1)
{
    return a1 & 0xFF;
}

// 0x4CB7A0
int mt_item_spell(int64_t item_obj, int index)
{
    int spl;

    if (item_obj == OBJ_HANDLE_NULL) {
        return -1;
    }

    spl = obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_1 + index);
    if (spl == 10000) {
        return -1;
    }

    return spl;
}

// 0x4CB7D0
void mt_item_notify_wear(int64_t item_obj, int64_t parent_obj)
{
    sub_4CB800(item_obj, parent_obj, OBJ_HANDLE_NULL, MT_ITEM_TRIG_WEAR);
}

// 0x4CB800
void sub_4CB800(int64_t a1, int64_t a2, int64_t a3, unsigned int flags)
{
    sub_4CB830(a1, a2, a3, OBJ_HANDLE_NULL, flags);
}

// 0x4CB830
void sub_4CB830(int64_t a1, int64_t a2, int64_t a3, int64_t a4, unsigned int flags)
{
    int index;
    int spl;
    unsigned int trig;
    MagicTechSerializedData v1;

    if (tig_net_is_active()
        && !tig_net_is_host()) {
        qword_5FF618 = OBJ_HANDLE_NULL;
        return;
    }

    for (index = 0; index < 5; index++) {
        spl = obj_field_int32_get(a1, OBJ_F_ITEM_SPELL_1 + index);
        if (spl == 10000) {
            break;
        }

        trig = mt_item_triggers(spl);
        if ((trig & flags) != 0) {
            if ((trig & MT_ITEM_TRIG_RANDOM_CHANCE_ANY) == 0
                || ((trig & MT_ITEM_TRIG_RANDOM_CHANCE_RARE) != 0
                    && random_between(1, 100) <= 10)
                || ((trig & MT_ITEM_TRIG_RANDOM_CHANCE_UNCOMMON) != 0
                    && random_between(1, 100) <= 20)
                || ((trig & MT_ITEM_TRIG_RANDOM_CHANCE_COMMON) != 0
                    && random_between(1, 100) <= 40)
                || ((trig & MT_ITEM_TRIG_RANDOM_CHANCE_FREQUENT) != 0
                    && random_between(1, 100) <= 66)) {
                sub_455A20(&v1, a1, sub_4CB790(spl));

                if ((trig & MT_ITEM_TRIG_TARGET_ATTACKER_ANY) != 0) {
                    if ((trig & MT_ITEM_TRIG_TARGET_ATTACKER) != 0) {
                        sub_4440E0(a3, &(v1.target_obj));
                    } else if ((trig & MT_ITEM_TRIG_TARGET_ATTACKER_WEAPON) != 0) {
                        if (a3 != OBJ_HANDLE_NULL) {
                            sub_4440E0(item_wield_get(a3, ITEM_INV_LOC_WEAPON), &(v1.target_obj));
                        } else {
                            sub_4440E0(OBJ_HANDLE_NULL, &(v1.target_obj));
                        }
                    } else if ((trig & MT_ITEM_TRIG_TARGET_ATTACKER_ARMOR) != 0) {
                        if (a3 != OBJ_HANDLE_NULL) {
                            sub_4440E0(item_wield_get(a3, ITEM_INV_LOC_ARMOR), &(v1.target_obj));
                        } else {
                            sub_4440E0(OBJ_HANDLE_NULL, &(v1.target_obj));
                        }
                    } else {
                        sub_4440E0(OBJ_HANDLE_NULL, &(v1.target_obj));
                    }
                } else if ((trig & MT_ITEM_TRIG_PARENT_ATKS_LOCATION) != 0) {
                    v1.target_loc = a4;
                } else {
                    sub_4440E0(a2, &(v1.target_obj));
                }

                v1.field_D8 = flags;
                v1.flags |= 0x1;

                if (qword_5FF618 != OBJ_HANDLE_NULL) {
                    sub_4440E0(qword_5FF618, &(v1.field_A0));
                }

                if (v1.target_obj.obj != OBJ_HANDLE_NULL || v1.target_loc != 0) {
                    sub_455AC0(&v1);
                }
            }
        }
    }

    qword_5FF618 = OBJ_HANDLE_NULL;
}

// 0x4CBAA0
void mt_item_notify_pickup(int64_t item_obj, int64_t parent_obj)
{
    // FIXME: Cast.
    sub_4CB800(item_obj, parent_obj, OBJ_HANDLE_NULL, (unsigned int)MT_ITEM_TRIG_PICKUP);
}

// 0x4CBAD0
void mt_item_notify_parent_hit(int64_t attacker_obj, CombatContext* combat, int64_t target_obj)
{
    unsigned int flags;
    int index;
    int64_t item_obj;

    if (target_obj == OBJ_HANDLE_NULL) {
        return;
    }

    flags = MT_ITEM_TRIG_PARENT_HIT;
    if ((combat->flags & 0x40000) != 0
        && (combat->weapon_obj == OBJ_HANDLE_NULL
            || item_weapon_range(combat->weapon_obj, attacker_obj) <= 1)) {
        flags |= MT_ITEM_TRIG_TARGET_ATTACKER_WEAPON_MELEE;
    }

    if (obj_type_is_critter(obj_field_int32_get(target_obj, OBJ_F_TYPE))) {
        for (index = 0; index < 9; index++) {
            item_obj = item_wield_get(target_obj, 1000 + index);
            if (item_obj != OBJ_HANDLE_NULL) {
                sub_4CB800(item_obj, target_obj, attacker_obj, flags);
            }
        }
    }
}

// 0x4CBB80
void mt_item_notify_parent_stunned(int64_t attacker_obj, int64_t target_obj)
{
    int type;
    int index;
    int64_t item_obj;

    if (target_obj == OBJ_HANDLE_NULL) {
        return;
    }

    type = obj_field_int32_get(target_obj, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    for (index = 0; index < 9; index++) {
        item_obj = item_wield_get(target_obj, 1000 + index);
        if (item_obj != OBJ_HANDLE_NULL) {
            sub_4CB800(item_obj, target_obj, attacker_obj, MT_ITEM_TRIG_PARENT_STUNNED);
        }
    }
}

// 0x4CBBF0
void mt_item_notify_parent_going_unconscious(int64_t attacker_obj, int64_t target_obj)
{
    int type;
    int index;
    int64_t item_obj;

    if (target_obj == OBJ_HANDLE_NULL) {
        return;
    }

    type = obj_field_int32_get(target_obj, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    for (index = 0; index < 9; index++) {
        item_obj = item_wield_get(target_obj, 1000 + index);
        if (item_obj != OBJ_HANDLE_NULL) {
            sub_4CB800(item_obj, target_obj, attacker_obj, MT_ITEM_TRIG_PARENT_GOING_UNCONSCIOUS);
        }
    }
}

// 0x4CBC60
void mt_item_notify_parent_dying(int64_t killer_obj, int64_t victim_obj)
{
    int type;
    int index;
    int64_t item_obj;

    type = obj_field_int32_get(victim_obj, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    for (index = 0; index < 9; index++) {
        item_obj = item_wield_get(victim_obj, 1000 + index);
        if (item_obj != OBJ_HANDLE_NULL) {
            sub_4CB800(item_obj, victim_obj, killer_obj, MT_ITEM_TRIG_PARENT_DYING);
        }
    }
}

// 0x4CBD40
void mt_item_notify_parent_attacks_obj(int64_t attacker_obj, int64_t target_obj)
{
    int type;
    int index;
    int64_t item_obj;

    type = obj_field_int32_get(attacker_obj, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    qword_5FF618 = attacker_obj;
    for (index = 0; index < 9; index++) {
        item_obj = item_wield_get(attacker_obj, 1000 + index);
        if (item_obj != OBJ_HANDLE_NULL) {
            sub_4CB800(item_obj, attacker_obj, target_obj, MT_ITEM_TRIG_PARENT_ATKS_OPPONENT);
        }
    }
}

// 0x4CBDB0
void mt_item_notify_parent_attacks_loc(int64_t attacker_obj, int64_t weapon_obj, int64_t target_loc)
{
    if (weapon_obj == OBJ_HANDLE_NULL) {
        return;
    }

    qword_5FF618 = attacker_obj;
    sub_4CB830(weapon_obj, attacker_obj, OBJ_HANDLE_NULL, target_loc, MT_ITEM_TRIG_PARENT_ATKS_LOCATION);
}

// 0x4CBE00
void mt_item_notify_target_going_unconscious(int64_t attacker_obj, int64_t target_obj)
{
    int type;
    int index;
    int64_t item_obj;

    if (attacker_obj == OBJ_HANDLE_NULL) {
        return;
    }

    type = obj_field_int32_get(attacker_obj, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    for (index = 0; index < 9; index++) {
        item_obj = item_wield_get(attacker_obj, 1000 + index);
        if (item_obj != OBJ_HANDLE_NULL) {
            sub_4CB800(item_obj, attacker_obj, target_obj, MT_ITEM_TRIG_TARGET_GOING_UNCONSCIOUS);
        }
    }
}

// 0x4CBE70
void mt_item_notify_parent_dmgs_obj(int64_t obj, int64_t weapon_obj, int64_t target_obj)
{
    int type;
    int index;
    int64_t item_obj;

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (obj_type_is_critter(type)) {
        for (index = 0; index < 9; index++) {
            item_obj = item_wield_get(obj, 1000 + index);
            if (item_obj != OBJ_HANDLE_NULL) {
                sub_4CB800(item_obj, obj, target_obj, MT_ITEM_TRIG_PARENT_DMGS_OPPONENT);
                sub_4CB800(item_obj, obj, target_obj, MT_ITEM_TRIG_TARGET_HIT);
                if (item_obj == weapon_obj) {
                    sub_4CB800(item_obj, obj, target_obj, MT_ITEM_TRIG_PARENT_DMGS_OPPONENT_W_ITEM);
                }
            }
        }
    } else if (obj_type_is_item(type)) {
        sub_4CB800(obj, target_obj, obj, MT_ITEM_TRIG_TARGET_HIT);
    }
}

// 0x4CBF70
void sub_4CBF70(int64_t a1, int64_t a2)
{
    int type;
    int64_t parent_obj;

    if (a1 == OBJ_HANDLE_NULL) {
        return;
    }

    type = obj_field_int32_get(a1, OBJ_F_TYPE);
    if (obj_type_is_item(type)) {
        parent_obj = obj_field_handle_get(a1, OBJ_F_ITEM_PARENT);
        sub_4CB800(a1, parent_obj, a2, 0x1000);
    }
}

// 0x4CBFC0
void mt_item_notify_unwear(int64_t item_obj, int64_t parent_obj)
{
    sub_4CBFF0(item_obj, parent_obj, MT_ITEM_TRIG_UNWEAR);
}

// 0x4CBFF0
void sub_4CBFF0(int64_t a1, int64_t a2, unsigned int flags)
{
    int index;
    int spl;
    MagicTechSerializedData v1;

    if (tig_net_is_active()
        && !tig_net_is_host()) {
        return;
    }

    if (a1 == OBJ_HANDLE_NULL) {
        return;
    }

    if (a2 == OBJ_HANDLE_NULL) {
        return;
    }

    for (index = 0; index < 5; index++) {
        spl = obj_field_int32_get(a1, OBJ_F_ITEM_SPELL_1 + index);
        if (spl == 10000) {
            break;
        }

        if ((mt_item_triggers(spl) & flags) != 0) {
            sub_455A20(&v1, a1, sub_4CB790(spl));
            sub_4440E0(a2, &(v1.target_obj));
            sub_4573D0(&v1);

            sub_455A20(&v1, a1, sub_4CB790(spl));
            sub_4440E0(a2, &(v1.target_obj));
            v1.field_D8 = flags;
            if (v1.target_obj.obj != OBJ_HANDLE_NULL) {
                sub_455AC0(&v1);
            }
        }
    }
}

// 0x4CC130
void mt_item_notify_drop(int64_t item_obj, int64_t parent_obj)
{
    sub_4CBFF0(item_obj, parent_obj, MT_ITEM_TRIG_DROP);
}

// 0x4CC160
bool mt_item_valid_ai_action(int64_t item_obj)
{
    int spl;

    // FIXME: Unused, probably should validate if item_obj is of item type.
    obj_field_int32_get(item_obj, OBJ_F_TYPE);

    if (obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_MANA_STORE) == 0) {
        return false;
    }

    spl = obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_1);
    if (spl == 10000) {
        return true;
    }

    if (magictech_spells[spl].item_triggers != 0) {
        return true;
    }

    if ((obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS) & OIF_VALID_AI_ACTION) == 0) {
        return false;
    }

    // FIXME: Unused.
    obj_field_int32_get(item_obj, OBJ_F_ITEM_AI_ACTION);

    return true;
}
