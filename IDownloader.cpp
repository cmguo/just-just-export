// IDownloader.cpp

#ifndef PPBOX_DISABLE_DOWNLOAD

#include "ppbox/ppbox/Common.h"
#define PPBOX_SOURCE
#include "ppbox/ppbox/IDownloader.h"
using namespace ppbox::error;

#include <ppbox/download/Manager.h>
using namespace ppbox::download;

#include <ppbox/demux/DemuxerModule.h>

using namespace boost::system;

//#ifndef PPBOX_DISABLE_DOWNLOAD

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

        error::errors Download_Open(
                      char const * playlink,
                      char const * format,
                      char const * filename,
                      PPBOX_Download_Hander & download_hander)
        {
            error_code ec;
            DownloadHander hander = NULL;
            download_manager_.add(playlink, format, store_path_.c_str(), filename, hander, ec);
            download_hander = (PPBOX_Download_Hander)hander;
            return last_error(__FUNCTION__, ec);
        }

        error::errors Download_Close(PPBOX_Download_Hander const hander)
        {
            error_code ec;
            download_manager_.del((DownloadHander)hander, ec);
            return last_error(__FUNCTION__, ec);
        }

        //refine
        boost::uint32_t Download_Get_Count(void)
        {
            return download_manager_.get_downloader_count();
        }

        error::errors Download_Get_Statistic(PPBOX_Download_Hander const hander,
                                             Download_Statistic & download_statistic)
        {
            error_code ec;

            DownloadStatistic statistic;
            download_manager_.get_download_statictis((DownloadHander)hander, statistic, ec);
            if (!ec) {
                download_statistic.finish_size = statistic.finish_size;
                download_statistic.speed       = statistic.speed;
                download_statistic.finish_percent  = statistic.finish_percent;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors Download_SetParamenter(char const * name,
                                             char const * value)
        {
            error_code ec;
            std::string para_name = name;
            std::string para_value = value;
            if ("storage" == para_name) {
                store_path_ = para_value;
            } else {
                ec = other_error;
            }
            return last_error(__FUNCTION__, ec);
        }

        error::errors Download_LastError(PPBOX_Download_Hander const handle)
        {
            error_code ec;

            ec = download_manager_.get_last_error((DownloadHander)handle);

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
                std::cout << title << ": " << ec.message() << std::endl;
            }
            return ppbox::error::last_error_enum(ec);
        }

    private:
        ppbox::download::Manager & download_manager_;
        std::string store_path_;
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
    PPBOX_DECL PP_int32 FileDownloadOpenItem(
        char const * playlink,
        char const * format,
        char const * save_filename,
        PPBOX_Download_Hander * hander)
    {
        return downloader().Download_Open(playlink, format, save_filename, *hander);
    }

    //关闭指定的下载用例
    PPBOX_DECL void FileDownloadCloseItem(PPBOX_Download_Hander hander)
    {
        downloader().Download_Close(hander);
    }

    //获取当前下载队列中下载实例的个数
    PPBOX_DECL PP_uint32 GetFileDownloadItemCount()
    {
        return downloader().Download_Get_Count();
    }

    // 获取指定下载用例的实时统计信息
    PPBOX_DECL PP_int32 GetFileDownloadItemInfo(
        PPBOX_Download_Hander hander,
        Download_Statistic * stat)
    {
        return downloader().Download_Get_Statistic(hander, *stat);
    }

    // 设置下载参数，name表式要设置项，value表示值
    PPBOX_DECL PP_int32 SetParamenter(char const * name, char const * value)
    {
        return downloader().Download_SetParamenter(name, value);
    }

    // 获取错误码
    PPBOX_DECL PP_int32 DownloadLastError(PPBOX_Download_Hander hander)
    {
        return downloader().Download_LastError(hander);
    }

#if __cplusplus
}
#endif // __cplusplus

#endif

