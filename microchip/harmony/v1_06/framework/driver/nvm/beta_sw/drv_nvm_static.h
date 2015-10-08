/*******************************************************************************
  NVM Driver Interface Declarations for static single instance driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_nvm_static_single.h

  Summary:
    NVM Driver Interface Declarations for static single instance driver

  Description:
    The NVM device driver provides a simple interface to manage the NVM
    modules on Microchip microcontrollers.  This file defines the interface
    Declarations for the NVM driver.
    
  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.
    
    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
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
*******************************************************************************/
//DOM-IGNORE-END

#ifndef _DRV_NVM_STATIC_SINGLE_H
#define _DRV_NVM_STATIC_SINGLE_H


// *****************************************************************************
// *****************************************************************************
// Section: Interface Headers for Instance 0 for the static single open driver
// *****************************************************************************
// *****************************************************************************

void DRV_NVM0_Initialize( const SYS_MODULE_INIT * const init);

void DRV_NVM0_Reinitialize( const SYS_MODULE_INIT * const init);

void DRV_NVM0_Deinitialize( void );

SYS_STATUS DRV_NVM0_Status( void );

void DRV_NVM0_TasksRead ( void );

void DRV_NVM0_TasksWrite ( void );

void DRV_NVM0_TasksErase ( void );

void DRV_NVM0_Open( const DRV_IO_INTENT intent );

void DRV_NVM0_Close( void );

DRV_NVM_CLIENT_STATUS DRV_NVM0_ClientStatus( void );

DRV_NVM_OPERATION_STATUS DRV_NVM0_ReadBlock ( uint8_t *targetBuffer, uint8_t *srcAddress,  const unsigned int numbytes );

DRV_NVM_OPERATION_STATUS DRV_NVM0_ReadRow( uint8_t *targetBuffer, uint8_t *srcAddress);

DRV_NVM_OPERATION_STATUS DRV_NVM0_WriteBlock( const uint8_t *buffer, const uint32_t*    NVMAddr,  const uint32_t numbytes );

DRV_NVM_OPERATION_STATUS DRV_NVM0_WriteWord( const uint8_t *buffer, const uint32_t*    NVMAddr);

DRV_NVM_OPERATION_STATUS DRV_NVM0_EraseBlock( const uint32_t*    NVMAddr,  const uint32_t numbytes );

DRV_NVM_OPERATION_STATUS DRV_NVM0_EraseWord( const uint32_t*    NVMAddr);

DRV_NVM_TRANSFER_STATUS DRV_NVM0_TransferStatus ();

DRV_NVM_BUFFER_TRANSFER_STATUS DRV_NVM0_BufferTransferStatus(const int8_t bufferHandle);

#endif // #ifndef _DRV_NVM_STATIC_SINGLE_H

/*******************************************************************************
 End of File
*/
