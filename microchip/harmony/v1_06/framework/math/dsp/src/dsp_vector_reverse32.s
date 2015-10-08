/*******************************************************************************
Microchip DSP Library
  DSP_VectorCopyReverse32:

      void      DSP_VectorCopyReverse32(int32_t *outdata, int32_t *indata, int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_reverse32.S

  Summary:  
    Reverses the order of elements in one vector and copies them into another.

  Description:  
    Fills the first N values of an input vector Outdata with the the reverse elements
    from INDATA.  N must be a multiple of 4 and greater than 4 or will be
    truncated to the nearest multiple of 4.  The vectors are both Q31 fractional format.
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
/*
    created    2013 September 9    C15637
 */
	.text
	.set	noreorder
	.set	nomacro
    .set    micromips

	.global	DSP_VectorCopyReverse32
	.ent	DSP_VectorCopyReverse32

DSP_VectorCopyReverse32:

# void DSP_VectorCopyReverse32(int32_t *outdata, int32_t *indata, int N)
#
# 174 clock cycles for a 8 element 32bit data vector
#
# Input Parameters
# $a0 - outdata (vector pointer)
# $a1 - indata (vector pointer)
# $a2 - N = 4*n >= 4, output vector address
# $a3 - end address

# $t0 - input value A
# $t1 - input value B
# $t2 - input value C
# $t3 - input value D
# $t4 -
# $t5 -
# $t6 -
# $t7 -
# $t8 -
# $t9 - N criteria trap

# $s0 -
# $s1 -
# $s2 -
# $s3 -
# $s4 -
# $s5 -
# $s6 -
# $s7 -

# $v0 -
# $v1 -
# Assumption: N must be multiple of 4

    addi    $t9, $a2, -4        # trap N if less than 4
    bltz    $t9, done
	andi	$a2, $a2, 0xFFFC	# make sure N is multiple of 4
	sll     $a2, $a2, 2         # N * sizeof(int32)
    addu    $a3, $a2, $a1       # end addr of input vector
    addu	$a2, $a0, $a2		# end addr of output vector
    
loop:
	lwp	$t0,  0($a1)            # load indata[i]
	lwp	$t2,  8($a1)            # load indata[i+2]
   
	sw	$t0,  -4($a2)           # store outdata[n]
	sw	$t1,  -8($a2)           # store outdata[n-1]
	sw	$t2,  -12($a2)          # store outdata[n-2]
	sw	$t3, -16($a2)           # store outdata[n-3]
	
	addiu	$a1, $a1, 16		# increment input address counter
	bne 	$a1, $a3, loop      # loop until we are done with input vector
	addiu	$a2, $a2, -16		# decrement output address counter

done:
    jr	$ra                     #return
	ssnop

	.end	DSP_VectorCopyReverse32
