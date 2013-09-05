// IDispatch.h

#ifndef _PPBOX_PPBOX_I_DISPATCH_H_
#define _PPBOX_PPBOX_I_DISPATCH_H_

#include "IPpbox.h"
#include "IFormat.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    PPBOX_FUNC_0(PP_handle, PPBOX_DispatchCreate, ());

    PPBOX_FUNC_3(PP_err, PPBOX_DispatchOpen, (
        (PP_handle, handle), 
        (PP_str, playlink), 
        (PP_str, format)));

    PPBOX_FUNC_5(PP_err, PPBOX_DispatchAsyncOpen, (
        (PP_handle, handle), 
        (PP_str, playlink), 
        (PP_str, format), 
        (PP_context, ctx), 
        (PPBOX_Callback, callback)));

    PPBOX_FUNC_2(PP_err, PPBOX_DispatchGetMediaInfo, (
        (PP_handle, handle), 
        (PPBOX_MediaInfo *, info)));

    PPBOX_FUNC_3(PP_err, PPBOX_DispatchGetStreamInfo, (
        (PP_handle, handle), 
        (PP_uint *, num), 
        (PPBOX_StreamInfo *, infos)));

    PPBOX_FUNC_1(PP_err, PPBOX_DispatchPause, (
        (PP_handle, handle)));

    PPBOX_FUNC_1(PP_err, PPBOX_DispatchResume, (
        (PP_handle, handle)));

    PPBOX_FUNC_3(PP_err, PPBOX_DispatchSeek, (
        (PP_handle, handle), 
        (PP_uint, type), 
        (PP_ulong, pos)));

    PPBOX_FUNC_2(PP_err, PPBOX_DispatchRead, (
        (PP_handle, handle), 
        (PPBOX_Sample *, sample)));

    PPBOX_FUNC_2(PP_err, PPBOX_DispatchGetStreamStatus, (
        (PP_handle, handle), 
        (PPBOX_StreamStatus *, status)));

    PPBOX_FUNC_2(PP_err, PPBOX_DispatchGetDataStat, (
        (PP_handle, handle), 
        (PPBOX_DataStat *, status)));

    PPBOX_FUNC_1(PP_err, PPBOX_DispatchClose, (
        (PP_handle, handle)));

    PPBOX_FUNC_1(PP_err, PPBOX_DispatchDestroy, (
        (PP_handle, handle)));

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DISPATCH_H_
