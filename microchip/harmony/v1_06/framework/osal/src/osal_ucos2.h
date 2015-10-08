/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                     (c) Copyright 2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                   Microchip OSAL 
*
* File    : OSAL_uCOS3.H
* By      : JJL
* Version : V3.03.02
*
* LICENSING TERMS:
* ---------------
*           uC/OS-III is provided in source form for FREE short-term evaluation, for educational use or 
*           for peaceful research.  If you plan or intend to use uC/OS-III in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-III for its use in your 
*           application/product.   We provide ALL the source code for your convenience and to help you 
*           experience uC/OS-III.  The fact that the source is provided does NOT mean that you can use 
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
************************************************************************************************************************
*/

#ifndef _OSAL_MICRIUM_H
#define _OSAL_MICRIUM_H

#define  OSAL_USE_RTOS

#ifdef __cplusplus
extern "C" {
#endif

/*
------------------------------------------------------------------------------------------------------------------------
*                                                    INCLUDE FILES
------------------------------------------------------------------------------------------------------------------------
*/

#include <xc.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*
------------------------------------------------------------------------------------------------------------------------
*                                                     DATA TYPES
------------------------------------------------------------------------------------------------------------------------
*/

#define OSAL_SEM_HANDLE_TYPE            OS_EVENT
#define OSAL_MUTEX_HANDLE_TYPE          OS_EVENT
#define OSAL_CRITSECT_DATA_TYPE         OS_CPU_SR
#define OSAL_WAIT_FOREVER               0xFFFF

#define OSAL_SEM_DECLARE(semID)         OS_EVENT  *semID
#define OSAL_MUTEX_DECLARE(mutexID)     OS_EVENT  *mutexID

/*
------------------------------------------------------------------------------------------------------------------------
*                                          INCLUDE 'osal.h' AFTER DATA TYPES
------------------------------------------------------------------------------------------------------------------------
*/

#include "osal/osal.h"


#ifdef __cplusplus
}
#endif

#endif // _OSAL_MICRIUM_H
