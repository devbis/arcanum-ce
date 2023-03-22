#ifndef ARCANUM_TIG_FELICITY_H_
#define ARCANUM_TIG_FELICITY_H_

#include "tig/types.h"

typedef struct TigFont {
    unsigned int flags;
    unsigned int art_id;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
};

// See 0x5352E0.
static_assert(sizeof(TigFont) == 0x2C, "wrong size");

int tig_font_init(TigContext* ctx);
void tig_font_exit();
void tig_font_create(TigFont* src, TigFont** dst);
void tig_font_destroy(TigFont* font);
int tig_font_push(TigFont* font);
void tig_font_pop();

#endif /* ARCANUM_TIG_FELICITY_H_ */
