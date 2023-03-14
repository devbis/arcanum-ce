#ifndef ARCANUM_TIG_STR_PARSE_H_
#define ARCANUM_TIG_STR_PARSE_H_

#include "tig/types.h"

int tig_str_parse_init(TigContext* ctx);
void tig_str_parse_exit();
void tig_str_parse_set_separator(char sep);
void tig_str_parse_simple_str_value(char** str, char* value);
void tig_str_parse_simple_value(char** str, int* value);

#endif /* ARCANUM_TIG_STR_PARSE_H_ */
