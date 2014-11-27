// IFormat.h

#ifndef _JUST_JUST_I_FORMAT_H_
#define _JUST_JUST_I_FORMAT_H_

#include "IPpboxTypes.h"

#ifdef BOOST_BIG_ENDIAN
#define JUST_MAKE_FOURC_TYPE(c1, c2, c3, c4) \
    ((((PP_uint)c1) << 24) | \
    (((PP_uint)c2) << 16) | \
    (((PP_uint)c3) << 8) | \
    (((PP_uint)c4)))
#else
#define JUST_MAKE_FOURC_TYPE(c1, c2, c3, c4) \
    ((((PP_uint)c1)) | \
    (((PP_uint)c2) << 8) | \
    (((PP_uint)c3)) << 16 | \
    (((PP_uint)c4) << 24))
#endif

JUST_ENUM_3(JUST_StreamType, (
    (NONE, 0),
    (VIDE, JUST_MAKE_FOURC_TYPE('V', 'I', 'D', 'E')),
    (AUDI, JUST_MAKE_FOURC_TYPE('A', 'U', 'D', 'I'))
    ));

JUST_ENUM_6(JUST_VideoSubType, (
    //(NONE, 0,
    (AVC1, JUST_MAKE_FOURC_TYPE('A', 'V', 'C', '1')),
    (MP4V, JUST_MAKE_FOURC_TYPE('M', 'P', '4', 'V')), 
    (WMV2, JUST_MAKE_FOURC_TYPE('W', 'M', 'V', '2')),
    (WMV3, JUST_MAKE_FOURC_TYPE('W', 'M', 'V', '3')),
    (I420, JUST_MAKE_FOURC_TYPE('I', '4', '2', '0')), 
    (RGBT, JUST_MAKE_FOURC_TYPE('R', 'G', 'B', 'T'))
    ));

JUST_ENUM_8(JUST_AudioSubType, (
    //(NONE, 0),
    (MP4A, JUST_MAKE_FOURC_TYPE('M', 'P', '4', 'A')),
    (MP3,  JUST_MAKE_FOURC_TYPE('M', 'P', '3', 0)),
    (MP2,  JUST_MAKE_FOURC_TYPE('M', 'P', '2', 0)),
    (WMA2, JUST_MAKE_FOURC_TYPE('W', 'M', 'A', '2')),
    (AC3,  JUST_MAKE_FOURC_TYPE('A', 'C', '3', 0)), 
    (EAC3, JUST_MAKE_FOURC_TYPE('E', 'A', 'C', '3')), 
    (FLT,  JUST_MAKE_FOURC_TYPE('F', 'L', 'T', 0)), 
    (PCM,  JUST_MAKE_FOURC_TYPE('P', 'C', 'M', 0)) 
    ));

JUST_ENUM_5(JUST_FormatType, (
    (none, 0),
    (video_avc_packet, 1),
    (video_avc_byte_stream, 2),
    (audio_raw, 1),
    (audio_adts, 2)
    ));

JUST_STRUCT_6(JUST_MediaInfo, (
    (PP_ulong, file_size), 
    (PP_ulong, duration), 
    (PP_uint, head_size), 
    (PP_uint, bitrate), 
    (PP_str, format), 
    (PP_str, format_data)
    ));

JUST_STRUCT_4(JUST_VideoInfo, (
    (PP_uint, width),
    (PP_uint, height),
    (PP_uint, frame_rate_num),
    (PP_uint, frame_rate_den)
    ));

JUST_STRUCT_5(JUST_AudioInfo, (
    (PP_uint, channel_count),
    (PP_uint, sample_size),
    (PP_uint, sample_rate),
    (PP_uint, block_align),
    (PP_uint, sample_per_frame)
    ));

JUST_ENUM_3(JUST_SeekType, (
    (reset, 0),
    (byte, 1),
    (time, 2)
    ));

#ifdef JUST_NO_UNION
JUST_STRUCT_12(JUST_StreamInfo, (
    (PP_uint, type),          // TypeEnum
    (PP_uint, sub_type),      // SubTypeEnum
    (PP_uint, time_scale),
    (PP_uint, bitrate),      // bps
    (PP_uint, __union0),
    (PP_uint, __union1),
    (PP_uint, __union2),
    (PP_uint, __union3),
    (PP_uint, __union4),
    (PP_uint, format_type),   // 格式说明的类型
    (PP_uint, format_size),  // 格式说明的大小
    (PP_ubyte const *, format_buffer)   // 不同的解码不同的结构体，格式说明的的内容
    ));
#else
JUST_STRUCT_8(JUST_StreamInfo, (
    (PP_uint, type),          // TypeEnum
    (PP_uint, sub_type),      // SubTypeEnum
    (PP_uint, time_scale),
    (PP_uint, bitrate),      // bps
    (union {
        JUST_VideoInfo video;
        JUST_AudioInfo audio;
} , format),
    (PP_uint, format_type),   // 格式说明的类型
    (PP_uint, format_size),  // 格式说明的大小
    (PP_ubyte const *, format_buffer)   // 不同的解码不同的结构体，格式说明的的内容
    ));
#endif

JUST_ENUM_2(JUST_SampleFlag, (
    (sync, 1),
    (discontinuity, 2)
));

JUST_STRUCT_1(JUST_Error, (
    (PP_err, ec)
    ));

JUST_STRUCT_1(JUST_Url, (
    (PP_str, str)
    ));

JUST_STRUCT_2(JUST_ConstBuffer, (
    (PP_ubyte const *, data),
    (PP_uint, len)
    ));

JUST_STRUCT_2(JUST_ConstBuffers, (
    (PP_uint, count), 
    (JUST_ConstBuffer *, buffers)
    ));

JUST_STRUCT_5(JUST_StreamStatus, (
    (PP_ulong, byte_pos), 
    (PP_ulong, byte_buf), 
    (PP_ulong, time_pos), 
    (PP_ulong, time_buf), 
    (PP_err, data_err)
));

JUST_STRUCT_9(JUST_Sample, (
    (PP_uint, itrack),           // 流的编号
    (PP_uint, flags),            // Sample的标志
    (PP_ulong, time),             // Sample对应的时间戳, 单位是微妙
    (PP_ulong, decode_time),
    (PP_uint, composite_time_delta),
    (PP_uint, duration),
    (PP_uint, size),           // Sample的大小
    (PP_ubyte const *, buffer),    // Sample的内容
    //当调用本函数的后，我们申请内存然后给他Sample
    //当调用下一个ReadSample函数的时候，或者Stop的时候，内存释放掉
    (PP_context, context)
    ));

JUST_ENUM_4(JUST_PlayStatus, (
    (closed, 0),
    (playing, 1),
    (buffering, 2), 
    (paused, 3)
    ));

JUST_STRUCT_3(JUST_PlayStatistic, (
    (PP_uint, play_status),         //播放状态 0-未启动 1-playing态 2-buffering态 3-Pausing态
    (PP_uint, buffering_present),   //播放缓冲百分比 10 表示 10%
    (PP_uint, buffer_time)          //下载缓冲区数据的总时间
    ));

JUST_ENUM_3(JUST_ConnectionStatus, (
    (closed, 0), 
    (opening, 1), 
    (receiving, 2) 
    ));

JUST_STRUCT_7(JUST_DataStat, (
    (PP_uint, total_elapse),                 // 持续时间
    (PP_uint, total_download_bytes),         // 总共下载的字节数
    (PP_uint, connection_status),            // 当前连接状态
    (PP_uint, average_speed_one_second),     // 平均下载速度 <1s统计>
    (PP_uint, average_speed_five_seconds),   // 平均下载速度 <5s统计>
    (PP_uint, average_speed_twenty_seconds), // 平均下载速度 <20s统计>
    (PP_uint, average_speed_sixty_seconds)   // 平均下载速度 <60s统计>
    ));

#endif // _JUST_JUST_I_DEMUXER_H_
