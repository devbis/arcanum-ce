#include "game/random.h"

#include <time.h>

// 0x601778
static int random_prev_value;

static void random_set_prev_value(int value);

// 0x4CEEB0
bool random_init(GameInitInfo* init_info)
{
    (void)init_info;

    random_seed_generate();

    return true;
}

// 0x4CEEC0
void random_exit()
{
}

// 0x4CEED0
void random_seed(int value)
{
    random_set_prev_value(value);
}

// 0x4CEEE0
int random_seed_generate()
{
    int seed = time(NULL);

    random_seed(seed);

    return seed;
}

// 0x4CEF00
int random_between(int lower, int upper)
{
    if (lower == upper) {
        return lower;
    } else if (lower < upper) {
        return lower + random_rand() % (upper - lower + 1);
    } else {
        return upper + random_rand() % (lower - upper + 1);
    }
}

// 0x4CEF40
void random_set_prev_value(int value)
{
    random_prev_value = value;
}

// 0x4CEF50
int random_rand()
{
    random_prev_value = 1664525 * random_prev_value + 1013904223;
    return (random_prev_value >> 8) & 0x7FFF;
}
