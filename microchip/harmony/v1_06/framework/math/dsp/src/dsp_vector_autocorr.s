/*******************************************************************************
Microchip DSP Library
  DSP_VectorAutocorr16:

      void  DSP_VectorAutocorr16(int16_t *outCorr, int16_t *inVector, int N, int K)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_autocorr.s

  Summary: 
    Computes the Autocorrelation of a Vector.

  Description: 
    Calculates the autocorrelation, with a lag of 1 to K, on the first N elements
    of inVector and returns the 16-bit scalar result in outCorr.  The autocorrelation is
    calculated from other statistical equations including mean and variance.
    While in some cases these equations exist inside the DSP library, the functions
    are exectued in a serial fashion within this code to provide enhanced performance.
    The unbiased function has the form -

    mean (M) = sum[0..N-1](x(n) / N)
    variance (V) = sum[0..N-1]((x(n) - M)^2) / (N-1)
    autocovariance (ACV)[k] = sum[0..(N-k)]((x(n) - M) * (x(n+k) - M) / (N-k))
    autocorrelation (AC)[k] = CV[k] / V
        where
      N is the number of vector elements, n is the index of those elements
      x(n) is a single element in the input vector
      M is the mean of the N elements of the vector
      k is the lag or series index

    The output of the function will return K elements, and the outCorr array
    should be sized to accept those 16-bit results.  The outputs correspond to
    k=1, k=2, .., k=K delay states.  The function returns a 16-bit value in rounded,
    saturated Q15 format.  Values exceeding the maximum or minimum will be
    saturated to the max or min value accordingly.

    Input values of the vector and output scalar value is Q15 fractional format.
    This format has data that ranges from -1 to 1, and has internal saturation
    limits of those same values.  Some care has been taken to reduce the impact
    of saturation by adding processing steps to effectively complete the processing
    in blocks.  However, in some extreme cases of data variance it is still possible
    to reach the saturation limits.

  Notes:
    The input vector elements number, N, must be at least 4 and a multiple of 4.
    This function is optimized with microMIPS and M14KCe ASE DSP instructions.
    This function is dependent on the LIBQ library, and the _LIBQ_Q16Div specifically.
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
/****************************************************************************
Change log

new content     C15637          2013-10-22

****************************************************************************/

	.text
	.set	noreorder
	.set	nomacro
    .set    micromips

	.global	DSP_VectorAutocorr16
    .global _LIBQ_Q16Div
    .ent	DSP_VectorAutocorr16

DSP_VectorAutocorr16:

# void  DSP_VectorAutocorr16(int16_t *outCorr, int16_t *inVector, int N, int K)
#
#  2268 -> 1740 cycles for 16 element vector, K (lag) = 4; M14Kce core
#  3414 -> 2918 cycles for 16 element vector, K (lag) = 8; M14Kce core
#   variable numbers with initial run (no cache) and with looping (cache)
#
# $a0 - output correlation vector pointer
# $a1 - input data vector pointer
# $a2 - Number of array elements (N), final address
# $a3 - Number of lag positions (K)

# $t0 - intermedite values
# $t1 - intermedite values
# $t2 - temp constant, intermedite values
# $t3 - intermedite values
# $t4 - 1 / N
# $t5 - temp constant, group of 4 sum
# $t6 - 1 / (N-1)
# $t7 - 
# $t8 - 
# $t9 - input N trap, function total accumulator

# $s0 - storage of N
# $s1 - inVector address pointer storage
# $s2 - k loop counter
# $s3 - output address
# $s4 -
# $s5 -
# $s6 -
# $s7 -

# $v0 - function 16-bit output
# $v1 - mean average
# Assumption: N must be a multiple of 4, and greater than 4

    addi    $t9, $a2, -4        # trap N if less than 4
    bltz    $t9, done
    andi	$a2, $a2, 0xFFFC	# make sure N is multiple of 4

	move    $t4, $a2            # number of elements(N)
    move    $s3, $a0            # store output vector address
    addiu   $a3, 1              # loop (k=1, k<=K, k++) set up K for loop

    li      $t5, 0x80000000     # unsigned 1.0
    divu    $t5, $t4            # div unsigned, 1.0/N

    addi    $t6, $a2, -1        # N - 1

    addiu	$sp, $sp, -32		# reserve stack space for s0-s3+
	swm     $s0-$s3, 0($sp)     # store registers
    sw      $ra, 16($sp)
                   
    extr_s.h    $t4, $ac0, 16   # extract quotient; scale factor 1/N Q15 format
    replv.ph    $t4, $t4        # duplicate 16-bit mean scale factor

    li      $t2, 0x80000000     # another 1.0, first one consumed in div
    divu    $t2, $t6            # div 1.0/(N-1)

    move    $s0, $a2            # store N
	sll     $a2, $a2, 1         # N * sizeof(int16)
    
	addu	$a2, $a1, $a2		# final address in indata
    move    $s1, $a1            # store vector starting address

    mflo    $t6, $ac0           # extract quotient 1/(N-1)
    shra_r.w  $t6, $t6, 16      # quotient 1/(N-1) in Q15

    MTHI    $zero, $ac0         # clear the accumulator
    MTLO    $zero, $ac0
    mthi    $zero, $ac1
    mtlo    $zero, $ac1

loop_mean:                      # calculate Mean average, M
	lwp         $t0,  0($a1)    # load indata[i | i+1]; indata[i+2 | i+3]
	
    DPAQ_S.W.PH $ac0,$t0,$t4    # accum += indata[i] * 1/N + indata[i+1] * 1/N
   	DPAQ_S.W.PH $ac0,$t1,$t4    # accum += indata[i+2] * 1/N + indata[i+3] * 1/N

    addiu       $a1, $a1, 8     # 4 * sizeof(int16), next i*4
	bne         $a2, $a1, loop_mean
	extr_s.h    $v1, $ac0, 16   # mean value (M), Q15; [delay slot]

# between loops
    move        $a1, $s1        # reset vector pointer for next loop
    mthi        $zero, $ac0     # clear accumulator
    mtlo        $zero, $ac0
    replv.ph    $v1, $v1        # copy (M) mean value, used in SIMD function

loop_var:                       # Calculate variance, Var
    lwp         $t0,  0($a1)    # load indata[i | i+1]; indata[i+2 | i+3]

    subq_s.ph   $t0, $t0, $v1   # x(i) - M | x(i+1) - M
    subq_s.ph   $t1, $t1, $v1   # x(i+2) - M | x(i+3) - M

    swp         $t0, 0($a1)     # new, store X-M values for use in ACV below
 
    dpaq_s.w.ph $ac0, $t0, $t0  # sum += [x(i) - M] ^2 + [x(i+1) - M] ^2
    dpaq_s.w.ph $ac0, $t1, $t1  # sum += [x(i+2) - M] ^2 + [x(i+3) - M] ^2

    addiu       $a1, $a1, 8     # next 4 inputs

    extr_s.h    $t5, $ac0, 16   # extract sum in Q15
    dpaq_sa.l.w  $ac1, $t5, $t6 # total += sum * 1/(N-1)

    mthi    $zero, $ac0         # clear (sum) accumulator
    mtlo    $zero, $ac0

    bne     $a2, $a1, loop_var  # loop N times
    extr_s.h    $t9, $ac1, 16   # extract variance (Var) in Q16, [delay slot]
    # working covariance, 32-bit 0x07D9C338

# outside of loop
    li          $s2, 1          # init the lag counter (k=1)

loop_autocorr:                  # outer loop, calculate autocorrelation, AC
    move        $a1, $s1        # reset vector pointer for next loop
    mthi        $zero, $ac1     # clear accumulator
    mtlo        $zero, $ac1

    sll         $t3, $s2, 1     # k * size(int16), offset pointer
    subu        $a2, $s0, $s2   # N-k
    sll         $a2, $a2, 1     # (N-k) * size(int16)
    addu        $a2, $s1, $a2   # end address of covar N-k

loop_covar:                     # inner loop, calculate autocovariance, ACV
    lh          $t0, 0($a1)     # get x(i)-M  
    lhx         $t1, $t3($a1)   # get x(i+k)-M

    muleq_s.w.phr  $t5, $t0, $t1   # NUM = (x(i)-M) * (x(i+k)-M)

    addiu       $a1, $a1, 2     # advance to next input

    shra_r.w    $t5, $t5, 16    # NUM (numerator) in Q15
    dpaq_sa.l.w $ac1, $t5, $t6  # ACV += [NUM] * 1/(N-k)

    bne         $a1, $a2, loop_covar
    extr_s.h    $t4, $ac1, 17   # extract autocovariance ACV[k] Q16, [delay slot]
    # autocovariance working multi-pass loop  0xFF444B12 in Q31,

# end of loop_covar, outer loop
    move        $a1, $t9        # set up ACV to call functino
    jal         _LIBQ_Q16Div    # call Div function  [a0 / a1], autocorrelation (AC) = ACV / Var
    move        $a0, $t4        # set up Var to call functino, [delay slot]

    sh          $v0, 0($s3)     # store Q15 value of AC in array
   
    addiu       $s2, 1          # inc autocorr loop counter (k++)
    subu        $t3, $s0, $s2   # N-k
    li          $t5, 0x80000000 # 'constant' 1.0

    divu        $t5, $t3        # 1.0 / (N-k)
    addiu       $s3, 2          # next output pointer address
    mflo        $t6, $ac0       # get [1 / (N-k)]

    bne         $s2, $a3, loop_autocorr # loop K times
    sra         $t6, 16         # 1/(N-k) in Q15, [delay slot]

# after loops
    lwm         $s0-$s3, 0($sp) # restore registers, before DONE since the function can exit on trap
    lw          $ra, 16($sp)
	addiu 	    $sp, $sp, 32

done:
   	jr      $ra                 # exit
	seh     $v0, $t4            # output = sqrt(total) in Q15, [delay slot]

	.end	DSP_VectorAutocorr16
