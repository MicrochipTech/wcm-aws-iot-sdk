/*******************************************************************************
  ADC Peripheral Library Template Implementation

  File Name:
    adc_CalibrationControl_Default.h

  Summary:
    ADC PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : CalibrationControl
    and its Variant : Default
    For following APIs :
        PLIB_ADC_CalibrationEnable
        PLIB_ADC_CalibrationDisable
        PLIB_ADC_ExistsCalibrationControl

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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

#ifndef _ADC_CALIBRATIONCONTROL_DEFAULT_H
#define _ADC_CALIBRATIONCONTROL_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _ADC_CALIBRATION_CONTROL_VREG(index)

  MASKs: 
    _ADC_CALIBRATION_CONTROL_MASK(index)

  POSs: 
    _ADC_CALIBRATION_CONTROL_POS(index)

  LENs: 
    _ADC_CALIBRATION_CONTROL_LEN(index)

*/


//******************************************************************************
/* Function :  ADC_CalibrationEnable_Default

  Summary:
    Implements Default variant of PLIB_ADC_CalibrationEnable 

  Description:
    This template implements the Default variant of the PLIB_ADC_CalibrationEnable function.
*/

PLIB_TEMPLATE void ADC_CalibrationEnable_Default( ADC_MODULE_ID index )
{
    _SFR_BIT_SET(_ADC_CALIBRATION_CONTROL_VREG(index),
        _ADC_CALIBRATION_CONTROL_POS(index) );
}


//******************************************************************************
/* Function :  ADC_CalibrationDisable_Default

  Summary:
    Implements Default variant of PLIB_ADC_CalibrationDisable 

  Description:
    This template implements the Default variant of the PLIB_ADC_CalibrationDisable function.
*/

PLIB_TEMPLATE void ADC_CalibrationDisable_Default( ADC_MODULE_ID index )
{
    _SFR_BIT_CLEAR(_ADC_CALIBRATION_CONTROL_VREG(index),
        _ADC_CALIBRATION_CONTROL_POS(index) );
}


//******************************************************************************
/* Function :  ADC_ExistsCalibrationControl_Default

  Summary:
    Implements Default variant of PLIB_ADC_ExistsCalibrationControl

  Description:
    This template implements the Default variant of the PLIB_ADC_ExistsCalibrationControl function.
*/

#define PLIB_ADC_ExistsCalibrationControl PLIB_ADC_ExistsCalibrationControl
PLIB_TEMPLATE bool ADC_ExistsCalibrationControl_Default( ADC_MODULE_ID index )
{
    return true;
}


#endif /*_ADC_CALIBRATIONCONTROL_DEFAULT_H*/

/******************************************************************************
 End of File
*/

