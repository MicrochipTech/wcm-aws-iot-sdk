<#--
/*******************************************************************************
  USB Framework Interrupt Handler Template File

  File Name:
    usb_interrupt.c

  Summary:
    This file contains source code necessary to place the USB ISR.

  Description:
    
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
<#if CONFIG_DRV_USB_INTERRUPT_MODE == true>
    <#if CONFIG_USB_DEVICE_INST_IDX0 == true >
		<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
<#if CONFIG_USE_3RDPARTY_RTOS>

<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR(${CONFIG_DRV_USB_ISR_VECTOR_IDX0}, ipl${CONFIG_DRV_USB_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0(void)
<#else>
void IntHandlerUSBInstance0(void)
</#if>
<#else>
void __ISR(${CONFIG_DRV_USB_ISR_VECTOR_IDX0}, ipl${CONFIG_DRV_USB_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0(void)
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

    USB_DEVICE_Tasks_ISR(sysObj.usbDevObject0);
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>
            
}
			<#if CONFIG_PIC32MZ == true>
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR ( ${CONFIG_DRV_USBDMA_ISR_VECTOR_IDX0},ipl${CONFIG_DRV_USBDMA_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0_USBDMA ( void )
<#else>
void IntHandlerUSBInstance0_USBDMA ( void )
</#if>
<#else>
void __ISR ( ${CONFIG_DRV_USBDMA_ISR_VECTOR_IDX0},ipl${CONFIG_DRV_USBDMA_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0_USBDMA ( void )
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

    USB_DEVICE_Tasks_ISR_USBDMA(sysObj.usbDevObject0);
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
			</#if>
		</#if>
    </#if>
    <#if CONFIG_USB_HOST_INSTANCE_0 == true >
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR( ${CONFIG_DRV_USB_HOST_ISR_VECTOR_IDX0} , ipl${CONFIG_DRV_USB_HOST_INT_PRIO_NUM_IDX0}AUTO)_IntHandler_USB_stub ( void )
<#else>
void IntHandler_USB_stub ( void )
</#if>
<#else>
void __ISR( ${CONFIG_DRV_USB_HOST_ISR_VECTOR_IDX0} , ipl${CONFIG_DRV_USB_HOST_INT_PRIO_NUM_IDX0}AUTO)_IntHandler_USB_stub ( void )
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>
    USB_HOST_Tasks_ISR(sysObj.usbHostObject0);
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}

<#if CONFIG_PIC32MZ == true>
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR ( ${CONFIG_DRV_USB_HOST_DMA_ISR_VECTOR_IDX0},ipl${CONFIG_DRV_USB_HOST_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0_USBDMA ( void )
<#else>
void IntHandlerUSBInstance0_USBDMA ( void )
</#if>
<#else>
void __ISR ( ${CONFIG_DRV_USB_HOST_DMA_ISR_VECTOR_IDX0},ipl${CONFIG_DRV_USB_HOST_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0_USBDMA ( void )
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>

    USB_HOST_Tasks_ISR_USBDMA(sysObj.usbHostObject0);
<#if CONFIG_USE_3RDPARTY_RTOS && CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>

}
			</#if>
    </#if>
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
