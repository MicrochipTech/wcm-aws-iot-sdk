/*******************************************************************************
  _LIBQ_Q2_29_acos_Q31:

    _Q2_29  _LIBQ_Q2_29_acos_Q31 (_Q31 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q2_29_acos_q31.S

  Summary:
    Calculates the value of acos(x).

  Description:
    Calculates the acos(x), where x is of type _Q31 and
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
.set	noreorder
.set	nomacro

.global _LIBQ_Q2_29_asin_Q31_Fast
.global _LIBQ_Q2_29_acos_Q31_Fast
.ent	_LIBQ_Q2_29_acos_Q31_Fast

_LIBQ_Q2_29_acos_Q31_Fast:

#  _Q2_29  _LIBQ_Q2_29_acos_Q31_Fast (_Q31 x);
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

#     $sp - Increased by 16 to store return address
#     $sp+0  - ra

# Push stack
    addiu     $sp,$sp,-8
    sw        $ra,0($sp)     # Save return address

# Get the acos value
    jal       _LIBQ_Q2_29_asin_Q31_Fast # Get asin value
    nop
    lui       $t0,0x3243                # result = pi/2 - asin(x)
    ori       $t0,0xf6a9
    subu      $v0,$t0,$v0

# Return
ReturnQ229:
    lw        $ra,0($sp)              # Restore return address, s0, s1, s2
    jraddiusp 8                       # Return

.end	_LIBQ_Q2_29_acos_Q31_Fast

/*******************************************************************************
 End of File
*/
