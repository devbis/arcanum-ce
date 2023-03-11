#include "tig/color.h"

typedef enum ColorComponent {
    CC_RED,
    CC_GREEN,
    CC_BLUE,
    CC_COUNT,
};

typedef enum ColorSettings {
    CS_CURRENT,
    CS_8,
    CS_16,
    CS_24,
    CS_32,
    CS_COUNT,
};

static void tig_color_set_mask(int color_component, unsigned int mask);
static int sub_52C760(int color, int color_component, int color_setting);
static void tig_color_table_1_init();
static void tig_color_table_1_exit();
static void tig_color_table_2_init();
static void tig_color_table_2_exit();
static void tig_color_table_3_init();
static void tig_color_table_3_exit();
static void tig_color_table_4_init();
static void tig_color_table_4_exit();

// 0x5C215C
static int tig_color_masks_table[CS_COUNT][CC_COUNT] = {
    { 0, 0, 0 },
    { 0xE0, 0x1C, 0x03 },
    { 0xF800, 0x7E0, 0x1F },
    { 0xFF0000, 0xFF00, 0xFF },
    { 0xFF0000, 0xFF00, 0xFF },
};

// 0x5C2198
static int tig_color_shifts_table[CS_COUNT][CC_COUNT] = {
    { 0, 0, 0 },
    { 5, 2, 0 },
    { 11, 5, 0 },
    { 16, 8, 0 },
    { 16, 8, 0 },
};

// 0x5C21D4
static int dword_5C21D4[CS_COUNT][CC_COUNT] = {
    { 0, 0, 0 },
    { 7, 7, 3 },
    { 0x1F, 0x3F, 0x1F },
    { 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF },
};

// 0x62B29C
static int tig_color_bpp;

// 0x62B2A0
static bool tig_color_initialized;

// 0x739E88
uint8_t* dword_739E88;

// 0x739E8C
unsigned int dword_739E8C;

// 0x739E90
unsigned int tig_color_blue_mask;

// 0x739E94
unsigned int dword_739E94;

// 0x739E98
uint8_t* dword_739E98;

// 0x739E9C
unsigned int dword_739E9C;

// 0x739EA0
uint8_t* dword_739EA0;

// 0x739EA4
uint8_t* dword_739EA4;

// 0x739EA8
uint8_t* dword_739EA8;

// 0x739EAC
unsigned int tig_color_blue_shift;

// 0x739EB0
unsigned int dword_739EB0;

// 0x739EB4
unsigned int dword_739EB4;

// 0x739EB8
unsigned int tig_color_green_mask;

// 0x739EBC
uint8_t* dword_739EBC;

// 0x739EC0
uint8_t* dword_739EC0;

// 0x739EC4
unsigned int dword_739EC4;

// 0x739EC8
unsigned int dword_739EC8;

// 0x739ECC
uint8_t* dword_739ECC;

// 0x739ED0
unsigned int dword_739ED0;

// 0x739ED4
unsigned int dword_739ED4;

// 0x739ED8
uint8_t* dword_739ED8;

// 0x739EDC
uint8_t* dword_739EDC;

// 0x739EE0
unsigned int dword_739EE0;

// 0x739EE4
unsigned int dword_739EE4;

// 0x739EE8
unsigned int dword_739EE8;

// 0x739EEC
unsigned int tig_color_red_shift;

// 0x739EF0
unsigned int tig_color_green_shift;

// 0x739EF4
unsigned int dword_739EF4;

// 0x739EF8
unsigned int dword_739EF8;

// 0x739EFC
unsigned int dword_739EFC;

// 0x739F00
unsigned int tig_color_red_mask;

// 0x739F04
unsigned int dword_739F04;

// 0x739F08
unsigned int dword_739F08;

// 0x739F0C
unsigned int dword_739F0C;

// 0x739F10
unsigned int dword_739F10;

// 0x739F14
unsigned int dword_739F14;

// 0x739F18
uint8_t* dword_739F18;

// 0x739F1C
unsigned int dword_739F1C;

// 0x739F20
uint8_t* dword_739F20;

// 0x739F24
unsigned int dword_739F24;

// 0x739F28
uint8_t* dword_739F28;

// 0x739F2C
uint8_t* dword_739F2C;

// 0x739F30
uint8_t* dword_739F30;

// 0x52BFC0
int tig_color_init(TigContext* ctx)
{
    if (tig_color_initialized) {
        return TIG_ALREADY_INITIALIZED;
    }

    tig_color_bpp = ctx->bpp;

    for (int cc = 0; cc < CC_COUNT; cc++) {
        tig_color_masks_table[CS_CURRENT][cc] = tig_color_masks_table[tig_color_bpp / 8][cc];
        tig_color_shifts_table[CS_CURRENT][cc] = tig_color_shifts_table[tig_color_bpp / 8][cc];
        dword_5C21D4[CS_CURRENT][cc] = dword_5C21D4[tig_color_bpp / 8][cc];
    }

    tig_color_initialized = false;

    return TIG_OK;
}

// 0x52C040
void tig_color_exit()
{
    if (tig_color_initialized) {
        tig_color_table_4_exit();
        tig_color_table_3_exit();
        tig_color_table_2_exit();
        tig_color_table_1_exit();
        tig_color_initialized = false;
    }
}

// 0x52C070
int tig_color_set_rgb_settings(unsigned int red_mask, unsigned int green_mask, unsigned int blue_mask)
{
    unsigned int bits;

    tig_color_set_mask(CC_RED, red_mask);
    tig_color_set_mask(CC_GREEN, green_mask);
    tig_color_set_mask(CC_BLUE, blue_mask);

    tig_color_red_mask = tig_color_masks_table[CS_CURRENT][CC_RED];
    tig_color_green_mask = tig_color_masks_table[CS_CURRENT][CC_GREEN];
    tig_color_blue_mask = tig_color_masks_table[CS_CURRENT][CC_BLUE];

    tig_color_red_shift = tig_color_shifts_table[CS_CURRENT][CC_RED];
    tig_color_green_shift = tig_color_shifts_table[CS_CURRENT][CC_GREEN];
    tig_color_blue_shift = tig_color_shifts_table[CS_CURRENT][CC_BLUE];

    dword_739F0C = dword_5C21D4[CS_CURRENT][CC_RED];
    dword_739ED0 = dword_5C21D4[CS_CURRENT][CC_GREEN];
    dword_739E94 = dword_5C21D4[CS_CURRENT][CC_BLUE];

    dword_739EB0 = 0;
    bits = dword_739F0C;
    while (bits != 0) {
        dword_739EB0++;
        bits >>= 1;
    }

    dword_739F14 = 0;
    bits = dword_739ED0;
    while (bits != 0) {
        dword_739F14++;
        bits >>= 1;
    }

    dword_739EF4 = 0;
    bits = dword_739E94;
    while (bits != 0) {
        dword_739EF4++;
        bits >>= 1;
    }

    tig_color_table_1_init();
    tig_color_table_2_init();
    tig_color_table_3_init();

    return TIG_OK;
}

// TODO: Check entire function.
//
// 0x52C160
int sub_52C160(unsigned int red_mask, unsigned int green_mask, unsigned int blue_mask, unsigned int alpha_mask)
{
    unsigned int mask;
    int bit;

    // RED

    dword_739F08 = red_mask;
    mask = red_mask;
    for (bit = 0; bit < 32; bit++) {
        if ((mask & 1) != 0) {
            break;
        }
        mask >>= 1;
    }
    dword_739EC4 = bit;

    for (bit = 0; bit < 32; bit++) {
        if ((mask & 1) == 0) {
            break;
        }
        mask >>= 1;
    }
    dword_739EFC = (1 << bit) - 1;

    dword_739ED4 = 0;
    mask = dword_739EFC;
    while (mask != 0) {
        dword_739ED4++;
        mask >>= 1;
    }

    // GREEN

    dword_739EF8 = green_mask;
    mask = green_mask;
    for (bit = 0; bit < 32; bit++) {
        if ((mask & 1) != 0) {
            break;
        }
        mask >>= 1;
    }
    dword_739EB4 = mask;

    for (bit = 0; bit < 32; bit++) {
        if ((mask & 1) == 0) {
            break;
        }
        mask >>= 1;
    }
    dword_739F1C = (1 << bit) - 1;

    dword_739E8C = 0;
    mask = dword_739F1C;
    while (mask != 0) {
        dword_739E8C++;
        mask >>= 1;
    }

    // BLUE

    dword_739EC8 = blue_mask;
    mask = blue_mask;
    for (bit = 0; bit < 32; bit++) {
        if ((mask & 1) != 0) {
            break;
        }
        mask >>= 1;
    }
    dword_739F04 = mask;

    for (bit = 0; bit < 32; bit++) {
        if ((mask & 1) == 0) {
            break;
        }
        mask >>= 1;
    }
    dword_739EE4 = (1 << bit) - 1;

    dword_739F24 = 0;
    mask = dword_739EE4;
    while (mask != 0) {
        dword_739F24++;
        mask >>= 1;
    }

    // ALPHA

    dword_739EE0 = alpha_mask;
    mask = alpha_mask;
    for (bit = 0; bit < 32; bit++) {
        if ((mask & 1) != 0) {
            break;
        }
        mask >>= 1;
    }
    dword_739EE8 = mask;

    for (bit = 0; bit < 32; bit++) {
        if ((mask & 1) == 0) {
            break;
        }
        mask >>= 1;
    }
    dword_739E9C = (1 << bit) - 1;

    dword_739F10 = 0;
    mask = dword_739E9C;
    while (mask != 0) {
        dword_739F10++;
        mask >>= 1;
    }

    tig_color_table_4_init();

    return TIG_OK;
}

// 0x52C2B0
void tig_color_get_masks(unsigned int* red_mask, unsigned int* green_mask, unsigned int* blue_mask)
{
    *red_mask = tig_color_masks_table[CS_CURRENT][CC_RED];
    *green_mask = tig_color_masks_table[CS_CURRENT][CC_GREEN];
    *blue_mask = tig_color_masks_table[CS_CURRENT][CC_BLUE];
}

// 0x52C2E0
void tig_color_get_shifts(unsigned int* red_shift, unsigned int* green_shift, unsigned int* blue_shift)
{
    *red_shift = tig_color_shifts_table[CS_CURRENT][CC_RED];
    *green_shift = tig_color_shifts_table[CS_CURRENT][CC_GREEN];
    *blue_shift = tig_color_shifts_table[CS_CURRENT][CC_BLUE];
}

// 0x52C310
int tig_color_get_red(int a1)
{
    return sub_52C760(a1, CC_RED, CS_CURRENT);
}

// 0x52C330
int tig_color_get_green(int a1)
{
    return sub_52C760(a1, CC_GREEN, CS_CURRENT);
}

// 0x52C350
int tig_color_get_blue(int a1)
{
    return sub_52C760(a1, CC_BLUE, CS_CURRENT);
}

// 0x52C370
int sub_52C370(int a1)
{
    int v1 = (a1 & tig_color_red_mask) >> tig_color_red_shift;
    int v2 = (a1 & tig_color_green_mask) >> tig_color_green_shift;
    int v3 = (a1 & tig_color_blue_mask) >> tig_color_blue_shift;
    int index = dword_739F20[v1] + dword_739F30[v2] + dword_739F18[v3];
    return (dword_739F2C[index] << tig_color_red_shift) | (dword_739F28[index] << tig_color_green_shift) | (dword_739E98[v1] << tig_color_blue_shift);
}

// 0x52C500
int sub_52C500(int a1)
{
    return sub_52C760(a1, CC_RED, CS_24);
}

// 0x52C520
int sub_52C520(int a1)
{
    return sub_52C760(a1, CC_GREEN, CS_24);
}

// 0x52C540
int sub_52C540(int a1)
{
    return sub_52C760(a1, CC_BLUE, CS_24);
}

// 0x52C560
int sub_52C560(unsigned int a1)
{
    // TODO: Check.
    unsigned int v1 = 255 * ((a1 & tig_color_masks_table[CS_24][CC_RED]) >> tig_color_shifts_table[CS_24][CC_RED]) / dword_5C21D4[CS_24][CC_RED];
    unsigned int v2 = 255 * ((a1 & tig_color_masks_table[CS_24][CC_GREEN]) >> tig_color_shifts_table[CS_24][CC_GREEN]) / dword_5C21D4[CS_24][CC_GREEN];
    unsigned int v3 = 255 * ((a1 & tig_color_masks_table[CS_24][CC_BLUE]) >> tig_color_shifts_table[CS_24][CC_BLUE]) / dword_5C21D4[CS_24][CC_BLUE];
    return (dword_739F2C[v1] << tig_color_red_shift) | (dword_739F28[v2] << tig_color_green_shift) | (dword_739E98[v3] << tig_color_blue_shift);
}

// 0x52C610
int sub_52C610(int red, int green, int blue)
{
    if (red > 255) {
        red = 255;
    } else if (red < 0) {
        red = 0;
    }

    unsigned int v1 = (dword_5C21D4[CS_24][CC_RED] * red / 255) << tig_color_shifts_table[CS_24][CC_RED];

    if (green > 255) {
        green = 255;
    } else if (green < 0) {
        green = 0;
    }

    unsigned int v2 = (dword_5C21D4[CS_24][CC_GREEN] * green / 255) << tig_color_shifts_table[CS_24][CC_RED];

    if (blue > 255) {
        blue = 255;
    } else if (blue < 0) {
        blue = 0;
    }

    unsigned int v3 = (dword_5C21D4[CS_24][CC_BLUE] * blue / 255) << tig_color_shifts_table[CS_24][CC_BLUE];

    return v1 + v2 + v3;
}

// 0x52C6E0
void tig_color_set_mask(int color_component, unsigned int mask)
{
    if (tig_color_bpp == 8) {
        tig_color_masks_table[CS_CURRENT][color_component] = tig_color_masks_table[CS_8][color_component];
        tig_color_shifts_table[CS_CURRENT][color_component] = tig_color_shifts_table[CS_8][color_component];
        dword_5C21D4[CS_CURRENT][color_component] = dword_5C21D4[CS_8][color_component];
    } else {
        tig_color_masks_table[0][color_component] = mask;

        int bit;
        for (bit = 0; bit < 32; bit++) {
            if ((mask & 1) != 0) {
                break;
            }
            mask >>= 1;
        }

        tig_color_shifts_table[0][color_component] = bit;

        for (bit = 0; bit < 32; bit++) {
            if ((mask & 1) == 0) {
                break;
            }
            mask >>= 1;
        }

        dword_5C21D4[0][color_component] = (1 << bit) - 1;
    }
}

// 0x52C760
int sub_52C760(int color, int color_component, int color_setting)
{
    int value = 255 * ((color & tig_color_masks_table[color_setting][color_component]) >> tig_color_shifts_table[color_setting][color_component]) / dword_5C21D4[color_setting][color_component];

    if (tig_color_bpp == 8 && value > 0) {
        value += (255 - value) / 8;
    }

    return value;
}

// 0x52C7C0
void tig_color_table_1_init()
{
    tig_color_table_1_exit();

    dword_739ED8 = (uint8_t*)malloc((dword_739F0C + 1) * (dword_739F0C + 1));
    for (unsigned int v1 = 0; v1 <= dword_739F0C; v1++) {
        for (unsigned int v2 = 0; v2 <= dword_739F0C; v2++) {
            // NOTE: Original code is slightly different.
            unsigned int index = v2 * (dword_739F0C + 1) + v1;
            dword_739ED8[index] = v1 * v2 / dword_739F0C;
        }
    }

    if (tig_color_green_mask >> tig_color_green_shift == tig_color_red_mask >> tig_color_red_shift) {
        dword_739E88 = dword_739ED8;
    } else {
        dword_739E88 = (uint8_t*)malloc((dword_739E94 + 1) * (dword_739E94 + 1));
        for (unsigned int v1 = 0; v1 <= dword_739E94; v1++) {
            for (unsigned int v2 = 0; v2 <= dword_739E94; v2++) {
                // NOTE: Original code is slightly different.
                unsigned int index = v2 * (dword_739E94 + 1) + v1;
                dword_739E88[index] = v1 * v2 / dword_739E94;
            }
        }
    }

    if (tig_color_blue_mask >> tig_color_blue_shift == tig_color_red_mask >> tig_color_red_shift) {
        dword_739EA4 = dword_739ED8;
    } else if (tig_color_blue_mask >> tig_color_blue_shift == tig_color_green_mask >> tig_color_green_shift) {
        dword_739EA4 = dword_739E88;
    } else {
        dword_739EA4 = (uint8_t*)malloc((dword_739ED0 + 1) * (dword_739ED0 + 1));
        for (unsigned int v1 = 0; v1 <= dword_739ED0; v1++) {
            for (unsigned int v2 = 0; v2 <= dword_739ED0; v2++) {
                // NOTE: Original code is slightly different.
                unsigned int index = v2 * (dword_739ED0 + 1) + v1;
                dword_739EA4[index] = v1 * v2 / dword_739ED0;
            }
        }
    }
}

// 0x52C940
void tig_color_table_1_exit()
{
    uint8_t* prev = dword_739E88;
    if (dword_739EA4 != NULL) {
        if (dword_739EA4 != dword_739E88 && dword_739EA4 != dword_739ED8) {
            free(dword_739EA4);
            prev = dword_739E88;
        }
        dword_739EA4 = NULL;
    }

    if (prev != NULL) {
        if (prev != dword_739ED8) {
            free(prev);
        }
        dword_739E88 = NULL;
    }

    if (dword_739ED8 != NULL) {
        free(dword_739ED8);
        dword_739ED8 = NULL;
    }
}

// 0x52C9B0
void tig_color_table_2_init()
{
    // NOTE: Missing call to exit routine which is seen in other initializers.
    // Probably a bug.

    dword_739F20 = (uint8_t*)malloc(dword_739F0C + 1);
    for (unsigned int index = 0; index <= dword_739F0C; index++) {
        dword_739F20[index] = (double)index / (double)dword_739F0C * 76.5;
    }

    dword_739F30 = (uint8_t*)malloc(dword_739E94 + 1);
    for (unsigned int index = 0; index <= dword_739E94; index++) {
        dword_739F30[index] = (double)index / (double)dword_739E94 * 150.45;
    }

    dword_739F18 = (uint8_t*)malloc(dword_739ED0 + 1);
    for (unsigned int index = 0; index <= dword_739ED0; index++) {
        dword_739F18[index] = (double)index / (double)dword_739ED0 * 28.05;
    }
}

// 0x52CAC0
void tig_color_table_2_exit()
{
    if (dword_739F18 != NULL) {
        free(dword_739F18);
        dword_739F18 = 0;
    }

    if (dword_739F30 != NULL) {
        free(dword_739F30);
        dword_739F30 = 0;
    }

    if (dword_739F20 != NULL) {
        free(dword_739F20);
        dword_739F20 = 0;
    }
}

// 0x52CB20
void tig_color_table_3_init()
{
    tig_color_table_3_exit();

    dword_739F2C = (uint8_t*)malloc(256);
    for (unsigned int index = 0; index < 256; index++) {
        dword_739F2C[index] = index >> (8 - dword_739EB0);
    }

    if (tig_color_green_mask >> tig_color_green_shift == tig_color_red_mask >> tig_color_red_shift) {
        dword_739F28 = dword_739F2C;
    } else {
        dword_739F28 = (uint8_t*)malloc(256);
        for (unsigned int index = 0; index < 256; index++) {
            dword_739F28[index] = index >> (8 - dword_739F14);
        }
    }

    if (tig_color_blue_mask >> tig_color_blue_shift == tig_color_red_mask >> tig_color_red_shift) {
        dword_739E98 = dword_739F2C;
    } else if (tig_color_blue_mask >> tig_color_blue_shift == tig_color_green_mask >> tig_color_green_shift) {
        dword_739E98 = dword_739F28;
    } else {
        dword_739E98 = (uint8_t*)malloc(256);
        for (unsigned int index = 0; index < 256; index++) {
            dword_739E98[index] = index >> (8 - dword_739EF4);
        }
    }

    dword_739EA0 = (uint8_t*)malloc(dword_739F0C + 1);
    for (unsigned int index = 0; index <= dword_739F0C; index++) {
        dword_739EA0[index] = (double)index / (double)dword_739F0C * 255.0;
    }

    if (tig_color_green_mask >> tig_color_green_shift == tig_color_red_mask >> tig_color_red_shift) {
        dword_739EC0 = dword_739EA0;
    } else {
        dword_739EC0 = (uint8_t*)malloc(dword_739E94 + 1);
        for (unsigned int index = 0; index <= dword_739E94; index++) {
            dword_739EC0[index] = (double)index / (double)dword_739E94 * 255.0;
        }
    }

    if (tig_color_blue_mask >> tig_color_blue_shift == tig_color_red_mask >> tig_color_red_shift) {
        dword_739EBC = dword_739EA0;
    } else if (tig_color_blue_mask >> tig_color_blue_shift == tig_color_green_mask >> tig_color_green_shift) {
        dword_739EBC = dword_739EC0;
    } else {
        dword_739EBC = (uint8_t*)malloc(dword_739ED0 + 1);
        for (unsigned int index = 0; index <= dword_739ED0; index++) {
            dword_739EBC[index] = (double)index / (double)dword_739ED0 * 255.0;
        }
    }
}

// 0x52CDE0
void tig_color_table_3_exit()
{
    uint8_t* prev;

    prev = dword_739EC0;
    if (dword_739EBC != NULL) {
        if (dword_739EBC != dword_739EC0 && dword_739EBC != dword_739EA0) {
            free(dword_739EBC);
            prev = dword_739EC0;
        }
        dword_739EBC = NULL;
    }

    if (prev != NULL) {
        if (prev != dword_739EA0) {
            free(prev);
        }
        dword_739EC0 = NULL;
    }

    if (dword_739EA0 != NULL) {
        free(dword_739EA0);
        dword_739EA0 = NULL;
    }

    prev = dword_739F28;
    if (dword_739E98 != NULL) {
        if (dword_739E98 != dword_739F28 && dword_739E98 != dword_739F2C) {
            free(dword_739E98);
            prev = dword_739F28;
        }
        dword_739E98 = NULL;
    }

    if (prev != NULL) {
        if (prev != dword_739F2C) {
            free(prev);
        }
        dword_739F28 = NULL;
    }

    if (dword_739F2C) {
        free(dword_739F2C);
        dword_739F2C = NULL;
    }
}

// 0x52CEB0
void tig_color_table_4_init()
{
    tig_color_table_4_exit();

    dword_739EA8 = (uint8_t*)malloc(dword_739F0C + 1);
    for (unsigned int index = 0; index <= dword_739F0C; index++) {
        dword_739EA8[index] = (double)index / (double)dword_739F0C * (double)dword_739EFC;
    }

    if (tig_color_green_mask >> tig_color_green_shift == tig_color_red_mask >> tig_color_red_shift) {
        dword_739EDC = dword_739EA8;
    } else {
        dword_739EDC = (uint8_t*)malloc(dword_739E94 + 1);
        for (unsigned int index = 0; index <= dword_739E94; index++) {
            dword_739EDC[index] = (double)index / (double)dword_739E94 * (double)dword_739F1C;
        }
    }

    if (tig_color_blue_mask >> tig_color_blue_shift == tig_color_red_mask >> tig_color_red_shift) {
        dword_739ECC = dword_739EA8;
    } else if (tig_color_blue_mask >> tig_color_blue_shift == tig_color_green_mask >> tig_color_green_shift) {
        dword_739ECC = dword_739EDC;
    } else {
        dword_739ECC = (uint8_t*)malloc(dword_739ED0 + 1);
        for (unsigned int index = 0; index <= dword_739ED0; index++) {
            dword_739ECC[index] = (double)index / (double)dword_739ED0 * (double)dword_739EE4;
        }
    }
}

// 0x52D070
void tig_color_table_4_exit()
{
    uint8_t* prev = dword_739EDC;
    if (dword_739EDC != NULL) {
        if (dword_739EDC != dword_739EDC && dword_739ECC != dword_739EA8) {
            free(dword_739ECC);
            prev = dword_739EDC;
        }
        dword_739ECC = NULL;
    }

    if (prev != NULL) {
        if (prev != dword_739EA8) {
            free(prev);
        }
        dword_739EDC = NULL;
    }

    if (dword_739EA8 != NULL) {
        free(dword_739EA8);
        dword_739EA8 = 0;
    }
}
