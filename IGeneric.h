// IGeneric.h

#ifndef _PPBOX_PPBOX_I_GENERIC_H_
#define _PPBOX_PPBOX_I_GENERIC_H_

#include "IPpboxTypes.h"

PPBOX_CALLBACK_2(void, PPBOX_Callback, (PP_context, PP_err));

PPBOX_FUNC_3(PP_err, PPBOX_StartEngine, (
    (PP_str, gid), 
    (PP_str, pid), 
    (PP_str, auth)));

PPBOX_FUNC_0(PP_err, PPBOX_StopEngine, ());

//获得错误码
//返回值: 错误码
PPBOX_FUNC_0(PP_err, PPBOX_GetLastError, ());

PPBOX_FUNC_0(PP_str, PPBOX_GetLastErrorMsg, ());

PPBOX_FUNC_0(PP_str, PPBOX_GetVersion, ());

PPBOX_FUNC_4(PP_err, PPBOX_SetConfig, (
    (PP_str, module), 
    (PP_str, section), 
    (PP_str, key), 
    (PP_str, value)));

PPBOX_FUNC_2(PP_err, PPBOX_SubmitMessage, (
    (PP_str, msg), 
    (PP_uint, size)));

#endif // _PPBOX_PPBOX_I_GENERIC_H_
