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
#endif // (ZM_KEY_MAX_NUM > 8)
#endif // (ZM_KEY_MAX_NUM > 16)
    
/* ZM respond press events */
#define ZM_KEY_EVENT_SHORT_PRESS_EACH       BS(0)
#ifdef ZM_KEY_USE_LONG_PRESS
#define ZM_KEY_EVENT_PRESS_LONG             BS(1)
#define ZM_KEY_EVENT_PRESS_LONG_HOLD        BS(2)
#define ZM_KEY_EVENT_PRESS_LONG_RELEASE     BS(3)
#endif // ZM_KEY_USE_LONG_PRESS
#define ZM_KEY_EVENT_PRESS_UP               BS(8)
#define ZM_KEY_EVENT_PRESS_DOWN             BS(9)
#define ZM_KEY_EVENT_PRESS_1                BS(10)
#define ZM_KEY_EVENT_PRESS_2                BS(11)
#define ZM_KEY_EVENT_PRESS_3                BS(12)
#define ZM_KEY_EVENT_PRESS_4                BS(13)
#define ZM_KEY_EVENT_PRESS_5                BS(14)
#define ZM_KEY_EVENT_PRESS_6                BS(15)
#define ZM_KEY_EVENT_PRESS_7                BS(16)
#define ZM_KEY_EVENT_PRESS_8                BS(17)
#define ZM_KEY_EVENT_PRESS_9                BS(18)
#define ZM_KEY_EVENT_PRESS_10               BS(19)


    
#define ZM_DEFAULT_POLL_TIME            (10)  //ms
#define ZM_DEFAULT_DEBOUNCE_TICKS       (2)  //max : 63
#define ZM_DEFAULT_PRESS_SHORT_TICKS    (300 / ZM_DEFAULT_POLL_TIME)
#define ZM_DEFAULT_PRESS_LONG_TICKS     (3000 / ZM_DEFAULT_POLL_TIME)
#define ZM_DEFAULT_PRESS_HOLD_TICKS     (ZM_DEFAULT_POLL_TIME / ZM_DEFAULT_POLL_TIME)
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/

#if (ZM_KEY_MAX_NUM < 8)
typedef uint8_t zmKeyType_t;
#elif (ZM_KEY_MAX_NUM < 16)
typedef uint16_t zmKeyType_t;
#else
typedef uint32_t zmKeyType_t;
#endif
     
//typedef uint16_t zmKeEvent_t;
typedef enum
{
    ZM_KEY_NONE_PRESS = 0,
    ZM_KEY_PRESS_DOWN,
    ZM_KEY_PRESS_UP,
    ZM_KEY_SHORT_PRESS,
#ifdef ZM_KEY_USE_LONG_PRESS
    ZM_KEY_LONG_PRESS,
    ZM_KEY_LONG_PRESS_HOLD,
    ZM_KEY_LONG_PRESS_RELEASE,
#endif
}zmKeEvent_t;

typedef struct zmKeyEventState
{
    union
    {
        uint8_t pressNum;
#ifdef ZM_KEY_USE_LONG_PRESS
        uint32_t pressLongTime;
#endif
    }status;
    zmKeEvent_t keyEvent;
}zmKeyEventState_t;

typedef void (* zmKeyEventCb)(zmKeyType_t key, zmKeyEventState_t keyEventState);
     
typedef uint8_t (* readKeyLevelFunc)(zmKeyType_t key);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/

/*****************************************************************
* FUNCTION: zm_keyPollStart
*
* DESCRIPTION:
*     This function to start key poll.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_keyPollStart(void);

#ifdef __cplusplus
}
#endif
#endif /* zm_key.h */
