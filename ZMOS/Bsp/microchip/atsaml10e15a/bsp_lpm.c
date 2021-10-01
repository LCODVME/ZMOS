/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* bsp_clock.c
*
* DESCRIPTION:
*     Low power management bsp.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/7/8
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
#include "definitions.h"
#include "bsp_lpm.h"
#include "common.h"
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
#define LPM_IDLE_MODE           1
#define LPM_STANDBY_MODE        2
#define LPM_OFF_MODE            3
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/

/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
static uint32_t lowPwrTime = 0;
static uint8_t sleepMode = 0;
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
void bsp_clockInit(void);
void timerClockLowPowerInit(void);
void bsp_compensateClockCount(uint32_t value);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/

/*****************************************************************
* FUNCTION: lowPowerTimeout
*
* DESCRIPTION:
*     .
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
static void lowPowerTimeout(TC_TIMER_STATUS status, uintptr_t context)
{
    bsp_compensateClockCount(lowPwrTime);
    lowPwrTime = 0;
}

/*****************************************************************
* FUNCTION: bsp_lowPwrEnterBefore
*
* DESCRIPTION:
*     This function is called before entering low power.
* INPUTS:
*     timeout : zmos timer next timeout.
*               A value of 0xFFFFFFFF indicates 
*               that no timer is running.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void bsp_lowPwrEnterBefore(uint32_t timeout)
{
    if(timeout)
    {
        lowPwrTime = timeout;
        SYSTICK_TimerStop();
        
        sleepMode = LPM_STANDBY_MODE;
        if(timeout != 0xFFFFFFFF)
        {
            uint32_t timerPeriod;
            
            TC0_TimerStop();
            
            timerPeriod = TC0_TimerFrequencyGet() / 1000/*000*/;  // to us
            //timerPeriod *= SYSTEM_CLOCK_US;
            timerPeriod *= lowPwrTime;
            
           // TC0_TimerInitialize();
            TC0_Timer32bitPeriodSet(timerPeriod);
            TC0_Timer32bitCounterSet(0);
            TC0_TimerCallbackRegister(lowPowerTimeout, 0);
            TC0_TimerStart();
        }
    }
}
/*****************************************************************
* FUNCTION: bsp_systemEnterLpm
*
* DESCRIPTION:
*     This function put the cpu enter low power mode.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void bsp_systemEnterLpm(void)
{
    switch(sleepMode)
    {
    case LPM_IDLE_MODE:
        PM_IdleModeEnter();
        break;
    case LPM_STANDBY_MODE:
        PM_StandbyModeEnter();
        break;
    case LPM_OFF_MODE:
        PM_OffModeEnter();
        break;
    default :
        break;
    }
}
/*****************************************************************
* FUNCTION: bsp_lowPwrExitAfter
*
* DESCRIPTION:
*     This function is called after exiting low power.
*     According to the MCU characteristics of low-power 
*     wake up processing, such as: initialization clock, 
*     clock compensation, etc.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void bsp_lowPwrExitAfter(void)
{
    TC0_TimerStop();
    if(lowPwrTime && lowPwrTime != 0xFFFFFFFF)
    {
        uint32_t timerRun = 0;
        timerRun = TC0_Timer32bitCounterGet();
        timerRun /= (TC0_TimerFrequencyGet() / 1000/*000*/);
        //timerRun /= SYSTEM_CLOCK_US;
        bsp_compensateClockCount(timerRun);
    }
    lowPwrTime = 0;
    SYS_Initialize ( NULL );
    bsp_clockInit();
}
/****************************************************** END OF FILE ******************************************************/
