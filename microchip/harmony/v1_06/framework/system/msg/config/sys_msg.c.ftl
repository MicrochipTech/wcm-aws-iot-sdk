<#--
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
/*** Message System Initialization Data ***/

<#if CONFIG_SYS_MSG_INST_IDX0 == true>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "0">
static uint16_t queuePriorities0[1] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "1">    
static uint16_t queuePriorities0[2] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "2">    
static uint16_t queuePriorities0[3] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "3">    
static uint16_t queuePriorities0[4] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "4">    
static uint16_t queuePriorities0[5] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3}, ${CONFIG_SYS_MSG_BUFFER_SIZES4} };
</#if>
SYS_MSG_INIT msg0Init =
{
    .nMaxMsgsDelivered = ${CONFIG_SYS_MSG_MAX_MSGS_DELIVERED},
    .nMessagePriorities = ${CONFIG_SYS_MSG_MAX_PRIORITY},
};
</#if>
<#if CONFIG_SYS_MSG_INST_IDX1 == true>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "0">    
static uint16_t queuePriorities1[1] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "1">    
static uint16_t queuePriorities1[2] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "2">    
static uint16_t queuePriorities1[3] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "3">    
static uint16_t queuePriorities1[4] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "4">    
static uint16_t queuePriorities1[5] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3}, ${CONFIG_SYS_MSG_BUFFER_SIZES4} };
</#if>
SYS_MSG_INIT msg1Init =
{
    .nMaxMsgsDelivered = ${CONFIG_SYS_MSG_MAX_MSGS_DELIVERED},
    .nMessagePriorities = ${CONFIG_SYS_MSG_MAX_PRIORITY},
};
</#if>
<#if CONFIG_SYS_MSG_INST_IDX2 == true>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "0">    
static uint16_t queuePriorities2[1] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "1">    
static uint16_t queuePriorities2[2] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "2">    
static uint16_t queuePriorities2[3] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "3">    
static uint16_t queuePriorities2[4] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "4">    
static uint16_t queuePriorities2[5] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3}, ${CONFIG_SYS_MSG_BUFFER_SIZES4} };
</#if>
SYS_MSG_INIT msg2Init =
{
    .nMaxMsgsDelivered = ${CONFIG_SYS_MSG_MAX_MSGS_DELIVERED},
    .nMessagePriorities = ${CONFIG_SYS_MSG_MAX_PRIORITY},
};
</#if>
<#if CONFIG_SYS_MSG_INST_IDX3 == true>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "0">    
static uint16_t queuePriorities3[1] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "1">    
static uint16_t queuePriorities3[2] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "2">    
static uint16_t queuePriorities3[3] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "3">    
static uint16_t queuePriorities3[4] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "4">    
static uint16_t queuePriorities3[5] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3}, ${CONFIG_SYS_MSG_BUFFER_SIZES4} };
</#if>
SYS_MSG_INIT msg3Init =
{
    .nMaxMsgsDelivered = ${CONFIG_SYS_MSG_MAX_MSGS_DELIVERED},
    .nMessagePriorities = ${CONFIG_SYS_MSG_MAX_PRIORITY},
};

</#if>
<#if CONFIG_SYS_MSG_INST_IDX4 == true>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "0">    
static uint16_t queuePriorities4[1] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "1">    
static uint16_t queuePriorities4[2] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "2">    
static uint16_t queuePriorities4[3] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "3">    
static uint16_t queuePriorities4[4] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3} };
</#if>
<#if CONFIG_SYS_MSG_MAX_PRIORITY = "4">    
static uint16_t queuePriorities4[5] = { ${CONFIG_SYS_MSG_BUFFER_SIZES0}, ${CONFIG_SYS_MSG_BUFFER_SIZES1}, ${CONFIG_SYS_MSG_BUFFER_SIZES2}, ${CONFIG_SYS_MSG_BUFFER_SIZES3}, ${CONFIG_SYS_MSG_BUFFER_SIZES4} };
</#if>
SYS_MSG_INIT msg4Init =
{
    .nMaxMsgsDelivered = ${CONFIG_SYS_MSG_MAX_MSGS_DELIVERED},
    .nMessagePriorities = ${CONFIG_SYS_MSG_MAX_PRIORITY},
};

</#if>


<#--
/*******************************************************************************
 End of File
*/
-->
