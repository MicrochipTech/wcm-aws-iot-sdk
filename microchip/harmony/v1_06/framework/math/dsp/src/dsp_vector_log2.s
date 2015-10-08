/*******************************************************************************
Microchip DSP Library
  DSP_VectorLog2:

      void DSP_VectorLog2(_Q5_10 *outVector, _Q16 *inVector, int N)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_vector_log2.S

  Summary:  
    Computes the Log2(x) of the first N elements of inVector, and stores the
    result in outVector.

  Description:  
    Computes the Log2 value, where log2(x) = ln(x) * log2(e), on the first N elements of
    inVector.  The output is stored to outVector.  Input vector is _Q16 format,
    which is 32-bit data with 15 bits for the integer and 16 bits for the fractional
    portion.  The output vector is reduced resolution Q5.10 format, which is a
    16-bit integer format with 10 bits representing the fractional resolution.
    If values exceed maximum or minimum they will saturate to the maximum
    or zero respectively.

  Notes:
    This function uses the Microchip PIC32MZ LIBQ library to function.  The user
    must include that library and header file into the design in order to operate
    this function.  For more information on the Log2 function see the LIBQ documentation
    for _LIBQ_Q5_10_log2_Q16.

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

	.global	DSP_VectorLog2
    .global _LIBQ_Q5_10_log2_Q16
	.ent	DSP_VectorLog2

DSP_VectorLog2:

# void DSP_VectorLog2(_Q5_10 *outVector, _Q16 *inVector, int N)
#
# 972 (variable) clock cycles for a 4 element 32bit data vector
# 1608 (variable) clock cycles for a 8 element 32bit data vector
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
# Assumption: Function does not save the stack prior to external call, but stores
#    critical loop values in $s0 and $s1.  Tested for interference with calling function.


# Push stack
    addiu     $sp,$sp,-32
    swp       $s0, 0($sp)       # save registers
	sw        $ra,16($sp)
 
	sll     $a2, $a2, 2         # N * sizeof(int32) [inVector]
    addu    $a2, $a2, $a1       # end addr of input vector
    move    $s0, $a0            # hold output address
    move    $s1, $a1            # hold input address

loop:
    jal     _LIBQ_Q5_10_log2_Q16       # call Log2 function
    lw      $a0, 0($s1)         # load inVector[i] to call function [delay slot]

    sh      $v0, 0($s0)         # store outVector[i]

    addiu   $s1, 4              # next inVector (32 bits)
    
    bne 	$s1, $a2, loop      # loop until we are done with input vector
    addiu   $s0, 2              # next outVector (16 bits)

#done
    lwp       $s0, 0($sp)       # restore registers
    lw        $ra,16($sp)       # pop stack
    jraddiusp 32                # Return
    
	.end	DSP_VectorLog2
