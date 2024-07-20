#include "game/lib/background.h"

#include "game/lib/effect.h"
#include "game/lib/message.h"
#include "tig/debug.h"

#define FIRST_DESCRIPTION_ID 1000
#define BACKGROUND_BLOCK_SIZE 10
#define BACKGROUND_FIELD_DESCRIPTION 0
#define BACKGROUND_FIELD_MONEY 3
#define BACKGROUND_FIELD_ITEMS 4

#define BACKGROUND_NAME_LENGTH 32

static int background_get_base_money();

// 0x5FD858
static char* background_description_name;

// 0x5FD85C
static int background_msg_file;

// 0x5FD860
static int background_description_msg_file;

// 0x4C2270
bool background_init(GameContext* ctx)
{
    if (!message_load("rules\\backgrnd.mes", &background_msg_file)) {
        return false;
    }

    if (!message_load("mes\\gameback.mes", &background_description_msg_file)) {
        message_unload(background_msg_file);
        return false;
    }

    background_description_name = (char*)calloc(BACKGROUND_NAME_LENGTH, sizeof(*background_description_name));

    return true;
}

// 0x4C22D0
void background_exit()
{
    free(background_description_name);
    message_unload(background_msg_file);
    message_unload(background_description_msg_file);
}

// 0x4C24B0
int background_get_description(int background)
{
    MessageListItem msg;
    msg.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_FIELD_DESCRIPTION;
    sub_4D43A0(background_msg_file, &msg);
    return atoi(msg.text);
}

// 0x4C24E0
const char* background_description_get_text(int num)
{
    MessageListItem msg;

    if (num < FIRST_DESCRIPTION_ID) {
        if (num != 0) {
            return NULL;
        }
        num = FIRST_DESCRIPTION_ID;
    }

    msg.num = num;
    if (!message_find(background_description_msg_file, &msg)) {
        return NULL;
    }

    return msg.text;
}

// NOTE: Why this function does not use `background_description_get_text` under
// the hood like `background_description_get_name` do?
//
// 0x4C2530
const char* background_description_get_body(int num)
{
    MessageListItem msg;

    if (num < FIRST_DESCRIPTION_ID) {
        if (num != 0) {
            return NULL;
        }
        num = FIRST_DESCRIPTION_ID;
    }

    msg.num = num;
    if (!message_find(background_description_msg_file, &msg)) {
        return NULL;
    }

    char* pch = strchr(msg.text, '\n');
    if (pch != NULL) {
        return pch + 1;
    } else {
        return msg.text;
    }
}

// 0x4C2590
const char* background_description_get_name(int num)
{
    const char* text = background_description_get_text(num);
    strncpy(background_description_name, text, BACKGROUND_NAME_LENGTH);

    char* pch = strchr(background_description_name, '\n');
    if (pch != NULL) {
        *pch = '\0';
    } else {
        tig_debug_printf("Background.c: ERROR: Background text has no name!");
    }

    return background_description_name;
}

// 0x4C2650
void background_obj_clear(object_id_t obj)
{
    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 0);
    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 0);
    sub_4EA2E0(obj, 1);
}

// 0x4C2690
int background_obj_get_background(object_id_t obj)
{
    if (obj != OBJ_HANDLE_NULL) {
        if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
            return obj_field_int32_get(obj, OBJ_F_PC_BACKGROUND);
        }
    }

    return 0;
}

// 0x4C26D0
int background_obj_get_background_text(object_id_t obj)
{
    return obj_field_int32_get(obj, OBJ_F_PC_BACKGROUND_TEXT);
}

// 0x4C2A70
static int background_get_base_money()
{
    MessageListItem msg;
    msg.num = BACKGROUND_FIELD_MONEY;
    sub_4D43A0(background_msg_file, &msg);
    return atoi(msg.text);
}

// 0x4C2AA0
int background_adjust_money(int amount, int background)
{
    MessageListItem msg;
    msg.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_FIELD_MONEY;
    sub_4D43A0(background_msg_file, &msg);
    return amount * (100 * atoi(msg.text) / background_get_base_money()) / 100;
}

// 0x4C2B10
bool background_get_items(char* dest, size_t size, int background)
{
    MessageListItem msg;
    msg.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_FIELD_ITEMS;
    strncpy(dest, msg.text, size);
    return true;
}
