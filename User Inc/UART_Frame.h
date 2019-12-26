/****************************************************
Name: UART_Frame Lib
version: 1.0 Released
Editor: Aysi
History: 2017.12.5 create
****************************************************/

#ifndef _UART_FRAME_H
#define _UART_FRAME_H

#define FRAME_HEAD    0xAA
#define FRAME_END     0x55
#define CMD_TYPE	  0xCE
#define CMD_STATUS	  0xC0
#define CMD_LED  	  0xC1
#define CMD_COUNTER	  0xC2
#define CMD_INC 	  0xC3
#define CMD_CLERA	  0xC4

extern u8 rx2Flag;
extern u8 buff2[5];
extern u8 RecBuffer[6];
extern u8 FrameSendBuf[7];

void usart1_send_char(u8 c);
u8 FrameCheck(u8 *_RecBuffer, u8 *_StatusList);
void FrameSend(u8 FrameCommand, u8 FrameData1, u8 FrameData2, u8 FrameData3, u8 FrameData4);
void FrameRec(void);
void commRx2Handler(u8 byte);

#endif
