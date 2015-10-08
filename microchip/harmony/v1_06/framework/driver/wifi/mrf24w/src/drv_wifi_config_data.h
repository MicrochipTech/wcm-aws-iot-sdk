/*******************************************************************************
  WiFi MAC interface functions

  File Name:
    drv_wifi_config_data.h

  Summary:
   WiFi-specific MAC function prototypes called by TCP/IP stack.

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

#ifndef _DRV_WIFI_CONFIG_DATA_H
#define _DRV_WIFI_CONFIG_DATA_H

//============================================================================
//                                  Data Types
//============================================================================

typedef struct __attribute__((__packed__))
{
    uint32_t verify_flag; // 0x00000000: empty;    0xffffffff: empty;    0x5a5a5a5a: verified.
    uint8_t networkType;
    uint8_t netSSID[DRV_WIFI_MAX_SSID_LENGTH];             // Wireless SSID
    uint8_t SsidLength;                                    // number of bytes in SSID
    uint8_t SecurityMode;                                  // DRV_WIFI_SECURITY_OPEN or one of the other security modes
    uint8_t defaultWepKey;                                 // WEP index value
    uint8_t SecurityKey[DRV_WIFI_MAX_SECURITY_KEY_LENGTH]; // WiFi Security key, or passphrase.
    uint8_t SecurityKeyLength;                             // number of bytes in security key (can be 0)
    uint8_t wepKeyType;
    uint8_t dataValid;
    uint8_t saveSecurityInfo;                              // Save 32-byte PSK
    union
    {
        struct
        {
            uint16_t bInterfaceEnabled            : 1; // 0 when TCPIP_MAC_POWER_DOWN/TCPIP_MAC_POWER_LOW
            volatile uint16_t  bNewTcpipEventAvlbl: 1; // event available flag
            uint16_t                              : 1; // unused
            uint16_t bIPv6InConfig                : 1;
            uint16_t bIPv6Enabled                 : 1;
            uint16_t bWFEasyConfig                : 1; // NOTE : This was not present in 5.36
            uint16_t bIsDHCPEnabled               : 1;
            uint16_t bInConfigMode                : 1;
            uint16_t localAndType                 : 2; // type of AND operation to be performed for local/nonlocal detection
            uint16_t localOrType                  : 2; // type of OR operation to be performed for local/nonlocal detection
            uint16_t reserved                     : 4; // not used
        };
        uint16_t v;
    }
    Flags; // Flags structure
    uint16_t flag_WpsSaving;
} DRV_WIFI_CONFIG_DATA;

//============================================================================
//                                  Globals
//============================================================================
extern DRV_WIFI_CONFIG_DATA * p_wifi_ConfigData;

#endif // _DRV_WIFI_CONFIG_DATA_H

// DOM-IGNORE-END
