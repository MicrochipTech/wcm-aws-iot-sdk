/*******************************************************************************
Microchip DSP Library
  DSP_MatrixTranspose32:

      void      DSP_MatrixTranspose32(matrix32 *DestMatrix, matrix32 *SrcMatrix)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_matrix_transpose.S

  Summary:
    Creates the transpose of an input matrix.

  Description:
    Creates a transpose of the matrix SrcMatrix into the output matrix DestMatrix.
    Both matrices must be in the matrix32 format which includes row, column and
    data pointer.  The value of row, column in SrcMatrix must equal the value of
    column, row of the DestMatrix.
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

	.global	DSP_MatrixTranspose32
	.ent	DSP_MatrixTranspose32

DSP_MatrixTranspose32:
#void DSP_MatrixTranspose32(matrix32 *DestMatrix, matrix32 *SrcMatrix)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 280 cycles
# Cache Prefilled: 88 cycles
#
# Input parameters:
#     $a1 - SrcMatrix pointer
#       $a1+0 - row value
#       $a1+4 - col value
#       $a1+8 - data pointer
#
# Output parameter:
#     $a0 - DestMatrix pointer
#       $a0+0 - row value
#       $a0+4 - col value
#       $a0+8 - data pointer
#
# Internal register usage:
#     $t0 - $t3 - temp input values
#     $t5 - temp hold element value for transfer
#     $t6 - input matrix column counter, inner loop
#     $t9 - output matrix column counter, outer loop

	lw $t0,8($a0)   #pointer to destination  matrix address
	lw $t1,8($a1)   #pointer to source matrix address
	lw $t2,0($a1)	#row size
	lw $t3,4($a1)	#column size of source matrix
    mtlo $0
	mthi $0
 	multu $ac0,$t2,$t3  #calc total input size ROW*COL
    or $a0,$t0,$0	#extract source and destination address from the pointers
	or $t9,$t0,$0
	or $a1,$t1,$0
    or $t6,$t3,$0
	mflo $t4            #get total input elements
	sll $t2,$t2,2       #input (row*4)
	sll $t4,$t4,2       #output (col*4)
	addu $a2,$a1,$t4    #address of the last element in the matrix
    sw $t6,0($sp)

 loop_row:

    # inner loop, count along input matrix, and down rows of output matrix
	lw $t5,($a1)        # get input element value
   	addiu $a1,$a1,4     # next input element
	sw $t5,($a0)        # store to output element
	addi $t6,$t6,-1     # loop counter (columns)
	bgtz $t6,loop_row
    addu $a0,$a0,$t2          # next row, input matrix

    # outer loop, count along columns of output matrix, repeat inner loop
	addiu $t9,$t9,4     # advance column of output matrix
	or $a0,$t9,$0
   	bne $a1,$a2,loop_row
	lw $t6,0($sp)

	jr  $ra             #return
	ssnop

.end	DSP_MatrixTranspose32


