<#--
/*******************************************************************************
  USART Driver Initialization File

  File Name:
    drv_tmr_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
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
<#if CONFIG_DRV_USART_INST_IDX0 == true>
<#if CONFIG_DRV_USART_DRIVER_MODE == "DYNAMIC">
    sysObj.drvUsart0 = DRV_USART_Initialize(DRV_USART_INDEX_0, (SYS_MODULE_INIT *)&drvUsart0InitData);
</#if>
<#if CONFIG_DRV_USART_DRIVER_MODE == "STATIC">
    DRV_USART0_Initialize();
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX1 == true>
<#if CONFIG_DRV_USART_DRIVER_MODE == "DYNAMIC">
    sysObj.drvUsart1 = DRV_USART_Initialize(DRV_USART_INDEX_1, (SYS_MODULE_INIT *)&drvUsart1InitData);
</#if>
<#if CONFIG_DRV_USART_DRIVER_MODE == "STATIC">
    DRV_USART1_Initialize();
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX2 == true>
<#if CONFIG_DRV_USART_DRIVER_MODE == "DYNAMIC">
    sysObj.drvUsart2 = DRV_USART_Initialize(DRV_USART_INDEX_2, (SYS_MODULE_INIT *)&drvUsart2InitData);
</#if>
<#if CONFIG_DRV_USART_DRIVER_MODE == "STATIC">
    DRV_USART2_Initialize();
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX3 == true>
<#if CONFIG_DRV_USART_DRIVER_MODE == "DYNAMIC">
    sysObj.drvUsart3 = DRV_USART_Initialize(DRV_USART_INDEX_3, (SYS_MODULE_INIT *)&drvUsart3InitData);
</#if>
<#if CONFIG_DRV_USART_DRIVER_MODE == "STATIC">
    DRV_USART3_Initialize();
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX4 == true>
<#if CONFIG_DRV_USART_DRIVER_MODE == "DYNAMIC">
    sysObj.drvUsart4 = DRV_USART_Initialize(DRV_USART_INDEX_4, (SYS_MODULE_INIT *)&drvUsart4InitData);
</#if>
<#if CONFIG_DRV_USART_DRIVER_MODE == "STATIC">
    DRV_USART4_Initialize();
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX5 == true>
<#if CONFIG_DRV_USART_DRIVER_MODE == "DYNAMIC">
    sysObj.drvUsart5 = DRV_USART_Initialize(DRV_USART_INDEX_5, (SYS_MODULE_INIT *)&drvUsart5InitData);
</#if>
<#if CONFIG_DRV_USART_DRIVER_MODE == "STATIC">
    DRV_USART5_Initialize();
</#if>
</#if>
<#if CONFIG_DRV_USART_DRIVER_MODE == "DYNAMIC">
<#if CONFIG_USE_SYS_INT == true && CONFIG_DRV_USART_INTERRUPT_MODE == true && 
    CONFIG_DRV_USART_SUPPORT_TRANSMIT_DMA == false && CONFIG_DRV_USART_SUPPORT_RECEIVE_DMA == false>
<#if CONFIG_DRV_USART_INST_IDX0 == true>
<#if CONFIG_PIC32MZ == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX0}, ${CONFIG_DRV_USART_XMIT_INT_PRIORITY_IDX0});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX0}, ${CONFIG_DRV_USART_XMIT_INT_SUB_PRIORITY_IDX0});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX0}, ${CONFIG_DRV_USART_RCV_INT_PRIORITY_IDX0});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX0}, ${CONFIG_DRV_USART_RCV_INT_SUB_PRIORITY_IDX0});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX0}, ${CONFIG_DRV_USART_ERR_INT_PRIORITY_IDX0});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX0}, ${CONFIG_DRV_USART_ERR_INT_SUB_PRIORITY_IDX0});
</#if>
<#if CONFIG_PIC32MX == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX0}, ${CONFIG_DRV_USART_INT_PRIORITY_IDX0});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX0}, ${CONFIG_DRV_USART_INT_SUB_PRIORITY_IDX0});
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX1 == true>
<#if CONFIG_PIC32MZ == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX1}, ${CONFIG_DRV_USART_XMIT_INT_PRIORITY_IDX1});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX1}, ${CONFIG_DRV_USART_XMIT_INT_SUB_PRIORITY_IDX1});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX1}, ${CONFIG_DRV_USART_RCV_INT_PRIORITY_IDX1});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX1}, ${CONFIG_DRV_USART_RCV_INT_SUB_PRIORITY_IDX1});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX1}, ${CONFIG_DRV_USART_ERR_INT_PRIORITY_IDX1});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX1}, ${CONFIG_DRV_USART_ERR_INT_SUB_PRIORITY_IDX1});
</#if>
<#if CONFIG_PIC32MX == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX1}, ${CONFIG_DRV_USART_INT_PRIORITY_IDX1});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX1}, ${CONFIG_DRV_USART_INT_SUB_PRIORITY_IDX1});
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX2 == true>
<#if CONFIG_PIC32MZ == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX2}, ${CONFIG_DRV_USART_XMIT_INT_PRIORITY_IDX2});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX2}, ${CONFIG_DRV_USART_XMIT_INT_SUB_PRIORITY_IDX2});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX2}, ${CONFIG_DRV_USART_RCV_INT_PRIORITY_IDX2});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX2}, ${CONFIG_DRV_USART_RCV_INT_SUB_PRIORITY_IDX2});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX2}, ${CONFIG_DRV_USART_ERR_INT_PRIORITY_IDX2});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX2}, ${CONFIG_DRV_USART_ERR_INT_SUB_PRIORITY_IDX2});
</#if>
<#if CONFIG_PIC32MX == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX2}, ${CONFIG_DRV_USART_INT_PRIORITY_IDX2});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX2}, ${CONFIG_DRV_USART_INT_SUB_PRIORITY_IDX2});
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX3 == true>
<#if CONFIG_PIC32MZ == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX3}, ${CONFIG_DRV_USART_XMIT_INT_PRIORITY_IDX3});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX3}, ${CONFIG_DRV_USART_XMIT_INT_SUB_PRIORITY_IDX3});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX3}, ${CONFIG_DRV_USART_RCV_INT_PRIORITY_IDX3});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX3}, ${CONFIG_DRV_USART_RCV_INT_SUB_PRIORITY_IDX3});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX3}, ${CONFIG_DRV_USART_ERR_INT_PRIORITY_IDX3});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX3}, ${CONFIG_DRV_USART_ERR_INT_SUB_PRIORITY_IDX3});
</#if>
<#if CONFIG_PIC32MX == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX3}, ${CONFIG_DRV_USART_INT_PRIORITY_IDX3});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX3}, ${CONFIG_DRV_USART_INT_SUB_PRIORITY_IDX3});
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX4 == true>
<#if CONFIG_PIC32MZ == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX4}, ${CONFIG_DRV_USART_XMIT_INT_PRIORITY_IDX4});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX4}, ${CONFIG_DRV_USART_XMIT_INT_SUB_PRIORITY_IDX4});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX4}, ${CONFIG_DRV_USART_RCV_INT_PRIORITY_IDX4});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX4}, ${CONFIG_DRV_USART_RCV_INT_SUB_PRIORITY_IDX4});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX4}, ${CONFIG_DRV_USART_ERR_INT_PRIORITY_IDX4});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX4}, ${CONFIG_DRV_USART_ERR_INT_SUB_PRIORITY_IDX4});
</#if>
<#if CONFIG_PIC32MX == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX4}, ${CONFIG_DRV_USART_INT_PRIORITY_IDX4});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX4}, ${CONFIG_DRV_USART_INT_SUB_PRIORITY_IDX4});
</#if>
</#if>
<#if CONFIG_DRV_USART_INST_IDX5 == true>
<#if CONFIG_PIC32MZ == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX5}, ${CONFIG_DRV_USART_XMIT_INT_PRIORITY_IDX5});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_XMIT_INT_VECTOR_IDX5}, ${CONFIG_DRV_USART_XMIT_INT_SUB_PRIORITY_IDX5});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX5}, ${CONFIG_DRV_USART_RCV_INT_PRIORITY_IDX5});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_RCV_INT_VECTOR_IDX5}, ${CONFIG_DRV_USART_RCV_INT_SUB_PRIORITY_IDX5});
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX5}, ${CONFIG_DRV_USART_ERR_INT_PRIORITY_IDX5});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_ERR_INT_VECTOR_IDX5}, ${CONFIG_DRV_USART_ERR_INT_SUB_PRIORITY_IDX5});
</#if>
<#if CONFIG_PIC32MX == true>
    SYS_INT_VectorPrioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX5}, ${CONFIG_DRV_USART_INT_PRIORITY_IDX5});
    SYS_INT_VectorSubprioritySet(${CONFIG_DRV_USART_INT_VECTOR_IDX5}, ${CONFIG_DRV_USART_INT_SUB_PRIORITY_IDX5});
</#if>
</#if>
</#if>
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->
