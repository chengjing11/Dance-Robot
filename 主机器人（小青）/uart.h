#ifndef __uart_H_
#define __uart_H_
#define    MAIN_Fosc		11059200L 	//定义主时钟
#include	"15W4KxxS4.H"
#include <intrins.h>					// 加入此头文件后,可使用_nop_库函数
#include <string.h>           // 加入此头文件后,可使用strstr库函数

//==========================================================================
void wagh();
void delay_ms(unsigned  int x);
void Uart1234_Init(void);

//void CLR_Buf1(void);
void SendDataByUart1(unsigned char dat);
void SendStringByUart1(unsigned char *s);

void CLR_Buf2(void);
void SendDataByUart2(unsigned char dat);
//void SendStringByUart2(unsigned char *s);

//void CLR_Buf3(void);
void SendDataByUart3(unsigned char dat);
//void SendStringByUart3(unsigned char *s);

void CLR_Buf4(void);
bit Hand4(unsigned char *a);
void SendDataByUart4(unsigned char dat);
void SendStringByUart4(unsigned char *s);
#endif