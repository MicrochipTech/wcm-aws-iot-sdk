 /*******************************************************************************
  USB CDC class definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_cdc_local.h

  Summary:
    USB CDC class definitions

  Description:
    This file describes the CDC class specific definitions.
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

#ifndef _USB_HOST_CDC_LOCAL_H
#define _USB_HOST_CDC_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/beta_sw/usb_host.h"
#include "usb/beta_sw/src/usb_host_local.h"
#include "usb/beta_sw/usb_host_cdc.h"


/* Configures DTE rate, stop-bits, parity, and number-of-character bits.*/
#define USB_CDC_GET_LINE_CODING             0x21
/* Number of bytes Line Coding transfer */
#define USB_CDC_LINE_CODING_LENGTH          0x07
/* Configures DTE rate, stop-bits, parity, and number-of-character bits. */
#define USB_CDC_SET_LINE_CODING             0x20
/* [V24] signal used to tell the DCE device the DTE device is now present. */
#define USB_CDC_SET_CONTROL_LINE_STATE      0x22
 /* Number of bytes Control line transfer */
#define USB_CDC_CONTROL_LINE_LENGTH         0x00
 /* Sends special carrier modulation used to specify [V24] style break. */
#define USB_CDC_SEND_BREAK                  0x23   
/* Reset the CDC device*/
#define USB_CDC_RESET           (0xFF)
#define MARK_RESET_RECOVERY     (0x0E)
// *****************************************************************************

/* USB HOST CDC Function Driver data structer

  Summary:
    USB HOST CDC Function Driver information

  Description:
    This structer has the details about USB HOST CDC instance info
    event handler function , states of device , instance , pipes info
    contain all the information about the device driver.

  Remarks:
    None.
 */

typedef struct _USB_HOST_CDC_INSTANCE_ {
    /* use or not*/
    bool assigned;
    /* instance number */
    uint8_t instanceNumber;
    uint8_t interfaceNumber;
    /* data interface */
    uint8_t datainterfaceNumber;
    /* communication interface*/
    uint8_t communicationInterfaceNumber;
    /* hcid + device id */
    USB_HOST_DEVICE_ID cdcDeviceId;
    /* IRPS for each endpoint */
    USB_HOST_IRP controlIRP;
    USB_HOST_IRP bulkInIRP;
    USB_HOST_IRP bulkOutIRP;
    USB_HOST_IRP interruptInIRP;
    /*Pipes for each endpoint */
    USB_HOST_PIPE_HANDLE controlPipeHandle;
    USB_HOST_PIPE_HANDLE bulkInPipeHandle;
    USB_HOST_PIPE_HANDLE bulkOutPipeHandle;
    USB_HOST_PIPE_HANDLE interruptInPipeHandle;
    /* setup packet information */
    USB_SETUP_PACKET    setupPacket;
    uintptr_t context;
    uint8_t   controlRequest;
    /* Application callback */
    USB_HOST_CDC_EVENT_HANDLER appEventCallBack;
} USB_HOST_CDC_INSTANCE;


/*************************************************************************/
/* Function:
   USB_ERROR  _USB_HOST_CDC_InterfaceInitialize( USB_HOST_DEVICE_ID id ,
                                                 uint8_t interfaceId,
                                                 uint8_t *cdcInstance
						 uint8_t speed)
  Summary:
    Initialize the CDC host driver.

  Description:
    After enumeration Host layer will initialize the CDC driver.
    
  Parameters:
     USB_HOST_DEVICE_ID                           id ,
     uint8_t                                interfaceId,
     uint8_t *                              cdcinstance

  Returns:
    None
*/
       /*Host Layer will call after Enumeration */
USB_ERROR      _USB_HOST_CDC_InterfaceInitialize( USB_HOST_DEVICE_ID id ,
                                                  uint8_t interfaceId,
                                                  uint8_t *cdcinstance,
						  USB_SPEED  speed);



/*************************************************************************/

/* Function:
   void  _USB_HOST_CDC_DeInitialize(USB_HOST_DEVICE_ID id )

  Summary:
    DeInitialize the CDC host driver.

  Description:
    After detach device Host will Deinitialize the CDC driver.
    

  Parameters:
     USB_HOST_DEVICE_ID                           id ,

  Returns:
        void
*/

void  _USB_HOST_CDC_DeInitialize ( USB_HOST_DEVICE_ID id  );

/*************************************************************************/

/* Function:
   _USB_HOST_CDC_findInstance(USB_HOST_DEVICE_ID id )

  Summary:
    Find the CDC instance

  Description:

  Parameters:
      USB_HOST_DEVICE_ID id

  Returns:
    uint8_t           instance
*/

uint8_t  _USB_HOST_CDC_findInstance(USB_HOST_DEVICE_ID id );


/*************************************************************************/

/* Function:
   _USB_HOST_CDC_ControlTransferComplete( USB_HOST_IRP *irp)

  Summary:
        Call back function for controltransfer status

  Description:


  Parameters:
      USB_HOST_IRP *irp

  Returns:
   void
*/
 void  _USB_HOST_CDC_ControlTransferComplete( USB_HOST_IRP *irp);

  /*************************************************************************/

/* Function:
   uint8_t _USB_HOST_CDC_BulkInTransferComplete(USB_HOST_IRP *irp)

  Summary:
        Call back function for Bulkin IRP status

  Description:


  Parameters:
        USB_HOST_IRP *irp

  Returns:
        None
*/

 void _USB_HOST_CDC_BulkInTransferComplete(USB_HOST_IRP *irp);

  /*************************************************************************/

/* Function:
   uint8_t _USB_HOST_CDC_BulkOutTransferComplete(USB_HOST_IRP *irp)

  Summary:
        Call back function for Bulkout IRP status

  Description:


  Parameters:
        USB_HOST_IRP *irp

  Returns:
        None
*/
void _USB_HOST_CDC_BulkOutTransferComplete( USB_HOST_IRP *irp);
  /*************************************************************************/

/* Function:
   uint8_t _USB_HOST_CDC_InterruptInTransferComplete(USB_HOST_IRP *irp)

  Summary:
        Call back function for Interrupt in IRP status

  Description:


  Parameters:
        USB_HOST_IRP *irp

  Returns:
        None
*/
void _USB_HOST_CDC_InterruptInTransferComplete( USB_HOST_IRP *irp);


void _USB_HOST_CDC_Task( uint8_t instanceNumber);

#endif

 /************ End of file *************************************/
