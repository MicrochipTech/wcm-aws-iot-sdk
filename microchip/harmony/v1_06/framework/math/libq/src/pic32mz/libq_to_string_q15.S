/*******************************************************************************
  _LIBQ_ToStringQ15:

    void  _LIBQ_ToStringQ15(_Q15 x, char *s);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_to_string_q15.S

  Summary:
    _Q15 to ascii conversion.

  Description:
    Convert a _Q15 fixed point value to an ascii
    string representation in a -N.NNNNNN format.
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

.global _LIBQ_ToStringQ15
.ent    _LIBQ_ToStringQ15

_LIBQ_ToStringQ15:

# void  _LIBQ_ToStringQ15(_Q15 x, char *s);
#
# Input parameters:
#     $a0 - _Q15 fixed point value
#     $a1 - Pointer to character array (size >= 10 chars)
#
# Output parameter:
#     None
#
# Internal register usage:
#     $t0 - _Q16 temporary
#     $t1 - Output string address
#     $t2 - '0'
#     $t3 - temp
#     $t4 - loop counter
#     $t5 - 10
#     $t6 - 0x00078000 mask for char
#     $t7 - temp

# Test value for -1
    li       $t2,0x30
    li       $t3,0x8000
    addu     $t3,$a0,$t3
    bnezc    $t3,NotMinusOne

# Value is minus one, so provide canned response
    li       $t3,0x2D           # '-'
    sb       $t3,0($a1)
    li       $t3,0x31           # '1'
    sb       $t3,1($a1)
    li       $t3,0x2E           # '.'
    sb       $t3,2($a1)
    sb       $t2,3($a1)         # '0'
    sb       $t2,4($a1)         # '0'
    sb       $t2,5($a1)         # '0'
    sb       $t2,6($a1)         # '0'
    sb       $t2,7($a1)         # '0'
    sb       $t2,8($a1)         # '0'
    b        ReturnToStringQ15
    sb       $zero,9($a1)       # 0x00 (delay slot)
NotMinusOne:

# If value is negative, then switch sign
    move     $t1,$a1            # save string address
    bgez     $a0,GEzero
    move     $t0,$a0            # save x (delay slot)
    subu     $t0,$zero,$t0      # Reverse sign
    li       $t3,0x2D           # '-'
    sb       $t3,0($t1)
    addiu    $t1,$t1,1          # increment string address
GEzero:

# Save additional chars
    sb       $t2,0($t1)         # '0'
    addiu    $t1,$t1,1
    li       $t3,0x2E           # '.'
    sb       $t3,0($t1)
    addiu    $t1,$t1,1

# Loop for six chars
    li       $t4,6              # load loop counter
    li       $t5,10             # load constant 10
    lui      $t6,0x07           # load 0x00078000
    ori      $t6,$t6,0x8000
LoopFrac:
    multu    $t0,$t5            # multiply by 10
    mflo     $t0
    and      $t3,$t0,$t6        # mask char
    xor      $t0,$t3,$t0        # mask char from value
    srl      $t3,$t3,15
    ori      $t7,$t3,0x30
    sb       $t7,0($t1)         # store char
    addiu    $t1,$t1,1
    addi     $t4,-1
    bnezc    $t4,LoopFrac       # loop

# Store null termination character
    sb       $zero,0($t1)

# Check for rounding
    srl      $t0,14
    beqzc    $t0,ReturnToStringQ15 # quit if no rounding
LoopRound:
    addiu    $t1,$t1,-1         # Decrement string address
    lb       $t3,0($t1)         # Reload byte
    addiu    $t3,$t3,1          # Increment byte
    addiu    $t0,$t3,-58        # Q: another round?
    movz     $t3,$t2,$t0        # If so, change char to '0'
    sb       $t3,0($t1)         # Restore byte
    beqzc    $t0,LoopRound

# Return
ReturnToStringQ15:
    jr        $ra
    nop

.end    _LIBQ_ToStringQ15

/*******************************************************************************
 End of File
*/
