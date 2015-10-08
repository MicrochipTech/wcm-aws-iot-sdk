/*******************************************************************************
  LLDP Module API Header File

  Company:
    Microchip Technology Inc.
    
  File Name:
    lldp.h

  Summary:
    The LLDP module implements the Link Local Discovery Protocol.

  Description:
    The LLDP module implements the Link Local Discovery Protocol.
********************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END

#ifndef __LLDP_H
#define __LLDP_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// LLDP Module Configuration
typedef struct
{
} TCPIP_LLDP_MODULE_CONFIG;

void TCPIP_LLDP_RxEnable(void);
void TCPIP_LLDP_TxEnable(void);
void TCPIP_LLDP_RxTxEnable(void);
void TCPIP_LLDP_PortDisable(void);

void TCPIP_LLDP_FastTxCounterSet(uint8_t n);

void TCPIP_LLDP_MacDestAddressSet(TCPIP_MAC_ADDR* pAddr);

bool TCPIP_LLDP_PoePowerIsEnabled(void);

// *****************************************************************************
/*
  Function:
    void  TCPIP_LLDP_Task(void)

  Summary:
    Standard TCP/IP stack module task function.

  Description:
    Performs LLDP module tasks in the TCP/IP stack.

  Precondition:
    LLDP module should have been initialized

  Parameters:
    None.

  Returns:
    None.

  Example:
    None

  Remarks:
    None.
*/
void  TCPIP_LLDP_Task(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // __LLDP_H

