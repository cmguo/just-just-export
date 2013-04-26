// IFormat.h

#ifndef _PPBOX_PPBOX_I_FORMAT_H_
#define _PPBOX_PPBOX_I_FORMAT_H_

#include "IPpbox.h"

#include <ppbox/avformat/Format.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

    typedef ppbox::avformat::StreamType PPBOX_StreamType;
    typedef ppbox::avformat::VideoSubType PPBOX_VideoSubType;
    typedef ppbox::avformat::AudioSubType PPBOX_AudioSubType;
    typedef ppbox::avformat::FormatType PPBOX_FormatType;

    typedef struct tag_PPBOX_VideoInfo
    {
        PP_uint32 width;
        PP_uint32 height;
        PP_uint32 frame_rate;
    } PPBOX_VideoInfo;

    typedef struct tag_PPBOX_AudioInfo
    {
        PP_uint32 channel_count;
        PP_uint32 sample_size;
        PP_uint32 sample_rate;
    } PPBOX_AudioInfo;

    typedef struct tag_PPBOX_StreamInfo
    {
        PP_int32 type;          // TypeEnum
        PP_int32 sub_type;      // SubTypeEnum
        PP_uint32 time_scale;
        PP_uint32 bitrate;      // bps
        union 
        {
            PPBOX_VideoInfo video_format;
            PPBOX_AudioInfo audio_format;
        };
        PP_int32 format_type;   // ��ʽ˵��������
        PP_uint32 format_size;  // ��ʽ˵���Ĵ�С
        PP_uchar const * format_buffer;   // ��ͬ�Ľ��벻ͬ�Ľṹ�壬��ʽ˵���ĵ�����
    } PPBOX_StreamInfo;

    enum PPBOX_SampleFlagEnum
    {
        f_sync = 1, 
        f_discontinuity = 2,
        f_config = 4, 
    };

    typedef struct tag_PPBOX_Sample
    {
        PP_uint32 itrack;           // ���ı��
        PP_uint32 flags;            // Sample�ı�־
        PP_uint64 time;             // Sample��Ӧ��ʱ���, ��λ��΢��
        PP_uint64 decode_time;
        PP_uint32 composite_time_delta;
        PP_uint32 duration;
        PP_uint32 length;           // Sample�Ĵ�С
        PP_uchar const * buffer;    // Sample������
        //�����ñ������ĺ����������ڴ�Ȼ�����Sample
        //��������һ��ReadSample������ʱ�򣬻���Stop��ʱ���ڴ��ͷŵ�
    } PPBOX_Sample;

    enum PPBOX_PlayStatusEnum
    {
        ppbox_closed = 0, 
        ppbox_playing, 
        ppbox_buffering, 
        ppbox_paused, 
    };

    typedef struct tag_PPBOX_PlayStatistic
    {
        PP_uint32 length;           //���ṹ��ĳ���
        PP_int32 play_status;       //����״̬ 0-δ���� 1-playing̬ 2-buffering̬ 3-Pausing̬
        PP_uint32 buffering_present;//���Ż���ٷֱ� 10 ��ʾ 10%
        PP_uint32 buffer_time;      //���ػ��������ݵ���ʱ��
    } PPBOX_PlayStatistic;

    typedef struct tag_PPBOX_DownloadMsg
    {
        PP_uint32 length;                   // ���ṹ��ĳ���
        PP_uint32 start_time;               // ��ʼʱ��
        PP_uint32 total_download_bytes;     // �ܹ����ص��ֽ���
        PP_uint32 total_upload_bytes;       // �ܹ��ϴ�ʱ�ֽ���
        PP_uint32 http_download_bytes;      // Http���ص��ֽ���
        PP_uint32 http_downloader_count;    // Http���ظ���
        PP_uint32 p2p_download_bytes;       // P2P���ص��ֽ���
        PP_uint32 p2p_upload_bytes;         // P2P�ϴ����ֽ���
        PP_uint32 p2p_downloader_count;     // P2P���ظ���
        PP_uint32 p2p_downloader_count_ext; // ��P2P���ص���Դ����
        PP_uint32 total_upload_cache_request_count; // �ܹ����ϴ�Cache������
        PP_uint32 total_upload_cache_hit_count;     // �ܹ����ϴ�Cache������
        PP_uint32 download_duration_in_sec; // �����ܹ�����ʱ��(��)
        PP_uint32 local_peer_version;       // �Լ��ں˰汾��
    } PPBOX_DownloadMsg;

    typedef struct tag_PPBOX_DownloadSpeedMsg
    {
        PP_uint32 length;                   // ���ṹ��ĳ���
        PP_uint32 now_download_speed;       // ��ǰ�����ٶ� <5sͳ��>
        PP_uint32 now_upload_speed;         // ��ǰ�ϴ��ٶ� <5sͳ��>
        PP_uint32 minute_download_speed;    // ���һ����ƽ�������ٶ� <60sͳ��>
        PP_uint32 minute_upload_speed;      // ���һ����ƽ���ϴ��ٶ� <60sͳ��>
        PP_uint32 avg_download_speed;       // ��ʷƽ�������ٶ�
        PP_uint32 avg_upload_speed;         // ��ʷƽ���ϴ��ٶ�

        PP_uint32 recent_download_speed;    // ��ǰ�����ٶ� <20sͳ��>
        PP_uint32 recent_upload_speed;      // ��ǰ�ϴ��ٶ� <20sͳ��>
        PP_uint32 second_download_speed;    // ��ǰ1s�������ٶ�
        PP_uint32 second_upload_speed;      // ��ǰ1s���ϴ��ٶ�
    } PPBOX_DownloadSpeedMsg;

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DEMUXER_H_