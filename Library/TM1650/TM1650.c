#include "TM1650.h"

static void I2C_Start_TM1650(void)
{          
	SET_SDA_OUT_TM1650();
	SCL_TM1650 = 1;
	SDA_TM1650 = 1;
	TM1650_IIC_Delay_us(5);
	SDA_TM1650 = 0;
}
 
//--------------------------------------------------------------
// Prototype      : void I2C_Stop(void)
// Calls          : Delay_5us()
// Description    : Stop Singnal
//-------------------------------------------------------------- 
static void I2C_Stop_TM1650(void)
{   
	SET_SDA_OUT_TM1650();    
	SCL_TM1650 = 1;  
	SDA_TM1650 = 0;
	TM1650_IIC_Delay_us(10);
	SDA_TM1650 = 1;
}
 
//Ӧ����
static void IIC_Ack_TM1650(void)
{
    //������һֱ����Ϊ�͵�ƽ��ʱ���߳��������ؼ�ΪӦ��
	SET_SDA_OUT_TM1650();
    SDA_TM1650 = 0;
	SCL_TM1650 = 1;
	TM1650_IIC_Delay_us(5);
    //Ӧ����ɺ� ��ʱ�������� ���������޸�
    SCL_TM1650 = 0;
	TM1650_IIC_Delay_us(5);
}

//��Ӧ��
// static void IIC_NAck_TM1650(void)
// {
//     //��Ӧ���෴ ��Ӧ������Ϊ�����߱��ָߵ�ƽ����
// 	SET_SDA_OUT_TM1650();
// 	TM1650_IIC_Delay_us(10);
//     SDA_TM1650 = 1;
//     TM1650_IIC_Delay_us(10);
// 	SCL_TM1650 = 0;
// 	TM1650_IIC_Delay_us(40);
//     SCL_TM1650 = 1;
//     TM1650_IIC_Delay_us(40);
//     //���Ҫ��ʱ�������� �������ݱ仯
//     SCL_TM1650 = 0;
// }

//�ȴ�Ӧ��
static uint8_t IIC_Wait_Ack_TM1650(void)//0Ϊ��Ӧ��1Ϊ��Ӧ��
{
	u8 timeout = 1;

	SET_SDA_IN_TM1650();
    SCL_TM1650 = 1;
    TM1650_IIC_Delay_us(5);
    SCL_TM1650 = 0;
	while((SDAR_TM1650)&&(timeout<=100))
	{
		timeout++;
	}
    TM1650_IIC_Delay_us(5);
    SCL_TM1650 = 0;
	return 0;
}
 
static void IIC_WrByte_TM1650(uint8_t txd)
{
	u8 i;

	SET_SDA_OUT_TM1650();
	SCL_TM1650 = 0;
	TM1650_IIC_Delay_us(1);
	for(i=0;i<8;i++)
	{
		if(txd&0x80)
			SDA_TM1650 = 1;
		else
			SDA_TM1650 = 0;
		
		txd = txd<<1;
		SCL_TM1650 = 0;
		TM1650_IIC_Delay_us(5);
		SCL_TM1650 = 1;
		TM1650_IIC_Delay_us(5);
		SCL_TM1650 = 0;
	}
	TM1650_IIC_Delay_us(5);
}

//--------------------------------------------------------------
// Prototype      : void IIC_Init_TM1650(void)
// Calls          : 
// Description    : 
//--------------------------------------------------------------
void IIC_Init_TM1650(void)
{
   SET_SCL_OUT_TM1650();
   SET_SDA_OUT_TM1650(); 
   SCL_TM1650  = 1;
   SDA_TM1650  = 1;
}

u8 TM1650_Key(void)	  //����ɨ��
{
	u8 i = 0;
	u8 rekey = 0;
	I2C_Start_TM1650();
	IIC_WrByte_TM1650(0x49);//����������
	IIC_Wait_Ack_TM1650();
	SET_SDA_IN_TM1650();//�л�Ϊ����ģʽ
	for(i=0;i<8;i++)
	{
	   SCL_TM1650=1;
	   rekey = rekey<<1;
	   
	   if(SDAR_TM1650)
	   {
	   	rekey++;
	   } 
	   TM1650_IIC_Delay_us(5);
	   SCL_TM1650=0;
	   TM1650_IIC_Delay_us(5);	
	}
	IIC_Ack_TM1650();
	I2C_Stop_TM1650();
	return(rekey);
}
 
void TM1650_Display(u8 add,u8 dat) //�������ʾ
{
	//д�Դ����Ӹߵ�ַ��ʼд
	I2C_Start_TM1650();
	IIC_WrByte_TM1650(add); //��һ���Դ��ַ
	IIC_Wait_Ack_TM1650();
	IIC_WrByte_TM1650(dat);
	IIC_Wait_Ack_TM1650();
	I2C_Stop_TM1650();
}

//light: light level: value:0=8 value:1~7=1~7; dis: dis 1 or close 0
u8 TM1650_Init(u8 light, u8 dis)  //init tm1650
{
	IIC_Init_TM1650();
	delay_ms(50);			//��Ҫ��ʱһС��ʱ�䣬������ʾ������Ӧ
	TM1650_Display(0x48, light<<4|dis);		
	if(TM1650_Key()==0XFF)	//check device
		return 1;	//error
	else
		return 0;	//success
}
 
 
 
 
 
 