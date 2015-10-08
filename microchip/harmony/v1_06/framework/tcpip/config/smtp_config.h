/*******************************************************************************
  Simple Mail Transfer Protocol (SMTP) Configuration file

  Company:
    Microchip Technology Inc.
    
  File Name:
    smtp_config.h

  Summary:
    SMTP configuration file

  Description:
    This file contains the SMTP module configuration options

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright � 2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND,
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
#ifndef _SMTPZ_CONFIG_H_
#define _SMTP_CONFIG_H_


// How long to wait before assuming the connection
// has been dropped (default 8 seconds)
#define TCPIP_SMTP_SERVER_REPLY_TIMEOUT     (8)

// SMTP task rate, milliseconds
#define TCPIP_SMTP_TASK_TICK_RATE           (55)

#endif  // _SMTP_CONFIG_H_



