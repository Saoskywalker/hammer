//////////////////////////////////////////////////////////////////////////////////
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��:
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   P1^0��SCL��
//              D1   P1^1��SDA��
//              RES  ��P12
//              DC   ��P13
//              CS   ��P1
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H

//OLED�����ú���
void OLED_WR_Byte(unsigned dat, unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(unsigned char j);
void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char t);
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr, 
                    unsigned char Char_Size, unsigned char mode);
void OLED_ShowHZK(unsigned char x, unsigned char y, unsigned char c, 
                    unsigned char Char_Size, unsigned char mode);
void OLED_ShowNum(unsigned char x, unsigned char y, unsigned long int num, 
                unsigned char len, unsigned char size, unsigned char mode);
void OLED_ShowString(unsigned char x, unsigned char y, unsigned char *chr, 
                        unsigned char Char_Size, unsigned char mode);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                  unsigned char y1, unsigned char BMP[]);
void OLED_fill_picture(const unsigned char *fill_Data);

#endif
