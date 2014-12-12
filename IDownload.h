// IDownload.h

#ifndef _JUST_EXPORT_I_DOWNLOAD_H_
#define _JUST_EXPORT_I_DOWNLOAD_H_

#include "IGeneric.h"

//��һ����������
JUST_FUNC_4(PP_handle, JUST_DownloadOpen, (
    (PP_str, playlink),
    (PP_str, format),
    (PP_str, save_filename),
    (JUST_Callback, resp)));

//�ر�ָ������������
JUST_FUNC_1(PP_err, JUST_DownloadClose, (
    (PP_handle, handle)));

JUST_STRUCT_3(JUST_DownloadStatistic, (
    (PP_ulong, total_size),
    (PP_ulong, finish_size),
    (PP_uint, speed) 
    ));

// ��ȡָ������������ʵʱͳ����Ϣ
JUST_FUNC_2(PP_err, JUST_GetDownloadInfo, (
    (PP_handle, handle),
    (JUST_DownloadStatistic *, stat)));

#endif // _JUST_EXPORT_I_DOWNLOAD_H_
