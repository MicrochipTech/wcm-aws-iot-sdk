/*******************************************************************************
Microchip DSP Library
  DSP_FilterFIRDecim32:

      void DSP_FilterFIRDecim32(int32 *outdata, int32 *indata, int32 *coeffs,
            int32 *delayline, int N, int K, int scale, int rate)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_fir_decim32.S

  Summary:
    Performs a decimating FIR filter on the input array.

  Description:
    Compute a FIR decimation filter on the input vector indata, and store the 
    results to the vector outdata.  The total number of output elements is set 
    by N, and therefore the outdata array must be at least N in length.  The 
    decimation ratio is given by rate.  The input is sampled every integer value 
    of rate, skipping every (rate-1) input samples.  The input array must therefore 
    be (rate*N) samples long.  The amount of filter taps is specified by K.  
    Coeffs specifies the coefficients array.  The delayline array holds delay 
    inputs for calculation, and must be initialized to zero prior to calling the 
    filter.  Both coeffs and delayline must be K in length.  Scale divides the
    input by a scaling factor by right shifting the number of bits (1/2^scale).
    All values of input, output, and coeffs are given in Q31 fractional data
    format.  The function will saturate if the output value exceeds the maximum
    or minmum value.

Notes:
    Coefs are loaded into the arry with corresponding to the least delay first (C0, C(-1), C(-2)).
    K must be greater than rate.
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
 *
 *  Created on: Mar 11, 2013
 *      Author: C15036
    updated 2013 July 19    C15637
 */
	.text
	.set		noreorder
	.set		nomacro
    .set        micromips

	.global		DSP_FilterFIRDecim32
	.ent		DSP_FilterFIRDecim32

DSP_FilterFIRDecim32:
#
#notes: if R>K, bad things will happen when the delay line address shifts
#
# void DSP_FilterFIRDecim32(int32 *outdata, int32 *indata, int32 *coeffs, int32 *delayline,
#				int N, int K, int scale, int rate)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 1382 cycles
# Cache Prefilled: 1028 cycles
#
# $a0 - outdata ptr, is updated every outer loop iteration
# $a1 - indata ptr, is updated every outer loop iteration
# $a2 - coeffs
# $a3 - delayline, is an array of 'K' elements
# 16($sp) - N, number of output samples
# 20($sp) - K, number of filter coefficients  (=M)
# 24($sp) - scale
# 28($sp) - rate, decimation rate R
# $t0 - Temporary data (coefficient value)
# $t1 - Temporary data (delay line value)
# $t2 - Temporary data place holder (indata in LOOP2 and outdata in LOOP3)
# $t3 - Temporary count variable 'i' in LOOP2
# $t4 - N * R, max position of indata[n]
# $t5 - 0x80000000, Rounding value loaded into accxlo in every outer loop iteration
# $t6 - 
# $t7 - temporary pointer for coeffs array
# $t8 - temporary pointer for delayline array, it initially
#		 points to delayline[0]
# $t9 - used in conjunction with t8, in Loop1, points to delayline[]
#		 and in Loop2, points to indata[]

# $s0 - N (Number of input samples)
# $s1 - K ( Number of filter taps ), later K*sizeof(int32)
# $s2 - scale
# $s3 - R (Decimation rate), later R*sizeof(int32)
# $s4 - maximum count of delayline array, (K-1)*(sizeofint32)
# $s5 - Temporary loop counter - LOOP1, LOOP2
# $s6 - {(K - R) * sizeof(int32)} in LOOP1, (R-1)*sizeof(int32) in LOOP2
# $s7 - Main Master Loop Counter ('n')
# $v0 -
# $v1 -
# Assumption: Delay line array is initialized with 0

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
	multu		$s0, $s3            # $t4 = N * R, a constant value the last value of the input array to be processed
    mflo        $t4
    sll         $t4, $t4, 2         # N*R * sizeof(int32)
	sll			$s1, $s1, 2			# $s1 = K*sizeof(int32)
	sll         $s3, $s3,2          # $s3 = R*sizeof(int32)
	sll         $s4,$s4,2			# K-1 * sizeof(int32)
	
	li			$t5, 0x80000000		# Rounding value.........
   	srav		$t5, $t5, $s2	    # Shift right by scale bits

	subu		$s6, $s1,$s3		# $s6 = {(K - R) * sizeof(int32)}
	move		$s7, $zero			# Initialize master loop counter  ### moved this init outside of the loop, killed us
	
OUTERLOOP:
	
	move 		$t8, $a3			# $t8 is temporary pointer pointing
                                            # to delayline[0] initially
	move        $s5, $zero			# Load '0' in $s5 (k counter)
	
    addu		$t9, $t8, $s3       # $t9 = *delayline[R]

LOOP1:	# loop K-R times; shift higher delayline array to lower positions
	
	lw			$t2, 0($t9)			# Making room in delayline array for next R samples
	sw 			$t2, 0($t8)			# this moves data from the higher address to the lower address

    addiu       $t9, $t9, 4         # next source address
	addiu       $s5, $s5, 4			# k++
																		
	bne         $s5,$s6, LOOP1      # Loop counter 'k' < M-R     ###(M=K in this case, number of taps)
	addiu       $t8, $t8, 4         # next destination address
	
#+++++++++++++++++++++++++++++++++++++++++++++++++++++
#	END of LOOP1. Delay line has been shifted K-R values.
#+++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	addu		$t8, $a3, $s3		# change pointer to *delayline[R]
    addiu       $t8, 4              # *delayline[R+1]... should this be K-R instead???
	
	move        $t9, $a1            # pointer to *indata[0]
	addu 		$t9, $s7			# increment to point to indata[n]  
		
	li          $s5, 0   			# Load '0' in $s5, Reuse the same pointer
	
LOOP2:	# loop a total of R times; put new input values into delayline[(K-R)++]
# move next R values from input, jumping K values, delay line should be continuous

	lw			$t2, 0($t9)			# $t2 = indata[i]
    srav        $t2, $t2, $s2       # shift right to divide input by scalar factor
	sw 			$t2, 0($t8)			# delayline[j] = indata[i]
	
	addiu 		$t9, $t9, 4			# Increment pointer to indata[] i++
	addiu		$s5, $s5, 4			# Increment loop count by one position
	
	bne			$s5, $s3, LOOP2		# Loop back
	addiu    	$t8, $t8, 4			# Increment pointer to delayline[] j++
	
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	END of LOOP2. Delay line has been updated with R new values.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	move		$s5, $zero			# Temporary loop counter 'k'
	mthi		$zero, $ac0			# Load '0' in acc-hi
	mtlo		$t5, $ac0		    # Accumulator rounding value[For SCALING]
	
    move 		$t7, $a2			# Pointer *Coefs[0]
	addiu		$t8, $t8, -4		# Adjust pointer to delayline to point to
                                        # delayline[max]
LOOP3: # calc the dot product, loop K times
#  Y = coef[0] x delayline[n] + coef[1] x delayline [n-1]...

	lw			$t0, 0($t7)			# $t0 = get value of Coefs[j]
	lw			$t1, 0($t8)			# $t1 = get value of delayline[(N-1)-k]
   	
	dpaq_sa.l.w		$ac0, $t0, $t1	# y0 = y0 + C[j] * delay[N-1-k]
	
	addiu 		$t7, $t7, 4			# Increment pointer to coefficients array, j++
	addiu		$s5, $s5, 4			# Increment loop counter, k++
	
	bne			$s5, $s1, LOOP3		# Branch if loop count != 'N'
	addiu		$t8, $t8, -4		# Decrement temp. pointer to delayline
	
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	END of LOOP3. New output Calculated
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	mfhi		$t2, $ac0
	sw			$t2, 0($a0)			# Updated output
	
	addu		$s7, $s7, $s3		# 'n' = 'n' + 'R'
	
	bne			$s7,$t4,OUTERLOOP	# Check if 'n' == N*R
	addiu 		$a0, 4				# Increment output pointer
	
###################################################
## OUTER LOOP ENDS
###################################################
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
	
	.end		DSP_FilterFIRDecim32
	
	
	
	

	


