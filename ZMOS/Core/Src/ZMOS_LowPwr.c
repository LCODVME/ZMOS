/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_LowPwr.c
*
* DESCRIPTION:
*     ZMOS low power management function.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/5/31
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
#include "ZMOS.h"
#include "ZMOS_Timers.h"
#include "bsp_lpm.h"
#include "ZMOS_Tasks.h"
#include "ZMOS_LowPwr.h"
     
#if ZMOS_USE_LOW_POWER
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
static zm_uint32_t zmos_lowPwrEvents = 0;
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
/*****************************************************************
* FUNCTION: zmos_lowPwrMgrInit
*
* DESCRIPTION:
*     ZMOS initialize the low power management system.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_lowPwrMgrInit(void)
{
    zmos_lowPwrEvents = 0;
}
/*****************************************************************
* FUNCTION: zmos_lowPwrSetEvent
*
* DESCRIPTION:
*     This function to set low power event.
* INPUTS:
*     event : Low power operation events bit(use 0 ~ 30).
* RETURNS:
*     null
* NOTE:
*     The bit 31 use by zmos system.
*****************************************************************/
void zmos_lowPwrSetEvent(zm_uint8_t event)
{
    if(event < 32)
    {
        zmos_lowPwrEvents |= BS(event);
    }
}
/*****************************************************************
* FUNCTION: zmos_lowPwrClearEvent
*
* DESCRIPTION:
*     This function to clear low power event.
* INPUTS:
*     event : Low power operation events bit(use 0 ~ 30).
* RETURNS:
*     null
* NOTE:
*     The bit 31 use by zmos system.
*****************************************************************/
void zmos_lowPwrClearEvent(zm_uint8_t event)
{
    if(event < 32)
    {
        zmos_lowPwrEvents &= BC(event);
    }
}
/*****************************************************************
* FUNCTION: zmos_lowPowerManagement
*
* DESCRIPTION:
*     This function is for power management.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     Shouldn't be called from anywhere else.
*****************************************************************/
void zmos_lowPowerManagement(void)
{
    // When no event runs
    if(zmos_lowPwrEvents == 0
#if ZMOS_LPM_WAIT_IDLE
       && !zmos_checkTaskIsIdle()
#endif
           )
    {
        zm_uint32_t nextTimeout;

        ZMOS_ENTER_CRITICAL();
        // Get next timeout
        nextTimeout  = zmos_getNextLowestTimeout();
        
        ZMOS_EXIT_CRITICAL();
        //Processing before entering low power
        bsp_lowPwrEnterBefore(nextTimeout);
        //Enter low power
        bsp_systemEnterLpm();
        //Processing after low power
        bsp_lowPwrExitAfter();
    }
}
#else
void zmos_lowPwrMgrInit(void) {}
void zmos_lowPwrSetEvent(zm_uint8_t event) {}
void zmos_lowPwrClearEvent(zm_uint8_t event) {}
void zmos_lowPowerManagement(void) {}
#endif
/****************************************************** END OF FILE ******************************************************/
