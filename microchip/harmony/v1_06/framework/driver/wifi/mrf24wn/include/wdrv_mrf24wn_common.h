/*************************************************************************
  Company: 
    Microchip Technology Inc.
  
  File Name:
    wdrv_mrf24wn_common.h
	
  Summary:
    WDRV common header file
	
  Description:
    WDRV common header file
                                                     
  *************************************************************************/

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
// DOM-IGNORE-END

#ifndef _WDRV_MRF24WN_COMMON_H
#define _WDRV_MRF24WN_COMMON_H

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "osal/osal.h"
#include "system/tmr/sys_tmr.h"
#include "tcpip/src/tcpip_private.h"

#include "wdrv_mrf24wn_priv.h"

#if !defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
    #error "TCPIP_STACK_USE_EVENT_NOTIFICATION must be defined for Wi-Fi demos"
#endif

#define WDRV_NETWORK_TYPE_INFRASTRUCTURE          1
#define WDRV_NETWORK_TYPE_ADHOC                   2    /* Unsupported for now. */
#define WDRV_NETWORK_TYPE_P2P                     3    /* Unsupported for now. */
#define WDRV_NETWORK_TYPE_SOFT_AP                 4    /* Unsupported for now. */

// Do not make this an enumerated type!
#define WDRV_FUNC_DISABLED     0
#define WDRV_FUNC_ENABLED      1

#define WDRV_MAX_SSID_LENGTH                32
#define WDRV_BSSID_LENGTH                   6

#define WDRV_MAX_SECURITY_KEY_LENGTH        64

/* Key size defines */
#define WDRV_MIN_WPA_PASS_PHRASE_LENGTH     8
#define WDRV_MAX_WPA_PASS_PHRASE_LENGTH     64 // must include string terminator
#define WDRV_MAX_WPA2_PASS_PHRASE_LENGTH    64

// WEP key lengths
#define WDRV_WEP40_KEY_LENGTH               20        // 4 keys of 5 bytes each
#define WDRV_WEP104_KEY_LENGTH              52        // 4 keys of 13 bytes each
#define WDRV_MAX_WEP_KEY_LENGTH             WDRV_WEP104_KEY_LENGTH

// WPS PIN Length
#define WF_WPS_PIN_LENGTH                       8

#define WDRV_SECURITY_OPEN                        0
#define WDRV_SECURITY_WEP_40                      1
#define WDRV_SECURITY_WEP_104                     2
#define WDRV_SECURITY_WPA_WITH_PASS_PHRASE        3
#define WDRV_SECURITY_WPA2_WITH_PASS_PHRASE       4
#define WDRV_SECURITY_WPS_PUSH_BUTTON             6
#define WDRV_SECURITY_WPS_PIN                     7

#define WDRV_WPS_PIN_LENGTH                       8         // 7 digits + checksum byte

#define MAX_ALLOWED_BAUD_RATE 25000000
#define WDRV_SPI_BAUDRATE 1000000

typedef int32_t * WDRV_MUTEX_HANDLE_TYPE;

#define WDRV_QUEUE_SUCCESS 0
#define WDRV_QUEUE_EMPTY 1
#define WDRV_QUEUE_FULL 2
#define WDRV_QUEUE_FAILURE -1

/*******************************************************************************
  Summary:
    Contains data pertaining to Wi-Fi scan results
	
  Description:
    Wi-Fi Scan Results
    
    This structure contains the result of Wi-Fi scan operation. See
    WDRV_CLI_ScanGetResult().
    
    apConfig Bit Mask
    <table>
    Bit 7   Bit 6   Bit 5      Bit 4     Bit 3      Bit 2      Bit 1      Bit 0
    ------  ------  ---------  --------  ---------  ---------  ---------  ------
    WPA2    WPA     Preamble   Privacy   Reserved   Reserved   Reserved   IE
    </table>
    
    <table>
    IE         1 if AP broadcasting one or more Information Elements, else 0
    Privacy    0 : AP is open (no security) 1: AP using security, if neither WPA
                and WPA2 set then security is WEP.
    Preamble   0: AP transmitting with short preamble 1: AP transmitting with long preamble
    WPA        Only valid if Privacy is 1. 0: AP does not support WPA 1: AP
                supports WPA
    WPA2       Only valid if Privacy is 1. 0: AP does not support WPA2 1: AP supports WPA2
    </table>
                                                                                
*/
typedef struct
{
    /* Network BSSID value */
    uint8_t      bssid[6];

    /*  Network SSID value */
    uint8_t      ssid[32]; 

    /* Access Point config (see description) */
    uint8_t      apConfig;

    /* not used */
    uint8_t      reserved;

    /* Network beacon interval */
    uint16_t     beaconPeriod;

    /* Only valid if bssType = WDRV_NETWORK_TYPE_INFRASTRUCTURE */
    uint16_t     atimWindow;   

    /*
      List of Network basic rates.  Each rate has the following format:
      
          Bit 7
      * 0: rate is not part of the basic rates set
      * 1: rate is part of the basic rates set

          Bits 6:0
      Multiple of 500kbps giving the supported rate.  For example, a value of 2 
      (2 * 500kbps) indicates that 1mbps is a supported rate.  A value of 4 in 
      this field indicates a 2mbps rate (4 * 500kbps).
      */
    uint8_t      basicRateSet[8];
    
    uint8_t      extRateSet[4];
    
    /* Signal strength of received frame beacon or probe response.  Will range
       from a low of 43 to a high of 128. */
    uint8_t      rssi;          

    /* Number of valid rates in basicRates */
    uint8_t      numRates;

    /* Part of TIM element */
    uint8_t      DtimPeriod;

    /* WDRV_NETWORK_TYPE_INFRASTRUCTURE or WDRV_NETWORK_TYPE_ADHOC */
    uint8_t      bssType;

    /* Channel number */
    uint8_t      channel;

    /* Number of valid characters in ssid */
    uint8_t      ssidLen;       

} WDRV_SCAN_RESULT;

enum {
    MZ_ESK_BD,
    MX_ESK_BD,
    EXP16_BD,
    MEB2_BD
};

typedef struct __attribute__((__packed__))
{
    uint8_t networkType;
    uint8_t netSSID[32];       // Wireless SSID
    uint8_t SsidLength;        // number of bytes in SSID
    uint8_t SecurityMode;      // WDRV_SECURITY_OPEN or one of the other security modes
    uint8_t defaultWepKey;     // WEP index value
    uint8_t SecurityKey[64];   // WiFi Security key, or passphrase.
    uint8_t SecurityKeyLength; // number of bytes in security key (can be 0)
    uint8_t wepKeyType;
    uint8_t dataValid;
    uint8_t saveSecurityInfo;  // Save 32-byte PSK
    union
    {
        struct
        {
            uint16_t bInterfaceEnabled            : 1;  // 0 when TCPIP_MAC_POWER_DOWN/TCPIP_MAC_POWER_LOW
            volatile uint16_t bNewTcpipEventAvlbl : 1;  // event available flag
            uint16_t                              : 1;  // unused
            uint16_t bIPv6InConfig                : 1;
            uint16_t bIPv6Enabled                 : 1;
            uint16_t bWFEasyConfig                : 1;  // NOTE : This was not present in 5.36
            uint16_t bIsDHCPEnabled               : 1;
            uint16_t bInConfigMode                : 1;
            uint16_t localAndType                 : 2;  // type of AND operation to be performed for local/nonlocal detection
            uint16_t localOrType                  : 2;  // type of OR operation to be performed for local/nonlocal detection
            uint16_t reserved                     : 4;  // not used
        };
        uint16_t v;
    }
    Flags; // Flags structure
} WDRV_CONFIG_DATA;

#define WDRV_ENABLE_INTR()              SYS_INT_Enable()
#define WDRV_DISABLE_INTR()             SYS_INT_Disable()
#define WDRV_TIME_DELAY(msec)           wdrv_usec_delay(msec * 1000)
#define WDRV_TASK_CREATE(func, alias, stackDepth, param, prio, handle) \
                                        wdrv_task_create(func, alias, stackDepth, param, prio, handle, 0) 
#define WDRV_TASK_DELETE(handle)        wdrv_task_destroy(handle)
#define WDRV_MALLOC(x)                  malloc((x))
#define WDRV_FREE(x)                    free((x))
#define WDRV_MUTEX_CREATE(handle)       wdrv_mutex_init(handle)
#define WDRV_MUTEX_LOCK(handle, waitMS) wdrv_mutex_acquire(handle, waitMS)
#define WDRV_MUTEX_UNLOCK(handle)       wdrv_mutex_release(handle)
#define WDRV_MUTEX_DELETE(handle)       wdrv_mutex_destroy(handle)
#define WDRV_DCACHE_FLUSH() 
#define WDRV_DCACHE_INVALIDATE()

#define WDRV_DBG_NONE     0
#define WDRV_DBG_ERROR    1
#define WDRV_DBG_INFORM   2
#define WDRV_DBG_TRACE    3
#define WDRV_DBG_VERBOSE  4
#define WDRV_DBG_LEVEL    WDRV_DBG_INFORM    

#if WDRV_DBG_LEVEL != WDRV_DBG_NONE
#define DEBUG_LOCK WDRV_MUTEX_LOCK(g_wdrv_priv.debugMutex, 0)
#define DEBUG_UNLOCK WDRV_MUTEX_UNLOCK(g_wdrv_priv.debugMutex)
#define WDRV_DBG_VERBOSE_MESSAGE(x) do { if (WDRV_DBG_LEVEL >= WDRV_DBG_VERBOSE) {DEBUG_LOCK; SYS_CONSOLE_MESSAGE x; DEBUG_UNLOCK;}} while (0)
#define WDRV_DBG_TRACE_MESSAGE(x) do { if (WDRV_DBG_LEVEL >= WDRV_DBG_TRACE) {DEBUG_LOCK; SYS_CONSOLE_MESSAGE x; DEBUG_UNLOCK;}} while (0)
#define WDRV_DBG_INFORM_MESSAGE(x) do { if (WDRV_DBG_LEVEL >= WDRV_DBG_INFORM) {DEBUG_LOCK; SYS_CONSOLE_MESSAGE x; DEBUG_UNLOCK;}} while (0)
#define WDRV_DBG_ERROR_MESSAGE(x) do { if (WDRV_DBG_LEVEL >= WDRV_DBG_ERROR) {DEBUG_LOCK; SYS_CONSOLE_MESSAGE x; DEBUG_UNLOCK;}} while (0)
#define WDRV_DBG_VERBOSE_PRINT(x) do { if (WDRV_DBG_LEVEL >= WDRV_DBG_VERBOSE) {DEBUG_LOCK; SYS_CONSOLE_PRINT x; DEBUG_UNLOCK;}} while (0)
#define WDRV_DBG_TRACE_PRINT(x) do { if (WDRV_DBG_LEVEL >= WDRV_DBG_TRACE) {DEBUG_LOCK; SYS_CONSOLE_PRINT x; DEBUG_UNLOCK;}} while (0)
#define WDRV_DBG_INFORM_PRINT(x) do { if (WDRV_DBG_LEVEL >= WDRV_DBG_INFORM) {DEBUG_LOCK; SYS_CONSOLE_PRINT x; DEBUG_UNLOCK;}} while (0)
#define WDRV_DBG_ERROR_PRINT(x) do { if (WDRV_DBG_LEVEL >= WDRV_DBG_ERROR) {DEBUG_LOCK; SYS_CONSOLE_PRINT x; DEBUG_UNLOCK;}} while (0)
#define WDRV_DBG_TRACE_MESSAGE_IN_ISR(x) do { if (WDRV_DBG_LEVEL >= WDRV_DBG_TRACE) SYS_CONSOLE_MESSAGE x; } while (0)
#else
#define WDRV_DBG_VERBOSE_MESSAGE(x) do {} while(0)
#define WDRV_DBG_TRACE_MESSAGE(x) do {} while(0)
#define WDRV_DBG_INFORM_MESSAGE(x) do {} while(0)
#define WDRV_DBG_ERROR_MESSAGE(x) do {} while(0)
#define WDRV_DBG_VERBOSE_PRINT(x) do {} while (0)
#define WDRV_DBG_TRACE_PRINT(x) do {} while (0)
#define WDRV_DBG_INFORM_PRINT(x) do {} while (0)
#define WDRV_DBG_ERROR_PRINT(x) do {} while (0)
#define WDRV_DBG_TRACE_MESSAGE_IN_ISR(x) do {} while (0)
#endif

typedef struct {
   void (*CopyFrameToStackPacketBuffer_CB)(uint32_t len, uint8_t const * const frame);
   void (*ProceedConnectEvent_CB)(int32_t connected, uint8_t devID, uint8_t *bssid, bool bssConn, uint8_t reason);
   void (*RFReady_CB)(uint8_t const * const addr);
   void (*ScanDone_CB)(int32_t status);
   void (*InitDone_CB)(void);
   void (*DeinitDone_CB)(void);
   void (*WPSDone_CB)(void);
} CallBacks;

#include "wdrv_mrf24wn_api.h"

void WDRV_Assert(int condition, const char * msg, const char * file, int line);

#endif /* _WDRV_MRF24WN_COMMON_H */
