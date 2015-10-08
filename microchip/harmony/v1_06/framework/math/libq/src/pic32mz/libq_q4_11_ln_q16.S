/*******************************************************************************
  _LIBQ_Q4_11_ln_Q16:

    _Q4_11  _LIBQ_Q4_11_ln_Q16 (_Q16 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q4_11_ln_q16.S

  Summary:
    Calculates the natural logarithm ln(x).

  Description:
    Calculates the natural logarithm ln(x).  x is of type _Q16 and must be positive.
    The resulting value is of type _Q4_11.

  Precondition:
    The input x must be positive.
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

.global _LIBQ_Q4_11_ln_Q16
.ent    _LIBQ_Q4_11_ln_Q16

_LIBQ_Q4_11_ln_Q16:

# _Q4_11  _LIBQ_Q4_11_ln_Q16 (_Q16 x);
#
# Input parameters:
#     $a0 - _Q16 input value
#
# Output parameter:
#     $v0 - _Q4_11 output value
#
# Internal register usage:
#     $t0 - temp
#     $t1 - temp
#     $t2 - temp
#     $t3 - temp
#     $t4 - loop count
#     $ac0 - temp

# Ensure x > 0
    slt       $t0,$zero,$a0
    bnezc     $t0,XisPos
    lui       $v0,0xffff                   # Saturate result low
    b         ReturnQ411
    ori       $v0,$v0,0x8000               # delay slot
XisPos:

# Calculate log2
    move      $v0,$zero                    # Initialize result
    ori       $t2,$zero,0x8000             # b = 1 << 15

# While x < 1.0
    lui       $t3,0x0001                   # 1.0
LoopOne:
    sltu      $t0,$a0,$t3
    beqzc     $t0,EndLoopOne
    sll       $a0,$a0,1                    # x <<= 1
    b         LoopOne
    subu      $v0,$v0,$t3                  # y -= 1.0 (delay slot)
EndLoopOne:

# While x >= 2.0
    lui       $t1,0x0002                   # 2.0 (2 << 16)
LoopTwo:
    sltu      $t0,$a0,$t1
    bnezc     $t0,EndLoopTwo
    srl       $a0,$a0,1                    # x >>= 1
    b         LoopTwo
    addu      $v0,$v0,$t3                  # y += 1.0 (delay slot)
EndLoopTwo:

# Loop 14 times
    li        $t4,14                       # Loop count
Loop14:
    mult      $a0,$a0                      # x = x^2
    extr_rs.w $a0,$ac0,16                  # x^2 >> 16 rounded
    sltu      $t0,$a0,$t1                  # if x >= 2.0
    bnezc     $t0,DecLoopCnt
    srl       $a0,$a0,1                    # x >>= 1
    addu      $v0,$v0,$t2                  # y += b
DecLoopCnt:
    srl       $t2,$t2,1                    # b >>= 1
    addiu     $t4,$t4,-1                   # decrement loop count
    bnezc     $t4,Loop14

# ln(x) = log2(x) / log2(e) = log2(x) * (1/log2(e)) = log2(x) * 0.69314718055
    ori       $t0,$zero,0xB172
    mult      $v0,$t0                      # log2(x) / log2(e)
    extr_rs.w $v0,$ac0,16

# convert to Q4_11
    li        $t0,0x0010
    addq_s.w  $v0,$v0,$t0                  # round result
    lui       $t0,0x0010                   # Limit high
    slt       $t0,$t0,$v0
    beqzc     $t0,CheckSatMin
    b         ReturnQ411                   # Saturate high
    li        $v0,0x7fff
CheckSatMin:
    lui       $t0,0xFFF0                   # Limit low
    slt       $t0,$t0,$v0
    bnezc     $t0,ShiftRes
    lui       $v0,0xffff                   # Saturate low
    b         ReturnQ411
    ori       $v0,$v0,0x8000
ShiftRes:
    sra       $v0,5

# Return
ReturnQ411:
    jr        $ra
    nop

.end    _LIBQ_Q4_11_ln_Q16

/*******************************************************************************
 End of File
*/
