<#--
/*******************************************************************************
  ADC Driver Initialization call 

  Company:
    Microchip Technology Inc.

  File Name:
    drv_adc_static_init.c.ftl

  Summary:
    This template is referenced in system_init.c.ftl to call the ADC driver
    initialization routine.

  Description:
    The OC device driver initialization routine sets the Timer functionality.
    
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
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTOCULAR PURPOSE.
IN NO EVENT SHALL MOCROCHIP OR ITS LOCENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STROCT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVOCES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END
-->
<#if CONFIG_USE_DRV_ADC == true>
<#if CONFIG_DRV_ADC_DRIVER_MODE == "STATIC">
    /* Initialize ADC */
    DRV_ADC_Initialize();
</#if>
</#if>	
<#--
/*******************************************************************************
 End of File
*/
-->