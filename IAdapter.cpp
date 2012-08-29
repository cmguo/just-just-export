// IDemuxer.cpp

#include "ppbox/ppbox/Common.h"
#define PPBOX_SOURCE
#include "ppbox/ppbox/IAdapter.h"
using namespace ppbox::error;

#include <ppbox/demux/base/BufferDemuxer.h>
#include <ppbox/mux/Muxer.h>
#include <ppbox/mux/flv/FlvMux.h>
#include <ppbox/mux/ts/TsMux.h>
#include <ppbox/demux/DemuxerModule.h>
#include <ppbox/demux/base/DemuxerError.h>
#include <ppbox/demux/base/SourceError.h>

#include <framework/logger/StreamRecord.h>
#include <framework/logger/LoggerSection.h>
#include <framework/system/LogicError.h>
#include <framework/string/Algorithm.h>
using namespace framework::logger;
using namespace framework::string;

#include <boost/bind.hpp>
#include <boost/ref.hpp>
using namespace boost::system;

#ifndef PPBOX_DISABLE_MUX

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("Ppbox", 0);

//#ifndef PPBOX_DISABLE_MUX

namespace ppbox
{

    struct Cache
    {
        Cache()
            : close_token(0)
            , demuxer(NULL)
            , paused(false)
            , need_seek_time(false)
        {
        }
        size_t close_token;
        ppbox::demux::BufferDemuxer * demuxer;
        ppbox::demux::MediaInfo media_info;
        boost::uint32_t video_media_index;
        bool paused;
        bool need_seek_time;
    };

    typedef enum {TAGHEAD, TAGDATA, TAGSIZE, UNDEFINE} TagField;
    struct TagReadPosition
    {
        TagReadPosition()
            : tag_field(UNDEFINE)
            , data_position(0)
        {
        }

        TagField       tag_field;
        PP_uint32      data_position;
    };

    class IAdapter
    {
    public:
        IAdapter()
            : demux_mod_(util::daemon::use_module<ppbox::demux::DemuxerModule>(global_daemon()))
            , mux_(NULL)
            , buffer_time_(3000)
        {
        }

        ~IAdapter()
        {
        }

        error::errors adapter_open(
            char const * playlink,
            char const * format)
        {
            assert(playlink != NULL);
            assert(format != NULL);

            error_code ec;
            if (cache_) {
                ec = already_open;
            } else {
                //set
                playlink_ = playlink;
                format_   = format;
                boost::shared_ptr<Cache> cache(new Cache);
                cache_ = cache;
                cache->demuxer = 
                    demux_mod_.open(playlink, cache->close_token, ec);
                if (!ec) {
                    opem_mux();
                    if (mux_) {
                        mux_->open(cache_->demuxer, ec);
                    } else {
                        ec = not_open;
                    }
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        bool is_open(
            error_code & ec) const
        {
            if (cache_ && cache_->demuxer) {
                ec.clear();
                return true;
            } else {
                ec = not_open;
                return false;
            }
        }

        void open_call_back(
            boost::shared_ptr<Cache> & cache, 
            Adapter_Open_Callback callback, 
            error_code const & ec, 
            ppbox::demux::BufferDemuxer * demuxer)
        {
            error_code lec = ec;
            cache->demuxer = demuxer;
            if (!lec) {
                opem_mux();
                mux_->open(cache_->demuxer, lec);
            }
            callback(async_last_error(__FUNCTION__, lec));
        }

        void adapter_async_open(
            char const * playlink,
            char const * format,
            Adapter_Open_Callback callback)
        {
            error_code ec;
            if (cache_) {
                ec = already_open;
            } else {
                cache_.reset(new Cache);
                format_ = format;
                demux_mod_.async_open(playlink, cache_->close_token, 
                    boost::bind(&IAdapter::open_call_back, this, cache_, callback, _1, _2));
            }
        }

        error::errors adapter_pause()
        {
            error_code ec;
            if (is_open(ec)) {
                mux_->pause(ec);
                if (!ec)
                    cache_->paused = true;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors adapter_resume()
        {
            error_code ec;
            if (is_open(ec)) {
                mux_->resume(ec);
                if (!ec)
                    cache_->paused = false;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors adapter_close()
        {
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
                if (mux_) {
                    mux_->close();
                }
                close_mux();
                cache_->demuxer = NULL;
                cache_.reset();
            } else {
                ec = not_open;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors adapter_seek(PP_uint32 time)
        {
            error_code ec;
            if (is_open(ec)) {
                mux_->seek(time, ec);
                if (!ec || ec == boost::asio::error::would_block) {
                    tag_read_position_.tag_field = UNDEFINE;
                    cache_->need_seek_time = true;
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        void adapter_reset()
        {
            tag_read_position_.tag_field = UNDEFINE;
            mux_->reset();
        }

        error::errors adapter_read(
            unsigned char * buffer,
            PP_uint32 buffer_size,
            PP_uint32 & read_size)
        {
            error_code ec;
            assert(buffer != NULL);
            assert(buffer_size >= 0);

            if (is_open(ec)) {
                if (cache_->paused) {
                    cache_->paused = false;
                }

                if (buffer_size < 32) {
                    ec = logic_error;
                    return last_error(__FUNCTION__, ec);
                }

                if (UNDEFINE == tag_read_position_.tag_field) {
                    mux_->read(read_tag_, ec);
                    if (!ec) {
                        tag_read_position_.tag_field     = TAGHEAD;
                        tag_read_position_.data_position = 0;
                        cache_->need_seek_time = false;
                    } else {
                        cache_->need_seek_time = true;
                        return last_error(__FUNCTION__, ec);
                    }
                }

                PP_uint32 offset = 0;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors adapter_get_play_stat(
            Adapter_PlayStatistic & stat)
        {
            error_code ec;
            memset(&stat, 0, sizeof(stat));
            stat.length = sizeof(stat);

            if (!is_open(ec)) {
                stat.buffer_time = 0;
                stat.buffering_present = 0;
                if (ec == boost::asio::error::would_block) {
                    stat.play_status = cache_->paused ? ppbox_adapter_paused : ppbox_adapter_buffering;
                } else {
                    stat.play_status = ppbox_adapter_closed;
                }
            } else {
                if (cache_->need_seek_time) {
                    //mux_->get_sample().time = cache_->demuxer->get_cur_time(ec);
                    if (ec) {
                        stat.buffer_time = 0;
                        stat.buffering_present = 0;
                        if (ec == boost::asio::error::would_block) {
                            stat.play_status = cache_->paused ? ppbox_adapter_paused : ppbox_adapter_buffering;
                        } else {
                            stat.play_status = ppbox_adapter_closed;
                        }
                        return last_error(__FUNCTION__, ec);
                    }
                    cache_->need_seek_time = false;
                }
                error_code ec_buf;
                boost::uint32_t buffer_time = cache_->demuxer->get_end_time(ec, ec_buf);
                //stat.buffer_time = buffer_time > mux_->get_sample().time ? buffer_time - mux_->get_sample().time : 0;

                if (ec && ec != boost::asio::error::would_block) {
                    stat.play_status = ppbox_adapter_closed;
                } else {
                    if (stat.buffer_time >= buffer_time_ || ec_buf == ppbox::demux::source_error::no_more_segment) {
                        stat.buffering_present = 100;
                        stat.play_status = ppbox_adapter_playing;
                    } else {
                        stat.buffering_present = stat.buffer_time * 100 / buffer_time_;
                        stat.play_status = ppbox_adapter_buffering;
                    }
                    // 在缓冲状态，缓冲的错误码更有参考意义
                    if (stat.play_status == ppbox_adapter_buffering) {
                        ec = ec_buf;
                    }
                }
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors adapter_get_mediainfo(
            Adapter_Mediainfo & media_info)
        {
            error_code ec;
            if (is_open(ec)) {
                ppbox::mux::MediaFileInfo const & mediainfo = mux_->mediainfo();
                media_info.channel_count = mediainfo.stream_count;
                media_info.duration      = mediainfo.duration_info.total;
                media_info.frame_rate    = 0;
                media_info.height        = 0;
                media_info.width         = 0;
                media_info.sample_rate   = 0;
                media_info.sample_size   = 0;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors adapter_get_playing_time(
            boost::uint64_t & time)
        {
            error_code ec;
            if (is_open(ec)) {
                time = mux_->current_time();
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors last_error(
            char const * title, 
            error_code & ec) const
        {
            ppbox::error::last_error(ec);
            return async_last_error(title, ec);
        }

        error::errors async_last_error(
            char const * title, 
            error_code & ec) const
        {
            if (ec && ec != boost::asio::error::would_block) {
                LOG_WARN(title << ": " << ec.message());
            }
            return ppbox::error::last_error_enum(ec);
        }

    private:
        void opem_mux(void)
        {
            if (strncasecmp(format_, "flv")) {
                mux_ = new ppbox::mux::FlvMux;
            } else if (strncasecmp(format_, "ts")) {
                mux_ = new ppbox::mux::TsMux;
            } else {
                mux_ = new ppbox::mux::FlvMux;
            }
        }

        void close_mux(void)
        {
            if (mux_) {
                delete mux_;
                mux_ = NULL;
            }
        }

    private:
        ppbox::demux::DemuxerModule & demux_mod_;
        ppbox::mux::Muxer * mux_;
        ppbox::demux::Sample read_tag_;
        TagReadPosition tag_read_position_;
        Adapter_Open_Callback open_callback_;
        boost::shared_ptr<Cache> cache_;
        boost::uint32_t buffer_time_;

        // 配置
        std::string playlink_;
        std::string format_;
    };

}

static ppbox::IAdapter & adapter()
{
    static ppbox::IAdapter the_adapter;
    return the_adapter;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus

    //打开一个适配视频
    PPBOX_DECL PP_int32 Adapter_Open(
        PP_char const * playlink,
        PP_char const * format)
    {
        return adapter().adapter_open(playlink, format);
    }

    //异步打开一个适配视频
    PPBOX_DECL void Adapter_AsyncOpen(
        PP_char const * playlink, 
        PP_char const * format,
        Adapter_Open_Callback callback)
    {
        adapter().adapter_async_open(playlink, format, callback);
    }

    //适配暂停
    PPBOX_DECL PP_int32 Adapter_Pause()
    {
        return adapter().adapter_pause();
    }

    PPBOX_DECL PP_int32 Adapter_Resume()
    {
        return adapter().adapter_resume();
    }

    //强制适配结束
    PPBOX_DECL void Adapter_Close()
    {
        adapter().adapter_close();
    }

    //跳到某个时刻开始播放
    PPBOX_DECL PP_int32 Adapter_Seek(
        PP_uint32 start_time)
    {
        return adapter().adapter_seek(start_time);
    }

    //重新获取头部数据
    PPBOX_DECL void Adapter_Reset()
    {
        adapter().adapter_reset();
    }

    //读取适配影片数据
    PPBOX_DECL PP_int32 Adapter_Read(
        unsigned char * buffer,
        PP_uint32 buffer_size,
        PP_uint32 * read_size)
    {
        return adapter().adapter_read(buffer, buffer_size, *read_size);
    }

    PPBOX_DECL PP_int32 Adapter_GetPlayMsg(
        Adapter_PlayStatistic * statistic_Msg)
    {
        return adapter().adapter_get_play_stat(*statistic_Msg);
    }

    PPBOX_DECL PP_int32 Adapter_GetMediaInfo(
        Adapter_Mediainfo * media_info)
    {
        return adapter().adapter_get_mediainfo(*media_info);
    }

    PPBOX_DECL PP_int32 Adapter_GetPlayTime(
        PP_uint64 * time)
    {
        return adapter().adapter_get_playing_time(*time);
    }

#if __cplusplus
}
#endif // __cplusplus

#endif

