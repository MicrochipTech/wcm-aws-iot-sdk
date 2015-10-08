/*******************************************************************************
  _LIBQ_Q16_tan_Q2_29:

     _Q31  _LIBQ_Q16_tan_Q2_29 (_Q2_29 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q31_tan_q2_29.S

    _Q31  _LIBQ_Q16_tan_Q2_29 (_Q2_29 x);

  Summary:
    Calculates the value of tan(x).

  Description:
    Calculates the tan(x), where x is of type _Q2_29 and
    the resulting value is of type _Q31.
*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

.text
.set    micromips
.set    noreorder
.set    nomacro

.global _LIBQ_Q16_tan_Q2_29
.global _LIBQ_Q31_sin_Q2_29
.global _LIBQ_Q31_cos_Q2_29
.global _LIBQ_Q16Div
.ent    _LIBQ_Q16_tan_Q2_29

_LIBQ_Q16_tan_Q2_29:

# _Q16  _LIBQ_Q16_tan_Q2_29 (_Q2_29 x);
#
# Input parameters:
#     $a0 - _Q2_29 input value
#
# Output parameter:
#     $v0 - _Q16 output value
#
# Internal register usage:
#     $s0 - _Q2_29 input value
#     $s1 - sin value

#     $sp - Increased by 16 to store return address and temporaries
#     $sp+8  - ra
#     $sp+4  - $s1
#     $sp+0  - $s0

# Push stack
    addiu     $sp,$sp,-16
    swm       $s0,$s1,$ra,0($sp)     # Save return address, s0, s1

# Check for saturated boundary values
    lui       $t0,0x3243             # Check for ~pi/2
    ori       $t0,$t0,0xf6a8
    subu      $t1,$t0,$a0
    bnezc     $t1,CheckNeg
ReturnSat:
    lui       $v0,0x7fff             # Saturate hi
    b         ReturnQ31
    ori       $v0,$v0,0xffff
CheckNeg:
    lui       $t0,0xcdbc             # Check for ~-pi/2
    ori       $t0,$t0,0x0957
    subu      $t1,$t0,$a0
    beqzc     $t1,ReturnSat

    jal       _LIBQ_Q31_sin_Q2_29
    move      $s0,$a0                # Save input (delay slot)
    move      $s1,$v0                # Save sin result

    jal       _LIBQ_Q31_cos_Q2_29
    move      $a0,$s0                # Pass arg (delay slot)

# Check for divide by 0
    bnezc     $v0,DivideSinCos
    b         ReturnQ31
    lui       $v0,0x8000             # Saturate low (delay slot)
DivideSinCos:

# Divide sin/cos
    move      $a1,$v0                # Divide sin/cos
    jal       _LIBQ_Q16Div
    move      $a0,$s1                # delay slot

# Return
ReturnQ31:
    lwm       $s0,$s1,$ra,0($sp)   # Restore return address, s0, s1
    jraddiusp 16                   # Return

.end    _LIBQ_Q16_tan_Q2_29

/*******************************************************************************
 End of File
*/
