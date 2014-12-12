// IDownload.cpp

#include "just/export/Common.h"
#include "just/export/IDownload.h"
#include "just/export/Callback.h"
using namespace just::error;

#include <just/download/DownloadModule.h>
#include <just/download/Downloader.h>
using namespace just::download;

#include <boost/bind.hpp>
using namespace boost::system;

#ifndef JUST_DISABLE_DOWNLOAD

namespace just
{

    class IDownload
    {
    public:
        IDownload()
            : download_manager_(util::daemon::use_module<just::download::DownloadModule>(global_daemon()))
        {
        }

        ~IDownload()
        {
        }
        
        static void download_open_callback(
            JUST_Callback callback,
            error_code const & ec, 
            Downloader * downloader)
        {
            if (NULL != callback) {
#ifndef JUST_ENABLE_REDIRECT_CALLBACK
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
            JUST_Callback resp)
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
                    boost::bind(&IDownload::download_open_callback, resp, _1, _2));
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
            JUST_DownloadStatistic & statistic)
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
            just::error::last_error(ec);
            return async_last_error(title, ec);
        }

        static error::errors async_last_error(
            PP_str title, 
            error_code const & ec) 
        {
            if (ec && ec != boost::asio::error::would_block) {
                std::cout << title << ": " << ec.message() << std::endl;
            }
            return just::error::last_error_enum(ec);
        }

    private:
        just::download::DownloadModule & download_manager_;
    };

}

static just::IDownload & downloader()
{
    static just::IDownload the_downloader;
    return the_downloader;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus


    //打开一个下载用例
    JUST_DECL PP_handle JUST_DownloadOpen(
        PP_str playlink,
        PP_str format,
        PP_str save_filename,
        JUST_Callback resp)
    {
        return downloader().download_open(playlink, format, save_filename, resp);
    }

    //关闭指定的下载用例
    JUST_DECL PP_err JUST_DownloadClose(
        PP_handle hander)
    {
        return downloader().download_close(hander);
    }

    // 获取指定下载用例的实时统计信息
    JUST_DECL PP_err JUST_GetDownloadInfo(
        PP_handle hander,
        JUST_DownloadStatistic * stat)
    {
        return downloader().download_get_statistic(hander, *stat);
    }


#if __cplusplus
}
#endif // __cplusplus

#endif

