/*******************************************************************************
  WiFi MAC interface functions

  File Name:
    wdrv_mrf24wn_priv.h

  Summary:
   WiFi-specific MAC function prototypes called by TCP/IP stack.

  Description:
    The functions in this header file are accessed by the TCP/IP stack via
    function pointers.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright ?2012 released Microchip Technology Inc. All rights reserved.

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

#ifndef _WDRV_MRF24WN_PRIV_H
#define _WDRV_MRF24WN_PRIV_H

#include "tcpip/tcpip_mac_object.h"
#include "tcpip/src/tcpip_private.h"

typedef struct
{
    uint8_t trafficEvents;
    uint16_t trafficEventInfo;
} WDRV_MRF24WN_USREV_DCPT;

// stack internal notification
typedef struct
{
    bool                     mrfEnabledEvents; // group enabled notification events
    volatile TCPIP_MAC_EVENT mrfPendingEvents; // group notification events that are set, waiting to be re-acknowledged
    TCPIP_MAC_EventF         mrfNotifyFnc; // group notification handler
    const void              *mrfNotifyParam; // notification parameter
} WDRV_MRF24WN_EVGROUP_DCPT; // event descriptor

typedef struct {
    volatile int32_t initDriverDone;
    volatile int32_t deinitDriverDone;
    volatile uint32_t updateMacAddressRequired;
    uint8_t macAddr[6];
    OSAL_MUTEX_HANDLE_TYPE *debugMutex;
} WDRV_MRF24WN_PRIV;

typedef struct {
   void (*CopyFrameToStackPacketBuffer_CB)(uint32_t len, uint8_t const * const frame);
   void (*ProceedConnectEvent_CB)(int connected, uint8_t devID, uint8_t *bssid, bool bssConn, uint8_t reason);
   void (*RFReady_CB)(uint8_t const * const addr);
   void (*ScanDone_CB)(int32_t status);
   void (*InitDone_CB)(void);
   void (*DeinitDone_CB)(void);
   void (*WPSDone_CB)(void);
   uint8_t address[6];
   uint32_t versions[2];
} WDRV_MRF24WN_UPPER_CTX;

typedef enum 
{  
    WDRV_ERROR                = -1,
    WDRV_OK                   = 0,
    WDRV_SUCCESS              = 0,
    WDRV_INVALID_TASK_ID      = 2,
    WDRV_INVALID_PARAMETER    = 3,
    WDRV_INVALID_POINTER      = 4,
    WDRV_ALREADY_EXISTS       = 5,
    WDRV_INVALID_EVENT        = 6,
    WDRV_EVENT_TIMEOUT        = 7,
    WDRV_INVALID_MUTEX        = 8,
    WDRV_TASK_ALREADY_LOCKED  = 9,
    WDRV_MUTEX_ALREADY_LOCKED = 10,  
    WDRV_OUT_OF_MEMORY        = 11,
} WDRV_STATUS;

uint32_t wdrv_mutex_init(OSAL_MUTEX_HANDLE_TYPE **mutex_ptr);
uint32_t wdrv_mutex_destroy(OSAL_MUTEX_HANDLE_TYPE **mutex_ptr);
uint32_t wdrv_mutex_acquire(OSAL_MUTEX_HANDLE_TYPE *mutex_ptr, uint32_t tick_count);
uint32_t wdrv_mutex_release(OSAL_MUTEX_HANDLE_TYPE *mutex_ptr);
WDRV_STATUS wdrv_get_error_code(uint32_t os_err);
WDRV_STATUS wdrv_task_create(void Task(void *), char *task_name, int stack_size, void *param, 
    unsigned long task_priority, TaskHandle_t *task_handle, bool auto_start);
WDRV_STATUS wdrv_task_destroy(TaskHandle_t task_handle);
void wdrv_usec_delay(uint32_t uSec);

void AsyncEventHandle(void);
bool AsyncEventPended(void);

extern WDRV_MRF24WN_PRIV g_wdrv_priv;
extern bool g_module_event_print;
extern char *g_ota_filename;
extern uint32_t g_ota_host_addr;
extern const char *g_ota_host_addr_str;
extern uint32_t g_wdrv_init_task_priority; 
extern uint32_t g_wdrv_main_task_priority;
extern uint32_t g_wdrv_init_task_stack_size; 
extern uint32_t g_wdrv_main_task_stack_size;

#endif /*_WDRV_MRF24WN_PRIV_H */

// DOM-IGNORE-END
