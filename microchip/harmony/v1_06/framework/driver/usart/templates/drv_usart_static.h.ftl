/*******************************************************************************
  USART Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart_static.h

  Summary:
    USART driver interface declarations for the static single instance driver.

  Description:
    The USART device driver provides a simple interface to manage the USART
    modules on Microchip microcontrollers. This file defines the interface
    Declarations for the USART driver.
    
  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.
    
    Static single-open interfaces also eliminate the need for the open handle.
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

#ifndef _DRV_USART_STATIC_H
#define _DRV_USART_STATIC_H
<#macro DRV_USART_STATIC_API DRV_INSTANCE TX_ENABLE RX_ENABLE>
// *****************************************************************************
// *****************************************************************************
// Section: Interface Headers for Instance ${DRV_INSTANCE} for the static driver
// *****************************************************************************
// *****************************************************************************

void DRV_USART${DRV_INSTANCE}_Initialize(void);
<#if RX_ENABLE == true>
uint8_t DRV_USART${DRV_INSTANCE}_ReadByte(void);
bool DRV_USART${DRV_INSTANCE}_ReceiverBufferIsEmpty(void);
</#if>
<#if TX_ENABLE == true>
void DRV_USART${DRV_INSTANCE}_WriteByte(const uint8_t byte);
</#if>

</#macro>

<#if CONFIG_DRV_USART_INST_IDX0 == true>
<@DRV_USART_STATIC_API
DRV_INSTANCE="0"
TX_ENABLE=CONFIG_DRV_USART_STATIC_TX_ENABLE_IDX0
RX_ENABLE=CONFIG_DRV_USART_STATIC_RX_ENABLE_IDX0
/>
</#if>
<#if CONFIG_DRV_USART_INST_IDX1 == true>
<@DRV_USART_STATIC_API
DRV_INSTANCE="1"
TX_ENABLE=CONFIG_DRV_USART_STATIC_TX_ENABLE_IDX1
RX_ENABLE=CONFIG_DRV_USART_STATIC_RX_ENABLE_IDX1
/>
</#if>
<#if CONFIG_DRV_USART_INST_IDX2 == true>
<@DRV_USART_STATIC_API
DRV_INSTANCE="2"
TX_ENABLE=CONFIG_DRV_USART_STATIC_TX_ENABLE_IDX2
RX_ENABLE=CONFIG_DRV_USART_STATIC_RX_ENABLE_IDX2
/>
</#if>
<#if CONFIG_DRV_USART_INST_IDX3 == true>
<@DRV_USART_STATIC_API
DRV_INSTANCE="3"
TX_ENABLE=CONFIG_DRV_USART_STATIC_TX_ENABLE_IDX3
RX_ENABLE=CONFIG_DRV_USART_STATIC_RX_ENABLE_IDX3
/>
</#if>
<#if CONFIG_DRV_USART_INST_IDX4 == true>
<@DRV_USART_STATIC_API
DRV_INSTANCE="4"
TX_ENABLE=CONFIG_DRV_USART_STATIC_TX_ENABLE_IDX4
RX_ENABLE=CONFIG_DRV_USART_STATIC_RX_ENABLE_IDX4
/>
</#if>
<#if CONFIG_DRV_USART_INST_IDX5 == true>
<@DRV_USART_STATIC_API
DRV_INSTANCE="5"
TX_ENABLE=CONFIG_DRV_USART_STATIC_TX_ENABLE_IDX5
RX_ENABLE=CONFIG_DRV_USART_STATIC_RX_ENABLE_IDX5
/>
</#if>
#endif // #ifndef _DRV_USART_STATIC_H

/*******************************************************************************
 End of File
*/
