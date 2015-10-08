/*******************************************************************************
  _LIBQ_Q31_cos_Q2_29:

     _Q31  _LIBQ_Q31_cos_Q2_29 (_Q2_29 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q31_cos_q2_29.S

    _Q31  _LIBQ_Q31_cos_Q2_29 (_Q2_29 x);

  Summary:
    Calculates the value of sine(x).

  Description:
    Calculates the cosine(x), where x is of type _Q2_29 and
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

.global _LIBQ_Q31_cos_Q2_29
.ent    _LIBQ_Q31_cos_Q2_29

_LIBQ_Q31_cos_Q2_29:

# _Q31  _LIBQ_Q31_cos_Q2_29 (_Q2_29 x);
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
#     $ac0 - temp


# Initialize constants
    lui       $t2,0x6487             # PI Q2_29
    ori       $t2,0xed51
    subu      $t3,$zero,$t2          # -PI Q_29

# If angle < -PI then add 2*PI to angle
    slt       $t0,$a0,$t3
    beqzc     $t0,AngleGEpi
    addu      $a0,$a0,$t2            # angle += 2*PI
    b         CheckRange
    addu      $a0,$a0,$t2            # delay slot
AngleGEpi:

# If angle < 0 then reverse sign of angle
    slt       $t0,$a0,$zero
    beqzc     $t0,AngleGEnegPiDiv2
    b         CheckRange
    subu      $a0,$zero,$a0          # delay slot
AngleGEnegPiDiv2:

# If angle > PI then subtract angle from 2PI
    slt       $t0,$t2,$a0
    beqzc     $t0,CheckRange
    subu      $a0,$t2,$a0
    b         CheckRange
    addu      $a0,$a0,$t2            # delay slot
CheckRange:

# If angle >= (_Q2_29)0x6487c000 then return -1
    lui       $t0,0x6487
    ori       $t0,$t0,0xc000
    slt       $t0,$a0,$t0
    bnezc     $t0,CheckHi
    b         ReturnQ31cos
    lui       $v0,0x8000             # delay slot
CheckHi:
    li        $t0,0x4000
    slt       $t0,$t0,$a0
    bnezc     $t0,CalcCos
    lui       $v0,0x7fff
    b         ReturnQ31cos
    ori       $v0,0xffff             # delay slot
CalcCos:

# Initialize constants
    li        $t1,1                  # constant 1

# x^15
    move      $t4,$zero
    lui       $t5,0x0067
    ori       $t5,$t5,0xC908
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^14
    lui       $t0,0xf672
    ori       $t0,$t0,0x9df4
    addsc     $t5,$t5,$t0
    subu      $t0,$zero,$t1
    addwc     $t4,$t4,$t0
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^13
    lui       $t0,0x10CE
    ori       $t0,$t0,0x99F0
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$zero
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^12
    lui       $t0,0x3EAA
    ori       $t0,$t0,0x4CD8
    addsc     $t5,$t5,$t0
    li        $t0,0x4
    addwc     $t4,$t4,$t0
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^11
    lui       $t0,0xA1C9
    ori       $t0,$t0,0x0FE4
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$zero
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    lui       $t7,0xffff
    ori       $t7,$t7,0xfdae
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^10
    lui       $t0,0xf70f
    ori       $t0,$t0,0xeb6a
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    li        $t7,0x0001
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^9
    lui       $t0,0xD73C
    ori       $t0,$t0,0x9DDE
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    ori       $t7,$zero,0xD00A
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^8
    lui       $t0,0xF83F
    ori       $t0,$t0,0x3F1B
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    li        $t7,0x0001
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^7
    lui       $t0,0xA95D
    ori       $t0,$t0,0x284A
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    lui       $t7,0xffd2
    ori       $t7,$t7,0x7d26
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^6
    lui       $t0,0xd69e
    ori       $t0,$t0,0x857a
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^5
    lui       $t0,0x688E
    ori       $t0,$t0,0xB8CE
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$zero
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    lui       $t7,0x0555
    ori       $t7,$t7,0x5555
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^4
    lui       $t0,0x386E
    ori       $t0,$t0,0x228C
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^3
    lui       $t0,0x04E6
    ori       $t0,$t0,0xCAA9
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$zero
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    lui       $t7,0xbfff
    ori       $t7,$t7,0xffff
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^2
    lui       $t0,0xff8e
    ori       $t0,$t0,0x90ca
    addsc     $t5,$t5,$t0
    addwc     $t4,$t4,$t7
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    shilo     $ac0,-3
    mflo      $t5
    mfhi      $t4

# x^1
    srl       $t0,$t5,1
    mulq_s.w  $t0,$t0,$a0
    mult      $t4,$a0
    madd      $ac0,$t0,$t1
    extr_r.w  $v0,$ac0,29
    lui       $t0,0x8000
    subu      $v0,$v0,$t0

# Return
ReturnQ31cos:
    jr        $ra
    nop

.end    _LIBQ_Q31_cos_Q2_29

/*******************************************************************************
 End of File
*/
