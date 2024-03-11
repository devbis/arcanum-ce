#ifndef ARCANUM_TIG_DXINPUT_H_
#define ARCANUM_TIG_DXINPUT_H_

#define DIRECTINPUT_VERSION 0x300
#include <dinput.h>

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

int tig_dxinput_init(TigContext* ctx);
void tig_dxinput_exit();
int tig_dxinput_get_instance(LPDIRECTINPUTA* direct_input_ptr);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_DXINPUT_H_ */
