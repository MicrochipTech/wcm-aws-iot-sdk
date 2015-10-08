 /*******************************************************************************
  USB HOST scsi subclass definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_scsi_local.h

  Summary:
    USB HOST SCSI subclass definitions

  Description:
    This file describes the SCSI sub class specific definitions.
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

#ifndef _USB_HOST_SCSI_LOCAL_H
#define _USB_HOST_SCSI_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "usb/usb_host_scsi.h"

/*****************************************************************************
*  SCSI Task state
*****************************************************************************/
typedef enum
{
    /* The SCSI object is not initialized */
    USB_HOST_SCSI_STATE_NOT_READY = 0,

    /* The first test unit ready */
    USB_HOST_SCSI_STATE_START_TEST_UNIT_READY,

    /* Wait for the start test unit ready to complete */
    USB_HOST_SCSI_STATE_WAIT_START_TEST_UNIT_READY,

    /* Get Inquiry response */
    USB_HOST_SCSI_STATE_INQUIRY_RESPONSE,

    /* Waiting for inquiry response */
    USB_HOST_SCSI_STATE_WAIT_INQUIRY_RESPONSE,

    /* Send read format capacity command */
    USB_HOST_SCSI_STATE_READ_FORMAT_CAPACITY,

    /* Read format capacity */
    USB_HOST_SCSI_STATE_WAIT_READ_FORMAT_CAPACITY,

    /* Send read capacity command */
    USB_HOST_SCSI_STATE_READ_CAPACITY,

    /* Wait for read capacity to complete */
    USB_HOST_SCSI_STATE_WAIT_READ_CAPACITY,
            
    /* Check if the unit is ready */
    USB_HOST_SCSI_STATE_TEST_UNIT_READY,
    
    /* Wait for Test unit ready to complete */
    USB_HOST_SCSI_STATE_WAIT_TEST_UNIT_READY,
    
    /* Unit is ready general data transfer */
    USB_HOST_SCSI_STATE_READY,

    /* The unit is in an error state */
    USB_HOST_SCSI_STATE_ERROR,

    /* Send the request sense command */
    USB_HOST_SCSI_STATE_REQUEST_SENSE,

    /* Wait for request sense to complete */
    USB_HOST_SCSI_STATE_WAIT_REQUEST_SENSE

} USB_HOST_SCSI_STATE;

/******************************************************
 * SCSI Command object
 ******************************************************/

typedef struct
{
    /* Command object is in use */
    bool inUse;

    /* The CDB buffer for this command object */
    uint8_t cdb[16];

    /* True if the command is completed */
    bool commandCompleted;

    /* The result of the command */
    USB_HOST_MSD_RESULT result;

    /* The processed size */
    size_t size;

    /* If true, event handler function will be called */
    bool generateEvent;

} USB_HOST_SCSI_COMMAND_OBJ;

/******************************************************
 * SCSI Object
 ******************************************************/
typedef struct 
{
    /* Flag to indicate in use  */
    bool inUse;

    /* Value to return to the file system at time of deinitialize */
    SYS_FS_MEDIA_HANDLE fsHandle;
    
    /* The status of the SCSI object*/
    USB_HOST_SCSI_STATE state;

    /* The status of the SCSI object*/
    USB_HOST_SCSI_STATE requestSenseNextState;
    
    /* The LUN Handle provided by MSD */
    USB_HOST_MSD_LUN_HANDLE lunHandle;

    /* The command object */
    USB_HOST_SCSI_COMMAND_OBJ commandObj;

    /* Media Geometry object */
    SYS_FS_MEDIA_GEOMETRY mediaGeometry;

    /* Media region table */
    SYS_FS_MEDIA_REGION_GEOMETRY mediaRegionGeometry[3];

    /* Event Handler */
    USB_HOST_SCSI_EVENT_HANDLER eventHandler;

    /* Application specified context */
    uintptr_t context;
    
    /* Internal buffer */
    uint8_t * buffer;

} USB_HOST_SCSI_OBJ;

/*****************************************************************************
*  SCSI commands
*****************************************************************************/
#define    USB_HOST_SCSI_FORMAT_COMMAND        0x04  // Format
#define    USB_HOST_SCSI_READ6_COMMAND         0x08   // Read
#define    USB_HOST_SCSI_READ10_COMMAND        0x28  // Read
#define    USB_HOST_SCSI_WRITE6_COMMAND        0x0A  // write
#define    USB_HOST_SCSI_WRITE10_COMMAND        0x2A  // write
#define    USB_HOST_SCSI_SEEK6_COMMAND          0x0B  // track access
#define    USB_HOST_SCSI_SEEK10_COMMAND         0x2B  // track access

/*****************************************************************************
*  SCSI Transparent Command Set Sub-class code
*****************************************************************************/
#define USB_SCSI_INQUIRY 				    0x12
#define USB_SCSI_READ_FORMAT_CAPACITY 		0x23
#define USB_SCSI_READ_CAPACITY 				0x25
#define USB_SCSI_READ_10 				    0x28
#define USB_SCSI_WRITE_10 				    0x2a
#define USB_SCSI_REQUEST_SENSE 				0x03
#define USB_SCSI_MODE_SENSE  				0x1a
#define USB_SCSI_TEST_UNIT_READY 			0x00
#define USB_SCSI_VERIFY 					0x2f
#define USB_SCSI_STOP_START 				0x1b

#define USB_SCSI_PREVENT_ALLOW_MEDIUM_REMOVAL   0x1E



/* Error */
#define USB_HOST_DRIVE_NUMBER_INVALID  -1
#define USB_HOST_DRIVE_MAP_INVALID     -2
/* command status */
#define    USB_HOST_SCSI_COMMAND_SUCCESS         0x00
#define    USB_HOST_SCSI_COMMAND_FAIL            0x02  //command fail, send request sense
#define    USB_HOST_SCSI_BUSY                    0x08  // Busy

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_TransferCallback 
    (
        USB_HOST_MSD_LUN_HANDLE lunHandle,
        USB_HOST_MSD_TRANSFER_HANDLE transferHandle,
        USB_HOST_MSD_RESULT result,
        size_t size,
        uintptr_t context
    )

  Summary:
    This function is called when a MSD transfer has completed.

  Description:
    This function is called when a MSD transfer has completed.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_TransferCallback 
(
    USB_HOST_MSD_LUN_HANDLE lunHandle,
    USB_HOST_MSD_TRANSFER_HANDLE transferHandle,
    USB_HOST_MSD_RESULT result,
    size_t size,
    uintptr_t context
);

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_InquiryResponseCommand 
    (
        USB_HOST_MSD_COMMAND * msdCommand
    )

  Summary:
    Sets up the SCSI Inquiry Response command.

  Description:
    This function sets up the SCSI Inquiry Response Command.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_InquiryResponseCommand (uint8_t * scsiCommand );

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_ReadFormatCapacityCommand
    (
        USB_HOST_MSD_COMMAND * msdCommand
    )

  Summary:
    Sets up the SCSI Read Format Capacity command.

  Description:
    This function sets up the SCSI Read Format Capacity Command.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_ReadFormatCapacityCommand (uint8_t * scsiCommand );

// ******************************************************************************
/* Function:
    int _USB_HOST_SCSI_LUNHandleToSCSIInstance 
    (
        USB_HOST_MSD_LUN_HANDLE lunHandle
    )

  Summary:
    This function maps a LUN Handle to the SCSI instance.

  Description:
    This function maps a LUN Handle to the SCSI instance. It returns the index
    of the SCSI object. It will return -1 if the object is not found.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

int _USB_HOST_SCSI_LUNHandleToSCSIInstance
(
    USB_HOST_MSD_LUN_HANDLE lunHandle
);

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_ReadCapacityCommand
    (
        USB_HOST_MSD_COMMAND * msdCommand
    )

  Summary:
    Sets up the Read Capacity Command.

  Description:
    This function sets up the Read Capacity Command.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_ReadCapacityCommand (uint8_t * scsiCommand );

// ******************************************************************************
/* Function:
    void _USB_HOST_SCSI_RequestSenseCommand 
    (
        USB_HOST_MSD_COMMAND * msdCommand
    )

  Summary:
    Sets up the Request Sense command.

  Description:
    This function sets up the Request Sense Command.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_SCSI_RequestSenseCommand (uint8_t * scsiCommand );
#endif

