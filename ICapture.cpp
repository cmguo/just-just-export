// ICapture.cpp

#include "just/export/Common.h"
#include "just/export/ICapture.h"
#include "just/export/Callback.h"
using namespace just::error;

#include <just/capture/CaptureModule.h>
#include <just/capture/CaptureSource.h>
using namespace just::capture;

#include <just/download/DownloadModule.h>
using namespace just::download;

#include <boost/bind.hpp>
using namespace boost::system;

#ifndef JUST_DISABLE_CAPTURE

namespace just
{

#ifdef JUST_ENABLE_REDIRECT_CALLBACK
    static JUST_CaptureConfigData s_config;
#endif

    class ICapture
    {
    public:
        ICapture()
            : module_(util::daemon::use_module<CaptureModule>(global_daemon()))
            , download_module_(util::daemon::use_module<DownloadModule>(global_daemon()))
        {
        }

        ~ICapture()
        {
        }
        
        void download_finish(
            error_code ec, 
            Downloader * downloader)
        {
            download_module_.close(downloader, ec);
        }

        PP_handle create(
            PP_str name, 
            PP_str dest)
        {
            error_code ec;
            framework::string::Url url(std::string("capture:///") + name);
            CaptureSource* handle = module_.create(url, ec);
            if (handle && dest) {
                framework::string::Url url(dest);
                url.param("url", url.to_string());
                download_module_.open(url, 
                    boost::bind(&ICapture::download_finish, this, _1, _2));
            }
            last_error(__FUNCTION__, ec);
            return (PP_handle)handle;
        }

        PP_err destroy(
            PP_handle handle)
        {
            error_code ec;
            CaptureSource * capture = (CaptureSource *)handle;
            capture->term();
            module_.destroy(capture, ec);
            return last_error(__FUNCTION__, ec);
        }

#ifdef JUST_ENABLE_REDIRECT_CALLBACK
        static bool redirect_get_sample_buffers(
            PP_context context, 
            JUST_ConstBuffer * buffers)
        {
            return redirect_call().call(s_config.get_sample_buffers, context, buffers);
        }

        static bool redirect_free_sample(
            PP_context context)
        {
            return redirect_call().call(s_config.free_sample, context);
        }
#endif

        PP_err init(
            PP_handle handle, 
            JUST_CaptureConfigData const & config)
        {
            error_code ec;
            CaptureSource * capture = (CaptureSource *)handle;
            CaptureConfigData data;
            data.stream_count = config.stream_count;
            data.flags = config.flags;
#ifndef JUST_ENABLE_REDIRECT_CALLBACK
            data.get_sample_buffers = (bool (*)(void const *, CaptureBuffer *))config.get_sample_buffers;
            data.free_sample = config.free_sample;
#else
            s_config = config;
            data.get_sample_buffers = config.get_sample_buffers ? (bool (*)(void const *, CaptureBuffer *))redirect_get_sample_buffers : NULL;
            data.free_sample = config.free_sample ? redirect_free_sample : NULL;
#endif
            capture->init(data, ec);
            return last_error(__FUNCTION__, ec);
        }

        PP_err set_stream(
            PP_handle handle, 
            PP_uint index, 
		    JUST_StreamInfo const & stream)
        {
            error_code ec;
            CaptureSource * capture = (CaptureSource *)handle;
            CaptureStream data;
            data.type = stream.type;
            data.sub_type = stream.sub_type;
            data.index = index;
            data.time_scale = stream.time_scale;
            data.bitrate = stream.bitrate;
            data.format_type = stream.format_type;
            switch (stream.type)
            {
            case JUST_StreamType::VIDE:
                data.video_format.width = stream.format.video.width;
                data.video_format.height = stream.format.video.height;
                data.video_format.frame_rate(stream.format.video.frame_rate_num, stream.format.video.frame_rate_den);
                break;
            case JUST_StreamType::AUDI:
                data.audio_format.channel_count = stream.format.audio.channel_count;
                data.audio_format.sample_size = stream.format.audio.sample_size;
                data.audio_format.sample_rate = stream.format.audio.sample_rate;
                data.audio_format.block_align = stream.format.audio.block_align;
                data.audio_format.sample_per_frame = stream.format.audio.sample_per_frame;
                break;
            default:
                break;
            }
            data.format_data.assign(stream.format_buffer, stream.format_buffer + stream.format_size);
            capture->set_stream(data, ec);
            return last_error(__FUNCTION__, ec);
        }

        PP_err put_sample(
            PP_handle handle, 
		    JUST_Sample const & sample)
        {
            error_code ec;
            CaptureSource * capture = (CaptureSource *)handle;
            CaptureSample data;
            data.itrack = sample.itrack;
            data.flags = sample.flags;
            data.dts = sample.decode_time;
            data.cts_delta = sample.composite_time_delta;
            data.duration = sample.duration;
            data.size = sample.size;
            data.buffer = sample.buffer;
            data.context = sample.context;
            capture->put_sample(data, ec);
            return last_error(__FUNCTION__, ec);
        }

        static error::errors last_error(
            char const * title, 
            error_code const & ec)
        {
            just::error::last_error(ec);
            return async_last_error(title, ec);
        }

        static error::errors async_last_error(
            char const * title, 
            error_code const & ec) 
        {
            if (ec && ec != boost::asio::error::would_block) {
                std::cout << title << ": " << ec.message() << std::endl;
            }
            return just::error::last_error_enum(ec);
        }

    private:
        CaptureModule & module_;
        DownloadModule & download_module_;
    };

}

static just::ICapture & capture()
{
    static just::ICapture the_capture;
    return the_capture;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus


    //打开一个下载用例
    JUST_DECL PP_handle JUST_CaptureCreate(
        PP_str name, 
        PP_str dest)
    {
        return capture().create(name, dest);
    }

    // 获取指定下载用例的实时统计信息
    JUST_DECL PP_err JUST_CaptureInit(
        PP_handle handle,
        JUST_CaptureConfigData const * config)
    {
        return capture().init(handle, *config);
    }

    JUST_DECL PP_err JUST_CaptureSetStream(
        PP_handle handle, 
        PP_uint index, 
		JUST_StreamInfo const * stream)
    {
        return capture().set_stream(handle, index, *stream);
    }

    JUST_DECL PP_err JUST_CapturePutSample(
        PP_handle handle, 
		JUST_Sample const * sample)
    {
        return capture().put_sample(handle, *sample);
    }

    //关闭指定的下载用例
    JUST_DECL PP_err JUST_CaptureDestroy(
        PP_handle handle)
    {
        return capture().destroy(handle);
    }


#if __cplusplus
}
#endif // __cplusplus

#endif

