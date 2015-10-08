/*******************************************************************************
  _LIBQ_Q2_29_asin_Q31:

    _Q2_29  _LIBQ_Q2_29_asin_Q31 (_Q31 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q2_29_asin_q31.S

  Summary:
    Calculates the value of asin(x).

  Description:
    Calculates the asin(x), where x is of type _Q31 and
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

.global _LIBQ_Q2_29_asin_Q31
.global _LIBQ_Q2_29_asin_Q31_Fast
.global _LIBQ_Q31_sin_Q2_29
.ent    _LIBQ_Q2_29_asin_Q31

_LIBQ_Q2_29_asin_Q31:

#  _Q2_29  _LIBQ_Q2_29_asin_Q31 (_Q31 x);
#
# Input parameters:
#     $a0 - _Q31 input value
#
# Output parameter:
#     $v0 - _Q2_29 output value
#
# Internal register usage:
#     $t0 - temp
#     $t1 - temp
#     $t2 - temp

#     $sp - Increased by 16 to store return address and temporaries
#     $sp+12  - ra
#     $sp+8   - $s2
#     $sp+4   - $s1
#     $sp+0   - $s0

# Push stack
    addiu     $sp,$sp,-16
    swm       $s0,$s1,$s2,$ra,0($sp)     # Save return address, s0, s1, s2

# Get an estimate of the asin value
    jal       _LIBQ_Q2_29_asin_Q31_Fast # Get asin estimate
    move      $s0,$a0                   # Save input (delay slot)
    move      $s1,$v0                   # Save result

# Check result
CheckResult:
    jal       _LIBQ_Q31_sin_Q2_29       # Call sin
    move      $a0,$v0                   # delay slot
    subu      $t0,$v0,$s0               # calc difference
    bnezc     $t0,RefineResult
    b         ReturnAsinQ229            # return accurate result
    move      $v0,$s1                   # restore result (delay slot)
RefineResult:
    bgtz      $t0,DecValue              # Should value be decreased
    li        $t1,0x0400                # Load constant 0x0400 (delay slot)
# Increase value
    addu      $s2,$s1,$t1               # Calculate value limit to try
    lui       $t0,0x3243                # Limit to pi/2
    ori       $t0,0xf6a8
    slt       $t2,$t0,$s2
    beqz      $t2,CalcBiSection
    move      $s2,$t1                   # Assume diff will not change (delay slot)
    b         CalcBiSection
    subu      $s2,$t0,$s1               # Limit to pi/2 (delay slot)

# Decrease value
DecValue:
    subu      $t3,$s1,$t1               # Calculate value limit to try
    lui       $t0,0xcdbc                # Limit to -pi/2
    ori       $t0,$t0,0x0958
    slt       $t2,$t3,$t0
    beqzc     $t2,NoDecLimit
    move      $t3,$t0                   # Limit to -pi/2
NoDecLimit:
    subu      $s2,$s1,$t3               # Calc diff
    move      $s1,$t3

# Use BiSection to find value to _Q31 precision
CalcBiSection:
    blez      $s2,ReturnAsinQ229        # quit if diff <= 0
    move      $v0,$s1                   # set result value (delay slot)
    sra       $s2,$s2,1                 # Diff/2
    jal       _LIBQ_Q31_sin_Q2_29       # Call sin
    addu      $a0,$s1,$s2               # Calculate sin value to try (delay slot)
    subu      $t0,$v0,$s0               # Calc error
    bgtz      $t0,CalcBiSection
    nop                                 # delay slot
    addu      $s1,$s1,$s2               # Update result
    bnez      $t0,CalcBiSection         # loop if error != 0
    move      $v0,$s1                   # set result value (delay slot)

# Return
ReturnAsinQ229:
    lwm       $s0,$s1,$s2,$ra,0($sp)   # Restore return address, s0, s1, s2
    jraddiusp 16                       # Return

.end    _LIBQ_Q2_29_asin_Q31

/*******************************************************************************
 End of File
*/
