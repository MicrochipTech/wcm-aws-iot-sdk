/*******************************************************************************
  MRF24W RTOS wrapper functions file

  File Name:
    drv_wifi_rtos_wrapper.c

  Summary:
    Module for Microchip TCP/IP Stack

  Description:
    RTOS wrapper functions for WiFi MRF24W module
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/

#include "tcpip/src/tcpip_private.h"

#if defined(TCPIP_IF_MRF24W)

#if defined(WIFI_USE_RTOS)
#include "drv_wifi_easy_config.h"

static OSAL_SEM_HANDLE_TYPE s_InitSem;
static OSAL_SEM_HANDLE_TYPE s_MacTaskSem;
static OSAL_SEM_HANDLE_TYPE s_DeferredISRSem;
static OSAL_MUTEX_HANDLE_TYPE s_SPIMutex;

////////////////////////////////////////////////////////////////////////////////
// Initialization State Machine Task

bool WF_Init_SemInit(void)
{
    if (OSAL_SEM_Create(&s_InitSem, OSAL_SEM_TYPE_COUNTING, 1, 0) == OSAL_RESULT_TRUE) 
        return true;
    else                                                                                             
        return false;
}

void WF_Init_SemTake(void)
{
    OSAL_SEM_Pend(&s_InitSem, OSAL_WAIT_FOREVER);
}

void WF_Init_SemGive(void)
{
    OSAL_SEM_Post(&s_InitSem);
}


////////////////////////////////////////////////////////////////////////////////
// SPI MUTEX
bool WF_SPI_MUTEX_Init(void)
{
    if (OSAL_MUTEX_Create(&s_SPIMutex) == OSAL_RESULT_TRUE) 
        return true;
    else                                                      
        return false;
}

bool WF_SPI_MUTEX_Lock(void)
{
    if (OSAL_MUTEX_Lock(&s_SPIMutex, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE) 
        return true;
    else                                                                       
        return false;
}

void WF_SPI_MUTEX_Unlock(void)
{
    OSAL_MUTEX_Unlock(&s_SPIMutex);
}

// ISR Task


bool DRV_WIFI_DeferredISR_SemInit(void)
{
    if (OSAL_SEM_Create(&s_DeferredISRSem, OSAL_SEM_TYPE_COUNTING, 1, 0) == OSAL_RESULT_TRUE)
        return true;
    else
        return false;
}

void DRV_WIFI_DeferredISR_SemTake(void)
{
    OSAL_SEM_Pend(&s_DeferredISRSem, OSAL_WAIT_FOREVER);
}
void DRV_WIFI_DeferredISR_SemGive(void)
{
    OSAL_SEM_PostISR(&s_DeferredISRSem);
}

void DRV_WIFI_TaskDestroy(TaskHandle_t task_handle)
{
    vTaskDelete(task_handle);
}


////////////////////////////////////////////////////////////////////////////////
// MAC Process Task

bool WF_MACTask_SemInit(void)
{
    if (OSAL_SEM_Create(&s_MacTaskSem, OSAL_SEM_TYPE_COUNTING, 1, 0) == OSAL_RESULT_TRUE)
        return true;
    else
        return false;
}

void WF_MACTask_SemTake(void)
{
    OSAL_SEM_Pend(&s_MacTaskSem, OSAL_WAIT_FOREVER);
}

void WF_MACTask_SemGive(void)
{
    OSAL_SEM_Post(&s_MacTaskSem);
}

////////////////////////////////////////////////////////////////////////////////
// Tasks Initialization
bool DRV_WIFI_RTOS_TaskInit(void)
{
    if (WF_SPI_MUTEX_Init() == false)
        return false;

    if (WF_MACTask_SemInit() == false)
    {
      SYS_CONSOLE_MESSAGE("WF_MACTask_SemInit() failed\r\n");
      return false;
    }

    if (DRV_WIFI_DeferredISR_SemInit() == false)
    {
        SYS_CONSOLE_MESSAGE("DRV_WIFI_DeferredISR_SemInit() failed\r\n");
        return false;
    }

    if (WF_Init_SemInit() == false)
    {
      SYS_CONSOLE_MESSAGE("WF_Init_SemInit() failed\r\n");
      return false;
    }

    return true;
}

#endif /* defined(WIFI_USE_RTOS) */

#endif /* defined(TCPIP_IF_MRF24W) */

//DOM-IGNORE-END
