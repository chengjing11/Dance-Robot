# include "uart.h"
# include "NARFL2401.h"
#include "asr.h"
# include "open.H"
#include "duoji.h"
extern  unsigned char cmd;

void main ()
{ 
	wagh();                       //IO初始化
	Uart1234_Init();              //四串口初始化
	Init_NRF24L01_MA();	          //初始化
	Set_RxMode_MA();	  				  //配置nRF24L01为接收模式
	zhongduanyi();                //中断初始化
	P2=0X00;
	start();
	while(1)
	{   
		switch (cmd)
			{
					case 0 :SendStringByUart1("0x00  ");break;                            //等待启动
					case 1 :SendStringByUart1("0x01  ");break;                            //紧急停止
					case 2 :SendStringByUart1("0x02  ");dancing();                        //舞蹈启动
					case 3 :SendStringByUart1("0x03  ");                                  //人机交互 
									Voice_interaction();
					case 4 :SendStringByUart1("0x04  ");  
									Visual_interaction();            
									break;          
					default :cmd=0;break;
			}
				delay_ms(200);
			}
}