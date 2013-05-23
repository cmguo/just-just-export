// IFormat.h

#ifndef _PPBOX_PPBOX_I_FORMAT_H_
#define _PPBOX_PPBOX_I_FORMAT_H_

#include "IPpbox.h"

#include <ppbox/avcodec/CodecType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>
#include <ppbox/avcodec/avc/AvcFormatType.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

    typedef ppbox::avcodec::StreamType PPBOX_StreamType;
    typedef ppbox::avcodec::StreamSubType PPBOX_StreamSubType;
    typedef ppbox::avcodec::StreamFormatType PPBOX_StreamFormatType;
    typedef ppbox::avcodec::VideoSubType PPBOX_VideoSubType;
    typedef ppbox::avcodec::AudioSubType PPBOX_AudioSubType;
    typedef ppbox::avcodec::AvcFormatType PPBOX_AvcFormatType;
    typedef ppbox::avcodec::AacFormatType PPBOX_AacFormatType;

    struct PPBOX_VideoInfo
    {
        PP_uint width;
        PP_uint height;
        PP_uint frame_rate;
    };

    struct PPBOX_AudioInfo
    {
        PP_uint channel_count;
        PP_uint sample_size;
        PP_uint sample_rate;
    };

    struct PPBOX_StreamInfo
    {
        PP_uint type;          // TypeEnum
        PP_uint sub_type;      // SubTypeEnum
        PP_uint time_scale;
        PP_uint bitrate;      // bps
        union 
        {
            PPBOX_VideoInfo video_format;
            PPBOX_AudioInfo audio_format;
        };
        PP_uint format_type;   // ��ʽ˵��������
        PP_uint format_size;  // ��ʽ˵���Ĵ�С
        PP_ubyte const * format_buffer;   // ��ͬ�Ľ��벻ͬ�Ľṹ�壬��ʽ˵���ĵ�����
    };

    struct PPBOX_SampleFlag
    {
        enum Enum
        {
            sync = 1, 
            discontinuity = 2,
        };
    };

    struct PPBOX_SampleBuffer
    {
        PP_ubyte const * data;
        PP_uint len;
    };

    struct PPBOX_Sample
    {
        PP_uint itrack;           // ���ı��
        PP_uint flags;            // Sample�ı�־
        PP_ulong time;             // Sample��Ӧ��ʱ���, ��λ�Ǻ���
        PP_ulong decode_time;
        PP_uint composite_time_delta;
        PP_uint duration;
        PP_uint size;             // Sample�Ĵ�С
        PP_ubyte const * buffer;    // Sample�����ݣ����bufferΪNULL��˵���ж��buffer��size����buffer�ĸ���
        //�����ñ������ĺ����������ڴ�Ȼ�����Sample
        //��������һ��ReadSample������ʱ�򣬻���Stop��ʱ���ڴ��ͷŵ�
        void const * context;
    };

    struct PPBOX_PlayStatus
    {
        enum Enum
        {
            closed = 0, 
            playing, 
            buffering, 
            paused, 
        };
    };

    struct PPBOX_PlayStatistic
    {
        PP_uint length;           //���ṹ��ĳ���
        PP_uint play_status;       //����״̬ 0-δ���� 1-playing̬ 2-buffering̬ 3-Pausing̬
        PP_uint buffering_present;//���Ż���ٷֱ� 10 ��ʾ 10%
        PP_uint buffer_time;      //���ػ��������ݵ���ʱ��
    };

    struct PPBOX_DownloadMsg
    {
        PP_uint length;                   // ���ṹ��ĳ���
        PP_uint start_time;               // ��ʼʱ��
        PP_uint total_download_bytes;     // �ܹ����ص��ֽ���
        PP_uint total_upload_bytes;       // �ܹ��ϴ�ʱ�ֽ���
        PP_uint http_download_bytes;      // Http���ص��ֽ���
        PP_uint http_downloader_count;    // Http���ظ���
        PP_uint p2p_download_bytes;       // P2P���ص��ֽ���
        PP_uint p2p_upload_bytes;         // P2P�ϴ����ֽ���
        PP_uint p2p_downloader_count;     // P2P���ظ���
        PP_uint p2p_downloader_count_ext; // ��P2P���ص���Դ����
        PP_uint total_upload_cache_request_count; // �ܹ����ϴ�Cache������
        PP_uint total_upload_cache_hit_count;     // �ܹ����ϴ�Cache������
        PP_uint download_duration_in_sec; // �����ܹ�����ʱ��(��)
        PP_uint local_peer_version;       // �Լ��ں˰汾��
    };

    struct PPBOX_DownloadSpeedMsg
    {
        PP_uint length;                   // ���ṹ��ĳ���
        PP_uint now_download_speed;       // ��ǰ�����ٶ� <5sͳ��>
        PP_uint now_upload_speed;         // ��ǰ�ϴ��ٶ� <5sͳ��>
        PP_uint minute_download_speed;    // ���һ����ƽ�������ٶ� <60sͳ��>
        PP_uint minute_upload_speed;      // ���һ����ƽ���ϴ��ٶ� <60sͳ��>
        PP_uint avg_download_speed;       // ��ʷƽ�������ٶ�
        PP_uint avg_upload_speed;         // ��ʷƽ���ϴ��ٶ�

        PP_uint recent_download_speed;    // ��ǰ�����ٶ� <20sͳ��>
        PP_uint recent_upload_speed;      // ��ǰ�ϴ��ٶ� <20sͳ��>
        PP_uint second_download_speed;    // ��ǰ1s�������ٶ�
        PP_uint second_upload_speed;      // ��ǰ1s���ϴ��ٶ�
    };

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DEMUXER_H_
