/****************************************************
Name: UART_Frame Lib
version: 1.0 Released
Editor: Aysi
History: 2017.12.5 create
****************************************************/

#include "UserBaseLib.h"
#include "UART_Frame.h"

u8 RecBuffer[6];
u8 FrameSendBuf[7];
u8 rx2Flag = 0;
u8 buff2[5];

//Rec one frame
void FrameRec(void)
{
     static u8 iii = 0;
     
     if(iii<6)
     {}
     else
     {
       iii = 0;
     }
     RecBuffer[iii] = UART1_ReceiveData8();
     ++iii;
}

/*******************************
Frame format check, change to need information
return: 1 succeed 
_RecBuffer: need to precress data
_StatusList: get status list
format: Head[0xaa+0x55]+Data[0xXX]*2+Tail[0x55]
*******************************/
u8 FrameCheck(u8 *_RecBuffer, u8 *_StatusList)
{
  u8 i = 0;
  u8 o = 0;
  for(i = 0;i<6;i++)
  {
    if(_RecBuffer[i]==0xAA)
    {
      if(++i>5)
      {
        i = 0;
      }
      if(_RecBuffer[i]==0x55)
      {
        if(++i>5)
        {
          i = 0;
        }
        switch(_RecBuffer[i])
        {
        case 0xC0: {break;}
        case 0xC1: {break;}
        case 0xC2: {break;}        
        case 0xCE: {break;}
        default: {return 0; break;}
        }
        _StatusList[0] = _RecBuffer[i];
        if(++i>5)
        {
          i = 0;
        }
        _StatusList[1] = _RecBuffer[i];
        if(++i>5)
        {
          i = 0;
        }
        _StatusList[2] = _RecBuffer[i];
        if(++i>5)
        {
          i = 0;
        }
        if(_RecBuffer[i]==0x55)
        {
          for(o = 0;o<6;o++)
          {
            _RecBuffer[o] = 0;
          }
          return 1;
        }
        else
        {
          return 0;
        }
      }
      return 0;
    }
  }
  return 0;
}

//Send one frame
void FrameSend(u8 FrameCommand, u8 FrameData1, u8 FrameData2, u8 FrameData3, u8 FrameData4)
{
  u8 i = 0;
  
  // REC_SEND_485 = SEND_485;
   FrameSendBuf[0] = 0xAA;
   FrameSendBuf[1] = FrameCommand;
   FrameSendBuf[2] = FrameData1;
   FrameSendBuf[3] = FrameData2;
   FrameSendBuf[4] = FrameData3;
   FrameSendBuf[5] = FrameData4;
   FrameSendBuf[6] = 0x55;
   for(i = 0;i<7;i++)
   {
      usart1_send_char(FrameSendBuf[i]);
   }
   while(!UART1_GetFlagStatus(UART1_FLAG_TC));
  //  REC_SEND_485 = REC_485;
}

//UART1 Send one byte 
//c:wait to send byte
void usart1_send_char(u8 c)
{   	
  while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
  UART1->DR = c;  //Load send buffer
} 

//Rec and process frame
void commRx2Handler(u8 byte)
{
  static u8 count = 0;
  
  if(count == 0){
    if(byte == FRAME_HEAD){
      count++;
    }
  }else if(count < 3){
    buff2[count-1] = byte;
    count++;
  }else{
    count = 0;
    if(byte == FRAME_END){
      rx2Flag = 1;
    }
  }
}

