#include "game/tf.h"

#include "game/gamelib.h"
#include "game/object.h"

#define TEXT_FLOATERS_KEY "text floaters"
#define FLOAT_SPEED_KEY "float speed"

typedef struct TextFloaterEntry {
    /* 0000 */ int offset;
    /* 0004 */ TigVideoBuffer* video_buffer;
    /* 0008 */ TigRect rect;
    /* 0018 */ uint8_t opacity;
    /* 001C */ struct TextFloaterEntry* next;
} TextFloaterEntry;

// See 0x4D5950.
static_assert(sizeof(TextFloaterEntry) == 0x20, "wrong size");

typedef struct TextFloaterList {
    /* 0000 */ int64_t obj;
    /* 0008 */ TextFloaterEntry* head;
    /* 000C */ struct TextFloaterList* next;
} TextFloaterList;

// See 0x4D57E0.
static_assert(sizeof(TextFloaterList) == 0x10, "wrong size");

static void tf_clear();
static TextFloaterList* tf_list_create();
static void tf_list_destroy(TextFloaterList *list);
static void tf_list_free(TextFloaterList* node);
static void tf_list_get_rect(TextFloaterList* node, TigRect* rect);
static void tf_get_rect_at_loc(int64_t loc, int offset_x, int offset_y, TigRect* rect);
static TextFloaterEntry* tf_entry_create();
static void tf_entry_free(TextFloaterEntry* entry);
static void tf_entry_destroy(TextFloaterList *list, TextFloaterEntry* entry);
static void tf_entry_get_rect(TextFloaterList *list, TextFloaterEntry* entry, TigRect* entry_rect);
static void tf_entry_get_rect_constrained_to(TigRect* list_rect, TextFloaterEntry* entry, TigRect* entry_rect);
static void tf_entry_recalc_opacity(TextFloaterEntry* entry);
static void tf_level_set_internal(int value);
static void tf_level_changed();
static void tf_float_speed_changed();

// 0x5B8E6C
static uint8_t tf_colors[TF_TYPE_COUNT][3] = {
    { 255, 255, 255 },
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 64, 64, 255 },
    { 255, 255, 0 },
};

// 0x602898
static int tf_line_height;

// 0x60289C
static TextFloaterList* tf_free_lists_head;

// 0x6028A0
static int float_speed;

// 0x6028A8
static ViewOptions text_floater_view_options;

// 0x6028B0
static TigRect tf_iso_content_rect;

// 0x6028C0
static tig_color_t dword_6028C0;

// 0x6028C4
static IsoInvalidateRectFunc* tf_iso_invalidate_rect;

// 0x6028C8
static tig_color_t tf_background_color;

// 0x6028D0
static TigRect tf_entry_content_rect;

// 0x6028E0
TextFloaterList *tf_list_head;

// 0x6028E4
tig_timestamp_t  tf_ping_timestamp;

// 0x6028E8
static tig_window_handle_t tf_iso_window_handle;

// 0x6028EC
static int tf_level;

// 0x6028F0
static TextFloaterEntry* tf_free_entries_head;

// 0x6028F8
static TigRect tf_list_content_rect;

// 0x602908
static tig_font_handle_t tf_fonts[TF_TYPE_COUNT];

// 0x4D4E20
bool tf_init(GameInitInfo* init_info)
{
    TigWindowData window_data;
    TigFont font_desc;
    TigArtFrameData art_frame_data;
    int index;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    tf_iso_window_handle = init_info->iso_window_handle;

    tf_iso_content_rect.x = 0;
    tf_iso_content_rect.y = 0;
    tf_iso_content_rect.width = window_data.rect.width;
    tf_iso_content_rect.height = window_data.rect.height;

    text_floater_view_options.type = VIEW_TYPE_ISOMETRIC;
    tf_iso_invalidate_rect = init_info->invalidate_rect_func;

    tf_background_color = tig_color_make(0, 0, 255);
    dword_6028C0 = tig_color_make(0, 0, 0);

    font_desc.flags = TIG_FONT_NO_ALPHA_BLEND | TIG_FONT_SHADOW;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));

    tig_art_frame_data(font_desc.art_id, &art_frame_data);
    tf_line_height = art_frame_data.height + 2;

    for (index = 0; index < TF_TYPE_COUNT; index++) {
        font_desc.color = tig_color_make(tf_colors[index][0], tf_colors[index][1], tf_colors[index][2]);
        tig_font_create(&font_desc, &(tf_fonts[index]));
    }

    tf_list_content_rect.x = 0;
    tf_list_content_rect.y = 0;
    tf_list_content_rect.width = 200;
    tf_list_content_rect.height = 5 * tf_line_height;

    tf_entry_content_rect.x = 0;
    tf_entry_content_rect.y = 0;
    tf_entry_content_rect.width = 200;
    tf_entry_content_rect.height = tf_line_height;

    settings_add(&settings, TEXT_FLOATERS_KEY, "1", tf_level_changed);
    tf_level_changed();

    settings_add(&settings, FLOAT_SPEED_KEY, "2", tf_float_speed_changed);
    tf_float_speed_changed();

    return true;
}

// 0x4D5050
void tf_resize(GameResizeInfo* resize_info)
{
    tf_iso_content_rect = resize_info->content_rect;
    tf_iso_window_handle = resize_info->window_handle;
}

// 0x4D5090
void tf_reset()
{
    tf_clear();
}

// 0x4D5DC0
void tf_exit()
{
    int index;
    TextFloaterEntry* next_node;
    TextFloaterList* next_list;

    // Clear active floaters.
    tf_clear();

    // Destroy fonts.
    for (index = 0; index < TF_TYPE_COUNT; index++) {
        tig_font_destroy(tf_fonts[index]);
    }

    // Clear free node objects.
    while (tf_free_entries_head != NULL) {
        next_node = tf_free_entries_head->next;
        tig_video_buffer_destroy(tf_free_entries_head->video_buffer);
        FREE(tf_free_entries_head);
        tf_free_entries_head = next_node;
    }

    // Clear free list objects.
    while (tf_free_lists_head != NULL) {
        next_list = tf_free_lists_head->next;
        FREE(tf_free_lists_head);
        tf_free_lists_head = next_list;
    }

    tf_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
    tf_iso_invalidate_rect = NULL;
}

// 0x4D5130
void tf_update_view(ViewOptions* view_options)
{
    text_floater_view_options = *view_options;
}

// 0x4D5150
void tf_map_close()
{
    tf_clear();
}

// 0x4D5160
int tf_level_set(int value)
{
    tf_level_set_internal(value);
    return tf_level;
}

// 0x4D5180
int tf_level_get()
{
    return tf_level;
}

// 0x4D5190
void tf_ping(tig_timestamp_t timestamp)
{
    TextFloaterList* list;
    TextFloaterList* prev_list;
    TextFloaterEntry* entry;
    TextFloaterEntry* prev_entry;
    TigRect list_rect;
    TigRect entry_rect;
    unsigned int flags;

    if (tig_timer_between(tf_ping_timestamp, timestamp) < 50) {
        return;
    }

    tf_ping_timestamp = timestamp;

    prev_list = NULL;
    list = tf_list_head;
    while (list != NULL) {
        tf_list_get_rect(list, &list_rect);

        prev_entry = NULL;
        entry = list->head;
        while (entry != NULL) {
            tf_entry_get_rect_constrained_to(&list_rect, entry, &entry_rect);
            if (tig_rect_intersection(&entry_rect, &list_rect, &entry_rect) == TIG_OK) {
                tf_iso_invalidate_rect(&entry_rect);
            }

            entry->offset -= float_speed;
            if (entry->offset < 0) {
                if (prev_entry != NULL) {
                    prev_entry->next = entry->next;
                } else {
                    list->head = entry->next;
                }

                tf_entry_free(entry);

                if (prev_entry != NULL) {
                    entry = prev_entry->next;
                } else {
                    entry = list->head;
                }
            } else {
                tf_entry_get_rect_constrained_to(&list_rect, entry, &entry_rect);
                if (tig_rect_intersection(&entry_rect, &list_rect, &entry_rect) == TIG_OK) {
                    tf_entry_recalc_opacity(entry);
                    tf_iso_invalidate_rect(&entry_rect);
                }
                prev_entry = entry;
                entry = entry->next;
            }
        }

        if (list->head != NULL) {
            prev_list = list;
            list = list->next;
        } else {
            if (prev_list != NULL) {
                prev_list->next = list->next;
                tf_list_destroy(list);
                list = prev_list->next;
            } else {
                tf_list_head = list->next;
                flags = obj_field_int32_get(list->obj, OBJ_F_FLAGS);
                flags &= ~OF_TEXT_FLOATER;
                obj_field_int32_set(list->obj, OBJ_F_FLAGS, flags);

                tf_list_destroy(list);
                list = tf_list_head;
            }
        }
    }
}

// 0x4D5310
void tf_draw(GameDrawInfo* draw_info)
{
    TextFloaterList* list;
    TextFloaterEntry* entry;
    TigWindowBlitInfo window_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigRect list_rect;
    TigRect entry_rect;
    TigRectListNode* rect_node;
    TigRect dirty_list_rect;

    if (tf_level == TF_LEVEL_NEVER) {
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

    for (list = tf_list_head; list != NULL; list = list->next) {
        tf_list_get_rect(list, &list_rect);

        for (rect_node = *draw_info->rects; rect_node != NULL; rect_node = rect_node->next) {
            if (tig_rect_intersection(&(rect_node->rect), &list_rect, &dirty_list_rect) != TIG_OK) {
                continue;
            }

            for (entry = list->head; entry != NULL; entry = entry->next) {
                tf_entry_get_rect_constrained_to(&list_rect, entry, &entry_rect);

                if (tig_rect_intersection(&entry_rect, &dirty_list_rect, &dst_rect) != TIG_OK) {
                    continue;
                }

                src_rect.x = dst_rect.x - entry_rect.x;
                src_rect.y = dst_rect.y - entry_rect.y;
                src_rect.width = dst_rect.width;
                src_rect.height = dst_rect.height;

                window_blit_info.src_video_buffer = entry->video_buffer;
                window_blit_info.alpha[0] = entry->opacity;

                tig_window_blit(&window_blit_info);
            }
        }
    }
}

// 0x4D5450
void tf_add(int64_t obj, int type, const char* str)
{
    TextFloaterList* list;
    TextFloaterList* prev_list;
    TextFloaterEntry* entry;
    TextFloaterEntry* prev_entry;
    TigRect rect;

    prev_list = NULL;
    list = tf_list_head;
    while (list != NULL) {
        if (list->obj == obj) {
            break;
        }
        prev_list = list;
        list = list->next;
    }

    if (list == NULL) {
        list = tf_list_create();
        list->obj = obj;
    }

    if (prev_list != NULL) {
        prev_list->next = list;
    } else {
        tf_list_head = list;
    }

    prev_entry = NULL;
    entry = list->head;
    while (entry != NULL) {
        prev_entry = entry;
        entry = entry->next;
    }

    entry = tf_entry_create();
    entry->offset = 4 * tf_line_height;

    if (prev_entry != NULL) {
        prev_entry->next = entry;
        if (prev_entry->offset > 3 * tf_line_height) {
            entry->offset = prev_entry->offset + tf_line_height;
        }
    } else {
        list->head = entry;
    }

    tig_video_buffer_fill(entry->video_buffer, &tf_entry_content_rect, tf_background_color);
    tig_font_push(tf_fonts[type]);
    tig_font_write(entry->video_buffer, str, &tf_entry_content_rect, &(entry->rect));
    tig_font_pop();

    object_flags_set(obj, OF_TEXT_FLOATER);
    tf_entry_recalc_opacity(entry);

    tf_entry_get_rect(list, entry, &rect);
    tf_iso_invalidate_rect(&rect);
}

// 0x4D5570
void tf_notify_moved(int64_t obj, int64_t loc, int offset_x, int offset_y)
{
    TextFloaterList* list;
    TextFloaterEntry* entry;
    TigRect old_list_rect;
    TigRect new_list_rect;
    TigRect entry_rect;

    list = tf_list_head;
    while (list != NULL) {
        if (list->obj == obj) {
            break;
        }
        list = list->next;
    }

    if (list != NULL) {
        tf_list_get_rect(list, &old_list_rect);
        tf_get_rect_at_loc(loc, offset_x, offset_y, &new_list_rect);

        entry = list->head;
        while (entry != NULL) {
            tf_entry_get_rect_constrained_to(&old_list_rect, entry, &entry_rect);
            tf_iso_invalidate_rect(&entry_rect);

            tf_entry_get_rect_constrained_to(&new_list_rect, entry, &entry_rect);
            tf_iso_invalidate_rect(&entry_rect);

            entry = entry->next;
        }
    }
}

// 0x4D5620
void tf_notify_killed(int64_t obj)
{
    TextFloaterList* list;
    TextFloaterEntry* entry;
    TextFloaterEntry* prev_entry;
    TextFloaterEntry* next_entry;

    if ((obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_TEXT_FLOATER) == 0) {
        return;
    }

    list = tf_list_head;
    while (list != NULL) {
        if (list->obj == obj) {
            break;
        }
        list = list->next;
    }

    if (list != NULL) {
        prev_entry = NULL;
        entry = list->head;
        while (entry != NULL) {
            if (entry->offset >= tf_list_content_rect.height) {
                break;
            }
            prev_entry = entry;
            entry = entry->next;
        }

        if (entry != NULL) {
            if (prev_entry != NULL) {
                prev_entry->next = NULL;
                while (entry != NULL) {
                    next_entry = entry->next;
                    tf_entry_free(entry);
                    entry = next_entry;
                }
            } else {
                tf_list_destroy(list);
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
    if ((flags & OF_TEXT_FLOATER) == 0) {
        return;
    }

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
        tf_list_get_rect(node, &rect);
        tf_iso_invalidate_rect(&rect);

        if (prev != NULL) {
            prev->next = node->next;
        } else {
            tf_list_head = node->next;
        }
        tf_list_destroy(node);
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

        tf_list_get_rect(list, &rect);
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

    if (tf_free_lists_head != NULL) {
        list = tf_free_lists_head;
        tf_free_lists_head = list->next;
    } else {
        list = (TextFloaterList*)MALLOC(sizeof(*list));
    }

    list->obj = OBJ_HANDLE_NULL;
    list->head = NULL;
    list->next = NULL;

    return list;
}

// 0x4D5820
void tf_list_destroy(TextFloaterList *list)
{
    TextFloaterEntry* curr;
    TextFloaterEntry* next;

    curr = list->head;
    while (curr != NULL) {
        next = curr->next;
        tf_entry_destroy(list, curr);
        curr = next;
    }

    tf_list_free(list);
}

// 0x4D5850
void tf_list_free(TextFloaterList* node)
{
    node->next = tf_free_lists_head;
    tf_free_lists_head = node;
}

// 0x4D5870
void tf_list_get_rect(TextFloaterList* node, TigRect* rect)
{
    int64_t loc;
    int offset_x;
    int offset_y;

    loc = obj_field_int64_get(node->obj, OBJ_F_LOCATION);
    offset_x = obj_field_int32_get(node->obj, OBJ_F_OFFSET_X);
    offset_y = obj_field_int32_get(node->obj, OBJ_F_OFFSET_Y);
    tf_get_rect_at_loc(loc, offset_x, offset_y, rect);
}

// 0x4D58C0
void tf_get_rect_at_loc(int64_t loc, int offset_x, int offset_y, TigRect* rect)
{
    int64_t sx;
    int64_t sy;

    location_xy(loc, &sx, &sy);

    rect->x = (int)sx + offset_x - 60;
    rect->y = (int)sy + offset_y + 20 - 5 * tf_line_height - 90;
    rect->width = tf_list_content_rect.width;
    rect->height = tf_list_content_rect.height;
}

// 0x4D5950
TextFloaterEntry* tf_entry_create()
{
    TextFloaterEntry* entry;
    TigVideoBufferCreateInfo vb_create_info;

    if (tf_free_entries_head != NULL) {
        entry = tf_free_entries_head;
        tf_free_entries_head = entry->next;

        entry->next = NULL;

        return entry;
    }

    entry = (TextFloaterEntry*)MALLOC(sizeof(*entry));
    entry->next = NULL;

    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_COLOR_KEY | TIG_VIDEO_BUFFER_CREATE_VIDEO_MEMORY;
    vb_create_info.width = 200;
    vb_create_info.height = tf_line_height;
    vb_create_info.background_color = tf_background_color;
    vb_create_info.color_key = tf_background_color;
    tig_video_buffer_create(&vb_create_info, &(entry->video_buffer));

    return entry;
}

// 0x4D59D0
void tf_entry_free(TextFloaterEntry* entry)
{
    entry->next = tf_free_entries_head;
    tf_free_entries_head = entry;
}

// 0x4D59F0
void tf_entry_destroy(TextFloaterList *list, TextFloaterEntry* entry)
{
    TigRect rect;

    tf_entry_get_rect(list, entry, &rect);
    tf_iso_invalidate_rect(&rect);

    tf_entry_free(entry);
}

// 0x4D5A30
void tf_entry_get_rect(TextFloaterList *list, TextFloaterEntry* entry, TigRect* entry_rect)
{
    TigRect rect;

    tf_list_get_rect(list, &rect);
    tf_entry_get_rect_constrained_to(&rect, entry, entry_rect);
}

// 0x4D5A60
void tf_entry_get_rect_constrained_to(TigRect* list_rect, TextFloaterEntry* entry, TigRect* entry_rect)
{
    entry_rect->x = list_rect->x + (list_rect->width - entry->rect.width) / 2;
    entry_rect->y = list_rect->y + entry->offset;
    entry_rect->width = entry->rect.width;
    entry_rect->height = entry->rect.height;
}

// 0x4D5AA0
void tf_entry_recalc_opacity(TextFloaterEntry* entry)
{
    int mid;
    int pos;

    mid = tf_line_height / 2;
    pos = entry->offset + mid;
    if (pos > 3 * tf_line_height) {
        pos = 5 * tf_line_height - pos;
    }

    if (pos > 2 * tf_line_height) {
        entry->opacity = 255;
    } else if (pos > mid) {
        entry->opacity = (uint8_t)((float)(pos - mid) / (2 * tf_line_height - mid) * 255.0f);
    } else {
        entry->opacity = 0;
    }
}

// 0x4D5B10
void tf_level_set_internal(int value)
{
    if (!(value >= 0 && value < TF_LEVEL_COUNT)) {
        return;
    }

    settings_set_value(&settings, TEXT_FLOATERS_KEY, value);
}

// 0x4D5B40
void tf_level_changed()
{
    tf_level = settings_get_value(&settings, TEXT_FLOATERS_KEY);
}

// 0x4D5B60
void tf_float_speed_changed()
{
    float_speed = settings_get_value(&settings, FLOAT_SPEED_KEY) + 1;
}
