/*******************************************************************************
  USB Host MSD function driver compile time options

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_msd_config_template.h

  Summary:
    USB Host MSD configuration template header file

  Description:
    This file contains USB host MSD function driver compile time 
    options(macros) that has to be configured by the user. This file is a 
    template file and must be used as an example only. This file must not 
    be directly included in the project.

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _USB_HOST_MSD_CONFIG_TEMPLATE_H_
#define _USB_HOST_MSD_CONFIG_TEMPLATE_H_

#error (" This is a template file and must not be included directly in the project" );

// *****************************************************************************
// *****************************************************************************
// Section: USB Device configuration Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* 
  Summary:
    This constant sets maximum possible number of instances of USB host MSD 
    function driver that can be instantiated by using USB_HOST_MSD_Initialize()
    routine. 

  Description:
   
    
  Remarks:
    The static implementation supports only one instance. Setting the value of
    this constant to > 1 has no effect on static implementations. Only in dynamic
    implementation of USB host MSD function driver this value can be set > 1.
    
    USB host MSD function driver has to support at least one instance. 
    Hence, make sure the value of this constant is set to > 0.
    
    Increasing the instance count consumes RAM and can lead to performance
    degradation.
*/
#define USB_HOST_MSD_MAX_INSTANCES                1



#endif



