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
<#include "/framework/usb/templates/system_config.c.ftl">
<#if CONFIG_PIC32MX == true >
/****************************************************
 * Endpoint Table needed by the Device Layer.
 ****************************************************/
uint8_t __attribute__((aligned(512))) endPointTable[USB_DEVICE_ENDPOINT_TABLE_SIZE];
</#if>

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

	/* Identifies peripheral (PLIB-level) ID */
<#if CONFIG_DRV_USB_PERIPHERAL_ID_IDX0?has_content> 
    .usbID = ${CONFIG_DRV_USB_PERIPHERAL_ID_IDX0},
</#if>

    /* Stop in idle */
<#if CONFIG_USB_DEVICE_SLEEP_IN_IDLE_IDX0 == true>
    .stopInIdle = true,
<#else>
    .stopInIdle = false,
</#if>

    /* Suspend in sleep */
<#if CONFIG_USB_DEVICE_SUSPEND_IN_SLEEP_IDX0 == true>
    .suspendInSleep= true,
<#else>
    .suspendInSleep = false,
</#if>
<#if CONFIG_DRV_USB_INTERRUPT_MODE == true>
    /* Interrupt Source for USB module */
    .interruptSource = ${CONFIG_DRV_USB_INTERRUPT_SOURCE_IDX0},

<#if CONFIG_PIC32MZ == true >

    /* Interrupt Source for USB module */
    .interruptSourceUSBDma = INT_SOURCE_USB_1_DMA,
</#if>
</#if>
<#if CONFIG_PIC32MX == true >
    /* Endpoint table */
    .endpointTable= endPointTable,
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
