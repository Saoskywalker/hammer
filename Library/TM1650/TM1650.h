/**********************************
TM1650Ð¾Æ¬¿ØÖÆ
**********************************/

#ifndef __TM1650_H
#define __TM1650_H

#include "UserBaseLib.h"
#include "delay.h"

//relate hardware 
#define SCL_TM1650              GPIOB_OUT->ODR4
#define SDA_TM1650              GPIOB_OUT->ODR5 //OUTPUT
#define SDAR_TM1650             GPIOB_IN->IDR5  //INPUT
#define SET_SCL_OUT_TM1650()    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_SLOW);
#define SET_SDA_OUT_TM1650()    {GPIOB->DDR|=GPIO_PIN_5;GPIOB->CR2&=~GPIO_PIN_5;GPIOB->CR1&=~GPIO_PIN_5;}
#define SET_SDA_IN_TM1650()     {GPIOB->DDR&=~GPIO_PIN_5;GPIOB->CR2&=~GPIO_PIN_5;GPIOB->CR1&=~GPIO_PIN_5;}

#define TM1650_IIC_Delay_us(z) delay_us((u8)z);  
///////////////////////////////// 
 
u8 TM1650_Key(void);
void TM1650_Display(u8 add, u8 dat);
u8 TM1650_Init(u8 light, u8 dis);
 
#endif
