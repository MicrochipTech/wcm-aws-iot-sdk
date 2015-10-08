 /*******************************************************************************
  Watchdog Timer System Service Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    sys_wdt.c

  Summary:
    Watchdog Timer (WDT) System Service implementation.

  Description:
    The WDT System Service provides a simple interface to manage the
    Watchdog Timer module on Microchip microcontrollers.  This file implements
    the core interface routines for the WDT System Service. While building the 
    system service from source, ALWAYS include this file in the build.
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

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include "system/wdt/sys_wdt.h"

/*******************************************************************************
  Function:
    void SYS_WDT_Enable ( bool windowModeEnable )

  Summary:
    Enables the WDT.

  Description:
    This function enables the WDT and its 'window mode'.

  Remarks:
    None.
*/

void SYS_WDT_Enable ( bool windowModeEnable )
{
	#if defined(PLIB_WDT_ExistsEnableControl)
    /* Enable/disable the watchdog timer. */
    if (true == PLIB_WDT_ExistsEnableControl(WDT_PLIB_ID))
    {

                PLIB_WDT_Enable(WDT_PLIB_ID);
    }
	#endif 

	#if defined(PLIB_WDT_ExistsWindowEnable)
    /* Enable/disable the watchdog timer window mode. */
    if (true == PLIB_WDT_ExistsWindowEnable(WDT_PLIB_ID))
    {
        if(true == windowModeEnable)
        {
                PLIB_WDT_WindowEnable(WDT_PLIB_ID);
        }
        else
        {
                PLIB_WDT_WindowDisable(WDT_PLIB_ID);
        }
    }
	#endif	
}


/*******************************************************************************
  Function:
    void SYS_WDT_Disable ( void )

  Summary:
    Disables the WDT.

  Description:
    This function disables the WDT.

  Remarks:
    None.
*/

void SYS_WDT_Disable ( void )
{
    PLIB_WDT_Disable(WDT_PLIB_ID);
}

/******************************************************************************
  Function:
    void SYS_WDT_TimerClear ( void )

  Summary:
     Resets the WDT.

  Description:
    This function resets the WDT. The WDT should be reset periodically before 
    the counter overflows and forces the device to Reset.

  Remarks:
    None.
*/

void SYS_WDT_TimerClear ( void )
{
	#if defined(PLIB_WDT_ExistsTimerClear)	
    if(PLIB_WDT_ExistsTimerClear(WDT_PLIB_ID))
    {
        PLIB_WDT_TimerClear(WDT_PLIB_ID);
    }
	#endif	
}


/*******************************************************************************
End of File
*/

