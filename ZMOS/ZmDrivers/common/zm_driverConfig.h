/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* zm_driverConfig.h
*
* DESCRIPTION:
*     ZM driver config.
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
#ifndef __ZM_DRIVERCONFIG_H__
#define __ZM_DRIVERCONFIG_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/

/*******************************************************************
 *  ZM led driver config.
 */
/**
 * @brief ZM led drivers maximum support leds is the number.
 *        0 : ZM led drivers are not used.
 *
 * @note No more than 32.
 *       The larger the value, the more space it takes up.
 */
#ifndef ZM_LED_MAX_NUM
#define ZM_LED_MAX_NUM          0
#endif
    
#if ZM_LED_MAX_NUM > 0
     
#if ZM_LED_MAX_NUM > 32
#error "The leds num no more than 32."
#endif
/* ZM led blink enable */
#define ZM_LED_BLINK
     
#endif
/**********************End of zm led dirver config*****************/
     
     
/*******************************************************************
 *  ZM key driver config.
 */
/**
 * @brief ZM key drivers maximum support keys is the number.
 *        0 : ZM key drivers are not used.
 *
 * @note No more than 32.
 *       The larger the value, the more space it takes up.
 */
#ifndef ZM_KEY_MAX_NUM
#define ZM_KEY_MAX_NUM          0
#endif
    
#if ZM_KEY_MAX_NUM > 0
     
#if ZM_KEY_MAX_NUM > 32
#error "The keys num no more than 32."
#endif
/**
 * Long press enable.
 *      1 : enable
 *      0 : disable
 */
#ifndef ZM_KEY_USE_LONG_PRESS
#define ZM_KEY_USE_LONG_PRESS   0
#endif
/**
 * Enable or disable custom key configuration.
 *      1 : enable
 *      0 : disable
 */
#ifndef ZM_KEY_ENABLE_CUSTOM
#define ZM_KEY_ENABLE_CUSTOM    0
#endif
/**
 * Press down time record.
 *      1 : enable
 *      0 : disable
 */
#ifndef ZM_KEY_USE_PRESS_DOWN_TIME_RECORD
#define ZM_KEY_USE_PRESS_DOWN_TIME_RECORD   0
#endif
     
#endif
/**********************End of zm key dirver config*****************/
    
/*******************************************************************
 *  ZM i2c driver config.
 */
/**
 * @brief ZM i2c drivers maximum support i2c is the number.
 *        0 : ZM i2c drivers are not used.
 *
 * @note No more than 32.
 *       The larger the value, the more space it takes up.
 */
#ifndef ZM_I2C_MAX_NUM
#define ZM_I2C_MAX_NUM          0
#endif
    
#if ZM_I2C_MAX_NUM > 0
     
#if ZM_I2C_MAX_NUM > 32
#error "The i2c num no more than 32."
#endif
     
#endif
/**********************End of zm i2c dirver config*****************/
    
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
#endif /* zm_driverConfig.h */
