/* Created by plibgen $Revision: 1.31 $ */

#ifndef _ADC_P32MX695F512H_H
#define _ADC_P32MX695F512H_H

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

    ADC_ID_1 = 0,
    ADC_NUMBER_OF_MODULES

} ADC_MODULE_ID;

typedef enum {

    ADC_MUX_A = 0x00,
    ADC_MUX_B = 0x01

} ADC_MUX;

typedef enum {

    ADC_FILLING_BUF_0TO7 = 0x00,
    ADC_FILLING_BUF_8TOF = 0x01

} ADC_RESULT_BUF_STATUS;

typedef enum {

    ADC_REFERENCE_VDD_TO_AVSS = 0x00,
    ADC_REFERENCE_VREFPLUS_TO_AVSS = 0x01,
    ADC_REFERENCE_AVDD_TO_VREF_NEG = 0x02,
    ADC_REFERENCE_VREFPLUS_TO_VREFNEG = 0x03

} ADC_VOLTAGE_REFERENCE;

typedef enum {

    ADC_SAMPLING_MODE_ALTERNATE_INPUT = 0x01,
    ADC_SAMPLING_MODE_MUXA = 0x00

} ADC_SAMPLING_MODE;

typedef enum {

    ADC_1SAMPLE_PER_INTERRUPT = 0x00,
    ADC_2SAMPLES_PER_INTERRUPT = 0x01,
    ADC_3SAMPLES_PER_INTERRUPT = 0x02,
    ADC_4SAMPLES_PER_INTERRUPT = 0x03,
    ADC_5SAMPLES_PER_INTERRUPT = 0x04,
    ADC_6SAMPLES_PER_INTERRUPT = 0x05,
    ADC_7SAMPLES_PER_INTERRUPT = 0x06,
    ADC_8SAMPLES_PER_INTERRUPT = 0x07,
    ADC_9SAMPLES_PER_INTERRUPT = 0x08,
    ADC_10SAMPLES_PER_INTERRUPT = 0x09,
    ADC_11SAMPLES_PER_INTERRUPT = 0x0A,
    ADC_12SAMPLES_PER_INTERRUPT = 0x0B,
    ADC_13SAMPLES_PER_INTERRUPT = 0x0C,
    ADC_14SAMPLES_PER_INTERRUPT = 0x0D,
    ADC_15SAMPLES_PER_INTERRUPT = 0x0E,
    ADC_16SAMPLES_PER_INTERRUPT = 0x0F

} ADC_SAMPLES_PER_INTERRUPT;

typedef enum {

    ADC_INPUT_POSITIVE_AN0 = 0x00,
    ADC_INPUT_POSITIVE_AN1 = 0x01,
    ADC_INPUT_POSITIVE_AN2 = 0x02,
    ADC_INPUT_POSITIVE_AN3 = 0x03,
    ADC_INPUT_POSITIVE_AN4 = 0x04,
    ADC_INPUT_POSITIVE_AN5 = 0x05,
    ADC_INPUT_POSITIVE_AN6 = 0x06,
    ADC_INPUT_POSITIVE_AN7 = 0x07,
    ADC_INPUT_POSITIVE_AN8 = 0x08,
    ADC_INPUT_POSITIVE_AN9 = 0x09,
    ADC_INPUT_POSITIVE_AN10 = 0x0A,
    ADC_INPUT_POSITIVE_AN11 = 0x0B,
    ADC_INPUT_POSITIVE_AN12 = 0x0C,
    ADC_INPUT_POSITIVE_AN13 = 0x0D,
    ADC_INPUT_POSITIVE_AN14 = 0x0E,
    ADC_INPUT_POSITIVE_AN15 = 0x0F

} ADC_INPUTS_POSITIVE;

typedef enum {

    ADC_INPUT_SCAN_AN0 = 0x0001,
    ADC_INPUT_SCAN_AN1 = 0x0002,
    ADC_INPUT_SCAN_AN2 = 0x0004,
    ADC_INPUT_SCAN_AN3 = 0x0008,
    ADC_INPUT_SCAN_AN4 = 0x0010,
    ADC_INPUT_SCAN_AN5 = 0x0020,
    ADC_INPUT_SCAN_AN6 = 0x0040,
    ADC_INPUT_SCAN_AN7 = 0x0080,
    ADC_INPUT_SCAN_AN8 = 0x0100,
    ADC_INPUT_SCAN_AN9 = 0x0200,
    ADC_INPUT_SCAN_AN10 = 0x0400,
    ADC_INPUT_SCAN_AN11 = 0x0800,
    ADC_INPUT_SCAN_AN12 = 0x1000,
    ADC_INPUT_SCAN_AN13 = 0x2000,
    ADC_INPUT_SCAN_AN14 = 0x4000,
    ADC_INPUT_SCAN_AN15 = 0x8000

} ADC_INPUTS_SCAN;

typedef enum {

    ADC_INPUTS_SCAN_EXTENDED_NONE

} ADC_INPUTS_SCAN_EXTENDED;

typedef enum {

    ADC_INPUT_NEGATIVE_VREF_MINUS = 0x00,
    ADC_INPUT_NEGATIVE_AN1 = 0x01

} ADC_INPUTS_NEGATIVE;

typedef enum {

    ADC_CLOCK_SOURCE_SYSTEM_CLOCK = 0x00,
    ADC_CLOCK_SOURCE_INTERNAL_RC = 0x01

} ADC_CLOCK_SOURCE;

typedef enum {

    ADC_CONVERSION_TRIGGER_SAMP_CLEAR = 0x00,
    ADC_CONVERSION_TRIGGER_INT0_TRANSITION = 0x01,
    ADC_CONVERSION_TRIGGER_TMR3_COMPARE_MATCH = 0x02,
    ADC_CONVERSION_TRIGGER_CTMU_EVENT = 0x03,
    ADC_CONVERSION_TRIGGER_INTERNAL_COUNT = 0x07

} ADC_CONVERSION_TRIGGER_SOURCE;

typedef enum {

    ADC_RESULT_FORMAT_INTEGER_16BIT = 0x00,
    ADC_RESULT_FORMAT_SIGNED_INTEGER_16BIT = 0x01,
    ADC_RESULT_FORMAT_FRACTIONAL_16BIT = 0x02,
    ADC_RESULT_FORMAT_SIGNED_FRACTIONAL_16BIT = 0x03,
    ADC_RESULT_FORMAT_INTEGER_32BIT = 0x04,
    ADC_RESULT_FORMAT_SIGNED_INTEGER_32BIT = 0x05,
    ADC_RESULT_FORMAT_FRACTIONAL_32BIT = 0x06,
    ADC_RESULT_FORMAT_SIGNED_FRACTIONAL_32BIT = 0x07

} ADC_RESULT_FORMAT;

typedef enum {

    ADC_BUFFER_MODE_ONE_16WORD_BUFFER = 0x00,
    ADC_BUFFER_MODE_TWO_8WORD_BUFFERS = 0x01

} ADC_BUFFER_MODE;

PLIB_INLINE SFR_TYPE* _ADC_CONVERSION_STATUS_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_CONVERSION_CONTROL_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_SAMPLING_CONTROL_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_SAMPLING_AUTO_START_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_CONVERSION_STOP_SEQUENCE_CONTROL_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_CONVERSION_TRIGGER_SOURCE_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_FORMAT_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_STOP_IN_IDLE_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_ENABLE_CONTROL_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_SAMPLING_MODE_CONTROL_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON2;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_BUFFER_MODE_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON2;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_SAMPLES_PER_INTERRUPT_SELECT_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON2;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_BUFFER_FILL_STATUS_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON2;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_MUX_INPUT_SCAN_CONTROL_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON2;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_CALIBRATION_CONTROL_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON2;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_VOLTAGE_REFERENCE_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON2;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_CONVERSION_CLOCK_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON3;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_SAMPLING_ACQUISITION_TIME_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON3;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_CONVERSION_CLOCK_SOURCE_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CON3;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_MUXA_POSITIVE_INPUT_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CHS;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_MUXA_NEGATIVE_INPUT_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CHS;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_MUXB_POSITIVE_INPUT_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CHS;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_MUXB_NEGATIVE_INPUT_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CHS;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_MUX_INPUT_SCAN_SELECT_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &AD1CSSL;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_0_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_1_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_2_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF2;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_3_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF3;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_4_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF4;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_5_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF5;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_6_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF6;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_7_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF7;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_8_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF8;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_9_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUF9;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_A_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUFA;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_B_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUFB;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_C_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUFC;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_D_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUFD;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_E_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUFE;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _ADC_RESULT_GETBY_INDEX_F_VREG(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return &ADC1BUFF;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_STATUS_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_DONE_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_CONTROL_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SAMP_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_CONTROL_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SAMP_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_AUTO_START_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_ASAM_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_STOP_SEQUENCE_CONTROL_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_CLRASAM_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_TRIGGER_SOURCE_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SSRC_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_FORMAT_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_FORM_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_STOP_IN_IDLE_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SIDL_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_ENABLE_CONTROL_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_ON_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_MODE_CONTROL_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_ALTS_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_BUFFER_MODE_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_BUFM_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLES_PER_INTERRUPT_SELECT_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_SMPI_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_BUFFER_FILL_STATUS_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_BUFS_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUX_INPUT_SCAN_CONTROL_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_CSCNA_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CALIBRATION_CONTROL_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_OFFCAL_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_VOLTAGE_REFERENCE_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_VCFG_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_CLOCK_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON3_ADCS_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_ACQUISITION_TIME_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON3_SAMC_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_CLOCK_SOURCE_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON3_ADRC_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXA_POSITIVE_INPUT_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0SA_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXA_NEGATIVE_INPUT_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0NA_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXB_POSITIVE_INPUT_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0SB_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXB_NEGATIVE_INPUT_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0NB_MASK;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUX_INPUT_SCAN_SELECT_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_0_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_1_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_2_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_3_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_4_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_5_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_6_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_7_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_8_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_9_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_A_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_B_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_C_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_D_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_E_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_F_MASK(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)-1;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_STATUS_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_DONE_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_CONTROL_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SAMP_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_CONTROL_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SAMP_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_AUTO_START_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_ASAM_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_STOP_SEQUENCE_CONTROL_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_CLRASAM_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_TRIGGER_SOURCE_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SSRC_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_FORMAT_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_FORM_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_STOP_IN_IDLE_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SIDL_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_ENABLE_CONTROL_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_ON_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_MODE_CONTROL_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_ALTS_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_BUFFER_MODE_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_BUFM_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLES_PER_INTERRUPT_SELECT_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_SMPI_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_BUFFER_FILL_STATUS_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_BUFS_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUX_INPUT_SCAN_CONTROL_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_CSCNA_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CALIBRATION_CONTROL_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_OFFCAL_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_VOLTAGE_REFERENCE_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_VCFG_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_CLOCK_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON3_ADCS_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_ACQUISITION_TIME_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON3_SAMC_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_CLOCK_SOURCE_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON3_ADRC_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXA_POSITIVE_INPUT_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0SA_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXA_NEGATIVE_INPUT_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0NA_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXB_POSITIVE_INPUT_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0SB_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXB_NEGATIVE_INPUT_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0NB_POSITION;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUX_INPUT_SCAN_SELECT_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_0_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_1_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_2_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_3_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_4_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_5_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_6_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_7_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_8_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_9_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_A_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_B_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_C_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_D_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_E_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_F_POS(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)0;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_STATUS_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_DONE_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_CONTROL_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SAMP_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_CONTROL_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SAMP_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_AUTO_START_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_ASAM_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_STOP_SEQUENCE_CONTROL_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_CLRASAM_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_TRIGGER_SOURCE_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SSRC_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_FORMAT_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_FORM_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_STOP_IN_IDLE_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_SIDL_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_ENABLE_CONTROL_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON1_ON_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_MODE_CONTROL_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_ALTS_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_BUFFER_MODE_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_BUFM_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLES_PER_INTERRUPT_SELECT_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_SMPI_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_BUFFER_FILL_STATUS_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_BUFS_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUX_INPUT_SCAN_CONTROL_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_CSCNA_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CALIBRATION_CONTROL_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_OFFCAL_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_VOLTAGE_REFERENCE_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON2_VCFG_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_CLOCK_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON3_ADCS_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_SAMPLING_ACQUISITION_TIME_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON3_SAMC_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_CONVERSION_CLOCK_SOURCE_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CON3_ADRC_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXA_POSITIVE_INPUT_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0SA_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXA_NEGATIVE_INPUT_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0NA_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXB_POSITIVE_INPUT_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0SB_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUXB_NEGATIVE_INPUT_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return _AD1CHS_CH0NB_LENGTH;
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_MUX_INPUT_SCAN_SELECT_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_0_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_1_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_2_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_3_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_4_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_5_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_6_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_7_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_8_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_9_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_A_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_B_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_C_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_D_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_E_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _ADC_RESULT_GETBY_INDEX_F_LEN(ADC_MODULE_ID i)
{
    switch (i) {
        case ADC_ID_1 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

/* Section 2 - Feature variant inclusion */

#define PLIB_TEMPLATE PLIB_INLINE
#include "../templates/adc_EnableControl_Default.h"
#include "../templates/adc_StopInIdle_Default.h"
#include "../templates/adc_CalibrationControl_Default.h"
#include "../templates/adc_MuxInputScanSelect_Default.h"
#include "../templates/adc_MuxInputScanSelectExtended_Unsupported.h"
#include "../templates/adc_VoltageReference_Default.h"
#include "../templates/adc_MuxChannel0PositiveInput_Default.h"
#include "../templates/adc_MuxChannel0NegativeInput_Default.h"
#include "../templates/adc_MuxInputScanControl_Default.h"
#include "../templates/adc_SamplingModeControl_Default.h"
#include "../templates/adc_SamplesPerInterruptSelect_Default.h"
#include "../templates/adc_SamplingAutoStart_Default.h"
#include "../templates/adc_SamplingControl_Default.h"
#include "../templates/adc_SamplingAcquisitionTime_Default.h"
#include "../templates/adc_SamplingStatus_Default.h"
#include "../templates/adc_ConversionControl_Default.h"
#include "../templates/adc_ConversionClock_Default.h"
#include "../templates/adc_ConversionClockSource_Default.h"
#include "../templates/adc_ConversionTriggerSource_Default.h"
#include "../templates/adc_ConversionStopSequenceControl_Default.h"
#include "../templates/adc_ConversionStatus_Default.h"
#include "../templates/adc_ResultFormat_Default.h"
#include "../templates/adc_ResultBufferMode_Default.h"
#include "../templates/adc_ResultBufferFillStatus_Default.h"
#include "../templates/adc_ResultGetByIndex_Default.h"

/* Section 3 - PLIB dispatch function definitions */

PLIB_INLINE_API void PLIB_ADC_Enable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_Enable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_ADC_Disable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_Disable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsEnableControl(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsEnableControl_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_StopInIdleEnable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_StopInIdleEnable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_ADC_StopInIdleDisable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_StopInIdleDisable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsStopInIdleControl(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsStopInIdleControl_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_CalibrationEnable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_CalibrationEnable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_ADC_CalibrationDisable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_CalibrationDisable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsCalibrationControl(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsCalibrationControl_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_InputScanMaskAdd(ADC_MODULE_ID index, ADC_INPUTS_SCAN scanInputs)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_InputScanMaskAdd_Default(index, scanInputs);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_ADC_InputScanMaskRemove(ADC_MODULE_ID index, ADC_INPUTS_SCAN scanInputs)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_InputScanMaskRemove_Default(index, scanInputs);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsMuxInputScanSelect(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsMuxInputScanSelect_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_ADC_InputScanMaskAddExtended(ADC_MODULE_ID index, ADC_INPUTS_SCAN_EXTENDED scanInputs)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_InputScanMaskAddExtended_Unsupported(index, scanInputs);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_ADC_InputScanMaskRemoveExtended(ADC_MODULE_ID index, ADC_INPUTS_SCAN_EXTENDED scanInputs)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_InputScanMaskRemoveExtended_Unsupported(index, scanInputs);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsMuxInputScanSelectExtended(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsMuxInputScanSelectExtended_Unsupported(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_VoltageReferenceSelect(ADC_MODULE_ID index, ADC_VOLTAGE_REFERENCE configValue)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_VoltageReferenceSelect_Default(index, configValue);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsVoltageReference(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsVoltageReference_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_MuxChannel0InputPositiveSelect(ADC_MODULE_ID index, ADC_MUX muxType, ADC_INPUTS_POSITIVE input)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_MuxChannel0InputPositiveSelect_Default(index, muxType, input);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsMuxChannel0PositiveInput(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsMuxChannel0PositiveInput_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_MuxChannel0InputNegativeSelect(ADC_MODULE_ID index, ADC_MUX muxType, ADC_INPUTS_NEGATIVE input)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_MuxChannel0InputNegativeSelect_Default(index, muxType, input);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsMuxChannel0NegativeInput(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsMuxChannel0NegativeInput_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_MuxAInputScanEnable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_MuxAInputScanEnable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_ADC_MuxAInputScanDisable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_MuxAInputScanDisable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsMuxInputScanControl(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsMuxInputScanControl_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_SamplingModeSelect(ADC_MODULE_ID index, ADC_SAMPLING_MODE mode)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_SamplingModeSelect_Default(index, mode);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsSamplingModeControl(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsSamplingModeControl_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_SamplesPerInterruptSelect(ADC_MODULE_ID index, ADC_SAMPLES_PER_INTERRUPT value)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_SamplesPerInterruptSelect_Default(index, value);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsSamplesPerInterruptSelect(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsSamplesPerInterruptSelect_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_SampleAutoStartEnable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_SampleAutoStartEnable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_ADC_SampleAutoStartDisable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_SampleAutoStartDisable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsSamplingAutoStart(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsSamplingAutoStart_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_SamplingStart(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_SamplingStart_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_ADC_SamplingStop(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_SamplingStop_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsSamplingControl(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsSamplingControl_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_SampleAcquisitionTimeSet(ADC_MODULE_ID index, ADC_ACQUISITION_TIME acqTime)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_SampleAcquisitionTimeSet_Default(index, acqTime);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsSamplingAcquisitionTime(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsSamplingAcquisitionTime_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_ADC_SamplingIsActive(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_SamplingIsActive_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsSamplingStatus(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsSamplingStatus_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_ConversionStart(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_ConversionStart_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsConversionControl(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsConversionControl_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_ConversionClockSet(ADC_MODULE_ID index, uint32_t baseFrequency, ADC_CONVERSION_CLOCK value)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_ConversionClockSet_Default(index, baseFrequency, value);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API ADC_CONVERSION_CLOCK PLIB_ADC_ConversionClockGet(ADC_MODULE_ID index, uint32_t baseFrequency)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ConversionClockGet_Default(index, baseFrequency);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (ADC_CONVERSION_CLOCK)0;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsConversionClock(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsConversionClock_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_ConversionClockSourceSelect(ADC_MODULE_ID index, ADC_CLOCK_SOURCE source)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_ConversionClockSourceSelect_Default(index, source);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsConversionClockSource(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsConversionClockSource_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_ConversionTriggerSourceSelect(ADC_MODULE_ID index, ADC_CONVERSION_TRIGGER_SOURCE source)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_ConversionTriggerSourceSelect_Default(index, source);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsConversionTriggerSource(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsConversionTriggerSource_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_ConversionStopSequenceEnable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_ConversionStopSequenceEnable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_ADC_ConversionStopSequenceDisable(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_ConversionStopSequenceDisable_Default(index);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsConversionStopSequenceControl(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsConversionStopSequenceControl_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ConversionHasCompleted(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ConversionHasCompleted_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsConversionStatus(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsConversionStatus_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_ResultFormatSelect(ADC_MODULE_ID index, ADC_RESULT_FORMAT resultFormat)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_ResultFormatSelect_Default(index, resultFormat);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsResultFormat(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsResultFormat_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_ADC_ResultBufferModeSelect(ADC_MODULE_ID index, ADC_BUFFER_MODE mode)
{
    switch (index) {
        case ADC_ID_1 :
            ADC_ResultBufferModeSelect_Default(index, mode);
            break;
        case ADC_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsResultBufferMode(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsResultBufferMode_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API ADC_RESULT_BUF_STATUS PLIB_ADC_ResultBufferStatusGet(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ResultBufferStatusGet_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (ADC_RESULT_BUF_STATUS)0;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsResultBufferFillStatus(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsResultBufferFillStatus_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API ADC_SAMPLE PLIB_ADC_ResultGetByIndex(ADC_MODULE_ID index, uint8_t bufferIndex)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ResultGetByIndex_Default(index, bufferIndex);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (ADC_SAMPLE)0;
    }
}

PLIB_INLINE_API bool PLIB_ADC_ExistsResultGetByIndex(ADC_MODULE_ID index)
{
    switch (index) {
        case ADC_ID_1 :
            return ADC_ExistsResultGetByIndex_Default(index);
        case ADC_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

#endif
