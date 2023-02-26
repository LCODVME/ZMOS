/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* zm_key.c
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
 
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS.h"
#include "zm_drivers.h"
#include <string.h>
#include "zm_key.h"

#if ZM_KEY_MAX_NUM > 0
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
#if ZM_KEY_ENABLE_CUSTOM
#define ZM_KEY_POLL_PERIOD              zmKeyPollPeriod
#define ZM_KEY_DEBOUNCE_TIME(stu)       (stu->debnceTime)
#define ZM_KEY_SHORT_PRESS_TIME(stu)    (stu->shortIntvlTm)
#if ZM_KEY_USE_LONG_PRESS
#define ZM_KEY_LONG_PRESS_TIME(stu)     (stu->longPressTime)
#define ZM_KEY_LONG_HOLD_TIME(stu)      (stu->holdCbInterval)
#endif //ZM_KEY_USE_LONG_PRESS
#else
#define ZM_KEY_POLL_PERIOD              ZM_DEFAULT_POLL_TIME
#define ZM_KEY_DEBOUNCE_TIME(stu)       (ZM_DEFAULT_DEBOUNCE_TIME)
#define ZM_KEY_SHORT_PRESS_TIME(stu)    (ZM_DEFAULT_PRESS_SHORT_TIME)
#if ZM_KEY_USE_LONG_PRESS
#define ZM_KEY_LONG_PRESS_TIME(stu)     (ZM_DEFAULT_PRESS_LONG_TIME)
#define ZM_KEY_LONG_HOLD_TIME(stu)      (ZM_DEFAULT_PRESS_HOLD_TIME)
#endif //ZM_KEY_USE_LONG_PRESS
#endif //ZM_KEY_ENABLE_CUSTOM

#define ZM_KEY_CALLBACK(key, stu)       if(stu->funcCb) stu->funcCb(key, stu->eventStatus)
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
typedef struct
{
    uint8_t keyLevel : 1;
    uint8_t activeLevel : 1;
    uint8_t debnceSte : 1;
    uint8_t reserve : 5;
    uint8_t debnceRem;  //debounce remaining time.
#if ZM_KEY_ENABLE_CUSTOM
    uint8_t debnceTime;  //debounce time.
    uint16_t shortIntvlTm;  //short press, release interval max time.
#if ZM_KEY_USE_LONG_PRESS
    uint32_t longPressTime;
    uint16_t holdCbInterval;
#endif //ZM_KEY_USE_LONG_PRESS
#endif //ZM_KEY_ENABLE_CUSTOM
    uint32_t respEvent;
    uint32_t timeRecord;
    uint32_t next;
    zmKeyEventState_t eventStatus;
    zmKeyEventCb funcCb;
    readKeyLevelFunc readKeyLevel;
}zmKeyStatus_t;

/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
static uint8_t zmKeyRun = 1;
static zmKeyType_t keyReg = 0;
static zmKeyStatus_t keyStatusTable[ZM_KEY_MAX_NUM];
#if ZM_KEY_ENABLE_CUSTOM
static uint16_t zmKeyPollPeriod = ZM_DEFAULT_POLL_TIME;
#endif
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
static uint8_t zm_readKeyLevel(zmKeyType_t key);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
extern uint8_t zm_keyBoardReadLevel(zmKeyType_t key);
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: zm_keyInit
*
* DESCRIPTION:
*     key driver initialize.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     The user does not need to call this function.
*****************************************************************/
void zm_keyInit(void)
{
    memset(keyStatusTable, 0, sizeof(zmKeyStatus_t) * ZM_KEY_MAX_NUM);
    zm_keyPollStart();
}
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
void zm_keyRegister(zmKeyType_t keys, uint8_t keyActive, uint32_t respEvent, zmKeyEventCb keyCallback)
{
    if(keys >= BS(ZM_KEY_MAX_NUM)) return;
    zmKeyType_t key;
    zmKeyStatus_t *stu;
    
    key = ZM_KEY_1;
    stu = keyStatusTable;
    
    while(keys)
    {
        if(keys & key)
        {
            stu->activeLevel = keyActive;
            stu->keyLevel = ~keyActive;
            stu->debnceSte = 0;
#if ZM_KEY_ENABLE_CUSTOM
            stu->debnceTime = ZM_DEFAULT_DEBOUNCE_TIME;
            stu->shortIntvlTm = ZM_DEFAULT_PRESS_SHORT_TIME;
#if ZM_KEY_USE_LONG_PRESS
            stu->longPressTime = ZM_DEFAULT_PRESS_LONG_TIME;
            stu->holdCbInterval = ZM_DEFAULT_PRESS_HOLD_TIME;
#endif //ZM_KEY_USE_LONG_PRESS
#endif //ZM_KEY_ENABLE_CUSTOM
            stu->respEvent = respEvent;
            stu->timeRecord = zmos_getTimerClock();
            stu->funcCb = keyCallback;
            stu->readKeyLevel = NULL;
            memset(&stu->eventStatus, 0, sizeof(zmKeyEventState_t));
            keyReg |= key;
            keys ^= key;
            
            zm_driverSetEvent(ZM_DRIVER_KEY_POLL_EVENT);
        }
        key <<= 1;
        stu++;
    }
}
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
void zm_keyUnregister(zmKeyType_t keys)
{
    keyReg &= ~keys;
}
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
zmKeyEventState_t zm_getKeyStatus(zmKeyType_t key)
{
    zmKeyEventState_t keyStatus = {0};
    for(zmKeyType_t i = 0; i < ZM_KEY_MAX_NUM; i++)
    {
        if(key & BS(i))
        {
            return keyStatusTable[i].eventStatus;
        }
    }
    return keyStatus;
}
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
void zm_resetKeyStatus(zmKeyType_t keys)
{
    if(keys >= BS(ZM_KEY_MAX_NUM)) return;
    zmKeyType_t key;
    zmKeyStatus_t *stu;
    
    key = ZM_KEY_1;
    stu = keyStatusTable;
    
    while(keys)
    {
        if(keys & key)
        {
            stu->keyLevel = ~stu->activeLevel;
            stu->debnceSte = 0;
            stu->timeRecord = zmos_getTimerClock();
            memset(&stu->eventStatus, 0, sizeof(zmKeyEventState_t));
            keys ^= key;
        }
        key <<= 1;
        stu++;
    }
}
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
void zm_keyPollStart(void)
{
    if(zmKeyRun == 0)
    {
        zmKeyType_t key;
        zmKeyType_t keys;
        zmKeyStatus_t *stu;
        
        key = ZM_KEY_1;
        keys = keyReg;
        stu = keyStatusTable;
        
        while(keys)
        {
            if(keys & key)
            {
                stu->timeRecord = zmos_getTimerClock();
                memset(&stu->eventStatus, 0, sizeof(zmKeyEventState_t));
                keys ^= key;
            }
            key <<= 1;
            stu++;
        }
    }
    zmKeyRun = 1;
    zm_driverSetEvent(ZM_DRIVER_KEY_POLL_EVENT);
}
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
void zm_keyPollStop(void)
{
    zmKeyRun = 0;
    zm_driverStopTimerEvent(ZM_DRIVER_KEY_POLL_EVENT);
}
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
void zm_setKeyConfig(zmKeyType_t keys, zmKeyConfItem_t confItem, uint32_t val)
{
#if ZM_KEY_ENABLE_CUSTOM
    if(confItem == ZM_KEY_CONF_POLL_TIME)
    {
        zmKeyPollPeriod = (uint16_t )val;
        return;
    }
#endif
    if(keys >= BS(ZM_KEY_MAX_NUM)) return;
    zmKeyType_t key;
    zmKeyStatus_t *stu;
    
    key = ZM_KEY_1;
    stu = keyStatusTable;
    
    while(keys)
    {
        if(keys & key)
        {
            switch(confItem)
            {
            case ZM_KEY_CONF_SET_RESP_EVENT:
                stu->respEvent = (uint32_t )val;
                break;
            case ZM_KEY_CONF_SET_ACTIVE_LEVEL:
                stu->activeLevel = (uint8_t )val;
                break;
            case ZM_KEY_CONF_ADD_RESP_EVENT:
                stu->respEvent |= (uint32_t )val;
                break;
            case ZM_KEY_CONF_DEL_RESP_EVENT:
                stu->respEvent &= ~(uint32_t )val;
                break;
#if ZM_KEY_ENABLE_CUSTOM
            case ZM_KEY_CONF_DEBOUNCE_TIME:
                stu->debnceTime = (uint8_t )val;
                break;
            case ZM_KEY_CONF_SHORT_TIME:
                stu->shortIntvlTm = (uint16_t )val;
                break;
#if ZM_KEY_USE_LONG_PRESS
            case ZM_KEY_CONF_LONG_TIME:
                stu->longPressTime = (uint32_t )val;
                break;
            case ZM_KEY_CONF_HOLD_TIME:
                stu->holdCbInterval = (uint16_t )val;
                break;
#endif //ZM_KEY_USE_LONG_PRESS
#endif //ZM_KEY_ENABLE_CUSTOM
            default : return;
            }
            keys ^= key;
        }
        key <<= 1;
        stu++;
    }
}

/*****************************************************************
* FUNCTION: zm_setReadKeyLevelFunc
*
* DESCRIPTION:
*     This function to set the read key pin level function.
* INPUTS:
*     keys : Bit mask value of keys to set.
*     func :  The function to set.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_setReadKeyLevelFunc(zmKeyType_t keys, readKeyLevelFunc func)
{
    if(keys >= BS(ZM_KEY_MAX_NUM)) return;
    zmKeyType_t key;
    zmKeyStatus_t *stu;
    
    key = ZM_KEY_1;
    stu = keyStatusTable;
    
    while(keys)
    {
        if(keys & key)
        {
            stu->readKeyLevel = func;
            keys ^= key;
        }
        key <<= 1;
        stu++;
    }
}
/*****************************************************************
* FUNCTION: zm_keyPollProcess
*
* DESCRIPTION:
*     This function to poll key press.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     The user does not need to call this function.
*****************************************************************/
void zm_keyPollProcess(void)
{
    zmKeyType_t key;
    zmKeyType_t keys;
    zmKeyStatus_t *stu;
    
    key = ZM_KEY_1;
    keys = keyReg;
    stu = keyStatusTable;
    uint8_t keyLevelVal;
    uint32_t time;
    uint32_t wait;
    uint32_t next = ZM_KEY_POLL_PERIOD;
    
    while(keys)
    {
        if(keys & key)
        {
            wait = ZM_KEY_POLL_PERIOD;
            
            time = zmos_getTimerClock() - stu->timeRecord;
            stu->timeRecord = zmos_getTimerClock();
            
            if(stu->readKeyLevel) keyLevelVal = stu->readKeyLevel(key);
            else keyLevelVal = zm_readKeyLevel(key);
            
            if(keyLevelVal != stu->keyLevel)
            {
                if(stu->debnceSte)
                {
                    if(time >= stu->debnceRem)
                    {
                        stu->keyLevel = keyLevelVal;
                        stu->debnceRem = 0;
                        stu->debnceSte = 0;
                    }
                    else
                    {
                        stu->debnceRem -= time;
                        wait = ZMOS_GET_MIN(ZM_KEY_POLL_PERIOD, stu->debnceRem);
                    }
                }
                else
                {
                    stu->debnceSte = 1;
                    stu->debnceRem = ZM_KEY_DEBOUNCE_TIME(stu);
                    wait = ZMOS_GET_MIN(ZM_KEY_POLL_PERIOD, stu->debnceRem);
                }
            }
            else stu->debnceSte = 0;
            
            switch(stu->eventStatus.keyEvent)
            {
            case ZM_KEY_NONE_PRESS:
                if(stu->keyLevel != stu->activeLevel) break;
                else
                {
                    stu->eventStatus.keyEvent = ZM_KEY_PRESS_DOWN;
                    if(stu->respEvent & ZM_KEY_EVENT_PRESS_DOWN)
                    {
                        ZM_KEY_CALLBACK(key, stu);
                    }
#if ZM_KEY_USE_LONG_PRESS
                    stu->next = ZM_KEY_LONG_PRESS_TIME(stu);
                    wait = ZMOS_GET_MIN(wait, stu->next);
#endif
                }
                break;
            case ZM_KEY_PRESS_DOWN:
                if(stu->keyLevel == stu->activeLevel)
                {
#if ZM_KEY_USE_PRESS_DOWN_TIME_RECORD
                    stu->eventStatus.pressTime += time;
#endif //ZM_KEY_USE_PRESS_DOWN_TIME_RECORD
#if ZM_KEY_USE_LONG_PRESS
                    if(time >= stu->next)
                    {
#if ZM_KEY_USE_PRESS_DOWN_TIME_RECORD
                    stu->eventStatus.pressTime = 0;
#endif //ZM_KEY_USE_PRESS_DOWN_TIME_RECORD
                        stu->next = ZM_KEY_LONG_HOLD_TIME(stu);
                        stu->eventStatus.keyEvent = ZM_KEY_LONG_PRESS;
                        stu->eventStatus.status.pressLongTime = ZM_KEY_LONG_PRESS_TIME(stu);
                        if(stu->respEvent & ZM_KEY_EVENT_PRESS_LONG)
                        {
                            ZM_KEY_CALLBACK(key, stu);
                        }
                    }
                    else
                    {
                        stu->next -= time;
                    }
                    wait = ZMOS_GET_MIN(wait, stu->next);
#endif
                }
                else
                {
                    stu->next = ZM_KEY_SHORT_PRESS_TIME(stu);
                    stu->eventStatus.keyEvent = ZM_KEY_PRESS_UP;
                    if(stu->respEvent & ZM_KEY_EVENT_PRESS_UP)
                    {
                        ZM_KEY_CALLBACK(key, stu);
                    }
#if ZM_KEY_USE_PRESS_DOWN_TIME_RECORD
                    stu->eventStatus.pressTime = 0;
#endif
                    wait = ZMOS_GET_MIN(wait, stu->next);
                }
                break;
            case ZM_KEY_PRESS_UP:
                if(time >= stu->next || stu->keyLevel == stu->activeLevel)
                {
                    ZMOS_U8_MAX_HOLD(stu->eventStatus.status.pressNum);
                    
                    if(stu->keyLevel == stu->activeLevel)
                    {
                        stu->eventStatus.keyEvent = ZM_KEY_PRESS_DOWN;
#if ZM_KEY_USE_LONG_PRESS
                        stu->next = ZM_KEY_LONG_PRESS_TIME(stu);
                        wait = ZMOS_GET_MIN(wait, stu->next);
#endif
                    }
                    else
                    {
                        if(stu->respEvent & ZM_KEY_EVENT_SHORT_PRESS_EACH || 
                           stu->respEvent & ZMOS_VLS(ZM_KEY_EVENT_PRESS_DOWN, stu->eventStatus.status.pressNum))
                        {
                            stu->eventStatus.keyEvent = ZM_KEY_SHORT_PRESS;
                            ZM_KEY_CALLBACK(key, stu);
                        }
                        stu->eventStatus.keyEvent = ZM_KEY_NONE_PRESS;
                        stu->eventStatus.status.pressNum = 0;
                        stu->next = 0;
                    }
                }
                else
                {
                    stu->next -= time;
                    wait = ZMOS_GET_MIN(wait, stu->next);
                }
                break;
#if ZM_KEY_USE_LONG_PRESS
            case ZM_KEY_LONG_PRESS:
                stu->eventStatus.keyEvent = ZM_KEY_LONG_PRESS_HOLD;
            case ZM_KEY_LONG_PRESS_HOLD:
                stu->eventStatus.status.pressLongTime += time;
                if(stu->keyLevel == stu->activeLevel)
                {
                    if(time >= stu->next)
                    {
                        stu->next = ZM_KEY_LONG_HOLD_TIME(stu);
                        if(stu->respEvent & ZM_KEY_EVENT_PRESS_LONG_HOLD)
                        {
                            ZM_KEY_CALLBACK(key, stu);
                        }
                    }
                    else
                    {
                        stu->next -= time;
                    }
                    wait = ZMOS_GET_MIN(wait, stu->next);
                }
                else
                {
                    if(stu->respEvent & ZM_KEY_EVENT_PRESS_LONG_RELEASE)
                    {
                        stu->eventStatus.keyEvent = ZM_KEY_LONG_PRESS_RELEASE;
                        ZM_KEY_CALLBACK(key, stu);
                    }
                    stu->eventStatus.keyEvent = ZM_KEY_NONE_PRESS;
                    stu->eventStatus.status.pressLongTime = 0;
                    stu->next = 0;
                }
                break;
#endif
            }
            next = ZMOS_GET_MIN(wait, next);
            keys ^= key;
        }
        key <<= 1;
        stu++;
    }
    if(zmKeyRun && keyReg)
    {
        zm_driverSetTimerEvent(ZM_DRIVER_KEY_POLL_EVENT, next, false);
    }
}
/*****************************************************************
* FUNCTION: zm_readKeyLevel
*
* DESCRIPTION:
*     This function to read the key pin level status.
* INPUTS:
*     key : Which key to read.
* RETURNS:
*     The key level status.
* NOTE:
*     null
*****************************************************************/
static uint8_t zm_readKeyLevel(zmKeyType_t key)
{
    return zm_keyBoardReadLevel(key);
}

#else

#endif
/****************************************************** END OF FILE ******************************************************/
