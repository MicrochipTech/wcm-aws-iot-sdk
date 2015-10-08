/*******************************************************************************
  ADC Peripheral Library Template Implementation

  File Name:
    adc_ResultGetByIndex_Default.h

  Summary:
    ADC PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : ResultGetByIndex
    and its Variant : Default
    For following APIs :
        PLIB_ADC_ResultGetByIndex
        PLIB_ADC_ExistsResultGetByIndex

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

#ifndef _ADC_RESULTGETBYINDEX_DEFAULT_H
#define _ADC_RESULTGETBYINDEX_DEFAULT_H


//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _ADC_RESULT_GETBY_INDEX_0_VREG(index)
    _ADC_RESULT_GETBY_INDEX_1_VREG(index)
    _ADC_RESULT_GETBY_INDEX_2_VREG(index)
    _ADC_RESULT_GETBY_INDEX_3_VREG(index)
    _ADC_RESULT_GETBY_INDEX_4_VREG(index)
    _ADC_RESULT_GETBY_INDEX_5_VREG(index)
    _ADC_RESULT_GETBY_INDEX_6_VREG(index)
    _ADC_RESULT_GETBY_INDEX_7_VREG(index)
    _ADC_RESULT_GETBY_INDEX_8_VREG(index)
    _ADC_RESULT_GETBY_INDEX_9_VREG(index)
    _ADC_RESULT_GETBY_INDEX_A_VREG(index)
    _ADC_RESULT_GETBY_INDEX_B_VREG(index)
    _ADC_RESULT_GETBY_INDEX_C_VREG(index)
    _ADC_RESULT_GETBY_INDEX_D_VREG(index)
    _ADC_RESULT_GETBY_INDEX_E_VREG(index)
    _ADC_RESULT_GETBY_INDEX_F_VREG(index)

  MASKs: 
    _ADC_RESULT_GETBY_INDEX_0_MASK(index)
    _ADC_RESULT_GETBY_INDEX_1_MASK(index)
    _ADC_RESULT_GETBY_INDEX_2_MASK(index)
    _ADC_RESULT_GETBY_INDEX_3_MASK(index)
    _ADC_RESULT_GETBY_INDEX_4_MASK(index)
    _ADC_RESULT_GETBY_INDEX_5_MASK(index)
    _ADC_RESULT_GETBY_INDEX_6_MASK(index)
    _ADC_RESULT_GETBY_INDEX_7_MASK(index)
    _ADC_RESULT_GETBY_INDEX_8_MASK(index)
    _ADC_RESULT_GETBY_INDEX_9_MASK(index)
    _ADC_RESULT_GETBY_INDEX_A_MASK(index)
    _ADC_RESULT_GETBY_INDEX_B_MASK(index)
    _ADC_RESULT_GETBY_INDEX_C_MASK(index)
    _ADC_RESULT_GETBY_INDEX_D_MASK(index)
    _ADC_RESULT_GETBY_INDEX_E_MASK(index)
    _ADC_RESULT_GETBY_INDEX_F_MASK(index)

  POSs: 
    _ADC_RESULT_GETBY_INDEX_0_POS(index)
    _ADC_RESULT_GETBY_INDEX_1_POS(index)
    _ADC_RESULT_GETBY_INDEX_2_POS(index)
    _ADC_RESULT_GETBY_INDEX_3_POS(index)
    _ADC_RESULT_GETBY_INDEX_4_POS(index)
    _ADC_RESULT_GETBY_INDEX_5_POS(index)
    _ADC_RESULT_GETBY_INDEX_6_POS(index)
    _ADC_RESULT_GETBY_INDEX_7_POS(index)
    _ADC_RESULT_GETBY_INDEX_8_POS(index)
    _ADC_RESULT_GETBY_INDEX_9_POS(index)
    _ADC_RESULT_GETBY_INDEX_A_POS(index)
    _ADC_RESULT_GETBY_INDEX_B_POS(index)
    _ADC_RESULT_GETBY_INDEX_C_POS(index)
    _ADC_RESULT_GETBY_INDEX_D_POS(index)
    _ADC_RESULT_GETBY_INDEX_E_POS(index)
    _ADC_RESULT_GETBY_INDEX_F_POS(index)

  LENs: 
    _ADC_RESULT_GETBY_INDEX_0_LEN(index)
    _ADC_RESULT_GETBY_INDEX_1_LEN(index)
    _ADC_RESULT_GETBY_INDEX_2_LEN(index)
    _ADC_RESULT_GETBY_INDEX_3_LEN(index)
    _ADC_RESULT_GETBY_INDEX_4_LEN(index)
    _ADC_RESULT_GETBY_INDEX_5_LEN(index)
    _ADC_RESULT_GETBY_INDEX_6_LEN(index)
    _ADC_RESULT_GETBY_INDEX_7_LEN(index)
    _ADC_RESULT_GETBY_INDEX_8_LEN(index)
    _ADC_RESULT_GETBY_INDEX_9_LEN(index)
    _ADC_RESULT_GETBY_INDEX_A_LEN(index)
    _ADC_RESULT_GETBY_INDEX_B_LEN(index)
    _ADC_RESULT_GETBY_INDEX_C_LEN(index)
    _ADC_RESULT_GETBY_INDEX_D_LEN(index)
    _ADC_RESULT_GETBY_INDEX_E_LEN(index)
    _ADC_RESULT_GETBY_INDEX_F_LEN(index)

*/


//******************************************************************************
/* Function :  ADC_ResultGetByIndex_Default

  Summary:
    Implements Default variant of PLIB_ADC_ResultGetByIndex 

  Description:
    This template implements the Default variant of the PLIB_ADC_ResultGetByIndex function.
*/

PLIB_TEMPLATE ADC_SAMPLE ADC_ResultGetByIndex_Default( ADC_MODULE_ID index , uint8_t bufferIndex )
{
    SFR_TYPE *adcResult = _ADC_RESULT_GETBY_INDEX_0_VREG(index);

    return _SFR_READ(&adcResult[bufferIndex * 4]);
}


//******************************************************************************
/* Function :  ADC_ExistsResultGetByIndex_Default

  Summary:
    Implements Default variant of PLIB_ADC_ExistsResultGetByIndex

  Description:
    This template implements the Default variant of the PLIB_ADC_ExistsResultGetByIndex function.
*/

#define PLIB_ADC_ExistsResultGetByIndex PLIB_ADC_ExistsResultGetByIndex
PLIB_TEMPLATE bool ADC_ExistsResultGetByIndex_Default( ADC_MODULE_ID index )
{
    return true;
}


#endif /*_ADC_RESULTGETBYINDEX_DEFAULT_H*/

/******************************************************************************
 End of File
*/

