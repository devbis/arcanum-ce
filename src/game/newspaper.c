#include "game/newspaper.h"

#include "game/mes.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/proto.h"
#include "game/random.h"
#include "game/ui.h"

#define QUEUE_SIZE 25
#define GENERIC_NEWSPAPER_NUM 5000

static bool is_in_queue(int num);
static void rotate_newspapers();
static int pick_random_generic_newspaper();

// 0x6876CC
static int newspaper_queue_size;

// 0x6876D0
static int* newspaper_queue;

// 0x4BF060
bool newspaper_init(GameInitInfo* init_info)
{
    (void)init_info;

    newspaper_queue = (int*)CALLOC(QUEUE_SIZE, sizeof(*newspaper_queue));
    newspaper_reset();

    return true;
}

// 0x4BF080
void newspaper_reset()
{
    int index;

    for (index = 0; index < NEWSPAPER_CURRENT; index++) {
        newspaper_queue[index] = -1;
    }

    newspaper_queue[NEWSPAPER_CURRENT] = GENERIC_NEWSPAPER_NUM;
    newspaper_queue_size = NEWSPAPER_CURRENT + 1;
}

// 0x4BF0B0
void newspaper_exit()
{
    FREE(newspaper_queue);
}

// 0x4BF0C0
bool newspaper_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(newspaper_queue, sizeof(*newspaper_queue) * QUEUE_SIZE, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&newspaper_queue_size, sizeof(newspaper_queue_size), 1, load_info->stream) != 1) return false;

    return true;
}

// 0x4BF100
bool newspaper_save(TigFile* stream)
{
    if (tig_file_fwrite(newspaper_queue, sizeof(*newspaper_queue) * QUEUE_SIZE, 1, stream) != 1) return false;
    if (tig_file_fwrite(&newspaper_queue_size, sizeof(newspaper_queue_size), 1, stream) != 1) return false;

    return true;
}

// 0x4BF140
void newspaper_enqueue(int num, bool priority)
{
    int index;

    if (num >= GENERIC_NEWSPAPER_NUM || !is_in_queue(num)) {
        if (priority) {
            for (index = QUEUE_SIZE - 1; index > NEWSPAPER_CURRENT; index--) {
                newspaper_queue[index] = newspaper_queue[index - 1];
            }

            newspaper_queue[NEWSPAPER_CURRENT + 1] = num;

            if (newspaper_queue_size < QUEUE_SIZE) {
                newspaper_queue_size++;
            }
        } else {
            if (newspaper_queue_size < QUEUE_SIZE) {
                newspaper_queue[newspaper_queue_size++] = num;
            } else {
                newspaper_queue[QUEUE_SIZE - 1] = num;
            }
        }
    }
}

// 0x4BF1D0
bool is_in_queue(int num)
{
    int index;

    for (index = 0; index < QUEUE_SIZE; index++) {
        if (newspaper_queue[index] == num) {
            return true;
        }
    }

    return false;
}

// 0x4BF200
int newspaper_get(int index)
{
    return newspaper_queue[index];
}

// 0x4BF210
int64_t newspaper_create(int start, int64_t loc)
{
    int64_t prototype_obj;
    int64_t obj;

    prototype_obj = sub_4685A0(14064);
    if (!object_create(prototype_obj, loc, &obj)) {
        return OBJ_HANDLE_NULL;
    }

    obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, start);

    return obj;
}

// 0x4BF270
bool newspaper_timeevent_process(TimeEvent* timeevent)
{
    TimeEvent next_timeevent;
    DateTime datetime;

    (void)timeevent;

    if (tig_net_is_active() && !tig_net_is_host()) {
        return true;
    }

    rotate_newspapers();

    next_timeevent.type = TIMEEVENT_TYPE_NEWSPAPERS;
    sub_45A950(&datetime, 86400000);
    sub_45B800(&next_timeevent, &datetime);

    return true;
}

// 0x4BF2C0
void rotate_newspapers()
{
    int index;

    index = newspaper_queue[NEWSPAPER_CURRENT] < GENERIC_NEWSPAPER_NUM ? 0 : NEWSPAPER_CURRENT;

    newspaper_queue_size--;

    while (index < newspaper_queue_size) {
        newspaper_queue[index] = newspaper_queue[index + 1];
        index++;
    }

    if (newspaper_queue_size < 5) {
        newspaper_queue[NEWSPAPER_CURRENT] = pick_random_generic_newspaper();
        newspaper_queue_size = NEWSPAPER_CURRENT + 1;
    }
}

// 0x4BF330
int pick_random_generic_newspaper()
{
    int cnt = 0;
    int num = GENERIC_NEWSPAPER_NUM;
    char str[MAX_STRING];

    do {
        cnt++;
        num += 10;
        sub_460800(num, str);
    } while (str[0] != '\0');

    return GENERIC_NEWSPAPER_NUM + random_between(0, cnt - 1) * 10;
}
