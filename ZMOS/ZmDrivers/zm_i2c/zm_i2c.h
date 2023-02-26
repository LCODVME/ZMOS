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
/**
 * zm i2c result type.
 */
typedef enum
{
    ZM_I2C_SUCCESS = 0u,
    ZM_I2C_FAIL,
    ZM_I2C_NONE_INIT,
    ZM_I2C_PARAM_ERR
}zmI2cRes_t;

/** 
 * zm i2c config item.
 */
typedef enum
{
    ZM_I2C_CONF_SET_DELAY_FN,
    ZM_I2C_CONF_SET_SCL_PIN_VAL_FN,
    ZM_I2C_CONF_SET_SDA_PIN_DIR_FN,
    ZM_I2C_CONF_SET_SDA_PIN_VAL_FN,
    ZM_I2C_CONF_SET_SDA_PIN_READ_FN
}zmI2cConfItem_t;

/**
 * zm i2c pin direction define.
 */
typedef enum
{
    ZM_I2C_PIN_IN = 0u,
    ZM_I2C_PIN_OUT
}zmI2cPinDir_t;

/**
 * zm i2c pin level define.
 */
typedef enum
{
    ZM_I2C_PIN_LOW = 0,
    ZM_I2C_PIN_HIGH,
}zmI2cPinVal_t;

/**
 * zm i2c type define.
 */
#if (ZM_I2C_MAX_NUM < 8)
typedef zm_uint8_t zmI2cType_t;
#elif (ZM_I2C_MAX_NUM < 16)
typedef zm_uint16_t zmI2cType_t;
#else
typedef zm_uint32_t zmI2cType_t;
#endif

/**
 * zm i2c delay us function type.
 *
 * @param[in] us : Microseconds of delay.
 */
typedef void (* zmI2cDelayUsFunc)(zm_uint8_t us);
/**
 * zm i2c set pin value function type.
 *
 * @param[in] val : Set pin value(@ref zmI2cPinVal_t).
 */
typedef void (* zmI2cPinzValSetFunc)(zmI2cPinVal_t val);
/**
 * zm i2c set pin direction function type.
 *
 * @param[in] val : Set pin value(@ref zmI2cPinDir_t).
 */
typedef void (* zmI2cPinDirSetFunc)(zmI2cPinDir_t dir);
/**
 * zm i2c read pin value function type.
 *
 * @return Pin value (@ref zmI2cPinVal_t).
 */
typedef zmI2cPinVal_t (* zmI2cPinReadFunc)(void);

/**
 * zm i2c api struct.
 */
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
/*****************************************************************
* FUNCTION: zm_i2cInit
*
* DESCRIPTION:
*     I2c driver initialize.
* INPUTS:
*     delayFunc : Microsecond delay function.
* RETURNS:
*     Init status (@ref zmI2cRes_t).
* NOTE:
*     
*****************************************************************/
zmI2cRes_t zm_i2cInit(zmI2cDelayUsFunc delayFunc);
/*****************************************************************
* FUNCTION: zm_i2cDeinit
*
* DESCRIPTION:
*     I2c driver deinit.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     
*****************************************************************/
void zm_i2cDeinit(void);
/*****************************************************************
* FUNCTION: zm_i2cApiSet
*
* DESCRIPTION:
*     Set i2c api.
* INPUTS:
*     i2c : Which i2c module to set.
*     api : The api struct (@ref zmI2cApi_t).
* RETURNS:
*     Execution status (@ref zmI2cRes_t).
* NOTE:
*     
*****************************************************************/
zmI2cRes_t zm_i2cApiSet(zmI2cType_t i2c, zmI2cApi_t api);
/*****************************************************************
* FUNCTION: zm_i2cSetConfig
*
* DESCRIPTION:
*     Set i2c module config.
* INPUTS:
*     i2c : Which i2c module to config.
*     item : Item to configure (@ref zmI2cConfItem_t).
*     conf : Configuration value.
* RETURNS:
*     Execution status (@ref zmI2cRes_t).
* NOTE:
*     
*****************************************************************/
zmI2cRes_t zm_i2cSetConfig(zmI2cType_t i2c, zmI2cConfItem_t item, void *conf);
/*****************************************************************
* FUNCTION: zm_i2cClearBus
*
* DESCRIPTION:
*     Clear i2c bus.
* INPUTS:
*     i2c : Which i2c bus to clear.
* RETURNS:
*     Execution status (@ref zmI2cRes_t).
* NOTE:
*     
*****************************************************************/
zmI2cRes_t zm_i2cClearBus(zmI2cType_t i2c);
/*****************************************************************
* FUNCTION: zm_i2cSend
*
* DESCRIPTION:
*     I2c send data to slave.
* INPUTS:
*     i2c : Which i2c module to send.
*     slaveAddr : Slave address.
*     regAddr : Register address.
*     regAddrLen : Register address length.
*     buf : Poing to data buffer.
*     dataLen : Length to send.
* RETURNS:
*     Execution status (@ref zmI2cRes_t).
* NOTE:
*     
*****************************************************************/
zmI2cRes_t zm_i2cSend(zmI2cType_t i2c,
                      zm_uint8_t slaveAddr,
                      zm_uint8_t *regAddr,
                      zm_uint8_t regAddrLen,
                      zm_uint8_t *buf,
                      zm_uint16_t dataLen);
/*****************************************************************
* FUNCTION: zm_i2cReceive
*
* DESCRIPTION:
*     I2c receive data.
* INPUTS:
*     i2c : Which i2c module to receive.
*     slaveAddr : Slave address.
*     regAddr : Register address.
*     regAddrLen : Register address length.
*     buf : Poing to data buffer.
*     dataLen : Length to receive.
* RETURNS:
*     Execution status (@ref zmI2cRes_t).
* NOTE:
*     
*****************************************************************/
zmI2cRes_t zm_i2cReceive(zmI2cType_t i2c,
                         zm_uint8_t slaveAddr,
                         zm_uint8_t *regAddr,
                         zm_uint8_t regAddrLen,
                         zm_uint8_t *buf,
                         zm_uint16_t dataLen);

    
#ifdef __cplusplus
}
#endif
#endif /* zm_i2c.h */
