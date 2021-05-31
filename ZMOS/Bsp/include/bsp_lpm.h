/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* bsp_lpm.h
*
* DESCRIPTION:
*     Low power management bsp.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/5/31
* REVISION:
*     v0.1
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*
*****************************************************************/
#ifndef __BSP_LPM_H__
#define __BSP_LPM_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
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
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: bsp_lowPwrEnterBefore
*
* DESCRIPTION:
*     This function is called before entering low power.
* INPUTS:
*     timeout : zmos timer next timeout.
*               A value of 0xFFFFFFFF indicates 
*               that no timer is running.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void bsp_lowPwrEnterBefore(uint32_t timeout);
/*****************************************************************
* FUNCTION: bsp_systemEnterLpm
*
* DESCRIPTION:
*     This function put the cpu enter low power mode.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void bsp_systemEnterLpm(void);
/*****************************************************************
* FUNCTION: bsp_lowPwrExitAfter
*
* DESCRIPTION:
*     This function is called after exiting low power.
*     According to the MCU characteristics of low-power 
*     wake up processing, such as: initialization clock, 
*     clock compensation, etc.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void bsp_lowPwrExitAfter(void);

#ifdef __cplusplus
}
#endif
#endif /* bsp_lpm.h */
