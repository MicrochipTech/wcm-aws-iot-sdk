 /*******************************************************************************
  USB HOST HID class definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hid_local.h

  Summary:
    USB HID class definitions

  Description:
    This file describes the HID class specific definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to  you  the  right  to  use,  modify,  copy  and  distribute
Software only when embedded on a Microchip  microcontroller  or  digital  signal
controller  that  is  integrated  into  your  product  or  third  party  product
(pursuant to the  sublicense  terms  in  the  accompanying  license  agreement).

You should refer  to  the  license  agreement  accompanying  this  Software  for
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
// DOM-IGNORE-END

#ifndef _USB_HOST_HID_LOCAL_H
#define _USB_HOST_HID_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/beta_sw/usb_host.h"
#include "usb/beta_sw/src/usb_host_local.h"
#include "usb/beta_sw/usb_host_hid.h"
#include "usb/beta_sw/usb_hid.h"


/***********************************************
 * Data buffers needed by for the HID function driver instance.
 ***********************************************/
volatile uint8_t hidReadBuffer[64] __attribute__((coherent)) __attribute__((aligned(4)));


// *****************************************************************************
/* USB HOST HID Function Driver data structure

  Summary:
    USB HOST HID Function Driver information

  Description:
    This structure has the details about USB HOST HID Function Driver info
    event handler function , states of device , instance , pipes info
    contain all the information about the device driver.

  Remarks:
    None.
 */


typedef struct _USB_HOST_HID_INSTANCE_ {
    /* use or not*/
    bool assigned;
    /* instance number */
    uint8_t instanceNumber;
    /* interface number owned by this instance */
    uint8_t interfaceNumber;
    /* Holds the task routine state of this HID instance */
    uint8_t       state;
    /* hcid + device id */
    USB_HOST_DEVICE_ID hidDeviceId;
    /* IRPS for each endpoint */
    USB_HOST_IRP controlIRP;
    USB_HOST_IRP interruptInIRP;
    /*Pipes for each endpoint */
    USB_HOST_PIPE_HANDLE controlPipeHandle;
    USB_HOST_PIPE_HANDLE interruptInPipeHandle;
    /* Interrupt IN endpoint address */
    uint8_t     interruptInEndpoint;
    /* Interrupt IN endpoint MaxPacketSize */
    uint8_t     interruptInEndpointSize;
    /* setup packet information */
    USB_SETUP_PACKET    setupPacket;
    /*connected HID devices*/
    uint8_t hidConnectedDevices;
    /* keep track of last setup request */
    uint8_t       setupRequest;
    /* Place holder for IDLE Rate */
    uint8_t       hidKeyRepeatDelay;
    /* Holds information about the device */
    USB_HOST_HID_DEVICE_INFO hidDeviceInfo;
    /* Application callback */
    USB_HOST_HID_EVENT_HANDLER appEventCallBack;
    /* For HID specific transfer protocol */
    volatile uint8_t  *hidReadBuffer;
    volatile uint8_t   lastNonmodifier;

} USB_HOST_HID_INSTANCE;

/*************************************************************************/
/* Function:
  USB_ERROR      _USB_HOST_HID_InterfaceInitialize( USB_HOST_DEVICE_ID id ,
                                                       uint8_t interfaceId,
                                                       uint8_t *hidInstance,
                                                       USB_SPEED speed);
  Summary:
    Initialize the HID host client driver.

  Description:
    After enumeration Host layer will initialize the HID driver.

  Parameters:
     USB_HOST_DEVICE_ID                     id ,
     uint8_t                                interfaceId,
     uint8_t *                              hidInstance
     USB_SPEED                              speed 

  Returns:
    USB_ERROR
*/
USB_ERROR _USB_HOST_HID_InterfaceInitialize
(
    USB_HOST_DEVICE_ID id ,
    uint8_t interfaceId,
    uint8_t *hidInstance,
    USB_SPEED speed
);

/*************************************************************************/
/* Function:
   void           _USB_HOST_HID_DeInitialize(USB_HOST_DEVICE_ID id )

  Summary:
    DeInitializes the HID host client driver.

  Description:
    After detach device Host will Deinitialize the HID client driver.

  Parameters:
     USB_HOST_DEVICE_ID                           id ,

  Returns:
        void
*/
void   _USB_HOST_HID_DeInitialize(USB_HOST_DEVICE_ID id );

/*************************************************************************/
/* Function:
   void _USB_HOST_HID_Tasks(uint8_t instanceNumber )

  Summary:
        State machine of the HID instance , handling errors 

  Description:


  Parameters:
        None

  Returns:
        None
 */


void _USB_HOST_HID_Tasks(uint8_t instanceNumber );


/*************************************************************************/
/* Function:
   uint8_t  _USB_HOST_HID_findInstance(USB_HOST_DEVICE_ID id )

  Summary:
    Find the HID instance

  Description:

  Parameters:
      USB_HOST_DEVICE_ID             id

  Returns:
    uint8_t                          instance Number
*/

uint8_t  _USB_HOST_HID_findInstance(USB_HOST_DEVICE_ID id );

/*************************************************************************/
/* Function:
   void   _USB_HOST_HID_InterruptTransferComplete( USB_HOST_IRP *irp)

  Summary:
        Call back function for Interrupt IRP status

  Description:

  Parameters:
        USB_HOST_IRP              *irp

  Returns:
        None
*/
void _USB_HOST_HID_InterruptTransferComplete( USB_HOST_IRP *irp);


/*************************************************************************/
/* Function:
   _USB_HOST_HID_ControlTransferComplete( USB_HOST_IRP *irp)

  Summary:
        Call back function for control transfer status

  Description:

  Parameters:
      USB_HOST_IRP                  *irp

  Returns:
   void
*/
void _USB_HOST_HID_ControlTransferComplete( USB_HOST_IRP *irp);

/*************************************************************************/
/* Function:
   USB_ERROR   _USB_HOST_HID_ClearEndpointHalt (
                                        uint8_t  instanceNumber ,
                                        uint8_t endpointNumber ,
                                        void* callback )

  Summary:
   Send a setup packet to clear the stall condition

  Description:

  Parameters:
      USB_HOST_IRP                  *irp

  Returns:
   void
*/
USB_ERROR   _USB_HOST_HID_ClearEndpointHalt
(
    uint8_t  instanceNumber ,
    uint8_t endpointNumber ,
    void* callback
);

/*************************************************************************/
/* Function:
   USB_ERROR   _USB_HOST_HID_setIdleRate (
                                       uint8_t  instanceNumber,
                                       uint16_t hidKeyRepeatDelay )

  Summary:
   Send a setup packet to send SET IDLE request

  Description:

  Parameters:
      USB_HOST_IRP                  *irp

  Returns:
   void
*/
USB_ERROR   _USB_HOST_HID_setIdleRate
(
    uint8_t  instanceNumber,
    uint16_t hidKeyRepeatDelay
);

/*************************************************************************/
/* Function:
   USB_ERROR   _USB_HOST_HID_setProtocol (
                                       uint8_t  instanceNumber,
                                       bool  protocolType )

  Summary:
   Send a setup packet to send SET PROTOCOL request

  Description:

  Parameters:
      USB_HOST_IRP                  *irp

  Returns:
   void
*/
USB_ERROR   _USB_HOST_HID_setProtocol
(
    uint8_t  instanceNumber,
    bool protocolType
);

USB_ERROR _USB_HOST_HID_sendReport( USB_HOST_HID_INSTANCE * hidInstnceInfo);

#endif

/********************** END OF FILE ***************************/
