/*******************************************************************************
Microchip DSP Library
  DSP_FilterIIRBQ16_cascade8_fast

      int16_t DSP_FilterIIRBQ16_cascade8_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter_Array)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_iirbq16_cascade8_fast.S

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
    All values are fractional Q15, see data structure for specifics.  The function
    will saturate the output should it exceed maximum or minimum values.

Notes:
    All delay registers values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.
    *** optimized for the M14KCe microAptive core.

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
    created    2013 August 9    C15637
 */

	.text
	.set		noreorder
	.set		nomacro
    .set        micromips

	.global		DSP_FilterIIRBQ16_cascade8_fast
	.ent		DSP_FilterIIRBQ16_cascade8_fast

DSP_FilterIIRBQ16_cascade8_fast:

#notes: global gain output not in function, may be required
#  measured 256 passes, each consistent at 198 M14K cycles per pass
#  hard coded gain of 2 for all fiter coefs, they should be input at half value
#  log2alpha currently not functional in this design
#
# int16_t DSP_FilterIIRBQ16_cascade8_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter_Array)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 516 cycles
# Cache Prefilled: 198 cycles
#
# $a0 - Xin
# $a1 - pFilter pointer, increments with loops
# $a2 - 
# $a3 -

# $t0 - z1 delay value
# $t1 - z2 delay value
# $t2 - b0 coef (feed forward),
# $t3 - loop end comparison, $a1 + (loops * filter length)
# $t4 - intermediate sum value
# $t5 - intermediate mult value
# $t6 - intermediate mult values
# $t7 - z2 output value
# $t8 - b2 | b1 coef
# $t9 - a2 | a1 coef

# $s0 - 
# $s1 - 
# $s2 - 
# $s3 - 
# $s4 - 
# $s5 - 
# $s6 - 
# $s7 - 

# $v0 - function 16-bit output
# $v1 - intermediate Y16 value
# Assumption: Delay line array is initialized with 0

    addu        $t3, $a1, 0xA0  # end address is 8*20 (len of filt stucture)
    replv.ph    $a0, $a0        # duplicate Xin, only needed in initial pass

loop:
    lhu         $t2, 6($a1)     # b0 coef
    lhu         $t0, 16($a1)    # z1
    lhu         $t1, 18($a1)    # z2
    lwp         $t8, 8($a1)     # b2 | b1 coef || a2 | a1

# calc Y
    mulq_rs.ph  $t6, $a0, $t2   # Xin * b0
    addq_s.ph   $v1, $t6, $t0   # (Xin * b0) + z1
    shll_s.ph   $v1, $v1, 1     # gain of 2 for log2alpha
    replv.ph    $v1, $v1        # duplicate Y

# calc z1 & z2
    mulq_rs.ph  $t5, $a0, $t8   # Xin * b2 | Xin * b1
    mulq_rs.ph  $t6, $v1, $t9   # Y * a2 | Y * a1

    subq_s.ph   $t4, $t5, $t6   # (Xin * b2) - (Y * a2) | (Xin * b1) - (Y * a1)
    addq_s.ph   $t0, $t4, $t1   # ((Xin * b2) - (Y * a2)) + 0 | ((Xin*b1) - (Y*a1)) + z2

    sw          $t0, 16($a1)    # store z2 | z1

    addiu       $a1, $a1, 20    # increment length of filter structure
    
    bne         $a1, $t3, loop  # loop if not at end address
    move        $a0, $v1        # assign output to the input for each loop pass
    
####

	jr          $ra             # return
	seh         $v0, $v1        # end value return, int 16

	.end		DSP_FilterIIRBQ16_cascade8_fast

