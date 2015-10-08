/*******************************************************************************
  RTCC Peripheral Library Template Implementation

  File Name:
    rtcc_AlarmTime_Default.h

  Summary:
    RTCC PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : AlarmTime
    and its Variant : Default
    For following APIs :
        PLIB_RTCC_AlarmTimeGet
        PLIB_RTCC_AlarmTimeSet
        PLIB_RTCC_AlarmHourGet
        PLIB_RTCC_AlarmHourSet
        PLIB_RTCC_AlarmMinuteGet
        PLIB_RTCC_AlarmMinuteSet
        PLIB_RTCC_AlarmSecondGet
        PLIB_RTCC_AlarmSecondSet
        PLIB_RTCC_ExistsAlarmTime

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

#ifndef _RTCC_ALARMTIME_DEFAULT_H
#define _RTCC_ALARMTIME_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _RTCC_ALARM_TIME_VREG(index)
    _RTCC_ALARM_HR10_VREG(index)
    _RTCC_ALARM_HR01_VREG(index)
    _RTCC_ALARM_MIN10_VREG(index)
    _RTCC_ALARM_MIN01_VREG(index)
    _RTCC_ALARM_SEC10_VREG(index)
    _RTCC_ALARM_SEC01_VREG(index)

  MASKs: 
    _RTCC_ALARM_TIME_MASK(index)
    _RTCC_ALARM_HR10_MASK(index)
    _RTCC_ALARM_HR01_MASK(index)
    _RTCC_ALARM_MIN10_MASK(index)
    _RTCC_ALARM_MIN01_MASK(index)
    _RTCC_ALARM_SEC10_MASK(index)
    _RTCC_ALARM_SEC01_MASK(index)

  POSs: 
    _RTCC_ALARM_TIME_POS(index)
    _RTCC_ALARM_HR10_POS(index)
    _RTCC_ALARM_HR01_POS(index)
    _RTCC_ALARM_MIN10_POS(index)
    _RTCC_ALARM_MIN01_POS(index)
    _RTCC_ALARM_SEC10_POS(index)
    _RTCC_ALARM_SEC01_POS(index)

  LENs: 
    _RTCC_ALARM_TIME_LEN(index)
    _RTCC_ALARM_HR10_LEN(index)
    _RTCC_ALARM_HR01_LEN(index)
    _RTCC_ALARM_MIN10_LEN(index)
    _RTCC_ALARM_MIN01_LEN(index)
    _RTCC_ALARM_SEC10_LEN(index)
    _RTCC_ALARM_SEC01_LEN(index)

*/


//******************************************************************************
/* Function :  RTCC_AlarmTimeGet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmTimeGet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmTimeGet function.
*/

PLIB_TEMPLATE uint32_t RTCC_AlarmTimeGet_Default( RTCC_MODULE_ID index )
{
    return _SFR_READ( _RTCC_ALARM_TIME_VREG( index ) );
}


//******************************************************************************
/* Function :  RTCC_AlarmTimeSet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmTimeSet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmTimeSet function.
*/

PLIB_TEMPLATE void RTCC_AlarmTimeSet_Default( RTCC_MODULE_ID index , uint32_t data )
{
    _SFR_WRITE( _RTCC_ALARM_TIME_VREG( index ), data );
}


//******************************************************************************
/* Function :  RTCC_AlarmHourGet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmHourGet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmHourGet function.
*/

PLIB_TEMPLATE uint32_t RTCC_AlarmHourGet_Default( RTCC_MODULE_ID index )
{
    return _SFR_FIELD_READ( _RTCC_ALARM_HR10_VREG( index ),
	                        _RTCC_ALARM_HR10_MASK( index ) | _RTCC_ALARM_HR01_MASK( index ),
							_RTCC_ALARM_HR01_POS( index ) );
}


//******************************************************************************
/* Function :  RTCC_AlarmHourSet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmHourSet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmHourSet function.
*/

PLIB_TEMPLATE void RTCC_AlarmHourSet_Default( RTCC_MODULE_ID index , uint32_t hour )
{
    _SFR_FIELD_WRITE( _RTCC_ALARM_HR10_VREG( index ),
	                  _RTCC_ALARM_HR10_MASK( index ) | _RTCC_ALARM_HR01_MASK( index ),
					  _RTCC_ALARM_HR01_POS( index ),
					  hour );
}


//******************************************************************************
/* Function :  RTCC_AlarmMinuteGet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmMinuteGet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmMinuteGet function.
*/

PLIB_TEMPLATE uint32_t RTCC_AlarmMinuteGet_Default( RTCC_MODULE_ID index )
{
    return _SFR_FIELD_READ( _RTCC_ALARM_MIN10_VREG( index ),
	                        _RTCC_ALARM_MIN10_MASK( index ) | _RTCC_ALARM_MIN01_MASK( index ),
							_RTCC_ALARM_MIN01_POS( index ) );
}


//******************************************************************************
/* Function :  RTCC_AlarmMinuteSet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmMinuteSet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmMinuteSet function.
*/

PLIB_TEMPLATE void RTCC_AlarmMinuteSet_Default( RTCC_MODULE_ID index , uint32_t minute )
{
    _SFR_FIELD_WRITE( _RTCC_ALARM_MIN10_VREG( index ),
	                  _RTCC_ALARM_MIN10_MASK( index ) | _RTCC_ALARM_MIN01_MASK( index ),
					  _RTCC_ALARM_MIN01_POS( index ),
					  minute );
}


//******************************************************************************
/* Function :  RTCC_AlarmSecondGet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmSecondGet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmSecondGet function.
*/

PLIB_TEMPLATE uint32_t RTCC_AlarmSecondGet_Default( RTCC_MODULE_ID index )
{
    return _SFR_FIELD_READ( _RTCC_ALARM_SEC10_VREG( index ),
	                        _RTCC_ALARM_SEC10_MASK( index ) | _RTCC_ALARM_SEC01_MASK( index ),
							_RTCC_ALARM_SEC01_POS( index ) );
}


//******************************************************************************
/* Function :  RTCC_AlarmSecondSet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmSecondSet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmSecondSet function.
*/

PLIB_TEMPLATE void RTCC_AlarmSecondSet_Default( RTCC_MODULE_ID index , uint32_t second )
{
    _SFR_FIELD_WRITE( _RTCC_ALARM_SEC10_VREG( index ),
	                  _RTCC_ALARM_SEC10_MASK( index ) | _RTCC_ALARM_SEC01_MASK( index ),
					  _RTCC_ALARM_SEC01_POS( index ),
					  second );
}


//******************************************************************************
/* Function :  RTCC_ExistsAlarmTime_Default

  Summary:
    Implements Default variant of PLIB_RTCC_ExistsAlarmTime

  Description:
    This template implements the Default variant of the PLIB_RTCC_ExistsAlarmTime function.
*/

#define PLIB_RTCC_ExistsAlarmTime PLIB_RTCC_ExistsAlarmTime
PLIB_TEMPLATE bool RTCC_ExistsAlarmTime_Default( RTCC_MODULE_ID index )
{
    return true;
}


#endif /*_RTCC_ALARMTIME_DEFAULT_H*/

/******************************************************************************
 End of File
*/

