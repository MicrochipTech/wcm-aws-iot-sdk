 /*******************************************************************************
  USB HOST HUB subclass definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hub_local.h

  Summary:
    USB HOST HUB definitions

  Description:
    This file describes the SCSI sub class specific definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to  you  the  right  to  use,  modify,  copy  and  distribute
Software only when embedded on a Microchip  microcontroller  or  digital  signal
controller  that  is  integrated  into  your  product  or  third  party  product
(pursuant to the  sublicense  terms  in  the  accompanying  license  agreement).

You should refer  to  the  license  agreement  accompanying  this  Software  for
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
// DOM-IGNORE-END

#ifndef _USB_HOST_HUB_LOCAL_H
#define _USB_HOST_HUB_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"

#define USB_HOST_HUB_PORT_REQUEST    0x00
#define USB_HOST_HUB_REQUEST         0x01

/* Hub or port status change information */
typedef struct _USB_HOST_HUB_STATUS_
{
    uint16_t       status;
    uint16_t       change;
} USB_HOST_HUB_STATUS;

// *****************************************************************************
/* USB Host Hub control request 

  Summary:
    Defines a defines types of control request that can be identify the 
    call back function
  Description:
 * 
  Remarks:
    None.
*/
typedef enum
{
    USB_HOST_HUB_PORT_CONNECTION       =    0 ,
    USB_HOST_HUB_PORT_ENABLE           =    1,
    USB_HOST_HUB_PORT_SUSPEND          =     2,
    USB_HOST_HUB_PORT_OVER_CURRENT     =     3,
    USB_HOST_HUB_PORT_RESET            =     4,
    USB_HOST_HUB_PORT_POWER            =     8,
    USB_HOST_HUB_PORT_LOW_SPEED        =     9,
    USB_HOST_HUB_PORT_HIGH_SPEED       =    10,
    USB_HOST_HUB_PORT_TEST             =    11,
    USB_HOST_HUB_PORT_INDICATOR        =    12,
    USB_HOST_HUB_GET_CONFIG_DESCRIPTOR =    13,
    USB_HOST_HUB_GET_HUB_DESCRIPTOR    =    14,
    USB_HOST_HUB_CLEAR_POET_RESET      =    15,
    USB_HOST_HUB_STATUS_REQUEST        =    16,
    USB_HOST_HUB_PORT_STATUS_REQUEST   =    17,
    USB_HOST_HUB_PORT_POWER_ENABLE_REQUEST = 18,
    USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_REQUEST = 19,
    USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_COMPLETE_REQUEST = 20,
    USB_HOST_HUB_CLEAR_PORT_RESET_STATE_REQUEST = 21,

}USB_HOST_HUB_CONTROL_REQUEST;

// *****************************************************************************
/* USB Host Hub instance state  

  Summary:
    Defines a defines hub instance state 
  Description:
 * 
  Remarks:
    None.
*/

typedef enum
{
    USB_HOST_HUB_GET_CONFIG_DESCRIPTOR_STATE                = 1,
    USB_HOST_HUB_GET_CONFIG_DESCRIPTOR_WAIT_STATE           = 2,
    USB_HOST_HUB_GET_CONFIG_DESCRIPTOR_STATE_COMPLETE       = 3,
    USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE                    = 4,
    USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE_WAIT               = 5,
    USB_HOST_HUB_GET_HUBDESCRIPTOR_STATE_COMPLETE           = 6,
    USB_HOST_HUB_UPDATE_PORT_STATE                          = 7 ,
    USB_HOST_HUB_STATUS_STATE                               = 8,
    USB_HOST_HUB_STATUS_STATE_WAIT                          = 9,
    USB_HOST_HUB_STATUS_STATE_COMPLETE                      = 10,
    USB_HOST_HUB_STATE_RUNNING                              =  100 ,
    USB_HOST_HUB_ERROR                                      = -1 

}USB_HOST_HUB_INSTANCE_STATE;

// *****************************************************************************
/* USB Host Hub Port state  

  Summary:
    Defines a defines port state 
  Description:
 
  Remarks:
    None.
*/

typedef enum
{
    USB_HOST_HUB_PORT_POWER_ENABLE_STATE                = 1,
    USB_HOST_HUB_PORT_POWER_ENABLE_STATE_WAIT           = 2,
    USB_HOST_HUB_PORT_POWER_ENABLE_STATE_COMPLETE       = 3,
    USB_HOST_HUB_PORT_STATUS_STATE                      = 4,
    USB_HOST_HUB_PORT_STATUS_STATE_WAIT                 = 5,
    USB_HOST_HUB_PORT_STATUS_STATE_COMPLETE             = 6,
    USB_HOST_HUB_PORT_RESET_STATE                       = 7 ,
    USB_HOST_HUB_PORT_RESET_STATE_WAIT                  = 8 ,
    USB_HOST_HUB_PORT_RESET_STATE_COMPLETE              = 9,
    USB_HOST_HUB_PORT_RESET_STATE_DELAY                 = 10,
    USB_HOST_HUB_PORT_RESET_STATE_DELAY_WAIT            = 16,
    USB_HOST_HUB_PORT_RESET_STATE_DELAY_WAIT_COMPLETE   = 17,
    USB_HOST_HUB_PORT_RESET_STATUS_STATE                = 11,
    USB_HOST_HUB_PORT_RESET_STATUS_STATE_COMPLETE       = 12,
    USB_HOST_HUB_PORT_SUSPEND_STATE                     = 13,
    USB_HOST_HUB_PORT_SUSPEND_STATE_WAIT                = 14,
    USB_HOST_HUB_PORT_SUSPEND_STATE_COMPLETE            = 15,
    USB_HOST_HUB_CLEAR_PORT_RESET_STATE_WAIT            = 19 ,
    USB_HOST_HUB_CLEAR_PORT_RESET_STATE_COMPLETE        = 18,
    USB_HOST_HUB_PORT_STATUS_CHANGED                    = 20 ,
    USB_HOST_HUB_PORT_READY_STATE                       = 100,
    USB_HOST_HUB_PORT_ERROR                             = -1

}USB_HOST_HUB_PORT_STATE;

// *****************************************************************************
/* USB Host Hub Port sub state  

  Summary:
    Defines a defines port sub state 
  Description:
 
  Remarks:
    None.
*/

typedef enum
{
    USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS              = 1 ,
    USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS_WAIT         = 2,
    USB_HOST_HUB_PORT_STATUS_CHANGED_GETSTATUS_COMPLETED    = 3,
    USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT            = 4 ,
    USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_WAIT       = 5,
    USB_HOST_HUB_PORT_STATUS_CHANGED_CLEAR_EVENT_COMPLETE   = 6 ,
    USB_HOST_HUB_PORT_STATUS_CHANGED_PORT_ENABLED           = 7,
    USB_HOST_HUB_PORT_STATUS_CHANGED_PORT_ENABLED_WAIT      = 8,
    USB_HOST_HUB_PORT_STATUS_CHANGED_PORT_ENABLED_COMPLETE  = 9,
    USB_HOST_HUB_PORT_ENUMERATION_WAIT                      = 10,
    USB_HOST_HUB_PORT_DEENUMERATION_WAIT                    = 11,
        
}USB_HOST_HUB_PORT_SUB_STATE;

// *****************************************************************************
/* USB Host Hub Descriptor

  Summary:
    
  Description:
 
  Remarks:
    None.
*/
/* Table 11-13 Hub descriptor */
typedef struct _USB_HOST_HUB_DESCRIPTOR_
{

    uint8_t               bDescLength;        /* Number of bytes in this descriptor */
    uint8_t               bDescriptorType;    /* value: 29H for hub descriptor  */
    uint8_t               bNbrPorts;          /* Number of downstream facing ports that this hub supports */
    uint16_t              wHubCharacteristics;    /*  description  in table 11-13 spec*/
    uint8_t               bPwrOn2PwrGood;     /* Time 2ms this    */
    uint8_t               bHubContrCurrent;   /* Max Current */
    uint8_t               DeviceRemovable;    /* Depends number of ports on hub */
    uint8_t               PortPwrCtrlMask;    
	/* TODO  size is depend PortPwrCtrlMask **********/
	/* reseved */
    
}USB_HOST_HUB_DESCRIPTOR;

// *****************************************************************************
/* USB Host Hub Transfer object  

  Summary:
    Defines a transfer information 
  Description:
 
  Remarks:
    None.
*/

typedef struct _USB_HOST_HUB_TRANSFER_OBJ_
{
    void                *hubInfo;
    uint8_t             index;
    uint8_t             transferNumber;        /* Easy to track */
    uint8_t             portStatus[USB_HOST_HUB_PORTS_NUMBER];
}USB_HOST_HUB_TRANSFER_OBJ;

// *****************************************************************************
/* USB Host Hub Port information

  Summary:
    Defines port information
  Description:
 
  Remarks:
    None.
*/


typedef struct _USB_HOST_HUB_PORT_INFO_
{
    USB_HOST_HUB_STATUS             portStatus;
    USB_SPEED                       speedOfDevice;
    USB_HOST_HUB_PORT_STATE         portState;
    USB_HOST_HUB_PORT_SUB_STATE     portSubState;
    USB_HOST_DEVICE_OBJ_HANDLE      deviceObjHandle;
    bool                            portPowered;
    bool                            portResetCompleted;
    bool                            portStatusChanged ;

}USB_HOST_HUB_PORT_INFO;

// *****************************************************************************
/* USB Host Hub driver instance information

  Summary:
    HUB information we have to maintain in array 
  Description:
 
  Remarks:
    None.
*/
typedef struct _USB_HOST_HUB_DRIVER_INFO_
{
    bool                inUse;
    uint8_t                  setupPacket[8];
    USB_HOST_DEVICE_INTERFACE_HANDLE hubInterfaceHandle;
    USB_HOST_DEVICE_OBJ_HANDLE   hubObjHandle;
    USB_HOST_CONTROL_PIPE_HANDLE  controlPipeHandle;
    USB_HOST_PIPE_HANDLE     interruptInPipeHandle;
    USB_HOST_HUB_DESCRIPTOR  hubDescriptor;
    uint8_t                  instanceNumber;
    uint8_t                  instanceState;
    USB_HOST_HUB_CONTROL_REQUEST  controlRequest;
    USB_HOST_HUB_INSTANCE_STATE   hubInstanceState;
    USB_HOST_DEVICE_CLIENT_HANDLE    deviceHandle;
    USB_CONFIGURATION_DESCRIPTOR    *configDescriptor;
    uint32_t                         power;
    uint8_t                       numberOfPorts;
    USB_HOST_HUB_PORT_INFO        portInfo[ USB_HOST_HUB_PORTS_NUMBER ];
    uint32_t                      parentPortNumber;
    uint8_t                       bInterfaceNumber;
    USB_HOST_HUB_STATUS           hubStatus;
    USB_HOST_HUB_TRANSFER_OBJ     hubTransferObj;
    uint32_t                      bufferStatus;
    SYS_TMR_HANDLE                 portResetTimerHandle;
    bool                          standardRequestDone;
    bool                          hubStatusChange;
    bool                          transferRequestReceived;
    
} USB_HOST_HUB_DRIVER_INFO;

/* Standard request */
#define   USB_HOST_HUB_SET_PORT_FEATURE     0x10
#define   USB_HOST_HUB_CLEAR_PORT_FEATURE   0x20
#define   USB_HOST_HUB_SET_HUB_FEATURE      0x30
#define   USB_HOST_HUB_CLEAR_HUB_FEATURE    0x40
#define   USB_HOST_GET_HUB_STATUS           0x50
#define   USB_HOST_GET_PORT_STATUS          0x60

// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_Initialize(void * hubInit)

  Summary:
    This function is called when the Host Layer is initializing.

  Description:
    This function is called when the Host Layer is initializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_HUB_Initialize ( void *hubInit);

// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_Deinitialize(void)

  Summary:
    This function is called when the Host Layer is deinitializing.

  Description:
    This function is called when the Host Layer is deinitializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_HUB_Deinitialize ( void );

// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_Reinitialize ( void  *hubInit );

  Summary:
    This function is called when the Host Layer is reinitializing.

  Description:
    This function is called when the Host Layer is reinitializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_HUB_Reinitialize ( void  *hubInit );

// *****************************************************************************
/* Function:
  void _USB_HOST_HUB_DeviceAssign ( USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
                                    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
                                    USB_DEVICE_DESCRIPTOR * deviceDescriptor );

  Summary:
    This function is called when the Host Layer attaches this driver to 
    device level driver.

  Description:
    This function is called when the Host Layer attaches this driver to 
    device level driver.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_HUB_DeviceAssign ( USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
                                  USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
                                   USB_DEVICE_DESCRIPTOR * deviceDescriptor );

// *****************************************************************************
/* Function:
  void _USB_HOST_HUB_DeviceRelease (USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle );

  Summary:
    This function is called when the Host Layer detaches this driver from 
    device unplugged or unsupported .

  Description:
    This function is called when the Host Layer detaches this driver from 
    device unplugged or unsupported .

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/


void _USB_HOST_HUB_DeviceRelease (USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle );

// *****************************************************************************
/* Function:
   void _USB_HOST_HUB_DeviceTasks (USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle);

  Summary:
    This function is called by the Host Layer to update the state of this
    driver.

  Description:
    This function is called by the Host Layer to update the state of this
    driver.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_HUB_DeviceTasks (USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle);


// *****************************************************************************
/* Function:
USB_HOST_DEVICE_EVENT_RESPONSE _USB_HOST_HUB_DeviceEventHandler (
                                    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
                                    USB_HOST_DEVICE_EVENT event,
                                    void * eventData,
                                    uintptr_t context );
  Summary:
    This function is called when the Host Layer generates device level
    events. 

  Description:
    This function is called when the Host Layer generates device level
    events. 

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_HOST_DEVICE_EVENT_RESPONSE _USB_HOST_HUB_DeviceEventHandler (
                                    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
                                    USB_HOST_DEVICE_EVENT event,
                                    void * eventData,
                                    uintptr_t context );

// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_InterfaceAssign (
        USB_HOST_DEVICE_INTERFACE_HANDLE * interfaces,
        USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
        size_t nInterfaces,
        uint8_t * descriptor );

  Summary:
    This function is called when the Host Layer attaches this driver to an
    interface.

  Description:
    This function is called when the Host Layer attaches this driver to an
    interface.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

  void _USB_HOST_HUB_InterfaceAssign (
        USB_HOST_DEVICE_INTERFACE_HANDLE * interfaces,
        USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
        size_t nInterfaces,
        uint8_t * descriptor );

// *****************************************************************************
/* Function:
    void _USB_HOST_HUB_InterfaceRelease 
   (
        USB_HOST_DEVICE_INTERFACE_HANDLE interface 
   )

  Summary:
    This function is called when the Host Layer detaches this driver from an
    interface.

  Description:
    This function is called when the Host Layer detaches this driver from an
    interface.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/
  void _USB_HOST_HUB_InterfaceRelease (
        USB_HOST_DEVICE_INTERFACE_HANDLE interface );
  
  // *****************************************************************************
/* Function:
 USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE _USB_HOST_HUB_InterfaceEventHandler
  (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle,
        USB_HOST_DEVICE_INTERFACE_EVENT event,
        void * eventData,
        uintptr_t context
  )

  Summary:
    This function is called when the Host Layer generates interface level
    events. 

  Description:
    This function is called when the Host Layer generates interface level
    events. 

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

  USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE _USB_HOST_HUB_InterfaceEventHandler
  (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle,
        USB_HOST_DEVICE_INTERFACE_EVENT event,
        void * eventData,
        uintptr_t context
  );

  // *****************************************************************************
/* Function:
  void _USB_HOST_HUB_InterfaceTasks (USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle)
 
  Summary:
    This function is called by the Host Layer to update the state of this
    driver.

  Description:
    This function is called by the Host Layer to update the state of this
    driver.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

  void _USB_HOST_HUB_InterfaceTasks (USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle);
  
  
  /* Local function */

#define  USB_HOST_HUB_SetPortFeature( x, y ,z)        USB_HOST_HUB_featureRequest ( x , y , z , USB_HOST_HUB_PORT_REQUEST , USB_HOST_COMMAND_SET_FEATURE )

#define  USB_HOST_HUB_SetHubFeature( x , y )          USB_HOST_HUB_featureRequest ( x , y ,  0 , USB_HOST_HUB_REQUEST , USB_HOST_COMMAND_SET_FEATURE )

#define  USB_HOST_HUB_ClearPortFeatre ( x , y, z )    USB_HOST_HUB_featureRequest ( x , y, z , USB_HOST_HUB_PORT_REQUEST , USB_HOST_COMMAND_CLEAR_FEATURE )

#define  USB_HOST_HUB_ClearHubFeature( x , y )        USB_HOST_HUB_featureRequest ( x , y ,  0 , USB_HOST_HUB_REQUEST , USB_HOST_COMMAND_CLEAR_FEATURE )

#define  USB_HOST_HUB_getPortStatus ( x , y, z )      USB_HOST_HUB_statusRequest ( x , y, z , USB_HOST_HUB_PORT_REQUEST  )

#define  USB_HOST_HUB_getHubStatus( x , y )           USB_HOST_HUB_statusRequest ( x , y ,  0 , USB_HOST_HUB_REQUEST )

  // *****************************************************************************
/* USB Host set feature 

  Summary:
    setFeature for any control request ..
  Description:
 
  Remarks:
    None.
*/
void  _usb_host_hub_setFeature 
(       USB_HOST_HUB_DRIVER_INFO * hubinfo, 
        uint8_t portNumber ,
        uint8_t Feature, 
        uint8_t hubCommand ,
        uint8_t requestType 
);
//*************************************************
/* USB_HOST_RESULT USB_HOST_HUB_statusRequest

  Summary:
 find the hub status 
  Description:
 
  Remarks:
    None.
*/
USB_HOST_RESULT USB_HOST_HUB_statusRequest 
(   USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo, 
    USB_HOST_HUB_STATUS  *hubStatus ,
    uint8_t portNumber ,
    uint8_t requestType 
);

//*************************************************
/* USB_HOST_HUB_DRIVER_INFO * _USB_HOST_findDriverInfo( uintptr_t deviceHandle );

  Summary:
    find the instance Driver information  
  Description:
 
  Remarks:
    None.
*/
USB_HOST_HUB_DRIVER_INFO * _USB_HOST_findDriverInfo( uintptr_t deviceHandle );

//*************************************************
/* USB_HOST_HUB_DRIVER_INFO * _USB_HOST_HUB_FindInstanceInfo
( 
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle 
);

  Summary:
    find the instance Information  
  Description:
 
  Remarks:
    None.
*/
USB_HOST_HUB_DRIVER_INFO * _USB_HOST_HUB_FindInstanceInfo
( 
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle 
);

//*************************************************
/* USB_HOST_RESULT  _USB_HOST_HUB_featureRequest 
(
        USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo,
        uint32_t portNumber ,
        uint8_t Feature,
        uint8_t hubCommand ,
        uint8_t requestType 
);

  Summary:
    Send feature request to the hub  
  Description:
 
  Remarks:
    None.
*/

USB_HOST_RESULT  _USB_HOST_HUB_featureRequest 
(
        USB_HOST_HUB_DRIVER_INFO *hubDriverInstnceInfo,
        uint32_t portNumber ,
        uint8_t Feature,
        uint8_t hubCommand ,
        uint8_t requestType 
);

//*************************************************
/* void _USB_HOST_UpdateHubState 
( 
    USB_HOST_HUB_DRIVER_INFO * hubDriverInstnceInfo 
);

  Summary:
 Update the Hub State 
  Description:
 
  Remarks:
    None.
*/
void _USB_HOST_UpdateHubState 
( 
    USB_HOST_HUB_DRIVER_INFO * hubDriverInstnceInfo 
);
 void  _USB_HOST_HUB_controlTransferComplete
  (
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    USB_HOST_REQUEST_HANDLE requestHandle,
    USB_HOST_RESULT result,
    uint32_t size,
    uintptr_t context
  );
 
 //*************************************************
/* USB_ERROR USB_HOST_HUB_PortReset  (uintptr_t deviceHandle, uint8_t port);

  Summary:
 Port reset ..Host layer will call this function .
  Description:
 
  Remarks:
    None.
*/
 USB_ERROR USB_HOST_HUB_PortReset  (uintptr_t deviceHandle, uint8_t port);
 
  //*************************************************
/*  USB_ERROR USB_HOST_HUB_PortSuspend(uintptr_t deviceHandle, uint8_t port);

  Summary:
  suspend the port  ..Host layer will call this function .
  Description:
 
  Remarks:
    None.
*/
 USB_ERROR USB_HOST_HUB_PortSuspend(uintptr_t deviceHandle, uint8_t port);
 
 //*************************************************
/*  USB_ERROR USB_HOST_HUB_PortResume(uintptr_t deviceHandle, uint8_t port);

  Summary:
  Resume the port  ..Host layer will call this function .
  Description:
 
  Remarks:
    None.
*/

 USB_ERROR USB_HOST_HUB_PortResume(uintptr_t deviceHandle, uint8_t port);
 
 //*************************************************
/*   USB_SPEED  USB_HOST_HUB_PortSpeedGet(uintptr_t deviceHandle, uint8_t port);

  Summary:
  Will find The connected device speed   ..Host layer will call this function .
  Description:
 
  Remarks:
    None.
*/
 USB_SPEED  USB_HOST_HUB_PortSpeedGet(uintptr_t deviceHandle, uint8_t port);
 
//*************************************************
/*    bool USB_HOST_HUB_PortResetComplete (uintptr_t deviceHandle, uint8_t port );

  Summary:
  Will find The Reset completed or not   ..Host layer will call this function .
  Description:
 
  Remarks:
    None.
*/
 
  bool USB_HOST_HUB_PortResetComplete (uintptr_t deviceHandle, uint8_t port );
  
  //*************************************************
/*     void _USB_HOST_HUB_PORT_UpdateState
  ( 
        USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo
  );

  Summary:
 Update the port status information ....
 
  Remarks:
    None.
*/
  void _USB_HOST_HUB_PORT_UpdateState
  ( 
        USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo
  );

 //*************************************************
/*    void _USB_HOST_HUB_TransferTasks
(
    USB_HOST_HUB_DRIVER_INFO * hubInstanceInfo,
    USB_HOST_RESULT result,
    size_t size
);

  Summary:
 Update the Transfers status .
 
  Remarks:
    None.
*/
void _USB_HOST_HUB_TransferTasks
(
    USB_HOST_HUB_DRIVER_INFO * hubInstanceInfo,
    USB_HOST_RESULT result,
    size_t size
);

 //*************************************************
/*   void _USB_HOST_HubRemove ( USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo );

  Summary:
 De allocate the hub and denumerate the connected device into the hub
 
  Remarks:
    None.
*/
void _USB_HOST_HubRemove ( USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo );

 //*************************************************
/*   void _USB_HOST_HUB_PortStatusChanged
( 
        USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo , 
        uint32_t  dataChanged 
);

  Summary:
  when the port status update will call this function to port status changed .
 
  Remarks:
    None.
*/
        
void _USB_HOST_HUB_PortStatusChanged
( 
        USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo , 
        uint32_t  dataChanged 
);

/* This is temporary function when the parent device information available will be removed */

  void _USB_HOST_HubRemoveParentIsRootHub ( USB_HOST_HUB_DRIVER_INFO *hubDriverInstanceInfo );

#endif

