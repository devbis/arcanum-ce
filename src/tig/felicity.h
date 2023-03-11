#ifndef ARCANUM_TIG_FELICITY_H_
#define ARCANUM_TIG_FELICITY_H_

#include "tig/types.h"

typedef struct Felicity {
    int field_0;
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
static_assert(sizeof(Felicity) == 0x2C, "wrong size");

int tig_felicity_init(TigContext* ctx);
void tig_felicity_exit();
void tig_felicity_create(Felicity* src, Felicity** dst);
void tig_felicity_destroy(Felicity* a1);
int tig_felicity_add(Felicity* a1);
void sub_535380();

#endif /* ARCANUM_TIG_FELICITY_H_ */
