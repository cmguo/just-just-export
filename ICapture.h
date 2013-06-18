// IDemuxer.h

#ifndef _PPBOX_PPBOX_I_CAPTURE_H_
#define _PPBOX_PPBOX_I_CAPTURE_H_

#include "IPpbox.h"
#include "IFormat.h"

#include <ppbox/capture/CaptureFormat.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

    typedef PP_bool (*PPBOX_GetSampleBuffersCallBack)(PP_context, PPBOX_SampleBuffer *);
    typedef PP_bool (*PPBOX_FreeSampleCallBack)(PP_context);

    struct PPBOX_CaptureConfigFlag
    {
        enum Enum
        {
            stream_ordered = ppbox::capture::CaptureConfigData::f_stream_ordered, 
            multi_thread = ppbox::capture::CaptureConfigData::f_multi_thread, 
        };
    };

    struct PPBOX_CaptureConfigData
    {
        PP_uint stream_count;
        PP_uint flags;
        PPBOX_GetSampleBuffersCallBack get_sample_buffers;
        PPBOX_FreeSampleCallBack free_sample;
    };

    //打开一个视频
    PPBOX_DECL PP_handle PPBOX_CaptureCreate(
        PP_str name, 
        PP_str dest);

    PPBOX_DECL PP_err PPBOX_CaptureInit(
        PP_handle handle, 
        PPBOX_CaptureConfigData const * config);

    PPBOX_DECL PP_err PPBOX_CaptureSetStream(
        PP_handle handle, 
        PP_uint index, 
		PPBOX_StreamInfo const * stream);

    PPBOX_DECL PP_err PPBOX_CapturePutSample(
        PP_handle handle, 
        PPBOX_Sample const * sample);

    PPBOX_DECL PP_err PPBOX_CaptureDestroy(
        PP_handle handle);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_CAPTURE_H_
