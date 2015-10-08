/*******************************************************************************
  _LIBQ_ToFloatQ31:

    float  _LIBQ_ToFloatQ31 (_Q31 x);

  Company:
    Microchip Technology Inc.

  File Name:
    libq_to_float_q31.S

  Summary:
    Converts a _Q31 value to a float.

  Description:
    Converts a _Q31 fixed point value to a floating point
    representation.  The floating point value is returned
    by the function.
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

.global _LIBQ_ToFloatQ31
.global __floatsisf
.global __divsf3
.ent    _LIBQ_ToFloatQ31

_LIBQ_ToFloatQ31:

# float  _LIBQ_ToFloatQ31 (_Q31 x);
#
# Input parameters:
#     $a0 - _Q31 input value
#
# Output parameter:
#     $v0 - float output value
#
# Internal register usage:
#     $sp - Increased by 8 to store return address

# Convert integer to float
    addiu     $sp,$sp,-8
    sw        $ra,0($sp)       # Save return address
    jal       __floatsisf      # Convert to float
    nop
    move      $a0,$v0
    jal       __divsf3
    lui       $a1,0x4F00       # Divide by (float)2**31 (delay slot)

# Return
    lw        $ra,0($sp)       # Restore return address
    jraddiusp 8                # Return

.end    _LIBQ_ToFloatQ31

/*******************************************************************************
 End of File
*/
