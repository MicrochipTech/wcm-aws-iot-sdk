/*******************************************************************************
  POWER Peripheral Library Template Implementation

  File Name:
    power_DeepSleepEventStatus_Default.h

  Summary:
    POWER PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : DeepSleepEventStatus
    and its Variant : Default
    For following APIs :
        PLIB_POWER_ExistsDeepSleepEventStatus
        PLIB_POWER_DeepSleepEventStatusGet
        PLIB_POWER_DeepSleepEventStatusClear

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

#ifndef _POWER_DEEPSLEEPEVENTSTATUS_DEFAULT_H
#define _POWER_DEEPSLEEPEVENTSTATUS_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _POWER_DEEP_SLEEP_BOR_EVENT_STATUS_VREG(index)
    _POWER_DEEP_SLEEP_IOC_STATUS_VREG(index)

  MASKs: 
    _POWER_DEEP_SLEEP_BOR_EVENT_STATUS_MASK(index)
    _POWER_DEEP_SLEEP_IOC_STATUS_MASK(index)

  POSs: 
    _POWER_DEEP_SLEEP_BOR_EVENT_STATUS_POS(index)
    _POWER_DEEP_SLEEP_IOC_STATUS_POS(index)

  LENs: 
    _POWER_DEEP_SLEEP_BOR_EVENT_STATUS_LEN(index)
    _POWER_DEEP_SLEEP_IOC_STATUS_LEN(index)

*/


//******************************************************************************
/* Function :  POWER_ExistsDeepSleepEventStatus_Default

  Summary:
    Implements Default variant of PLIB_POWER_ExistsDeepSleepEventStatus

  Description:
    This template implements the Default variant of the PLIB_POWER_ExistsDeepSleepEventStatus function.
*/

#define PLIB_POWER_ExistsDeepSleepEventStatus PLIB_POWER_ExistsDeepSleepEventStatus
PLIB_TEMPLATE bool POWER_ExistsDeepSleepEventStatus_Default( POWER_MODULE_ID index )
{
    return true;
}


//******************************************************************************
/* Function :  POWER_DeepSleepEventStatusGet_Default

  Summary:
    Implements Default variant of PLIB_POWER_DeepSleepEventStatusGet 

  Description:
    This template implements the Default variant of the PLIB_POWER_DeepSleepEventStatusGet function.
*/

PLIB_TEMPLATE DEEP_SLEEP_EVENT POWER_DeepSleepEventStatusGet_Default( POWER_MODULE_ID index )
{
  bool BOR_Event;
  uint32_t events,dswake;
  
  BOR_Event = _SFR_BIT_READ(_POWER_DEEP_SLEEP_BOR_EVENT_STATUS_VREG(index), _POWER_DEEP_SLEEP_BOR_EVENT_STATUS_POS(index));
  
  dswake = _SFR_READ(_POWER_DEEP_SLEEP_IOC_STATUS_VREG(index));
  
  events = (dswake | (BOR_Event<<1));
  
  return (DEEP_SLEEP_EVENT)events;
  
}


//******************************************************************************
/* Function :  POWER_DeepSleepEventStatusClear_Default

  Summary:
    Implements Default variant of PLIB_POWER_DeepSleepEventStatusClear 

  Description:
    This template implements the Default variant of the PLIB_POWER_DeepSleepEventStatusClear function.
*/

PLIB_TEMPLATE void POWER_DeepSleepEventStatusClear_Default( POWER_MODULE_ID index , DEEP_SLEEP_EVENT event )
{
	
	uint32_t dswake, dswakemask;
	
    switch(event)
	{
		case DEEP_SLEEP_EVENT_BOR: 
					_SFR_BIT_CLEAR(_POWER_DEEP_SLEEP_BOR_EVENT_STATUS_VREG(index),_POWER_DEEP_SLEEP_BOR_EVENT_STATUS_POS(index));
					break;
					
		default:
		            dswake = _SFR_READ(_POWER_DEEP_SLEEP_IOC_STATUS_VREG(index));
                    dswakemask = (dswake & (~event));
                    _SFR_WRITE( _POWER_DEEP_SLEEP_IOC_STATUS_VREG( index ), dswakemask );					
					break;
	}
}


#endif /*_POWER_DEEPSLEEPEVENTSTATUS_DEFAULT_H*/

/******************************************************************************
 End of File
*/

