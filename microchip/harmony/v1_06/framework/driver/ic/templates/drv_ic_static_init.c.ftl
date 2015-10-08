<#--
/*******************************************************************************
  IC Driver Initialization call 

  Company:
    Microchip Technology Inc.

  File Name:
    drv_ic_static_init.c.ftl

  Summary:
    This template is referenced in system_init.c.ftl to call the IC driver
    initialization routine.

  Description:
    The IC device driver initialization routine sets the Input Capture functionality.
    
  Remarks:
    None.
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
<#macro DRV_IC_STATIC_API DRV_INSTANCE>

    /*Initialize IC${DRV_INSTANCE} */
    DRV_IC${DRV_INSTANCE}_Initialize();
</#macro>

<#if CONFIG_DRV_IC_INST_IDX0 == true>
<@DRV_IC_STATIC_API DRV_INSTANCE="0"/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX1 == true>
<@DRV_IC_STATIC_API DRV_INSTANCE="1"/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX2 == true>
<@DRV_IC_STATIC_API DRV_INSTANCE="2"/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX3 == true>
<@DRV_IC_STATIC_API DRV_INSTANCE="3"/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX4 == true>
<@DRV_IC_STATIC_API DRV_INSTANCE="4"/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX5 == true>
<@DRV_IC_STATIC_API DRV_INSTANCE="5"/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX6 == true>
<@DRV_IC_STATIC_API DRV_INSTANCE="6"/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX7 == true>
<@DRV_IC_STATIC_API DRV_INSTANCE="7"/>
</#if>
<#if CONFIG_DRV_IC_INST_IDX8 == true>
<@DRV_IC_STATIC_API DRV_INSTANCE="8"/>
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->