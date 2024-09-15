#include "game/portrait.h"

#include <stdio.h>

#include "game/mes.h"
#include "game/stat.h"

static bool sub_4CEAC0(object_id_t object_id, const char* str);

// 0x5B7C0C
static const char* portrait_race_specifiers[RACE_COUNT] = {
    /*     HUMAN */ "HU",
    /*     DWARF */ "DW",
    /*       ELF */ "EL",
    /*  HALF_ELF */ "HE",
    /*     GNOME */ "GN",
    /*  HALFLING */ "HA",
    /*  HALF_ORC */ "HO",
    /* HALF_OGRE */ "HG",
    /*  DARK_ELF */ "EL",
    /*      OGRE */ "HG",
    /*       ORC */ "HO",
};

// 0x5B7C38
static char portrait_gender_specifiers[GENDER_COUNT] = {
    /* FEMALE */ 'F',
    /*   MALE */ 'M',
};

// 0x601744
static int portrait_user_msg_file;

// 0x601748
static bool portrait_user_msg_file_loaded;

// 0x60174C
static int portrait_msg_file;

// 0x4CE350
bool portait_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (!mes_load("portrait\\gameport.mes", &portrait_msg_file)) {
        return false;
    }

    portrait_user_msg_file_loaded = mes_load("portrait\\userport.mes", &portrait_user_msg_file);

    return true;
}

// 0x4CE390
void portrait_exit()
{
    mes_unload(portrait_msg_file);

    if (portrait_user_msg_file_loaded) {
        mes_unload(portrait_user_msg_file);
    }
}

// 0x4CEAC0
bool sub_4CEAC0(int64_t obj, const char* str)
{
    int race;
    int gender;
    char buffer[4];

    if (str != NULL) {
        race = stat_get_base(obj, STAT_RACE);
        gender = stat_get_base(obj, STAT_GENDER);
        sprintf(buffer, "%s%c",
            portrait_race_specifiers[race],
            portrait_gender_specifiers[gender]);

        if (strnicmp(buffer, str, 3) == 0) {
            return true;
        }

        if (strnicmp("ANY", str, 3) == 0) {
            return true;
        }

        if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC
            && strnicmp("NPC", str, 3) == 0) {
            return true;
        }
    }

    return false;
}
