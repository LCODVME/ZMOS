/*****************************************************************
* Copyright (C) 2022 zm. All rights reserved.                    *
******************************************************************
* zm_i2c.h
*
* DESCRIPTION:
*     zm i2c driver.
* AUTHOR:
*     zm
* CREATED DATE:
*     2022/7/2
* REVISION:
*     v0.1
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*
*****************************************************************/
#ifndef __ZM_I2C_H__
#define __ZM_I2C_H__
 
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

/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
typedef enum
{
    ZM_I2C_SUCCESS = 0u,
    ZM_I2C_FAIL,
    ZM_I2C_NONE_INIT,
    ZM_I2C_PARAM_ERR,
    
}zmI2cRes_t;

/** 
 * zm i2c config item.
 */
typedef enum
{
    ZM_I2C_CONF_SET_DELAY_FN,
    ZM_I2C_CONF_SET_SCL_PIN_SET_FN,
    ZM_I2C_CONF_SET_SDA_PIN_DIR_FN,
    ZM_I2C_CONF_SET_SDA_PIN_VAL_FN,
    ZM_I2C_CONF_SET_SDA_PIN_READ_FN
}zmI2cConfItem_t;

typedef enum
{
    ZM_I2C_PIN_IN = 0u,
    ZM_I2C_PIN_OUT
}zmI2cPinDir_t;

typedef enum
{
    ZM_I2C_PIN_LOW = 0,
    ZM_I2C_PIN_HIGH,
}zmI2cPinVal_t;

/**
 * zm i2c type define.
 */
#if (ZM_I2C_MAX_NUM < 8)
typedef uint8_t zmI2cType_t;
#elif (ZM_I2C_MAX_NUM < 16)
typedef uint16_t zmI2cType_t;
#else
typedef uint32_t zmI2cType_t;
#endif
     
typedef void (* zmI2cDelayUsFunc)(uint8_t us);
    
typedef void (* zmI2cPinzValSetFunc)(zmI2cPinVal_t val);
typedef void (* zmI2cPinDirSetFunc)(zmI2cPinDir_t dir);
typedef zmI2cPinVal_t (* zmI2cPinReadFunc)(void);

typedef struct
{
    zmI2cPinDirSetFunc i2cSdaPinDirSet;
    zmI2cPinzValSetFunc i2cSdaPinValSet;
    zmI2cPinzValSetFunc i2cSclPinValSet;
    zmI2cPinReadFunc i2cSdaPinRead;
}zmI2cApi_t;



/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
    
    
#ifdef __cplusplus
}
#endif
#endif /* zm_i2c.h */
