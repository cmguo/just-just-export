// ICallback.h

#ifndef _PPBOX_PPBOX_I_CALLBACK_H_
#define _PPBOX_PPBOX_I_CALLBACK_H_

#include "IPpbox.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus


    typedef void (*PPBOX_CallbackRedirect)(PP_context, PP_handle, void *, va_list);
    typedef void (*PPBOX_CallbackFree)(PP_context);

    PPBOX_DECL PP_err PPBOX_RedirectCallback(
        PPBOX_CallbackRedirect redirect, 
        PPBOX_CallbackFree free, 
        PP_context user_data);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_CALLBACK_H_
