#include "tig/palette.h"

#include "tig/debug.h"

typedef struct TigPaletteJack {
    void* field_0;
    TigPaletteJack* next;
};

static_assert(sizeof(TigPaletteJack) == 0x8, "wrong size");

static void tig_palette_jack_create_();
static void tig_palette_jack_destroy_();

// 0x6301F8
static bool tig_palette_initialized;

// 0x6301FC
static int tig_palette_bpp;

// 0x630200
static size_t dword_630200;

// 0x630204
static TigPaletteJack* off_630204;

// 0x533D50
int tig_palette_init(TigContext* ctx)
{
    if (tig_palette_initialized) {
        return TIG_ALREADY_INITIALIZED;
    }

    switch (ctx->bpp) {
    case 8:
        break;
    case 16:
        dword_630200 = 512;
        break;
    case 24:
    case 32:
        dword_630200 = 1024;
        break;
    default:
        tig_debug_println("Unknown BPP in tig_palette_init()\n");
        return TIG_ERR_12;
    }

    tig_palette_bpp = ctx->bpp;
    tig_palette_initialized = true;

    for (int index = 0; index < 16; index++) {
        tig_palette_jack_create_();
    }

    return 0;
}

// 0x533DD0
void tig_palette_exit()
{
    if (tig_palette_initialized) {
        tig_palette_jack_destroy_();
        tig_palette_initialized = false;
    }
}

// 0x533DF0
void* sub_533DF0()
{
    TigPaletteJack* v1 = off_630204;
    if (v1 == NULL) {
        tig_palette_jack_create_();
        v1 = off_630204;
    }

    off_630204 = v1->next;

    // TODO: Check.
    return v1->field_0 + 4;
}

// 0x533E20
void sub_533E20(void* a1)
{
    // TODO: Check.
    TigPaletteJack* v1 = a1 - 4;
    v1->next = off_630204;
    off_630204 = v1;
}

// 0x533E40
void sub_533E40(void* a1, int a2)
{
    switch (tig_palette_bpp) {
    case 8:
        break;
    case 16:
        for (int index = 0; index < 128; index++) {
            // TODO: Check.
            a1[index] = (a2 & 0xFFFF) & ((a2 & 0xFFFF) << 16);
        }
    case 24:
    case 32:
        for (int index = 0; index < 256; index++) {
            a1[index] = a2;
        }
    }
}

// 0x533E90
void sub_533E90(void* dest, const void* src)
{
    memcpy(dest, src, dword_630200);
}

// 0x534290
size_t sub_534290()
{
    return dword_630200 + 4;
}

// 0x5342A0
void tig_palette_jack_create_()
{
    for (int index = 0; index < 16; index++) {
        TigPaletteJack* v1 = (TigPaletteJack*)malloc(sizeof(*v1));
        v1->field_0 = malloc(dword_630200 + 4);
        v1->next = off_630204;
        off_630204 = v1;
    }
}

// 0x5342E0
void tig_palette_jack_destroy_()
{
    TigPaletteJack* curr = off_630204;
    while (curr != NULL) {
        TigPaletteJack* next = curr->next;
        free(curr->field_0);
        free(curr);
        curr = next;
    }

    off_630204 = NULL;
}
