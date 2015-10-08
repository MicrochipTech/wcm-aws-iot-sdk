/*******************************************************************************
  _LIBQ_Q7_8_tan_Q2_13:

      _Q7_8  _LIBQ_Q7_8_tan_Q2_13 (_Q2_13 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q7_8_tan_q2_13.S

  Summary:
    Calculates the value of tan(x).

  Description:
    Calculates the tan(x), where x is of type _Q2_13 and
    the resulting value is of type _Q7_8.
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

.global _LIBQ_Q7_8_tan_Q2_13
.global _LIBQ_Q31_sin_Q2_29
.global _LIBQ_Q31_cos_Q2_29
.ent    _LIBQ_Q7_8_tan_Q2_13

_LIBQ_Q7_8_tan_Q2_13:

# _Q7_8  _LIBQ_Q7_8_tan_Q2_13 (_Q2_13 x);
#
# Input parameters:
#     $a0 - _Q2_13 input value
#
# Output parameter:
#     $v0 - _Q7_8 output value
#
# Register usage:
#     $t0 - temp
#     $t1 - temp
#     $t2 - temp
#     $t3 - temp
#     $t4 - sin accum hi
#     $t5 - sin accum lo
#     $t6 - cos accum hi
#     $t7 - cos accum lo
#     $t8 - temp
#     $t9 - cTable
#     $ac0 - temp
#     $ac1 - cos accum
#     $ac2 - sin accum

# Check for lookup table at upper boundary
    slti      $t0,$a0,0x31f4
    bnezc     $t0,NoLUT1
    slti      $t0,$a0,0x3204
    beqzc     $t0,NoLUT1
    la        $t0,tanLUT              # Return value from LUT
    addiu     $t1,$a0,0xce0c
    sll       $t1,$t1,1
    b         ReturnQ78
    lhx       $v0,$t0($t1)            # delay slot
NoLUT1:

# Check for lookup table at lower boundary
    slti      $t0,$a0,0xce0d
    beqzc     $t0,NoLUT2
    slti      $t0,$a0,0xcdfd
    bnezc     $t0,NoLUT2
    la        $t0,tanLUT              # Return value from LUT
    lui       $t1,0xffff
    ori       $t1,$t1,0xce0c
    subu      $t1,$t1,$a0
    sll       $t1,$t1,1
    lhx       $v0,$t0($t1)
    b         ReturnQ78
    subu      $v0,$zero,$v0           # return -value (delay slot)
NoLUT2:

# Check for special case
    li        $t0,0x3284
    subu      $t0,$t0,$a0
    bnezc     $t0,NotSpecial
    lui       $v0,0xffff             # return special value
    b         ReturnQ78
    ori       $v0,$v0,0x8013
NotSpecial:

# Convert to _Q2_29
    la        $t9,cTable
    sll       $a0,$a0,16
    lw        $t2,0($t9)             #  PI Q2_29
    subu      $t3,$zero,$t2          # -PI Q2_29

# If angle > PI then subtract 2*PI from angle
    slt       $t0,$t2,$a0
    beqzc     $t0,AngleLEpi
    subu      $a0,$a0,$t2            # angle -= 2*PI
    b         CheckPiDiv2
    subu      $a0,$a0,$t2            # delay slot
AngleLEpi:

# If angle < -PI then add 2*PI to angle
    slt       $t0,$a0,$t3
    beqzc     $t0,CheckPiDiv2
    addu      $a0,$a0,$t2            # angle += 2*PI
    addu      $a0,$a0,$t2
CheckPiDiv2:

# If angle > PI/2 then angle = angle - PI
    srl       $t8,$t2,1
    slt       $t0,$t8,$a0
    beqzc     $t0,AngleLEpi2
    b         AngleOK
    subu      $a0,$a0,$t2            # angle = angle - PI (delay slot)
AngleLEpi2:

# If angle < -PI/2 then angle = -PI - angle
    lw        $t0,4($t9)             # -PI/2
    slt       $t0,$a0,$t0
    beqzc     $t0,AngleOK
    addu      $a0,$t2,$a0            # angle += PI
AngleOK:

# Check tan boundary conditions
    lw        $t0,8($t9)             # upper bound to saturate
    slt       $t0,$t0,$a0
    beqzc     $t0,CheckSatLo
    b         ReturnQ78              # saturate hi
    li        $v0,0x7fff             # delay slot
CheckSatLo:
    lw        $t0,0xc($t9)           # lower bound to saturate
    slt       $t0,$a0,$t0
    beqzc     $t0,NotSatLo
    lui       $v0,0xffff             # saturate lo
    b         ReturnQ78
    ori       $v0,0x8000             # delay slot
NotSatLo:

# Calculate cos angle (sin angle + pi/2)
    slt       $t0,$zero,$a0          # adjust for tan range
    bnez      $t0,GotCosAngle
    subu      $a1,$t8,$a0            # delay slot
    addu      $a1,$t8,$a0
GotCosAngle:

# Initialize constants
    li        $t1,1                  # constant 1

# sin x^11
    lw        $t5,0x18($t9)          # sin
    lw        $t4,0x14($t9)          # sin
    srl       $t0,$t5,1              # sin
    mulq_s.w  $t0,$t0,$a0            # sin
    mult      $ac2,$t4,$a0           # sin
    madd      $ac2,$t0,$t1           # sin
    lw        $t7,0x18($t9)            # cos
    lw        $t6,0x14($t9)            # cos
    srl       $t0,$t7,1                # cos
    shilo     $ac2,-3                # sin
    mulq_s.w  $t0,$t0,$a1              # cos
    mfhi      $t4,$ac2               # sin
    mflo      $t5,$ac2               # sin

# cos x^11
    mult      $ac1,$t6,$a1             # cos
    madd      $ac1,$t0,$t1             # cos
    srl       $t0,$t5,1              # sin
    mulq_s.w  $t0,$t0,$a0            # sin
    mult      $ac2,$t4,$a0           # sin
    shilo     $ac1,-3                  # cos
    madd      $ac2,$t0,$t1           # sin
    mfhi      $t6,$ac1                 # cos
    mflo      $t7,$ac1                 # cos

# sin x^10
    shilo     $ac2,-3                # sin
    srl       $t0,$t7,1                # cos
    mfhi      $t4,$ac2               # sin
    mflo      $t5,$ac2               # sin

# cos x^10
    mulq_s.w  $t0,$t0,$a1              # cos
    mult      $ac1,$t6,$a1             # cos
    madd      $ac1,$t0,$t1             # cos
    lw        $t3,0x1c($t9)          # sin
    addsc     $t5,$t5,$t3            # sin
    li        $t2,0x171D             # sin
    shilo     $ac1,-3                  # cos
    addwc     $t4,$t4,$t2            # sin
    mfhi      $t6,$ac1                 # cos
    mflo      $t7,$ac1                 # cos

# sin x^9
    srl       $t0,$t5,1              # sin
    mulq_s.w  $t0,$t0,$a0            # sin
    mult      $ac2,$t4,$a0           # sin
    madd      $ac2,$t0,$t1           # sin
    addsc     $t7,$t7,$t3              # cos
    addwc     $t6,$t6,$t2              # cos
    srl       $t0,$t7,1                # cos
    shilo     $ac2,-3                # sin
    mulq_s.w  $t0,$t0,$a1              # cos
    mfhi      $t4,$ac2               # sin
    mflo      $t5,$ac2               # sin

# cos x^9
    mult      $ac1,$t6,$a1             # cos
    madd      $ac1,$t0,$t1             # cos
    srl       $t0,$t5,1              # sin
    mulq_s.w  $t0,$t0,$a0            # sin
    mult      $ac2,$t4,$a0           # sin
    shilo     $ac1,-3                  # cos
    madd      $ac2,$t0,$t1           # sin
    mfhi      $t6,$ac1                 # cos
    mflo      $t7,$ac1                 # cos

# sin x^8
    shilo     $ac2,-3                # sin
    srl       $t0,$t7,1                # cos
    mfhi      $t4,$ac2               # sin
    mflo      $t5,$ac2               # sin

# cos x^8
    mulq_s.w  $t0,$t0,$a1              # cos
    mult      $ac1,$t6,$a1             # cos
    madd      $ac1,$t0,$t1             # cos
    lw        $t3,0x24($t9)          # sin
    addsc     $t5,$t5,$t3            # sin
    lw        $t2,0x20($t9)          # sin
    shilo     $ac1,-3                  # cos
    addwc     $t4,$t4,$t2            # sin
    mfhi      $t6,$ac1                 # cos
    mflo      $t7,$ac1                 # cos

# sin x^7
    srl       $t0,$t5,1              # sin
    mulq_s.w  $t0,$t0,$a0            # sin
    mult      $ac2,$t4,$a0           # sin
    madd      $ac2,$t0,$t1           # sin
    addsc     $t7,$t7,$t3              # cos
    addwc     $t6,$t6,$t2              # cos
    srl       $t0,$t7,1                # cos
    shilo     $ac2,-3                # sin
    mulq_s.w  $t0,$t0,$a1              # cos
    mfhi      $t4,$ac2               # sin
    mflo      $t5,$ac2               # sin

# cos x^7
    mult      $ac1,$t6,$a1             # cos
    madd      $ac1,$t0,$t1             # cos
    srl       $t0,$t5,1              # sin
    mulq_s.w  $t0,$t0,$a0            # sin
    mult      $ac2,$t4,$a0           # sin
    shilo     $ac1,-3                  # cos
    madd      $ac2,$t0,$t1           # sin
    mfhi      $t6,$ac1                 # cos
    mflo      $t7,$ac1                 # cos

# sin x^6
    shilo     $ac2,-3                # sin
    srl       $t0,$t7,1                # cos
    mfhi      $t4,$ac2               # sin
    mflo      $t5,$ac2               # sin

# cos x^6
    mulq_s.w  $t0,$t0,$a1              # cos
    mult      $ac1,$t6,$a1             # cos
    madd      $ac1,$t0,$t1             # cos
    lw        $t3,0x2c($t9)          # sin
    addsc     $t5,$t5,$t3            # sin
    lw        $t2,0x28($t9)          # sin
    shilo     $ac1,-3                  # cos
    addwc     $t4,$t4,$t2            # sin
    mfhi      $t6,$ac1                 # cos
    mflo      $t7,$ac1                 # cos

# sin x^5
    srl       $t0,$t5,1              # sin
    mulq_s.w  $t0,$t0,$a0            # sin
    mult      $ac2,$t4,$a0           # sin
    madd      $ac2,$t0,$t1           # sin
    addsc     $t7,$t7,$t3              # cos
    addwc     $t6,$t6,$t2              # cos
    srl       $t0,$t7,1                # cos
    shilo     $ac2,-3                # sin
    mulq_s.w  $t0,$t0,$a1              # cos
    mfhi      $t4,$ac2               # sin
    mflo      $t5,$ac2               # sin

# cos x^5
    mult      $ac1,$t6,$a1             # cos
    madd      $ac1,$t0,$t1             # cos
    srl       $t0,$t5,1             # sin
    mulq_s.w  $t0,$t0,$a0           # sin
    mult      $ac2,$t4,$a0          # sin
    shilo     $ac1,-3                  # cos
    madd      $ac2,$t0,$t1          # sin
    mfhi      $t6,$ac1                 # cos
    mflo      $t7,$ac1                 # cos

# sin x^4
    shilo     $ac2,-3               # sin
    srl       $t0,$t7,1                # cos
    mfhi      $t4,$ac2              # sin
    mflo      $t5,$ac2              # sin

# cos x^4
    mulq_s.w  $t0,$t0,$a1              # cos
    mult      $ac1,$t6,$a1             # cos
    madd      $ac1,$t0,$t1             # cos
    lw        $t3,0x34($t9)         # sin
    addsc     $t5,$t5,$t3           # sin
    lw        $t2,0x30($t9)         # sin
    shilo     $ac1,-3                  # cos
    addwc     $t4,$t4,$t2           # sin
    mfhi      $t6,$ac1                 # cos
    mflo      $t7,$ac1                 # cos

# sin x^3
    srl       $t0,$t5,1             # sin
    mulq_s.w  $t0,$t0,$a0           # sin
    mult      $ac2,$t4,$a0          # sin
    madd      $ac2,$t0,$t1          # sin
    addsc     $t7,$t7,$t3             # cos
    addwc     $t6,$t6,$t2             # cos
    srl       $t0,$t7,1               # cos
    shilo     $ac2,-3               # sin
    mulq_s.w  $t0,$t0,$a1             # cos
    mfhi      $t4,$ac2              # sin
    mflo      $t5,$ac2              # sin

# cos x^3
    mult      $ac1,$t6,$a1            # cos
    madd      $ac1,$t0,$t1            # cos
    srl       $t0,$t5,1             # sin
    mulq_s.w  $t0,$t0,$a0           # sin
    mult      $ac2,$t4,$a0          # sin
    shilo     $ac1,-3                 # cos
    madd      $ac2,$t0,$t1          # sin
    mfhi      $t6,$ac1                # cos
    mflo      $t7,$ac1                # cos

# sin x^2
    shilo     $ac2,-3               # sin
    srl       $t0,$t7,1               # cos
    mfhi      $t4,$ac2              # sin
    mflo      $t5,$ac2              # sin

# cos x^2
    mulq_s.w  $t0,$t0,$a1             # cos
    mult      $ac1,$t6,$a1            # cos
    madd      $ac1,$t0,$t1            # cos
    lui       $t2,0x8000            # sin
    subu      $t4,$t4,$t2           # sin
    srl       $t0,$t5,1             # sin
    shilo     $ac1,-3                 # cos
    mulq_s.w  $t0,$t0,$a0           # sin
    mfhi      $t6,$ac1                # cos
    mflo      $t7,$ac1                # cos

# sin x^1
    mult      $ac2,$t4,$a0          # sin
    madd      $ac2,$t0,$t1          # sin

# cos x^1
    subu      $t6,$t6,$t2             # cos
    srl       $t0,$t7,1               # cos
    mulq_s.w  $t0,$t0,$a1             # cos
    mult      $ac1,$t6,$a1            # cos
    madd      $ac1,$t0,$t1            # cos
    extr_rs.w $v1,$ac1,29             # cos
    extr_rs.w $v0,$ac2,29           # sin

# Divide sin/cos
    addiu     $v1,$v1,0x200    # Round cos
    sra       $v1,$v1,10       # Bias by 2^10 for Q7.10 result
# Check for divide by 0
    bnezc     $v1,DivideSinCos
    lui       $v0,0xFFFF       # Saturate to -128
    b         ReturnQ78
    ori       $v0,0x8000       # delay slot
DivideSinCos:
    div       $zero,$v0,$v1
    mflo      $v0
    addi      $v0,$v0,2        # round result to Q7.8
    sra       $v0,$v0,2        # Convert to Q7.8

# Return
ReturnQ78:
    jr        $ra
    nop

    .align 2
cTable:
    .word  /*0*/      0x6487ed51             # PI Q2_29
    .word  /*4*/      0xcdbc0958             # -PI/2 Q2_29
    .word  /*8*/      0x3203f6fd             # tan upper boundary
    .word  /*0xc*/    0xcdfd0903             # tan lower boundary
    .word  /*0x10*/   0x58462e33             # sin x13 const
    .word  /*0x14*/   0xffffffca             # sin x11 const
    .word  /*0x18*/   0x3377e419
    .word  /*0x1c*/   0xE3A3E90E             # sin x9 const
    .word  /*0x20*/   0xfff97f97             # sin x7 const
    .word  /*0x24*/   0xf98005a9
    .word  /*0x28*/   0x01111111             # sin x5 const
    .word  /*0x2c*/   0x1110FFA7
    .word  /*0x30*/   0xeaaaaaaa             # sin x3 const
    .word  /*0x34*/   0xaaaa9ea8

tanLUT:                                      # lookup table
    .hword /*0x31f4*/ 0x6672
    .hword /*0x31f5*/ 0x67BD
    .hword /*0x31f6*/ 0x6913
    .hword /*0x31f7*/ 0x6A70
    .hword /*0x31f8*/ 0x6BD7
    .hword /*0x31f9*/ 0x6D47
    .hword /*0x31fa*/ 0x6EC1
    .hword /*0x31fb*/ 0x7046
    .hword /*0x31fc*/ 0x71D5
    .hword /*0x31fd*/ 0x7370
    .hword /*0x31fe*/ 0x7516
    .hword /*0x31ff*/ 0x76C9
    .hword /*0x3200*/ 0x7888
    .hword /*0x3201*/ 0x7A55
    .hword /*0x3202*/ 0x7C30
    .hword /*0x3203*/ 0x7E19

.end    _LIBQ_Q7_8_tan_Q2_13

/*******************************************************************************
 End of File
*/
