/*******************************************************************************
Microchip DSP Library
  DSP_FilterFIR32:

      void DSP_FilterFIR32(int32_t *outdata, int32_t *indata, int32_t *coeffs2x,
            int32_t *delayline, int N, int K, int scale)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_filter_fir32.S

  Summary:
    Performs a finite infinite response (FIR) filter on a vector.

  Description:
    Performs an FIR filter on the vector indata, and stores the output in the
    vector outdata.  The number of samples processed in the array is given by N.
    The number of filter taps is given by K.  The values are scaled upon input
    by the binary scaling factor (right shift), scale.  The array of coefficients
    is contained in the array coeffs2x, where the values are in order b0, b1, b2...
    and repeated twice.  Lastly the delayline is an array of K values that are
    initialized to zero and represent previous values.  All values are in
    fractional Q31 data format.  The function will saturate results if minimum
    or maximum values are exceeded.
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
 */


	.text
	.set		noreorder
	.set		nomacro
    .set        micromips

	.global		DSP_FilterFIR32
	.ent		DSP_FilterFIR32

DSP_FilterFIR32:

# void DSP_FilterFIR32(int32_t *outdata, int32_t *indata, int32_t *coeffs2x, int32_t *delayline,
#				int N, int K, int scale)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 1320 cycles
# Cache Prefilled: 550 cycles
#
# $a0 - outdata ptr, is updated every outer loop iteration
# $a1 - indata ptr, is updated every outer loop iteration
# $a2 - coeffs2x
# $a3 - delayline, is an array of 'K+3' elements
# 16($sp) - N = 4*n >= 4
# 20($sp) - K = 4*k >= 4
# 24($sp) - scale
# $t0 -
# $t1 -
# $t2 -
# $t3 -
# $t4 - K,(INNER LOOP counter can be used to hold value of 'K' later)
# $t5 - 0x80000000, Rounding value loaded into accxlo in every outer loop iteration
# $t6 - holds value of coefficient inside inner loop, $t6 = 0($t7)
# $t7 - temporary pointer for coeffs array
# $t8 - temporary pointer for vector shift of delayline array, it initially
#		points to delayline[0]
# $t9 - temporary pointer for delayline,

# $s0 - N (Number of input samples)
# $s1 - K ( Number of filter taps )
# $s2 - scale
# $s3 - maximum count of delayline array, (K-1)*(sizeofint32)
# $s4 - a constant value ('4')
# $s5 -
# $s6 -
# $s7 -
# $v0 -
# $v1 -
# Assumption: Delay line array is initialized with 0

	# Load N(number of input samples), K(Filter length, size of coeffs array) and scale


   addiu	$sp,$sp,-32         # reserve stack space, needed if ANY storage is used
	sw		$s0,28($sp)         # save registers, required for optimized C code
	sw		$s1,24($sp)
	sw		$s2,20($sp)
	sw		$s3,16($sp)
	sw		$s4,12($sp)
#	sw		$s5, 8($sp)
#	sw		$s6, 4($sp)
#	sw		$s7, 0($sp)

	lwm32		$s0-$s2, 16+32($sp)	# load N, K, scale

	addiu	    $s3,$s1,-1			# s3 is max index(offset for temp ptr) of delayline...
									# $s3 = K-1 i.e. delayline[max]
	sll         $s3,$s3,2			# K-1 * sizeof(int32_t)

	li			$t5, 0x80000000		# Rounding value.........
	srav		$t5, $t5, $s2	    # Shift right by scale bits

	li			$s4, 0x4			# A constant value, 'Kmin'

loopN:

	mthi		$zero, $ac0		# zero high accumulator bits
	mthi		$zero, $ac1
	mthi		$zero, $ac2
	mthi		$zero, $ac3

	mtlo		$t5, $ac0		# accumulator rounding value
	mtlo		$t5, $ac1		#
	mtlo		$t5, $ac2
	mtlo		$t5, $ac3

# Reset temporary pointer for delayline that points to the last(latest) value of delayline array
	addu	$t9,$s3,$a3			# $t9 points to the last(latest)value
								# of delayline array, i.e. d7 if K = 8

	lwp		$t0, 0($a1)			# t0 = x[i],   t1 = x[i+1]
	lwp		$t2, 8($a1)			# t2 = x[i+2], t3 = x[i+3]

	srav    $t0, $t0, $s2		# Shift contents of input to the right by scale amount
	srav    $t1, $t1, $s2		# Shift contents of input to the right by scale amount
	srav    $t2, $t2, $s2		# Shift contents of input to the right by scale amount
	srav    $t3, $t3, $s2		# Shift contents of input to the right by scale amount

	swp		$t0, 0($t9)			# d[max]   = x[i] 	Delay line updated with new scaled down values
							    # d[max+1] = x[i+1]
	swp		$t2, 8($t9) 		# d[max+2] = x[i+2]
							    # d[max+3] = x[i+3]

	#LOAD loopK counter

	move	$t4, $s1		  # inner loop counter,a temporary copy of 'K', not 'K'*sizeof(int32_t)

	move	$t7, $a2		  # temporary pointer for coeffs array, which will be modified inside inner loop
							  # pointer is reset to base of coeffs array, each time program enters inner loop

loopK:

	# Load the coefficient into a temp register,
	lw		$t6, 0($t7)			# t6 = b[j]

	###################################################
	### COMPUTATION for inner loop
	### Load the delayline values in $t0-$t3
	###################################################

	lwp		$t0, 0($t9)			#t0 = d[k],t1 = d[k + 1]

	lwp		$t2, 8($t9) 		#t2 = d[k + 2],t3 = d[k + 3]


	dpaq_sa.l.w				$ac0,$t6,$t0		# y0 = y0 + bj*dk

	dpaq_sa.l.w				$ac1,$t6,$t1		# y1 = y1 + bj*dk+1

	dpaq_sa.l.w				$ac2,$t6,$t2		# y2 = y2 + bj*dk+2

	dpaq_sa.l.w				$ac3,$t6,$t3		# y3 = y3 + bj*dk+3

	addiu	$t9,$t9,-4							# Decrement temporary delayline pointer

	addiu	$t7,$t7,4							# Increment temp pointer to coeffs array  i.e. sizeof(int32_t)

	addiu	$t4,$t4,-1							# Decrement inner loop counter by 1

	bnez	$t4, loopK							# Repeat inner loop 'K' times
	ssnop
	# ####################################################
	# # INNER LOOP ENDS
	# ####################################################

	mfhi		$t0,  $ac0				# Save the outputs
	mfhi		$t1,  $ac1				# Save the outputs
	mfhi		$t2,  $ac2				# Save the outputs
	mfhi		$t3,  $ac3				# Save the outputs

	# Store the result in the output array
	swp			$t0,0($a0)		# y[i]   = $t0, y[i+1] = $t1
	swp			$t2,8($a0)		# y[i+2] = $t2, y[i+3] = $t3,


	 ####################################################
	 # Shifting the entire delayline array by four values.....
	 # $t8 is the temporary pointer
	 ####################################################

	move	$t4, $s1		# 'K' value to for vector copy of delayline array
	move	$t8, $a3		# $t8 = &delayline[0], working pointer for delayline array

loopDelaylineAdjust:
	beq	  $t4,$s4, lastShift	# if loopcount[$t4] == '4', shift only three entries in the delayline array
	ssnop

	lwp	$t0,  16($t8)		# t0 = delayline[j+4]
							# t1 = delayline[j+5]

	lwp	$t2,  24($t8)		# t2 = delayline[j+6]
							# t3 = delayline[j+7]

	swp	$t0,  0($t8)		# delayline[j]   = t0 = delayline[j+4]
							# delayline[j+1] = t1 = delayline[j+5]
	swp	$t2,  8($t8)		# delayline[j+2] = t2 = delayline[j+6]
							# delayline[j+3] = t3 = delayline[j+7]

	addiu $t8,$t8, 16		# Update/Increment pointer by 4 places, i.e. 4*sizeof(int32_t), i.e. by '16'

	addiu $t4, -4			# Decrement loop count 'K' value by '4'


	bnez  $t4,loopDelaylineAdjust	# Repeat for '(K-4)/4' times

	ssnop

lastShift:
	lwp	$t0,  16($t8)		# t0 = delayline[j+4]
							# t1 = delayline[j+5]
	lw  $t2,  24($t8)		# t2 = delayline[j+6]

	swp	$t0,  0($t8)		# delayline[j]   = t0 = delayline[j+4]
							# delayline[j+1] = t1 = delayline[j+5]
	sw  $t2,  8($t8)		# delayline[j+2] = t2 = delayline[j+6]
							# If K=8, delayline is updated for entries d0 to d6.
							# Entries d7,d8,d9,d10 will be updated when the next batch of 4 samples are input...

	#################################################
	 # Delayline array has been updated
	 # delayline[max] will have the the next, latest sample.
	 ################################################/

	## Update input data and output data pointers

	addiu		$a0, $a0, 16		# 4 * sizeof(int32_t)
	addiu		$a1, $a1, 16		# 4 * sizeof(int32_t)


	addiu		$s0, $s0, -4		# Decrement counter for outer loop, i.e. 'N'
									# As loop runs for 'N/4' times, decrement counter by '4'

	bnez	 	$s0, loopN			# IF $s0(outer loop count) !=0, repeat the outer loop

	ssnop
	###################################################
	## OUTER LOOP ENDS
	###################################################

  lw      $s0,28($sp)        # restore registers
  lw      $s1,24($sp)
  lw      $s2,20($sp)
  lw      $s3,16($sp)
  lw      $s4,12($sp)
#  lw      $s5, 8($sp)
#  lw      $s6, 4($sp)
#  lw      $s7, 0($sp)

    jr      $ra                # return
    addiu   $sp,$sp,32

	.end		DSP_FilterFIR32



