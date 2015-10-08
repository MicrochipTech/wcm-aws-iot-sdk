/*******************************************************************************
  MRF24W GPIO Driver

  File Name: 
    wdrv_mrf24wn_gpio.c  
  
  Summary:
    Module for Microchip TCP/IP Stack
    
  Description:
    Supports SPI communications to the MRF24W module
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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <sys/attribs.h>

#include "driver/spi/drv_spi.h"
#include "system/clk/sys_clk.h"
#include "system/clk/src/sys_clk_local.h"
#include "system/int/sys_int.h"
#include "peripheral/dma/plib_dma.h"

#include "wdrv_mrf24wn_common.h"

#define WF_TEST_PORT_CHANNEL PORT_CHANNEL_E
#define WF_TEST_BIT_POS      0

void WDRV_GPIO_Init(int board)      
{
    switch (board) {
    case MZ_ESK_BD:
        break;
    case MX_ESK_BD:
        break;
    case EXP16_BD:
        break;
    case MEB2_BD:
        break;
    default:
        WDRV_ASSERT(false, "Unsupported board");
        break;
    }
}

void WDRV_GPIO_OutLow(int board)        
{ 
    switch (board) {
    case MZ_ESK_BD:
        PLIB_PORTS_PinModePerPortSelect(PORTS_ID_0, WF_TEST_PORT_CHANNEL,WF_TEST_BIT_POS,PORTS_PIN_MODE_DIGITAL);
        PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0,WF_TEST_PORT_CHANNEL,WF_TEST_BIT_POS);
        SYS_PORTS_PinClear  (PORTS_ID_0, WF_TEST_PORT_CHANNEL,WF_TEST_BIT_POS);
        break;
    case MX_ESK_BD:
        break;
    case EXP16_BD:
        break;
    case MEB2_BD:
        break;
    default:
        WDRV_ASSERT(false, "Unsupported board");
        break;
    }
    
}

void WDRV_GPIO_OutHigh(int board)      
{
    switch (board) {
    case MZ_ESK_BD:
        PLIB_PORTS_PinModePerPortSelect(PORTS_ID_0, WF_TEST_PORT_CHANNEL,WF_TEST_BIT_POS,PORTS_PIN_MODE_DIGITAL);
        PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0,WF_TEST_PORT_CHANNEL,WF_TEST_BIT_POS);
        SYS_PORTS_PinSet    (PORTS_ID_0, WF_TEST_PORT_CHANNEL,WF_TEST_BIT_POS);
        break;
    case MX_ESK_BD:
        break;
    case EXP16_BD:
        break;
    case MEB2_BD:
        break;
    default:
        WDRV_ASSERT(false, "Unsupported board");
        break;
    }
}

//DOM-IGNORE-END
