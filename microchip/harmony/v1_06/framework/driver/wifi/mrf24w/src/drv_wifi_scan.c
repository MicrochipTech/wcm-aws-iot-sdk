/*******************************************************************************
  MRF24W Driver Scan functions

  File Name:
    drv_wifi_scan.c

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

/*
*********************************************************************************************************
*                                           INCLUDES
*********************************************************************************************************
*/
#include "tcpip/src/tcpip_private.h"

#if defined(TCPIP_IF_MRF24W)
/*
*********************************************************************************************************
*                                           LOCAL GLOBALS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static bool isHostScanAllowed(void);

/*******************************************************************************
  Function:
    uint16_t DRV_WIFI_Scan(bool scanAll)

  Summary:
    Commands the MRF24W to start a scan operation.  This will generate the
    WF_EVENT_SCAN_RESULTS_READY event.

  Description:
    Directs the MRF24W to initiate a scan operation.  The Host Application will
    be notified that the scan results are ready when it receives the
    WF_EVENT_SCAN_RESULTS_READY event.  The eventInfo field for this event will
    contain the number of scan results.  Once the scan results are ready they
    can be retrieved with DRV_WIFI_ScanGetResult().

    Scan results are retained on the MRF24W until:
    1. Calling DRV_WIFI_Scan() again (after scan results returned from previous
         call).
    2. MRF24W reset.

  Parameters:
    scanAll -
            If false:
            * If SSID defined then only scan results with that SSID are retained.
            * If SSID not defined then all scanned SSID’s will be retained
            * Only scan results from Infrastructure or AdHoc networks are retained
            * The channel list that is scanned will be determined from the channels
              passed in via DRV_WIFI_ChannelListSet().

            If true:
            * All scan results are retained (both Infrastructure and Ad Hoc
               networks).
            * All channels within the MRF24W’s regional domain will be
               scanned.

  Returns:
    None.

  Remarks:
    Host scan can work only in the idle or connected state.
    To use this API safely, we recommend to disable MRF24W FW connection manager
    by setting WF_MODULE_CONNECTION_MANAGER == DRV_WIFI_DISABLED in system_config.h
  *****************************************************************************/
uint16_t DRV_WIFI_Scan(bool scanAll)
{
    uint8_t   hdr[4];

    /* Check if we can call host scan. Host scan can work only in the idle or connecte state */
    if (!isHostScanAllowed())
    {
        return DRV_WIFI_ERROR_OPERATION_CANCELLED;
    }

    hdr[0] = WF_MGMT_REQUEST_TYPE;
    hdr[1] = WF_SCAN_START_SUBTYPE;
    hdr[2] = (scanAll == true) ? 0xff : CPID;
    hdr[3] = 0;                   /* not used              */

    SendMgmtMsg(hdr,             /* header           */
                sizeof(hdr),     /* size of header   */
                NULL,            /* no data          */
                0);              /* no data          */

    /* wait for mgmt response, free it after it comes in (no data needed) */
    WaitForMgmtResponse(WF_SCAN_START_SUBTYPE, FREE_MGMT_BUFFER);

    return DRV_WIFI_SUCCESS;
}

/*******************************************************************************
  Function:
    void DRV_WIFI_ScanGetResult(uint8_t listIndex, t_wfScanResult  *p_scanResult)

  Summary:
    Read selected scan results back from MRF24W.

  Description:
    After a scan has completed this function is used to read one scan result at
    a time from the MRF24WG.

  Parameters:
    listIndex    - Index (0-based list) of the scan entry to retrieve.
    p_scanResult - Pointer to where scan result is written.  See DRV_WIFI_SCAN_RESULT
                   structure.

  Returns:
    None.
  *****************************************************************************/
void DRV_WIFI_ScanGetResult(uint8_t listIndex,
                            DRV_WIFI_SCAN_RESULT *p_scanResult)
{
    uint8_t   hdr[4];

    hdr[0] = WF_MGMT_REQUEST_TYPE;
    hdr[1] = WF_SCAN_GET_RESULTS_SUBTYPE;
    hdr[2] = listIndex;        /* scan result index to read from */
    hdr[3] = 1;                /* number of results to read      */

    SendMgmtMsg(hdr,             /* header           */
                sizeof(hdr),     /* size of header   */
                NULL,            /* no data          */
                0);              /* no data          */

    /* index 4 contains number of scan results returned, index 5 is first byte of first scan result */
    WaitForMgmtResponseAndReadData(WF_SCAN_GET_RESULTS_SUBTYPE,         /* expected subtype               */
                                   sizeof(DRV_WIFI_SCAN_RESULT),        /* num data bytes to read         */
                                   5,                                   /* starting at this index         */
                                  (uint8_t *)p_scanResult);             /* write the response data here   */

    /* fix up endianness on the two 16-bit values in the scan results */
    p_scanResult->beaconPeriod = TCPIP_Helper_ntohs(p_scanResult->beaconPeriod);
    p_scanResult->atimWindow   = TCPIP_Helper_ntohs(p_scanResult->atimWindow);
}

void WF_PreScanStart(void)
{
    uint8_t channelList[] = {}; // WF_PRESCAN
    DRV_WIFI_SCAN_CONTEXT  scanContext;

    WFInitScan();
    // WF_PRESCAN: Pre-scan before starting up as SoftAP mode
    scanContext.scanType       = DRV_WIFI_PASSIVE_SCAN;
    scanContext.minChannelTime = DRV_WIFI_DEFAULT_SCAN_MIN_CHANNEL_TIME;
    scanContext.maxChannelTime = DRV_WIFI_DEFAULT_SCAN_MAX_CHANNEL_TIME;
    scanContext.scanCount      = DRV_WIFI_DEFAULT_SCAN_COUNT;
    scanContext.probeDelay     = DRV_WIFI_DEFAULT_SCAN_PROBE_DELAY;
    DRV_WIFI_ScanContextSet(&scanContext);
    DRV_WIFI_ChannelListSet(channelList, sizeof(channelList));

    WFStartScan();
}

/*******************************************************************************
  Function:
    bool isHostScanAllowed(void)

  Summary:
    Determines if MRF24WG can support a scan request

  Description:
    Called by DRV_WIFI_Scan().  MRF24WG cannot scan when in midst of connection process.
    This function checks for that condition.

  Precondition:
    None

  Parameters:
    None

  Returns:
    True if OK to scan, else False

  Remarks:
    None.
  *****************************************************************************/
static bool isHostScanAllowed(void)
{
    uint8_t connectionState;

    DRV_WIFI_ConnectionStateGet(&connectionState);
    if (connectionState == DRV_WIFI_CSTATE_CONNECTION_IN_PROGRESS || connectionState == DRV_WIFI_CSTATE_RECONNECTION_IN_PROGRESS)
    {
        return false;
    }

    return true;
}

#endif /* TCPIP_IF_MRF24W */

//DOM-IGNORE-END
