/*******************************************************************************
  _LIBQ_Q15FromFloat:

    _Q15  _LIBQ_Q15FromFloat(float x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q15_from_float.S

  Summary:
    Converts a float to a _Q15 value.

  Description:
    Converts a floating point value to a _Q15 fixed point
    representation.  The _Q15 fixed point value is returned
    by the function.  The conversion will saturate if
    the value is outside the range of the _Q15 representation.
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

.global _LIBQ_Q15FromFloat
.global __gesf2
.global __lesf2
.global __addsf3
.global __mulsf3
.global __fixsfsi
.ent    _LIBQ_Q15FromFloat

_LIBQ_Q15FromFloat:

# _Q15  _LIBQ_Q15FromFloat(float x);
#
# Input parameters:
#     $a0 - float input value
#
# Output parameter:
#     $v0 - _Q15 output value
#
# Internal register usage:
#     $sp - Increased by 8 to store $ra and $s0
#     $a0 - float input value
#     $a1 - second function call argument
#     $s0 - saved float input value
#     $t0 - temp

# Convert integer to float
    addiu     $sp,$sp,-8
    swm       $s0,$ra,0($sp)   # Save return address and s0

# Multiply float by 2**15
    jal       __mulsf3         # Multiply by 0x8000
    lui       $a1,0x4700       # delay slot
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
    lui       $a1,0x46FF       # Is input >= 0x7FFF
    jal       __gesf2
    ori       $a1,0xFE00       # delay slot
    bltz      $v0,NotSatHigh
    move      $a0,$s0          # Restore input param (delay slot)
    b         ReturnQ15FromFloat
    li        $v0,0x7FFF       # Saturate 0x7FFF (delay slot)
NotSatHigh:

# Check low range
    jal       __lesf2          # Is input <= -0x8000
    lui       $a1,0xC700       # delay slot
    bgtz      $v0,NotSat
    move      $a0,$s0          # Restore input param (delay slot)
    b         ReturnQ15FromFloat
    li        $v0,0x8000       # Saturate 0x8000 (delay slot)
NotSat:

# Convert to integer
    jal       __fixsfsi        # Convert to integer
    nop                        # delay slot

# Return
ReturnQ15FromFloat:
    lwm       $s0,$ra,0($sp)   # Restore return address and s0
    jraddiusp 8                # Return

.end    _LIBQ_Q15FromFloat

/*******************************************************************************
 End of File
*/
