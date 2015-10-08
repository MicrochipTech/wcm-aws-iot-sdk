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
/*** USB Driver Configuration ***/

/* Enables Device Support */
<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
#define DRV_USB_DEVICE_SUPPORT      true

<#else>
#define DRV_USB_DEVICE_SUPPORT      false

</#if>
/* Enables Device Support */
<#if CONFIG_DRV_USB_HOST_SUPPORT == true>
#define DRV_USB_HOST_SUPPORT        true

#define DRV_USB_INSTANCES_NUMBER    1

<#else>
#define DRV_USB_HOST_SUPPORT        false

</#if>

<#if CONFIG_PIC32MZ == true>
/* Maximum USB driver instances */

<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
#define DRV_USB_INSTANCES_NUMBER    ${CONFIG_USB_DEVICE_INSTANCES_NUMBER}

</#if>
<#elseif CONFIG_DRV_USB_DRIVER_MODE == "DYNAMIC">
/* Maximum USB driver instances */
<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
#define DRV_USB_INSTANCES_NUMBER    ${CONFIG_USB_DEVICE_INSTANCES_NUMBER}
</#if>
<#elseif CONFIG_DRV_USB_DRIVER_MODE == "STATIC">
/* DRV_USB_INSTANCES_NUMBER should not be defined for static.
 * The below macro needs to be defined for static.
 */
#define DRV_USB_PERIPHERAL_ID       USB_ID_1
#define DRV_USB_INTERRUPT_SOURCE    INT_SOURCE_USB_1
#define DRV_USB_INDEX               0
</#if>

/* Interrupt mode enabled */
<#if CONFIG_DRV_USB_INTERRUPT_MODE == true>
#define DRV_USB_INTERRUPT_MODE      true

<#else>
#define DRV_USB_INTERRUPT_MODE      false

</#if>
/* Number of Endpoints used */
<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
#define DRV_USB_ENDPOINTS_NUMBER    ${CONFIG_DRV_USB_ENDPOINTS_NUMBER}
<#else>
#define DRV_USB_ENDPOINTS_NUMBER    1
</#if>

<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
<#include "/framework/usb/templates/usb_device.h.ftl">
</#if>

<#if CONFIG_DRV_USB_HOST_SUPPORT == true>
<#include "/framework/usb/templates/usb_host.h.ftl">
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->
