/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* zm_led.h
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
#ifndef __ZM_LED_H__
#define __ZM_LED_H__
 
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
     
/* Led modes */
#define ZM_LED_MODE_OFF     0x00
#define ZM_LED_MODE_ON      0x01
#define ZM_LED_MODE_TOGGLE  0x02
#define ZM_LED_MODE_BLINK   0x04
#define ZM_LED_MODE_FLASH   0x08

/* ZM leds */
#define ZM_LED_1   BS(0)
#define ZM_LED_2   BS(1)
#define ZM_LED_3   BS(2)
#define ZM_LED_4   BS(3)
#define ZM_LED_5   BS(4)
#define ZM_LED_6   BS(5)
#define ZM_LED_7   BS(6)
#define ZM_LED_8   BS(7)
#if (ZM_LED_MAX_NUM > 8)
#define ZM_LED_9   BS(8)
#define ZM_LED_10  BS(9)
#define ZM_LED_11  BS(10)
#define ZM_LED_12  BS(11)
#define ZM_LED_13  BS(12)
#define ZM_LED_14  BS(13)
#define ZM_LED_15  BS(14)
#define ZM_LED_16  BS(15)
#if (ZM_LED_MAX_NUM > 16)
#define ZM_LED_17  BS(16)
#define ZM_LED_18  BS(17)
#define ZM_LED_19  BS(18)
#define ZM_LED_20  BS(19)
#define ZM_LED_21  BS(20)
#define ZM_LED_22  BS(21)
#define ZM_LED_23  BS(22)
#define ZM_LED_24  BS(23)
#define ZM_LED_25  BS(24)
#define ZM_LED_26  BS(25)
#define ZM_LED_27  BS(26)
#define ZM_LED_28  BS(27)
#define ZM_LED_29  BS(28)
#define ZM_LED_30  BS(29)
#define ZM_LED_31  BS(30)
#define ZM_LED_32  BS(31)
#endif //(ZM_LED_MAX_NUM > 8)
#endif // (ZM_LED_MAX_NUM > 16)
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
#if (ZM_LED_MAX_NUM <= 8)
typedef uint8_t zmLedType_t;
#elif (ZM_LED_MAX_NUM <= 16)
typedef uint16_t zmLedType_t;
#else
typedef uint32_t zmLedType_t;
#endif


/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
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
void zm_ledInit(void);
/*****************************************************************
* FUNCTION: zm_ledSet
*
* DESCRIPTION:
*     Set leds mode.
* INPUTS:
*     leds : bit mask value of leds to be set.
*     mode : led mode on, off, toggle, blink, flash.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_ledSet(zmLedType_t leds, uint8_t mode);
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
void zm_ledOnTime(zmLedType_t leds, uint32_t time);
/*****************************************************************
* FUNCTION: zm_ledSetBlinkNum
*
* DESCRIPTION:
*     Leds start blink.
* INPUTS:
*     leds : bit mask value of leds to be set blink.
*     numBlinks : The number of blinks, 0 is keeps flashing.
*     percent : The percentage in each period where the led will be on.
*     period : Length of each cycle.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_ledSetBlinkNum(zmLedType_t leds, uint8_t numBlinks, uint8_t percent, uint16_t period);
/*****************************************************************
* FUNCTION: zm_ledSetBlinkTime
*
* DESCRIPTION:
*     Leds start blink.
* INPUTS:
*     leds : bit mask value of leds to be set blink.
*     timeBlinks : Blink time, 0 is keeps flashing.
*     percent : The percentage in each period where the led will be on.
*     period : Length of each cycle.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zm_ledSetBlinkTime(zmLedType_t leds, uint32_t timeBlinks, uint8_t percent, uint16_t period);


#ifdef __cplusplus
}
#endif
#endif /* zm_led.h */
