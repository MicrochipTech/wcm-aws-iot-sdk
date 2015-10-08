/*******************************************************************************
Microchip DSP Library
  DSP_VectorBexp32:

      int   DSP_VectorBexp32(int32_t *DataIn, int N)

  Company:
    Microchip Technology Inc

  File Name:
    dsp_vector_bexp32.S

  Summary:
    Computes the maximum binary exponent of a vector.

  Description:
    Calculates the maximum binary exponent on the first N elements of the input
    vector DataIn, and stores the integer result.  The returned value represents
    the potential binary scaling of the vector, and may be used with other functions
    that auto scale their output without saturation.  Inputs are given in Q31
    fractional data format.

  Notes:
    
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

new content     C15637          2013-8-15
modified        C15637          2013-10-8
    - removed references to $Sx registers, improve speed and stability

****************************************************************************/

	.text
	.set		noreorder
	.set		nomacro
    .set        micromips

	.global		DSP_VectorBexp32
	.ent		DSP_VectorBexp32

DSP_VectorBexp32:

# int   DSP_VectorBexp32(int32_t *DataIn, int N)
#
# evaluated on M14Kec core, uses 302 cycles @ 4 input vector
#
# $a0 - input data vector
# $a1 - Number of array elements, final address loop 1
# $a2 -
# $a3 -

# $t0 - Mask value, entire array or'ed together
# $t1 - max exponent value (const)
# $t2 - constant 1
# $t3 - Temporary data values
# $t4 - Temoporary ABS data values
# $t5 -
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

# $v0 - function 16-bit output, range {31..0}
# $v1 - Binary exponent value (Bexp)
# Assumption:

	sll		$a1, $a1, 2         # N * sizeof(int32)
	addu	$a1, $a0, $a1		# final address of DataIn
    li      $t0, 0              # initialize OR Mask value
    li      $t1, 32             # maximum exponent value
    li      $t2, 1              # constant 1

loop1:
	lw      $t3, 0($a0)         # load InData value
	addiu	$a0, $a0, 4         # next &InData

	absq_s.w   $t4, $t3         # ABS of value

	bne		$a0, $a1, loop1
    or      $t0, $t0, $t4       # rolling OR Mask of vector, 2 16-bit words

    li      $t6, 0              # initialize exponent

loop2:
    shllv_s.w  $v1, $t2, $t6    # binary exponent increment
    beq        $t1, $t6, done   # quit if exponent is max value
    subu       $t7, $v1, $t0    # Bexp - mask
    blez       $t7, loop2       # loop until Bexp > mask
    addq_s.w   $t6, $t6, $t2    # increment exponent

done:
   	jr         $ra              # return
    subu       $v0, $t1, $t6    # output = max - Bexp

	.end		DSP_VectorBexp32
