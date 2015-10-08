/*******************************************************************************
  DMA System Service Interface Declarations for Static Single Instance System Service

  Company:
    Microchip Technology Inc.

  File Name:
    drv_dma_static.h

  Summary:
    DMA driver interface declarations for the static single instance System Service.

  Description:
    The DMA System Service provides a simple interface to manage the DMA
    modules on Microchip microcontrollers. This file defines the interface
    Declarations for the DMA.
    
  Remarks:
    Static interfaces incorporate the System Service instance number within the names
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


#ifndef _DRV_DMA_STATIC_H
#define _DRV_DMA_STATIC_H

/* This is a temporary workaround for an issue with the peripheral library "Exists"
   functions that causes superfluous warnings.  It "nulls" out the definition of
   The PLIB function attribute that causes the warning.  Once that issue has been
   resolved, this definition should be removed. */
#define _PLIB_UNSUPPORTED

#include "peripheral/dma/plib_dma.h"
#include "peripheral/int/plib_int.h"

// *****************************************************************************
/* DMA Channel ignore pattern match

  Summary:
    Enable/Disable the feature to ignore a byte between a 2-byte match abort
    pattern.

  Description:
    This data type allows enabling/disabling the feature to ignore a byte
    between a 2-byte match abort pattern.

  Remarks:
    This feature may not be available on all micro-controllers.
*/
typedef enum
{
    /* Disabling the feature to ignore a byte between a 2-byte match abort pattern. */
    SYS_DMA_CHANNEL_IGNORE_MATCH_DISABLE,

    /* Enable the feature to ignore a byte between a 2-byte match abort pattern. */
    SYS_DMA_CHANNEL_IGNORE_MATCH_ENABLE

} SYS_DMA_CHANNEL_IGNORE_MATCH;

// *****************************************************************************
/* DMA channel CRC mode

  Summary:
    Identifies the operation mode of the CRC module

  Description:
    This data type identifies CRC operating mode. The CRC computation can be
    in background mode or append mode.

  Remarks:
    None.
*/
typedef enum
{
    /* DMA reads the data from the source, passes it through the CRC module and
       writes it to the destination. the calculated CRC is left in the DCRCDATA
       register at the end of the block transfer. */
    SYS_DMA_CHANNEL_CRC_MODE_BACKGROUND,

    /* DMA only feeds source data to the CRC module. it does not write source data
       to the destination address. When a block transfer complete or pattern abort
       event occur, The DMA writes the CRC value to the destination address */
    SYS_DMA_CHANNEL_CRC_MODE_APPEND

} SYS_DMA_CHANNEL_CRC_MODE;

// *****************************************************************************
/* DMA channel CRC write order

  Summary:
    Identifies altering/maintaining write order post CRC computation

  Description:
    This data type identifies write byte order selection post CRC
    computation.

  Remarks:
    None.
*/
typedef enum
{
    /* Source data is written to the destination unchanged regardless
       of the selected pre-CRC byte order. */
    SYS_DMA_CRC_WRITE_ORDER_MAINTAIN,

    /* Source data is written to the destination re-ordered according
       to the selected pre-CRC byte order. */
    SYS_DMA_CRC_WRITE_ORDER_CHANGE

} SYS_DMA_CRC_WRITE_ORDER;

// *****************************************************************************
/* DMA channel operation modes

  Summary:
    Identifies the available DMA operating modes

  Description:
    This data type Identifies the available DMA operating modes.

    The supported DMA modes are not mutually exclusive, but can be simultaneously
    operational.

  Remarks:
    These supported DMA modes can be logically OR'd together. They are passed
    into the SYS_DMA_ChannelSetup function through the modeEnable parameter.
*/
typedef enum
{
    /* The normal DMA operation mode. The DMA channel will transfer data from
       a source to a destination without CPU intervention */
    SYS_DMA_CHANNEL_OP_MODE_BASIC       /*DOM-IGNORE-BEGIN*/ = (1<<0)  /*DOM-IGNORE-END*/,

    /* Pattern Match abort mode allows the user to end a transfer if data
       written during a transaction matches a specific pattern, as defined by
       the DCHxDAT register */
    SYS_DMA_CHANNEL_OP_MODE_MATCH_ABORT /*DOM-IGNORE-BEGIN*/ = (1<<1)  /*DOM-IGNORE-END*/,

    /* Channel chaining operating mode enhances the normal DMA channel
       operations. A channel(slave) can be chained to an adjacent channel(master).
       The slave channel will be enabled when a block transfer of the master
       channel completes. */
    SYS_DMA_CHANNEL_OP_MODE_CHAIN_HIGHER       /*DOM-IGNORE-BEGIN*/ = (1<<2)  /*DOM-IGNORE-END*/,
    SYS_DMA_CHANNEL_OP_MODE_CHAIN_LOWER       /*DOM-IGNORE-BEGIN*/ = (1<<3)  /*DOM-IGNORE-END*/,
    /* Auto enable operating mode allows a channel to be kept active, even if a
       block transfer completes or pattern match occurs. This prevents the user
       from having to re-enable the channel each time a block transfer completes. */
    SYS_DMA_CHANNEL_OP_MODE_AUTO        /*DOM-IGNORE-BEGIN*/ = (1<<4)  /*DOM-IGNORE-END*/,

    /* CRC operating mode allows computation of 16, 32-bit CRC. */
    SYS_DMA_CHANNEL_OP_MODE_CRC         /*DOM-IGNORE-BEGIN*/ = (1<<5)  /*DOM-IGNORE-END*/

} SYS_DMA_CHANNEL_OP_MODE;

// *****************************************************************************
/* DMA channel CRC mode

  Summary:
    Defines the attributes for CRC operation mode

  Description:
    This data type defines the attributes for CRC operation mode.

  Remarks:
    Not all features are available on all micro-controllers.
*/
typedef struct
{
    /* CRC type (PLIB-level).
       Type Selection identifies whether the CRC module will calculate an IP
       header checksum or an LFSR CRC */
    DMA_CRC_TYPE type;

    /* CRC mode, Background or Append */
    SYS_DMA_CHANNEL_CRC_MODE mode;

    /* Polynomial length, This value is redundant when the selected CRC type is
       IP header checksum. When the CRC type is LFSR, Identifies the length of
       the polynomial. */
    uint8_t polyLength;

    /* Bit order (PLIB-level).
       Identifies whether the CRC is computed LSb or MSb first */
    DMA_CRC_BIT_ORDER bitOrder;

    /* Byte order (PLIB-level).
       Identifies the byte selection order input pre-CRC Generator. */
    DMA_CRC_BYTE_ORDER byteOrder;

    /* Post CRC Write order */
    SYS_DMA_CRC_WRITE_ORDER writeOrder;

    /* CRC data feed: While enabling the CRC mode, this field gives
       the seed for the CRC computation. On block transfer complete or
       pattern match abort the field have the computed CRC. */
    uint32_t data;

    /* XOR bit mask, This value is redundant when the selected CRC type is
       IP header checksum. When the CRC type is LFSR, Identifies the XOR
       bit mask input to the shift register during CRC computation. */
    uint32_t xorBitMask;

} SYS_DMA_CHANNEL_OPERATION_MODE_CRC;

<#macro DRV_DMA_STATIC_FUNCTIONS INSTANCE>	
void SYS_DMA_Channel${INSTANCE}TransferAdd(void);

void SYS_DMA_Channel${INSTANCE}Enable(void);

void SYS_DMA_Channel${INSTANCE}Setup(SYS_DMA_CHANNEL_OP_MODE modeEnable);

void SYS_DMA_Channel${INSTANCE}ForceStart(void);


bool SYS_DMA_Channel${INSTANCE}IsBusy(void);
void SYS_DMA_Channel${INSTANCE}ForceAbort(void);
void SYS_DMA_Channel${INSTANCE}ForceStart(void);
void SYS_DMA_Channel${INSTANCE}Disable(void);
void SYS_DMA_Suspend(void);
void SYS_DMA_Resume(void);
bool SYS_DMA_IsBusy(void);
uint32_t SYS_DMA_Channel${INSTANCE}GetCRC(void);
void SYS_DMA_Channel${INSTANCE}SetCRC(SYS_DMA_CHANNEL_OPERATION_MODE_CRC crc);
void SYS_DMA_Channel${INSTANCE}SetupMatchAbortMode( uint16_t pattern,
                                         DMA_PATTERN_LENGTH length,
                                         SYS_DMA_CHANNEL_IGNORE_MATCH ignore,
                                         uint8_t ignorePattern );
</#macro>

// *****************************************************************************
// *****************************************************************************
// Section: Interface Headers for Instance for the static System Service
// *****************************************************************************
// *****************************************************************************

void SYS_DMA_Initialize(void);

<#if CONFIG_SYS_DMA_CHANNEL_0 == true>
<@DRV_DMA_STATIC_FUNCTIONS INSTANCE=0/>
</#if>
<#if CONFIG_SYS_DMA_CHANNEL_1 == true>
<@DRV_DMA_STATIC_FUNCTIONS INSTANCE=1/>
</#if>
<#if CONFIG_SYS_DMA_CHANNEL_2 == true>
<@DRV_DMA_STATIC_FUNCTIONS INSTANCE=2/>
</#if>
<#if CONFIG_SYS_DMA_CHANNEL_3 == true>
<@DRV_DMA_STATIC_FUNCTIONS INSTANCE=3/>
</#if>
<#if CONFIG_SYS_DMA_CHANNEL_4 == true>
<@DRV_DMA_STATIC_FUNCTIONS INSTANCE=4/>
</#if>
<#if CONFIG_SYS_DMA_CHANNEL_5 == true>
<@DRV_DMA_STATIC_FUNCTIONS INSTANCE=5/>
</#if>
<#if CONFIG_SYS_DMA_CHANNEL_6 == true>
<@DRV_DMA_STATIC_FUNCTIONS INSTANCE=6/>
</#if>
<#if CONFIG_SYS_DMA_CHANNEL_7 == true>
<@DRV_DMA_STATIC_FUNCTIONS INSTANCE=7/>
</#if>

#endif // #ifndef _DRV_DMA_STATIC_H

/*******************************************************************************
 End of File
*/
