/*******************************************************************************
  TMR Peripheral Library Template Implementation

  File Name:
    tmr_Period32Bit_In16BitRegister.h

  Summary:
    TMR PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : Period32Bit
    and its Variant : In16BitRegister
    For following APIs :
        PLIB_TMR_Period32BitSet
        PLIB_TMR_Period32BitGet
        PLIB_TMR_ExistsPeriod32Bit

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

#ifndef _TMR_PERIOD32BIT_IN16BITREGISTER_H
#define _TMR_PERIOD32BIT_IN16BITREGISTER_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are

  VREGs:
    _TMR_PERIOD_REGISTER_16BIT_LOW_VREG(index)

  MASKs:
    _TMR_PERIOD_REGISTER_16BIT_LOW_MASK(index)

  POSs:
    _TMR_PERIOD_REGISTER_16BIT_LOW_POS(index)

  LENs:
    _TMR_PERIOD_REGISTER_16BIT_LOW_LEN(index)

*/


//******************************************************************************
/* Function :  TMR_Period32BitSet_In16BitRegister

  Summary:
    Implements In16BitRegister variant of PLIB_TMR_Period32BitSet

  Description:
    This template implements the In16BitRegister variant of the PLIB_TMR_Period32BitSet function.
*/

PLIB_TEMPLATE void TMR_Period32BitSet_In16BitRegister( const TMR_MODULE_ID index , uint32_t period )
{
    if( _TMR_PERIOD_MATCH_MODE_VREG( index ) == (SFR_TYPE *)-1 )
    {
        /* Overflow TMR */
        _SFR_WRITE( _TMR_VALUE_REGISTER_16BIT_HIGH_VREG( index + 1 ), _PLIB_ACCESS_DOUBLE_BYTE( ((UINT32_MAX + 1)-(uint32_t)period), 1 ) );
        _SFR_WRITE( _TMR_VALUE_REGISTER_16BIT_LOW_VREG( index ), ((UINT32_MAX + 1)-(uint32_t)period) );
    }
    else
    {
        /* Period Match TMR */
        switch(index)
        {
            case TMR_ID_2:
                _SFR_WRITE( _TMR_PERIOD_REGISTER_16BIT_LOW_VREG( TMR_ID_2 ), period );
                _SFR_WRITE( _TMR_PERIOD_REGISTER_16BIT_HIGH_VREG( TMR_ID_3 ), _PLIB_ACCESS_DOUBLE_BYTE( period , 1 ) );
                break;
            case TMR_ID_4:
                _SFR_WRITE( _TMR_PERIOD_REGISTER_16BIT_LOW_VREG( TMR_ID_4 ), period );
                _SFR_WRITE( _TMR_PERIOD_REGISTER_16BIT_HIGH_VREG( TMR_ID_5 ), _PLIB_ACCESS_DOUBLE_BYTE( period , 1 ) );
                break;
            default:
                break;
        }
    }
}


//******************************************************************************
/* Function :  TMR_Period32BitGet_In16BitRegister

  Summary:
    Implements In16BitRegister variant of PLIB_TMR_Period32BitGet

  Description:
    This template implements the In16BitRegister variant of the PLIB_TMR_Period32BitGet function.
*/

PLIB_TEMPLATE uint32_t TMR_Period32BitGet_In16BitRegister( TMR_MODULE_ID index )
{
    uint32_t val1 = 0, val2 = 0;
    val1 = _SFR_READ( _TMR_PERIOD_REGISTER_16BIT_LOW_VREG( index ) );
    val2 = ((uint32_t)_SFR_READ( _TMR_PERIOD_REGISTER_16BIT_HIGH_VREG( index + 1 ) ) << 16);
    return ( val1 | val2 );
}


//******************************************************************************
/* Function :  TMR_ExistsPeriod32Bit_In16BitRegister

  Summary:
    Implements In16BitRegister variant of PLIB_TMR_ExistsPeriod32Bit

  Description:
    This template implements the In16BitRegister variant of the PLIB_TMR_ExistsPeriod32Bit function.
*/

#define PLIB_TMR_ExistsPeriod32Bit PLIB_TMR_ExistsPeriod32Bit
PLIB_TEMPLATE bool TMR_ExistsPeriod32Bit_In16BitRegister( TMR_MODULE_ID index )
{
    return true;
}


#endif /*_TMR_PERIOD32BIT_IN16BITREGISTER_H*/

/******************************************************************************
 End of File
*/

