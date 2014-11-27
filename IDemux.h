// IDemux.h

#ifndef _JUST_JUST_I_DEMUX_H_
#define _JUST_JUST_I_DEMUX_H_

#include "IGeneric.h"
#include "IFormat.h"

//打开一个视频
JUST_FUNC_1(PP_err, JUST_Open, (
    (PP_str, playlink)));

JUST_FUNC_2(PP_err, JUST_OpenEx, (
    (PP_str, playlink), 
    (PP_str, format)));

JUST_CALLBACK_1(void, JUST_Open_Callback, (PP_err));

JUST_FUNC_2(PP_err, JUST_AsyncOpen, (
    (PP_str, playlink), 
    (JUST_Open_Callback, callback)));

JUST_FUNC_4(PP_err, JUST_AsyncOpenEx, (
    (PP_str, playlink), 
    (PP_str, format), 
    (PP_context, user_data), 
    (JUST_Callback, callback)));

//暂停
JUST_FUNC_0(PP_err, JUST_Pause, ());

//强制结束
JUST_FUNC_0(PP_err, JUST_Close, ());

//获得有多少流
JUST_FUNC_0(PP_uint, JUST_GetStreamCount, ());


//获得流的详细信息
JUST_FUNC_2(PP_err, JUST_GetStreamInfo, (
    (PP_uint, index), 
    (JUST_StreamInfo *, info)));

//获得总时长
JUST_FUNC_0(PP_uint, JUST_GetDuration, ());

//跳到某个时刻开始播放
JUST_FUNC_1(PP_err, JUST_Seek, (
    (PP_uint, time)));

//同步读取Sample接口，不阻塞
JUST_FUNC_1(PP_err, JUST_ReadSample, (
    (JUST_Sample *, sample)));

//设置播放缓冲区的缓冲时间 (随时可以调用)
// 主要用于计算播放状态，如果不调用这个函数，默认3s
// 如果 下载缓冲区数据的总时间 < 播放缓冲时间 则 处于 buffering 状态
// 如果 下载缓冲区数据的总时间 >=播放缓冲时间 则 处于 playing 状态
// 如果 人为调用了 Pause 使之暂停的，则处于 Pausing 状态
// 如果处于buffering状态，下载缓冲区数据的总时间/播放缓冲时间*100%就是缓冲百分比
JUST_FUNC_1(PP_err, JUST_SetPlayBufferTime, (
    (PP_uint, time)));

//设置下载缓冲区大小 （只能在Open前调用）
// 主要用于控制内存，如果下载速度大于ReadSample的速度，那么下载的数据将存放
// 于内存之中，当内存中的下载缓冲大于这个预设值，那么将停止下载。直到被调用了
// ReadSample，少了一些内存占用后，再继续下载，
// length: 预设的下载内存缓冲的大小
JUST_FUNC_1(PP_err, JUST_SetDownloadBufferSize, (
    (PP_uint, length)));

//获得播放信息
//返回值: 错误码
//    just_success      表示成功
//    其他数值表示失败
JUST_FUNC_1(PP_err, JUST_GetPlayStat, (
    (JUST_PlayStatistic *, stat)));

//获得下载信息
//返回值: 错误码
//    just_success      表示成功
//    其他数值表示失败
JUST_FUNC_1(PP_err, JUST_GetDataStat, (
    (JUST_DataStat *, stat)));

#endif // _JUST_JUST_I_DEMUX_H_
