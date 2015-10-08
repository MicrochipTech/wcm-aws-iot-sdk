/*******************************************************************************
  Wi-Fi MAC interface functions

  File Name:
    drv_wifi_mac.h

  Summary:
   Wi-Fi specific MAC function prototypes called by TCP/IP stack.

  Description:
    The functions in this header file are accessed by the TCP/IP stack via
    function pointers.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright © 2012 released Microchip Technology Inc. All rights reserved.

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

#ifndef _DRV_WIFI_MAC_H
#define _DRV_WIFI_MAC_H

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#if defined(TCPIP_IF_MRF24W)
#include "tcpip/tcpip_mac_object.h"

#include "driver/wifi/mrf24w/drv_wifi.h"

#include "drv_wifi_mgmt_msg.h"
#include "drv_wifi_priv.h"
#include "drv_wifi_raw.h"
#include "drv_wifi_config_data.h"
#include "drv_wifi_debug_output.h"
#include "drv_wifi_easy_config.h"
#include "drv_wifi_spi.h"
#include "drv_wifi_softap_client_cache.h"

// MRF24W MAC descriptor
typedef struct
{
    const TCPIP_MAC_OBJECT* pObj;   // safe cast to TCPIP_MAC_DCPT

    // specific MRF24W MAC data
    TCPIP_NET_IF*          pNetIf;  // interface we belong to
    bool                   isInit;  // simple init status flag
    bool                   isOpen;  // simple open status flag
                                    // just one hardware module supported for now
    SYS_STATUS             sysStat; // driver status

    // add other MRF24W data here
} MRF24W_MAC_DCPT;

extern bool g_RxDataLock;
extern bool g_TxDataLock;


// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

// initialization function
TCPIP_MAC_RES MRF24W_MACInit(const TCPIP_MAC_MODULE_CTRL * const stackData, const TCPIP_MODULE_MAC_MRF24W_CONFIG * initData);

// interface functions
// TODO: Normally all the functions should have the hMac as a parameter
// (to be able to support multiple instances of the same MAC interface)
// For now, it was set as a parameter just where it was unavoidable
// TODO: add it everywhere!

void          MRF24W_MACPutHeader(TCPIP_MAC_ADDR *remote, uint16_t type, uint16_t dataLen);
bool          _MRF24W_MACCheckLink(void);
void          MRF24W_MACPowerDown(void);
void          MRF24W_MACPowerUp(void);

TCPIP_MAC_RES   MRF24W_MACEventInit(MRF24W_MAC_DCPT * pDcpt, TCPIP_MAC_EventF eventF, const void * eventParam, int intPri, int subPri);
TCPIP_MAC_RES   MRF24W_MACEventDeInit(MRF24W_MAC_DCPT * pDcpt);
bool            _MRF24W_MACEventSetMask(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents, bool enable);
bool            _MRF24W_MACEventAcknowledge(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents);
TCPIP_MAC_EVENT _MRF24W_MACEventGetPending(TCPIP_MAC_HANDLE hMac);
void            MRF24W_SetUserEvents(uint8_t event, uint16_t eventInfo, bool isMgmt);

#endif // defined(TCPIP_IF_MRF24W)

#ifdef __cplusplus
}
#endif

#endif /* _DRV_WIFI_MAC_H */

// DOM-IGNORE-END
