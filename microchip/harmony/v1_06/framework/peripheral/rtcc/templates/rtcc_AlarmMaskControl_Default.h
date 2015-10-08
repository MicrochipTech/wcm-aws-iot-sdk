/*******************************************************************************
  RTCC Peripheral Library Template Implementation

  File Name:
    rtcc_AlarmMaskControl_Default.h

  Summary:
    RTCC PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : AlarmMaskControl
    and its Variant : Default
    For following APIs :
        PLIB_RTCC_AlarmMaskModeSelect
        PLIB_RTCC_ExistsAlarmMaskControl

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

#ifndef _RTCC_ALARMMASKCONTROL_DEFAULT_H
#define _RTCC_ALARMMASKCONTROL_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _RTCC_ALARM_MASK_VREG(index)

  MASKs: 
    _RTCC_ALARM_MASK_MASK(index)

  POSs: 
    _RTCC_ALARM_MASK_POS(index)

  LENs: 
    _RTCC_ALARM_MASK_LEN(index)

*/


//******************************************************************************
/* Function :  RTCC_AlarmMaskModeSelect_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmMaskModeSelect 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmMaskModeSelect function.
*/

PLIB_TEMPLATE void RTCC_AlarmMaskModeSelect_Default( RTCC_MODULE_ID index , RTCC_ALARM_MASK_CONFIGURATION data )
{
    _SFR_FIELD_WRITE( _RTCC_ALARM_MASK_VREG( index ),
	                  _RTCC_ALARM_MASK_MASK( index ),
					  _RTCC_ALARM_MASK_POS( index ),
					  data );
}


//******************************************************************************
/* Function :  RTCC_ExistsAlarmMaskControl_Default

  Summary:
    Implements Default variant of PLIB_RTCC_ExistsAlarmMaskControl

  Description:
    This template implements the Default variant of the PLIB_RTCC_ExistsAlarmMaskControl function.
*/

#define PLIB_RTCC_ExistsAlarmMaskControl PLIB_RTCC_ExistsAlarmMaskControl
PLIB_TEMPLATE bool RTCC_ExistsAlarmMaskControl_Default( RTCC_MODULE_ID index )
{
    return true;
}


#endif /*_RTCC_ALARMMASKCONTROL_DEFAULT_H*/

/******************************************************************************
 End of File
*/

