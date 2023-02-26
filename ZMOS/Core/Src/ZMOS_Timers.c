/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Timers.c
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
 
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS_Common.h"
#include "ZMOS_Timers.h"
#include "ZMOS_Memory.h"
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
typedef struct zmos_timer
{
    zm_uint32_t timeout;
    zm_uint32_t reloadTime;
    zmos_taskHandle_t taskHandle;
    uTaskEvent_t event;
    struct zmos_timer *next;
}zmos_timer_t;
/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
/* Timer Clock */
static zm_uint32_t zmos_timerClock;
static zmos_timer_t *timerListHead = NULL;
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
static zmos_timer_t *zmos_findTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event);
static zmos_timer_t *zmos_addTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, zm_uint32_t timeout);
static void zmos_deleteTimer(zmos_timer_t *pTimer);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
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
void zmos_timerInit(void)
{
    zmos_timerClock = 0;
}
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
timerReslt_t zmos_startSingleTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, zm_uint32_t timeout)
{
    zmos_timer_t *newTimer;
    
    ZMOS_ENTER_CRITICAL();
    newTimer = zmos_addTimer(pTaskHandle, event, timeout);
    ZMOS_EXIT_CRITICAL();
    
    return (newTimer != NULL ? ZMOS_TIMER_SUCCESS : ZMOS_TIMER_FAILD);
}

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
timerReslt_t zmos_startReloadTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, zm_uint32_t timeout)
{
    zmos_timer_t *newTimer;
    
    ZMOS_ENTER_CRITICAL();
    newTimer = zmos_addTimer(pTaskHandle, event, timeout);
    if(newTimer)
    {
        newTimer->reloadTime = timeout;
    }
    ZMOS_EXIT_CRITICAL();
    return (newTimer != NULL ? ZMOS_TIMER_SUCCESS : ZMOS_TIMER_FAILD);
}
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
timerReslt_t zmos_stopTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event)
{
    zmos_timer_t *pTimer;
    
    pTimer = zmos_findTimer(pTaskHandle, event);
    
    if(pTimer)
    {
        zmos_deleteTimer(pTimer);
        return ZMOS_TIMER_SUCCESS;
    }
    return ZMOS_TIMER_FAILD;
}
/*****************************************************************
* FUNCTION: zmos_getCurrentTimeout
*
* DESCRIPTION:
*     Get timer current timeout.
* INPUTS:
*     pTaskHandle : task handle of timer to check.
*     event : task event of timer to check.
*     timeout : Timer timeout.
* RETURNS:
*     Return the timer's tick count if found, zero otherwise.
* NOTE:
*     null
*****************************************************************/
zm_uint32_t zmos_getCurrentTimeout(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event)
{
    zmos_timer_t *pTimer;
    
    pTimer = zmos_findTimer(pTaskHandle, event);
    
    if(pTimer)
    {
        return pTimer->timeout;
    }
    return 0;
}
/*****************************************************************
* FUNCTION: zmos_getReloadTimeout
*
* DESCRIPTION:
*     Get timer reload time.
* INPUTS:
*     pTaskHandle : task handle of timer to check.
*     event : task event of timer to check.
*     timeout : Timer timeout.
* RETURNS:
*     Timer reload time.
* NOTE:
*     null
*****************************************************************/
zm_uint32_t zmos_getReloadTimeout(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event)
{
    zmos_timer_t *pTimer;
    
    pTimer = zmos_findTimer(pTaskHandle, event);
    
    if(pTimer)
    {
        return pTimer->reloadTime;
    }
    return 0;
}
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
zm_uint32_t zmos_getNextLowestTimeout(void)
{
    zm_uint32_t timeout = TIMER_MAX_TIMEOUT;
    zmos_timer_t *srchTimer = timerListHead;
    
    while(srchTimer)
    {
        if(srchTimer->timeout < timeout)
        {
            timeout = srchTimer->timeout;
        }
        srchTimer = srchTimer->next;
    }
    return timeout;
}
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
void zmos_timeTickUpdate(zm_uint32_t upTime)
{
    zmos_timer_t *srchTimer;
    zmos_timer_t *prevTimer;
    zmos_timer_t *freeTimer;
    
    ZMOS_ENTER_CRITICAL();
    zmos_timerClock += upTime;
    ZMOS_EXIT_CRITICAL();
    
    prevTimer = NULL;
    srchTimer = timerListHead;
    
    while(srchTimer)
    {
        freeTimer = NULL;
        
        if(srchTimer->timeout > upTime)
        {
            srchTimer->timeout -= upTime;
        }
        else
        {
            srchTimer->timeout = 0;
        }
        
        if(srchTimer->timeout == 0 && srchTimer->event)
        {
            //Set Task event.
            zmos_setTaskEvent(srchTimer->taskHandle, srchTimer->event);
            //Reload time value.
            srchTimer->timeout = srchTimer->reloadTime;
                
        }
        if(srchTimer->timeout == 0 || srchTimer->event == 0)
        {
            if(prevTimer)
            {
                prevTimer->next = srchTimer->next;
            }
            else
            {
                timerListHead = timerListHead->next;
            }
            
            freeTimer = srchTimer;
            srchTimer = srchTimer->next;
        }
        else
        {
            prevTimer = srchTimer;
            srchTimer = srchTimer->next;
        }
        
        if(freeTimer)
        {
            zmos_free(freeTimer);
        }
    }
}
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
zm_uint32_t zmos_getTimerClock(void)
{
    return zmos_timerClock;
}
/*****************************************************************
* FUNCTION: zmos_findTimer
*
* DESCRIPTION:
*     Find a timer in timer list
* INPUTS:
*     
* RETURNS:
*     
* NOTE:
*     null
*****************************************************************/
static zmos_timer_t *zmos_findTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event)
{
    zmos_timer_t *srchTimer = timerListHead;
    
    while(srchTimer)
    {
        if(srchTimer->taskHandle == pTaskHandle && 
           srchTimer->event == event)
        {
            break;
        }
        srchTimer = srchTimer->next;
    }
    return srchTimer;
}

/*****************************************************************
* FUNCTION: zmos_addTimer
*
* DESCRIPTION:
*     Add a timer to timer list.
* INPUTS:
*     pTaskHandle : Which task to set event.
*     event : What event to set.
*     timeout : Timer timeout.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
static zmos_timer_t *zmos_addTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, zm_uint32_t timeout)
{
    if(pTaskHandle)
    {
        zmos_timer_t *srchTimer = timerListHead;
        zmos_timer_t *prevTimer;
        zmos_timer_t *newTimer;
        
        while(srchTimer)
        {
            if(srchTimer->taskHandle == pTaskHandle && 
               srchTimer->event == event)
            {
                //The timer already exists - update time.
                srchTimer->timeout = timeout;
                srchTimer->reloadTime = 0;
                return srchTimer;
            }
            prevTimer = srchTimer;
            srchTimer = srchTimer->next;
        }
        //new timer
        newTimer = (zmos_timer_t *)zmos_malloc(sizeof(zmos_timer_t));
        
        if(newTimer)
        {
            newTimer->taskHandle = pTaskHandle;
            newTimer->event = event;
            newTimer->timeout = timeout;
            newTimer->reloadTime = 0;
            newTimer->next = NULL;
            
            if(timerListHead)
            {
                prevTimer->next = newTimer;
            }
            else timerListHead = newTimer;
            
            return newTimer;
        }
    }
    return NULL;
}

/*****************************************************************
* FUNCTION: zmos_deleteTimer
*
* DESCRIPTION:
*     Delete a timer.
* INPUTS:
*     pTimer : timer.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
static void zmos_deleteTimer(zmos_timer_t *pTimer)
{
    if(pTimer)
    {
        //Clear event.
        pTimer->event = 0;
    }
}
/****************************************************** END OF FILE ******************************************************/
