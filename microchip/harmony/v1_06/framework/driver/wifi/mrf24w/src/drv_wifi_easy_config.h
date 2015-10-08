/*******************************************************************************
  WiFi MAC interface functions

  File Name:
    drv_wifi_easy_config.h

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

#ifndef _DRV_WIFI_EASY_CONFIG_H
#define _DRV_WIFI_EASY_CONFIG_H

typedef struct
{
    uint8_t configBits;      /* used to dictate MAC behavior following the calculation */
    uint8_t phraseLen;       /* number of valid bytes in passphrase */
    uint8_t ssidLen;         /* number of valid bytes in SSID */
    uint8_t reserved;        /* alignment byte */
    int8_t ssid[32 + 1];       /* the string of characters representing the SSID */
    int8_t passPhrase[64 + 1]; /* the string of characters representing the pass phrase */
} t_wfPskCalcReq;

typedef struct {
    uint8_t            ssid[32 + 1];        // 32 for SSID and one for term character
    uint8_t            type;              // net type: infrastructure/adhoc
    uint8_t            prevSSID[32 + 1];    // previous SSID we were connected to
    uint8_t            prevWLAN;          // previous WLAN type

    uint8_t            security;          // security type
    uint8_t            defaultWepKey;     // WEP index value
    uint8_t            key[64 + 1];         // 64-byte key plus term character
    uint8_t            SecurityKeyLength; // number of bytes in security key (can be 0)
    uint32_t           timeStart;
    SYS_TMR_HANDLE     timer;             // used for delay
    uint8_t            state;             // state of EasyConfig state machine
} t_wfEasyConfigCtx;

t_wfEasyConfigCtx    g_easyConfigCtx;
#define EZ_CFGCXT    g_easyConfigCtx

typedef struct
{
    int8_t    scanState;
    uint16_t  numScanResults;
    uint16_t  displayIdx;
} DRV_WIFI_EZ_CONFIG_SCAN_CONTEXT;

DRV_WIFI_EZ_CONFIG_SCAN_CONTEXT    g_ScanCtx;
#define SCANCXT    g_ScanCtx

DRV_WIFI_SCAN_RESULT preScanResult[50]; //WF_PRESCAN // May change struct later for memory optimization

/* Easy Config Public Functions */
void WFEasyConfigInit(void);

void WFInitScan(void);

void WFScanEventHandler(uint16_t scanResults);

void WFDisplayScanMgr(void);

uint16_t WFStartScan(void);

uint16_t WFRetrieveScanResult(uint8_t Idx, DRV_WIFI_SCAN_RESULT *p_ScanResult);

void WF_InitForSoftApReDirection_enable(void);
int WF_InitForSoftApReDirection(void);

/* Macros */
#define WF_WEP_KEY_INVALID 0xff

/* Definitions for stall feature of state machine */
#define WF_EASY_CONFIG_DELAY_TIME (1ul) /* In seconds */

/* Scan status/control bits */
#define SCAN_STATE_IN_PROGRESS  0x0001 /* If scan is in progress */
#define SCAN_STATE_VALID_RESULTS  0x0002 /* If we have the valid scan results */
#define SCAN_STATE_DISPLAY_RESULTS  0x0004 /* This flag is only used to control WFDisplayScanMgr() */

#define IS_SCAN_IN_PROGRESS(x)       ((x) & SCAN_STATE_IN_PROGRESS)
#define IS_SCAN_STATE_VALID(x)       ((x) & SCAN_STATE_VALID_RESULTS)
#define IS_SCAN_STATE_DISPLAY(x)     ((x) & SCAN_STATE_DISPLAY_RESULTS)
#define SCAN_SET_IN_PROGRESS(x)      ((x) |= SCAN_STATE_IN_PROGRESS)
#define SCAN_SET_VALID(x)            ((x) |= SCAN_STATE_VALID_RESULTS)
#define SCAN_SET_DISPLAY(x)          ((x) |= SCAN_STATE_DISPLAY_RESULTS)
#define SCAN_CLEAR_IN_PROGRESS(x)    ((x) &= ~SCAN_STATE_IN_PROGRESS)
#define SCAN_CLEAR_VALID(x)          ((x) &= ~SCAN_STATE_VALID_RESULTS)
#define SCAN_CLEAR_DISPLAY(x)        ((x) &= ~SCAN_STATE_DISPLAY_RESULTS)

#endif /* _DRV_WIFI_EASY_CONFIG_H */

// DOM-IGNORE-END
