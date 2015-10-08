/*******************************************************************************
Microchip DSP Library
  DSP_ComplexMult32:

      void DSP_ComplexMult32(int32c *indata1, int32c *indata2, int32c *Output)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_complex_mult.S

  Summary:
    Multiplies 2 complex numbers.

  Description:
    Multiplies 2 complex numbers, Indata1 and Indata2, and stores the complex
    result in Output. All numbers must be in int32c complex data structure. All
    data is Q31 fractional format. The function will saturate if maximum or
    minimum values are exceeded.
    Output(real) = (Input1.re * Input2.re) - (Input1.im * Input2.im);
    Output(img) = [(Input1.re * Input2.im) + (Input1.im * Input2.re)]i
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

	.global	DSP_ComplexMult32
	.ent	DSP_ComplexMult32

DSP_ComplexMult32:

# void DSP_ComplexMult32(int32c *indata1, int32c *indata2, int32c *Output)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 134 cycles
# Cache Prefilled: 26 cycles
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

    #(a+ib) (c+id) original data format
    lw	$t0,  0($a0)		# load complex.re (a)
	lw	$t1,  4($a0)		# load complex.im (b)
	lw	$t2,  0($a1)		# load complex.re (c)
	lw	$t3,  4($a1)		# load complex.im (d)

	mulq_s.w $t4,$t0,$t2    #compute ac-bd
	mulq_s.w $t5,$t1,$t3
	subq_s.w $t5,$t4,$t5

	mulq_s.w $t7,$t0,$t3    #compute (ad+bc)i
	mulq_s.w $t6,$t1,$t2
	addq_s.w $t6,$t7,$t6

	sw $t5, 0($a2)          # save real component
   	sw $t6, 4($a2)          # save img component
		
   jr	$ra
   ssnop

	.end	DSP_ComplexMult32
