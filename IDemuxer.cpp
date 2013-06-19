// IDemuxer.cpp

#include "ppbox/ppbox/Common.h"
#define PPBOX_SOURCE
#include "ppbox/ppbox/IDemuxer.h"
#include "ppbox/ppbox/Callback.h"

#include <ppbox/demux/DemuxModule.h>
#include <ppbox/demux/base/DemuxerBase.h>
#include <ppbox/demux/base/DemuxError.h>
#include <ppbox/mux/MuxModule.h>
#include <ppbox/mux/MuxerBase.h>
#include <ppbox/data/base/SourceError.h>
#include <ppbox/data/base/DataStatistic.h>
using namespace ppbox::data;
using namespace ppbox::demux;
using namespace ppbox::mux;
using namespace ppbox::avbase;

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

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.IDemuxer", framework::logger::Debug);

namespace ppbox
{

    class IDemuxer
    {
    private:
        struct Cache
        {
            Cache()
                : close_token(0)
                , demuxer(NULL)
                , muxer(NULL)
                , paused(false)
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

            size_t close_token;
            DemuxerBase * demuxer;
            MuxerBase * muxer;
            std::vector<StreamInfo> stream_infos;
            Sample sample;
            bool paused;
            std::vector<boost::uint8_t> sample_buffer;
        };

    public:
        IDemuxer()
            : demux_mod_(util::daemon::use_module<DemuxModule>(global_daemon()))
            , mux_mod_(util::daemon::use_module<MuxModule>(global_daemon()))
            , buffer_time_(3000)
        {
        }

        ~IDemuxer()
        {
        }

        PP_err open(
            PP_str playlink)
        {
            return open_ex(playlink, "format=raw");
        }

        PP_err open_ex(
            PP_str playlink, 
            PP_str format)
        {
            LOG_SECTION();
            LOG_INFO("open playlink: " << playlink << ", format: " << format);

            error_code ec;
            if (cache_) {
                ec = ppbox::error::already_open;
            } else {
                boost::shared_ptr<Cache> cache(new Cache);
                cache_ = cache;
                framework::string::Url play_link(playlink);
                framework::string::Url config(std::string("config:///interface?") + format);
                cache->demuxer = demux_mod_.open(play_link, config, cache->close_token, ec);
                if (!ec) {
                    framework::string::Url play_link(playlink);
                    cache->muxer = mux_mod_.open(cache->demuxer, config, ec);
                    cache->muxer->stream_info(cache->stream_infos);
                }
            }
            return last_error(__FUNCTION__, ec);
        }

#ifdef PPBOX_ENABLE_REDIRECT_CALLBACK
        static void redirect_open_callback(
            PPBOX_Open_Callback callback, 
            PP_err ec)
        {
            redirect_call().call(callback, ec);
        }
#endif

        PP_err async_open(
            PP_str playlink, 
            PPBOX_Open_Callback callback)
        {
            return async_open(playlink, "format=raw", 
#ifndef PPBOX_ENABLE_REDIRECT_CALLBACK
                boost::bind(callback, _1));
#else
                boost::bind(redirect_open_callback, callback, _1));
#endif
        }

#ifdef PPBOX_ENABLE_REDIRECT_CALLBACK
        static void redirect_open_ex_callback(
            PPBOX_Callback callback, 
            PP_context user_data, 
            PP_err ec)
        {
            redirect_call().call(callback, user_data, ec);
        }
#endif

        PP_err async_open_ex(
            PP_str playlink, 
            PP_str format, 
			PP_context user_data, 
            PPBOX_Callback callback)
        {
            return async_open(playlink, format, 
#ifndef PPBOX_ENABLE_REDIRECT_CALLBACK
                boost::bind(callback, user_data, _1));
#else
                boost::bind(redirect_open_ex_callback, callback, user_data, _1));
#endif
        }

        PP_err async_open(
            PP_str playlink, 
            PP_str format, 
            boost::function<void (error::errors)> const & callback)
        {
            LOG_SECTION();
            LOG_INFO("async_open playlink: " << playlink << ", format: " << format);

            error_code ec;
            if (cache_) {
                ec = ppbox::error::already_open;
            } else {
                cache_.reset(new Cache);
                framework::string::Url play_link(playlink);
                framework::string::Url config(std::string("config:///interface?") + format);
                demux_mod_.async_open(play_link, config, cache_->close_token, 
                    boost::bind(&IDemuxer::open_call_back, this, cache_, config, callback, _1, _2));
            }
            if (ec) {
				global_daemon().io_svc().post(
					boost::bind(callback, async_last_error(__FUNCTION__, ec)));
            }
            return ppbox_success;
        }

        void open_call_back(
            boost::shared_ptr<Cache> & cache, 
            framework::string::Url const & config, 
            boost::function<void (error::errors)> const & callback, 
            error_code ec, 
            DemuxerBase * demuxer)
        {
            cache->demuxer = demuxer;
            if (!ec) {
                cache->muxer = mux_mod_.open(cache->demuxer, config, ec);
                cache->muxer->stream_info(cache->stream_infos);
            }
            callback(async_last_error(__FUNCTION__, ec));
        }

        bool is_open(
            error_code & ec) const
        {
            if (cache_ && cache_->demuxer) {
                ec.clear();
                return true;
            } else {
                ec = ppbox::error::not_open;
                return false;
            }
        }

        PP_err seek(
            boost::uint64_t time)
        {
            LOG_SECTION();
            LOG_INFO("seek time: " << time);

            error_code ec;
            if (is_open(ec)) {
                cache_->demuxer->free_sample(cache_->sample, ec);
                cache_->muxer->time_seek(time, ec);
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err pause()
        {
            LOG_SECTION();
            LOG_INFO("pause");

            error_code ec;
            if (is_open(ec)) {
                cache_->demuxer->pause(ec);
                if (!ec)
                    cache_->paused = true;
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err resume()
        {
            LOG_SECTION();
            LOG_INFO("resume");

            error_code ec;
            if (is_open(ec)) {
                cache_->demuxer->resume(ec);
                if (!ec)
                    cache_->paused = false;
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err close()
        {
            LOG_SECTION();
            LOG_INFO("close");

            error_code ec;
            if (cache_) {
                while (true) {
                    if (cache_->muxer) {
                        mux_mod_.close(cache_->muxer, ec);
                    }
                    if (cache_->demuxer) {
                        cache_->demuxer->free_sample(cache_->sample, ec);
                    }
                    demux_mod_.close(cache_->close_token, ec);
                    if (ec == framework::system::logic_error::item_not_exist) {
                        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
                    } else {
                        break;
                    }
                }
                cache_->demuxer = NULL;
                cache_.reset();
            } else {
                ec = ppbox::error::not_open;
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_stream_count(
            PP_uint & count)
        {
            error_code ec;
            if (is_open(ec)) {
                count = cache_->stream_infos.size();
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_stream_info(
            PP_uint index,
            PPBOX_StreamInfo & info)
        {
            error_code ec;
            if (!is_open(ec)) {
            } else if(index >= cache_->stream_infos.size()) {
                ec = framework::system::logic_error::out_of_range;
            } else {
                StreamInfo const & info2 = cache_->stream_infos[index];
                info.type = info2.type;
                info.sub_type = info2.sub_type;
                info.time_scale = info2.time_scale;
                info.bitrate = info2.bitrate;
                info.format_type = info2.format_type;
                if (info2.type == StreamType::VIDE) {
                    info.video_format.width = info2.video_format.width;
                    info.video_format.height = info2.video_format.height;
                    info.video_format.frame_rate_num = info2.video_format.frame_rate_num;
                    info.video_format.frame_rate_den = info2.video_format.frame_rate_den;
                } else if (info2.type == StreamType::AUDI) {
                    info.audio_format.channel_count = info2.audio_format.channel_count;
                    info.audio_format.sample_size = info2.audio_format.sample_size;
                    info.audio_format.sample_rate = info2.audio_format.sample_rate;
                    info.audio_format.sample_per_frame = info2.audio_format.sample_per_frame;
                }
                info.format_size = info2.format_data.size();
                info.format_buffer = info.format_size ? &info2.format_data.at(0) : NULL;
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_duration(
            PP_uint & duration)
        {
            error_code ec;
            if (is_open(ec)) {
                ppbox::data::MediaInfo info;
                cache_->demuxer->get_media_info(info, ec);
                duration = ec ? 0 : (boost::uint32_t)info.duration;
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err read_sample(
            PPBOX_Sample & sample)
        {
            error_code ec;
            if (is_open(ec)) {
                if (cache_->paused) {
                    cache_->paused = false;
                }
                if (cache_->muxer->read(cache_->sample, ec)) {
                    sample.itrack = cache_->sample.itrack;
                    sample.flags = cache_->sample.flags;
                    sample.time = cache_->sample.time;
                    sample.decode_time = cache_->sample.dts;
                    sample.composite_time_delta = cache_->sample.cts_delta;
                    sample.duration = cache_->sample.duration;
                    sample.size = cache_->sample.size;
                    sample.buffer = cache_->copy_sample_data();
                    sample.context = cache_->sample.context;
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_play_stat(
            PPBOX_PlayStatistic & stat)
        {
            error_code ec;
            memset(&stat, 0, sizeof(stat));
            stat.length = sizeof(stat);
            if (!is_open(ec)) {
                stat.buffer_time = 0;
                stat.buffering_present = 0;
                if (ec == boost::asio::error::would_block) {
                    stat.play_status = cache_->paused ? PPBOX_PlayStatus::paused : PPBOX_PlayStatus::buffering;
                } else {
                    stat.play_status = PPBOX_PlayStatus::closed;
                }
            } else {
                cache_->demuxer->fill_data(ec);
                ppbox::data::StreamStatus status;
                cache_->demuxer->get_stream_status(status, ec);
                if (ec && ec != boost::asio::error::would_block) {
                    stat.play_status = PPBOX_PlayStatus::closed;
                } else {
                    stat.buffer_time = (boost::uint32_t)(status.time_range.buf - status.time_range.pos);
                    if (stat.buffer_time >= buffer_time_ || status.buf_ec == ppbox::data::source_error::no_more_segment) {
                        stat.buffering_present = 100;
                        stat.play_status = PPBOX_PlayStatus::playing;
                    } else if (buffer_time_ != 0) {
                        stat.buffering_present = stat.buffer_time * 100 / buffer_time_;
                        stat.play_status = PPBOX_PlayStatus::buffering;
                    }
                    if (cache_->paused) {
                        stat.play_status = PPBOX_PlayStatus::paused;
                    }
                    // �ڻ���״̬������Ĵ�������вο�����
                    if (stat.play_status == PPBOX_PlayStatus::buffering) {
                        ec = status.buf_ec;
                    }
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_download_stat(
            PPBOX_DownloadMsg & stat)
        {
            error_code ec;
            stat.length = sizeof(stat);
            if (is_open(ec)) {
                DataStatistic data_stat;
                cache_->demuxer->get_data_stat(data_stat, ec);
                memset(&stat, 0, sizeof(stat));
                stat.length = sizeof(stat);
                stat.start_time = (boost::uint32_t)data_stat.start_time;
                stat.total_download_bytes = (boost::uint32_t)data_stat.total_bytes;
                stat.http_download_bytes = (boost::uint32_t)data_stat.total_bytes;
                stat.download_duration_in_sec = (boost::uint32_t)(time(NULL) - data_stat.start_time);
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_download_speed(
            PPBOX_DownloadSpeedMsg & stat)
        {
            error_code ec;
            if (is_open(ec)) {
                DataStatistic data_stat;
                cache_->demuxer->get_data_stat(data_stat, ec);
                memset(&stat, 0, sizeof(stat));
                stat.length = sizeof(stat);
                time_t now = time(NULL);
                if (now > data_stat.start_time) {
                    stat.avg_download_speed = (boost::uint32_t)(data_stat.total_bytes / (now - data_stat.start_time));
                }
                stat.second_download_speed = data_stat.speeds[0].cur_speed;
                stat.recent_download_speed = data_stat.speeds[1].cur_speed;
                stat.now_download_speed = data_stat.speeds[2].cur_speed;
                stat.minute_download_speed = data_stat.speeds[3].cur_speed;
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err set_download_buffer_size(
            PP_uint buffer_size)
        {
            error_code ec;
            demux_mod_.set_download_buffer_size(buffer_size);
            return last_error(__FUNCTION__, ec);
        }

        PP_err set_play_buffer_time(
            PP_uint buffer_time)
        {
            error_code ec;
            buffer_time_ = buffer_time;
            return last_error(__FUNCTION__, ec);
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
        DemuxModule & demux_mod_;
        MuxModule & mux_mod_;
        boost::uint32_t buffer_time_;
        boost::shared_ptr<Cache> cache_;
    };
}

static ppbox::IDemuxer & demuxer()
{
    static ppbox::IDemuxer the_demuxer;
    return the_demuxer;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus

    PPBOX_DECL PP_err PPBOX_Open(
        PP_str playlink)
    {
        return demuxer().open(playlink);
    }

    PPBOX_DECL PP_err PPBOX_AsyncOpen(
        PP_str playlink, 
        PPBOX_Open_Callback callback)
    {
        return demuxer().async_open(playlink, callback);
    }

    PPBOX_DECL PP_err PPBOX_OpenEx(
        PP_str playlink, 
        PP_str format)
    {
        return demuxer().open_ex(playlink, format);
    }

    PPBOX_DECL PP_err PPBOX_AsyncOpenEx(
        PP_str playlink, 
        PP_str format, 
		PP_context user_data, 
        PPBOX_Callback callback)
    {
        return demuxer().async_open_ex(playlink, format, user_data, callback);
    }

    PPBOX_DECL PP_uint PPBOX_GetStreamCount()
    {
        PP_uint n = 0;
        demuxer().get_stream_count(n);
        return n;
    }

    PPBOX_DECL PP_err PPBOX_GetStreamInfo(
        PP_uint index, 
        PPBOX_StreamInfo * stream_info)
    {
        return demuxer().get_stream_info(index, *stream_info);
    }

    PPBOX_DECL PP_uint PPBOX_GetDuration()
    {
        PP_uint n = 0;
        demuxer().get_duration(n);
        return n;
    }

    PPBOX_DECL PP_err PPBOX_Seek(
        PP_uint start_time)
    {
        return demuxer().seek(start_time);
    }

    PPBOX_DECL PP_err PPBOX_ReadSample(
        PPBOX_Sample * sample)
    {
        return demuxer().read_sample(*sample);
    }

    PPBOX_DECL PP_err PPBOX_Close()
    {
        return demuxer().close();
    }

    PPBOX_DECL PP_err PPBOX_Pause()
    {
        return demuxer().pause();
    }

    PPBOX_DECL PP_err PPBOX_SetDownloadBufferSize(
        PP_uint length)
    {
        return demuxer().set_download_buffer_size(length);
    }

    PPBOX_DECL PP_err PPBOX_SetPlayBufferTime(
        PP_uint time)
    {
        return demuxer().set_play_buffer_time(time);
    }

    PPBOX_DECL PP_err PPBOX_GetPlayMsg(
        PPBOX_PlayStatistic * statistic_Msg)
    {
        return demuxer().get_play_stat(*statistic_Msg);
    }

    PPBOX_DECL PP_err PPBOX_GetDownMsg(
        PPBOX_DownloadMsg* download_Msg)
    {
        return demuxer().get_download_stat(*download_Msg);
    }

    PPBOX_DECL PP_err PPBOX_GetDownSedMsg(
        PPBOX_DownloadSpeedMsg* download_spped_Msg)
    {
        return demuxer().get_download_speed(*download_spped_Msg);
    }

#if __cplusplus
}
#endif // __cplusplus
