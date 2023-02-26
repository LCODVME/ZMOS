/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Tasks.h
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
#ifndef __ZMOS_TASKS_H__
#define __ZMOS_TASKS_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
    
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS_Types.h"
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
     
/* ZMOS task return cordes */
#define ZMOS_TASK_SUCCESS           0
#define ZMOS_TASK_FAILD             1
#define ZMOS_TASK_ERROR_PARAM       2
     
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/

/**
 * ZMOS task result type.
 * @ref ZMOS task return cordes.
 */
typedef zm_uint8_t taskReslt_t;
/**
 * Zmos task handle.
 */
typedef struct zmos_task*  zmos_taskHandle_t;
    
/**
 * Event handler function prototype.
 *
 * @param event : task event.
 */
typedef uTaskEvent_t (*taskFunction_t)(uTaskEvent_t event);
/**
 * ZMOS task struct.
 */
typedef struct zmos_task
{
    uTaskEvent_t event;
    taskFunction_t taskFunc;
}zmos_task_t;

/**
 * ZMOS idle task function.
 */
typedef void (* idleTaskFunc)(void);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
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
taskReslt_t zmos_taskThreadRegister(zmos_taskHandle_t * const pTaskHandle, taskFunction_t taskFunc);
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
taskReslt_t zmos_setTaskEvent(zmos_taskHandle_t pTaskHandle, uTaskEvent_t events);
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
taskReslt_t zmos_clearTaskEvent(zmos_taskHandle_t pTaskHandle, uTaskEvent_t events);
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
void zmos_setIdleTaskFunction(idleTaskFunc func);
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
zmos_taskHandle_t zmos_getCurrentTaskHandle(void);
/*****************************************************************
* FUNCTION: zmos_checkTaskIsIdle
*
* DESCRIPTION:
*     This function to check task is idle.
* INPUTS:
*     null
* RETURNS:
*     0:task idle
*     1:task busy
* NOTE:
*     null
*****************************************************************/
zm_uint8_t zmos_checkTaskIsIdle(void);


#ifdef __cplusplus
}
#endif
#endif /* ZMOS_Tasks.h */
