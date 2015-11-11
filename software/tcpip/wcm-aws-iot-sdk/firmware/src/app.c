/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "config.h"
#include "MQTTPacket/src/MQTTPacket.h"
#include <cyassl/ssl.h>
#include "parson.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define IOT_WIFI_G_AWS_IOT_STARTER_KIT_APP_VERSION "0.3.0"
#define MQTT_KEEP_ALIVE 60
#define MQTT_PING_REQ 45
#define MQTT_PING_RESP_TIMEOUT 10

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
extern BSP_DATA bspData;

extern bool g_scan_done; // WF_PRESCAN    This will be set wheneven event scan results are ready.
extern bool g_prescan_waiting; // WF_PRESCAN    This is used only to allow prescan once.
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback funtions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
bool APP_NVM_EraseConfig(void);
bool APP_NVM_ReadConfig(void);
bool APP_NVM_WriteConfig(void);

/* TODO:  Add any necessary local functions.
*/
void handler_topic_delta(unsigned char * payload);

bool APP_TIMER_Expired(uint32_t * timer, uint32_t seconds)
{
    if((SYS_TMR_TickCountGet() - *timer) > (seconds * 1000))
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

bool APP_TIMER_Set(uint32_t * timer)
{
    *timer = SYS_TMR_TickCountGet();
    return true;
}
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    // Your AWS IoT Service Host
    memset(appData.aws_iot_host, '\0', sizeof(appData.aws_iot_host));
    appData.port = 8883;
    appData.tcpipTimeout = 0;
    init_queue(&appData.myQueue);
    
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{  
    NET_PRES_SKT_ERROR_T error;

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	unsigned char buf[1024];
	int buflen = sizeof(buf);
	int msgid = 1;
	MQTTString topicStringUpdate = MQTTString_initializer;
    MQTTString topicStringDelta = MQTTString_initializer;
    MQTTString topicStringError = MQTTString_initializer;
	int req_qos = 0;
	int len = 0;
    
    // Unique client ID for each MQTT connection
    data.clientID.cstring = "000001iot";
	data.keepAliveInterval = MQTT_KEEP_ALIVE;
	data.cleansession = 1;
    
    // To Shadow Topic
    // This is the thing you created and are subscribing to
    topicStringUpdate.cstring = "$aws/things/000001iot/shadow/update";
    
    // From Shadow Topic
    topicStringDelta.cstring = "$aws/things/000001iot/shadow/update/delta";
          
    // Send a publish for each switch press
    // If a switch is detected, we set the flag to publish its value and put this
    // publish onto the message queue
    MY_QUEUE_OBJECT publishObject = {false, false, false, false, false, false, false, false, false};
    if(BSP_SWITCH_SwitchTest(BSP_SWITCH_1) != bspData.previousStateS1)
    {
        appData.pubFlag = 1;
        publishObject.s1 = true;
        bspData.previousStateS1 = BSP_SWITCH_SwitchTest(BSP_SWITCH_1);
    }
    if(BSP_SWITCH_SwitchTest(BSP_SWITCH_2) != bspData.previousStateS2)
    {
        appData.pubFlag = 1;
        publishObject.s2 = true;
        bspData.previousStateS2 = BSP_SWITCH_SwitchTest(BSP_SWITCH_2);
    }
    if(BSP_SWITCH_SwitchTest(BSP_SWITCH_3) != bspData.previousStateS3)
    {
        appData.pubFlag = 1;
        publishObject.s3 = true;
        bspData.previousStateS3 = BSP_SWITCH_SwitchTest(BSP_SWITCH_3);
    }
    if(BSP_SWITCH_SwitchTest(BSP_SWITCH_4) != bspData.previousStateS4)
    {
        appData.pubFlag = 1;
        publishObject.s4 = true;
        bspData.previousStateS4 = BSP_SWITCH_SwitchTest(BSP_SWITCH_4);
    }
    
    // ADC Filtering
    static uint16_t adcCurrent;
    adcCurrent = adcCurrent + ((PLIB_ADC_ResultGetByIndex(ADC_ID_1, 0) - adcCurrent)>>4);
    if((SYS_TMR_TickCountGet() - appData.potTimer) > 500)
    {
        appData.potTimer = SYS_TMR_TickCountGet();
        {
            // If we have a change in the ADC value, set the publish flag
            if(appData.potentiometer != adcCurrent)
            {
                publishObject.potentiometer_value = true;
                appData.potentiometer = adcCurrent;
                appData.pubFlag = 1;
            }
        }
    }
    // If any of the switches or potentiometer flags for a publish, we queue 
    // the publish object onto the queue to be processed
    if(appData.pubFlag == 1)
    {
        enqueue((&appData.myQueue), publishObject);
        appData.pubFlag = 0;
    }

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            appData.state = APP_NVM_MOUNT_DISK;
            break;        
        }
        
        case APP_NVM_MOUNT_DISK:
        {
            if(SYS_FS_Mount(SYS_FS_NVM_VOL, LOCAL_WEBSITE_PATH_FS, MPFS2, 0, NULL)  == 0)
            {
                appData.state = APP_NVM_ERASE_SERVER;
            }
            else
            {
                ; // Continue, NVM is mounting...
            }
            break;
        }
        
        /* Erase server configuration if needed */
        case APP_NVM_ERASE_SERVER:
        {
            // If switch 2 and switch 3 are pressed, then we erase the server from NVM
            if( (BSP_SwitchStateGet(BSP_SWITCH_3) == BSP_SWITCH_STATE_ASSERTED) && (BSP_SwitchStateGet(BSP_SWITCH_2) == BSP_SWITCH_STATE_ASSERTED) )
            {
                APP_NVM_EraseConfig();
                DRV_WIFI_ConfigDataErase(); // Erase the stored config
                DRV_WIFI_ConfigDataLoad();  // Load the erased data to clear out initialized config
                BSP_LED_LightShowSet(BSP_LED_EASY_CONFIGURATION);
            }
            appData.state = APP_NVM_GET_SERVER;
            break;
        }
        
        /* Read server address from NVM */
        case APP_NVM_GET_SERVER:
        {     
            if(APP_NVM_ReadConfig() != true)
                break;  
            appData.state = APP_TCPIP_WAIT_FOR_TCPIP_INIT;
            break;
        }
        
        // Wait for the TCPIP stack to be initialized
        case APP_TCPIP_WAIT_FOR_TCPIP_INIT:
        {
            SYS_STATUS tcpipStat;
            tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if(tcpipStat < 0)
            {   // some error occurred
                appData.state = APP_TCPIP_WAIT_FOR_TCPIP_INIT;
                BSP_LED_LightShowSet(BSP_LED_TCPIP_STACK_INIT_FAILURE);
            }
            else if(tcpipStat == SYS_STATUS_READY)
            {
                if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
                {
                    // Retrieve MAC address and store it into app data to be used later
                    uint8_t v[6];
                    DRV_WIFI_MacAddressGet((uint8_t *)(v));
                    sprintf(appData.uuid, "%02x%02x%02x", v[3], v[4], v[5]); // convert to string
                    //Set the SSID for SoftAP mode to have the last 6 digits of mac address
                    sprintf((char *)p_wifi_ConfigData->netSSID,"%02x%02x%02x_IoT", v[3], v[4], v[5]);
                    appData.state = APP_WIFI_WAIT_FOR_SCAN;
                }
                else
                {
                    BSP_LED_LightShowSet(BSP_LED_CONNECTING_TO_AP);
                    appData.state = APP_TCPIP_WAIT_FOR_IP;
                }
                APP_TIMER_Set(&appData.tcpipTimeout);
            }
            break;
        }
        
        case APP_WIFI_WAIT_FOR_SCAN:
        {
            if (g_scan_done)
            {
                if (g_prescan_waiting)
                {
                    if(IS_SCAN_STATE_DISPLAY(SCANCXT.scanState) && (SCANCXT.numScanResults > 0))
                    { 
                        // Display scan results on console if needed
                        SCANCXT.displayIdx = 0;
                        appData.state = APP_TCPIP_WIFI_CONNECT;
                    }
                    else if (IS_SCAN_STATE_DISPLAY(SCANCXT.scanState) && (SCANCXT.numScanResults == 0))
                    {
                        // No AP Found - Add in console messages if needed
                        appData.state = APP_TCPIP_WIFI_CONNECT;
                    }
                    else
                    {
                        // Prescan Failed - Add in console messages if needed
                        appData.state = APP_TCPIP_WIFI_CONNECT;
                    }
                    g_prescan_waiting = false;
                }
            }
            break;
        }
            
        case APP_TCPIP_WIFI_CONNECT:
        {
            #if (WF_DEFAULT_NETWORK_TYPE == DRV_WIFI_NETWORK_TYPE_SOFT_AP) && (WF_SOFTAP_CHECK_LINK_STATUS == DRV_WIFI_ENABLED)
                DRV_WIFI_SetLinkDownThreshold(WF_SOFTAP_LINK_FAILURE_THRESHOLD);
            #endif

            if (DRV_WIFI_ConnectStateMachine() == TCPIP_MAC_RES_OK)
            {
                appData.state = APP_TCPIP_WAIT_FOR_IP;
            }
            break;
        }
        
        // Wait for TCPIP to retrieve an IP address
        case APP_TCPIP_WAIT_FOR_IP:
        {
            int                 i, nNets;
            TCPIP_NET_HANDLE    netH;
            IPV4_ADDR           ipAddr;
            nNets = TCPIP_STACK_NumberOfNetworksGet();
            if(APP_TIMER_Expired(&appData.tcpipTimeout, 10))
            {
                APP_TIMER_Set(&appData.tcpipTimeout);
                BSP_LED_LightShowSet(BSP_LED_CONNECTION_FAILED);
                break;
            }
            
            for (i = 0; i < nNets; i++)
            {
                netH = TCPIP_STACK_IndexToNet(i);
                ipAddr.Val = TCPIP_STACK_NetAddress(netH);
                if(0 != ipAddr.Val)
                {
                    if (ipAddr.v[0] != 0 && ipAddr.v[0] != 169) // Wait for a Valid IP
                    {
                        appData.state = APP_TCPIP_CONFIGURATION;
                    }
                }
            } 
           
            break;
        }
        
        case APP_TCPIP_CONFIGURATION:
        {
            if(p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
            {
                break;
            }
            else
            {
                // Write the server we retrieved from user
                if (APP_NVM_WriteConfig() != true)
                {
                    break;
                }
                else
                {
                    TCPIP_NET_HANDLE    netH;
                    int i, nNets;

                    nNets = TCPIP_STACK_NumberOfNetworksGet();
                    for (i = 0; i < nNets; i++)
                    {
                        netH = TCPIP_STACK_IndexToNet(i);
                    }
                    appData.state = APP_TCPIP_RESOLVE_DNS;  
                }       
            }
            break;
        }
        
        // Begin resolving the AWS IoT service domain name
        case APP_TCPIP_RESOLVE_DNS:
        {
            TCPIP_DNS_RESULT result;
            result = TCPIP_DNS_Resolve(appData.aws_iot_host, DNS_TYPE_A);
            
            //If there is an error, set error code on LEDs
            if(result != DNS_RES_OK)
            {
                BSP_LED_LightShowSet(BSP_LED_DNS_FAILED);
                break;
            }
            appData.state = APP_TCPIP_WAIT_DNS;
            APP_TIMER_Set(&appData.tcpipTimeout);
            break;
        }
        
        // Wait for domain name to be resolved
        case APP_TCPIP_WAIT_DNS:
        {
            // If it takes longer than 10 seconds to resolve domain name, timeout
            if(APP_TIMER_Expired(&appData.tcpipTimeout, 5))
            {
                BSP_LED_LightShowSet(BSP_LED_DNS_FAILED);
                appData.state = APP_TCPIP_RESOLVE_DNS;
                break;
            }
            
            TCPIP_DNS_RESULT result = TCPIP_DNS_IsResolved(appData.aws_iot_host, &appData.aws_iot_ipv4);
            switch(result)
            {
                case DNS_RES_PENDING:
                    break;
                case DNS_RES_OK:
                        appData.state = APP_TCPIP_DNS_RESOLVED;
                    break;
                       
                case DNS_RES_SERVER_TMO:
                case DNS_RES_NO_ENTRY:
                default:
                    break;    
            }
            break;
        }
        
        case APP_TCPIP_DNS_RESOLVED:
        {
            // Domain name is resolved into IP address, now we open a socket
            // Default port is 8883 for MQTT over SSL
            appData.socket = NET_PRES_SKT_Open(0, NET_PRES_SKT_ENCRYPTED_STREAM_CLIENT, IP_ADDRESS_TYPE_IPV4, appData.port, (NET_PRES_ADDRESS *)&appData.aws_iot_ipv4, &appData.error);
            NET_PRES_SKT_WasReset(appData.socket, &appData.error);
            if(appData.socket == INVALID_SOCKET)
            {
                BSP_LED_LightShowSet(BSP_LED_SERVER_CONNECT_FAILED);
                break;
            }
            APP_TIMER_Set(&appData.tcpipTimeout);
            appData.state = APP_TCPIP_WAIT_SOCKET;
            break;
        }
        
        case APP_TCPIP_WAIT_SOCKET:
        {
            // Wait for a valid socket connection, else timeout if none present
            if(APP_TIMER_Expired(&appData.tcpipTimeout, 20))
            {
                BSP_LED_LightShowSet(BSP_LED_SERVER_CONNECT_FAILED);
                APP_TIMER_Set(&appData.tcpipTimeout);
                appData.state = APP_TCPIP_DNS_RESOLVED;
                break;
            }
            
            if(!NET_PRES_SKT_IsConnected(appData.socket, &appData.error))
            {
                break;   
            }
            
            if(NET_PRES_SKT_WasReset(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
            
            if (NET_PRES_SKT_IsNegotiatingEncryption(appData.socket, &appData.error))
            {
                break;
            }
            if (!NET_PRES_SKT_IsSecure(appData.socket, &appData.error))
            {
                BSP_LED_LightShowSet(BSP_LED_SERVER_CONNECT_FAILED);
                appData.state = APP_TCPIP_ERROR;
                break;
            }
            // At this point we have an SSL encrypted socket to the AWS IoT service
            // Lets send our current LED status
            BSP_LED_LightShowSet(BSP_LED_ALL_GOOD);
            appData.led1 ? BSP_LEDOn(BSP_LED_1) : BSP_LEDOff(BSP_LED_1);
            appData.led2 ? BSP_LEDOn(BSP_LED_2) : BSP_LEDOff(BSP_LED_2);
            appData.led3 ? BSP_LEDOn(BSP_LED_3) : BSP_LEDOff(BSP_LED_3);
            appData.led4 ? BSP_LEDOn(BSP_LED_4) : BSP_LEDOff(BSP_LED_4);
            
            MY_QUEUE_OBJECT currentLeds = {false, false, false, false, false, true, true, true, true};
            
            enqueue((&appData.myQueue), currentLeds);
            appData.state = APP_TCPIP_MQTT_CONNECT;
            break;
        }
        
        case APP_TCPIP_MQTT_CONNECT:
        {
            // We connect as an MQTT client so we send a CONNECT packet
            if(NET_PRES_SKT_WasReset(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
             
            if(NET_PRES_SKT_WriteIsReady(appData.socket, &error) == 0)
            {
                break;
            }
            
            // Serialize an MQTT CONNECT packet
            len = MQTTSerialize_connect(buf, buflen, &data);
            
            // Write the packet to the network TX buffer to be sent
            NET_PRES_SKT_Write(appData.socket, (uint8_t*)buf, len, &error);
            APP_TIMER_Set(&appData.mqttMessageTimer);
            appData.state = APP_TCPIP_MQTT_CONNACK;
            break;
        }
        
        case APP_TCPIP_MQTT_CONNACK:
        {
            if(APP_TIMER_Expired(&appData.mqttMessageTimer, 20))
            {
                BSP_LED_LightShowSet(BSP_LED_SERVER_CONNECT_FAILED);
                appData.state = APP_TCPIP_DNS_RESOLVED;
                break;
            }
            
            if(NET_PRES_SKT_WasReset(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
             
            if (NET_PRES_SKT_ReadIsReady(appData.socket,&error) == 0)
            {
                if (NET_PRES_SKT_WasReset(appData.socket, &error))
                {
                    appData.state = APP_TCPIP_ERROR;
                }
                break;
            }
            
            // Read from network RX buffer
            uint16_t res = NET_PRES_SKT_Read(appData.socket, (uint8_t*)buf, sizeof(buf), &error);
            
            // Check to make sure we receive an CONNACK after our CONNECT packet is sent
            unsigned char sessionPresent, connack_rc;
            if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
            else
            {
                // We got a CONNACK, lets set our subscriptions
                appData.state = APP_TCPIP_MQTT_SUBSCRIBE;
                APP_TIMER_Set(&appData.mqttPingTimer);
            }
            break;
        }
        
        case APP_TCPIP_MQTT_SUBSCRIBE:
        {       
            if(NET_PRES_SKT_WasReset(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
             
             if(NET_PRES_SKT_WriteIsReady(appData.socket, &error) == 0)
            {
                break;
            }
            // Serialize an MQTT packet using the variables set at the top
            len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicStringDelta, &req_qos);
            
            // Send packet to network TX buffer to be sent
            NET_PRES_SKT_Write(appData.socket, (uint8_t*)buf, len, &error);
            appData.state = APP_TCPIP_MQTT_SUBACK;
            APP_TIMER_Set(&appData.mqttMessageTimer);
            break;
        }
        
        case APP_TCPIP_MQTT_SUBACK:
        {
            if(APP_TIMER_Expired(&appData.mqttMessageTimer, 20))
            {
                BSP_LED_LightShowSet(BSP_LED_SERVER_CONNECT_FAILED);
                appData.state = APP_TCPIP_ERROR;
                break;
            }
            
            if(NET_PRES_SKT_WasReset(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
             
            if (NET_PRES_SKT_ReadIsReady(appData.socket,&error) == 0)
            {
                if (NET_PRES_SKT_WasReset(appData.socket, &error))
                {
                    appData.state = APP_TCPIP_ERROR;
                }
                break;
            }
            
            // Read network RX buffer to check for SUBACK packet 
            uint16_t res = NET_PRES_SKT_Read(appData.socket, (uint8_t*)buf, sizeof(buf), &error);
            unsigned short submsgid;
            int subcount;
            int granted_qos;
            res = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
            if(granted_qos != 0)
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
            else
            {
                appData.state = APP_TCPIP_MQTT_IDLE;
                APP_TIMER_Set(&appData.mqttPingTimer);
            }
            break;
        }
        
        case APP_TCPIP_MQTT_IDLE:
        {
            // We are now in an IDLE state.  
            // At this point we have subscribed to any topics we wish and will continuously check
            // the network RX buffer for data.  When we receive data, if its a valid MQTT publish
            // packet, we will route the packet to the proper topic handler by checking which
            // topic was published to our board.  The topic handler will process the JSON payload
            // and build a JSON payload to be reported back to the AWS IoT service.  The IDLE state
            // will also continuously check the message queue to see if there are any PUBLISH
            // messages that need to be sent.  This example state machine uses a QoS 0 for now, so
            // we don't have to check for a PUBACK.  Once the publishes are all sent from the queue
            // we don't do anything, and check our timer to make sure our keepalive value is not 
            // being hit.  The keepalive value is 60 seconds, so we arbitrarily send a PING at 45
            // seconds to keep the socket to the server open, and our MQTT client connected.
            if(!NET_PRES_SKT_IsConnected(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_DNS_RESOLVED;
                break;   
            }
            
            if(APP_TIMER_Expired(&appData.mqttPingTimer, MQTT_PING_REQ))
            {
                APP_TIMER_Set(&appData.mqttPingTimer);
                appData.state = APP_TCPIP_MQTT_PINGREQ;
                break;
            }
            
            if(NET_PRES_SKT_WasReset(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
             
            if (NET_PRES_SKT_ReadIsReady(appData.socket,&error) == 0)
            {
                if (NET_PRES_SKT_WasReset(appData.socket, &error))
                {
                    appData.state = APP_TCPIP_ERROR;
                    break;
                }
                
                if(!queue_isEmpty(&(appData.myQueue)))
                { 
                    appData.state = APP_TCPIP_MQTT_PUBLISH;
                    break;
                }
                break;
            }
            
            unsigned char dup;
			int qos;
			unsigned char retained;
			unsigned short msgid;
			int payloadlen_in;
			unsigned char* payload_in;
			int rc;
			MQTTString receivedTopic;
            // Read network RX buffer for publishes
            int16_t res = NET_PRES_SKT_Read(appData.socket, (uint8_t*)buf, sizeof(buf), &error);
            if(res < 0)
            {
               appData.state = APP_TCPIP_ERROR;
               break;  
            }
            if(error != NET_PRES_SKT_OK)
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
			rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
					&payload_in, &payloadlen_in, buf, buflen);
            BSP_LED_LightShowSet(BSP_LED_RX);
            
            //Check to see if the topic that came in matches what we subscribed to,
            // if it does (ret = 1) then call our message handler and pass it the payload
            int ret = MQTTPacket_equals(&receivedTopic, topicStringDelta.cstring);
            if(ret){
                handler_topic_delta(payload_in);
            }

            break;
        }
        
        case APP_TCPIP_MQTT_PINGREQ:
        {
            // We have no activity so send a PINREQ to keep the connection alive
            APP_TIMER_Set(&appData.mqttPingTimer);
            BSP_LED_LightShowSet(BSP_LED_TX);
            if(NET_PRES_SKT_WasReset(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
             
            if(NET_PRES_SKT_WriteIsReady(appData.socket, &error) == 0)
            {
                break;
            }
            len = MQTTSerialize_pingreq(buf, buflen);
            NET_PRES_SKT_Write(appData.socket, (uint8_t*)buf, len, &error);
            appData.state = APP_TCPIP_MQTT_PINGRESP;
            break;
        }
        
        case APP_TCPIP_MQTT_PINGRESP:
        {   
            // Wait for PINGRESP from service
             if(APP_TIMER_Expired(&appData.mqttPingTimer, MQTT_PING_RESP_TIMEOUT))
            {
                APP_TIMER_Set(&appData.mqttPingTimer);
                appData.state = APP_TCPIP_ERROR;
                break;
            }
             
            if(NET_PRES_SKT_WasReset(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
             
            if (NET_PRES_SKT_ReadIsReady(appData.socket,&error) == 0)
            {
                if (NET_PRES_SKT_WasReset(appData.socket, &error))
                {
                    appData.state = APP_TCPIP_ERROR;
                }
                break;
            }
            
            uint16_t res = NET_PRES_SKT_Read(appData.socket, (uint8_t*)buf, sizeof(buf), &error);
            int rc = MQTTDeserialize_pingresp((uint8_t*)buf, sizeof(buf));
            if(rc == 1)
            {
                APP_TIMER_Set(&appData.mqttPingTimer);
                appData.state = APP_TCPIP_MQTT_IDLE;
                BSP_LEDOff(BSP_LED_6);
            }
            break;
        }

        case APP_TCPIP_MQTT_PUBLISH:
        {
            if(NET_PRES_SKT_WasReset(appData.socket, &appData.error))
            {
                appData.state = APP_TCPIP_ERROR;
                break;
            }
             
            if(NET_PRES_SKT_WriteIsReady(appData.socket, &error) == 0)
            {
                break;
            }
            
            // We build our PUBLISH payload
            char reportedPayload[200];
            // Get ADC value
            char adcBuffer[5] = {'\0'};
                sprintf(adcBuffer, "%4d", appData.potentiometer);
                
            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            char *serialized_string = NULL;
            
            // Build our JSON payload by checking values and putting them into the JSON object
            if(appData.myQueue.message_queue[appData.myQueue.front].s1)
            {
                json_object_dotset_string(root_object, "state.reported.button1", (bspData.previousStateS1 ? "up" : "down"));
            }
            
            if(appData.myQueue.message_queue[appData.myQueue.front].s2)
            {
                json_object_dotset_string(root_object, "state.reported.button2", (bspData.previousStateS2 ? "up" : "down"));
            }
            
            if(appData.myQueue.message_queue[appData.myQueue.front].s3)
            {
                json_object_dotset_string(root_object, "state.reported.button3", (bspData.previousStateS3 ? "up" : "down"));
            }
            
            if(appData.myQueue.message_queue[appData.myQueue.front].s4)
            {
                json_object_dotset_string(root_object, "state.reported.button4", (bspData.previousStateS4 ? "up" : "down"));
            }
            
            if(appData.myQueue.message_queue[appData.myQueue.front].led1)
            {
                json_object_dotset_string(root_object, "state.reported.led1", (appData.led1 ? "on" : "off"));
            }
            
            if(appData.myQueue.message_queue[appData.myQueue.front].led2)
            {
                json_object_dotset_string(root_object, "state.reported.led2", (appData.led2 ? "on" : "off"));
            }
            
            if(appData.myQueue.message_queue[appData.myQueue.front].led3)
            {
                json_object_dotset_string(root_object, "state.reported.led3", (appData.led3 ? "on" : "off"));
            }
            
            if(appData.myQueue.message_queue[appData.myQueue.front].led4)
            {
                json_object_dotset_string(root_object, "state.reported.led4", (appData.led4 ? "on" : "off"));
            }
            
            if(appData.myQueue.message_queue[appData.myQueue.front].potentiometer_value)
            {
                json_object_dotset_string(root_object, "state.reported.potentiometer", adcBuffer);
            }
            
            // Serialize this JSON payload into a string
            serialized_string = json_serialize_to_string(root_value);
            sprintf(reportedPayload, serialized_string);
            json_free_serialized_string(serialized_string);
            json_value_free(root_value);
              
            int  reportedPayload_len = strlen(reportedPayload);
            
            // Serialize this JSON payload into an MQTT PUBLISH and send to network TX buffer
            len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicStringUpdate, (unsigned char*)reportedPayload, reportedPayload_len);
            NET_PRES_SKT_Write(appData.socket, (uint8_t*)buf, len, &error);
            BSP_LED_LightShowSet(BSP_LED_TX);
            APP_TIMER_Set(&appData.mqttPingTimer);
            dequeue(&appData.myQueue);
            appData.state = APP_TCPIP_MQTT_IDLE;
            break;
        }
        
        case APP_TCPIP_MQTT_PUBACK:
        {
            // No PUBACK for this demo, we use QoS 0
            break;
        }
        case APP_TCPIP_ERROR:
        {
            // Set error code and close network connection and reset state
            BSP_LED_LightShowSet(BSP_LED_SERVER_CONNECT_FAILED);
            NET_PRES_SKT_Close(appData.socket, &error);
            appData.state = APP_TCPIP_DNS_RESOLVED;
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

void handler_topic_delta(unsigned char * payload)
{
    MY_QUEUE_OBJECT publishObject = {false, false, false, false, false, false, false, false, false};
    JSON_Value *root_value = json_parse_string(payload);
    if (json_value_get_type(root_value) != JSONObject)
        return;
    JSON_Object * tObject = json_value_get_object(root_value);
    if(json_object_dotget_string(tObject, "state.led1") != NULL)
    {
        if(strcmp(json_object_dotget_string(tObject, "state.led1"), "on") == 0)
        {
            BSP_LEDOn(BSP_LED_1);
            appData.led1 = true;
            publishObject.led1 = true;
        }
        else if (strcmp(json_object_dotget_string(tObject, "state.led1"), "off") == 0)
        {
            BSP_LEDOff(BSP_LED_1);
            appData.led1 = false;
            publishObject.led1 = true;
        }
    }
    if(json_object_dotget_string(tObject, "state.led2") != NULL)
    {
        if(strcmp(json_object_dotget_string(tObject, "state.led2"), "on") == 0)
        {
            BSP_LEDOn(BSP_LED_2);
            appData.led2 = true;
            publishObject.led2 = true;
        }
        else if (strcmp(json_object_dotget_string(tObject, "state.led2"), "off") == 0)
        {
            BSP_LEDOff(BSP_LED_2);
            appData.led2 = false;
            publishObject.led2 = true;
        }
    }
    if(json_object_dotget_string(tObject, "state.led3") != NULL)
    {
        if(strcmp(json_object_dotget_string(tObject, "state.led3"), "on") == 0)
        {
            BSP_LEDOn(BSP_LED_3);
            appData.led3 = true;
            publishObject.led3 = true;
        }
        else if (strcmp(json_object_dotget_string(tObject, "state.led3"), "off") == 0)
        {
            BSP_LEDOff(BSP_LED_3);
            appData.led3 = false;
            publishObject.led3 = true;
        }
    }
    if(json_object_dotget_string(tObject, "state.led4") != NULL)
    {
        if(strcmp(json_object_dotget_string(tObject, "state.led4"), "on") == 0)
        {
            BSP_LEDOn(BSP_LED_4);
            appData.led4 = true;
            publishObject.led4 = true;
        }
        else if (strcmp(json_object_dotget_string(tObject, "state.led4"), "off") == 0)
        {
            BSP_LEDOff(BSP_LED_4);
            appData.led4 = false;
            publishObject.led4 = true;
        }
    }
    json_value_free(root_value);
    if( publishObject.led1 == true || 
        publishObject.led2 == true || 
        publishObject.led3 == true ||
        publishObject.led4 == true )
    {
        enqueue((&appData.myQueue), publishObject);
    }   
}

bool APP_NVM_EraseConfig(void)
{
    int tmp;
    
    appData.nvmHandle = DRV_NVM_Open(0, DRV_IO_INTENT_READWRITE);
    if(DRV_HANDLE_INVALID == appData.nvmHandle)
    {
        return false;
    }

    appData.gAppNVMMediaGeometry = DRV_NVM_GeometryGet(appData.nvmHandle);
    if(NULL ==appData. gAppNVMMediaGeometry)
    {
        return false;
    }  

    tmp = DRV_SERVER_NVM_SPACE_ADDR * (appData.gAppNVMMediaGeometry->geometryTable[2].numBlocks) / (DRV_NVM_MEDIA_SIZE * 1024);
    
    DRV_NVM_Erase(appData.nvmHandle, &appData.nvmCommandHandle,  tmp, 1);
    if(appData.nvmCommandHandle == DRV_NVM_COMMAND_HANDLE_INVALID)
    {
        return false;
    }

    if(DRV_NVM_COMMAND_COMPLETED == DRV_NVM_CommandStatus(appData.nvmHandle, appData.nvmCommandHandle))
    {
        DRV_NVM_Close(appData.nvmHandle);
        return true;
    }
    else
    {
        return false;
    }
}

bool APP_NVM_ReadConfig(void)
{
    static uint8_t state_read = 0;
    
    switch (state_read)
    {
        case 0:
        {
            appData.nvmHandle = DRV_NVM_Open(0, DRV_IO_INTENT_READWRITE);
            if(DRV_HANDLE_INVALID == appData.nvmHandle)
            {
                return false;
            }
            
            appData.gAppNVMMediaGeometry = DRV_NVM_GeometryGet(appData.nvmHandle);
            if(NULL == appData.gAppNVMMediaGeometry)
            {
                return false;
            }

            DRV_NVM_Read(appData.nvmHandle, &appData.nvmCommandHandle, appData.aws_iot_host, DRV_SERVER_NVM_SPACE_ADDR, sizeof(appData.aws_iot_host));
            if(DRV_NVM_COMMAND_HANDLE_INVALID == appData.nvmCommandHandle)
            {
                return false;
            }    
            
            state_read = 1;
            return false;
        }

        case 1:
        {
            if(DRV_NVM_COMMAND_COMPLETED == DRV_NVM_CommandStatus(appData.nvmHandle, appData.nvmCommandHandle))
            {
                DRV_NVM_Close(appData.nvmHandle);
                state_read = 0;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool APP_NVM_WriteConfig(void)
{   
    static uint8_t state_write = 0;
    int tmp;
                
    switch (state_write)
    {
        case 0:
        {
            appData.nvmHandle = DRV_NVM_Open(0, DRV_IO_INTENT_READWRITE);
            if(DRV_HANDLE_INVALID == appData.nvmHandle){
                return false;
            }

            appData.gAppNVMMediaGeometry = DRV_NVM_GeometryGet(appData.nvmHandle);
            if(NULL ==appData. gAppNVMMediaGeometry){
                return false;
            }  

            tmp = DRV_SERVER_NVM_SPACE_ADDR * (appData.gAppNVMMediaGeometry->geometryTable[2].numBlocks) / (DRV_NVM_MEDIA_SIZE * 1024);
            DRV_NVM_Erase(appData.nvmHandle, &appData.nvmCommandHandle,  tmp, 1);
            if(appData.nvmCommandHandle == DRV_NVM_COMMAND_HANDLE_INVALID){
                return false;
            }
            state_write = 1;
            return false;
        }

        case 1:
        {
            if(DRV_NVM_COMMAND_COMPLETED == DRV_NVM_CommandStatus(appData.nvmHandle, appData.nvmCommandHandle))
            {
                state_write = 2;
                return false;
            }
            else
            {
                return false;
            }
        }
        
        case 2:
        {
            tmp = DRV_SERVER_NVM_SPACE_ADDR * (appData.gAppNVMMediaGeometry->geometryTable[1].numBlocks) / (DRV_NVM_MEDIA_SIZE * 1024);
            DRV_NVM_Write(appData.nvmHandle, &appData.nvmCommandHandle, (uint8_t *)appData.aws_iot_host, tmp, 1);
            if(DRV_NVM_COMMAND_HANDLE_INVALID == appData.nvmCommandHandle)
            {
                return false;
            }
            else
            {
                state_write = 3;
            }
        }
        
        case 3:
        {
            if(DRV_NVM_COMMAND_COMPLETED == DRV_NVM_CommandStatus(appData.nvmHandle, appData.nvmCommandHandle))
            {
                DRV_NVM_Close(appData.nvmHandle);
                state_write = 0;
                return true;
            }
            else
            {
                return false;
            }
        }
        
    }
    return false;
}

int MQTTDeserialize_pingresp(unsigned char* buf, int buflen)
{
    unsigned char* curdata = buf;
	int rc = 0;
    MQTTHeader header = {0};

    header.byte = readChar(&curdata);
	if (header.bits.type != PINGRESP)
		return 0;
    else
        return 1;
}
/*******************************************************************************
 End of File
 */
