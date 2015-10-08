/*******************************************************************************
  PORTS Peripheral Library Template Implementation

  File Name:
    ports_PortsDirection_MCU32_PPS.h

  Summary:
    PORTS PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : PortsDirection
    and its Variant : MCU32_PPS
    For following APIs :
        PLIB_PORTS_PinDirectionInputSet
        PLIB_PORTS_PinDirectionOutputSet
        PLIB_PORTS_DirectionInputSet
        PLIB_PORTS_DirectionOutputSet
        PLIB_PORTS_DirectionGet
        PLIB_PORTS_ExistsPortsDirection

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

#ifndef _PORTS_PORTSDIRECTION_MCU32_PPS_H
#define _PORTS_PORTSDIRECTION_MCU32_PPS_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _PORTS_DIRECTION_B_VREG(index)
    _PORTS_REMAP_FUNC_INT1_VREG(index)

  MASKs: 
    _PORTS_DIRECTION_B_MASK(index)
    _PORTS_REMAP_FUNC_INT1_MASK(index)

  POSs: 
    _PORTS_DIRECTION_B_POS(index)
    _PORTS_REMAP_FUNC_INT1_POS(index)

  LENs: 
    _PORTS_DIRECTION_B_LEN(index)
    _PORTS_REMAP_FUNC_INT1_LEN(index)

*/


//******************************************************************************
/* Function :  PORTS_PinDirectionInputSet_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_PinDirectionInputSet 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_PinDirectionInputSet function.
*/

PLIB_TEMPLATE void PORTS_PinDirectionInputSet_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_BIT_POS       bitPos )
{
    _SFR_BIT_SET( _PORTS_DIRECTION_B_VREG(index) + ((channel-1) * 0x40), bitPos );
}


//******************************************************************************
/* Function :  PORTS_PinDirectionOutputSet_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_PinDirectionOutputSet 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_PinDirectionOutputSet function.
*/

PLIB_TEMPLATE void PORTS_PinDirectionOutputSet_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_BIT_POS       bitPos )
{
    _SFR_BIT_CLEAR( _PORTS_DIRECTION_B_VREG(index) + ((channel-1) * 0x40), bitPos );
}


//******************************************************************************
/* Function :  PORTS_DirectionInputSet_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_DirectionInputSet 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_DirectionInputSet function.
*/

PLIB_TEMPLATE void PORTS_DirectionInputSet_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_DATA_MASK mask )
{
    _SFR_SET( _PORTS_DIRECTION_B_VREG(index) + ((channel-1) * 0x40), mask );
}


//******************************************************************************
/* Function :  PORTS_DirectionOutputSet_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_DirectionOutputSet 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_DirectionOutputSet function.
*/

PLIB_TEMPLATE void PORTS_DirectionOutputSet_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel , PORTS_DATA_MASK mask )
{
    _SFR_CLEAR( _PORTS_DIRECTION_B_VREG(index) + ((channel-1) * 0x40), mask );
}


//******************************************************************************
/* Function :  PORTS_DirectionGet_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_DirectionGet 

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_DirectionGet function.
*/

PLIB_TEMPLATE PORTS_DATA_MASK PORTS_DirectionGet_MCU32_PPS( PORTS_MODULE_ID index , PORTS_CHANNEL channel )
{
    return( _SFR_READ( _PORTS_DIRECTION_B_VREG(index) + ((channel-1) * 0x40) ) );
}


//******************************************************************************
/* Function :  PORTS_ExistsPortsDirection_MCU32_PPS

  Summary:
    Implements MCU32_PPS variant of PLIB_PORTS_ExistsPortsDirection

  Description:
    This template implements the MCU32_PPS variant of the PLIB_PORTS_ExistsPortsDirection function.
*/

#define PLIB_PORTS_ExistsPortsDirection PLIB_PORTS_ExistsPortsDirection
PLIB_TEMPLATE bool PORTS_ExistsPortsDirection_MCU32_PPS( PORTS_MODULE_ID index )
{
    return true;
}


#endif /*_PORTS_PORTSDIRECTION_MCU32_PPS_H*/

/******************************************************************************
 End of File
*/

