/*******************************************************************************
 USB CDC ACM HOST Class Driver

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_cdc_acm.c

  Summary:
    USB CDC Host class driver.

  Description:
    USB CDC Host class driver.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

 Microchip licenses to you the right to use, modify, copy and distribute
 Software only when embedded on a Microchip microcontroller or digital signal
 controller that is integrated into your product or third party product
 (pursuant to the sublicense terms in the accompanying license agreement).

 You should refer to the license agreement accompanying this Software for
 additional information regarding your rights and obligations.

 SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************


#include "usb/beta_sw/usb_host_cdc.h"
#include "usb/beta_sw/usb_host.h"

/*************************************************************************/

/* Function:
 uint8_t USB_HOST_CDC_ACM_GetData(   USB_HOST_CDC_HANDLE cdcHandle ,
                                    USB_HOST_CDC_TRANSFER_HANDLE  *rxHandle,
                                    uint8_t size, uint8_t  *data)

  Summary:
        Application will get data from the device

  Description:


  Parameters:
   USB_HOST_CDC_HANDLE                  cdcHandle ,
   USB_HOST_CDC_TRANSFER_HANDLE         *rxHandle
   uint8_t                              size,
   uint8_t                              *data
  Returns:
     uint8_t
*/
uint8_t USB_HOST_CDC_ACM_GetData(   USB_HOST_CDC_HANDLE cdcHandle ,
                                    USB_HOST_CDC_TRANSFER_HANDLE  *rxHandle,
                                    uint8_t size, uint8_t  *data)
{
    USB_HOST_CDC_DRIVER_INFO *cdcDriverInfo;
    USB_HOST_CDC_TRANSFER_FLAGS flags;
    USB_HOST_IRP *irp;
    /* Get CDC driver info*/
    cdcDriverInfo = (USB_HOST_CDC_DRIVER_INFO *) cdcHandle;
    flags = 0;
    irp = &(cdcDriverInfo->bulkinIRP);
    rxHandle = (USB_HOST_CDC_TRANSFER_HANDLE  *)&(cdcDriverInfo->bulkinIRP);
    cdcDriverInfo->rxHandle = ( USB_HOST_CDC_TRANSFER_HANDLE) rxHandle;
    irp->data = data;
    irp->size = size;
    irp->callback = (void * )_USB_HOST_CDC_BulkInTransferComplete;
    irp->userData = (uintptr_t)cdcDriverInfo;
    /* Submit IRP */
    USB_HOST_IRPSubmit(cdcDriverInfo->bulkinpipeHandle, irp);
    return USB_HOST_CDC_RESULT_OK;
}



/*************************************************************************/

/* Function:
   uint8_t USB_HOST_CDC_ACM_SendData( 
                            USB_HOST_CDC_HANDLE cdcHandle ,
                            USB_HOST_CDC_TRANSFER_HANDLE  *txHandle ,
                            uint8_t size, uint8_t  *data)

  Summary:
    Application will send data to the device 

  Description:


  Parameters:
      USB_HOST_CDC_HANDLE               cdcHandle
      USB_HOST_CDC_TRANSFER_HANDLE     *txHandle
      uint8_t                           size
      uint8_t                           *data
  Returns:
        uint8_t
*/
uint8_t USB_HOST_CDC_ACM_SendData(
                            USB_HOST_CDC_HANDLE cdcHandle ,
                            USB_HOST_CDC_TRANSFER_HANDLE  *txHandle ,
                            uint8_t size, uint8_t  *data)
{
    USB_HOST_CDC_DRIVER_INFO *cdcDriverInfo;
    USB_HOST_CDC_TRANSFER_FLAGS flags;
 
    USB_HOST_IRP *irp;
       flags = 0;
    cdcDriverInfo = (USB_HOST_CDC_DRIVER_INFO *) cdcHandle;
    irp = &(cdcDriverInfo->bulkoutIRP);
    txHandle = (USB_HOST_CDC_TRANSFER_HANDLE  *) &(cdcDriverInfo->bulkoutIRP);
    cdcDriverInfo->txHandle = (USB_HOST_CDC_TRANSFER_HANDLE )txHandle;
    irp->data = data;
    irp->size = size;
    irp->callback = _USB_HOST_CDC_BulkOutTransferComplete;
    irp->userData = (uintptr_t)cdcDriverInfo;
    USB_HOST_IRPSubmit(cdcDriverInfo->bulkoutpipeHandle, irp);
    return 1;
}


/*************************************************************************/
/* Function:
    uint8_t USB_HOST_CDC_ACM_CommandRequest(    USB_HOST_CDC_HANDLE cdcHandle,
                                            uint8_t requestType,
                                            uint8_t size, uint8_t* data)

  Summary:
        Application can send ACM command request 

  Description:


  Parameters:
        USB_HOST_CDC_HANDLE                 cdcHandle
        uint8_t                             requestType
        uint8_t                             size
        uint8_t*                            data
  Returns:
       uint8_t                             status
*/


uint8_t USB_HOST_CDC_ACM_CommandRequest(    USB_HOST_CDC_HANDLE cdcHandle,
                                            uint8_t requestType,
                                            uint8_t size, uint8_t* data)
{
    //uint8_t i;
    uint8_t returnVal = 0x01; //USB_CDC_COMMAND_FAILED;
    USB_HOST_CDC_DRIVER_INFO *cdcDriverInfo;
    USB_HOST_CDC_TRANSFER_FLAGS flags;
    flags = USB_HOST_CDC_TRANSFER_FLAGS_MORE_DATA_PENDING;
    cdcDriverInfo = (USB_HOST_CDC_DRIVER_INFO *) cdcHandle;
    // Find the correct device.
    if (cdcHandle == (USB_HOST_CDC_HANDLE) NULL) {
        return USB_CDC_DEVICE_NOT_FOUND;
    }
    if (requestType == USB_CDC_SEND_ENCAPSULATED_COMMAND)
    {
        returnVal = USB_HOST_CDC_Transfer(
                                cdcHandle,
                                USB_CDC_SEND_ENCAPSULATED_COMMAND,
                                0,
                                cdcDriverInfo->communicationInterfaceNumber,
                                size, data, flags);
    }
    else if (requestType == USB_CDC_GET_ENCAPSULATED_REQUEST)
    {
        returnVal = USB_HOST_CDC_Transfer(
                                    cdcHandle,
                                    USB_CDC_GET_ENCAPSULATED_REQUEST, 1,
                                    cdcDriverInfo->communicationInterfaceNumber,
                                    size, data, flags);
    }
    else if (requestType == USB_CDC_SET_COMM_FEATURE)
    {

    }
    else if (requestType == USB_CDC_GET_COMM_FEATURE)
    {

    } 
    else if (requestType == USB_CDC_SET_LINE_CODING)
    {
        returnVal = USB_HOST_CDC_Transfer(
                                    cdcHandle,
                                    USB_CDC_SET_LINE_CODING, 0,
                                    cdcDriverInfo->communicationInterfaceNumber,
                                    size, data, flags);
    }
    else if (requestType == USB_CDC_GET_LINE_CODING)
    {
        returnVal = USB_HOST_CDC_Transfer(
                                    cdcHandle,
                                    USB_CDC_GET_LINE_CODING,
                                    1,
                                    cdcDriverInfo->communicationInterfaceNumber,
                                    size, data, flags);
    } 
    else if (requestType == USB_CDC_SET_CONTROL_LINE_STATE)
    {
        returnVal = USB_HOST_CDC_Transfer(
                                    cdcHandle,
                                    USB_CDC_SET_CONTROL_LINE_STATE, 0,
                                    cdcDriverInfo->communicationInterfaceNumber,
                                    size, data, flags);
    }
    else if (requestType == USB_CDC_SEND_BREAK)
    {

    } 
    else
    {
        return USB_CDC_ILLEGAL_REQUEST;
    }
    return returnVal;
}





