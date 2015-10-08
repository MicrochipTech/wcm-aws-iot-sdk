/*******************************************************************************
Microchip DSP Library
  DSP_FilterIIRBQ16_parallel8_fast:

      int16 DSP_FilterIIRBQ16_parallel8_fast(int16 Xin, PARM_EQUAL_FILTER_16 *pFilter)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_iirbq16_parallel8.S

  Summary:
    Performs a 8 parallel single-pass IIR Biquad Filters, and sums the result.

  Description: 
    Calculates a 8 parallel, single pass IIR biquad filters on Xin, sums the result
    and delivers the result as a 16-bit output.  All math is performed using 16
    bit instructions, which results rounded to 16-bits for the output.  The
    delay register is stored as a 16-bit value for subsequent functions.
    Output is tuned by 2 multipier factors.  First each parallel section has a
    fractional gain (attenuation) that enables individual scaling of that section.
    Second, a global binary (log2N) gain is applied to the result.  The combination
    of gain factors enable both gain and attentuation.
    All values are fractional Q15.  The function will round outputs and saturate
    if maximum or minimum values are exceeded.

Notes:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the biquad functional block. This
    implies that all coefs should be input at half value. This is purposeful,
    since many filter designs need a div2 to have each coef between the required
    -1<n<1.
    A div 8 has been applied in the function for each block, as the 8 blocks are
    summed to form the end output.  Each block has a functional hard-coded gain
    of 0.125.
    Digital attentuation on a per channel basis has been been applied by fracgain.
    This gain is in addition to the programmable values of fracgain and expgain.
    Digital attentuation on a per channel basis has been been applied by fracgain.
    This is a Q15 fractional value.
    Binary gain globally has been applied to the first instance of expgain.
    Code has been optimized for the microAptive core.
    

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

	.global		DSP_FilterIIRBQ16_parallel8_fast
	.ent		DSP_FilterIIRBQ16_parallel8_fast

DSP_FilterIIRBQ16_parallel8_fast:

#notes: 
#  global gain output in function
#  gain param may be required to keep final sum below 16-bits
#  measured 256 passes, each consistent at 234 M14K cycles per pass
#  hard coded gain of 2 for all fiter coefs, they should be input at half value
#  log2alpha currently not functional in this design
#
# int16 DSP_FilterIIRBQ16_parallel8_fast(int16 Xin, PARM_EQUAL_FILTER_16 *pFilter)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 538 cycles
# Cache Prefilled: 198 cycles
#
# $a0 - Xin
# $a1 - pFilter pointer
# $a2 - 
# $a3 - loop counter end value (loops * filter structure)

# $t0 - z1 delay value
# $t1 - z2 delay value
# $t2 - expGain (log2N) global function gain
# $t3 - fracGain - output 16-bit fractional gain value
# $t4 - intermediate add value
# $t5 - intermediate mult value
# $t6 - intermediate mult values
# $t7 - b0 coef (feed forward)
# $t8 - b2 coef | b1 coef
# $t9 - a2 coef | a1 coef (feed back)

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

    li          $v0, 0              # init Y output
    addu        $a3, $a1, 0xA0      # end address is 8*20 (len of filt stucture)
    replv.ph    $a0, $a0            # duplicate Xin, only needed once, all structures have same value

loop:       # loop through 8 parallel filters
 	lhu         $t3, 0($a1)         # fracGain
    lhu         $t7, 6($a1)         # b0 coef
    lhu         $t0, 16($a1)        # z1
    
# calc Y
    mulq_rs.ph  $t6, $a0, $t7       # Xin * b0
    addq_s.ph   $v1, $t6, $t0       # (Xin * b0) + z1
    shll_s.ph   $v1, $v1, 1         # gain of 2 for log2alpha
    replv.ph    $v1, $v1            # duplicate Y
    
# calc z1 & z2
    lhu         $t1, 18($a1)        # z2
    lwp         $t8, 8($a1)         # b2 | b1 coef || a2 | a1

    mulq_rs.ph  $t5, $a0, $t8       # Xin * b2 | Xin * b1
    mulq_rs.ph  $t6, $v1, $t9       # Y * a2 | Y * a1

    mulq_rs.ph  $v1, $v1, $t3       # output fractional gain
    
    subq_s.ph   $t4, $t5, $t6       # (Xin * b2) - (Y * a2) | (Xin * b1) - (Y * a1)
    addq_s.ph   $t0, $t4, $t1       # ((Xin * b2) - (Y * a2)) + 0 | ((Xin*b1) - (Y*a1)) + z2

    sw          $t0, 16($a1)        # store z2 | z1

    shra_r.ph   $v1, $v1, 3         # shift right 3 bits, div 8 because of 8 parallel blocks summing

    addiu       $a1, 20             # increment to next filter address

    bne         $a3, $a1, loop      # loop while i<8
    addq_s.ph   $v0, $v0, $v1       # add Y from this filter to summation

####
    lh          $t2, 2-20($a1)      # expGain, binary shift output gain
    shllv_s.ph  $v0, $v0, $t2       # shift expGain

	jr          $ra                 # return
	seh         $v0, $v0            # end value return, int 16 [delay slot]

	.end		DSP_FilterIIRBQ16_parallel8_fast

