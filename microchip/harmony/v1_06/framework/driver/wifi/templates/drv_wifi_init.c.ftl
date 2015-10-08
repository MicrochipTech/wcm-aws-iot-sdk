<#--
/*******************************************************************************
  Wi-Fi Driver Initialization File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_wifi_init.c.ftl

  Summary:
    This file contains source code necessary to initialize the Wi-Fi driver.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the sysObj structure that
	contains the object handles to all the MPLAB Harmony module objects in
    the system.
*******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
-->

<#if CONFIG_DRV_WIFI_DEVICE == "MRF24WG">
    /* Enable MRF24W Interrupt */
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 WF_INT_PORT_CHANNEL,
                                 WF_INT_BIT_POS);

<#if CONFIG_BSP_PIC32MX_ETH_SK == true>
#if defined(MRF24W_USE_CN_INT)
	// If using CN9 interrupt, we should use a jump line to use another gpio to read CN9 pin status
	SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 WF_INT_PORT_CHANNEL_READ,
                                 WF_INT_BIT_POS_READ);
#endif

    // Enable change notification
    SYS_PORTS_ChangeNotificationEnable(PORTS_ID_0, CN9, SYS_PORTS_PULLUP_ENABLE);

    // Turn on change notification
    SYS_PORTS_ChangeNotificationGlobalEnable(PORTS_ID_0);

<#elseif CONFIG_BSP_PIC32MZ_EC_SK_MEB2 == true>
    /* Setup falling edge interrupt (this might change once system level
       API is implemented */
    PLIB_INT_ExternalFallingEdgeSelect(INT_ID_0, MRF_INT_SOURCE);

<#elseif CONFIG_BSP_PIC32MZ_EC_SK == true>
    // Temporary implementation
    ANSELEbits.ANSE8 = 0; /* configure IO pin as input and External Interrupt pin */
    //SYS_PORTS_PinModeSelect(PORTS_ID_0, PORTS_ANALOG_PIN_25, PORTS_PIN_MODE_DIGITAL);
    SYS_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_INT1, INPUT_PIN_RPE8);
    PLIB_INT_ExternalFallingEdgeSelect(INT_ID_0, MRF_INT_SOURCE);

</#if><#-- CONFIG_BSP -->
<#if CONFIG_3RDPARTY_RTOS_USED == "FreeRTOS_V8.x.x">
    if(!DRV_WIFI_RTOS_TaskInit())
    {
        return;
    }
</#if>
</#if><#-- CONFIG_DRV_WIFI_DEVICE == "MRF24WG" -->

<#if CONFIG_DRV_WIFI_DEVICE == "MRF24WN">
<#if CONFIG_BSP_PIC32MZ_EC_SK == true>
    /* Enable MRF24W Interrupt */
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 WF_INT_PORT_CHANNEL,
                                 WF_INT_BIT_POS);

    // Temporary implementation
    ANSELEbits.ANSE8 = 0; /* configure IO pin as input and External Interrupt pin */
    //SYS_PORTS_PinModeSelect(PORTS_ID_0, PORTS_ANALOG_PIN_25, PORTS_PIN_MODE_DIGITAL);
    SYS_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_INT1, INPUT_PIN_RPE8);
    PLIB_INT_ExternalFallingEdgeSelect(INT_ID_0, MRF_INT_SOURCE);
</#if><#-- CONFIG_BSP -->
</#if><#-- CONFIG_DRV_WIFI_DEVICE == "MRF24WN" -->
<#--
/*******************************************************************************
 End of File
*/
-->
