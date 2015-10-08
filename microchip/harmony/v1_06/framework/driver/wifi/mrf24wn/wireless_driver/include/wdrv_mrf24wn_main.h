/*******************************************************************************
  WiFi MAC interface functions

  File Name:
    wdrv_mrf24wn_main.h

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

#ifndef _WDRV_MRF24WN_MAIN_H
#define _WDRV_MRF24WN_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include "wdrv_mrf24wn_priv.h"
#if defined(TCPIP_IF_MRF24WN)

// Async Events and macros
#define ASYNC_EASY_CONFIG_PENDING       0x04
#define ASYNC_DHCP_CONFIG_PENDING       0x08
#define ASYNC_POWER_SAVE_PENDING        0x10

/**************************************************************************
  Summary:
    Default values for Wi-Fi AdHoc settings
	
  Description:
    Wi-Fi AdHoc default settings
    
    These defines identify various default Wi-Fi AdHoc settings that can be
    used in the WDRV_ADHOC_NETWORK_CONTEXT structure.                  
*/
#define WDRV_DEFAULT_ADHOC_HIDDEN_SSID            false
#define WDRV_DEFAULT_ADHOC_BEACON_PERIOD          100   // ms
#define WDRV_DEFAULT_ADHOC_MODE                   WDRV_ADHOC_CONNECT_THEN_START

#define WDRV_SUCCESS 0
#define WDRV_ERROR  (-1)
// *****************************************************************************
/*  Wi-Fi PS-Poll Listen Interval default settings

  Summary:
    Default values for Wi-Fi PS PS-Poll Listen Interval settings

  Description
    These defines identify various default Wi-Fi PS-Poll settings that can
    be used in the WDRV_PS_POLL_CONTEXT structure.
*/
#define WDRV_DEFAULT_PS_LISTEN_INTERVAL           ((uint16_t)1)       // 100ms multiplier, e.g. 1 * 100ms = 100ms
#define WDRV_DEFAULT_PS_DTIM_INTERVAL             ((uint16_t)2)       // number of beacon periods
#define WDRV_DEFAULT_PS_DTIM_ENABLED              true                // DTIM wake-up enabled (normally the case)

/************************************************************************
  Summary:
    Selection of different AdHoc connection modes
	
  Description:
    AdHoc Modes
    
    This enumeration identifies the AdHoc modes that can be selected when
    connecting in AdHoc mode.                                            
*/
typedef enum adhocMode
{
    WDRV_ADHOC_CONNECT_THEN_START = 0,  // try to connect existing AdHoc network, if not found then start network
    WDRV_ADHOC_CONNECT_ONLY       = 1,  // only connect to existing AdHoc network
    WDRV_ADHOC_START_ONLY         = 2   // only start a new AdHoc network
} WDRV_ADHOC_MODES;

/*************************************************************************
  Summary:
    Selection of WPS Authorization types
	
  Description:
    Wi-Fi WPS authorization types

    This enumeration identifies the WPS authorization types
*/
typedef enum
{
    WDRV_WPS_AUTH_OPEN       = 0x01,
    WDRV_WPS_AUTH_WPA_PSK    = 0x02,
    WDRV_WPS_AUTH_SHARED     = 0x04,
    WDRV_WPS_AUTH_WPA        = 0x08,
    WDRV_WPS_AUTH_WPA2       = 0x10,
    WDRV_WPS_AUTH_WPA2_PSK   = 0x20
} WDRV_WPS_AUTH_TYPES;

/************************************************************
ding types
	
  Description:
    Wi-Fi WPS encoding types

    This enumeration identifies the WPS encoding types
*/
typedef enum
{
    WDRV_WPS_ENC_NONE        = 0x01,
    WDRV_WPS_ENC_WEP         = 0x02,
    WDRV_WPS_ENC_TKIP        = 0x04,
    WDRV_ENC_AES             = 0x08
} WDRV_WPS_ENCODE_TYPES;

// *****************************************************************************
/*  Deauthorization/Disassociate Reason Codes

  Summary:
    Selection of different codes when a deauthorization or disassociation event has occurred.

  Description
    This enumeration identifies the reason codes for a connection lost due to a
    deauthorization or disassociation from the AP.
*/
typedef enum
{
    WDRV_UNSPECIFIED                          = 1,
    WDRV_REASON_PREV_AUTH_NOT_VALID           = 2,
    WDRV_DEAUTH_LEAVING                       = 3,
    WDRV_DISASSOC_DUE_TO_INACTIVITY           = 4,
    WDRV_DISASSOC_AP_BUSY                     = 5,
    WDRV_CLASS2_FRAME_FROM_NONAUTH_STA        = 6,
    WDRV_CLASS3_FRAME_FROM_NONASSOC_STA       = 7,
    WDRV_DISASSOC_STA_HAS_LEFT                = 8,
    WDRV_STA_REQ_ASSOC_WITHOUT_AUTH           = 9,
    WDRV_INVALID_IE                           = 13,
    WDRV_MIC_FAILURE                          = 14,
    WDRV_4WAY_HANDSHAKE_TIMEOUT               = 15,
    WDRV_GROUP_KEY_HANDSHAKE_TIMEOUT          = 16,
    WDRV_IE_DIFFERENT                         = 17,
    WDRV_INVALID_GROUP_CIPHER                 = 18,
    WDRV_INVALID_PAIRWISE_CIPHER              = 19,
    WDRV_INVALID_AKMP                         = 20,
    WDRV_UNSUPP_RSN_VERSION                   = 21,
    WDRV_INVALID_RSN_IE_CAP                   = 22,
    WDRV_IEEE8021X_FAILED                     = 23,
    WDRV_CIPHER_SUITE_REJECTED                = 24
} WDRV_REASON_CODES;

/***********************************************************
  Summary:
    Wi-Fi Power-Saving states
	
  Description:
    Wi-Fi Power-Saving states
    
    This enumeration identifies Wi-Fi Power-Saving states. See
    WDRV_PsPollEnable().                                                
*/

typedef enum
{
    /* enable hibernate mode */
    WDRV_PS_HIBERNATE,          
    WDRV_PS_SLEEP,        
    WDRV_PS_ACTIVE                 
} WDRV_POWER_SAVE_STATES;

/***********************************************************
  Summary:
    Wi-Fi Connection states
	
  Description:
    Wi-Fi Connection States
    
    This enumeration identifies Wi-Fi Connection states. See
    WDRV_CLI_ConnectionStateGet().                          
 */
typedef enum
{
    /*No Wi-Fi connection exists*/
    WDRV_CSTATE_NOT_CONNECTED                = 1,

    /*Wi-Fi connection in progress*/
    WDRV_CSTATE_CONNECTION_IN_PROGRESS       = 2,

    /*Wi-Fi connected in infrastructure mode*/
    WDRV_CSTATE_CONNECTED_INFRASTRUCTURE     = 3,

    /*Wi-Fi connected in adHoc mode*/
    WDRV_CSTATE_CONNECTED_ADHOC              = 4,

    /*Wi-Fi in process of reconnecting*/
    WDRV_CSTATE_RECONNECTION_IN_PROGRESS     = 5,

    /*Wi-Fi connection temporarily lost*/
    WDRV_CSTATE_CONNECTION_TEMPORARY_LOST    = 6,

    /*Wi-Fi connection permanently lost*/
    WDRV_CSTATE_CONNECTION_PERMANENTLY_LOST  = 7
} WDRV_CONNECTION_STATES;

/***********************************************************
  Summary:
    Wi-Fi Soft AP events
	
  Description:
    Wi-Fi Soft AP events

    This enumeration identifies Wi-Fi Soft AP events.
 */
typedef enum
{
    WDRV_SOFTAP_EVENT_CONNECTED    = 0,
    WDRV_SOFTAP_EVENT_DISCONNECTED = 1
} WDRV_SOFT_AP_STATES;

/***********************************************************
  Summary:
    Wi-Fi Soft AP event reason codes
	
  Description:
    Wi-Fi Soft AP event reason codes

    This enumeration identifies Wi-Fi Soft AP events.
 */
typedef enum
{
    WDRV_SOFTAP_EVENT_LINK_LOST       = 0,
    WDRV_SOFTAP_EVENT_RECEIVED_DEAUTH = 1
} WDRV_SOFT_AP_EVENT_REASON_CODES ;
    
/***************************************************************
  Summary:
    Contains data pertaining to MRF24WG connection context
	
  Description:
    Wi-Fi Connection Context
    
    This structure contains MRF24WG connection context data. See
    WDRV_ConnectContextGet().                               
*/
typedef struct 
{
    /* channel number of current connection */
    uint16_t  channel;

    /* bssid of connected AP */
    uint8_t  bssid[6];   
} WDRV_CONNECTION_CONTEXT;

/*********************************************************
  Summary:
    Contains data pertaining to Wi-Fi scan context
	
  Description:
    Wi-Fi Scan Context
    
    This structure contains MRF24WG scan context data. See
    WDRV_ScanContextSet().                            
*/
typedef struct 
{
    /* 802.11 allows for active scanning, where the device sends out a broadcast
     probe request seeking an access point.  Also allowed is passive scanning
     where the device only listens to beacons being broadcast from access points.
     Set to WDRV_ACTIVE_SCAN (default) or WDRV_PASSIVE_SCAN */
    uint8_t scanType; 

    /* The number of times to scan a channel while attempting to find a particular
     access point. Default is 1 */
    uint8_t scanCount;

    /************************************************************************
      The minimum time (in milliseconds) the MRF24WG will wait for a probe
      response after sending a probe request. If no probe responses are
      received in minChannelTime, the MRF24WG will go on to the next channel,
      if any are left to scan, or quit. Default is 200ms.                    
      ************************************************************************/
    uint16_t minChannelTime;

    /************************************************************************
      If a probe response is received within minChannelTime, the MRF24WG will
      continue to collect any additional probe responses up to maxChannelTime
      before going to the next channel in the channelList. Units are in
      milliseconds. Default is 400ms.                                        
      ************************************************************************/
    uint16_t maxChannelTime;

    /* The number of microseconds to delay before transmitting a probe request
     following the channel change during scanning.  Default is 20uS. */
    uint16_t probeDelay;

} WDRV_SCAN_CONTEXT;

/**********************************************************
  Summary:
    Contains data pertaining to Wi-Fi AdHoc context
	
  Description:
    Wi-Fi AdHoc Context
    
    This structure contains MRF24WG AdHoc context data. See
    WDRV_AdhocContextSet().                            
*/
typedef struct 
{
    /* Defines how to start the AdHoc network. See WDRV_ADHOC_MODE. */
    /* Default is WDRV_ADHOC_CONNECT_THEN_START. */
    uint8_t  mode;

    /* When starting an AdHoc network, the SSID can be hidden in the beacons. */
    /* Set true to hide the SSID, else false.  Default is false. */
    bool     hiddenSsid;

    /* Sets the beacon period, in ms.  Default is 100ms */
    uint16_t beaconPeriod; 
} WDRV_ADHOC_NETWORK_CONTEXT;


/***************************************************
  Summary:
    Contains data pertaining to Wi-Fi WEP context
	
  Description:
    Wi-Fi Wep Security Context
    
    This structure contains MRF24WG WEP context. See
    WDRV_SecurityWepSet().                      
*/
typedef struct 
{
    /* WDRV_SECURITY_WEP_40 or WDRV_SECURITY_WEP_104 */
    uint8_t wepSecurityType;

    /* Array containing four WEP binary keys. This will be four, 5-byte keys for
     WEP-40 or four, thirteen-byte keys for WEP-104. */
    uint8_t wepKey[WDRV_MAX_WEP_KEY_LENGTH];

    /* number of bytes pointed to by p_wepKey */
    uint8_t wepKeyLength;
} WDRV_WEP_CONTEXT;


/**************************************************************************
  Summary:
    Contains data pertaining to Wi-Fi WPA Key
	
  Description:
    Wi-Fi WPA Key context
    
    This structure contains MRF24WG WPA key info. This structure is used in
    the WDRV_WPA_CONTEXT and WDRV_WPS_CONTEXT structures.          
*/
typedef struct
{
    /* binary key or passphrase */
    uint8_t key[WDRV_MAX_WPA2_PASS_PHRASE_LENGTH];

    /* number of bytes in binary key (always 32) or passphrase */
    uint8_t keyLength;
    
} WDRV_WPA_KEY_INFO;

/******************************************************************************
  Summary:
    Contains data pertaining to Wi-Fi WPA.
	
  Description:
    Wi-Fi WPA context
    
    This structure contains MRF24WG WPA context. See
    WDRV_SecurityWpaSet().
    
    <table>
    WDRV_SECURITY_WPA_WITH_PASS_PHRASE        WPA passphrase
    WDRV_SECURITY_WPA2_WITH_PASS_PHRASE       WPA2 passphrase
    WPA_AUTO_WITH_PASS_PHRASE                     Auto-select between WPA/WPA2 with passphrase
*/
typedef struct 
{
    /* desired security type (see description) */
    uint8_t wpaSecurityType;

    /* see WDRV_WPA_KEY_INFO */
    WDRV_WPA_KEY_INFO keyInfo;
    
} WDRV_WPA_CONTEXT;

/************************************************************
  Summary:
    Contains data pertaining to Wi-Fi WPS security.
	
  Description:
    Wi-Fi WPS security context
    
    This structure contains MRF24WG WPS security context. See
    WDRV_SecurityWpsSet().                               
*/
typedef struct 
{
    /* WDRV_SECURITY_WPS_PUSH_BUTTON or WDRV_SECURITY_WPS_PIN */
    uint8_t wpsSecurityType;                    

    /* if using WDRV_SECURITY_WPS_PIN then pointer to 8-digit pin */
    uint8_t wpsPin[WDRV_WPS_PIN_LENGTH];

    /* should always be 8 */
    uint8_t wpsPinLength;                       

} WDRV_WPS_CONTEXT;

/********************************************************************
  Summary:
    Contains data pertaining to Wi-Fi security.
	
  Description:
    Wi-Fi security context
    
    Structure/union can be used in functions WDRV_SecurityWepSet,
    WDRV_SecurityWpaSet, and WDRV_SecurityWpsSet             
*/
typedef union
{
    /* set WEP security context */
    WDRV_WEP_CONTEXT wepContext;

    /* set WPA security context */
    WDRV_WPA_CONTEXT wpaContext;

    /* set WPS security context */
    WDRV_WPS_CONTEXT wpsContext;
} WDRV_SECURITY_CONTEXT;

typedef enum
{
    WDRV_DISCONNECT_REASON_NO_NETWORK_AVAIL      = 0x01,
    WDRV_DISCONNECT_REASON_LOST_LINK             = 0x02,
    WDRV_DISCONNECT_REASON_DISCONNECT_CMD        = 0x03,
    WDRV_DISCONNECT_REASON_BSS_DISCONNECTED      = 0x04,
    WDRV_DISCONNECT_REASON_AUTH_FAILED           = 0x05,
    WDRV_DISCONNECT_REASON_ASSOC_FAILED          = 0x06,
    WDRV_DISCONNECT_REASON_NO_RESOURCES_AVAIL    = 0x07,
    WDRV_DISCONNECT_REASON_CONNECTION_DENIED     = 0x08,
    WDRV_DISCONNECT_REASON_INVALID_PROFILE       = 0x0A,
    WDRV_DISCONNECT_REASON_PROFILE_MISMATCH      = 0x0C,
    WDRV_DISCONNECT_REASON_CONNECTION_EVICTED    = 0x0d
} WDRV_DISCONNECTION_REASON;

typedef struct {
    bool powerSaveEnabled;
    bool dhcpcUsed;
    bool dhcpsUsed;
    uint8_t securityMode;
    bool staticIPEnabled;
} WDRV_CONFIG;

extern WDRV_CONFIG_DATA *p_wdrv_configData;
extern WDRV_SCAN_CONTEXT *p_wdrv_scanContext;
extern WDRV_CONFIG g_wdrv_config;

bool NetworkConnected();
void TrafficEventInit(TCPIP_MAC_EventF eventF, const void* eventParam);
void TrafficEventDeInit(void);
void TrafficEventRequest(uint16_t event, uint16_t eventInfo);
bool TrafficEventMask(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents, bool enable);
bool TrafficEventAcknowledge(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents);
TCPIP_MAC_EVENT TrafficEventGet(TCPIP_MAC_HANDLE hMac);
void AsyncEventClearAll(void);
void AsyncEventSet(uint8_t event);
void WDRV_CONFIG_DataErase(void);
void WDRV_CONFIG_DataSave(void);
void WDRV_CONFIG_DataPrint(void);
void ProceedConnectEventCB(int32_t connected, uint8_t devID, uint8_t *bssid, bool bssConn, uint8_t reason);
TCPIP_MAC_RES ConnectToNetwork(void);
bool WDRV_CONFIG_DataLoad(void);
void DhcpConfigTask(void);
WDRV_CONNECTION_STATES WDRV_ConnectStatus_Get(void);
void WPSDoneCB(void);

#endif // defined(TCPIP_IF_MRF24WN)

#ifdef __cplusplus
}

#endif

#endif /* _WDRV_MRF24WN_MAIN_H */

// DOM-IGNORE-END
