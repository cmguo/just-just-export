// IDemuxer.h

#ifndef _PPBOX_PPBOX_I_DEMUXER_H_
#define _PPBOX_PPBOX_I_DEMUXER_H_

#include "IPpbox.h"
#include "IFormat.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    //打开一个视频
    PPBOX_DECL PP_err PPBOX_Open(
        PP_char const * playlink);

    typedef void (*PPBOX_Open_Callback)(PP_int32);

    PPBOX_DECL void PPBOX_AsyncOpen(
        PP_char const * playlink, 
        PPBOX_Open_Callback callback);

    PPBOX_DECL PP_err PPBOX_OpenEx(
        PP_char const * playlink, 
        PP_char const * format);

    PPBOX_DECL void PPBOX_AsyncOpenEx(
        PP_char const * playlink, 
        PP_char const * format, 
		void * user_data, 
        PPBOX_Callback callback);

    //暂停
    PPBOX_DECL PP_err PPBOX_Pause();

    //强制结束
    PPBOX_DECL void PPBOX_Close();

    //获得有多少流
    PPBOX_DECL PP_uint32 PPBOX_GetStreamCount();

    //获得流的详细信息
    PPBOX_DECL PP_err PPBOX_GetStreamInfo(
        PP_uint32 index, 
        PPBOX_StreamInfo * info);

    //获得总时长
    PPBOX_DECL PP_uint32 PPBOX_GetDuration();

    //跳到某个时刻开始播放
    PPBOX_DECL PP_err PPBOX_Seek(
        PP_uint32 start_time);

    //同步读取Sample接口，不阻塞
    PPBOX_DECL PP_err PPBOX_ReadSample(
        PPBOX_Sample * sample);

    //设置下载缓冲区大小 （只能在Open前调用）
    // 主要用于控制内存，如果下载速度大于ReadSample的速度，那么下载的数据将存放
    // 于内存之中，当内存中的下载缓冲大于这个预设值，那么将停止下载。直到被调用了
    // ReadSample，少了一些内存占用后，再继续下载，
    // length: 预设的下载内存缓冲的大小
    PPBOX_DECL void PPBOX_SetDownloadBufferSize(
        PP_uint32 length);

    //设置播放缓冲区的缓冲时间 (随时可以调用)
    // 主要用于计算播放状态，如果不调用这个函数，默认3s
    // 如果 下载缓冲区数据的总时间 < 播放缓冲时间 则 处于 buffering 状态
    // 如果 下载缓冲区数据的总时间 >=播放缓冲时间 则 处于 playing 状态
    // 如果 人为调用了 Pause 使之暂停的，则处于 Pausing 状态
    // 如果处于buffering状态，下载缓冲区数据的总时间/播放缓冲时间*100%就是缓冲百分比
    PPBOX_DECL void PPBOX_SetPlayBufferTime(
        PP_uint32 time);

    //获得播放信息
    //返回值: 错误码
    //    ppbox_success      表示成功
    //    其他数值表示失败
    PPBOX_DECL PP_err PPBOX_GetPlayMsg(
        PPBOX_PlayStatistic * statistic_Msg);

    //获得下载信息
    // download_Msg: 调用这个接口后，用于获取的下载信息数据
    //返回值: 错误码
    //    ppbox_success      表示成功
    //    其他数值表示失败
    PPBOX_DECL PP_err PPBOX_GetDownMsg(
        PPBOX_DownloadMsg* download_Msg);

    //获得下载速度信息
    // download_Msg: 调用这个接口后，用于获取的下载信息数据
    //返回值: 错误码
    //    ppbox_success      表示成功
    //    其他数值表示失败
    PPBOX_DECL PP_err PPBOX_GetDownSedMsg(
        PPBOX_DownloadSpeedMsg* download_spped_Msg);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DEMUXER_H_
