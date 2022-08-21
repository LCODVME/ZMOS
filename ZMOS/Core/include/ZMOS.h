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
#define ZMOS_INIT_SECTION_NAME      zmos_func_init    
    
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
 * @param[in] funcInit : The function to register(@ref zmos_funcInit).
 */
#define ZMOS_FUNC_INIT_REGISTER(funcInit) \
        ZM_SECTION_ITEM_REGISTER(ZMOS_INIT_SECTION_NAME, \
                                 zmos_funcInit const CONCAT_3(zmos_, funcInit, _fn)) = funcInit
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
typedef void (* zmos_funcInit)(void);
#endif
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: ZNOS_INIT_REGISTER
*
* DESCRIPTION:
*     Register ZMOS init functuion.
* INPUTS:
*     funcInit : The function to register(@ref zmos_funcInit).
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
#if (defined ZMOS_INIT_SECTION) && (ZMOS_INIT_SECTION)
#define ZNOS_INIT_REGISTER(funcInit)    ZMOS_FUNC_INIT_REGISTER(funcInit)
#else
#define ZNOS_INIT_REGISTER(funcInit) 
#endif
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

#ifdef __cplusplus
}
#endif
#endif /* ZMOS.h */
