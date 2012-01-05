// IDownloader.h

#ifndef _PPBOX_PPBOX_I_DOWNLOADER_H_
#define _PPBOX_PPBOX_I_DOWNLOADER_H_

#include "IPpbox.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    // refine
    typedef void * PPBOX_Download_Hander;

    typedef struct tag_PPBOX_frame // IN
    {
        PP_uint32 stream_index;     // ���ı��
        PP_uint32 start_time;       // Sample��Ӧ��ʱ���
        PP_uint32 buffer_length;    // Frame�Ĵ�С
        PP_uchar const * buffer;    // Frame������
    } PPBOX_Frame;

    //��һ����������
    PPBOX_DECL PP_int32 FileDownloadOpenItem(
                char const * playlink,
                char const * format,
                char const * save_filename,
                PPBOX_Download_Hander * handle);

    //�ر�ָ������������
    PPBOX_DECL void FileDownloadCloseItem(PPBOX_Download_Hander hander);

    //��ȡ��ǰ���ض���������ʵ���ĸ���
    PPBOX_DECL PP_uint32 GetFileDownloadItemCount();

    typedef struct PPboxDownloadStatistic
    {
        float finish_percent;
        PP_uint64 finish_size;
        PP_uint32 speed; // B/s
    }Download_Statistic;

    // ��ȡָ������������ʵʱͳ����Ϣ
    PPBOX_DECL PP_int32 GetFileDownloadItemInfo(
        PPBOX_Download_Hander hander,
        Download_Statistic * stat);

    // �������ز�����name��ʽҪ�����value��ʾֵ
    PPBOX_DECL PP_int32 SetParamenter(char const * name, char const * value);

    // ��ȡ������
    PPBOX_DECL PP_int32 DownloadLastError(PPBOX_Download_Hander hander);


    // record
    PPBOX_DECL PP_int32 RecordCreate(void);

    PPBOX_DECL void SetPoolSize(PP_uint32 size);

    PPBOX_DECL void SetAudioInfo(
        PP_uint32 index,
        PP_uint32 sample_rate,
        PP_uint32 channel_count,
        PP_uint32 sample_size,
        PP_uint32 time_scale,
        char * const spec_buffer,
        PP_uint32 buffer_size);

    PPBOX_DECL void SetVideoInfo(
        boost::uint32_t index,
        boost::uint32_t width,
        boost::uint32_t height,
        boost::uint32_t frame_rate,
        boost::uint32_t time_scale,
        char * const spec_buffer,
        PP_uint32 buffer_size);

    PPBOX_DECL void Push(PPBOX_Frame const * frame);

    PPBOX_DECL void Destory(void);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DOWNLOADER_H_
