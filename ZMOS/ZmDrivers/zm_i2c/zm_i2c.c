/*****************************************************************
* Copyright (C) 2022 zm. All rights reserved.                    *
******************************************************************
* zm_i2c.c
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
 
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include <string.h>
#include "ZMOS_Types.h"
#include "zm_i2c.h"

#if ZM_I2C_MAX_NUM > 0
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
#define ZM_I2C_CHECK_INIT()     if(!zmI2cDelayUs) return ZM_I2C_NONE_INIT

#define ZM_I2C_DELAY_US(us)     if(zmI2cDelayUs) zmI2cDelayUs((us));\
                                else return ZM_I2C_NONE_INIT
                                    
#define ZM_I2C_DELAY_NOT_RTN(us) if(zmI2cDelayUs) zmI2cDelayUs((us))
                                    
#define ZM_I2C_PIN_FUNC(fn, val) if(fn) fn(val)
                                    
                                    
#define ZM_I2C_ACK      1
#define ZM_I2C_NOT_ACK  0

#define ZM_I2C_WAIT_ACK_US     2000
                                    
#define ZM_I2C_SLAVE_ADDR_DEAL << 1
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
typedef struct
{
    zmI2cApi_t i2cApi;
}i2cStu_t;
/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
static zmI2cDelayUsFunc zmI2cDelayUs = NULL;
static i2cStu_t zmI2cStuTabel[ZM_I2C_MAX_NUM];
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
static void zm_i2cStart(zmI2cType_t i2c);
static void zm_i2cStop(zmI2cType_t i2c);
static void zm_i2cAck(zmI2cType_t i2c);
static void zm_i2cNotAck(zmI2cType_t i2c);
static uint8_t zm_i2cGetAck(zmI2cType_t i2c);
static void zm_i2cSendByte(zmI2cType_t i2c, uint8_t data);
static uint8_t zm_i2cReadByte(zmI2cType_t i2c);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
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
zmI2cRes_t zm_i2cInit(zmI2cDelayUsFunc delayFunc)
{
    if(!delayFunc) return ZM_I2C_PARAM_ERR;
    
    memset(zmI2cStuTabel, 0, sizeof(i2cStu_t) * ZM_I2C_MAX_NUM);
    
    zmI2cDelayUs = delayFunc;
    
    return ZM_I2C_SUCCESS;
}

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
void zm_i2cDeinit(void)
{
    zmI2cDelayUs = NULL;
}
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
zmI2cRes_t zm_i2cApiSet(zmI2cType_t i2c, zmI2cApi_t api)
{
    if(i2c >= ZM_I2C_MAX_NUM) return ZM_I2C_PARAM_ERR;
    
    ZM_I2C_CHECK_INIT();
    
    zmI2cStuTabel[i2c].i2cApi = api;
    
    return ZM_I2C_SUCCESS;
}
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
zmI2cRes_t zm_i2cSetConfig(zmI2cType_t i2c, zmI2cConfItem_t item, void *conf)
{
    if(i2c >= ZM_I2C_MAX_NUM) return ZM_I2C_PARAM_ERR;
    
    ZM_I2C_CHECK_INIT();
    
    switch(item)
    {
    case ZM_I2C_CONF_SET_DELAY_FN:
        zmI2cDelayUs = (zmI2cDelayUsFunc)conf;
        break;
    case ZM_I2C_CONF_SET_SCL_PIN_VAL_FN:
        zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet = (zmI2cPinzValSetFunc)conf;
        break;
    case ZM_I2C_CONF_SET_SDA_PIN_DIR_FN:
        zmI2cStuTabel[i2c].i2cApi.i2cSdaPinDirSet = (zmI2cPinDirSetFunc)conf;
        break;
    case ZM_I2C_CONF_SET_SDA_PIN_VAL_FN:
        zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet = (zmI2cPinzValSetFunc)conf;
        break;
    case ZM_I2C_CONF_SET_SDA_PIN_READ_FN:
        zmI2cStuTabel[i2c].i2cApi.i2cSdaPinRead = (zmI2cPinReadFunc)conf;
        break;
    default:
        return ZM_I2C_PARAM_ERR;
    }
    return ZM_I2C_SUCCESS;
}

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
zmI2cRes_t zm_i2cClearBus(zmI2cType_t i2c)
{
    if(i2c >= ZM_I2C_MAX_NUM) return ZM_I2C_PARAM_ERR;
    
    ZM_I2C_CHECK_INIT();
    
    for(uint8_t i = 0; i < 9; i++)
    {
        ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
        ZM_I2C_DELAY_US(2);
        ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_HIGH);
        ZM_I2C_DELAY_US(2);
    }
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
    
    return ZM_I2C_SUCCESS;
}

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
                      uint8_t slaveAddr,
                      uint8_t *regAddr,
                      uint8_t regAddrLen,
                      uint8_t *buf,
                      uint16_t dataLen)
{
    if(i2c >= ZM_I2C_MAX_NUM) return ZM_I2C_PARAM_ERR;
    
    ZM_I2C_CHECK_INIT();
    
    zm_i2cStart(i2c);
    
    zm_i2cSendByte(i2c, slaveAddr ZM_I2C_SLAVE_ADDR_DEAL);
    
    if(zm_i2cGetAck(i2c) == ZM_I2C_NOT_ACK) return ZM_I2C_FAIL;
    
    if(regAddr)
    {
        for(uint8_t i = 0; i < regAddrLen; i++)
        {
            zm_i2cSendByte(i2c, regAddr[i]);
            if(zm_i2cGetAck(i2c) == ZM_I2C_NOT_ACK) return ZM_I2C_FAIL;
        }
    }
    
    if(buf)
    {
        for(uint16_t i = 0; i < dataLen; i++)
        {
            zm_i2cSendByte(i2c, buf[i]);
            if(zm_i2cGetAck(i2c) == ZM_I2C_NOT_ACK) return ZM_I2C_FAIL;
        }
    }
    
    zm_i2cStop(i2c);
    
    ZM_I2C_DELAY_US(4);
    
    return ZM_I2C_SUCCESS;
}

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
                         uint8_t slaveAddr,
                         uint8_t *regAddr,
                         uint8_t regAddrLen,
                         uint8_t *buf,
                         uint16_t dataLen)
{
    if(i2c >= ZM_I2C_MAX_NUM) return ZM_I2C_PARAM_ERR;
    
    ZM_I2C_CHECK_INIT();
    
    if(regAddr && regAddrLen)
    {
        zm_i2cStart(i2c);
        
        zm_i2cSendByte(i2c, slaveAddr ZM_I2C_SLAVE_ADDR_DEAL);
        
        if(zm_i2cGetAck(i2c) == ZM_I2C_NOT_ACK) return ZM_I2C_FAIL;
        
        for(uint8_t i = 0; i < regAddrLen; i++)
        {
            zm_i2cSendByte(i2c, regAddr[i]);
            if(zm_i2cGetAck(i2c) == ZM_I2C_NOT_ACK) return ZM_I2C_FAIL;
        }
    }
    
    if(buf)
    {
        zm_i2cStart(i2c);
        
        zm_i2cSendByte(i2c, (slaveAddr ZM_I2C_SLAVE_ADDR_DEAL) | 0x01);
        
        if(zm_i2cGetAck(i2c) == ZM_I2C_NOT_ACK) return ZM_I2C_FAIL;
        
        for(uint16_t i = 0; i < dataLen;)
        {
            buf[i++] = zm_i2cReadByte(i2c);
            if(i < dataLen) zm_i2cAck(i2c);
            else zm_i2cNotAck(i2c);
        }
    }
    
    zm_i2cStop(i2c);
    
    ZM_I2C_DELAY_US(4);
    
    return ZM_I2C_SUCCESS;
}

/*****************************************************************
* FUNCTION: zm_i2cStart
*
* DESCRIPTION:
*     I2c generate start signal.
* INPUTS:
*     i2c : Which i2c module.
* RETURNS:
*     null
* NOTE:
*     
*****************************************************************/
static void zm_i2cStart(zmI2cType_t i2c)
{
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_HIGH);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinDirSet, ZM_I2C_PIN_OUT);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_HIGH);
    
    ZM_I2C_DELAY_NOT_RTN(3);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_LOW);
    
    ZM_I2C_DELAY_NOT_RTN(3);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
    
    ZM_I2C_DELAY_NOT_RTN(3);
}

/*****************************************************************
* FUNCTION: zm_i2cStop
*
* DESCRIPTION:
*     I2c generate stop signal.
* INPUTS:
*     i2c : Which i2c module.
* RETURNS:
*     null
* NOTE:
*     
*****************************************************************/
static void zm_i2cStop(zmI2cType_t i2c)
{
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinDirSet, ZM_I2C_PIN_OUT);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_LOW);
    
    ZM_I2C_DELAY_NOT_RTN(1);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_HIGH);
    
    ZM_I2C_DELAY_NOT_RTN(3);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_HIGH);
    
    ZM_I2C_DELAY_NOT_RTN(3);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_LOW);
}

/*****************************************************************
* FUNCTION: zm_i2cAck
*
* DESCRIPTION:
*     I2c generate ack signal.
* INPUTS:
*     i2c : Which i2c module.
* RETURNS:
*     null
* NOTE:
*     
*****************************************************************/
static void zm_i2cAck(zmI2cType_t i2c)
{
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinDirSet, ZM_I2C_PIN_OUT);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_LOW);
    
    ZM_I2C_DELAY_NOT_RTN(2);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_HIGH);
    
    ZM_I2C_DELAY_NOT_RTN(2);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
}

/*****************************************************************
* FUNCTION: zm_i2cNotAck
*
* DESCRIPTION:
*     I2c not ack.
* INPUTS:
*     i2c : Which i2c module.
* RETURNS:
*     null
* NOTE:
*     
*****************************************************************/
static void zm_i2cNotAck(zmI2cType_t i2c)
{
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinDirSet, ZM_I2C_PIN_OUT);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_LOW);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_HIGH);
    
    ZM_I2C_DELAY_NOT_RTN(1);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_HIGH);
    
    ZM_I2C_DELAY_NOT_RTN(3);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
}
/*****************************************************************
* FUNCTION: zm_i2cGetAck
*
* DESCRIPTION:
*     I2c wait slave ack signal.
* INPUTS:
*     i2c : Which i2c module.
* RETURNS:
*     Whether the slave ack, ZM_I2C_ACK or ZM_I2C_NOT_ACK.
* NOTE:
*     
*****************************************************************/
static uint8_t zm_i2cGetAck(zmI2cType_t i2c)
{
    uint32_t waitCnt = 0;
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinDirSet, ZM_I2C_PIN_IN);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
    
    ZM_I2C_DELAY_NOT_RTN(1);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_HIGH);
    
    ZM_I2C_DELAY_NOT_RTN(1);
    
    while(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinRead() == ZM_I2C_PIN_HIGH)
    {
        if(++waitCnt >= ZM_I2C_WAIT_ACK_US)
        {
            zm_i2cStop(i2c);
            return ZM_I2C_NOT_ACK;
        }
        ZM_I2C_DELAY_NOT_RTN(1);
    }
    return ZM_I2C_ACK;
}

/*****************************************************************
* FUNCTION: zm_i2cSendByte
*
* DESCRIPTION:
*     I2c send a byte data to slave.
* INPUTS:
*     i2c : Which i2c module.
*     data : data.
* RETURNS:
*     null
* NOTE:
*     
*****************************************************************/
static void zm_i2cSendByte(zmI2cType_t i2c, uint8_t data)
{
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinDirSet, ZM_I2C_PIN_OUT);
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
    
    for(uint8_t i = 0; i < 8; i++)
    {
        if(data & 0x80)
        {
            ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_HIGH);
        }
        else ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinValSet, ZM_I2C_PIN_LOW);
        
        data <<= 1;
        
        ZM_I2C_DELAY_NOT_RTN(1);
        
        ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_HIGH);
        
        ZM_I2C_DELAY_NOT_RTN(2);
        
        ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
        
        ZM_I2C_DELAY_NOT_RTN(2);
    }
}

/*****************************************************************
* FUNCTION: zm_i2cReadByte
*
* DESCRIPTION:
*     I2c receive a byte data.
* INPUTS:
*     i2c : Which i2c module.
* RETURNS:
*     Received data.
* NOTE:
*     
*****************************************************************/
static uint8_t zm_i2cReadByte(zmI2cType_t i2c)
{
    uint8_t data = 0;
    
    ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinDirSet, ZM_I2C_PIN_IN);
    
    for(uint8_t i = 0; i < 8; i++)
    {
        ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_LOW);
        
        ZM_I2C_DELAY_NOT_RTN(2);
        
        ZM_I2C_PIN_FUNC(zmI2cStuTabel[i2c].i2cApi.i2cSclPinValSet, ZM_I2C_PIN_HIGH);
        
        data <<= 1;
        
        if(zmI2cStuTabel[i2c].i2cApi.i2cSdaPinRead() == ZM_I2C_PIN_HIGH)
        {
            data++;
        }
        
        ZM_I2C_DELAY_NOT_RTN(2);
    }
    
    return data;
}

#endif
/****************************************************** END OF FILE ******************************************************/
