#ifndef ARCANUM_TIG_STR_PARSE_H_
#define ARCANUM_TIG_STR_PARSE_H_

#include "tig/types.h"

int tig_str_parse_init(TigContext* ctx);
void tig_str_parse_exit();
void tig_str_parse_set_separator(char sep);

#endif /* ARCANUM_TIG_STR_PARSE_H_ */
