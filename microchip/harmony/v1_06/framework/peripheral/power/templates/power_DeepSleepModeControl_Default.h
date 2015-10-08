/*******************************************************************************
  POWER Peripheral Library Template Implementation

  File Name:
    power_DeepSleepModeControl_Default.h

  Summary:
    POWER PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : DeepSleepModeControl
    and its Variant : Default
    For following APIs :
        PLIB_POWER_ExistsDeepSleepMode
        PLIB_POWER_DeepSleepModeEnable
        PLIB_POWER_DeepSleepModeDisable
        PLIB_POWER_DeepSleepModeIsEnabled

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

#ifndef _POWER_DEEPSLEEPMODECONTROL_DEFAULT_H
#define _POWER_DEEPSLEEPMODECONTROL_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _POWER_DEEP_SLEEP_ENABLE_VREG(index)

  MASKs: 
    _POWER_DEEP_SLEEP_ENABLE_MASK(index)

  POSs: 
    _POWER_DEEP_SLEEP_ENABLE_POS(index)

  LENs: 
    _POWER_DEEP_SLEEP_ENABLE_LEN(index)

*/


//******************************************************************************
/* Function :  POWER_ExistsDeepSleepMode_Default

  Summary:
    Implements Default variant of PLIB_POWER_ExistsDeepSleepMode

  Description:
    This template implements the Default variant of the PLIB_POWER_ExistsDeepSleepMode function.
*/

#define PLIB_POWER_ExistsDeepSleepMode PLIB_POWER_ExistsDeepSleepMode
PLIB_TEMPLATE bool POWER_ExistsDeepSleepMode_Default( POWER_MODULE_ID index )
{
    return true;
}


//******************************************************************************
/* Function :  POWER_DeepSleepModeEnable_Default

  Summary:
    Implements Default variant of PLIB_POWER_DeepSleepModeEnable 

  Description:
    This template implements the Default variant of the PLIB_POWER_DeepSleepModeEnable function.
*/

PLIB_TEMPLATE void POWER_DeepSleepModeEnable_Default( POWER_MODULE_ID index )
{
   _SFR_BIT_SET(_POWER_DEEP_SLEEP_ENABLE_VREG(index),_POWER_DEEP_SLEEP_ENABLE_POS(index));
}


//******************************************************************************
/* Function :  POWER_DeepSleepModeDisable_Default

  Summary:
    Implements Default variant of PLIB_POWER_DeepSleepModeDisable 

  Description:
    This template implements the Default variant of the PLIB_POWER_DeepSleepModeDisable function.
*/

PLIB_TEMPLATE void POWER_DeepSleepModeDisable_Default( POWER_MODULE_ID index )
{
     _SFR_BIT_CLEAR(_POWER_DEEP_SLEEP_ENABLE_VREG(index),_POWER_DEEP_SLEEP_ENABLE_POS(index));
}


//******************************************************************************
/* Function :  POWER_DeepSleepModeIsEnabled_Default

  Summary:
    Implements Default variant of PLIB_POWER_DeepSleepModeIsEnabled 

  Description:
    This template implements the Default variant of the PLIB_POWER_DeepSleepModeIsEnabled function.
*/

PLIB_TEMPLATE bool POWER_DeepSleepModeIsEnabled_Default( POWER_MODULE_ID index )
{
   	return (bool)(_SFR_BIT_READ(_POWER_DEEP_SLEEP_ENABLE_VREG(index), _POWER_DEEP_SLEEP_ENABLE_POS(index)));
}


#endif /*_POWER_DEEPSLEEPMODECONTROL_DEFAULT_H*/

/******************************************************************************
 End of File
*/

