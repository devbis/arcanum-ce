#ifndef TIG_NET_UTILS_H_
#define TIG_NET_UTILS_H_

// NET UTILS
// ---
//
// Some network-related utility functions. The purpose is not very clear right
// now.
//
// The module name is obviously wrong. Given it's location in the binary the
// name should start with D.

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool sub_53AB00();
void sub_53AB20();
bool sub_53AB90(const char* a1, const char* a2);
bool sub_53ABF0(int a1, const char* a2);

#ifdef __cplusplus
}
#endif

#endif /* TIG_NET_UTILS_H_ */
