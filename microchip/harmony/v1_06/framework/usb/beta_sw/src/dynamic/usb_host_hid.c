/*******************************************************************************
 USB HOST HID Class Driver

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hid.c

  Summary:
    USB HID Host class driver.

  Description:
    USB HID Host class driver.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

 Microchip licenses to you the right to use, modify, copy and distribute
 Software only when embedded on a Microchip microcontroller or digital signal
 controller that is integrated into your product or third party product
 (pursuant to the sublicense terms in the accompanying license agreement).

 You should refer to the license agreement accompanying this Software for
 additional information regarding your rights and obligations.

 SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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

#include "system_config.h"
#include "usb/beta_sw/usb_host.h"
#include "usb/beta_sw/src/usb_host_local.h"
#include "usb/beta_sw/usb_host_hid.h"
#include "usb/beta_sw/src/usb_host_hid_local.h"
#include "system/debug/sys_debug.h"



// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data Types
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
/* HID Host driver structure

  Summary:
    Defines the Host class driver structure required for the host layer.

  Description:
    This data type defines the host driver structure required for the
    host layer.

  Remarks:
    This structure is private to the USB Host stack.
 */
USB_HOST_CLASS_DRIVER hidDriver =
{
     /* HID device level init function */
    .initializeDeviceDriver = NULL ,

    /* HID interface level init function */
    .initializeInterfaceDriver = _USB_HOST_HID_InterfaceInitialize ,

    /* HID de-init function */
    .deInitializeDriver = _USB_HOST_HID_DeInitialize ,

    /* HID tasks function */
    .task           = _USB_HOST_HID_Tasks

};

// *****************************************************************************
/* HID Host Instance structure

  Summary:
    Defines the HID instance(s).

  Description:
    This data type defines the HID instance(s). The number of instances is
    defined by the application using USB_HOST_HID_INSTANCES_NUMBER.

  Remarks:
    This structure is private to the HID.
 */

USB_HOST_HID_INSTANCE  gUSBHostHIDInstance[USB_HOST_HID_INSTANCES_NUMBER];


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

// ******************************************************************************
/* Function:
    USB_ERROR   _USB_HOST_HID_InterfaceInitialize
	( 
	    USB_HOST_DEVICE_ID id ,
        uint8_t interfaceId,
        uint8_t *hidInstance
        uint8_t  speed
	)

  Summary:
    USB Host HID function is called by the host layer.

  Description:
    USB Host HID function called by the host layer after interface descriptor
    values are matching with TPL list. It initializes the HID client driver
	instance.

  Remarks:
    This is local function and should not be called directly by the application.
*/
USB_ERROR     _USB_HOST_HID_InterfaceInitialize
( 
    USB_HOST_DEVICE_ID deviceId ,
    uint8_t interfaceId,
    uint8_t *hidInstance,
    USB_SPEED speed
)

{
    /* Start of local variables */
    USB_HOST_QUERY           hidQuery;
    USB_ERROR                status = USB_ERROR_NONE ;
    USB_INTERFACE_DESCRIPTOR *interfaceDesc = (USB_INTERFACE_DESCRIPTOR *) NULL;
    USB_HID_DESCRIPTOR       *hidClassDesc = (USB_HID_DESCRIPTOR *) NULL;
    USB_ENDPOINT_DESCRIPTOR  *interruptInEndpointDesc = (USB_ENDPOINT_DESCRIPTOR  *) NULL;
    USB_ENDPOINT_DESCRIPTOR  controlEndpointDesc;
    USB_HOST_HID_INSTANCE    *hidInstanceInfo = (USB_HOST_HID_INSTANCE    *) NULL;
    USB_HOST_HID_DEVICE_INFO *hidDeviceInfo = (USB_HOST_HID_DEVICE_INFO *)NULL;
    uint8_t                  instanceCount = 0;
    /* End of local variables */

    /* Find the free space for this HID instance */
    for( instanceCount = 0 ; instanceCount < USB_HOST_HID_INSTANCES_NUMBER ;
                                                 instanceCount ++)
    {
        hidInstanceInfo = &(gUSBHostHIDInstance[instanceCount]);
        if (hidInstanceInfo->assigned == false )
        {
            /* Found free slot */
            break;
        }
    }
    /* validate the instance */
    if ( instanceCount >= USB_HOST_HID_INSTANCES_NUMBER)
    {
        /* Assert on invalid HID index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_HID_InterfaceInitialize : Invalid HID instance number" );
        return USB_ERROR_HOST_DEVICE_INSTANCE_INVALID;
    }

    /* count is the instance number return to host */
    *hidInstance = instanceCount;
    /* Mark flag as use */
    hidInstanceInfo->assigned = true;

    /* store the Device address in instanceInfo */
    hidInstanceInfo->hidDeviceId = deviceId;

    /* save instance number */
    hidInstanceInfo->instanceNumber = instanceCount;

    /* HID interface Number */
    hidInstanceInfo->interfaceNumber = interfaceId;
    /* Fill the query */
    hidQuery.id       = deviceId;
    hidQuery.class    = USB_HID_CLASS_CODE;
    hidQuery.subClass = USB_HID_SUBCLASS_CODE_BOOT_INTERFACE_SUBCLASS;
    hidQuery.protocol = USB_HID_PROTOCOL_CODE_MOUSE;
    /* class match */
    hidQuery.flags    = USB_HOST_MATCH_CLASS_SUBCLASS_PROTOCOL;
    hidQuery.interfaceNumber = interfaceId;
    hidQuery.alternateSettingNumber  = 0x00;

    /* Find interface */
    status = _USB_HOST_FindInterface(&(hidQuery), &(interfaceDesc));

    if ( USB_ERROR_NONE != status )
    {
         SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_HID_InterfaceInitialize : Interface not found " );
        return ( USB_ERROR_HOST_INTERFACE_NOT_FOUND );
    }

    /* Error checking and extracting information */
    if(((uint8_t *)interfaceDesc) + sizeof(USB_INTERFACE_DESCRIPTOR) != NULL)
    {
        hidClassDesc = (USB_HID_DESCRIPTOR *)(((uint8_t *)interfaceDesc) +
                sizeof(USB_INTERFACE_DESCRIPTOR));
        if(hidClassDesc->bDescriptorType != (uint8_t) USB_HID_DESCRIPTOR_TYPES_HID)
        {
            /* No HID Class descriptor */
            SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_HID_InterfaceInitialize : HID class desc not found " );
            return ( USB_ERROR_HOST_INTERFACE_NOT_FOUND );
        }
        
        /* Extract device information */
        hidDeviceInfo = &(hidInstanceInfo->hidDeviceInfo);
        hidDeviceInfo->countryCode = hidClassDesc->bcdHID;
        hidDeviceInfo->isBootInterfaceClass = interfaceDesc->bInterfaceSubClass;
        if(USB_HID_PROTOCOL_CODE_KEYBOARD == interfaceDesc->bInterfaceProtocol)
        {
            hidDeviceInfo->isKeyboardDevice = 1;
        }
        else
        {
            hidDeviceInfo->isKeyboardDevice = 0;
        }
    }

    /* Get control pipe information */
    controlEndpointDesc.transferType =  USB_TRANSFER_TYPE_CONTROL ;
    hidInstanceInfo->controlPipeHandle =  
            _USB_HOST_PipeSetup ( deviceId ,  &controlEndpointDesc , speed );

    if ( USB_HOST_PIPE_HANDLE_INVALID == hidInstanceInfo->controlPipeHandle )
    {
         SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
          " _USB_HOST_HID_InterfaceInitialize : control pipe handle not found ");
         return ( USB_ERROR_HOST_DEVICE_INSTANCE_INVALID );
    }
    /* Fill Control IRP */
    hidInstanceInfo->controlIRP.callback = _USB_HOST_HID_ControlTransferComplete;

    hidInstanceInfo->controlIRP.userData = (uintptr_t )hidInstanceInfo;
    hidQuery.alternateSettingNumber = interfaceDesc->bAlternateSetting;
   
   /* Find interruptIn endpoint descriptor */
    hidQuery.endpointType      = USB_TRANSFER_TYPE_INTERRUPT;
    hidQuery.endpointDirection = USB_DATA_DIRECTION_DEVICE_TO_HOST ;
    hidQuery.flags =    USB_HOST_MATCH_ENDPOINT_TYPE |
                        USB_HOST_MATCH_ENDPOINT_DIRECTION;

    /* Find the Interrupt IN  endpoint descriptor */
    status = _USB_HOST_FindEndpoint (&(hidQuery), &(interruptInEndpointDesc));
    if(  USB_ERROR_NONE  != status  )
    {
         SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
          " _USB_HOST_HID_InterfaceInitialize : interrupt in pipe handle not found ");
        return USB_ERROR_HOST_ENDPOINT_INVALID;
    }

    /* Save the interrupt IN endpoint address */
    hidInstanceInfo->interruptInEndpoint =
            interruptInEndpointDesc->bEndpointAddress;
    /* Save the interrupt IN endpoint size */
    hidInstanceInfo->interruptInEndpointSize =
            (uint8_t)interruptInEndpointDesc->wMaxPacketSize;
    /* Create Interrupt in pipe */
    hidInstanceInfo->interruptInPipeHandle = _USB_HOST_PipeSetup
                                ( deviceId ,  interruptInEndpointDesc ,speed );
    if( hidInstanceInfo->interruptInPipeHandle == USB_HOST_PIPE_HANDLE_INVALID )
    {
        SYS_DEBUG ( 0 , "Interrupt IN  pipe is not created " );
        return USB_ERROR_HOST_PIPE_INVALID;
    }
    /* Fill Interrupt in IRP */
    hidInstanceInfo->interruptInIRP.callback =
                            (void * )_USB_HOST_HID_InterruptTransferComplete;
    hidInstanceInfo->hidReadBuffer = hidReadBuffer;
    /* HID Client driver attached */
    hidInstanceInfo->state = USB_HOST_HID_ATTACHED;
    return status;
}/* end of _USB_HOST_HID_InterfaceInitialize() */

// ******************************************************************************
/* Function:
   void  _USB_HOST_HID_DeInitialize(USB_HOST_DEVICE_ID id )

  Summary:
    DeInitialize the HID host driver.

  Description:
    This USB Host HID function is called by the host layer once device is 
	detached from the host.
    Function de-initializes the HID driver. Pipes
    will be closed and appropriate data structures released.

  Remarks:
    This is local function and should not be called directly by the application.
*/
void  _USB_HOST_HID_DeInitialize(USB_HOST_DEVICE_ID id )
 {
    /* Start of local variables */
    USB_HOST_HID_INSTANCE *hidInstanceInfo = (USB_HOST_HID_INSTANCE *) NULL;
    uint8_t hidInstance = 0;
    /* End of local variables */
	
    /* Find instance */
    hidInstance = _USB_HOST_HID_findInstance( id );
    if(hidInstance != USB_HOST_HID_INSTANCE_INVALID)
    {
        hidInstanceInfo = &(gUSBHostHIDInstance[hidInstance]);
        /* Close HID device related pipes */
        _USB_HOST_PipeClose( hidInstanceInfo->controlPipeHandle );
        _USB_HOST_PipeClose( hidInstanceInfo->interruptInPipeHandle );

        hidInstanceInfo->controlPipeHandle = USB_HOST_PIPE_HANDLE_INVALID;
        hidInstanceInfo->interruptInPipeHandle = USB_HOST_PIPE_HANDLE_INVALID;

        hidInstanceInfo->assigned = false;
        /* This is required for the task routine to continue on next attach */
        hidInstanceInfo->interruptInIRP.status = USB_HOST_IRP_STATUS_COMPLETED;

	/* inform to the hid device detach event to the application */
        hidInstanceInfo->appEventCallBack ( hidInstanceInfo->instanceNumber ,
                               USB_HOST_HID_EVENT_DETACH ,
                               &(hidInstanceInfo->hidDeviceInfo) );
        
        hidInstanceInfo->state = USB_HOST_HID_WAIT ;
    }
    else
    {
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
            " _USB_HOST_HID_DeInitialize : instance not found for this device ");
    }

 }/* end of _USB_HOST_HID_DeInitialize() */

/*************************************************************************/
/* Function:
   USB_HOST_HID_HANDLE  USB_HOST_HID_Open( uint8_t  instance )

  Summary:
   Application opens a HID instance.

  Description:
    Function facilitates to open HID client driver. Valid handle is passed
	on success.

  Remarks:
    Refer to usb_host_hid.h for usage information.
*/
USB_HOST_HID_HANDLE  USB_HOST_HID_Open( uint8_t  instance )
{
    /* Start of local variables */
    USB_HOST_HID_INSTANCE  *hidInstanceInfo = (USB_HOST_HID_INSTANCE  *) NULL;
    USB_HOST_HID_HANDLE hidHandle = USB_HOST_HID_HANDLE_INVALID;
    /* End of local variables */
	
    /* Get instance driver information */
    hidInstanceInfo = &(gUSBHostHIDInstance[instance]);

    /* Check if instance has been initialized */	
    if(hidInstanceInfo->assigned == 0x01 )
    {
	/* Update Valid handle */
        hidHandle = (USB_HOST_HID_HANDLE  )hidInstanceInfo;
    }
    else
    {
        /* Instance not yet initialized */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
                " USB_HOST_HID_Open : instance not yet initialized ");
    }
    return hidHandle;
}/* end of USB_HOST_HID_Open() */

/****************************************************************************/
/* Function:
   void  USB_HOST_HID_Close( USB_HOST_HID_HANDLE hidHandle )

  Summary:
    Closes handle to an instance of the USB HID driver.

  Description:
   This function closes an opened handle to an instance of the USB HID,
   invalidating the handle.

   Remarks:
    Refer to usb_host_hid.h for usage information.
*/
void  USB_HOST_HID_Close( USB_HOST_HID_HANDLE hidHandle )
{
    /* Start of local variable */
    USB_HOST_HID_INSTANCE  *hidInstanceInfo = (USB_HOST_HID_INSTANCE  *) NULL;
    /* End of local variable */
	
    hidInstanceInfo = (USB_HOST_HID_INSTANCE  *)hidHandle;
    /* Checking if the handle is valid */
    if(hidInstanceInfo != NULL)
    {
        if(hidInstanceInfo->assigned != 0x01)
        {
            SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
                    " USB_HOST_HID_Close : instance not yet initialized ");
        }
    }
    /* Invalidate the handle */
    hidHandle = (USB_HOST_HID_HANDLE  ) NULL;
}/* end of USB_HOST_HID_Close() */


/*************************************************************************/
/* Function:
   USB_ERROR USB_HOST_HID_EventHandlerSet
   (
       USB_HOST_HID_EVENT_HANDLER appHIDHandler
   )

  Summary:
    Application callback function is registered

  Description:
    This is an API function which registers application callback function
	with the HID client driver. Various HID events are notified
	to the application by calling the registered callback function.

  Remarks:
    Refer to usb_host_hid.h for usage information.
*/
USB_ERROR USB_HOST_HID_EventHandlerSet
(
    USB_HOST_HID_EVENT_HANDLER appHIDHandler
)
{
    /* Start of local variables */
    USB_HOST_HID_INDEX        HIDInstance;
    USB_HOST_HID_INSTANCE    *hidInstanceInfo = (USB_HOST_HID_INSTANCE *)NULL;
    /* End of local variables */

    /* check if the given event handler is valid */
    if ( appHIDHandler )
    {
        for ( HIDInstance = 0 ; HIDInstance < USB_HOST_HID_INSTANCES_NUMBER ; HIDInstance++ )
        {
            hidInstanceInfo = &(gUSBHostHIDInstance[HIDInstance]);
            /* update the event handler for this instance */
            hidInstanceInfo->appEventCallBack = appHIDHandler;
        }  
        /* return success */
        return USB_ERROR_NONE;
    }
    return USB_HOST_HID_INSTANCE_INVALID;
}/* end of USB_HOST_HID_EventHandlerSet() */

/*************************************************************************/
/* Function:
   uint8_t  _USB_HOST_HID_findInstance(USB_HOST_DEVICE_ID id )

  Summary:
    Find the HID instance

  Description:
    This is an internal function which obtains the HID instance number
	for the queried device ID.

  Remarks:
    This is local function and should not be called directly by the application.
*/
uint8_t  _USB_HOST_HID_findInstance (USB_HOST_DEVICE_ID id )
{
    /* Start of local variable */
    USB_HOST_HID_INSTANCE *hidInstanceInfo = (USB_HOST_HID_INSTANCE *) NULL;
    uint8_t count = 0;
    /* End of local variable */
    
    for(count = 0 ; count < USB_HOST_HID_INSTANCES_NUMBER ; count ++)
    {
        hidInstanceInfo = &(gUSBHostHIDInstance[count]);
	/* Check if device ID matches */
        if ( hidInstanceInfo->hidDeviceId == id )
        {
            /* Matching device found. Return the instance number */
            return count;
        }
    }
    /* On failure */
    return USB_HOST_HID_INSTANCE_INVALID ;
}/* end of _USB_HOST_HID_findInstance() */

// ******************************************************************************
/* Function:
    USB_ERROR   _USB_HOST_HID_setIdleRate
    (
        uint8_t  instanceNumber,
        uint16_t hidKeyRepeatDelay
    )
 
  Summary:
    SET IDLE packet is send
  
  Description:
    Function facilitates in forming of set up packet and submission of IRP
	for sending SET IDLE request.

  Remarks:
    This is local function and should not be called directly by the application.
*/
USB_ERROR   _USB_HOST_HID_setIdleRate
(
    uint8_t  instanceNumber,
    uint16_t hidKeyRepeatDelay
)
{
    /* Start of local variable */
    USB_ERROR status;
    USB_HOST_HID_INSTANCE *hidInstanceInfo = &(gUSBHostHIDInstance[instanceNumber]);
    /* End of local variable */

    /* Fill setup packet */
    hidInstanceInfo->setupPacket.bmRequestType  = ( USB_SETUP_TYPE_CLASS |
                                                  USB_SETUP_RECIPIENT_INTERFACE );
    hidInstanceInfo->setupPacket.bRequest =  USB_HID_REQUESTS_SET_IDLE ;
    hidInstanceInfo->setupPacket.wValue = hidKeyRepeatDelay;
    hidInstanceInfo->setupPacket.wIndex = hidInstanceInfo->interfaceNumber;
    hidInstanceInfo->setupPacket.wLength = 0x0;

    /* Fill IRP */
    hidInstanceInfo->controlIRP.data = NULL;
    hidInstanceInfo->controlIRP.flags = 0x00;
    hidInstanceInfo->controlIRP.size = 0;
    hidInstanceInfo->controlIRP.setup = &( hidInstanceInfo->setupPacket ) ;
    /* keep track of setup request */
    hidInstanceInfo->setupRequest = USB_HID_REQUESTS_SET_IDLE ;
    /* submit IRP */
    status = _USB_HOST_IRPSubmit ( hidInstanceInfo->controlPipeHandle,
                                &(hidInstanceInfo->controlIRP));
    if ( status != USB_ERROR_NONE)
        return USB_HOST_HID_ERROR;
    /* success */
    return USB_HOST_HID_RESULT_OK;
 
}/* end of _USB_HOST_HID_setIdleRate() */

/*************************************************************************/
/* Function:
   void _USB_HOST_HID_InterruptTransferComplete(USB_HOST_IRP *irp)

  Summary:
    Call back function for Interrupt IRP status

  Description:
    This function is a Interrupt transfer call back function. Function
	checks for the transfer completion status and resubmits IRP on success.
	On failure case appropriate error recovery is initiated.

  Remarks:
    This is local function and should not be called directly by the application.
*/
void _USB_HOST_HID_InterruptTransferComplete( USB_HOST_IRP *irp)
{
    /* Start of local variable */
    uint8_t hidInstance;
    USB_HOST_HID_INSTANCE *hidInstanceInfo = (USB_HOST_HID_INSTANCE *)NULL;
    /* End of local variable */

    hidInstanceInfo = ((USB_HOST_HID_INSTANCE *) (irp->userData));
    hidInstance = hidInstanceInfo->instanceNumber;

    if( irp->status ==  USB_HOST_IRP_STATUS_ERROR_STALL )
    {
          hidInstanceInfo->state = USB_HOST_CLEAR_INTERRUPTIN_ENDPOINT;
    }
    else if( irp->status ==   USB_HOST_IRP_STATUS_ABORTED  )
    {
          /* Nothing */
    }
    else if( (irp->status == USB_HOST_IRP_STATUS_COMPLETED) ||
            (irp->status == USB_HOST_IRP_STATUS_COMPLETED_SHORT) )
    {
        if(hidInstanceInfo->appEventCallBack != NULL && (hidInstanceInfo->assigned))
        {
            if(hidInstanceInfo->hidDeviceInfo.isKeyboardDevice)
            {
                /* Check if non modifier key has changed */
                if(hidInstanceInfo->lastNonmodifier != ((uint8_t *)(irp->data))[0])
                {
                    hidInstanceInfo->lastNonmodifier = ((uint8_t *)(irp->data))[0];
                    _USB_HOST_HID_sendReport(hidInstanceInfo);
                }
            }
            hidInstanceInfo->appEventCallBack(hidInstanceInfo->instanceNumber,
                        USB_HOST_HID_EVENT_REPORT_RECEIVED, (irp->data));
            
            /* Submit the next IRP */
            hidInstanceInfo->interruptInIRP.size =
                        hidInstanceInfo->interruptInEndpointSize;
            hidInstanceInfo->interruptInIRP.userData =
                        (uintptr_t )hidInstanceInfo;
            hidInstanceInfo->interruptInIRP.data =
                        (void *)(hidInstanceInfo->hidReadBuffer);
            _USB_HOST_IRPSubmit(hidInstanceInfo->interruptInPipeHandle,
                    &(hidInstanceInfo->interruptInIRP));
        }
    }
}/* end of _USB_HOST_HID_InterruptTransferComplete() */

/*************************************************************************/
/* Function:
   _USB_HOST_HID_ControlTransferComplete( USB_HOST_IRP *irp)

  Summary:
    Call back function for control transfer status

  Description:
    This function is a control transfer call back function. Function
	checks for the control request and does necessary processing as
	per the request.

  Remarks:
    This is local function and should not be called directly by the application.
*/
void _USB_HOST_HID_ControlTransferComplete( USB_HOST_IRP *irp)
{
    /* Start of local variables */
    USB_HOST_HID_INSTANCE *hidInstanceInfo = (USB_HOST_HID_INSTANCE *)NULL;
	/* End of local variables */

    hidInstanceInfo = ((USB_HOST_HID_INSTANCE *) (irp->userData));

    switch ( hidInstanceInfo->setupRequest )
    {
        case USB_HID_REQUESTS_SET_IDLE:
            hidInstanceInfo->state = USB_HOST_HID_SEND_SET_PROTOCOL;
            if (hidInstanceInfo->controlIRP.status ==
                                USB_HOST_IRP_STATUS_COMPLETED)
            {
                /* SET IDLE passed */
            }
            else
            {
                /* SET IDLE failed. As per specification SET IDLE is optional
                 * for both boot and non-boot mouse. For e.g. Microsoft mouse
                 * does NOT support SET_IDLE request and STALLs it.
                 * For keyboard it is mandatory. */
                 
                /* Check if it is a keyboard */
                if(1 == (hidInstanceInfo->hidDeviceInfo.isKeyboardDevice))
                {
                    /* Failure case */
                    hidInstanceInfo->state = USB_HOST_HID_DETACHED;
                }
            }
            break;
            
        case USB_HID_REQUESTS_SET_PROTOCOL :
            hidInstanceInfo->state = USB_HOST_HID_READY;
            if (!(hidInstanceInfo->controlIRP.status ==
                                USB_HOST_IRP_STATUS_COMPLETED))
            {
                /* SET PROTOCOL request has failed. It can fail for mouse
                 * as it is optional for mouse.For Keyboard device it is mandatory
                 */
                if(1 == (hidInstanceInfo->hidDeviceInfo.isKeyboardDevice))
                {
                    SYS_DEBUG ( 0 , "SET PROTOCOL request failed for keyboard type device" );
                    hidInstanceInfo->state = USB_HOST_HID_DETACHED;
                }
            }
            break;
            
        case USB_HID_REQUESTS_SET_REPORT:
            if (!(hidInstanceInfo->controlIRP.status ==
                                USB_HOST_IRP_STATUS_COMPLETED))
            {
                SYS_DEBUG ( 0 , "SET REPORT request failed" );
            }
            break;

        case USB_REQUEST_CLEAR_FEATURE:
            if (!(hidInstanceInfo->controlIRP.status ==
                                USB_HOST_IRP_STATUS_COMPLETED))
            {
                SYS_DEBUG ( 0 , "CLEAR ENDPOINT request failed" );
                hidInstanceInfo->state = USB_HOST_HID_DETACHED;
            }
            else
            {
                hidInstanceInfo->state = USB_HOST_HID_READY;
            }
            break;
            
        default :
            break;

    }
}/* end of _USB_HOST_HID_ControlTransferComplete() */

// ******************************************************************************
/* Function:
    void _USB_HOST_HID_Tasks(uint8_t instanceNumber)
 
  Summary:
    State machine of the HID client driver
  
  Description:
    Function runs the HID client driver state machine and facilitates device
	enumeration and HID data processing activities. 
	Once the HID client driver is initialized, this function should be
	periodically called from the USB Host layer for HID protocol related
	activities.

  Remarks:
    This is local function and should not be called directly by the application.
*/
void _USB_HOST_HID_Tasks(uint8_t instanceNumber)
{
    /* Start of local variable */
    USB_HOST_HID_INSTANCE *hidInstanceInfo;
    uint16_t hidKeyRepeatDelay = 0;
    /* End of local variable */

    hidInstanceInfo = &(gUSBHostHIDInstance[instanceNumber]);

    if (hidInstanceInfo == NULL )
        return;
    switch(hidInstanceInfo->state)
    {
        case USB_HOST_HID_ATTACHED:
            /*
             * The recommended idle rate is infinite(value 0) for mouse and 500 ms(value 125)
             * for keyboard. We will start with the HID specification recommended
             * values.
             */
            if(0 == hidInstanceInfo->hidDeviceInfo.isKeyboardDevice)
            {
                /* Mouse device */
                hidKeyRepeatDelay = 0;
            }
            else
            {
                /* Keyboard device */
                /* Key Repeat Delay has 4 millisecs resoltuion */
                hidKeyRepeatDelay = (hidInstanceInfo->hidKeyRepeatDelay) / 4;
            }
            hidInstanceInfo->state = USB_HOST_HID_SET_IDLE_SEND;
            hidInstanceInfo->appEventCallBack(hidInstanceInfo->instanceNumber,
                        USB_HOST_HID_EVENT_ATTACH, &(hidInstanceInfo->hidDeviceInfo));
            if(_USB_HOST_HID_setIdleRate (hidInstanceInfo->instanceNumber, hidKeyRepeatDelay) < 0)
            {
                hidInstanceInfo->state = USB_HOST_HID_DETACHED;
                SYS_DEBUG ( 0 , "SET IDLE request submit failed for keyboard type device" );
            }
            
            break;
            
        case USB_HOST_HID_SET_IDLE_SEND:
            /* Do nothing. The state will be moved from from CONTROL transfer
             complete handler*/
            break;
            
        case USB_HOST_HID_SEND_SET_PROTOCOL:
            /* Checking if Boot Interface Subclass is supported */
            if(hidInstanceInfo->hidDeviceInfo.isBootInterfaceClass)
            {
                /* Sending SET PROTOCOL request */
                if(_USB_HOST_HID_setProtocol(hidInstanceInfo->instanceNumber, 0) < 0)
                {
                    hidInstanceInfo->state = USB_HOST_HID_DETACHED;
                    SYS_DEBUG ( 0 , "SET PROTOCOL request submit failed for keyboard type device" );
                }
                else
                {
                    hidInstanceInfo->state = USB_HOST_HID_SET_PROTOCOL_SEND;
                }
            }/* end of if (Device supports Boot Interface) */
            else
            {
                /* Does not support boot interface */
                hidInstanceInfo->state = USB_HOST_HID_DETACHED;
            }
            break;
            
        case USB_HOST_HID_SET_PROTOCOL_SEND:
            /* Do nothing. The state will be moved from from CONTROL transfer
             complete handler*/
            break;

        case USB_HOST_HID_READY:
            if(hidInstanceInfo->interruptInIRP.status <=
                    USB_HOST_IRP_STATUS_COMPLETED_SHORT)
            {
                /* On HID device attach, first time IRP is submitted
                 * from here. From then onwards it is submitted from
                 * interrupt transfer handler function.
                 *
                 * If the IRP ends with STALL error or is aborted,then
                 * again it is submitted from here(1st time) and from the
                 * interrupt transfer handler function going forward.
                 * 
                 */
                hidInstanceInfo->interruptInIRP.size =
                        hidInstanceInfo->interruptInEndpointSize;
                hidInstanceInfo->interruptInIRP.userData =
                        (uintptr_t )hidInstanceInfo;
                hidInstanceInfo->interruptInIRP.data =
                        (void *)(hidInstanceInfo->hidReadBuffer);
                _USB_HOST_IRPSubmit(hidInstanceInfo->interruptInPipeHandle,
                    &(hidInstanceInfo->interruptInIRP));
            }
            break;
        case USB_HOST_CLEAR_INTERRUPTIN_ENDPOINT:
            hidInstanceInfo->state = USB_HOST_CLEAR_INTERRUPTIN_ENDPOINT_DONE;
            _USB_HOST_HID_ClearEndpointHalt(hidInstanceInfo->instanceNumber,
                       hidInstanceInfo->interruptInEndpoint, _USB_HOST_HID_ControlTransferComplete);
            break;
        case USB_HOST_CLEAR_INTERRUPTIN_ENDPOINT_DONE:
            /* Do nothing. The state will be changed from CONTROL transfer handler */
            break;
        case USB_HOST_HID_DETACHED:
            if(hidInstanceInfo->appEventCallBack != NULL)
            {
                hidInstanceInfo->appEventCallBack(hidInstanceInfo->instanceNumber,
                        USB_HOST_HID_EVENT_DETACH, &(hidInstanceInfo->hidDeviceInfo));
            }
            hidInstanceInfo->state = USB_HOST_HID_WAIT;
            break;
        case USB_HOST_HID_WAIT:
            /* Waiting for interface initialize from Host layer */
            break;
            
        default:
            break;
    }/* end of switch() */
    
}/* end of USB_HOST_HID_Tasks() */

// ******************************************************************************
/* Function:
    USB_ERROR   _USB_HOST_HID_setProtocol
    (
        uint8_t  instanceNumber,
        bool protocolType
    )
 
  Summary:
    SET PROTOCOL packet is send
  
  Description:
    Function facilitates in forming of set up packet and submission of IRP
	for sending SET PROTOCOL request. Protocol can be BOOT Protocol or
	Report Protocol.

  Remarks:
    This is local function and should not be called directly by the application.
*/
USB_ERROR   _USB_HOST_HID_setProtocol
(
    uint8_t  instanceNumber,
    bool protocolType
)
{
    /* Start of local variable */
    USB_ERROR status;
    USB_HOST_HID_INSTANCE *hidInstanceInfo = &(gUSBHostHIDInstance[instanceNumber]);
    /* End of local variable */

    /* Fill setup packet */
    hidInstanceInfo->setupPacket.bmRequestType  = ( USB_SETUP_TYPE_CLASS |
                                                  USB_SETUP_RECIPIENT_INTERFACE );
    hidInstanceInfo->setupPacket.bRequest =  USB_HID_REQUESTS_SET_PROTOCOL ;
    hidInstanceInfo->setupPacket.wValue = (uint16_t)protocolType; /* 0 = Boot Protocol */
    hidInstanceInfo->setupPacket.wIndex = hidInstanceInfo->interfaceNumber;
    hidInstanceInfo->setupPacket.wLength = 0x0;

    /* Fill IRP */
    hidInstanceInfo->controlIRP.data = NULL;
    hidInstanceInfo->controlIRP.flags = 0x00;
    hidInstanceInfo->controlIRP.size = 0;
    hidInstanceInfo->controlIRP.setup = &( hidInstanceInfo->setupPacket ) ;
    /* keep track of setup request */
    hidInstanceInfo->setupRequest = USB_HID_REQUESTS_SET_PROTOCOL ;
    /* submit IRP */
    status = _USB_HOST_IRPSubmit ( hidInstanceInfo->controlPipeHandle,
                                &(hidInstanceInfo->controlIRP));
    if ( status != USB_ERROR_NONE)
        return USB_HOST_HID_ERROR;
    /* success */
    return USB_HOST_HID_RESULT_OK;
    
}/* end of _USB_HOST_HID_setProtocol() */

// ******************************************************************************
/* Function:
    USB_ERROR _USB_HOST_HID_ClearEndpointHalt
    (
        uint8_t  instanceNumber,
        uint8_t endpointNumber,
        void* callback
    )
 
  Summary:
    Clears a STALL condition on an endpoint.
  
  Description:
    Function facilitates in forming of set up packet and submission of IRP
	for sending CLEAR FEATURE request.

  Remarks:
    This is local function and should not be called directly by the application.
*/
USB_ERROR _USB_HOST_HID_ClearEndpointHalt
(
    uint8_t  instanceNumber,
    uint8_t endpointNumber,
    void* callback
)
{
    /* Start of local variables */
    USB_ERROR status = USB_ERROR_NONE;
    USB_HOST_HID_INSTANCE *hidInstanceInfo = (USB_HOST_HID_INSTANCE *) NULL;
    /* End of local variables */

    hidInstanceInfo = &(gUSBHostHIDInstance[instanceNumber]);
    /* Cancel all transfers */

    _USB_HOST_FillSetupPacket(
                   &(hidInstanceInfo->setupPacket),
                   ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                     USB_SETUP_TYPE_STANDARD |
                     USB_SETUP_RECIPIENT_ENDPOINT ),
                     USB_REQUEST_CLEAR_FEATURE ,
                     USB_FEATURE_SELECTOR_ENDPOINT_HALT , endpointNumber  ,0 ) ;

    /* Fill irp */
    hidInstanceInfo->controlIRP.data = NULL;
    hidInstanceInfo->controlIRP.setup = & ( hidInstanceInfo->setupPacket );
    hidInstanceInfo->controlIRP.size = 0;
    hidInstanceInfo->controlIRP.callback = ( USB_HOST_IRP_CALLBACK ) callback;

    hidInstanceInfo->setupRequest = USB_REQUEST_CLEAR_FEATURE ;
    status = _USB_HOST_IRPSubmit( hidInstanceInfo->controlPipeHandle,
                                &hidInstanceInfo->controlIRP );

    return status;
 }/* end of _USB_HOST_HID_ClearEndpointHalt() */

 // ******************************************************************************
/* Function:
    USB_ERROR _USB_HOST_HID_sendReport( USB_HOST_HID_INSTANCE * hidInstanceInfo)
 
  Summary:
    SET REPORT packet is send
  
  Description:
    Function facilitates in forming of set up packet and submission of IRP
	for sending SET REPORT request.

  Remarks:
    This is local function and should not be called directly by the application.
*/
USB_ERROR _USB_HOST_HID_sendReport( USB_HOST_HID_INSTANCE * hidInstanceInfo)
{
    /* Start of local variable */
    USB_ERROR status;
    /* End of local variable */

    /* Fill set up packet */
    hidInstanceInfo->setupPacket.bmRequestType  = ( USB_SETUP_TYPE_CLASS |
                                                  USB_SETUP_RECIPIENT_INTERFACE );
    hidInstanceInfo->setupPacket.bRequest =  USB_HID_REQUESTS_SET_REPORT ;
    hidInstanceInfo->setupPacket.wValue = USB_HID_REPORT_TYPE_OUTPUT << 8;
    hidInstanceInfo->setupPacket.wIndex = hidInstanceInfo->interfaceNumber;
    hidInstanceInfo->setupPacket.wLength = 8;/* 8 byte report */

    /* Fill IRP */
    hidInstanceInfo->controlIRP.data = (void *)&(hidInstanceInfo->lastNonmodifier);
    hidInstanceInfo->controlIRP.flags = 0x00;
    hidInstanceInfo->controlIRP.size = 8;
    hidInstanceInfo->controlIRP.setup = &( hidInstanceInfo->setupPacket ) ;
    /* keep track of setup request */
    hidInstanceInfo->setupRequest = USB_HID_REQUESTS_SET_REPORT ;
    /* submit IRP */
    status = _USB_HOST_IRPSubmit ( hidInstanceInfo->controlPipeHandle,
                                &(hidInstanceInfo->controlIRP));
    if ( status != USB_ERROR_NONE)
        return USB_HOST_HID_ERROR;
    /* success */
    return USB_HOST_HID_RESULT_OK;
}

/***************  End of  File ************************************/
