#include "game/newspaper.h"

#include "game/mes.h"
#include "game/random.h"

#define TWENTY_FIVE 25
#define GENERIC_NEWSPAPER_NUM 5000

static void sub_4BF2C0();
static int sub_4BF330();

// 0x6876CC
static int dword_6876CC;

// 0x6876D0
static int* off_6876D0;

// 0x4BF060
bool newspaper_init(GameContext* init_info)
{
    (void)init_info;

    off_6876D0 = (int*)CALLOC(TWENTY_FIVE, sizeof(*off_6876D0));
    newspaper_reset();

    return true;
}

// 0x4BF080
void newspaper_reset()
{
    int index;

    for (index = 0; index < 4; index++) {
        off_6876D0[index] = -1;
    }

    off_6876D0[4] = 5000;
    dword_6876CC = 5;
}

// 0x4BF0B0
void newspaper_exit()
{
    FREE(off_6876D0);
}

// 0x4BF0C0
bool newspaper_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(off_6876D0, sizeof(TWENTY_FIVE) * sizeof(*off_6876D0), 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_6876CC, sizeof(dword_6876CC), 1, load_info->stream) != 1) return false;

    return true;
}

// 0x4BF100
bool newspaper_save(TigFile* stream)
{
    if (tig_file_fwrite(off_6876D0, sizeof(TWENTY_FIVE) * sizeof(*off_6876D0), 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_6876CC, sizeof(dword_6876CC), 1, stream) != 1) return false;

    return true;
}

// 0x4BF140
void newspaper_queue(int num, bool priority)
{
    int index;

    if (num >= GENERIC_NEWSPAPER_NUM || !newspaper_is_queued(num)) {
        if (priority) {
            for (index = TWENTY_FIVE - 1; index > 4; index--) {
                off_6876D0[index] = off_6876D0[index - 1];
            }

            off_6876D0[5] = num;

            if (dword_6876CC < TWENTY_FIVE) {
                dword_6876CC++;
            }
        } else {
            if (dword_6876CC < TWENTY_FIVE) {
                off_6876D0[dword_6876CC++] = num;
            } else {
                off_6876D0[TWENTY_FIVE - 1] = num;
            }
        }
    }
}

// 0x4BF1D0
bool newspaper_is_queued(int num)
{
    int index;

    for (index = 0; index < TWENTY_FIVE; index++) {
        if (off_6876D0[index] == num) {
            return true;
        }
    }

    return false;
}

// 0x4BF200
int sub_4BF200(int index)
{
    return off_6876D0[index];
}

// 0x4BF210
object_id_t sub_4BF210(int start, int64_t a2)
{
    object_id_t obj;

    if (!object_create(sub_4685A0(14064), a2, &obj)) {
        return OBJ_HANDLE_NULL;
    }

    obj_f_set_int32(obj, OBJ_F_WRITTEN_TEXT_START_LINE, start);

    return obj;
}

// 0x4BF270
bool newspaper_timeevent_process(TimeEvent* timeevent)
{
    TimeEvent next_timeevent;
    DateTime datetime;

    (void)timeevent;

    if ((tig_net_flags & 0x1) == 0 || (tig_net_flags & 0x2) != 0) {
        sub_4BF2C0();

        next_timeevent.type = TIMEEVENT_TYPE_NEWSPAPERS;
        sub_45A950(&datetime, 86400000);
        sub_45B800(&next_timeevent, &datetime);
    }

    return true;
}

// 0x4BF2C0
void sub_4BF2C0()
{
    int index;

    index = off_6876D0[4] < GENERIC_NEWSPAPER_NUM ? 0 : 4;
    while (index < dword_6876CC - 1) {
        off_6876D0[index] = off_6876D0[index + 1];
        index++;
    }

    if (dword_6876CC < 5) {
        off_6876D0[4] = sub_4BF330();
        dword_6876CC = 5;
    }
}

// 0x4BF330
int sub_4BF330()
{
    int cnt = 0;
    int v2 = GENERIC_NEWSPAPER_NUM;
    char str[MAX_STRING];

    do {
        cnt++;
        v2 += 10;
        sub_460800(v2, str);
    } while (str[0] != '\0');

    return 10 * (random_between(0, cnt - 1) + 500);
}
