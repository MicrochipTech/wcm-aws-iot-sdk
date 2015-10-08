/*******************************************************************************
  Board Support Package Implementation.

  Company:      
    Microchip Technology Inc.

  File Name:    
    bsp_sys_init.c

  Summary:      
    Board Support Package Implementation for Explorer 16 Development Board when
    used with a PIC32MX795F512L Plug In Module (PIM).

  Description:
    This file contains the implementation of the Board Support Package for the
    Explorer 16 Development Board when used with a PIC32MX795F512L PIM.
*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "bsp_config.h"
#include "system/tmr/sys_tmr.h"

#define BSP_SWITCH_MS_ELLAPSED_TIME_TO_HZ(x) (1250/(x)) // convert time to frequency

BSP_DATA bspData;

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function: void BSP_Initialize(void)

  Summary:
    Performs the neccassary actions to initialize a board

  Description:
    This routine performs the neccassary actions to initialize a board

  Remarks:
    Refer to bsp_config.h for usage information.

*/

void BSP_Initialize(void )
{

    /* set the switch pins as input */
    PLIB_PORTS_PinDirectionInputSet ( PORTS_ID_0 , PORT_CHANNEL_B , BSP_SWITCH_1 );
    PLIB_PORTS_PinDirectionInputSet ( PORTS_ID_0 , PORT_CHANNEL_B , BSP_SWITCH_2 );
    PLIB_PORTS_PinDirectionInputSet ( PORTS_ID_0 , PORT_CHANNEL_B , BSP_SWITCH_3 );
    PLIB_PORTS_PinDirectionInputSet ( PORTS_ID_0 , PORT_CHANNEL_B , BSP_SWITCH_4 );

    /* set the switch pins as digital */
    PLIB_PORTS_PinModeSelect ( PORTS_ID_0 , BSP_SWITCH_1 , PORTS_PIN_MODE_DIGITAL );
    PLIB_PORTS_PinModeSelect ( PORTS_ID_0 , BSP_SWITCH_2 , PORTS_PIN_MODE_DIGITAL );
    PLIB_PORTS_PinModeSelect ( PORTS_ID_0 , BSP_SWITCH_3 , PORTS_PIN_MODE_DIGITAL );
    PLIB_PORTS_PinModeSelect ( PORTS_ID_0 , BSP_SWITCH_4 , PORTS_PIN_MODE_DIGITAL );

    /* Set LED pins as outputs */
    PLIB_PORTS_PinDirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_1 );
    PLIB_PORTS_PinDirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_2 );
    PLIB_PORTS_PinDirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_D, BSP_LED_3 );
    PLIB_PORTS_PinDirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_4 );
    PLIB_PORTS_PinDirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_5 );
    PLIB_PORTS_PinDirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_6 );
    PLIB_PORTS_PinDirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_E, BSP_LED_7 );

    /* Clear pin values for LEDs */
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_1 );
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_2 );
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_D, BSP_LED_3 );
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_4 );
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_5 );
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_6 );
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_E, BSP_LED_7 );

    /* Set Boost enable pin */
    PLIB_PORTS_PinDirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_E, BSP_BOOST_EN );
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_E, BSP_BOOST_EN );

    /* Initialize the LED light show value and switch states*/
    bspData.light_show      = BSP_LED_CONNECTING_TO_AP;
    bspData.previousStateS1 = BSP_SWITCH_STATE_DEASSERTED;
    bspData.previousStateS2 = BSP_SWITCH_STATE_DEASSERTED;
    bspData.previousStateS3 = BSP_SWITCH_STATE_DEASSERTED;
    bspData.previousStateS4 = BSP_SWITCH_STATE_DEASSERTED;
    bspData.s1              = BSP_SWITCH_STATE_DEASSERTED;
    bspData.s2              = BSP_SWITCH_STATE_DEASSERTED;
    bspData.s3              = BSP_SWITCH_STATE_DEASSERTED;
    bspData.s4              = BSP_SWITCH_STATE_DEASSERTED;

    /* Initialize switch state machine values for each switch object */
    int i;
    for(i=0 ; i<BSP_MAX_SWITCHES; i++)
    {
       bspData.switches[i].duration     =  BSP_SWITCH_DEBOUNCE_TIME;
       bspData.switches[i].startTick    = 0;
       bspData.switches[i].endTick      = 0;
       bspData.switches[i].timerActive  = false;
    }

    /* Initialize potentiometer adc */
    DDPCONbits.JTAGEN = 0;
    AD1CHSbits.CH0SA  = 5; // Input to AN5 (potentiometer)
    AD1CHSbits.CH0SB = 4;  // Input to AN4 (battery voltage)
    AD1PCFGbits.PCFG5 = 0; // Disable digital input on AN5, potentiometer
    AD1PCFGbits.PCFG4 = 0; // Disable digital input on AN4, potentiometer

    TRISBbits.TRISB4 = 1;
    
    AD1CON1 = 0x84E4; // Turn on, auto sample start, auto-convert, 12 bit mode (on parts with a 12bit A/D)
    AD1CON2 = 0x0405; // AVdd, AVss, int every 2 conversions, MUXA only, scan
    AD1CON3 = 0x1003; // 16 Tad auto-sample, Tad = 3*Tcy
    AD1CSSL = 1 << 5; // Scan pot

    /* Comparator CVREF setup */
    PLIB_CMP_CVREF_OutputDisable(CMP_ID_1); // Disables the output voltage
    PLIB_CMP_CVREF_WideRangeDisable(CMP_ID_1);
    PLIB_CMP_CVREF_SourceVoltageSelect(CMP_ID_1, CMP_CVREF_VOLTAGE_SOURCE_VDD); //Connects the Comparator module to the selected voltage source.
    PLIB_CMP_CVREF_ValueSelect(CMP_ID_1, CMP_CVREF_VALUE_10); // Selects the voltage reference value.
    PLIB_CMP_CVREF_Enable(CMP_ID_1);  // Enables the voltage reference of the comparator module
    
    CM1CONbits.CCH = 0b00;
    CM1CONbits.CREF = 1;

    /* Comparator CMP1 setup */
    PLIB_CMP_OutputInvertEnable(CMP_ID_1); // Comparator output is inverted
    PLIB_CMP_OutputDisable(CMP_ID_1); // Disables the comparator output
    PLIB_CMP_StopInIdleModeDisable(CMP_ID_1); // Will run in idle so we can check to boost in sleep
    PLIB_CMP_InterruptEventSelect(CMP_ID_1, CMP_INTERRUPT_GENERATION_DISABLED); // Disable interuupt event
    PLIB_CMP_Enable(CMP_ID_1);

    /* Initialize power saving on "WAIT" */
    PLIB_DEVCON_SystemUnlock(DEVCON_ID_0);
    PLIB_OSC_OnWaitActionSet(OSC_ID_0, OSC_ON_WAIT_SLEEP);

}

// *****************************************************************************
/* Function: 
    void BSP_LEDStateSet(BSP_LED led, BSP_LED_STATE state);

  Summary:
    Controls the state of the LED.
  
  Description:
    This function allows the application to specify the state of the LED.

  Remarks:
    Refer to bsp_config.h for usage information.
*/

void BSP_LEDStateSet(BSP_LED led, BSP_LED_STATE state)
{
    switch(led)
    {
        case(BSP_LED_1):
             PLIB_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_B, led, state);
        break;

        case(BSP_LED_2):
             PLIB_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_B, led, state);
        break;

        case(BSP_LED_3):
             PLIB_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_D, led, state);
        break;

        case(BSP_LED_4):
             PLIB_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_B, led, state);
        break;

        case(BSP_LED_5):
             PLIB_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_B, led, state);
        break;

        case(BSP_LED_6):
             PLIB_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_B, led, state);
        break;

        case(BSP_LED_7):
             PLIB_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_E, led, state);
        break;
        default:
            // Invalid LED
        break;
    }
}

// *****************************************************************************
/* Function: 
    void BSP_LEDOn(BSP_LED led);

  Summary:
    Switches ON the specified LED.
  
  Description:
    This function switches ON the specified LED.

  Remarks:
    Refer to bsp_config.h for usage information.
*/

void BSP_LEDOn(BSP_LED led)
{
    switch(led)
    {
        case(BSP_LED_1):
             PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_2):
             PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_3):
             PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_D, led);
        break;

        case(BSP_LED_4):
             PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_5):
             PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_6):
             PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_7):
             PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_E, led);
        break;
        default:
            // Invalid LED
        break;
    }
}

// *****************************************************************************
/* Function: 
    void BSP_LEDOff(BSP_LED led);

  Summary:
    Switches OFF the specified LED.
  
  Description:
    This function switches OFF the specified LED.

  Remarks:
    Refer to bsp_config.h for usage information.
*/

void BSP_LEDOff(BSP_LED led)
{
switch(led)
    {
        case(BSP_LED_1):
             PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_2):
             PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_3):
             PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_D, led);
        break;

        case(BSP_LED_4):
             PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_5):
             PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_6):
             PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_7):
             PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_E, led);
        break;
        default:
            // Invalid LED
        break;
    }
}

// *****************************************************************************
/* Function:
    void BSP_LEDAllOff();

  Summary:
    Switches OFF all LEDs.

  Description:
    This function switches OFF all LEDs.

  Remarks:
    Refer to bsp_config.h for usage information.
*/
void BSP_LEDAllOff()
{
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_1);
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_2);
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_D, BSP_LED_3);
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_4);
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_5);
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, BSP_LED_6);
    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_E, BSP_LED_7);
}

// *****************************************************************************
/* Function: 
    BSP_LED_STATE BSP_LEDStateGet(BSP_LED led);

  Summary:
    Returns the present state of the LED.
  
  Description:
    This function returns the present state of the LED.

  Remarks:
    Refer to bsp_config.h for usage information.
*/

BSP_LED_STATE BSP_LEDStateGet(BSP_LED led)
{
switch(led)
    {
        case(BSP_LED_1):
             return(PLIB_PORTS_PinGetLatched( PORTS_ID_0, PORT_CHANNEL_B, led));
        break;

        case(BSP_LED_2):
             return(PLIB_PORTS_PinGetLatched( PORTS_ID_0, PORT_CHANNEL_B, led));
        break;

        case(BSP_LED_3):
             return(PLIB_PORTS_PinGetLatched( PORTS_ID_0, PORT_CHANNEL_D, led));
        break;

        case(BSP_LED_4):
             return(PLIB_PORTS_PinGetLatched( PORTS_ID_0, PORT_CHANNEL_B, led));
        break;

        case(BSP_LED_5):
             return(PLIB_PORTS_PinGetLatched( PORTS_ID_0, PORT_CHANNEL_B, led));
        break;

        case(BSP_LED_6):
             return(PLIB_PORTS_PinGetLatched( PORTS_ID_0, PORT_CHANNEL_B, led));
        break;

        case(BSP_LED_7):
             return(PLIB_PORTS_PinGet( PORTS_ID_0, PORT_CHANNEL_E, led));
        break;
        default:
            // Invalid LED
            return 0;
        break;
    }
}

// *****************************************************************************
/* Function: 
    void BSP_LEDToggle(BSP_LED led);

  Summary:
    Toggles the state of the LED between BSP_LED_STATE_ON and BSP_LED_STATE_OFF.
  
  Description:
    This function toggles the state of the LED between BSP_LED_STATE_ON and
    BSP_LED_STATE_OFF.

  Remarks:
    Refer to bsp_config.h for usage information.
*/

void BSP_LEDToggle(BSP_LED led)
{
//    PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_A,led );
switch(led)
    {
        case(BSP_LED_1):
             PLIB_PORTS_PinToggle( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_2):
             PLIB_PORTS_PinToggle( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_3):
             PLIB_PORTS_PinToggle( PORTS_ID_0, PORT_CHANNEL_D, led);
        break;

        case(BSP_LED_4):
             PLIB_PORTS_PinToggle( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_5):
             PLIB_PORTS_PinToggle( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_6):
             PLIB_PORTS_PinToggle( PORTS_ID_0, PORT_CHANNEL_B, led);
        break;

        case(BSP_LED_7):
             PLIB_PORTS_PinToggle( PORTS_ID_0, PORT_CHANNEL_E, led);
        break;
        default:
            // Invalid LED
        break;
    }
}

// *****************************************************************************
/* Function: 
    void BSP_SwitchStateGet(BSP_SWITCH switch);

  Summary:
    Returns the present state (pressed or not pressed) of the specified switch.
  
  Description:
    This function returns the present state (pressed or not pressed) of the
    specified switch.

  Remarks:
    Refer to bsp_config.h for usage information.
*/

BSP_SWITCH_STATE BSP_SwitchStateGet( BSP_SWITCH bspSwitch )
{
    return (PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_B, bspSwitch));
}

// *****************************************************************************
/* Function:
    void BSP_LED_SetLightShow(BSP_LED_LIGHT_SHOW lightShow);

  Summary:
    Sets the LED light show to a configuration

  Description:
    This function will change the BSP LED configuration to certain "light shows"
    These light shows will indicate certain functions on the board.

  Remarks:
    Refer to bsp_config.h for usage information.
*/

void BSP_LED_LightShowSet(BSP_LED_LIGHT_SHOW lightShow)
{
    bspData.light_show = lightShow;
}

// *****************************************************************************
/* Function:
    void BSP_SYS_Tasks();

  Summary:
    Runs the applications BSP tasks such as LED and Switch control

  Description:
    This function will handle the LED state machine and switch state machine.

  Remarks:
    Refer to bsp_config.h for usage information.
*/

void BSP_SYS_Tasks ()
{
    BSP_LED_LightShow(bspData.light_show);
    BSP_SWITCH_Tasks();
}

// *****************************************************************************
/* Function:
    void BSP_LED_LightShow(void)

  Summary:
     Polling function to call different LED light shows

  Description:
    Each function will toggle the LEDs with different "light shows" that can be used
    for indications such as an error

  Precondition:
    Tick system is setup and LEDs are initialized

  Parameters:
   None

  Returns:
   None
*/

void BSP_LED_LightShow(BSP_LED_LIGHT_SHOW lightShow)
{
    static uint32_t ledTick = 0;
    static uint8_t count = 0;
    static uint8_t count2 = 0;
    static uint32_t connectionTimeout = 0;

    switch (lightShow)
    {
        case     BSP_LED_EASY_CONFIGURATION:
            if(SYS_TMR_TickCountGet() - ledTick >= 625)
            {
                ledTick = SYS_TMR_TickCountGet();
                BSP_LEDToggle(BSP_LED_5);
                BSP_LEDOff(BSP_LED_6);
            }
            break;

        case     BSP_LED_CONNECTING_TO_AP:
            if(SYS_TMR_TickCountGet() - connectionTimeout >= (1250 * 10))
            {
                connectionTimeout = SYS_TMR_TickCountGet();
                BSP_LED_LightShowSet(BSP_LED_CONNECTION_FAILED);
            }
            if (SYS_TMR_TickCountGet() - ledTick >= 150)
            {
                ledTick = SYS_TMR_TickCountGet();
                BSP_LEDOff(BSP_LED_5);
                BSP_LEDOn(BSP_LED_6);
                switch (count)
                {
                    case (0):
                        BSP_LEDOn(BSP_LED_1);
                        BSP_LEDOff(BSP_LED_2);
                        BSP_LEDOff(BSP_LED_3);
                        BSP_LEDOff(BSP_LED_4);
                        count++;
                        break;
                    case (1):
                        BSP_LEDOff(BSP_LED_1);
                        BSP_LEDOn(BSP_LED_2);
                        BSP_LEDOff(BSP_LED_3);
                        BSP_LEDOff(BSP_LED_4);
                        count++;
                        break;
                    case (2):
                        BSP_LEDOff(BSP_LED_1);
                        BSP_LEDOff(BSP_LED_2);
                        BSP_LEDOff(BSP_LED_3);
                        BSP_LEDOn(BSP_LED_4);
                        count++;
                        break;
                    case (3):
                        BSP_LEDOff(BSP_LED_1);
                        BSP_LEDOff(BSP_LED_2);
                        BSP_LEDOn(BSP_LED_3);
                        BSP_LEDOff(BSP_LED_4);
                        count = 0;
                        break;
                    default:
                        count = 0;
                        break;
                }

            }
            break;
            //Not implemented
        case     BSP_LED_CONNECTION_FAILED:
                BSP_LEDOff(BSP_LED_1);
                BSP_LEDOn(BSP_LED_2);
                BSP_LEDOff(BSP_LED_3);
                BSP_LEDOff(BSP_LED_4);
                BSP_LEDOn(BSP_LED_5);
                BSP_LEDOn(BSP_LED_6);

            if(SYS_TMR_TickCountGet() - connectionTimeout >= 4750)
            {
                connectionTimeout = SYS_TMR_TickCountGet();
                ledTick = SYS_TMR_TickCountGet();
                BSP_LED_LightShowSet(BSP_LED_CONNECTING_TO_AP);
            }
            break;
        case    BSP_LED_AP_CONNECTED:
                if(SYS_TMR_TickCountGet() - ledTick >= 625)
                {
                    count2++;
                    ledTick = SYS_TMR_TickCountGet();
                    BSP_LEDStateSet(BSP_LED_5, BSP_LEDStateGet(BSP_LED_6));
                    BSP_LEDToggle(BSP_LED_5);
                    BSP_LEDToggle(BSP_LED_6);
                    BSP_LEDOff(BSP_LED_1);
                    BSP_LEDOff(BSP_LED_2);
                    BSP_LEDOff(BSP_LED_3);
                    BSP_LEDOff(BSP_LED_4);
                }
            break;

        case    BSP_LED_SERVER_CONNECT_FAILED:
                BSP_LEDOn(BSP_LED_1);
                BSP_LEDOff(BSP_LED_2);
                BSP_LEDOff(BSP_LED_3);
                BSP_LEDOff(BSP_LED_4);
                BSP_LEDOn(BSP_LED_5);
                BSP_LEDOn(BSP_LED_6);
            break;

        case    BSP_LED_ALL_GOOD:
                BSP_LEDOff(BSP_LED_5);
                BSP_LEDOff(BSP_LED_6);
                ledTick = SYS_TMR_TickCountGet();
            // Idle state
            break;
            
        case    BSP_LED_TX:
            BSP_LEDOn(BSP_LED_6);
            if(SYS_TMR_TickCountGet() - ledTick >= 30)
                BSP_LED_LightShowSet(BSP_LED_ALL_GOOD);
            break;
            
        case    BSP_LED_RX:
            BSP_LEDOn(BSP_LED_5);
            if(SYS_TMR_TickCountGet() - ledTick >= 30)
                BSP_LED_LightShowSet(BSP_LED_ALL_GOOD);
            break; 
            
        case    BSP_LED_HTTP_400:
                BSP_LEDOff(BSP_LED_1);
                BSP_LEDOff(BSP_LED_2);
                BSP_LEDOn(BSP_LED_3);
                BSP_LEDOff(BSP_LED_4);
                BSP_LEDOn(BSP_LED_5);
                BSP_LEDOn(BSP_LED_6);
            break;

        case    BSP_LED_HTTP_500:
                BSP_LEDOff(BSP_LED_1);
                BSP_LEDOff(BSP_LED_2);
                BSP_LEDOff(BSP_LED_3);
                BSP_LEDOn(BSP_LED_4);
                BSP_LEDOn(BSP_LED_5);
                BSP_LEDOn(BSP_LED_6);
            break;

        case    BSP_LED_DNS_FAILED:
            BSP_LEDOn(BSP_LED_1);
            BSP_LEDOff(BSP_LED_2);
            BSP_LEDOn(BSP_LED_3);
            BSP_LEDOff(BSP_LED_4);
            BSP_LEDOn(BSP_LED_5);
            BSP_LEDOn(BSP_LED_6);
        break;
        
        case    BSP_LED_TCPIP_STACK_INIT_FAILURE:
            BSP_LEDOff(BSP_LED_1);
            BSP_LEDOn(BSP_LED_2);
            BSP_LEDOn(BSP_LED_3);
            BSP_LEDOff(BSP_LED_4);
            BSP_LEDOn(BSP_LED_5);
            BSP_LEDOn(BSP_LED_6);
        break;
        
        default:
            break;
    }
}

BSP_SWITCH_STATE BSP_SWITCH_SwitchTest(BSP_SWITCH switchId)
{
    switch(switchId)
    {
        case BSP_SWITCH_1:
          return (BSP_SWITCH_STATE)bspData.s1;
        break;
        case BSP_SWITCH_2:
          return (BSP_SWITCH_STATE)bspData.s2;
        break;
        case BSP_SWITCH_3:
          return (BSP_SWITCH_STATE)bspData.s3;
        break;
        case BSP_SWITCH_4:
          return (BSP_SWITCH_STATE)bspData.s4;
        break;
        default:
            return BSP_SWITCH_STATE_DEASSERTED;
    }
}

int32_t BSP_SWITCH_DeviceDebounce(BSP_SWITCH_DEBOUNCE_T *handle, uint32_t curVal)
{
    if(handle->timerActive==false)
    {
       handle->timerActive = true;
       handle->prevValue   = curVal;
       handle->startTick   = SYS_TMR_TickCountGet();
    }

    if ((SYS_TMR_TickCountGet() - handle->startTick) >= (1250
                         / BSP_SWITCH_MS_ELLAPSED_TIME_TO_HZ(handle->duration)) )
    {
        handle->timerActive=false;
        if(handle->prevValue == curVal)
          return handle->prevValue;
        else
          return curVal; // Return the assert value
    }
    return BSP_SWITCH_BUSY;  // Busy
}

void BSP_SWITCH_Tasks(void)
{
    int32_t val;
    //DGC_FINISH change hard coded ports to BSP_SWITCH_GetState
    val = BSP_SWITCH_DeviceDebounce(&bspData.switches[0], PORTBbits.RB2);
    if(val != BSP_SWITCH_BUSY)
    {
        bspData.s1 = val;
    }

    val = BSP_SWITCH_DeviceDebounce(&bspData.switches[1], PORTBbits.RB9);
    if(val != BSP_SWITCH_BUSY) 
    {
        bspData.s2 = val;
    }

    val = BSP_SWITCH_DeviceDebounce(&bspData.switches[2], PORTBbits.RB10);
    if(val != BSP_SWITCH_BUSY) 
    {
        bspData.s3 = val;
    }

    val = BSP_SWITCH_DeviceDebounce(&bspData.switches[3], PORTBbits.RB11);
    if(val != BSP_SWITCH_BUSY) 
    {
        bspData.s4 = val;
    }
}

/*******************************************************************************
 End of File
*/
