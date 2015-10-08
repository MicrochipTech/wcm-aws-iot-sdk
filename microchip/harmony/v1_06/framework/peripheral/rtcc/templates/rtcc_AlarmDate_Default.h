/*******************************************************************************
  RTCC Peripheral Library Template Implementation

  File Name:
    rtcc_AlarmDate_Default.h

  Summary:
    RTCC PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : AlarmDate
    and its Variant : Default
    For following APIs :
        PLIB_RTCC_AlarmDateGet
        PLIB_RTCC_AlarmDateSet
        PLIB_RTCC_AlarmMonthGet
        PLIB_RTCC_AlarmMonthSet
        PLIB_RTCC_AlarmDayGet
        PLIB_RTCC_AlarmDaySet
        PLIB_RTCC_AlarmWeekDayGet
        PLIB_RTCC_AlarmWeekDaySet
        PLIB_RTCC_ExistsAlarmDate

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

#ifndef _RTCC_ALARMDATE_DEFAULT_H
#define _RTCC_ALARMDATE_DEFAULT_H

//******************************************************************************
/* Routines available for accessing VREGS, MASKS, POS, LEN are 

  VREGs: 
    _RTCC_ALARM_DATE_VREG(index)
    _RTCC_ALARM_MONTH10_VREG(index)
    _RTCC_ALARM_MONTH01_VREG(index)
    _RTCC_ALARM_DAY10_VREG(index)
    _RTCC_ALARM_DAY01_VREG(index)
    _RTCC_ALARM_WDAY01_VREG(index)

  MASKs: 
    _RTCC_ALARM_DATE_MASK(index)
    _RTCC_ALARM_MONTH10_MASK(index)
    _RTCC_ALARM_MONTH01_MASK(index)
    _RTCC_ALARM_DAY10_MASK(index)
    _RTCC_ALARM_DAY01_MASK(index)
    _RTCC_ALARM_WDAY01_MASK(index)

  POSs: 
    _RTCC_ALARM_DATE_POS(index)
    _RTCC_ALARM_MONTH10_POS(index)
    _RTCC_ALARM_MONTH01_POS(index)
    _RTCC_ALARM_DAY10_POS(index)
    _RTCC_ALARM_DAY01_POS(index)
    _RTCC_ALARM_WDAY01_POS(index)

  LENs: 
    _RTCC_ALARM_DATE_LEN(index)
    _RTCC_ALARM_MONTH10_LEN(index)
    _RTCC_ALARM_MONTH01_LEN(index)
    _RTCC_ALARM_DAY10_LEN(index)
    _RTCC_ALARM_DAY01_LEN(index)
    _RTCC_ALARM_WDAY01_LEN(index)

*/


//******************************************************************************
/* Function :  RTCC_AlarmDateGet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmDateGet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmDateGet function.
*/

PLIB_TEMPLATE uint32_t RTCC_AlarmDateGet_Default( RTCC_MODULE_ID index )
{
    return _SFR_READ( _RTCC_ALARM_DATE_VREG( index ) );
}


//******************************************************************************
/* Function :  RTCC_AlarmDateSet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmDateSet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmDateSet function.
*/

PLIB_TEMPLATE void RTCC_AlarmDateSet_Default( RTCC_MODULE_ID index , uint32_t data )
{
    _SFR_WRITE( _RTCC_ALARM_DATE_VREG( index ), data );
}


//******************************************************************************
/* Function :  RTCC_AlarmMonthGet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmMonthGet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmMonthGet function.
*/

PLIB_TEMPLATE uint32_t RTCC_AlarmMonthGet_Default( RTCC_MODULE_ID index )
{
    return _SFR_FIELD_READ( _RTCC_ALARM_MONTH10_VREG( index ),
	                        _RTCC_ALARM_MONTH10_MASK( index ) | _RTCC_ALARM_MONTH01_MASK( index ),
							_RTCC_ALARM_MONTH01_POS( index ) );
}


//******************************************************************************
/* Function :  RTCC_AlarmMonthSet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmMonthSet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmMonthSet function.
*/

PLIB_TEMPLATE void RTCC_AlarmMonthSet_Default( RTCC_MODULE_ID index , uint32_t month )
{
    _SFR_FIELD_WRITE( _RTCC_ALARM_MONTH10_VREG( index ),
	                  _RTCC_ALARM_MONTH10_MASK( index ) | _RTCC_ALARM_MONTH01_MASK( index ),
					  _RTCC_ALARM_MONTH01_POS( index ),
					  month );
}


//******************************************************************************
/* Function :  RTCC_AlarmDayGet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmDayGet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmDayGet function.
*/

PLIB_TEMPLATE uint32_t RTCC_AlarmDayGet_Default( RTCC_MODULE_ID index )
{
    return _SFR_FIELD_READ( _RTCC_ALARM_DAY10_VREG( index ),
	                        _RTCC_ALARM_DAY10_MASK( index ) | _RTCC_ALARM_DAY01_MASK( index ),
							_RTCC_ALARM_DAY01_POS( index ) );
}


//******************************************************************************
/* Function :  RTCC_AlarmDaySet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmDaySet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmDaySet function.
*/

PLIB_TEMPLATE void RTCC_AlarmDaySet_Default( RTCC_MODULE_ID index , uint32_t day )
{
    _SFR_FIELD_WRITE( _RTCC_ALARM_DAY10_VREG( index ),
	                  _RTCC_ALARM_DAY10_MASK( index ) | _RTCC_ALARM_DAY01_MASK( index ),
					  _RTCC_ALARM_DAY01_POS( index ),
					  day );
}


//******************************************************************************
/* Function :  RTCC_AlarmWeekDayGet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmWeekDayGet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmWeekDayGet function.
*/

PLIB_TEMPLATE uint32_t RTCC_AlarmWeekDayGet_Default( RTCC_MODULE_ID index )
{
    return _SFR_FIELD_READ( _RTCC_ALARM_WDAY01_VREG( index ),
	                        _RTCC_ALARM_WDAY01_MASK( index ),
							_RTCC_ALARM_WDAY01_POS( index ) );
}


//******************************************************************************
/* Function :  RTCC_AlarmWeekDaySet_Default

  Summary:
    Implements Default variant of PLIB_RTCC_AlarmWeekDaySet 

  Description:
    This template implements the Default variant of the PLIB_RTCC_AlarmWeekDaySet function.
*/

PLIB_TEMPLATE void RTCC_AlarmWeekDaySet_Default( RTCC_MODULE_ID index , uint32_t weekday )
{
    _SFR_FIELD_WRITE( _RTCC_ALARM_WDAY01_VREG( index ),
	                  _RTCC_ALARM_WDAY01_MASK( index ),
					  _RTCC_ALARM_WDAY01_POS( index ),
					  weekday );
}


//******************************************************************************
/* Function :  RTCC_ExistsAlarmDate_Default

  Summary:
    Implements Default variant of PLIB_RTCC_ExistsAlarmDate

  Description:
    This template implements the Default variant of the PLIB_RTCC_ExistsAlarmDate function.
*/

#define PLIB_RTCC_ExistsAlarmDate PLIB_RTCC_ExistsAlarmDate
PLIB_TEMPLATE bool RTCC_ExistsAlarmDate_Default( RTCC_MODULE_ID index )
{
    return true;
}


#endif /*_RTCC_ALARMDATE_DEFAULT_H*/

/******************************************************************************
 End of File
*/

