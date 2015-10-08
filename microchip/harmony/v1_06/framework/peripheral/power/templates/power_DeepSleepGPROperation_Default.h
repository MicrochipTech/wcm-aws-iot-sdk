/*******************************************************************************
  POWER Peripheral Library Template Implementation

  File Name:
    power_DeepSleepGPROperation_Default.h

  Summary:
    POWER PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : DeepSleepGPROperation
    and its Variant : Default
    For following APIs :
        PLIB_POWER_ExistsDeepSleepGPROperation
        PLIB_POWER_DeepSleepGPRWrite
        PLIB_POWER_DeepSleepGPRRead

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

#ifndef _POWER_DEEPSLEEPGPROPERATION_DEFAULT_H
#define _POWER_DEEPSLEEPGPROPERATION_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _POWER_DSGPR_0_VREG(index)

  MASKs: 
    _POWER_DSGPR_0_MASK(index)

  POSs: 
    _POWER_DSGPR_0_POS(index)

  LENs: 
    _POWER_DSGPR_0_LEN(index)

*/


//******************************************************************************
/* Function :  POWER_ExistsDeepSleepGPROperation_Default

  Summary:
    Implements Default variant of PLIB_POWER_ExistsDeepSleepGPROperation

  Description:
    This template implements the Default variant of the PLIB_POWER_ExistsDeepSleepGPROperation function.
*/

#define PLIB_POWER_ExistsDeepSleepGPROperation PLIB_POWER_ExistsDeepSleepGPROperation
PLIB_TEMPLATE bool POWER_ExistsDeepSleepGPROperation_Default( POWER_MODULE_ID index )
{
    return true;
}


//******************************************************************************
/* Function :  POWER_DeepSleepGPRWrite_Default

  Summary:
    Implements Default variant of PLIB_POWER_DeepSleepGPRWrite 

  Description:
    This template implements the Default variant of the PLIB_POWER_DeepSleepGPRWrite function.
*/

PLIB_TEMPLATE void POWER_DeepSleepGPRWrite_Default( POWER_MODULE_ID index , DEEP_SLEEP_GPR gpr , uint32_t data )
{
	uint32_t gpr1address = (_POWER_DSGPR_0_VREG( index ) + 8));
	uint32_t gprxaddress = (gpr1address + ((gpr-1) * 4));

    switch(gpr)
	{
		case 0: 
					_SFR_WRITE( _POWER_DSGPR_0_VREG( index ), data );
					break;
		case 1: 
					_SFR_WRITE( gpr1address, data );
					break;
		default: 
					_SFR_WRITE( gprxaddress, data );
					break;

	}	
}


//******************************************************************************
/* Function :  POWER_DeepSleepGPRRead_Default

  Summary:
    Implements Default variant of PLIB_POWER_DeepSleepGPRRead 

  Description:
    This template implements the Default variant of the PLIB_POWER_DeepSleepGPRRead function.
*/

PLIB_TEMPLATE uint32_t POWER_DeepSleepGPRRead_Default( POWER_MODULE_ID index , DEEP_SLEEP_GPR gpr )
{
	uint32_t gpr1address = (_POWER_DSGPR_0_VREG( index ) + 8));
	uint32_t gprxaddress = (gpr1address + ((gpr-1) * 4));

    switch(gpr)
	{
		case 0: 
					return (uint32_t)(_SFR_READ( _POWER_DSGPR_0_VREG( index )));
					break;
		case 1: 
					return (uint32_t)(_SFR_READ( gpr1address));
					break;
		default: 
					return (uint32_t)(_SFR_READ( gprxaddress));
					break;

	}		
	
}


#endif /*_POWER_DEEPSLEEPGPROPERATION_DEFAULT_H*/

/******************************************************************************
 End of File
*/

