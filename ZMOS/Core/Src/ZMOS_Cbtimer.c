/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Cbtimer.c
*
* DESCRIPTION:
*     Callback timer function.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/5/29
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
#include "ZMOS_Tasks.h"
#include "ZMOS_Timers.h"
#include "ZMOS_Cbtimer.h"
#include <string.h>

#if ZMOS_USE_CBTIMERS_NUM > 0

#if (ZMOS_USE_CBTIMERS_NUM > ZMOS_TASK_EVENT_NUM_MAX)
#error "ZMOS_USE_CBTIMERS_NUM cannot exceed ZMOS_TASK_EVENT_NUM_MAX!"
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
/**
 * Callback timer struct.
 */
typedef struct
{
    cbTimerFunction timerFunc;
    void *param;
}zmos_cbTimer_t;
/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
/* Callback timer task handle */
static zmos_taskHandle_t cbTimerTaskHandle;
/* Callback timer table */
static zmos_cbTimer_t cbTimers[ZMOS_USE_CBTIMERS_NUM];
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
static uTaskEvent_t zmos_cbTImerTaskProcess(uTaskEvent_t event);
static timerReslt_t zmos_addCbTimer(cbTimerId_t *timerId, void *param, cbTimerFunction cbfunc);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: zmos_cbTimerInit
*
* DESCRIPTION:
*     ZMOS callback timer initialize.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_cbTimerInit(void)
{
    memset(cbTimers, 0, ZMOS_USE_CBTIMERS_NUM * sizeof(zmos_cbTimer_t));
    zmos_taskThreadRegister(&cbTimerTaskHandle, zmos_cbTImerTaskProcess);
}
/*****************************************************************
* FUNCTION: zmos_startSingleCbtimer
*
* DESCRIPTION:
*     This function is called to start a single callback timer.
* INPUTS:
*     timerId : The callback timer id.
*     timeout : Timer timeout.
*     param : Param to be passed in to callback function.
*     cbfunc : Callback function.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_startSingleCbtimer(cbTimerId_t *timerId, uint32_t timeout, void *param, cbTimerFunction cbfunc)
{
    cbTimerId_t cTimerId;
    if(zmos_addCbTimer(&cTimerId, param, cbfunc) == ZMOS_TIMER_SUCCESS)
    {
        if(zmos_startSingleTimer(cbTimerTaskHandle, BS(cTimerId), timeout) == ZMOS_TIMER_SUCCESS)
        {
            if(timerId) *timerId = cTimerId;
            return ZMOS_TIMER_SUCCESS;
        }
        else
        {
            cbTimers[cTimerId].timerFunc = NULL;
            cbTimers[cTimerId].param = NULL;
        }
    }
    return ZMOS_TIMER_FAILD;
}
/*****************************************************************
* FUNCTION: zmos_startReloadCbtimer
*
* DESCRIPTION:
*     This function is called to start a reload callback timer.
* INPUTS:
*     timerId : The callback timer id.
*     timeout : Timer timeout.
*     param : Param to be passed in to callback function.
*     cbfunc : Callback function.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_startReloadCbtimer(cbTimerId_t *timerId, uint32_t timeout, void *param, cbTimerFunction cbfunc)
{
    cbTimerId_t cTimerId;
    if(zmos_addCbTimer(&cTimerId, param, cbfunc) == ZMOS_TIMER_SUCCESS)
    {
        if(zmos_startReloadTimer(cbTimerTaskHandle, BS(cTimerId), timeout) == ZMOS_TIMER_SUCCESS)
        {
            if(timerId) *timerId = cTimerId;
            return ZMOS_TIMER_SUCCESS;
        }
        else
        {
            cbTimers[cTimerId].timerFunc = NULL;
            cbTimers[cTimerId].param = NULL;
        }
    }
    return ZMOS_TIMER_FAILD;
}
/*****************************************************************
* FUNCTION: zmos_changeCbTimerTimeout
*
* DESCRIPTION:
*     This to change a callback timer timeout.
* INPUTS:
*     timerId : The callback timer id.
*     timeout : Timer new timeout.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_changeCbTimerTimeout(cbTimerId_t timerId, uint32_t timeout)
{
    if(timerId < ZMOS_USE_CBTIMERS_NUM)
    {
        if(cbTimers[timerId].timerFunc)
        {
            if(zmos_getReloadTimeout(cbTimerTaskHandle, BS(timerId)))
            {
                return zmos_startReloadTimer(cbTimerTaskHandle, BS(timerId), timeout);
            }
            else return zmos_startSingleTimer(cbTimerTaskHandle, BS(timerId), timeout);
        }
    }
    return ZMOS_TIMER_FAILD;
}
/*****************************************************************
* FUNCTION: zmos_stopCbtimer
*
* DESCRIPTION:
*     This function to stop a callback timer.
* INPUTS:
*     timerId : The callback timer id.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_stopCbtimer(cbTimerId_t timerId)
{
    if(timerId < ZMOS_USE_CBTIMERS_NUM)
    {
        if(cbTimers[timerId].timerFunc)
        {
            cbTimers[timerId].timerFunc = NULL;
            cbTimers[timerId].param = NULL;
            return zmos_stopTimer(cbTimerTaskHandle, BS(timerId));
        }
    }
    return ZMOS_TIMER_FAILD;
}
/*****************************************************************
* FUNCTION: zmos_cbTImerTaskProcess
*
* DESCRIPTION:
*     ZMOS callback timer task process.
* INPUTS:
*     event
* RETURNS:
*     event
* NOTE:
*     null
*****************************************************************/
static uTaskEvent_t zmos_cbTImerTaskProcess(uTaskEvent_t event)
{
    if(event)
    {
        for(uTaskEvent_t i = 0; i < ZMOS_USE_CBTIMERS_NUM && event; i++)
        {
            if(event & BS(i))
            {
                if(cbTimers[i].timerFunc)
                {
                    cbTimers[i].timerFunc(cbTimers[i].param);
                }
                // Check if reload timer.
                if(zmos_getReloadTimeout(cbTimerTaskHandle, BS(i)) == 0)
                {
                    cbTimers[i].timerFunc = NULL;
                    cbTimers[i].param = NULL;
                }
                event ^= BS(i);
            }
        }
    }
    return 0;
}

/*****************************************************************
* FUNCTION: zmos_addCbTimer
*
* DESCRIPTION:
*     ZMOS Add callback timer.
* INPUTS:
*     timerId : The callback timer id.
*     param : Param to be passed in to callback function.
*     cbfunc : Callback function.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
static timerReslt_t zmos_addCbTimer(cbTimerId_t *timerId, void *param, cbTimerFunction cbfunc)
{
    if(cbfunc == NULL) return ZMOS_TIMER_FAILD;
    
    for(cbTimerId_t i = 0; i < ZMOS_USE_CBTIMERS_NUM; i++)
    {
        if(cbTimers[i].timerFunc == NULL)
        {
            cbTimers[i].timerFunc = cbfunc;
            cbTimers[i].param = param;
            
            if(timerId)
            {
                *timerId = i;
            }
            return ZMOS_TIMER_SUCCESS;
        }
    }
    return ZMOS_TIMER_FAILD;
}

#else
void zmos_cbTimerInit(void) {}
timerReslt_t zmos_startSingleCbtimer(cbTimerId_t *timerId, uint32_t timeout, void *param, cbTimerFunction cbfunc) {return ZMOS_TIMER_FAILD;}
timerReslt_t zmos_startReloadCbtimer(cbTimerId_t *timerId, uint32_t timeout, void *param, cbTimerFunction cbfunc) {return ZMOS_TIMER_FAILD;}
timerReslt_t zmos_changeCbTimerTimeout(cbTimerId_t timerId, uint32_t timeout) {return ZMOS_TIMER_FAILD;}
timerReslt_t zmos_stopCbtimer(cbTimerId_t timerId) {return ZMOS_TIMER_FAILD;}
#endif
/****************************************************** END OF FILE ******************************************************/
