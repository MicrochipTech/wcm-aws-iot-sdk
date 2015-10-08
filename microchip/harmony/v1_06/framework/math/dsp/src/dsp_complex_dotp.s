/*******************************************************************************
Microchip DSP Library
  DSP_ComplexDotProd32:

      void DSP_ComplexDotProd32(int32c *indata1, int32c *indata2, int32c *Output)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_complex_dotp.S

  Summary:
    Calculates the dot product of 2 complex numbers.

  Description:
    Calculates the dot product of 2 complex numbers, Indata1 and Indata2, and
    stores the result in Output. All numbers must be in complex structuaral
    format that includes real and imaginary components, and numbers are in
    fractional Q31 format. The function will saturate the output if it exceeds
    maximum or minimum ratings. Formula for dot product is as follows:
    Output(real) = (Input1.re * Input2.re) + (Input1.im * Input2.im);
    Output(img) = [(Input1.re * Input2.im) - (Input1.im * Input2.re)]i
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

	.global	DSP_ComplexDotProd32
	.ent	DSP_ComplexDotProd32

DSP_ComplexDotProd32:

# void DSP_ComplexDotProd32(int32c *indata1, int32c *indata2, int32c *Output)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 148 cycles
# Cache Prefilled: 24 cycles
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
# $t4, $t5, $t6, $v0 - intermediate calculations
#
# input: (a + bi) dot (c + di)
# Real = (a * c) + (b * d)
# IM = (a * d) - (b * c)

    lw	$t0,  0($a0)		# load A.re (a)
	lw	$t1,  4($a0)		# load A.im (b)
	lw	$t2,  0($a1)		# load B.re (c)
	lw	$t3,  4($a1)		# load B.im (d)
  
   	mulq_s.w    $v0,$t2,$t0 # A.re*B.re
   	mulq_s.w    $v1,$t3,$t1 # A.im*B.im
    mulq_s.w    $t4,$t0,$t3 # A.re*B.im
    mulq_s.w    $t5,$t1,$t2 # A.im*B.re
  
    addq_s.w    $v0,$v0,$v1 # real term
    subq_s.w    $t6,$t4,$t5 # imaginary term

    sw  $v0, 0($a2)     # store to output
    sw  $t6, 4($a2)
		
   jr	$ra             # exit
   ssnop

	.end	DSP_ComplexDotProd32
