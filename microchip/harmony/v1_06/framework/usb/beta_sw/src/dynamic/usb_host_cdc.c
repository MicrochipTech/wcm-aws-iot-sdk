/*******************************************************************************
 USB HOST CDC Class Driver

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_cdc.c

  Summary:
    USB CDC Host class driver.

  Description:
    USB Host CDC class driver.
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
 ******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "usb/beta_sw/usb_host.h"
#include "usb/beta_sw/src/usb_host_local.h"
#include "usb/beta_sw/usb_host_cdc.h"
#include "usb/beta_sw/src/usb_host_cdc_local.h"
#include "usb/beta_sw/usb_cdc.h"
#include "system/debug/sys_debug.h"


// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* CDC Host Instance structure

  Summary:
    Defines the CDC instance(s).

  Description:
    This data type defines the CDC instance(s). The number of instances is
    defined by the application using USB_HOST_CDC_INSTANCES_NUMBER.

  Remarks:
    This structure is private to the CDC.
 */
USB_HOST_CDC_INSTANCE  gUSBHostCDCInstance[USB_HOST_CDC_INSTANCES_NUMBER];

// *****************************************************************************
/* CDC Host driver structure

  Summary:
    Defines the Host driver structure required for the host layer.

  Description:
    This data type defines the host driver structure required for the
    host layer.

  Remarks:
    This structure is private to the USB Host stack.
 */
USB_HOST_CLASS_DRIVER cdcDriver =
{
    /*name of the driver */
    .initializeDeviceDriver = NULL ,
    .initializeInterfaceDriver = _USB_HOST_CDC_InterfaceInitialize ,
    .deInitializeDriver = _USB_HOST_CDC_DeInitialize ,
    .task           = _USB_HOST_CDC_Task
 };


/*************************************************************************/

/* Function:
   USB_ERROR      _USB_HOST_CDC_InterfaceInitialize(   USB_HOST_DEVICE_ID id ,
                                                       uint8_t interfaceId,
                                                       uint8_t *cdcinstance,
						       USB_SPEED speed)

  Summary:
    Initialize the CDC host driver.

  Description:
    After enumeration (set_Configuration )Host will initialize the CDC driver

  Parameters:
     USB_HOST_DEVICE_ID                           id ,
     uint8_t                                interfaceId,
     uint8_t *                              cdcinstance

  Returns:
    None
*/
 USB_ERROR      _USB_HOST_CDC_InterfaceInitialize(     USB_HOST_DEVICE_ID id ,
                                                       uint8_t interfaceId,
                                                       uint8_t *cdcInstance,
						       USB_SPEED speed)
{
    USB_HOST_QUERY  cdcQuery;
    USB_ERROR  status;
    USB_INTERFACE_DESCRIPTOR *comInterfaceDesc;
    USB_INTERFACE_DESCRIPTOR *dataInterfaceDesc;
    USB_ENDPOINT_DESCRIPTOR *bulkInEndpointDesc ,
                            *bulkOutEndpointDesc ,
                            *interruptInEndpointDesc ;
    USB_ENDPOINT_DESCRIPTOR controlEndpointDesc;
    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    uint8_t     count;

    status = USB_ERROR_NONE;
    /* Find the free space for CDC instance*/
    for(count = 0 ; count < USB_HOST_CDC_INSTANCES_NUMBER ; count ++)
    {
        cdcInstanceInfo = &(gUSBHostCDCInstance[count]);
        if (cdcInstanceInfo->assigned == false )
            break;
    }
    /* Validate the instance*/
    if ( count >= USB_HOST_CDC_INSTANCES_NUMBER)
    {
        /* Assert on invalid CDC index */
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
         "_USB_HOST_CDC_InterfaceInitialize : Invalid CDC instance number \r\n " );
        return USB_ERROR_HOST_DRIVER_INSTANCE_INVALID;
    }
    
    *cdcInstance = count;
    cdcInstanceInfo->assigned = true;
    cdcInstanceInfo->cdcDeviceId = id;
    cdcInstanceInfo->instanceNumber = count;
    cdcInstanceInfo->interfaceNumber = interfaceId;
    cdcQuery.interfaceNumber = interfaceId ;
    cdcQuery.id = id;
    /* active configuration number will change after*/
    cdcQuery.configurationNumber = 0;
    cdcQuery.class = USB_CDC_CLASS_CODE;
    cdcQuery.subClass = USB_CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL;
    cdcQuery.protocol = USB_CDC_PROTOCOL_AT_V250;
    /* class match */
    cdcQuery.flags = USB_HOST_MATCH_CLASS_SUBCLASS_PROTOCOL;
    cdcQuery.alternateSettingNumber = 0;
    /* find alternate setting of interface */
    status = _USB_HOST_FindInterface(&(cdcQuery), &(comInterfaceDesc));

    if ( status != USB_ERROR_NONE )
    {
         SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
                 "_USB_HOST_CDC_InterfaceInitialize : Interface Not found  \r\n" );
    }

    cdcInstanceInfo->communicationInterfaceNumber = comInterfaceDesc->bInterfaceNumber ;
    cdcQuery.class = USB_CDC_DATA_INTERFACE_CLASS_CODE;
    cdcQuery.subClass = USB_CDC_DATA_INTERFACE_SUBCLASS_CODE;
    cdcQuery.protocol = USB_CDC_DATA_INTERFACE_PROTOCOL;
    /* class match */
    cdcQuery.flags = USB_HOST_MATCH_CLASS_SUBCLASS_PROTOCOL;

    status = _USB_HOST_FindInterface(&(cdcQuery), &(dataInterfaceDesc));
    cdcInstanceInfo->datainterfaceNumber = dataInterfaceDesc->bInterfaceNumber;


    /* Find the Interrupt IN endpoint address */
    cdcQuery.endpointAddress = 0 ; // we have to get temp
    cdcQuery.endpointType = USB_TRANSFER_TYPE_INTERRUPT;
    cdcQuery.endpointDirection = USB_DATA_DIRECTION_DEVICE_TO_HOST ;
    cdcQuery.flags = USB_HOST_MATCH_ENDPOINT_TYPE | USB_HOST_MATCH_ENDPOINT_DIRECTION;
    cdcQuery.interfaceNumber = cdcInstanceInfo->communicationInterfaceNumber;
    /* Find the Interrupt endpoint descriptor */
    status = _USB_HOST_FindEndpoint (&(cdcQuery), &(interruptInEndpointDesc));
    /* Validate the Endpoint is available */
    if( status != USB_ERROR_NONE )
    {
       SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
       "_USB_HOST_CDC_InterfaceInitialize : Interrupt endpoint not found \r\n" );
        return USB_ERROR_HOST_ENDPOINT_INVALID;
    }
    /* Create Interrupt pipe */
    cdcInstanceInfo->interruptInPipeHandle = _USB_HOST_PipeSetup (
                                id ,  interruptInEndpointDesc ,speed );

    if( cdcInstanceInfo->interruptInPipeHandle == USB_HOST_PIPE_HANDLE_INVALID )
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
       "_USB_HOST_CDC_InterfaceInitialize : Interrupt pipe is not created \r\n" );
        
        return USB_ERROR_HOST_PIPE_INVALID;
    }
    /* Fill Interrupt IRP*/
    /* Notification callback function */
    cdcInstanceInfo->interruptInIRP.callback =
                                    _USB_HOST_CDC_InterruptInTransferComplete;
    /* Interrupt endpoint maxpacket size */
    cdcInstanceInfo->interruptInIRP.size =
                                    interruptInEndpointDesc->wMaxPacketSize;
    /*To identify the instanceInfo in callback functions */
    cdcInstanceInfo->interruptInIRP.userData = (uintptr_t)cdcInstanceInfo;

    /* Get control pipe information */
    controlEndpointDesc.bmAttributes =  USB_TRANSFER_TYPE_CONTROL;
    cdcInstanceInfo->controlPipeHandle =  _USB_HOST_PipeSetup
                                ( id ,  &controlEndpointDesc , speed );
    if(cdcInstanceInfo->controlPipeHandle == USB_HOST_PIPE_HANDLE_INVALID )
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
       "_USB_HOST_CDC_InterfaceInitialize : Control pipe is not created \r\n" );
        return USB_ERROR_HOST_PIPE_INVALID;
    }
    /* Fill Control IRP */
    cdcInstanceInfo->controlIRP.callback =
                                    _USB_HOST_CDC_ControlTransferComplete;
    /*find CDC instance in callback functions */
    cdcInstanceInfo->controlIRP.userData = (uintptr_t)cdcInstanceInfo;

    /* Fine bulkIn endpoint descriptor */
    cdcQuery.endpointAddress = 0 ; // we have to get temp
    cdcQuery.endpointType = USB_TRANSFER_TYPE_BULK;
    cdcQuery.endpointDirection = USB_DATA_DIRECTION_DEVICE_TO_HOST;
    cdcQuery.interfaceNumber = cdcInstanceInfo->datainterfaceNumber;
    cdcQuery.flags = USB_HOST_MATCH_ENDPOINT_TYPE | USB_HOST_MATCH_ENDPOINT_DIRECTION;
    status = _USB_HOST_FindEndpoint (&(cdcQuery), &(bulkInEndpointDesc));
     /* Validate the Endpoint is available */
    if( status != USB_ERROR_NONE )
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
        "_USB_HOST_CDC_InterfaceInitialize : Bulk in endpoint not found \r\n" );
        return USB_ERROR_HOST_ENDPOINT_INVALID;
    }
    /* Create Bulkin pipe */
    cdcInstanceInfo->bulkInPipeHandle = _USB_HOST_PipeSetup
                                ( id ,  bulkInEndpointDesc , speed );
    /* validate BulkInPipe handle*/
    if( cdcInstanceInfo->bulkInPipeHandle == USB_HOST_PIPE_HANDLE_INVALID )
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
        "_USB_HOST_CDC_InterfaceInitialize : Bulk IN pipe is not created \r\n" );
        return USB_ERROR_HOST_PIPE_INVALID;
    }
    /* Fill Bulkin IRP */
    cdcInstanceInfo->bulkInIRP.callback =
                            (void * )_USB_HOST_CDC_BulkInTransferComplete;
    cdcInstanceInfo->bulkInIRP.size = bulkInEndpointDesc->wMaxPacketSize ;
    /*Instance info required in callback function */
    cdcInstanceInfo->bulkInIRP.userData = (uintptr_t)cdcInstanceInfo;
     /* Fill the cdcQuery to find bulkOut endpoint */
 
    cdcQuery.endpointAddress = 0 ; // we have to get temp
    cdcQuery.endpointType = USB_TRANSFER_TYPE_BULK;
    cdcQuery.endpointDirection = USB_DATA_DIRECTION_HOST_TO_DEVICE;
    cdcQuery.flags = USB_HOST_MATCH_ENDPOINT_TYPE | USB_HOST_MATCH_ENDPOINT_DIRECTION;
    /* Find the bulkout endpoint descriptor */
    status = _USB_HOST_FindEndpoint (&(cdcQuery), &(bulkOutEndpointDesc));
      if( status != USB_ERROR_NONE )
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
        "_USB_HOST_CDC_InterfaceInitialize : Bulk out endpoint not found \r\n" );
        return USB_ERROR_HOST_ENDPOINT_INVALID;
    }
    /* Create Bulkout pipe */
    cdcInstanceInfo->bulkOutPipeHandle = _USB_HOST_PipeSetup
                                ( id ,  bulkOutEndpointDesc ,speed);
      /* validate BulkOutPipe handle*/
    if( cdcInstanceInfo->bulkOutPipeHandle == USB_HOST_PIPE_HANDLE_INVALID )
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
        "_USB_HOST_CDC_InterfaceInitialize : Bulk Out pipe is not created \r\n" );
        return USB_ERROR_HOST_PIPE_INVALID;
    }
    /* Fill Bulk out piep */
    cdcInstanceInfo->bulkOutIRP.callback =
                                    _USB_HOST_CDC_BulkOutTransferComplete;
    cdcInstanceInfo->bulkOutIRP.size = bulkOutEndpointDesc->wMaxPacketSize ;
    /*Instance info for callback function */
    cdcInstanceInfo->bulkOutIRP.userData = (uintptr_t )cdcInstanceInfo;

  
    /* Sent attach event to the application */
    cdcInstanceInfo->appEventCallBack(cdcInstanceInfo->instanceNumber,
                                      USB_HOST_CDC_EVENT_ATTACH,
                                      &(cdcInstanceInfo->cdcDeviceId),
                                        cdcInstanceInfo->context);
    return status;
}

/*************************************************************************/

/* Function:
   void  _USB_HOST_CDC_DeInitialize(USB_HOST_DEVICE_ID id )

  Summary:
    DeInitialize the CDC host driver.

  Description:
    After deenumeration Host will Deinitialize the CDC driver

  Parameters:
     USB_HOST_DEVICE_ID                           id ,

  Returns:
        void
*/

void  _USB_HOST_CDC_DeInitialize ( USB_HOST_DEVICE_ID id  )
 {
      USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
      /* Find CDC instance */
      uint8_t   instanceNumber;

      instanceNumber = _USB_HOST_CDC_findInstance( id );
       /* CDC Driver info */
      cdcInstanceInfo = &(gUSBHostCDCInstance[instanceNumber]);

      if(cdcInstanceInfo->assigned == false)
      {
           SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "_USB_HOST_CDC_DeInitialize : Instance Already closed \r\n" );
           return;
      }
      /* inform to the cdc device detach event to the application */
      cdcInstanceInfo->appEventCallBack(cdcInstanceInfo->instanceNumber,
                                USB_HOST_CDC_EVENT_DETACH,
                                &(cdcInstanceInfo->cdcDeviceId),
                                  cdcInstanceInfo->context);

      /* Close CDC device related pipes */
      _USB_HOST_PipeClose( cdcInstanceInfo->bulkInPipeHandle );
     
      _USB_HOST_PipeClose( cdcInstanceInfo->bulkOutPipeHandle );
      _USB_HOST_PipeClose( cdcInstanceInfo->interruptInPipeHandle );
      /* close the instance */
      cdcInstanceInfo->assigned = false ;
 }


USB_ERROR USB_HOST_CDC_EventHandlerSet
    (
        USB_HOST_CDC_EVENT_HANDLER appCDCHandler
    )
{

    USB_HOST_CDC_INDEX        CDCInstance;
    USB_HOST_CDC_INSTANCE    *cdcInstnceInfo;

    /* validate callback handler */
    if( NULL == appCDCHandler )
    {
        return USB_ERROR_HOST_POINTER_INVALID;
    }
    /* check if the given event handler is valid */
   
    for ( CDCInstance = 0 ; CDCInstance < USB_HOST_CDC_INSTANCES_NUMBER ; CDCInstance++ )
    {
        cdcInstnceInfo = &(gUSBHostCDCInstance[CDCInstance]);
            /* update the event handler for this instance */
        cdcInstnceInfo->appEventCallBack = appCDCHandler ;
    }
    /* return success */
    return USB_ERROR_NONE;
}
// ***********************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_Read(USB_HOST_CDC_INDEX index,
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, void * dest,
        size_t length);

  Summary:
    This function reads data from the attached CDC device.

  Description:
    This function reads data from the attached CDC device. The function will
    schedule a read transfer. If successful, the transferHandle parameter will
    contain a valid transfer handle, else it will contain
    USB_HOST_CDC_TRANSFER_HANDLE_INVALID. When completed, the CDC class driver
    will generate a USB_HOST_CDC_EVENT_READ_COMPLETE event. Multiple read
    requests can be queued. In such a case, the transfer handle for each request
    will be unique.

  Precondition:
    None.

  Parameters:
    index - CDC Class Driver Instance Index where the request should be scheduled.

    transferHandle  - Pointer to USB_HOST_CDC_TRANSFER_HANDLE type of a variable.
                      This will contain a valid transfer handle if the request
                      was successful.

    dest            - pointer to the buffer where the read data will be stored.

    length          - Amount of data to read (in bytes).

  Returns:
    USB_HOST_CDC_RESULT_OK - The operation was successful. transferHandle will
    contain a valid transfer handle.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_INVALID - The specified instance does
    not exist.
    USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL - The transfer queue is full
    and the requested transfer cannot be scheduled.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY - The specified instance does
    not have a CDC device associated with it. This can happen if the attached
    device was disconnected or the read function was called with an CDC class
    driver instance that was not attached to a CDC device.
    USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID - The destination buffer pointer
    is NULL or the length parameter is zero.

  Example:
    <code>
    // The following code snippet shows an example of scheduling a CDC Class
    // Driver read. In this example, the class driver reads 64 bytes from the
    // attached device.

    USB_HOST_CDC_TRANSFER_HANDLE transferHandle;
    USB_HOST_CDC_RESULT result;
    uint8_t buffer[64];

    result = USB_HOST_CDC_Read(USB_HOST_CDC_0, &transferHandle, buffer, 64);

    if(USB_HOST_CDC_RESULT_OK != result)
    {
        // Error handling here
    }

    // The completion of the read request can be tracked by using the
    // USB_HOST_CDC_EVENT_READ_COMPLETE.

    </code>

  Remarks:
    None.

 */

USB_HOST_CDC_RESULT USB_HOST_CDC_Read
( 
    USB_HOST_CDC_INDEX index,
    USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
    void * destination,
    size_t length
)
{
    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    USB_HOST_IRP *irp;
    USB_ERROR status;

    if ( index >= USB_HOST_CDC_INSTANCES_NUMBER)
    {
        /* Assert on invalid CDC index */
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_Read : Invalid CDC instance number \r\n" );
        return USB_ERROR_HOST_DRIVER_INSTANCE_INVALID;
    }

    if( destination == NULL || length == 0x00)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_Read : Invalid prameters \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID;
    }
    /* Get CDC driver info*/
    cdcInstanceInfo = &gUSBHostCDCInstance[index];

    if(cdcInstanceInfo->assigned == false)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_Read : Invalid prameters \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY;
    }
    /* IRP*/
    irp = &(cdcInstanceInfo->bulkInIRP);
    /* Transfer handle to get the data*/
    *transferHandle = (USB_HOST_CDC_TRANSFER_HANDLE  )
                        &(cdcInstanceInfo->bulkInIRP);

    /* Fill IRP*/
    irp->data = destination;
    irp->size = length;
    irp->callback = (void *) _USB_HOST_CDC_BulkInTransferComplete;
    irp->userData = (uintptr_t) cdcInstanceInfo;
    /* Set CDC states for to Submit IRP */
    status = _USB_HOST_IRPSubmit(cdcInstanceInfo->bulkInPipeHandle,
            &(cdcInstanceInfo->bulkInIRP));
    if (status != USB_ERROR_NONE)
    {
        return USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL;
    }
    /* success */
    return USB_HOST_CDC_RESULT_OK;
}

// *****************************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_Write(USB_HOST_CDC_INDEX index,
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, void * source,
        size_t length);

  Summary:
    This function writes data to the attached CDC device.

  Description:
    This function writes data to the attached CDC device. The function will
    schedule a write transfer. If successful, the transferHandle parameter will
    contain a valid transfer handle, else it will contain
    USB_HOST_CDC_TRANSFER_HANDLE_INVALID. When completed, the CDC class driver
    will generate a USB_HOST_CDC_EVENT_WRITE_COMPLETE event. Multiple write
    requests can be queued. In such a case, the transfer handle for each request
    will be unique.

  Precondition:
    None.

  Parameters:
    index - CDC Class Driver Instance Index where the request should be scheduled.

    transferHandle  - Pointer to USB_HOST_CDC_TRANSFER_HANDLE type of a variable.
                      This will contain a valid transfer handle if the request
                      was successful.

    source          - pointer to the buffer containing data to be written to the
                      device.

    length          - Amount of data to written (in bytes).

  Returns:
    USB_HOST_CDC_RESULT_OK - The operation was successful. transferHandle will
    contain a valid transfer handle.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_INVALID - The specified instance does
    not exist.
    USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL - The transfer queue is full
    and the requested transfer cannot be scheduled.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY - The specified instance does
    not have a CDC device associated with it. This can happen if the attached
    device was disconnected or the write function was called with an CDC class
    driver instance that was not attached to a CDC device.
    USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID - The source buffer pointer
    is NULL or the length parameter is zero.

  Example:
    <code>
    // The following code snippet shows an example of scheduling a CDC Class
    // Driver Write. In this example, the class driver writes 64 bytes to the
    // attached device.

    USB_HOST_CDC_TRANSFER_HANDLE transferHandle;
    USB_HOST_CDC_RESULT result;
    uint8_t buffer[64];

    result = USB_HOST_CDC_Write(USB_HOST_CDC_0, &transferHandle, buffer, 64);

    if(USB_HOST_CDC_RESULT_OK != result)
    {
        // Error handling here
    }

    // The completion of the write request can be tracked by using the
    // USB_HOST_CDC_EVENT_WRITE_COMPLETE.

    </code>

  Remarks:
    None.

*/

USB_HOST_CDC_RESULT USB_HOST_CDC_Write (
                                  USB_HOST_CDC_INDEX index,
                                  USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
                                  void * source, size_t length)
{
     USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
     USB_HOST_IRP *irp;
     USB_ERROR status;

    if ( index >= USB_HOST_CDC_INSTANCES_NUMBER)
    {
        /* Assert on invalid CDC index */
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_Write : Invalid CDC instance number \r\n" );
        return USB_ERROR_HOST_DRIVER_INSTANCE_INVALID;
    }

    if( source == NULL || length == 0x00)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_Write : Invalid prameters \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID;
    }

    /* Get CDC driver info*/
    cdcInstanceInfo = &gUSBHostCDCInstance[index];
  
    if(cdcInstanceInfo->assigned == false)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_Write : Invalid prameters \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY;
    }
    irp = &(cdcInstanceInfo->bulkOutIRP);
    *transferHandle = (USB_HOST_CDC_TRANSFER_HANDLE  )
                        &(cdcInstanceInfo->bulkOutIRP);
   
    /* Fill IRP */
    irp->data = source;
    irp->size = length;
    irp->callback = _USB_HOST_CDC_BulkOutTransferComplete;
    irp->userData = (uintptr_t)cdcInstanceInfo;
     /* Set CDC Task states for to Submit IRP */
    status = _USB_HOST_IRPSubmit(cdcInstanceInfo->bulkOutPipeHandle,
                                &(cdcInstanceInfo->bulkOutIRP));
     if(status != USB_ERROR_NONE )
     {
         return USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL;
     }
    return USB_HOST_CDC_RESULT_OK;
}
// *****************************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_LineCodingGet(USB_HOST_CDC_INDEX index,
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
        USB_CDC_LINE_CODING * lineCoding);

  Summary:
    This function requests line coding from the attached CDC device.

  Description:
    This function requests line coding from the attached CDC device. The
    function schedules a GET LINE CODING control transfer.  If successful, the
    transferHandle parameter will contain a valid transfer handle, else it will
    contain USB_HOST_CDC_TRANSFER_HANDLE_INVALID. When completed, the CDC class
    driver will generate a USB_HOST_CDC_EVENT_GET_LINE_CODING_COMPLETE event.
    When completed, the lineCoding parameter will contain the line coding. The
    CDC Class driver does not support queuing of control transfers. If a control
    transfer is in progress, this function will return with an error.

  Precondition:
    None.

  Parameters:
    index - CDC Class Driver Instance Index where the request should be scheduled.

    transferHandle  - Pointer to USB_HOST_CDC_TRANSFER_HANDLE type of a variable.
                      This will contain a valid transfer handle if the request
                      was successful.

    lineCoding      - pointer to the buffer where the line coding data will be
                      stored.

  Returns:
    USB_HOST_CDC_RESULT_OK - The operation was successful. transferHandle will
    contain a valid transfer handle.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_INVALID - The specified instance does
    not exist.
    USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL - Another control transfer is
    in progress and this transfer cannot be scheduled.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY - The specified instance does
    not have a CDC device associated with it. This can happen if the attached
    device was disconnected or the get request was called with an CDC class
    driver instance that was not attached with a CDC device.
    USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID - The lineCoding buffer pointer
    is NULL.

  Example:
    <code>
    // The following code snippet shows an example of scheduling a CDC Class
    // Driver Get Line Coding Request.

    USB_HOST_CDC_TRANSFER_HANDLE transferHandle;
    USB_HOST_CDC_RESULT result;
    USB_CDC_LINE_CODING * buffer;

    result = USB_HOST_CDC_LineCodingGet(USB_HOST_CDC_0,
                                        &transferHandle, buffer);

    if(USB_HOST_CDC_RESULT_OK != result)
    {
        // Error handling here
    }

    // The completion of the get request can be tracked by using the
    // USB_HOST_CDC_EVENT_GET_LINE_CODING_COMPLETE event.

    </code>

  Remarks:
    None.

*/
USB_HOST_CDC_RESULT USB_HOST_CDC_LineCodingGet ( USB_HOST_CDC_INDEX index,
                                USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
                                USB_CDC_LINE_CODING * lineCoding)
{
     USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
     USB_ERROR  status;

   if ( index >= USB_HOST_CDC_INSTANCES_NUMBER)
    {
        /* Assert on invalid CDC index */
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_LineCodingGet : Invalid CDC instance number \r\n" );
        return USB_ERROR_HOST_DRIVER_INSTANCE_INVALID;
    }
    if (lineCoding == NULL) 
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_LineCodingGet : Invalid Parameter \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID;
    }
    /* Get CDC driver info*/
    cdcInstanceInfo = &gUSBHostCDCInstance[index];

    if (cdcInstanceInfo->assigned == false)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_LineCodingGet : Invalid Parameter \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY;
    }
    /* Transfer handle for Application*/
    *transferHandle = (USB_HOST_CDC_TRANSFER_HANDLE )
            &(cdcInstanceInfo->controlIRP);

    cdcInstanceInfo->setupPacket.bmRequestType  = ( USB_SETUP_DIRN_DEVICE_TO_HOST |
                                                  USB_SETUP_TYPE_CLASS |
                                                  USB_SETUP_RECIPIENT_INTERFACE ); //interface , Host to device , class
    cdcInstanceInfo->setupPacket.bRequest =  USB_CDC_GET_LINE_CODING ;

    cdcInstanceInfo->setupPacket.wValue = 0x0000;

    cdcInstanceInfo->setupPacket.wIndex = cdcInstanceInfo->communicationInterfaceNumber;

    cdcInstanceInfo->setupPacket.wLength = USB_CDC_LINE_CODING_LENGTH;

   
    /* Data Buffer for get line code */
    cdcInstanceInfo->controlIRP.data = (uint8_t *) lineCoding;
    cdcInstanceInfo->controlIRP.setup = &( cdcInstanceInfo->setupPacket );
    cdcInstanceInfo->controlIRP.size = USB_CDC_LINE_CODING_LENGTH;
    /* set the CDC Task states for submit IRP*/
    cdcInstanceInfo->controlRequest = USB_CDC_GET_LINE_CODING;
    status = _USB_HOST_IRPSubmit(cdcInstanceInfo->controlPipeHandle,
            &(cdcInstanceInfo->controlIRP));
    if (status != USB_ERROR_NONE) {
        return USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL;
    }
    return USB_HOST_CDC_RESULT_OK;
}

// *****************************************************************************

/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_LineCodingSet(USB_HOST_CDC_INDEX index,
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
        USB_CDC_LINE_CODING * lineCoding);

  Summary:
    This function sets the line coding of the attached CDC device.

  Description:
    This function sets the line coding of the attached CDC device. The function
    schedules a SET LINE CODING control transfer.  If successful, the
    transferHandle parameter will contain a valid transfer handle, else it will
    contain USB_HOST_CDC_TRANSFER_HANDLE_INVALID. When completed, the CDC class
    driver will generate a USB_HOST_CDC_EVENT_SET_LINE_CODING_COMPLETE event.
    The lineCoding parameter contain the line coding to be sent to the attached
    device. The CDC Class driver does not support queuing of control transfers.
    If a control transfer is in progress, this function will return with an
    error.

  Precondition:
    None.

  Parameters:
    index - CDC Class Driver Instance Index where the request should be scheduled.

    transferHandle  - Pointer to USB_HOST_CDC_TRANSFER_HANDLE type of a variable.
                      This will contain a valid transfer handle if the request
                      was successful.

    lineCoding      - pointer to the buffer containing line coding data to be
                      sent to the device

  Returns:
    USB_HOST_CDC_RESULT_OK - The operation was successful. transferHandle will
    contain a valid transfer handle.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_INVALID - The specified instance does
    not exist.
    USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL - Another control transfer is
    in progress and this transfer cannot be scheduled.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY - The specified instance does
    not have a CDC device associated with it. This can happen if the attached
    device was disconnected or the set request was called with an CDC class
    driver instance that was not attached with a CDC device.
    USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID - The lineCoding buffer pointer
    is NULL.

  Example:
    <code>
    // The following code snippet shows an example of scheduling a CDC Class
    // Driver Set Line Coding Request.

    USB_HOST_CDC_TRANSFER_HANDLE transferHandle;
    USB_HOST_CDC_RESULT result;
    USB_CDC_LINE_CODING * buffer;

    result = USB_HOST_CDC_LineCodingSet(USB_HOST_CDC_0,
                                        &transferHandle, buffer);

    if(USB_HOST_CDC_RESULT_OK != result)
    {
        // Error handling here
    }

    // The completion of the set request can be tracked by using the
    // USB_HOST_CDC_EVENT_SET_LINE_CODING_COMPLETE event.

    </code>

  Remarks:
    None.

 */


USB_HOST_CDC_RESULT USB_HOST_CDC_LineCodingSet(USB_HOST_CDC_INDEX index,
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
        USB_CDC_LINE_CODING * lineCoding)
{
    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    USB_ERROR status;

   if ( index >= USB_HOST_CDC_INSTANCES_NUMBER)
    {
        /* Assert on invalid CDC index */
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_LineCodingSet : Invalid CDC instance number \r\n" );
        return USB_ERROR_HOST_DRIVER_INSTANCE_INVALID;
    }
    if (lineCoding == NULL) 
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_LineCodingSet : Invalid Parameter \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID;
    }
    /* Get CDC driver info*/
    cdcInstanceInfo = &gUSBHostCDCInstance[index];
    
    if (cdcInstanceInfo->assigned == false) 
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_LineCodingSet : Invalid Parameter \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY;
    }
    /* Transfer Handle*/
    *transferHandle = (USB_HOST_CDC_TRANSFER_HANDLE )
            &(cdcInstanceInfo->controlIRP);


    cdcInstanceInfo->setupPacket.bmRequestType  = ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                                                  USB_SETUP_TYPE_CLASS |
                                                  USB_SETUP_RECIPIENT_INTERFACE ); //interface , Host to device , class
    cdcInstanceInfo->setupPacket.bRequest =  USB_CDC_SET_LINE_CODING ;

    cdcInstanceInfo->setupPacket.wValue = ( 0x01 << 8 );

    cdcInstanceInfo->setupPacket.wIndex = cdcInstanceInfo->communicationInterfaceNumber;

    cdcInstanceInfo->setupPacket.wLength = USB_CDC_LINE_CODING_LENGTH;


     /* Fill control irp */
    cdcInstanceInfo->controlIRP.data = (uint8_t *) lineCoding;
    cdcInstanceInfo->controlIRP.setup = & ( cdcInstanceInfo->setupPacket );
    cdcInstanceInfo->controlIRP.size = USB_CDC_LINE_CODING_LENGTH;

    cdcInstanceInfo->controlRequest = USB_CDC_SET_LINE_CODING;
    /* Set CDC states for submit IRP */
    status = _USB_HOST_IRPSubmit(cdcInstanceInfo->controlPipeHandle,
            &(cdcInstanceInfo->controlIRP));
    if (status != USB_ERROR_NONE) {
        return USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL;
    }
    return USB_HOST_CDC_RESULT_OK;
}

// *****************************************************************************

/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_ControlLineStateSet (
                        USB_HOST_CDC_INDEX index,
                        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
                        USB_CDC_CONTROL_LINE_STATE * controlLineState)
  Summary:
    This function sets the control line state of the attached CDC device.

  Description:
    This function sets the control line state of the attached CDC device.
    The function schedules a SET CONTROL LINE control transfer.  If successful, the
    transferHandle parameter will contain a valid transfer handle, else it will
    contain USB_HOST_CDC_TRANSFER_HANDLE_INVALID. When completed, the CDC class
    driver will generate a USB_HOST_CDC_EVENT_SET_CONTROL_LINE_STATE_COMPLETE event.
    The lineCoding parameter contain the line coding to be sent to the attached
    device. The CDC Class driver does not support queuing of control transfers.
    If a control transfer is in progress, this function will return with an
    error.

  Precondition:
    None.

  Parameters:
    index - CDC Class Driver Instance Index where the request should be scheduled.

    transferHandle  - Pointer to USB_HOST_CDC_TRANSFER_HANDLE type of a variable.
                      This will contain a valid transfer handle if the request
                      was successful.

    lineCoding      - pointer to the buffer containing line coding data to be
                      sent to the device

  Returns:
    USB_HOST_CDC_RESULT_OK - The operation was successful. transferHandle will
    contain a valid transfer handle.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_INVALID - The specified instance does
    not exist.
    USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL - Another control transfer is
    in progress and this transfer cannot be scheduled.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY - The specified instance does
    not have a CDC device associated with it. This can happen if the attached
    device was disconnected or the set request was called with an CDC class
    driver instance that was not attached with a CDC device.
    USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID - The lineCoding buffer pointer
    is NULL.

  Example:
    <code>
    // The following code snippet shows an example of scheduling a CDC Class
    // Driver Set Line Coding Request.

    USB_HOST_CDC_TRANSFER_HANDLE transferHandle;
    USB_HOST_CDC_RESULT result;
    USB_CDC_CONTROL_LINE_STATE * controlLineState;

    result = USB_HOST_CDC_ControlLineStateSet(USB_HOST_CDC_0,
                                        &transferHandle, controlLineState);

    if(USB_HOST_CDC_RESULT_OK != result)
    {
        // Error handling here
    }

    // The completion of the set request can be tracked by using the
    // USB_HOST_CDC_EVENT_SET_LINE_CODING_COMPLETE event.

    </code>

  Remarks:
    None.

 */
USB_HOST_CDC_RESULT USB_HOST_CDC_ControlLineStateSet(USB_HOST_CDC_INDEX index,
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
        USB_CDC_CONTROL_LINE_STATE * controlLineState)
{
    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    USB_ERROR status;

   if ( index >= USB_HOST_CDC_INSTANCES_NUMBER)
    {
        /* Assert on invalid CDC index */
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_ControlLineStateSet : Invalid CDC instance number \r\n" );
        return USB_ERROR_HOST_DRIVER_INSTANCE_INVALID;
    }
    if (controlLineState == NULL)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_ControlLineStateSet : Invalid Parameter \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID;
    }
    /* Get CDC driver info*/
    cdcInstanceInfo = &gUSBHostCDCInstance[index];
    if (cdcInstanceInfo->assigned == false)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_ControlLineStateSet : Invalid Parameter \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY;
    }
    *transferHandle = (USB_HOST_CDC_TRANSFER_HANDLE )
            &(cdcInstanceInfo->bulkOutIRP);

     cdcInstanceInfo->setupPacket.bmRequestType  = ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                                                  USB_SETUP_TYPE_CLASS |
                                                  USB_SETUP_RECIPIENT_INTERFACE ); //interface , Host to device , class
    cdcInstanceInfo->setupPacket.bRequest =  USB_CDC_SET_CONTROL_LINE_STATE ;

    cdcInstanceInfo->setupPacket.wValue =  (( *(uint8_t *) controlLineState ) << 8 );

    cdcInstanceInfo->setupPacket.wIndex = cdcInstanceInfo->communicationInterfaceNumber;

    cdcInstanceInfo->setupPacket.wLength = USB_CDC_CONTROL_LINE_LENGTH;

     /* Fill irp */
    cdcInstanceInfo->controlIRP.setup = &( cdcInstanceInfo->setupPacket );
    cdcInstanceInfo->controlIRP.size = USB_CDC_CONTROL_LINE_LENGTH;
    /* Set CDC Task state for submit IRP */
    cdcInstanceInfo->controlRequest = USB_CDC_SET_CONTROL_LINE_STATE;
    status = _USB_HOST_IRPSubmit(cdcInstanceInfo->controlPipeHandle,
            &(cdcInstanceInfo->controlIRP));
    if (status != USB_ERROR_NONE) {
        return USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL;
    }
    return USB_HOST_CDC_RESULT_OK;
}

/**********************************************************************/
USB_HOST_CDC_RESULT USB_HOST_CDC_SerialStateNotificationGet
                (USB_HOST_CDC_INDEX index,
                 USB_HOST_CDC_TRANSFER_HANDLE *transferHandle,
                 USB_HOST_CDC_SERIAL_STATE_NOTIFICATION_DATA * dest)
{
    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    USB_ERROR status;

   if ( index >= USB_HOST_CDC_INSTANCES_NUMBER)
    {
        /* Assert on invalid CDC index */
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_SerialStateNotificationGet : Invalid CDC instance number \r\n" );
        return USB_ERROR_HOST_DRIVER_INSTANCE_INVALID;
    }
    if (dest == NULL)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_SerialStateNotificationGet : Invalid Parameter \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID;
    }
    /* Get CDC driver info*/
    cdcInstanceInfo = &gUSBHostCDCInstance[index];

   if (cdcInstanceInfo->assigned == false)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_SerialStateNotificationGet : Invalid Parameter \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY;
    }
    *transferHandle = (USB_HOST_CDC_TRANSFER_HANDLE )
            &(cdcInstanceInfo->interruptInIRP);
    cdcInstanceInfo->interruptInIRP.data = (uint8_t *) dest;
   
    cdcInstanceInfo->interruptInIRP.size = USB_HOST_CDC_SERIAL_NOTIFICATION_SIZE ;
    /* Set CDC Task state for submit IRP */

    status = _USB_HOST_IRPSubmit(cdcInstanceInfo->interruptInPipeHandle,
            &(cdcInstanceInfo->interruptInIRP));
    if (status != USB_ERROR_NONE)
    {
        return USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL;
    }
    /* Support Next release */
    return USB_HOST_CDC_RESULT_OK;
}


/*******************************************************************/
USB_HOST_CDC_RESULT USB_HOST_CDC_BreakSend(USB_HOST_CDC_INDEX index,
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
        uint16_t breakDuration)
{
    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    USB_ERROR status;

   if ( index >= USB_HOST_CDC_INSTANCES_NUMBER)
    {
        /* Assert on invalid CDC index */
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_BreakSend : Invalid CDC instance number \r\n" );
        return USB_ERROR_HOST_DRIVER_INSTANCE_INVALID;
    }
    /* Get CDC driver info*/
    cdcInstanceInfo = &gUSBHostCDCInstance[index];

    if (cdcInstanceInfo->assigned == false)
    {
        SYS_DEBUG_MESSAGE( SYS_ERROR_INFO ,
           "USB_HOST_CDC_BreakSend : Invalid prameters \r\n" );
        return USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY;
    }
    /* Transfer Handle*/
    *transferHandle = (USB_HOST_CDC_TRANSFER_HANDLE )
            &(cdcInstanceInfo->controlIRP);


     cdcInstanceInfo->setupPacket.bmRequestType  = ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                                                  USB_SETUP_TYPE_CLASS |
                                                  USB_SETUP_RECIPIENT_INTERFACE ); //interface , Host to device , class
    cdcInstanceInfo->setupPacket.bRequest =  USB_CDC_SEND_BREAK ;

    cdcInstanceInfo->setupPacket.wValue =  ( breakDuration << 8 );

    cdcInstanceInfo->setupPacket.wIndex = cdcInstanceInfo->communicationInterfaceNumber;

    cdcInstanceInfo->setupPacket.wLength = USB_CDC_CONTROL_LINE_LENGTH;


  
    /* Fill irp */
    cdcInstanceInfo->controlIRP.setup = & ( cdcInstanceInfo->setupPacket );

    /* Set CDC Task state for submit IRP */
    cdcInstanceInfo->controlRequest = USB_CDC_SEND_BREAK;
    status = _USB_HOST_IRPSubmit(cdcInstanceInfo->controlPipeHandle,
                                    &(cdcInstanceInfo->controlIRP));

    if (status != USB_ERROR_NONE) {
        return USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL;
    }
    return USB_HOST_CDC_RESULT_OK;
}


/*************************************************************************/
/* Function:
   _USB_HOST_CDC_ControlTransferComplete( USB_HOST_IRP *irp)

  Summary:
        Control transfer data callback
  Description:
      This function handles Controltransfer data events
  Parameters:
      USB_HOST_IRP *irp

  Returns:
   void
 */
void _USB_HOST_CDC_ControlTransferComplete(USB_HOST_IRP *irp)
{
    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    USB_HOST_CDC_EVENT_DATA_SET_LINE_CODING_COMPLETE setLineCodingEventData;
    USB_HOST_CDC_EVENT_DATA_GET_LINE_CODING_COMPLETE getLineCodingEventData;
    USB_HOST_CDC_EVENT_DATA_SET_CONTROL_LINE_STATE_COMPLETE
                                                setControlLineStateEventData;

    cdcInstanceInfo = ((USB_HOST_CDC_INSTANCE *) (irp->userData));

    if (irp->status == USB_HOST_IRP_STATUS_COMPLETED ||
            irp->status == USB_HOST_IRP_STATUS_COMPLETED_SHORT)
    {
        switch (cdcInstanceInfo->controlRequest)
        {
            case USB_CDC_GET_LINE_CODING:
                getLineCodingEventData.length = irp->size;
                getLineCodingEventData.transferHandle =
                                            (USB_HOST_CDC_TRANSFER_HANDLE) irp;
                getLineCodingEventData.transferStatus =
                                            USB_HOST_CDC_TRANSFER_STATUS_OK;
                cdcInstanceInfo->appEventCallBack(
                        cdcInstanceInfo->instanceNumber,
                        USB_HOST_CDC_EVENT_GET_LINE_CODING_COMPLETE,
                        &getLineCodingEventData,
                        cdcInstanceInfo->context);

                break;

            case USB_CDC_SET_LINE_CODING:

                setLineCodingEventData.length = irp->size;
                setLineCodingEventData.transferHandle =
                        (USB_HOST_CDC_TRANSFER_HANDLE) irp;
                setLineCodingEventData.transferStatus =
                        USB_HOST_CDC_TRANSFER_STATUS_OK;
                cdcInstanceInfo->appEventCallBack(
                        cdcInstanceInfo->instanceNumber,
                        USB_HOST_CDC_EVENT_SET_LINE_CODING_COMPLETE,
                        &setLineCodingEventData,
                        cdcInstanceInfo->context);

                break;

            case USB_CDC_SET_CONTROL_LINE_STATE:
                setControlLineStateEventData.length = irp->size;
                setControlLineStateEventData.transferHandle =
                        (USB_HOST_CDC_TRANSFER_HANDLE) irp;
                setControlLineStateEventData.transferStatus =
                                        USB_HOST_CDC_TRANSFER_STATUS_OK;
                cdcInstanceInfo->appEventCallBack(
                        cdcInstanceInfo->instanceNumber,
                        USB_HOST_CDC_EVENT_SET_CONTROL_LINE_STATE_COMPLETE,
                        &setControlLineStateEventData,
                        cdcInstanceInfo->context);
                break;
        }
    }
    else {
   //     SYS_ASSERT(false, " Setup request failed ");
        if (irp->status == USB_HOST_IRP_STATUS_ERROR_NAK_TIMEOUT) {
            // Timing problem will resend the same packet
        }
        if (irp->status == USB_HOST_IRP_STATUS_ERROR_DATA ||
                irp->status == USB_HOST_IRP_STATUS_ERROR_BUS) {
            // Resend the request
        }
        if (irp->status == USB_HOST_IRP_STATUS_ERROR_STALL) {
         //   _USB_HOST_ClearEndpointHalt(cdcInstanceInfo->cdcDeviceId, (uint8_t) 0 , _USB_HOST_CDC_ControlTransferComplete );
            //clear the stall
        }
        switch (cdcInstanceInfo->controlRequest) {
            case USB_CDC_GET_LINE_CODING:
                getLineCodingEventData.length = irp->size;
                getLineCodingEventData.transferHandle =
                                    (USB_HOST_CDC_TRANSFER_HANDLE) irp;
                getLineCodingEventData.transferStatus =
                                    USB_HOST_CDC_TRANSFER_STATUS_ERROR;
                cdcInstanceInfo->appEventCallBack(
                        cdcInstanceInfo->instanceNumber,
                        USB_HOST_CDC_EVENT_GET_LINE_CODING_COMPLETE,
                        &getLineCodingEventData,
                        cdcInstanceInfo->context);

                break;

            case USB_CDC_SET_LINE_CODING:

                setLineCodingEventData.length = irp->size;
                setLineCodingEventData.transferHandle =
                        (USB_HOST_CDC_TRANSFER_HANDLE) irp;
                setLineCodingEventData.transferStatus =
                        USB_HOST_CDC_TRANSFER_STATUS_ERROR;
                cdcInstanceInfo->appEventCallBack(
                        cdcInstanceInfo->instanceNumber,
                        USB_HOST_CDC_EVENT_SET_LINE_CODING_COMPLETE,
                        &setLineCodingEventData,
                        cdcInstanceInfo->context);

                break;

            case USB_CDC_SET_CONTROL_LINE_STATE:
                setControlLineStateEventData.length = irp->size;
                setControlLineStateEventData.transferHandle =
                        (USB_HOST_CDC_TRANSFER_HANDLE) irp;
                setControlLineStateEventData.transferStatus =
                        USB_HOST_CDC_TRANSFER_STATUS_ERROR;
                cdcInstanceInfo->appEventCallBack(
                        cdcInstanceInfo->instanceNumber,
                        USB_HOST_CDC_EVENT_SET_CONTROL_LINE_STATE_COMPLETE,
                        &setControlLineStateEventData,
                        cdcInstanceInfo->context);
                break;

        }
    }
}
   /*************************************************************************/

/* Function:
   void _USB_HOST_CDC_BulkInTransferComplete(USB_HOST_IRP *irp)
  Summary:
        ReadComplete data callback
  Description:
        This function handles Read data events
  Parameters:
        USB_HOST_IRP *irp
  Returns:
        None
*/

 void _USB_HOST_CDC_BulkInTransferComplete(USB_HOST_IRP *irp)
 {

    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    USB_HOST_CDC_EVENT_DATA_READ_COMPLETE readCompleteEventData;
    cdcInstanceInfo = ((USB_HOST_CDC_INSTANCE *) (irp->userData));
    if (irp->status == USB_HOST_IRP_STATUS_COMPLETED
            || irp->status == USB_HOST_IRP_STATUS_COMPLETED_SHORT) {
        readCompleteEventData.length = irp->size;
        /* Transfer was successful */
        readCompleteEventData.transferStatus = USB_HOST_CDC_TRANSFER_STATUS_OK;
    }
    else {
        /* Transfer failed */
        readCompleteEventData.transferStatus =
                USB_HOST_CDC_TRANSFER_STATUS_ERROR;
    }
    readCompleteEventData.transferHandle = (USB_HOST_CDC_TRANSFER_HANDLE) irp;
    cdcInstanceInfo->appEventCallBack(
            cdcInstanceInfo->instanceNumber,
            USB_HOST_CDC_EVENT_READ_COMPLETE,
            &readCompleteEventData,
            cdcInstanceInfo->context);

}

/*************************************************************************/
/* Function:
  void _USB_HOST_CDC_BulkOutTransferComplete(USB_HOST_IRP *irp)

  Summary:
        WriteComplete data callback
  Description:
        This function handles Write data events
  Parameters:
        USB_HOST_IRP *irp

  Returns:
        None
*/
void _USB_HOST_CDC_BulkOutTransferComplete( USB_HOST_IRP *irp)
{
    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    USB_HOST_CDC_EVENT_DATA_WRITE_COMPLETE writeCompleteEventData;
    cdcInstanceInfo = ((USB_HOST_CDC_INSTANCE *) (irp->userData));


    if (irp->status == USB_HOST_IRP_STATUS_COMPLETED ||
            irp->status == USB_HOST_IRP_STATUS_COMPLETED_SHORT) {

        /* This means the Read IRP has terminated */
        writeCompleteEventData.length = irp->size;
        /* Transfer was successful */
        writeCompleteEventData.transferStatus =
                USB_HOST_CDC_TRANSFER_STATUS_OK;
    } else {
        /* Transfer failed */
        writeCompleteEventData.transferStatus =
                USB_HOST_CDC_TRANSFER_STATUS_ERROR;
    }
    writeCompleteEventData.transferHandle = (USB_HOST_CDC_TRANSFER_HANDLE) irp;

    /* Callback to the application */
    cdcInstanceInfo->appEventCallBack(
            cdcInstanceInfo->instanceNumber,
            USB_HOST_CDC_EVENT_WRITE_COMPLETE,
            &writeCompleteEventData,
            cdcInstanceInfo->context);
}



uint8_t  _USB_HOST_CDC_findInstance(USB_HOST_DEVICE_ID id )
{
     USB_HOST_CDC_INSTANCE *cdcInstnceInfo;
     uint8_t                  count;
    for(count = 0 ; count < USB_HOST_CDC_INSTANCES_NUMBER ; count ++)
    {
         cdcInstnceInfo = &(gUSBHostCDCInstance[count]);

        if( cdcInstnceInfo->cdcDeviceId == id )
         {
             return count;
         }
    }
    return 0;
}

  /*************************************************************************/

/* Function:
   void _USB_HOST_CDC_InterruptInTransferComplete(USB_HOST_IRP *irp)

  Summary:
       Notification data callback
  Description:
        This function handles Notification data events
  Parameters:
        USB_HOST_IRP *irp

  Returns:
        None
*/
void _USB_HOST_CDC_InterruptInTransferComplete( USB_HOST_IRP *irp)
{
    USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
    USB_HOST_CDC_EVENT_DATA_SERIAL_STATE_NOTIFICATION_RECEIVED
                                        serialStateNotificationData;
    cdcInstanceInfo = ((USB_HOST_CDC_INSTANCE *) (irp->userData));
    if (irp->status == USB_HOST_IRP_STATUS_COMPLETED ||
            irp->status == USB_HOST_IRP_STATUS_COMPLETED_SHORT)
    {

        /* This means the Read IRP has terminated */
        serialStateNotificationData.length = irp->size;
        /* Transfer was successful */
        serialStateNotificationData.transferStatus =
                USB_HOST_CDC_TRANSFER_STATUS_OK;
    } 
    else
    {
        /* Transfer failed */
        serialStateNotificationData.transferStatus =
                USB_HOST_CDC_TRANSFER_STATUS_ERROR;
    }
    serialStateNotificationData.transferHandle =
                                        (USB_HOST_CDC_TRANSFER_HANDLE) irp;

    /* Callback to the application */
    cdcInstanceInfo->appEventCallBack(
            cdcInstanceInfo->instanceNumber,
            USB_HOST_CDC_EVENT_SERIAL_STATE_NOTIFICATION_RECEIVED,
            &serialStateNotificationData,
            cdcInstanceInfo->context);

}


void _USB_HOST_CDC_Task( uint8_t instanceNumber)
{
   //  USB_HOST_CDC_INSTANCE *cdcInstanceInfo;
   //  cdcInstanceInfo = &(gUSBHostCDCInstance[instanceNumber]);

}


/***************  End of  File ************************************/




