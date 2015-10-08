/*******************************************************************************
  MRF24W Driver

  File Name:
    drv_wifi_easy_config.c

  Summary:
    Module for Microchip TCP/IP Stack

  Description:
    -Provides access to MRF24W WiFi controller
    -Reference: MRF24W Data sheet, IEEE 802.11 Standard
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
#include <ctype.h>
#include "drv_wifi_easy_config.h"

typedef enum
{
    EZ_WAIT_FOR_START = 0,
    EZ_WAIT_FOR_DELAY = 1,
    EZ_MIDDLE_START   = 2,
} t_EasyConfigStates;

/* Easy Config Private Functions */
static void EasyConfigConnect(void);
static void EasyConfigTimerHandler(uintptr_t, uint32_t);

void WFEasyConfigInit()
{
    EZ_CFGCXT.ssid[0] = 0;
    EZ_CFGCXT.security = DRV_WIFI_SECURITY_OPEN;
    EZ_CFGCXT.key[0] = 0;
    EZ_CFGCXT.defaultWepKey = WF_WEP_KEY_INVALID;
    EZ_CFGCXT.type = DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE;
    EZ_CFGCXT.state = EZ_WAIT_FOR_START;
}

void WiFi_EasyConfigTask()
{
    uint16_t timeout;

    switch (EZ_CFGCXT.state) {
    case EZ_WAIT_FOR_START:
        // first thing to do is delay one second so user can see on the web
        // page that disconnect is about occur in process of connecting to
        // designated AP.  So create and start timer.
        timeout = SYS_TMR_TickCounterFrequencyGet() * WF_EASY_CONFIG_DELAY_TIME;
        EZ_CFGCXT.timer = SYS_TMR_CallbackSingle(timeout, 0, EasyConfigTimerHandler);
        EZ_CFGCXT.state = EZ_MIDDLE_START;
        break;
    case EZ_MIDDLE_START:
        break;
    case EZ_WAIT_FOR_DELAY:
        // connect to AP that user selected on web page
        EasyConfigConnect();
        EZ_CFGCXT.state = EZ_WAIT_FOR_START;

        break;
    }
}

static void EasyConfigTimerHandler(uintptr_t context, uint32_t currTick)
{
    //it is SYS_TMR_CallbackSingle, only once, we don't have to stop it
    //SYS_TMR_CallbackStop(EZ_CFGCXT.timer);

    // This function being called from timer interrupt, so don't do any work
    // here, but, schedule the async event handler to call EasyConfigStateMachine
    EZ_CFGCXT.state = EZ_WAIT_FOR_DELAY;
    WiFiAsyncSetEventPending(ASYNC_EASY_CONFIG_PENDING);
}

static void EasyConfigConnect(void)
{
    // ToDo: here we let Adhoc work as Software mode, reset WiFi module,re-initialize it, then connect to AP
    // SoftAP: To allow redirection, need to hibernate before changing network type. Module
    //         FW has SoftAP flag and therefore hibernate mode is needed to clear this
    //         indication and allow proper network change. This should work for non-SoftAP,
    //         but these have not been tested yet.
    int ret_init;

    WF_InitForSoftApReDirection_enable();

    do
    {
        ret_init = WF_InitForSoftApReDirection();
    } while (ret_init == TCPIP_MAC_RES_PENDING);
}

void WFInitScan(void)
{
    SCANCXT.scanState = 0;
    SCANCXT.numScanResults = 0;
    SCANCXT.displayIdx = 0;
}

uint16_t WFStartScan(void)
{
    /* If scan already in progress bail out */
    if (IS_SCAN_IN_PROGRESS(SCANCXT.scanState))
    {
        return DRV_WIFI_ERROR_OPERATION_CANCELLED;
    }
    if (DRV_WIFI_Scan(true) != DRV_WIFI_SUCCESS)
    {
        return DRV_WIFI_ERROR_OPERATION_CANCELLED;
    }
    SCAN_SET_IN_PROGRESS(SCANCXT.scanState);

    return DRV_WIFI_SUCCESS;
}

uint16_t WFRetrieveScanResult(uint8_t Idx, DRV_WIFI_SCAN_RESULT *p_ScanResult)
{
    if (Idx >= SCANCXT.numScanResults)
    {
        return DRV_WIFI_ERROR_INVALID_PARAM;
    }

    DRV_WIFI_ScanGetResult(Idx, p_ScanResult);
    //p_ScanResult->ssid[p_ScanResult->ssidLen] = 0; /* Terminate */

    return DRV_WIFI_SUCCESS;
}

void WFScanEventHandler(uint16_t scanResults)
{
    /* Clear the scan in progress flag */
    SCAN_CLEAR_IN_PROGRESS(SCANCXT.scanState);

    SCAN_SET_VALID(SCANCXT.scanState);

    SCAN_SET_DISPLAY(SCANCXT.scanState);
    SCANCXT.numScanResults = scanResults; /* Cache number APs found in scan */
    SCANCXT.displayIdx = 0;
}

void WFDisplayScanMgr(void)
{
    DRV_WIFI_SCAN_RESULT bssDesc;
    char ssid[32 + 1];
    int i;

    if ((SCANCXT.numScanResults == 0)               ||
        (!IS_SCAN_STATE_DISPLAY(SCANCXT.scanState)) ||
        (IS_SCAN_IN_PROGRESS(SCANCXT.scanState))    ||
        (!IS_SCAN_STATE_VALID(SCANCXT.scanState)))
    {
       SCAN_CLEAR_DISPLAY(SCANCXT.scanState);
       return;
    }

    WFRetrieveScanResult(SCANCXT.displayIdx, &bssDesc);

    memset(ssid, ' ', sizeof(ssid));

    /* Display SSID */
    for (i = 0; i < bssDesc.ssidLen; i++)
    {
        if (!isprint(bssDesc.ssid[i]))
        {
            ssid[i] = '*';
        }
        else
        {
            ssid[i] = bssDesc.ssid[i];
        }
    }

    ssid[32] = 0;

    /* Display Index & SSID */
    //SYS_CONSOLE_PRINT("%2d)  ",SCANCXT.displayIdx + 1);
    //SYS_CONSOLE_PRINT("%s", ssid);

    /* Display RSSI & Channel */
    /* RSSI_MAX : 128, RSSI_MIN : 43 */
    //SYS_CONSOLE_PRINT(" %2u    %u\r\n", bssDesc.rssi, bssDesc.channel);

    SYS_CONSOLE_PRINT(" %2d) %s %2u    %u\r\n", SCANCXT.displayIdx + 1, ssid, bssDesc.rssi, bssDesc.channel);

    if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
    {
        preScanResult[SCANCXT.displayIdx] = bssDesc; // WF_PRESCAN
        if (SCANCXT.displayIdx == sizeof(preScanResult) / sizeof(preScanResult[0]) - 1)
        {
            /* Prescan is finished, re-initialize the scan context state. Scan is not allowed in Soft AP mode.
             * However, SCANCXT.numScanResults needs to be preserved for webpage using.
             */
            SCAN_CLEAR_VALID(SCANCXT.scanState);
            SCAN_CLEAR_DISPLAY(SCANCXT.scanState);
            SCANCXT.displayIdx = 0;
        }
    }
    else
    {
        #if (DRV_WIFI_DEFAULT_ADHOC_PRESCAN == DRV_WIFI_ENABLED)
        preScanResult[SCANCXT.displayIdx] = bssDesc; // WF_PRESCAN
        if (SCANCXT.displayIdx == sizeof(preScanResult) / sizeof(preScanResult[0]) - 1)
        {
            SCAN_CLEAR_DISPLAY(SCANCXT.scanState);
            SCANCXT.displayIdx = 0;
        }
        #endif
    }

    if (++SCANCXT.displayIdx == SCANCXT.numScanResults)
    {
        if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
        {
            /* Prescan is finished, re-initialize the scan context state. Scan is not allowed in Soft AP mode.
             * However, SCANCXT.numScanResults needs to be preserved for webpage using.
             */
            SCAN_CLEAR_VALID(SCANCXT.scanState);
            SCAN_CLEAR_DISPLAY(SCANCXT.scanState);
            SCANCXT.displayIdx = 0;
        }
        else
        {
            SCAN_CLEAR_DISPLAY(SCANCXT.scanState);
            SCANCXT.displayIdx = 0;
        }
    }
}

#endif /* TCPIP_IF_MRF24W */

//DOM-IGNORE-END
