/******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    sys_fs.h

  Summary:
    This file contains function and type declarations required to interact with
    the MPLAB Harmony File System Framework.

  Description:
    File System System-Library Interface Declarations and types.

    This file contains function and type declarations required to interact
    with the MPLAB Harmony File System Framework.
********************************************************************************/

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

#ifndef		_SYS_FS
#define		_SYS_FS

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  


// *****************************************************************************
/* SYS FS File Handle

  Summary:
    This type defines the file handle.

  Description:
    While the application code opens a file, a file handle is returned by the
    open function (on successful open). This type defines the type of file
    handle.

  Remarks:
    None.
*/

typedef uintptr_t       SYS_FS_HANDLE;

// *****************************************************************************
/* SYS FS File Invalid Handle

  Summary:
    This definitions defines the SYS FS file invalid handle.

  Description:
    While the application code opens a file, if the file open is unsuccessful,
    an invalid file handle is returned. This definition defines the type of
    invalid file handle.

  Remarks:
    None.
*/

#define SYS_FS_HANDLE_INVALID ((SYS_FS_HANDLE)(-1))

//*******************************************************************************
/* File System type

  Summary:
    Enumerated data type identifying native file systems supported.

  Description:
    These enumerated values are the possible native file system that can be
    supported by the SYS FS layer.

  Remarks:
	None.

*/

typedef enum
{
	/* FAT FS native File system */
	FAT,
	/* MPFS2 native File system */
	MPFS2

} SYS_FS_FILE_SYSTEM_TYPE;

// *****************************************************************************
/* File or directory attribute

  Summary:
    Enumerated data type identifying the various attributes for file/ directory.

  Description:
    These enumerated values are the possible attributes for a file or directory.

  Remarks:
    None.
*/

typedef enum
{
	/* Read only */
    SYS_FS_ATTR_RDO =   0x01,
	/* Hidden */
    SYS_FS_ATTR_HID =   0x02,
	/* System */
    SYS_FS_ATTR_SYS =   0x04,
	/* Volume label */
    SYS_FS_ATTR_VOL =   0x08,
	/* LFN entry */
    SYS_FS_ATTR_LFN =   0x0F,
	/* Directory */
    SYS_FS_ATTR_DIR =   0x10,
	/* Archive */
    SYS_FS_ATTR_ARC =   0x20,
	/* Mask of defined bits */
    SYS_FS_ATTR_MASK = 	0x3F

}SYS_FS_FILE_DIR_ATTR;

// *****************************************************************************
/* File Seek control

  Summary:
    Lists the various modes of file seek

  Description:
    This enumeration lists the various modes of file seek. When the application
    calls the SYS_FS_FileSeek function, it specifies the kind of seek that needs
    to be performed. The enumeration lists the various modes of file seek.

  Remarks:
    None.
*/
typedef enum
{
    /* The file offset shall be set to input number of bytes from the start */
	SYS_FS_SEEK_SET,
    /* The file offset shall be set to its current location plus input number of
       bytes */
	SYS_FS_SEEK_CUR,
    /* The file offset shall be set to the size of the file plus input number of
       bytes */
	SYS_FS_SEEK_END,

} SYS_FS_FILE_SEEK_CONTROL;

// *****************************************************************************
/* File operation result enum

  Summary:
    Lists the various results of a file operation

  Description:
    This enumeration lists the various results of a file operation. When a file
    operation function is called from the application, and if the return type of
    the function is SYS_FS_RESULT, then, the enumeration below specifies the
    value of function return.

  Remarks:
    None.
*/

typedef enum
{
	/* Operation successful */
    SYS_FS_RES_SUCCESS	=  0,
	/* operation failed */
    SYS_FS_RES_FAILURE	= -1

} SYS_FS_RESULT;

// *****************************************************************************
/* File formating partition rule

  Summary:
    Lists the various formats of file formatting

  Description:
    When a disk is the enumeration below specified the format. When
    SYS_FS_FORMAT_FDISK format is specified, a primary partition occupies the
    entire disk space is created and then an FAT volume is created on the
    partition. When SYS_FS_FORMAT_SFD format is specified, the FAT volume starts
    from the first sector of the physical drive.

    Note that there are two partitioning rules, SYS_FS_FORMAT_FDISK and
    SYS_FS_FORMAT_SFD. The SYS_FS_FORMAT_FDISK partitioning is usually used for
    hard disk, MMC, SDC, CFC and U Disk. It can divide a physical drive into one
    or more partitions with a partition table on the MBR. However Windows does
    not support multiple partition on the removable media. The SYS_FS_FORMAT_SFD
    is non-partitioned method. The FAT volume starts from the first sector on
    the physical drive without partition table. It is usually used for floppy
    disk, microdrive, optical disk, and super-floppy media.

*/

typedef enum
{
	/* Format disk with multiple partition */
    SYS_FS_FORMAT_FDISK	=  0,
	/* Format disk with single partition */
    SYS_FS_FORMAT_SFD	= 1

}SYS_FS_FORMAT;

// *****************************************************************************
/* File open attributes

  Summary:
    Lists the various attributes (modes) in which a file can be opened

  Description:
    Lists the various attributes (modes) in which a file can be opened

  Remarks:
    None.
*/

typedef enum
{
        /*

           reading the file    =   possible, if file exists.

           reading the file    =   file open returns error, if file does not
                                   exist.

           writing to the file =   not possible. Write operation returns error

         */
    SYS_FS_FILE_OPEN_READ	=  0,

        /*

           reading the file    =  not possible. Read operation returns error

           writing to the file =  possible. If file exists, write happens from
                                  the beginning of the file, overwriting the existing
                                  content of the file.

           writing to the file =  If file does not exist, a new file will be created
                                  and data will be written into the newly created file.

         */
    SYS_FS_FILE_OPEN_WRITE,

        /*

           reading the file    =  not possible. Read operation returns error

           writing to the file =  possible. If file exists, write happens from
                                  the end of the file, preserving the existing
                                  content of the file.

           writing to the file =  If file does not exist, a new file will be created
                                  and data will be written into the newly created file.

         */
    SYS_FS_FILE_OPEN_APPEND,

        /*

           reading the file    =   possible, if file exists.

           reading the file    =   file open returns error, if file does not
                                   exist.

           writing to the file =   possible, if file exists, staring from the beginning
                                   of the file (overwriting).

            writing to the file =  file open returns error, if file does not
                                   exist.

         */
    SYS_FS_FILE_OPEN_READ_PLUS,

        /*

           reading the file    =  possible, if file exists.

           reading the file    =  If file does not exist, a new file will be
                                  created.

           writing to the file =  possible. If file exists, write happens from
                                  the beginning of the file, overwriting the existing
                                 content of the file.

           writing to the file =  If file does not exist, a new file will be created
                                  and data will be written into the newly created file.

         */
    SYS_FS_FILE_OPEN_WRITE_PLUS,

        /*

           reading the file    =  possible, if file exists. File read pointer
                                  will be moved to end of the file in this mode.

           reading the file    =  If file does not exist, a new file will be
                                  created.

           writing to the file =  possible. If file exists, write happens from
                                  the end of the file, preserving the existing
                                  content of the file.

           writing to the file =  If file does not exist, a new file will be created
                                  and data will be written into the newly created file.

         */
    SYS_FS_FILE_OPEN_APPEND_PLUS

}SYS_FS_FILE_OPEN_ATTRIBUTES;

// *****************************************************************************
/* File Error error enumeration

  Summary:
    Lists the various error cases

  Description:
    This enumeration lists the various error cases. When the application calls
    for a file system function which has a return type of SYS_FS_RESULT and if
    the return value is SYS_FS_RES_FAILURE, the application can know the
    specific reason for failure by calling the SYS_FS_FileError function. The
    return value of SYS_FS_FileError function will be one of the enumeration of
    the type SYS_FS_ERROR.

  Remarks:
    None.

*/

typedef enum
{
    /* Success */
    SYS_FS_ERROR_OK = 0,
    /* (1) A hard error occurred in the low level disk I/O layer */
    SYS_FS_ERROR_DISK_ERR,
    /* (2) Assertion failed */
    SYS_FS_ERROR_INT_ERR,
    /* (3) The physical drive cannot work */
    SYS_FS_ERROR_NOT_READY,
    /* (4) Could not find the file */
    SYS_FS_ERROR_NO_FILE,
    /* (5) Could not find the path */
    SYS_FS_ERROR_NO_PATH,
    /* (6) The path name format is invalid */
    SYS_FS_ERROR_INVALID_NAME,
    /* (7) Access denied due to prohibited access or directory full */
    SYS_FS_ERROR_DENIED,
    /* (8) Access denied due to prohibited access */
    SYS_FS_ERROR_EXIST,
    /* (9) The file/directory object is invalid */
    SYS_FS_ERROR_INVALID_OBJECT,
    /* (10) The physical drive is write protected */
    SYS_FS_ERROR_WRITE_PROTECTED,
    /* (11) The logical drive number is invalid */
    SYS_FS_ERROR_INVALID_DRIVE,
    /* (12) The volume has no work area */
    SYS_FS_ERROR_NOT_ENABLED,
    /* (13) There is no valid volume */
    SYS_FS_ERROR_NO_FILESYSTEM,
    /* (14) The Format() aborted due to any parameter error */
    SYS_FS_ERROR_FORMAT_ABORTED,
    /* (15) Could not get a grant to access the volume within defined period */
    SYS_FS_ERROR_TIMEOUT,
    /* (16) The operation is rejected according to the file sharing policy */
    SYS_FS_ERROR_LOCKED,
    /* (17) LFN working buffer could not be allocated */
    SYS_FS_ERROR_NOT_ENOUGH_CORE,
    /* (18) Number of open files */
    SYS_FS_ERROR_TOO_MANY_OPEN_FILES,
    /* (19) Given parameter is invalid */
    SYS_FS_ERROR_INVALID_PARAMETER,
    /* (20) Too many mounts requested. Not enough free volume available */
    SYS_FS_ERROR_NOT_ENOUGH_FREE_VOLUME,
    /* (21) Requested native file system is not supported */
    SYS_FS_ERROR_FS_NOT_SUPPORTED,
    /* (22) Requested native file system does not match the format of volume */
    SYS_FS_ERROR_FS_NOT_MATCH_WITH_VOLUME,
    /* (23) Function not supported in native file system layer */
    SYS_FS_ERROR_NOT_SUPPORTED_IN_NATIVE_FS

} SYS_FS_ERROR;
// *****************************************************************************
/* SYS FS Media Events

   Summary
    Identifies the possible file system events.

   Description
    This enumeration identifies the possible events that can result from a
    file system.

   Remarks:
    One of these values is passed in the "event" parameter of the event
    handling callback function that client registered with the file system by
    setting the event handler when media mount or unmount is completed.
*/

typedef enum
{
   /* Media has been mounted successfully. */
    SYS_FS_EVENT_MOUNT,
    /* Media has been unmounted successfully. */
    SYS_FS_EVENT_UNMOUNT,       
    /* There was an error during the operation */
    SYS_FS_EVENT_ERROR

} SYS_FS_EVENT;

// *****************************************************************************
/* FAT File System Sector size

  Summary:
    Lists the definitions for FAT file system sector size.

  Description:
    Maximum sector size to be handled.  Always set the value of sector size to
    512

  Remarks:
    None.
*/

#define	FAT_FS_MAX_SS	512

// *****************************************************************************
/* FAT File System LFN (long file name) selection

  Summary:
    Lists the definitions for FAT file system LFN selection

  Description:
    The FAT_FS_USE_LFN option switches the LFN support.  Set the value to 1

  Remarks:
    None.

*/

#define	FAT_FS_USE_LFN	1

// *****************************************************************************
/* FAT File System LFN (long file name) max length

  Summary:
    Lists the maximum length of file name during LFN selection

  Description:
	Set the value to 255

  Remarks:
    None.
*/

#define	FAT_FS_MAX_LFN	255

// *****************************************************************************
/* SYS FS Function signature structure for native file systems

  Summary:
    SYS FS Function signature structure for native file systems

  Description:
    The SYS FS layer supports functions from each native file system layer. This
    structure specifies the signature for each function from native file system
    (parameter that needs to be passed to each function and return type for each
    function). If a new native file system is to be integrated with the SYS FS
    layer, the functions should follow the signature.

	The structure of function pointer for 2 native file system	-- FAT FS and
	MPFS2 is already provided in the respective source files for the native
	file system. Hence the following structure is not immediately useful for the
	user. But the explanation for the structure is still provided for
	advanced users who would wish to integrate a new native file system to the
	MPLAB Harmony File System framework.

  Remarks:
    None.

*/

typedef struct
{
    /* Function pointer of native file system for mounting a volume */
    int (*mount) (uint8_t vol);
    /* Function pointer of native file system for unmounting a volume */
    int (*unmount) (uint8_t vol);
    /* Function pointer of native file system for opening a file */
    int (*open) (uintptr_t handle, const char* path, uint8_t mode);
    /* Function pointer of native file system for reading a file */
    int (*read) (uintptr_t fp, void* buff, uint32_t btr, uint32_t *br);
    /* Function pointer of native file system for writing to a file */
    int (*write) (uintptr_t fp, const void* buff, uint32_t btw, uint32_t* bw);
    /* Function pointer of native file system for closing a file */
    int (*close) (uintptr_t fp);
    /* Function pointer of native file system for moving the file pointer by a desired offset */
    int (*seek) (uintptr_t handle, uint32_t offset);
    /* Function pointer of native file system for finding the position of the file pointer */
    uint32_t (*tell) (uintptr_t handle);
    /* Function pointer of native file system to check if the end of file is reached */
    bool (*eof) (uintptr_t handle);
    /* Function pointer of native file system to know the size of file */
    uint32_t (*size) (uintptr_t handle);
    /* Function pointer of native file system to know the status of file */
    int (*fstat) (const char* path, uintptr_t fno);
    /* Function pointer of native file system to create a directory */
    int (*mkdir)(const char *path);
    /* Function pointer of native file system to change a directory */
    int (*chdir)(const char *path);
    /* Function pointer of native file system to remove a file or directory */
    int (*remove)(const char *path);
    /* Function pointer of native file system to get the volume label */
    int (*getlabel)(const char *path, char *buff, uint32_t *sn);
    /* Function pointer of native file system to set the volume label */
    int (*setlabel)(const char *label);
    /* Function pointer of native file system to truncate the file */
    int (*truncate)(uintptr_t handle);
    /* Function pointer of native file system to obtain the current working directory */
    int (*currWD)(char* buff, uint32_t len);
    /* Function pointer of native file system to set the current drive */
    int(*chdrive)(uint8_t drive);
    /* Function pointer of native file system to change the attribute for file or directory */
    int(*chmode)(const char* path, uint8_t attr, uint8_t mask);
    /* Function pointer of native file system to change the time for a file or directory */
    int(*chtime)(const char* path, uintptr_t ptr);
    /* Function pointer of native file system to rename a file or directory */
    int(*rename)(const char *oldPath, const char *newPath);
    /* Function pointer of native file system to flush file */
    int(*sync)(uintptr_t fp);
    /* Function pointer of native file system to read a string from a file */
    char *(*getstrn)(char* buff, int len, uintptr_t handle);
    /* Function pointer of native file system to write a character into a file */
    int(*putchr)(char c, uintptr_t handle);
    /* Function pointer of native file system to write a string into a file */
    int(*putstrn)(const char* str, uintptr_t handle);
    /* Function pointer of native file system to print a formatted string to file */
    int(*formattedprint)(uintptr_t handle, const char *str, ... );
    /* Function pointer of native file system to test an error in a file */
    bool(*testerror)(uintptr_t handle);
    /* Function pointer of native file system to format a disk */
    int(*formatDisk)(uint8_t vol, uint8_t sfd, uint32_t au);
    /* Function pointer of native file system to open a directory */
    int(*openDir)(uintptr_t handle, const char *path);
    /* Function pointer of native file system to read a directory */
    int(*readDir)(uintptr_t handle, uintptr_t stat);
    /* Function pointer of native file system to close an opened directory */
    int(*closeDir)(uintptr_t handle);
    /* Function pointer of native file system to partition a physical drive */
    int(*partitionDisk)(uint8_t pdrv, const uint32_t szt[], void* work);
    /* Function pointer of native file system to get total sectors and free sectors */
    int(*getCluster)(const char *path, uint32_t *tot_sec, uint32_t *free_sec);
}
SYS_FS_FUNCTIONS;

// *****************************************************************************
/* SYS_FS_REGISTRATION_TABLE structure

  Summary:
    The sys_fs layer has to be initialized by passing this structure with
    suitably initialized members.

  Description:
    When the SYS FS layer is initialized, it has to know the type of native file
    system it has to support and the list of functions for native file system.
    The members of this structure can be initialized with suitable values and
    then passed on to SYS_FS_Initialize initialization function. Please refer to
    the example code provided for SYS_FS_Initialize.

  Remarks:
    None.

*/
typedef struct
{
	/* Native file system of type SYS_FS_FILE_SYSTEM_TYPE */
    SYS_FS_FILE_SYSTEM_TYPE nativeFileSystemType;

	/* Pointer to the structure of type SYS_FS_FUNCTIONS which has the list of
	function-pointers for the native file system */
    const SYS_FS_FUNCTIONS    *nativeFileSystemFunctions;

}SYS_FS_REGISTRATION_TABLE;
// *****************************************************************************
/* File System Event Handler function pointer

  Summary:
    Pointer to the File system Handler function.

  Description
    This data type defines the required function signature for the 
    file system event handling callback function. A client must register
    a pointer to an event handling function whose function signature (parameter
    and return value types) match the types specified by this function pointer
    in order to receive event calls back from the file system.
    
  Parameters:
    event           - Identifies the type of event
    
    eventData      -  Handle returned from the media operation requests
    
    context         - Value identifying the context of the application that
                      registered the event handling function

  Returns:
    None.

  Remarks:
    None.
*/
typedef void (* SYS_FS_EVENT_HANDLER)
(
    SYS_FS_EVENT event,
    void* eventData,
    uintptr_t context
);
// *****************************************************************************
/* SYS FS File status structure

  Summary:
    The structure to obtain the status of a file.

  Description:
    This structure holds the various status of a file. The structure is passed
    when SYS_FS_Stat function is called and after a successful execution of the
    function, the members of this structure carries the file status.

  Remarks:
    None.

*/
typedef struct
{
	/* File size */
	uint32_t	fsize;
	/* Last modified date */
	uint16_t	fdate;
	/* Last modified time */
	uint16_t	ftime;
	/* Attribute */
	uint8_t	fattrib;
	/* Short file name (8.3 format) */
	char            fname[13];
#if FAT_FS_USE_LFN
	/* Pointer to the LFN buffer */
	char*	lfname;
	/* Size of LFN buffer in TCHAR */
	uint32_t 	lfsize;
#endif
} SYS_FS_FSTAT;

// *****************************************************************************
/* SYS FS File time structure

  Summary:
    The structure to specify the time for a file or directory.

  Description:
    This structure holds the date and time, which can then be set for a file or
    directory.

    bit31:25
        Year from 1980 (0..127)
    bit24:21
        Month (1..12)
    bit20:16
        Day in month(1..31)
    bit15:11
        Hour (0..23)
    bit10:5
        Minute (0..59)
    bit4:0
        Second / 2 (0..29)

  Remarks:
    None.

*/
typedef union
{
    struct discreteTime
    {
        /* Second / 2 (0..29) */
        unsigned second:    5;
        /* Minute (0..59) */
        unsigned minute:    6;
        /* Hour (0..23) */
        unsigned hour:      5;
        /* Day in month(1..31) */
        unsigned day:       5;
        /* Month (1..12) */
        unsigned month:     4;
        /* Year from 1980 (0..127) */
        unsigned year:      7;  // bit 31-25
    }discreteTime;

    struct  timeDate
    {
        /* Time (hour, min, seconds) */
        uint16_t    time;
        /* Date (year, month, day) */
        uint16_t    date;
    }timeDate;

    /* Combined time information in a 32-bit value */
    uint32_t packedTime;
}SYS_FS_TIME;

// ****************************************************************************
// ****************************************************************************
// Section: File System Abstraction Layer Interface Routines
// ****************************************************************************
// ****************************************************************************

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_Initialize(const void* initData)

    Summary:
        Initializes the File system Abstraction Layer (sys_fs layer).

    Description:
        Initializes the abstraction layer (sys_fs layer) and sets up the necessary parameters.

    Precondition:
        This is the first function to be called during usage of sys_fs.
		Calling other functions from sys_fs without initializing the sys_fs will
		cause unpredictable behavior.

    Parameters:
        initData	-	The pointer to array of the type SYS_FS_REGISTRATION_TABLE,
						but type casted to (const void *).
						The number of elements of array is decided by the definition
						SYS_FS_MAX_FILE_SYSTEM_TYPE. If the application uses 1 file system
						(say only FAT FS), then SYS_FS_MAX_FILE_SYSTEM_TYPE is defined to be 1.
						Otherwise, if the application uses 2 file systems (say FAT FS and MPFS2),
						then SYS_FS_MAX_FILE_SYSTEM_TYPE is defined to be 2. The
						SYS_FS_MAX_FILE_SYSTEM_TYPE has to be defined in system_config.h file.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

	Example:
		<code>
		// This code snippet shows an example of how the
		// SYS FS is initialized

		// Only 1 file system is used

		#define SYS_FS_MAX_FILE_SYSTEM_TYPE			1

		// Functions pointer table for FAT FS

		const SYS_FS_FUNCTIONS FatFsFunctions =
		{
			.mount  = f_mount,
			.unmount = f_unmount,
			.open   = f_open,
			.read   = f_read,
			.write  = f_write,
			.close  = f_close,
			.seek   = f_lseek,
			.tell   = f_tell,
			.eof    = f_eof,
			.size   = f_size,
			.fstat   = f_stat,
		};

		const SYS_FS_REGISTRATION_TABLE sysFSInit [ SYS_FS_MAX_FILE_SYSTEM_TYPE ] =
		{
			{
			.nativeFileSystemType = FAT,
			.nativeFileSystemFunctions = &FatFsFunctions
			}
		};

		SYS_FS_Initialize( (const void *) sysFSInit );

		</code>
***************************************************************************/
SYS_FS_RESULT SYS_FS_Initialize(const void* initData);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_Mount(const char *devName, const char *mountName,
	SYS_FS_FILE_SYSTEM_TYPE filesystemtype, unsigned long mountflags, const void *data);

  Summary:
      Mount file systems

  Description:
      The mount command attaches the file system specified to a volume.

	  When a media (say, SD card or USB thumb drive) is attached to the system, the
	  SYS_FS_Tasks() function needs to run at least 8 times (for each partition),
	  from the SYS_Tasks(), before the volume names are assigned to each partitions.
	  Hence it is mandatory that the SYS_FS_Tasks() is run periodically.
	  Also, it is mandatory that the SYS_FS_Mount() function call from the application code, is
	  not blocking. The application code has to allow the SYS_FS_Tasks() to
	  run periodically while calling the SYS_FS_Mount function. If the SYS_FS_Mount() is called
	  in a blocking mode, then the SYS_Tasks() never gets a chance to run and hence, the media
	  will not be analyzed and finally, the SYS_FS_Mount will never succeed. This will result
	  in a dead-lock.

		There is no mechanism available for the application to know if the specified volume (devName)
		is really attached or not. The only available possibility is to keep trying to mount the volume
		(with the devname), until success is achieved.
		It is prudent that the application code implements a time out mechanism while trying to
		mount a volume (by calling SYS_FS_Mount). The trial for mount should continue at least
		10 times before before assuming that the mount will never succeed. This has to be done
		for every new volume to be mounted.

		The name standard of volume (devName) used in the MPLAB Harmony file system is as follows:

		- NVM			- "nvm" "media number" "volume number"

		- SD card		- "mmcblk" "media number" "volume number"

		- MSD			- "sd" "media number" "volume number"

		Where, "media number" a, b, c... depends on the number of the type of connected media,
		and where, "volume number" 1, 2, 3... depends on the number of partitions in that media.

		The convention for assigning names to volumes is given by an example: -

		If a SD card (with 4 partitions) is attached to the system, and assuming all 4
		partitions are recognized, then there will be 4 devNames -- mmcblka1, mmcblka2, mmcblka3
		and mmcblka4.

		Subsequently, a NVM media is attached which has only 1 partition, the devname
		will be -- nvma1.

		Later, another SD card is attached to the system which has 1 partition, the devname
		will be -- mmcblkb1.

		Finally, there will be 6 volume names (or devNames), which is available for the application
		to be mounted and used for file system.

    Precondition:
        The "devName" name for the volume has to be known.
		The file system type with which each of the volumes are formatted has to be known.
		Trying to mount a volume with a file system which is different from what the volume
		is actually formatted, will cause mounting failure.

    Parameters:
        devName 		- The device name (name of volume) which needs to be mounted. The devName
						  has to be preceded by the string "/dev/".

        mountName 		- Mount name for the device to be mounted. This is a name provided
						  by the user. In future, while accessing the mounted volume (say,
						  during SYS_FS_FileOpen operation), the mountName is used to refer the
						  path for file. The mount name has to be preceded by the string
						  "/mnt/"

        filesystemtype 	- native file system of SYS_FS_FILE_SYSTEM_TYPE type.

        mountflags 		- mounting control flags. This parameter is reserved for future
						  enhancements. Hence, always pass zero.

        data 			- The data argument is interpreted by the different file systems.
					      This parameter is reserved for future enhancements. Hence, always
						  pass NULL.

    Returns:
        If Success		- SYS_FS_RES_SUCCESS

        If Failure		- SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

	Example:
		<code>
			switch(appState)
			{
				case TRY_MOUNT:
					if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive", FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
					{
						// Failure, hence try mounting again
					}
					else
					{
						// Mount was successful. Do further file operations
						appState = DO_FURTHER_STUFFS;

					}
					break;
		</code>
***************************************************************************/
SYS_FS_RESULT SYS_FS_Mount(const char *devName, const char *mountName,
	SYS_FS_FILE_SYSTEM_TYPE filesystemtype, unsigned long mountflags, const void *data);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_Unmount(const char *mountName);


    Summary:
        unmount file systems

    Description:
        remove the attachment of the volume from the file system

    Precondition:
		The volume name has to be know in order to pass as input to Unmount.
        The specified volume name to be unmounted should have been already mounted.

    Parameters:
        mountName 	-	Mount name for the volume to be mounted. The mount name
						has to be preceded by the string "/mnt/".

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

	Example:
		<code>
            if(SYS_FS_Unmount("/mnt/myDrive") != SYS_FS_RES_SUCCESS)
            {
                // Failure, hence try unmounting again
            }
            else
            {
                // Unmount was successful.
            }
		</code>
***************************************************************************/
SYS_FS_RESULT SYS_FS_Unmount(const char *mountName);
// *****************************************************************************
/* Function:
    void SYS_FS_EventHandlerSet
    (
        const void * eventHandler,
        const uintptr_t context
    );

  Summary:
    Allows a client to identify an event handling function for the file system to
    call back when mount/unmount operation has completed.

  Description:
    This function allows a client to identify an event handling function for
    the File System to call back when mount/unmount operation has completed.  
    The file system will pass mount name back to the client by calling 
    "eventHandler".
   
  Precondition:
    The SYS_FS_Initialize() routine must have been called.

  Parameters:
    eventHandler - Pointer to the event handler function implemented by the user
    
    context      - The value of parameter will be passed back to the client 
                   unchanged, when the eventHandler function is called. It can
                   be used to identify any client specific data object that 
                   identifies the instance of the client module (for example, 
                   it may be a pointer to the client module's state structure).

  Returns:
    None.

  Example:
    <code>
    // Client registers an event handler with file system. This is done once.

    SYS_FS_EventHandlerSet(APP_SysFSEventHandler,(uintptr_t)NULL);

    // Event Processing Technique. Event is received when operation is done.

    void APP_SysFSEventHandler(SYS_FS_EVENT event,void* eventData,uintptr_t context)
    {
         switch(event)
        {
            case SYS_FS_EVENT_MOUNT:
                if(0 == strcmp((const char *)mountName,"/mnt/myDrive1"))
                {
                    gSDCardMountFlag = true;
                }
                else if(0 == strcmp((const char *)mountName,"/mnt/myDrive2"))
                {
                    gNVMMountFlag = true;
                }
                break;
            case SYS_FS_EVENT_UNMOUNT:
                if(0 == strcmp((const char *)mountName,"/mnt/myDrive1"))
                {
                    gSDCardMountFlag = false;
                }
                else if(0 == strcmp((const char *)mountName,"/mnt/myDrive2"))
                {
                    gNVMMountFlag = false;
                }
                appData.state = APP_ERROR;
                  break;
            case SYS_FS_EVENT_ERROR:
                break;      
        }
    }
    </code>

  Remarks:
    If the client does not want to be notified when the mount/unmount operation
    has completed, it does not need to register a callback.
*/
void SYS_FS_EventHandlerSet
(
    const void * eventHandler,
    const uintptr_t context
);
//******************************************************************************
/* Function:
	SYS_FS_HANDLE SYS_FS_FileOpen(const char* fname, SYS_FS_FILE_OPEN_ATTRIBUTES attributes);

  Summary:
     Open a file

  Description:
	The SYS_FS_FileOpen opens a requested file in a specific mode (attribute).

  Precondition:
    Prior to opening a file, the name of the volume on which the
	file resides should be known. Also, that volume should be already mounted.
	While opening the file, the name of	the volume is to be passed along with the file name.

  Parameters:
    path      	     			- Path to the file along with the volume name. The string of
								  volume and file name has to be preceded by "/mnt/". Also,
								  the volume name and file name has to be separated by a
								  slash "/".

    attributes					- Access mode of the file, of the type
								  SYS_FS_FILE_OPEN_ATTRIBUTES


  Returns:
	If Success					- Valid handle will be returned

	If Failure 					- Returned handle will be SYS_FS_HANDLE_INVALID

	The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
			SYS_FS_HANDLE fileHandle;

			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle != SYS_FS_HANDLE_INVALID)
			{
				// File open is successful
			}
		</code>

  Remarks:
	None
*/

SYS_FS_HANDLE SYS_FS_FileOpen(const char* fname, SYS_FS_FILE_OPEN_ATTRIBUTES attributes);

//******************************************************************************
/* Function:
	SYS_FS_RESULT SYS_FS_FileClose(SYS_FS_HANDLE handle);

  Summary:
     Close a file.

  Description:
	The SYS_FS_FileClose() function closes an opened file

  Precondition:
    A valid file handle must be obtained before closing a file.

  Parameters:
    handle			- A valid handle, which was obtained while opening the file.

  Returns:
	If Success		- SYS_FS_RES_SUCCESS

	If Failure		- SYS_FS_RES_FAILURE

	The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
			SYS_FS_HANDLE fileHandle;
			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle != SYS_FS_HANDLE_INVALID)
			{
				// File open is successful
			}
			...
			...

			SYS_FS_FileClose(fileHandle);

		</code>

  Remarks:
	None
*/

SYS_FS_RESULT SYS_FS_FileClose(SYS_FS_HANDLE handle);

//******************************************************************************
/* Function:
	size_t SYS_FS_FileRead(SYS_FS_HANDLE handle, void *buf, size_t nbyte)


  Summary:
     Read specified bytes from a file

  Description:
	The SYS_FS_FileRead() function shall attempt to read nbyte bytes from the file associated with
	the file handle into the buffer pointed to by buf.

  Precondition:
    A valid file handle must be obtained before reading a file.

  Parameters:
        handle			- File handle obtained during file open.

		buf				- Pointer to buffer into which data is read.

		nbyte			- Number of bytes to be read


  Returns:
	If Success			- The number of bytes successfully read (0 or positive number)

	If Failure			- (-1)

	The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError


  Example:
		<code>
			...
			char buf[20];
			size_t nbytes;
			size_t bytes_read;
			SYS_FS_HANDLE fd;
			...
			nbytes = sizeof(buf);
			bytes_read = SYS_FS_FileRead(fd, buf, nbytes);
			...
		</code>

  Remarks:
	None
*/

size_t SYS_FS_FileRead(SYS_FS_HANDLE handle, void *buf, size_t nbyte);

//******************************************************************************
/* Function:
	size_t SYS_FS_FileWrite(SYS_FS_HANDLE handle, const void *buf, size_t nbyte)

  Summary:
     Write on the file

  Description:
	The SYS_FS_FileWrite() function shall attempt to write nbyte bytes from the buffer pointed to by buf to
	the file associated with the file handle.

  Precondition:
    A valid file handle must be obtained before writing a file.

  Parameters:
        handle			- File handle obtained during file open.

		buf				- Pointer to buffer from which data is to be written

		nbyte			- Number of bytes to be written


  Returns:
        If Success		- The number of bytes successfully written (0 or positive number)

        If Failure		- (-1)

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
			...
			const char *buf = "Hello World";
			size_t nbytes;
			size_t bytes_written;
			SYS_FS_HANDLE fd;
			...

			bytes_written = SYS_FS_FileWrite(fd, (const void *)buf, nbytes);
			...
		</code>

  Remarks:
	None
*/

size_t SYS_FS_FileWrite(SYS_FS_HANDLE handle, const void *buf, size_t nbyte);

//******************************************************************************
/* Function:
	SYS_FS_RESULT SYS_FS_FileStat(const char* path, SYS_FS_FSTAT *buf)

  Summary:
     Get file status

  Description:
	The SYS_FS_FileStat() function shall obtain information about a file associated
	with the file name, and shall write it to the structure pointed to by buf.

	The buf argument is a pointer to a SYS_FS_FSTAT structure,into which information
	is placed concerning the file.

	This function can read the status of file regardless of whether a file is opened or not.

  Precondition:
    Prior to opening a file, the name of the volume on which the
	file resides should be known. Also, that volume should be already mounted.
	While opening the file, the name of	the volume is to be passed along with the file name.

  Parameters:
    path      	     			- Path to the file along with the volume name. The string of
								  volume and file name has to be preceded by "/mnt/". Also,
								  the volume name and file name has to be separated by a
								  slash "/".

	buf							- pointer to SYS_FS_FSTAT structure.

  Returns:
	If Success					- SYS_FS_RES_SUCCESS

	If Failure					- SYS_FS_RES_FAILURE

	The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
			SYS_FS_fStat fileStat;


			if(SYS_FS_FileStat("/mnt/myDrive/FILE.TXT", &fileStat) == SYS_FS_RES_SUCCESS)
			{
				// Successfully read the status of file "FILE.TXT"
			}


		</code>

  Remarks:
	None
*/

SYS_FS_RESULT SYS_FS_FileStat(const char* fname, SYS_FS_FSTAT *buf);

//******************************************************************************
/* Function:
	int32_t SYS_FS_FileSeek(SYS_FS_HANDLE handle, int32_t offset, SYS_FS_FILE_SEEK_CONTROL whence)

  Summary:
     Move the file pointer.

  Description:
	The SYS_FS_FileSeek() function shall set the file pointer for a open file associated with
	the file handle, as follows:

    If whence is SYS_FS_SEEK_SET, the file offset shall be set to offset bytes from the beginning.

    If whence is SYS_FS_SEEK_CUR, the file offset shall be set to its current location plus offset.

    If whence is SYS_FS_SEEK_END, the file offset shall be set to the size of the file plus offset. If
	SYS_FS_SEEK_END is selected, offset has to be a negative number, for the file pointer
	to be valid.

	Trying to move the file pointer using SYS_FS_FileSeek, beyond the range of file will only cause the
	pointer to be moved to the last location of the file.

  Precondition:
    A valid file handle must be obtained before seeking a file.

  Parameters:
    handle				- A valid file handle obtained during file open.

	offset				- The number of bytes which act as file offset. This value could be a positive
						  or negative value.
	whence				- File seek control input of type SYS_FS_FILE_SEEK_CONTROL.

  Returns:
	If Success			- The number of bytes by which file pointer is moved (0 or positive number)

	If Failure			- (-1) If the chosen offset value was (-1), then the success or failure can be
							ascertained with SYS_FS_Error.

							The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
			SYS_FS_HANDLE fileHandle;
			int status;

			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle != SYS_FS_HANDLE_INVALID)
			{
				// File open is successful
			}
			...
			...

			status = SYS_FS_FileSeek(fileHandle, 5, SYS_FS_SEEK_CUR);

			if((status != -1) && (status == 5))
			{
				// Success
			}

		</code>

  Remarks:
	None
*/

int32_t SYS_FS_FileSeek(SYS_FS_HANDLE fildes, int32_t offset, SYS_FS_FILE_SEEK_CONTROL whence);

//******************************************************************************
/*Function:
    int32_t SYS_FS_FileTell(SYS_FS_HANDLE handle)

    Summary:
        Obtains the file pointer position

    Description:
        Obtains the current value of the file position indicator for the
        file, pointed to by handle.

    Precondition:
        A valid file handle must be obtained before performing a file tell.

    Parameters:
        handle 		- 	File handle obtained during file Open.

    Returns:
        If Success	-	current file position

        If Failure	-	(-1)

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
			SYS_FS_HANDLE fileHandle;
			int32_t tell;

			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle != SYS_FS_HANDLE_INVALID)
			{
				// File open is successful
			}
			...
			...

			tell = SYS_FS_FileTell(fileHandle);

			if(tell != -1)
			{
				// Success
			}

		</code>

  Remarks:
	None

***************************************************************************/
int32_t SYS_FS_FileTell(SYS_FS_HANDLE handle);

//******************************************************************************
/*Function:
    int32_t SYS_FS_FileSize( SYS_FS_HANDLE handle )

    Summary:
    Returns the size of the file

    Description:
        Returns the size of the file as pointed by the handle.

    Precondition:
        A valid file handle must be obtained before knowing a file size.

    Parameters:
        handle 		- File handle obtained during file Open.

    Returns:
        If Success	- file size

        If Failure	- (-1)

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
			SYS_FS_HANDLE fileHandle;
			long fileSize;

			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle != SYS_FS_HANDLE_INVALID)
			{
				// File open is successful
			}
			...
			...

			fileSize = SYS_FS_FileSize(fileHandle);

			if(fileSize != -1)
			{
				// Success
			}

		</code>

  Remarks:
	None
***************************************************************************/
int32_t SYS_FS_FileSize( SYS_FS_HANDLE handle );

//******************************************************************************
/*Function:
    bool SYS_FS_FileEOF(SYS_FS_HANDLE handle)

    Summary:
        check for end of file

    Description:
        Checks whether or not the file position indicator is at the end of
        the file.

    Precondition:
        A valid file handle must be obtained before knowing a file eof.

    Parameters:
        handle 											- file handle obtained during file Open.

    Returns:
		If Success	-

			When file pointer has not reached the end of file	- false

			When file pointer has reached the end of file		- true

		If Failure									- false

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
			SYS_FS_HANDLE fileHandle;
			bool eof;

			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle != SYS_FS_HANDLE_INVALID)
			{
				// File open is successful
			}
			...
			...

			eof = SYS_FS_FileEOF(fileHandle);

			if(eof == false)
			{
				// could be not EOF or a failure
				// Check the error state using SYS_FS_FileError
			}

		</code>

  Remarks:
	None
***************************************************************************/
bool SYS_FS_FileEOF(SYS_FS_HANDLE handle);

//******************************************************************************
/*Function:
    bool SYS_FS_FileNameGet(SYS_FS_HANDLE handle, uint8_t* cName, uint16_t wLen)

    Summary:
        Reads the file name.

    Description:
        Reads the file name of a file that is already open.

    Precondition:
        The file handle referenced by handle is already open.

    Parameters:
        handle 	- file handle obtained during file Open.
 	cName - where to store the name of the file.
	wLen - the maximum length of data to store in cName.

    Returns:
		If Success	-

			The file name was successfully located	- true

        	If Failure
                        The file handle provided is not currently open	- false

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
			SYS_FS_HANDLE fileHandle;
			bool stat;
                        uint8_t fileName[255];

			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle != SYS_FS_HANDLE_INVALID)
			{
				// File open is successful
			}
			...
			...

			stat = SYS_FS_FileNameGet(fileHandle, fileName, 8 );

			if(stat == false)
			{
				// file not located based on handle passed
				// Check the error state using SYS_FS_FileError
			}

		</code>

  Remarks:
	None
***************************************************************************/
bool SYS_FS_FileNameGet(SYS_FS_HANDLE handle, uint8_t* cName, uint16_t wLen);

//******************************************************************************
/*Function:
    SYS_FS_ERROR SYS_FS_Error(void)

    Summary:
        check the type of error

    Description:
        When a file system operation fails, the application can know the exact
        reason of failure by calling the SYS_FS_Error. This function only reports
        the errors which are not file (or file handle) specific. For example, for
        functions such as SYS_FS_Mount and SYS_FS_FileOpen, which do not take handle,
        any errors happening inside such function calls could be reported using
        SYS_FS_Error function. Even for functions, which take handle as its input
        parameters, the SYS_FS_Error function can be used to report the type of
        error for cases where the passed handle itself is invalid.


    Precondition:
        This function has to be called immediately after a failure is observed while doing
        a file operation. Any subsequent failure will overwrite the cause
        of previous failure.

    Parameters:
        none

    Returns:
		Error code of type SYS_FS_ERROR

  Example:
		<code>
			SYS_FS_HANDLE fileHandle;
			SYS_FS_ERROR err;

			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle == SYS_FS_HANDLE_INVALID)
			{
				// If failure, now know the specific reason for failure
				err = SYS_FS_Error();
			}

		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_ERROR SYS_FS_Error(void);

//******************************************************************************
/*Function:
    SYS_FS_ERROR SYS_FS_FileError(SYS_FS_HANDLE handle)

    Summary:
        check the type of file specific error

    Description:
        For file system functions which accepts valid handle, any error happening
        in those functions could be retrieved with SYS_FS_FileError. This function
        returns errors which are file specific.

        Please note that if an invalid handle is passed to a file system function,
        in such a case, SYS_FS_FileError will not return the correct type of error,
        as the handle was invalid. Hence it would be prudent to check the errors
        using the SYS_FS_Error function.

    Precondition:
        This function has to be called immediately after a failure is observed while doing
        a file operation. Any subsequent failure will overwrite the cause
        of previous failure.

    Parameters:
        handle      -   A valid file handle

    Returns:
		Error code of type SYS_FS_ERROR

  Example:
		<code>
 			...
			const char *buf = "Hello World";
			size_t nbytes;
			size_t bytes_written;
			SYS_FS_HANDLE fd;
                        SYS_FS_ERROR err;
			...

			bytes_written = SYS_FS_FileWrite((const void *)buf, nbytes, fd);

                         if(bytes_written == -1)
                         {
                             // error while writing file
                             // find the type (reason) of error
                             err = SYS_FS_FileError(fd);
                         }
		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_ERROR SYS_FS_FileError(SYS_FS_HANDLE handle);

// *****************************************************************************
/* Function:
    void SYS_FS_Tasks ( void )

  Summary:
    Tasks for the sys_fs layer

  Description:
    This routine is used to run the various tasks and functionalities of sys_fs
    layer.

  Precondition:
    The SYS_FS_Initialize routine must have been called before running the tasks.

  Parameters:
    None.

  Returns:
    None

  Example:
    <code>

    void SYS_Tasks ( void )
    {
        SYS_FS_Tasks ();

        // Do other tasks
    }
    </code>

  Remarks:
    This routine is not called directly by an application.  It is
    called by the system's Tasks routine (SYS_Tasks).
*/

void SYS_FS_Tasks ( void );

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_DirectoryMake(const char* path)

    Summary:
        Make directory

    Description:
        Make a new directory as per the specified path.

    Precondition:
        The disk has to be mounted before a directory could be made.

    Parameters:
        path 	- A path for making the directory.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
			SYS_FS_RESULT res;

			res = SYS_FS_DirectoryMake("Dir1");

			if(res == SYS_FS_RES_FAILURE)
			{
				// Directory make failed
			}
			//...
			//...


		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_DirectoryMake(const char* path);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_DirectoryChange(const char* path)

    Summary:
        Change directory

    Description:
        Change the present directory to a new directory.

    Precondition:
        The disk has to be mounted before a directory could be made.
        The directory to be changed has to present.

    Parameters:
        path 	- A path for changing the directory.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
			SYS_FS_RESULT res;

			res = SYS_FS_DirectoryChange("Dir1");

			if(res == SYS_FS_RES_FAILURE)
			{
				// Directory change failed
			}
			//...
			//...


		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_DirectoryChange(const char* path);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FileDirectoryRemove(const char* path)

    Summary:
        Remove a file or directory

    Description:
        Remove a file or directory as specified by the path.

    Precondition:
        - The disk has to be mounted before a directory could be removed.
        - The file or directory to be removed has to present.
        - The file/sub-directory must not have read-only attribute (AM_RDO),
          or the function will be rejected with FR_DENIED.
        - The sub-directory must be empty and must not be current directory,
          or the function will be rejected with FR_DENIED.
        - The file/sub-directory must not be opened.

    Parameters:
        path 	- A path for removing the file or directory.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
			SYS_FS_RESULT res;

			res = SYS_FS_FileDirectoryRemove("Dir1");

			if(res == SYS_FS_RES_FAILURE)
			{
				// Directory make failed
			}
			//...
			//...


		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_FileDirectoryRemove(const char* path);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_CurrentDriveSet(const char* path)

    Summary:
        Set drive

    Description:
        Set the present drive to the one as specified by the path. By default, the
 drive mounted latest becomes the current drive for the system. This is useful for
 applications where only 1 drive (volume) is used. In such an application, there is
 no need to call the SYS_FS_CurrentDriveSet function. But in case of application
 where there are multiple volumes, the user can select the current drive for the
 application by calling this function.

    Precondition:
        The disk has to be mounted before it could be set.


    Parameters:
        path 	- A path for the drive to be set.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
			SYS_FS_RESULT res;

			res = SYS_FS_CurrentDriveSet("/mnt/myDrive");

			if(res == SYS_FS_RES_FAILURE)
			{
				// Drive change failed
			}
			//...
			//...


		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_CurrentDriveSet(const char* path);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_CurrentDriveGet(char* buffer)

    Summary:
        Get current drive

    Description:
        Get the present drive being used and put the name of drive into the buffer.

    Precondition:
        The disk has to be mounted before this function could be set.


    Parameters:
        Pointer to buffer which will hold the name of present drive being used.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
			SYS_FS_RESULT res;
                        char buffer[255];

			res = SYS_FS_CurrentDriveGet(buffer);

			if(res == SYS_FS_RES_FAILURE)
			{
				// Drive change failed
			}
			//...
			//...


		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_CurrentDriveGet(char* buffer);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_DriveLabelGet(const char* drive, char *buff, uint32_t *sn)

    Summary:
        Get drive label

    Description:
        Get the label for the drive specified. If no drive is specified, then the label
        for the current drive is obtained.

    Precondition:
        At least one disk must be mounted before this function can be set.


    Parameters:
        drive           -       Pointer to buffer which will hold the name of
                                drive being for which, the label is requested. If
                                this string is NULL, then then label of the current
                                drive is obtained by using this function.
        buff            -       Buffer which will hold the string of label.
        sn              -       Serial number of the drive. If this information is
                                not needed, it can be set as NULL.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
                    SYS_FS_RESULT res;
                    char buffer[255];
                    uint32_t serialNo;

                    switch(appState)
                    {
                            case TRY_MOUNT:
                                    if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive", FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
                                    {
                                            // Failure, hence try mounting again
                                    }
                                    else
                                    {
                                            // Mount was successful. Get label now
                                            appState = GET_LABEL;

                                    }
                                    break;
                            case GET_LABEL:
                                res = SYS_FS_DriveLabelGet("/mnt/myDrive", buffer, &serialNo);

                                if(res == SYS_FS_RES_FAILURE)
                                {
                                        // Drive label get failed
                                }
                                //...
                                //...
                                break;
		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_DriveLabelGet(const char* drive, char *buff, uint32_t *sn);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_DriveLabelSet(const char* drive, const char *label)

    Summary:
        Set drive label

    Description:
        Set the label for the drive specified. If no drive is specified, then the label
        for the current drive is set.

    Precondition:
        At least one disk must be mounted before this function can be set.


    Parameters:
        drive           -       Pointer to string which holds the name of
                                drive being for which, the label is to be set. If
                                this string is NULL, then then label of the current
                                drive is set by using this function.
        label           -       Pointer to string which contains the label to be set.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
                    SYS_FS_RESULT res;

                    switch(appState)
                    {
                            case TRY_MOUNT:
                                    if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive", FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
                                    {
                                            // Failure, hence try mounting again
                                    }
                                    else
                                    {
                                            // Mount was successful. Get label now
                                            appState = GET_LABEL;

                                    }
                                    break;
                            case GET_LABEL:
                                res = SYS_FS_DriveLabelGet("/mnt/myDrive", "MY_LABEL");

                                if(res == SYS_FS_RES_FAILURE)
                                {
                                        // Drive label get failed
                                }
                                //...
                                //...
                                break;
		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_DriveLabelSet(const char *drive, const char *label);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FileTruncate( SYS_FS_HANDLE handle )

    Summary:
        Truncate a file

    Description:
        The function truncates the file size to the current file read/write pointer.
        This function has no effect if the file read/write pointer is already pointing
        end of the file.

    Precondition:
        A valid handle of a file has to be passed as input to the function.
         The file has to be opened in a mode where write to file is possible
        (such as read plus or write mode).


    Parameters:
        handle           -      A valid handle which was obtained while opening the file.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

 Example:
		<code>
			SYS_FS_HANDLE fileHandle;
 			size_t nbytes;
			size_t bytes_read;
                        SYS_FS_RESULT res;

			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle != SYS_FS_HANDLE_INVALID)
			{
				// File open is successful
			}

                        // Read the file content
                        nbytes = sizeof(buf);
			bytes_read = SYS_FS_FileRead(buf, nbytes, fileHandle);
                        // Truncate the file
                        res = SYS_FS_FileTruncate( fileHandle );
                        if( res != SYS_FS_RES_SUCCESS)
                         {
                            // truncation gone wrong.
                         }

			SYS_FS_FileClose(fileHandle);

		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_FileTruncate( SYS_FS_HANDLE handle );

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_CurrentWorkingDirectoryGet(char *buff, uint32_t len)

    Summary:
    Get the current working directory

    Description:
        Get the current working directory path along with the working drive.

    Precondition:
        At least one disk must be mounted before this function can be set.


    Parameters:
        buff            -       Pointer to a buffer which will contain the name
                                of the current working directory and drive, once
                                the function completes.

        len           -         Size of the buffer.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
                    SYS_FS_RESULT res;
                    char buffer[16];

                    switch(appState)
                    {
                            case TRY_MOUNT:
                                    if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive", FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
                                    {
                                            // Failure, hence try mounting again
                                    }
                                    else
                                    {
                                            // Mount was successful. Create a directory now.
                                            appState = CREATE_DIR;

                                    }
                                    break;
                            case CREATE_DIR:
                                res = SYS_FS_DirectoryMake("Dir1");

                                if(res == SYS_FS_RES_FAILURE)
                                {
                                        // Directory make failed
                                        appState = ERROR;
                                }
                                 else
                                 {
                                    // If successful, then change directory
                                    appState = CHANGE_DIR;
                                 }
                                break;
                            case CHANGE_DIR:
                                res = SYS_FS_DirectoryChange("Dir1");

                                if(res == SYS_FS_RES_FAILURE)
                                {
                                        // Directory change failed
                                        appState = ERROR;
                                }
                                  else
                                 {
                                    // If successful, then get current working directory
                                    appState = GET_CWD;
                                 }
                                break;
                            case GET_CWD:
                                res = SYS_FS_CurrentWorkingDirectoryGet(buffer, 15);
                                 if(res == SYS_FS_RES_FAILURE)
                                {
                                        // Directory change failed
                                        appState = ERROR;
                                }
                                break;
		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_CurrentWorkingDirectoryGet(char *buff, uint32_t len);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FileDirectoryModeSet(const char* path, SYS_FS_FILE_DIR_ATTR attr,
        SYS_FS_FILE_DIR_ATTR mask);

    Summary:
        Mode set for file/ directory.

    Description:
        Sets the mode for a file or directory from the specified list of attributes.

    Precondition:
        At least one disk must be mounted before this function can be set.
        The file/directory for this the mode is set, has to be present.


    Parameters:
        path 	- A path for the file/directory, for which the mode is to
                  be set.

        attr     - Attribute flags to be set in one or more combination of
                   the type SYS_FS_FILE_DIR_ATTR. The specified flags are
                   set and others are cleared.

        mask     - Attribute mask  of type SYS_FS_FILE_DIR_ATTR that specifies
                   which attribute is changed. The specified attributes are set
                   or cleared.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
                    // Set read-only flag, clear archive flag and others are retained.
                    SYS_FS_FileDirectoryModeSet("file.txt", SYS_FS_ATTR_RDO,
                    SYS_FS_ATTR_RDO | SYS_FS_ATTR_ARC);
		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_FileDirectoryModeSet(const char* path, SYS_FS_FILE_DIR_ATTR attr,
        SYS_FS_FILE_DIR_ATTR mask);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FileDirectoryTimeSet(const char* path, SYS_FS_TIME *time)

    Summary:
        Time set for file/directory.

    Description:
        Sets or change the time for a file or directory.

    Precondition:
        At least one disk must be mounted before this function can be set.
        The file/directory for which time has to be set, has to be present.


    Parameters:
        path 	- A path for the file/directory, for which the mode is to
                  be set.

        ptr     - Pointer to structure of type SYS_FS_TIME, which contains the
                  time data already set in.


    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
                     void setTime(void)
                     {
                            SYS_FS_RESULT res;
                            SYS_FS_TIME time;

                            time.packedTime = 0;

                            // All FAT FS times are calculated based on 0 = 1980
                            time.discreteTime.year = (2013 - 1980); // year is 2013
                            time.discreteTime.month = 8;    // August
                            time.discreteTime.day = 9;     // 9th date
                            time.discreteTime.hour = 15;    // 3pm afternoon
                            time.discreteTime.minute = 06;  // 06 min
                            time.discreteTime.second = 00;  // 00 sec


                            res = SYS_FS_FileDirectoryTimeSet("file.txt", &time);
                             if( res != SYS_FS_RES_SUCCESS)
                             {
                                // time change gone wrong
                             }
                        }
		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_FileDirectoryTimeSet(const char* path, SYS_FS_TIME *time);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FileDirectoryRenameMove(const char *oldPath, const char *newPath)

    Summary:
        Rename or move a file/directory.

    Description:
        Renames or moves a file or directory.

    Precondition:
        At least one disk must be mounted before this function can be set.
        The file/directory which has to be renamed/moved, has to be present.

        This function cannot move files/directory from one drive to another.
        Do not rename/move files which are open.


    Parameters:
        oldPath 	- Path for the file/directory, which has to be renamed/moved.

        newPath 	- New Path for the file/directory.


    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
                    SYS_FS_RESULT res;

                    // rename "file.txt" to "renamed_file.txt"
                    res = SYS_FS_FileDirectoryRenameMove("file.txt", "renamed_file.txt");
                     if( res != SYS_FS_RES_SUCCESS)
                     {
                        // renaming has gone wrong
                     }

                     // Now, move "renamed_file.txt" inside directory "Dir1"
                    res = SYS_FS_FileDirectoryRenameMove("renamed_file.txt", "Dir1/renamed_file.txt");
                     if( res != SYS_FS_RES_SUCCESS)
                     {
                        // file move has gone wrong
                     }
		</code>

  Remarks:
        This function cannot move files/ directory from one drive to another.
        Do not rename/ move files which are open.
***************************************************************************/
SYS_FS_RESULT SYS_FS_FileDirectoryRenameMove(const char *oldPath, const char *newPath);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FileSync( SYS_FS_HANDLE handle )

    Summary:
        File flush.

    Description:
        The function flushes the cached information of a writing file.
        The SYS_FS_FileSync() function performs the same process as SYS_FS_FileClose()
        function but the file is left opened and can continue read/write/seek operations
        to the file.

    Precondition:
        At least one disk must be mounted before this function can be set.
        The file which has to be flushed, has to be present and should have been opened
        in write mode.


    Parameters:
        handle         -       Handle for the file received when the file was opened.


    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
                    SYS_FS_RESULT res;
                    SYS_FS_HANDLE fileHandle;
                    const char *buf = "Hello World";
                    size_t nbytes;
                    size_t bytes_written;

                    fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
                                    (SYS_FS_FILE_OPEN_WRITE_PLUS));

                    if(fileHandle != SYS_FS_HANDLE_INVALID)
                    {
                            // File open is successful
                    }

                    // Write data to the file
                    bytes_written = SYS_FS_FileWrite((const void *)buf, nbytes, fileHandle);

                    // Flush the file
                    res = SYS_FS_FileSync(fileHandle);
                     if( res != SYS_FS_RES_SUCCESS)
                     {
                        // renaming has gone wrong
                     }
		</code>

  Remarks:
        None.
***************************************************************************/
SYS_FS_RESULT SYS_FS_FileSync( SYS_FS_HANDLE handle );

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FileStringGet( SYS_FS_HANDLE handle, char* buff, uint32_t len )

    Summary:
        String read.

    Description:
        The function reads a string of specified length from the file into a buffer.
        The read operation continues until a 'n' is stored, reached end of the file
        or the buffer is filled with len - 1 characters. The read string is terminated
        with a '0'.

    Precondition:
        At least one disk must be mounted before this function can be set.
        The file from which a string has to be read, has to be present and should have
        been opened.


    Parameters:
        handle         -       Handle for the file received when the file was opened.
        buff           -       Buffer to read string.
        len            -       length of string to be read.


    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
                    SYS_FS_RESULT res;
                    SYS_FS_HANDLE fileHandle;
                    char buffer[100];

                    fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
                                    (SYS_FS_FILE_OPEN_WRITE_PLUS));

                    if(fileHandle != SYS_FS_HANDLE_INVALID)
                    {
                            // File open is successful
                    }

                    // Flush a string
                    res = SYS_FS_FileStringGet(fileHandle, buffer, 50);
                     if( res != SYS_FS_RES_SUCCESS)
                     {
                        // string reading gone wrong
                     }
		</code>

  Remarks:
        None.
***************************************************************************/
SYS_FS_RESULT SYS_FS_FileStringGet( SYS_FS_HANDLE handle, char* buff, uint32_t len );

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FileCharacterPut( SYS_FS_HANDLE handle, char data )

    Summary:
        Character write.

    Description:
        The function writes a character into a file.
        When the function failed due to disk full or any error, SYS_FS_RES_FAILURE
        will be returned.

    Precondition:
        At least one disk must be mounted before this function can be set.
        The file into which a character has to be written, has to be present and
        should have been opened.


    Parameters:
        handle         -       Handle for the file received when the file was opened.
        data           -       A character to be written to the file.


    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
                    SYS_FS_RESULT res;
                    SYS_FS_HANDLE fileHandle;

                    fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
                                    (SYS_FS_FILE_OPEN_WRITE_PLUS));

                    if(fileHandle != SYS_FS_HANDLE_INVALID)
                    {
                            // File open is successful
                    }

                    // Flush a string
                    res = SYS_FS_FileCharacterPut(fileHandle, 'c');
                     if( res != SYS_FS_RES_SUCCESS)
                     {
                        // character write has gone wrong
                     }
		</code>

  Remarks:
        None.
***************************************************************************/
SYS_FS_RESULT SYS_FS_FileCharacterPut( SYS_FS_HANDLE handle, char data );

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FileStringPut( SYS_FS_HANDLE handle, const char *string )

    Summary:
        String write.

    Description:
        The function writes a string into a file.
        When the function succeeded, it returns SYS_FS_RES_SUCCESS.
        When the write operation is aborted due to disk full or any error a
        SYS_FS_RES_FAILURE is returned.

    Precondition:
        At least one disk must be mounted before this function can be set.
        The file into which a string has to be written, has to be present and
        should have been opened.


    Parameters:
        handle         -       Handle for the file received when the file was opened.
        string         -       Pointer to string which has to be written into file.


    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
                    SYS_FS_RESULT res;
                    SYS_FS_HANDLE fileHandle;

                    fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
                                    (SYS_FS_FILE_OPEN_WRITE_PLUS));

                    if(fileHandle != SYS_FS_HANDLE_INVALID)
                    {
                            // File open is successful
                    }

                    // Flush a string
                    res = SYS_FS_FileStringPut(fileHandle, "Hello World");
                     if( res != SYS_FS_RES_SUCCESS)
                     {
                        // character write has gone wrong
                     }
		</code>

  Remarks:
        None.
***************************************************************************/
SYS_FS_RESULT SYS_FS_FileStringPut( SYS_FS_HANDLE handle, const char *string );

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_FilePrintf( SYS_FS_HANDLE handle, const char *string, ... )

    Summary:
        Formatted string write.

    Description:
        The function writes a formatted string into a file.
        When the function succeeded, it returns SYS_FS_RES_SUCCESS.
        When the write operation is aborted due to disk full or any error a
        SYS_FS_RES_FAILURE is returned.

    Precondition:
        At least one disk must be mounted before this function can be set.
        The file into which a string has to be written, has to be present and
        should have been opened.


    Parameters:
        handle         -       Handle for the file received when the file was opened.
        string         -       Pointer to string which has to be written into file.


    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError

  Example:
		<code>
                    SYS_FS_RESULT res;
                    SYS_FS_HANDLE fileHandle;

                    fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.txt",
                                    (SYS_FS_FILE_OPEN_WRITE_PLUS));

                    if(fileHandle != SYS_FS_HANDLE_INVALID)
                    {
                            // File open is successful
                    }

                    // Write a string
                    res = SYS_FS_FilePrintf(fileHandle, "%d", 1234);
                     if( res != SYS_FS_RES_SUCCESS)
                     {
                        // write has gone wrong
                     }
		</code>

  Remarks:
        None.
***************************************************************************/
SYS_FS_RESULT SYS_FS_FilePrintf( SYS_FS_HANDLE handle, const char *string, ... );

//******************************************************************************
/*Function:
    bool SYS_FS_FileTestError(SYS_FS_HANDLE handle);

    Summary:
        Check for errors in the file

    Description:
        Checks whether or not the file has any errors.

    Precondition:
        A valid file handle must be obtained before passing to the function

    Parameters:
        handle 	- file handle obtained during file Open.

    Returns:
		If Success	-

			When file has an error	- true

			When file has no errors	- false

                        If Failure		- true

		The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError
  Example:
		<code>
			SYS_FS_HANDLE fileHandle;
			bool err;

			fileHandle = SYS_FS_FileOpen("/mnt/myDrive/FILE.JPG",
					(SYS_FS_FILE_OPEN_READ));

			if(fileHandle != SYS_FS_HANDLE_INVALID)
			{
				// File open is successful
			}
			...
			...

			err = SYS_FS_FileTestError(fileHandle);

			if(err == true)
			{
				// either file has error, or there
                                // was an error in working with the "SYS_FS_FileTestError" function
			}

		</code>

  Remarks:
	None
***************************************************************************/
bool SYS_FS_FileTestError(SYS_FS_HANDLE handle);

//******************************************************************************
/*Function:
        SYS_FS_RESULT SYS_FS_DriveFormat(const char* drive, SYS_FS_FORMAT fmt, uint32_t clusterSize)

    Summary:
        Format a drive

    Description:
        Formats a logic drive (create a FAT file system on the logical drive), as per the format specified.

        If the logical drive that has to be formatted has been bound to any partition (1-4) by
        multiple partition feature, the FAT volume is created into the specified partition. In
        this case, the second argument fmt is ignored. The physical drive must have been
        partitioned prior to use this function.

    Precondition:
        At least one disk must be mounted before this function can be set.


    Parameters:
        drive           -       Pointer to buffer which will hold the name of
                                drive being for which, the label is requested. If
                                this string is NULL, then then label of the current
                                drive is obtained by using this function. It is important
                                to end the drive name with a "/".
        fmt             -       Format into which the disk has to be formatted. It could
                                be of the type SYS_FS_FORMAT.
        clusterSize     -       Cluster size. The value must be sector (size * n), where
                                n is 1 to 128 and power of 2. When a zero is given, the
                                cluster size is determined depends on the volume size.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
                    SYS_FS_RESULT res;

                    switch(appState)
                    {
                            case TRY_MOUNT:
                                    if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive", FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
                                    {
                                            // Failure, hence try mounting again
                                    }
                                    else
                                    {
                                            // Mount was successful. Format now.
                                            appState = FORMAT_DRIVE;

                                    }
                                    break;
                            case FORMAT_DRIVE:
                                res = SYS_FS_DriveFormat("/mnt/myDrive", SYS_FS_FORMAT_SFD, 0);

                                if(res == SYS_FS_RES_FAILURE)
                                {
                                        // Drive format went wrong
                                }
                                //...
                                //...
                                break;
		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_DriveFormat(const char* drive, SYS_FS_FORMAT fmt, uint32_t clusterSize);

//******************************************************************************
/* Function:
	SYS_FS_HANDLE SYS_FS_DirOpen(const char* path)

  Summary:
     Open a directory

  Description:
	The SYS_FS_DirOpen opens the requested directory.

  Precondition:
    Prior to opening a file, the name of the volume on which the
	directory resides should be known. Also, that volume should be already mounted.

  Parameters:
    path      	     			- Path to the directory along with the volume name. The string of
                                          volume and directory name has to be preceded by "/mnt/". Also,
                                          the volume name and directory name has to be separated by a
                                          slash "/". If the directory specified is only the root directory,
                                          then, the path has to be ended with "/" (See example below).

  Returns:
	If Success					- Valid handle will be returned

	If Failure 					- Returned handle will be SYS_FS_HANDLE_INVALID

	The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
			SYS_FS_HANDLE dirHandle;

			dirHandle = SYS_FS_DirOpen("/mnt/myDrive/Dir1");
                        // For root directory, end with a "/"
 			// dirHandle = SYS_FS_DirOpen("/mnt/myDrive/");

			if(dirHandle != SYS_FS_HANDLE_INVALID)
			{
				// Directory open is successful
			}
		</code>

  Remarks:
	None
*/

SYS_FS_HANDLE SYS_FS_DirOpen(const char* path);

//******************************************************************************
/* Function:
	SYS_FS_RESULT SYS_FS_DirRead(SYS_FS_HANDLE handle, SYS_FS_FSTAT *stat)


  Summary:
     Read specified directory

  Description:
	The SYS_FS_DirRead() function shall attempt to read the files and directories
        specified in the open directory.

  Precondition:
    A valid directory handle must be obtained before reading a directory.

  Parameters:
        handle			- directory handle obtained during directory open.

        stat                    - Empty structure of type SYS_FS_FSTAT, where the properties
                                  of the open directory will be populated after the SYS_FS_DirRead()
                                  function returns successfully.
                                  If LFN is used, then the "lfname" member of the
                                  SYS_FS_FSTAT structure should be initialized with address of
                                  suitable buffer. Also, the "lfsize" should be initialized with
                                  the size of buffer.
                                  Once the function returns, the buffer whose address is held in "lfname"
                                  will have the file name (long file name)



  Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

	The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError


  Example:
		<code>
			SYS_FS_HANDLE dirHandle;
                        SYS_FS_FSTAT stat;
                        char longFileName[300];
                        uintptr_t  longFileSize;

			dirHandle = SYS_FS_DirOpen("/mnt/myDrive/Dir1");

			if(dirHandle != SYS_FS_HANDLE_INVALID)
			{
				// Directory open is successful
			}

                         // If long file name is used, the following elements of the "stat"
                         // structure needs to be initialized with address of proper buffer.
                         stat.lfname = longFileName;
                         stat.lfsize = 300;

                         if(SYS_FS_DirRead(dirHandle, &stat) == SYS_FS_RES_FAILURE)
                         {
                            // Something went wrong in reading directory
                         }

		</code>

  Remarks:
	None
*/

SYS_FS_RESULT SYS_FS_DirRead(SYS_FS_HANDLE handle, SYS_FS_FSTAT *stat);

//******************************************************************************
/* Function:
	SYS_FS_RESULT SYS_FS_DirRewind(SYS_FS_HANDLE handle)


  Summary:
     Read specified directory

  Description:
	The SYS_FS_DirRewind() function shall attempt to rewind the directory to the
        start. Once a search or directory read is completed, the rewind function is
        used to begin searching the directory from the start.

  Precondition:
    A valid directory handle must be obtained before reading a directory.

  Parameters:
        handle			- directory handle obtained during directory open.

  Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

	The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError


  Example:
		<code>
			SYS_FS_HANDLE dirHandle;
                        SYS_FS_FSTAT stat;
                        char longFileName[300];
                        uintptr_t  longFileSize;

			dirHandle = SYS_FS_DirOpen("/mnt/myDrive/Dir1");

			if(dirHandle != SYS_FS_HANDLE_INVALID)
			{
				// Directory open is successful
			}

                         // If long file name is used, the following elements of the "stat"
                         // structure needs to be initialized with address of proper buffer.
                         stat.lfname = longFileName;
                         stat.lfsize = 300;

                         if(SYS_FS_DirRead(dirHandle, &stat) == SYS_FS_RES_FAILURE)
                         {
                            // Something went wrong in reading directory
                         }

                         // Do more search
                         // Do some more search

                         // Now, rewind the directory to begin search from start

                         if(SYS_FS_DirRewind(dirHandle) == SYS_FS_RES_FAILURE)
                         {
                            // Something went wrong in rewinding directory
                         }

		</code>

  Remarks:
	None
*/

SYS_FS_RESULT SYS_FS_DirRewind(SYS_FS_HANDLE handle);

//******************************************************************************
/* Function:
	SYS_FS_RESULT SYS_FS_DirSearch(SYS_FS_HANDLE handle, const char * name, SYS_FS_FILE_DIR_ATTR attr, SYS_FS_FSTAT *stat)


  Summary:
        Find a file/ directory

  Description:
	Find a file or directory as specified by the name. The type of name is specified
        by attr variable, which if of type SYS_FS_FILE_DIR_ATTR.

  Precondition:
    A valid directory handle must be obtained before reading a directory.

  Parameters:
        handle			- directory handle obtained during directory open.

        name                    - name of file or directory needed to be opened.
                                  The file name can have wild card entries as mentioned below: -
                                - * - Indicates the rest of the filename or extension can vary (e.g. FILE.*)
                                - ? - Indicates that one character in a filename can vary (e.g. F?LE.T?T)

        attr                    - Attribute of the name of type SYS_FS_FILE_DIR_ATTR. This
                                  attribute specifies whether the name is for a file or a
                                  directory. Also, other attribute types could be specified.

        stat                    - Empty structure of type SYS_FS_FSTAT, where the properties
                                  of the open directory will be populated after the SYS_FS_DirRead()
                                  function returns successfully.
                                  If LFN is used, then the "lfname" member of the
                                  SYS_FS_FSTAT structure should be initialized with address of
                                  suitable buffer. Also, the "lfsize" should be initialized with
                                  the size of buffer.
                                  Once the function returns, the buffer whose address is held in "lfname"
                                  will have the file name (long file name)



  Returns:
        If specified file/ directory found	-	SYS_FS_RES_SUCCESS

        If specified file/ directory not found	-	SYS_FS_RES_FAILURE

	The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError


  Example:
		<code>
			SYS_FS_HANDLE dirHandle;
                        SYS_FS_FSTAT stat;
                        char longFileName[300];
                        uintptr_t  longFileSize;

			dirHandle = SYS_FS_DirOpen("/mnt/myDrive/Dir1");

			if(dirHandle != SYS_FS_HANDLE_INVALID)
			{
				// Directory open is successful
			}

                         // If long file name is used, the following elements of the "stat"
                         // structure needs to be initialized with address of proper buffer.
                         stat.lfname = longFileName;
                         stat.lfsize = 300;

                         if(SYS_FS_DirSearch(dirHandle, "FIL*.*", SYS_FS_ATTR_ARC, &stat) == SYS_FS_RES_FAILURE)
                         {
                            // Specified file not found
                         }
                        else
                       {
                           // File found. Read the complete file name from "stat.lfname" and other file parameters
                           // from the "stat" structure
                       }

		</code>

  Remarks:
	None
*/

SYS_FS_RESULT SYS_FS_DirSearch(SYS_FS_HANDLE handle, const char * name, SYS_FS_FILE_DIR_ATTR attr, SYS_FS_FSTAT *stat);


//******************************************************************************
/* Function:
	SYS_FS_RESULT SYS_FS_DirClose(SYS_FS_HANDLE handle)


  Summary:
     Close an opened directory

  Description:
	The SYS_FS_DirClose() function closes a directory which was earlier opened
        with the function SYS_FS_DirOpen(). Internally, this function just clears
        the allocated objects, so that they can be reused.

  Precondition:
    A valid directory handle must be obtained before closing the directory.

  Parameters:
        handle			- directory handle obtained during directory open.

  Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

	The reason for failure could be retrieved with SYS_FS_Error or SYS_FS_FileError


  Example:
		<code>
			SYS_FS_HANDLE dirHandle;

			dirHandle = SYS_FS_DirOpen("/mnt/myDrive/Dir1");

			if(dirHandle != SYS_FS_HANDLE_INVALID)
			{
				// Directory open is successful
			}

                        // .... Do required directory related stuffs

                        // Now, close the directory
                         if(SYS_FS_DirClose(dirHandle) == SYS_FS_RES_FAILURE)
                         {
                            // Something went wrong while closing directory
                         }

		</code>

  Remarks:
	None
*/

SYS_FS_RESULT SYS_FS_DirClose(SYS_FS_HANDLE handle);

// ******************************************************************************
/* Function:
    SYS_FS_RESULT SYS_FS_DrivePartition
    (
        const char *path, 
        const uint32_t partition[], 
        void * work
    )

  Summary:
    Partitions a physical drive (media).

  Description:
    This function partitions a physical drive (media) into requested partition
    sizes. This function will alter the MBR of the physical drive and make it
    into multi partitions.  Windows operating systems do not support multi
    partitioned removable media.

    Maximum 4 partitions can be created on a media.

  Precondition:
    Prior to partitioning the media, the media should have a valid MBR and it
    should be mounted as a volume with the file system.

  Parameters:
    path   	- Path to the volume with the volume name. The string of
              volume name has to be preceded by "/mnt/". Also,
              the volume name and directory name has to be separated by a
              slash "/".
    
    partition - Array with 4 items, where each items mentions the sizes
                of each partition in terms of number of sector. 0th element of
                array specifies the number of sectors for first partition and
                3rd element of array specifies the number of sectors for fourth
                partition.

    work      - Pointer to the buffer for function work area. The size must be
                at least FAT_FS_MAX_SS bytes.

  Returns:
	If Success - SYS_FS_RES_SUCCESS

	If Failure - SYS_FS_RES_FAILURE

	The reason for failure could be retrieved with SYS_FS_Error.

  Example:
    <code>
    //============================================================================
    // Initially, we will consider the case of a SD card which had only 1 partition.
    //============================================================================
    SYS_FS_RESULT res;

    // Following 4 element array specifies the size of 2 partitions as
    // 256MB (=524288 sectors). The 3rd and 4th partition are not created
    // since, the sizes of those are zero.
    uint32_t plist[] = {524288, 524288, 0, 0};

    // Work area for function SYS_FS_DrivePartition
    char work[FAT_FS_MAX_SS];

    switch(appState)
    {
        case TRY_MOUNT:
            if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive", FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
            {
                // Failure, hence try mounting again
            }
            else
            {
                // Mount was successful. Partition now.
                appState = PARTITION_DRIVE;
            }
            break;
            
        case PARTITION_DRIVE:
            res = SYS_FS_DrivePartition("/mnt/myDrive", plist, work);
            if(res == SYS_FS_RES_FAILURE)
            {
                // Drive partition went wrong
            }
            else
            {
                // Partition was successful. Power cycle the board
                // so that all partitions are recognized. Then try mounting
                // both partitions.
            }
            break;

        default:
            break;
    }

    //============================================================================
    // Code below is after the SD card is partitioned and then powered ON.
    //============================================================================
    SYS_FS_RESULT res;

    switch(appState)
    {
        case TRY_MOUNT_1ST_PARTITION:
            if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive1", FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
            {
                // Failure, hence try mounting again
                appState = TRY_MOUNT_1ST_PARTITION;
            }
            else
            {
                // Mount was successful. Mount second partition.
                appState = TRY_MOUNT_2ND_PARTITION;
            }
            break;
   
        case TRY_MOUNT_2ND_PARTITION:
            if(SYS_FS_Mount("/dev/mmcblka2", "/mnt/myDrive2", FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
            {
                // Failure, hence try mounting again
                appState = TRY_MOUNT_2ND_PARTITION;
            }
            else
            {
                // Mount was successful. Try formating first partitions.
                appState = TRY_FORMATING_1ST_PARTITION;
            }
            break;

        case TRY_FORMATING_1ST_PARTITION:
            if(SYS_FS_DriveFormat("/mnt/myDrive1/", SYS_FS_FORMAT_FDISK, 0) == SYS_FS_RES_FAILURE)
            {
                // Failure
            }
            else
            {
                // Try formating second partitions.
                appState = TRY_FORMATING_2ND_PARTITION;
            }
    
        case TRY_FORMATING_2ND_PARTITION:
            if(SYS_FS_DriveFormat("/mnt/myDrive2/", SYS_FS_FORMAT_FDISK, 0) == SYS_FS_RES_FAILURE)
            {
                // Failure
            }
            else
            {
                // Use both partitions as 2 separate volumes.
            }

        default:
            break;
        }
		</code>

  Remarks:
	None
*/

SYS_FS_RESULT SYS_FS_DrivePartition
(
    const char *path, 
    const uint32_t partition[], 
    void *work
);

//******************************************************************************
/*Function:
    SYS_FS_RESULT SYS_FS_DriveSectorGet(const char* path, uint32_t *totalSectors, uint32_t *freeSectors)

    Summary:
        Obtain total and free sectors

    Description:
        Function to obtain the total number of sectors and number of free sectors in a
        drive (media).

    Precondition:
        At least one disk must be mounted before this function can be set.

    Parameters:
        path      	     		- Path to the volume with the volume name. The string of
                                          volume name has to be preceded by "/mnt/". Also,
                                          the volume name and directory name has to be separated by a
                                          slash "/".

        totalSectors                    - Pointer to a variable passed to the
                                          function, which will contain the total
                                          number of sectors available in the
                                          drive (media). This data will be
                                          available only if the function
                                          returns successfully.

        freeSectors                     - Pointer to a variable passed to the
                                          function, which will contain the free
                                          number of sectors available in the
                                          drive (media). This data will be
                                          available only if the function
                                          returns successfully.

    Returns:
        If Success	-	SYS_FS_RES_SUCCESS

        If Failure	-	SYS_FS_RES_FAILURE

		The reason for failure could be retrieved with SYS_FS_Error

  Example:
		<code>
                        uint32_t totalSectors, freeSectors;
                        SYS_FS_RESULT res;

                        if(SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive", FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
                        {
                                // Failure, hence try mounting again
                        }
                        else
                        {
                                // Mount was successful.
                                // Do other FS stuffs.

                        }
                         // Perform usual FS tasks.
                         //....
                         //....
                         //....

                         // Now, determine the total sectors and free sectors
                         res = SYS_FS_DriveSectorGet("/mnt/myDrive", &totalSectors, &freeSectors);
                         if(res == SYS_FS_RES_FAILURE)
                         {
                            // Something went wrong in determining sectors
                         }
		</code>

  Remarks:
	None
***************************************************************************/
SYS_FS_RESULT SYS_FS_DriveSectorGet
(
    const char * path, 
    uint32_t * totalSectors, 
    uint32_t * freeSectors
);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif
