/*******************************************************************************
Microchip DSP Library
  DSP_FilterIIRBQ16_parallel8:

      int16 DSP_FilterIIRBQ16_parallel8(int16 Xin, PARM_EQUAL_FILTER *pFilter)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_iirbq16_parallel8.S

  Summary:
    Performs a 8 parallel single-pass IIR Biquad Filters, and sums the result.

  Description: 
    Calculates a 8 parallel, single pass IIR biquad filters on Xin, sums the result
    and delivers the result as a 16-bit output.  All math is performed using 32
    bit instructions, which results truncated to 16-bits for the output.  The
    delay register is stored as a 32-bit value for subsequent functions.
    Output is tuned by 2 multipier factors.  First each parallel section has a
    fractional gain (attenuation) that enables individual scaling of that section.
    Second, a global binary (log2N) gain is applied to the result.  The combination
    of gain factors enable both gain and attentuation.
    All values are fractional Q15 and Q31, see data structure for specifics.

Notes:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A coef gain of 2 has been hard coded into the biquad functional block. This
    implies that all coefs should be input at half value. This is purposeful,
    since many filter designs need a div2 to have each coef between the required
    -1<n<1.
    A div 8 has been applied in the function for each block, as the 8 blocks are
    summed to form the end output.  Each block has a functional hard-coded gain
    of 0.125.  This gain is in addition to the programmable values of fracgain
    and expgain.
    Digital attentuation on a per channel basis has been been applied by fracgain.
    This is a Q15 fractional value.
    Binary gain globally has been applied to the first instance of expgain.
    

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
    created    2013 August 5    C15637
 */

	.text
	.set		noreorder
	.set		nomacro

	.global		DSP_FilterIIRBQ16_parallel8
	.ent		DSP_FilterIIRBQ16_parallel8

DSP_FilterIIRBQ16_parallel8:

#notes: 
#  global gain output in function
#  gain param may be required to keep final sum below 16-bits
#  measured 256 passes, each consistent at 266 M4K cycles per pass
#  hard coded gain of 2 for all fiter coefs, they should be input at half value
#  log2alpha currently not functional in this design
#
# int16 DSP_FilterIIRBQ16_parallel8(int16 Xin, PARM_EQUAL_FILTER *pFilter)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 476 cycles
# Cache Prefilled: 246 cycles
#
# $a0 - Xin
# $a1 - pFilter pointer
# $a2 - 
# $a3 - final address, loop compare

# $t0 - z1 delay value, b1 coef
# $t1 - fracGain - output 16-bit fractional gain value
# $t2 - b0 coef (feed forward), a1 coef (feed back), expGain - log2N global gain
# $t3 - b2 coef
# $t4 - a2 coef
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

# $v0 - function 16-bit output
# $v1 - intermediate Y16 value
# Assumption: Delay line array is initialized with 0
    
    li      $v0, 0              # init Y output
    addu    $a3, $a1, 192       # end address is 8*24 (len of filt stucture)

loop:
 	lh		$t1, 0($a1)         # fracGain
    lh      $t2, 6($a1)         # b0 coef
    lw      $t0, 16($a1)        # z1
    
# calc Y
    mul     $t6, $a0, $t2       # Xin * b0
    add     $v1, $t6, $t0       # (Xin * b0) + z1
    sra     $v1, 14             # truncate (Y16), 2 bits gain, one for log2alpha, one for 15 bit math
    
# calc z1 & z2

    lh      $t0, 8($a1)         # b1 coef
    lh      $t2, 12($a1)        # a1 coef
    lw      $t7, 20($a1)        # z2
    lh      $t3, 10($a1)        # b2 coef
    lh      $t4, 14($a1)        # a2 coef

    mul     $t5, $a0, $t0       # Xin * b1
    mul     $t6, $v1, $t2       # Y * a1
    mul     $t8, $a0, $t3       # Xin * b2
    mul     $t9, $v1, $t4       # Y * a2

    mul     $v1, $v1, $t1       # output fractional gain
    
    subu    $t0, $t5, $t6       # (Xin * b1) - (Y * a1)
    add     $t0, $t0, $t7       # (((Xin*b1)) - (Y*a1)) + z2
    sw      $t0, 16($a1)        # store z1

    subu    $t7, $t8, $t9       # (Xin * b2) - (Y * a2)
    sw      $t7, 20($a1)        # store z2

    sra     $v1, 2              # div by 8 because of 8 summing blocks, less 1 (frac gain) mult
    addiu   $a1, 24             # increment to next filter address

    bne     $a3, $a1, loop      # loop while i<8
    add     $v0, $v0, $v1       # add block Y value to summation

##
    lh      $t2, 2-24($a1)      # expGain, binary shift output gain (from last filter coef structure)
    sllv    $v1, $v0, $t2       # global gain, binary shift
    sra     $v1, 16             # shift output to LS byte, truncate result

	jr		$ra                 # return
	seh		$v0, $v1            # end value return, int 16

	.end		DSP_FilterIIRBQ16_parallel8

