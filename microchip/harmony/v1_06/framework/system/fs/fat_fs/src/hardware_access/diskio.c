/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "system/fs/fat_fs/src/hardware_access/diskio.h"		/* FatFs lower layer API */

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/* This function is left unchanged from what was implemted in FAT FS. This
 is done to maintain compatibility with future versions of FAT FS. Also,
 no task is done in this function. It is not required as Harmony File system
 has its own media manager layer, which takes care of disk_initialization */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
        uint8_t pdrv				/* Physical drive nmuber (0..) */
        )
{


    switch(pdrv)
    {
        case 0:
            {

                break;
            }
        case 1:
            {


                break;
            }
    }
    return 0;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/* This function is left unchanged from what was implemted in FAT FS. This
 is done to maintain compatibility with future versions of FAT FS. Also,
 no task is done in this function. It is not required as Harmony File system
 has its own media manager layer, which takes care of disk_status        */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
        uint8_t pdrv		/* Physical drive nmuber (0..) */
        )
{


    return 0;

}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/* This function is left unchanged from what was implemted in FAT FS. This
 is done to maintain compatibility with future versions of FAT FS. Also,
 no task is done in this function. This function will be implemented in
 future                                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
        uint8_t pdrv,		/* Physical drive nmuber (0..) */
        uint8_t cmd,		/* Control code */
        void *buff		/* Buffer to send/receive control data */
        )
{

    return RES_OK;
}
#endif

/****************************************************************************
 The get_fattime function is used to know the present time, which is used by
 FAT FS code. The present time should ideally be updated by a Real time clock
 hardware module. Since this module is not integrated with Harmony FS framework,
 a fixed time is set as given by the implementation of the function below   */
uint32_t get_fattime(void)
{
    /* RTC should return time here */
    /* For now, just a value */
    SYS_FS_TIME time;
    time.packedTime = 0;

    // All FAT FS times are calculated based on 0 = 1980
    time.discreteTime.year = (2013 - 1980); // year is 2013
    time.discreteTime.month = 8;    // August
    time.discreteTime.day = 9;     // 9th date
    time.discreteTime.hour = 15;    // 3pm afternoon
    time.discreteTime.minute = 06;  // 06 min
    time.discreteTime.second = 00;  // 00 sec

    return (time.packedTime);
}



