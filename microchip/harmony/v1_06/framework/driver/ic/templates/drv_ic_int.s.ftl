<#--
/*******************************************************************************
  IC Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_ic_int.s.ftl

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
<#macro DRV_IC_ASM_INT DRV_IC_INSTANCE DRV_IC_ID>
<#if CONFIG_PIC32MX == true>
<#if DRV_IC_ID == "IC_ID_1">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_1>
</#if>
<#if DRV_IC_ID == "IC_ID_2">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_2>
</#if>
<#if DRV_IC_ID == "IC_ID_3">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_3>
</#if>
<#if DRV_IC_ID == "IC_ID_4">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_4>
</#if>
<#if DRV_IC_ID == "IC_ID_5">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_5>
</#if>
<#if DRV_IC_ID == "IC_ID_6">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_6>
</#if>
<#if DRV_IC_ID == "IC_ID_7">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_7>
</#if>
<#if DRV_IC_ID == "IC_ID_8">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_8>
</#if>
<#if DRV_IC_ID == "IC_ID_9">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_9>
</#if>
<#if DRV_IC_INSTANCE == "0">
<#assign IC_ISR_NAME = "DrvICInstance0">
</#if>
<#if DRV_IC_INSTANCE == "1">
<#assign IC_ISR_NAME = "DrvICInstance1">
</#if>
<#if DRV_IC_INSTANCE == "2">
<#assign IC_ISR_NAME = "DrvICInstance2">
</#if>
<#if DRV_IC_INSTANCE == "3">
<#assign IC_ISR_NAME = "DrvICInstance3">
</#if>
<#if DRV_IC_INSTANCE == "4">
<#assign IC_ISR_NAME = "DrvICInstance4">
</#if>
<#if DRV_IC_INSTANCE == "5">
<#assign IC_ISR_NAME = "DrvICInstance5">
</#if>
<#if DRV_IC_INSTANCE == "6">
<#assign IC_ISR_NAME = "DrvICInstance6">
</#if>
<#if DRV_IC_INSTANCE == "7">
<#assign IC_ISR_NAME = "DrvICInstance7">
</#if>
<#if DRV_IC_INSTANCE == "8">
<#assign IC_ISR_NAME = "DrvICInstance8">
</#if>
/* IC Instance ${DRV_IC_INSTANCE} Interrupt */

<@RTOS_ISR VECTOR = IC_ISR_VECTOR NAME = IC_ISR_NAME/>
<#else>
<#if DRV_IC_ID == "IC_ID_1">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_1>
<#assign IC_FAULT_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_1_ERROR>
</#if>
<#if DRV_IC_ID == "IC_ID_2">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_2>
<#assign IC_FAULT_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_2_ERROR>
</#if>
<#if DRV_IC_ID == "IC_ID_3">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_3>
<#assign IC_FAULT_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_3_ERROR>
</#if>
<#if DRV_IC_ID == "IC_ID_4">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_4>
<#assign IC_FAULT_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_4_ERROR>
</#if>
<#if DRV_IC_ID == "IC_ID_5">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_5>
<#assign IC_FAULT_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_5_ERROR>
</#if>
<#if DRV_IC_ID == "IC_ID_6">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_6>
<#assign IC_FAULT_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_6_ERROR>
</#if>
<#if DRV_IC_ID == "IC_ID_7">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_7>
<#assign IC_FAULT_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_7_ERROR>
</#if>
<#if DRV_IC_ID == "IC_ID_8">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_8>
<#assign IC_FAULT_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_8_ERROR>
</#if>
<#if DRV_IC_ID == "IC_ID_9">
<#assign IC_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_9>
<#assign IC_FAULT_ISR_VECTOR = CONFIG_INT_VECT_INPUT_CAPTURE_9_ERROR>
</#if>
<#if DRV_IC_INSTANCE == "0">
<#assign IC_ISR_NAME = "DrvICInstance0">
<#assign IC_FAULT_ISR_NAME = "DrvICErrorInstance0">
</#if>
<#if DRV_IC_INSTANCE == "1">
<#assign IC_ISR_NAME = "DrvICInstance1">
<#assign IC_FAULT_ISR_NAME = "DrvICErrorInstance1">
</#if>
<#if DRV_IC_INSTANCE == "2">
<#assign IC_ISR_NAME = "DrvICInstance2">
<#assign IC_FAULT_ISR_NAME = "DrvICErrorInstance2">
</#if>
<#if DRV_IC_INSTANCE == "3">
<#assign IC_ISR_NAME = "DrvICInstance3">
<#assign IC_FAULT_ISR_NAME = "DrvICErrorInstance3">
</#if>
<#if DRV_IC_INSTANCE == "4">
<#assign IC_ISR_NAME = "DrvICInstance4">
<#assign IC_FAULT_ISR_NAME = "DrvICErrorInstance4">
</#if>
<#if DRV_IC_INSTANCE == "5">
<#assign IC_ISR_NAME = "DrvICInstance5">
<#assign IC_FAULT_ISR_NAME = "DrvICErrorInstance5">
</#if>
<#if DRV_IC_INSTANCE == "6">
<#assign IC_ISR_NAME = "DrvICInstance6">
<#assign IC_FAULT_ISR_NAME = "DrvICErrorInstance6">
</#if>
<#if DRV_IC_INSTANCE == "7">
<#assign IC_ISR_NAME = "DrvICInstance7">
<#assign IC_FAULT_ISR_NAME = "DrvICErrorInstance7">
</#if>
<#if DRV_IC_INSTANCE == "8">
<#assign IC_ISR_NAME = "DrvICInstance8">
<#assign IC_FAULT_ISR_NAME = "DrvICErrorInstance8">
</#if>
/* IC Instance ${DRV_IC_INSTANCE} Interrupts */

<@RTOS_ISR VECTOR = IC_ISR_VECTOR NAME = IC_ISR_NAME/>
<@RTOS_ISR VECTOR = IC_FAULT_ISR_VECTOR NAME = IC_FAULT_ISR_NAME/>
</#if>
</#macro>
<#if CONFIG_DRV_IC_INTERRUPT_MODE == true>
<#if CONFIG_DRV_IC_INST_IDX0 == true>
<@DRV_IC_ASM_INT DRV_IC_INSTANCE="0" DRV_IC_ID=CONFIG_DRV_IC_PERIPHERAL_ID_IDX0/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX1 == true>
<@DRV_IC_ASM_INT DRV_IC_INSTANCE="1" DRV_IC_ID=CONFIG_DRV_IC_PERIPHERAL_ID_IDX1/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX2 == true>
<@DRV_IC_ASM_INT DRV_IC_INSTANCE="2" DRV_IC_ID=CONFIG_DRV_IC_PERIPHERAL_ID_IDX2/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX3 == true>
<@DRV_IC_ASM_INT DRV_IC_INSTANCE="3" DRV_IC_ID=CONFIG_DRV_IC_PERIPHERAL_ID_IDX3/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX4 == true>
<@DRV_IC_ASM_INT DRV_IC_INSTANCE="4" DRV_IC_ID=CONFIG_DRV_IC_PERIPHERAL_ID_IDX4/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX5 == true>
<@DRV_IC_ASM_INT DRV_IC_INSTANCE="5" DRV_IC_ID=CONFIG_DRV_IC_PERIPHERAL_ID_IDX5/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX6 == true>
<@DRV_IC_ASM_INT DRV_IC_INSTANCE="6" DRV_IC_ID=CONFIG_DRV_IC_PERIPHERAL_ID_IDX6/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX7 == true>
<@DRV_IC_ASM_INT DRV_IC_INSTANCE="7" DRV_IC_ID=CONFIG_DRV_IC_PERIPHERAL_ID_IDX7/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX8 == true>
<@DRV_IC_ASM_INT DRV_IC_INSTANCE="8" DRV_IC_ID=CONFIG_DRV_IC_PERIPHERAL_ID_IDX8/>
</#if>
</#if>
