/*******************************************************************************
  _LIBQ_Q2_13_asin_Q15:

      _Q2_13  _LIBQ_Q2_13_asin_Q15 (_Q15 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q2_13_asin_q15.S

  Summary:
    Calculates the asin value of asin(x).

  Description:
    Calculates asin(x), where x is of type _Q15 and
    the resulting value is of type _Q2_13.  The output value
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

.global _LIBQ_Q2_13_asin_Q15
.global _LIBQ_Q16Div
.global _LIBQ_Q16Sqrt
.ent    _LIBQ_Q2_13_asin_Q15

_LIBQ_Q2_13_asin_Q15:

#  _Q2_13  _LIBQ_Q2_13_asin_Q15 (_Q15 x);
#
# Input parameters:
#     $a0 - _Q15 input value
#
# Output parameter:
#     $v0 - _Q2_13 output value
#
# Internal register usage:
#     $t0 - temp
#     $t1 - angle ^ n
#     $t2 - angle squared
#     $t3 - result

#     $s0 - sign of y
#     $s1 - x and sign of x

#     $sp - Increased by 16 to store return address, s0, & s1
#     $sp+8  - $ra
#     $sp+4  - $s1
#     $sp+0  - $s0

# Push stack
    addiu     $sp,$sp,-16
    swm       $s0,$s1,$ra,0($sp)   # Save return address, s0, & s1

# If x is -1, prevent divide by zero and return -pi/2
    lui       $t0,0xffff
    ori       $t0,0x8000
    subu      $t0,$a0,$t0
    bnezc     $t0,NoDivZero
    b         ReturnQ213
    ori       $v0,$zero,0xCDBC             # delay slot
NoDivZero:

# Convert to _Q16
    sll       $s1,$a0,1

# Calculate x/(1 - x^2)^0.5
    mult      $s1,$s1
    extr_rs.w $t0,$ac0,16
    lui       $t1,0x0001
    jal       _LIBQ_Q16Sqrt
    subu      $a0,$t1,$t0                  # delay slot
    move      $a0,$s1
    jal       _LIBQ_Q16Div
    move      $a1,$v0                      # delay slot

# Calculate atan2(x/(1-x^2)^0.5, 1)
    move      $a0,$v0
    lui       $a1,0x0001

# Calculate abs(y)
    slt       $s1,$a1,$zero         # store sign of x
    slt       $s0,$a0,$zero         # store sign of y
    beqzc     $s0,YisPos
    subu      $a0,$zero,$a0
YisPos:

# Calculate angle adjustment
    slt       $t0,$a1,$zero
    bnezc     $t0,XltZero
    subu      $t0,$a1,$a0
    b         DoDivide
    addu      $a1,$a1,$a0            # delay slot
XltZero:
    addu      $t0,$a1,$a0
    subu      $a1,$a0,$a1
DoDivide:
    jal       _LIBQ_Q16Div
    move      $a0,$t0                # delay slot

# Saturate Q15 adjusted angle
    lui       $t1,0x0001
    slt       $t0,$v0,$t1
    bnezc     $t0,AngleLtOne
    b         AngleAdjusted          # Saturate Q15 high
    li        $v0,0x7fff
AngleLtOne:
    lui       $t1,0xffff
    slt       $t0,$v0,$t1
    beqzc     $t0,AngleGEMinusOne
    b         AngleAdjusted          # Saturate Q15 low
    ori       $v0,$t1,0x8000
AngleGEMinusOne:
    sra       $v0,$v0,1              # Convert Q16 to Q15
AngleAdjusted:

# Calculate angle^2
    mul       $t2,$v0,$v0
    sra       $t2,$t2,15

# Calculate angle^3
    mul       $t1,$t2,$v0
    sra       $t1,$t1,15

# Calculate angle^3 * 0.331544619 - angle*0.999964798
    li        $t0,0x2a70
    mul       $t3,$t1,$t0
    li        $t0,0x7fff
    mul       $t0,$v0,$t0
    subu      $t3,$t3,$t0

# Calculate angle^5
    mul       $t1,$t1,$t2
    sra       $t1,$t1,15

# Result -= angle^5 * 0.184463558
    li        $t0,0x179d
    mul       $t0,$t1,$t0
    subu      $t3,$t3,$t0

# Calculate angle^7
    mul       $t1,$t1,$t2
    sra       $t1,$t1,15

# Result += angle^7 * 0.090752018
    li        $t0,0x0b9e
    mul       $t0,$t1,$t0
    addu      $t3,$t3,$t0

# Calculate angle^9
    mul       $t1,$t1,$t2
    sra       $t1,$t1,15

# Result -= angle^9 * 0.023286008
    li        $t0,0x02fb
    mul       $t0,$t1,$t0
    subu      $t3,$t3,$t0

# Result = result >> 14
    shra_r.w  $t3,$t3,14

# Adjust angle
    bnezc     $s1,XisNegative
    ori       $t0,$zero,0xc90f
    b         ResultAdjusted
    addu      $v0,$t3,$t0          # add PI/4 (delay slot)
XisNegative:
    lui       $t0,0x0002           # add 3PI/4
    ori       $t0,0x5B2F
    addu      $v0,$t3,$t0
ResultAdjusted:
    beqzc     $s0,SatQ213          # if y < 0
    subu      $v0,$zero,$v0        #   then angle = -angle

# Convert to Q2_13 rounded, saturated
SatQ213:
    addu      $v0,0x0004           # round
    lui       $t0,0x0004
    slt       $t0,$v0,$t0          # if result >= 4 then saturate high
    bnezc     $t0,CheckSatLow
    b         ReturnQ213
    li        $v0,0x7fff           # saturate return value (delay slot)
CheckSatLow:
    lui       $t0,0xfffc
    slt       $t0,$t0,$v0          # if result <= -4 then saturate low
    bnezc     $t0,ConvertQ213
    lui       $v0,0xffff
    b         ReturnQ213
    ori       $v0,$v0,0x8000       # saturate return value (delay slot)
ConvertQ213:
    sra       $v0,$v0,3            # Convert to Q7_8

# Return
ReturnQ213:
    lwm       $s0,$s1,$ra,0($sp)   # Restore return address, s0, & s1
    jraddiusp 16                   # Return

.end    _LIBQ_Q2_13_asin_Q15

/*******************************************************************************
 End of File
*/
