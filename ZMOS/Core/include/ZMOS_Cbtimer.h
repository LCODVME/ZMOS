/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Cbtimer.h
*
* DESCRIPTION:
*     Callback timer function.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/5/29
* REVISION:
*     v0.1
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*
*****************************************************************/
#ifndef __ZMOS_CBTIMER_H__
#define __ZMOS_CBTIMER_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS_Types.h"
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
 * Callback timer function prototype.
 */
typedef void (* cbTimerFunction)(void *param);
/**
 * Callback timer id type.
 */
typedef uint8_t cbTimerId_t;
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
/*****************************************************************
* FUNCTION: zmos_cbTimerInit
*
* DESCRIPTION:
*     ZMOS callback timer initialize
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_cbTimerInit(void);
/*****************************************************************
* FUNCTION: zmos_startSingleCbtimer
*
* DESCRIPTION:
*     This function is called to start a single callback timer.
* INPUTS:
*     timerId : The callback timer id.
*     timeout : Timer timeout.
*     param : Param to be passed in to callback function.
*     cbfunc : Callback function.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_startSingleCbtimer(cbTimerId_t *timerId, uint32_t timeout, void *param, cbTimerFunction cbfunc);
/*****************************************************************
* FUNCTION: zmos_startReloadCbtimer
*
* DESCRIPTION:
*     This function is called to start a reload callback timer.
* INPUTS:
*     timerId : The callback timer id.
*     timeout : Timer timeout.
*     param : Param to be passed in to callback function.
*     cbfunc : Callback function.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_startReloadCbtimer(cbTimerId_t *timerId, uint32_t timeout, void *param, cbTimerFunction cbfunc);
/*****************************************************************
* FUNCTION: zmos_changeCbTimerTimeout
*
* DESCRIPTION:
*     This to change a callback timer timeout.
* INPUTS:
*     timerId : The callback timer id.
*     timeout : Timer new timeout.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_changeCbTimerTimeout(cbTimerId_t timerId, uint32_t timeout);
/*****************************************************************
* FUNCTION: zmos_stopCbtimer
*
* DESCRIPTION:
*     This function to stop a callback timer.
* INPUTS:
*     timerId : The callback timer id.
* RETURNS:
*     0 : success (ZMOS_TIMER_SUCCESS).
* NOTE:
*     null
*****************************************************************/
timerReslt_t zmos_stopCbtimer(cbTimerId_t timerId);

#ifdef __cplusplus
}
#endif
#endif /* ZMOS_Cbtimer.h */
