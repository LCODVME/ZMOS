/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Common.h
*
* DESCRIPTION:
*     ZMOS common define.
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
#ifndef __ZMOS_COMMON_H__
#define __ZMOS_COMMON_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS_Config.h"
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/

/**
 * ZMOS null define.
 */
#ifndef NULL
#define NULL   0
#endif
    
/**
 * ZMOS Bit set.
 */
#define ZMOS_BS(n)    (1 << (n))
/**
 * ZMOS Bit clear.
 */
#define ZMOS_BC(n)    ~(1 << (n))
/**
 * ZMOS Value left shift.
 */
#define ZMOS_VLS(val, n) ((val) << (n))
 /**
 * ZMOS Value right shift.
 */
#define ZMOS_VRS(val, n) ((val) >> (n))
    
/**
 * Bit set.
 */
#ifndef BS
#define BS(n)   ZMOS_BS(n)
#endif
    
/**
 * Bit clear.
 */
#ifndef BC
#define BC(n)   ZMOS_BC(n)
#endif

/**
 * ZMOS Value plus one, and hold the 8-bit maximum.
 */
#define ZMOS_U8_MAX_HOLD(val)         ((val) = (val) == 0xFF ? 0xFF : ((val) + 1))
/**
 * ZMOS Value plus one, and hold the 16-bit maximum.
 */
#define ZMOS_U16_MAX_HOLD(val)        ((val) = (val) == 0xFFFF ? 0xFFFF : ((val) + 1))
/**
 * ZMOS Value plus one, and hold the 32-bit maximum.
 */
#define ZMOS_U32_MAX_HOLD(val)        ((val) = (val) == 0xFFFFFFFF ? 0xFFFFFFFF : ((val) + 1))
/**
 * ZMOS Value plus one, and hold the value no more than param max.
 */
#define ZMOS_MAX_VAL_HOLD(val, max)   ((val) = (val) == (max) ? (max) : ((val) + 1))

/**
 * ZMOS Get the maximum from a and b.
 */
#define ZMOS_GET_MAX(a, b)            ((a) > (b) ? (a) : (b))
/**
 * ZMOS Get the minimin form a and b.
 */
#define ZMOS_GET_MIN(a, b)            ((a) < (b) ? (a) : (b))
    
/**
 * ZMOS general return codes.
 */
//#define ZMOS_
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
#endif /* ZMOS_Common.h */
