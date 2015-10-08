/*******************************************************************************
 Microchip DSP library
  DSP_MatrixInit32:

      void      DSP_MatrixInit32(int32* indata1,int N,int num)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_matrix_init.S

  Summary:
    Initializes an input matrix or vector array to a given value

  Description:
    Store the value num into the first N postions of the matrix indata1.
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

	.global	DSP_MatrixInit32
	.ent	DSP_MatrixInit32

DSP_MatrixInit32:

# void DSP_MatrixInit32(int32* indata1,int N,int num)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 152 cycles
# Cache Prefilled: 62 cycles
#
# $a0 - indata1
# $a1 - N = 4*n >= 4
# $a2 - num = value to be stored into indata1
#
# Internal register usage:
#     $t4 - value of num
#
#   $t9 - temporary input trap

    addi    $t9, $a1, -4        # trap N if less than 4
    bltz    $t9, done
	andi	$a1, $a1, 0xFFFC	# make sure N is multiple of 4
	sll	    $a1, $a1, 2         # N * sizeof(int32)
	addu	$a1, $a0, $a1		# final address in indata
	addiu   $t4,$a2,0

loop:

	sw $t4,0($a0)           #initialize the data
	sw $t4,4($a0)
	sw $t4,8($a0)
	sw $t4,12($a0)

	addiu	$a0, $a0, 16		# 4 * sizeof(int32)
	bne	$a0, $a1, loop
	ssnop
		# 4 stall cycles - mispredicted branch
done:
	jr	$ra
	ssnop

	.end	DSP_MatrixInit32
