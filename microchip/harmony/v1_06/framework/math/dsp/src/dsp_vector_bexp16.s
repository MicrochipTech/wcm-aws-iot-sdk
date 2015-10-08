/*******************************************************************************
Microchip DSP Library
  DSP_VectorBexp16:

      int  DSP_VectorBexp16(int16_t *DataIn, int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_bexp16.S

  Summary:
    Computes the maximum binary exponent of a vector.

  Description:
    Calculates the maximum binary exponent on the first N elements of the input
    vector DataIn, and stores the integer result.  The returned value represents
    the potential binary scaling of the vector, and may be used with other functions
    that auto scale their output without saturation.  Inputs are given in Q15 
    fractional data format.

  Notes:
    N must be a multiple of 2 and greater or equal to 2.
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

new content     C15637          2013-8-14
reworked        C15637          2013-10-8
  - removed Sx registers for speed

****************************************************************************/

	.text
	.set		noreorder
	.set		nomacro
    .set        micromips

	.global		DSP_VectorBexp16
	.ent		DSP_VectorBexp16

DSP_VectorBexp16:

# int   DSP_VectorBexp16(int16_t *DataIn, int N)
#
# evaluated on M14Kce core, 286 cycles w/ 4 vector input
#
# $a0 - input data vector
# $a1 - N = 8*n >= 8, final address loop 1
# $a2 - 
# $a3 - 

# $t0 - Mask value, entire array or'ed together
# $t1 - max exponent value (const)
# $t2 - constant value (1)
# $t3 - Temporary data values
# $t4 - Temoporary ABS data values
# $t5 - Temporary OR values, half word
# $t6 - exponent value
# $t7 - Bexp > OR Mask comparator
# $t8 -
# $t9 - Minimum vector size trap

# $s0 - 
# $s1 - 
# $s2 - 
# $s3 - 
# $s4 - 
# $s5 - 
# $s6 - 
# $s7 -

# $v0 - function 16-bit output, range {15..0}
# $v1 - Binary exponent value (Bexp)
# Assumption: Vector is >2 and multiple of 2 in length

	addi    $t9, $a1, -2        # trap N if less than 2
    bltz    $t9, done
    li      $v0, 0              # set output to zero in case of trap
	andi	$a1, $a1, 0xFFFE	# make sure N is multiple of 2
	sll		$a1, $a1, 1         # N * sizeof(int16)
	addu	$a1, $a0, $a1		# final address of DataIn
    li      $t0, 0              # initialize OR Mask value
    li      $t1, 16             # maximum exponent value
    li      $t2, 1              # constant 1

loop1:
	lw      $t3, 0($a0)         # load 2 indata samples
	addiu	$a0, $a0, 4         # 2 * sizeof(int16)

	absq_s.ph   $t4, $t3        # ABS of both 16-bit numbers
	
	bne		$a0, $a1, loop1
    or      $t0, $t0, $t4       # rolling OR Mask of vector, 2 16-bit words

    srl     $t5, $t0, 16        # move upper byte to lower, leave zeros
    or      $t0, $t5, $t0       # Mask OR last 2 bytes, in lower byte
    seh     $t0, $t0            # keep only lower byte, final OR Mask
    li      $t6, 0              # initialize exponent

loop2:
    shllv_s.ph  $v1, $t2, $t6   # binary exponent increment
    beq         $t1, $t6, done  # quit if exponent is max value

    subu_s.ph   $t7, $v1, $t0   # Bexp - mask
    blez        $t7, loop2      # loop until Bexp > mask
    addu_s.ph   $t6, $t6, $t2   # increment exponent

done:
   	jr          $ra             # return
    subu_s.ph   $v0, $t1, $t6   # output = max - Bexp

	.end		DSP_VectorBexp16
