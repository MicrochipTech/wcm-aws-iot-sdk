/*******************************************************************************
  tcpip MRF24WN connection manager implementation

  File Name:  
    wdrv_mrf24wn_connmgr.c  
  
  Summary:
    
  Description:
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

#include "wdrv_mrf24wn_common.h"
#include "wdrv_mrf24wn_main.h"

#if defined(TCPIP_IF_MRF24WN)

static TCPIP_DHCP_HANDLE s_dhcpHandle = NULL;
static bool s_dhcpSuccessful = false;
static WDRV_CONNECTION_STATES s_ConnectionStatus = WDRV_CSTATE_NOT_CONNECTED;
static bool s_logicalConnection = false;
static bool s_inPowerSave = false;
static char *s_connect_failure_reason[] = {
    "",
    "NO_NETWORK_AVAIL",
    "LOST_LINK",
    "DISCONNECT_CMD",
    "BSS_DISCONNECTED",
    "AUTH_FAILED",
    "ASSOC_FAILED",
    "NO_RESOURCES_AVAIL",
    "CONNECTION_DENIED",
    "",
    "INVALID_PROFILE",
    "",
    "PROFILE_MISMATCH",
    "CONNECTION_EVICTED",    
};

static void ConnectionStateSet(bool state);
static void SignalDHCPSuccessful(void);
static void DhcpEventHandler(TCPIP_NET_HANDLE hNet, TCPIP_DHCP_EVENT_TYPE evType, const void* param);
static void ConnectStatusUpdate(bool connected, uint8_t reason);

void DhcpConfigTask(void)
{
    TCPIP_NET_HANDLE netH;
    uint8_t networkType;

    networkType = p_wdrv_configData->networkType;;

    netH = TCPIP_STACK_NetHandleGet("MRF24WN");

    if (networkType == WDRV_NETWORK_TYPE_INFRASTRUCTURE) {
        if (g_wdrv_config.dhcpcUsed) {
            #if defined(TCPIP_STACK_USE_DHCP_CLIENT)
            TCPIP_DHCP_Enable(netH);
            #endif
        }
        if (g_wdrv_config.dhcpsUsed) {
            #if defined(TCPIP_STACK_USE_DHCP_SERVER)
            TCPIP_DHCPS_Disable(netH);
            #endif
        }
    } else {
        WDRV_ASSERT(false, "Unsupported for now.");
    }
}

void ProceedConnectEventCB(int32_t connected, uint8_t devID, uint8_t* bssid, bool bssConn, uint8_t reason)
{
    if (connected == true) {
        ConnectionStateSet(true);
        AsyncEventSet(ASYNC_DHCP_CONFIG_PENDING); 
        if (g_wdrv_config.dhcpcUsed) {
            if(s_dhcpHandle == NULL)
                s_dhcpHandle = TCPIP_DHCP_HandlerRegister(TCPIP_STACK_NetHandleGet("MRF24WN"), DhcpEventHandler, NULL);
        } else {
            if (g_wdrv_config.powerSaveEnabled) {  
                WDRV_EXT_CmdPowerSavePut(true); 
                s_inPowerSave = true;
            }
        }
        WDRV_DBG_INFORM_MESSAGE(("Connection succeeded\r\n"));
    } else if (connected == false){             
        ConnectionStateSet(false);
        if (g_wdrv_config.powerSaveEnabled && s_inPowerSave) {  
            WDRV_EXT_CmdPowerSavePut(false); 
            s_inPowerSave = false;
        }
        WDRV_DBG_INFORM_PRINT(("Connection failed - %s\r\n", s_connect_failure_reason[reason]));
    } 

    ConnectStatusUpdate(connected, reason);
}

bool NetworkConnected()
{
    return s_logicalConnection;   
}      

static void ConnectionStateSet(bool state)
{
    s_logicalConnection = state;
}

static void SignalDHCPSuccessful(void)
{
    s_dhcpSuccessful = true;
}    

// callback function; stack calls this upon DHCP events
static void DhcpEventHandler(TCPIP_NET_HANDLE hNet, TCPIP_DHCP_EVENT_TYPE evType, const void* param)
{
    if (evType == DHCP_EVENT_BOUND) {
        SignalDHCPSuccessful();
        if (g_wdrv_config.powerSaveEnabled) {  
            WDRV_EXT_CmdPowerSavePut(true); 
            s_inPowerSave = true;
        }
    }
}

static void NetModeSet(uint8_t networkType)
{
    switch (networkType) {
    case WDRV_NETWORK_TYPE_INFRASTRUCTURE:
        WDRV_EXT_CmdNetModeBSSSet();
        break;
    case WDRV_NETWORK_TYPE_ADHOC:
        WDRV_ASSERT(false, "Adhoc is not supported for now");
        break;
    case WDRV_NETWORK_TYPE_SOFT_AP:
        WDRV_ASSERT(false, "SoftAP is not supported for now");
        break;
    case WDRV_NETWORK_TYPE_P2P:
        WDRV_ASSERT(false, "P2P is not supported for now");
        break;
    default:
        WDRV_ASSERT(false, "Undefined network type");
        break;
    }
}

static void SecuritySet(uint8_t securityMode)
{
    bool pinMode;

    switch (securityMode) {
    case WDRV_SECURITY_OPEN:
        WDRV_EXT_CmdSecNoneSet();
        break;
    case WDRV_SECURITY_WEP_40:
    case WDRV_SECURITY_WEP_104:
        WDRV_EXT_CmdSecWEPSet(p_wdrv_configData->SecurityKey, p_wdrv_configData->SecurityKeyLength);
        break;
    case WDRV_SECURITY_WPA_WITH_PASS_PHRASE:
        WDRV_EXT_CmdSecWPASet(p_wdrv_configData->SecurityKey, p_wdrv_configData->SecurityKeyLength);
        break;
    case WDRV_SECURITY_WPA2_WITH_PASS_PHRASE:
        WDRV_EXT_CmdSecWPA2Set(p_wdrv_configData->SecurityKey, p_wdrv_configData->SecurityKeyLength);
        break;
    case WDRV_SECURITY_WPS_PIN:
    case WDRV_SECURITY_WPS_PUSH_BUTTON:
         pinMode = p_wdrv_configData->SecurityMode == WDRV_SECURITY_WPS_PIN ? true : false;
         WDRV_EXT_CmdSecWpsSet(pinMode, p_wdrv_configData->SecurityKey, p_wdrv_configData->SecurityKeyLength);
        break;
    default:
        WDRV_ASSERT(false, "Undefined security mode");
        break;
    }
}

TCPIP_MAC_RES ConnectToNetwork(void)
{
    TCPIP_MAC_RES res = TCPIP_MAC_RES_OK;

    WDRV_EXT_CmdSSIDSet(p_wdrv_configData->netSSID, p_wdrv_configData->SsidLength);

    NetModeSet(p_wdrv_configData->networkType);

    if (p_wdrv_configData->networkType == WDRV_NETWORK_TYPE_SOFT_AP)
        WDRV_EXT_CmdChannelSet(WDRV_DEFAULT_CHANNEL); 

    SecuritySet(p_wdrv_configData->SecurityMode);
    WDRV_EXT_CmdPowerSavePut(false); 

    WDRV_DBG_INFORM_MESSAGE(("\r\nStart WiFi Connect . . .\r\n"));

    // start the WiFi connection process
    if ((p_wdrv_configData->SecurityMode != WDRV_SECURITY_WPS_PIN) &&
        (p_wdrv_configData->SecurityMode != WDRV_SECURITY_WPS_PUSH_BUTTON)) {
        if (WDRV_EXT_CmdConnect() == 0)
            s_ConnectionStatus = WDRV_CSTATE_CONNECTION_IN_PROGRESS;
    }

    return res;
}

void WPSDoneCB(void)
{
    uint32_t status;

    WDRV_EXT_WPSResultsRead(p_wdrv_configData, &status);

    if (status == WDRV_SUCCESS) {
        WDRV_DBG_INFORM_MESSAGE(("WPS process finished successfully.\r\n"));
        ConnectToNetwork();
    } else {
        WDRV_DBG_INFORM_PRINT(("WPS process did not finish successfully. (%d)\r\n", status));
        /* TODO: handle error case */
    }
}

static void LinkDownTimeoutCallback(uintptr_t context, uint32_t currTick)
{
    if (s_ConnectionStatus == WDRV_CSTATE_CONNECTION_TEMPORARY_LOST) {
        s_ConnectionStatus = WDRV_CSTATE_CONNECTION_PERMANENTLY_LOST;
        WDRV_EXT_CmdDisconnect();
        WDRV_DBG_INFORM_MESSAGE(("Lost connection permanently !\r\n")); 
    }
}

static void ConnectStatusUpdate(bool connected, uint8_t reason)
{
    static SYS_TMR_HANDLE timer = 0;
    uint16_t timeout;
    
    if (connected == true) {
        s_ConnectionStatus = WDRV_CSTATE_CONNECTED_INFRASTRUCTURE;
    } else {
        switch (reason) {
        case WDRV_DISCONNECT_REASON_NO_NETWORK_AVAIL:      // = 0x01
        case WDRV_DISCONNECT_REASON_DISCONNECT_CMD:        // = 0x03,
        case WDRV_DISCONNECT_REASON_BSS_DISCONNECTED:      // = 0x04,
        case WDRV_DISCONNECT_REASON_AUTH_FAILED:           // = 0x05,
        case WDRV_DISCONNECT_REASON_ASSOC_FAILED:          // = 0x06,
        case WDRV_DISCONNECT_REASON_NO_RESOURCES_AVAIL:    // = 0x07,
        case WDRV_DISCONNECT_REASON_CONNECTION_DENIED:     // = 0x08,
        case WDRV_DISCONNECT_REASON_INVALID_PROFILE:       // = 0x0A,
        case WDRV_DISCONNECT_REASON_PROFILE_MISMATCH:      // = 0x0C   
        case WDRV_DISCONNECT_REASON_CONNECTION_EVICTED:    // = 0x0d,
            s_ConnectionStatus = WDRV_CSTATE_NOT_CONNECTED;
            break;
        case WDRV_DISCONNECT_REASON_LOST_LINK:             // = 0x02
            s_ConnectionStatus = WDRV_CSTATE_CONNECTION_TEMPORARY_LOST;
                
            if (timer != 0)
                SYS_TMR_CallbackStop(timer);
                
            timeout = SYS_TMR_TickCounterFrequencyGet() * 30;   // waiting 30 seconds
            timer = SYS_TMR_CallbackSingle(timeout, 0, LinkDownTimeoutCallback);
            break;
        default:
            WDRV_ASSERT(false, "Undefined reason code\r\n");
            break;
        }
    }
}

WDRV_CONNECTION_STATES WDRV_ConnectStatus_Get(void)
{
    return s_ConnectionStatus;
}

#endif // defined(TCPIP_IF_MRF24WN)

//DOM-IGNORE-END
