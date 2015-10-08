/********************************************************************************
  USB Host Layer implemention.

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host.c

  Summary:
    This file contains implementations of both private and public functions
    of the USB Host Layer.

  Description:
    This file contains the USB host layer implementation. This file should be
    included in the project if USB Host functionality is desired.
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
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "system_config.h"
#include "usb/beta_sw/usb_common.h"
#include "usb/beta_sw/usb_chapter_9.h"
#include "usb/beta_sw/usb_host.h"
#include "usb/beta_sw/src/usb_host_local.h"
#include "driver/tmr/drv_tmr.h"
#include "system/tmr/sys_tmr.h"
#include "system/debug/sys_debug.h"
#include "system/common/sys_module.h"



// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/**********************************
 * Host layer instance objects.
 *********************************/
static USB_HOST_OBJ  gUSBHostObj[USB_HOST_INSTANCES_NUMBER];

/**********************************
 * Host client objects.
 *********************************/
static USB_HOST_INSTANCE_CLIENT gUSBHostClients [USB_HOST_CLIENTS_NUMBER];

// *****************************************************************************
// *****************************************************************************
// Section: USB HOST Layer Interface Implementations
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ USB_HOST_Initialize
    (
       const SYS_MODULE_INDEX index,
       const SYS_MODULE_INIT * const initData
    )

  Summary:
    Dynamic impementation of USB_HOST_Initialize system interface function.

  Description:
    This is the dynamic impementation of USB_HOST_Initialize system interface
    function.

  Remarks:
    See usb_host.h for usage information.
*/

SYS_MODULE_OBJ USB_HOST_Initialize
(
    const SYS_MODULE_INDEX index,
    const SYS_MODULE_INIT * const initData 
)
{
    /* Host controller object */
    USB_HOST_OBJ            *hcObj = (USB_HOST_OBJ *)NULL;
    USB_HOST_INIT           *hostInit = NULL;
    DRV_USB_INIT             drvUsbInit;

    /* Make sure the index is with in range. */
    if(( index < 0 ) || ( index >= USB_HOST_INSTANCES_NUMBER ))
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO,
                    "USB_HOST_Initialize : Invalid System Module Index \r\n ");
        return (SYS_MODULE_OBJ_INVALID);
    }

    /* Assign to the local pointer the init data passed */
    hostInit = ( USB_HOST_INIT * ) initData ;

    /* Make sure that initData is not NULL. */
    if(hostInit == NULL)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                     "USB_HOST_Initialize : Initialization data is NULL \r\n");
        return (SYS_MODULE_OBJ_INVALID);
    }

    /* Check if this hardware instance was already initialized */
    if(gUSBHostObj[index].inUse != false)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                        "USB_HOST_Initialize : Instance already in use \r\n ");
        return ( SYS_MODULE_OBJ_INVALID );
    }
    
    /* Allocate the Host controller object and set the operation
     * flag to be in use */
    hcObj =  &(gUSBHostObj[index]);
    hcObj->inUse = true;
    hcObj->attachDevicesCount = 0;
    hcObj->usbHostTpl  = hostInit->tplList ;
    hcObj->nTPLEntries = hostInit->nTPLEntries;

    /* Fill Controller driver data structer */
    drvUsbInit.interruptSource  = hostInit->interruptSource ;
    drvUsbInit.interruptSourceUSBDma = hostInit->interruptSourceUSBDma;
    drvUsbInit.moduleInit       = hostInit->moduleInit ;
    drvUsbInit.usbID            = hostInit->usbID ;
    drvUsbInit.stopInIdle       = hostInit->stopInIdle ;
    drvUsbInit.endpointTable    = hostInit->endpointTable;
    /* DRV_USB_OPMODE_HOST. This value is independant of PIC32MX and PIC32MZ */
    drvUsbInit.operationMode    = DRV_USB_OPMODE_HOST ;
    drvUsbInit.suspendInSleep   = hostInit->suspendInSleep ;
    drvUsbInit.operationSpeed   = hostInit->usbSpeed ;
    drvUsbInit.interruptSourceUSBDma = hostInit->interruptSourceUSBDma;
    /* USB HCD Initialize */
    hcObj->usbHCDSystemModuleObject =
                    DRV_USB_Initialize(index ,( SYS_MODULE_INIT* )&drvUsbInit);

    /* Check if the HCD initialization was successful */
    if ( SYS_MODULE_OBJ_INVALID == hcObj->usbHCDSystemModuleObject )
    {
        hcObj->inUse = false;
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO, "USB_HOST_Initialize : "
                            "Could not intialize Host Controller Driver \r\n");
        return ( SYS_MODULE_OBJ_INVALID );
    }
    
    /* Based on the speed support we have to allocate band width
     * for periodic transfer in a frame */
    if( hostInit->usbSpeed ==  USB_SPEED_FULL || 
        hostInit->usbSpeed ==  USB_SPEED_LOW )
    {
        hcObj->totalBandwidth =  USB_HOST_FULL_SPEED_BANDWIDTH;
        hcObj->availableBandwidth = USB_HOST_FULL_SPEED_BANDWIDTH;
    }
    else
    {
        hcObj->totalBandwidth =  USB_HOST_HIGH_SPEED_BANDWIDTH;
        hcObj->availableBandwidth = USB_HOST_HIGH_SPEED_BANDWIDTH;
    }

    /* Set the status to busy. We cannot open the Host Layer yet */
    hcObj->usbHostInstanceState = SYS_STATUS_BUSY ;

    /* Root Hub has only one port So may not have separate root hub driver */
    /* Root hub also one device so will enumerate the root hub and assign
     * address for root hub */
    /* Root hub responsabilities : Port suspend
     *                             port resume
     *                             port reset
     *                             port enable
     *                             port power   */
    /* We are sure that one controller is having only one port */

    /* Device address " 0 " for newly connected device */
    /* Device address is " 1 " for reserved for RootHUB */
    /* Fill these address */
    hcObj->addressBits[0] =  0x3 ;

    /* The rest of the initialization is completed in the task routine */
    hcObj->state = USB_HOST_TASK_STATE_OPENING_USBHCD;

    /* Return handle as object */
    return(index);
}

// *****************************************************************************
/* Function:
    SYS_STATUS USB_HOST_Status( SYS_MODULE_OBJ object )

  Summary:
    Dynamic impementation of USB_HOST_Status system interface function.

  Description:
    This is the dynamic impementation of USB_HOST_Status system interface
    function.

  Remarks:
    See usb_host.h for usage information.
*/

SYS_STATUS USB_HOST_Status (SYS_MODULE_OBJ usbHostObject)
{
    SYS_MODULE_INDEX index = (SYS_MODULE_INDEX)usbHostObject;
    /* Check if we have a valid object */
    if(usbHostObject == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                        "USB_HOST_Status : Invalid system module object \r\n");
        return(SYS_STATUS_UNINITIALIZED);
    }

    /* Validate Host instance number */
    if( usbHostObject >= USB_HOST_INSTANCES_NUMBER)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                        "USB_HOST_Status : Invalid System Module Object \r\n");
        return ( SYS_STATUS_UNINITIALIZED );
    }

    /* Return the system status of the hardware instance object */
    return ( gUSBHostObj[index].usbHostInstanceState );
}

// *****************************************************************************
/* Function:
    void USB_HOST_Deinitialize ( SYS_MODULE_OBJ usbHostObject )

  Summary:
    Dynamic impementation of USB_HOST_Deinitialize system interface function.

  Description:
    This is the dynamic impementation of USB_HOST_Deinitialize system interface
    function.

  Remarks:
    See usb_host.h for usage information.
*/

void USB_HOST_Deinitialize ( SYS_MODULE_OBJ usbHostObject )
{
    /* Host object */
    USB_HOST_OBJ       *hcObj;
    SYS_MODULE_INDEX index = (SYS_MODULE_INDEX)usbHostObject;

    /* Validate the USB Host index */
    if(usbHostObject == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                  "USB_HOST_Deinitialize : Invalid system module object \r\n");
        return;
    }

    /* Validate the USB Host instance number */
    if( usbHostObject >= USB_HOST_INSTANCES_NUMBER)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                  "USB_HOST_Deinitialize : Invalid System Module Object \r\n");
        return;
    }

    /* Address mapping */
    hcObj =  &(gUSBHostObj[index]);

    /* Validate Host is already assigned */
    if(hcObj->inUse == false )
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                   "USB_HOST_Deinitialize : Instance is not initialized \r\n");
        return;
    }
 
    /* Indicate that this object is not is use */
    hcObj->inUse = false;

    /* Set the instance status to deinitialized */
    hcObj->usbHostInstanceState =  SYS_STATUS_UNINITIALIZED ;
}


// *****************************************************************************
/* Function:
    void USB_HOST_Tasks (SYS_MODULE_OBJ object );

  Summary:
    Maintains the USB Host's state machine. It manages the USB Module driver and
    responds to USB Module driver events.

  Description:
    This routine maintains the USB Host layer's state machine. It manages and
    the USB Module driver and responds to USB module driver events.  This
    function should be called from the SYS_Tasks() function.

 Remarks:
    See usb_host.h for usage information.
*/

void USB_HOST_Tasks   ( SYS_MODULE_OBJ usbHostObject )
{

    USB_HOST_OBJ                    *hcObj;
    uint8_t                         deviceCount;
    USB_HOST_CONFIGURATION_INFO     *configurationInfo;
    uint8_t                         interfaceNumber;
    SYS_MODULE_INDEX                index = (SYS_MODULE_INDEX)usbHostObject;

    /* Assert object is valid. */
    if( usbHostObject == SYS_MODULE_OBJ_INVALID )
    {
       SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                  "USB_HOST_Tasks : Invalid system module object \r\n");
        return;
    }

    /* Get this instance of USB host layer.*/
    hcObj =  &( gUSBHostObj[index] );

    /* Validate Host is already assigned */
    if( hcObj->inUse == false )
    {
        return;
    }

    /* Proceed only if this instance is in initialized state. */
    if( hcObj->usbHostInstanceState <= SYS_STATUS_UNINITIALIZED )
    {
        /* Instance is not yet initialized. Just do a return. */
        return;
    }

    /* Update the USB HCD State machine here */
    DRV_USB_Tasks( hcObj->usbHCDSystemModuleObject );

    switch ( hcObj->state )
    {
        case USB_HOST_TASK_STATE_OPENING_USBHCD:

            hcObj->hostControllerHandle =
                    DRV_USB_Open ( index , DRV_IO_INTENT_EXCLUSIVE |
                                           DRV_IO_INTENT_NONBLOCKING |
                                           DRV_IO_INTENT_READWRITE );

            /* Validate the Open function status */
            if ( DRV_HANDLE_INVALID == hcObj->hostControllerHandle )
            {
                SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                                   "USB_HOST_Tasks : HCD open failed  \r\n");
                return;

            }
            else
            {
                /* The driver could be opened. Register the event callback */
                DRV_USB_ClientEventCallBackSet( hcObj->hostControllerHandle,
                                                usbHostObject ,
                                                _USB_HOST_EventHandler );
                hcObj->usbHostInstanceState = SYS_STATUS_READY ;
                hcObj->state = USB_HOST_TASK_STATE_RUNNING;
            }

            break;

        case   USB_HOST_TASK_STATE_RUNNING :

            /* In this state, the host layer performs it's actual task, that
             * is calling the tasks routines of the class drivers, checking if a
             * device attached or detached and enumerating the attached device */

            /* Update device state */
            _USB_HOST_UpdateDeviceState( hcObj );

            /* Find any device is enumerated */
            if ( hcObj->attachDevicesCount > 0 )
            {
                /* This means that one or more devices are attached.
                 * Call the task routines for the attached devices Tasks */
                /* This loop is for Vendor or Class specific driver tasks */
                for ( deviceCount = 0 ;
                      deviceCount < USB_HOST_MAX_DEVICES_NUMBER;
                      deviceCount++ )
                {

                    /* Check if this device has a device level driver attached
                     * to it */
                    if (( hcObj->attachedDevicesList[deviceCount].inUse )
                            && ( hcObj->attachedDevicesList
                                [deviceCount].driver != NULL))
                    {
                        /* Call the device level driver tasks routine. */
                        if ( hcObj->attachedDevicesList[deviceCount].
                                                        driver->task != NULL )
                        {
                            hcObj->attachedDevicesList[deviceCount].driver->
                                task( hcObj->attachedDevicesList[deviceCount].
                                instanceNumber );

                        }
                            
                    }

                    /* Get the interface drivers and call the tasks routines if
                     * there are any interface level drivers loaded for this
                     * device */
                    configurationInfo =  &( hcObj->attachedDevicesList
                                          [deviceCount].configDescriptorInfo);

                    if (configurationInfo->configurationDescriptor == 0)
                    {
                        // not a valid device, check the next one
                        continue;
                    }
                    for( interfaceNumber = 0 ;
                         interfaceNumber < configurationInfo->
                            configurationDescriptor->bNumInterfaces ;
                            interfaceNumber++ )
                    {
                        if ( configurationInfo->interfaceInfo[interfaceNumber].
                                interfaceDriver != NULL )
                        {
                            /* Call the Interface level driver task routines */
                            configurationInfo->interfaceInfo[interfaceNumber].
                        interfaceDriver->task( configurationInfo->interfaceInfo
                                        [interfaceNumber].instanceNumber );
                        }
                    }
                }
            }
            break;

        default :
            break;
    }
}
// *****************************************************************************
/* Function:
    USB_HOST_HANDLE USB_HOST_Open
    (
        const SYS_MODULE_INDEX index,
        const DRV_IO_INTENT    ioIntent
    )

  Summary:
    Dynamic impementation of USB_HOST_Open client interface function.

  Description:
    This is the dynamic impementation of USB_HOST_Open client interface
    function.

  Remarks:
    See usb_host.h for usage information.
*/

USB_HOST_HANDLE USB_HOST_Open
( 
    const SYS_MODULE_INDEX index,
    const DRV_IO_INTENT    intent
)
{
    USB_HOST_OBJ       *hcObj;
    int                iClient;
    USB_HOST_INSTANCE_CLIENT* usbHostClient;

    /* Validate host index number */
    if ( (index < 0) || ( index >= USB_HOST_INSTANCES_NUMBER))
    {
        /* Invalid host index */
          SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                            " USB_HOST_Open : Invalid System Module Index");
        return ( USB_HOST_HANDLE_INVALID );
    }
    
    /* Address mapping */
    hcObj =  &(gUSBHostObj[index]);

    /* We can open the Host Layer only if its ready. */
    if( hcObj->usbHostInstanceState != SYS_STATUS_READY )
    {
        /* The USB Host module should not be ready */
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                "USB_HOST_Open : Host Layer is not ready to be opened yet.\n");
        return ( USB_HOST_HANDLE_INVALID );
    }

    /* Find the slot from the pool */
    for(iClient = 0; iClient < USB_HOST_CLIENTS_NUMBER ; iClient++)
    {
         if ( !gUSBHostClients[iClient].inUse)
        {
            /* This means we have a free client object to use */
           usbHostClient= (USB_HOST_INSTANCE_CLIENT *)&gUSBHostClients[iClient];
           usbHostClient->inUse = true;
           usbHostClient->usbHostInstance = hcObj;
           usbHostClient->callBackFunc = NULL;
           usbHostClient->context = ( uintptr_t ) 0 ; 
           /* Update the client status */
           usbHostClient->clientState =  DRV_CLIENT_STATUS_READY;
           hcObj->appHandle = (USB_HOST_HANDLE) usbHostClient;
           return ((USB_HOST_HANDLE) usbHostClient );
        }
     }

    SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                            " USB_HOST_Open : No free client objects");

    /* If we have reached here, it means either we could not find a spare
       client object. */
    return (USB_HOST_HANDLE_INVALID);
}

// *****************************************************************************
/* Function:
    void USB_HOST_Close ( DRV_HANDLE handle)

  Summary:
    Dynamic impementation of USB_HOST_Close client interface function.

  Description:
    This is the dynamic impementation of USB_HOST_Close client interface
    function.

  Remarks:
    See usb_host.h for usage information.
*/

void USB_HOST_Close (   USB_HOST_HANDLE hClient )
{
    USB_HOST_INSTANCE_CLIENT *usbHostClient;

    /* Validate the client handle */
    usbHostClient =   _USB_HOST_ClientHandleValidate(hClient);

    if(usbHostClient == NULL)
    {
       /* Handle is not valid */
       SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
               "USB_HOST_Close : Invalid client handle");
       return;
    }

    /* close the handle. */
    usbHostClient->clientState = DRV_CLIENT_STATUS_CLOSED;
    
    /* Free the client */
    usbHostClient->inUse = false;
}


// *****************************************************************************
/* Function:
    void USB_HOST_EventCallBackSet
    (
        USB_HOST_HANDLE hostHandle,
        USB_HOST_EVENT_CALLBACK callback,
        uintptr_t context
    );

  Summary:
    Allows a client to identify an event handling function for the host layer
    to call back when host layer events are generated.

  Description:
    This function allows a client to identify an event handling function for the
    host layer to call back when host layer events are generated.

    The event handler should be set before the client the host layer operation
    is enabled. The event handler once set, persists until the client closes the
    driver or sets another event handler.

  Remarks:
    See usb_host.h for usage information.
*/

void USB_HOST_EventCallBackSet
(
    USB_HOST_HANDLE hostHandle,
    USB_HOST_EVENT_CALLBACK callBackFunk,
    uintptr_t context
)
{
    USB_HOST_INSTANCE_CLIENT* usbHostClient;

    /* Validate the client handle */
    usbHostClient =   _USB_HOST_ClientHandleValidate(hostHandle);

    if(usbHostClient == NULL)
    {
       /* Handle is not valid */
       SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
               "USB_HOST_EventCallBackSet : Invalid client handle");
       return;
    }
    
    /* Register the callback function. Note that the call back function is
     * invoked for control transfer events.  */
    usbHostClient->callBackFunc = callBackFunk;
    usbHostClient->context = context;
       
}

// *****************************************************************************
/* Function:
    void USB_HOST_OperationEnable( USB_HOST_HANDLE handle )

  Summary:
    Enables host operation.

  Description:
    The function enables host operation. When enabled, the host layer can detect
    and enumerate attached devices. The application must call the
    USB_HOST_OperationIsEnabled() function to check if the operation has
    completed.

  Remarks:
    See usb_host.h for usage information.
*/

void USB_HOST_OperationEnable ( USB_HOST_HANDLE hostHandle )
{
    USB_HOST_INSTANCE_CLIENT* usbHostClient;
    USB_HOST_OBJ       *hcObj;

    usbHostClient = _USB_HOST_ClientHandleValidate( hostHandle );

    if(usbHostClient == NULL)
    {
       /* Handle is not valid */
       SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
               "USB_HOST_OperationEnable : Invalid client handle");
       return;
    }
    /* Host object*/
    hcObj = usbHostClient->usbHostInstance;
    
    if( hcObj->usbHostInstanceState == SYS_STATUS_READY )
    {
        /* Enable USB controller */
        DRV_USB_HOST_OperationEnable( hcObj->hostControllerHandle , true);
    }
}

// *****************************************************************************
/* Function:
    void USB_HOST_OperationDisable( USB_HOST_HANDLE handle )

  Summary:
    Disabled host operation.

  Description:
    The function disables host operation. Disabling the host operation will cause
    the host to ignore attached device. All bus communication will be halted and
    USB bus will be suspended.

  Remarks:
    See usb_host.h for usage information.
*/

void  USB_HOST_OperationDisable (USB_HOST_HANDLE hostHandle)
{
    
    USB_HOST_INSTANCE_CLIENT* usbHostClient;
    USB_HOST_OBJ       *hcObj;

    usbHostClient = _USB_HOST_ClientHandleValidate( hostHandle );

    if(usbHostClient == NULL)
    {
       /* Handle is not valid */
       SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
               "USB_HOST_OperationDisable : Invalid client handle");
       return;
    }

    hcObj = usbHostClient->usbHostInstance;
     /* Disable USB controller */
    DRV_USB_HOST_OperationEnable(hcObj->hostControllerHandle, false);
}

// *****************************************************************************
/* Function:
    bool USB_HOST_OperationIsEnabled( USB_HOST_HANDLE handle )

  Summary:
    Allows the application to check if the host operation is enabled.

  Description:
    The function allows the application to check if the host operation is
    enabled. This function should be called after the USB_HOST_OperationEnable()
    function is called to check if the operation has been enabled.

  Remarks:
    See usb_host.h for usage information.
*/

bool  USB_HOST_OperationIsEnabled (USB_HOST_HANDLE hostHandle)
{
    USB_HOST_INSTANCE_CLIENT* usbHostClient;
    USB_HOST_OBJ       *hcObj;
 
    /* validate and get clientHandle*/
    usbHostClient = _USB_HOST_ClientHandleValidate( hostHandle );

    if(usbHostClient == NULL)
    {
       /* Handle is not valid */
       SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
               "USB_HOST_OperationDisable : Invalid client handle");
       return false;
    }
   
    /*Find the host object*/
    hcObj = usbHostClient->usbHostInstance;
    
    /* check the SOF status */
    return ( DRV_USB_HOST_OperationIsEnabled(hcObj->hostControllerHandle));
    
}


// *****************************************************************************
/* Function:
    void USB_HOST_Tasks_ISR (SYS_MODULE_OBJ devLayerObj);

  Summary:
    USB Host Layer Tasks routine to be called in the USB Interrupt Service
    Routine.

  Description:
    This function must be called in the USB Interrupt Service Routine if the
    Host Stack is configured for interrupt mode. In case the Host Stack is
    configured for polling mode, this function is automatically called from the
    USB_HOST_Tasks() function. hostLayerObj must be the system module object
    associated with the USB module generating the interrupt.

  Remarks:
    Refer to usb_device.h for usage information.
*/

void USB_HOST_Tasks_ISR(SYS_MODULE_OBJ hostLayerObj)
{
    USB_HOST_OBJ * usbHostThisInstance;

    /* Assert object is valid. */
    if(hostLayerObj == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                " USB_HOST_Tasks_ISR : System Module Object is invalid");
        return;
    }

    /* Get this instance of USB Host layer.*/
    usbHostThisInstance = &gUSBHostObj[hostLayerObj];

    /* Proceed only if this instance is in initialized state. */
    if( usbHostThisInstance->usbHostInstanceState <= SYS_STATUS_UNINITIALIZED )
    {
        /* Instance is not yet initialized. Just do a return. */
        return;
    }

    DRV_USB_Tasks_ISR(usbHostThisInstance->usbHCDSystemModuleObject);
}

void USB_HOST_Tasks_ISR_USBDMA(SYS_MODULE_OBJ hostLayerObj)
{
    USB_HOST_OBJ * usbHostThisInstance;

    /* Assert object is valid. */
    if(hostLayerObj == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                " USB_HOST_Tasks_ISR : System Module Object is invalid");
        return;
    }

    /* Get this instance of USB Host layer.*/
    usbHostThisInstance = &gUSBHostObj[hostLayerObj];

    /* Proceed only if this instance is in initialized state. */
    if( usbHostThisInstance->usbHostInstanceState <= SYS_STATUS_UNINITIALIZED )
    {
        /* Instance is not yet initialized. Just do a return. */
        return;
    }

    DRV_USB_Tasks_ISR_USBDMA(usbHostThisInstance->usbHCDSystemModuleObject);
}

// *****************************************************************************
/* Function:
    USB_HOST_DEVICE_INFO *  _USB_HOST_GetDeviceInfo( USB_HOST_DEVICE_ID Id )

  Summary:
    Get device information based on host controller id and device id

  Description:
    Access the deviceinfo

   Remarks
    This is a local function and should not be called directly by the
    application.
*/

USB_HOST_DEVICE_INFO *  _USB_HOST_GetDeviceInfo( USB_HOST_DEVICE_ID deviceId )
{
    uint8_t                count;
    USB_HOST_OBJ           *hcObj;
    USB_HOST_DEVICE_INFO   *deviceInfo;
    uint8_t                hostControllerId;
    uint8_t                deviceAddress;

     /*Host controller index */
    hostControllerId = ( uint8_t ) deviceId >> 8 ;

    if(  USB_HOST_INSTANCES_NUMBER <=  hostControllerId )
    {
         SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                     " _USB_HOST_GetDeviceInfo : Invalid host object ");
         return ( NULL);

    }
    /* Device Address */
    deviceAddress    = ( uint8_t ) ( deviceId & 0xFF );

    /* Bus information  */
    hcObj =  & ( gUSBHostObj[ hostControllerId ] );

    /* Compare device address with the host attached device list */
    for ( count = 0; count < USB_HOST_MAX_DEVICES_NUMBER ; count++ )
    {
        if( hcObj->attachedDevicesList[ count ].deviceId == deviceId )
        {
            /* Device address found */
            deviceInfo = &( hcObj->attachedDevicesList[ count ] );
            return ( deviceInfo );
        }
    }

    SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                     " _USB_HOST_GetDeviceInfo : Invalid device address ");
    /* Device address is not available error */
    return ( NULL ) ;
}

// *****************************************************************************
/* Function:
   USB_ERROR  _USB_HOST_FindInterface
   (
        USB_HOST_QUERY *query,
        USB_INTERFACE_DESCRIPTOR **interfaceDesc
   )

  Summary:
    Find interface descriptor based on query

  Description:
    USB Host driver can query the host and get the interface descriptor

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/


USB_ERROR  _USB_HOST_FindInterface
(
    USB_HOST_QUERY *query,
    USB_INTERFACE_DESCRIPTOR **interfaceDesc
)
{
    USB_ERROR                       status = USB_ERROR_NONE ;
    USB_INTERFACE_DESCRIPTOR        *interfaceDescriptor;
    USB_DESCRIPTOR_HEADER           *header;
    uint16_t                         size;
    USB_HOST_DEVICE_INFO            *deviceInfo;
    USB_CONFIGURATION_DESCRIPTOR    *tempcfg;
    uint8_t                         *buffer;

    if ( query == NULL || interfaceDesc == NULL  )
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
              "_USB_HOST_FindInterface : Arguments invalid \r\n");
        return USB_ERROR_HOST_ARGUMENTS_INVALID;
    }
    /* Get device information */
    deviceInfo = _USB_HOST_GetDeviceInfo( query->id );

    if ( NULL == deviceInfo )
    {
         SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
              "_USB_HOST_FindInterface : Invalid device address  \r\n");
         return USB_ERROR_HOST_DEVICE_INSTANCE_INVALID;
    }

    /* copy the configuration info into local pointer */
    buffer  =  deviceInfo->rawBuffer ;

    /* copy the size of total configuration size */
    size =  deviceInfo->configDescSize ;
    tempcfg  = ( USB_CONFIGURATION_DESCRIPTOR *) buffer ;

    /* validate the length of configuration descriptor*/
    if ( USB_HOST_CONFIG_DESCRPTR_SIZE  != tempcfg->bLength )
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
              "_USB_HOST_FindInterface : Config desc header size invalid \r\n");
        return USB_ERROR_HOST_HEADERSIZE_INVALID;

    }

    /* increment to next descriptor */
    buffer = buffer + USB_HOST_CONFIG_DESCRPTR_SIZE ;
    size = size - USB_HOST_CONFIG_DESCRPTR_SIZE ;

    while (size > 2)
    {
        /* parse header information */
        header = (USB_DESCRIPTOR_HEADER *) buffer;
        /* validate the descriptor size */
        if ( header->size > size )
        {
             SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
              "_USB_HOST_FindInterface : Config desc header size invalid \r\n");
             return ( USB_ERROR_HOST_HEADERSIZE_INVALID );
        }

        if( USB_DESCRIPTOR_INTERFACE  == header->descType )
        {
            interfaceDescriptor = (USB_INTERFACE_DESCRIPTOR * )buffer ;
            /* interface checking */
            if ( query->flags == USB_HOST_MATCH_CLASS_SUBCLASS_PROTOCOL )
            {
                if ((query->class == interfaceDescriptor->bInterfaceClass)
                    && (query->subClass == interfaceDescriptor->bInterfaceSubClass)
                    && ( query->protocol == interfaceDescriptor->bInterfaceProtocol ) )
                {
                    /* This condition useful for multiple OAD */
                    if ( interfaceDescriptor->bInterfaceNumber >=
                                                        query->interfaceNumber )
                    {
                        /* check alternate setting number should be same */
                        if(  interfaceDescriptor->bAlternateSetting ==
                                                query->alternateSettingNumber)
                        {
                            /* Find the interface */
                            *interfaceDesc = interfaceDescriptor;
                            return  USB_ERROR_NONE;
                         }
                     }
                }
            }
        }
        size = size - header->size ;
        buffer = buffer + header->size ;
    }

     SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
              "_USB_HOST_FindInterface : interface not found  \r\n");
     status = USB_ERROR_HOST_INTERFACE_NOT_FOUND ;
    return ( status );
}


// ******************************************************************************
/* Function:
    void _USB_HOST_EventHandler
    (
        void* referenceHandle,
        DRV_USB_EVENT eventType,
        DRV_USB_EVENT_DATA *eventData
    )

  Summary:
    Handles the events originating from USB Host Controller Driver

  Description:
    This routine is registered into the USB Host controller driver as a callback
    function. The USB Host controller driver calls this function in case of
    events from the USB controller driver.

  Parameters:
    referenceHandle           - USB Host Instance
    eventType                 - Event type
    eventData                 - Data associated with the event

  Returns:
    none.

  Remarks:
    This is a local function and should not be called directly by the application.
*/

void _USB_HOST_EventHandler
(
    uintptr_t referenceHandle,
    DRV_USB_EVENT eventType,
    void * eventData
)
{
    USB_HOST_DEVICE_INFO *deviceInfo;
    switch (eventType)
    {
        case DRV_USB_EVENT_SOF_DETECT:
            break;
        case DRV_USB_EVENT_IDLE_DETECT:
            break;
        case DRV_USB_EVENT_RESUME_DETECT:
            break;
        case DRV_USB_EVENT_ERROR:
            deviceInfo = &(gUSBHostObj[referenceHandle].attachedDevicesList[0]);
            // Reset the bus for crc errors ( physical errors )
            DRV_USB_HOST_BusResetControl ( deviceInfo-> hostControllerHandle, true);

            break;
        case DRV_USB_EVENT_HOST_DETACH:

            deviceInfo = &(gUSBHostObj[referenceHandle].attachedDevicesList[0]);
            /* Parent Device ID is root hub. Disable the SOF */
           DRV_USB_HOST_Suspend ( deviceInfo-> hostControllerHandle );
            /* Get device information */
            deviceInfo = &(gUSBHostObj[referenceHandle].attachedDevicesList[0]);

            _USB_HOST_DeEnumerate_Device( deviceInfo->deviceId );
            /* Free the device object */
          

            deviceInfo->deviceState =  USB_HOST_DEVICE_STATE_DETACHED ;

            break;

        case DRV_USB_EVENT_HOST_ATTACH:

            /* Parent Device ID is root hub. Device object */
            /* This is first device directly connected to the Host controller */
            deviceInfo = &(gUSBHostObj[referenceHandle].attachedDevicesList[0]);

            /* Set device state for enumeration */
            deviceInfo->deviceState =  USB_HOST_DEVICE_STATE_ATTACHED |
                                       USB_HOST_DEVICE_SUBSTATE_SETTLE |
                           USB_HOST_DEVICE_SUB_SUBSTATE_START_SETTLING_DELAY ;

            /* Parent ID is root hub */
            deviceInfo->parentId =  ( referenceHandle << 8 | USB_HOST_ROOT_HUB );

            /* Mark the device object is assigned */
            deviceInfo->inUse = true ;

            /* Save host controller handle */
            deviceInfo->hostControllerHandle = gUSBHostObj[referenceHandle].hostControllerHandle;

            /* At a time only one device has to enumerate so mark this */
            gUSBHostObj[referenceHandle].enumerationPending = true;

            deviceInfo->activeConfigurationNumber = 0;

            break;

        default:
            break;
    }
}

// *****************************************************************************
/* Function:
    USB_HOST_PIPE_HANDLE  _USB_HOST_PipeSetup
    (
        USB_HOST_DEVICE_ID id ,
        USB_ENDPOINT_DESCRIPTOR *endPointDesc,
        USB_SPEED speed
    )

  Summary:
    Create pipe for transfer the data

  Description:
    Function controller can setup the pipe for transfer data

  Parameters:
    USB_HOST_DEVICE_ID                id           //Hc id + Device id
    USB_ENDPOINT_DESCRIPTOR          *endPointDesc // Endpoint desc information
    USB_SPEED                         speed        // Speed of the device

  Returns:
    USB_HOST_PIPE_HANDLE

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_HOST_PIPE_HANDLE  _USB_HOST_PipeSetup
(
    USB_HOST_DEVICE_ID id ,
    USB_ENDPOINT_DESCRIPTOR *endPointDesc,
    USB_SPEED speed 
)
{
    USB_HOST_PIPE_HANDLE pipeHandle;
    USB_ENDPOINT endpointAndDirection;
    USB_HOST_DEVICE_INFO *deviceInfo;
    USB_HOST_OBJ *hcObj;
    uint8_t  deviceAddress;
    uint8_t  transferType;
    uint8_t  interval;
    uint16_t maxPacketSize;
    uint8_t  hostControllerId;
    uint8_t  hubAdress;
    uint8_t  hubPort;
    uint8_t  parentAddress;

   

    /*Host controller index */
    hostControllerId = (( id >> 8 ) & 0xFF ) ;

    /* Device Address */
    deviceAddress    = ( id & 0xFF );

    /* Address mapping */
    hcObj =  &( gUSBHostObj[ hostControllerId ] );

    /* Device information */
    deviceInfo = _USB_HOST_GetDeviceInfo( id );

    /* Validate the deviceInfo pointer */
    if ( deviceInfo == NULL )
    {
        return USB_HOST_PIPE_HANDLE_INVALID ;
    }
    /* Validate the parent device is hub or not */
    parentAddress =  ( ( deviceInfo->parentId >> 8 ) & 0xFF );

    if ( parentAddress ==  USB_HOST_ROOT_HUB )
    {
        hubAdress = 0x00;
        hubPort   = 0x00;
    }
    else
    {
        hubAdress = parentAddress;
        hubPort   = deviceInfo->portNumber ;
    }

    /* Transfer type */
    transferType = endPointDesc->transferType;

     if ( transferType == USB_TRANSFER_TYPE_CONTROL )
    {
        /* Pipe handle is already available no need to create */
        pipeHandle = deviceInfo->controlPipeHandle;
        return pipeHandle;
    }

    /* Interval for periodic transfers */
    if ( transferType ==  USB_TRANSFER_TYPE_BULK )
    {
        /* Bulk endpoints we ignore binterval value
         * reported by endpoint descriptor */
        interval = 0x00;
    }
    else
    {
        interval = endPointDesc->bInterval;
    }

    /* Endpoint maxpacket size */
    maxPacketSize = endPointDesc->wMaxPacketSize;

    /* Endpoint address and direction */
    endpointAndDirection = USB_ENDPOINT_AND_DIRECTION( endPointDesc->dirn, endPointDesc->bEndpointAddress );

       /* New pipe created */
    pipeHandle = DRV_USB_HOST_PipeSetup( deviceInfo->hostControllerHandle,
                deviceAddress, endpointAndDirection,hubAdress, hubPort,transferType, interval,
                maxPacketSize, speed );
    

    /* Return pipe handle to the host function driver */
    return  pipeHandle;
}



// *************************************************************************/
/* Function:
    void _USB_HOST_UpdateDeviceState( USB_HOST_OBJ *hcObj )

  Summary:
    Enumerates newly connected device.

  Description:
    Host state machine for enumeration

  Parameters:
    USB_HOST_OBJ             -  pointer to the host object

  Returns:
    None

  Remarks
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_UpdateDeviceState( USB_HOST_OBJ *hcObj )
{
    USB_ENDPOINT endpointAndDirection;
    uint8_t      newAddress;
    uint16_t      configDescLength;
    bool      driverFoundStatus;
    USB_HOST_DEVICE_INFO *enumerateDeviceInfo;
    uint8_t      deviceCount;
    USB_CONFIGURATION_DESCRIPTOR *configDesc;
     USB_ERROR status;



     for ( deviceCount = 0; deviceCount < USB_HOST_MAX_DEVICES_NUMBER; deviceCount ++)
     {

            enumerateDeviceInfo = &( hcObj->attachedDevicesList[ deviceCount ] );

            if( enumerateDeviceInfo->inUse == false)
                continue ;

     
   

    /* Device state */
    switch (   enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_STATE_MASK )
    {
        case USB_HOST_DEVICE_STATE_DETACHED:
     
            /* De enumerate the device */
         //   _USB_HOST_DeEnumerate_Device(enumerateDeviceInfo->deviceId );
            /* Free the device object */
        //    enumerateDeviceInfo->inUse =  false;

            break;

        case USB_HOST_DEVICE_STATE_ATTACHED:

            /* A device is attached. Update the enumeration state of the device
             * */

            switch ( enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSTATE_MASK )
            {
                /* In this state, the host layer provides 100 msec power state
                 * settling delay. This allows the attached device to charge any
                 * on board capacitances and get ready for the reset */

                case USB_HOST_DEVICE_SUBSTATE_SETTLE:
                 
                    switch ( enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSUBSTATE_MASK )
                    {
                        case USB_HOST_DEVICE_SUB_SUBSTATE_START_SETTLING_DELAY:

                            enumerateDeviceInfo->deviceState =
                                USB_HOST_DEVICE_STATE_ATTACHED |
                                USB_HOST_DEVICE_SUBSTATE_SETTLE |
                                USB_HOST_DEVICE_SUB_SUBSTATE_SETTLING_DONE;
                 
                            /* Wait 100ms for the insertion process to
                               complete and power at the device to be stable.*/
                            
                           hcObj->sysTmrHandle = SYS_TMR_DelayMS(100);

                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_SETTLING_DONE:

                            if(SYS_TMR_DelayStatusGet(hcObj->sysTmrHandle))
                            {
                                /* 100msec power state settling period is
                                 * complete. We can now reset the device */

                                enumerateDeviceInfo->deviceState =
                                    ( USB_HOST_DEVICE_STATE_ATTACHED |
                                      USB_HOST_DEVICE_SUBSTATE_RESET_DEVICE |
                                      USB_HOST_DEVICE_SUB_SUBSTATE_SET_RESET ) ;
                            }
                            break;

                        default :
                            break;
                        }
                        break;

                case USB_HOST_DEVICE_SUBSTATE_RESET_DEVICE:
                    
                    /* In this state, the host layer resets the device */

                    switch ( enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSUBSTATE_MASK )
                    {
                        case USB_HOST_DEVICE_SUB_SUBSTATE_SET_RESET :

                            /* In this state, the host layer starts the reset
                             * signalling */

                            enumerateDeviceInfo->deviceState =
                                ( USB_HOST_DEVICE_STATE_ATTACHED |
                                  USB_HOST_DEVICE_SUBSTATE_RESET_DEVICE |
                                  USB_HOST_DEVICE_SUB_SUBSTATE_RESET_WAIT ) ;

                            if ( ( enumerateDeviceInfo->parentId & 0xFF) == USB_HOST_ROOT_HUB)
                            {
                                /* If the device is connected directly to the
                                 * root hub, then we call the HCD bus reset 
                                 * control function directly. */
                                DRV_USB_HOST_BusResetControl ( enumerateDeviceInfo-> hostControllerHandle, true);
                                hcObj->sysTmrHandle = SYS_TMR_DelayMS(50);
                            }
                           
                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_RESET_WAIT:

                            if(SYS_TMR_DelayStatusGet(hcObj->sysTmrHandle))
                            {
                                enumerateDeviceInfo->deviceState =
                                    ( USB_HOST_DEVICE_STATE_ATTACHED |
                                      USB_HOST_DEVICE_SUBSTATE_RESET_DEVICE |
                                      USB_HOST_DEVICE_SUB_SUBSTATE_RESET_COMPLETE );

                                if ( (enumerateDeviceInfo->parentId & 0xFF ) == USB_HOST_ROOT_HUB )
                                {
                                    /* Stop the reset signalling */
                                    DRV_USB_HOST_BusResetControl ( enumerateDeviceInfo-> hostControllerHandle , false);
                                    /* Enable SOF generation */
                                    DRV_USB_HOST_Resume ( enumerateDeviceInfo-> hostControllerHandle );

                                   /* Provide an additional 100 millisecond delay
                                    * after the reset signalling is complete */
                                     hcObj->sysTmrHandle = SYS_TMR_DelayMS(100);
                                }
                              
                            }

                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_RESET_COMPLETE:

                            /* Here the host layer waits for the post reset 100
                             * millisecond delay */

                            if(SYS_TMR_DelayStatusGet( hcObj->sysTmrHandle ))
                            {
                                enumerateDeviceInfo->deviceState =
                                    ( USB_HOST_DEVICE_STATE_ATTACHED |
                                      USB_HOST_DEVICE_SUBSTATE_GET_DEVICE_DESCRIPTOR_SIZE |
                                      USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_DEVICE_DESCRIPTOR_SIZE );
                            }
                            break;

                        default :
                            break;
                           }
                           break;

                case USB_HOST_DEVICE_SUBSTATE_GET_DEVICE_DESCRIPTOR_SIZE:

                    /* In this state, the host layer reads 8 bytes of the device
                     * descriptor to get the device descriptor and endpoint size
                     * */
                    switch ( enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSUBSTATE_MASK )
                    {
                        case USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_DEVICE_DESCRIPTOR_SIZE:

                            /* Get the speed of the attached device */   
                            enumerateDeviceInfo->speed = DRV_USB_HOST_DeviceCurrentSpeedGet( enumerateDeviceInfo->hostControllerHandle );

                            endpointAndDirection = USB_ENDPOINT_AND_DIRECTION( USB_DATA_DIRECTION_DEVICE_TO_HOST, 0x00);

                            hcObj->defaultPipeHandle =   DRV_USB_HOST_PipeSetup( hcObj->hostControllerHandle,
                                    /*TEMP_DEVICE_ID*/ 0 ,endpointAndDirection , 0, 0, USB_TRANSFER_TYPE_CONTROL, 0,
                                    8, enumerateDeviceInfo->speed );

                            /* Fill setup packet */

                            _USB_HOST_FillSetupPacket(
                               &(enumerateDeviceInfo->setupPacket),
                               ( USB_SETUP_DIRN_DEVICE_TO_HOST |
                                 USB_SETUP_TYPE_STANDARD |
                                 USB_SETUP_RECIPIENT_DEVICE ),
                                 USB_REQUEST_GET_DESCRIPTOR,
                                 ( USB_DESCRIPTOR_DEVICE << 8 ), 0 , 8 ) ;

                             
                            /* Fill control IRP. */
                            enumerateDeviceInfo->controlIrp.data = (void *) &( enumerateDeviceInfo->deviceDescriptor );
                            enumerateDeviceInfo->controlIrp.setup = & (enumerateDeviceInfo->setupPacket ) ;
                            enumerateDeviceInfo->controlIrp.size = 8 ;
                            enumerateDeviceInfo->controlIrp.callback = (void *) _USB_HOST_setupPacketCallback;

                            enumerateDeviceInfo->deviceState =
                                USB_HOST_DEVICE_STATE_ATTACHED |
                                USB_HOST_DEVICE_SUBSTATE_GET_DEVICE_DESCRIPTOR_SIZE |
                                USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_GET_DEVICE_DESCRIPTOR_SIZE ;

                            /* Submit the IRP */
                            DRV_USB_HOST_IRPSubmit( hcObj->defaultPipeHandle, &( enumerateDeviceInfo->controlIrp ));

                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_GET_DEVICE_DESCRIPTOR_SIZE:

                            /* The host layer waits for the IRP to complete */
                            if ( enumerateDeviceInfo->controlIrp.status == USB_HOST_IRP_STATUS_COMPLETED )
                            {
                                //Just to know EP0 size
                                enumerateDeviceInfo->deviceState =
                                    USB_HOST_DEVICE_STATE_ADDRESSING |
                                    USB_HOST_DEVICE_SUBSTATE_SET_DEVICE_ADDRESS |
                                    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_SET_DEVICE_ADDRESS ;
                            }
                            else if ( enumerateDeviceInfo->controlIrp.status < 0)
                            {
                                 if (enumerateDeviceInfo-> enumerationFailCount < 3)
                                 {
                                    /* If we could not get the device descriptor
                                     * in 3 attempts, we fail this device. */
                                    enumerateDeviceInfo->enumerationFailCount ++ ;

                                    /* Set device state for enumeration */
                                    enumerateDeviceInfo->deviceState =  USB_HOST_DEVICE_STATE_ATTACHED |
                                                    USB_HOST_DEVICE_SUBSTATE_SETTLE |
                                                USB_HOST_DEVICE_SUB_SUBSTATE_START_SETTLING_DELAY ;

                                 }

                                    /* Check if we have tried this 3 times
                                     * already */
                                 else
                                    {
                                         DRV_USB_HOST_PipeClose( hcObj->defaultPipeHandle );
                                        

                                        enumerateDeviceInfo->inUse =  false;
                                        
                                        /* 
                                         * We should send an event to the
                                         * application and then wait for device
                                         * attach */
                                         

                                         ((USB_HOST_INSTANCE_CLIENT *)   hcObj->appHandle)->callBackFunc( USB_HOST_EVENT_CANNOT_ENUMERATE , enumerateDeviceInfo , hcObj->context);

                                    }
                               }
                            
                            break;

                        default :
                            break;

                       }
                       break;
                default: 
                    break;
            }
            break;

        case USB_HOST_DEVICE_STATE_ADDRESSING:

            /* In this state, the Host sets the device address */

            switch ( enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSTATE_MASK )
            {
                case USB_HOST_DEVICE_SUBSTATE_SET_DEVICE_ADDRESS :

                    /* In this state, the host sends the Set Address commmand */

                    switch ( enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSUBSTATE_MASK )
                    {
                        case USB_HOST_DEVICE_SUB_SUBSTATE_SEND_SET_DEVICE_ADDRESS :

                            /* Allocate new address for the device */
                            newAddress = _USB_HOST_GetNewAddress( enumerateDeviceInfo->deviceId );
                            enumerateDeviceInfo->deviceId = ( enumerateDeviceInfo->deviceId | newAddress );

                             _USB_HOST_FillSetupPacket(
                               &(enumerateDeviceInfo->setupPacket),
                               ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                                 USB_SETUP_TYPE_STANDARD |
                                 USB_SETUP_RECIPIENT_DEVICE ),
                                 USB_REQUEST_SET_ADDRESS ,
                                 newAddress , 0 , 0 ) ;

                        
                            /* Fill IRP */
                            enumerateDeviceInfo->controlIrp.data = (void *) ( enumerateDeviceInfo->buffer );
                            enumerateDeviceInfo->controlIrp.setup = &( enumerateDeviceInfo->setupPacket ) ;
                            enumerateDeviceInfo->controlIrp.size = 0 ;
                            enumerateDeviceInfo->controlIrp.callback = (void *) _USB_HOST_setupPacketCallback;
                           
                            /* Set the next host layer state */ 
                            enumerateDeviceInfo->deviceState =
                                USB_HOST_DEVICE_STATE_ADDRESSING |
                                USB_HOST_DEVICE_SUBSTATE_SET_DEVICE_ADDRESS |
                                USB_HOST_DEVICE_SUB_SUBSTATE_SET_DEVICE_ADDRESS_COMPLETE;

                          
                            /* Send the control request */
                            DRV_USB_HOST_IRPSubmit( hcObj->defaultPipeHandle, &enumerateDeviceInfo->controlIrp );

                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_SET_DEVICE_ADDRESS_COMPLETE :
                            
                            /* Check if the Set Address Request completed */

                            if ( enumerateDeviceInfo->controlIrp.status == USB_HOST_IRP_STATUS_COMPLETED )
                            {
                            
                                /* Open control pipe with new address */
                                endpointAndDirection = USB_ENDPOINT_AND_DIRECTION( USB_DATA_DIRECTION_DEVICE_TO_HOST, 0x00 );

                                /* Now open a control transfer pipe with the
                                 * actual pipe size */
                                
                                enumerateDeviceInfo->controlPipeHandle = DRV_USB_HOST_PipeSetup( enumerateDeviceInfo->hostControllerHandle,
                                        (enumerateDeviceInfo->deviceId & 0xFF ), endpointAndDirection /* Endpoint */, 0, 0,
                                        USB_TRANSFER_TYPE_CONTROL/* Pipe type */, 0, /* bInterval */
                                        enumerateDeviceInfo->deviceDescriptor.bMaxPacketSize0, enumerateDeviceInfo->speed );

                                if( DRV_USB_HOST_PIPE_HANDLE_INVALID == enumerateDeviceInfo->controlPipeHandle )
                                {
                                    //Close the default pipe handle
                                     DRV_USB_HOST_PipeClose ( enumerateDeviceInfo->defaultPipeHandle );
                                    /* Release address */
                                    _USB_HOST_FreeAddress ( enumerateDeviceInfo->deviceId );

                                    enumerateDeviceInfo->inUse = false;
                                    //send error event to the application
                                    ((USB_HOST_INSTANCE_CLIENT *)   hcObj->appHandle)->callBackFunc(
                                        USB_HOST_EVENT_CANNOT_ENUMERATE , enumerateDeviceInfo ,hcObj->context);
                                }
                                else
                                {
                                enumerateDeviceInfo->deviceState =
                                    USB_HOST_DEVICE_STATE_CONFIGURING |
                                    USB_HOST_DEVICE_SUBSTATE_GET_DEVICE_DESCRIPTOR |
                                    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_DEVICE_DESCRIPTOR ;
                                }
                            }
                            else
                            {
                                if ( enumerateDeviceInfo->controlIrp.status < 0)
                                {
                                    if (enumerateDeviceInfo-> enumerationFailCount < 3)
                                 {
                                    /* If we could not get the device descriptor
                                     * in 3 attempts, we fail this device. */
                                    enumerateDeviceInfo->enumerationFailCount ++ ;

                                    /* Set device state for enumeration */
                                    enumerateDeviceInfo->deviceState =  USB_HOST_DEVICE_STATE_ATTACHED |
                                                    USB_HOST_DEVICE_SUBSTATE_SETTLE |
                                                USB_HOST_DEVICE_SUB_SUBSTATE_START_SETTLING_DELAY ;

                                 }

                                    /* Check if we have tried this 3 times
                                     * already */
                                 else
                                    {
                                         DRV_USB_HOST_PipeClose( hcObj->defaultPipeHandle );
                                        DRV_USB_HOST_PipeClose( enumerateDeviceInfo->controlPipeHandle );

                                        enumerateDeviceInfo->inUse =  false;

                                        /*
                                         * We should send an event to the
                                         * application and then wait for device
                                         * attach */

                                         ((USB_HOST_INSTANCE_CLIENT *)   hcObj->appHandle)->callBackFunc(
                                        USB_HOST_EVENT_CANNOT_ENUMERATE , enumerateDeviceInfo , hcObj->context);

                                    }
                                }
                            }

                            break;
                        default :
                            break;

                           
                    }

                    break;
            }
            break;

        case USB_HOST_DEVICE_STATE_CONFIGURING:

            /* In this state the host layer configures the device */

            switch ( enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSTATE_MASK )
            {
                /* In this state, the host layer gets the device descriptor */

                case USB_HOST_DEVICE_SUBSTATE_GET_DEVICE_DESCRIPTOR :

                    switch (enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSUBSTATE_MASK )
                    {
                        case USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_DEVICE_DESCRIPTOR :

                            /*Fill setup packet */

                            _USB_HOST_FillSetupPacket(
                               &(enumerateDeviceInfo->setupPacket),
                               ( USB_SETUP_DIRN_DEVICE_TO_HOST |
                                 USB_SETUP_TYPE_STANDARD |
                                 USB_SETUP_RECIPIENT_DEVICE ),
                                 USB_REQUEST_GET_DESCRIPTOR,
                                 ( USB_DESCRIPTOR_DEVICE << 8 ), 0 ,
                               enumerateDeviceInfo->deviceDescriptor.bLength ) ;

                            
                            /* Fill IRP */
                            enumerateDeviceInfo->controlIrp.data = (void *) &( enumerateDeviceInfo->deviceDescriptor );
                            enumerateDeviceInfo->controlIrp.setup = &(enumerateDeviceInfo->setupPacket);
                            enumerateDeviceInfo->controlIrp.size = enumerateDeviceInfo->deviceDescriptor.bLength;
                            enumerateDeviceInfo->controlIrp.callback = (USB_HOST_IRP_CALLBACK) _USB_HOST_setupPacketCallback;

                            enumerateDeviceInfo->deviceState =
                                ( USB_HOST_DEVICE_STATE_CONFIGURING |
                                  USB_HOST_DEVICE_SUBSTATE_GET_DEVICE_DESCRIPTOR |
                                  USB_HOST_DEVICE_SUB_SUBSTATE_GET_DEVICE_DESCRIPTOR_COMPLETE );

                            /* Submit IRP */
                            DRV_USB_HOST_IRPSubmit( enumerateDeviceInfo->controlPipeHandle, &(enumerateDeviceInfo->controlIrp ));

                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_GET_DEVICE_DESCRIPTOR_COMPLETE:

                            /* If the IRP completes, we go to the next state */
                            if (enumerateDeviceInfo->controlIrp.status == USB_HOST_IRP_STATUS_COMPLETED)
                            {
                                enumerateDeviceInfo->deviceState =
                                    ( USB_HOST_DEVICE_STATE_CONFIGURING |
                                      USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR |
                                      USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_CONFIG_DESCRIPTOR_SIZE );
                            }
                            else
                            {
                                if ( enumerateDeviceInfo->controlIrp.status < 0)
                                {
                                     if (enumerateDeviceInfo-> enumerationFailCount < 3)
                                 {
                                    /* If we could not get the device descriptor
                                     * in 3 attempts, we fail this device. */
                                    enumerateDeviceInfo->enumerationFailCount ++ ;

                                    /* Set device state for enumeration */
                                    enumerateDeviceInfo->deviceState =  USB_HOST_DEVICE_STATE_ATTACHED |
                                                    USB_HOST_DEVICE_SUBSTATE_SETTLE |
                                                USB_HOST_DEVICE_SUB_SUBSTATE_START_SETTLING_DELAY ;

                                 }

                                    /* Check if we have tried this 3 times
                                     * already */
                                 else
                                    {
                                        DRV_USB_HOST_PipeClose( hcObj->defaultPipeHandle );
                                        DRV_USB_HOST_PipeClose( enumerateDeviceInfo->controlPipeHandle );

                                        enumerateDeviceInfo->inUse =  false;

                                        /*
                                         * We should send an event to the
                                         * application and then wait for device
                                         * attach */

                                         ((USB_HOST_INSTANCE_CLIENT *)   hcObj->appHandle)->callBackFunc(
                                        USB_HOST_EVENT_CANNOT_ENUMERATE , enumerateDeviceInfo , hcObj->context);

                                    }
                                }
                            }
                            break;

                        default :
                            break;

                            
                    }

                    break;

                case USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR :

                    switch ( enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSUBSTATE_MASK )
                    {
                        /* In this state the host layer gets the configuration
                         * descriptor header */
                        case USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_CONFIG_DESCRIPTOR_SIZE:


                            _USB_HOST_FillSetupPacket(
                               &(enumerateDeviceInfo->setupPacket),
                               ( USB_SETUP_DIRN_DEVICE_TO_HOST |
                                 USB_SETUP_TYPE_STANDARD |
                                 USB_SETUP_RECIPIENT_DEVICE ),
                                 USB_REQUEST_GET_DESCRIPTOR,
                                 ( USB_DESCRIPTOR_CONFIGURATION << 8 )+ enumerateDeviceInfo->activeConfigurationNumber , 0 , 9 ) ;

                              /* Fill IRP */

                            enumerateDeviceInfo->controlIrp.data = enumerateDeviceInfo->buffer;
                            enumerateDeviceInfo->controlIrp.setup = &(enumerateDeviceInfo->setupPacket);
                            enumerateDeviceInfo->controlIrp.size = 9;
                            enumerateDeviceInfo->controlIrp.callback = (USB_HOST_IRP_CALLBACK) _USB_HOST_setupPacketCallback;

                            enumerateDeviceInfo->deviceState =
                                USB_HOST_DEVICE_STATE_CONFIGURING |
                                USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR |
                                USB_HOST_DEVICE_SUB_SUBSTATE_GET_CONFIG_DESCRIPTOR_SIZE_COMPLETE;

                            DRV_USB_HOST_IRPSubmit( enumerateDeviceInfo->controlPipeHandle, &( enumerateDeviceInfo->controlIrp ));

                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_GET_CONFIG_DESCRIPTOR_SIZE_COMPLETE:

                            if ( enumerateDeviceInfo->controlIrp.status == USB_HOST_IRP_STATUS_COMPLETED )
                            {

                                enumerateDeviceInfo->deviceState =
                                    USB_HOST_DEVICE_STATE_CONFIGURING |
                                    USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR |
                                    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_CONFIG_DESCRIPTOR;

                            }
                            else
                            {
                                if ( enumerateDeviceInfo->controlIrp.status < 0)
                                {
                                     if (enumerateDeviceInfo-> enumerationFailCount < 3)
                                 {
                                    /* If we could not get the device descriptor
                                     * in 3 attempts, we fail this device. */
                                    enumerateDeviceInfo->enumerationFailCount ++ ;

                                    /* Set device state for enumeration */
                                    enumerateDeviceInfo->deviceState =  USB_HOST_DEVICE_STATE_ATTACHED |
                                                    USB_HOST_DEVICE_SUBSTATE_SETTLE |
                                                USB_HOST_DEVICE_SUB_SUBSTATE_START_SETTLING_DELAY ;

                                 }

                                    /* Check if we have tried this 3 times
                                     * already */
                                 else
                                    {

                                        DRV_USB_HOST_PipeClose( hcObj->defaultPipeHandle );
                                        DRV_USB_HOST_PipeClose( enumerateDeviceInfo->controlPipeHandle );

                                        enumerateDeviceInfo->inUse =  false;

                                        /*
                                         * We should send an event to the
                                         * application and then wait for device
                                         * attach */

                                         ((USB_HOST_INSTANCE_CLIENT *)   hcObj->appHandle)->callBackFunc(
                                        USB_HOST_EVENT_CANNOT_ENUMERATE , enumerateDeviceInfo , hcObj->context);

                                    }
                                  
                                }
                            }
                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_CONFIG_DESCRIPTOR:

                              configDesc  =  ( USB_CONFIGURATION_DESCRIPTOR *)enumerateDeviceInfo->buffer;
                              configDescLength = configDesc->wTotalLength ;
                            
                            /* Fill setup packet */

                             _USB_HOST_FillSetupPacket(
                               &(enumerateDeviceInfo->setupPacket),
                               ( USB_SETUP_DIRN_DEVICE_TO_HOST |
                                 USB_SETUP_TYPE_STANDARD |
                                 USB_SETUP_RECIPIENT_DEVICE ),
                                 USB_REQUEST_GET_DESCRIPTOR,
                                 ( USB_DESCRIPTOR_CONFIGURATION << 8 )+ enumerateDeviceInfo->activeConfigurationNumber , 0 ,configDesc->wTotalLength) ;

                            /* Dynamically allocate buffer  */
                            enumerateDeviceInfo->rawBuffer = USB_HOST_MALLOC ( configDesc->wTotalLength );
                            enumerateDeviceInfo->controlIrp.data = enumerateDeviceInfo->rawBuffer;
                            enumerateDeviceInfo->configDescSize = configDesc->wTotalLength ;

                            enumerateDeviceInfo->controlIrp.setup = &(enumerateDeviceInfo->setupPacket);
                            enumerateDeviceInfo->controlIrp.size = configDesc->wTotalLength;
                            enumerateDeviceInfo->controlIrp.callback = (USB_HOST_IRP_CALLBACK) _USB_HOST_setupPacketCallback;
                            enumerateDeviceInfo->deviceState =
                                USB_HOST_DEVICE_STATE_CONFIGURING |
                                USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR |
                                USB_HOST_DEVICE_SUB_SUBSTATE_GET_CONFIG_DESCRIPTOR_COMPLETE;

                            DRV_USB_HOST_IRPSubmit( enumerateDeviceInfo->controlPipeHandle, &( enumerateDeviceInfo->controlIrp ));
                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_GET_CONFIG_DESCRIPTOR_COMPLETE:

                            if ( enumerateDeviceInfo->controlIrp.status == USB_HOST_IRP_STATUS_COMPLETED )
                            {
                                configDescLength = enumerateDeviceInfo->controlIrp.size;

                                status = _USB_HOST_ParseConfigDescriptor( enumerateDeviceInfo->rawBuffer,
                                        &(enumerateDeviceInfo->configDescriptorInfo),
                                        configDescLength );
                                if( status != USB_ERROR_NONE )
                                {
                                    //callback
                                    ((USB_HOST_INSTANCE_CLIENT *)   hcObj->appHandle)->callBackFunc(
                                        USB_HOST_EVENT_CANNOT_ENUMERATE , enumerateDeviceInfo ,hcObj->context);
                                }

                                /* Verify VID/PID in TPL table */
                                driverFoundStatus = _USB_HOST_FindVendorDriver( enumerateDeviceInfo );

                                /* Driver found */
                                if( driverFoundStatus == true )
                                {
                                    /*Initialize VID/PID driver */
                                    enumerateDeviceInfo->driver->initializeDeviceDriver (
                                            enumerateDeviceInfo->deviceId,
                                            & (enumerateDeviceInfo->instanceNumber ),
                                            enumerateDeviceInfo->speed );
                                    
                                    /* Wait the state to vedor driver confirm the selection of configuration */
                                    enumerateDeviceInfo->deviceState =
                                        USB_HOST_DEVICE_STATE_CONFIGURING |
                                        USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR|
                                        USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_CONFIG_DESCRIPTOR_SELECT ;
                                }
                                /* VID/PID is not found */
                                else
                                {
                                    /* Device level C/SC/P in device ? */

                                    /*Match an entry in TPL */
                                    driverFoundStatus = _USB_HOST_FindClassDriver( enumerateDeviceInfo );
                                    
                                    /* Driver found*/
                                    if( driverFoundStatus == true )
                                    {
                                        /* Initialize C/SC/P driver */
                                        enumerateDeviceInfo->driver->initializeDeviceDriver(
                                                    enumerateDeviceInfo->deviceId, &( enumerateDeviceInfo->instanceNumber ),
                                                    enumerateDeviceInfo->speed);

                                        enumerateDeviceInfo->deviceState =
                                            USB_HOST_DEVICE_STATE_CONFIGURING |
                                            USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR|
                                            USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_CONFIG_DESCRIPTOR_SELECT ;
                                    }
                                    else
                                    {
                                        /* Default configuration */
                                        enumerateDeviceInfo->activeConfigurationNumber = 0x01;
                                        enumerateDeviceInfo->deviceState =
                                            ( USB_HOST_DEVICE_STATE_CONFIGURING |
                                              USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR |
                                              USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_INTERFACE_DRIVER_SELECT );
                                    }
                                }
                            }
                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_INTERFACE_DRIVER_SELECT :

                            /* configuration is selected */
                            driverFoundStatus = _USB_HOST_FindInterfaceDriver( enumerateDeviceInfo );

                            /* If driver not found and Number of configuration > 1 */
                            if ( (driverFoundStatus == false ) && ( enumerateDeviceInfo->deviceDescriptor.bNumConfigurations ) > 1 )
                            {
                                enumerateDeviceInfo->deviceState = (
                                        USB_HOST_DEVICE_STATE_CONFIGURING |
                                        USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR |
                                        USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_CONFIG_DESCRIPTOR_SIZE );
                                enumerateDeviceInfo->configurationNumber ++ ;
                                USB_HOST_FREE(enumerateDeviceInfo->rawBuffer);

                            }
                            else
                            {
                                enumerateDeviceInfo->deviceState =
                                    USB_HOST_DEVICE_STATE_CONFIGURING |
                                    USB_HOST_DEVICE_SUBSTATE_SET_CONFIGURATION |
                                    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_SET_CONFIGURATION;

                            }
                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_CONFIG_DESCRIPTOR_SELECT:
                            /* Vendor driver has to call set configuration */
                            if ( enumerateDeviceInfo->activeConfigurationNumber )
                            {
                                enumerateDeviceInfo->deviceState =
                                    USB_HOST_DEVICE_STATE_CONFIGURING |
                                    USB_HOST_DEVICE_SUBSTATE_SET_CONFIGURATION |
                                    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_SET_CONFIGURATION;
                            }
                            break;
                        default :
                            break;
                          
                    }
                    break;

                case USB_HOST_DEVICE_SUBSTATE_SET_CONFIGURATION:
                    switch ( enumerateDeviceInfo->deviceState & USB_HOST_DEVICE_SUBSUBSTATE_MASK )
                    {
                        case USB_HOST_DEVICE_SUB_SUBSTATE_SEND_SET_CONFIGURATION:

                             configDesc  =  ( USB_CONFIGURATION_DESCRIPTOR *)enumerateDeviceInfo->buffer;
                              _USB_HOST_FillSetupPacket(
                               &(enumerateDeviceInfo->setupPacket),
                               ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                                 USB_SETUP_TYPE_STANDARD |
                                 USB_SETUP_RECIPIENT_DEVICE ),
                                 USB_REQUEST_SET_CONFIGURATION,
                                 configDesc->bConfigurationValue , 0 ,0 ) ;

                            /* Fill irp */
                            enumerateDeviceInfo->controlIrp.data = NULL;
                            enumerateDeviceInfo->controlIrp.setup = &(enumerateDeviceInfo->setupPacket);
                            enumerateDeviceInfo->controlIrp.size = 0;
                            enumerateDeviceInfo->controlIrp.callback = (USB_HOST_IRP_CALLBACK) _USB_HOST_setupPacketCallback;
                            enumerateDeviceInfo->deviceState =
                                USB_HOST_DEVICE_STATE_CONFIGURING |
                                USB_HOST_DEVICE_SUBSTATE_SET_CONFIGURATION |
                                USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_SET_CONFIGURATION ;

                            DRV_USB_HOST_IRPSubmit( enumerateDeviceInfo->controlPipeHandle, &enumerateDeviceInfo->controlIrp );
                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_SET_CONFIGURATION:
                            if (enumerateDeviceInfo->controlIrp.status == USB_HOST_IRP_STATUS_COMPLETED)
                            {
                                hcObj->sysTmrHandle = SYS_TMR_DelayMS(50);

                                enumerateDeviceInfo->deviceState =
                                    USB_HOST_DEVICE_STATE_CONFIGURING |
                                    USB_HOST_DEVICE_SUBSTATE_SET_CONFIGURATION |
                                    USB_HOST_DEVICE_SUB_SUBSTATE_SET_CONFIGURATION_COMPLETE ;
                            }
                            break;

                        case USB_HOST_DEVICE_SUB_SUBSTATE_SET_CONFIGURATION_COMPLETE:

                            if (SYS_TMR_DelayStatusGet(hcObj->sysTmrHandle))
                            {
                                _USB_HOST_AttachInterfaceDriver ( enumerateDeviceInfo );
                                hcObj->attachDevicesCount++;
                                enumerateDeviceInfo->deviceState = USB_HOST_DEVICE_STATE_DEVICE_READY ;
                            }
                            break;
                    }
                    break;
            }
            break;

        case USB_HOST_DEVICE_STATE_DEVICE_READY:
          
            hcObj->enumerationPending = false ;
         
            break;

        default: 
            break;
    }

     }
}

//*****************************************************************************
/* Function:
    uint8_t _USB_HOST_GetNewAddress(USB_HOST_DEVICE_ID id )

  Summary:
    Allocate a unique 8 bit device address

  Description:
    USB Host can allocate upto 127 addrss as per standard. This function will
    look into the free address and return the free address

  Parameters:
    USB_HOST_DEVICE_ID             - Host controllerId + device ID

  Returns:
    uint8_t

  Remarks:
    This is a local function and should not be called directly by the
    application
*/

uint8_t _USB_HOST_GetNewAddress( USB_HOST_DEVICE_ID id )
{
    USB_HOST_OBJ *hcObj;
    uint8_t tempAddress;
    uint8_t hostIndex;

    /* Get hostIndex value */
    hostIndex = ( id >> 8) & 0xFF ;

    /* Address mapping */
    hcObj =  &(gUSBHostObj[hostIndex]);

    /* Find Free address */
    for ( tempAddress = 1; tempAddress <= USB_HOST_MAX_DEVICES_NUMBER ; tempAddress++ )
    {
        if ((hcObj->addressBits[ tempAddress / 8] & (1 << ( tempAddress % 8 ))) == 0)
        {
            break;
        }
    }

    /*Mark for allocated address */
    hcObj->addressBits[ tempAddress / 8] |= (1 << ( tempAddress % 8 ));
    return tempAddress;

    
}

// *****************************************************************************
/* Function:
    void _USB_HOST_FreeAddress ( USB_HOST_DEVICE_ID id );

  Summary:
    Free address

  Description:
    This function is used for free the adress to the Host controller database.

  Parameters:
   USB_HOST_DEVICE_ID             - Host controllerId + device ID

  Returns:
    void
*/

void _USB_HOST_FreeAddress (USB_HOST_DEVICE_ID id )
{
    USB_HOST_OBJ *hcObj;
    uint8_t hostIndex;
    uint8_t deviceAddress;

    /* Get hostIndex value */
    hostIndex = ( id >> 8) & 0xFF ;

    /* Device address */
    deviceAddress = ( id & 0xFF );

    /* Address mapping */
    hcObj =  &( gUSBHostObj[ hostIndex ] );
    
    /* The address is no longer being used. */
    hcObj->addressBits[ deviceAddress/ 8 ] &= ~(1 << ( deviceAddress % 8));
}

//*****************************************************************************
/* Function:
    void _USB_HOST_setupPacketCallback ( USB_HOST_IRP * irp  )

  Summary:
    _USB_HOST_setupPacketCallback for status of setup IRP.

  Description:
    USB Host controller driver will call when the setup request has been done.

  Parameters:
   USB_HOST_IRP *             - Pointer to the setupIrp

  Returns:
    void

  Remarks
    This is a local function and should not be called directly by the
    application.  
*/

void _USB_HOST_setupPacketCallback ( USB_HOST_IRP * irp )
{
    if( irp->status != USB_HOST_IRP_STATUS_COMPLETED )
    {
        if( irp->status == USB_HOST_IRP_STATUS_ERROR_NAK_TIMEOUT)
        {
            // Timing problem will resend the same packet
        }
        if( irp->status == USB_HOST_IRP_STATUS_ERROR_BUS)
        {
            // Re enumerate the device
        }
        if( irp->status == USB_HOST_IRP_STATUS_ERROR_DATA || irp->status == USB_HOST_IRP_STATUS_ERROR_BUS )
        {
            // Resend the request
        }
        if( irp->status == USB_HOST_IRP_STATUS_ERROR_STALL)
        {
            //clear the stall
        }
    }
}

// *****************************************************************************
/* Function:
    USB_ERROR _USB_HOST_ParseConfigDescriptor
    (
        uint8_t *buffer,
        USB_HOST_CONFIGURATION_INFO *configDescriptorInfo,
        uint16_t length
    );

  Summary:
    Parse raw descriptors.

  Description:
    This routine is parse the configuration descriptor that holds the
    configuration descriptor, interface descriptor and endpoint descriptors

  Parameters:
    uint8_t*                         - raw configuration descriptor
    USB_HOST_CONFIGURATION_INFO *    - configuration information
    uint8_t                          - Length of the configuration descriptor

  Returns:
    USB_ERROR                        - Status

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_ERROR _USB_HOST_ParseConfigDescriptor 
(
    uint8_t *buffer,
    USB_HOST_CONFIGURATION_INFO *configDescriptorInfo,
    uint16_t length
)
{
    USB_ERROR status;
    USB_CONFIGURATION_DESCRIPTOR *tempcfg;
    USB_INTERFACE_DESCRIPTOR *interfaceDescriptor;
    USB_ENDPOINT_DESCRIPTOR *endpointDescriptor;
    USB_DESCRIPTOR_HEADER *header;
    // USB_HOST_INTERFACE_ALTERNATE_SETTING_INFO *altSetting = NULL;
    uint8_t totalInterfaces= 0;
    uint8_t totalEndpoints = 0;

    status = USB_ERROR_NONE ;

    if ( buffer == NULL || configDescriptorInfo == NULL )
    {
        return USB_ERROR_HOST_ARGUMENTS_INVALID ;
    }
    /* Typecast to raw configuration info to configuration descriptor */
    tempcfg = ( USB_CONFIGURATION_DESCRIPTOR *) buffer;
    /* validate the length of configuration descriptor*/
    if ( tempcfg->bLength != USB_HOST_CONFIG_DESCRPTR_SIZE)
    {
        return USB_ERROR_HOST_HEADERSIZE_INVALID;
    }
    /* Validate the number of interfaces */
    if ( tempcfg->bNumInterfaces > USB_HOST_MAX_INTERFACES )
    {
        return USB_ERROR_HOST_MAX_INTERFACES_INVALID;
    }

    configDescriptorInfo->configurationDescriptor = tempcfg;
    buffer = buffer + USB_HOST_CONFIG_DESCRPTR_SIZE ;
    length = length - USB_HOST_CONFIG_DESCRPTR_SIZE ;
    /* parse the configuration descriptor */
    while (length > 2)
    {
        /* parse header information */
        header = (USB_DESCRIPTOR_HEADER *) buffer;
        if ( header->size > length )
            return USB_ERROR_HOST_HEADERSIZE_INVALID;

        /* Every alt setting starts with an exactly one interface descriptor
           followed by zero or more class specific interace descriptors
           followed by zero or more endpoint descriptors .
           Each endpoint descriptor is immediateley followed by zero or
           more class specific endpoint descriptor */
        switch (header->descType)
        {
            case USB_DESCRIPTOR_INTERFACE:
                interfaceDescriptor = (USB_INTERFACE_DESCRIPTOR *) buffer;
                /* valid interface place into configuration desc */

                configDescriptorInfo->interfaceInfo
                    [interfaceDescriptor->bInterfaceNumber].interfaceDesc
                    = interfaceDescriptor ;

                length -= header->size;
                buffer += header->size;
                if (interfaceDescriptor->bAlternateSetting == 0)
                    totalInterfaces ++ ;
                if (totalInterfaces > tempcfg->bNumInterfaces )
                    return USB_ERROR_HOST_MAX_INTERFACES_INVALID; // invalid desc
                /* standard interface desc */

                totalEndpoints = 0;
                break;
            case USB_DESCRIPTOR_ENDPOINT:

                endpointDescriptor = (USB_ENDPOINT_DESCRIPTOR *) buffer;
                /* End point zero never have descriptor */
                if (( endpointDescriptor->bEndpointAddress & 0xf) == 0)
                    return USB_ERROR_HOST_ENDPOINT_DESC_SIZE_INVALID; 
                if ( totalEndpoints >= USB_HOST_ENDPOINT_NUMBER )
                    // Invalid descriptor
                    return USB_ERROR_HOST_MAX_ENDPOINT_INVALID; 
                /* valid endpoint place into cfg desc */
                length -= header->size;
                buffer += header->size;
                totalEndpoints++;
                break;
            case USB_DESCRIPTOR_DEVICE:
            case USB_DESCRIPTOR_CONFIGURATION:
            case USB_DESCRIPTOR_STRING:
                // invalid descriptor
                return USB_ERROR_HOST_DESCRIPTOR_INVALID; 
                break;

            default:
                length -= header->size;
                buffer += header->size;
                break;
        }
    }
    return status;
}

//*****************************************************************************
/* Function:
    int8_t  _USB_HOST_FindVendorDriver( USB_HOST_DEVICE_INFO *newDevice )

  Summary:
    Find a maching a driver from the target peripheral list based on flag.

  Description:
    Driver has to find based on Vendorid , productid, version number in
    device descritor

  Parameters:
    USB_HOST_DEVICE_INFO *                 - newdevice information

*/

bool  _USB_HOST_FindVendorDriver( USB_HOST_DEVICE_INFO *newDevice )
{
    uint8_t count;
    USB_HOST_TARGET_PERIPHERAL_LIST *driverInfo;
    uint8_t hostIndex;
    USB_HOST_DEVICE_ID  id;
    bool  driverFound = false;
    USB_HOST_OBJ *hcObj;
    id = newDevice->deviceId;
    hostIndex = ( id >> 8 ) & 0xFF;

    /* Address mapping */
    hcObj =  &(gUSBHostObj[hostIndex]);

    /*Find VID , PID match in TPL List */
    for( count = 0 ; count < hcObj->nTPLEntries ; count++ )
    {
        driverInfo = &(gUSBHostObj[hostIndex].usbHostTpl[count]);

        if( driverInfo->tplFlag == TPL_FLAG_VID_PID  )
        {
            /* First validate vendor specific driver is available */
            if (( driverInfo->device.idVendor == newDevice->deviceDescriptor.idVendor) &&
                    ( driverInfo->device.idProduct == newDevice->deviceDescriptor.idProduct ))
            {
                newDevice->driverType = USB_HOST_VENDOR_DEVICE_DRIVER;
                newDevice->driver = driverInfo->classDriver;
                driverFound = true;
                return driverFound;
            }
        }

        if  (driverInfo->tplFlag == TPL_FLAG_IGNORE_PID)
        {
            if (driverInfo->device.idVendor == newDevice->deviceDescriptor.idVendor )
            {
                newDevice->driverType = USB_HOST_VENDOR_DEVICE_DRIVER;
                newDevice->driver = driverInfo->classDriver;
                driverFound = true;
                return driverFound;
            }
        }
    }
    return driverFound;
}

//*****************************************************************************
/* Function:
    int8_t  _USB_HOST_FindClassDriver ( USB_HOST_DEVICE_INFO *newdevice  )

  Summary:
    Find a maching a class driver from the target peripheral list based on flag.

  Description:
    Driver has to find based on Class , Subclass , Protocol number in
    device descritor

  Parameters:
    USB_HOST_DEVICE_INFO *                 - newdevice information
  Returns:
    int8_t                                 - return status
*/

bool  _USB_HOST_FindClassDriver( USB_HOST_DEVICE_INFO *newDevice )
{
    uint8_t count;
    USB_HOST_TARGET_PERIPHERAL_LIST *driverInfo;
    uint8_t hostIndex;
    USB_HOST_DEVICE_ID  id;
    USB_HOST_OBJ *hcObj;
    id = newDevice->deviceId;
    hostIndex = ( id >> 8 ) & 0xFF;

    /* Address mapping */
    hcObj =  &(gUSBHostObj[hostIndex]);

    for( count = 0 ; count < hcObj->nTPLEntries ; count++ )
    {
        driverInfo = &(gUSBHostObj[hostIndex].usbHostTpl[count]);

        if( driverInfo->tplFlag == TPL_FLAG_CLASS_SUBCLASS_PROTOCOL)
        {
            /* first validate class specific driver is available */
            if(( driverInfo->device.class == newDevice->deviceDescriptor.bDeviceClass) &&
                    ( driverInfo->device.subClass == newDevice->deviceDescriptor.bDeviceSubClass ) &&
                    ( driverInfo->device.protocol == newDevice->deviceDescriptor.bDeviceProtocol))
            {
                newDevice->driverType = USB_HOST_STANDARD_CLASS_DEVICE_DRIVER ;
                newDevice->driver = driverInfo->classDriver ;
                return true;
            }
        }

        if( driverInfo->tplFlag == TPL_FLAG_IGNORE_PROTOCOL)
        {
            if(( driverInfo->device.class == newDevice->deviceDescriptor.bDeviceClass) &&
                    ( driverInfo->device.subClass == newDevice->deviceDescriptor.bDeviceSubClass ) )
            {
                newDevice->driverType = USB_HOST_STANDARD_CLASS_DEVICE_DRIVER ;
                newDevice->driver = driverInfo->classDriver ;
                return true ;
            }
        }

        if( driverInfo->tplFlag == TPL_FLAG_IGNORE_CLASS)
        {
            /* First validate class specific driver is available */
            if(( driverInfo->device.subClass == newDevice->deviceDescriptor.bDeviceSubClass) &&
                    ( driverInfo->device.protocol == newDevice->deviceDescriptor.bDeviceProtocol ))
            {
                newDevice->driverType = USB_HOST_STANDARD_CLASS_DEVICE_DRIVER ;
                newDevice->driver = driverInfo->classDriver ;
                return true;
            }
        }

        if( driverInfo->tplFlag == TPL_FLAG_IGNORE_SUBCLASS)
        {
            /* first validate class specific driver is available */
            if(( driverInfo->device.class == newDevice->deviceDescriptor.bDeviceClass ) &&
                    ( driverInfo->device.protocol == newDevice->deviceDescriptor.bDeviceProtocol ))
            {
                newDevice->driverType = USB_HOST_STANDARD_CLASS_DEVICE_DRIVER ;
                newDevice->driver = driverInfo->classDriver ;
                return true ;
            }
        }
    }

    return false ;
}

//*****************************************************************************
/* Function:
  int8_t  _USB_HOST_FindInterfaceDriver ( USB_HOST_DEVICE_INFO *newdevice )

  Summary:
    Find a maching a class driver from the target peripheral list based on flag.

  Description:
    Driver has to find based on Class , Subclass , Protocol number in
    interface descritors

  Parameters:
    USB_HOST_DEVICE_INFO *                 - newdevice information

*/

bool  _USB_HOST_FindInterfaceDriver( USB_HOST_DEVICE_INFO *newDevice )
{
    uint8_t count;
    uint8_t hostIndex;
    uint8_t  configNumber;
    USB_HOST_DEVICE_ID  id;
    uint8_t interfaceNumber;
    USB_HOST_OBJ *hcObj;
    uint8_t  driverFound = false ;
    USB_HOST_TARGET_PERIPHERAL_LIST *driverInfo;
    USB_HOST_CONFIGURATION_INFO *configurationInfo;

    USB_INTERFACE_DESCRIPTOR * interfaceDescriptor;
  
    configNumber = newDevice->activeConfigurationNumber;

    id = newDevice->deviceId;
    hostIndex = ( id >> 8 ) & 0xFF;

    /* Address mapping */
    hcObj =  &(gUSBHostObj[hostIndex]);

    /* Based on interface descriptor values we have to find driver */
    configurationInfo = &(newDevice->configDescriptorInfo );
    for( interfaceNumber =0 ; interfaceNumber < configurationInfo->
            configurationDescriptor->bNumInterfaces ; interfaceNumber++ )
    {
        if ( configurationInfo->interfaceInfo[interfaceNumber].
                isClaimed == true )
            continue;
        interfaceDescriptor = configurationInfo->interfaceInfo [interfaceNumber].interfaceDesc;
        for( count = 0 ; count < hcObj->nTPLEntries ; count++ )
        {
            driverInfo = &(gUSBHostObj[hostIndex].usbHostTpl[count]);

            if(( interfaceDescriptor->bInterfaceClass == driverInfo->device.class ) &&
                    ( interfaceDescriptor->bInterfaceSubClass == driverInfo->device.subClass )&&
                    ( interfaceDescriptor->bInterfaceProtocol == driverInfo->device.protocol ))
            {
                newDevice->driverType =
                    USB_HOST_STANDARD_INTERFACE_DRIVER ;
                configurationInfo->interfaceInfo
                    [interfaceNumber].interfaceDriver =
                    driverInfo->classDriver ;
                driverFound = true ;
                configurationInfo->interfaceInfo[ interfaceNumber ].isClaimed = true ;
              
                break;
            }
        }
    }
    return   driverFound;
}

bool  _USB_HOST_AttachInterfaceDriver ( USB_HOST_DEVICE_INFO *newDevice )
{
    USB_HOST_CONFIGURATION_INFO *configurationInfo;
    uint8_t interfaceNumber;
    uint8_t instanceNumber;
    configurationInfo = &(newDevice->configDescriptorInfo );
    for( interfaceNumber =0 ; interfaceNumber < configurationInfo->
            configurationDescriptor->bNumInterfaces ; interfaceNumber++ )
    {
        if(   configurationInfo->interfaceInfo [interfaceNumber].interfaceDriver != NULL )
        {
            configurationInfo->interfaceInfo [interfaceNumber].interfaceDriver->
                initializeInterfaceDriver( newDevice->deviceId , interfaceNumber,&instanceNumber ,
                        newDevice->speed );
            configurationInfo->interfaceInfo[ interfaceNumber ].instanceNumber = instanceNumber;
        }
    }
    return true;
}



/*****************************************************************************/
/* Function:
    USB_ERROR _USB_HOST_IRPSubmit 
    (
        USB_HOST_PIPE_HANDLE pipeHandle ,
        USB_HOST_IRP *irp 
    )

  Summary:
    Submit the IRP packet to the Controller driver

  Description:
    Submit the IRP packet to the Controller driver

  Parameters:
    USB_HOST_IRP      - i/o request packet from class drivers

  Returns:
    USB_ERROR

  Remarks:
    This is a local function and should not be directly called by the
    application.
*/

USB_ERROR _USB_HOST_IRPSubmit (USB_HOST_PIPE_HANDLE pipeHandle , USB_HOST_IRP *irp)
{
    USB_ERROR status;
     /*default status*/
    status = USB_ERROR_NONE;
    /* Valied endpoint or not */
    if ( pipeHandle != DRV_USB_HOST_PIPE_HANDLE_INVALID && irp != NULL)
    {
        /* pass the info to HCD */
        status = DRV_USB_HOST_IRPSubmit( pipeHandle , irp );
    }
    else
    {
        /* pipe handle is not valid*/
        return USB_ERROR_ENDPOINT_NOT_CONFIGURED;
    }
    return status;
}

// *****************************************************************************
/* Function:
    USB_ERROR _USB_HOST_IRPCancel 
    (
        USB_HOST_PIPE_HANDLE pipeHandle, 
        USB_HOST_IRP *irp
    )

  Summary:
      cancel irps

  Description:
      API provides cancel the irps related to this pipe

  Parameters:
    USB_HOST_PIPE_HANDLE      - pipe information
    USB_HOST_IRP              *irp

  Returns:
    USB_ERROR                   status
 */

USB_ERROR _USB_HOST_IRPCancel (  USB_HOST_IRP *irp )
{
    USB_ERROR status;
    /* default state */
    status = USB_ERROR_NONE;
    /*validate the pipe handle and irp*/
    if( irp != (USB_HOST_IRP *)NULL)
    {
         /* pass the info to HCD */
         DRV_USB_HOST_IRPCancel(irp);
    }
    else
    {
        /* pipe handle is not valid*/
        return USB_ERROR_ENDPOINT_NOT_CONFIGURED;
    }
    return status;
}

// *****************************************************************************
/* Function:
  USB_ERROR  _USB_HOST_PipeClose( USB_HOST_PIPE_HANDLE pipeHandle )

  Summary:
     Close the pipe

  Description:
     After detach the device we have close all the pipes related to the device.

  Parameters:
    USB_HOST_PIPE_HANDLE          PipeHandle          //Pipe handle

  Returns:
    USB_HOST_STATUS
*/

USB_ERROR _USB_HOST_PipeClose( USB_HOST_PIPE_HANDLE pipeHandle )
{
    USB_ERROR status;
    status = USB_ERROR_NONE;
    /* validate pipe handle */
    if(pipeHandle != ( DRV_USB_HOST_PIPE_HANDLE ) NULL)
    {
        /* close the pipe */
        DRV_USB_HOST_PipeClose ( pipeHandle );
        return status;
    }
    else
    {
        status = USB_ERROR_HOST_PIPE_INVALID;
    }
    return status;
}

//*****************************************************************************
/* Function:
    USB_ERROR _USB_HOST_SetInterface
    (
        USB_HOST_DEVICE_ID deviceId,
        uint8_t interfaceNumber,
        uint8_t alrernateSettingNumber,
        void* callback 
    )

  Summary:
    Set Interface

  Description:
    Function drivers can set the interface based on alternatesetting

  Parameters:
    USB_HOST_DEVICE_ID        - deviceId // HC id + Device id
    uint8_t                   - interfaceNumber
    uint8_t                   - alrernateSettingNumber

  Returns:
    USB_ERROR                 - status

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_ERROR _USB_HOST_SetInterface
(
    USB_HOST_DEVICE_ID deviceId,
    uint8_t interfaceNumber,
    uint8_t alrernateSettingNumber,
    void * callback 
)
{
    USB_ERROR status;
    USB_HOST_OBJ *hcObj;
    USB_HOST_DEVICE_INFO *deviceInfo;
    USB_HOST_CONFIGURATION_INFO *configurationInfo;
  //  USB_HOST_INTERFACE_ALTERNATE_SETTING_INFO *altSettingInfo;
    uint8_t  hostIndex;

    status = USB_ERROR_NONE;
    /* Get hostIndex value */
    hostIndex = ( deviceId >> 8) & 0xFF ;
    /* Address mapping */
    hcObj =  &( gUSBHostObj[ hostIndex ] );
    /* device information */
    deviceInfo = _USB_HOST_GetDeviceInfo( deviceId );
    if( hcObj == NULL || deviceInfo == NULL )
        return USB_HOST_OBJ_INVALID;
    /* get configuration information */
    configurationInfo = &( deviceInfo->configDescriptorInfo );
  


 _USB_HOST_FillSetupPacket(
                   &(deviceInfo->setupPacket),
                   ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                     USB_SETUP_TYPE_STANDARD |
                     USB_SETUP_RECIPIENT_INTERFACE ),
                     USB_REQUEST_SET_INTERFACE,
                     alrernateSettingNumber , interfaceNumber  ,0 ) ;

    /* Fill irp */
    deviceInfo->controlIrp.data = NULL;
    deviceInfo->controlIrp.setup = & (deviceInfo->setupPacket);
    deviceInfo->controlIrp.size = 0;
    deviceInfo->controlIrp.callback=(USB_HOST_IRP_CALLBACK)callback;
    /* submit irp */
    DRV_USB_HOST_IRPSubmit(deviceInfo->controlPipeHandle, &deviceInfo->controlIrp); 
    return status;
}

// *****************************************************************************
/* Function:
    USB_ERROR _USB_HOST_EndpointHalt
    (
        USB_HOST_DEVICE_ID deviceId ,
        uint8_t endpointNumber ,
        void* callback 
    );

  Summary:
    Setup packet with set feature of halt endpoint

  Description:
    Use for prevent the device from deenumerated

  Parameters:
    USB_HOST_DEVICE_ID      - deviceId
    uint8_t                 - endpointNumber
    void*                   - callback

  Returns:
    USB_ERROR              - status

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_ERROR   _USB_HOST_EndpointHalt 
(
    USB_HOST_DEVICE_ID deviceId ,
    uint8_t endpointNumber ,
    void* callback 
)
{
    USB_ERROR status;
    USB_HOST_OBJ *hcObj;
    USB_HOST_DEVICE_INFO *deviceInfo;
    uint8_t    hostIndex;

    /* prevent the device from deenumerated */
    status = USB_ERROR_NONE ;
    /* Get hostIndex value */
    hostIndex = ( deviceId >> 8) & 0xFF ;
    /* Address mapping */
    hcObj =  &( gUSBHostObj[ hostIndex ] );
    /* device information */
    deviceInfo = _USB_HOST_GetDeviceInfo( deviceId );
    if(hcObj == NULL || deviceInfo == NULL )
        return USB_HOST_OBJ_INVALID;
    /* set feature */

   _USB_HOST_FillSetupPacket(
                   &(deviceInfo->setupPacket),
                   ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                     USB_SETUP_TYPE_STANDARD |
                     USB_SETUP_RECIPIENT_ENDPOINT ),
                     USB_REQUEST_SET_FEATURE ,
                     USB_FEATURE_SELECTOR_ENDPOINT_HALT , endpointNumber  ,0 ) ;

  
    /* Fill irp */
    deviceInfo->controlIrp.data = NULL;
    deviceInfo->controlIrp.setup = &( deviceInfo->setupPacket );
    deviceInfo->controlIrp.size = 0;
    deviceInfo->controlIrp.callback = ( USB_HOST_IRP_CALLBACK ) callback;
    status = DRV_USB_HOST_IRPSubmit( deviceInfo->controlPipeHandle, &deviceInfo->controlIrp );
     // Give some time delay to settle down for set feature
    return status;
}


//*****************************************************************************
/* Function:
    USB_ERROR   _USB_HOST_ClearEndpointHalt 
    (
        USB_HOST_DEVICE_ID deviceId ,
        uint8_t endpointNumber,
        void* callback 
    )

  Summary:
    Setup packet with set feature of clear halt endpoint

  Description:
    This function Use for prevent the device from deenumerated and
    clear the halt

  Parameters:
    USB_HOST_DEVICE_ID   - deviceId
    uint8_t              - endpointNumber
    void*               - callback
  
  Returns:
    USB_ERROR             - status

  Remarks:
    This is a local function and should not be called directly by the
    application.

*/

USB_ERROR   _USB_HOST_ClearEndpointHalt 
(
    USB_HOST_DEVICE_ID deviceId ,
    uint8_t endpointNumber ,
    void * callback 
)
{
    USB_ERROR status;
    USB_HOST_OBJ *hcObj;
    USB_HOST_DEVICE_INFO *deviceInfo;
    uint8_t    hostIndex;
    /* prevent the device from deenumerated */
    /* clear the halt */
    status = USB_ERROR_NONE;
    /* Get hostIndex value */
    hostIndex = ( deviceId >> 8) & 0xFF ;
    /* Address mapping */
    hcObj =  &( gUSBHostObj[ hostIndex ] );
    /* device information */
    deviceInfo = _USB_HOST_GetDeviceInfo( deviceId );

    if( hcObj == NULL || deviceInfo == NULL )
        return USB_HOST_OBJ_INVALID;

    /* set feature */
   _USB_HOST_FillSetupPacket(
                   &(deviceInfo->setupPacket),
                   ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                     USB_SETUP_TYPE_STANDARD |
                     USB_SETUP_RECIPIENT_ENDPOINT ),
                     USB_REQUEST_CLEAR_FEATURE ,
                     USB_FEATURE_SELECTOR_ENDPOINT_HALT , endpointNumber  ,0 ) ;

    
    /* Fill irp */
    deviceInfo->controlIrp.data = NULL;
    deviceInfo->controlIrp.setup = & ( deviceInfo->setupPacket );
    deviceInfo->controlIrp.size = 0;
    deviceInfo->controlIrp.callback = ( USB_HOST_IRP_CALLBACK ) callback;

    status = DRV_USB_HOST_IRPSubmit( deviceInfo->controlPipeHandle, &deviceInfo->controlIrp );
     
    return status;
}

// ***************************************************************************
/* Function:
    USB_ERROR   _USB_HOST_IsEndpointIsHalted 
    (
                                            USB_HOST_DEVICE_ID deviceId ,
                                            uint8_t endpointNumber ,
                                            uint16_t *data ,
                                            void* callback )
  Summary:
        setup packet with find status of endpoint

  Description:
    This function use for find the status of endpoint.

  Parameters:
         USB_HOST_DEVICE_ID   - deviceId
         uint8_t              - endpointNumber
         uint16_t *           - data
         void*                - callback
  Returns:
        USB_ERROR             - status
 */

/* TODO: Who calls these functions */
USB_ERROR _USB_HOST_IsEndpointIsHalted 
(
    USB_HOST_DEVICE_ID deviceId ,
    uint8_t endpointNumber ,
    uint16_t *data ,
    void* callback
)
{
    USB_ERROR status;
    USB_HOST_OBJ *hcObj;
    USB_HOST_DEVICE_INFO *deviceInfo;
    uint8_t  hostIndex;
    /* prevent the device from deenumerated */
    /* clear the halt */
    status = USB_ERROR_NONE;
     /* Get hostIndex value */
    hostIndex = ( deviceId >> 8) & 0xFF ;

     /* Address mapping */
    hcObj =  &( gUSBHostObj[ hostIndex ] );
    /* device information */
    deviceInfo = _USB_HOST_GetDeviceInfo( deviceId );

    if( hcObj == NULL || deviceInfo == NULL )
        return USB_HOST_OBJ_INVALID;

    _USB_HOST_FillSetupPacket(
                   &(deviceInfo->setupPacket),
                   ( USB_SETUP_DIRN_DEVICE_TO_HOST |
                     USB_SETUP_TYPE_STANDARD |
                     USB_SETUP_RECIPIENT_ENDPOINT ),
                     USB_REQUEST_GET_STATUS ,
                     0 , endpointNumber  , 2 ) ;

    /* Fill setup packet for set interface */

    /* Fill irp */
    deviceInfo->controlIrp.data = data;
    deviceInfo->controlIrp.setup = &( deviceInfo->setupPacket );
    deviceInfo->controlIrp.size = 2;
    deviceInfo->controlIrp.callback = ( USB_HOST_IRP_CALLBACK ) callback;
    status = DRV_USB_HOST_IRPSubmit( deviceInfo->controlPipeHandle, &deviceInfo->controlIrp );

    return status;
}

/*************************************************************************/
/* Function:
    USB_ERROR _USB_HOST_DeEnumerate_Device ( USB_HOST_DEVICE_ID  deviceId )

  Summary:
    DeEnumerates disconnected device

  Description:
    This function deenumerate newly disconnected device. All the resourcess
    allocated for the device has to be removed.

  Parameters:
   USB_HOST_DEVICE_ID             -  device id

  Returns:
    None

  Remarks:
    This is a local function and  should not be called directly by the
    application.
*/

USB_ERROR  _USB_HOST_DeEnumerate_Device( USB_HOST_DEVICE_ID  deviceID )
{
     USB_ERROR  status;
     USB_HOST_OBJ *hcObj;
     USB_HOST_DEVICE_INFO *deviceInfo = NULL;
     uint8_t  deviceAddress = 0 ;
     uint8_t  interfaceNumber;
     uint8_t   hostIndex;
     USB_HOST_CONFIGURATION_INFO *configurationInfo;
     status =  USB_ERROR_NONE;
     /* Get hostIndex value */
     hostIndex = ( deviceID >> 8) & 0xFF ;

     /* Address mapping */
     hcObj =  &( gUSBHostObj[ hostIndex ]);
     
   /* Get parent device information */
     deviceInfo = _USB_HOST_GetDeviceInfo( deviceID );

   
     if(hcObj ==  NULL  || deviceInfo == NULL)
     {
         return  USB_HOST_OBJ_INVALID ;
     }

      /* Device Address */
    deviceAddress    = ( uint8_t ) ( deviceID & 0xFF );

    if ( deviceAddress == 0x00 )
    {
        /* Device is not addressed.*/
        return USB_ERROR_NONE;
    }

 

     /* Disconnect the driver */
     if( deviceInfo->driver )
     {
        /* if the driver is class or vendor driver */
        deviceInfo->driver->deInitializeDriver( deviceID );
     }

     configurationInfo = & ( deviceInfo->configDescriptorInfo );
     for( interfaceNumber =0 ; interfaceNumber < configurationInfo->
                 configurationDescriptor->bNumInterfaces ; interfaceNumber++ )
     {
        /* Make all configurations unclaimed */
        configurationInfo->interfaceInfo[interfaceNumber].isClaimed = false;
        if ( configurationInfo->interfaceInfo[ interfaceNumber ].
                                                     interfaceDriver != NULL )
        {

           configurationInfo->interfaceInfo[ interfaceNumber ].
                               interfaceDriver->deInitializeDriver( deviceID ) ;
        }
            /* interface level drivers */
       }
     // pipes are available we have to close all the pipes
     if ( deviceInfo->controlPipeHandle != USB_HOST_PIPE_HANDLE_INVALID )
     {
        _USB_HOST_PipeClose( deviceInfo->controlPipeHandle );
     }
     if( hcObj->defaultPipeHandle != USB_HOST_PIPE_HANDLE_INVALID )
     {
        _USB_HOST_PipeClose( hcObj->defaultPipeHandle );
     }
     /* Release address */
     _USB_HOST_FreeAddress (deviceID);
     deviceInfo->deviceId = 0x00;

     if( deviceInfo->inUse )
     {
        deviceInfo->inUse = 0x00;
        hcObj->attachDevicesCount --;
     }
     /* Free allocated buffer */
     if ( deviceInfo->rawBuffer != NULL )
     {
        USB_HOST_FREE(deviceInfo->rawBuffer);
     }
     return status;
}

// ***************************************************************************
/* Function:
    void _USB_HOST_ClaimInterface 
    (
        USB_HOST_DEVICE_ID id ,
        uint8_t configurationNumber ,
        uint8_t interfaceNumber 
    )

  Summary:
    USB Host vendor drivers can claim the interface.

  Description:
    This function use for inform the host that interface is belongs
    to the vendor driver

  Parameters:
     USB_HOST_DEVICE_ID   - deviceId
     uint8_t              - configurationNumber
     uint8_t              - interfaceNumbe
  
  Returns:
     None

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_ClaimInterface 
(
    USB_HOST_DEVICE_ID id ,
    uint8_t configurationNumber ,
    uint8_t interfaceNumber 
)
{
     USB_HOST_DEVICE_INFO *deviceInfo;
     /* Get parent device information */
     deviceInfo = _USB_HOST_GetDeviceInfo( id );
     deviceInfo->configDescriptorInfo.interfaceInfo[ interfaceNumber ].isClaimed = true ;
}

// ***************************************************************************
/* Function:
    void _USB_HOST_SelectConfiguration 
    (
        USB_HOST_DEVICE_ID id ,
        uint8_t configurationNumber
    )

  Summary:
    USB Host vendor drivers can select the configuration.

  Description:
    This function use for Host vendor drivers after the validation of supporting
    device then select the configuration.

  Parameters:
     USB_HOST_DEVICE_ID   - deviceId
     uint8_t              - configurationNumber
     uint8_t              - interfaceNumbe

  Returns:
     None
*/

void _USB_HOST_SelectConfiguration 
( 
    USB_HOST_DEVICE_ID id ,
    uint8_t configurationNumber 
)
{
     USB_HOST_DEVICE_INFO *deviceInfo;
     /* Get parent device information */
     deviceInfo = _USB_HOST_GetDeviceInfo(id );
     deviceInfo->activeConfigurationNumber = configurationNumber;
    
}

// ***************************************************************************
/* Function:
    USB_ERROR USB_HOST_ConfigurationSet 
    (
        USB_HOST_HANDLE hostHandle,
        uint8_t deviceAddress ,
        uint8_t configurationNumber
    )

  Summary:
    USB Host Application can select the configuration.

  Description:
    This function use for Host application can change configuration

  Parameters:
     USB_HOST_HANDLE      - HostHandle
     uint8_t              - deviceAddress
     uint8_t              - configurationNumber

  Returns:
     None.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_ERROR USB_HOST_ConfigurationSet 
( 
    USB_HOST_HANDLE hostHandle,
    uint8_t deviceAddress ,
    uint8_t configurationNumber 
)
{
     USB_HOST_DEVICE_INFO *deviceInfo=  NULL;
     uint8_t count;
     USB_ERROR  status ;
     USB_HOST_OBJ *hcObj;
    
     status =  USB_ERROR_NONE;
       /* bus information  */
     hcObj =  &( gUSBHostObj[ hostHandle ] );

     for ( count = 0; count < USB_HOST_MAX_DEVICES_NUMBER; count++)
     {
        if( ( hcObj->attachedDevicesList[count].deviceId & 0xFF)
                                                      == deviceAddress)
        {
            deviceInfo = &( hcObj->attachedDevicesList[ count ] );
            break;
        }
    }

   if (  hcObj->enumerationPending != true )
   {
       hcObj->enumerationPending = true ;
       hcObj->enumerateDeviceInfo = deviceInfo ;
       deviceInfo->deviceState =
                 USB_HOST_DEVICE_STATE_CONFIGURING |
                 USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR |
                 USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_CONFIG_DESCRIPTOR_SIZE;
       deviceInfo->configurationNumber =  configurationNumber  ;
       USB_HOST_FREE(deviceInfo->rawBuffer);
       return status;
   }
   return USB_ERROR_HOST_BUSY;
}

// *****************************************************************************
/* Function:
    void USB_HOST_DeviceSuspend ( USB_HOST_HANDLE handle, uint8_t address );

  Summary:
    Suspends the USB.

  Description:
    The function suspends the USB. If address is an address of a device that is
    connected directly to the USB module, this function will directly suspend
    the bus. If the address is an address of a device that is connected to the
    module via a hub, the USB Host layer will suspend the hub port to which this
    device is connected. The function only places a request to suspend the
    device. The application must use the USB_HOST_DeviceIsSuspended() function
    to check if the device suspend is complete.

 Remarks:
    See usb_host.h for usage information.
 */

USB_ERROR   USB_HOST_DeviceSuspend ( USB_HOST_HANDLE hostHandle,
                                     uint8_t deviceAddress)
{
    //To be implemented in next release
    return USB_ERROR_NONE;
}

// *****************************************************************************
/* Function:
    void USB_HOST_DeviceIsSuspended ( USB_HOST_HANDLE handle, uint8_t address );

  Summary:
    validate the device is suspended or not .

  Description:


 Remarks:
    See usb_host.h for usage information.
 */

bool   USB_HOST_DeviceIsSuspended      ( USB_HOST_HANDLE hostHandle,
                                         uint8_t deviceAddress)
{

    //To be implemented in next release
    return USB_ERROR_NONE;
}

// *****************************************************************************
/* Function:
    void USB_HOST_DeviceResume ( USB_HOST_HANDLE handle, uint8_t address );

  Summary:
    Resumes the USB.

  Description:
    The function resumes the USB. If address is an address of a device that is
    connected directly to the USB module, this function will directly resume
    the bus. If the address is an address of a device that is connected to the
    module via a hub, the USB Host layer will resume the hub port to which this
    device is connected. The function only places a request to resume the
    device. The application must use the USB_HOST_DeviceIsResumeed() function
    to check if the device resume is complete.

 Remarks:
    See usb_host.h for usage information.
 */

USB_ERROR USB_HOST_DeviceResume ( USB_HOST_HANDLE hostHandle,
                                  uint8_t deviceAddress )
{

    //To be implemented in next release
    return USB_ERROR_NONE;
}

// *****************************************************************************
/* Function:
    bool USB_HOST_DeviceIsResumed ( USB_HOST_HANDLE handle, uint8_t address );

  Summary:
    This function to check if the device has been
    resumed. USB_HOST_DeviceIsResumed

  Description:
    The function resumes the USB. If address is an address of a device that is
    connected directly to the USB module, this function will directly resume
    the bus. If the address is an address of a device that is connected to the
    module via a hub, the USB Host layer will resume the hub port to which this
    device is connected. The function only places a request to resume the
    device. The application must use the USB_HOST_DeviceIsResumeed() function
    to check if the device resume is complete.

 Remarks:
    See usb_host.h for usage information.
 */
bool USB_HOST_DeviceIsResumed(USB_HOST_HANDLE hostHandle, uint8_t deviceAddress)
{
    //To be implemented in next release
    return USB_ERROR_NONE;
}


/*****************************************************************************/
/*  Function:
   USB_ERROR  _USB_HOST_FindEndpoint (
                                    USB_HOST_QUERY *query,
                                    USB_ENDPOINT_DESCRIPTOR **endPointDesc )

  Summary:
    Find endpoint descriptor

  Description:
   USB Host driver can query the host and get the endpoint descriptor information

  Parameters:
    USB_HOST_DRIVER_QUERY *         - query,
    USB_ENDPOINT_DESCRIPTOR *      - *endPointDesc
  Returns:
    USB_ERROR            - status
*/

USB_ERROR  _USB_HOST_FindEndpoint (
                                    USB_HOST_QUERY *query ,
                                    USB_ENDPOINT_DESCRIPTOR **endPointDesc )
{
    USB_ERROR status;
    USB_INTERFACE_DESCRIPTOR *interfaceDescriptor;
    USB_ENDPOINT_DESCRIPTOR *endpointDescriptor;
    USB_DESCRIPTOR_HEADER *header;
    uint16_t   size;
    bool interfaceFound = false;
    USB_HOST_DEVICE_INFO *deviceInfo;
    USB_CONFIGURATION_DESCRIPTOR *tempcfg;
    uint8_t     *buffer;

    /* Get device information */
    deviceInfo = _USB_HOST_GetDeviceInfo( query->id );
    status = USB_ERROR_NONE;
    /* validate arguments */
    if ( query == NULL  )
        return USB_ERROR_HOST_ARGUMENTS_INVALID;
    /* copy the configuration info into local pointer */
    buffer  =  deviceInfo->rawBuffer ;

    /* copy the size of total configuration size */
    size =  deviceInfo->configDescSize ;
    tempcfg  = ( USB_CONFIGURATION_DESCRIPTOR *) buffer ;

    /* validate the length of configuration descriptor*/
    if ( tempcfg->bLength != USB_HOST_CONFIG_DESCRPTR_SIZE)
        return USB_ERROR_HOST_HEADERSIZE_INVALID;

         /* Validate the number of interfaces */
    if ( query->interfaceNumber  >= tempcfg->bNumInterfaces  )
        return USB_ERROR_HOST_MAX_INTERFACES_INVALID;

    buffer = buffer + USB_HOST_CONFIG_DESCRPTR_SIZE ;
    size = size - USB_HOST_CONFIG_DESCRPTR_SIZE ;

    while (size > 2)
    {
        /* parse header information */
        header = (USB_DESCRIPTOR_HEADER *) buffer;
        if ( header->size > size )
            return USB_ERROR_HOST_HEADERSIZE_INVALID;

            if( header->descType == USB_DESCRIPTOR_INTERFACE )
            {
                interfaceDescriptor = (USB_INTERFACE_DESCRIPTOR * )buffer ;
                /* interface checking */
                if ( ( query->interfaceNumber ==
                        interfaceDescriptor->bInterfaceNumber)  &&
                        (query->alternateSettingNumber ==
                        interfaceDescriptor->bAlternateSetting ))
                {
                    interfaceFound = true ;
                }
               
            }
           
            if ( interfaceFound == true  &&  
                    ( header->descType ==  USB_DESCRIPTOR_ENDPOINT ) )
            {
                if( ( query->flags & USB_HOST_MATCH_ENDPOINT_TYPE) && 
                        (query->flags & USB_HOST_MATCH_ENDPOINT_DIRECTION) )
                {
                    endpointDescriptor = (USB_ENDPOINT_DESCRIPTOR *) buffer;
                    if ( ( endpointDescriptor->transferType ==
                                            query->endpointType ) &&
                                            ( query->endpointDirection ==
                                                endpointDescriptor->dirn  ))
                    {
                        *endPointDesc = endpointDescriptor;
                        return status;
                    }
                   
                }
               
            }

            size = size - header->size ;
            buffer = buffer + header->size ;

         }
    return USB_ERROR_HOST_ENDPOINT_NOT_FOUND ;
}


 
 // *****************************************************************************
/* Function:
    USB_ERROR USB_HOST_FindClassInterface ( USB_HOST_QUERY *query,
                                      void **classInterfaceDesc )

  Summary:
    Find class specific interface descriptor

  Description:
    USB Host driver can query the host and get the class specific interface
    descriptor

  Parameters:
    USB_HOST_DRIVER_QUERY               *query,
    void *                              *classInterfaceDesc

  Returns:
    USB_ERROR
*/


 USB_ERROR USB_HOST_FindClassInterface ( USB_HOST_QUERY *query,
                                      void **classInterfaceDesc )
{
    USB_ERROR status;
    status = USB_ERROR_NONE;
    USB_HOST_CONFIGURATION_INFO *configurationInfo;
    USB_INTERFACE_DESCRIPTOR *interfaceDescriptor;
    USB_DESCRIPTOR_HEADER *header;
    USB_HOST_DEVICE_INFO *deviceInfo;
    USB_CONFIGURATION_DESCRIPTOR *tempcfg;
    uint16_t   size;
    uint8_t     *buffer;
    status = USB_ERROR_NONE;
    deviceInfo = _USB_HOST_GetDeviceInfo(query->id);
    /* Get active configuration information */
    configurationInfo = &( deviceInfo->configDescriptorInfo );
    /* copy the configuration info into local pointer */
    buffer  =  deviceInfo->rawBuffer ;
    /* copy the size of total configuration size */
    size =  deviceInfo->configDescSize ;
    tempcfg  = ( USB_CONFIGURATION_DESCRIPTOR *) buffer ;

    /* validate the length of configuration descriptor*/
    if ( tempcfg->bLength != USB_HOST_CONFIG_DESCRPTR_SIZE)
        return USB_ERROR_HOST_HEADERSIZE_INVALID;

    /* Validate the number of interfaces */
    /* Every alt setting starts with an exactly one interface descriptor
       followed by zero or more class specific interace descriptors
       followed by zero or more endpoint descriptors .
       Each endpoint descriptor is immediateley followed by zero or
       more class specific endpoint descriptor */

    buffer = buffer + USB_HOST_CONFIG_DESCRPTR_SIZE ;
    size = size - USB_HOST_CONFIG_DESCRPTR_SIZE ;

    while (size > 2)
    {
        /* parse header information */
        header = (USB_DESCRIPTOR_HEADER *) buffer;
        if ( header->size > size )
            return USB_ERROR_HOST_HEADERSIZE_INVALID;

        if( header->descType == USB_DESCRIPTOR_INTERFACE )
        {
            interfaceDescriptor = (USB_INTERFACE_DESCRIPTOR * )buffer ;
            /* interface checking */
            if ( interfaceDescriptor->bInterfaceNumber ==  query->interfaceNumber )
            {
                if(  interfaceDescriptor->bAlternateSetting == query->alternateSettingNumber)
                {
                    buffer = buffer + header->size ;
                    *classInterfaceDesc = buffer;
                }

            }
        }
        size = size - header->size ;
        buffer = buffer + header->size ;
    }

    return status;
}


uint32_t _USB_HOST_CheckBandwidth 
(
    uint8_t direction,
    uint8_t endpointType,
    uint8_t  speed,
    uint32_t maxPacketSize
)
{
    uint32_t bandwidth = 0 ;
    switch ( speed )
    {
        case USB_SPEED_LOW :

            if( endpointType == USB_TRANSFER_TYPE_CONTROL  ||
                    endpointType ==  USB_TRANSFER_TYPE_BULK  ||
                    endpointType  ==  USB_TRANSFER_TYPE_INTERRUPT ||
                    endpointType  ==  USB_TRANSFER_TYPE_ISOCHRONOUS )

            {
                // bandwidth = 64060 + (2 * Hub_LS_Setup) + (676.67 *
                //  Floor(3.167 + BitStuffTime(Data_bc))) + Host_Delay;
                if ( direction ==  USB_EP_DIRECTION_IN )
                    bandwidth = 40 ;
                else
                    bandwidth = 60;
            }

            break;
        case   USB_SPEED_FULL:

            if( endpointType == USB_TRANSFER_TYPE_CONTROL  ||
                    endpointType ==  USB_TRANSFER_TYPE_BULK  ||
                    endpointType  ==  USB_TRANSFER_TYPE_INTERRUPT
              )
            {
                // bandwidth = 9107 + (83.54 * Floor(3.167 +
                //         BitStuffTime(Data_bc))) + Host_Delay ;

                bandwidth =  80 ;
            }

            if ( endpointType == USB_TRANSFER_TYPE_ISOCHRONOUS )
            {
                //bandwidth = 7268 + (83.54 * Floor(3.167 +
                //         BitStuffTime(Data_bc))) + Host_Delay;
                if ( direction ==  USB_EP_DIRECTION_IN )
                    bandwidth = 200 ;
            }
            if (endpointType ==  USB_TRANSFER_TYPE_ISOCHRONOUS )
            {
                // bandwidth = 6265 + (83.54 * Floor(3.167 +
                //        BitStuffTime(Data_bc))) + Host_Delay;
                if ( direction ==  USB_EP_DIRECTION_OUT )
                    bandwidth = 160 ;
            }

            break;
        case   USB_SPEED_HIGH  :
            if( endpointType == USB_TRANSFER_TYPE_CONTROL  ||
                    endpointType ==  USB_TRANSFER_TYPE_BULK  ||
                    endpointType  ==  USB_TRANSFER_TYPE_INTERRUPT 
              )
            {

                // bandwidth = (55 * 8 * 2.083) + (2.083 * Floor(3.167 +
                //        BitStuffTime(Data_bc))) + Host_Delay;
                bandwidth = 100;
            }

            else if ( endpointType == USB_TRANSFER_TYPE_ISOCHRONOUS    )
            {
                // bandwidth = (38 * 8 * 2.083) + (2.083 * Floor(3.167 +
                //       BitStuffTime(Data_bc))) + Host_Delay;
                bandwidth = 200;
            }
            break;


    }
    return bandwidth;
}


// *****************************************************************************
/* Function:
    USB_HOST_INSTANCE_CLIENT * _USB_HOST_ClientHandleValidate
    (
        USB_HOST_HANDLE handle
    )

  Summary:
    This function validates the client handle.

  Description:
    This function validates the client handle and returns NULL if the handle is
    invalid.

  Remarks:
    This is a local function and should not be called directly by the application.
*/

USB_HOST_INSTANCE_CLIENT * _USB_HOST_ClientHandleValidate(USB_HOST_HANDLE handle)
{
    /* This function validates the client handle. It returns NULL if the client
     * handle is invalid or if the Host Instance is invalid */

    USB_HOST_OBJ * hostObj;
    USB_HOST_INSTANCE_CLIENT * clientObj;

    if(USB_HOST_HANDLE_INVALID == handle)
    {
        return NULL;
    }

    /* Now check if the hardware instance is ready */
    clientObj = (USB_HOST_INSTANCE_CLIENT *)handle;
    hostObj = clientObj->usbHostInstance;

    if(hostObj->usbHostInstanceState != SYS_STATUS_READY)
    {
        return NULL;
    }
    else
    {
        return clientObj;
    }
}


void  _USB_HOST_FillSetupPacket
(
    USB_SETUP_PACKET *setupPacket ,
    uint8_t requestType,
    uint8_t request ,
    uint16_t value,
    uint16_t index,
    uint16_t length
)
{
    setupPacket->bmRequestType = requestType;
    setupPacket->bRequest = request ;
    setupPacket->wValue = value ;
    setupPacket->wIndex = index ;
    setupPacket->wLength = length;


}

 /************************  EOF **********************************************/
