/*******************************************************************************
  MRF24W Config Data

  File Name: 
    wdrv_mrf24wn_config_data.c  
  
  Summary:
    Module for the Microchip TCP/IP stack using WiFi MAC

  Description:
    - Stores and retrieves WiFi configuration information to non-volatile memory
      (NVM)
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

#include <string.h>
#include <sys/attribs.h>

#include "driver/nvm/drv_nvm.h"

#include "wdrv_mrf24wn_common.h"
#include "wdrv_mrf24wn_main.h"

#if defined(TCPIP_IF_MRF24WN)

static WDRV_CONFIG_DATA s_configData;
static WDRV_SCAN_CONTEXT s_scanContext;
WDRV_CONFIG_DATA *p_wdrv_configData = &s_configData;
WDRV_SCAN_CONTEXT *p_wdrv_scanContext = &s_scanContext;
WDRV_CONFIG_DATA s_configDataBackup = {0,};

static void WriteConfigToMemory(void);
static void ReadConfigFromMemory(void);
static void EraseConfigFromMemory(void);

static void WriteConfigToMemory(void)
{
    int i;
    uint8_t *p_data1, *p_data2;

    p_data1 = (void*)p_wdrv_configData;
    p_data2 = (void*)&s_configDataBackup; 

    for (i = 0; i < sizeof(WDRV_CONFIG_DATA); i++)
        p_data2[i] = p_data1[i];
}

static void ReadConfigFromMemory(void)
{
    int i;
    uint8_t *p_data1, *p_data2;

    p_data1 = (void *)&s_configDataBackup;
    p_data2 = (void *)p_wdrv_configData; 

    for (i = 0; i < sizeof(WDRV_CONFIG_DATA); i++)
        p_data2[i] = p_data1[i];
}

static void EraseConfigFromMemory(void)
{
    memset(&s_configDataBackup, 0, sizeof(s_configDataBackup));
}

void WDRV_CONFIG_DataPrint(void)
{
    int i;

    if (s_configDataBackup.networkType == 0) {
        SYS_CONSOLE_MESSAGE("No saved configuration data.\r\n");
        return;
    }

    SYS_CONSOLE_PRINT("Size of ConfigData:  %lu\r\n", sizeof(s_configDataBackup));

    SYS_CONSOLE_MESSAGE("NetworkType:         ");
    switch (s_configDataBackup.networkType) {
    case WDRV_NETWORK_TYPE_INFRASTRUCTURE:
        SYS_CONSOLE_MESSAGE("Infrastructure\r\n");
        break;
    case WDRV_NETWORK_TYPE_ADHOC:
        SYS_CONSOLE_MESSAGE("Ad-hoc\r\n");
        break;
    case WDRV_NETWORK_TYPE_P2P:
        SYS_CONSOLE_MESSAGE("P2P\r\n");
        break;
    case WDRV_NETWORK_TYPE_SOFT_AP:
        SYS_CONSOLE_MESSAGE("Soft AP\r\n");
        break;
    default:
        SYS_CONSOLE_MESSAGE("Invalid NetworkType\r\n");
        break;
    }

    SYS_CONSOLE_PRINT("NetSSID:             %s\r\n",  s_configDataBackup.netSSID);
    SYS_CONSOLE_PRINT("SSIDLength:          %d\r\n",  s_configDataBackup.SsidLength);
    SYS_CONSOLE_MESSAGE("SecurityMode:        ");
    
    switch (s_configDataBackup.SecurityMode) {
    case WDRV_SECURITY_OPEN:
        SYS_CONSOLE_MESSAGE("Open\r\n");
        break;
    case WDRV_SECURITY_WEP_40:
        SYS_CONSOLE_MESSAGE("WEP40\r\n");
        break;
    case WDRV_SECURITY_WEP_104:
        SYS_CONSOLE_MESSAGE("WEP104\r\n");
        break;
    case WDRV_SECURITY_WPA_WITH_PASS_PHRASE:
        SYS_CONSOLE_MESSAGE("WPA-PSK\r\n");
        break;
    case WDRV_SECURITY_WPA2_WITH_PASS_PHRASE:
        SYS_CONSOLE_MESSAGE("WPA2-PSK\r\n");
        break;
    case WDRV_SECURITY_WPS_PUSH_BUTTON:
        SYS_CONSOLE_MESSAGE("WPS PBC\r\n");
        break;
    case WDRV_SECURITY_WPS_PIN:
        SYS_CONSOLE_MESSAGE("WPS PIN\r\n");
        break;
    default:
        SYS_CONSOLE_MESSAGE("Invalid SecurityMode\r\n");
        break;
    }

    SYS_CONSOLE_MESSAGE("SecurityKey:         ");
    for (i = 0; i < s_configDataBackup.SecurityKeyLength ; i++) {
        SYS_CONSOLE_PRINT("%.2x", s_configDataBackup.SecurityKey[i]);
    }
    SYS_CONSOLE_MESSAGE("\r\n");
    SYS_CONSOLE_PRINT("SecurityKeyLength:   %d\r\n", s_configDataBackup.SecurityKeyLength);
}

void WDRV_CONFIG_DataErase(void)
{
    EraseConfigFromMemory();
}

void WDRV_CONFIG_DataSave(void)
{
    WriteConfigToMemory();
}

bool WDRV_CONFIG_DataLoad(void)
{
    bool bVerifySuccess = false;
    
    ReadConfigFromMemory();

    // verify
    if ((p_wdrv_configData->networkType == 0) || 
        (p_wdrv_configData->networkType == 0xff) ||
        (p_wdrv_configData->SsidLength ==0) ||
        (strlen((const char *)p_wdrv_configData->netSSID) != p_wdrv_configData->SsidLength)) 
    {
        bVerifySuccess = false;
    }
    else 
    {
        bVerifySuccess = true;
    }

    if (bVerifySuccess == false) { // use default value
        // load the default SSID Name
        if (sizeof(WDRV_DEFAULT_SSID_NAME) > sizeof(p_wdrv_configData->netSSID))
            return false;

        memcpy(p_wdrv_configData->netSSID, (const void*)WDRV_DEFAULT_SSID_NAME, sizeof(WDRV_DEFAULT_SSID_NAME));
        p_wdrv_configData->SsidLength = sizeof(WDRV_DEFAULT_SSID_NAME) - 1;
        p_wdrv_configData->networkType = WDRV_DEFAULT_NETWORK_TYPE;
        p_wdrv_configData->Flags.bWFEasyConfig = 0;
        p_wdrv_configData->SecurityMode = WDRV_DEFAULT_WIFI_SECURITY_MODE;

        switch (g_wdrv_config.securityMode) {
        case WDRV_SECURITY_OPEN:
            memset(p_wdrv_configData->SecurityKey, 0x00, sizeof(p_wdrv_configData->SecurityKey));
            p_wdrv_configData->SecurityKeyLength = 0;
            break;
        case WDRV_SECURITY_WEP_40:
            memcpy(p_wdrv_configData->SecurityKey, (const void*)WDRV_DEFAULT_WEP_KEYS_40, sizeof(WDRV_DEFAULT_WEP_KEYS_40) - 1);
            p_wdrv_configData->SecurityKeyLength = sizeof(WDRV_DEFAULT_WEP_KEYS_40) - 1;
            break;
        case WDRV_SECURITY_WEP_104:
            memcpy(p_wdrv_configData->SecurityKey, (const void*)WDRV_DEFAULT_WEP_KEYS_104, sizeof(WDRV_DEFAULT_WEP_KEYS_104) - 1);
            p_wdrv_configData->SecurityKeyLength = sizeof(WDRV_DEFAULT_WEP_KEYS_104) - 1;
            break;
        case WDRV_SECURITY_WPA_WITH_PASS_PHRASE:
        case WDRV_SECURITY_WPA2_WITH_PASS_PHRASE:
            memcpy(p_wdrv_configData->SecurityKey, (const void*)WDRV_DEFAULT_PSK_PHRASE, sizeof(WDRV_DEFAULT_PSK_PHRASE) - 1);
            p_wdrv_configData->SecurityKeyLength = sizeof(WDRV_DEFAULT_PSK_PHRASE) - 1;
            break;
        case WDRV_SECURITY_WPS_PUSH_BUTTON:
            memset(p_wdrv_configData->SecurityKey, 0x00, sizeof(p_wdrv_configData->SecurityKey));
            p_wdrv_configData->SecurityKeyLength = 0;
            break;
        case WDRV_SECURITY_WPS_PIN:
            memcpy(p_wdrv_configData->SecurityKey, (const void*)WDRV_DEFAULT_WPS_PIN, sizeof(WDRV_DEFAULT_WPS_PIN));
            p_wdrv_configData->SecurityKeyLength = sizeof(WDRV_DEFAULT_WPS_PIN);
            break;
        default:
            WDRV_ASSERT(false, "Unsupported security mode.\r\n");
            break;
        }

        if(g_wdrv_config.staticIPEnabled)
            p_wdrv_configData->Flags.bIsDHCPEnabled = false;
        else
            p_wdrv_configData->Flags.bIsDHCPEnabled = true;
    }

    return true;
}

#endif // TCPIP_IF_MRF24WN

//DOM-IGNORE-END
