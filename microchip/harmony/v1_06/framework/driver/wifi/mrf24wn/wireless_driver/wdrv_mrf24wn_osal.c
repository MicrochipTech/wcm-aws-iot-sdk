/*******************************************************************************
  MRF24WN OS Abstraction Layer

  File Name:
    wdrv_mrf24wn_osal.c

  Summary:
    Module for Microchip TCP/IP Stack

  Description:
    MRF24WN OS Abstraction Layer
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

#include "wdrv_mrf24wn_common.h"

#if defined(TCPIP_IF_MRF24WN)
#if defined(WIFI_USE_RTOS)

uint32_t wdrv_mutex_init(OSAL_MUTEX_HANDLE_TYPE **mutex_ptr)
{
	*mutex_ptr = malloc(sizeof(OSAL_MUTEX_HANDLE_TYPE));
	OSAL_MUTEX_Create(*mutex_ptr) ;
    return 0;
}

uint32_t wdrv_mutex_destroy(OSAL_MUTEX_HANDLE_TYPE **mutex_ptr)
{
    OSAL_MUTEX_Delete(*mutex_ptr);   
    free(*mutex_ptr);
    return 0;
}

uint32_t wdrv_mutex_acquire(OSAL_MUTEX_HANDLE_TYPE *mutex_ptr, uint32_t tick_count)
{
    OSAL_MUTEX_Lock(mutex_ptr, OSAL_WAIT_FOREVER);
    return 0;
}

uint32_t wdrv_mutex_release(OSAL_MUTEX_HANDLE_TYPE *mutex_ptr)
{
    OSAL_MUTEX_Unlock(mutex_ptr);
    return 0;
}

WDRV_STATUS wdrv_get_error_code(uint32_t os_err)
{
    WDRV_STATUS osal_ret;    

    switch(os_err)
    {
    case pdPASS:
        osal_ret = WDRV_SUCCESS;
        break;

    case pdFAIL:
        osal_ret = WDRV_ERROR;
        break;

    default:
        osal_ret = WDRV_ERROR;
    }

    return osal_ret;
}

WDRV_STATUS wdrv_task_create(void Task(void *), char *task_name, int stack_size, void *param, 
    unsigned long task_priority, TaskHandle_t *task_handle, bool auto_start)
{
    WDRV_STATUS ret = WDRV_OK;
    uint32_t os_ret;
    
    os_ret = xTaskCreate((TaskFunction_t)Task, /* pointer to the task entry code */
                         task_name,            /* task name */
                         stack_size,           /* task stack size */
                         param,                /* parameters to pass */
                         task_priority,        /* task priority - lower no= lower priority */
                         task_handle);         /* handle by which task can be referenced */

    if(os_ret == pdFAIL)
    {         
         return WDRV_OK;
    } 

    ret = wdrv_get_error_code(os_ret);

    return ret;
}

WDRV_STATUS wdrv_task_destroy(TaskHandle_t task_handle)
{
    WDRV_STATUS ret = WDRV_OK;

    vTaskDelete(task_handle);
    return ret;
}

void wdrv_usec_delay(uint32_t uSec)
{  
    unsigned long xUnblockPeriod = uSec/1000;

    if(!xUnblockPeriod)
    {
        xUnblockPeriod = 1;
    }    

    vTaskDelay(xUnblockPeriod * portTICK_RATE_MS);

    return;
}

#endif /* defined(WIFI_USE_RTOS) */
#endif /* defined(TCPIP_IF_MRF24WN) */

//DOM-IGNORE-END
