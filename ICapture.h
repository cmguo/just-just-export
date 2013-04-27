// IDemuxer.h

#ifndef _PPBOX_PPBOX_I_CAPTURE_H_
#define _PPBOX_PPBOX_I_CAPTURE_H_

#include "IPpbox.h"
#include "IFormat.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    struct PPBOX_CaptureConfigData
    {
        PP_uint32 stream_count;
        bool (*get_sample_buffers)(void const *, PPBOX_SampleBuffer *);
        bool (*free_sample)(void const *);
    };

    //打开一个视频
    PPBOX_DECL PPBOX_HANDLE PPBOX_CaptureCreate(
        PP_char const * name, 
        PP_char const * dest);

    PPBOX_DECL PP_err PPBOX_CaptureInit(
        PPBOX_HANDLE handle, 
        PPBOX_CaptureConfigData const * config);

    PPBOX_DECL PP_err PPBOX_CaptureSetStream(
        PPBOX_HANDLE handle, 
        PP_uint32 index, 
		PPBOX_StreamInfo const * stream);

    PPBOX_DECL PP_err PPBOX_CapturePutSample(
        PPBOX_HANDLE handle, 
        PPBOX_Sample const * sample);

    PPBOX_DECL PP_err PPBOX_CaptureDestroy(
        PPBOX_HANDLE handle);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_CAPTURE_H_
