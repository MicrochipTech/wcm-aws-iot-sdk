/*******************************************************************************
  _LIBQ_Q15_cos_Q2_13:

     _Q15  _LIBQ_Q15_cos_Q2_13 (_Q2_13 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q15_cos_q2_13.S

  Summary:
    Calculates the value of cosine(x).

  Description:
    Calculates the cosine(x), where x is of type _Q2_13 and
    the resulting value is of type _Q15.
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

.global _LIBQ_Q15_cos_Q2_13
.ent    _LIBQ_Q15_cos_Q2_13

_LIBQ_Q15_cos_Q2_13:

# _Q15  _LIBQ_Q15_cos_Q2_13 (_Q2_13 x);
#
# Input parameters:
#     $a0 - _Q2_13 input value
#
# Output parameter:
#     $v0 - _Q15 output value
#
# Internal register usage:
#     $t0 - temp
#     $t1 - temp
#     $t2 - temp
#     $t3 - angle squared
#     $ac0 - temp
#     $ac1 - temp

# Convert to _Q16
    sll       $a0,$a0,3

# Add pi/2
    lui       $t3,1
    ori       $t3,0x9220
    addu      $a0,$a0,$t3

# If input > pi then input -= 2*pi
    lui       $t1,3
    ori       $t1,0x243F
    slt       $t0,$t1,$a0
    beqzc     $t0,InputLePi
    subu      $a0,$a0,$t1
    subu      $a0,$a0,$t1
InputLePi:

# If input < -pi then input += 2*pi
    subu      $t2,$zero,$t1
    slt       $t0,$a0,$t2
    beqzc     $t0,InputGeMinusPi
    addu      $a0,$a0,$t1
    addu      $a0,$a0,$t1
InputGeMinusPi:

# If input > pi/2 then input = pi - input
    slt       $t0,$t3,$a0
    beqzc     $t0,InputLePi2
    subu      $a0,$t1,$a0
InputLePi2:

# If input < -pi/2 then input = -pi - input
    subu      $t3,$zero,$t3
    slt       $t0,$a0,$t3
    beqzc     $t0,InputGeMinusPi2
    subu      $a0,$t2,$a0
InputGeMinusPi2:

# Calculate angle squared
    mult      $a0,$a0

# Initialize result
    mthi      $zero,$ac1            # result = input
    mtlo      $zero,$ac1
    extr_rs.w $t3,$ac0,16           # angle squared
    lui       $t0,0x8000
    msub      $ac1,$a0,$t0

# Result -= (x^3 / 6)
    mult      $a0,$t3
    lui       $t0,0x1555
    extr_rs.w $t2,$ac0,16
    ori       $t0,0x5555
    msub      $ac1,$t2,$t0

# Result += (x^5 / 120)
    mult      $t2,$t3
    lui       $t0,0x0111
    extr_rs.w $t2,$ac0,16
    ori       $t0,0x1111
    madd      $ac1,$t2,$t0

# Result -= (x^7 / 5040)
    mult      $t2,$t3
    lui       $t0,0x0006
    extr_rs.w $t2,$ac0,16
    ori       $t0,0x8068
    msub      $ac1,$t2,$t0

# Result += (x^9 / 362880)
    mult      $t2,$t3
    extr_rs.w $t2,$ac0,16
    li        $t0,0x171E
    madd      $ac1,$t2,$t0

# Result -= (x^11 / 39916800)
    mult      $t2,$t3
    extr_rs.w $t2,$ac0,16
    li        $t0,0x0036
    msub      $ac1,$t2,$t0

# Extract, round, and saturate to _Q15
    extr_rs.w $v0,$ac1,16
    ori       $t0,$zero,0x8000
    addq_s.w  $v0,$v0,$t0
    sra       $v0,$v0,16

# Return
ReturnQ15cos:
    jr        $ra
    nop

.end    _LIBQ_Q15_cos_Q2_13

/*******************************************************************************
 End of File
*/
