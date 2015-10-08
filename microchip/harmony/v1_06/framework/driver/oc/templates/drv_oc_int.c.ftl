<#--
/*******************************************************************************
  OC Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_oc_int.c.ftl

  Summary:
    OC driver interrupt handler templates.

  Description:
    The OC device driver provides a simple interface to manage the OC
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
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTOCULAR PURPOSE.
IN NO EVENT SHALL MOCROCHIP OR ITS LOCENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STROCT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVOCES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END
-->
<#macro DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE OC_INT_SRC OC_INT_ISR OC_INT_PRINUM>
<#if CONFIG_DRV_OC_INTERRUPT_MODE == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${OC_INT_ISR}, ipl${OC_INT_PRINUM}) _IntHandlerDrvOCInstance${DRV_INSTANCE}(void)
<#else>
void IntHandlerDrvOCInstance${DRV_INSTANCE}(void)
</#if>

<#else>
void __ISR(${OC_INT_ISR}, ipl${OC_INT_PRINUM}) _IntHandlerDrvOCInstance${DRV_INSTANCE}(void)
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS == true && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>
    PLIB_INT_SourceFlagClear(INT_ID_0, ${OC_INT_SRC});
<#if CONFIG_USE_3RDPARTY_RTOS == true && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>
</#macro>

<#if CONFIG_DRV_OC_INST_IDX0 == true>
<@DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE="0" OC_INT_SRC=CONFIG_DRV_OC_INTERRUPT_SOURCE_IDX0 
OC_INT_ISR=CONFIG_DRV_OC_ISR_VECTOR_IDX0 OC_INT_PRINUM=CONFIG_DRV_OC_INT_PRIO_NUM_IDX0/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX1 == true>
<@DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE="1" OC_INT_SRC=CONFIG_DRV_OC_INTERRUPT_SOURCE_IDX1 
OC_INT_ISR=CONFIG_DRV_OC_ISR_VECTOR_IDX1 OC_INT_PRINUM=CONFIG_DRV_OC_INT_PRIO_NUM_IDX1/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX2 == true>
<@DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE="2" OC_INT_SRC=CONFIG_DRV_OC_INTERRUPT_SOURCE_IDX2 
OC_INT_ISR=CONFIG_DRV_OC_ISR_VECTOR_IDX2 OC_INT_PRINUM=CONFIG_DRV_OC_INT_PRIO_NUM_IDX2/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX3 == true>
<@DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE="3" OC_INT_SRC=CONFIG_DRV_OC_INTERRUPT_SOURCE_IDX3 
OC_INT_ISR=CONFIG_DRV_OC_ISR_VECTOR_IDX3 OC_INT_PRINUM=CONFIG_DRV_OC_INT_PRIO_NUM_IDX3/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX4 == true>
<@DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE="4" OC_INT_SRC=CONFIG_DRV_OC_INTERRUPT_SOURCE_IDX4  
OC_INT_ISR=CONFIG_DRV_OC_ISR_VECTOR_IDX4 OC_INT_PRINUM=CONFIG_DRV_OC_INT_PRIO_NUM_IDX4/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX5 == true>
<@DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE="5" OC_INT_SRC=CONFIG_DRV_OC_INTERRUPT_SOURCE_IDX5
OC_INT_ISR=CONFIG_DRV_OC_ISR_VECTOR_IDX5 OC_INT_PRINUM=CONFIG_DRV_OC_INT_PRIO_NUM_IDX5/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX6 == true>
<@DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE="6" OC_INT_SRC=CONFIG_DRV_OC_INTERRUPT_SOURCE_IDX6 
OC_INT_ISR=CONFIG_DRV_OC_ISR_VECTOR_IDX6 OC_INT_PRINUM=CONFIG_DRV_OC_INT_PRIO_NUM_IDX6/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX7 == true>
<@DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE="7" OC_INT_SRC=CONFIG_DRV_OC_INTERRUPT_SOURCE_IDX7 
OC_INT_ISR=CONFIG_DRV_OC_ISR_VECTOR_IDX7 OC_INT_PRINUM=CONFIG_DRV_OC_INT_PRIO_NUM_IDX7/>
</#if>
<#if CONFIG_DRV_OC_INST_IDX8 == true>
<@DRV_OC_STATIC_FUNCTIONS DRV_INSTANCE="8" OC_INT_SRC=CONFIG_DRV_OC_INTERRUPT_SOURCE_IDX8 
OC_INT_ISR=CONFIG_DRV_OC_ISR_VECTOR_IDX8 OC_INT_PRINUM=CONFIG_DRV_OC_INT_PRIO_NUM_IDX8/>
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->
