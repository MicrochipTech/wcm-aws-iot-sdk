<#--
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
// *****************************************************************************
/* System Touch Initialization Data
*/

const DRV_TOUCH_INIT sysTouchInit0 =
{
<#if CONFIG_USE_DRV_TOUCH_MTCH6301 == true>
    .drvInitialize           = DRV_TOUCH_MTCH6301_Initialize,
    .drvOpen                 = DRV_TOUCH_MTCH6301_Open,
    .drvTouchGetX            = DRV_TOUCH_MTCH6301_TouchGetX,
    .drvTouchGetY            = DRV_TOUCH_MTCH6301_TouchGetY,
<#elseif CONFIG_USE_DRV_TOUCH_AR1020 == true>
    .drvInitialize           = DRV_TOUCH_AR1020_Initialize,
    .drvOpen                 = DRV_TOUCH_AR1020_Open,
    .drvCalibrationSet       = DRV_TOUCH_AR1020_CalibrationSet,
    .drvTouchGetX            = DRV_TOUCH_AR1020_TouchGetX,
    .drvTouchGetY            = DRV_TOUCH_AR1020_TouchGetY,
<#elseif CONFIG_USE_DRV_TOUCH_ADC10BIT == true>
    .drvInitialize           = DRV_TOUCH_ADC10BIT_Initialize,
    .drvOpen                 = DRV_TOUCH_ADC10BIT_Open,
    .drvCalibrationSet       = DRV_TOUCH_ADC10BIT_CalibrationSet,
    .drvTouchGetX            = DRV_TOUCH_ADC10BIT_TouchGetX,
    .drvTouchGetY            = DRV_TOUCH_ADC10BIT_TouchGetY,
</#if>
    .orientation             = DISP_ORIENTATION,
    .horizontalResolution    = DISP_HOR_RESOLUTION,
    .verticalResolution      = DISP_VER_RESOLUTION,
};

<#--
/*******************************************************************************
 End of File
*/
-->

