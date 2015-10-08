/*******************************************************************************
Microchip DSP Library
  DSP_MatrixScale32:

      void      DSP_MatrixScale32(int32 *data_buffer, int32 N, int32 num)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_matrix_scale.S

  Summary:
    Scales elements of an input matrix by a given value.

  Description:
    Multipies the first N elements of data_buffer by num, and assigns the result
    back to data_buffer.  Scaling function assumes Q31 fractional data format.
    Results will saturate if they exceed the maximum or minimum values.
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

	.text
	.set	noreorder
	.set	nomacro

	.global	DSP_MatrixScale32
	.ent	DSP_MatrixScale32

DSP_MatrixScale32:

# void DSP_MatrixScale32(int32 *data_buffer, int32 N, int32 num)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 366 cycles
# Cache Prefilled: 192 cycles
#
# Inputs
# $a0 - indata1; matrix or buffer array, fractional
#   $a0+0 - row
#   $a0+4 - column
#   $a0+8 - data pointer
# $a1 - N = 4*n >= 4
# $a2 - num = scale factor, fractional
#
# Internal register usage:
#     $t1 - temp sum 0
#     $t2 - temp sum 1
#     $t3 - temp sum 2
#     $t5 - temp sum 3
#     $t6 - constant zero
#   $t9 - temporary input trap

    addi    $t9, $a1, -4        # trap N if less than 4
    bltz    $t9, done
	andi	$a1, $a1, 0xFFFC	# make sure N is multiple of 4
	sll	    $a1, $a1, 2         # N * sizeof(int32)
	addu	$a1, $a0, $a1		# final address in indata
	addiu   $t4,$a2,0           #t4 is pointer to num
  	li      $t6,0

loop:
    lw $t1,0($a0)           # first element
    lw $t2,4($a0)
    lw $t3,8($a0)
    lw $t5,12($a0)          # fourth element

    MTHI $t6,$ac0           # clear accumulator
    MTLO $t6,$ac0
    dpaq_sa.l.w $ac0,$t1,$t4
    MFHI $t1,$ac0           # get result 1
    MTHI $t6,$ac0
    MTLO $t6,$ac0
    dpaq_sa.l.w $ac0,$t2,$t4
    MFHI $t2,$ac0           # get result 2
    MTHI $t6,$ac0
    MTLO $t6,$ac0
    dpaq_sa.l.w $ac0,$t3,$t4
    MFHI $t3,$ac0           # get result 3
    MTHI $t6,$ac0
    MTLO $t6,$ac0
    dpaq_sa.l.w $ac0,$t5,$t4
    MFHI $t5,$ac0           # get result 4

	sw $t1,0($a0)           # write data to array, 1-4
	sw $t2,4($a0)
	sw $t3,8($a0)
	sw $t5,12($a0)

	addiu	$a0, $a0, 16	# 4 * sizeof(int32), jump 4 elements
	bne	$a0, $a1, loop
	ssnop
		# 4 stall cycles - mispredicted branch
done:
	jr	$ra                 # return
	ssnop

	.end	DSP_MatrixScale32
