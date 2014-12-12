// IDispatch.h

#ifndef _JUST_EXPORT_I_DISPATCH_H_
#define _JUST_EXPORT_I_DISPATCH_H_

#include "IJust.h"
#include "IFormat.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    JUST_FUNC_0(PP_handle, JUST_DispatchCreate, ());

    JUST_FUNC_3(PP_err, JUST_DispatchOpen, (
        (PP_handle, handle), 
        (PP_str, playlink), 
        (PP_str, format)));

    JUST_FUNC_5(PP_err, JUST_DispatchAsyncOpen, (
        (PP_handle, handle), 
        (PP_str, playlink), 
        (PP_str, format), 
        (PP_context, ctx), 
        (JUST_Callback, callback)));

    JUST_FUNC_2(PP_err, JUST_DispatchGetMediaInfo, (
        (PP_handle, handle), 
        (JUST_MediaInfo *, info)));

    JUST_FUNC_3(PP_err, JUST_DispatchGetStreamInfo, (
        (PP_handle, handle), 
        (PP_uint *, num), 
        (JUST_StreamInfo *, infos)));

    JUST_FUNC_1(PP_err, JUST_DispatchPause, (
        (PP_handle, handle)));

    JUST_FUNC_1(PP_err, JUST_DispatchResume, (
        (PP_handle, handle)));

    JUST_FUNC_3(PP_err, JUST_DispatchSeek, (
        (PP_handle, handle), 
        (PP_uint, type), 
        (PP_ulong, pos)));

    JUST_FUNC_2(PP_err, JUST_DispatchRead, (
        (PP_handle, handle), 
        (JUST_Sample *, sample)));

    JUST_FUNC_2(PP_err, JUST_DispatchGetStreamStatus, (
        (PP_handle, handle), 
        (JUST_StreamStatus *, status)));

    JUST_FUNC_2(PP_err, JUST_DispatchGetDataStat, (
        (PP_handle, handle), 
        (JUST_DataStat *, status)));

    JUST_FUNC_1(PP_err, JUST_DispatchClose, (
        (PP_handle, handle)));

    JUST_FUNC_1(PP_err, JUST_DispatchDestroy, (
        (PP_handle, handle)));

#if __cplusplus
}
#endif // __cplusplus

#endif // _JUST_EXPORT_I_DISPATCH_H_
