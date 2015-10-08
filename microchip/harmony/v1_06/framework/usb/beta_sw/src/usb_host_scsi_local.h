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
#include "usb/beta_sw/usb_host_scsi.h"


uint8_t gUSBSCSIBuffer[512] __attribute__((coherent)) __attribute__((aligned(4)));

typedef SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE USB_HOST_SCSI_COMMAND_HANDLE;

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

typedef struct _USB_HOST_SCSI_OBJ_
{
    USB_HOST_SCSI_HANDLE  scsiHandle;
    SYS_FS_MEDIA_HANDLE   fsHandle;
    uint8_t               physicalDriveNumber;
   /* Flag to indicate in use  */
    bool                            inUse;
     /* Different states in sending a command */
    USB_HOST_SCSI_CMD_STATES           cmdState;
    /* The status of the scsi obj*/
    USB_HOST_SCSI_MEDIA_STATUS     status;
    uint8_t             state;
   // uint8_t             buffer[512];
    /* Media Geometry object */
    SYS_FS_MEDIA_GEOMETRY mediaGeometry;
    
    /* Media region table */
    SYS_FS_MEDIA_REGION_GEOMETRY mediaRegionGeometry[3];
    
     /* The command object */
    USB_HOST_SCSI_COMMAND_OBJ commandObj;
    
     /* Event Handler */
    USB_HOST_SCSI_EVENT_HANDLER eventHandler;
    
    /* Application specified context */
    uintptr_t context;

}USB_HOST_SCSI_OBJ;



/*****************************************************************************
*  SCSI Task state
*****************************************************************************/
typedef enum
{
    USB_HOST_SCSI_WAIT = 1,
    USB_HOST_SCSI_TEST_UNIT_READY = 2,
    USB_HOST_SCSI_TEST_UNIT_READY_DONE = 3 ,
    USB_HOST_SCSI_READY = 4,
    USB_HOST_SCSI_READ_CAPACITY = 5 ,
    USB_HOST_SCSI_READ_CAPACITY_DONE = 6 ,
    USB_HOST_SCSI_REQUEST_SENSE = 7,
    USB_HOST_SCSI_REQUEST_SENSE_DONE = 8,
    USB_HOST_SCSI_INQUIRY = 9 ,
    USB_HOST_SCSI_INQUIRY_DONE = 10 ,
    USB_HOST_SCSI_FS_REGISTER = 11 ,
    USB_HOST_SCSI_READ_FORMAT_CAPACITY = 12 ,
    USB_HOST_SCSI_READ_FORMAT_CAPACITY_DONE = 13
}USB_HOST_SCSI_STATE;

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
#define USB_MSD_INQUIRY 				0x12
#define USB_MSD_READ_FORMAT_CAPACITY 			0x23
#define USB_MSD_READ_CAPACITY 				0x25
#define USB_MSD_READ_10 				0x28
#define USB_MSD_WRITE_10 				0x2a
#define USB_MSD_REQUEST_SENSE 				0x03
#define USB_MSD_MODE_SENSE  				0x1a
#define USB_MSD_PREVENT_ALLOW_MEDIUM_REMOVAL            0x1e
#define USB_MSD_TEST_UNIT_READY 			0x00
#define USB_MSD_VERIFY 					0x2f
#define USB_MSD_STOP_START 				0x1b


#define USB_HOST_MSD_MAX_PHYSICAL_DRIVE    1
#define USB_HOST_SCSI_MAX_INSTANCES        2
/* Error */
#define USB_HOST_DRIVE_NUMBER_INVALID  -1
#define USB_HOST_DRIVE_MAP_INVALID     -2
/* command status */
#define    USB_HOST_SCSI_COMMAND_SUCCESS         0x00
#define    USB_HOST_SCSI_COMMAND_FAIL            0x02  //command fail, send request sense
#define    USB_HOST_SCSI_BUSY                    0x08  // Busy

//******************************************************************************
/* Function:
USB_HOST_SCSI_BUFFER_HANDLE  _USB_HOST_SCSI_ReadAndWite ( uint8_t physicalDriveNumber ,
                                                          uint32_t sectorNumber,
                                                          uint32_t noOfSectors,
                                                          uint8_t direction,
                                                          uint8_t *buffer )

  Summary:
   This function convert the data into MSD command format

  Description:


  Parameters:
        physicalDriveNumber - drver number

	sector_addr - The address of the sector on the MSD.

	buffer -  The buffer with the data to write.

  Returns:
    If successful, returns a valid handle to a buffer status handle.
    Otherwise, it returns invalid buffer handle .
*/
 USB_HOST_SCSI_BUFFER_HANDLE  _USB_HOST_SCSI_ReadAndWite ( uint8_t physicalDriveNumber ,
                                                          uint32_t sectorNumber,
                                                          uint32_t noOfSectors,
                                                          uint8_t direction,
                                                          uint8_t *buffer );


//******************************************************************************
/* Function:
USB_HOST_SCSI_BUFFER_HANDLE _USB_HOST_SCSI_processCommand(uint8_t driveNumber)

  Summary:
   This function process the command to MSD to tranfer

  Description:

  Parameters:
        physicalDriveNumber - drver number

  Returns:
    Returns the Buffer status handle .
*/

 USB_HOST_SCSI_BUFFER_HANDLE _USB_HOST_SCSI_processCommand(uint8_t driveNumber);
 //******************************************************************************
/* Function:
 USB_ERROR  _USB_HOST_SCSI_identifyCommand ( USB_HOST_MSD_DEVICE_INFO *deviceInfo,
                                            uint8_t physicalDriveNumber )

  Summary:
   This function is to identify the deviceinfo based on drivenumber
   inquiry device Informatio

  Description:


  Parameters:
        physicalDriveNumber - drver number

  Returns:
    Returns the Device information about size ( bootsector information ) .
*/
/*******************************************************************************/
USB_ERROR  _USB_HOST_SCSI_identifyCommand ( uint8_t index , uint8_t physicalDriveNumber );

 //******************************************************************************
/* Function:
 USB_ERROR  _USB_HOST_SCSI_testUintReadyCommand (  uint8_t physicalDriveNumber )

  Summary:
  Test unit ready command to identify the device status.

  Description:


  Parameters:
        physicalDriveNumber - drver number

  Returns:
    Returns the Device status information.
*/
/*******************************************************************************/
 USB_ERROR  _USB_HOST_SCSI_testUintReadyCommand ( uint8_t index, uint8_t physicalDriveNumber );
  //******************************************************************************
/* Function:
 USB_ERROR  _USB_HOST_SCSI_requestSense ( uint8_t index, uint8_t physicalDriveNumber

  Summary:
 SCSI request sense command

  Description:


  Parameters:
        physicalDriveNumber - drver number

  Returns:
    Returns the Device status information.
*/
/*******************************************************************************/
USB_ERROR  _USB_HOST_SCSI_requestSense ( uint8_t index, uint8_t physicalDriveNumber );

/* Function:
 USB_ERROR  _USB_HOST_SCSI_readCapacity (uint8_t index, uint8_t physicalDriveNumber )

  Summary:
 SCSI Read capacity command

  Description:


  Parameters:
        physicalDriveNumber - drver number

  Returns:
    Returns the Device Capacity information.
*/
/*******************************************************************************/
USB_ERROR  _USB_HOST_SCSI_readCapacity (uint8_t index, uint8_t physicalDriveNumber );

/* Function:
 USB_ERROR  _USB_HOST_SCSI_readFormatCapacity (uint8_t index, uint8_t physicalDriveNumber )

  Summary:
 SCSI Read capacity command

  Description:


  Parameters:
        physicalDriveNumber - drver number

  Returns:
    Returns the Device  Format Capacity information.
*/
/*******************************************************************************/

 USB_ERROR  _USB_HOST_SCSI_readFormatCapacity (uint8_t index, uint8_t physicalDriveNumber );

#endif

/************* End of File ***************************************/
