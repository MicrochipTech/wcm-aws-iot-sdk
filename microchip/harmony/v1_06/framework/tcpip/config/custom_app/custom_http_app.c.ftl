/*******************************************************************************
  Application to Demo HTTP Server

  Summary:
    Support for HTTP module in Microchip TCP/IP Stack

  Description:
    -Implements the application
    -Reference: RFC 1002
*******************************************************************************/

/*******************************************************************************
File Name: custom_http_app.c
Copyright (C) 2012 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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

<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
#include <ctype.h>
</#if>
#include "system_config.h"

#if defined(TCPIP_STACK_USE_HTTP_SERVER)

#include "tcpip/tcpip.h"
#include "system/tmr/sys_tmr.h"
#include "system/random/sys_random.h"
#include "tcpip/src/common/hashes.h"
#include "tcpip/src/common/helpers.h"

<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
#include "tcpip/src/tcpip_private.h"
</#if>

/****************************************************************************
  Section:
    Definitions
  ***************************************************************************/

// Use the web page in the Demo App (~2.5kb ROM, ~0b RAM)
#define HTTP_APP_USE_RECONFIG

// Use the MD5 Demo web page (~5kb ROM, ~160b RAM)
#define HTTP_APP_USE_MD5

// Use the e-mail demo web page
#if defined (TCPIP_STACK_USE_SMTP_CLIENT)
#define HTTP_APP_USE_EMAIL
#endif

/****************************************************************************
  Section:
    Function Prototypes and Memory Globalizers
  ***************************************************************************/
#if defined(TCPIP_HTTP_USE_POST)
    #if defined(SYS_OUT_ENABLE)
        static HTTP_IO_RESULT HTTPPostLCD(HTTP_CONN_HANDLE connHandle);
    #endif
    #if defined(HTTP_APP_USE_MD5)
        static HTTP_IO_RESULT HTTPPostMD5(HTTP_CONN_HANDLE connHandle);
    #endif
<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
        static HTTP_IO_RESULT HTTPPostWifiConfig(HTTP_CONN_HANDLE connHandle);
</#if>
    #if defined(HTTP_APP_USE_RECONFIG)
        static HTTP_IO_RESULT HTTPPostConfig(HTTP_CONN_HANDLE connHandle);
        #if defined(TCPIP_STACK_USE_SNMP_SERVER)
        static HTTP_IO_RESULT HTTPPostSNMPCommunity(HTTP_CONN_HANDLE connHandle);
        #endif
    #endif
    #if defined(HTTP_APP_USE_EMAIL) || defined(TCPIP_STACK_USE_SMTP_CLIENT)
        static HTTP_IO_RESULT HTTPPostEmail(HTTP_CONN_HANDLE connHandle);
    #endif
    #if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
        static HTTP_IO_RESULT HTTPPostDDNSConfig(HTTP_CONN_HANDLE connHandle);
    #endif
#endif

<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
static DRV_WIFI_SCAN_RESULT bssDesc;
static bool bssDescIsValid = false;
</#if>

// RAM allocated for DDNS parameters
#if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
    static uint8_t DDNSData[100];
#endif

// Sticky status message variable.
// This is used to indicated whether or not the previous POST operation was
// successful.  The application uses these to store status messages when a
// POST operation redirects.  This lets the application provide status messages
// after a redirect, when connection instance data has already been lost.
static bool lastSuccess = false;

// Stick status message variable.  See lastSuccess for details.
static bool lastFailure = false;

<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
/****************************************************************************
  Section:
    Helper Functions
  ***************************************************************************/

/*******************************************************************************
 * FUNCTION: HexToBin
 *
 * RETURNS:  binary value associated with ASCII hex input value
 *
 * PARAMS:   hexChar -- ASCII hex character
 *
 * NOTES:    Converts an input ascii hex character to its binary value.  Function
 *           does not error check; it assumes only hex characters are passed in.
 *******************************************************************************/
static uint8_t HexToBin(uint8_t hexChar)
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

/*******************************************************************************
 * FUNCTION: ConvertASCIIHexToBinary
 *
 * RETURNS:  true if conversion successful, else false
 *
 * PARAMS:   p_ascii   -- ascii string to be converted
 *           p_binary  -- binary value if conversion successful
 *
 * NOTES:    Converts an input ascii hex string to binary value (up to 32-bit value)
 *******************************************************************************/
static bool ConvertASCIIHexToBinary(char *p_ascii, uint16_t *p_binary)
{
    int8_t i;
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

bool ConvertAsciiToHexInPlace(char *p_string, uint8_t expectedHexBinSize )
{
    char ascii_buffer[3];
    uint8_t hex_binary_index = 0;
    char *hex_string_start = p_string;
    uint16_t hex_buffer = 0;

    /* gobble up any hex prefix */
    if (memcmp(hex_string_start, (const char*) "0x", 2) == 0)
    {
        hex_string_start+=2;
    }

    if (strlen( (char *) hex_string_start) != (expectedHexBinSize*2))
    {
        return false;
    }

    while (hex_binary_index < expectedHexBinSize)
    {
      memcpy (ascii_buffer, (const char*) hex_string_start, 2);
      ascii_buffer[2] = '\0';

      /* convert the hex string to a machine hex value */
      if (!ConvertASCIIHexToBinary(ascii_buffer, &hex_buffer))
      {
        return false;
      }

      p_string[hex_binary_index++] = (uint8_t) hex_buffer;
      hex_string_start +=2;
    }

    return true;
}
</#if><#-- CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo" -->

/****************************************************************************
  Section:
    Authorization Handlers
  ***************************************************************************/

/*****************************************************************************
  Function:
    uint8_t TCPIP_HTTP_FileAuthenticate(HTTP_CONN_HANDLE connHandle, uint8_t* cFile)

  Internal:
    See documentation in the TCP/IP Stack API or HTTP.h for details.
  ***************************************************************************/
#if defined(TCPIP_HTTP_USE_AUTHENTICATION)
uint8_t TCPIP_HTTP_FileAuthenticate(HTTP_CONN_HANDLE connHandle, uint8_t* cFile)
{
    // If the filename begins with the folder "protect", then require auth
    if(memcmp(cFile, (const void*)"protect", 7) == 0)
        return 0x00;        // Authentication will be needed later

    // If the filename begins with the folder "snmp", then require auth
    if(memcmp(cFile, (const void*)"snmp", 4) == 0)
        return 0x00;        // Authentication will be needed later

    #if defined(HTTP_MPFS_UPLOAD_REQUIRES_AUTH)
    if(memcmp(cFile, (const void*)"mpfsupload", 10) == 0)
        return 0x00;
    #endif
    // You can match additional strings here to password protect other files.
    // You could switch this and exclude files from authentication.
    // You could also always return 0x00 to require auth for all files.
    // You can return different values (0x00 to 0x79) to track "realms" for below.

    return 0x80; // No authentication required
}
#endif

/*****************************************************************************
  Function:
    uint8_t TCPIP_HTTP_UserAuthenticate(uint8_t* cUser, uint8_t* cPass)

  Internal:
    See documentation in the TCP/IP Stack API or HTTP.h for details.
  ***************************************************************************/
#if defined(TCPIP_HTTP_USE_AUTHENTICATION)
uint8_t TCPIP_HTTP_UserAuthenticate(HTTP_CONN_HANDLE connHandle, uint8_t* cUser, uint8_t* cPass)
{
    if(strcmp((char *)cUser,(const char *)"admin") == 0
        && strcmp((char *)cPass, (const char *)"microchip") == 0)
        return 0x80;        // We accept this combination

    // You can add additional user/pass combos here.
    // If you return specific "realm" values above, you can base this
    //   decision on what specific file or folder is being accessed.
    // You could return different values (0x80 to 0xff) to indicate
    //   various users or groups, and base future processing decisions
    //   in TCPIP_HTTP_GetExecute/Post or HTTPPrint callbacks on this value.

    return 0x00;            // Provided user/pass is invalid
}
#endif

/****************************************************************************
  Section:
    GET Form Handlers
  ***************************************************************************/
<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
typedef union
{
    uint16_t Val;
    uint8_t v[2];
} MCHP_UINT16_VAL;
</#if>

/*****************************************************************************
  Function:
    HTTP_IO_RESULT TCPIP_HTTP_GetExecute(HTTP_CONN_HANDLE connHandle)

  Internal:
    See documentation in the TCP/IP Stack API or HTTP.h for details.
  ***************************************************************************/
HTTP_IO_RESULT TCPIP_HTTP_GetExecute(HTTP_CONN_HANDLE connHandle)
{
    const uint8_t *ptr;
<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
    const uint8_t *ptr1;
    uint8_t bssIdx;
</#if>
    uint8_t filename[20];
    uint8_t* httpDataBuff;
<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
    MCHP_UINT16_VAL bssIdxStr;
</#if>

    // Load the file name
    // Make sure uint8_t filename[] above is large enough for your longest name
    SYS_FS_FileNameGet(TCPIP_HTTP_CurrentConnectionFileGet(connHandle), filename, 20);

    httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);

    // If its the forms.htm page
    if(!memcmp(filename, "forms.htm", 9))
    {
        // Seek out each of the four LED strings, and if it exists set the LED states
        ptr = TCPIP_HTTP_ArgGet(httpDataBuff, (const uint8_t *)"led2");
        if(ptr)
            BSP_LEDStateSet(APP_TCPIP_LED_3, (*ptr == '1'));
            //LED2_IO = (*ptr == '1');

        ptr = TCPIP_HTTP_ArgGet(httpDataBuff, (const uint8_t *)"led1");
        if(ptr)
            BSP_LEDStateSet(APP_TCPIP_LED_2, (*ptr == '1'));
            //LED1_IO = (*ptr == '1');
    }

    else if(!memcmp(filename, "cookies.htm", 11))
    {
        // This is very simple.  The names and values we want are already in
        // the data array.  We just set the hasArgs value to indicate how many
        // name/value pairs we want stored as cookies.
        // To add the second cookie, just increment this value.
        // remember to also add a dynamic variable callback to control the printout.
        TCPIP_HTTP_CurrentConnectionHasArgsSet(connHandle, 0x01);
    }

    // If it's the LED updater file
    else if(!memcmp(filename, "leds.cgi", 8))
    {
        // Determine which LED to toggle
        ptr = TCPIP_HTTP_ArgGet(httpDataBuff, (const uint8_t *)"led");

        // Toggle the specified LED
        switch(*ptr) {
            case '0':
                BSP_LEDToggle(APP_TCPIP_LED_1);
                //LED0_IO ^= 1;
                break;
            case '1':
                BSP_LEDToggle(APP_TCPIP_LED_2);
                //LED1_IO ^= 1;
                break;
            case '2':
                BSP_LEDToggle(APP_TCPIP_LED_3);
                //LED2_IO ^= 1;
                break;
        }
    }

<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
    else if(!memcmp(filename, "scan.cgi", 8))
    {
        ptr = TCPIP_HTTP_ArgGet(httpDataBuff, (const uint8_t *)"scan");
        ptr1 = TCPIP_HTTP_ArgGet(httpDataBuff, (const uint8_t *)"getBss");

        if ((ptr != NULL) && (ptr1 == NULL))
        {
            bssDescIsValid = false;
            // scan request
            if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
            {
                // display pre-scan results before starting as SoftAP. SoftAP does not scan.
                //SYS_CONSOLE_MESSAGE("HTTPExecuteGet: ... Scan bypass scan ............... \r\n");
            }
            else if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_ADHOC)
            {
                #if (DRV_WIFI_DEFAULT_ADHOC_PRESCAN == DRV_WIFI_ENABLED)
                    // display pre-scan results before starting as SoftAP. SoftAP does not scan.
                    //SYS_CONSOLE_MESSAGE("HTTPExecuteGet: ... Scan bypass scan ............... \r\n");
                #else
                if (WFStartScan() == DRV_WIFI_SUCCESS)
                {
                    SYS_CONSOLE_MESSAGE("HTTPExecuteGet: WFStartScan success ............... \r\n");
                }
                #endif
            }
            else
            {
                if (WFStartScan() == DRV_WIFI_SUCCESS)
                {
                    SYS_CONSOLE_MESSAGE("HTTPExecuteGet: WFStartScan success ............... \r\n");
                }
            }
        }
        else if ((ptr == NULL) && (ptr1 != NULL))
        {
            // getBss request
            // use the value to get the nth bss stored on chip
            bssDescIsValid = false;
            bssIdxStr.v[1] = *ptr1;
            bssIdxStr.v[0] = *(ptr1+1);
            bssIdx = hexatob(bssIdxStr.Val);

            if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
            {
                // display pre-scan results before starting as SoftAP. SoftAP does not scan.
                bssDesc = preScanResult[bssIdx];

                //putsUART("HTTPExecuteGet: SoftAP ... display pre-scan  ................. \r\n");
            }
            else if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_ADHOC)
            {
                #if (DRV_WIFI_DEFAULT_ADHOC_PRESCAN == DRV_WIFI_ENABLED)
                // display pre-scan results before starting as Adhoc.
                bssDesc = preScanResult[bssIdx];

                //putsUART("HTTPExecuteGet: Adhoc ... display pre-scan  ................. \r\n");
                #else
                WFRetrieveScanResult(bssIdx, &bssDesc);
                #endif
            }
            else
            {
                WFRetrieveScanResult(bssIdx, &bssDesc);
            }

            if(bssDesc.ssidLen<32)
                bssDesc.ssid[bssDesc.ssidLen]=0;

            bssDescIsValid = true;
        }
        else
        {
            // impossible to get here
        }
    }
</#if>

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

#if defined(SYS_OUT_ENABLE)
    if(!memcmp(filename, "forms.htm", 9))
        return HTTPPostLCD(connHandle);
#endif

#if defined(HTTP_APP_USE_MD5)
    if(!memcmp(filename, "upload.htm", 10))
        return HTTPPostMD5(connHandle);
#endif

<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
    if(!memcmp(filename, "configure.htm", 13))
        return HTTPPostWifiConfig(connHandle); /* HTTPPostConfig() */
</#if>

#if defined(HTTP_APP_USE_RECONFIG)
    if(!memcmp(filename, "protect/config.htm", 18))
        return HTTPPostConfig(connHandle);
    #if defined(TCPIP_STACK_USE_SNMP_SERVER)
    else if(!memcmp(filename, "snmp/snmpconfig.htm", 19))
        return HTTPPostSNMPCommunity(connHandle);
    #endif
#endif

#if defined(TCPIP_STACK_USE_SMTP_CLIENT)
    if(!strcmp((char*)filename, "email/index.htm"))
        return HTTPPostEmail(connHandle);
#endif

#if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
    if(!strcmp((char*)filename, "dyndns/index.htm"))
        return HTTPPostDDNSConfig(connHandle);
#endif

    return HTTP_IO_DONE;
}

/*****************************************************************************
  Function:
    static HTTP_IO_RESULT HTTPPostLCD(HTTP_CONN_HANDLE connHandle)

  Summary:
    Processes the LCD form on forms.htm

  Description:
    Locates the 'lcd' parameter and uses it to update the text displayed
    on the board's LCD display.

    This function has four states.  The first reads a name from the data
    string returned as part of the POST request.  If a name cannot
    be found, it returns, asking for more data.  Otherwise, if the name
    is expected, it reads the associated value and writes it to the LCD.
    If the name is not expected, the value is discarded and the next name
    parameter is read.

    In the case where the expected string is never found, this function
    will eventually return HTTP_IO_NEED_DATA when no data is left.  In that
    case, the HTTP server will automatically trap the error and issue an
    Internal Server Error to the browser.

  Precondition:
    None

  Parameters:
    connHandle  - HTTP connection handle

  Return Values:
    HTTP_IO_DONE - the parameter has been found and saved
    HTTP_IO_WAITING - the function is pausing to continue later
    HTTP_IO_NEED_DATA - data needed by this function has not yet arrived
  ***************************************************************************/
#if defined(SYS_OUT_ENABLE)
static HTTP_IO_RESULT HTTPPostLCD(HTTP_CONN_HANDLE connHandle)
{
    uint8_t* cDest;
    uint8_t* httpDataBuff;

    #define SM_POST_LCD_READ_NAME       (0u)
    #define SM_POST_LCD_READ_VALUE      (1u)

    httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);
    switch(TCPIP_HTTP_CurrentConnectionPostSmGet(connHandle))
    {
        // Find the name
        case SM_POST_LCD_READ_NAME:

            // Read a name
            if(TCPIP_HTTP_PostNameRead(connHandle, httpDataBuff, TCPIP_HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
                return HTTP_IO_NEED_DATA;

            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_POST_LCD_READ_VALUE);
            // No break...continue reading value

        // Found the value, so store the LCD and return
        case SM_POST_LCD_READ_VALUE:

            // If value is expected, read it to data buffer,
            // otherwise ignore it (by reading to NULL)
            if(!strcmp((char*)httpDataBuff, (const char*)"lcd"))
                cDest = httpDataBuff;
            else
                cDest = NULL;

            // Read a value string
            if(TCPIP_HTTP_PostValueRead(connHandle, cDest, TCPIP_HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
                return HTTP_IO_NEED_DATA;

            // If this was an unexpected value, look for a new name
            if(!cDest)
            {
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_POST_LCD_READ_NAME);
                break;
            }

                        SYS_OUT_MESSAGE((char*)cDest);

            // This is the only expected value, so callback is done
            strcpy((char*)httpDataBuff, "/forms.htm");
            TCPIP_HTTP_CurrentConnectionStatusSet(connHandle, HTTP_REDIRECT);
            return HTTP_IO_DONE;
    }

    // Default assumes that we're returning for state machine convenience.
    // Function will be called again later.
    return HTTP_IO_WAITING;
}
#endif

<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
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
               - WPA Auto passphrase
               - WPA Auto pre-calculated key
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
        uint32_t byteCount;
        TCP_SOCKET sktHTTP;
        uint8_t *httpDataBuff = 0;
        byteCount = TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle);
        sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
        if(byteCount > TCPIP_TCP_GetIsReady(sktHTTP) + TCPIP_TCP_FifoRxFreeGet(sktHTTP))
            goto ConfigFailure;

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
            if(!strcmp((char *)httpDataBuff, (const char *)"sec"))
            {// Read security type
                char security_type[7];

                if (strlen((char *)(httpDataBuff+6)) > 6) /* Sanity check */
                    goto ConfigFailure;

                memcpy(security_type, (void*)(httpDataBuff+6), strlen((char*)(httpDataBuff+6)));
                security_type[strlen((char*)(httpDataBuff+6))] = 0; /* Terminate string */

                if (!strcmp((char *)security_type, (const char *)"no"))
                {
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_OPEN;
                }
                else if(!strcmp((char *)security_type, (const char *)"wpa"))
                {
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE;
                }
                else if(!strcmp((char *)security_type, (const char *)"calc"))
                { /* Pre-calculated key */
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY;
                }
                else if(!strcmp((char *)security_type, (const char *)"wep40"))
                {
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_WEP_40;
                }
                else if(!strcmp((char *)security_type, (const char *)"wep104"))
                {
                    EZ_CFGCXT.security = DRV_WIFI_SECURITY_WEP_104;
                }
                else
                { // Security type no good :-(
                    SYS_CONSOLE_MESSAGE("\r\n1.5 Unknown key type on www\r\n");
                    goto ConfigFailure;
                }
            }
            else if(!strcmp((char *)httpDataBuff, (const char *)"key"))
            { // Read new key material
                uint8_t key_size = 0, ascii_key = 0;
                switch ((uint8_t)EZ_CFGCXT.security) {
                    case DRV_WIFI_SECURITY_OPEN: // keep compiler happy, nothing to do here!
                        break;
                    case DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE: // wpa passphrase
                        //putrsUART((ROM char *)"\r\nPassphrase type of key! ");
                        ascii_key = 1;
                        key_size = strlen((char *)(httpDataBuff+6));
                        //between 8-63 characters, passphrase
                        if ((key_size < 8 ) || (key_size > 63))
                            goto ConfigFailure;
                        break;
                    case DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY: // wpa pre-calculated key!!!
                        key_size = 64;
                        break;
                    case DRV_WIFI_SECURITY_WEP_40:
                        key_size = 10; /* Assume hex size */
                        if (strlen((char *)(httpDataBuff+6)) == 5) {
                            key_size = 5; /* ASCII key support */
                            ascii_key = 1;
                        }
                        EZ_CFGCXT.defaultWepKey = 0; /* Example uses only key idx 0 (sometimes called 1) */
                        break;
                    case DRV_WIFI_SECURITY_WEP_104:
                        key_size = 26; /* Assume hex size */
                        if (strlen((char *)(httpDataBuff+6)) == 13) {
                            key_size = 13; /* ASCII key support */
                            ascii_key = 1;
                        }
                        EZ_CFGCXT.defaultWepKey = 0; /* Example uses only key idx 0 (sometimes called 1) */
                        break;
                }
                if (strlen((char *)(httpDataBuff+6)) != key_size) {
                    SYS_CONSOLE_MESSAGE("\r\nIncomplete key received\r\n");
                    goto ConfigFailure;
                }
                memcpy(EZ_CFGCXT.key, (void *)(httpDataBuff+6), key_size);
                EZ_CFGCXT.key[key_size] = 0; /* terminate string */
                EZ_CFGCXT.SecurityKeyLength = key_size;
                if (!ascii_key) {
                    //if ((cfg.security == sec_wep64) || (cfg.security == sec_wep128))
                        key_size /= 2;
                        //CFGCXT.SecurityKeyLength = key_size; // yes Or No? Need more debug
                    if (!ConvertAsciiToHexInPlace((char *)&EZ_CFGCXT.key[0], key_size)) {
                        SYS_CONSOLE_MESSAGE("\r\nFailed to convert ASCII to hex!\r\n");
                        goto ConfigFailure;
                    }
                }
            }
            else if(!strcmp((char *)httpDataBuff, "ssid"))
            { // Get new ssid and make sure it is valid
                if(strlen((char *)(httpDataBuff+6)) < 33u)
                {
                    memcpy(EZ_CFGCXT.ssid, (void *)(httpDataBuff+6), strlen((char *)(httpDataBuff+6)));
                    EZ_CFGCXT.ssid[strlen((char *)(httpDataBuff+6))] = 0; /* Terminate string */

                    /* save current profile SSID for displaying later */
                    DRV_WIFI_SsidGet((uint8_t *)&EZ_CFGCXT.prevSSID, &ssidLen);
                    EZ_CFGCXT.prevSSID[ssidLen] = 0;
                }
                else
                { // Invalid SSID... fail :-(
                    goto ConfigFailure;
                }
            }
            else if(!strcmp((char *)httpDataBuff, (const char *)"wlan"))
            { // Get the wlan mode: adhoc or infrastructure
                char mode[6];
                if (strlen((char *)(httpDataBuff+6)) > 5) /* Sanity check */
                    goto ConfigFailure;

                memcpy(mode, (void *)(httpDataBuff+6), strlen((char *)(httpDataBuff+6)));
                mode[strlen((char *)(httpDataBuff+6))] = 0; /* Terminate string */
                if(!strcmp((char *)mode, (const char *)"infra"))
                {
                    EZ_CFGCXT.type = DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE;
                }
                else if(!strcmp((char *)mode, "adhoc"))
                {
                    DRV_WIFI_ADHOC_NETWORK_CONTEXT adhocContext;
                    EZ_CFGCXT.type = DRV_WIFI_NETWORK_TYPE_ADHOC;

                    // always setup adhoc to attempt to connect first, then start
                    adhocContext.mode = DRV_WIFI_ADHOC_CONNECT_THEN_START;
                    adhocContext.beaconPeriod = DRV_WIFI_DEFAULT_ADHOC_BEACON_PERIOD;
                    adhocContext.hiddenSsid = DRV_WIFI_DEFAULT_ADHOC_HIDDEN_SSID;
                    DRV_WIFI_AdhocContextSet(&adhocContext);
                }
                else
                {
                    // Mode type no good :-(
                    //memset(LCDText, ' ', 32);
                    //putrsUART((ROM char *)"\r\nUnknown mode type on www! ");
                    //strcpy((char *)LCDText, (char *)mode);
                    //LCDUpdate();
                    SYS_CONSOLE_MESSAGE((const char *)"\r\nUnknown mode type on www! ");
                    goto ConfigFailure;
                }

                // save old WLAN mode
                DRV_WIFI_NetworkTypeGet(&EZ_CFGCXT.prevWLAN);
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
        //TCPIP_NET_IF *p_config= (TCPIP_NET_IF *)GetNetworkConfig();
        strcpy((char *)p_wifi_ConfigData->netSSID, (char *)EZ_CFGCXT.ssid);
        p_wifi_ConfigData->SsidLength = strlen((char *)(EZ_CFGCXT.ssid));
        /* Going to set security type */
        p_wifi_ConfigData->SecurityMode = EZ_CFGCXT.security;
        /* Going to save the key, if required */
        if (EZ_CFGCXT.security != DRV_WIFI_SECURITY_OPEN) {
            uint8_t  key_size =0;

            switch ((uint8_t)EZ_CFGCXT.security) {
                case DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE: // wpa passphrase
                    key_size = strlen((char*)(EZ_CFGCXT.key)); // ascii so use strlen
                    break;
                case DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY: // wpa pre-calculated key!!!
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
            p_wifi_ConfigData->SecurityKey[strlen((char *)(EZ_CFGCXT.key))] = 0;
            p_wifi_ConfigData->SecurityKeyLength = key_size;
        }

        /* Going to save the network type */
        p_wifi_ConfigData->networkType = EZ_CFGCXT.type;
        p_wifi_ConfigData->dataValid = 1; /* Validate wifi configuration */
        DRV_WIFI_ConfigDataSave();

        strcpy((char *)httpDataBuff, "/reconnect.htm");
        TCPIP_HTTP_CurrentConnectionStatusSet(connHandle, HTTP_REDIRECT);
        WiFiAsyncSetEventPending(ASYNC_EASY_CONFIG_PENDING);

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
</#if>

/*****************************************************************************
  Function:
    static HTTP_IO_RESULT HTTPPostConfig(HTTP_CONN_HANDLE connHandle)

  Summary:
    Processes the configuration form on config/index.htm

  Description:
    Accepts configuration parameters from the form, saves them to a
    temporary location in RAM, then eventually saves the data to EEPROM or
    external Flash.

    When complete, this function redirects to config/reboot.htm, which will
    display information on reconnecting to the board.

    This function creates a shadow copy of a network info structure in
    RAM and then overwrites incoming data there as it arrives.  For each
    name/value pair, the name is first read to cur connection data[0:5].  Next, the
    value is read to newNetConfig.  Once all data has been read, the new
    network info structure is saved back to storage and the browser is redirected to
    reboot.htm.  That file includes an AJAX call to reboot.cgi, which
    performs the actual reboot of the machine.

    If an IP address cannot be parsed, too much data is POSTed, or any other
    parsing error occurs, the browser reloads config.htm and displays an error
    message at the top.

  Precondition:
    None

  Parameters:
    connHandle  - HTTP connection handle

  Return Values:
    HTTP_IO_DONE - all parameters have been processed
    HTTP_IO_NEED_DATA - data needed by this function has not yet arrived
  ***************************************************************************/
#if defined(HTTP_APP_USE_RECONFIG)
// network configuration/information storage space
static struct
{
    TCPIP_NET_HANDLE    currNet;              // current working interface + valid flag
    char                ifName[10 + 1];       // interface name
    char                nbnsName[16 + 1];     // host name
    char                ifMacAddr[17 + 1];    // MAC address
    char                ipAddr[15 +1];        // IP address
    char                ipMask[15 + 1];       // mask
    char                gwIP[15 + 1];         // gateway IP address
    char                dns1IP[15 + 1];       // DNS IP address
    char                dns2IP[15 + 1];       // DNS IP address

    TCPIP_NETWORK_CONFIG   netConfig;  // configuration in the interface requested format
}httpNetData;

static HTTP_IO_RESULT HTTPPostConfig(HTTP_CONN_HANDLE connHandle)
{
    uint8_t i;
    IPV4_ADDR newIPAddress, newMask;
    TCPIP_MAC_ADDR    newMACAddr;
    uint32_t byteCount;
    TCP_SOCKET sktHTTP;
    uint8_t* httpDataBuff = 0;
    bool bConfigFailure = false;

    httpNetData.currNet = 0; // forget the old settings

    // Check to see if the browser is attempting to submit more data than we
    // can parse at once.  This function needs to receive all updated
    // parameters and validate them all before committing them to memory so that
    // orphaned configuration parameters do not get written (for example, if a
    // static IP address is given, but the subnet mask fails parsing, we
    // should not use the static IP address).  Everything needs to be processed
    // in a single transaction.  If this is impossible, fail and notify the user.
    // As a web devloper, if you add parameters to the network info and run into this
    // problem, you could fix this by to splitting your update web page into two
    // seperate web pages (causing two transactional writes).  Alternatively,
    // you could fix it by storing a static shadow copy of network info someplace
    // in memory and using it when info is complete.
    // Lastly, you could increase the TCP RX FIFO size for the HTTP server.
    // This will allow more data to be POSTed by the web browser before hitting this limit.
    byteCount = TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle);
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    if(byteCount > TCPIP_TCP_GetIsReady(sktHTTP) + TCPIP_TCP_FifoRxFreeGet(sktHTTP))
    {   // Configuration Failure
        lastFailure = true;
        TCPIP_HTTP_CurrentConnectionStatusSet(connHandle, HTTP_REDIRECT);
        return HTTP_IO_DONE;
    }

    // Ensure that all data is waiting to be parsed.  If not, keep waiting for
    // all of it to arrive.
    if(TCPIP_TCP_GetIsReady(sktHTTP) < byteCount)
        return HTTP_IO_NEED_DATA;

    // Use current config in non-volatile memory as defaults
    httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);

    // Read all browser POST data
    while(TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle))
    {
        // Read a form field name
        if(TCPIP_HTTP_PostNameRead(connHandle, httpDataBuff, 6) != HTTP_READ_OK)
        {
            bConfigFailure = true;
            break;
        }

        // Read a form field value
        if(TCPIP_HTTP_PostValueRead(connHandle, httpDataBuff + 6, TCPIP_HTTP_MAX_DATA_LEN-6-2) != HTTP_READ_OK)
        {
            bConfigFailure = true;
            break;
        }

        // Parse the value that was read
        if(!strcmp((char*)httpDataBuff, (const char*)"ip"))
        {// Save new static IP Address
            if(!TCPIP_Helper_StringToIPAddress((char*)(httpDataBuff+6), &newIPAddress))
            {
                bConfigFailure = true;
                break;
            }
            strncpy(httpNetData.ipAddr, (char*)httpDataBuff + 6, sizeof(httpNetData.ipAddr));
        }
        else if(!strcmp((char*)httpDataBuff, (const char*)"gw"))
        {// Read new gateway address
            if(!TCPIP_Helper_StringToIPAddress((char*)(httpDataBuff+6), &newIPAddress))
            {
                bConfigFailure = true;
                break;
            }
            strncpy(httpNetData.gwIP, (char*)httpDataBuff + 6, sizeof(httpNetData.gwIP));
        }
        else if(!strcmp((char*)httpDataBuff, (const char*)"sub"))
        {// Read new static subnet
            if(!TCPIP_Helper_StringToIPAddress((char*)(httpDataBuff+6), &newMask))
            {
                bConfigFailure = true;
                break;
            }
            strncpy(httpNetData.ipMask, (char*)httpDataBuff + 6, sizeof(httpNetData.ipMask));
        }
        else if(!strcmp((char*)httpDataBuff, (const char*)"dns1"))
        {// Read new primary DNS server
            if(!TCPIP_Helper_StringToIPAddress((char*)(httpDataBuff+6), &newIPAddress))
            {
                bConfigFailure = true;
                break;
            }
            strncpy(httpNetData.dns1IP, (char*)httpDataBuff + 6, sizeof(httpNetData.dns1IP));
        }
        else if(!strcmp((char*)httpDataBuff, (const char*)"dns2"))
        {// Read new secondary DNS server
            if(!TCPIP_Helper_StringToIPAddress((char*)(httpDataBuff+6), &newIPAddress))
            {
                bConfigFailure = true;
                break;
            }
            strncpy(httpNetData.dns2IP, (char*)httpDataBuff + 6, sizeof(httpNetData.dns2IP));
        }
        else if(!strcmp((char*)httpDataBuff, (const char*)"mac"))
        {   // read the new MAC address
            if(!TCPIP_Helper_StringToMACAddress((char*)(httpDataBuff+6), newMACAddr.v))
            {
                bConfigFailure = true;
                break;
            }
            strncpy(httpNetData.ifMacAddr, (char*)httpDataBuff + 6, sizeof(httpNetData.ifMacAddr));
        }
        else if(!strcmp((char*)httpDataBuff, (const char*)"host"))
        {   // Read new hostname
            strncpy(httpNetData.nbnsName, (char*)httpDataBuff + 6, sizeof(httpNetData.nbnsName));
        }
        else if(!strcmp((char*)httpDataBuff, (const char*)"dhcp"))
        {// Read new DHCP Enabled flag
            httpNetData.netConfig.startFlags = httpDataBuff[6] == '1' ? TCPIP_NETWORK_CONFIG_DHCP_CLIENT_ON : 0;
        }
    }

    if(bConfigFailure == false)
    {
        // All parsing complete!  Save new settings and force an interface restart
        // Set the interface to restart and display reconnecting information
        strcpy((char*)httpDataBuff, "/protect/reboot.htm?");
        TCPIP_Helper_FormatNetBIOSName((uint8_t*)httpNetData.nbnsName);
        memcpy((void*)(httpDataBuff+20), httpNetData.nbnsName, 16);
        httpDataBuff[20+16] = 0x00; // Force null termination
        for(i = 20; i < 20u+16u; i++)
        {
            if(httpDataBuff[i] == ' ')
                httpDataBuff[i] = 0x00;
        }
        httpNetData.currNet = TCPIP_TCP_SocketNetGet(sktHTTP);   // save current interface and mark as valid
        strncpy(httpNetData.ifName, TCPIP_STACK_NetNameGet(httpNetData.currNet), sizeof(httpNetData.ifName));
    }
    else
    {   // Configuration error

        lastFailure = true;
        if(httpDataBuff)
        {
            strcpy((char*)httpDataBuff, "/protect/config.htm");
        }
    }

    TCPIP_HTTP_CurrentConnectionStatusSet(connHandle, HTTP_REDIRECT);

    return HTTP_IO_DONE;
}

#if defined(TCPIP_STACK_USE_SNMP_SERVER)
static HTTP_IO_RESULT HTTPPostSNMPCommunity(HTTP_CONN_HANDLE connHandle)
{
    uint8_t vCommunityIndex;
    uint8_t* httpDataBuff;
    uint8_t     len=0;

    #define SM_CFG_SNMP_READ_NAME   (0u)
    #define SM_CFG_SNMP_READ_VALUE  (1u)

    httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);
    switch(TCPIP_HTTP_CurrentConnectionPostSmGet(connHandle))
    {
        case SM_CFG_SNMP_READ_NAME:
            // If all parameters have been read, end
            if(TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle) == 0u)
            {
                return HTTP_IO_DONE;
            }

            // Read a name
            if(TCPIP_HTTP_PostNameRead(connHandle, httpDataBuff, TCPIP_HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
                return HTTP_IO_NEED_DATA;

            // Move to reading a value, but no break
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_CFG_SNMP_READ_VALUE);

        case SM_CFG_SNMP_READ_VALUE:
            // Read a value
            if(TCPIP_HTTP_PostValueRead(connHandle, httpDataBuff + 6, TCPIP_HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
                return HTTP_IO_NEED_DATA;

            // Default action after this is to read the next name, unless there's an error
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_CFG_SNMP_READ_NAME);

            // See if this is a known parameter and legal (must be null
            // terminator in 4th field name byte, string must no greater than
            // TCPIP_SNMP_COMMUNITY_MAX_LEN bytes long, and TCPIP_SNMP_MAX_COMMUNITY_SUPPORT
            // must not be violated.
            vCommunityIndex = httpDataBuff[3] - '0';
            if(vCommunityIndex >= TCPIP_SNMP_MAX_COMMUNITY_SUPPORT)
                break;
            if(httpDataBuff[4] != 0x00u)
                break;
            len = strlen((char*)httpDataBuff + 6);
            if(len > TCPIP_SNMP_COMMUNITY_MAX_LEN)
            {
                break;
            }
            if(memcmp((void*)httpDataBuff, (const void*)"rcm", 3) == 0)
            {
                if(TCPIP_SNMP_ReadCommunitySet(vCommunityIndex,len,httpDataBuff+6)!=true)
                    break;
            }
            else if(memcmp((void*)httpDataBuff, (const void*)"wcm", 3) == 0)
            {
                if(TCPIP_SNMP_WriteCommunitySet(vCommunityIndex,len,httpDataBuff+6) != true)
                    break;
            }
            else
            {
                break;
            }

            break;
    }

    return HTTP_IO_WAITING;     // Assume we're waiting to process more data
}
#endif // #if defined(TCPIP_STACK_USE_SNMP_SERVER)

#endif // #if defined(HTTP_APP_USE_RECONFIG)

/*****************************************************************************
  Function:
    static HTTP_IO_RESULT HTTPPostMD5(HTTP_CONN_HANDLE connHandle)

  Summary:
    Processes the file upload form on upload.htm

  Description:
    This function demonstrates the processing of file uploads.  First, the
    function locates the file data, skipping over any headers that arrive.
    Second, it reads the file 64 bytes at a time and hashes that data.  Once
    all data has been received, the function calculates the MD5 sum and
    stores it in current connection data buffer.

    After the headers, the first line from the form will be the MIME
    separator.  Following that is more headers about the file, which we
    discard.  After another CRLFCRLF, the file data begins, and we read
    it 16 bytes at a time and add that to the MD5 calculation.  The reading
    terminates when the separator string is encountered again on its own
    line.  Notice that the actual file data is trashed in this process,
    allowing us to accept files of arbitrary size, not limited by RAM.
    Also notice that the data buffer is used as an arbitrary storage array
    for the result.  The ~uploadedmd5~ callback reads this data later to
    send back to the client.

  Precondition:
    None

  Parameters:
    connHandle  - HTTP connection handle

  Return Values:
    HTTP_IO_DONE - all parameters have been processed
    HTTP_IO_WAITING - the function is pausing to continue later
    HTTP_IO_NEED_DATA - data needed by this function has not yet arrived
  ***************************************************************************/
#if defined(HTTP_APP_USE_MD5)
static HTTP_IO_RESULT HTTPPostMD5(HTTP_CONN_HANDLE connHandle)
{
    uint32_t lenA, lenB;
    static HASH_SUM md5;            // Assume only one simultaneous MD5
    TCP_SOCKET sktHTTP;
    uint8_t* httpDataBuff;

    #define SM_MD5_READ_SEPARATOR   (0u)
    #define SM_MD5_SKIP_TO_DATA     (1u)
    #define SM_MD5_READ_DATA        (2u)
    #define SM_MD5_POST_COMPLETE    (3u)

    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    switch(TCPIP_HTTP_CurrentConnectionPostSmGet(connHandle))
    {
        // Just started, so try to find the separator string
        case SM_MD5_READ_SEPARATOR:
            // Reset the MD5 calculation
            MD5Initialize(&md5);

            // See if a CRLF is in the buffer
            lenA = TCPIP_TCP_ArrayFind(sktHTTP, (const uint8_t*)"\r\n", 2, 0, 0, false);
            if(lenA == 0xffff)
            {//if not, ask for more data
                return HTTP_IO_NEED_DATA;
            }

            // If so, figure out where the last byte of data is
            // Data ends at CRLFseparator--CRLF, so 6+len bytes
            TCPIP_HTTP_CurrentConnectionByteCountDec(connHandle, lenA + 6);

            // Read past the CRLF
            TCPIP_HTTP_CurrentConnectionByteCountDec(connHandle, TCPIP_TCP_ArrayGet(sktHTTP, NULL, lenA+2));

            // Save the next state (skip to CRLFCRLF)
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_MD5_SKIP_TO_DATA);

            // No break...continue reading the headers if possible

        // Skip the headers
        case SM_MD5_SKIP_TO_DATA:
            // Look for the CRLFCRLF
            lenA = TCPIP_TCP_ArrayFind(sktHTTP, (const uint8_t*)"\r\n\r\n", 4, 0, 0, false);

            if(lenA != 0xffff)
            {// Found it, so remove all data up to and including
                lenA = TCPIP_TCP_ArrayGet(sktHTTP, NULL, lenA+4);
                TCPIP_HTTP_CurrentConnectionByteCountDec(connHandle, lenA);
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_MD5_READ_DATA);
            }
            else
            {// Otherwise, remove as much as possible
                lenA = TCPIP_TCP_ArrayGet(sktHTTP, NULL, TCPIP_TCP_GetIsReady(sktHTTP) - 4);
                TCPIP_HTTP_CurrentConnectionByteCountDec(connHandle, lenA);

                // Return the need more data flag
                return HTTP_IO_NEED_DATA;
            }

            // No break if we found the header terminator

        // Read and hash file data
        case SM_MD5_READ_DATA:
            // Find out how many bytes are available to be read
            httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);
            lenA = TCPIP_TCP_GetIsReady(sktHTTP);
            lenB = TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle);
            if(lenA > lenB)
                lenA = lenB;

            while(lenA > 0u)
            {// Add up to 64 bytes at a time to the sum
                lenB = TCPIP_TCP_ArrayGet(sktHTTP, httpDataBuff, (lenA < 64u)?lenA:64);
                TCPIP_HTTP_CurrentConnectionByteCountDec(connHandle, lenB);
                lenA -= lenB;
                MD5AddData(&md5, httpDataBuff, lenB);
            }

            // If we've read all the data
            if(TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle) == 0u)
            {// Calculate and copy result data buffer for printout
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_MD5_POST_COMPLETE);
                MD5Calculate(&md5, httpDataBuff);
                return HTTP_IO_DONE;
            }

            // Ask for more data
            return HTTP_IO_NEED_DATA;
    }

    return HTTP_IO_DONE;
}
#endif // #if defined(HTTP_APP_USE_MD5)

/*****************************************************************************
  Function:
    static HTTP_IO_RESULT HTTPPostEmail(void)

  Summary:
    Processes the e-mail form on email/index.htm

  Description:
    This function sends an e-mail message using the SMTP client and
    optionally encrypts the connection to the SMTP server.  It
    demonstrates the use of the SMTP client, waiting for asynchronous
    processes in an HTTP callback, and how to send e-mail attachments using
    the stack.

    Messages with attachments are sent using multipart/mixed MIME encoding,
    which has three sections.  The first has no headers, and is only to be
    displayed by old clients that cannot interpret the MIME format.  (The
    overwhelming majority of these clients have been obseleted, but the
    so-called "ignored" section is still used.)  The second has a few
    headers to indicate that it is the main body of the message in plain-
    text encoding.  The third section has headers indicating an attached
    file, along with its name and type.  All sections are separated by a
    boundary string, which cannot appear anywhere else in the message.

  Precondition:
    None

  Parameters:
    connHandle  - HTTP connection handle

  Return Values:
    HTTP_IO_DONE - the message has been sent
    HTTP_IO_WAITING - the function is waiting for the SMTP process to complete
    HTTP_IO_NEED_DATA - data needed by this function has not yet arrived
  ***************************************************************************/
#if defined(TCPIP_STACK_USE_SMTP_CLIENT)
static HTTP_IO_RESULT HTTPPostEmail(HTTP_CONN_HANDLE connHandle)
{
    static uint8_t *ptrData;
    static uint8_t *szPort;
    static TCPIP_SMTP_CLIENT_MESSAGE mySMTPClient;
    uint16_t len, rem;
    uint8_t cName[8];
    uint8_t* httpDataBuff;
    TCP_SOCKET sktHTTP;

    #define SM_EMAIL_CLAIM_MODULE               (0u)
    #define SM_EMAIL_READ_PARAM_NAME            (1u)
    #define SM_EMAIL_READ_PARAM_VALUE           (2u)
    #define SM_EMAIL_PUT_IGNORED                (3u)
    #define SM_EMAIL_PUT_BODY                   (4u)
    #define SM_EMAIL_PUT_ATTACHMENT_HEADER      (5u)
    #define SM_EMAIL_PUT_ATTACHMENT_DATA_BTNS   (6u)
    #define SM_EMAIL_PUT_ATTACHMENT_DATA_LEDS   (7u)
    #define SM_EMAIL_PUT_ATTACHMENT_DATA_POT    (8u)
    #define SM_EMAIL_PUT_TERMINATOR             (9u)
    #define SM_EMAIL_FINISHING                  (10u)

    httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    switch(TCPIP_HTTP_CurrentConnectionPostSmGet(connHandle))
    {
        case SM_EMAIL_CLAIM_MODULE:
            // Try to claim module
            if(TCPIP_SMTP_UsageBegin())
            {// Module was claimed, so set up static parameters
                memset(&mySMTPClient, 0, sizeof(mySMTPClient));
                mySMTPClient.Subject = "Microchip TCP/IP Stack Status Update";
                mySMTPClient.From = "\"SMTP Service\" <mchpboard@picsaregood.com>";

                // The following two lines indicate to the receiving client that
                // this message has an attachment.  The boundary field *must not*
                // be included anywhere in the content of the message.  In real
                // applications it is typically a long random string.
                mySMTPClient.OtherHeaders = "MIME-version: 1.0\r\nContent-type: multipart/mixed; boundary=\"frontier\"\r\n";

                // Move our state machine forward
                ptrData = httpDataBuff;
                szPort = NULL;
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_READ_PARAM_NAME);
            }
            return HTTP_IO_WAITING;

        case SM_EMAIL_READ_PARAM_NAME:
            // Search for a parameter name in POST data
            if(TCPIP_HTTP_PostNameRead(connHandle, cName, sizeof(cName)) == HTTP_READ_INCOMPLETE)
                return HTTP_IO_NEED_DATA;

            // Try to match the name value
            if(!strcmp((char*)cName, (const char*)"server"))
            {// Read the server name
                mySMTPClient.Server = (char*)ptrData;
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_READ_PARAM_VALUE);
            }
            else if(!strcmp((char*)cName, (const char*)"port"))
            {// Read the server port
                szPort = ptrData;
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_READ_PARAM_VALUE);
            }
            else if(!strcmp((char*)cName, (const char*)"user"))
            {// Read the user name
                mySMTPClient.Username = (char*)ptrData;
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_READ_PARAM_VALUE);
            }
            else if(!strcmp((char*)cName, (const char*)"pass"))
            {// Read the password
                mySMTPClient.Password = (char*)ptrData;
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_READ_PARAM_VALUE);
            }
            else if(!strcmp((char*)cName, (const char*)"to"))
            {// Read the To string
                mySMTPClient.To = (char*)ptrData;
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_READ_PARAM_VALUE);
            }
            else if(!strcmp((char*)cName, (const char*)"msg"))
            {// Done with headers, move on to the message
                // Delete paramters that are just null strings (no data from user) or illegal (ex: password without username)
                if(mySMTPClient.Server )
                    if(*mySMTPClient.Server == 0x00u)
                        mySMTPClient.Server = NULL;
                if(mySMTPClient.Username )
                    if(*mySMTPClient.Username == 0x00u)
                        mySMTPClient.Username = NULL;
                if(mySMTPClient.Password)
                    if((*mySMTPClient.Password == 0x00u) || (mySMTPClient.Username == NULL))
                        mySMTPClient.Password = NULL;

                // Decode server port string if it exists
                if(szPort)
                    if(*szPort)
                        mySMTPClient.ServerPort = (uint16_t)atol((char*)szPort);

                // Start sending the message
                TCPIP_SMTP_MailSend(&mySMTPClient);
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_PUT_IGNORED);
                return HTTP_IO_WAITING;
            }
            else
            {// Don't know what we're receiving
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_READ_PARAM_VALUE);
            }

            // No break...continue to try reading the value

        case SM_EMAIL_READ_PARAM_VALUE:
            // Search for a parameter value in POST data
            rem = TCPIP_HTTP_MAX_DATA_LEN - (ptrData - httpDataBuff);
            if(TCPIP_HTTP_PostValueRead(connHandle, ptrData, rem) == HTTP_READ_INCOMPLETE)
                return HTTP_IO_NEED_DATA;

            // Move past the data that was just read
            ptrData += strlen((char*)ptrData);
            if(ptrData < httpDataBuff + TCPIP_HTTP_MAX_DATA_LEN - 1)
                ptrData += 1;

            // Try reading the next parameter
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_READ_PARAM_NAME);
            return HTTP_IO_WAITING;

        case SM_EMAIL_PUT_IGNORED:
            // This section puts a message that is ignored by compatible clients.
            // This text will not display unless the receiving client is obselete
            // and does not understand the MIME structure.
            // The "--frontier" indicates the start of a section, then any
            // needed MIME headers follow, then two CRLF pairs, and then
            // the actual content (which will be the body text in the next state).

            // Check to see if a failure occured
            if(!TCPIP_SMTP_IsBusy())
            {
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_FINISHING);
                return HTTP_IO_WAITING;
            }

            // See if we're ready to write data
            if(TCPIP_SMTP_IsPutReady() < 90u)
                return HTTP_IO_WAITING;

            // Write the ignored text
            TCPIP_SMTP_StringPut("This is a multi-part message in MIME format.\r\n");
            TCPIP_SMTP_StringPut("--frontier\r\nContent-type: text/plain\r\n\r\n");
            TCPIP_SMTP_Flush();

            // Move to the next state
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_PUT_BODY);

        case SM_EMAIL_PUT_BODY:
            // Write as much body text as is available from the TCP buffer
            // return HTTP_IO_NEED_DATA or HTTP_IO_WAITING
            // On completion, => PUT_ATTACHMENT_HEADER and continue

            // Check to see if a failure occurred
            if(!TCPIP_SMTP_IsBusy())
            {
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_FINISHING);
                return HTTP_IO_WAITING;
            }

            // Loop as long as data remains to be read
            while(TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle))
            {
                // See if space is available to write
                len = TCPIP_SMTP_IsPutReady();
                if(len == 0u)
                    return HTTP_IO_WAITING;

                // See if data is ready to be read
                rem = TCPIP_TCP_GetIsReady(sktHTTP);
                if(rem == 0u)
                    return HTTP_IO_NEED_DATA;

                // Only write as much as we can handle
                if(len > rem)
                    len = rem;
                if(len > TCPIP_HTTP_MAX_DATA_LEN - 2)
                    len = TCPIP_HTTP_MAX_DATA_LEN - 2;

                // Read the data from HTTP POST buffer and send it to SMTP
                TCPIP_HTTP_CurrentConnectionByteCountDec(connHandle, TCPIP_TCP_ArrayGet(sktHTTP, httpDataBuff, len));
                httpDataBuff[len] = '\0';
                TCPIP_HTTP_URLDecode(httpDataBuff);
                TCPIP_SMTP_StringPut((char*)httpDataBuff);
                TCPIP_SMTP_Flush();
            }

            // We're done with the POST data, so continue
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_PUT_ATTACHMENT_HEADER);

        case SM_EMAIL_PUT_ATTACHMENT_HEADER:
            // This section writes the attachment to the message.
            // This portion generally will not display in the reader, but
            // will be downloadable to the local machine.  Use caution
            // when selecting the content-type and file name, as certain
            // types and extensions are blocked by virus filters.

            // The same structure as the message body is used.
            // Any attachment must not include high-bit ASCII characters or
            // binary data.  If binary data is to be sent, the data should
            // be encoded using Base64 and a MIME header should be added:
            // Content-transfer-encoding: base64

            // Check to see if a failure occurred
            if(!TCPIP_SMTP_IsBusy())
            {
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_FINISHING);
                return HTTP_IO_WAITING;
            }

            // See if we're ready to write data
            if(TCPIP_SMTP_IsPutReady() < 100u)
                return HTTP_IO_WAITING;

            // Write the attachment header
            TCPIP_SMTP_StringPut("\r\n--frontier\r\nContent-type: text/csv\r\nContent-Disposition: attachment; filename=\"status.csv\"\r\n\r\n");
            TCPIP_SMTP_Flush();

            // Move to the next state
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_PUT_ATTACHMENT_DATA_BTNS);

        case SM_EMAIL_PUT_ATTACHMENT_DATA_BTNS:
            // The following states output the system status as a CSV file.

            // Check to see if a failure occurred
            if(!TCPIP_SMTP_IsBusy())
            {
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_FINISHING);
                return HTTP_IO_WAITING;
            }

            // See if we're ready to write data
            if(TCPIP_SMTP_IsPutReady() < 36u)
                return HTTP_IO_WAITING;

            // Write the header and button strings
            TCPIP_SMTP_StringPut("SYSTEM STATUS\r\n");
            TCPIP_SMTP_StringPut("Buttons:,");
            TCPIP_SMTP_Put(BSP_SwitchStateGet(APP_TCPIP_SWITCH_1) + '0');
            TCPIP_SMTP_Put(',');
            TCPIP_SMTP_Put(BSP_SwitchStateGet(APP_TCPIP_SWITCH_2) + '0');
            TCPIP_SMTP_Put(',');
            TCPIP_SMTP_Put(BSP_SwitchStateGet(APP_TCPIP_SWITCH_3) + '0');
            TCPIP_SMTP_Put('\r');
            TCPIP_SMTP_Put('\n');
            TCPIP_SMTP_Flush();

            // Move to the next state
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_PUT_ATTACHMENT_DATA_LEDS);

        case SM_EMAIL_PUT_ATTACHMENT_DATA_LEDS:
            // Check to see if a failure occurred
            if(!TCPIP_SMTP_IsBusy())
            {
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_FINISHING);
                return HTTP_IO_WAITING;
            }

            // See if we're ready to write data
            if(TCPIP_SMTP_IsPutReady() < 30u)
                return HTTP_IO_WAITING;

            // Write the header and button strings
            TCPIP_SMTP_StringPut("LEDs:,");
            TCPIP_SMTP_Put(BSP_LEDStateGet(APP_TCPIP_LED_1) + '0');
            TCPIP_SMTP_Put(',');
            TCPIP_SMTP_Put(BSP_LEDStateGet(APP_TCPIP_LED_2) + '0');
            TCPIP_SMTP_Put(',');
            TCPIP_SMTP_Put(BSP_LEDStateGet(APP_TCPIP_LED_3) + '0');
            TCPIP_SMTP_Put('\r');
            TCPIP_SMTP_Put('\n');
            TCPIP_SMTP_Flush();

            // Move to the next state
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_PUT_ATTACHMENT_DATA_POT);

        case SM_EMAIL_PUT_ATTACHMENT_DATA_POT:
            // Check to see if a failure occurred
            if(!TCPIP_SMTP_IsBusy())
            {
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_FINISHING);
                return HTTP_IO_WAITING;
            }

            // See if we're ready to write data
            if(TCPIP_SMTP_IsPutReady() < 16u)
                return HTTP_IO_WAITING;

            // Display Random Number
            len = (uint16_t)SYS_RANDOM_PseudoGet();

            uitoa(len, (uint8_t*)&httpDataBuff[1]);

            // Write the header and button strings
            TCPIP_SMTP_StringPut("Pot:,");
            TCPIP_SMTP_StringPut((char*)(httpDataBuff+1));
            TCPIP_SMTP_Put('\r');
            TCPIP_SMTP_Put('\n');
            TCPIP_SMTP_Flush();

            // Move to the next state
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_PUT_TERMINATOR);

        case SM_EMAIL_PUT_TERMINATOR:
            // This section finishes the message
            // This consists of two dashes, the boundary, and two more dashes
            // on a single line, followed by a CRLF pair to terminate the message.

            // Check to see if a failure occured
            if(!TCPIP_SMTP_IsBusy())
            {
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_FINISHING);
                return HTTP_IO_WAITING;
            }

            // See if we're ready to write data
            if(TCPIP_SMTP_IsPutReady() < 16u)
                return HTTP_IO_WAITING;

            // Write the ignored text
            TCPIP_SMTP_StringPut("--frontier--\r\n");
            TCPIP_SMTP_PutIsDone();
            TCPIP_SMTP_Flush();

            // Move to the next state
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_EMAIL_FINISHING);

        case SM_EMAIL_FINISHING:
            // Wait for status
            if(!TCPIP_SMTP_IsBusy())
            {
                // Release the module and check success
                // Redirect the user based on the result
                if(TCPIP_SMTP_UsageEnd() == SMTP_SUCCESS)
                    lastSuccess = true;
                else
                    lastFailure = true;

                // Redirect to the page
                strcpy((char*)httpDataBuff, "/email/index.htm");
                TCPIP_HTTP_CurrentConnectionStatusSet(connHandle, HTTP_REDIRECT);
                return HTTP_IO_DONE;
            }

            return HTTP_IO_WAITING;
    }

    return HTTP_IO_DONE;
}
#endif  // #if defined(TCPIP_STACK_USE_SMTP_CLIENT)

/****************************************************************************
  Function:
    HTTP_IO_RESULT HTTPPostDDNSConfig(HTTP_CONN_HANDLE connHandle)

  Summary:
    Parsing and collecting http data received from http form.

  Description:
    This routine will be excuted every time the Dynamic DNS Client
    configuration form is submitted.  The http data is received
    as a string of the variables seperated by '&' characters in the TCP RX
    buffer.  This data is parsed to read the required configuration values,
    and those values are populated to the global array (DDNSData) reserved
    for this purpose.  As the data is read, DDNSPointers is also populated
    so that the dynamic DNS client can execute with the new parameters.

  Precondition:
     cur HTTP connection is loaded.

  Parameters:
    connHandle  - HTTP connection handle

  Return Values:
    HTTP_IO_DONE        -  Finished with procedure
    HTTP_IO_NEED_DATA   -  More data needed to continue, call again later
    HTTP_IO_WAITING     -  Waiting for asynchronous process to complete,
                            call again later
  ***************************************************************************/
#if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
static HTTP_IO_RESULT HTTPPostDDNSConfig(HTTP_CONN_HANDLE connHandle)
{
    static uint8_t *ptrDDNS;
    uint8_t* httpDataBuff;
    uint8_t smPost;

    #define SM_DDNS_START           (0u)
    #define SM_DDNS_READ_NAME       (1u)
    #define SM_DDNS_READ_VALUE      (2u)
    #define SM_DDNS_READ_SERVICE    (3u)
    #define SM_DDNS_DONE            (4u)

    #define DDNS_SPACE_REMAINING                (sizeof(DDNSData) - (ptrDDNS - DDNSData))

    httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);
    smPost = TCPIP_HTTP_CurrentConnectionPostSmGet(connHandle);
    switch(smPost)
    {
        // Sets defaults for the system
        case SM_DDNS_START:
            ptrDDNS = DDNSData;
            TCPIP_DDNS_ServiceSet(0);
            DDNSClient.Host.szROM = NULL;
            DDNSClient.Username.szROM = NULL;
            DDNSClient.Password.szROM = NULL;
            DDNSClient.ROMPointers.Host = 0;
            DDNSClient.ROMPointers.Username = 0;
            DDNSClient.ROMPointers.Password = 0;
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, ++smPost);

        // Searches out names and handles them as they arrive
        case SM_DDNS_READ_NAME:
            // If all parameters have been read, end
            if(TCPIP_HTTP_CurrentConnectionByteCountGet(connHandle) == 0u)
            {
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_DDNS_DONE);
                break;
            }

            // Read a name
            if(TCPIP_HTTP_PostNameRead(connHandle, httpDataBuff, TCPIP_HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
                return HTTP_IO_NEED_DATA;

            if(!strcmp((char *)httpDataBuff, (const char*)"service"))
            {
                // Reading the service (numeric)
                TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_DDNS_READ_SERVICE);
                break;
            }
            else if(!strcmp((char *)httpDataBuff, (const char*)"user"))
                DDNSClient.Username.szRAM = ptrDDNS;
            else if(!strcmp((char *)httpDataBuff, (const char*)"pass"))
                DDNSClient.Password.szRAM = ptrDDNS;
            else if(!strcmp((char *)httpDataBuff, (const char*)"host"))
                DDNSClient.Host.szRAM = ptrDDNS;

            // Move to reading the value for user/pass/host
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, ++smPost);

        // Reads in values and assigns them to the DDNS RAM
        case SM_DDNS_READ_VALUE:
            // Read a name
            if(TCPIP_HTTP_PostValueRead(connHandle, ptrDDNS, DDNS_SPACE_REMAINING) == HTTP_READ_INCOMPLETE)
                return HTTP_IO_NEED_DATA;

            // Move past the data that was just read
            ptrDDNS += strlen((char*)ptrDDNS);
            if(ptrDDNS < DDNSData + sizeof(DDNSData) - 1)
                ptrDDNS += 1;

            // Return to reading names
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_DDNS_READ_NAME);
            break;

        // Reads in a service ID
        case SM_DDNS_READ_SERVICE:
            // Read the integer id
            if(TCPIP_HTTP_PostValueRead(connHandle, httpDataBuff, TCPIP_HTTP_MAX_DATA_LEN) == HTTP_READ_INCOMPLETE)
                return HTTP_IO_NEED_DATA;

            // Convert to a service ID
            TCPIP_DDNS_ServiceSet((uint8_t)atol((char*)httpDataBuff));

            // Return to reading names
            TCPIP_HTTP_CurrentConnectionPostSmSet(connHandle, SM_DDNS_READ_NAME);
            break;

        // Sets up the DDNS client for an update
        case SM_DDNS_DONE:
            // Since user name and password changed, force an update immediately
            TCPIP_DDNS_UpdateForce();

            // Redirect to prevent POST errors
            lastSuccess = true;
            strcpy((char*)httpDataBuff, "/dyndns/index.htm");
            TCPIP_HTTP_CurrentConnectionStatusSet(connHandle, HTTP_REDIRECT);
            return HTTP_IO_DONE;
    }

    return HTTP_IO_WAITING;     // Assume we're waiting to process more data
}
#endif  // #if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)

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

void TCPIP_HTTP_Print_builddate(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, 0x01);
    if(TCPIP_TCP_PutIsReady(sktHTTP) < strlen((const char*)__DATE__" "__TIME__))
        return;

    TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, 0x00);
    TCPIP_TCP_StringPut(sktHTTP, (const void*)__DATE__" "__TIME__);
}

void TCPIP_HTTP_Print_version(HTTP_CONN_HANDLE connHandle)
{
    TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (const void*)TCPIP_STACK_VERSION_STR);
}

const uint8_t HTML_UP_ARROW[] = "up";
const uint8_t HTML_DOWN_ARROW[] = "dn";

void TCPIP_HTTP_Print_btn(HTTP_CONN_HANDLE connHandle, uint16_t num)
{
    // Determine which button
    switch(num)
    {
<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
        case 0:
            num = BSP_SwitchStateGet(APP_TCPIP_SWITCH_1);
            break;
<#else>
        case 0:
            num = BSP_SwitchStateGet(APP_TCPIP_SWITCH_1);
            break;
        case 1:
            num = BSP_SwitchStateGet(APP_TCPIP_SWITCH_2);
            break;
        case 2:
            num = BSP_SwitchStateGet(APP_TCPIP_SWITCH_3);
            break;
</#if>
        default:
            num = 0;
    }

    // Print the output
    TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (num?HTML_UP_ARROW:HTML_DOWN_ARROW));
}

void TCPIP_HTTP_Print_led(HTTP_CONN_HANDLE connHandle, uint16_t num)
{
    // Determine which LED
    switch(num)
    {
        case 0:
<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
            num = BSP_LEDStateGet(APP_TCPIP_LED_1);
<#else>
            // This is a temporary work-around
#if defined(EX16)
            num = BSP_LEDStateGet(APP_TCPIP_LED_1);
#else
            num = BSP_LEDStateGet(APP_TCPIP_LED_3);
#endif
</#if>
            break;
        case 1:
            num = BSP_LEDStateGet(APP_TCPIP_LED_2);
            break;
        case 2:
            num = BSP_LEDStateGet(APP_TCPIP_LED_3);
            break;
        default:
            num = 0;
    }

    // Print the output
    TCPIP_TCP_Put(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (num?'1':'0'));
}

void TCPIP_HTTP_Print_ledSelected(HTTP_CONN_HANDLE connHandle, uint16_t num, uint16_t state)
{
    // Determine which LED to check
    switch(num)
    {
        case 0:
            num = BSP_LEDStateGet(APP_TCPIP_LED_1);
            break;
        case 1:
            num = BSP_LEDStateGet(APP_TCPIP_LED_2);
            break;
        case 2:
            num = BSP_LEDStateGet(APP_TCPIP_LED_3);
            break;
        default:
            num = 0;
    }

    // Print output if true and ON or if false and OFF
    if((state && num) || (!state && !num))
        TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (const uint8_t*)"SELECTED");
}

void TCPIP_HTTP_Print_pot(HTTP_CONN_HANDLE connHandle)
{
    uint8_t AN0String[8];
    uint16_t ADval;

    ADval = (uint16_t)SYS_RANDOM_PseudoGet();

    uitoa(ADval, (uint8_t*)AN0String);

    TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), AN0String);
}

void TCPIP_HTTP_Print_drive(HTTP_CONN_HANDLE connHandle)
{
 TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (const void*)SYS_FS_DRIVE);
}

void TCPIP_HTTP_Print_fstype(HTTP_CONN_HANDLE connHandle)
{
<#if CONFIG_TCPIP_STACK_USE_FS_WRAPPER == true>
<#if CONFIG_TCPIP_SYS_FS_DRIVE?has_content >
<#if CONFIG_TCPIP_SYS_FS_DRIVE == "FLASH">
 TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (const void*)SYS_FS_MPFS_STRING);
<#elseif CONFIG_TCPIP_SYS_FS_DRIVE == "SDCARD">
 TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (const void*)SYS_FS_FATFS_STRING);
</#if> 
</#if>
</#if>
}

void TCPIP_HTTP_Print_hellomsg(HTTP_CONN_HANDLE connHandle)
{
    const uint8_t *ptr;
    TCP_SOCKET sktHTTP;

    ptr = TCPIP_HTTP_ArgGet(TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle), (const uint8_t*)"name");

    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    // We omit checking for space because this is the only data being written
    if(ptr != NULL)
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"Hello, ");
        TCPIP_TCP_StringPut(sktHTTP, ptr);
    }
}

void TCPIP_HTTP_Print_cookiename(HTTP_CONN_HANDLE connHandle)
{
    const uint8_t *ptr;
    TCP_SOCKET sktHTTP;

    ptr = TCPIP_HTTP_ArgGet(TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle), (const uint8_t*)"name");

    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    if(ptr)
        TCPIP_TCP_StringPut(sktHTTP, ptr);
    else
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"not set");
}

void TCPIP_HTTP_Print_cookiefav(HTTP_CONN_HANDLE connHandle)
{
    const uint8_t *ptr;
    TCP_SOCKET sktHTTP;

    ptr = TCPIP_HTTP_ArgGet(TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle), (const uint8_t*)"fav");

    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    if(ptr)
        TCPIP_TCP_StringPut(sktHTTP, ptr);
    else
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"not set");
}

void TCPIP_HTTP_Print_uploadedmd5(HTTP_CONN_HANDLE connHandle)
{
    uint8_t i;
    uint8_t* httpDataBuff;
    TCP_SOCKET sktHTTP;

    // Set a flag to indicate not finished
    TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, 1);

    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    // Make sure there's enough output space
    if(TCPIP_TCP_PutIsReady(sktHTTP) < 32u + 37u + 5u)
        return;

    // Check for flag set in HTTPPostMD5
#if defined(HTTP_APP_USE_MD5)
    if(TCPIP_HTTP_CurrentConnectionPostSmGet(connHandle) != SM_MD5_POST_COMPLETE)
#endif
    {// No file uploaded, so just return
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"<b>Upload a File</b>");
        TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, 0);
        return;
    }

    TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"<b>Uploaded File's MD5 was:</b><br />");
    httpDataBuff = TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle);

    // Write a byte of the md5 sum at a time
    for(i = 0; i < 16u; i++)
    {
        TCPIP_TCP_Put(sktHTTP, btohexa_high(httpDataBuff[i]));
        TCPIP_TCP_Put(sktHTTP, btohexa_low(httpDataBuff[i]));
        if((i & 0x03) == 3u)
            TCPIP_TCP_Put(sktHTTP, ' ');
    }

    TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, 0x00);
}

void HTTPPrintIP(HTTP_CONN_HANDLE connHandle, IPV4_ADDR ip)
{
    uint8_t digits[4];
    uint8_t i;
    TCP_SOCKET sktHTTP;

    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    for(i = 0; i < 4u; i++)
    {
        if(i)
            TCPIP_TCP_Put(sktHTTP, '.');
        uitoa(ip.v[i], digits);
        TCPIP_TCP_StringPut(sktHTTP, digits);
    }
}

void TCPIP_HTTP_Print_config_hostname(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_TCP_StringPut(sktHTTP, (uint8_t*)TCPIP_STACK_NetBIOSName(TCPIP_TCP_SocketNetGet(sktHTTP)));
}

void TCPIP_HTTP_Print_config_dhcpchecked(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    if(TCPIP_DHCP_IsEnabled(TCPIP_TCP_SocketNetGet(sktHTTP)))
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"checked");
    }
}

void TCPIP_HTTP_Print_config_ip(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_NET_HANDLE netH = TCPIP_TCP_SocketNetGet(sktHTTP);
    IPV4_ADDR ipAddress;

    ipAddress.Val = TCPIP_STACK_NetAddress(netH);
    HTTPPrintIP(connHandle, ipAddress);
}

void TCPIP_HTTP_Print_config_gw(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_NET_HANDLE netH = TCPIP_TCP_SocketNetGet(sktHTTP);
    IPV4_ADDR gwAddress;

    gwAddress.Val = TCPIP_STACK_NetAddressGateway(netH);

    HTTPPrintIP(connHandle, gwAddress);
}

void TCPIP_HTTP_Print_config_subnet(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_NET_HANDLE netH = TCPIP_TCP_SocketNetGet(sktHTTP);
    IPV4_ADDR ipMask;

    ipMask.Val = TCPIP_STACK_NetMask(netH);
    HTTPPrintIP(connHandle, ipMask);
}

void TCPIP_HTTP_Print_config_dns1(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_NET_HANDLE netH = TCPIP_TCP_SocketNetGet(sktHTTP);
    IPV4_ADDR priDnsAddr;

    priDnsAddr.Val = TCPIP_STACK_NetAddressDnsPrimary(netH);

    HTTPPrintIP(connHandle, priDnsAddr);
}

void TCPIP_HTTP_Print_config_dns2(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_NET_HANDLE netH = TCPIP_TCP_SocketNetGet(sktHTTP);
    IPV4_ADDR secondDnsAddr;

    secondDnsAddr.Val = TCPIP_STACK_NetAddressDnsSecond(netH);

    HTTPPrintIP(connHandle, secondDnsAddr);
}

void TCPIP_HTTP_Print_config_mac(HTTP_CONN_HANDLE connHandle)
{
    uint8_t i;
    TCP_SOCKET sktHTTP;
    TCPIP_NET_HANDLE hNet;
    const uint8_t* pMacAdd;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    if(TCPIP_TCP_PutIsReady(sktHTTP) < 18u)
    {//need 17 bytes to write a MAC
        TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, 0x01);
        return;
    }

    hNet = TCPIP_TCP_SocketNetGet(sktHTTP);
    pMacAdd = TCPIP_STACK_NetAddressMac(hNet);
    // Write each byte
    for(i = 0; i < 6u; i++)
    {
        if(i)
            TCPIP_TCP_Put(sktHTTP, ':');
        TCPIP_TCP_Put(sktHTTP, btohexa_high(pMacAdd[i]));
        TCPIP_TCP_Put(sktHTTP, btohexa_low(pMacAdd[i]));
    }

    // Indicate that we're done
    TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, 0x00);
    return;
}

// SNMP Read communities configuration page
void TCPIP_HTTP_Print_read_comm(HTTP_CONN_HANDLE connHandle, uint16_t num)
{
#if defined(TCPIP_STACK_USE_SNMP_SERVER)
    uint8_t dest[TCPIP_SNMP_COMMUNITY_MAX_LEN+1];
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    // Ensure no one tries to read illegal memory addresses by specifying
    // illegal num values.
    if(num >= TCPIP_SNMP_MAX_COMMUNITY_SUPPORT)
        return;
    memset(dest,0,sizeof(dest));
    if(TCPIP_SNMP_ReadCommunityGet(num,TCPIP_SNMP_COMMUNITY_MAX_LEN,dest) != true)
        return;
    // Send proper string
    TCPIP_TCP_StringPut(sktHTTP,dest);
#endif
}

// SNMP Write communities configuration page
void TCPIP_HTTP_Print_write_comm(HTTP_CONN_HANDLE connHandle, uint16_t num)
{
#if defined(TCPIP_STACK_USE_SNMP_SERVER)
    uint8_t dest[TCPIP_SNMP_COMMUNITY_MAX_LEN+1];
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    // Ensure no one tries to read illegal memory addresses by specifying
    // illegal num values.
    if(num >= TCPIP_SNMP_MAX_COMMUNITY_SUPPORT)
        return;
    memset(dest,0,sizeof(dest));
    // Send proper string
    if(TCPIP_SNMP_WriteCommunityGet(num,TCPIP_SNMP_COMMUNITY_MAX_LEN,dest) != true)
        return;
    TCPIP_TCP_StringPut(sktHTTP,dest);
#endif
}

void TCPIP_HTTP_Print_reboot(HTTP_CONN_HANDLE connHandle)
{
    // This is not so much a print function, but causes the interface to restart
    // when the configuration is changed.  If called via an AJAX call, this
    // will gracefully restart the interface and bring it back online immediately
    if(httpNetData.currNet != 0)
    {   // valid data
        httpNetData.netConfig.interface = httpNetData.ifName;
        httpNetData.netConfig.hostName = httpNetData.nbnsName;
        httpNetData.netConfig.macAddr = httpNetData.ifMacAddr;
        httpNetData.netConfig.ipAddr = httpNetData.ipAddr;
        httpNetData.netConfig.ipMask = httpNetData.ipMask;
        httpNetData.netConfig.gateway = httpNetData.gwIP;
        httpNetData.netConfig.priDNS = httpNetData.dns1IP;
        httpNetData.netConfig.secondDNS = httpNetData.dns2IP;
        httpNetData.netConfig.powerMode = TCPIP_STACK_IF_POWER_FULL;
        // httpNetData.netConfig.startFlags should be already set;
        httpNetData.netConfig.pMacObject = TCPIP_STACK_MACObjectGet(httpNetData.currNet);

        TCPIP_STACK_NetDown(httpNetData.currNet);
        TCPIP_STACK_NetUp(httpNetData.currNet, &httpNetData.netConfig);
    }
}

void TCPIP_HTTP_Print_rebootaddr(HTTP_CONN_HANDLE connHandle)
{// This is the expected address of the board upon rebooting
    TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), TCPIP_HTTP_CurrentConnectionDataBufferGet(connHandle));
}

void TCPIP_HTTP_Print_ddns_user(HTTP_CONN_HANDLE connHandle)
{
    #if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
    uint32_t callbackPos;

    if(DDNSClient.ROMPointers.Username || !DDNSClient.Username.szRAM)
        return;

    callbackPos = TCPIP_HTTP_CurrentConnectionCallbackPosGet(connHandle);
    if(callbackPos == 0x00u)
        callbackPos = (uint32_t)DDNSClient.Username.szRAM;
    callbackPos = (uint32_t)TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (uint8_t*)callbackPos);
    if(*(uint8_t*)callbackPos == '\0')
        callbackPos = 0x00;
    TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, callbackPos);
    #endif
}

void TCPIP_HTTP_Print_ddns_pass(HTTP_CONN_HANDLE connHandle)
{
    #if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
    uint32_t callbackPos;
    if(DDNSClient.ROMPointers.Password || !DDNSClient.Password.szRAM)
        return;

    callbackPos = TCPIP_HTTP_CurrentConnectionCallbackPosGet(connHandle);

    if(callbackPos == 0x00u)
        callbackPos = (uint32_t)DDNSClient.Password.szRAM;
    callbackPos = (uint32_t)TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (uint8_t*)callbackPos);
    if(*(uint8_t*)callbackPos == '\0')
        callbackPos = 0x00;
    TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, callbackPos);
    #endif
}

void TCPIP_HTTP_Print_ddns_host(HTTP_CONN_HANDLE connHandle)
{
    #if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
    uint32_t callbackPos;
    if(DDNSClient.ROMPointers.Host || !DDNSClient.Host.szRAM)
        return;
    callbackPos = TCPIP_HTTP_CurrentConnectionCallbackPosGet(connHandle);
    if(callbackPos == 0x00u)
        callbackPos = (uint32_t)DDNSClient.Host.szRAM;
    callbackPos = (uint32_t)TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (uint8_t*)callbackPos);
    if(*(uint8_t*)callbackPos == '\0')
        callbackPos = 0x00;
    TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, callbackPos);
    #endif
}

extern const char * const ddnsServiceHosts[];
void TCPIP_HTTP_Print_ddns_service(HTTP_CONN_HANDLE connHandle, uint16_t i)
{
    #if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
    if(!DDNSClient.ROMPointers.UpdateServer || !DDNSClient.UpdateServer.szROM)
        return;
    if((const char*)DDNSClient.UpdateServer.szROM == ddnsServiceHosts[i])
        TCPIP_TCP_StringPut(TCPIP_HTTP_CurrentConnectionSocketGet(connHandle), (const uint8_t*)"selected");
    #endif
}

void TCPIP_HTTP_Print_ddns_status(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    #if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
    DDNS_STATUS s;
    s = TCPIP_DDNS_LastStatusGet();
    if(s == DDNS_STATUS_GOOD || s == DDNS_STATUS_UNCHANGED || s == DDNS_STATUS_NOCHG)
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"ok");
    else if(s == DDNS_STATUS_UNKNOWN)
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"unk");
    else
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"fail");
    #else
    TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"fail");
    #endif
}

void TCPIP_HTTP_Print_ddns_status_msg(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    if(TCPIP_TCP_PutIsReady(sktHTTP) < 75u)
    {
        TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, 0x01);
        return;
    }

    #if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
    switch(TCPIP_DDNS_LastStatusGet())
    {
        case DDNS_STATUS_GOOD:
        case DDNS_STATUS_NOCHG:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"The last update was successful.");
            break;
        case DDNS_STATUS_UNCHANGED:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"The IP has not changed since the last update.");
            break;
        case DDNS_STATUS_UPDATE_ERROR:
        case DDNS_STATUS_CHECKIP_ERROR:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"Could not communicate with DDNS server.");
            break;
        case DDNS_STATUS_INVALID:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"The current configuration is not valid.");
            break;
        case DDNS_STATUS_UNKNOWN:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"The Dynamic DNS client is pending an update.");
            break;
        default:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"An error occurred during the update.<br />The DDNS Client is suspended.");
            break;
    }
    #else
    TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"The Dynamic DNS Client is not enabled.");
    #endif

    TCPIP_HTTP_CurrentConnectionCallbackPosSet(connHandle, 0);
}

void TCPIP_HTTP_Print_smtps_en(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"none");
}

void TCPIP_HTTP_Print_snmp_en(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    #if defined(TCPIP_STACK_USE_SNMP_SERVER)
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"none");
    #else
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"block");
    #endif
}

void TCPIP_HTTP_Print_status_ok(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    if(lastSuccess)
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"block");
    else
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"none");
    lastSuccess = false;
}

void TCPIP_HTTP_Print_status_fail(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    if(lastFailure)
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"block");
    else
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"none");
    lastFailure = false;
}

<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
void TCPIP_HTTP_Print_scan(HTTP_CONN_HANDLE connHandle)
{
    uint8_t scanInProgressString[4];
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    uitoa(IS_SCAN_IN_PROGRESS(SCANCXT.scanState), scanInProgressString);
    TCPIP_TCP_StringPut(sktHTTP, scanInProgressString);
}

void TCPIP_HTTP_Print_fwver(HTTP_CONN_HANDLE connHandle)
{
    static bool firstTime = true;
    static DRV_WIFI_DEVICE_INFO deviceInfo;
    uint8_t fwverString[8];
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    if (firstTime)
    {
        firstTime = false;
        DRV_WIFI_DeviceInfoGet(&deviceInfo); // only call this once, not continually
    }

    sprintf((char *)fwverString,"%02x%02x", deviceInfo.romVersion, deviceInfo.patchVersion);
    TCPIP_TCP_StringPut(sktHTTP, fwverString);
}

void TCPIP_HTTP_Print_ssid(HTTP_CONN_HANDLE connHandle)
{
    static bool firstTime = true;
    static uint8_t ssidString[33];
    static uint8_t ssidLength;
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    // we don't need to check the connection state as the only way this function
    // is called is from the web server.  if the web server is requesting this,
    // then you can infer that we should be connected to the network
    if (firstTime)
    {
        firstTime = false;
        DRV_WIFI_SsidGet(ssidString, &ssidLength);
    }
    TCPIP_TCP_ArrayPut(sktHTTP, ssidString, ssidLength);
}

void TCPIP_HTTP_Print_bssCount(HTTP_CONN_HANDLE connHandle)
{
    uint8_t bssCountString[4];
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    uitoa(SCANCXT.numScanResults, bssCountString);
    TCPIP_TCP_StringPut(sktHTTP, bssCountString);
}

void TCPIP_HTTP_Print_valid(HTTP_CONN_HANDLE connHandle)
{
    uint8_t bssDescIsValidString[4];
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    uitoa((uint8_t)bssDescIsValid, bssDescIsValidString);
    TCPIP_TCP_StringPut(sktHTTP, bssDescIsValidString);
}

void TCPIP_HTTP_Print_name(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    if (bssDescIsValid)
    {
        if(strlen((const char*)bssDesc.ssid)<32)
            TCPIP_TCP_StringPut(sktHTTP, bssDesc.ssid);
        else
        {
            uint8_t buf_tmp[33];
            int i;
            for(i=0;i<32;i++) buf_tmp[i] = bssDesc.ssid[i];
            buf_tmp[32] = 0;
            TCPIP_TCP_StringPut(sktHTTP, buf_tmp);
        }
    }
    else
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t *)"0");
    }
}

void TCPIP_HTTP_Print_privacy(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    uint8_t security = (bssDesc.apConfig & 0xd0) >> 4;
    uint8_t secString[4];

    if (bssDescIsValid)
    {
        uitoa(security, secString);
        TCPIP_TCP_StringPut(sktHTTP, secString);
    }
    else
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"0");
    }
}

void TCPIP_HTTP_Print_wlan(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    uint8_t bssTypeString[4];
    if (bssDescIsValid)
    {
        uitoa(bssDesc.bssType, bssTypeString);
        TCPIP_TCP_StringPut(sktHTTP, bssTypeString);
    }
    else
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"0");
    }
}

void TCPIP_HTTP_Print_strength(HTTP_CONN_HANDLE connHandle)
{
    uint8_t strVal;
    uint8_t strString[4];
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    if (bssDescIsValid)
    {
        if (bssDesc.rssi < 61)
        {
            strVal = 1;
        }
        else if (bssDesc.rssi < 81)
        {
            strVal = 2;
        }
        else if (bssDesc.rssi < 101)
        {
            strVal = 3;
        }
        else
        {
            strVal = 4;
        }

        uitoa(strVal, strString);
        TCPIP_TCP_StringPut(sktHTTP, strString);
    }
    else
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"0");
    }
}

void TCPIP_HTTP_Print_prevSSID(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_TCP_StringPut(sktHTTP, (uint8_t*)EZ_CFGCXT.prevSSID);//prevSSID
}

void TCPIP_HTTP_Print_nextSSID(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);
    TCPIP_TCP_StringPut(sktHTTP, (uint8_t*)EZ_CFGCXT.ssid); //nextSSID
}

void TCPIP_HTTP_Print_prevWLAN(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    if (EZ_CFGCXT.prevWLAN == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE)
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"infrastructure (BSS)");
    }
    else if (EZ_CFGCXT.prevWLAN == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"softAP (BSS)");
    }
    else if (EZ_CFGCXT.prevWLAN == DRV_WIFI_NETWORK_TYPE_ADHOC)
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"adhoc (IBSS)");
    }
    else
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"unknown");
    }
}

void TCPIP_HTTP_Print_currWLAN(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    // index.htm differs between EZConfig and Wi-Fi G Demo in that index.htm
    // in Wi-Fi G Demo displays BSS information of MRF24WG.

    if(p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_ADHOC)
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"AdHoc (IBSS)");
    else if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE)
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"Infrastructure (BSS)");
    else if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"SoftAP (BSS)");
    else if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_P2P)
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WiFi Direct P2P (BSS)");
    else
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"Unknown BSS");
}

void TCPIP_HTTP_Print_nextWLAN(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    if (EZ_CFGCXT.type == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE)
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"infrastructure (BSS)");
    }
    else if (EZ_CFGCXT.type == DRV_WIFI_NETWORK_TYPE_ADHOC)
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"adhoc (IBSS)");
    }
    else if (EZ_CFGCXT.type == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"softAP (BSS)");
    }
    else
    {
        TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"unknown");
    }
}

<#if CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo">
void TCPIP_HTTP_Print_currPrivacy(HTTP_CONN_HANDLE connHandle)
{
    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    switch (p_wifi_ConfigData->SecurityMode)
    {
        case 0:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"None");
            break;
        case 1:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WEP");
            break;
        case 2:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WEP");
            break;
        case 3:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WPA");
            break;
        case 4:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WPA");
            break;
        case 5:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WPA2");
            break;
        case 6:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WPA2");
            break;
        case 7:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WPA-AUTO");
            break;
        case 8:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WPA-AUTO");
            break;
        case 9 :
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WPS PBC");
            break;
        case 10:
            TCPIP_TCP_StringPut(sktHTTP, (const uint8_t*)"WPS PIN");
            break;
        default:
            // impossible to get here!
            break;
    }
}

// ========================
// = Sends the IP Address =
// ========================
void TCPIP_HTTP_Print_ipaddr(HTTP_CONN_HANDLE connHandle)
{
    char ipAddrDisplay[16];

    TCP_SOCKET sktHTTP;
    sktHTTP = TCPIP_HTTP_CurrentConnectionSocketGet(connHandle);

    TCPIP_NET_HANDLE netH = TCPIP_TCP_SocketNetGet(sktHTTP);
    IPV4_ADDR ipAddress;

    ipAddress.Val = TCPIP_STACK_NetAddress(netH);
    sprintf(ipAddrDisplay, "%u.%u.%u.%u", ipAddress.v[0], ipAddress.v[1], ipAddress.v[2], ipAddress.v[3]);
    TCPIP_TCP_StringPut(sktHTTP, (uint8_t*)ipAddrDisplay);
}
</#if><#-- CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo" -->
</#if><#-- CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Easy Configuration Demo" || CONFIG_DRV_WIFI_HTTP_CUSTOM_TEMPLATE == "Wi-Fi G Demo" -->

#endif
