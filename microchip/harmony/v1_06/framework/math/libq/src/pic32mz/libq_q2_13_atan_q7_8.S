/*******************************************************************************
  _LIBQ_Q2_13_atan_Q7_8:

      _Q2_13  _LIBQ_Q2_13_atan_Q7_8 (_Q7_8 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_q2_13_atan_q7_8.S

  Summary:
    Calculates the value of atan(x).

  Description:
    Calculates the atan(x), where x is of type _Q7_8 and
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

.global _LIBQ_Q2_13_atan_Q7_8
.global _LIBQ_Q2_13_atan2_Q7_8
.ent    _LIBQ_Q2_13_atan_Q7_8

_LIBQ_Q2_13_atan_Q7_8:

#  _Q2_13  _LIBQ_Q2_13_atan_Q7_8 (_Q7_8 x);
#
# Input parameters:
#     $a0 - _Q7_8 input value y
#
# Output parameter:
#     $v0 - _Q2_13 output value
#
# Internal register usage:
#     $a1 - One

#     $sp - Increased by 8 to store return address
#     $sp+0  - $ra

# Push stack
    addiu     $sp,$sp,-8
    sw        $ra,0($sp)   # Save return address

# Call atan2(x, 1)
    jal       _LIBQ_Q2_13_atan2_Q7_8
    li        $a1,0x0100           # delay slot

# Return
ReturnQ213:
    lw        $ra,0($sp)           # Restore return address
    jraddiusp 8                    # Return

.end    _LIBQ_Q2_13_atan_Q7_8

/*******************************************************************************
 End of File
*/
