// IDebug.h

#ifndef _JUST_JUST_I_DEBUG_H_
#define _JUST_JUST_I_DEBUG_H_

#include "IPpboxTypes.h"

JUST_FUNC_1(PP_err, JUST_ChangeDebugMode, (
    (PP_bool, mode)));

JUST_STRUCT_5(JUST_DebugMessage, (
    (PP_uint, time),         // time(NULL)���ص�ֵ
    (PP_str, module), // \0��β
    (PP_uint, level),        // ��־�ȼ�
    (PP_uint, size),         // msg�ĳ��ȣ�������\0��β
    (PP_str, msg)    // \0��β
    ));

// vector:������־��Ϣ��buffer
// size:������ȡ��־������
// module:��ȡ��־��ģ�����ƣ���NULL ��ȡ���⣩
// level <= ��־�ȼ�
// ����ֵ��ʵ�ʻ�ȡ��־������
// ����0��ʾû�л�ȡ���κ���־��
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
