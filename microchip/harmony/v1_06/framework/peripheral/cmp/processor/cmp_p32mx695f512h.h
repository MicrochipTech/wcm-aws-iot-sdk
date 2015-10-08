/* Created by plibgen $Revision: 1.30 $ */

#ifndef _CMP_P32MX695F512H_H
#define _CMP_P32MX695F512H_H

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

    CMP_ID_1 = 0,
    CMP_ID_2 = 1,
    CMP_NUMBER_OF_MODULES

} CMP_MODULE_ID;

typedef enum {

    CMP_CVREF_VOLTAGE_SOURCE_NEG_REFERENCE_NONE = 0x00

} CMP_CVREF_VOLTAGE_SOURCE_NEG_REFERENCE;

typedef enum {

    CMP_MASK_C_PWM1H = 0x02,
    CMP_MASK_C_PWM2L = 0x03,
    CMP_MASK_C_PWM2H = 0x04,
    CMP_MASK_C_PWM3L = 0x05,
    CMP_MASK_C_PWM3H = 0x06,
    CMP_MASK_C_PTGO18 = 0x07,
    CMP_MASK_C_PTGO19 = 0x05,
    CMP_MASK_C_FLT2 = 0x06,
    CMP_MASK_C_FLT4 = 0x07

} CMP_MASK_C;

typedef enum {

    CMP_MASK_B_PWM1H = 0x02,
    CMP_MASK_B_PWM2L = 0x03,
    CMP_MASK_B_PWM2H = 0x04,
    CMP_MASK_B_PWM3L = 0x05,
    CMP_MASK_B_PWM3H = 0x06,
    CMP_MASK_B_PTGO18 = 0x07,
    CMP_MASK_B_PTGO19 = 0x05,
    CMP_MASK_B_FLT2 = 0x06,
    CMP_MASK_B_FLT4 = 0x07

} CMP_MASK_B;

typedef enum {

    CMP_MASK_A_PWM1H = 0x02,
    CMP_MASK_A_PWM2L = 0x03,
    CMP_MASK_A_PWM2H = 0x04,
    CMP_MASK_A_PWM3L = 0x05,
    CMP_MASK_A_PWM3H = 0x06,
    CMP_MASK_A_PTGO18 = 0x07,
    CMP_MASK_A_PTGO19 = 0x05,
    CMP_MASK_A_FLT2 = 0x06,
    CMP_MASK_A_FLT4 = 0x07

} CMP_MASK_A;

typedef enum {

    CMP_SPEED_POWER_LOWER = 0x01,
    CMP_SPEED_POWER_HIGHER = 0x02

} CMP_SPEED_POWER;

typedef enum {

    CMP_FILTER_CLOCK_FP = 0x01,
    CMP_FILTER_CLOCK_FOSC = 0x02,
    CMP_FILTER_CLOCK_SYNCO1 = 0x03,
    CMP_FILTER_CLOCK_T2CLK = 0x04,
    CMP_FILTER_CLOCK_T3CLK = 0x05,
    CMP_FILTER_CLOCK_T4CLK = 0x06,
    CMP_FILTER_CLOCK_T5CLK = 0x07

} CMP_FILTER_CLOCK;

typedef enum {

    CMP_FILTER_CLOCK_DIVIDE_1_1 = 0x01,
    CMP_FILTER_CLOCK_DIVIDE_1_2 = 0x02,
    CMP_FILTER_CLOCK_DIVIDE_1_4 = 0x04,
    CMP_FILTER_CLOCK_DIVIDE_1_8 = 0x08,
    CMP_FILTER_CLOCK_DIVIDE_1_16 = 0x10,
    CMP_FILTER_CLOCK_DIVIDE_1_32 = 0x20,
    CMP_FILTER_CLOCK_DIVIDE_1_64 = 0x40,
    CMP_FILTER_CLOCK_DIVIDE_1_128 = 0x80

} CMP_CLOCK_DIVIDE;

typedef enum {

    CMP_CVREF_VALUE_0 = 0x00,
    CMP_CVREF_VALUE_1 = 0x01,
    CMP_CVREF_VALUE_2 = 0x02,
    CMP_CVREF_VALUE_3 = 0x03,
    CMP_CVREF_VALUE_4 = 0x04,
    CMP_CVREF_VALUE_5 = 0x05,
    CMP_CVREF_VALUE_6 = 0x06,
    CMP_CVREF_VALUE_7 = 0x07,
    CMP_CVREF_VALUE_8 = 0x08,
    CMP_CVREF_VALUE_9 = 0x09,
    CMP_CVREF_VALUE_10 = 0x0A,
    CMP_CVREF_VALUE_11 = 0x0B,
    CMP_CVREF_VALUE_12 = 0x0C,
    CMP_CVREF_VALUE_13 = 0x0D,
    CMP_CVREF_VALUE_14 = 0x0E,
    CMP_CVREF_VALUE_15 = 0x0F

} CMP_CVREF_VALUE;

typedef enum {

    CMP_CVREF_VOLTAGE_SOURCE_VDD = 0x00,
    CMP_CVREF_VOLTAGE_SOURCE_EXTERNAL = 0x01

} CMP_CVREF_VOLTAGE_SOURCE;

typedef enum {

    CMP_INTERRUPT_GENERATION_DISABLED = 0x00,
    CMP_INTERRUPT_GENERATION_LOW_TO_HIGH = 0x01,
    CMP_INTERRUPT_GENERATION_HIGH_TO_LOW = 0x02,
    CMP_INTERRUPT_GENERATION_BOTH = 0x03

} CMP_INTERRUPT_EVENT;

typedef enum {

    CMP_INVERTING_INPUT_EXTERNAL_NEGATIVE_PIN = 0x00,
	CMP_INVERTING_INPUT_EXTERNAL_POSITIVE_PIN = 0x01,
	CMP_INVERTING_INPUT_OTHER_MODULE_EXTERNAL_POSITIVE_PIN = 0x02,
	CMP_INVERTING_INPUT_IVREF = 0x03,
	
    CMP_INPUT_C2IN_NEGATIVE = 0x00,
    CMP_INPUT_C2IN_POSITIVE = 0x01,
    CMP_INPUT_C1IN_POSITIVE = 0x02,
    CMP_INPUT_IVREF = 0x03

} CMP_INVERTING_INPUT;

typedef enum {

	CMP_NON_INVERTING_INPUT_EXTERNAL_POSITIVE_PIN = 0x00,
	CMP_NON_INVERTING_INPUT_CVREF = 0x01,
	
    CMP_INPUT_EXTERNAL_PIN = 0x00,
    CMP_INPUT_INTERNAL_CVREF = 0x01

} CMP_NON_INVERTING_INPUT;

typedef enum {

    CMP_CVREF_RESISTOR_LADDER_VOLTAGE = 0x00,
    CMP_CVREF_POSITIVE_REFERENCE_VOLTAGE = 0x01

} CMP_CVREF_REFERENCE_SELECT;

typedef enum {

    CMP_CVREF_BANDGAP_1_2V = 0x00,
    CMP_CVREF_BANDGAP_0_6V = 0x01,
    CMP_CVREF_BANDGAP_VREFPLUS = 0x02

} CMP_CVREF_BANDGAP_SELECT;

PLIB_INLINE SFR_TYPE* _CMP_CMP_ENABLE_CONTROL_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CM1CON;
        case CMP_ID_2 :
            return &CM2CON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_OUTPUT_ENABLE_CONTROL_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CM1CON;
        case CMP_ID_2 :
            return &CM2CON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_INVERT_OUTPUT_CONTROL_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CM1CON;
        case CMP_ID_2 :
            return &CM2CON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_OUTPUT_LEVEL_CONTROL_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CM1CON;
        case CMP_ID_2 :
            return &CM2CON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_INTERRUPT_EVENT_SELECT_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CM1CON;
        case CMP_ID_2 :
            return &CM2CON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_NON_INVERTING_INPUT_SELECTION_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CM1CON;
        case CMP_ID_2 :
            return &CM2CON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_INVERTING_INPUT_SELECTION_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CM1CON;
        case CMP_ID_2 :
            return &CM2CON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_OUTPUT_STATUS_GET1_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CMSTAT;
        case CMP_ID_2 :
            return &CMSTAT;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_OUTPUT_STATUS_GET2_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CMSTAT;
        case CMP_ID_2 :
            return &CMSTAT;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_STOP_IN_IDLE_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CMSTAT;
        case CMP_ID_2 :
            return &CMSTAT;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_CVREF_ENABLE_CONTROL_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CVRCON;
        case CMP_ID_2 :
            return &CVRCON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_CVREF_OUTPUT_ENABLE_CONTROL_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CVRCON;
        case CMP_ID_2 :
            return &CVRCON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_CVREF_WIDE_RANGE_CONTROL_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CVRCON;
        case CMP_ID_2 :
            return &CVRCON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_CVREF_VOLTAGE_RANGE_SELECT_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CVRCON;
        case CMP_ID_2 :
            return &CVRCON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _CMP_CMP_CVREF_VALUE_SELECT_VREG(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return &CVRCON;
        case CMP_ID_2 :
            return &CVRCON;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_ENABLE_CONTROL_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_ON_MASK;
        case CMP_ID_2 :
            return _CM2CON_ON_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_ENABLE_CONTROL_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_COE_MASK;
        case CMP_ID_2 :
            return _CM2CON_COE_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_INVERT_OUTPUT_CONTROL_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_CPOL_MASK;
        case CMP_ID_2 :
            return _CM2CON_CPOL_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_LEVEL_CONTROL_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_COUT_MASK;
        case CMP_ID_2 :
            return _CM2CON_COUT_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_INTERRUPT_EVENT_SELECT_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_EVPOL_MASK;
        case CMP_ID_2 :
            return _CM2CON_EVPOL_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_NON_INVERTING_INPUT_SELECTION_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_CREF_MASK;
        case CMP_ID_2 :
            return _CM2CON_CREF_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_INVERTING_INPUT_SELECTION_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_CCH_MASK;
        case CMP_ID_2 :
            return _CM2CON_CCH_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_STATUS_GET1_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CMSTAT_C1OUT_MASK;
        case CMP_ID_2 :
            return _CMSTAT_C2OUT_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_STATUS_GET2_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CMSTAT_C1OUT_MASK;
        case CMP_ID_2 :
            return _CMSTAT_C2OUT_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_STOP_IN_IDLE_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CMSTAT_SIDL_MASK;
        case CMP_ID_2 :
            return _CMSTAT_SIDL_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_ENABLE_CONTROL_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_ON_MASK;
        case CMP_ID_2 :
            return _CVRCON_ON_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_OUTPUT_ENABLE_CONTROL_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVROE_MASK;
        case CMP_ID_2 :
            return _CVRCON_CVROE_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_WIDE_RANGE_CONTROL_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVRR_MASK;
        case CMP_ID_2 :
            return _CVRCON_CVRR_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_VOLTAGE_RANGE_SELECT_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVRSS_MASK;
        case CMP_ID_2 :
            return _CVRCON_CVRSS_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_VALUE_SELECT_MASK(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVR_MASK;
        case CMP_ID_2 :
            return _CVRCON_CVR_MASK;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_ENABLE_CONTROL_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_ON_POSITION;
        case CMP_ID_2 :
            return _CM2CON_ON_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_ENABLE_CONTROL_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_COE_POSITION;
        case CMP_ID_2 :
            return _CM2CON_COE_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_INVERT_OUTPUT_CONTROL_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_CPOL_POSITION;
        case CMP_ID_2 :
            return _CM2CON_CPOL_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_LEVEL_CONTROL_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_COUT_POSITION;
        case CMP_ID_2 :
            return _CM2CON_COUT_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_INTERRUPT_EVENT_SELECT_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_EVPOL_POSITION;
        case CMP_ID_2 :
            return _CM2CON_EVPOL_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_NON_INVERTING_INPUT_SELECTION_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_CREF_POSITION;
        case CMP_ID_2 :
            return _CM2CON_CREF_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_INVERTING_INPUT_SELECTION_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_CCH_POSITION;
        case CMP_ID_2 :
            return _CM2CON_CCH_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_STATUS_GET1_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CMSTAT_C1OUT_POSITION;
        case CMP_ID_2 :
            return _CMSTAT_C2OUT_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_STATUS_GET2_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CMSTAT_C1OUT_POSITION;
        case CMP_ID_2 :
            return _CMSTAT_C2OUT_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_STOP_IN_IDLE_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CMSTAT_SIDL_POSITION;
        case CMP_ID_2 :
            return _CMSTAT_SIDL_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_ENABLE_CONTROL_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_ON_POSITION;
        case CMP_ID_2 :
            return _CVRCON_ON_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_OUTPUT_ENABLE_CONTROL_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVROE_POSITION;
        case CMP_ID_2 :
            return _CVRCON_CVROE_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_WIDE_RANGE_CONTROL_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVRR_POSITION;
        case CMP_ID_2 :
            return _CVRCON_CVRR_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_VOLTAGE_RANGE_SELECT_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVRSS_POSITION;
        case CMP_ID_2 :
            return _CVRCON_CVRSS_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_VALUE_SELECT_POS(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVR_POSITION;
        case CMP_ID_2 :
            return _CVRCON_CVR_POSITION;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_ENABLE_CONTROL_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_ON_LENGTH;
        case CMP_ID_2 :
            return _CM2CON_ON_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_ENABLE_CONTROL_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_COE_LENGTH;
        case CMP_ID_2 :
            return _CM2CON_COE_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_INVERT_OUTPUT_CONTROL_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_CPOL_LENGTH;
        case CMP_ID_2 :
            return _CM2CON_CPOL_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_LEVEL_CONTROL_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_COUT_LENGTH;
        case CMP_ID_2 :
            return _CM2CON_COUT_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_INTERRUPT_EVENT_SELECT_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_EVPOL_LENGTH;
        case CMP_ID_2 :
            return _CM2CON_EVPOL_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_NON_INVERTING_INPUT_SELECTION_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_CREF_LENGTH;
        case CMP_ID_2 :
            return _CM2CON_CREF_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_INVERTING_INPUT_SELECTION_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CM1CON_CCH_LENGTH;
        case CMP_ID_2 :
            return _CM2CON_CCH_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_STATUS_GET1_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CMSTAT_C1OUT_LENGTH;
        case CMP_ID_2 :
            return _CMSTAT_C2OUT_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_OUTPUT_STATUS_GET2_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CMSTAT_C1OUT_LENGTH;
        case CMP_ID_2 :
            return _CMSTAT_C2OUT_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_STOP_IN_IDLE_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CMSTAT_SIDL_LENGTH;
        case CMP_ID_2 :
            return _CMSTAT_SIDL_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_ENABLE_CONTROL_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_ON_LENGTH;
        case CMP_ID_2 :
            return _CVRCON_ON_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_OUTPUT_ENABLE_CONTROL_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVROE_LENGTH;
        case CMP_ID_2 :
            return _CVRCON_CVROE_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_WIDE_RANGE_CONTROL_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVRR_LENGTH;
        case CMP_ID_2 :
            return _CVRCON_CVRR_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_VOLTAGE_RANGE_SELECT_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVRSS_LENGTH;
        case CMP_ID_2 :
            return _CVRCON_CVRSS_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _CMP_CMP_CVREF_VALUE_SELECT_LEN(CMP_MODULE_ID i)
{
    switch (i) {
        case CMP_ID_1 :
            return _CVRCON_CVR_LENGTH;
        case CMP_ID_2 :
            return _CVRCON_CVR_LENGTH;
        case CMP_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

/* Section 2 - Feature variant inclusion */

#define PLIB_TEMPLATE PLIB_INLINE
#include "../templates/cmp_ComparatorEnableControl_Default.h"
#include "../templates/cmp_ComparatorOutputEnableControl_Default.h"
#include "../templates/cmp_InvertOutputSelectControl_Default.h"
#include "../templates/cmp_OutputLevelSelectControl_Default.h"
#include "../templates/cmp_OutputStatusGet_Default.h"
#include "../templates/cmp_InterruptEventSelect_Default.h"
#include "../templates/cmp_NonInvertingInputSelect_Default.h"
#include "../templates/cmp_InvertingInputSelect_Default.h"
#include "../templates/cmp_StopInIdle_Default.h"
#include "../templates/cmp_CVREFEnableControl_Default.h"
#include "../templates/cmp_CVREFOutputEnableControl_Default.h"
#include "../templates/cmp_CVREFWideRangeControl_Default.h"
#include "../templates/cmp_CVREFVoltageRangeSelect_Default.h"
#include "../templates/cmp_CVREFRefVoltageRangeSelect_Unsupported.h"
#include "../templates/cmp_CVREFBGRefVoltageRangeSelect_Unsupported.h"
#include "../templates/cmp_CVREFValueSelect_Default.h"

/* Section 3 - PLIB dispatch function definitions */

PLIB_INLINE_API bool PLIB_CMP_ExistsEnableControl(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsEnableControl_Default(index);
         case CMP_ID_2 :
            return CMP_ExistsEnableControl_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_Enable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_Enable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_Enable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_CMP_Disable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_Disable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_Disable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsOutputEnableControl(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsOutputEnableControl_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsOutputEnableControl_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_OutputEnable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_OutputEnable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_OutputEnable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_CMP_OutputDisable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_OutputDisable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_OutputDisable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsInvertOutputControl(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsInvertOutputControl_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsInvertOutputControl_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_OutputInvertEnable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_OutputInvertEnable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_OutputInvertEnable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_CMP_OutputInvertDisable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_OutputInvertDisable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_OutputInvertDisable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsOutputLevelControl(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsOutputLevelControl_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsOutputLevelControl_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_OutputLogicHigh(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_OutputLogicHigh_Default(index);
            break;
        case CMP_ID_2 :
            CMP_OutputLogicHigh_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_CMP_OutputLogicLow(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_OutputLogicLow_Default(index);
            break;
        case CMP_ID_2 :
            CMP_OutputLogicLow_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsOutputStatusGet(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsOutputStatusGet_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsOutputStatusGet_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_CMP_OutputStatusGet(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_OutputStatusGet_Default(index);
        case CMP_ID_2 :
            return CMP_OutputStatusGet_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsInterruptEventSelect(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsInterruptEventSelect_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsInterruptEventSelect_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_InterruptEventSelect(CMP_MODULE_ID index, CMP_INTERRUPT_EVENT event)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_InterruptEventSelect_Default(index, event);
            break;
        case CMP_ID_2 :
            CMP_InterruptEventSelect_Default(index, event);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsNonInvertingInputSelect(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsNonInvertingInputSelect_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsNonInvertingInputSelect_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_NonInvertingInputChannelSelect(CMP_MODULE_ID index, CMP_NON_INVERTING_INPUT input)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_NonInvertingInputChannelSelect_Default(index, input);
            break;
        case CMP_ID_2 :
            CMP_NonInvertingInputChannelSelect_Default(index, input);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsInvertingInputSelect(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsInvertingInputSelect_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsInvertingInputSelect_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_InvertingInputChannelSelect(CMP_MODULE_ID index, CMP_INVERTING_INPUT channel)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_InvertingInputChannelSelect_Default(index, channel);
            break;
        case CMP_ID_2 :
            CMP_InvertingInputChannelSelect_Default(index, channel);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsStopInIdle(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsStopInIdle_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsStopInIdle_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_StopInIdleModeEnable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_StopInIdleModeEnable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_StopInIdleModeEnable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_CMP_StopInIdleModeDisable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_StopInIdleModeDisable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_StopInIdleModeDisable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsCVREFEnableControl(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsCVREFEnableControl_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsCVREFEnableControl_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_CVREF_Enable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_Enable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_CVREF_Enable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_CMP_CVREF_Disable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_Disable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_CVREF_Disable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsCVREFOutputEnableControl(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsCVREFOutputEnableControl_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsCVREFOutputEnableControl_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_CVREF_OutputEnable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_OutputEnable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_CVREF_OutputEnable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_CMP_CVREF_OutputDisable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_OutputDisable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_CVREF_OutputDisable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsCVREFWideRangeControl(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsCVREFWideRangeControl_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsCVREFWideRangeControl_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_CVREF_WideRangeEnable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_WideRangeEnable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_CVREF_WideRangeEnable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_CMP_CVREF_WideRangeDisable(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_WideRangeDisable_Default(index);
            break;
        case CMP_ID_2 :
            CMP_CVREF_WideRangeDisable_Default(index);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_CVREF_WideRangeIsEnabled(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_CVREF_WideRangeIsEnabled_Default(index);
        case CMP_ID_2 :
            return CMP_CVREF_WideRangeIsEnabled_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsCVREFVoltageRangeSelect(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsCVREFVoltageRangeSelect_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsCVREFVoltageRangeSelect_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_CVREF_SourceVoltageSelect(CMP_MODULE_ID index, CMP_CVREF_VOLTAGE_SOURCE source)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_SourceVoltageSelect_Default(index, source);
            break;
        case CMP_ID_2 :
            CMP_CVREF_SourceVoltageSelect_Default(index, source);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsCVREFRefVoltageRangeSelect(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsCVREFRefVoltageRangeSelect_Unsupported(index);
        case CMP_ID_2 :
            return CMP_ExistsCVREFRefVoltageRangeSelect_Unsupported(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_CMP_CVREF_ReferenceVoltageSelect(CMP_MODULE_ID index, CMP_CVREF_REFERENCE_SELECT reference)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_ReferenceVoltageSelect_Unsupported(index, reference);
            break;
        case CMP_ID_2 :
            CMP_CVREF_ReferenceVoltageSelect_Unsupported(index, reference);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsCVREFBGRefVoltageRangeSelect(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsCVREFBGRefVoltageRangeSelect_Unsupported(index);
        case CMP_ID_2 :
            return CMP_ExistsCVREFBGRefVoltageRangeSelect_Unsupported(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_CMP_CVREF_BandGapReferenceSourceSelect(CMP_MODULE_ID index, CMP_CVREF_BANDGAP_SELECT select)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_BandGapReferenceSourceSelect_Unsupported(index, select);
            break;
        case CMP_ID_2 :
            CMP_CVREF_BandGapReferenceSourceSelect_Unsupported(index, select);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_CMP_ExistsCVREFValueSelect(CMP_MODULE_ID index)
{
    switch (index) {
        case CMP_ID_1 :
            return CMP_ExistsCVREFValueSelect_Default(index);
        case CMP_ID_2 :
            return CMP_ExistsCVREFValueSelect_Default(index);
        case CMP_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_CMP_CVREF_ValueSelect(CMP_MODULE_ID index, CMP_CVREF_VALUE value)
{
    switch (index) {
        case CMP_ID_1 :
            CMP_CVREF_ValueSelect_Default(index, value);
            break;
        case CMP_ID_2 :
            CMP_CVREF_ValueSelect_Default(index, value);
            break;
        case CMP_NUMBER_OF_MODULES :
        default :
            break;
    }
}

#endif
