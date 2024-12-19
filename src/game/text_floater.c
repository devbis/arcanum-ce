#include "game/text_floater.h"

#include "game/gamelib.h"
#include "game/object.h"

#define TEXT_FLOATERS_KEY "text floaters"
#define FLOAT_SPEED_KEY "float speed"

typedef struct TextFloaterNode {
    /* 0000 */ int field_0;
    /* 0004 */ TigVideoBuffer* video_buffer;
    /* 0008 */ TigRect rect;
    /* 0018 */ uint8_t field_18;
    /* 001C */ struct TextFloaterNode* next;
} TextFloaterNode;

// See 0x4D5950.
static_assert(sizeof(TextFloaterNode) == 0x20, "wrong size");

typedef struct TextFloaterList {
    int64_t obj;
    TextFloaterNode* head;
    struct TextFloaterList* next;
} TextFloaterList;

// See 0x4D57E0.
static_assert(sizeof(TextFloaterList) == 0x10, "wrong size");

static void tf_clear();
static TextFloaterList* tf_list_create();
static void tf_list_destroy(TextFloaterList *list);
static void sub_4D5850(TextFloaterList* node);
static void sub_4D5870(TextFloaterList* node, TigRect* rect);
static void sub_4D58C0(int64_t loc, int offset_x, int offset_y, TigRect* rect);
static TextFloaterNode* tf_node_create();
static void tf_node_destroy(TextFloaterNode* node);
static void sub_4D59F0(TextFloaterList *list, TextFloaterNode *node);
static void sub_4D5A30(TextFloaterList *list, TextFloaterNode *node, TigRect *a3);
static void sub_4D5A60(TigRect* a1, TextFloaterNode *node, TigRect *a3);
static void sub_4D5AA0(TextFloaterNode *node);
static void text_floaters_set_internal(int value);
static void text_floaters_changed();
static void float_speed_changed();

// 0x5B8E6C
static uint8_t tf_colors[TF_TYPE_COUNT][3] = {
    { 255, 255, 255 },
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 64, 64, 255 },
    { 255, 255, 0 },
};

// 0x602898
static int dword_602898;

// 0x60289C
static TextFloaterList* tf_free_list_head;

// 0x6028A0
static int float_speed;

// 0x6028A8
static ViewOptions text_floater_view_options;

// 0x6028B0
static TigRect stru_6028B0;

// 0x6028C0
static tig_color_t dword_6028C0;

// 0x6028C4
static GameContextF8* tf_iso_invalidate_rect;

// 0x6028C8
static tig_color_t dword_6028C8;

// 0x6028D0
static TigRect stru_6028D0;

// 0x6028E0
TextFloaterList *tf_list_head;

// 0x6028E4
tig_timestamp_t  dword_6028E4;

// 0x6028E8
static tig_window_handle_t tf_iso_window_handle;

// 0x6028EC
static int text_floaters;

// 0x6028F0
static TextFloaterNode* tf_free_node_head;

// 0x6028F8
static TigRect stru_6028F8;

// 0x602908
static tig_font_handle_t tf_fonts[TF_TYPE_COUNT];

// 0x4D4E20
bool text_floater_init(GameInitInfo* init_info)
{
    TigWindowData window_data;
    TigFont font_desc;
    TigArtFrameData art_frame_data;
    int index;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    tf_iso_window_handle = init_info->iso_window_handle;

    stru_6028B0.x = 0;
    stru_6028B0.y = 0;
    stru_6028B0.width = window_data.rect.width;
    stru_6028B0.height = window_data.rect.height;

    text_floater_view_options.type = VIEW_TYPE_ISOMETRIC;
    tf_iso_invalidate_rect = init_info->field_8;

    dword_6028C8 = tig_color_make(0, 0, 255);
    dword_6028C0 = tig_color_make(0, 0, 0);

    font_desc.flags = TIG_FONT_NO_ALPHA_BLEND | TIG_FONT_SHADOW;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));

    tig_art_frame_data(font_desc.art_id, &art_frame_data);
    dword_602898 = art_frame_data.height + 2;

    for (index = 0; index < TF_TYPE_COUNT; index++) {
        font_desc.color = tig_color_make(tf_colors[index][0], tf_colors[index][1], tf_colors[index][2]);
        tig_font_create(&font_desc, &(tf_fonts[index]));
    }

    stru_6028F8.x = 0;
    stru_6028F8.y = 0;
    stru_6028F8.width = 200;
    stru_6028F8.height = 5 * dword_602898;

    stru_6028D0.x = 0;
    stru_6028D0.y = 0;
    stru_6028D0.width = 200;
    stru_6028D0.height = dword_602898;

    settings_add(&settings, TEXT_FLOATERS_KEY, "1", text_floaters_changed);
    text_floaters_changed();

    settings_add(&settings, FLOAT_SPEED_KEY, "2", float_speed_changed);
    float_speed_changed();

    return true;
}

// 0x4D5050
void text_floater_resize(ResizeInfo* resize_info)
{
    stru_6028B0 = resize_info->field_14;
    tf_iso_window_handle = resize_info->iso_window_handle;
}

// 0x4D5090
void text_floater_reset()
{
    tf_clear();
}

// 0x4D5DC0
void text_floater_exit()
{
    int index;
    TextFloaterNode* next_node;
    TextFloaterList* next_list;

    // Clear active floaters.
    tf_clear();

    // Destroy fonts.
    for (index = 0; index < TF_TYPE_COUNT; index++) {
        tig_font_destroy(tf_fonts[index]);
    }

    // Clear free node objects.
    while (tf_free_node_head != NULL) {
        next_node = tf_free_node_head->next;
        tig_video_buffer_destroy(tf_free_node_head->video_buffer);
        FREE(tf_free_node_head);
        tf_free_node_head = next_node;
    }

    // Clear free list objects.
    while (tf_free_list_head != NULL) {
        next_list = tf_free_list_head->next;
        FREE(tf_free_list_head);
        tf_free_list_head = next_list;
    }

    tf_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
    tf_iso_invalidate_rect = 0;
}

// 0x4D5130
bool text_floater_update_view(ViewOptions* view_options)
{
    text_floater_view_options = *view_options;
    return true;
}

// 0x4D5150
void text_floater_map_close()
{
    tf_clear();
}

// 0x4D5160
int text_floaters_set(int value)
{
    text_floaters_set_internal(value);
    return text_floaters;
}

// 0x4D5180
int text_floaters_get()
{
    return text_floaters;
}

// 0x4D5190
void tf_ping(tig_timestamp_t timestamp)
{
    TextFloaterList* cur_list;
    TextFloaterList* prev_list;
    TextFloaterNode* cur_node;
    TextFloaterNode* prev_node;
    TigRect v3;
    TigRect v4;
    unsigned int flags;

    if (tig_timer_between(dword_6028E4, timestamp) >= 50) {
        dword_6028E4 = timestamp;

        prev_list = NULL;
        cur_list = tf_list_head;
        while (cur_list != NULL) {
            sub_4D5870(cur_list, &v3);

            prev_node = NULL;
            cur_node = cur_list->head;
            while (cur_node != NULL) {
                sub_4D5A60(&v3, cur_node, &v4);
                if (tig_rect_intersection(&v4, &v3, &v4) == TIG_OK) {
                    tf_iso_invalidate_rect(&v4);
                }

                cur_node->field_0 -= float_speed;
                if (cur_node->field_0 < 0) {
                    if (prev_node != NULL) {
                        prev_node->next = cur_node->next;
                    } else {
                        cur_list->head = cur_node->next;
                    }

                    tf_node_destroy(cur_node);

                    if (prev_node != NULL) {
                        cur_node = prev_node->next;
                    } else {
                        cur_node = cur_list->head;
                    }
                } else {
                    sub_4D5A60(&v3, cur_node, &v4);
                    if (tig_rect_intersection(&v4, &v3, &v4) == TIG_OK) {
                        tf_iso_invalidate_rect(&v4);
                    }
                    prev_node = cur_node;
                    cur_node = cur_node->next;
                }
            }

            if (cur_list->head != NULL) {
                prev_list = cur_list;
                cur_list = cur_list->next;
            } else {
                if (prev_list != NULL) {
                    prev_list->next = cur_list->next;
                    tf_list_destroy(cur_list);
                    cur_list = prev_list->next;
                } else {
                    tf_list_head = cur_list->next;
                    flags = obj_field_int32_get(cur_list->obj, OBJ_F_FLAGS);
                    flags &= ~OF_TEXT_FLOATER;
                    obj_field_int32_set(cur_list->obj, OBJ_F_FLAGS, flags);

                    tf_list_destroy(cur_list);
                    cur_list = tf_list_head;
                }
            }
        }
    }
}

// 0x4D5310
void sub_4D5310(UnknownContext *a1)
{
    TextFloaterList* list;
    TextFloaterNode* node;
    TigWindowBlitInfo window_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigRect tf_rect;
    TigRect v2;
    TigRectListNode* rect_node;
    TigRect rect;

    if (text_floaters == 0) {
        return;
    }

    if (text_floater_view_options.type != VIEW_TYPE_ISOMETRIC) {
        return;
    }

    window_blit_info.type = TIG_WINDOW_BLIT_VIDEO_BUFFER_TO_WINDOW;
    window_blit_info.vb_blit_flags = TIG_VIDEO_BUFFER_BLIT_BLEND_ALPHA_CONST;
    window_blit_info.src_rect = &src_rect;
    window_blit_info.dst_rect = &dst_rect;
    window_blit_info.dst_window_handle = tf_iso_window_handle;

    list = tf_list_head;
    while (list != NULL) {
        sub_4D5870(list, &tf_rect);

        rect_node = *a1->rects;
        while (rect_node != NULL) {
            if (tig_rect_intersection(&(rect_node->rect), &tf_rect, &rect) == TIG_OK) {
                node = list->head;
                while (node != NULL) {
                    sub_4D5A60(&tf_rect, node, &v2);
                    if (tig_rect_intersection(&v2, &rect, &dst_rect) == TIG_OK) {
                        src_rect.x = dst_rect.x - v2.x;
                        src_rect.y = dst_rect.y - v2.y;
                        src_rect.width = dst_rect.width;
                        src_rect.height = dst_rect.height;
                        window_blit_info.src_video_buffer = node->video_buffer;
                        window_blit_info.alpha[0] = node->field_18;
                        tig_window_blit(&window_blit_info);
                    }
                    node = node->next;
                }
            }
            rect_node = rect_node->next;
        }
        list = list->next;
    }
}

// 0x4D5450
void tf_add(int64_t obj, int type, const char* str)
{
    TextFloaterList* cur_list;
    TextFloaterList* prev_list;
    TextFloaterNode* cur_node;
    TextFloaterNode* prev_node;
    TigRect rect;

    prev_list = NULL;
    cur_list = tf_list_head;
    while (cur_list != NULL) {
        if (cur_list->obj == obj) {
            break;
        }
        prev_list = cur_list;
        cur_list = cur_list->next;
    }

    if (cur_list == NULL) {
        cur_list = tf_list_create();
        cur_list->obj = obj;
    }

    if (prev_list != NULL) {
        prev_list->next = cur_list;
    } else {
        tf_list_head = cur_list;
    }

    prev_node = NULL;
    cur_node = cur_list->head;
    while (cur_node != NULL) {
        prev_node = cur_node;
        cur_node = cur_node->next;
    }

    cur_node = tf_node_create();
    cur_node->field_0 = 4 * dword_602898;

    if (prev_node != NULL) {
        prev_node->next = cur_node;
        if (prev_node->field_0 > 3 * dword_602898) {
            cur_node->field_0 = prev_node->field_0 + dword_602898;
        }
    } else {
        cur_list->head = cur_node;
    }

    tig_video_buffer_fill(cur_node->video_buffer, &stru_6028D0, dword_6028C8);
    tig_font_push(tf_fonts[type]);
    tig_font_write(cur_node->video_buffer, str, &stru_6028D0, &(cur_node->rect));
    tig_font_pop();

    sub_43D0E0(obj, OF_TEXT_FLOATER);
    sub_4D5AA0(cur_node);

    sub_4D5A30(cur_list, cur_node, &rect);
    tf_iso_invalidate_rect(&rect);
}

// 0x4D5570
void sub_4D5570(int64_t obj, int64_t loc, int offset_x, int offset_y)
{
    TextFloaterList* list;
    TextFloaterNode* node;
    TigRect v2;
    TigRect v3;
    TigRect v5;

    list = tf_list_head;
    while (list != NULL) {
        if (list->obj == obj) {
            break;
        }
        list = list->next;
    }

    if (list != NULL) {
        sub_4D5870(list, &v2);
        sub_4D58C0(loc, offset_x, offset_y, &v3);

        node = list->head;
        while (node != NULL) {
            sub_4D5A60(&v2, node, &v5);
            tf_iso_invalidate_rect(&v5);

            sub_4D5A60(&v3, node, &v5);
            tf_iso_invalidate_rect(&v5);

            node = node->next;
        }
    }
}

// 0x4D5620
void sub_4D5620(int64_t obj)
{
    TextFloaterList* list;
    TextFloaterNode* node;
    TextFloaterNode* prev;
    TextFloaterNode* next;

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_TEXT_FLOATER) != 0) {
        list = tf_list_head;
        while (list != NULL) {
            if (list->obj == obj) {
                break;
            }
            list = list->next;
        }

        if (list != NULL) {
            prev = NULL;
            node = list->head;
            while (node != NULL) {
                if (node->field_0 >= stru_6028F8.height) {
                    break;
                }
                prev = node;
                node = node->next;
            }

            if (node != NULL) {
                if (prev != NULL) {
                    prev->next = NULL;
                    while (node != NULL) {
                        next = node->next;
                        tf_node_destroy(node);
                        node = next;
                    }
                } else {
                    tf_list_destroy(list);
                }
            }
        }
    }
}

// 0x4D56C0
void tf_remove(int64_t obj)
{
    unsigned int flags;
    TextFloaterList* prev;
    TextFloaterList* node;
    TigRect rect;

    flags = obj_field_int32_get(obj, OBJ_F_FLAGS);
    if ((flags & OF_TEXT_FLOATER) != 0) {
        flags &= ~OF_TEXT_FLOATER;
        obj_field_int32_set(obj, OBJ_F_FLAGS, flags);

        prev = NULL;
        node = tf_list_head;
        while (node != NULL) {
            if (node->obj == obj) {
                break;
            }
            prev = node;
            node = node->next;
        }

        if (node != NULL) {
            sub_4D5870(node, &rect);
            tf_iso_invalidate_rect(&rect);

            if (prev != NULL) {
                prev->next = node->next;
            } else {
                tf_list_head = node->next;
            }
            tf_list_destroy(node);
        }
    }
}

// 0x4D5780
void tf_clear()
{
    TextFloaterList* list;
    TextFloaterList* next;
    TigRect rect;

    list = tf_list_head;
    while (list != NULL) {
        next = list->next;

        sub_4D5870(list, &rect);
        tf_iso_invalidate_rect(&rect);

        tf_list_destroy(list);
        list = next;
    }
    tf_list_head = NULL;
}

// 0x4D57E0
TextFloaterList* tf_list_create()
{
    TextFloaterList* list;

    list = tf_free_list_head;
    if (list != NULL) {
        tf_free_list_head = list->next;
    } else {
        list = (TextFloaterList*)MALLOC(sizeof(*list));
    }

    list->obj = 0;
    list->head = 0;
    list->next = NULL;

    return list;
}

// 0x4D5820
void tf_list_destroy(TextFloaterList *list)
{
    TextFloaterNode* node;
    TextFloaterNode* next;

    node = list->head;
    while (node != NULL) {
        next = node->next;
        sub_4D59F0(list, node);
        node = next;
    }

    sub_4D5850(list);
}

// 0x4D5850
void sub_4D5850(TextFloaterList* node)
{
    node->next = tf_free_list_head;
    tf_free_list_head = node;
}

// 0x4D5870
void sub_4D5870(TextFloaterList* node, TigRect* rect)
{
    int64_t loc;
    int offset_x;
    int offset_y;

    loc = obj_field_int64_get(node->obj, OBJ_F_LOCATION);
    offset_x = obj_field_int32_get(node->obj, OBJ_F_OFFSET_X);
    offset_y = obj_field_int32_get(node->obj, OBJ_F_OFFSET_Y);
    sub_4D58C0(loc, offset_x, offset_y, rect);
}

// 0x4D58C0
void sub_4D58C0(int64_t loc, int offset_x, int offset_y, TigRect* rect)
{
    int64_t x;
    int64_t y;

    sub_4B8680(loc, &x, &y);

    rect->x = (int)x + offset_x - 60;
    rect->y = (int)y + offset_y + 20 - 5 * dword_602898 - 90;
    rect->width = stru_6028F8.width;
    rect->height = stru_6028F8.height;
}

// 0x4D5950
TextFloaterNode* tf_node_create()
{
    TextFloaterNode* node;
    TigVideoBufferCreateInfo vb_create_info;

    node = tf_free_node_head;
    if (node != NULL) {
        tf_free_node_head = tf_free_node_head->next;
        node->next = NULL;
    } else {
        node = (TextFloaterNode*)MALLOC(sizeof(*node));
        node->next = NULL;

        vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_COLOR_KEY | TIG_VIDEO_BUFFER_CREATE_VIDEO_MEMORY;
        vb_create_info.width = 200;
        vb_create_info.height = dword_602898;
        vb_create_info.background_color = dword_6028C8;
        vb_create_info.color_key = dword_6028C8;
        tig_video_buffer_create(&vb_create_info, &(node->video_buffer));
    }

    return node;
}

// 0x4D59D0
void tf_node_destroy(TextFloaterNode* node)
{
    node->next = tf_free_node_head;
    tf_free_node_head = node;
}

// 0x4D59F0
void sub_4D59F0(TextFloaterList *list, TextFloaterNode *node)
{
    TigRect rect;

    sub_4D5A30(list, node, &rect);
    tf_iso_invalidate_rect(&rect);

    tf_node_destroy(node);
}

// 0x4D5A30
void sub_4D5A30(TextFloaterList *list, TextFloaterNode *node, TigRect *a3)
{
    TigRect rect;

    sub_4D5870(list, &rect);
    sub_4D5A60(&rect, node, a3);
}

// 0x4D5A60
void sub_4D5A60(TigRect* a1, TextFloaterNode *node, TigRect *a3)
{
    a3->x = a1->x + (a1->width - node->rect.width) / 2;
    a3->y = a1->y + node->field_0;
    a3->width = node->rect.width;
    a3->height = node->rect.height;
}

// 0x4D5AA0
void sub_4D5AA0(TextFloaterNode *node)
{
    int v1;
    int v2;

    v1 = dword_602898 / 2;
    v2 = node->field_0 + v1;
    if (v2 > 3 * dword_602898) {
        v2 = 5 * dword_602898 - v2;
    }

    if (v2 > 2 * dword_602898) {
        node->field_18 = 255;
    } else if (v2 > v1) {
        node->field_18 = (uint8_t)((v2 - v1) / (2 * dword_602898 - v1) * 255.0f);
    } else {
        node->field_18 = 0;
    }
}

// 0x4D5B10
void text_floaters_set_internal(int value)
{
    if (value >= 0 && value <= 2) {
        settings_set_value(&settings, TEXT_FLOATERS_KEY, value);
    }
}

// 0x4D5B40
void text_floaters_changed()
{
    text_floaters = settings_get_value(&settings, TEXT_FLOATERS_KEY);
}

// 0x4D5B60
void float_speed_changed()
{
    float_speed = settings_get_value(&settings, FLOAT_SPEED_KEY) + 1;
}
