/*******************************************************************************
  _LIBQ_Q31FromFloat:

    _Q31  _LIBQ_Q31FromFloat(float x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q31_from_float.S

  Summary:
    Converts a float to a _Q31 value.

  Description:
    Converts a floating point value to a _Q31 fixed point
    representation.  The _Q31 fixed point value is returned
    by the function.  The conversion will saturate if
    the value is outside the range of the _Q31 representation.
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

.global _LIBQ_Q31FromFloat
.global __gesf2
.global __lesf2
.global __mulsf3
.global __fixsfsi
.ent    _LIBQ_Q31FromFloat

_LIBQ_Q31FromFloat:

# _Q31  _LIBQ_Q31FromFloat(float x);
#
# Input parameters:
#     $a0 - float input value
#
# Output parameter:
#     $v0 - _Q31 output value
#
# Internal register usage:
#     $sp - Increased by 8 to store $ra and $s0
#     $a0 - float input value
#     $a1 - second function call argument
#     $s0 - saved float input value

# Convert integer to float
    addiu     $sp,$sp,-8
    swm       $s0,$ra,0($sp)
    move      $s0,$a0          # Preserve input parameter

# Multiply by 0x80000000
    jal       __mulsf3         # Multiply by 0x80000000
    lui       $a1,0x4F00       # delay slot
    move      $a0,$v0

# Round result to nearest integer
    lui       $a1,0x3F00       # +/- 0.5
    lui       $t0,0x8000
    and       $t0,$a0,$t0
    jal       __addsf3
    or        $a1,$a1,$t0      # delay slot
    move      $s0,$v0
    move      $a0,$s0

# Check high range
    jal       __gesf2          # Is result >= 0x7FFFFFFF
    lui       $a1,0x4F00       # (float)0x7FFFFFFF (delay slot)
    bltz      $v0,NotSatHigh
    move      $a0,$s0          # Restore result (delay slot)
    lui       $v0,0x7FFF       # Saturate 0x7FFFFFFF
    ori       $v0,$v0,0xFFFF
    beqzc     $zero,ReturnQ31FromFloat
NotSatHigh:

# Check low range
    jal       __lesf2          # Is result <= -0x80000000
    lui       $a1,0xCF00       # (float)-0x80000000 (delay slot)
    bgtz      $v0,NotSat
    move      $a0,$s0          # Restore result (delay slot)
    lui       $v0,0x8000       # Saturate 0x80000000
    beqzc     $zero,ReturnQ31FromFloat
NotSat:

# Convert to integer
    jal       __fixsfsi        # Convert to integer
    nop                        # delay slot

# Return
ReturnQ31FromFloat:
    lwm       $s0,$ra,0($sp)
    jraddiusp 8                # Return

.end    _LIBQ_Q31FromFloat

/*******************************************************************************
 End of File
*/
