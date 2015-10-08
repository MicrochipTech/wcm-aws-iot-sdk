<#--
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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
 -->
<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
<#assign usbDeviceCDCIndexInit = 0>
<#assign usbDeviceHIDIndex = 0>
<#assign usbDeviceHIDIndexInit = 0>
<#assign usbDeviceMSDIndex = 0>
<#assign usbDeviceMSDIndexInit = 0>
<#assign usbDeviceAudioIndexInit = 0>
<#assign usbDeviceAudio2IndexInit = 0>
<#assign usbDeviceVendorIndexInit = 0>
<#assign usbDeviceConfigOneDescriptorSize = 9>
<#assign usbDeviceCdcConfigDescriptorSize = 58>
<#assign usbDeviceHidConfigDescriptorSize = 32>
<#assign usbDeviceMsdConfigDescriptorSize = 23>
<#assign usbDeviceAudio1ConfigDescriptorSize = 101>
<#assign usbDeviceAudio2ConfigDescriptorSize = 151>
<#assign usbDeviceVendorConfigDescriptorSize = 23>
<#assign usbDeviceConfigOneTotalInterfacesTracking = 0>
<#assign usbDeviceCdcNumberOfInterfaces = 2>
<#assign usbDeviceHidNumberOfInterfaces = 1>
<#assign usbDeviceMsdNumberOfInterfaces = 1>


<#include "/framework/usb/templates/usb_device_descriptor_hid_report.c.ftl">
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>

/*******************************************
 * MSD Function Driver initialization
 *******************************************/
USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if>
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer${usbDeviceMSDIndex} [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>
/*******************************************
 * MSD Function Driver initialization
 *******************************************/

USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if> 
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer${usbDeviceMSDIndex} [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>
/*******************************************
 * MSD Function Driver initialization
 *******************************************/

USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if> 
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer${usbDeviceMSDIndex} [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>
/*******************************************
 * MSD Function Driver initialization
 *******************************************/

USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if>
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer${usbDeviceMSDIndex} [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>
/*******************************************
 * MSD Function Driver initialization
 *******************************************/

USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if>
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>
/*******************************************
 * MSD Function Driver initialization
 *******************************************/

USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if>
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>
/*******************************************
 * MSD Function Driver initialization
 *******************************************/

USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if>
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>
/*******************************************
 * MSD Function Driver initialization
 *******************************************/

USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if>
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>
/*******************************************
 * MSD Function Driver initialization
 *******************************************/

USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if>
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_MSD_IDX0 == true >
<#if CONFIG_PIC32MZ == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer${usbDeviceMSDIndex}[512] __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));
USB_MSD_CSW msdCSW${usbDeviceMSDIndex} __attribute__((coherent)) __attribute__((aligned(4)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer [DRV_NVM_ROW_SIZE];
<#elseif CONFIG_PIC32MX == true>

/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer[512];

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${usbDeviceMSDIndex};
USB_MSD_CSW msdCSW${usbDeviceMSDIndex};

</#if>
/*******************************************
 * MSD Function Driver initialization
 *******************************************/

USB_DEVICE_MSD_MEDIA_INIT_DATA msdMediaInit${usbDeviceMSDIndex}[1] =
{
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_msd_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_msd_basic_demo" || CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_serial_emulator_msd_demo">
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        (void *)diskImage,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    }
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "msd_basic_sdcard_demo">
        {
        DRV_SDCARD_INDEX_0,
        DRV_SDCARD_BLOCK_SIZE,
        sectorBuffer,
        <#if CONFIG_PIC32MX == true>
        NULL,
        <#elseif CONFIG_PIC32MZ == true>
        flashRowBackupBuffer,
        </#if>
        0,
        {
            0x00,	// peripheral device is connected, direct access block device
            0x80,   // removable
            0x04,	// version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,	// response is in format specified by SPC-2
            0x20,	// n-4 = 36-4=32= 0x20
            0x00,	// sccs etc.
            0x00,	// bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                    // but as in case of other device, we are just using 00
            0x00,	// 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_SDCARD_IsAttached,
            DRV_SDCARD_Open,
            DRV_SDCARD_Close,
            DRV_SDCARD_GeometryGet,
            DRV_SDCARD_Read,
            DRV_SDCARD_Write,
            DRV_SDCARD_IsWriteProtected,
            DRV_SDCARD_EventHandlerSet,
            NULL /* DRV_NVM_StartAddressSet */
        }
    } 
    <#else>
	//TODO: Add MSD Media Initialize structure here. 
    </#if>
};
<#assign usbDeviceMSDIndex = usbDeviceMSDIndex + 1>
</#if>
/**************************************************
 * USB Device Function Driver Init Data
 **************************************************/
<#if CONFIG_USB_DEVICE_FUNCTION_1_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_1_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_1_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_1_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_1_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_1_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_1_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_2_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_2_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_2_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_2_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_2_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_2_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_3_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_3_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_3_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_3_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_3_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_3_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_4_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_4_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_4_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_4_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_4_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_4_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_5_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_5_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_5_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_5_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_5_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_5_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_6_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_6_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_6_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_6_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_6_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_6_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_7_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_7_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_7_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_7_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_7_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_7_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_8_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_8_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_8_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_8_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_8_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_8_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_9_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_9_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_9_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_9_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_9_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_9_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_IDX0 == true>
    <#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_CDC_IDX0 == true>
    const USB_DEVICE_CDC_INIT cdcInit${usbDeviceCDCIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_10_CDC_READ_Q_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_10_CDC_WRITE_Q_SIZE_IDX0},
        .queueSizeSerialStateNotification = ${CONFIG_USB_DEVICE_FUNCTION_10_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0}
    };
    <#assign usbDeviceCDCIndexInit = usbDeviceCDCIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX0 == true >
    const USB_DEVICE_HID_INIT hidInit${usbDeviceHIDIndexInit} =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt${usbDeviceHIDIndexInit}),
        .hidReportDescriptor = &hid_rpt${usbDeviceHIDIndexInit},
        .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_10_HID_READ_Q_SIZE_IDX0},
        .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_10_HID_WRITE_Q_SIZE_IDX0}
    };
    <#assign usbDeviceHIDIndexInit = usbDeviceHIDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_MSD_IDX0 == true >
    const USB_DEVICE_MSD_INIT msdInit${usbDeviceMSDIndexInit} =
    {
        .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_10_MSD_LUN_IDX0},
        .msdCBW = &msdCBW${usbDeviceMSDIndexInit},
        .msdCSW = &msdCSW${usbDeviceMSDIndexInit},
        .mediaInit = &msdMediaInit${usbDeviceMSDIndexInit}[0]
    };
    <#assign usbDeviceMSDIndexInit = usbDeviceMSDIndexInit + 1>
    <#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX0 == true >
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
	<#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
    const USB_DEVICE_AUDIO_V2_INIT audioInit${usbDeviceAudio2IndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudio2IndexInit = usbDeviceAudio2IndexInit + 1>
    </#if>
</#if>
/**************************************************
 * USB Device Layer Function Driver Registration 
 * Table
 **************************************************/
const USB_DEVICE_FUNCTION_REGISTRATION_TABLE funcRegistrationTable[${CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0}] =
{
    <#assign usbDeviceCDCInstancesTracking = 0>
    <#assign usbDeviceHIDInstancesTracking = 0>
    <#assign usbDeviceMSDInstancesTracking = 0>
    <#assign usbDeviceAudioInstancesTracking = 0>
	<#assign usbDeviceAudio2InstancesTracking = 0>
    <#assign usbDeviceVendorInstancesTracking = 0>
    <#if CONFIG_USB_DEVICE_FUNCTION_1_IDX0 == true>
    /* Function 1 */
    { 
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX0},    /* Configuration value */ 
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_1_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */ 
		<#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_1_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_1_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},    /* Function driver init data*/
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
         <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
         <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_1_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_1_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_1_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_1_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_1_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_1_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if>	 
    <#if CONFIG_USB_DEVICE_FUNCTION_2_IDX0 == true>
    /* Function 2 */
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_2_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_2_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_2_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_2_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_2_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */  
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_2_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_2_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_2_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_2_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_2_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_2_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_2_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_2_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},   /* Function driver init data */
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_2_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_2_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_2_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
         <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
         <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if>
    <#if CONFIG_USB_DEVICE_FUNCTION_3_IDX0 == true>
    /* Function 3 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_3_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_3_SPEED_HS_IDX0},    /* Function Speed */   
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_3_SPEED_FS_IDX0},    /* Function Speed */
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_3_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_3_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_3_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_3_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_3_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_3_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_3_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_3_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_3_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_3_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_3_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_3_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if>
    <#if CONFIG_USB_DEVICE_FUNCTION_4_IDX0 == true>
    /* Function 4 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_4_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_4_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_4_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_4_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_4_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_4_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_4_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_4_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_4_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_4_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_4_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_4_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_4_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},   /* Function driver init data */
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_4_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_4_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_4_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if>
    <#if CONFIG_USB_DEVICE_FUNCTION_5_IDX0 == true>
    /* Function 5 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_5_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_5_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_5_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_5_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_5_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_5_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_5_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_5_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_5_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_5_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_5_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_5_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_5_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_5_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_5_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_5_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if>  
	<#if CONFIG_USB_DEVICE_FUNCTION_6_IDX0 == true>
    /* Function 6 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_6_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_6_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_6_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_6_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_6_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_6_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_6_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_6_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_6_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_6_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_6_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_6_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_6_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_6_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_6_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_6_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if> 
	<#if CONFIG_USB_DEVICE_FUNCTION_7_IDX0 == true>
    /* Function 7 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_7_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_7_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_7_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_7_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_7_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_7_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_7_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_7_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_7_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_7_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_7_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_7_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_7_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_7_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_7_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_7_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if> 
	<#if CONFIG_USB_DEVICE_FUNCTION_8_IDX0 == true>
    /* Function 8 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_8_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_8_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_8_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_8_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_8_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_8_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_8_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_8_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_8_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if>   
	<#if CONFIG_USB_DEVICE_FUNCTION_9_IDX0 == true>
    /* Function 9 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_9_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_9_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_9_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_9_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_9_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_9_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_9_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_9_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_9_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_9_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_9_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_9_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_9_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_9_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_9_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_9_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if> 
	<#if CONFIG_USB_DEVICE_FUNCTION_10_IDX0 == true>
    /* Function 10 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_10_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_10_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_10_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_10_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_CDC_IDX0 == true>
        .numberOfInterfaces = ${usbDeviceCdcNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_10_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceCdcNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
            || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
            <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + 8>
        </#if>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceCdcConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceHidNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_10_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceHidNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceHidConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_MSD_IDX0 == true >
        .numberOfInterfaces = ${usbDeviceMsdNumberOfInterfaces},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_10_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + usbDeviceMsdNumberOfInterfaces>
        </#if>
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceMsdConfigDescriptorSize>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_10_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_10_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_10_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio1ConfigDescriptorSize>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_10_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_10_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_10_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceAudio2InstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_V2_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudio2InstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudio2InstancesTracking = usbDeviceAudio2InstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceAudio2ConfigDescriptorSize>
        <#else>
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_10_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_USB_DEVICE_FUNCTION_10_CONFIGURATION_IDX0?number == 1>
        <#assign usbDeviceConfigOneTotalInterfacesTracking = usbDeviceConfigOneTotalInterfacesTracking + CONFIG_USB_DEVICE_FUNCTION_10_NUMBER_OF_INTERFACES_IDX0?number>
        </#if>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        <#assign usbDeviceConfigOneDescriptorSize = usbDeviceConfigOneDescriptorSize + usbDeviceVendorConfigDescriptorSize>
        </#if>
    },
    </#if>   
};

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/
/*******************************************
 *  USB Device Descriptor 
 *******************************************/
const USB_DEVICE_DESCRIPTOR deviceDescriptor =
{
    0x12,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,          // DEVICE descriptor type
    0x0200,                         // USB Spec Release Number in BCD format
 <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_com_port_single_demo">
    USB_CDC_CLASS_CODE,         // Class Code
    USB_CDC_SUBCLASS_CODE,      // Subclass code
    0x00,                       // Protocol code
 <#elseif CONFIG_USB_DEVICE_USE_CDC == true>
    0xEF,                           // Class Code
    0x02,                           // Subclass code
    0x01,                           // Protocol code
<#elseif CONFIG_USB_DEVICE_USE_AUDIO_2_0 == true>
	0xEF,                           // Class Code
    0x02,                           // Subclass code
    0x01,                           // Protocol code	
<#else>
    0x00,                           // Class Code
    0x00,                           // Subclass code
    0x00,                           // Protocol code
</#if>
    USB_DEVICE_EP0_BUFFER_SIZE,     // Max packet size for EP0, see system_config.h
    ${CONFIG_USB_DEVICE_VENDOR_ID_IDX0},                         // Vendor ID
    ${CONFIG_USB_DEVICE_PRODUCT_ID_IDX0},                         // Product ID
<#if CONFIG_USB_DEVICE_USE_AUDIO_2_0 == true>	
	0x0200,
<#else>	
    0x0100,                         // Device release number in BCD format
</#if>	
    0x01,                           // Manufacturer string index
    0x02,                           // Product string index
    0x00,                           // Device serial number string index
    0x01                            // Number of possible configurations
};

<#assign usbDeviceConfigOneDescriptorSizeUpperByte = usbDeviceConfigOneDescriptorSize / 256>
<#if CONFIG_PIC32MZ == true>
<#if CONFIG_USB_DEVICE_SPEED_HS_IDX0 == "USB_SPEED_HIGH">
/*******************************************
 *  USB Device Qualifier Descriptor for this
 *  demo.
 *******************************************/
const USB_DEVICE_QUALIFIER deviceQualifierDescriptor1 =
{
    0x0A,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE_QUALIFIER,    // Device Qualifier Type
    0x0200,                             // USB Specification Release number
 <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "cdc_com_port_single_demo">
    USB_CDC_CLASS_CODE,         // Class Code
    USB_CDC_SUBCLASS_CODE,      // Subclass code
    0x00,                       // Protocol code
 <#elseif CONFIG_USB_DEVICE_USE_CDC == true>
    0xEF,                           // Class Code
    0x02,                           // Subclass code
    0x01,                           // Protocol code
<#elseif CONFIG_USB_DEVICE_USE_AUDIO_2_0 == true>
	0xEF,                           // Class Code
    0x02,                           // Subclass code
    0x01,                           // Protocol code	
<#else>
    0x00,                           // Class Code
    0x00,                           // Subclass code
    0x00,                           // Protocol code
</#if>
    USB_DEVICE_EP0_BUFFER_SIZE,         // Maximum packet size for endpoint 0
<#if CONFIG_USB_DEVICE_USE_AUDIO_2_0 == true>
	0x00,
<#else>	
    0x01,                               // Number of possible configurations
</#if>	
    0x00                                // Reserved for future use.
};

/*******************************************
 *  USB High Speed Configuration Descriptor
 *******************************************/
 
const uint8_t highSpeedConfigurationDescriptor[]=
{
    /* Configuration Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    ${usbDeviceConfigOneDescriptorSize % 256},${usbDeviceConfigOneDescriptorSizeUpperByte?floor},                //(${usbDeviceConfigOneDescriptorSize?number} Bytes)Size of the Config descriptor.e
    ${usbDeviceConfigOneTotalInterfacesTracking},                                               // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,                                                 // Max power consumption (2X mA)
    
<#assign usbDeviceHIDReportCount = 0>
<#assign usbDeviceSpeed = 1>
<#include "/framework/usb/templates/usb_device_descriptor_function_1.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_2.c.ftl">
<#include "/framework/usb/templates/usb_device_descriptor_function_3.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_4.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_5.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_6.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_7.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_8.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_9.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_10.c.ftl">   
};

/*******************************************
 * Array of High speed config descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE highSpeedConfigDescSet[1] =
{
    highSpeedConfigurationDescriptor
};
</#if>
</#if>

/*******************************************
 *  USB Full Speed Configuration Descriptor
 *******************************************/
const uint8_t fullSpeedConfigurationDescriptor[]=
{
    /* Configuration Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    ${usbDeviceConfigOneDescriptorSize % 256},${usbDeviceConfigOneDescriptorSizeUpperByte?floor},                //(${usbDeviceConfigOneDescriptorSize?number} Bytes)Size of the Config descriptor.e
    ${usbDeviceConfigOneTotalInterfacesTracking},                                               // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,                                                 // Max power consumption (2X mA)
<#assign usbDeviceHIDReportCount = 0>
<#assign usbDeviceSpeed = 2>
<#include "/framework/usb/templates/usb_device_descriptor_function_1.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_2.c.ftl">
<#include "/framework/usb/templates/usb_device_descriptor_function_3.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_4.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_5.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_6.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_7.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_8.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_9.c.ftl"> 
<#include "/framework/usb/templates/usb_device_descriptor_function_10.c.ftl">   

};

/*******************************************
 * Array of Full speed config descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE fullSpeedConfigDescSet[1] =
{
    fullSpeedConfigurationDescriptor
};


/**************************************
 *  String descriptors.
 *************************************/

 /*******************************************
 *  Language code string descriptor
 *******************************************/
 <#if CONFIG_USB_DEVICE_STRING_DESCRIPTOR_TABLE_ADVANCED == true >
    const struct __attribute__ ((packed))
    {
        uint8_t stringIndex;    //Index of the string descriptor
        uint16_t languageID ;   // Language ID of this string.
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type 
        uint16_t string[1];     // String
    }
    sd000 =
    {
        0, // Index of this string is 0
        0, // This field is always blank for String Index 0
        sizeof(sd000)-sizeof(sd000.stringIndex)-sizeof(sd000.languageID),
        USB_DESCRIPTOR_STRING,
        {0x0409}                // Language ID
    };  
<#else>
    const struct
    {
        uint8_t bLength;
        uint8_t bDscType;
        uint16_t string[1];
    }
    sd000 =
    {
        sizeof(sd000),          // Size of this descriptor in bytes
        USB_DESCRIPTOR_STRING,  // STRING descriptor type
        {0x0409}                // Language ID
    };
</#if>
/*******************************************
 *  Manufacturer string descriptor
 *******************************************/
 <#if CONFIG_USB_DEVICE_STRING_DESCRIPTOR_TABLE_ADVANCED == true >
    const struct __attribute__ ((packed))
    {
        uint8_t stringIndex;    //Index of the string descriptor
        uint16_t languageID ;    // Language ID of this string.
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[${CONFIG_USB_DEVICE_MANUFACTURER_STRING_IDX0?length}];    // String
    }
    sd001 =
    {
        1,      // Index of this string descriptor is 1. 
        0x0409, // Language ID of this string descriptor is 0x0409 (English)
        sizeof(sd001)-sizeof(sd001.stringIndex)-sizeof(sd001.languageID),
        USB_DESCRIPTOR_STRING,
        {<#list 1..CONFIG_USB_DEVICE_MANUFACTURER_STRING_IDX0?length as index>'${CONFIG_USB_DEVICE_MANUFACTURER_STRING_IDX0?substring(index-1,index)}'<#if index_has_next>,</#if></#list>}
    };
<#else>
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[${CONFIG_USB_DEVICE_MANUFACTURER_STRING_IDX0?length}];    // String
    }
    sd001 =
    {
        sizeof(sd001),
        USB_DESCRIPTOR_STRING,
        {<#list 1..CONFIG_USB_DEVICE_MANUFACTURER_STRING_IDX0?length as index>'${CONFIG_USB_DEVICE_MANUFACTURER_STRING_IDX0?substring(index-1,index)}'<#if index_has_next>,</#if></#list>}
    };
</#if>

/*******************************************
 *  Product string descriptor
 *******************************************/
<#if CONFIG_USB_DEVICE_STRING_DESCRIPTOR_TABLE_ADVANCED == true >
    const struct __attribute__ ((packed))
    {
        uint8_t stringIndex;    //Index of the string descriptor
        uint16_t languageID ;   // Language ID of this string.
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type 
        uint16_t string[${CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR_IDX0?length}];    // String
    }
    sd002 =
    {
        2,       // Index of this string descriptor is 2. 
        0x0409,  // Language ID of this string descriptor is 0x0409 (English)
        sizeof(sd002)-sizeof(sd002.stringIndex)-sizeof(sd002.languageID),
        USB_DESCRIPTOR_STRING,
        {<#list 1..CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR_IDX0?length as index>'${CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR_IDX0?substring(index-1,index)}'<#if index_has_next>,</#if></#list>} 
    }; 
<#else>
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[${CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR_IDX0?length}];    // String
    }
    sd002 =
    {
        sizeof(sd002),
        USB_DESCRIPTOR_STRING,
		{<#list 1..CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR_IDX0?length as index>'${CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR_IDX0?substring(index-1,index)}'<#if index_has_next>,</#if></#list>}
    }; 
</#if>

/***************************************
 * Array of string descriptors
 ***************************************/
USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[3]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002
};


/*******************************************
 * USB Device Layer Master Descriptor Table 
 *******************************************/
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &deviceDescriptor,          /* Full speed descriptor */
    1,                          /* Total number of full speed configurations available */
    fullSpeedConfigDescSet,     /* Pointer to array of full speed configurations descriptors*/
<#if CONFIG_PIC32MZ == true && CONFIG_USB_DEVICE_SPEED_HS_IDX0 == "USB_SPEED_HIGH">
    &deviceDescriptor,          /* High speed device descriptor*/
    1,                          /* Total number of high speed configurations available */
    highSpeedConfigDescSet,     /* Pointer to array of high speed configurations descriptors. */
<#else>
    NULL, 
    0, 
    NULL, 
</#if>
    3,                          // Total number of string descriptors available.
    stringDescriptors,          // Pointer to array of string descriptors.
<#if CONFIG_PIC32MZ == true && CONFIG_USB_DEVICE_SPEED_HS_IDX0 == "USB_SPEED_HIGH">
    &deviceQualifierDescriptor1,// Pointer to full speed dev qualifier.
    &deviceQualifierDescriptor1 // Pointer to high speed dev qualifier.
<#else>
    NULL, 
    NULL
</#if>   
};


/****************************************************
 * USB Device Layer Initialization Data
 ****************************************************/
<#-- Instance 0 -->
<#if CONFIG_USB_DEVICE_INST_IDX0 == true>
const USB_DEVICE_INIT usbDevInitData =
{
<#if CONFIG_USB_DEVICE_POWER_STATE_IDX0?has_content>
    /* System module initialization */
    .moduleInit = {${CONFIG_USB_DEVICE_POWER_STATE_IDX0}},
    
</#if>
    /* Number of function drivers registered to this instance of the
       USB device layer */
<#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?has_content>
    .registeredFuncCount = ${CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0},
    
</#if>
    /* Function driver table registered to this instance of the USB device layer*/
    .registeredFunctions = (USB_DEVICE_FUNCTION_REGISTRATION_TABLE*)funcRegistrationTable,

    /* Pointer to USB Descriptor structure */
    .usbMasterDescriptor = (USB_DEVICE_MASTER_DESCRIPTOR*)&usbMasterDescriptor,

    /* USB Device Speed */
<#if CONFIG_PIC32MX == true >
<#if CONFIG_USB_DEVICE_SPEED_FS_IDX0?has_content>
    .deviceSpeed = ${CONFIG_USB_DEVICE_SPEED_FS_IDX0},
    
</#if>
<#elseif CONFIG_PIC32MZ == true >
<#if CONFIG_USB_DEVICE_SPEED_HS_IDX0?has_content>
    .deviceSpeed = ${CONFIG_USB_DEVICE_SPEED_HS_IDX0},
    
</#if>
</#if>
<#if CONFIG_PIC32MX == true >
    /* Index of the USB Driver to be used by this Device Layer Instance */
    .driverIndex = DRV_USBFS_INDEX_0,

    /* Pointer to the USB Driver Functions. */
    .usbDriverInterface = DRV_USBFS_DEVICE_INTERFACE,
    
<#elseif CONFIG_PIC32MZ == true >
    /* Index of the USB Driver to be used by this Device Layer Instance */
    .driverIndex = DRV_USBHS_INDEX_0,

    /* Pointer to the USB Driver Functions. */
    .usbDriverInterface = DRV_USBHS_DEVICE_INTERFACE,
    
</#if>
<#if CONFIG_USB_DEVICE_ENDPOINT_QUEUE_SIZE_WRITE_IDX0?has_content>
    /* Specify queue size for vendor endpoint read */
    .queueSizeEndpointRead = ${CONFIG_USB_DEVICE_ENDPOINT_QUEUE_SIZE_READ_IDX0},
    
</#if>
<#if CONFIG_USB_DEVICE_ENDPOINT_QUEUE_SIZE_READ_IDX0?has_content>
    /* Specify queue size for vendor endpoint write */
    .queueSizeEndpointWrite= ${CONFIG_USB_DEVICE_ENDPOINT_QUEUE_SIZE_WRITE_IDX0},
</#if>
};
</#if>
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
