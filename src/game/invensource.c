#include "game/invensource.h"

#include <stdio.h>

#include "game/mes.h"
#include "game/obj_private.h"
#include "game/proto.h"

#define MAX_INVEN_SOURCE_SET_NAME 70

typedef struct NamedInvenSourceSet {
    /* 0000 */ char name[MAX_INVEN_SOURCE_SET_NAME];
    /* 0048 */ InvenSourceSet set;
} NamedInvenSourceSet;

static_assert(sizeof(NamedInvenSourceSet) == 0xE6C, "wrong size");

static bool parse_set_data(mes_file_handle_t invensource_mes_file, mes_file_handle_t invensourcebuy_mes_file);
static bool sub_4BF7B0(MesFileEntry* mes_file_entry, char* str);
static bool sub_4BFAA0(MesFileEntry* mes_file_entry, char* str);
static void show_error(const char* msg);

// 0x5B64A8
static const char* off_5B64A8 = "Rules\\InvenSource.mes";

// 0x5B64AC
static const char* off_5B64AC = "Rules\\InvenSourceBuy.mes";

// 0x5FC520
static NamedInvenSourceSet* invensource_sets;

// 0x5FC524
static int invensource_num_sets;

// 0x5FC528
static bool invensource_editor;

// 0x5FC52C
static char invensource_error[256];

// 0x5FC62C
static bool dword_5FC62C;

// 0x5FC630
static bool invensource_initialized;

// 0x5FC634
static bool invensource_have_buy;

// 0x4BF390
bool invensource_init(GameInitInfo* init_info)
{
    mes_file_handle_t invensource_mes_file;
    mes_file_handle_t invensourcebuy_mes_file;

    if (!invensource_initialized) {
        invensource_editor = init_info->editor;

        if (!mes_load(off_5B64A8, &invensource_mes_file)) {
            sprintf(invensource_error, "Can't load message file [%s].", off_5B64A8);
            show_error(invensource_error);
            return false;
        }

        if (mes_load(off_5B64AC, &invensourcebuy_mes_file)) {
            invensource_have_buy = true;
        } else {
            invensource_have_buy = false;
            sprintf(invensource_error, "Can't load message file [%s].", off_5B64AC);
            show_error(invensource_error);
        }

        invensource_num_sets = mes_entries_count(invensource_mes_file);
        if (invensource_num_sets < 1) {
            sprintf(invensource_error, "No sets to parse in [%s].", off_5B64A8);
            mes_unload(invensource_mes_file);
            mes_unload(invensourcebuy_mes_file);
            return false;
        }

        invensource_sets = MALLOC(sizeof(*invensource_sets) * invensource_num_sets);
        if (!parse_set_data(invensource_mes_file, invensourcebuy_mes_file)) {
            // FIXME: Leaking `invensource_sets`.
            mes_unload(invensource_mes_file);
            mes_unload(invensourcebuy_mes_file);
            return false;
        }

        mes_unload(invensource_mes_file);
        mes_unload(invensourcebuy_mes_file);
        invensource_have_buy = false;

        invensource_initialized = true;
    }

    return true;
}

// 0x4BF510
void invensource_exit()
{
    if (invensource_initialized) {
        FREE(invensource_sets);
        invensource_initialized = false;
    }
}

// 0x4BF5D0
void invensource_get_id_list(int id, InvenSourceSet* set)
{
    if (invensource_initialized) {
        if (id > 0 && id < invensource_num_sets) {
            *set = invensource_sets[id - 1].set;
            return;
        }

        tig_debug_printf("Range error in invensource_get_id_list, set: %d\n", id);
    }

    set->cnt = 0;
    set->buy_cnt = 0;
}

// 0x4BF640
bool parse_set_data(mes_file_handle_t invensource_mes_file, mes_file_handle_t invensourcebuy_mes_file)
{
    MesFileEntry mes_file_entry1;
    MesFileEntry mes_file_entry2;
    char str[MAX_STRING];
    int index;
    NamedInvenSourceSet* named_set;

    index = 0;
    mes_file_entry1.num = 0;
    while (mes_find_next(invensource_mes_file, &mes_file_entry1)) {
        if (mes_file_entry1.num != ++index) {
            sprintf(invensource_error, "[%s] discontinuous at line: %d.  Skipping remaining sets.", off_5B64A8, mes_file_entry1.num);
            show_error(invensource_error);
            return true;
        }

        named_set = &(invensource_sets[mes_file_entry1.num - 1]);
        if (sub_4BF7B0(&mes_file_entry1, str)) {
            named_set->set.buy_cnt = 0;
            if (invensource_have_buy) {
                mes_file_entry2.num = mes_file_entry1.num;
                if (mes_search(invensourcebuy_mes_file, &mes_file_entry2)) {
                    mes_get_msg(invensourcebuy_mes_file, &mes_file_entry2);
                    if (!sub_4BFAA0(&mes_file_entry2, str)) {
                        sprintf(invensource_error,
                            "Error parsing [%s] line %d.  Emptying set.",
                            off_5B64AC,
                            mes_file_entry1.num);
                        show_error(invensource_error);
                        named_set->set.cnt = 0;
                        named_set->set.buy_cnt = 0;
                    }
                }
            }
        } else {
            sprintf(invensource_error,
                "Error parsing [%s] line %d.  Emptying set.",
                off_5B64A8,
                mes_file_entry1.num);
            show_error(invensource_error);
            named_set->set.cnt = 0;
            named_set->set.buy_cnt = 0;
        }
    }

    if (index != invensource_num_sets) {
        show_error("Set count is off in InvenSource, parse_set_data.");
    }

    return true;
}

// 0x4BF7B0
bool sub_4BF7B0(MesFileEntry* mes_file_entry, char* str)
{
    NamedInvenSourceSet* named_set;
    char* tok;
    int cnt;
    int rate;
    int basic_prototype;
    bool is_first;
    int64_t obj;

    if (strlen(mes_file_entry->str) >= MAX_STRING) {
        sprintf(invensource_error,
            "Line %d in [%s] exceeds the maximum line length for message files.",
            mes_file_entry->num,
            off_5B64A8);
        show_error(invensource_error);
        return false;
    }

    strcpy(str, mes_file_entry->str);
    named_set = &(invensource_sets[mes_file_entry->num - 1]);

    tok = strtok(str, ":");
    if (tok == NULL) {
        sprintf(invensource_error, "Line %d in [%s] is empty.\n", mes_file_entry->num, off_5B64A8);
        show_error(invensource_error);
        return false;
    }

    if (strlen(tok) >= MAX_INVEN_SOURCE_SET_NAME) {
        sprintf(invensource_error,
            "Line %d in [%s] has too long of a name.\n  Name length should be kept under %d characters.\n",
            mes_file_entry->num,
            off_5B64A8,
            MAX_INVEN_SOURCE_SET_NAME);
        show_error(invensource_error);
        return false;
    }

    strcpy(named_set->name, tok);

    tok = strtok(NULL, ",");
    if (tok == NULL) {
        named_set->set.cnt = 0;
        named_set->set.min_coins = 0;
        named_set->set.max_coins = 0;
        return true;
    }

    cnt = 0;
    is_first = true;
    while (tok != NULL) {
        if (cnt >= INVEN_SOURCE_SET_SIZE) {
            sprintf(invensource_error,
                "Line %d in [%s] has too many items.\n  Item total should not exceed %d.\n",
                mes_file_entry->num,
                off_5B64A8,
                INVEN_SOURCE_SET_SIZE);
            show_error(invensource_error);
            return false;
        }

        rate = atoi(tok);

        tok = strtok(NULL, " ");
        if (tok == NULL) {
            sprintf(invensource_error,
                "Error: Line %d in [%s] has an incomplete rate,bp pair at entry %d.\n",
                mes_file_entry->num,
                off_5B64A8,
                cnt + 1);
            show_error(invensource_error);
            return false;
        }

        basic_prototype = atoi(tok);

        if (is_first) {
            named_set->set.min_coins = rate;
            named_set->set.max_coins = basic_prototype;
            is_first = false;
        } else {
            if (!proto_is_valid(basic_prototype)) {
                sprintf(invensource_error,
                    "Error: Invalid prototype in [%s]: set %d, entry %d, basic prototype %d\n",
                    off_5B64A8,
                    mes_file_entry->num,
                    cnt + 1,
                    basic_prototype);
                show_error(invensource_error);
                return false;
            }

            obj = sub_4685A0(basic_prototype);
            if (!obj_handle_is_valid(obj)) {
                sprintf(invensource_error,
                    "Error: Can't get valid handle for prototype in [%s]: set %d, entry %d, basic prototype %d\n",
                    off_5B64A8,
                    mes_file_entry->num,
                    cnt + 1,
                    basic_prototype);
                show_error(invensource_error);
                return false;
            }

            named_set->set.rate[cnt] = rate;
            named_set->set.basic_prototype[cnt] = basic_prototype;
            cnt++;
        }

        tok = strtok(NULL, ",");
    }

    named_set->set.cnt = cnt;
    return true;
}

// 0x4BFAA0
bool sub_4BFAA0(MesFileEntry* mes_file_entry, char* str)
{
    NamedInvenSourceSet* named_set;
    char* tok;
    int cnt;
    int basic_prototype;
    int64_t obj;

    if (strlen(mes_file_entry->str) >= MAX_STRING) {
        sprintf(invensource_error,
            "Line %d in [%s] exceeds the maximum line length for message files.",
            mes_file_entry->num,
            off_5B64AC);
        show_error(invensource_error);
        return false;
    }

    strcpy(str, mes_file_entry->str);
    named_set = &(invensource_sets[mes_file_entry->num - 1]);

    tok = strtok(str, " ");
    if (tok == NULL) {
        named_set->set.buy_all = false;
        named_set->set.buy_cnt = 0;
        return true;
    }

    if (strcmp(strupr(tok), "ALL") == 0) {
        named_set->set.buy_all = true;
        named_set->set.buy_cnt = 0;
        return true;
    }

    named_set->set.buy_all = false;

    cnt = 0;
    while (tok != NULL) {
        if (cnt >= INVEN_SOURCE_SET_SIZE) {
            sprintf(invensource_error,
                "Line %d in [%s] has too many items.\n  Item total should not exceed %d.\n",
                mes_file_entry->num,
                off_5B64AC,
                INVEN_SOURCE_SET_SIZE);
            show_error(invensource_error);
            return false;
        }

        basic_prototype = atoi(tok);
        if (!proto_is_valid(basic_prototype)) {
            sprintf(invensource_error,
              "Error: Invalid prototype in [%s]: set %d, entry %d, basic prototype %d\n",
              off_5B64AC,
              mes_file_entry->num,
              cnt + 1,
              basic_prototype);
            show_error(invensource_error);
            return false;
        }

        obj = sub_4685A0(basic_prototype);
        if (!obj_handle_is_valid(obj)) {
            sprintf(invensource_error,
              "Error: Can't get valid handle for prototype in [%s]: set %d, entry %d, basic prototype %d\n",
              off_5B64AC,
              mes_file_entry->num,
              cnt + 1,
              basic_prototype);
            show_error(invensource_error);
            return false;
        }

        named_set->set.buy_basic_prototype[cnt] = basic_prototype;

        cnt++;
        tok = strtok(NULL, " ");
    }

    named_set->set.buy_cnt = cnt;
    return true;
}

// 0x4BFCA0
void show_error(const char* msg)
{
    tig_debug_println(msg);
    if (!dword_5FC62C && invensource_editor) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", msg, NULL);
    }
}
