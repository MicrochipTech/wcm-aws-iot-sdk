/**************************************************************************
 USB Device Layer Implementation

  Company:
    Microchip Technology Inc.
    
  File Name:
    usb_device.c
    
  Summary:
    This file contains implementations of both private and public functions
    of the USB Device Layer.
    
  Description:
    This file contains the USB device layer implementation.
**************************************************************************/

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

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_config.h"
#include "system/common/sys_module.h"
#include "usb/usb_common.h"
#include "usb/usb_chapter_9.h"
#include "usb/usb_device.h"
#include "system/debug/sys_debug.h"
#include "usb/src/usb_device_local.h"

/**********************************
 * Device layer instance objects.
 *********************************/
static USB_DEVICE_OBJ usbDeviceInstance[USB_DEVICE_INSTANCES_NUMBER];

/*************************************
 * Device layer endpoint constants. 
 *************************************/
static const USB_ENDPOINT controlEndpointTx = 0x80;
static const USB_ENDPOINT controlEndpointRx  = 0x00;

// *****************************************************************************
// *****************************************************************************
// Section: USB Device Layer System Interface functions.
// *****************************************************************************
// *****************************************************************************

// ******************************************************************************
/* Function:
    SYS_MODULE_OBJ USB_DEVICE_Initialize
    (
        const SYS_MODULE_INDEX index, 
        const SYS_MODULE_INIT * const initData
    )

  Summary:
    Initializes the required USB device layer state machines.

  Description:
    This function initializes the required state machines of the USB device
    layer.

  Remarks:
    Refer to usb_device.h for usage information.
*/

SYS_MODULE_OBJ USB_DEVICE_Initialize
(
    const SYS_MODULE_INDEX index, 
    const SYS_MODULE_INIT * const initData
)
{
    USB_DEVICE_INIT *deviceInit;
    USB_DEVICE_OBJ* usbDeviceThisInstance;
    DRV_USB_INIT drvUsbInit;
    USB_DEVICE_IRP * irpEp0Rx;
    USB_DEVICE_IRP * irpEp0Tx;
    uint8_t count;
    USB_DEVICE_FUNCTION_DRIVER * driver;
    USB_DEVICE_FUNCTION_REGISTRATION_TABLE * funcRegTable;
         
    /* Copy init data to local variable. */
    deviceInit = (USB_DEVICE_INIT *)initData;
    
    /* Make sure the index is with in range. */
    if(( index < 0 ) || ( index >= USB_DEVICE_INSTANCES_NUMBER ))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid index value");
        return (SYS_MODULE_OBJ_INVALID);
    }
    
    /* Make sure that initData is not NULL. */
    if(deviceInit == NULL)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Initialization data is NULL");
        return (SYS_MODULE_OBJ_INVALID);
    }
    
    /* Make sure that the USB Device Master Descriptor is valid */
    if(deviceInit->usbMasterDescriptor == NULL)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: USB Master Descriptor table is NULL");
        return (SYS_MODULE_OBJ_INVALID);
    }
    
    /* Get this instance of USB device layer. */
    usbDeviceThisInstance = &usbDeviceInstance[index];

    /* Initialize this instance */        
    usbDeviceThisInstance->usbDeviceInstanceState = SYS_STATUS_BUSY;

    /* Save the "self" index for future use */    
    usbDeviceThisInstance->usbDevLayerIndex = index;
    
    /* Set the device state to dettached.*/
    usbDeviceThisInstance->usbDeviceStatusStruct.usbDeviceState = USB_DEVICE_STATE_DETACHED;
    
    /* Initialize the instance structure. */
    usbDeviceThisInstance->ptrMasterDescTable           = deviceInit->usbMasterDescriptor;
    usbDeviceThisInstance->registeredFuncDriverCount    = deviceInit->registeredFuncCount;
    usbDeviceThisInstance->registeredFuncDrivers        = deviceInit->registeredFunctions;

    /* Initialize remote wakeup to disabled */
    usbDeviceThisInstance->remoteWakeupStatus = USB_DEVICE_REMOTE_WAKEUP_DISABLED;

    /* Initialize power source to bus power */
    usbDeviceThisInstance->usbDeviceStatusStruct.powerState = USB_DEVICE_POWER_STATE_BUS_POWERED;

    /* USBCD initialization */
    drvUsbInit.interruptSource  = deviceInit->interruptSource ;
    drvUsbInit.interruptSourceUSBDma = deviceInit->interruptSourceUSBDma;
    drvUsbInit.moduleInit       = deviceInit->moduleInit ;
    drvUsbInit.operationMode    = DRV_USB_OPMODE_DEVICE ;
    drvUsbInit.stopInIdle       = deviceInit->stopInIdle ;
    drvUsbInit.suspendInSleep   = deviceInit->suspendInSleep ;
    drvUsbInit.usbID            = deviceInit->usbID ;
    drvUsbInit.operationSpeed   = deviceInit->deviceSpeed ;
    drvUsbInit.endpointTable    = deviceInit->endpointTable;


    /* Initialize the USBCD */
    usbDeviceThisInstance->usbCDSystemModuleObject = 
        DRV_USB_Initialize(index, (SYS_MODULE_INIT *)&drvUsbInit);

    /* Check if the initialization was successful */
    if(usbDeviceThisInstance->usbCDSystemModuleObject == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Could not initialize USBCD");
        return (SYS_MODULE_OBJ_INVALID);
    }
    
    /* Reset set address flag.*/
    usbDeviceThisInstance->usbDeviceStatusStruct.setAddressPending = false;
 
    /* Initialize the RX IRP */
    irpEp0Rx            = &usbDeviceThisInstance->irpEp0Rx;
    irpEp0Rx->data      = usbDeviceThisInstance->ep0RxBuffer;
    irpEp0Rx->size      = USB_DEVICE_EP0_BUFFER_SIZE;
    irpEp0Rx->flags     = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;
    irpEp0Rx->status    = USB_DEVICE_IRP_STATUS_COMPLETED;
    irpEp0Rx->callback  = &_USB_DEVICE_Ep0ReceiveCompleteCallback;
    irpEp0Rx->userData  = (uintptr_t)usbDeviceThisInstance;

    /* Initialize the TX IRP */
    irpEp0Tx            = &usbDeviceThisInstance->irpEp0Tx;
    irpEp0Tx->callback  = &_USB_DEVICE_Ep0TransmitCompleteCallback;
    irpEp0Tx->userData  = (uintptr_t)usbDeviceThisInstance;
    irpEp0Tx->flags     = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;

    /* Device is not suspended */
    usbDeviceThisInstance->usbDeviceStatusStruct.isSuspended = false;

    /* Set the task state to opening the USB driver */
    usbDeviceThisInstance->taskState = USB_DEVICE_TASK_STATE_OPENING_USBCD;

    /* Initialize Endpoint Q size */ 
    _USB_DEVICE_Initialize_Endpoint_Q(index, deviceInit->queueSizeEndpointRead,
                                      deviceInit->queueSizeEndpointWrite);

    /* Create Mutex for Endpoint Read Write */
    _USB_DEVICE_EndpointMutexCreate (usbDeviceThisInstance);

    funcRegTable    = usbDeviceThisInstance->registeredFuncDrivers;

    for(count = 0; count < usbDeviceThisInstance->registeredFuncDriverCount; count++ )
    {
        /* Get a pointer to the driver function pointer table and
           call the Global Initialize routine of the driver. */
	driver = (USB_DEVICE_FUNCTION_DRIVER *)funcRegTable->driver;

	if (driver != NULL)
        {
            if(driver->globalInitialize != NULL)
            {
                driver->globalInitialize();
            }
        }
	funcRegTable++;
    }

    /* Return the index as the system module object */
    return index;  
}    

// *****************************************************************************
/* Function:
    SYS_STATUS USB_DEVICE_Status ( SYS_MODULE_OBJ object )

  Summary:
    Provides the current status of the USB device layer.

  Description:
    This function provides the current status of the USB device layer.

  Precondition:
    The USB_DEVICE_Initialize function must have been called before calling this
    function.

  Remarks
    Refer to usb_device.h for usage information.
*/

SYS_STATUS USB_DEVICE_Status( SYS_MODULE_OBJ object )
{
    SYS_MODULE_INDEX index = (SYS_MODULE_INDEX)object;

    if(object == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid System Module Object");
        return(SYS_STATUS_UNINITIALIZED);
    }

    return( usbDeviceInstance[index].usbDeviceInstanceState );
}

// *****************************************************************************
/* Function:
    bool USB_DEVICE_IsSuspended( USB_DEVICE_HANDLE usbDeviceHandle )

  Summary:
    Returns true if the device is in a suspended state.

  Description:
    This function returns true is the device is presently in suspended state.

  Precondition:
    The USB Device Layer must have been initialized and opened before calling this
    function.

  Parameters:
    usbDeviceHandle    - Pointer to the Device Layer Handle that is returned from
                         USB_DEVICE_Open

  Returns:
    Returns true if the device is suspended. 

  Remarks:
    Refer to usb_device.h for usage information.
*/
bool USB_DEVICE_IsSuspended( USB_DEVICE_HANDLE usbDeviceHandle )
{
    USB_DEVICE_OBJ* usbClientHandle;

    /* Validate the handle */
    usbClientHandle = _USB_DEVICE_ClientHandleValidate(usbDeviceHandle );
    if(usbClientHandle == NULL)
    {
       /* Handle is not valid */
       SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid handle");
    }
    return (usbClientHandle->usbDeviceStatusStruct.isSuspended); 
}

// *****************************************************************************
/* Function:
    USB_DEVICE_STATE USB_DEVICE_StateGet( USB_DEVICE_HANDLE usbDeviceHandle )

  Summary:
    Returns the current state of the USB device.

  Description:
    This function returns the current state of the USB device, as described in
    Chapter 9 of the USB 2.0 Specification.

  Precondition:
    The USB device layer must have been initialized and opened before calling
    this function.

  Parameters:
    usbDeviceHandle    - Pointer to the device layer handle that is returned from
                         USB_DEVICE_Open

  Returns:
    USB_DEVICE_STATE_DETACHED        - Device is not in any of the known states

    USB_DEVICE_STATE_ATTACHED        - Device is attached to the USB,
                                       but is not powered

    USB_DEVICE_STATE_POWERED         - Device is attached to the USB and powered,
                                       but has not been reset

    USB_DEVICE_STATE_DEFAULT         - Device is attached to the USB and powered
                                       and has been reset, but has not been assigned
                                       a unique address

    USB_DEVICE_STATE_ADDRESS         - Device is attached to the USB, powered,
                                       has been reset, and a unique device address
                                       has been assigned

    USB_DEVICE_STATE_CONFIGURED      - Device is attached to the USB, powered,
                                       has been reset, has a unique address,
                                       is configured, and is not suspended

  Remarks:
    Refer to usb_device.h for usage information.
*/
USB_DEVICE_STATE USB_DEVICE_StateGet( USB_DEVICE_HANDLE usbDeviceHandle )
{
    USB_DEVICE_OBJ* usbClientHandle;

    /* Validate the handle */
    usbClientHandle = _USB_DEVICE_ClientHandleValidate(usbDeviceHandle );
    if(usbClientHandle == NULL)
    {
       /* Handle is not valid */
       SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid handle");
    }
    return (usbClientHandle->usbDeviceStatusStruct.usbDeviceState); 
}

// *****************************************************************************
/* Function:
    void USB_DEVICE_Deinitialize ( SYS_MODULE_OBJ usbDeviceobj)

  Summary:
    De initializes the specified instance of the USB device layer.

  Description:
    This function de initializes the specified instance of the USB device layer, 
    disabling its operation (and any hardware) and invalidates all of the internal data.

  Parameters:
    usbDeviceObj    - USB Device Layer object handle, returned by USB_DEVICE_Initialize

  Returns:
    None.
  
*/

void USB_DEVICE_Deinitialize ( SYS_MODULE_OBJ usbDeviceObj )
{
    USB_DEVICE_OBJ * usbDeviceThisInstance;
    SYS_MODULE_INDEX index = (SYS_MODULE_INDEX)usbDeviceObj;

    /* Check if we have a valid system module object */
    if(usbDeviceObj == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid System Module Object");
        return;
    }

    usbDeviceThisInstance = ( USB_DEVICE_OBJ *)&usbDeviceInstance[index];

    /* Invalidate the object */
    usbDeviceThisInstance->usbDeviceInstanceState =  SYS_STATUS_UNINITIALIZED;
}
 
// *****************************************************************************
/* Function:
    void USB_DEVICE_Tasks( SYS_MODULE_OBJ devLayerObj )

  Summary:
    Calls all USB device layer tasks.

  Description:
    This function internally calls all USB Device layer tasks.

  Remarks:
    Refer to usb_device.h for usage information.
*/

void USB_DEVICE_Tasks( SYS_MODULE_OBJ devLayerObj )
{
    uint8_t count;
    USB_SPEED speed;
    uint16_t configValue;
    uint16_t maxFunctionCounts;
    USB_DEVICE_OBJ * usbDeviceThisInstance;
    USB_DEVICE_FUNCTION_REGISTRATION_TABLE * funcRegTable;
    USB_DEVICE_FUNCTION_DRIVER * driver;

    /* Assert object is valid. */
    if(devLayerObj == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: System Module Object is invalid");
        return;
    }
      
    /* Get this instance of USB device layer.*/
    usbDeviceThisInstance = &usbDeviceInstance[devLayerObj];    

    /* Proceed only if this instance is in initialized state. */
    if( usbDeviceThisInstance->usbDeviceInstanceState <= SYS_STATUS_UNINITIALIZED )
    {
        /* Instance is not yet initialized. Just do a return. */
        return;
    }

    /* Update the USBCD Tasks routine */
    DRV_USB_Tasks(usbDeviceThisInstance->usbCDSystemModuleObject);

    /* Get device layer data. */
    speed           = usbDeviceThisInstance->usbDeviceStatusStruct.usbSpeed ;
    configValue     = usbDeviceThisInstance->activeConfiguration ;
    funcRegTable    = usbDeviceThisInstance->registeredFuncDrivers;
    maxFunctionCounts = usbDeviceThisInstance->registeredFuncDriverCount;

    /* Run the device layer task routine */
    switch(usbDeviceThisInstance->taskState)
    {
        case USB_DEVICE_TASK_STATE_OPENING_USBCD:

            /* Try to open the driver handle. This could fail if the driver is
             * not ready to be opened. */
            usbDeviceThisInstance->usbCDHandle = DRV_USB_Open( devLayerObj, 
                    DRV_IO_INTENT_EXCLUSIVE|DRV_IO_INTENT_NONBLOCKING|DRV_IO_INTENT_READWRITE);

            /* Check if the driver was opened */
            if(usbDeviceThisInstance->usbCDHandle != DRV_HANDLE_INVALID)
            {
                /* Yes the driver could be opened. */

                /* Advance the state to the next state */
                usbDeviceThisInstance->taskState = USB_DEVICE_TASK_STATE_RUNNING;

                /* Update the USB Device Layer state to indicate that it can be
                 * opened */
                usbDeviceThisInstance->usbDeviceInstanceState = SYS_STATUS_READY;
            }

            break;

        case USB_DEVICE_TASK_STATE_RUNNING:

            /* In this state, the device layer performs it's actual task, that
             * is calling the tasks routines of the functions drivers */

            for(count = 0; count < maxFunctionCounts; count++ )
            {
                if( (funcRegTable->speed & speed) && (funcRegTable->configurationValue == configValue))
                {
                    if((usbDeviceThisInstance->usbDeviceStatusStruct.usbDeviceState == USB_DEVICE_STATE_CONFIGURED) &&
                            (usbDeviceThisInstance->usbDeviceStatusStruct.isSuspended == false))
                    {
                        /* Get a pointer to the driver function pointer table and
                           call the task routine of the driver. */
                        driver = (USB_DEVICE_FUNCTION_DRIVER *)funcRegTable->driver;

                        if (driver != NULL)
                        {
                            if(driver->tasks != NULL)
                            {
                                driver->tasks( funcRegTable->funcDriverIndex );
                            }
                        }
                     }
                }
                funcRegTable++;
            }
            break;
        default:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: USB Device Layer Client Interface Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    USB_DEVICE_HANDLE USB_DEVICE_Open
    (
        const SYS_MODULE_INDEX index,
        const DRV_IO_INTENT intent
    )

  Summary:
    Opens the specific module instance and returns a handle.

  Description:
    This function opens the USB device layer for use by any client module and
    provides a handle that must be provided to any of the other device layer
    operations to identify the caller and the instance of the driver/hardware
    module.

  Remarks
    Refer to usb_device.h for usage information.
*/

USB_DEVICE_HANDLE USB_DEVICE_Open
(
    const SYS_MODULE_INDEX index, 
    const DRV_IO_INTENT intent
)
{
    USB_DEVICE_OBJ * usbDeviceThisClient;

    /* Make sure the index is with in range. */
    if((index < 0) || (index >= USB_DEVICE_INSTANCES_NUMBER))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Index is invalid");
        return(USB_DEVICE_HANDLE_INVALID);
    }

    /* Check if the instance is initialized. */   
    if(usbDeviceInstance[index].usbDeviceInstanceState != SYS_STATUS_READY)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Device Layer is not ready to be opened");
        return(USB_DEVICE_HANDLE_INVALID);
    }

    /* Copy this local client */
    usbDeviceThisClient = &usbDeviceInstance[index];

    if(!usbDeviceThisClient->inUse)
    {
        /* We found a free client object */
        usbDeviceThisClient->inUse = true;

        /* update client status */
        usbDeviceThisClient->clientState = USB_DEVICE_CLIENT_STATUS_READY;

        /* Get the Client Handle */
        return((USB_DEVICE_HANDLE)usbDeviceThisClient);
    }
      
    /* If we have reached here, then we did not find a free client object */
    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: No free client objects");

    return(USB_DEVICE_HANDLE_INVALID);   
}    

// *****************************************************************************
/* Function:
    void USB_DEVICE_Close( DRV_HANDLE usbDevHandle )

  Summary:
    Closes an opened instance of the USB device layer.

  Description:
    This function closes an opened instance of the USB device layer,
    invalidating the handle.

  Remarks:
    Refer to usb_device.h for usage information.
*/

void USB_DEVICE_Close(USB_DEVICE_HANDLE hClient )
{
    USB_DEVICE_OBJ* usbClientHandle;
    
    /* Validate the handle */
    
    usbClientHandle = _USB_DEVICE_ClientHandleValidate(hClient);
    if(usbClientHandle == NULL)
    {
       /* Handle is not valid */
       SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid handle");
       return; 
    }
    
    /* Close this client */
    usbClientHandle->clientState = DRV_CLIENT_STATUS_CLOSED;
    usbClientHandle->inUse = false;
}    

// *****************************************************************************
/* Function:
    USB_DEVICE_CLIENT_STATUS USB_DEVICE_ClientStatusGet
    (
        USB_DEVICE_HANDLE usbDevHandle
    );

  Summary:
    Gets the current client-specific status of the USB device layer.

  Description:
    This function gets the client-specific status of the USB device layer associated
    with the specified handle.

  Returns:
    Refer to usb_device.h for usage information.
*/

USB_DEVICE_CLIENT_STATUS USB_DEVICE_ClientStatusGet( USB_DEVICE_HANDLE hHandle )
{
    USB_DEVICE_OBJ * devClientHandle;

    devClientHandle = _USB_DEVICE_ClientHandleValidate(hHandle);

    if(devClientHandle == NULL)
    {
        return (USB_DEVICE_CLIENT_STATUS_CLOSED);
    }
    
    /* Return the state of the client. */  
    return( devClientHandle->clientState ); 
}

// *****************************************************************************
/* Function:
    void USB_DEVICE_Tasks_ISR (SYS_MODULE_OBJ devLayerObj);

  Summary:
    USB Device Layer Tasks routine to be called in the USB Interrupt Service
    Routine.

  Description:
    This function must be called in the USB Interrupt Service Routine if the
    Device Stack is configured for interrupt mode. In case the Device Stack is
    configured for polling mode, this function is automatically called from the
    USB_DEVICE_Tasks() function. devLayerObj must be the system module object
    associated with the USB module generating the interrupt.

  Remarks:
    Refer to usb_device.h for usage information.
*/

void USB_DEVICE_Tasks_ISR(SYS_MODULE_OBJ devLayerObj)
{
    USB_DEVICE_OBJ * usbDeviceThisInstance;

    /* Assert object is valid. */
    if(devLayerObj == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: System Module Object is invalid");
        return;
    }

    /* Get this instance of USB device layer.*/
    usbDeviceThisInstance = &usbDeviceInstance[devLayerObj];

    /* Proceed only if this instance is in initialized state. */
    if( usbDeviceThisInstance->usbDeviceInstanceState <= SYS_STATUS_UNINITIALIZED )
    {
        /* Instance is not yet initialized. Just do a return. */
        return;
    }

    DRV_USB_Tasks_ISR(usbDeviceThisInstance->usbCDSystemModuleObject);

}

void USB_DEVICE_Tasks_ISR_USBDMA(SYS_MODULE_OBJ devLayerObj)
{
    USB_DEVICE_OBJ * usbDeviceThisInstance;

    /* Assert object is valid. */
    if(devLayerObj == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: System Module Object is invalid");
        return;
    }

    /* Get this instance of USB device layer.*/
    usbDeviceThisInstance = &usbDeviceInstance[devLayerObj];

    /* Proceed only if this instance is in initialized state. */
    if( usbDeviceThisInstance->usbDeviceInstanceState <= SYS_STATUS_UNINITIALIZED )
    {
        /* Instance is not yet initialized. Just do a return. */
        return;
    }

    DRV_USB_Tasks_ISR_USBDMA(usbDeviceThisInstance->usbCDSystemModuleObject);

}

// ******************************************************************************
/* Function:
    void USB_DEVICE_EventCallBackSet
    (
        USB_DEVICE_HANDLE hHandle,
        const USB_DEVICE_EVENT_HANDLER callBackFunc
        uintptr_t context
    );

  Summary:
    Sets up the callback function that will be called in case of an
    event from the USB device layer.

  Description:
    This function sets up the callback function. This callback function 
    will be called when an event occurs in the USB device layer.

  Remarks:
    Refer to usb_device.h for usage information.
*/

void USB_DEVICE_EventHandlerSet
(
    USB_DEVICE_HANDLE hHandle, 
    const USB_DEVICE_EVENT_HANDLER callBackFunc,
    uintptr_t context
)
{
    USB_DEVICE_OBJ* devClientHandle;

    /* Validate the handle */

    devClientHandle = _USB_DEVICE_ClientHandleValidate(hHandle);
    if(devClientHandle == NULL)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid client handle");
        return;
    }

    /* Register the callback function. Note that the call back function is
     * invoked for control transfer events.  */
    devClientHandle->callBackFunc = callBackFunc;
    devClientHandle->context = context;

    /* Register a callback with the driver. */
    DRV_USB_ClientEventCallBackSet(devClientHandle->usbCDHandle,
                   (uintptr_t)devClientHandle, &_USB_DEVICE_EventHandler);
}   

// ******************************************************************************
/* Function:
   uint8_t USB_DEVICE_ActiveConfigurationGet( USB_DEVICE_HANDLE usbDeviceHandle )

  Summary:
    Informs the client of the current USB device configuration set by the USB
    host.

  Description:
    This function returns the current active USB device configuration.

  Precondition:
    The USB Device Layer must have been initialized and opened before calling this
    function.

  Remarks:  
    See usb_device.h for usage information.
*/

uint8_t USB_DEVICE_ActiveConfigurationGet(USB_DEVICE_HANDLE hHandle)
{
    USB_DEVICE_OBJ * devClientHandle;

    /* Validate the client handle */

    devClientHandle = _USB_DEVICE_ClientHandleValidate(hHandle);
    if(devClientHandle == NULL)
    {
        /* Client handle is invalid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid client handle");
        return(0);
    }
    
    return(devClientHandle->activeConfiguration);
}

// *****************************************************************************
/* Function:
    USB_SPEED USB_DEVICE_ActiveSpeedGet(USB_DEVICE_HANDLE usbDeviceHandle)

  Summary:
    Informs the client of the current operation speed of the USB bus.

  Description:
    The USB device stack supports both high speed and full speed operations.
    This function returns the current operation speed of the USB bus. This
    function should be called after the USB_DEVICE_EVENT_RESET event has
    occurred.

  Precondition:
    The USB device layer must have been initialized and a valid handle
    to USB device layer must have been opened.
  
  Remarks:  
    See usb_device.h for usage information.
*/

USB_SPEED USB_DEVICE_ActiveSpeedGet(USB_DEVICE_HANDLE hHandle)
{
    USB_DEVICE_OBJ* devClientHandle;
    
    /* Validate the handle */
    devClientHandle = _USB_DEVICE_ClientHandleValidate(hHandle);
    if(devClientHandle == NULL)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid Handle");
        return(USB_SPEED_LOW);
    }

    /* Return the current speed */
    return(devClientHandle->usbDeviceStatusStruct.usbSpeed);
}

// *****************************************************************************
/* Function:
    USB_DEVICE_CONTROL_TRANSFER_RESULT USB_DEVICE_ControlSend
    (
        USB_DEVICE_HANDLE usbDeviceHandle,
        uint8_t * data, 
        size_t length 
    )

  Summary:
    Sends data stage of the control transfer from device to host.

  Description:
    Sends data stage of the control transfer from device to host.

  Remarks:
    Refer to usb_device.h for usage information.
*/

USB_DEVICE_CONTROL_TRANSFER_RESULT USB_DEVICE_ControlSend
(
    USB_DEVICE_HANDLE hClient,
    void * data, 
    size_t length 
)
{
    USB_DEVICE_OBJ * usbDeviceThisInstance;
    USB_DEVICE_IRP * irpHandle;   

    usbDeviceThisInstance = _USB_DEVICE_ClientHandleValidate(hClient);

    /* Validate the client handle */
    if(usbDeviceThisInstance == NULL)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid Client Handle");
        return(USB_DEVICE_CONTROL_TRANSFER_RESULT_FAILED);
    }


    /* Get a handle to the TX IRP */
    irpHandle = &usbDeviceThisInstance->irpEp0Tx;     
    irpHandle->data = data;
    irpHandle->size = (unsigned int )length;

    if(length == usbDeviceThisInstance->controlTransferDataStageSize)
    {
        /* This means we are sending the amount of data that is required.
         * So no need to send send ZLP */
        irpHandle->flags = USB_DEVICE_IRP_FLAG_DATA_PENDING;
    }
    else if (length < usbDeviceThisInstance->controlTransferDataStageSize )
    {
        /* The length is less than requested. We let the driver manage the
         * ZLP. */
        irpHandle->flags = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;
    }
    /* Submit the IRP to the USBCD */
    (void)DRV_USB_DEVICE_IRPSubmit( usbDeviceThisInstance->usbCDHandle, controlEndpointTx, irpHandle);

    return USB_DEVICE_CONTROL_TRANSFER_RESULT_SUCCESS;
}

// *****************************************************************************
/* Function:
    USB_DEVICE_CONTROL_TRANSFER_RESULT USB_DEVICE_ControlReceive
    (
        USB_DEVICE_HANDLE usbDeviceHandle,
        void * data, 
        size_t length
    )

  Summary:
    Receives data stage of the control transfer from host to device.

  Description:
    Receives data stage of the control transfer from host to device.

  Remarks:
    Refer to usb_device.h for usage information.
*/

USB_DEVICE_CONTROL_TRANSFER_RESULT USB_DEVICE_ControlReceive
(
    USB_DEVICE_HANDLE handle,
    void * data, 
    size_t length
)
{
    USB_DEVICE_CONTROL_TRANSFER_STRUCT * pControlTransfer;
    USB_DEVICE_OBJ * client;

    /* Validate the client handle */
    client = _USB_DEVICE_ClientHandleValidate(handle);
    if(client == NULL)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid Client Handle");
        return(USB_DEVICE_CONTROL_TRANSFER_RESULT_FAILED);
    }

    /* Get the control transfer object */
    pControlTransfer = &client->controlTransfer;
    pControlTransfer->rxBuffer = data ;
    pControlTransfer->rxDataCount = 0;
    pControlTransfer->expectedRxDataCount = length ;

    /* We are always ready to receive data on endpoint 0. So no need to submit a
     * IRP */
    return USB_DEVICE_CONTROL_TRANSFER_RESULT_SUCCESS;
}

// *****************************************************************************
/* Function:
    USB_DEVICE_CONTROL_TRANSFER_RESULT USB_DEVICE_ControlStatus
    (
        USB_DEVICE_HANDLE hClient,
        USB_DEVICE_CONTROL_STATUS status 
    )

  Summary:
    Initiates status stage of the control transfer.

  Description:
    Initiates status stage of the control transfer.

  Remarks:
    Refer to usb_device.h for usage information.
*/

USB_DEVICE_CONTROL_TRANSFER_RESULT USB_DEVICE_ControlStatus
(
    USB_DEVICE_HANDLE handle,
    USB_DEVICE_CONTROL_STATUS status
)
{
    USB_DEVICE_OBJ * usbDeviceThisInstance;
    USB_DEVICE_IRP * irpHandle;

    usbDeviceThisInstance = _USB_DEVICE_ClientHandleValidate(handle);
    if(usbDeviceThisInstance == NULL)
    {
        /* Client handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid Client Handle");
        return(USB_DEVICE_CONTROL_TRANSFER_RESULT_FAILED);
    }

    usbDeviceThisInstance->controlTransfer.inProgress = false;

    if(USB_DEVICE_CONTROL_STATUS_ERROR == status)
    {
        /* This means the control transfer should be stalled. We stall endpoint
         * 0 */
        DRV_USB_DEVICE_EndpointStall(usbDeviceThisInstance->usbCDHandle , controlEndpointTx);        
    }
    else
    {
        /* We must acknowledge the control transfer. This is done by sending a
         * ZLP */
        irpHandle = &usbDeviceThisInstance->irpEp0Tx;
        irpHandle->data = NULL;
        irpHandle->size = 0;

        (void)DRV_USB_DEVICE_IRPSubmit( usbDeviceThisInstance->usbCDHandle, controlEndpointTx, irpHandle);
    }

    return USB_DEVICE_CONTROL_TRANSFER_RESULT_SUCCESS;
}
// *****************************************************************************
/* Function:
    void USB_DEVICE_PowerStateSet
    (
        USB_DEVICE_HANDLE handle,
        USB_DEVICE_POWER_STATE state
    )

  Summary:
    Sets the power state of the device.

  Description:
    Sets the power state of the device.
    
  Remarks:
    Refer to usb_device.h for usage information.
*/

void USB_DEVICE_PowerStateSet
(
    USB_DEVICE_HANDLE handle,
    USB_DEVICE_POWER_STATE state
)
{
    USB_DEVICE_OBJ* client;

    /* Validate the client handle */

    client = _USB_DEVICE_ClientHandleValidate(handle);
    if(client == NULL)
    {
        /* Client handle is invalid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid Handle");
        return;
    }

    /* Store the power state */
    client->usbDeviceStatusStruct.powerState = state;
}


// ******************************************************************************
/* Function:
    USB_DEVICE_REMOTE_WAKEUP_STATUS USB_DEVICE_RemoteWakeupStatusGet
    (
        USB_DEVICE_HANDLE handle
    )

  Summary:
    This function returns the status of remote wakeup capability of the device.

  Description:
    This function returns the status of remote wakeup capability of the device.

  Remarks:
    Refer to usb_device.h for usage information.
*/

USB_DEVICE_REMOTE_WAKEUP_STATUS USB_DEVICE_RemoteWakeupStatusGet
(
    USB_DEVICE_HANDLE handle
)
{
    USB_DEVICE_OBJ* client;

    /* Validate the client handle */

    client = _USB_DEVICE_ClientHandleValidate(handle);
    if(client == NULL)
    {
        /* Client handle is invalid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid Handle");
        return USB_DEVICE_REMOTE_WAKEUP_DISABLED;
    }

    return (client->remoteWakeupStatus);
}
// *****************************************************************************
/* Function:
    void USB_DEVICE_RemoteWakeupStop ( USB_DEVICE_HANDLE usbDeviceHandle )

  Summary:
    This function will stop the resume signalling.

  Description:
    This function will stop the resume signalling. This function should be called
    after the client has called the USB_DEVICE_RemoteWakeupStart() function.

  Remarks:
    Refer to usb_device.h for usage information.
*/

void USB_DEVICE_RemoteWakeupStop( USB_DEVICE_HANDLE handle )
{
    USB_DEVICE_OBJ* client;

    /* Validate the device handle */
    client = _USB_DEVICE_ClientHandleValidate(handle);
    if(NULL == client)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer Client Handle is invalid");
        return;
    }

    /* Call the driver remote wake up function here */
    DRV_USB_DEVICE_RemoteWakeupStop(client->usbCDHandle);
}


// *****************************************************************************
/* Function:
    void USB_DEVICE_RemoteWakeupStart( USB_DEVICE_HANDLE usbDeviceHandle )

  Summary:
    This function will start the resume signalling.

  Description:
    This function will start the resume signalling on the bus. The client calls
    this function after it has detected a idle bus (through the
    USB_DEVICE_EVENT_SUSPENDED event). The remote wakeup feature should have
    been enabled by the host, before the client can call this function. The
    client can use the USB_DEVICE_RemoteWakeupStatusGet() function to check if
    the host has enabled the remote wakeup feature.

  Remarks:
    Refer to usb_device.h for usage information.
*/

void USB_DEVICE_RemoteWakeupStart( USB_DEVICE_HANDLE usbDeviceHandle )
{
    USB_DEVICE_OBJ * client;

    /* Validate the device handle */
    client = _USB_DEVICE_ClientHandleValidate(usbDeviceHandle);
    if(NULL == client)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer Client Handle is invalid");
        return;
    }

    /* Call the driver remote wake up function here */
    DRV_USB_DEVICE_RemoteWakeupStart(client->usbCDHandle);
}

// *****************************************************************************
// *****************************************************************************
// Section: USB Device Layer Local Functions
// *****************************************************************************
// *****************************************************************************

// ******************************************************************************
/* Function:
    void _USB_DEVICE_Ep0ReceiveCompleteCallback( USB_DEVICE_IRP * handle )

  Summary:
    Control receive complete callback.

  Description:
    This callback is called when either data/SETUP packet is received
    on the control endpoint.

  Remarks:
    This is local function. It should not be used directly by the client.
*/

void _USB_DEVICE_Ep0ReceiveCompleteCallback( USB_DEVICE_IRP * handle )
{
    USB_DEVICE_IRP * irpHandle = (USB_DEVICE_IRP *)handle;
    USB_DEVICE_OBJ * usbDeviceThisInstance;  
    USB_DEVICE_EVENT controlEvent = 0;
    USB_DEVICE_CONTROL_TRANSFER_STRUCT * controlTransfer;
    void * eventData;

    /* If the IRP was aborted, there is nothing for us to do */
    if(irpHandle->status == USB_DEVICE_IRP_STATUS_ABORTED)
    {
        return;
    }

    usbDeviceThisInstance = (USB_DEVICE_OBJ *)irpHandle->userData;
    controlTransfer = &usbDeviceThisInstance->controlTransfer;

    /* Something is received on EP0. */

    if(irpHandle->status == USB_DEVICE_IRP_STATUS_SETUP)
    {
        /* If we have received a SETUP packet, then abort any on-going control
         * transfer. Reset the control transfer handler back to the device layer
         * */

        if(controlTransfer->inProgress )
        {
            /* Abort any previous transfer */
            if ( controlTransfer->handler != NULL)
            {
                if (controlTransfer->handler == (void*)usbDeviceThisInstance->callBackFunc)
                {
                    /* If the Control transfer handler is Application callback for USB Device layer,
                       then invoke the application callback function.  */
                    usbDeviceThisInstance->callBackFunc(USB_DEVICE_EVENT_CONTROL_TRANSFER_ABORTED, NULL, usbDeviceThisInstance->context ); 
                }
                else
                {
                    controlTransfer->handler(controlTransfer->handlerIndex,
                    USB_DEVICE_EVENT_CONTROL_TRANSFER_ABORTED , NULL );
                }
            }
            else
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Control transfer Handler is NULL" );
            }
        }

        /* Mark the control transfer as "in progress". */
        controlTransfer->inProgress = true;

        /* Now change the transfer handler back to USB device layer handler. We
         * need to send the SETUP packet event and the SETUP packet to the
         * device layer */
        controlTransfer->handler = &_USB_DEVICE_ControlTransferHandler;
        controlTransfer->handlerIndex = usbDeviceThisInstance->usbDevLayerIndex;
        controlEvent = USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST; 
        eventData = usbDeviceThisInstance->ep0RxBuffer;
    }
    else if( irpHandle->size == 0)
    {
        /* ZLP received. This marks the completion of control transfer. Inform
         * the handler. */
        controlTransfer->inProgress = false;
        controlEvent = USB_DEVICE_EVENT_CONTROL_TRANSFER_DATA_SENT;
        eventData = NULL;
    }
    else if( irpHandle->size &&  controlTransfer->inProgress )
    {
        /* We are in the data stage of the control transfer.  See if the control
         * transfer handler was expecting the data.       */

        if(controlTransfer->rxDataCount <  controlTransfer->expectedRxDataCount)
        {
            /* Copy the RX buffer to control transfer handler given driver
             * buffer and advance the buffer. Continue the transfer  */
            memcpy(&usbDeviceThisInstance->controlTransfer.rxBuffer[controlTransfer->rxDataCount],
                    usbDeviceThisInstance->ep0RxBuffer, irpHandle->size );

            controlTransfer->rxDataCount += irpHandle->size;
        }
        
        /* Now that we have received the data, check if the IRP is completed */
        if(controlTransfer->rxDataCount >=  controlTransfer->expectedRxDataCount)
        {
            /* We have received all the the data that we need. Send an event to
             * the upper layer */
            controlEvent = USB_DEVICE_EVENT_CONTROL_TRANSFER_DATA_RECEIVED;
            eventData = NULL;
        }
    }

    if(controlEvent)
    {
        if (controlTransfer->handler == (void*)usbDeviceThisInstance->callBackFunc)
        {
            /* If the Control transfer handler is Application callback for USB Device layer,
             * then invoke the application callback function.  */
            usbDeviceThisInstance->callBackFunc(controlEvent, eventData, usbDeviceThisInstance->context ); 
        }
        else
        {
            /* Propagate the event to the control transfer handler */
            controlTransfer->handler(controlTransfer->handlerIndex, controlEvent , eventData);
        }
    }

    usbDeviceThisInstance->irpEp0Rx.size = USB_DEVICE_EP0_BUFFER_SIZE;

    /* Submit IRP to endpoint 0 to receive the SETUP packet */
    (void)DRV_USB_DEVICE_IRPSubmit( usbDeviceThisInstance->usbCDHandle, controlEndpointRx , &usbDeviceThisInstance->irpEp0Rx);

}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_Ep0TransmitCompleteCallback( void * handle)

  Summary:
    EP0 transmit complete callback.

  Description:
    This function is called by the controller driver after the completing the
    EP0 transmit.

  Returns:
    This is local function. It should not be used directly by the client.
*/

void _USB_DEVICE_Ep0TransmitCompleteCallback(USB_DEVICE_IRP * handle)
{
    USB_DEVICE_IRP * irpHandle = (USB_DEVICE_IRP *)handle;
    USB_DEVICE_OBJ * usbDeviceThisInstance;
    USB_DEVICE_CONTROL_TRANSFER_STRUCT * controlTransfer;

    usbDeviceThisInstance = (USB_DEVICE_OBJ *)irpHandle->userData;
    controlTransfer = &(usbDeviceThisInstance->controlTransfer);
    
    if(irpHandle->status == USB_DEVICE_IRP_STATUS_ABORTED)
    {
        return;
    }

    if(usbDeviceThisInstance->usbDeviceStatusStruct.setAddressPending)
    {
        DRV_USB_DEVICE_AddressSet(usbDeviceThisInstance->usbCDHandle, usbDeviceThisInstance->deviceAddress);
        usbDeviceThisInstance->usbDeviceStatusStruct.setAddressPending = false;
    }

    if(irpHandle->status == USB_DEVICE_IRP_STATUS_COMPLETED)
    {
        if( irpHandle->size == 0 )
        {
            // Just completed transmitting ZLP.
            controlTransfer->inProgress = false;
        }
    }
}

// *****************************************************************************
/* Function:
    void _USB_DEVICE_DeInitializeAllFunctionDrivers
    (
        USB_DEVICE_OBJ * usbDeviceThisInstance
    )

  Summary:
    De-initializes all function drivers that are presently loaded for the
    selected configuration.

  Description:
    De initializes all function drivers that are presently loaded for the
    selected configuration.

  Remarks
    This function is a local function and should not be called directly by the
    client.
*/

void _USB_DEVICE_DeInitializeAllFunctionDrivers
(
    USB_DEVICE_OBJ * usbDeviceThisInstance
)
{
    uint8_t count = 0;
    USB_SPEED speed = usbDeviceThisInstance->usbDeviceStatusStruct.usbSpeed ;
    uint16_t configValue = usbDeviceThisInstance->activeConfiguration ;
    uint16_t maxFunctionCounts = usbDeviceThisInstance->registeredFuncDriverCount;
    USB_DEVICE_FUNCTION_REGISTRATION_TABLE * funcRegTable = usbDeviceThisInstance->registeredFuncDrivers;
    USB_DEVICE_FUNCTION_DRIVER * driver;

    for(count = 0; count < maxFunctionCounts; count++ )
    {
        if( (funcRegTable->speed & speed) && (funcRegTable->configurationValue == configValue))
        {
            /* De initialize the function driver */
            driver = (USB_DEVICE_FUNCTION_DRIVER *)funcRegTable->driver;
            if (driver != NULL)
            {
                if(driver->deInitialize != NULL)
                {
                    /* Call the function driver deInitialize routine */
                    driver->deInitialize( funcRegTable->funcDriverIndex );
                }
            }           
        }
        funcRegTable++;
    }
    
    /* Deallocate all mutexes */
    if (usbDeviceThisInstance->isMutexEndpointIrpInitialized == true)
    {
        /* Delete Mutex*/
        _USB_DEVICE_EndpointMutexDelete(usbDeviceThisInstance);
    }
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_EventHandler
    (
        uintptr_t referenceHandle,
        DRV_USB_EVENT eventType, 
        void * eventData
    )

  Summary:
    Handles the events originating from the USB Controller driver.

  Description:
    This function is registered into the USB Controller driver as a callback
    function. The USB Controller driver calls this function in case of events
    from the USB Controller driver.
    
  Remarks:
    This is a local function and should not be called directly by a client.
*/

void _USB_DEVICE_EventHandler
(
    uintptr_t referenceHandle,
    DRV_USB_EVENT eventType, 
    void * eventData
)
{
    USB_DEVICE_OBJ* usbDeviceThisInstance;
    USB_DEVICE_MASTER_DESCRIPTOR * ptrMasterDescTable;
    USB_DEVICE_EVENT_DATA_SOF SOFFrameNumber;

    usbDeviceThisInstance = (USB_DEVICE_OBJ *)referenceHandle;

    /* Handle events, only if this instance is in initialized state */
    if( usbDeviceThisInstance->usbDeviceInstanceState <= SYS_STATUS_UNINITIALIZED )
    {
        /* The device should anyway not be attached when the device layer is
         * not initialized. If we receive driver event when the device layer is
         * not initialized, there is nothing we can do but ignore them. */
        return;                
    }

    switch(eventType)
    {
        case DRV_USB_EVENT_RESET_DETECT:

            /* Clear the suspended state */
            usbDeviceThisInstance->usbDeviceStatusStruct.isSuspended = false;

            /* Cancel any IRP already submitted in the RX direction. */
            DRV_USB_DEVICE_IRPCancelAll( usbDeviceThisInstance->usbCDHandle,
                    controlEndpointRx );

            /* Cancel any IRP already submitted in the TX direction. */
            DRV_USB_DEVICE_IRPCancelAll( usbDeviceThisInstance->usbCDHandle,
                    controlEndpointTx );

            /* Deinitialize all function drivers.*/
            _USB_DEVICE_DeInitializeAllFunctionDrivers ( usbDeviceThisInstance );

            /* Disable all endpoints except for EP0.*/
            DRV_USB_DEVICE_EndpointDisableAll(usbDeviceThisInstance->usbCDHandle);

            /* Enable EP0 endpoint in RX direction */
            (void)DRV_USB_DEVICE_EndpointEnable( usbDeviceThisInstance->usbCDHandle,
                    controlEndpointTx,
                    USB_TRANSFER_TYPE_CONTROL,
                    USB_DEVICE_EP0_BUFFER_SIZE);

            if(usbDeviceThisInstance->irpEp0Rx.status <= USB_DEVICE_IRP_STATUS_SETUP)
            {
                /* Submit IRP to endpoint 0 to receive the setup packet */
                (void)DRV_USB_DEVICE_IRPSubmit( usbDeviceThisInstance->usbCDHandle,
                        controlEndpointRx ,
                        &usbDeviceThisInstance->irpEp0Rx);
            }

            /* Change device state to Default */
            usbDeviceThisInstance->usbDeviceStatusStruct.usbDeviceState = USB_DEVICE_STATE_DEFAULT;

            /* Reset means chirping has already happened. So, we must be knowing
               the speed. Get the speed and save it for future. */
            usbDeviceThisInstance->usbDeviceStatusStruct.usbSpeed = DRV_USB_DEVICE_CurrentSpeedGet(usbDeviceThisInstance->usbCDHandle);

            /* Get the master descriptor table entry.*/
            ptrMasterDescTable = usbDeviceThisInstance->ptrMasterDescTable;

            /* Now we know the speed. So for this speed get the pointer that
               points to correct group of configurations. */
            if( usbDeviceThisInstance->usbDeviceStatusStruct.usbSpeed == USB_SPEED_HIGH )
            {
                usbDeviceThisInstance->configDescriptorsPtr = ptrMasterDescTable->highSpeedConfigDescriptorTable;
                
                /* Also get the max configurations available in this group.*/
                usbDeviceThisInstance->maxConfigs = ptrMasterDescTable->highSpeedConfigDescriptorCount;
            }
            else
            {
                /* Classic speeds (full/low speed) */
                usbDeviceThisInstance->configDescriptorsPtr = ptrMasterDescTable->configDescriptorTable;

                /* Get the maximum configurations available in this group.*/
                usbDeviceThisInstance->maxConfigs = ptrMasterDescTable->configDescriptorCount;
            }

            /* Invalidate the current active configuration */
            usbDeviceThisInstance->activeConfiguration = 0;
            break;

        case DRV_USB_EVENT_RESUME_DETECT:
            /* USB device resumed. Update the flag.*/
            usbDeviceThisInstance->usbDeviceStatusStruct.isSuspended = false;
            break;

        case DRV_USB_EVENT_IDLE_DETECT:
            /* USB Device is suspended */
            usbDeviceThisInstance->usbDeviceStatusStruct.isSuspended = true;
            break;        

        case DRV_USB_EVENT_SOF_DETECT:
            _USB_DEVICE_Set_Sof_Event_type(eventType);
            if (eventType)
            {
                eventType = USB_DEVICE_EVENT_SOF;
                /* Get the frame number */
                SOFFrameNumber.frameNumber
                            = DRV_USB_DEVICE_SOFNumberGet(usbDeviceThisInstance->usbCDHandle);
                eventData = &SOFFrameNumber;
            }

            break;

         case DRV_USB_EVENT_DEVICE_SESSION_VALID:
            usbDeviceThisInstance->usbDeviceStatusStruct.usbDeviceState = USB_DEVICE_STATE_ATTACHED;
            break;

         case DRV_USB_EVENT_DEVICE_SESSION_INVALID:
             usbDeviceThisInstance->usbDeviceStatusStruct.usbDeviceState = USB_DEVICE_STATE_DETACHED;  
             break;
        default:
            // Nothing to do for all other cases.
            eventType = 0;
            break;
    }

    if(eventType)
    {
        /* Inform the client about the event */
        if((usbDeviceThisInstance->clientState == USB_DEVICE_CLIENT_STATUS_READY) &&
           (usbDeviceThisInstance->callBackFunc != NULL))
        {
            /* This means this client is valid and is a client of this device
               layer instance. Pass event to application */
            usbDeviceThisInstance->callBackFunc(eventType, eventData, usbDeviceThisInstance->context);
        }
    }
}    

// ******************************************************************************
/* Function:
    void _USB_DEVICE_ControlTransferHandler
    (
        SYS_MODULE_INDEX handlerIndex,
        USB_DEVICE_CONTROL_TRANSFER_EVENT transferEvent,
        USB_DEVICE_CONTROL_TRANSFER_EVENT_DATA * eventData 
    )

  Summary:
    Processes the SETUP packet received from the USB Controller driver.

  Description:
    This function processes the SETUP packet received from the USB Controller 
    driver.
   
  Remarks:
    This is a local function and should not be called directly by the client.
*/

void  _USB_DEVICE_ControlTransferHandler
(    
    SYS_MODULE_INDEX handlerIndex,
    USB_DEVICE_EVENT transferEvent,
    void * eventData
)
{
    USB_DEVICE_OBJ * usbDeviceThisInstance =
                            (USB_DEVICE_OBJ *)&usbDeviceInstance[handlerIndex];
    USB_SETUP_PACKET * setupPkt;
    uint8_t interfaceNumber = 0;
    USB_ENDPOINT endpointNumber;
    bool enpointFoundInDescriptors = false;
  
    if( transferEvent == USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST )
    {
        /* Get pointer to Setup Packet */ 
        setupPkt = (USB_SETUP_PACKET *)(eventData);

        /* Get of Length of the Data Stage */
        usbDeviceThisInstance->controlTransferDataStageSize = setupPkt->wLength;

        /* Cancel any IRP that is in the pipe. */
        DRV_USB_DEVICE_IRPCancelAll( usbDeviceThisInstance->usbCDHandle,
                controlEndpointTx );
        
        switch (setupPkt->Recipient)
        {
            case USB_SETUP_RECIPIENT_DEVICE:
                if (setupPkt->RequestType == USB_SETUP_REQUEST_TYPE_STANDARD)
                {
                    /* This is a standard Device Request */ 
                    if (setupPkt->DataDir == USB_SETUP_REQUEST_DIRECTION_HOST_TO_DEVICE)
                    {
                        /* Serve standard Device SET requests */
                        _USB_DEVICE_ProcessStandardDeviceSetRequests
                                            ( usbDeviceThisInstance, setupPkt );
                    }
                    else
                    {
                        /* Serve standard Device GET requests */
                        _USB_DEVICE_ProcessStandardDeviceGetRequests
                                            ( usbDeviceThisInstance, setupPkt );
                    }
                }
                else if (setupPkt->RequestType == USB_SETUP_REQUEST_TYPE_VENDOR)
                {
                    /* This is a SETUP request of Vendor type  to recipient
                     Device. The device layer does not know how to Handle these
                     requests. Forward this request to application. */
                    _USB_DEVICE_RedirectControlXfrToClient
                                (usbDeviceThisInstance, 
                                 USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST,
                                 setupPkt);
                }
                else if (setupPkt->RequestType == USB_SETUP_REQUEST_TYPE_CLASS )
                {
                    /* This is a SETUP request of Type Class to recipient
                     Device. */
                    USB_DEVICE_ControlStatus
                            ((USB_DEVICE_HANDLE)usbDeviceThisInstance,
                                               USB_DEVICE_CONTROL_STATUS_ERROR);
                }
                break;

            case USB_SETUP_RECIPIENT_INTERFACE:
                 /* Serve any requests that is not "standard" type and whose
                    recipient is not "device". (Any request whose recipient is
                    interface/endpoint must be handled by the function driver. 
                    This is because function driver has all the information about
                    endpoint and interface) */
                _USB_DEVICE_ForwardControlXfrToFunction(usbDeviceThisInstance, setupPkt->bIntfID,setupPkt ); 
                break;

            case USB_SETUP_RECIPIENT_ENDPOINT:
                /* retrieve Endpoint number from Setup Packet */
                endpointNumber = setupPkt->bEPID;

                /* Any Control request to a Non Zero Endpoint should be Stalled
                 if the Device is not in Configured State. The request will also
                 be stalled if there is no descriptor available for this
                 Endpoint */ 
                if (endpointNumber != 0)
                {
                    /* Check if the Endpoint is present in the Descriptors. If
                       yes find the interface number */
                    enpointFoundInDescriptors = _USB_DEVICE_FindEndpoint(
                            usbDeviceThisInstance,
                            endpointNumber,
                            &interfaceNumber);

                    if ((usbDeviceThisInstance->usbDeviceStatusStruct.usbDeviceState
                                                 != USB_DEVICE_STATE_CONFIGURED)
                        ||(enpointFoundInDescriptors != true))

                    {
                        /* The Device should not service the Non Zero Endpoint
                           request if the Device is not configured or the Endpoint
                           is not present in descriptors. So stall this request.
                         */

                        USB_DEVICE_ControlStatus
                            ((USB_DEVICE_HANDLE)usbDeviceThisInstance,
                                               USB_DEVICE_CONTROL_STATUS_ERROR);
                        return;
                    }
                }

                /* Program control reached here means that we have received an
                 Endpoint request on a valid Endpoint Number. */
                if (setupPkt->RequestType == USB_SETUP_REQUEST_TYPE_STANDARD)
                {
                    /* Standard Endpoint requests are handled by Device Layer */
                    _USB_DEVICE_ProcessStandardEndpointRequest(
                            usbDeviceThisInstance,
                            interfaceNumber,
                            setupPkt);
                }
                else 
                {
                    /* This is Class or Vendor request. Forward the request to
                     right Function driver or Client */ 
                    _USB_DEVICE_ForwardControlXfrToFunction
                            (usbDeviceThisInstance, interfaceNumber,setupPkt );
                   
                }
                break;
            
            case USB_SETUP_RECIPIENT_OTHER:
                /* This is a SETUP request of Type Vendor to recipient
                   Device. The device layer does not know how to Handle these
                   requests. Forward this request to application. */
                _USB_DEVICE_RedirectControlXfrToClient(
                        usbDeviceThisInstance,
                        USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST,
                        setupPkt);
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Unknow Control Transfer received from Host" );
                break;
        }
    }   
}


// ******************************************************************************
/* Function:
    void _USB_DEVICE_ForwardControlXfrToFunction (USB_DEVICE_OBJ * usbDeviceThisInstance,
                                              uint8_t interfaceNumber,
                                              USB_SETUP_PACKET * setupPkt)

  Summary:
    This function Forwards control transfers to registred function drivers.

  Description:
    This function Forwards control transfers to registred function drivers. If
    control handler fucntion callback is null, then the control transfer is
    forwarded to client.

  Remarks:
    This is local function. It should not be used directly by the client.
*/
void _USB_DEVICE_ForwardControlXfrToFunction (USB_DEVICE_OBJ * usbDeviceThisInstance,
                                              uint8_t interfaceNumber,
                                              USB_SETUP_PACKET * setupPkt)
{
    USB_DEVICE_FUNCTION_REGISTRATION_TABLE * lFuncDriverRegTable;
    USB_DEVICE_CONTROL_TRANSFER_STRUCT * controlTransfer = &usbDeviceThisInstance->controlTransfer;
    USB_DEVICE_FUNCTION_DRIVER * driver;
       /* This is a function driver /vendor specific setup packet. The recipient
     field is either interface or other. Check which function driver has to
     handle this setup packet.  */
    lFuncDriverRegTable = _USB_DEVICE_GetFunctionDriverEntryByInterface( interfaceNumber, usbDeviceThisInstance);
    if (lFuncDriverRegTable != NULL)
    {
        if (lFuncDriverRegTable->driver != NULL)
        {
            driver = (USB_DEVICE_FUNCTION_DRIVER *)(lFuncDriverRegTable->driver);
            if (driver->controlTransferNotification != NULL)
            {
                /* The interface number is owned by one of the registered function
                driver. Pass the SETUP packet to the function driver. */

                /* Save the callback and index for future use.  Here we change the
                handler to function driver specific handler.  All further control
                transfer stage must be handled by the function driver control
                transfer handler.*/

                controlTransfer->handler = (void *)driver->controlTransferNotification;
                controlTransfer->handlerIndex = lFuncDriverRegTable->funcDriverIndex;

                /* Forward the SETUP packet to the function driver */
                controlTransfer->handler( controlTransfer->handlerIndex,
                    USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST, setupPkt);
            }
            else
            {
                /* There is no valid control Transfer Notification Handler found in
                the Function registration Table. This control transfer will be
                handled by the client */
                _USB_DEVICE_RedirectControlXfrToClient
                 (usbDeviceThisInstance,
                  USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST,
                  setupPkt );
            }
        }
        else
        {
            _USB_DEVICE_RedirectControlXfrToClient
                 (usbDeviceThisInstance,
                  USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST,
                  setupPkt );
        }
    }
    else
    {
       /* The interface number specified in the SETUP Packet does not belong
          to any of the registered function driver. Host should not have sent
          this request. STALL this request */
        USB_DEVICE_ControlStatus((USB_DEVICE_HANDLE)usbDeviceThisInstance, USB_DEVICE_CONTROL_STATUS_ERROR);
    }

}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_ProcessStandardEndpointRequest
    (
        USB_DEVICE_OBJ * usbDeviceThisInstance,
        uint8_t interfaceNumber,
        USB_SETUP_PACKET * setupPkt
    )

  Summary:
    This function handles standard Endpoint requests.

  Description:
    This function handles standard Endpoint requests. 

  Remarks:
    This is local function. It should not be used directly by the client.
*/
void _USB_DEVICE_ProcessStandardEndpointRequest
(
    USB_DEVICE_OBJ * usbDeviceThisInstance,
    uint8_t interfaceNumber,
    USB_SETUP_PACKET * setupPkt
)
{
    USB_ENDPOINT usbEndpoint;
    usbEndpoint = setupPkt->bEPID;

    if( setupPkt->bRequest == USB_REQUEST_GET_STATUS )
    {
        usbDeviceThisInstance->getStatusResponse.status = 0x00;
        usbDeviceThisInstance->getStatusResponse.endPointHalt
            =  DRV_USB_DEVICE_EndpointIsStalled(usbDeviceThisInstance->usbCDHandle, usbEndpoint );

        USB_DEVICE_ControlSend( (USB_DEVICE_HANDLE)usbDeviceThisInstance,
                (uint8_t *)&usbDeviceThisInstance->getStatusResponse, 2 );
    }
    else if( setupPkt->bRequest == USB_REQUEST_CLEAR_FEATURE )
    {
        if( setupPkt->wValue == USB_FEATURE_SELECTOR_ENDPOINT_HALT )
        {
            DRV_USB_DEVICE_EndpointStallClear(usbDeviceThisInstance->usbCDHandle, usbEndpoint );
            USB_DEVICE_ControlStatus((USB_DEVICE_HANDLE)usbDeviceThisInstance,
                                            USB_DEVICE_CONTROL_STATUS_OK );
        }
    }
    else if (setupPkt->bRequest == USB_REQUEST_SET_FEATURE )
    {
        if( setupPkt->wValue == USB_FEATURE_SELECTOR_ENDPOINT_HALT )
        {
            usbEndpoint = setupPkt->bEPID;
            DRV_USB_DEVICE_EndpointStall(usbDeviceThisInstance->usbCDHandle,
                    usbEndpoint );
            USB_DEVICE_ControlStatus((USB_DEVICE_HANDLE)usbDeviceThisInstance,
                                            USB_DEVICE_CONTROL_STATUS_OK );
        }
    }
    else if (setupPkt->bRequest == USB_REQUEST_SYNCH_FRAME)
    {
        /* Forward this request to Function driver. */
        _USB_DEVICE_Handle_Synch_Frame_Request(usbDeviceThisInstance, interfaceNumber, setupPkt);
    }
}
// ******************************************************************************
/* Function:
    void _USB_DEVICE_ProcessStandardGetRequests
    (
        USB_DEVICE_OBJ * usbDeviceThisInstance,
        USB_SETUP_PACKET * setupPkt 
    )

  Summary:
    Processes the standard "get" requests received from the USB Controller driver.

  Description:
    This function processes the standard "get" requests received from the USB 
    Controller driver.
   
  Remarks:
    This is local function and should not be called directly by the client.
*/
void _USB_DEVICE_ProcessStandardDeviceGetRequests
(
    USB_DEVICE_OBJ * usbDeviceThisInstance,
    USB_SETUP_PACKET * setupPkt 
)
{
    USB_CONFIGURATION_DESCRIPTOR * lConfigDescriptor = NULL;
    uint8_t* stringDesc;
    uint16_t size = 0;
    void*  pData = NULL;
    uint8_t stringIndex;
    uint16_t langID;
    uint8_t NumberLangSupported;
    USB_DEVICE_MASTER_DESCRIPTOR * ptrMasterDescTable ;
    uint16_t langIndex;
    uint16_t CurrentLangId;

    /* Copy the Master descriptor table to a local pointer. */
    ptrMasterDescTable = usbDeviceThisInstance->ptrMasterDescTable ;
    
    if(setupPkt->bRequest == USB_REQUEST_GET_DESCRIPTOR)
    {
        switch(setupPkt->bDescriptorType)
        {
            case USB_DESCRIPTOR_DEVICE:
                if(usbDeviceThisInstance->usbDeviceStatusStruct.usbSpeed == USB_SPEED_HIGH)
                {
                    /* High speed descriptor. */
                    if( ptrMasterDescTable->highSpeedDeviceDescriptor == NULL )
                    {
                        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: High speed device descriptor is NULL" );
                    
                    }

                    /* Pointer to high speed device descriptor.*/
                    pData = ( (uint8_t*) ptrMasterDescTable->highSpeedDeviceDescriptor );
                }
                else
                {
                    /* Full/low speed descriptor.*/
                    if( ptrMasterDescTable->deviceDescriptor == NULL )
                    {   
                        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Full/Low speed device descriptor is NULL");
                    }
                    
                    /* Full/low speed device descriptor points usbDeviceThisInstance->ptrMasterDescTabler */
                    pData = ( (uint8_t*) ptrMasterDescTable->deviceDescriptor );
                }

                /* Total size of the device descriptor (Its always 18). */
                size = 18;
                break;
                
            case USB_DESCRIPTOR_CONFIGURATION:                
               
                /* Get correct pointer to the descriptor based on config value.
                 * setupPkt->bDscIndex indicates the host requested
                 * configuration index.  Make sure that the requested
                 * configuration index is with in the limits.*/
                
                if( (setupPkt->bDscIndex) < usbDeviceThisInstance->maxConfigs )
                {
                    lConfigDescriptor = (USB_CONFIGURATION_DESCRIPTOR *)usbDeviceThisInstance->configDescriptorsPtr[setupPkt->bDscIndex];
                    if(lConfigDescriptor != NULL )
                    {
                        /* We have a valid specified configuration. Prepare the
                         * output variables */
                        pData  = (uint8_t *)lConfigDescriptor;
                        size = lConfigDescriptor->wTotalLength;
                    }
                    else
                    {
                        /* Configuration not found */
                        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid pointer to configuration descriptor");
                        pData = NULL;
                    }
                }
                break;
            case USB_DESCRIPTOR_OTHER_SPEED:

                if( usbDeviceThisInstance->usbDeviceStatusStruct.usbSpeed == USB_SPEED_HIGH )
                {
                    if (((setupPkt->bDscIndex) < ptrMasterDescTable->configDescriptorCount)
                            && (ptrMasterDescTable->configDescriptorTable != NULL))
                    {
                        lConfigDescriptor = (USB_CONFIGURATION_DESCRIPTOR *)ptrMasterDescTable->configDescriptorTable[setupPkt->bDscIndex];
                    }
                }
                else
                {
                    if (((setupPkt->bDscIndex) < ptrMasterDescTable->highSpeedConfigDescriptorCount)
                            && (ptrMasterDescTable->highSpeedConfigDescriptorTable != NULL))
                    {
                        lConfigDescriptor = (USB_CONFIGURATION_DESCRIPTOR *)ptrMasterDescTable->highSpeedConfigDescriptorTable[setupPkt->bDscIndex];
                    }
                }
                if(lConfigDescriptor != NULL )
                {
                    /* We have a valid specified configuration. Prepare the
                     * output variables */
                    lConfigDescriptor->bDescriptorType = USB_DESCRIPTOR_OTHER_SPEED; 
                    pData  = (uint8_t *)lConfigDescriptor;
                    size = lConfigDescriptor->wTotalLength;
                }
                else
                {
                    /* Configuration not found */
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Invalid pointer to configuration descriptor");
                    pData = NULL;
                }
                break;
            case USB_DESCRIPTOR_DEVICE_QUALIFIER:

                if( usbDeviceThisInstance->usbDeviceStatusStruct.usbSpeed == USB_SPEED_HIGH )
                {
                    /* For high speed, respond with the other speed (full speed) device_qualifier. */
                    pData  = (uint8_t*) ptrMasterDescTable->fullSpeedDeviceQualifier;
                }
                else
                {
                    /* For full speed, respond with the other speed (high speed) device_qualifier. */
                    pData  = ((uint8_t*) ptrMasterDescTable->highSpeedDeviceQualifier );
                } 
                          
                /* Size of device_qualifier descriptor is always 10. */
                size = 10;
                break;
            
                
            case USB_DESCRIPTOR_STRING:
                /* Get the string descriptor index from setup packet*/
                stringIndex = setupPkt->bDscIndex;
                if(setupPkt->bDscIndex < ptrMasterDescTable->stringDescCount)
                {
                    /* This means the index is valid. Get correct string
                     * descriptor and update the response variable */
                    if (stringIndex == 0)
                    {
                        stringDesc = (uint8_t*)(ptrMasterDescTable->stringDescriptorTable[0] );
                        pData = (uint8_t *)stringDesc;
                        size = stringDesc[0];
                    }
                    else
                    {
                        /* Retrieve Language ID from SETUP packet */
                        langID = setupPkt->wLangID;

                        /* Find Number Languages supported from String Descriptor array index 0 */
                        NumberLangSupported = (uint8_t)((ptrMasterDescTable->stringDescriptorTable[0][0]) - 2)/2;

                        /* Search through the String Descriptor Array (Index Zero)
                        to find out Language ID Index*/
                        for (langIndex = 1; langIndex <= NumberLangSupported; langIndex++)
                        {
                            CurrentLangId = *((uint16_t*)ptrMasterDescTable->stringDescriptorTable[0] + langIndex);
                            if (CurrentLangId == langID)
                            {
                                stringDesc = (uint8_t*)( ptrMasterDescTable->stringDescriptorTable[langIndex*stringIndex] );
                                pData = (uint8_t *)stringDesc;
                                size = stringDesc[0];
                                break;
                            }
                        }
                    }
                }                                   
                break;
                
            case USB_DESCRIPTOR_BOS:
                    _USB_DEVICE_Handle_Get_Bos_Descriptor_Request
                    (
                        ptrMasterDescTable->bosDescriptor,
                        pData,
                        size
                    );
                break; 
                
            default:
                
                break;
        } 
    }
    else if (setupPkt->bRequest == USB_REQUEST_GET_CONFIGURATION)
    {
        pData = &(usbDeviceThisInstance->activeConfiguration);
        size = 1;
    }
    else if (setupPkt->bRequest == USB_REQUEST_GET_STATUS)
    {
        usbDeviceThisInstance->getStatusResponse.status = 0;
        usbDeviceThisInstance->getStatusResponse.selfPowered = usbDeviceThisInstance->usbDeviceStatusStruct.powerState;
        usbDeviceThisInstance->getStatusResponse.remoteWakeup = usbDeviceThisInstance->remoteWakeupStatus;
        pData = (uint8_t *)&usbDeviceThisInstance->getStatusResponse;
        size = 2;
    }
    if(pData == NULL)
    {
        /* We don't have valid data to send. STALL the transfer */
        USB_DEVICE_ControlStatus((USB_DEVICE_HANDLE)usbDeviceThisInstance,
                                 USB_DEVICE_CONTROL_STATUS_ERROR );
    }
    else
    {
        /* Limit the size. */
        if( size > setupPkt->wLength )
        {
            size = setupPkt->wLength;
        }

        /* Send the data stage */
        USB_DEVICE_ControlSend( (USB_DEVICE_HANDLE)usbDeviceThisInstance, pData, size );
    }
}

// ******************************************************************************
/* Function:
    USB_DEVICE_FUNCTION_REGISTRATION_TABLE * _USB_DEVICE_GetFunctionDriverEntryByInterface
    (
        uint8_t interfaceNumber,
        USB_DEVICE_OBJ * usbDeviceThisInstance
    )

  Summary:
    This function gets the correct entry in the function driver registration
    table for a given interface.

  Description:
    This function gets the correct entry in the function driver registration
    table for a given interface.

  Remarks:
    This is an internal function and should not be called directly by the client.

*/

USB_DEVICE_FUNCTION_REGISTRATION_TABLE * _USB_DEVICE_GetFunctionDriverEntryByInterface
(
    uint8_t interfaceNumber,
    USB_DEVICE_OBJ * usbDeviceThisInstance
)
{
    uint8_t count;
    uint16_t maxFunctionCounts = usbDeviceThisInstance->registeredFuncDriverCount;
    USB_DEVICE_FUNCTION_REGISTRATION_TABLE * funcRegTable = usbDeviceThisInstance->registeredFuncDrivers;
    uint8_t speed = usbDeviceThisInstance->usbDeviceStatusStruct.usbSpeed ;
    uint8_t configValue = usbDeviceThisInstance->activeConfiguration ;

    for(count = 0; count < maxFunctionCounts; count++ )
    {
        if( (funcRegTable->speed & speed) && (funcRegTable->configurationValue == configValue)
                && ( interfaceNumber >= funcRegTable->interfaceNumber ) &&
                (interfaceNumber < ( funcRegTable->interfaceNumber + funcRegTable->numberOfInterfaces )))
        {
            // Found a correct driver.
            return(funcRegTable);
        }
        funcRegTable++;
    }
    return NULL;
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_ConfigureDevice( USB_DEVICE_OBJ* usbDeviceThisInstance )

  Summary:
    This function configures the device. 

  Description:
    The initialization of all the function drivers and opening of all endpoints
    are done here.

  Parameters:
    usbDeviceThisInstance   - This instance of the USB device layer.

  Returns:
    None.
*/

void _USB_DEVICE_ConfigureDevice( USB_DEVICE_OBJ* usbDeviceThisInstance )
{
    uint16_t parsedLength= 0;
    uint16_t confTotalLength;
    uint8_t * pDescriptor = usbDeviceThisInstance->pActiveConfigDesc;
    uint8_t descriptorType;
    uint8_t interfaceNumber = 0;
    uint8_t alternateSetting = 0;
    USB_DEVICE_FUNCTION_REGISTRATION_TABLE * pFunctionRegTable = NULL;
    USB_DEVICE_FUNCTION_DRIVER * driver;
   
    confTotalLength = ((USB_CONFIGURATION_DESCRIPTOR *)pDescriptor)->wTotalLength;

    while(parsedLength < confTotalLength)
    {
        descriptorType = ((USB_DEVICE_SERVICE_DESCRIPTOR_HEAD *)pDescriptor)->bDescriptorType;
        if(descriptorType == USB_DESCRIPTOR_INTERFACE)
        {
            pFunctionRegTable = NULL;
            interfaceNumber = ((USB_INTERFACE_DESCRIPTOR * )pDescriptor)->bInterfaceNumber;
            alternateSetting = ((USB_INTERFACE_DESCRIPTOR * )pDescriptor)->bAlternateSetting;
            // Find out which function driver owns this interface in the function registration table
            pFunctionRegTable = _USB_DEVICE_GetFunctionDriverEntryByInterface( interfaceNumber ,
                                                                                    usbDeviceThisInstance);
        } 
        if( pFunctionRegTable != NULL )
        {
            driver = (USB_DEVICE_FUNCTION_DRIVER *)(pFunctionRegTable->driver);
            
            if (driver != NULL)
            {
                driver->initializeByDescriptor(pFunctionRegTable->funcDriverIndex,
                              (USB_DEVICE_HANDLE)usbDeviceThisInstance,
                              pFunctionRegTable->funcDriverInit,
                              interfaceNumber, alternateSetting,
                              descriptorType, pDescriptor);
            }
        }

        parsedLength += ((USB_DEVICE_SERVICE_DESCRIPTOR_HEAD *)pDescriptor)->bLength;
        pDescriptor += ((USB_DEVICE_SERVICE_DESCRIPTOR_HEAD *)pDescriptor)->bLength;
    }
}


// ******************************************************************************
/* Function:
    bool _USB_DEVICE_FindEndpoint( USB_DEVICE_OBJ* usbDeviceThisInstance,
                          USB_ENDPOINT endpointNumber, uint8_t* interfaceNumber)

  Summary:
    This function takes an Endpoint Number as a parameter and finds the Endpoint
    belongs to which Interface.

  Description:
    This function takes an Endpoint Number as a parameter and finds the Endpoint
    belongs to which Interface. This function finds the interface by parsing the
    USB descriptors. This is a local function and should not be called by
    applications directly.

  Parameters:
    usbDeviceThisInstance   - This instance of the USB device layer.
    endpointNumber          - Endpoint Number .
    interfaceNumber         - pointer to Interface Number. This is an out parameter.
    

  Returns:
    true - Returns true if the Endpoint number belongs to any of the registered
           interfaces. True indicates that a valid interface is found and user
           can read the interface number from the interfaceNumber parameter.
    false -Endpoint does not belong to any registered interface.
*/

bool _USB_DEVICE_FindEndpoint( USB_DEVICE_OBJ* usbDeviceThisInstance, 
                          USB_ENDPOINT endpointNumber, uint8_t* interfaceNum)
{
    uint16_t parsedLength= 0;
    uint16_t confTotalLength;
    uint8_t * pDescriptor = usbDeviceThisInstance->pActiveConfigDesc;
    uint8_t descriptorType;


    confTotalLength = ((USB_CONFIGURATION_DESCRIPTOR *)pDescriptor)->wTotalLength;

    while(parsedLength < confTotalLength)
    {
        descriptorType = ((USB_DEVICE_SERVICE_DESCRIPTOR_HEAD *)pDescriptor)->bDescriptorType;
        if(descriptorType == USB_DESCRIPTOR_INTERFACE)
        {
            *(interfaceNum) = ((USB_INTERFACE_DESCRIPTOR *)pDescriptor)->bInterfaceNumber;
        }
        else if (descriptorType == USB_DESCRIPTOR_ENDPOINT)
        {
            if(endpointNumber == ((USB_ENDPOINT_DESCRIPTOR * )pDescriptor)->bEndpointAddress)
            {
                return true;
            }
        }

        parsedLength += ((USB_DEVICE_SERVICE_DESCRIPTOR_HEAD *)pDescriptor)->bLength;
        pDescriptor += ((USB_DEVICE_SERVICE_DESCRIPTOR_HEAD *)pDescriptor)->bLength;
    }
    return false;
}
// ******************************************************************************
/* Function:
    void _USB_DEVICE_ProcessStandardSetRequests
    (
        USB_DEVICE_OBJ * usbDeviceThisInstance,
        USB_SETUP_PACKET * setupPkt
    )

  Summary:
    Processes the standard "set" requests received from the USB Controller driver.

  Description:
    This function processes the standard "set" requests received from the USB 
    Controller driver.
   
  Remarks:
    This is a local function and should not be called directly by the client.
*/

void _USB_DEVICE_ProcessStandardDeviceSetRequests
(
    USB_DEVICE_OBJ * usbDeviceThisInstance,
    USB_SETUP_PACKET * setupPkt
)
{   
    uint8_t count;
    USB_DEVICE_EVENT_DATA_CONFIGURED configuredEventData;
    USB_DEVICE_CONTROL_STATUS controlStatus = USB_DEVICE_CONTROL_STATUS_ERROR;
    bool sendStatus = true;
    
    switch(setupPkt->bRequest)
    {
        case USB_REQUEST_SET_ADDRESS:
            /* Got set address command. Change the address only after responding
               to the current request.*/
            usbDeviceThisInstance->usbDeviceStatusStruct.setAddressPending = true;
            usbDeviceThisInstance->deviceAddress = setupPkt->bDevADR;
            controlStatus = USB_DEVICE_CONTROL_STATUS_OK;
            break;

        case USB_REQUEST_SET_CONFIGURATION: 
            /* Device falls back to addressed state if configuration value is 0,
             * and if the device is already in configured state. */
            if((setupPkt->wValue == 0) && (usbDeviceThisInstance->usbDeviceStatusStruct.usbDeviceState == USB_DEVICE_STATE_CONFIGURED))
            {
                /* Configuration value 0 means, host is trying to de configure
                 * the device.  Set a event here. USB device layer task will
                 * de initialize the function drivers later.*/
                usbDeviceThisInstance->event = USB_DEVICE_EVENT_DECONFIGURED;

                /* Deinit all function drivers. */
                _USB_DEVICE_DeInitializeAllFunctionDrivers ( usbDeviceThisInstance );

                /* Change the current active configuration to Zero */
                usbDeviceThisInstance->activeConfiguration = 0;

                /* Change the state to Addressed   */
                usbDeviceThisInstance->usbDeviceStatusStruct.usbDeviceState = USB_DEVICE_STATE_ADDRESSED;
            }  
            else
            {
                /* Proceed only if new configuration value is different from
                 * current configuration value. */
                if( usbDeviceThisInstance->activeConfiguration != (uint8_t)setupPkt->wValue)
                {
                    for(count = 0; count < usbDeviceThisInstance->maxConfigs; count++)
                    {
                        /* 5th byte in the configuration descriptor table
                         * specifies the configuration value. */
                        if( usbDeviceThisInstance->configDescriptorsPtr[count][5] == setupPkt->bConfigurationValue )
                        {
                            /* Got a configuration match. Get the pointer to
                             * configuration descriptor. We have to pass this to
                             * function driver, so that function driver can
                             * parse configuration descriptor and get the
                             * required info. */
                            usbDeviceThisInstance->pActiveConfigDesc = (uint8_t *)usbDeviceThisInstance->configDescriptorsPtr[count];
                        }
                    }

                    /* Save the current active configuration.  This may be
                     * required for clients to know which configuration is
                     * presently active. */
                    usbDeviceThisInstance->activeConfiguration = (uint8_t)setupPkt->wValue;

                    /* In case the endpoint funcitions are enabled for Vendor operation,
                     * the endpoint queue sizes need to reset. */
                     _USB_DEVICE_EndpointCurrentQueueSizeReset(usbDeviceThisInstance->usbDevLayerIndex);

                    /* Initialize all function drivers and change to configured
                     * state only if all function drivers are initialized
                     * successfully. */
                    _USB_DEVICE_ConfigureDevice(usbDeviceThisInstance);

                    /* Change the state to configured. */
                    usbDeviceThisInstance->usbDeviceStatusStruct.usbDeviceState = USB_DEVICE_STATE_CONFIGURED;

                    /* Set an event, so that application and function drivers
                     * are informed  about the same. */
                    configuredEventData.configurationValue = (uint8_t)setupPkt->wValue;
                    /* Inform the client about the event */
                    if((usbDeviceThisInstance->clientState == USB_DEVICE_CLIENT_STATUS_READY) &&
                        (usbDeviceThisInstance->callBackFunc != NULL))
                    {
                        /* This means this client is valid and is a client of this device
                           layer instance. Pass event to application */
                        usbDeviceThisInstance->callBackFunc
                                (USB_DEVICE_EVENT_CONFIGURED,
                                 &configuredEventData,
                                 usbDeviceThisInstance->context);
                    }
                }
            }
            controlStatus = USB_DEVICE_CONTROL_STATUS_OK;
            break;
         case  USB_REQUEST_CLEAR_FEATURE:
            if( setupPkt->wValue == USB_FEATURE_SELECTOR_DEVICE_REMOTE_WAKEUP )
            {
                usbDeviceThisInstance->remoteWakeupStatus
                                           = USB_DEVICE_REMOTE_WAKEUP_DISABLED ;
                controlStatus = USB_DEVICE_CONTROL_STATUS_OK;
            }
            break;

        case USB_REQUEST_SET_FEATURE:
            if (setupPkt->wValue == USB_FEATURE_SELECTOR_DEVICE_REMOTE_WAKEUP)
            {
                usbDeviceThisInstance->remoteWakeupStatus
                                            = USB_DEVICE_REMOTE_WAKEUP_ENABLED;
                controlStatus = USB_DEVICE_CONTROL_STATUS_OK;
            }
            else if (setupPkt->wValue == USB_FEATURE_SELECTOR_DEVICE_TEST_MODE)
            {
                /* Not supported as of now */ 
                controlStatus = USB_DEVICE_CONTROL_STATUS_ERROR;
            }
            break;

        case USB_REQUEST_SET_DESCRIPTOR:      
            /* All SET_DESCRIPTOR requests are directly forwarded to application */
            _USB_DEVICE_Handle_Set_Descriptor_Request (
                    usbDeviceThisInstance,
                    USB_DEVICE_EVENT_SET_DESCRIPTOR,
                    setupPkt);
            controlStatus = USB_DEVICE_CONTROL_STATUS_ERROR;
            break;

        default:
            /* Respond with a request error. Stall the endpoint. Stall the EP0
             * TX. */
            controlStatus = USB_DEVICE_CONTROL_STATUS_ERROR;
            break;
    }

    /* Send ZLP */
    if (sendStatus)
    {
        USB_DEVICE_ControlStatus( (USB_DEVICE_HANDLE)usbDeviceThisInstance,
                              controlStatus);
    }
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_RedirectControlXfrToClient
    (
        USB_DEVICE_OBJ* usbDeviceThisInstance ,
        USB_DEVICE_EVENT event,
        USB_SETUP_PACKET * setupPkt
    )

  Summary:
    This function forwards control transfers to client.

  Description:
    This function forwards control transfers to client.

  Remarks:
    This is local function. It should not be used directly by the client.
*/
void _USB_DEVICE_RedirectControlXfrToClient
(
    USB_DEVICE_OBJ* usbDeviceThisInstance ,
    USB_DEVICE_EVENT event,
    USB_SETUP_PACKET * setupPkt
)
{
    USB_DEVICE_CONTROL_TRANSFER_STRUCT * controlTransfer = &usbDeviceThisInstance->controlTransfer;
    
    /* This control transfer will be handled by the client */
    controlTransfer->handler = (void *)usbDeviceThisInstance->callBackFunc;
    controlTransfer->handlerIndex = usbDeviceThisInstance->usbDevLayerIndex ;

    /* Let app clients handle the SETUP packet. */
    if((usbDeviceThisInstance->clientState == USB_DEVICE_CLIENT_STATUS_READY) &&
    (usbDeviceThisInstance->callBackFunc != NULL))
    {
        /* This means this is a valid client. Pass the control transfer
         * event to the client. */
        usbDeviceThisInstance->callBackFunc(
            event,
            setupPkt,
            usbDeviceThisInstance->context );
    }

}

// ******************************************************************************
/* Function:
    USB_DEVICE_CLIENT_HANDLE _USB_DEVICE_ClientHandleValidate
    (
        USB_DEVICE_HANDLE deviceHandle
    )

  Summary:
    Validates the client device handle.

  Description:
    This function validates a device handle. It returns NULL if the device
    handle is not valid. It returns the pointer to the client object associated
    with the handle otherwise.

  Remarks:
    This is a local function and should not be called directly by the client.
*/

USB_DEVICE_OBJ* _USB_DEVICE_ClientHandleValidate(USB_DEVICE_HANDLE deviceHandle)
{
    /* This function validates the client handle and return NULL if the client
       handle is invalid. */

    USB_DEVICE_OBJ* client;

    if((USB_DEVICE_HANDLE_INVALID == deviceHandle) 
        || (0 == deviceHandle))
    {
        return (NULL);
    }

    /* Check if the client object is in use */
    client = (USB_DEVICE_OBJ *) deviceHandle;
    if(!client->inUse)
    {
        return(NULL);
    }

    /* Return the client handle */
    return(client);
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_EndpointMutexCreateFunction(USB_DEVICE_OBJ* usbDeviceThisInstance)

  Summary:
    Creats mutex which is required for Endpoint read write functions.

  Description:
    Creats mutex which is required for Endpoint read write functions.

  Remarks:
    This is a local function and should not be called directly by the client.
*/
void _USB_DEVICE_EndpointMutexCreateFunction(USB_DEVICE_OBJ* usbDeviceThisInstance)
{
    OSAL_RESULT osalResult;
    osalResult = OSAL_MUTEX_Create(&(usbDeviceThisInstance->mutexEndpointIRP));
    if(osalResult != OSAL_RESULT_TRUE)
    {
        usbDeviceThisInstance->isMutexEndpointIrpInitialized = false;
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Mutex Create Failed");
    }
    else
    {
        usbDeviceThisInstance->isMutexEndpointIrpInitialized = true;
    }
}

// ******************************************************************************
/* Function:
    void _USB_DEVICE_EndpointMutexDeleteFunction(USB_DEVICE_OBJ* usbDeviceThisInstance)

  Summary:
    Deletes mutex which is required for Endpoint read write functions.

  Description:
    Deletes mutex which is required for Endpoint read write functions.

  Remarks:
    This is a local function and should not be called directly by the client.
*/
void _USB_DEVICE_EndpointMutexDeleteFunction(USB_DEVICE_OBJ* usbDeviceThisInstance)
{
    OSAL_RESULT osalResult;
    osalResult = OSAL_MUTEX_Delete(&(usbDeviceThisInstance->mutexEndpointIRP));
    if(osalResult != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "USB Device Layer: Mutex Delete Failed");
    }
    else
    {
        usbDeviceThisInstance->isMutexEndpointIrpInitialized = false;
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: USB Device Layer Software version functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    unsigned int USB_DEVICE_VersionGet ( void )

  Summary:
    Gets the USB Device Layer version in numerical format.

  Description:
    This function gets the USB Device Layer version. The version is encoded as
    major * 10000 + minor * 100 + patch. The stringed version can be obtained
    using USB_DEVICE_VersionStrGet()
 
  Remarks:
    Refer to usb_device.h for usage information.

*/

// *****************************************************************************
/* Function:
    char * USB_DEVICE_VersionStrGet ( )

  Summary:
    Gets the USB Device Layer version in string format.

  Description:
    This function gets the USB Device Layer version. The version is returned as
    major.minor.path[type], where type is optional. The numerical version can
    be obtained using USB_DEVICE_VersionGet.

  Remarks:
    Refer to usb_device.h for usage information.
*/
unsigned int USB_DEVICE_VersionGet( void )
{
    return( ( _USB_DEVICE_VERSION_MAJOR * 100000 ) +
            ( _USB_DEVICE_VERSION_MINOR * 100 ) +
            ( _USB_DEVICE_VERSION_PATCH ) );

} /* USB_DEVICE_VersionGet */


// *****************************************************************************

char * USB_DEVICE_VersionStrGet( void )
{
    return _USB_DEVICE_VERSION_STR;

} /* USB_DEVICE_VersionStrGet */
/********************End of file********************************/
