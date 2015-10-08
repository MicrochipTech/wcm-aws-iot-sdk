/*******************************************************************************
  CMP Peripheral Library Template Implementation

  File Name:
    cmp_InvertingInputSelect_Default.h

  Summary:
    CMP PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : InvertingInputSelect
    and its Variant : Default
    For following APIs :
        PLIB_CMP_ExistsInvertingInputSelect
        PLIB_CMP_InvertingInputChannelSelect

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

#ifndef _CMP_INVERTINGINPUTSELECT_DEFAULT_H
#define _CMP_INVERTINGINPUTSELECT_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are

  VREGs:
    _CMP_INVERTING_INPUT_SELECTION_VREG(index)

  MASKs:
    _CMP_INVERTING_INPUT_SELECTION_MASK(index)

  POSs:
    _CMP_INVERTING_INPUT_SELECTION_POS(index)

  LENs:
    _CMP_INVERTING_INPUT_SELECTION_LEN(index)

*/


//******************************************************************************
/* Function :  CMP_ExistsInvertingInputSelect_Default

  Summary:
    Implements Default variant of PLIB_CMP_ExistsInvertingInputSelect

  Description:
    This template implements the Default variant of the PLIB_CMP_ExistsInvertingInputSelect function.
*/

#define PLIB_CMP_ExistsInvertingInputSelect PLIB_CMP_ExistsInvertingInputSelect
PLIB_TEMPLATE bool CMP_ExistsInvertingInputSelect_Default( CMP_MODULE_ID index )
{
    return true;
}


//******************************************************************************
/* Function :  CMP_InvertingInputChannelSelect _Default

  Summary:
    Implements Default variant of PLIB_CMP_InvertingInputChannelSelect

  Description:
    This template implements the Default variant of the PLIB_CMP_InvertingInputChannelSelect  function.
*/

PLIB_TEMPLATE void CMP_InvertingInputChannelSelect_Default( CMP_MODULE_ID index , CMP_INVERTING_INPUT channel )
{
    _SFR_FIELD_WRITE(_CMP_INVERTING_INPUT_SELECTION_VREG(index), _CMP_INVERTING_INPUT_SELECTION_MASK(index),
	_CMP_INVERTING_INPUT_SELECTION_POS(index), channel);
}


#endif /*_CMP_INVERTINGINPUTSELECT_DEFAULT_H*/

/******************************************************************************
 End of File
*/

