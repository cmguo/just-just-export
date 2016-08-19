// IDemux.cpp

#include "just/export/Common.h"
#include "just/export/Callback.h"

#include <just/demux/DemuxModule.h>
#include <just/demux/base/DemuxerBase.h>
#include <just/demux/base/DemuxError.h>
#include <just/mux/MuxModule.h>
#include <just/mux/MuxerBase.h>
#include <just/data/base/Error.h>
#include <just/data/base/DataStat.h>
#include <just/avbase/MediaInfo.h>
#include <just/avbase/StreamStatus.h>
using namespace just::data;
using namespace just::demux;
using namespace just::mux;
using namespace just::avbase;

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

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("just.IDemux", framework::logger::Debug);

namespace just
{

    class IDemux
    {
    private:
        struct Cache
        {
            enum StatusEnum
            {
                closed, 
                opening, 
                canceled, 
                opened, 
            };

            Cache()
                : status(closed)
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

            StatusEnum status;
            DemuxerBase * demuxer;
            MuxerBase * muxer;
            std::vector<StreamInfo> stream_infos;
            Sample sample;
            bool paused;
            std::vector<boost::uint8_t> sample_buffer;
        };

    public:
        IDemux()
            : demux_mod_(util::daemon::use_module<DemuxModule>(global_daemon()))
            , mux_mod_(util::daemon::use_module<MuxModule>(global_daemon()))
            , buffer_time_(3000)
        {
        }

        ~IDemux()
        {
        }

        PP_err open(
            PP_str url)
        {
            return open_ex(url, "format=raw");
        }

        PP_err open_ex(
            PP_str url, 
            PP_str format)
        {
            LOG_SECTION();
            LOG_INFO("open url: " << url << ", format: " << format);

            error_code ec;
            boost::mutex::scoped_lock lock(mutex_);
            if (cache_) {
                ec = just::error::already_open;
            } else {
                boost::shared_ptr<Cache> cache(new Cache);
                cache_ = cache;
                framework::string::Url play_link(url);
                framework::string::Url config(std::string("config:///interface?") + format);
                cache->demuxer = demux_mod_.create(play_link, config, ec);
                if (cache->demuxer) {
                    cache->status = Cache::opening;
                    lock.unlock();
                    cache->demuxer->open(ec);
                    lock.lock();
                    if (cache->status == Cache::canceled) {
                        cache->demuxer->close(ec);
                        demux_mod_.destroy(cache->demuxer, ec);
                        ec = boost::asio::error::operation_aborted;
                    } else {
                        cache->status = Cache::opened;
                        if (!ec) {
                            cache->muxer = mux_mod_.open(cache->demuxer, config, ec);
                            cache->muxer->stream_info(cache->stream_infos);
                        }
                    }
                }
            }
            return last_error(__FUNCTION__, ec);
        }

#ifdef JUST_ENABLE_REDIRECT_CALLBACK
        static void redirect_open_callback(
            JUST_Open_Callback callback, 
            PP_err ec)
        {
            redirect_call().call(callback, ec);
        }
#endif

        PP_err async_open(
            PP_str url, 
            JUST_Open_Callback callback)
        {
            return async_open(url, "format=raw", 
#ifndef JUST_ENABLE_REDIRECT_CALLBACK
                boost::bind(callback, _1));
#else
                boost::bind(redirect_open_callback, callback, _1));
#endif
        }

#ifdef JUST_ENABLE_REDIRECT_CALLBACK
        static void redirect_open_ex_callback(
            JUST_Callback callback, 
            PP_context user_data, 
            PP_err ec)
        {
            redirect_call().call(callback, user_data, ec);
        }
#endif

        PP_err async_open_ex(
            PP_str url, 
            PP_str format, 
			PP_context user_data, 
            JUST_Callback callback)
        {
            return async_open(url, format, 
#ifndef JUST_ENABLE_REDIRECT_CALLBACK
                boost::bind(callback, user_data, _1));
#else
                boost::bind(redirect_open_ex_callback, callback, user_data, _1));
#endif
        }

        PP_err async_open(
            PP_str url, 
            PP_str format, 
            boost::function<void (error::errors)> const & callback)
        {
            LOG_SECTION();
            LOG_INFO("async_open url: " << url << ", format: " << format);

            error_code ec;
            boost::mutex::scoped_lock lock(mutex_);
            if (cache_) {
                ec = just::error::already_open;
            } else {
                cache_.reset(new Cache);
                framework::string::Url play_link(url);
                framework::string::Url config(std::string("config:///interface?") + format);
                cache_->demuxer = demux_mod_.create(play_link, config, ec);
                if (cache_->demuxer) {
                    cache_->status = Cache::opening;
                    cache_->demuxer->async_open(
                        boost::bind(&IDemux::open_call_back, this, cache_, config, callback, _1));
                }
            }
            if (ec) {
				global_daemon().io_svc().post(
					boost::bind(callback, async_last_error(__FUNCTION__, ec)));
            }
            return just_success;
        }

        void open_call_back(
            boost::shared_ptr<Cache> & cache, 
            framework::string::Url const & config, 
            boost::function<void (error::errors)> const & callback, 
            error_code ec)
        {
            boost::mutex::scoped_lock lock(mutex_);
            if (cache->status == Cache::canceled) {
                cache->demuxer->close(ec);
                demux_mod_.destroy(cache->demuxer, ec);
                ec = boost::asio::error::operation_aborted;
            } else {
                cache->status = Cache::opened;
                if (!ec) {
                    cache->muxer = mux_mod_.open(cache->demuxer, config, ec);
                    cache->muxer->stream_info(cache->stream_infos);
                }
            }
            lock.unlock();
            callback(async_last_error(__FUNCTION__, ec));
        }

        bool is_open(
            error_code & ec) const
        {
            if (cache_ && cache_->status == Cache::opened) {
                ec.clear();
                return true;
            } else {
                ec = just::error::not_open;
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
            boost::mutex::scoped_lock lock(mutex_);
            if (cache_) {
                if (cache_->status == Cache::closed) {
                    ec = error::not_open;
                } else if (cache_->status == Cache::opening) {
                    cache_->status = Cache::canceled;
                    cache_->demuxer->cancel(ec);
                } else if (cache_->status == Cache::opened) {
                    if (cache_->muxer) {
                        mux_mod_.close(cache_->muxer, ec);
                    }
                    cache_->demuxer->free_sample(cache_->sample, ec);
                    cache_->demuxer->close(ec);
                    demux_mod_.destroy(cache_->demuxer, ec);
                } else if (cache_->status == Cache::canceled) {
                    ec = error::not_open;
                }
                cache_.reset();
            } else {
                ec = just::error::not_open;
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
            JUST_StreamInfo & info)
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
                    info.format.video.width = info2.video_format.width;
                    info.format.video.height = info2.video_format.height;
                    info.format.video.frame_rate_num = info2.video_format.frame_rate_num;
                    info.format.video.frame_rate_den = info2.video_format.frame_rate_den;
                } else if (info2.type == StreamType::AUDI) {
                    info.format.audio.channel_count = info2.audio_format.channel_count;
                    info.format.audio.sample_size = info2.audio_format.sample_size;
                    info.format.audio.sample_rate = info2.audio_format.sample_rate;
                    info.format.audio.block_align = info2.audio_format.block_align;
                    info.format.audio.sample_per_frame = info2.audio_format.sample_per_frame;
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
                just::data::MediaInfo info;
                cache_->demuxer->get_media_info(info, ec);
                duration = ec ? 0 : (boost::uint32_t)info.duration;
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err read_sample(
            JUST_Sample & sample)
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
            JUST_PlayStatistic & stat)
        {
            error_code ec;
            if (!is_open(ec)) {
                stat.buffer_time = 0;
                stat.buffering_present = 0;
                if (ec == boost::asio::error::would_block) {
                    stat.play_status = cache_->paused ? JUST_PlayStatus::paused : JUST_PlayStatus::buffering;
                } else {
                    stat.play_status = JUST_PlayStatus::closed;
                }
            } else {
                cache_->demuxer->fill_data(ec);
                StreamStatus status;
                cache_->demuxer->get_stream_status(status, ec);
                if (ec && ec != boost::asio::error::would_block) {
                    stat.play_status = JUST_PlayStatus::closed;
                } else {
                    stat.buffer_time = (boost::uint32_t)(status.time_range.buf - status.time_range.pos);
                    if (stat.buffer_time >= buffer_time_ 
                        || status.buf_ec == boost::asio::error::eof
                        || status.buf_ec == just::data::error::no_more_segment) {
                            stat.buffering_present = 100;
                            stat.play_status = JUST_PlayStatus::playing;
                    } else if (buffer_time_ != 0) {
                        stat.buffering_present = stat.buffer_time * 100 / buffer_time_;
                        stat.play_status = JUST_PlayStatus::buffering;
                    }
                    if (cache_->paused) {
                        stat.play_status = JUST_PlayStatus::paused;
                    }
                    // 在缓冲状态，缓冲的错误码更有参考意义
                    if (stat.play_status == JUST_PlayStatus::buffering) {
                        ec = status.buf_ec;
                    }
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err get_data_stat(
            JUST_DataStat & stat)
        {
            error_code ec;
            if (cache_ && cache_->demuxer) {
                DataStat data_stat;
                cache_->demuxer->get_data_stat(data_stat, ec);
                stat.total_elapse = (boost::uint32_t)(time(NULL) - data_stat.start_time);
                stat.total_download_bytes = (boost::uint32_t)data_stat.total_bytes;
                stat.connection_status = data_stat.connection_status;
                stat.average_speed_one_second = data_stat.speeds[0].cur_speed;
                stat.average_speed_five_seconds = data_stat.speeds[1].cur_speed;
                stat.average_speed_twenty_seconds = data_stat.speeds[2].cur_speed;
                stat.average_speed_sixty_seconds = data_stat.speeds[3].cur_speed;
            } else {
                ec = error::not_open;
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
            return just::error::last_error_enum(ec);
        }

        static error::errors last_error(
            PP_str title, 
            error_code const & ec)
        {
            just::error::last_error(ec);
            return async_last_error(title, ec);
        }

    private:
        DemuxModule & demux_mod_;
        MuxModule & mux_mod_;
        boost::uint32_t buffer_time_;
        boost::shared_ptr<Cache> cache_;
        boost::mutex mutex_;
    };
}

static just::IDemux & demuxer()
{
    static just::IDemux the_demuxer;
    return the_demuxer;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus

    JUST_DECL PP_err JUST_Open(
        PP_str url)
    {
        return demuxer().open(url);
    }

    JUST_DECL PP_err JUST_AsyncOpen(
        PP_str url, 
        JUST_Open_Callback callback)
    {
        return demuxer().async_open(url, callback);
    }

    JUST_DECL PP_err JUST_OpenEx(
        PP_str url, 
        PP_str format)
    {
        return demuxer().open_ex(url, format);
    }

    JUST_DECL PP_err JUST_AsyncOpenEx(
        PP_str url, 
        PP_str format, 
		PP_context user_data, 
        JUST_Callback callback)
    {
        return demuxer().async_open_ex(url, format, user_data, callback);
    }

    JUST_DECL PP_uint JUST_GetStreamCount()
    {
        PP_uint n = 0;
        demuxer().get_stream_count(n);
        return n;
    }

    JUST_DECL PP_err JUST_GetStreamInfo(
        PP_uint index, 
        JUST_StreamInfo * stream_info)
    {
        return demuxer().get_stream_info(index, *stream_info);
    }

    JUST_DECL PP_uint JUST_GetDuration()
    {
        PP_uint n = 0;
        demuxer().get_duration(n);
        return n;
    }

    JUST_DECL PP_err JUST_Seek(
        PP_uint start_time)
    {
        return demuxer().seek(start_time);
    }

    JUST_DECL PP_err JUST_ReadSample(
        JUST_Sample * sample)
    {
        return demuxer().read_sample(*sample);
    }

    JUST_DECL PP_err JUST_Close()
    {
        return demuxer().close();
    }

    JUST_DECL PP_err JUST_Pause()
    {
        return demuxer().pause();
    }

    JUST_DECL PP_err JUST_SetDownloadBufferSize(
        PP_uint length)
    {
        return demuxer().set_download_buffer_size(length);
    }

    JUST_DECL PP_err JUST_SetPlayBufferTime(
        PP_uint time)
    {
        return demuxer().set_play_buffer_time(time);
    }

    JUST_DECL PP_err JUST_GetPlayStat(
        JUST_PlayStatistic * stat)
    {
        return demuxer().get_play_stat(*stat);
    }

    JUST_DECL PP_err JUST_GetDataStat(
        JUST_DataStat * stat)
    {
        return demuxer().get_data_stat(*stat);
    }


#if __cplusplus
}
#endif // __cplusplus
