/*******************************************************************************
  MRF24WN Wireless Driver

  File Name:  
    wdrv_mrf24wn_misc.c  
  
  Summary:
    Module for Microchip TCP/IP Stack PIC32 implementation
    for multiple MAC support
    
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

#include "tcpip/src/tcpip_private.h"

// Compile only for PIC32MX or PIC32MZ MRF24WN MAC interface
#if (defined(__PIC32MZ__) && defined(TCPIP_IF_MRF24WN)) || (defined(__PIC32MX__) && defined(TCPIP_IF_MRF24WN))

#ifdef WDRV_OTA_FILE_NAME
char *g_ota_filename = WDRV_OTA_FILE_NAME;
uint32_t g_ota_host_addr = 0;
const char *g_ota_host_addr_str = WDRV_OTA_SERVER_IP_ADDR_STR;
#else /* dummy information only for compilation */
char *g_ota_filename = "";
uint32_t g_ota_host_addr = 0;
const char *g_ota_host_addr_str = "";
#endif /* WDRV_OTA_ENABLED */

void WDRV_Assert(int condition, const char * msg, const char * file, int line)
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

#endif // (defined(__PIC32MZ__) && defined(TCPIP_IF_MRF24WN)) || (defined(__PIC32MX__) && defined(TCPIP_IF_MRF24WN))

//DOM-IGNORE-END
