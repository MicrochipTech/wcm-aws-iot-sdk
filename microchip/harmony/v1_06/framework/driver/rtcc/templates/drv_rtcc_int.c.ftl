<#--
/*******************************************************************************
  RTCC Driver Interrupt Handler Template File

  File Name:
    drv_rtcc_int.c.ftl

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

SOFTWARE AND DICUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICRICHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PRICUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
 -->
<#if CONFIG_DRV_RTCC_INTERRUPT_MODE == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_RTCC_ISR_VECTOR}, ipl${CONFIG_DRV_RTCC_INT_PRIO_NUM}AUTO) _IntHandlerRTCC(void)
<#else>
void IntHandlerRTCC(void)
</#if>

<#else>
void __ISR(${CONFIG_DRV_RTCC_ISR_VECTOR}, ipl${CONFIG_DRV_RTCC_INT_PRIO_NUM}AUTO) _IntHandlerRTCC(void)
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
    /* Call ThreadX context save.  */
    _tx_thread_context_save();
</#if>

    PLIB_INT_SourceFlagClear(INT_ID_0, ${CONFIG_DRV_RTCC_INT_SOURCE});

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
