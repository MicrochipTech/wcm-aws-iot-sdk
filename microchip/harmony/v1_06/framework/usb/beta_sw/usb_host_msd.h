/* ******************************************************************************
  USB Host MSD Class Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_msd.h

  Summary:
    USB Host MSD Class Driver Interface Header

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the USB Host MSD
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
#ifndef _USB_HOST_MSD_H_
#define _USB_HOST_MSD_H_


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/beta_sw/usb_host.h"
#include "usb/beta_sw/src/usb_host_local.h"
#include "usb/beta_sw/usb_msd.h"


// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* USB HOST MSD Transfer Handle

  Summary:
    USB HOST MSD Transfer Handle

  Description:
    This type defines a USB Host MSD Transfer Handle.

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_MSD_TRANSFER_HANDLE;

/* Mass storage class*/
#define USB_HOST_MSD_CLASS          0x08
/* scsi subclass */
#define USB_HOST_MSD_SUBCLASS       0x06
/* BOT Protocol */
#define USB_HOST_MSD_PROTOCOL       0x50


// *****************************************************************************
/* USB HOST MSD Function Driver instance Handle

  Summary:
    USB HOST MSD Function Driver instance Handle

  Description:
    This defines a USB Host MSD Function Driver  Handle.
    A  Handle is owned by the application, USB_HOST_MSD_OPEN() handle is valid
    till the detach event


  Remarks:
    None.
*/
typedef uintptr_t USB_HOST_MSD_HANDLE;



// *****************************************************************************
/* USB HOST MSD transfer result

  Summary:
    USB HOST MSD transfer result

  Description:
    These macros are useful to identify  USB HOST MSD transfer status.

 */
typedef enum
{
    /* MSD Command result was success */
    USB_HOST_MSD_RESULT_COMMAND_PASSED = 0,
            
     /* The operation was successful */
    USB_HOST_MSD_RESULT_SUCCESS = 3,
    /* The operation was successful */
    USB_HOST_MSD_RESULT_OK = 1 ,
    /* The transfer size is invalid. Refer to the description
     * of the read or write function for more details */
    USB_HOST_MSD_RESULT_ERROR_TRANSFER_SIZE_INVALID =
                                        USB_ERROR_IRP_SIZE_INVALID ,
    /* The transfer queue is full and no new transfers can be
     * scheduled */
    USB_HOST_MSD_RESULT_ERROR_TRANSFER_QUEUE_FULL =
                                        USB_ERROR_IRP_QUEUE_FULL ,

    /* The specified instance is not provisioned in the system */
    USB_HOST_MSD_INSTANCE_INVALID =
                                USB_ERROR_HOST_DEVICE_INSTANCE_INVALID ,

    /* The specified instance is not confifgured yet */
    USB_HOST_MSD_RESULT_ERROR_INSTANCE_NOT_CONFIGURED =
                            USB_ERROR_HOST_DRIVER_NOT_READY ,

    /* invalid event handler */
    USB_HOST_MSD_INVALID_HANDLER = - 15 ,

    USB_HOST_MSD_ERROR = -1
}USB_HOST_MSD_RESULT;

// *****************************************************************************
/* USB HOST MSD driver state

  Summary:
    Identifies the different MSD instance states.

  Description:
    
  Remarks:
    None.
 */
typedef enum
{
    USB_HOST_MSD_ATTACHED                       = 1,
    USB_HOST_MSD_DETACHED                       = 2,
    USB_HOST_MSD_WAIT                           = 3,
    USB_HOST_MSD_READY                          = 4,
    USB_HOST_MSD_ERROR_RECOVERY                 = 5,
    USB_HOST_MSD_RESET_DONE                     = 6,
    USB_HOST_CLEAR_BULKIN_ENDPOINT              = 7,
    USB_HOST_CLEAR_BULKOUT_ENDPOINT             = 8,
    USB_HOST_MSD_CLEAR_CONTROL_ENDPOINT         = 9,
    USB_HOST_MSD_CLEAR_CONTROL_ENDPOINT_DONE    = 10,
    USB_HOST_CLEAR_BULKOUT_ENDPOINT_DONE        = 11 ,
    USB_HOST_CLEAR_BULKIN_ENDPOINT_DONE         = 12

}USB_HOST_MSD_STATE;



// *****************************************************************************
/*  USB Host MSD Class Driver Index

  Summary:
     USB Host MSD Class Driver Index

  Description:
    This uniquely identifies a MSD Class Driver instance.

  Remarks:
    None.
*/
typedef uintptr_t USB_HOST_MSD_INDEX;
 
// *****************************************************************************
/* USB Host MSD command structure.

  Summary:
    command structure.

  Description:

  Remarks:
   None
*/
typedef struct __attribute__ ((packed)) _USB_HOST_MSD_COMMAND
{
    uint8_t    command[10];
    uint8_t commandLength;
    void   *data;
    uint32_t receivedDataLength;
    uint32_t length;
    uint8_t commandStatus;
    uint8_t direction ;

}USB_HOST_MSD_COMMAND;



// *****************************************************************************
/* USB HOST MSD Function Driver Events

  Summary:
    USB HOST MSD Function Driver Events

  Description:
    These events are specific to the USB HOST MSD  Function  Driver  instance.
    Each event description contains details on  the  values  of  the  parameters
    passed with event handler. The contents of pData depends on the generated
    event.
 */


typedef enum
{
    /* Attach event */
    USB_HOST_MSD_EVENT_ATTACH,
    /*Detach event*/
    USB_HOST_MSD_EVENT_DETACH,
    /* Reset the device */
    USB_HOST_MSD_EVENT_RESET,
    /* MSD Instance ready */
    USB_HOST_MSD_EVENT_READY
} USB_HOST_MSD_EVENT;

// *****************************************************************************
// *****************************************************************************
// Section: MSD Interface Function Definitions
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* USB HOST MSD Event Handler Function Pointer Type.

  Summary:
    USB HOST MSD Event Handler Function Pointer Type.

  Description:
    This defines the USB HOST MSD Function Driver event handler function
    pointer type. The application must register a function of this type to
    receive MSD Function Driver events. The function driver will invoke this
    function with event relevant parameters.

  Remarks:
    None.
*/
typedef bool (*USB_HOST_MSD_EVENT_HANDLER)(
    USB_HOST_MSD_INDEX index,
    USB_HOST_MSD_EVENT event,
    void* pData
);

/*************************************************************************/
/* Function:
   USB_HOST_MSD_HANDLE  USB_HOST_MSD_Open( uint8_t  instance )

  Summary:
   Application opens a MSD instance.

  Description:
    After deenumeration Host will Deinitialize the MSD driver

  Parameters:
  uint8_t                    instance;

  Returns:
   USB_HOST_MSD_HANDLE         msdHandle
*/
USB_HOST_MSD_HANDLE  USB_HOST_MSD_Open( uint8_t  instance );

/****************************************************************************/
/* Function:
   void  USB_HOST_MSD_Close( USB_HOST_MSD_HANDLE msdHandle )

  Summary:
    Closes handle to an instance of the USB MSD driver.

  Description:
   This function closes an opened handle to an instance of the USB host msdlayer,
   invalidating the handle.

   Precondition:
    USB_HOST_MSD_Open must have been called to obtain a valid opened device handle.

  Parameters:
    USB_HOST_MSD_HANDLE             msdHandle

  Returns:
      void
*/

void    USB_HOST_MSD_Close( USB_HOST_MSD_HANDLE msdHandle );

/*************************************************************************/
/* Function:
USB_ERROR USB_HOST_MSD_Transfer (uint8_t driveNumber, USB_HOST_MSD_COMMAND *msdCommand)

  Summary:
        Transfer the data to/from the MSD

  Description:

  Parameters:
   uint8_t               driveNumber
   USB_HOST_MSD_COMMAND      *msdCommand
  Returns:
  USB_ERROR                             status
*/

USB_ERROR USB_HOST_MSD_Transfer (uint8_t driveNumber, volatile USB_HOST_MSD_COMMAND *msdCommand);


USB_ERROR USB_HOST_MSD_EventHandlerSet ( 
                                  USB_HOST_MSD_EVENT_HANDLER appMSDHandler );

#endif


/*********** End of file ***************************************/

