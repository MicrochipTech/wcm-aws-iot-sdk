/*******************************************************************************
  MRF24W Driver Connection Algorithm

  File Name:
    drv_wifi_connection_algorithm.c

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
*                                           DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           LOCAL DATA TYPES
*********************************************************************************************************
*/

/* header format for response to CP Get Element message */
typedef struct caElementResponseStruct
{
    tMgmtMsgRxHdr   mgmtHdr;                /* normal 4-byte hdr for all mgmt responses */
    uint8_t         elementId;              /* index 4 */
    uint8_t         elementDataLength;      /* index 5 */
    /* element data follows */
} tCAElementResponseHdr;

/*
*********************************************************************************************************
*                                           LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void LowLevel_CASetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength);

static void LowLevel_CAGetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength,
                                  uint8_t dataReadAction);

/*******************************************************************************
  Function:
    void DRV_WIFI_ScanContextSet(DRV_WIFI_SCAN_CONTEXT *p_context);

  Summary:
    Sets the WiFi scan context.

  Description:
    This function sets the WiFi scan context.  The MRF24WG defaults are fine for
    most applications, but they can be changed by this function.

  Parameters:
    p_context - Pointer to scan context.  See DRV_WIFI_SCAN_CONTEXT structure.

  Returns:
    None.
  *****************************************************************************/
void DRV_WIFI_ScanContextSet(DRV_WIFI_SCAN_CONTEXT *p_context)
{
    uint16_t tmp;

    LowLevel_CASetElement(WF_CA_ELEMENT_SCANTYPE,           // Element ID
                          &p_context->scanType,             // pointer to element data
                          sizeof(p_context->scanType));     // number of element data bytes

    LowLevel_CASetElement(WF_CA_ELEMENT_SCAN_COUNT,
                          &p_context->scanCount,
                          sizeof(p_context->scanCount));

    tmp = TCPIP_Helper_htons(p_context->minChannelTime);
    LowLevel_CASetElement(WF_CA_ELEMENT_MIN_CHANNEL_TIME,
                          (uint8_t *)&tmp,
                          sizeof(tmp));

    tmp = TCPIP_Helper_htons(p_context->minChannelTime);
    LowLevel_CASetElement(WF_CA_ELEMENT_MAX_CHANNEL_TIME,
                          (uint8_t *)&tmp,
                          sizeof(tmp));

    tmp = TCPIP_Helper_htons(p_context->probeDelay);
    LowLevel_CASetElement(WF_CA_ELEMENT_PROBE_DELAY,
                          (uint8_t *)&tmp,
                          sizeof(tmp));
}

/*******************************************************************************
  Function:
    void DRV_WIFI_ScanContextGet(DRV_WIFI_SCAN_CONTEXT *p_context);

  Summary:
    Gets the WiFi scan context.

  Description:
    This function gets the WiFi scan context.

  Parameters:
    p_context - Pointer to where scan context will be written.  See
                DRV_WIFI_SCAN_CONTEXT structure.

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void DRV_WIFI_ScanContextGet(DRV_WIFI_SCAN_CONTEXT *p_context)
{
    uint16_t tmp;

    LowLevel_CAGetElement(WF_CA_ELEMENT_SCANTYPE,           // Element ID
                          &p_context->scanType,             // pointer to element data
                          sizeof(p_context->scanType),      // number of element data bytes
                          true);                            // delete mgmt msg after read

    LowLevel_CAGetElement(WF_CA_ELEMENT_SCAN_COUNT,
                          &p_context->scanCount,
                          sizeof(p_context->scanCount),
                          true);

    LowLevel_CAGetElement(WF_CA_ELEMENT_MIN_CHANNEL_TIME,
                          (uint8_t *)&tmp,
                          sizeof(tmp),
                          true);
    tmp = TCPIP_Helper_htons(p_context->minChannelTime);

    LowLevel_CAGetElement(WF_CA_ELEMENT_MAX_CHANNEL_TIME,
                          (uint8_t *)&tmp,
                          sizeof(tmp),
                          true);
    tmp = TCPIP_Helper_htons(p_context->minChannelTime);

    LowLevel_CAGetElement(WF_CA_ELEMENT_PROBE_DELAY,
                          (uint8_t *)&tmp,
                          sizeof(tmp),
                          true);
    tmp = TCPIP_Helper_htons(p_context->probeDelay);
}

// *****************************************************************************
/* Function:
    void DRV_WIFI_RssiSet(uint8_t rssi);

  Summary:
    Sets RSSI restrictions when connecting

  Description:
    This setting is only used if:
      1) Neither an SSID or BSSID has been configured
                         or
      2) An SSID is defined and multiple AP's are discovered with the same SSID

    <table>
    rssi       Description
    ----       -----------
    0          Connect to first network found
    1-254      Only connect to network if the RSSI is greater or equal to this value
    255        Connect to the highest RSSI found (default)
    </table>

  Precondition:
    WiFi initialization must be complete

  Parameters:
    rssi - See description

 Returns:
    None

  Example:
    <code>
        DRV_WIFI_RssiSet(255);
    </code>

  Remarks:
    Rarely needed
*/
void DRV_WIFI_RssiSet(uint8_t rssi)
{
    LowLevel_CASetElement(WF_CA_ELEMENT_RSSI,  /* Element ID                   */
                          &rssi,                /* pointer to element data      */
                          sizeof(rssi));        /* number of element data bytes */
}

// *****************************************************************************
/* Function:
    void DRV_WIFI_RssiGet(uint8_t *p_rssi);

  Summary:
    Gets RSSI value set in DRV_WIFI_RssiSet()

  Description:
    This function retrieves the value set in Gets RSSI value set in DRV_WIFI_RssiSet().
    It does not retrieve the current connection RSSI value.  The scan result will
    yield the current RSSI.

  Precondition:
    WiFi initialization must be complete

  Parameters:
    p_rssi - pointer where rssi value is written

 Returns:
    None

  Example:
    <code>
        uint8_t rssi;

        DRV_WIFI_RssiGet(&rssi);
    </code>

  Remarks:
    None
*/
void DRV_WIFI_RssiGet(uint8_t *p_rssi)
{
    LowLevel_CAGetElement(WF_CA_ELEMENT_RSSI,      /* Element ID                   */
                          p_rssi,                  /* pointer to element data      */
                          1,                       /* number of element data bytes */
                         true);                    /* read data, free buffer       */
}

// *****************************************************************************
/* Function:
    void DRV_WIFI_ReconnectModeSet(uint8_t retryCount, uint8_t deauthAction,
                                   uint8_t beaconTimeout, uint8_t beaconTimeoutAction);

  Summary:
    Sets the WiFi reconnection mode.

  Description:
    This function controls how the MRF24WG behaves when an existing WiFi connection
    is lost.  The MRF24WG can lose an existing connection in one of two ways:
    1) Beacon timeout
    2) Deauth received from AP

    There are two options with respect to regaining a lost WiFi connection:
        1) MRF24WG informs the host that the connection was temporarily lost and
           then the MRF24WG retries N times (or forever) to regain the connection.
        2) MRF24WG simply informs the host application that the connection is lost,
           and it is up to the host to regain the connection via the API.

    Note: The retryCount parameter also applies when initially connecting.
          That is, the retryCount tells the MRF24WG how many time to try to
          connect to a WiFi network before giving up and generating the
          DRV_WIFI_EVENT_CONNECTION_FAILED event.

        <table>
        'retryCount' field    Description
        ------------------    -----------
        0                     Do not try to regain a connection (simply report event to host)
        1:254                 Number of times MRF24WG should try to regain the connection
        255                   MRF24WG will retry forever (do not use for AdHoc connections)
        </table>

        <table>
        'deauthAction' field                   Description
        --------------------                   -----------
        DRV_WIFI_DO_NOT_ATTEMPT_TO_RECONNECT   Do not attempt to reconnect after a deauth
        DRV_WIFI_ATTEMPT_TO_RECONNECT          Attempt to reconnect after a deauth
        </table>

        <table>
        'beaconTimeout' field   Description
        ---------------------   -----------
        0                       MRF24WG will not monitor the beacon timeout condition
        1:255                   Number of missed beacons before designating the connection as lost.
        </table>

        <table>
        'beaconTimeoutAction' field             Description
        ---------------------------             -----------
        DRV_WIFI_DO_NOT_ATTEMPT_TO_RECONNECT    Do not attempt to reconnect after a beacon timeout
        DRV_WIFI_ATTEMPT_TO_RECONNECT           Attempt to reconnect after a beacon timeout
        </table>

  Parameters:
    retryCount        - Number of times the MRF24WG should try to regain the connection (see description)

    deauthAction      - In the event of a deauth from the AP, the action the MRF24WG
                        should take (see description)

    beaconTimeout     - Number of missed beacons before the MRF24WG designates the
                        connection as lost (see description)

    beaconTimeoutAction - In the event of a beacon timeout, the action the MRF24WG
                          should take (see description)

 Returns:
    None
*/
void DRV_WIFI_ReconnectModeSet(uint8_t retryCount, uint8_t deauthAction, uint8_t beaconTimeout, uint8_t beaconTimeoutAction)
{
    LowLevel_CASetElement(WF_CA_ELEMENT_LIST_RETRY_COUNT,       // Element ID
                          &retryCount,                          // pointer to element data
                          sizeof(retryCount));                  // number of element data bytes

    LowLevel_CASetElement(WF_CA_ELEMENT_DEAUTH_ACTION,          // Element ID
                          &deauthAction,                        // pointer to element data
                          sizeof(deauthAction));                // number of element data bytes

    LowLevel_CASetElement(WF_CA_ELEMENT_BEACON_TIMEOUT,         // Element ID
                          &beaconTimeout,                       // pointer to element data
                          sizeof(beaconTimeout));               // number of element data bytes

    LowLevel_CASetElement(WF_CA_ELEMENT_BEACON_TIMEOUT_ACTION,  // Element ID
                          &beaconTimeoutAction,                 // pointer to element data
                          sizeof(beaconTimeoutAction));         // number of element data bytes
}

// *****************************************************************************
/* Function:
    void DRV_WIFI_ReconnectModeGet(uint8_t *p_retryCount, uint8_t *p_deauthAction,
                                   uint8_t *p_beaconTimeout, uint8_t *p_beaconTimeoutAction);

  Summary:
    Gets the WiFi reconnection mode.

  Description:
    This function gets the reconnection mode parameters set in
    DRV_WIFI_ReconnectModeGet().

  Precondition:
    WiFi initialization must be complete.

  Parameters:
    p_retryCount      - Pointer where retry count is written

    p_deauthAction    - Pointer where deauth action is written

    p_beaconTimeout   - Pointer where beacon timeout is written

    p_beaconTimeoutAction - Pointer where beacon timeout action is written.

 Returns:
    None

  Example:
    <code>
        uint8_t retryCount, deauthAction, beaconTimeout, beaconTimeoutAction;

        DRV_WIFI_ReconnectModeGet(&retryCount,
                                  &deauthAction,
                                  &beaconTimeout,
                                  &beaconTimeoutAction);
    </code>

  Remarks:
    None
*/
void DRV_WIFI_ReconnectModeGet(uint8_t *p_retryCount, uint8_t *p_deauthAction, uint8_t *p_beaconTimeout, uint8_t *p_beaconTimeoutAction)
{
    LowLevel_CAGetElement(WF_CA_ELEMENT_LIST_RETRY_COUNT,       // Element ID
                          p_retryCount,                         // pointer to element data
                          1,                                    // number of element data bytes
                          true);                                // read data, free mgmt buf

    LowLevel_CAGetElement(WF_CA_ELEMENT_DEAUTH_ACTION,          // Element ID
                          p_deauthAction,                       // pointer to element data
                          1,                                    // number of element data bytes
                          true);                                // read data, free mgmt buf

    LowLevel_CAGetElement(WF_CA_ELEMENT_BEACON_TIMEOUT,         // Element ID
                          p_beaconTimeout,                      // pointer to element data
                          1,                                    // number of element data bytes
                          true);                                // read data, free mgmt buf

    LowLevel_CAGetElement(WF_CA_ELEMENT_BEACON_TIMEOUT_ACTION,  // Element ID
                          p_beaconTimeoutAction,                // pointer to element data
                          1,                                    // number of element data bytes
                          true);                                // read data, free mgmt buf
}

// *****************************************************************************
/* Function:
    void DRV_WIFI_ChannelListSet(uint8_t *p_channelList, uint8_t numChannels);

  Summary:
    Sets the the channel list.

  Description:
    This function sets the channel list that the MRF24WG will use when scanning
    or connecting.

  Parameters:
    p_channelList - list of channels
    numChannels   - number of channels in list; if set to 0, then MRF24WG will set
                    its channel list to all valid channels in its regional domain.

  Returns:
    None

  Remarks:
    None
*/
void DRV_WIFI_ChannelListSet(uint8_t *p_channelList, uint8_t numChannels)
{
    LowLevel_CASetElement(WF_CA_ELEMENT_CHANNEL_LIST,  /* Element ID                   */
                          p_channelList,               /* pointer to element data      */
                          numChannels);                /* number of element data bytes */
}

// *****************************************************************************
/* Function:
    void RV_WIFI_ChannelListGet(uint8_t *p_channelList, uint8_t *p_numChannels);

  Summary:
    Gets the the channel list.

  Description:
    This function gets the current channel list.

  Parameters:
    p_channelList - pointer to where channel list will be written
    numChannels   - pointer to where number of channels in the list will be written.

  Returns:
    None
*/
void DRV_WIFI_ChannelListGet(uint8_t *p_channelList, uint8_t *p_numChannels)
{
    tCAElementResponseHdr mgmtHdr;

    /* send request, wait for mgmt response, do not read and do not free up response buffer */
     LowLevel_CAGetElement(WF_CA_ELEMENT_CHANNEL_LIST,    /* Element ID      */
                           NULL,                          /* do not read     */
                           0,                             /* do not read     */
                           false);                        /* do not read, do not free mgmt buffer */

    /* read managment header */
    RawRead(RAW_SCRATCH_ID, MGMT_RX_BASE, sizeof(tCAElementResponseHdr), (uint8_t *)&mgmtHdr);

    /* extract data length (which will be channel list length) */
    *p_numChannels = mgmtHdr.elementDataLength;

    // read channel list
    RawReadRelative(RAW_SCRATCH_ID, *p_numChannels, p_channelList);
}

// *****************************************************************************
/* Function:
    void DRV_WIFI_AdhocContextSet(DRV_WIFI_ADHOC_NETWORK_CONTEXT *p_context);

  Summary:
    Sets the AdHoc context.

  Description:
    This function sets the AdHoc context.  It is only applicable when the
    DRV_WIFI_NETWORK_TYPE_ADHOC has been selected in DRV_WIFI_NetworkTypeSet().

  Parameters:
    p_context   - pointer to AdHoc context data; see definition for the
                  DRV_WIFI_ADHOC_NETWORK_CONTEXT structure.

  Returns:
    None
*/
void DRV_WIFI_AdhocContextSet(DRV_WIFI_ADHOC_NETWORK_CONTEXT *p_context)
{
    uint16_t tmp;

    SetAdHocMode(p_context->mode);

    SetHiddenSsid(p_context->hiddenSsid);

    tmp = TCPIP_Helper_htons(p_context->beaconPeriod);
    LowLevel_CASetElement(WF_CA_ELEMENT_BEACON_PERIOD,  // Element ID
                          (uint8_t *)&tmp,              // pointer to element data
                          sizeof(tmp));                 // number of element data bytes
}

void WF_AdhocContextGet(DRV_WIFI_ADHOC_NETWORK_CONTEXT *p_context)
{
    p_context->mode = GetAdHocMode();

    LowLevel_CAGetElement(WF_CA_ELEMENT_BEACON_PERIOD,        /* Element ID                   */
                         (uint8_t *)&p_context->beaconPeriod, /* pointer to element data      */
                          sizeof(p_context->beaconPeriod),    /* number of element data bytes */
                          true);                              /* read data, free buffer       */

    /* fix endianness before returning value */
    p_context->beaconPeriod = TCPIP_Helper_ntohs(p_context->beaconPeriod);

    p_context->hiddenSsid = GetHiddenSsid();
}

void DRV_WIFI_SoftAPContextSet(DRV_WIFI_SOFTAP_NETWORK_CONTEXT *p_context)
{
#if defined(NOT_COMPLETED_YET) // ToDo
    uint16_t tmp;
#endif

    SetHiddenSsid(p_context->hiddenSsid);

#if defined(NOT_COMPLETED_YET) // ToDo
    SetSoftAPMode(p_context->mode);
    tmp = TCPIP_Helper_htons(p_context->beaconPeriod);
    LowLevel_CASetElement(WF_CA_ELEMENT_BEACON_PERIOD, // element ID
                          (uint8_t *)&tmp,             // pointer to element data
                          sizeof(tmp));                // number of element data bytes
#endif
}

void DRV_WF_SoftAPContextGet(DRV_WIFI_SOFTAP_NETWORK_CONTEXT *p_context)
{
#if defined(NOT_COMPLETED_YET)   // ToDo
    p_context->mode = GetAdHocMode();

    LowLevel_CAGetElement(WF_CA_ELEMENT_BEACON_PERIOD,        /* Element ID                   */
                         (uint8_t *)&p_context->beaconPeriod, /* pointer to element data      */
                          sizeof(p_context->beaconPeriod),    /* number of element data bytes */
                          true);                              /* read data, free buffer       */

    /* fix endianness before returning value */
    p_context->beaconPeriod = TCPIP_Helper_ntohs(p_context->beaconPeriod);
#endif
    p_context->hiddenSsid = GetHiddenSsid();
}

void SetListenInterval(uint16_t listenInterval)
{
    /* correct endianness before sending message */
    listenInterval = TCPIP_Helper_htons(listenInterval);
    LowLevel_CASetElement(WF_CA_ELEMENT_LISTEN_INTERVAL,    /* Element ID                   */
                         (uint8_t *)&listenInterval,        /* pointer to element data      */
                          sizeof(listenInterval));          /* number of element data bytes */
}

void SetDtimInterval(uint16_t dtimInterval)
{
    /* correct endianness before sending message */
    dtimInterval = TCPIP_Helper_htons(dtimInterval);
    LowLevel_CASetElement(WF_CA_ELEMENT_DTIM_INTERVAL,    /* Element ID                   */
                          (uint8_t *)&dtimInterval,       /* pointer to element data      */
                          sizeof(dtimInterval));          /* number of element data bytes */
}

/*******************************************************************************
  Function:
    static void LowLevel_CASetElement(uint8_t elementId,
                                      uint8_t *p_elementData,
                                      uint8_t elementDataLength)

  Summary:
    Set an element of the connection algorithm on the MRF24W.

  Description:
    Low-level function to send the appropriate management message to the
    MRF24W to set the Connection Algorithm element.

  Precondition:
    MACInit must be called first.

  Parameters:
    elementId - Element that is being set
    p_elementData - Pointer to element data
    elementDataLength - Number of bytes pointed to by p_elementData

  Returns:
    None.

  Remarks:
    All Connection Algorithm 'Set Element' functions call this function
    to construct the management message.  The caller must fix up any endian
    issues prior to calling this function.
 *****************************************************************************/
static void LowLevel_CASetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength)
{
    uint8_t  hdrBuf[4];

    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;           /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CA_SET_ELEMENT_SUBTYPE;      /* mgmt request subtype            */
    hdrBuf[2] = elementId;                      /* Element ID                      */
    hdrBuf[3] = elementDataLength;              /* number of bytes of element data */

    SendMgmtMsg(hdrBuf,
                sizeof(hdrBuf),
                p_elementData,
                elementDataLength);

    /* wait for mgmt response, free after it comes in, don't need data bytes */
    WaitForMgmtResponse(WF_CA_SET_ELEMENT_SUBTYPE, FREE_MGMT_BUFFER);
}

/*******************************************************************************
  Function:
    static void LowLevel_CAGetElement(uint8_t elementId,
                                      uint8_t *p_elementData,
                                      uint8_t elementDataLength,
                                      uint8_t dataReadAction)

  Summary:
    Get an element of the connection algorithm on the MRF24W.

  Description:
    Low-level function to send the appropriate management message to the
    MRF24W to get the Connection Algorithm element.

  Precondition:
    MACInit must be called first.

  Parameters:
    elementId - Element that is being read
    p_elementData - Pointer to where element data will be written
    elementDataLength - Number of element data bytes that will be read
    dataReadAction - If true then read data per paramters and free mgmt response buffer.
                      If false then return after response received, do not read any data as the
                      caller will do that, and don't free buffer, as caller will do that as well.

  Returns:
    None.

  Remarks:
    All Connection Algorithm 'Get Element' functions call this function to
    construct the management message.  The caller must fix up any endian issues
    after getting the data from this function.
 *****************************************************************************/
static void LowLevel_CAGetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength,
                                  uint8_t dataReadAction)    /* true or false */
{
    uint8_t  hdrBuf[4];

    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;       /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CA_GET_ELEMENT_SUBTYPE;  /* mgmt request subtype            */
    hdrBuf[2] = elementId;                  /* Element ID                      */
    hdrBuf[3] = 0;                          /* not used                        */

    SendMgmtMsg(hdrBuf,
                sizeof(hdrBuf),
                NULL,
                0);

    if (dataReadAction == (uint8_t)true)
    {
        /* wait for mgmt response, read desired data, and then free response buffer */
        WaitForMgmtResponseAndReadData(WF_CA_GET_ELEMENT_SUBTYPE,
                                                    elementDataLength,                   /* num data bytes to read                */
                                                    sizeof(tCAElementResponseHdr),       /* index of first byte of element data   */
                                                    p_elementData);                      /* where to write element data           */
    }
    else
    {
        /* wait for mgmt response, don't read any data bytes, do not release mgmt buffer */
        WaitForMgmtResponse(WF_CA_GET_ELEMENT_SUBTYPE, DO_NOT_FREE_MGMT_BUFFER);
    }
}

#endif /* TCPIP_IF_MRF24W */

//DOM-IGNORE-END