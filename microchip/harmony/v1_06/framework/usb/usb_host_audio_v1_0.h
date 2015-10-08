/********************************************************************************
  USB Host Audio v1.0 Class Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_audio_v1_0.h

  Summary:
    USB Host Audio v1_0 Class Driver Interface Header

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the USB Host Audio
    v1.0 Class Driver.
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
#ifndef _USB_HOST_AUDIO_V1_H_
#define _USB_HOST_AUDIO_V1_H_

//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/usb_host.h"
#include "usb/usb_host_client_driver.h"
#include "usb/usb_audio_v1_0.h"


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Host Audio v1.0 Object 
 
  Summary: 
    Defines the type of the Audio v1.0 Host Client Object.

  Description:
    This type defines the type of the Audio Host Client Object. This type
    is returned by the Client Driver Attach Event Handler and is used by the 
    application to open the attached Audio v1.0 Device.  

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_AUDIO_V1_OBJ;

// *****************************************************************************
/* USB Host Audio v1.0 Streaming interface Object 
 
  Summary: 
    Defines the type of the Audio v1.0 Host Streaming interface Object. 

  Description:
    This type defines the type of the Audio v1.0 Host Streaming interface 
    Object. This type is returned by USB_AUDIO_V1_StreamingInterfaceGetFirst()
    and USB_AUDIO_V1_StreamingInterfaceGetNext() functions. 

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ;

// *****************************************************************************
/* USB Host Audio v1.0 Streaming Interface Setting Object 
 
  Summary: 
    Defines the type of the Audio v1.0 Host Streaming interface Setting Object. 

  Description:
    This type defines the type of the Audio v1.0 Host Streaming interface
    setting Object. This type is returned by
    USB_AUDIO_V1_StreamingInterfaceSettingGetFirst() and
    USB_AUDIO_V1_StreamingInterfaceSettingGetNext() functions. 

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ;

// *****************************************************************************
/* USB Host Audio v1.0 Client Driver Request Handle

  Summary:
     USB Host Audio v1.0 Client Driver Request Handle

  Description:
     This is returned by the Audio v1.0 Host client driver Entity Control
     Functions and Audio stream Control request functions.  Application should use
     this handle to track a request. 

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_AUDIO_V1_REQUEST_HANDLE;

// *****************************************************************************
/* USB Host Audio v1.0 Client Driver Invalid Request Handle

  Summary:
     USB Host Audio v1.0 Client Driver Invalid Request Handle

  Description:
    This is returned by the Audio v1.0 Client driver command routines when the 
    request could not be scheduled.

  Remarks:
    None.
*/

#define USB_HOST_AUDIO_V1_REQUEST_HANDLE_INVALID ((USB_HOST_AUDIO_V1_REQUEST_HANDLE)(-1))

// *****************************************************************************
/* USB Host Audio v1.0 Control Entity Object 

  Summary:
    Defines the type of the Audio v1.0 Host Control Entity Object 

  Description:
    This type defines the type of the object returned by 
    USB_HOST_AUDIO_V1_ControlEntityGetFirst() or 
    USB_HOST_AUDIO_V1_ControlEntityGetNext() functions. This application uses
    this object to get more information about that audio control entity. 

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ; 

// *****************************************************************************
/* USB Host Audio stream handle

  Summary:
    Defines the type of the Audio v1.0 Host Stream Handle.

  Description:
    This type defines the type of the handle returned by
    USB_HOST_AUDIO_V1_StreamOpen() function. The application uses this handle
    to interact with an  Audio Stream. 

  Remarks:
    None.
*/
typedef uintptr_t USB_HOST_AUDIO_V1_STREAM_HANDLE;

// *****************************************************************************
/* USB Host Audio stream Invalid handle

  Summary:
   Defines Audio v1.0 Host Stream Invalid Handle.

  Description:
   This is returned by the USB_HOST_AUDIO_V1_StreamOpen() function when stream
   open failed.  

  Remarks:
    None.
*/

#define USB_HOST_AUDIO_V1_STREAM_HANDLE_INVALID ((USB_HOST_AUDIO_V1_STREAM_HANDLE)(-1))

// *****************************************************************************
/* USB Host Audio v1.0 Class Driver Stream Data Transfer Handle

  Summary:
   USB Host Audio v1.0 Class Driver Stream Data Transfer Handle

  Description:
   This is returned by the Audio v1.0 Class driver stream data transfer
   routines and should be used by the application to track the transfer
   especially in cases where transfers are queued.

  Remarks:
   None.
*/

typedef uintptr_t USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE;

// *****************************************************************************
/* USB Host Audio v1.0 Class Driver Invalid Stream Data Transfer Handle Definition
 
  Summary:
   USB Host Audio v1.0 Class Driver Invalid Stream Data Transfer Handle
   Definition.

  Description:
   This definition defines a  USB Host Audio v1.0 Class Driver invalid stream
   data transfer Handle. A Invalid Transfer Handle is returned by the Audio
   v1.0 Class Driver stream data transfer routines when the request was not
   successful. 

  Remarks:
    None.
*/

#define USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE_INVALID ((USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE)(-1))

/*DOM-IGNORE-BEGIN*/#define USB_HOST_AUDIO_V1_RESULT_MIN -100 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Audio v1.0 Class Driver Result enumeration.
 
  Summary:
    USB Host Audio v1.0 Class Driver Result enumeration.

  Description:
    This enumeration lists the possible USB Host Audio v1.0 Class Driver
    operation results. These values are returned by Audio v1.0 Class Driver
    functions.

  Remarks:
    None.
*/

typedef enum
{
    /* An unknown failure has occurred */
    USB_HOST_AUDIO_V1_RESULT_FAILURE /*DOM-IGNORE-BEGIN*/ = USB_HOST_AUDIO_V1_RESULT_MIN /*DOM-IGNORE-END*/,

     /* The transfer or request could not be scheduled because internal queues
      * are full. The request or transfer should be retried */
    USB_HOST_AUDIO_V1_RESULT_BUSY,

    /* The request was stalled */
    USB_HOST_AUDIO_V1_RESULT_REQUEST_STALLED,

    /* A required parameter was invalid */
    USB_HOST_AUDIO_V1_RESULT_INVALID_PARAMETER,

    /* The associated device does not exist in the system. */
    USB_HOST_AUDIO_V1_RESULT_DEVICE_UNKNOWN,

    /* The specified handle is not valid */
    USB_HOST_AUDIO_V1_RESULT_HANDLE_INVALID,

    /* The transfer or requested was aborted */
    USB_HOST_AUDIO_V1_RESULT_TRANSFER_ABORTED,

    /* The specified Audio v1.0 Object is Invalid */ 
    USB_HOST_AUDIO_V1_RESULT_OBJ_INVALID,

    /* No more audio control entity */ 
    USB_HOST_AUDIO_V1_RESULT_END_OF_CONTROL_ENTITY, 
    
    /* No more streaming interface settings present in the audio Device */ 
    USB_HOST_AUDIO_V1_RESULT_END_OF_STREAMING_INTERFACE, 

    /* No more Interface alternate settings present in the Audio Streaming 
     * Interface */
    USB_HOST_AUDIO_V1_RESULT_END_OF_INTERFACE_SETTINGS,

    /* Indicates that the operation succeeded or the request was accepted and
       will be processed. */ 
    USB_HOST_AUDIO_V1_RESULT_SUCCESS
    
    /*DOM-IGNORE-BEGIN*/ = 1 /*DOM-IGNORE-END*/,
} USB_HOST_AUDIO_V1_RESULT;
   
// *****************************************************************************
/* USB Host Audio v1.0 Stream Event Handler Return Type 
 
  Summary: 
    Return type of the USB Host Audio v1.0 Stream Event Handler.

  Description:
    This enumeration list the possible return values of the USB Host Audio v1.0
    Stream Event Handler.

  Remarks:
    None.
*/

typedef enum
{
    /* This means no response is required */
    USB_HOST_AUDIO_V1_STREAM_EVENT_RESPONSE_NONE /*DOM-IGNORE-BEGIN*/= 0 /*DOM-IGNORE-END*/
    
} USB_HOST_AUDIO_V1_STREAM_EVENT_RESPONSE;

// *****************************************************************************
/*  USB Host Audio v1.0 Class Driver Stream Direction
 
  Summary:
    USB Host Audio v1.0 Class Driver Stream Direction

  Description:
    This enumeration lists possible Audio Stream directions.  

  Remarks:
    None.
*/
typedef enum
{
    /* Stream Direction Host to Device  */ 
    USB_HOST_AUDIO_V1_DIRECTION_OUT  /*DOM-IGNORE-BEGIN*/= 0 /*DOM-IGNORE-END*/,
    
    /* Stream Direction Device to Host  */
    USB_HOST_AUDIO_V1_DIRECTION_IN  /*DOM-IGNORE-BEGIN*/= 1 /*DOM-IGNORE-END*/,
            
} USB_HOST_AUDIO_V1_STREAM_DIRECTION;

// *****************************************************************************
/* Audio v1.0 Class Driver Events

  Summary:
   Identifies the possible events that the Audio v1.0 Class Driver Attach Event
   Handler can generate.
 
 Description:
   This enumeration identifies the possible events that the Audio v1.0 Class
   Driver attach event handler can generate. The application should register an
   event handler using the USB_HOST_AUDIO_V1_AttachEventHandlerSet() function
   to receive Audio v1.0 Class Driver Attach events.
*/

typedef enum 
{
    /* This event occurs when host layer has detected the Audio v1.0 class
     * driver instance from a USB Audio v1.0 Device. There is no event data
     * associated with this event. */ 
    USB_HOST_AUDIO_V1_EVENT_ATTACH, 

   /* This event occurs when host layer has detached the Audio v1.0 class
      driver instance from a USB Audio v1.0 device. This can happen if the
      device itself was detached, or if the device configuration was changed.
      There is no event data associated with this event. */
    USB_HOST_AUDIO_V1_EVENT_DETACH,

} USB_HOST_AUDIO_V1_EVENT;

// *****************************************************************************
/* Audio v1.0 Stream Events

  Summary:
    Identifies the possible events that the Audio v1.0 Stream can generate.

  Description:
    This enumeration identifies the possible events that the Audio v1.0 Stream
    can generate. The application should register an event handler using the
    USB_HOST_AUDIO_V1_StreamEventHandlerSet() function to receive Audio v1.0
    stream events.
    
    An event may have data associated with it.  Events that are generated due
    to a transfer of data between the host and device are accompanied by data
    structures that provide the status of the transfer termination. For
    example, the USB_HOST_AUDIO_V1_STREAM_EVENT_READ_COMPLETE event is
    accompanied by a pointer to a
    USB_HOST_AUDIO_V1_STREAM_EVENT_READ_COMPLETE_DATA data structure. The
    transferStatus member of this data structure indicates the success or
    failure of the transfer. A transfer may fail due to device not responding
    on the bus, if the device stalls any stages of the transfer.  The event
    description provides details on the nature of the event and the data that
    is associated with the event.
*/

typedef enum
{
    /* This event occurs when a Audio v1.0 Stream Read operation has completed
       i.e when the data has been received from the connected Audio v1.0
       Stream.  This event is generated after the application calls the
       USB_HOST_AUDIO_V1_StreamRead() function. The eventData parameter in the
       event call back function will be of a pointer to a
       USB_HOST_AUDIO_V1_STREAM_EVENT_READ_COMPLETE_DATA structure. This
       contains details about the transfer handle associated with this read
       request, the amount of data read and the termination status of the read
       request. */
    USB_HOST_AUDIO_V1_STREAM_EVENT_READ_COMPLETE,

    /* This event occurs when an Audio v1.0 Stream Write operation has
       completed i.e when the data has been written to the connected Audio v1.0
       Stream.  This event is generated after the application calls the
       USB_HOST_AUDIO_V1_StreamWrte()function. The eventData parameter in the
       event call back function will be of a pointer to a
       USB_HOST_AUDIO_V1_STREAM_EVENT_WRITE_COMPLETE_DATA structure. This
       contains details about the transfer handle associated with this write
       request, the amount of data written and the termination status of the
       write request. */
    USB_HOST_AUDIO_V1_STREAM_EVENT_WRITE_COMPLETE,

   /* This event occurs when an Audio Streaming Set Interface request has been 
      completed. This event is generated after the application calls the
      USB_HOST_AUDIO_V1_StreamingInterfaceSet() function. The eventData
      parameter in the event call back function will be of a pointer to a
      USB_HOST_AUDIO_V1_STREAM_EVENT_INTERFACE_SET_COMPLETE_DATA. This contains
      details about the request handle associated with the Interface Set
      request and the termination status of the request.*/
    USB_HOST_AUDIO_V1_STREAM_EVENT_INTERFACE_SET_COMPLETE, 
            
   /* This event occurs when an Audio v1.0 Sampling Frequency Set request has
      been completed. This event is generated after the application calls the
      USB_HOST_AUDIO_V1_StreamSamplingFrequencySet() function. The eventData
      parameter in the event call back function will be of a pointer to a
      USB_HOST_AUDIO_V1_STREAM_EVENT_SAMPLING_FREQUENCY_SET_COMPLETE_DATA. This
      contains details about the request handle associated with this Sampling
      Frequency Set request and the termination status of the Stream Disable
      request.*/
    USB_HOST_AUDIO_V1_STREAM_EVENT_SAMPLING_FREQUENCY_SET_COMPLETE,  

   /* This event occurs when an audio stream is detached from the Host.This 
      can happen if the Audio device itself was detached, or if the Audio
      device configuration was changed. There is no event data associated with
      this event. */
    USB_HOST_AUDIO_V1_STREAM_EVENT_DETACH

} USB_HOST_AUDIO_V1_STREAM_EVENT;

// *****************************************************************************
/*  USB Host Audio v1.0 Class Stream Data Transfer Event Data.
 
  Summary:
    USB Host Audio v1.0 Class Stream Data Transfer Event Data.

  Description:
    This data type defines the data structure returned by the Audio V1.0 stream
    along with the following events:
    USB_HOST_AUDIO_V1_STREAM_EVENT_READ_COMPLETE_DATA,
    USB_HOST_AUDIO_V1_STREAM_EVENT_WRITE_COMPLETE_DATA,

  Remarks:
    None.
*/

typedef struct
{
    /* Transfer handle of this transfer */
    USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE transferHandle;

    /* Amount of data transferred */
    size_t length;
    
    /* Transfer termination status */
    USB_HOST_AUDIO_V1_RESULT result;

}
USB_HOST_AUDIO_V1_STREAM_EVENT_READ_COMPLETE_DATA,
USB_HOST_AUDIO_V1_STREAM_EVENT_WRITE_COMPLETE_DATA;


// *****************************************************************************
/* USB Host Audio v1.0 Class Stream Control Event Data.
 
  Summary:
     USB Host Audio v1.0 Class Stream Control Event Data.

  Description:
    This data type defines the data structure returned by the Audio V1.0 stream
    along with the following events:
    USB_HOST_AUDIO_V1_STREAM_EVENT_INTERFACE_SET_COMPLETE_DATA,
    USB_HOST_AUDIO_V1_STREAM_EVENT_SAMPLING_FREQUENCY_SET_COMPLETE_DATA

  Remarks:
    None.
*/

typedef struct
{
    /* Transfer handle of this transfer */
    USB_HOST_AUDIO_V1_REQUEST_HANDLE  requestHandle; 

    /* Transfer termination status */
    USB_HOST_AUDIO_V1_RESULT requestStatus;

} 
USB_HOST_AUDIO_V1_STREAM_EVENT_INTERFACE_SET_COMPLETE_DATA,
USB_HOST_AUDIO_V1_STREAM_EVENT_SAMPLING_RATE_SET_COMPLETE_DATA;

// *****************************************************************************
/* USB Host Audio v1.0 Client Driver Attach Event Handler Function Pointer Type.

  Summary:
   USB Host Audio v1.0 Client Driver Attach Event Handler Function Pointer Type.

  Description:
   This data type defines the required function signature of the USB Host Audio
   v1.0 Client Driver attach event handling callback function. The application
   must register a pointer to a Audio v1.0 Client Driver attach events handling
   function whose function signature (parameter and return value types) match
   the types specified by this function pointer in order to receive attach and
   detach events call backs from the Audio v1.0 Client Driver. The application
   should use USB_HOST_AUDIO_V1_AttachEventHandlerSet() function to register an
   Attach Event Handler.   The client driver will invoke this function with
   event relevant parameters. The description of the event handler function
   parameters is given here.

   audioObj - Audio Device Object to which this event is directed.

   event - Event indicates if it is an Attach or Detach 

   context - Value identifying the context of the application that was
   registered along with  the event handling function.

  Remarks:
    None.
*/

typedef void (* USB_HOST_AUDIO_V1_ATTACH_EVENT_HANDLER)
(
    USB_HOST_AUDIO_V1_OBJ audioObj, 
    USB_HOST_AUDIO_V1_EVENT event, 
    uintptr_t context
);

// *****************************************************************************
/* USB Host Audio v1.0 Class Driver Stream Event Handler Function Pointer Type.

  Summary:
   USB Host Audio v1.0 Class Driver Stream Event Handler Function Pointer Type.

  Description:
   This data type defines the required function signature of the USB Host Audio
   v1.0 Class Driver Stream event handling callback function. The application
   must register a pointer to a Audio v1.0 Class Driver stream events handling
   function whose function signature (parameter and return value types) match
   the types specified by this function pointer in order to receive event call
   backs from the Audio v1.0 Class Driver. The application should use
   USB_HOST_AUDIO_V1_StreamEventHandlerSet() function to register an Audio
   Stream event Handler.The class driver will invoke this function with event
   relevant parameters. The description of the stream event handler function
   parameters is given here.

   handle           - Handle to the Audio v1.0 Stream 
    
   event            - Type of event generated.
    
   eventData        - This parameter should be type casted to an event specific
                       pointer type based on the event that has occurred. Refer 
                       to the USB_HOST_AUDIO_V1_STREAM_EVENT enumeration 
                       description for more details.
    
   context          - Value identifying the context of the application that 
                       was registered along with  the event handling function.

  Remarks:
    None.
*/

typedef USB_HOST_AUDIO_V1_STREAM_EVENT_RESPONSE (* USB_HOST_AUDIO_V1_STREAM_EVENT_HANDLER )
(  
    USB_HOST_AUDIO_V1_STREAM_HANDLE handle,
    USB_HOST_AUDIO_V1_STREAM_EVENT event,
    void * eventData,
    uintptr_t context 
);

//*****************************************************************************
/* USB Host Audio v1.0 Class driver Control Transfer Complete Callback Function
   Pointer type

  Summary:
   USB Host Audio v1.0 Class driver Control Transfer Complete Callback Function
   Pointer Type

  Description:
   This data type defines the required function signature of the USB Host Audio
   v1.0 class driver Control Transfer Complete callback function. The client
   must provide a pointer to a Control Transfer Complete Callback function
   who's function signature (parameter and return value types) must match the
   types specified by this function pointer in order to receive notification
   when a control transfer has completed. The application should use
   USB_HOST_AUDIO_V1_EntityRequestCallbackSet() function to register an Entity
   Control Request Callback. The Audio v1.0 client driver will invoke this
   function with event relevant parameters.  The description of the event
   handler function parameters is given here.

   audioObj      - Audio v1.0 client driver object associated with this event.

   requestHandle - request handle of the control transfer request that caused
                   this event.

   result        - completion result of the control transfer. This will be
                   USB_HOST_AUDIO_V1_RESULT_SUCCESS if the control transfer
		           completed successfully, USB_HOST_AUDIO_V1_RESULT_FAILURE if 
		           an unknown failure occurred or 
		           USB_HOST_AUDIO_V1_RESULT_REQUEST_STALLED if the request was 
		           stalled.

   size          - size of the data stage that was transferred.

   context       - Value identifying the context of the application that was
                   provided when the USB_HOST_AUDIO_V1_ControlRequest() 
		           function was called.

  Remarks:
    None.
*/

typedef void (* USB_HOST_AUDIO_V1_ENTITY_REQUEST_CALLBACK)
(
    USB_HOST_AUDIO_V1_OBJ audioObj, 
    USB_HOST_AUDIO_V1_REQUEST_HANDLE requestHandle,
    USB_HOST_AUDIO_V1_RESULT result,
    size_t size,
    uintptr_t context
); 

// ****************************************************************************
// ****************************************************************************
// Section: Client Access Functions
// ****************************************************************************
// ****************************************************************************

// ****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_AttachEventHandlerSet
    (
        USB_HOST_AUDIO_V1_ATTACH_EVENT_HANDLER eventHandler,
        uintptr_t context
    );

  Summary:
    This function will set an attach/detach event handler.

  Description:
    This function will set an attach event handler. The attach event handler
    will be called when a Audio v1.0 device has been attached or detached. The 
    context will be returned in the event handler. This function should be 
    called before the bus has been enabled.
    
  Precondition:
    None.

  Parameters:
    eventHandler - Pointer to the attach event handler
    
    context      - An application defined context that will be returned in the event
                   handler.

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - if the attach event handler was registered
    successfully. 

    - USB_HOST_AUDIO_V1_RESULT_FAILURE - if the number of registered event 
    handlers has exceeded USB_HOST_AUDIO_V1_ATTACH_LISTENERS_NUMBER.
    
  Example:
    <code> 
    </code>

  Remarks:
    Function should be called before USB_HOST_BusEnable() function is called.
*/

USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_AttachEventHandlerSet
(
    USB_HOST_AUDIO_V1_ATTACH_EVENT_HANDLER eventHandler,
    uintptr_t context
);

// ****************************************************************************
/* Function:
    USB_HOST_DEVICE_OBJ_HANDLE USB_HOST_AUDIO_V1_DeviceObjHandleGet
    (
        USB_HOST_AUDIO_V1_OBJ audioDeviceObj
    );
           
  Summary:
    This function returns the Device Object Handle for this Audio v1.0 device.

  Description:
    This function returns the Device Object Handle for this Audio v1.0 device.
    This returned Device Object Handle can be used by the application to
    perform device level operations such as getting the string descriptors.

  Precondition:
    None.

  Input:
    audioDeviceObj - Audio V1.0 device object handle returned in the
    USB_HOST_AUDIO_V1_ATTACH_EVENT_HANDLER function.

  Return:
    Will return a valid device object handle if the device is still connected
    to the system. Will return an USB_HOST_DEVICE_OBJ_HANDLE_INVALID otherwise.
    
  Example:
    <code>
    </code>

  Remarks:
    None.                                                                   
*/

USB_HOST_DEVICE_OBJ_HANDLE USB_HOST_AUDIO_V1_DeviceObjHandleGet
(
    USB_HOST_AUDIO_V1_OBJ audioDeviceObj
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_EntityRequestCallbackSet
    (
        USB_HOST_AUDIO_V1_OBJ audioDeviceObj, 
        USB_HOST_AUDIO_V1_CONTROL_EVENT_HANDLER appAudioEntityRequestCallback,
        uintptr_t context 
    ); 

   Summary:
    Registers an Audio Entity request callback function with the Audio v1.0
    Client Driver.

   Description:
    This function registers a callback function for the Audio v1.0 Control
    Entity requests. The Audio v1.0 Host Client Driver will invoke this
    callback function when an Audio Entity control request is completed.   
    
   Precondition:
    None.

   Parameters:
    audioDeviceObj  - Audio v1.0 device object.

    appAudioEntityRequestCallback - A pointer to event handler function. If NULL,
    then events will not be generated.
    
    context - Application specific context that is returned in the event handler.

   Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_RESULT_OBJ_INVALID - The specified audio 
      Object does not exist
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An unknown failure occurred
    
   Example:
     <code>
     </code>

   Remarks:
     None.
*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_EntityRequestCallbackSet
(
    USB_HOST_AUDIO_V1_OBJ audioDeviceObj, 
    USB_HOST_AUDIO_V1_ENTITY_REQUEST_CALLBACK appAudioEntityRequestCallback,
    uintptr_t context 
); 


// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceGetFirst
    (
        USB_HOST_AUDIO_V1_OBJ audioObj, 
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ* streamingInterfaceObj
    );

   Summary:
    This function will Get the first streaming interface object from the
    attached Audio Device.

   Description:
    This function will Get the first streaming interface object from the
    attached Audio Device.

   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio v1.0 client driver object

    streamingInterfaceObj - Pointer to an Audio Streaming Interface Object 

   Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The request completed successfully
    - USB_HOST_AUDIO_V1_RESULT_END_OF_STREAMING_INTERFACE - No more streaming 
    interfaces available
    - USB_HOST_AUDIO_V1_RESULT_DEVICE_UNKNOWN - Device is not attached currently
    - USB_HOST_AUDIO_V1_RESULT_OBJ_INVALID - Audio Device object is invalid
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An error has occurred
     
   Example:
    <code> 
    </code>

   Remarks: 
    None
*/

USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceGetFirst
(
    USB_HOST_AUDIO_V1_OBJ audioObj, 
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ* streamingInterfaceObj
);


// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceGetNext
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,    
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObjCurrent 
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ* streamingInterfaceObjNext
    );

  Summary:
    This function will Get the next streaming interface object from the
    attached Audio Device.

  Description:
    This function will Get the next streaming interface object from the
    attached Audio Device. 
    
  Precondition:
    Audio v1.0 device should have been attached. 

  Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObjCurrent - Current Audio Streaming Interface Object

    streamingInterfaceObj - Pointer to Audio Streaming Interface Object 

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The request completed successfully
    - USB_HOST_AUDIO_V1_RESULT_END_OF_STREAMING_INTERFACE - No more streaming 
    interfaces available 
    - USB_HOST_AUDIO_V1_RESULT_DEVICE_UNKNOWN - Device is not attached currently
    - USB_HOST_AUDIO_V1_RESULT_OBJ_INVALID - Audio Device object is invalid
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An error has occurred
    
     
  Example:
    <code> 
    </code>

  Remarks:

*/

USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceGetNext
(
    USB_HOST_AUDIO_V1_OBJ audioObj, 
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObjCurrent, 
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ* streamingInterfaceObjNext
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceSettingGetFirst
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ *interfaceSettingObj
    );

  Summary:
    This function gets the first streaming interface setting object with in a
    an Audio Streaming Interface.
    
  Description:
    This function gets the first streaming interface setting object with in a
    an Audio Streaming Interface.
    
  Precondition:
    Audio v1.0 device should have been attached. 

  Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObj - Audio Streaming Interface Object

    interfaceSettingObj- Pointer to Audio Streaming Interface Setting Object 

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The request completed successfully
    - USB_HOST_AUDIO_V1_RESULT_END_OF_INTERFACE_SETTINGS - No more streaming 
      interface settings available 
    - USB_HOST_AUDIO_V1_RESULT_DEVICE_UNKNOWN - Device is not attached currently
    - USB_HOST_AUDIO_V1_RESULT_OBJ_INVALID - Audio Device object is invalid
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An error has occurred
    
     
  Example:
    <code> 
    </code>

  Remarks:

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceSettingGetFirst
(
    USB_HOST_AUDIO_V1_OBJ audioObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ *interfaceSettingObj
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceSettingGetNext 
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObjCurrent, 
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ *interfaceSettingObjNext
    );

   Summary:
    This function gets the Next streaming interface setting object with in a
    an Audio Streaming Interface.
    
   Description:
    This function gets the Next streaming interface setting object with in a
    an Audio Streaming Interface.
    
   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObj - Audio Streaming Interface Object

    interfaceSettingObjCurrent - Current Audio Streaming Interface Setting Object 

    interfaceSettingObjNext - Pointer to Next Audio Streaming Interface Setting Object  

   Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The request completed successfully
    - USB_HOST_AUDIO_V1_RESULT_END_OF_INTERFACE_SETTINGS - No more streaming 
      interface settings available 
    - USB_HOST_AUDIO_V1_RESULT_DEVICE_UNKNOWN - Device is not attached currently
    - USB_HOST_AUDIO_V1_RESULT_OBJ_INVALID - Audio Device object is invalid
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An error has occurred
    
     
   Example:
    <code> 
    </code>

   Remarks:

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceSettingGetNext 
(
    USB_HOST_AUDIO_V1_OBJ audioObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObjCurrent, 
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ *interfaceSettingObjNext
);

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceTerminalLinkGet
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
    );

   Summary:
    This function returns Terminal Link of the specified streaming interface
    setting. 
    
   Description:
    This function returns Terminal Link of the specified streaming interface
    setting.
    
   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObj - Audio Streaming Interface Object

    interfaceSettingObj - Audio Streaming Interface Setting Object 

   Returns:
     Terminal Link of the Audio Streaming Interface setting. 
     
   Example:
    <code> 
    </code>

   Remarks:

*/
uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceTerminalLinkGet
(
    USB_HOST_AUDIO_V1_OBJ audioObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
);

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceFormatTagGet
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
    );

   Summary:
    This function returns Format Tag of the specified streaming interface
    setting. 
    
   Description:
    This function returns Format Tag Link of the specified streaming interface
    setting.
    
   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObj - Audio Streaming Interface Object

    interfaceSettingObj - Audio Streaming Interface Setting Object 

   Returns:
     Format Tag of the Audio Streaming Interface setting. 
     
   Example:
    <code> 
    </code>

   Remarks:

*/
USB_AUDIO_V1_FORMAT_TAG USB_HOST_AUDIO_V1_StreamingInterfaceFormatTagGet
(
    USB_HOST_AUDIO_V1_OBJ audioObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
);

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceChannelNumbersGet
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
    );

   Summary:
    This function returns number of channels of the specified streaming interface
    setting. 
    
   Description:
    This function returns Number of channels of the specified streaming interface
    setting. 
    
   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObj - Audio Streaming Interface Object

    interfaceSettingObj - Audio Streaming Interface Setting Object 

   Returns:
     Number of Channels present in the Audio Streaming Interface setting. 
     
   Example:
    <code> 
    </code>

   Remarks:

*/
uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceChannelNumbersGet
(
    USB_HOST_AUDIO_V1_OBJ audioObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
); 

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceSubFrameSizeGet
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
    );

   Summary:
    This function returns sub-frame size of the specified streaming interface
    setting. 
    
   Description:
    This function returns sub-frame Size of the specified streaming interface
    setting.
    
   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio Device Object
    streamingInterfaceObj - Audio Streaming Interface Object
    interfaceSettingObj - Audio Streaming Interface Setting Object 

   Returns:
     Sub-frame Size of the Audio Streaming Interface setting. 
     
   Example:
    <code> 
    </code>

   Remarks:

*/
uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceSubFrameSizeGet
(
    USB_HOST_AUDIO_V1_OBJ audioObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
);


// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceBitResolutionGet
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
    );

   Summary:
    This function returns Bit resolution of the specified streaming interface
    setting. 
    
   Description:
    This function returns Bit Resolution size of the specified streaming interface
    setting.
    
   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObj - Audio Streaming Interface Object

    interfaceSettingObj - Audio Streaming Interface Setting Object 

   Returns:
     Bit Resolution size of the Audio Streaming Interface setting. 
     
   Example:
    <code> 
    </code>

   Remarks:

*/
uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceBitResolutionGet
(
    USB_HOST_AUDIO_V1_OBJ audioObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
);

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceSamplingFrequencyTypeGet
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
    );

   Summary:
    Returns Sampling Frequency Type of the specified streaming interface
    setting. 
    
   Description:
    This function returns Sampling Frequency Type of the specified streaming 
    interface setting.
    
   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObj - Audio Streaming Interface Object

    interfaceSettingObj - Audio Streaming Interface Setting Object 

   Returns:
     Sampling Frequency Type of the Audio Streaming Interface setting 
     - 0 - Continuous Sampling frequency is supported
     - 1 to 255 - The number of discrete sampling frequencies supported by the
                  AudioStreaming interface
     
   Example:
    <code> 
    </code>

   Remarks:

*/
uint8_t USB_HOST_AUDIO_V1_StreamingInterfaceSamplingFrequencyTypeGet
(
    USB_HOST_AUDIO_V1_OBJ audioObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
);


// *****************************************************************************
/* Function:
    uint8_t* USB_HOST_AUDIO_V1_StreamingInterfaceSamplingFrequenciesGet
    (
        USB_HOST_AUDIO_V1_OBJ audioObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
    );

   Summary:
    Returns Sampling Frequencies supported by the specified streaming interface
    setting. 
    
   Description:
    This function returns Sampling Frequencies supported by the specified streaming 
    interface setting. 
    
   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObj - Audio Streaming Interface Object

    interfaceSettingObj - Audio Streaming Interface Setting Object 

   Returns:
     Pointer to Sampling Frequencies supported by the Audio Streaming Interface setting. 
     
   Example:
    <code> 
    </code>

   Remarks:

*/
uint8_t* USB_HOST_AUDIO_V1_StreamingInterfaceSamplingFrequenciesGet
(
    USB_HOST_AUDIO_V1_OBJ audioObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_STREAM_DIRECTION USB_HOST_AUDIO_V1_StreamingInterfaceDirectionGet
    (
        USB_HOST_AUDIO_V1_OBJ audioObj, 
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
    );

   Summary:
    Returns Direction of the specified streaming interface
    setting. 
    
   Description:
    This function returns Direction of the specified streaming 
    interface setting.
    
   Precondition:
    Audio v1.0 device should have been attached. 

   Parameters:
    audioObj - Audio Device Object

    streamingInterfaceObj - Audio Streaming Interface Object

    interfaceSettingObj - Audio Streaming Interface Setting Object 

   Returns:
     - USB_HOST_AUDIO_V1_DIRECTION_OUT - Host to Device
     - USB_HOST_AUDIO_V1_DIRECTION_IN - Device to Host
     
   Example:
    <code> 
    </code>

   Remarks:

*/
USB_HOST_AUDIO_V1_STREAM_DIRECTION USB_HOST_AUDIO_V1_StreamingInterfaceDirectionGet
(
    USB_HOST_AUDIO_V1_OBJ audioObj, 
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ streamingInterfaceObj,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
); 
        
// *****************************************************************************
/* Function:
   USB_HOST_AUDIO_V1_REESULT USB_HOST_AUDIO_V1_ControlEntityGetFirst
   (
       USB_HOST_AUDIO_V1_OBJ  audioObj, 
       USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ * pEntityObject
   ); 

  Summary:
    Retrieves handle to the first Audio Control entity   

  Description:
    This function retrieves handle to first Audio Control entity. 
    
  Precondition:
    

  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    pEntityObject - pointer to the Audio control entity handle

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_RESULT_END_OF_CONTROL_ENTITY - No more audio control
      entity
    - USB_HOST_AUDIO_V1_RESULT_OBJ_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An unknown failure occurred.
    
  Example:
    <code>
    </code>
  Remarks:
    None.

*/

USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_ControlEntityGetFirst
(
    USB_HOST_AUDIO_V1_OBJ  audioObj, 
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ * pEntityObject
);
// *****************************************************************************
/* Function:
   USB_HOST_AUDIO_V1_REESULT USB_HOST_AUDIO_V1_ControlEntityGetNext
   (
       USB_HOST_AUDIO_V1_OBJ  audioObj, 
       USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObjectCurrent
       USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ * pEntityObject
   ); 

  Summary:
    Retrieves handle to the next Audio Control entity.

  Description:
    This function retrieves handle to the next Audio Control entity. 
    
  Precondition:
    

  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObjectCurrent - Handle to current Audio control entity. 

    pEntityObject  -  pointer to Audio control entity Handle. 


  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_RESULT_END_OF_CONTROL_ENTITY - No more audio control
      entity
    - USB_HOST_AUDIO_V1_RESULT_OBJ_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_ControlEntityGetNext
(
    USB_HOST_AUDIO_V1_OBJ  audioObj, 
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObjectCurrent,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ * pEntityObject
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_EntityObjectGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj,
        uint8_t entityId,
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ* entityObj
    ); 

  Summary:
    Retrieves Entity Object for Entity ID.

  Description:
    This function retrieves Entity Object for Entity ID.  
    
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 
 
    entityId - Entity ID

    entityObject - Audio control entity Object 

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
	- USB_HOST_AUDIO_V1_RESULT_FAILURE - Entity Id could not e found. Or an 
	  unknown failure occurred.

  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_EntityObjectGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    uint8_t entityId,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ* entityObj
);

// *****************************************************************************
/* Function:
    USB_AUDIO_V1_ENTITY_TYPE USB_HOST_AUDIO_V1_EntityTypeGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj, 
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
    ); 

  Summary:
    Returns Entity Type of the Audio Control entity.

  Description:
    This function returns Entity Type of the Audio Control entity. Prior to
    calling this function Entity Object should obtained by calling
    USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 

  Returns:
    USB_AUDIO_V1_ENTITY_TYPE. 
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_AUDIO_V1_ENTITY_TYPE USB_HOST_AUDIO_V1_EntityTypeGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj, 
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
);

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_TerminalIDGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj,
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
    ); 

  Summary:
    Returns Terminal ID of the Audio Control entity.

  Description:
    This function returns Terminal ID of the Audio Control entity. Prior to
    calling this function Entity Object should obtained by calling
    USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 

  Returns:
    The terminal ID.
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
 
uint8_t USB_HOST_AUDIO_V1_TerminalIDGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
);

// *****************************************************************************
/* Function:
    USB_AUDIO_V1_TERMINAL_TYPE USB_HOST_AUDIO_V1_TerminalTypeGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj,
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
    ); 

  Summary:
    Returns Terminal Type of the Audio Control entity.

  Description:
    This function returns Terminal type of the Audio Control entity. Prior to
    calling this function Entity Object should obtained by calling
    USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object
    entityObject - Audio control entity Object 

  Returns:
    The terminal type.
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_AUDIO_V1_TERMINAL_TYPE USB_HOST_AUDIO_V1_TerminalTypeGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
);

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_TerminalAssociationGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj,
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
    ); 

  Summary:
    Returns Associated Terminal of the Audio Control Terminal.

  Description:
    This function returns ID Associated Terminal type of the Audio Control
    Terminal.  Prior to calling this function Entity Object should obtained by
    calling USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 

  Returns:
    The ID of the associated terminal.
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
uint8_t USB_HOST_AUDIO_V1_TerminalAssociationGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
);

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_TerminalInputChannelNumbersGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj,
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
    ); 

  Summary:
    Returns Number of logical output channels in the Terminal's output audio 
    channel cluster.

  Description:
    This function returns Number of logical output channels in the Terminal's
    output audio channel cluster. This function is only applicable to Input
    Terminal.  Prior to calling this function Entity Object should obtained by
    calling USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 

  Returns:
    Number of logical output channels in the terminal's output audio channel 
    cluster.  
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
uint8_t USB_HOST_AUDIO_V1_TerminalInputChannelNumbersGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
);

// *****************************************************************************
/* Function:
    USB_AUDIO_CHANNEL_CONFIG USB_HOST_AUDIO_V1_TerminalInputChannelConfigGet
	(
		USB_HOST_AUDIO_V1_OBJ  audioObj,
		USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
	); 

  Summary:
    Returns a structure which describes the spatial location of the logical 
	channels of in the Terminal's output audio channel cluster.
	
  Description:
    This function returns a structure which describes the spatial location of 
	the logical channels of in the Terminal's output audio channel cluster. 
	This function is only applicable to Input Terminal.  Prior to calling this
	function Entity Object should obtained by calling 
	USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 

  Returns:
    Structure which describes the spatial location of the logical channels 
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_AUDIO_CHANNEL_CONFIG USB_HOST_AUDIO_V1_TerminalInputChannelConfigGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
); 

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_TerminalSourceIDGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj,
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
    ); 

  Summary:
    Returns ID of the Unit or Terminal to which this Terminal is connected.

  Description:
    This function returns ID of the Unit or Terminal to which this Terminal is
    connected. This function is only applicable to Output Terminal.  Prior to
    calling this function Entity Object should obtained by calling
    USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 

  Returns:
    The ID of the unit or terminal to which this terminal is connected. 
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
uint8_t USB_HOST_AUDIO_V1_TerminalSourceIDGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
);

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_FeatureUnitIDGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj,
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
    ); 

  Summary:
    Returns ID of the Feature Unit.

  Description:
    This function returns ID of the D of the Feature Unit.  This function is
    only applicable to Feature Unit. Prior to calling this function Entity
    Object should obtained by calling
    USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 

  Returns:
    The ID of the feature unit. 
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
uint8_t USB_HOST_AUDIO_V1_FeatureUnitIDGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
); 
// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_FeatureUnitSourceIDGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj,
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
    );

  Summary:
    Returns ID of the Unit or Terminal to which this Feature Unit is connected.

  Description:
    This function returns ID of the Unit or Terminal to which this Feature Unit
    is connected. This function is only applicable to Feature Unit. Prior to
    calling this function Entity Object should obtained by calling
    USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object 
    entityObject - Audio control entity Object 

  Returns:
    The ID of the unit or terminal to which this feature unit is connected. 
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
uint8_t USB_HOST_AUDIO_V1_FeatureUnitSourceIDGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
);
// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_FeatureUnitChannelNumbersGet
    (
        USB_HOST_AUDIO_V1_OBJ  audioObj,
        USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
    ); 

  Summary:
    Returns Number of channels.

  Description:
    This function returns number of channels. This function is only applicable
    to Feature Unit. Prior to calling this function Entity Object should
    obtained by calling USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object 
    entityObject - Audio control entity Object 

  Returns:
    The number of channels. 
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
uint8_t USB_HOST_AUDIO_V1_FeatureUnitChannelNumbersGet
(
    USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject
);

// *****************************************************************************
/* Function:
    bool USB_HOST_AUDIO_V1_FeatureUnitChannelMuteExists
    (
         USB_HOST_AUDIO_V1_OBJ  audioObj,
         USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
         uint8_t channel
    );

  Summary:
    Returns true if Mute Control exists for the specified channel of the Feature Unit. 

  Description:
    This function returns true if Mute Control exists on the specified channel
    of the Feature Unit.  Channel 0 would return indicates Master mute control.
    This function is only applicable to Feature Unit. Prior to calling this
    function Entity Object should obtained by calling
    USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 
    channel - Channel Number. 

  Returns:
    - true  - Mute control exists on the specified channel 
    - false - Mute control does not exist on the specified channel 
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
bool USB_HOST_AUDIO_V1_FeatureUnitChannelMuteExists
(
     USB_HOST_AUDIO_V1_OBJ  audioObj,
     USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
     uint8_t channel
); 
// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelMuteSet
    (
         USB_HOST_AUDIO_V1_OBJ  audioObj,
         USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
         USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
         uint8_t channelNumber,
         bool *muteStatus
    ); 

  Summary:
    Schedules a Set Mute control request to the specified channel.  

  Description:
    This function schedules a set Mute control request to the specified
    channel.  Prior to calling this function user should check if mute control
    exists on the specified channel by calling
    USB_HOST_AUDIO_V1_FeatureUnitChannelMuteExists() function. 
 
    If the request was scheduled successfully, requestHandle will contain a
    request handle that uniquely identifies this transfer. If the transfer
    could not be scheduled successfully, requestHandle will contain
    USB_HOST_AUDIO_V1_REQUEST_HANDLE_INVALID.
 
    When the control request completes, the Audio v1.0 client driver will call
    callback function which was set using the
    USB_HOST_AUDIO_V1_EntityRequestCallbackSet() function. The context
    parameter specified here will be returned in the callback.

  Parameters:
    audioObj     - USB Host Audio v1.0 Device Object. 

    entityObject - Audio Control Entity Object 
  
    requestHandle - Output Parameter that will contain the Handle to this request.
 
    channelNumber - Channel Number 
   
    muteStatus -  Value of Mute control, 1 Mutes the channel, 0 removes Mute. 

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - the request was scheduled successfully.
      requestHandle will contain a valid request handle.
   -  USB_HOST_AUDIO_V1_RESULT_BUSY - The control request mechanism is currently busy. 
      Retry the request 
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - an unknown failure occurred. requestHandle will
      contain USB_HOST_AUDIO_V1_0_REQUEST_HANDLE_INVALID
    - USB_HOST_AUDIO_V1_RESULT_PARAMETER_INVALID - The data pointer or requestHandle pointer
      is NULL
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelMuteSet
(
     USB_HOST_AUDIO_V1_OBJ  audioObj,
     USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
     USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
     uint8_t channelNumber,
     bool *muteStatus
); 
// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelMuteGet
    (
         USB_HOST_AUDIO_V1_OBJ  audioObj,
         USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
         USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
         uint8_t channelNumber,
         bool *muteStatus
    ); 

  Summary:
    Schedules a Get Mute control request to the specified channel.  

  Description:
    This function schedules a Get Mute control request to the specified
    channel. Prior to calling this function user should check if mute control
    exists on the specified channel by calling
    USB_HOST_AUDIO_V1_FeatureUnitChannelMuteExists() function. 
 
    If the request was scheduled successfully, requestHandle will contain a
    request handle that uniquely identifies this request. If the transfer
    could not be scheduled successfully, requestHandle will contain
    USB_HOST_AUDIO_V1_REQUEST_HANDLE_INVALID.
 
    When the control request completes, the Audio v1.0 client driver will call
    callback function which was set using the
    USB_HOST_AUDIO_V1_EntityRequestCallbackSet() function. The context
    parameter specified here will be returned in the callback.
     
  Parameters:
    audioObj     - USB Host Audio v1.0 Device Object. 

    entityObject - Audio Control Entity Object 
  
    requestHandle - Output Parameter that will contain the Handle to this request.
 
    channelNumber - Channel Number
   
    muteStatus -  Output Parameter that will contain Current Mute status when
    Request is completed and a Callback is received.    

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - the request was scheduled successfully.
      requestHandle will contain a valid request handle.
    - USB_HOST_AUDIO_V1_RESULT_BUSY - The control request mechanism is currently
      busy.  Retry the request. 
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - an unknown failure occurred
      requestHandle will contain USB_HOST_AUDIO_V1_0_REQUEST_HANDLE_INVALID.
    - USB_HOST_AUDIO_V1_RESULT_PARAMETER_INVALID - The data pointer or
      requestHandle pointer is NULL
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelMuteGet
(
     USB_HOST_AUDIO_V1_OBJ  audioObj,
     USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
     USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
     uint8_t channelNumber,
     bool *muteStatus
); 

// *****************************************************************************
/* Function:
    bool USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeExists
    (
         USB_HOST_AUDIO_V1_OBJ  audioObj,
         USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
         uint8_t channel
    ); 

  Summary:
    Returns true if Volume Control exists for the specified channel of the
    Feature Unit. 

  Description:
    This function returns true if Volume Control exists on the specified
    channel of the Feature Unit. Channel 0 indicates Master Volume control.
    This function is only applicable to Feature Unit. Prior to calling this
    function Entity Object should obtained by calling
    USB_HOST_AUDIO_V1_ControlEntityGetFirst(),
    USB_HOST_AUDIO_V1_ControlEntityGetNext() or
    USB_HOST_AUDIO_V1_EntityObjectGet() function.
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 
 
    channel - Channel Number 

  Returns:
    - true  - Volume control exists on the specified channel 
    - false - Volume control does not exist on the specified channel 
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/

bool USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeExists
(
     USB_HOST_AUDIO_V1_OBJ  audioObj,
     USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
     uint8_t channel
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSet
    (
         USB_HOST_AUDIO_V1_OBJ  audioObj,
         USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
         USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
         uint8_t channelNumber,
         uint16_t *volume
    );

  Summary:
    Schedules a Set Current Volume control request to the specified channel.  

  Description:
    This function schedules a Set Current Volume request to the specified
    channel.  Prior to calling this function user should check if volume
    control exists on the specified channel by calling
    USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeExists() function. 
 
    If the request was scheduled successfully, requestHandle will contain a
    request handle that uniquely identifies this request. If the request
    could not be scheduled successfully, requestHandle will contain
    USB_HOST_AUDIO_V1_REQUEST_HANDLE_INVALID.
 
    When the control request completes, the Audio v1.0 client driver will call
    callback function which was set using the
    USB_HOST_AUDIO_V1_EntityRequestCallbackSet() function. The context
    parameter specified here will be returned in the callback. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 Device Object. 

    entityObject - Audio Control Entity Object 
  
    requestHandle - Output Parameter that will contain the Handle to this
    request.
 
    channelNumber - Channel Number to which the volume control is addressed to. 
   
    volume -  Current Volume control value that should be set in the Audio
    Device. 

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - the request was scheduled successfully.
      requestHandle will contain a valid request handle.
    - USB_HOST_AUDIO_V1_RESULT_BUSY - The control request mechanism is currently
       busy.  Retry the request. 
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - an unknown failure occurred
      requestHandle will contain USB_HOST_AUDIO_V1_0_REQUEST_HANDLE_INVALID.
    - USB_HOST_AUDIO_V1_RESULT_PARAMETER_INVALID - The data pointer or
      requestHandle pointer is NULL
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSet
(
     USB_HOST_AUDIO_V1_OBJ  audioObj,
     USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
     USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
     uint8_t channelNumber,
     uint16_t *volume
); 


// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeGet
    (
         USB_HOST_AUDIO_V1_OBJ  audioObj,
         USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
         USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
         uint8_t channelNumber,
         uint16_t *volume
    );

  Summary:
    Schedules a Get Current Volume control request to the specified Channel.  

  Description:
    This function schedules a Get Current Volume Control request to the
    specified channel. Prior to calling this function user should check if
    volume control exists on the specified channel by calling
    USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeExists() function. 
 
    If the request was scheduled successfully, requestHandle will contain a
    request handle that uniquely identifies this request. If the request
    could not be scheduled successfully, requestHandle will contain
    USB_HOST_AUDIO_V1_REQUEST_HANDLE_INVALID.
 
    When the control request completes, the Audio v1.0 client driver will call
    callback function which was set using the
    USB_HOST_AUDIO_V1_EntityRequestCallbackSet() function. The context
    parameter specified here will be returned in the callback. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 
  
    requestHandle - Output parameter that will contain the handle to this
    request.
 
    channelNumber - Channel Number to which the volume control is addressed to. 
   
    volume -  Output Parameter that will contain Current Volume when request is
    completed and a callback is received. 

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - the request was scheduled successfully.
      requestHandle will contain a valid request handle.
    - USB_HOST_AUDIO_V1_RESULT_BUSY - The control request mechanism is currently
      busy.  Retry the request. 
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - an unknown failure occurred
      requestHandle will contain USB_HOST_AUDIO_V1_0_REQUEST_HANDLE_INVALID
    - USB_HOST_AUDIO_V1_RESULT_PARAMETER_INVALID - The data pointer or
      requestHandle pointer is NULL
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeGet
(
     USB_HOST_AUDIO_V1_OBJ  audioObj,
     USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
     USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
     uint8_t channelNumber,
     uint16_t *volume
);


// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSubRangeNumbersGet
	(
		USB_HOST_AUDIO_V1_OBJ  audioObj,
		USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
		USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
		uint8_t channelNumber,
		uint16_t *nSubRanges	 
	);

  Summary:
    Schedules a control request to an Audio Device Feature Unit to get number Sub-ranges Supported by the 
	volume control on the specified channel. 

  Description:
    Schedules a control request to the Audio Device Feature Unit to get number Sub-ranges Supported by the 
	volume control on the specified channel. Prior to calling this function user should check if
    volume control exists on the specified channel by calling
    USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeExists() function. 
 
    If the request was scheduled successfully, requestHandle will contain a
    request handle that uniquely identifies this request. If the request
    could not be scheduled successfully, requestHandle will contain
    USB_HOST_AUDIO_V1_REQUEST_HANDLE_INVALID.
 
    When the control request completes, the Audio v1.0 client driver will call
    callback function which was set using the
    USB_HOST_AUDIO_V1_EntityRequestCallbackSet() function. The context
    parameter specified here will be returned in the callback. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 
  
    requestHandle - Output parameter that will contain the handle to this
    request.
 
    channelNumber - Channel Number to which the volume control is addressed to. 
   
    nSubRanges -  Output Parameter that will contain Number of Sub-ranges when request is
    completed and a callback is received. 

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - the request was scheduled successfully.
      requestHandle will contain a valid request handle.
    - USB_HOST_AUDIO_V1_RESULT_BUSY - The control request mechanism is currently
      busy.  Retry the request. 
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - an unknown failure occurred
      requestHandle will contain USB_HOST_AUDIO_V1_0_REQUEST_HANDLE_INVALID
    - USB_HOST_AUDIO_V1_RESULT_PARAMETER_INVALID - The data pointer or
      requestHandle pointer is NULL
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSubRangeNumbersGet
(
	USB_HOST_AUDIO_V1_OBJ  audioObj,
    USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
    USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
    uint8_t channelNumber,
	uint16_t *nSubRanges	 
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeRangeGet
	(
		 USB_HOST_AUDIO_V1_OBJ  audioObj,
		 USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
		 USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
		 uint8_t channelNumber,
		 void * data, 
		 size_t size
	);

  Summary:
    Schedules a control request to the Audio Device Feature Unit to get Range Supported by the 
	volume control on the specified channel. 

  Description:
    Schedules a control request to the Audio Device Feature Unit to get Range Supported by the 
	volume control on the specified channel. 
	
	Prior to calling this function user invoke
    USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSubRangeNumbersGet() function to know how many 
	Sub-ranges are supported.

	User should calculate 'size' parameter of this function as follows. 
	
	size = Size of Number of ranges  +  nSubRanges*( Size (MIN) + Size (MAX) + Size of (RES) ) 
	
    If the request was scheduled successfully, requestHandle will contain a
    request handle that uniquely identifies this request. If the request
    could not be scheduled successfully, requestHandle will contain
    USB_HOST_AUDIO_V1_REQUEST_HANDLE_INVALID.
 
    When the control request completes, the Audio v1.0 client driver will call
    callback function which was set using the
    USB_HOST_AUDIO_V1_EntityRequestCallbackSet() function. The context
    parameter specified here will be returned in the callback. 
     
  Parameters:
    audioObj     - USB Host Audio v1.0 device object. 

    entityObject - Audio control entity Object 
  
    requestHandle - Output parameter that will contain the handle to this
    request.
 
    channelNumber - Channel Number to which the volume control is addressed to. 
   
    nSubRanges -  Output Parameter that will contain Number of Sub-ranges when request is
    completed and a callback is received. 

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - the request was scheduled successfully.
      requestHandle will contain a valid request handle.
    - USB_HOST_AUDIO_V1_RESULT_BUSY - The control request mechanism is currently
      busy.  Retry the request. 
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - an unknown failure occurred
      requestHandle will contain USB_HOST_AUDIO_V1_0_REQUEST_HANDLE_INVALID
    - USB_HOST_AUDIO_V1_RESULT_PARAMETER_INVALID - The data pointer or
      requestHandle pointer is NULL
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeRangeGet
(
     USB_HOST_AUDIO_V1_OBJ  audioObj,
     USB_HOST_AUDIO_V1_CONTROL_ENTITY_OBJ entityObject, 
     USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
     uint8_t channelNumber,
	 void * data, 
	 size_t size
);

// ****************************************************************************
// ****************************************************************************
// Section: Audio Stream Access Functions
// ****************************************************************************
// ****************************************************************************

// ****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_STREAM_HANDLE USB_HOST_AUDIO_V1_StreamOpen
    ( 
        USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ audiostreamingInterfaceObj  
    );

  Summary:
    This function opens the specified Audio Stream. 

  Description:
    This function will open the specified Audio Stream. Once opened, the Audio
    stream can be accessed via the handle which this function returns. The
    audiostreamingInterfaceObj parameter is the value returned in the
    USB_HOST_AUDIO_V1_StreamingInterfaceGetFirst() or
    USB_HOST_AUDIO_V1_StreamingInterfaceGetNext() functions.

  Precondition:
    Audio streaming interface object should be valid.

  Input:
    audiostreamingInterfaceObj - Audio streaming interface object 

  Return:
    Will return a valid handle if the audio stream could be opened
    successfully, else will return USB_HOST_AUDIO_V1_RESULT_HANDLE_INVALID. The
    function will return a valid handle if the stream is ready to be opened.
    
  Example:
    <code>
    </code>

  Remarks:
    None.                                                                   
*/

USB_HOST_AUDIO_V1_STREAM_HANDLE USB_HOST_AUDIO_V1_StreamOpen
( 
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_OBJ audiostreamingInterfaceObj  
); 

// ****************************************************************************
/* Function:
    void USB_HOST_AUDIO_V1_StreamClose
    ( 
        USB_HOST_AUDIO_V1_STREAM_HANDLE audioSteamHandle
    );
           
  Summary:
    This function closes the Audio Stream. 

  Description:
    This function will close the open Audio Stream. This closes the association
    between the application entity that opened the audio stream and the audio
    stream. The audio stream handle becomes invalid.

  Precondition:
    None.

  Input:
    audioSteamHandle - handle to the audio stream obtained from the
    USB_HOST_AUDIO_V1_StreamOpen() function.

  Return:
    None.
    
  Example:
    <code>
    </code>

  Remarks:
    The device handle becomes invalid after calling this function.                                                                   
*/

void USB_HOST_AUDIO_V1_StreamClose
( 
    USB_HOST_AUDIO_V1_STREAM_HANDLE audioStreamHandle
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamEventHandlerSet
    (
        USB_HOST_AUDIO_V1_STREAM_HANDLE handle,
        USB_HOST_AUDIO_V1_STREAM_EVENT_HANDLER appAudioHandler,
        uintptr_t context
    ); 

  Summary:
    Registers an event handler with the Audio v1.0 Client Driver Stream.

  Description:
    This function registers a client specific Audio v1.0 stream event handler.
    The Audio v1.0 Host Client Driver will call appAudioHandler function
    specified as 2nd argument with relevant event and associate event data, in
    response to audio stream data transfers that have been scheduled by the
    client.
    
  Precondition:
    None.

  Parameters:
    handle  - handle to the Audio v1.0 Stream.

    eventHandler - A pointer to event handler function. If NULL, then events
                   will not be generated.
    
    context - Application specific context that is returned in the event handler.

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/

USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamEventHandlerSet
(
    USB_HOST_AUDIO_V1_STREAM_HANDLE handle,
    USB_HOST_AUDIO_V1_STREAM_EVENT_HANDLER appAudioHandler,
    uintptr_t context
); 

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceSet
    (
        USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle, 
        USB_INTERFACE_DESCRIPTOR* pInterfaceDesc,
        USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle
    ); 

  Summary:
    Schedules a SET_INTERFACE request to the specified audio stream. 

  Description:
    This function schedules an Audio stream enable request for the specified
    audio stream. An audio stream must be enable before scheduling any data
    transfer with the stream. An event
    USB_HOST_AUDIO_V1_STREAM_EVENT_ENABLE_COMPLETE is generated when  this
    request is completed. USB_HOST_AUDIO_V1_STREAM_EVENT_ENABLE_COMPLETE_DATA
    returns the status and request handle of the request. 
        
       
  Precondition:
    Audio stream should have been opened. Only one audio stream from an audio
    stream group can be enabled at a time. 

  Parameters:
    streamHandle  - Handle to the Audio v1.0 Stream.
    requestHandle - Handle to the Stream Enable request. 

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/

USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamingInterfaceSet
(
    USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle, 
    USB_HOST_AUDIO_V1_REQUEST_HANDLE * requestHandle,
    USB_HOST_AUDIO_V1_STREAMING_INTERFACE_SETTING_OBJ interfaceSettingObj
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamSamplingFrequencySet
    (
        USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
        USB_HOST_AUDIO_V1_REQUEST_HANDLE requestHandle,
        uint32_t *samplingFrequency
    )
  Summary:
    Schedules an  Audio stream Set Sampling rate request for the specified 
    audio stream. 

  Description:
    This function schedules an  Audio stream Set Sampling rate request for the
    specified audio stream. An event 
    USB_HOST_AUDIO_V1_STREAM_EVENT_SAMPLING_RATE_SET_COMPLETE is generated 
    when this request is completed. 
    USB_HOST_AUDIO_V1_STREAM_EVENT_SAMPLING_RATE_SET_COMPLETE_DATA returns 
    the status and request handle of the request. 
        
       
  Precondition:
    Audio stream should have been opened. 

  Parameters:
    streamHandle  - Handle to the Audio v1.0 Stream.
    requestHandle - Handle to the Stream Set Sampling rate request
    samplingRate  - Pointer to Sampling Rate

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/

USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamSamplingFrequencySet
(
    USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
    USB_HOST_AUDIO_V1_REQUEST_HANDLE *requestHandle,
    uint32_t *samplingFrequency
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamSamplingFrequencyGet
    (
        USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
        USB_HOST_AUDIO_V1_REQUEST_HANDLE requestHandle,
        uint32_t *samplingFrequency
    )
  Summary:
    Schedules an  Audio stream Get Sampling rate request for the specified 
    audio stream. 

  Description:
    This function schedules an  Audio stream Set Sampling rate request for the
    specified audio stream. An event 
    USB_HOST_AUDIO_V1_STREAM_EVENT_SAMPLING_RATE_SET_COMPLETE is generated 
    when this request is completed. 
    USB_HOST_AUDIO_V1_STREAM_EVENT_SAMPLING_RATE_SET_COMPLETE_DATA returns 
    the status and request handle of the request. 
        
       
  Precondition:
    Audio stream should have been opened. 

  Parameters:
    streamHandle  - Handle to the Audio v1.0 Stream.
    requestHandle - Handle to the Stream Set Sampling rate request
    samplingRate  - Pointer to Sampling Rate

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/

USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamSamplingFrequencyGet
(
    USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
    USB_HOST_AUDIO_V1_REQUEST_HANDLE *requestHandle,
    uint32_t *samplingFrequency
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamWrite 
    (
        USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
        USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE * transferHandle,
        void * source, 
        size_t length
    ); 

  Summary:
    Schedules an  Audio stream Write request for the specified audio stream. 

  Description:
    This function schedules an  Audio stream Write request for the specified 
    audio stream. An event 
    USB_HOST_AUDIO_V1_STREAM_EVENT_WRITE_COMPLETE is generated when this 
    request is completed. 
    USB_HOST_AUDIO_V1_STREAM_EVENT_WRITE_COMPLETE_DATA returns 
    the status and request handle of the request.
    
  Precondition:
    Audio stream should have been opened and enabled. The direction of the 
    Audio Stream should be USB_HOST_AUDIO_V1_DIRECTION_OUT. 

  Parameters:
    streamHandle    - Handle to the Audio v1.0 Stream.

    transferHandle  - Handle to the Stream Write transfer request 

    source          - Pointer to the buffer containing data to be written to the 
                      device. 

    length          - Amount of data to written (in bytes).

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/

USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamWrite 
(
    USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
    USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE * transferHandle,
    void * source, 
    size_t length
); 

// *****************************************************************************
/* Function:
   USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamRead 
   (
       USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
       USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE * transferHandle,
       void * source, 
       size_t length
   ); 

  Summary:
    Schedules an  Audio stream Read request for the specified audio stream. 

  Description:
    This function schedules an  Audio stream Read request for the specified 
    audio stream. An event 
    USB_HOST_AUDIO_V1_STREAM_EVENT_READ_COMPLETE is generated when this 
    request is completed. 
    USB_HOST_AUDIO_V1_STREAM_EVENT_READ_COMPLETE_DATA returns 
    the status and request handle of the request.
    
  Precondition:
    Audio stream should have been opened and enabled. The direction of the 
    Audio Stream should be USB_HOST_AUDIO_V1_DIRECTION_IN. 

  Parameters:
    streamHandle    - Handle to the Audio v1.0 Stream.

    transferHandle  - Handle to the Stream Read transfer request 

    source          - Pointer to the buffer containing data to be read from the 
                      device. 

    length          - Amount of data to read (in bytes).

  Returns:
    - USB_HOST_AUDIO_V1_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.

*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_StreamRead 
(
    USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
    USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE * transferHandle,
    void * source, 
    size_t length
);

// *****************************************************************************
// Section: Global Data Types. This section is specific to PIC32 implementation
//          of the USB Host Audio V1 Client Driver
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB HOST Audio V1 Client Driver Interface
 
  Summary:
    USB HOST Audio V1 Client Driver Interface

  Description:
    This macro should be used by the application in TPL table while adding
    support for the USB Audio V1 Host Client Driver.

  Remarks:
    None.
*/ 
/*DOM-IGNORE-BEGIN*/extern const USB_HOST_CLIENT_DRIVER gUSBHostAudioV1Driver; /*DOM-IGNORE-END*/
#define USB_HOST_AUDIO_V1_INTERFACE  /*DOM-IGNORE-BEGIN*/&gUSBHostAudioV1Driver /*DOM-IGNORE-END*/

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************      
// *****************************************************************************
// Section: Depreciated API _ Not recommended for new applications 
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Host Audio v1.0 Object 
 
  Summary: 
    Defines the type of the Audio v1.0 Host Client Object.

  Description:
    This type defines the type of the Audio Host Client Object. This type
    is returned by the Attach Event Handler and is used by the application to
    open the attached Audio v1.0 Device.  

  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_OBJ USB_HOST_AUDIO_V1_OBJ

// *****************************************************************************
/* USB Host Audio v1.0 Stream Object 
 
  Summary: 
    Defines the type of the Audio v1.0 Host Stream Object. 

  Description:
    This type defines the type of the Audio v1.0 Host Stream Object. This type
    is returned by USB_AUDIO_V1_0_StreamGetFirst() and USB_AUDIO_V1_0_StreamGetNext()
    as part of USB_HOST_AUDIO_V1_0_STREAM_INFO structure.  

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_AUDIO_V1_0_STREAM_OBJ;

// *****************************************************************************
/*  USB Host Audio v1.0 Client Driver Request Handle

  Summary:
     USB Host Audio v1.0 Client Driver Request Handle

  Description:
    This is returned by the Audio v1.0 Client driver command routines and should 
    be used by the application to track the command especially in cases where
    transfers are queued.

  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_REQUEST_HANDLE USB_HOST_AUDIO_V1_REQUEST_HANDLE

// *****************************************************************************
/*  USB Host Audio v1.0 Client Driver Invalid Request Handle

  Summary:
     USB Host Audio v1.0 Client Driver Invalid Request Handle

  Description:
    This is returned by the Audio v1.0 Client driver command routines when the request
    could not be scheduled.

  Remarks:
    None.
*/

#define USB_HOST_AUDIO_V1_0_REQUEST_HANDLE_INVALID ((USB_HOST_AUDIO_V1_0_REQUEST_HANDLE)(-1))

// *****************************************************************************
/* USB HOST Audio Client Driver Interface
 
  Summary:
    USB HOST Audio Client Driver Interface

  Description:
    This macro should be used by the application in TPL table while adding
    support for the USB Audio Host Client Driver.

  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_INTERFACE (void*)USB_HOST_AUDIO_V1_INTERFACE

// *****************************************************************************
/* USB Host Audio stream handle

  Summary:
    Defines the type of the Audio v1.0 Host Stream Handle.

  Description:
    This type defines the type of the handle returned by 
    USB_HOST_AUDIO_V1_0_StreamOpen() function. This application uses this
    handle to interact with an  Audio Stream. 


  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_STREAM_HANDLE USB_HOST_AUDIO_V1_STREAM_HANDLE

// *****************************************************************************
/* USB Host Audio stream Invalid handle

  Summary:
    Defines the type of the Audio v1.0 Host Stream Invalid Handle.

  Description:
    This is returned by the USB_HOST_AUDIO_V1_0_StreamOpen() function when 
    stream open failed.  

  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_STREAM_HANDLE_INVALID USB_HOST_AUDIO_V1_STREAM_HANDLE_INVALID

// *****************************************************************************
/*  USB Host Audio v1.0 Class Driver Transfer Handle

  Summary:
     USB Host Audio v1.0 Class Driver Transfer Handle

  Description:
    This is returned by the Audio v1.0 Class driver command and data transfer 
    routines and should be used by the application to track the transfer 
    especially in cases where transfers are queued.

  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_STREAM_TRANSFER_HANDLE USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE

// *****************************************************************************
/*  USB Host Audio v1.0 Class Driver Invalid Transfer Handle Definition
 
  Summary:
     USB Host Audio v1.0 Class Driver Invalid Transfer Handle Definition.

  Description:
    This definition defines a  USB Host Audio v1.0 Class Driver Invalid Transfer
    Handle. A Invalid Transfer Handle is returned by the Audio v1.0 Class Driver
    data and command transfer routines when the request was not successful. 

  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_STREAM_TRANSFER_HANDLE_INVALID USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE_INVALID

// *****************************************************************************
/* USB Host Audio v1.0 Class Driver Result enumeration.
 
  Summary:
    USB Host Audio v1.0 Class Driver Audio Result enumeration.

  Description:
    This enumeration lists the possible USB Host Audio v1.0 Class Driver operation
    results. These values are returned by Audio v1.0 Class Driver functions.

  Remarks:
    None.
*/

typedef enum
{
	
	/* The transfer or request could not be scheduled because internal
     * queues are full. The request or transfer should be retried */
    USB_HOST_AUDIO_V1_0_RESULT_BUSY  = /*  DOM-IGNORE-BEGIN */ USB_HOST_RESULT_REQUEST_BUSY, /* DOM-IGNORE-END*/

	/* The transfer or requested was aborted */
    USB_HOST_AUDIO_V1_0_RESULT_TRANSFER_ABORTED,
    
	/* The request was stalled */    
    USB_HOST_AUDIO_V1_0_RESULT_REQUEST_STALLED,

	/* The specified Audio v1.0 Object is Invalid */ 
    USB_HOST_AUDIO_V1_0_RESULT_OBJ_INVALID,

    /* No more Audio Stream present in the Device */       
    USB_HOST_AUDIO_V1_0_RESULT_END_OF_STREAM_LIST,
    
	/* DOM-IGNORE-BEGIN */
    USB_HOST_AUDIO_V1_0_RESULT_ERROR_INTERFACE_UNKNOWN, 
    /* DOM-IGNORE-END*/
	
	/* A required parameter was invalid */
    USB_HOST_AUDIO_V1_0_RESULT_PARAMETER_INVALID, 
    
	/* DOM-IGNORE-BEGIN */
    USB_HOST_AUDIO_V1_0_RESULT_CONFIGURATION_UNKNOWN, 

    USB_HOST_AUDIO_V1_0_RESULT_BUS_NOT_ENABLED,

    USB_HOST_AUDIO_V1_0_RESULT_BUS_UNKNOWN,
	/* DOM-IGNORE-END*/
	
    /* The specified device does not exist in the system */
    USB_HOST_AUDIO_V1_0_RESULT_DEVICE_UNKNOWN,

    /* An unknown failure has occurred */
    USB_HOST_AUDIO_V1_0_RESULT_FAILURE,

    /* Indicates a false condition */
    USB_HOST_AUDIO_V1_0_RESULT_FALSE = 0,

    /* Indicate a true condition */
    USB_HOST_AUDIO_V1_0_RESULT_TRUE = 1,

    /* Indicates that the operation succeeded or the request was accepted and
       will be processed. */
    USB_HOST_AUDIO_V1_0_RESULT_SUCCESS = USB_HOST_RESULT_TRUE
}
USB_HOST_AUDIO_V1_0_RESULT;

// *****************************************************************************
/* USB Host Audio v1.0 Stream Result enumeration.
 
  Summary:
    USB Host Audio v1.0 Stream Result enumeration.

  Description:
    This enumeration lists the possible USB Host Audio v1.0 Stream operation
    results. These values are returned by Audio v1.0 Stream functions.

  Remarks:
    None.
*/
typedef enum 
{
    /* The transfer or request could not be scheduled because internal
     * queues are full. The request or transfer should be retried */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_REQUEST_BUSY =  USB_HOST_RESULT_REQUEST_BUSY,
            
    /* Request was aborted */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_TRANSFER_ABORTED,
            
    /* Request was stalled */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_REQUEST_STALLED,

    /* The specified Stream Handle is not valid */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_HANDLE_INVALID,
 
    /* The end of the device list was reached.*/
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_END_OF_DEVICE_LIST,
    
    /* The specified interface is not available */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_INTERFACE_UNKNOWN,
   
    /* A NULL parameter was passed to the function */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_PARAMETER_INVALID,

    /* The specified configuration does not exist on this device.*/
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_CONFIGURATION_UNKNOWN, 

    /* A bus operation was requested but the bus was not operated */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_BUS_NOT_ENABLED,

    /* The specified bus does not exist in the system */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_BUS_UNKNOWN,
 
    /* The specified Audio Stream does not exist in the system */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_UNKNOWN,

    /* An unknown failure has occurred */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_FAILURE,

    /* Indicates a false condition */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_FALSE = 0,

    /* Indicate a true condition */
    USB_HOST_AUDIO_V1_0_STREAM_RESULT_TRUE = 1,

    /* Indicates that the operation succeeded or the request was accepted and
       will be processed. */
    USB_HOST_AUDIO_V1_0_STREAM_SUCCESS = USB_HOST_RESULT_TRUE
    
}USB_HOST_AUDIO_V1_0_STREAM_RESULT;

// *****************************************************************************
/* USB Host Audio v1.0 Event Handler Return Type 
 
  Summary: 
    Return type of the USB Audio v1.0 Host Client Driver Event Handler.

  Description:
    This enumeration list the possible return values of the USB Audio v1.0 Host
    Client Driver Event Handler.

  Remarks:
    None.
*/      
#define USB_HOST_AUDIO_V1_0_STREAM_EVENT_RESPONSE USB_HOST_AUDIO_V1_STREAM_EVENT_RESPONSE

// *****************************************************************************
/* USB Host Audio v1.0 Stream Event Handler Return Type 
 
  Summary: 
    Return type of the USB Host Audio v1.0 Stream Event Handler.

  Description:
    This enumeration list the possible return values of the USB Host Audio v1.0 
    Stream Event Handler.

  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_STREAM_EVENT_RESPONSE_NONE USB_HOST_AUDIO_V1_STREAM_EVENT_RESPONSE_NONE

// *****************************************************************************
/* Audio v1.0 Class Driver Events

  Summary:
    Identifies the possible events that the Audio v1.0 Class Driver can generate.
 
 Description:
    This enumeration identifies the possible events that the Audio v1.0 Class Driver
    can generate. The application should register an event handler using the
    USB_HOST_AUDIO_V1_0_AttachEventHandlerSet() function to receive Audio v1.0 Class 
    Driver events.
*/
#define USB_HOST_AUDIO_V1_0_EVENT USB_HOST_AUDIO_V1_EVENT
#define USB_HOST_AUDIO_V1_0_EVENT_ATTACH USB_HOST_AUDIO_V1_EVENT_ATTACH
#define USB_HOST_AUDIO_V1_0_EVENT_DETACH USB_HOST_AUDIO_V1_EVENT_DETACH  

// *****************************************************************************
/* Audio v1.0 Stream Events

  Summary:
    Identifies the possible events that the Audio v1.0 Stream can generate.

  Description:
    This enumeration identifies the possible events that the Audio v1.0 Stream
    can generate. The application should register an event handler using the
    USB_HOST_AUDIO_V1_0_StreamEventHandlerSet() function to receive Audio v1.0 
    stream events.
    
    An event may have data associated with it.  Events that are generated due to
    a transfer of data between the host and device are accompanied by data
    structures that provide the status of the transfer termination. For example,
    the USB_HOST_AUDIO_V1_0_STREAM_EVENT_READ_COMPLETE event is accompanied by a
    pointer to a USB_HOST_AUDIO_V1_0_STREAM_EVENT_READ_COMPLETE_DATA data
    structure. The transferStatus member of this data structure indicates the
    success or failure of the transfer. A transfer may fail due to device not
    responding on the bus, if the device stalls any stages of the transfer or
    due to NAK time-outs. The event description provides details on the nature of
    the event and the data that is associated with the event.
*/

typedef enum
{
    /* This event occurs when a Audio v1.0 Stream Read operation has completed
       i.e when the data has been received from the connected Audio v1.0 Stream.
       This event is generated after the application calls the 
       USB_HOST_AUDIO_V1_0_StreamRead() function. The eventData parameter in the
       event call back function will be of a pointer to a 
       USB_HOST_AUDIO_V1_0_STREAM_EVENT_READ_COMPLETE_DATA structure. This
       contains details about the transfer handle associated with this read
       request, the amount of data read and the termination status of the read
       request. */
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_READ_COMPLETE,

    /* This event occurs when an Audio v1.0 Stream Write operation has completed
       i.e when the data has been written to the connected Audio v1.0 Stream. 
       This event is generated after the application calls the 
       USB_HOST_AUDIO_V1_0_StreamWrte()function. The eventData parameter in the 
       event call back function will be of a pointer to a 
       USB_HOST_AUDIO_V1_0_STREAM_EVENT_WRITE_COMPLETE_DATA structure. This
       contains details about the transfer handle associated with this write
       request, the amount of data written and the termination status of the 
       write request. */
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_WRITE_COMPLETE,

   /*This event occurs when an Audio v1.0 Stream Enable request has been 
      completed. This event is generated after the application calls the 
      USB_HOST_AUDIO_V1_0_StreamEnable() function. The eventData parameter in the
      event call back function will be of a pointer to a 
      USB_HOST_AUDIO_V1_0_STREAM_EVENT_ENABLE_COMPLETE_DATA. This contains details
      about the request handle associated with this Stream Enable request and the
      termination status of the Stream Enable request.*/ 
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_ENABLE_COMPLETE, 
    
    /*This event occurs when an Audio v1.0 Stream Disable request has been 
      completed. This event is generated after the application calls the 
      USB_HOST_AUDIO_V1_0_StreamDisable() function. The eventData parameter in the
      event call back function will be of a pointer to a 
      USB_HOST_AUDIO_V1_0_STREAM_EVENT_DISABLE_COMPLETE_DATA. This contains details
      about the request handle associated with this Stream Disable request and the
      termination status of the Stream Disable request.*/
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_DISABLE_COMPLETE,
    
    /*This event occurs when an Audio v1.0 Sampling Rate Set request has been 
      completed. This event is generated after the application calls the 
      USB_HOST_AUDIO_V1_0_StreamSamplingRateSet() function. The eventData 
      parameter in the event call back function will be of a pointer to a 
      USB_HOST_AUDIO_V1_0_STREAM_EVENT_SAMPLING_RATE_SET_COMPLETE_DATA. This 
      contains details about the request handle associated with this Sampling 
      Rate Set request and the termination status of the Stream Disable request.*/         
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_SAMPLING_RATE_SET_COMPLETE,       
}   
USB_HOST_AUDIO_V1_0_STREAM_EVENT;

// *****************************************************************************
/*  USB Host Audio v1.0 Class Driver Stream Direction
 
  Summary:
     USB Host Audio v1.0 Class Driver Stream Direction

  Description:
   

  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_STREAM_DIRECTION USB_HOST_AUDIO_V1_STREAM_DIRECTION
#define USB_HOST_AUDIO_V1_0_DIRECTION_OUT USB_HOST_AUDIO_V1_DIRECTION_OUT
#define USB_HOST_AUDIO_V1_0_DIRECTION_IN USB_HOST_AUDIO_V1_DIRECTION_IN 

// *****************************************************************************
/* USB Host Audio stream Info table structure

  Summary:
    This structure defines USB Host Audio Stream Info Structure. 

  Description:
     This structure is an out parameter to the functions 
     USB_HOST_AUDIO_V1_0_StreamGetFirst() and USB_HOST_AUDIO_V1_0_StreamGetNext()
     functions. This structure contains information about an audio stream in the 
     attached Audio Device. This structure contains Stream object, Audio format etc.      

  Remarks:
    None.
*/
#if defined (USB_HOST_AUDIO_V1_0_SAMPLING_FREQUENCIES_NUMBER) && !defined (USB_HOST_AUDIO_V1_SAMPLING_FREQUENCIES_NUMBER)
 #define USB_HOST_AUDIO_V1_SAMPLING_FREQUENCIES_NUMBER USB_HOST_AUDIO_V1_0_SAMPLING_FREQUENCIES_NUMBER
 #endif
typedef struct
{
    /* Audio Stream Object. Clients need to pass this object when opening this 
       Audio Stream using USB_HOST_AUDIO_V1_0_StreamOpen() function. */ 
    USB_HOST_AUDIO_V1_0_STREAM_OBJ streamObj;

    /* Audio Format code for this Stream */ 
    USB_AUDIO_FORMAT_CODE format;

    /* Stream direction */ 
    USB_HOST_AUDIO_V1_0_STREAM_DIRECTION streamDirection;

    /* Number of physical channels in the audio stream */ 
    uint8_t nChannels;

    /* Number of bytes occupied by one audio sub-frame */ 
    uint8_t subFrameSize;
    
    /* Number of effectively used bits from the available bits in an audio sub-frame */ 
    uint8_t bitResolution;

    /* Indicates how the sampling frequency can be programmed:
        0: Continuous sampling frequency
        1..255: Number of discrete sampling frequencies supported by Audio stream
                   */ 
    uint8_t nSamplingRates;
    
    /* Supported sampling Frequencies */ 
    uint32_t tSamFreq[USB_HOST_AUDIO_V1_SAMPLING_FREQUENCIES_NUMBER]; 

} USB_HOST_AUDIO_V1_0_STREAM_INFO;
// *****************************************************************************
/*  USB Host Audio v1.0 Class Stream Transfer Event Data.
 
  Summary:
     USB Host Audio v1.0 Class Stream Transfer Event Data.

  Description:
    This data type defines the data structure returned by the Audio V1.0 client 
    driver along with the following events:
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_READ_COMPLETE_DATA,
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_WRITE_COMPLETE_DATA,

  Remarks:
    None.
*/
#define USB_HOST_AUDIO_V1_0_STREAM_EVENT_WRITE_COMPLETE_DATA USB_HOST_AUDIO_V1_STREAM_EVENT_WRITE_COMPLETE_DATA
#define USB_HOST_AUDIO_V1_0_STREAM_EVENT_READ_COMPLETE_DATA USB_HOST_AUDIO_V1_STREAM_EVENT_READ_COMPLETE_DATA

// *****************************************************************************
/* USB Host Audio v1.0 Class Stream Control Event Data.
 
  Summary:
     USB Host Audio v1.0 Class Stream Control Event Data.

  Description:
    This data type defines the data structure returned by the Audio V1.0 client 
    driver along with the following events:
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_ENABLE_COMPLETE_DATA,
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_DISABLE_COMPLETE_DATA

  Remarks:
    None.
*/

typedef struct
{
    /* Transfer handle of this transfer */
    USB_HOST_AUDIO_V1_0_REQUEST_HANDLE  requestHandle; 

    /* Transfer termination status */
    USB_HOST_AUDIO_V1_0_RESULT requestStatus;

} 
USB_HOST_AUDIO_V1_0_STREAM_EVENT_ENABLE_COMPLETE_DATA,
USB_HOST_AUDIO_V1_0_STREAM_EVENT_DISABLE_COMPLETE_DATA;

// *****************************************************************************
/* USB Host Audio v1.0 Client Driver Attach Event Handler Function Pointer Type.

  Summary:
    USB Host Audio v1.0 Client Driver Attach Event Handler Function Pointer Type.

  Description:
    This data type defines the required function signature of the USB Host Audio v1.0
    Client Driver attach event handling callback function. The application must
    register a pointer to a Audio v1.0 Client Driver attach events handling function
    whose function signature (parameter and return value types) match the types
    specified by this function pointer in order to receive attach and detach events call
    backs from the Audio v1.0 Client Driver. The client driver will invoke this
    function with event relevant parameters. The description of the event
    handler function parameters is given here.

    audioObj - Handle of the client to which this event is directed.
    event -    event indicates if it is an attach or detach 
    context - Value identifying the context of the application that was
    registered along with  the event handling function.

  Remarks:
    None.
*/
typedef void (* USB_HOST_AUDIO_V1_0_ATTACH_EVENT_HANDLER)
             (
                 USB_HOST_AUDIO_V1_0_OBJ audioObj, 
                 USB_HOST_AUDIO_V1_0_EVENT event, 
                 uintptr_t context
              );
			  
// *****************************************************************************
/* USB Host Audio v1.0 Class Driver Stream Event Handler Function Pointer Type.

  Summary:
    USB Host Audio v1.0 Class Driver Stream Event Handler Function Pointer Type.

  Description:
    This data type defines the required function signature of the USB Host 
    Audio v1.0 Class Driver Stream event handling callback function. The 
    application must register a pointer to a Audio v1.0 Class Driver stream
    events handling function whose function signature (parameter and return
    value types) match the types specified by this function pointer in order
    to receive event call backs from the Audio v1.0 Class Driver. The class 
    driver will invoke this function with event relevant parameters. The 
    description of the event handler function parameters is given here.

    handle           - Handle to the Audio v1.0 Stream 
    
    event            - Type of event generated.
    
    eventData        - This parameter should be type casted to an event specific
                       pointer type based on the event that has occurred. Refer 
                       to the USB_HOST_AUDIO_V1_0_STREAM_EVENT enumeration 
                       description for more details.
    
    context          - Value identifying the context of the application that 
                       was registered along with  the event handling function.

  Remarks:
    None.
*/

typedef USB_HOST_AUDIO_V1_0_STREAM_EVENT_RESPONSE (* USB_HOST_AUDIO_V1_0_STREAM_EVENT_HANDLER )
 (  USB_HOST_AUDIO_V1_0_STREAM_HANDLE handle,
    USB_HOST_AUDIO_V1_0_STREAM_EVENT event,
    void * eventData,
    uintptr_t context );
	
// *****************************************************************************
/* USB Host Audio v1.0 Class driver Control Transfer Complete Callback Function
   Pointer type

  Summary:
   USB Host Audio v1.0 Class driver Control Transfer Complete Callback Function
   Pointer Type

  Description:
   This data type defines the required function signature of the USB Host 
   Audio v1.0 class driver Control Transfer Complete callback function. The 
   client must provide a pointer to a Control Transfer Complete Callback 
   function who's function signature (parameter and return value types) must 
   match the types specified by this function pointer in order to receive 
   notification when a control transfer has completed. The pointer to the 
   callback function must be specified in USB_HOST_AUDIO_V1_0_ControlRequest() 
   function. The Audio v1.0 client driver will invoke this function with event
   relevant parameters. The description of the event handler function parameters
   is given here.

   audioObj - Audio v1.0 client driver object associated with this event.

   requestHandle - request handle of the control transfer request that caused
   this event.

   result - completion result of the control transfer. This will be
   USB_HOST_AUDIO_V1_0_RESULT_SUCCESS if the control transfer completed 
   successfully,
   USB_HOST_AUDIO_V1_0_RESULT_FAILURE if an unknown failure occurred or
   USB_HOST_AUDIO_V1_0_RESULT_REQUEST_STALLED if the request was stalled.

   size - size of the data stage that was transferred.

   context - Value identifying the context of the application that was
   provided when the USB_HOST_AUDIO_V1_0_ControlRequest() function was called.

  Remarks:
    None.
*/
typedef void (* USB_HOST_AUDIO_V1_0_CONTROL_CALLBACK)
(
    USB_HOST_AUDIO_V1_0_OBJ audioObj, 
    USB_HOST_AUDIO_V1_0_REQUEST_HANDLE requestHandle,
    USB_HOST_AUDIO_V1_0_RESULT result,
    size_t size,
    uintptr_t context
); 

// ****************************************************************************
// ****************************************************************************
// Section: Client Access Functions
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_0_RESULT USB_HOST_AUDIO_V1_0_AttachEventHandlerSet
    (
        USB_HOST_AUDIO_V1_0_ATTACH_EVENT_HANDLER eventHandler,
        uintptr_t context
    );

  Summary:
    This function will set an attach/detach event handler.

  Description:
    This function will set an attach event handler. The attach event handler
    will be called when a Audio v1.0 device has been attached or detached. The 
    context will be returned in the event handler. This function should be 
    called before the bus has been enabled.
    
  Precondition:
    None.

  Parameters:
    eventHandler - Pointer to the attach event handler
    
    context      - An application defined context that will be returned in the event
                   handler.

  Returns:
    - USB_HOST_AUDIO_V1_0_RESULT_SUCCESS - if the attach event handler was registered
    successfully. 

    - USB_HOST_AUDIO_V1_0_RESULT_FAILURE - if the number of registered event 
    handlers has exceeded USB_HOST_AUDIO_V1_0_ATTACH_LISTENERS_NUMBER.
    
  Example:
    <code> 
    </code>

  Remarks:
    Function should be called before USB_HOST_BusEnable() function is called.

 */
 #define USB_HOST_AUDIO_V1_0_AttachEventHandlerSet USB_HOST_AUDIO_V1_AttachEventHandlerSet
 
 // ****************************************************************************
/* Function:
    USB_HOST_DEVICE_OBJ_HANDLE USB_HOST_AUDIO_V1_0_DeviceObjHandleGet
    (
        USB_HOST_AUDIO_V1_0_OBJ audioDeviceObj
    );
           
  Summary:
    This function returns the Device Object Handle for this Audio v1.0 device.

  Description:
    This function returns the Device Object Handle for this Audio v1.0 device. This
    returned Device Object Handle can be used by the application to perform
    device level operations such as getting the string descriptors.

  Precondition:
    None.

  Input:
    audioDeviceObj - Audio V1.0 device object handle returned in the
    USB_HOST_AUDIO_V1_0_ATTACH_EVENT_HANDLER function.

  Return:
    Will return a valid device object handle if the device is still connected to
    the system. Will return an USB_HOST_DEVICE_OBJ_HANDLE_INVALID otherwise.
    
  Example:
    <code>
    </code>

  Remarks:
    None.                                                                   
*/
#define USB_HOST_AUDIO_V1_0_DeviceObjHandleGet USB_HOST_AUDIO_V1_DeviceObjHandleGet

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_0_RESULT USB_HOST_AUDIO_V1_0_ControlRequest
    (
        USB_HOST_AUDIO_V1_0_OBJ audioObj,
        USB_HOST_AUDIO_V1_0_REQUEST_HANDLE * requestHandle,
        USB_SETUP_PACKET *setupPacket,
        void * data,
        USB_HOST_AUDIO_V1_0_CONTROL_CALLBACK callback, 
        uintptr_t context 
    );

  Summary:
    Schedules an Audio v1.0 control transfer.

  Description:
    This function schedules an Audio v1.0 control transfer. audioObj is an 
    Object of Audio v1.0 class driver to which the audio control transfer is to
    be scheduled. setupPacket points to the SETUP command to be sent in the 
    SETUP Stage of the control transfer. The size and the direction of the data
    stage is indicated by the SETUP packet. In case of control transfers where
    there is no data stage, data is ignored and can be NULL. In all other cases,
    data should point to the data to data be transferred in the data stage of 
    the control transfer. 
    
    If the transfer was scheduled successfully, requestHandle will contain a
    transfer handle that uniquely identifies this transfer. If the transfer
    could not be scheduled successfully, requestHandle will contain
    USB_HOST_AUDIO_V1_0_REQUEST_HANDLE_INVALID.

    When the control transfer completes, the Audio v1.0 client driver will call
    the specified callback function. The context parameter specified here will 
    be returned in the callback.

  Precondition:
    Audio v1.0 Device should have attached. 

  Parameters:
    audioObj - Audio v1.0 client driver object 
    requestHandle - output parameter that will contain the handle to this
    transfer
    setupPacket - Pointer to the SETUP packet to sent to the device in the SETUP
    stage of the control transfer
    data -  For control transfer with a data stage, this should point to data to
    be sent to the device (for a control write transfer) or point to the buffer
    that will receive data from the device (for a control read transfer). For
    control transfers that do not require a data stage, this parameter is
    ignored and can be NULL.
    callback - pointer to the callback function that will be called when the
    control transfer completes. If the callback function is NULL, there will be
    no notification of when the control transfer will complete.
    context - user defined context that is returned with the callback function

  Returns:
    - USB_HOST_AUDIO_V1_0_RESULT_SUCCESS - the transfer was scheduled successfully.
      requestHandle will contain a valid transfer handle.
    - USB_HOST_AUDIO_V1_0_RESULT_FAILURE - an unknown failure occurred. requestHandle will
      contain USB_HOST_AUDIO_V1_0_REQUEST_HANDLE_INVALID.
    - USB_HOST_AUDIO_V1_0_RESULT_PARAMETER_INVALID - The data pointer or requestHandle pointer
      is NULL

  Example:
    <code>
    </code>

  Remarks:
    None.
*/
USB_HOST_AUDIO_V1_0_RESULT USB_HOST_AUDIO_V1_0_ControlRequest
(
    USB_HOST_AUDIO_V1_0_OBJ OBJ,
    USB_HOST_AUDIO_V1_0_REQUEST_HANDLE * transferHandle,
    USB_SETUP_PACKET *setupPacket,
    void * data,
    USB_HOST_AUDIO_V1_0_CONTROL_CALLBACK callback, 
    uintptr_t context 
);

// *****************************************************************************
/* Function:
    uint8_t USB_HOST_AUDIO_V1_0_NumberOfStreamGroupsGet
    (
        USB_HOST_AUDIO_V1_0_OBJ audioObj
    );

  Summary:
    This function will Get number of stream groups present in the attached 
    Audio v1.0 Device. 

  Description:
    This function will Get number of stream groups present in the attached 
    Audio v1.0 Device. Audio stream with in a audio streams cannot be enabled
    at the same time. 
    
  Precondition:
    Audio v1.0 device should have been attached. 

  Parameters:
    audioObj - Audio v1.0 client driver object 

  Returns:
    Returned uint8_t indicates number of audio stream groups present in the 
    attached Audio v1.0 device. 
    
  Example:
    <code> 
    </code>

  Remarks:

 */
uint8_t USB_HOST_AUDIO_V1_0_NumberOfStreamGroupsGet
(
    USB_HOST_AUDIO_V1_0_OBJ audioObj
);
        
// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_0_RESULT USB_HOST_AUDIO_V1_0_StreamGetFirst 
    (
        USB_HOST_AUDIO_V1_0_OBJ audioDeviceObj, 
        uint8_t streamGroupIndex, 
        USB_HOST_AUDIO_V1_0_STREAM_INFO * streamInfo
    );

  Summary:
    Returns information about first audio stream in the specified Audio stream 
    group. 

  Description:
    This function returns information about first audio stream in the specified
    Audio stream group. The stream group index is parameter to this function 
    and it can be any value starting from Zero to number of stream groups minus
    one. Number of stream groups can be obtained by using 
    USB_HOST_AUDIO_V1_0_NumberOfStreamGroupsGet() function. 
    
    streamInfo object is an out parameter to this function. 
    
  Precondition:
    Audio v1.0 device should have been attached to the Host. 

  Parameters:
    audioDeviceObj   - Audio v1.0 client driver object

    streamGroupIndex - Stream Group Index.  
    
    streamInfo       -  Pointer to streamInfo object 

  Returns:
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_0_RESULT_OBJ_INVALID - The specified Audio v1.0 client
      driver object does not exist
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/
USB_HOST_AUDIO_V1_0_RESULT USB_HOST_AUDIO_V1_0_StreamGetFirst 
(
    USB_HOST_AUDIO_V1_0_OBJ audioDeviceObj, 
    uint8_t streamGroupIndex, 
    USB_HOST_AUDIO_V1_0_STREAM_INFO * streamInfo
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_0_RESULT USB_HOST_AUDIO_V1_0_StreamGetNext 
    (
        USB_HOST_AUDIO_V1_0_STREAM_OBJ audioStreamObj,
        USB_HOST_AUDIO_V1_0_STREAM_INFO * streamInfo
    );

  Summary:
    Returns information about next audio stream in the specified Audio stream 
    group.

  Description:
    This function returns information about next audio stream in the specified
    Audio stream group. USB_HOST_AUDIO_V1_0_StreamGetFirst() function should 
    have been called at least once on the same Audio stream group before calling 
    this function. Then calling this function repeatedly on the stream group 
    will return information about the next audio stream in the stream group. 
    When there are no more audio streams to report, the function returns 
    USB_HOST_AUDIO_V1_0_RESULT_END_OF_STREAM_LIST. 
    
    Calling the USB_HOST_AUDIO_V1_0_StreamGetFirst() function on the stream group
    index after the USB_HOST_AUDIO_V1_0_StreamGetNext() function has been called 
    will cause Audio v1.0 client driver to reset the audio stream group to point
    to the first stream in the stream group.
    
  Precondition:
    The USB_HOST_AUDIO_V1_0_StreamGetFirst() function must have been called
    before calling this function.

  Parameters:
    audioStreamObj   -  Present Audio Stream object.   
    
    streamInfo       -  Pointer to streamInfo object 

  Returns:
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_0_RESULT_OBJ_INVALID - The specified Audio v1.0 client
      driver object does not exist
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_FAILURE - An unknown failure occurred
    - USB_HOST_AUDIO_V1_0_RESULT_END_OF_STREAM_LIST - There are no more audio 
    streams in the stream group
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/ 
USB_HOST_AUDIO_V1_0_RESULT USB_HOST_AUDIO_V1_0_StreamGetNext 
(
    USB_HOST_AUDIO_V1_0_STREAM_OBJ audioStreamObj,
    USB_HOST_AUDIO_V1_0_STREAM_INFO * streamInfo
); 

// ****************************************************************************
// ****************************************************************************
// Section: Audio Stream Access Functions
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_0_STREAM_HANDLE USB_HOST_AUDIO_V1_0_StreamOpen
    ( 
        USB_HOST_AUDIO_V1_0_STREAM_OBJ audioStreamObj  
    );
           
  Summary:
    This function opens the specified Audio Stream. 

  Description:
    This function will open the specified Audio Stream. Once opened, the Audio
    stream can be accessed via the handle which this function returns. The
    audioStreamObj parameter is the value returned in the
    USB_HOST_AUDIO_V1_0_StreamGetFirst() or USB_HOST_AUDIO_V1_0_StreamGetNext()
    functions.

  Precondition:
    Audio stream object should be valid.

  Input:
    audioStreamObj - Audio Stream object. 

  Return:
    Will return a valid handle if the audio stream could be opened successfully,
    else will return USB_HOST_AUDIO_V1_0_STREAM_RESULT_HANDLE_INVALID. The 
    function will return a valid handle if the stream is ready to be opened.
    
  Example:
    <code>
    </code>

  Remarks:
    None.                                                                   
*/
#define USB_HOST_AUDIO_V1_0_StreamOpen USB_HOST_AUDIO_V1_StreamOpen

// ****************************************************************************
/* Function:
    void USB_HOST_AUDIO_V1_0_StreamClose
    ( 
        USB_HOST_AUDIO_V1_0_STREAM_HANDLE audioSteamHandle
    );
           
  Summary:
    This function closes the Audio Stream. 

  Description:
    This function will close the open Audio Stream. This closes the association
    between the application entity that opened the audio stream and the audio 
    stream. The audio stream handle becomes invalid.

  Precondition:
    None.

  Input:
    audioSteamHandle - handle to the audio stream obtained from the
    USB_HOST_AUDIO_V1_0_StreamOpen() function.

  Return:
    None.
    
  Example:
    <code>
    </code>

  Remarks:
    The device handle becomes invalid after calling this function.                                                                   
*/
#define USB_HOST_AUDIO_V1_0_StreamClose USB_HOST_AUDIO_V1_StreamClose

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamEventHandlerSet
    (
        USB_HOST_AUDIO_V1_0_STREAM_HANDLE handle,
        USB_HOST_AUDIO_V1_0_STREAM_EVENT_HANDLER appAudioHandler,
        uintptr_t context
    ); 

  Summary:
    Registers an event handler with the Audio v1.0 Client Driver Stream.

  Description:
    This function registers a client specific Audio v1.0 stream event handler.
    The Audio v1.0 Host Client Driver will call appAudioHandler function 
    specified as 2nd argument with relevant event and associate event data, in
    response to audio stream data transfers that have been scheduled by the 
    client.
    
  Precondition:
    None.

  Parameters:
    handle  - handle to the Audio v1.0 Stream.

    eventHandler - A pointer to event handler function. If NULL, then events
                   will not be generated.
    
    context - Application specific context that is returned in the event handler.

  Returns:
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/

USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamEventHandlerSet
(
    USB_HOST_AUDIO_V1_0_STREAM_HANDLE handle,
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_HANDLER appAudioHandler,
    uintptr_t context
); 

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamEnable
    (
        USB_HOST_AUDIO_V1_0_STREAM_HANDLE streamHandle, 
        USB_HOST_AUDIO_V1_0_REQUEST_HANDLE * requestHandle
    ); 

  Summary:
    Schedules an  Audio stream enable request for the specified audio stream. 

  Description:
    This function schedules an Audio stream enable request for the specified 
    audio stream. An audio stream must be enable before scheduling any data 
    transfer with the stream. An event 
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_ENABLE_COMPLETE is generated when  this 
    request is completed. USB_HOST_AUDIO_V1_0_STREAM_EVENT_ENABLE_COMPLETE_DATA 
    returns the status and request handle of the request. 
        
       
  Precondition:
    Audio stream should have been opened. Only one audio stream from an audio
    stream group can be enabled at a time. 

  Parameters:
    streamHandle  - Handle to the Audio v1.0 Stream.
    requestHandle - Handle to the Stream Enable request. 

  Returns:
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/
USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamEnable
(
    
    USB_HOST_AUDIO_V1_0_STREAM_HANDLE streamHandle, 
    USB_HOST_AUDIO_V1_0_REQUEST_HANDLE * requestHandle
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamDisable
    (
        USB_HOST_AUDIO_V1_0_STREAM_HANDLE streamHandle, 
        USB_HOST_AUDIO_V1_0_REQUEST_HANDLE * requestHandle
    ); 

  Summary:
    Schedules an  Audio stream disable request for the specified audio stream. 

  Description:
    This function schedules an Audio stream disable request for the specified 
    audio stream. An event 
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_DISABLE_COMPLETE is generated when this
    request is completed. USB_HOST_AUDIO_V1_0_STREAM_EVENT_DISABLE_COMPLETE_DATA 
    returns the status and request handle of the request. 
        
       
  Precondition:
    Audio stream should have been opened. 

  Parameters:
    streamHandle  - Handle to the Audio v1.0 Stream.
    requestHandle - Handle to the Stream Disable request. 

  Returns:
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/
USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamDisable
(
    USB_HOST_AUDIO_V1_0_STREAM_HANDLE streamHandle,
    USB_HOST_AUDIO_V1_0_REQUEST_HANDLE *requestHandle
);

// *****************************************************************************
/* Function:
    USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamSamplingRateSet
    (
        USB_HOST_AUDIO_V1_0_STREAM_HANDLE streamHandle,
        USB_HOST_AUDIO_V1_0_REQUEST_HANDLE requestHandle,
        uint32_t* samplingRate
    ); 

  Summary:
    Schedules an  Audio stream Set Sampling rate request for the specified 
    audio stream. 

  Description:
    This function schedules an  Audio stream Set Sampling rate request for the
    specified audio stream. An event 
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_SAMPLING_RATE_SET_COMPLETE is generated 
    when this request is completed. 
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_SAMPLING_RATE_SET_COMPLETE_DATA returns 
    the status and request handle of the request. 
        
       
  Precondition:
    Audio stream should have been opened. 

  Parameters:
    streamHandle  - Handle to the Audio v1.0 Stream
    requestHandle - Handle to the Stream Set Sampling rate request
    samplingRate  - Pointer to Sampling Rate

  Returns:
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_HANDLE_INVALID - The specified audio 
      stream does not exist
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_FAILURE - An unknown failure occurred
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/
USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamSamplingRateSet
(
    USB_HOST_AUDIO_V1_0_STREAM_HANDLE streamHandle,
    USB_HOST_AUDIO_V1_0_REQUEST_HANDLE *requestHandle,
    uint32_t *samplingRate
);

// *****************************************************************************
/* Function:
   USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamWrite 
   (
       USB_HOST_AUDIO_V1_0_STREAM_HANDLE streamHandle,
       USB_HOST_AUDIO_V1_0_STREAM_TRANSFER_HANDLE * transferHandle,
       void * source, 
       size_t length
   ); 

  Summary:
    Schedules an  Audio stream Write request for the specified audio stream. 

  Description:
    This function schedules an  Audio stream Write request for the specified 
    audio stream. An event 
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_WRITE_COMPLETE is generated when this 
    request is completed. 
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_WRITE_COMPLETE_DATA returns 
    the status and request handle of the request.
    
  Precondition:
    Audio stream should have been opened and enabled. The direction of the 
    Audio Stream should be USB_HOST_AUDIO_V1_0_DIRECTION_OUT. 

  Parameters:
    streamHandle    - Handle to the Audio v1.0 Stream.

    transferHandle  - Handle to the Stream Write transfer request 

    source          - Pointer to the buffer containing data to be written to the 
                      device. 

    length          - Amount of data to written (in bytes).

  Returns:
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_HANDLE_INVALID - The specified audio 
    stream does not exist.
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_FAILURE - An unknown failure occurred.
    
  Example:
    <code>
    </code>

  Remarks:
    None.
*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_0_StreamWrite 
(
    USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
    USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE * transferHandle,
    void * source, 
    size_t length
);

// *****************************************************************************
/* Function:
   USB_HOST_AUDIO_V1_0_STREAM_RESULT USB_HOST_AUDIO_V1_0_StreamRead 
   (
       USB_HOST_AUDIO_V1_0_STREAM_HANDLE streamHandle,
       USB_HOST_AUDIO_V1_0_STREAM_TRANSFER_HANDLE * transferHandle,
       void * source, 
       size_t length
   ); 

  Summary:
    Schedules an  Audio stream Read request for the specified audio stream. 

  Description:
    This function schedules an  Audio stream Read request for the specified 
    audio stream. An event 
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_READ_COMPLETE is generated when this 
    request is completed. 
    USB_HOST_AUDIO_V1_0_STREAM_EVENT_READ_COMPLETE_DATA returns 
    the status and request handle of the request.
    
  Precondition:
    Audio stream should have been opened and enabled. The direction of the 
    Audio Stream should be USB_HOST_AUDIO_V1_0_DIRECTION_IN. 

  Parameters:
    streamHandle    - Handle to the Audio v1.0 Stream.

    transferHandle  - Handle to the Stream Read transfer request 

    source          - Pointer to the buffer containing data to be read from the 
                      device. 

    length          - Amount of data to read (in bytes).

  Returns:
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_SUCCESS - The operation was successful
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_HANDLE_INVALID - The specified audio 
    stream does not exist.
    - USB_HOST_AUDIO_V1_0_STREAM_RESULT_FAILURE - An unknown failure occurred.
    
  Example:
    <code>
    </code>

  Remarks:
    None
*/
USB_HOST_AUDIO_V1_RESULT USB_HOST_AUDIO_V1_0_StreamRead 
(
    USB_HOST_AUDIO_V1_STREAM_HANDLE streamHandle,
    USB_HOST_AUDIO_V1_STREAM_TRANSFER_HANDLE * transferHandle,
    void * source, 
    size_t length
);
#include "usb/src/usb_host_audio_v1_mapping.h"
#endif 

