# include "uart.h"
#define  uint8   unsigned char
#define  uint16  unsigned int 
extern  unsigned char cmd;
#define BAUD  115200			                 // 波特率
#define BAUD3 19200	
#define BAUD4 19200
#define S2_S  0x01	
#define S2RI 0x01
#define S2TI 0x02
#define S3RI 0x01
#define S3TI 0x02
#define S4RI 0x01
#define S4TI 0x02
#define Buf_MaxOPEN 1                     //定义数组大小
#define Buf_MaxASR  3
#define Buf_MaxDUO 14
#define Buf_MaxTS   4
uint8 xdata Rec_Buf1[Buf_MaxTS];         //调试舵机接收数据
uint8 xdata Rec_Buf2[Buf_MaxASR];        //语音识别模块接收缓存
uint8 xdata Rec_Buf3[Buf_MaxDUO];        //接收串口3缓存数组
uint8 xdata Rec_Buf4[Buf_MaxOPEN];           //接收串口4缓存数组
uint8 i = 0; 		 	
uint8 j = 0; 
uint8 m = 0; 		 	
uint8 n = 0;
//==================================================================================
//设置IO口--------------------------------------------------------------------------
void wagh()
{
  P0M1 = 0;	P0M0 = 0;	//设置P0.0~P0.7为准双向口
	P1M1 = 0;	P1M0 = 0;	//设置P1.0~P1.7为准双向口
	P2M1 = 0;	P2M0 = 0;	//设置P2.0~P2.7为准双向口 
	P3M1 = 0;	P3M0 = 0;	//设置P3.0~P3.7为准双向口
	P4M1 = 0;	P4M0 = 0;	//设置P4.0~P4.7为准双向口
	P5M1 = 0;	P5M0 = 0;	//设置P5.0~P5.7为准双向口
}
//延时函数---------------------------------------------------------------------------
void delay_ms(unsigned  int x) 
{  
	unsigned int j,i;   
	for(j=0;j<x;j++)   
	{    
		for(i=0;i<1100;i++);   
	}  
}
//串口1初始化函数--------------------------------------------------------------------
void Uart1234_Init(void)
{	
  P_SW1|=0X00;        //选择P16 P17为串口1
	P_SW1&=0XFF;        //选择P16 P17为串口1
	P_SW2|=S2_S;        //选择P46 P47为串口2
	//串口1配置	
	PCON &= 0x3f;		    //（0011 1111）串口1波特率不倍速
	SCON = 0x50;		    //（0101 0000）串口1的8位数据,可变波特率，启动串行接收器（工作方式一）
	AUXR |= 0x01;		    //（0000 0000）串口1选择定时器2为波特率发生器
  //串口2配置	
 	S2CON = 0x50;		    //串口2的8位数据,可变波特率	
  //串口3配置	
	S3CON |= 0x50;		  //串口3启动串行接收器
	S3CON &= 0x70;		  //串口3选择定时器2为波特率发生器，8位数据,可变波特率
	//串口4配置	
	S4CON |= 0x10;		  //（0001 0000）启动串行接收器	
	S4CON &= 0x30;		  //（0011 0000）8位数据,可变波特率，串口4选择定时器2为波特率发生器
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	
	AUXR |= 0x04;		    //定时器2时钟为Fosc,即1T	
	T2L=(65536-(MAIN_Fosc/4/BAUD));
	T2H=(65536-(MAIN_Fosc/4/BAUD))>>8;	      //设定定时初值
	AUXR |= 0x10;       //启动定时器2
	
	T4T3M |= 0x22;		//定时器3时钟为Fosc,即1T/定时器4时钟为Fosc,即1T
	T3L=(65536-(MAIN_Fosc/4/BAUD3));
	T3H=(65536-(MAIN_Fosc/4/BAUD3))>>8;
	T4L=(65536-(MAIN_Fosc/4/BAUD4));
	T4H=(65536-(MAIN_Fosc/4/BAUD4))>>8;
	T4T3M |= 0x88;		//启动定时器3和4
	
}
//===========================================================================================================
//串口1发送字符函数
void SendDataByUart1(uint8 dat)
{
    SBUF = dat;                 //写数据到UART数据寄存器
		while(TI == 0);             //在停止位没有发送时，TI为0即一直等待
		TI = 0;                     //清除TI位（该位必须软件清零）
}

//串口1发送字符串函数

void SendStringByUart1(uint8 *s)
{
	while(*s)
	{
		SendDataByUart1(*s++);       //将字符串中的字符一个一个发送
	}
}

//清除串口1缓存内容函数
//void CLR_Buf1(void)
//{
//	  uint8 k;
//    for(k=0;k<Buf_MaxTS;k++)      //将串口1缓存数组的值都清为零
//	    {
//			   Rec_Buf1[k] = 0;
//		  }
//    i = 0;                    
//}

void Uart1() interrupt UART1_VECTOR using 1
{
	ES = 0;                       // 串口1中断关闭
	if (RI)                       //串行接收到停止位的中间时刻时，该位置1
  {
      RI = 0;                   //清除RI位 （该位必须软件清零）
			Rec_Buf1[i] = SBUF;       //把串口1缓存SBUF寄存器数据依次存放到数组Rec_Buf1中
			i++;                      
	    if(i>=Buf_MaxTS)             //接收数大于定义接收数组最大个数时，覆盖接收数组之前值
				{
					i = 0;                 
				}           
   }
   if (TI)                    //在停止位开始发送时，该位置1
   {
      TI = 0;                 //清除TI位（该位必须软件清零）
   }
	 ES =  1;                   // 串口1中断打开
}
////===========================================================================================================
////串口2发送字符函数
void SendDataByUart2(uint8 dat)
{
    S2BUF = dat;                 //写数据到UART数据寄存器
		while(!(S2CON&S2TI));        //在停止位没有发送时，S2TI为0即一直等待
    S2CON&=~S2TI;                //清除S2CON寄存器对应S2TI位（该位必须软件清零）
}
//// 串口2发送字符串函数
//void SendStringByUart2(uint8 *s)
//{
//  IE2 &= 0xFE; 			          // 串口2中断关闭
//	while (*s)                  //检测字符串结束标志
//  {
//    SendDataByUart2(*s++);         //发送当前字符
//  }
//  IE2 |= 0x01;                     // 串口2中断打开
//}
//清除串口1缓存内容函数
void CLR_Buf2(void)
{
	unsigned char k;
  for(k=0;k<Buf_MaxASR;k++)      //将串口2缓存数组的值都清为零
	{
		 Rec_Buf2[k] = 0;
  }
  j = 0;                    
}
void Uart2() interrupt UART2_VECTOR using 1
{
	IE2 &= 0xFE; 			                          // 串口2中断关闭
	if (S2CON & S2RI)                           //串行接收到停止位的中间时刻时，该位置1
  {
      S2CON &= ~S2RI;                         //清除S2CON寄存器对应S2RI位（该位必须软件清零）
			Rec_Buf2[j] = S2BUF;                    //把串口2缓存SBUF寄存器数据依次存放到数组Rec_Buf2中                                  
		  j++;
		
			if(j>Buf_MaxASR)                          //接收数大于定义接收数组最大个数时，覆盖接收数组之前值
				{
						j = 0;
				}			
   }
      if (S2CON & S2TI)                       //在停止位开始发送时，该位置1
      {
        S2CON &= ~S2TI;   			              //清除S2CON寄存器对应S2TI位（该位必须软件清零）
      }  
	 IE2 |= 0x01;                               // 串口2中断打开	
}
//////====================================================================================================
//串口3发送数据函数
void SendDataByUart3(uint8 dat)
{
    S3BUF = dat;                 //写数据到UART数据寄存器
		while(!(S3CON&S3TI));        //在停止位没有发送时，S3TI为0即一直等待
    S3CON&=~S3TI;                //清除S3CON寄存器对应S3TI位（该位必须软件清零）
}

// 串口3发送字符串函数
void SendStringByUart3(uint8 *s)
{
    IE2 &= 0xF7; 			               // 串口3中断关闭
	  while (*s)                       //检测字符串结束标志
    {
      SendDataByUart3(*s++);         //发送当前字符
    }
    IE2 |= 0x08;                     // 串口3中断打开
}
//清除缓存内容函数
void CLR_Buf3(void)
{
	  uint8 k;
    for(k=0;k<Buf_MaxDUO;k++)      //将串口3缓存数组的值都清为零
	  {
			   Rec_Buf3[k] = 0;
		}
    m = 0;                    
}

void Uart3() interrupt UART3_VECTOR using 1
{
	IE2 &= 0xF7; 			               // 串口3中断关闭
	if (S3CON & S3RI)                //串行接收到停止位的中间时刻时，该位置1
  {
      S3CON &= ~S3RI;              //清除S3CON寄存器对应S3RI位（该位必须软件清零）
			Rec_Buf3[m] = S3BUF;         //把串口3缓存SBUF寄存器数据依次存放到数组Rec_Buf3中
			m++;               
	    if(m>Buf_MaxDUO)                //接收数大于定义接收数组最大个数时，覆盖接收数组之前值
				{
					m = 0;
				}     
   }
  if (S3CON & S3TI)                //在停止位开始发送时，该位置1
   {
      S3CON &= ~S3TI;   			     //清除S3CON寄存器对应S3TI位（该位必须软件清零）
   }
	 IE2 |= 0x08;                     // 串口3中断打开
}
//////========================================================================================================
void SendDataByUart4(uint8 dat)
{
    S4BUF = dat;                 //写数据到UART数据寄存器
		while(!(S4CON&S4TI));        //在停止位没有发送时，S4TI为0即一直等待
    S4CON&=~S4TI;                //清除S4CON寄存器对应S4TI位（该位必须软件清零）
}
//串口4发送字符串函数
void SendStringByUart4(char *s)
{
  IE2 &= 0xEF; 			               //串口4中断关闭
	while (*s)                       //检测字符串结束标志
  {
    SendDataByUart4(*s++);         //发送当前字符
  }
  IE2 |= 0x10;                     //串口4中断打开
}
//清除缓存内容函数
void CLR_Buf4(void)
{
	unsigned char k;
  for(k=0;k<Buf_MaxOPEN;k++)         //将串口4缓存数组的值都清为零
	{
		 Rec_Buf4[k] = 0;
	}
  n = 0;                    
}

//串口4中断服务函数
void Uart4() interrupt UART4_VECTOR
{			
  IE2 &= 0xEF; 			          // 串口4中断关闭  
	if(S4CON & S4RI)           //串行接收到停止位的中间时刻时，该位置1
	{
		S4CON &= ~S4RI;	         //清除S4CON寄存器对应S4RI位（该位必须软件清零）
		Rec_Buf4[n] = S4BUF;     //把串口4缓存SBUF寄存器数据依次存放到数组Rec_Buf4中
		n++;               
	  if(n>=Buf_MaxOPEN)            //接收数大于定义接收数组最大个数时，覆盖接收数组之前值
		{
				n = 0;
		}
	}
	if(S4CON & S4TI)           //在停止位开始发送时，该位置1
	 {
		S4CON &= ~S4TI;          //清除S4CON寄存器对应S4TI位（该位必须软件清零）
	 }
	 cmd=Rec_Buf4[0];
	 IE2 |= 0x10;              // 串口4中断打开
}