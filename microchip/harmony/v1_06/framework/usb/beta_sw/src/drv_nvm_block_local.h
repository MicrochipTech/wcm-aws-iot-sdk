/*******************************************************************************
  NVM Block Driver Interface Definition Local Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_nvm_block_local.h

  Summary:
    NVM Block Driver Interface Definition Local Header File

  Description:
    The NVM Block Driver provides a block interface to access the NVM on the
    PIC32 microcontroller.  This file contains local typedefs and data structure
    definitions.
******************************************************************************/

//DOM-IGNORE-BEGIN
/******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

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
******************************************************************************/
//DOM-IGNORE-END
#ifndef _DRV_NVM_BLOCK_LOCAL_H_
#define _DRV_NVM_BLOCK_LOCAL_H_

#include "../drv_nvm_block.h"

/**********************************************
 * NVM Block Driver Client Object.
 *********************************************/
typedef struct
{
    /* Keep track of the number of client.
     * Needed for exclusive checking */

    int nClients;

    /* Set to true if the driver was opened
     * exclusively */
    bool wasOpenedExclusively;

} DRV_NVM_BLOCK_OBJ;



/**********************************************
 * NVM Block Driver Client Object.
 *********************************************/

typedef struct
{
    /* Indicates the client object is valid */
    bool inUse;

    /* Event handler callback function */
    DRV_NVM_BLOCK_EVENT_HANDLER eventHandler;

    /* Native NVM driver handle */
    DRV_HANDLE nvmDriverHandle;

    /* Client specific context */
    uintptr_t context;

    /* IO Intent */
    DRV_IO_INTENT ioIntent;

    /* Driver to which this client belongs */
    DRV_NVM_BLOCK_OBJ * hDriver;

    /* Block start address for this client */
    uint32_t blockStartAddress;

} DRV_NVM_BLOCK_CLIENT_OBJ;


#endif

