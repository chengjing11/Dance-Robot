#include "asr.h"
#include "duoji.h"
void SendDataByUart2(unsigned char dat);
void SendStringByUart1(unsigned char *s);
extern unsigned char cmd;
extern unsigned char xdata Rec_Buf2[3]; 
//-----------------------------------------------------------------
void Delay2500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 106;
	j = 15;
	k = 131;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay5000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 211;
	j = 30;
	k = 11;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay10000ms()		//@11.0592MHz
{
	Delay5000ms();
	Delay5000ms(); 
}
void Delay6000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 253;
	j = 35;
	k = 220;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay3500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 148;
	j = 21;
	k = 83;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

//==============================================================
void ASR_init(void)                             //语音识别模块初始化
{  IE2 &= 0xFE; 			                         // 串口2中断关闭
	 SendDataByUart2(0xA0);   
	 SendDataByUart2(0xA0);
	 SendDataByUart2(0xA0);
	 IE2 |= 0x01;  	// 串口2中断打开	
	 delay_ms(1000);
}
void ASR_init1(void)                           //灵敏度初始化
{  IE2 &= 0xFE; 			                         // 串口2中断关闭
	 SendDataByUart2(0xA1);
	 SendDataByUart2(0x00);
	 SendDataByUart2(0x00);
   IE2 |= 0x01;                               // 串口2中断打开
   delay_ms(1000);	                                                                
}
void ASR_init2(void)                           //音量输入初始化
{  IE2 &= 0xFE; 			                         // 串口2中断关闭
	 SendDataByUart2(0xA2);
	 SendDataByUart2(0x00);
	 SendDataByUart2(0x00);
   IE2 |= 0x01;                               // 串口2中断打开
   delay_ms(1000);	
}
//启动连续语音识别函数start_repeatedly_Speech_Recognition()
void start_repeatedly_Speech_Recognition(void)
{ 
	IE2 &= 0xFE; 			                         // 串口2中断关闭
  SendDataByUart2(0xAB);
	SendDataByUart2(0xAB);
	SendDataByUart2(0x00);
	IE2 |= 0x01;                               // 串口2中断打开
  delay_ms(1000);	
}
//退出语音识别，进入休眠状态
void ASR_end(void)                         
{  IE2 &= 0xFE; 			                         // 串口2中断关闭
	 SendDataByUart2(0xAC);
	 SendDataByUart2(0xAC);
	 SendDataByUart2(0x00);
   IE2 |= 0x01;                               // 串口2中断打开
   delay_ms(1000);	
}
//================================================================================================
void human_computer()
{  
	 ASR_init();                            //语音识别模块初始化
	 ASR_init1();                           //灵敏度初始化
	 ASR_init2();                           //音量输入初始化
	 start_repeatedly_Speech_Recognition(); //启动连续识别
		while(cmd!=0x01)
		{ 
			if(Rec_Buf2[0]==0x5B)
			{
				delay_ms(500);                                     //等待接收三位数数据
				EA=0;                                              //关闭总中断
	      SendDataByUart1(Rec_Buf2[0])	;
        SendDataByUart1(Rec_Buf2[1])	;				
				switch (Rec_Buf2[1])
				{
					case 1:P2=~0X02;
					       Delay2500ms(); 
        				 P2=0X00 ;
					       break;                                     //可以
  				case 2:P2=~0X03;
					       Delay5000ms();  
					       P2=0X00; 
					       break;                                     //我是17年入伍，目前为止，已经有两年军龄
          case 3:P2=~0X04;
					       Delay10000ms(); 
					       P2=0X00; 
					       break;                                     //“不当兵后悔一辈子”，当兵首先对自己是一种锻炼，使我更加的自律，但更深层次是对国旗，对国家，对人民的信仰。
					case 4:P2=~0X05;
					       Delay6000ms();  
					       P2=0X00; 
					       break;                                      //我的老班长，当兵过程中，老班长就像一面旗帜，引领我们前进。
  				case 5:P2=~0X06;
					       Delay3500ms();  
					       P2=0X00; 
					       cmd=0x01;                                   //准备退出语音模块
					       break;                                      //好好学习，为国争光。
				}
				CLR_Buf2();
				delay_ms(200);
				EA=1;                                               //打开总中断
			}
		}
    ASR_end();                                             //退出语音识别
}