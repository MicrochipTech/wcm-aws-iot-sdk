/* ******************************************************************************
  USB Host HID Client Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hid.h

  Summary:
    USB Host HID Client Driver Interface Header

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the USB Host HID
    Client Driver.
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
#ifndef _USB_HOST_HID_H_
#define _USB_HOST_HID_H_


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/beta_sw/usb_host.h"
#include "usb/beta_sw/src/usb_host_local.h"
#include "usb/beta_sw/usb_hid.h"


// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
/* USB HOST HID Function Driver instance Handle

  Summary:
    USB HOST HID Function Driver instance Handle

  Description:
    This defines a USB Host HID Function Driver  Handle.
    A  Handle is owned by the application, USB_HOST_HID_OPEN() handle is valid
    till the detach event


  Remarks:
    None.
*/
typedef uintptr_t USB_HOST_HID_HANDLE;

// *****************************************************************************
/* USB HOST HID transfer result

  Summary:
    USB HOST HID transfer result

  Description:
    These macros are useful to identify  USB HOST HID transfer status.

 */
typedef enum
{
    /* The operation was successful */
    USB_HOST_HID_RESULT_OK = 1 ,
    /* The transfer size is invalid. Refer to the description
     * of the read or write function for more details */
    USB_HOST_HID_RESULT_ERROR_TRANSFER_SIZE_INVALID =
                                        USB_ERROR_IRP_SIZE_INVALID ,
    /* The transfer queue is full and no new transfers can be
     * scheduled */
    USB_HOST_HID_RESULT_ERROR_TRANSFER_QUEUE_FULL =
                                        USB_ERROR_IRP_QUEUE_FULL ,

    /* The specified instance is not provisioned in the system */
    USB_HOST_HID_INSTANCE_INVALID =
                                USB_ERROR_HOST_DEVICE_INSTANCE_INVALID ,

    /* The specified instance is not configured yet */
    USB_HOST_HID_RESULT_ERROR_INSTANCE_NOT_CONFIGURED =
                            USB_ERROR_HOST_DRIVER_NOT_READY ,

    /* invalid event handler */
    USB_HOST_HID_INVALID_HANDLER = - 15 ,

    USB_HOST_HID_ERROR = -1
}USB_HOST_HID_RESULT;

// *****************************************************************************
/* USB HOST HID driver state

  Summary:
    Identifies the different HID instance states.

  Description:
    These macros are useful to identify  USB HOST HID driver status.
 */
typedef enum
{
    USB_HOST_HID_ATTACHED                      = 1,
    USB_HOST_HID_DETACHED,
    USB_HOST_HID_SET_IDLE_SEND,
    USB_HOST_HID_SEND_SET_PROTOCOL,
    USB_HOST_HID_SET_PROTOCOL_SEND,
    USB_HOST_HID_READY,
    USB_HOST_HID_WAIT,
    USB_HOST_CLEAR_INTERRUPTIN_ENDPOINT,
    USB_HOST_CLEAR_INTERRUPTIN_ENDPOINT_DONE

}USB_HOST_HID_STATE;

// *****************************************************************************
/*  USB Host HID Class Driver Index

  Summary:
     USB Host HID Class Driver Index

  Description:
    This uniquely identifies a HID Class Driver instance.

*/
typedef uintptr_t USB_HOST_HID_INDEX;

#define USB_HOST_HID_HANDLE_INVALID ((USB_HOST_HID_HANDLE)(-1))

// *****************************************************************************
/*  USB Host HID device information

  Summary:
     USB Host HID device information

  Description:
    Structure represents the data structure passed to application on device
	attach or detach events.

*/
typedef struct _USB_HOST_HID_DEVICE_INFO_
{
    bool isKeyboardDevice;
    bool isBootInterfaceClass;
    uint8_t countryCode;
} USB_HOST_HID_DEVICE_INFO;


// *****************************************************************************
/* USB HOST HID Function Driver Events

  Summary:
    USB HOST HID Function Driver Events

  Description:
    These events are specific to the USB HOST HID  Function  Driver  instance.
    Each event description contains details on  the  values  of  the  parameters
    passed with event handler. The contents of pData depends on the generated
    event.
 */

typedef enum
{
    /* HID Attach event */
    USB_HOST_HID_EVENT_ATTACH,
    /* HID Detach event*/
    USB_HOST_HID_EVENT_DETACH,
    /* IN Report data available */
    USB_HOST_HID_EVENT_REPORT_RECEIVED,
    /* OUTPUT Report sent */
    USB_HOST_HID_EVENT_REPORT_SENT
} USB_HOST_HID_EVENT;

// *****************************************************************************
/* USB HOST HID Event Handler Function Pointer Type.

  Summary:
    USB HOST HID Event Handler Function Pointer Type.

  Description:
    This defines the USB HOST HID Function Driver event handler function
    pointer type. The application must register a function of this type to
    receive HID Function Driver events. The function driver will invoke this
    function with event relevant parameters.

*/
typedef bool (*USB_HOST_HID_EVENT_HANDLER)
(
    USB_HOST_HID_INDEX index,
    USB_HOST_HID_EVENT event,
    void* pData
);

// *****************************************************************************
// *****************************************************************************
// Section: HID Interface Function Definitions
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    USB_HOST_HID_HANDLE  USB_HOST_HID_Open( uint8_t  instance )

  Summary:
    This function opens HID client driver

  Description:
    This function opens HID client driver specific to the instance passed. 
    
  Precondition:
    This function should be called after HID device attach event is obtained
	by the application

  Parameters:
    instance        - Instance of the HID client Driver.

  Returns:
    USB_HOST_HID_HANDLE_INVALID - On failure
	HID client driver instance address - On success
    
  Example:
    <code>
    // This code snippet shows an example of opening USB HID client driver
   
  
    </code>

  Remarks:
    None.
*/
USB_HOST_HID_HANDLE  USB_HOST_HID_Open( uint8_t  instance );

// *****************************************************************************
/* Function:
    void  USB_HOST_HID_Close( USB_HOST_HID_HANDLE hidHandle )

  Summary:
    This function closes HID client driver

  Description:
    This function closes HID client driver specific to the handle passed,
    invalidating the handle.	
    
  Precondition:
    This function should be called after HID client driver is opened.

  Parameters:
    hidHandle        - Handle obtained during opening of HID client Driver.

  Returns:
    None.
    
  Example:
    <code>
    // This code snippet shows an example of closing USB HID client driver
   
  
    </code>

  Remarks:
    None.
*/
void    USB_HOST_HID_Close( USB_HOST_HID_HANDLE hidHandle );

// *****************************************************************************
/* Function:
    USB_ERROR USB_HOST_HID_EventHandlerSet ( USB_HOST_HID_EVENT_HANDLER appHIDHandler )

  Summary:
    This function registers application callback function with HID client driver

  Description:
    This function registers application callback function with HID client driver.
    Any subsequent HID events is passed to the application by calling the
    registered application function.
	Function prototype should be of USB_HOST_HID_EVENT_HANDLER type.
    
  Precondition:
    This function should be called after USB Host operation is enabled.

  Parameters:
    appHIDHandler  - Function pointer to the application function.

  Returns:
    None.
    
  Example:
    <code>
    // This code snippet shows an example of closing USB HID client driver
   
    </code>

  Remarks:
    None.
*/
USB_ERROR USB_HOST_HID_EventHandlerSet ( USB_HOST_HID_EVENT_HANDLER appHIDHandler );

#endif


/*********** End of file ***************************************/
