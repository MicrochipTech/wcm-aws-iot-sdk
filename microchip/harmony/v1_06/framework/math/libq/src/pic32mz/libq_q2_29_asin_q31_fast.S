/*******************************************************************************
  _LIBQ_Q2_29_asin_Q31_Fast:

    _Q2_29  _LIBQ_Q2_29_asin_Q31_Fast (_Q31 x);

  Summary:
    Calculates the value of asin(x).  This function executes faster than
    the _LIBQ_Q2_29_asin_Q31 function, but is less precise.

  Description:
    Calculates the asin(x), where x is of type _Q31 and
    the resulting value is of type _Q2_29.  The output value
    will be in radians the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    x     - The _Q31 input value from which to calculate asin(x).

  Returns:
    _LIBQ_Q2_29_asin_Q31_Fast returns the _Q2_29 fixed point result
    from the calculation asin(x).
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
.set	noreorder
.set	nomacro

.global _LIBQ_Q2_29_asin_Q31_Fast
.ent    _LIBQ_Q2_29_asin_Q31_Fast

_LIBQ_Q2_29_asin_Q31_Fast:

#  _Q2_29  _LIBQ_Q2_29_asin_Q31_Fast (_Q31 x)
#
# Provide a fast calculation of asin
#
# Input parameters:
#     $a0 - _Q31 input value
#
# Output parameter:
#     $v0 - _Q2_29 output value
#
# Internal register usage:
#     $a1 - ABS(x)
#     $a2 - boolean true if NOT using sqrt
#     $a3 - series expansion arg

#     $t0 - temp
#     $t1 - angle ^ n
#     $t2 - sqrt res (lo)
#     $t3 - sqrt res (hi)
#     $t4 - sqrt num (lo)
#     $t5 - sqrt num (hi)
#     $t6 - sqrt bit (lo)
#     $t7 - sqrt bit (hi)

#     $ac0 - temp
#     $ac1 - sum

# Calc abs(x)
    absq_s.w  $a1,$a0

# Check boundary region
    lui       $t1,0x8000
    ori       $t1,0x0020
    slt       $t0,$t1,$a0
    beqzc     $t0,UseLUTneg
    lui       $t1,0x7fff
    ori       $t1,0xffe0
    slt       $t0,$a0,$t1
    bnezc     $t0,CalcResult

# Use LUT for boundary region
UseLUT:
    subu      $t0,$a0,$t1
    sll       $t0,2                      # Use LUT for pos boundary
    la        $t1,AsinLUT
    b         ReturnQ229
    lwx       $v0,$t1($t0)               # Load LUT entry (delay slot)

UseLUTneg:
    subu      $t0,$t1,$a0
    sll       $t0,2                      # Use LUT for pos boundary
    la        $t1,AsinLUT
    lwx       $v0,$t1($t0)               # Load LUT entry
    b         ReturnQ229
    subu      $v0,$zero,$v0              # -result (delay slot)

# Calculate the asin value
CalcResult:
    move      $a3,$a1                     # initialize series exp arg
    lui       $t0,0x5A82                  # check for series exp range
    ori       $t0,0x799A
    slt       $a2,$a1,$t0
    bnezc     $a2,UseSeriesExp

    mulq_s.w  $t0,$a1,$a1
    li        $t1,0x80000000
    subu      $t0,$t1,$t0

# Calculate _Q31 square root
    mthi      $t0                        # initialize num
    mtlo      $zero
    shilo     $ac0,1
    mfhi      $t5
    mflo      $t4
    move      $t3,$zero                  # initialize res
    move      $t2,$zero
    lui       $t7,0x4000                 # initialize bit
    move      $t6,$zero
    mthi      $t7
    mtlo      $t6

ShiftLoop:
    subu      $v0,$t4,$t6                # num - bit
    sltu      $t0,$t4,$v0
    subu      $v1,$t5,$t7
    subu      $v1,$v1,$t0
    slt       $t0,$v1,$zero              # if num > bit then start loop
    beqzc     $t0,StartLoop
    shilo     $ac0,2
    mfhi      $t7
    b         ShiftLoop
    mflo      $t6                        # delay slot

# Calculate sqrt result
StartLoop:
    bnezc     $t6,CheckNum
    beqzc     $t7,EndLoop

# bit != 0
CheckNum:
    addsc     $t0,$t2,$t6                # res + bit
    addwc     $t1,$t3,$t7

    subu      $v0,$t4,$t0                # num - (res + bit)
    sltu      $t0,$t4,$v0
    subu      $v1,$t5,$t1
    subu      $v1,$v1,$t0
    slt       $t0,$v1,$zero
    bnezc     $t0,ShiftRes
    move      $t4,$v0                    # num -= res + bit
    move      $t5,$v1
    mthi      $t3                        # res >> 1
    mtlo      $t2
    shilo     $ac0,1
    mfhi      $t3
    mflo      $t2
    addsc     $t2,$t2,$t6                # res = (res >> 1) + bit
    b         ShiftBit2
    addwc     $t3,$t3,$t7                # delay slot

ShiftRes:
    mthi      $t3                        # res >>= 1
    mtlo      $t2
    shilo     $ac0,1
    mfhi      $t3
    mflo      $t2
ShiftBit2:
    mthi      $t7                        # bit >>= 2
    mtlo      $t6
    shilo     $ac0,2
    mfhi      $t7
    b         StartLoop
    mflo      $t6                        # delay slot
EndLoop:

# Round result
    move      $a3,$t2                    # Set result for series exp
    subu      $v0,$t2,$t4                # res - num
    sltu      $t0,$t2,$v0
    subu      $v1,$t3,$t5
    subu      $v1,$v1,$t0
    slt       $t0,$v1,$zero
    beqzc     $t0,SkipRound
    addi      $a3,1
SkipRound:
# Sqrt result is in a3

UseSeriesExp:
# sum = x^1 * 0x100000
    lui       $t0,0x0010
    mult      $ac1,$a3,$t0

# sum += x^3 * 0x2AAAD
    mulq_s.w  $t1,$a3,$a3
    lui       $t0,0x0002
    mulq_s.w  $t1,$t1,$a3
    ori       $t0,0xaaad
    madd      $ac1,$t0,$t1

# sum -= x^4 * 0x49
    mulq_s.w  $t1,$t1,$a3
    li        $t0,0x49
    msub      $ac1,$t0,$t1

# sum += x^5 * 0x139F5
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x0001
    ori       $t0,0x39f5
    madd      $ac1,$t0,$t1

# sum -=  x^6 * 0x6A7E
    mulq_s.w  $t1,$t1,$a3
    li        $t0,0x6A7E
    msub      $ac1,$t0,$t1

# sum += x^7 * 0x54EF0
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x0005
    ori       $t0,0x4EF0
    madd      $ac1,$t0,$t1

# sum -= x^8 * 0x24E78E
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x0024
    ori       $t0,0xE78E
    msub      $ac1,$t0,$t1

# sum += x^9 * 0xDDB545
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x00dd
    ori       $t0,0xb545
    madd      $ac1,$t0,$t1

# sum -= x^10 * 0x3EE8B29
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x03ee
    ori       $t0,0x8B29
    msub      $ac1,$t0,$t1

# sum += x^11 * 0xDB81AA5
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x0db8
    ori       $t0,0x1aa5
    madd      $ac1,$t0,$t1

# sum -= x^12 * 0x24DA30A0
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x24DA
    ori       $t0,0x30A0
    msub      $ac1,$t0,$t1

# sum += x^13 * 0x4C166E5A
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x4C16
    ori       $t0,0x6E5A
    madd      $ac1,$t0,$t1

# sum -= x^14 * 0x77AA04C6
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x77AA
    ori       $t0,0x04C6
    msub      $ac1,$t0,$t1

# sum += x^15 * 0x8CCDBF6F
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x4666
    ori       $t0,0xDFB7
    madd      $ac1,$t0,$t1
    addi      $t0,1
    madd      $ac1,$t0,$t1

# sum -=  x^16 * 0x780FC118
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x780F
    ori       $t0,0xC118
    msub      $ac1,$t0,$t1

# sum += x^17 * 0x461D65A7
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x461D
    ori       $t0,0x65A7
    madd      $ac1,$t0,$t1

# sum -= x^18 * 0x191D1385
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x191D
    ori       $t0,0x1385
    msub      $ac1,$t0,$t1

# sum += x^19 * 0x42B4D62
    mulq_s.w  $t1,$t1,$a3
    lui       $t0,0x042B
    ori       $t0,0x4D62
    madd      $ac1,$t0,$t1

# Adjust sum to Q2_29
    shilo     $ac1,-10
    mfhi      $v0,$ac1

# If we used sqrt, then result = pi/2 - result

    bnezc     $a2,CheckNeg
    lui       $t0,0x3243
    ori       $t0,0xf6a9
    subu      $v0,$t0,$v0
CheckNeg:

# If x < 0, then result = - result
    slt       $t0,$a0,$zero
    beqzc     $t0,ReturnQ229
    subu      $v0,$zero,$v0

# Return
ReturnQ229:
    jr        $ra
    nop

    .align 2
AsinLUT:
    .word 0x32428C9F
    .word 0x32429252
    .word 0x3242981E
    .word 0x32429E02
    .word 0x3242A401
    .word 0x3242AA1B
    .word 0x3242B052
    .word 0x3242B69F
    .word 0x3242BD20
    .word 0x3242C3BA
    .word 0x3242CA79
    .word 0x3242D160
    .word 0x3242D871
    .word 0x3242DFB0
    .word 0x3242E721
    .word 0x3242EEC8
    .word 0x3242F6A9
    .word 0x3242FECA
    .word 0x32430731
    .word 0x32430FE7
    .word 0x324318F5
    .word 0x32432265
    .word 0x32432C46
    .word 0x324336A9
    .word 0x324341A4
    .word 0x32434D55
    .word 0x324359E4
    .word 0x3243678D
    .word 0x324376A9
    .word 0x324387CF
    .word 0x32439C26
    .word 0x3243B6A9
    .word 0x3243F6A9

.end	_LIBQ_Q2_29_asin_Q31_Fast

/*******************************************************************************
 End of File
*/
