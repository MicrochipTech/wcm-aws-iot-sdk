/*******************************************************************************
  Watchdog Timer System Service Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    sys_wdt_config.h

  Summary:
    Watchdog Timer (WDT) System Service interface definition.

  Description:
    This file contains the interface definition for the WDT System Service. It
    provides a way to interact with the Watchdog timer subsystem to manage
    the timing requests supported by the system
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

#ifndef _SYS_WDT_CONFIG_TEMPLATE_H
#define _SYS_WDT_CONFIG_TEMPLATE_H


#error "This is a configuration template file.  Do not include it directly."



// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system.h"	// system queue header


// *****************************************************************************
/* Watchdog timer module ID used by the Peripheral Library

  Summary:
    Configures the module's ID used by the peripheral library.

  Description:
    This macro configures the module's ID used by the peripheral library.

  Remarks:
    None.
*/

#define WDT_PLIB_ID    			WDT_ID_1


#endif //_SYS_WDT_CONFIG_TEMPLATE_H

/*******************************************************************************
 End of File
*/

