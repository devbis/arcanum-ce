#include "tig/dxinput.h"

// 0x630D58
static LPDIRECTINPUTA direct_input;

/// This unit manages shared DirectInput instance. However modern Windows SDK
/// does not include `dinput` library so we have to use dynamic loading to
/// provide it's create function.
#ifndef HAVE_DINPUT
typedef HRESULT(__stdcall* DIRECTINPUTCREATEA)(HINSTANCE, DWORD, LPDIRECTINPUTA*, LPUNKNOWN);

static HMODULE dinput_dll;
static DIRECTINPUTCREATEA direct_input_create;

/// Redefine `DirectInputCreateA` that is declared in `dinput.h` (and is
/// correctly linked when compiling in VC6) to be a mere function pointer which
/// will be provided during initialization.
#define DirectInputCreateA direct_input_create
#endif /* HAVE_DINPUT */

// 0x537A80
int tig_dxinput_init(TigContext* ctx)
{
    HRESULT hr;

    if (direct_input != NULL) {
        return TIG_ALREADY_INITIALIZED;
    }

#ifndef HAVE_DINPUT
    dinput_dll = LoadLibraryA("dinput.dll");
    if (dinput_dll == NULL) {
        return TIG_ERR_7;
    }

    DirectInputCreateA = (DIRECTINPUTCREATEA)GetProcAddress(dinput_dll, "DirectInputCreateA");
    if (DirectInputCreateA == NULL) {
        FreeLibrary(dinput_dll);
        dinput_dll = NULL;
        return TIG_ERR_7;
    }
#endif /* HAVE_DINPUT */

    hr = DirectInputCreateA(ctx->instance, DIRECTINPUT_VERSION, &direct_input, NULL);
    if (FAILED(hr)) {
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

#ifndef HAVE_DINPUT
    if (dinput_dll != NULL) {
        FreeLibrary(dinput_dll);
        dinput_dll = NULL;

        DirectInputCreateA = NULL;
    }
#endif /* HAVE_DINPUT */
}

// 0x537AE0
int tig_dxinput_get_instance(LPDIRECTINPUTA* direct_input_ptr)
{
    if (direct_input == NULL) {
        return TIG_ERR_7;
    }

    *direct_input_ptr = direct_input;

    return TIG_OK;
}
