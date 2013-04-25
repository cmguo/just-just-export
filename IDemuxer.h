// IDemuxer.h

#ifndef _PPBOX_PPBOX_I_DEMUXER_H_
#define _PPBOX_PPBOX_I_DEMUXER_H_

#include "IPpbox.h"
#include "IFormat.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    //��һ����Ƶ
    PPBOX_DECL PP_err PPBOX_Open(
        PP_char const * playlink);

    typedef void (*PPBOX_Open_Callback)(PP_int32);

    PPBOX_DECL void PPBOX_AsyncOpen(
        PP_char const * playlink, 
        PPBOX_Open_Callback callback);

    PPBOX_DECL PP_err PPBOX_OpenEx(
        PP_char const * playlink, 
        PP_char const * format);

    PPBOX_DECL void PPBOX_AsyncOpenEx(
        PP_char const * playlink, 
        PP_char const * format, 
		void * user_data, 
        PPBOX_Callback callback);

    //��ͣ
    PPBOX_DECL PP_err PPBOX_Pause();

    //ǿ�ƽ���
    PPBOX_DECL void PPBOX_Close();

    //����ж�����
    PPBOX_DECL PP_uint32 PPBOX_GetStreamCount();

    //���������ϸ��Ϣ
    PPBOX_DECL PP_err PPBOX_GetStreamInfo(
        PP_uint32 index, 
        PPBOX_StreamInfo * info);

    //�����ʱ��
    PPBOX_DECL PP_uint32 PPBOX_GetDuration();

    //����ĳ��ʱ�̿�ʼ����
    PPBOX_DECL PP_err PPBOX_Seek(
        PP_uint32 start_time);

    //ͬ����ȡSample�ӿڣ�������
    PPBOX_DECL PP_err PPBOX_ReadSample(
        PPBOX_Sample * sample);

    //�������ػ�������С ��ֻ����Openǰ���ã�
    // ��Ҫ���ڿ����ڴ棬��������ٶȴ���ReadSample���ٶȣ���ô���ص����ݽ����
    // ���ڴ�֮�У����ڴ��е����ػ���������Ԥ��ֵ����ô��ֹͣ���ء�ֱ����������
    // ReadSample������һЩ�ڴ�ռ�ú��ټ������أ�
    // length: Ԥ��������ڴ滺��Ĵ�С
    PPBOX_DECL void PPBOX_SetDownloadBufferSize(
        PP_uint32 length);

    //���ò��Ż������Ļ���ʱ�� (��ʱ���Ե���)
    // ��Ҫ���ڼ��㲥��״̬��������������������Ĭ��3s
    // ��� ���ػ��������ݵ���ʱ�� < ���Ż���ʱ�� �� ���� buffering ״̬
    // ��� ���ػ��������ݵ���ʱ�� >=���Ż���ʱ�� �� ���� playing ״̬
    // ��� ��Ϊ������ Pause ʹ֮��ͣ�ģ����� Pausing ״̬
    // �������buffering״̬�����ػ��������ݵ���ʱ��/���Ż���ʱ��*100%���ǻ���ٷֱ�
    PPBOX_DECL void PPBOX_SetPlayBufferTime(
        PP_uint32 time);

    //��ò�����Ϣ
    //����ֵ: ������
    //    ppbox_success      ��ʾ�ɹ�
    //    ������ֵ��ʾʧ��
    PPBOX_DECL PP_err PPBOX_GetPlayMsg(
        PPBOX_PlayStatistic * statistic_Msg);

    //���������Ϣ
    // download_Msg: ��������ӿں����ڻ�ȡ��������Ϣ����
    //����ֵ: ������
    //    ppbox_success      ��ʾ�ɹ�
    //    ������ֵ��ʾʧ��
    PPBOX_DECL PP_err PPBOX_GetDownMsg(
        PPBOX_DownloadMsg* download_Msg);

    //��������ٶ���Ϣ
    // download_Msg: ��������ӿں����ڻ�ȡ��������Ϣ����
    //����ֵ: ������
    //    ppbox_success      ��ʾ�ɹ�
    //    ������ֵ��ʾʧ��
    PPBOX_DECL PP_err PPBOX_GetDownSedMsg(
        PPBOX_DownloadSpeedMsg* download_spped_Msg);

#if __cplusplus
}
#endif // __cplusplus

#endif // _PPBOX_PPBOX_I_DEMUXER_H_
