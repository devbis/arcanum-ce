#ifndef ARCANUM_TIG_RECT_H_
#define ARCANUM_TIG_RECT_H_

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// A 2D axis-aligned rectangle whose coordinates are specified using origin and
/// size.
typedef struct TigRect {
    /* 0000 */ int x;
    /* 0004 */ int y;
    /* 0008 */ int width;
    /* 000C */ int height;
} TigRect;

static_assert(sizeof(TigRect) == 0x10, "wrong size");

typedef struct TigRectListNode {
    /* 0000 */ TigRect rect;
    /* 0010 */ struct TigRectListNode* next;
} TigRectListNode;

static_assert(sizeof(TigRectListNode) == 0x14, "wrong size");

/// A 2D axis-aligned rectangle whose coordinates are specified using two
/// points (left-top/right-bottom).
///
/// NOTE: I'm pretty much sure there was no such type and they simply used
/// Windows' `RECT`.
typedef struct TigLtrbRect {
    /* 0000 */ int left;
    /* 0004 */ int top;
    /* 0008 */ int right;
    /* 000C */ int bottom;
} TigLtrbRect;

static_assert(sizeof(TigLtrbRect) == 0x10, "wrong size");

int tig_rect_init(TigContext* ctx);
void tig_rect_exit();
TigRectListNode* tig_rect_node_create();
void tig_rect_node_destroy(TigRectListNode* node);

/// Computes an intersection of two given rectangles.
///
/// If the two rectangles overlap returns `TIG_OK` and the resulting rect is
/// available in `r`. If the two rectangles do not overlap returns `TIG_ERR_4`
/// and the resulting rect is undefined.
int tig_rect_intersection(const TigRect* a, const TigRect* b, TigRect* r);

/// Slices given rectangle `a` with `b` producing at most 4 excluded rectangles,
/// one per side.
///
/// Returns number of excluded rectangles.
///
/// Slicing scheme:
///     +-----------------+
///     | (1)             |
///     +-----+-----+-----+
///     | (2) | (b) | (3) |
///     +-----+-----+-----+
///     | (4)             |
///     +-----------------+
int tig_rect_clip(const TigRect* a, const TigRect* b, TigRect* rects);

/// Computes a union of two rectangles.
///
/// Returns `TIG_OK` (its always possible to compute a union)
int tig_rect_union(const TigRect* a, const TigRect* b, TigRect* r);

int tig_rect_clamp(const TigRect* rect, TigLtrbRect* ltrb);

/// Convers LTRB-style rectangle to Origin/Size-style.
int tig_rect_normalize(const TigLtrbRect* ltrb, TigRect* rect);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_RECT_H_ */
