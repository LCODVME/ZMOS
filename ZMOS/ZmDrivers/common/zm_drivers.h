/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* zm_drivers.h
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
#ifndef __ZM_DRIVERS_H__
#define __ZM_DRIVERS_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/

/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
/***********************************************
 * ZM driver event.
 *
 * MIN = 0
 * MAX = 31
 */
#define ZM_DRIVER_LED_BLINK_EVENT       BS(0)
#define ZM_DRIVER_KEY_POLL_EVENT        BS(1)
     
     
     
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
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
void zmDriverInit(void);
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
taskReslt_t zmDriverSetEvent(uTaskEvent_t events);
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
timerReslt_t zmDriverSetTimerEvent(uTaskEvent_t events, uint32_t timeout, bool reload);
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
timerReslt_t zmDriverStopTimerEvent(uTaskEvent_t events);

#ifdef __cplusplus
}
#endif
#endif /* zm_drivers.h */
