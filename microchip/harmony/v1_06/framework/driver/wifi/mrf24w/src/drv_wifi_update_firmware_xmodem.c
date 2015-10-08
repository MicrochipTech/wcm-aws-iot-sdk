/*******************************************************************************
  MRF24W Driver Medium Access Control (MAC) Layer

  File Name:
    drv_wifi_upate_firmware_xmodem.c

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
bool g_xmodem_update_started = false;

#if defined(WF_UPDATE_FIRMWARE_XMODEM)

#define MAX_USER_RESPONSE_LEN   (20u)

#define XMODEM_SOH 0x01u
#define XMODEM_EOT 0x04u
#define XMODEM_ACK 0x06u
#define XMODEM_NAK 0x15u
#define XMODEM_CAN 0x18u
#define XMODEM_BLOCK_LEN 128u

typedef struct
{
    size_t bytesRead;
    bool rdComplete;
    uint8_t buf_recv[2];
    uint8_t buf_send[2];
} _WfUpdateConsoleDATA;

extern uint8_t AutoUpdate_Initialize_SM(void);
extern void AutoUpdate_Completed(void);
extern void AutoUpdate_Restore(void);

extern uint32_t ImageUpdate_Addr;
extern uint32_t ImageUpdate_Checksum;
extern uint32_t ImageUpdate_Size;

static _WfUpdateConsoleDATA WfUpdateConsoleData =
{
    .bytesRead = 0,
    .rdComplete = true
};

static void XMODEM_SendToModule_subAPI(uint8_t *buf)
{
    int i;
    uint8_t buf_module[36];

    buf_module[0] = (ImageUpdate_Addr & 0x00FF0000) >> 16;
    buf_module[1] = (ImageUpdate_Addr & 0x0000FF00) >>  8;
    buf_module[2] = (ImageUpdate_Addr & 0xFF);
    buf_module[3] = 32;

    for (i = 0; i < 32; i++) 
        buf_module[i + 4]= buf[i];

    SendSetParamMsg(PARAM_FLASH_WRITE, buf_module, 36);
    ImageUpdate_Addr += 32;
}

static void XMODEM_SendToModule(uint8_t *xmodm_buf)
{
    int i;

    //  1. Calculate checksum
    for (i = 0; i < 128; i++)
    {
        if ((ImageUpdate_Size % 4) == 0) 
            ImageUpdate_Checksum += (uint32_t)xmodm_buf[i] << 24;
        if ((ImageUpdate_Size % 4) == 1) 
            ImageUpdate_Checksum += (uint32_t)xmodm_buf[i] << 16;
        if ((ImageUpdate_Size % 4) == 2) 
            ImageUpdate_Checksum += (uint32_t)xmodm_buf[i] << 8;
        if ((ImageUpdate_Size % 4) == 3) 
            ImageUpdate_Checksum += (uint32_t)xmodm_buf[i];
        ImageUpdate_Size++;
    }
    
    // 2. send 128 bytes
    XMODEM_SendToModule_subAPI(&xmodm_buf[ 0]);
    XMODEM_SendToModule_subAPI(&xmodm_buf[32]);
    XMODEM_SendToModule_subAPI(&xmodm_buf[64]);
    XMODEM_SendToModule_subAPI(&xmodm_buf[96]);
}

static uint8_t tempData[XMODEM_BLOCK_LEN];

static void wf_update_uart_FeedBackground(void)
{
    SYS_TMR_Tasks(sysObj.sysTmr);
    DRV_TMR_Tasks(sysObj.drvTmr0);

    SYS_DEVCON_Tasks(sysObj.sysDevcon);
    SYS_CONSOLE_Tasks(sysObj.sysConsole0);
}

static void wf_update_console_ReadCallback (void *handle)
{
    size_t *readSize = handle;
    
    WfUpdateConsoleData.bytesRead = *readSize;
    WfUpdateConsoleData.rdComplete = true;
}
static void wf_update_console_init(void)
{
    SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, wf_update_console_ReadCallback, SYS_CONSOLE_EVENT_READ_COMPLETE);
}
static void wf_update_console_ReadByte_Kick(void)
{
    do {
        wf_update_uart_FeedBackground();
    } while (SYS_CONSOLE_Status(sysObj.sysConsole0) != SYS_STATUS_READY) ;
    
    WfUpdateConsoleData.rdComplete = false;
    SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, STDIN_FILENO, WfUpdateConsoleData.buf_recv, 1);
}
static bool wf_update_console_ReadByte_Wait(void)
{
    wf_update_uart_FeedBackground();
    return WfUpdateConsoleData.rdComplete;
}
static uint8_t wf_update_console_ReadByte_get(void)
{
    return WfUpdateConsoleData.buf_recv[0];
}
static void wf_update_console_SendByte(uint32_t c)
{
    WfUpdateConsoleData.buf_send[0] = c;
    wf_update_uart_FeedBackground();
    SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, STDOUT_FILENO, (const char*)WfUpdateConsoleData.buf_send, 1);
}
static void wf_update_console_flush(void)
{
    do {
        wf_update_uart_FeedBackground();
    } while (SYS_CONSOLE_Status(sysObj.sysConsole0) != SYS_STATUS_READY) ;
}
static bool wf_update_uart_WaitForConfirm(void)
{
    char c;

    SYS_CONSOLE_MESSAGE("Press 'y' to continue, any other character will cause abort\r\n, 'y'?\r\n");
    wf_update_console_ReadByte_Kick();

    while (wf_update_console_ReadByte_Wait() == false)
        ;

    c = wf_update_console_ReadByte_get();
    wf_update_console_SendByte(c);
    wf_update_console_flush();

    if (c == 'y') {
        SYS_CONSOLE_MESSAGE("\r\nYou said 'y', so continue...\r\n"); 
        return true;
    } else {
        SYS_CONSOLE_MESSAGE("\r\nYou didn't say 'y', so abort...\r\n"); 
        return false;
    }
}
bool WF_FirmwareUpdate_XMODEM(void)
{
    enum SM_FIRMWARE_UPDATE
    {
        SM_FIRMWARE_UPDATE_SOH,
        SM_FIRMWARE_UPDATE_BLOCK,
        SM_FIRMWARE_UPDATE_BLOCK_CMP,
        SM_FIRMWARE_UPDATE_DATA,
        SM_FIRMWARE_UPDATE_CHECKSUM,
        SM_FIRMWARE_UPDATE_FINISH,
    } state;

    uint8_t c;
    bool lbDone;
    uint8_t blockLen=0;
    bool lResult = false;
    uint8_t BlockNumber = 0;
    uint8_t preBlockNum = 0;
    uint8_t checksum    = 0;

    uint32_t lastTick;
    uint32_t currentTick;
    state = SM_FIRMWARE_UPDATE_SOH;
    lbDone = false;

    TCPIP_NET_HANDLE netH;

    netH = TCPIP_STACK_NetHandleGet("MRF24W");

    if (g_xmodem_update_started == false) {
        return false;
    }

    g_xmodem_update_started = false;

    wf_update_console_init();
    if (false == wf_update_uart_WaitForConfirm()) {
        return false;
    }

    SYS_CONSOLE_MESSAGE("Start to erase old firmware, please DO NOT shutdown system!\r\n");
    wf_update_console_flush();
    while (1 != AutoUpdate_Initialize_SM())
        ;

    SYS_CONSOLE_MESSAGE("I am ready, Please transfer firmware patch by XMODEM.\r\nAttention: THE PATCH FILE TYPE MUSE BE \".bin\".\r\n");

    wf_update_console_ReadByte_Kick();
    lastTick = SYS_TMR_TickCountGet();
    do
    {
        currentTick = SYS_TMR_TickCountGet();
        if ( currentTick - lastTick >= (SYS_TMR_TickCounterFrequencyGet() * 2) )
        {
            lastTick = SYS_TMR_TickCountGet();
            wf_update_console_SendByte(XMODEM_NAK);
        }

        wf_update_uart_FeedBackground();
    } while (!wf_update_console_ReadByte_Wait());

    while (!lbDone)
    {
        wf_update_uart_FeedBackground();
        if (wf_update_console_ReadByte_Wait()) 
        {
            c = wf_update_console_ReadByte_get();
            lastTick = SYS_TMR_TickCountGet();
        }
        else
        {
            // put some timeout to make sure  that we do not wait forever.
             currentTick = SYS_TMR_TickCountGet();
            if (currentTick - lastTick >= (SYS_TMR_TickCounterFrequencyGet() * 10))
            {
                //if time out, copy old patch image from bank2 to bank1
                SYS_CONSOLE_MESSAGE("Timeout, revert starts...\r\n");
                AutoUpdate_Restore();
                SYS_CONSOLE_MESSAGE("Revert done!\r\n");
                return false;
            }
 
            continue;
        }

        switch (state) {
        case SM_FIRMWARE_UPDATE_SOH:
            if (c == XMODEM_SOH)
            {
                state = SM_FIRMWARE_UPDATE_BLOCK;
                checksum = c;
                lResult = true;
            }
            else if ( c == XMODEM_EOT )
            {
                state = SM_FIRMWARE_UPDATE_FINISH;
                wf_update_console_SendByte(XMODEM_ACK);
                lbDone = true;
            }
            else
            {
                DRV_WIFI_ASSERT(false, "WiFI Update By Uart Error: data Error");
            }

            break;

        case SM_FIRMWARE_UPDATE_BLOCK:
            BlockNumber = c;
            checksum += c;
            state = SM_FIRMWARE_UPDATE_BLOCK_CMP;
            break;

        case SM_FIRMWARE_UPDATE_BLOCK_CMP:
            //Judge: Is it correct ?
            if (c != (BlockNumber ^ 0xFF))
            {
                lResult = false;
            }
            else
            {
                if ((uint8_t)(preBlockNum + 1) != BlockNumber)
                {
                    lResult = false;
                }
            }

            checksum += c;
            blockLen = 0;
            state = SM_FIRMWARE_UPDATE_DATA;
            break;

        case SM_FIRMWARE_UPDATE_DATA:
            // Buffer block data until it is over.
            tempData[blockLen++] = c;

            if ( blockLen == XMODEM_BLOCK_LEN )
            {
                state = SM_FIRMWARE_UPDATE_CHECKSUM;
            }

            checksum += c;
            break;

        case SM_FIRMWARE_UPDATE_CHECKSUM:

            if (checksum != c)
            {
                lResult = false;
            }

            XMODEM_SendToModule(tempData);

            if (lResult == true)
            {
                wf_update_console_SendByte(XMODEM_ACK);
                preBlockNum++;
            }
            else
            {
                wf_update_console_SendByte(XMODEM_NAK);
            }

            state = SM_FIRMWARE_UPDATE_SOH;
            break;

        default:
            DRV_WIFI_ASSERT(false, "WiFI Update By Uart Error: State Error");
            break;
        }

        wf_update_console_ReadByte_Kick();
    }

    AutoUpdate_Completed();
    return true;
}

#endif /* defined(WF_UPDATE_FIRMWARE_XMODEM) */

#endif // TCPIP_IF_MRF24W
