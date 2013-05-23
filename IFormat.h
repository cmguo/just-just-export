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
        PP_uint format_type;   // 格式说明的类型
        PP_uint format_size;  // 格式说明的大小
        PP_ubyte const * format_buffer;   // 不同的解码不同的结构体，格式说明的的内容
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
        PP_uint itrack;           // 流的编号
        PP_uint flags;            // Sample的标志
        PP_ulong time;             // Sample对应的时间戳, 单位是毫秒
        PP_ulong decode_time;
        PP_uint composite_time_delta;
        PP_uint duration;
        PP_uint size;             // Sample的大小
        PP_ubyte const * buffer;    // Sample的内容，如果buffer为NULL，说明有多段buffer，size代表buffer的个数
        //当调用本函数的后，我们申请内存然后给他Sample
        //当调用下一个ReadSample函数的时候，或者Stop的时候，内存释放掉
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
        PP_uint length;           //本结构体的长度
        PP_uint play_status;       //播放状态 0-未启动 1-playing态 2-buffering态 3-Pausing态
        PP_uint buffering_present;//播放缓冲百分比 10 表示 10%
        PP_uint buffer_time;      //下载缓冲区数据的总时间
    };

    struct PPBOX_DownloadMsg
    {
        PP_uint length;                   // 本结构体的长度
        PP_uint start_time;               // 开始时刻
        PP_uint total_download_bytes;     // 总共下载的字节数
        PP_uint total_upload_bytes;       // 总共上传时字节数
        PP_uint http_download_bytes;      // Http下载的字节数
        PP_uint http_downloader_count;    // Http下载个数
        PP_uint p2p_download_bytes;       // P2P下载的字节数
        PP_uint p2p_upload_bytes;         // P2P上传的字节数
        PP_uint p2p_downloader_count;     // P2P下载个数
        PP_uint p2p_downloader_count_ext; // 候补P2P下载的资源个数
        PP_uint total_upload_cache_request_count; // 总共的上传Cache请求数
        PP_uint total_upload_cache_hit_count;     // 总共的上传Cache命中数
        PP_uint download_duration_in_sec; // 下载总共持续时长(秒)
        PP_uint local_peer_version;       // 自己内核版本号
    };

    struct PPBOX_DownloadSpeedMsg
    {
        PP_uint length;                   // 本结构体的长度
        PP_uint now_download_speed;       // 当前下载速度 <5s统计>
        PP_uint now_upload_speed;         // 当前上传速度 <5s统计>
        PP_uint minute_download_speed;    // 最近一分钟平均下载速度 <60s统计>
        PP_uint minute_upload_speed;      // 最近一分钟平均上传速度 <60s统计>
        PP_uint avg_download_speed;       // 历史平均下载速度
        PP_uint avg_upload_speed;         // 历史平均上传速度

        PP_uint recent_download_speed;    // 当前下载速度 <20s统计>
        PP_uint recent_upload_speed;      // 当前上传速度 <20s统计>
        PP_uint second_download_speed;    // 当前1s的下载速度
        PP_uint second_upload_speed;      // 当前1s的上传速度
    };

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DEMUXER_H_
