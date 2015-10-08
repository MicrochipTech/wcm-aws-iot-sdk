/******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_msd.c

  Summary:
    USB Device Mass Storage Class Function Driver Implementation.

  Description:
    This file contains implementations of both private and public functions of
    USB Device MSD function driver. It should be included in any project that
    requires USB Device MSD functionality.
********************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "system/common/sys_module.h"
#include "driver/driver_common.h"
#include "usb/usb_device_msd.h"
#include "usb/src/usb_device_msd_local.h"

/*************************************
 * USB device MSD instance objects.
 *************************************/
static USB_DEVICE_MSD_INSTANCE gUSBDeviceMSDInstance [USB_DEVICE_INSTANCES_NUMBER];

/***************************************
 * USB device MSD init objects.
 ***************************************/

USB_DEVICE_MSD_INIT gUSBDeviceMSDInit[USB_DEVICE_MSD_INSTANCES_NUMBER];

/****************************************
 * MSD Device function driver structure
 ****************************************/
  
USB_DEVICE_FUNCTION_DRIVER msdFunctionDriver = 
{
    /* MSD init function */
    .initializeByDescriptor = _USB_DEVICE_MSD_InitializeByDescriptorType ,

    /* MSD de-init function */
    .deInitialize = _USB_DEVICE_MSD_Deinitialization , 
      
    /* MSD set-up packet handler */
    .controlTransferNotification = _USB_DEVICE_MSD_ControlTransferHandler ,

    /* MSD tasks function */
    .tasks = _USB_DEVICE_MSD_Tasks
};

// ******************************************************************************
/* Function:
    void _USB_DEVICE_MSD_InitializeEndpoint
    (
        USB_DEVICE_MSD_INSTANCE * msdInstance,
        DRV_HANDLE usbDevHandle,
        USB_ENDPOINT_DESCRIPTOR * epDescriptor
    )

  Summary:
    USB Device MSD endpoint initialization.

  Description:
    Initialize endpoints and endpoint related variables here.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_DEVICE_MSD_InitializeEndpoint
(
    USB_DEVICE_MSD_INSTANCE * msdInstance,
    DRV_HANDLE usbDevHandle,
    USB_ENDPOINT_DESCRIPTOR * epDescriptor
)
{
    /* This function is called from _USB_DEVICE_MSD_InitializeByDescriptorType
     * when the descriptor type is an endpoint. The bulk endpoints associated 
     * with this interface are enabled in this function */

    if( epDescriptor->transferType == USB_TRANSFER_TYPE_BULK )
    {
        if(epDescriptor->dirn == USB_DATA_DIRECTION_DEVICE_TO_HOST)
        {
            /* Save the TX endpoint information */
            msdInstance->bulkEndpointTx = epDescriptor->bEndpointAddress;
            msdInstance->bulkEndpointTxSize =  epDescriptor->wMaxPacketSize;
            
            /* Enable the TX endpoint */
            USB_DEVICE_EndpointEnable(usbDevHandle, 0, msdInstance->bulkEndpointTx, epDescriptor->transferType, epDescriptor->wMaxPacketSize);
        }
        else
        {
            /* Enable the receive endpoint */
            msdInstance->bulkEndpointRx = epDescriptor->bEndpointAddress;
            msdInstance->bulkEndpointRxSize = epDescriptor->wMaxPacketSize;

            /* Enable the endpoint */
            USB_DEVICE_EndpointEnable(usbDevHandle, 0, msdInstance->bulkEndpointRx, epDescriptor->transferType,epDescriptor->wMaxPacketSize);

            /* Now since device layer has already opened the bulk endpoint we can submit an
             * IRP to receive the CBW. */
            msdInstance->irpRx.data = (void *)msdInstance->msdCBW;
            msdInstance->irpRx.size = epDescriptor->wMaxPacketSize;
            msdInstance->irpRx.flags = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;

            /* Arm the RX endpoint to receive data.*/
            USB_DEVICE_IRPSubmit( msdInstance->hUsbDevHandle, msdInstance->bulkEndpointRx, &msdInstance->irpRx);
        }
    }
    else
    {
        SYS_ASSERT( false, "USB DEVICE MSD: MSD does not support anything other than Bulk endpoints. Please check the descriptors.");
    }    
}

// *****************************************************************************
/* Function:
    void _USB_DEVICE_MSD_InitializeInterface
    (
        USB_DEVICE_MSD_OBJ * msdDeviceObj,
        DRV_HANDLE usbDeviceHandle,
        void * funcDriverInit,
        USB_INTERFACE_DESCRIPTOR * intfDesc
    )

  Summary:
    USB Device MSD Interface Initialization.

  Description:
    Initialize only the interface related variables here.

  Remarks:
    This is a local function and should not be called directly by the
    applicaiton.
*/

void _USB_DEVICE_MSD_InitializeInterface
(
    USB_DEVICE_MSD_INSTANCE * msdDeviceObj,
    DRV_HANDLE usbDeviceHandle,
    void * funcDriverInit,
    USB_INTERFACE_DESCRIPTOR * intfDesc
)
{
    /* This function is called from _USB_DEVICE_MSD_InitializeByDescriptorType
     * when the descriptor type is interface */

    int count;
    USB_DEVICE_MSD_INIT * msdInitializationData;

    /* Access the MSD Function Driver Initialization data */
    msdInitializationData = (USB_DEVICE_MSD_INIT *)funcDriverInit;

    /* Update the MSD function driver instance with the 
     * initialization data. The media data is a pointer
     * to table containing data about each media
     * that this MSD instance is managing. */
    msdDeviceObj->numberOfLogicalUnits = msdInitializationData->numberOfLogicalUnits;
    msdDeviceObj->mediaData = msdInitializationData->mediaInit;
    msdDeviceObj->msdCSW = msdInitializationData->msdCSW;
    msdDeviceObj->msdCBW = msdInitializationData->msdCBW;

    /* Initialize all media handles to an initial value i.e invalid handles. */
    for(count = 0; count < msdDeviceObj->numberOfLogicalUnits; count++)
    {
        msdDeviceObj->mediaDynamicData[count].mediaHandle = DRV_HANDLE_INVALID;
        SYS_ASSERT(msdDeviceObj->mediaData->mediaFunctions[count].open != NULL, "This function pointer cannot be NULL");
        SYS_ASSERT(msdDeviceObj->mediaData->mediaFunctions[count].close != NULL, "This function pointer cannot be NULL");
        SYS_ASSERT(msdDeviceObj->mediaData->mediaFunctions[count].isAttached != NULL, "This function pointer cannot be NULL");
        SYS_ASSERT(msdDeviceObj->mediaData->mediaFunctions[count].isWriteProtected != NULL, "This function pointer cannot be NULL");
        SYS_ASSERT(msdDeviceObj->mediaData->mediaFunctions[count].geometryGet != NULL, "This function pointer cannot be NULL");
        SYS_ASSERT(msdDeviceObj->mediaData->mediaFunctions[count].blockEventHandlerSet != NULL, "This function pointer cannot be NULL");
        SYS_ASSERT(msdDeviceObj->mediaData->mediaFunctions[count].blockRead != NULL, "This function pointer cannot be NULL");
    }

    /* Remember the USB Device Layer handle */
    msdDeviceObj->hUsbDevHandle = usbDeviceHandle;

    /* Initialize some of the IRP members to an intial value. Note that the
     * user data of each IRP is set to point to the owner MSD Device Instance */

    msdDeviceObj->irpRx.userData = (uintptr_t)msdDeviceObj;
    msdDeviceObj->irpTx.userData = (uintptr_t)msdDeviceObj;
    msdDeviceObj->irpRx.status = USB_DEVICE_IRP_STATUS_COMPLETED;
    msdDeviceObj->irpTx.status = USB_DEVICE_IRP_STATUS_COMPLETED;
    msdDeviceObj->irpTx.callback = &_USB_DEVICE_MSD_CallBackBulkTxTransfer;
    msdDeviceObj->irpRx.callback = &_USB_DEVICE_MSD_CallBackBulkRxTransfer;

    /* The Host may set an alternate inteface on this instance. Intialize the
     * alternate setting to zero */
    msdDeviceObj->alternateSetting = 0;
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_MSD_InitializeByDescriptorType
    (
        SYS_MODULE_INDEX iMSD, 
        DRV_HANDLE usbDeviceHandle,
        void* funcDriverInit, 
        uint8_t intfNumber, 
        uint8_t altSetting,
        uint8_t descriptorType, 
        uint8_t * pDescriptor
    )

  Summary:
    USB Device MSD Initialization. The device layer calls this function to
    initialize the MSD based on the descriptor found (from the configurations
    descriptor).  

  Description:
    This function initializes MSD function driver based on the descriptor.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_DEVICE_MSD_InitializeByDescriptorType
(
    SYS_MODULE_INDEX iMSD, 
    DRV_HANDLE usbDeviceHandle,
    void* funcDriverInit, 
    uint8_t intfNumber, 
    uint8_t altSetting,
    uint8_t descriptorType, 
    uint8_t * pDescriptor
)
{
    /* This function is called by the Device Layer when it comes across an MSD
     * interface descriptor or an endpoint belonging to a MSD interface */

    SYS_ASSERT(altSetting == 0, "USB Device MSD: MSD supports only one setting and does not support alternate settings ");

    USB_DEVICE_MSD_INSTANCE * msdDeviceObj = &gUSBDeviceMSDInstance[iMSD];

    switch(descriptorType )
    {
        case USB_DESCRIPTOR_ENDPOINT:
            
            /* Device layer came across an MSD endpoint. Initialize the endpoint */
            _USB_DEVICE_MSD_InitializeEndpoint(msdDeviceObj, usbDeviceHandle,(USB_ENDPOINT_DESCRIPTOR *)pDescriptor);
            break;

        case USB_DESCRIPTOR_INTERFACE:

            /* Device Layer came across an MSD interface. Initialize the
             * interface */
            _USB_DEVICE_MSD_InitializeInterface(msdDeviceObj, usbDeviceHandle, funcDriverInit, (USB_INTERFACE_DESCRIPTOR *)pDescriptor);
            break;

        default:
            SYS_ASSERT( false, "USB DEVICE MSD: MSD doesnot support this descriptor type. Please check the descriptors");
            break;
    }
}

// ******************************************************************************
/* Function: 
   void _USB_DEVICE_MSD_ControlTransferHandler
    (
        SYS_MODULE_INDEX MSDIndex,
        USB_DEVICE_EVENT transferState,
        USB_SETUP_PACKET * setupPkt
    )
  
  Summary:
    MSD control transfer handler.

  Description:
    MSD control transfer handler. This is the callback the device layer calls
    when there is a setup packet that is targeted to this particular instance
    of MSD.

  Returns:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_DEVICE_MSD_ControlTransferHandler
(
    SYS_MODULE_INDEX MSDIndex,
    USB_DEVICE_EVENT controlTransferEvent,
    USB_SETUP_PACKET * setupPkt
)
{
    USB_DEVICE_MSD_INSTANCE * msdThisInstance = &gUSBDeviceMSDInstance[MSDIndex] ;

    if(controlTransferEvent == USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST)
    {
        if(( setupPkt->Recipient == 0x01) && (setupPkt->RequestType == 0x00))
        {
            /* This means the recipient of the control transfer is interface
             * and this is a standard request type */

            switch(setupPkt->bRequest)
            {
                case USB_REQUEST_SET_INTERFACE:

                    /* If the host does a Set Interface, we simply acknowledge
                     * it. We also remember the interface that was set */
                    msdThisInstance->alternateSetting = setupPkt->W_Value.byte.LB;
                    USB_DEVICE_ControlStatus( msdThisInstance->hUsbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);
                    break;

                case USB_REQUEST_GET_INTERFACE:

                    /* The host is requesting for the current interface setting
                     * number. Return the one that the host set */
                    USB_DEVICE_ControlSend( msdThisInstance->hUsbDevHandle, &msdThisInstance->alternateSetting, 1);
                    break;

                default:
                    break; 
            }
        }
        else if( setupPkt->bmRequestType & USB_MSD_REQUEST_CLASS_SPECIFIC )
        {
            /* We have got setup request */
            switch ( setupPkt->bRequest )
            {
                case USB_MSD_GET_MAX_LUN:

                    /* First make sure all request parameters are correct:
                     * MSD BOT specs require wValue to be == 0x0000, and wLengh == 1 */
                    if((setupPkt->wValue != 0) || (setupPkt->wLength != 1))
                    {
                        USB_DEVICE_ControlStatus( msdThisInstance->hUsbDevHandle,
                                USB_DEVICE_CONTROL_STATUS_ERROR );
                        return ;
                    }

                    /* Return the number of LUNs in this device less one. */

                    msdThisInstance->msdBuffer  = msdThisInstance->numberOfLogicalUnits - 1;
                    USB_DEVICE_ControlSend( msdThisInstance->hUsbDevHandle, &msdThisInstance->msdBuffer, 1 );

                    break;

                case USB_MSD_RESET:

                    /* First make sure all request parameters are correct:
                     * MSD BOT specs require wValue to be == 0x0000, and wLengh == 0 */
                   if((setupPkt->wValue != 0) || (setupPkt->wLength != 0))
                    {
                        USB_DEVICE_ControlStatus( msdThisInstance->hUsbDevHandle,
                                USB_DEVICE_CONTROL_STATUS_ERROR );
                        return ;
                    }

                   /* Get ready to receive a CBW */
                   msdThisInstance->irpRx.data = (void *)msdThisInstance->msdCBW;
                   msdThisInstance->irpRx.size = msdThisInstance->bulkEndpointRxSize;
                   msdThisInstance->irpRx.flags = USB_DEVICE_IRP_FLAG_DATA_PENDING;
                   USB_DEVICE_IRPSubmit( msdThisInstance->hUsbDevHandle,
                        msdThisInstance->bulkEndpointRx, &msdThisInstance->irpRx);
                    msdThisInstance->msdMainState = USB_DEVICE_MSD_STATE_CBW;

                    USB_DEVICE_ControlStatus( msdThisInstance->hUsbDevHandle, USB_DEVICE_CONTROL_STATUS_OK );
                    break;

                default:

                    /* Stall other requests. */
                    USB_DEVICE_ControlStatus( msdThisInstance->hUsbDevHandle, USB_DEVICE_CONTROL_STATUS_ERROR );
                    break;
            } 
        }
    }
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_MSD_CallBackBulkRxTransfer( void * handle )

  Summary:
    This is a callback function that gets called by controller driver,
    after the completion of bulk-out transfer.

  Description:
    This is a callback function that gets called by controller driver,
    after the completion of bulk-out transfer.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void _USB_DEVICE_MSD_CallBackBulkRxTransfer( USB_DEVICE_IRP *  handle )
{
    /* Code to be add if required in a future release */
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_MSD_CallBackBulkTxTransfer( void *  handle )

  Summary:
    This is a callback function that gets called by controller driver,
    after the completion of bulk-in transfer.

  Description:
    This is a callback function that gets called by controller driver,
    after the completion of bulk-in transfer.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void _USB_DEVICE_MSD_CallBackBulkTxTransfer( USB_DEVICE_IRP *  handle )
{
    /* Code to be add if required in a future release */
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_MSD_PreDataStageRoutine ( SYS_MODULE_INDEX iMSD )

  Summary:
    Do some routine pre-data stage checks.

  Description:
    After we received CBW and before entering data stage we will have to
    do some settings.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void _USB_DEVICE_MSD_PreDataStageRoutine ( SYS_MODULE_INDEX iMSD )
{
    /* This function is called when the function driver has receieved
     * a CBW. It is called from the MSD tasks routine. This function
     * resets all parameters related to the data stage of the bulk 
     * transfer. */

    uint8_t logicalUnit;

    USB_DEVICE_MSD_INSTANCE * msdThisInstance = &gUSBDeviceMSDInstance[iMSD];

    logicalUnit = msdThisInstance->msdCBW->bCBWLUN;

    /* Reset media state */
    msdThisInstance->mediaDynamicData[logicalUnit].mediaState = USB_DEVICE_MSD_MEDIA_OPERATION_IDLE;

    /* Reset the total bytes sent/rcvd counter. */
    msdThisInstance->rxTxTotalDataByteCount = 0;   

    /* Reset IRP RX size. */
    msdThisInstance->irpRx.size = 0;

    /* See if last command was request sense. If so host has already read the
     * sense data. Now we should reset sense data. */

    if(msdThisInstance->mediaDynamicData[logicalUnit].resetSenseData)
    {
        msdThisInstance->mediaDynamicData[logicalUnit].resetSenseData = false;
        _USB_DEVICE_MSD_ResetSenseData(&msdThisInstance-> mediaDynamicData[logicalUnit].gblSenseData);
    }
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_MSD_PostDataStageRoutine ( SYS_MODULE_INDEX iMSD )

  Summary:
    Do some post data stage routines (Before we initiate CSW).

  Description:
    Do some post data stage routines (Before we initiate CSW).
  
  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void _USB_DEVICE_MSD_PostDataStageRoutine ( SYS_MODULE_INDEX iMSD )
{
    /* This function is called int function driver tasks routine when
     * the CSW is being prepared. */
    
    uint32_t residueLength;
    USB_DEVICE_MSD_INSTANCE * msdObj = &gUSBDeviceMSDInstance[iMSD];

    /* Build the CSW */
    msdObj->msdCSW->dCSWSignature = USB_MSD_VALID_CSW_SIGNATURE;
    msdObj->msdCSW->dCSWTag = msdObj->msdCBW->dCBWTag;

    residueLength = (msdObj->msdCBW->dCBWDataTransferLength - msdObj->rxTxTotalDataByteCount);

    /* Update the Residue length in the CSW */
    msdObj->msdCSW->dCSWDataResidue = residueLength;
    //msdObj->msdCSW->bCSWStatus = USB_MSD_CSW_COMMAND_PASSED;
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_MSD_Tasks ( SYS_MODULE_INDEX iMSD )

  Summary:
    This function handles the main MSD state machine.

  Description:
    This function handles the main MSD state machine. It process the CBW, data
    stage and the CSW. This function is called by the Device Layer tasks routine.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void _USB_DEVICE_MSD_Tasks ( SYS_MODULE_INDEX iMSD )
{
    uintptr_t rxTxBufferAdrs;
    uint32_t rxTxBufferSize;    
    uint8_t commandStatus;
    USB_DEVICE_MSD_INSTANCE * msdObj = &gUSBDeviceMSDInstance[iMSD];

    /* Check if we TX or RX IRP has completed */
    if(  ( msdObj->irpRx.status <= USB_DEVICE_IRP_STATUS_COMPLETED_SHORT )
            && ( msdObj->irpTx.status <= USB_DEVICE_IRP_STATUS_COMPLETED_SHORT )
            && ( !USB_DEVICE_EndpointIsStalled( msdObj->hUsbDevHandle, msdObj->bulkEndpointTx )) )
    {
        switch (msdObj->msdMainState)
        {
            case USB_DEVICE_MSD_STATE_CBW :
                
                /* Verify the command. The return value of the function
                 * indicates what the next state should be. */
                msdObj->msdMainState = _USB_DEVICE_MSD_VerifyCommand (iMSD);

                /* We have got a command. We need to reset some variables */
                _USB_DEVICE_MSD_PreDataStageRoutine( iMSD );
                break;

            case USB_DEVICE_MSD_STATE_STALL_IN:
                
                /* Stall the IN endpoint and set the CSW status to command 
                 * failed. This will be sent to the host when the CSW stage
                 * occurs */

                msdObj->msdCSW->bCSWStatus = USB_MSD_CSW_COMMAND_FAILED;
                USB_DEVICE_EndpointStall( msdObj->hUsbDevHandle, msdObj->bulkEndpointTx );

                /* Set Rx/Tx byte count to requested transfer length.
                 * Eventhough, we have not sent the entire bytes, this is very
                 * much required to assume that we have sent all the bytes.
                 * Otherwise we generate a PHASE ERROR in post data-stage
                 * routine. */
                msdObj->rxTxTotalDataByteCount = msdObj->msdCBW->dCBWDataTransferLength;
                msdObj->msdMainState = USB_DEVICE_MSD_STATE_CSW;
                break;

            case USB_DEVICE_MSD_STATE_DATA_IN:

                /* This means we have to send or have to continue sending data.
                 * Check if we have any/more data to send. The return value of
                 * this function indicates what the next state should be. */
                msdObj->msdMainState = _USB_DEVICE_MSD_ProcessCommand(iMSD, &rxTxBufferAdrs, &rxTxBufferSize, &commandStatus );

                msdObj->msdCSW->bCSWStatus = commandStatus;

                if( rxTxBufferSize != 0) 
                {
                    /* If response size is not 0, this means data is ready.
                     * Prepare the IRP and submit
                     */
                    msdObj->rxTxTotalDataByteCount += rxTxBufferSize;
                    msdObj->irpTx.data = (void *)rxTxBufferAdrs;
                    msdObj->irpTx.size = rxTxBufferSize;

                    /* Submit the endpoint */
                    USB_DEVICE_IRPSubmit( msdObj->hUsbDevHandle, msdObj->bulkEndpointTx, &msdObj->irpTx);
                }
                else
                {
                    /* The data is not ready to sent yet. The data stage will be
                     * continue to be nacked. */
                }
                break;

            case USB_DEVICE_MSD_STATE_DATA_OUT:

                /* Check if we have any/more data to receive. */
                msdObj->msdMainState = _USB_DEVICE_MSD_ProcessCommand(iMSD, &rxTxBufferAdrs, &rxTxBufferSize, &commandStatus );

                /* Update the CSW command status */
                msdObj->msdCSW->bCSWStatus = commandStatus;

                if( rxTxBufferSize != 0) 
                {
                    /* This means we are ready to receive more data */
                    msdObj->rxTxTotalDataByteCount += rxTxBufferSize;
                    msdObj->irpRx.data = (void *)rxTxBufferAdrs;
                    msdObj->irpRx.size = rxTxBufferSize;
                    msdObj->irpRx.flags = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;

                    /* Submit IRP to receive more data */
                    USB_DEVICE_IRPSubmit( msdObj->hUsbDevHandle, msdObj->bulkEndpointRx, &msdObj->irpRx);
                }

                break;

            case USB_DEVICE_MSD_STATE_CSW:

                _USB_DEVICE_MSD_PostDataStageRoutine(iMSD);

                /* Submit IRP to send CSW */
                msdObj->irpTx.data = (void *)msdObj->msdCSW;
                msdObj->irpTx.size = sizeof(USB_MSD_CSW);
                msdObj->irpTx.flags = USB_DEVICE_IRP_FLAG_DATA_PENDING;
                USB_DEVICE_IRPSubmit( msdObj->hUsbDevHandle, msdObj->bulkEndpointTx, &msdObj->irpTx);

                /* Get ready to receive a CBW */
                msdObj->irpRx.data = (void *)msdObj->msdCBW;
                msdObj->irpRx.size = msdObj->bulkEndpointRxSize;
                msdObj->irpRx.flags = USB_DEVICE_IRP_FLAG_DATA_PENDING;
                USB_DEVICE_IRPSubmit( msdObj->hUsbDevHandle, msdObj->bulkEndpointRx, &msdObj->irpRx);
                msdObj->msdMainState = USB_DEVICE_MSD_STATE_CBW;
                break;

            case USB_DEVICE_MSD_STATE_STALL_OUT:
                
                /* Stall the OUT endpoint and set the CSW status to command
                 * failed. This will be sent to the host when the CSW stage
                 * occurs */

                msdObj->msdCSW->bCSWStatus = USB_MSD_CSW_COMMAND_FAILED;
                USB_DEVICE_EndpointStall( msdObj->hUsbDevHandle, msdObj->bulkEndpointRx );

                /* Set Rx/Tx byte count to requested transfer length.
                 * Eventhough, we have not sent the entire bytes, this is very
                 * much required to assume that we have sent all the bytes.
                 * Otherwise we generate a PHASE ERROR in post data-stage
                 * routine. */
                msdObj->rxTxTotalDataByteCount = msdObj->msdCBW->dCBWDataTransferLength;
                msdObj->msdMainState = USB_DEVICE_MSD_STATE_CSW;
                break;

            case USB_DEVICE_MSD_STATE_IDLE:
                break;
        }
    }
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_MSD_BlockEventHandler
    (
        SYS_FS_MEDIA_BLOCK_EVENT event,
        SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle,
        uintptr_t context
    )

  Summary:
    This is a callback function called by the media after the completion of
    media operation.

  Description:
    This is a callback function called by the media after the completion of
    media operation.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void _USB_DEVICE_MSD_BlockEventHandler
(
    SYS_FS_MEDIA_BLOCK_EVENT event,
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle,
    uintptr_t context
)
{
    USB_DEVICE_MSD_MEDIA_DYNAMIC_DATA * mediaDynamicData = (USB_DEVICE_MSD_MEDIA_DYNAMIC_DATA *)context;
    switch(event)
    {
        case SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_COMPLETE:
            mediaDynamicData->mediaState = USB_DEVICE_MSD_MEDIA_OPERATION_COMPLETE;
            break;
        case SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_ERROR:
            /* This case is not handled yet */
            break;
    }
 }


// ******************************************************************************
/* Function:
    void _USB_DEVICE_MSD_CheckAndUpdateMediaState
    (
        SYS_MODULE_INDEX iMSD,
        uint8_t logicalUnit
    )

  Summary:
     This function must be called periodically in the MSD task to update the
     media state.

  Description:
     This function must be called periodically in the MSD task to update the
     media state.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void _USB_DEVICE_MSD_CheckAndUpdateMediaState
(
    SYS_MODULE_INDEX iMSD,
    uint8_t logicalUnit
)
{
    /* This function is called by the _USB_DEVICE_MSD_VerifyCommand() 
     * function. The mediaPresent flag in this function is checked
     * by the _USB_DEVICE_MSD_ProcessCommand() function to decide
     * whether it should be continue processing the command */

    USB_DEVICE_MSD_MEDIA_FUNCTIONS * mediaFunctions;
    USB_DEVICE_MSD_MEDIA_DYNAMIC_DATA * mediaDynamicData;
    SYS_MODULE_INDEX mediaInstanceIndex;
    DRV_HANDLE drvHandle;

    USB_DEVICE_MSD_INSTANCE * msdThisInstance = &gUSBDeviceMSDInstance[iMSD];

    /* Get data about this LUN. The dynamic information about this media */
    mediaDynamicData = &msdThisInstance->mediaDynamicData[logicalUnit];

    /* Pointer to the media functions */
    mediaFunctions = &msdThisInstance->mediaData[logicalUnit].mediaFunctions;

    /* Harmony module index for this media */
    mediaInstanceIndex = msdThisInstance->mediaData[logicalUnit].instanceIndex;

    /* Check if this media has been opened. If not can it be opened */        
    if( mediaDynamicData->mediaHandle == DRV_HANDLE_INVALID )
    {
        /* Try to open the media */
        drvHandle = mediaFunctions->open( mediaInstanceIndex, DRV_IO_INTENT_READWRITE| DRV_IO_INTENT_NONBLOCKING);

        /* If the driver could be opened, then we need to do a few things */
        if (drvHandle != DRV_HANDLE_INVALID)
        {
            /* Update the media dynamic data with the valid handle */
            mediaDynamicData->mediaHandle = drvHandle;

            /* Get the sector size */
            if(msdThisInstance->mediaData[logicalUnit].sectorSize != 0)
            {
                /* This means the sector size is specified in the media init table
                 * */
                mediaDynamicData->sectorSize = msdThisInstance->mediaData[logicalUnit].sectorSize;
            }

            /* Set the block start address if available */
            if((msdThisInstance->mediaData[logicalUnit].block0StartAddress != NULL)
                    && (mediaFunctions->blockStartAddressSet != NULL))
            {
                mediaFunctions->blockStartAddressSet(drvHandle, msdThisInstance->mediaData[logicalUnit].block0StartAddress);
            }

            /* We should set an event handler with the media driver */
            mediaFunctions->blockEventHandlerSet(drvHandle, _USB_DEVICE_MSD_BlockEventHandler, (uintptr_t)mediaDynamicData);
            
            /* Get a pointer to the media geomtery */
            mediaDynamicData->mediaGeometry = mediaFunctions->geometryGet(drvHandle);
            
        }
    }

    /* The driver is open. Update the media status */
    if (mediaDynamicData->mediaHandle != DRV_HANDLE_INVALID)
    {
        /* The media driver could be opended. Now check if media is present. */
        mediaDynamicData->mediaPresent = mediaFunctions->isAttached(mediaDynamicData->mediaHandle);
       
    }
    else
    {
        /* Media is not yet ready to be opened. So consider it as media not
         * being present.*/
        mediaDynamicData->mediaPresent = false;
    }
}

// ******************************************************************************
/* Function:
    USB_DEVICE_MSD_STATE _USB_DEVICE_MSD_VerifyCommand(SYS_MODULE_INDEX iMSD )

  Summary:
    This function verifies the recieved CBW.

  Description:
    This function verifies the recieved CBW.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

USB_DEVICE_MSD_STATE _USB_DEVICE_MSD_VerifyCommand(SYS_MODULE_INDEX iMSD )
{
    /* This function is called when the device has recieved a CBW. It updates
     * the media state and then lets the main state machine know what the next
     * state should be based on the CBW verfication. */

    USB_MSD_CBW * lCBW;
    USB_DEVICE_MSD_STATE msdNextState = USB_DEVICE_MSD_STATE_CSW ;
    USB_DEVICE_MSD_INSTANCE * thisMSDInstance = &gUSBDeviceMSDInstance[iMSD];
    

    /* Obtain the pointer to the CBW data structure */
    lCBW = (USB_MSD_CBW *)thisMSDInstance->msdCBW;

    /* Make sure we have received an integral CBW with the 
     * right size and signature */
  
    if((thisMSDInstance->irpRx.size == sizeof(USB_MSD_CBW))
            && (lCBW->dCBWSignature == USB_MSD_VALID_CBW_SIGNATURE))
    {
          /* Case 2 Fix*/
          if (lCBW->dCBWDataTransferLength == 0)
          {
              msdNextState = USB_DEVICE_MSD_STATE_CSW;
          }
          /* Case 6 Fix*/
          else if(((lCBW->CBWCB[0] == SCSI_READ_10) || (lCBW->CBWCB[0] == SCSI_WRITE_10))
                && (( lCBW->CBWCB[7] == 0)&& ( lCBW->CBWCB[8] == 0)))
          {
              msdNextState = USB_DEVICE_MSD_STATE_CSW;
          }
          else if((lCBW->bCBWLUN < thisMSDInstance->numberOfLogicalUnits )     /* Verify the command is addressed to a supported LUN */
                &&(lCBW->bCBWCBLength <= USB_MSD_MAX_CB_SIZE)           /* Verify the claimed CB length is reasonable/valid */
                &&(lCBW->bCBWCBLength >= 0x01)                          /* Verify the claimed CB length is reasonable/valid */
                &&((lCBW->bmCBWFlags.value & USB_MSD_CBWFLAGS_RESERVED_BITS_MASK) == 0x00))   /* Verify reserved bits are clear */
        {
            /* Update the media state */
            _USB_DEVICE_MSD_CheckAndUpdateMediaState(iMSD, lCBW->bCBWLUN);

            /* Decide on the next state in the MSD functin state machine */
            if (lCBW->bmCBWFlags.value & USB_MSD_CBW_DIRECTION_BITMASK)
            {
                /* This means the data stage moves from device
                 * to host */
                msdNextState = USB_DEVICE_MSD_STATE_DATA_IN;
            }
            else 
            {
                /* This means the data stage moves from host
                 * to device */
                msdNextState = USB_DEVICE_MSD_STATE_DATA_OUT;
            }
        }
    }
    else
    {
        /* The CBW wasn't meaningful.  Section 6.4 of BOT specs v1.0 says, "The
         * response of a device to a CBW that is not meaningful is not
         * specified." Lets keep arming the rx bulk endpoint and hope at one
         * point of time the host sends correct CBW. */

        msdNextState = USB_DEVICE_MSD_STATE_CBW;
    }

    return msdNextState;
}

// ******************************************************************************
/* Function:
    void  _USB_DEVICE_MSD_GetBlockAddressAndLength
    (
        USB_MSD_CBW * lCBW,
        USB_DEVICE_MSD_DWORD_VAL * logicalBlockAddress,
        USB_DEVICE_MSD_DWORD_VAL * logicalBlockLength
    )

  Summary:
    This function retrieves the logical unit's block address and block length
    from CBW.

  Description:
    This function retrieves the logical unit's block address and block length
    from CBW.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void  _USB_DEVICE_MSD_GetBlockAddressAndLength
(
    USB_MSD_CBW * lCBW,
    USB_DEVICE_MSD_DWORD_VAL * logicalBlockAddress,
    USB_DEVICE_MSD_DWORD_VAL * logicalBlockLength
)
{
    /* Get block address and block length from CBW command structure.*/
    logicalBlockAddress->v[3] =  lCBW->CBWCB[2];
    logicalBlockAddress->v[2] =  lCBW->CBWCB[3];
    logicalBlockAddress->v[1] =  lCBW->CBWCB[4];
    logicalBlockAddress->v[0] =  lCBW->CBWCB[5];        
    logicalBlockLength->byte.HB = lCBW->CBWCB[7];   /* MSB of Transfer Length (in number of blocks, not bytes) */
    logicalBlockLength->byte.LB = lCBW->CBWCB[8];   /* LSB of Transfer Length (in number of blocks, not bytes) */   
}    

// ******************************************************************************
/* Function:
    void  _USB_DEVICE_MSD_SaveBlockAddressAndLength
    (
        USB_MSD_CBW * lCBW,
        USB_DEVICE_MSD_DWORD_VAL * logicalBlockAddress,
        USB_DEVICE_MSD_DWORD_VAL * logicalBlockLength
    )

  Summary:
    This function saves the logical unit's block address and block length back
    into the CBW structure.

  Description:
    This function saves the logical unit's block address and block length back
    into the CBW structure.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void  _USB_DEVICE_MSD_SaveBlockAddressAndLength
(
    USB_MSD_CBW * lCBW,
    USB_DEVICE_MSD_DWORD_VAL * logicalBlockAddress,
    USB_DEVICE_MSD_DWORD_VAL * logicalBlockLength
)
{
    /* Save back the updated address and logical block into the CBW
     * command structure. */
    lCBW->CBWCB[2] = logicalBlockAddress->v[3];
    lCBW->CBWCB[3] = logicalBlockAddress->v[2];
    lCBW->CBWCB[4] = logicalBlockAddress->v[1];
    lCBW->CBWCB[5] = logicalBlockAddress->v[0];

    /* MSB of Transfer Length (in number of blocks, not bytes) */
    lCBW->CBWCB[7] = logicalBlockLength->byte.HB;
    /* LSB of Transfer Length (in number of blocks, not bytes) */
    lCBW->CBWCB[8] = logicalBlockLength->byte.LB;   

}    

// *****************************************************************************
/* Function:
    USB_DEVICE_MSD_STATE _USB_DEVICE_MSD_ProcessCommand
    (
        SYS_MODULE_INDEX iMSD,
        uintptr_t *rxTxBufferAdrs,
        uint32_t * rxTxBufferSize,
        uint8_t * commandStatus
    )  

  Summary:
    Processes the command.

  Description:
    Processes the command.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

USB_DEVICE_MSD_STATE _USB_DEVICE_MSD_ProcessCommand
(
    SYS_MODULE_INDEX iMSD,
    uintptr_t * rxTxBufferAdrs,
    uint32_t * rxTxBufferSize,
    uint8_t * commandStatus
)
{
    /* This function processes the command received in the CBW */

    USB_MSD_CBW *lCBW;
    uint8_t logicalUnit;
    uint8_t * msdBuffer;
    uint8_t * writeBlockBackupBuffer;
    size_t mediaReadBlockSize = 0;
    size_t mediaWriteBlockSize = 0;
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE meadiaReadWriteHandle = SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;
    USB_DEVICE_MSD_INSTANCE * thisMSDInstance;
    USB_DEVICE_MSD_MEDIA_FUNCTIONS * mediaFunctions;
    USB_DEVICE_MSD_MEDIA_DYNAMIC_DATA * mediaDynamicData;
    USB_DEVICE_MSD_STATE msdNextState = USB_DEVICE_MSD_STATE_CSW ;

    USB_DEVICE_MSD_DWORD_VAL capacity;
    USB_DEVICE_MSD_DWORD_VAL sectorSize;
    USB_DEVICE_MSD_DWORD_VAL transferLength;
    USB_DEVICE_MSD_DWORD_VAL logicalBlockLength;
    USB_DEVICE_MSD_DWORD_VAL logicalBlockAddress;

    DRV_HANDLE              drvHandle;
    SYS_FS_MEDIA_GEOMETRY * mediaGeometry;

    thisMSDInstance = &gUSBDeviceMSDInstance[iMSD];
   
    /* Pointer to the CBW */ 
    lCBW = (USB_MSD_CBW *)thisMSDInstance->msdCBW; // Pointer to CBW

    /* Logical unit being addressed */
    logicalUnit = lCBW->bCBWLUN;   

    /* Get the media dynamic data */
    mediaDynamicData = &thisMSDInstance->mediaDynamicData[logicalUnit] ;

    /* Pointer to the media functions for this LUN */
    mediaFunctions = &thisMSDInstance->mediaData[logicalUnit].mediaFunctions;

    /* Pointer to the working buffer for this LUN */
    msdBuffer = thisMSDInstance->mediaData[logicalUnit].sectorBuffer;
    writeBlockBackupBuffer = thisMSDInstance->mediaData[logicalUnit].blockBuffer;

    /* Assume that we dont have anything to send or receive */
    (* rxTxBufferSize) = 0;

    /* Assume that the command will pass */ 
    (* commandStatus) = USB_MSD_CSW_COMMAND_PASSED; 

    /* Reset some of the local variables */
    logicalBlockAddress.Val = 0;
    logicalBlockLength.Val = 0;
    transferLength.Val = 0;
    capacity.Val = 0;
    sectorSize.Val = 0;
    drvHandle = mediaDynamicData->mediaHandle;


    /* Process the command */
    switch(lCBW->CBWCB[0])
    {
        case SCSI_INQUIRY:
            
            /* We need to send the response to the SCSI Inquiry.
             * Build the response to the inquiry command */
            *rxTxBufferAdrs = (uintptr_t)&thisMSDInstance->mediaData[logicalUnit].inquiryResponse;
            *rxTxBufferSize = sizeof(SCSI_INQUIRY_RESPONSE);
            if(lCBW->dCBWDataTransferLength <= (*rxTxBufferSize))
            {
                /* Send whatever has been requested by host */
                *rxTxBufferSize = lCBW->dCBWDataTransferLength;
            }
            else
            {
                /* Size of data returned is less than size mentioned by host */
                /* STALL the IN token */
                msdNextState = USB_DEVICE_MSD_STATE_STALL_IN;
            }

            break;

        case SCSI_MODE_SENSE:
            
            /* Pad the bytes with zeroes first */
            memset(msdBuffer, 0, lCBW->dCBWDataTransferLength);
            
            /* Add required information */
            msdBuffer[0] = 0x03;
            msdBuffer[1] = 0x00;
            msdBuffer[2] = 0x00;
            
            if(mediaFunctions->isWriteProtected(drvHandle))
            {
                /* Media is write protected */
                msdBuffer[2] = 0x80;
            }

            msdBuffer[3]= 0x00;
            
            /* Response buffer address */
            *rxTxBufferAdrs = (uintptr_t)&msdBuffer[0];
            
            /* Response buffer size is same as whatever host requested for */
            *rxTxBufferSize = lCBW->dCBWDataTransferLength;
            break;

        case SCSI_REQUEST_SENSE:

            if(mediaDynamicData->mediaPresent == false)
            {
                /* Media is not present */
                mediaDynamicData->gblSenseData.SenseKey = SCSI_SENSE_NOT_READY;
                mediaDynamicData->gblSenseData.ASC = SCSI_ASC_MEDIUM_NOT_PRESENT;
                mediaDynamicData->gblSenseData.ASCQ = SCSI_ASCQ_MEDIUM_NOT_PRESENT;
            }

            /* Request sense. Host requests for a sense, if the device has
             * responded with an error. */
            *rxTxBufferAdrs = (uintptr_t)&mediaDynamicData->gblSenseData;
            *rxTxBufferSize = sizeof(SCSI_SENSE_DATA);

            if(lCBW->dCBWDataTransferLength <= sizeof(SCSI_SENSE_DATA))
            {
                /* Send whatever has been requested by host */
                *rxTxBufferSize = lCBW->dCBWDataTransferLength;
            }
            else
            {
                /* Size of data returned is less than size mentioned by host */
                /* STALL the IN token */
                msdNextState = USB_DEVICE_MSD_STATE_STALL_IN;
            }
            /* Now the host is reading the sense data, we are good to reset in
             * the next CBW stage */
            mediaDynamicData->resetSenseData = true;
            break;

        case SCSI_TEST_UNIT_READY:
           
            /* Next state will be CSW */
            msdNextState = USB_DEVICE_MSD_STATE_CSW;

            if( mediaDynamicData->mediaPresent == false )
            {
                /* Status failed */
                (* commandStatus) = USB_MSD_CSW_COMMAND_FAILED;
            }
            break;


        case SCSI_READ_CAPACITY:

            /* Get the information from the physical media */

            mediaGeometry = mediaDynamicData->mediaGeometry;
            if(mediaGeometry->numWriteRegions != 0)
            {
                /* If there is a write region then return
                 * the size of the write region. The size of
                 * the write region will be available after
                 * read regions.  */

                capacity.Val = ((mediaGeometry->geometryTable[1].numBlocks *
                        mediaGeometry->geometryTable[1].blockSize)/mediaDynamicData->sectorSize) - 1 ;
            }
            else
            {
                /* Return the number of read blocks. Only the first entry is
                 * considered. Refer to the geometry table structure for more
                 * details. */
                capacity.Val = ((mediaGeometry->geometryTable[0].numBlocks *
                        mediaGeometry->geometryTable[0].blockSize)/mediaDynamicData->sectorSize) - 1 ;
            }

            /* Sector size was udpated when the media was opened in
             * the _USB_DEVICE_MSD_CheckAndUpdateMediaState() function */
            sectorSize.Val = mediaDynamicData->sectorSize;

            /* Copy the data to the buffer.  Host expects the response in big
             * endian format */
            msdBuffer[0] = capacity.v[3];
            msdBuffer[1] = capacity.v[2];
            msdBuffer[2] = capacity.v[1];
            msdBuffer[3] = capacity.v[0];

            msdBuffer[4] = sectorSize.v[3];
            msdBuffer[5] = sectorSize.v[2];
            msdBuffer[6] = sectorSize.v[1];
            msdBuffer[7] = sectorSize.v[0];

            *rxTxBufferAdrs = (uintptr_t)&msdBuffer[0];
            *rxTxBufferSize = 8;

            if(lCBW->dCBWDataTransferLength < 8)
            {
                /* Send whatever has been requested by host */
                *rxTxBufferSize = lCBW->dCBWDataTransferLength;
            }

            break;

        case SCSI_READ_10:
            
            /* The host issues a "Read 10" request when it wants to read some
             * number of 10-bit length blocks (512 byte blocks) of data from the
             * media.  Let's first assume that the next CBW state by default is
             * going to be USB_DEVICE_MSD_STATE_DATA_IN. We shall change to CSW
             * state (see code further below) only if we are done reading all
             * the blocks.*/

            msdNextState = USB_DEVICE_MSD_STATE_DATA_IN; 
            _USB_DEVICE_MSD_GetBlockAddressAndLength(lCBW, &logicalBlockAddress, &logicalBlockLength);

            //Case 7 error of MSD spec.
            if(lCBW->dCBWDataTransferLength < logicalBlockLength.Val*mediaDynamicData->sectorSize)
            {
                msdNextState = USB_DEVICE_MSD_STATE_STALL_IN;
                break; 
            }
            
            /* This is the media read block size in bytes. We will need to make
             * a translation from media block size to the sector size */
            mediaReadBlockSize = mediaDynamicData->mediaGeometry->geometryTable[0].blockSize;

            if( mediaDynamicData->mediaState == USB_DEVICE_MSD_MEDIA_OPERATION_IDLE  )
            {
                /* Check if the read for this media is blocking */
                if (mediaDynamicData->mediaGeometry->mediaProperty & SYS_FS_MEDIA_READ_IS_BLOCKING)
                {
                    /* The read is blocking. Set the media state to operation
                     * pending. This state will be updated in the media driver
                     * event callback. */
                    mediaDynamicData->mediaState = USB_DEVICE_MSD_MEDIA_OPERATION_PENDING;

                    /* This is the media read function */
                    mediaFunctions->blockRead
                    (drvHandle, &meadiaReadWriteHandle, (uint8_t*)&msdBuffer[0],
                    (logicalBlockAddress.Val * (mediaDynamicData->sectorSize/mediaReadBlockSize)),
                    (mediaDynamicData->sectorSize/mediaReadBlockSize));

                    /* Update the amount of data read and the sector address
                     * read. */
                    logicalBlockLength.Val --;
                    logicalBlockAddress.Val ++;
                    _USB_DEVICE_MSD_SaveBlockAddressAndLength(lCBW, &logicalBlockAddress, &logicalBlockLength);

                }
                else
                {
                    /* Media read is not blocking. Note that the command handle 
                     * is not tracked as we will do only one operation at a time and
                     * dont need to queue up operation. */
                    
                    mediaFunctions->blockRead
                    (drvHandle, &meadiaReadWriteHandle, (uint8_t*)&msdBuffer[0],
                    (logicalBlockAddress.Val * (mediaDynamicData->sectorSize/mediaReadBlockSize)),
                    (mediaDynamicData->sectorSize/mediaReadBlockSize));

                    /* Because this is non blocking, we can send data
                    * immediately. */
                    mediaDynamicData->mediaState = USB_DEVICE_MSD_MEDIA_OPERATION_COMPLETE;
                    logicalBlockLength.Val --;
                    logicalBlockAddress.Val ++;
                    _USB_DEVICE_MSD_SaveBlockAddressAndLength(lCBW, &logicalBlockAddress, &logicalBlockLength);

                }
            }
            if (meadiaReadWriteHandle == SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID)
            {
                /* Media Read Failed. Stall the next request*/
                msdNextState = USB_DEVICE_MSD_STATE_STALL_IN;
                (*commandStatus) = USB_MSD_CSW_PHASE_ERROR;
            }
            else if(mediaDynamicData->mediaState == USB_DEVICE_MSD_MEDIA_OPERATION_COMPLETE)
            {
                /* In case of blocking read, the media state gets updated in the
                 * event callback. In case of non blocking read, it is updated
                 * immediately after the read is complete. */
                mediaDynamicData->mediaState = USB_DEVICE_MSD_MEDIA_OPERATION_IDLE;
                *rxTxBufferSize = mediaDynamicData->sectorSize; // sector size.
                *rxTxBufferAdrs = (uintptr_t)&msdBuffer[0];
                if(logicalBlockLength.Val <= 0)
                {
                    msdNextState = USB_DEVICE_MSD_STATE_CSW;
                }
            }
 
            break;

        case SCSI_WRITE_10:
            /* We have recieved some data from host. This needs to be written
             * into media. But befrore we write into the media, check if the
             * media is write protected */
            if(mediaFunctions->isWriteProtected( drvHandle ))
            {
                /* Media is write protected. Set sense keys so the host knows
                 * what caused the error. */
                mediaDynamicData->gblSenseData.SenseKey = SCSI_SENSE_DATA_PROTECT;
                mediaDynamicData->gblSenseData.ASC = SCSI_ASC_WRITE_PROTECTED;
                mediaDynamicData->gblSenseData.ASCQ = SCSI_ASCQ_WRITE_PROTECTED;
                
                /* Request main state machine to stall the endpoint. */
                msdNextState = USB_DEVICE_MSD_STATE_STALL_IN;
                /*Indicate in CSW that command failed. */
                (*commandStatus) = USB_MSD_CSW_COMMAND_FAILED;
            }
            else
            {

                /* Media is not write protected. Go ahead writing the media 
                 * Get the logical block address, transfer length fields from
                 * Command Block Wrapper */
                _USB_DEVICE_MSD_GetBlockAddressAndLength(lCBW, &logicalBlockAddress, &logicalBlockLength);

                /* Let's stay in data-out state until we are done writing all
                 * logical blocks */
                msdNextState = USB_DEVICE_MSD_STATE_DATA_OUT;

                if( thisMSDInstance->irpRx.size ) // See if we have some data.
                {
                    /* Yes, we have received a sector worth of data.
                     * Now check if media is free */
                    if(( mediaDynamicData->mediaState == USB_DEVICE_MSD_MEDIA_OPERATION_IDLE  )
                        || (mediaDynamicData->mediaState == USB_DEVICE_MSD_MEDIA_OPERATION_COMPLETE))
                    {
                        if(mediaDynamicData->mediaGeometry->mediaProperty & SYS_FS_MEDIA_WRITE_IS_BLOCKING)
                        {
                            /* Put media into busy state. */
                            mediaDynamicData->mediaState = USB_DEVICE_MSD_MEDIA_OPERATION_PENDING;

                            /* This is the media write block size in bytes. We will need to make
                             * a translation from media block size to the sector size */
                            mediaWriteBlockSize = mediaDynamicData->mediaGeometry->geometryTable[1].blockSize;

                            /* If the media write block size is greater than the
                             * sector size then we need to first read the write
                             * block. */
                            if(mediaWriteBlockSize > mediaDynamicData->sectorSize)
                            {
                                unsigned int sectorsPerBlock = mediaWriteBlockSize/ mediaDynamicData->sectorSize;
                                unsigned int memoryBlock = logicalBlockAddress.Val/sectorsPerBlock;
                                unsigned int sectorOffsetWithinBlock = (logicalBlockAddress.Val - (memoryBlock * sectorsPerBlock)) * mediaDynamicData->sectorSize;
                                unsigned int i;
                                mediaReadBlockSize = mediaDynamicData->mediaGeometry->geometryTable[0].blockSize;

                                /* Read the write block first */
                                mediaFunctions->blockRead(drvHandle, &meadiaReadWriteHandle,
                                        writeBlockBackupBuffer, (memoryBlock * (mediaWriteBlockSize/mediaReadBlockSize)),
                                        (mediaWriteBlockSize/mediaReadBlockSize));

                                /* Update the sector in the write block */
                                for(i = 0; i < mediaDynamicData->sectorSize; i ++)
                                {
                                    writeBlockBackupBuffer[sectorOffsetWithinBlock + i] = msdBuffer[i];
                                }

                                 /* Write data into the media */
                                mediaFunctions->blockWrite (drvHandle, &meadiaReadWriteHandle, (uint8_t*)writeBlockBackupBuffer, memoryBlock, 1);

                            }
                            else
                            {
                                /* Write data into the media */
                                mediaFunctions->blockWrite
                                (drvHandle, &meadiaReadWriteHandle, (uint8_t*)&msdBuffer[0],
                                (logicalBlockAddress.Val * (mediaDynamicData->sectorSize/mediaWriteBlockSize)),
                                (mediaDynamicData->sectorSize/mediaWriteBlockSize));
                            }
                            if (meadiaReadWriteHandle == SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID)
                            {
                                /* Media Read Failed. Stall the next request*/
                                msdNextState = USB_DEVICE_MSD_STATE_STALL_OUT;
                                (*commandStatus) = USB_MSD_CSW_PHASE_ERROR;
                            }
                            else
                            {
                                /* We have written a sector. Now decrement the block. */
                                logicalBlockLength.Val --;
                        
                                /* Increment logical block address. In the next pass write
                                * into this address. */
                                logicalBlockAddress.Val ++;

                                /* Save back the updated address and logical block */
                                _USB_DEVICE_MSD_SaveBlockAddressAndLength(lCBW, &logicalBlockAddress, &logicalBlockLength);

                                if( (logicalBlockLength.Val <= 0) ||
                                    (thisMSDInstance->irpRx.status ==
                                     USB_DEVICE_IRP_STATUS_COMPLETED_SHORT) )
                                {
                                    // We have received all the blocks or host has completed the transfer.
                                    /* End the data stage and move to CSW state */
                                    *rxTxBufferSize = 0; // No more data to be received.
                                    msdNextState = USB_DEVICE_MSD_STATE_CSW;

                                }
                            }
                        }
                        else
                        {
                            /* Write is not blocking */
                        }
                    }
                }

                if(logicalBlockLength.Val > 0 )
                {
                    // We are not yet done receiving all the blocks.
                    // Request the main state machine to receive a sector worth of data.
                    *rxTxBufferSize = mediaDynamicData->sectorSize; // endpoint size
                    // Buffer address, where the main state machine has to copy the incomming data.
                    *rxTxBufferAdrs = (uintptr_t)&msdBuffer[0];
                }
            }              
            break;

        case SCSI_VERIFY:
        case SCSI_STOP_START:
            msdNextState = USB_DEVICE_MSD_STATE_CSW;
            if(mediaDynamicData->mediaPresent == false)
            {
                (* commandStatus) = USB_MSD_CSW_COMMAND_FAILED;
            }
            break;

        case SCSI_PREVENT_ALLOW_MEDIUM_REMOVAL:
            mediaDynamicData->gblSenseData.SenseKey = SCSI_SENSE_ILLEGAL_REQUEST;
            mediaDynamicData->gblSenseData.ASC = SCSI_ASC_INVALID_COMMAND_OPCODE;
            mediaDynamicData->gblSenseData.ASCQ = SCSI_ASCQ_INVALID_COMMAND_OPCODE;
            (* commandStatus) = USB_MSD_CSW_COMMAND_FAILED;
            msdNextState = USB_DEVICE_MSD_STATE_CSW;
            break;

        default:
            _USB_DEVICE_MSD_ResetSenseData(&mediaDynamicData->gblSenseData);
            // Set sense data.
            mediaDynamicData->gblSenseData.SenseKey = SCSI_SENSE_ILLEGAL_REQUEST;
            mediaDynamicData->gblSenseData.ASC = SCSI_ASC_INVALID_COMMAND_OPCODE;
            mediaDynamicData->gblSenseData.ASCQ = SCSI_ASCQ_INVALID_COMMAND_OPCODE;
            /* Decide on the next state in the MSD functin state machine */
            if (lCBW->bmCBWFlags.value & USB_MSD_CBW_DIRECTION_BITMASK)
            {
                /* This means the data stage moves from device
                 * to host */
                msdNextState = USB_DEVICE_MSD_STATE_STALL_IN;
            }
            else
            {
                /* This means the data stage moves from host
                 * to device */
                msdNextState = USB_DEVICE_MSD_STATE_STALL_OUT;
            }
            (* commandStatus) = USB_MSD_CSW_COMMAND_FAILED;          
            break;
    }

    return msdNextState;
}

// *****************************************************************************
/* Function:
    void    _USB_DEVICE_MSD_Deinitialization (SYS_MODULE_INDEX iMSD)

  Summary:
    MSD function driver deinitialization.

  Description:
    Deinitializes the given instance of the MSD function driver. This function
    is called by the USB DEVICE layer.

  Remarks:
    This is a local function and should not be called directly by an
    application.
 
*/

void _USB_DEVICE_MSD_Deinitialization ( SYS_MODULE_INDEX iMSD )
{
    USB_DEVICE_MSD_INSTANCE * thisMSDInstance;
    USB_DEVICE_MSD_MEDIA_FUNCTIONS * mediaFunctions;
    uint8_t count;

    thisMSDInstance = &gUSBDeviceMSDInstance[ iMSD ] ;
    mediaFunctions = &thisMSDInstance->mediaData[0].mediaFunctions;

    // close all open logical units..
    for(count = 0; count < thisMSDInstance->numberOfLogicalUnits; count++)
    {
        if( thisMSDInstance->mediaDynamicData[count].mediaHandle != DRV_HANDLE_INVALID )
        {
            mediaFunctions->close(thisMSDInstance->mediaDynamicData[count].mediaHandle);
        }
    }
    /* Cancel all RX IRPs */
    USB_DEVICE_IRPCancelAll( thisMSDInstance->hUsbDevHandle, thisMSDInstance->bulkEndpointRx );
    
    /* Cancel all TX IRPs */
    USB_DEVICE_IRPCancelAll( thisMSDInstance->hUsbDevHandle, thisMSDInstance->bulkEndpointTx );

    /* Close the endpoint */
    USB_DEVICE_EndpointDisable( thisMSDInstance->hUsbDevHandle, thisMSDInstance->bulkEndpointRx );
    USB_DEVICE_EndpointDisable( thisMSDInstance->hUsbDevHandle, thisMSDInstance->bulkEndpointTx );
}

// ******************************************************************************
/* Function:
     void _USB_DEVICE_MSD_ResetSenseData ( USB_DEVICE_MSD_SENSE_DATA * senseData )

  Summary:
    MSD function driver deinitialization.
  
  Description:
    This routine resets the Sense Data, initializing the structure
    USB_DEVICE_MSD_SENSE_DATA gblSenseData.

  Remarks:
    This is a local function and should not be called directly by an
    application.
*/

void _USB_DEVICE_MSD_ResetSenseData( SCSI_SENSE_DATA * senseData )
{
   	senseData->ResponseCode = SCSI_SENSE_CURRENT;
	senseData->VALID = 0;			// no data in the information field
	senseData->Obsolete = 0x0;
	senseData->SenseKey = SCSI_SENSE_NO_SENSE;
	//gblSenseData.Resv;
	senseData->ILI = 0;
	senseData->EOM = 0;
	senseData->FILEMARK = 0;
	senseData->InformationB0 = 0x00;
	senseData->InformationB1 = 0x00;
	senseData->InformationB2 = 0x00;
	senseData->InformationB3 = 0x00;
	senseData->AddSenseLen = 0x0a;	// n-7 (n=17 (0..17))
	senseData->CmdSpecificInfo = 0x00000000;
	senseData->ASC = 0x0;
	senseData->ASCQ = 0x0;
	senseData->FRUC = 0x0;
	senseData->SenseKeySpecific[0] = 0x0;
	senseData->SenseKeySpecific[1] = 0x0;
	senseData->SenseKeySpecific[2] = 0x0;
}

