/*******************************************************************************
  MRF24WN CLI (Based on System Cmder) Implementation

  File Name:  
    wdrv_mrf24wn_cli.c  
  
  Summary:
    Module for Microchip TCP/IP Stack
    
  Description:
    -Provides access to MRF24W WiFi controller
    -Reference: MRF24W Data sheet, IEEE 802.11 Standard
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

//==============================================================================
//                                  Includes
//==============================================================================

#include <ctype.h>

#include "wdrv_mrf24wn_common.h"
#include "wdrv_mrf24wn_main.h"
#include "wdrv_mrf24wn_priv.h"

#if defined(TCPIP_STACK_COMMANDS_WIFI_ENABLE)

// iwconfig control block
typedef struct
{
    uint8_t powerSaveState; // power save state
    uint8_t connState; // connection state
    bool isIdle; // true if connState is WDRV_CSTATE_NOT_CONNECTED
} WDRV_IWCONFIG_CB;

static struct
{
    int8_t scanState;
    uint16_t numScanResults;
    uint16_t displayIdx;
    bool getScanResultList;
} s_scanResult;

static struct
{
    uint16_t Channel;

    uint8_t Domain;

    struct
    {
        uint8_t name[WDRV_MAX_SSID_LENGTH+1];
        uint8_t len;
    } Ssid;

    uint8_t NetworkType;

    WDRV_CONNECTION_CONTEXT Context;

    uint8_t SecurityType;

    WDRV_SCAN_CONTEXT ScanContext;

    uint8_t MAC[6];
} s_ctx; 

static WDRV_IWCONFIG_CB s_IwconfigCb;
static bool s_IwconfigCbInitialized = false;
static char s_helpInfo[800];

static bool IwconfigUpdateCb(SYS_CMD_DEVICE_NODE* pCmdIO);
static void IwconfigDisplayHelp(SYS_CMD_DEVICE_NODE* pCmdIO);
static void IwconfigDisplayStatus(SYS_CMD_DEVICE_NODE* pCmdIO);
static bool IwconfigSetSsid(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static bool IwconfigSetMode(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static bool IwconfigSetChannel(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static bool IwconfigSetSecurity(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static bool IwconfigSetPower(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_Iwconfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_RFTest(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_ReadConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_EraseConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_SaveConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_GetMacAddr(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int Cmd_OTAUpdate(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void DisplayScanResult(SYS_CMD_DEVICE_NODE* pCmdIO, uint16_t index, WDRV_SCAN_RESULT *p_scanResult);
static void WDRV_CLI_MacAddressGet(uint8_t *p_mac);
static void WDRV_CLI_PowerSaveEnable(bool enable);

static const SYS_CMD_DESCRIPTOR wfCmdTbl[] =
{
    {"iwconfig",     Cmd_Iwconfig,       ": Wi-Fi configuration"},
    {"rftest",       Cmd_RFTest,         ": RF test"},
    {"mac",          Cmd_GetMacAddr,     ": Get MAC address"},
    {"readconf",     Cmd_ReadConfig,     ": Read from storage"},
    {"eraseconf",    Cmd_EraseConfig,    ": Erase storage"},
    {"saveconf",     Cmd_SaveConfig,     ": Save storage"},
    {"ota",          Cmd_OTAUpdate,      ": Over The Air FW update"},
};

static int Cmd_GetMacAddr(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t macAddr[6];
    
    memset(macAddr, 0x00, sizeof(macAddr));
    WDRV_CLI_MacAddressGet(macAddr);
    SYS_CONSOLE_PRINT("MAC address: %02x:%02x:%02x:%02x:%02x:%02x\r\n", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    return true;
}

static int Cmd_OTAUpdate(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    IPV4_ADDR addr;
    uint8_t tmp = 0;
	
    if (!TCPIP_Helper_StringToIPAddress(g_ota_host_addr_str, &addr)) {
        WDRV_ASSERT(false, "Failed to convert string to valid IPv4 address\r\n");
    }

    tmp =  addr.v[0];
    addr.v[0] = addr.v[3];
    addr.v[3] = tmp;

    tmp =  addr.v[1];
    addr.v[1] = addr.v[2];
    addr.v[2] = tmp;  

    g_ota_host_addr = addr.Val;
	
    WDRV_EXT_CmdFWUpdate();
    return true;
}

static void WDRV_CLI_PowerSaveStateGet(uint8_t *p_powerSaveState)
{
    bool power_status;

    WDRV_EXT_CmdPowerSaveGet(&power_status);
    if (power_status == true) {   
        *p_powerSaveState = WDRV_PS_SLEEP;
    } else if (power_status == false) {
        *p_powerSaveState = WDRV_PS_ACTIVE;
    } else {
        WDRV_ASSERT(false, "never happen");
    }
}

static void WDRV_CLI_SsidGet(uint8_t *p_ssid, uint8_t *p_ssidLength)
{
    memcpy(p_ssid, p_wdrv_configData->netSSID, p_wdrv_configData->SsidLength);
    p_ssid[p_wdrv_configData->SsidLength] = 0;
    *p_ssidLength = p_wdrv_configData->SsidLength;
}

static void WDRV_CLI_SecurityTypeGet(uint8_t *p_securityType)
{
    *p_securityType = p_wdrv_configData->SecurityMode;
}

static void WDRV_CLI_MacAddressGet(uint8_t *p_mac)
{
    WDRV_EXT_CmdMacAddressGet(p_mac);
}

static void WDRV_CLI_ConnectionStateGet(uint8_t *p_state)
{
    *p_state = WDRV_ConnectStatus_Get();
}

static void WDRV_CLI_ChannelGet(uint16_t *bssChannel)
{
    WDRV_EXT_CmdConnectContextChannelGet(bssChannel);
}

static void WDRV_CLI_NetworkTypeGet(uint8_t *p_networkType)
{
    *p_networkType = p_wdrv_configData->networkType;
}

static void WDRV_CLI_ConnectContextBssidGet(uint8_t *bssid)
{
    WDRV_EXT_CmdConnectContextBssidGet(bssid);
}

static void WDRV_CLI_Connect(void)
{
    ConnectToNetwork();
}

static void WDRV_CLI_Disconnect(void)
{
    WDRV_EXT_CmdDisconnect();
}

static void WDRV_CLI_SsidSet(uint8_t *p_ssid,  uint16_t ssidLength)
{
    if (ssidLength <= 32) {
        memcpy(p_wdrv_configData->netSSID, p_ssid, ssidLength);
        p_wdrv_configData->SsidLength = ssidLength;
        if (p_wdrv_configData->SsidLength < 32)
            p_wdrv_configData->netSSID[ssidLength] = 0;
    } else {
        WDRV_DBG_ERROR_PRINT(("Invalid SSID length error %d\r\n", ssidLength));
    }
}

static void WDRV_CLI_SecNoneSet(void)
{
    p_wdrv_configData->SecurityMode = WDRV_SECURITY_OPEN;
}

static void WDRV_CLI_SecWEPSet(uint8_t *key, uint16_t len)
{
    if (len == 10) {
        p_wdrv_configData->SecurityMode = WDRV_SECURITY_WEP_40;
        p_wdrv_configData->SecurityKeyLength = len;
        memcpy(p_wdrv_configData->SecurityKey, key, len);
    } else if (len == 26) {
        p_wdrv_configData->SecurityMode = WDRV_SECURITY_WEP_104;
        p_wdrv_configData->SecurityKeyLength = len;
        memcpy(p_wdrv_configData->SecurityKey, key, len);
    } else {
        WDRV_DBG_ERROR_PRINT(("Invalid WEP key length %d\r\n", len));
    }
}

static void WDRV_CLI_SecWPASet(uint8_t *key, uint16_t len)
{
    if ((len >= 8) && (len <= 63)) {
        p_wdrv_configData->SecurityMode = WDRV_SECURITY_WPA_WITH_PASS_PHRASE;
        p_wdrv_configData->SecurityKeyLength = len;
        memcpy(p_wdrv_configData->SecurityKey, key, len);
    } else {
        WDRV_DBG_ERROR_PRINT(("Invalid WPA passphrase length %d\r\n", len));
    }
}

static void WDRV_CLI_SecWPA2Set(uint8_t *key, uint16_t len)
{
    if ((len >= 8) && (len <= 63)) {
        p_wdrv_configData->SecurityMode = WDRV_SECURITY_WPA2_WITH_PASS_PHRASE;
        p_wdrv_configData->SecurityKeyLength = len;
        memcpy(p_wdrv_configData->SecurityKey, key, len);
    } else {
        WDRV_DBG_ERROR_PRINT(("Invalid WPA2 passphrase length %d\r\n", len));
    }
}

static void WDRV_CLI_SecWPSPINSet(uint8_t *key, uint16_t len)
{
    if (len == 8) {
        p_wdrv_configData->SecurityMode = WDRV_SECURITY_WPS_PIN;
        p_wdrv_configData->SecurityKeyLength = len;
        memcpy(p_wdrv_configData->SecurityKey, key, len);
    } else {
        WDRV_DBG_ERROR_PRINT(("Invalid PIN length %d\r\n", len));
    }
}

static void WDRV_CLI_SecWPSPushButtonSet(void)
{
    p_wdrv_configData->SecurityMode = WDRV_SECURITY_WPS_PUSH_BUTTON;
}

static void WDRV_CLI_ChannelSet(uint16_t channel)
{
    if (p_wdrv_configData->networkType == WDRV_NETWORK_TYPE_SOFT_AP) {
        WDRV_EXT_CmdChannelSet(channel); 
    } else {
        SYS_CONSOLE_MESSAGE("set channel is not supported for Infrastructure mode.");
    }
}

static void WDRV_CLI_PowerSaveEnable(bool enable)
{
    if (enable)
        WDRV_EXT_CmdPowerSavePut(true); 
    else
        WDRV_EXT_CmdPowerSavePut(false); 

    g_wdrv_config.powerSaveEnabled = enable;
}

static uint16_t StartScan(void)
{
    WDRV_EXT_CmdScanStart();
    return WDRV_SUCCESS;
}

static uint16_t ScanGet(void)
{
    if (true == WDRV_EXT_ScanIsInProgress())
        return WDRV_ERROR;
    
    WDRV_EXT_CmdScanGet(&s_scanResult.numScanResults);
    return WDRV_SUCCESS;
}

static void WDRV_CLI_ScanGetResult(uint8_t listIndex, WDRV_SCAN_RESULT *p_scanResult)
{
	WDRV_EXT_ScanGetResult(listIndex, p_scanResult);
}

bool WDRV_CLI_Init(void)
{
    if (SYS_CMD_ADDGRP(wfCmdTbl, sizeof(wfCmdTbl)/sizeof(*wfCmdTbl), "wifi", ": wifi commands") == -1) 
        return false;

    return true;
}

static int Cmd_RFTest(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
   WDRV_EXT_CmdRFTest(argc, argv);
   return true;
}

static int Cmd_Iwconfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint16_t scanIndex;
    WDRV_SCAN_RESULT scanResult;

    if (!IwconfigUpdateCb(pCmdIO)) {
        SYS_CONSOLE_MESSAGE("s_IwconfigCb structure set failed\r\n");
        return false;
    }

    // if user only typed in iwconfig with no other parameters
    if (argc == 1) {
        IwconfigDisplayStatus(pCmdIO);
        SYS_CONSOLE_MESSAGE("Try \"iwconfig --help\" or \"iwconfig -h\" for more information\r\n");
        return true;
    }

    if ((2 == argc) && (strcmp((char*)argv[1], "-h") == 0 || strcmp((char*)argv[1], "--help") == 0)) {
        IwconfigDisplayHelp(pCmdIO);
    } else if ((2 <= argc) && (strcmp((char*)argv[1], "ssid") == 0)) {
        return IwconfigSetSsid(pCmdIO, argc, argv);
    } else if ((2 <= argc) && (strcmp((char*)argv[1], "mode") == 0)) {
       return IwconfigSetMode(pCmdIO, argc, argv);
    } else if ((2 <= argc) && (strcmp((char*)argv[1], "channel") == 0)) {
        return IwconfigSetChannel(pCmdIO, argc, argv);
    } else if ((2 <= argc) && (strcmp((char*)argv[1], "security") == 0)) {
        return IwconfigSetSecurity(pCmdIO, argc, argv);        
    } else if ((2 <= argc) && (strcmp((char*)argv[1], "power") == 0)) {
        return IwconfigSetPower(pCmdIO, argc, argv);
    } else if ((2 == argc) && (strcmp((char*)argv[1], "scan") == 0)) {
        if (StartScan() == WDRV_SUCCESS) {
            SYS_CONSOLE_MESSAGE("Scan started ...\r\n");
            s_scanResult.displayIdx = 0;
			s_scanResult.getScanResultList = true;
        } else {
            SYS_CONSOLE_MESSAGE("Scan failed.\r\n");
        }
        return false;
    } else if ( (2 <= argc) && (strcmp((char*)argv[1], "scanget") == 0)) {
        if (ScanGet() == WDRV_ERROR) {
            SYS_CONSOLE_MESSAGE("Scan is still in progress, please wait...\r\n");
        }
        if (s_scanResult.numScanResults == 0) {
            SYS_CONSOLE_MESSAGE("No APs are found.\r\n");
            return false;
        } else {
            SYS_CONSOLE_PRINT("Found %d APs.\r\n", s_scanResult.numScanResults);
        }
        
        if (argc != 3) {
            SYS_CONSOLE_MESSAGE("Type scan index from 1 to N.\r\n");
            return false;
        }

        scanIndex = atoi((char *)argv[2]);
        if ((scanIndex > 0) && (scanIndex <= s_scanResult.numScanResults)) {
            WDRV_CLI_ScanGetResult(scanIndex-1, &scanResult);
            DisplayScanResult(pCmdIO, scanIndex-1, &scanResult);
        } else {
            SYS_CONSOLE_PRINT("Scan index must be between 1 and %d\r\n", s_scanResult.numScanResults);
            return false;
        }
    } else  {
        SYS_CONSOLE_MESSAGE("Unknown parameter\r\n");
        return false;
    }

    return true;
}

static bool IwconfigUpdateCb(SYS_CMD_DEVICE_NODE* pCmdIO)
{
    if (!s_IwconfigCbInitialized) { // first time call of IwconfigUpdateCb
        memset(&s_IwconfigCb, 0, sizeof(s_IwconfigCb));
        s_IwconfigCbInitialized = true;
    }

    WDRV_CLI_PowerSaveStateGet(&s_IwconfigCb.powerSaveState);
    if (s_IwconfigCb.powerSaveState == WDRV_PS_HIBERNATE) {
        WDRV_DBG_INFORM_MESSAGE(("WF device hibernated\r\n"));
        return false;
    }

    WDRV_CLI_ConnectionStateGet(&s_IwconfigCb.connState);

    if ((s_IwconfigCb.connState == WDRV_CSTATE_NOT_CONNECTED) || 
        (s_IwconfigCb.connState == WDRV_CSTATE_CONNECTION_PERMANENTLY_LOST)) {
        s_IwconfigCb.isIdle = true;
    } else {
        s_IwconfigCb.isIdle = false;
    }

    return true;
}

static void IwconfigDisplayHelp(SYS_CMD_DEVICE_NODE* pCmdIO)
{
    strcpy(s_helpInfo, "Usage:\r\n  iwconfig\r\n");
    strcat(s_helpInfo, "  iwconfig ssid <ssid>\r\n");
    strcat(s_helpInfo, "  iwconfig mode <mode>\r\n");
    strcat(s_helpInfo, "  iwconfig security <security_mode> <key>/<pin>\r\n");
    strcat(s_helpInfo, "  iwconfig power <enable/disable>\r\n");
    strcat(s_helpInfo, "  iwconfig scan\r\n");
    strcat(s_helpInfo, "  iwconfig scanget <scan_index>\r\n");

    strcat(s_helpInfo, "<ssid>:\r\n");
    strcat(s_helpInfo, "  32 characters string - no blank or space allowed in this demo\r\n");
    strcat(s_helpInfo, "<mode>:\r\n");
    strcat(s_helpInfo, "  managed/idle\r\n");
    strcat(s_helpInfo, "<security_mode>:\r\n");
    strcat(s_helpInfo, "  open/wep40/wep104/wpa/wpa2/pin/pbc\r\n");
    strcat(s_helpInfo, "  No blank or space allowed in <key> in current console commands\r\n");
    strcat(s_helpInfo, "  Ex: iwconfig security open\r\n");
    strcat(s_helpInfo, "      iwconfig security wep40 5AFB6C8E77\r\n");
    strcat(s_helpInfo, "      iwconfig security wep104 90E96780C739409DA50034FCAA\r\n");
    strcat(s_helpInfo, "      iwconfig security wpa microchip_psk\r\n");
    strcat(s_helpInfo, "      iwconfig security wpa2 microchip_psk\r\n");
    strcat(s_helpInfo, "      iwconfig security pin 12390212\r\n");
    strcat(s_helpInfo, "      iwconfig security pbc\r\n");

    //SYS_CONSOLE_PRINT("Length of s_helpInfo is: %u\r\n", strlen(s_helpInfo));

    SYS_CONSOLE_PRINT("%s", s_helpInfo);
}

static void IwconfigDisplayStatus(SYS_CMD_DEVICE_NODE* pCmdIO)
{
    int i;

    // Channel
    WDRV_CLI_ChannelGet(&s_ctx.Channel);
    SYS_CONSOLE_PRINT("\tFrequency: %d\r\n", s_ctx.Channel);

    // Mode
    WDRV_CLI_NetworkTypeGet(&s_ctx.NetworkType);
    SYS_CONSOLE_MESSAGE("\tMode:      ");
    if (s_IwconfigCb.isIdle) {
        if (s_IwconfigCb.connState == WDRV_CSTATE_NOT_CONNECTED) {
            SYS_CONSOLE_MESSAGE("Idle");
        } else if (s_IwconfigCb.connState == WDRV_CSTATE_CONNECTION_PERMANENTLY_LOST) {
            SYS_CONSOLE_MESSAGE("Idle (connection permanently lost)");
        } else {
            SYS_CONSOLE_MESSAGE("Idle (undefined)");
        } 
    } else {
        if (s_ctx.NetworkType == WDRV_NETWORK_TYPE_INFRASTRUCTURE) {
            if (s_IwconfigCb.connState == WDRV_CSTATE_CONNECTED_INFRASTRUCTURE) {
                SYS_CONSOLE_MESSAGE("Managed (connected)");
            } else if (s_IwconfigCb.connState == WDRV_CSTATE_CONNECTION_IN_PROGRESS) {
                SYS_CONSOLE_MESSAGE("Managed (connection in progress)");
            } else  {
                SYS_CONSOLE_MESSAGE("Managed (undefined)");
            }    
        } else if (s_ctx.NetworkType == WDRV_NETWORK_TYPE_ADHOC) {
            if (s_IwconfigCb.connState == WDRV_CSTATE_CONNECTED_ADHOC) {
                SYS_CONSOLE_MESSAGE("AdHoc (connected)");
            } else {
                SYS_CONSOLE_MESSAGE("AdHoc (undefined)");
            }
        } else if (s_ctx.NetworkType == WDRV_NETWORK_TYPE_SOFT_AP) {
            SYS_CONSOLE_MESSAGE("SoftAP");
        } else {
            SYS_CONSOLE_MESSAGE("Unknown");
        }
    }
    SYS_CONSOLE_MESSAGE("\r\n");

    // SSID
    WDRV_CLI_SsidGet(s_ctx.Ssid.name, &s_ctx.Ssid.len);
    s_ctx.Ssid.name[s_ctx.Ssid.len] = '\0';
    SYS_CONSOLE_MESSAGE("\tSSID:      ");
    
    for (i = 0; i < s_ctx.Ssid.len; i++) {
        SYS_CONSOLE_PRINT("%c", s_ctx.Ssid.name[i]);
    }
    SYS_CONSOLE_MESSAGE("\r\n");

    // Power
    switch (s_IwconfigCb.powerSaveState) {
        case WDRV_PS_ACTIVE:
            SYS_CONSOLE_MESSAGE("\tPwrSave:   Disabled.\r\n");
            break;
        case WDRV_PS_SLEEP:
            SYS_CONSOLE_MESSAGE("\tPwrSave:   Enabled.\r\n");
            break;
        case WDRV_PS_HIBERNATE:
            SYS_CONSOLE_MESSAGE("\tPwrSave:   Enabled. In Hibernate mode.\r\n");
            break;
        default:
            SYS_CONSOLE_PRINT("\tPwrSave:   Unknown %d\r\n", s_IwconfigCb.powerSaveState);
            break;
    }

	// BSSID
    WDRV_CLI_ConnectContextBssidGet(s_ctx.Context.bssid);
    SYS_CONSOLE_MESSAGE("\tBSSID:     ");
    SYS_CONSOLE_PRINT("%02X:%02X:%02X:%02X:%02X:%02X\r\n", 
        s_ctx.Context.bssid[0],s_ctx.Context.bssid[1],s_ctx.Context.bssid[2],
        s_ctx.Context.bssid[3],s_ctx.Context.bssid[4],s_ctx.Context.bssid[5]);

    // Network Type
    SYS_CONSOLE_MESSAGE("\tNetwork:   ");
    if (s_ctx.NetworkType == WDRV_NETWORK_TYPE_ADHOC) {
        SYS_CONSOLE_MESSAGE("AdHoc");
    } else if (s_ctx.NetworkType == WDRV_NETWORK_TYPE_SOFT_AP) {
        SYS_CONSOLE_MESSAGE("SoftAP");
    } else if (s_ctx.NetworkType == WDRV_NETWORK_TYPE_INFRASTRUCTURE) {
        SYS_CONSOLE_MESSAGE("Infrastructure");
    } else {
        SYS_CONSOLE_MESSAGE("Invalid");
    }
    SYS_CONSOLE_MESSAGE("\r\n");

	// Security type
    SYS_CONSOLE_MESSAGE("\tSecurity:  ");
    WDRV_CLI_SecurityTypeGet(&s_ctx.SecurityType);
    switch (s_ctx.SecurityType) {
        case WDRV_SECURITY_OPEN:
            SYS_CONSOLE_MESSAGE("Open\r\n");
            break;
        case WDRV_SECURITY_WEP_40:
        case WDRV_SECURITY_WEP_104:
            if (s_ctx.SecurityType == WDRV_SECURITY_WEP_40) {
                SYS_CONSOLE_MESSAGE("WEP40, ");
            } else if (s_ctx.SecurityType == WDRV_SECURITY_WEP_104) {
                SYS_CONSOLE_MESSAGE("WEP104, ");
            } else {
                SYS_CONSOLE_MESSAGE("Invalid WEP mode, ");
            }
            break;
        case WDRV_SECURITY_WPA_WITH_PASS_PHRASE:
            SYS_CONSOLE_MESSAGE("WPA PSK with pass phrase\r\n");
            break;
        case WDRV_SECURITY_WPA2_WITH_PASS_PHRASE:
            SYS_CONSOLE_MESSAGE("WPA2 PSK with pass phrase\r\n");
            break;
        case WDRV_SECURITY_WPS_PIN:
            SYS_CONSOLE_MESSAGE("WPS PIN method\r\n");
            break;
        case WDRV_SECURITY_WPS_PUSH_BUTTON:
            SYS_CONSOLE_MESSAGE("WPS push button method\r\n");
            break;
        default:
            SYS_CONSOLE_MESSAGE("Invalid security setting\r\n");
    }

	// MAC
    WDRV_CLI_MacAddressGet(s_ctx.MAC);
    SYS_CONSOLE_MESSAGE("\tMAC:       ");
    SYS_CONSOLE_PRINT("%02X:%02X:%02X:%02X:%02X:%02X\r\n", 
		s_ctx.MAC[0],s_ctx.MAC[1],s_ctx.MAC[2],s_ctx.MAC[3],s_ctx.MAC[4],s_ctx.MAC[5]);
}

static bool IwconfigSetSsid(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc < 3) {
        SYS_CONSOLE_MESSAGE("Missing value for last parameter\r\n");
        return false;
    }

    if (argc > 3) {
        SYS_CONSOLE_MESSAGE("SSID may not contain space for this demo\r\n");
        return false;
    }

    WDRV_CLI_SsidSet((uint8_t *)argv[2], strlen((char *)argv[2]));

    return true;
}

static bool IwconfigSetMode(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t networkType;

    WDRV_CLI_NetworkTypeGet(&networkType);

    if ((3 <= argc) && (strcmp((char*)argv[2], "idle") == 0)) {
        if ( s_IwconfigCb.isIdle ) {
            SYS_CONSOLE_MESSAGE("Already in the idle mode\r\n");
        } else {
            WDRV_CLI_Disconnect();
        }
    } else if ((3 <= argc) && (strcmp((char*)argv[2], "managed") == 0)) {
        if ( s_IwconfigCb.isIdle ) {
            WDRV_CLI_Connect();
        } else {
            WDRV_CLI_NetworkTypeGet(&networkType);
            if (networkType == WDRV_NETWORK_TYPE_INFRASTRUCTURE) {
                SYS_CONSOLE_MESSAGE("Already in the managed mode. Disconnect first.\r\n");
            } 
        }
    } else if ((3 <= argc) && (strcmp((char*)argv[2], "adhoc") == 0)) {
        // TODO : Unsupported for now. 
    } else {
        SYS_CONSOLE_MESSAGE("Unknown parameter\r\n");
        return false;
    }

    return true;
}

static bool IwconfigSetChannel(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc < 3) {
        SYS_CONSOLE_MESSAGE("No channel numbers entered\r\n");
        return false;
    }

    if (!s_IwconfigCb.isIdle) {
        SYS_CONSOLE_MESSAGE("Channel(s) can only be set in idle mode\r\n");
        return false;
    }

    WDRV_CLI_ChannelSet((uint16_t)(*argv[1]));

    return true;
}

static bool IwconfigSetSecurity(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (!s_IwconfigCb.isIdle) {
        SYS_CONSOLE_MESSAGE("Security mode can only be set in idle mode\r\n");
        return false;
    }

    if ((3 <= argc) && (strcmp((char *)argv[2], "open") == 0)) {
        WDRV_CLI_SecNoneSet();
    } else if ((4 <= argc) && (strcmp((char *)argv[2], "wep40") == 0)) {
        if (strlen((char *)argv[3]) != 10) {
            SYS_CONSOLE_MESSAGE("WEP40 key length error\r\n");
            return false;
        }
        WDRV_CLI_SecWEPSet((uint8_t *)argv[3], strlen((char *)argv[3]));
    } else if ((4 <= argc) && (strcmp((char *)argv[2], "wep104") == 0)) {
        if (strlen((char *)argv[3]) != 26) {
            SYS_CONSOLE_MESSAGE("WEP104 key length error\r\n");
            return false;
        }
        WDRV_CLI_SecWEPSet((uint8_t *)argv[3], strlen((char *)argv[3]));
    } else if ((4 <= argc) && (strcmp((char *)argv[2], "wpa") == 0)) {
        if ((strlen((char *)argv[3]) < 8) || (strlen((char *)argv[3]) > 63)) {
            SYS_CONSOLE_MESSAGE("WPA pass phrase length error\r\n");
            return false;
        }
        WDRV_CLI_SecWPASet((uint8_t *)argv[3], strlen((char *)argv[3]));
    } else if ((4 <= argc) && (strcmp((char *)argv[2], "wpa2") == 0)) {
        if ((strlen((char *)argv[3]) < 8) || (strlen((char *)argv[3]) > 63)) {
            SYS_CONSOLE_MESSAGE("WPA2 pass phrase length error\r\n");
            return false;
        }
        WDRV_CLI_SecWPA2Set((uint8_t *)argv[3], strlen((char *)argv[3]));
    } else if ((4 <= argc) && (strcmp((char *)argv[2], "pin") == 0)) {
        if ((strlen((char *)argv[3]) != 8)) {
            SYS_CONSOLE_MESSAGE("WPS PIN length error\r\n");
            return false;
        }
        WDRV_CLI_SecWPSPINSet((uint8_t *)argv[3], strlen((char *)argv[3]));
    } else if ((3 <= argc) && (strcmp((char *)argv[2], "pbc") == 0)) {
        WDRV_CLI_SecWPSPushButtonSet();
    } else {
        SYS_CONSOLE_MESSAGE("Unknown security mode or wrong parameters were typed.\r\n");
        SYS_CONSOLE_MESSAGE("Usage: \r\n");
        SYS_CONSOLE_MESSAGE("1. open : iwconfig security open.\r\n");
        SYS_CONSOLE_MESSAGE("2. wep40 : iwconfig security wep40 <key>.\r\n");
        SYS_CONSOLE_MESSAGE("3. wep104 : iwconfig security wep104 <key>.\r\n");
        SYS_CONSOLE_MESSAGE("4. wpa : iwconfig security wpa <key>.\r\n");
        SYS_CONSOLE_MESSAGE("5. wpa2 : iwconfig security wpa2 <key>.\r\n");
        SYS_CONSOLE_MESSAGE("6. wps pin : iwconfig security pin <pin>.\r\n");
        SYS_CONSOLE_MESSAGE("7. wps pbc : iwconfig security pbc.\r\n");
        SYS_CONSOLE_MESSAGE("Try \"iwconfig --help\" or \"iwconfig -h\" for more information\r\n");
        return false;
    }

    return true;
}

static bool IwconfigSetPower(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc < 3) {
        SYS_CONSOLE_MESSAGE("Missing value for last parameter\r\n");
        return false;
    }

    if ( (3 <= argc) && (strcmp((char*)argv[2], "enable") == 0) ) {    // reenable power saving
        WDRV_CLI_PowerSaveEnable(true);
    } else if ( (3 <= argc) && (strcmp((char*)argv[2], "disable") == 0) ) {    // disable power saving
        WDRV_CLI_PowerSaveEnable(false);
    } else {
        SYS_CONSOLE_MESSAGE("Unknown parameter\r\n");
        return false;
    }

    return true;
}

static int Cmd_EraseConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    WDRV_CONFIG_DataErase();
    SYS_CONSOLE_MESSAGE("--Done\r\n");
    return 0;
}

static int Cmd_SaveConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    WDRV_CONFIG_DataSave();
    SYS_CONSOLE_MESSAGE("--Done\r\n");
    return 0;
}

static int Cmd_ReadConfig(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    WDRV_CONFIG_DataPrint();
    return 0;
}

static void DisplayScanResult(SYS_CMD_DEVICE_NODE* pCmdIO, uint16_t index, WDRV_SCAN_RESULT *p_scanResult)
{
    int i;

    SYS_CONSOLE_MESSAGE("\r\n======================\r\n");
    SYS_CONSOLE_PRINT("Scan Result  %d\r\n", index + 1);

    // SSID
    if(p_scanResult->ssidLen < 32) {
        p_scanResult->ssid[p_scanResult->ssidLen] = '\0'; // ensure string terminator
        SYS_CONSOLE_PRINT("SSID:     %s\r\n", p_scanResult->ssid);

    } else {
        SYS_CONSOLE_MESSAGE("SSID:     ");
        for(i = 0; i < 32; i++) {
            SYS_CONSOLE_PRINT("%c", p_scanResult->ssid[i]);
        }
        SYS_CONSOLE_MESSAGE("\r\n");
    }

    // BSSID
    SYS_CONSOLE_PRINT("BSSID:    %02x %02x %02x %02x %02x %02x\r\n",
                          p_scanResult->bssid[0], p_scanResult->bssid[1],
                          p_scanResult->bssid[2], p_scanResult->bssid[3],
                          p_scanResult->bssid[4], p_scanResult->bssid[5]);

    // BSS Type
    SYS_CONSOLE_MESSAGE("BSS Type: ");
    if (p_scanResult->bssType == WDRV_NETWORK_TYPE_INFRASTRUCTURE) {
        SYS_CONSOLE_MESSAGE("Infrastructure\r\n");
    } else {
        SYS_CONSOLE_MESSAGE("AdHoc\r\n");
    }

    // Channel
    SYS_CONSOLE_PRINT("Channel:  %d\r\n", p_scanResult->channel);

    // Beacon Period
    SYS_CONSOLE_PRINT("Beacon:   %d ms\r\n", p_scanResult->beaconPeriod);

    // Security
    SYS_CONSOLE_MESSAGE("Security: ");
    if ((p_scanResult->apConfig & 0x10) > 0) {        // if privacy bit set
        if ((p_scanResult->apConfig & 0x80) > 0) {    // if WPA2 bit is set
            SYS_CONSOLE_MESSAGE("WPA2\r\n");
        } else if ((p_scanResult->apConfig & 0x40) > 0) {    // if WPA bit is set
            SYS_CONSOLE_MESSAGE("WPA\r\n");
        } else {
            SYS_CONSOLE_MESSAGE("WEP\r\n");
        }
    } else {
        SYS_CONSOLE_MESSAGE("Open\r\n");
    }

    // Preamble
    SYS_CONSOLE_MESSAGE("Preamble: ");
    if ((p_scanResult->apConfig & 0x20) > 0) {
        SYS_CONSOLE_MESSAGE("Long\r\n");
    } else {
         SYS_CONSOLE_MESSAGE("Short\r\n");
    }

    // RSSI
    SYS_CONSOLE_PRINT("RSSI:     %d\r\n", p_scanResult->rssi);
}

#endif /* TCPIP_STACK_COMMANDS_WIFI_ENABLE */

//DOM-IGNORE-END
