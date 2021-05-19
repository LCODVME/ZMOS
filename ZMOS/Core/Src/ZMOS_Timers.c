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
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
typedef struct
{
    uint32_t timeout;
    uint32_t reloadTime;
    zmos_taskHandle_t taskHandle;
    uTaskEvent_t event;
    void *next;
}zmos_timer_t;
/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
/* Timer Clock */
static uint32_t zmos_timerClock;
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
static zmos_timer_t *zmos_addTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, uint32_t timeout);
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
timerReslt_t zmos_startSingleTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, uint32_t timeout)
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
timerReslt_t zmos_startReloadTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, uint32_t timeout)
{
    zmos_timer_t *newTimer;
    
    ZMOS_ENTER_CRITICAL();
    newTimer = zmos_addTimer(pTaskHandle, event, timeout);
    if(newTimer)
    {
        newTimer->reloadTime = timeout;
        return ZMOS_TIMER_SUCCESS;
    }
    ZMOS_EXIT_CRITICAL();
    return ZMOS_TIMER_FAILD;
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
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
static zmos_timer_t *zmos_addTimer(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event, uint32_t timeout)
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
                return srchTimer;
            }
            prevTimer = srchTimer;
            srchTimer = srchTimer->next;
        }
        //new timer
        newTimer = (zmos_timer_t *)zmos_mem_malloc(sizeof(zmos_timer_t));
        
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
