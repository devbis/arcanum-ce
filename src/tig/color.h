#ifndef ARCANUM_TIG_COLOR_H_
#define ARCANUM_TIG_COLOR_H_

#include "tig/types.h"

extern uint8_t* dword_739E88;
extern unsigned int dword_739E8C;
extern unsigned int tig_color_blue_mask;
extern unsigned int dword_739E94;
extern uint8_t* dword_739E98;
extern unsigned int dword_739E9C;
extern uint8_t* dword_739EA0;
extern uint8_t* dword_739EA4;
extern uint8_t* dword_739EA8;
extern unsigned int tig_color_blue_shift;
extern unsigned int dword_739EB0;
extern unsigned int dword_739EB4;
extern unsigned int tig_color_green_mask;
extern uint8_t* dword_739EBC;
extern uint8_t* dword_739EC0;
extern unsigned int dword_739EC4;
extern unsigned int dword_739EC8;
extern uint8_t* dword_739ECC;
extern unsigned int dword_739ED0;
extern unsigned int dword_739ED4;
extern uint8_t* dword_739ED8;
extern uint8_t* dword_739EDC;
extern unsigned int dword_739EE0;
extern unsigned int dword_739EE4;
extern unsigned int dword_739EE8;
extern unsigned int tig_color_red_shift;
extern unsigned int tig_color_green_shift;
extern unsigned int dword_739EF4;
extern unsigned int dword_739EF8;
extern unsigned int dword_739EFC;
extern unsigned int tig_color_red_mask;
extern unsigned int dword_739F04;
extern unsigned int dword_739F08;
extern unsigned int dword_739F0C;
extern unsigned int dword_739F10;
extern unsigned int dword_739F14;
extern uint8_t* dword_739F18;
extern unsigned int dword_739F1C;
extern uint8_t* dword_739F20;
extern unsigned int dword_739F24;
extern uint8_t* dword_739F28;
extern uint8_t* dword_739F2C;
extern uint8_t* dword_739F30;

int tig_color_init(TigContext* ctx);
void tig_color_exit();
int tig_color_set_rgb_settings(unsigned int red_mask, unsigned int green_mask, unsigned int blue_mask);
int tig_color_set_rgba_settings(unsigned int red_mask, unsigned int green_mask, unsigned int blue_mask, unsigned int alpha_mask);
void tig_color_get_masks(unsigned int* red_mask, unsigned int* green_mask, unsigned int* blue_mask);
void tig_color_get_shifts(unsigned int* red_shift, unsigned int* green_shift, unsigned int* blue_shift);
int tig_color_get_red(int a1);
int tig_color_get_green(int a1);
int tig_color_get_blue(int a1);
int sub_52C370(int a1);
int sub_52C500(int a1);
int sub_52C520(int a1);
int sub_52C540(int a1);
int sub_52C560(unsigned int a1);
int sub_52C610(int red, int green, int blue);

// TODO: Not sure about the name.
static inline int tig_color_rgb_make(int red, int green, int blue)
{
    return (dword_739F2C[red] << tig_color_red_shift) | (dword_739F28[green] << tig_color_green_shift) | (dword_739E98[blue] << tig_color_blue_shift);
}

#endif /* ARCANUM_TIG_COLOR_H_ */
