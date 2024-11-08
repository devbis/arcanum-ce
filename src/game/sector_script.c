#include "game/sector_script.h"

#include "game/sector.h"

// 0x4C0410
bool sector_script_init(GameInitInfo* init_info)
{
    (void)init_info;

    return true;
}

// 0x4C0420
void sector_script_reset()
{
}

// 0x4C0430
void sector_script_exit()
{
}

// 0x4C0440
bool sector_script_get(int64_t sector_id, Script* scr)
{
    Sector* sector;
    bool ret;

    ret = false;
    if (sector_lock(sector_id, &sector)) {
        ret = sub_4F61F0(&(sector->sector_scripts), scr);
        sector_unlock(sector_id);
    }

    return ret;
}

// 0x4C0490
bool sector_script_set(int64_t sector_id, Script* scr)
{
    Sector* sector;
    bool ret;

    ret = false;
    if (sector_lock(sector_id, &sector)) {
        ret = sub_4F61C0(&(sector->sector_scripts), scr);
        sector_unlock(sector_id);
    }

    return ret;
}
