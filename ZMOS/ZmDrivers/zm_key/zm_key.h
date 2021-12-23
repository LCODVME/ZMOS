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
#define ZM_KEY_EVENT_SHORT_PRESS_EACH       BS(0)  //Respond to each short press.
#if ZM_KEY_USE_LONG_PRESS
#define ZM_KEY_EVENT_PRESS_LONG             BS(1)  //Respond long press.
#define ZM_KEY_EVENT_PRESS_LONG_HOLD        BS(2)  //Respond long press hold.
#define ZM_KEY_EVENT_PRESS_LONG_RELEASE     BS(3)  //Respond long press to release.
#endif // ZM_KEY_USE_LONG_PRESS
#define ZM_KEY_EVENT_PRESS_UP               BS(8)  //Respond press down to up.
#define ZM_KEY_EVENT_PRESS_DOWN             BS(9)  //Respond press down.
#define ZM_KEY_EVENT_PRESS_1                BS(10) //Respond once short press.
#define ZM_KEY_EVENT_PRESS_2                BS(11) //Respond twice short press.
#define ZM_KEY_EVENT_PRESS_3                BS(12)
#define ZM_KEY_EVENT_PRESS_4                BS(13)
#define ZM_KEY_EVENT_PRESS_5                BS(14)
#define ZM_KEY_EVENT_PRESS_6                BS(15)
#define ZM_KEY_EVENT_PRESS_7                BS(16)
#define ZM_KEY_EVENT_PRESS_8                BS(17)
#define ZM_KEY_EVENT_PRESS_9                BS(18)
#define ZM_KEY_EVENT_PRESS_10               BS(19)


    
#define ZM_DEFAULT_POLL_TIME                (10)  //default poll time(1 ~ 65535 ms).
#define ZM_DEFAULT_DEBOUNCE_TIME            (20)  //default debounce time(0 ~ 255 ms).
#define ZM_DEFAULT_PRESS_SHORT_TIME         (300) //default short press interval time(1 ~ 65535 ms).
#define ZM_DEFAULT_PRESS_LONG_TIME          (2000) //default long press time(1 ~ 0xFFFFFFFF ms).
#define ZM_DEFAULT_PRESS_HOLD_TIME          (1000) //default long press hold respond interval(1 ~ 65535 ms).
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
/**
 * zm key type define.
 */
#if (ZM_KEY_MAX_NUM < 8)
typedef uint8_t zmKeyType_t;
#elif (ZM_KEY_MAX_NUM < 16)
typedef uint16_t zmKeyType_t;
#else
typedef uint32_t zmKeyType_t;
#endif
/**
 * zm key events enum.
 */
typedef enum
{
    ZM_KEY_NONE_PRESS = 0,
    ZM_KEY_PRESS_DOWN,
    ZM_KEY_PRESS_UP,
    ZM_KEY_SHORT_PRESS,
#if ZM_KEY_USE_LONG_PRESS
    ZM_KEY_LONG_PRESS,
    ZM_KEY_LONG_PRESS_HOLD,
    ZM_KEY_LONG_PRESS_RELEASE,
#endif
}zmKeyEvent_t;

/** 
 * zm custom config item.
 */
typedef enum
{
    ZM_KEY_CONF_SET_ACTIVE_LEVEL,
    ZM_KEY_CONF_SET_RESP_EVENT,
    ZM_KEY_CONF_ADD_RESP_EVENT,
    ZM_KEY_CONF_DEL_RESP_EVENT,
#if ZM_KEY_ENABLE_CUSTOM
    ZM_KEY_CONF_POLL_TIME,
    ZM_KEY_CONF_DEBOUNCE_TIME,
    ZM_KEY_CONF_SHORT_TIME,
#if ZM_KEY_USE_LONG_PRESS
    ZM_KEY_CONF_LONG_TIME,
    ZM_KEY_CONF_HOLD_TIME,
#endif //ZM_KEY_USE_LONG_PRESS
#endif //ZM_KEY_ENABLE_CUSTOM
}zmKeyConfItem_t;

/**
 * zm key status struct. 
 */
typedef struct zmKeyEventState
{
    union
    {
        uint8_t pressNum;
#if ZM_KEY_USE_LONG_PRESS
        uint32_t pressLongTime;
#endif
    }status;
    zmKeyEvent_t keyEvent;
#if ZM_KEY_USE_PRESS_DOWN_TIME_RECORD
    uint32_t pressTime;
#endif
}zmKeyEventState_t;
/**
 * zm key event repond call back.
 *
 * @param key : The key that triggers the response.
 * @param keyEventState : key status(@ref zmKeyEventState_t).
 */
typedef void (* zmKeyEventCb)(zmKeyType_t key, zmKeyEventState_t keyEventState);
/**
 * Read key pin level function.
 *
 * @param key : which key to read.
 *
 * @return the key pin level.
 *
 * @note : config by zm_setKeyConfig, confItem ZM_KEY_CONF_READ_LEVEL_FUNC.
 */
typedef uint8_t (* readKeyLevelFunc)(zmKeyType_t key);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: zm_keyRegister
*
* DESCRIPTION:
*     This function to register key.
*     When key was pressed, callback will be call.
* INPUTS:
*     keys : Bit mask value of keys to register.
*     keyActive : The level at which the key is pressed.
*                 0 : active low.
*                 1 : active high.
*     respEvent : The event to respond to and callback.
*                  Example : ZM_EVENT_PRESS_1 is Respond once by pressing.
*                            ZM_EVENT_PRESS_1 | ZM_EVENT_PRESS_2 is respond to
*                            one and two press downs.
*     keyCallback : Key callback function.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_keyRegister(zmKeyType_t keys, uint8_t keyActive, uint32_t respEvent, zmKeyEventCb keyCallback);
/*****************************************************************
* FUNCTION: zm_keyUnregister
*
* DESCRIPTION:
*     This function to unregister keys.
* INPUTS:
*     keys : Bit mask value of keys to unregister.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_keyUnregister(zmKeyType_t keys);
/*****************************************************************
* FUNCTION: zm_getKeyStatus
*
* DESCRIPTION:
*     This function to get key status.
* INPUTS:
*     key : The state of the key you want to get.
* RETURNS:
*     key status.
* NOTE:
*     All returns 0 if key does not exist.
*****************************************************************/
zmKeyEventState_t zm_getKeyStatus(zmKeyType_t key);
/*****************************************************************
* FUNCTION: zm_resetKeyStatus
*
* DESCRIPTION:
*     This function to reset the key status.
* INPUTS:
*     keys : Bit mask value of keys to reset.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_resetKeyStatus(zmKeyType_t keys);
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
/*****************************************************************
* FUNCTION: zm_keyPollStop
*
* DESCRIPTION:
*     This function to stop key poll.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_keyPollStop(void);
/*****************************************************************
* FUNCTION: zm_setKeyConfig
*
* DESCRIPTION:
*     This function to config key parameters.
* INPUTS:
*     keys : Bit mask value of keys to config.
*     confItem : Item to configure.
*     val :  value.
* RETURNS:
*     null
* NOTE:
*     If the confItem is ZM_KEY_CONF_POLL_TIME, 
*     the keys can be arbitrary.
*****************************************************************/
void zm_setKeyConfig(zmKeyType_t keys, zmKeyConfItem_t confItem, uint32_t val);
/*****************************************************************
* FUNCTION: zm_setReadKeyLevelFunc
*
* DESCRIPTION:
*     This function to set the read key pin level function.
* INPUTS:
*     keys : Bit mask value of keys to config.
*     func :  The function to set.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_setReadKeyLevelFunc(zmKeyType_t keys, readKeyLevelFunc func);

#ifdef __cplusplus
}
#endif
#endif /* zm_key.h */
