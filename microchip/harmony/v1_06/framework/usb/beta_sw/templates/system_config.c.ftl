<#assign usbDeviceCDCIndexInit = 0>
<#assign usbDeviceHIDIndex = 0>
<#assign usbDeviceHIDIndexInit = 0>
<#assign usbDeviceMSDIndex = 0>
<#assign usbDeviceMSDIndexInit = 0>
<#assign usbDeviceAudioIndexInit = 0>
<#assign usbDeviceVendorIndexInit = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	// TODO : Add HID Report Descriptor here 
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
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
	//TODO: Add MSD Media Initialize structure here. 
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
	//TODO: Add MSD Media Initialize structure here. 
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
	//TODO: Add MSD Media Initialize structure here. 
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
	//TODO: Add MSD Media Initialize structure here. 
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
	//TODO: Add MSD Media Initialize structure here. 
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
	//TODO: Add MSD Media Initialize structure here. 
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
	//TODO: Add MSD Media Initialize structure here. 
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
	//TODO: Add MSD Media Initialize structure here. 
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
	//TODO: Add MSD Media Initialize structure here. 
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
	//TODO: Add MSD Media Initialize structure here. 
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    const USB_DEVICE_AUDIO_INIT audioInit${usbDeviceAudioIndexInit} =
    {
        .queueSizeRead = ${CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_READ_QUEUE_SIZE_IDX0},
        .queueSizeWrite = ${CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0}
    };
    <#assign usbDeviceAudioIndexInit = usbDeviceAudioIndexInit + 1>
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
    <#assign usbDeviceVendorInstancesTracking = 0>
    <#if CONFIG_USB_DEVICE_FUNCTION_1_IDX0 == true>
    /* Function 1 */
    { 
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX0},    /* Configuration value */ 
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_1_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */ 
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_1_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
		<#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_1_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_1_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},    /* Function driver init data*/
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
         <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if>	 
    <#if CONFIG_USB_DEVICE_FUNCTION_2_IDX0 == true>
    /* Function 2 */
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_2_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_2_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_2_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_2_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_2_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */  
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
         <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if>
    <#if CONFIG_USB_DEVICE_FUNCTION_3_IDX0 == true>
    /* Function 3 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_3_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_3_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_3_SPEED_HS_IDX0},    /* Function Speed */   
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_3_SPEED_FS_IDX0},    /* Function Speed */
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if>
    <#if CONFIG_USB_DEVICE_FUNCTION_4_IDX0 == true>
    /* Function 4 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_4_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_4_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_4_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_4_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_4_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if>
    <#if CONFIG_USB_DEVICE_FUNCTION_5_IDX0 == true>
    /* Function 5 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_5_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_5_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_5_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_5_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_5_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if>  
	<#if CONFIG_USB_DEVICE_FUNCTION_6_IDX0 == true>
    /* Function 6 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_6_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_6_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_6_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_6_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_6_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if> 
	<#if CONFIG_USB_DEVICE_FUNCTION_7_IDX0 == true>
    /* Function 7 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_7_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_7_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_7_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_7_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_7_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if> 
	<#if CONFIG_USB_DEVICE_FUNCTION_8_IDX0 == true>
    /* Function 8 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_8_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_8_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_8_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_8_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if>   
	<#if CONFIG_USB_DEVICE_FUNCTION_9_IDX0 == true>
    /* Function 9 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_9_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_9_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_9_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_9_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_9_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if> 
	<#if CONFIG_USB_DEVICE_FUNCTION_10_IDX0 == true>
    /* Function 5 */ 
    {
        .configurationValue = ${CONFIG_USB_DEVICE_FUNCTION_10_CONFIGURATION_IDX0},    /* Configuration value */
        .interfaceNumber = ${CONFIG_USB_DEVICE_FUNCTION_10_INTERFACE_NUMBER_IDX0},       /* First interfaceNumber of this function */
        .numberOfInterfaces = ${CONFIG_USB_DEVICE_FUNCTION_10_NUMBER_OF_INTERFACES_IDX0},    /* Number of interfaces */
        <#if CONFIG_PIC32MZ == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_10_SPEED_HS_IDX0},    /* Function Speed */ 
		<#elseif CONFIG_PIC32MX == true>
        .speed = ${CONFIG_USB_DEVICE_FUNCTION_10_SPEED_FS_IDX0},    /* Function Speed */ 
		</#if>
        <#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_CDC_IDX0 == true>
        .funcDriverIndex = ${usbDeviceCDCInstancesTracking},  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit${usbDeviceCDCInstancesTracking}    /* Function driver init data */
        <#assign usbDeviceCDCInstancesTracking = usbDeviceCDCInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX0 == true >
        .funcDriverIndex = ${usbDeviceHIDInstancesTracking},  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit${usbDeviceHIDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceHIDInstancesTracking = usbDeviceHIDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_MSD_IDX0 == true >
        .funcDriverIndex = ${usbDeviceMSDInstancesTracking},  /* Index of MSD Function Driver */
        .driver = (void*)USB_DEVICE_MSD_FUNCTION_DRIVER,    /* USB MSD function data exposed to device layer */
        .funcDriverInit = (void*)&msdInit${usbDeviceMSDInstancesTracking},   /* Function driver init data */
        <#assign usbDeviceMSDInstancesTracking = usbDeviceMSDInstancesTracking + 1>
        <#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,  /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},  /* Function driver init data */
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
		<#elseif CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX0 == true >
        .funcDriverIndex = ${usbDeviceAudioInstancesTracking},  /* Index of Audio Function Driver */
        .driver = (void*)USB_DEVICE_AUDIO_FUNCTION_DRIVER,   /* USB Audio function data exposed to device layer */
        .funcDriverInit = (void*)&audioInit${usbDeviceAudioInstancesTracking},   /* Function driver init data*/
        <#assign usbDeviceAudioInstancesTracking = usbDeviceAudioInstancesTracking + 1>
        <#else>
        .funcDriverIndex = ${usbDeviceVendorInstancesTracking},  /* Index of Vendor Function Driver */
        .driver = NULL,            /* No Function Driver data */ 
        .funcDriverInit = NULL     /* No Function Driver Init data */
        <#assign usbDeviceVendorInstancesTracking = usbDeviceVendorInstancesTracking + 1>
        </#if>
    },
    </#if>   
};

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/
//TODO - Copy USB Descriptors - Device descriptor,
//Configuration Descriptors and String Descriptors. 


/*******************************************
 * USB Device Layer Master Descriptor Table 
 *******************************************/
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    //TODO: Add Master Descriptor here. 
};

