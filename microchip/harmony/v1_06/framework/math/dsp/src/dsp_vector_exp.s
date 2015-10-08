/*******************************************************************************
Microchip DSP Library
  DSP_VectorExp:

      void DSP_VectorExp(_Q16 *outVector, _Q16 *inVector, int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_exp.S

  Summary:  
    Computes the EXP (e^x) of the first N elements of inVector, and stores the
    result in outVector.

  Description:
    Computes the Exp value, e to the power of X, on the first N elements of
    inVector.  The output is stored to outVector.  Both vectors are _Q16 format,
    which is 32-bit data with 15 bits for the integer and 16 bits for the fractional
    portion.  If values exceed maximum or minimum they will saturate to the maximum
    or zero respectively.

  Notes:
    This function uses the Microchip PIC32MZ LIBQ library to function.  The user
    must include that library and header file into the design in order to operate
    this function.  For more information on the Exp function see the LIBQ documentation
    for _LIBQ_Q16Exp.

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

	.global	DSP_VectorExp
    .global _LIBQ_16Exp
	.ent	DSP_VectorExp

DSP_VectorExp:

# void DSP_VectorExp(_Q16 *outVector, _Q16 *inVector, int N)
#
# 1242 (variable) clock cycles for a 4 element 32bit data vector
# 2104 (variable) clock cycles for a 8 element 32bit data vector
#   requires more cycles for negative numbers
#
# Input Parameters
# $a0 - outVector pointer, data for scalar Exp function
# $a1 - inVector pointer
# $a2 - N, output vector end address
# $a3 - 

# $t0 - 
# $t1 - 
# $t2 - 
# $t3 - 
# $t4 -
# $t5 -
# $t6 -
# $t7 -
# $t8 -
# $t9 - 

# $s0 - address for OutVector
# $s1 - address for InVector
# $s2 -
# $s3 -
# $s4 -
# $s5 -
# $s6 -
# $s7 -

# $v0 - intermediate output value
# $v1 -
# Assumption: 


# Push stack
    addiu     $sp,$sp,-32
    swp       $s0, 0($sp)       # store registers
	sw        $ra,16($sp)
 
	sll     $a2, $a2, 2         # N * sizeof(int32)
    addu    $a2, $a2, $a0       # end addr of output vector
    move    $s0, $a0            # hold output address
    move    $s1, $a1            # hold input address

loop:
    jal     _LIBQ_Q16Exp        # call Exp function
    lw      $a0, 0($s1)         # load inVector[i] to call function

    sw      $v0, 0($s0)         # store outVector[i]

    addiu   $s0, 4              # next outVector

    bne 	$s0, $a2, loop      # loop until we are done with input vector
    addiu   $s1, 4              # next inVector

#done
    lwp       $s0, 0($sp)       # restore registers
    lw        $ra,16($sp)       # pop return address
    jraddiusp 32                # Return
    
	.end	DSP_VectorExp
