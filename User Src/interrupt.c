#include "UserBaseLib.h"
//#include <stdio.h>

/************************************************************************************************************
*    Timer2 interrupt subroutine
************************************************************************************************************/
void Timer2_ISR(void) interrupt 5
{
    static u16 Flag2msCnt = 0, Flag1msCnt;

    clr_TF2; //Clear Timer2 Interrupt Flag
    FlagState.u100 = 1;
    if (++Flag1msCnt >= 10)
    {
        Flag1msCnt = 0;
        FlagState.ms1 = 1;
        HeatPWM(TempIntensity, FlagState.work);
    }
    if (++Flag2msCnt >= 20)
    {
        Flag2msCnt = 0;
        FlagState.ms2 = 1;
    }
    BIO1PWM(1, FlagState.work);
    BIO1Power(BIOIntensity, FlagState.work);
    MCU_DK_DisAndKey_Handle();
}

/**
 * FUNCTION_PURPOSE: serial interrupt, echo received data.
 * FUNCTION_INPUTS: P0.7(RXD) serial input
 * FUNCTION_OUTPUTS: P0.6(TXD) serial output
 */ 
void SerialPort0_ISR(void) interrupt 4 
{
    if (RI==1) 
    {                                       /* if reception occur */
        clr_RI;                             /* clear reception flag for next reception */
        // UART_BUFFER[u16CNT] = SBUF;
    }
    if(TI==1)
    {
        clr_TI;                             /* if emission occur */
    }
}

/*for printf*/
char putchar (char c)  
{
    TI = 0;
    SBUF = c;
    while(TI==0);
    return c;
}
