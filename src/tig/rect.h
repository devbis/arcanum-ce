#ifndef ARCANUM_TIG_RECT_H_
#define ARCANUM_TIG_RECT_H_

#include "tig/types.h"

typedef struct TigRectListNode {
    TigRect rect;
    TigRectListNode* next;
};

#endif /* ARCANUM_TIG_RECT_H_ */
