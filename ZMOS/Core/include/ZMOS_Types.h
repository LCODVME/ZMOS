/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Types.h
*
* DESCRIPTION:
*     ZMOS types define
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
#ifndef __ZMOS_TYPES_H__
#define __ZMOS_TYPES_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS_Common.h"
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
    
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
#if ZMOS_TYPES_USE_CLIB
    
#include <stdint.h>
#include <stdbool.h>
    
#else

typedef signed char int8_t;         //!< Signed 8 bit integer
typedef unsigned char uint8_t;      //!< Unsigned 8 bit integer

typedef signed short int16_t;       //!< Signed 16 bit integer
typedef unsigned short uint16_t;    //!< Unsigned 16 bit integer

typedef signed int int32_t;         //!< Signed 32 bit integer
typedef unsigned int uint32_t;      //!< Unsigned 32 bit integer

typedef unsigned char bool;         //!< Boolean data type

#endif

typedef int8_t      zm_int8_t;
typedef uint8_t     zm_uint8_t;

typedef int16_t     zm_int16_t;
typedef uint16_t    zm_uint16_t;

typedef int32_t     zm_int32_t;
typedef uint32_t    zm_uint32_t;

typedef zm_uint32_t zm_size_t;

/**
 * Task event types.
 */
#if (ZMOS_TASK_EVENT_NUM_MAX <= 8)
    typedef uint8_t uTaskEvent_t;
#elif (ZMOS_TASK_EVENT_NUM_MAX <= 16)
    typedef uint16_t uTaskEvent_t;
#else
    typedef uint32_t uTaskEvent_t;
#endif
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
#ifdef __cplusplus
}
#endif
#endif /* ZMOS_Types.h */
