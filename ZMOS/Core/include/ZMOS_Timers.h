/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Timers.h
*
* DESCRIPTION:
*     ZMOS timer function.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/5/19
* REVISION:
*     v0.1
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*
*****************************************************************/
#ifndef __ZMOS_TIMERS_H__
#define __ZMOS_TIMERS_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS_Tasks.h"
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
/* T */
#define TIMER_MAX_TIMEOUT           0xFFFFFFFF
/* ZMOS task return cordes */
#define ZMOS_TIMER_SUCCESS          0
#define ZMOS_TIMER_FAILD            1
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
/**
 * ZMOS timer result type.
 * @ref ZMOS timer return cordes.
 */
typedef uint8_t timerReslt_t;
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/

/*****************************************************************
* FUNCTION: zmos_timerInit
*
* DESCRIPTION:
*     ZMOS timer initialize
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_timerInit(void);
/*****************************************************************
* FUNCTION: zmos_startSingleTimer
*
* DESCRIPTION:
*     This function is called to start a single timer.
* INPUTS:
*     pTaskHandle : Which task to set event.
*     event : What event to set.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_startSingleTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, uint32_t timeout);
/*****************************************************************
* FUNCTION: zmos_startReloadTimer
*
* DESCRIPTION:
*     This function is called to start a reload timer.
* INPUTS:
*     pTaskHandle : Which task to set event.
*     event : What event to set.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_startReloadTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, uint32_t timeout);
/*****************************************************************
* FUNCTION: zmos_stopTimer
*
* DESCRIPTION:
*     This function to stop a timer.
* INPUTS:
*     pTaskHandle : task handle of timer to stop.
*     event : task event of timer to stop.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_stopTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event);
/*****************************************************************
* FUNCTION: zmos_getCurrentTimeout
*
* DESCRIPTION:
*     Get timer current timeout.
* INPUTS:
*     pTaskHandle : task handle of timer to check.
*     event : task event of timer to check.
* RETURNS:
*     Return the timer's tick count if found, zero otherwise.
* NOTE:
*     null
*****************************************************************/
uint32_t zmos_getCurrentTimeout(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event);
/*****************************************************************
* FUNCTION: zmos_getNextLowestTimeout
*
* DESCRIPTION:
*     Search timer table to return the lowest timeout value. 
* INPUTS:
*     null
* RETURNS:
*     The lowest timeout value. 
* NOTE:
*     If the timer list is empty, then the returned timeout will 
*     be TIMER_MAX_TIMEOUT.
*****************************************************************/
uint32_t zmos_getNextLowestTimeout(void);
/*****************************************************************
* FUNCTION: zmos_timeTickUpdate
*
* DESCRIPTION:
*     Update the timer time tick.
* INPUTS:
*     upTime : update time value.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_timeTickUpdate(uint32_t upTime);
/*****************************************************************
* FUNCTION: zmos_getTimerClock
*
* DESCRIPTION:
*     Read the local timer clock.
* INPUTS:
*     null
* RETURNS:
*     timer clock.
* NOTE:
*     null
*****************************************************************/
uint32_t zmos_getTimerClock(void);


#ifdef __cplusplus
}
#endif
#endif /* ZMOS_Timers.h */
