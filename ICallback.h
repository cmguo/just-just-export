// ICallback.h

#ifndef _JUST_JUST_I_CALLBACK_H_
#define _JUST_JUST_I_CALLBACK_H_

#include "IPpboxTypes.h"

#include <stdarg.h>

JUST_CALLBACK_4(void, JUST_CallbackRedirect, (PP_context, PP_handle, void *, va_list));

JUST_CALLBACK_1(void, JUST_CallbackFree, (PP_context));

JUST_FUNC_3(PP_err, JUST_RedirectCallback, (
    (JUST_CallbackRedirect, redirect), 
    (JUST_CallbackFree, free), 
    (PP_context, user_data)));

JUST_FUNC_3(PP_handle, JUST_ScheduleCallback, (
    (PP_uint, delay), 
    (PP_context, user_data), 
    (JUST_Callback, callback)));

JUST_FUNC_1(PP_err, JUST_CancelCallback, (
    (PP_handle, handle)));

#endif // _JUST_JUST_I_CALLBACK_H_
