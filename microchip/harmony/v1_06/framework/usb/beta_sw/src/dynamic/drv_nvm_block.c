/*******************************************************************************
  NVM Block Driver Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    drv_nvm_block.h

  Summary:
    NVM Block Driver Implementation

  Description:
    The NVM Block Driver provides a block interface to access the NVM on the
    PIC32 microcontroller.  This file defines the Implementation of the
    NVM Block Driver.
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

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "usb/src/drv_nvm_block_local.h"
#include <p32xxxx.h>

// *****************************************************************************
// *****************************************************************************
// Section: Local variables and data structures and constants
// *****************************************************************************
// *****************************************************************************

#define GEOMETRY_TABLE_READ_ENTRY 0
#define GEOMETRY_TABLE_WRITE_ENTRY 1
#define GEOMETRY_TABLE_ERASE_ENTRY 2

/***************************************
 * NVM Block driver Client Objects
 **************************************/
DRV_NVM_BLOCK_CLIENT_OBJ gDrvNVMBlockClientObj[DRV_NVM_CLIENTS_NUMBER];

/**************************************
 * NVM Block driver hardware instance
 * objects.
 *************************************/
DRV_NVM_BLOCK_OBJ gDrvNVMBlockObj[DRV_NVM_INSTANCES_NUMBER];

/*************************************
 * NVM Block driver geometry object
 ************************************/
SYS_FS_MEDIA_GEOMETRY gDrvNVMMediaGeometry = 
{

    /* Does not support OTP, is byte write capable and is can be written to */
    .mediaProperty = SYS_FS_MEDIA_WRITE_IS_BLOCKING,

    /* Number of read, write and erase entries in the table */
    .numReadRegions = 1,
    .numWriteRegions = 1,
    .numEraseRegions = 1,

    /* Geometry table will be updated in the DRV_NVM_GeometryGet() function */
    .geometryTable = NULL
};

/*****************************************
 * Media Geomtery Table.
 ****************************************/
SYS_FS_MEDIA_REGION_GEOMETRY gNVMGeometryTable[3] = 
{
    {
        /* Read Region Geometry */
        .blockSize = 1,
        .numBlocks = (DRV_NVM_BLOCK_MEMORY_SIZE * 1024),
    },
    {
        /* Write Region Geometry */
       .blockSize = DRV_NVM_ROW_SIZE,
       .numBlocks = ((DRV_NVM_BLOCK_MEMORY_SIZE * 1024)/DRV_NVM_ROW_SIZE)
    },
    {
        /* Erase Region Geometry */
       .blockSize = DRV_NVM_PAGE_SIZE,
       .numBlocks = ((DRV_NVM_BLOCK_MEMORY_SIZE * 1024)/DRV_NVM_PAGE_SIZE)
    }
};

// *****************************************************************************
// *****************************************************************************
// Section: NVM Block Driver Client Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void _DRV_NVM_BLOCK_NVMEventHandler
    ( 
        DRV_NVM_EVENT event
        DRV_NVM_BUFFER_HANDLE handle
        uintptr_t context
    );

  Summary:
    Event handler for NVM Driver events.

  Description:
    Event handler for NVM Driver events.

  Remarks:
    This is a local function and should not be called directly by the 
    application.
*/

void _DRV_NVM_BLOCK_NVMEventHandler
(
    DRV_NVM_EVENT event,
    DRV_NVM_BUFFER_HANDLE bufferHandle,
    uintptr_t context
)
{
    /* This function is called when a NVM Driver event
     * occurs. The NVM Block driver would have set
     * this function as the event handler. The context
     * parameter is the NVM Block driver client 
     * handle */

    DRV_NVM_BLOCK_CLIENT_OBJ * client = (DRV_NVM_BLOCK_CLIENT_OBJ *)context;

    if(client->inUse && (client->eventHandler != NULL))
    {
        switch(event)
        {
            case DRV_NVM_EVENT_BUFFER_COMPLETE:
                client->eventHandler(DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE, bufferHandle, client->context);
                break;
            case DRV_NVM_EVENT_BUFFER_ERROR:
                client->eventHandler(DRV_NVM_EVENT_BLOCK_COMMAND_ERROR, bufferHandle, client->context);
                break;
            default:
                break;
        }
    } 
}

// *****************************************************************************
/* Function:
    DRV_NVM_BLOCK_CLIENT * _DRV_NVM_BLOCK_ClientHandleValidate( DRV_HANDLE handle );

  Summary:
    Returns NULL if the client handle is not valid

  Description:
    This function returns NULL if the client handle is invalid else return the
    client object associated with the handle.

  Remarks:
    This is a local function and should not be called directly by the 
    application.
*/

DRV_NVM_BLOCK_CLIENT_OBJ * _DRV_NVM_BLOCK_ClientHandleValidate(DRV_HANDLE handle)
{
    DRV_NVM_BLOCK_CLIENT_OBJ * clientObj;

    /* Validate the handle */
    if((0 == handle)|| (DRV_HANDLE_INVALID == handle))
    {
        return NULL;
    }

    /* See if the client has been opened */
    
    clientObj = (DRV_NVM_BLOCK_CLIENT_OBJ *)handle;

    if(!clientObj->inUse)
    {
        return NULL;
    }

    return clientObj;
}

// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_NVM_BLOCK_Open
    (
        const SYS_MODULE_INDEX drvIndex,
        const DRV_IO_INTENT ioIntent
    );
    
  Summary:
    Opens the specified NVM Block driver instance and returns a handle to it.
  
  Description:
    This function opens the specified NVM driver instance for block operations
    and provides a handle that must be provided to all other client-level
    operations to identify the caller and the instance of the driver.
  
  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

DRV_HANDLE DRV_NVM_BLOCK_Open
(
    const SYS_MODULE_INDEX drvIndex,
    const DRV_IO_INTENT ioIntent
)
{
    DRV_HANDLE nvmDriverHandle;
    DRV_NVM_BLOCK_CLIENT_OBJ * client;
    DRV_NVM_BLOCK_OBJ * hDriver;
    unsigned int i;

    /* Validate the driver system module index */
    if(drvIndex >= DRV_NVM_INSTANCES_NUMBER)
    {
        return DRV_HANDLE_INVALID;
    } 

    hDriver = &gDrvNVMBlockObj[drvIndex];

    /* Check for exclusive opening */
    if((ioIntent & DRV_IO_INTENT_EXCLUSIVE) 
            && (hDriver->nClients > 0))
    {
        /* This means the driver is already opened
         * and cannot be opened exclusively */

        return(DRV_HANDLE_INVALID);
    }

    for(i = 0; i < DRV_NVM_CLIENTS_NUMBER; i ++)
    {
        client = &gDrvNVMBlockClientObj[i];

        /* Search for free client objects */
        if(!client->inUse)
        {
            /* Found a free NVM Block driver client object. Now try opening 
             * NVM driver */

            nvmDriverHandle = DRV_NVM_Open(drvIndex, ioIntent);
            if(DRV_HANDLE_INVALID != nvmDriverHandle)
            {
                /* This means the NVM driver could be opened successfully.
                 * Update the client object and the other parameters. */
                client->inUse = true;
                client->nvmDriverHandle = nvmDriverHandle;
                client->eventHandler = NULL;
                client->context = 0;
                client->ioIntent = ioIntent;
                client->hDriver = hDriver;
                hDriver->nClients ++;
                hDriver->wasOpenedExclusively = (ioIntent & DRV_IO_INTENT_EXCLUSIVE) ? true : false;
                return ((DRV_HANDLE)client);
            }
            else
            {
                /* If the NVM driver itself could not be opened, then
                 * return from this function */
                break;
            }
        }

    }
    
    /* If here, the driver could not be opened */
    return(DRV_HANDLE_INVALID);
}

// *****************************************************************************
/* Function:
    void DRV_NVM_BLOCK_Close( DRV_Handle handle );

  Summary:
    Closes an opened-instance of the NVM Block driver.

  Description:
    This function closes an opened-instance of the NVM Block driver, invalidating
    the handle.

  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

void DRV_NVM_BLOCK_Close( const DRV_HANDLE handle)
{
    DRV_NVM_BLOCK_CLIENT_OBJ * clientObj;
    DRV_NVM_BLOCK_OBJ * hDriver;

    clientObj = _DRV_NVM_BLOCK_ClientHandleValidate(handle);

    /* Validate the handle */
    if(NULL == clientObj)
    {
        /* Dont do anything if the handle is invalid */
        return;
    }

    /* Close the NVM driver */
    DRV_NVM_Close(clientObj->nvmDriverHandle);

    /* Update the client count */
    hDriver = clientObj->hDriver;
    hDriver->nClients --;

    /* Reset the exclusive flag */
    hDriver->wasOpenedExclusively = false;

    /* Indicate client object is not valid */
    clientObj->inUse = false;

    return;
}

// *****************************************************************************
/* Function:
    void DRV_NVM_BlockEventHandlerSet
    (
        const DRV_HANDLE handle,
        const DRV_NVM_BLOCK_EVENT_HANDLER eventHandler,
        const uintptr_t context
    );

  Summary:
    Allows a client to identify an event handling function for the driver to
    call back when queued operation has completed.

  Description:
    This function allows a client to identify an event handling function for the
    driver to call back when queued operation has completed.  When a client
    calls any read, write or erase function, it is provided with a handle
    identifying the buffer that was added to the driver's buffer queue. The
    driver will pass this handle back to the client by calling "eventHandler"
    function when the queued operation has completed.
    
    The event handler should be set before the client performs any
    read/write/erase operations that could generate events. The event handler
    once set, persists until the client closes the driver or sets another event
    handler (which could be a "NULL" pointer to indicate no callback).

  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

void DRV_NVM_BLOCK_EventHandlerSet
(
    const DRV_HANDLE handle,
    const void * eventHandler,
    const uintptr_t context
)
{
    DRV_NVM_BLOCK_CLIENT_OBJ * clientObj;

    /* Validate the handle */

    clientObj = _DRV_NVM_BLOCK_ClientHandleValidate(handle);

    if(NULL == clientObj)
    {
        /* Client handle is invalid */
        return;
    }

    /* Set the event Handler */
    clientObj->eventHandler = (DRV_NVM_BLOCK_EVENT_HANDLER)eventHandler;

    /* Client context */
    clientObj->context = context;

    /* Register an event handler with the NVM driver */
    DRV_NVM_BlockEventHandlerSet(clientObj->nvmDriverHandle, _DRV_NVM_BLOCK_NVMEventHandler, (uintptr_t)clientObj);
}

// **************************************************************************
/* Function:
    void DRV_NVM_BlockErase
    (
        const DRV_HANDLE handle,
        DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
        uint32_t blockStart,
        uint32_t nBlock
    );
    
  Summary:
    Erase the specified number of blocks in flash memory.
  
  Description:
    This function schedules a non-blocking erase operation in flash memory.  The
    function returns with a valid erase handle in the commandHandle argument if
    the erase request was scheduled successfully. The function adds the request
    to the hardware instance queue and returns immediately.  The function
    returns DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID in the commandHandle argument
    under the following circumstances:
    - if the client opened the driver for read only
    - if nBlock is 0
    - if the queue size is full or queue depth is insufficient
    - if the driver handle is invalid 
    If the requesting client registered an event callback with the driver, the
    driver will issue a DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE event if the
    erase operation was successful or DRV_NVM_EVENT_BLOCK_COMMAND_ERROR
    event if the erase operation was not successful.
      
  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

void DRV_NVM_BlockErase
(
    const DRV_HANDLE handle,
    DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
    uint32_t blockStart,
    uint32_t nBlock
)
{
    /* NVM Driver erase is blocking */
    
    DRV_NVM_BLOCK_CLIENT_OBJ * clientObj;
    DRV_NVM_BLOCK_COMMAND_HANDLE * tempHandle1, tempHandle2;

    clientObj = _DRV_NVM_BLOCK_ClientHandleValidate(handle);
    tempHandle1 = (commandHandle == NULL) ? &tempHandle2 : commandHandle;

    if(clientObj == NULL)
    {
        /* Client handle is not valid */
        *commandHandle = DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID;    
        return;
    }

    if(((blockStart + nBlock) >= gNVMGeometryTable[GEOMETRY_TABLE_ERASE_ENTRY].numBlocks) || (nBlock == 0))
    {
        /* Input parameters are not valid */
        *tempHandle1 = DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID;    
        return;
    }

    *tempHandle1 = DRV_NVM_Erase(clientObj->nvmDriverHandle, 
            (uint8_t *)(clientObj->blockStartAddress  + (blockStart * DRV_NVM_PAGE_SIZE)), (nBlock * DRV_NVM_PAGE_SIZE));
}

// *****************************************************************************
/* Function:
    void DRV_NVM_BlockRead
    (
        const DRV_HANDLE handle,
        DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
        void * targetBuffer,
        uint32_t blockStart,
        uint32_t nBlock
    );

  Summary:
    Reads blocks of data starting from the specified address in flash memory.

  Description:
    This function schedules a non-blocking read operation for reading blocks of
    data from flash memory. The function returns with a valid handle in the
    commandHandle argument if the read request was scheduled successfully.  The
    function adds the request to the hardware instance queue and returns
    immediately. While the request is in the queue, the application buffer is
    owned by the driver and should not be modified.  The function returns
    DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID in the commandHandle
    argument under the following circumstances:
    - if a buffer could not be allocated to the request
    - if the target buffer pointer is NULL
    - if the client opened the driver for write only
    - if the buffer size is 0 
    - if the read queue size is full or queue depth is insufficient
    - if the driver handle is invalid 
    If the requesting client registered an event callback with the driver, the
    driver will issue a DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE event if the
    buffer was processed successfully of DRV_NVM_EVENT_BLOCK_COMMAND_ERROR
    event if the buffer was not processed successfully.

  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

void DRV_NVM_BlockRead
(
    const DRV_HANDLE handle,
    DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
    void * targetBuffer,
    uint32_t blockStart,
    uint32_t nBlock
)
{
    /* NVM Driver read is not blocking */
    
    DRV_NVM_BLOCK_CLIENT_OBJ * clientObj;
    DRV_NVM_BLOCK_COMMAND_HANDLE * tempHandle1, tempHandle2;

    clientObj = _DRV_NVM_BLOCK_ClientHandleValidate(handle);
    tempHandle1 = (commandHandle == NULL) ? &tempHandle2 : commandHandle;

    if(clientObj == NULL)
    {
        /* Client handle is not valid */
        *commandHandle = DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID;    
        return;
    }
    
    if((targetBuffer == NULL) 
            || ((blockStart + nBlock) >= gNVMGeometryTable[GEOMETRY_TABLE_READ_ENTRY].numBlocks)
            || (nBlock == 0))
    {
         /* Client handle is not valid */
        *tempHandle1 = DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID;    
        return;
    }

    *tempHandle1 = (DRV_NVM_Read(clientObj->nvmDriverHandle, 
            targetBuffer, (uint8_t *)(clientObj->blockStartAddress  + blockStart), nBlock));
}

// *****************************************************************************
/* Function:
    SYS_FS_MEDIA_GEOMETRY * DRV_NVM_GeometryGet( DRV_HANDLE handle );

  Summary:
    Returns the geometry of the device.

  Description:
    This API gives the following geometrical details of the NVM Flash:
    - Media Property
    - Number of Read/Write/Erase regions in the flash device
    - Number of Blocks and their size in each region of the device

  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

SYS_FS_MEDIA_GEOMETRY * DRV_NVM_GeometryGet( DRV_HANDLE handle )
{
    if(NULL == _DRV_NVM_BLOCK_ClientHandleValidate(handle))
    {
        /* Incase of an invalid driver handle, return NULL */
        return NULL;
    }
    
    /* Update the pointer to the media region geometry table
     * and then return the pointer to the media geometry. */
    gDrvNVMMediaGeometry.geometryTable = gNVMGeometryTable; 
    return (&gDrvNVMMediaGeometry);
}

// *****************************************************************************
/* Function:
    void DRV_NVM_BlockWrite
    (
        DRV_HANDLE handle,
        DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
        void * sourceBuffer,
        uint32_t blockStart,
        uint32_t nBlock
    );

  Summary:
    Write blocks of data starting from a specified address in flash memory.

  Description:
    This function schedules a non-blocking write operation for writing blocks of
    data into flash memory. The function returns with a valid buffer handle in
    the commandHandle argument if the write request was scheduled successfully.
    The function adds the request to the hardware instance queue and
    returns immediately. While the request is in the queue, the application
    buffer is owned by the driver and should not be modified.  The function
    returns DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID in the commandHandle
    argument under the following circumstances:
    - if a buffer could not be allocated to the request
    - if the input buffer pointer is NULL
    - if the client opened the driver for read only
    - if the buffer size is 0 
    - if the write queue size is full or queue depth is insufficient
    - if the driver handle is invalid 
    If the requesting client registered an event callback with the driver, the
    driver will issue a DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE event if the
    buffer was processed successfully or DRV_NVM_EVENT_BLOCK_COMMAND_ERROR
    event if the buffer was not processed successfully.

  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

void DRV_NVM_BlockWrite
(
    DRV_HANDLE handle,
    DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
    void * sourceBuffer,
    uint32_t blockStart,
    uint32_t nBlock
)
{
    /* NVM Driver write is blocking */
    
    DRV_NVM_BLOCK_CLIENT_OBJ * clientObj;
    DRV_NVM_BLOCK_COMMAND_HANDLE * tempHandle1, tempHandle2;

    clientObj = _DRV_NVM_BLOCK_ClientHandleValidate(handle);
    tempHandle1 = (commandHandle == NULL) ? &tempHandle2 : commandHandle;

    if(clientObj == NULL)
    {
        /* Client handle is not valid */
        *tempHandle1 = DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID;    
        return;
    }

    if((sourceBuffer == NULL) 
            || ((blockStart + nBlock) >= gNVMGeometryTable[GEOMETRY_TABLE_WRITE_ENTRY].numBlocks)
            || (nBlock == 0))
    {
         /* Client handle is not valid */
        *tempHandle1 = DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID;    
        return;
    }

    *tempHandle1 = (DRV_NVM_Write(clientObj->nvmDriverHandle, 
            (uint8_t *)(clientObj->blockStartAddress  + (blockStart * DRV_NVM_ROW_SIZE)),
            sourceBuffer, (nBlock * DRV_NVM_ROW_SIZE)));
}

// *****************************************************************************
/* Function:
    bool DRV_NVM_isAttached( DRV_HANDLE handle );

  Summary:
    Returns the physical attach status of the NVM.

  Description:
    This function always returns the physical attach status of the NVM. This
    function always returns true.

  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

bool DRV_NVM_IsAttached(DRV_HANDLE handle)
{
    /* Make sure we have a valid handle */
    if(NULL == _DRV_NVM_BLOCK_ClientHandleValidate(handle))
    {
        return false;
    }

    /* This function always returns true */
   return true;
}

// *****************************************************************************
/* Function:
    bool DRV_NVM_isWriteProtected( DRV_HANDLE handle );

  Summary:
    Returns the write protect status of NVM.

  Description:
    This function returns the write protect status of the NVM. Always returns
    false.

  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

bool DRV_NVM_IsWriteProtected(DRV_HANDLE handle)
{
    /* This function always returns false */
   return false;
}

// *****************************************************************************
/* Function:
    void DRV_NVM_BlockStartAddressSet
    (
        DRV_HANDLE handle,
        void * pointerToBlock0
    );

  Summary:
    Sets the logical block 0 address for this client.

  Description:
    This function sets the block 0 address for this client. The driver
    treats this address as block 0 address for read, write and erase operations.

  Remarks:
    Refer to drv_nvm_block.h for usage information.
*/

void DRV_NVM_BlockStartAddressSet
(
    const DRV_HANDLE handle,
    const void * pointerToBlock0
)
{
    DRV_NVM_BLOCK_CLIENT_OBJ * client;

    client = _DRV_NVM_BLOCK_ClientHandleValidate(handle);

    if(NULL == client)
    {
        /* Invalid client handle */
        return;
    }

    client->blockStartAddress = (uint32_t)pointerToBlock0;
}

// *****************************************************************************
/* Function:
    void DRV_NVM_BlockEraseWrite
    (
        DRV_HANDLE handle,
        DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
        void * sourceBuffer,
        uint32_t blockStart,
        uint32_t nBlock
    );

  Summary:
    Erase and Write blocks of data starting from a specified address in flash
    memory.

  Description:
    This function schedules a non-blocking operation to erase and write blocks of
    data into flash memory. The function returns with a valid buffer handle in
    the commandHandle argument if the write request was scheduled successfully.
    The function adds the request to the hardware instance queue and
    returns immediately. While the request is in the queue, the application
    buffer is owned by the driver and should not be modified.  The function
    returns DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID in the commandHandle
    argument under the following circumstances:
    - if a buffer could not be allocated to the request
    - if the input buffer pointer is NULL
    - if the client opened the driver for read only
    - if the buffer size is 0 
    - if the write queue size is full or queue depth is insufficient
    - if the driver handle is invalid 
    If the requesting client registered an event callback with the driver, the
    driver will issue a DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE event if the
    buffer was processed successfully or DRV_NVM_EVENT_BLOCK_COMMAND_ERROR
    event if the buffer was not processed successfully.

  Remarks
    Refer to drv_nvm_block.h for usage information.
*/

void DRV_NVM_BlockEraseWrite
(
    DRV_HANDLE handle,
    DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
    void * sourceBuffer,
    uint32_t blockStart,
    uint32_t nBlock
)
{
    /* NVM Driver write is blocking */

    DRV_NVM_BLOCK_CLIENT_OBJ * clientObj;
    DRV_NVM_BLOCK_COMMAND_HANDLE * tempHandle1, tempHandle2;

    clientObj = _DRV_NVM_BLOCK_ClientHandleValidate(handle);
    tempHandle1 = (commandHandle == NULL) ? &tempHandle2 : commandHandle;

    if(clientObj == NULL)
    {
        /* Client handle is not valid */
        *tempHandle1 = DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID;
        return;
    }

    if((sourceBuffer == NULL)
            || ((blockStart + nBlock) >= gNVMGeometryTable[GEOMETRY_TABLE_WRITE_ENTRY].numBlocks)
            || (nBlock == 0))
    {
         /* Client handle is not valid */
        *tempHandle1 = DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID;
        return;
    }

    *tempHandle1 = (DRV_NVM_EraseWrite(clientObj->nvmDriverHandle,
            (uint8_t *)(clientObj->blockStartAddress  + (blockStart * DRV_NVM_ROW_SIZE)),
            sourceBuffer, (nBlock * DRV_NVM_ROW_SIZE)));
}

