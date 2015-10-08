/*******************************************************************************
Microchip DSP Library
  DSP_MatrixMul32:

      void      DSP_MatrixMul32(matrix32 *DestMatrix, matrix32 * A, matrix32 *B)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_matrix_multiplication.S

  Summary:
    Calculates the multiplication of 2 matrices

  Description:
    Calculates the multiplication of matrix A x matrix B and stores the result
    in matrix DestMatrix.  All matrices must be defined in the matrix32 format
    which includes row, column and data pointer information.  All elements of the
    source matrices must be in fractional Q31 data format.  The columns value of
    matrix A must be equal to the rows of matrix B.  Matrix DestMatrix must
    contain the same rows as matrix A, and the same columns as matrix B.  Dot
    products within the resulting matrix will be saturated if the value exceeds
    the maximum or minimum value.
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

new content     C15637          2013-6-25

****************************************************************************/

	.text
	.set	noreorder
	.set	nomacro
	.global	DSP_MatrixMul32
	.ent	DSP_MatrixMul32

DSP_MatrixMul32:
# void DSP_MatrixMul32(matrix32 *DestMatrix, matrix32 * A, matrix32 *B)
# 
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 668 cycles
# Cache Prefilled: 340 cycles
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
#     $a0 - DestMatrix Matrix pointer
#       $a0+0 - row value
#       $a0+4 - col value
#       $a0+8 - data pointer
#
# Internal register usage:
#     $t0 - destMatrix data pointer
#     $t1 - matA data pointer
#     $t2 - matB data pointer
#     $t3 - matA rows
#     $t4 - matA col counter
#     $t5 - matB columns
#     $t6 - matA row counter
#     $t7 - matB column counter
#     $t8 - matA column counter
#

	lw $t0,8($a0)   #pointer to destMatrix data [load word]
	lw $t1,8($a1)   #pointer to A data
	lw $t2,8($a2)   #pointer to B data
	lw $t3,0($a1)   #matrix A row size (dwnRowA)
	lw $t4,4($a1)	#matrix A column size (dwnColA)
    lw $t7,4($a2)   #matB columns (downColB)
	or $a0,$t0,$0   #ptr to destMat  [logical or]
	or $a1,$t1,$0   #ptr to matA
	or $a2,$t2,$0   #ptr to matB
	or $t6,$t3,$0   #rowA counter

    or $t5,$t7,$0   #col B - total number along matB row
	sll $t5,$t5,2      #(colB) (*4)
    or $t8,$t4,$0   #t8 = t4, ptr to col A - total along matA row
	sll $t8,$t8,2      #(col A) *4
	sw $a2,0($sp)   #(matB) => SP+0 (outer loop)
	sw $a1,4($sp)   #(matA) => SP+4 (inner loop)
	sw $a2,8($sp)   #(matB again) => SP+8 (inner loop)
	sw $t3,12($sp)  #(rowA) => SP + 12
	sw $t4,16($sp)  #(colA) => SP + 16
    sw $t7,20($sp)  #(colB) => SP + 20
	sw $a1,24($sp)  #(matA again) => SP + 24 (outer loop)
	mtlo $0         #LO = 0 [move to LO]
	mthi $0         #HI = 0 [move to HI]

loop_col:
    loop_row:
     loop_element:

                # count along matA row, matB column, dot product accumulate
				lw $t0,($a1)        #t0 = a1 (mat A)
				lw $t1,($a2)        #t1 = a2 (mat B)
				addiu $a1,$a1,4     #a1 = a1 + 4  (Next column MatA)
				dpaq_sa.l.w $ac0,$t0,$t1    # [acc = acc + valA * valB]
				addi $t4,$t4,-1     #decrement colA counter (dwnColA --)
				bgtz $t4,loop_element       # if dwnColA > 0, loop
				addu  $a2,$a2,$t5   #a2 = (matB + colB*4) - next row matB

            #done with accumulate, store value, move to next matB column, repeat matA row
			mfhi $t0            #accum result HI -> t0
	        addi $t7,$t7,-1     #count down number of columns in mat B (dwnColB--)
			sw $t0,($a0)        #accum result => destMat
			addi $a0,$a0,4      #Next val destMat
			mthi $0             #HI = 0
			lw $a2,8($sp)       #a2 = [address MatB]
			addi $a2,$a2,4      #a2 = a2 + 4 [next MatB]-> increment next B column
			lw $a1,4($sp)       #a1 = [address MatA], start row over
           	sw $a2,8($sp)       #SP + 8 = a2 [store address MatB+4]
			lw $t4,16($sp)      #t4 = SP + 16 [colA], start colA counter over (dwnColA)
	        bgtz $t7,loop_row   # if t5 (dwnColB) > 0, loop
			mtlo $0             #LO = 0

    #done with matB columns, move to next matA row, repeat for all matB columns
	lw $a1,24($sp)      #a1 = SP + 24 = Mat A
	lw $a2,0($sp)       # a2 = SP + 0 = Mat B
	addi $t3,$t3,-1     #t3 = dwnRowA - 1
	addu $a1,$a1,$t8    #a1 = a1 + t8 => MatA pointer + colA * 4  (next rowA)
	sw $a1,4($sp)       #SP + 4 = a1
	sw $a1,24($sp)      #SP + 24 = a1
    lw $t4,16($sp)      #t4 = SP + 16 [colA] (reset colA pointer)
    lw $t7,20($sp)      #t5 = SP +20 [colB] (reset colB pointer)
	bgtz $t3,loop_col   #if dwnRowA > 0, branch to loop_col
	sw $a2,8($sp)       #SP + 8 = a2 [matB]

    jr $ra              #exit
    ssnop
	
.end	DSP_MatrixMul32
