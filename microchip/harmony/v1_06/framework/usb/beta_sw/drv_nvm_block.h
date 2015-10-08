/*******************************************************************************
  NVM Block Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    drv_nvm_block.h

  Summary:
    NVM Block Driver Interface Definition

  Description:
    The NVM Block Driver provides a block interface to access the NVM on the
    PIC32 microcontroller.  This file defines the interface definition for the
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
#ifndef _DRV_NVM_BLOCK_H_
#define _DRV_NVM_BLOCK_H_

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "driver/nvm/legacy/drv_nvm.h"
#include "system/fs/sys_fs_media_manager.h"

// *****************************************************************************
/* NVM Block Driver command handle.

  Summary:
    Handle identifying block commands queued in the driver.

  Description:
    A block command handle is returned by a call to the Read, Write, or Erase
    functions. This handle allows the application to track the completion of
    the operation. The handle is returned back to the client by the "event
    handler callback" function registered with the driver.

    The handle assigned to a client request expires when the client has 
    been notified of the completion of the operation (after event handler 
    function that notifies the client returns) or after the buffer has been 
    retired by the driver if no event handler callback was set. 

  Remarks:
    None.
*/

typedef SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE  DRV_NVM_BLOCK_COMMAND_HANDLE;

// *****************************************************************************
/* NVM Block Driver Invalid Command Handle.

  Summary:
    This value defines the NVM Block Driver Invalid Command Handle.

  Description:
    This value defines the NVM Block Driver Invalid Command Handle. This value
    returned by read/write/erase routines when the request could not be taken.

  Remarks:
    None.
*/

#define DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID   /*DOM-IGNORE-BEGIN*/ SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID /*DOM-IGNORE-END*/

// *****************************************************************************
/* NVM Block Driver Events

   Summary
    Identifies the possible events that can result from a request.

   Description
    This enumeration identifies the possible events that can result from a 
    Read, Write, or Erase request caused by the client.

   Remarks:
    One of these values is passed in the "event" parameter of the event 
    handling callback function that client registered with the driver by
    calling the DRV_NVM_BLOCK_EventHandlerSet function when a block 
    request is completed.
*/

typedef enum
{
    /* Block operation has been completed successfully. */
    DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE
       /*DOM-IGNORE-BEGIN*/ = SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_COMPLETE /*DOM-IGNORE-END*/,

    /* There was an error during the block operation */
    DRV_NVM_EVENT_BLOCK_COMMAND_ERROR
       /*DOM-IGNORE-BEGIN*/ = SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_ERROR /*DOM-IGNORE-END*/

} DRV_NVM_BLOCK_EVENT;

// *****************************************************************************
/* NVM Block Driver Event Handler Function Pointer

   Summary
    Pointer to a NVM Block Driver Event handler function

   Description
    This data type defines the required function signature for the NVM Block
    Driver event handling callback function. A client must register a pointer to
    an event handling function whose function signature (parameter and return
    value types) match the types specified by this function pointer in order to
    receive event call back from the driver.
    
    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    event           - Identifies the type of event
    
    commandHandle   - Handle returned from the Read/Write/Erase requests
    
    context         - Value identifying the context of the application that
                      registered the event handling function

  Returns:
    None.

  Example:
    <code>
    void APP_MyBufferEventHandler
    (
        DRV_NVM_BLOCK_EVENT event,
        DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle, 
        uintptr_t context
    )
    {
        MY_APP_DATA_STRUCT * pAppData = (MY_APP_DATA_STRUCT *) context;
        
        switch(event)
        {
            case DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE:

                // Handle the completed buffer. 
                break;
            
            case DRV_NVM_EVENT_BLOCK_COMMAND_ERROR:
            default:

                // Handle error.
                break;
        }
    }
    </code>

  Remarks:
    If the event is DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE, it means that the
    data was transferred successfully. 
    
    If the event is DRV_NVM_EVENT_BLOCK_COMMAND_ERROR, it means that the data
    was not transferred successfully.
     
    The context parameter contains the a handle to the client context,  
    provided at the time the event handling function was registered using the
    DRV_NVM_BlockEventHandlerSet function. This context handle value is
    passed back to the client as the "context" parameter.  It can be any value
    necessary to identify the client context or instance (such as a pointer to
    the client's data) instance of the client that made the read/write/erase
    request.

    The event handler function executes in the driver peripheral's interrupt
    context when the driver is configured for interrupt mode operation. It is
    recommended of the application to not perform process intensive or blocking
    operations with in this function.

    The Read, Write, and Erase functions can be called in the event handler to
    add a buffer to the driver queue. These functions can only be called to add
    buffers to the driver whose event handler is running. 
*/

typedef void ( *DRV_NVM_BLOCK_EVENT_HANDLER ) 
(
    DRV_NVM_BLOCK_EVENT event, 
    DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle,
    uintptr_t context
);

// *****************************************************************************
// *****************************************************************************
// Section: NVM Block Driver Client Routines
// *****************************************************************************
// *****************************************************************************

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
  
  Precondition:
    Function DRV_NVM_Initialize must have been called before calling
    this function.
  
  Parameters:
    drvIndex -  Identifier for the object instance to be opened
    intent -    Zero or more of the values from the enumeration
                DRV_IO_INTENT "ORed" together to indicate the intended use
                of the driver
  
  Returns:
    If successful, the function returns a valid open-instance handle (a
    number identifying both the caller and the module instance).
    
    If an error occurs, the return value is DRV_HANDLE_INVALID. Errors can occur
	under the following circumstances:
    - if the number of client objects allocated via
         DRV_NVM_CLIENTS_NUMBER is insufficient
    - if the client is trying to open the driver but driver has been opened
         exclusively by another client
    - if the driver hardware instance being opened is not initialized or is
         invalid
    - if the client is trying to open the driver exclusively, but has already
         been opened in a non exclusive mode by another client.
  
  Example:
    <code>
    DRV_HANDLE handle;
    
    handle = DRV_NVM_BLOCK_Open(DRV_NVM_INDEX_0, DRV_IO_INTENT_EXCLUSIVE);
    if (DRV_HANDLE_INVALID == handle)
    {
        // Unable to open the driver
    }
    </code>
  
  Remarks:
    The driver will always work in Non-Blocking mode even if IO-intent is
    selected as blocking.

    The handle returned is valid until the DRV_NVM_BLOCK_Close function is
    called.
    
    This function will NEVER block waiting for hardware.                                            
*/

DRV_HANDLE DRV_NVM_BLOCK_Open
(
    const SYS_MODULE_INDEX drvIndex,
    const DRV_IO_INTENT ioIntent
);

// *****************************************************************************
/* Function:
    void DRV_NVM_BLOCK_Close( DRV_Handle handle );

  Summary:
    Closes an opened-instance of the NVM Block driver.

  Description:
    This function closes an opened-instance of the NVM Block driver, invalidating
    the handle.

  Precondition:
    The DRV_NVM_Initialize function must have been called for the
    specified NVM driver instance.

    DRV_NVM_BLOCK_Open must have been called to obtain a valid opened device
    handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

  Returns:
    None

  Example:
    <code>
    DRV_HANDLE handle;  // Returned from DRV_NVM_BLOCK_Open

    DRV_NVM_BLOCK_Close(handle);
    </code>

  Remarks:
    After calling this function, the handle passed in "handle" must not be used
    with any of the remaining driver routines.  A new handle must be obtained by
    calling DRV_NVM_BLOCK_Open before the caller may use the driver again.

    Note: Usually, there is no need for the driver client to verify that the
    Close operation has completed.
*/

void DRV_NVM_BLOCK_Close( const DRV_HANDLE handle);

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

  Precondition:
    The DRV_NVM_Initialize function must have been called for the
    specified NVM Block Driver instance.

    DRV_NVM_BLOCK_Open must have been called to obtain a valid opened device
    handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

    eventHandler - Pointer to the event handler function implemented by the user
    
    context      - The value of parameter will be passed back to the client 
                   unchanged, when the eventHandler function is called. It can
                   be used to identify any client specific data object that 
                   identifies the instance of the client module (for example, 
                   it may be a pointer to the client module's state structure).

  Returns:
    None.

  Example:
    <code>
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    uint8_t myBuffer[MY_BUFFER_SIZE];
    uint32_t blockStart, nBlock;
    DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle;

    // drvNVMHandle is the handle returned 
    // by the DRV_NVM_BLOCK_Open function.
    
    // Client registers an event handler with driver. This is done once.

    DRV_NVM_BlockEventHandlerSet( drvNVMHandle, APP_NVMBlockEventHandler, (uintptr_t)&myAppObj );

    DRV_NVM_BlockRead( drvNVMHandle, commandHandle, &myBuffer, blockStart, nBlock );

    if(DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID == commandHandle)
    {
        // Error handling here
    }

    // Event Processing Technique. Event is received when operation is done.

    void APP_NVMBlockEventHandler(DRV_NVM_BLOCK_EVENT event, 
            DRV_NVM_BLOCK_COMMAND_HANDLE handle, uintptr_t context)
    {
        // The context handle was set to an application specific
        // object. It is now retrievable easily in the event handler.
        MY_APP_OBJ myAppObj = (MY_APP_OBJ *) context;

        switch(event)
        {
            case DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE:

                // This means the data was transferred. 
                break;
            
            case DRV_NVM_EVENT_BLOCK_COMMAND_ERROR:

                // Error handling here.

                break;

            default:
                break;
        }
    }
    </code>

  Remarks:
    If the client does not want to be notified when the queued operation
    has completed, it does not need to register a callback.
*/

void DRV_NVM_BLOCK_EventHandlerSet
(
    const DRV_HANDLE handle,
    const void * eventHandler,
    const uintptr_t context
);

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
      
  Precondition:
    The DRV_NVM_Initialize function must have been called for the
    specified NVM driver instance.
    
    DRV_NVM_BLOCK_Open must have been called to obtain a valid opened device
    handle.
    
    DRV_IO_INTENT_WRITE or DRV_IO_INTENT_READWRITE must have been specified
    in the DRV_NVM_Open call.
  
  Parameters:
    handle -       A valid open-instance handle, returned from the
                   driver's open function

    commandHandle -  Pointer to an argument that will contain the return buffer
                     handle

    blockStart -   Start block address in NVM memory from where the
                   erase should begin. This should be aligned on a erase block
                   boundary

    nBlock -       Total number of blocks to be erased. 
  
  Returns:
    The buffer handle is returned in the commandHandle argument. It Will be
    DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID if the request was not queued.
  
  Example:
    <code>
   
    // Destination address should be block aligned.
    uint32_t blockStart;
    uint32_t nBlock; 
    DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle;
    MY_APP_OBJ myAppObj;    

    // myNVMHandle is the handle returned 
    // by the DRV_NVM_BLOCK_Open function.
    
    // Client registers an event handler with driver

    DRV_NVM_BlockEventHandlerSet(myNVMHandle, APP_NVMEventHandler, (uintptr_t)&myAppObj);

    DRV_NVM_BlockErase( myNVMHandle, commandHandle, blockStart, nBlock );

    if(DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID == commandHandle)
    {
        // Error handling here
    }

    // Event is received when
    // the buffer queue is processed.

    void APP_NVMEventHandler(DRV_NVM_BLOCK_EVENT event, 
            DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle, uintptr_t contextHandle)
    {
        // contextHandle points to myAppObj.

        switch(event)
        {
            case DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE:

                // This means the data was transferred. 
                break;
            
            case DRV_NVM_EVENT_BLOCK_COMMAND_ERROR:

                // Error handling here.

                break;

            default:
                break;
        }
    }
    
    </code>
  
  Remarks:
    None.
*/

void DRV_NVM_BlockErase
(
    const DRV_HANDLE handle,
    DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
    uint32_t blockStart,
    uint32_t nBlock
);

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

  Precondition:
    The DRV_NVM_Initialize function must have been called for the
    specified NVM driver instance.

    DRV_NVM_BLOCK_Open must have been called to obtain a valid opened device
    handle.

    DRV_IO_INTENT_READ or DRV_IO_INTENT_READWRITE must have been specified in
    the DRV_NVM_Open call.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

    commandHandle - Pointer to an argument that will contain the return buffer
                   handle
                   
    targetBuffer - Buffer into which the data read from the SPI Flash instance
                   will be placed

    blockStart -  Start block address in NVM memory from where the
                  read should begin. It can be any address of the flash.

    nBlock -      Total number of blocks to be read. Each Read block is of 1
                  byte.

  Returns:
    The buffer handle is returned in the commandHandle argument. It will be
    DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID if the request was not successful.

  Example:
    <code>

    uint8_t myBuffer[MY_BUFFER_SIZE];
    
    // address should be block aligned.
    uint32_t blockStart = NVM_BASE_ADDRESS_TO_READ_FROM;
    uint32_t    nBlock = 2;
    DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle;
    MY_APP_OBJ myAppObj;    

    // myNVMHandle is the handle returned 
    // by the DRV_NVM_Open function.
    
    // Client registers an event handler with driver

    DRV_NVM_BlockEventHandlerSet(myNVMHandle, APP_NVMEventHandler, (uintptr_t)&myAppObj);

    DRV_NVM_BlockRead( myNVMHandle, &commandHandle, &myBuffer, blockStart, nBlock );

    if(DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID == commandHandle)
    {
        // Error handling here
    }

    // Event is received when the buffer is processed.

    void APP_NVMEventHandler(DRV_NVM_BLOCK_EVENT event, 
            DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle, uintptr_t contextHandle)
    {
        // contextHandle points to myAppObj.

        switch(event)
        {
            case DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE:

                // This means the data was transferred. 
                break;
            
            case DRV_NVM_EVENT_BLOCK_COMMAND_ERROR:

                // Error handling here.

                break;

            default:
                break;
        }
    }

    </code>

  Remarks:
    None.
*/

void DRV_NVM_BlockRead
(
    const DRV_HANDLE handle,
    DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
    void * targetBuffer,
    uint32_t blockStart,
    uint32_t nBlock
);

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

  Precondition:
    None.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

  Returns:
    SYS_FS_MEDIA_GEOMETRY - Structure which holds the media geometry information.

  Example:
    <code> 
    
    SYS_FS_MEDIA_GEOMETRY * nvmFlashGeometry;
    uint32_t readBlockSize, writeBlockSize, eraseBlockSize;
    uint32_t nReadBlocks, nReadRegions, totalFlashSize;

    nvmFlashGeometry = DRV_NVM_GeometryGet(nvmOpenHandle1);

    readBlockSize  = nvmFlashGeometry->geometryTable->blockSize;
    nReadBlocks = nvmFlashGeometry->geometryTable->numBlocks;
    nReadRegions = nvmFlashGeometry->numReadRegions;

    writeBlockSize  = (nvmFlashGeometry->geometryTable +1)->blockSize;
    eraseBlockSize  = (nvmFlashGeometry->geometryTable +2)->blockSize;

    totalFlashSize = readBlockSize * nReadBlocks * nReadRegions;

    </code>

  Remarks:
    None.
*/

SYS_FS_MEDIA_GEOMETRY * DRV_NVM_GeometryGet( DRV_HANDLE handle );

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

  Precondition:
    The DRV_NVM_Initialize function must have been called for the
    specified NVM driver instance.

    DRV_NVM_BLOCK_Open must have been called to obtain a valid opened
    device handle.

    DRV_IO_INTENT_WRITE or DRV_IO_INTENT_READWRITE must have been specified in
    the DRV_NVM_Open call.

    The flash address location which has to be written, must be erased before
    using the DRV_NVM_BlockErase().

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

    commandHandle -Pointer to an argument that will contain the return buffer
                   handle
                   
    sourceBuffer - The source buffer containing data to be programmed into SPI
                   Flash

    blockStart -   Start block address of NVM Flash where the write 
                   should begin. This address should be aligned on a block boundary.

    nBlock -       Total number of blocks to be written. 

  Returns:
    The buffer handle is returned in the commandHandle argument. It will be
    DRV_BUFFER_HANDLE_INVALID if the request was not successful.

  Example:
    <code>
    
    uint8_t myBuffer[MY_BUFFER_SIZE];
    
    // address should be block aligned.
    uint32_t blockStart = NVM_BASE_ADDRESS_TO_WRITE_TO;
    uint32_t    nBlock = 2;
    DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle;
    MY_APP_OBJ myAppObj;    

    // myNVMHandle is the handle returned 
    // by the DRV_NVM_BLOCK_Open function.
    
    // Client registers an event handler with driver

    DRV_NVM_BlockEventHandlerSet(myNVMHandle, APP_NVMEventHandler, (uintptr_t)&myAppObj);

    DRV_NVM_BlockWrite( myNVMHandle, commandHandle, &myBuffer, blockStart, nBlock );

    if(DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID == commandHandle)
    {
        // Error handling here
    }

    // Event is received when
    // the buffer is processed.

    void APP_NVMBlockEventHandler(DRV_NVM_BLOCK_EVENT event, 
            DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle, uintptr_t contextHandle)
    {
        // contextHandle points to myAppObj.

        switch(event)
        {
            case DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE:

                // This means the data was transferred. 
                break;
            
            case DRV_NVM_EVENT_BLOCK_COMMAND_ERROR:

                // Error handling here.

                break;

            default:
                break;
        }
    }

    </code>

  Remarks:
    None.
*/

void DRV_NVM_BlockWrite
(
    DRV_HANDLE handle,
    DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
    void * sourceBuffer,
    uint32_t blockStart,
    uint32_t nBlock
);

// *****************************************************************************
/* Function:
    bool DRV_NVM_IsAttached( DRV_HANDLE handle );

  Summary:
    Returns the physical attach status of the NVM.

  Description:
    This function returns the physical attach status of the NVM. This
    function always returns true.

  Precondition:
    None.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

  Returns:
    Always returns true.

  Example:
    <code> 

    // The NVM media is always attached and so the below
    // always returns true.
    
    bool isNVMAttached;
    isNVMAttached = DRV_NVM_isAttached(drvNVMBlockHandle);

    </code>

  Remarks:
    None.
*/

bool DRV_NVM_IsAttached(DRV_HANDLE handle);

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

  Precondition:
    Driver should have been opened and handle should be valid.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function
    pointerToBlock0 - Should be uint32_t pointer to the block 0 address.

  Returns:
    None.

  Example:
    <code>

    // This sets the 0 block address as the start address of nvmBuffer

    uint8_t nvmBuffer[4096] __attribure__((space(prog), aligned(4096)));
    DRV_NVM_BlockStartAddress(handle, nvmBuffer);

    </code>

  Remarks:
    None.
*/

void DRV_NVM_BlockStartAddressSet
(
    const DRV_HANDLE handle,
    const void * pointerToBlock0
);

// *****************************************************************************
/* Function:
    bool DRV_NVM_IsWriteProtected( DRV_HANDLE handle );

  Summary:
    Returns the write protect status of the NVM.

  Description:
    This function returns the physical attach status of the NVM. This
    function always returns false.

  Precondition:
    None.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

  Returns:
    Always returns true.

  Example:
    <code>

    // The NVM media is treated as always writeable and so the below
    // always returns false.

    bool isWriteProtected;
    isWriteProtected = DRV_NVM_IsWriteProtected(drvNVMBlockHandle);

    </code>

  Remarks:
    None.
*/

bool DRV_NVM_IsWriteProtected(DRV_HANDLE handle);

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

  Precondition:
    The DRV_NVM_Initialize function must have been called for the
    specified NVM driver instance.

    DRV_NVM_BLOCK_Open must have been called to obtain a valid opened
    device handle.

    DRV_IO_INTENT_WRITE or DRV_IO_INTENT_READWRITE must have been specified in
    the DRV_NVM_Open call.

    The flash address location which has to be written, must be erased before
    using the DRV_NVM_BlockErase().

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

    commandHandle -Pointer to an argument that will contain the return buffer
                   handle. If NULL, then buffer handle is not returned.
                   
    sourceBuffer - The source buffer containing data to be programmed into SPI
                   Flash

    blockStart -   Start block address of NVM Flash where the write 
                   should begin. This address should be aligned on a block boundary.

    nBlock -       Total number of blocks to be written. 

  Returns:
    The buffer handle is returned in the commandHandle argument. It will be
    DRV_BUFFER_HANDLE_INVALID if the request was not successful.

  Example:
    <code>
    
    uint8_t myBuffer[MY_BUFFER_SIZE];
    
    // address should be block aligned.
    uint32_t blockStart = NVM_BASE_ADDRESS_TO_WRITE_TO;
    uint32_t    nBlock = 2;
    DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle;
    MY_APP_OBJ myAppObj;    

    // myNVMHandle is the handle returned 
    // by the DRV_NVM_BLOCK_Open function.
    
    // Client registers an event handler with driver

    DRV_NVM_BlockEventHandlerSet(myNVMHandle, APP_NVMEventHandler, (uintptr_t)&myAppObj);

    DRV_NVM_BlockEraseWrite( myNVMHandle, commandHandle, &myBuffer, blockStart, nBlock );

    if(DRV_NVM_BLOCK_COMMAND_HANDLE_INVALID == commandHandle)
    {
        // Error handling here
    }

    // Event is received when
    // the buffer is processed.

    void APP_NVMBlockEventHandler(DRV_NVM_BLOCK_EVENT event, 
            DRV_NVM_BLOCK_COMMAND_HANDLE commandHandle, uintptr_t contextHandle)
    {
        // contextHandle points to myAppObj.

        switch(event)
        {
            case DRV_NVM_EVENT_BLOCK_COMMAND_COMPLETE:

                // This means the data was transferred. 
                break;
            
            case DRV_NVM_EVENT_BLOCK_COMMAND_ERROR:

                // Error handling here.

                break;

            default:
                break;
        }
    }

    </code>

  Remarks:
    None.
*/

void DRV_NVM_BlockEraseWrite
(
    DRV_HANDLE handle,
    DRV_NVM_BLOCK_COMMAND_HANDLE * commandHandle,
    void * sourceBuffer,
    uint32_t blockStart,
    uint32_t nBlock
);

#endif
