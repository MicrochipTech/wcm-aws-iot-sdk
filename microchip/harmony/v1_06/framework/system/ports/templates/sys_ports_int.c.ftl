<#--
/*******************************************************************************
  Port System Service Interrupt Handler Template File

  File Name:
    sys_port_int.c

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

<#macro SYS_PORTS_MZ_CN_ISR PORT_CHANNEL INT_CHANNEL INT_CHANNEL_VECTOR INT_CALLBACK>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${INT_CHANNEL_VECTOR}, ipl${CONFIG_SYS_PORTS_CN_INTERRUPT_PRIORITY_NUM}AUTO) _IntHandlerChangeNotification_${PORT_CHANNEL}(void)
<#else>
void IntHandlerChangeNotification_${PORT_CHANNEL}(void)
</#if>
<#else>
void __ISR(${INT_CHANNEL_VECTOR}, ipl${CONFIG_SYS_PORTS_CN_INTERRUPT_PRIORITY_NUM}AUTO) _IntHandlerChangeNotification_${PORT_CHANNEL}(void)
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>
    ${INT_CALLBACK}
    PLIB_INT_SourceFlagClear(INT_ID_0,${INT_CHANNEL});
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>
}
</#macro>
<#if CONFIG_PIC32MZ == true>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_A_CNEN != '0x0'>
   <#if CONFIG_USE_DRV_OVM7690 == true>
     <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortA"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_A"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_A_VECTOR"
        INT_CALLBACK = "DRV_CAMERA_OVM7690_HsyncEventHandler(sysObj.drvOvm7690);"/>
   <#else>
     <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortA"
       INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_A"
       INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_A_VECTOR"
       INT_CALLBACK = ""/>
    </#if>
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_B_CNEN != '0x0'>
    <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortB"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_B"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_B_VECTOR"
        INT_CALLBACK = ""/>
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_C_CNEN != '0x0'>
    <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortC"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_C"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_C_VECTOR"
        INT_CALLBACK = ""/>
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_D_CNEN != '0x0'>
    <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortD"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_D"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_D_VECTOR"
        INT_CALLBACK = ""/>
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_E_CNEN != '0x0'>
    <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortE"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_E"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_E_VECTOR"
        INT_CALLBACK = ""/>
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_F_CNEN != '0x0'>
    <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortF"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_F"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_F_VECTOR"
        INT_CALLBACK = ""/>
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_G_CNEN != '0x0'>
    <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortG"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_G"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_G_VECTOR"
        INT_CALLBACK = ""/>
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_H_CNEN != '0x0'>
    <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortH"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_H"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_H_VECTOR"
        INT_CALLBACK = ""/>
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_J_CNEN != '0x0'>
   <#if CONFIG_USE_DRV_OVM7690 == true>
     <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortJ"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_J"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_J_VECTOR"
        INT_CALLBACK = "DRV_CAMERA_OVM7690_VsyncEventHandler(sysObj.drvOvm7690);"/>
   <#else>
     <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortA"
       INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_A"
       INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_A_VECTOR"
       INT_CALLBACK = ""/>
    </#if>
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_K_CNEN != '0x0'>
    <@SYS_PORTS_MZ_CN_ISR PORT_CHANNEL = "PortK"
        INT_CHANNEL = "INT_SOURCE_CHANGE_NOTICE_K"
        INT_CHANNEL_VECTOR = "_CHANGE_NOTICE_K_VECTOR"
        INT_CALLBACK = ""/>
</#if>
<#else>
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(_CHANGE_NOTICE_VECTOR, ipl${CONFIG_SYS_PORTS_CN_INTERRUPT_PRIORITY_NUM}AUTO) _IntHandlerChangeNotification(void)
<#else>
void IntHandlerChangeNotification(void)
</#if>
<#else>
void __ISR(_CHANGE_NOTICE_VECTOR, ipl${CONFIG_SYS_PORTS_CN_INTERRUPT_PRIORITY_NUM}AUTO) _IntHandlerChangeNotification(void)
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

    /* TODO: Add code to process interrupt here */
<#if CONFIG_HAVE_PPS == true>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_A_CNEN != '0x0'>
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_CHANGE_NOTICE_A);
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_B_CNEN != '0x0'>
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_CHANGE_NOTICE_B);
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_C_CNEN != '0x0'>
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_CHANGE_NOTICE_C);
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_D_CNEN != '0x0'>
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_CHANGE_NOTICE_D);
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_E_CNEN != '0x0'>
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_CHANGE_NOTICE_E);
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_F_CNEN != '0x0'>
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_CHANGE_NOTICE_F);
</#if>
<#if CONFIG_USE_SYS_PORTS_CN_INTERRUPT == true && CONFIG_SYS_PORT_G_CNEN != '0x0'>
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_CHANGE_NOTICE_G);
</#if>
<#else>
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_CHANGE_NOTICE);
</#if>
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
