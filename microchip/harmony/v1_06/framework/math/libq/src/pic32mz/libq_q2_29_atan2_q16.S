/*******************************************************************************
  _LIBQ_Q2_29_atan2_Q16:

    _Q2_29  _LIBQ_Q2_29_atan2_Q16 (_Q16 y, _Q16 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q2_29_atan2_q16.S

  Summary:
    Calculates the value of atan2(y, x).

  Description:
    Calculates the atan(y, x), where y and x are of type _Q16 and
    the resulting value is of type _Q2_29.  The output value
    will be in the range pi >= result >= -pi.
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

.global _LIBQ_Q2_29_atan2_Q16
.global __divdi3
.ent    _LIBQ_Q2_29_atan2_Q16

_LIBQ_Q2_29_atan2_Q16:

#  _Q2_29  _LIBQ_Q2_29_atan2_Q16 (_Q16 y, _Q16 x);
#
# Input parameters:
#     $a0 - _Q16 input value y
#     $a1 - _Q16 input value x
#
# Output parameter:
#     $v0 - _Q2_29 output value
#
# Internal register usage:
#     $t0 - temp
#     $t1 - angle ^ n
#     $t2 - angle squared
#     $t3 - result
#     $t4 - numerator
#     $t5 - demoninator
#     $ac0 - temp
#     $ac1 - result accumulation

#     $s0 - sign of y
#     $s1 - sign of x

#     $sp - Increased by 16 to store temporaries and return address
#     $sp+8  - ra
#     $sp+4  - s1
#     $sp+0  - s0

# Push stack
    addiu     $sp,$sp,-16
    swm       $s0,$s1,$ra,0($sp)

# If both x and y are 0, cast undefined value to 0
    bnezc     $a0,ValuesNotZero
    beqz      $a1,ReturnQ229
    move      $v0,$zero             # delay slot
ValuesNotZero:

# Calculate abs(y)
    slt       $s0,$a0,$zero         # store sign of y
    absq_s.w  $a0,$a0

# Calculate angle adjustment
    wrdsp     $zero,8               # clear saturation bit
    slt       $s1,$a1,$zero         # store sign of x
    bnezc     $s1,XltZero
    subq_s.w  $t4,$a1,$a0
    addq_s.w  $t5,$a1,$a0
    rddsp     $t0                   # read dsp control reg
    ext       $t0,$t0,20,1          # extract sat bit
    beqzc     $t0,DoDivide
    sra       $a0,$a0,1             # scale operands to prevent sat
    sra       $a1,$a1,1
    subq_s.w  $t4,$a1,$a0
    b         DoDivide
    addq_s.w  $t5,$a1,$a0           # delay slot

XltZero:
    addq_s.w  $t4,$a1,$a0
    subq_s.w  $t5,$a0,$a1
    rddsp     $t0                   # read dsp control reg
    ext       $t0,$t0,20,1          # extract sat bit
    beqzc     $t0,DoDivide
    sra       $a0,$a0,1             # scale operands to prevent sat
    sra       $a1,$a1,1
    addq_s.w  $t4,$a1,$a0
    subq_s.w  $t5,$a0,$a1

DoDivide:
    absq_s.w  $t0,$t4               # check divide for saturation
    absq_s.w  $t1,$t5
    beqzc     $t1,DivSatHigh
    slt       $t2,$t0,$t1
    bnezc     $t2,DivNotSat
    xor       $t0,$t4,$t5
    slt       $t0,$t0,$zero
    beqzc     $t0,DivSatHigh
    b         DivDone               # Saturate divide result low
    lui       $v0,0x8000
DivSatHigh:
    lui       $v0,0x7fff            # Saturate divide result high
    b         DivDone
    ori       $v0,0xffff
DivNotSat:
    move      $a1,$t4               # Divide numerator by denominator
    sra       $a1,1
    move      $a0,$t4
    sll       $a0,31
    move      $a2,$t5
    move      $a3,$t5
    jal       __divdi3
    sra       $a3,31

DivDone:
# Calculate angle squared
    mult      $v0,$v0
    extr.w    $t2,$ac0,31

# Calculate angle cubed
    mult      $t2,$v0
    extr.w    $t1,$ac0,31

# Result = 0.3333327629198127 * angle^3
#    rSum2_61  = int64_shift(int64_mul_i64_i32(r31_3, 0x2AAAA5E2), -1);
    lui       $t0,0x1555
    ori       $t0,0x52f1
    mult      $ac1,$t0,$t1

# Result -= 0.9999999971605453 * r
#    rSum2_61 -= int64_shift(int64_mul_i64_i32(r31,   0x7FFFFFFA), -1);
    lui       $t0,0x3fff
    ori       $t0,0xfffd
    msub      $ac1,$t0,$v0

# Calculate angle ^ 5
#    r31_3 = int64_hi(int64_shift(int64_mul_i64_i32(r31_3, r31_2), 1));
    mult      $t1,$t2
    extr.w    $t1,$ac0,31

# Result -= 0.1999807528112382 * r^5
#    rSum2_61 -= int64_shift(int64_mul_i64_i32(r31_3, 0x1998F825), -1);
    lui       $t0,0x0ccc
    ori       $t0,0x7c12
    msub      $ac1,$t0,$t1

# Calculate angle ^ 7
#     r31_3 = int64_hi(int64_shift(int64_mul_i64_i32(r31_3, r31_2), 1));
    mult      $t1,$t2
    extr.w    $t1,$ac0,31

# Result += 0.1426001608261613 * r^7
#    rSum2_61 += int64_shift(int64_mul_i64_i32(r31_3, 0x1240B8DA), -1);
    lui       $t0,0x0920
    ori       $t0,0x5C6D
    madd      $ac1,$t0,$t1

# Calculate angle ^ 9
#    r31_3 = int64_hi(int64_shift(int64_mul_i64_i32(r31_3, r31_2), 1));
    mult      $t1,$t2
    extr.w    $t1,$ac0,31

# Result -= 0.1093234150042210 * r^9
#    rSum2_61 -= int64_shift(int64_mul_i64_i32(r31_3, 0x0DFE4F46), -1);
    lui       $t0,0x06FF
    ori       $t0,0x27A3
    msub      $ac1,$t0,$t1

# Calculate angle ^ 11
#    r31_3 = int64_hi(int64_shift(int64_mul_i64_i32(r31_3, r31_2), 1));
    mult      $t1,$t2
    extr.w    $t1,$ac0,31

# Result += 0.0834972496611996 * r^11
#    rSum2_61 += int64_shift(int64_mul_i64_i32(r31_3, 0x0AB009B2), -1);
    lui       $t0,0x0558
    ori       $t0,0x04D9
    madd      $ac1,$t0,$t1

# Calculate angle ^ 13
#     r31_3 = int64_hi(int64_shift(int64_mul_i64_i32(r31_3, r31_2), 1));
    mult      $t1,$t2
    extr.w    $t1,$ac0,31

# Result -= 0.0570895558979657 * r^13
#    rSum2_61 -= int64_shift(int64_mul_i64_i32(r31_3, 0x074EB5E8), -1);
    lui       $t0,0x03A7
    ori       $t0,0x5AF4
    msub      $ac1,$t0,$t1

# Calcualte angle ^ 15
#    r31_3 = int64_hi(int64_shift(int64_mul_i64_i32(r31_3, r31_2), 1));
    mult      $t1,$t2
    extr.w    $t1,$ac0,31

# Result += 0.0303518647573583 * r^15
#    rSum2_61 += int64_shift(int64_mul_i64_i32(r31_3, 0x03E291E5), -1);
    lui       $t0,0x01F1
    ori       $t0,0x48F2
    madd      $ac1,$t0,$t1

# Calculate angle ^ 17
#    r31_3 = int64_hi(int64_shift(int64_mul_i64_i32(r31_3, r31_2), 1));
    mult      $t1,$t2
    extr.w    $t1,$ac0,31

# Result -= 0.0104876492412586 * r^17
#    rSum2_61 -= int64_shift(int64_mul_i64_i32(r31_3, 0x0157A8C7), -1);
    lui       $t0,0x00AB
    ori       $t0,0xD463
    mult      $t0,$t1
    msub      $ac1,$t0,$t1

# Calculate angle ^ 19
#    r31_3 = int64_hi(int64_shift(int64_mul_i64_i32(r31_3, r31_2), 1));
    mult      $t1,$t2
    extr.w    $t1,$ac0,31

# Result += 0.0017011700612784 * r^19
#    rSum2_61 += int64_shift(int64_mul_i64_i32(r31_3, 0x0037BE73), -1);
    lui       $t0,0x001B
    ori       $t0,0xDF39
    madd      $ac1,$t0,$t1
    mfhi      $v0,$ac1
    mflo      $v1,$ac1

# If (inX >= 0)
    bnezc     $s1,XisNeg
# Result += pi/4
    lui       $t0,0x442D
    ori       $t0,0x157D
    addsc     $v1,$t0,$v1
    lui       $t0,0x1921
    ori       $t0,0xfb54
    b         CheckSign
    addwc     $v0,$v0,$t0              # delay slot
XisNeg:
# Result += 3pi/4
    lui       $t0,0xCC87
    ori       $t0,0x1C6F
    addsc     $v1,$t0,$v1
    lui       $t0,0x4B65
    ori       $t0,0xF1FC
    addwc     $v0,$v0,$t0              # delay slot
CheckSign:

# Round result
    srl       $v1,$v1,31
    addu      $v0,$v0,$v1

# If y is negative, change sign of result
    beqzc     $s0,ReturnQ229
    subu      $v0,$zero,$v0

# Return
ReturnQ229:
    lwm       $s0,$s1,$ra,0($sp)
    jraddiusp 16                   # Return

.end    _LIBQ_Q2_29_atan2_Q16

/*******************************************************************************
 End of File
*/
