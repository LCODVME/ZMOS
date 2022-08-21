/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Config.h
*
* DESCRIPTION:
*     ZMOS config
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
#ifndef __ZMOS_CONFIG_H__
#define __ZMOS_CONFIG_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
/**
 * @brief ZMOS types lib select.
 *        1 : enable
 *        0 : disable
 */
#ifndef ZMOS_TYPES_USE_CLIB
#define ZMOS_TYPES_USE_CLIB         1
#endif
    
/**
 * @brief ZMOS use init section.
 *        1 : enable
 *        0 : disable
 */
#ifndef ZMOS_INIT_SECTION
#define ZMOS_INIT_SECTION           1
#endif
     
/**
 * @brief ZMOS use memory management.
 *        1 : enable
 *        0 : disable
 */
#ifndef ZMOS_USE_MEM_MGR
#define ZMOS_USE_MEM_MGR            1
#endif
     
#if ZMOS_USE_MEM_MGR
/**
 * @brief ZMOS memory management align size.
 * 
 * @note Preferably the same as the CPU.
 */
#ifndef ZMOS_ALIGN_SIZE
#define ZMOS_ALIGN_SIZE             4
#endif

/**
 * @brief ZMOS memory management pool size.
 * 
 */
#ifndef ZMOS_MEM_SIZE
#define ZMOS_MEM_SIZE               (8192)
#endif
/**
 * @brief Whether to enable memory statistics.
 *        1 : enable
 *        0 : disable
 *
 */
#ifndef ZMOS_MEM_STATS
#define ZMOS_MEM_STATS              1
#endif
     
#endif
     
/**
 * @brief ZMOS task maximum support event is the number.
 *        the value is 8¡¢16 or 32.
 *
 * @note No more than 32.
 */
#ifndef ZMOS_TASK_EVENT_NUM_MAX
#define ZMOS_TASK_EVENT_NUM_MAX     32
#endif
    
/**
 * @brief Number of ZMOS callback timers used.
 *        0 : disable.
 * 
 *
 * @note The maximum number of callback timers that can be 
 *       created is no more than ZMOS_TASK_EVENT_NUM_MAX.
 */
#ifndef ZMOS_USE_CBTIMERS_NUM
#define ZMOS_USE_CBTIMERS_NUM       8
#endif

/**
 * @brief ZMOS low power management use enable.
 *        1 : enable
 *        0 : disable
 *
 */
#ifndef ZMOS_USE_LOW_POWER
#define ZMOS_USE_LOW_POWER          0
#endif
/**
 * @brief ZMOS low power management whether to wait for the task to become idle.
 *        1 : enable
 *        0 : disable
 *
 */
#ifndef ZMOS_LPM_WAIT_IDLE
#define ZMOS_LPM_WAIT_IDLE          1
#endif
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
#ifdef __cplusplus
}
#endif
#endif /* ZMOS_Config.h */
