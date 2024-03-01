#include "tig/rect.h"

#include "tig/memory.h"

/// Size of batch during node allocation.
#define GROW 20

#define MISS_BOTTOM 0x8
#define MISS_TOP 0x4
#define MISS_RIGHT 0x2
#define MISS_LEFT 0x1

static void tig_rect_node_reserve();
static void sub_52DC90(float x, float y, TigLtrbRect* ltrb, unsigned int* flags);

// 0x62B2A4
static TigRectListNode* tig_rect_free_node_head;

// 0x52D0E0
int tig_rect_init(TigContext* ctx)
{
    (void)ctx;

    return TIG_OK;
}

// 0x52D0F0
void tig_rect_exit()
{
    TigRectListNode* curr;
    TigRectListNode* next;

    curr = tig_rect_free_node_head;
    while (curr != NULL) {
        next = curr->next;
        FREE(curr);
        curr = next;
    }

    tig_rect_free_node_head = NULL;
}

// 0x52D120
TigRectListNode* tig_rect_node_create()
{
    TigRectListNode* node;

    node = tig_rect_free_node_head;
    if (node == NULL) {
        tig_rect_node_reserve();

        node = tig_rect_free_node_head;
        if (node == NULL) {
            return NULL;
        }
    }

    tig_rect_free_node_head = node->next;
    node->next = NULL;

    return node;
}

// 0x52D150
void tig_rect_node_reserve()
{
    int index;
    TigRectListNode* node;

    for (index = 0; index < GROW; index++) {
        node = (TigRectListNode*)MALLOC(sizeof(*node));
        if (node != NULL) {
            node->next = tig_rect_free_node_head;
            tig_rect_free_node_head = node;
        }
    }
}

// 0x52D180
void tig_rect_node_destroy(TigRectListNode* node)
{
    node->next = tig_rect_free_node_head;
    tig_rect_free_node_head = node;
}

// 0x52D1A0
int tig_rect_intersection(const TigRect* a, const TigRect* b, TigRect* r)
{
    int x;
    int y;
    int width;
    int height;

    if (a->x >= b->x) {
        x = a->x;
    } else {
        x = b->x;
    }

    if (a->y >= b->y) {
        y = a->y;
    } else {
        y = b->y;
    }

    if (a->width + a->x <= b->width + b->x) {
        width = a->x + a->width - x;
    } else {
        width = b->x + b->width - x;
    }

    if (width <= 0) {
        return TIG_ERR_4;
    }

    if (a->height + a->y <= b->height + b->y) {
        height = a->y + a->height - y;
    } else {
        height = b->y + b->height - y;
    }

    if (height <= 0) {
        return TIG_ERR_4;
    }

    r->x = x;
    r->y = y;
    r->width = width;
    r->height = height;

    return TIG_OK;
}

// 0x52D260
int tig_rect_clip(const TigRect* a, const TigRect* b, TigRect* rects)
{
    TigRect intersection;
    int count = 0;

    if (a->x >= b->x
        && a->y >= b->y
        && a->x + a->width <= b->x + b->width
        && a->y + a->height <= b->y + b->height) {
        return 0;
    }

    if (tig_rect_intersection(a, b, &intersection) == 4) {
        rects[0] = *a;
        return 1;
    }

    if (intersection.y > a->y) {
        rects[count].x = a->x;
        rects[count].y = a->y;
        rects[count].width = a->width;
        rects[count].height = intersection.y - a->y;
        count++;
    }

    if (intersection.x > a->x) {
        rects[count].x = a->x;
        rects[count].y = intersection.y;
        rects[count].width = intersection.x - a->x;
        rects[count].height = intersection.height;
        count++;
    }

    if (intersection.width + intersection.x < a->width + a->x) {
        rects[count].x = intersection.width + intersection.x;
        rects[count].y = intersection.y;
        rects[count].width = a->width + a->x - intersection.width - intersection.x;
        rects[count].height = intersection.height;
        count++;
    }

    if (intersection.height + intersection.y < a->height + a->y) {
        rects[count].x = a->x;
        rects[count].y = intersection.height + intersection.y;
        rects[count].width = a->width;
        rects[count].height = a->y + a->height - intersection.height - intersection.y;
        count++;
    }

    return count;
}

// 0x52DBB0
int tig_rect_union(const TigRect* a, const TigRect* b, TigRect* r)
{
    int x;
    int y;
    int width;
    int height;

    if (a->x >= b->x) {
        x = b->x;
    } else {
        x = a->x;
    }

    if (a->y >= b->y) {
        y = b->y;
    } else {
        y = a->y;
    }

    if (a->width + a->x <= b->width + b->x) {
        width = b->x + b->width - x;
    } else {
        width = a->x + a->width - x;
    }

    if (a->height + a->y <= b->height + b->y) {
        height = b->height + b->y - y;
    } else {
        height = a->height + a->y - y;
    }

    r->x = x;
    r->y = y;
    r->width = width;
    r->height = height;

    return TIG_OK;
}

// 0x52DC90
void sub_52DC90(float x, float y, TigLtrbRect* ltrb, unsigned int* flags)
{
    *flags = 0;

    if (y > (float)ltrb->bottom) {
        *flags |= MISS_BOTTOM;
    } else if (y < (float)ltrb->top) {
        *flags |= MISS_TOP;
    }

    if (x > (float)ltrb->right) {
        *flags |= MISS_RIGHT;
    } else if (x < (float)ltrb->left) {
        *flags |= MISS_LEFT;
    }
}

// 0x52DD00
int tig_rect_clamp(const TigRect* rect, TigLtrbRect* ltrb)
{
    bool clamped = false;
    float x1, y1;
    float x2, y2;
    float x3, y3;
    TigLtrbRect temp_ltrb;
    unsigned int flags1;
    unsigned int flags2;
    unsigned int flags;

    x1 = (float)ltrb->left;
    y1 = (float)ltrb->top;
    x2 = (float)ltrb->right;
    y2 = (float)ltrb->bottom;

    x3 = x2;
    y3 = y2;

    temp_ltrb.left = rect->x;
    temp_ltrb.top = rect->y;
    temp_ltrb.right = rect->x + rect->width;
    temp_ltrb.bottom = rect->y + rect->height;

    sub_52DC90(x1, y1, &temp_ltrb, &flags1);
    sub_52DC90(x2, y2, &temp_ltrb, &flags2);

    while (1) {
        if (flags1 == 0 && flags2 == 0) {
            clamped = true;
            break;
        }

        if (flags1 != 0 && flags2 != 0) {
            break;
        }

        flags = flags1 != 0 ? flags1 : flags2;

        if ((flags & MISS_BOTTOM) != 0) {
            y3 = (float)temp_ltrb.bottom;
            x3 = (y3 - y1) * (x2 - x1) / (y2 - y1) + x1;
        } else if ((flags & MISS_TOP) != 0) {
            y3 = (float)temp_ltrb.top;
            x3 = (y3 - y1) * (x2 - x1) / (y2 - y1) + x1;
        } else if ((flags & MISS_RIGHT) != 0) {
            x3 = (float)temp_ltrb.right;
            y3 = (x3 - x1) * (y2 - y1) / (x2 - x1) + y1;
        } else if ((flags & MISS_LEFT) != 0) {
            x3 = (float)temp_ltrb.left;
            y3 = (x3 - x1) * (y2 - y1) / (x2 - x1) + y1;
        }

        if (flags1 == flags2) {
            x1 = x3;
            y1 = y3;
            sub_52DC90(x1, y1, &temp_ltrb, &flags1);
        } else {
            x2 = x3;
            x2 = y3;
            sub_52DC90(x2, y2, &temp_ltrb, &flags2);
        }
    }

    ltrb->left = (int)x1;
    ltrb->top = (int)y1;
    ltrb->right = (int)x2;
    ltrb->bottom = (int)y2;

    return clamped ? TIG_OK : TIG_ERR_4;
}

// 0x52DF20
int tig_rect_normalize(const TigLtrbRect* ltrb, TigRect* rect)
{
    if (ltrb->right <= ltrb->left) {
        rect->x = ltrb->right;
        rect->width = ltrb->left - ltrb->right;
    } else {
        rect->x = ltrb->left;
        rect->width = ltrb->right - ltrb->left;
    }

    if (ltrb->bottom <= ltrb->top) {
        rect->y = ltrb->bottom;
        rect->height = ltrb->top - ltrb->bottom;
    } else {
        rect->y = ltrb->top;
        rect->height = ltrb->bottom - ltrb->top;
    }

    return TIG_OK;
}
