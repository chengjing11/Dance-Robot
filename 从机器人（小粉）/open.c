# include "open.H"
extern unsigned char xdata Rec_Buf4[1];
void openmv()
{
	unsigned char i;
	i=Rec_Buf4[0];
	if(i==0x0B||0X0C||0X0D)        //剪刀手
	{
	SendStringByUart1("摇手");//
	}else if(i==0x0E||0X0F||0X11)  //握手
	{
	SendStringByUart1("握手");//握手
	}else if(i==0x12||0X13||0X14)   //真棒
	{
  SendStringByUart1("劈叉");	//劈叉
	}
}