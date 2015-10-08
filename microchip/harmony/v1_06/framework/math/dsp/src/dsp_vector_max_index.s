/*******************************************************************************
Microchip DSP Library
  DSP_VectorMax32:

      int32         DSP_VectorMax32(int32 *indata, int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_max_index.S

  Summary:
    Returns the index of the maximum value of a vector.

  Description:
    Returns the index of the highest value of the first N elements of the vector
    INDATA.  The comparison requires that all numbers be in Q31 fractional data
    format.  N must be >= 4 and a multiple of 4, or will be truncated to the
    nearest multiple of 4.
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

new content     C15637          2013-7-2

****************************************************************************/
	.text
	.set	noreorder
	.set	nomacro

	.global	DSP_VectorMaxIndex32
	.ent	DSP_VectorMaxIndex32

DSP_VectorMaxIndex32:

# int DSP_VectorMaxIndex32(int32 *indata, int N)
#
# Input Parameters
# $a0 - indata
# $a1 - N = 4*n >= 4
# $a2 - return value of index
#
# Output Parameters
#  #v0 - int return value of the index (of the maximum element)
#
# Internal register usage:
#   $t0 - $t3 - temp input values
#   $t4 - flag, low if input is > max
#   $t5 - temp value, position of index
#   $t6 - loop index counter
#   $t7 - intermediate high value
#   $t9 - temporary input trap

    addi    $t9, $a1, -4        # trap N if less than 4
    bltz    $t9, done
    li      $v0, 0              # initialize output
	andi	$a1, $a1, 0xFFFC	# make sure N is multiple of 4
	sll     $a1, $a1, 2         # N * sizeof(int32)
	addu	$a1, $a0, $a1		# final address in indata

    li      $t7,0x80000000      # return value = -1
    li      $t6, 0              # starting index counter

loop:
	lw	$t0,  0($a0)		# load indata[i]
	lw	$t1,  4($a0)		# load indata[i+1]
	lw	$t2,  8($a0)		# load indata[i+2]
	lw	$t3, 12($a0)		# load indata[i+3]

    slt $t4, $t0, $t7       # t4=0 if t0 >= t7
    addiu  $t5, $t6, 0      # calc index (loop + offset)
    movz $v0, $t5, $t4      # if t4 = 0, store index
    movz $t7, $t0, $t4      # hold intermediate high value

    slt $t4, $t1, $t7       # t4=1 if t1<t7
    addiu  $t5, $t6, 1
    movz $v0, $t5, $t4
    movz $t7, $t1, $t4

    slt $t4, $t2, $t7       # t4=1 if t2<t7
    addiu  $t5, $t6, 2
    movz $v0, $t5, $t4
    movz $t7, $t2, $t4

    slt $t4, $t3, $t7      # t4=1 if t3<t7
    addiu  $t5, $t6, 3
    movz $v0, $t5, $t4
    movz $t7, $t3, $t4

    addiu	$a0, $a0, 16	# 4 * sizeof(int32), next 4 elements
    addiu   $t6, $t6, 4     # relative index counter
   	bne	$a0, $a1, loop
    ssnop
done:
	jr	$ra                 # exit
	ssnop

	.end	DSP_VectorMaxIndex32
