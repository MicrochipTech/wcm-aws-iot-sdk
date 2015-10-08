/*******************************************************************************
Microchip DSP Library
  DSP_VectorRMS16:

      int16_t DSP_VectorRMS16(int16_t *inVector, int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_rms.S

  Summary: 
    Computes the root mean square (RMS) value of a vector.

  Description: 
    Computes the root mean square value of the first N values of inVector.  Both
    input and output are Q15 fractional values.  The function will saturate if
    maximum or minimum values are exceeded.

  Notes:
    The input vector elements number, N, must be at least 4 and a multiple of 4.
    This function is optimized with microMIPS and M14KCe ASE DSP instructions.
    This function is dependent on the LIBQ library, and uses the _LIBQ_Q16Sqrt
    external function call.
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

new content     C15637          2013-10-15

****************************************************************************/

	.text
	.set	noreorder
	.set	nomacro
    .set    micromips

	.global	DSP_VectorRMS16
    .global _LIBQ_Q16Sqrt
	.ent	DSP_VectorRMS16

DSP_VectorRMS16:

# int16_t DSP_VectorRMS16(int16_t *inVector, int N)
#
#  666 -> 360 cycles for 4 element vector, on PIC32MZ M14Kec core
#  676 -> 370 cycles for 8 element vector, on PIC32MZ M14Kec core
#   number differences are cache miss to hit numbers, running function multiple times allows cache setup
#
# $a0 - input data vector pointer
# $a1 - Number of array elements, final address
# $a2 -
# $a3 -

# $t0 - indata i, i+1
# $t1 - indata i+2, i+3
# $t2 - indata i, i+1
# $t3 - indata i+2, i+3
# $t4 - 1 / N
# $t5 - constant value (1.0), used early
# $t6 - 
# $t7 - 
# $t8 - 
# $t9 - input N trap, accumulator MSW

# $s0 - 
# $s1 -
# $s2 -
# $s3 -
# $s4 -
# $s5 -
# $s6 -
# $s7 -

# $v0 - function 16-bit output
# $v1 - mean square average
# Assumption: N must be a multiple of 4, and greater than 4

    addi    $t9, $a1, -4        # trap N if less than 4
    bltz    $t9, done

    andi	$a1, $a1, 0xFFFC	# make sure N is multiple of 4
	move    $t4, $a1            # number of elements(N)

    li      $t5, 0x80000000     # unsigned 1.0
    divu    $t5, $t4            # div unsigned, 1.0/N

    addiu   $sp, $sp, -4        # store return register
    sw      $ra, 0($sp)

    sll     $a1, $a1, 1         # N * sizeof(int16)
                
    extr_s.h    $t4, $ac0, 16   # quotient in $t4; scale factor 1/N Q15 format
    replv.ph    $t4, $t4        # duplicate 16-bit quotient for multiply
      
	addu	$a1, $a0, $a1		# final address in indata
    move    $v1, $zero          # init v1 (accumulator)

# calculate mean of the squares of the vector
loop_mean:
	lwp     $t0,  0($a0)        # load indata[i | i+1]
	move    $t2, $t0            # copy input values for in^2 function
    move    $t3, $t1
	
    mulq_rs.ph   $t0, $t0, $t2  # indata[i] ^2 | indata[i+1] ^2
    mulq_rs.ph   $t1, $t1, $t3  # indata[i+2] ^2 | indata[i+3] ^2

    mulq_rs.ph   $t0, $t0, $t4  # (indata[i]^2 / N) | (indata[i+1]^2 / N)
    mulq_rs.ph   $t1, $t1, $t4  # (indata[i+2]^2 / N) | (indata[i+3]^2 / N)

    addq_s.ph    $v1, $v1, $t0  # accum += (indata[i]^2 / N) | (indata[i+1]^2 / N)

    addiu	$a0, $a0, 8         # 4 * sizeof(int16), next i*2
	bne     $a1, $a0, loop_mean
	addq_s.ph    $v1, $v1, $t1  # accum += (indata[i+2]^2 / N) | (indata[i+3]^2 / N)

# outside of loop
    shra_r.w    $t9, $v1, 16    # get accum MSW
    addqh_r.ph  $v1, $v1, $t9   # add MSW to LSW, shift 1-bit for Q16 format
    jal         _LIBQ_Q16Sqrt   # perform square root function, Q16 frac format
    seh         $a0, $v1        # load inputs to sqrt (mean square value), [delay slot]

    lw          $ra, 0($sp)     # load return register
    addiu       $sp, $sp, 4

done:
   	jr      $ra                 # exit
	seh     $v0, $v0            # get LSW, Q15 output; [delay slot]

	.end	DSP_VectorRMS16
