/*******************************************************************************
  SYS PORTS Static Functions for PORTS System Service

  Company:
    Microchip Technology Inc.

  File Name:
    sys_ports_static.c

  Summary:
    SYS PORTS static function implementations for the Ports System Service.

  Description:
    The Ports System Service provides a simple interface to manage the ports
    on Microchip microcontrollers. This file defines the static implementation for the 
    Ports System Service.
    
  Remarks:
    Static functions incorporate all system ports configuration settings as
    determined by the user via the Microchip Harmony Configurator GUI.  It provides 
    static version of the routines, eliminating the need for an object ID or 
    object handle.

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

#include "system_config.h"
#include "peripheral/ports/plib_ports.h"
#include "peripheral/int/plib_int.h"

<#macro PLIB_PPS_PORTS_APIs PORT_CHANNEL SYS_PORT_TRIS SYS_PORT_LAT SYS_PORT_ODC SYS_PORT_ANSEL SYS_PORT_CNEN SYS_PORT_CNPU SYS_PORT_CNPD>
    PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, ${PORT_CHANNEL}, ${SYS_PORT_ODC});
    PLIB_PORTS_Write( PORTS_ID_0, ${PORT_CHANNEL},  ${SYS_PORT_LAT});
    PLIB_PORTS_DirectionOutputSet( PORTS_ID_0, ${PORT_CHANNEL},  ${SYS_PORT_TRIS} ^ 0xFFFF);
    PLIB_PORTS_ChangeNoticePerPortTurnOn(PORTS_ID_0, ${PORT_CHANNEL});
    PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, ${PORT_CHANNEL}, ${SYS_PORT_ANSEL} ^ 0xFFFF, PORTS_PIN_MODE_DIGITAL);
    PLIB_PORTS_ChannelChangeNoticeEnable(PORTS_ID_0, ${PORT_CHANNEL}, ${SYS_PORT_CNEN});
    PLIB_PORTS_ChannelChangeNoticePullUpEnable(PORTS_ID_0, ${PORT_CHANNEL}, ${SYS_PORT_CNPU});
    PLIB_PORTS_ChannelChangeNoticePullDownEnable(PORTS_ID_0, ${PORT_CHANNEL}, ${SYS_PORT_CNPD});
</#macro>

<#macro PLIB_PPS_PORTS_INT_APIs INT_CHANNEL INT_CHANNEL_VECTOR PORT_CHANNEL>
    PLIB_PORTS_Read(PORTS_ID_0, ${PORT_CHANNEL});    
    PLIB_INT_SourceFlagClear(INT_ID_0, ${INT_CHANNEL});         
    PLIB_INT_SourceEnable(INT_ID_0, ${INT_CHANNEL});
<#if CONFIG_PIC32MZ == true>
    PLIB_INT_VectorPrioritySet(INT_ID_0, ${INT_CHANNEL_VECTOR}, ${CONFIG_SYS_PORTS_CN_INTERRUPT_PRIORITY});
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, ${INT_CHANNEL_VECTOR}, ${CONFIG_SYS_PORTS_CN_INTERRUPT_SUB_PRIORITY});
</#if>
</#macro>

<#macro PLIB_NON_PPS_PORTS_APIs PORT_CHANNEL SYS_PORT_TRIS SYS_PORT_LAT SYS_PORT_ODC>
    PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, ${PORT_CHANNEL}, ${SYS_PORT_ODC});
    PLIB_PORTS_Write( PORTS_ID_0, ${PORT_CHANNEL},  ${SYS_PORT_LAT});
	PLIB_PORTS_DirectionOutputSet( PORTS_ID_0, ${PORT_CHANNEL},  ${SYS_PORT_TRIS} ^ 0xFFFF);
</#macro>

void SYS_PORTS_Initialize(void)
{
    <#if CONFIG_HAVE_PPS == true>
        <#if CONFIG_USE_PORT_A == true>
    /* PORT A Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_A"
                SYS_PORT_TRIS = "SYS_PORT_A_TRIS"
                SYS_PORT_LAT = "SYS_PORT_A_LAT"
                SYS_PORT_ODC = "SYS_PORT_A_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_A_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_A_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_A_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_A_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_A_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_A"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_A"
                PORT_CHANNEL = "PORT_CHANNEL_A"
            />
            </#if>
        </#if>
        <#if CONFIG_USE_PORT_B == true>
    
    /* PORT B Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_B"
                SYS_PORT_TRIS = "SYS_PORT_B_TRIS"
                SYS_PORT_LAT = "SYS_PORT_B_LAT"
                SYS_PORT_ODC = "SYS_PORT_B_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_B_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_B_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_B_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_B_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_B_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_B"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_B"
                PORT_CHANNEL = "PORT_CHANNEL_B"
            />
            </#if>
        </#if>
        <#if CONFIG_USE_PORT_C == true>
    
    /* PORT C Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_C"
                SYS_PORT_TRIS = "SYS_PORT_C_TRIS"
                SYS_PORT_LAT = "SYS_PORT_C_LAT"
                SYS_PORT_ODC = "SYS_PORT_C_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_C_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_C_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_C_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_C_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_C_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_C"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_C"
                PORT_CHANNEL = "PORT_CHANNEL_C"
            />
            </#if>
        </#if>
        <#if CONFIG_USE_PORT_D == true>
    
    /* PORT D Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_D"
                SYS_PORT_TRIS = "SYS_PORT_D_TRIS"
                SYS_PORT_LAT = "SYS_PORT_D_LAT"
                SYS_PORT_ODC = "SYS_PORT_D_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_D_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_D_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_D_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_D_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_D_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_D"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_D"
                PORT_CHANNEL = "PORT_CHANNEL_D"
            />
            </#if>
        </#if>
        <#if CONFIG_USE_PORT_E == true>
    
    /* PORT E Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_E"
                SYS_PORT_TRIS = "SYS_PORT_E_TRIS"
                SYS_PORT_LAT = "SYS_PORT_E_LAT"
                SYS_PORT_ODC = "SYS_PORT_E_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_E_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_E_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_E_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_E_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_E_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_E"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_E"
                PORT_CHANNEL = "PORT_CHANNEL_E"
            />
            </#if>
        </#if>   
        <#if CONFIG_USE_PORT_F == true>
    
    /* PORT F Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_F"
                SYS_PORT_TRIS = "SYS_PORT_F_TRIS"
                SYS_PORT_LAT = "SYS_PORT_F_LAT"
                SYS_PORT_ODC = "SYS_PORT_F_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_F_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_F_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_F_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_F_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_F_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_F"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_F"
                PORT_CHANNEL = "PORT_CHANNEL_F"
            />
            </#if>
        </#if>   
        <#if CONFIG_USE_PORT_G == true>
    
    /* PORT G Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_G"
                SYS_PORT_TRIS = "SYS_PORT_G_TRIS"
                SYS_PORT_LAT = "SYS_PORT_G_LAT"
                SYS_PORT_ODC = "SYS_PORT_G_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_G_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_G_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_G_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_G_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_G_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_G"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_G"
                PORT_CHANNEL = "PORT_CHANNEL_G"
            />
            </#if>
        </#if>
        <#if CONFIG_USE_PORT_H == true>
    
    /* PORT H Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_H"
                SYS_PORT_TRIS = "SYS_PORT_H_TRIS"
                SYS_PORT_LAT = "SYS_PORT_H_LAT"
                SYS_PORT_ODC = "SYS_PORT_H_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_H_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_H_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_H_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_H_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_H_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_H"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_H"
                PORT_CHANNEL = "PORT_CHANNEL_H"
            />
            </#if>
        </#if>
        <#if CONFIG_USE_PORT_J == true>
    
    /* PORT J Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_J"
                SYS_PORT_TRIS = "SYS_PORT_J_TRIS"
                SYS_PORT_LAT = "SYS_PORT_J_LAT"
                SYS_PORT_ODC = "SYS_PORT_J_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_J_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_J_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_J_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_J_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_J_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_J"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_J"
                PORT_CHANNEL = "PORT_CHANNEL_J"
            />
            </#if>
        </#if>
        <#if CONFIG_USE_PORT_K == true>
    
    /* PORT K Initialization */
            <@PLIB_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_K"
                SYS_PORT_TRIS = "SYS_PORT_K_TRIS"
                SYS_PORT_LAT = "SYS_PORT_K_LAT"
                SYS_PORT_ODC = "SYS_PORT_K_ODC"
                SYS_PORT_ANSEL = "SYS_PORT_K_ANSEL"
                SYS_PORT_CNEN = "SYS_PORT_K_CNEN"
                SYS_PORT_CNPU = "SYS_PORT_K_CNPU"
                SYS_PORT_CNPD = "SYS_PORT_K_CNPD"
            />
            <#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_K_CNEN != '0x0'>
            <@PLIB_PPS_PORTS_INT_APIs 
                INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_K"
                INT_CHANNEL_VECTOR = "INT_VECTOR_CHANGE_NOTICE_K"
                PORT_CHANNEL = "PORT_CHANNEL_K"
            />
            </#if>
        </#if>

        <#if CONFIG_PIC32MX == true && CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true>
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_CN, ${CONFIG_SYS_PORTS_CN_INTERRUPT_PRIORITY});
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_CN, ${CONFIG_SYS_PORTS_CN_INTERRUPT_SUB_PRIORITY});
        </#if>

    /* PPS Input Remapping */
<#macro PPS_INPUT USE_PPS PPS_IN_FUNCTION PPS_IN_PIN>
<#assign res = PPS_IN_FUNCTION ?matches(r"(\w+) \((\w+)\)")>
<#if USE_PPS == true>
<#if res>
    PLIB_PORTS_RemapInput(PORTS_ID_0, ${res?groups[1]}, ${PPS_IN_PIN} );
<#else>
    PLIB_PORTS_RemapInput(PORTS_ID_0, ${PPS_IN_FUNCTION}, ${PPS_IN_PIN} );
</#if>
</#if>
</#macro>
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_0 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_0 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_0 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_1 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_1 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_1 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_2 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_2 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_2 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_3 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_3 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_3 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_4 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_4 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_4 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_5 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_5 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_5 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_6 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_6 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_6 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_7 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_7 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_7 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_8 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_8 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_8 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_9 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_9 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_9 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_10 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_10 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_10 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_11 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_11 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_11 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_12 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_12 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_12 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_13 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_13 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_13 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_14 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_14 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_14 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_15 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_15 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_15 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_16 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_16 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_16 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_17 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_17 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_17 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_18 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_18 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_18 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_19 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_19 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_19 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_20 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_20 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_20 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_21 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_21 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_21 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_22 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_22 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_22 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_23 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_23 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_23 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_24 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_24 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_24 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_25 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_25 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_25 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_26 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_26 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_26 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_27 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_27 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_27 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_28 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_28 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_28 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_29 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_29 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_29 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_30 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_30 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_30 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_31 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_31 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_31 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_32 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_32 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_32 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_33 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_33 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_33 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_34 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_34 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_34 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_35 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_35 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_35 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_36 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_36 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_36 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_37 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_37 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_37 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_38 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_38 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_38 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_39 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_39 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_39 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_40 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_40 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_40 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_41 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_41 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_41 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_42 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_42 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_42 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_43 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_43 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_43 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_44 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_44 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_44 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_45 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_45 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_45 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_46 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_46 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_46 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_47 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_47 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_47 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_48 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_48 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_48 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_49 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_49 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_49 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_50 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_50 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_50 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_51 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_51 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_51 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_52 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_52 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_52 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_53 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_53 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_53 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_54 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_54 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_54 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_55 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_55 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_55 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_56 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_56 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_56 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_57 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_57 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_57 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_58 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_58 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_58 />
 <@PPS_INPUT USE_PPS=CONFIG_USE_PPS_INPUT_59 PPS_IN_FUNCTION=CONFIG_SYS_PORT_PPS_INPUT_FUNCTION_59 PPS_IN_PIN=CONFIG_SYS_PORT_PPS_INPUT_PIN_59 />

    /* PPS Output Remapping */
<#macro PPS_OUTPUT USE_PPS PPS_OUT_FUNCTION PPS_OUT_PIN>
<#assign res = PPS_OUT_FUNCTION ?matches(r"(\w+) \((\w+)\)")>
<#if USE_PPS == true>
<#if res>
    PLIB_PORTS_RemapOutput(PORTS_ID_0, ${res?groups[1]}, ${PPS_OUT_PIN} );
<#else>
    PLIB_PORTS_RemapOutput(PORTS_ID_0, ${PPS_OUT_FUNCTION}, ${PPS_OUT_PIN} );
</#if>
</#if>
</#macro>
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_0 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_0 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_0 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_1 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_1 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_1 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_2 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_2 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_2 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_3 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_3 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_3 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_4 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_4 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_4 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_5 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_5 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_5 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_6 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_6 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_6 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_7 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_7 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_7 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_8 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_8 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_8 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_9 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_9 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_9 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_10 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_10 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_10 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_11 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_11 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_11 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_12 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_12 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_12 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_13 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_13 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_13 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_14 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_14 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_14 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_15 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_15 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_15 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_16 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_16 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_16 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_17 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_17 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_17 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_18 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_18 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_18 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_19 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_19 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_19 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_20 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_20 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_20 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_21 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_21 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_21 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_22 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_22 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_22 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_23 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_23 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_23 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_24 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_24 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_24 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_25 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_25 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_25 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_26 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_26 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_26 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_27 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_27 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_27 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_28 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_28 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_28 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_29 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_29 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_29 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_30 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_30 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_30 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_31 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_31 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_31 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_32 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_32 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_32 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_33 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_33 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_33 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_34 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_34 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_34 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_35 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_35 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_35 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_36 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_36 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_36 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_37 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_37 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_37 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_38 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_38 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_38 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_39 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_39 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_39 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_40 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_40 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_40 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_41 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_41 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_41 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_42 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_42 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_42 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_43 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_43 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_43 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_44 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_44 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_44 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_45 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_45 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_45 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_46 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_46 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_46 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_47 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_47 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_47 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_48 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_48 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_48 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_49 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_49 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_49 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_50 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_50 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_50 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_51 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_51 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_51 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_52 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_52 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_52 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_53 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_53 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_53 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_54 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_54 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_54 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_55 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_55 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_55 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_56 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_56 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_56 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_57 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_57 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_57 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_58 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_58 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_58 />
 <@PPS_OUTPUT USE_PPS=CONFIG_USE_PPS_OUTPUT_59 PPS_OUT_FUNCTION=CONFIG_SYS_PORT_PPS_OUTPUT_FUNCTION_59 PPS_OUT_PIN=CONFIG_SYS_PORT_PPS_OUTPUT_PIN_59 />

    
    <#else>
    /* AN and CN Pins Initialization */
    PLIB_PORTS_AnPinsModeSelect(PORTS_ID_0, SYS_PORT_AD1PCFG, PORTS_PIN_MODE_DIGITAL);
    PLIB_PORTS_CnPinsPullUpEnable(PORTS_ID_0, SYS_PORT_CNPUE);
    PLIB_PORTS_CnPinsEnable(PORTS_ID_0, SYS_PORT_CNEN);
    PLIB_PORTS_ChangeNoticeEnable(PORTS_ID_0);

<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true>
<#if CONFIG_USE_PORT_A == true>
    PLIB_PORTS_Read(PORTS_ID_0, PORT_CHANNEL_A);    
</#if>
<#if CONFIG_USE_PORT_B == true>
    PLIB_PORTS_Read(PORTS_ID_0, PORT_CHANNEL_B);    
</#if>
<#if CONFIG_USE_PORT_C == true>
    PLIB_PORTS_Read(PORTS_ID_0, PORT_CHANNEL_C);    
</#if>
<#if CONFIG_USE_PORT_D == true>
    PLIB_PORTS_Read(PORTS_ID_0, PORT_CHANNEL_D);    
</#if>
<#if CONFIG_USE_PORT_E == true>
    PLIB_PORTS_Read(PORTS_ID_0, PORT_CHANNEL_E);    
</#if>
<#if CONFIG_USE_PORT_F == true>
    PLIB_PORTS_Read(PORTS_ID_0, PORT_CHANNEL_F);    
</#if>
<#if CONFIG_USE_PORT_G == true>
    PLIB_PORTS_Read(PORTS_ID_0, PORT_CHANNEL_G);    
</#if>
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CHANGE_NOTICE);         
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_CHANGE_NOTICE);
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_CN, ${CONFIG_SYS_PORTS_CN_INTERRUPT_PRIORITY});
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_CN, ${CONFIG_SYS_PORTS_CN_INTERRUPT_SUB_PRIORITY});          
</#if>
    
    <#if CONFIG_USE_PORT_A == true>
    /* PORT A Initialization */
            <@PLIB_NON_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_A"
                SYS_PORT_TRIS = "SYS_PORT_A_TRIS"
                SYS_PORT_LAT = "SYS_PORT_A_LAT"
                SYS_PORT_ODC = "SYS_PORT_A_ODC"
            />
        </#if>
        <#if CONFIG_USE_PORT_B == true>
    
    /* PORT B Initialization */
            <@PLIB_NON_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_B"
                SYS_PORT_TRIS = "SYS_PORT_B_TRIS"
                SYS_PORT_LAT = "SYS_PORT_B_LAT"
                SYS_PORT_ODC = "SYS_PORT_B_ODC"
            />
        </#if>
        <#if CONFIG_USE_PORT_C == true>
    
    /* PORT C Initialization */
            <@PLIB_NON_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_C"
                SYS_PORT_TRIS = "SYS_PORT_C_TRIS"
                SYS_PORT_LAT = "SYS_PORT_C_LAT"
                SYS_PORT_ODC = "SYS_PORT_C_ODC"
            />
        </#if>
        <#if CONFIG_USE_PORT_D == true>
    
    /* PORT D Initialization */
            <@PLIB_NON_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_D"
                SYS_PORT_TRIS = "SYS_PORT_D_TRIS"
                SYS_PORT_LAT = "SYS_PORT_D_LAT"
                SYS_PORT_ODC = "SYS_PORT_D_ODC"
            />
        </#if>
        <#if CONFIG_USE_PORT_E == true>
    
    /* PORT E Initialization */
            <@PLIB_NON_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_E"
                SYS_PORT_TRIS = "SYS_PORT_E_TRIS"
                SYS_PORT_LAT = "SYS_PORT_E_LAT"
                SYS_PORT_ODC = "SYS_PORT_E_ODC"
            />
        </#if>   
        <#if CONFIG_USE_PORT_F == true>
    
    /* PORT F Initialization */
            <@PLIB_NON_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_F"
                SYS_PORT_TRIS = "SYS_PORT_F_TRIS"
                SYS_PORT_LAT = "SYS_PORT_F_LAT"
                SYS_PORT_ODC = "SYS_PORT_F_ODC"
            />
        </#if>   
        <#if CONFIG_USE_PORT_G == true>
    
    /* PORT G Initialization */
            <@PLIB_NON_PPS_PORTS_APIs 
                PORT_CHANNEL = "PORT_CHANNEL_G"
                SYS_PORT_TRIS = "SYS_PORT_G_TRIS"
                SYS_PORT_LAT = "SYS_PORT_G_LAT"
                SYS_PORT_ODC = "SYS_PORT_G_ODC"
            />
        </#if>
    
    </#if>    
}


/*******************************************************************************
 End of File
*/
