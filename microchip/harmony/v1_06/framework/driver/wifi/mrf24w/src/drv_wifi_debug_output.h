/*******************************************************************************
  WiFi MAC interface functions

  File Name:
    drv_wifi_debug_output.h

  Summary:
   WiFi-specific MAC function prototypes called by TCP/IP stack.

  Description:
    The functions in this header file are accessed by the TCP/IP stack via
    function pointers.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright © 2012 released Microchip Technology Inc. All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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

#ifndef _DRV_WIFI_DEBUG_OUTPUT_H
#define _DRV_WIFI_DEBUG_OUTPUT_H

//============================================================================
//                                  Function Prototypes
//============================================================================
void OutputDemoHeader(void);
void WF_DisplayModuleAssertInfo(void);
void WF_OutputConnectionDebugMsg(uint8_t event, uint16_t eventInfo);
void WF_OutputConnectionContext(void);
void WF_AssertFunction(int, const char *, const char *, int);

#endif // _DRV_WIFI_DEBUG_OUTPUT_H

// DOM-IGNORE-END
