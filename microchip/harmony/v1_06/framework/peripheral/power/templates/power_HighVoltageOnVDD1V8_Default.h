/*******************************************************************************
  POWER Peripheral Library Template Implementation

  File Name:
    power_HighVoltageOnVDD1V8_Default.h

  Summary:
    POWER PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : HighVoltageOnVDD1V8
    and its Variant : Default
    For following APIs :
        PLIB_POWER_ExistsHighVoltageOnVDD1V8
        PLIB_POWER_HighVoltageOnVDD1V8HasOccurred

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _POWER_HIGHVOLTAGEONVDD1V8_DEFAULT_H
#define _POWER_HIGHVOLTAGEONVDD1V8_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _POWER_HVD_ON_DDR2_VREG(index)

  MASKs: 
    _POWER_HVD_ON_DDR2_MASK(index)

  POSs: 
    _POWER_HVD_ON_DDR2_POS(index)

  LENs: 
    _POWER_HVD_ON_DDR2_LEN(index)

*/


//******************************************************************************
/* Function :  POWER_ExistsHighVoltageOnVDD1V8_Default

  Summary:
    Implements Default variant of PLIB_POWER_ExistsHighVoltageOnVDD1V8

  Description:
    This template implements the Default variant of the PLIB_POWER_ExistsHighVoltageOnVDD1V8 function.
*/

#define PLIB_POWER_ExistsHighVoltageOnVDD1V8 PLIB_POWER_ExistsHighVoltageOnVDD1V8
PLIB_TEMPLATE bool POWER_ExistsHighVoltageOnVDD1V8_Default( POWER_MODULE_ID index )
{
    return true;
}


//******************************************************************************
/* Function :  POWER_HighVoltageOnVDD1V8HasOccurred_Default

  Summary:
    Implements Default variant of PLIB_POWER_HighVoltageOnVDD1V8HasOccurred 

  Description:
    This template implements the Default variant of the PLIB_POWER_HighVoltageOnVDD1V8HasOccurred function.
*/

PLIB_TEMPLATE bool POWER_HighVoltageOnVDD1V8HasOccurred_Default( POWER_MODULE_ID index )
{
   	return (bool)(_SFR_BIT_READ(_POWER_HVD_ON_DDR2_VREG(index), _POWER_HVD_ON_DDR2_POS(index)));
}


#endif /*_POWER_HIGHVOLTAGEONVDD1V8_DEFAULT_H*/

/******************************************************************************
 End of File
*/

