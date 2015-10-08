<#--
/*******************************************************************************
  MPLAB Harmony Sample Library Module Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init.call.c.ftl

  Summary:
    MPLAB Harmony Sample Library Module Freemarker Template File for
    system_init.c call to the module's Initialize function.

  Description:
    This file defines the template for the call to the module's initialization 
    function that will be inserted into the SYS_Initialize funciton in the 
    system_init.c file when the sample library module is used.
 *******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

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
<#if CONFIG_USE_SAMPLE_FUNC_TEST != true>
<#-- Instance 0 -->
<#if CONFIG_SAMPLE_MODULE_INST_IDX0 == true>
    sysObj.sampleModule0 = SAMPLE_Initialize(SAMPLE_INDEX_0, (SYS_MODULE_INIT *)&sampleModule0InitData);
</#if>
<#-- Instance 1 -->
<#if CONFIG_SAMPLE_MODULE_INST_IDX1 == true>
    sysObj.sampleModule1 = SAMPLE_Initialize(SAMPLE_INDEX_1, (SYS_MODULE_INIT *)&sampleModule1InitData);
</#if>
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->
