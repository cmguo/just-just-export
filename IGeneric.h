// IGeneric.h

#ifndef _PPBOX_PPBOX_I_GENERIC_H_
#define _PPBOX_PPBOX_I_GENERIC_H_

#include "IPpbox.h"

PPBOX_FUNC_3(PP_err, PPBOX_StartEngine, (
    (PP_str, gid), 
    (PP_str, pid), 
    (PP_str, auth)));

PPBOX_FUNC_0(PP_err, PPBOX_StopEngine, ());

//��ô�����
//����ֵ: ������
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
