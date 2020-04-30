/*****************************************************************************
Name: hammer
version: v1.0 Released
Editor: Aysi
Describe: 
History: 
2019.12.26: Created
*****************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
// #define DEBUG
#include "UserBaseLib.h"
#include "delay.h"
#include "oled.h"
//#include <stdio.h>
// #include "UART_Frame.h"

/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

//BIO1 PWM
u8 BIOIntensity = 0;
const u16 BIO1IntensityTable[] = {0, 25, 30, 33, 36, 39,
																	42, 45, 50};
const u16 BIO1ModPeriod[] = {11, 330, 400};
const u16 BIO1ModCompare[] = {3, 2, 240};	
void BIO1PWM(u8 i, u8 Work)
{
	static u16 BIO1TimeCnt = 0;
	static u8 BIO1ModRenew = 0;
	
	if(Work)
	{
		if(++BIO1TimeCnt>=BIO1ModPeriod[BIO1ModRenew])
		{
			BIO1TimeCnt = 0;
			BIO1ModRenew = i;
		}
		if((BIO1TimeCnt<BIO1ModCompare[BIO1ModRenew]))
			BIOS_PIN = 0; //open
		else
			BIOS_PIN = 1; //close
	}
	else
	{
		BIO1TimeCnt = 0;
		BIOS_PIN = 1; //close
	}
}

void BIO1Power(u8 i, u8 Work)
{
  static u16 BIO1TimeCnt = 0;
	static u8 BIO1ModRenew = 0;
	
	if(Work)
	{
		if(++BIO1TimeCnt>=75)
		{
			BIO1TimeCnt = 0;
			BIO1ModRenew = i;
		}
		if((BIO1TimeCnt<BIO1IntensityTable[BIO1ModRenew]))
			BIOA_PIN = 1;
		else
			BIOA_PIN = 0;
	}
	else
	{
		BIO1TimeCnt = 0;
		BIOA_PIN = 0;
	}
}

//Heat
u8 TempIntensity = 0;
const u16 HeatIntensityTable[] = {0, 1, 2, 3, 4,
																	5, 6, 7, 8};
void HeatPWM(u8 i, u8 Work)
{
	static u16 HeatTimeCnt = 0;
	static u8 HeatModRenew = 0;
	
	if(Work)
	{
		if(++HeatTimeCnt>=60)
		{
			HeatTimeCnt = 0;
			HeatModRenew = i;
		}
		if((HeatTimeCnt<HeatIntensityTable[HeatModRenew]))
			HEAT_PIN = 1;	//ON
		else
			HEAT_PIN = 0;	//OFF
	}
	else
	{
		HeatTimeCnt = 0;
		HEAT_PIN = 0;	//ON
	}
}

//key scan
static u8 MCU_DK_KeyData = 0;

u8 Key_Get(void)
{
  return MCU_DK_KeyData;
}

void Key_Scan(void)
{
  static u8 keyCnt1 = 0, keyCnt2 = 0, keyCnt3 = 0, keyCnt4 = 0, keyCnt5 = 0, keyCnt6 = 0;
  static u16 KeyStartCnt = 0;

  if (POWER_PIN == 1)
  {
    if (++keyCnt2 >= 20) //sensibility
    {
      keyCnt2 = 20;
      MCU_DK_KeyData |= KEY_START;
      if (++KeyStartCnt >= 3000) //3s
      {
        KeyStartCnt = 3000;
        MCU_DK_KeyData |= KEY_POWER_KEEP;
      }
    }
  }
  else
  {
    keyCnt2 = 0;
    KeyStartCnt = 0;
    MCU_DK_KeyData &= ~KEY_START;
    MCU_DK_KeyData &= ~KEY_POWER_KEEP;
  }
  if (A_IN_PIN == 0)
  {
    if (++keyCnt1 >= 20) //sensibility
    {
      keyCnt1 = 20;
      MCU_DK_KeyData |= KEY_TEMP_DOWN;
    }
  }
  else
  {
    keyCnt1 = 0;
    MCU_DK_KeyData &= ~KEY_TEMP_DOWN;
  }
  if (B_IN_PIN == 0)
  {
    if (++keyCnt3 >= 20) //sensibility
    {
      keyCnt3 = 20;
      MCU_DK_KeyData |= KEY_SEL;
    }
  }
  else
  {
    keyCnt3 = 0;
    MCU_DK_KeyData &= ~KEY_SEL;
  }
  if (C_IN_PIN == 0)
  {
    if (++keyCnt4 >= 20) //sensibility
    {
      keyCnt4 = 20;
      MCU_DK_KeyData |= KEY_INTEN_DOWN;
    }
  }
  else
  {
    keyCnt4 = 0;
    MCU_DK_KeyData &= ~KEY_INTEN_DOWN;
  }
  if (D_IN_PIN == 0)
  {
    if (++keyCnt5 >= 20) //sensibility
    {
      keyCnt5 = 20;
      MCU_DK_KeyData |= KEY_INTEN_UP;
    }
  }
  else
  {
    keyCnt5 = 0;
    MCU_DK_KeyData &= ~KEY_INTEN_UP;
  }
  if (E_IN_PIN == 0)
  {
    if (++keyCnt6 >= 20) //sensibility
    {
      keyCnt6 = 20;
      MCU_DK_KeyData |= KEY_CHARGE;
    }
  }
  else
  {
    keyCnt6 = 0;
    MCU_DK_KeyData &= ~KEY_CHARGE;
  }
}

#define FUNCTION_BIO 0
#define FUNCTION_HEAT 1

extern const unsigned char code PIC1[][32];

void main(void)
{ 
  u16 s1 = 0, adtemp = 0, power_up = 0; 
  u8 VolLowCnt = 0, ChaLowCnt = 0;
  u8 TaskNumber = 1, KeyValue = 0;
  u8 KeyUp = 0, KeyT = 0;
  u8 averageCnt = 0;
  u8 FunctionSelect = 0, CharColor = 0, roll = 0;
  
  P17_PushPull_Mode; KEEP_PIN = 1; //power kepp
  //io init 
  //note: after power up, N76E003 IO is only input(float) mode, P0,P1,P3 is 1 P2 only input mode
  clr_P2S_0; //POWER KEY, HIZ mode
  // Quasi-Bidirectional MODE
  P15_PushPull_Mode; HEAT_PIN = 0; //HEAT
  P06_PushPull_Mode; LED2_CON = 0; //LED
  P07_PushPull_Mode; LED1_CON = 0; 
  P03_PushPull_Mode; BIOS_PIN = 1; //BIO S
  P04_PushPull_Mode; BIOA_PIN = 0;//BIO A

  P01_Quasi_Mode; A_OUT_PIN = 1; //A
  P00_Quasi_Mode; B_OUT_PIN = 1; //B
  P10_Quasi_Mode; C_OUT_PIN = 1; //C
  P11_Quasi_Mode; D_OUT_PIN = 1; //D
  P12_Quasi_Mode; E_OUT_PIN = 1; //E

  #ifndef DEBUG
  IWDG_Configuration(); //Open IWDG
  #endif

  // Uart1Init(115200);
  Tim2_Time_Upmode_conf(TIMER_DIV4_VALUE_100us);  //100us      
  set_EA;//Open main interrupt

  // delay_init();
  // PWM_Init(0x7CF, 0);
  AD1Init();

  delay_ms(100);

  	OLED_Init();			//init oled  
		OLED_Clear(0);

    KeyValue = Key_Get(); //display version
    if((KeyValue&KEY_INTEN_UP)==KEY_INTEN_UP&&(KeyValue&KEY_INTEN_DOWN)==KEY_INTEN_DOWN) 
    {
      OLED_ShowString(5, 2, "v1.0 by MTF", 16, 1);
      delay_ms(2000);
    }

    while (1)
    {
#ifndef DEBUG
    IWDG_Feed;  //Clear IWDG cnt
#endif

    if (power_up == 0)
    {
      power_up = 1;
      OLED_fill_picture(&PIC1[0][0]);
      delay_ms(1000);
      OLED_Clear(0);
      OLED_ShowHZK(3, 2, 0, 24, FunctionSelect); //微电
      OLED_ShowHZK(3 + 24, 2, 1, 24, FunctionSelect);
      OLED_ShowHZK(3, 5, 2, 24, FunctionSelect + 1); //发热
      OLED_ShowHZK(3 + 24, 5, 3, 24, FunctionSelect + 1);
      OLED_ShowNum(86, 2, BIOIntensity, 1, 24, 1);
      OLED_ShowNum(86, 5, TempIntensity, 1, 24, 1);
      OLED_ShowHZK(50, 0, 3, 16, 1); //暂停
      OLED_ShowHZK(50 + 16, 0, 4, 16, 1);
      if (FunctionSelect == FUNCTION_BIO)
      {
        OLED_ShowChar(60, 2, '<', 24, 1);
        OLED_ShowChar(60, 5, ' ', 24, 1);
        OLED_ShowChar(109, 2, '>', 24, 1);
        OLED_ShowChar(109, 5, ' ', 24, 1);
      }
      else
      {
        OLED_ShowChar(60, 2, ' ', 24, 1);
        OLED_ShowChar(60, 5, '<', 24, 1);
        OLED_ShowChar(109, 2, ' ', 24, 1);
        OLED_ShowChar(109, 5, '>', 24, 1);
      }
    }
    
    if (FlagState.ms2)
    {
      FlagState.ms2 = 0;
      switch (TaskNumber)
      {
        case 1: //UART T&R process
        {
          TaskNumber++;
          break;
        }
        case 2: //KEY GET
        {
          KeyValue = Key_Get();
          TaskNumber++;
          break;
        }
        case 3: //key process
        {       
          if((KeyValue&KEY_START)!=KEY_START&&(KeyValue&KEY_INTEN_UP)!=KEY_INTEN_UP&&
              (KeyValue&KEY_SEL)!=KEY_SEL&&(KeyValue&KEY_INTEN_DOWN)!=KEY_INTEN_DOWN&&
              (KeyValue&KEY_TEMP_DOWN)!=KEY_TEMP_DOWN)
          {
            KeyUp = 1;
          }   
          if(((KeyValue&KEY_POWER_KEEP)==KEY_POWER_KEEP))
          {
            OLED_Clear(0);
            KEEP_PIN = 0; //close keep voltage, after releasing key, it will close
            Set_All_GPIO_Only_Input_Mode;
          }
          
          if(((KeyValue&KEY_START)==KEY_START)&&KeyUp)
          {
            KeyUp = 0;
            if (FlagState.work)
            {
              TempIntensity = 0;
              BIOIntensity = 0;
              FlagState.work = 0;
              OLED_ShowHZK(50, 0, 3, 16, 1); //暂停
              OLED_ShowHZK(50 + 16, 0, 4, 16, 1);
              OLED_ShowNum(86, 2, BIOIntensity, 1, 24, 1);
              OLED_ShowNum(86, 5, TempIntensity, 1, 24, 1);
              OLED_ShowChar(50 + 32 + 8 + (roll)*8, 0, ' ', 16, 1);
              OLED_ShowChar(50 - 8 - 8 - (roll)*8, 0, ' ', 16, 1);
              roll = 0;
            }
            else
            {
              OLED_ShowHZK(50, 0, 0, 16, 1); //工作
              OLED_ShowHZK(50 + 16, 0, 1, 16, 1);
              FlagState.work = 1; 
            } 
          }
          else if(((KeyValue&KEY_INTEN_UP)==KEY_INTEN_UP)&&KeyUp)
          {           
            KeyUp = 0;
            if (FunctionSelect == FUNCTION_BIO)
            {
              if (BIOIntensity == 8)
              {
              }
              else
              {
                BIOIntensity++;
                OLED_ShowNum(86, 2, BIOIntensity, 1, 24, 1);
              }
            }
            else
            {
              if (TempIntensity == 8)
              {
              }
              else
              {
                TempIntensity++;
                OLED_ShowNum(86, 5, TempIntensity, 1, 24, 1);
              }
            }
          }
          else if(((KeyValue&KEY_SEL)==KEY_SEL)&&KeyUp)
          {
            KeyUp = 0;
            FunctionSelect = !FunctionSelect;
            OLED_ShowHZK(3, 2, 0, 24, FunctionSelect); //微电
            OLED_ShowHZK(3 + 24, 2, 1, 24, FunctionSelect);
            OLED_ShowHZK(3, 5, 2, 24, FunctionSelect + 1); //发热
            OLED_ShowHZK(3 + 24, 5, 3, 24, FunctionSelect + 1);
            if(FunctionSelect==FUNCTION_BIO)
            {
              OLED_ShowChar(60, 2, '<', 24, 1);
              OLED_ShowChar(60, 5, ' ', 24, 1);
              OLED_ShowChar(109, 2, '>', 24, 1);
              OLED_ShowChar(109, 5, ' ', 24, 1);
            }
            else
            {
              OLED_ShowChar(60, 2, ' ', 24, 1);
              OLED_ShowChar(60, 5, '<', 24, 1);
              OLED_ShowChar(109, 2, ' ', 24, 1);
              OLED_ShowChar(109, 5, '>', 24, 1);
            }
          }     
          else if(((KeyValue&KEY_INTEN_DOWN)==KEY_INTEN_DOWN)&&KeyUp)
          {
            KeyUp = 0;
            if (FunctionSelect == FUNCTION_BIO)
            {
              if (BIOIntensity == 0)
              {
              }
              else
              {
                BIOIntensity--;
                OLED_ShowNum(86, 2, BIOIntensity, 1, 24, 1);
              }
            }
            else
            {
              if (TempIntensity == 0)
              {
              }
              else
              {
                TempIntensity--;
                OLED_ShowNum(86, 5, TempIntensity, 1, 24, 1);
              }
            }
          }     
          else if(((KeyValue&KEY_TEMP_DOWN)==KEY_TEMP_DOWN)&&KeyUp)
          {
            KeyUp = 0;
          }     
          TaskNumber++;    
          break;
        }
        case 4: //work process
        {
          if (++s1 >= 100) //1s
          {
            s1 = 0;
            if(FlagState.work)
            {
              if (++roll > 3)
              {
                OLED_ShowChar(50 + 32 + 8 + (roll - 1) * 8, 0, ' ', 16, 1);
                OLED_ShowChar(50 - 8 - 8 - (roll - 1) * 8, 0, ' ', 16, 1);
                roll = 0;
              }
              else
              {
                OLED_ShowChar(50 + 32 + roll * 8, 0, ' ', 16, 1);
                OLED_ShowChar(50 + 32 + 8 + roll * 8, 0, ' ', 16, 0);
                OLED_ShowChar(50 - 8 - roll * 8, 0, ' ', 16, 1);
                OLED_ShowChar(50 - 8 - 8 - roll * 8, 0, ' ', 16, 0);
              }
            }
          }

          if(FlagState.work)
          {
            LED1_CON = 1; 
            LED2_CON = 1;
          }
          else
          {
            LED1_CON = 0;
            LED2_CON = 0;
          }     
          TaskNumber++;
          break;
        }
        case 5: //error process
        {
          TaskNumber = 1;
          break;
        }
        default:
        {
          TaskNumber = 5;
          FlagState.error = 1;
          break;
        }
      }
    }
  } 
}
