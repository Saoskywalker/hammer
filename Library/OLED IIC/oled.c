//////////////////////////////////////////////////////////////////////////////////
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明:
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   P1^0（SCL）
//              D1   P1^1（SDA）
//              RES  接P12
//              DC   接P13
//              CS   接P14
//              ----------------------------------------------------------------
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
//******************************************************************************/。
#include "oled.h"
#include "oledfont.h"
#include "delay.h"

#include "N76E003_BSP.h"

#define OLED_CMD 0  //写命令
#define OLED_DATA 1 //写数据
#define OLED_MODE 0

//-----------------OLED端口定义----------------
#define OLED_SCL P13
#define OLED_SDIN P14

#define OLED_CS_Clr() OLED_CS = 0
#define OLED_CS_Set() OLED_CS = 1

#define OLED_RST_Clr() OLED_RST = 0
#define OLED_RST_Set() OLED_RST = 1

#define OLED_DC_Clr() OLED_DC = 0
#define OLED_DC_Set() OLED_DC = 1

#define OLED_SCLK_Clr() OLED_SCL = 0
#define OLED_SCLK_Set() OLED_SCL = 1

#define OLED_SDIN_Clr() OLED_SDIN = 0
#define OLED_SDIN_Set() OLED_SDIN = 1

//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE 16
#define XLevelL 0x02
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

/**********************************************
//IIC Start
**********************************************/
static void IIC_Start()
{
	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
static void IIC_Stop()
{
	OLED_SCLK_Set();
	//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

static void IIC_Wait_Ack()
{

	//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
	//GPIOB->CRH |= 0x00080000;
	//	OLED_SDA = 1;
	//	delay_us(1);
	//OLED_SCL = 1;
	//delay_us(50000);
	/*	while(1)
	{
		if(!OLED_SDA)				//判断是否接收到OLED 应答信号
		{
			//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
			//GPIOB->CRH |= 0x00030000;
			return;
		}
	}
*/
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}

/**********************************************
// IIC Write byte
**********************************************/
static void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m, da;
	da = IIC_Byte;
	OLED_SCLK_Clr();
	for (i = 0; i < 8; i++)
	{
		m = da;
		//	OLED_SCLK_Clr();
		m = m & 0x80;
		if (m == 0x80)
		{
			OLED_SDIN_Set();
		}
		else
			OLED_SDIN_Clr();
		da = da << 1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}

/**********************************************
// IIC Write Command
**********************************************/
static void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78); //Slave address,SA0=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00); //write command
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}

/**********************************************
// IIC Write Data
**********************************************/
static void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78); //D/C#=0; R/W#=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40); //write data
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
	if (cmd)
	{
		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}

/********************************************
// fill_Picture
********************************************/
void OLED_fill_picture(const unsigned char *fill_Data)
{
	unsigned char m, n;
	unsigned int i = 0;
	for (m = 0; m < 8; m++)
	{
		OLED_WR_Byte(0xb0 + m, 0); //page0-page1
		OLED_WR_Byte(0x00, 0);	 //low column start address
		OLED_WR_Byte(0x10, 0);	 //high column start address
		for (n = 0; n < 128; n++)
		{
			OLED_WR_Byte(fill_Data[i], 1);
			i++;
		}
	}
}

//坐标设置 x: x坐标(自增), y: 注意:Y为页号, 12864 以8行为一页, 垂直方向共8页
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

//开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}

//关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}

//清屏函数,清完屏
//j: 0正常清屏, 0XFF: 反色清屏
void OLED_Clear(unsigned char j)
{
	unsigned char i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	 //设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	 //设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(j, OLED_DATA);
	} //更新显示
}

void OLED_On(void)
{
	unsigned char i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	 //设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	 //设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(1, OLED_DATA);
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~7
//mode: 0bit: 0,反色显示;1,正常显示
//size:选择字体
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr, 
					unsigned char Char_Size, unsigned char mode)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; //得到偏移后的值
	if (x > Max_Column - Char_Size / 2 + 1) //x超界, 不显示
		return;
	if (y > Max_Row / 8 - 1) //y超界, 不显
		return;
	if (Char_Size == 16)
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 8; i++)
		{
			if (mode & 0X01)
				OLED_WR_Byte(ASCII16[c][i], OLED_DATA);
			else
				OLED_WR_Byte(~ASCII16[c][i], OLED_DATA);
		}
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 8; i++)
		{
			if (mode & 0X01)
				OLED_WR_Byte(ASCII16[c][i + 8], OLED_DATA);
			else
				OLED_WR_Byte(~ASCII16[c][i + 8], OLED_DATA);
		}
	}
	else if (Char_Size == 24)
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 12; i++)
		{
			if(mode&0X01)
				OLED_WR_Byte(ASCII24[c][i], OLED_DATA);
			else
				OLED_WR_Byte(~ASCII24[c][i], OLED_DATA);
		}
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 12; i++)
		{
			if(mode&0X01)
				OLED_WR_Byte(ASCII24[c][i + 12], OLED_DATA);
			else
				OLED_WR_Byte(~ASCII24[c][i + 12], OLED_DATA);
		}
		OLED_Set_Pos(x, y + 2);
		for (i = 0; i < 12; i++)
		{
			if(mode&0X01)
				OLED_WR_Byte(ASCII24[c][i + 24], OLED_DATA);
			else
				OLED_WR_Byte(~ASCII24[c][i + 24], OLED_DATA);
		}
	}
}

//在指定位置显示一个汉字字符
//x:0~127
//y:0~7
//mode: 0bit: 0,反色显示;1,正常显示
//size:选择字体 
void OLED_ShowHZK(unsigned char x, unsigned char y, unsigned char c, 
					unsigned char Char_Size, unsigned char mode)
{
	unsigned char i = 0;
	if (x > Max_Column - Char_Size + 1) //x超界, 不显示
		return;
	if (y > Max_Row / 8 - 1) //y超界, 不显
		return;
	if (Char_Size == 16)
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 16; i++)
		{
			if (mode & 0X01)
				OLED_WR_Byte(HZK16[c][i], OLED_DATA);
			else
				OLED_WR_Byte(~HZK16[c][i], OLED_DATA);
		}
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 16; i++)
		{
			if (mode & 0X01)
				OLED_WR_Byte(HZK16[c][i + 16], OLED_DATA);
			else
				OLED_WR_Byte(~HZK16[c][i + 16], OLED_DATA);
		}
	}
	else if (Char_Size == 24)
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 24; i++)
		{
			if(mode&0X01)
				OLED_WR_Byte(HZK24[c][i], OLED_DATA);
			else
				OLED_WR_Byte(~HZK24[c][i], OLED_DATA);
		}
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 24; i++)
		{
			if(mode&0X01)
				OLED_WR_Byte(HZK24[c][i + 24], OLED_DATA);
			else
				OLED_WR_Byte(~HZK24[c][i + 24], OLED_DATA);
		}
		OLED_Set_Pos(x, y + 2);
		for (i = 0; i < 24; i++)
		{
			if(mode&0X01)
				OLED_WR_Byte(HZK24[c][i + 48], OLED_DATA);
			else
				OLED_WR_Byte(~HZK24[c][i + 48], OLED_DATA);
		}
	}
}

//m^n函数
static unsigned long int oled_pow(unsigned char m, unsigned char n)
{
	unsigned long int result = 1;
	while (n--)
		result *= m;
	return result;
}

//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(unsigned char x, unsigned char y, unsigned long int num, 
					unsigned char len, unsigned char size2, unsigned char mode)
{
	unsigned char t, temp;
	unsigned char enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2, mode);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2, mode);
	}
}

//显示一个字符号串
void OLED_ShowString(unsigned char x, unsigned char y, unsigned char *chr, 
						unsigned char Char_Size, unsigned char mode)
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{
		if(chr[j]<='~'&&chr[j]>=' ')
		{
			OLED_ShowChar(x, y, chr[j], Char_Size, mode);
			x += Char_Size/2;
		}
		j++;
	}
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

//初始化SSD1306
void OLED_Init(void)
{
	P13_OpenDrain_Mode; OLED_SCL = 1; //SCL
  	P14_OpenDrain_Mode; OLED_SDIN = 1; //SDA
	delay_ms(10);
	OLED_WR_Byte(0xAE, OLED_CMD); //--display off
	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
	OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
	OLED_WR_Byte(0x81, OLED_CMD); // contract control
	OLED_WR_Byte(0xFF, OLED_CMD); //--128
	OLED_WR_Byte(0xA1, OLED_CMD); //set segment remap
	OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
	OLED_WR_Byte(0xC8, OLED_CMD); //Com scan direction
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
	OLED_WR_Byte(0x00, OLED_CMD); //

	OLED_WR_Byte(0xD5, OLED_CMD); //set osc division
	OLED_WR_Byte(0x80, OLED_CMD); //

	OLED_WR_Byte(0xD8, OLED_CMD); //set area color mode off
	OLED_WR_Byte(0x05, OLED_CMD); //

	OLED_WR_Byte(0xD9, OLED_CMD); //Set Pre-Charge Period
	OLED_WR_Byte(0xF1, OLED_CMD); //

	OLED_WR_Byte(0xDA, OLED_CMD); //set com pin configuartion
	OLED_WR_Byte(0x12, OLED_CMD); //

	OLED_WR_Byte(0xDB, OLED_CMD); //set Vcomh
	OLED_WR_Byte(0x30, OLED_CMD); //

	OLED_WR_Byte(0x8D, OLED_CMD); //set charge pump enable
	OLED_WR_Byte(0x14, OLED_CMD); //

	OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}
