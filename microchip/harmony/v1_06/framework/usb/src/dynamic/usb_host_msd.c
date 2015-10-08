/*******************************************************************************
  USB Host Layer implemention.

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_msd.c

  Summary:
    This file contains implementations of both private and public functions
    of the USB Host MSD client driver.

  Description:
    This file contains the USB host MSD client driver implementation. This file 
    should be included in the project if USB MSD devices are to be supported.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/usb_host_msd.h"
#include "usb/src/usb_host_msd_local.h"
#include "usb/usb_host_client_driver.h"
#include "system/debug/sys_debug.h"
#include "usb/usb_host_scsi.h"
#include <p32xxxx.h>

/*************************************************
 * Driver interface that is provide to the 
 * host layer.
 *************************************************/

USB_HOST_CLIENT_DRIVER gUSBHostMSDClientDriver = 
{
    .initialize = _USB_HOST_MSD_Initialize,
    .deinitialize = _USB_HOST_MSD_Deinitialize,
    .reinitialize = _USB_HOST_MSD_Reinitialize,
    .interfaceAssign = _USB_HOST_MSD_InterfaceAssign,
    .interfaceRelease = _USB_HOST_MSD_InterfaceRelease,
    .interfaceEventHandler = _USB_HOST_MSD_InterfaceEventHandler,
    .interfaceTasks = _USB_HOST_MSD_InterfaceTasks,
    .deviceEventHandler = NULL,
    .deviceAssign = NULL,
    .deviceEventHandler = NULL,
    .deviceRelease = NULL
         
};

/**************************************************
 * Global array of MSD Instance Objects. Each for
 * one MSD device attached.
 ***************************************************/
USB_HOST_MSD_INSTANCE  gUSBHostMSDInstance[USB_HOST_MSD_INSTANCES_NUMBER];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
uint8_t gUSBHostMSDCBW[USB_HOST_MSD_INSTANCES_NUMBER][32] __attribute__((coherent)) __attribute__((aligned(4)));
uint8_t gUSBHostMSDCSW[USB_HOST_MSD_INSTANCES_NUMBER][16] __attribute__((coherent)) __attribute__((aligned(4)));

// *****************************************************************************
// *****************************************************************************
// USB Host MSD Local Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
   void _USB_HOST_MSD_TransferTasks
   (
        USB_HOST_MSD_INSTANCE * msdInstanceInfo,
        USB_HOST_RESULT result,
        size_t size
   );

  Summary:
    This function is called when a transfer event has occurred. This updates the
    transfer state of an ongoing MSD transfer.

  Description:
    This function is called when a transfer event has occurred. This updates the
    transfer state of an ongoing MSD transfer.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_TransferTasks
(
    USB_HOST_MSD_INSTANCE * msdInstanceInfo,
    USB_HOST_RESULT result,
    size_t size
)
{
    USB_HOST_TRANSFER_HANDLE transferHandle;
    USB_HOST_MSD_RESULT msdResult;
    USB_HOST_PIPE_HANDLE pipeHandle;
    USB_MSD_CSW * msdCSW;
    size_t processedBytes = 0;
    USB_HOST_MSD_TRANSFER_OBJ * transferObj;
    bool transferIsDone = false;

    if(msdInstanceInfo->msdState == USB_HOST_MSD_STATE_READY)
    {
        /* This means this transfer event is related to a regular BOT transfer
         * */

        transferObj = &msdInstanceInfo->transferObj;

        switch(msdInstanceInfo->transferState)
        {
            case USB_HOST_MSD_TRANSFER_STATE_WAIT_FOR_CBW:

                /* This means the CBW stage has completed */
                if(result == USB_HOST_RESULT_SUCCESS)
                {
                    /* Check if the command needs a data stage */
                    if(transferObj->size > 0)
                    {
                        /* Data stage is needed. Find out the direction */
                        if(transferObj->transferDirection == 
                                USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST)
                        {
                            /* We need the in pipe */
                            pipeHandle = msdInstanceInfo->bulkInPipeHandle;
                        }
                        else
                        {
                            /* Else we need the out pipe */
                            pipeHandle = msdInstanceInfo->bulkOutPipeHandle;
                        }

                        /* Update the state to indicate that we are waiting for data
                         * */
                        msdInstanceInfo->transferState = USB_HOST_MSD_TRANSFER_STATE_WAIT_FOR_DATA;

                        /* Send the transfer */
                        USB_HOST_DeviceTransfer(pipeHandle, &transferHandle, transferObj->buffer,
                                transferObj->size,(uintptr_t)(msdInstanceInfo));
                    }
                    else
                    {
                        /* Data stage is not needed. We can launch CSW */
                        msdInstanceInfo->transferState = USB_HOST_MSD_TRANSFER_STATE_WAIT_FOR_CSW;

                        /* Note that we are not checking for return value here
                         * because we are in an interrupt context. If the function
                         * fails here, there isn't much that we can do. */
                        USB_HOST_DeviceTransfer(msdInstanceInfo->bulkInPipeHandle, &transferHandle, 
                                msdInstanceInfo->msdCSW, 13, (uintptr_t)(msdInstanceInfo));
                    }
                }
                break;

            case USB_HOST_MSD_TRANSFER_STATE_WAIT_FOR_DATA:

                /* We were waiting for the data stage to complete */
                if (result == USB_HOST_RESULT_SUCCESS)
                {
                    /* We got the data stage. Go to CSW stage */
                    msdInstanceInfo->transferState = USB_HOST_MSD_TRANSFER_STATE_WAIT_FOR_CSW;

                    /* Note that we are not checking for return value here
                     * because we are in an interrupt context. If the function
                     * fails here, there isn't much that we can do. */
                    USB_HOST_DeviceTransfer(msdInstanceInfo->bulkInPipeHandle, &transferHandle, 
                            msdInstanceInfo->msdCSW, 13, (uintptr_t)(msdInstanceInfo));
                }
                else if(result == USB_HOST_RESULT_REQUEST_STALLED)
                {
                    /* This command was stalled. End the transfer */
                    transferIsDone = true;
                    msdResult = _USB_HOST_MSD_HostResultToMSDResultMap(result);

                    /* Move the MSD control transfer state machine to clear the
                     * stall on the endpoint. We must figure which endpoint stall to
                     * clear. The MSD control state machine should have be in a
                     * ready state else we would not have been here in the first
                     * place */

                    if(transferObj->transferDirection == USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST)
                    {
                        /* Clear the halt on the in pipe */
                        msdInstanceInfo->msdState = USB_HOST_MSD_STATE_IN_PIPE_HALT_CLEAR;
                    }
                    else
                    {
                        /* Clear the halt on the out pipe */
                        msdInstanceInfo->msdState = USB_HOST_MSD_STATE_OUT_PIPE_HALT_CLEAR;
                    }
                }
                else
                {
                    /* End the command. This is an error that we cannot handle */
                    transferIsDone = true;
                    msdResult = _USB_HOST_MSD_HostResultToMSDResultMap(result);
                }

                break;

            case USB_HOST_MSD_TRANSFER_STATE_WAIT_FOR_CSW:
                /* This means the CSW has completed. Check if we have meaningful CSW */

                msdCSW = msdInstanceInfo->msdCSW;

                if((result == USB_HOST_RESULT_SUCCESS) && 
                        (msdCSW->dCSWSignature == USB_MSD_VALID_CSW_SIGNATURE) &&
                        (msdCSW->dCSWTag == USB_MSD_VALID_CBW_TAG))
                {
                    /* The CSW itself is valid. The status field can be mapped
                     * directly to the CSW status */
                    msdResult = msdCSW->bCSWStatus;
                    processedBytes = msdInstanceInfo->transferObj.size - msdCSW->dCSWDataResidue;
                }
                else if(result == USB_HOST_RESULT_REQUEST_STALLED)
                {
                    /* The CSW request got stalled ! Lets try to clear the IN
                     * pipe stall condition */
                    transferIsDone = true;
                    msdResult = _USB_HOST_MSD_HostResultToMSDResultMap(result);
                    msdInstanceInfo->msdState = USB_HOST_MSD_STATE_IN_PIPE_HALT_CLEAR;

                }
                else
                {
                    /* Either the transfer failed or the CSW is not valid. Either
                     * way we cannot do anything. Map the result to MSD result */
                    msdResult = _USB_HOST_MSD_HostResultToMSDResultMap(result);
                    processedBytes = 0;
                }

                /* End the transfer */
                transferIsDone = true;
                break;

            default:
                break;
        }

        if(transferIsDone)
        {
            if(msdInstanceInfo->transferObj.callback != NULL)
            {
                /* Let the caller who initiated the command know that this is
                 * done */
                msdInstanceInfo->transferObj.callback(msdInstanceInfo->transferObj.lunHandle,
                        (USB_HOST_MSD_TRANSFER_HANDLE)(&msdInstanceInfo->transferObj), 
                        msdResult, processedBytes, 
                        msdInstanceInfo->transferObj.context);
            }

            /* Return the transfer object back */
            msdInstanceInfo->transferObj.inUse = false;

            /* Make the transfer state ready for another transfer */
            msdInstanceInfo->transferState = USB_HOST_MSD_TRANSFER_STATE_READY;
        }
    }
    else
    {
        /* The MSD host state is not in a regular state. It is trying to clear
         * up a stall or performing error handling. */

        if(msdInstanceInfo->msdState == USB_HOST_MSD_STATE_WAIT_FOR_POST_PIPE_HALT_CLEAR_CSW)
        {
            /* This means the MSD Host had launched a post pipe halt clear CSW
             * and that CSW has arrived. At this point we dont do anything. Lets
             * try to continue by putting the MSD Host state back into ready */

            msdInstanceInfo->msdState = USB_HOST_MSD_STATE_READY;
        }
    }
}

// *****************************************************************************
/* Function:
   USB_HOST_MSD_RESULT _USB_HOST_MSD_HostResultToMSDResultMap
   (
        USB_HOST_RESULT result
   )

  Summary:
    Maps USB_HOST_RESULT to USB_HOST_MSD_RESULT.

  Description:
    Maps USB_HOST_RESULT to USB_HOST_MSD_RESULT.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_HOST_MSD_RESULT _USB_HOST_MSD_HostResultToMSDResultMap
(
    USB_HOST_RESULT result
)
{
    USB_HOST_MSD_RESULT msdResult;

    switch(result)
    {
        case USB_HOST_RESULT_SUCCESS:
            msdResult = USB_HOST_MSD_RESULT_SUCCESS;
            break;
        case USB_HOST_RESULT_FAILURE:
            /* Note the fall through here. This is intentional */
        case USB_HOST_RESULT_PARAMETER_INVALID:
        case USB_HOST_RESULT_TRANSFER_ABORTED:
        case USB_HOST_RESULT_PIPE_HANDLE_INVALID:
            msdResult = USB_HOST_MSD_RESULT_FAILURE;
            break;
        case USB_HOST_RESULT_REQUEST_BUSY:
            msdResult = USB_HOST_MSD_RESULT_BUSY;
            break;
        case USB_HOST_RESULT_REQUEST_STALLED:
            msdResult = USB_HOST_MSD_RESULT_COMMAND_STALLED;
            break;
        default:
            msdResult = USB_HOST_MSD_RESULT_FAILURE;
            break;
    }

    return(msdResult);
}

// *****************************************************************************
/* Function:
   int_USB_HOST_MSD_InterfaceHandleToMSDInstance
   ( 
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
   )

  Summary:
    This function will return the MSD instance object that is associated with
    this interface.

  Description:
    This function will return the MSD instance object that is associated with
    this interface.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

int _USB_HOST_MSD_InterfaceHandleToMSDInstance
( 
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)
{
    int iterator;
    int msdInstanceInfo = -1;

    /* Find the MSD Instance object that owns this interface */
    for (iterator = 0; iterator < USB_HOST_MSD_INSTANCES_NUMBER; iterator ++)
    {
        if(gUSBHostMSDInstance[iterator].assigned)
        {
            if(gUSBHostMSDInstance[iterator].interfaceHandle == interfaceHandle)
            {
                /* Found it */
                msdInstanceInfo = iterator;
                break;
            }
        }
    }
    return(msdInstanceInfo);
}

// *****************************************************************************
/* Function:
   void _USB_HOST_MSD_InstanceRelease
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
    );

  Summary:
    This function will release the device interface. It will close any open
    pipes. It will deinitialize any SCSI instances.

  Description:
    This function will release the device interface. It will close any open
    pipes. It will deinitialize any SCSI instances.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_InstanceRelease
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)
{
    int msdInstanceIndex;
    int iterator;
    USB_HOST_MSD_INSTANCE * msdInstanceInfo;

    /* Find the MSD instance for this interface */
    msdInstanceIndex = _USB_HOST_MSD_InterfaceHandleToMSDInstance(interfaceHandle);

    if(msdInstanceIndex >= 0)
    {
        /* Get the object pointer */
        msdInstanceInfo = &(gUSBHostMSDInstance[msdInstanceIndex]);
        
        /* Close any open pipes */
        if(msdInstanceInfo->bulkOutPipeHandle != USB_HOST_PIPE_HANDLE_INVALID)
        {
            /* Close the pipe */
            USB_HOST_DevicePipeClose(msdInstanceInfo->bulkOutPipeHandle);
            msdInstanceInfo->bulkOutPipeHandle = USB_HOST_PIPE_HANDLE_INVALID;
        }

        if(msdInstanceInfo->bulkInPipeHandle != USB_HOST_PIPE_HANDLE_INVALID)
        {
            /* Close the pipe */
            USB_HOST_DevicePipeClose(msdInstanceInfo->bulkInPipeHandle);
            msdInstanceInfo->bulkInPipeHandle = USB_HOST_PIPE_HANDLE_INVALID;
        }
        
        /* The interface is release after the pipes are closed. This will allow
         * the IRP callback functions to be valid when the pipes are closed
         * in the statement above. */
        
        USB_HOST_DeviceInterfaceRelease(interfaceHandle);

        if(msdInstanceInfo->msdState == USB_HOST_MSD_STATE_READY)
        {
            /* This means we should also deinitialize the SCSI drivers for each
             * LUN */
            for(iterator = 0; iterator < msdInstanceInfo->logicalUnitNumber + 1; iterator ++)
            {
                USB_HOST_SCSI_Deinitialize(USB_HOST_MSD_LUNHandleGet(iterator, msdInstanceIndex));
            }
        }

        msdInstanceInfo->assigned = false;
    }
}

// *****************************************************************************
/* Function:
   void _USB_HOST_MSD_ControlTransferCallback
    (
        USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
        USB_HOST_REQUEST_HANDLE requestHandle,
        USB_HOST_RESULT result,
        size_t size,
        uintptr_t context
    );

  Summary:
    This function is called when a control transfer completes.

  Description:
    This function is called when a control transfer completes.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_ControlTransferCallback
(
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    USB_HOST_REQUEST_HANDLE requestHandle,
    USB_HOST_RESULT result,
    size_t size,
    uintptr_t context
)
{
    USB_HOST_MSD_INSTANCE * msdInstanceInfo;
    
    /* The context will be a pointer to the MSD instance */
    msdInstanceInfo = (USB_HOST_MSD_INSTANCE *)(context);
    
    /* Update the request object with the result */
    msdInstanceInfo->requestObj.result = result;
    msdInstanceInfo->requestObj.size = size;
    msdInstanceInfo->requestObj.controlRequestDone = true; 
}

// *****************************************************************************
/* Function:
   void _USB_HOST_MSD_GetMaxLUNPacketCreate
   ( 
       USB_HOST_MSD_REQUEST * requestObj,
       uint8_t bInterfaceNumber
   )

  Summary:
    This function will create the Get MAX LUN

  Description:
    This function will create the Get MAX LUN

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_GetMaxLUNPacketCreate
(
   USB_HOST_MSD_REQUEST * requestObj,
   uint8_t bInterfaceNumber
)
{
    /* Fill setup packet. The Get MAX LUN is target to the interface */
    requestObj->setupPacket.bmRequestType  = ( USB_SETUP_DIRN_DEVICE_TO_HOST |
            USB_SETUP_TYPE_CLASS |
            USB_SETUP_RECIPIENT_INTERFACE );

    /* Setup the other setup packet values */
    requestObj->setupPacket.bRequest =  USB_MSD_GET_MAX_LUN ;
    requestObj->setupPacket.wValue = 0x0000;
    requestObj->setupPacket.wIndex = bInterfaceNumber;
    requestObj->setupPacket.wLength = 0x01;
}

// *****************************************************************************
/* Function:
   void _USB_HOST_MSD_ResetPacketCreate
   ( 
       USB_HOST_MSD_REQUEST * requestObj,
       uint8_t bInterfaceNumber
   )

  Summary:
    This function will create the Reset Packet.

  Description:
    This function will create the Reset Packet.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_ResetPacketCreate
( 
   USB_HOST_MSD_REQUEST * requestObj,
   uint8_t bInterfaceNumber
)
{
    /* Create the setup packet */
    requestObj->setupPacket.bmRequestType  = ( USB_SETUP_DIRN_HOST_TO_DEVICE |
            USB_SETUP_TYPE_CLASS |
            USB_SETUP_RECIPIENT_INTERFACE ); 
    requestObj->setupPacket.bRequest =  USB_MSD_RESET ;
    requestObj->setupPacket.wValue = 0x0000;
    requestObj->setupPacket.wIndex = bInterfaceNumber;
    requestObj->setupPacket.wLength = 0x00;

}

// *****************************************************************************
/* Function:
    void _USB_HOST_MSD_Initialize(void * msdInitData)

  Summary:
    This function is called when the Host Layer is initializing.

  Description:
    This function is called when the Host Layer is initializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_Initialize(void * msdInitData)
{
    /* Make sure all the pipe handles are invalid */
    
    int iterator;
    USB_HOST_MSD_INSTANCE * msdInstanceInfo;
    
    for (iterator = 0; iterator < USB_HOST_MSD_INSTANCES_NUMBER; iterator ++)
    {
        msdInstanceInfo = &gUSBHostMSDInstance[iterator];
        msdInstanceInfo->controlPipeHandle = USB_HOST_CONTROL_PIPE_HANDLE_INVALID;
        msdInstanceInfo->bulkInPipeHandle = USB_HOST_PIPE_HANDLE_INVALID;
        msdInstanceInfo->bulkOutPipeHandle = USB_HOST_PIPE_HANDLE_INVALID;
        msdInstanceInfo->msdCBW = (USB_MSD_CBW *)(&gUSBHostMSDCBW[iterator][0]);
        msdInstanceInfo->msdCSW = (USB_MSD_CSW *)(&gUSBHostMSDCSW[iterator][0]);
    }
}

// *****************************************************************************
/* Function:
    void _USB_HOST_MSD_Deinitialize(void)

  Summary:
    This function is called when the Host Layer is deinitializing.

  Description:
    This function is called when the Host Layer is deinitializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_Deinitialize(void)
{
    /* This function is not implemented in this release of the driver */
}

// *****************************************************************************
/* Function:
    void _USB_HOST_MSD_Reinitialize(void)

  Summary:
    This function is called when the Host Layer is reinitializing.

  Description:
    This function is called when the Host Layer is reinitializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_Reinitialize(void * msdInitData)
{
    /* This function is not implemented in this release of the driver */
}

// *****************************************************************************
/* Function:
    void _USB_HOST_MSD_InterfaceAssign 
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE * interfaces,
        USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
        size_t nInterfaces,
        uint8_t * descriptor
    )

  Summary:
    This function is called when the Host Layer attaches this driver to an
    interface.

  Description:
    This function is called when the Host Layer attaches this driver to an
    interface.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_InterfaceAssign 
(
    USB_HOST_DEVICE_INTERFACE_HANDLE * interfaces,
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    size_t nInterfaces,
    uint8_t * descriptor
)
{
    int driverIndex;
    USB_HOST_MSD_INSTANCE * msdInstanceInfo = NULL;
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle;
    USB_HOST_ENDPOINT_DESCRIPTOR_QUERY endpointQuery;
    USB_ENDPOINT_DESCRIPTOR * endpointDescriptor;
    USB_INTERFACE_DESCRIPTOR * interfaceDescriptor;
    bool result = false;
    
    /* This client driver will support only one interface which is the first
     * interface in the interfaces table */
    interfaceHandle = interfaces[0];
    
    /* This function is being called because an MSD device was attached and the 
     * driver has matched. Check if there is a free MSD object. Searching for
     * free MSD object need not be mutex protected because this function will
     * always be called from one thread. */
    
    for ( driverIndex = 0 ; driverIndex < USB_HOST_MSD_INSTANCES_NUMBER ; driverIndex++ )
    {
        if (!gUSBHostMSDInstance[driverIndex].assigned)
        {
            /* Found a free instance */
            gUSBHostMSDInstance[driverIndex].assigned = true;
            msdInstanceInfo = &gUSBHostMSDInstance[driverIndex];
            msdInstanceInfo->transferObj.inUse = false;
            msdInstanceInfo->transferState = USB_HOST_MSD_TRANSFER_STATE_READY;
            break;
        }
    }
    
    if (msdInstanceInfo != NULL)
    {
        /* Save these handles */
        msdInstanceInfo->deviceObjHandle = deviceObjHandle;
        msdInstanceInfo->interfaceHandle = interfaceHandle;
        
        /* Get the interface descriptor pointer */
        interfaceDescriptor = (USB_INTERFACE_DESCRIPTOR *)(descriptor);
        msdInstanceInfo->bInterfaceNumber = interfaceDescriptor->bInterfaceNumber;
        
        /* This means that we have found an MSD instance object and this device
         * can be processed. Open a control pipe to the device.  */
        
        msdInstanceInfo->controlPipeHandle = USB_HOST_DeviceControlPipeOpen(deviceObjHandle);
        
        if(msdInstanceInfo->controlPipeHandle != USB_HOST_CONTROL_PIPE_HANDLE_INVALID)
        {
            /* Find the bulk in and bulk out endpoint. To do this, first setup the
             * endpoint descriptor query */

            USB_HOST_DeviceEndpointQueryContextClear(&endpointQuery);
            endpointQuery.flags = USB_HOST_ENDPOINT_QUERY_BY_DIRECTION|USB_HOST_ENDPOINT_QUERY_BY_TRANSFER_TYPE;
            endpointQuery.direction  = USB_DATA_DIRECTION_DEVICE_TO_HOST;
            endpointQuery.transferType = USB_TRANSFER_TYPE_BULK;
            
            /* Now find the endpoint */
            endpointDescriptor = USB_HOST_DeviceEndpointDescriptorQuery(interfaceDescriptor, &endpointQuery);
            if(endpointDescriptor != NULL)
            {
                /* We have found the IN bulk endpoint. Try opening a pipe on this 
                 * endpoint */

                msdInstanceInfo->bulkInPipeHandle = USB_HOST_DevicePipeOpen(interfaceHandle, endpointDescriptor->bEndpointAddress);
                if(msdInstanceInfo->bulkInPipeHandle != USB_HOST_PIPE_HANDLE_INVALID)
                {
                    /* Now open the the bulk out pipe */
                    
                    USB_HOST_DeviceEndpointQueryContextClear(&endpointQuery);
                    endpointQuery.flags = USB_HOST_ENDPOINT_QUERY_BY_DIRECTION|USB_HOST_ENDPOINT_QUERY_BY_TRANSFER_TYPE;
                    endpointQuery.direction  = USB_DATA_DIRECTION_HOST_TO_DEVICE;
                    endpointQuery.transferType = USB_TRANSFER_TYPE_BULK;

                    /* Now find the endpoint */
                    endpointDescriptor = USB_HOST_DeviceEndpointDescriptorQuery(interfaceDescriptor, &endpointQuery);
                    if(endpointDescriptor != NULL)
                    {
                        /* Found the bulk out endpoint. Now open the pipe */
                        msdInstanceInfo->bulkOutPipeHandle = USB_HOST_DevicePipeOpen(interfaceHandle, endpointDescriptor->bEndpointAddress);
                        if(msdInstanceInfo->bulkOutPipeHandle != USB_HOST_PIPE_HANDLE_INVALID)
                        {
                            /* All pipes open. The device can be used */
                            result = true;
                        }
                        
                    }
                    else
                    {
                        /* Could not find bulk out endpoint */
                        result = false;
                    }
                }
                else
                {
                    /* Could not open the bulk in pipe */
                    result = false;
                }
            }
            else
            {
                /* We could not find the endpoint. Cannot proceed */
                result = false;
            }
        }
        else
        {
            /* Control pipe could not be opened */
            result = false;
        }
    }
    
    if (result == false)
    {
        /* Let the host know that this interface cannot be processed */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "Could not find MSD object");
        _USB_HOST_MSD_InterfaceRelease(interfaceHandle);
    }
    else
    {
        /* The device is valid. We should launch the first control request */
        msdInstanceInfo->msdState = USB_HOST_MSD_STATE_GET_MAX_LUN;
    }

    return;
}

// *****************************************************************************
/* Function:
    void USB_HOST_MSD_InterfaceRelease
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
    )

  Summary:
    This function is called when the Host Layer detaches this driver from an
    interface.

  Description:
    This function is called when the Host Layer detaches this driver from an
    interface.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_InterfaceRelease
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)
{
    /* The device is detached or the configuration has changed. Shut down
     * everything for this interface */
    _USB_HOST_MSD_InstanceRelease(interfaceHandle);
}

// *****************************************************************************
/* Function:
    USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE _USB_HOST_MSD_InterfaceEventHandler
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle,
        USB_HOST_DEVICE_INTERFACE_EVENT event,
        void * eventData,
        uintptr_t context
    )

  Summary:
    This function is called when the Host Layer generates interface level
    events. 

  Description:
    This function is called when the Host Layer generates interface level
    events. 

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE _USB_HOST_MSD_InterfaceEventHandler
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle,
    USB_HOST_DEVICE_INTERFACE_EVENT event,
    void * eventData,
    uintptr_t context
)
{
    USB_HOST_MSD_INSTANCE * msdInstanceInfo;
    USB_HOST_DEVICE_INTERFACE_EVENT_TRANSFER_COMPLETE_DATA * transferCompleteEventData;

    /* The context given at the time of the request is the pointer to the
     * USB_HOST_MSD_INSTANCE object */

    msdInstanceInfo = (USB_HOST_MSD_INSTANCE *)(context);
    
    switch(event)
    {
        case USB_HOST_DEVICE_INTERFACE_EVENT_TRANSFER_COMPLETE:
    
            /* This means a transfer completed. Update the transfer state
             * machine. */
            transferCompleteEventData = (USB_HOST_DEVICE_INTERFACE_EVENT_TRANSFER_COMPLETE_DATA *)(eventData);
            _USB_HOST_MSD_TransferTasks (msdInstanceInfo, transferCompleteEventData->result, transferCompleteEventData->length);
            break;
            
        case USB_HOST_DEVICE_INTERFACE_EVENT_SET_INTERFACE_COMPLETE:
            
            break;
            
        case USB_HOST_DEVICE_INTERFACE_EVENT_PIPE_HALT_CLEAR_COMPLETE:
            /* Let the main state machine know that the standard request is done
             * */
            msdInstanceInfo->standardRequestDone = true;
            break;
            
        default:
            break;
    }


    return(USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE_NONE);
}

// *****************************************************************************
/* Function:
    void USB_HOST_MSD_InterfaceTasks
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
    )

  Summary:
    This function is called by the Host Layer to update the state of this
    driver.

  Description:
    This function is called by the Host Layer to update the state of this
    driver.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_MSD_InterfaceTasks
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)
{
    int msdInstanceIndex;
    int iterator;
    USB_HOST_MSD_INSTANCE * msdInstanceInfo;
    USB_HOST_TRANSFER_HANDLE  transferHandle;
    USB_HOST_REQUEST_HANDLE requestHandle;
    
    /* Get the MSD instance for this interface */
    msdInstanceIndex = _USB_HOST_MSD_InterfaceHandleToMSDInstance(interfaceHandle);

    if(msdInstanceIndex >= 0)
    {
        msdInstanceInfo = &gUSBHostMSDInstance[msdInstanceIndex];
        
        switch(msdInstanceInfo->msdState)
        {
            case USB_HOST_MSD_STATE_NOT_READY:

                /* This object is in the process of getting initialized. Dont do
                 * anything yet */
                break;

            case USB_HOST_MSD_STATE_GET_MAX_LUN:

                /* In this state we launch the Get Max LUN request. Allocate the
                 * request object  */
                msdInstanceInfo->requestObj.inUse = true;

                /* Create the Get Max LUN packet */
                _USB_HOST_MSD_GetMaxLUNPacketCreate(&msdInstanceInfo->requestObj, msdInstanceInfo->bInterfaceNumber);

                /* Set the flag indicating we are waiting for the control
                 * request to complete */
                msdInstanceInfo->requestObj.controlRequestDone = false;

                /* Launch the request. We give a pointer to the
                 * logicalUnitNumber member of the instance object. The received
                 * data should be stored there. */
                if(USB_HOST_DeviceControlTransfer(msdInstanceInfo->controlPipeHandle, 
                        &transferHandle,
                        &msdInstanceInfo->requestObj.setupPacket, 
                        &msdInstanceInfo->logicalUnitNumber,
                        _USB_HOST_MSD_ControlTransferCallback,
                        (uintptr_t)(msdInstanceInfo)) == USB_HOST_RESULT_SUCCESS)
                {
                    /* Update state. We will wait for the Get Max LUN request to
                     * complete. */
                    msdInstanceInfo->msdState = USB_HOST_MSD_STATE_WAITING_GET_MAX_LUN;
                }
                else
                {
                    /* Wait in the same state */
                }

                break;
                
            case USB_HOST_MSD_STATE_WAITING_GET_MAX_LUN:
                
                /* Here we wait for the Get Max Lun to complete */
                if(msdInstanceInfo->requestObj.controlRequestDone)
                {
                    /* This means the control transfer completed. Check the 
                     * result */
                    if(msdInstanceInfo->requestObj.result == USB_HOST_RESULT_FAILURE)
                    {
                        /* This means an unknown error has occurred. We should
                         * perform reset recovery */
                        msdInstanceInfo->msdState = USB_HOST_MSD_STATE_RESET_COMMAND;
                        
                    }
                    else if ((msdInstanceInfo->requestObj.result == USB_HOST_RESULT_SUCCESS) ||
                            (msdInstanceInfo->requestObj.result == USB_HOST_RESULT_REQUEST_STALLED))
                    {
                        /* The command has either succeeded or was stalled. In 
                         * both cases this is fine. Send each LUN to the SCSI
                         * driver */
                        
                        for (iterator = 0; iterator < msdInstanceInfo->logicalUnitNumber + 1; iterator ++)
                        {
                            USB_HOST_SCSI_Initialize(USB_HOST_MSD_LUNHandleGet(iterator,msdInstanceIndex));
                        }

                        /* Release the control transfer object and move the MSD
                         * object to the ready state */
                        msdInstanceInfo->requestObj.inUse = false;
                        msdInstanceInfo->msdState = USB_HOST_MSD_STATE_READY;
                    }
                }
                break;

            case USB_HOST_MSD_STATE_RESET_COMMAND:

                /* Some error has occurred while getting Max LUN. Reset the MSD
                 * device */

                msdInstanceInfo->requestObj.inUse = true;

                /* Create the Get Max LUN packet */
                _USB_HOST_MSD_ResetPacketCreate(&msdInstanceInfo->requestObj, msdInstanceInfo->bInterfaceNumber);

                /* Set the flag indicating we are waiting for the control
                 * request to complete */
                msdInstanceInfo->requestObj.controlRequestDone = false;

                /* Update state. We are waiting for reset to complete */
                msdInstanceInfo->msdState = USB_HOST_MSD_STATE_WAIT_FOR_RESET;

                /* Launch the request */
                if(USB_HOST_DeviceControlTransfer(msdInstanceInfo->controlPipeHandle, 
                        &transferHandle,
                        &msdInstanceInfo->requestObj.setupPacket, 
                        NULL, _USB_HOST_MSD_ControlTransferCallback,
                        (uintptr_t)(msdInstanceInfo)) == USB_HOST_RESULT_SUCCESS)
                {
                    msdInstanceInfo->msdState = USB_HOST_MSD_STATE_WAIT_FOR_RESET;
                }
                else
                {
                    /* Wait in the same state */
                }
                
                break;

            case USB_HOST_MSD_STATE_WAIT_FOR_RESET:

                /* We have sent a MSD reset. Has this request completed */
                if(msdInstanceInfo->requestObj.controlRequestDone == true)
                {
                    /* Request has completed */
                    if(msdInstanceInfo->requestObj.result == USB_HOST_RESULT_SUCCESS)
                    {
                        /* The reset function was called because the Get Max LUN
                         * failed. Try to Get MAX LUN again */
                    
                         msdInstanceInfo->msdState = USB_HOST_MSD_STATE_GET_MAX_LUN;
                    }
                    else
                    {
                        /* Both the reset and Get max LUN failed. Place the
                         * device in an error state */
                        msdInstanceInfo->msdState = USB_HOST_MSD_STATE_ERROR;
                    }
                }
                break;

            case USB_HOST_MSD_STATE_READY:
                
                /* Device is in a ready state. BOT protocol can be supported now
                 * Run the task routines for each LUN */
                for (iterator = 0; iterator < msdInstanceInfo->logicalUnitNumber + 1; iterator ++)
                {
                    USB_HOST_SCSI_Tasks(USB_HOST_MSD_LUNHandleGet(iterator,msdInstanceIndex));
                }
                break;

            case USB_HOST_MSD_STATE_IN_PIPE_HALT_CLEAR:

                /* In this state, the MSD driver tries to clear the endpoint
                 * halt. The flow will reach this state when a BOT transfer
                 * stalled. Clear the stall condition. The standardRequestDone
                 * flag is updated in the event handler. */

                msdInstanceInfo->standardRequestDone = false;
                if(USB_HOST_DevicePipeHaltClear(msdInstanceInfo->bulkInPipeHandle, &requestHandle, 
                        (uintptr_t)(msdInstanceInfo)) == USB_HOST_RESULT_SUCCESS)
                {
                    /* The request was accepted. Wait for completion */
                    msdInstanceInfo->msdState = USB_HOST_MSD_STATE_WAIT_IN_PIPE_HALT_CLEAR_COMPLETE;
                }
                break;

            case USB_HOST_MSD_STATE_WAIT_IN_PIPE_HALT_CLEAR_COMPLETE:

                /* Here we are waiting for the standard request to complete */
                if(msdInstanceInfo->standardRequestDone)
                {
                    /* We get the CSW to find out why the pipe was halted */
                    if( USB_HOST_DeviceTransfer(msdInstanceInfo->bulkInPipeHandle, &transferHandle, 
                        msdInstanceInfo->msdCSW, 13, (uintptr_t)(msdInstanceInfo)) == USB_HOST_RESULT_SUCCESS)
                    {
                        msdInstanceInfo->msdState = USB_HOST_MSD_STATE_WAIT_FOR_POST_PIPE_HALT_CLEAR_CSW;
                    }
                }
                break;

            case USB_HOST_MSD_STATE_ERROR:

                /* Device is in an error state. It must be unplugged. */
                break;

            default:
                break;
        }
    }
}

// *****************************************************************************
/* Function:
   USB_HOST_MSD_RESULT USB_HOST_MSD_Transfer
   (
       uint8_t * cdb,
       uint8_t cdbLength,
       void * data,
       size_t size,
       USB_HOST_MSD_TRANSFER_DIRECTION transferDirection,
       USB_HOST_MSD_TRANSFER_CALLBACK callback,
       uintptr_t context
   )

  Summary:
    This function schedules a MSD BOT transfer.

  Description:
    This function schedules a MSD BOT transfer. The command to be executed is
    specified in the cdb. This should be pointer to a 16 byte command descriptor
    block. The actual length of the command is specified by cdbLength. If there
    is data to be transferred, the pointer to the buffer is specified by data.
    The size of the buffer is specified in size. When the transfer completes,
    the callback function will be called. The context will be returned in the
    callback function.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/


USB_HOST_MSD_RESULT USB_HOST_MSD_Transfer
(
    USB_HOST_MSD_LUN_HANDLE lunHandle,
    uint8_t * cdb,
    uint8_t cdbLength,
    void * data,
    size_t size,
    USB_HOST_MSD_TRANSFER_DIRECTION transferDirection,
    USB_HOST_MSD_TRANSFER_CALLBACK callback,
    uintptr_t context
)
{
    USB_HOST_MSD_RESULT result;
    int iterator;
    USB_HOST_MSD_INSTANCE * msdInstanceInfo;
    int msdInstanceIndex;
    USB_HOST_RESULT hostResult;
    USB_HOST_TRANSFER_HANDLE transferHandle;

    if(USB_HOST_MSD_LUN_HANDLE_INVALID == lunHandle)
    {
        result = USB_HOST_MSD_RESULT_LUN_HANDLE_INVALID;
    }
    else
    {
        /* Get the MSD Instance Index from the LUN Handle */
        msdInstanceIndex = USB_HOST_MSD_INDEX(lunHandle);

        /* Get the pointer to the MSD instance */
        msdInstanceInfo = &gUSBHostMSDInstance[msdInstanceIndex];

        if(!msdInstanceInfo->assigned)
        {
            /* This object is not valid */
            result = USB_HOST_MSD_RESULT_FAILURE;
        }
        else
        {
            /* Object is valid. Check if we are ready to accept requests */
            if((msdInstanceInfo->msdState == USB_HOST_MSD_STATE_READY) && 
                    (!msdInstanceInfo->transferObj.inUse) &&
                    (msdInstanceInfo->transferState == USB_HOST_MSD_TRANSFER_STATE_READY))
            {
                /* We can proceed with the request. Grab the transfer object */
                msdInstanceInfo->transferObj.inUse = true;

                /* Setup the CBW */
                msdInstanceInfo->msdCBW->dCBWSignature = USB_MSD_VALID_CBW_SIGNATURE;
                msdInstanceInfo->msdCBW->dCBWTag = USB_MSD_VALID_CBW_TAG;
                msdInstanceInfo->msdCBW->bCBWCBLength = cdbLength;
                msdInstanceInfo->msdCBW->dCBWDataTransferLength = size;
                msdInstanceInfo->msdCBW->bmCBWFlags.value = transferDirection;

                msdInstanceInfo->msdCBW->bCBWLUN = USB_HOST_MSD_LUN(lunHandle);

                /* Copy the cdb. It should be zero padded */
                for(iterator = 0; iterator < 16; iterator ++)
                {
                    /* Clear the command block */
                    msdInstanceInfo->msdCBW->CBWCB[iterator] = 0;
                }

                /* Now copy the command */
                for(iterator = 0; iterator < cdbLength; iterator ++)
                {
                    msdInstanceInfo->msdCBW->CBWCB[iterator] = cdb[iterator];
                }

                /* Save the caller data in the transfer object */
                msdInstanceInfo->transferObj.callback = callback;
                msdInstanceInfo->transferObj.context = context;
                msdInstanceInfo->transferObj.size = size;
                msdInstanceInfo->transferObj.transferDirection = transferDirection;
                msdInstanceInfo->transferObj.cdb = cdb;
                msdInstanceInfo->transferObj.cdbLength = cdbLength;
                msdInstanceInfo->transferObj.lunHandle = lunHandle;
                msdInstanceInfo->transferObj.buffer = data;

                msdInstanceInfo->transferState = USB_HOST_MSD_TRANSFER_STATE_WAIT_FOR_CBW;

                /* CBW must go out on the bulk out pipe handle */
                hostResult = USB_HOST_DeviceTransfer(msdInstanceInfo->bulkOutPipeHandle, &transferHandle, msdInstanceInfo->msdCBW, 31, (uintptr_t)(msdInstanceInfo));

                /* Map the result */
                result = _USB_HOST_MSD_HostResultToMSDResultMap(hostResult); 
                
            }
            else
            {
                result = USB_HOST_MSD_RESULT_BUSY;
            }
        }
    }

    return(result);

}
