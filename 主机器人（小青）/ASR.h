#ifndef __asr_H_
#define __asr_H_
#include "uart.h"
void ASR();
void speaking(unsigned char b);
void ASR_init(void);                                //����ģ���ʼ��
void ASR_init1(void);                               //�����ȳ�ʼ��
void ASR_init2(void);                               //���������ʼ��
void start_repeatedly_Speech_Recognition(void);     //��������ʶ��
void human_computer();
void human_computer2();
#endif