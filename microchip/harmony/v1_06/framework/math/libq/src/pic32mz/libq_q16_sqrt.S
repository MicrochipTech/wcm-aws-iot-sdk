/*******************************************************************************
  _LIBQ_Q16Sqrt:

    _Q16  _Q16sqrt (_Q16 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q16_sqrt.S

  Summary:
    Square root of a positive _Q16 fixed point value.

  Description:
    Calculate the square root of a positive _Q16 fixed point value,
    and return the _Q16 result.
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

.global _LIBQ_Q16Sqrt
.ent    _LIBQ_Q16Sqrt

_LIBQ_Q16Sqrt:

# _Q16  _LIBQ_Q16Sqrt (_Q16 x);
#
# Input parameters:
#     $a0 - _Q16 input value
#
# Output parameter:
#     $v0 - _Q16 output value of the square root
#
# Internal register usage:
#     $a0 - Remainder
#     $t0 - Bit
#     $t1 - Loop count
#     $t6 - temp
#     $t7 - temp

# Initialize result
    li     $v0,0               # result = 0

# Estimate starting range
    clz    $t7,$a0             # Count leading zeros
    sltiu  $t6,$t7,32          # Limit shift count to 31
    bnezc  $t6,ShiftValid
    li     $t6,31
ShiftValid:
    lui    $t0,0x8000          # Calculate initial bit value
    srlv   $t0,$t0,$t6

# Calculate top 24-bits
WhileBit:
    beqzc  $t0,BitIsZero       # while (Bit)
    addu   $t7,$v0,$t0         # result + bit
    sltu   $t6,$a0,$t7         # if remainder >= result + bit
    bnez   $t6,RemLT
    srl    $v0,1               # result /= 2 (delay slot)
    subu   $a0,$a0,$t7         # remainder -= result + bit
    add    $v0,$v0,$t0         # result += bit
RemLT:
    b      WhileBit
    srl    $t0,$t0,2           # bit >>= 2 (delay slot)
BitIsZero:

# Calculate lowest 8 bits
    lui    $t7,0xFFFF          # remainder >= 0x00010000
    and    $t7,$t7,$a0
    beqzc  $t7,LowRem
    subu   $a0,$a0,$v0         # remainder -= result
    sll    $a0,$a0,16          # remainder <<= 16
    li     $t7,0x8000          # remainder -= 0x8000
    subu   $a0,$a0,$t7
    sll    $v0,$v0,16          # result <<= 16
    b      EndNumCheck
    addu   $v0,$v0,$t7         # result += 0x8000 (delay slot)
LowRem:
    sll    $a0,$a0,16          # remainder <<= 16
    sll    $v0,$v0,16          # result <<= 16
EndNumCheck:
    li     $t0,0x4000          # bit = 0x4000

# Recalculate 24-bits
WhileBit2:
    beqzc  $t0,BitIsZero2      # while (Bit)
    addu   $t7,$v0,$t0         # result + bit
    sltu   $t6,$a0,$t7         # if remainder >= result + bit
    bnez   $t6,RemLT2
    srl    $v0,1               # result /= 2 (delay slot)
    subu   $a0,$a0,$t7         # remainder -= result + bit
    add    $v0,$v0,$t0         # result += bit
RemLT2:
    b      WhileBit2
    srl    $t0,$t0,2           # bit >>= 2 (delay slot)
BitIsZero2:

# Round result
    sltu   $t7,$v0,$a0         # remainder > result?
    beqzc  $t7,EndRound
    addiu  $v0,1               # round result
EndRound:

# Return
    jr        $ra
    nop

.end    _LIBQ_Q16Sqrt

/*******************************************************************************
 End of File
*/
