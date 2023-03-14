#include "tig/anna.h"

#include "tig/color.h"
#include "tig/window.h"

#define FIFTY 50

#define ANNA_FLAG_0x01 0x01

typedef struct Bella {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int window_handle;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
};

static_assert(sizeof(Bella) == 0x28, "wrong size");

typedef struct Anna {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
};

static_assert(sizeof(Anna) == 0x30, "wrong size");

typedef struct Sarah {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
};

static_assert(sizeof(Sarah) == 0x14, "wrong size");

// 0x636550
static Bella stru_636550;

// 0x636578
static Anna stru_636578[FIFTY];

// 0x636F00
static Sarah stru_636F00;

// 0x538F60
int tig_anna_init(TigContext* ctx)
{
    for (int index = 0; index < FIFTY; index++) {
        stru_636578[index].field_0 = 1;
    }

    Sarah v1;
    v1.field_0 = tig_color_rgb_make(255, 255, 255);
    v1.field_4 = tig_color_rgb_make(64, 64, 64);
    v1.field_8 = tig_color_rgb_make(255, 0, 0);
    v1.field_C = tig_color_rgb_make(128, 128, 128);
    v1.field_10 = tig_color_rgb_make(255, 255, 255);
    sub_5390F0(&v1);
}

// 0x5390C0
void tig_anna_exit()
{
    for (int index = 0; index < FIFTY; index++) {
        if ((stru_636578[index].field_0 & ANNA_FLAG_0x01) == 0) {
            int anna_handle = tig_anna_index_to_handle(index);
            sub_539A40(anna_handle);
        }
    }
}

// 0x5390F0
int sub_5390F0(Sarah* a1)
{
    stru_636F00 = *a1;
    return TIG_OK;
}

// 0x539A40
int sub_539A40(int anna_handle)
{
    int anna_index = tig_anna_handle_to_index(anna_handle);
    free(stru_636578[anna_index].field_2C);
    stru_636578[anna_index].field_0 |= ANNA_FLAG_0x01;
    return TIG_OK;
}

// 0x539EC0
int tig_anna_handle_to_index(int handle)
{
    return handle;
}

// 0x539ED0
int tig_anna_index_to_handle(int index)
{
    return index;
}

// 0x53A020
void sub_53A020(Bella* a1)
{
    tig_window_destroy(a1->window_handle);
}
