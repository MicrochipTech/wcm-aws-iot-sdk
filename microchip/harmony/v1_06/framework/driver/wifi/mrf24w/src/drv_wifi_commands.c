/*******************************************************************************
  MRF24W commands(based on system commander) implementation

  File Name:
    drv_wifi_commands.c

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

//==============================================================================
//                                  Includes
//==============================================================================
#include <ctype.h>
#include "tcpip/src/tcpip_private.h"

#if defined(TCPIP_STACK_COMMANDS_WIFI_ENABLE)

// iwconfig control block
typedef struct
{
    uint8_t powerSaveState; // power save state
    uint8_t connState; // connection state
    bool isIdle; // true if connState is DRV_WIFI_CSTATE_NOT_CONNECTED
} t_wfIwconfigCb;

//==============================================================================
//                                  Constants
//==============================================================================

//==============================================================================
//                                  Local Globals
//==============================================================================
extern bool g_xmodem_update_started;

static t_wfIwconfigCb s_IwconfigCb;
static bool s_IwconfigCbInitialized = false;
bool g_ota_update_started = false;
static char s_helpInfo[600];

extern bool MRF24W_IsMACInit(void);
//==============================================================================
//                                  Local Function Prototypes
//==============================================================================
static bool IwconfigUpdateCb(SYS_CMD_DEVICE_NODE* pCmdIO);
static void IwconfigDisplayHelp(SYS_CMD_DEVICE_NODE* pCmdIO);
static void IwconfigDisplayStatus(SYS_CMD_DEVICE_NODE* pCmdIO);
static bool IwconfigSetSsid(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static bool IwconfigSetMode(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static bool IwconfigSetChannel(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static bool IwconfigSetPower(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static bool IwconfigSetRTS(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void IwconfigDisplayScanResult(SYS_CMD_DEVICE_NODE* pCmdIO, uint16_t index, DRV_WIFI_SCAN_RESULT *p_scanResult);
static int Cmd_Iwconfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_ReadConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_EraseConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_SaveConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void ConsoleFlush(void);

#if defined(WF_UPDATE_FIRMWARE_OTA)
static int Cmd_OTAUpdate(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    g_ota_update_started = true;
    WF_UserEventsSet(DRV_WIFI_EVENT_UPDATE, 0, false);
    return true;
}
#endif

#if defined(WF_UPDATE_FIRMWARE_XMODEM)
static int Cmd_XmodemUpdate(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    g_xmodem_update_started = true;
    WF_UserEventsSet(DRV_WIFI_EVENT_UPDATE, 0, false);
    return true;
}
#endif

// command table (placed here because needs above function prototypes)
static const SYS_CMD_DESCRIPTOR wfCmdTbl[]=
{
    {"iwconfig",  Cmd_Iwconfig,     ": wifi mode config"},
    {"readconf",  Cmd_ReadConfig,   ": read file from storage"},
    {"eraseconf", Cmd_EraseConfig,  ": erase storage"},
    {"saveconf",  Cmd_SaveConfig,   ": save storage"},
#if defined(WF_UPDATE_FIRMWARE_OTA)
    {"ota",       Cmd_OTAUpdate,    ": iw_update_ota"},
#endif
#if defined(WF_UPDATE_FIRMWARE_XMODEM)
    {"xmodem",    Cmd_XmodemUpdate, ": iw_update_xmodem"},
#endif
};

bool WIFICmdsInit(void)
{
    if (SYS_CMD_ADDGRP(wfCmdTbl, sizeof(wfCmdTbl)/sizeof(*wfCmdTbl), "wifi", ": wifi commands") == -1)
    {
        return false;
    }

    return true;
}

static int Cmd_Iwconfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint16_t scanIndex;
    DRV_WIFI_SCAN_RESULT scanResult;

    if (DRV_WIFI_isHibernateEnable())
    {
        SYS_CONSOLE_MESSAGE("The Wi-Fi module is in hibernate mode - command failed\r\n");
        return false;
    }

    if (!IwconfigUpdateCb(pCmdIO))
    {
        SYS_CONSOLE_MESSAGE("s_IwconfigCb structure set failed\r\n");
        return false;
    }

    // if user only typed in "iwconfig" with no other parameters
    if (argc == 1)
    {
        IwconfigDisplayStatus(pCmdIO);
        SYS_CONSOLE_MESSAGE("Try \"iwconfig --help\" or \"iwconfig -h\" for more information\r\n");
        return true;
    }

    if ((2 == argc) && (strcmp((char*)argv[1], "-h") == 0 || strcmp((char*)argv[1], "--help") == 0))
    {
        IwconfigDisplayHelp(pCmdIO);
    }
    else if ((2 <= argc) && (strcmp((char*)argv[1], "ssid") == 0))
    {
        return IwconfigSetSsid(pCmdIO, argc, argv);
    }
    else if ((2 <= argc) && (strcmp((char*)argv[1], "mode") == 0))
    {
       return IwconfigSetMode(pCmdIO, argc, argv);
    }
    else if ((2 <= argc) && (strcmp((char*)argv[1], "channel") == 0))
    {
        return IwconfigSetChannel(pCmdIO, argc, argv);
    }
    else if ((2 <= argc) && (strcmp((char*)argv[1], "power") == 0))
    {
        return IwconfigSetPower(pCmdIO, argc, argv);
    }
    else if ((2 <= argc) && (strcmp((char*)argv[1], "rts") == 0))
    {
        return IwconfigSetRTS(pCmdIO, argc, argv);
    }
    else if ((2 == argc) && (strcmp((char*)argv[1], "scan") == 0))
    {
        if (WFStartScan() == DRV_WIFI_SUCCESS)
        {
            SYS_CONSOLE_MESSAGE("Scan started ...\r\n");
        }
        else
        {
            if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
            {
                SYS_CONSOLE_MESSAGE("Scan is not allowed in Soft AP mode\r\n");
            }
            else
                SYS_CONSOLE_MESSAGE("Scan failed\r\n");
        }
        return false;
    }
    else if ((2 <= argc) && (strcmp((char*)argv[1], "scanget") == 0))
    {
        if (IS_SCAN_IN_PROGRESS(SCANCXT.scanState))
        {
            SYS_CONSOLE_MESSAGE("Scan in progress, please wait\r\n");
            return false;
        }

        if (!IS_SCAN_STATE_VALID(SCANCXT.scanState))
        {
            SYS_CONSOLE_MESSAGE("No scan results to display\r\n");
            return false;
        }

        if (argc != 3)
        {
            SYS_CONSOLE_MESSAGE("Need scan index to display (1 - N)\r\n");
            return false;
        }

        if (SCANCXT.numScanResults == 0)
        {
            SYS_CONSOLE_MESSAGE("No scan results to display\r\n");
            return false;
        }

        scanIndex = atoi((char *)argv[2]);

        if ((scanIndex > 0) && (scanIndex <= SCANCXT.numScanResults))
        {
            DRV_WIFI_ScanGetResult(scanIndex - 1, &scanResult);
            IwconfigDisplayScanResult(pCmdIO, scanIndex - 1, &scanResult);
        }
        else
        {
            SYS_CONSOLE_PRINT("Scan index must be between 1 and %d\r\n", SCANCXT.numScanResults);
            return false;
        }
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Unknown parameter\r\n");
        return false;
    }

    return true;
}

//==============================================================================
//                 Local Functions for "iwconfig" Command
//==============================================================================

/*****************************************************************************
 * FUNCTION: IwconfigSetCb
 *
 * RETURNS:  true or false
 *
 * PARAMS:   None
 *
 * NOTES:    Updates the s_IwconfigCb structure
 *****************************************************************************/
static bool IwconfigUpdateCb(SYS_CMD_DEVICE_NODE* pCmdIO)
{
    if (!s_IwconfigCbInitialized) // first time call of IwconfigUpdateCb
    {
        memset(&s_IwconfigCb, 0, sizeof(s_IwconfigCb));
        s_IwconfigCbInitialized = true;
    }

    DRV_WIFI_PowerSaveStateGet(&s_IwconfigCb.powerSaveState);
    if (s_IwconfigCb.powerSaveState == DRV_WIFI_PS_HIBERNATE)
    {
        SYS_CONSOLE_MESSAGE("WF device hibernated\r\n");
        return false;
    }

    DRV_WIFI_ConnectionStateGet(&s_IwconfigCb.connState);

    if ((s_IwconfigCb.connState == DRV_WIFI_CSTATE_NOT_CONNECTED) || (s_IwconfigCb.connState == DRV_WIFI_CSTATE_CONNECTION_PERMANENTLY_LOST))
    {
        s_IwconfigCb.isIdle = true;
    }
    else
    {
        s_IwconfigCb.isIdle = false;
    }

    return true;
}

/*****************************************************************************
 * FUNCTION: IwconfigDisplayStatus
 *
 * RETURNS:  None
 *
 * PARAMS:   None
 *
 * NOTES:    Responds to the user invoking "iwconfig" with no parameters
 *****************************************************************************/
static void IwconfigDisplayStatus(SYS_CMD_DEVICE_NODE* pCmdIO)
{
    uint8_t *p;
    uint8_t tmp;
    uint8_t connectionState;

    union
    {
        struct
        {
            uint8_t list[DRV_WIFI_MAX_CHANNEL_LIST_LENGTH];
            uint8_t num;
        } Channel;

        uint8_t Domain;

        struct
        {
            uint8_t name[DRV_WIFI_MAX_SSID_LENGTH + 1];
            uint8_t len;
        } Ssid;

        uint16_t RtsThreshold;

        DRV_WIFI_CONNECTION_CONTEXT Context;

        uint8_t NetworkType;

        struct
        {
            uint8_t beaconTimeout;
            uint8_t retryCount;
            uint8_t deauthAction;
            uint8_t beaconTimeoutAction;
        } ReconnectMode;

        struct
        {
            uint8_t securityType;
            uint8_t keyType;
        } Security;

        DRV_WIFI_SCAN_CONTEXT ScanContext;

        uint8_t MAC[6];
    } ws; // workspace

    // channel
    DRV_WIFI_ChannelListGet(ws.Channel.list, &ws.Channel.num);
    SYS_CONSOLE_MESSAGE("\tchannel:  ");
    p = ws.Channel.list;
    tmp = ws.Channel.num;
    while (--tmp > 0u)
    {
        SYS_CONSOLE_PRINT("%d,", *p);
        p++;
    }

    SYS_CONSOLE_PRINT("%d\r\n", *p);

    // domain
    DRV_WIFI_RegionalDomainGet(&ws.Domain);
    SYS_CONSOLE_MESSAGE("\tdomain:   ");
    if (ws.Domain == DRV_WIFI_DOMAIN_FCC)
    {
        SYS_CONSOLE_MESSAGE("FCC");
    }
    else if (ws.Domain == DRV_WIFI_DOMAIN_ETSI)
    {
        SYS_CONSOLE_MESSAGE("ETSI");
    }
    else if (ws.Domain == DRV_WIFI_DOMAIN_JAPAN)
    {
        SYS_CONSOLE_MESSAGE("JAPAN or OTHER");
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Unknown");
    }

    SYS_CONSOLE_MESSAGE("\r\n");

    // rts
    DRV_WIFI_RtsThresholdGet(&ws.RtsThreshold);
    SYS_CONSOLE_PRINT("\trts:      %d\r\n", ws.RtsThreshold);

    // mode
    DRV_WIFI_ConnectionStateGet(&connectionState);
    DRV_WIFI_NetworkTypeGet(&ws.NetworkType);
    SYS_CONSOLE_MESSAGE("\tmode:     ");
    ConsoleFlush();
    if (s_IwconfigCb.isIdle)
    {
        if (s_IwconfigCb.connState == DRV_WIFI_CSTATE_NOT_CONNECTED)
        {
            SYS_CONSOLE_MESSAGE("Idle");
        }
        else if (s_IwconfigCb.connState == DRV_WIFI_CSTATE_CONNECTION_PERMANENTLY_LOST)
        {
            SYS_CONSOLE_MESSAGE("Idle (connection permanently lost)");
        }
        else
        {
            SYS_CONSOLE_MESSAGE("Idle (undefined)");
        }
    }
    else
    {
        if (ws.NetworkType == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE)
        {
            if (s_IwconfigCb.connState == DRV_WIFI_CSTATE_CONNECTION_IN_PROGRESS)
            {
                SYS_CONSOLE_MESSAGE("Managed (connection in progress)");
            }
            else if (s_IwconfigCb.connState == DRV_WIFI_CSTATE_CONNECTED_INFRASTRUCTURE)
            {
                SYS_CONSOLE_MESSAGE("Managed (connected)");
            }
            else if (s_IwconfigCb.connState == DRV_WIFI_CSTATE_RECONNECTION_IN_PROGRESS)
            {
                SYS_CONSOLE_MESSAGE("Managed (reconnection in progress)");
            }
            else
            {
                SYS_CONSOLE_MESSAGE("Managed (undefined)");
            }
        }
        else if (ws.NetworkType == DRV_WIFI_NETWORK_TYPE_ADHOC)
        {
            if (s_IwconfigCb.connState == DRV_WIFI_CSTATE_CONNECTION_IN_PROGRESS)
            {
                SYS_CONSOLE_MESSAGE("AdHoc (connection in progress)");
            }
            else if (s_IwconfigCb.connState == DRV_WIFI_CSTATE_CONNECTED_ADHOC)
            {
                SYS_CONSOLE_MESSAGE("AdHoc (connected)");
            }
            else if (s_IwconfigCb.connState == DRV_WIFI_CSTATE_RECONNECTION_IN_PROGRESS)
            {
                SYS_CONSOLE_MESSAGE("AdHoc (reconnection in progress)");
            }
            else
            {
                SYS_CONSOLE_MESSAGE("AdHoc (undefined)");
            }
        }
        else if (ws.NetworkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
        {
            SYS_CONSOLE_MESSAGE("SoftAP");
        }
        else
        {
            SYS_CONSOLE_MESSAGE("Unknown");
        }
    }

    SYS_CONSOLE_MESSAGE("\r\n");

    // ssid
    DRV_WIFI_SsidGet(ws.Ssid.name, &ws.Ssid.len);
    ws.Ssid.name[ws.Ssid.len] = '\0';
    SYS_CONSOLE_MESSAGE("\tssid:     ");
    SYS_CONSOLE_PRINT("%s", ws.Ssid.name);
    SYS_CONSOLE_MESSAGE("\r\n");

    // power
    switch (s_IwconfigCb.powerSaveState) {
    case DRV_WIFI_PS_PS_POLL_DTIM_ENABLED:
        SYS_CONSOLE_MESSAGE("\tpwrsave:  Enabled\r\n");
        SYS_CONSOLE_MESSAGE("\tdtim rx:  Enabled\r\n");
        break;
    case DRV_WIFI_PS_PS_POLL_DTIM_DISABLED:
        SYS_CONSOLE_MESSAGE("\tpwrsave:  Enabled\r\n");
        SYS_CONSOLE_MESSAGE("\tdtim rx:  Disabled\r\n");
        break;
    case DRV_WIFI_PS_OFF:
        SYS_CONSOLE_MESSAGE("\tpwrsave:  Disabled\r\n");
        break;
    default:
        SYS_CONSOLE_PRINT("\tpwrsave:  Unknown %d\r\n", s_IwconfigCb.powerSaveState);
        break;
    }

    // bssid
    DRV_WIFI_ConnectContextGet(&ws.Context);
    SYS_CONSOLE_MESSAGE("\tbssid:    ");
    SYS_CONSOLE_PRINT("%02X:%02X:%02X:%02X:%02X:%02X\r\n",
                        ws.Context.bssid[0],
                        ws.Context.bssid[1],
                        ws.Context.bssid[2],
                        ws.Context.bssid[3],
                        ws.Context.bssid[4],
                        ws.Context.bssid[5]);

    // channel
    SYS_CONSOLE_MESSAGE("\tchannel:  ");
    SYS_CONSOLE_PRINT("%d\r\n", ws.Context.channel);

    // network type
    DRV_WIFI_NetworkTypeGet(&ws.NetworkType); // ws is an union, so need to call this function again
    SYS_CONSOLE_MESSAGE("\tnetwork:  ");
    if (ws.NetworkType == DRV_WIFI_NETWORK_TYPE_ADHOC) {
        SYS_CONSOLE_MESSAGE("AdHoc");
    } else if (ws.NetworkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP) {
        SYS_CONSOLE_MESSAGE("SoftAP");
    } else if (ws.NetworkType == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE) {
        SYS_CONSOLE_MESSAGE("Infrastructure");
    } else {
        SYS_CONSOLE_MESSAGE("Invalid");
    }

    SYS_CONSOLE_MESSAGE("\r\n");

    // retries
    DRV_WIFI_ReconnectModeGet(&ws.ReconnectMode.retryCount, &ws.ReconnectMode.deauthAction, &ws.ReconnectMode.beaconTimeout, &ws.ReconnectMode.beaconTimeoutAction);
    ConsoleFlush();
    SYS_CONSOLE_MESSAGE("\tretries:  ");
    if (ws.ReconnectMode.retryCount == DRV_WIFI_RETRY_FOREVER) {
        SYS_CONSOLE_MESSAGE("255 (Retry Forever)");
    }
    else if (ws.ReconnectMode.retryCount > 0 && ws.ReconnectMode.retryCount < DRV_WIFI_RETRY_FOREVER ) {
        SYS_CONSOLE_PRINT("%d", ws.ReconnectMode.retryCount);
    }
    else {
        SYS_CONSOLE_MESSAGE("Invalid");
    }
    SYS_CONSOLE_MESSAGE("\r\n");

    // security type
    SYS_CONSOLE_MESSAGE("\tsecurity: ");
    DRV_WIFI_SecurityTypeGet(&ws.Security.securityType);
    ConsoleFlush();

    switch (ws.Security.securityType) {
    case DRV_WIFI_SECURITY_OPEN:
        SYS_CONSOLE_MESSAGE("Open\r\n");
        break;
    case DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY:
        SYS_CONSOLE_MESSAGE("WPA PSK with key, auto-select\r\n");
        break;
    case DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:
        SYS_CONSOLE_MESSAGE("WPA PSK with pass phrase, auto-select\r\n");
        break;
    case DRV_WIFI_SECURITY_WPS_PUSH_BUTTON:
        SYS_CONSOLE_MESSAGE("WPS push button method\r\n");
        break;
    case  DRV_WIFI_SECURITY_WPS_PIN:
        SYS_CONSOLE_MESSAGE("WPS PIN method\r\n");
        break;
    case DRV_WIFI_SECURITY_EAP:
        SYS_CONSOLE_MESSAGE("WPA Enterprise\r\n");
        break;
    case DRV_WIFI_SECURITY_WEP_40:
    case DRV_WIFI_SECURITY_WEP_104:
        if (ws.Security.securityType == DRV_WIFI_SECURITY_WEP_40)
            SYS_CONSOLE_MESSAGE("WEP40, ");
        else if (ws.Security.securityType == DRV_WIFI_SECURITY_WEP_104)
            SYS_CONSOLE_MESSAGE("WEP104, ");
        else
            SYS_CONSOLE_MESSAGE("Invalid WEP mode, ");

        DRV_WIFI_WepKeyTypeGet(&ws.Security.keyType);

        if (ws.Security.keyType == DRV_WIFI_SECURITY_WEP_OPENKEY)
            SYS_CONSOLE_MESSAGE("Open Key\r\n");
        else if (ws.Security.keyType == DRV_WIFI_SECURITY_WEP_SHAREDKEY)
            SYS_CONSOLE_MESSAGE("Shared Key\r\n");
        else
            SYS_CONSOLE_MESSAGE("Invalid key type\r\n");

        break;
    default:
        SYS_CONSOLE_MESSAGE("Invalid security setting\r\n");
    }

    ConsoleFlush();
    // scan
    SYS_CONSOLE_MESSAGE("\tscan:     ");
    DRV_WIFI_ScanContextGet(&ws.ScanContext);
    if (ws.ScanContext.scanType == DRV_WIFI_PASSIVE_SCAN)
        SYS_CONSOLE_MESSAGE("Passive Scan\r\n");
    else if (ws.ScanContext.scanType == DRV_WIFI_ACTIVE_SCAN)
        SYS_CONSOLE_MESSAGE("Active Scan\r\n");
    else
        SYS_CONSOLE_MESSAGE("Invalid\r\n");

    // mac
    DRV_WIFI_MacAddressGet(ws.MAC);
    SYS_CONSOLE_MESSAGE("\tmac:      ");

    SYS_CONSOLE_PRINT("%02X:%02X:%02X:%02X:%02X:%02X\r\n",
                        ws.MAC[0],
                        ws.MAC[1],
                        ws.MAC[2],
                        ws.MAC[3],
                        ws.MAC[4],
                        ws.MAC[5]);
}

static bool IwconfigSetSsid(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc < 3)
    {
        SYS_CONSOLE_MESSAGE("Missing value for last parameter\r\n");
        return false;
    }

    if (argc > 3)
    {
        SYS_CONSOLE_MESSAGE("SSID may not contain space for this demo\r\n");
        return false;
    }

    if (strlen((const char *)argv[2]) > DRV_WIFI_MAX_SSID_LENGTH)
    {
        SYS_CONSOLE_PRINT("SSID may not contain more than %u characters\r\n", DRV_WIFI_MAX_SSID_LENGTH);
        return false;
    }

    DRV_WIFI_SsidSet((uint8_t *)argv[2], strlen((const char *)argv[2]));

    return true;
}

static bool IwconfigSetMode(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t networkType;

    DRV_WIFI_NetworkTypeGet(&networkType);

    if ((3 <= argc) && (strcmp((char*)argv[2], "idle") == 0))
    {
        if (s_IwconfigCb.isIdle)
        {
            SYS_CONSOLE_MESSAGE("Already in the idle mode\r\n");
        }
        else
        {
            DRV_WIFI_Disconnect();
        }
    }
    else if ((3 <= argc) && (strcmp((char*)argv[2], "managed") == 0))
    {
        if (s_IwconfigCb.isIdle)
        {
            DRV_WIFI_NetworkTypeSet(DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE);
            DRV_WIFI_Connect();
        }
        else
        {
            if (networkType == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE)
            {
                SYS_CONSOLE_MESSAGE("Already in the managed mode\r\n");
            }
            else
            {
                DRV_WIFI_Disconnect();
                DRV_WIFI_NetworkTypeSet(DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE);
                DRV_WIFI_Connect();
            }
        }
    }
    else if ((3 <= argc) && (strcmp((char*)argv[2], "adhoc") == 0))
    {
        if (s_IwconfigCb.isIdle)
        {
            DRV_WIFI_ADHOC_NETWORK_CONTEXT adhocContext;

            DRV_WIFI_NetworkTypeSet(DRV_WIFI_NETWORK_TYPE_ADHOC);
            adhocContext.beaconPeriod = DRV_WIFI_DEFAULT_ADHOC_BEACON_PERIOD;
            adhocContext.hiddenSsid = DRV_WIFI_DEFAULT_ADHOC_HIDDEN_SSID;
            adhocContext.mode = DRV_WIFI_DEFAULT_ADHOC_MODE;
            DRV_WIFI_AdhocContextSet(&adhocContext);
            DRV_WIFI_ReconnectModeSet(3,                              // retry N times to start or join AdHoc network
                                DRV_WIFI_DO_NOT_ATTEMPT_TO_RECONNECT, // do not attempt to reconnect on deauth from station
                                40,                                   // beacon timeout is 40 beacon periods
                                DRV_WIFI_ATTEMPT_TO_RECONNECT);       // reconnect on beacon timeout
            DRV_WIFI_Connect();
        }
        else
        {
            if (networkType == DRV_WIFI_NETWORK_TYPE_ADHOC)
            {
                SYS_CONSOLE_MESSAGE("Already in the adhoc mode\r\n");
            }
            else
            {
                DRV_WIFI_Disconnect();
                DRV_WIFI_NetworkTypeSet(DRV_WIFI_NETWORK_TYPE_ADHOC);
                DRV_WIFI_Connect();
            }
        }
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Unknown parameter\r\n");
        return false;
    }

    return true;
}

static bool IwconfigSetChannel(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    char *p1, *p2;
    char *p_channelList;
    uint8_t index = 0;
    uint16_t temp;
    uint8_t i;
    uint8_t regionalDomain;

    DRV_WIFI_RegionalDomainGet(&regionalDomain);

    if (argc < 3)
    {
        SYS_CONSOLE_MESSAGE("No channel numbers entered\r\n");
        return false;
    }

    if ( !s_IwconfigCb.isIdle )
    {
        SYS_CONSOLE_MESSAGE("Channel(s) can only be set in idle mode\r\n");
        return false;
    }

    p_channelList = argv[2];
    p1 = p2 = p_channelList;

    if ((3 <= argc) && (strcmp((char*)argv[2], "all") == 0))
    {
        DRV_WIFI_ChannelListSet((uint8_t *)p_channelList, 0); // reset to domain default channel list
        return true;
    }

    do
    {
        if ((p2 = strchr(p1, (int) ',')) != NULL)
        {
            *p2='\0';
            p2++;
        }

        temp = atoi(p1);
        if (temp == 0)
        {
            SYS_CONSOLE_MESSAGE("Invalid channel\r\n");
            return  false;
        }

        p1 = p2;
        p_channelList[index] = (uint8_t)temp;
        index++;

    } while (p2 != NULL);

    // Validate channels against current Domain
    if (regionalDomain == DRV_WIFI_DOMAIN_FCC)
    {
        for (i = 0; i < index; i++)
        {
            if (p_channelList[i] < 1 || p_channelList[i] > 11)
            {
                SYS_CONSOLE_MESSAGE("Invalid channel for FCC domain\r\n");
                return false;
            }
        }
    }
    else if (regionalDomain == DRV_WIFI_DOMAIN_ETSI)
    {
        for (i = 0; i < index; i++)
        {
            if ((p_channelList[i] < 1) || (p_channelList[i] > 13))
            {
                SYS_CONSOLE_MESSAGE("Invalid channel for ETSI domain\r\n");
                return false;
            }
        }
    }
    else if ((regionalDomain == DRV_WIFI_DOMAIN_JAPAN) || (regionalDomain == DRV_WIFI_DOMAIN_OTHER))
    {
        for (i = 0; i < index; i++)
        {
            if ((p_channelList[i] < 1) || (p_channelList[i] > 14))
            {
                SYS_CONSOLE_MESSAGE("Invalid channel for Japan or Other domain\r\n");
                return false;
            }
        }
    }

    DRV_WIFI_ChannelListSet((uint8_t *)p_channelList, index);

    return true;
}

static bool IwconfigSetPower(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    DRV_WIFI_PS_POLL_CONTEXT psPollContext;

    psPollContext.dtimInterval   = DRV_WIFI_DEFAULT_PS_DTIM_INTERVAL;
    psPollContext.listenInterval = DRV_WIFI_DEFAULT_PS_LISTEN_INTERVAL;
    psPollContext.useDtim        = true;

    if (argc < 3)
    {
        SYS_CONSOLE_MESSAGE("Missing value for last parameter\r\n");
        return false;
    }

    if ((3 <= argc) && (strcmp((char*)argv[2], "reenable") == 0))
    {    // reenable power saving
        DRV_WIFI_PsPollEnable(&psPollContext);
    }
    else if ((3 <= argc) && (strcmp((char*)argv[2], "disable") == 0))
    {    // disable power saving
        DRV_WIFI_PsPollDisable();
    }
    else if ((3 <= argc) && (strcmp((char*)argv[2], "unicast") == 0))
    {    // enable power saving but don't poll for DTIM
        psPollContext.useDtim = false;
        DRV_WIFI_PsPollEnable(&psPollContext);
    }
    else if ((3 <= argc) && (strcmp((char*)argv[2], "all") == 0))
    {    // enable power saving and poll for DTIM
        DRV_WIFI_PsPollEnable(&psPollContext);
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Unknown parameter\r\n");
        return false;
    }

    return true;
}

static bool IwconfigSetRTS(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint16_t rtsThreshold;

    if (argc < 3)
    {
        SYS_CONSOLE_MESSAGE("Missing value for last parameter\r\n");
        return false;
    }

    rtsThreshold = atoi(argv[2]);
    if (rtsThreshold > 255)
    {
        return  false;
    }

    DRV_WIFI_RtsThresholdSet(rtsThreshold);

    return true;
}

static int Cmd_EraseConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    DRV_WIFI_ConfigDataErase();
    SYS_CONSOLE_MESSAGE("--Done\r\n");
    return true;
}

static int Cmd_SaveConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    DRV_WIFI_ConfigDataSave();
    SYS_CONSOLE_MESSAGE("--Done\r\n");
    return true;
}

static int Cmd_ReadConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (DRV_WIFI_isHibernateEnable())
    {
        SYS_CONSOLE_MESSAGE("The Wi-Fi module is in hibernate mode - command failed\r\n");
        return false;
    }
    if (MRF24W_IsMACInit() == false)
    {
        SYS_CONSOLE_MESSAGE("The Wi-Fi module is not intialzed - command failed\r\n");
        return false;
    }

#if defined(SYS_CONSOLE_ENABLE) || defined(SYS_CONSOLE_INSTANCES_NUMBER)
    SYS_CONSOLE_MESSAGE("\r\n=== Following data is run time config data: ===\r\n");
    OutputDemoHeader();
    SYS_CONSOLE_MESSAGE("\r\n=== Following data is read from storage space: ===\r\n");
    DRV_WIFI_ConfigDataPrint();    
#endif

    return true;
}

static void IwconfigDisplayHelp(SYS_CMD_DEVICE_NODE* pCmdIO)
{
    strcpy(s_helpInfo, "Usage:\r\n  iwconfig\r\n");
    strcat(s_helpInfo, "  iwconfig ssid <ssid>\r\n");
    strcat(s_helpInfo, "  iwconfig mode <mode>\r\n");
    strcat(s_helpInfo, "  iwconfig channel <channels>\r\n");
    strcat(s_helpInfo, "  iwconfig power <power saving mode>\r\n");
    strcat(s_helpInfo, "  iwconfig rts <rts threshold>\r\n");
    strcat(s_helpInfo, "  iwconfig scan\r\n");
    strcat(s_helpInfo, "  iwconfig scanget <scan index>\r\n");

    strcat(s_helpInfo, "Mode:\r\n  managed/idle/adhoc\r\n");
    strcat(s_helpInfo, "Channels:\r\n  valid channels depend on region, have to be comma-separated,\r\n");
    strcat(s_helpInfo, "  use \"all\" set to all valid channels\r\n");
    strcat(s_helpInfo, "  Ex: iwconfig channel 1, 6, 11\r\n");
    strcat(s_helpInfo, "      iwconfig channel all\r\n");
    strcat(s_helpInfo, "Power saving mode:\r\n  reenable/disable/unicast/all\r\n");
    strcat(s_helpInfo, "RTS threshold:\r\n  maximum value 255\r\n");

    //SYS_CONSOLE_PRINT("Length of s_helpInfo is: %u\r\n", strlen(s_helpInfo));

    SYS_CONSOLE_PRINT("%s", s_helpInfo);
}

static void IwconfigDisplayScanResult(SYS_CMD_DEVICE_NODE* pCmdIO, uint16_t index, DRV_WIFI_SCAN_RESULT *p_scanResult)
{
    int i;
    uint32_t rate;

    SYS_CONSOLE_MESSAGE("\r\n======================\r\n");
    SYS_CONSOLE_PRINT("Scan Result  %d\r\n", index + 1);

    // ssid
    if (p_scanResult->ssidLen < DRV_WIFI_MAX_SSID_LENGTH)
    {
        p_scanResult->ssid[p_scanResult->ssidLen] = '\0'; // ensure string terminator
        SYS_CONSOLE_PRINT("SSID:     %s\r\n", p_scanResult->ssid);
    }
    else
    {
        SYS_CONSOLE_MESSAGE("SSID:     ");
        for (i = 0; i < p_scanResult->ssidLen; i++)
            SYS_CONSOLE_PRINT("%c", p_scanResult->ssid[i]);
        SYS_CONSOLE_MESSAGE("\r\n");
    }

    // bssid
    SYS_CONSOLE_PRINT("BSSID:    %02x %02x %02x %02x %02x %02x\r\n",
                          p_scanResult->bssid[0], 
                          p_scanResult->bssid[1],
                          p_scanResult->bssid[2],
                          p_scanResult->bssid[3],
                          p_scanResult->bssid[4],
                          p_scanResult->bssid[5]);

    // BSS Type
    SYS_CONSOLE_MESSAGE("BSS Type: ");
    if (p_scanResult->bssType == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE)
    {
        SYS_CONSOLE_MESSAGE("Infrastructure\r\n");
    }
    else
    {
        SYS_CONSOLE_MESSAGE("AdHoc\r\n");
    }

    // channel
    SYS_CONSOLE_PRINT("Channel:  %d\r\n", p_scanResult->channel);

    // beacon period
    SYS_CONSOLE_PRINT("Beacon:   %d ms\r\n", p_scanResult->beaconPeriod);

    // basic rates
    SYS_CONSOLE_MESSAGE("Rates:    ");
    for (i = 0; i < p_scanResult->numRates; ++i)
    {
        rate = (p_scanResult->basicRateSet[i] & ~0x80) * 500000;
        if ((rate % 1000000) == 0)
        {
            SYS_CONSOLE_PRINT("%d", rate / 1000000);
        }
        else
        {
            SYS_CONSOLE_PRINT("%d.5", rate / 1000000);
        }
        if (i + 1 < p_scanResult->numRates) {
            SYS_CONSOLE_MESSAGE(" ,");
        }
    }

    SYS_CONSOLE_MESSAGE("(mbps)\r\n");

    // security
    SYS_CONSOLE_MESSAGE("Security: ");
    if ((p_scanResult->apConfig & 0x10) > 0)        // if privacy bit set
    {
        if ((p_scanResult->apConfig & 0x80) > 0)    // if WPA2 bit is set
        {
            SYS_CONSOLE_MESSAGE("WPA2\r\n");
        }
        else if ((p_scanResult->apConfig & 0x40) > 0)    // if WPA bit is set
        {
            SYS_CONSOLE_MESSAGE("WPA\r\n");
        }
        else
        {
            SYS_CONSOLE_MESSAGE("WEP\r\n");
        }
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Open\r\n");
    }

    // preamble
    SYS_CONSOLE_MESSAGE("Preamble: ");
    if ((p_scanResult->apConfig & 0x20) > 0)
    {
        SYS_CONSOLE_MESSAGE("Long\r\n");
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Short\r\n");
    }

    // rssi
    SYS_CONSOLE_PRINT("RSSI:     %d\r\n", p_scanResult->rssi);
}

static void ConsoleFlush(void)
{
#if defined(SYS_CONSOLE_ENABLE) || defined(SYS_CONSOLE_INSTANCES_NUMBER)
    do {
        SYS_DEVCON_Tasks(sysObj.sysDevcon);
        SYS_CONSOLE_Tasks(sysObj.sysConsole0);
    } while (SYS_CONSOLE_Status(sysObj.sysConsole0)!= SYS_STATUS_READY);
#endif
}

#endif /* TCPIP_STACK_COMMANDS_WIFI_ENABLE */

// DOM-IGNORE-END
