/*******************************************************************************
  USB Device Layer Compile Time Options

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_config_template.h

  Summary:
    USB device configuration template header file.

  Description:
    This file contains USB device layer compile time options (macros) that are
    to be configured by the user. This file is a template file and must be used
    as an example only. This file must not be directly included in the project.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _USB_DEVICE_CONFIG_TEMPLATE_H_
#define _USB_DEVICE_CONFIG_TEMPLATE_H_

#error (" This is a template file and must not be included directly in the project" );

// *****************************************************************************
// *****************************************************************************
// Section: USB Device configuration Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Number of Device Layer Instances.
 
  Summary:
    Number of Device Layer instances to provisioned in the application.

  Description:
    This configuration macro defines the number of Device Layer instances in the
    application. In cases of microcontrollers that feature multiple USB
    peripherals, this allows the application to run multiple instances of the
    Device Layer.
    
  Remarks:
    None.
*/

#define USB_DEVICE_INSTANCES_NUMBER 1

// *****************************************************************************
/* Endpoint 0 Buffer Size
 
  Summary:
    Buffer Size in Bytes for Endpoint 0. 

  Description:
    This number defines the size (in bytes) of Endpoint 0. For High Speed USB Devices, 
    this number should be 64. For Full Speed USB Devices, this number can be 8, 16, 32 
    or 64 bytes. This number will be applicable to all USB Device Stack instances.
    
  Remarks:
    None.
*/

#define USB_DEVICE_EP0_BUFFER_SIZE 

// *****************************************************************************
/* USB Device Layer SOF Event Enable
 
  Summary:
    Enables the Device Layer SOF event.

  Description:
    Specifying this configuration macro will cause the USB Device Layer to
    generate the USB_DEVICE_EVENT_SOF event.  

  Remarks:
    None.
*/

#define USB_DEVICE_SOF_EVENT_ENABLE

// *****************************************************************************
/* USB Device Layer Set Descriptor Event Enable
 
  Summary:
    Enables the Device Layer Set Descriptor Event.

  Description:
    Specifying this configuration macro will cause the USB Device Layer to
    generate the USB_DEVICE_EVENT_SET_DESCRIPTOR event. If this macro is not
    defined, the USB Device Layer will stall the control transfer request
    associated with this event.

  Remarks:
    None.
*/

#define USB_DEVICE_SET_DESCRIPTOR_EVENT_ENABLE

// *****************************************************************************
/* USB Device Layer Synch Frame Event Enable
 
  Summary:
    Enables the Device Layer Synch Frame Event.

  Description:
    Specifying this configuration macro will cause the USB Device Layer to
    generate the USB_DEVICE_EVENT_SYNCH_FRAME event. If this macro is not
    defined, the USB Device Layer will stall the control transfer request
    associated with this event.

  Remarks:
    None.
*/

#define USB_DEVICE_SYNCH_FRAME_EVENT_ENABLE

// *****************************************************************************
/* USB Device Layer Combined Endpoint Queue Depth
 
  Summary:
    Specifies the combined endpoint queue depth in case of a vendor USB device
    implementation.

  Description:
    This configuration constant specifies the combined endpoint queue depth in a
    case where the endpoint read and endpoint write functions are used to
    implement a vendor USB device. This constant should be used in conjunction
    with the usb_device_endpoint_functions.c file.

    This macro defines the number of entries in all IN and OUT endpoint queues
    in all instances of the USB Device Layer. This value can be obtained by
    adding up the endpoint read and write queue sizes of each USB Device Layer
    instance . In a simple single instance USB Device Layer, that requires only
    one read and one write endpoint with one buffer each, the
    USB_DEVICE_ENDPOINT_QUEUE_DEPTH_COMBINED macro can be set to 2. Consider a
    case with one Device Layer instance using 2 IN and 2 OUT endpoints, each
    requiring 2 buffers, this macro should be set to 8 (2 + 2 + 2 + 2). 

  Remarks:
    This constant needs to be specified only if a Vendor USB Device is to be
    implemented and the usb_device_endpoint_functions.c file is included in the
    project. This constant does not have any effect on queues of other standard
    function drivers that are included in the USB device implementation.
*/

#define USB_DEVICE_ENDPOINT_QUEUE_DEPTH_COMBINED  2



#endif



