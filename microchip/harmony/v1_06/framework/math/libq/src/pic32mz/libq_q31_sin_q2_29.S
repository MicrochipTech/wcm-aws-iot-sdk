/*******************************************************************************
  _LIBQ_Q31_sin_Q2_29:

     _Q31  _LIBQ_Q31_sin_Q2_29 (_Q2_29 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q31_sin_q2_29.S

    _Q31  _LIBQ_Q31_sin_Q2_29 (_Q2_29 x);

  Summary:
    Calculates the value of sine(x).

  Description:
    Calculates the sine(x), where x is of type _Q2_29 and
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

.global _LIBQ_Q31_sin_Q2_29
.ent    _LIBQ_Q31_sin_Q2_29

_LIBQ_Q31_sin_Q2_29:

# _Q31  _LIBQ_Q31_sin_Q2_29 (_Q2_29 x);
#
# Input parameters:
#     $a0 - _Q2_29 input value
#
# Output parameter:
#     $v0 - _Q31 output value
#
# Internal register usage:
#     $t0 - temp
#     $t1 - temp and const 1
#     $t2 - PI Q2_29
#     $t3 - -PI Q2_29
#     $t4 - 64-bit accum hi
#     $t5 - 64-bit accum lo
#     $t6 - temp
#     $ac0 - temp


# Initialize constants
    lui       $t2,0x6487             # PI Q2_29
    ori       $t2,0xed51
    subu      $t3,$zero,$t2          # -PI Q_29

# If angle > PI then subtract 2*PI from angle
    slt       $t0,$t2,$a0
    beqzc     $t0,AngleLEpi
    subu      $a0,$a0,$t2            # angle -= 2*PI
    b         CheckPiDiv2
    subu      $a0,$a0,$t2            # delay slot
AngleLEpi:

# If angle < -PI then add 2*PI to angle
    slt       $t0,$a0,$t3
    beqzc     $t0,CheckPiDiv2
    addu      $a0,$a0,$t2            # angle += 2*PI
    addu      $a0,$a0,$t2
CheckPiDiv2:

# If angle > PI/2 then angle = PI - angle
    srl       $t0,$t2,1
    slt       $t0,$t0,$a0
    beqzc     $t0,AngleLEpi2
    b         AngleOK
    subu      $a0,$t2,$a0            # angle = PI - angle (delay slot)
AngleLEpi2:

# If angle < -PI/2 then angle = -PI - angle
    lui       $t0,0xcdbc
    ori       $t0,0x0958
    slt       $t0,$a0,$t0
    beqzc     $t0,AngleOK
    subu      $a0,$t3,$a0            # angle = -PI - angle
AngleOK:

# Initialize constants
    li        $t1,1                  # constant 1

# x^15
    addiu     $t4,$zero,-1
    lui       $t5,0xff96
    ori       $t5,$t5,0x0b8f
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^14
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^13
    lui       $t6,0x5846
    ori       $t6,$t6,0x2e33
    addsc     $t5,$t5,$t6
    addwc     $t4,$t4,$zero
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^12
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    lui       $t7,0xffff
    ori       $t7,$t7,0xffca
    shilo     $ac0,-3
    lui       $t6,0x3377
    mflo      $t5
    mfhi      $t4

# x^11
    ori       $t6,$t6,0xe419
    addsc     $t5,$t5,$t6
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^10
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    li        $t7,0x171D
    shilo     $ac0,-3
    lui       $t6,0xE3A3
    mflo      $t5
    mfhi      $t4

# x^9
    ori       $t6,$t6,0xE90E
    addsc     $t5,$t5,$t6
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^8
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    lui       $t7,0xfff9
    ori       $t7,$t7,0x7f97
    shilo     $ac0,-3
    mfhi      $t4
    mflo      $t5

# x^7
    lui       $t6,0xf980
    ori       $t6,$t6,0x05a9
    addsc     $t5,$t5,$t6
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^6
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    lui       $t7,0x0111
    ori       $t7,$t7,0x1111
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^5
    lui       $t6,0x1110
    ori       $t6,$t6,0xFFA7
    addsc     $t5,$t5,$t6
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^4
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    lui       $t7,0xeaaa
    ori       $t7,$t7,0xaaaa
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^3
    lui       $t6,0xaaaa
    ori       $t6,$t6,0x9ea8
    addsc     $t5,$t5,$t6
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^2
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mfhi      $t4
    mflo      $t5

# x^1
    lui       $t6,0x8000
    subu      $t4,$t4,$t6
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    extr_rs.w $v0,$ac0,29

# Return
ReturnQ31sin:
    jr        $ra
    nop

.end    _LIBQ_Q31_sin_Q2_29

/*******************************************************************************
 End of File
*/
