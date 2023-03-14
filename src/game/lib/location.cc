#include "game/lib/location.h"

// 0x5FC2F8
static LocationFunc5FC2F8* dword_5FC2F8;

// 0x4B8D40
void location_set_func_5FC2F8(LocationFunc5FC2F8* func)
{
    dword_5FC2F8 = func;
}
