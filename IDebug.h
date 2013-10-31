// IDebug.h

#ifndef _PPBOX_PPBOX_I_DEBUG_H_
#define _PPBOX_PPBOX_I_DEBUG_H_

#include "IPpboxTypes.h"

PPBOX_FUNC_1(PP_err, PPBOX_ChangeDebugMode, (
    (PP_bool, mode)));

PPBOX_STRUCT_5(PPBOX_DebugMessage, (
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
