/*******************************************************************************
  USB Host Layer Compile Time Options

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_config_template.h

  Summary:
    USB host configuration template header file.

  Description:
    This file contains USB host layer compile time options (macros) that are to
    be configured by the user. This file is a template file and must be used as
    an example only. This file must not be directly included in the project.

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

#ifndef _USB_HOST_CONFIG_TEMPLATE_H_
#define _USB_HOST_CONFIG_TEMPLATE_H_


#error "This is a configuration template file.  Do not include it directly."

// *****************************************************************************
// *****************************************************************************
// Section: USB Host configuration Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Number of Host Layer Instances.
 
  Summary:
    Number of Host Layer instances to provisioned in the application.

  Description:
    This configuration macro defines the number of Host Layer instances in the
    application. 
    
  Remarks:
    None.
*/

#define           USB_HOST_INSTANCES_NUMBER                     1   


// *****************************************************************************
/* Number of Host Layer Clients
 
  Summary:
    Number of Host Layer Clients to be provisioned in the application.

  Description:
    This macros defines the total number of Host Layer client across all
    instances of the Host Layer. 
    
  Remarks:
    None.
*/


#define      USB_HOST_CLIENTS_NUMBER                            1




#endif // #ifndef __USB_HOST_CONFIG_TEMPLATE_H_

/*******************************************************************************
 End of File
*/
