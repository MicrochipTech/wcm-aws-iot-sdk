/*******************************************************************************
  _LIBQ_Q15FromString:

    _Q15  _LIBQ_Q15FromString(char *s);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q15_from_string.S

  Summary:
    Ascii to _Q15 conversion.

  Description:
    Convert an ascii string into a _Q15 fixed point value.
    The ascii string must be in an -N.NNNNNN format.
    Leading spaces are ignored.  The conversion stops at
    either the first non-conforming character in the
    string or the Null string terminator.  There must be
    no spaces within the string value itself.
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

.global _LIBQ_Q15FromString
.ent    _LIBQ_Q15FromString

_LIBQ_Q15FromString:

# _Q15  _LIBQ_Q15FromString(char *s);
#
# Input parameters:
#     $a0 - Pointer to character array
#
# Output parameter:
#     $v0 - _Q15 fixed point value
#
# Internal register usage:
#     $v0 - Integer value
#     $t0 - Current char from string
#     $t1 - Comparison char
#     $t2 - Divisor value
#     $t3 - Sign
#     $t4 - Loop counter
#     $t5 - 10

# Skip spaces
    li       $v0,0              # Initialize int value to 0
    li       $t1,0x20           # space ' '
SkipSpaces:
    lb       $t0,0($a0)
    bne      $t0,$t1,EndSkipSpaces
    li       $t2,1              # Initialize div val to 1 (delay slot)
    addiu    $a0,$a0,1          # Next char
    beqzc    $zero,SkipSpaces   # no delay slot
EndSkipSpaces:

# Check sign
    li       $t1,0x2D           # '-'
    bne      $t0,$t1,EndCheckSign
    li       $t3,0              # Set positive sign (delay slot)
    li       $t3,1              # Set negative sign
    addiu    $a0,$a0,1          # Next char
    lb       $t0,0($a0)
EndCheckSign:

# If first digit is not zero, then saturate result
    li       $t1,0x30           # '0'
    beq      $t0,$t1,FirstDigZero
    addiu    $a0,$a0,1          # Next address (delay slot)
    li       $v0,0x7FFF         # load max value
    beqzc    $t3,ReturnQ15atoi  # is value negative?
    li       $v0,0xFFFF8000     # Yes, so load min value
    beqzc    $zero,ReturnQ15atoi
FirstDigZero:

# Check for period
    lb       $t0,0($a0)         # Next char
    li       $t1,0x2E           # '.'
    beq      $t0,$t1,SkipPeriod # Skip period
    addiu    $a0,$a0,1          # Next address (delay slot
    b        ReturnQ15atoi      # Return if not period
    li       $v0,0              # Return value 0 (delay slot)
SkipPeriod:

# Loop for six chars
    li       $t4,6              # Load loop counter
    li       $t5,10             # Load constant 10
LoopDigs:
    lb       $t0,0($a0)         # Next char
    sltiu    $t1,$t0,0x30       # End if char < '0'
    bnezc    $t1,EndLoopDigs
    sltiu    $t1,$t0,0x3A       # End if char > '9'
    beqzc    $t1,EndLoopDigs
    andi     $t0,$t0,0xF        # Extract digit value
    sll      $t0,$t0,28         # Shift left for unsigned Q4_28
    multu    $t2,$t5            # Div = Div*10
    mflo     $t2
    divu     $zero,$t0,$t2      # Calculate fraction
    mflo     $t0
    addu     $v0,$v0,$t0        # Accumulate fraction into total
    addi     $t4,$t4,-1         # Decrement loop count
    bgtz     $t4,LoopDigs
    addiu    $a0,$a0,1          # Next address (delay slot)
EndLoopDigs:

# Calculate the fixed point value
    srl      $v0,12
    addiu    $v0,$v0,1
    srl      $v0,$v0,1
    beqzc    $t3,ReturnQ15atoi
    subu     $v0,$zero,$v0

# Return
ReturnQ15atoi:
    jr        $ra
    nop

.end    _LIBQ_Q15FromString

/*******************************************************************************
 End of File
*/
