/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS.h
*
* DESCRIPTION:
*     ZMOS
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
#ifndef __ZMOS_H__
#define __ZMOS_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS_Common.h"
#include "ZMOS_Timers.h"
#include "ZMOS_Cbtimer.h"
#include "ZMOS_Tasks.h"
#include "ZMOS_LowPwr.h"
#include "ZMOS_Memory.h"
#if (defined ZMOS_INIT_SECTION) && (ZMOS_INIT_SECTION)
#include "ZMOS_Section.h"
#endif
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
#define ZMOS_PRIMAIRE_VERSION    1
#define ZMOS_SUB_VERSION         2
#define ZMOS_REVISED_VERSION     0
#define VERSION_STRING          STRINGIFY(ZMOS_PRIMAIRE_VERSION.ZMOS_SUB_VERSION.ZMOS_REVISED_VERSION)
    
#if (defined ZMOS_INIT_SECTION) && (ZMOS_INIT_SECTION)

/**
 * @brief ZMOS init section name.
 */
#define ZMOS_INIT_SECTION_NAME          zmos_func_init
#define ZMOS_BOARD_SECTION_NAME         CONCAT_2(ZMOS_INIT_SECTION_NAME, 1)
#define ZMOS_SYS_SECTION_NAME           CONCAT_2(ZMOS_INIT_SECTION_NAME, 2)
#define ZMOS_COMPONENT_SECTION_NAME     CONCAT_2(ZMOS_INIT_SECTION_NAME, 3)
#define ZMOS_APP_SECTION_NAME           CONCAT_2(ZMOS_INIT_SECTION_NAME, 4)
    
/**
 * @brief   Macro for creating a ZMOS init section.
 *          Auxiliary macro used by @ref ZNOS_INIT_REGISTER.
 *
 * @param[in]   section_name    Name of the section.
 * @param[in]   data_type       Data type of the variables to be registered in the section.
 */
#define ZMOS_INIT_SECTION_DEF(section_name, data_type) \
        ZM_SECTION_DEF(section_name, data_type)
    
/**
 * @brief Register ZMOS init functuion.
 *
 * @param[in] funcInit : The function to register(@ref zmos_funcInit_t).
 */
#define ZMOS_FUNC_INIT_REGISTER(funcInit) \
        ZM_SECTION_ITEM_REGISTER(ZMOS_INIT_SECTION_NAME, \
                                 zmos_funcInit_t const CONCAT_3(zmos_, funcInit, _fn)) = funcInit
        
/**
 * @brief Register ZMOS init functuion with level.
 *
 * @param[in] funcInit : The function to register(@ref zmos_funcInit_t).
 * @param[in] section_name : Register which section.
 */
#define ZMOS_FUNC_INIT_REGISTER_SECTION(funcInit, section_name) \
        ZM_SECTION_ITEM_REGISTER(section_name, \
                                 zmos_funcInit_t const CONCAT_3(CONCAT_2(zmos_, level), funcInit, _fn)) = funcInit
            
/**
 * @brief Register ZMOS init functuion with level.
 *
 * @param[in] funcInit : The function to register(@ref zmos_funcInit_t).
 * @param[in] level : Init level.
 * @param[in] section_name : Register which section.
 */
#define ZMOS_FUNC_INIT_LEVEL_REGISTER(funcInit, level, section_name) \
        ZM_SECTION_ITEM_REGISTER(section_name, \
                                 zmos_funcInit_t const CONCAT_3(CONCAT_2(zmos_, level), funcInit, _fn)) = funcInit
#else
#define ZMOS_FUNC_INIT_REGISTER(funcInit)
#define ZMOS_FUNC_INIT_REGISTER_SECTION(funcInit, section_name) 
#define ZMOS_FUNC_INIT_LEVEL_REGISTER(funcInit, level) 
#endif
    
    
#define ZMOS_ENTER_CRITICAL()   zmos_sysEnterCritical()
#define ZMOS_EXIT_CRITICAL()    zmos_sysExitCritical()
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
#if (defined ZMOS_INIT_SECTION) && (ZMOS_INIT_SECTION)
typedef void (* zmos_funcInit_t)(void);
#endif
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
     
/*********************************** ZMOS system interface ***************************************************************/

/*****************************************************************
* FUNCTION: ZNOS_INIT_REGISTER
*
* DESCRIPTION:
*     Register ZMOS init functuion.
* INPUTS:
*     funcInit : The function to register(@ref zmos_funcInit_t).
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
// Register init at board
#define ZMOS_INIT_BOARD_REGISTER(funcInit)      ZMOS_FUNC_INIT_REGISTER_SECTION(funcInit, ZMOS_BOARD_SECTION_NAME)
// Register init at system
#define ZMOS_INIT_SYS_REGISTER(funcInit)        ZMOS_FUNC_INIT_REGISTER_SECTION(funcInit, ZMOS_SYS_SECTION_NAME)
// Register init at component
#define ZMOS_INIT_COMPONENT_REGISTER(funcInit)  ZMOS_FUNC_INIT_REGISTER_SECTION(funcInit, ZMOS_COMPONENT_SECTION_NAME)
// Register init at application
#define ZMOS_INIT_APP_REGISTER(funcInit)        ZMOS_FUNC_INIT_REGISTER_SECTION(funcInit, ZMOS_APP_SECTION_NAME)
/*****************************************************************
* FUNCTION: ZNOS_INIT_REGISTER
*
* DESCRIPTION:
*     Register ZMOS init functuion.
* INPUTS:
*     funcInit : The function to register(@ref zmos_funcInit_t).
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
#define ZNOS_INIT_REGISTER(funcInit)    ZMOS_FUNC_INIT_REGISTER(funcInit)
/*****************************************************************
* FUNCTION: zmos_sysEnterCritical
*
* DESCRIPTION:
*     System enter critical.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_sysEnterCritical(void);
/*****************************************************************
* FUNCTION: zmos_sysExitCritical
*
* DESCRIPTION:
*     System exit critical.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_sysExitCritical(void);
/*****************************************************************
* FUNCTION: zmos_system_init
*
* DESCRIPTION:
*     ZMOS system initialize.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_system_init(void);
/*****************************************************************
* FUNCTION: zmos_system_start
*
* DESCRIPTION:
*     ZMOS system run start
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     This function is the main loop function of the task system. 
*     This Function doesn't return.
*****************************************************************/
void zmos_system_start(void);
/*****************************************************************
* FUNCTION: zmos_system_run
*
* DESCRIPTION:
*     This function is used to schedule once tasks.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_system_run(void);

/*********************************** ZMOS task interface ***************************************************************/

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
uint8_t zmos_checkTaskIsIdle(void);


/*********************************** ZMOS timer interface ***************************************************************/

/*****************************************************************
* FUNCTION: zmos_startSingleTimer
*
* DESCRIPTION:
*     This function is called to start a single timer.
* INPUTS:
*     pTaskHandle : Which task to set event.
*     event : What event to set.
*     timeout : Timer timeout.
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
*     timeout : Timer timeout.
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
* FUNCTION: zmos_getReloadTime
*
* DESCRIPTION:
*     Get timer reload time.
* INPUTS:
*     pTaskHandle : task handle of timer to check.
*     event : task event of timer to check.
* RETURNS:
*     Timer reload time.
* NOTE:
*     null
*****************************************************************/
uint32_t zmos_getReloadTimeout(zmos_taskHandle_t pTaskHandle, uTaskEvent_t event);
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


/*********************************** ZMOS cbtimer interface ***************************************************************/

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
timerReslt_t zmos_startSingleCbtimer(cbTimerId_t *timerId, uint32_t timeout, void *param, cbTimerFunction cbfunc);
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
timerReslt_t zmos_startReloadCbtimer(cbTimerId_t *timerId, uint32_t timeout, void *param, cbTimerFunction cbfunc);
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
timerReslt_t zmos_changeCbTimerTimeout(cbTimerId_t timerId, uint32_t timeout);
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
timerReslt_t zmos_stopCbtimer(cbTimerId_t timerId);


/*********************************** ZMOS memory interface ***************************************************************/

/*****************************************************************
* FUNCTION: zmos_malloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
* INPUTS:
*     size : The number of bytes to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     null
*****************************************************************/
void *zmos_malloc(zm_size_t size);
/*****************************************************************
* FUNCTION: zmos_realloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
*     This function will change the previously allocated memory block.
* INPUTS:
*     ptr : pointer to memory allocated by zmos_mem_malloc.
*     newsize : The number of new size to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     null
*****************************************************************/
void *zmos_realloc(void *ptr, zm_size_t newsize);
/*****************************************************************
* FUNCTION: zmos_mem_calloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
*     This function will contiguously allocate enough space for count objects
*     that are size bytes of memory each and returns a pointer to the allocated.
* memory.
* INPUTS:
*     count : Number of objects to allocate.
*     size : The number of size to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     null
*****************************************************************/
void *zmos_calloc(zm_size_t count, zm_size_t size);
/*****************************************************************
* FUNCTION: zmos_free
*
* DESCRIPTION: 
*       ZMOS dynamic memory de-allocation.
* INPUTS:
*     ptr : The first address assigned by zmos_malloc().
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_free(void *ptr);
/*****************************************************************
* FUNCTION: zmos_getMemTotal
*
* DESCRIPTION: 
*       Get zmos memory management total size.
* INPUTS:
*     null
* RETURNS:
*     memory total size.
* NOTE:
*     null
*****************************************************************/
zm_size_t zmos_getMemTotal(void);
/*****************************************************************
* FUNCTION: zmos_getMemUsed
*
* DESCRIPTION: 
*       Get zmos memory management used size.
* INPUTS:
*     null
* RETURNS:
*     memory used size.
* NOTE:
*     If no set ZMOS_MEM_STATS to 1, It always returns 0.
*****************************************************************/
zm_size_t zmos_getMemUsed(void);
/*****************************************************************
* FUNCTION: zmos_getMemMaxUsed
*
* DESCRIPTION: 
*       Get zmos memory management max used size.
* INPUTS:
*     null
* RETURNS:
*     memory max used size.
* NOTE:
*     If no set ZMOS_MEM_STATS to 1, It always returns 0.
*****************************************************************/
zm_size_t zmos_getMemMaxUsed(void);


/*********************************** ZMOS low  power interface ***************************************************************/

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
void zmos_lowPwrSetEvent(uint8_t event);
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
void zmos_lowPwrClearEvent(uint8_t event);






#ifdef __cplusplus
}
#endif
#endif /* ZMOS.h */
