/*******************************************************************************
  MRF24W Driver WiFi Console

  File Name:
    drv_wifi_debug_output.c

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

//============================================================================
// Includes
//============================================================================
#include "tcpip/src/tcpip_private.h"

#if defined(TCPIP_IF_MRF24W)

//============================================================================
// Global Function Prototypes
//============================================================================
extern void SYS_RESET_SoftwareReset( void );

#if defined(SYS_CONSOLE_ENABLE) || defined(SYS_CONSOLE_INSTANCES_NUMBER)
const char *connectionFailureStrings[] = {
                                          "NULL",                                         /* 0 - not used */
                                          "NULL",                                         /* 1 - not used */
                                          "DRV_WIFI_JOIN_FAILURE",                        /* 2            */
                                          "DRV_WIFI_AUTHENTICATION_FAILURE",              /* 3            */
                                          "DRV_WIFI_ASSOCIATION_FAILURE",                 /* 4            */
                                          "DRV_WIFI_WEP_HANDSHAKE_FAILURE",               /* 5            */
                                          "DRV_WIFI_PSK_CALCULATION_FAILURE",             /* 6            */
                                          "DRV_WIFI_PSK_HANDSHAKE_FAILURE",               /* 7            */
                                          "DRV_WIFI_ADHOC_JOIN_FAILURE",                  /* 8            */
                                          "DRV_WIFI_SECURITY_MISMATCH_FAILURE",           /* 9            */
                                          "DRV_WIFI_NO_SUITABLE_AP_FOUND_FAILURE",        /* 10           */
                                          "DRV_WIFI_RETRY_FOREVER_NOT_SUPPORTED_FAILURE", /* 11           */
                                          "DRV_WIFI_LINK_LOST",                           /* 12           */
                                          "",                                             /* 13           */
                                          "DRV_WIFI_RSN_MIXED_MODE_NOT_SUPPORTED",        /* 14           */
                                          "DRV_WIFI_RECV_DEAUTH",                         /* 15           */
                                          "DRV_WIFI_RECV_DISASSOC",                       /* 16           */
                                          "DRV_WIFI_WPS_FAILURE",                         /* 17           */
                                          "",                                             /* 18           */
                                          "DRV_WIFI_LINK_DOWN"                            /* 19           */
                                         };

const char *connectionLostStrings[] = {
                                        "DRV_WIFI_ASSOCIATION_FAILURE",               /* 0 */
                                        "DRV_WIFI_BEACON_TIMEOUT",                    /* 1 */
                                        "DRV_WIFI_DEAUTH_RECEIVED",                   /* 2 */
                                        "DRV_WIFI_DISASSOCIATE_RECEIVED",             /* 3 */
                                        "DRV_WIFI_TKIP_MIC_FAILURE",                  /* 4 */
                                        "DRV_WIFI_LINK_DOWN"                          /* 5 */
                                      };

const char *deauthDisssocReasonCodes[] =
                                {
                                 "",                                           /* 0 */
                                 "DRV_WIFI_UNSPECIFIED",                       /* 1 */
                                 "DRV_WIFI_PREV_AUTH_NOT_VALID",               /* 2 */
                                 "DRV_WIFI_DEAUTH_LEAVING",                    /* 3 */
                                 "DRV_WIFI_DISASSOC_DUE_TO_INACTIVITY",        /* 4 */
                                 "DRV_WIFI_DISASSOC_AP_BUSY",                  /* 5 */
                                 "DRV_WIFI_CLASS2_FRAME_FROM_NONAUTH_STA",     /* 6 */
                                 "DRV_WIFI_CLASS3_FRAME_FROM_NONASSOC_STA",    /* 7 */
                                 "DRV_WIFI_DISASSOC_STA_HAS_LEFT",             /* 8 */
                                 "DRV_WIFI_STA_REQ_ASSOC_WITHOUT_AUTH",        /* 9 */
                                 "",                                           /* 10 */
                                 "",                                           /* 11 */
                                 "",                                           /* 12 */
                                 "DRV_WIFI_INVALID_IE",                        /* 13 */
                                 "DRV_WIFI_MIC_FAILURE",                       /* 14 */
                                 "DRV_WIFI_4WAY_HANDSHAKE_TIMEOUT",            /* 15 */
                                 "DRV_WIFI_GROUP_KEY_HANDSHAKE_TIMEOUT",       /* 16 */
                                 "DRV_WIFI_IE_DIFFERENT",                      /* 17 */
                                 "DRV_WIFI_INVALID_GROUP_CIPHER",              /* 18 */
                                 "DRV_WIFI_INVALID_PAIRWISE_CIPHER",           /* 19 */
                                 "DRV_WIFI_INVALID_AKMP",                      /* 20 */
                                 "DRV_WIFI_UNSUPP_RSN_VERSION",                /* 21 */
                                 "DRV_WIFI_INVALID_RSN_IE_CAP",                /* 22 */
                                 "DRV_WIFI_IEEE8021X_FAILED",                  /* 23 */
                                 "DRV_WIFI_CIPHER_SUITE_REJECTED"              /* 24 */
                                };

const char *statusCodes[] = {
                             "",                                     /* 0 */
                             "DRV_WIFI_UNSPECIFIED_FAILURE",         /* 1 */
                             "",                                     /* 2 */
                             "",                                     /* 3 */
                             "",                                     /* 4 */
                             "",                                     /* 5 */
                             "",                                     /* 6 */
                             "",                                     /* 7 */
                             "",                                     /* 8 */
                             "",                                     /* 9 */
                             "DRV_WIFI_CAPS_UNSUPPORTED",            /* 10 */
                             "DRV_WIFI_REASSOC_NO_ASSOC",            /* 11 */
                             "DRV_WIFI_ASSOC_DENIED_UNSPEC",         /* 12 */
                             "DRV_WIFI_NOT_SUPPORTED_AUTH_ALG",      /* 13 */
                             "DRV_WIFI_UNKNOWN_AUTH_TRANSACTION",    /* 14 */
                             "DRV_WIFI_CHALLENGE_FAIL",              /* 15 */
                             "DRV_WIFI_AUTH_TIMEOUT",                /* 16 */
                             "DRV_WIFI_AP_UNABLE_TO_HANDLE_NEW_STA", /* 17 */
                             "DRV_WIFI_ASSOC_DENIED_RATES",          /* 18 */
                             "DRV_WIFI_ASSOC_DENIED_NOSHORTPREAMBLE",/* 19 */
                             "DRV_WIFI_ASSOC_DENIED_NOPBCC",         /* 20 */
                             "DRV_WIFI_ASSOC_DENIED_NOAGILITY",      /* 21 */
                             "",                                     /* 22 */
                             "",                                     /* 23 */
                             "",                                     /* 24 */
                             "DRV_WIFI_ASSOC_DENIED_NOSHORTTIME",    /* 25 */
                             "DRV_WIFI_ASSOC_DENIED_NODSSSOFDM",     /* 26 */
                             "",                                     /* 27 */
                             "",                                     /* 28 */
                             "",                                     /* 29 */
                             "",                                     /* 30 */
                             "",                                     /* 31 */
                             "",                                     /* 32 */
                             "",                                     /* 33 */
                             "",                                     /* 34 */
                             "",                                     /* 35 */
                             "",                                     /* 36 */
                             "",                                     /* 37 */
                             "",                                     /* 38 */
                             "",                                     /* 39 */
                             "DRV_WIFI_NOT_VALID_IE",                /* 40 */
                             "DRV_WIFI_NOT_VALID_GROUPCIPHER",       /* 41 */
                             "DRV_WIFI_NOT_VALID_PAIRWISE_CIPHER",   /* 42 */
                             "DRV_WIFI_NOT_VALID_AKMP",              /* 43 */
                             "DRV_WIFI_UNSUPPORTED_RSN_VERSION",     /* 44 */
                             "DRV_WIFI_INVALID_RSN_IE_CAP",          /* 45 */
                             "DRV_WIFI_CIPHER_SUITE_REJECTED",       /* 46 */
                             "DRV_WIFI_TIMEOUT"                      /* 47 */
                            };

const char *wpsState[] = {
                          "",
                          "DRV_WIFI_EAPOL_START",
                          "DRV_WIFI_EAP_REQ_IDENTITY",
                          "DRV_WIFI_EAP_RSP_IDENTITY",
                          "DRV_WIFI_EAP_WPS_START",
                          "DRV_WIFI_EAP_RSP_M1",
                          "DRV_WIFI_EAP_REQ_M2",
                          "DRV_WIFI_EAP_RSP_M3",
                          "DRV_WIFI_EAP_REQ_M4",
                          "DRV_WIFI_EAP_RSP_M5",
                          "DRV_WIFI_EAP_REQ_M6",
                          "DRV_WIFI_EAP_RSP_M7",
                          "DRV_WIFI_EAP_REQ_M8",
                          "DRV_WIFI_EAP_RSP_DONE",
                          "DRV_WIFI_EAP_FAILURE"
                         };

const char *wpsConfigErr[] = {
                              "DRV_WIFI_WPS_NOERR",
                              "DRV_WIFI_WPS_SESSION_OVERLAPPED",
                              "DRV_WIFI_WPS_DECRYPT_CRC_FAILURE",
                              "DRV_WIFI_WPS_24G_NOT_SUPPORTED",
                              "DRV_WIFI_WPS_RETRY_FAILURE",
                              "DRV_WIFI_WPS_INVALID_MSG",
                              "DRV_WIFI_WPS_AUTH_FAILURE",
                              "DRV_WIFI_WPS_ASSOC_FAILURE",
                              "DRV_WIFI_WPS_MSG_TIMEOUT",
                              "DRV_WIFI_WPS_SESSION_TIMEOUT",
                              "DRV_WIFI_WPS_DEVPASSWD_AUTH_FAILURE",    /* 10 */
                              "DRV_WIFI_WPS_NO_CONN_TOREG",
                              "DRV_WIFI_WPS_MULTI_PBC_DETECTED",
                              "DRV_WIFI_WPS_EAP_FAILURE",
                              "DRV_WIFI_WPS_DEV_BUSY",
                              "DRV_WIFI_WPS_SETUP_LOCKED"
                              };

//============================================================================
// Local Function Prototypes
//============================================================================
static void OutputDeviceInfo(void);
static void OutputDomain(void);
static void OutputMacAddress(void);
static void OutputSsid(void);
static void OutputNetworkType(void);
static void OutputScanType(void);
static void OutputChannelList(void);
static void OutputBeaconTimeout(void);
static void OutputSecurityMode(void);
static void OutputPowerSaveMode(void);
static void OutputIpAddress(void);

static void OutputConnectionTempLostMsg(uint16_t eventInfo);
static void OutputConnectionFailedMsg(uint16_t eventInfo);
static void OutputConnectionPermLostMsg(uint16_t eventInfo);

static void OutputTxMode(void);

static bool ValidatePinChecksum(unsigned long int pin);

/*******************************************************************************
  Function:
    void OutputDemoHeader(void);

  Summary:
    Outputs WiFi demo header to console.

  Description:
    Outputs the name of the demo, and various information items about how the
    demo is configured.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
void OutputDemoHeader(void)
{
    SYS_CONSOLE_MESSAGE("\r\n");
    #if defined(WIFI_TCPIP_WEB_SERVER_DEMO)
        SYS_CONSOLE_MESSAGE("====================================\r\n");
        SYS_CONSOLE_MESSAGE("*** Wi-Fi TCP/IP Web Server Demo ***\r\n");
        SYS_CONSOLE_MESSAGE("====================================\r\n");
    #elif defined(WIFI_EASY_CONFIG_DEMO)
        SYS_CONSOLE_MESSAGE("==================================\r\n");
        SYS_CONSOLE_MESSAGE("*** Wi-Fi TCP/IP EZConfig Demo ***\r\n");
        SYS_CONSOLE_MESSAGE("==================================\r\n");
    #elif defined(WIFI_CYASSL_TCP_CLIENT_DEMO)
        SYS_CONSOLE_MESSAGE("====================================\r\n");
        SYS_CONSOLE_MESSAGE("*** Wi-Fi CyaSSL TCP Client Demo ***\r\n");
        SYS_CONSOLE_MESSAGE("====================================\r\n");
    #elif defined(WIFI_CYASSL_TCP_SERVER_DEMO)
        SYS_CONSOLE_MESSAGE("====================================\r\n");
        SYS_CONSOLE_MESSAGE("*** Wi-Fi CyaSSL TCP Server Demo ***\r\n");
        SYS_CONSOLE_MESSAGE("====================================\r\n");
    #else
        SYS_CONSOLE_MESSAGE("===================================\r\n");
        SYS_CONSOLE_MESSAGE("*** Microchip Wi-Fi TCP/IP Demo ***\r\n");
        SYS_CONSOLE_MESSAGE("===================================\r\n");
    #endif

    OutputDeviceInfo();
    OutputDomain();
    OutputMacAddress();
    OutputSsid();
    OutputNetworkType();
    OutputScanType();
    OutputChannelList();
    OutputBeaconTimeout();
    OutputSecurityMode();
    OutputTxMode();
    OutputPowerSaveMode();
    OutputIpAddress();
}

static void OutputDeviceInfo(void)
{
    DRV_WIFI_DEVICE_INFO deviceInfo;

    SYS_CONSOLE_MESSAGE("Device:          ");

    DRV_WIFI_DeviceInfoGet(&deviceInfo);
    if (deviceInfo.romVersion >= 0x30)
    {
        SYS_CONSOLE_PRINT("MRF24WG (0x%02X%02X)\r\n", deviceInfo.romVersion, deviceInfo.patchVersion);
    }
    else
    {
        SYS_CONSOLE_PRINT("MRF24WB (0x%02X%02X)\r\n", deviceInfo.romVersion, deviceInfo.patchVersion);
    }
}

/*******************************************************************************
  Function:
    static void OutputDomain(void)

  Summary:
    Outputs WiFi domain string.

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputDomain(void)
{
    uint8_t regionalDomain;

    SYS_CONSOLE_MESSAGE("Domain:          ");

    DRV_WIFI_RegionalDomainGet(&regionalDomain);

    switch (regionalDomain) {
    case DRV_WIFI_DOMAIN_FCC:
        SYS_CONSOLE_MESSAGE("FCC");
        break;
    case DRV_WIFI_DOMAIN_ETSI:
        SYS_CONSOLE_MESSAGE("ETSI");
        break;
    case DRV_WIFI_DOMAIN_JAPAN:
        SYS_CONSOLE_MESSAGE("JAPAN or OTHER");
        break;
    default:
        SYS_CONSOLE_MESSAGE("Invalid Domain");
    }

    SYS_CONSOLE_MESSAGE("\r\n");
}

/*******************************************************************************
  Function:
    static void OutputMacAddress(void)

  Summary:
    Outputs WiFi MAC address

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputMacAddress(void)
{
    uint8_t mac[6];
    int i;

    DRV_WIFI_MacAddressGet(mac);
    SYS_CONSOLE_MESSAGE("MAC:             ");
    for (i = 0; i < 6; ++i)
    {
        SYS_CONSOLE_PRINT("%02X ", mac[i]);
    }

    SYS_CONSOLE_MESSAGE("\r\n");
}

/*******************************************************************************
  Function:
    static void OutputSsid(void)

  Summary:
    Outputs Wi-Fi SSID

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputSsid(void)
{
    int i;

    SYS_CONSOLE_MESSAGE("SSID:            ");

    if (p_wifi_ConfigData->netSSID[0] == '\0')
    {
        SYS_CONSOLE_MESSAGE("(None)");
    }
    else
    {
        if (p_wifi_ConfigData->SsidLength < DRV_WIFI_MAX_SSID_LENGTH)
        {
            p_wifi_ConfigData->netSSID[p_wifi_ConfigData->SsidLength] = '\0';
            SYS_CONSOLE_PRINT("%s", p_wifi_ConfigData->netSSID);
        }
        else
        {
            for (i = 0; i < p_wifi_ConfigData->SsidLength; i++)
                SYS_CONSOLE_PRINT("%c", p_wifi_ConfigData->netSSID[i]);
        }
    }

    SYS_CONSOLE_MESSAGE("\r\n");
}

/*******************************************************************************
  Function:
    static void OutputNetworkType(void)

  Summary:
    Outputs Wi-Fi network type in connection profile

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputNetworkType(void)
{
    uint8_t networkType;

    SYS_CONSOLE_MESSAGE("Network Type:    ");

    DRV_WIFI_NetworkTypeGet(&networkType);

    if (networkType == DRV_WIFI_NETWORK_TYPE_ADHOC)
        SYS_CONSOLE_MESSAGE("AdHoc");
    else if (networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
    SYS_CONSOLE_MESSAGE("SoftAP");
    else if (networkType == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE)
        SYS_CONSOLE_MESSAGE("Infrastructure");
    else
        SYS_CONSOLE_MESSAGE("Invalid");

    SYS_CONSOLE_MESSAGE("\r\n");
}

/*******************************************************************************
  Function:
    static void OutputScanType(void)

  Summary:
    Outputs Wi-Fi Scan Type

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputScanType(void)
{
    DRV_WIFI_SCAN_CONTEXT scanContext;

    SYS_CONSOLE_MESSAGE("Scan Type:       ");

    DRV_WIFI_ScanContextGet(&scanContext);

    if (scanContext.scanType == DRV_WIFI_PASSIVE_SCAN)
        SYS_CONSOLE_MESSAGE("Passive Scan");
    else if (scanContext.scanType == DRV_WIFI_ACTIVE_SCAN)
        SYS_CONSOLE_MESSAGE("Active Scan");
    else
        SYS_CONSOLE_MESSAGE("Invalid");

    SYS_CONSOLE_MESSAGE("\r\n");
}

/*******************************************************************************
  Function:
    static OutputChannelList(void)

  Summary:
    Outputs Wi-Fi channel list

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputChannelList(void)
{
    uint8_t channelList[DRV_WIFI_MAX_CHANNEL_LIST_LENGTH];
    uint8_t numChannels;
    int     i;

    DRV_WIFI_ChannelListGet(channelList, &numChannels);

    SYS_CONSOLE_MESSAGE("Channel List:    ");

    i = 0;
    while (i < numChannels && channelList[i] != '\0')
    {
        SYS_CONSOLE_PRINT("%d", channelList[i]);
        i++;
        if (channelList[i] != '\0' && i < numChannels)
            SYS_CONSOLE_MESSAGE(", ");
    }

    SYS_CONSOLE_MESSAGE("\r\n");
}

/*******************************************************************************
  Function:
    static void OutputBeaconTimeout(void)

  Summary:
    Outputs Wi-Fi network connection Beacon timeout count and retry count

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputBeaconTimeout(void)
{
    uint8_t beaconTimeout;
    uint8_t retryCount, deauthAction, beaconTimeoutAction;

    DRV_WIFI_ReconnectModeGet(&retryCount, &deauthAction, &beaconTimeout, &beaconTimeoutAction);

    SYS_CONSOLE_MESSAGE("Beacon Timeout:  ");

    if (beaconTimeout == 0)
    {
        SYS_CONSOLE_MESSAGE("No Beacon Timeout");
    }
    else
    {
        SYS_CONSOLE_PRINT("%d", beaconTimeout);
    }

    SYS_CONSOLE_MESSAGE("\r\n");

    SYS_CONSOLE_MESSAGE("Retry Count:     ");
    if (retryCount == DRV_WIFI_RETRY_FOREVER)
        SYS_CONSOLE_MESSAGE("255 (Retry Forever)");
    else if (retryCount > 0 && retryCount < DRV_WIFI_RETRY_FOREVER )
    {
        SYS_CONSOLE_PRINT("%d", retryCount);
    }
    else
        SYS_CONSOLE_MESSAGE("Invalid");

    SYS_CONSOLE_MESSAGE("\r\n");
}

/*******************************************************************************
  Function:
    static void OutputSecurityMode(void)

  Summary:
    Outputs Wi-Fi network connection security mode

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputSecurityMode(void)
{
    uint8_t     securityType;
    uint8_t     securityKey[80];
    uint8_t     securityKeyLength;
    uint8_t     keyType;
    uint8_t*    p;
    int         i;

    SYS_CONSOLE_MESSAGE("Security:        ");

    DRV_WIFI_SecurityGet( &securityType,
                          securityKey,
                          &securityKeyLength);
    p = securityKey;

    switch (securityType) {
    case DRV_WIFI_SECURITY_OPEN:
        SYS_CONSOLE_MESSAGE("Open\r\n");
        break;
    case DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY:
        SYS_CONSOLE_MESSAGE("WPA PSK with Key, Auto-select\r\n");
        break;
    case DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:
        SYS_CONSOLE_MESSAGE("WPA PSK with Pass Phrase, Auto-select\r\n");
        break;
    case DRV_WIFI_SECURITY_WPS_PUSH_BUTTON:
        SYS_CONSOLE_MESSAGE("WPS Push Button Method\r\n");
        break;
    case  DRV_WIFI_SECURITY_WPS_PIN:
        SYS_CONSOLE_MESSAGE("WPS PIN Method\r\n");
        break;
    case DRV_WIFI_SECURITY_EAP:
        SYS_CONSOLE_MESSAGE("WPA Enterprise\r\n");
        break;
    case DRV_WIFI_SECURITY_WEP_40:
    case DRV_WIFI_SECURITY_WEP_104:
        if (securityType == DRV_WIFI_SECURITY_WEP_40)
            SYS_CONSOLE_MESSAGE("WEP40, ");
        else if (securityType == DRV_WIFI_SECURITY_WEP_104)
            SYS_CONSOLE_MESSAGE("WEP104, ");
        else
            SYS_CONSOLE_MESSAGE("Invalid WEP mode, ");

        DRV_WIFI_WepKeyTypeGet(&keyType);

        if (keyType == DRV_WIFI_SECURITY_WEP_OPENKEY)
            SYS_CONSOLE_MESSAGE("Open Key\r\n");
        else if (keyType == DRV_WIFI_SECURITY_WEP_SHAREDKEY)
            SYS_CONSOLE_MESSAGE("Shared Key\r\n");
        else
            SYS_CONSOLE_MESSAGE("Invalid Key Type\r\n");

        break;
    default:
        SYS_CONSOLE_MESSAGE("Invalid Security Setting\r\n");
    }

    SYS_CONSOLE_MESSAGE("Security Key:    ");
    if (securityType == DRV_WIFI_SECURITY_WPS_PIN || securityType == DRV_WIFI_SECURITY_WPS_PUSH_BUTTON)
       SYS_CONSOLE_MESSAGE("Acquire from AP");
    else
    {
        // Security Length is not Null terminated.  So for WEP key we need to limit
        // the length appropriately.
        if (securityType == DRV_WIFI_SECURITY_WEP_40)
            securityKeyLength=5;
        else if (securityType == DRV_WIFI_SECURITY_WEP_104)
            securityKeyLength=13;

        for ( i = 0; i < securityKeyLength ; i++ )
        {
            SYS_CONSOLE_PRINT("%.2x", *p++);
        }
    }

    SYS_CONSOLE_MESSAGE("\r\n");
}

/*******************************************************************************
  Function:
    static void OutputTxMode(void)

  Summary:
    Outputs Wi-Fi network mode

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputTxMode(void)
{
    uint8_t txMode;

    SYS_CONSOLE_MESSAGE("Tx Mode:         ");

    DRV_WIFI_TxModeGet(&txMode);

    if (txMode == DRV_WIFI_TXMODE_G_RATES)
    {
        SYS_CONSOLE_MESSAGE("802.11bg Mixed");
    }
    else if (txMode == DRV_WIFI_TXMODE_B_RATES)
    {
        SYS_CONSOLE_MESSAGE("802.11b Only");
    }
    else if (txMode == DRV_WIFI_TXMODE_LEGACY_RATES)
    {
        SYS_CONSOLE_MESSAGE("802.11b (1 and 2 Mbps Only)");
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Invalid Network Mode");
    }

    SYS_CONSOLE_MESSAGE("\r\n");
}

/*******************************************************************************
  Function:
    static void OutputPowerSaveMode(void)

  Summary:
    Outputs Wi-Fi network power save mode

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputPowerSaveMode(void)
{
    SYS_CONSOLE_MESSAGE("Power Save:      ");
    #if (WF_DEFAULT_PS_POLL == DRV_WIFI_ENABLED)
    {
        SYS_CONSOLE_MESSAGE("Enabled\r\n");
        SYS_CONSOLE_MESSAGE("PS State:        Active\r\n");
    }
    #elif (WF_DEFAULT_PS_POLL == DRV_WIFI_DISABLED)
        SYS_CONSOLE_MESSAGE("Disabled\r\n");
    #endif
}

/*******************************************************************************
  Function:
    static void OutputIpAddress(void)

  Summary:
    Outputs Wi-Fi network IP address

  Description:
    None

  Precondition:
    None

  Parameters: TCPIP_MAC_HANDLE hMac
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
static void OutputIpAddress(void)
{
    SYS_CONSOLE_MESSAGE("IP Config:       ");
#ifdef TCPIP_STACK_USE_DHCP_CLIENT
    SYS_CONSOLE_MESSAGE("Dynamic\r\n");
#else
    SYS_CONSOLE_MESSAGE("Static\n\r");
#endif
}

void WF_OutputConnectionContext(void)
{
    DRV_WIFI_CONNECTION_CONTEXT context;
    int i;

    DRV_WIFI_ConnectContextGet(&context);
    SYS_CONSOLE_MESSAGE("  Connected BSSID: ");
    for (i = 0; i < DRV_WIFI_BSSID_LENGTH; ++i)
    {
        if (i < (DRV_WIFI_BSSID_LENGTH - 1))
        {
            SYS_CONSOLE_PRINT("%02X:", context.bssid[i]);
        }
        else
        {
            SYS_CONSOLE_PRINT("%02X\r\n", context.bssid[i]);
        }
    }

    SYS_CONSOLE_MESSAGE("  Channel:         ");
    SYS_CONSOLE_PRINT("%d\r\n", context.channel);
}

void WF_OutputConnectionDebugMsg(uint8_t event, uint16_t eventInfo)
{
    if (event == DRV_WIFI_EVENT_CONNECTION_TEMPORARILY_LOST)
    {
        OutputConnectionTempLostMsg(eventInfo);
    }
    else if (event == DRV_WIFI_EVENT_CONNECTION_FAILED)
    {
        OutputConnectionFailedMsg(eventInfo);
    }
    else if (event == DRV_WIFI_EVENT_CONNECTION_PERMANENTLY_LOST)
    {
        OutputConnectionPermLostMsg(eventInfo);
    }
}

static void OutputConnectionTempLostMsg(uint16_t eventInfo)
{
    if (eventInfo & DRV_WIFI_DEAUTH_REASONCODE_MASK)
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Temporarily Lost -- EventInfo = Received Deauth : ");
        SYS_CONSOLE_MESSAGE(deauthDisssocReasonCodes[eventInfo & ~DRV_WIFI_DEAUTH_REASONCODE_MASK]);
    }
    else if (eventInfo & DRV_WIFI_DISASSOC_REASONCODE_MASK)
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Temporarily Lost -- EventInfo = Received Disassoc : ");
        SYS_CONSOLE_MESSAGE(deauthDisssocReasonCodes[eventInfo & ~DRV_WIFI_DISASSOC_REASONCODE_MASK]);
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Temporarily Lost -- EventInfo = ");
        SYS_CONSOLE_PRINT("%d, ", eventInfo);
        SYS_CONSOLE_MESSAGE(connectionLostStrings[eventInfo]);
    }

    SYS_CONSOLE_MESSAGE("\r\n");
}

static void OutputConnectionFailedMsg(uint16_t eventInfo)
{
    uint8_t status;
    uint8_t reason;

    status = (uint8_t)(eventInfo >> 8);
    reason = (uint8_t)(eventInfo & 0xff);
    if (status == DRV_WIFI_RECV_DEAUTH || status == DRV_WIFI_RECV_DISASSOC)
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Failed : ");
        SYS_CONSOLE_MESSAGE(connectionFailureStrings[status]);
        SYS_CONSOLE_MESSAGE(" : ");
        SYS_CONSOLE_MESSAGE(deauthDisssocReasonCodes[reason]);

    }
    else if (status == DRV_WIFI_AUTHENTICATION_FAILURE || status == DRV_WIFI_ASSOCIATION_FAILURE)
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Failed : ");
        SYS_CONSOLE_MESSAGE(connectionFailureStrings[status]);
        SYS_CONSOLE_MESSAGE(" : ");
        SYS_CONSOLE_MESSAGE(statusCodes[reason]);
    }
    else if (status == DRV_WIFI_WPS_FAILURE)
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Failed : ");
        SYS_CONSOLE_MESSAGE(connectionFailureStrings[status]);
        SYS_CONSOLE_MESSAGE(" : ");
        SYS_CONSOLE_MESSAGE(wpsState[reason >> 4]);
        SYS_CONSOLE_MESSAGE(" : ");
        SYS_CONSOLE_MESSAGE(wpsConfigErr[reason & 0x0f]);
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Failed : ");
        SYS_CONSOLE_MESSAGE(connectionFailureStrings[status]);
    }
    SYS_CONSOLE_MESSAGE("\r\n");
}

static void OutputConnectionPermLostMsg(uint16_t eventInfo)
{
    if (eventInfo & DRV_WIFI_DEAUTH_REASONCODE_MASK)
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Permanently Lost -- EventInfo = Received Deauth : ");
        SYS_CONSOLE_MESSAGE(deauthDisssocReasonCodes[eventInfo & ~DRV_WIFI_DEAUTH_REASONCODE_MASK]);
    }
    else if (eventInfo & DRV_WIFI_DISASSOC_REASONCODE_MASK)
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Permanently Lost -- EventInfo = Received Disassoc : ");
        SYS_CONSOLE_MESSAGE(deauthDisssocReasonCodes[eventInfo & ~DRV_WIFI_DISASSOC_REASONCODE_MASK]);
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Event: Connection Permanently Lost -- EventInfo = ");
        SYS_CONSOLE_PRINT("%d, ", eventInfo);
        SYS_CONSOLE_MESSAGE(connectionLostStrings[eventInfo]);
    }

    SYS_CONSOLE_MESSAGE("\r\n");
}

void ValidateConfig(void)
{
    uint8_t buf[64];
    uint8_t regionalDomain;

    DRV_WIFI_RegionalDomainGet(&regionalDomain);

    DRV_WIFI_ASSERT(p_wifi_ConfigData->SsidLength <= 32, "Incorrect SSID length");

    if ( (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE) &&
         (p_wifi_ConfigData->SecurityMode != DRV_WIFI_SECURITY_WPS_PIN && p_wifi_ConfigData->SecurityMode != DRV_WIFI_SECURITY_WPS_PUSH_BUTTON) )
    {
        memcpy(buf, p_wifi_ConfigData->netSSID, p_wifi_ConfigData->SsidLength * sizeof(uint8_t));
        buf[p_wifi_ConfigData->SsidLength] = '\0';
        DRV_WIFI_ASSERT(strlen((const char*)buf) != 0, "Must define an SSID for this configuration");
    }

    if (p_wifi_ConfigData->SecurityMode == DRV_WIFI_SECURITY_WPS_PIN)
    {
        unsigned long int numericPin;

        memcpy(buf, p_wifi_ConfigData->netSSID, p_wifi_ConfigData->SsidLength * sizeof(uint8_t));
        buf[p_wifi_ConfigData->SsidLength] = '\0';
        DRV_WIFI_ASSERT(strlen((const char*)buf) != 0, "For DRV_WIFI_SECURITY_WPS_PIN mode, an SSID must be defined");

        memcpy(buf, p_wifi_ConfigData->SecurityKey, sizeof(p_wifi_ConfigData->SecurityKey));
        // WF_WPS_PIN_LENGTH should always be 8
        DRV_WIFI_ASSERT(strlen((const char*)buf) == 8 &&
            p_wifi_ConfigData->SecurityKeyLength == 8 &&
            WF_WPS_PIN_LENGTH == 8, "PIN must be exactly 8 digits");

        // Validate the PIN
        sscanf((const char *)buf, "%ld", &numericPin);
        if (!ValidatePinChecksum(numericPin))
        {
            // This console printing message is in case of DRV_WIFI_ASSERT is disabled
            SYS_CONSOLE_MESSAGE("\r\nPIN has an invalid checksum\r\n");
            DRV_WIFI_ASSERT(false, "PIN has an invalid checksum");
        }
    }

    if (p_wifi_ConfigData->SecurityMode == DRV_WIFI_SECURITY_WPS_PUSH_BUTTON)
    {
        memcpy(buf, p_wifi_ConfigData->netSSID, p_wifi_ConfigData->SsidLength * sizeof(uint8_t));
        buf[p_wifi_ConfigData->SsidLength] = '\0';
        DRV_WIFI_ASSERT(strlen((const char*)buf) == 0, "For DRV_WIFI_SECURITY_WPS_PUSH_BUTTON, WF_DEFAULT_SSID_NAME must be defined as \"\" (empty string)");
    }

    if (p_wifi_ConfigData->SecurityMode == DRV_WIFI_SECURITY_WPS_PIN || p_wifi_ConfigData->SecurityMode == DRV_WIFI_SECURITY_WPS_PUSH_BUTTON)
    {
        uint8_t channelList[] = WF_DEFAULT_CHANNEL_LIST;
        uint8_t i;

        if (regionalDomain == DRV_WIFI_DOMAIN_FCC)
        {
            DRV_WIFI_ASSERT((sizeof(channelList) == 11) || (sizeof(channelList) == 0), "If using WPS under FCC must have all 11 channels in list");

            if (sizeof(channelList) == 11)
            {
                for (i = 1; i <= 11; ++i)
                {
                    DRV_WIFI_ASSERT(channelList[i - 1] == i, "If using WPS under FCC must have channels 1-11 in list");
                }
            }
        }

        else if (regionalDomain == DRV_WIFI_DOMAIN_ETSI)
        {
            DRV_WIFI_ASSERT((sizeof(channelList) == 13) || (sizeof(channelList) == 0), "If using WPS under ETSI domain must have all 13 channels in list");

            if (sizeof(channelList) == 13)
            {
                for (i = 1; i <= 13; ++i)
                {
                    DRV_WIFI_ASSERT(channelList[i - 1] == i, "If using WPS under ETSI domain must have channels 1-13 in list");
                }
            }
        }

        else if (regionalDomain == DRV_WIFI_DOMAIN_JAPAN)
        {
            DRV_WIFI_ASSERT((sizeof(channelList) == 14) || (sizeof(channelList) == 0), "If using WPS under JAPAN domain must have all 14 channels in list");

            if (sizeof(channelList) == 14)
            {
                for (i = 1; i <= 14; ++i)
                {
                    DRV_WIFI_ASSERT(channelList[i - 1] == i, "If using WPS under JAPAN domain must have channels 1-14 in list");
                }
            }
        }

        else
        {
            DRV_WIFI_ASSERT(false, "Invalid domain");
        }
    }
}

static bool ValidatePinChecksum(unsigned long int pin)
{
    unsigned long int accum = 0;
    accum += 3 * ((pin / 10000000) % 10);
    accum += 1 * ((pin / 1000000) % 10);
    accum += 3 * ((pin / 100000) % 10);
    accum += 1 * ((pin / 10000) % 10);
    accum += 3 * ((pin / 1000) % 10);
    accum += 1 * ((pin / 100) % 10);
    accum += 3 * ((pin / 10) % 10);
    accum += 1 * ((pin / 1) % 10);
    return (0 == (accum % 10));
}

#endif /* SYS_CONSOLE_ENABLE || SYS_CONSOLE_INSTANCES_NUMBER */

/*******************************************************************************
  Function:
    void WF_AssertFunction(int condition, const char * msg, const char * file, int line)

  Summary:
    Make the system halt when assert condition is false

  Description:
    None

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 *******************************************************************************/
void WF_AssertFunction(int condition, const char * msg, const char * file, int line)
{
#if !defined(__DEBUG)
    uint32_t delayCount = 0;
#endif
    if (!condition)
    {
        if (*msg)
            SYS_CONSOLE_PRINT("\r\nWi-Fi Assert: %s\r\n%s, line %u\r\n", msg, file, line);
        else
            SYS_CONSOLE_PRINT("\r\nWi-Fi Assert: \r\n%s, line %u\r\n", file, line);
     
        while (1) {
#if defined(SYS_CONSOLE_ENABLE) || defined(SYS_CONSOLE_INSTANCES_NUMBER)
            SYS_CONSOLE_Tasks(sysObj.sysConsole0);
#endif

#if !defined(__DEBUG)
            // Need to wait for SYS_CONSOLE_PRINT message to complete before ASSERT
            if (++delayCount == 35000)
            {
                SYS_RESET_SoftwareReset();
            }
#endif // !defined(__DEBUG)
        }
    }
    return;
}

#endif /* #if defined(TCPIP_IF_MRF24W) */

//DOM-IGNORE-END
