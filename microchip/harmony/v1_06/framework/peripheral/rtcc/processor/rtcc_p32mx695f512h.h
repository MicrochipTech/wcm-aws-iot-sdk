/* Created by plibgen $Revision: 1.31 $ */

#ifndef _RTCC_P32MX695F512H_H
#define _RTCC_P32MX695F512H_H

/* Section 1 - Enumerate instances, define constants, VREGs */

#include <xc.h>
#include <stdbool.h>

#include "peripheral/peripheral_common_32bit.h"

/* Default definition used for all API dispatch functions */
#ifndef PLIB_INLINE_API
    #define PLIB_INLINE_API extern inline
#endif

/* Default definition used for all other functions */
#ifndef PLIB_INLINE
    #define PLIB_INLINE extern inline
#endif

typedef enum {

    RTCC_ID_0 = 0,
    RTCC_NUMBER_OF_MODULES

} RTCC_MODULE_ID;

typedef enum {

    RTCC_ALARM_EVERY_HALF_SECOND = 0x00,
    RTCC_ALARM_EVERY_SECOND = 0x01,
    RTCC_ALARM_EVERY_10_SECONDS = 0x02,
    RTCC_ALARM_EVERY_MINUTE = 0x03,
    RTCC_ALARM_EVERY_10_MINUTES = 0x04,
    RTCC_ALARM_EVERY_HOUR = 0x05,
    RTCC_ALARM_ONCE_A_DAY = 0x06,
    RTCC_ALARM_ONCE_A_WEEK = 0x07,
    RTCC_ALARM_ONCE_A_MONTH = 0x08,
    RTCC_ALARM_ONCE_A_YEAR = 0x09

} RTCC_ALARM_MASK_CONFIGURATION;

typedef enum {

    RTCC_OUTPUT_ALARM_PULSE = 0x00,
    RTCC_OUTPUT_SECONDS_CLOCK = 0x01

} RTCC_OUTPUT_SELECT;

typedef enum {

    RTCC_CLOCK_SOURCE_SELECT_NONE

} RTCC_CLOCK_SOURCE_SELECT;

PLIB_INLINE SFR_TYPE* _RTCC_CALIBRATION_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCCON;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ENABLE_CONTROL_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCCON;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_STOP_IN_IDLE_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCCON;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_OUTPUT_SELECT_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCCON;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_CLOCK_RUNNING_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCCON;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_WRITE_ENABLE_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCCON;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_SYNC_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCCON;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_HALFSEC_STATUS_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCCON;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_OUTPUT_ENABLE_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCCON;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_CONTROL_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCALRM;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_CHIME_CONTROL_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCALRM;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_PULSE_INITIAL_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCALRM;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_SYNC_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCALRM;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_MASK_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCALRM;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_REPEAT_COUNTER_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCALRM;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_TIME_HR10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_TIME_HR01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_TIME_MIN10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_TIME_MIN01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_TIME_SEC10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_TIME_SEC01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_RTCTIME_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_DATE_YEAR10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_DATE_YEAR01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_DATE_MONTH10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_DATE_MONTH01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_DATE_DAY10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_DATE_DAY01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_DATE_WDAY01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_RTCDATE_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &RTCDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_HR10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_HR01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_MIN10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_MIN01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_SEC10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_SEC01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_TIME_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMTIME;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_MONTH10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_MONTH01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_DAY10_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_DAY01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_WDAY01_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _RTCC_ALARM_DATE_VREG(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return &ALRMDATE;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_CALIBRATION_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_CAL_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ENABLE_CONTROL_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_ON_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_STOP_IN_IDLE_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_SIDL_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_OUTPUT_SELECT_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTSECSEL_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_CLOCK_RUNNING_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCCLKON_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_WRITE_ENABLE_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCWREN_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_SYNC_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCSYNC_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_HALFSEC_STATUS_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_HALFSEC_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_OUTPUT_ENABLE_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCOE_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_CONTROL_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_ALRMEN_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_CHIME_CONTROL_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_CHIME_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_PULSE_INITIAL_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_PIV_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_SYNC_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_ALRMSYNC_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MASK_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_AMASK_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_REPEAT_COUNTER_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_ARPT_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_HR10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_HR10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_HR01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_HR01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_MIN10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_MIN10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_MIN01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_MIN01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_SEC10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_SEC10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_SEC01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_SEC01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_RTCTIME_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)-1;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_YEAR10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_YEAR10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_YEAR01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_YEAR01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_MONTH10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_MONTH10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_MONTH01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_MONTH01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_DAY10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_DAY10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_DAY01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_DAY01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_WDAY01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_WDAY01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_RTCDATE_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)-1;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_HR10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_HR10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_HR01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_HR01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MIN10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_MIN10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MIN01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_MIN01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_SEC10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_SEC10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_SEC01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_SEC01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_TIME_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)-1;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MONTH10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_MONTH10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MONTH01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_MONTH01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_DAY10_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_DAY10_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_DAY01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_DAY01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_WDAY01_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_WDAY01_MASK;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_DATE_MASK(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)-1;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_CALIBRATION_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_CAL_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ENABLE_CONTROL_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_ON_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_STOP_IN_IDLE_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_SIDL_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_OUTPUT_SELECT_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTSECSEL_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_CLOCK_RUNNING_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCCLKON_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_WRITE_ENABLE_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCWREN_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_SYNC_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCSYNC_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_HALFSEC_STATUS_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_HALFSEC_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_OUTPUT_ENABLE_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCOE_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_CONTROL_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_ALRMEN_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_CHIME_CONTROL_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_CHIME_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_PULSE_INITIAL_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_PIV_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_SYNC_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_ALRMSYNC_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MASK_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_AMASK_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_REPEAT_COUNTER_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_ARPT_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_HR10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_HR10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_HR01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_HR01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_MIN10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_MIN10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_MIN01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_MIN01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_SEC10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_SEC10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_SEC01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_SEC01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_RTCTIME_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)0;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_YEAR10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_YEAR10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_YEAR01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_YEAR01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_MONTH10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_MONTH10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_MONTH01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_MONTH01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_DAY10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_DAY10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_DAY01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_DAY01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_WDAY01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_WDAY01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_RTCDATE_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)0;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_HR10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_HR10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_HR01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_HR01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MIN10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_MIN10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MIN01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_MIN01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_SEC10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_SEC10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_SEC01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_SEC01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_TIME_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)0;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MONTH10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_MONTH10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MONTH01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_MONTH01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_DAY10_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_DAY10_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_DAY01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_DAY01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_WDAY01_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_WDAY01_POSITION;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_DATE_POS(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)0;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_CALIBRATION_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_CAL_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ENABLE_CONTROL_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_ON_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_STOP_IN_IDLE_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_SIDL_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_OUTPUT_SELECT_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTSECSEL_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_CLOCK_RUNNING_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCCLKON_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_WRITE_ENABLE_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCWREN_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_SYNC_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCSYNC_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_HALFSEC_STATUS_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_HALFSEC_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_OUTPUT_ENABLE_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCCON_RTCOE_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_CONTROL_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_ALRMEN_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_CHIME_CONTROL_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_CHIME_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_PULSE_INITIAL_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_PIV_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_SYNC_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_ALRMSYNC_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MASK_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_AMASK_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_REPEAT_COUNTER_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCALRM_ARPT_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_HR10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_HR10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_HR01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_HR01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_MIN10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_MIN10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_MIN01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_MIN01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_SEC10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_SEC10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_TIME_SEC01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCTIME_SEC01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_RTCTIME_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_YEAR10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_YEAR10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_YEAR01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_YEAR01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_MONTH10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_MONTH10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_MONTH01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_MONTH01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_DAY10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_DAY10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_DAY01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_DAY01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_DATE_WDAY01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _RTCDATE_WDAY01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_RTCDATE_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_HR10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_HR10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_HR01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_HR01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MIN10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_MIN10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MIN01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_MIN01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_SEC10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_SEC10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_SEC01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMTIME_SEC01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_TIME_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MONTH10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_MONTH10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_MONTH01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_MONTH01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_DAY10_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_DAY10_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_DAY01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_DAY01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_WDAY01_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return _ALRMDATE_WDAY01_LENGTH;
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _RTCC_ALARM_DATE_LEN(RTCC_MODULE_ID i)
{
    switch (i) {
        case RTCC_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

/* Section 2 - Feature variant inclusion */

#define PLIB_TEMPLATE PLIB_INLINE
#include "../templates/rtcc_EnableControl_Default.h"
#include "../templates/rtcc_WriteEnable_Default.h"
#include "../templates/rtcc_StopInIdle_Default.h"
#include "../templates/rtcc_OutputSelect_Default.h"
#include "../templates/rtcc_ClockSelect_Unsupported.h"
#include "../templates/rtcc_ClockRunning_Default.h"
#include "../templates/rtcc_Calibration_Default.h"
#include "../templates/rtcc_Sync_Default.h"
#include "../templates/rtcc_HalfSecond_Default.h"
#include "../templates/rtcc_OutputControl_Default.h"
#include "../templates/rtcc_AlarmControl_Default.h"
#include "../templates/rtcc_AlarmChimeControl_Default.h"
#include "../templates/rtcc_AlarmPulseInitial_Default.h"
#include "../templates/rtcc_AlarmSync_Default.h"
#include "../templates/rtcc_AlarmMaskControl_Default.h"
#include "../templates/rtcc_AlarmRepeatControl_Default.h"
#include "../templates/rtcc_RTCTime_Default.h"
#include "../templates/rtcc_RTCDate_Default.h"
#include "../templates/rtcc_AlarmTime_Default.h"
#include "../templates/rtcc_AlarmDate_Default.h"

/* Section 3 - PLIB dispatch function definitions */

PLIB_INLINE_API void PLIB_RTCC_Enable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_Enable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_RTCC_Disable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_Disable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsEnableControl(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsEnableControl_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_WriteEnable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_WriteEnable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_RTCC_WriteDisable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_WriteDisable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsWriteEnable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsWriteEnable_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_StopInIdleEnable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_StopInIdleEnable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_RTCC_StopInIdleDisable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_StopInIdleDisable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsStopInIdleControl(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsStopInIdleControl_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_OutputSelect(RTCC_MODULE_ID index, RTCC_OUTPUT_SELECT data)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_OutputSelect_Default(index, data);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsOutputSelect(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsOutputSelect_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_RTCC_ClockSourceSelect(RTCC_MODULE_ID index, RTCC_CLOCK_SOURCE_SELECT source)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_ClockSourceSelect_Unsupported(index, source);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsClockSelect(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsClockSelect_Unsupported(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ClockRunningStatus(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ClockRunningStatus_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsClockRunning(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsClockRunning_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint16_t PLIB_RTCC_DriftCalibrateGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_DriftCalibrateGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint16_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_DriftCalibrateSet(RTCC_MODULE_ID index, uint16_t calibrationbits)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_DriftCalibrateSet_Default(index, calibrationbits);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsCalibration(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsCalibration_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_RTCSyncStatusGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCSyncStatusGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsSynchronization(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsSynchronization_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_HalfSecondStatusGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_HalfSecondStatusGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsHalfSecond(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsHalfSecond_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_ClockOutputEnable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_ClockOutputEnable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_RTCC_ClockOutputDisable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_ClockOutputDisable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsOutputControl(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsOutputControl_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmEnable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmEnable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmDisable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmDisable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsAlarmControl(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsAlarmControl_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmChimeEnable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmChimeEnable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmChimeDisable(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmChimeDisable_Default(index);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsAlarmChimeControl(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsAlarmChimeControl_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmPulseInitialSet(RTCC_MODULE_ID index, bool data)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmPulseInitialSet_Default(index, data);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_AlarmPulseInitialGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmPulseInitialGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsAlarmPulseInitial(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsAlarmPulseInitial_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_AlarmSyncStatusGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmSyncStatusGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsAlarmSynchronization(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsAlarmSynchronization_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmMaskModeSelect(RTCC_MODULE_ID index, RTCC_ALARM_MASK_CONFIGURATION data)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmMaskModeSelect_Default(index, data);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsAlarmMaskControl(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsAlarmMaskControl_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmRepeatCountSet(RTCC_MODULE_ID index, uint8_t data)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmRepeatCountSet_Default(index, data);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint8_t PLIB_RTCC_AlarmRepeatCountGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmRepeatCountGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint8_t)0;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsAlarmRepeatControl(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsAlarmRepeatControl_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_RTCTimeGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCTimeGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_RTCTimeSet(RTCC_MODULE_ID index, uint32_t data)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_RTCTimeSet_Default(index, data);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_RTCHourGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCHourGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_RTCHourSet(RTCC_MODULE_ID index, uint32_t hour)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_RTCHourSet_Default(index, hour);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_RTCMinuteGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCMinuteGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_RTCMinuteSet(RTCC_MODULE_ID index, uint32_t minute)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_RTCMinuteSet_Default(index, minute);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_RTCSecondGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCSecondGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_RTCSecondSet(RTCC_MODULE_ID index, uint32_t second)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_RTCSecondSet_Default(index, second);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsRTCTime(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsRTCTime_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_RTCDateGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCDateGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_RTCDateSet(RTCC_MODULE_ID index, uint32_t data)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_RTCDateSet_Default(index, data);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_RTCYearGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCYearGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_RTCYearSet(RTCC_MODULE_ID index, uint32_t year)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_RTCYearSet_Default(index, year);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_RTCMonthGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCMonthGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_RTCMonthSet(RTCC_MODULE_ID index, uint32_t month)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_RTCMonthSet_Default(index, month);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_RTCDayGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCDayGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_RTCDaySet(RTCC_MODULE_ID index, uint32_t day)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_RTCDaySet_Default(index, day);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_RTCWeekDayGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_RTCWeekDayGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_RTCWeekDaySet(RTCC_MODULE_ID index, uint32_t weekday)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_RTCWeekDaySet_Default(index, weekday);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsRTCDate(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsRTCDate_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_AlarmTimeGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmTimeGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmTimeSet(RTCC_MODULE_ID index, uint32_t data)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmTimeSet_Default(index, data);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_AlarmHourGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmHourGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmHourSet(RTCC_MODULE_ID index, uint32_t hour)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmHourSet_Default(index, hour);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_AlarmMinuteGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmMinuteGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmMinuteSet(RTCC_MODULE_ID index, uint32_t minute)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmMinuteSet_Default(index, minute);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_AlarmSecondGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmSecondGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmSecondSet(RTCC_MODULE_ID index, uint32_t second)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmSecondSet_Default(index, second);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsAlarmTime(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsAlarmTime_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_AlarmDateGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmDateGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmDateSet(RTCC_MODULE_ID index, uint32_t data)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmDateSet_Default(index, data);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_AlarmMonthGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmMonthGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmMonthSet(RTCC_MODULE_ID index, uint32_t month)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmMonthSet_Default(index, month);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_AlarmDayGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmDayGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmDaySet(RTCC_MODULE_ID index, uint32_t day)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmDaySet_Default(index, day);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_RTCC_AlarmWeekDayGet(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_AlarmWeekDayGet_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_RTCC_AlarmWeekDaySet(RTCC_MODULE_ID index, uint32_t weekday)
{
    switch (index) {
        case RTCC_ID_0 :
            RTCC_AlarmWeekDaySet_Default(index, weekday);
            break;
        case RTCC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_RTCC_ExistsAlarmDate(RTCC_MODULE_ID index)
{
    switch (index) {
        case RTCC_ID_0 :
            return RTCC_ExistsAlarmDate_Default(index);
        case RTCC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

#endif
