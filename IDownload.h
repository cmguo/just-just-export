// IDownload.h

#ifndef _JUST_EXPORT_I_DOWNLOAD_H_
#define _JUST_EXPORT_I_DOWNLOAD_H_

#include "IGeneric.h"

//打开一个下载用例
JUST_FUNC_4(PP_handle, JUST_DownloadOpen, (
    (PP_str, playlink),
    (PP_str, format),
    (PP_str, save_filename),
    (JUST_Callback, resp)));

//关闭指定的下载用例
JUST_FUNC_1(PP_err, JUST_DownloadClose, (
    (PP_handle, handle)));

JUST_STRUCT_3(JUST_DownloadStatistic, (
    (PP_ulong, total_size),
    (PP_ulong, finish_size),
    (PP_uint, speed) 
    ));

// 获取指定下载用例的实时统计信息
JUST_FUNC_2(PP_err, JUST_GetDownloadInfo, (
    (PP_handle, handle),
    (JUST_DownloadStatistic *, stat)));

#endif // _JUST_EXPORT_I_DOWNLOAD_H_
