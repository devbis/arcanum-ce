#include "tig/dxinput.h"

// 0x630D58
static LPDIRECTINPUTA direct_input;

// 0x537A80
int tig_dxinput_init(TigContext* ctx)
{
    if (direct_input != NULL) {
        return TIG_ALREADY_INITIALIZED;
    }

    if (FAILED(DirectInputCreateA(ctx->instance, DIRECTINPUT_VERSION, &direct_input, NULL))) {
        return TIG_ERR_7;
    }

    return TIG_OK;
}

// 0x537AC0
void tig_dxinput_exit()
{
    if (direct_input != NULL) {
        IDirectInput_Release(direct_input);
        direct_input = NULL;
    }
}

// 0x537AE0
int tig_dxinput_get_instance(LPDIRECTINPUTA* out_direct_input)
{
    if (direct_input == NULL) {
        return TIG_ERR_7;
    }

    *out_direct_input = direct_input;

    return TIG_OK;
}
