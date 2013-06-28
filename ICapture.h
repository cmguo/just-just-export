// IDemuxer.h

#ifndef _PPBOX_PPBOX_I_CAPTURE_H_
#define _PPBOX_PPBOX_I_CAPTURE_H_

#include "IPpbox.h"
#include "IFormat.h"

PPBOX_CALLBACK_2(bool, PPBOX_GetSampleBuffersCallBack, (PP_context, PPBOX_SampleBuffer *));
PPBOX_CALLBACK_1(bool, PPBOX_FreeSampleCallBack, (PP_context));

enum PPBOX_CaptureConfigFlagEnum
{
    PPBOX_CaptureConfigFlag_stream_ordered = 1,  
    PPBOX_CaptureConfigFlag_multi_thread = 2, 
};

PPBOX_STRUCT_4(PPBOX_CaptureConfigData, (
    (PP_uint, stream_count),
    (PP_uint, flags),
    (PPBOX_GetSampleBuffersCallBack, get_sample_buffers),
    (PPBOX_FreeSampleCallBack, free_sample)
    ));

//打开一个视频
PPBOX_FUNC_2(PP_handle, PPBOX_CaptureCreate, (
    (PP_str, name), 
    (PP_str, dest)));

PPBOX_FUNC_2(PP_err, PPBOX_CaptureInit, (
    (PP_handle, handle), 
    (PPBOX_CaptureConfigData const *, config)));

PPBOX_FUNC_3(PP_err, PPBOX_CaptureSetStream, (
    (PP_handle, handle), 
    (PP_uint, index), 
    (PPBOX_StreamInfo const *, stream)));

PPBOX_FUNC_2(PP_err, PPBOX_CapturePutSample, (
    (PP_handle, handle), 
    (PPBOX_Sample const *, sample)));

PPBOX_FUNC_1(PP_err, PPBOX_CaptureDestroy, (
    (PP_handle, handle)));

#endif // _PPBOX_PPBOX_I_CAPTURE_H_
