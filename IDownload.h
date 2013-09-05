// IDownload.h

#ifndef _PPBOX_PPBOX_I_DOWNLOAD_H_
#define _PPBOX_PPBOX_I_DOWNLOAD_H_

#include "IGeneric.h"

//打开一个下载用例
PPBOX_FUNC_4(PP_handle, PPBOX_DownloadOpen, (
    (PP_str, playlink),
    (PP_str, format),
    (PP_str, save_filename),
    (PPBOX_Callback, resp)));

//关闭指定的下载用例
PPBOX_FUNC_1(PP_err, PPBOX_DownloadClose, (
    (PP_handle, handle)));

PPBOX_STRUCT_3(PPBOX_DownloadStatistic, (
    (PP_ulong, total_size),
    (PP_ulong, finish_size),
    (PP_uint, speed) 
    ));

// 获取指定下载用例的实时统计信息
PPBOX_FUNC_2(PP_err, PPBOX_GetDownloadInfo, (
    (PP_handle, handle),
    (PPBOX_DownloadStatistic *, stat)));

#endif // _PPBOX_PPBOX_I_DOWNLOAD_H_
