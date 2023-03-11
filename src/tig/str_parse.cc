#include "tig/str_parse.h"

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
