// IDebug.h

#ifndef _PPBOX_PPBOX_I_DEBUG_H_
#define _PPBOX_PPBOX_I_DEBUG_H_

#include "IPpboxTypes.h"

PPBOX_FUNC_1(PP_err, PPBOX_ChangeDebugMode, (
    (PP_bool, mode)));

PPBOX_STRUCT_5(PPBOX_DebugMessage, (
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
PPBOX_FUNC_4(PP_uint, PPBOX_GetDebugMessage, (
    (PPBOX_DebugMessage *, vector), 
    (PP_uint, size), 
    (PP_str, module), 
    (PP_uint, level)));

PPBOX_CALLBACK_2(void, PPBOX_LogHook, (PP_str, PP_uint));

PPBOX_FUNC_2(PP_err, PPBOX_SetLogHook, (
    (PPBOX_LogHook, hook),
    (PP_uint, level)));

#endif // _PPBOX_PPBOX_I_DEBUG_H_
