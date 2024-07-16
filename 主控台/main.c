# include "narfl24l01.h"
# include "LCD2406.h"
sbit key = P0^4;
unsigned char a;
void three(unsigned int date)
{
		  TF_senddat(date);
			TF_senddat(date);
			TF_senddat(date);
}
void main ()
{
  wagh();
	Uart1234_Init();
	Init_NRF24L01_MA();
	Set_TxMode_MA();
	delay_ms(200);
	//SendStringByUart1("1");
	CLR_Buf1();
	while (1)
	{    
		if(key==0)
		{
			delay_ms(200);
		if(key==0)
		{
		 a++;
		}
		}
    switch(a)	
		{ 
			case 0 :  LCD2406("Power up!       Waitting cmd!   ");break;//µÈ´ý
		  case 1 :  three(0x02);a++;LCD2406("Start Dancing........          ");delay_ms(1000);break;//Æô¶¯Îèµ¸
			case 2 :  LCD2406("Dancing........                  ");break;//Îèµ¸
			
			case 3 :  three(0x01);a++;LCD2406("Emergency stop   button ...     ");delay_ms(1000);break;//½ô¼±Í£Ö¹
			case 4 :  LCD2406("Stop dancing -  Waitting cmd.....     ");break;//µÈ´ý

			case 5 :  a=0;break;
		}			

	}
}