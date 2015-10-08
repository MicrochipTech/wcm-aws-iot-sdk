/*******************************************************************************
  Board Support Package Header File.

  Company:      
    Microchip Technology Inc.

  File Name:    
    bsp_config.h

  Summary:      
    Board Support Package Header file for WCM Development Kit 1.

  Description:
    This file contains constants, macros, type defintions and function
    declarations required by the Explorer 16 Development Board when used with a
    PIC32MX795F512L PIM.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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
// DOM-IGNORE-END

#ifndef _BSP_CONFIG_H
#define _BSP_CONFIG_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "peripheral/ports/plib_ports.h"
#include "peripheral/adc/plib_adc.h"
#include "peripheral/power/plib_power.h"
#include "peripheral/devcon/plib_devcon.h"
#include "peripheral/cmp/plib_cmp.h"

// *****************************************************************************
// *****************************************************************************
// Section: Constants and Type Definitions.
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Oscillator Frequency

  Summary:
    Defines frequency value of crystal/oscillator used on the board

  Description:
    Defines frequency value of crystal/oscillator used on the board
*/

#define BSP_OSC_FREQUENCY 8000000


#define BSP_POTENTIOMETER PORTS_BIT_POS_5

typedef enum
{
    BSP_LED_EASY_CONFIGURATION,            
    BSP_LED_CONNECTING_TO_AP,       
    BSP_LED_CONNECTION_FAILED,      
    BSP_LED_SERVER_CONNECT_FAILED,  
    BSP_LED_AP_CONNECTED,     
    BSP_LED_ALL_GOOD,  
    BSP_LED_TX,
    BSP_LED_RX,
    BSP_LED_HTTP_400,
    BSP_LED_HTTP_500,
    BSP_LED_DNS_FAILED,
    BSP_LED_TCPIP_STACK_INIT_FAILURE
} BSP_LED_LIGHT_SHOW;

// **********************   *******************************************************
/* BSP Switch.

  Summary:
    Defines the switches available on this board.

  Description:
    This enumeration defines the switches available on this board.

  Remarks:
    None.
*/

typedef enum
{

     /* SWITCH 1 */
     BSP_SWITCH_1 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_2/*DOM-IGNORE-END*/,

     /* SWITCH 2 */
     BSP_SWITCH_2 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_9/*DOM-IGNORE-END*/,
    
     /* SWITCH 3 */
     BSP_SWITCH_3 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_10/*DOM-IGNORE-END*/,
     
     /* SWITCH 4 */
     BSP_SWITCH_4 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_11/*DOM-IGNORE-END*/,

} BSP_SWITCH;

// *****************************************************************************
/* BSP Switch state.

  Summary:
    Defines possible states of the switches on this board.

  Description:
    This enumeration defines the possible states of the switches on this board.

  Remarks:
    None.
*/

typedef enum
{
    /* Switch pressed */
    BSP_SWITCH_STATE_ASSERTED = /*DOM-IGNORE-BEGIN*/0/*DOM-IGNORE-END*/,

   /* Switch not pressed */
    BSP_SWITCH_STATE_DEASSERTED = /*DOM-IGNORE-BEGIN*/1/*DOM-IGNORE-END*/

} BSP_SWITCH_STATE;

#define BSP_SWITCH_BUSY -1

//DGC_FINISH  Comment these
typedef struct
{
    int32_t prevValue;
    int32_t timerActive;
    int32_t duration; // In milliseconds
    uint32_t startTick;
    uint32_t endTick;

}BSP_SWITCH_DEBOUNCE_T;

#define BSP_MAX_SWITCHES 4
#define BSP_SWITCH_DEBOUNCE_TIME 200

typedef struct
{
    uint32_t light_show;

    BSP_SWITCH_STATE  s1, s2, s3, s4;
    BSP_SWITCH_STATE previousStateS1;
    BSP_SWITCH_STATE previousStateS2;
    BSP_SWITCH_STATE previousStateS3;
    BSP_SWITCH_STATE previousStateS4;
    BSP_SWITCH_DEBOUNCE_T switches[BSP_MAX_SWITCHES];

} BSP_DATA;

// *****************************************************************************
/* LED Number.

  Summary:
    Defines the LEDs available on this board.

  Description:
    This enumeration defines the LEDs available on this board.

  Remarks:
    None.
*/

typedef enum
{
    /* LED 1 */
    BSP_LED_1 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_12/*DOM-IGNORE-END*/,
                                                                      
    /* LED 2 */
    BSP_LED_2 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_13/*DOM-IGNORE-END*/,
                                                                      
    /* LED 3 */
    BSP_LED_3 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_11/*DOM-IGNORE-END*/,
                                                                      
    /* LED 4 */
    BSP_LED_4 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_15/*DOM-IGNORE-END*/,
                                                                      
    /* LED 5 */
    BSP_LED_5 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_6/*DOM-IGNORE-END*/,
                                                                      
    /* LED 6 */
    BSP_LED_6 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_7/*DOM-IGNORE-END*/,
                                                                      
    /* LED 7 */
    BSP_LED_7 = /*DOM-IGNORE-BEGIN*/PORTS_BIT_POS_2/*DOM-IGNORE-END*/,

} BSP_LED;

#define BSP_BOOST_EN                PORTS_BIT_POS_3
#define BOOST_ON                    1
#define BOOST_OFF                   0

#define BSP_BOOST_SINK              PORTS_BIT_POS_3

// *****************************************************************************
/* LED State

  Summary:
    Enumerates the supported LED states.

  Description:
    This enumeration defines the supported LED states.

  Remarks:
    None.
*/

typedef enum
{
    /* LED State is on */
    BSP_LED_STATE_OFF = /*DOM-IGNORE-BEGIN*/0/*DOM-IGNORE-END*/,

    /* LED State is off */
    BSP_LED_STATE_ON = /*DOM-IGNORE-BEGIN*/1,/*DOM-IGNORE-END*/

} BSP_LED_STATE;

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function: 
    void BSP_Initialize(void)

  Summary:
    Performs the necessary actions to initialize a board
  
  Description:
    This function initializes the LED and Switch ports on the board.  This
    function must be called by the user before using any APIs present on this
    BSP.  

  Precondition:
    None.

  Parameters:
    None
  
  Returns:
    None.

  Example:
    <code>
    //Initialize the BSP
    BSP_Initialize();
    </code>

  Remarks:
    None                                                                   
*/

void BSP_Initialize(void);

// *****************************************************************************
/* Function: 
    void BSP_LEDStateSet(BSP_LED led, BSP_LED_STATE state);

  Summary:
    Controls the state of the LED.
  
  Description:
    This function allows the application to specify the state of the LED.

  Precondition:
    BSP_Initialize() should have been called.

  Parameters:
    led     - The LED to operate on.
    state   - The state to be set.
  
  Returns:
    None.

  Example:
    <code>
    
    // Initialize the BSP
    BSP_Initialize();

    // Switch on LED3 on the board 
    BSP_LEDStateSet(BSP_LED_3, BSP_LED_STATE_ON);
    
    // Switch off LED4 on the board 
    BSP_LEDStateSet(BSP_LED_4, BSP_LED_STATE_OFF);

    </code>

  Remarks:
    None                                                                   
*/

void BSP_LEDStateSet(BSP_LED led, BSP_LED_STATE state);

// *****************************************************************************
/* Function: 
    BSP_LED_STATE BSP_LEDStateGet(BSP_LED led);

  Summary:
    Returns the present state of the LED.
  
  Description:
    This function returns the present state of the LED.

  Precondition:
    BSP_Initialize() should have been called.

  Parameters:
    led - The LED to whose status needs to be obtained.
  
  Returns:
    The ON/OFF state of the LED.

  Example:
    <code>
    
    // Initialize the BSP
    BSP_Initialize();

    // Check if LED3 is off
    if(BSP_LED_STATE_OFF == BSP_LEDStateGet(BSP_LED_3)
    {
        // Switch on the LED.
        BSP_LEDStateSet(BSP_LED_3, BSP_LED_STATE_ON);
    }

    </code>

  Remarks:
    None                                                                   
*/

BSP_LED_STATE BSP_LEDStateGet(BSP_LED led);

// *****************************************************************************
/* Function: 
    void BSP_LEDToggle(BSP_LED led);

  Summary:
    Toggles the state of the LED between BSP_LED_STATE_ON and BSP_LED_STATE_OFF.
  
  Description:
    This function toggles the state of the LED between BSP_LED_STATE_ON and
    BSP_LED_STATE_OFF.

  Precondition:
    BSP_Initialize() should have been called.

  Parameters:
    led - The LED to toggle.
  
  Returns:
    None.

  Example:
    <code>
    
    // Initialize the BSP
    BSP_Initialize();

    // Switch on LED3 on the board 
    BSP_LEDStateSet(BSP_LED_3, BSP_LED_STATE_ON);
    
    // Switch off LED4 on the board 
    BSP_LEDStateSet(BSP_LED_4, BSP_LED_STATE_OFF);

    // Toggle state of LED3
    BSP_LEDToggle(BSP_LED_3);
    </code>

  Remarks:
    None                                                                   
*/

void BSP_LEDToggle(BSP_LED led);

// *****************************************************************************
/* Function: 
    void BSP_LEDOn(BSP_LED led);

  Summary:
    Switches ON the specified LED.
  
  Description:
    This function switches ON the specified LED.

  Precondition:
    BSP_Initialize() should have been called.

  Parameters:
    led - The LED to switch on.
  
  Returns:
    None.

  Example:
    <code>
    
    // Initialize the BSP
    BSP_Initialize();

    // Switch on LED D3 on the board 
    BSP_LEDOn(BSP_LED_3);
    
    </code>

  Remarks:
    None                                                                   
*/

void BSP_LEDOn(BSP_LED led);

// *****************************************************************************
/* Function: 
    void BSP_LEDOff(BSP_LED led);

  Summary:
    Switches OFF the specified LED.
  
  Description:
    This function switches OFF the specified LED.

  Precondition:
    BSP_Initialize() should have been called.

  Parameters:
    led - The LED to switch off.
  
  Returns:
    None.

  Example:
    <code>
    
    // Initialize the BSP
    BSP_Initialize();

    // Switch off LED D3 on the board 
    BSP_LEDOff(BSP_LED_3);
    
    </code>

  Remarks:
    None                                                                   
*/

void BSP_LEDOff(BSP_LED led);

// *****************************************************************************
/* Function:
    void BSP_LEDAllOff();

  Summary:
    Switches OFF all LEDs.

  Description:
    This function switches OFF all LEDs.

  Precondition:
    BSP_Initialize() should have been called.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>

    // Initialize the BSP
    BSP_Initialize();

    // Switch off all LEDs
    BSP_LEDAllOff();

    </code>

  Remarks:
    None
*/

void BSP_LEDAllOff();

// *****************************************************************************
/* Function: 
    BSP_SWITCH_STATE BSP_SwitchStateGet(BSP_SWITCH switch);

  Summary:
    Returns the present state (pressed or not pressed) of the specified switch.
  
  Description:
    This function returns the present state (pressed or not pressed) of the
    specified switch.

  Precondition:
    BSP_Initialize() should have been called.

  Parameters:
    switch  - The switch whose state needs to be obtained.
  
  Returns:
    The pressed released state of the switch.

  Example:
    <code>
    
    // Initialize the BSP
    BSP_Initialize();

    // Check the state of the switch.
    if(BSP_SWITCH_STATE_PRESSED == BSP_SwitchStateGet(BSP_SWITCH_3))
    {
        // This means that Switch 3 on the board is pressed.
    }

    </code>

  Remarks:
    None                                                                   
*/

BSP_SWITCH_STATE BSP_SwitchStateGet(BSP_SWITCH bspSwitch);

//DGC_FINISH Comment this function prototype
void BSP_LED_LightShow(BSP_LED_LIGHT_SHOW lightShow);
void BSP_LED_LightShowSet(BSP_LED_LIGHT_SHOW lightshow);
void BSP_SYS_Tasks ();
int32_t BSP_SWITCH_DeviceDebounce(BSP_SWITCH_DEBOUNCE_T *handle, uint32_t curVal);
BSP_SWITCH_STATE BSP_SWITCH_SwitchTest(BSP_SWITCH switchId);
void BSP_SWITCH_Tasks(void);

#endif //_BSP_CONFIG_H

/*******************************************************************************
 End of File
*/
