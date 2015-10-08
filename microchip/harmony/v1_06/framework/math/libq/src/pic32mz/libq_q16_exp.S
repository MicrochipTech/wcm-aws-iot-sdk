/*******************************************************************************
  _LIBQ_Q16Exp:

     _Q16  _LIBQ_Q16Exp(_Q16 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q16_exp.S

  Summary:
    Calculates the exponential function e^x.

  Description:
    Calculates the exponential function e^x.  The calculation
    will saturate if the resulting value is outside the range of the
    _Q16 representation.  For x > 10.3972015380859375, the resulting
    value will be saturated to 0x7fffffff.  For x < -10.3972015380859375
    the resulting value will be saturated to 0.
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

.global _LIBQ_Q16Exp
.global _LIBQ_Q16Div
.ent    _LIBQ_Q16Exp

_LIBQ_Q16Exp:

# _Q16  _LIBQ_Q16Exp (_Q16 x);
#
# Input parameters:
#     $a0 - _Q16 input value
#
# Output parameter:
#     $v0 - _Q16 output value
#
# Internal register usage:
#     $t0 - temp
#     $t1 - temp
#     $t2 - temp
#     $t3 - temp
#     $t4 - boolean (input is negative)
#     $t6 - exp(fractional) multiplier
#     $t7 - exp(integer) multiplier
#     $ac0 - temp
#     $ac1 - temp

#     $sp - Increased by 8 to store return address
#     $sp+0  - ra

# Push stack
    addiu     $sp,$sp,-8
    sw        $ra,0($sp)

# If input == 0 return one
    lui       $v0,0x0001
    beqzc     $a0,ReturnQ16Exp

# If input == one return e
    subu      $t0,$a0,$v0      # comparison
    bnezc     $t0,NotOne
    lui       $v0,0x0002       # return e
    b         ReturnQ16Exp
    ori       $v0,0xB7E1       # delay slot
NotOne:

# If input > 681391 return saturated maximum
    lui       $t0,0x000a       # 681391
    ori       $t0,0x65AF
    slt       $t1,$t0,$a0
    beqzc     $t1,LessThanMax
    lui       $v0,0x7fff       # return maximum
    b         ReturnQ16Exp
    ori       $v0,0xffff       # delay slot
LessThanMax:

# If input < -681391 return 0
    subu      $t0,$zero,$t0    # -681391
    slt       $t1,$a0,$t0
    bnez      $t1,ReturnQ16Exp
    move      $v0,$zero        # delay slot

# Determine if input is negative
    slt       $t4,$a0,$zero

# If input is negative, reverse sign
    beqzc     $t4,InputPositive
    subu      $a0,$zero,$a0    # input = - input
InputPositive:

# Separate integer and fractional portion
    sra       $t7,$a0,16
    sll       $t0,$t7,16
    subu      $a0,$a0,$t0
    sll       $a0,$a0,15

# Look up integer multiplier
    la        $t0,ExpIntLUT
    sll       $t1,$t7,2
    lwx       $t7,$t0($t1)

# Look up fractional multiplier
    la        $t0,ExpFracLUT
    lwx       $t6,$t0($t1)

# Calculate exp(fraction)
# exp(x) = x^2/2!
    mulq_rs.w $t1,$a0,$a0
    lui       $t0,0x4000
    mult      $ac1,$t0,$t1

# exp(x) += x^3/3!
    mulq_rs.w $t1,$t1,$a0
    lui       $t0,0x1555
    ori       $t0,$t0,0x5555
    madd      $ac1,$t0,$t1

# exp(x) += x^4/4!
    mulq_rs.w $t1,$t1,$a0
    lui       $t0,0x0555
    ori       $t0,$t0,0x5555
    madd      $ac1,$t0,$t1

# exp(x) += x^5/5!
    mulq_rs.w $t1,$t1,$a0
    lui       $t0,0x0111
    ori       $t0,$t0,0x1111
    madd      $ac1,$t0,$t1

# exp(x) += x^6/6!
    mulq_rs.w $t1,$t1,$a0
    lui       $t0,0x002D
    ori       $t0,$t0,0x82D8
    madd      $ac1,$t0,$t1

# exp(x) += x^7/7!
    mulq_rs.w $t1,$t1,$a0
    lui       $t0,0x0006
    ori       $t0,$t0,0x8068
    madd      $ac1,$t0,$t1

# exp(x) += x^8/8!
    mulq_rs.w $t1,$t1,$a0
    ori       $t0,$zero,0xD00D
    madd      $ac1,$t0,$t1

# exp(x) += x^9/9!
    mulq_rs.w $t1,$t1,$a0
    li        $t0,0x171e
    madd      $ac1,$t0,$t1

# exp(x) += x^10/10!
    mulq_rs.w $t1,$t1,$a0
    li        $t0,0x0250
    madd      $ac1,$t0,$t1

# exp(x) += x^11/11!
    mulq_rs.w $t1,$t1,$a0
    li        $t0,0x0036
    madd      $ac1,$t0,$t1

# exp(x) += x^12/12!
    mulq_rs.w $t1,$t1,$a0
    li        $t0,0x0004
    madd      $ac1,$t0,$t1

# Extract result
    extr_rs.w $v0,$ac1,31
    shra_r.w  $v0,$v0,2

# exp(x) += 1 + x
    sra       $a0,$a0,2
    addu      $v0,$v0,$a0
    lui       $t0,0x2000
    addu      $v0,$v0,$t0

# multiply by exp(integer) & exp(fraction)
    mult      $ac1,$v0,$t7
    mulq_rs.w $t0,$v0,$t6
    mfhi      $t3,$ac1
    mflo      $t2,$ac1
    addsc     $t0,$t0,$t2
    addwc     $t1,$zero,$t3
    mthi      $t1
    mtlo      $t0
    extr_rs.w $v0,$ac0,29

# If input value was negative then calculate inverse
    beqzc     $t4,ReturnQ16Exp
    lui       $a0,0x0001
    jal       _LIBQ_Q16Div
    move      $a1,$v0           # delay slot

# Return
ReturnQ16Exp:
    lw        $ra,0($sp)
    jraddiusp 8                 # Return

    .align 2
ExpIntLUT:
    .word 0x00010000
    .word 0x0002B7E1
    .word 0x00076399
    .word 0x001415E5
    .word 0x00369920
    .word 0x009469C4
    .word 0x01936DC5
    .word 0x0448A216
    .word 0x0BA4F53E
    .word 0x1FA7157C
    .word 0x560A773E

ExpFracLUT:
    .word 0x00000000
    .word 0x28B14577
    .word 0x171A9BB5
    .word 0x5FB7D887
    .word 0x2E273A52
    .word 0x65C0CE74
    .word 0x34860468
    .word 0x55DBB613
    .word 0x51C31C6E
    .word 0x2387BEAD
    .word 0x2A0AB629

.end    _LIBQ_Q16Exp

/*******************************************************************************
 End of File
*/
