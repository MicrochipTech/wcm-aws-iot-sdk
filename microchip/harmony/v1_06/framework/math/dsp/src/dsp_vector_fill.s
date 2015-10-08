/*******************************************************************************
Microchip DSP Library
  DSP_VectorFill:

      void      DSP_VectorFill(int32 *indata, int32 data, int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_fill.S

  Summary:
    Fills an input vector with scalar data.

  Description:
    Fills the first N values of an input vector INDATA with the value DATA.  N
    must be a multiple of 4 and greater than 4 or will be truncated to the nearest
    multiple.  The vector result and the scalar value to fill are both Q31
    fractional format.
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

	.global	DSP_VectorFill
	.ent	DSP_VectorFill

DSP_VectorFill:

# void DSP_VectorFill(int32 *indata, int32 data, int N)
#
# Input Parameters
# $a0 - indata (vector pointer)
# $a1 - data (input scalar)
# $a2 - N = 4*n >= 4
#
# Internal register usage:
#
#   $t9 - temporary input trap

    addi    $t9, $a2, -4        # trap N if less than 4
    bltz    $t9, done
	andi	$a2, $a2, 0xFFFC	# make sure N is multiple of 4
	sll     $a2, $a2, 2         # N * sizeof(int32)
	addu	$a2, $a0, $a2		# final address in indata

loop:
	sw	$a1,  0($a0)            # store outdata[i]
	sw	$a1,  4($a0)            # store outdata[i+1]
	sw	$a1,  8($a0)            # store outdata[i+2]
	sw	$a1, 12($a0)            # store outdata[i+3]

	addiu	$a0, $a0, 16		# 4 * sizeof(int32)
	bne     $a0, $a2, loop      # while a0 < a2 loop
	ssnop
    	# 4 stall cycles - mispredicted branch
done:
	jr	$ra                     # return
	ssnop

	.end	DSP_VectorFill
