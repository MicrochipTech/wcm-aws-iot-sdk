<#--
/*******************************************************************************
  CMP Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_cmp_int.c.ftl

  Summary:
    CMP driver interrupt handler templates.

  Description:
    The CMP device driver provides a simple interface to manage the CMP
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
<#macro DRV_CMP_STATIC_FUNCTIONS DRV_INSTANCE CMP_INT_SRC CMP_INT_ISR CMP_INT_PRINUM>
<#if CONFIG_DRV_CMP_INTERRUPT_MODE == true>
<#if CONFIG_USE_3RDPARTY_RTOS>
void IntHandlerDrvCMPInstance${DRV_INSTANCE}(void)
<#else>
void __ISR(${CMP_INT_ISR}, ipl${CMP_INT_PRINUM}AUTO) _IntHandlerDrvCMPInstance${DRV_INSTANCE}(void)
</#if>
{
    PLIB_INT_SourceFlagClear(INT_ID_0, ${CMP_INT_SRC});
}
</#if>
</#macro>

<#if CONFIG_DRV_CMP_INST_IDX0 == true>
<@DRV_CMP_STATIC_FUNCTIONS DRV_INSTANCE="0" CMP_INT_SRC=CONFIG_DRV_CMP_INTERRUPT_SOURCE_IDX0
 CMP_INT_ISR=CONFIG_DRV_CMP_ISR_VECTOR_IDX0
CMP_INT_PRINUM=CONFIG_DRV_CMP_INT_PRIO_NUM_IDX0/>
</#if>
<#if CONFIG_DRV_CMP_INST_IDX1 == true>
<@DRV_CMP_STATIC_FUNCTIONS DRV_INSTANCE="1" CMP_INT_SRC=CONFIG_DRV_CMP_INTERRUPT_SOURCE_IDX1
 CMP_INT_ISR=CONFIG_DRV_CMP_ISR_VECTOR_IDX1
CMP_INT_PRINUM=CONFIG_DRV_CMP_INT_PRIO_NUM_IDX1/>
</#if>
<#if CONFIG_DRV_CMP_INST_IDX2 == true>
<@DRV_CMP_STATIC_FUNCTIONS DRV_INSTANCE="2" CMP_INT_SRC=CONFIG_DRV_CMP_INTERRUPT_SOURCE_IDX2
 CMP_INT_ISR=CONFIG_DRV_CMP_ISR_VECTOR_IDX2
CMP_INT_PRINUM=CONFIG_DRV_CMP_INT_PRIO_NUM_IDX2/>
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
