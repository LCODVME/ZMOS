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
#define NULL   (void *)0
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
 * The upper 8 bits of a 32 bit value.
 */
#define ZMOS_MSB_32(a)                  (((a) & 0xFF000000) >> 24)
/**
 * The lower 8 bits (of a 32 bit value).
 */
#define ZMOS_LSB_32(a)                  ((a) & 0x000000FF)

/**
 * The upper 8 bits of a 16 bit value.
 */
#define ZMOS_MSB_16(a)                  (((a) & 0xFF00) >> 8)
/**
 * The lower 8 bits (of a 16 bit value).
 */
#define ZMOS_LSB_16(a)                  ((a) & 0x00FF)
    
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
 * Concatenates two parameters.
 */
#define CONCAT_2(p1, p2)      CONCAT_2_(p1, p2)
/** Auxiliary macro used by @ref CONCAT_2 */
#define CONCAT_2_(p1, p2)     p1##p2
/**
 * Concatenates three parameters.
 */
#define CONCAT_3(p1, p2, p3)  CONCAT_3_(p1, p2, p3)
/** Auxiliary macro used by @ref CONCAT_3 */
#define CONCAT_3_(p1, p2, p3) p1##p2##p3
/**
 * Converts a macro argument into a character constant.
 */   
#define STRINGIFY(val)  STRINGIFY_(val)
/** Auxiliary macro used by @ref STRINGIFY */
#define STRINGIFY_(val) #val    
    
/**
 * Counts number of elements inside the array.
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
    
#ifndef __LINT__

#if defined(__CC_ARM)
#define STATIC_ASSERT_SIMPLE(EXPR)      extern char (*_do_assert(void)) [sizeof(char[1 - 2*!(EXPR)])]
#define STATIC_ASSERT_MSG(EXPR, MSG)    extern char (*_do_assert(void)) [sizeof(char[1 - 2*!(EXPR)])]

#elif defined(__GNUC__)
#ifdef __cplusplus
#define STATIC_ASSERT_SIMPLE(EXPR)      extern char (*_do_assert(void)) [sizeof(char[1 - 2*!(EXPR)])]
#define STATIC_ASSERT_MSG(EXPR, MSG)    extern char (*_do_assert(void)) [sizeof(char[1 - 2*!(EXPR)])]
#else
#define STATIC_ASSERT_SIMPLE(EXPR)      _Static_assert(EXPR, "unspecified message")
#define STATIC_ASSERT_MSG(EXPR, MSG)    _Static_assert(EXPR, MSG)
#endif

#elif defined(__ICCARM__)
#define STATIC_ASSERT_SIMPLE(EXPR)      static_assert(EXPR, "unspecified message")
#define STATIC_ASSERT_MSG(EXPR, MSG)    static_assert(EXPR, MSG)
#else
#error "Not compatible??"
#endif

#else // __LINT__

#define STATIC_ASSERT_SIMPLE(EXPR)      extern char (*_ignore(void))
#define STATIC_ASSERT_MSG(EXPR, MSG)    extern char (*_ignore(void))

#endif
    

/**
 * @brief   Static (i.e. compile time) assert macro.
 *
 * @note The output of STATIC_ASSERT can be different across compilers.
 *
 * Usage:
 * STATIC_ASSERT(expression);
 * STATIC_ASSERT(expression, message);
 *
 * @hideinitializer
 */
//lint -save -esym(???, STATIC_ASSERT)
#define STATIC_ASSERT(...)                                                                          \
    _SELECT_ASSERT_FUNC(x, ##__VA_ARGS__,                                                           \
                        STATIC_ASSERT_MSG(__VA_ARGS__),                                             \
                        STATIC_ASSERT_SIMPLE(__VA_ARGS__))


#define _SELECT_ASSERT_FUNC(x, EXPR, MSG, ASSERT_MACRO, ...) ASSERT_MACRO
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
