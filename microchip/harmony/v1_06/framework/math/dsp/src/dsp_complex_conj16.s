/*******************************************************************************
Microchip DSP Library
  DSP_ComplexConj16:

      void  DSP_ComplexConj16(int16c *indata, int16c *Output)

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_complex_conj16.s

  Summary:
    Calculates the complex conjugate of a complex number.

  Description:
    Calculates the complex conjugate of Indata, and stores the result in Outdata.
    Both numbers must be in the complex number data structure which includes real
    and imaginary components. Values are in Q15 fractional data format. The
    function will saturate the output if maximum or minimum values are exceeded.
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

	.global	DSP_ComplexConj16
	.ent	DSP_ComplexConj16

DSP_ComplexConj16:

# void  DSP_ComplexConj16(int16c *indata, int16c *Output
#
# DSP Benchmarks on a PIC32MZ2048ECH144 @200MHz
# Cache Unfilled: 120 cycles
# Cache Prefilled: 14 cycles
#
# Input Parameters
# $a0 - indata1 complex number
# $a1 - Output complex number
#
# Internal register usage
# $t0 - indata1 real
# $t1 - indata1 imaginary
# $t2 - constant zero
# $v0 - negated imaginary coef


    lh	$t0,  0($a0)            # load complex.im
	lh	$t1,  2($a0)            # load complex.im

	li $t2,0                    # constant zero
   	subq_s.ph    $v0,$t2,$t1     # reverse sign of imaginary data

   sh $v0, 2($a1)
   sh $t0, 0($a1)
		
   jr	$ra                     # exit
   ssnop

	.end	DSP_ComplexConj16
