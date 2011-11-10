// IDemuxer.cpp

#include "ppbox/ppbox/Common.h"
#define PPBOX_SOURCE
#include "ppbox/ppbox/IDemuxer.h"

#include <ppbox/demux/DemuxerModule.h>
#include <ppbox/demux/PptvDemuxer.h>
#include <ppbox/demux/DemuxerError.h>
using namespace ppbox::demux;

#include <framework/logger/LoggerStreamRecord.h>
#include <framework/logger/LoggerSection.h>
#include <framework/system/LogicError.h>
#include <util/buffers/BufferCopy.h>
using namespace framework::logger;
using namespace framework::timer;

#include <boost/bind.hpp>
#include <boost/thread.hpp>
using namespace boost::system;

extern void pool_dump();

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("Ppbox", 0);

namespace ppbox
{

    class IDemuxer
    {
    private:
        struct Cache
        {
            Cache()
                : close_token(0)
                , paused(false)
                , sample_buffer(NULL)
            {
            }


            boost::uint8_t const * copy_sample_data()
            {
                if (sample.data.size() == 1) {
                    return boost::asio::buffer_cast<boost::uint8_t const *>(sample.data[0]);
                } else {
                    sample_buffer.resize(sample.size);
                    util::buffers::buffer_copy(boost::asio::buffer(sample_buffer), sample.data);
                    return sample_buffer.size() ? &sample_buffer.at(0) : NULL;
                }
            }

            size_t close_token;
            PptvDemuxer * demuxer;
#ifdef PPBOX_DEMUX_RETURN_SEGMENT_INFO
            ppbox::demux::SegmentInfo segment;
#endif
            ppbox::demux::MediaInfo media_info;
            boost::uint32_t video_media_index;
            std::vector<boost::uint8_t> media_type_buffer;
            std::vector<boost::uint8_t> avc_buf;
            ppbox::demux::Sample sample;
            bool paused;
            std::vector<boost::uint8_t> sample_buffer;
        };

    public:
        IDemuxer()
            : demux_mod_(util::daemon::use_module<ppbox::demux::DemuxerModule>(global_daemon()))
            , buffer_time_(3000)
        {
        }

        ~IDemuxer()
        {
        }

        error::errors open(
            char const * playlink)
        {
            LOG_SECTION();
            LOG_S(Logger::kLevelEvent, "open playlink: " << playlink);

            error_code ec;
            if (cache_) {
                ec = ppbox::demux::error::already_open;
            } else {
                boost::shared_ptr<Cache> cache(new Cache);
                cache_ = cache;
                cache->demuxer = demux_mod_.open(playlink, cache->close_token, ec);
            }
            return last_error(__FUNCTION__, ec);
        }

        void async_open(
            char const * playlink, 
            PPBOX_Open_Callback callback)
        {
            LOG_SECTION();
            LOG_S(Logger::kLevelEvent, "async_open playlink: " << playlink);

            error_code ec;
            if (cache_) {
                ec = ppbox::demux::error::already_open;
            } else {
                cache_.reset(new Cache);
                demux_mod_.async_open(playlink, cache_->close_token, 
                    boost::bind(&IDemuxer::open_call_back, cache_, callback, _1, _2));
            }
            if (ec) {
                boost::thread th(boost::bind(callback, async_last_error(__FUNCTION__, ec)));
                (void)th;
            }
        }

        static void open_call_back(
            boost::shared_ptr<Cache> & cache, 
            PPBOX_Open_Callback callback, 
            error_code const & ec, 
            PptvDemuxer * demuxer)
        {
            cache->demuxer = demuxer;
            callback(async_last_error(__FUNCTION__, ec));
        }

        bool is_open(
            error_code & ec) const
        {
            if (cache_ && cache_->demuxer) {
                ec.clear();
                return true;
            } else {
                ec = ppbox::demux::error::not_open;
                return false;
            }
        }

        error::errors seek(
            boost::uint32_t time)
        {
            LOG_SECTION();
            LOG_S(Logger::kLevelEvent, "seek time: " << time);

            error_code ec;
            if (is_open(ec)) {
                cache_->demuxer->seek(time, ec);
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors pause()
        {
            LOG_SECTION();
            LOG_S(Logger::kLevelEvent, "pause");

            error_code ec;
            if (is_open(ec)) {
                cache_->demuxer->pause(ec);
                if (!ec)
                    cache_->paused = true;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors resume()
        {
            LOG_SECTION();
            LOG_S(Logger::kLevelEvent, "resume");

            error_code ec;
            if (is_open(ec)) {
                cache_->demuxer->resume(ec);
                if (!ec)
                    cache_->paused = false;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors close()
        {
            LOG_SECTION();
            LOG_S(Logger::kLevelEvent, "close");

            error_code ec;
            if (cache_) {
                while (true) {
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
                ec = ppbox::demux::error::not_open;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_stream_count(
            boost::uint32_t & count)
        {
            error_code ec;
            if (is_open(ec)) {
                count = cache_->demuxer->get_media_count(ec);
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_stream_info(
            boost::int32_t index, 
            PPBOX_StreamInfo & info)
        {
            error_code ec;
            if (!is_open(ec)) {
            } else if (cache_->demuxer->get_media_info(index, cache_->media_info, ec)) {
            } else {
                if (cache_->media_info.type == MEDIA_TYPE_VIDE) {
                    info.type = ppbox_video;
                    if (cache_->media_info.sub_type == VIDEO_TYPE_AVC1) {
                        info.sub_type = ppbox_video_avc;
                    } else {
                        info.sub_type = 0;
                    }
                    if (cache_->media_info.format_type == MediaInfo::video_avc_packet) {
                        info.format_type = ppbox_video_avc_packet;
                    } else if (cache_->media_info.format_type == MediaInfo::video_avc_byte_stream) {
                        info.format_type = ppbox_video_avc_byte_stream;
                    } else {
                        info.format_type = 0;
                    }
                } else {
                    info.type = ppbox_audio;
                    if (cache_->media_info.sub_type == AUDIO_TYPE_MP4A) {
                        info.sub_type = ppbox_audio_aac;
                    } else if (cache_->media_info.sub_type == AUDIO_TYPE_MP1A) {
                        info.sub_type = ppbox_audio_mp3;
                    } else if (cache_->media_info.sub_type == AUDIO_TYPE_WMA2) {
                        info.sub_type = ppbox_audio_wma;
                    } else {
                        info.sub_type = 0;
                    }
                    // 增加对音频format_type的判断
                    if (cache_->media_info.format_type == MediaInfo::audio_iso_mp4) {
                        info.format_type = ppbox_audio_iso_mp4;
                    } else if (cache_->media_info.format_type == MediaInfo::audio_microsoft_wave) {
                        info.format_type = ppbox_audio_microsoft_wave;
                    } else {
                        info.format_type = 0;
                    }
                }
                //info.format_type = cache_->media_info.format_type;
                info.format_size = cache_->media_info.format_data.size();
                info.format_buffer = info.format_size ? &cache_->media_info.format_data.at(0) : NULL;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_stream_info_ex(
            boost::int32_t index,
            PPBOX_StreamInfoEx & info)
        {
            error_code ec;
            if (!is_open(ec)) {
            } else if(cache_->demuxer->get_media_info(index, cache_->media_info, ec)) {
            } else {
                if (cache_->media_info.type == MEDIA_TYPE_VIDE) {
                    info.type = ppbox_video;
                    info.video_format.frame_rate = cache_->media_info.video_format.frame_rate;
                    info.video_format.height = cache_->media_info.video_format.height;
                    info.video_format.width = cache_->media_info.video_format.width;
                    if (cache_->media_info.sub_type == VIDEO_TYPE_AVC1) {
                        info.sub_type = ppbox_video_avc;
                    } else {
                        info.sub_type = 0;
                    }
                    if (cache_->media_info.format_type == MediaInfo::video_avc_packet) {
                        info.format_type = ppbox_video_avc_packet;
                    } else if (cache_->media_info.format_type == MediaInfo::video_avc_byte_stream) {
                        info.format_type = ppbox_video_avc_byte_stream;
                    } else {
                        info.format_type = 0;
                    }
                } else {
                    info.type = ppbox_audio;
                    info.audio_format.sample_rate = cache_->media_info.audio_format.sample_rate;
                    info.audio_format.sample_size = cache_->media_info.audio_format.sample_size;
                    info.audio_format.channel_count = cache_->media_info.audio_format.channel_count;
                    if (cache_->media_info.sub_type == AUDIO_TYPE_MP4A) {
                        info.sub_type = ppbox_audio_aac;
                    } else if (cache_->media_info.sub_type == AUDIO_TYPE_MP1A) {
                        info.sub_type = ppbox_audio_mp3;
                    } else if (cache_->media_info.sub_type == AUDIO_TYPE_WMA2) {
                        info.sub_type = ppbox_audio_wma;
                    } else {
                        info.sub_type = 0;
                    }
                    // 增加对音频format_type的判断
                    if (cache_->media_info.format_type == MediaInfo::audio_iso_mp4) {
                        info.format_type = ppbox_audio_iso_mp4;
                    } else if (cache_->media_info.format_type == MediaInfo::audio_microsoft_wave) {
                        info.format_type = ppbox_audio_microsoft_wave;
                    } else {
                        info.format_type = 0;
                    }
                }
                // info.format_type = cache_->media_info.format_type;
                info.format_size = cache_->media_info.format_data.size();
                info.format_buffer = info.format_size ? &cache_->media_info.format_data.at(0) : NULL;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_avc_config(
            boost::uint8_t const * & buffer, 
            boost::uint32_t & length)
        {
            error_code ec;
            if (!is_open(ec)) {
            } else if (cache_->avc_buf.empty()) {
                boost::uint32_t stream_count = 
                    cache_->demuxer->get_media_count(ec);
                for (boost::uint32_t i = 0; i < stream_count; ++i) {
                    if (cache_->demuxer->get_media_info(i, cache_->media_info, ec)) {
                        break;
                    } else if (cache_->media_info.type == MEDIA_TYPE_VIDE 
                        && cache_->media_info.sub_type == VIDEO_TYPE_AVC1
                        && cache_->media_info.format_type == MediaInfo::video_avc_packet) {
                            cache_->avc_buf = cache_->media_info.format_data;
                            break;
                    }
                }
                if (!ec && cache_->avc_buf.empty()) {
                    ec = framework::system::logic_error::no_data;
                } else {
                    buffer = &cache_->avc_buf.at(0);
                    length = cache_->avc_buf.size();
                }
            } else {
                buffer = &cache_->avc_buf.at(0);
                length = cache_->avc_buf.size();
                ec.clear();
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_duration(
            boost::uint32_t & duration)
        {
            error_code ec;
            if (is_open(ec)) {
                duration = cache_->demuxer->get_duration(ec);
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_width_height(
            boost::uint32_t & width, 
            boost::uint32_t & height)
        {
            error_code ec;
            if (is_open(ec)) {
                boost::uint32_t stream_count = cache_->demuxer->get_media_count(ec);
                if (!ec) {
                    ec = framework::system::logic_error::no_data;
                    for (boost::uint32_t i = 0; i < stream_count; ++i) {
                        if (cache_->demuxer->get_media_info(i, cache_->media_info, ec)) {
                            break;
                        } else if (cache_->media_info.type == MEDIA_TYPE_VIDE) {
                            width = cache_->media_info.video_format.width;
                            height = cache_->media_info.video_format.height;
                            ec = boost::system::error_code();
                        }
                    }
                }
            }
            return last_error(__FUNCTION__, ec);
        }

#ifdef PPBOX_DEMUX_RETURN_SEGMENT_INFO

        error::errors get_first_seg_header(
            boost::uint8_t const * & buffer, 
            boost::uint32_t & length)
        {
            return get_seg_header(0, buffer, length);
        }

        error::errors get_seg_header(
            boost::uint32_t index, 
            boost::uint8_t const * & buffer, 
            boost::uint32_t & length)
        {
            error_code ec;
            if (!is_open(ec)) {
            } else if ((cache_->segment.head_data.empty() || cache_->segment.index != index) 
                && index == (cache_->segment.index = index) 
                && cache_->demuxer && cache_->demuxer->get_segment_info(cache_->segment, true, ec)) {
            } else {
                length = cache_->segment.head_data.size();
                buffer = length ? &cache_->segment.head_data.at(0) : NULL;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_seg_mdat_size(
            boost::uint32_t index, 
            boost::uint64_t & length)
        {
            error_code ec;
            if (is_open(ec)) {
                SegmentInfo info;
                if (!cache_->demuxer->get_segment_info(info, false, ec)) {
                    length = info.file_length - info.head_length;
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_segment_count(
            boost::uint32_t & count)
        {
            error_code ec;
            if (is_open(ec)) {
                count = cache_->demuxer->get_segment_count(ec);
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_segment_info(
            PPBOX_SegmentInfo & info)
        {
            error_code ec;
            if (is_open(ec)) {
                cache_->segment.index = (size_t(-1));
                if (!cache_->demuxer->get_segment_info(cache_->segment, true, ec)) {
                    info.index = cache_->segment.index;
                    info.duration = cache_->segment.duration;
                    info.duration_offset = cache_->segment.duration_offset;
                    info.head_length = cache_->segment.head_data.size();
                    info.file_length = cache_->segment.file_length;
                    info.head_buffer = info.file_length ? &cache_->segment.head_data.at(0) : NULL;
                }
            }
            return last_error(__FUNCTION__, ec);
        }

#endif

        error::errors read_sample(
            PPBOX_Sample & sample)
        {
            error_code ec;
            if (is_open(ec)) {
                if (cache_->paused) {
                    cache_->paused = false;
                }
                if (!cache_->demuxer->get_sample_buffered(cache_->sample, ec)) {
                    sample.stream_index = cache_->sample.itrack;
                    sample.start_time = cache_->sample.ustime;
                    sample.buffer_length = cache_->sample.size;
                    sample.buffer = cache_->copy_sample_data();
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors read_sample_ex(
            PPBOX_SampleEx & sample)
        {
            error_code ec;
            if (is_open(ec)) {
                if (cache_->paused) {
                    cache_->paused = false;
                }
                if (!cache_->demuxer->get_sample_buffered(cache_->sample, ec)) {
                    sample.stream_index = cache_->sample.itrack;
                    sample.start_time = cache_->sample.ustime;
                    sample.offset_in_file = cache_->sample.blocks[0].offset;
                    sample.buffer_length = cache_->sample.size;
                    sample.duration = cache_->sample.duration;
                    sample.desc_index = cache_->sample.idesc;
                    sample.decode_time = cache_->sample.dts;
                    sample.composite_time_delta = cache_->sample.cts_delta;
                    sample.is_sync = cache_->sample.flags & Sample::sync;
                    sample.buffer = cache_->copy_sample_data();
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors read_sample_ex2(
            PPBOX_SampleEx2 & sample)
        {
            error_code ec;
            if (is_open(ec)) {
                if (cache_->paused) {
                    cache_->paused = false;
                }
                if (!cache_->demuxer->get_sample_buffered(cache_->sample, ec)) { 
                    sample.stream_index = cache_->sample.itrack;
                    sample.start_time = cache_->sample.ustime;
                    sample.buffer_length = cache_->sample.size;
                    sample.duration = cache_->sample.duration;
                    sample.desc_index = cache_->sample.idesc;
                    sample.decode_time = cache_->sample.dts;
                    sample.composite_time_delta = cache_->sample.cts_delta;
                    sample.is_sync = cache_->sample.flags & Sample::sync;
                    sample.is_discontinuity = cache_->sample.flags & Sample::discontinuity;
                    sample.buffer = cache_->copy_sample_data();
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_play_stat(
            PPBOX_PlayStatistic & stat)
        {
            error_code ec;
            memset(&stat, 0, sizeof(stat));
            stat.length = sizeof(stat);
            if (!is_open(ec)) {
                stat.buffer_time = 0;
                stat.buffering_present = 0;
                if (ec == boost::asio::error::would_block) {
                    stat.play_status = cache_->paused ? ppbox_paused : ppbox_buffering;
                } else {
                    stat.play_status = ppbox_closed;
                }
            } else {
                error_code ec_buf;
                stat.buffer_time = cache_->demuxer->get_buffer_time(ec, ec_buf);
                if (ec && ec != boost::asio::error::would_block) {
                    stat.play_status = ppbox_closed;
                } else {
                    if (stat.buffer_time >= buffer_time_ || ec_buf == boost::asio::error::eof) {
                        stat.buffering_present = 100;
                        stat.play_status = ppbox_playing;
                    } else if (buffer_time_ != 0) {
                        stat.buffering_present = stat.buffer_time * 100 / buffer_time_;
                        stat.play_status = ppbox_buffering;
                    }
                    if (cache_->paused) {
                        stat.play_status = ppbox_paused;
                    }
                    // 在缓冲状态，缓冲的错误码更有参考意义
                    if (stat.play_status == ppbox_buffering) {
                        ec = ec_buf;
                    }
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_download_stat(
            PPBOX_DownloadMsg & stat)
        {
            error_code ec;
            stat.length = sizeof(stat);
            if (is_open(ec)) {
                BufferStatistic const & buffer_stat = cache_->demuxer->buffer_stat();
                memset(&stat, 0, sizeof(stat));
                stat.length = sizeof(stat);
                stat.start_time = (boost::uint32_t)buffer_stat.start_time;
                stat.total_download_bytes = (boost::uint32_t)buffer_stat.total_bytes;
                stat.http_download_bytes = (boost::uint32_t)buffer_stat.total_bytes;
                stat.download_duration_in_sec = (boost::uint32_t)(time(NULL) - buffer_stat.start_time);
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors get_download_speed(
            PPBOX_DownloadSpeedMsg & stat)
        {
            error_code ec;
            if (is_open(ec)) {
                BufferStatistic const & buffer_stat = cache_->demuxer->buffer_stat();
                memset(&stat, 0, sizeof(stat));
                stat.length = sizeof(stat);
                time_t now = time(NULL);
                if (now > buffer_stat.start_time) {
                    stat.avg_download_speed = (boost::uint32_t)(buffer_stat.total_bytes / (now - buffer_stat.start_time));
                }
                stat.second_download_speed = buffer_stat.speeds[0].cur_speed;
                stat.recent_download_speed = buffer_stat.speeds[1].cur_speed;
                stat.now_download_speed = buffer_stat.speeds[2].cur_speed;
                stat.minute_download_speed = buffer_stat.speeds[3].cur_speed;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors set_download_buffer_size(
            boost::uint32_t buffer_size)
        {
            error_code ec;
            demux_mod_.set_download_buffer_size(buffer_size);
            return last_error(__FUNCTION__, ec);
        }

        error::errors set_download_max_speed(
            boost::uint32_t speed)
        {
            error_code ec;
            demux_mod_.set_download_max_speed(speed);
            return last_error(__FUNCTION__, ec);
        }

        error::errors set_http_proxy(
            char const * addr)
        {
            error_code ec;
            demux_mod_.set_http_proxy(addr);
            return last_error(__FUNCTION__, ec);
        }

        error::errors set_play_buffer_time(
            boost::uint32_t buffer_time)
        {
            error_code ec;
            buffer_time_ = buffer_time;
            return last_error(__FUNCTION__, ec);
        }

        static error::errors async_last_error(
            char const * log_title, 
            error_code const & ec)
        {
            if (ec && ec != boost::asio::error::would_block) {
                LOG_S(Logger::kLevelAlarm, log_title << ": " << ec.message());
            }
            return ppbox::error::last_error_enum(ec);
        }

        error::errors last_error(
            char const * title, 
            error_code const & ec) const
        {
            ppbox::error::last_error(ec);
            return async_last_error(title, ec);
        }

    private:
        ppbox::demux::DemuxerModule & demux_mod_;
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

    PPBOX_DECL boost::int32_t PPBOX_Open(
        char const * playlink)
    {
        return demuxer().open(playlink);
    }

    PPBOX_DECL void PPBOX_AsyncOpen(
        char const * playlink, 
        PPBOX_Open_Callback callback)
    {
        demuxer().async_open(playlink, callback);
    }

    PPBOX_DECL boost::uint32_t PPBOX_GetStreamCount()
    {
        boost::uint32_t n = 0;
        demuxer().get_stream_count(n);
        return n;
    }

    PPBOX_DECL boost::int32_t PPBOX_GetStreamInfo(
        boost::uint32_t index, 
        PPBOX_StreamInfo * stream_info)
    {
        return demuxer().get_stream_info(index, *stream_info);
    }

    PPBOX_DECL boost::int32_t PPBOX_GetStreamInfoEx(
        boost::uint32_t index, 
        PPBOX_StreamInfoEx * stream_info)
    {
        return demuxer().get_stream_info_ex(index, *stream_info);
    }

    PPBOX_DECL boost::uint32_t PPBOX_GetDuration()
    {
        boost::uint32_t n = 0;
        demuxer().get_duration(n);
        return n;
    }

    PPBOX_DECL boost::int32_t PPBOX_GetWidthHeight(
        boost::uint32_t * pwidth, 
        boost::uint32_t * pheight)
    {
        return demuxer().get_width_height(*pwidth, *pheight);
    }

    PPBOX_DECL boost::int32_t PPBOX_Seek(
        boost::uint32_t start_time)
    {
        return demuxer().seek(start_time);
    }

    PPBOX_DECL boost::int32_t PPBOX_GetAvcConfig(
        boost::uint8_t const * * buffer, 
        boost::uint32_t * length)
    {
        return demuxer().get_avc_config(*buffer, *length);
    }

#ifdef PPBOX_DEMUX_RETURN_SEGMENT_INFO

    PPBOX_DECL boost::int32_t PPBOX_GetFirstSegHeader(
        PP_uchar const * * buffer, 
        boost::uint32_t * length)
    {
        return demuxer().get_first_seg_header(*buffer, *length);
    }

    PPBOX_DECL boost::int32_t PPBOX_GetSegHeader(
        boost::uint32_t index, 
        PP_uchar const * * buffer, 
        boost::uint32_t * length)
    {
        return demuxer().get_seg_header(index, *buffer, *length);
    }

    PPBOX_DECL boost::uint32_t PPBOX_GetSegDataSize(
        boost::uint32_t index)
    {
        boost::uint64_t size = 0;
        demuxer().get_seg_mdat_size(index, size);
        return (boost::uint32_t)size;
    }

    PPBOX_DECL boost::uint32_t PPBOX_GetSegmentCount()
    {
        boost::uint32_t n = 0;
        demuxer().get_segment_count(n);
        return n;
    }

    PPBOX_DECL boost::int32_t PPBOX_GetSegmentInfo(
        PPBOX_SegmentInfo * segment_info)
    {
        return demuxer().get_segment_info(*segment_info);
    }

#endif

    PPBOX_DECL boost::int32_t PPBOX_ReadSample(
        PPBOX_Sample * sample)
    {
        return demuxer().read_sample(*sample);
    }

    PPBOX_DECL boost::int32_t PPBOX_ReadSampleEx(
        PPBOX_SampleEx * sample)
    {
        return demuxer().read_sample_ex(*sample);
    }

    PPBOX_DECL boost::int32_t PPBOX_ReadSampleEx2(
        PPBOX_SampleEx2 *sample)
    {
        return demuxer().read_sample_ex2(*sample);
    }

    PPBOX_DECL void PPBOX_Close()
    {
        demuxer().close();
    }

    PPBOX_DECL boost::int32_t PPBOX_Pause()
    {
        return demuxer().pause();
    }

    PPBOX_DECL void PPBOX_SetDownloadBufferSize(
        boost::uint32_t length)
    {
        demuxer().set_download_buffer_size(length);
    }

    PPBOX_DECL void PPBOX_SetDownloadMaxSpeed(
        boost::uint32_t speed)
    {
        demuxer().set_download_max_speed(speed);
    }

    PPBOX_DECL void PPBOX_SetHttpProxy(
        char const * addr)
    {
        demuxer().set_http_proxy(addr);
    }

    PPBOX_DECL void PPBOX_SetPlayBufferTime(
        boost::uint32_t time)
    {
        demuxer().set_play_buffer_time(time);
    }

    PPBOX_DECL boost::int32_t PPBOX_GetPlayMsg(
        PPBOX_PlayStatistic * statistic_Msg)
    {
        return demuxer().get_play_stat(*statistic_Msg);
    }

    PPBOX_DECL boost::int32_t PPBOX_GetDownMsg(
        PPBOX_DownloadMsg* download_Msg)
    {
        return demuxer().get_download_stat(*download_Msg);
    }

    PPBOX_DECL boost::int32_t PPBOX_GetDownSedMsg(
        PPBOX_DownloadSpeedMsg* download_spped_Msg)
    {
        return demuxer().get_download_speed(*download_spped_Msg);
    }

#if __cplusplus
}
#endif // __cplusplus
