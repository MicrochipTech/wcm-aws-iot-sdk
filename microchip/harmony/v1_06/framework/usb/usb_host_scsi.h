/*******************************************************************************
  USB Host SCSI Client Driver definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_scsi.h

  Summary:
    USB Host SCSI related definitions.

  Description:
    This file contains USB Host SCSI driver related definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to  you  the  right  to  use,  modify,  copy  and  distribute
Software only when embedded on a Microchip  microcontroller  or  digital  signal
controller  that  is  integrated  into  your  product  or  third  party  product
(pursuant to the  sublicense  terms  in  the  accompanying  license  agreement).

You should refer  to  the  license  agreement  accompanying  this  Software  for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef _USB_HOST_SCSI_H
#define _USB_HOST_SCSI_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system/fs/sys_fs_media_manager.h"
#include "usb/usb_host_msd.h"
#include "driver/driver_common.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
/* SCSI Driver command handle.

  Summary:
    Handle identifying commands queued in the driver.

  Description:
    A command handle is returned by a call to the Read, Write functions. This
    handle allows the application to track the completion of the operation. This
    command handle is also returned to the client along with the event that has
    occurred with respect to the command.  This allows the application to
    connect the event to a specific command in case where multiple commands are
    queued.

    The command handle associated with the command request expires when the
    client has been notified of the completion of the command (after event
    handler function that notifies the client returns) or after the command has
    been retired by the driver if no event handler callback was set. 

  Remarks:
    None.
*/

typedef SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE USB_HOST_SCSI_COMMAND_HANDLE;

// *****************************************************************************
/* SCSI Driver Invalid Command Handle.

  Summary:
    This value defines the SCSI Driver's Invalid Command Handle.

  Description:
    This value defines the SCSI Driver Invalid Command Handle. This value is
    returned by read/write routines when the command request was not accepted.

  Remarks:
    None.
*/

#define USB_HOST_SCSI_COMMAND_HANDLE_INVALID SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID

// *****************************************************************************
/* SCSI Driver Events

   Summary
    Identifies the possible events that can result from a request.

   Description
    This enumeration identifies the possible events that can result from a 
    Write or Erase request caused by the client.

   Remarks:
    One of these values is passed in the "event" parameter of the event handling
    callback function that client registered with the driver by calling the
    USB_HOST_SCSI_EventHandlerSet function when a request is completed.
*/

typedef enum
{
    /* Operation has been completed successfully. */
    USB_HOST_SCSI_EVENT_COMMAND_COMPLETE = SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_COMPLETE,

    /* There was an error during the operation */
    USB_HOST_SCSI_EVENT_COMMAND_ERROR = SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_ERROR 

} USB_HOST_SCSI_EVENT;

// *****************************************************************************
/* SCSI Driver Event Handler Function Pointer

   Summary
    Pointer to a SCSI Driver Event handler function

   Description
    This data type defines the required function signature for the NVM event
    handling callback function. A client must register a pointer to an event
    handling function whose function signature (parameter and return value 
    types) match the types specified by this function pointer in order to 
    receive event calls back from the driver.
    
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
    void APP_MySCSIEventHandler
    (
        USB_HOST_SCSI_EVENT event,
        USB_HOST_SCSI_COMMAND_HANDLE commandHandle,
        uintptr_t context
    )
    {
        MY_APP_DATA_STRUCT pAppData = (MY_APP_DATA_STRUCT) context;
        
        switch(event)
        {
            case USB_HOST_SCSI_EVENT_COMMAND_COMPLETE:

                // Handle the completed buffer. 
                break;
            
            case USB_HOST_SCSI_EVENT_COMMAND_ERROR:
            default:

                // Handle error.
                break;
        }
    }
    </code>

  Remarks:
    If the event is USB_HOST_SCSI_EVENT_COMMAND_COMPLETE, it means that the
    write or a read operation was completed successfully. 
    
    If the event is USB_HOST_SCSI_EVENT_COMMAND_ERROR, it means that the
    scheduled operation was not completed successfully.
     
    The context parameter contains the handle to the client context, provided at
    the time the event handling function was  registered using the
    USB_HOST_SCSI_EventHandlerSet function. This context handle value is passed
    back to the client as the "context" parameter.  It can be any value
    necessary to identify the client context or instance (such as a pointer to
    the client's data) instance of the client that made the read/write/erase
    request.

    The event handler function executes in the driver peripheral's interrupt
    context when the driver is configured for interrupt mode operation. It is
    recommended of the application to not perform process intensive or blocking
    operations within this function.
*/

typedef SYS_FS_MEDIA_EVENT_HANDLER USB_HOST_SCSI_EVENT_HANDLER;

// *****************************************************************************
/*  USB Host SCSI Driver  Handle

  Summary:
    USB Host SCSI Driver Handle.

  Description:
    This type defines the type of the handle that is returned by the
    USB_HOST_SCSI_Open() function.

  Remarks:
    None.
*/

typedef DRV_HANDLE USB_HOST_SCSI_HANDLE;

// *****************************************************************************
/*  USB Host SCSI Driver Invalid Handle

  Summary:
    USB Host SCSI Driver Invalid Handle.

  Description:
    This constant defines an invalid handle. The USB_HOST_SCSI_Open() function
    will return an invalid handle if the open function failed.

  Remarks:
    None.
*/

#define USB_HOST_SCSI_HANDLE_INVALID ((USB_HOST_SCSI_HANDLE)(-1))

// ******************************************************************************
/* Function:
    void USB_HOST_SCSI_Initialize (USB_HOST_MSD_LUN_HANDLE lunHandle)

  Summary:
    This function is called by the MSD Host Client Driver for detected LUN.

  Description:
    This function is called by the MSD Host Client Driver for detected LUN. The
    function will then register this LUN as drive with the file system media
    manager, thus allowing the media manager to access this drive.

  Precondition:
    None.

  Parameters:
    lunHandle - handle to the LUN that the SCSI driver should handle.

  Returns:
    None.

  Examples:
  <code>
  </code>

  Remarks:
    The application will never need to call this function. This function is
    always called the USB Host MSD Client Driver.
*/

void USB_HOST_SCSI_Initialize(USB_HOST_MSD_LUN_HANDLE lunHandle);

// ******************************************************************************
/* Function:
    void USB_HOST_SCSI_DeInitialize (USB_HOST_MSD_LUN_HANDLE lunHandle)

  Summary:
    This function is called by the MSD Host Client Driver for detached LUN.

  Description:
    This function is called by the MSD Host Client Driver for detached LUN. The
    function will then deregister this LUN from the file system media manager.

  Precondition:
    None.

  Parameters:
    lunHandle - handle to the LUN that was deregistered.

  Returns:
    None.

  Examples:
    <code>
    </code>

  Remarks:
    The application will never need to call this function. This function is
    always called the USB Host MSD Client Driver.
*/

void USB_HOST_SCSI_Deinitialize(USB_HOST_MSD_LUN_HANDLE lunHandle);

// ******************************************************************************
/* Function:
    void USB_HOST_SCSI_Tasks (USB_HOST_MSD_LUN_HANDLE lunHandle)

  Summary:
    This function is called by the MSD Host Client Driver to update the driver
    state machine.

  Description:
    This function is called by the MSD Host Client Driver to update the driver
    state machine.

  Precondition:
    None.

  Parameters:
    lunHandle - handle to the LUN for which the state machine needs to be udpated.

  Returns:
    None.

  Examples:
    <code>
    </code>

  Remarks:
    The application will never need to call this function. This function is
    always called the USB Host MSD Client Driver.
*/

void USB_HOST_SCSI_Tasks(USB_HOST_MSD_LUN_HANDLE lunHandle);

// ******************************************************************************
/* Function:
    USB_HOST_SCSI_HANDLE USB_HOST_SCSI_Open 
    (
        const SYS_MODULE_INDEX index, 
        const DRV_IO_INTENT intent
    );

  Summary:
    This function will open the specified instance of the SCSI driver.

  Description:
    This function will open the specified instance of the SCSI driver. The
    handle will allow the client to read and write data to the SCSI device.

  Precondition:
    None.

  Parameters:
    index - index of the SCSI driver to open.
    intent - access intent of client (read only, write only, read-write etc.)

  Returns:
    USB_HOST_SCSI_HANDLE_INVALID if the instance is not ready to opened, a valid
    handle otherwise.

  Example:
    <code>
    </code>

  Remarks:
    This function is typically called by the file system media manager.
*/

USB_HOST_SCSI_HANDLE USB_HOST_SCSI_Open 
(
    SYS_MODULE_INDEX index, 
    DRV_IO_INTENT intent
);

// ******************************************************************************
/* Function:
    USB_HOST_SCSI_HANDLE USB_HOST_SCSI_Close 
    (
        USB_HOST_SCSI_HANDLE scsiHandle
    );

  Summary:
    This function will close the specified instance of the SCSI driver.

  Description:
    This function will close the specified instance of the SCSI driver. The
    handle in scsiHandle will not be valid anymore. The driver must be opened
    once again.

  Precondition:
    None.

  Parameters:
    scsiHandle - handle to the driver to be closed.

  Returns:
    None.

  Example:
    <code>
    </code>

  Remarks:
    This function is typically called by the file system media manager.
*/

void USB_HOST_SCSI_Close 
(
    USB_HOST_SCSI_HANDLE scsiHandle
);
					
// ******************************************************************************
/* Function:
    bool USB_HOST_SCSI_MediaStatusGet 
    (
        USB_HOST_SCSI_HANDLE scsiHandle
    );

  Summary:
    This function will return true if the SCSI media is attached and ready to
    use.

  Description:
    This function will return true if the SCSI media is attached and ready to
    use.

  Precondition:
    None.

  Parameters:
    scsiHandle - handle to the driver to be closed.

  Returns:
    true - media is attached and ready to use.
    false - media is detached.

  Example:
    <code>
    </code>

  Remarks:
    This function is typically called by the file system media manager.
*/

bool USB_HOST_SCSI_MediaStatusGet 
(
    USB_HOST_SCSI_HANDLE scsiHandle
);

// ******************************************************************************
/* Function:
    void USB_HOST_SCSI_SectorRead 
    (
        USB_HOST_SCSI_HANDLE scsiHandle,
        SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE * commandHandle,
        void * buffer,
        uint32_t blockStart,
        uint32_t nBlock
    );

  Summary:
    Peforms a block read operation.

  Description:
    This function will perform a block read operation. The operation will not
    complete when the function returns. Instead a handle to the operation will
    be returned in commandHandle and driver will generate an event when the
    operation has completed. The command handle will be returned along with the
    event. The data will be stored in buffer. Data will be read from the
    blockStart block and nBlock buffers will be read.

  Precondition:
    None.

  Parameters:
    scsiHandle - handle to the driver to be closed.

    commandHandle - pointer to a variable where the command handle will be stored.

    buffer - Destination buffer. The contents of this buffer are valid only when
    data has been received (data has been generated).

    blockStart - block from which data should be read.

    nBlock - number of blocks to read.

  Returns:
    A handle to the command is returned in commandHandle. This will be
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID if the request failed.

  Example:
    <code>
    </code>

  Remarks:
    This function is typically called by the file system media manager.
*/

void USB_HOST_SCSI_SectorRead 
(
    USB_HOST_SCSI_HANDLE scsiHandle,
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE * commandHandle,
    void * buffer,
    uint32_t blockStart,
    uint32_t nBlock
);

// ******************************************************************************
/* Function:
    void USB_HOST_SCSI_SectorWrite 
    (
        USB_HOST_SCSI_HANDLE scsiHandle,
        SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE * commandHandle,
        void * buffer,
        uint32_t blockStart,
        uint32_t nBlock
    );

  Summary:
    Peforms a block write operation.

  Description:
    This function will perform a block write operation. The operation will not
    complete when the function returns. Instead a handle to the operation will
    be returned in commandHandle and driver will generate an event when the
    operation has completed. The command handle will be returned along with the
    event. The data to be written is specified by buffer. Data will be written
    to the blockStart block and nBlock buffers will be written.

  Precondition:
    None.

  Parameters:
    scsiHandle - handle to the driver to be closed.

    commandHandle - pointer to a variable where the command handle will be stored.

    buffer - Source buffer. The contents of this buffer should not be modified
    untill the write operation has completed (till the event has been
    generated).

    blockStart - block to which data should be written to.

    nBlock - number of blocks to written.

  Returns:
    A handle to the command is returned in commandHandle. This will be
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID if the request failed.

  Example:
    <code>
    </code>

  Remarks:
    This function is typically called by the file system media manager.
*/

void USB_HOST_SCSI_SectorWrite 
(
    USB_HOST_SCSI_HANDLE scsiHandle,
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE * commandHandle,
    void * buffer,
    uint32_t blockStart,
    uint32_t nBlock
);

// ******************************************************************************
/* Function:
    void USB_HOST_SCSI_EventHandlerSet 
    (
        USB_HOST_SCSI_HANDLE scsiHandle,
        const void * eventHandler,
        const uintptr_t context
    );

  Summary:
    Registers an event handler with media driver.

  Description:
    This function will register an event handler with the SCSI driver. When read
    or a write command has completed, the SCSI driver will call this function
    with the relevant event. The context parameter will returned with this
    event.

  Precondition:
    None.

  Parameters:
    scsiHandle - handle to the driver to be closed.

    eventHandler - pointer to the event handler.

    context - client specific context that is returned in the event handler.

  Returns:
    None.

  Example:
    <code>
    </code>

  Remarks:
    This function is typically called by the file system media manager.
*/

void USB_HOST_SCSI_EventHandlerSet 
(
    USB_HOST_SCSI_HANDLE scsiHandle,
    const void * eventHandler,
    const uintptr_t context
);

// ******************************************************************************
/* Function:
    SYS_FS_MEDIA_GEOMETRY * USB_HOST_SCSI_MediaGeometryGet 
    (
        USB_HOST_SCSI_HANDLE scsiHandle
    );

  Summary:
    Return the media geometry of this media.

  Description:
    This function will return the media geometry of this media. 

  Precondition:
    None.

  Parameters:
    scsiHandle - handle to the driver.

  Returns:
    None.

  Example:
    <code>
    </code>

  Remarks:
    This function is typically called by the file system media manager.
*/

SYS_FS_MEDIA_GEOMETRY * USB_HOST_SCSI_MediaGeometryGet 
(
    USB_HOST_SCSI_HANDLE scsiHandle
);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif

