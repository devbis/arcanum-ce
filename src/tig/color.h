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
extern uint8_t* tig_color_blue_grayscale_table;
extern unsigned int tig_color_rgba_green_range;
extern uint8_t* tig_color_red_grayscale_table;
extern unsigned int tig_color_rgba_blue_range_bit_length;
extern uint8_t* tig_color_green_index_table;
extern uint8_t* tig_color_red_index_table;
extern uint8_t* tig_color_green_grayscale_table;

int tig_color_init(TigInitializeInfo* init_info);
void tig_color_exit();
int tig_color_set_rgb_settings(unsigned int red_mask, unsigned int green_mask, unsigned int blue_mask);
int tig_color_set_rgba_settings(unsigned int red_mask, unsigned int green_mask, unsigned int blue_mask, unsigned int alpha_mask);
void tig_color_get_masks(unsigned int* red_mask, unsigned int* green_mask, unsigned int* blue_mask);
void tig_color_get_shifts(unsigned int* red_shift, unsigned int* green_shift, unsigned int* blue_shift);
int tig_color_get_red(color_t color);
int tig_color_get_green(color_t color);
int tig_color_get_blue(color_t color);
unsigned int sub_52C370(color_t color);
int sub_52C500(color_t color);
int sub_52C520(color_t color);
int sub_52C540(color_t color);
unsigned int tig_color_index_of(color_t color);
unsigned int tig_color_to_24_bpp(int red, int green, int blue);

// TODO: Not sure about the name.
static inline unsigned int tig_color_rgb_make(int red, int green, int blue)
{
    return (tig_color_red_index_table[red] << tig_color_red_shift) | (tig_color_green_index_table[green] << tig_color_green_shift) | (tig_color_blue_index_table[blue] << tig_color_blue_shift);
}

static inline unsigned int tig_color_16_to_32(uint32_t color)
{
    unsigned int red = (color & tig_color_red_mask) >> tig_color_red_shift;
    unsigned int green = (color & tig_color_green_mask) >> tig_color_green_shift;
    unsigned int blue = (color & tig_color_blue_mask) >>tig_color_blue_shift;
    return tig_color_rgba_alpha_mask
        | ((tig_color_rgba_red_table[red]) << tig_color_rgba_red_shift)
        | ((tig_color_rgba_green_table[green]) << tig_color_rgba_green_shift)
        | ((tig_color_rgba_blue_table[blue]) << tig_color_rgba_blue_shift);
}

static inline unsigned int tig_color_make_3(uint32_t color1, uint32_t color2)
{
    unsigned int red1 = (color1 & tig_color_red_mask) >> tig_color_red_shift;
    unsigned int green1 = (color1 & tig_color_green_mask) >> tig_color_green_shift;
    unsigned int blue1 = (color1 & tig_color_blue_mask) >> tig_color_blue_shift;
    unsigned int red2 = (color2 & tig_color_red_mask) >> tig_color_red_shift;
    unsigned int green2 = (color2 & tig_color_green_mask) >> tig_color_green_shift;
    unsigned int blue2 = (color2 & tig_color_blue_mask) >> tig_color_blue_shift;

    return (tig_color_red_intensity_table[(tig_color_red_range + 1) * red1 + red2] << tig_color_red_shift)
        | (tig_color_green_intensity_table[(tig_color_green_range + 1) * green1 + green2] << tig_color_green_shift)
        | (tig_color_blue_intensity_table[(tig_color_blue_range + 1) * blue1 + blue2] << tig_color_blue_shift);
}

static inline unsigned int tig_color_make_4(uint32_t color1, uint32_t color2)
{
    unsigned int red1 = color1 & tig_color_red_mask;
    unsigned int green1 = color1 & tig_color_green_mask;
    unsigned int blue1 = color1 & tig_color_blue_mask;
    unsigned int red2 = color2 & tig_color_red_mask;
    unsigned int green2 = color2 & tig_color_green_mask;
    unsigned int blue2 = color2 & tig_color_blue_mask;
    return min(red1 + red2, tig_color_red_mask)
        | min(green1 + green2, tig_color_green_mask)
        | min(blue1 + blue2, tig_color_blue_mask);
}

static inline unsigned int tig_color_make_5(uint32_t color1, uint32_t color2)
{
    unsigned int red1 = (color1 & tig_color_red_mask) >> tig_color_red_shift;
    unsigned int green1 = (color1 & tig_color_green_mask) >> tig_color_green_shift;
    unsigned int blue1 = (color1 & tig_color_blue_mask) >> tig_color_blue_shift;
    unsigned int red2 = (color2 & tig_color_red_mask) >> tig_color_red_shift;
    unsigned int green2 = (color2 & tig_color_green_mask) >> tig_color_green_shift;
    unsigned int blue2 = (color2 & tig_color_blue_mask) >> tig_color_blue_shift;

    return (tig_color_red_intensity_table[(tig_color_red_range + 1) * red2 + red1] << tig_color_red_shift)
        | (tig_color_green_intensity_table[(tig_color_green_range + 1) * green2 + green1] << tig_color_green_shift)
        | (tig_color_blue_intensity_table[(tig_color_blue_range + 1) * blue2 + blue1] << tig_color_blue_shift);
}

static inline unsigned int tig_color_make_6(uint32_t color1, uint32_t color2)
{
    unsigned int red1 = (color1 & tig_color_red_mask);
    unsigned int green1 = (color1 & tig_color_green_mask);
    unsigned int blue1 = (color1 & tig_color_blue_mask);
    unsigned int red2 = (color2 & tig_color_red_mask);
    unsigned int green2 = (color2 & tig_color_green_mask);
    unsigned int blue2 = (color2 & tig_color_blue_mask);
    unsigned int luminosity = tig_color_red_grayscale_table[red1 >> tig_color_red_shift]
        + tig_color_green_grayscale_table[green1 >> tig_color_green_shift]
        + tig_color_blue_grayscale_table[blue1 >> tig_color_blue_shift];

    return ((red2 + ((luminosity * (red1 - red2)) >> 8)) & tig_color_red_mask)
        | ((green2 + ((luminosity * (green1 - green2)) >> 8)) & tig_color_green_mask)
        | ((blue2 + ((luminosity * (blue1 - blue2)) >> 8)) & tig_color_blue_mask);
}

static inline unsigned int tig_color_make_7(uint32_t color1, uint32_t color2, unsigned int intensity)
{
    unsigned int red1 = (color1 & tig_color_red_mask);
    unsigned int green1 = (color1 & tig_color_green_mask);
    unsigned int blue1 = (color1 & tig_color_blue_mask);
    unsigned int red2 = (color2 & tig_color_red_mask);
    unsigned int green2 = (color2 & tig_color_green_mask);
    unsigned int blue2 = (color2 & tig_color_blue_mask);

    return ((red2 + ((intensity * (red1 - red2)) >> 8)) & tig_color_red_mask)
        | ((green2 + ((intensity * (green1 - green2)) >> 8)) & tig_color_green_mask)
        | ((blue2 + ((intensity * (blue1 - blue2)) >> 8)) & tig_color_blue_mask);
}

static inline unsigned int tig_color_make_8(uint32_t color1, uint32_t color2)
{
    unsigned int red1 = (color1 & tig_color_red_mask);
    unsigned int green1 = (color1 & tig_color_green_mask);
    unsigned int blue1 = (color1 & tig_color_blue_mask);
    unsigned int red2 = (color2 & tig_color_red_mask);
    unsigned int green2 = (color2 & tig_color_green_mask);
    unsigned int blue2 = (color2 & tig_color_blue_mask);

    return ((red2 + ((tig_color_red_value_table[red1 >> tig_color_red_shift] * (red1 - red2)) >> 8)) & tig_color_red_mask)
        | ((green2 + ((tig_color_red_value_table[red1 >> tig_color_red_shift] * (green1 - green2)) >> 8)) & tig_color_green_mask)
        | ((blue2 + ((tig_color_red_value_table[red1 >> tig_color_red_shift] * (blue1 - blue2)) >> 8)) & tig_color_blue_mask);
}

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_COLOR_H_ */
