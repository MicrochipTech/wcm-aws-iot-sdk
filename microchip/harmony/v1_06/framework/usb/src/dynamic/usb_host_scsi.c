/*******************************************************************************
  USB Host Layer implemention.

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_msd_scsi.c

  Summary:
    This file contains implementations of both private and public functions
    of the USB Host MSD SCSI client driver.

  Description:
    This file contains the USB Host MSD SCSI client driver implementation. This
    file should be included in the project if USB MSD devices are to be
    supported.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute Software
only when embedded on a Microchip microcontroller or digital  signal  controller
that is integrated into your product or third party  product  (pursuant  to  the
sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
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
//DOM-IGNORE-END

#include "usb/usb_host_msd.h"
#include "usb/usb_host_scsi.h"
#include "usb/src/usb_host_scsi_local.h"
#include "system/fs/sys_fs_media_manager.h"
#include "system/debug/sys_debug.h"

/******************************************************
 * USB HOST MSD SCSI Instance object. One for each LUN
 ******************************************************/

USB_HOST_SCSI_OBJ gUSBHostSCSIObj[USB_HOST_MSD_LUN_NUMBERS];

/******************************************************
 * USB HOST MSD SCSI buffers needed for SCSI operation
 ******************************************************/

uint8_t gUSBSCSIBuffer[USB_HOST_MSD_LUN_NUMBERS][256] __attribute__((coherent)) __attribute__((aligned(4)));

/*****************************************************
 * Media functions table that is exported to the
 * file system. Structure is defined by the file
 * system.
 *****************************************************/
SYS_FS_MEDIA_FUNCTIONS gUSBHostMSDSCSIMediaFunctions =
{
    .mediaStatusGet     = USB_HOST_SCSI_MediaStatusGet,
    .mediaGeometryGet   = USB_HOST_SCSI_MediaGeometryGet,
    .sectorRead         = USB_HOST_SCSI_SectorRead,
    .sectorWrite        = USB_HOST_SCSI_SectorWrite,
    .eventHandlerset    = USB_HOST_SCSI_EventHandlerSet,
    .open               = USB_HOST_SCSI_Open,
    .close              = USB_HOST_SCSI_Close,
    .tasks              = NULL
};

// ******************************************************************************
// ******************************************************************************
// Local Functions
// ******************************************************************************
// ******************************************************************************

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_Transfer 
    (
        USB_HOST_MSD_LUN_HANDLE lunHandle,
        USB_HOST_MSD_TRANSFER_HANDLE * transferHandle,
        uint32_t startSector
        uint32_t nSectors
        void * buffer
        USB_HOST_MSD_TRANSFER_DIRECTION direction
    )

  Summary:
    This function performs a block read or block write.

  Description:
    This function performs a block read or block write.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_Transfer 
(
    USB_HOST_SCSI_HANDLE scsiHandle,
    USB_HOST_MSD_TRANSFER_HANDLE * transferHandle,
    uint32_t startSector,
    uint32_t nSectors,
    void * buffer,
    USB_HOST_MSD_TRANSFER_DIRECTION direction
)
{
    USB_HOST_SCSI_OBJ * scsiObj;
    USB_HOST_SCSI_COMMAND_OBJ * commandObj;
    USB_HOST_MSD_RESULT result;
    USB_HOST_MSD_TRANSFER_HANDLE temp, * commandHandle;

    /* If the transfer handle parameter is NULL, we setup up a local variable to
     * temporarily store the transfer handle */

    if(transferHandle == NULL)
    {
        commandHandle = &temp;
    }
    else
    {
        commandHandle = transferHandle;
    }

    /* Validate the handle */
    if((scsiHandle == USB_HOST_SCSI_HANDLE_INVALID) || (scsiHandle == 0))
    {
        *commandHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;
    }
    else
    {
        /* The handle is actually a pointer to the scsci object */
        scsiObj = (USB_HOST_SCSI_OBJ *)(scsiHandle);

        if((scsiObj->inUse) && 
                (scsiObj->state == USB_HOST_SCSI_STATE_READY) &&
                (!scsiObj->commandObj.inUse))
        {
            /* This means the SCSI instance is valid and the command object is
             * available. Start by grabbing the command object */

            scsiObj->commandObj.inUse = true;
            commandObj = &scsiObj->commandObj;


            /* Set up the command based on direction */
            if(direction == USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST)
            {
                commandObj->cdb[0] = USB_HOST_SCSI_READ10_COMMAND;
            }
            else
            {
                commandObj->cdb[0] = USB_HOST_SCSI_WRITE10_COMMAND;
            }

            commandObj->cdb[1] = 0x00;

            /* Set up the sector address */
            commandObj->cdb[2] = (uint8_t)(startSector >> 24);
            commandObj->cdb[3] = (uint8_t)(startSector >> 16);
            commandObj->cdb[4] = (uint8_t)(startSector >> 8);
            commandObj->cdb[5] = (uint8_t)(startSector);

            /* The number of sectors to read or write */
            commandObj->cdb[6] = 0x00;
            commandObj->cdb[7] = (uint8_t)(nSectors >> 8);
            commandObj->cdb[8] = (uint8_t)(nSectors);
            commandObj->cdb[9] = 0x00;

            /* Setting the generateEvent flag to true will cause the driver to
             * generate a client event. */
            commandObj->generateEvent = true;

            /* Schedule the transfer */
            result = USB_HOST_MSD_Transfer(scsiObj->lunHandle, 
                    scsiObj->commandObj.cdb, 0x0A, buffer , (nSectors * 0x0200), 
                    direction, _USB_HOST_SCSI_TransferCallback, (uintptr_t)(scsiObj));

            /* Update the transfer handle */
            if(result == USB_HOST_MSD_RESULT_SUCCESS)
            {
                /* Command was successful. Return a valid handle */
                *commandHandle = (SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE)(commandObj);
            }
            else
            {
                /* The request failed. Return the command object continue
                 * and return an invalid handle */

                *commandHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;
                commandObj->inUse = false;
                commandObj->generateEvent = false;
            }
        }
        else
        {
            /* The specified SCSI handle is not valid */
            *commandHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;
        }
    }
}

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_TransferCallback 
    (
        USB_HOST_MSD_LUN_HANDLE lunHandle,
        USB_HOST_MSD_TRANSFER_HANDLE transferHandle,
        USB_HOST_MSD_RESULT result,
        size_t size,
        uintptr_t context
    )

  Summary:
    This function is called when a MSD transfer has completed.

  Description:
    This function is called when a MSD transfer has completed.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_TransferCallback 
(
    USB_HOST_MSD_LUN_HANDLE lunHandle,
    USB_HOST_MSD_TRANSFER_HANDLE transferHandle,
    USB_HOST_MSD_RESULT result,
    size_t size,
    uintptr_t context
)
{
    int scsiObjIndex;
    USB_HOST_SCSI_OBJ * scsiObj;
    USB_HOST_SCSI_COMMAND_OBJ * commandObj;
    USB_HOST_SCSI_EVENT event;

    /* Get the SCSI object index from the lunHandle */
    scsiObjIndex = _USB_HOST_SCSI_LUNHandleToSCSIInstance(lunHandle);

    /* Get the pointer to the SCSI object */
    scsiObj = &gUSBHostSCSIObj[scsiObjIndex];

    /* Pointer to the command object */
    commandObj = &scsiObj->commandObj;

    /* The processed size */
    commandObj->size = size;

    /* The result of the command */
    commandObj->result = result;
    
    /* Let the main state machine know that the command is completed */
    commandObj->commandCompleted = true;

    if((commandObj->generateEvent) && (scsiObj->eventHandler != NULL))
    {
        /* This means we should generate an event to the SCSI driver client.
         * Evaluate the result to understand which event to generate */

        if((result == USB_HOST_MSD_RESULT_SUCCESS) || (result == USB_HOST_MSD_RESULT_COMMAND_PASSED))
        {
            /* The command passed */
            event = USB_HOST_SCSI_EVENT_COMMAND_COMPLETE;
        }
        else
        {
            /* The command failed */
            event = USB_HOST_SCSI_EVENT_COMMAND_ERROR;
        }

        if(scsiObj->eventHandler != NULL)
        {
            /* Generate the event */
            (scsiObj->eventHandler)(event, (USB_HOST_SCSI_COMMAND_HANDLE)(commandObj), scsiObj->context);
        }
        
    }

    /* Release the command object */
    commandObj->inUse = false;
    
}

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_TestUnitReadyCommand 
    (
        USB_HOST_MSD_COMMAND * msdCommand
    )

  Summary:
    Sets up the Test Unit Ready command.

  Description:
    This function sets up the Test Unit Ready Command.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_TestUnitReadyCommand (uint8_t * scsiCommand )
{
    /* inquiry device Information */
    scsiCommand[0] = USB_SCSI_TEST_UNIT_READY;
    scsiCommand[1] = 0x00;
    scsiCommand[2] = 0x00;
    scsiCommand[3] = 0x00;
    scsiCommand[4] = 0x00;
    scsiCommand[5] = 0x00;

}

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_RequestSenseCommand 
    (
        USB_HOST_MSD_COMMAND * msdCommand
    )

  Summary:
    Sets up the Request Sense command.

  Description:
    This function sets up the Request Sense Command.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_RequestSenseCommand (uint8_t * scsiCommand )
{
    /* inquiry device Information */
    scsiCommand[0] = USB_SCSI_REQUEST_SENSE;
    scsiCommand[1] = 0x00;
    scsiCommand[2] = 0x00;
    scsiCommand[3] = 0x00;
    scsiCommand[4] = 0xFF;
    scsiCommand[5] = 0x00;
}

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_ReadCapacityCommand
    (
        USB_HOST_MSD_COMMAND * msdCommand
    )

  Summary:
    Sets up the Read Capacity Command.

  Description:
    This function sets up the Read Capacity Command.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_ReadCapacityCommand (uint8_t * scsiCommand )
{
    /* inquiry device Information */
    scsiCommand[0] = USB_SCSI_READ_CAPACITY;
    scsiCommand[1] = 0x00;
    scsiCommand[2] = 0x00;
    scsiCommand[3] = 0x00;
    scsiCommand[4] = 0x00;
    scsiCommand[5] = 0x00;
    scsiCommand[6] = 0x00;
    scsiCommand[7] = 0x00;
    scsiCommand[8] = 0x00;
    scsiCommand[9] = 0x00;
}

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_InquiryResponseCommand 
    (
        USB_HOST_MSD_COMMAND * msdCommand
    )

  Summary:
    Sets up the SCSI Inquiry Response command.

  Description:
    This function sets up the SCSI Inquiry Response Command.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_InquiryResponseCommand (uint8_t * scsiCommand )
{
    /* inquiry device Information */
    scsiCommand[0] = USB_SCSI_INQUIRY;
    scsiCommand[1] = 0x00;
    scsiCommand[2] = 0x00;
    scsiCommand[3] = 0x00;
    scsiCommand[4] = 0x24;
    scsiCommand[5] = 0x00;
}

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_ReadFormatCapacityCommand
    (
        USB_HOST_MSD_COMMAND * msdCommand
    )

  Summary:
    Sets up the SCSI Read Format Capacity command.

  Description:
    This function sets up the SCSI Read Format Capacity Command.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_ReadFormatCapacityCommand (uint8_t * scsiCommand )
{
    /* inquiry device Information */
    scsiCommand[0] = USB_SCSI_READ_FORMAT_CAPACITY;
    scsiCommand[1] = 0x00;
    scsiCommand[2] = 0x00;
    scsiCommand[3] = 0x00;
    scsiCommand[4] = 0xFF;
    scsiCommand[5] = 0x00;
}

// ******************************************************************************
/* Function:
    int _USB_HOST_SCSI_LUNHandleToSCSIInstance 
    (
        USB_HOST_MSD_LUN_HANDLE lunHandle
    )

  Summary:
    This function maps a LUN Handle to the SCSI instance.

  Description:
    This function maps a LUN Handle to the SCSI instance. It returns the index
    of the SCSI object. It will return -1 if the object is not found.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

int _USB_HOST_SCSI_LUNHandleToSCSIInstance
(
    USB_HOST_MSD_LUN_HANDLE lunHandle
)
{
    int iterator;
    int result = -1;
    
    for(iterator = 0;iterator < USB_HOST_MSD_LUN_NUMBERS; iterator ++)
    {
        if(gUSBHostSCSIObj[iterator].inUse)
        {
            if(gUSBHostSCSIObj[iterator].lunHandle == lunHandle)
            {
                /* Got a match */
                result = iterator;
                break;
            }
        }
    }

    return(result);
}

// ******************************************************************************
/* Function:
    void USB_HOST_SCSI_Initialize (USB_HOST_MSD_LUN_HANDLE lunHandle)

  Summary:
    This function is called by the MSD Host Client Driver for detected LUN.

  Description:
    This function is called by the MSD Host Client Driver for detected LUN. The
    function will then register this LUN as drive with the file system media
    manager, thus allowing the media manager to access this drive.

  Remarks:
    The application will never need to call this function. This function is
    always called the USB Host MSD Client Driver.
*/

void USB_HOST_SCSI_Initialize(USB_HOST_MSD_LUN_HANDLE lunHandle)
{
    int iterator;
    USB_HOST_SCSI_OBJ * scsiObj = NULL;

    /* Search for a free SCSI object */
    for(iterator = 0; iterator < USB_HOST_MSD_LUN_NUMBERS; iterator ++)
    {
        if(!gUSBHostSCSIObj[iterator].inUse)
        {
            scsiObj = &gUSBHostSCSIObj[iterator];
            
            /* Clear this object */
            memset(scsiObj, 0, sizeof(USB_HOST_SCSI_OBJ));
            
            /* Setup the rest of the object */
            scsiObj->inUse = true;
            scsiObj->lunHandle = lunHandle;
            scsiObj->buffer = &gUSBSCSIBuffer[iterator][0];
            scsiObj->state = USB_HOST_SCSI_STATE_INQUIRY_RESPONSE;
            break;
        }
    }
}

// ******************************************************************************
/* Function:
    void USB_HOST_SCSI_DeInitialize (USB_HOST_MSD_LUN_HANDLE lunHandle)

  Summary:
    This function is called by the MSD Host Client Driver for detached LUN.

  Description:
    This function is called by the MSD Host Client Driver for detached LUN. The
    function will then deregister this LUN from the file system media manager.

  Remarks:
    The application will never need to call this function. This function is
    always called the USB Host MSD Client Driver.
*/

void USB_HOST_SCSI_Deinitialize(USB_HOST_MSD_LUN_HANDLE lunHandle)
{
    USB_HOST_SCSI_OBJ * scsiObj;
    int scsiObjIndex;

    /* Find the SCSI object that owns this LUN */
    scsiObjIndex = _USB_HOST_SCSI_LUNHandleToSCSIInstance(lunHandle);

    if(scsiObjIndex >= 0)
    {
        /* Valid index. Get the pointer to the SCSI object */
        scsiObj = &gUSBHostSCSIObj[scsiObjIndex];
        
        /* Here we check if a SCSI transfer was in progress. It is possible that
         * SCSI driver would have received a request from the file system but
         * the device got detached before the SCSI driver could submit this 
         * request to the MSD Host Client driver. In such a case, the IRP would
         * not be scheduled and hence the SCSI driver would not get a transfer
         * failure callback when the underlying pipes are closed. The SCSI
         * command object in such case would still indicate that a command was
         * requested by the file system. In such a case we should let the file
         * system know that command has failed */
        
        if(scsiObj->commandObj.inUse)
        {
            /* This means a command was requested */
            
            if(scsiObj->eventHandler != NULL)
            {
                /* Generate the error event */
                (scsiObj->eventHandler)(USB_HOST_SCSI_EVENT_COMMAND_ERROR, 
                        (USB_HOST_SCSI_COMMAND_HANDLE)(&scsiObj->commandObj), scsiObj->context);
            }
            
        }

        /* Deregister from the file system */
        SYS_FS_MEDIA_MANAGER_DeRegister(scsiObj->fsHandle);

        /* Set the state to not ready */
        scsiObj->state = USB_HOST_SCSI_STATE_NOT_READY;

        /* Release this object back */
        scsiObj->inUse = false;
    }
}

// ******************************************************************************
/* Function:
    void USB_HOST_SCSI_Tasks (USB_HOST_MSD_LUN_HANDLE lunHandle)

  Summary:
    This function is called by the MSD Host Client Driver to update the driver
    state machine.

  Description:
    This function is called by the MSD Host Client Driver to update the driver
    state machine.

  Remarks:
    The application will never need to call this function. This function is
    always called the USB Host MSD Client Driver.
*/

void USB_HOST_SCSI_Tasks(USB_HOST_MSD_LUN_HANDLE lunHandle)
{
    int scsiObjIndex;
    USB_HOST_SCSI_OBJ * scsiObj;
    USB_HOST_MSD_RESULT result;

    /* Find the SCSI object that own this LUN */
    scsiObjIndex = _USB_HOST_SCSI_LUNHandleToSCSIInstance(lunHandle);

    if(scsiObjIndex >= 0)
    {
        /* We have found a SCSI object */
        scsiObj = &gUSBHostSCSIObj[scsiObjIndex];

        switch(scsiObj->state)
        {
            case USB_HOST_SCSI_STATE_NOT_READY:
                /* We are not ready to do anything yet */
                break;
                
            case USB_HOST_SCSI_STATE_START_TEST_UNIT_READY:

                /* We first make sure that media is inserted and ready */
               _USB_HOST_SCSI_TestUnitReadyCommand(scsiObj->commandObj.cdb);

                /* Send the command. The commandCompleted flag will be update in
                 * the event handler  */
                scsiObj->commandObj.inUse = true;
                scsiObj->commandObj.commandCompleted = false;
                scsiObj->commandObj.generateEvent = false;

                result = USB_HOST_MSD_Transfer(scsiObj->lunHandle, 
                        scsiObj->commandObj.cdb, 6, NULL, 0, 
                        USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST,
                        _USB_HOST_SCSI_TransferCallback, (uintptr_t)(scsiObj));

                if(result == USB_HOST_MSD_RESULT_SUCCESS)
                {
                    /* The request was scheduled successfully. Wait for the
                     * request to compelete */

                    scsiObj->state = USB_HOST_SCSI_STATE_WAIT_START_TEST_UNIT_READY;
                }

                break;

            case USB_HOST_SCSI_STATE_WAIT_START_TEST_UNIT_READY:

                /* Here we are waiting for the test unit command to complete */

                if(scsiObj->commandObj.commandCompleted)
                {
                    /* The status of the test unit ready command can be read
                     * from the CSW */

                    if(scsiObj->commandObj.result == USB_HOST_MSD_RESULT_COMMAND_PASSED)
                    {
                        /* We are ready for next state */
                        scsiObj->state = USB_HOST_SCSI_STATE_INQUIRY_RESPONSE;
                    }
                    else
                    {
                        /* Send the test unit ready again */
                        scsiObj->state = USB_HOST_SCSI_STATE_START_TEST_UNIT_READY;
                    }
                }

                break;

            case USB_HOST_SCSI_STATE_INQUIRY_RESPONSE:

                /* We get the SCSI Enquiry response. Although there isn't much
                 * that we can do with this data */
                _USB_HOST_SCSI_InquiryResponseCommand(scsiObj->commandObj.cdb);

                /* The commandCompleted flag will be updated in the callback.
                 * Update the state and send the command.   */
                scsiObj->commandObj.inUse = true;
                scsiObj->commandObj.commandCompleted = false;
                scsiObj->commandObj.generateEvent = false;

                result = USB_HOST_MSD_Transfer(scsiObj->lunHandle, 
                        scsiObj->commandObj.cdb, 6, scsiObj->buffer, 36, 
                        USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST,
                        _USB_HOST_SCSI_TransferCallback, (uintptr_t)(scsiObj));

                if(result == USB_HOST_MSD_RESULT_SUCCESS)
                {
                    scsiObj->state = USB_HOST_SCSI_STATE_WAIT_INQUIRY_RESPONSE;
                }

                break;

            case USB_HOST_SCSI_STATE_WAIT_INQUIRY_RESPONSE:

                /* Here we wait for the Inquiry Response */
                if(scsiObj->commandObj.commandCompleted)
                {
                    /* This means the command has completed. We can launch the
                     * next command which is the Read Format capacities command
                     * */
                    scsiObj->state = USB_HOST_SCSI_STATE_READ_FORMAT_CAPACITY;
                }
                break;

            case USB_HOST_SCSI_STATE_READ_FORMAT_CAPACITY:

                /* Here we send the Read Format Capacity Command */

                _USB_HOST_SCSI_ReadFormatCapacityCommand(scsiObj->commandObj.cdb);

                /* The commandCompleted flag will be updated in the callback.
                 * Update the state and send the command.   */
                scsiObj->commandObj.inUse = true;
                scsiObj->commandObj.commandCompleted = false;
                scsiObj->commandObj.generateEvent = false;

                result = USB_HOST_MSD_Transfer(scsiObj->lunHandle, 
                        scsiObj->commandObj.cdb, 6, scsiObj->buffer, 256, 
                        USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST,
                        _USB_HOST_SCSI_TransferCallback, (uintptr_t)(scsiObj));

                if(result == USB_HOST_MSD_RESULT_SUCCESS)
                {
                    /* Advance to the next state only if the request was placed
                     * successfully. */
                    scsiObj->state = USB_HOST_SCSI_STATE_WAIT_READ_FORMAT_CAPACITY;
                }

                break;

            case USB_HOST_SCSI_STATE_WAIT_READ_FORMAT_CAPACITY:

                /* Waiting for the read format capacity command to complete */
                if(scsiObj->commandObj.commandCompleted)
                {
                    /* While we are not specifically interested in the read
                     * format capacity data at this point, we need to check if
                     * an error has occured. If so we should issues an request
                     * sense command to move the unit out of sense mode */

                    if(scsiObj->commandObj.result != USB_HOST_MSD_RESULT_COMMAND_PASSED)
                    {
                        /* Send a request sense command. We should also specify
                         * what is the next thing to do after request sense is
                         * complete */

                        scsiObj->requestSenseNextState = USB_HOST_SCSI_STATE_READ_CAPACITY;
                        scsiObj->state = USB_HOST_SCSI_STATE_REQUEST_SENSE;

                    }
                    else if(scsiObj->commandObj.result == USB_HOST_MSD_RESULT_COMMAND_PASSED)
                    {
                        /* The read format capacity command worked. Read the
                         * disk capacity. */
                        scsiObj->state = USB_HOST_SCSI_STATE_READ_CAPACITY;
                    }
                }
                break;
                
            case USB_HOST_SCSI_STATE_REQUEST_SENSE:

                /* In this state, we send the request sense command */

                _USB_HOST_SCSI_RequestSenseCommand(scsiObj->commandObj.cdb);

                 /* The commandCompleted flag will be updated in the callback.
                 * Update the state and send the command.   */
                scsiObj->commandObj.inUse = true;
                scsiObj->commandObj.commandCompleted = false;
                scsiObj->commandObj.generateEvent = false;

                /* Send the command */
                result = USB_HOST_MSD_Transfer(scsiObj->lunHandle, 
                        scsiObj->commandObj.cdb, 0x6, scsiObj->buffer, 256 , 
                        USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST,
                        _USB_HOST_SCSI_TransferCallback, (uintptr_t)(scsiObj));

                if(result == USB_HOST_MSD_RESULT_SUCCESS)
                {
                    /* Go to the next state only if the request was placed
                     * successfully. */
                    scsiObj->state = USB_HOST_SCSI_STATE_WAIT_REQUEST_SENSE;
                }

                break;

            case USB_HOST_SCSI_STATE_WAIT_REQUEST_SENSE:

                /* In this state we wait for the command to complete */

                if(scsiObj->commandObj.commandCompleted)
                {
                    if((scsiObj->commandObj.result == USB_HOST_MSD_RESULT_COMMAND_PASSED) ||
                            (scsiObj->commandObj.result == USB_HOST_MSD_RESULT_COMMAND_STALLED))
                    {
                        /* Either the command was successful or was stalled.
                         * The media will stall the mode sense command if there 
                         * was no sense data to send. Set the state to the next 
                         * state */
                        scsiObj->state = scsiObj->requestSenseNextState;
                    }
                    else
                    {
                        /* The SCSI Request Sense command must pass */
                        scsiObj->state = USB_HOST_SCSI_STATE_ERROR;
                    }
                }

                break;

            case USB_HOST_SCSI_STATE_READ_CAPACITY:

                /* Here we send the read capacity command */
                _USB_HOST_SCSI_ReadCapacityCommand(scsiObj->commandObj.cdb);

                /* The commandCompleted flag will be updated in the callback.
                 * Update the state and send the command.   */
                scsiObj->commandObj.inUse = true;
                scsiObj->commandObj.commandCompleted = false;
                scsiObj->commandObj.generateEvent = false;

                result = USB_HOST_MSD_Transfer(scsiObj->lunHandle, 
                        scsiObj->commandObj.cdb, 0xA, scsiObj->buffer, 0x8 , 
                        USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST,
                        _USB_HOST_SCSI_TransferCallback, (uintptr_t)(scsiObj));

                if(result == USB_HOST_MSD_RESULT_SUCCESS)
                {
                    /* Go to the next state only if the request was placed
                     * successfully. */
                    scsiObj->state = USB_HOST_SCSI_STATE_WAIT_READ_CAPACITY;
                }

                break;

            case USB_HOST_SCSI_STATE_WAIT_READ_CAPACITY:

                /* Here we wait for the read capacity to complete */
                if(scsiObj->commandObj.commandCompleted)
                {
                    if(scsiObj->commandObj.result == USB_HOST_MSD_RESULT_COMMAND_PASSED)
                    {
                        /* We have the capacity information. Update the geometry
                         * object */

                        uint8_t * buffer = scsiObj->buffer;

                        /* The read and write will be blocking */
                        scsiObj->mediaGeometry.mediaProperty = (SYS_FS_MEDIA_WRITE_IS_BLOCKING|SYS_FS_MEDIA_READ_IS_BLOCKING);

                        /* There is one read, write and erase region */
                        scsiObj->mediaGeometry.numReadRegions = 1;
                        scsiObj->mediaGeometry.numWriteRegions = 1;
                        scsiObj->mediaGeometry.numEraseRegions = 1;

                        /* The size of the read region and size of the read block */
                        scsiObj->mediaRegionGeometry[0].blockSize = (buffer[7])|(buffer[6] << 8)|(buffer[5]<<16)|
                            (buffer[4] << 24);
                        scsiObj->mediaRegionGeometry[0].numBlocks = ((buffer[3])|(buffer[2] << 8)|(buffer[1]<<16)|
                                (buffer[0] << 24)) + 1;

                        /* The size of the write region and size of the read block */
                        scsiObj->mediaRegionGeometry[1].blockSize = scsiObj->mediaRegionGeometry[0].blockSize;
                        scsiObj->mediaRegionGeometry[1].numBlocks = scsiObj->mediaRegionGeometry[0].numBlocks;

                        /* The size of the erase region and size of the read block */
                        scsiObj->mediaRegionGeometry[2].blockSize = scsiObj->mediaRegionGeometry[0].blockSize;
                        scsiObj->mediaRegionGeometry[2].numBlocks = scsiObj->mediaRegionGeometry[0].numBlocks;

                        /* Assing the region specific geometry table */
                        scsiObj->mediaGeometry.geometryTable = scsiObj->mediaRegionGeometry;

                        /* Now we can check if the unit is ready for read and writes
                         * */
                        scsiObj->state = USB_HOST_SCSI_STATE_TEST_UNIT_READY;
                    }
                    else
                    {
                        /* The Read capacity command failed. We cannot proceed
                         * unless this command succeeeds. Try again */

                        scsiObj->state = USB_HOST_SCSI_STATE_READ_CAPACITY;
                    }
                }
                break;

            case USB_HOST_SCSI_STATE_TEST_UNIT_READY:

                /* In this state the driver will check if the device is ready.
                 * Grab the command object. Populate the command block with
                 * command */
                _USB_HOST_SCSI_TestUnitReadyCommand(scsiObj->commandObj.cdb);

                /* Send the command. The commandCompleted flag will be update in
                 * the event handler  */
                scsiObj->commandObj.inUse = true;
                scsiObj->commandObj.commandCompleted = false;
                scsiObj->commandObj.generateEvent = false;

                result = USB_HOST_MSD_Transfer(scsiObj->lunHandle, 
                        scsiObj->commandObj.cdb, 6, NULL, 0, 
                        USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST,
                        _USB_HOST_SCSI_TransferCallback, (uintptr_t)(scsiObj));

                if(result == USB_HOST_MSD_RESULT_SUCCESS)
                {
                    /* The request was scheduled successfully. Wait for the
                     * request to compelete */

                    scsiObj->state = USB_HOST_SCSI_STATE_WAIT_TEST_UNIT_READY;
                }

                break;

            case USB_HOST_SCSI_STATE_WAIT_TEST_UNIT_READY:

                /* Here we are waiting for the test unit command to complete */

                if(scsiObj->commandObj.commandCompleted)
                {
                    /* The status of the test unit ready command can be read
                     * from the CSW */

                    if(scsiObj->commandObj.result == USB_HOST_MSD_RESULT_COMMAND_PASSED)
                    {
                        /* We are ready for read write */
                        scsiObj->state = USB_HOST_SCSI_STATE_READY;

                        /* The unit is ready. Register with the file system */
                        scsiObj->fsHandle = SYS_FS_MEDIA_MANAGER_Register((SYS_MODULE_OBJ)NULL, scsiObjIndex, 
                                &gUSBHostMSDSCSIMediaFunctions, SYS_FS_MEDIA_TYPE_MSD);
                    }
                    else
                    {
                        /* Send the test unit ready again */
                        scsiObj->state = USB_HOST_SCSI_STATE_TEST_UNIT_READY;
                    }
                }
                
                break;

            default:
                break;
        }
    }

}

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

  Remarks:
    This function is typically called by the file system media manager.
*/

USB_HOST_SCSI_HANDLE USB_HOST_SCSI_Open 
(
    SYS_MODULE_INDEX index, 
    DRV_IO_INTENT intent
)
{
    USB_HOST_SCSI_OBJ * scsiObj;
    USB_HOST_SCSI_HANDLE result;

    if(index >= USB_HOST_MSD_LUN_NUMBERS)
    {
        /* Invalid driver index */
        result = USB_HOST_SCSI_HANDLE_INVALID;
    }
    else
    {
        scsiObj = &gUSBHostSCSIObj[index];
        if((scsiObj->inUse) && (scsiObj->state == USB_HOST_SCSI_STATE_READY))
        {
            /* This means this object is ready for use */
            result = (USB_HOST_SCSI_HANDLE)(scsiObj);
        }
        else
        {
            /* Not ready to be opened */
            result = USB_HOST_SCSI_HANDLE_INVALID;
        }
    }

    return(result);
}

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

  Remarks:
    This function is typically called by the file system media manager.
*/

void USB_HOST_SCSI_Close 
(
    USB_HOST_SCSI_HANDLE scsiHandle
)
{
    /* This function is not implemented in this release of the driver */
}

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

  Remarks:
    This function is typically called by the file system media manager.
*/

bool USB_HOST_SCSI_MediaStatusGet 
(
    USB_HOST_SCSI_HANDLE scsiHandle
)
{
    USB_HOST_SCSI_OBJ * scsiObj;
    bool result = false;

    /* Validate the SCSI handle */
    if((USB_HOST_SCSI_HANDLE_INVALID != scsiHandle) &&
            (0 != scsiHandle))
    {
        /* The handle is actually a pointer to the SCSI object */
        scsiObj = (USB_HOST_SCSI_OBJ *)scsiHandle;

        if((scsiObj->inUse) && (scsiObj->state == USB_HOST_SCSI_STATE_READY))
        {
            /* Device is attached and ready to use */
            result = true;
        }
    }

    return(result);
}

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
)
{
     /* Perform a read transfer */
    _USB_HOST_SCSI_Transfer(scsiHandle, commandHandle,  
            blockStart, nBlock, buffer, USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST);
}

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
)
{
    /* Perform a write transfer */
    _USB_HOST_SCSI_Transfer(scsiHandle, commandHandle,  
            blockStart, nBlock, buffer, USB_HOST_MSD_TRANSFER_DIRECTION_HOST_TO_DEVICE);
}

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

  Remarks:
    This function is typically called by the file system media manager.
*/

void USB_HOST_SCSI_EventHandlerSet 
(
    USB_HOST_SCSI_HANDLE scsiHandle,
    const void * eventHandler,
    const uintptr_t context
)
{
    USB_HOST_SCSI_OBJ * scsiObj;

    if((scsiHandle != USB_HOST_SCSI_HANDLE_INVALID) && (scsiHandle != 0))
    {
        /* Set the event handler and the context */
        scsiObj = (USB_HOST_SCSI_OBJ *)(scsiHandle);
        if(scsiObj->inUse)
        {
            /* Only if the object is valid */
            scsiObj->eventHandler = eventHandler;
            scsiObj->context = context;
        }
    }
}

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

  Remarks:
    This function is typically called by the file system media manager.
*/

SYS_FS_MEDIA_GEOMETRY * USB_HOST_SCSI_MediaGeometryGet 
(
    USB_HOST_SCSI_HANDLE scsiHandle
)
{
    USB_HOST_SCSI_OBJ * scsiObj;
    SYS_FS_MEDIA_GEOMETRY * result;

    result = NULL;
    if((scsiHandle != USB_HOST_SCSI_HANDLE_INVALID) && (scsiHandle != 0))
    {
        /* Set the event handler and the context */
        scsiObj = (USB_HOST_SCSI_OBJ *)(scsiHandle);
        if((scsiObj->inUse) && (scsiObj->state == USB_HOST_SCSI_STATE_READY))
        {
            /* Only if the object is valid */
            result = &scsiObj->mediaGeometry;
        }
    }
    
    return(result);
}

