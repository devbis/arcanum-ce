#include "tig/palette.h"

#include "tig/color.h"
#include "tig/debug.h"
#include "tig/memory.h"

typedef struct TigPaletteListNode {
    void* data;
    struct TigPaletteListNode* next;
} TigPaletteListNode;

static_assert(sizeof(TigPaletteListNode) == 0x8, "wrong size");

static void tig_palette_node_reserve();
static void tig_palette_node_clear();

// 0x6301F8
static bool tig_palette_initialized;

// 0x6301FC
static int tig_palette_bpp;

// 0x630200
static size_t tig_palette_size;

// 0x630204
static TigPaletteListNode* tig_palette_head;

// 0x533D50
int tig_palette_init(TigInitializeInfo* init_info)
{
    int index;

    if (tig_palette_initialized) {
        return TIG_ALREADY_INITIALIZED;
    }

    switch (init_info->bpp) {
    case 8:
        break;
    case 16:
        tig_palette_size = sizeof(uint16_t) * 256;
        break;
    case 24:
    case 32:
        tig_palette_size = sizeof(uint32_t) * 256;
        break;
    default:
        tig_debug_println("Unknown BPP in tig_palette_init()\n");
        return TIG_ERR_12;
    }

    tig_palette_bpp = init_info->bpp;
    tig_palette_initialized = true;

    // Warm palette cache.
    for (index = 0; index < 16; index++) {
        tig_palette_node_reserve();
    }

    return 0;
}

// 0x533DD0
void tig_palette_exit()
{
    if (tig_palette_initialized) {
        tig_palette_node_clear();
        tig_palette_initialized = false;
    }
}

// 0x533DF0
TigPalette tig_palette_create()
{
    TigPaletteListNode* node;

    node = tig_palette_head;
    if (node == NULL) {
        tig_palette_node_reserve();
        node = tig_palette_head;
    }

    tig_palette_head = node->next;

    // Return "entries" area of the palette.
    return (TigPalette)((unsigned char*)node->data + sizeof(TigPaletteListNode*));
}

// 0x533E20
void tig_palette_destroy(TigPalette palette)
{
    TigPaletteListNode* node;

    node = (TigPaletteListNode*)((unsigned char*)palette - sizeof(TigPaletteListNode*));
    node->next = tig_palette_head;
    tig_palette_head = node;
}

// 0x533E40
void tig_palette_set_color(TigPalette palette, unsigned int color)
{
    int index;

    if (tig_palette_bpp != 8) {
        switch (tig_palette_bpp) {
        case 16:
            for (index = 0; index < 256; index++) {
                ((uint16_t*)desc->dst_palette)[index] = (uint16_t)color;
            }
            break;
        case 24:
        case 32:
            for (index = 0; index < 256; index++) {
                ((uint32_t*)desc->dst_palette)[index] = color;
            }
            break;
        }
    }
}

// 0x533E90
void tig_palette_copy(TigPalette dst, const TigPalette src)
{
    memcpy(dst, src, tig_palette_size);
}

// 0x533EC0
void tig_palette_adjust(TigPaletteAdjustDesc* desc)
{
    int index;

    if (desc->flags == 0) {
        return;
    }

    switch (tig_palette_bpp) {
    case 16:
        if (desc->dst_palette != desc->src_palette) {
            memcpy(desc->dst_palette, desc->src_palette, sizeof(uint16_t) * 256);
        }

        if ((desc->flags & TIG_PALETTE_ADJUST_GRAYSCALE) != 0) {
            for (index = 0; index < 256; index++) {
                ((uint16_t*)desc->dst_palette)[index] = (uint16_t)tig_color_rgb_to_grayscale(((uint16_t*)desc->dst_palette)[index]);
            }
        }

        if ((desc->flags & TIG_PALETTE_ADJUST_TINT) != 0) {
            for (index = 0; index < 256; index++) {
                ((uint16_t*)desc->dst_palette)[index] = (uint16_t)tig_color_mult(((uint16_t*)desc->dst_palette)[index], desc->tint_color);
            }
        }
        break;
    case 24:
        if (desc->dst_palette != desc->src_palette) {
            memcpy(desc->dst_palette, desc->src_palette, sizeof(uint32_t) * 256);
        }

        if ((desc->flags & TIG_PALETTE_ADJUST_GRAYSCALE) != 0) {
            for (index = 0; index < 256; index++) {
                ((uint32_t*)desc->dst_palette)[index] = tig_color_rgb_to_grayscale(((uint32_t*)desc->dst_palette)[index]);
            }
        }

        if ((desc->flags & TIG_PALETTE_ADJUST_TINT) != 0) {
            for (index = 0; index < 256; index++) {
                ((uint32_t*)desc->dst_palette)[index] = tig_color_mult(((uint32_t*)desc->dst_palette)[index], desc->tint_color);
            }
        }
        break;
    case 32:
        // NOTE: The code in this branch is binary identical to 24 bpp, but for
        // unknown reason the generated assembly is not collapsed.
        if (desc->dst_palette != desc->src_palette) {
            memcpy(desc->dst_palette, desc->src_palette, sizeof(uint32_t) * 256);
        }

        if ((desc->flags & TIG_PALETTE_ADJUST_GRAYSCALE) != 0) {
            for (index = 0; index < 256; index++) {
                ((uint32_t*)desc->dst_palette)[index] = tig_color_rgb_to_grayscale(((uint32_t*)desc->dst_palette)[index]);
            }
        }

        if ((desc->flags & TIG_PALETTE_ADJUST_TINT) != 0) {
            for (index = 0; index < 256; index++) {
                ((uint32_t*)desc->dst_palette)[index] = tig_color_mult(((uint32_t*)desc->dst_palette)[index], desc->tint_color);
            }
        }
        break;
    }
}

// 0x534290
size_t tig_palette_get_size()
{
    return tig_palette_size + sizeof(TigPaletteListNode*);
}

// 0x5342A0
void tig_palette_node_reserve()
{
    int index;
    TigPaletteListNode* node;

    for (index = 0; index < 16; index++) {
        node = (TigPaletteListNode*)MALLOC(sizeof(*node));
        node->data = MALLOC(tig_palette_size + sizeof(TigPaletteListNode*));
        // Link back from palette to node.
        *(TigPaletteListNode**)node->data = node;
        node->next = tig_palette_head;
        tig_palette_head = node;
    }
}

// 0x5342E0
void tig_palette_node_clear()
{
    TigPaletteListNode* curr;
    TigPaletteListNode* next;

    curr = tig_palette_head;
    while (curr != NULL) {
        next = curr->next;
        FREE(curr->data);
        FREE(curr);
        curr = next;
    }

    tig_palette_head = NULL;
}
