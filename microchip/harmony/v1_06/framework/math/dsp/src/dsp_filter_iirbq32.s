/*******************************************************************************
Microchip DSP Library
  DSP_FilterIIRBQ32:

      int32_t DSP_FilterIIRBQ32(int32_t Xin, PARM_EQUAL_FILTER_32 *pFilter)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_iirbq32.S

  Summary:
    Performs a single-pass IIR Biquad Filter.

  Description:
    Calculates a single pass IIR biquad filter on Xin, and delivers the result 
    as a 16-bit output.  All math is performed using 32 bit instructions, with
    results truncated to 32-bits for the output.  The delay register is stored
    as a 32-bit value for subsequent functions.
    All values are fractional Q31, see data structure for specifics.

Notes:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.
    ** optimized for M14K microAptiv core

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
    created    2013 July 26    C15637
 */

	.text
	.set		noreorder
	.set		nomacro
    .set        micromips

	.global		DSP_FilterIIRBQ32
	.ent		DSP_FilterIIRBQ32

DSP_FilterIIRBQ32:

#notes: global gain output not in function, may be required
#  measured 256 passes, each consistent at 34 M14K cycles per pass
#  hard coded gain of 2 for all fiter coefs, they should be input at half value
#  log2alpha currently not functional in this design
#
# int32_t DSP_FilterIIRBQ32(int32_t Xin, PARM_EQUAL_FILTER_32 *pFilter)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 178 cycles
# Cache Prefilled: 34 cycles
#
# $a0 - Xin
# $a1 - pFilter pointer
# $a2 - 
# $a3 -

# $t0 - z1 delay value
# $t1 - z2 delay value
# $t2 - 
# $t3 - b0 coef (feed forward)
# $t4 - b1 coef
# $t5 - b2 coef
# $t6 - a1 coef (feed back)
# $t7 - a2 coef
# $t8 - 
# $t9 - 

# $s0 - 
# $s1 - 
# $s2 - 
# $s3 - 
# $s4 - 
# $s5 - 
# $s6 - 
# $s7 - 

# $v0 - function 32-bit output
# $v1 - 
# Assumption: Delay line array is initialized with 0

    lw      $t3, 8($a1)         # b0 coef
    lwp     $t0, 28($a1)        # t0 = z1, t1 = z2
    lwp     $t4, 12($a1)        # t4 = b1, t5 = b2
    lwp     $t6, 20($a1)        # t6 = a1, t7 = a2

# calc Y
    mtlo    $zero
    mthi    $t0, $ac0           # z1
    madd    $ac0, $a0, $t3      # [z1] + Xin * b0
    mfhi    $v0, $ac0
    sll     $v0, 2              # 2 bits gain, one for log2alpha, one for 31 bit math
    
# calc z1 ($ac1) & z2 ($ac2)
    mtlo    $zero, $ac1
    mthi    $t1, $ac1           # load z2
    madd    $ac1, $a0, $t4      # [z2] + Xin * b1
    mult    $ac2, $a0, $t5      # Xin * b2
    msub    $ac1, $v0, $t6      # [z2 + (Xin * b1)] - (Y * a1)
    msub    $ac2, $v0, $t7      # [Xin * b2] - (Y * a2)
    
    mfhi    $t0, $ac1           # get z1
    mfhi    $t1, $ac2           # get z2

####

	jr		$ra                 # return
    swp     $t0, 28($a1)        # store z1 & z2

	.end		DSP_FilterIIRBQ32

