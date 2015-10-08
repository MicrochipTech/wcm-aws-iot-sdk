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

#include "usb/usb_host.h"
#include "usb/usb_host_client_driver.h"
#include "usb/usb_msd.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB HOST MSD LUN Handle

  Summary:
    USB HOST MSD LUN Handle

  Description:
    This type defines a MSD LUN Handle. This handle is used by SCSI driver to 
    identify the LUN.

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_MSD_LUN_HANDLE;

// *****************************************************************************
/* USB HOST MSD LUN Handle Invalid
 
  Summary:
    USB HOST MSD LUN Handle Invalid

  Description:
    This value defines an invalid LUN Handle.

  Remarks:
    None.
*/

#define USB_HOST_MSD_LUN_HANDLE_INVALID /*DOM-IGNORE-BEGIN*/((USB_HOST_MSD_LUN_HANDLE)(-1))/*DOM-IGNORE-END*/

// *****************************************************************************
/* USB HOST MSD Client Driver Interface
 
  Summary:
    USB HOST MSD Client Driver Interface

  Description:
    This macro should be used by the application in TPL table while adding
    support for the USB MSD Host Client Driver.

  Remarks:
    None.
*/

/*DOM-IGNORE-BEGIN*/extern USB_HOST_CLIENT_DRIVER gUSBHostMSDClientDriver; /*DOM-IGNORE-END*/
#define USB_HOST_MSD_INTERFACE  /*DOM-IGNORE-BEGIN*/&gUSBHostMSDClientDriver /*DOM-IGNORE-END*/


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

// *****************************************************************************
/* USB HOST MSD Transfer Handle Invalid
 
  Summary:
    USB HOST MSD Transfer Handle Invalid

  Description:
    This value defines an invalid Transfer Handle.

  Remarks:
    None.
*/

#define USB_HOST_MSD_TRANSFER_HANDLE_INVALID /*DOM-IGNORE-BEGIN*/((USB_HOST_MSD_TRANSFER_HANDLE)(-1))/*DOM-IGNORE-END*/

// *****************************************************************************
/* USB HOST MSD Result

  Summary:
    USB HOST MSD Result

  Description:
    This enumeration defines the possible return values of different USB HOST
    MSD Client driver function call. Refer to the specific function
    documentation for details on the return values.
    
  Remarks:
    None.

*/

typedef enum
{

    /* MSD Command result was success */
    USB_HOST_MSD_RESULT_COMMAND_PASSED = 0,

    /* MSD Command failed */
    USB_HOST_MSD_RESULT_COMMAND_FAILED = 1,

    /* MSD Command failed with phase error */
    USB_HOST_MSD_RESULT_COMMAND_PHASE_ERROR = 2,

    /* The operation was successful */
    USB_HOST_MSD_RESULT_SUCCESS,
    
    /* An unknown failure has occurred. */
    USB_HOST_MSD_RESULT_FAILURE,

    /* The request cannot be accepted at this time */
    USB_HOST_MSD_RESULT_BUSY,

    /* The specified LUN is not valid */
    USB_HOST_MSD_RESULT_LUN_HANDLE_INVALID,

    /* The MSD request was stalled */
    USB_HOST_MSD_RESULT_COMMAND_STALLED

} USB_HOST_MSD_RESULT;

// *****************************************************************************
/* USB HOST Transfer Direction

  Summary:
    USB HOST MSD Transfer Direction.

  Description:
    This enumeration specifies the direction of the data stage.
    
  Remarks:
    None.
*/

typedef enum
{
    /* Data moves from host to device */
    USB_HOST_MSD_TRANSFER_DIRECTION_HOST_TO_DEVICE = 0x00,

    /* Data moves from device to host */
    USB_HOST_MSD_TRANSFER_DIRECTION_DEVICE_TO_HOST = 0x80

} USB_HOST_MSD_TRANSFER_DIRECTION;

// *****************************************************************************
/* USB HOST MSD Transfer Complete Callback

  Summary:
    USB HOST MSD Transfer Complete Callback

  Description:
    This type defines the type of the callback function that the application
    must register in the USB_HOST_MSD_Transfer function to receive notification
    when a transfer has completed. The callback function will be called with the
    following parameters.

    lunHandle - The handle to the LUN from where this notification originated.

    transferHandle - the handle to the MSD transfer.

    result - result of the transfer.

    size - of the transfer.

    context - context that specified when this transfer was scheduled.
    
  Remarks:
    None.
*/

typedef void (*USB_HOST_MSD_TRANSFER_CALLBACK)
(
    USB_HOST_MSD_LUN_HANDLE lunHandle,
    USB_HOST_MSD_TRANSFER_HANDLE transferHandle,
    USB_HOST_MSD_RESULT result,
    size_t size,
    uintptr_t context
);

// *****************************************************************************
// *****************************************************************************
// Section: MSD Interface Function Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
   USB_HOST_MSD_RESULT USB_HOST_MSD_Transfer
   (
       uint8_t * cdb,
       uint8_t cdbLength,
       void * data,
       size_t size,
       USB_HOST_MSD_TRANSFER_CALLBACK callback,
       uintptr_t context
   )

  Summary:
    This function schedules a MSD BOT transfer.

  Description:
    This function schedules a MSD BOT transfer. The command to be executed is
    specified in the cdb. This should be pointer to a 16 byte command descriptor
    block. The actual length of the command is specified by cdbLength. If there
    is data to be transferred, the pointer to the buffer is specified by data.
    The size of the buffer is specified in size. When the transfer completes,
    the callback function will be called. The context will be returned in the
    callback function.

  Preconditions:
    None.

  Parameters:
    cdb - pointer to the command to be executted. Should be a pointer to a 16
    byte array. Unused bytes should be zero-padded.

    cdbLength - Actual size of the command.

    data - pointer to the data buffer if a data stage is involved.

    size - size of the data buffer.

    callback - callback function to called when the transfer has completed.

    transferDirection - specifies the direction of the MSD transfer.

    context - caller defined context that is returned in the callback function.

  Returns:
    USB_HOST_MSD_RESULT_FAILURE - An unknown failure occurred.
    USB_HOST_MSD_RESULT_BUSY - The transfer cannot be scheduled right now. The
    caller should retry.
    USB_HOST_MSD_RESULT_LUN_HANDLE_INVALID - This LUN does not exist in the
    system.
    USB_HOST_MSD_RESULT_SUCCESS - The transfer request was scheduled.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/


USB_HOST_MSD_RESULT USB_HOST_MSD_Transfer
(
    USB_HOST_MSD_LUN_HANDLE lunHandle,
    uint8_t * cdb,
    uint8_t cdbLength,
    void * data,
    size_t size,
    USB_HOST_MSD_TRANSFER_DIRECTION transferDirection,
    USB_HOST_MSD_TRANSFER_CALLBACK callback,
    uintptr_t context
);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif

//DOM-IGNORE-END

#endif



