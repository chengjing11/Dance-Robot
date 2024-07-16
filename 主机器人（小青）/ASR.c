#include "asr.h"
#include "duoji.h"
void SendDataByUart2(unsigned char dat);
void SendStringByUart1(unsigned char *s);
extern unsigned char cmd;
extern unsigned char xdata Rec_Buf2[3]; 
extern unsigned char xdata Rec_Buf4[2];
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
void Voice_interaction()
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
					case 1:P2=~0X11;
					       Delay2500ms(); 
        				 P2=0X00 ;
					       break;                                     //����
  				case 2:P2=~0X12;
					       Delay5000ms();  
					       P2=0X00; 
					       break;                                     //����17�����飬ĿǰΪֹ���Ѿ����������
          case 3:P2=~0X13;
					       Delay10000ms(); 
					       P2=0X00; 
					       break;                                     //�����������һ���ӡ����������ȶ��Լ���һ�ֶ�����ʹ�Ҹ��ӵ����ɣ����������ǶԹ��죬�Թ��ң��������������
					case 4:P2=~0X14;
					       Delay6000ms();  
					       P2=0X00; 
					       break;                                      //�ҵ��ϰ೤�����������У��ϰ೤����һ�����ģ���������ǰ����
  				case 5:P2=~0X15;
					       Delay3500ms();  
					       P2=0X00; 
					       cmd=0x05;                                   //׼���˳�����ģ��
					       break;                                      //�ú�ѧϰ��Ϊ�����⡣
				}
				delay_ms(200);
				CLR_Buf2();
				EA=1;                                               //�����ж�
			}
		}
    ASR_end();                                             //�˳�����ʶ��
}

void Visual_interaction()
{ 
	SendDataByUart4(0x31);                  //������һ����ɫʶ��
	SendDataByUart4(0x31);                  //������һ����ɫʶ��
	SendDataByUart4(0x31);                  //������һ����ɫʶ��
  while(1)
	{  
		  if (Rec_Buf4[0]==0x31||0x32||0x33)
			{
					EA=0; 
					switch (Rec_Buf4[0])
					{
					case 0x31:   SendStringByUart1("red");   P2=~0X1B; delay_ms(3500);   P2=0X00;break;    
					case 0x32:   SendStringByUart1("green"); P2=~0X1C; delay_ms(3000);   P2=0X00; break;   
					case 0x33:   SendStringByUart1("purple"); P2=~0X1D; delay_ms(3000);  P2=0X00; break;   
					}
					CLR_Buf4();
					EA=1;
		      SendStringByUart4("1");             //������һ����ɫʶ��
					SendStringByUart4("1");             //������һ����ɫʶ��
		 			SendStringByUart4("1");             //������һ����ɫʶ��
	  	}
		}
}