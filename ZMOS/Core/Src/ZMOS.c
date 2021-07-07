/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS.c
*
* DESCRIPTION:
*     ZMOS
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/5/16
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
#include "ZMOS_Common.h"
#include "ZMOS_Timers.h"
#include "ZMOS_Tasks.h"
#include "bsp_clock.h"
#include "bsp.h"
#include "ZMOS.h"
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
/* ZMOS nesting variable */
static uint16_t zmosCriticalNesting = 0xCCCC;
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
extern void zmos_taskStartScheduler(void);
#if ZMOS_USE_LOW_POWER
extern void zmos_lowPowerManagement(void);
#endif
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: zmos_systemClockUpdate
*
* DESCRIPTION:
*     Update system clock.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
static void zmos_systemClockUpdate(void)
{
    uint32_t zmos_clock;
    uint32_t clockCnt;
    
    //Get the clock count of timer ticks.
    clockCnt = bsp_getClockCount();
    zmos_clock = zmos_getTimerClock();
    
    if(zmos_clock != clockCnt)
    {
        zmos_timeTickUpdate(clockCnt - zmos_clock);
    }
}
/*****************************************************************
* FUNCTION: zmos_sysEnterCritical
*
* DESCRIPTION:
*     System enter critical.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_sysEnterCritical(void)
{
    bsp_mcuDisableInterrupt();
    
    zmosCriticalNesting++;
}
/*****************************************************************
* FUNCTION: zmos_sysExitCritical
*
* DESCRIPTION:
*     System exit critical.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_sysExitCritical(void)
{
    if(zmosCriticalNesting && --zmosCriticalNesting == 0)
    {
        bsp_mcuEnableInterrupt();
    }
}
/*****************************************************************
* FUNCTION: zmos_system_init
*
* DESCRIPTION:
*     ZMOS system initialize.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_system_init(void)
{
    // Initialize bsp
    bsp_init();
    // Initialize zmos timer
    zmos_timerInit();
    
#if ZMOS_USE_CBTIMERS_NUM > 0
    // Initialize the callback timer
    zmos_cbTimerInit();
#endif
    
#if ZMOS_USE_LOW_POWER
    // Initialize the power management system
    zmos_lowPwrMgrInit();
#endif
    //Initialize critical nesting
    zmosCriticalNesting = 0;
}

/*****************************************************************
* FUNCTION: zmos_system_start
*
* DESCRIPTION:
*     ZMOS system run start.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     This function is the main loop function of the task system. 
*     This Function doesn't return.
*****************************************************************/
void zmos_system_start(void)
{
    while(1)
    {
        /* Loop run zmos system */
        zmos_system_run();
    }
}

/*****************************************************************
* FUNCTION: zmos_system_run
*
* DESCRIPTION:
*     This function is used to schedule once task.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_system_run(void)
{
    zmos_systemClockUpdate();
    //ZMOS start a task schedule
    zmos_taskStartScheduler();
    
#if ZMOS_USE_LOW_POWER
    // Put the processor/system into sleep
    zmos_lowPowerManagement();
#endif
}

/****************************************************** END OF FILE ******************************************************/
