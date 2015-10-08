/*******************************************************************************
  Timer Static Driver File

  File Name:
    drv_tmr_static.c

  Company:
    Microchip Technology Inc.	

  Summary:
    Timer driver implementation for the static single instance driver.

  Description:
    The Timer device driver provides a simple interface to manage the Timer
    modules on Microchip microcontrollers.
    
  Remarks:
    None
 *******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Mirochip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a MTMRrochip mTMRrocontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublTMRense terms in the accompanying lTMRense agreement).

You should refer to the lTMRense agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTTMRULAR PURPOSE.
IN NO EVENT SHALL MTMRROCHIP OR ITS LTMRENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRTMRT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVTMRES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Header Includes
// *****************************************************************************
// *****************************************************************************
#include "framework/driver/tmr/drv_tmr_static.h"

 <#if CONFIG_USE_DRV_TMR == true>
<#if CONFIG_DRV_TMR_DRIVER_MODE == "STATIC">
<#macro DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE TMR_INSTANCE TMR_CLKSRC TMR_PRESCALE
TMR_MODE TMR_PERIOD TMR_INT_SRC TMR_INT_VEC TMR_INT_PRI TMR_INT_SPRI>
// *****************************************************************************
// *****************************************************************************
// Section: Instance ${DRV_INSTANCE} static driver functions
// *****************************************************************************
// *****************************************************************************
void DRV_TMR${DRV_INSTANCE}_Initialize(void)
{	
    /* Initialize Timer Instance${DRV_INSTANCE} */
    /* Disable Timer */
    PLIB_TMR_Stop(${TMR_INSTANCE});
    /* Select clock source */
    PLIB_TMR_ClockSourceSelect(${TMR_INSTANCE}, ${TMR_CLKSRC});
    /* Select prescalar value */
    PLIB_TMR_PrescaleSelect(${TMR_INSTANCE}, ${TMR_PRESCALE});
<#if TMR_MODE == "DRV_TMR_OPERATION_MODE_32_BIT">
    /* Enable 32 bit mode */
    PLIB_TMR_Mode32BitEnable(${TMR_INSTANCE});
    /* Clear counter */
    PLIB_TMR_Counter32BitClear(${TMR_INSTANCE});
    /*Set period */	
    PLIB_TMR_Period32BitSet(${TMR_INSTANCE}, ${TMR_PERIOD});
<#else>
    /* Enable 16 bit mode */
    PLIB_TMR_Mode16BitEnable(${TMR_INSTANCE});
    /* Clear counter */	
    PLIB_TMR_Counter16BitClear(${TMR_INSTANCE});
    /*Set period */	
    PLIB_TMR_Period16BitSet(${TMR_INSTANCE}, ${TMR_PERIOD});
</#if>

<#if CONFIG_DRV_TMR_INTERRUPT_MODE == true>
    /* Setup Interrupt */   
    PLIB_INT_SourceEnable(INT_ID_0, ${TMR_INT_SRC});
    PLIB_INT_VectorPrioritySet(INT_ID_0, ${TMR_INT_VEC}, ${TMR_INT_PRI});
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, ${TMR_INT_VEC}, ${TMR_INT_SPRI});          
</#if>
}

inline void DRV_TMR${DRV_INSTANCE}_Start(void)
{
    /* Start Timer*/
    PLIB_TMR_Start(${TMR_INSTANCE});
}

inline void DRV_TMR${DRV_INSTANCE}_Stop(void)
{
    /* Stop Timer*/
    PLIB_TMR_Stop(${TMR_INSTANCE});
}

inline void DRV_TMR${DRV_INSTANCE}_CounterClear(void)
{
<#if TMR_MODE == "DRV_TMR_OPERATION_MODE_32_BIT">
    /* Clear 32-bit counter value */
    PLIB_TMR_Counter32BitClear(${TMR_INSTANCE});
<#else>
    /* Clear 16-bit counter value */
    PLIB_TMR_Counter16BitClear(${TMR_INSTANCE});    
</#if>
}

void DRV_TMR${DRV_INSTANCE}_CounterValueSet(uint32_t value)
{
<#if TMR_MODE == "DRV_TMR_OPERATION_MODE_32_BIT">
    /* Set 32-bit counter value*/
    PLIB_TMR_Counter32BitSet(${TMR_INSTANCE}, value);
<#else>
    /* Set 16-bit counter value*/
    PLIB_TMR_Counter16BitSet(${TMR_INSTANCE}, (uint16_t)value);
</#if>
}

uint32_t DRV_TMR${DRV_INSTANCE}_CounterValueGet(void)
{
<#if TMR_MODE == "DRV_TMR_OPERATION_MODE_32_BIT">
    /* Get 32-bit counter value*/
    return PLIB_TMR_Counter32BitGet(${TMR_INSTANCE});
<#else>
    /* Get 16-bit counter value*/
    return (uint32_t) PLIB_TMR_Counter16BitGet(${TMR_INSTANCE});
</#if>
}

</#macro>
<#if CONFIG_DRV_TMR_INST_0 == true>
<@DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE="0" TMR_INSTANCE=CONFIG_DRV_TMR_PERIPHERAL_ID_IDX0
TMR_CLKSRC=CONFIG_DRV_TMR_CLOCK_SOURCE_ST_IDX0 TMR_PRESCALE=CONFIG_DRV_TMR_PRESCALE_IDX0
TMR_MODE=CONFIG_DRV_TMR_OPERATION_MODE_IDX0 TMR_PERIOD=CONFIG_DRV_TMR_PERIOD_IDX0
TMR_INT_SRC=CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX0 TMR_INT_VEC=CONFIG_DRV_TMR_INTERRUPT_VECTOR_IDX0 
TMR_INT_PRI=CONFIG_DRV_TMR_INTERRUPT_PRIORITY_IDX0 TMR_INT_SPRI=CONFIG_DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX0/>
</#if>
<#if CONFIG_DRV_TMR_INST_1 == true>
<@DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE="1" TMR_INSTANCE=CONFIG_DRV_TMR_PERIPHERAL_ID_IDX1 
TMR_CLKSRC=CONFIG_DRV_TMR_CLOCK_SOURCE_ST_IDX1 TMR_PRESCALE=CONFIG_DRV_TMR_PRESCALE_IDX1
TMR_MODE=CONFIG_DRV_TMR_OPERATION_MODE_IDX1 TMR_PERIOD=CONFIG_DRV_TMR_PERIOD_IDX1
TMR_INT_SRC=CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX1 TMR_INT_VEC=CONFIG_DRV_TMR_INTERRUPT_VECTOR_IDX1 
TMR_INT_PRI=CONFIG_DRV_TMR_INTERRUPT_PRIORITY_IDX1 TMR_INT_SPRI=CONFIG_DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX1/>
</#if>
<#if CONFIG_DRV_TMR_INST_2 == true>
<@DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE="2" TMR_INSTANCE=CONFIG_DRV_TMR_PERIPHERAL_ID_IDX2 
TMR_CLKSRC=CONFIG_DRV_TMR_CLOCK_SOURCE_ST_IDX2 TMR_PRESCALE=CONFIG_DRV_TMR_PRESCALE_IDX2
TMR_MODE=CONFIG_DRV_TMR_OPERATION_MODE_IDX2 TMR_PERIOD=CONFIG_DRV_TMR_PERIOD_IDX2
TMR_INT_SRC=CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX2 TMR_INT_VEC=CONFIG_DRV_TMR_INTERRUPT_VECTOR_IDX2 
TMR_INT_PRI=CONFIG_DRV_TMR_INTERRUPT_PRIORITY_IDX2 TMR_INT_SPRI=CONFIG_DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX2/>
</#if>
<#if CONFIG_DRV_TMR_INST_3 == true>
<@DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE="3" TMR_INSTANCE=CONFIG_DRV_TMR_PERIPHERAL_ID_IDX3 
TMR_CLKSRC=CONFIG_DRV_TMR_CLOCK_SOURCE_ST_IDX3 TMR_PRESCALE=CONFIG_DRV_TMR_PRESCALE_IDX3
TMR_MODE=CONFIG_DRV_TMR_OPERATION_MODE_IDX3 TMR_PERIOD=CONFIG_DRV_TMR_PERIOD_IDX3
TMR_INT_SRC=CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX3 TMR_INT_VEC=CONFIG_DRV_TMR_INTERRUPT_VECTOR_IDX3 
TMR_INT_PRI=CONFIG_DRV_TMR_INTERRUPT_PRIORITY_IDX3 TMR_INT_SPRI=CONFIG_DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX3/>
</#if>
<#if CONFIG_DRV_TMR_INST_4 == true>
<@DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE="4" TMR_INSTANCE=CONFIG_DRV_TMR_PERIPHERAL_ID_IDX4 
TMR_CLKSRC=CONFIG_DRV_TMR_CLOCK_SOURCE_ST_IDX4 TMR_PRESCALE=CONFIG_DRV_TMR_PRESCALE_IDX4
TMR_MODE=CONFIG_DRV_TMR_OPERATION_MODE_IDX4 TMR_PERIOD=CONFIG_DRV_TMR_PERIOD_IDX4
TMR_INT_SRC=CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX4 TMR_INT_VEC=CONFIG_DRV_TMR_INTERRUPT_VECTOR_IDX4 
TMR_INT_PRI=CONFIG_DRV_TMR_INTERRUPT_PRIORITY_IDX4 TMR_INT_SPRI=CONFIG_DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX4/>
</#if>
<#if CONFIG_DRV_TMR_INST_5 == true>
<@DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE="5" TMR_INSTANCE=CONFIG_DRV_TMR_PERIPHERAL_ID_IDX5 
TMR_CLKSRC=CONFIG_DRV_TMR_CLOCK_SOURCE_ST_IDX5 TMR_PRESCALE=CONFIG_DRV_TMR_PRESCALE_IDX5
TMR_MODE=CONFIG_DRV_TMR_OPERATION_MODE_IDX5 TMR_PERIOD=CONFIG_DRV_TMR_PERIOD_IDX5
TMR_INT_SRC=CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX5 TMR_INT_VEC=CONFIG_DRV_TMR_INTERRUPT_VECTOR_IDX5 
TMR_INT_PRI=CONFIG_DRV_TMR_INTERRUPT_PRIORITY_IDX5 TMR_INT_SPRI=CONFIG_DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX5/>
</#if>
<#if CONFIG_DRV_TMR_INST_6 == true>
<@DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE="6" TMR_INSTANCE=CONFIG_DRV_TMR_PERIPHERAL_ID_IDX6 
TMR_CLKSRC=CONFIG_DRV_TMR_CLOCK_SOURCE_ST_IDX6 TMR_PRESCALE=CONFIG_DRV_TMR_PRESCALE_IDX6
TMR_MODE=CONFIG_DRV_TMR_OPERATION_MODE_IDX6 TMR_PERIOD=CONFIG_DRV_TMR_PERIOD_IDX6
TMR_INT_SRC=CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX6 TMR_INT_VEC=CONFIG_DRV_TMR_INTERRUPT_VECTOR_IDX6 
TMR_INT_PRI=CONFIG_DRV_TMR_INTERRUPT_PRIORITY_IDX6 TMR_INT_SPRI=CONFIG_DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX6/>
</#if>
<#if CONFIG_DRV_TMR_INST_7 == true>
<@DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE="7" TMR_INSTANCE=CONFIG_DRV_TMR_PERIPHERAL_ID_IDX7 
TMR_CLKSRC=CONFIG_DRV_TMR_CLOCK_SOURCE_ST_IDX7 TMR_PRESCALE=CONFIG_DRV_TMR_PRESCALE_IDX7
TMR_MODE=CONFIG_DRV_TMR_OPERATION_MODE_IDX7 TMR_PERIOD=CONFIG_DRV_TMR_PERIOD_IDX7
TMR_INT_SRC=CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX7 TMR_INT_VEC=CONFIG_DRV_TMR_INTERRUPT_VECTOR_IDX7 
TMR_INT_PRI=CONFIG_DRV_TMR_INTERRUPT_PRIORITY_IDX7 TMR_INT_SPRI=CONFIG_DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX7/>
</#if>
<#if CONFIG_DRV_TMR_INST_8 == true>
<@DRV_TMR_STATIC_FUNCTIONS DRV_INSTANCE="8" TMR_INSTANCE=CONFIG_DRV_TMR_PERIPHERAL_ID_IDX8 
TMR_CLKSRC=CONFIG_DRV_TMR_CLOCK_SOURCE_ST_IDX8 TMR_PRESCALE=CONFIG_DRV_TMR_PRESCALE_IDX8
TMR_MODE=CONFIG_DRV_TMR_OPERATION_MODE_IDX8 TMR_PERIOD=CONFIG_DRV_TMR_PERIOD_IDX8
TMR_INT_SRC=CONFIG_DRV_TMR_INTERRUPT_SOURCE_IDX8 TMR_INT_VEC=CONFIG_DRV_TMR_INTERRUPT_VECTOR_IDX8 
TMR_INT_PRI=CONFIG_DRV_TMR_INTERRUPT_PRIORITY_IDX8 TMR_INT_SPRI=CONFIG_DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX8/>
</#if>
</#if> <#--CONFIG_USE_DRV_TMR == true-->
</#if> <#--CONFIG_DRV_TMR_DRIVER_MODE == "STATIC"-->
/*******************************************************************************
 End of File
*/
