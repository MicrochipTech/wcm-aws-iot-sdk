/*******************************************************************************
Microchip DSP Library
  DSP_FilterFIRInterp32:

      void DSP_FilterFIRInterp32(int32 *outdata, int32 *indata, int32 *coeffs,
            int32 *delayline, int N, int K, int scale, int rate)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_fir_interp32.S

  Summary:
    Performs an interpolating FIR filter on the input array.

  Description:
    Perform an interpolating FIR filter on the first N samples of indata, and
    stores the result in outdata. The number of output elements is N*rate. The
    number of filter taps, K, must be an even multiple of N. The coefficients
    array, Coeffs, must be K elements long. The delay line array, delayline,
    must be K/R elements long, and be initialzed to zero. All data elements must
    be in Q31 fractional data format. The function will saturate the output if
    it exceeds maximum or minimum values.
    The function creates R output values for each input value processed. The
    delayline of previous values is processed with R elements of the coefficient
    array. Numerically:

    Y(1,0) = X(0)*C(0) + X(-1)*C(rate) + X(-2)*C(2*rate) ...
    Y(1,1) = X(0)*C(1) + X(-1)*C(rate+1) + X(-2)*C(2*rate + 1) ...
    Y(1,rate) = X(0)*C(N) + X(-1)*C(rate+N) + X(-2)*C(2*rate + N) ...

    where output Y corresponds to rate different outputs, input X has sample
    delays and C is the coefficient array.

Notes:
    K (taps) must be an even multiple of R (rate).
    outdata must have R*N elements.

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
 * mips_firinter32.s
 *
 *  Created on: Mar 11, 2013
 *      Author: C15036
 *  Updated 2013 July 22   C15637
 */
	.text
	.set		noreorder
	.set		nomacro
    .set        micromips

	.global		DSP_FilterFIRInterp32
	.ent		DSP_FilterFIRInterp32

DSP_FilterFIRInterp32:

# void DSP_FilterFIRInterp32(int32 *outdata, int32 *indata, int32 *coeffs, int32 *delayline,
#				int N, int K, int scale, int rate)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 890 cycles
# Cache Prefilled: 638 cycles
#
# $a0 - outdata ptr, is updated every outer loop iteration
# $a1 - indata ptr, is updated every outer loop iteration
# $a2 - coeffs
# $a3 - delayline, is an array of 'K' elements
# 16($sp) - N, number of input samples
# 20($sp) - K, number of filter coefficients
# 24($sp) - scale
# 28($sp) - R, Interpolation rate factor
# $t0 - Temporary data (coefficient value)
# $t1 - Temporary data (delay line value)
# $t2 - Temporary data place holder (indata in LOOP2 and outdata in LOOP3)
# $t3 - Coef offset counter in LOOP2
# $t4 - Temporary count variable 'k' in LOOP2
# $t5 - 0x80000000, Rounding value loaded into accxlo in every outer loop iteration
# $t6 - 
# $t7 - temporary pointer for coeffs array
# $t8 - temporary pointer for delayline array
# $t9 - placeholder for $t8 value before entering LOOP2

# $s0 - N (Number of input samples)
# $s1 - K ( Number of filter taps ), later K*sizeof(int32)
# $s2 - scale, binary right shift of input data
# $s3 - R (Interpolation rate), later R*sizeof(int32)
# $s4 - K-1, max element of delayline[]
# $s5 - Temporary loop counter - LOOP1 (m), LOOP2 (j)
# $s6 - {K/R - 1), shifted to adjust for int32 size
# $s7 - Main Master Loop Counter ('n')
# $v0 -
# $v1 -
# Assumption: Delay line array is initialized with 0

# Note: Scaling not implemented yet


    addiu	$sp,$sp,-32         # reserve stack space, needed if ANY storage is used
	sw		$s0,28($sp)         # save registers, required for optimized C code
	sw		$s1,24($sp)
	sw		$s2,20($sp)
	sw		$s3,16($sp)
	sw		$s4,12($sp)
	sw		$s5, 8($sp)
	sw		$s6, 4($sp)
	sw		$s7, 0($sp)

	# Load N(number of input samples), K(Filter length),scale and R(Filter rate)
	lwm32		$s0-$s3, 16+32($sp)	# load N, K, scale, rate
	addiu	    $s4,$s1,-1			# s4 is max index(offset for temp ptr) of delayline...
									# $s4 = K-1 i.e. delayline[max]
	divu        $t4, $s1, $s3       # $t4 = K/R, a constant value
	mflo        $t4
    sll         $t4, $t4, 2         # $t4 = K/R * size(int32)

	sll			$s1, $s1, 2			# $s1 = K*sizeof(int32)
	sll         $s3, $s3,2          # $s3 = R*sizeof(int32)
	sll         $s4,$s4,2			# (K-1) * sizeof(int32)
	
	li          $t5, 0x80000000     # Rounding value.........
	srav		$t5, $t5, $s2	    # Shift right by scale bits

	addiu		$s6, $t4,-4		    # $s6 = (K/R - 1) * size(int32)
	li          $s7, 0              # init main loop counter, n = 0

# Filter N number of input samples
LOOP4:
	move 		$t8, $a3			# $t8 = *delayline[0]
	move        $s5, $zero			# m = 0

# update delay line, shift delayline array by 1 sample
LOOP1:	
	addu        $t8, $s5            # *delayline[m]
	
	lw			$t2, 4($t8)			# delayline[m] = delayline[m+1]
	sw 			$t2, 0($t8)			
	
	addiu       $s5, $s5, 4         # m++
	bne         $s5,$s6, LOOP1		# loop if m < (K/R-1)
	ssnop
	#+++++++++++++++++++++++++++++++++++++++++++++++++++++
	#	END of LOOP1. Delay line has been updated.
	#+++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	addu		$t8, $a3, $s6		# t8 = *delayline [(K/R) - 1]
	lw			$t2, 0($a1)			# delay[(K/R) - 1] = x[n]
    srav        $t2, $t2, $s2       # scale input by binary shift
	sw			$t2, 0($t8)
	
    move		$t9, $t8			# store delayline[n] pointer
    move        $s5, $zero			# Loop Counter 'j'=0 for LOOP2

# process R number of input sets
LOOP3:
	mthi        $zero, $ac0         # clear acc for new dot product
    mtlo 		$t5, $ac0			# set to round value low accumulator bits
	move 		$t8, $t9			# Reinitialize delayline[n] pointer
	move 		$t3, $s5			# Initialize coef offset, j
    li          $t4, 0              # init loop count, k=0

#calculate dot product of K/R samples, this staggers Coef array, use same delayline for each
LOOP2:
	addu		$t7, $a2, $t3		# *Coefs[j+k*R], update of coeffs temporary pointer
	lw			$t0, 0($t7)			# $t0 = Coefs[j+k*R]
	lw 			$t1, 0($t8)			# Load delayline[(K/R)-1-k]
	dpaq_sa.l.w		$ac0,$t0,$t1
	addu 		$t3, $t3, $s3		# Coef Offset (j += 'R') (this is j+k*R)
    addu        $t4, $t4, $s3       # k += R
	bne         $t4, $s1, LOOP2     # loop if k < (K-1)
	addiu		$t8, $t8, -4		# Decrement delayline pointer
	
	#+++++++++++++++++++++++++++++++++++++++++++++++++++++
	#	END of LOOP2
	#+++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	mfhi		$t2, $ac0
	sw			$t2,0($a0)			# Store the result in memory
	addiu		$s5, 4              # j++
	bne         $s5, $s3, LOOP3     # loop if j < R
	addiu		$a0, $a0, 4			# Increment output pointer, next outdata

	#+++++++++++++++++++++++++++++++++++++++++++++++++++++
	#	END of LOOP3
	#+++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	addiu		$s7, $s7, 1			# n++ (LOOP4)
    bne         $s7, $s0, LOOP4     # loop while n < N (number of samples)
    addiu       $a1, $a1, 4         # increment input data pointer, next indata

	#+++++++++++++++++++++++++++++++++++++++++++++++++++++
	#	END of LOOP4
	#+++++++++++++++++++++++++++++++++++++++++++++++++++++

  lw      $s0,28($sp)        # restore registers
  lw      $s1,24($sp)
  lw      $s2,20($sp)
  lw      $s3,16($sp)
  lw      $s4,12($sp)
  lw      $s5, 8($sp)
  lw      $s6, 4($sp)
  lw      $s7, 0($sp)

	jr 			$ra                 # exit
	ssnop
	
	.end		DSP_FilterFIRInterp32
	