/*******************************************************************************
  MRF24W Config Data

  File Name:
    drv_wifi_config_data.c

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
#include "tcpip/src/tcpip_private.h"

#if defined(TCPIP_IF_MRF24W)
#define WF_NVM_SPACE_VERIFY_VALUE 0x5a5a5a5a

DRV_WIFI_CONFIG_DATA wifi_ConfigData;
DRV_WIFI_CONFIG_DATA *p_wifi_ConfigData = &wifi_ConfigData;


#if defined(DRV_WIFI_NVM_SPACE_ENABLE)
uint8_t __attribute__((coherent)) NVM_DATA_BUFF_local[DRV_NVM_ROW_SIZE];

SYS_FS_MEDIA_GEOMETRY *gAppNVMMediaGeometry;

bool DRV_WIFI_NVM_write(uint8_t *buf, int size)
{
    int i, tmp;
    DRV_HANDLE nvmHandle;
    DRV_NVM_COMMAND_HANDLE nvmCommandHandle;
    uint8_t NVM_DATA_READ_BUFF_local[DRV_NVM_ROW_SIZE];

    if (size > DRV_NVM_ROW_SIZE) {
        return false;
    }

    for (i = 0; i < size; i++)
        NVM_DATA_BUFF_local[i] = buf[i];

    nvmHandle = DRV_NVM_Open(0, DRV_IO_INTENT_READWRITE);
    if (DRV_HANDLE_INVALID == nvmHandle) {
        return false;
    }

    gAppNVMMediaGeometry = DRV_NVM_GeometryGet(nvmHandle);
    if (NULL == gAppNVMMediaGeometry) {
        return false;
    }

    DRV_NVM_Read (nvmHandle, &nvmCommandHandle, NVM_DATA_READ_BUFF_local, DRV_WIFI_NVM_SPACE_ADDR, DRV_NVM_ROW_SIZE);
    if (DRV_NVM_COMMAND_HANDLE_INVALID == nvmCommandHandle) {
        return false;;
    }

    tmp = DRV_WIFI_NVM_SPACE_ADDR * (gAppNVMMediaGeometry->geometryTable[2].numBlocks) / (DRV_NVM_MEDIA_SIZE * 1024);
    DRV_NVM_Erase(nvmHandle, &nvmCommandHandle,  tmp, 1);
    if (nvmCommandHandle == DRV_NVM_COMMAND_HANDLE_INVALID) {
        return false;;
    }

    while (DRV_NVM_COMMAND_COMPLETED != DRV_NVM_CommandStatus(nvmHandle, nvmCommandHandle))
        ;

    tmp = DRV_WIFI_NVM_SPACE_ADDR * (gAppNVMMediaGeometry->geometryTable[1].numBlocks) / (DRV_NVM_MEDIA_SIZE * 1024);
    DRV_NVM_Write(nvmHandle, &nvmCommandHandle, (uint8_t *)NVM_DATA_BUFF_local, tmp, 1);
    if (DRV_NVM_COMMAND_HANDLE_INVALID == nvmCommandHandle) {
        return false;;
    }

    while (DRV_NVM_COMMAND_COMPLETED != DRV_NVM_CommandStatus(nvmHandle, nvmCommandHandle))
        ;

    DRV_NVM_Close(nvmHandle);

    return true;
}

bool DRV_WIFI_NVM_read(uint8_t *buf, int size)
{
    DRV_HANDLE                  nvmHandle;
    DRV_NVM_COMMAND_HANDLE      nvmCommandHandle;

    if (size > DRV_NVM_PAGE_SIZE) {
        return false;
    }

    nvmHandle = DRV_NVM_Open(0, DRV_IO_INTENT_READWRITE);
    if (DRV_HANDLE_INVALID == nvmHandle) {
        return false;
    }

    gAppNVMMediaGeometry = DRV_NVM_GeometryGet(nvmHandle);
    if (NULL == gAppNVMMediaGeometry) {
        return false;
    }

    DRV_NVM_Read(nvmHandle, &nvmCommandHandle, buf, DRV_WIFI_NVM_SPACE_ADDR, size);
    if (DRV_NVM_COMMAND_HANDLE_INVALID == nvmCommandHandle) {
        return false;
    }

    while (DRV_NVM_COMMAND_COMPLETED != DRV_NVM_CommandStatus(nvmHandle, nvmCommandHandle))
        ;

    DRV_NVM_Close(nvmHandle);

    return true;
}

bool DRV_WIFI_NVM_erase(void)
{
    int tmp;
    DRV_HANDLE nvmHandle;
    DRV_NVM_COMMAND_HANDLE nvmCommandHandle;
    uint8_t NVM_DATA_READ_BUFF_local[DRV_NVM_ROW_SIZE];

    nvmHandle = DRV_NVM_Open(0, DRV_IO_INTENT_READWRITE);
    if (DRV_HANDLE_INVALID == nvmHandle) {
        return false;
    }

    gAppNVMMediaGeometry = DRV_NVM_GeometryGet(nvmHandle);
    if (NULL == gAppNVMMediaGeometry) {
        return false;
    }

    DRV_NVM_Read(nvmHandle, &nvmCommandHandle, NVM_DATA_READ_BUFF_local, DRV_WIFI_NVM_SPACE_ADDR, DRV_NVM_ROW_SIZE);
    if (DRV_NVM_COMMAND_HANDLE_INVALID == nvmCommandHandle) {
        return false;;
    }

    tmp = DRV_WIFI_NVM_SPACE_ADDR * (gAppNVMMediaGeometry->geometryTable[2].numBlocks) / (DRV_NVM_MEDIA_SIZE * 1024);
    DRV_NVM_Erase(nvmHandle, &nvmCommandHandle,  tmp, 1);
    if (nvmCommandHandle == DRV_NVM_COMMAND_HANDLE_INVALID) {
        return false;;
    }

    while (DRV_NVM_COMMAND_COMPLETED != DRV_NVM_CommandStatus(nvmHandle, nvmCommandHandle))
        ;

    DRV_NVM_Close(nvmHandle);

    return true;
}

void WiFi_WriteConfigToMemory(void)
{
    p_wifi_ConfigData->verify_flag = WF_NVM_SPACE_VERIFY_VALUE;
    DRV_WIFI_NVM_write((uint8_t *)p_wifi_ConfigData, sizeof(DRV_WIFI_CONFIG_DATA));
}
void WiFi_ReadConfigFromMemory(void)
{
    DRV_WIFI_NVM_read((uint8_t *)p_wifi_ConfigData, sizeof(DRV_WIFI_CONFIG_DATA));
}
void WiFi_EraseConfigFromMemory(void)
{
    DRV_WIFI_NVM_erase();
}

#else // DRV_WIFI_NVM_SPACE_ENABLE

DRV_WIFI_CONFIG_DATA SaveWiFiConf_in_memory={0};

bool DRV_WIFI_NVM_write(uint8_t *buf, int size) {return true;}
bool DRV_WIFI_NVM_read(uint8_t *buf, int size)  {return true;}
bool DRV_WIFI_NVM_erase(void)                   {return true;}

void WiFi_WriteConfigToMemory(void)
{
    int i;
    uint8_t *p_data1, *p_data2;

    p_wifi_ConfigData->verify_flag = WF_NVM_SPACE_VERIFY_VALUE;
    p_data1 = (void *)p_wifi_ConfigData;
    p_data2 = (void *)&SaveWiFiConf_in_memory;
    for (i = 0; i < sizeof(DRV_WIFI_CONFIG_DATA); i++)
    {
        p_data2[i] = p_data1[i];
    }
}

void WiFi_ReadConfigFromMemory(void)
{
    int i;
    uint8_t *p_data1, *p_data2;

    p_data1 = (void *)&SaveWiFiConf_in_memory;
    p_data2 = (void *)p_wifi_ConfigData;
    for (i = 0; i < sizeof(DRV_WIFI_CONFIG_DATA); i++)
    {
        p_data2[i] = p_data1[i];
    }
}

void WiFi_EraseConfigFromMemory(void)
{
    memset(&SaveWiFiConf_in_memory, 0, sizeof(SaveWiFiConf_in_memory));
}

#endif // DRV_WIFI_NVM_SPACE_ENABLE

bool WF_NVM_Verify(void)
{
    uint32_t Space_verified = 0x00;

    DRV_WIFI_NVM_read((uint8_t *)&Space_verified, sizeof(Space_verified));
    
    return (Space_verified == WF_NVM_SPACE_VERIFY_VALUE);
}

#if defined(TCPIP_STACK_COMMANDS_WIFI_ENABLE)
/*******************************************************************************
  Function:
    void DRV_WIFI_ConfigDataPrint(void);

  Summary:
    Outputs to console the configuration data from the board EEPROM.

  Description:
    This function outputs configuration data from the board EEPROM.

  Precondition:
    TCPIP stack should be initialized.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 *******************************************************************************/
void DRV_WIFI_ConfigDataPrint(void)
{
    int i;
    uint8_t tmpNetSSID[DRV_WIFI_MAX_SSID_LENGTH + 1] = {0};
    DRV_WIFI_CONFIG_DATA tmp_wifi_ConfigData;

    DRV_WIFI_NVM_read((uint8_t *)&tmp_wifi_ConfigData, sizeof(DRV_WIFI_CONFIG_DATA));

    if (WF_NVM_SPACE_VERIFY_VALUE != tmp_wifi_ConfigData.verify_flag) {
        SYS_CONSOLE_MESSAGE("!!Cannot read data from storage space, because it is not initialized\r\n");
        return;
    }

    SYS_CONSOLE_PRINT("\r\nSize of Configdata:  %lu\r\n", sizeof(tmp_wifi_ConfigData));

    for (i = 0; i < tmp_wifi_ConfigData.SsidLength; i++) {
        tmpNetSSID[i] = tmp_wifi_ConfigData.netSSID[i];
    }

    SYS_CONSOLE_PRINT("networkType:         %d\r\n", tmp_wifi_ConfigData.networkType);
    SYS_CONSOLE_PRINT("netSSID:             %s\r\n", tmpNetSSID);
    SYS_CONSOLE_PRINT("SsidLength:          %d\r\n", tmp_wifi_ConfigData.SsidLength);
    SYS_CONSOLE_PRINT("SecurityMode:        %d\r\n", tmp_wifi_ConfigData.SecurityMode);
    SYS_CONSOLE_MESSAGE("SecurityKey:         ");
    for (i = 0; i < tmp_wifi_ConfigData.SecurityKeyLength; i++) {
        SYS_CONSOLE_PRINT("%.2x", tmp_wifi_ConfigData.SecurityKey[i]);
    }

    SYS_CONSOLE_MESSAGE("\r\n");
    SYS_CONSOLE_PRINT("SecurityKeyLength:   %d\r\n", tmp_wifi_ConfigData.SecurityKeyLength);
}
#endif /* TCPIP_STACK_COMMANDS_WIFI_ENABLE */

/*******************************************************************************
  Function:
    bool DRV_WIFI_ConfigDataErase(void);

  Summary:
    Erases configuration data from the board EEPROM.

  Description:
    This function erases configuration data from the board EEPROM.

 Parameters:
    None

  Returns:
    None
 *******************************************************************************/
bool DRV_WIFI_ConfigDataErase(void)
{
    WiFi_EraseConfigFromMemory();

    return true;
}

#if defined(WF_NVM_SPACE_TESTING)
uint8_t test_buf_read[250];
uint8_t test_buf_write[250];

void WiFi_config_print(int flag, DRV_WIFI_CONFIG_DATA *tmp_Config)
{
    int i;

    SYS_CONSOLE_PRINT("%d, verify_flag=%x\r\n", flag, tmp_Config->verify_flag);
    SYS_CONSOLE_PRINT("networkType=%x\r\n", tmp_Config->networkType);
    for (i = 0; i < DRV_WIFI_MAX_SSID_LENGTH; i++)
        SYS_CONSOLE_PRINT("%c ", tmp_Config->netSSID[i]);

    SYS_CONSOLE_PRINT("\r\nSsidLength=%x\r\n", tmp_Config->SsidLength);
    SYS_CONSOLE_PRINT("SecurityMode=%x\r\n",   tmp_Config->SecurityMode);
    SYS_CONSOLE_PRINT("flag_WpsSaving=%x\r\n", tmp_Config->flag_WpsSaving);

    for (i = 0; i < 30; i++)
        SYS_CONSOLE_Tasks(sysObj.sysConsole0);
}

int CommandIw_config_print(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    DRV_WIFI_CONFIG_DATA tmp_wifi_ConfigData;

    DRV_WIFI_NVM_read((uint8_t *)&tmp_wifi_ConfigData, sizeof(DRV_WIFI_CONFIG_DATA));
    WiFi_config_print(1, &tmp_wifi_ConfigData);
    
    return true;
}

void wf_test_nvm_print(DRV_WIFI_CONFIG_DATA *wifi_ConfigData_print)
{
    SYS_CONSOLE_PRINT("SsidLength        = %d\r\n", wifi_ConfigData_print->SsidLength);
    SYS_CONSOLE_PRINT("SecurityMode      = %d\r\n", wifi_ConfigData_print->SecurityMode);
    SYS_CONSOLE_PRINT("defaultWepKey     = %d\r\n", wifi_ConfigData_print->defaultWepKey);
    SYS_CONSOLE_PRINT("SecurityKeyLength = %d\r\n", wifi_ConfigData_print->SecurityKeyLength);
    SYS_CONSOLE_PRINT("wepKeyType        = %d\r\n", wifi_ConfigData_print->wepKeyType);
    SYS_CONSOLE_PRINT("dataValid         = %d\r\n", wifi_ConfigData_print->dataValid);
}

int Command_WiFi_NVM_erase(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    DRV_WIFI_NVM_erase();
    
    return true;
}



int Command_test_NVM_write_a(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int i;

    for (i = 0; i < 250; i++)
        test_buf_write[i] = i;

    DRV_WIFI_NVM_write(test_buf_write, 250);

    return true;
}

int Command_test_NVM_write_b(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int i;

    for (i = 0; i < 250; i++)
        test_buf_write[i] = 255 - i;

    DRV_WIFI_NVM_write(test_buf_write, 250);

    return true;
}

DRV_WIFI_CONFIG_DATA wifi_ConfigData_test;

int Command_test_NVM_write_c(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int i;

    uint8_t *pBuf = (uint8_t *)&wifi_ConfigData_test;
    for (i = 0; i < sizeof(wifi_ConfigData_test); i++)
        pBuf[i] = i;

    DRV_WIFI_NVM_write((uint8_t *)&wifi_ConfigData_test, sizeof(wifi_ConfigData_test));

    return true;
}

int Command_test_NVM_write_d(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int i;

    uint8_t *pBuf = (uint8_t *)&wifi_ConfigData_test;
    for (i = 0; i < sizeof(wifi_ConfigData_test); i++)
        pBuf[i] = 255-i;

    DRV_WIFI_NVM_write((uint8_t *)&wifi_ConfigData_test, sizeof(wifi_ConfigData_test));

    return true;
}

int Command_test_NVM_read(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int i;
 
    DRV_WIFI_NVM_read(test_buf_read, 250);
    
    for (i = 0; i < 100; i++)
        SYS_CONSOLE_PRINT("%02x ", test_buf_read[i]);

    if (strncmp(test_buf_read, test_buf_write,250) != 0)
    {
        SYS_CONSOLE_MESSAGE("Error\r\n");
    }
    else
    {
        SYS_CONSOLE_MESSAGE("Success\r\n");
    }

    return true;
}

#endif

#endif // TCPIP_IF_MRF24W

//DOM-IGNORE-END
