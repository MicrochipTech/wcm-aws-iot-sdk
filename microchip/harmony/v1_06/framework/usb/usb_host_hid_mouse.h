/********************************************************************************
  USB HOST HID Mouse Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hid_mouse.h

  Summary:
    USB Host HID Mouse Driver Definition Header

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface between HID Mouse driver
	and top level application.
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
#ifndef _USB_HOST_HID_MOUSE_H_
#define _USB_HOST_HID_MOUSE_H_


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/usb_host_hid.h"


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
// *****************************************************************************
// Section: USB HID Mouse Driver Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Host HID Mouse Driver Result Minimum Constant

  Summary:
    USB Host HID Mouse Driver Result Minimum Constant.

  Description:
    Constant identifying the USB Host HID Mouse Driver Result Minimum Value.
	Constant is used in the USB_HOST_HID_MOUSE_RESULT enumeration.

  Remarks:
    None.
*/
#define USB_HOST_HID_MOUSE_RESULT_MIN -50


// *****************************************************************************
/* USB HOST HID Mouse Driver Events

  Summary:
    Defines the possible USB HOST HID Mouse Driver Events

  Description:
    This enumeration lists the possible Mouse events that Mouse driver can
    provide to application.
    Some of these events have event data associated with them.
 */

typedef enum
{
    /* Mouse has been attached */
    USB_HOST_HID_MOUSE_EVENT_ATTACH = 0,
	/* Mouse has been detached */
	USB_HOST_HID_MOUSE_EVENT_DETACH,
	/* Mouse IN Report data available */
    USB_HOST_HID_MOUSE_EVENT_REPORT_RECEIVED

} USB_HOST_HID_MOUSE_EVENT;


// *****************************************************************************
/* USB Host HID MOUSE Result

  Summary:
    USB Host HID Mouse driver Results.

  Description:
    This enumeration defines the possible returns values of USB Host HID Mouse
    driver API. A function may only return some of the values in this 
    enumeration. Refer to function description for details on which values will
    be returned.

  Remarks:
    None.
*/

typedef enum
{	
    /* An unknown failure occurred */
    USB_HOST_HID_MOUSE_RESULT_FAILURE = USB_HOST_HID_MOUSE_RESULT_MIN,
	
    /* Invalid or NULL parameter passed */
    USB_HOST_HID_MOUSE_RESULT_INVALID_PARAMETER,
	
    /* Indicates that the operation succeeded or the request was accepted and
       will be processed. */
    USB_HOST_HID_MOUSE_RESULT_SUCCESS = 0

} USB_HOST_HID_MOUSE_RESULT;


// *****************************************************************************
/* USB HOST HID Mouse Driver instance Handle

  Summary:
    USB HOST HID Mouse Driver instance Handle

  Description:
    This defines a USB Host HID Mouse Driver Handle.

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_HID_MOUSE_HANDLE;

#define USB_HOST_HID_MOUSE_HANDLE_INVALID ((USB_HOST_HID_MOUSE_HANDLE)(-1))


// *****************************************************************************
/* USB Host HID Mouse Data Object

  Summary:
    Defines the USB Host HID Mouse Data Object.

  Description:
    Defines the USB Host HID Mouse Data Object.

  Remarks:
*/

typedef struct
{
    /* Button state for the buttons. USB_HOST_HID_MOUSE_BUTTONS_NUMBER is
	   system configurable option. The actual number of buttons in the mouse
	   needs to be <= USB_HOST_HID_MOUSE_BUTTONS_NUMBER
	 */
    USB_HID_BUTTON_STATE buttonState[USB_HOST_HID_MOUSE_BUTTONS_NUMBER];
    USB_HID_BUTTON_ID buttonID [USB_HOST_HID_MOUSE_BUTTONS_NUMBER];
	/* X - Coordinate displacement */
	int16_t xMovement; /* Applicable for 2D Mouse */
	/* Y - Coordinate displacement */
	int16_t yMovement; /* Applicable for 2D Mouse */
	/* Z - Coordinate displacement */
	int16_t zMovement;/* Applicable only for 3D Mouse */
    
} USB_HOST_HID_MOUSE_DATA;


// *****************************************************************************
/* USB HOST Mouse Driver Event Handler Function Pointer Type.

  Summary:
    USB HOST Mouse Driver Event Handler Function Pointer Type.

  Description:
    This defines the USB HOST HID Mouse Driver event handler function
    pointer type. Application must register a function of this type to
    receive HID mouse events. Registration should happen before USB BUS is
	enabled by the application.

*/
typedef void (*USB_HOST_HID_MOUSE_EVENT_HANDLER)
(
    /* Unique Handle passed to the caller. This will be unique
       for each Mouse interface attached
	 */
    USB_HOST_HID_MOUSE_HANDLE handle,
	/* Associated event type */
    USB_HOST_HID_MOUSE_EVENT event,
	/* Associated event data. For USB_HOST_HID_MOUSE_EVENT_ATTACH
       and USB_HOST_HID_MOUSE_EVENT_DETACH this will be NULL.
       For USB_HOST_HID_MOUSE_EVENT_REPORT_RECEIVED event, this
	   will be USB_HOST_HID_MOUSE_DATA data type
	 */
	void *pData
);


// *****************************************************************************
// *****************************************************************************
// Section: MOUSE Driver Function Definitions
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void _USB_HOST_HID_MOUSE_Task(USB_HOST_HID_OBJ_HANDLE handle);
void _USB_HOST_HID_MOUSE_EventHandler
(
    USB_HOST_HID_OBJ_HANDLE handle,
    USB_HOST_HID_EVENT event,
    void * eventData
);


// *****************************************************************************
/* Function:
    USB_HOST_HID_MOUSE_RESULT USB_HOST_HID_MOUSE_EventHandlerSet
	(
		USB_HOST_HID_MOUSE_EVENT_HANDLER appMouseEventHandler
	);

  Summary:
    This function registers application callback function with Mouse driver

  Description:
    This function registers application callback function with Mouse driver.
    Any subsequent Mouse events is passed to the application by calling the
    registered application function.
	Function prototype should be of USB_HOST_HID_MOUSE_EVENT_HANDLER type.
    
  Precondition:
    This function should be called before USB BUS is enabled.

  Parameters:
    appMouseEventHandler  - Function pointer to the application function.

  Returns:
    Returns data structure of USB_HOST_HID_MOUSE_RESULT type.
	  USB_HOST_HID_MOUSE_RESULT_INVALID_PARAMETER: Invalid Parameter
	  USB_HOST_HID_MOUSE_RESULT_FAILURE: On failure
	  USB_HOST_HID_MOUSE_RESULT_SUCCESS: On success
    
  Example:
    <code>
    // This code snippet shows an example of registering event handler
   
    </code>

  Remarks:
    This function should be called before USB BUS is enabled
*/
USB_HOST_HID_MOUSE_RESULT USB_HOST_HID_MOUSE_EventHandlerSet
(
    USB_HOST_HID_MOUSE_EVENT_HANDLER appMouseEventHandler
);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}

#endif
//DOM-IGNORE-END

#endif

/*********** End of file ***************************************/
