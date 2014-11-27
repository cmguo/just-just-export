// IDebug.h

#ifndef _JUST_JUST_I_DEBUG_H_
#define _JUST_JUST_I_DEBUG_H_

#include "IPpboxTypes.h"

JUST_FUNC_1(PP_err, JUST_ChangeDebugMode, (
    (PP_bool, mode)));

JUST_STRUCT_5(JUST_DebugMessage, (
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
JUST_FUNC_4(PP_uint, JUST_GetDebugMessage, (
    (JUST_DebugMessage *, vector), 
    (PP_uint, size), 
    (PP_str, module), 
    (PP_uint, level)));

JUST_CALLBACK_2(void, JUST_LogHook, (PP_str, PP_uint));

JUST_FUNC_2(PP_err, JUST_SetLogHook, (
    (JUST_LogHook, hook),
    (PP_uint, level)));

#endif // _JUST_JUST_I_DEBUG_H_
