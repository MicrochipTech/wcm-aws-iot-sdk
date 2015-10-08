/*******************************************************************************
  PORTS Peripheral Library Template Implementation

  File Name:
    ports_PortsWrite_MCU32_PPS.h

  Summary:
    PORTS PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : PortsWrite
    and its Variant : MCU32_PPS
    For following APIs :
        PLIB_PORTS_PinWrite
        PLIB_PORTS_PinSet
        PLIB_PORTS_PinClear
        PLIB_PORTS_PinToggle
        PLIB_PORTS_Write
        PLIB_PORTS_Set
        PLIB_PORTS_Toggle
        PLIB_PORTS_Clear
        PLIB_PORTS_ExistsPortsWrite

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

#ifndef _PORTS_PORTSWRITE_MCU32_PPS_H
#define _PORTS_PORTSWRITE_MCU32_PPS_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _PORTS_WRITE_B_VREG(index)
    _PORTS_REMAP_FUNC_INT1_VREG(index)

  MASKs: 
    _PORTS_WRITE_B_MASK(index)
    _PORTS_REMAP_FUNC_INT1_MASK(index)

  POSs: 
    _PORTS_WRITE_B_POS(index)
    _PORTS_REMAP_FUNC_INT1_POS(index)

  LENs: 
    _PORTS_WRITE_B_LEN(index)
    _PORTS_REMAP_FUNC_INT1_LEN(index)

*/


//******************************************************************************
/* Function :  PORTS_PinWrite_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_PinWrite 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_PinWrite function.
*/

PLIB_TEMPLATE void PORTS_PinWrite_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_BIT_POS       bitPos , bool            value )
{
    _SFR_BIT_WRITE(_PORTS_WRITE_B_VREG(index) + ((channel-1) * 0x40),
                           bitPos ,
                           value                      );
	
}


//******************************************************************************
/* Function :  PORTS_PinSet_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_PinSet 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_PinSet function.
*/

PLIB_TEMPLATE void PORTS_PinSet_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_BIT_POS       bitPos )
{

	_SFR_BIT_SET(_PORTS_WRITE_B_VREG(index) + ((channel-1) * 0x40),
                           bitPos );
    
}


//******************************************************************************
/* Function :  PORTS_PinClear_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_PinClear 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_PinClear function.
*/

PLIB_TEMPLATE void PORTS_PinClear_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_BIT_POS       bitPos )
{
	
	_SFR_BIT_CLEAR(_PORTS_WRITE_B_VREG(index) + ((channel-1) * 0x40) ,
                           bitPos );
						   
}


//******************************************************************************
/* Function :  PORTS_PinToggle_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_PinToggle 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_PinToggle function.
*/

PLIB_TEMPLATE void PORTS_PinToggle_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_BIT_POS       bitPos )
{

	_SFR_BIT_INVERT(_PORTS_WRITE_B_VREG(index) + ((channel-1) * 0x40),
                           bitPos );
						   
}


//******************************************************************************
/* Function :  PORTS_Write_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_Write 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_Write function.
*/

PLIB_TEMPLATE void PORTS_Write_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_DATA_TYPE value )
{
    _SFR_WRITE(_PORTS_WRITE_B_VREG(index) + ((channel-1) * 0x40),
                           value );
}


//******************************************************************************
/* Function :  PORTS_Set_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_Set 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_Set function.
*/

PLIB_TEMPLATE void PORTS_Set_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_DATA_TYPE value , PORTS_DATA_MASK mask )
{
    
    _SFR_FIELD_SET(_PORTS_WRITE_B_VREG(index) + ((channel-1) * 0x40),
                         mask, 0 , value);
	
}


//******************************************************************************
/* Function :  PORTS_Toggle_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_Toggle 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_Toggle function.
*/

PLIB_TEMPLATE void PORTS_Toggle_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_DATA_MASK toggleMask )
{
    _SFR_FIELD_INVERT( _PORTS_WRITE_B_VREG(index) + ((channel-1) * 0x40),
                                toggleMask, 0,
                                0xFFFF );
}


//******************************************************************************
/* Function :  PORTS_Clear_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_Clear 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_Clear function.
*/

PLIB_TEMPLATE void PORTS_Clear_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_DATA_MASK clearMask )
{
    
	_SFR_FIELD_CLEAR(_PORTS_WRITE_B_VREG(index) + ((channel-1) * 0x40),
                         clearMask, 0 , 0xFFFF);
						 
}


//******************************************************************************
/* Function :  PORTS_ExistsPortsWrite_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_ExistsPortsWrite

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_ExistsPortsWrite function.
*/

#define PLIB_PORTS_ExistsPortsWrite PLIB_PORTS_ExistsPortsWrite
PLIB_TEMPLATE bool PORTS_ExistsPortsWrite_MCU32_PPS( PORTS_MODULE_ID index )
{
    return true;
}


#endif /*_PORTS_PORTSWRITE_MCU32_PPS_H*/

/******************************************************************************
 End of File
*/

