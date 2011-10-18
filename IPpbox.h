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

typedef char PP_char;
typedef bool PP_bool;
typedef unsigned char PP_uchar;
typedef boost::int32_t PP_int32;
typedef boost::uint16_t PP_uint16;
typedef boost::uint32_t PP_uint32;
typedef boost::uint64_t PP_uint64;

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

    PPBOX_DECL PP_int32 PPBOX_StartP2PEngine(
        PP_char const * gid, 
        PP_char const * pid, 
        PP_char const * auth);

    PPBOX_DECL void PPBOX_StopP2PEngine();

    //��ô�����
    //����ֵ: ������
    PPBOX_DECL PP_int32 PPBOX_GetLastError();

    PPBOX_DECL PP_char const * PPBOX_GetLastErrorMsg();

    PPBOX_DECL PP_char const * PPBOX_GetVersion();

    PPBOX_DECL void PPBOX_SetConfig(
        PP_char const * module, 
        PP_char const * section, 
        PP_char const * key, 
        PP_char const * value);

    PPBOX_DECL void PPBOX_DebugMode(
        PP_bool mode);

    typedef struct tag_DialogMessage
    {
        PP_uint32 time;         // time(NULL)���ص�ֵ
        PP_char const * module; // \0��β
        PP_uint32 level;        // ��־�ȼ�
        PP_uint32 size;         // msg�ĳ��ȣ�������\0��β
        PP_char const * msg;    // \0��β
    } DialogMessage;

    // vector:������־��Ϣ��buffer
    // size:������ȡ��־������
    // module:��ȡ��־��ģ�����ƣ���NULL ��ȡ���⣩
    // level <= ��־�ȼ�
    // ����ֵ��ʵ�ʻ�ȡ��־������
    // ����0��ʾû�л�ȡ���κ���־��
    PPBOX_DECL PP_uint32 PPBOX_DialogMessage(
        DialogMessage * vector, 
        PP_int32 size, 
        PP_char const * module, 
        PP_int32 level);

    PPBOX_DECL void PPBOX_SubmitMessage(
        PP_char const * msg, 
        PP_int32 size);

    typedef void (*PPBOX_Callback)(void *, PP_int32);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_PPBOX_H_