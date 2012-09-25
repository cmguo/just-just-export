// IDownloader.cpp

#include "ppbox/ppbox/Common.h"
#define PPBOX_SOURCE
#include "ppbox/ppbox/IDownloader.h"
using namespace ppbox::error;

#include <ppbox/download/Manager.h>
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
            : download_manager_(util::daemon::use_module<ppbox::download::Manager>(global_daemon()))
        {
        }

        ~IDownloader()
        {
        }
        
        static void download_open_callback(
            PPBOX_Download_Callback resp,
            error_code const & ec)
        {
            if (NULL != resp) {
                resp(async_last_error(__FUNCTION__, ec));
            } else {
                async_last_error(__FUNCTION__, ec);
            }
        }

        PPBOX_Download_Handle download_open(
            char const * playlink,
            char const * format,
            char const * filename,
            PPBOX_Download_Callback resp)
        {
            error_code ec;
            Downloader* hander = download_manager_.add(playlink, format, filename, ec, 
                    boost::bind(&IDownloader::download_open_callback, resp, _1 ));
            return  (PPBOX_Download_Handle)hander;
        }

        error::errors download_close(
            PPBOX_Download_Handle const hander)
        {
            error_code ec;
            download_manager_.del((Downloader*)hander, ec);
            return last_error(__FUNCTION__, ec);
        }

        error::errors download_get_statistic(
            PPBOX_Download_Handle const hander,
            PPBOX_DownloadStatistic & download_statistic)
        {
            error_code ec;

            DownloadStatistic statistic;
            download_manager_.get_download_statictis((Downloader*)hander, statistic, ec);
            if (!ec) {
                download_statistic.finish_size = statistic.finish_size;
                download_statistic.speed       = statistic.speed;
                download_statistic.total_size  = statistic.total_size;
            }
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
        ppbox::download::Manager & download_manager_;
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


    //��һ����������
    PPBOX_DECL PPBOX_Download_Handle PPBOX_DownloadOpen(
        char const * playlink,
        char const * format,
        char const * save_filename,
        PPBOX_Download_Callback resp)
    {
        return downloader().download_open(playlink, format, save_filename, resp);
    }

    //�ر�ָ������������
    PPBOX_DECL void PPBOX_DownloadClose(
        PPBOX_Download_Handle hander)
    {
        downloader().download_close(hander);
    }

    // ��ȡָ������������ʵʱͳ����Ϣ
    PPBOX_DECL PP_int32 PPBOX_GetDownloadInfo(
        PPBOX_Download_Handle hander,
        PPBOX_DownloadStatistic * stat)
    {
        return downloader().download_get_statistic(hander, *stat);
    }


#if __cplusplus
}
#endif // __cplusplus

#endif

