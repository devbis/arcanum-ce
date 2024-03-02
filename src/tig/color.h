#ifndef ARCANUM_TIG_COLOR_H_
#define ARCANUM_TIG_COLOR_H_

#include <stdint.h>

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t color_t;

extern uint8_t* tig_color_green_intensity_table;
extern unsigned int tig_color_rgba_green_range_bit_length;
extern unsigned int tig_color_blue_mask;
extern unsigned int tig_color_green_range;
extern uint8_t* tig_color_blue_index_table;
extern unsigned int tig_color_rgba_alpha_range;
extern uint8_t* tig_color_red_value_table;
extern uint8_t* tig_color_blue_intensity_table;
extern uint8_t* tig_color_rgba_red_table;
extern unsigned int tig_color_blue_shift;
extern unsigned int tig_color_red_range_bit_length;
extern unsigned int tig_color_rgba_green_shift;
extern unsigned int tig_color_green_mask;
extern uint8_t* tig_color_blue_value_table;
extern uint8_t* tig_color_green_value_table;
extern unsigned int tig_color_rgba_red_shift;
extern unsigned int tig_color_rgba_blue_mask;
extern uint8_t* tig_color_rgba_blue_table;
extern unsigned int tig_color_blue_range;
extern unsigned int tig_color_rgba_red_range_bit_length;
extern uint8_t* tig_color_red_intensity_table;
extern uint8_t* tig_color_rgba_green_table;
extern unsigned int tig_color_rgba_alpha_mask;
extern unsigned int tig_color_rgba_blue_range;
extern unsigned int tig_color_rgba_alpha_shift;
extern unsigned int tig_color_red_shift;
extern unsigned int tig_color_green_shift;
extern unsigned int tig_color_blue_range_bit_length;
extern unsigned int tig_color_rgba_green_mask;
extern unsigned int tig_color_rgba_red_range;
extern unsigned int tig_color_red_mask;
extern unsigned int tig_color_rgba_blue_shift;
extern unsigned int tig_color_rgba_red_mask;
extern unsigned int tig_color_red_range;
extern unsigned int tig_color_rgba_alpha_range_bit_length;
extern unsigned int tig_color_green_range_bit_length;
extern uint8_t* tig_color_blue_luminosity_table;
extern unsigned int tig_color_rgba_green_range;
extern uint8_t* tig_color_red_luminosity_table;
extern unsigned int tig_color_rgba_blue_range_bit_length;
extern uint8_t* tig_color_green_index_table;
extern uint8_t* tig_color_red_index_table;
extern uint8_t* tig_color_green_luminosity_table;

int tig_color_init(TigContext* ctx);
void tig_color_exit();
int tig_color_set_rgb_settings(unsigned int red_mask, unsigned int green_mask, unsigned int blue_mask);
int tig_color_set_rgba_settings(unsigned int red_mask, unsigned int green_mask, unsigned int blue_mask, unsigned int alpha_mask);
void tig_color_get_masks(unsigned int* red_mask, unsigned int* green_mask, unsigned int* blue_mask);
void tig_color_get_shifts(unsigned int* red_shift, unsigned int* green_shift, unsigned int* blue_shift);
int tig_color_get_red(color_t color);
int tig_color_get_green(color_t color);
int tig_color_get_blue(color_t color);
int sub_52C370(color_t color);
int sub_52C500(color_t color);
int sub_52C520(color_t color);
int sub_52C540(color_t color);
unsigned int tig_color_index_of(color_t color);
unsigned int tig_color_to_24_bpp(int red, int green, int blue);

// TODO: Not sure about the name.
static inline int tig_color_rgb_make(int red, int green, int blue)
{
    return (tig_color_red_index_table[red] << tig_color_red_shift) | (tig_color_green_index_table[green] << tig_color_green_shift) | (tig_color_blue_index_table[blue] << tig_color_blue_shift);
}

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_COLOR_H_ */
