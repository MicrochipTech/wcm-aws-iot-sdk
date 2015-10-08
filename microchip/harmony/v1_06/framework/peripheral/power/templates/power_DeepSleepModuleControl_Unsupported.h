/*******************************************************************************
  POWER Peripheral Library Template Implementation

  File Name:
    power_DeepSleepModuleControl_Unsupported.h

  Summary:
    POWER PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : DeepSleepModuleControl
    and its Variant : Unsupported
    For following APIs :
        PLIB_POWER_ExistsDeepSleepModuleControl
        PLIB_POWER_DeepSleepModuleEnable
        PLIB_POWER_DeepSleepModuleDisable

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

#ifndef _POWER_DEEPSLEEPMODULECONTROL_UNSUPPORTED_H
#define _POWER_DEEPSLEEPMODULECONTROL_UNSUPPORTED_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    None.

  MASKs: 
    None.

  POSs: 
    None.

  LENs: 
    None.

*/


//******************************************************************************
/* Function :  POWER_ExistsDeepSleepModuleControl_Unsupported

  Summary:
    Implements Unsupported variant of PLIB_POWER_ExistsDeepSleepModuleControl

  Description:
    This template implements the Unsupported variant of the PLIB_POWER_ExistsDeepSleepModuleControl function.
*/

PLIB_TEMPLATE bool POWER_ExistsDeepSleepModuleControl_Unsupported( POWER_MODULE_ID index )
{
    return false;
}


//******************************************************************************
/* Function :  POWER_DeepSleepModuleEnable_Unsupported

  Summary:
    Implements Unsupported variant of PLIB_POWER_DeepSleepModuleEnable 

  Description:
    This template implements the Unsupported variant of the PLIB_POWER_DeepSleepModuleEnable function.
*/

PLIB_TEMPLATE void POWER_DeepSleepModuleEnable_Unsupported( POWER_MODULE_ID index , DEEP_SLEEP_MODULE module )
{
    PLIB_ASSERT(false, "The device selected does not implement PLIB_POWER_DeepSleepModuleEnable");
}


//******************************************************************************
/* Function :  POWER_DeepSleepModuleDisable_Unsupported

  Summary:
    Implements Unsupported variant of PLIB_POWER_DeepSleepModuleDisable 

  Description:
    This template implements the Unsupported variant of the PLIB_POWER_DeepSleepModuleDisable function.
*/

PLIB_TEMPLATE void POWER_DeepSleepModuleDisable_Unsupported( POWER_MODULE_ID index , DEEP_SLEEP_MODULE module )
{
    PLIB_ASSERT(false, "The device selected does not implement PLIB_POWER_DeepSleepModuleDisable");
}


#endif /*_POWER_DEEPSLEEPMODULECONTROL_UNSUPPORTED_H*/

/******************************************************************************
 End of File
*/

