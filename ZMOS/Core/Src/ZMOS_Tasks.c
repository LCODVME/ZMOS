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
* FUNCTION: zmos_taskThreadRegister
*
* DESCRIPTION:
*     Register task thread to ZMOS.
* INPUTS:
*     pTaskHandle : The handle of the task.
*     taskFunc : Task function.
* RETURNS:
*     0 : Success.
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
* FUNCTION: zmos_taskThreadLogout
*
* DESCRIPTION:
*     ZMOS log out the task.
* INPUTS:
*     pTaskHandle : The handle of the task to be deleted.
* RETURNS:
*     null
* NOTE:
*    Passing NULL will cause the calling task to be deleted.
*****************************************************************/
void zmos_taskThreadLogout(zmos_taskHandle_t pTaskHandle)
{
    zmosTaskList_t *srchTask;
    zmosTaskList_t *prevTask;
    
    srchTask = taskListHead;
    
    while(srchTask)
    {
        if(&srchTask->taskHandle == pTaskHandle)
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
zmos_taskHandle_t zmos_getReadyTask(void)
{
    zmosTaskList_t *srchTask;
    
    srchTask = taskListHead;
    
    while(srchTask)
    {
        if(srchTask->taskHandle.event)
        {
            return &srchTask->taskHandle;
        }
        srchTask = srchTask->next;
    }
    return NULL;
}

/****************************************************** END OF FILE ******************************************************/
