/*******************************************************************************
  CMP Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_cmp_static.c

  Summary:
    CMP driver implementation for the static single instance driver.

  Description:
    The CMP device driver provides a simple interface to manage the CMP
    modules on Microchip microcontrollers.
    
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

// *****************************************************************************
// *****************************************************************************
// Header Includes
// *****************************************************************************
// *****************************************************************************
#include "peripheral/cmp/plib_cmp.h"
<#if CONFIG_DRV_CMP_INTERRUPT_MODE == true>
#include "peripheral/int/plib_int.h"
</#if>
<#macro DRV_CMP_STATIC_FUNCTIONS DRV_INSTANCE CMP_INSTANCE NON_INVERTING_INPUT 
INVERTING_INPUT OUTPUT_ENABLE CMP_EVE_INT CMP_INT_SRC CMP_INT_VEC CMP_INT_PRI CMP_INT_SPRI OUTPUT_INVERT>
    PLIB_CMP_NonInvertingInputChannelSelect(${CMP_INSTANCE}, ${NON_INVERTING_INPUT});
    PLIB_CMP_InvertingInputChannelSelect(${CMP_INSTANCE}, ${INVERTING_INPUT});

    <#if OUTPUT_INVERT == false>
    PLIB_CMP_OutputInvertDisable(${CMP_INSTANCE});
    </#if>    
    <#if OUTPUT_ENABLE == false>
    PLIB_CMP_OutputDisable(${CMP_INSTANCE});
    </#if>

    <#if CONFIG_DRV_CMP_INTERRUPT_MODE == true>
    /* Setup Interrupt */
    PLIB_CMP_InterruptEventSelect(${CMP_INSTANCE}, ${CMP_EVE_INT});   
    PLIB_INT_SourceEnable(INT_ID_0, ${CMP_INT_SRC});
    PLIB_INT_VectorPrioritySet(INT_ID_0, ${CMP_INT_VEC}, ${CMP_INT_PRI});
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, ${CMP_INT_VEC}, ${CMP_INT_SPRI});          
    </#if>

    PLIB_CMP_Enable(${CMP_INSTANCE});
</#macro>

// *****************************************************************************
// *****************************************************************************
// Section: CMP static driver functions
// *****************************************************************************
// *****************************************************************************
void DRV_CMP_Initialize(void)
{

    <#if CONFIG_DRV_CVREF_ENABLE>	
    /* Configure CVREF for comparator use. */
    PLIB_CMP_CVREF_SourceVoltageSelect(CMP_ID_1, ${CONFIG_DRV_CMP_CVREF_VOLTAGE_SOURCE});
    <#if CONFIG_DRV_CMP_CVREF_WIDE_RANGE == true>
    PLIB_CMP_CVREF_WideRangeEnable(CMP_ID_1);
    </#if>
    PLIB_CMP_CVREF_ValueSelect(CMP_ID_1, ${CONFIG_DRV_CMP_CVREF_VALUE});
    <#if CONFIG_DRV_CMP_CVREF_OE == true>
    PLIB_CMP_CVREF_OutputEnable(CMP_ID_1);
    </#if> 
    PLIB_CMP_CVREF_Enable(CMP_ID_1);
    </#if> 

<#if CONFIG_DRV_CMP_INST_IDX0 == true>
<@DRV_CMP_STATIC_FUNCTIONS DRV_INSTANCE="0" CMP_INSTANCE=CONFIG_DRV_CMP_PERIPHERAL_ID_IDX0 
NON_INVERTING_INPUT=CONFIG_DRV_CMP_NON_INVERTING_INPUT_IDX0 
INVERTING_INPUT=CONFIG_DRV_CMP_INVERTING_INPUT_IDX0 
OUTPUT_ENABLE=CONFIG_DRV_CMP_OUTPUT_ENABLE_IDX0
CMP_EVE_INT=CONFIG_DRV_CMP_INTERRUPT_EVENT_IDX0 CMP_INT_SRC=CONFIG_DRV_CMP_INTERRUPT_SOURCE_IDX0
CMP_INT_VEC=CONFIG_DRV_CMP_INTERRUPT_VECTOR_IDX0 CMP_INT_PRI=CONFIG_DRV_CMP_INT_PRIORITY_IDX0
CMP_INT_SPRI=CONFIG_DRV_CMP_INT_SUB_PRIORITY_IDX0 OUTPUT_INVERT=CONFIG_DRV_CMP_OUTPUT_INVERT_IDX0/>
</#if>
<#if CONFIG_DRV_CMP_INST_IDX1 == true>
<@DRV_CMP_STATIC_FUNCTIONS DRV_INSTANCE="1" CMP_INSTANCE=CONFIG_DRV_CMP_PERIPHERAL_ID_IDX1 
NON_INVERTING_INPUT=CONFIG_DRV_CMP_NON_INVERTING_INPUT_IDX1 
INVERTING_INPUT=CONFIG_DRV_CMP_INVERTING_INPUT_IDX1 
OUTPUT_ENABLE=CONFIG_DRV_CMP_OUTPUT_ENABLE_IDX1
CMP_EVE_INT=CONFIG_DRV_CMP_INTERRUPT_EVENT_IDX1 CMP_INT_SRC=CONFIG_DRV_CMP_INTERRUPT_SOURCE_IDX1
CMP_INT_VEC=CONFIG_DRV_CMP_INTERRUPT_VECTOR_IDX1 CMP_INT_PRI=CONFIG_DRV_CMP_INT_PRIORITY_IDX1
CMP_INT_SPRI=CONFIG_DRV_CMP_INT_SUB_PRIORITY_IDX1 OUTPUT_INVERT=CONFIG_DRV_CMP_OUTPUT_INVERT_IDX1/>
</#if>
<#if CONFIG_DRV_CMP_INST_IDX2 == true>
<@DRV_CMP_STATIC_FUNCTIONS DRV_INSTANCE="2" CMP_INSTANCE=CONFIG_DRV_CMP_PERIPHERAL_ID_IDX2 
NON_INVERTING_INPUT=CONFIG_DRV_CMP_NON_INVERTING_INPUT_IDX2 
INVERTING_INPUT=CONFIG_DRV_CMP_INVERTING_INPUT_IDX2 
OUTPUT_ENABLE=CONFIG_DRV_CMP_OUTPUT_ENABLE_IDX2
CMP_EVE_INT=CONFIG_DRV_CMP_INTERRUPT_EVENT_IDX2 CMP_INT_SRC=CONFIG_DRV_CMP_INTERRUPT_SOURCE_IDX2
CMP_INT_VEC=CONFIG_DRV_CMP_INTERRUPT_VECTOR_IDX2 CMP_INT_PRI=CONFIG_DRV_CMP_INT_PRIORITY_IDX2
CMP_INT_SPRI=CONFIG_DRV_CMP_INT_SUB_PRIORITY_IDX2 OUTPUT_INVERT=CONFIG_DRV_CMP_OUTPUT_INVERT_IDX2/>
</#if>

}

/*******************************************************************************
 End of File
*/
