/*******************************************************************************
  Watchdog Timer System Service Local Data

  Company:
    Microchip Technology Inc.

  File Name:
    sys_wdt_local.h

  Summary:
    Watchdog Timer (WDT) System Service declarations and definitions.

  Description:
    This file contains the WDT System Service local declarations.
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

#ifndef _SYS_WDT_LOCAL_H
#define _SYS_WDT_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "system/common/sys_common.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


// *****************************************************************************
// *****************************************************************************
// Section: Watchdog timer PLIB Id
// *****************************************************************************
// *****************************************************************************
/* Watchdog timer module PLIB id */

// *****************************************************************************
/* Watchdog timer Peripheral Library ID.

  Summary:
    Defines the WDT module ID.

  Description:
    The module ID for the WDT Peripheral Library will be constant as
    long as there is no more than one WDT in a device.

  Remarks:
    None.

*/

#define WDT_PLIB_ID     				WDT_ID_0


#endif //#ifndef _SYS_WDT_LOCAL_H

/*******************************************************************************
 End of File
*/

