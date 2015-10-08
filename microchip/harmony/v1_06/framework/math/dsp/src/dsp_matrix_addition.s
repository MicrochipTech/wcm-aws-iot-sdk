/*******************************************************************************
Microchip DSP Library
  DSP_MatrixAdd32:

      void      DSP_MatrixAdd32(matrix32 * res , matrix32 * A, matrix32 * B)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_matrix_addition.S

  Summary:
    Calculates the sum of 2 matrices.

  Description:
    Calculates the sum of 2 input matrices A and B, and stores the result in a
    third matrix RES.  All matrices must be in the matrix32 format which includes
    row, column and data pointer. Function saturates the output values if maximum
    or minimum are exceeded. Assumes Q31 fractional matrix values.
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

	.global	DSP_MatrixAdd32
	.ent	DSP_MatrixAdd32

DSP_MatrixAdd32:

# void DSP_MatrixAdd32(matrix32 * res , matrix32 * A, matrix32 * B)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 370 cycles
# Cache Prefilled: 136 cycles
#
# Input parameters:
#     $a1 - A Matrix pointer
#       $a1+0 - row value
#       $a1+4 - col value
#       $a1+8 - data pointer
#     $a2 - B Matrix pointer
#       $a2+0 - row value
#       $a2+4 - col value
#       $a2+8 - data pointer

# Output parameter:
#     $a0 - res Matrix pointer
#       $a0+0 - row value
#       $a0+4 - col value
#       $a0+8 - data pointer
#
# Internal register usage:
#     $t0 - $t3 - temp input values
#     $t7 - total number of elements ROW*COL, loop counter
#     $t6 - 4 byte offset, constant
#     $t5 - temp value of element addition

	lw $t0,0($a1)  #load row size.  load word
	lw $t1,4($a1)  #load column size.  both A and B must have same COL & ROW
	lw $t2,8($a1)  #t2 is pointer to MatA data
   	lw $v0,8($a0)  #v0 is pointer to res output data
	lw $t3,8($a2)  #t3 is pointer to MatB data
	li $t4,0                # t4 = 0, constant
	li $t6,4                # t6 = 4, constant
    mthi $t4                # clear HI
    mtlo $t4                # clear LO
    multu $ac0,$t0,$t1      # ac0 = rows * columns
	addq_s.w $a1,$t4,$t2    #point to the elements of the matrix A
	addq_s.w $a2,$t4,$t3    #point to the elements of the matrix B
	addq_s.w $a0,$t4,$v0    #point to the elements of the matrix out
    mflo $t7                #number of elements in the matrix, move from LO
    addi $t7,$t7,1          #inc loop counter, set up loop variable

loop:
    lw $t2,0($a1)               #load a1 (val of MatA) into t2
    lw $t3,0($a2)               #load a2 (val of MatB) into t3
    addi $t7,$t7,-1             #t7-- loop counter
    ssnop                       #super scalar nop, delay for one cycle
    addq_s.w $t5,$t2,$t3        #sum of matA(val) + matB(val)
    addq_s.w  $a1,$a1,$t6       #next matA
    addq_s.w  $a2,$a2,$t6       #next matB
    sw $t5,0($a0)               #store t5 -> a0 (sum -> output matrix)
    addq_s.w  $a0,$a0,$t6       #next matRES
    bnez $t7, loop
    ssnop
		# 4 stall cycles - mispredicted branch

	jr	$ra                     #return
	ssnop

	.end	DSP_MatrixAdd32
