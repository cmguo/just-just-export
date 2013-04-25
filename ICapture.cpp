// ICapture.cpp

#include "ppbox/ppbox/Common.h"
#define PPBOX_SOURCE
#include "ppbox/ppbox/ICapture.h"
using namespace ppbox::error;

#include <ppbox/capture/CaptureModule.h>
#include <ppbox/capture/CaptureSource.h>
using namespace ppbox::capture;

#include <ppbox/download/DownloadModule.h>
using namespace ppbox::download;

#include <boost/bind.hpp>
using namespace boost::system;

#ifndef PPBOX_DISABLE_CAPTURE

namespace ppbox
{

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

        PPBOX_HANDLE create(
            PP_char const * name, 
            PP_char const * dest)
        {
            error_code ec;
            framework::string::Url playlink(std::string("capture:///") + name);
            CaptureSource* handle = module_.create(playlink, ec);
            if (handle && dest) {
                framework::string::Url url(dest);
                playlink.encode();
                url.param("playlink", playlink.to_string());
                download_module_.open(url, 
                    boost::bind(&ICapture::download_finish, this, _1, _2));
            }
            last_error(__FUNCTION__, ec);
            return (PPBOX_HANDLE)handle;
        }

        error::errors destroy(
            PPBOX_HANDLE handle)
        {
            error_code ec;
            CaptureSource * capture = (CaptureSource *)handle;
            module_.destroy(capture, ec);
            return last_error(__FUNCTION__, ec);
        }

        error::errors init(
            PPBOX_HANDLE handle, 
            PPBOX_CaptureConfigData const & config)
        {
            error_code ec;
            CaptureSource * capture = (CaptureSource *)handle;
            CaptureConfigData data;
            data.stream_count = config.stream_count;
            data.get_sample_buffers = (bool (*)(void const *, CaptureBuffer *))config.get_sample_buffers;
            data.free_sample = config.free_sample;
            capture->init(data, ec);
            return last_error(__FUNCTION__, ec);
        }

        error::errors set_stream(
            PPBOX_HANDLE handle, 
            boost::uint32_t index, 
		    PPBOX_CaptureStream const & stream)
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
            case PPBOX_StreamType::VIDE:
                data.video_format.width = stream.video_format.width;
                data.video_format.height = stream.video_format.height;
                data.video_format.frame_rate = stream.video_format.frame_rate;
                break;
            case PPBOX_StreamType::AUDI:
                data.audio_format.sample_rate = stream.audio_format.sample_rate;
                data.audio_format.sample_size = stream.audio_format.sample_size;
                data.audio_format.channel_count = stream.audio_format.channel_count;
                break;
            default:
                break;
            }
            data.format_data.assign(stream.format_buffer, stream.format_buffer + stream.format_size);
            capture->set_stream(data, ec);
            return last_error(__FUNCTION__, ec);
        }

        error::errors put_sample(
            PPBOX_HANDLE handle, 
		    PPBOX_CaptureSample const & sample)
        {
            error_code ec;
            CaptureSource * capture = (CaptureSource *)handle;
            CaptureSample data;
            data.itrack = sample.itrack;
            data.flags = sample.flags;
            data.dts = sample.dts;
            data.cts_delta = sample.cts_delta;
            data.duration = sample.duration;
            data.context = sample.context;
            data.size = sample.size;
            data.cbuf = sample.cbuf;
            capture->put_sample(data, ec);
            return last_error(__FUNCTION__, ec);
        }

        static error::errors last_error(
            char const * title, 
            error_code const & ec)
        {
            ppbox::error::last_error(ec);
            return async_last_error(title, ec);
        }

        static error::errors async_last_error(
            char const * title, 
            error_code const & ec) 
        {
            if (ec && ec != boost::asio::error::would_block) {
                std::cout << title << ": " << ec.message() << std::endl;
            }
            return ppbox::error::last_error_enum(ec);
        }

    private:
        CaptureModule & module_;
        DownloadModule & download_module_;
    };

}

static ppbox::ICapture & capture()
{
    static ppbox::ICapture the_capture;
    return the_capture;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus


    //打开一个下载用例
    PPBOX_DECL PPBOX_HANDLE PPBOX_CaptureCreate(
        PP_char const * name, 
        PP_char const * dest)
    {
        return capture().create(name, dest);
    }

    // 获取指定下载用例的实时统计信息
    PPBOX_DECL PP_int32 PPBOX_CaptureInit(
        PPBOX_HANDLE handle,
        PPBOX_CaptureConfigData const * config)
    {
        return capture().init(handle, *config);
    }

    PPBOX_DECL PP_err PPBOX_CaptureSetStream(
        PPBOX_HANDLE handle, 
        boost::uint32_t index, 
		PPBOX_CaptureStream const * stream)
    {
        return capture().set_stream(handle, index, *stream);
    }

    PPBOX_DECL PP_err PPBOX_CapturePutSample(
        PPBOX_HANDLE handle, 
		PPBOX_CaptureSample const * sample)
    {
        return capture().put_sample(handle, *sample);
    }

    //关闭指定的下载用例
    PPBOX_DECL PP_err PPBOX_CaptureDestroy(
        PPBOX_HANDLE handle)
    {
        return capture().destroy(handle);
    }


#if __cplusplus
}
#endif // __cplusplus

#endif

