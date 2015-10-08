<#--
/*******************************************************************************
  IC Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_oc_int.s.ftl

  Summary:
    IC driver interrupt handler templates.

  Description:
    The IC device driver provides a simple interface to manage the IC
    modules on Microchip microcontrollers and this module implements the
    interrupts.
    
  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.
    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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
-->
<#macro DRV_OC_ASM_INT DRV_OC_INSTANCE DRV_OC_ID>
<#if DRV_OC_ID == "OC_ID_1">
<#assign OC_ISR_VECTOR = CONFIG_INT_VECT_OUTPUT_COMPARE_1>
</#if>
<#if DRV_OC_ID == "OC_ID_2">
<#assign OC_ISR_VECTOR = CONFIG_INT_VECT_OUTPUT_COMPARE_2>
</#if>
<#if DRV_OC_ID == "OC_ID_3">
<#assign OC_ISR_VECTOR = CONFIG_INT_VECT_OUTPUT_COMPARE_3>
</#if>
<#if DRV_OC_ID == "OC_ID_4">
<#assign OC_ISR_VECTOR = CONFIG_INT_VECT_OUTPUT_COMPARE_4>
</#if>
<#if DRV_OC_ID == "OC_ID_5">
<#assign OC_ISR_VECTOR = CONFIG_INT_VECT_OUTPUT_COMPARE_5>
</#if>
<#if DRV_OC_ID == "OC_ID_6">
<#assign OC_ISR_VECTOR = CONFIG_INT_VECT_OUTPUT_COMPARE_6>
</#if>
<#if DRV_OC_ID == "OC_ID_7">
<#assign OC_ISR_VECTOR = CONFIG_INT_VECT_OUTPUT_COMPARE_7>
</#if>
<#if DRV_OC_ID == "OC_ID_8">
<#assign OC_ISR_VECTOR = CONFIG_INT_VECT_OUTPUT_COMPARE_8>
</#if>
<#if DRV_OC_ID == "OC_ID_9">
<#assign OC_ISR_VECTOR = CONFIG_INT_VECT_OUTPUT_COMPARE_9>
</#if>
<#if DRV_OC_INSTANCE == "0">
<#assign OC_ISR_NAME = "DrvOCInstance0">
</#if>
<#if DRV_OC_INSTANCE == "1">
<#assign OC_ISR_NAME = "DrvOCInstance1">
</#if>
<#if DRV_OC_INSTANCE == "2">
<#assign OC_ISR_NAME = "DrvOCInstance2">
</#if>
<#if DRV_OC_INSTANCE == "3">
<#assign OC_ISR_NAME = "DrvOCInstance3">
</#if>
<#if DRV_OC_INSTANCE == "4">
<#assign OC_ISR_NAME = "DrvOCInstance4">
</#if>
<#if DRV_OC_INSTANCE == "5">
<#assign OC_ISR_NAME = "DrvOCInstance5">
</#if>
<#if DRV_OC_INSTANCE == "6">
<#assign OC_ISR_NAME = "DrvOCInstance6">
</#if>
<#if DRV_OC_INSTANCE == "7">
<#assign OC_ISR_NAME = "DrvOCInstance7">
</#if>
<#if DRV_OC_INSTANCE == "8">
<#assign OC_ISR_NAME = "DrvOCInstance8">
</#if>

/* OC Instance ${DRV_OC_INSTANCE} Interrupt */

<@RTOS_ISR VECTOR = OC_ISR_VECTOR NAME = OC_ISR_NAME/>
</#macro>
<#if CONFIG_DRV_OC_INTERRUPT_MODE == true>
<#if CONFIG_DRV_OC_INST_IDX0 == true>
<@DRV_OC_ASM_INT DRV_OC_INSTANCE="0" DRV_OC_ID=CONFIG_DRV_OC_PERIPHERAL_ID_IDX0/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX1 == true>
<@DRV_OC_ASM_INT DRV_OC_INSTANCE="1" DRV_OC_ID=CONFIG_DRV_OC_PERIPHERAL_ID_IDX1/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX2 == true>
<@DRV_OC_ASM_INT DRV_OC_INSTANCE="2" DRV_OC_ID=CONFIG_DRV_OC_PERIPHERAL_ID_IDX2/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX3 == true>
<@DRV_OC_ASM_INT DRV_OC_INSTANCE="3" DRV_OC_ID=CONFIG_DRV_OC_PERIPHERAL_ID_IDX3/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX4 == true>
<@DRV_OC_ASM_INT DRV_OC_INSTANCE="4" DRV_OC_ID=CONFIG_DRV_OC_PERIPHERAL_ID_IDX4/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX5 == true>
<@DRV_OC_ASM_INT DRV_OC_INSTANCE="5" DRV_OC_ID=CONFIG_DRV_OC_PERIPHERAL_ID_IDX5/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX6 == true>
<@DRV_OC_ASM_INT DRV_OC_INSTANCE="6" DRV_OC_ID=CONFIG_DRV_OC_PERIPHERAL_ID_IDX6/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX7 == true>
<@DRV_OC_ASM_INT DRV_OC_INSTANCE="7" DRV_OC_ID=CONFIG_DRV_OC_PERIPHERAL_ID_IDX7/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX8 == true>
<@DRV_OC_ASM_INT DRV_OC_INSTANCE="8" DRV_OC_ID=CONFIG_DRV_OC_PERIPHERAL_ID_IDX8/>
</#if>
</#if>
