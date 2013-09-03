// IDemuxer.h

#ifndef _PPBOX_PPBOX_I_DEMUXER_H_
#define _PPBOX_PPBOX_I_DEMUXER_H_

#include "IGeneric.h"
#include "IFormat.h"

//��һ����Ƶ
PPBOX_FUNC_1(PP_err, PPBOX_Open, (
    (PP_str, playlink)));

PPBOX_FUNC_2(PP_err, PPBOX_OpenEx, (
    (PP_str, playlink), 
    (PP_str, format)));

PPBOX_CALLBACK_1(void, PPBOX_Open_Callback, (PP_err));

PPBOX_FUNC_2(PP_err, PPBOX_AsyncOpen, (
    (PP_str, playlink), 
    (PPBOX_Open_Callback, callback)));

PPBOX_FUNC_4(PP_err, PPBOX_AsyncOpenEx, (
    (PP_str, playlink), 
    (PP_str, format), 
    (PP_context, user_data), 
    (PPBOX_Callback, callback)));

//��ͣ
PPBOX_FUNC_0(PP_err, PPBOX_Pause, ());

//ǿ�ƽ���
PPBOX_FUNC_0(PP_err, PPBOX_Close, ());

//����ж�����
PPBOX_FUNC_0(PP_uint, PPBOX_GetStreamCount, ());


//���������ϸ��Ϣ
PPBOX_FUNC_2(PP_err, PPBOX_GetStreamInfo, (
    (PP_uint, index), 
    (PPBOX_StreamInfo *, info)));

//�����ʱ��
PPBOX_FUNC_0(PP_uint, PPBOX_GetDuration, ());

//����ĳ��ʱ�̿�ʼ����
PPBOX_FUNC_1(PP_err, PPBOX_Seek, (
    (PP_uint, time)));

//ͬ����ȡSample�ӿڣ�������
PPBOX_FUNC_1(PP_err, PPBOX_ReadSample, (
    (PPBOX_Sample *, sample)));

//���ò��Ż������Ļ���ʱ�� (��ʱ���Ե���)
// ��Ҫ���ڼ��㲥��״̬��������������������Ĭ��3s
// ��� ���ػ��������ݵ���ʱ�� < ���Ż���ʱ�� �� ���� buffering ״̬
// ��� ���ػ��������ݵ���ʱ�� >=���Ż���ʱ�� �� ���� playing ״̬
// ��� ��Ϊ������ Pause ʹ֮��ͣ�ģ����� Pausing ״̬
// �������buffering״̬�����ػ��������ݵ���ʱ��/���Ż���ʱ��*100%���ǻ���ٷֱ�
PPBOX_FUNC_1(PP_err, PPBOX_SetPlayBufferTime, (
    (PP_uint, time)));

//�������ػ�������С ��ֻ����Openǰ���ã�
// ��Ҫ���ڿ����ڴ棬��������ٶȴ���ReadSample���ٶȣ���ô���ص����ݽ����
// ���ڴ�֮�У����ڴ��е����ػ���������Ԥ��ֵ����ô��ֹͣ���ء�ֱ����������
// ReadSample������һЩ�ڴ�ռ�ú��ټ������أ�
// length: Ԥ��������ڴ滺��Ĵ�С
PPBOX_FUNC_1(PP_err, PPBOX_SetDownloadBufferSize, (
    (PP_uint, length)));

//��ò�����Ϣ
//����ֵ: ������
//    ppbox_success      ��ʾ�ɹ�
//    ������ֵ��ʾʧ��
PPBOX_FUNC_1(PP_err, PPBOX_GetPlayStat, (
    (PPBOX_PlayStatistic *, stat)));

//���������Ϣ
//����ֵ: ������
//    ppbox_success      ��ʾ�ɹ�
//    ������ֵ��ʾʧ��
PPBOX_FUNC_1(PP_err, PPBOX_GetNetStat, (
    (PPBOX_NetStatistic *, stat)));

#endif // _PPBOX_PPBOX_I_DEMUXER_H_
