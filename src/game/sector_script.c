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
bool sub_4C0440(int64_t a1, void* a2)
{
    Sector* sector;
    bool ret;

    ret = false;
    if (sector_lock(a1, &sector)) {
        ret = sub_4F61F0(&(sector->dword_4630), a2);
        sector_unlock(sector);
    }

    return ret;
}

// 0x4C0490
bool sub_4C0490(int64_t v1, void* a2)
{
    Sector* sector;
    bool ret;

    ret = false;
    if (sector_lock(a1, &sector)) {
        ret = sub_4F61C0(&(sector->dword_4630), a2);
        sector_unlock(sector);
    }

    return ret;
}
