<#--
/*******************************************************************************
  Timer Driver Interrupt Handler Template File

  File Name:
    drv_tmr_int.c

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
<#if CONFIG_DRV_TMR_INTERRUPT_MODE == true>
<#if CONFIG_DRV_TMR_INST_0 == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX0}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerDrvTmrInstance0(void)
<#else>
void IntHandlerDrvTmrInstance0(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX0}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerDrvTmrInstance0(void)
</#if>    
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

<#if CONFIG_DRV_TMR_DRIVER_MODE == "DYNAMIC">
    DRV_TMR_Tasks_ISR(sysObj.drvTmr0);
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,${CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX0});
</#if>    
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>
<#if CONFIG_DRV_TMR_INST_1 == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX1}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX1}AUTO) _IntHandlerDrvTmrInstance1(void)
<#else>
IntHandlerDrvTmrInstance1(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX1}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX1}AUTO) _IntHandlerDrvTmrInstance1(void)
</#if> 
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

<#if CONFIG_DRV_TMR_DRIVER_MODE == "DYNAMIC">
    DRV_TMR_Tasks_ISR(sysObj.drvTmr1);
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,${CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX1});
</#if> 
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>
<#if CONFIG_DRV_TMR_INST_2 == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX2}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX2}AUTO) _IntHandlerDrvTmrInstance2(void)
<#else>
IntHandlerDrvTmrInstance2(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX2}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX2}AUTO) _IntHandlerDrvTmrInstance2(void)
</#if> 
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

<#if CONFIG_DRV_TMR_DRIVER_MODE == "DYNAMIC">
    DRV_TMR_Tasks_ISR(sysObj.drvTmr2);
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,${CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX2});
</#if> 
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>
<#if CONFIG_DRV_TMR_INST_3 == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX3}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX3}AUTO) _IntHandlerDrvTmrInstance3(void)
<#else>
void IntHandlerDrvTmrInstance3(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX3}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX3}AUTO) _IntHandlerDrvTmrInstance3(void)
</#if> 
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

<#if CONFIG_DRV_TMR_DRIVER_MODE == "DYNAMIC">
    DRV_TMR_Tasks_ISR(sysObj.drvTmr3);
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,${CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX3});
</#if> 
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>
<#if CONFIG_DRV_TMR_INST_4 == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX4}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX4}AUTO) _IntHandlerDrvTmrInstance4(void)
<#else>
void IntHandlerDrvTmrInstance4(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX4}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX4}AUTO) _IntHandlerDrvTmrInstance4(void)
</#if> 
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

<#if CONFIG_DRV_TMR_DRIVER_MODE == "DYNAMIC">
    DRV_TMR_Tasks_ISR(sysObj.drvTmr4);
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,${CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX4});
</#if> 
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>
<#if CONFIG_DRV_TMR_INST_5 == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX5}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX5}AUTO) _IntHandlerDrvTmrInstance5(void)
<#else>
void IntHandlerDrvTmrInstance5(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX5}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX5}AUTO) _IntHandlerDrvTmrInstance5(void)
</#if> 
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

<#if CONFIG_DRV_TMR_DRIVER_MODE == "DYNAMIC">
    DRV_TMR_Tasks_ISR(sysObj.drvTmr5);
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,${CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX5});
</#if> 
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>
<#if CONFIG_DRV_TMR_INST_6 == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX6}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX6}AUTO) _IntHandlerDrvTmrInstance6(void)
<#else>
void IntHandlerDrvTmrInstance6(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX6}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX6}AUTO) _IntHandlerDrvTmrInstance6(void)
</#if> 
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

<#if CONFIG_DRV_TMR_DRIVER_MODE == "DYNAMIC">
    DRV_TMR_Tasks_ISR(sysObj.drvTmr6);
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,${CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX6});
</#if> 
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>
<#if CONFIG_DRV_TMR_INST_7 == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX7}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX7}AUTO) _IntHandlerDrvTmrInstance7(void)
<#else>
void IntHandlerDrvTmrInstance7(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX7}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX7}AUTO) _IntHandlerDrvTmrInstance7(void)
</#if> 
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

<#if CONFIG_DRV_TMR_DRIVER_MODE == "DYNAMIC">
    DRV_TMR_Tasks_ISR(sysObj.drvTmr7);
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,${CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX7});
</#if> 
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>
<#if CONFIG_DRV_TMR_INST_8 == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX8}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX8}AUTO) _IntHandlerDrvTmrInstance8(void)
<#else>
void IntHandlerDrvTmrInstance8(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_TMR_ISR_VECTOR_IDX8}, ipl${CONFIG_DRV_TMR_INT_PRIO_NUM_IDX8}AUTO) _IntHandlerDrvTmrInstance8(void)
</#if> 
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

<#if CONFIG_DRV_TMR_DRIVER_MODE == "DYNAMIC">
    DRV_TMR_Tasks_ISR(sysObj.drvTmr8);
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,${CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX8});
</#if> 
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>
}
</#if>
</#if> <#--CONFIG_DRV_TMR_INTERRUPT_MODE == true-->

<#--
/*******************************************************************************
 End of File
*/
-->

