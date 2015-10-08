/*******************************************************************************
  POWER Peripheral Library Template Implementation

  File Name:
    power_DeepSleepWakeupEventControl_Default.h

  Summary:
    POWER PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : DeepSleepWakeupEventControl
    and its Variant : Default
    For following APIs :
        PLIB_POWER_ExistsDeepSleepWakeupEventControl
        PLIB_POWER_DeepSleepWakeupEventEnable
        PLIB_POWER_DeepSleepWakeupEventDisable

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

#ifndef _POWER_DEEPSLEEPWAKEUPEVENTCONTROL_DEFAULT_H
#define _POWER_DEEPSLEEPWAKEUPEVENTCONTROL_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _POWER_DEEP_SLEEP_RTCC_WAKEUP_CONTROL_VREG(index)
    _POWER_DEEP_SLEEP_EXTERNAL_WAKEUP_CONTROL_VREG(index)

  MASKs: 
    _POWER_DEEP_SLEEP_RTCC_WAKEUP_CONTROL_MASK(index)
    _POWER_DEEP_SLEEP_EXTERNAL_WAKEUP_CONTROL_MASK(index)

  POSs: 
    _POWER_DEEP_SLEEP_RTCC_WAKEUP_CONTROL_POS(index)
    _POWER_DEEP_SLEEP_EXTERNAL_WAKEUP_CONTROL_POS(index)

  LENs: 
    _POWER_DEEP_SLEEP_RTCC_WAKEUP_CONTROL_LEN(index)
    _POWER_DEEP_SLEEP_EXTERNAL_WAKEUP_CONTROL_LEN(index)

*/


//******************************************************************************
/* Function :  POWER_ExistsDeepSleepWakeupEventControl_Default

  Summary:
    Implements Default variant of PLIB_POWER_ExistsDeepSleepWakeupEventControl

  Description:
    This template implements the Default variant of the PLIB_POWER_ExistsDeepSleepWakeupEventControl function.
*/

#define PLIB_POWER_ExistsDeepSleepWakeupEventControl PLIB_POWER_ExistsDeepSleepWakeupEventControl
PLIB_TEMPLATE bool POWER_ExistsDeepSleepWakeupEventControl_Default( POWER_MODULE_ID index )
{
    return true;
}


//******************************************************************************
/* Function :  POWER_DeepSleepWakeupEventEnable_Default

  Summary:
    Implements Default variant of PLIB_POWER_DeepSleepWakeupEventEnable 

  Description:
    This template implements the Default variant of the PLIB_POWER_DeepSleepWakeupEventEnable function.
*/

PLIB_TEMPLATE void POWER_DeepSleepWakeupEventEnable_Default( POWER_MODULE_ID index , DEEP_SLEEP_WAKE_UP_EVENT event )
{
    switch(event)
	{
		case DEEP_SLEEP_WAKE_UP_EVENT_RTCC: 
					_SFR_BIT_CLEAR(_POWER_DEEP_SLEEP_RTCC_WAKEUP_CONTROL_VREG(index),_POWER_DEEP_SLEEP_RTCC_WAKEUP_CONTROL_POS(index));
					break;
					
		case DEEP_SLEEP_WAKE_UP_EVENT_EXTERNAL_INTERRUPT: 
					_SFR_BIT_CLEAR(_POWER_DEEP_SLEEP_EXTERNAL_WAKEUP_CONTROL_VREG(index),_POWER_DEEP_SLEEP_EXTERNAL_WAKEUP_CONTROL_POS(index));
					break;
					
		default: 
					break;
	}	
}


//******************************************************************************
/* Function :  POWER_DeepSleepWakeupEventDisable_Default

  Summary:
    Implements Default variant of PLIB_POWER_DeepSleepWakeupEventDisable 

  Description:
    This template implements the Default variant of the PLIB_POWER_DeepSleepWakeupEventDisable function.
*/

PLIB_TEMPLATE void POWER_DeepSleepWakeupEventDisable_Default( POWER_MODULE_ID index , DEEP_SLEEP_WAKE_UP_EVENT event )
{
    switch(event)
	{
		case DEEP_SLEEP_WAKE_UP_EVENT_RTCC: 
					_SFR_BIT_SET(_POWER_DEEP_SLEEP_RTCC_WAKEUP_CONTROL_VREG(index),_POWER_DEEP_SLEEP_RTCC_WAKEUP_CONTROL_POS(index));
					break;
					
		case DEEP_SLEEP_WAKE_UP_EVENT_EXTERNAL_INTERRUPT: 
					_SFR_BIT_SET(_POWER_DEEP_SLEEP_EXTERNAL_WAKEUP_CONTROL_VREG(index),_POWER_DEEP_SLEEP_EXTERNAL_WAKEUP_CONTROL_POS(index));
					break;
					
		default: 
					break;
	}
}


#endif /*_POWER_DEEPSLEEPWAKEUPEVENTCONTROL_DEFAULT_H*/

/******************************************************************************
 End of File
*/

