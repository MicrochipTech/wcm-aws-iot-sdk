/*******************************************************************************
  Application to Demo HTTP Server

  Summary:
    Support for HTTP module in Microchip TCP/IP Stack

  Description:
    -Implements the application
    -Reference: RFC 1002
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

#define __CUSTOMHTTPAPP_C

#include "system_config.h"

#if defined(TCPIP_STACK_USE_HTTP_SERVER)

#include "tcpip/tcpip.h"
#include "system/tmr/sys_tmr.h"
#include "tcpip/src/common/hashes.h"
#include "tcpip/src/common/helpers.h"

#include "tcpip/src/tcpip_private.h"
#include "driver/wifi/mrf24w/src/drv_wifi_easy_config.h"
#include "app.h"

extern APP_DATA appData;

/****************************************************************************
  Section:
    Function Prototypes and Memory Globalizers
  ***************************************************************************/
#if defined(TCPIP_HTTP_USE_POST)
    static HTTP_IO_RESULT HTTPPostWifiConfig(HTTP_CONN_HANDLE connHandle);
#endif

static DRV_WIFI_SCAN_RESULT bssDesc;
static bool bssDescIsValid = false;

// Stick status message variable.  See lastSuccess for details.
static bool lastFailure = false;



extern DRV_WIFI_SCAN_RESULT preScanResult[50]; // WF_PRESCAN

/*****************************************************************************
 * FUNCTION: HexToBin
 *
 * RETURNS:  binary value associated with ASCII hex input value
 *
 * PARAMS:   hexChar -- ASCII hex character
 *
 * NOTES:    Converts an input ascii hex character to its binary value.  Function
 *           does not error check; it assumes only hex characters are passed in.
 *****************************************************************************/
uint8_t HexToBin(uint8_t hexChar)
{
    if ((hexChar >= 'a') && (hexChar <= 'f'))
    {
        return (0x0a + (hexChar - 'a'));
    }
    else if ((hexChar >= 'A') && (hexChar <= 'F'))
    {
        return (0x0a + (hexChar - 'A'));
    }
    else //  ((hexChar >= '0') && (hexChar <= '9'))
    {
        return (0x00 + (hexChar - '0'));
    }

}

/*****************************************************************************
 * FUNCTION: ConvertASCIIHexToBinary
 *
 * RETURNS:  true if conversion successful, else false
 *
 * PARAMS:   p_ascii   -- ascii string to be converted
 *           p_binary  -- binary value if conversion successful
 *
 * NOTES:    Converts an input ascii hex string to binary value (up to 32-bit value)
 *****************************************************************************/
static bool ConvertASCIIHexToBinary(char *p_ascii, uint16_t *p_binary)
{
    int8_t  i;
    uint32_t multiplier = 1;

    *p_binary = 0;

    // not allowed to have a string of more than 4 nibbles
    if (strlen((char*)p_ascii) > 8u)
    {
        return false;
    }

    // first, ensure all characters are a hex digit
    for (i = (uint8_t)strlen((char *)p_ascii) - 1; i >= 0 ; --i)
    {
        if (!isxdigit(p_ascii[i]))
        {
            return false;
        }
        *p_binary += multiplier * HexToBin(p_ascii[i]);
        multiplier *= 16;
    }

    return true;
}

bool convertAsciiToHexInPlace(char *p_string, uint8_t expectedHexBinSize )
{
    char  ascii_buffer[3];
    uint8_t  hex_binary_index = 0;
    char  *hex_string_start = p_string;
    uint16_t hex_buffer = 0;

    /* gobble up any hex prefix */
    if ( memcmp(hex_string_start, (const char*) "0x", 2) == 0 )
    {
        hex_string_start+=2;
    }

    if ( strlen( (char *) hex_string_start) != (expectedHexBinSize*2) )
    {
        return false;
    }

    while ( hex_binary_index < expectedHexBinSize )
    {

      memcpy ( ascii_buffer, (const char*) hex_string_start, 2 );
      ascii_buffer[2] = '\0';

      /* convert the hex string to a machine hex value */
      if ( !ConvertASCIIHexToBinary( ascii_buffer,&hex_buffer) )
      {
        return false;
      }

      p_string[hex_binary_index++] = (uint8_t) hex_buffer;
      hex_string_start +=2;
    }

    return true;
}

/****************************************************************************
  Section:
    GET Form Handlers
  ***************************************************************************/
  typedef union
{
    uint16_t Val;
    uint8_t v[2];
} MCHP_UINT16_VAL;
/****************************************************************************
  Section:
    GET Form Handlers
  ***************************************************************************/

/*****************************************************************************
  Function:
    HTTP_IO_RESULT TCPIP_HTTP_GetExecute(HTTP_CONN_HANDLE connHandle)

  Internal:
    See documentation in the TCP/IP Stack API or HTTP.h for details.
  ***************************************************************************/
HTTP_IO_RESULT TCPIP_HTTP_GetExecute(HTTP_CONN_HANDLE connHandle)
{
    const uint8_t *ptr, *ptr1;
    uint8_t filename[20];
    uint8_t* httpDataBuff;
    uint8_t bssIdx;
    MCHP_UINT16_VAL bssIdxStr;

    // Load the file name
    // Make sure uint8_t filename[] above is large enough for your longest name
    SYS_FS_FileNameGet(TCPIP_HTTP_CurrentConnectionFileGet(connHandle), filename, 20);

    httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);

    if(!memcmp(filename, "scan.cgi", 8))
    {
        ptr = TCPIP_HTTP_ArgGet(httpDataBuff, (const uint8_t *)"scan");
        ptr1 = TCPIP_HTTP_ArgGet(httpDataBuff, (const uint8_t *)"getBss");

        if ((ptr != NULL) && (ptr1 == NULL))
        {
            bssDescIsValid = false;
            // scan request
            #if (WF_DEFAULT_NETWORK_TYPE == DRV_WIFI_NETWORK_TYPE_SOFT_AP) || (DRV_WIFI_DEFAULT_ADHOC_PRESCAN == DRV_WIFI_ENABLED)
            // display pre-scan results before starting as SoftAP. SoftAP does not scan.
//            SYS_CONSOLE_MESSAGE("HTTPExecuteGet: ... Scan bypass scan  ................. \r\n");
            #else
            if (WFStartScan() == DRV_WIFI_SUCCESS)
            {
                    SYS_CONSOLE_MESSAGE("HTTPExecuteGet: WFStartScan success ................. \r\n");
                    SCAN_SET_DISPLAY(SCANCXT.scanState);
                    SCANCXT.displayIdx = 0;
                
            }
            #endif
        }
        else if ((ptr == NULL) && (ptr1 != NULL))
        {
            // getBss request
            // use the value to get the nth bss stored on chip
            bssDescIsValid = false;
            bssIdxStr.v[1] = *ptr1;
            bssIdxStr.v[0] = *(ptr1+1);
            bssIdx = hexatob(bssIdxStr.Val);

            #if (WF_DEFAULT_NETWORK_TYPE == DRV_WIFI_NETWORK_TYPE_SOFT_AP) || (DRV_WIFI_DEFAULT_ADHOC_PRESCAN == DRV_WIFI_ENABLED)
            // display pre-scan results before starting as SoftAP. SoftAP does not scan.
            bssDesc = preScanResult[bssIdx];
            
            //putsUART("HTTPExecuteGet: SoftAP ... display pre-scan  ................. \r\n");
            #else
            WFRetrieveScanResult(bssIdx, &bssDesc);
            #endif

            if(bssDesc.ssidLen<32)
                bssDesc.ssid[bssDesc.ssidLen]=0;

            bssDescIsValid = true;
        }
        else
        {
            // impossible to get here
        }
    }
    return HTTP_IO_DONE;
}


/****************************************************************************
  Section:
    POST Form Handlers
  ***************************************************************************/
#if defined(TCPIP_HTTP_USE_POST)

/*****************************************************************************
  Function:
    HTTP_IO_RESULT TCPIP_HTTP_PostExecute(HTTP_CONN_HANDLE connHandle)

  Internal:
    See documentation in the TCP/IP Stack API or HTTP.h for details.
  ***************************************************************************/
HTTP_IO_RESULT TCPIP_HTTP_PostExecute(HTTP_CONN_HANDLE connHandle)
{
    // Resolve which function to use and pass along
    uint8_t filename[20];

    // Load the file name
    // Make sure uint8_t filename[] above is large enough for your longest name
    SYS_FS_FileNameGet(TCPIP_HTTP_CurrentConnectionFileGet(connHandle), filename, sizeof(filename));

    // Post resources
    if(!memcmp(filename, "index.htm", 9))
        return HTTPPostWifiConfig(connHandle) /*HTTPPostConfig()*/;

    return HTTP_IO_DONE;
}

/*****************************************************************************
  Function:
    static HTTP_IO_RESULT HTTPPostWifiConfig(HTTP_CONN_HANDLE connHandle)

  Summary:
    Processes the wifi config data

  Description:
    Accepts wireless configuration data from the www site and saves them to a
    structure to be applied by the ZG configuration manager.

    The following configurations are possible:
         i) Mode: adhoc or infrastructure
        ii) Security:
               - None
               - WPA/WPA2 passphrase
               - WPA/WPA2 pre-calculated key
               - WEP 64-bit
               - WEP 128-bit
       iii) Key material

    If an error occurs, such as data is invalid they will be redirected to a page
    informing the user of such results.

    NOTE: This code for modified originally from HTTPPostWifiConfig as distributed
          by Microchip.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    HTTP_IO_DONE - all parameters have been processed
    HTTP_IO_NEED_DATA - data needed by this function has not yet arrived
  ***************************************************************************/
static HTTP_IO_RESULT HTTPPostWifiConfig(HTTP_CONN_HANDLE connHandle)
{
        uint8_t ssidLen;
       // TCPIP_NET_HANDLE currNet;//, newNet;
        uint32_t byteCount;
        TCP_SOCKET sktHTTP;
        uint8_t* httpDataBuff = 0;
        byteCount = TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle);
        sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
        if(byteCount > TCPIP_TCP_GetIsReady(sktHTTP) + TCPIP_TCP_FifoRxFreeGet(sktHTTP))
            goto ConfigFailure;

        // current interface we're working on
        //currNet = TCPIP_TCP_SocketNetGet(sktHTTP);
        // ensure our static approach is working OK

        // Ensure that all data is waiting to be parsed.  If not, keep waiting for
        // all of it to arrive.
        if(TCPIP_TCP_GetIsReady(sktHTTP) < byteCount)
            return HTTP_IO_NEED_DATA;

        httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);
        // Read all browser POST data
        while(TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle))
        {
            // Read a form field name
            if(TCPIP_HTTP_PostNameRead(connHandle, httpDataBuff, 6) != HTTP_READ_OK)
                goto ConfigFailure;

            // Read a form field value
            if(TCPIP_HTTP_PostValueRead(connHandle, httpDataBuff + 6, TCPIP_HTTP_MAX_DATA_LEN-6-2) != HTTP_READ_OK)
                goto ConfigFailure;

            // Parse the value that was read
            if(!strcmp((char*)httpDataBuff, (const char*)"sec"))
            {// Read security type
                char   security_type[7];

                if (strlen((char*)(httpDataBuff+6)) > 6) /* Sanity check */
                    goto ConfigFailure;

                memcpy(security_type, (void*)(httpDataBuff+6), strlen((char*)(httpDataBuff+6)));
                security_type[strlen((char*)(httpDataBuff+6))] = 0; /* Terminate string */

                if (!strcmp((char*)security_type, (const char*)"no"))
                {
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_OPEN;
                }
                else if(!strcmp((char*)security_type, (const char*)"wpa"))
                {
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE;
                }
                else if(!strcmp((char*)security_type, (const char*)"calc"))
                {    /* Pre-calculated key */
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY;
                }
                else if(!strcmp((char*)security_type, (const char*)"wep40"))
                {
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_WEP_40;
                }
                else if(!strcmp((char*)security_type, (const char*)"wep104"))
                {
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_WEP_104;
                }
                else
                {    //Security type no good  :-(
                    SYS_CONSOLE_MESSAGE("\r\n1.5 Unknown key type on www\r\n");
                    goto ConfigFailure;
                }
            }
            else if(!strcmp((char*)httpDataBuff, (const char*)"key"))
            {// Read new key material
                uint8_t key_size = 0, ascii_key = 0;
                switch ((uint8_t)EZ_CFGCXT.security) {
                    case DRV_WIFI_SECURITY_OPEN:     //keep compiler happy, nothing to do here!
                        break;
                    case DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:  //wpa passphrase
                        //putrsUART((ROM char*)"\r\nPassphrase type of key! ");
                        //Set flag to save passphrase->key conversion to flash
                        p_wifi_ConfigData->saveSecurityInfo = 1;
                        ascii_key = 1;
                        key_size = strlen((char *)(httpDataBuff+6));
                        //between 8-63 characters, passphrase
                        if ((key_size < 8 ) || (key_size > 63))
                              goto ConfigFailure;
                        break;
                    case DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY: //wpa pre-calculated key!!!
                        key_size = 64;
                        break;
                    case DRV_WIFI_SECURITY_WEP_40:
                        key_size = 10; /* Assume hex size */
                        if (strlen((char *)(httpDataBuff+6)) == 5) {
                            key_size = 5;  /* ASCII key support */
                            ascii_key = 1;
                         }
                        EZ_CFGCXT.defaultWepKey = 0; /* Example uses only key idx 0 (sometimes called 1) */
                        break;
                    case DRV_WIFI_SECURITY_WEP_104:
                        key_size = 26; /* Assume hex size */
                        if (strlen((char *)(httpDataBuff+6)) == 13) {
                            key_size = 13;    /* ASCII key support */
                            ascii_key = 1;
                        }
                        EZ_CFGCXT.defaultWepKey = 0; /* Example uses only key idx 0 (sometimes called 1) */
                        break;
                }
                if (strlen((char *)(httpDataBuff+6)) != key_size) {
                    SYS_CONSOLE_MESSAGE("\r\nIncomplete key received\r\n");
                    goto ConfigFailure;
                }
                memcpy(EZ_CFGCXT.key, (void*)(httpDataBuff+6), key_size);
                EZ_CFGCXT.key[key_size] = 0; /* terminate string */
                EZ_CFGCXT.SecurityKeyLength = key_size;
                if (!ascii_key) {
                    //if ((cfg.security == sec_wep64) || (cfg.security == sec_wep128))
                        key_size /= 2;
                        //CFGCXT.SecurityKeyLength = key_size;   // yes Or No? Need more debug
                    if (!convertAsciiToHexInPlace((char *)&EZ_CFGCXT.key[0], key_size)) {
                        SYS_CONSOLE_MESSAGE("\r\nFailed to convert ASCII to hex!\r\n");
                        goto ConfigFailure;
                    }
                }
            }
            else if(!strcmp((char*)httpDataBuff, "ssid"))
            {// Get new ssid and make sure it is valid
                if(strlen((char*)(httpDataBuff+6)) < 33u)
                {
                    memcpy(EZ_CFGCXT.ssid, (void*)(httpDataBuff+6), strlen((char*)(httpDataBuff+6)));
                    EZ_CFGCXT.ssid[strlen((char*)(httpDataBuff+6))] = 0; /* Terminate string */

                    /* save current profile SSID for displaying later */
                    DRV_WIFI_SsidGet((uint8_t*)&EZ_CFGCXT.prevSSID, &ssidLen);
                    EZ_CFGCXT.prevSSID[ssidLen] = 0;
                }
                else
                {    //Invalid SSID... fail :-(
                    goto ConfigFailure;
                }
            }
            else if(!strcmp((char*)httpDataBuff, (const char*)"wlan"))
            {// Get the wlan mode: adhoc or infrastructure
                char mode[6];
                if (strlen((char*)(httpDataBuff+6)) > 5) /* Sanity check */
                    goto ConfigFailure;

                memcpy(mode, (void*)(httpDataBuff+6), strlen((char*)(httpDataBuff+6)));
                mode[strlen((char*)(httpDataBuff+6))] = 0; /* Terminate string */
                if(!strcmp((char*)mode, (const char*)"infra"))
                {
                    EZ_CFGCXT.type = DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE;
                }
                else if(!strcmp((char*)mode, "adhoc"))
                {
                    DRV_WIFI_ADHOC_NETWORK_CONTEXT adhocContext;
                    EZ_CFGCXT.type = DRV_WIFI_NETWORK_TYPE_ADHOC;

                    // always setup adhoc to attempt to connect first, then start
                    adhocContext.mode = DRV_WIFI_ADHOC_CONNECT_THEN_START;
                    adhocContext.beaconPeriod = DRV_WIFI_DEFAULT_ADHOC_BEACON_PERIOD;
                    adhocContext.hiddenSsid   = DRV_WIFI_DEFAULT_ADHOC_HIDDEN_SSID;
                    DRV_WIFI_AdhocContextSet(&adhocContext);
                }
                else
                {   //Mode type no good  :-(
                    SYS_CONSOLE_MESSAGE((const char*)"\r\nUnknown mode type on www! ");
                    goto ConfigFailure;
                }

                // save old WLAN mode
                DRV_WIFI_NetworkTypeGet(&EZ_CFGCXT.prevWLAN);
            }
            else if(!strcmp((char*)httpDataBuff, (const char*)"rs"))
            {
                memcpy((uint8_t *)appData.aws_iot_host, (void*)(httpDataBuff+6), strlen((char*)(httpDataBuff+6)));
                appData.aws_iot_host[strlen((char*)(httpDataBuff+6))] = 0; /* Terminate string */
            }
        }

        /* Check if WPA hasn't been selected with adhoc, if it has we choke! */
        if ((EZ_CFGCXT.type == DRV_WIFI_NETWORK_TYPE_ADHOC) &&
          ((EZ_CFGCXT.security == DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE) || (EZ_CFGCXT.security == DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY)))
            goto ConfigFailure;

        /*
         * All parsing complete!  If we have got to here all data has been validated and
         * We can handle what is necessary to start the reconfigure process of the WiFi device
         */

        /* Copy wifi cfg data to be committed to NVM */
        //TCPIP_NET_IF* p_config= (TCPIP_NET_IF*)GetNetworkConfig();
        strcpy((char *)p_wifi_ConfigData->netSSID, (char *)EZ_CFGCXT.ssid);
        p_wifi_ConfigData->SsidLength = strlen((char*)(EZ_CFGCXT.ssid));
        /* Going to set security type */
        p_wifi_ConfigData->SecurityMode = EZ_CFGCXT.security;
        /* Going to save the key, if required */
        if (EZ_CFGCXT.security != DRV_WIFI_SECURITY_OPEN) {
            uint8_t  key_size =0;

            switch ((uint8_t)EZ_CFGCXT.security) {
                case DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:  //wpa passphrase
                    key_size = strlen((char*)(EZ_CFGCXT.key)); //ascii so use strlen
                    break;
                case DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY: //wpa pre-calculated key!!!
                    key_size = 32;
                    break;
                case DRV_WIFI_SECURITY_WEP_40:
                    key_size = 5;
                    break;
                case DRV_WIFI_SECURITY_WEP_104:
                    key_size = 13;
                    break;

            }

            memcpy(p_wifi_ConfigData->SecurityKey, EZ_CFGCXT.key, key_size);
            p_wifi_ConfigData->SecurityKey[strlen((char*)(EZ_CFGCXT.key))] = 0;
            p_wifi_ConfigData->SecurityKeyLength = key_size;

        }
        /* Going to save the network type */
        p_wifi_ConfigData->networkType = EZ_CFGCXT.type;
        p_wifi_ConfigData->dataValid = 1; /* Validate wifi configuration */

        strcpy((char*)httpDataBuff, "/reconnect.htm");
        TCPIP_HTTP_CurrentConnectionStatusSet(connHandle, HTTP_REDIRECT);
        WiFiAsyncSetEventPending(ASYNC_EASY_CONFIG_PENDING);
        
        DRV_WIFI_ConfigDataSave();
        
        BSP_LED_LightShowSet(BSP_LED_CONNECTING_TO_AP);
        
        return HTTP_IO_DONE;


    ConfigFailure:
                lastFailure = true;
                if(httpDataBuff)
                {
                    strcpy((char*)httpDataBuff, "/error.htm");
                }
                TCPIP_HTTP_CurrentConnectionStatusSet(connHandle, HTTP_REDIRECT);
        return HTTP_IO_DONE;
}

#endif //(use_post)


/****************************************************************************
  Section:
    Dynamic Variable Callback Functions
  ***************************************************************************/

/*****************************************************************************
  Function:
    void TCPIP_HTTP_Print_varname(void)

  Internal:
    See documentation in the TCP/IP Stack API or HTTP.h for details.
  ***************************************************************************/
void TCPIP_HTTP_Print_remoteServer(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)appData.aws_iot_host);
}

void TCPIP_HTTP_Print_nextSSID(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_TCP_StringPut(sktHTTP, (uint8_t*)EZ_CFGCXT.ssid); //nextSSID
}

#endif

//DOM-IGNORE-BEGIN

