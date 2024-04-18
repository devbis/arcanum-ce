#ifndef TIG_BMP_UTILS_H_
#define TIG_BMP_UTILS_H_

// BMP UTILS
// ---
//
// The BMP UTILS module provides a set of utility functions to convert 24-bit
// BGR surface to 8-bit palette-indexed surface.
//
// NOTES
//
// - The name is wrong. Given it's location in the binary the name should
// start with `D`.
//
// - These functions are never used in Arcanum. They are referenced inside
// screenshotting routine in VIDEO module, but the calling code never sets
// appropriate flag to activate proper code path. ToEE has not been checked.
//
// - The implementation is rather obscure. I decided not to invest time into
// deep understanding of the math behind computations.

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void sub_53A2A0(uint8_t* pixels, int size, int a3);
void sub_53A310();
void sub_53A390(uint8_t* palette);
void sub_53A3C0();
int sub_53A4D0(int b, int g, int r);
void sub_53A8B0();

#ifdef __cplusplus
}
#endif

#endif /* TIG_BMP_UTILS_H_ */
