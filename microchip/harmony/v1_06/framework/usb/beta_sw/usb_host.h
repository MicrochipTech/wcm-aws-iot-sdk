/* ******************************************************************************
  USB Host Layer Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host.h

  Summary:
    USB HOST Layer Interface Header

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the USB HOST
    layer.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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
#ifndef _USB_HOST_H_
#define _USB_HOST_H_

//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include "system_config.h"
#include "system/common/sys_module.h"
#include "system/int/sys_int.h"
#include "driver/driver_common.h"
#include "usb/beta_sw/usb_common.h"


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Driver USB Host Module Index

  Summary:
    USB Host module index definitions

  Description:
    These constants provide USB Host index definitions.

  Remarks:
    These constants should be used in place of hard-coded numeric literals.
    These values should be passed into the USB_HOST_Initialize and
    USB_HOST_Open functions to identify the host layer instance in use.
*/

#define USB_HOST_INDEX_0      0
#define USB_HOST_INDEX_1      1

// *****************************************************************************
/* USB Host Client Handle

  Summary:
    Handle identifying a client when calling Host layer client functions.

  Description:
    A USB Host Client Handle is returned by a call to the USB_HOST_Open
    function.  The handle maintains the relationship between USB Host Layer and
    its client. All USB Host Client functions required the client handle to be
    specified when the functions are called. A client handle is valid till the
    the client calls the USB_HOST_Close function at which point it becomes
    invalid.

  Remarks:
    None.
*/

typedef uintptr_t USB_HOST_HANDLE;

// *****************************************************************************
/* USB Host Invalid Client Handle

  Summary:
    Definition of an invalid USB Host Client Handle

  Description:
    This is the definition of an invalid USB Host Client Handle. An invalid
    client handle is returned by the USB_HOST_Open function when it was not
    able to open the USB Host Layer successfully.

  Remarks:
    None.
*/

#define USB_HOST_HANDLE_INVALID /*DOM-IGNORE-BEGIN*/((USB_HOST_HANDLE)(-1))/*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Event Handler Response Type

  Summary:
    USB Host Event Handler Response Type

  Description:
    This is the return type of the USB Host Layer Event Handler

  Remarks:
    None.
*/

typedef enum 
{
    /* The event does not require any action */ 
    USB_HOST_EVENT_RESPONSE_NONE = 0,

    /* The event action is acceptable to the application */
    USB_HOST_EVENT_RESPONSE_OK = 1,
    
    /* The event action is not acceptable to the application */
    USB_HOST_EVENT_RESPONSE_ERROR = -1

} USB_HOST_EVENT_RESPONSE;

// *****************************************************************************
/* USB Host Layer TPL Table Entry Flags

  Summary:
    USB Host Layer Target Peripheral List Entry flags

  Description:
    This type defines the possible flags that can be used when adding an entry
    into the TPL table. Using the TPL_FLAG_VID_PID will direct the host layer to
    match the class driver to the attached device based on the device Vendor ID
    and the Product ID. Using the TPL_FLAG_CLASS_SUBCLASS_PROTOCOL will direct
    the host layer to match the class driver to the attached device based on the
    device class, subclass and protocol fields. 

  Remarks:
    None.
*/

typedef enum
{
    /* Match by VID and PID */
    TPL_FLAG_VID_PID = 0x1,

    /* Match by Class, Subclass and Protocol */
    TPL_FLAG_CLASS_SUBCLASS_PROTOCOL = 0x2,

    /* Ignore Class, only consider subclass and protocol */
    TPL_FLAG_IGNORE_CLASS = 0x4,

    /* Ignore Subclass, consider only class and protocol */
    TPL_FLAG_IGNORE_SUBCLASS = 0x10,

    /* Ignore protocol, consider only class and subclass */
    TPL_FLAG_IGNORE_PROTOCOL = 0x20,

    /* Ignore PID, only consider VID */
    TPL_FLAG_IGNORE_PID = 0x40

} USB_HOST_TPL_FLAGS;

// *****************************************************************************
/* USB Host Endpoint Table Size

  Summary:
    USB Host Endpoint Table size needed while running the host on PIC32MX 
    devices.

  Description:
    This macro defines the USB Endpoint Table size while running the host layer
    on PIC32MX devices. The application should allocate a uint8_t type array
    of this size. The array should be aligned at a 512 byte boundary.

  Remarks:
    None.
*/

#define USB_HOST_ENDPOINT_TABLE_SIZE /*DOM-IGNORE-BEGIN*/32/*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Layer Target Peripheral List Entry Type

  Summary:
    Defines the structure of an entry into the USB Host Layer Target Peripheral
    List Table.

  Description:
    This data type defines the structure of an entry into the USB Host Layer
    Target Peripheral List (TPL) Table. The TPL table provisions the class
    driver support for the Host Layer and thus identifies the ranges of USB
    devices to be supported by the host layer. 

  Remarks:
    Also refer to TPL_MATCH_CL_SC_P and TPL_MATCH_VID_PID macros.
    
*/

typedef struct
{
    union
    {
        uint32_t value;
        struct
        {
            /* Device Vendor ID to be matched */
            uint16_t idVendor;
            /* Device Product ID to be matched */
            uint16_t idProduct;
        };
        struct
        {
            /* Device class to be matched */
            uint8_t class;
            /* Device sub-class to be matched */
            uint8_t subClass;
            /* Device protocol to be matched */
            uint8_t protocol;
        };
    }device;
    /* TPL Flag for this entry */
    USB_HOST_TPL_FLAGS tplFlag;
    /* Pointer to Class driver function pointers table */
    void * classDriver;
}
USB_HOST_TARGET_PERIPHERAL_LIST;

// *****************************************************************************
/* USB Host Class Driver Match by Class, Subclass and Protocol

  Summary:
    This macro when used in the TPL table will direct the Host Layer to match
    the corresponding class driver by class, subclass and protocol.

  Description:
    This macro when used in the TPL table will direct the Host Layer to match
    the corresponding class driver by class, subclass and protocol. The
    application must set the device member of the TPL table entry to this macro
    is order to match the attached device to the class driver by device class,
    subclass and protocol.
 
  Remarks:
    None.
*/

#define TPL_MATCH_CL_SC_P(class,subClass,protocol)    /*DOM-IGNORE-BEGIN*/{((class)|((subClass) << 8)|((protocol) << 16))}/*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Class Driver Match by VID and PID

  Summary:
    This macro when used in the TPL table will direct the Host Layer to match
    the corresponding class driver by device vendor ID (VID) and product ID
   (PID).

  Description:
    This macro when used in the TPL table will direct the Host Layer to match
    the corresponding class driver by device vendor ID and product ID. The
    application must set the device member of the TPL table entry to this macro
    is order to match the attached device to the class driver by device Vendor ID
    and Product ID.

  Remarks:
    None.
*/

#define TPL_MATCH_VID_PID(vid,pid)   /*DOM-IGNORE-BEGIN*/ {((vid)|((pid) << 16))}/*DOM-IGNORE-END*/


// *****************************************************************************
/* USB Host Initialization Data Structure

  Summary:
    Defines the data required to initialize a USB Host Layer instance.

  Description:
    This data type defines the data required to initialize an USB Host Layer
    Instance. A pointer to a structure of this type is required by the
    USB_HOST_Initialize() function.  

  Remarks:
    Intentionally defined to be empty.
*/

typedef struct
{
    /* System Module Initialization data */
    SYS_MODULE_INIT     moduleInit;

    /* Identifies the Peripheral Library ID of the USB Peripheral to be used by
       this instance of the host layer */
    uint8_t             usbID;
    
    /* Identifies the Idle mode behavior. If true the USB module will stop when
       the microcontroller enter IDLE mode */
    bool                stopInIdle;

    /* If true, the USB module will automatically suspend when the 
       microcontroller enters Sleep mode */
    bool                suspendInSleep;

    /* Pointer to an endpoint table whose size should be
       USB_HOST_ENDPOINT_TABLE_SIZE number of bytes. */
    void *              endpointTable;

    /* Interrupt Peripheral Library ID of the interrupt source of USB module
       which this Host layer will control */
    INT_SOURCE          interruptSource;

    INT_SOURCE          interruptSourceUSBDma;

    /* Number of entries in the TPL table */
    int                  nTPLEntries;

    /* Speed at which the module should operate */
    USB_SPEED           usbSpeed;

    /* Pointer to the TPL table */
    USB_HOST_TARGET_PERIPHERAL_LIST *tplList;
}
USB_HOST_INIT;

// *****************************************************************************
/* USB Host VBUS Power Request Event Data Structure

  Summary:
    Defines the data returned along with the USB_HOST_EVENT_VBUS_REQUEST_POWER
    event. 

  Description:
    This data type defines the data returned along with the
    USB_HOST_EVENT_VBUS_REQUEST_POWER event. 

  Remarks:
    None.
*/

typedef struct
{
    /* Port where the device is connected */
    uint8_t port;
    /* Current (in 2mA units) that the device is requesting*/ 
    uint8_t current;
}
USB_HOST_EVENT_DATA_VBUS_REQUEST_POWER;

// *****************************************************************************
/* USB Host Events

  Summary:
    Identifies the possible events that the host layer can generate.

  Description:
    This enumeration identifies the possible events that the host layer can
    generate. A Host Layer client should register an event handler using the
    USB_HOST_EventCallBackSet function to receive host layer events. An event
    may have data associated with it. The event description before provides
    details of such cases.

  Remarks:
    None.
*/

typedef enum
{
    /* An attached device is requesting a change in VBUS power level. The
       eventData parameter will point to a
       USB_HOST_EVENT_DATA_VBUS_REQUEST_POWER data type that contains
       information on the amount of current requested. The application must
       respond with USB_HOST_EVENT_RESPONSE_OK if the requested current
       requirement can be met. The application should respond with
       USB_HOST_EVENT_RESPONSE_ERROR in case the current requirement cannot be
       met.*/
    USB_HOST_EVENT_VBUS_REQUEST_POWER,
    
    /* An unsupported device was attached. The attached device could not
       be matched to any entries in the TPL table. There is not event data
       associated with this event. The application should respond with
       USB_HOST_EVENT_RESPONSE_NONE. */
       
    USB_HOST_EVENT_UNSUPPORTED_DEVICE,

    /* The host layer could not enumerate the attached device. This can happen
       when the device does not respond to control transfer. There is no event
       data associated with this event. The application should respond with
       USB_HOST_EVENT_RESPONSE_NONE. */
    USB_HOST_EVENT_CANNOT_ENUMERATE,

    USB_HOST_EVENT_CONFIGURATION_COMPLETE,

    USB_HOST_EVENT_CONFIGURATION_FAILED,

    USB_HOST_EVENT_DEVICE_SUSPENDED,

    USB_HOST_EVENT_DEVICE_RESUMED


}
USB_HOST_EVENTS;

// *****************************************************************************
/* USB Host Event Handler Function Pointer Type.

  Summary:
    USB Host Event Handler Function Pointer Type.

  Description:
    This data type defines the required function signature of the USB Host Layer
    event handling callback function. The application must register a pointer to
    a USB Host Event handling function whose function signature (parameter and
    return value types) match the types specified by this function pointer in
    order to receive event call backs from the USB Host Layer. The function
    driver will invoke this function with event relevant parameters. The
    description of the event handler function parameters is given here.
    
    event                   - Type of event generated.
    
    eventData                - This parameter should be type casted to a event
                               specific pointer type based on the event that has
                               occurred. Refer to the USB_HOST_EVENTS 
                               enumeration description for more details.
    
    context                 - Value identifying the context of the application that 
                              registered the event handling function.

  Remarks:
    None.
*/
typedef USB_HOST_EVENT_RESPONSE (* USB_HOST_EVENT_CALLBACK )
            (USB_HOST_EVENTS event, void * eventData, uintptr_t context);

// *****************************************************************************
// *****************************************************************************
// Section: USB Host Layer Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
      SYS_MODULE_OBJ USB_HOST_Initialize( const SYS_MODULE_INDEX index,
                                           const SYS_MODULE_INIT * const init )

  Summary:
    Initializes the USB Host layer instance for the specified instance index.

  Description:
    This routine initializes the USB Host Layer instance for the specified
    instance index, making it ready for clients to open and use it. The
    initialization data is specified by the init parameter. The initialization
    may fail if the number of host layer objects allocated are insufficient or
    if the specified host layer instance is already initialized. This function
    also initializes the USB Module driver for host mode operation.  The init
    data structure thus contains information required for driver initialization.
    Note that initializing the Host Layer does not enable its operations. This 
    must be done using USB_HOST_OperationEnable function. 

  Precondition:
    None.

  Input:
    index  - Identifier for the instance to be initialized
    
    init   - Pointer to a USB_HOST_INIT data structure containing data necessary 
             to initialize the driver. 

  Return:
    If successful, returns a valid handle to a driver instance object.  
    Otherwise, returns SYS_MODULE_OBJ_INVALID.

  Example:
    <code>

    // Initializes the host layer for use with PIC32MX devices.
    // We assume that the TPL table is already created and has two
    // entries.

    USB_HOST_TARGET_PERIPHERAL_LIST * tplTable;

    USB_HOST_INIT   usbHostInit;               
    SYS_MODULE_OBJ  objectHandle;

    // Allocate an endpoint table
    uint8_t __attribute__((aligned(512))) endpointTable[USB_HOST_ENDPOINT_TABLE_SIZE];

    usbHostInit.moduleInit = SYS_MODULE_POWER_RUN_FULL;
    
    // The usbID member of the USB_HOST_INIT data structure
    // should be USB_ID_x for PIC32MX devices and should be
    // USBHS_ID_x PIC32MZ devices. Typical values are USB_ID_1
    // for PIC32MX and USBHS_ID_0 for PIC32MZ devices.

    usbHostInit.usbID           = USB_ID_1; //for PIC32MX devices
    usbHostInit.stopInIdle      = false;
    usbHostInit.suspendInSleep  = false;
    usbHostInit.endpointTable   = endpointTable;
    usbHostInit.interruptSource = INT_SOURCE_USB_1;
    usbHostInit.nTPLEntries     = 2
    usbHostInit.usbSpeed        = USB_SPEED_FULL;
    usbHostInit.tplTable        = tplTable;
    
    objectHandle = USB_HOST_Initialize(USB_HOST_0, (SYS_MODULE_INIT*)usbHostInit);
    if (SYS_MODULE_OBJ_INVALID == objectHandle)
    {
        // Handle error
    }
    </code>
  Remarks:
    This routine must be called before any other USB Host routine is called.

    This routine should only be called once during system initialization
    unless USB_HOST_Deinitialize is called to deinitialize the Host Layer 
    instance. This routine will NEVER block for hardware access. 
*/

SYS_MODULE_OBJ  USB_HOST_Initialize (const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * init);

// *****************************************************************************
/* Function:
    void USB_HOST_Deinitialize( SYS_MODULE_OBJ object )

  Summary:
    Deinitializes the specified instance of the USB Host Layer.

  Description:
    Deinitializes the specified instance of the USB Host Layer. This function
    will also deinitialize the USB Module driver and hence stop all USB Host
    related operation on the bus. All internal data structures will be reset.
  
  Precondition:
    Function USB_HOST_Initialize should have been called before calling this
    function.

  Parameters:
    object          - USB Host layer object handle, returned from the 
                      USB_HOST_Initialize routine

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;     //  Returned from USB_HOST_Initialize
    SYS_STATUS          status;


    USB_HOST_Deinitialize(object);

    status = USB_HOST_Status(object);
    if (SYS_MODULE_DEINITIALIZED != status)
    {
        // Check again later if you need to know 
        // when the driver is deinitialized.
    }
    </code>

  Remarks:
    Once the Initialize operation has been called, the Deinitialize operation
    must be called before the Initialize operation can be called again. This 
    routine will NEVER block waiting for hardware. 
*/
void USB_HOST_Deinitialize          (SYS_MODULE_OBJ driverObject);

// *****************************************************************************
/* Function:
    SYS_STATUS USB_HOST_Status( SYS_MODULE_OBJ object )

  Summary:
    Gets the current status of the USB Host Layer.

  Description:
    This routine provides the current status of the USB Host Layer.

  Precondition:
    Function USB_HOST_Initialize should have been called before calling this
    function.

  Parameters:
    object          - USB Host Layer object handle, returned from the 
                      USB_HOST_Initialize routine

  Returns:
    SYS_STATUS_READY          - Indicates that the USB Host layer is ready for
                                operations. 
    
    SYS_STATUS_DEINITIALIZED  - Indicates that the driver has been 
                                deinitialized

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from USB_HOST_Initialize
    SYS_STATUS          status;

    status = USB_HOST_Status(object);
    if (SYS_STATUS_READY == status)
    {
        // This means the driver can be opened using the
        // USB_HOST_Open function.
    }
    </code>

  Remarks:
    None.
*/
SYS_STATUS USB_HOST_Status          (SYS_MODULE_OBJ driverObject);

// *****************************************************************************
/* Function:
    void USB_HOST_Tasks (SYS_MODULE_OBJ object );

  Summary:
    Maintains the USB Host's state machine. It manages the USB Module driver and
    responds to USB Module driver events.

  Description:
    This routine maintains the USB Host layer's state machine. It manages and
    the USB Module driver and responds to USB module driver events.  This
    function should be called from the SYS_Tasks function.

  Precondition:
    The USB_HOST_Initialize routine must have been called for the specified 
    USB Host Layer instance.

  Parameters:
    object      - Object handle for the specified driver instance (returned from
                  USB_HOST_Initialize)

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from USB_HOST_Initialize

    while (true)
    {
        USB_HOST_Tasks (object);
        
        // Do other tasks
    }
    </code>

  Remarks:
    This routine is normally not called directly by an application.  
*/
void USB_HOST_Tasks   (SYS_MODULE_OBJ driverObject);

// *****************************************************************************
// *****************************************************************************
// Section: USB Host Layer Client Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    USB_HOST_HANDLE USB_HOST_Open( const SYS_MODULE_INDEX index,
                               const DRV_IO_INTENT    ioIntent )

  Summary:
    Opens the specified Host Layer instance and returns a handle to it

  Description:
    This routine opens the specified Host Layer instance and provides a handle
    that must be provided to all other client-level operations to identify the
    caller and the instance of the host layer. The host layer can be opened
    exclusively by a client by specifying the ioIntent as
    DRV_IO_INTENT_EXCLUSIVE. Any other flag settings will not have an effect on
    clients interaction with the host layer. 

  Precondition:
    Function USB_HOST_Initialize must have been called before calling this
    function.

  Parameters:
    index       - Identifier for the object instance to be opened
    
    intent      - can be DRV_IO_INTENT_EXCLUSIVE which allows a client to open the
                  host layer exclusively. Any other flags are ignored.
  
  Returns:
    If successful, the routine returns a valid open-instance handle (a number
    identifying both the caller and the module instance).
    
    If an error occurs, the return value is USB_HOST_HANDLE_INVALID. Error can occur
    - if the number of client objects allocated via USB_HOST_CLIENTS_NUMBER is
    insufficient.
    - if the client is trying to open the host layer but host layer has been opened
    exclusively by another client.
    - if the host layer instance being opened is not initialized or is
    invalid.

  Example:
    <code>
    USB_HOST_HANDLE handle;

    handle = USB_HOST_Open(USB_HOST_0, DRV_IO_INTENT_EXCLUSIVE);
    if (USB_HOST_HANDLE_INVALID == handle)
    {
        // Unable to open the driver
        // May be the driver is not initialized or the initialization
        // is not complete.
    }
    </code>

  Remarks:
    The handle returned is valid until the USB_HOST_Close routine is called.
    This routine will NEVER block waiting for hardware. This function is thread
    safe in a RTOS application. It should not be called in an ISR.
*/
USB_HOST_HANDLE USB_HOST_Open           (SYS_MODULE_INDEX index, const DRV_IO_INTENT ioIntent);

// *****************************************************************************
/* Function:
    void USB_HOST_Close( USB_HOST_HANDLE handle )

  Summary:
    Closes an opened-instance of the USB Host Layer.

  Description:
    This routine closes an opened-instance of the USB Host Layer, invalidating
    the handle.

  Precondition:
    The USB_HOST_Initialize routine must have been called for the specified
    USB Host Layer instance.

  Parameters:
    handle       - A valid open-instance handle, returned from the Host Layer's
                   open routine

  Returns:
    None

  Example:
    <code>
    USB_HOST_HANDLE handle;  // Returned from USB_HOST_Open

    USB_HOST_Close(handle);
    </code>

  Remarks:
    After calling this routine, the handle passed in "handle" must not be used
    with any of the remaining driver routines.  A new handle must be obtained by
    calling USB_HOST_Open before the caller may use the driver again.  This
    function is thread safe in a RTOS application.

    Note:
    Usually there is no need for the driver client to verify that the Close 
    operation has completed.
*/

void        USB_HOST_Close              (USB_HOST_HANDLE hostHandle);

// *****************************************************************************
/* Function:
    USB_ERROR USB_HOST_EventCallBackSet ( USB_HOST_HANDLE hostHandle,
        USB_HOST_EVENT_CALLBACK callback, uintptr_t context);

  Summary:
    Allows a client to identify an event handling function for the host layer
    to call back when host layer events are generated.

  Description:
    This function allows a client to identify an event handling function for the
    host layer to call back when host layer events are generated.
    
    The event handler should be set before the client the host layer operation
    is enabled. The event handler once set, persists until the client closes the
    driver or sets another event handler. 

  Precondition:
    The USB_HOST_Initialize routine must have been called for the specified
    Host Layer instance.

    USB_HOST_Open must have been called to obtain a valid opened Host Layer
    handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the host layer's
                   open routine

    callback     - Pointer to the event handler function.
    
    context      - The value of parameter will be passed back to the client 
                   unchanged, when the eventHandler function is called.  It can
                   be used to identify any client specific data object that 
                   identifies the instance of the client module (for example, 
                   it may be a pointer to the client module's state structure).

  Returns:
    USB_ERROR_NONE - Callback function was registered successfully.
    USB_ERROR_PARAMETER_INVALID - Callback function was NULL or hostHandle
    is invalid.

  Example:
    <code>
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    USB_HOST_HANDLE usbHostHandle;

    // usbHostHandle is the handle returned 
    // by the USB_HOST_Open function.
    
    // Client registers an event handler with host layer. 

    USB_HOST_EventCallBackSet( usbHostHandle, APP_USBHostEventHandler, 
                                     (uintptr_t)&myAppObj );

    USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler(uintptr_t context, 
                USB_HOST_EVENTS event, void * eventData)
    {
        switch(event)
        {
            case USB_HOST_EVENT_UNSUPPORTED_DEVICE:
                break;
            ...
        }
        return(USB_HOST_EVENT_RESPONSE_NONE);
    }
    </code>

  Remarks:
    None.
   
*/

void USB_HOST_EventCallBackSet ( USB_HOST_HANDLE hostHandle,
    USB_HOST_EVENT_CALLBACK callback, uintptr_t context);

// *****************************************************************************
/* Function:
    void USB_HOST_OperationEnable( USB_HOST_HANDLE handle )

  Summary:
    Enables host operation. 

  Description:
    The function enables host operation. When enabled, the host layer can detect
    and enumerate attached devices. The application must call the
    USB_HOST_OperationIsEnabled() function to check if the operation has
    completed.

  Precondition:
    The USB_HOST_Initialize routine must have been called for the specified
    USB Host Layer instance.

  Parameters:
    handle       - A valid open-instance handle, returned from the Host Layer's
                   open routine

  Returns:
    None

  Example:
    <code>
    USB_HOST_HANDLE handle;  // Returned from USB_HOST_Open

    // Enabled host operation
    USB_HOST_OperationEnable(handle);

    // Wait till host operation is enabled.
    while(!USB_HOST_OperationIsEnabled(handle));

    </code>

  Remarks:
    It is recommended that only on host layer client call this function.
    Multiple host layer clients calling this may cause indeterministic behavior.

*/
void        USB_HOST_OperationEnable    (USB_HOST_HANDLE hostHandle);

// *****************************************************************************
/* Function:
    void USB_HOST_OperationDisable( USB_HOST_HANDLE handle )

  Summary:
    Disabled host operation. 

  Description:
    The function disables host operation. Disabling the host operation will cause
    the host to ignore attached device. All bus communication will be halted and 
    USB bus will be suspended. 

  Precondition:
    The USB_HOST_Initialize routine must have been called for the specified
    USB Host Layer instance.

  Parameters:
    handle       - A valid open-instance handle, returned from the Host Layer's
                   open routine

  Returns:
    None

  Example:
    <code>
    USB_HOST_HANDLE handle;  // Returned from USB_HOST_Open

    // Enabled host operation
    USB_HOST_OperationDisable(handle);

    </code>

  Remarks:
    This function may not be used in a typical USB Host application. One
    possible use could be in a case where a fatal system error has occurred.
    Once disabled, the Host operation must be enabled again using the
    USB_HOST_OperationEnable function.
    
*/

void        USB_HOST_OperationDisable   (USB_HOST_HANDLE hostHandle);

// *****************************************************************************
/* Function:
    bool USB_HOST_OperationIsEnabled( USB_HOST_HANDLE handle )

  Summary:
    Allows the application to check if the host operation is enabled.

  Description:
    The function allows the application to check if the host operation is
    enabled. This function should be called after the USB_HOST_OperationEnable
    function is called to check if the operation has been enabled.

  Precondition:
    The USB_HOST_Initialize routine must have been called for the specified
    USB Host Layer instance.

  Parameters:
    handle       - A valid open-instance handle, returned from the Host Layer's
                   open routine

  Returns:
    None.

  Example:
    <code>
    USB_HOST_HANDLE handle;  // Returned from USB_HOST_Open

    // Enabled host operation
    USB_HOST_OperationEnable(handle);

    // Wait till host operation is enabled.
    while(!USB_HOST_OperationIsEnabled(handle));
    </code>

  Remarks:
    None.
*/

bool        USB_HOST_OperationIsEnabled (USB_HOST_HANDLE hostHandle);

// *****************************************************************************
/* Function:
    void USB_HOST_DeviceSuspend ( USB_HOST_HANDLE handle, uint8_t address );

  Summary:
    Suspends the USB.

  Description:
    The function suspends the USB. If address is an address of a device that is
    connected directly to the USB module, this function will directly suspend
    the bus. If the address is an address of a device that is connected to the
    module via a hub, the USB Host layer will suspend the hub port to which this
    device is connected. The function only places a request to suspend the
    device. The application must use the USB_HOST_DeviceIsSuspended function
    to check if the device suspend is complete. 

  Precondition:
    The USB_HOST_Initialize routine must have been called for the specified
    USB Host Layer instance.

  Parameters:
    handle       - A valid open-instance handle, returned from the Host Layer's
                   open routine
    
    address     - Address of the device.

  Returns:
    USB_ERROR_NONE  - The request was accepted successfully.
    USB_ERROR_PARAMETER_INVALID - The device address or handle is invalid.

  Example:
    <code>
    USB_HOST_HANDLE handle;  // Returned from USB_HOST_Open
    USB_ERROR result;

    // Suspend the device with address 2
    result = USB_HOST_Device(handle, 2);
    if(USB_ERROR_NONE == result)
    {
        // Wait till device is suspended.
        while(!USB_HOST_DeviceIsSuspended(handle, 2));
    }

    </code>

  Remarks:
    In cases where the device is connected directly to the USB Module, the bus
    suspend request will be serviced immediately. In cases where the device is
    connected to the USB module via a hub, the application must call the
    USB_HOST_DeviceIsSuspended function to check if the device has been
    suspended. 
*/

USB_ERROR   USB_HOST_DeviceSuspend      (USB_HOST_HANDLE hostHandle, uint8_t deviceAddress);


// *****************************************************************************
/* Function:
    void USB_HOST_DeviceResume ( USB_HOST_HANDLE handle, uint8_t address );

  Summary:
    Resumes the USB.

  Description:
    The function resumes the USB. If address is an address of a device that is
    connected directly to the USB module, this function will directly resume
    the bus. If the address is an address of a device that is connected to the
    module via a hub, the USB Host layer will resume the hub port to which this
    device is connected. The function only places a request to resume the
    device. The application must use the USB_HOST_DeviceIsResumeed function
    to check if the device resume is complete. 

  Precondition:
    The USB_HOST_Initialize routine must have been called for the specified
    USB Host Layer instance.

  Parameters:
    handle       - A valid open-instance handle, returned from the Host Layer's
                   open routine
    
    address     - Address of the device.

  Returns:
    USB_ERROR_NONE  - The request was accepted successfully.
    USB_ERROR_PARAMETER_INVALID - The device address or handle is invalid.

  Example:
    <code>
    USB_HOST_HANDLE handle;  // Returned from USB_HOST_Open
    USB_ERROR result;

    // Resume the device with address 2
    result = USB_HOST_Device(handle, 2);
    if(USB_ERROR_NONE == result)
    {
        // Wait till device is resumed.
        while(!USB_HOST_DeviceIsResumed(handle, 2));
    }

    </code>

  Remarks:
    In cases where the device is connected directly to the USB Module, the bus
    resume request will be serviced immediately. In cases where the device is
    connected to the USB module via a hub, the application must call the
    USB_HOST_DeviceIsResumed function to check if the device has been
    resumed.
*/



// *****************************************************************************
/* Function:
    void USB_HOST_Tasks_ISR( SYS_MODULE_OBJ hostLayerObj )

  Summary:
    USB Host Layer Tasks routine to be called in the USB Interrupt Service
    Routine.

  Description:
    This function must be called in the USB Interrupt Service Routine if the
    host Stack is configured for interrupt mode. In case the host Stack is
    configured for polling mode, this function is automatically called from the
    USB_HOST_Tasks function. hostLayerObj must be the system module object
    associated with the USB module generating the interrupt.

  Precondition:
    Host layer must have been initialized.

  Parameters:
    hostLayerObj    - Pointer to the Device Layer Object that is returned from
                     USB_HOST_Initialize

  Returns:
    None.

  Example:
    <code>
    // hostLayerObj is returned while initializing the USB1 module.
    // The USB_HOST_Tasks_ISR() function should be placed in the
    // USB1 module ISR.

    SYS_MODULE_OBJ hostLayerObj; // Returned by USB_HOST_Initialize().

    void __ISR(_USB_1_VECTOR, ipl4) USB1InterruptHandle(void)
    {
        USB_HOST_Tasks_ISR(usbHostLayerObj);
    }

    </code>

  Remarks:
    None.
*/

void USB_HOST_Tasks_ISR( SYS_MODULE_OBJ object );

void USB_HOST_Tasks_ISR_USBDMA(SYS_MODULE_OBJ hostLayerObj);


USB_ERROR   USB_HOST_DeviceResume       (USB_HOST_HANDLE hostHandle, uint8_t deviceAddress);

USB_ERROR USB_HOST_ConfigurationSet(USB_HOST_HANDLE hostHandle, uint8_t deviceAddress, uint8_t configurationNumber); 

#endif //_USB_HOST_H_


