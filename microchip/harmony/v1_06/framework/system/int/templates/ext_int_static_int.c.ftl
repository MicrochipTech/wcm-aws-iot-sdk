<#--
/*******************************************************************************
  External Interrupt Handler Template File

  File Name:
    ext_int_static.c

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

<#macro EXTERNAL_INTERRUPT_FUNCTION INST_NUMBER INTERRUPT_NUMBER ISR_VECTOR INT_PRIO>
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${ISR_VECTOR}, IPL${INT_PRIO}AUTO) _IntHandlerExternalInterruptInstance${INST_NUMBER}(void)
<#else>
void IntHandlerExternalInterruptInstance${INST_NUMBER}(void)
</#if>
<#else>
void __ISR(${ISR_VECTOR}, IPL${INT_PRIO}AUTO) _IntHandlerExternalInterruptInstance${INST_NUMBER}(void)
</#if>
{           
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>
    PLIB_INT_SourceFlagClear(INT_ID_0, ${INTERRUPT_NUMBER});

    <#if CONFIG_DRV_TOUCH_MTCH6301_INTERRUPT_MODE == true>

    DRV_TOUCH_MTCH6301_ReadRequest(sysObj.drvMtch6301);

    </#if>
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>
}

</#macro>

<#if CONFIG_EXT_INT_INST_IDX0 == true && CONFIG_EXT_INT_GENERATE_CODE_IDX0 == true>
<@EXTERNAL_INTERRUPT_FUNCTION INST_NUMBER="0" INTERRUPT_NUMBER=CONFIG_EXT_INT_INTERRUPT_SOURCE_IDX0 
ISR_VECTOR=CONFIG_EXT_INT_ISR_VECTOR_IDX0 INT_PRIO=CONFIG_EXT_INT_PRIO_NUM_IDX0/>
</#if>
<#if CONFIG_EXT_INT_INST_IDX1 == true && CONFIG_EXT_INT_GENERATE_CODE_IDX1 == true>
<@EXTERNAL_INTERRUPT_FUNCTION INST_NUMBER="1" INTERRUPT_NUMBER=CONFIG_EXT_INT_INTERRUPT_SOURCE_IDX1 
ISR_VECTOR=CONFIG_EXT_INT_ISR_VECTOR_IDX1 INT_PRIO=CONFIG_EXT_INT_PRIO_NUM_IDX1/>
</#if>
<#if CONFIG_EXT_INT_INST_IDX2 == true && CONFIG_EXT_INT_GENERATE_CODE_IDX2 == true>
<@EXTERNAL_INTERRUPT_FUNCTION INST_NUMBER="2" INTERRUPT_NUMBER=CONFIG_EXT_INT_INTERRUPT_SOURCE_IDX2 
ISR_VECTOR=CONFIG_EXT_INT_ISR_VECTOR_IDX2 INT_PRIO=CONFIG_EXT_INT_PRIO_NUM_IDX2/>
</#if>
<#if CONFIG_EXT_INT_INST_IDX3 == true && CONFIG_EXT_INT_GENERATE_CODE_IDX3 == true>
<@EXTERNAL_INTERRUPT_FUNCTION INST_NUMBER="3" INTERRUPT_NUMBER=CONFIG_EXT_INT_INTERRUPT_SOURCE_IDX3 
ISR_VECTOR=CONFIG_EXT_INT_ISR_VECTOR_IDX3 INT_PRIO=CONFIG_EXT_INT_PRIO_NUM_IDX3/>
</#if>
<#if CONFIG_EXT_INT_INST_IDX4 == true && CONFIG_EXT_INT_GENERATE_CODE_IDX4 == true>
<@EXTERNAL_INTERRUPT_FUNCTION INST_NUMBER="4" INTERRUPT_NUMBER=CONFIG_EXT_INT_INTERRUPT_SOURCE_IDX4 
ISR_VECTOR=CONFIG_EXT_INT_ISR_VECTOR_IDX4 INT_PRIO=CONFIG_EXT_INT_PRIO_NUM_IDX4/>
</#if>
