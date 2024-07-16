# include "NARFL2401.h"
#define  uint16   unsigned int  
#define  uint8    unsigned char
 unsigned char cmd;	
//无线收发地址宽度(字节数)
#define TX_ADDR_WIDTH 5
#define RX_ADDR_WIDTH 5

//无线收发数据长度(字节数)
#define TX_PLOAD_WIDTH 1
#define RX_PLOAD_WIDTH 1

#define SPIF 0x80 //SPSTAT.7
#define WCOL 0x40 //SPSTAT.6
#define SSIG 0x80 //SPCTL.7
#define SPEN 0x40 //SPCTL.6
#define DORD 0x20 //SPCTL.5
#define MSTR 0x10 //SPCTL.4
#define CPOL 0x08 //SPCTL.3
#define CPHA 0x04 //SPCTL.2
#define SPDHH 0x00 //CPU_CLK/4
#define SPDH 0x01 //CPU_CLK/8
#define SPDL 0x02 //CPU_CLK/16
#define SPDLL 0x03 //CPU_CLK/32

/****************************************************************************************************/
//NRF24L01寄存器操作命令
#define SPI_READ_REG    0x00  //读配置寄存器,低5位为寄存器地址
#define SPI_WRITE_REG   0x20  //写配置寄存器,低5位为寄存器地址
#define RD_RX_PLOAD     0x61  //读RX有效数据,1~32字节
#define WR_TX_PLOAD     0xA0  //写TX有效数据,1~32字节
#define FLUSH_TX        0xE1  //清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX        0xE2  //清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL     0xE3  //重新使用上一包数据,CE为高,数据包被不断发送.
//#define NOP             0xFF  //空操作,可以用来读状态寄存器	 
//SPI(NRF24L01)寄存器地址
#define CONFIG          0x00  //配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
                              //bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define EN_AA           0x01  //使能自动应答功能  bit0~5,对应通道0~5
#define EN_RXADDR       0x02  //接收地址允许,bit0~5,对应通道0~5
#define SETUP_AW        0x03  //设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define SETUP_RETR      0x04  //建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define RF_CH           0x05  //RF通道,bit6:0,工作通道频率;
#define RF_SETUP        0x06  //RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define STATUS          0x07  //状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
                              //bit5:数据发送完成中断;bit6:接收数据中断;
#define MAX_TX  	    0x10    //达到最大发送次数中断
#define TX_OK       	0x20    //TX发送完成中断
#define RX_OK   	    0x40    //接收到数据中断

#define OBSERVE_TX      0x08  //发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define CD              0x09  //载波检测寄存器,bit0,载波检测;
#define RX_ADDR_P0      0x0A  //数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1      0x0B  //数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2      0x0C  //数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3      0x0D  //数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4      0x0E  //数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5      0x0F  //数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define TX_ADDR         0x10  //发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define RX_PW_P0        0x11  //接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1        0x12  //接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2        0x13  //接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3        0x14  //接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4        0x15  //接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5        0x16  //接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define FIFO_STATUS     0x17  //FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
                              //bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环;
															
#define CE_MA_LOW        en=0                        
#define CE_MA_HIGH       en=1

#define CS_MA_LOW        SPI_CS=0
#define CS_MA_HIGH       SPI_CS=1
															
const uint8 TX_ADDRESS[TX_ADDR_WIDTH]={0xE1,0xE2,0xE3,0xE4,0xE5}; //发送地址
const uint8 RX_ADDRESS[RX_ADDR_WIDTH]={0xE1,0xE2,0xE3,0xE4,0xE5}; //接收地址	
uint8  RxPayload[1];   //无线接收缓存
uint8  TxPayload[1];   //无线发送缓存

/**********************
引脚别名定义
***********************/	
sbit led_r=P0^5;      //开发板用于无线接收指示灯 
sbit IRQ=P3^3;        //中断 
sbit en= P3^4;         //使能控制
sbit SPI_CS=P1^2;     //片选 
//初始化SPI//
void InitSPI(void)
{
  SPDAT = 0;                                        //初始化SPI数据
  SPSTAT = SPIF | WCOL;                             //清除SPI状态位
  SPCTL = SPEN | MSTR | SSIG;                       //主机模式
}
//NRF24L01初始化函数，初始化连接NRF24L01模块的管脚
//调用SPI初始化函数完成和NRF24L01模块通讯的SPI总线的初始化
void Init_NRF24L01_MA(void)
{	
	delay_ms(2);
	led_r=0;                                          //初始化点亮通信指示灯
	CE_MA_LOW; 	                                      //使能NRF24L01
	CS_MA_HIGH;                                       //SPI片选取消
	InitSPI();                                        //初始化SPI
}
// 模拟SPI读写数据函数，读写一个字节
//入  参 : 写入的数据                                            返回值 : 读取的数据
uint8 SPI_RW(uint8 byte)
{
  SPDAT = byte;                                                 //触发SPI发送数据
  while (!(SPSTAT & SPIF));                                     //等待发送完成
  SPSTAT = SPIF | WCOL;                                         //清除SPI状态位
  return SPDAT;                                                 //返回SPI数据
}
//***************************************************************************
// 向指定地址写入指定长度的数据
// 入  参 : pBuf:读出数据的存放地址；datlen：读出的数据字节数     返回值 :返回状态寄存器值
uint8 NRF24L01_MA_Write_Buf(uint8 regaddr, uint8 *pBuf, uint8 datalen)
{
	uint8 status,u8_ctr;	    
 	CS_MA_LOW;                                              //使能SPI传输
	status = SPI_RW(regaddr);  
  for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)
  {
	  SPI_RW(*pBuf++); //写入数据		
  }
  CS_MA_HIGH;                                             //关闭SPI传输
  return status;                                          //返回读到的状态值
}	
///***************************************************************************
//NRF24L01寄存器写函数
//入  参 : regaddr：要写的寄存器地址；data：写入到寄存器的数据    返回值 : 读取的状态值
uint8 NRF24L01_MA_Write_Reg(uint8 regaddr,uint8 dat)
{
	uint8 status;	
  CS_MA_LOW;  
	status =SPI_RW(regaddr); 
  SPI_RW(dat);                           //写入寄存器的值
  CS_MA_HIGH;                            //禁止SPI传输	
  return(status);       		             //返回状态值
}

// NRF24L01寄存器读函数
// * 入  参 : regaddr:要读取的寄存器地址              返回值 : 读取的寄存器的值
uint8 NRF24L01_MA_Read_Reg(uint8 regaddr)
{
	uint8 reg_val;	  
 	CS_MA_LOW;                          //使能SPI传输	
	SPI_RW(regaddr); 
	reg_val=SPI_RW(0XFF);
  CS_MA_HIGH;                         //禁止SPI传输		
  return(reg_val);                    //返回读取的值
}	
//从指定地址读出指定长度的数据
//入  参 : pBuf:读出数据的存放地址；datlen：读出的数据字节数          返回值 : 读取的寄存器的值
uint8 NRF24L01_MA_Read_Buf(uint8 regaddr,uint8 *pBuf,uint8 datlen)
{
	uint8 status,u8_ctr;	       
  CS_MA_LOW;                               //使能SPI传输
	status=SPI_RW(regaddr);
	for(u8_ctr=0;u8_ctr<datlen;u8_ctr++)
	{
		pBuf[u8_ctr]=SPI_RW(0XFF);//读出数据
	}
  CS_MA_HIGH;                              //关闭SPI传输
  CS_MA_HIGH;                              //关闭SPI传输

  return status;                        //返回读到的状态值
}


//初始化NRF24L01到RX模式，配置相关参数，CE变高后,即进入RX模式
void Set_RxMode_MA(void)
{
	CE_MA_LOW;	                     	// Delay_1ms(20);
  NRF24L01_MA_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(uint8*)RX_ADDRESS,RX_ADDR_WIDTH);	//写RX节点地址
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);  //使能通道0的自动应答         	 
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);  //使能通道0的接收地址	  
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+RF_CH,55);	  //设置RF通信频率	     	    
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);  //选择通道0的有效数据宽度 
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x26);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+CONFIG, 0x0f);                //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX接收模式 
  CE_MA_HIGH;                                                   //CE为高,进入接收模式               
}	
// 启动NRF24L01读取一次数据
//入  参 : buf:待发送数据首地址     返回值 : RX_OK：接收到数据；0：未接收到数据
uint8 NRF24L01_RxPacket_MA(uint8 *buf)
{
	uint8 state;		    
	state=NRF24L01_MA_Read_Reg(STATUS);                //读取状态寄存器的值    	 
	NRF24L01_MA_Write_Reg(SPI_WRITE_REG+STATUS,state); //清除TX_DS或MAX_RT中断标志
	if(state&RX_OK)                                    //接收到数据
	{
		NRF24L01_MA_Read_Buf(RD_RX_PLOAD,buf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_MA_Write_Reg(FLUSH_RX,0xff);            //清除RX FIFO寄存器 
		return RX_OK; 
	}	   
	 return 0;                                           //没收到任何数据
}
void zhongduanyi()
{
	ES=1;
	IE2 |= 0x01;
	IE2 |= 0x08;
	IE2 |= 0x10;
	
	IE1  = 0;	     //外部中断1标志位
	EX1 = 1;	     //INT1 Enable
	IT1 = 1;		   //INT1 下降沿中断		
	EA = 1;		     //允许总中断
}
void RF_ReceDat()
  {  
		 if(NRF24L01_RxPacket_MA(RxPayload) == RX_OK)	//如果接收到数据
		   {	
				 cmd=RxPayload[0];
				 RxPayload[0]=0;
         delay_ms(200);				 
		   }		
 }
//========================================================================
void INT1_int (void) interrupt INT1_VECTOR
{
	RF_ReceDat();
}