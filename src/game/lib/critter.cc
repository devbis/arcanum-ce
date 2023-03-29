#include "game/lib/critter.h"

#include "game/lib/effect.h"
#include "game/lib/stat.h"
#include "tig/debug.h"

// 0x45FC70
void critter_debug_obj(object_id_t obj)
{
    char name[1000];

    if (obj != OBJ_HANDLE_NULL) {
        if (obj_type_is_critter(object_field_get(obj, OBJ_F_TYPE))) {
            tig_debug_println("\n\n--------------------------------------");
            tig_debug_println("Critter Debug Obj:\n\n");
            sub_441B60(obj, obj, name);
            tig_debug_printf("Name: %s\n", name);
            tig_debug_println("Stats:  Level (Base)");

            for (int stat = 0; stat < STAT_COUNT; stat++) {
                int level = stat_level(obj, stat);
                int base = stat_get_base(obj, stat);
                tig_debug_printf("\t[%s]:\t%d (%d)\n",
                    stat_get_name(stat),
                    level,
                    base);
            }

            effect_debug_obj(obj);
        }
    }
}
