/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* zm_key.h
*
* DESCRIPTION:
*     zm key driver.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/6/7
* REVISION:
*     v0.1
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*
*****************************************************************/
#ifndef __ZM_KEY_H__
#define __ZM_KEY_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS_Common.h"
#include "zm_driverConfig.h"
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
/* ZM keys */
#define ZM_KEY_1   BS(0)
#define ZM_KEY_2   BS(1)
#define ZM_KEY_3   BS(2)
#define ZM_KEY_4   BS(3)
#define ZM_KEY_5   BS(4)
#define ZM_KEY_6   BS(5)
#define ZM_KEY_7   BS(6)
#define ZM_KEY_8   BS(7)
#if (ZM_KEY_MAX_NUM > 8)
#define ZM_KEY_9   BS(8)
#define ZM_KEY_10  BS(9)
#define ZM_KEY_11  BS(10)
#define ZM_KEY_12  BS(11)
#define ZM_KEY_13  BS(12)
#define ZM_KEY_14  BS(13)
#define ZM_KEY_15  BS(14)
#define ZM_KEY_16  BS(15)
#if (ZM_KEY_MAX_NUM > 16)
#define ZM_KEY_17  BS(16)
#define ZM_KEY_18  BS(17)
#define ZM_KEY_19  BS(18)
#define ZM_KEY_20  BS(19)
#define ZM_KEY_21  BS(20)
#define ZM_KEY_22  BS(21)
#define ZM_KEY_23  BS(22)
#define ZM_KEY_24  BS(23)
#define ZM_KEY_25  BS(24)
#define ZM_KEY_26  BS(25)
#define ZM_KEY_27  BS(26)
#define ZM_KEY_28  BS(27)
#define ZM_KEY_29  BS(28)
#define ZM_KEY_30  BS(29)
#define ZM_KEY_31  BS(30)
#define ZM_KEY_32  BS(31)
#endif //(ZM_KEY_MAX_NUM > 8)
#endif // (ZM_KEY_MAX_NUM > 16)
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/

#if (ZM_KEY_MAX_NUM <= 8)
typedef uint8_t zmKeyType_t;
#elif (ZM_KEY_MAX_NUM <= 16)
typedef uint16_t zmKeyType_t;
#else
typedef uint32_t zmKeyType_t;
#endif
     
/*typedef enum
{
    
}zm_*/ 

typedef enum
{
    E_KEY_NONE = 0,
    E_KEY_PRESS,
#ifdef ZM_KEY_LONG_PRESS
    E_KEY_LONG_PRESS,
    E_KEY_RELEASED
#endif
}zm_keyAction_t;

typedef struct
{
    union
    {
        uint8_t num;
        uint32_t time;
    }u;
    zm_keyAction_t keyAction;
}zmKeyEvent_t;

typedef void (* zmKeyEventCb)(zmKeyType_t key, zmKeyEvent_t keyEvent);
     

/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
#ifdef __cplusplus
}
#endif
#endif /* zm_key.h */
