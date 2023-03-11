#include "tig/felicity.h"

#include "tig/art.h"
#include "tig/color.h"

#define FELICITY_MAX 20

// 0x630CFC
static int tig_felicity_index;

// 0x630D00
static Felicity* dword_630D00[FELICITY_MAX];

// 0x630D50
static unsigned int dword_630D50;

// 0x630D54
static Felicity* off_630D54;

// 0x5351D0
int tig_felicity_init(TigContext* ctx)
{
    tig_felicity_index = -1;

    Felicity v1;
    v1.field_0 = 8;
    sub_501DD0(10, 0, &(v1.art_id));
    dword_630D50 = tig_color_rgb_make(10, 10, 10);
    v1.field_18 = 0x3FC00000;
    v1.field_1C = 0;
    v1.field_8 = tig_color_rgb_make(255, 255, 255);
    v1.field_C = v1.field_8;
    v1.field_14 = v1.field_8;
    tig_felicity_create(&v1, &off_630D54);
    tig_felicity_add(off_630D54);

    return TIG_OK;
}

// 0x5352C0
void tig_felicity_exit()
{
    sub_535380();
    tig_felicity_destroy(off_630D54);
}

// 0x5352E0
void tig_felicity_create(Felicity* src, Felicity** dst)
{
    Felicity* copy = (Felicity*)malloc(sizeof(*copy));
    memcpy(copy, src, sizeof(Felicity));

    TigArtData art_data;
    if (tig_art_data(src->art_id, &art_data) != TIG_OK) {
        exit(EXIT_FAILURE);
    }

    *dst = copy;
}

// 0x535330
void tig_felicity_destroy(Felicity* a1)
{
    free(a1);
}

// 0x535340
int tig_felicity_add(Felicity* a1)
{
    tig_felicity_index++;
    if (tig_felicity_index < FELICITY_MAX) {
        dword_630D00[tig_felicity_index] = a1;
    } else {
        tig_felicity_index = 1;
        dword_630D00[tig_felicity_index] = a1;
    }

    return tig_felicity_index;
}

// 0x535380
void sub_535380()
{
    if (tig_felicity_index > 0) {
        tig_felicity_index--;
    }
}
