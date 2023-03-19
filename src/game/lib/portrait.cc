#include "game/lib/portrait.h"

#include <stdio.h>

#include "game/lib/message.h"
#include "game/lib/stat.h"

// 0x601744
static int portrait_user_msg_file;

// 0x601748
static bool portrait_user_msg_file_loaded;

// 0x60174C
static int portrait_msg_file;

// 0x4CE350
bool portait_init(GameContext* ctx)
{
    if (!message_load("portrait\\gameport.mes", &portrait_msg_file)) {
        return false;
    }

    portrait_user_msg_file_loaded = message_load("portrait\\userport.mes", &portrait_user_msg_file);

    return true;
}

// 0x4CE390
void portrait_exit()
{
    message_unload(portrait_msg_file);

    if (portrait_user_msg_file_loaded) {
        message_unload(portrait_user_msg_file);
    }
}

// 0x4CEAC0
bool sub_4CEAC0(object_id_t object_id, char* buffer)
{
    // 0x5B7C0C
    static const char* race_specifiers[RACE_COUNT] = {
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
    static char gender_specifiers[GENDER_COUNT] = {
        /* FEMALE */ 'F',
        /*   MALE */ 'M',
    };

    if (buffer != NULL) {
        int race = sub_4B0740(object_id, STAT_RACE);
        int gender = sub_4B0740(object_id, STAT_GENDER);
        sprintf(buffer, "%s%c", race_specifiers[race], gender_specifiers[gender]);

        // FIXME: What for?
        if (strnicmp(buffer, buffer, 3) == 0) {
            return true;
        }

        // FIXME: Always false with current implementation.
        if (strnicmp("ANY", buffer, 3) == 0) {
            return true;
        }

        if (object_field_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_16) {
            // FIXME: Always false with current implementation.
            if (strnicmp("NPC", buffer, 3) == 0) {
                return true;
            }
        }
    }

    return false;
}
