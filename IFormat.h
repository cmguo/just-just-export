// IFormat.h

#ifndef _PPBOX_PPBOX_I_FORMAT_H_
#define _PPBOX_PPBOX_I_FORMAT_H_

#include "IPpboxTypes.h"

#ifdef BOOST_BIG_ENDIAN
#define PPBOX_MAKE_FOURC_TYPE(c1, c2, c3, c4) \
    ((((PP_uint)c1) << 24) | \
    (((PP_uint)c2) << 16) | \
    (((PP_uint)c3) << 8) | \
    (((PP_uint)c4)))
#else
#define PPBOX_MAKE_FOURC_TYPE(c1, c2, c3, c4) \
    ((((PP_uint)c1)) | \
    (((PP_uint)c2) << 8) | \
    (((PP_uint)c3)) << 16 | \
    (((PP_uint)c4) << 24))
#endif

PPBOX_ENUM_3(PPBOX_StreamType, (
    (NONE, 0),
    (VIDE, PPBOX_MAKE_FOURC_TYPE('V', 'I', 'D', 'E')),
    (AUDI, PPBOX_MAKE_FOURC_TYPE('A', 'U', 'D', 'I'))
    ));

PPBOX_ENUM_6(PPBOX_VideoSubType, (
    //(NONE, 0,
    (AVC1, PPBOX_MAKE_FOURC_TYPE('A', 'V', 'C', '1')),
    (MP4V, PPBOX_MAKE_FOURC_TYPE('M', 'P', '4', 'V')), 
    (WMV2, PPBOX_MAKE_FOURC_TYPE('W', 'M', 'V', '2')),
    (WMV3, PPBOX_MAKE_FOURC_TYPE('W', 'M', 'V', '3')),
    (I420, PPBOX_MAKE_FOURC_TYPE('I', '4', '2', '0')), 
    (RGBT, PPBOX_MAKE_FOURC_TYPE('R', 'G', 'B', 'T'))
    ));

PPBOX_ENUM_8(PPBOX_AudioSubType, (
    //(NONE, 0),
    (MP4A, PPBOX_MAKE_FOURC_TYPE('M', 'P', '4', 'A')),
    (MP3,  PPBOX_MAKE_FOURC_TYPE('M', 'P', '3', 0)),
    (MP2,  PPBOX_MAKE_FOURC_TYPE('M', 'P', '2', 0)),
    (WMA2, PPBOX_MAKE_FOURC_TYPE('W', 'M', 'A', '2')),
    (AC3,  PPBOX_MAKE_FOURC_TYPE('A', 'C', '3', 0)), 
    (EAC3, PPBOX_MAKE_FOURC_TYPE('E', 'A', 'C', '3')), 
    (FLT,  PPBOX_MAKE_FOURC_TYPE('F', 'L', 'T', 0)), 
    (PCM,  PPBOX_MAKE_FOURC_TYPE('P', 'C', 'M', 0)) 
    ));

PPBOX_ENUM_5(PPBOX_FormatType, (
    (none, 0),
    (video_avc_packet, 1),
    (video_avc_byte_stream, 2),
    (audio_raw, 1),
    (audio_adts, 2)
    ));

PPBOX_STRUCT_5(PPBOX_MediaInfo, (
    (PP_ulong, file_size), 
    (PP_ulong, duration), 
    (PP_uint, bitrate), 
    (PP_str, format), 
    (PP_str, format_data)
    ));

PPBOX_STRUCT_4(PPBOX_VideoInfo, (
    (PP_uint, width),
    (PP_uint, height),
    (PP_uint, frame_rate_num),
    (PP_uint, frame_rate_den)
    ));

PPBOX_STRUCT_5(PPBOX_AudioInfo, (
    (PP_uint, channel_count),
    (PP_uint, sample_size),
    (PP_uint, sample_rate),
    (PP_uint, block_align),
    (PP_uint, sample_per_frame)
    ));

PPBOX_ENUM_3(PPBOX_SeekType, (
    (reset, 0),
    (byte, 1),
    (time, 2)
    ));

#ifdef PPBOX_NO_UNION
PPBOX_STRUCT_12(PPBOX_StreamInfo, (
    (PP_uint, type),          // TypeEnum
    (PP_uint, sub_type),      // SubTypeEnum
    (PP_uint, time_scale),
    (PP_uint, bitrate),      // bps
    (PP_uint, __union0),
    (PP_uint, __union1),
    (PP_uint, __union2),
    (PP_uint, __union3),
    (PP_uint, __union4),
    (PP_uint, format_type),   // ��ʽ˵��������
    (PP_uint, format_size),  // ��ʽ˵���Ĵ�С
    (PP_ubyte const *, format_buffer)   // ��ͬ�Ľ��벻ͬ�Ľṹ�壬��ʽ˵���ĵ�����
    ));
#else
PPBOX_STRUCT_8(PPBOX_StreamInfo, (
    (PP_uint, type),          // TypeEnum
    (PP_uint, sub_type),      // SubTypeEnum
    (PP_uint, time_scale),
    (PP_uint, bitrate),      // bps
    (union {
        PPBOX_VideoInfo video;
        PPBOX_AudioInfo audio;
} , format),
    (PP_uint, format_type),   // ��ʽ˵��������
    (PP_uint, format_size),  // ��ʽ˵���Ĵ�С
    (PP_ubyte const *, format_buffer)   // ��ͬ�Ľ��벻ͬ�Ľṹ�壬��ʽ˵���ĵ�����
    ));
#endif

PPBOX_ENUM_2(PPBOX_SampleFlag, (
    (sync, 1),
    (discontinuity, 2)
));

PPBOX_STRUCT_1(PPBOX_Error, (
    (PP_err, ec)
    ));

PPBOX_STRUCT_1(PPBOX_Url, (
    (PP_str, str)
    ));

PPBOX_STRUCT_2(PPBOX_ConstBuffer, (
    (PP_ubyte const *, data),
    (PP_uint, len)
    ));

PPBOX_STRUCT_2(PPBOX_ConstBuffers, (
    (PP_uint, count), 
    (PPBOX_ConstBuffer *, buffers)
    ));

PPBOX_STRUCT_7(PPBOX_StreamStatus, (
    (PP_uint, video_index), 
    (PP_uint, audio_index), 
    (PP_ulong, byte_pos), 
    (PP_ulong, byte_buf), 
    (PP_ulong, time_pos), 
    (PP_ulong, time_buf), 
    (PP_err, data_err)
));

PPBOX_STRUCT_9(PPBOX_Sample, (
    (PP_uint, itrack),           // ���ı��
    (PP_uint, flags),            // Sample�ı�־
    (PP_ulong, time),             // Sample��Ӧ��ʱ���, ��λ��΢��
    (PP_ulong, decode_time),
    (PP_uint, composite_time_delta),
    (PP_uint, duration),
    (PP_uint, size),           // Sample�Ĵ�С
    (PP_ubyte const *, buffer),    // Sample������
    //�����ñ������ĺ����������ڴ�Ȼ�����Sample
    //��������һ��ReadSample������ʱ�򣬻���Stop��ʱ���ڴ��ͷŵ�
    (PP_context, context)
    ));

PPBOX_ENUM_4(PPBOX_PlayStatus, (
    (closed, 0),
    (playing, 1),
    (buffering, 2), 
    (paused, 3)
    ));

PPBOX_STRUCT_3(PPBOX_PlayStatistic, (
    (PP_uint, play_status),         //����״̬ 0-δ���� 1-playing̬ 2-buffering̬ 3-Pausing̬
    (PP_uint, buffering_present),   //���Ż���ٷֱ� 10 ��ʾ 10%
    (PP_uint, buffer_time)          //���ػ��������ݵ���ʱ��
    ));

PPBOX_ENUM_3(PPBOX_ConnectionStatus, (
    (closed, 0), 
    (opening, 1), 
    (receiving, 2) 
    ));

PPBOX_STRUCT_7(PPBOX_DataStat, (
    (PP_uint, total_elapse),                 // ����ʱ��
    (PP_uint, total_download_bytes),         // �ܹ����ص��ֽ���
    (PP_uint, connection_status),            // ��ǰ����״̬
    (PP_uint, average_speed_one_second),     // ƽ�������ٶ� <1sͳ��>
    (PP_uint, average_speed_five_seconds),   // ƽ�������ٶ� <5sͳ��>
    (PP_uint, average_speed_twenty_seconds), // ƽ�������ٶ� <20sͳ��>
    (PP_uint, average_speed_sixty_seconds)   // ƽ�������ٶ� <60sͳ��>
    ));

#endif // _PPBOX_PPBOX_I_DEMUXER_H_
