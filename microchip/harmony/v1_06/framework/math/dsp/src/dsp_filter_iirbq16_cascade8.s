/*******************************************************************************
Microchip DSP Library
  DSP_FilterIIRBQ16_cascade8

      int16_t DSP_FilterIIRBQ16_cascade8(int16_t Xin, PARM_EQUAL_FILTER *pFilter_Array)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_iirbq16_cascade8.S

  Summary:
    Performs a single IIR Biquad Filter as a cascade of 8 series filters.

  Description: 
    Calculates a single pass IIR biquad cascade filter on Xin, and delivers the
    result as a 16-bit output.  The cascade of filters is 8 unique biquad filters
    arranged in series such that the output of one is provided as the input to
    the next.  A unique filter coefficient set is provided to each, and 32 bit
    delay lines are maintained for each.  All math is performed using 32 bit
    instructions, which results truncated to 16-bits for the output.  The delay
    register is stored as a 32-bit value for subsequent functions.
    Global gain values are available on the output. Fracgain is a Q15 fractional
    gain value and expgain is a binary shift gain value.  The combination of the
    two can be utilized to normalize the output as desired.
    All values are fractional Q15 and Q31, see data structure for specifics.

Notes:
    All delay registers values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.

*******************************************************************************/

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
/*
    created    2013 August 1    C15637
 */

	.text
	.set		noreorder
	.set		nomacro

	.global		DSP_FilterIIRBQ16_cascade8
	.ent		DSP_FilterIIRBQ16_cascade8

DSP_FilterIIRBQ16_cascade8:

#notes: global gain output not in function, may be required
#  measured 256 passes, each consistent at 246 M4K cycles per pass
#  hard coded gain of 2 for all fiter coefs, they should be input at half value
#  log2alpha currently not functional in this design
#
# int16_t DSP_FilterIIRBQ16_cascade8(int16_t Xin, PARM_EQUAL_FILTER *pFilter_Array)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 500 cycles
# Cache Prefilled: 246 cycles
#
# $a0 - Xin
# $a1 - pFilter pointer, increments with loops
# $a2 - b1 coef
# $a3 - b2 coef

# $t0 - z1 delay value
# $t1 - fracGain (Q15 fractional global gain)
# $t2 - expGain (binary shift global gain)
# $t3 - b0 coef (feed forward), a2 coef
# $t4 - loop end comparison, $a1 + (loops * filter length)
# $t5 - intermediate mult value
# $t6 - intermediate mult values
# $t7 - z2 delay value
# $t8 - intermediate mult value
# $t9 - intermediate mult value

# $s0 -
# $s1 -
# $s2 -
# $s3 -
# $s4 -
# $s5 -
# $s6 -
# $s7 -

# $at -

# $v0 - function 16-bit output
# $v1 - intermediate Y16 value
# Assumption: Delay line array is initialized with 0
#  to save cycles, only save the $s registers when used

    addu    $t4, $a1, 192       # end address is 8*24 (len of filt stucture)

loop:
    lh      $t3, 6($a1)         # b0 coef
    lw      $t0, 16($a1)        # z1
    lh      $a2, 8($a1)         # b1 coef
    lh      $v0, 12($a1)        # a1 coef
    lw      $t7, 20($a1)        # z2
    lh      $a3, 10($a1)        # b2 coef

# calc Y
    mul     $t6, $a0, $t3       # Xin * b0
    add     $v1, $t6, $t0       # Y = (Xin * b0) + z1
    sra     $v1, 14             # truncate (Y16), 2 bits gain, one for log2alpha, one for 15 bit math

    lh      $t3, 14($a1)        # a2 coef

# calc z1 & z2
    mul     $t5, $a0, $a2       # Xin * b1
    mul     $t6, $v1, $v0       # Y * a1
    mul     $t8, $a0, $a3       # Xin * b2
    mul     $t9, $v1, $t3       # Y * a2

    subu    $t0, $t5, $t6       # (Xin * b1) - (Y * a1)
    add     $t0, $t0, $t7       # (((Xin*b1)) - (Y*a1)) + z2
    sw      $t0, 16($a1)        # store z1

    subu    $t7, $t8, $t9       # (Xin * b2) - (Y * a2)
    sw      $t7, 20($a1)        # store z2

    add     $a1, $a1, 24        # increment length of filter structure

    bne     $a1, $t4, loop      # loop if not at end address
    move    $a0, $v1            # assign output to the input for each loop pass

####
    lh      $t1, -24($a1)       # fracGain
    lh      $t2, -22($a1)       # expGain, binary shift output gain

    mul     $v1, $v1, $t1       # frac gain factor
    sllv    $v1, $v1, $t2       # exp gain factor
    sra     $v1, 15             # move data into LSB, one additional bit for 16 bit math

    jr      $ra                 # return
    seh     $v0, $v1            # end value return, int 16 [delay slot]

	.end		DSP_FilterIIRBQ16_cascade8

