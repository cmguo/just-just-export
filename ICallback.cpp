// IPpbox.cpp

#include "ppbox/ppbox/Common.h"
#define PPBOX_SOURCE
#include "ppbox/ppbox/ICallback.h"
#include "ppbox/ppbox/Callback.h"
using namespace ppbox::error;

#if __cplusplus
extern "C" {
#endif // __cplusplus

    PPBOX_DECL PP_err PPBOX_RedirectCallback(
        PPBOX_CallbackRedirect redirect, 
        PPBOX_CallbackFree free, 
        PP_context user_data)
    {
        ppbox::varg_call().set(redirect, free, user_data);
        return ppbox_success;
    }

#if __cplusplus
}
#endif // __cplusplus
