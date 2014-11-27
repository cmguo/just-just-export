// IDemux.h

#ifndef _JUST_JUST_I_DEMUX_H_
#define _JUST_JUST_I_DEMUX_H_

#include "IGeneric.h"
#include "IFormat.h"

//��һ����Ƶ
JUST_FUNC_1(PP_err, JUST_Open, (
    (PP_str, playlink)));

JUST_FUNC_2(PP_err, JUST_OpenEx, (
    (PP_str, playlink), 
    (PP_str, format)));

JUST_CALLBACK_1(void, JUST_Open_Callback, (PP_err));

JUST_FUNC_2(PP_err, JUST_AsyncOpen, (
    (PP_str, playlink), 
    (JUST_Open_Callback, callback)));

JUST_FUNC_4(PP_err, JUST_AsyncOpenEx, (
    (PP_str, playlink), 
    (PP_str, format), 
    (PP_context, user_data), 
    (JUST_Callback, callback)));

//��ͣ
JUST_FUNC_0(PP_err, JUST_Pause, ());

//ǿ�ƽ���
JUST_FUNC_0(PP_err, JUST_Close, ());

//����ж�����
JUST_FUNC_0(PP_uint, JUST_GetStreamCount, ());


//���������ϸ��Ϣ
JUST_FUNC_2(PP_err, JUST_GetStreamInfo, (
    (PP_uint, index), 
    (JUST_StreamInfo *, info)));

//�����ʱ��
JUST_FUNC_0(PP_uint, JUST_GetDuration, ());

//����ĳ��ʱ�̿�ʼ����
JUST_FUNC_1(PP_err, JUST_Seek, (
    (PP_uint, time)));

//ͬ����ȡSample�ӿڣ�������
JUST_FUNC_1(PP_err, JUST_ReadSample, (
    (JUST_Sample *, sample)));

//���ò��Ż������Ļ���ʱ�� (��ʱ���Ե���)
// ��Ҫ���ڼ��㲥��״̬��������������������Ĭ��3s
// ��� ���ػ��������ݵ���ʱ�� < ���Ż���ʱ�� �� ���� buffering ״̬
// ��� ���ػ��������ݵ���ʱ�� >=���Ż���ʱ�� �� ���� playing ״̬
// ��� ��Ϊ������ Pause ʹ֮��ͣ�ģ����� Pausing ״̬
// �������buffering״̬�����ػ��������ݵ���ʱ��/���Ż���ʱ��*100%���ǻ���ٷֱ�
JUST_FUNC_1(PP_err, JUST_SetPlayBufferTime, (
    (PP_uint, time)));

//�������ػ�������С ��ֻ����Openǰ���ã�
// ��Ҫ���ڿ����ڴ棬��������ٶȴ���ReadSample���ٶȣ���ô���ص����ݽ����
// ���ڴ�֮�У����ڴ��е����ػ���������Ԥ��ֵ����ô��ֹͣ���ء�ֱ����������
// ReadSample������һЩ�ڴ�ռ�ú��ټ������أ�
// length: Ԥ��������ڴ滺��Ĵ�С
JUST_FUNC_1(PP_err, JUST_SetDownloadBufferSize, (
    (PP_uint, length)));

//��ò�����Ϣ
//����ֵ: ������
//    just_success      ��ʾ�ɹ�
//    ������ֵ��ʾʧ��
JUST_FUNC_1(PP_err, JUST_GetPlayStat, (
    (JUST_PlayStatistic *, stat)));

//���������Ϣ
//����ֵ: ������
//    just_success      ��ʾ�ɹ�
//    ������ֵ��ʾʧ��
JUST_FUNC_1(PP_err, JUST_GetDataStat, (
    (JUST_DataStat *, stat)));

#endif // _JUST_JUST_I_DEMUX_H_
