#include "tig/str_parse.h"

#include <ctype.h>

#define DEFAULT_LIST_SEPARATOR ','

// 0x5C22AC
static char tig_str_parse_separator = DEFAULT_LIST_SEPARATOR;

// 0x5318C0
int tig_str_parse_init(TigContext* ctx)
{
    tig_str_parse_separator = DEFAULT_LIST_SEPARATOR;
    return TIG_OK;
}

// 0x5318D0
void tig_str_parse_exit()
{
}

// 0x5318E0
void tig_str_parse_set_separator(char sep)
{
    tig_str_parse_separator = sep;
}

// 0x5318F0
void tig_str_parse_simple_str_value(char** str, char* value)
{
    while (isspace(*(*str))) {
        (*str)++;
    }

    char* sep = strchr(*str, tig_str_parse_separator);
    if (sep != NULL) {
        *sep = '\0';
    }

    memcpy(value, *str, strlen(*str) + 1);

    if (sep != NULL) {
        *sep = tig_str_parse_separator;
        *str = sep + 1;
    } else {
        *str = NULL;
    }
}

// 0x531990
void tig_str_parse_simple_value(char** str, int* value)
{
    while (isspace(*(*str))) {
        (*str)++;
    }

    char* sep = strchr(*str, tig_str_parse_separator);
    if (sep != NULL) {
        *sep = '\0';
    }

    *value = atoi(*str);

    if (sep != NULL) {
        *sep = tig_str_parse_separator;
        *str = sep + 1;
    } else {
        *str = NULL;
    }
}
