/*******************************************************************************
  MRF24W Driver Medium Access Control (MAC) Layer

  File Name:
    drv_wifi_update_firmware_OTA.c

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
//DOM-IGNORE-END

/*
*********************************************************************************************************
*                                           INCLUDES
*********************************************************************************************************
*/
#include "tcpip/src/tcpip_private.h"
#include "tcpip/tcpip_mac.h"
#include "tcpip/tcpip_mac_object.h"

#if defined(TCPIP_IF_MRF24W)
#if defined(WF_UPDATE_FIRMWARE_OTA)
#include "tcpip/tcpip.h"


extern uint8_t AutoUpdate_Initialize_SM(void);
extern void AutoUpdate_Completed(void);
extern void AutoUpdate_Restore(void);
static void parse_checksum(void);
static void AutoUpdate_SendToModule(uint8_t c, bool all);
bool TCPIP_TCP_ConnectionSetToMSS(uint16_t mssLocal, uint16_t mssNonLocal);

extern uint32_t ImageUpdate_Addr;
extern uint32_t ImageUpdate_Checksum;
extern uint32_t ImageUpdate_Size;
extern bool g_ota_update_started;

static char ServerName[] = "www.microchip.com";
static uint16_t ServerPort = 80;
// Enter RF transceiver FW file name here. Case-sensitive.
static uint8_t PatchName[] = "/mrfupdates/A2Patch_310c.txt";
static uint8_t Key_authorization[] = "bXJmdXBkYXRlczptY2hwMTIzNA==";
static IPV4_ADDR  serverIP;
uint32_t signature_file;
uint32_t length_file;
uint32_t checksum_file;
uint8_t buf_checksum[12] = {0,};
uint8_t buf_checksum_index = 0;
uint8_t parse_index = 0;
uint8_t parse_tmp[4] = {0,};
static char s_helpInfo[1024];

static void AU_print_string(uint8_t *buf,uint8_t length)
{
    int i;

    for (i = 0; i < length; i++)
        s_helpInfo[i] = buf[i];
   
    SYS_CONSOLE_PRINT("%s", s_helpInfo);
}
uint8_t char_to_i(char c)
{
    uint8_t tmp = 0;

    if      (( c >= '0')&&( c <= '9') )  tmp = c-'0';
    else if (( c >= 'a')&&( c <= 'f') )  tmp = c-'a' + 0x0a;
    else if (( c >= 'A')&&( c <= 'F') )  tmp = c-'A' + 0x0a;
    else
    {
        DRV_WIFI_ASSERT(false, "Invalid data");
    }

    return tmp;
}

static void parse_byte(uint8_t c)
{
    uint8_t real_byte;

    if ((c == ',') || (c == 0x0a) || (c == 0x20))
    {
        parse_index = 0;
        return;
    }

    switch (parse_index) {
    case 0:
        if (c == '0')
        {
            parse_tmp[0] = '0';
            parse_index++;
        }
        break;
        
    case 1:
        if (c == 'x')
        {
            parse_tmp[1] = 'x';
            parse_index++;
        }
        else
        {
            DRV_WIFI_ASSERT(false, "It should be 'x'");
        }
        break;
        
    case 2:
        parse_tmp[2] = char_to_i(c);
        parse_index++;
        break;
    case 3:
        parse_tmp[3] = char_to_i(c);
        parse_index++;

        real_byte = parse_tmp[2] << 4;
        real_byte += parse_tmp[3];

        if (buf_checksum_index < 12) // it is head
        {
            buf_checksum[buf_checksum_index] = real_byte;
            buf_checksum_index++;
            if (buf_checksum_index == 12)
            {
                parse_checksum();
            }
        }
        else      //it is real bin data
        {
            //Size and CheckSum
            if ((ImageUpdate_Size % 4u) == 0u) 
                ImageUpdate_Checksum += (uint32_t)real_byte << 24;
            if ((ImageUpdate_Size % 4u) == 1u) 
                ImageUpdate_Checksum += (uint32_t)real_byte << 16;
            if ((ImageUpdate_Size % 4u) == 2u) 
                ImageUpdate_Checksum += (uint32_t)real_byte <<  8;
            if ((ImageUpdate_Size % 4u) == 3u) 
                ImageUpdate_Checksum += (uint32_t)real_byte;
            ImageUpdate_Size++;

            AutoUpdate_SendToModule(real_byte,false);

        }
        break;
        
    case 4:
        parse_tmp[0] = 0; 
        parse_tmp[1] = 0; 
        parse_tmp[2] = 0; 
        parse_tmp[3] = 0;
        
    default:
        break;
        
    }
}

static void parse_checksum(void)
{
    //int i;

    signature_file  = buf_checksum[0] << 24 ;
    signature_file += buf_checksum[1] << 16;
    signature_file += buf_checksum[2] <<  8;
    signature_file += buf_checksum[3] <<  0;

    length_file  = buf_checksum[4] << 24;
    length_file += buf_checksum[5] << 16 ;
    length_file += buf_checksum[6] <<  8 ;
    length_file += buf_checksum[7] <<  0;

    checksum_file  = buf_checksum[ 8] << 24;
    checksum_file += buf_checksum[ 9] << 16 ;
    checksum_file += buf_checksum[10] <<  8 ;
    checksum_file += buf_checksum[11] <<  0;

    //for (i = 0; i < 24; i++)
        //{SYS_CONSOLE_PRINT("%02x;\r\n", buf_checksum[i]);}

    //SYS_CONSOLE_PRINT("signature=0x%x\r\n", signature_file);
    //SYS_CONSOLE_PRINT("length=0x%x\r\n", length_file);
    //SYS_CONSOLE_PRINT("checksum=0x%x\r\n", checksum_file);
}

static void AutoUpdate_SendToModule(uint8_t c, bool all)
{
    static uint8_t buf_module[36]={0};
    static uint8_t index_b = 0;

    if (all == true)
    {
        if (index_b >0)
        {
            buf_module[0] = (ImageUpdate_Addr & 0x00FF0000) >> 16;
            buf_module[1] = (ImageUpdate_Addr & 0x0000FF00) >> 8;
            buf_module[2] = (ImageUpdate_Addr & 0xFF);
            buf_module[3] = index_b;
            SendSetParamMsg(PARAM_FLASH_WRITE, buf_module, index_b + 4);
            ImageUpdate_Addr += index_b;
            index_b = 0;
        }
    }
    else
    {
        buf_module[index_b + 4] = c;
        index_b++;
        if (index_b == 32)
        {
            buf_module[0] = (ImageUpdate_Addr & 0x00FF0000) >> 16;
            buf_module[1] = (ImageUpdate_Addr & 0x0000FF00) >>  8;
            buf_module[2] = (ImageUpdate_Addr & 0xFF);
            buf_module[3] = 32;
            SendSetParamMsg(PARAM_FLASH_WRITE, buf_module, index_b + 4);
            ImageUpdate_Addr += 32;
            index_b = 0;
        }
    }
}

/*******************************************************************************
  Function:
    void WF_FirmwareUpdate_OTA(void)

  Summary:

  Description:

  Precondition:
    TCP is initialized.

  Parameters:
    None

  Returns:
    None
 *******************************************************************************/
void WF_FirmwareUpdate_OTA(void)
{
    uint16_t w;
    uint16_t i,j;
    TCPIP_DNS_RESULT dnsRes;
    uint8_t vBuffer[256];
    static TCPIP_NET_HANDLE netH;
    static TCP_SOCKET MySocket = INVALID_SOCKET;
    static uint32_t Timer    = 0;
    static int SizeOfPackage = 0;
    static int SizeOfReceive = 0;
    uint16_t lenA,lenB;
    static enum _AutoUpdateTCPExampleState
    {
        SM_UPDATE_HOME = 0,
        SM_UPDATE_WAIT_DNS,
        SM_UPDATE_SOCKET_OPEN,
        SM_UPDATE_SOCKET_OBTAINED,
        SM_UPDATE_SENDREQUEST,
        SM_UPDATE_GET_SIZE_1,
        SM_UPDATE_GET_SIZE_2,
        SM_UPDATE_SKIP_TO_DATA,
        SM_UPDATE_READ_DATA,
        SM_UPDATE_DISCONNECT,
        SM_UPDATE_DONE
    } AutoUpdateTCPExampleState = SM_UPDATE_DONE;

    switch (AutoUpdateTCPExampleState) {
    case SM_UPDATE_HOME:
#if (TCPIP_TCP_MAX_SEG_SIZE_RX_NON_LOCAL > TCPIP_TCP_SOCKET_DEFAULT_RX_SIZE)
        TCPIP_TCP_ConnectionSetToMSS(TCPIP_TCP_MAX_SEG_SIZE_RX_LOCAL, TCPIP_TCP_SOCKET_DEFAULT_RX_SIZE);
#endif
        SYS_CONSOLE_MESSAGE((const char*)"\r\n\r\nResolving the server name using Microchip DNS API...\r\n");
        netH = TCPIP_STACK_NetDefaultGet();
        TCPIP_DNS_Resolve(ServerName, DNS_TYPE_A);
        memset(buf_checksum, 0, sizeof(buf_checksum));
        memset(parse_tmp,    0, sizeof(parse_tmp));
        AutoUpdateTCPExampleState++;
        break;

    case SM_UPDATE_WAIT_DNS:
        dnsRes = TCPIP_DNS_IsResolved(ServerName, &serverIP);
        if (dnsRes == DNS_RES_PENDING)
        {   // ongoing operation;
            break;
        }
        else if (dnsRes < 0)
        {   // some DNS error occurred; retry
            SYS_CONSOLE_MESSAGE((const char*)"\r\n\r\nDNS name resolving failed...\r\n");
            TCPIP_TCP_Close(MySocket);
            MySocket = INVALID_SOCKET;
            AutoUpdateTCPExampleState = SM_UPDATE_HOME;
        }
        else
        {
            Timer = SYS_TMR_TickCountGet();
            AutoUpdateTCPExampleState++;
        }
        break;

    case SM_UPDATE_SOCKET_OPEN:
        // Connect the socket to the remote TCP server
        MySocket = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4, ServerPort, (IP_MULTI_ADDRESS*)&serverIP);

        // Abort operation if no TCP socket could be opened.
        // If this ever happens, you need to update your tcp_config.h
        if (MySocket == INVALID_SOCKET)
        {   // retry
            break;
        }

        SYS_CONSOLE_MESSAGE(( char*)"\r\n\r\nUpdate using Microchip TCP API...\r\n");

        for (i = 0; i < 12; i++) 
            buf_checksum[i]=0;
        
        buf_checksum_index = 0;
        
        for (i = 0; i < 4; i++) 
            parse_tmp[i] = 0;
        
        parse_index = 0;

        SizeOfPackage = 0;
        SizeOfReceive = 0;

        AutoUpdateTCPExampleState++;
        Timer = SYS_TMR_TickCountGet();
        break;

    case SM_UPDATE_SOCKET_OBTAINED:
        // Wait for the remote server to accept our connection request
        if (!TCPIP_TCP_IsConnected(MySocket))
        {
            // Time out if more than 5 seconds is spent in this state
            if ((SYS_TMR_TickCountGet()-Timer) > 5 * SYS_TMR_TickCounterFrequencyGet() )
            {
                // Close the socket so it can be used by other modules
                TCPIP_TCP_Close(MySocket);
                MySocket = INVALID_SOCKET;
                AutoUpdateTCPExampleState--;
                SYS_CONSOLE_MESSAGE((const char*)"\r\n\r\nFailed connecting to the remote server...\r\n");
            }
            break;
        }
        
        if (1 == AutoUpdate_Initialize_SM())
        {
            AutoUpdateTCPExampleState++;
        }

        break;

   case SM_UPDATE_SENDREQUEST:
        if (!TCPIP_TCP_IsConnected(MySocket))
        {
            AutoUpdateTCPExampleState = SM_UPDATE_DISCONNECT;
            SYS_CONSOLE_MESSAGE("Connection lost, I am sad...\r\n");
            break;
        }
        Timer = SYS_TMR_TickCountGet();

        // Make certain the socket can be written to
        if (TCPIP_TCP_PutIsReady(MySocket) < 125u)
            break;

        TCPIP_TCP_StringPut(MySocket, (uint8_t*)"GET ");
        TCPIP_TCP_StringPut(MySocket, (uint8_t*)PatchName);
        TCPIP_TCP_StringPut(MySocket, (uint8_t*)" HTTP/1.1\r\nHost:");
        TCPIP_TCP_StringPut(MySocket, (uint8_t*)ServerName);
        TCPIP_TCP_StringPut(MySocket, (uint8_t*)"\r\nConnection: close\r\n");
        TCPIP_TCP_StringPut(MySocket, (uint8_t*)"Authorization: Basic ");
        TCPIP_TCP_StringPut(MySocket, (uint8_t*)Key_authorization);
        TCPIP_TCP_StringPut(MySocket, (uint8_t*)"\r\n\r\n");

        // Send the packet
        TCPIP_TCP_Flush(MySocket);
        AutoUpdateTCPExampleState++;
        break;

    case SM_UPDATE_GET_SIZE_1:  //Looking for "Length:"
        if (!TCPIP_TCP_IsConnected(MySocket))
        {
            AutoUpdateTCPExampleState = SM_UPDATE_DISCONNECT;
            break;
        }
        // 2. Check, if there is data?
        w = TCPIP_TCP_GetIsReady(MySocket);
        if (w == 0) {
            break;
        }

        lenA = TCPIP_TCP_ArrayFind(MySocket, (uint8_t*)"Length:", 7, 0, 0, false);
        if (lenA != 0xffff)
        {// Found it, so remove all data up to and including
            while (lenA > 0)
            {
                lenB = TCPIP_TCP_ArrayGet(MySocket, vBuffer, ((lenA <= sizeof(vBuffer)) ? lenA : sizeof(vBuffer)));
                AU_print_string(vBuffer,lenB);
                lenA -= lenB;
            }
            
            AutoUpdateTCPExampleState = SM_UPDATE_GET_SIZE_2;
            // No break if we found the "Length:"
        }
        else
        {// Otherwise, remove as much as possible
            w -= 7;
            while (w > 0)
            {
                lenB = TCPIP_TCP_ArrayGet(MySocket, vBuffer, ((w <= sizeof(vBuffer)) ? w : sizeof(vBuffer)));
                AU_print_string(vBuffer, lenB);
                w -= lenB;
            }
            
            break;
        }

    case SM_UPDATE_GET_SIZE_2: // Looking for "\r\n", if yes, we get the size of packet
        if (!TCPIP_TCP_IsConnected(MySocket))
        {
            AutoUpdateTCPExampleState = SM_UPDATE_DISCONNECT;
            break;
        }
        
        // 2. Check ,if there is data?
        w = TCPIP_TCP_GetIsReady(MySocket);
        if (w == 0) {
            break;
        }
        
        lenA = TCPIP_TCP_ArrayFind(MySocket, ( uint8_t*)"\r\n", 2, 0, 0, false);
        if (lenA != 0xffff)
        {// Found it, get the size
            lenA = TCPIP_TCP_ArrayGet(MySocket, vBuffer, lenA + 2);
            AU_print_string(vBuffer, lenA);
            sscanf((const char*)vBuffer, "Length: %d\r\n", &SizeOfPackage);
            AutoUpdateTCPExampleState = SM_UPDATE_SKIP_TO_DATA;
            // No break if we found the size
        }
        else
        {// Otherwise, wait to next read
            break;
        }

    case SM_UPDATE_SKIP_TO_DATA: // look for "\r\n\r\n"
        // 1. Check to see if the remote node has disconnected from us or sent us any application data
        if (!TCPIP_TCP_IsConnected(MySocket))
        {
            AutoUpdateTCPExampleState = SM_UPDATE_DISCONNECT;
            // Do not break; We might still have data in the TCP RX FIFO waiting for us
        }

        // 2. Check ,if there is data?
        w = TCPIP_TCP_GetIsReady(MySocket);
        if (w == 0) {
            break;
        }
        
        lenA = TCPIP_TCP_ArrayFind(MySocket, (uint8_t*)"\r\n\r\n", 4, 0, 0,false);

        if (lenA != 0xffff)
        {// Found it, so remove all data up to and including
            while (lenA > 0)
            {
                lenB = TCPIP_TCP_ArrayGet(MySocket, vBuffer, ((lenA <= sizeof(vBuffer)) ? lenA : sizeof(vBuffer)));
                AU_print_string(vBuffer, lenB);
                lenA -= lenB;
            }
            lenB = TCPIP_TCP_ArrayGet(MySocket, vBuffer, 4);
            AU_print_string(vBuffer, lenB);
            AutoUpdateTCPExampleState = SM_UPDATE_READ_DATA;

            // No break if we found the header terminator
        }
        else
        {// Otherwise, remove as much as possible

            w -= 4;
            while (w > 0)
            {
                lenB = TCPIP_TCP_ArrayGet(MySocket, vBuffer, ((w <= sizeof(vBuffer)) ? w : sizeof(vBuffer)));
                AU_print_string(vBuffer,lenB);
                w -= lenB;
            }

            break;
        }

    case SM_UPDATE_READ_DATA:
        if (!TCPIP_TCP_IsConnected(MySocket))
        {
            AutoUpdateTCPExampleState = SM_UPDATE_DISCONNECT;
            // Do not break; We might still have data in the TCP RX FIFO waiting for us
        }
        
        // Get count of RX bytes waiting
        w = TCPIP_TCP_GetIsReady(MySocket);
        j = sizeof(vBuffer) - 1;
        if (w != 0) {
            SYS_CONSOLE_PRINT("\r\n!-Total=%u, Current=%u, Size=%u-!",SizeOfPackage, SizeOfReceive, w);
        }
        
        while (w)
        {
            int i;
            
            if (w < j) {j = w; }
            vBuffer[j] = '\0';
            lenB= TCPIP_TCP_ArrayGet(MySocket, vBuffer, j);
            SizeOfReceive += lenB;
            w -= lenB;
            for (i = 0; i < lenB; i++)
            {
                parse_byte(vBuffer[i]);
            }
        }
        
        break;

    case SM_UPDATE_DISCONNECT:
        // Close the socket so it can be used by other modules
        // For this application, we wish to stay connected, but this state will still get entered if the remote server decides to disconnect
        TCPIP_TCP_Close(MySocket);
        MySocket = INVALID_SOCKET;
        AutoUpdateTCPExampleState = SM_UPDATE_DONE;
        //did we receive enough data? Is the size correct?
        SYS_CONSOLE_PRINT("\r\nImageUpdate_Size=0x%x, ImageUpdate_Checksum=0x%x\r\n",ImageUpdate_Size,ImageUpdate_Checksum);

        if ((length_file == ImageUpdate_Size) && (checksum_file == ImageUpdate_Checksum) && (ImageUpdate_Size != 0))
        {
            AutoUpdate_SendToModule(0, true); // Send all data left in buffer
            AutoUpdate_Completed();
            SYS_CONSOLE_MESSAGE("Update Done!\r\n");
        }
        else // We lost data or checksum error, so cancel , and roll back
        {
            SYS_CONSOLE_MESSAGE("Revert Starts...\r\n");
            AutoUpdate_Restore();
            SYS_CONSOLE_MESSAGE("Revert Done!\r\n");
        }
        
        break;

    case SM_UPDATE_DONE:
        // Do nothing unless the user pushes BUTTON1 and wants to restart the whole connection/download process
        // On many boards, SYS_USERIO_BUTTON_0 is assigned to sw1
        // SYS_USERIO_BUTTON_1=sw2 and SYS_USERIO_BUTTON_2=sw3
        if (g_ota_update_started == true)
        {
            g_ota_update_started = false;
            AutoUpdateTCPExampleState = SM_UPDATE_HOME;
        }
        
        break;

    default:
        break;
    }

    if (AutoUpdateTCPExampleState != SM_UPDATE_DONE)
    {
        WF_UserEventsSet(DRV_WIFI_EVENT_UPDATE, 0, false);
    }
}

#endif /* #if defined(WF_UPDATE_FIRMWARE_OTA) */

#endif // TCPIP_IF_MRF24W
