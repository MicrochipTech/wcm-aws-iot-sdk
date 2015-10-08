 /*******************************************************************************
  USB HOST MSD class definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_msd_local.h

  Summary:
    USB MSD class definitions

  Description:
    This file describes the MSD class specific definitions.
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

#ifndef _USB_HOST_MSD_LOCAL_H
#define _USB_HOST_MSD_LOCAL_H




// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/beta_sw/usb_host.h"
#include "usb/beta_sw/src/usb_host_local.h"
#include "usb/beta_sw/usb_host_msd.h"
#include "usb/beta_sw/usb_msd.h"


/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW __attribute__((coherent)) __attribute__((aligned(4)));


// *****************************************************************************
/* USB HOST MSD Function Driver data structer

  Summary:
    USB HOST MSD Function Driver information

  Description:
    This structer has the details about USB HOST MSD Function Driver info
    event handler function , states of device , instance , pipes info
    contain all the information about the device driver.

  Remarks:
    None.
 */


typedef struct _USB_HOST_MSD_INSTANCE_ {
    /* use or not*/
    bool assigned;
    /* drive number */
    uint8_t driveNumber;
    /* instance number */
    uint8_t instanceNumber;
    uint8_t interfaceNumber;
    /* hcid + device id */
    USB_HOST_DEVICE_ID msdDeviceId;
    /* IRPS for each endpoint */
    USB_HOST_IRP controlIRP;
    USB_HOST_IRP bulkInIRP;
    USB_HOST_IRP bulkOutIRP;
    /*Pipes for each endpoint */
    USB_HOST_PIPE_HANDLE controlPipeHandle;
    USB_HOST_PIPE_HANDLE bulkInPipeHandle;
    USB_HOST_PIPE_HANDLE bulkOutPipeHandle;
    uint8_t     bulkInEndpoint;
    uint8_t     bulkOutEndpoint;
    /* setup packet information */
    USB_SETUP_PACKET    setupPacket;
    /*Ready for the transmit data */
    uint8_t deviceReady;
    /*connectd MSD devices*/
    uint8_t msdConnectedDevices;
    /* Logical Unit Number */
    uint8_t       logicalUnitNumber;
    /* keep track of last setup request */
    uint8_t       setupRequest;
    /* keep track of last bulk request */
    uint8_t       bulkRequest;
    uint8_t       state;
    /* Application callback */
    USB_HOST_MSD_EVENT_HANDLER appEventCallBack;
    /* For Bulk only transfer protocol */
    USB_MSD_CBW   *msdCBW;
    USB_MSD_CSW   *msdCSW;
    volatile USB_HOST_MSD_COMMAND *msdCommand;


} USB_HOST_MSD_INSTANCE;


#define USB_HOST_MSD_CBW            0x11
#define USB_HOST_MSD_DATA_IN        0x12
#define USB_HOST_MSD_DATA_OUT       0x13
#define USB_HOST_MSD_CSW            0x14
#define USB_HOST_CLEAR_ENDPOINT     0x15


#define USB_MSD_CSW_STATUS_SUCCESS  0x00
#define USB_MSD_CSW_STATUS_INVALID     -1
#define USB_MSD_CSW_TAG_INVALID        -2
#define  USB_MSD_CSW_SIGNATURE_INVALID -3

/* BOT CSW Status */
#define USB_MSD_CSW_STATUS_GOOD         0x00
#define USB_MSD_CSW_STATUS_FAIL         0x01
#define USB_MSD_CSW_STATUS_PHASE_ERROR  0x02
#define USB_MSD_TRANSFER_SUCESSS 0x00
#define USB_MSD_TRANSFER_FAIL      -1
#define USB_MSD_TRANSPORT_ERROR    -2

// *****************************************************************************
/* USB Host MSD Transfer Flags

  Summary:
    USB Host MSD Function Driver Transfer Flags

  Description:
    These flags are used to indicate status of the pending data while sending
    data to the device by using the USB_HOST_MSD_Transfer() function.

  Remarks:
    The relevance of the specified flag depends on the size of the buffer. Refer
    to the individual flag descriptions for more details.
*/
#define USB_MSD_CBW_FLAG_IN  0x80
#define USB_MSD_CBW_FLAG_OUT  0x00

/* CBW Tag value */
 #define USB_MSD_VALID_CBW_TAG     (uint32_t) 0xDD1331DD
/*************************************************************************/
/* Function:
  USB_ERROR      _USB_HOST_MSD_InterfaceInitialize( USB_HOST_DEVICE_ID id ,
                                                       uint8_t interfaceId,
                                                       uint8_t *msdInstance,
                                                       USB_SPEED speed);
  Summary:
    Initialize the MSD host driver.

  Description:
    After enumeration Host layer will initialize the MSD driver.

  Parameters:
     USB_HOST_DEVICE_ID                           id ,
     uint8_t                                interfaceId,
     uint8_t *                              cdcinstance
     USB_SPEED                              speed 

  Returns:
    USB_ERROR
*/
USB_ERROR      _USB_HOST_MSD_InterfaceInitialize( USB_HOST_DEVICE_ID id ,
                                                       uint8_t interfaceId,
                                                       uint8_t *msdInstance,
                                                       USB_SPEED speed);

/*************************************************************************/
/* Function:
   void           _USB_HOST_MSD_DeInitialize(USB_HOST_DEVICE_ID id )

  Summary:
    DeInitialize the MSD host driver.

  Description:
    After detach device Host will Deinitialize the MSD driver.


  Parameters:
     USB_HOST_DEVICE_ID                           id ,

  Returns:
        void
*/
void   _USB_HOST_MSD_DeInitialize(USB_HOST_DEVICE_ID id );

/*************************************************************************/
/* Function:
   void _USB_HOST_MSD_Tasks(uint8_t instanceNumber )

  Summary:
        State machine of the MSD instance , handling errors 

  Description:


  Parameters:
        None

  Returns:
        None
 */


void _USB_HOST_MSD_Tasks(uint8_t instanceNumber );


/*************************************************************************/

/* Function:
   USB_HOST_MSD_INSTANCE *   _USB_HOST_MSD_getDriveInfo  ( uint8_t driveNumber )

  Summary:
  Get the Driver information for this driveNumber

  Description:


  Parameters:
      uint8_t                       driveNumber

  Returns:
    USB_HOST_MSD_INSTANCE           driveNumber
*/

USB_HOST_MSD_INSTANCE *   _USB_HOST_MSD_getDriveInfo  ( uint8_t driveNumber );

/*************************************************************************/
/* Function:
   USB_HOST_MSD_RESULT  _USB_HOST_MSD_reset( USB_HOST_MSD_INSTANCE *msdDriverInfo )

  Summary:
  Reset the Mass storage driver instance

  Description:

  Parameters:
     USB_HOST_MSD_INSTANCE       *msdDriverInfo

  Returns:
    USB_HOST_MSD_RESULT
*/

USB_HOST_MSD_RESULT  _USB_HOST_MSD_reset( USB_HOST_MSD_INSTANCE *msdDriverInfo );

/*************************************************************************/
/* Function:
uint8_t _USB_HOST_MSD_validateCSW( USB_MSD_CSW csw, USB_HOST_MSD_COMMAND * msdCommand)

  Summary:
  validate the CSW status and signatures

  Description:

  Parameters:
        None
  Returns:
        uint8_t                    result of command 
 */
uint8_t _USB_HOST_MSD_validateCSW( USB_MSD_CSW *csw, volatile USB_HOST_MSD_COMMAND * msdCommand);

/*************************************************************************/
/* Function:
   uint8_t  _USB_HOST_MSD_findInstance(USB_HOST_DEVICE_ID id )

  Summary:
    Find the MSD instance

  Description:

  Parameters:
      USB_HOST_DEVICE_ID             id

  Returns:
    uint8_t                          instance Number
*/

uint8_t  _USB_HOST_MSD_findInstance(USB_HOST_DEVICE_ID id );

/*************************************************************************/
/* Function:
   void   _USB_HOST_MSD_BulkTransferComplete( USB_HOST_IRP *irp)

  Summary:
        Call back function for Bulk IRP status

  Description:

  Parameters:
        USB_HOST_IRP              *irp

  Returns:
        None
*/
void _USB_HOST_MSD_BulkTransferComplete( USB_HOST_IRP *irp);


/*************************************************************************/
/* Function:
   _USB_HOST_MSD_ControlTransferComplete( USB_HOST_IRP *irp)

  Summary:
        Call back function for controltransfer status

  Description:

  Parameters:
      USB_HOST_IRP                  *irp

  Returns:
   void
*/
void _USB_HOST_MSD_ControlTransferComplete( USB_HOST_IRP *irp);

/*************************************************************************/
/* Function:
   USB_ERROR   _USB_HOST_MSD_ClearEndpointHalt (
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
USB_ERROR   _USB_HOST_MSD_ClearEndpointHalt (
                                        uint8_t  instanceNumber ,
                                        uint8_t endpointNumber ,
                                        void* callback );

/*************************************************************************/
/* Function:
   USB_HOST_MSD_RESULT  _USB_HOST_MSD_getMaxLun( USB_HOST_MSD_INSTANCE *msdDriverInfo )

  Summary:
   This function request the LUN number of MSD device.

  Description:

  Parameters:
     USB_HOST_MSD_INSTANCE          *msdDriverInfo

  Returns:
    USB_HOST_MSD_RESULT
*/
USB_HOST_MSD_RESULT  _USB_HOST_MSD_getMaxLun( USB_HOST_MSD_INSTANCE *msdDriverInfo );
#endif


/********************** END OF FILE ***************************/
