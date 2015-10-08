/*******************************************************************************
  MRF24W Driver Medium Access Control (MAC) Layer

  File Name:
    drv_wifi_update_firmware_common.c

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

#if defined(TCPIP_IF_MRF24W)

#include "tcpip/tcpip_mac_object.h"

#if defined(WF_UPDATE_FIRMWARE_XMODEM) && defined(WF_UPDATE_FIRMWARE_OTA)
#error "Please check system_config.h -- cannot have both WF_UPDATE_FIRMWARE_XMODEM and WF_UPDATE_FIRMWARE_OTA defined"
#endif

#if defined(WF_UPDATE_FIRMWARE_XMODEM) || defined(WF_UPDATE_FIRMWARE_OTA)
void AutoUpdate_Restore(void);

uint32_t ImageUpdate_Addr     = 0;
uint32_t ImageUpdate_Checksum = 0;
uint32_t ImageUpdate_Size     = 0;

uint8_t AutoUpdate_Initialize_SM(void)
{   // Initialze AutoUpdate
    uint8_t RtnCode = 0;
    uint8_t buf_command[4];
    static enum _AutoUpdate_Initialize
    {
        AutoUpdate_Initialize_0 = 0,
        AutoUpdate_Initialize_1 = 1,
        AutoUpdate_Initialize_2 = 2
    } Initialize_State = AutoUpdate_Initialize_0;

    switch (Initialize_State) {
    case AutoUpdate_Initialize_0:
        SYS_CONSOLE_MESSAGE("Update Image Initialize..Starts\r\n");
        ImageUpdate_Addr     = 12;
        ImageUpdate_Checksum = 0;
        ImageUpdate_Size     = 0;
        buf_command[1] = UPDATE_SECURITY_KEY_1; 
        buf_command[2] = UPDATE_SECURITY_KEY_2; 
        buf_command[3] = UPDATE_SECURITY_KEY_3;
        buf_command[0] = UPDATE_CMD_ERASE1;                   //Erase bank1
        SendSetParamMsg(PARAM_FLASH_update, buf_command, 4);
        Initialize_State++;
        RtnCode = 0;
        break;
        
    case AutoUpdate_Initialize_1:
        buf_command[0] = UPDATE_CMD_CPY_0TO1;                 //Copy bank0 to bank1
        buf_command[1] = UPDATE_SECURITY_KEY_1; 
        buf_command[2] = UPDATE_SECURITY_KEY_2; 
        buf_command[3] = UPDATE_SECURITY_KEY_3;
        SendSetParamMsg(PARAM_FLASH_update, buf_command, 4);
        Initialize_State++;
        RtnCode = 0;
        break;
        
    case AutoUpdate_Initialize_2:
        buf_command[0] = UPDATE_CMD_ERASE0;                   //Erase bank0
        buf_command[1] = UPDATE_SECURITY_KEY_1; 
        buf_command[2] = UPDATE_SECURITY_KEY_2; 
        buf_command[3] = UPDATE_SECURITY_KEY_3;
        SendSetParamMsg(PARAM_FLASH_update, buf_command, 4);
        SYS_CONSOLE_MESSAGE("Update Image Initialize..Done\r\n");
        Initialize_State = AutoUpdate_Initialize_0;
        RtnCode = 1;
        break;
        
    }
    return RtnCode;
}

void AutoUpdate_Completed(void)
{
    #define PATCH_SIGNATURE (uint32_t)(~(0x00047fbc))
    uint8_t SignatureData[4 + 12];
    uint8_t buf_command[4];
    
    ImageUpdate_Addr = 0;
    SignatureData[0] = (ImageUpdate_Addr & 0x00FF0000) >> 16;
    SignatureData[1] = (ImageUpdate_Addr & 0x0000FF00) >>  8;
    SignatureData[2] = (ImageUpdate_Addr & 0xFF);
    SignatureData[3] = 12;

    SignatureData[4 +  0] = (PATCH_SIGNATURE >> 24) & 0xFF;
    SignatureData[4 +  1] = (PATCH_SIGNATURE >> 16) & 0xFF;
    SignatureData[4 +  2] = (PATCH_SIGNATURE >>  8) & 0xFF;
    SignatureData[4 +  3] = (PATCH_SIGNATURE >>  0) & 0xFF;

    SignatureData[4 +  4] = (ImageUpdate_Size >> 24) & 0xFF;
    SignatureData[4 +  5] = (ImageUpdate_Size >> 16) & 0xFF;
    SignatureData[4 +  6] = (ImageUpdate_Size >>  8) & 0xFF;
    SignatureData[4 +  7] = (ImageUpdate_Size >>  0) & 0xFF;

    SignatureData[4 +  8] = (ImageUpdate_Checksum >> 24) & 0xFF;
    SignatureData[4 +  9] = (ImageUpdate_Checksum >> 16) & 0xFF;
    SignatureData[4 + 10] = (ImageUpdate_Checksum >>  8) & 0xFF;
    SignatureData[4 + 11] = (ImageUpdate_Checksum >>  0) & 0xFF;
    SendSetParamMsg(PARAM_FLASH_WRITE, SignatureData, sizeof(SignatureData));

    SYS_CONSOLE_PRINT("Size=0x%x, Checksum=%x\r\n", (unsigned int)ImageUpdate_Size, (unsigned int)ImageUpdate_Checksum);
    
    buf_command[0] = (uint8_t)(UPDATE_CMD_CPY_CALIBRATION); 
    buf_command[1] = (uint8_t)(UPDATE_SECURITY_KEY >> 16); 
    buf_command[2] = (uint8_t)(UPDATE_SECURITY_KEY >>  8); 
    buf_command[3] = (uint8_t)(UPDATE_SECURITY_KEY);      
    SendSetParamMsg(PARAM_FLASH_update, buf_command, 4);   // Copy last 128 Bytes from Bank1 to Bank0
    SYS_CONSOLE_MESSAGE("Update Image Done!\r\n");
}

void AutoUpdate_Restore(void)
{
    uint8_t buf_command[4];
    
    buf_command[0] = UPDATE_CMD_ERASE0; //Erase bank0
    buf_command[1] = UPDATE_SECURITY_KEY_1; 
    buf_command[2] = UPDATE_SECURITY_KEY_2; 
    buf_command[3] = UPDATE_SECURITY_KEY_3;
    SendSetParamMsg(PARAM_FLASH_update, buf_command, 4);
    
    buf_command[0] = UPDATE_CMD_CPY_1TO0; //Copy bank1 to bank0
    buf_command[1] = UPDATE_SECURITY_KEY_1; 
    buf_command[2] = UPDATE_SECURITY_KEY_2; 
    buf_command[3] = UPDATE_SECURITY_KEY_3;
    SendSetParamMsg(PARAM_FLASH_update, buf_command, 4);
}

#endif /* defined(WF_UPDATE_FIRMWARE_XMODEM) && defined(WF_UPDATE_FIRMWARE_OTA) */

#endif // TCPIP_IF_MRF24W
