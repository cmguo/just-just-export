// IDownloader.cpp

#include "ppbox/ppbox/Common.h"
#define PPBOX_SOURCE
#include "ppbox/ppbox/IDownloader.h"
#include "ppbox/ppbox/Callback.h"
using namespace ppbox::error;

#include <ppbox/download/DownloadModule.h>
#include <ppbox/download/Downloader.h>
using namespace ppbox::download;

#include <boost/bind.hpp>
using namespace boost::system;

#ifndef PPBOX_DISABLE_DOWNLOAD

namespace ppbox
{

    class IDownloader
    {
    public:
        IDownloader()
            : download_manager_(util::daemon::use_module<ppbox::download::DownloadModule>(global_daemon()))
        {
        }

        ~IDownloader()
        {
        }
        
        static void download_open_callback(
            PPBOX_Callback callback,
            error_code const & ec, 
            Downloader * downloader)
        {
            if (NULL != callback) {
#ifndef PPBOX_ENABLE_REDIRECT_CALLBACK
                callback(downloader, async_last_error(__FUNCTION__, ec));
#else
                redirect_call().call(callback, (PP_context)downloader, (PP_err)async_last_error(__FUNCTION__, ec));
#endif
            } else {
                async_last_error(__FUNCTION__, ec);
            }
        }

        PP_handle download_open(
            PP_str playlink,
            PP_str format,
            PP_str filename,
            PPBOX_Callback resp)
        {
            error_code ec;
            framework::string::Url url(filename);
            framework::string::Url url_format(std::string("format:///?format=") + format);
            url.param("playlink", playlink);
            for (framework::string::Url::param_iterator iter = url_format.param_begin(); 
                iter != url_format.param_end(); ++iter) {
                    url.param(iter->key(), iter->value());
            }
            Downloader* hander = download_manager_.open(url, 
                    boost::bind(&IDownloader::download_open_callback, resp, _1, _2));
            return (PP_handle)hander;
        }

        PP_err download_close(
            PP_handle const hander)
        {
            error_code ec;
            download_manager_.close((Downloader*)hander, ec);
            return last_error(__FUNCTION__, ec);
        }

        PP_err download_get_statistic(
            PP_handle const hander,
            PPBOX_DownloadStatistic & statistic)
        {
            error_code ec;
            Downloader * downloader = (Downloader *)hander;
            DownloadStatistic stat;
            downloader->get_statictis(stat, ec);
            if (!ec) {
                statistic.finish_size = stat.finish_size;
                statistic.speed = stat.speed;
                statistic.total_size  = stat.total_size;
            }
            return last_error(__FUNCTION__, ec);
        }

        static error::errors last_error(
            PP_str title, 
            error_code const & ec)
        {
            ppbox::error::last_error(ec);
            return async_last_error(title, ec);
        }

        static error::errors async_last_error(
            PP_str title, 
            error_code const & ec) 
        {
            if (ec && ec != boost::asio::error::would_block) {
                std::cout << title << ": " << ec.message() << std::endl;
            }
            return ppbox::error::last_error_enum(ec);
        }

    private:
        ppbox::download::DownloadModule & download_manager_;
    };

}

static ppbox::IDownloader & downloader()
{
    static ppbox::IDownloader the_downloader;
    return the_downloader;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus


    //打开一个下载用例
    PPBOX_DECL PP_handle PPBOX_DownloadOpen(
        PP_str playlink,
        PP_str format,
        PP_str save_filename,
        PPBOX_Callback resp)
    {
        return downloader().download_open(playlink, format, save_filename, resp);
    }

    //关闭指定的下载用例
    PPBOX_DECL PP_err PPBOX_DownloadClose(
        PP_handle hander)
    {
        return downloader().download_close(hander);
    }

    // 获取指定下载用例的实时统计信息
    PPBOX_DECL PP_err PPBOX_GetDownloadInfo(
        PP_handle hander,
        PPBOX_DownloadStatistic * stat)
    {
        return downloader().download_get_statistic(hander, *stat);
    }


#if __cplusplus
}
#endif // __cplusplus

#endif

