#ifndef ARCANUM_TIG_RECT_H_
#define ARCANUM_TIG_RECT_H_

#include "tig/types.h"

typedef struct TigRectListNode {
    TigRect rect;
    TigRectListNode* next;
};

int tig_rect_init(TigContext* ctx);
void tig_rect_exit();
TigRectListNode* tig_rect_node_create();
void tig_rect_node_destroy(TigRectListNode* node);
int tig_rect_intersection(TigRect* r1, TigRect* r2, TigRect* intersection);
int tig_rect_clip(TigRect* r1, TigRect* r2, TigRect* rects);
int tig_rect_union(TigRect* r1, TigRect* r2, TigRect* r3);
int tig_rect_normalize(TigRect* r1, TigRect* r2);

#endif /* ARCANUM_TIG_RECT_H_ */
