# include "uart.h"
# include "NARFL2401.h"
#include "asr.h"
# include "open.H"
#include "duoji.h"
extern  unsigned char cmd;

void main ()
{ 
	wagh();                       //IO��ʼ��
	Uart1234_Init();              //�Ĵ��ڳ�ʼ��
	Init_NRF24L01_MA();	          //��ʼ��
	Set_RxMode_MA();	  				  //����nRF24L01Ϊ����ģʽ
	zhongduanyi();                //�жϳ�ʼ��
	P2=0X00;
	start();
	while(1)
	{   
		switch (cmd)
			{
					case 0 :SendStringByUart1("0x00  ");break;                            //�ȴ�����
					case 1 :SendStringByUart1("0x01  ");break;                            //����ֹͣ
					case 2 :SendStringByUart1("0x02  ");dancing();                        //�赸����
					case 3 :SendStringByUart1("0x03  ");                                  //�˻����� 
									Voice_interaction();
					case 4 :SendStringByUart1("0x04  ");  
									Visual_interaction();            
									break;          
					default :cmd=0;break;
			}
				delay_ms(200);
			}
}