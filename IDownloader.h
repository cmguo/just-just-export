// IDownloader.h

#ifndef _PPBOX_PPBOX_I_DOWNLOADER_H_
#define _PPBOX_PPBOX_I_DOWNLOADER_H_

#include "IPpbox.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    //打开一个下载用例
    PPBOX_DECL PP_handle PPBOX_DownloadOpen(
        PP_str playlink,
        PP_str format,
        PP_str save_filename,
        PPBOX_Callback resp);

    //关闭指定的下载用例
    PPBOX_DECL PP_err PPBOX_DownloadClose(PP_handle hander);

    typedef struct tag_PPBOX_DownloadStatistic
    {
        PP_ulong total_size;
        PP_ulong finish_size;
        PP_uint speed; 
    } PPBOX_DownloadStatistic;

    // 获取指定下载用例的实时统计信息
    PPBOX_DECL PP_err PPBOX_GetDownloadInfo(
        PP_handle hander,
        PPBOX_DownloadStatistic * stat);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DOWNLOADER_H_
