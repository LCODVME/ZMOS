/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Tasks.c
*
* DESCRIPTION:
*     ZMOS task driver
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
#include "ZMOS_Tasks.h"
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
/**
 * Task link list.
 */
typedef struct zmosTaskList_T
{
    zmos_task_t taskHandle;
    struct zmosTaskList_T *next;
}zmosTaskList_t;
/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
/* Task list head */
static zmosTaskList_t *taskListHead = NULL;
/* Index of active task */
static zmos_taskHandle_t activeTask = NULL;
/* Idle task function */
static idleTaskFunc zmosIdleTaskFunc = NULL;
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
static zmos_taskHandle_t zmos_getReadyTask(void);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/

/*****************************************************************
* FUNCTION: zmos_taskThreadRegister
*
* DESCRIPTION:
*     Register task thread to ZMOS.
* INPUTS:
*     pTaskHandle : The handle of the task.
*     taskFunc : Task function.
* RETURNS:
*     0 : Success (ZMOS_TASK_SUCCESS).
*     other : ref ZMOS task return cordes.
* NOTE:
*     It supports a maximum of 255 tasks.
*****************************************************************/
taskReslt_t zmos_taskThreadRegister(zmos_taskHandle_t * const pTaskHandle, taskFunction_t taskFunc)
{
    if(!taskFunc) return ZMOS_TASK_ERROR_PARAM;
    
    zmosTaskList_t *newTask;
    zmosTaskList_t *srchTask;
    zmosTaskList_t *prevTask;
    
    srchTask = taskListHead;
    
    while(srchTask)
    {
        //Whether task is registered.
        if(srchTask->taskHandle.taskFunc == taskFunc)
        {
            if(pTaskHandle != NULL)
            {
                *pTaskHandle = &srchTask->taskHandle;
            }
            return ZMOS_TASK_SUCCESS;
        }
        prevTask = srchTask;
        srchTask = srchTask->next;
    }
    
    newTask = (zmosTaskList_t *)zmos_mem_malloc(sizeof(zmosTaskList_t));
    if(newTask)
    {
        newTask->next = NULL;
        newTask->taskHandle.event = 0;
        newTask->taskHandle.taskFunc = taskFunc;
        
        /* Add to the linked list */
        if(taskListHead)
        {
            prevTask->next = newTask;
        }
        else taskListHead = newTask;
        
        if(pTaskHandle != NULL)
        {
            *pTaskHandle = &newTask->taskHandle;
        }
        
        return ZMOS_TASK_SUCCESS;
    }
    return ZMOS_TASK_FAILD;
}
/*****************************************************************
* FUNCTION: zmos_taskThreadUnregister
*
* DESCRIPTION:
*     ZMOS unregister the task.
* INPUTS:
*     pTaskHandle : The handle of the task to be deleted.
* RETURNS:
*     null
* NOTE:
*    Passing NULL will cause the calling task to be deleted.
*****************************************************************/
void zmos_taskThreadUnregister(zmos_taskHandle_t pTaskHandle)
{
    zmosTaskList_t *srchTask;
    zmosTaskList_t *prevTask;
    zmos_taskHandle_t pDelTask = pTaskHandle;
    
    if(pDelTask == NULL)
    {
        pDelTask = activeTask;
    }
    
    srchTask = taskListHead;
    
    while(srchTask)
    {
        if(&srchTask->taskHandle == pDelTask)
        {
            break;
        }
        prevTask = srchTask;
        srchTask = srchTask->next;
    }
    if(srchTask)
    {
        if(srchTask == taskListHead)
        {
            taskListHead = taskListHead->next;
        }
        else
        {
            prevTask->next = srchTask->next;
        }
        zmos_mem_free(srchTask);
    }
}

/*****************************************************************
* FUNCTION: zmos_setTaskEvent
*
* DESCRIPTION:
*     This function to set task event.
* INPUTS:
*     pTaskHandle : The handle of the task to set event.
*     events : what event to set.
* RETURNS:
*     0 : Success (ZMOS_TASK_SUCCESS).
*     other : ref ZMOS task return cordes.
* NOTE:
*     null
*****************************************************************/
taskReslt_t zmos_setTaskEvent(zmos_taskHandle_t pTaskHandle, uTaskEvent_t events)
{
    if(pTaskHandle)
    {
        ZMOS_ENTER_CRITICAL();
        pTaskHandle->event |= events;
        ZMOS_EXIT_CRITICAL();
        return ZMOS_TASK_SUCCESS;
    }
    return ZMOS_TASK_ERROR_PARAM;
}

/*****************************************************************
* FUNCTION: zmos_clearTaskEvent
*
* DESCRIPTION:
*     This function to clear task event.
* INPUTS:
*     pTaskHandle : The handle of the task to clear event.
*     events : what event to clear.
* RETURNS:
*     0 : Success (ZMOS_TASK_SUCCESS).
*     other : ref ZMOS task return cordes.
* NOTE:
*     null
*****************************************************************/
taskReslt_t zmos_clearTaskEvent(zmos_taskHandle_t pTaskHandle, uTaskEvent_t events)
{
    if(pTaskHandle)
    {
        ZMOS_ENTER_CRITICAL();
        pTaskHandle->event &= ~events;
        ZMOS_EXIT_CRITICAL();
        return ZMOS_TASK_SUCCESS;
    }
    return ZMOS_TASK_ERROR_PARAM;
}
/*****************************************************************
* FUNCTION: zmos_setIdleTaskFunction
*
* DESCRIPTION:
*     This function to set the idle task function.
* INPUTS:
*     func : idle task function.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_setIdleTaskFunction(idleTaskFunc func)
{
    zmosIdleTaskFunc  = func;
}
/*****************************************************************
* FUNCTION: zmos_getCurrentTaskHandle
*
* DESCRIPTION:
*     This function to gets the handle to the current task.
* INPUTS:
*     null
* RETURNS:
*     Current task handle.
*     NULL : No tasks are currently running.
* NOTE:
*     null
*****************************************************************/
zmos_taskHandle_t zmos_getCurrentTaskHandle(void)
{
    return activeTask;
}
/*****************************************************************
* FUNCTION: zmos_taskStartScheduler
*
* DESCRIPTION:
*     This function performs task scheduling.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     Shouldn't be called from anywhere else.
*****************************************************************/
void zmos_taskStartScheduler(void)
{
    zmos_taskHandle_t pNextTask;
        
    pNextTask = zmos_getReadyTask();
    
    if(pNextTask)
    {
        uTaskEvent_t events;
        
        ZMOS_ENTER_CRITICAL();
        events = pNextTask->event;
        pNextTask->event = 0;
        ZMOS_EXIT_CRITICAL();
        
        activeTask = pNextTask;
        events = pNextTask->taskFunc(events);
        activeTask = NULL;
        
        ZMOS_ENTER_CRITICAL();
        pNextTask->event |= events;
        ZMOS_EXIT_CRITICAL();
    }
    else
    {
        if(zmosIdleTaskFunc) zmosIdleTaskFunc();
    }
}

/*****************************************************************
* FUNCTION: zmos_getReadyTask
*
* DESCRIPTION:
*     ZMOS Get the registered ready task.
* INPUTS:
*     null
* RETURNS:
*     Task handle.
* NOTE:
*     null
*****************************************************************/
static zmos_taskHandle_t zmos_getReadyTask(void)
{
    zmosTaskList_t *srchTask;
    zmos_task_t *task = NULL;
    
    srchTask = taskListHead;
    
    while(srchTask)
    {
        if(srchTask->taskHandle.event)
        {
            task = &srchTask->taskHandle;
            break;
        }
        srchTask = srchTask->next;
    }
    return (zmos_taskHandle_t)task;
}

/****************************************************** END OF FILE ******************************************************/
