/*******************************************************************************
  File System System-Library Interface Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    sys_fs_media_manager.c

  Summary:
    This file contains implementation of SYS FS Media Manager functions. 

  Description:
    This file contains implementation of SYS FS Media Manager functions. 
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is fsegrated fso your product or third party product
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

#include "system/fs/src/sys_fs_media_manager_local.h"
#include "system/fs/src/sys_fs_local.h"

// *****************************************************************************
/* Media object

  Summary:
    Defines the media object for each media that should be controlled by media manager

  Description:
    This data type defines the media objects that are available on
    the part.
  Remarks:
    None
*/
SYS_FS_MEDIA gSYSFSMediaObject[SYS_FS_MEDIA_NUMBER];

// *****************************************************************************
/* Volume object

  Summary:
    Defines the volume object for each media that should be controlled by media manager

  Description:
     Each partition of the media is assigned as a volume using the volume object.

  Remarks:
    None
*/
SYS_FS_VOLUME gSYSFSVolumeObject[SYS_FS_VOLUME_NUMBER];
// *****************************************************************************
/* Media Event Handler

  Summary:
    Defines the media Event handler that stores the application event handler.

  Description:
    This data type defines the media event handler
    the part.
  Remarks:
    None
*/
extern SYS_FS_EVENT_HANDLER gSYSFSEventHandler;
// *****************************************************************************
/* Media Page Buffer

  Summary:
    Defines the media page buffer to store the data on media.

  Description:
    This data type defines the media page buffer.
  Remarks:
    None
*/
uint8_t __attribute__((coherent)) gSYSFSPagebuffer[SYS_FS_MEDIA_MANAGER_BUFFER_SIZE] = {0};
// *****************************************************************************
/* Media Mount Table

  Summary:
    Defines the mount table specified by application.

  Description:
    This data type defines the mount table that maps device and mount name.
  Remarks:
    None
*/
extern const SYS_FS_MEDIA_MOUNT_DATA sysfsMountTable[];
// *****************************************************************************
/* Media Command Status

  Summary:
    Defines the media command status

  Description:
    This data type defines the media command status that is updated in media 
    event handler.
  Remarks:
    None
*/
volatile SYS_FS_MEDIA_COMMAND_RESULT gSYSFSCommandStatus = SYS_FS_MEDIA_COMMAND_FAILURE;
// *****************************************************************************
/* Volume to Partition translation

  Summary:
    Defines the volume to partition translation table, used by FAT FS.

  Description:
 * The following array was added to enable the "multipartition"
 * feature of FAT FS. This array is already declared in ff.h and
 * the intention was to make as little change on ff.h. Also, type of this
 * array is non-standard since, it is defined in ff.c. The implementation
 * is kept unchanged to maintain compatibility with future releases of FAT FS

  Remarks:
    None
*/
PARTITION VolToPart[SYS_FS_VOLUME_NUMBER];

/* Following structure holds the variables for media manager, including the task states */
SYS_FS_MEDIA_MANAGER_TASK_OPERATION gCurMedia  __attribute__((coherent , aligned(16))) =
{
    0,
    (SYS_FS_MEDIA *)0,
    SYS_FS_MM_STATE_NEW_DISK,
    {0}
};
/* Read entry in geometry table */
#define SYS_FS_MEDIA_GEOMETRY_READ          (0)
/* Write entry in geometry table */
#define SYS_FS_MEDIA_GEOMETRY_WRITE         (1)
/* Erase entry in geometry table */
#define SYS_FS_MEDIA_GEOMETRY_ERASE         (2)
/* Shift Value for multiply or divide by a sector of size 512 bytes*/
#define SYS_FS_MEDIA_SHIFT_SECTOR_VALUE     (9)
/* Partition one offset */
#define PARTITION_ONE_OFFSET                (450)
/* Partition two offset */
#define PARTITION_TWO_OFFSET                (466)
/* Partition three offset */
#define PARTITION_THREE_OFFSET              (482)
/* Partition four offset */
#define PARTITION_FOUR_OFFSET               (498)

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
                     value passed during driver initilization and opening)
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
)
{
    int diskIndex;
    SYS_FS_MEDIA *disk;
    uint8_t findmediaId = 'a';

    for(diskIndex = 0; diskIndex < SYS_FS_MEDIA_NUMBER; diskIndex++)
    {
         if((gSYSFSMediaObject[diskIndex].inUse)
                 && (mediaType == gSYSFSMediaObject[diskIndex].mediaType))
         {
             /* For every type of media, increment the mediaID as a, b, c... */
             findmediaId++;
         }
    }
    // Start with 0th disk and find a disk which is available (not in use)
    for(diskIndex = 0; diskIndex < SYS_FS_MEDIA_NUMBER; diskIndex++)
    {
        if(!gSYSFSMediaObject[diskIndex].inUse) // not in use, hence this is available
        {
            disk = &gSYSFSMediaObject[diskIndex];
            disk->inUse = true;
            disk->mediaDriverFunctions = mediaFunctions;
            disk->mediaDriverObject = obj;
            disk->index = index;
            disk->mediaDriverClientHandle = DRV_HANDLE_INVALID;  // Initially, INVALID
            disk->mediaState = SYS_FS_MEDIA_STATE_REGISTERED;
            disk->mediaType = mediaType;
            disk->numberOfPartitions = 0;
            disk->volumeCounter = 0;
            disk->mediaCounter = diskIndex;
            disk->mediaId = findmediaId;
            return (SYS_FS_MEDIA_HANDLE)disk;
        }
    }

    return SYS_FS_MEDIA_HANDLE_INVALID;
}

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
)
{
    SYS_FS_MEDIA *disk = (SYS_FS_MEDIA *)handle;

    if(handle == SYS_FS_MEDIA_HANDLE_INVALID)
        return;

    disk->numberOfPartitions = 0;
    disk->mediaState = SYS_FS_MEDIA_STATE_DEREGISTERED;
    disk->mediaId = 0;
    disk->mediaCounter = 0;
    disk->volumeCounter = 0;
 }

/*****************************************************************************
 * Function:
 * void SYS_FS_MEDIA_MANAGER_Tasks(void)
 *
 * Description:
 *   Task function of media manager. This task must be called repeteadly from the
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

const uint16_t partitionTypeOffset [4] =
{
    450,
    466,
    482,
    498
};

void SYS_FS_MEDIA_MANAGER_Tasks ( void )
{
    uint8_t volIndex = 0;
    uint8_t index = 0;
    SYS_FS_VOLUME *vol = NULL;
    uint8_t fileSystemType = 0xFF;
    uint8_t partitionMap = 0;
    uint8_t devNameSize = 0;
    uint16_t locShiftOp = 0;
    uint8_t  bootRecord = 0;
    uint8_t partitionNum = 0;
            
    switch(gCurMedia.taskState)
    {
        case SYS_FS_MM_STATE_NEW_DISK:
            
            /* Reset the disk index if we have analyzed all Media */
            if(gCurMedia.diskIndex >= SYS_FS_MEDIA_NUMBER)
            {
                gCurMedia.diskIndex = 0;
            }            
            /* Get the Media Object from Index */
            gCurMedia.disk = &gSYSFSMediaObject[gCurMedia.diskIndex];
            gCurMedia.diskIndex++;
            /* Take a disk which is already in use */
            if(gCurMedia.disk->inUse)
            {
                switch (gCurMedia.disk->mediaState)
                {

                    case SYS_FS_MEDIA_STATE_REGISTERED:
                         /* If the disk is one which is just "registered" newly */
                        gCurMedia.taskState = SYS_FS_MM_STATE_OPEN_MEDIA;
                        break;
                    /* If the media has been  de registered  */
                    case SYS_FS_MEDIA_STATE_DEREGISTERED:
                        gCurMedia.disk->inUse = false;
                    /* If the media has been analyzed, check for the attach/detach Status */
                    case SYS_FS_MEDIA_STATE_ANALYZED:
                    case SYS_FS_MEDIA_STATE_ANALYZED_BUT_UNSUPPORTED:
                         /* check if Media is still attached */
                        if(true != gCurMedia.disk->mediaDriverFunctions->mediaStatusGet(
                                                    gCurMedia.disk->mediaDriverClientHandle))
                                                                                                                        
                        {                     
                            /* Do not call close. For SD card, closing SD card driver 
                             * will prevent getting media status.
                             * If the media is detached, clear all paramaters in the "volume" object */
                            for(volIndex = 0; volIndex < SYS_FS_VOLUME_NUMBER; volIndex++)
                            {
                                vol = &gSYSFSVolumeObject[volIndex];
                                if((vol->inUse == true) && (vol->obj == gCurMedia.disk))
                                {
                                    
                                    vol->inUse = false;
                                    /* Unmount the media if auto mount feature is enable */
                                    if(SYS_FS_AUTOMOUNT_ENABLE)
                                    {
                                        for(index = 0; index < SYS_FS_VOLUME_NUMBER; index++)
                                        {
                                            /* Find out the mount name from the media mount table */
                                            if(gCurMedia.disk->mediaType == sysfsMountTable[index].mediaType)
                                            {
                                                if(0 == strcmp((const char *)(vol->devName), 
                                                               (const char *)(sysfsMountTable[index].devName+5)))
                                                {
                                                    /* If the device is present in the mount table then fetch the mount name
                                                       which is the input for the unmount function */
                                                    if(SYS_FS_RES_SUCCESS == SYS_FS_Unmount(sysfsMountTable[index].mountName))
                                                    {
                                                        if (gSYSFSEventHandler)
                                                        {
                                                            /* trigger an event for the application that media has been 
                                                               unmounted */
                                                            gSYSFSEventHandler(SYS_FS_EVENT_UNMOUNT,(void*)
                                                                    sysfsMountTable[index].mountName,
                                                                    (uintptr_t)NULL);
                                                        }
                                                        /* Break the inner for loop so that we donot mount any other volume*/
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                        
                                }
                            }
                            gCurMedia.disk->volumeCounter = 0;
                            gCurMedia.disk->mediaState = SYS_FS_MEDIA_STATE_DETACHED;
                            gCurMedia.taskState = SYS_FS_MM_STATE_DETACHED;
                        }
                        break;
                    case SYS_FS_MEDIA_STATE_DETACHED:
                         /* Check if the media is attached now */
                        if(true == gCurMedia.disk->mediaDriverFunctions->mediaStatusGet(
                                                gCurMedia.disk->mediaDriverClientHandle))
                        {
                            gCurMedia.disk->mediaStatus = SYS_FS_MEDIA_ATTACHED;
                            gCurMedia.disk->mediaState = SYS_FS_MEDIA_STATE_ATTACHED;
                            gCurMedia.taskState = SYS_FS_MM_STATE_ATTACHED;
                        }
                        else /* If media is still detached */
                        {
                            gCurMedia.disk->mediaStatus = SYS_FS_MEDIA_DETACHED;
                            gCurMedia.disk->mediaState = SYS_FS_MEDIA_STATE_DETACHED;
                            gCurMedia.taskState = SYS_FS_MM_STATE_DETACHED;
                        }
                        break;
                    case SYS_FS_MEDIA_STATE_OPENED:
                        /* Switch the state to analyze media */
                        gCurMedia.taskState = SYS_FS_MM_STATE_CHECK_ATTACHED_MEDIA;
                        break;
                    default:
                        break;
                }
            }
            break;
        case SYS_FS_MM_STATE_DETACHED:
            /* Media is detached, keep checking for Media Status */
            gCurMedia.taskState = SYS_FS_MM_STATE_NEW_DISK;
            break;
        case SYS_FS_MM_STATE_OPEN_MEDIA:
            /* Open the driver of the media */
            gCurMedia.disk->mediaDriverClientHandle =
                    gCurMedia.disk->mediaDriverFunctions->open(gCurMedia.disk->index, DRV_IO_INTENT_READWRITE);
            /* If the media driver open is sucessful, set the event handle for Mount/Unmount Events */
            if(gCurMedia.disk->mediaDriverClientHandle != DRV_HANDLE_INVALID)
            {
                /* Set the event handler with Media Driver*/
                gCurMedia.disk->mediaDriverFunctions->eventHandlerset(gCurMedia.disk->mediaDriverClientHandle,
                                                                                  SYS_FS_MEDIA_MANAGER_EventHandler,
                                                                                 (uintptr_t) gCurMedia.disk);
                gCurMedia.disk->mediaState = SYS_FS_MEDIA_STATE_OPENED;
                gCurMedia.taskState = SYS_FS_MM_STATE_CHECK_ATTACHED_MEDIA;
            }
            break;
        case SYS_FS_MM_STATE_CHECK_ATTACHED_MEDIA:
            /* Check if the media is attached */
            if(true == gCurMedia.disk->mediaDriverFunctions->mediaStatusGet(
                                        gCurMedia.disk->mediaDriverClientHandle))
            {
                gCurMedia.disk->mediaStatus = SYS_FS_MEDIA_ATTACHED;
                gCurMedia.disk->mediaState = SYS_FS_MEDIA_STATE_ATTACHED;
                gCurMedia.taskState = SYS_FS_MM_STATE_ATTACHED;
            }
            else
            {
                gCurMedia.disk->mediaState = SYS_FS_MEDIA_STATE_DETACHED;
                gCurMedia.taskState = SYS_FS_MM_STATE_NEW_DISK;
            }
            break;
        case SYS_FS_MM_STATE_ATTACHED:
            /* Get the geometry from the media driver. The geometry from media driver will 
             * return the media read, write and erase block sizes  */
            gCurMedia.disk->mediaGeometry = 
                            gCurMedia.disk->mediaDriverFunctions->mediaGeometryGet
                                                (gCurMedia.disk->mediaDriverClientHandle);

            /* Compute the shift values from block sizes to avoid the multiply and divide operations
             * during read / write from media */
            locShiftOp = gCurMedia.disk->mediaGeometry->geometryTable[SYS_FS_MEDIA_GEOMETRY_READ].blockSize;
            locShiftOp = locShiftOp >> 1;
            gCurMedia.disk->mediaBlockReadShift = 0;
            while(locShiftOp)
            {
                locShiftOp = locShiftOp >> 1;
                gCurMedia.disk->mediaBlockReadShift++;
            }
             /* Compute the shift values from block sizes to avoid the multiply and divide operations
             * during read / write from media */           
            locShiftOp = gCurMedia.disk->mediaGeometry->geometryTable[SYS_FS_MEDIA_GEOMETRY_WRITE].blockSize;
            locShiftOp = locShiftOp >> 1;
            gCurMedia.disk->mediaBlockWriteShift = 0;
            while(locShiftOp)
            {
                locShiftOp = locShiftOp >> 1;
                gCurMedia.disk->mediaBlockWriteShift++;
            }
            /* Clear the buffer before reading from disk */
            memset(gCurMedia.buffer,0,512);

            gCurMedia.disk->mediaCommandStatus = SYS_FS_MEDIA_COMMAND_INPROGRESS;
            /* Read the MBR sector from the media */
            gCurMedia.disk->mediaDriverFunctions->sectorRead(gCurMedia.disk->mediaDriverClientHandle,
                                                                &(gCurMedia.disk->mediaCommandHandle),
                                                                gCurMedia.buffer,
                                                                0, /* Read block 0 / MBR */
                                                                (SYS_FS_MEDIA_MAX_BLOCK_SIZE >> gCurMedia.disk->mediaBlockReadShift));

            
            if((gCurMedia.disk->mediaCommandHandle) != SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID)
            {
                gCurMedia.taskState = SYS_FS_MM_STATE_ANALYZING_FILE_SYSTEM;
            }
            break;
        case SYS_FS_MM_STATE_ANALYZING_FILE_SYSTEM:
            if(true == gCurMedia.disk->mediaDriverFunctions->mediaStatusGet(
                                        gCurMedia.disk->mediaDriverClientHandle))
            {
                while(SYS_FS_MEDIA_COMMAND_INPROGRESS == gCurMedia.disk->mediaCommandStatus)
                {
                    if(gCurMedia.disk->mediaDriverFunctions->tasks != NULL)
                    {
                        gCurMedia.disk->mediaDriverFunctions->tasks(gCurMedia.disk->mediaDriverObject);
                    }
                }
                gCurMedia.disk->mediaCommandStatus = SYS_FS_MEDIA_COMMAND_INPROGRESS;
            }

            fileSystemType = _SYS_FS_MEDIA_MANAGER_AnalyzeFileSystem(gCurMedia.buffer,
                                            &gCurMedia.disk->numberOfPartitions, 
                                            &bootRecord,
                                            &partitionMap);

            if(0xFF == fileSystemType)
            {
                 gCurMedia.disk->mediaState = SYS_FS_MEDIA_STATE_ANALYZED_BUT_UNSUPPORTED;
                 gCurMedia.taskState = SYS_FS_MM_STATE_ANALYZED;
                 break;
            }

            for(volIndex = 0; volIndex < SYS_FS_VOLUME_NUMBER; volIndex++)
            {
                vol = &gSYSFSVolumeObject[volIndex];

                if(vol->inUse != true) /* Found a free volume */
                {
                    switch(gCurMedia.disk->mediaType)
                    {
                        case SYS_FS_MEDIA_TYPE_SD_CARD:
                            devNameSize = 6;
                            memcpy(vol->devName,"mmcblk",devNameSize);
                            break;
                        case SYS_FS_MEDIA_TYPE_NVM:
                            devNameSize = 3;
                            memcpy(vol->devName,"nvm",devNameSize);
                            break;
                        case SYS_FS_MEDIA_TYPE_SPIFLASH:
                            devNameSize = 3;
                            memcpy(vol->devName,"mtd",devNameSize);
                            break;
                        case SYS_FS_MEDIA_TYPE_MSD:
                            devNameSize = 2;
                            memcpy(vol->devName,"sd",devNameSize);
                            break;
                        default:
                            break;
                    }

                    vol->devName[devNameSize++] = gCurMedia.disk->mediaId;
                    gCurMedia.disk->volumeCounter++;
                    vol->devName[devNameSize++] = gCurMedia.disk->volumeCounter + '0';
                    vol->devName[devNameSize] = '\0';
                    devNameSize = 0;
                    vol->obj = gCurMedia.disk;
                    vol->fileSystemType = fileSystemType;
                    /* Register Media and Volume mapping with FAT File System */
                    /* Register the volumes to partition table only if this device has 
                        partition table entry or MBR. Skip if media contains VBR */
                    if (partitionMap)
                    {
                        if (partitionMap & 0x01)
                        {
                            partitionNum = 0;
                            partitionMap &= ~0x01;
                        }
                        else if (partitionMap & 0x02)
                        {
                            partitionNum = 1;
                            partitionMap &= ~0x02;
                        }
                        else if (partitionMap & 0x04)
                        {
                            partitionNum = 2;
                            partitionMap &= ~0x04;
                        }
                        else if (partitionMap & 0x08)
                        {
                            partitionNum = 3;
                            partitionMap &= ~0x08;
                        }

                        vol->numberOfSector = ((gCurMedia.buffer[partitionTypeOffset[partitionNum] + 11] << 24)
                                              + (gCurMedia.buffer[partitionTypeOffset[partitionNum] + 10] << 16)
                                              + (gCurMedia.buffer[partitionTypeOffset[partitionNum] + 9] << 8)
                                              + gCurMedia.buffer[partitionTypeOffset[partitionNum] + 8]);
                        vol->startSector = ((gCurMedia.buffer[partitionTypeOffset[partitionNum] + 7] << 24)
                                           + (gCurMedia.buffer[partitionTypeOffset[partitionNum] + 6] << 16)
                                           + (gCurMedia.buffer[partitionTypeOffset[partitionNum] + 5] << 8)
                                           + gCurMedia.buffer[partitionTypeOffset[partitionNum] + 4]);
                    }
                    else
                    {
                        if ('M' == vol->fileSystemType)
                        {
                            vol->numberOfSector = 0;
                            vol->startSector = 0;
                        }
                    }

                    if(bootRecord)
                    {
                        _SYS_FS_MEDIA_MANAGER_updateVolToPart(volIndex, 
                                                           gCurMedia.disk->mediaCounter, 
                                                           partitionNum + 1);
                    }
                    else
                    {
                        _SYS_FS_MEDIA_MANAGER_updateVolToPart( volIndex, 
                                                               gCurMedia.disk->mediaCounter, 
                                                               0);                        
                    }
                    

                    /* Set the flag at the end, this is to ensure the syncronization between mount API and
                    task routine. Mount can called from one task while media manager task is running from
                    another task.  */
                    vol->inUse = true;
                    /* Mount the Media if Auto mount feature is enable */
                    if(SYS_FS_AUTOMOUNT_ENABLE)
                    {
                        /* Find out the registered mount name for this Volume */
                        for(index = 0; index < SYS_FS_VOLUME_NUMBER; index++)
                        {
                            if(gCurMedia.disk->mediaType == sysfsMountTable[index].mediaType)
                            {
                                /* If the device name of volume is found in the mount table */
                                if(0 == strcmp((const char *)(gSYSFSVolumeObject[volIndex].devName), 
                                                     (const char *)(sysfsMountTable[index].devName+5)))
                                {
                                    
                                    /* Mount the Media , fetch Mount name and device name from the table  */
                                    if(0 != SYS_FS_Mount(sysfsMountTable[index].devName, 
                                                sysfsMountTable[index].mountName, 
                                                sysfsMountTable[index].fsType, 0, NULL))
                                    {   
                                        gCurMedia.taskState = SYS_FS_MM_STATE_NEW_DISK;
                                    }   
                                    else
                                    {
                                        if (gSYSFSEventHandler)
                                        {
                                            /* Call the event handler to raise Mount Notification */
                                            gSYSFSEventHandler(SYS_FS_EVENT_MOUNT,(void*)sysfsMountTable[index].mountName,(uintptr_t)NULL);
                                            gCurMedia.taskState = SYS_FS_MM_STATE_NEW_DISK;
                                            break;
                                        }
                                    }
                                }
                            }                           
                        }
                    }

                    /* Continue if there are more than one partition on media */
                    if (!partitionMap)
                        break;
                }
            }

            gCurMedia.disk->mediaState = SYS_FS_MEDIA_STATE_ANALYZED;
            gCurMedia.taskState = SYS_FS_MM_STATE_NEW_DISK;
            break;           
        default:
            break;
    }
}

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
)
{
	SYS_FS_MEDIA *media = NULL;
	uint32_t numofblocks =0;
    uint32_t sizeinbytes =0;

    if(diskNo >= SYS_FS_VOLUME_NUMBER)
    {
        SYS_ASSERT(false, "Invalid Disk");
        return SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;
    }	
	
    media = &gSYSFSMediaObject[diskNo];

    if(media->mediaDriverClientHandle == DRV_HANDLE_INVALID)
    {
        return SYS_FS_MEDIA_HANDLE_INVALID;
    }
    /* Number of sectors * Sector Size */
    sizeinbytes = noSectors << SYS_FS_MEDIA_SHIFT_SECTOR_VALUE;

    /* compute the start block number */
    if (media->mediaBlockReadShift > SYS_FS_MEDIA_SHIFT_SECTOR_VALUE)
    {
        sector >>= (media->mediaBlockReadShift - SYS_FS_MEDIA_SHIFT_SECTOR_VALUE);
    }
    else
    {
        sector <<= (SYS_FS_MEDIA_SHIFT_SECTOR_VALUE - media->mediaBlockReadShift);
    }

    /* compute number of block numbers to read */
    numofblocks = sizeinbytes >> media->mediaBlockReadShift;
        
    media->mediaCommandStatus = SYS_FS_MEDIA_COMMAND_INPROGRESS;
    /* call media driver function to read the data from media */
    media->mediaDriverFunctions->sectorRead(media->mediaDriverClientHandle,
                                            &(media->mediaCommandHandle),
                                            dataBuffer, sector, numofblocks);

    return (media->mediaCommandHandle);
}

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
 * Function to read data from a specific address of media. This function is intended to work with
 * NVM media only, which can have byte level adressing. For other media (like SD card),byte
 * addressing is not possible and hence this function will not work. Also, this function is
 * intended to work with MPFS2 file system only, which uses byte adressing.
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
( uint16_t diskNo, uint8_t* destination, uint8_t * source, const unsigned int nBytes )
{
    SYS_FS_VOLUME * vol =NULL;
    uint32_t numofblocks=0;
    uint32_t blockaddress=0;
    uint32_t blockstart=0;

    if(diskNo >= SYS_FS_VOLUME_NUMBER)
    {
        SYS_ASSERT(false, "Invalid Disk");
        return SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;
    }

    vol = &gSYSFSVolumeObject[diskNo];

    if(vol->obj->mediaDriverClientHandle == DRV_HANDLE_INVALID)
    {
        return SYS_FS_MEDIA_HANDLE_INVALID;
    }
    /* compute the start and number of block to read */
    numofblocks = nBytes / vol->obj->mediaGeometry->geometryTable[SYS_FS_MEDIA_GEOMETRY_READ].blockSize;

    blockaddress = vol->obj->mediaDriverFunctions->addressGet(vol->obj->mediaDriverClientHandle);

    blockstart = (uint32_t)source - (uint32_t)blockaddress;
    /* call media driver function to read from media */
    vol->obj->mediaDriverFunctions->Read(vol->obj->mediaDriverClientHandle,
                                         &(vol->obj->mediaCommandHandle),
                                         destination, blockstart, numofblocks);

    return (vol->obj->mediaCommandHandle);
}

/*****************************************************************************
 * Function:
 * uintptr_t SYS_FS_MEDIA_MANAGER_AddressGet
 * (
 *  uint16_t diskNo,
 * )
 *
 * Description:
 * Function to know the NVM memory address against a disk number. This function
 * is intended to work only with MPFS2, which does byte adressing and hence needs
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
)
{
    SYS_FS_VOLUME * vol = NULL;

    if(diskNo >= SYS_FS_VOLUME_NUMBER)
    {
        SYS_ASSERT(false, "Invalid Disk");
        return (uintptr_t)NULL;
    }

    vol = &gSYSFSVolumeObject[diskNo];

    return(vol->obj->mediaDriverFunctions->addressGet(vol->obj->mediaDriverClientHandle));
}

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
)
{
	SYS_FS_MEDIA *media = NULL;
	uint32_t numofblocks = 0;
    uint32_t sizeinbytes = 0;
    uint32_t blockstart = 0;

    if(diskNo >= SYS_FS_VOLUME_NUMBER)
	{
		SYS_ASSERT(false, "Invalid Disk");
		return SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;
	}	
	
	media = &gSYSFSMediaObject[diskNo];

    if(media->mediaDriverClientHandle == DRV_HANDLE_INVALID)
    {
        return SYS_FS_MEDIA_HANDLE_INVALID;
    }

    sizeinbytes = noSectors << SYS_FS_MEDIA_SHIFT_SECTOR_VALUE;

    blockstart =  (sector << SYS_FS_MEDIA_SHIFT_SECTOR_VALUE) >> media->mediaBlockWriteShift;

    /* assumption that sizeinbytes is never more than Row Size of device */
    if(sizeinbytes % media->mediaGeometry->geometryTable[SYS_FS_MEDIA_GEOMETRY_WRITE].blockSize)
    {
               
        /* Read block size is more than the requested number of sectors to read*/
          media->mediaCommandStatus = SYS_FS_MEDIA_COMMAND_INPROGRESS;

          media->mediaDriverFunctions->sectorRead(media->mediaDriverClientHandle,
                                          &(media->mediaCommandHandle),
                                          gSYSFSPagebuffer, blockstart,1);

        while(SYS_FS_MEDIA_COMMAND_INPROGRESS == media->mediaCommandStatus)
        {
            if(media->mediaDriverFunctions->tasks != NULL)
            {
                media->mediaDriverFunctions->tasks(media->mediaDriverObject);
            }
        }
        sector %= ( media->mediaGeometry->geometryTable[SYS_FS_MEDIA_GEOMETRY_WRITE].blockSize / 
                                            SYS_FS_MEDIA_MAX_BLOCK_SIZE);

        uint32_t startIndex = sector << SYS_FS_MEDIA_SHIFT_SECTOR_VALUE;

        memcpy((void *)&gSYSFSPagebuffer[startIndex],(const void *)dataBuffer,(size_t)sizeinbytes);

        media->mediaCommandStatus = SYS_FS_MEDIA_COMMAND_INPROGRESS;
        media->mediaDriverFunctions->sectorWrite(media->mediaDriverClientHandle,
                                    &(media->mediaCommandHandle),
                                    gSYSFSPagebuffer, blockstart, 1);
    }
    else
    {
        numofblocks = sizeinbytes >> media->mediaBlockWriteShift;

        media->mediaCommandStatus = SYS_FS_MEDIA_COMMAND_INPROGRESS;

        media->mediaDriverFunctions->sectorWrite(media->mediaDriverClientHandle,
                                                &(media->mediaCommandHandle),
                                                dataBuffer, blockstart, numofblocks);
     }
        
    return (media->mediaCommandHandle);

}

/*****************************************************************************
 * Function:
 * SYS_FS_MEDIA_BUFFER_STATUS SYS_FS_MEDIA_MANAGER_EventHandler
 *   (
 *       SYS_FS_MEDIA_BLOCK_EVENT event,
 *       SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle,
 *       uintptr_t context
 *   )
 *
 * Description:
 * Function to know the buffer status. The sector read and sector write are
 * non blocking functions. Hence, this interface is provided, where the code
 * should periodically poll for the buffer status. If status is completed,
 * the read/ write operation is considered to be complete.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   diskNo         - media number
 *   bufferHandle   - the buffer handle which was obtained during sector read/ write
 *
 * Returns:
 *   Buffer status of type SYS_FS_MEDIA_BUFFER_STATUS
*/
void  SYS_FS_MEDIA_MANAGER_EventHandler (
        SYS_FS_MEDIA_BLOCK_EVENT event,
        SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle,
        uintptr_t context)
{
     
    switch(event)
    {
        case SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_COMPLETE:
            ((SYS_FS_MEDIA*)context)->mediaCommandStatus = SYS_FS_MEDIA_COMMAND_SUCCESS;
            break;
        case SYS_FS_MEDIA_EVENT_BLOCK_COMMAND_ERROR:
            ((SYS_FS_MEDIA*)context)->mediaCommandStatus = SYS_FS_MEDIA_COMMAND_FAILURE;
            break;
        default:
            break;
    }
}
/*****************************************************************************
 * Function:
 * SYS_FS_MEDIA_BUFFER_STATUS SYS_FS_MEDIA_MANAGER_CommandStatusGet
 *   (
 *       uint16_t diskNo,
 *       SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle
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
 *   bufferHandle   - the buffer handle which was obtained during sector read/ write
 *
 * Returns:
 *   Buffer status of type SYS_FS_MEDIA_BUFFER_STATUS
*/
SYS_FS_MEDIA_COMMAND_STATUS SYS_FS_MEDIA_MANAGER_CommandStatusGet
(
    uint16_t     diskNo,
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle
)
{
    SYS_FS_MEDIA *media;
	
	if(diskNo >= SYS_FS_VOLUME_NUMBER)
	{
		SYS_ASSERT(false, "Invalid Disk");
		return SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID;
	}	
	
	media = &gSYSFSMediaObject[diskNo];

    if(media->mediaDriverClientHandle == DRV_HANDLE_INVALID)
    {
        return SYS_FS_MEDIA_COMMAND_UNKNOWN;
    }

    return(media->mediaDriverFunctions->commandStatusGet(media->mediaDriverClientHandle,
                                                            commandHandle));
}
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
 *   Media status of type SYS_FS_MEDIA_STATUS
*/
bool SYS_FS_MEDIA_MANAGER_MediaStatusGet( const char *devName )
{
	SYS_FS_VOLUME * vol =  NULL;
    uint8_t volIndex = 0;

    // Start with 0th disk and find a disk which is available (not in use)
    for(volIndex = 0; volIndex < SYS_FS_VOLUME_NUMBER; volIndex++)
    {
        vol = &gSYSFSVolumeObject[volIndex];
        if(vol->inUse == true)
        {
            if(strncmp("/dev/",devName,5))
            {
                if(strcmp((const char*)(devName), (const char *)vol->devName) == 0)
                {
                    return (vol->obj->mediaDriverFunctions->mediaStatusGet(
                                                vol->obj->mediaDriverClientHandle));
                }
            }
            else
            {
                // Find the element from "gSYSFSVolumeObject" which has matching "devName"
                if(strcmp((const char*)(devName+5), (const char *)vol->devName) == 0)
                {
                    return (vol->obj->mediaDriverFunctions->mediaStatusGet(
                                                vol->obj->mediaDriverClientHandle));                    
                }
            }
        }
    }

    return SYS_FS_MEDIA_DETACHED;
}	

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

bool SYS_FS_MEDIA_MANAGER_VolumePropertyGet( const char *devName, SYS_FS_VOLUME_PROPERTY *str)
{
	SYS_FS_VOLUME * vol = NULL;
    uint8_t volIndex = 0;

    // Start with 0th disk and find a disk which is available (not in use)
    for(volIndex = 0; volIndex < SYS_FS_VOLUME_NUMBER; volIndex++)
    {
        vol = &gSYSFSVolumeObject[volIndex];
        if(vol->inUse == true)
        {
            // Find the element from "gSYSFSVolumeObject" which has matching "devName"
            if(strcmp((const char*)(devName + 5), (const char *)vol->devName) == 0)
            {
                switch(vol->fileSystemType)
                {
                    case 0x01: // FAT12
                    case 0x04: // FAT16
                    case 0x05: // Extended partitino
                    case 0x06: // FAT16
                    case 0x0B: // FAT32
                    case 0x0C: // FAT32
                    case 0x0E: // FAT16
                    case 0x0F: // FAT16
                        str->fsType = FAT;
                    break;
                    case 'M': // MPFS
                        str->fsType = MPFS2;
                    break;
                    default:
                        return false;                           
                }
                    str->volNumber = volIndex;
                    return true;

            }
        }
    }
    return(false);
}

/*****************************************************************************
 * Function:
 * void _SYS_FS_MEDIA_MANAGER_updateVolToPart(uint8_t volNumber, uint8_t pd, uint8_t pt)
 *
 * Description:
 * This function is present to enable the multipartition operation
 * of FAT FS. FAT FS uses the structure VolToPart to know the physical
 * drive media number and partition number of that media, using this
 * structure. This function is used to populate the structure.
 * REFER TO MORE EXPLANATION on SYS_fs_media_manager_local.h
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   volNumber      - volume number
 *   pd             - physical drive number (media number)
 *   pt             - partition number from that media
 *
 * Returns:
 *   none.
*/

void _SYS_FS_MEDIA_MANAGER_updateVolToPart(uint8_t volNumber, uint8_t pd, uint8_t pt)
{
    /* Update the "VolToPart" table for multipartition support on FAT FS */
    /* pd = Physical drive, starting from "zero". This is to be compatible with FAT FS code */
    VolToPart[volNumber].pd = pd;
    /* pt = partition, starting from "one". This is to be compatible with FAT FS code */
    VolToPart[volNumber].pt = pt;
}
/*****************************************************************************
 * Function:
 * uint8_t _SYS_FS_MEDIA_MANAGER_AnalyzeFileSystem(uint8_t* firstSector,uint8_t* noOfPartition,uint8_t* bootRecord)
 *
 * Description:
 * This function analyzes the type of file system on media. It also finds out
 * how many partitions are available on media.
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   firstSector      - Pointer to a buffer that contains MBR
 *   noOfPartition    - No of partitions on media
 *
 * Returns:
 *   uint8_t Type of File System on Media.
*/

uint8_t _SYS_FS_MEDIA_MANAGER_AnalyzeFileSystem
(
    uint8_t* firstSector,
    uint8_t* noOfPartition,
    uint8_t* bootRecord,
    uint8_t *partitionMap
)
{
    uint8_t numberOfPartitions = 0;
    uint8_t fileSystemType = 0xFF;

    *partitionMap = 0;
    /* Sheck for the Valid Boot Signature if this is the FAT */
    if((firstSector[510] == 0x55) && (firstSector[511] == 0xAA))
    {
        /* analyze if the first sector is the boot sector on a media that has one partition */
        /* check if the first sector on the media is volume Boot record and not Master Boot Record */
        if((0xEB == firstSector[0]) &&
                ((0x3C == firstSector[1]) || (0x58 == firstSector[1]) || (0xFE == firstSector[1])) &&
                (0x90 == firstSector[2]))
        {
            numberOfPartitions = 1;
            *bootRecord = 0;

            if( (0x46 == firstSector[82]) || (0x41 == firstSector[83]) ||
                    (0x54 == firstSector[84]))
            {
                /* File System tyoe should be read from the partition type*/
                /* this implementation need to be corrected */
                fileSystemType = 0x0B;
            }
            else if( (0x46 == firstSector[54]) || (0x41 == firstSector[55]) ||
                    (0x54 == firstSector[56]))
            {
                fileSystemType = 0x06;
            }
        }
        else
        {     
            /* The partition table in the MBR sector has room for four 16-byte entries that each specify the sectors
             * that belong to a partition. The table is in bytes 446 through 509. An entry can begin at
             *  byte 446, 462, 478, or 494
             */
            /* Determine total partitions in the media */
            *bootRecord = 1;
            if(firstSector[450] != 0)    // 1st
            {
                *partitionMap = (1 << 0);
                numberOfPartitions++;
                fileSystemType = firstSector[450];
            }
            if(firstSector[466] != 0)    // 2nd
            {
                *partitionMap = (1 << 1);
                numberOfPartitions++;
                fileSystemType = firstSector[466];
            }
            if(firstSector[482] != 0)    // 3rd
            {
                *partitionMap = (1 << 2);
                numberOfPartitions++;
                fileSystemType = firstSector[482];
            }
            if(firstSector[498] != 0)    // 4th
            {
                *partitionMap = (1 << 3);
                numberOfPartitions++;
                fileSystemType = firstSector[498];
            }
        }
    }
    else if(0 == memcmp(firstSector,"MPFS",4))
    {
        /* No partitions in MPFS, hence, go to other state */
        /* allocate a new volume to each partition */
        numberOfPartitions = 1;
        /* This is 0x4D which also mean file system Primary QNX POSIX volume on disk */
        /* Need to find an unused value from the partition type*/
        fileSystemType = 'M';
        gCurMedia.taskState = SYS_FS_MM_STATE_ALLOCATING_VOLUME;
    }
    else    /* If MBR is not detected, make media as unsupported */
    {
        /* File system either not present or not supported */
        fileSystemType = 0xFF;
    }

    *noOfPartition = numberOfPartitions;

    return fileSystemType;
}

/*****************************************************************************
 * Function:
 * DRESULT disk_read
 *   (
 *       uint8_t pdrv,
 *       uint8_t *buff,
 *       uint32_t sector,
 *       uint8_t count
 *   )
 *
 * Description:
 * Function to read a sector of specified media (disk). This is the function
 * called from ff.c. This function is block untill the read is complete.
 * Since this function is called by ff.c, the name of function, parameter passed
 * and return types are not changed. This is to avoid compatibility issues if we
 * use future releases of ff.c
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   pdrv         - media number
 *   buff         - pointer to buffer where data to be placed after read
 *   sector       - Sector # which is to be read
 *   count        - Number of sectors to read
 *
 * Returns:
 *   DRESULT
*/

DRESULT disk_read (
        uint8_t pdrv,		/* Physical drive nmuber (0..) */
        uint8_t *buff,		/* Data buffer to store read data */
        uint32_t sector,	/* Sector address (LBA) */
        uint8_t count		/* Number of sectors to read (1..128) */
        )
{
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle;
    SYS_FS_MEDIA *media = NULL;
   
    media = &gSYSFSMediaObject[pdrv];
    if(true != media->mediaDriverFunctions->mediaStatusGet(media->mediaDriverClientHandle))
    {
        /* Media is not attached, do not proceed further */
        /* return read/write error to file system */
        return RES_ERROR;
    }
    /* submit the read request */
    commandHandle = SYS_FS_MEDIA_MANAGER_SectorRead(pdrv /* DISK 0 */ ,
            buff /* Destination Sector*/,
            sector,
            count /* Number of Sectors */);
    
    /* Buffer is invalid report error */
    if(SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID == commandHandle)
    {
        return RES_PARERR;
    }

    /* process the read request by blocking on the task routine that process the 
     I/O request */
    while(SYS_FS_MEDIA_COMMAND_INPROGRESS == media->mediaCommandStatus)
    {

        if(media->mediaDriverFunctions->tasks != NULL)
            media->mediaDriverFunctions->tasks(media->mediaDriverObject);
    }
    
    if(SYS_FS_MEDIA_COMMAND_SUCCESS == media->mediaCommandStatus)
    {
        /* Buffer processed successfully */
        return RES_OK;
    }        
    else
    {
        /* Buffer processing failed */
        return RES_ERROR;
    }

}
/*****************************************************************************
 * Function:
 * DRESULT disk_write
 *   (
 *       uint8_t pdrv,
 *       uint8_t *buff,
 *       uint32_t sector,
 *       uint8_t count
 *   )
 *
 * Description:
 * Function to write to a sector of specified media (disk). This is the function
 * called from ff.c. This function is block untill the write is complete.
 * Since this function is called by ff.c, the name of function, parameter passed
 * and return types are not changed. This is to avoid compatibility issues if we
 * use future releases of ff.c
 *
 * Precondition:
 *   None
 *
 * Parameters:
 *   pdrv         - media number
 *   buff         - pointer to buffer which holds the data to write
 *   sector       - Sector # where data to be written
 *   count        - Number of sectors to write
 *
 * Returns:
 *   DRESULT
*/

#if _USE_WRITE
DRESULT disk_write (
        uint8_t pdrv,			/* Physical drive nmuber (0..) */
        const uint8_t *buff,	/* Data to be written */
        uint32_t sector,		/* Sector address (LBA) */
        uint8_t count			/* Number of sectors to write (1..128) */
        )
{
    SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE commandHandle;
    SYS_FS_MEDIA *media = NULL;
    
    media = &gSYSFSMediaObject[pdrv];
    if(true != media->mediaDriverFunctions->mediaStatusGet(media->mediaDriverClientHandle))
    {
        /* Media is not attached, do not proceed further */
        /* return read/write error to file system */
        return RES_ERROR;
    }
    /* Submit the write request to media */
    commandHandle = SYS_FS_MEDIA_MANAGER_SectorWrite(pdrv /* DISK 0 */ ,
            sector /* Destination Sector*/,
            (uint8_t *)buff,
            count /* Number of Sectors */);
    /* Write request failed , return with error */
    if(SYS_FS_MEDIA_BLOCK_COMMAND_HANDLE_INVALID == commandHandle)
    {
        return RES_PARERR;
    }
    /* Run the task routine of media to process the request  */
    while(SYS_FS_MEDIA_COMMAND_INPROGRESS == media->mediaCommandStatus)
    {
        if(media->mediaDriverFunctions->tasks != NULL)
            media->mediaDriverFunctions->tasks(media->mediaDriverObject);
    }
    
    if(SYS_FS_MEDIA_COMMAND_SUCCESS == media->mediaCommandStatus)
    {
        /* Buffer processed successfully */
        return RES_OK;
    }        
    else
    {
        /* Buffer processing failed */
        return RES_ERROR;
    }
}
#endif
