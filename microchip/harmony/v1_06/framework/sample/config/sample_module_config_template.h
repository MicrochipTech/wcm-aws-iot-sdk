/*******************************************************************************
  Sample Module Configuration Templates

  Company:
    Microchip Technology Inc.

  File Name:
    sample_module_config_template.h

  Summary:
    Sample module configuration templates.

  Description:
    This file contains examples of constants to configure the sample module.
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

#ifndef _SAMPLE_MODULE_CONFIG_TEMPLATE_H
#define _SAMPLE_MODULE_CONFIG_TEMPLATE_H


// *****************************************************************************
/* Sample Module, Number of Instances

  Summary:
    Defines how many instances of the sample module are supported.

  Description:
    This macro defines how many instances of the sample module are supported.

  Remarks:
    Defaults to 1 and displays a build warning if not specified..
*/

#define SAMPLE_MODULE_INSTANCES_NUMBER      2


// *****************************************************************************
/* Sample Module, Interrupt Mode

  Summary:
    Determines whether or not the sample module will support interrupt-driven 
    execution mode.

  Description:
    This macro determines whether or not the sample module will support 
    interrupt interrupt driven execution.  If defined as true, then the sample
    module will enable, disable, and clear the interrupt source identified in
    the SAMPLE_MODULE_INIT_DATA structure.  Otherwise, it shoud be defined as
    false and the module will only support polled operation.
*/

#define SAMPLE_MODULE_INTERRUPT_MODE      false


// *****************************************************************************
/* Sample Module, Timeout

  Summary:
    Defines OSAL mutex timeout for Sample module (in milliseconds).

  Description:
    This macro defines how many milliseconds the sample module will block, 
    waiting to obtain a mutex, when accessing its internal shared resources.  
    This is only utilized in interrupt-driven modes, but needs to be defined
    to build correclty.

  Remarks:
    Used by OSAL_MUTEX_Lock.
*/

#define SAMPLE_MODULE_TIMEOUT           100


#endif //_SAMPLE_MODULE_CONFIG_TEMPLATE_H

/*******************************************************************************
 End of File
*/

