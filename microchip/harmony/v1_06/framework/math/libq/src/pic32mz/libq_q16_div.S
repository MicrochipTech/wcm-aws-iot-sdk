/*******************************************************************************
  _LIBQ_Q16Div:

    _Q16  _LIBQ_Q16Div (_Q16 dividend, _Q16 divisor);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q16_div.S

  Summary:
    Optimized _Q16 divide.

  Description:
    _Q16 divide optimized for speed.
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

.global _LIBQ_Q16Div
.ent    _LIBQ_Q16Div

_LIBQ_Q16Div:

# _Q16  _LIBQ_Q16Div (_Q16 dividend, _Q16 divisor);
#
# Input parameters:
#     $a0 - dividend
#     $a1 - divisor
#
# Output parameter:
#     $v0 - quotient
#
# Internal register usage:
#     $a0 - Remainder
#     $a1 - Divider
#     $t0 - Sign of result
#     $t2 - Bit position
#     $t3 - 0xFFFFFFFF
#     $t6 - Temp
#     $t7 - Temp

# Ensure divisor is non-zero, otherwise return 0
    beqz    $a1,ReturnDivQ
    li      $v0,0              # Quotient = 0 (delay slot)

# Make dividend and remainder positive
    bgez    $a0,DividendPos
    xor     $t0,$a0,$a1        # Save result sign (delay slot)
    negu    $a0,$a0
DividendPos:
    srl     $t0,31             # Result sign is 1 if negative
    bgez    $a1,DivisorPos
    li      $t2,17             # Bit Pos  = 17 (delay slot)
    negu    $a1,$a1
DivisorPos:

# Skip iterations if divisor is large
    move    $t7,$a1
    srl     $t7,20
    beqzc   $t7,DivisorNotLarge  # No delay slot
    move    $t7,$a1            # small divisor = (divisor >> 17) + 1
    srl     $t7,17
    addiu   $t7,$t7,1
    divu    $zero,$a0,$t7      # quotient = remainder / small divisor
    mflo    $v0
    multu   $v0,$a1            # temp = (quotient * divider) >> 17
    shilo   $ac0,17
    mflo    $t7
    subu    $a0,$a0,$t7        # remainder = remainder - temp
DivisorNotLarge:

# Optimize divisor that is multiple of 16
LoopOpt16:
    andi    $t7,$a1,0xF        # is divisor multiple of 16
    bnezc   $t7,EndLoopOpt16   # no delay slot
    srl     $t7,$t2,2          # is bit position >=4
    beqzc   $t7,EndLoopOpt16   # no delay slot
    srl     $a1,4              # divisor >>= 4
    li      $t7,4              # bit position -= 4
    b       LoopOpt16          # one delay slot
    sub     $t2,$t2,$t7
EndLoopOpt16:

# Load constant 0xFFFFFFFF to use as bit mask
    addiu     $t3,$zero,-1       # all ones

# For all remaining bit positions, while remainder > 0
LoopBitPos:
    beqzc   $a0,EndLoopBitPos
    clz     $t7,$a0            # count leading zeros in remainder
    bltz    $t2,EndLoopBitPos
    slt     $t6,$t2,$t7        # limit leading zeros to bit position (delay slot)
    beqzc   $t6,LZgtrBP        # no delay slot
    move    $t7,$t2
LZgtrBP:
    sllv    $a0,$a0,$t7        # remainder <<= leading zeros
    subu    $t2,$t2,$t7        # bit position -= leading zeros

# Calculate new estimate
    divu    $zero,$a0,$a1      # remainder / divider
    mfhi    $a0                # update remainder
    mflo    $t7                # shift divide result
    sllv    $t6,$t7,$t2
    addu    $v0,$v0,$t6        # add to quotient

# Check divide result for saturation
    srlv      $t6,$t3,$t2      # shift bit mask right bit position bits
    not       $t6,$t6
    and       $t7,$t7,$t6      # if any upper bit is one
    beqzc     $t7,NotSat       # no delay slot
    lui       $v0,0x7FFF       # load max value
    beqz      $t0,ReturnDivQ   # is value negative?
    ori       $v0,$v0,0xFFFF   # delay slot
    b         ReturnDivQ       # negative, so load min value
    addiu     $v0,$v0,1        # 0x80000000 (delay slot)

NotSat:

# Iterate LoopBitPos
    sll       $a0,$a0,1        # shift remainder left 1 bit
    b         LoopBitPos       # loop
    addiu     $t2,$t2,-1       # decrement bit position (delay slot)

# Adjust quotient
EndLoopBitPos:
    addiu     $v0,$v0,1        # round up
    srl       $v0,$v0,1        # divide by 2

# Adjust sign
AdjustSign:
    beqzc     $t0,SignAdjusted # no delay slot
    subq_s.w  $v0,$zero,$v0    # make result negative
SignAdjusted:

# Return
ReturnDivQ:
    jr        $ra
    nop

.end    _LIBQ_Q16Div

/*******************************************************************************
 End of File
*/
