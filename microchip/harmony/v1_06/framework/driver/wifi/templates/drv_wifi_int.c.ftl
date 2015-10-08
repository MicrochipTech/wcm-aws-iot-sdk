<#--
/*******************************************************************************
  Wi-Fi Driver Interrupt Handler Template File

  File Name:
    drv_wifi_int.c.ftl

  Summary:
    This file contains source code for interrupt handler.

  Description:
    It implements interrupt calls necessary for Wi-Fi driver in the application.
 *******************************************************************************/

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

<#if CONFIG_DRV_WIFI_DEVICE == "MRF24WG">
<#if CONFIG_BSP_PIC32MX795_PIM_E16 == true>
void __ISR(_EXTERNAL_1_VECTOR, ipl3AUTO) _InterruptHandler_MRF24W_Ext1(void)
{
<#if CONFIG_3RDPARTY_RTOS_USED == "FreeRTOS_V8.x.x">
    SYS_INT_SourceDisable(MRF_INT_SOURCE); // disable further interrupts
    DRV_WIFI_DeferredISR_SemGive();
<#else>
    DRV_WIFI_MRF24W_ISR((SYS_MODULE_OBJ)0);
</#if>
}
<#elseif CONFIG_BSP_PIC32MX_ETH_SK == true>
void __ISR(_CHANGE_NOTICE_VECTOR, ipl3AUTO) _InterruptHandler_MRF24W_CN(void)
{
<#if CONFIG_3RDPARTY_RTOS_USED == "FreeRTOS_V8.x.x">
    SYS_INT_SourceDisable(MRF_INT_SOURCE); // disable further interrupts
    DRV_WIFI_DeferredISR_SemGive();
<#else>
    DRV_WIFI_MRF24W_ISR((SYS_MODULE_OBJ)0);
</#if>
}
<#elseif CONFIG_BSP_PIC32MZ_EC_SK == true>
void __ISR(_EXTERNAL_1_VECTOR, ipl3AUTO) _InterruptHandler_MRF24W_Ext1(void)
{
<#if CONFIG_3RDPARTY_RTOS_USED == "FreeRTOS_V8.x.x">
    SYS_INT_SourceDisable(MRF_INT_SOURCE); // disable further interrupts
    DRV_WIFI_DeferredISR_SemGive();
<#else>
    DRV_WIFI_MRF24W_ISR((SYS_MODULE_OBJ)0);
</#if>
}
<#elseif CONFIG_BSP_PIC32MZ_EC_SK_MEB2 == true>
void __ISR(_EXTERNAL_0_VECTOR, ipl3AUTO) _InterruptHandler_MRF24W_Ext0(void)
{
<#if CONFIG_3RDPARTY_RTOS_USED == "FreeRTOS_V8.x.x">
    SYS_INT_SourceDisable(MRF_INT_SOURCE); // disable further interrupts
    DRV_WIFI_DeferredISR_SemGive();
<#else>
    DRV_WIFI_MRF24W_ISR((SYS_MODULE_OBJ)0);
</#if>
}
<#elseif CONFIG_BSP_WIFI_G_DB == true>
void __ISR(_EXTERNAL_1_VECTOR, ipl3AUTO) _InterruptHandler_MRF24W_Ext1(void)
{
<#if CONFIG_3RDPARTY_RTOS_USED == "FreeRTOS_V8.x.x">
    SYS_INT_SourceDisable(MRF_INT_SOURCE); // disable further interrupts
    DRV_WIFI_DeferredISR_SemGive();
<#else>
    DRV_WIFI_MRF24W_ISR((SYS_MODULE_OBJ)0);
</#if>
}
</#if>
</#if><#-- CONFIG_DRV_WIFI_DEVICE == "MRF24WG" -->

<#if CONFIG_DRV_WIFI_DEVICE == "MRF24WN">
<#if CONFIG_BSP_PIC32MZ_EC_SK == true>
void __ISR(_EXTERNAL_1_VECTOR, ipl3AUTO) _InterruptHandler_MRF24W_Ext1(void)
{
    WDRV_MRF24WN_ISR((SYS_MODULE_OBJ)0);
}
</#if><#-- CONFIG_BSP -->
</#if><#-- CONFIG_DRV_WIFI_DEVICE == "MRF24WN" -->
<#--
/*******************************************************************************
 End of File
*/
-->
