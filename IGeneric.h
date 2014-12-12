// IGeneric.h

#ifndef _JUST_EXPORT_I_GENERIC_H_
#define _JUST_EXPORT_I_GENERIC_H_

#include "IJustTypes.h"

JUST_CALLBACK_2(void, JUST_Callback, (PP_context, PP_err));

JUST_FUNC_3(PP_err, JUST_StartEngine, (
    (PP_str, gid), 
    (PP_str, pid), 
    (PP_str, auth)));

JUST_FUNC_0(PP_err, JUST_StopEngine, ());

//获得错误码
//返回值: 错误码
JUST_FUNC_0(PP_err, JUST_GetLastError, ());

JUST_FUNC_0(PP_str, JUST_GetLastErrorMsg, ());

JUST_FUNC_0(PP_str, JUST_GetVersion, ());

JUST_FUNC_4(PP_err, JUST_SetConfig, (
    (PP_str, module), 
    (PP_str, section), 
    (PP_str, key), 
    (PP_str, value)));

JUST_FUNC_2(PP_err, JUST_SubmitMessage, (
    (PP_str, msg), 
    (PP_uint, size)));

#endif // _JUST_EXPORT_I_GENERIC_H_
