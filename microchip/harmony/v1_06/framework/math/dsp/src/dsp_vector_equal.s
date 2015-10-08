/*******************************************************************************
Microchip DSP Library
  DSP_VectorChkEqu32:

      int   DSP_VectorChkEqu32(int32* indata1, int32 *indata2, int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_equal.S

  Summary:
    Compares two input vectors, returns an integer 1 if equal, and 0 if not equal.

  Description:
    Compares the first N values of INDATA1 to the same elements of INDATA2.  The
    comparison requires that all numbers be in Q31 fractional data format.
    Returns the integer value 1 if all numbers are equal, and 0 if they are not
    equal. N must be >= 4 and a multiple of 4, or will be truncated to the nearest
    multiple of 4.
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

	.global	DSP_VectorChkEqu32
	.ent	DSP_VectorChkEqu32

DSP_VectorChkEqu32:

# int DSP_VectorChkEqu32(int32* indata1, int32 *indata2, int N)
#
# Input Parameters
# $a0 - indata
# $a1 - N = 4*n >= 4
#
# Output Parameters
#  #v0 - int return value, 1 if equal, 0 if not equal
#
# Internal register usage:
#   $t0 - $t3 - temp input values, indata2
#   $t4 - $t7 - temp input values, indata1
#
#   $t9 - temporary input trap

    addi    $t9, $a2, -4        # trap N if less than 4
    bltz    $t9, loop_end
	andi	$a2, $a2, 0xFFFC	# make sure N is multiple of 4
	sll     $a2, $a2, 2         # N * sizeof(int32)
	addu	$a2, $a1, $a2		# final address in indata

loop:
	lw	$t0,  0($a1)		# load indata2[i]
	lw	$t1,  4($a1)		# load indata2[i+1]
	lw	$t2,  8($a1)		# load indata2[i+2]
	lw	$t3, 12($a1)		# load indata2[i+3]

	lw	$t4,  0($a0)		# load indata1[i]
	lw	$t5,  4($a0)		# load indata1[i+1]
	lw	$t6,  8($a0)		# load indata1[i+2]
	lw	$t7, 12($a0)		# load indata1[i+3]

	bne     $t0,$t4, loop_unequal   #if element not equal, break
	ssnop
	bne     $t1,$t5, loop_unequal
	ssnop
	bne     $t2,$t6, loop_unequal
	ssnop
	bne     $t3,$t7, loop_unequal
	ssnop
	addiu	$a1, $a1, 16            # 4 * sizeof(int32), next 4 indata2
	addiu	$a0, $a0, 16            # 4 * sizeof(int32), next 4 indata1
	bne     $a1, $a2, loop
	ssnop
		# 4 stall cycles - mispredicted branch
    li $v0,1                        # set output to 1 if equal
    j loop_end
    ssnop

loop_unequal:
    li $v0,0                        # return output 0 if not equal

loop_end:
	jr	$ra                         # exit
	ssnop

	.end	DSP_VectorChkEqu32
