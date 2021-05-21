/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* bsp_clock.h
*
* DESCRIPTION:
*     Provide clock tick for ZMOS.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/5/21
* REVISION:
*     v0.1
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*
*****************************************************************/
#ifndef __BSP_CLOCK_H__
#define __BSP_CLOCK_H__
 
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
 
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: bsp_getClockCount
*
* DESCRIPTION:
*     Get clock count, it provide system clock for ZMOS.
* INPUTS:
*     null
* RETURNS:
*     Clock count.
* NOTE:
*     null
*****************************************************************/
uint32_t bsp_getClockCount(void);

#ifdef __cplusplus
}
#endif
#endif /* bsp_clock.h */
