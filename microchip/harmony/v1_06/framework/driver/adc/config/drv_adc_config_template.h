/*******************************************************************************
  ADC Device Driver Configuration Template

  Company:    
    Microchip Technology Inc.

  File Name:   
    drv_adc_config_template.h
  
  Summary:    
    ADC Device Driver configuration template.

  Description:
    This header file contains the build-time configuration selections for the
    ADC device driver.  This is the template file which give all possible 
    configurations that can be made. This file should not be included in 
    any project.
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

#ifndef _DRV_ADC_CONFIG_TEMPLATE_H
#define _DRV_ADC_CONFIG_TEMPLATE_H

//#error "This is a configuration template file.  Do not include it directly."

// *****************************************************************************
// *****************************************************************************
// Section: ADC Core Functionality Configuration Macros
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* ADC hardware instance configuration

  Summary:
    Sets up the maximum number of hardware instances that can be supported.

  Description:
    This macro sets up the maximum number of hardware instances that can be 
    supported.

  Remarks:
    None.
*/

#define DRV_ADC_INSTANCES_NUMBER            1


/**********************************************************************
  Summary:
    Selects the maximum number of clients.
  Description:
    ADC Maximum Number of Clients
    
    This definition selected the maximum number of clients that the ADC
    driver can support at run time.
  Remarks:
    None.                                                              
  **********************************************************************/

#define DRV_ADC_CLIENTS_NUMBER            1


// *****************************************************************************
/* ADC Static Index Selection

  Summary:
    ADC static index selection.

  Description:
    ADC static index selection for the driver object reference.

  Remarks:
    This index is required to make a reference to the driver object.
*/

#define DRV_ADC_INDEX                            DRV_ADC_INDEX_0


// *****************************************************************************
/* ADC Interrupt And Polled Mode Operation Control

  Summary:
    Controls operation of the driver in the interrupt or polled mode.

  Description:
    This macro controls the operation of the driver in the interrupt
    mode of operation. The possible values of this macro are:
    - true - Select if interrupt mode of ADC operation is desired
    - false - Select if polling mode of ADC operation is desired
    Not defining this option to true or false will result in a build error.

  Remarks:
    None.
*/

#define DRV_ADC_INTERRUPT_MODE      true


// *****************************************************************************
/* ADC Internal buffer size 

  Summary:
    Define the internal buffer size.

  Description:
    This macro defines the internal buffer size. 

  Remarks:
    None.
*/

#define DRV_ADC_INTERNAL_BUFFER_SIZE             2


/**************************************************************************
  Summary:
    Enables the auto-sampling feature of the ADC.
  Description:
    ADC Auto Sampling Enable
    
    This macro enables the auto-sampling feature of the ADC. This macro can
    take the following values:
      * true - Enables the auto-sampling feature of the ADC
      * false - Disables the auto-sampling feature of the ADC
      * DRV_CONFIG_NOT_SUPPORTED - When the feature is not supported on
        the instance
  Remarks:
    None.                                                                  
  **************************************************************************/

#define DRV_ADC_AUTO_SAMPLING_ENABLE             true


// *****************************************************************************
/* ADC Alternate Input Sampling Enable

  Summary:
    Enable the alternate input sampling feature of the ADC.

  Description:
    This macro enables the alternate input sampling feature of the ADC. This macro can
    take the following values:
    - true - Enables the alternate Input sampling feature of the ADC
    - false - Disables the alternate Input sampling feature of the ADC
    - DRV_CONFIG_NOT_SUPPORTED - When the feature is not supported on the instance

  Remarks:
    None.
*/

#define DRV_ADC_ALTERNATE_INPUT_SAMPLING_ENABLE        false


// *****************************************************************************
/* ADC Stop on conversion Enable

  Summary:
    Enable the stop on conversion feature of the ADC.

  Description:
    This macro enables the stop on conversion feature of the ADC.  This macro can
    take the following values:
    - true - Enables the ADC to stop on conversion
    - false - Disables the ADC to stop on conversion
    - DRV_CONFIG_NOT_SUPPORTED - When the feature is not supported on the instance

  Remarks:
    None.
*/

#define DRV_ADC_STOP_ON_CONVERSION_ENABLE        false


// *****************************************************************************
// *****************************************************************************
// Section: Initialization Overrides
// *****************************************************************************
// *****************************************************************************
/* This section defines the initialization overrides */

/**************************************************************************
  Summary:
    ADC PLIB ID Selection
  Description:
    ADC PLIB ID Selection
    
    This macro selects the ADC PLIB ID Selection. This is an initialization
    override of the adcID member of the initialization configuration.
  Remarks:
    None.                                                                  
  **************************************************************************/

#define DRV_ADC_PERIPHERAL_ID                     ADC_ID_1


// *****************************************************************************
/* ADC Interrupt Source

  Summary:
    Defines the interrupt source of the static driver.

  Description:
    Macro to define the interrupt source of the static driver.

  Remarks:
    Refer to the Interrupt Peripheral Library document for more information on 
    the PLIB_INT_SOURCE enumeration.
*/

#define DRV_ADC_INTERRUPT_SOURCE            PLIB_INT_SOURCE_ADC_1


// *****************************************************************************
/* Samples per Interrupt

  Summary:
    Define the sample per interrupt.

  Description:
    This macro defines the samples per interrupt of the ADC driver. This
    provides static override of the dynamic selection of the sample per
    interrupt. If this macro is defined, this will be used for setting up the
    samples per interrupt and not the samples per interrupt value provided by
    DRV_ADC_INIT.

  Remarks:
    Select this size based on the device available and the number of samples
    that are required to form a set.

*/

#define DRV_ADC_SAMPLES_PER_INTERRUPT       2


/***********************************************************************
  Summary:
    Defines the acquisition time.
  Description:
    ADC Acquisition Time
    
    This macro defines the acquisition time of the ADC driver. This
    provides static override of the dynamic selection of the acquisition
    time. If this macro is defined, this will be used for setting up the
    acquisition time and not the acquisition time value provided by
    DRV_ADC_INIT.
  Remarks:
    None.                                                               
  ***********************************************************************/

#define DRV_ADC_ACQUISITION_TIME            ADC_ACQUISITION_TIME_4_TAD


// *****************************************************************************
/* ADC Voltage Reference

  Summary:
    Defines the voltage reference.

  Description:
    This macro defines the voltage reference of the ADC driver. This provides
    static override of the dynamic selection of the voltage reference. If this
    macro is defined, this will be used for setting up the voltage reference
    and not the voltage reference value provided by DRV_ADC_INIT.

  Remarks:
    None.

*/

#define DRV_ADC_VOLTAGE_REFERENCE           ADC_VREF_POS_TO_VDD_VREF_NEG_TO_VSS


// *****************************************************************************
/* ADC Data Output Format

  Summary:
    Defines the data output format.

  Description:
    This macro defines the data output format for the ADC driver. This
    provides static override of the dynamic selection of the data output format.
    If this macro is defined, this will be used for setting up the data output
    format and not the data output format value provided by DRV_ADC_INIT.

  Remarks:
    None.

*/

#define DRV_ADC_RESULT_FORMAT          ADC_RESULT_FORMAT_INTEGER_16BIT


// *****************************************************************************
/* Conversion Trigger Source

  Summary:
    Defines the conversion trigger source.

  Description:
    This macro defines the conversion trigger source for the ADC driver. This
    provides static override of the dynamic selection of the conversion trigger
    source. If this macro is defined, this will be used for setting up the
    conversion trigger source and not the conversion trigger source value
    provided by DRV_ADC_INIT.

  Remarks:
    None.

*/

#define DRV_ADC_CONVERSION_TRIGGER_SOURCE   ADC_CONVERSION_TRIGGER_INTERNAL_COUNT


// *****************************************************************************
/* ADC Conversion Clock Source

  Summary:
    Defines the conversion clock source.

  Description:
    This macro defines the conversion clock source for the ADC driver. This
    provides static override of the dynamic selection of the conversion clock
    source. If this macro is defined, this will be used for setting up the
    conversion clock source and not the conversion clock source value provided
    by DRV_ADC_INIT.

  Remarks:
    None.

*/

#define DRV_ADC_CONVERSION_CLOCK_SOURCE     ADC_CLOCK_SRC_SYSTEM_CLOCK


// *****************************************************************************
/* ADC Conversion Clock

  Summary:
    Defines the conversion clock.

  Description:
    This macro defines the conversion clock for the ADC driver. This provides
    static override of the dynamic selection of the conversion clock. If this
    macro is defined, this will be used for setting up the conversion clock
    and not the conversion clock value provided by DRV_ADC_INIT.

  Remarks:
    None.
*/

#define DRV_ADC_CONVERSION_CLOCK_PRESCALER  ADC_CONV_CLOCK_4_TCY


// *****************************************************************************
/* ADC Analog input channel

  Summary:
    Defines the analog input channel.

  Description:
    This macro defines the analog input channel for the ADC driver. This
    provides static override of the dynamic selection of the analog input. If
    this macro is defined, this will be used for setting up the analog input
    and not the analog input value provided by DRV_ADC_INIT.

  Remarks:
    None.
*/

#define DRV_ADC_ANALOG_INPUT                ADC_INPUT_AN2


// *****************************************************************************
/* ADC power state configuration

  Summary:
    Controls the power state of the ADC.

  Description:
    This macro controls the power state of the ADC.

  Remarks:
    Note: This feature may not be available in the device or the ADC module
    selected.
*/

#define DRV_ADC_POWER_STATE                 SYS_MODULE_POWER_IDLE_STOP


#endif // _DRV_ADC_CONFIG_TEMPLATE_H

/*******************************************************************************
 End of File
*/

