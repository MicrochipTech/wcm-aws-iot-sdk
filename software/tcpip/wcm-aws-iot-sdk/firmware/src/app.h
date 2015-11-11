/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "queue.h"
#include "driver/wifi/mrf24w/src/drv_wifi_config_data.h"
#include "driver/wifi/mrf24w/src/drv_wifi_easy_config.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT=0,
    APP_NVM_MOUNT_DISK,
    APP_NVM_ERASE_SERVER,
    APP_NVM_GET_SERVER,
    APP_TCPIP_WAIT_FOR_TCPIP_INIT,
    APP_WIFI_WAIT_FOR_SCAN,
    APP_TCPIP_WIFI_CONNECT,
    APP_TCPIP_WAIT_FOR_IP,
    APP_TCPIP_CONFIGURATION,
    APP_TCPIP_RESOLVE_DNS,
    APP_TCPIP_DNS_RESOLVED,
    APP_TCPIP_WAIT_DNS,
    APP_TCPIP_WAIT_SOCKET,
    APP_TCPIP_MQTT_CONNECT,
    APP_TCPIP_MQTT_CONNACK,
    APP_TCPIP_MQTT_PUBLISH,
    APP_TCPIP_MQTT_PUBACK,
    APP_TCPIP_MQTT_SUBSCRIBE,
    APP_TCPIP_MQTT_SUBACK,
    APP_TCPIP_MQTT_PINGREQ,
    APP_TCPIP_MQTT_PINGRESP,
    APP_TCPIP_MQTT_DISCONNECT,
    APP_TCPIP_MQTT_IDLE,
    APP_TCPIP_ERROR
           
} APP_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_STATES state;
    
    // Application hosts
    char aws_iot_host[256];
    IP_MULTI_ADDRESS  aws_iot_ipv4;
    NET_PRES_ADDRESS  aws_iot_ipv4test;
    char uuid[7];
    
    // Thing Data
    bool switch1;
    bool switch2;
    bool switch3;
    bool switch4; 
    bool led1;
    bool led2;
    bool led3;
    bool led4;
    uint32_t potentiometer;
    
    MY_QUEUE myQueue;
    uint32_t pubFlag;
    
    // Network variables
    NET_PRES_SKT_HANDLE_T socket;
    TCP_PORT port;
    void* ctx;
    void* ssl;
    uint8_t ipMode;
    uint8_t queryState;
    int8_t cyasslConnectionState;
    NET_PRES_SKT_ERROR_T error;
    int connection_valid;
    
    // Timers
    uint32_t potTimer;
    uint32_t tcpipTimeout;
    uint32_t mqttPingTimer;
    uint32_t mqttMessageTimer;
    
    // Drivers
    DRV_HANDLE nvmHandle;
    DRV_NVM_COMMAND_HANDLE      nvmCommandHandle;
    SYS_FS_MEDIA_GEOMETRY       *gAppNVMMediaGeometry;
    DRV_NVM_COMMAND_STATUS      nvmStatus;
    
} APP_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );


#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

