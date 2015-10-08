/*******************************************************************************
Microchip DSP Library
  DSP_VectorShift:

      void      DSP_VectorShift(int32 *outdata, int32 *indata, int N, int shift)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_shift.S

  Summary:
    Shifts the data index of an input data vector.

  Description:
    Shifts N data elements of INDATA to OUTDATA, with an index change of SHIFT.
    The amount of data shifted includes zero padding for the first (shift)
    elements if SHIFT is positive.  The vector size of INDATA and OUTDATA need
    not be the same, however N must not exceeded either array size.
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

new content     C15637          2013-6-27

****************************************************************************/

	.text
	.set	noreorder
	.set	nomacro

	.global	DSP_VectorShift
	.ent	DSP_VectorShift

DSP_VectorShift:

# void DSP_VectorShift(int32 *outdata, int32 *indata, int N, int shift)
#
# Input Parameters
# $a0 - outdata
# $a1 - indata
# $a2 - N number of elements processed
#
# Output Parameters
#
#
# Internal register usage
#
#

   	sll     $a2, $a2, 2         # N * sizeof(int32), number of shifts (words)
    addu    $t6, $a2, $a0       # final output address, positive shift
    addu	$a2, $a1, $a2		# final input address, negative shift
    sll     $a3, $a3, 2         # shift in whole words
    beqz    $a3, loop           # just copy buffer if zero shifting

    addu    $t5, $a3, $a0       # intial offset address, used for padding zeros
    blez    $a3, neg_offset     # if SHIFT negative, skip padding
    li      $t4,0               # padding constant, zero
    
pos_loop:
    beq $a0, $t6, done          # if we have loaded the output with N samples, finish
    ssnop
    sw      $t4, 0($a0)             # pad positive shift with zero
    addiu   $a0, $a0, 4             # next outdata[i]
    bne     $a0, $t5, pos_loop      # branch when index is = shift
    ssnop                           # same as NOP
    j       loop                    # skip negative shift
    ssnop

neg_offset:
    li      $t7, -1
    mult    $ac0,$a3,$t7            # get abs(shift)
    mflo    $t9
    addu    $a1, $a1, $t9           # change index, input[i+abs(shift)]
    
loop:
    beq $a0, $t6, done      # if we have loaded the output with N samples, finish
	lw	$t0,  0($a1)		# load indata[i]
    
	sw	$t0,  0($a0)		# store outdata[i+shift]

	addiu	$a1, $a1, 4		# 4 * sizeof(int32), next indata[i]
	addiu	$a0, $a0, 4		# 4 * sizeof(int32), next outdata[i]

	bne     $a1, $a2, loop  # if we have reached the the maximum of the input array, finish
	ssnop
    	
done:
	jr	$ra                 # exit
	ssnop

	.end	DSP_VectorShift
