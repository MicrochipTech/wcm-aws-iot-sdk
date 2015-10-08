<#--
/*******************************************************************************
  ADC Driver Interrupt Handler Template File

  File Name:
    drv_adc_int.c

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
<#if CONFIG_DRV_ADC_INTERRUPT_MODE == true>
<#if CONFIG_USE_3RDPARTY_RTOS>
void IntHandlerDrvAdc(void)
<#else>
void __ISR(${CONFIG_DRV_ADC_ISR_VECTOR}, ipl${CONFIG_DRV_ADC_INT_IPL}AUTO) _IntHandlerDrvAdc(void)
</#if>	
{
<#if CONFIG_DRV_ADC_DRIVER_MODE == "DYNAMIC">
    DRV_ADC_Tasks(sysObj.drvAdc);
<#else>
    /* Clear ADC Interrupt Flag */
    PLIB_INT_SourceFlagClear(INT_ID_0, ${CONFIG_DRV_ADC_INTERRUPT_SOURCE});
</#if>	
}
</#if>


<#if CONFIG_DRV_ADCHS_INTERRUPT_MODE == true>
<#if CONFIG_USE_3RDPARTY_RTOS>
void IntHandlerDrvAdc(void)
<#else>
void __ISR(${CONFIG_DRV_ADCHS_ISR_VECTOR}, ipl${CONFIG_DRV_ADCHS_INT_IPL}AUTO) _IntHandlerDrvAdc(void)
</#if>	
{
<#if CONFIG_DRV_ADCHS_DRIVER_MODE == "DYNAMIC">
    DRV_ADC_Tasks(sysObj.drvAdc);
<#else>
    /* Clear ADC Interrupt Flag */
    PLIB_INT_SourceFlagClear(INT_ID_0, ${CONFIG_DRV_ADCHS_INTERRUPT_SOURCE});
</#if>	
}
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
