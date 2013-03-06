// IDispatch.h

#ifndef _PPBOX2_PPBOX2_I_DISPATCH_H_
#define _PPBOX2_PPBOX2_I_DISPATCH_H_

#include "IPpbox.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    //�������ػ�������С ��ֻ����Openǰ���ã�
    // ��Ҫ���ڿ����ڴ棬��������ٶȴ���ReadSample���ٶȣ���ô���ص����ݽ����
    // ���ڴ�֮�У����ڴ��е����ػ���������Ԥ��ֵ����ô��ֹͣ���ء�ֱ����������
    // ReadSample������һЩ�ڴ�ռ�ú��ټ������أ�
    // length: Ԥ��������ڴ滺��Ĵ�С
    PPBOX_DECL void PPBOX2_SetBufferSize(
        PP_uint32 length);

    //��һ����Ƶ
    PPBOX_DECL PPBOX_HANDLE PPBOX2_Open(
        PP_char const * playlink, 
        PP_char const * config);

    typedef void (*PPBOX2_Open_Callback)(PP_err);

    PPBOX_DECL PPBOX_HANDLE PPBOX2_AsyncOpen(
        PP_char const * playlink, 
        PP_char const * config, 
        PPBOX2_Open_Callback callback);

    //����ж�����
    PPBOX_DECL PP_uint32 PPBOX2_GetStreamCount(
        PPBOX_HANDLE handle);

    enum PPBOX2_StreamTypeEnum
    {
        ppbox2_video = 1, 
        ppbox2_audio = 2, 
    };

    enum PPBOX2_StreamSubTypeEnum
    {
        ppbox2_video_avc = 1, 
        ppbox2_audio_aac = 2, 
        ppbox2_audio_mp3 = 3, 
        ppbox2_audio_wma = 4,
    };

    enum PPBOX2_StreamFormatTypeEnum
    {
        ppbox2_video_avc_packet = 1, 
        ppbox2_video_avc_byte_stream = 2, 
        ppbox2_audio_microsoft_wave = 3, 
        ppbox2_audio_iso_mp4 = 4, 
    };

    typedef struct tag_PPBOX2_VideoInfo
    {
        PP_uint32 width;
        PP_uint32 height;
        PP_uint32 frame_rate;
    } PPBOX2_VideoInfo;

    typedef struct tag_PPBOX2_AudioInfo
    {
        PP_uint32 channel_count;
        PP_uint32 sample_size;
        PP_uint32 sample_rate;
    } PPBOX2_AudioInfo;

    typedef struct tag_PPBOX2_StreamInfo
    {
        PP_int32 type;          // TypeEnum
        PP_int32 sub_type;      // SubTypeEnum
        PP_uint32 time_scale;   // ʱ�侫��
        union 
        {
            PPBOX2_VideoInfo video_format;
            PPBOX2_AudioInfo audio_format;
        };
        PP_int32 format_type;   // ��ʽ˵��������
        PP_uint32 format_size;  // ��ʽ˵���Ĵ�С
        PP_uchar const * format_buffer;   // ��ͬ�Ľ��벻ͬ�Ľṹ�壬��ʽ˵���ĵ�����
    } PPBOX2_StreamInfo;

    //���������ϸ��չ��Ϣ
    PPBOX_DECL PP_err PPBOX2_GetStreamInfo(
        PPBOX_HANDLE handle, 
        PP_uint32 index,
        PPBOX2_StreamInfo *info);

    //�����ʱ��
    PPBOX_DECL PP_uint64 PPBOX2_GetDuration(
        PPBOX_HANDLE handle);

    //��ͣ
    //PPBOX_DECL void PPBOX2_AsyncDispatch(
    //    PPBOX_HANDLE handle, 
    //    );

    PPBOX_DECL PP_err PPBOX2_Pause(
        PPBOX_HANDLE handle);

    //��ͣ
    PPBOX_DECL PP_err PPBOX2_Resume(
        PPBOX_HANDLE handle);

    enum PPBOX2_SeekTypeEnum
    {
    };

    //����ĳ��ʱ�̿�ʼ����
    PPBOX_DECL PP_err PPBOX2_Seek(
        PPBOX_HANDLE handle, 
        PP_int32 type, 
        PP_uint64 start, 
        PP_uint64 end);

    enum PPBOX2_SampleFlagEnum
    {
        ppbox2_sample_sync = 1, 
    };

    typedef struct tag_PPBOX2_Sample
    {
        PP_uint32 stream_index;     // ���ı��
        PP_uint32 flags;            // Sample flags
        PP_uint64 start_time;       // Sample��Ӧ��ʱ���, ��λ��΢��
        PP_uint64 decode_time;      // Sample��Ӧ��ʱ���, PPBOX2_StreamInfo ��ָʾ�� time scale ����
        PP_uint32 composite_time_delta; // PPBOX2_StreamInfo ��ָʾ�� time scale ����
        PP_uint32 duration;         // PPBOX2_StreamInfo ��ָʾ�� time scale ����
        PP_uint32 buffer_length;    // Sample�Ĵ�С
        PP_uchar const * buffer;    // Sample������
        //�����ñ������ĺ����������ڴ�Ȼ�����Sample
        //��������һ��ReadSample������ʱ�򣬻���Stop��ʱ���ڴ��ͷŵ�
    } PPBOX2_Sample;

    PPBOX_DECL PP_err PPBOX2_ReadSample(
        PPBOX_HANDLE handle, 
        PPBOX2_Sample * sample);

    typedef struct tag_PPBOX2_StreamStatus
    {
        PP_uint32 buffer_time;      //���ػ��������ݵ���ʱ��
    } PPBOX2_StreamStatus;

    //��ò�����Ϣ
    //����ֵ: ������
    //    ppbox2_success      ��ʾ�ɹ�
    //    ������ֵ��ʾʧ��
    PPBOX_DECL PP_err PPBOX2_GetStreamStatus(
        PPBOX_HANDLE handle, 
        PPBOX2_StreamStatus * status);

    typedef struct tag_PPBOX2_BufferStatus
    {
        PP_uint32 total;    // �ܹ����ص��ֽ���
        PP_uint32 speed;    // �ܹ��ϴ�ʱ�ֽ���
    } PPBOX2_BufferStatus;

    //���������Ϣ
    // download_Msg: ��������ӿں����ڻ�ȡ��������Ϣ����
    //����ֵ: ������
    //    ppbox2_success      ��ʾ�ɹ�
    //    ������ֵ��ʾʧ��
    PPBOX_DECL PP_err PPBOX2_GetBufferStatus(
        PPBOX_HANDLE handle, 
        PPBOX2_BufferStatus * status);

    //ǿ�ƽ���
    PPBOX_DECL void PPBOX2_Close(
        PPBOX_HANDLE handle);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX2_PPBOX2_I_DISPATCH_H_
