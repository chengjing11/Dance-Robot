# include "narfl24l01.h"
#define  uint16   unsigned int  
#define  uint8    unsigned char 
/*******************************************************************************************************/
//�����շ���ַ���(�ֽ���)
#define TX_ADDR_WIDTH 5
#define RX_ADDR_WIDTH 5

//�����շ����ݳ���(�ֽ���)
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
//NRF24L01�Ĵ�����������
#define SPI_READ_REG    0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define SPI_WRITE_REG   0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
//#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
                              //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                              //bit5:���ݷ�������ж�;bit6:���������ж�;
#define MAX_TX  	    0x10    //�ﵽ����ʹ����ж�
#define TX_OK       	0x20    //TX��������ж�
#define RX_OK   	    0x40    //���յ������ж�

#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define FIFO_STATUS     0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                              //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
															
#define CE_MA_LOW        en=0                        
#define CE_MA_HIGH       en=1

#define CS_MA_LOW        SPI_CS=0
#define CS_MA_HIGH       SPI_CS=1
															
const uint8 TX_ADDRESS[TX_ADDR_WIDTH]={0xE1,0xE2,0xE3,0xE4,0xE5}; //���͵�ַ
const uint8 RX_ADDRESS[RX_ADDR_WIDTH]={0xE1,0xE2,0xE3,0xE4,0xE5}; //���յ�ַ	
uint8  RxPayload[32];   //���߽��ջ���
uint8  TxPayload[32];   //���߷��ͻ���

/**********************
���ű�������
***********************/	
sbit IRQ=P3^3;        //�ж� 
sbit led=P0^6;        //����������ͨ��ָʾ�� 
sbit en=P3^4;         //ʹ�ܿ���
sbit SPI_CS=P1^2;     //Ƭѡ 
//**********************************************************************************
//��ʼ��SPI//
void InitSPI(void)
{
  SPDAT = 0;                                        //��ʼ��SPI����
  SPSTAT = SPIF | WCOL;                             //���SPI״̬λ
  SPCTL = SPEN | MSTR | SSIG;                       //����ģʽ
}
// ģ��SPI��д���ݺ�������дһ���ֽ�
//��  �� : д�������                                            ����ֵ : ��ȡ������
uint8 SPI_RW(uint8 byte)
{
  SPDAT = byte;                                                 //����SPI��������
  while (!(SPSTAT & SPIF));                                     //�ȴ��������
  SPSTAT = SPIF | WCOL;                                         //���SPI״̬λ
  return SPDAT;                                                 //����SPI����
}
//NRF24L01�Ĵ���д����
//��  �� : regaddr��Ҫд�ļĴ�����ַ��data��д�뵽�Ĵ���������    ����ֵ : ��ȡ��״ֵ̬
uint8 NRF24L01_MA_Write_Reg(uint8 regaddr,uint8 dat)
{
	uint8 status;	
  CS_MA_LOW;  
	status =SPI_RW(regaddr); 
  SPI_RW(dat);                           //д��Ĵ�����ֵ
  CS_MA_HIGH;                            //��ֹSPI����	
  return(status);       		             //����״ֵ̬
}

// NRF24L01�Ĵ���������
// * ��  �� : regaddr:Ҫ��ȡ�ļĴ�����ַ              ����ֵ : ��ȡ�ļĴ�����ֵ
uint8 NRF24L01_MA_Read_Reg(uint8 regaddr)
{
	uint8 reg_val;	  
 	CS_MA_LOW;                          //ʹ��SPI����	
	SPI_RW(regaddr); 
	reg_val=SPI_RW(0XFF);
  CS_MA_HIGH;                         //��ֹSPI����		
  return(reg_val);                    //���ض�ȡ��ֵ
}	
// ��ָ����ַд��ָ�����ȵ�����
// ��  �� : pBuf:�������ݵĴ�ŵ�ַ��datlen�������������ֽ���     ����ֵ :����״̬�Ĵ���ֵ
uint8 NRF24L01_MA_Write_Buf(uint8 regaddr, uint8 *pBuf, uint8 datalen)
{
	uint8 status,u8_ctr;	    
 	CS_MA_LOW;                                              //ʹ��SPI����
	status = SPI_RW(regaddr);  
  for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)
  {
	  SPI_RW(*pBuf++); //д������		
  }
  CS_MA_HIGH;                                             //�ر�SPI����
  return status;                                          //���ض�����״ֵ̬
}	
//����NRF24L01����һ������
//��  �� : buf:�����������׵�ַ                            ����ֵ : �������״��
uint8 NRF24L01_TxPacket_MA(uint8 *buf)
{
	uint8 state;   
	
	CE_MA_LOW;
  NRF24L01_MA_Write_Buf(WR_TX_PLOAD,buf,TX_PLOAD_WIDTH);  //д���ݵ�TX BUF 
 	CE_MA_HIGH;                                             //��������	   
	while(IRQ!=0);                         //�ȴ��������
	state=NRF24L01_MA_Read_Reg(STATUS);                     //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_MA_Write_Reg(SPI_WRITE_REG+STATUS,state);      //���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_TX)                                     //�ﵽ����ط�����
	{
		NRF24L01_MA_Write_Reg(FLUSH_TX,0xff);                 //���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(state&TX_OK)                                      //�������
	{
		return TX_OK;
	}
	return 0xff;                                         //����ԭ����ʧ��
}
//��ʼ��NRF24L01��TXģʽ��������ز���
void Set_TxMode_MA(void)
{														 
	CE_MA_LOW;	                                                                           //дTX�ڵ��ַ 
  NRF24L01_MA_Write_Buf(SPI_WRITE_REG+TX_ADDR,(uint8*)TX_ADDRESS,TX_ADDR_WIDTH);         //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  NRF24L01_MA_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(uint8*)RX_ADDRESS,RX_ADDR_WIDTH);      //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);                                       //ʹ��ͨ��0�Ľ��յ�ַ  
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);                                   //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x21);                                  //����RFͨ��Ϊ55
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+RF_CH,55);                                         //����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x26);                                    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX����ģʽ,���������ж�
  NRF24L01_MA_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);                                  
}
//NRF24L01��ʼ����������ʼ������NRF24L01ģ��Ĺܽ�
//����SPI��ʼ��������ɺ�NRF24L01ģ��ͨѶ��SPI���ߵĳ�ʼ��
void Init_NRF24L01_MA(void)
{	
	delay_ms(2);
	led=0;                                          //��ʼ������ͨ��ָʾ��                                         //��ʼ������ͨ��ָʾ��
	CE_MA_LOW; 	                                      //ʹ��NRF24L01
	CS_MA_HIGH;                                       //SPIƬѡȡ��
	InitSPI();                                        //��ʼ��SPI
}

//========================================================================
void TF_senddat(unsigned char i)
{
	TxPayload[0] =i;
	if(NRF24L01_TxPacket_MA(TxPayload) == TX_OK)	  //������ͳɹ�
		{
		  led=~led;  			//���߷���ɹ� 
      delay_ms(200);			
		 }
   
}