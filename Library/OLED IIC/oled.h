//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   :
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��:
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   P1^0��SCL��
//              D1   P1^1��SDA��
//              RES  ��P12
//              DC   ��P13
//              CS   ��P14
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   :
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H

#include "N76E003_BSP.h"

#define OLED_CMD 0  //д����
#define OLED_DATA 1 //д����
#define OLED_MODE 0

//-----------------OLED�˿ڶ���----------------
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

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ

#define SIZE 16
#define XLevelL 0x02
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

//OLED�����ú���
void OLED_WR_Byte(unsigned dat, unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size);
void OLED_ShowString(u8 x, u8 y, u8 *p, u8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x, u8 y, u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                  unsigned char y1, unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture();
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();

#endif