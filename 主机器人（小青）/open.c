# include "open.H"
extern unsigned char xdata Rec_Buf4[1];
void openmv()
{
	unsigned char i;
	i=Rec_Buf4[0];
	if(i==0x0B||0X0C||0X0D)        //������
	{
	SendStringByUart1("ҡ��");//
	}else if(i==0x0E||0X0F||0X11)  //����
	{
	SendStringByUart1("����");//����
	}else if(i==0x12||0X13||0X14)   //���
	{
  SendStringByUart1("����");	//����
	}
}