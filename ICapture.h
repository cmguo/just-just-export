// IDemuxer.h

#ifndef _JUST_EXPORT_I_CAPTURE_H_
#define _JUST_EXPORT_I_CAPTURE_H_

#include "IGeneric.h"
#include "IFormat.h"

JUST_CALLBACK_2(PP_bool, JUST_GetSampleBuffersCallBack, (PP_context, JUST_ConstBuffer *));
JUST_CALLBACK_1(PP_bool, JUST_FreeSampleCallBack, (PP_context));

enum JUST_CaptureConfigFlagEnum
{
    JUST_CaptureConfigFlag_stream_ordered = 1,  
    JUST_CaptureConfigFlag_multi_thread = 2, 
};

JUST_STRUCT_4(JUST_CaptureConfigData, (
    (PP_uint, stream_count),
    (PP_uint, flags),
    (JUST_GetSampleBuffersCallBack, get_sample_buffers),
    (JUST_FreeSampleCallBack, free_sample)
    ));

//打开一个视频
JUST_FUNC_2(PP_handle, JUST_CaptureCreate, (
    (PP_str, name), 
    (PP_str, dest)));

JUST_FUNC_2(PP_err, JUST_CaptureInit, (
    (PP_handle, handle), 
    (JUST_CaptureConfigData const *, config)));

JUST_FUNC_3(PP_err, JUST_CaptureSetStream, (
    (PP_handle, handle), 
    (PP_uint, index), 
    (JUST_StreamInfo const *, stream)));

JUST_FUNC_2(PP_err, JUST_CapturePutSample, (
    (PP_handle, handle), 
    (JUST_Sample const *, sample)));

JUST_FUNC_1(PP_err, JUST_CaptureDestroy, (
    (PP_handle, handle)));

#endif // _JUST_EXPORT_I_CAPTURE_H_
