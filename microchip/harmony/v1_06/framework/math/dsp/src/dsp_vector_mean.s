/*******************************************************************************
Microchip DSP Library
  DSP_VectorMean32:

      int32         DSP_VectorMean32(int32* indata1,int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_mean.S

  Summary:
    Calculates the mean average of an input vector.

  Description:
    Calculates the mean average of the first N elements of the vector INDATA1.
    The values of INDATA 1 are in Q31 fractional format.  The value N must be
    greater than 4 and a multiple of 4, or it will be truncated to the nearest
    multiple.
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

	.global	DSP_VectorMean32
	.ent	DSP_VectorMean32

DSP_VectorMean32:

# int32 DSP_VectorMean32(int32* indata1,int N)
#
# Input Parameters
# $a0 - indata1
# $a1 - N = 4*n >= 4
#
# Output Parameters
# $v0 - int32 return of Mean average
#
# Temporary variable usage
#  t4 - scale value of -1/N
#  t6 - zero const
#   $t9 - temporary input trap

    addi    $t9, $a1, -4        # trap N if less than 4
    bltz    $t9, done
    li      $v0, 0              # initialize output
	andi	$a1, $a1, 0xFFFC	# make sure N is multiple of 4
	li      $t4,0
	add     $t4,$a1             # number of elements(N)
	sll     $a1, $a1, 2         # N * sizeof(int32)
	addu	$a1, $a0, $a1		# final address in indata
    li      $t5, 0x80000000     # unsigned 1.0
    divu    $t5,$t4             # div unsigned, 1.0/N
    MFLO    $t4                 # quotient in $t4  # scale factor 1/N
    li      $t6,0               # clear the accumulator
    MTHI    $t6
    MTLO    $t6

loop:
	lw	$t0,  0($a0)		# load indata[i]
	lw	$t1,  4($a0)		# load indata[i+1]
	lw	$t2,  8($a0)		# load indata[i+2]
	lw	$t3, 12($a0)		# load indata[i+3]

    DPAQ_SA.L.W $ac0,$t0,$t4    # accum, indata[i] * 1/N
   	DPAQ_SA.L.W $ac0,$t1,$t4
   	DPAQ_SA.L.W $ac0,$t2,$t4
    DPAQ_SA.L.W $ac0,$t3,$t4

    addiu	$a0, $a0, 16		# 4 * sizeof(int32), next i*4
	bne	$a1, $a0, loop
	ssnop
		
	MFHI $v0                # output int32
done:
   	jr	$ra                 # exit
	ssnop

	.end	DSP_VectorMean32
