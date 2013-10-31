// ICallback.h

#ifndef _PPBOX_PPBOX_I_CALLBACK_H_
#define _PPBOX_PPBOX_I_CALLBACK_H_

#include "IPpboxTypes.h"

#include <stdarg.h>

PPBOX_CALLBACK_4(void, PPBOX_CallbackRedirect, (PP_context, PP_handle, void *, va_list));

PPBOX_CALLBACK_1(void, PPBOX_CallbackFree, (PP_context));

PPBOX_FUNC_3(PP_err, PPBOX_RedirectCallback, (
    (PPBOX_CallbackRedirect, redirect), 
    (PPBOX_CallbackFree, free), 
    (PP_context, user_data)));

PPBOX_FUNC_3(PP_handle, PPBOX_ScheduleCallback, (
    (PP_uint, delay), 
    (PP_context, user_data), 
    (PPBOX_Callback, callback)));

PPBOX_FUNC_1(PP_err, PPBOX_CancelCallback, (
    (PP_handle, handle)));

#endif // _PPBOX_PPBOX_I_CALLBACK_H_
