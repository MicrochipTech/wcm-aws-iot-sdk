<#--
/*******************************************************************************
  MPLAB Harmony Sample Library Module Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.ftl

  Summary:
    MPLAB Harmony Sample Library Module Freemarker Template File for
    system_config.h

  Description:
    This file defines the template for the system configuration entries 
    inserted into the system_config.h file when the sample library module is 
    used.
*******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
-->
<#if CONFIG_USE_SAMPLE_MODULE == true>
/******************************************************************************
  Sample Library Module Configuration Options
  
  <editor-fold defaultstate="collapsed"
   desc="Sample Library Module Configuration Options">
*/
    
#define SAMPLE_MODULE_INSTANCES_NUMBER  ${CONFIG_SAMPLE_MODULE_INSTANCES_NUMBER}
<#if CONFIG_SAMPLE_MODULE_INTERRUPT_MODE == true>
#define SAMPLE_MODULE_INTERRUPT_MODE    true
<#else>
#define SAMPLE_MODULE_INTERRUPT_MODE    false
</#if>
#define SAMPLE_MODULE_TIMEOUT           ${CONFIG_SAMPLE_MODULE_TIMEOUT}

<#-- Instance 0 -->
<#if CONFIG_SAMPLE_MODULE_INST_IDX0 == true>
#define SAMPLE_MODULE_POWER_STATE_IDX0  ${CONFIG_SAMPLE_MODULE_POWER_STATE_IDX0}
#define SAMPLE_MODULE_SOME_DATA_IDX0    ${CONFIG_SAMPLE_MODULE_SOME_DATA_IDX0}
<#if CONFIG_SAMPLE_MODULE_INTERRUPT_MODE == true>
#define SAMPLE_MODULE_TIMER_ID_IDX0                     ${CONFIG_SAMPLE_MODULE_TIMER_ID_IDX0}
#define SAMPLE_MODULE_TIMER_CLOCK_PRESCALER_IDX0        ${CONFIG_SAMPLE_MODULE_TIMER_CLOCK_PRESCALER_IDX0}
#define SAMPLE_MODULE_TIMER_PERIOD_IDX0                 ${CONFIG_SAMPLE_MODULE_TIMER_PERIOD_IDX0}
#define SAMPLE_MODULE_INTERRUPT_SOURCE_IDX0             ${CONFIG_SAMPLE_MODULE_INTERRUPT_SOURCE_IDX0}
#define SAMPLE_MODULE_INTERRUPT_VECTOR_IDX0             ${CONFIG_SAMPLE_MODULE_INTERRUPT_VECTOR_IDX0}
#define SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX0           ${CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX0}
#define SAMPLE_MODULE_TIMER_INTERRUPT_SUBPRIORITY_IDX0  ${CONFIG_SAMPLE_MODULE_TIMER_INTERRUPT_SUBPRIORITY_IDX0}
</#if>

</#if>
<#-- Instance 1 -->
<#if CONFIG_SAMPLE_MODULE_INST_IDX1 == true>
#define SAMPLE_MODULE_POWER_STATE_IDX1  ${CONFIG_SAMPLE_MODULE_POWER_STATE_IDX1}
#define SAMPLE_MODULE_SOME_DATA_IDX1    ${CONFIG_SAMPLE_MODULE_SOME_DATA_IDX1}
<#if CONFIG_SAMPLE_MODULE_INTERRUPT_MODE == true>
#define SAMPLE_MODULE_TIMER_ID_IDX1                     ${CONFIG_SAMPLE_MODULE_TIMER_ID_IDX1}
#define SAMPLE_MODULE_TIMER_CLOCK_PRESCALER_IDX1        ${CONFIG_SAMPLE_MODULE_TIMER_CLOCK_PRESCALER_IDX1}
#define SAMPLE_MODULE_TIMER_PERIOD_IDX1                 ${CONFIG_SAMPLE_MODULE_TIMER_PERIOD_IDX1}
#define SAMPLE_MODULE_INTERRUPT_SOURCE_IDX1             ${CONFIG_SAMPLE_MODULE_INTERRUPT_SOURCE_IDX1}
#define SAMPLE_MODULE_INTERRUPT_VECTOR_IDX1             ${CONFIG_SAMPLE_MODULE_INTERRUPT_VECTOR_IDX1}
#define SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX1           ${CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX1}
#define SAMPLE_MODULE_TIMER_INTERRUPT_SUBPRIORITY_IDX1  ${CONFIG_SAMPLE_MODULE_TIMER_INTERRUPT_SUBPRIORITY_IDX0}
</#if>
</#if>
// </editor-fold>

</#if>
<#--
/*******************************************************************************
 End of File
*/
-->
