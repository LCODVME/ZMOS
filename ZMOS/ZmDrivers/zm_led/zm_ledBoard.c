/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* zm_ledBoard.c
*
* DESCRIPTION:
*     zm led board
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/6/6
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
#include "ZMOS_Types.h"
#include "zm_led.h"
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: zm_ledBoardOnOff
*
* DESCRIPTION:
*     This function to achieve MCU driven LED on/off function.
* INPUTS:
*     led : Bit mask value of led to be set.
*     mode : Turn on or off.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
__weak void zm_ledBoardOnOff(zmLedType_t led, zm_uint8_t mode)
{
    switch(led)
    {
    case ZM_LED_1:
        if(mode)
        {
            // Led on.
        }
        else
        {
             //Led off.
        }
        break;
    case ZM_LED_2:
        if(mode)
        {
            
        }
        else
        {
            
        }
        break;
    default :
        break;
    };
}
/****************************************************** END OF FILE ******************************************************/
