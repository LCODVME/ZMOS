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
    zmos_timerInit();
}

/*****************************************************************
* FUNCTION: zmos_system_start
*
* DESCRIPTION:
*     ZMOS system run start
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
        zmos_systemClockUpdate();
        
        zmos_taskStartScheduler();
        
    }
}
/****************************************************** END OF FILE ******************************************************/
