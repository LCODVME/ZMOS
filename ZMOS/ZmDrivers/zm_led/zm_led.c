/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* zm_led.c
*
* DESCRIPTION:
*     zm led driver.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/6/2
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
#include "zm_driverConfig.h"
#include "ZMOS.h"
#include "zm_drivers.h"
#include "zm_led.h"

#if ZM_LED_MAX_NUM > 0
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
#define ZM_LED_ALL      0xFFFFFFFF
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
typedef enum
{
    ZM_CYC_NUM,
    ZM_CYC_TIME
}zmCycMode_t;
typedef struct
{
    union
    {
        uint8_t num;
        uint32_t timeout;
    }u;
    zmCycMode_t mode;
}zmBlinkCyc_t;

typedef struct
{
    uint8_t mode;       /* Operation mode */
    zmBlinkCyc_t cyc;       /* Blink cycles left */
    uint8_t onPct;      /* On cycle percentage */
    uint16_t period;      /* On/off cycle time (msec) */
    uint32_t next;      /* Time for next change */
}zmLedControl_t;

typedef struct
{
    zmLedControl_t  ledCtrlTable[ZM_LED_MAX_NUM];
}zmLedStatus_t;



/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
static zmLedType_t zmLedsState;
static zmLedType_t zmPreBlinkState;
static zmLedStatus_t zmLedCtrlStatus;
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
static void zm_ledOnOff(zmLedType_t leds, uint8_t mode);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
extern void zm_ledBoardOnOff(zmLedType_t led, uint8_t mode);
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: zm_ledInit
*
* DESCRIPTION:
*     Initialize led driver.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_ledInit(void)
{
    zm_ledSet(BS(ZM_LED_MAX_NUM) - 1, ZM_LED_MODE_OFF);
}
/*****************************************************************
* FUNCTION: zm_ledSet
*
* DESCRIPTION:
*     Set leds mode.
* INPUTS:
*     leds : Bit mask value of leds to be set.
*     mode : Led mode on, off, toggle, blink, flash.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_ledSet(zmLedType_t leds, uint8_t mode)
{
#ifdef ZM_LED_BLINK
    zmLedType_t led;
    zmLedControl_t *stu;
    
    if(leds >= BS(ZM_LED_MAX_NUM)) return;
    
    switch(mode)
    {
    case ZM_LED_MODE_BLINK:
        zm_ledSetBlinkNum(leds, 1, 5, 1000);
        break;
    case ZM_LED_MODE_FLASH:
        zm_ledSetBlinkNum(leds, 55, 5, 1000);
        break;
    case ZM_LED_MODE_ON:
    case ZM_LED_MODE_OFF:
    case ZM_LED_MODE_TOGGLE:
        led = ZM_LED_1;
        stu = zmLedCtrlStatus.ledCtrlTable;
        
        while(leds)
        {
            if(leds & led)
            {
                if(mode == ZM_LED_MODE_TOGGLE)
                {
                    stu->mode ^= ZM_LED_MODE_ON;
                }
                else
                {
                    stu->mode = mode;
                }
                zm_ledOnOff(led, stu->mode);
                leds ^= led;
            }
            led <<= 1;
            stu++;
        }
        break;
    default:
        break;
    }
#else
    
#endif
}
/*****************************************************************
* FUNCTION: zm_ledOnTime
*
* DESCRIPTION:
*     Turn on for a time.
* INPUTS:
*     leds : Bit mask value of leds to be turn on.
*     time : Turn on for a time.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_ledOnTime(zmLedType_t leds, uint32_t time)
{
    if(leds >= BS(ZM_LED_MAX_NUM)) return;
    
#ifdef ZM_LED_BLINK
    
    if(time)
    {
        zm_ledSet(leds, ZM_LED_MODE_OFF);
        zm_ledSetBlinkNum(leds, 1, 50, time * 2);
    }
#else
    
#endif
}
/*****************************************************************
* FUNCTION: zm_ledSetBlinkNum
*
* DESCRIPTION:
*     Leds start blink a certain number of times.
* INPUTS:
*     leds : Bit mask value of leds to be set blink.
*     numBlinks : The number of blinks, 0 is keeps flashing.
*     percent : The percentage in each period where the led will be on.
*     period : Length of each cycle.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_ledSetBlinkNum(zmLedType_t leds, uint8_t numBlinks, uint8_t percent, uint16_t period)
{
#ifdef ZM_LED_BLINK
    zmLedType_t led;
    zmLedControl_t *stu;
    
    if(leds >= BS(ZM_LED_MAX_NUM)) return;
    
    if (leds && percent && period)
    {
        if(percent < 100)
        {
            led = ZM_LED_1;
            stu = zmLedCtrlStatus.ledCtrlTable;
            
            while(leds)
            {
                if(leds & led)
                {
                    if(stu->mode < ZM_LED_MODE_TOGGLE)
                    {
                        //Store the current state of the led
                        zmPreBlinkState |= (led & zmLedsState);
                    }
                    stu->mode = ZM_LED_MODE_OFF;
                    stu->onPct = percent;
                    stu->period = period;
                    stu->cyc.mode = ZM_CYC_NUM;
                    stu->cyc.u.num = numBlinks;
                    if(!numBlinks)
                        stu->mode |= ZM_LED_MODE_FLASH;
                    stu->mode |= ZM_LED_MODE_BLINK;
                    stu->next = zmos_getTimerClock();
                    leds ^= led;
                }
                led <<= 1;
                stu++;
            }
            zm_driverSetEvent(ZM_DRIVER_LED_BLINK_EVENT);
        }
        else
        {
            zm_ledOnOff(leds, ZM_LED_MODE_ON);
        }
    }
    else
    {
        zm_ledOnOff(leds, ZM_LED_MODE_OFF);
    }
#else
    
#endif
}
/*****************************************************************
* FUNCTION: zm_ledSetBlinkTime
*
* DESCRIPTION:
*     Leds start blink for a certain time.
* INPUTS:
*     leds : Bit mask value of leds to be set blink.
*     timeBlinks : Blink time, 0 is keeps flashing.
*     percent : The percentage in each period where the led will be on.
*     period : Length of each cycle.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_ledSetBlinkTime(zmLedType_t leds, uint32_t timeBlinks, uint8_t percent, uint16_t period)
{
#ifdef ZM_LED_BLINK
    zmLedType_t led;
    zmLedControl_t *stu;
    
    if(leds >= BS(ZM_LED_MAX_NUM)) return;
    
    if (leds && percent && period)
    {
        if(percent < 100)
        {
            led = ZM_LED_1;
            stu = zmLedCtrlStatus.ledCtrlTable;
            
            while(leds)
            {
                if(leds & led)
                {
                    if(stu->mode < ZM_LED_MODE_TOGGLE)
                    {
                        //Store the current state of the led
                        zmPreBlinkState |= (led & zmLedsState);
                    }
                    stu->mode = ZM_LED_MODE_OFF;
                    stu->onPct = percent;
                    stu->period = period;
                    stu->cyc.mode = ZM_CYC_TIME;
                    stu->cyc.u.timeout = timeBlinks;
                    if(!timeBlinks)
                        stu->mode |= ZM_LED_MODE_FLASH;
                    stu->mode |= ZM_LED_MODE_BLINK;
                    stu->next = zmos_getTimerClock();
                    leds ^= led;
                }
                led <<= 1;
                stu++;
            }
            zm_driverSetEvent(ZM_DRIVER_LED_BLINK_EVENT);
        }
        else
        {
            zm_ledOnOff(leds, ZM_LED_MODE_ON);
        }
    }
    else
    {
        zm_ledOnOff(leds, ZM_LED_MODE_OFF);
    }
#else
    
#endif
}
/*****************************************************************
* FUNCTION: zmos_ledSetToggleNum
*
* DESCRIPTION:
*     Leds start toggle a certain number of times.
* INPUTS:
*     leds : Bit mask value of leds to be set blink.
*     numToggles : The number of toggle, 0 is keeps flashing.
*     period : Length of each cycle.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_ledSetToggleNum(zmLedType_t leds, uint8_t numToggles, uint16_t period)
{
#ifdef ZM_LED_BLINK
    zmLedType_t led;
    zmLedControl_t *stu;
    
    if(leds >= BS(ZM_LED_MAX_NUM)) return;
    
    if (leds && period)
    {
        led = ZM_LED_1;
        stu = zmLedCtrlStatus.ledCtrlTable;
        
        while(leds)
        {
            if(leds & led)
            {
                if(stu->mode < ZM_LED_MODE_TOGGLE)
                {
                    //Calc and store the end state of the led
                    zmPreBlinkState |= (led & (zmLedsState ^ ((BS(ZM_LED_MAX_NUM) - 1) * (numToggles % 2))));
                }
                stu->mode = (led & zmLedsState);
                stu->onPct = 50;
                stu->period = period;
                stu->cyc.mode = ZM_CYC_NUM;
                stu->cyc.u.num = (numToggles + 1) / 2;
                if(!numToggles)
                    stu->mode |= ZM_LED_MODE_FLASH;
                stu->mode |= ZM_LED_MODE_BLINK;
                stu->next = zmos_getTimerClock();
                if((stu->mode & ZM_LED_MODE_ON))
                {
                    stu->cyc.u.num -= ((numToggles % 2) ? 1 : 0);
                    if(stu->cyc.u.num)
                    {
                        stu->mode ^= ZM_LED_MODE_ON;
                        zm_ledOnOff(led, stu->mode & ZM_LED_MODE_ON);
                        stu->next +=(((uint32_t)stu->onPct * (uint32_t)stu->period) / 100);
                    }
                }
                leds ^= led;
            }
            led <<= 1;
            stu++;
        }
        zm_driverSetEvent(ZM_DRIVER_LED_BLINK_EVENT);
    }
    else
    {
        zm_ledOnOff(leds, ZM_LED_MODE_ON);
    }
#else
    
#endif
}
/*****************************************************************
* FUNCTION: zm_ledSetToggleTime
*
* DESCRIPTION:
*     Leds start toggle for a certain time.
* INPUTS:
*     leds : Bit mask value of leds to be set blink.
*     timeToggles : Toggle time, 0 is keeps flashing.
*     period : Length of each cycle.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_ledSetToggleTime(zmLedType_t leds, uint32_t timeToggles, uint16_t period)
{
#ifdef ZM_LED_BLINK
    //zmLedType_t led;
    //zmLedControl_t *stu;
    
    if(leds >= BS(ZM_LED_MAX_NUM)) return;
    
    zmos_ledSetToggleNum(leds, (timeToggles / period) * 2, period);
    /*if (leds && period)
    {
        led = ZM_LED_1;
        stu = zmLedCtrlStatus.ledCtrlTable;
        
        while(leds)
        {
            if(leds & led)
            {
                
                stu->mode = led & zmLedsState;
                stu->onPct = 50;
                stu->period = period;
                stu->cyc.mode = ZM_CYC_TIME;
                stu->cyc.u.timeout = timeToggles;
                if(!timeToggles)
                    stu->mode |= ZM_LED_MODE_FLASH;
                stu->mode |= ZM_LED_MODE_BLINK;
                stu->next = zmos_getTimerClock();
                
                leds ^= led;
            }
            led <<= 1;
            stu++;
        }
        zm_driverSetEvent(ZM_DRIVER_LED_BLINK_EVENT);
    }
    else
    {
        zm_ledOnOff(leds, ZM_LED_MODE_ON);
    }*/
#else
    
#endif
}
/*****************************************************************
* FUNCTION: zm_updateLedBlink
*
* DESCRIPTION:
*     Update leds to work with blink.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_updateLedBlink(void)
{
    zmLedType_t led;
    zmLedType_t leds;
    uint8_t pct;
    uint16_t next;
    uint16_t wait;
    uint32_t pass;
    uint32_t time;
    zmLedControl_t *stu;
    
    next = 0;
    led = ZM_LED_1;
    leds = BS(ZM_LED_MAX_NUM) - 1;
    stu = zmLedCtrlStatus.ledCtrlTable;
    
    while(leds)
    {
        if(leds & led)
        {
            if(stu->mode & ZM_LED_MODE_BLINK)
            {
                time = zmos_getTimerClock();
                if(time >= stu->next)
                {
                    if(stu->mode & ZM_LED_MODE_ON)
                    {
                        pct = 100 - stu->onPct;               /* Percentage of cycle for off */
                        stu->mode &= ~ZM_LED_MODE_ON;
                        zm_ledOnOff(led, ZM_LED_MODE_OFF);
                        if(!(stu->mode & ZM_LED_MODE_FLASH))
                        {
                            if(stu->cyc.mode == ZM_CYC_NUM)
                            {
                                if(!stu->cyc.u.num || --stu->cyc.u.num == 0)
                                {
                                    stu->mode ^= ZM_LED_MODE_BLINK;
                                }
                            }
                            else
                            {
                                pass = time - stu->next;
                                pass += stu->period;
                                if(stu->cyc.u.timeout > pass)
                                {
                                    stu->cyc.u.timeout -= pass;
                                }
                                else
                                {
                                    stu->cyc.u.timeout = 0;
                                    stu->mode ^= ZM_LED_MODE_BLINK;
                                }
                            }
                        }
                    }
                    else
                    {
                        pct = stu->onPct;                     /* Percentage of cycle for on */
                        stu->mode |= ZM_LED_MODE_ON;
                        zm_ledOnOff(led, ZM_LED_MODE_ON);
                    }
                    
                    if(stu->mode & ZM_LED_MODE_BLINK)
                    {
                        wait = (((uint32_t)pct * (uint32_t)stu->period) / 100);
                        stu->next = time + wait;
                    }
                    else
                    {
                        wait = 0;
                        /* After blinking, set the LED back to the state before it blinks */
                        zm_ledSet(led, ((zmPreBlinkState & led) != 0)?ZM_LED_MODE_ON:ZM_LED_MODE_OFF);
                        /* Clear the saved bit */
                        zmPreBlinkState &= (led ^ (zmLedType_t)ZM_LED_ALL);
                    }
                }
                else
                {
                    wait = stu->next - time;  /* Time left */
                }
                
                if (!next || (wait && (wait < next)))
                {
                    next = wait;
                }
            }
            leds ^= led;
        }
        led <<= 1;
        stu++;
    }
    if(next)
    {
        zm_driverSetTimerEvent(ZM_DRIVER_LED_BLINK_EVENT, next, false);
    }
}
/*****************************************************************
* FUNCTION: zm_ledOnOff
*
* DESCRIPTION:
*     Turn on/off given leds.
* INPUTS:
*     leds : Bit mask value of leds to be set on/off.
*     mode : led mode on, off.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
static void zm_ledOnOff(zmLedType_t leds, uint8_t mode)
{
    zmLedType_t led;
    
    if(leds >= BS(ZM_LED_MAX_NUM)) return;
    
    if(mode)
    {
        zmLedsState |= leds;
    }
    else
    {
        zmLedsState &= (leds ^ ((zmLedType_t)ZM_LED_ALL));
    }
    
    led = ZM_LED_1;
    
    while(leds)
    {
        if(leds & led)
        {
            zm_ledBoardOnOff(led, mode);
            leds ^= led;
        }
        led <<= 1;
    }
    
}
#else
void zm_ledInit(void) {}
void zm_ledSet(zmLedType_t leds, uint8_t mode) {}
void zm_ledOnTime(zmLedType_t leds, uint32_t time) {}
void zm_ledSetBlinkNum(zmLedType_t leds, uint8_t numBlinks, uint8_t percent, uint16_t period) {}
void zm_ledSetBlinkTime(zmLedType_t leds, uint32_t timeBlinks, uint8_t percent, uint16_t period) {}
#endif
/****************************************************** END OF FILE ******************************************************/
