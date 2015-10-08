/*******************************************************************************
Microchip DSP Library
  DSP_MatrixEqual32:

      void      DSP_MatrixEqual32(matrix32 * res , matrix32 * A)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_matrix_equal.S

  Summary:
    Copies all elements of an input matrix to an output matrix.

  Description:
    Takes each element of matrix A and assigns it to the corresponding element of
    RES.  Both matrices must be in the matrix32 format that includes row, column
    and data pointer.  Both matrices must be equal in row and column.
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

	.global	DSP_MatrixEqual32
	.ent	DSP_MatrixEqual32

DSP_MatrixEqual32:
# void DSP_MatrixEqual32(matrix32 * res , matrix32 * A)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 264 cycles
# Cache Prefilled: 114 cycles
#
# Input parameters:
#     $a1 - A Matrix pointer
#       $a1+0 - row value
#       $a1+4 - col value
#       $a1+8 - data pointer
#
# Output parameter:
#     $a0 - res Matrix pointer
#       $a0+0 - row value
#       $a0+4 - col value
#       $a0+8 - data pointer
#
# Internal register usage:
#     $t0 - $t3 - temp input values
#     $t4 - total number of elements ROW*COL, loop counter
#     $t7 - 4 byte offset, constant
#     $t6 - temp value of element

	lw $t0,0($a1)   #load row size.
	lw $t1,4($a1)   #load column size.
	lw $t2,8($a1)   # values of input A
    lw $t3,8($a0)   # values of output RES

    #calculate the number of elements in the matrix
	mthi $0                 #clear the accumulator
    mtlo $0
    multu $ac0,$t0,$t1
    mflo $t4
    addi $t4,$t4,1          # t4 = ROW*COL, loop counter
	addq_s.w $a1,$0,$t2     #point to the elements of the matrix A
	addq_s.w $a0,$0,$t3     #point to the elements of the matrix B
	li $t7,4

loop:
    lw $t6,0($a1)
    addi $t4,$t4,-1         # loop counter (t4--)
    ssnop
    sw $t6,0($a0)
    addq_s.w  $a1,$a1,$t7   #next input A
    addq_s.w  $a0,$a0,$t7   #next output RES
    bnez $t4, loop
    ssnop
		# 4 stall cycles - mispredicted branch

	jr	$ra                 # return
	ssnop

	.end	DSP_MatrixEqual32
