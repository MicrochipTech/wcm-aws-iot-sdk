/*******************************************************************************
  SPI Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_static.h

  Summary:
    SPI driver interface declarations for the static single instance driver.

  Description:
    The SPI device driver provides a simple interface to manage the SPI
    modules on Microchip microcontrollers. This file defines the interface
    Declarations for the SPI driver.
    
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

#ifndef _DRV_SPI_STATIC_H
#define _DRV_SPI_STATIC_H

// *****************************************************************************
// *****************************************************************************
// Header Includes
// *****************************************************************************
// *****************************************************************************
#include "peripheral/spi/plib_spi.h"
 

<#macro DRV_SPI_STATIC_API DRV_INSTANCE>
// *****************************************************************************
// *****************************************************************************
// Section: Interface Headers for Instance ${DRV_INSTANCE} for the static driver
// *****************************************************************************
// *****************************************************************************
void DRV_SPI${DRV_INSTANCE}_Initialize(void);
void DRV_SPI${DRV_INSTANCE}_DeInitialize(void);
bool DRV_SPI${DRV_INSTANCE}_ReceiverBufferIsFull(void);
bool DRV_SPI${DRV_INSTANCE}_TransmitterBufferIsFull(void);
int32_t DRV_SPI${DRV_INSTANCE}_BufferAddWriteRead(const void * txBuffer, void * rxBuffer, uint32_t size);
</#macro>

<#if CONFIG_DRV_SPI_STATIC_IDX0 == true>
<@DRV_SPI_STATIC_API DRV_INSTANCE="0"/>
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX1 == true>
<@DRV_SPI_STATIC_API DRV_INSTANCE="1"/>
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX2 == true>
<@DRV_SPI_STATIC_API DRV_INSTANCE="2"/>
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX3 == true>
<@DRV_SPI_STATIC_API DRV_INSTANCE="3"/>
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX4 == true>
<@DRV_SPI_STATIC_API DRV_INSTANCE="4"/>
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX5 == true>
<@DRV_SPI_STATIC_API DRV_INSTANCE="5"/>
</#if>
#endif // #ifndef _DRV_SPI_STATIC_H

/*******************************************************************************
 End of File
*/
