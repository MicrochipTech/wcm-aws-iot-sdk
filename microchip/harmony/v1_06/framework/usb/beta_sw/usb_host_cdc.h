/* ******************************************************************************
  USB Host CDC Class Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_cdc.h

  Summary:
    USB Host CDC Class Driver Interface Header

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the USB Host CDC
    Class Driver.
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
#ifndef _USB_HOST_CDC_H_
#define _USB_HOST_CDC_H_

//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/beta_sw/usb_common.h"
#include "usb/beta_sw/usb_cdc.h"
#include "usb/beta_sw/src/usb_host_local.h"


// *****************************************************************************
/* USB Host CDC Class Driver Module Index

  Summary:
    USB Host CDC Class Driver module index definitions

  Description:
    These constants provide USB Host CDC Class Driver index definitions.

  Remarks:
    These constants should be used in place of hard-coded numeric literals.
    These values should be passed into the USB Host Class Driver functions  to
    identify the CDC host class driver instance in use.
*/

#define USB_HOST_CDC_INDEX_0      0
#define USB_HOST_CDC_INDEX_1      1

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* CDC Class Driver Events

  Summary:
    Identifies the possible events that the CDC Class Driver can generate.

  Description:
    This enumeration identifies the possible events that the CDC Class Driver
    can generate. The application should register an event handler using the
    USB_HOST_CDC_EventHandlerSet function to receive CDC Class Driver events.
    
    An event may have data associated with it.  Events that are generated due to
    a transfer of data between the host and device are accompanied by data
    structures that provide the status of the transfer termination. For example,
    the USB_HOST_CDC_EVENT_SET_LINE_CODING_COMPLETE event is accompanied by a
    pointer to a USB_HOST_CDC_EVENT_DATA_SET_LINE_CODING_COMPLETE data
    structure. The transferStatus member of this data structure indicates the
    success or failure of the transfer. A transfer may fail due to device not
    responding on the bus, if the device stalls any stages of the transfer or
    due to NAK timeouts. The event description provides details on the nature of
    the event and the data that is associated with the event.

    The following code snippet shows an example event handling scheme for 

    <code>
    
    // This code snippet shows an example event handling scheme for
    // CDC Class Driver events. 

    USB_HOST_CDC_EVENT_RESPONSE APP_USBHostCDCEventHandler
    (
        USB_HOST_CDC_INDEX index, 
        USB_HOST_CDC_EVENT event, 
        void * eventData,
        uintptr_t context
    )
    {
        uint8_t deviceAddress;
        USB_HOST_CDC_EVENT_DATA_SET_LINE_CODING_COMPLETE * setLineCodingEventData;
        USB_HOST_CDC_EVENT_DATA_GET_LINE_CODING_COMPLETE * getLineCodingEventData;
        USB_HOST_CDC_EVENT_DATA_SET_CONTROL_LINE_STATE_COMPLETE * setControlLineStateEventData;
        USB_HOST_CDC_EVENT_DATA_SEND_BREAK_COMPLETE * sendBreakEventData;
        USB_HOST_CDC_EVENT_DATA_READ_COMPLETE * readCompleteEventData;
        USB_HOST_CDC_EVENT_DATA_WRITE_COMPLETE * writeCompleteEventData;
        USB_HOST_CDC_EVENT_DATA_SERIAL_STATE_NOTIFICATION_RECEIVED * serialStateNotificationEventData;

        switch(event)
        {
            case USB_HOST_CDC_EVENT_ATTACH:
                
                // The event data in this case is the address of the 
                // attached device.

                deviceAddress = (uint8_t *)eventData;
                break;
            
            case USB_HOST_CDC_EVENT_DETACH:
                
                // This means the device was detached. There is no event data
                // associated with this event.

                break;

            case USB_HOST_CDC_EVENT_SET_LINE_CODING_COMPLETE:
                
                // This means the Set Line Coding request completed. We can
                // find out if the request was successful.

                setLineCodingEventData = 
                    (USB_HOST_CDC_EVENT_DATA_SET_LINE_CODING_COMPLETE *)eventData;
                if(setLineCodingEventData->transferStatus 
                                == USB_HOST_CDC_TRANSFER_STATUS_ERROR)
                {
                    // This means there transfer terminated because of an
                    // error.
                }
                break;

            case USB_HOST_CDC_EVENT_GET_LINE_CODING_COMPLETE:

                // This means the Get Line Coding request completed. We can
                // find out if the request was successful.

                getLineCodingEventData = 
                    (USB_HOST_CDC_EVENT_DATA_GET_LINE_CODING_COMPLETE *)eventData;
                if(getLineCodingEventData->transferStatus 
                                    == USB_HOST_CDC_TRANSFER_STATUS_ERROR)
                {
                    // This means there transfer terminated because of an
                    // error.
                }

            case USB_HOST_CDC_EVENT_SET_CONTROL_LINE_STATE_COMPLETE:

                // This means the Set Control Line State request completed. We can
                // find out if the request was successful.

                setControlLineStateEventData = 
                    (USB_HOST_CDC_EVENT_DATA_SET_CONTROL_LINE_STATE_COMPLETE *)eventData;
                if(setControlLineStateEventData->transferStatus 
                                    == USB_HOST_CDC_TRANSFER_STATUS_ERROR)
                {
                    // This means there transfer terminated because of an
                    // error.
                }
                break;

            case USB_HOST_CDC_EVENT_SEND_BREAK_COMPLETE:

                // This means the Send Break request completed. We can
                // find out if the request was successful.

                sendBreakEventData = 
                    (USB_HOST_CDC_EVENT_DATA_SEND_BREAK_COMPLETE *)eventData;
                if(sendBreakEventData->transferStatus 
                                    == USB_HOST_CDC_TRANSFER_STATUS_ERROR)
                {
                    // This means there transfer terminated because of an
                    // error.
                }
                break;

            case USB_HOST_CDC_EVENT_WRITE_COMPLETE:
                
                // This means the Write request completed. We can
                // find out if the request was successful.

                writeCompleteEventData = 
                    (USB_HOST_CDC_EVENT_DATA_WRITE_COMPLETE *)eventData;
                if(writeCompleteEventData->transferStatus 
                                    == USB_HOST_CDC_TRANSFER_STATUS_ERROR)
                {
                    // This means there transfer terminated because of an
                    // error.
                }
                break;

            case USB_HOST_CDC_EVENT_READ_COMPLETE:
                
                // This means the Read request completed. We can
                // find out if the request was successful.

                readCompleteEventData = 
                    (USB_HOST_CDC_EVENT_DATA_READ_COMPLETE *)eventData;
                if(readCompleteEventData->transferStatus 
                                    == USB_HOST_CDC_TRANSFER_STATUS_ERROR)
                {
                    // This means there transfer terminated because of an
                    // error.
                }
                break;

            case USB_HOST_CDC_EVENT_SERIAL_STATE_NOTIFICATION_RECEIVED:

                // This means the Serial State Notification request completed.
                // We can find out if the request was successful.

                serialStateNotificationEventData = 
                    (USB_HOST_CDC_EVENT_DATA_SERIAL_STATE_NOTIFICATION_RECEIVED *)eventData;
                if(serialStateNotificationEventData->transferStatus 
                                    == USB_HOST_CDC_TRANSFER_STATUS_ERROR)
                {
                    // This means there transfer terminated because of an
                    // error.
                }
                break;

            default:
                break;
        }
        return(USB_HOST_CDC_EVENT_RESPONSE_NONE);
    }
    </code>
  Remarks:
    None.
*/

typedef enum
{
    /* This event occurs when a CDC Class Driver Read operation has completed
       i.e when the data has been received from the connected CDC device. This
       event is generated after the application calls the USB_HOST_CDC_Read
       function. The eventData parameter in the event call back function will be
       of a pointer to a USB_HOST_CDC_EVENT_DATA_READ_COMPLETE structure. This
       contains details about the transfer handle associated with this read
       request, the amount of data read and the termination status of the read
       request. */
    USB_HOST_CDC_EVENT_READ_COMPLETE,

    /* This event occurs when a CDC Class Driver Write operation has completed
       i.e when the data has been written to the connected CDC device. This
       event is generated after the application calls the USB_HOST_CDC_Write
       function. The eventData parameter in the event call back function will be
       of a pointer to a USB_HOST_CDC_EVENT_DATA_WRITE_COMPLETE structure. This
       contains details about the transfer handle associated with this write
       request, the amount of data written and the termination status of the write
       request. */
    USB_HOST_CDC_EVENT_WRITE_COMPLETE,

    /* This event occurs when a CDC Class Driver Serial State Notification
       Get operation has completed.  This event is generated after the
       application calls the USB_HOST_CDC_SerialStateNotificationGet and the
       device sends a serial state notification to the host. The eventData
       parameter in the event call back function will be of a pointer to a
       USB_HOST_CDC_EVENT_DATA_SERIAL_STATE_NOTIFICATION_RECEIVED structure.
       This contains details about the transfer handle associated with this
       request, the amount of data received and the termination status of the
       get request. */
    USB_HOST_CDC_EVENT_SERIAL_STATE_NOTIFICATION_RECEIVED,
    
    /* This event occurs when a CDC Class Driver Get Line Coding
       request has completed.  This event is generated after the application
       calls the USB_HOST_CDC_LineCodingGet function and the device sends the
       line coding to the host. The eventData parameter in the event call back
       function will be of a pointer to a
       USB_HOST_CDC_EVENT_DATA_GET_LINE_CODING_COMPLETE structure.  This
       contains details about the transfer handle associated with this request,
       the amount of data received and the termination status of the get
       request. */
    USB_HOST_CDC_EVENT_GET_LINE_CODING_COMPLETE,
    
    /* This event occurs when a CDC Class Driver Set Line Coding
       request has completed. This event is generated after the application
       calls the USB_HOST_CDC_LineCodingSet function and the device either
       acknowledged or stalled the request. The eventData parameter in the event
       call back function will be of a pointer to a
       USB_HOST_CDC_EVENT_DATA_SET_LINE_CODING_COMPLETE structure. This contains
       details about the transfer handle associated with this request, the
       amount of data sent and the termination status of the set request. */
    USB_HOST_CDC_EVENT_SET_LINE_CODING_COMPLETE,

    /* This event occurs when a CDC Class Driver Set Control Line State 
       request has completed. This event is generated after the application
       calls the USB_HOST_CDC_ControlLineStateSet function and the device has
       either acknowledged or stalled the request. The eventData parameter in
       the event call back function will be of a pointer to a
       USB_HOST_CDC_EVENT_DATA_SET_CONTROL_LINE_STATE_COMPLETE structure. This
       contains details about the transfer handle associated with this request,
       the amount of data sent and the termination status of the set request. */
    USB_HOST_CDC_EVENT_SET_CONTROL_LINE_STATE_COMPLETE,

    /* This event occurs when a CDC Class Driver Send Break 
       request has completed. This event is generated after the application
       calls the USB_HOST_CDC_BreakSend function and the device has either
       acknowledged or stalled the request. The eventData parameter in the event
       call back function will be of a pointer to a
       USB_HOST_CDC_EVENT_DATA_SET_CONTROL_LINE_STATE_COMPLETE structure. This
       contains details about the transfer handle associated with this request,
       the amount of data sent and the termination status of the set request. */
    USB_HOST_CDC_EVENT_SEND_BREAK_COMPLETE,

    /* This event occurs when the host layer has successfully attached the CDC
       Class Driver instance to the attached USB CDC device. The event also
       indicates that the class driver is fully initialized and is ready to
       accept command and data transfer requests from the application. The
       eventData parameter should be interpreted as a uint8_t pointer to the
       device address. */
    USB_HOST_CDC_EVENT_ATTACH,

    /* This event occurs when host layer has detached the CDC class driver
       instance from a USB CDC device. This can happen if the device itself was
       detached, or if the device configuration was changed. There is no event
       data associated with this event. */
    USB_HOST_CDC_EVENT_DETACH
}
USB_HOST_CDC_EVENT;


#define    USB_HOST_CDC_SERIAL_NOTIFICATION_SIZE   64

// *****************************************************************************
/* USB Host CDC Class Driver Result enumeration.
 
  Summary:
    USB Host CDC Class Driver CDC Result enumeration.

  Description:
    This enumeration lists the possible USB Host CDC Class Driver operation
    results. These values are returned by CDC Class Driver functions.

  Remarks:
    None.
*/

typedef enum
{
    /* The operation was successful */
    USB_HOST_CDC_RESULT_OK /* DOM-IGNORE-BEGIN */ = USB_ERROR_NONE /* DOM-IGNORE-END */,

    /* The transfer queue is full and no new transfers can be
     * scheduled */
    USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL 
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_IRP_QUEUE_FULL /* DOM-IGNORE-END */,

    /* The specified instance is not provisioned in the system */
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_INVALID
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_HOST_DRIVER_INSTANCE_INVALID /* DOM-IGNORE-END */,

    /* The specified instance is not ready. The attached device does not use
       this instance of the class driver. */
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY 
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_HOST_DRIVER_NOT_READY /* DOM-IGNORE-END */,

    /* The event handler provided is NULL */
    USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID 
        /* DOM-IGNORE-BEGIN */ = USB_ERROR_PARAMETER_INVALID /* DOM-IGNORE-END */,
}
USB_HOST_CDC_RESULT;

// *****************************************************************************
/* USB Host CDC Class Driver Transfer Status.
 
  Summary:
    USB Host CDC Class Driver Transfer Status enumeration.

  Description:
    This enumeration lists the possible termination status of CDC class driver 
    transfers. This information is a part of the event data that is returned 
    along with transfer related events. 

  Remarks:
    None.
*/

typedef enum
{
    /* The transfer completed successfully. In case of read transfers, either
       the requested amount of data was received or the device sent less 
       data */
    USB_HOST_CDC_TRANSFER_STATUS_OK,

    /* The transfer terminated due to an error. The error could be due to device
       malfunction on the bus, a NAK timeout occurred or the device stalled the
       request */
    USB_HOST_CDC_TRANSFER_STATUS_ERROR
}
USB_HOST_CDC_TRANSFER_STATUS;

// *****************************************************************************
/*  USB Host CDC Class Driver Transfer Handle

  Summary:
     USB Host CDC Class Driver Transfer Handle

  Description:
    This is returned by the CDC Class driver command and data transfer routines
    and should be used by the application to track the transfer especially in
    cases where transfers are queued.

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_CDC_TRANSFER_HANDLE;

// *****************************************************************************
/*  USB Host CDC Class Driver Invalid Transfer Handle Definition
 
  Summary:
     USB Host CDC Class Driver Invalid Transfer Handle Definition.

  Description:
    This definition defines a  USB Host CDC Class Driver Invalid Transfer
    Handle. A Invalid Transfer Handle is returned by the CDC Class Driver data
    and command transfer routines when the request was not successful. 

  Remarks:
    None.
*/

#define USB_HOST_CDC_TRANSFER_HANDLE_INVALID ((USB_HOST_CDC_TRANSFER_HANDLE)(-1))

// *****************************************************************************
/*  USB Host CDC Class Driver Index

  Summary:
     USB Host CDC Class Driver Index

  Description:
    This uniquely identifies a CDC Class Driver instance.

  Remarks:
    None.
*/
typedef uintptr_t USB_HOST_CDC_INDEX;

// *****************************************************************************
/*  USB Host CDC Class Driver Event Handler Response Type

  Summary:
     USB Host CDC Class Driver Event Callback Response Type

  Description:
    This is the return type of the CDC Class Driver event handler.

  Remarks:
    None.
*/

typedef void USB_HOST_CDC_EVENT_RESPONSE;

// *****************************************************************************
/*  USB Host CDC Class Driver Event Handler Response None  

  Summary:
     USB Host CDC Class Driver Event Handler Response Type None.

  Description:
    This is the definition of the CDC Class Driver Event Handler Response
    Type none.

  Remarks:
    Intentionally defined to be empty.
*/

#define USB_HOST_CDC_EVENT_RESPONSE_NONE  

// *****************************************************************************
/*  USB Host CDC Class Driver Event Data.
 
  Summary:
     USB Host CDC Class Driver Event Data.

  Description:
    This data type defines the data structure returned by the driver along with
    the following events:
    USB_HOST_CDC_EVENT_DATA_READ_COMPLETE,
    USB_HOST_CDC_EVENT_DATA_WRITE_COMPLETE,
    USB_HOST_CDC_EVENT_DATA_SET_LINE_CODING_COMPLETE,
    USB_HOST_CDC_EVENT_DATA_SEND_BREAK_COMPLETE,
    USB_HOST_CDC_EVENT_DATA_GET_LINE_CODING_COMPLETE,
    USB_HOST_CDC_EVENT_DATA_SET_CONTROL_LINE_STATE_COMPLETE,
    USB_HOST_CDC_EVENT_DATA_SERIAL_STATE_NOTIFICATION_RECEIVED

  Remarks:
    None.
*/

typedef struct
{
    /* Transfer handle of this transfer */
    USB_HOST_CDC_TRANSFER_HANDLE transferHandle;

    /* Termination transfer status */
    USB_HOST_CDC_TRANSFER_STATUS transferStatus;

    /* Amount of data transferred */
    size_t length;
}
USB_HOST_CDC_EVENT_DATA_READ_COMPLETE,
USB_HOST_CDC_EVENT_DATA_WRITE_COMPLETE,
USB_HOST_CDC_EVENT_DATA_SERIAL_STATE_NOTIFICATION_RECEIVED,
USB_HOST_CDC_EVENT_DATA_GET_LINE_CODING_COMPLETE,
USB_HOST_CDC_EVENT_DATA_SET_LINE_CODING_COMPLETE,
USB_HOST_CDC_EVENT_DATA_SET_CONTROL_LINE_STATE_COMPLETE,
USB_HOST_CDC_EVENT_DATA_SEND_BREAK_COMPLETE;

// *****************************************************************************
/* USB Host CDC Serial State Notification Data Type.

  Summary:
    USB Host Serial State Notification Data Type.

  Description:
    This defines the data type for the CDC Serial State. This data is requested
    from the device over the CDC notification interface.

  Remarks:
    None.
*/

typedef USB_CDC_SERIAL_STATE USB_HOST_CDC_SERIAL_STATE_NOTIFICATION_DATA;

// *****************************************************************************
/* USB Host CDC Control Line State Data Type.

  Summary:
    USB Host Control Line State Data Type.

  Description:
    This defines the data type for the CDC Control Line State. This data is
    requested from the device over the CDC control interface.

  Remarks:
    None.
*/

typedef USB_CDC_CONTROL_LINE_STATE USB_HOST_CDC_CONTROL_LINE_STATE;

// *****************************************************************************
/* USB Host CDC Class Driver Event Handler Function Pointer Type.

  Summary:
    USB Host CDC Class Driver Event Handler Function Pointer Type.

  Description:
    This data type defines the required function signature of the USB Host CDC
    Class Driver event handling callback function. The application must
    register a pointer to a CDC Class Driver events handling function whose
    function signature (parameter and return value types) match the types
    specified by this function pointer in order to receive event call backs from
    the CDC Class Driver. The class driver will invoke this function with
    event relevant parameters. The description of the event handler function
    parameters is given here.

    instanceIndex           - Instance index of the CDC Class Driver that generated the 
                              event.
    
    event                   - Type of event generated.
    
    eventData               - This parameter should be type casted to a event specific
                              pointer type based on the event that has occurred. Refer 
                              to the USB_HOST_CDC_EVENT enumeration description for
                              more details.
    
    context                 - Value identifying the context of the application that 
                              was registered along with  the event handling function.

  Remarks:
    None.
*/

typedef USB_HOST_CDC_EVENT_RESPONSE (* USB_HOST_CDC_EVENT_HANDLER )
 (  USB_HOST_CDC_INDEX index,
    USB_HOST_CDC_EVENT event,
    void * eventData,
    uintptr_t context );

// *****************************************************************************
// *****************************************************************************
// Section: CDC Class Driver Interface Function Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_EventHandlerSet (
                USB_HOST_CDC_INDEX instance 
                USB_HOST_CDC_EVENT_HANDLER eventHandler 
                uintptr_t context);


  Summary:
    This class registers a event handler for the specified CDC class
    driver instance. 

  Description:
    This function registers an application event handler for the specified CDC
    class driver instance events. The event handler should be registered before
    the host layer operation is enabled.
    
  Precondition:
    None.

  Parameters:
    instance  - Instance of the CDC Class Driver.
    eventHandler - A pointer to event handler function.
    context - Application specific context that is returned in the event handler.

  Returns:
    USB_HOST_CDC_RESULT_OK - The operation was successful
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_INVALID - The specified instance does 
    not exist.
    USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID - The eventHandler parameter is 
    NULL
    
  Example:
    <code>
    // This code snippet shows an example of registering an event handler. Here
    // the application specifies the context parameter as a pointer to an
    // application object (appObject) that should be associated with this 
    // instance of the CDC class driver.
    
    USB_HOST_CDC_RESULT result;
    
    USB_HOST_CDC_EVENT_RESPONSE APP_USBHostCDCEventHandler 
    (
        USB_HOST_CDC_INDEX instanceIndex ,
        USB_HOST_CDC_EVENT event ,
        void * event, uintptr_t context 
    )
    {
        // Event Handling comes here

        switch(event) 
        {
            ...
        }

        return(USB_HOST_CDC_EVENT_RESPONSE_NONE);
    }

    result = USB_HOST_CDC_EventHandlerSet ( &APP_EventHandler);

    if(USB_HOST_CDC_RESULT_OK != result)
    {
        SYS_ASSERT ( false , "invalid event handler class" );
    }
  
    </code>

  Remarks:
    None.

 */

USB_ERROR USB_HOST_CDC_EventHandlerSet (
                                  USB_HOST_CDC_EVENT_HANDLER appCDCHandler );

// *****************************************************************************
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

USB_HOST_CDC_RESULT USB_HOST_CDC_Read(USB_HOST_CDC_INDEX index, 
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, void * destination, 
        size_t length);

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

USB_HOST_CDC_RESULT USB_HOST_CDC_Write(USB_HOST_CDC_INDEX index, 
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, void * source, 
        size_t length);

// *****************************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_SerialStateNotificationGet
    (
        USB_HOST_CDC_INDEX index, 
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, 
        USB_HOST_CDC_SERIAL_STATE_NOTIFICATION_DATA * dest, 
    );

  Summary:
    This function requests serial state notification data from the attached 
    CDC device.

  Description:
    This function requests serial state notification data from the attached CDC
    device.  The function will schedule a read transfer over the CDC
    notification interface. If successful, the transferHandle parameter will
    contain a valid transfer handle, else it will contain
    USB_HOST_CDC_TRANSFER_HANDLE_INVALID. When completed, the CDC class driver
    will generate a USB_HOST_CDC_EVENT_SERIAL_STATE_NOTIFICATION_RECEIVED event.
    Multiple get requests can be queued. In such a case, the transfer handle for
    each request will be unique.
    
  Precondition:
    None.

  Parameters:
    index - CDC Class Driver Instance Index where the request should be scheduled.

    transferHandle  - Pointer to USB_HOST_CDC_TRANSFER_HANDLE type of a variable.
                      This will contain a valid transfer handle if the request 
                      was successful.

    dest            - pointer to the buffer where the serial state notification
                      data should be stored.

  Returns:
    USB_HOST_CDC_RESULT_OK - The operation was successful. transferHandle will
    contain a valid transfer handle.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_INVALID - The specified instance does 
    not exist.
    USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL - The transfer queue is full
    and the requested transfer cannot be scheduled.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY - The specified instance does
    not have a CDC device associated with it. This can happen if the attached
    device was disconnected or the get request was called with an CDC class
    driver instance that was not attached with a CDC device.
    USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID - The source buffer pointer
    is NULL.
    
  Example:
    <code>
    // The following code snippet shows an example of scheduling a CDC Class
    // Driver Serial State Notification Get function. 
    
    USB_HOST_CDC_TRANSFER_HANDLE transferHandle;
    USB_HOST_CDC_RESULT result;
    USB_HOST_CDC_SERIAL_STATE_NOTIFICATION_DATA * buffer;
    
    result = USB_HOST_CDC_SerialStateNotificationGet(USB_HOST_CDC_0, 
                                            &transferHandle, buffer); 

    if(USB_HOST_CDC_RESULT_OK != result)
    {
        // Error handling here
    }

    // The completion of the get request can be tracked by using the
    // USB_HOST_CDC_EVENT_SERIAL_STATE_NOTIFICATION_RECEIVED.
  
    </code>

  Remarks:
    None.

*/

USB_HOST_CDC_RESULT USB_HOST_CDC_SerialStateNotificationGet
        (USB_HOST_CDC_INDEX index, USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, 
         USB_HOST_CDC_SERIAL_STATE_NOTIFICATION_DATA * dest);

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

USB_HOST_CDC_RESULT USB_HOST_CDC_LineCodingGet(USB_HOST_CDC_INDEX index, 
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, USB_CDC_LINE_CODING * lineCoding);

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
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, USB_CDC_LINE_CODING * lineCoding);

// *****************************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_ControlLineStateSet(USB_HOST_CDC_INDEX index,
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, 
        USB_CDC_HOST_CONTROL_LINE_STATE * controlLineState);

  Summary:
    This function sets the state of the control line connecting the host to 
    the device.

  Description:
    This function sets the state of the control line connecting the host to the
    device.  The function schedules a SET CONTROL LINE STATE control transfer.
    If successful, the transferHandle parameter will contain a valid transfer
    handle, else it will contain USB_HOST_CDC_TRANSFER_HANDLE_INVALID. When
    completed, the CDC class driver will generate a
    USB_HOST_CDC_EVENT_SET_CONTROL_LINE_STATE_COMPLETE event.  The CDC Class
    driver does not support queuing of control transfers.  If a control transfer
    is in progress, this function will return with an error.
    
  Precondition:
    None.

  Parameters:
    index - CDC Class Driver Instance Index where the request should be scheduled.

    transferHandle  - Pointer to USB_HOST_CDC_TRANSFER_HANDLE type of a variable.
                      This will contain a valid transfer handle if the request 
                      was successful.

    controlLineState - pointer to the buffer containing control line state data to be
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
    USB_HOST_CDC_RESULT_ERROR_PARAMETER_INVALID - The controlLineState buffer pointer
    is NULL.
    
  Example:
    <code>
    // The following code snippet shows an example of scheduling a CDC Class
    // Driver Control Line State Set Request.
    
    USB_HOST_CDC_TRANSFER_HANDLE transferHandle;
    USB_HOST_CDC_RESULT result;
    USB_CDC_HOST_CONTROL_LINE_STATE * buffer;
    
    result = USB_HOST_CDC_ControlLineStateSet(USB_HOST_CDC_0, 
                                        &transferHandle, buffer); 

    if(USB_HOST_CDC_RESULT_OK != result)
    {
        // Error handling here
    }

    // The completion of the set request can be tracked by using the
    // USB_HOST_CDC_EVENT_SET_CONTROL_LINE_STATE_COMPLETE event.
  
    </code>

  Remarks:
    None.

*/

USB_HOST_CDC_RESULT USB_HOST_CDC_ControlLineStateSet(USB_HOST_CDC_INDEX index,
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, 
        USB_CDC_CONTROL_LINE_STATE * controlLineState);

/***************************************************************************
  Function:
       USB_HOST_CDC_RESULT USB_HOST_CDC_BreakSend(USB_HOST_CDC_INDEX index,
           USB_HOST_CDC_TRANSFER_HANDLE * transferHandle,
           uint16_t breakDuration);
    
  Summary:
    This function sends a request to the attached to update its break
    duration.
  Description:
    This function sends a request to the attached to update its break
    duration. The function schedules a SEND BREAK control transfer. If
    successful, the transferHandle parameter will contain a valid transfer
    handle, else it will contain USB_HOST_CDC_TRANSFER_HANDLE_INVALID. When
    completed, the CDC class driver will generate a
    USB_HOST_CDC_EVENT_SEND_BREAK_COMPLETE event. The CDC Class driver does
    not support queuing of control transfers. If a control transfer is in
    progress, this function will return with an error.
  Conditions:
    None.
  Input:
    index -           CDC Class Driver Instance Index where the request
                      should be scheduled.
    transferHandle -  Pointer to USB_HOST_CDC_TRANSFER_HANDLE type of a
                      variable. This will contain a valid transfer handle if
                      the request was successful.
    breakDuration -   Break duration.
  Return:
    USB_HOST_CDC_RESULT_OK - The operation was successful. transferHandle
    will contain a valid transfer handle.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_INVALID - The specified instance
    does not exist. USB_HOST_CDC_RESULT_ERROR_TRANSFER_QUEUE_FULL - Another
    control transfer is in progress and this transfer cannot be scheduled.
    USB_HOST_CDC_RESULT_ERROR_INSTANCE_NOT_READY - The specified instance
    does not have a CDC device associated with it. This can happen if the
    attached device was disconnected or the set request was called with an
    CDC class driver instance that was not attached with a CDC device.
  Example:
    <code>
    // The following code snippet shows an example of scheduling a CDC Class
    // Driver Control Line State Set Request.
    
    USB_HOST_CDC_TRANSFER_HANDLE transferHandle;
    USB_HOST_CDC_RESULT result;
    uint16_t breakDuration;
    
    result = USB_HOST_CDC_BreakSend(USB_HOST_CDC_0,
                                        &transferHandle, breakDuration);
    
    if(USB_HOST_CDC_RESULT_OK != result)
    {
        // Error handling here
    }
    
    // The completion of the set request can be tracked by using the
    // USB_HOST_CDC_EVENT_SEND_BREAK_COMPLETE event.
    
    </code>
  Remarks:
    None.                                                                   
  ***************************************************************************/

USB_HOST_CDC_RESULT USB_HOST_CDC_BreakSend(USB_HOST_CDC_INDEX index, 
        USB_HOST_CDC_TRANSFER_HANDLE * transferHandle, uint16_t breakDuration);




#endif 

