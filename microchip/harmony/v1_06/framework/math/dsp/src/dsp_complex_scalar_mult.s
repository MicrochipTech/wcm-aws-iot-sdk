/*******************************************************************************
Microchip DSP Library
  DSP_ComplexScalarMult32:

      void DSP_ComplexScalarMult32(int32c *indata, int32 Scalar, int32c *Output)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_complex_scalar_mult.S

  Summary:
    Multiplies a complex number and a scalar number.

  Description:
    Multiplies a complex number Indata by a scalar number, Scalar, and stores
    the result in Output. Indata and Output must be in int32c structure with
    real and imaginary components. All data must be in fractional Q31 formats.
    The function will saturate if maximum or minimum values are exceeded.
    Output(real) = (Input1.re * Scalar);
    Output(img) = [(Input1.im * Scalar)]i
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
	.global	DSP_ComplexScalarMult32
	.ent	DSP_ComplexScalarMult32

DSP_ComplexScalarMult32:

# void DSP_ComplexScalarMult32(int32c *indata, int32 Scalar, int32c *Output)
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 108 cycles
# Cache Prefilled: 16 cycles
#
# Input Parameters
# $a0 - indata1 complex number
# $a1 - constant Q31 (c)
# $a2 - Output complex number
#
# Internal register usage
# $t0 - indata1 real
# $t1 - indata1 imaginary
# $t4, $t5 - intermediate calculations

# (a+ib) * c
    lw	$t0,  0($a0)		# load complex.re (a)
	lw	$t1,  4($a0)		# load complex.im (b)
	
	mulq_s.w $t4,$t0,$a1    # compute a*C and b*C
	mulq_s.w $t5,$t1,$a1

	sw $t4, 0($a2)          # store real
   	sw $t5, 4($a2)          # store imaginary
		
   jr	$ra                 # exit
   ssnop
	.end	DSP_ComplexScalarMult32
