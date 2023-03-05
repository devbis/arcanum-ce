#ifndef ARCANUM_TIG_DXINPUT_H_
#define ARCANUM_TIG_DXINPUT_H_

#include "tig/types.h"

#define DIRECTINPUT_VERSION 0x300
#include <dinput.h>

int tig_dxinput_init(TigContext* ctx);
void tig_dxinput_exit();
int tig_dxinput_get_instance(LPDIRECTINPUTA* out_direct_input);

#endif /* ARCANUM_TIG_DXINPUT_H_ */
