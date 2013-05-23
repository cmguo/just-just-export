// IPpbox.h

#ifndef _PPBOX_PPBOX_I_PPBOX_H_
#define _PPBOX_PPBOX_I_PPBOX_H_

#include <boost/config.hpp>

#ifdef BOOST_HAS_DECLSPEC

#ifdef PPBOX_SOURCE
#  define PPBOX_DECL __declspec(dllexport)
#else
#  define PPBOX_DECL __declspec(dllimport)
#endif  // PPBOX_SOURCE

#else
#  define PPBOX_DECL __attribute__ ((visibility("default")))
#endif

#include <boost/cstdint.hpp>

typedef bool PP_bool;
typedef char PP_char;
typedef boost::int8_t PP_byte;
typedef boost::int16_t PP_short;
typedef boost::int32_t PP_int;
typedef boost::int64_t PP_long;
typedef boost::uint8_t PP_ubyte;
typedef boost::uint16_t PP_ushort;
typedef boost::uint32_t PP_uint;
typedef boost::uint64_t PP_ulong;

typedef PP_int PP_err;
typedef PP_char const * PP_str;
typedef void const * PP_handle;
typedef void const * PP_context;

enum PPBOX_ErrorEnum
{
    ppbox_success = 0, 
    ppbox_not_start, 
    ppbox_already_start, 
    ppbox_not_open, 
    ppbox_already_open, 
    ppbox_operation_canceled, 
    ppbox_would_block, 
    ppbox_stream_end, 
    ppbox_logic_error, 
    ppbox_network_error, 
    ppbox_demux_error, 
    ppbox_certify_error, 
    ppbox_other_error = 1024, 
};

#if __cplusplus
extern "C" {
#endif // __cplusplus


    PPBOX_DECL PP_err PPBOX_StartEngine(
        PP_str gid, 
        PP_str pid, 
        PP_str auth);

    PPBOX_DECL PP_err PPBOX_StopEngine();

    //��ô�����
    //����ֵ: ������
    PPBOX_DECL PP_err PPBOX_GetLastError();

    PPBOX_DECL PP_str PPBOX_GetLastErrorMsg();

    PPBOX_DECL PP_str PPBOX_GetVersion();

    PPBOX_DECL PP_err PPBOX_SetConfig(
        PP_str module, 
        PP_str section, 
        PP_str key, 
        PP_str value);

    PPBOX_DECL PP_err PPBOX_DebugMode(
        PP_bool mode);

    typedef struct tag_DialogMessage
    {
        PP_uint time;         // time(NULL)���ص�ֵ
        PP_str module; // \0��β
        PP_uint level;        // ��־�ȼ�
        PP_uint size;         // msg�ĳ��ȣ�������\0��β
        PP_str msg;    // \0��β
    } DialogMessage;

    // vector:������־��Ϣ��buffer
    // size:������ȡ��־������
    // module:��ȡ��־��ģ�����ƣ���NULL ��ȡ���⣩
    // level <= ��־�ȼ�
    // ����ֵ��ʵ�ʻ�ȡ��־������
    // ����0��ʾû�л�ȡ���κ���־��
    PPBOX_DECL PP_uint PPBOX_DialogMessage(
        DialogMessage * vector, 
        PP_uint size, 
        PP_str module, 
        PP_uint level);

    PPBOX_DECL PP_err PPBOX_SubmitMessage(
        PP_str msg, 
        PP_uint size);

    typedef void (*PPBOX_Callback)(PP_context, PP_err);

    typedef void (*PPBOX_OnLogDump)(PP_str, PP_uint);

    PPBOX_DECL PP_err PPBOX_LogDump(
        PPBOX_OnLogDump callback,
        PP_uint level);

    PPBOX_DECL PP_handle PPBOX_ScheduleCallback(
        PP_uint delay, 
        PP_context user_data, 
        PPBOX_Callback callback);

    PPBOX_DECL PP_err PPBOX_CancelCallback(
        PP_handle handle);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_PPBOX_H_
