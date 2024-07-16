#ifndef __asr_H_
#define __asr_H_
#include "uart.h"
void ASR();
void speaking(unsigned char b);
void ASR_init(void);                                //语音模块初始化
void ASR_init1(void);                               //灵敏度初始化
void ASR_init2(void);                               //音量输入初始化
void start_repeatedly_Speech_Recognition(void);     //启动连续识别
void human_computer();
void human_computer2();
#endif