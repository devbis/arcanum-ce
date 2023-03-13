#include "tig/rect.h"

#define TIG_RECT_LIST_NODE_GROW_SIZE 20

// 0x62B2A4
static TigRectListNode* tig_rect_free_nodes;

static void tig_rect_node_reserve();

// 0x52D0E0
int tig_rect_init(TigContext* ctx)
{
    return TIG_OK;
}

// 0x52D0F0
void tig_rect_exit()
{
    TigRectListNode* curr = tig_rect_free_nodes;
    while (curr != NULL) {
        TigRectListNode* next = curr->next;
        free(curr);
        curr = next;
    }
    tig_rect_free_nodes = NULL;
}

// 0x52D120
TigRectListNode* tig_rect_node_create()
{
    TigRectListNode* node = tig_rect_free_nodes;
    if (node == NULL) {
        tig_rect_node_reserve();

        node = tig_rect_free_nodes;
        if (node == NULL) {
            return NULL;
        }
    }

    tig_rect_free_nodes = node->next;
    node->next = NULL;

    return node;
}

// 0x52D150
void tig_rect_node_reserve()
{
    for (int index = 0; index < TIG_RECT_LIST_NODE_GROW_SIZE; index++) {
        TigRectListNode* node = (TigRectListNode*)malloc(sizeof(*node));
        if (node != NULL) {
            node->next = tig_rect_free_nodes;
            tig_rect_free_nodes = node;
        }
    }
}

// 0x52D180
void tig_rect_node_destroy(TigRectListNode* node)
{
    node->next = tig_rect_free_nodes;
    tig_rect_free_nodes = node;
}

// 0x52D1A0
int tig_rect_intersection(TigRect* r1, TigRect* r2, TigRect* intersection)
{
    int x;
    int y;
    int width;
    int height;

    if (r1->x >= r2->x) {
        x = r1->x;
    } else {
        x = r2->x;
    }

    if (r1->y >= r2->y) {
        y = r1->y;
    } else {
        y = r2->y;
    }

    if (r1->width + r1->x <= r2->width + r2->x) {
        width = r1->x + r1->width - x;
    } else {
        width = r2->x + r2->width - x;
    }

    if (width <= 0) {
        return TIG_ERR_4;
    }

    if (r1->height + r1->y <= r2->height + r2->y) {
        height = r1->y + r1->height - x;
    } else {
        height = r2->y + r2->height - y;
    }

    if (height <= 0) {
        return TIG_ERR_4;
    }

    intersection->x = x;
    intersection->y = y;
    intersection->width = width;
    intersection->height = height;

    return TIG_OK;
}

// 0x52D260
int tig_rect_clip(TigRect* r1, TigRect* r2, TigRect* rects)
{
    if (r1->x >= r2->x
        && r1->y >= r2->y
        && r1->x + r1->width <= r2->x + r2->width
        && r1->y + r1->height <= r2->y + r2->height) {
        return 0;
    }

    TigRect intersection;
    if (tig_rect_intersection(r1, r2, &intersection) == TIG_ERR_4) {
        rects[0] = *r1;
        return 1;
    }

    int count = 0;

    if (intersection.y > r1->y) {
        rects[count].x = r1->x;
        rects[count].y = r1->y;
        rects[count].width = r1->width;
        rects[count].height = intersection.y - r1->y;
        count++;
    }

    if (intersection.x > r1->x) {
        rects[count].x = r1->x;
        rects[count].y = intersection.y;
        rects[count].width = intersection.x - r1->x;
        rects[count].height = intersection.height;
        count++;
    }

    if (intersection.width + intersection.x < r1->width + r1->x) {
        rects[count].x = intersection.width + intersection.x;
        rects[count].y = intersection.y;
        rects[count].width = r1->width + r1->x - intersection.width - intersection.x;
        rects[count].height = intersection.height;
        count++;
    }

    if (intersection.height + intersection.y < r1->height + r1->y) {
        rects[count].x = r1->x;
        rects[count].y = intersection.height + intersection.y;
        rects[count].width = r1->width;
        rects[count].height = r1->y + r1->height - intersection.height - intersection.y;
        count++;
    }

    return count;
}

// 0x52DBB0
int tig_rect_union(TigRect* r1, TigRect* r2, TigRect* r3)
{
    int x;
    int y;
    int width;
    int height;

    if (r1->x >= r2->x) {
        x = r2->x;
    } else {
        x = r1->x;
    }

    if (r1->y >= r2->y) {
        y = r2->y;
    } else {
        y = r1->y;
    }

    if (r1->width + r1->x <= r2->width + r2->x) {
        width = r2->x + r2->width - x;
    } else {
        width = r1->x + r1->width - x;
    }

    if (r1->height + r1->y <= r2->height + r2->x) {
        height = r2->height + r2->y - y;
    } else {
        height = r1->height + r1->y - y;
    }

    r3->x = x;
    r3->y = y;
    r3->width = width;
    r3->height = height;

    return TIG_OK;
}

// 0x52DF20
int tig_rect_normalize(TigRect* r1, TigRect* r2)
{
    if (r1->width <= r1->x) {
        r2->x = r1->width;
        r2->width = r1->x - r1->width;
    } else {
        r2->x = r1->x;
        r2->width = r1->width - r1->x;
    }

    if (r1->height <= r1->y) {
        r2->y = r1->height;
        r2->height = r1->y - r1->height;
    } else {
        r2->y = r1->y;
        r2->height = r1->height - r1->y;
    }

    return TIG_OK;
}
