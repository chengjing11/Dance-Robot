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
void ASR_init(void)                             //����ʶ��ģ���ʼ��
{  IE2 &= 0xFE; 			                         // ����2�жϹر�
	 SendDataByUart2(0xA0);   
	 SendDataByUart2(0xA0);
	 SendDataByUart2(0xA0);
	 IE2 |= 0x01;  	// ����2�жϴ�	
	 delay_ms(1000);
}
void ASR_init1(void)                           //�����ȳ�ʼ��
{  IE2 &= 0xFE; 			                         // ����2�жϹر�
	 SendDataByUart2(0xA1);
	 SendDataByUart2(0x00);
	 SendDataByUart2(0x00);
   IE2 |= 0x01;                               // ����2�жϴ�
   delay_ms(1000);	                                                                
}
void ASR_init2(void)                           //���������ʼ��
{  IE2 &= 0xFE; 			                         // ����2�жϹر�
	 SendDataByUart2(0xA2);
	 SendDataByUart2(0x00);
	 SendDataByUart2(0x00);
   IE2 |= 0x01;                               // ����2�жϴ�
   delay_ms(1000);	
}
//������������ʶ����start_repeatedly_Speech_Recognition()
void start_repeatedly_Speech_Recognition(void)
{ 
	IE2 &= 0xFE; 			                         // ����2�жϹر�
  SendDataByUart2(0xAB);
	SendDataByUart2(0xAB);
	SendDataByUart2(0x00);
	IE2 |= 0x01;                               // ����2�жϴ�
  delay_ms(1000);	
}
//�˳�����ʶ�𣬽�������״̬
void ASR_end(void)                         
{  IE2 &= 0xFE; 			                         // ����2�жϹر�
	 SendDataByUart2(0xAC);
	 SendDataByUart2(0xAC);
	 SendDataByUart2(0x00);
   IE2 |= 0x01;                               // ����2�жϴ�
   delay_ms(1000);	
}
//================================================================================================
void human_computer()
{  
	 ASR_init();                            //����ʶ��ģ���ʼ��
	 ASR_init1();                           //�����ȳ�ʼ��
	 ASR_init2();                           //���������ʼ��
	 start_repeatedly_Speech_Recognition(); //��������ʶ��
		while(cmd!=0x01)
		{ 
			if(Rec_Buf2[0]==0x5B)
			{
				delay_ms(500);                                     //�ȴ�������λ������
				EA=0;                                              //�ر����ж�
	      SendDataByUart1(Rec_Buf2[0])	;
        SendDataByUart1(Rec_Buf2[1])	;				
				switch (Rec_Buf2[1])
				{
					case 1:P2=~0X02;
					       Delay2500ms(); 
        				 P2=0X00 ;
					       break;                                     //����
  				case 2:P2=~0X03;
					       Delay5000ms();  
					       P2=0X00; 
					       break;                                     //����17�����飬ĿǰΪֹ���Ѿ����������
          case 3:P2=~0X04;
					       Delay10000ms(); 
					       P2=0X00; 
					       break;                                     //�����������һ���ӡ����������ȶ��Լ���һ�ֶ�����ʹ�Ҹ��ӵ����ɣ����������ǶԹ��죬�Թ��ң��������������
					case 4:P2=~0X05;
					       Delay6000ms();  
					       P2=0X00; 
					       break;                                      //�ҵ��ϰ೤�����������У��ϰ೤����һ�����ģ���������ǰ����
  				case 5:P2=~0X06;
					       Delay3500ms();  
					       P2=0X00; 
					       cmd=0x01;                                   //׼���˳�����ģ��
					       break;                                      //�ú�ѧϰ��Ϊ�����⡣
				}
				CLR_Buf2();
				delay_ms(200);
				EA=1;                                               //�����ж�
			}
		}
    ASR_end();                                             //�˳�����ʶ��
}