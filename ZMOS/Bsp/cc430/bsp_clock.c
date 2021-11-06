/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* bsp_clock.c
*
* DESCRIPTION:
*     Provide clock tick for ZMOS.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/8/6
* REVISION:
*     v0.1
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*
*****************************************************************/
 
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "driverlib.h"
#include "bsp_clock.h"
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
static uint32_t clockTicks = 0;
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/
static void setTimerTimeout(uint32_t time_ms)
{
    uint16_t timePeriod = 0;
    uint32_t timeClock = 0;
    
    if(!time_ms) timePeriod = 1;
    else
    {
        timeClock = UCS_getSMCLK()/8;
        timePeriod = time_ms * timeClock/1000;
    }
    Timer_A_stop(TIMER_A0_BASE);
    //Start timer in upMode sourced by ACLK
	Timer_A_clearTimerInterrupt(TIMER_A0_BASE);

    Timer_A_initUpModeParam initUpParam = {0};
    initUpParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initUpParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_8;
    initUpParam.timerPeriod = timePeriod - 300; // -300 : compensation. test - the clock is not accurate.
    initUpParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    initUpParam.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    initUpParam.timerClear = TIMER_A_DO_CLEAR;
    initUpParam.startTimer = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &initUpParam);
}
/*****************************************************************
* FUNCTION: bsp_clockInit
*
* DESCRIPTION:
*     Bsp clock initialize.
* INPUTS:
*     null
* RETURNS:
*     Clock count.
* NOTE:
*     null
*****************************************************************/
void bsp_clockInit(void)
{
    clockTicks = 0;
    setTimerTimeout(1);
}
/*****************************************************************
* FUNCTION: bsp_getClockCount
*
* DESCRIPTION:
*     Get clock count, it provide system clock for ZMOS.
* INPUTS:
*     null
* RETURNS:
*     Clock count.
* NOTE:
*     null
*****************************************************************/
uint32_t bsp_getClockCount(void)
{
    return clockTicks;
}


//******************************************************************************
//
//This is the Timer0_A5 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER0_A1_VECTOR)))
#endif
void TIMER0_A1_ISR (void)
{
    switch (__even_in_range(TA0IV,14)){
        case  0: break;                          //No interrupt
        case  2: break;                          //CCR1 not used
        case  4: break;                          //CCR2 not used
        case  6: break;                          //reserved
        case  8: break;                          //reserved
        case 10: break;                          //reserved
        case 12: break;                          //reserved
        case 14:                                 //overflow
            clockTicks++;
            break;
        default: break;
    }
}
/****************************************************** END OF FILE ******************************************************/
