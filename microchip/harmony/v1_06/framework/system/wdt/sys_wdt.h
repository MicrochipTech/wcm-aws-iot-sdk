/*******************************************************************************
  Watchdog Timer System Service Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    sys_wdt.h

  Summary:
    Watchdog Timer (WDT) System Service interface definition.

  Description:
    This file contains the interface definition for the WDT System Service.  It
    provides a way to interact with the WDT subsystem to manage the
    timing requests supported by the system.
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

#ifndef _SYS_WDT_H
#define _SYS_WDT_H

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include "system/wdt/src/sys_wdt_local.h"
#include "peripheral/wdt/plib_wdt.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

// *****************************************************************************
// *****************************************************************************
// Section: SYS WDT Module Initialization Routines
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/* Function:
    void SYS_WDT_Enable ( bool windowModeEnable )

  Summary:
    Enables the WDT. The argument 'windowModeEnable' will be used only
    for those devices that support 'window mode'. Otherwise, it will be discarded.

  Description:
    This function enables the WDT. The argument 'windowModeEnable' will 
    be used only for those devices that support 'window mode'. Otherwise, the argument 
    will be discarded. This function could be called multiple times to enable/disable 
    the 'window mode'.

  Precondition:
    None.

  Parameters:
    windowModeEnable - A flag indicates whether to enable/disable the 'window mode'.
	- true  - Enable the 'window mode'
	- false - Disable the 'window mode'

  Returns:
    None.

  Example:
    <code>
    bool windowModeEnable = true;

    SYS_WDT_Enable(windowModeEnable);
    </code>

  Remarks:
     Calling this function is not necessary if the WDT is enabled through
     Configuration bits.
*/

void SYS_WDT_Enable ( bool windowModeEnable );


//******************************************************************************
/* Function:
    void SYS_WDT_Disable ( void )

  Summary:
    Disables the WDT if it is enabled in software.

  Description:
    This function disables the WDT if it is enabled in software. If the WDT is 
    enabled through 'configuration bits' it cannot be disabled using this function.

  Precondition:
    The WDT should be disabled through 'configuration bits'.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
	SYS_WDT_Disable();
    </code>

  Remarks:
    The example code doesn't include the settings that should be done through
    configuration bits.
*/

void SYS_WDT_Disable ( void );


//******************************************************************************
/* Function:
    void SYS_WDT_TimerClear ( void )

  Summary:
    Reset the WDT.

  Description:
    This function clears the WDT counter. The WDT should be cleared periodically 
    before the count overflows and forces the device to Reset.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>

    //Application loop
    while(1)
    {
        SYS_WDT_TimerClear();
        //user code
    }
    </code>

  Remarks:
    Clearing the WDT before the count reaches the window, will cause a reset in 
    Windowed mode.

    The example code doesn't include the settings that should be done through
    Configuration bits.

    This feature is not available on all devices.  Please refer to the specific
    device data sheet to determine whether this feature is supported.
*/

void SYS_WDT_TimerClear ( void );

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif


#endif //SYS_WDT_H

/*******************************************************************************
 End of File
*/

