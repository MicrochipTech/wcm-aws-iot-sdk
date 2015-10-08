<#--
/*******************************************************************************
  SPI interrupt Template File

  File Name:
    drv_spi_int.s

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to run the system.  It
	generates code that is added to system_interrupt.c in order to handle
	all interrupts.
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
<#macro DRV_SPI_ASM_MX_INT DRV_SPI_INSTANCE DRV_SPI_ID>
<#if DRV_SPI_ID == "SPI_ID_1">
<#assign SPI_ISR_VECTOR = CONFIG_INT_VECT_SPI_1>
</#if>
<#if DRV_SPI_ID == "SPI_ID_2">
<#assign SPI_ISR_VECTOR = CONFIG_INT_VECT_SPI_2>
</#if>
<#if DRV_SPI_ID == "SPI_ID_3">
<#assign SPI_ISR_VECTOR = CONFIG_INT_VECT_SPI_3>
</#if>
<#if DRV_SPI_ID == "SPI_ID_4">
<#assign SPI_ISR_VECTOR = CONFIG_INT_VECT_SPI_4>
</#if>
<#if DRV_SPI_ID == "SPI_ID_5">
<#assign SPI_ISR_VECTOR = CONFIG_INT_VECT_SPI_5>
</#if>
<#if DRV_SPI_ID == "SPI_ID_6">
<#assign SPI_ISR_VECTOR = CONFIG_INT_VECT_SPI_6>
</#if>
<#if DRV_SPI_INSTANCE == "0">
<#assign SPI_ISR_NAME = "SPIInstance0">
</#if>
<#if DRV_SPI_INSTANCE == "1">
<#assign SPI_ISR_NAME = "SPIInstance1">
</#if>
<#if DRV_SPI_INSTANCE == "2">
<#assign SPI_ISR_NAME = "SPIInstance2">
</#if>
<#if DRV_SPI_INSTANCE == "3">
<#assign SPI_ISR_NAME = "SPIInstance3">
</#if>
<#if DRV_SPI_INSTANCE == "4">
<#assign SPI_ISR_NAME = "SPIInstance4">
</#if>
<#if DRV_SPI_INSTANCE == "5">
<#assign SPI_ISR_NAME = "SPIInstance5">
</#if>

/* SPI Instance ${DRV_SPI_INSTANCE} Interrupt */

<@RTOS_ISR VECTOR = SPI_ISR_VECTOR NAME = SPI_ISR_NAME/>
</#macro>
<#macro DRV_SPI_ASM_MZ_INT DRV_SPI_INSTANCE DRV_SPI_ID>
<#if DRV_SPI_ID == "SPI_ID_1">
<#assign SPI_RX_ISR_VECTOR = CONFIG_INT_VECT_SPI1_RX>
<#assign SPI_TX_ISR_VECTOR = CONFIG_INT_VECT_SPI1_TX>
<#assign SPI_FAULT_ISR_VECTOR = CONFIG_INT_VECT_SPI1_FAULT>
</#if>
<#if DRV_SPI_ID == "SPI_ID_2">
<#assign SPI_RX_ISR_VECTOR = CONFIG_INT_VECT_SPI2_RX>
<#assign SPI_TX_ISR_VECTOR = CONFIG_INT_VECT_SPI2_TX>
<#assign SPI_FAULT_ISR_VECTOR = CONFIG_INT_VECT_SPI2_FAULT>
</#if>
<#if DRV_SPI_ID == "SPI_ID_3">
<#assign SPI_RX_ISR_VECTOR = CONFIG_INT_VECT_SPI3_RX>
<#assign SPI_TX_ISR_VECTOR = CONFIG_INT_VECT_SPI3_TX>
<#assign SPI_FAULT_ISR_VECTOR = CONFIG_INT_VECT_SPI3_FAULT>
</#if>
<#if DRV_SPI_ID == "SPI_ID_4">
<#assign SPI_RX_ISR_VECTOR = CONFIG_INT_VECT_SPI4_RX>
<#assign SPI_TX_ISR_VECTOR = CONFIG_INT_VECT_SPI4_TX>
<#assign SPI_FAULT_ISR_VECTOR = CONFIG_INT_VECT_SPI4_FAULT>
</#if>
<#if DRV_SPI_ID == "SPI_ID_5">
<#assign SPI_RX_ISR_VECTOR = CONFIG_INT_VECT_SPI5_RX>
<#assign SPI_TX_ISR_VECTOR = CONFIG_INT_VECT_SPI5_TX>
<#assign SPI_FAULT_ISR_VECTOR = CONFIG_INT_VECT_SPI5_FAULT>
</#if>
<#if DRV_SPI_ID == "SPI_ID_6">
<#assign SPI_RX_ISR_VECTOR = CONFIG_INT_VECT_SPI6_RX>
<#assign SPI_TX_ISR_VECTOR = CONFIG_INT_VECT_SPI6_TX>
<#assign SPI_FAULT_ISR_VECTOR = CONFIG_INT_VECT_SPI6_FAULT>
</#if>
<#if DRV_SPI_INSTANCE == "0">
<#assign SPI_RX_ISR_NAME = "SPIRxInstance0">
<#assign SPI_TX_ISR_NAME = "SPITxInstance0">
<#assign SPI_FAULT_ISR_NAME = "SPIFaultInstance0">
</#if>
<#if DRV_SPI_INSTANCE == "1">
<#assign SPI_RX_ISR_NAME = "SPIRxInstance1">
<#assign SPI_TX_ISR_NAME = "SPITxInstance1">
<#assign SPI_FAULT_ISR_NAME = "SPIFaultInstance1">
</#if>
<#if DRV_SPI_INSTANCE == "2">
<#assign SPI_RX_ISR_NAME = "SPIRxInstance2">
<#assign SPI_TX_ISR_NAME = "SPITxInstance2">
<#assign SPI_FAULT_ISR_NAME = "SPIFaultInstance2">
</#if>
<#if DRV_SPI_INSTANCE == "3">
<#assign SPI_RX_ISR_NAME = "SPIRxInstance3">
<#assign SPI_TX_ISR_NAME = "SPITxInstance3">
<#assign SPI_FAULT_ISR_NAME = "SPIFaultInstance3">
</#if>
<#if DRV_SPI_INSTANCE == "4">
<#assign SPI_RX_ISR_NAME = "SPIRxInstance4">
<#assign SPI_TX_ISR_NAME = "SPITxInstance4">
<#assign SPI_FAULT_ISR_NAME = "SPIFaultInstance4">
</#if>
<#if DRV_SPI_INSTANCE == "5">
<#assign SPI_RX_ISR_NAME = "SPIRxInstance5">
<#assign SPI_TX_ISR_NAME = "SPITxInstance5">
<#assign SPI_FAULT_ISR_NAME = "SPIFaultInstance5">
</#if>

/* SPI Instance ${DRV_SPI_INSTANCE} Interrupt */

<@RTOS_ISR VECTOR = SPI_RX_ISR_VECTOR NAME = SPI_RX_ISR_NAME/>
<@RTOS_ISR VECTOR = SPI_TX_ISR_VECTOR NAME = SPI_TX_ISR_NAME/>
<@RTOS_ISR VECTOR = SPI_FAULT_ISR_VECTOR NAME = SPI_FAULT_ISR_NAME/>
</#macro>

<#if CONFIG_PIC32MX == true>
<#if CONFIG_DRV_SPI_IDX0 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX0 == true>
<@DRV_SPI_ASM_MX_INT DRV_SPI_INSTANCE="0" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX0/>
</#if>
<#if CONFIG_DRV_SPI_IDX1 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX1 == true>
<@DRV_SPI_ASM_MX_INT DRV_SPI_INSTANCE="1" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX1/>
</#if>
<#if CONFIG_DRV_SPI_IDX2 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX2 == true>
<@DRV_SPI_ASM_MX_INT DRV_SPI_INSTANCE="2" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX2/>
</#if>
<#if CONFIG_DRV_SPI_IDX3 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX3 == true>
<@DRV_SPI_ASM_MX_INT DRV_SPI_INSTANCE="3" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX3/>
</#if>
<#if CONFIG_DRV_SPI_IDX4 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX4 == true>
<@DRV_SPI_ASM_MX_INT DRV_SPI_INSTANCE="4" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX4/>
</#if>
<#if CONFIG_DRV_SPI_IDX5 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX5 == true>
<@DRV_SPI_ASM_MX_INT DRV_SPI_INSTANCE="5" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX5/>
</#if>
<#else>
<#if CONFIG_DRV_SPI_IDX0 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX0 == true>
<@DRV_SPI_ASM_MZ_INT DRV_SPI_INSTANCE="0" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX0/>
</#if>
<#if CONFIG_DRV_SPI_IDX1 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX1 == true>
<@DRV_SPI_ASM_MZ_INT DRV_SPI_INSTANCE="1" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX1/>
</#if>
<#if CONFIG_DRV_SPI_IDX2 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX2 == true>
<@DRV_SPI_ASM_MZ_INT DRV_SPI_INSTANCE="2" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX2/>
</#if>
<#if CONFIG_DRV_SPI_IDX3 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX3 == true>
<@DRV_SPI_ASM_MZ_INT DRV_SPI_INSTANCE="3" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX3/>
</#if>
<#if CONFIG_DRV_SPI_IDX4 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX4 == true>
<@DRV_SPI_ASM_MZ_INT DRV_SPI_INSTANCE="4" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX4/>
</#if>
<#if CONFIG_DRV_SPI_IDX5 == true && CONFIG_DRV_SPI_TASK_MODE_ISR_IDX5 == true>
<@DRV_SPI_ASM_MZ_INT DRV_SPI_INSTANCE="5" DRV_SPI_ID=CONFIG_DRV_SPI_SPI_ID_IDX5/>
</#if>
</#if>
