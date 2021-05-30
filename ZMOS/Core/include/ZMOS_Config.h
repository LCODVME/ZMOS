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
 * @brief types lib select.
 */
#ifndef ZMOS_TYPES_USE_CLIB
#define ZMOS_TYPES_USE_CLIB         1
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
 * @brief ZMOS callback timer use enable.
 *        1 : enable
 *        0 : disable
 *
 * @note The maximum number of callback timers that can be 
 *       created is ZMOS_TASK_EVENT_NUM_MAX.
 */
#ifndef ZMOS_USE_CBTIMERS
#define ZMOS_USE_CBTIMERS           1
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
