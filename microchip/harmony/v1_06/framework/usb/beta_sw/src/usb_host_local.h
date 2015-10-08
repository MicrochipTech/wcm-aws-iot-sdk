/*******************************************************************************
  USB HOST Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_local.h

  Summary:
    USB HOST layer Local Data Structures

  Description:
    Host layer Local Data Structures
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute Software
only when embedded on a Microchip microcontroller or digital  signal  controller
that is integrated into your product or third party  product  (pursuant  to  the
sublicense terms in the accompanying license agreement).

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
//DOM-IGNORE-END

#ifndef _USB_HOST_LOCAL_H
#define _USB_HOST_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "usb/beta_sw/usb_common.h"
#include "usb/beta_sw/usb_chapter_9.h"
#include "usb/beta_sw/usb_host.h"
#include "driver/usb/beta_sw/drv_usb.h"
#include "system/tmr/sys_tmr.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

#define USB_HOST_PIPE_HANDLE_INVALID ((USB_HOST_PIPE_HANDLE)(-1))

/* Host controller ID + Device address */
typedef uint16_t USB_HOST_DEVICE_ID;

/* Enumeration retry count  */
#define  USB_HOST_ENUMERATION_RETRY_COUNT      3

/* Host Pipe handle and Host controller driver pipe handle are same */
typedef  DRV_USB_HOST_PIPE_HANDLE   USB_HOST_PIPE_HANDLE;

/* Call back should be function pointer */
typedef void *  USB_HOST_IRP_CALLBACK;

/* standard USB config descriptor size */
#define    USB_HOST_CONFIG_DESCRPTR_SIZE        9

/* This values based on USB standards It will change based on
   controller driver */
#define  USB_HOST_FULL_SPEED_BANDWIDTH   900
#define  USB_HOST_HIGH_SPEED_BANDWIDTH   800
#define  USB_HOST_ENDPOINT_NUMBER      DRV_USB_HOST_PIPES_NUMBER

/* By default RootHub address is always 1*/
#define     USB_HOST_ROOT_HUB            1

/* Dynamic allocation of memory */
#define      USB_HOST_MALLOC(size)       malloc(size)
/* Free memory */
#define      USB_HOST_FREE(ptr)          free(ptr)
// *****************************************************************************
/*  USB Host Class Driver Function Pointer Structure.

  Summary:
    USB Host Class Driver Function Pointer Structure.

  Description:
    This structure defines the group of function pointers that the class driver
    should provide to the Host Layer via the Host TPL table. It contains
    pointers to class driver functions that will be invoked by the host layer.

  Remarks:
    None.
*/

typedef struct _USB_HOST_CLASS_DRIVER
{
    /* Pointer to Class Driver Device Level Initialize function. */
    USB_ERROR (*initializeDeviceDriver) 
    (
        USB_HOST_DEVICE_ID hostId,
        uint8_t *instanceIndex ,
        USB_SPEED speed
    );

    /* Pointer to the Class Driver Interface Level Initialize function. */
    USB_ERROR (*initializeInterfaceDriver)
    (
        USB_HOST_DEVICE_ID hostId,
        uint8_t interfaceId,
        uint8_t * instanceIndex ,
        USB_SPEED speed
    );

    /* Pointer to the Class Driver Deinitialize function */
    void (*deInitializeDriver ) ( USB_HOST_DEVICE_ID id);

    /* Pointer to the Driver Task routine */
    void (* task ) (uint8_t instanceIndex);

} USB_HOST_CLASS_DRIVER;

// *****************************************************************************
/* USB Host Layer Query flags.

  Summary:
    USB HOST Layer Query flags
  
  Description:
    These flags should be used along with the USB Host Driver Query Structure.

  Remarks:
    None.
*/
typedef enum
{
    USB_HOST_MATCH_ENDPOINT_TYPE = 0x1,
    USB_HOST_MATCH_ENDPOINT_DIRECTION = 0x2,
    USB_HOST_MATCH_CLASS_SUBCLASS_PROTOCOL = 0x3

} USB_HOST_QUERY_FLAGS;

// *****************************************************************************
/*  USB Host Query Structure.

  Summary:
    Query object that can be submitted by class driver to obtain information
    from the configuration descriptor.

  Description:
    This object defines the structure of the query the can be submitted by class
    drivers to obtain information about the attached device.
  
  Remarks:
    None.
*/

typedef struct _USB_HOST_QUERY_
{
    /* Host controller id + Device Id */
    USB_HOST_DEVICE_ID id;

    /* Alternate setting number */
    uint8_t alternateSettingNumber;

    /* Interface Number */
    uint8_t interfaceNumber;

    /* Configuration Number */
    uint8_t configurationNumber;

    /* Endpoint Address */
    uint8_t endpointAddress;

    /* Endpoint Type bulk/interrupt/iso */
    uint8_t endpointType;
    
    /* Endpoint Direction in or out */
    uint8_t endpointDirection;

    /*class*/
    uint8_t class;

    uint8_t subClass;

    uint8_t protocol;
    
    /* Query flags to match */
    USB_HOST_QUERY_FLAGS flags;

} USB_HOST_QUERY;

// *****************************************************************************
/*  USB Host Endpoint Information

  Summary:
    USB Host Endpoint Information.

  Description:
    This data structure contains information about the endpoint of an attached
    device. It contains the pointer to the actual endpoint descriptor and other
    data that is needed by the host layer to manage device endpoints.

  Remarks:
    None.
*/
#if 0
typedef struct _USB_HOST_ENDPOINT_INFO_
{
    /* Standard USB Endpoint Descriptor for this endpoint*/
    USB_ENDPOINT_DESCRIPTOR * endpointDescriptor;
    
   /* Extra class specific descriptor information */
    uint8_t * classSpecificDescriptor;
    
    /* Length of the extra descriptors, in bytes.  */
    uint8_t * classSpecificDescriptorEndPtr;
    
    /* Pipe Handle for this endpoint */
    USB_HOST_PIPE_HANDLE pipeHandle;
    
} USB_HOST_ENDPOINT_INFO;

// *****************************************************************************
/*  USB Host Interface Alternate Setting Information 

  Summary:
    USB Host Interface Alternate Setting Information 

  Description:
    This USB Host Layer object contains information about an interface alternate
    setting.

  Remarks:
    None.
*/

typedef struct _USB_HOST_INTERFACE_ALTERNATE_SETTING_INFO_
{
    /* Standard USB Interface descriptor */
    USB_INTERFACE_DESCRIPTOR * interfaceDescriptor;

    /* Endpoint descriptor info */
    USB_HOST_ENDPOINT_INFO  endpointInfo[USB_HOST_ENDPOINT_NUMBER];

    /* Class specific descriptor information */
    uint8_t * classSpecificInterfaceDesc;
    
    /* Pointer to the end of class specific descriptor */
    uint8_t * classSpecificIntefaceDescEndPtr;

} USB_HOST_INTERFACE_ALTERNATE_SETTING_INFO;

#endif
// *****************************************************************************
/*  USB Host Interface Descriptor Information.

  Summary:
    USB Host Interface Descriptor Information.
 
  Description:
    This USB Host Layer object contains information about about the attached
    device interface. 

  Remarks:
    None.
*/

typedef struct _USB_HOST_INTERFACE_DESC_INFO_
{
    /* This is true if this interface is claimed */
    bool isClaimed;
    
    /* Standard USB Interface Descriptor */
    USB_INTERFACE_DESCRIPTOR * interfaceDesc;
    
    /* Alternate Setting Information */
  //  USB_HOST_INTERFACE_ALTERNATE_SETTING_INFO
  //                        alternateSettingInfo[USB_HOST_MAX_ALTERNATE_SETTINGS];
    
    /* Driver for this interface */
    USB_HOST_CLASS_DRIVER * interfaceDriver;

    /*instanceNumber*/
    uint8_t          instanceNumber;

} USB_HOST_INTERFACE_DESC_INFO;

// *****************************************************************************
/*  USB Host Device Configuration Information. 

  Summary:
    USB Host Device Configuration Information.

  Description:
    This USB Host Layer object contains information about the configuration of
    the attached device.

  Remarks:
    None.
*/

typedef struct _USB_HOST_CONFIGURATION_INFO_
{
    /* Standard USB Configuration Descriptor */
    USB_CONFIGURATION_DESCRIPTOR * configurationDescriptor;
    
    /* Local interface info */
    USB_HOST_INTERFACE_DESC_INFO interfaceInfo[USB_HOST_MAX_INTERFACES];
    
    /* Bandwidth for each configuration */
    uint8_t load;
    
    /* Configuration number */
    uint8_t configurationNumber ;

 } USB_HOST_CONFIGURATION_INFO;

// *****************************************************************************
/*  USB Host Device Information.

  Summary:
    USB Host Device Information.

  Description:
    This USB Host Layer object contains information about the attached device.

  Remarks:
    None.
*/

typedef struct  _USB_HOST_DEVICE_INFO_
{
    /* True if this object is allocated */
    bool inUse;

    /* Host controller id + Device Id */
    USB_HOST_DEVICE_ID deviceId;
    
    /* Parent device information for Hub support */
    USB_HOST_DEVICE_ID parentId;
    
        /* Default pipe handle */
    USB_HOST_PIPE_HANDLE defaultPipeHandle;
    
    /* Control pipe handle */
    USB_HOST_PIPE_HANDLE controlPipeHandle;
    
    /* Host controller handle */
    USB_HOST_HANDLE hostControllerHandle;
    
    /* Original device descriptor information */
    USB_DEVICE_DESCRIPTOR deviceDescriptor;
    
    /* Configurations information */
    USB_HOST_CONFIGURATION_INFO  configDescriptorInfo;
    
    /* Control irp*/
    USB_HOST_IRP controlIrp;
    
    /* Active configuration number */
    uint8_t activeConfigurationNumber;

    /* configuration number */

    uint8_t configurationNumber;
    
    /* Speed of the device */
    USB_SPEED speed;
    
    /* Port number for reset/suspend/resume/power */
    uint8_t portNumber;
    
    /* Vendor or Class specific device driver*/
    USB_HOST_CLASS_DRIVER * driver;

    /* instanceNumber */
    uint8_t  instanceNumber;
    
    /* Driver type */
    uint8_t driverType;

    /* Setup packet information */

    USB_SETUP_PACKET    setupPacket;
    
    
    /* Buffer for data information */
    uint8_t   buffer[10];
    /* Dynamically allocate rawBuffer */
    uint8_t   *rawBuffer;
    /* size of the config descriptor size */
    uint16_t    configDescSize;
    
    /* Device state information */
    uint32_t deviceState;
    
    /* Enumeration fail count*/
    uint8_t  enumerationFailCount;

}USB_HOST_DEVICE_INFO;

// *****************************************************************************
/*  USB Host Root Hub Information.

  Summary:
    USB Host Root Hub Information.

  Description:
    This object contains information about the root hub.

  Remarks:
    None.
*/

typedef struct _USB_HOST_ROOT_HUB_INFO_
{
    bool  alwaysPowered;

} USB_HOST_ROOT_HUB_INFO;


typedef enum
{
 USB_HOST_TASK_STATE_OPENING_USBHCD = 0x1 ,
 USB_HOST_TASK_STATE_RUNNING = 0x2

} USB_HS_OBJ_STATE;


// *****************************************************************************
/*  USB Host Layer Object.

  Summary:
    USB Host Layer Object.

  Description:
    This USB Host Layer Object contains information about the Host Layer
    Instance.

  Remarks:
    None.
*/

typedef struct  _USB_HOST_OBJ_
{
    /* Handle to the Host controller driver */
    DRV_HANDLE hostControllerHandle;

       /* Default pipe handle */
    USB_HOST_PIPE_HANDLE defaultPipeHandle;

    /* True if this object is valid */
    bool inUse;

    /* Available BandWidth per controller in a frame */
    uint32_t totalBandwidth;

    /* BandWidth available for new End points */
    uint32_t availableBandwidth;

    /* Data base for attached devicesInfor allocate from the pool */
    // For future release: This should be a pool.
    USB_HOST_DEVICE_INFO attachedDevicesList[USB_HOST_MAX_DEVICES_NUMBER];
    
    /* No of devices attached */
    uint8_t attachDevicesCount;
    
    /* Based on bits position the device address assigned and free*/
    uint8_t addressBits[(USB_HOST_MAX_DEVICES_NUMBER/8) + 1];
    
    /* Root hub information */
    USB_HOST_ROOT_HUB_INFO usbRootHubInfo;

    /* State of this instance */
    SYS_STATUS usbHostInstanceState;

    /*Enumeration pending */
    bool    enumerationPending ;

    /*EnumerationDeviceInfo*/

    USB_HOST_DEVICE_INFO   *enumerateDeviceInfo;

    /* Number of entries in the TPL table */
    int               nTPLEntries;

    /*TPL list*/

    USB_HOST_TARGET_PERIPHERAL_LIST *  usbHostTpl;

    /*state */

    USB_HS_OBJ_STATE        state;

    SYS_MODULE_OBJ          usbHCDSystemModuleObject;

    SYS_TMR_HANDLE          sysTmrHandle;

    /* Make sure only one control request outstanding */
    bool                    controlRequest;

    uintptr_t               context;

    USB_HOST_HANDLE         appHandle;

} USB_HOST_OBJ;



// *****************************************************************************
/*  USB Host Client Object

  Summary:
    USB Host Client Object

  Description:
    This USB Host Layer data structure contains information about a Host Layer
    Client.

  Remarks:
    None.
*/

typedef struct _USB_HOST_INSTANCE_CLIENT_
{
    /* Client status */
    DRV_CLIENT_STATUS clientState;

    /* Pointer to the callback function */
    USB_HOST_EVENT_CALLBACK callBackFunc;

    /* Bus information*/
    USB_HOST_OBJ * usbHostInstance;
    
    /* Context index or pointer */
    uintptr_t context;

    /* Indicates if this object is in use. */
    bool  inUse;

} * USB_HOST_INSTANCE_CLIENT_HANDLE, USB_HOST_INSTANCE_CLIENT;

// *****************************************************************************
/*  USB Host Layer Device State Constants

  Summary:
    USB Host Layer Device State Mask Constants.

  Description:
    These are definitions of USB Host Device State Masks constants

  Remarks:
    None.
*/

#define USB_HOST_DEVICE_STATE_MASK                             0xF000
#define USB_HOST_DEVICE_SUBSTATE_MASK                          0x0F00
#define USB_HOST_DEVICE_SUBSUBSTATE_MASK                       0x00FF

// *****************************************************************************
/*  USB Host Layer Device State Enumeration

  Summary:
    USB Host Layer Device State Enumeration.

  Description:
    These are the possible states of the device attached to the host.

  Remarks:
    None.
*/

typedef enum
{
    USB_HOST_DEVICE_STATE_DETACHED  = 0x1000,
    USB_HOST_DEVICE_STATE_ATTACHED  = 0x2000,
    USB_HOST_DEVICE_STATE_ADDRESSING = 0x3000,
    USB_HOST_DEVICE_STATE_CONFIGURING = 0x4000,
    USB_HOST_DEVICE_STATE_DEVICE_READY = 0x5000

} USB_HOST_DEVICE_STATE;

// *****************************************************************************
/*  USB Host Class driver type

  Summary:
    USB Host Class driver type.

  Description:
    Device and driver type .

  Remarks:
    None.
*/
typedef enum
{
    USB_HOST_VENDOR_DEVICE_DRIVER = 0x00,
    USB_HOST_STANDARD_CLASS_DEVICE_DRIVER = 1,
    USB_HOST_STANDARD_INTERFACE_DRIVER = 2


}USB_HOST_DRIVER_TYPE;
// *****************************************************************************
/*  USB Host Layer Device Sub State Enumeration

  Summary:
    USB Host Layer Device Sub State Enumeration.

  Description:
    These are the possible Sub states of the device attached to the host.

  Remarks:
    None.
*/

typedef enum 
{
    USB_HOST_DEVICE_SUBSTATE_INITIALIZE                = 0x0100,
    USB_HOST_DEVICE_SUBSTATE_WAIT_FOR_POWER            = 0x0200,
    USB_HOST_DEVICE_SUBSTATE_TURN_ON_POWER             = 0x0300,
    USB_HOST_DEVICE_SUBSTATE_WAIT_FOR_DEVICE           = 0x0400,
    USB_HOST_DEVICE_SUBSTATE_SETTLE                    = 0x0500,
    USB_HOST_DEVICE_SUBSTATE_RESET_DEVICE              = 0x0600,
    USB_HOST_DEVICE_SUBSTATE_GET_DEVICE_DESCRIPTOR_SIZE = 0x0700,
    USB_HOST_DEVICE_SUBSTATE_GET_DEVICE_DESCRIPTOR      = 0x0800,
    USB_HOST_DEVICE_SUBSTATE_SET_DEVICE_ADDRESS         = 0x0900,
    USB_HOST_DEVICE_SUBSTATE_INIT_CONFIGURATION         = 0x0a00,
    USB_HOST_DEVICE_SUBSTATE_GET_CONFIG_DESCRIPTOR      = 0x0b00,
    USB_HOST_DEVICE_SUBSTATE_SELECT_CONFIGURATION       = 0x0c00,
    USB_HOST_DEVICE_SUBSTATE_SET_CONFIGURATION          = 0x0d00,
    USB_HOST_DEVICE_SUBSTATE_NORMAL_RUN                 = 0x0e00,
    USB_HOST_DEVICE_SUBSTATE_SUSPEND_AND_RESUME         = 0x0f00

} USB_HOST_DEVICE_SUBSTATE;

// *****************************************************************************
/*  USB Host Layer Device Sub Substate Enumeration

  Summary:
    USB Host Layer Device Sub Substate Enumeration.

  Description:
    These are the possible Sub Substates of the device attached to the host.

  Remarks:
    None.
*/

typedef enum
{
    USB_HOST_DEVICE_SUB_SUBSTATE_START_SETTLING_DELAY              = 0x0000,
    USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_SETTLING                 = 0x0001,
    USB_HOST_DEVICE_SUB_SUBSTATE_SETTLING_DONE                     = 0x0002,
    USB_HOST_DEVICE_SUB_SUBSTATE_SET_RESET                         = 0x0003,
    USB_HOST_DEVICE_SUB_SUBSTATE_RESET_WAIT                        = 0x0004,
    USB_HOST_DEVICE_SUB_SUBSTATE_RESET_RECOVERY                    = 0x0005,
    USB_HOST_DEVICE_SUB_SUBSTATE_RECOVERY_WAIT                     = 0x0006,
    USB_HOST_DEVICE_SUB_SUBSTATE_RESET_COMPLETE                    = 0x0007,
    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_DEVICE_DESCRIPTOR_SIZE   = 0x0008,
    USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_GET_DEVICE_DESCRIPTOR_SIZE   = 0x0009,
    USB_HOST_DEVICE_SUB_SUBSTATE_GET_DEVICE_DESCRIPTOR_SIZE_COMPLETE   = 0x000a,
    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_DEVICE_DESCRIPTOR            = 0x000b,
    USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_GET_DEVICE_DESCRIPTOR        = 0x000c,
    USB_HOST_DEVICE_SUB_SUBSTATE_GET_DEVICE_DESCRIPTOR_COMPLETE        = 0x000d,
    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_SET_DEVICE_ADDRESS               = 0x000e,
    USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_SET_DEVICE_ADDRESS            = 0x000f,
    USB_HOST_DEVICE_SUB_SUBSTATE_SET_DEVICE_ADDRESS_COMPLETE           = 0x0010,
    USB_HOST_DEVICE_SUB_SUBSTATE_GET_CONFIG_DESCRIPTOR_SIZE            = 0x0011,
    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_CONFIG_DESCRIPTOR_SIZE       = 0x0012,
    USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_GET_CONFIG_DESCRIPTOR_SIZE   = 0x0013,
    USB_HOST_DEVICE_SUB_SUBSTATE_GET_CONFIG_DESCRIPTOR_SIZE_COMPLETE    = 0x0014,
    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_GET_CONFIG_DESCRIPTOR            = 0x0015,
    USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_GET_CONFIG_DESCRIPTOR        = 0x0016,
    USB_HOST_DEVICE_SUB_SUBSTATE_GET_CONFIG_DESCRIPTOR_COMPLETE        = 0x0017,
    USB_HOST_DEVICE_SUB_SUBSTATE_SEND_SET_CONFIGURATION                = 0x0018,
    USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_FOR_SET_CONFIGURATION            = 0x0019,
    USB_HOST_DEVICE_SUB_SUBSTATE_SET_CONFIGURATION_COMPLETE            = 0x001a,
    USB_HOST_DEVICE_SUB_SUBSTATE_INIT_CLIENT_DRIVERS                   = 0x001b,
    USB_HOST_DEVICE_SUB_SUBSTATE_SUSPEND                               = 0x001c,
    USB_HOST_DEVICE_SUB_SUBSTATE_RESUME                                = 0x001d,
    USB_HOST_DEVICE_SUB_SUBSTATE_RESUME_WAIT                           = 0x001e,
    USB_HOST_DEVICE_SUB_SUBSTATE_RESUME_RECOVERY                       = 0x001f,
    USB_HOST_DEVICE_SUB_SUBSTATE_RESUME_RECOVERY_WAIT                  = 0x0020,
    USB_HOST_DEVICE_SUB_SUBSTATE_RESUME_COMPLETE                       = 0x0021,
    USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_CONFIG_DESCRIPTOR_SELECT         = 0x0022,
    USB_HOST_DEVICE_SUB_SUBSTATE_WAIT_INTERFACE_DRIVER_SELECT          = 0x0023

} USB_HOST_DEVICE_SUB_SUBSTATE;


/***********************************************************
 * Local functions
 ***********************************************************/
// *****************************************************************************
/* Function:
    void _USB_HOST_EventHandler(void* referenceHandle,
        DRV_USB_EVENT eventType, DRV_USB_EVENT_DATA *eventData )

  Summary:
    Handles the events originating from USB Host Controller Driver

  Description:
    This routine is registered into the USB Host controller driver as a callback
    function. The USB Host controller driver calls this function in case of
    events from the USB controller driver.

  Parameters:
    referenceHandle           - USB Host Instance
    eventType                 - Event type
    eventData                 - Data associated with the event

  Returns:
    none.
*/
void _USB_HOST_EventHandler
        (
            uintptr_t referenceHandle,
            DRV_USB_EVENT eventType,
            void * eventData );


/*****************************************************************************/
/* Function:
    USB_ERROR _USB_HOST_IRPSubmit (USB_HOST_PIPE_HANDLE pipeHandle ,
                                     USB_HOST_IRP *irp )
  Summary:
         submit the IRP packet to the Controller driver

  Description:
      Send request to the Host controller driver

  Parameters:
    USB_HOST_IRP      - i/o request packet from class drivers

  Returns:
    USB_ERROR
*/

USB_ERROR _USB_HOST_IRPSubmit ( USB_HOST_PIPE_HANDLE pipeHandle ,
                                USB_HOST_IRP *irp   );

/*************************************************************************/
/* Function:
    void _USB_HOST_EnumerateDevice( USB_HOST_OBJ *hcObj )

  Summary:
      Enumerates newly connected device.

  Description:
    Host state machine for enumeration

  Parameters:
   USB_HOST_OBJ             -  pointer to the host object

  Returns:
    None.
*/
void _USB_HOST_UpdateDeviceState( USB_HOST_OBJ *hcObj );

/*************************************************************************/
/* Function:
    USB_ERROR _USB_HOST_DeEnumerate_Device ( USB_HOST_DEVICE_ID  deviceId )

  Summary:
    DeEnumerates disconnected device

  Description:
    This function de-enumerate newly disconnected device. All resources
    allocated for the device must be removed.

  Parameters:
   USB_HOST_DEVICE_ID             -  device id

  Returns:
    None.
*/
USB_ERROR _USB_HOST_DeEnumerate_Device ( USB_HOST_DEVICE_ID  deviceId );


// *****************************************************************************
/* Function:
   USB_HOST_PIPE_HANDLE  _USB_HOST_PipeSetup
                                (
                                    USB_HOST_DEVICE_ID id ,
                                    USB_ENDPOINT_DESCRIPTOR *endPointDesc,
                                    USB_SPEED speed )

   Summary:
         Create pipe for transfer the data

   Description:
        Function controller can setup the pipe for transfer data

  Parameters:
    USB_HOST_DEVICE_ID                id           // Hc id + Device id
    USB_ENDPOINT_DESCRIPTOR          *endPointDesc // Endpoint desc. information
    USB_SPEED                         speed        // Speed of the device

  Returns:
    USB_HOST_PIPE_HANDLE
*/

USB_HOST_PIPE_HANDLE  _USB_HOST_PipeSetup
                                (
                                    USB_HOST_DEVICE_ID id ,
                                    USB_ENDPOINT_DESCRIPTOR *endPointDesc,
                                    USB_SPEED speed );

// *****************************************************************************
/* Function:
  USB_HOST_DEVICE_INFO *  _USB_HOST_GetDeviceInfo(USB_HOST_DEVICE_ID Id )

  Summary:
   Get device information based on host controller id and device id

  Description:
  Get all the device information

  Parameters:
    USB_HOST_DEVICE_ID             - Host controllerId + device ID

  Returns:
    USB_HOST_DEVICE_INFO *         - pointer to the device information
*/

USB_HOST_DEVICE_INFO*  _USB_HOST_GetDeviceInfo
                                        (
                                            USB_HOST_DEVICE_ID id );

//*****************************************************************************
/* Function:
  uint8_t _USB_HOST_GetNewAddress(USB_HOST_DEVICE_ID id )

  Summary:
    Allocate a unique 8 bit device address

  Description:
    USB Host can allocate up to 127 address as per standard. This function will
    look into the free address and return the free address.

  Parameters:
   USB_HOST_DEVICE_ID             - Host controllerId + device ID

  Returns:
  uint8_t
*/

uint8_t _USB_HOST_GetNewAddress
                        (
                            USB_HOST_DEVICE_ID id );

//*****************************************************************************
/* Function:
  void _USB_HOST_setupPacketCallback ( USB_HOST_IRP * irp )

  Summary:
    _USB_HOST_setupPacketCallback for status of setup IRP.

  Description:
    USB Host controller driver will call when the setup request has been done.


  Parameters:
   USB_HOST_IRP *             - Pointer to the setupIrp

  Returns:
    void
*/

void _USB_HOST_setupPacketCallback ( USB_HOST_IRP * irp );

//*****************************************************************************
/* Function:
    void _USB_HOST_FreeAddress ( USB_HOST_DEVICE_ID id );

  Summary:
    Free address

  Description:
    This function is used for free the address to the Host controller database.

  Parameters:
   USB_HOST_DEVICE_ID             - Host controllerId + device ID

  Returns:
    void
*/

void _USB_HOST_FreeAddress ( USB_HOST_DEVICE_ID id );

//*****************************************************************************
/* Function:
    USB_ERROR _USB_HOST_ParseConfigDescriptor
                        (
                            uint8_t *buffer,
                            USB_HOST_CONFIGURATION_INFO *configDescriptorInfo,
                            uint8_t length  );
  Summary:
    Parse raw descriptors.

  Description:
    This routine is parse the configuration descriptor that holds the
    configuration descriptor, interface descriptor and endpoint descriptors

  Parameters:
    uint8_t*                         - raw configuration descriptor
    USB_HOST_CONFIGURATION_INFO *    - configuration information
    uint8_t                          - Length of the configuration descriptor

  Returns:
    USB_ERROR                        - Status
*/

USB_ERROR _USB_HOST_ParseConfigDescriptor
                        (
                            uint8_t *buffer,
                            USB_HOST_CONFIGURATION_INFO *configDescriptorInfo,
                            uint16_t length  );

//*****************************************************************************
/* Function:
    int8_t  _USB_HOST_FindVendorDriver (  USB_HOST_DEVICE_INFO *newdevice )

  Summary:
    Find a matching driver from the target peripheral list based on flag.

  Description:
    Driver has to find based on Vendorid , productid, version number in
    device descriptor.

  Parameters:
    USB_HOST_DEVICE_INFO *                 - new device information
	
  Returns:
    int8_t                                 - return status
*/

bool  _USB_HOST_FindVendorDriver (  USB_HOST_DEVICE_INFO *newdevice );

//*****************************************************************************
/* Function:
    int8_t  _USB_HOST_FindClassDriver ( USB_HOST_DEVICE_INFO *newdevice  )

  Summary:
    Find a matching a class driver from the target peripheral list based on flag.

  Description:
    Driver has to find based on Class , Subclass , Protocol number in
    device descriptor.

  Parameters:
    USB_HOST_DEVICE_INFO *                 - new device information
  Returns:
    int8_t                                 - return status
*/

bool  _USB_HOST_FindClassDriver ( USB_HOST_DEVICE_INFO *newdevice  );

//*****************************************************************************
/* Function:
  int8_t  _USB_HOST_FindInterfaceDriver ( USB_HOST_DEVICE_INFO *newdevice )
 
  Summary:
    Find a matching a class driver from the target peripheral list based on flag.

  Description:
    Driver has to find based on Class , Subclass , Protocol number in
    interface descriptors

  Parameters:
    USB_HOST_DEVICE_INFO *                 - new device information
	
  Returns:
    int8_t                                 - return status
*/
bool  _USB_HOST_FindInterfaceDriver ( USB_HOST_DEVICE_INFO *newdevice );

// *****************************************************************************
/* Function:
  USB_ERROR _USB_HOST_IRPCancel ( USB_HOST_IRP *irp   )
  Summary:
      Cancel IRPs

  Description:
      API provides cancel the IRPs related to this pipe.

  Parameters:
    USB_HOST_IRP              *irp

  Returns:
    USB_ERROR                   status
 */
USB_ERROR _USB_HOST_IRPCancel ( USB_HOST_IRP *irp   );

// *****************************************************************************
/* Function:
  USB_ERROR  _USB_HOST_PipeClose(USB_HOST_PIPE_HANDLE pipeHandle)

  Summary:
     Close the pipe

  Description:
     After detach the device we have close all the pipes related to the device.

  Parameters:
    USB_HOST_PIPE_HANDLE          PipeHandle          //Pipe handle

  Returns:
    USB_HOST_STATUS
*/

 USB_ERROR _USB_HOST_PipeClose( USB_HOST_PIPE_HANDLE pipeHandle );

//*****************************************************************************
/* Function:
    USB_ERROR _USB_HOST_SetInterface
                        (
                            USB_HOST_DEVICE_ID deviceId,
                            uint8_t interfaceNumber,
                            uint8_t alrernateSettingNumber,
                            void* callback )
  Summary:
    Set Interface

  Description:
    Function drivers can set the interface based on alternate setting.

  Parameters:
    USB_HOST_DEVICE_ID        - deviceId // HC id + Device id
    uint8_t                   - interfaceNumber
    uint8_t                   - alrernateSettingNumber

  Returns:
    USB_ERROR                 - status
*/

USB_ERROR _USB_HOST_SetInterface
                        (
                            USB_HOST_DEVICE_ID deviceId,
                            uint8_t interfaceNumber,
                            uint8_t alrernateSettingNumber,
                            void* callback );

//*****************************************************************************
/*  Function:
    USB_ERROR   _USB_HOST_EndpointHalt(
                                    USB_HOST_DEVICE_ID deviceId ,
                                    uint8_t endpointNumber ,
                                    void* callback );
  Summary:
        setup packet with set feature of halt endpoint
  Description:
    Use to prevent the device from being de-enumerated.

  Parameters:
       USB_HOST_DEVICE_ID      - deviceId
       uint8_t                 - endpointNumber
       void*                   - callback
	   
  Returns:
        USB_ERROR              - status
 */

USB_ERROR   _USB_HOST_EndpointHalt(
                                    USB_HOST_DEVICE_ID deviceId ,
                                    uint8_t endpointNumber ,
                                    void* callback );

//*****************************************************************************
/* Function:
    USB_ERROR   _USB_HOST_ClearEndpointHalt (
                                        USB_HOST_DEVICE_ID deviceId ,
                                        uint8_t endpointNumber,
                                        void* callback )
  Summary:
    Set up packet with set feature of clear halt endpoint.

  Description:
    This function prevents the device from being de-enumerated and
    clear the halt

  Parameters:
         USB_HOST_DEVICE_ID   - deviceId
         uint8_t              - endpointNumber
          void*               - callback
		  
   Returns:
        USB_ERROR             - status

 */
USB_ERROR   _USB_HOST_ClearEndpointHalt (
                                        USB_HOST_DEVICE_ID deviceId ,
                                        uint8_t endpointNumber,
                                        void* callback );

// ***************************************************************************
/* Function:
  USB_ERROR   _USB_HOST_IsEndpointIsHalted (
                                            USB_HOST_DEVICE_ID deviceId ,
                                            uint8_t endpointNumber ,
                                            uint16_t *data ,
                                            void* callback )
  Summary:
    Set up packet with find status of endpoint.

  Description:
    This function is used to find the status of an endpoint.

  Parameters:
         USB_HOST_DEVICE_ID   - deviceId
         uint8_t              - endpointNumber
         uint16_t *           - data
         void*                - callback
		 
  Returns:
        USB_ERROR             - status
 */
USB_ERROR   _USB_HOST_IsEndpointIsHalted (
                                            USB_HOST_DEVICE_ID deviceId ,
                                            uint8_t endpointNumber ,
                                            uint16_t *data ,
                                            void* callback );

// ***************************************************************************
/* Function:
    void _USB_HOST_ClaimInterface (
                                USB_HOST_DEVICE_ID id ,
                                uint8_t configurationNumber ,
                                uint8_t interfaceNumber )
  Summary:
    USB Host vendor drivers can claim the interface.

  Description:
    This function use for inform the host that interface is belongs
    to the vendor driver

  Parameters:
         USB_HOST_DEVICE_ID   - deviceId
         uint8_t              - configurationNumber
         uint8_t              - interfaceNumber
		 
  Returns:
     None.
 */
void _USB_HOST_ClaimInterface (
                                USB_HOST_DEVICE_ID id ,
                                uint8_t configurationNumber ,
                                uint8_t interfaceNumber );

// ***************************************************************************
/* Function:
   void _USB_HOST_SelectConfiguration (
                                    USB_HOST_DEVICE_ID id ,
                                    uint8_t configurationNumber )
  Summary:
    USB Host vendor drivers can select the configuration.

  Description:
    This function use for Host vendor drivers after the validation of supporting
    device then select the configuration.

  Parameters:
         USB_HOST_DEVICE_ID   - deviceId
         uint8_t              - configurationNumber
         uint8_t              - interfaceNumber
		 
  Returns:
     None.
 */

void _USB_HOST_SelectConfiguration (
                                    USB_HOST_DEVICE_ID id ,
                                    uint8_t configurationNumber );

/*****************************************************************************/
/*  Function:
   USB_ERROR  _USB_HOST_FindEndpoint (
                                    USB_HOST_QUERY *query,
                                    USB_ENDPOINT_DESCRIPTOR **endPointDesc )

  Summary:
    Find endpoint descriptor

  Description:
   USB Host driver can query the host and get the endpoint descriptor information

  Parameters:
    USB_HOST_DRIVER_QUERY *         - query,
    USB_ENDPOINT_DESCRIPTOR *      - *endPointDesc
  Returns:
    USB_ERROR            - status
*/
USB_ERROR  _USB_HOST_FindEndpoint (
                                    USB_HOST_QUERY *query,
                                    USB_ENDPOINT_DESCRIPTOR **endPointDesc );

 // *****************************************************************************
/* Function:
     USB_ERROR  _USB_HOST_FindInterface ( USB_HOST_QUERY *query,
                                      USB_INTERFACE_DESCRIPTOR **interfaceDesc )

  Summary:
    Find interface descriptor

  Description:
    USB Host driver can query the host and get the interface descriptor

  Parameters:
    USB_HOST_DRIVER_QUERY               *query,
    USB_INTERFACE_DESCRIPTOR           **interfaceDesc


  Returns:
    USB_ERROR
*/
 USB_ERROR  _USB_HOST_FindInterface ( USB_HOST_QUERY *query,
                                      USB_INTERFACE_DESCRIPTOR **interfaceDesc );

  // *****************************************************************************
/* Function:
    USB_ERROR USB_HOST_FindClassInterface ( USB_HOST_QUERY *query,
                                      void **classInterfaceDesc )

  Summary:
    Find class specific interface descriptor

  Description:
    USB Host driver can query the host and get the class specific interface
    descriptor

  Parameters:
    USB_HOST_DRIVER_QUERY               *query,
    void *                              *classInterfaceDesc

  Returns:
    USB_ERROR
*/


 USB_ERROR USB_HOST_FindClassInterface ( USB_HOST_QUERY *query,
                                      void **classInterfaceDesc );

 /*****************************************************************************/
/*  Function:
   USB_ERROR USB_HOST_FindClassSpecificEndpoint ( USB_HOST_QUERY *query,
                                        void **ClassSpecificEndpointDesc )
  Summary:
    Find class specific endpoint descriptor

  Description:
   USB Host driver can query the host and get the class specific endpoint
   descriptors information

  Parameters:
    USB_HOST_DRIVER_QUERY *         - query,
    void *                          - *ClassSpecificEndpointDesc
  Returns:
    USB_ERROR                       - status
*/

 USB_ERROR USB_HOST_FindClassSpecificEndpoint ( USB_HOST_QUERY *query,
                                        void **ClassSpecificEndpointDesc );

 uint32_t _USB_HOST_CheckBandwidth ( uint8_t direction,
                                     uint8_t endpointType,
                                     uint8_t  speed,
                                     uint32_t max_pkt_size);
 bool  _USB_HOST_AttachInterfaceDriver ( USB_HOST_DEVICE_INFO *newDevice );

 USB_HOST_INSTANCE_CLIENT * _USB_HOST_ClientHandleValidate(USB_HOST_HANDLE handle);

 void  _USB_HOST_FillSetupPacket
(
    USB_SETUP_PACKET *setupPacket ,
    uint8_t requestType,
    uint8_t request ,
    uint16_t value,
    uint16_t index,
    uint16_t length
);
#endif


/**************************   EOF   *****************************************/
