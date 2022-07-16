/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* zm_drivers.c
*
* DESCRIPTION:
*     Based on the ZMOS system driver.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/6/1
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
#include "zm_driverConfig.h"
#include "zm_drivers.h"

#if ZM_LED_MAX_NUM > 0
#include "zm_led.h"
#endif

#if ZM_KEY_MAX_NUM > 0
#include "zm_key.h"
#endif
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
static zmos_taskHandle_t driverTaskHandle;
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
static uTaskEvent_t zmDriverPorcessEvent(uTaskEvent_t events);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
#if ZM_LED_MAX_NUM > 0
extern void zm_ledInit(void);
#ifdef ZM_LED_BLINK
extern void zm_updateLedBlink(void);
#endif /* ZM_LED_BLINK */
#endif /* ZM_LED_MAX_NUM > 0 */

/* ZM key */
#if ZM_KEY_MAX_NUM > 0
extern void zm_keyInit(void);
extern void zm_keyPollProcess(void);
#endif /* ZM_KEY_MAX_NUM > 0 */
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: zmDriverInit
*
* DESCRIPTION:
*     ZM drvers initialize
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmDriverInit(void)
{
    //Register task in ZMOS
    zmos_taskThreadRegister(&driverTaskHandle, zmDriverPorcessEvent);
    /* ZM led */
#if ZM_LED_MAX_NUM > 0
    zm_ledInit();
#endif
    /* ZM key */
#if ZM_KEY_MAX_NUM > 0
    zm_keyInit();
#endif
}
/*****************************************************************
* FUNCTION: zmDriverPorcessEvent
*
* DESCRIPTION:
*     
* INPUTS:
*     
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
static uTaskEvent_t zmDriverPorcessEvent(uTaskEvent_t events)
{
    
    if(events & ZM_DRIVER_LED_BLINK_EVENT)
    {
#ifdef ZM_LED_BLINK
        zm_updateLedBlink();
#endif
        return events ^ ZM_DRIVER_LED_BLINK_EVENT;
    }
    
    if(events & ZM_DRIVER_KEY_POLL_EVENT)
    {
#if ZM_KEY_MAX_NUM > 0
        zm_keyPollProcess();
#endif
        return events ^ ZM_DRIVER_KEY_POLL_EVENT;
    }
    return 0;
}
/*****************************************************************
* FUNCTION: zmDriverSetEvent
*
* DESCRIPTION:
*     This function set event in drver process.
* INPUTS:
*     events : The event to set.
* RETURNS:
*     0 : success.
* NOTE:
*     null
*****************************************************************/
taskReslt_t zmDriverSetEvent(uTaskEvent_t events)
{
    return zmos_setTaskEvent(driverTaskHandle, events);
}
/*****************************************************************
* FUNCTION: zmDriverSetTimerEvent
*
* DESCRIPTION:
*     This function start an event timer.
* INPUTS:
*     events : The event to set.
*     timeout : timeout.
*     reload : Is reload timer.
*              true : reload timer.
*              false : single timer.
* RETURNS:
*     0 : success.
* NOTE:
*     If the event timer already exists, Timeout and Reload 
*     properties will be updated.
*****************************************************************/
timerReslt_t zmDriverSetTimerEvent(uTaskEvent_t events, uint32_t timeout, bool reload)
{
    if(reload)
    {
        return zmos_startReloadTimer(driverTaskHandle, events, timeout);
    }
    else
    {
        return zmos_startSingleTimer(driverTaskHandle, events, timeout);
    }
}
/*****************************************************************
* FUNCTION: zmDriverStopTimerEvent
*
* DESCRIPTION:
*     This function stop an event timer.
* INPUTS:
*     events : The event to stop.
* RETURNS:
*     0 : success.
*     other : The timer doesn't exist.
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmDriverStopTimerEvent(uTaskEvent_t events)
{
    return zmos_stopTimer(driverTaskHandle, events);
}

#if (defined ZMOS_INIT_SECTION) && (ZMOS_INIT_SECTION)
ZNOS_INIT_REGISTER(zmDriverInit);
#endif
/****************************************************** END OF FILE ******************************************************/
