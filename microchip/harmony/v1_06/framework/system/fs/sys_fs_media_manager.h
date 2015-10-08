/*************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    sys_fs_media_manager.h

  Summary:
    File System Media Manager Interface Declarations and types.

  Description:    
    This file contains function and type declarations required to interact
    with the MPLAB Harmony File System Media Manager Framework.                         
  *************************************************************************/

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

#ifndef _SYS_FS_MEDIA_MANAGER_H_
#define _SYS_FS_MEDIA_MANAGER_H_

#include "driver/driver_common.h"
#include "system/fs/sys_fs.h"
#include "system/common/sys_module.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
/* SYS FS Media Block Command Handle

  Summary:
    Handle identifying block commands of the media.

  Description:
    A block command handle is returned by a call to the Read, Write, or Erase
    functions. This handle allows the application to track the completion of
    the operation. The handle is returned back to the client by the "event
    handler callback" function registered with the driver.

    The handle assigned to a client request expires when the client has
    been notified of the completion of the operation (after event handler
    function that notifies the client returns) or after the buffer has been
    retired by the driver if no event handler callback was set.

  Remarks:
    None.
*/

typedef uintptr_t SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE;
// *****************************************************************************
/* SPI Flash Driver Block Event Invalid Handle

  Summary:
    This value defines the SPI Flash Driver Block Command Invalid handle.

  Description:
    This value defines the SPI Flash Driver Block Command Invalid handle. It is
    returned by read/write/erase routines when the request could not be taken.

  Remarks:
    None.
*/

#define SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID /*DOM-IGNORE-BEGIN*/((SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE)(-1))/*DOM-IGNORE-END*/

// *****************************************************************************
/* Media attach trial counter timeout value

  Summary:
    This value defines the number of trials for media attach.

  Description:
    If a media (SD card) is not attached, the media manager will try for certain
    number of times, before it goes and tries other media. This is helpful to work
    in applications, which need 2 or more media. So, other media will work, even
    though, SD card is not inserted. This constant defines the number of trials.

  Remarks:
    None.
*/
#define SYS_FS_MEDIA_ATTACH_TEST_MAX_COUNT  100

// *****************************************************************************
/* SYS FS Media Events

   Summary
    Identifies the possible events that can result from a request.

   Description
    This enumeration identifies the possible events that can result from a
    media.

   Remarks:
    One of these values is passed in the "event" parameter of the event
    handling callback function that client registered with the driver by
    setting the event handler when a block request is completed.
*/

typedef enum
{
    /* Block operation has been completed successfully. */
    SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_COMPLETE,

    /* There was an error during the block operation */
    SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_ERROR

} SYS_FS_MEDIA_BLOCK_EVENT;

// *****************************************************************************
/* SYS FS Media Events

   Summary
    Identifies the possible events that can result from a request.

   Description
    This enumeration identifies the possible events that can result from a
    media.

   Remarks:
    One of these values is passed in the "event" parameter of the event
    handling callback function that client registered with the driver by
    setting the event handler when a block request is completed.
*/
typedef enum
{
    /* Block operation has been completed successfully. */
    SYS_FS_MEDIA_COMMAND_SUCCESS,
    /* Block operation is in progress. */
    SYS_FS_MEDIA_COMMAND_INPROGRESS,
    /* There was an error during the block operation */
    SYS_FS_MEDIA_COMMAND_FAILURE

} SYS_FS_MEDIA_COMMAND_RESULT;
// *****************************************************************************
/* SYS FS Media Property Structure

  Summary:
    Contains information of property of a media.

  Description:
    This structure contains the information of property of a media device.

  Remarks:
    For a device, if multiple properties  are applicable, they can be ORed
    together and used.
*/

typedef enum
{

    /* Media supports Byte Write */
    SYS_FS_MEDIA_SUPPORTS_BYTE_WRITES = 0x01,
    
    /* Media supports only Read operation */
    SYS_FS_MEDIA_SUPPORTS_READ_ONLY = 0x02,
    
    /* Media supports OTP (One Time Programming) */
    SYS_FS_MEDIA_SUPPORTS_ONE_TIME_PROGRAMING = 0x04,

    /* Read in blocking */
    SYS_FS_MEDIA_READ_IS_BLOCKING = 0x08,

    /* Write is blocking */
    SYS_FS_MEDIA_WRITE_IS_BLOCKING = 0x10,
  
} SYS_FS_MEDIA_PROPERTY;

// *****************************************************************************
/* SYS FS Media Region Geometry Structure

  Summary:
    Contains information of a sys media region.

  Description:
    This structure contains the information of a sys media region.

  Remarks:
    A media can have multiple regions. Sum of size of all the regions is the
    total memory size of the media. Each region is further divided into blocks
    of identical size.
*/

typedef struct
{

    /* Size of a each block in Bytes */
    uint32_t blockSize;
    
    /* Number of Blocks of identical size within the Region */
    uint32_t numBlocks;
    
} SYS_FS_MEDIA_REGION_GEOMETRY;



// *****************************************************************************
/* SYS FS Media Device Geometry

  Summary:
    Contains all the geometrical information of a media device.

  Description:
    This structure contains all the geometrical information of a media device.
    the structure also has property of the media like media is one time
    programmable (OTP) or Read Only etc.

  Remarks:
    A memory device can have multiple erase block regions. Sum of all the
    regions is the total memory size of the device.
*/

typedef struct 
{
    /* Properties of a Media. For a device, if multiple properties  are
       applicable, they can be ORed */
    SYS_FS_MEDIA_PROPERTY mediaProperty;
    
    /* Number of Read Regions */
    uint32_t numReadRegions;
    
    /* Number of Write Regions */
    uint32_t numWriteRegions;
    
    /* Number of Erase Regions */
    uint32_t numEraseRegions;
    
    /* Pointer to the table containing the geometry information */
    SYS_FS_MEDIA_REGION_GEOMETRY * geometryTable;
    
} SYS_FS_MEDIA_GEOMETRY;

// *****************************************************************************
/* Status of media

  Summary:
    The state of media.

  Description:
    This enumeration states if the media is attached or not

  Remarks:
    None.
*/

typedef enum
{
    /* Media is detached */
    SYS_FS_MEDIA_DETACHED,

    /* Media is attached */
    SYS_FS_MEDIA_ATTACHED
}
SYS_FS_MEDIA_STATUS;

// *****************************************************************************
/* State of media

  Summary:
    The enumeration for state of media.

  Description:
    The media manager task picks a disk for analysis and takes it through a number
 *  of states. This enumeration mentions the state of the media.

  Remarks:
    None.
*/
typedef enum
{
    /* Media is detached */
    SYS_FS_MEDIA_STATE_DETACHED = 0,
    /* Media is registered with the media manager */
    SYS_FS_MEDIA_STATE_REGISTERED,
    /* Media is de registered with the media manager */
    SYS_FS_MEDIA_STATE_DEREGISTERED,
    /* Media driver is successfully opened by the media manager */
    SYS_FS_MEDIA_STATE_OPENED,
    /* Media is attached as detected by the media manager */
    SYS_FS_MEDIA_STATE_ATTACHED,
    /* Media manager has completed all analysis of the media */
    SYS_FS_MEDIA_STATE_ANALYZED,
    /* Media manager could not detect a MBR, hence unsupported media */
    SYS_FS_MEDIA_STATE_ANALYZED_BUT_UNSUPPORTED
}
SYS_FS_MEDIA_STATE;

// *****************************************************************************
/* Type of media

  Summary:
    The enumeration for type of media.

  Description:
    This enumeration is for the type of media registered with the media manager.
  Remarks:
    None.
*/
typedef enum
{
    /* Media is of type NVM (internal flash (non volatile) memory)*/
    SYS_FS_MEDIA_TYPE_NVM,
    /* Media is of type mass storage device */
    SYS_FS_MEDIA_TYPE_MSD,
    /* Media is of type SD card */
    SYS_FS_MEDIA_TYPE_SD_CARD,
    /* Media is of type RAM */
    SYS_FS_MEDIA_TYPE_RAM,
    /* Media is of type NVM (internal flash (non volatile) memory)*/
    SYS_FS_MEDIA_TYPE_SPIFLASH
}SYS_FS_MEDIA_TYPE;
// *****************************************************************************
/* Status of buffer

  Summary:
    The enumeration for status of buffer

  Description:
    This enumeration contains the various status of buffer.

  Remarks:
    None.
*/
typedef enum
{
    /*Done OK and ready */
    SYS_FS_MEDIA_COMMAND_COMPLETED          = 0 ,

    /*Scheduled but not started */
    SYS_FS_MEDIA_COMMAND_QUEUED             = 1,

    /*Currently being in transfer */
    SYS_FS_MEDIA_COMMAND_IN_PROGRESS        = 2,

    /*Unknown buffer */
    SYS_FS_MEDIA_COMMAND_UNKNOWN      = -1,

}
SYS_FS_MEDIA_COMMAND_STATUS;

// *****************************************************************************
/* Media Event Handler function pointer

  Summary:
    Pointer to the Media Event Handler function.

  Description
    This data type defines the required function signature for the 
    media event handling callback function. A client must register
    a pointer to an event handling function whose function signature (parameter
    and return value types) match the types specified by this function pointer
    in order to receive event calls back from the driver.
    
  Parameters:
    event           - Identifies the type of event
    
    commandHandle   - Handle returned from the media operation requests
    
    context         - Value identifying the context of the application that
                      registered the event handling function

  Returns:
    None.

  Remarks:
    None.
*/
typedef void (* SYS_FS_MEDIA_EVENT_HANDLER)
(
    SYS_FS_MEDIA_BLOCK_EVENT event,
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle,
    uintptr_t context
);
/*****************************************************************************
 * Function:
 * SYS_FS_MEDIA_COMMAND_STATUS SYS_FS_MEDIA_MANAGER_CommandStatusGet
 *   (
 *       uint16_t diskNo,
 *       SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE bufferHandle
 *   )
 *
 * Description:
 * Function to know the command status. The sector read and sector write are
 * non blocking functions. Hence, this interface is provided, where the code
 * should periodically poll for the buffer status. If status is completed,
 * the read/ write operation is considered to be complete.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   diskNo         - media number
 *   bufferHandle   - the command handle which was obtained during sector read/ write
 *
 * Returns:
 *   Command status of type SYS_FS_MEDIA_COMMAND_STATUS
*/
SYS_FS_MEDIA_COMMAND_STATUS SYS_FS_MEDIA_MANAGER_CommandStatusGet
(
    uint16_t     diskNo,
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle
);
// *****************************************************************************
/* Media Mount Data

  Summary:
    Structure to obtain the device and mount name of media

  Description:
    This structure is an input for the media manager to auto mount the media
    when the auto mount feature is enabled.

  Remarks:
    None.
*/
typedef struct
{
    /* Media Mount Name */
    const char* mountName;
    /* Media Device Name */
    const char* devName;
     /* Media Type */
    SYS_FS_MEDIA_TYPE mediaType;
     /* File system type on Media */
    SYS_FS_FILE_SYSTEM_TYPE fsType;
    
} SYS_FS_MEDIA_MOUNT_DATA;
// *****************************************************************************
/* Volume property

  Summary:
    Structure to obtain the property of volume

  Description:
    This structure is passed by sys_fs layer to know the property of a volume.
 * The function "SYS_FS_MEDIA_MANAGER_VolumePropertyGet" is used for the call.

  Remarks:
    None.
*/
typedef struct _SYS_FS_VOLUME_PROPERTY
{
    /* Volume # */
    unsigned int volNumber;
    /* File system type */
    SYS_FS_FILE_SYSTEM_TYPE fsType;
}
SYS_FS_VOLUME_PROPERTY;

typedef uintptr_t   SYS_FS_MEDIA_HANDLE;
#define  SYS_FS_MEDIA_HANDLE_INVALID DRV_HANDLE_INVALID

// *****************************************************************************
/* Media function pointers

  Summary:
    Structure of function pointers for media driver

  Description:
    This structure contains the definition for functions of media driver, which is
 * registered with the media manager. In future, if any new driver needs to be
 * registered with the media manager (say, to act as a media for file system), the
 * new driver should have implemented all these functions.

  Remarks:
    None.
*/

typedef struct
{

    /* To obtains status of media */
    bool (*mediaStatusGet)(DRV_HANDLE handle);
    /* Function to get media geometry */
    SYS_FS_MEDIA_GEOMETRY * (*mediaGeometryGet)(const DRV_HANDLE handle);
    /* Function for sector read */
    void (*sectorRead)(DRV_HANDLE clientHandle,SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE * commandHandle,
                                            void * buffer, uint32_t blockStart, uint32_t nBlock);
    /* Function for sector write */
    void (*sectorWrite)(const DRV_HANDLE handle,SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE * commandHandle,
                                         void * sourceBuffer, uint32_t blockStart,uint32_t nBlock);
    /* Function register the event handler with media */
    void (*eventHandlerset)(DRV_HANDLE handle, const void * eventHandler, const uintptr_t context);
    /* Function to obtain the command status */
    SYS_FS_MEDIA_COMMAND_STATUS (*commandStatusGet)(DRV_HANDLE handle,
                                SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle);    
    /* Function to read certain bytes from the media */
    void (*Read) (DRV_HANDLE clientHandle,SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE * commandHandle,
                                         void * buffer, uint32_t blockStart, uint32_t nBlock);
    /* Function to obtain the address of the media (to be used for NVM only) */
    uintptr_t (*addressGet) ( const DRV_HANDLE hClient);
    void (*erase) ( const DRV_HANDLE handle,SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE * commandHandle,
                                                         uint32_t blockStart,uint32_t nBlock);
    /* Function to open the media driver */
    DRV_HANDLE (*open)(SYS_MODULE_INDEX index, DRV_IO_INTENT intent);
    /* Function to close the media */
    void (*close)(DRV_HANDLE client);
    /* Task function of the media */
    void (*tasks)(SYS_MODULE_OBJ obj);
}
SYS_FS_MEDIA_FUNCTIONS;

/*****************************************************************************
  Function:
    SYS_FS_MEDIA_HANDLE SYS_FS_MEDIA_MANAGER_Register
    (
            SYS_MODULE_OBJ	obj,
            SYS_MODULE_INDEX index,
            const SYS_FS_MEDIA_FUNCTIONS * mediaFunctions,
            SYS_FS_MEDIA_TYPE mediaType
    )

  Description:
    Function called by a media to register itself to the media manager. For static
 * media, (like NVM or SD card), this "register function" is called during static
 * initialization. For dynamic media (like MSD), this register function is called
 * dynamically, once the MSD media is connected.

  Precondition:
    None

  Parameters:
    obj             - driver object (of type SYS_MODULE_OBJ,
                     value returned when driver is initialized)
    index           - driver index (of type SYS_MODULE_INDEX,
                     value passed during driver initialization and opening)
    mediaFunctions  - List of driver functions
    mediaType       - Type of media
 *
  Returns:
    handle          - Of type SYS_FS_MEDIA_HANDLE
*/
SYS_FS_MEDIA_HANDLE SYS_FS_MEDIA_MANAGER_Register
(
	SYS_MODULE_OBJ	obj,    // Used to run the "Tasks" routine
    SYS_MODULE_INDEX index, // Used to open the driver
	const SYS_FS_MEDIA_FUNCTIONS * mediaFunctions,
	SYS_FS_MEDIA_TYPE mediaType // To know if NVM, SD card or MSD etc..
);

/*****************************************************************************
  Function:
    void SYS_FS_MEDIA_MANAGER_DeRegister
    (
            SYS_FS_MEDIA_HANDLE	handle
    )

  Description:
    Function called by a media to deregister itself to the media manager. For static
 * media, (like NVM or SD card), this "deregister function" is never called, since
 * static media never gets deregistered once they are initialized.
 * For dynamic media (like MSD), this register function is called
 * dynamically, once the MSD media is connected.

  Precondition:
    None

  Parameters:
    handle          - handle of type SYS_FS_MEDIA_HANDLE received when the media
                      was registered
*
  Returns:
    None
*/
void SYS_FS_MEDIA_MANAGER_DeRegister
(
	SYS_FS_MEDIA_HANDLE	handle
);

/*****************************************************************************
 * Function:
 * SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE SYS_FS_MEDIA_MANAGER_SectorRead
 *   (
 *       uint16_t diskNo,
 *       uint8_t * dataBuffer,
 *       uint32_t sector,
 *       uint32_t noSectors
 *   )
 *
 * Description:
 * Function to read a sector of specified media (disk). This is the function in
 * media manager layer. This function in turn call the specific sector read function from
 * the list of function pointer of media driver.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   diskNo         - media number
 *   dataBuffer     - pointer to buffer where data to be placed after read
 *   sector         - Sector # which is to be read
 *   noSectors      - Number of sectors to read
 *
 * Returns:
 *   Buffer handle of type SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE
*/
SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE SYS_FS_MEDIA_MANAGER_SectorRead
(
    uint16_t diskNo,        /* SYS_FS_MEDIA disk no */
    uint8_t * dataBuffer,   /* Application buffer */
    uint32_t sector,        /* Start sector */
    uint32_t noSectors      /* Number of sectors to read */
);

/*****************************************************************************
 * Function:
 * SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE SYS_FS_MEDIA_MANAGER_SectorWrite
 *   (
 *       uint16_t diskNo,
 *       uint32_t sector,
 *       uint8_t * dataBuffer,
 *       uint32_t noSectors
 *   )
 *
 * Description:
 * Function to write to a sector of specified media (disk). This is the function in
 * media manager layer. This function in turn call the specific sector write function from
 * the list of function pointer of media driver.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   diskNo         - media number
 *   sector         - Sector # to which data to be written
 *   dataBuffer     - pointer to buffer which holds the data to be written
 *   noSectors      - Number of sectors to be written
 *
 * Returns:
 *   Buffer handle of type SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE
*/
SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE SYS_FS_MEDIA_MANAGER_SectorWrite
(
    uint16_t diskNo,        /* SYS_FS_MEDIA disk no */
    uint32_t sector,        /* Start sector */
    uint8_t * dataBuffer,   /* Application buffer */
    uint32_t noSectors      /* Number of sectors to write */
);
/*****************************************************************************
 * Function:
 * void SYS_FS_MEDIA_MANAGER_EventHandler
 *   (
 *       SYS_FS_MEDIA_BLOCK_EVENT event,
 *       SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle,
 *       uintptr_t context
 *   )
 *
 * Description:
 * Function to register the event handler with media drivers. Event handler will
 * be called by media drivers whenever a media command has executed successfully
 * or if it resulted in error.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   event          -  Type SYS_FS_MEDIA_BLOCK_EVENT of event
 *   commandHandle  - the buffer handle which was obtained during sector read/write
 *   context        - Context in which event handler was invoked
 * Returns:
 *   Buffer status of type SYS_FS_MEDIA_BUFFER_STATUS
*/
void  SYS_FS_MEDIA_MANAGER_EventHandler
(
        SYS_FS_MEDIA_BLOCK_EVENT event,
        SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle,
        uintptr_t context
);

/*****************************************************************************
 * Function:
 * bool SYS_FS_MEDIA_MANAGER_VolumePropertyGet
 *   (
 *       const char *devName
 *       SYS_FS_VOLUME_PROPERTY *str
 *   )
 *
 * Description:
 * Function to know the property of the volume. This function is used by higher layers
 * (sys_fs layer) to know the property of the volume as specified in the
 * SYS_FS_VOLUME_PROPERTY structure.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   devName        - string name of the media
 *   *str           - pointer to structure of type SYS_FS_VOLUME_PROPERTY
 *
 * Returns:
 *   true or false
*/

bool SYS_FS_MEDIA_MANAGER_VolumePropertyGet
(
    const char *devName, SYS_FS_VOLUME_PROPERTY *str
);

/*****************************************************************************
 * Function:
 * bool SYS_FS_MEDIA_MANAGER_MediaStatusGet
 *   (
 *       const char *devName
 *   )
 *
 * Description:
 * Function to know the media status. This function is used by higher layers
 * (sys_fs layer) to know the status of the media, whether, the media is attached
 * or detached.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   devName        - string name of the media
 *
 * Returns:
 *   Media attach/detach status of type bool
*/

bool SYS_FS_MEDIA_MANAGER_MediaStatusGet
(
        const char *devName
);

/*****************************************************************************
 * Function:
 * SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE SYS_FS_MEDIA_MANAGER_Read
 * (
 *  uint16_t diskNo,
 *  uint8_t* destination,
 *  uint8_t * source,
 *  const unsigned int nBytes
 * )
 *
 * Description:
 * Function to data from a specific address of media. This function is intended to work with
 * NVM media only, which can have byte level addressing. For other media (like SD card),byte
 * addressing is not possible and hence this function will not work. Also, this function is
 * intended to work with MPFS2 file system only, which uses byte addressing.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   diskNo         - media number
 *   destination    - pointer to buffer where data to be placed after read
 *   source         - pointer from where data to be read
 *   nBytes         - Number of bytes to be read
 *
 * Returns:
 *   Buffer handle of type SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE
*/
SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE SYS_FS_MEDIA_MANAGER_Read
( uint16_t diskNo, uint8_t* destination, uint8_t * source, const unsigned int nBytes );

/*****************************************************************************
 * Function:
 * uintptr_t SYS_FS_MEDIA_MANAGER_AddressGet
 * (
 *  uint16_t diskNo,
 * )
 *
 * Description:
 * Function to know the NVM memory address against a disk number. This function
 * is intended to work only with MPFS2, which does byte addressing and hence needs
 * memory address (not disk number).
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   diskNo         - media number
 *
 * Returns:
 *   memory address of type uintptr_t
*/
uintptr_t SYS_FS_MEDIA_MANAGER_AddressGet
(
	uint16_t	diskNo
);

/*****************************************************************************
 * Function:
 * void SYS_FS_MEDIA_MANAGER_Tasks(void)
 *
 * Description:
 *   Task function of media manager. This task must be called repeatedly from the
 * main loop.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   None.
 *
 * Returns:
 *   None.
*/

void SYS_FS_MEDIA_MANAGER_Tasks ( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif
