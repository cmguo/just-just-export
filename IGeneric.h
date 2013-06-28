// IGeneric.h

#ifndef _PPBOX_PPBOX_I_GENERIC_H_
#define _PPBOX_PPBOX_I_GENERIC_H_

#include "IPpbox.h"

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

PPBOX_FUNC_1(PP_err, PPBOX_DebugMode, (
    (PP_bool, mode)));

PPBOX_STRUCT_5(DialogMessage, (
    (PP_uint, time),         // time(NULL)返回的值
    (PP_str, module), // \0结尾
    (PP_uint, level),        // 日志等级
    (PP_uint, size),         // msg的长度，不包括\0结尾
    (PP_str, msg)    // \0结尾
    ));

// vector:保存日志信息的buffer
// size:期望获取日志的条数
// module:获取日志的模块名称，（NULL 获取任意）
// level <= 日志等级
// 返回值：实际获取日志的条数
// 返回0表示没有获取到任何日志。
PPBOX_FUNC_4(PP_uint, PPBOX_DialogMessage, (
    (DialogMessage *, vector), 
    (PP_uint, size), 
    (PP_str, module), 
    (PP_uint, level)));

PPBOX_FUNC_2(PP_err, PPBOX_SubmitMessage, (
    (PP_str, msg), 
    (PP_uint, size)));

PPBOX_CALLBACK_2(void, PPBOX_OnLogDump, (PP_str, PP_uint));

PPBOX_FUNC_2(PP_err, PPBOX_LogDump, (
    (PPBOX_OnLogDump, callback),
    (PP_uint, level)));

PPBOX_FUNC_3(PP_handle, PPBOX_ScheduleCallback, (
    (PP_uint, delay), 
    (PP_context, user_data), 
    (PPBOX_Callback, callback)));

PPBOX_FUNC_1(PP_err, PPBOX_CancelCallback, (
    (PP_handle, handle)));


#endif // _PPBOX_PPBOX_I_GENERIC_H_
