/************************************************************************
Name: UserBaseLib for N76E003 KEIL
Editor: Aysi
Date: 2019.10.15
************************************************************************/

#ifndef _User_Base_Lib_H
#define _User_Base_Lib_H

#include "N76E003_BSP.h"

#define BIT_CLEAR(dv,db) (dv &= (~(1<<db)))
#define BIT_SET(dv,db) (dv |= (1<<db))
#define BIT_GET(dv,db) (dv&(1<<db))
#define BIT0_1 0x01
#define BIT1_1 0x02
#define BIT2_1 0x04
#define BIT3_1 0x08
#define BIT4_1 0x10
#define BIT5_1 0x20
#define BIT6_1 0x40
#define BIT7_1 0x80

void Uart0Init(u32 u32Baudrate);
volatile void Delay(uint16_t nCount);
void UART0SendBuf(u8 *SendBufAddr, u8 SendLen);
void IWDG_Configuration(void);
#define IWDG_Feed set_WDCLR
void Tim2_Time_Upmode_conf(uint16_t TIM2_Period);
void PWM_Init(uint16_t Period, uint16_t pules);
void AD1Init(void);

//App define
#define HEAT_PIN P15
#define LED2_CON P06
#define LED1_CON P07
#define BIOS_PIN P03
#define BIOA_PIN P04
#define POWER_PIN P20
#define KEEP_PIN P17

#define A_OUT_PIN P01
#define B_OUT_PIN P00
#define C_OUT_PIN P10
#define D_OUT_PIN P11
#define E_OUT_PIN P12

#define A_IN_PIN P01
#define B_IN_PIN P00
#define C_IN_PIN P10
#define D_IN_PIN P11
#define E_IN_PIN P12

//App value
typedef struct
{
 unsigned char ms1        : 1;
 unsigned char ms2        : 1;
 unsigned char u100       : 1;
 unsigned char ms4        : 1;
 unsigned char ms5        : 1;
 unsigned char ms6        : 1;
 unsigned char error      : 1;
 unsigned char work       : 1;
}_StateBit;
extern _StateBit FlagState;

extern u8 SendBufLen;
extern u8 *SendBuffer;
extern u8 UART1BusyFlag;

// extern u8 EEPROMLocal;
// extern u32 EECountTemp;

//App Function
// u32 EEPROMRestartDeal();
// void EEPROMStorage(u32 *EECount);

//Key get
#define KEY_SEL 0X01
#define KEY_START 0X02
#define KEY_INTEN_UP 0X04
#define KEY_CHARGE 0X08
#define KEY_POWER_KEEP 0X10
#define KEY_INTEN_DOWN 0X20
#define KEY_TEMP_DOWN 0X40

void Key_Scan(void);

extern u8 TempIntensity, BIOIntensity, BIOMode;
void BIO1PWM(u8 i, u8 Work);
void BIO1Power(u8 i, u8 Work);
void HeatPWM(u8 i, u8 Work);

#endif
