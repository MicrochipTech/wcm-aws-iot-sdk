/*******************************************************************************
  SPI Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_static.c

  Summary:
    SPI driver impementation for the static single instance driver.

  Description:
    The SPI device driver provides a simple interface to manage the SPI
    modules on Microchip microcontrollers. This file contains implemenation
    for the SPI driver.
    
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

// *****************************************************************************
// *****************************************************************************
// Header Includes
// *****************************************************************************
// *****************************************************************************
#include "system_definitions.h"
#include "framework/driver/spi/drv_spi_static.h"


<#macro DRV_SPI_STATIC_FUNCTIONS 
            DRV_INSTANCE 
            DRV_SPI_ID 
            DRV_SPI_CLOCK
            DRV_SPI_BAUDRATE
            DRV_SPI_MSTR 
            DRV_SPI_SII
            DRV_SPI_CM
            DRV_SPI_IPS
            DRV_SPI_CW
            DRV_SPI_PROTO
            DRV_SPI_FSPDS
            DRV_SPI_FSPPS
            DRV_SPI_FSPES
            DRV_SPI_FSPWS
            DRV_SPI_FSPCS
            DRV_SPI_ATMS
            DRV_SPI_APMS
            DRV_SPI_EBM
            >
// *****************************************************************************
// *****************************************************************************
// Section: Instance ${DRV_INSTANCE} static driver functions
// *****************************************************************************
// *****************************************************************************

void DRV_SPI${DRV_INSTANCE}_Initialize(void)
{
    PLIB_SPI_Disable(${DRV_SPI_ID});
    <#if DRV_SPI_MSTR=="DRV_SPI_MODE_MASTER">
    PLIB_SPI_MasterEnable(${DRV_SPI_ID});
    PLIB_SPI_SlaveSelectEnable(${DRV_SPI_ID});
    <#else>
    PLIB_SPI_SlaveEnable(${DRV_SPI_ID});
    PLIB_SPI_SlaveSelectDisable(${DRV_SPI_ID});
    </#if>
    <#if DRV_SPI_SII==true>
    PLIB_SPI_StopInIdleEnable(${DRV_SPI_ID});
    <#else>
    PLIB_SPI_StopInIdleDisable(${DRV_SPI_ID});
    </#if>
    <#if DRV_SPI_CM=="DRV_SPI_CLOCK_MODE_IDLE_LOW_EDGE_RISE">
    PLIB_SPI_ClockPolaritySelect(${DRV_SPI_ID}, SPI_CLOCK_POLARITY_IDLE_LOW);
    PLIB_SPI_OutputDataPhaseSelect(${DRV_SPI_ID}, SPI_OUTPUT_DATA_PHASE_ON_IDLE_TO_ACTIVE_CLOCK);
    <#elseif DRV_SPI_CM=="DRV_SPI_CLOCK_MODE_IDLE_LOW_EDGE_FALL">
    PLIB_SPI_ClockPolaritySelect(${DRV_SPI_ID}, SPI_CLOCK_POLARITY_IDLE_LOW);
    PLIB_SPI_OutputDataPhaseSelect(${DRV_SPI_ID}, SPI_OUTPUT_DATA_PHASE_ON_ACTIVE_TO_IDLE_CLOCK);
    <#elseif DRV_SPI_CM=="DRV_SPI_CLOCK_MODE_IDLE_HIGH_EDGE_FALL">
    PLIB_SPI_ClockPolaritySelect(${DRV_SPI_ID}, SPI_CLOCK_POLARITY_IDLE_HIGH);
    PLIB_SPI_OutputDataPhaseSelect(${DRV_SPI_ID}, SPI_OUTPUT_DATA_PHASE_ON_IDLE_TO_ACTIVE_CLOCK);
    <#elseif DRV_SPI_CM=="DRV_SPI_CLOCK_MODE_IDLE_HIGH_EDGE_RISE">
    PLIB_SPI_ClockPolaritySelect(${DRV_SPI_ID}, SPI_CLOCK_POLARITY_IDLE_HIGH);
    PLIB_SPI_OutputDataPhaseSelect(${DRV_SPI_ID}, SPI_OUTPUT_DATA_PHASE_ON_ACTIVE_TO_IDLE_CLOCK);
    </#if>
    PLIB_SPI_InputSamplePhaseSelect(${DRV_SPI_ID}, ${DRV_SPI_IPS});
    PLIB_SPI_CommunicationWidthSelect(${DRV_SPI_ID}, ${DRV_SPI_CW});
    <#if DRV_SPI_PROTO=="DRV_SPI_PROTOCOL_TYPE_STANDARD">
    PLIB_SPI_FramedCommunicationDisable( ${DRV_SPI_ID}  );
        <#if CONFIG_AUDIO_CODEC_SUPPORT==true>
    PLIB_SPI_AudioProtocolDisable(${DRV_SPI_ID});
        </#if>
    <#elseif DRV_SPI_PROTO=="DRV_SPI_PROTOCOL_TYPE_FRAMED">
    PLIB_SPI_FrameSyncPulseDirectionSelect(${DRV_SPI_ID}, ${DRV_SPI_FSPDS});
    PLIB_SPI_FrameSyncPulsePolaritySelect(${DRV_SPI_ID}, ${DRV_SPI_FSPPS});
    PLIB_SPI_FrameSyncPulseEdgeSelect(${DRV_SPI_ID}, ${DRV_SPI_FSPES});
    PLIB_SPI_FrameSyncPulseWidthSelect(${DRV_SPI_ID}, ${DRV_SPI_FSPWS});
    PLIB_SPI_FrameSyncPulseCounterSelect(${DRV_SPI_ID}, ${DRV_SPI_FSPCS});
        <#if CONFIG_AUDIO_CODEC_SUPPORT==true>
    PLIB_SPI_AudioProtocolDisable(${DRV_SPI_ID});
        </#if>
    PLIB_SPI_FramedCommunicationEnable( ${DRV_SPI_ID}  );
    <#elseif CONFIG_AUDIO_CODEC_SUPPORT==true>
    PLIB_SPI_FramedCommunicationDisable( ${DRV_SPI_ID}  );
    PLIB_SPI_AudioTransmitModeSelect(${DRV_SPI_ID}, ${DRV_SPI_ATMS});
    PLIB_SPI_AudioProtocolModeSelect(${DRV_SPI_ID}, ${DRV_SPI_APMS});
    PLIB_SPI_AudioProtocolDisable(${DRV_SPI_ID});
    </#if>
    <#if CONFIG_ENHANCED_BUFFER_MODE_SUPPORT==true>
    <#if DRV_SPI_EBM="DRV_SPI_BUFFER_TYPE_ENHANCED">
    PLIB_SPI_FIFOEnable( ${DRV_SPI_ID}  );
    <#else>
    PLIB_SPI_FIFODisable( ${DRV_SPI_ID}  );
    </#if>
    </#if>
    PLIB_SPI_BaudRateSet(${DRV_SPI_ID}, SYS_CLK_PeripheralFrequencyGet(${DRV_SPI_CLOCK}), ${DRV_SPI_BAUDRATE});
    PLIB_SPI_Enable(${DRV_SPI_ID});
}

void DRV_SPI${DRV_INSTANCE}_DeInitialize(void)
{
    /* Disable SPI */
	PLIB_SPI_Disable(${DRV_SPI_ID});
}

bool DRV_SPI${DRV_INSTANCE}_ReceiverBufferIsFull(void)
{
    return (PLIB_SPI_ReceiverBufferIsFull(${DRV_SPI_ID}));
}

bool DRV_SPI${DRV_INSTANCE}_TransmitterBufferIsFull(void)
{
    return (PLIB_SPI_TransmitBufferIsFull(${DRV_SPI_ID}));
}
int32_t DRV_SPI${DRV_INSTANCE}_BufferAddWriteRead(const void * txBuffer, void * rxBuffer, uint32_t size)
{
    bool continueLoop;
    int32_t txcounter = 0;
    int32_t rxcounter = 0;
<#if CONFIG_ENHANCED_BUFFER_MODE_SUPPORT==true>
    uint8_t unitsTxed = 0;
    <#if DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_8BITS">
    const uint8_t maxUnits = 16;
    <#elseif DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_16BITS">
    const uint8_t maxUnits = 8;
    <#else>
    const uint8_t maxUnits = 4;
    </#if>
</#if>
    do {
        continueLoop = false;
<#if CONFIG_ENHANCED_BUFFER_MODE_SUPPORT==true && DRV_SPI_EBM="DRV_SPI_BUFFER_TYPE_ENHANCED">
        unitsTxed = 0;
        if (PLIB_SPI_TransmitBufferIsEmpty(${DRV_SPI_ID}))
        {
    <#if DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_8BITS">
            while (!PLIB_SPI_TransmitBufferIsFull(${DRV_SPI_ID}) && (txcounter < size) && unitsTxed != maxUnits)
    <#elseif DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_16BITS">
            while (!PLIB_SPI_TransmitBufferIsFull(${DRV_SPI_ID}) && (txcounter < (size>>1)) && unitsTxed != maxUnits)
    <#else>
            while (!PLIB_SPI_TransmitBufferIsFull(${DRV_SPI_ID}) && (txcounter < (size>>2)) && unitsTxed != maxUnits)
    </#if>
            {
    <#if DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_8BITS">
                PLIB_SPI_BufferWrite(${DRV_SPI_ID}, ((uint8_t*)txBuffer)[txcounter]);
    <#elseif DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_16BITS">
                PLIB_SPI_BufferWrite16bit(${DRV_SPI_ID}, ((uint16_t*)txBuffer)[txcounter]);
    <#else>
                PLIB_SPI_BufferWrite32bit(${DRV_SPI_ID}, ((uint32_t*)txBuffer)[txcounter]);
    </#if>
                txcounter++;
                continueLoop = true;
                unitsTxed++;
            }
        }
<#else>
    <#if DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_8BITS">
        if(!PLIB_SPI_TransmitBufferIsFull(${DRV_SPI_ID}) && txcounter < size)
    <#elseif DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_16BITS">
        if(!PLIB_SPI_TransmitBufferIsFull(${DRV_SPI_ID}) && txcounter < (size>>1))
    <#else>
        if(!PLIB_SPI_TransmitBufferIsFull(${DRV_SPI_ID}) && txcounter < (size>>2))
    </#if>
        {
    <#if DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_8BITS">
            PLIB_SPI_BufferWrite(${DRV_SPI_ID}, ((uint8_t*)txBuffer)[txcounter]);
    <#elseif DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_16BITS">
            PLIB_SPI_BufferWrite16bit(${DRV_SPI_ID}, ((uint16_t*)txBuffer)[txcounter]);
    <#else>
            PLIB_SPI_BufferWrite32bit(${DRV_SPI_ID}, ((uint32_t*)txBuffer)[txcounter]);
    </#if>
            txcounter++;
            continueLoop = true;
        }
</#if>
    
        while (txcounter != rxcounter)
        {
<#if CONFIG_ENHANCED_BUFFER_MODE_SUPPORT==true && DRV_SPI_EBM="DRV_SPI_BUFFER_TYPE_ENHANCED">
            while (PLIB_SPI_ReceiverFIFOIsEmpty(${DRV_SPI_ID}));
    <#if DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_8BITS">
            ((uint8_t*)rxBuffer)[rxcounter] = PLIB_SPI_BufferRead(${DRV_SPI_ID});
    <#elseif DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_16BITS">
            ((uint16_t*)rxBuffer)[rxcounter] = PLIB_SPI_BufferRead16bit(${DRV_SPI_ID});
    <#else>
            ((uint32_t*)rxBuffer)[rxcounter] = PLIB_SPI_BufferRead32bit(${DRV_SPI_ID});
    </#if>            
            rxcounter++;
            continueLoop = true;
<#else>
            while(!PLIB_SPI_ReceiverBufferIsFull(${DRV_SPI_ID}));
    <#if DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_8BITS">
            ((uint8_t*)rxBuffer)[rxcounter] = PLIB_SPI_BufferRead(${DRV_SPI_ID});
    <#elseif DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_16BITS">
            ((uint16_t*)rxBuffer)[rxcounter] = PLIB_SPI_BufferRead16bit(${DRV_SPI_ID});
    <#else>
            ((uint32_t*)rxBuffer)[rxcounter] = PLIB_SPI_BufferRead32bit(${DRV_SPI_ID});
    </#if>
            rxcounter++;
            continueLoop = true;
</#if>
        }
        if (txcounter > rxcounter)
        {
            continueLoop = true;
        }
    }while(continueLoop);
<#if DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_8BITS">
    return txcounter;
<#elseif DRV_SPI_CW=="SPI_COMMUNICATION_WIDTH_16BITS">
    return txcounter<<1;
<#else>
    return txcounter<<2;
</#if>
}
</#macro>
<#if CONFIG_DRV_SPI_STATIC_IDX0 == true>
<@DRV_SPI_STATIC_FUNCTIONS DRV_INSTANCE="0" 
                      DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_STATIC_IDX0 
                      DRV_SPI_CLOCK=CONFIG_DRV_SPI_SPI_CLOCK_STATIC_IDX0
                      DRV_SPI_BAUDRATE=CONFIG_DRV_SPI_BAUD_RATE_STATIC_IDX0
                      DRV_SPI_MSTR=CONFIG_DRV_SPI_SPI_MODE_STATIC_IDX0
                      DRV_SPI_SII=CONFIG_DRV_SPI_ALLOW_IDLE_RUN_STATIC_IDX0
                      DRV_SPI_CM=CONFIG_DRV_SPI_CLOCK_MODE_STATIC_IDX0
                      DRV_SPI_IPS=CONFIG_DRV_SPI_INPUT_PHASE_STATIC_IDX0
                      DRV_SPI_CW=CONFIG_DRV_SPI_COMM_WIDTH_STATIC_IDX0
                      DRV_SPI_PROTO=    CONFIG_DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX0
                      DRV_SPI_FSPDS=CONFIG_DRV_SPI_FRAME_PULSE_DIRECTION_STATIC_IDX0
                      DRV_SPI_FSPPS= CONFIG_DRV_SPI_FRAME_PULSE_POLARITY_STATIC_IDX0
                      DRV_SPI_FSPES=     CONFIG_DRV_SPI_FRAME_PULSE_EDGE_STATIC_IDX0
                      DRV_SPI_FSPWS=    CONFIG_DRV_SPI_FRAME_PULSE_WIDTH_STATIC_IDX0
                      DRV_SPI_FSPCS=     CONFIG_DRV_SPI_FRAME_SYNC_PULSE_STATIC_IDX0
                      DRV_SPI_ATMS=CONFIG_DRV_SPI_AUDIO_TRANSMIT_MODE_STATIC_IDX0
                      DRV_SPI_APMS=CONFIG_DRV_SPI_AUDIO_PROTOCOL_MODE_STATIC_IDX0
                      DRV_SPI_EBM=CONFIG_DRV_SPI_BUFFER_STATIC_IDX0
                      />
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX1 == true>
<@DRV_SPI_STATIC_FUNCTIONS DRV_INSTANCE="1" 
                      DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_STATIC_IDX1 
                      DRV_SPI_CLOCK=CONFIG_DRV_SPI_SPI_CLOCK_STATIC_IDX1
                      DRV_SPI_BAUDRATE=CONFIG_DRV_SPI_BAUD_RATE_STATIC_IDX1
                      DRV_SPI_MSTR=CONFIG_DRV_SPI_SPI_MODE_STATIC_IDX1
                      DRV_SPI_SII=CONFIG_DRV_SPI_ALLOW_IDLE_RUN_STATIC_IDX1
                      DRV_SPI_CM=CONFIG_DRV_SPI_CLOCK_MODE_STATIC_IDX1
                      DRV_SPI_IPS=CONFIG_DRV_SPI_INPUT_PHASE_STATIC_IDX1
                      DRV_SPI_CW=CONFIG_DRV_SPI_COMM_WIDTH_STATIC_IDX1
                      DRV_SPI_PROTO=CONFIG_DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX1
                      DRV_SPI_FSPDS=CONFIG_DRV_SPI_FRAME_PULSE_DIRECTION_STATIC_IDX1
                      DRV_SPI_FSPPS= CONFIG_DRV_SPI_FRAME_PULSE_POLARITY_STATIC_IDX1
                      DRV_SPI_FSPES=     CONFIG_DRV_SPI_FRAME_PULSE_EDGE_STATIC_IDX1
                      DRV_SPI_FSPWS=    CONFIG_DRV_SPI_FRAME_PULSE_WIDTH_STATIC_IDX1
                      DRV_SPI_FSPCS=     CONFIG_DRV_SPI_FRAME_SYNC_PULSE_STATIC_IDX1
                      DRV_SPI_ATMS=CONFIG_DRV_SPI_AUDIO_TRANSMIT_MODE_STATIC_IDX1
                      DRV_SPI_APMS=CONFIG_DRV_SPI_AUDIO_PROTOCOL_MODE_STATIC_IDX1
                      DRV_SPI_EBM=CONFIG_DRV_SPI_BUFFER_STATIC_IDX1
                      />
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX2 == true>
<@DRV_SPI_STATIC_FUNCTIONS DRV_INSTANCE="2" 
                      DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_STATIC_IDX2 
                      DRV_SPI_CLOCK=CONFIG_DRV_SPI_SPI_CLOCK_STATIC_IDX2
                      DRV_SPI_BAUDRATE=CONFIG_DRV_SPI_BAUD_RATE_STATIC_IDX2
                      DRV_SPI_MSTR=CONFIG_DRV_SPI_SPI_MODE_STATIC_IDX2
                      DRV_SPI_SII=CONFIG_DRV_SPI_ALLOW_IDLE_RUN_STATIC_IDX2
                      DRV_SPI_CM=CONFIG_DRV_SPI_CLOCK_MODE_STATIC_IDX2
                      DRV_SPI_IPS=CONFIG_DRV_SPI_INPUT_PHASE_STATIC_IDX2
                      DRV_SPI_CW=CONFIG_DRV_SPI_COMM_WIDTH_STATIC_IDX2
                      DRV_SPI_PROTO=CONFIG_DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX2
                      DRV_SPI_FSPDS=CONFIG_DRV_SPI_FRAME_PULSE_DIRECTION_STATIC_IDX2
                      DRV_SPI_FSPPS= CONFIG_DRV_SPI_FRAME_PULSE_POLARITY_STATIC_IDX2
                      DRV_SPI_FSPES=     CONFIG_DRV_SPI_FRAME_PULSE_EDGE_STATIC_IDX2
                      DRV_SPI_FSPWS=    CONFIG_DRV_SPI_FRAME_PULSE_WIDTH_STATIC_IDX2
                      DRV_SPI_FSPCS=     CONFIG_DRV_SPI_FRAME_SYNC_PULSE_STATIC_IDX2
                      DRV_SPI_ATMS=CONFIG_DRV_SPI_AUDIO_TRANSMIT_MODE_STATIC_IDX2
                      DRV_SPI_APMS=CONFIG_DRV_SPI_AUDIO_PROTOCOL_MODE_STATIC_IDX2
                      DRV_SPI_EBM=CONFIG_DRV_SPI_BUFFER_STATIC_IDX2
                      />
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX3 == true>
<@DRV_SPI_STATIC_FUNCTIONS DRV_INSTANCE="3" 
                      DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_STATIC_IDX3 
                      DRV_SPI_CLOCK=CONFIG_DRV_SPI_SPI_CLOCK_STATIC_IDX3
                      DRV_SPI_BAUDRATE=CONFIG_DRV_SPI_BAUD_RATE_STATIC_IDX3
                      DRV_SPI_MSTR=CONFIG_DRV_SPI_SPI_MODE_STATIC_IDX3
                      DRV_SPI_SII=CONFIG_DRV_SPI_ALLOW_IDLE_RUN_STATIC_IDX3
                      DRV_SPI_CM=CONFIG_DRV_SPI_CLOCK_MODE_STATIC_IDX3
                      DRV_SPI_IPS=CONFIG_DRV_SPI_INPUT_PHASE_STATIC_IDX3
                      DRV_SPI_CW=CONFIG_DRV_SPI_COMM_WIDTH_STATIC_IDX3
                      DRV_SPI_PROTO=CONFIG_DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX3
                      DRV_SPI_FSPDS=CONFIG_DRV_SPI_FRAME_PULSE_DIRECTION_STATIC_IDX3
                      DRV_SPI_FSPPS= CONFIG_DRV_SPI_FRAME_PULSE_POLARITY_STATIC_IDX3
                      DRV_SPI_FSPES=     CONFIG_DRV_SPI_FRAME_PULSE_EDGE_STATIC_IDX3
                      DRV_SPI_FSPWS=    CONFIG_DRV_SPI_FRAME_PULSE_WIDTH_STATIC_IDX3
                      DRV_SPI_FSPCS=     CONFIG_DRV_SPI_FRAME_SYNC_PULSE_STATIC_IDX3
                      DRV_SPI_ATMS=CONFIG_DRV_SPI_AUDIO_TRANSMIT_MODE_STATIC_IDX3
                      DRV_SPI_APMS=CONFIG_DRV_SPI_AUDIO_PROTOCOL_MODE_STATIC_IDX3
                      DRV_SPI_EBM=CONFIG_DRV_SPI_BUFFER_STATIC_IDX3
                      />
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX4 == true>
<@DRV_SPI_STATIC_FUNCTIONS DRV_INSTANCE="4" 
                      DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_STATIC_IDX4 
                      DRV_SPI_CLOCK=CONFIG_DRV_SPI_SPI_CLOCK_STATIC_IDX4
                      DRV_SPI_BAUDRATE=CONFIG_DRV_SPI_BAUD_RATE_STATIC_IDX4
                      DRV_SPI_MSTR=CONFIG_DRV_SPI_SPI_MODE_STATIC_IDX4
                      DRV_SPI_SII=CONFIG_DRV_SPI_ALLOW_IDLE_RUN_STATIC_IDX4
                      DRV_SPI_CM=CONFIG_DRV_SPI_CLOCK_MODE_STATIC_IDX4
                      DRV_SPI_IPS=CONFIG_DRV_SPI_INPUT_PHASE_STATIC_IDX4
                      DRV_SPI_CW=CONFIG_DRV_SPI_COMM_WIDTH_STATIC_IDX4
                      DRV_SPI_PROTO=CONFIG_DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX4
                      DRV_SPI_FSPDS=CONFIG_DRV_SPI_FRAME_PULSE_DIRECTION_STATIC_IDX4
                      DRV_SPI_FSPPS= CONFIG_DRV_SPI_FRAME_PULSE_POLARITY_STATIC_IDX4
                      DRV_SPI_FSPES=     CONFIG_DRV_SPI_FRAME_PULSE_EDGE_STATIC_IDX4
                      DRV_SPI_FSPWS=    CONFIG_DRV_SPI_FRAME_PULSE_WIDTH_STATIC_IDX4
                      DRV_SPI_FSPCS=     CONFIG_DRV_SPI_FRAME_SYNC_PULSE_STATIC_IDX4
                      DRV_SPI_ATMS=CONFIG_DRV_SPI_AUDIO_TRANSMIT_MODE_STATIC_IDX4
                      DRV_SPI_APMS=CONFIG_DRV_SPI_AUDIO_PROTOCOL_MODE_STATIC_IDX4
                      DRV_SPI_EBM=CONFIG_DRV_SPI_BUFFER_STATIC_IDX4
                      />
</#if>
<#if CONFIG_DRV_SPI_STATIC_IDX5 == true>
<@DRV_SPI_STATIC_FUNCTIONS DRV_INSTANCE="5" 
                      DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_STATIC_IDX5 
                      DRV_SPI_CLOCK=CONFIG_DRV_SPI_SPI_CLOCK_STATIC_IDX5
                      DRV_SPI_BAUDRATE=CONFIG_DRV_SPI_BAUD_RATE_STATIC_IDX5
                      DRV_SPI_MSTR=CONFIG_DRV_SPI_SPI_MODE_STATIC_IDX5
                      DRV_SPI_SII=CONFIG_DRV_SPI_ALLOW_IDLE_RUN_STATIC_IDX5
                      DRV_SPI_CM=CONFIG_DRV_SPI_CLOCK_MODE_STATIC_IDX5
                      DRV_SPI_IPS=CONFIG_DRV_SPI_INPUT_PHASE_STATIC_IDX5
                      DRV_SPI_CW=CONFIG_DRV_SPI_COMM_WIDTH_STATIC_IDX5
                      DRV_SPI_PROTO=CONFIG_DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX5
                      DRV_SPI_FSPDS=CONFIG_DRV_SPI_FRAME_PULSE_DIRECTION_STATIC_IDX5
                      DRV_SPI_FSPPS= CONFIG_DRV_SPI_FRAME_PULSE_POLARITY_STATIC_IDX5
                      DRV_SPI_FSPES=     CONFIG_DRV_SPI_FRAME_PULSE_EDGE_STATIC_IDX5
                      DRV_SPI_FSPWS=    CONFIG_DRV_SPI_FRAME_PULSE_WIDTH_STATIC_IDX5
                      DRV_SPI_FSPCS=     CONFIG_DRV_SPI_FRAME_SYNC_PULSE_STATIC_IDX5
                      DRV_SPI_ATMS=CONFIG_DRV_SPI_AUDIO_TRANSMIT_MODE_STATIC_IDX5
                      DRV_SPI_APMS=CONFIG_DRV_SPI_AUDIO_PROTOCOL_MODE_STATIC_IDX5
                      DRV_SPI_EBM=CONFIG_DRV_SPI_BUFFER_STATIC_IDX5
                      />
</#if>

/*******************************************************************************
 End of File
*/
