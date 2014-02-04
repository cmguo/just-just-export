// IDispatch.cpp

#include "ppbox/ppbox/Common.h"
#include "ppbox/ppbox/Callback.h"

#include <ppbox/dispatch/DispatchModule.h>
#include <ppbox/dispatch/DispatcherBase.h>
using namespace ppbox::dispatch;

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>
#include <framework/logger/Section.h>
#include <framework/system/LogicError.h>
#include <util/buffers/BufferCopy.h>
using namespace framework::timer;

#include <boost/bind.hpp>
#include <boost/thread.hpp>
using namespace boost::system;

extern void pool_dump();

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.IDispatch", framework::logger::Debug);

namespace ppbox
{

    class IDispatch
    {
    private:
        struct Cache
        {
            Cache()
                : dispatcher(NULL)
            {
            }

            boost::uint8_t const * copy_sample_data()
            {
                if (sample.data.size() == 1) {
                    return boost::asio::buffer_cast<boost::uint8_t const *>(sample.data[0]);
                } else {
                    if (sample_buffer.size() < sample.size)
                        sample_buffer.resize(sample.size);
                    util::buffers::buffer_copy(boost::asio::buffer(sample_buffer), sample.data);
                    return sample_buffer.size() ? &sample_buffer.at(0) : NULL;
                }
            }

            DispatcherBase * dispatcher;
            MediaInfo media_info;
            std::vector<StreamInfo> stream_infos;
            Sample sample;
            std::vector<boost::uint8_t> sample_buffer;
        };

    public:
        IDispatch()
            : module_(util::daemon::use_module<DispatchModule>(global_daemon()))
        {
        }

        ~IDispatch()
        {
        }

        PP_handle create()
        {
            DispatcherBase * dispatcher = module_.alloc_dispatcher(false);
            if (dispatcher == NULL) {
                return NULL;
            }
            Cache * cache = new Cache;
            cache->dispatcher = dispatcher;
            return cache;
        }

        PP_err open(
            PP_handle handle, 
            PP_str playlink, 
            PP_str format)
        {
            LOG_SECTION();
            LOG_INFO("open playlink: " << playlink);

            Cache * cache = (Cache *)handle;
            error_code ec;
            //cache->dispatcher->o
            return last_error(__FUNCTION__, ec);
        }

#ifdef PPBOX_ENABLE_REDIRECT_CALLBACK
        static void redirect_open_callback(
            PPBOX_Callback callback, 
            PP_context ctx, 
            PP_err ec)
        {
            redirect_call().call(callback, ctx, ec);
        }
#endif

        PP_err async_open(
            PP_handle handle, 
            PP_str playlink, 
            PP_str format, 
			PP_context ctx, 
            PPBOX_Callback callback)
        {
            LOG_SECTION();
            LOG_INFO("async_open playlink: " << playlink);

            framework::string::Url url(std::string("disp:///play?") + format);
            url.param("playlink", playlink);
            Cache * cache = (Cache *)handle;
            cache->dispatcher->async_open((url), 
                boost::bind(&IDispatch::handle_async_open, this, ctx, callback, _1));
            return ppbox_success;
        }

        void handle_async_open(
            PP_context ctx, 
            PPBOX_Callback callback, 
            boost::system::error_code const & ec)
        {
#ifndef PPBOX_ENABLE_REDIRECT_CALLBACK
            callback(ctx, async_last_error(__FUNCTION__, ec));
#else
            redirect_open_callback(callback, ctx, async_last_error(__FUNCTION__, ec));
#endif
        }

        PP_err get_media_info(
            PP_handle handle, 
            PPBOX_MediaInfo & info)
        {
            LOG_SECTION();
            LOG_INFO("get_media_info");

            Cache * cache = (Cache *)handle;
            error_code ec;
            if (cache->dispatcher->get_media_info(cache->media_info, ec)) {
                info.file_size = cache->media_info.file_size;
                info.duration = cache->media_info.duration;
                info.bitrate = cache->media_info.bitrate;
                info.format = cache->media_info.format.c_str();
                info.format_data = cache->media_info.format_data.c_str();
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_stream_info(
            PP_handle handle, 
            PP_uint & num,
            PPBOX_StreamInfo * info)
        {
            LOG_SECTION();
            LOG_INFO("get_stream_info");

            Cache * cache = (Cache *)handle;
            error_code ec;
            if (cache->dispatcher->get_stream_info(cache->stream_infos, ec)) {
                for (size_t i = 0; i < cache->stream_infos.size() && i < num; ++i) {
                    PPBOX_StreamInfo & info1 = info[i];
                    StreamInfo const & info2 = cache->stream_infos[i];
                    info1.type = info2.type;
                    info1.sub_type = info2.sub_type;
                    info1.time_scale = info2.time_scale;
                    info1.bitrate = info2.bitrate;
                    info1.format_type = info2.format_type;
                    if (info2.type == PPBOX_StreamType::VIDE) {
                        info1.format.video.width = info2.video_format.width;
                        info1.format.video.height = info2.video_format.height;
                        info1.format.video.frame_rate_num = info2.video_format.frame_rate_num;
                        info1.format.video.frame_rate_den = info2.video_format.frame_rate_den;
                    } else if (info2.type == PPBOX_StreamType::AUDI) {
                        info1.format.audio.channel_count = info2.audio_format.channel_count;
                        info1.format.audio.sample_size = info2.audio_format.sample_size;
                        info1.format.audio.sample_rate = info2.audio_format.sample_rate;
                        info1.format.audio.block_align = info2.audio_format.block_align;
                        info1.format.audio.sample_per_frame = info2.audio_format.sample_per_frame;
                    }
                    info1.format_size = info2.format_data.size();
                    info1.format_buffer = info1.format_size ? &info2.format_data.at(0) : NULL;
                }
                num = cache->stream_infos.size();
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err seek(
            PP_handle handle, 
            PP_uint type, 
            PP_ulong pos)
        {
            LOG_SECTION();
            LOG_INFO("seek type: " << type << " pos: " << pos);

            Cache * cache = (Cache *)handle;
            SeekRange range;
            range.type = (SeekRange::TypeEnum)type;
            range.beg = pos;
            error_code ec;
            cache->dispatcher->free(cache->sample, ec);
            cache->dispatcher->seek(range, ec);
            return last_error(__FUNCTION__, ec);
        }

        PP_err pause(
            PP_handle handle)
        {
            LOG_SECTION();
            LOG_INFO("pause");

            Cache * cache = (Cache *)handle;
            error_code ec;
            cache->dispatcher->pause(ec);
            return last_error(__FUNCTION__, ec);
        }

        PP_err resume(
            PP_handle handle)
        {
            LOG_SECTION();
            LOG_INFO("resume");

            Cache * cache = (Cache *)handle;
            error_code ec;
            cache->dispatcher->resume(ec);
            return last_error(__FUNCTION__, ec);
        }

        PP_err read(
            PP_handle handle, 
            PPBOX_Sample & sample)
        {
            Cache * cache = (Cache *)handle;
            error_code ec;
            if (cache->dispatcher->read(cache->sample, ec)) {
                sample.itrack = cache->sample.itrack;
                sample.flags = cache->sample.flags;
                sample.time = cache->sample.time;
                sample.decode_time = cache->sample.dts;
                sample.composite_time_delta = cache->sample.cts_delta;
                sample.duration = cache->sample.duration;
                sample.size = cache->sample.size;
                sample.buffer = cache->copy_sample_data();
                sample.context = cache->sample.context;
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_stream_status(
            PP_handle handle, 
            PPBOX_StreamStatus & stat)
        {
            Cache * cache = (Cache *)handle;
            ppbox::data::StreamStatus status;
            error_code ec;
            if (cache->dispatcher->get_stream_status(status, ec)) {
                stat.video_index = status.video_index;
                stat.audio_index = status.audio_index;
                stat.byte_pos = status.byte_range.pos;
                stat.byte_buf = status.byte_range.buf;
                stat.time_pos = status.time_range.pos;
                stat.time_buf = status.time_range.buf;
                stat.data_err = ppbox::error::last_error_enum(status.buf_ec);
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_data_stat(
            PP_handle handle, 
            PPBOX_DataStat & stat)
        {
            Cache * cache = (Cache *)handle;
            DataStat stat2;
            error_code ec;
            if (cache->dispatcher->get_data_stat(stat2, ec)) {
                stat.total_elapse = (boost::uint32_t)(time(NULL) - stat2.start_time);
                stat.total_download_bytes = (boost::uint32_t)stat2.total_bytes;
                stat.connection_status = stat2.connection_status;
                stat.average_speed_one_second = stat2.speeds[0].cur_speed;
                stat.average_speed_five_seconds = stat2.speeds[1].cur_speed;
                stat.average_speed_twenty_seconds = stat2.speeds[2].cur_speed;
                stat.average_speed_sixty_seconds = stat2.speeds[3].cur_speed;
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err close(
            PP_handle handle)
        {
            LOG_SECTION();
            LOG_INFO("close");

            Cache * cache = (Cache *)handle;
            error_code ec;
            if (!cache->sample.data.empty()) {
                cache->dispatcher->free(cache->sample, ec);
            }
            cache->dispatcher->close(ec);
            return last_error(__FUNCTION__, ec);
        }

        PP_err destroy(
            PP_handle handle)
        {
            Cache * cache = (Cache *)handle;
            delete cache->dispatcher;
            delete cache;
            return ppbox_success;
        }

        static error::errors async_last_error(
            PP_str log_title, 
            error_code const & ec)
        {
            if (ec && ec != boost::asio::error::would_block) {
                LOG_WARN(log_title << ": " << ec.message());
            }
            return ppbox::error::last_error_enum(ec);
        }

        static error::errors last_error(
            PP_str title, 
            error_code const & ec)
        {
            ppbox::error::last_error(ec);
            return async_last_error(title, ec);
        }

    private:
        DispatchModule & module_;
        boost::uint32_t buffer_time_;
        boost::mutex mutex_;
    };
}

static ppbox::IDispatch & dispatch()
{
    static ppbox::IDispatch the_dispatch;
    return the_dispatch;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus

    PPBOX_DECL PP_handle PPBOX_DispatchCreate()
    {
        return dispatch().create();
    }

    PPBOX_DECL PP_err PPBOX_DispatchOpen(
        PP_handle handle, 
        PP_str playlink, 
        PP_str format)
    {
        return dispatch().open(handle, playlink, format);
    }

    PPBOX_DECL PP_err PPBOX_DispatchAsyncOpen(
        PP_handle handle, 
        PP_str playlink, 
        PP_str format, 
        PP_context ctx, 
        PPBOX_Callback callback)
    {
        return dispatch().async_open(handle, playlink, format, ctx, callback);
    }

    PPBOX_DECL PP_err PPBOX_DispatchGetMediaInfo(
        PP_handle handle, 
        PPBOX_MediaInfo * info)
    {
        return dispatch().get_media_info(handle, *info);
    }

    PPBOX_DECL PP_err PPBOX_DispatchGetStreamInfo(
        PP_handle handle, 
        PP_uint * num, 
        PPBOX_StreamInfo * info)
    {
        return dispatch().get_stream_info(handle, *num, info);
    }

    PPBOX_DECL PP_err PPBOX_DispatchSeek(
        PP_handle handle, 
        PP_uint type, 
        PP_ulong pos)
    {
        return dispatch().seek(handle, type, pos);
    }

    PPBOX_DECL PP_err PPBOX_DispatchRead(
        PP_handle handle, 
        PPBOX_Sample * sample)
    {
        return dispatch().read(handle, *sample);
    }

    PPBOX_DECL PP_err PPBOX_DispatchPause(
        PP_handle handle)
    {
        return dispatch().pause(handle);
    }

    PPBOX_DECL PP_err PPBOX_DispatchResume(
        PP_handle handle)
    {
        return dispatch().resume(handle);
    }

    PPBOX_DECL PP_err PPBOX_DispatchClose(
        PP_handle handle)
    {
        return dispatch().close(handle);
    }

    PPBOX_DECL PP_err PPBOX_DispatchGetStreamStatus(
        PP_handle handle, 
        PPBOX_StreamStatus * status)
    {
        return dispatch().get_stream_status(handle, *status);
    }

    PPBOX_DECL PP_err PPBOX_DispatchGetDataStat(
        PP_handle handle, 
        PPBOX_DataStat * stat)
    {
        return dispatch().get_data_stat(handle, *stat);
    }

    PPBOX_DECL PP_err PPBOX_DispatchDestroy(
        PP_handle handle)
    {
        return dispatch().destroy(handle);
    }

#if __cplusplus
}
#endif // __cplusplus
