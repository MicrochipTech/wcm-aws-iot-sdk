/*******************************************************************************
  ADC Peripheral Library Template Implementation

  File Name:
    adc_StopInIdle_Default.h

  Summary:
    ADC PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : StopInIdle
    and its Variant : Default
    For following APIs :
        PLIB_ADC_StopInIdleEnable
        PLIB_ADC_StopInIdleDisable
        PLIB_ADC_ExistsStopInIdleControl

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

#ifndef _ADC_STOPINIDLE_DEFAULT_H
#define _ADC_STOPINIDLE_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _ADC_STOP_IN_IDLE_VREG(index)

  MASKs: 
    _ADC_STOP_IN_IDLE_MASK(index)

  POSs: 
    _ADC_STOP_IN_IDLE_POS(index)

  LENs: 
    _ADC_STOP_IN_IDLE_LEN(index)

*/


//******************************************************************************
/* Function :  ADC_StopInIdleEnable_Default

  Summary:
    Implements Default variant of PLIB_ADC_StopInIdleEnable 

  Description:
    This template implements the Default variant of the PLIB_ADC_StopInIdleEnable function.
*/

PLIB_TEMPLATE void ADC_StopInIdleEnable_Default( ADC_MODULE_ID index )
{
    _SFR_BIT_SET(_ADC_STOP_IN_IDLE_VREG(index),
        _ADC_STOP_IN_IDLE_POS(index) );
}


//******************************************************************************
/* Function :  ADC_StopInIdleDisable_Default

  Summary:
    Implements Default variant of PLIB_ADC_StopInIdleDisable 

  Description:
    This template implements the Default variant of the PLIB_ADC_StopInIdleDisable function.
*/

PLIB_TEMPLATE void ADC_StopInIdleDisable_Default( ADC_MODULE_ID index )
{
    _SFR_BIT_CLEAR(_ADC_STOP_IN_IDLE_VREG(index),
        _ADC_STOP_IN_IDLE_POS(index) );
}


//******************************************************************************
/* Function :  ADC_ExistsStopInIdleControl_Default

  Summary:
    Implements Default variant of PLIB_ADC_ExistsStopInIdleControl

  Description:
    This template implements the Default variant of the PLIB_ADC_ExistsStopInIdleControl function.
*/

#define PLIB_ADC_ExistsStopInIdleControl PLIB_ADC_ExistsStopInIdleControl
PLIB_TEMPLATE bool ADC_ExistsStopInIdleControl_Default( ADC_MODULE_ID index )
{
    return true;
}


#endif /*_ADC_STOPINIDLE_DEFAULT_H*/

/******************************************************************************
 End of File
*/

