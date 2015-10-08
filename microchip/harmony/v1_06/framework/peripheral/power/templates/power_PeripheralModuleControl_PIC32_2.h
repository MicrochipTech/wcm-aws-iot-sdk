/*******************************************************************************
  POWER Peripheral Library Template Implementation

  File Name:
    power_PeripheralModuleControl_PIC32_2.h

  Summary:
    POWER PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : PeripheralModuleControl
    and its Variant : PIC32_2
    For following APIs :
        PLIB_POWER_ExistsPeripheralModuleControl
        PLIB_POWER_PeripheralModuleDisable
        PLIB_POWER_PeripheralModuleEnable
        PLIB_POWER_PeripheralModuleIsEnabled

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

#ifndef _POWER_PERIPHERALMODULECONTROL_PIC32_2_H
#define _POWER_PERIPHERALMODULECONTROL_PIC32_2_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _POWER_DMA_CONTROL_VREG(index)

  MASKs: 
    _POWER_DMA_CONTROL_MASK(index)

  POSs: 
    _POWER_DMA_CONTROL_POS(index)

  LENs: 
    _POWER_DMA_CONTROL_LEN(index)

*/


//******************************************************************************
/* Function :  POWER_ExistsPeripheralModuleControl_PIC32_2

  Summary:
    Implements PIC32_2 variant of PLIB_POWER_ExistsPeripheralModuleControl

  Description:
    This template implements the PIC32_2 variant of the PLIB_POWER_ExistsPeripheralModuleControl function.
*/

#define PLIB_POWER_ExistsPeripheralModuleControl PLIB_POWER_ExistsPeripheralModuleControl
PLIB_TEMPLATE bool POWER_ExistsPeripheralModuleControl_PIC32_2( POWER_MODULE_ID index )
{
    return true;
}


//******************************************************************************
/* Function :  POWER_PeripheralModuleDisable_PIC32_2

  Summary:
    Implements PIC32_2 variant of PLIB_POWER_PeripheralModuleDisable 

  Description:
    This template implements the PIC32_2 variant of the PLIB_POWER_PeripheralModuleDisable function.
*/

PLIB_TEMPLATE void POWER_PeripheralModuleDisable_PIC32_2( POWER_MODULE_ID index , POWER_MODULE source )
{
uint8_t pmdRegister, pmdPosition;

pmdRegister = (source/32) +1;
pmdPosition = source % 32;

    switch(pmdRegister)
	{
		case 1: 
					_SFR_BIT_SET(&PMD1, pmdPosition);
					break;
		case 2: 
					_SFR_BIT_SET(&PMD2, pmdPosition);
					break;
		case 3: 
					_SFR_BIT_SET(&PMD3, pmdPosition);
					break;
		case 4: 
					_SFR_BIT_SET(&PMD4, pmdPosition);
					break;
		case 5: 
					_SFR_BIT_SET(&PMD5, pmdPosition);
					break;
		case 6: 
					_SFR_BIT_SET(&PMD6, pmdPosition);
					break;
		case 7: 
					_SFR_BIT_SET(&PMD7, pmdPosition);
					break;

	}
}


//******************************************************************************
/* Function :  POWER_PeripheralModuleEnable_PIC32_2

  Summary:
    Implements PIC32_2 variant of PLIB_POWER_PeripheralModuleEnable 

  Description:
    This template implements the PIC32_2 variant of the PLIB_POWER_PeripheralModuleEnable function.
*/

PLIB_TEMPLATE void POWER_PeripheralModuleEnable_PIC32_2( POWER_MODULE_ID index , POWER_MODULE source )
{
uint8_t pmdRegister, pmdPosition;

pmdRegister = (source/32) +1;
pmdPosition = source % 32;

    switch(pmdRegister)
	{
		case 1: 
					_SFR_BIT_CLEAR(&PMD1, pmdPosition);
					break;
		case 2: 
					_SFR_BIT_CLEAR(&PMD2, pmdPosition);
					break;
		case 3: 
					_SFR_BIT_CLEAR(&PMD3, pmdPosition);
					break;
		case 4: 
					_SFR_BIT_CLEAR(&PMD4, pmdPosition);
					break;
		case 5: 
					_SFR_BIT_CLEAR(&PMD5, pmdPosition);
					break;
		case 6: 
					_SFR_BIT_CLEAR(&PMD6, pmdPosition);
					break;
		case 7: 
					_SFR_BIT_CLEAR(&PMD7, pmdPosition);
					break;

	}
}


//******************************************************************************
/* Function :  POWER_PeripheralModuleIsEnabled_PIC32_2

  Summary:
    Implements PIC32_2 variant of PLIB_POWER_PeripheralModuleIsEnabled 

  Description:
    This template implements the PIC32_2 variant of the PLIB_POWER_PeripheralModuleIsEnabled function.
*/

PLIB_TEMPLATE bool POWER_PeripheralModuleIsEnabled_PIC32_2( POWER_MODULE_ID index , POWER_MODULE source )
{
uint8_t pmdRegister, pmdPosition;

pmdRegister = (source/32) +1;
pmdPosition = source % 32;

    switch(pmdRegister)
	{
		case 1: 
					return (bool)(!(_SFR_BIT_READ(&PMD1, pmdPosition)));
					
		case 2: 
					return (bool)(!(_SFR_BIT_READ(&PMD2, pmdPosition)));
					
		case 3: 
					return (bool)(!(_SFR_BIT_READ(&PMD3, pmdPosition)));
					
		case 4: 
					return (bool)(!(_SFR_BIT_READ(&PMD4, pmdPosition)));
					
		case 5: 
					return (bool)(!(_SFR_BIT_READ(&PMD5, pmdPosition)));
					
		case 6: 
					return (bool)(!(_SFR_BIT_READ(&PMD6, pmdPosition)));
					
		case 7: 
					return (bool)(!(_SFR_BIT_READ(&PMD7, pmdPosition)));
					

	}
}


#endif /*_POWER_PERIPHERALMODULECONTROL_PIC32_2_H*/

/******************************************************************************
 End of File
*/

