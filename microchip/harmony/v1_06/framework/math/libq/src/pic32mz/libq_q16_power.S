/*******************************************************************************
  _LIBQ_Q16Power:

    _Q16  _LIBQ_Q16Power (_Q16 x, _Q16 y);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q16_power.S

  Summary:
    Calculates the value of x raised to the y power (x^y).

  Description:
    Calculates the x raised to the y power.
    Both x and y are of type _Q16.  x must be positive.
    The calculation will saturate if the resulting value is
    outside the range of the _Q16 representation.

  Precondition:
    x must be positive.
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

.global _LIBQ_Q16Power
.ent    _LIBQ_Q16Power

_LIBQ_Q16Power:

# _Q16  _LIBQ_Q16Power (_Q16 x, _Q16 y);
#
# Input parameters:
#     $a0 - _Q16 x input value
#     $a1 - _Q16 y input value
#
# Output parameter:
#     $v0 - _Q16 output value
#
# Internal register usage:
#     $v1 - temp for 64-bit value
#     $t0 - temp
#     $t1 - temp
#     $t2 - temp - 64-bit lo
#     $t3 - temp - 64-bit hi
#     $t4 - loop count
#     $t5 - n
#     $t6 - temp
#     $ac0 - temp
#     $ac1 - temp


# Ensure x > 0
    slt       $t0,$zero,$a0
    bnezc     $t0,XisPos
    b         ReturnQ16
    lui       $v0,0x8000                   # Saturate result low (delay slot)
XisPos:

# Adjust x by 2^N where 1<=x<2
    clz       $t5,$a0                      # n = clz(x)
    sllv      $a0,$a0,$t5                  # x <<= n
    li        $t0,15
    subu      $t5,$t0,$t5                  # n = 15 - n

# Calculate log2
    move      $v0,$zero                    # Initialize result
    lui       $t2,0x4000                   # b = 1 << 30

# While x < 1.0
    lui       $t3,0x8000                   # 1.0
LoopOne:
    sltu      $t0,$a0,$t3
    beqzc     $t0,EndLoopOne
    sll       $a0,$a0,1                    # x <<= 1
    b         LoopOne
    subu      $v0,$v0,$t3                  # y -= 1.0 (delay slot)
EndLoopOne:

# Loop 31 times
    li        $t4,31                       # Loop count
Loop31:
    multu     $a0,$a0                      # x = x^2
    addiu     $t4,$t4,-1                   # decrement loop count
    extr_r.w  $a0,$ac0,31                  # x^2 >> 31 rounded
    shilo     $ac0,31                      # if z >= 2^31
    mfhi      $t0
    beqzc     $t0,DecLoopCnt
    mflo      $t0
    subu      $t0,$a0,$t0                  # duplicate rounding
    maddu     $ac0,$t0,$t0
    addu      $v0,$v0,$t2                  # y += b
    shilo     $ac0,1                       # x >>= 1
    mflo      $a0

DecLoopCnt:
    srl       $t2,$t2,1                    # b >>= 1
    bnezc     $t4,Loop31

# Round log2 result
    multu     $a0,$a0                      # x = x^2
    shilo     $ac0,31                      # if z >= 2^31
    mfhi      $t0
    beqzc     $t0,SkipRnd
    addiu     $v0,$v0,1                    # y += 1 (round)
SkipRnd:

# log2x += ((uint64_t)n) << 31
    sll       $t0,$t5,31
    addsc     $v0,$v0,$t0
    sra       $t0,$t5,1
    addwc     $v1,$t0,$zero                # log2x is in v1,v0

# Check for overflow
    mthi      $v1                          # extract int portion
    mtlo      $v0
    shilo     $ac0,-1
    mfhi      $t0
    mult      $t0,$a1                      # mult int * y
    shilo     $ac0,-1
    mfhi      $t0
    beqzc     $t0,NoOverflow               # no overflow if 0
    addiu     $t1,$t0,1                    # no overflow if -1
    beqzc     $t1,NoOverflow
    bltz      $t0,ReturnQ16                # return 0 if negative
    move      $v0,$zero                    # delay slot
    lui       $v0,0x7fff                   # sat high
    b         ReturnQ16
    ori       $v0,0xffff                   # delay slot
NoOverflow:

# Calculate ylog2x
    bgez      $a1,UpperMult
    multu     $ac1,$v0,$a1                 # mult lower 32 bits (delay slot)
    subu      $a2,$zero,$a1                # mult unsigned by signed
    mult      $ac1,$zero,$zero
    msubu     $ac1,$v0,$a2
UpperMult:
    shilo     $ac1,-1
    mult      $ac0,$v1,$a1                 # mult upper 32 bits
    shilo     $ac0,-1
    mflo      $t0
    mfhi      $v1,$ac1
    mflo      $v0,$ac1
    addu      $v1,$v1,$t0

# If ylog2x > 0
    bltz      $v1,Ylog2xLE0
    li        $t2,13                       # constant 13
    bnezc     $v1,Ylog2xGT0
    beqzc     $v0,Ylog2xLE0
Ylog2xGT0:
    sra       $t5,$v1,16                   # int portion
    b         EndNcalc
    subu      $t5,$t2,$t5                  # delay slot
Ylog2xLE0:
    subu      $t0,$zero,$v0                # -ylog2x
    sltu      $t3,$zero,$t0
    subu      $t1,$zero,$v1
    subu      $t1,$t1,$t3
    sra       $t5,$t1,16                   # int portion
    bnezc     $t0,IncN                     # inc n if frac portion non-zero
    andi      $t3,$t1,0xFFFF
    beqzc     $t3,SkipIncN
IncN:
    addiu     $t5,$t5,1                    # increment n
SkipIncN:
    sll       $t4,$t5,16                   # add n to ylog2x
    addu      $v1,$v1,$t4
    addu      $t5,$t5,$t2                  # n += 13
EndNcalc:

# Extract fractional value
    andi      $v1,$v1,0xffff               # extract frac(ylog2x)
    mthi      $v1
    mtlo      $v0
    shilo     $ac0,17
    lui       $t1,0x58b9                   # mult by ln(2)
    mflo      $t0
    ori       $t1,$t1,0x0bfc
    mult      $t0,$t1
    extr_r.w  $t4,$ac0,31                  # round

# Calculate exp
# x^10
    li        $t3,0x0093

    mult      $ac1,$t3,$t4                 # tempOut *= x
    lui       $t2,0xF27D                   # tempOut   = 0x00000093F27DBBC5;
    ori       $t2,$t2,0xBBC5
    multu     $ac0,$t2,$t4
    lui       $t0,0x78E9                   # tempOut   += 0x000005C778E955B2
    shilo     $ac1,-1
    shilo     $ac0,-1
    mflo      $t2,$ac1
    mfhi      $t6
    mfhi      $t3,$ac1
    ori       $t0,0x55B2
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# x^9
    li        $t1,0x05c7
    addsc     $t2,$t2,$t0
    addwc     $t3,$t3,$t1
    mult      $ac1,$t3,$t4                 # tempOut *= x
    multu     $ac0,$t2,$t4
    lui       $t0,0x4034                   # tempOut   += 0x0000340340340340
    ori       $t0,0x0340
    shilo     $ac1,-1
    shilo     $ac0,-1
    li        $t1,0x3403
    mfhi      $t6
    mflo      $t2,$ac1
    mfhi      $t3,$ac1
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# x^8
    addsc     $t2,$t2,$t0
    addwc     $t3,$t3,$t1
    mult      $ac1,$t3,$t4                 # tempOut *= x
    multu     $ac0,$t2,$t4
    lui       $t0,0x01A0                   # tempOut   += 0x0001A01A01A01A02
    ori       $t0,0x1A02
    shilo     $ac1,-1
    shilo     $ac0,-1
    lui       $t1,0x0001
    mfhi      $t6
    mflo      $t2,$ac1
    mfhi      $t3,$ac1
    ori       $t1,0xA01A
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# x^7
    addsc     $t2,$t2,$t0
    addwc     $t3,$t3,$t1
    mult      $ac1,$t3,$t4                 # tempOut *= x
    multu     $ac0,$t2,$t4
    lui       $t0,0x0B60                   # tempOut   += 0x000B60B60B60B60B
    ori       $t0,0xB60B
    shilo     $ac1,-1
    shilo     $ac0,-1
    lui       $t1,0x000B
    mfhi      $t6
    mflo      $t2,$ac1
    mfhi      $t3,$ac1
    ori       $t1,0x60B6
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# x^6
    addsc     $t2,$t2,$t0
    addwc     $t3,$t3,$t1
    mult      $ac1,$t3,$t4                 # tempOut *= x
    multu     $ac0,$t2,$t4
    lui       $t0,0x4444                   # tempOut   += 0x0044444444444444
    ori       $t0,0x4444
    shilo     $ac1,-1
    shilo     $ac0,-1
    lui       $t1,0x0044
    mfhi      $t6
    mflo      $t2,$ac1
    mfhi      $t3,$ac1
    ori       $t1,0x4444
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# x^5
    addsc     $t2,$t2,$t0
    addwc     $t3,$t3,$t1
    mult      $ac1,$t3,$t4                 # tempOut *= x
    multu     $ac0,$t2,$t4
    lui       $t0,0x5555                   # tempOut   += 0x0155555555555555
    ori       $t0,0x5555
    shilo     $ac1,-1
    shilo     $ac0,-1
    lui       $t1,0x0155
    mfhi      $t6
    mflo      $t2,$ac1
    mfhi      $t3,$ac1
    ori       $t1,0x5555
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# x^4
    addsc     $t2,$t2,$t0
    addwc     $t3,$t3,$t1
    mult      $ac1,$t3,$t4                 # tempOut *= x
    multu     $ac0,$t2,$t4
    lui       $t0,0x5555                   # tempOut   += 0x0555555555555555
    ori       $t0,0x5555
    shilo     $ac1,-1
    shilo     $ac0,-1
    lui       $t1,0x0555
    mfhi      $t6
    mflo      $t2,$ac1
    mfhi      $t3,$ac1
    ori       $t1,0x5555
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# x^3
    addsc     $t2,$t2,$t0
    addwc     $t3,$t3,$t1
    mult      $ac1,$t3,$t4                 # tempOut *= x
    multu     $ac0,$t2,$t4
    lui       $t1,0x1000                   # tempOut   += 0x1000000000000000
    shilo     $ac1,-1
    shilo     $ac0,-1
    mfhi      $t6
    mflo      $t2,$ac1
    mfhi      $t3,$ac1
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# x^2
    addu      $t3,$t3,$t1
    mult      $ac1,$t3,$t4                 # tempOut *= x
    multu     $ac0,$t2,$t4
    lui       $t1,0x2000                   # tempOut   += 0x2000000000000000
    shilo     $ac1,-1
    shilo     $ac0,-1
    mfhi      $t6
    mflo      $t2,$ac1
    mfhi      $t3,$ac1
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# x^1
    addu      $t3,$t3,$t1
    mult      $ac1,$t3,$t4                 # tempOut *= x
    multu     $ac0,$t2,$t4
    shilo     $ac1,-1
    shilo     $ac0,-1
    mfhi      $t6
    mflo      $t2,$ac1
    mfhi      $t3,$ac1
    addsc     $t2,$t6,$t2
    addwc     $t3,$zero,$t3

# Shift by n
    blez      $t5,Nle0
    addu      $t3,$t3,$t1                  # x^0 tempOut   += 0x2000000000000000; (delay slot)

# Shift right by n
    li        $t0,32                       # If n > 32 return 0 due to overflow
    slt       $t0,$t0,$t5
    bnez      $t0,ReturnQ16
    move      $v0,$zero                    # delay slot
    mthi      $t3                          # shift right by n
    mtlo      $t2
    srl       $t4,$t5,5                    # if n == 32 add an extra shift
    beqzc     $t4,SkipShift32
    addiu     $t5,$t5,-1                   # n was 32 so shift extra
    shilo     $ac0,1
SkipShift32:
    andi      $t4,$t5,0x001F               # shift right up to 31
    shilov    $ac0,$t4
    shilo     $ac0,1                       # adjust for extract and rounding
    b         ReturnQ16                    # return result
    extr_rs.w $v0,$ac0,31                  # extract rounded result (delay slot(

# Shift left by n
Nle0:
    clz       $t0,$t3                      # n < 0, so shift left
    bnezc     $t3,SkipCLZlo                # count leading zeros
    clz       $t1,$t2
    addu      $t0,$t0,$t1
SkipCLZlo:
    li        $t1,64                       # ensure left shift will not overflow
    slt       $t1,$t0,$t1
    beqz      $t1,ReturnQ16
    move      $v0,$zero                    # will overflow if -n > # zeros - 1
    subu      $t4,$zero,$t5
    addiu     $t4,$t4,1
    slt       $t4,$t0,$t4
    beqzc     $t4,ShiftLeft
    lui       $v0,0x7fff                   # Will overflow, so saturate high
    b         ReturnQ16
    ori       $v0,$v0,0xffff
ShiftLeft:
    mthi      $t3                          # shift left by n
    mtlo      $t2
    lui       $t0,0xffff                   # if n < -32
    ori       $t0,0xffe0
    slt       $t1,$t0,$t5
    bnezc     $t1,SkipLeftShift32
    subu      $t5,$t5,$t0                  # shift left by 32
    shilo     $ac0,-1
    shilo     $ac0,-31
SkipLeftShift32:
    shilov    $ac0,$t5                     # shift left up to 31
    shilo     $ac0,1                       # adjust for extract
    extr_rs.w $v0,$ac0,31                  # extract rounded result

# Return
ReturnQ16:
    jr        $ra
    nop

.end    _LIBQ_Q16Power

/*******************************************************************************
 End of File
*/
