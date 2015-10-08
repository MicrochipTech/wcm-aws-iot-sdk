/*******************************************************************************
  Comparator Peripheral Library Interface Header

  Company:
    Microchip Technology Inc.

  File Name:
    plib_cmp.h

  Summary:
    Comparator Peripheral Library Interface Header for Comparator module definitions.

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the Comparator
    Peripheral Library for all families of Microchip microcontrollers. The
    definitions in this file are common to the Comparator peripheral.
*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef _PLIB_CMP_H
#define _PLIB_CMP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files (continued at end of file)
// *****************************************************************************
// *****************************************************************************
/*  This section lists the other files that are included in this file.  However,
    please see the bottom of the file for additional implementation header files
    that are also included
*/

#include "peripheral/cmp/processor/cmp_processor.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Comparator Voltage Reference Configuration
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_Enable ( CMP_CVREF_MOD index )

 Summary:
   Enables the voltage reference of the Comparator module.

 Description:
   This function enables the voltage reference of the Comparator module.

 Precondition:
   The Comparator module should be appropriately configured before being enabled.

 Parameters:
   index    - Identifier for the device instance to be configured

 Returns:
   None.

 Example:
   <code>
    PLIB_CMP_CVREF_Enable ( CMP_ID_1 );
   </code>

 Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFEnableControl
	in your application to determine whether this feature is available.
 */

void PLIB_CMP_CVREF_Enable ( CMP_MODULE_ID index );


//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_Disable ( CMP_MODULE_ID index )

  Summary:
    Disables the voltage reference of the Comparator module.

  Description:
    This function disables the voltage reference of the Comparator module.

  Precondition:
    None.

  Parameters:
    index    - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_Disable ( CMP_ID_1 );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFEnableControl
	in your application to determine whether this feature is available.
 */

void PLIB_CMP_CVREF_Disable ( CMP_MODULE_ID index );


//******************************************************************************
/*  Function:
    void PLIB_CMP_CVREF_OutputEnable ( CMP_MODULE_ID index )

  Summary:
    Enables the voltage output.

  Description:
    This function enables the voltage output

  Precondition:
    None.

  Parameters:
    index    - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_OutputEnable(CMP_ID_1);
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFOutputEnableControl
	in your application to determine whether this feature is available.

 */

void PLIB_CMP_CVREF_OutputEnable(CMP_MODULE_ID index);


//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_OutputDisable ( CMP_MODULE_ID index )

  Summary:
    Disables the output voltage.

  Description:
    This function disables the reference voltage output.

  Precondition:
    None.

  Parameters:
    index    - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_OutputDisable ( CMP_ID_1 );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFOutputEnableControl
	in your application to determine whether this feature is available.
 */

void PLIB_CMP_CVREF_OutputDisable ( CMP_MODULE_ID index );


// *****************************************************************************
// *****************************************************************************
// Section: Comparator Voltage Reference- External Voltage Reference
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_ReferenceVoltageSelect ( CMP_MODULE_ID index,
        CMP_CVREF_REFERENCE_SELECT reference );

  Summary:
    Selects the voltage reference value, CVref.

  Description:
    This function selects the voltage reference value, CVref. This value decides which 
	voltage source should be taken as reference voltage from the 
	set CMP_CVREF_REFERENCE_SELECT.

  Precondition:
    Determine the correct value that should be passed.

  Parameters:
    index    - Identifier for the device instance to be configured
    value    - Select value from CMP_CVREF_REFERENCE_SELECT

  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_ReferenceVoltageSelect ( CMP_ID_1,  CMP_CVREF_RESISTOR_LADDER_VOLTAGE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFRefVoltageRangeSelect
    in your application to determine whether this feature is available.
*/

void PLIB_CMP_CVREF_ReferenceVoltageSelect ( CMP_MODULE_ID index, CMP_CVREF_REFERENCE_SELECT reference );


//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_SourceVoltageSelect ( CMP_MODULE_ID index, CMP_CVREF_VOLTAGE_SOURCE source )

  Summary:
    Connects the Comparator module to the selected voltage source.

  Description:
    This function connects the Comparator module to the selected voltage source.
  Precondition:
    None.

  Parameters:
    index    - Identifier for the device instance to be configured
    source   - Select the voltage source from CMP_CVREF_VOLTAGE_SOURCE

  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_SourceVoltageSelect ( CMP_ID_1, CMP_CVREF_VOLTAGE_SOURCE_INTERNAL );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFVoltageRangeSelect in your application
	to determine whether this feature is available.
 */

void PLIB_CMP_CVREF_SourceVoltageSelect ( CMP_MODULE_ID index, CMP_CVREF_VOLTAGE_SOURCE source );


//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_SourceNegativeInputSelect ( CMP_MODULE_ID index,
									CMP_CVREF_VOLTAGE_SOURCE_NEG_REFERENCE negInput )

  Summary:
    Configures the Comparator module to use the selected input as a negative reference.

  Description:
    This function configures the Comparator module to use the selected input as a negative
    reference for the voltage source.

  Precondition:
    None.

  Parameters:
    index    - Identifier for the device instance to be configured.
    negInput - Select the voltage source negative reference from
			   CMP_CVREF_VOLTAGE_SOURCE_NEG_REFERENCE
  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_SourceNegativeInputSelect ( CMP_ID_1, CMP_CVREF_VOLTAGE_SOURCE_NEG_REF_GROUND );
    </code>

  Remarks:
    Note:  This feature is not available on all devices.  Please refer to the
    specific device data sheet for availability. For such devices, selecting
    the positive source will automatically select the negative input.
 */

void PLIB_CMP_CVREF_SourceNegativeInputSelect ( CMP_MODULE_ID index,
								CMP_CVREF_VOLTAGE_SOURCE_NEG_REFERENCE negInput );


//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_WideRangeEnable ( CMP_MODULE_ID index )

  Summary:
    Enables the wide range.

  Description:
    This function enables the wide range for reference voltage. The voltage range starts
    from zero if the wide range is selected.

  Precondition:
    None.

  Parameters:
    index    - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_WideRangeEnable(CMP_ID_1);
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFWideRangeControl
	in your application to determine whether this feature is available.
 */

void PLIB_CMP_CVREF_WideRangeEnable ( CMP_MODULE_ID index );


//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_WideRangeDisable ( CMP_MODULE_ID index )

  Summary:
    Disables the wide range.

  Description:
    This function disables the wide range for reference voltage. The range of possible
    voltages will become narrower, and finer voltage options can be achieved
    in this case.

  Precondition:
    None.

  Parameters:
    index    - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_WideRangeDisable ( CMP_ID_1 );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFWideRangeControl
	in your application to determine whether this feature is available.
 */

void PLIB_CMP_CVREF_WideRangeDisable ( CMP_MODULE_ID index );


//******************************************************************************
/* Function:
    bool PLIB_CMP_CVREF_WideRangeIsEnabled ( CMP_MODULE_ID index );

  Summary:
    Returns whether the wide range is selected for the reference voltage.

  Description:
    This function returns whether the wide range is enabled.

  Precondition:
    None.

  Parameters:
    index    - Identifier for the device instance to be configured

  Returns:
             - true  = The wide range is enabled
             - false = The wide range is not enabled

  Example:
    <code>
    bool range;

    range = PLIB_CMP_CVREF_WideRangeIsEnabled ( MY_CMP_CVREF_ID );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFWideRangeControl
	in your application to determine whether this feature is available.
 */

bool PLIB_CMP_CVREF_WideRangeIsEnabled ( CMP_MODULE_ID index );


//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_ValueSelect ( CMP_MODULE_ID index,
        CMP_CVREF_VALUE value );

  Summary:
    Selects the voltage reference value.

  Description:
    This function selects the voltage reference value. This value decides how
    many resistance units will be added and therefore, decides the output voltage.

  Precondition:
    Determine the correct value that should be passed.

  Parameters:
    index    - Identifier for the device instance to be configured
    value    - Select value from CMP_CVREF_VALUE

  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_ValueSelect ( CMP_ID_1,  CMP_CVREF_VALUE_13 );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFValueSelect
	in your application to determine whether this feature is available.
 */

void PLIB_CMP_CVREF_ValueSelect ( CMP_MODULE_ID index, CMP_CVREF_VALUE value );


//******************************************************************************
/* Function:
    void PLIB_CMP_CVREF_BandGapReferenceSourceSelect ( CMP_MODULE_ID index,
        CMP_CVREF_BANDGAP_SELECT bandGap );

  Summary:
    Selects the band gap reference voltage source.

  Description:
    This function selects the band gap reference voltage source from the
    available options from CMP_CVREF_BANDGAP_SELECT.

  Precondition:
    None.

  Parameters:
    index    - Identifier for the device instance to be configured
    select   - Select a band gap reference source from CMP_CVREF_BANDGAP_SELECT

  Returns:
    None.

  Example:
    <code>
    PLIB_CMP_CVREF_BandGapReferenceSourceSelect ( CMP_ID_1,  CMP_CVREF_BANDGAP_0_6V );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_CVREF_ExistsCVREFBGRefVoltageRangeSelect
	in your application to determine whether this feature is available.
*/
void PLIB_CMP_CVREF_BandGapReferenceSourceSelect ( CMP_MODULE_ID index,
											CMP_CVREF_BANDGAP_SELECT select );


// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - Comparator Control
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void PLIB_CMP_Enable ( CMP_MODULE_ID index )

  Summary:
    Enables the Comparator module.

  Description:
    This function enables (i.e., turns ON) the selected Comparator module.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_Enable ( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsEnableControl
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_Enable ( CMP_MODULE_ID index );


// *****************************************************************************
/* Function:
    void PLIB_CMP_Disable ( CMP_MODULE_ID index )

  Summary:
    Disables the Comparator module.

  Description:
    This function disables (i.e., turns OFF) the selected Comparator module.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_Disable ( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsEnableControl
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_Disable ( CMP_MODULE_ID index );


// *****************************************************************************
/* Function:
    void PLIB_CMP_OutputEnable ( CMP_MODULE_ID index )

  Summary:
    Enables the Comparator output.

  Description:
    This function enables (i.e., turns ON) the Comparator output.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_OutputEnable( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsOutputEnableControl
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_OutputEnable ( CMP_MODULE_ID index );


// *****************************************************************************
/* Function:
    void PLIB_CMP_OutputDisable ( CMP_MODULE_ID index )

  Summary:
    Disables the Comparator output.

  Description:
    This function disables (i.e., turns OFF) the Comparator output.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_OutputDisable ( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsOutputEnableControl
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_OutputDisable ( CMP_MODULE_ID index );


// *****************************************************************************
/* Function:
    void PLIB_CMP_OutputInvertEnable (CMP_MODULE_ID index)

  Summary:
    Comparator output is inverted.

  Description:
    Calling this function will set the comparator to make it's output inverted.

  Precondition:
    None.

  Parameters:
    index     - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_OutputDisable ( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsOutputEnableControlExistsInvertOutputControl
	in your application to determine whether this feature is
    available.

	Setting this bit will invert the signal to the comparator interrupt generator
	as well. This will result in an interrupt being generated on the opposite edge
	from the one selected by PLIB_CMP_InterruptEventSelect function.
*/

void PLIB_CMP_OutputInvertEnable (CMP_MODULE_ID index);


// *****************************************************************************
/* Function:
    void PLIB_CMP_OutputInvertDisable ( CMP_MODULE_ID index )

  Summary:
    Comparator output is non-inverted.

  Description:
    This function will select the non-inverted comparator output.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_OutputInvertDisable ( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsOutputEnableControlExistsInvertOutputControl
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_OutputInvertDisable ( CMP_MODULE_ID index );


// *****************************************************************************
/* Function:
    void PLIB_CMP_OutputLogicHigh ( CMP_MODULE_ID index )

  Summary:
    Comparator output bit will give a 'logic high' on satisfying the
	input condition.

  Description:
    Calling this API will set the Comparator module to output a 'logic high' on
	satisfying the input condition.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_OutputLogicHigh ( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsOutputLevelControl
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_OutputLogicHigh ( CMP_MODULE_ID index );


// *****************************************************************************
/* Function:
    void PLIB_CMP_OutputLogicLow ( CMP_MODULE_ID index )

  Summary:
    Comparator will be set to give 'logic low' on satisfying the input condition.

  Description:
    This function will set the Comparator to give 'logic low' on satisfying
	the input condition.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_OutputLogicLow( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsOutputLevelControl
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_OutputLogicLow ( CMP_MODULE_ID index );


// *****************************************************************************
/* Function:
    void PLIB_CMP_InterruptEventSelect ( CMP_MODULE_ID index, CMP_INTERRUPT_EVENT event )

  Summary:
    Comparator interrupt event select.

  Description:
    This function will select when the Comparator interrupt should occur.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured
    event           - One of the possible values from CMP_INTERRUPT_EVENT
  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    // CMP_INTERRUPT_EVENT - CMP_LOW_TO_HIGH
    PLIB_CMP_InterruptEventSelect ( MY_CMP_INSTANCE,
    						CMP_INTERRUPT_GENERATION_LOW_TO_HIGH );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsInterruptEventSelect
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_InterruptEventSelect ( CMP_MODULE_ID index, CMP_INTERRUPT_EVENT event );


// *****************************************************************************
/* Function:
    void PLIB_CMP_InvertingInputChannelSelect ( CMP_MODULE_ID index,
												CMP_INVERTING_INPUT channel )

  Summary:
    Comparator inverting input channel select.

  Description:
    This function will select the inverting input channels for the Comparator.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured
    channel         - One of the possible values from CMP_INVERTING_INPUT
  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
     PLIB_CMP_InvertingInputChannelSelect ( MY_CMP_INSTANCE,
								CMP_INVERTING_INPUT_IVREF );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsInvertingInputSelect
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_InvertingInputChannelSelect ( CMP_MODULE_ID index, CMP_INVERTING_INPUT channel );


// *****************************************************************************
/* Function:
    void PLIB_CMP_NonInvertingInputChannelSelect ( CMP_MODULE_ID index,
											CMP_NON_INVERTING_INPUT input )

  Summary:
    Comparator input channel select.

  Description:
    This function will select the non-inverting input channels for the Comparator.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured
    channel         - One of the possible values from CMP_NON_INVERTING_INPUT
  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_NonInvertingInputChannelSelect ( MY_CMP_INSTANCE, CMP_NON_INVERTING_INPUT_CVREF );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsNonInvertingInputSelect
	in your application to determine whether this feature is
    available.s
*/

void PLIB_CMP_NonInvertingInputChannelSelect ( CMP_MODULE_ID index,
												CMP_NON_INVERTING_INPUT input );


// *****************************************************************************
/* Function:
    void PLIB_CMP_StopInIdleModeEnable ( CMP_MODULE_ID index )

  Summary:
    Enables Stop in Idle mode.

  Description:
    This function will discontinue operation of all comparators when the device
	enters Idle mode.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_StopInIdleModeEnable ( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsStopInIdle
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_StopInIdleModeEnable ( CMP_MODULE_ID index );


// *****************************************************************************
/* Function:
    void PLIB_CMP_StopInIdleModeDisable ( CMP_MODULE_ID index )

  Summary:
    Disables Stop in Idle mode.

  Description:
    This function will continue operation of all enabled comparators when the
	device enters Idle mode.

  Precondition:
    None.

  Parameters:
    index           - Identifier for the device instance to be configured

  Returns:
    None.

  Example:
    <code>
	#define MY_CMP_INSTANCE   CMP_ID_1

    // Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
    // application developer.
    PLIB_CMP_StopInIdleModeDisable ( MY_CMP_INSTANCE );
    </code>

  Remarks:
    Note: This feature may not be available on all devices.
	Please refer to the specific device data sheet to determine
	availability or use PLIB_CMP_ExistsStopInIdle
	in your application to determine whether this feature is
    available.
*/

void PLIB_CMP_StopInIdleModeDisable ( CMP_MODULE_ID index );


// *****************************************************************************
/* Function:
   bool PLIB_CMP_OutputStatusGet ( CMP_MODULE_ID index )

 Summary:
   Comparator output status.

 Description:
   This function will return the current status of the Comparator output.

 Precondition:
   None.

 Parameters:
   index           - Identifier for the device instance to be configured

 Returns:
   true  - The status flag is set
   false - The status flag is clear

 Example:
   <code>
	#define MY_CMP_INSTANCE   CMP_ID_1
	bool cmp_status;

	// Where MY_CMP_INSTANCE, is the Comparator instance selected for use by the
	// application developer.
   cmp_status=PLIB_CMP_OutputStatusGet ( MY_CMP_INSTANCE );
   </code>

 Remarks:
   Note: This feature is not available on all devices. Please refer to the
   specific device data sheet for availability.
*/

bool PLIB_CMP_OutputStatusGet ( CMP_MODULE_ID index );


// *****************************************************************************
// *****************************************************************************
// Section: CMP Peripheral Library Exists API Routines
// *****************************************************************************
// *****************************************************************************
/* The functions below indicate the existence of the features on the device.
*/

//******************************************************************************
/* Function :  PLIB_CMP_ExistsEnableControl( CMP_MODULE_ID index )

  Summary:
    Identifies whether the ComparatorEnableControl feature exists on the CMP module

  Description:
    This function identifies whether the ComparatorEnableControl feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_Enable
    - PLIB_CMP_Disable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The ComparatorEnableControl feature is supported on the device
    - false  - The ComparatorEnableControl feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsEnableControl( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsOutputEnableControl( CMP_MODULE_ID index )

  Summary:
    Identifies whether the ComparatorOutputEnableControl feature exists on the CMP module

  Description:
    This function identifies whether the ComparatorOutputEnableControl feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_OutputEnable
    - PLIB_CMP_OutputDisable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The ComparatorOutputEnableControl feature is supported on the device
    - false  - The ComparatorOutputEnableControl feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsOutputEnableControl( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsInvertOutputControl( CMP_MODULE_ID index )

  Summary:
    Identifies whether the InvertOutputSelectControl feature exists on the CMP module

  Description:
    This function identifies whether the InvertOutputSelectControl feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_OutputInvertEnable
    - PLIB_CMP_OutputInvertDisable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The InvertOutputSelectControl feature is supported on the device
    - false  - The InvertOutputSelectControl feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsInvertOutputControl( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsOutputLevelControl( CMP_MODULE_ID index )

  Summary:
    Identifies whether the OutputLevelSelectControl feature exists on the CMP module

  Description:
    This function identifies whether the OutputLevelSelectControl feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_OutputLogicHigh
    - PLIB_CMP_OutputLogicLow

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The OutputLevelSelectControl feature is supported on the device
    - false  - The OutputLevelSelectControl feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsOutputLevelControl( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsOutputStatusGet( CMP_MODULE_ID index )

  Summary:
    Identifies whether the OutputStatusGet feature exists on the CMP module

  Description:
    This function identifies whether the OutputStatusGet feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_OutputStatusGet

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The OutputStatusGet feature is supported on the device
    - false  - The OutputStatusGet feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsOutputStatusGet( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsInterruptEventSelect( CMP_MODULE_ID index )

  Summary:
    Identifies whether the InterruptEventSelect feature exists on the CMP module

  Description:
    This function identifies whether the InterruptEventSelect feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_InterruptEventSelect

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The InterruptEventSelect feature is supported on the device
    - false  - The InterruptEventSelect feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsInterruptEventSelect( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsNonInvertingInputSelect( CMP_MODULE_ID index )

  Summary:
    Identifies whether the NonInvertingInputSelect feature exists on the CMP module

  Description:
    This function identifies whether the NonInvertingInputSelect feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_NonInvertingInputChannelSelect

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The NonInvertingInputSelect feature is supported on the device
    - false  - The NonInvertingInputSelect feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsNonInvertingInputSelect( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsInvertingInputSelect( CMP_MODULE_ID index )

  Summary:
    Identifies whether the InvertingInputSelect feature exists on the CMP module

  Description:
    This function identifies whether the InvertingInputSelect feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_InvertingInputChannelSelect

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The InvertingInputSelect feature is supported on the device
    - false  - The InvertingInputSelect feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsInvertingInputSelect( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsStopInIdle( CMP_MODULE_ID index )

  Summary:
    Identifies whether the StopInIdle feature exists on the CMP module

  Description:
    This function identifies whether the StopInIdle feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_StopInIdleModeEnable
    - PLIB_CMP_StopInIdleModeDisable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The StopInIdle feature is supported on the device
    - false  - The StopInIdle feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsStopInIdle( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsCVREFEnableControl( CMP_MODULE_ID index )

  Summary:
    Identifies whether the CVREFEnableControl feature exists on the CMP module

  Description:
    This function identifies whether the CVREFEnableControl feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_CVREF_Enable
    - PLIB_CMP_CVREF_Disable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The CVREFEnableControl feature is supported on the device
    - false  - The CVREFEnableControl feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsCVREFEnableControl( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsCVREFOutputEnableControl( CMP_MODULE_ID index )

  Summary:
    Identifies whether the CVREFOutputEnableControl feature exists on the CMP module

  Description:
    This function identifies whether the CVREFOutputEnableControl feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_CVREF_OutputEnable
    - PLIB_CMP_CVREF_OutputDisable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The CVREFOutputEnableControl feature is supported on the device
    - false  - The CVREFOutputEnableControl feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsCVREFOutputEnableControl( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsCVREFWideRangeControl( CMP_MODULE_ID index )

  Summary:
    Identifies whether the CVREFWideRangeControl feature exists on the CMP module

  Description:
    This function identifies whether the CVREFWideRangeControl feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_CVREF_WideRangeEnable
    - PLIB_CMP_CVREF_WideRangeDisable
    - PLIB_CMP_CVREF_WideRangeIsEnabled

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The CVREFWideRangeControl feature is supported on the device
    - false  - The CVREFWideRangeControl feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsCVREFWideRangeControl( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsCVREFVoltageRangeSelect( CMP_MODULE_ID index )

  Summary:
    Identifies whether the CVREFVoltageRangeSelect feature exists on the CMP module

  Description:
    This function identifies whether the CVREFVoltageRangeSelect feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_CVREF_SourceVoltageSelect

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The CVREFVoltageRangeSelect feature is supported on the device
    - false  - The CVREFVoltageRangeSelect feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsCVREFVoltageRangeSelect( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsCVREFRefVoltageRangeSelect( CMP_MODULE_ID index )

  Summary:
    Identifies whether the CVREFRefVoltageRangeSelect feature exists on the CMP module

  Description:
    This function identifies whether the CVREFRefVoltageRangeSelect feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_CVREF_ReferenceVoltageSelect

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The CVREFRefVoltageRangeSelect feature is supported on the device
    - false  - The CVREFRefVoltageRangeSelect feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsCVREFRefVoltageRangeSelect( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsCVREFBGRefVoltageRangeSelect( CMP_MODULE_ID index )

  Summary:
    Identifies whether the CVREFBGRefVoltageRangeSelect feature exists on the CMP module

  Description:
    This function identifies whether the CVREFBGRefVoltageRangeSelect feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_CVREF_BandGapReferenceSourceSelect

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The CVREFBGRefVoltageRangeSelect feature is supported on the device
    - false  - The CVREFBGRefVoltageRangeSelect feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsCVREFBGRefVoltageRangeSelect( CMP_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_CMP_ExistsCVREFValueSelect( CMP_MODULE_ID index )

  Summary:
    Identifies whether the CVREFValueSelect feature exists on the CMP module

  Description:
    This function identifies whether the CVREFValueSelect feature is available on the CMP module.
    When this function returns true, these functions are supported on the device:
    - PLIB_CMP_CVREF_ValueSelect

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The CVREFValueSelect feature is supported on the device
    - false  - The CVREFValueSelect feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_CMP_ExistsCVREFValueSelect( CMP_MODULE_ID index );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef _PLIB_CMP_H
/*******************************************************************************
 End of File
*/

