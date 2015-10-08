/*******************************************************************************
  USB Device layer interface names mapping

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_mapping.h

  Summary:
    USB Device Layer Interface names mapping

  Description:
    This header file maps the interface prototypes in "usb_device.h" to
    static variants of these routines appropriate for the selected configuration.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _USB_DEVICE_MAPPING_H
#define _USB_DEVICE_MAPPING_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
/* Note:  See the bottom of file for implementation header include files.
*/

#include <stdint.h>
#include <stdbool.h>
#include "usb/src/usb_device_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: USB Driver API Mapping to USB device layer API
// *****************************************************************************
// *****************************************************************************
#define USB_CD_HANDLE_mac (((USB_DEVICE_OBJ *)handle)->usbCDHandle)

#define USB_DEVICE_EndpointEnable( handle, interface, epAddrs, epType, epSize )   \
        DRV_USB_DEVICE_EndpointEnable( (((USB_DEVICE_OBJ *)handle)->usbCDHandle), epAddrs, epType, epSize )
#define USB_DEVICE_EndpointDisable( handle, epAdrs )     \
        DRV_USB_DEVICE_EndpointDisable( (((USB_DEVICE_OBJ *)handle)->usbCDHandle), epAdrs )
#define USB_DEVICE_EndpointStall( handle, epAdrs )      \
        DRV_USB_DEVICE_EndpointStall((((USB_DEVICE_OBJ *)handle)->usbCDHandle), epAdrs )
#define USB_DEVICE_EndpointStallClear( handle, epAdrs )  \
        DRV_USB_DEVICE_EndpointStallClear( (((USB_DEVICE_OBJ *)handle)->usbCDHandle), epAdrs )
#define USB_DEVICE_EndpointIsEnabled( handle, epAdrs )   \
        DRV_USB_DEVICE_EndpointIsEnabled( (((USB_DEVICE_OBJ *)handle)->usbCDHandle), epAdrs )
#define USB_DEVICE_EndpointIsStalled( handle, epAdrs )   \
        DRV_USB_DEVICE_EndpointIsStalled( (((USB_DEVICE_OBJ *)handle)->usbCDHandle), epAdrs )
#define USB_DEVICE_IRPSubmit( handle, epAdrs, irp )       \
        DRV_USB_DEVICE_IRPSubmit( (((USB_DEVICE_OBJ *)handle)->usbCDHandle), epAdrs, irp )
#define USB_DEVICE_IRPCancelAll( handle, epAdrs )        \
        DRV_USB_DEVICE_IRPCancelAll( (((USB_DEVICE_OBJ *)handle)->usbCDHandle), epAdrs )
#define USB_DEVICE_Attach( handle )     DRV_USB_DEVICE_Attach( ((USB_DEVICE_OBJ *)handle)->usbCDHandle)
#define USB_DEVICE_Detach( handle )     DRV_USB_DEVICE_Detach( ((USB_DEVICE_OBJ *)handle)->usbCDHandle)


#if defined USB_DEVICE_SOF_EVENT_ENABLE
    #define _USB_DEVICE_Set_Sof_Event_type(x)  x=true
#else
    #define _USB_DEVICE_Set_Sof_Event_type(x)  x=false
#endif



#ifdef USB_DEVICE_ENDPOINT_QUEUE_DEPTH_COMBINED
    #define _USB_DEVICE_Initialize_Endpoint_Q(x,y,z) _USB_DEVICE_Initialize_Endpoint_Q_Size(x,y,z)
    #define _USB_DEVICE_EndpointCurrentQueueSizeReset(x)  _USB_DEVICE_EndpointQueueSizeReset(x)
    #define _USB_DEVICE_EndpointMutexCreate(x)       _USB_DEVICE_EndpointMutexCreateFunction(x)
    #define _USB_DEVICE_EndpointMutexDelete(x)       _USB_DEVICE_EndpointMutexDeleteFunction(x)
#else
    #define _USB_DEVICE_Initialize_Endpoint_Q(x,y,z)
    #define _USB_DEVICE_EndpointMutexCreate(x)
    #define _USB_DEVICE_EndpointCurrentQueueSizeReset(x)
    #define _USB_DEVICE_EndpointMutexDelete(x)
    #define _USB_DEVICE_EndpointDeclareOsalResult(x)
#endif 

#ifdef USB_DEVICE_SET_DESCRIPTOR_EVENT_ENABLE
    #define _USB_DEVICE_Handle_Set_Descriptor_Request(x,y,z) _USB_DEVICE_RedirectControlXfrToClient(x,y,z)
#else
    #define _USB_DEVICE_Handle_Set_Descriptor_Request(x,y,z) sendStatus = true
#endif

#ifdef USB_DEVICE_SYNCH_FRAME_EVENT_ENABLE
    #define _USB_DEVICE_Handle_Synch_Frame_Request(x,y,z) _USB_DEVICE_ForwardControlXfrToFunction(x,y,z)
#else
    #define _USB_DEVICE_Handle_Synch_Frame_Request(x,y,z) USB_DEVICE_ControlStatus((USB_DEVICE_HANDLE)x, USB_DEVICE_CONTROL_STATUS_ERROR )
#endif

#ifdef USB_DEVICE_BOS_DESCRIPTOR_SUPPORT_ENABLE 
    #define _USB_DEVICE_Handle_Get_Bos_Descriptor_Request(pBosDesc, data, size)  if (pBosDesc != NULL)\
                                                                                 {\
                                                                                     data = (uint8_t *)(pBosDesc);\
                                                                                     size = ((USB_BOS_DESCRIPTOR *)pBosDesc)->wTotalLength;\
                                                                                 }\

#else
    #define _USB_DEVICE_Handle_Get_Bos_Descriptor_Request(x,y,z) 
#endif

#endif // #ifndef _USB_DEVICE_MAPPING_H

/*******************************************************************************
 End of File
*/

