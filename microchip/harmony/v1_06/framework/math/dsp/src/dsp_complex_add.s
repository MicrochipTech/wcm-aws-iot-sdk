/*******************************************************************************
Microchip DSP Library
  DSP_ComplexAdd32:

      void  DSP_ComplexAdd32(int32c *indata1, int32c *indata2, int32c *Output)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_complex_add.S

  Summary:
    Calculates the sum of 2 complex numbers.

  Description:
    Calculates the sum of 2 complex numbers, Indata1 and Indata2, and stores the
    complex result in Output. Complex numbers must be in the structural form that
    includes real and imaginary components. Function saturates the output values
    if maximum or minimum are exceeded. All values are in Q31 fractional data
    format.
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

	.global	DSP_ComplexAdd32
	.ent	DSP_ComplexAdd32

DSP_ComplexAdd32:

# void DSP_ComplexAdd32(int32c *indata1, int32c *indata2, int32c *Output)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 130 cycles
# Cache Prefilled: 16 cycles
#
# Input Parameters
# $a0 - indata1 complex number
# $a1 - indata2 complex number
# $a2 - Output complex number
#
# Internal register usage
# $t0 - indata1 real
# $t1 - indata1 imaginary
# $t2 - indata2 real
# $t3 - indata2 imaginary

    #load two complex number at a time
	lw	$t0,  0($a0)		# load indata1 complex.re
	lw	$t1,  4($a0)		# load indata1 complex.im
	lw	$t2,  0($a1)		# load indata2 complex.re
	lw	$t3,  4($a1)		# load indata2 complex.im

   	addq_s.w    $v0,$t2,$t0
   	addq_s.w    $v1,$t3,$t1

    sw      $v0, 0($a2)     # store to output complex.re
    sw      $v1, 4($a2)     # store to output complex.im
		
     jr	$ra
     ssnop

	.end	DSP_ComplexAdd32
