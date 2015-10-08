/*******************************************************************************
  ADC Driver Feature Variant Implementations

  Company:
    Microchip Technology Inc.

  File Name:
    drv_adc_variant_mapping.h
  
  Summary:
    ADC Driver feature variant implementations.

  Description:
    This file implements the functions which differ based on different parts
    and various implementations of the same feature.
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


#ifndef _DRV_ADC_VARIANT_MAPPING_H
#define _DRV_ADC_VARIANT_MAPPING_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************


#if !defined(DRV_ADC_INSTANCES_NUMBER)

    #if !defined(DRV_ADC_CLIENTS_NUMBER)

        /* Map macros and function to the static single open variant */
        #include "driver/adc/src/static/drv_adc_hw_static.h"
        #include "driver/adc/src/client_single/drv_adc_client_single.h"

    #else
    
        /* Map macros and function to the multi static variant */
        #include "driver/adc/src/static/drv_adc_hw_static.h"
        #include "driver/adc/src/client_multi/drv_adc_client_multi.h"

    #endif

#else

    /* Map macros and function to the dynamic variant */
    #include "driver/adc/src/dynamic/drv_adc_hw_dynamic.h"
    #include "driver/adc/src/client_multi/drv_adc_client_multi.h"

#endif


// *****************************************************************************
// *****************************************************************************
// Section: Feature Variant Mapping
// *****************************************************************************
// *****************************************************************************
/* Some variants are determined by hardware feature existence and some features
   are determined user configuration of the driver, and some variants are
   combination of the two above.
*/


// *****************************************************************************
/* peripheral library ID Static Configuration Override

  Summary:
    Allows static override of the peripehral library ID.

  Description:
    These macros allow the peripheral library ID to be statically overriden by
    the DRV_ADC_PERIPHERAL_ID configuration macro, if it is defined.

    _DRV_ADC_PERIPHERAL_ID_GET replaces the value passed in with the value defined by
    the DRV_ADC_PERIPHERAL_ID configuration option.
*/

#if defined(DRV_ADC_PERIPHERAL_ID)

    #define _DRV_ADC_PERIPHERAL_ID_GET(plibID)                DRV_ADC_PERIPHERAL_ID

#else

    #define _DRV_ADC_PERIPHERAL_ID_GET(plibId)                ( plibId )

#endif


// *****************************************************************************
/* Interrupt Source Static Configuration Override

  Summary:
    Allows static override of the interrupt source.

  Description:
    These macros allow the interrupt source to be statically overriden by the
    DRV_ADC_INTERRUPT_SOURCE configuration macro, if it is defined.

    _DRV_ADC_GET_INT_SRC replaces the value passed in with the value defined by
    the DRV_ADC_INTERRUPT_SOURCE configuration option.

    _DRV_ADC_STATIC_INT_SRC removes any statement passed into it from the build
    if the DRV_ADC_INTERRUPT_SOURCE configuration option is defined.
*/

#if defined(DRV_ADC_INTERRUPT_SOURCE)

    /* Static access macros */
    #define _DRV_ADC_GET_INT_SRC(source)                DRV_ADC_INTERRUPT_SOURCE
    #define _DRV_ADC_STATIC_INT_SRC(any)

#else

    /* Dynamic access macros */
    #define _DRV_ADC_GET_INT_SRC(source)                source
    #define _DRV_ADC_STATIC_INT_SRC(any)                any
 
#endif

// *****************************************************************************
/* Macro:
    _DRV_ADC_EnableInternal(index)

  Summary:
    Starts the ADC module defined by the index parameter and maps to the
    respective peripheral library API.

  Description:
    This macro starts the ADC module defined by the index parameter and maps to
    the respective peripheral library API.

    This macro expands to the peripheral library API only if the feature is 
    supported by the respective device.

  Remarks:
    None.
*/

#if defined (_ADC_EXISTS_ENABLE_CONTROL)

    #define _DRV_ADC_EnableInternal( index )            \
            PLIB_ADC_Enable( _DRV_ADC_PERIPHERAL_ID_GET ( index ) )

#else

    #define _DRV_ADC_EnableInternal( index )

#endif


// *****************************************************************************
/* Macro:
    _DRV_ADC_DisableInternal(index)

  Summary:
    Stops the ADC module defined by the index parameter and maps to the respective 
    peripheral library API.

  Description:
    This macro stops the ADC module defined by the index parameter and maps to
    the respective peripheral library API.

    This macro expands to the peripheral library API only if the feature is 
    supported by the respective device.

  Remarks:
    None.
*/

#if defined (_ADC_EXISTS_ENABLE_CONTROL)

    #define _DRV_ADC_DisableInternal( index )           \
            PLIB_ADC_Disable( _DRV_ADC_PERIPHERAL_ID_GET ( index ) )

#else

    #define _DRV_ADC_DisableInternal( index )

#endif


// *****************************************************************************
/* Macro:
    _DRV_ADC_SelectInput( index, input )

  Summary:
    Selects the input channel for the ADC module defined by the index parameter
    and maps to the respective peripheral library API.

  Description:
    This macro selects the input channel for the ADC module defined by the index
    parameter and maps to the respective peripheral library API.

    This macro expands to the peripheral library API only if the feature is 
    supported by the respective device.

  Remarks:
    None
*/

#if defined(_ADC_EXISTS_MUXA_INPUT_SCAN)

    #define _DRV_ADC_SelectInput( index, input )        \
            PLIB_ADC_InputScanMaskAdd( _DRV_ADC_PERIPHERAL_ID_GET ( index ), input )

#elif defined(_ADC_EXISTS_INPUT_SELECT)

    #define _DRV_ADC_SelectInput( index, input )        \
            PLIB_ADC_InputSelect( _DRV_ADC_PERIPHERAL_ID_GET ( index ), input )
#else

    #define _DRV_ADC_SelectInput( index, input )

#endif


// *****************************************************************************
/* Macro:
    _DRV_ADC_ManualStart( index )

  Summary:
    Initiates manual mode for the ADC module defined by the index parameter and 
    maps to the respective peripheral library API.

  Description:
    This macro initiates the manual mode for the ADC module defined by the index
    parameter and maps to the respective peripheral library API.

    This macro expands to the peripheral library API only if the feature is 
    supported by the respective device.

    This API starts manual sampling for PIC24/32 devices, however, conversion
    happens in automatic mode after the sampling is finished

    This API starts conversion in case of PIC18 devices.

  Remarks:
    None.
*/

#if defined(_ADC_EXISTS_SAMPLE_START_STOP) && (DRV_ADC_CONFIG_AUTO_SAMPLING_ENABLE == false)

    #define _DRV_ADC_ManualStart( index )               \
            PLIB_ADC_SamplingStart( _DRV_ADC_PERIPHERAL_ID_GET ( index ) )

#elif defined(_ADC_EXISTS_CONVERSION_START)

    #define _DRV_ADC_ManualStart( index )               \
            PLIB_ADC_ConversionStart( _DRV_ADC_PERIPHERAL_ID_GET ( index ) )

#else

    #define _DRV_ADC_ManualStart( index )

#endif

// *****************************************************************************
/* Macro:
    _DRV_ADC_SampleAutoStartDisable(index)

  Summary:
    Disables auto-sampling mode for the ADC module defined by the index parameter 
    and maps to the respective peripheral library API.

  Description:
    This macro disables auto-sampling mode for the ADC module defined by the index
    parameter and maps to the respective peripheral library API.

    This macro expands to the peripheral library API only if the feature is 
    supported by the respective device.

  Remarks:
    None.
*/

#if defined(_ADC_EXISTS_AUTO_SAMPLE_ENABLE) && (DRV_ADC_CONFIG_AUTO_SAMPLING_ENABLE == true)

    #define _DRV_ADC_SampleAutoStartDisable( index )    \
            PLIB_ADC_SampleAutoStartDisable( _DRV_ADC_PERIPHERAL_ID_GET ( index ) )

#else

    #define _DRV_ADC_SampleAutoStartDisable( index )

#endif



// *****************************************************************************
/* Macro:
    _DRV_ADC_ConversionStopSequenceDisable(index)

  Summary:
    Enable normal conversion of the ADC module defined by the index parameter and 
    maps to the respective peripheral library API.

  Description:
    This macro enables normal conversion of the ADC module defined by the index
    parameter and maps to the respective peripheral library API.

    This macro expands to the peripheral library API only if the feature is 
    supported by the respective device.

  Remarks:
    None.
*/

#if defined(_ADC_EXISTS_CONVERSION_STOP) && (DRV_ADC_CONFIG_STOP_ON_CONVERSION_ENABLE == true)

    #define _DRV_ADC_ConversionStopSequenceDisable( index )    \
            PLIB_ADC_ConversionStopSequenceDisable( _DRV_ADC_PERIPHERAL_ID_GET ( index ) )

#else

    #define _DRV_ADC_ConversionStopSequenceDisable( index )

#endif

// *****************************************************************************
/* Interrupt Source Control

  Summary:
    Rnables, disables or clears the interrupt source.

  Description:
    This macro enables, disables or clears the interrupt source

    This macro gets mapped to the respective SYS module APIs if the configuration
    option DRV_ADC_INTERRUPT_MODE is set to true.

  Remarks:
    This macro is mandatory.
*/

#if defined (DRV_ADC_INTERRUPT_MODE) && (DRV_ADC_INTERRUPT_MODE == true)

    #define _DRV_ADC_InterruptSourceEnable( source )        SYS_INT_SourceEnable( source )
    #define _DRV_ADC_InterruptSourceDisable( source )       SYS_INT_SourceDisable( source )
    #define _DRV_ADC_InterruptSourceClear( source )         SYS_INT_SourceStatusClear( source )

    #define _DRV_ADC_InterruptSourceStatusGet( source )     SYS_INT_SourceStatusGet( source )

#elif defined (DRV_ADC_INTERRUPT_MODE) && (DRV_ADC_INTERRUPT_MODE == false)

    #define _DRV_ADC_InterruptSourceEnable( source )
    #define _DRV_ADC_InterruptSourceDisable( source )
    #define _DRV_ADC_InterruptSourceClear( source )         SYS_INT_SourceStatusClear( source )

    #define _DRV_ADC_InterruptSourceStatusGet( source )     SYS_INT_SourceStatusGet( source )

#else

    #error "No Task mode chosen at build, interrupt or polling needs to be selected. "

#endif


// *****************************************************************************
/* Macro:
    _DRV_ADC_ReadSample( index, bufferIndex )

  Summary:
    Read the samples from the ADC module defined by the index parameter and maps 
    to the respective peripheral library API.

  Description:
    This macro reads the samples from the ADC module defined by the index
    parameter and maps to the respective peripheral library API

    This macro expands to the peripheral library API only if the feature is 
    supported by the respective device.

  Remarks:
    None.
*/
#define _DRV_ADC_ReadSample( index, bufferIndex )   \
            _DRV_ADC_ReadSampleFromArray( index, bufferIndex )


// *****************************************************************************
/* ADC read result mapping

  Summary:
    Maps the read result API either statically or dynamically.

  Description:
    This macro maps the read result API either statically or dynamically based
    on the configuration of DRV_ADC_INSTANCES_NUMBER.

  Remarks:
    This macro is mandatory.
*/

#if !defined(DRV_ADC_INSTANCES_NUMBER)

    /* Maps the Get Result macros for Static case */
    #define _DRV_ADC_RES_NAME_B(name, si)   name ## si
    #define _DRV_ADC_RES_NAME_A(name, si)   _DRV_ADC_RES_NAME_B(name, si)
    #define _DRV_ADC_RES_MAKE_NAME(name)    _DRV_ADC_RES_NAME_A(name, DRV_ADC_SAMPLES_PER_INTERRUPT)

    #define _DRV_ADC_ReadSampleFromArray( index, bufferIndex ) \
            _DRV_ADC_RES_MAKE_NAME(_DRV_ADC_GetResult)( _DRV_ADC_PERIPHERAL_ID_GET ( index ), bufferIndex )

#else

    /* Maps the Get Result macros for Dynamic case */
    #define _DRV_ADC_ReadSampleFromArray( index, bufferIndex ) \
            PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), bufferIndex )

#endif

// *****************************************************************************
/* Static read ADC result mapping

  Summary:
    Macros to read out the ADC buffer data statically.

  Description:
    These macros read out the ADC buffer data staticlaly based on the configuration
    entity DRV_ADC_SAMPLES_PER_INTERRUPT.

  Remarks:
    This macro is mandatory.
*/

#define _DRV_ADC_GetResult1( index, bufferIndex )       \
        (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 0 ))

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 1)

    #define _DRV_ADC_GetResult2( index, bufferIndex )   \
        ( (bufferIndex == 1)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 1 )) \
                            : _DRV_ADC_GetResult1( index, bufferIndex ) )
#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 2)

    #define _DRV_ADC_GetResult3( index, bufferIndex )   \
        ( (bufferIndex == 2)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 2 )) \
                            : _DRV_ADC_GetResult2( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 3)

    #define _DRV_ADC_GetResult4( index, bufferIndex )   \
        ( (bufferIndex == 3)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 3 )) \
                            : _DRV_ADC_GetResult3( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 4)

    #define _DRV_ADC_GetResult5( index, bufferIndex )   \
        ( (bufferIndex == 4)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 4 )) \
                            : _DRV_ADC_GetResult4( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 5)

    #define _DRV_ADC_GetResult6( index, bufferIndex )   \
        ( (bufferIndex == 5)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 5 )) \
                            : _DRV_ADC_GetResult5( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 6)

    #define _DRV_ADC_GetResult7( index, bufferIndex )   \
        ( (bufferIndex == 6)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 6 )) \
                            : _DRV_ADC_GetResult6( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 7)

    #define _DRV_ADC_GetResult8( index, bufferIndex )   \
        ( (bufferIndex == 7)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 7 )) \
                            : _DRV_ADC_GetResult7( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 8)

    #define _DRV_ADC_GetResult9( index, bufferIndex )   \
        ( (bufferIndex == 8)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 8 )) \
                            : _DRV_ADC_GetResult8( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 9)

    #define _DRV_ADC_GetResult10( index, bufferIndex )  \
        ( (bufferIndex == 9)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 9 )) \
                            : _DRV_ADC_GetResult9( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 10)

    #define _DRV_ADC_GetResult11( index, bufferIndex )  \
        ( (bufferIndex == 10)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 10 )) \
                             : _DRV_ADC_GetResult10( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 11)

    #define _DRV_ADC_GetResult12( index, bufferIndex )  \
        ( (bufferIndex == 11)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 11 )) \
                             : _DRV_ADC_GetResult11( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 12)

    #define _DRV_ADC_GetResult13( index, bufferIndex )  \
        ( (bufferIndex == 12)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 12 )) \
                             : _DRV_ADC_GetResult12( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 13)

    #define _DRV_ADC_GetResult14( index, bufferIndex )  \
        ( (bufferIndex == 13)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 13 )) \
                             : _DRV_ADC_GetResult13( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 14)

    #define _DRV_ADC_GetResult15( index, bufferIndex )  \
        ( (bufferIndex == 14)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 14 )) \
                             : _DRV_ADC_GetResult14( index, bufferIndex ) )

#endif

#if (DRV_ADC_SAMPLES_PER_INTERRUPT > 15)

    #define _DRV_ADC_GetResult16( index, bufferIndex )  \
        ( (bufferIndex == 15)? (PLIB_ADC_ResultGetByIndex( _DRV_ADC_PERIPHERAL_ID_GET ( index ), 15 ))  \
                             : _DRV_ADC_GetResult15( index, bufferIndex ) )

#endif


// *****************************************************************************
// *****************************************************************************
// Initializtion Parameter Static Overrides
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Analog channel input Configuration Override

  Summary:
    Allows static override of the analog chaneel input

  Description:
    This macro allows the analog input to be statically overriden by
    the DRV_ADC_ANALOG_INPUT configuration macro, if it is defined.

    _DRV_ADC_ANALOG_INPUT replaces the value passed in with the value defined by
    the DRV_ADC_ANALOG_INPUT configuration option.

   Remarks:
    None.
*/

#if defined(DRV_ADC_ANALOG_INPUT)

    #define _DRV_ADC_ANALOG_INPUT(arg)                  DRV_ADC_ANALOG_INPUT

#else

    #define _DRV_ADC_ANALOG_INPUT(arg)                  arg

#endif


// *****************************************************************************
/* Conversion clock prescaler Configuration Override

  Summary:
    Allows static override of the conversion clock prescaler.

  Description:
    This macro allows the conversion clock prescaler to be statically overriden by
    the DRV_ADC_CONVERSION_CLOCK_PRESCALER configuration macro, if it is defined.

    _DRV_ADC_CONVERSION_CLOCK replaces the value passed in with the value defined by
    the DRV_ADC_CONVERSION_CLOCK_PRESCALER configuration option.

   Remarks:
    None.
*/

#if defined(DRV_ADC_CONVERSION_CLOCK_PRESCALER)

    #define _DRV_ADC_CONVERSION_CLOCK(arg)              DRV_ADC_CONVERSION_CLOCK_PRESCALER

#else

    #define _DRV_ADC_CONVERSION_CLOCK(arg)              arg

#endif


// *****************************************************************************
/* Conversion clock source Configuration Override

  Summary:
    Allows static override of the conversion clock source.

  Description:
    This macro allows the conversion clock source to be statically overriden by
    the DRV_ADC_CONVERSION_CLOCK_SOURCE configuration macro, if it is defined.

    _DRV_ADC_CONVERSION_CLOCK_SOURCE_GET replaces the value passed in with the 
    value defined by the DRV_ADC_CONVERSION_CLOCK_SOURCE configuration option.

   Remarks:
    None.
*/

#if defined(DRV_ADC_CONVERSION_CLOCK_SOURCE)

    #define _DRV_ADC_CONVERSION_CLOCK_SOURCE_GET(arg)   DRV_ADC_CONVERSION_CLOCK_SOURCE

#else

    #define _DRV_ADC_CONVERSION_CLOCK_SOURCE_GET(arg)   arg

#endif


// *****************************************************************************
/* Conversion trigger source Configuration Override

  Summary:
    Allows static override of the conversion trigger source.

  Description:
    This macro allows the conversion trigger source to be statically overriden by
    the DRV_ADC_CONVERSION_TRIGGER_SOURCE configuration macro, if it is defined.

    _DRV_ADC_CONVERSION_TRIGGER_SOURCE_GET replaces the value passed in with the 
    value defined by the DRV_ADC_CONVERSION_TRIGGER_SOURCE configuration option.

   Remarks:
    None.
*/

#if defined(DRV_ADC_CONVERSION_TRIGGER_SOURCE)

    #define _DRV_ADC_CONVERSION_TRIGGER_SOURCE_GET(arg) DRV_ADC_CONVERSION_TRIGGER_SOURCE

#else

    #define _DRV_ADC_CONVERSION_TRIGGER_SOURCE_GET(arg) arg

#endif


// *****************************************************************************
/* Samples per interrupt Configuration Override

  Summary:
    Allows static override of the samples per interrupt.

  Description:
    This macro allows the Samples per interrupt to be statically overriden by
    the DRV_ADC_SAMPLES_PER_INTERRUPT configuration macro, if it is defined.

    _DRV_ADC_SAMPLES_PER_INTERRUPT_GET replaces the value passed in with the value 
    defined by the DRV_ADC_SAMPLES_PER_INTERRUPT configuration option.

   Remarks:
    None.
*/

#if defined(DRV_ADC_SAMPLES_PER_INTERRUPT)

    #define _DRV_ADC_SAMPLES_PER_INTERRUPT_GET(arg)     DRV_ADC_SAMPLES_PER_INTERRUPT

#else

    #define _DRV_ADC_SAMPLES_PER_INTERRUPT_GET(arg)     arg

#endif


// *****************************************************************************
/* Result format Configuration Override

  Summary:
    Allows static override of the Output data format.

  Description:
    This macro allows the Output data format to be statically overriden by
    the DRV_ADC_RESULT_FORMAT configuration macro, if it is defined.

    _DRV_ADC_RESULT_FORMAT replaces the value passed in with the value defined by
    the DRV_ADC_RESULT_FORMAT configuration option.

   Remarks:
    None.
*/

#if defined(DRV_ADC_RESULT_FORMAT)

    #define _DRV_ADC_RESULT_FORMAT(arg)            DRV_ADC_RESULT_FORMAT

#else

    #define _DRV_ADC_RESULT_FORMAT(arg)            arg

#endif


// *****************************************************************************
/* Voltage reference Configuration Override

  Summary:
    Allows static override of the voltage reference.

  Description:
    This macro allows the voltage reference to be statically overriden by
    the DRV_ADC_VOLTAGE_REFERENCE configuration macro, if it is defined.

    _DRV_ADC_VOLTAGE_REF_GET replaces the value passed in with the value defined by
    the DRV_ADC_VOLTAGE_REFERENCE configuration option.

   Remarks:
    None.
*/

#if defined(DRV_ADC_VOLTAGE_REFERENCE)

    #define _DRV_ADC_VOLTAGE_REF_GET(arg)               DRV_ADC_VOLTAGE_REFERENCE

#else

    #define _DRV_ADC_VOLTAGE_REF_GET(arg)               arg

#endif


// *****************************************************************************
/* Acquisition time Configuration Override

  Summary:
    Allows static override of the acquisition time.

  Description:
    This macro allows the acquisition time to be statically overriden by
    the DRV_ADC_ACQUISITION_TIME configuration macro, if it is defined.

    _DRV_ADC_ACQUISITION_TIME_GET replaces the value passed in with the value 
    defined by the DRV_ADC_ACQUISITION_TIME configuration option.

   Remarks:
    None.
*/

#if defined(DRV_ADC_ACQUISITION_TIME)

    #define _DRV_ADC_ACQUISITION_TIME_GET(arg)          DRV_ADC_ACQUISITION_TIME

#else

    #define _DRV_ADC_ACQUISITION_TIME_GET(arg)          arg

#endif


// *****************************************************************************
/* Auto sampling Configuration Override

  Summary:
    Allows static override of the auto-sampling.

  Description:
    This macro allows the auto-sampling to be statically overriden by
    the DRV_ADC_AUTO_SAMPLING_ENABLE configuration macro, if it is defined.

    If the DRV_ADC_AUTO_SAMPLING_ENABLE is true, the macro _DRV_ADC_AUTO_SAMPLING_GET
    gets replaced by DRV_ADC_AUTO_SAMPLING otherwise with DRV_ADC_MANUAL_SAMPLING

   Remarks:
    None.
*/

#if defined(DRV_ADC_AUTO_SAMPLING_ENABLE)

    #if (DRV_ADC_AUTO_SAMPLING_ENABLE == true)
        #define _DRV_ADC_AUTO_SAMPLING_GET(arg)                     DRV_ADC_AUTO_SAMPLING
    #else
        #define _DRV_ADC_AUTO_SAMPLING_GET(arg)                     DRV_ADC_MANUAL_SAMPLING
    #endif

#else

    #define _DRV_ADC_AUTO_SAMPLING_GET(arg)                         arg

#endif


// *****************************************************************************
/* Stop on conversion Configuration Override

  Summary:
    Allows static override of the stop on conversion.

  Description:
    This macro allows the Stop on conversion to be statically overriden by
    the DRV_ADC_STOP_ON_CONVERSION_ENABLE configuration macro, if it is defined.

    If the DRV_ADC_STOP_ON_CONVERSION_ENABLE is true, the macro
    _DRV_ADC_STOP_CONVERSION_ON_INTERRUPT_GET gets replaced by 
    DRV_ADC_STOP_CONVERSION_ON_INTERRUPT; otherwise, 0.

   Remarks:
    None.
*/

#if defined(DRV_ADC_STOP_ON_CONVERSION_ENABLE)

    #if (DRV_ADC_STOP_ON_CONVERSION_ENABLE == true)
        #define _DRV_ADC_STOP_CONVERSION_ON_INTERRUPT_GET(arg)      DRV_ADC_STOP_CONVERSION_ON_INTERRUPT
    #else
        #define _DRV_ADC_STOP_CONVERSION_ON_INTERRUPT_GET(arg)      0
    #endif

#else

    #define _DRV_ADC_STOP_CONVERSION_ON_INTERRUPT_GET(arg)          arg

#endif


// *****************************************************************************
/* Alternate input sampling Configuration Override

  Summary:
    Allows static override of the alternate input sampling.

  Description:
    This macro allows the alternate input sampling to be statically overriden by
    the DRV_ADC_ALTERNATE_INPUT_SAMPLING_ENABLE configuration macro, if it is defined.

    If the DRV_ADC_ALTERNATE_INPUT_SAMPLING_ENABLE is true, the macro
    _DRV_ADC_ALTERNATE_INPUT_SAMPLING_GET gets replaced by 
    DRV_ADC_ALTERNATE_INPUT_SAMPLING; otherwise, 0.

   Remarks:
    None.
*/

#if defined(DRV_ADC_ALTERNATE_INPUT_SAMPLING_ENABLE)

    #if (DRV_ADC_ALTERNATE_INPUT_SAMPLING_ENABLE == true)
        #define _DRV_ADC_ALTERNATE_INPUT_SAMPLING_GET(arg)          DRV_ADC_ALTERNATE_INPUT_SAMPLING
    #else
        #define _DRV_ADC_ALTERNATE_INPUT_SAMPLING_GET(arg)          0
    #endif

#else

    #define _DRV_ADC_ALTERNATE_INPUT_SAMPLING_GET(arg)              arg

#endif


// *****************************************************************************
/* Power mode Configuration Override

  Summary:
    Allows static override of the power mode.

  Description:
    These macros allow the Power mode to be statically overriden by
    the DRV_ADC_POWER_STATE configuration macro, if it is defined.

    _DRV_ADC_POWER_STATE_GET replaces the value passed in with the value defined by
    the DRV_ADC_POWER_STATE configuration option.

   Remarks:
    None.
*/

#if defined(DRV_ADC_POWER_STATE)

    #define _DRV_ADC_POWER_STATE_GET(arg)                         DRV_ADC_POWER_STATE

#else

    #define _DRV_ADC_POWER_STATE_GET(arg)                         (arg)

#endif


#endif //_DRV_ADC_VARIANT_MAPPING_H

/*******************************************************************************
 End of File
*/
