# include "LCD2406.h"

#define  uint unsigned int  
#define  uchar unsigned char
	
//引脚别名定义
sbit rs=P4^4;        //1602的数据/指令选择控制线 
sbit rw=P3^7;        //1602的读写控制线 
sbit en=P3^6;        //1602的使能控制线 
sbit lcdsel=P0^2;
//数据保存数组
uchar xdata table[32];      //要显示的内容1放入数组table
 
//LCD1602写命令函数-------------------------------------------------
//入参：uchar com       返回值：无
void lcd_wcom(uchar com)                 
{ 
    rs=0;               //选择指令寄存器 
    rw=0;               //选择写 
    P2=com;             //把命令字送入P2 
    delay_ms(1);        //延时一小会儿，让1602准备接收数据 
    en=1;               //使能线电平变化，命令送入1602的8位数据口 
    en=0; 
} 
//LCD1602写数据函数-------------------------------------------------
//入参：uchar com       返回值：无
void lcd_wdat(uchar dat)          
{ 
    rs=1;               //选择数据寄存器 
    rw=0;               //选择写 
    P2=dat;             //把要显示的数据送入P2 
    delay_ms(1);        //延时一小会儿，让1602准备接收数据 
    en=1;               //使能线电平变化，数据送入1602的8位数据口 
    en=0; 
} 


//功能描述：LCD1602初始化函数
//入参:无       返回值：无
void lcd_init()                 
{ 
    lcd_wcom(0x38);       //8位数据，双列，5*7字形       
    lcd_wcom(0x0c);       //开启显示屏，关光标，光标不闪烁 
    lcd_wcom(0x06);       //显示地址递增，即写一个数据后，显示位置右移一位 
    lcd_wcom(0x01);       //清屏 
} 

//次主函数
void LCD2406(unsigned char *dat)        
{ 
	uchar j=0; 
	P2M1 = 0;	P2M0 = 0xff;	//设置P2.0~P2.7为推挽输出
	lcdsel = 1;
	lcd_init();           //液晶初始化 
	while(*dat)
	{
		table[j++] =*dat++;     //将字符串中的字符一个一个发送
	}
	

	 lcd_wcom(0x80);       //显示地址设为80H（即00H，）上排第一位       
    for(j=0;j<16;j++)     //将table[]中的数据依次写入1602显示 
    { 
            lcd_wdat(table[j]);           
            delay_ms(1); 
    } 	
    lcd_wcom(0x80+0x40); //重新设定显示地址为0xc4,即下排第5位 
    for(j=16;j<34;j++)   //将table1[]中的数据依次写入1602显示 
    {     
           lcd_wdat(table[j]); 
           delay_ms(1); 
    } 	
} 