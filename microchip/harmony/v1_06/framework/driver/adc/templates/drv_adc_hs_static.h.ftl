/*******************************************************************************
  ADC Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_adc_static.h

  Summary:
    ADC driver interface declarations for the static single instance driver.

  Description:
    The ADC device driver provides a simple interface to manage the ADC
    modules on Microchip microcontrollers. This file defines the interface
    Declarations for the ADC driver.
    
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

#ifndef _DRV_ADC_STATIC_H
#define _DRV_ADC_STATIC_H

#include <stdbool.h>
#include "system_config.h"
#include "peripheral/adchs/plib_adchs.h"
<#if CONFIG_DRV_ADCHS_INTERRUPT_MODE == true>
#include "peripheral/int/plib_int.h"
</#if>

typedef enum {

    DRV_ADC_ID_1 = ADCHS_ID_0,
    DRV_ADCHS_NUMBER_OF_MODULES

} DRV_ADC_MODULE_ID;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Headers for ADC Static Driver
// *****************************************************************************
// *****************************************************************************
void DRV_ADC_Initialize(void);

inline void DRV_ADC_DeInitialize(void);

<#macro ADCHS_CHANNEL_FUNCTION CHANNEL_INST CHANNEL_ID>
inline void DRV_ADC${CHANNEL_INST}_Open(void);

inline void DRV_ADC${CHANNEL_INST}_Close(void);
</#macro>
<#if CONFIG_DRV_ADCHS_CHANNEL_INST_IDX0 == true>
<@ADCHS_CHANNEL_FUNCTION CHANNEL_INST="0" CHANNEL_ID=CONFIG_DRV_ADCHS_CHANNEL_ID_IDX0/>
</#if>
<#if CONFIG_DRV_ADCHS_CHANNEL_INST_IDX1 == true>
<@ADCHS_CHANNEL_FUNCTION CHANNEL_INST="1" CHANNEL_ID=CONFIG_DRV_ADCHS_CHANNEL_ID_IDX1/>
</#if>
<#if CONFIG_DRV_ADCHS_CHANNEL_INST_IDX2 == true>
<@ADCHS_CHANNEL_FUNCTION CHANNEL_INST="2" CHANNEL_ID=CONFIG_DRV_ADCHS_CHANNEL_ID_IDX2/>
</#if>
<#if CONFIG_DRV_ADCHS_CHANNEL_INST_IDX3 == true>
<@ADCHS_CHANNEL_FUNCTION CHANNEL_INST="3" CHANNEL_ID=CONFIG_DRV_ADCHS_CHANNEL_ID_IDX3/>
</#if>
<#if CONFIG_DRV_ADCHS_CHANNEL_INST_IDX4 == true>
<@ADCHS_CHANNEL_FUNCTION CHANNEL_INST="4" CHANNEL_ID=CONFIG_DRV_ADCHS_CHANNEL_ID_IDX4/>
</#if>

inline void DRV_ADC_Start(void);

inline void DRV_ADC_Stop(void);

inline void DRV_ADC_NegativeInputSelect(ADCHS_SCAN_TRIGGER_SOURCE mux, ADCHS_INPUT_MODE input);

inline void DRV_ADC_PositiveInputSelect(ADCHS_SCAN_TRIGGER_SOURCE mux, ADCHS_INPUT_MODE input);

inline void DRV_ADC_ChannelScanInputsAdd(ADCHS_AN_INPUT_ID scanInput);

inline void DRV_ADC_ChannelScanInputsRemove(ADCHS_AN_INPUT_ID scanInput);
<#if CONFIG_HAVE_ADC_ESCAN == true>

inline void DRV_ADC_ChannelExtendedScanInputsAdd(ADCHS_AN_INPUT_ID eScanInput);

inline void DRV_ADC_ChannelExtendedScanInputsRemove(ADCHS_AN_INPUT_ID eScanInput);
</#if>

uint32_t DRV_ADC_SamplesRead(uint8_t bufIndex);

bool DRV_ADC_SamplesAvailable(uint8_t bufIndex);



<#macro DIGITAL_COMP_FUNCTIONS DIG_COMP_INST DIG_COMP_ID>

inline void DRV_ADC_DigitalComparator${DIG_COMP_INST}_Open(void);

inline void DRV_ADC_DigitalComparator${DIG_COMP_INST}_Close(void);

inline bool DRV_ADC_DigitalComparator${DIG_COMP_INST}_EventHasOccurred(void);

</#macro>
<#if CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_INST_IDX0 == true>
<@DIGITAL_COMP_FUNCTIONS DIG_COMP_INST="0" DIG_COMP_ID=CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_ID_IDX0/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_INST_IDX1 == true>
<@DIGITAL_COMP_FUNCTIONS DIG_COMP_INST="1" DIG_COMP_ID=CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_ID_IDX1/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_INST_IDX2 == true>
<@DIGITAL_COMP_FUNCTIONS DIG_COMP_INST="2" DIG_COMP_ID=CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_ID_IDX2/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_INST_IDX3 == true>
<@DIGITAL_COMP_FUNCTIONS DIG_COMP_INST="3" DIG_COMP_ID=CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_ID_IDX3/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_INST_IDX4 == true>
<@DIGITAL_COMP_FUNCTIONS DIG_COMP_INST="4" DIG_COMP_ID=CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_ID_IDX4/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_INST_IDX5 == true>
<@DIGITAL_COMP_FUNCTIONS DIG_COMP_INST="5" DIG_COMP_ID=CONFIG_DRV_ADCHS_DIGITAL_COMPARATOR_ID_IDX5/>
</#if>

<#macro DIGITAL_FILT_FUNCTIONS DIG_FILT_INST DIG_FILT_ID>

inline void DRV_ADC_DigitalFilter${DIG_FILT_INST}_Open(void);

inline void DRV_ADC_DigitalFilter${DIG_FILT_INST}_Close(void);

inline bool DRV_ADC_DigitalFilter${DIG_FILT_INST}_DataIsReady(void);

inline int16_t DRV_ADC_DigitalFilter${DIG_FILT_INST}_DataRead(void);


</#macro>
<#if CONFIG_DRV_ADCHS_DIGITAL_FILTER_INST_IDX0 == true>
<@DIGITAL_FILT_FUNCTIONS DIG_FILT_INST="0" DIG_FILT_ID=CONFIG_DRV_ADCHS_DIGITAL_FILTER_ID_IDX0/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_FILTER_INST_IDX1 == true>
<@DIGITAL_FILT_FUNCTIONS DIG_FILT_INST="1" DIG_FILT_ID=CONFIG_DRV_ADCHS_DIGITAL_FILTER_ID_IDX1/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_FILTER_INST_IDX2 == true>
<@DIGITAL_FILT_FUNCTIONS DIG_FILT_INST="2" DIG_FILT_ID=CONFIG_DRV_ADCHS_DIGITAL_FILTER_ID_IDX2/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_FILTER_INST_IDX3 == true>
<@DIGITAL_FILT_FUNCTIONS DIG_FILT_INST="3" DIG_FILT_ID=CONFIG_DRV_ADCHS_DIGITAL_FILTER_ID_IDX3/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_FILTER_INST_IDX4 == true>
<@DIGITAL_FILT_FUNCTIONS DIG_FILT_INST="4" DIG_FILT_ID=CONFIG_DRV_ADCHS_DIGITAL_FILTER_ID_IDX4/>
</#if>
<#if CONFIG_DRV_ADCHS_DIGITAL_FILTER_INST_IDX5 == true>
<@DIGITAL_FILT_FUNCTIONS DIG_FILT_INST="5" DIG_FILT_ID=CONFIG_DRV_ADCHS_DIGITAL_FILTER_ID_IDX5/>
</#if>

<#if CONFIG_USE_DRV_ADCHS_CVD == true>
inline void DRV_ADC_CVD_Start(void);

inline void DRV_ADC_CVD_Stop(void);

inline uint16_t DRV_ADC_CVD_ResultRead(void);

inline bool DRV_ADC_DigitalComparator_EventHasOccurred(void);

ADCHS_AN_INPUT_ID DRV_ADC_DigitalComparator_AnalogInputGet(void);
</#if>

#endif // #ifndef _DRV_ADC_STATIC_H

/*******************************************************************************
 End of File
*/
