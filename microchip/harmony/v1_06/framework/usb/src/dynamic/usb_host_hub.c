
/*******************************************************************************
  USB Host Hub driver implemention.

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hub.c

  Summary:
    This file contains implementations of both private and public functions
    of the USB Host HUB client driver.

  Description:
    This file contains the USB host Hub client driver implementation. This file
    should be included in the project if USB HUB devices are to be supported.
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

#include "usb/usb_host_client_driver.h"
#include "usb/usb_host.h"
#include "usb/usb_hub.h"
#include "usb/usb_host_hub_interface.h"
#include "usb/src/usb_host_hub_local.h"
#include "system/debug/sys_debug.h"



/**************************************************
 * Global array of HUB Instance Objects. Each for
 * one HUB device attached.
 ***************************************************/
USB_HOST_HUB_DRIVER_INFO      gUSBHostHubObj[ USB_HOST_HUB_INSTANCES_NUMBER ];

/*************************************************
 * Driver interface that is provide to the
 * host layer.
 *************************************************/

USB_HOST_CLIENT_DRIVER  gUSBHostHUBClientDriver =
{

    .initialize         = _USB_HOST_HUB_Initialize ,
    .deinitialize       = _USB_HOST_HUB_Deinitialize ,
    .reinitialize       = _USB_HOST_HUB_Reinitialize,
    .deviceAssign       = _USB_HOST_HUB_DeviceAssign,
    .deviceRelease      = NULL,
    .deviceTasks        = NULL,
    .deviceEventHandler = NULL,
    .interfaceAssign    = _USB_HOST_HUB_InterfaceAssign,
    .interfaceRelease   = _USB_HOST_HUB_InterfaceRelease,
    .interfaceEventHandler = _USB_HOST_HUB_InterfaceEventHandler,
    .interfaceTasks     = _USB_HOST_HUB_InterfaceTasks
};


// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_Initialize(void * hubInitData)

  Summary:
    This function is called when the Host Layer is initializing.

  Description:
    This function is called when the Host Layer is initializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_HUB_Initialize(void * hubInitData )
{
    /* Make sure all the pipe handles are invalid */
    
    int iterator;
    USB_HOST_HUB_DRIVER_INFO * hubInstanceInfo;
    
    for (iterator = 0; iterator < USB_HOST_HUB_INSTANCES_NUMBER; iterator ++)
    {
        hubInstanceInfo = &gUSBHostHubObj[iterator];
        hubInstanceInfo->controlPipeHandle = USB_HOST_CONTROL_PIPE_HANDLE_INVALID;
        hubInstanceInfo->interruptInPipeHandle = USB_HOST_PIPE_HANDLE_INVALID;
        
    }
}


// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_Deinitialize(void)

  Summary:
    This function is called when the Host Layer is deinitializing.

  Description:
    This function is called when the Host Layer is deinitializing .

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_HUB_Deinitialize(void)
{
    /* This function is not implemented in this release  */
}

// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_Reinitialize(void)

  Summary:
    This function is called when the Host Layer is reinitializing.

  Description:
    This function is called when the Host Layer is reinitializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_HUB_Reinitialize(void * hubInitData)
{
    /* This function is not implemented in this release  */
}

// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_DeviceAssign 
    (
        USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,,
        USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
        USB_DEVICE_DESCRIPTOR * deviceDescriptor ,
    )

  Summary:
    This function is called when the Host Layer attaches this driver to an
    device.

  Description:
    This function is called when the Host Layer attaches this driver to an
    device level.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

/*************************************************************************/
void _USB_HOST_HUB_DeviceAssign 
( 
    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    USB_DEVICE_DESCRIPTOR * deviceDescriptor 
)
{

    /* HUB is not implemented device level driver in this release of the driver */
    USB_HOST_DeviceRelease(deviceHandle);

}

 // *****************************************************************************
/* USB Hub Port API Interface

  Summary:
    Group of function pointers to the USB Hub Port Functions.

  Description:
    This structure is a group of function pointers pointing to the USB Hub Port
    API routines. The USB Hub or USB root hub should export this group of
    functions so that the Host and device layer can access the port
    functionality.

  Remarks:
    None.
*/
  USB_HUB_INTERFACE  externalHubInterface =
  {
      .hubPortReset             =   USB_HOST_HUB_PortReset,
      .hubPortResetIsComplete   =   USB_HOST_HUB_PortResetComplete,
      .hubPortSuspend           =   USB_HOST_HUB_PortSuspend,
      .hubPortResume            =   USB_HOST_HUB_PortResume,
      .hubPortSpeedGet          =   USB_HOST_HUB_PortSpeedGet

  };
 
  // ****************************************************************************
/* Function:
    void USB_HOST_HUB_PortReset( uintptr_t deviceHandle, uint8_t portNumber )
    
  Summary:
    Resets the specified root hub port.
	
  Description:
    This function resets the hub port.The status of the reset signalling can be
    checked using the USB_HOST_HUB_PortResetComplete() function.
	
  Remarks:
    
*/

USB_ERROR USB_HOST_HUB_PortReset  (uintptr_t deviceHandle, uint8_t portNumber)
{
     USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo;
     USB_HOST_RESULT result = USB_ERROR_NONE ;
     hubDriverInstanceInfo = _USB_HOST_findDriverInfo( deviceHandle );
     if ( NULL == hubDriverInstanceInfo )
     {
            /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " USB_HOST_HUB_PortReset : Invalid HUB instance number " );
        return USB_ERROR_HOST_DEVICE_INSTANCE_INVALID;
         
     }
     hubDriverInstanceInfo->portInfo[portNumber].portResetCompleted = false;
     hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                        USB_HOST_HUB_PORT_RESET_STATE_WAIT;
     hubDriverInstanceInfo->controlRequest = USB_HOST_HUB_PORT_RESET;
     result = _USB_HOST_HUB_featureRequest ( 
                                    hubDriverInstanceInfo, portNumber , 
                                    USB_HOST_HUB_PORT_RESET, 
                                    USB_HOST_HUB_COMMAND_SET_FEATURE,
                                    USB_HOST_HUB_PORT_REQUEST );
     return result;
}

// ****************************************************************************
/* Function:
    bool USB_HOST_HUB_PortResetComplete 
    (
        uintptr_t deviceHandle,
        uint8_t portNumber
    );

  Summary:
    Returns true if the root hub has completed the port reset operation.

  Description:
    This function returns true if the port reset operation has completed. It
    should be called after the USB_HOST_HUB_PortReset() function to
    check if the reset operation has completed.

  Remarks:
    
 */
 bool USB_HOST_HUB_PortResetComplete 
 (
    uintptr_t deviceHandle, 
    uint8_t portNumber 
 )
 {
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo;
    hubDriverInstanceInfo = _USB_HOST_findDriverInfo( deviceHandle );

    if ( NULL == hubDriverInstanceInfo )
    {
            /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " USB_HOST_HUB_PortResetComplete : Invalid HUB instance number " );
        return USB_ERROR_HOST_DEVICE_INSTANCE_INVALID;
    }
    if( hubDriverInstanceInfo->portInfo[portNumber].portResetCompleted == true )
    {
        return true;
    }
    else
    {
        return false;
    }

 }
// ****************************************************************************
/* Function:
    USB_ERROR USB_HOST_HUB_PortSuspend 
    (
        uintptr_t deviceHandle, 
        uint8_t portNumber
    )

  Summary:
    Suspend the specified hub port.

  Description:
   
  Precondition:
    None.

  Parameters:
    handle - handle to the driver.

    port   - port to resume.

  Returns:
    None.

*/
USB_ERROR USB_HOST_HUB_PortSuspend 
(
    uintptr_t deviceHandle, 
    uint8_t portNumber
)
{
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo;
    USB_HOST_RESULT result = USB_ERROR_NONE ;
    hubDriverInstanceInfo = _USB_HOST_findDriverInfo( deviceHandle );
    
    if ( NULL == hubDriverInstanceInfo )
    {
            /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " USB_HOST_HUB_PortSuspend : Invalid HUB instance number " );
        return USB_ERROR_HOST_DEVICE_INSTANCE_INVALID;
    }
    hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                    USB_HOST_HUB_PORT_SUSPEND_STATE_WAIT;
    hubDriverInstanceInfo->controlRequest = USB_HOST_HUB_PORT_SUSPEND;
    result = _USB_HOST_HUB_featureRequest (
                                    hubDriverInstanceInfo, portNumber , 
                                    USB_HOST_HUB_PORT_SUSPEND, 
                                    USB_HOST_HUB_COMMAND_SET_FEATURE, 
                                    USB_HOST_HUB_PORT_REQUEST );

    return result;
}
// ****************************************************************************
/* Function:
USB_ERROR USB_HOST_HUB_PortResume 
( 
    uintptr_t deviceHandle, 
    uint8_t portNumber
)

  Summary:
    Resumes the specified root hub port.

  Description:
    This function resumes the hub port. 
*/

USB_ERROR USB_HOST_HUB_PortResume 
( 
    uintptr_t deviceHandle, 
    uint8_t portNumber
)
{
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo;
    USB_HOST_RESULT result = USB_ERROR_NONE ;
    hubDriverInstanceInfo = _USB_HOST_findDriverInfo( deviceHandle );
    if ( NULL == hubDriverInstanceInfo )
    {
            /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " USB_HOST_HUB_PortSuspend : Invalid HUB instance number " );
        return USB_ERROR_HOST_DEVICE_INSTANCE_INVALID;
    }
     hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                    USB_HOST_HUB_PORT_SUSPEND_STATE_WAIT;
     hubDriverInstanceInfo->controlRequest = USB_HOST_HUB_PORT_SUSPEND;
     result = _USB_HOST_HUB_featureRequest ( 
                                            hubDriverInstanceInfo, portNumber ,
                                            HUB_FEATURE_C_PORT_ENABLE , 
                                            USB_HOST_HUB_COMMAND_SET_FEATURE, 
                                            USB_HOST_HUB_PORT_REQUEST );

     return result;

}
// ****************************************************************************
/* Function:
USB_SPEED  USB_HOST_HUB_PortSpeedGet
(
    uintptr_t deviceHandle, 
    uint8_t port
)

  Summary:
    Returns the speed at which the bus to which this hub is connected is
    operating.

  Description:
    This function returns the speed at which the bus to which this port is
    connected is operating.

 Remarks:
    None.
*/
USB_SPEED  USB_HOST_HUB_PortSpeedGet
(
    uintptr_t deviceHandle, 
    uint8_t port
)
{
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo;
    USB_HOST_HUB_PORT_INFO *portInfo;
    hubDriverInstanceInfo = _USB_HOST_findDriverInfo( deviceHandle );
    if ( NULL == hubDriverInstanceInfo )
    {
            /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " USB_HOST_HUB_PortSpeedGet : Invalid HUB instance number " );
        return USB_ERROR_HOST_DEVICE_INSTANCE_INVALID;
    }
    portInfo = &(hubDriverInstanceInfo->portInfo[port]);
    if ( portInfo->portStatus.status & USB_HOST_HUB_BIT_PORT_LOW_SPEED  )
    {
        return  USB_SPEED_LOW;
    }
    else if (  portInfo->portStatus.status & USB_HOST_HUB_BIT_PORT_HIGH_SPEED )
    {
        return  USB_SPEED_HIGH;
    }
    else
    {
        return USB_SPEED_FULL;
    }
}
// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_InterfaceAssign 
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
  void _USB_HOST_HUB_InterfaceAssign 
(
    USB_HOST_DEVICE_INTERFACE_HANDLE * interfaces,
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    size_t nInterfaces,
    uint8_t * descriptor 
)

  {
      uint32_t hubIndex;
      USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo = NULL;
      USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle;
      USB_HOST_ENDPOINT_DESCRIPTOR_QUERY endpointQuery;
      USB_ENDPOINT_DESCRIPTOR *interruptEndpointDescriptor;
      USB_INTERFACE_DESCRIPTOR * interfaceDescriptor;
      USB_HOST_TRANSFER_HANDLE transferHandle;
      

      /* Hub driver will support only one interface which is the first
       * interface in the interfaces table */
      interfaceHandle = interfaces[0];

      /* This function is being called because an HUB device was attached and the
     * driver has matched. Check if there is a free HUB object. Searching for
     * free HUB object need not be mutex protected because this function will
     * always be called from one thread. */

      for ( hubIndex = 0 ; hubIndex < USB_HOST_HUB_INSTANCES_NUMBER ; hubIndex++ )
      {
         hubDriverInstnceInfo = &(gUSBHostHubObj[hubIndex]);
         if (hubDriverInstnceInfo->inUse == false )
         {
             /* Found a free instance */
             break;
         }

     }
     if ( hubIndex >= USB_HOST_HUB_INSTANCES_NUMBER)
     {
         /* Let the host know that this interface cannot be processed */
        _USB_HOST_HUB_InterfaceRelease(interfaceHandle);
           /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_HUB_InterfaceAssign : Invalid HUB instance number" );
        return ;

     }

     hubDriverInstnceInfo->instanceNumber = hubIndex;
     hubDriverInstnceInfo->inUse = true;
     hubDriverInstnceInfo->hubInterfaceHandle = interfaceHandle;
     hubDriverInstnceInfo->hubObjHandle = deviceObjHandle ;

     /* Get the interface descriptor pointer */
     interfaceDescriptor = (USB_INTERFACE_DESCRIPTOR *)(descriptor);
     hubDriverInstnceInfo->bInterfaceNumber = interfaceDescriptor->bInterfaceNumber;

     /* This means that we have found an HUB instance object and this device
      * can be processed. Open a control pipe to the device.  */

     hubDriverInstnceInfo->controlPipeHandle = USB_HOST_DeviceControlPipeOpen( deviceObjHandle );

     if( USB_HOST_CONTROL_PIPE_HANDLE_INVALID == hubDriverInstnceInfo->controlPipeHandle )
     {
         /* Let the host know that this interface cannot be processed */
        _USB_HOST_HUB_InterfaceRelease(interfaceHandle);
		
		hubDriverInstnceInfo->inUse = false;
            /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_HUB_InterfaceAssign : Invalid HUB pipe Handle " );
        return ;

     }

     USB_HOST_DeviceEndpointQueryContextClear(&endpointQuery);
     /* Find the interrupt in endpoint. To do this, first setup the
      * endpoint descriptor query */
     endpointQuery.transferType = USB_TRANSFER_TYPE_INTERRUPT;
     endpointQuery.direction = USB_DATA_DIRECTION_DEVICE_TO_HOST;
     endpointQuery.flags = USB_HOST_ENDPOINT_QUERY_BY_TRANSFER_TYPE | USB_HOST_ENDPOINT_QUERY_BY_DIRECTION ;
     
     /* Now find the endpoint */
     interruptEndpointDescriptor =  USB_HOST_DeviceEndpointDescriptorQuery ( interfaceDescriptor ,&endpointQuery);

     if ( NULL == interruptEndpointDescriptor  )
     {
           /* Let the host know that this interface cannot be processed */
        _USB_HOST_HUB_InterfaceRelease(interfaceHandle);
            /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_HUB_InterfaceAssign : inerrupt endpoint not found  " );
        return ;

     }

     /* We have found the IN interrupt endpoint. Try opening a pipe on this
      * endpoint */
     
     hubDriverInstnceInfo->interruptInPipeHandle = 
             USB_HOST_DevicePipeOpen ( interfaceHandle , 
                                interruptEndpointDescriptor->bEndpointAddress );

      if (  USB_HOST_CONTROL_PIPE_HANDLE_INVALID == 
                        hubDriverInstnceInfo->interruptInPipeHandle  )
     {
           /* Let the host know that this interface cannot be processed */
        _USB_HOST_HUB_InterfaceRelease(interfaceHandle);
            /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_HUB_InterfaceAssign : inerrupt pipe is notcreated ");
        return ;

     }

     /* Ready for launch interrupt transfer  */
     USB_HOST_DeviceTransfer( hubDriverInstnceInfo->interruptInPipeHandle,
        &transferHandle,&( hubDriverInstnceInfo->bufferStatus ), 1 , hubIndex );

     /* Get Hub descriptor state */
     hubDriverInstnceInfo->hubInstanceState = 
                                    USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE;
   
  }

   // *****************************************************************************
/* Function:
    void _USB_HOST_HUB_InterfaceRelease
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
    )

  Summary:
    This function is called when the Host Layer detaches this driver from an
    interface.

  
  Remarks:
    This is a local function and should not be called directly by the
    application.
*/
 void _USB_HOST_HUB_InterfaceRelease 
  ( 
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle 
  )
  {
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo;
    hubDriverInstanceInfo = _USB_HOST_HUB_FindInstanceInfo( interfaceHandle);

    if ( NULL == hubDriverInstanceInfo )
    {
            /* Assert on invalid HUB index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_HUB_InterfaceRelease : Invalid HUB instance number " );
        return ;
    }
    /* Denumerate all connected devices */
    _USB_HOST_HubRemoveParentIsRootHub( hubDriverInstanceInfo);
    /* Interface release */
    USB_HOST_DeviceInterfaceRelease( interfaceHandle );
    if ( hubDriverInstanceInfo->interruptInPipeHandle != 
                                    USB_HOST_PIPE_HANDLE_INVALID )
    {
        /* Close the pipe */
        USB_HOST_DevicePipeClose(hubDriverInstanceInfo->interruptInPipeHandle);
        hubDriverInstanceInfo->interruptInPipeHandle = 
                                        USB_HOST_PIPE_HANDLE_INVALID;
    }
    /* Free the hub instance */
    hubDriverInstanceInfo->inUse = false;
    
    
  }
// *****************************************************************************
/* Function:
  USB_HOST_RESULT _USB_HOST_HUB_GetConfigDescriptor
 ( 
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo 
 )

  Summary:
   To find the Configuration descriptor 
  
  Remarks:
    This is a local function and should not be called directly by the
    application.
*/
  
 USB_HOST_RESULT _USB_HOST_HUB_GetConfigDescriptor
 ( 
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo 
 )
  {
     USB_HOST_RESULT result;
     
     result = USB_HOST_RESULT_SUCCESS;
      // By default configuration , 
      //the correct value when the device level driver implemented 
      return result;
      
  }


// *****************************************************************************
/* Function:
USB_HOST_RESULT _USB_HOST_HUB_GetHubDescriptor
(   
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo
)

  Summary:
   To find the Number of ports available in this hub 
  
  Remarks:
    This is a local function and should not be called directly by the
    application.
*/
USB_HOST_RESULT _USB_HOST_HUB_GetHubDescriptor
(   
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo
)
{
    USB_HOST_RESULT  result;
    USB_HOST_CONTROL_PIPE_HANDLE  pipeHandle;
    USB_HOST_TRANSFER_HANDLE     transferHandle;
    USB_SETUP_PACKET * setupPacket;
    void * data;
    uintptr_t context;
    USB_HOST_DEVICE_CONTROL_REQUEST_COMPLETE_CALLBACK callback;

  
    setupPacket = ( USB_SETUP_PACKET *)& ( hubDriverInstnceInfo->setupPacket[0]);
    /* Fill setup packet */
    setupPacket->bRequest = 0x06 ;
    setupPacket->bmRequestType = 0xA0;
    setupPacket->wLength = 0x09 ; // TODO depends on hubdesc size
    setupPacket->wIndex = 0x00;
    setupPacket->wValue = ( 0x29 << 8 );
  
    data = & ( hubDriverInstnceInfo->hubDescriptor );
    callback = ( USB_HOST_DEVICE_CONTROL_REQUEST_COMPLETE_CALLBACK )
                                _USB_HOST_HUB_controlTransferComplete;
    context = hubDriverInstnceInfo->instanceNumber;
    pipeHandle = hubDriverInstnceInfo->controlPipeHandle;
    hubDriverInstnceInfo->controlRequest = USB_HOST_HUB_GET_HUB_DESCRIPTOR;
    result =  USB_HOST_DeviceControlTransfer ( pipeHandle , &transferHandle ,
                                 setupPacket, data , callback , context );
    return result;
}

// *****************************************************************************
/* Function:
 void  _USB_HOST_HUB_controlTransferComplete
  (
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    USB_HOST_REQUEST_HANDLE requestHandle,
    USB_HOST_RESULT result,
    uint32_t size,
    uintptr_t context
  )

  Summary:
   Call back function for control transfer complete ...
  
  Remarks:
    
*/

 void  _USB_HOST_HUB_controlTransferComplete
  (
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    USB_HOST_REQUEST_HANDLE requestHandle,
    USB_HOST_RESULT result,
    uint32_t size,
    uintptr_t context
  )
{
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo;
    uint32_t   hubIndex;
    uint32_t   portIndex;

    portIndex =  ( context & PORT_INDEX_MASK ) >> 16 ;
    hubIndex =   ( context & HUB_INDEX_MASK );
    hubDriverInstanceInfo = &(gUSBHostHubObj[hubIndex]);

    if ( result != USB_HOST_RESULT_SUCCESS)
    {
       hubDriverInstanceInfo->hubInstanceState = USB_HOST_HUB_ERROR;
       return;
    }

    switch ( hubDriverInstanceInfo->controlRequest )
    {
        case USB_HOST_HUB_GET_CONFIG_DESCRIPTOR:
            hubDriverInstanceInfo->hubInstanceState = USB_HOST_HUB_GET_CONFIG_DESCRIPTOR_STATE_COMPLETE;
            break;
        case USB_HOST_HUB_GET_HUB_DESCRIPTOR:
            hubDriverInstanceInfo->hubInstanceState =  USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE_COMPLETE;
            break;
        case USB_HOST_HUB_STATUS_REQUEST:
            hubDriverInstanceInfo->hubInstanceState =  USB_HOST_HUB_STATUS_STATE_COMPLETE;
            break;
        case USB_HOST_HUB_PORT_STATUS_REQUEST:
            hubDriverInstanceInfo->portInfo[portIndex].portState = USB_HOST_HUB_PORT_STATUS_STATE_COMPLETE;
            break;
        case USB_HOST_HUB_PORT_POWER_ENABLE_REQUEST:
            hubDriverInstanceInfo->portInfo[portIndex].portState = USB_HOST_HUB_PORT_POWER_ENABLE_STATE_COMPLETE;
            break;
        case USB_HOST_HUB_PORT_RESET:
            hubDriverInstanceInfo->portInfo[portIndex].portState = USB_HOST_HUB_PORT_RESET_STATE_COMPLETE;
            break;
        case USB_HOST_HUB_PORT_SUSPEND:
            hubDriverInstanceInfo->portInfo[portIndex].portState = USB_HOST_HUB_PORT_SUSPEND_STATE_COMPLETE;
            break;
        case USB_HOST_HUB_CLEAR_POET_RESET:
            hubDriverInstanceInfo->portInfo[portIndex].portState = USB_HOST_HUB_CLEAR_PORT_RESET_STATE_COMPLETE;
            break;
        case USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS:
            hubDriverInstanceInfo->portInfo[portIndex].portSubState = USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS_COMPLETED;
            break;
        case  USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_REQUEST:
            hubDriverInstanceInfo->portInfo[portIndex].portSubState = USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_COMPLETE;
            break;
        case USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_COMPLETE_REQUEST:
            hubDriverInstanceInfo->portInfo[portIndex].portSubState = USB_HOST_HUB_PORT_STATUS_CHANGED_PORT_ENABLED_COMPLETE;
            break;
        case  USB_HOST_HUB_CLEAR_PORT_RESET_STATE_REQUEST:
            hubDriverInstanceInfo->portInfo[portIndex].portState = USB_HOST_HUB_CLEAR_PORT_RESET_STATE_COMPLETE;
            break;
        default :
            break;
      }
}

// *****************************************************************************
/* Function:
int _USB_HOST_HUB_InterfaceHandleToHUBInstance
( 
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)

  Summary:
     To find instance number
  
  Remarks:
    
*/
  
  
int _USB_HOST_HUB_InterfaceHandleToHUBInstance
( 
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)
{
    int iterator;
    int hubInstanceInfo = -1;

    /* Find the HUB Instance object that owns this interface */
    for (iterator = 0; iterator < USB_HOST_HUB_INSTANCES_NUMBER; iterator ++)
    {
        if(gUSBHostHubObj[iterator].inUse)
        {
            if(gUSBHostHubObj[iterator].hubInterfaceHandle == interfaceHandle)
            {
                /* Found it */
                hubInstanceInfo = iterator;
                break;
            }
        }
    }
    return(hubInstanceInfo);
}
  
  
// *****************************************************************************
/* Function:
 USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE _USB_HOST_HUB_InterfaceEventHandler
  (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle,
        USB_HOST_DEVICE_INTERFACE_EVENT event,
        void * eventData,
        uintptr_t context
  )
  Summary:
 Handle the interface events...
  
  Remarks:
    
*/
USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE _USB_HOST_HUB_InterfaceEventHandler
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle,
    USB_HOST_DEVICE_INTERFACE_EVENT event,
    void * eventData,
    uintptr_t context
)
{
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo;
        
    USB_HOST_DEVICE_INTERFACE_EVENT_TRANSFER_COMPLETE_DATA * transferCompleteEventData;
    
    /* Find instance driver info */
    hubDriverInstanceInfo = _USB_HOST_HUB_FindInstanceInfo( interfaceHandle);
    switch ( event )
    {
        case USB_HOST_DEVICE_INTERFACE_EVENT_TRANSFER_COMPLETE:
            /* This means a transfer completed. Update the transfer state
             * machine. */
            transferCompleteEventData = (USB_HOST_DEVICE_INTERFACE_EVENT_TRANSFER_COMPLETE_DATA *)(eventData);
            _USB_HOST_HUB_TransferTasks (hubDriverInstanceInfo, transferCompleteEventData->result, transferCompleteEventData->length);
            break;

        case USB_HOST_DEVICE_INTERFACE_EVENT_SET_INTERFACE_COMPLETE:
            break;

        case USB_HOST_DEVICE_INTERFACE_EVENT_PIPE_HALT_CLEAR_COMPLETE:
                hubDriverInstanceInfo->standardRequestDone = true;
            break;
             
        default:
            break;
      }

 return(USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE_NONE);
  }

// *****************************************************************************
/* Function:
void _USB_HOST_HUB_TransferTasks
(
    USB_HOST_HUB_DRIVER_INFO * hubDriverInstanceInfo,
    USB_HOST_RESULT result,
    size_t size
)
  Summary:
 Event Transfer State machine 
  
  Remarks:
    
*/

void _USB_HOST_HUB_TransferTasks
(
    USB_HOST_HUB_DRIVER_INFO * hubDriverInstanceInfo,
    USB_HOST_RESULT result,
    size_t size
)

{
    uint32_t   dataChanged;
    if(result == USB_HOST_RESULT_SUCCESS)
    {
        dataChanged =  hubDriverInstanceInfo->bufferStatus ;
        //No status change 
        if ( dataChanged == 0x00)
        {
            return;
        }
        /* check hub status changed ?*/
        else if ( dataChanged & 0x1 )
        {
            hubDriverInstanceInfo->hubStatusChange = true ;
        }
        /* clear the hub status */
        dataChanged =  dataChanged & ~0x01;
        /* validate the any port status changed */
        if ( dataChanged )
        {
            _USB_HOST_HUB_PortStatusChanged(hubDriverInstanceInfo , dataChanged );
        }
    }
    else
    {
        // keep track of the errors best ..may be some other errors may come 
        /* Check if the hub is removed */
        _USB_HOST_HubRemove( hubDriverInstanceInfo);
    }
}

 void  _USB_HOST_HubStatusChanged (USB_HOST_HUB_DRIVER_INFO * hubDriverInstanceInfo )
 {
     /* This feature will be implemented in next release */
 }

// *****************************************************************************
/* Function:
 void _USB_HOST_HUB_PortStatusChanged
 ( 
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo , 
    uint32_t  dataChanged 
 )
  Summary:
 When ever the port status changed will call this function 
  
  Remarks:
    
*/
 void _USB_HOST_HUB_PortStatusChanged
 ( 
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo , 
    uint32_t  dataChanged 
 )
{
    uint32_t  portNumber;
    uint32_t  mask= 1;
    mask = mask << 1;
    for ( portNumber = 1 ; 
          portNumber <= hubDriverInstanceInfo->hubDescriptor.bNbrPorts ;
          portNumber ++ , mask = mask << 1 )
    {
        if( ( dataChanged & mask ) == 0x00)
        {
            continue;      // means this port no status change
        }
        else
        {
            hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                        USB_HOST_HUB_PORT_STATUS_CHANGED ;
            hubDriverInstanceInfo->portInfo[portNumber].portSubState = 
                                    USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS;
            hubDriverInstanceInfo->portInfo[portNumber].portStatusChanged = true ;
            hubDriverInstanceInfo->transferRequestReceived = true ;
        }
    }
}

// *****************************************************************************
/* Function:
void _USB_HOST_HUB_InterfaceTasks 
( 
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle 
)
  Summary:
   Hub Task routine will be called by Host layer  
  
  Remarks:
    
*/

void _USB_HOST_HUB_InterfaceTasks 
( 
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle 
)
{
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo;
    uint32_t      portNumber;
    uint32_t      hubIndex;
    USB_HOST_TRANSFER_HANDLE transferHandle;
    hubDriverInstanceInfo = _USB_HOST_HUB_FindInstanceInfo( interfaceHandle);
    _USB_HOST_UpdateHubState(hubDriverInstanceInfo);
      
    if ( hubDriverInstanceInfo->transferRequestReceived )
    {
        for ( portNumber = 1 ; portNumber <= hubDriverInstanceInfo->numberOfPorts ; )
        {
            if ( hubDriverInstanceInfo->portInfo[portNumber].portState !=  USB_HOST_HUB_PORT_READY_STATE)
            {
                break;
            }
            else
            {
                portNumber++ ;
                if ( portNumber >  hubDriverInstanceInfo->numberOfPorts  )
                {
                    //hub index 
                    hubIndex = _USB_HOST_HUB_InterfaceHandleToHUBInstance( interfaceHandle );
                    hubDriverInstanceInfo->transferRequestReceived = false;
                    USB_HOST_DeviceTransfer( hubDriverInstanceInfo->interruptInPipeHandle, &transferHandle,&( hubDriverInstanceInfo->bufferStatus ), 1 , hubIndex );
                }
                       
            }
        }
    }
}

// *****************************************************************************
/* Function:
 void _USB_HOST_UpdateHubState ( USB_HOST_HUB_DRIVER_INFO * hubDriverInstanceInfo )
 
  Summary:
   Hub Task routine will be called by Host layer  
  
  Remarks:
    
*/
void _USB_HOST_UpdateHubState ( USB_HOST_HUB_DRIVER_INFO * hubDriverInstanceInfo )
{
    USB_HOST_RESULT  result;
    USB_CONFIGURATION_DESCRIPTOR *configDescriptor;
    uint8_t         selfPoweredHub;
    uint32_t        portNumber;
    
    switch ( hubDriverInstanceInfo->hubInstanceState )
    {
        case USB_HOST_HUB_GET_CONFIG_DESCRIPTOR_STATE :
            result = _USB_HOST_HUB_GetConfigDescriptor( hubDriverInstanceInfo );
            if ( result == USB_HOST_RESULT_SUCCESS )
            {
                hubDriverInstanceInfo->hubInstanceState = 
                            USB_HOST_HUB_GET_CONFIG_DESCRIPTOR_WAIT_STATE;
            }
            else if ( result == USB_HOST_RESULT_REQUEST_BUSY )
            {
                hubDriverInstanceInfo->hubInstanceState = USB_HOST_HUB_GET_CONFIG_DESCRIPTOR_STATE;
            }
            else
            {
                hubDriverInstanceInfo->hubInstanceState = USB_HOST_HUB_ERROR;
            }
            break;
        case  USB_HOST_HUB_GET_CONFIG_DESCRIPTOR_WAIT_STATE:
            break;
        case USB_HOST_HUB_GET_CONFIG_DESCRIPTOR_STATE_COMPLETE:
            configDescriptor = hubDriverInstanceInfo->configDescriptor;
            /* check if the hub is a self powered or bus powered    */
            selfPoweredHub = ((configDescriptor->bmAttributes) & 0x40);
            if(selfPoweredHub)
            {
                /* No need to give power for self powered hubs */
                hubDriverInstanceInfo->power = 0;
            }
            else
            {
                hubDriverInstanceInfo->power = 200; /* as per spec 7.2*/
            }
            hubDriverInstanceInfo->hubInstanceState = 
                                USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE;
            break;
        case USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE:
            result =  _USB_HOST_HUB_GetHubDescriptor( hubDriverInstanceInfo );
            if ( result == USB_HOST_RESULT_SUCCESS )
            {
                hubDriverInstanceInfo->hubInstanceState = 
                                USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE_WAIT;
            }
            else if ( result == USB_HOST_RESULT_REQUEST_BUSY )
            {
                hubDriverInstanceInfo->hubInstanceState = USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE;
            }
            else
            {
                hubDriverInstanceInfo->hubInstanceState = USB_HOST_HUB_ERROR;
            }
            break;
        case USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE_WAIT:
            break;
        case USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE_COMPLETE:
            hubDriverInstanceInfo->numberOfPorts = hubDriverInstanceInfo->hubDescriptor.bNbrPorts;
            hubDriverInstanceInfo->hubInstanceState = USB_HOST_HUB_STATUS_STATE ;
            break;
        case USB_HOST_HUB_STATUS_STATE :
            hubDriverInstanceInfo->hubInstanceState = USB_HOST_HUB_STATUS_STATE_WAIT ;
            hubDriverInstanceInfo->controlRequest = USB_HOST_HUB_STATUS_REQUEST;
            USB_HOST_HUB_statusRequest( hubDriverInstanceInfo, 
                    & (hubDriverInstanceInfo->hubStatus),0 ,USB_HOST_HUB_REQUEST );
            break;
        case USB_HOST_HUB_STATUS_STATE_WAIT:
            break;
              
        case USB_HOST_HUB_STATUS_STATE_COMPLETE:
            hubDriverInstanceInfo->hubInstanceState = USB_HOST_HUB_STATE_RUNNING ;
            for ( portNumber = 1 ; portNumber <= hubDriverInstanceInfo->numberOfPorts ; portNumber++)
            {
                hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                        USB_HOST_HUB_PORT_STATUS_STATE  ;
            }
            break;
        case USB_HOST_HUB_STATE_RUNNING :
            if ( hubDriverInstanceInfo->hubStatusChange == true )
            {
                _USB_HOST_HubStatusChanged (hubDriverInstanceInfo );
            }
            _USB_HOST_HUB_PORT_UpdateState( hubDriverInstanceInfo);
            break;
        case USB_HOST_HUB_ERROR:
            break;
        default:
            break;
    }
}
 // *****************************************************************************
/* Function:
 void _USB_HOST_HUB_PORT_UpdateState( USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo)
 
  Summary:
   Hub Port Update state 
  
  Remarks:
    
*/

void _USB_HOST_HUB_PORT_UpdateState( USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo)
{
    uint32_t          numberOfPorts;
    uint32_t          portNumber;
    uint32_t          portState;
    USB_HOST_HUB_STATUS          portStatus;
    USB_HOST_HUB_DRIVER_INFO *hubInfo;
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle;
    /* Number of Ports */
    numberOfPorts = hubDriverInstanceInfo->numberOfPorts;
    for ( portNumber = 1 ; portNumber <= numberOfPorts ;  )
    {
        portState = hubDriverInstanceInfo->portInfo[portNumber].portState ;
        switch( portState )
        {
            case USB_HOST_HUB_PORT_STATUS_STATE :
                hubDriverInstanceInfo->controlRequest = 
                                        USB_HOST_HUB_PORT_STATUS_REQUEST;
                USB_HOST_HUB_statusRequest( hubDriverInstanceInfo, 
                        &(hubDriverInstanceInfo->portInfo[portNumber].portStatus),
                        portNumber ,USB_HOST_HUB_PORT_REQUEST );
                hubDriverInstanceInfo->portInfo[portNumber].portState = 
                        USB_HOST_HUB_PORT_STATUS_STATE_WAIT;
                break;
            case USB_HOST_HUB_PORT_STATUS_STATE_WAIT :
                break;
            case USB_HOST_HUB_PORT_STATUS_STATE_COMPLETE:
                portStatus = hubDriverInstanceInfo->portInfo[portNumber].portStatus;
                if ( hubDriverInstanceInfo->portInfo[portNumber].portPowered == false )
                {
                    hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                            USB_HOST_HUB_PORT_POWER_ENABLE_STATE;
                }
                else
                {
                    if ((portStatus.change & USB_HOST_HUB_BIT_C_PORT_RESET) &&
                            (portStatus.status & USB_HOST_HUB_BIT_PORT_ENABLE))
                    {
                         /* Reset signaling done */
                         /* clear feature */
                        hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                USB_HOST_HUB_CLEAR_PORT_RESET_STATE_WAIT;
                        hubDriverInstanceInfo->controlRequest = 
                                    USB_HOST_HUB_CLEAR_PORT_RESET_STATE_REQUEST;
                        _USB_HOST_HUB_featureRequest(hubDriverInstanceInfo , 
                                portNumber , HUB_FEATURE_C_PORT_RESET , 
                                USB_HOST_HUB_COMMAND_CLEAR_FEATURE ,
                                USB_HOST_HUB_PORT_REQUEST );
                         
                    }
                    else
                    {
                        hubDriverInstanceInfo->portInfo[portNumber].portState =
                                                USB_HOST_HUB_PORT_READY_STATE;
                    }
                }
                // check any overcurrent ?
                break;
            case USB_HOST_HUB_CLEAR_PORT_RESET_STATE_WAIT:
                break;

            case USB_HOST_HUB_CLEAR_PORT_RESET_STATE_COMPLETE:
                hubDriverInstanceInfo->portInfo[portNumber].portResetCompleted = true;
                hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                            USB_HOST_HUB_PORT_READY_STATE;
                break;
            case  USB_HOST_HUB_PORT_POWER_ENABLE_STATE:
                hubDriverInstanceInfo->controlRequest = USB_HOST_HUB_PORT_POWER_ENABLE_REQUEST;
                _USB_HOST_HUB_featureRequest(hubDriverInstanceInfo ,
                        portNumber , USB_HOST_HUB_PORT_POWER , 
                        USB_HOST_HUB_COMMAND_SET_FEATURE , 
                        USB_HOST_HUB_PORT_REQUEST );
                hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                USB_HOST_HUB_PORT_POWER_ENABLE_STATE_WAIT;
                break;

            case USB_HOST_HUB_PORT_POWER_ENABLE_STATE_WAIT:
                break;

            case USB_HOST_HUB_PORT_POWER_ENABLE_STATE_COMPLETE:
                hubDriverInstanceInfo->portInfo[portNumber].portPowered = true;
                hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                        USB_HOST_HUB_PORT_READY_STATE;
                break;
            case  USB_HOST_HUB_PORT_RESET_STATE :
                break;
            case USB_HOST_HUB_PORT_RESET_STATE_WAIT:
                break;
            case USB_HOST_HUB_PORT_RESET_STATE_COMPLETE:
                if ( hubDriverInstanceInfo->portInfo[portNumber].portResetCompleted != true)
                {
                    hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                    USB_HOST_HUB_PORT_RESET_STATE_DELAY;
                }
                else
                {
                    hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                            USB_HOST_HUB_PORT_READY_STATE;
                }
                break;
            case USB_HOST_HUB_PORT_RESET_STATE_DELAY:
                hubDriverInstanceInfo->portResetTimerHandle = SYS_TMR_DelayMS(50);
                hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                    USB_HOST_HUB_PORT_RESET_STATE_DELAY_WAIT;
                break;
            case USB_HOST_HUB_PORT_RESET_STATE_DELAY_WAIT:
                if(SYS_TMR_DelayStatusGet( hubDriverInstanceInfo->portResetTimerHandle ))
                {
                    hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                        USB_HOST_HUB_PORT_STATUS_STATE;
                }
                break;
            case USB_HOST_HUB_PORT_SUSPEND_STATE:
                break;
            case USB_HOST_HUB_PORT_SUSPEND_STATE_WAIT:
                break;
            case USB_HOST_HUB_PORT_SUSPEND_STATE_COMPLETE:
                hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                            USB_HOST_HUB_PORT_READY_STATE;
                break;
            case USB_HOST_HUB_PORT_STATUS_CHANGED:
                switch (hubDriverInstanceInfo->portInfo[portNumber].portSubState)
                {
                    case USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS :
                        hubDriverInstanceInfo->controlRequest =
                                  USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS;
                        hubDriverInstanceInfo->portInfo[portNumber].portSubState = 
                                USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS_WAIT;
                        USB_HOST_HUB_statusRequest( hubDriverInstanceInfo, 
                                &(hubDriverInstanceInfo->portInfo[portNumber].portStatus),
                                portNumber ,USB_HOST_HUB_PORT_REQUEST );

                        break;

                    case USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS_WAIT:
                        break;

                    case USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS_COMPLETED:
                        if ( hubDriverInstanceInfo->portInfo[portNumber].portStatus.change & USB_HOST_HUB_BIT_C_PORT_CONNECTION )
                        {
                            /* we have to clear the event... */
                            hubDriverInstanceInfo->portInfo[portNumber].portSubState =
                                    USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT;
                        }
                        break;
                    case USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT:
                        hubDriverInstanceInfo->controlRequest = 
                                USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_REQUEST;
                        hubDriverInstanceInfo->portInfo[portNumber].portSubState = 
                                USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_WAIT ;
                        _USB_HOST_HUB_featureRequest( hubDriverInstanceInfo, 
                                portNumber ,USB_HOST_HUB_FEATURE_C_PORT_CONNECTION , 
                                USB_HOST_HUB_COMMAND_CLEAR_FEATURE ,
                                USB_HOST_HUB_PORT_REQUEST  );
                        break;
                    case USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_WAIT:
                        break;
                    case USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_COMPLETE :
                        if ( hubDriverInstanceInfo->portInfo[portNumber].portStatus.status & USB_HOST_HUB_BIT_PORT_CONNECTION )
                        {
                            /* new device Connected */
                            /* TODO  we have to find the depth here
                              ? how to get parent info */
                            deviceObjHandle = USB_HOST_DeviceEnumerate( hubDriverInstanceInfo->hubObjHandle, portNumber) ;
                            hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle = deviceObjHandle;
                            hubDriverInstanceInfo->portInfo[portNumber].portSubState = USB_HOST_HUB_PORT_ENUMERATION_WAIT;
                        }
                        else
                        {
                            hubDriverInstanceInfo->controlRequest = 
                                    USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_COMPLETE_REQUEST;
                            _USB_HOST_HUB_featureRequest( hubDriverInstanceInfo, portNumber, 
                                    USB_HOST_HUB_FEATURE_PORT_ENABLE ,
                                    USB_HOST_HUB_COMMAND_CLEAR_FEATURE ,
                                    USB_HOST_HUB_PORT_REQUEST   );
                            hubDriverInstanceInfo->portInfo[portNumber].portSubState =
                                    USB_HOST_HUB_PORT_STATUS_CHANGED_PORT_ENABLED_WAIT;
                        }
                        break;
                    case USB_HOST_HUB_PORT_ENUMERATION_WAIT:
                        break;

                    case USB_HOST_HUB_PORT_STATUS_CHANGED_PORT_ENABLED_WAIT:
                        break;

                    case  USB_HOST_HUB_PORT_STATUS_CHANGED_PORT_ENABLED_COMPLETE:
                        
                        /* deleted device is hub */
                        hubInfo =  _USB_HOST_findDriverInfo( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle );
                        if ( hubInfo == NULL )
                        {
                            USB_HOST_DeviceDenumerate( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle );
							hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;
                        }
                        else
                        {
                            _USB_HOST_HubRemove( hubInfo );
                        }
                        hubDriverInstanceInfo->portInfo[portNumber].portState = 
                                        USB_HOST_HUB_PORT_READY_STATE;
                        hubDriverInstanceInfo->portInfo[portNumber].portSubState = 
                                            USB_HOST_HUB_PORT_DEENUMERATION_WAIT;
                        break;
                          
                    case USB_HOST_HUB_PORT_DEENUMERATION_WAIT :
                        break;
                    default:
                        break;
                }
            case USB_HOST_HUB_PORT_READY_STATE:
                       // portNumber++ ;
                break;
            default:
                break;
        }
        if( hubDriverInstanceInfo->portInfo[portNumber].portState == USB_HOST_HUB_PORT_READY_STATE )
        {
            portNumber++;
        }
        else
        {
            break;
        }
    }
}

// *****************************************************************************
/* Function:
 void _USB_HOST_HubRemoveParentIsRootHub ( USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo )
 
  Summary:
   Remove the hub which is connected to the roothub and deenumerate all connected devices 
  
  Remarks:
    
*/
  
void _USB_HOST_HubRemoveParentIsRootHub ( USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo )
{
    uint32_t        portNumber;
    USB_HOST_HUB_DRIVER_INFO *hubInfo;
    /* traverse till the devices found */
    for ( portNumber = 1 ; portNumber <= hubDriverInstanceInfo->hubDescriptor.bNbrPorts ; portNumber++  )
    {
        hubDriverInstanceInfo->portInfo[portNumber].portPowered = false;
        if ( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle == 
                                         ( USB_HOST_DEVICE_OBJ_HANDLE )  NULL )
        {
            continue;
        }
        hubInfo =  _USB_HOST_findDriverInfo( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle );
        if ( hubInfo == NULL )
        {
            if ( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle != 
                    ( USB_HOST_DEVICE_OBJ_HANDLE )NULL )
            {
                USB_HOST_DeviceDenumerate( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle );
				hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;
            }
        }
        else
        {
            _USB_HOST_HubRemove( hubInfo );
        }
    }
}

  // *****************************************************************************
/* Function:
 void _USB_HOST_HubRemove ( USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo )
 
  Summary:
 * Remove the hub if the parent device is not root hub
  
  Remarks:
    
*/

void _USB_HOST_HubRemove ( USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo )
{
    uint32_t        portNumber;
    USB_HOST_HUB_DRIVER_INFO *hubInfo;
    /* traverse till the devices found */
    for ( portNumber = 1 ; portNumber <= hubDriverInstanceInfo->hubDescriptor.bNbrPorts ; portNumber++  )
    {
        hubDriverInstanceInfo->portInfo[portNumber].portPowered = false;
        if ( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle == 
                ( USB_HOST_DEVICE_OBJ_HANDLE )  NULL )
        {
            continue;
        }
        hubInfo =  _USB_HOST_findDriverInfo( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle );
        if ( hubInfo == NULL )
        {
            if ( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle != 
                                        ( USB_HOST_DEVICE_OBJ_HANDLE )NULL )
            {
                USB_HOST_DeviceDenumerate( hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle );
				hubDriverInstanceInfo->portInfo[portNumber].deviceObjHandle = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;
            }
        }
        else
        {
            _USB_HOST_HubRemove( hubInfo );
        }
    }
    /* Now remove the hub */
    USB_HOST_DeviceDenumerate( hubDriverInstanceInfo->hubObjHandle );
	hubDriverInstanceInfo->hubObjHandle = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;
}


  /*******************************************************************/
  void _USB_HOST_HubTask( USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo)
  {

      
  }
  // *****************************************************************************
/* Function:
USB_HOST_RESULT  _USB_HOST_HUB_featureRequest 
(
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo,
    uint32_t portNumber ,
    uint8_t Feature, 
    uint8_t hubCommand ,
    uint8_t requestType
)
 
  Summary:

 Hub feature Request for set or clear 
  
  Remarks:
    
*/

USB_HOST_RESULT  _USB_HOST_HUB_featureRequest 
(
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo,
    uint32_t portNumber ,
    uint8_t Feature, 
    uint8_t hubCommand ,
    uint8_t requestType
)
{
    USB_HOST_RESULT  result;
    USB_HOST_CONTROL_PIPE_HANDLE  pipeHandle;
    USB_HOST_TRANSFER_HANDLE     transferHandle;
    USB_SETUP_PACKET * setupPacket;
    uint32_t data;
    uintptr_t context;
    USB_HOST_DEVICE_CONTROL_REQUEST_COMPLETE_CALLBACK callback;

    setupPacket = ( USB_SETUP_PACKET * )& ( hubDriverInstanceInfo->setupPacket[0]);

    if ( requestType == USB_HOST_HUB_PORT_REQUEST )
    {
        setupPacket->bmRequestType = 0x23;
    }
    if (requestType == USB_HOST_HUB_REQUEST )
    {
        setupPacket->bmRequestType = 0x20;
        portNumber = 0x0;
    }
    setupPacket->bRequest = hubCommand;
    setupPacket->wLength = 0x00;
    setupPacket->wValue = Feature;
    setupPacket->wIndex = portNumber;

    callback = (USB_HOST_DEVICE_CONTROL_REQUEST_COMPLETE_CALLBACK)_USB_HOST_HUB_controlTransferComplete;
    context = ( portNumber << 16) + hubDriverInstanceInfo->instanceNumber;
    pipeHandle = hubDriverInstanceInfo->controlPipeHandle;
    result =  USB_HOST_DeviceControlTransfer ( pipeHandle ,
                 &transferHandle , setupPacket, &data , callback , context );
    return result;
}

  // *****************************************************************************
/* Function:
USB_HOST_RESULT USB_HOST_HUB_statusRequest 
( 
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo, 
    USB_HOST_HUB_STATUS  *hubStatus ,
    uint8_t portNumber , 
    uint8_t requestType 
)
 
  Summary:

 Hub & Port status Request 
  
  Remarks:
    
*/

USB_HOST_RESULT USB_HOST_HUB_statusRequest 
( 
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo, 
    USB_HOST_HUB_STATUS  *hubStatus ,
    uint8_t portNumber , 
    uint8_t requestType 
)
{
    USB_HOST_RESULT  result;
    USB_HOST_CONTROL_PIPE_HANDLE  pipeHandle;
    USB_HOST_TRANSFER_HANDLE     transferHandle;
    USB_SETUP_PACKET * setupPacket;
    uintptr_t context;
    USB_HOST_DEVICE_CONTROL_REQUEST_COMPLETE_CALLBACK callback;
    setupPacket = ( USB_SETUP_PACKET * )& ( hubDriverInstnceInfo->setupPacket[0]);
    if ( requestType == USB_HOST_HUB_PORT_REQUEST )
    {
        setupPacket->bmRequestType = 0xA3;
    }
    if (requestType == USB_HOST_HUB_REQUEST )
    {
        setupPacket->bmRequestType = 0xA0;
        portNumber = 0x0;
    }
    setupPacket->bRequest = USB_HOST_HUB_COMMAND_GET_STATUS;
    setupPacket->wLength = 0x04 ;
    setupPacket->wIndex = portNumber;
    setupPacket->wValue = 0x00;

    callback = ( USB_HOST_DEVICE_CONTROL_REQUEST_COMPLETE_CALLBACK ) _USB_HOST_HUB_controlTransferComplete;
    context = ( portNumber << 16 ) + hubDriverInstnceInfo->instanceNumber;
    pipeHandle = hubDriverInstnceInfo->controlPipeHandle;
    result =  USB_HOST_DeviceControlTransfer ( pipeHandle , &transferHandle ,
                              setupPacket, hubStatus , callback , context );
    return result;
}

  // *****************************************************************************
/* Function:
USB_HOST_HUB_DRIVER_INFO * _USB_HOST_HUB_FindInstanceInfo
( 
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)
 
  Summary:

 Find the Hub Driver instance information 
  
  Remarks:
    
*/
USB_HOST_HUB_DRIVER_INFO * _USB_HOST_HUB_FindInstanceInfo
( 
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)
{
    USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo;
    uint32_t      hubCount;
    for ( hubCount = 0 ; hubCount < USB_HOST_HUB_INSTANCES_NUMBER ; hubCount++)
    {
        hubDriverInstnceInfo = &(gUSBHostHubObj[hubCount]);
        if( hubDriverInstnceInfo->hubInterfaceHandle == interfaceHandle )
        {
            return hubDriverInstnceInfo;
        }
    }
    return NULL ; 
}
  // *****************************************************************************
/* Function:
USB_HOST_HUB_DRIVER_INFO * _USB_HOST_findDriverInfo( uintptr_t deviceHandle )
 
  Summary:

 Find the Hub Driver instance information if device handle is available 
  
  Remarks:
    
*/
USB_HOST_HUB_DRIVER_INFO * _USB_HOST_findDriverInfo( uintptr_t deviceHandle )
{

    USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo;
    uint32_t      hubCount;
    for ( hubCount = 0 ; hubCount < USB_HOST_HUB_INSTANCES_NUMBER ; hubCount++)
    {
        hubDriverInstnceInfo = &(gUSBHostHubObj[hubCount]);
        if( hubDriverInstnceInfo->hubObjHandle == deviceHandle )
        {
            return hubDriverInstnceInfo;
        }
    }
    return NULL ;
}

/******************** End of the file *************************************/