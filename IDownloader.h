// IDownloader.h

#ifndef _PPBOX_PPBOX_I_DOWNLOADER_H_
#define _PPBOX_PPBOX_I_DOWNLOADER_H_

#include "IPpbox.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    //��һ����������
    PPBOX_DECL PP_handle PPBOX_DownloadOpen(
        PP_str playlink,
        PP_str format,
        PP_str save_filename,
        PPBOX_Callback resp);

    //�ر�ָ������������
    PPBOX_DECL PP_err PPBOX_DownloadClose(PP_handle hander);

    typedef struct tag_PPBOX_DownloadStatistic
    {
        PP_ulong total_size;
        PP_ulong finish_size;
        PP_uint speed; 
    } PPBOX_DownloadStatistic;

    // ��ȡָ������������ʵʱͳ����Ϣ
    PPBOX_DECL PP_err PPBOX_GetDownloadInfo(
        PP_handle hander,
        PPBOX_DownloadStatistic * stat);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DOWNLOADER_H_
