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
#define ZM_U8_MAX_HOLD(val)         ((val) = (val) == 0xFF ? 0xFF : ((val) + 1))
#define ZM_U16_MAX_HOLD(val)        ((val) = (val) == 0xFFFF ? 0xFFFF : ((val) + 1))
#define ZM_U32_MAX_HOLD(val)        ((val) = (val) == 0xFFFFFFFF ? 0xFFFFFFFF : ((val) + 1))

#define ZM_MAX_VAL_HOLD(val, max)   ((val) = (val) == (max) ? (max) : ((val) + 1))
#define ZM_GET_MAX(a, b)            ((a) > (b) ? (a) : (b))
#define ZM_GET_MIN(a, b)            ((a) < (b) ? (a) : (b))

#define ZM_VS(val, n) ((val) << (n))

#define ZM_KEY_DEBOUNCE_TIME(stu)       (stu->debounceTick)
#define ZM_KEY_SHORT_PRESS_TIME(stu)    (stu->shortPressTick)
#ifdef ZM_KEY_USE_LONG_PRESS
#define ZM_KEY_LONG_PRESS_TIME(stu)     (stu->longPressTick)
#define ZM_KEY_LONG_HOLD_TIME(stu)      (stu->holdCbInterval)
#endif //ZM_KEY_USE_LONG_PRESS

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
    uint8_t debounceTick : 6;
    uint8_t debounceCnt : 6;
    uint8_t reserve : 2;
    uint16_t shortPressTick;
#ifdef ZM_KEY_USE_LONG_PRESS
    uint16_t longPressTick;
    uint16_t holdCbInterval;
#endif
    uint16_t ticksRecord;
    uint32_t respEvent;
    zmKeyEventState_t eventStatus;
    zmKeyEventCb funcCb;
    readKeyLevelFunc readKeyLevel;
}zmKeyStatus_t;

/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
static zmKeyType_t keyReg = 0;
static zmKeyStatus_t keyStatusTable[ZM_KEY_MAX_NUM];
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
*     null
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
*     key : Bit mask value of keys to register.
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
            stu->debounceTick = ZM_DEFAULT_DEBOUNCE_TICKS;
            stu->shortPressTick = ZM_DEFAULT_PRESS_SHORT_TICKS;
#ifdef ZM_KEY_USE_LONG_PRESS
            stu->longPressTick = ZM_DEFAULT_PRESS_LONG_TICKS;
            stu->holdCbInterval = ZM_DEFAULT_PRESS_HOLD_TICKS;
#endif
            stu->respEvent = respEvent;
            stu->ticksRecord = 0;
            stu->funcCb = keyCallback;
            stu->readKeyLevel = NULL;
            memset(&stu->eventStatus, 0, sizeof(zmKeyEventState_t));
            //stu->keyLevel = zm_readKeyLevel(key);
            keyReg |= key;
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
    zmDriverSetTimerEvent(ZM_DRIVER_KEY_POLL_EVENT, ZM_DEFAULT_POLL_TIME, true);
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
    zmDriverStopTimerEvent(ZM_DRIVER_KEY_POLL_EVENT);
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
*     null
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
    
    while(keys)
    {
        if(keys & key)
        {
            if(stu->eventStatus.keyEvent > ZM_KEY_NONE_PRESS) stu->ticksRecord++;
            
            if(stu->readKeyLevel) keyLevelVal = stu->readKeyLevel(key);
            else keyLevelVal = zm_readKeyLevel(key);
            
            if(keyLevelVal != stu->keyLevel)
            {
                if(++stu->debounceCnt >= ZM_KEY_DEBOUNCE_TIME(stu))
                {
                    stu->keyLevel = keyLevelVal;
                    stu->debounceCnt = 0;
                }
            }
            else stu->debounceCnt = 0;
            switch(stu->eventStatus.keyEvent)
            {
            case ZM_KEY_NONE_PRESS:
                if(!stu->keyLevel) break;
                else
                {
                    stu->ticksRecord = 0;
                    stu->eventStatus.keyEvent = ZM_KEY_PRESS_DOWN;
                    if(stu->respEvent & ZM_KEY_EVENT_PRESS_DOWN)
                    {
                        ZM_KEY_CALLBACK(key, stu);
                    }
                }
                break;
            case ZM_KEY_PRESS_DOWN:
                if(stu->keyLevel)
                {
#ifdef ZM_KEY_USE_LONG_PRESS
                    if(stu->ticksRecord >= ZM_KEY_LONG_PRESS_TIME(stu))
                    {
                        stu->eventStatus.keyEvent = ZM_KEY_LONG_PRESS;
                        stu->eventStatus.status.pressLongTime = 0;
                        if(stu->respEvent & ZM_KEY_EVENT_PRESS_LONG)
                        {
                            ZM_KEY_CALLBACK(key, stu);
                        }
                    }
#endif
                }
                else
                {
                    stu->ticksRecord = 0;
                    stu->eventStatus.keyEvent = ZM_KEY_PRESS_UP;
                    if(stu->respEvent & ZM_KEY_EVENT_PRESS_UP)
                    {
                        ZM_KEY_CALLBACK(key, stu);
                    }
                }
                break;
            case ZM_KEY_PRESS_UP:
                if(stu->ticksRecord >= ZM_KEY_SHORT_PRESS_TIME(stu))
                {
                    ZM_U8_MAX_HOLD(stu->eventStatus.status.pressNum);
                    if(stu->respEvent & ZM_KEY_EVENT_SHORT_PRESS_EACH || 
                       stu->respEvent & ZM_VS(ZM_KEY_EVENT_PRESS_DOWN, stu->eventStatus.status.pressNum))
                    {
                        ZM_KEY_CALLBACK(key, stu);
                        stu->eventStatus.keyEvent = ZM_KEY_NONE_PRESS;
                        stu->eventStatus.status.pressLongTime = 0;
                        stu->ticksRecord = 0;
                    }
                }
                break;
            case ZM_KEY_SHORT_PRESS:
                
                break;
            case ZM_KEY_LONG_PRESS:
                
                break;
            }
            keys ^= key;
        }
        key <<= 1;
        stu++;
    }
}

static uint8_t zm_readKeyLevel(zmKeyType_t key)
{
    return zm_keyBoardReadLevel(key);
}

#else

#endif
/****************************************************** END OF FILE ******************************************************/
