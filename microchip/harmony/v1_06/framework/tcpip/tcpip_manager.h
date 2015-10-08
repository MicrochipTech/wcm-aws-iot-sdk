/*******************************************************************************
  Microchip TCP/IP Stack Definitions

  Company:
    Microchip Technology Inc.
    
  File Name:
    tcpip_manager.h

  Summary:
    This is the TCP/IP stack manager.

  Description:
    The TCP/IP manager provides access to the stack initialization, configuration and status.
    It also process the packets received from different network interfaces (MAC drivers)
    and dispatches them accordingly based on their type to the proper higher layer in the stack.

*******************************************************************************/
//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef __TCPIP_MANAGER_H_
#define __TCPIP_MANAGER_H_

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  


// *****************************************************************************
/*
  Type:
    TCPIP_NET_HANDLE

  Summary:
    Defines a network interface handle.

  Description:
    Definition of the network handle which clients
    use to get access to control the interfaces.

*/
typedef const void* TCPIP_NET_HANDLE;

// *****************************************************************************
/*
  Enumeration:
    TCPIP_LOCAL_MASK_TYPE

  Summary:
    Mask operation for local/non-local network interface.

  Description:
    These are the available operations that set
    the local/non-local detection network mask.
*/

typedef enum
{
    TCPIP_LOCAL_MASK_ZERO,      // use an all zero network mask
    TCPIP_LOCAL_MASK_ONE,       // use an all ones network mask
    TCPIP_LOCAL_MASK_NET,       // use the current network mask
    TCPIP_LOCAL_MASK_SET,       // use the set value for the network mask
                                
}TCPIP_LOCAL_MASK_TYPE;

// *****************************************************************************
/* TCPIP Stack Events Codes

  Summary:
    Defines the possible TCPIP event types.

  Description:
    This enumeration defines all the possible events that can be reported by the TCPIP stack.
    These are events received by the stack from the network interfaces.
    They are reported by the MAC driver of the network interface.

    Note that, depending on the type of the hardware interface, not all events are possible.

    Note that specific interfaces can offer specific events and functions to retrieve those events.
 */typedef enum
{
    // no event
    TCPIP_EV_NONE               = 0x0000,


    /*DOM-IGNORE-BEGIN*/
  // RX triggered events
    /*DOM-IGNORE-END*/

    // A receive packet is pending
    TCPIP_EV_RX_PKTPEND         = 0x0001,

    // RX FIFO overflow (system level latency, no descriptors, etc.)
    TCPIP_EV_RX_OVFLOW          = 0x0002,

    // no RX descriptor available to receive a new packet
    TCPIP_EV_RX_BUFNA           = 0x0004,

    // There's RX data available
    TCPIP_EV_RX_ACT             = 0x0008,

    // A packet was successfully received
    TCPIP_EV_RX_DONE            = 0x0010,

    // the number of received packets is >= than the RX Full Watermark
    TCPIP_EV_RX_FWMARK          = 0x0020,

    // the number of received packets is <= than the RX Empty Watermark
    TCPIP_EV_RX_EWMARK          = 0x0040,

    // a bus error encountered during an RX transfer
    TCPIP_EV_RX_BUSERR          = 0x0080,


    /*DOM-IGNORE-BEGIN*/
  // TX triggered events.
    /*DOM-IGNORE-END*/

    // A packet was transmitted and it's status is available
    TCPIP_EV_TX_DONE            = 0x0100,

    // a TX packet was aborted by the MAC (jumbo/system under-run/excessive defer/late collision/excessive collisions)
    TCPIP_EV_TX_ABORT           = 0x0200,

    // a bus error encountered during a TX transfer
    TCPIP_EV_TX_BUSERR          = 0x0400,


    /*DOM-IGNORE-BEGIN*/
  // Connection triggered events
    /*DOM-IGNORE-END*/

    // Connection Established
    TCPIP_EV_CONN_ESTABLISHED   = 0x0800,

    // Connection Lost
    TCPIP_EV_CONN_LOST          = 0x1000,

    /*DOM-IGNORE-BEGIN*/
    // Some useful masks:
    /*DOM-IGNORE-END*/
    // Mask of all RX related events
    TCPIP_EV_RX_ALL             = (TCPIP_EV_RX_PKTPEND|TCPIP_EV_RX_OVFLOW|TCPIP_EV_RX_BUFNA|TCPIP_EV_RX_ACT|
                                   TCPIP_EV_RX_DONE|TCPIP_EV_RX_FWMARK|TCPIP_EV_RX_EWMARK|TCPIP_EV_RX_BUSERR),

    // Mask of all TX related events
    TCPIP_EV_TX_ALL            = (TCPIP_EV_TX_DONE|TCPIP_EV_TX_ABORT|TCPIP_EV_TX_BUSERR),

    // All events showing some abnormal traffic/system condition
    // Action should be taken accordingly by the stack (or the stack user)
    TCPIP_EV_RXTX_ERRORS        = (TCPIP_EV_RX_OVFLOW|TCPIP_EV_RX_BUFNA|TCPIP_EV_RX_BUSERR|TCPIP_EV_TX_ABORT|TCPIP_EV_TX_BUSERR),


    // Mask of all Connection related events
    TCPIP_EV_CONN_ALL            = (TCPIP_EV_CONN_ESTABLISHED|TCPIP_EV_CONN_LOST),

} TCPIP_EVENT;


// *****************************************************************************
/* TCPIP event notification handler Pointer

  Function:
    void* <FunctionName> ( TCPIP_NET_HANDLE hNet, TCPIP_EVENT tcpEvent, void* fParam )

  Summary:
    Pointer to a function(handler) that will get called to process an event

  Description:
    Pointer to a function that may be called from within an ISR
    when a TCPIP event is available.

  Precondition:
    None

  Parameters:
    hNet        - network handle
    tcpEvent    - ORed mask of events that occurred
    fParam      - user passed parameter

  Returns:
    None

  Remarks:
    This function may be invoked  from within an ISR.
    It should be kept as short as possible and it should not include
    blocking or polling code.
 */
 typedef void (*TCPIP_STACK_EVENT_HANDLER)(TCPIP_NET_HANDLE hNet, TCPIP_EVENT, const void* fParam);

// *****************************************************************************
/*
  Type:
    TCPIP_EVENT_HANDLE

  Summary:
    Defines a TCPIP stack event handle.

  Description:
    Definition of an event handle used for event
    registration by the stack clients.

*/
typedef const void* TCPIP_EVENT_HANDLE;


// *****************************************************************************
/*
  Enumeration:
    TCPIP_MODULE_SIGNAL

  Summary:
    Lists the signals that are generated by the TCP/IP stack manager
    and processed by the stack modules.

  Description:
    These signals are generated by the stack manager towards the internal stack modules;
    A stack app could use them to provide wake up conditions for a thread waiting for a signal to occur.
    This is mainly useful when the stack modules tasks functions are executed at the app level
    and the app needs a way to identify that a module needs attention.

 Remarks:
    Multiple signals could be ORed.

    Only 16 bit values are maintained for this type of signal.

*/


typedef enum
{
    /* regular module processing signals */

    /* no pending signal */
    TCPIP_MODULE_SIGNAL_NONE                = 0x0000,       

    /* RX packet pending for processing */
    TCPIP_MODULE_SIGNAL_RX_PENDING          = 0x0001,

    /* module timeout has occurred */
    /* for modules that implement a state machine advancing on timer signals */
    TCPIP_MODULE_SIGNAL_TMO                 = 0x0002,       
    
    /* other signal will be added here */

    /* Special signals */
    /* module asynchronous attention required */
    /* this is a signal that's requested by modules that need special attention  */
    /* module is required to clear this flag when out of the critical processing */
    TCPIP_MODULE_SIGNAL_ASYNC               = 0x0100,       


    /* All processing signals mask */
    /* Note that TCPIP_MODULE_SIGNAL_ASYNC has to be cleared explicitely */
    TCPIP_MODULE_SIGNAL_MASK_ALL            = (TCPIP_MODULE_SIGNAL_RX_PENDING | TCPIP_MODULE_SIGNAL_TMO),

                                                    
}TCPIP_MODULE_SIGNAL;

// *****************************************************************************
/*
  Type:
    TCPIP_MODULE_SIGNAL_HANDLE

  Summary:
    Defines a TCPIP stack signal function handle.

  Description:
    Definition of an signal function handle used
    for signal registration by the stack clients.

*/
typedef const void* TCPIP_MODULE_SIGNAL_HANDLE;

// *****************************************************************************
/* TCPIP module signal function

  Function:
    void* <FunctionName> ( TCPIP_MODULE_SIGNAL_HANDLE sigHandle, TCPIP_STACK_MODULE moduleId, TCPIP_MODULE_SIGNAL signal )

  Summary:
    Pointer to a function(handler) that will get called when a stack internal signal occurred

  Description:
    Pointer to a function that will be called from within the TCP/IP stack
    when a signal is delivered to a stack module.

  Precondition:
    None

  Parameters:
    sigHandle   - signal handle obtained from a registration call
    moduleId    - module that receives the signal
    signal      - the occurring signal

  Returns:
    None

  Remarks:
    This function should be kept as short as possible and it should not include
    blocking or polling code.
    It's for setting flags/signalling purposes only.

 */

typedef void (*TCPIP_MODULE_SIGNAL_FUNC)(TCPIP_MODULE_SIGNAL_HANDLE sigHandle, TCPIP_STACK_MODULE moduleId, TCPIP_MODULE_SIGNAL signal);

/*********************************************************************
   Function:  
    SYS_MODULE_OBJ      TCPIP_STACK_Initialize(const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init);
  
   Summary:
    Stack initialization function.

   Description:     The function initializes the stack.
                    If an error occurs, the SYS_ERROR() is called
                    and the function deinitialize itself.
                    
   Precondition:    None 


   Parameters:
    - index - index of the TCP/IP stack to be initialized
    - init  - Pointer to initialization data.
              It should be a TCPIP_STACK_INIT structure carrying the following data:
                    pNetConf  	- pointer to an array of TCPIP_NETWORK_CONFIG to support
                    nNets       - number of network configurations in the array
                    pModConfig  - pointer to an array of TCPIP_STACK_MODULE_CONFIG
                    nModules    - number of modules to initialize 
  
   Returns: 
    - a valid handle to a driver object, if successful.
    - SYS_MODULE_OBJ_INVALID if initialization failed.
  
  
   Remarks: 
    This function must be called before any of the
    stack or its component routines are used.
  
    New TCPIP_NETWORK_CONFIG types could be added/removed at run time for implementations that support
    dynamic network interface creation.

    Only one instance of the TCP/IP stack can run in the system and 
    the index parameter is irrelevant

    If this call is made after the stack is successfully initialized,
    the current TCP/IP stack object handle will be returned.
 */
SYS_MODULE_OBJ      TCPIP_STACK_Initialize(const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init);

/*********************************************************************
   Function:        void TCPIP_STACK_Deinitialize(SYS_MODULE_OBJ object)
  
   Summary:
    Stack deinitialization function.

   Description:
    This function performs the deinitialization of the TCPIP stack.
    All allocated resources are released.
  
   Precondition:
    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()

   Parameters:
    - object    - Object handle, returned from TCPIP_STACK_Initialize call
  
   Returns: 
    None
  
   Remarks:
    None
 */
void                TCPIP_STACK_Deinitialize(SYS_MODULE_OBJ object);

/**************************************************************************
  Function:
       SYS_STATUS TCPIP_STACK_Status ( SYS_MODULE_OBJ object )
    
  Summary:
    Provides the current status of the TCPIP stack module.

  Description:
    This function provides the current status of the TCPIP stack
    module.

  Precondition:
    The TCPIP_STACK_Initialize function must have been called before calling
    this function.

  Parameters:
    object -  Object handle, returned from TCPIP_STACK_Initialize

  Returns:
    - SYS_STATUS_READY  - Indicates that any previous initialization operation for the
                          stack has completed
    - SYS_STATUS_BUSY   - Indicates that a previous initialization operation for the
                          stack has not yet completed
    - SYS_STATUS_ERROR  - Indicates that the initialization operation has failed
                          and the stack is in an error state

  Example:
    <code>
    </code>

  Remarks:
    None.
 **************************************************************************/

SYS_STATUS          TCPIP_STACK_Status ( SYS_MODULE_OBJ object );

/*********************************************************************
   Function:        void TCPIP_STACK_Task(SYS_MODULE_OBJ object)
  
   Summary:
    TCP/IP Stack task function.

   Description:
    TCP/IP stack execution state machine. 
    Stack Finite-state Machine (FSM) is executed.

   Precondition:
    TCPIP_STACK_Initialize() is already called.

   Parameters:
    - object -  Object handle returned by TCPIP_STACK_Initialize
  
   Returns:
    None 
  
   Remarks:
    This FSM checks for new incoming packets,
    and routes it to appropriate stack components.
    It also performs timed operations.
  
    This function must be called periodically to
    ensure timely responses.
 */
void                TCPIP_STACK_Task(SYS_MODULE_OBJ object);


/*********************************************************************
   Function:        int TCPIP_STACK_NumberOfNetworksGet(void)
  
   Summary:
    Number of network interfaces in the stack.

   Description:
    This function returns the number of interfaces currently active in the TCP/IP stack.

   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:           None
  
   Returns:          Number of network interfaces
  
   Remarks:            None
 */
int                 TCPIP_STACK_NumberOfNetworksGet(void);

/*********************************************************************
   Function:        TCPIP_NET_HANDLE TCPIP_STACK_NetHandleGet(const char* interface)
  
   Summary:
    Network interface handle from a name.

   Description:
    This function resolves a network interface name to a handle. 

  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      interface - The names specified in TCPIP_NETWORK_CONFIG. 
  
   Returns:         Network handle. 
                 
  Example:
  <code>
   TCPIP_NET_HANDLE hNet = TCPIP_STACK_NetHandleGet("PIC32INT");
    or
   TCPIP_NET_HANDLE hNet = TCPIP_STACK_NetHandleGet("eth0");
  </code>
  
   Remarks:         None
 */
TCPIP_NET_HANDLE    TCPIP_STACK_NetHandleGet(const char* interface);

/*********************************************************************
   Function:        const char* TCPIP_STACK_NetNameGet(TCPIP_NET_HANDLE netH)
  
   Summary:
    Network interface name from a handle.

   Description:
    This function returns the name associated with the interface handle. 
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get the name of.
  
   Returns:         It returns the name associated to that interface handle. 
   					Returns 0 if no such name.
                 
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_IndexToNet(0);
   const char* netName = TCPIP_STACK_NetNameGet(netH);
  </code>
  
   Remarks:         None
 */
const char*         TCPIP_STACK_NetNameGet(TCPIP_NET_HANDLE netH);


/*********************************************************************
   Function:        int TCPIP_STACK_NetAliasNameGet(TCPIP_NET_HANDLE netH, char* nameBuffer, int buffSize)
  
   Summary:
    Network interface alias name from a handle.

   Description:
    This function returns the alias name associated with the interface handle. 
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get the name of.
                    nameBuffer  - buffer to receive the name; could be 0 if only name size is needed
                    buffSize    - size of the provided buffer
  
   Returns:         It returns the number of characters of the interface alias name. 
   					Returns 0 if no such interface.
                 
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_IndexToNet(0);
   char ifName[8];
   TCPIP_STACK_NetAliasNameGet(netH, ifName, sizeof(ifName));
  </code>
  
   Remarks:
    The aliases names are:
        - "eth0", "eth1", etc. for Ethernet interfaces
        - "wlan0", "wlan1", etc. for Wi-Fi interfaces
    See the TCPIP_STACK_IF_NAME_ALIAS_ETH, TCPIP_STACK_IF_NAME_ALIAS_WLAN in tcpip.h
    for the aliases names.

    Alias interface names are at most 8 characters long.

 */
int TCPIP_STACK_NetAliasNameGet(TCPIP_NET_HANDLE netH, char* nameBuffer, int buffSize);


/*********************************************************************
   Function:
    const TCPIP_MAC_OBJECT* TCPIP_STACK_MACObjectGet(TCPIP_NET_HANDLE netH);
  
   Summary:
    Returns the network MAC driver object of this interface.
   
   Description:
    This function returns the MAC driver object that's associated with the interface handle. 
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get the name of.
  
   Returns:         It returns a valid MAC driver object pointer if success.
   					Returns 0 if no such interface or there is no MAC object.
                 
  Example:
  <code>
  </code>
  
   Remarks:
    The MAC driver object is the one that's passed at the stack/interface initialization.
    
 */
const TCPIP_MAC_OBJECT* TCPIP_STACK_MACObjectGet(TCPIP_NET_HANDLE netH);




/*********************************************************************
   Function:        uint32_t TCPIP_STACK_NetAddress(TCPIP_NET_HANDLE netH)
  
   Summary:
    Network interface IPv4 address.

   Description:
    If interface is enabled then the function will return the IPv4 address of the
    network interface.
    Else it will return 0.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get address of.
  
   Returns:         If interface is enabled then it returns the IP address of that interface 
   					else return 0
                 
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("eth0");
   uint32_t ipAdd = TCPIP_STACK_NetAddress(netH);
  </code>
  
   Remarks:         None
 */
uint32_t            TCPIP_STACK_NetAddress(TCPIP_NET_HANDLE netH);


/*********************************************************************
   Function:        uint32_t TCPIP_STACK_NetAddressGateway(TCPIP_NET_HANDLE netH)
  
   Summary:
    Network interface IPv4 gateway address.

   Description:
    If interface is enabled then the function will return the IPv4 gateway address of the
    network interface.
    Else it will return 0.
  

  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get address of.
  
   Returns:         If interface is enabled then it returns the gateway address of that interface.
   					Else return 0.
                 
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("eth0");
   uint32_t ipAdd = TCPIP_STACK_NetAddressGateway(netH);
  </code>
  
   Remarks:         None
 */
uint32_t            TCPIP_STACK_NetAddressGateway(TCPIP_NET_HANDLE netH);


/*********************************************************************
   Function:        uint32_t TCPIP_STACK_NetAddressDnsPrimary(TCPIP_NET_HANDLE netH)
  
   Summary:
    Network interface DNS address.

   Description:
    If interface is enabled then the function will return the IPv4 address of the
    primary DNS of the network interface.
    Else it will return 0.

  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get the DNS address of.
  
   Returns:         IPv4 address of the primary DNS server.
                    0 if not such interface or interface is down.
                 
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   myIPAddress = TCPIP_STACK_NetAddressDnsPrimary(netH);
  </code>
  
   Remarks:         None
 */
uint32_t            TCPIP_STACK_NetAddressDnsPrimary(TCPIP_NET_HANDLE netH);


/*********************************************************************
   Function:        uint32_t TCPIP_STACK_NetAddressDnsSecond(TCPIP_NET_HANDLE netH)
  
   Summary:
    Network interface secondary DNS address.

   Description:
    If interface is enabled then the function will return the IPv4 address of the
    secondary DNS of the network interface.
    Else it will return 0.
  

  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get the DNS address of.
  
   Returns:         The secondary DNS address if success.
                    0 if not such interface or interface is down.
                 
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   myIPAddress = TCPIP_STACK_NetAddressDnsSecond(netH);
  </code>
  
   Remarks:         None
 */
uint32_t            TCPIP_STACK_NetAddressDnsSecond(TCPIP_NET_HANDLE netH);


/*********************************************************************
   Function:        uint32_t TCPIP_STACK_NetMask(TCPIP_NET_HANDLE netH)
  
   Summary:
    Network interface IPv4 address mask.

   Description:
    The function returns the IPv4 address mask of the specified interface.
    If the interface is enabled then it returns the IP address mask of that
    interface else return 0.
   

  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get mask of.
  
   Returns:         IPv4 address mask of that interface 
   					0 if interface is disabled/non-existent.
  
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   uint32_t subMask = TCPIP_STACK_NetMask(netH);
  </code>
  
   Remarks:         None
 */
uint32_t            TCPIP_STACK_NetMask(TCPIP_NET_HANDLE netH);


/*********************************************************************
   Function:        const char* TCPIP_STACK_NetBIOSName(TCPIP_NET_HANDLE netH)
  
   Summary:
    Network interface NetBIOS name.

   Description:
     The function returns the network interface NetBIOS name.
     The interface should be enabled for this function to work.
  

  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get name of.
  
   Returns:         pointer to the NetBIOS name of that interface 
  
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   const char* biosName = TCPIP_STACK_NetBIOSName(netH);
  </code>
  
   Remarks:         None
 */
const char*         TCPIP_STACK_NetBIOSName(TCPIP_NET_HANDLE netH);

/*********************************************************************
   Function:        const uint8_t* TCPIP_STACK_NetAddressMac(TCPIP_NET_HANDLE netH)
  
   Summary:
    Network interface MAC address.

   Description:
     The function returns the network interface physical (MAC) address.
     The interface should be enabled for this function to work.
  

   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get the address of.
  
   Returns:         Constant pointer to the MAC address
   					0 if no such interface
  
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   const TCPIP_MAC_ADDR* pAdd = TCPIP_STACK_NetAddressMac(netH);
  </code>
  
   Remarks:         None
 */
const uint8_t*     TCPIP_STACK_NetAddressMac(TCPIP_NET_HANDLE netH);


/*********************************************************************
   Function:        uint32_t TCPIP_STACK_NetAddressBcast(TCPIP_NET_HANDLE netH)
  
   Summary:
    Network interface broadcast address.

   Description:
     The function returns the network interface broadcast address.
     The interface should be enabled for this function to work.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle to get address of.
  
   Returns:         Broadcast IP address of that interface 
   					0 if no such interface
  
   Remarks:	   	 	None
 */
uint32_t            TCPIP_STACK_NetAddressBcast(TCPIP_NET_HANDLE netH);

/*********************************************************************
   Function:        TCPIP_NET_HANDLE TCPIP_STACK_NetDefaultGet(void)
  
   Summary:
    Default network interface handle.

   Description:
     The function returns the current default network interface in the TCP/IP stack.
     This is the interface on which packets will be transmitted when the internal
     routing algorithm cannot detect a match for an outgoing packet.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      None
  
   Returns:         The default network interface.
  
   Remarks:        This function is intended for multi-homed hosts,
                   the TCP/IP stack running multiple interfaces.

 */
TCPIP_NET_HANDLE    TCPIP_STACK_NetDefaultGet(void);

/*********************************************************************
   Function:        bool TCPIP_STACK_NetDefaultSet(TCPIP_NET_HANDLE netH)
  
   Summary:
    Sets the default network interface handle.

   Description:
     The function sets the current default network interface in the TCP/IP stack.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle.
  
   Returns:         - true if success
   					- false if failed (the old interface does not change)
  
   Remarks:         None.
 */
bool                TCPIP_STACK_NetDefaultSet(TCPIP_NET_HANDLE netH);


/*********************************************************************
   Function:        TCPIP_NET_HANDLE TCPIP_STACK_IndexToNet(int netIx)
  
   Summary:
    Network interface handle from interface number.

   Description:
    This function converts an interface number to a network interface handle.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netIx - Interface index.
  
   Returns:         Network interface handle.
                 
   Remarks:         None
 */
TCPIP_NET_HANDLE    TCPIP_STACK_IndexToNet(int netIx);

/*********************************************************************
   Function:        int TCPIP_STACK_NetIndexGet(TCPIP_NET_HANDLE hNet);
  
   Summary:
    Network interface number from interface handle.

   Description:
    This function converts a network interface handle to an interface number.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      hNet - Interface handle.
  
   Returns:         Index of this network handle in the stack. 
  
   Remarks:         None
 */
int                 TCPIP_STACK_NetIndexGet(TCPIP_NET_HANDLE hNet);

/*********************************************************************
   Function:        bool TCPIP_STACK_NetIsUp(TCPIP_NET_HANDLE hNet);
  
   Summary:
    Gets the network interface up or down status.


   Description:
    This function returns the network interface up or down (enabled) status.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      hNet - Interface handle.
  
   Returns:         - true if interface exists and is enabled
                    - false otherwise
  
   Remarks:         None
 */
bool                TCPIP_STACK_NetIsUp(TCPIP_NET_HANDLE hNet);

/*********************************************************************
   Function:        bool TCPIP_STACK_NetIsLinked(TCPIP_NET_HANDLE hNet);
  
   Summary:
    Gets the network interface link status.

   Description:
    This function returns the network interface link status.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      hNet - Interface handle.
  
   Returns:         - true if interface exists and the corresponding MAC is linked
                    - false otherwise
  
   Remarks:         None
 */
bool                TCPIP_STACK_NetIsLinked(TCPIP_NET_HANDLE hNet);


/*********************************************************************
   Function:        bool TCPIP_STACK_NetIsReady(TCPIP_NET_HANDLE hNet);
  
   Summary:
    Gets the network interface configuration status.

   Description:
    This function returns the network interface link status.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      hNet - Interface handle.
  
   Returns:         - true if interface exists, the corresponding MAC is linked and the interface
                      is properly configured and ready for transfers
                    - false otherwise
  
   Remarks:         None
 */
bool                TCPIP_STACK_NetIsReady(TCPIP_NET_HANDLE hNet);


/*********************************************************************
   Function:        bool TCPIP_STACK_NetUp(TCPIP_NET_HANDLE netH, const TCPIP_NETWORK_CONFIG* pUsrConfig)
  
   Summary:
   Turns up a network interface.
   As part of this process, the corresponding MAC driver is initialized.

   Description:
    This function brings a network interface up and perform its initialization.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH 		- Interface handle.
   				 	- pUsrConfig  - pointer to a TCPIP_NETWORK_CONFIG for the interface initialization
  
   Returns:         - true if success
                    - false if no such network or an error occurred
  
   Remarks:         None
 */
bool                TCPIP_STACK_NetUp(TCPIP_NET_HANDLE netH, const TCPIP_NETWORK_CONFIG* pUsrConfig);

/*********************************************************************
   Function:        bool TCPIP_STACK_NetDown(TCPIP_NET_HANDLE netH)
  
   Summary:
   Turns down a network interface.

   Description:
    This function performs the deinitialization of a net interface.
    As part of this process, the corresponding MAC driver is deinitialized.

   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH - Interface handle.
  
   Returns:         - true if success
                    - false if no such network
  
   Remarks:            None
 */
bool                TCPIP_STACK_NetDown(TCPIP_NET_HANDLE netH);

/*********************************************************************
   Function:        bool TCPIP_STACK_SetLocalMasksType(TCPIP_NET_HANDLE netH, TCPIP_LOCAL_MASK_TYPE andType, TCPIP_LOCAL_MASK_TYPE orType)
  
   Summary:
    Sets the local/non-local masks type for network interface.

   Description:     This function sets the types of masks used in the stack decision of
                    a destination address being a local or non-local address.
                    For example, when a TCP connection is made, the advertised MSS
                    usually has different values for local versus non-local networks.
  
                    In order to decide if a destination IP address (destAdd) is local to
                    a network interface (having the netAdd address) the following calculation is performed:
                    if( ((destAdd & andMask) | orMask) == ((netAdd & andMask) | orMask))
                        then the destination address is considered to be a local address.
                    else
                        the destination address is non-local.
  
                    This function sets the types of masks used for the AND and OR operations, as follows:
                        - TCPIP_LOCAL_MASK_ZERO: use the all 0's mask (0x00000000)
                        - TCPIP_LOCAL_MASK_ONE: use the all 1's mask (0xffffffff)
                        - TCPIP_LOCAL_MASK_NET: use the current network mask
                        - TCPIP_LOCAL_MASK_SET: use a mask that's set by the application
                            There are operations to set a specific AND and OR masks
  
                    Using different values for the AND and OR masks an application can select
                    various destination networks to be considered as local/non-local:
                    - only the own network
                    - any network
                    - no network
                    - specific (range of) networks
                    - etc.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH    - handle of the interface to use
                    - andType - AND type of mask to use for local network detection 
                    - orType  - OR type of mask to use for local network detection 
  
   Returns:         - true if interface exists and is enabled
                    - false if an error occurred
                    
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   TCPIP_STACK_SetLocalMasksType(netH, TCPIP_LOCAL_MASK_NET, TCPIP_LOCAL_MASK_ZERO);
  </code>
  
   Remarks:         - The default value for both AND and OR masks is set to
                      TCPIP_LOCAL_MASK_ZERO so that any destination network
                      is considered to be local!
  
 */
bool                TCPIP_STACK_SetLocalMasksType(TCPIP_NET_HANDLE netH, TCPIP_LOCAL_MASK_TYPE andType, TCPIP_LOCAL_MASK_TYPE orType);

/*********************************************************************
   Function:        bool TCPIP_STACK_SetLocalMasks(TCPIP_NET_HANDLE netH, uint32_t andMask, uint32_t orMask)
  
   Summary:
    Sets the local/non-local masks for network interface.

   Description:     This function sets the masks used in the stack decision of
                    a destination address being a local or non-local address.
                    These masks will be used when the corresponding mask type is set to
                    TCPIP_LOCAL_MASK_SET.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH    - handle of the interface to use
                    - andMask - AND mask to use for local network detection, big endian (BE) format 
                    - orType  - OR mask to use for local network detection , BE format
  
   Returns:         - true if interface exists and is enabled
                    - false if an error occurred
                    
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   TCPIP_STACK_SetLocalMasks(netH, 0x0101a0c0, 0x0);
  </code>
  
   Remarks:            None
 */
bool                TCPIP_STACK_SetLocalMasks(TCPIP_NET_HANDLE netH, uint32_t andMask, uint32_t orMask);

/*********************************************************************
   Function:        bool  TCPIP_STACK_InitializeDataGet(SYS_MODULE_OBJ object, TCPIP_STACK_INIT* pStackInit)
  
   Summary:
    Get the TCP/IP stack initialization data.

   Description:
    This function returns the data that was used for the last call to the TCPIP_STACK_Initialize().
   
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:
    - object        - Object handle, returned from TCPIP_STACK_Initialize call
    - pStackInit    - Pointer to an address to store the stack initialization data
  
   Returns:
       true if the stack is up and running and the stack init data has been copied
       false if the stack is down or an error has occurred
                    
  Example:
  <code>
    TCPIP_STACK_INIT initData;
    TCPIP_STACK_InitializeDataGet(tcpipObject, &initData);
  </code>

  
   Remarks:
    The stack does not make a persistent copy of the
    TCPIP_NETWORK_CONFIG and TCPIP_STACK_MODULE_CONFIG data
    that are pased in the TCPIP_STACK_INIT at the moment of the
    stack initialization!
    It is up to the application to insure that the initialization data 
    is still available after the TCPIP_STACK_Initialize() has been called
    if this API is used.

 */
bool                TCPIP_STACK_InitializeDataGet(SYS_MODULE_OBJ object, TCPIP_STACK_INIT* pStackInit);


/*********************************************************************
   Function:        TCPIP_STACK_MODULE  TCPIP_STACK_NetMACIdGet(TCPIP_NET_HANDLE netH)
  
   Summary:
    Get the MAC ID of the network interface.

   Description:
    This function returns the module ID of the MAC that's attached to the
    specified network interface.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      netH    - handle of the interface to use
  
   Returns:         A TCPIP_STACK_MODULE ID that belongs to the MAC of that network interface.
                    
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   TCPIP_STACK_MODULE modId = TCPIP_STACK_NetMACIdGet(netH);
   if(modId == TCPIP_MODULE_MAC_PIC32INT)
   {
        // an internal PIC32 MAC attached to this interface
   }
  </code>

  
   Remarks:            None
 */
TCPIP_STACK_MODULE  TCPIP_STACK_NetMACIdGet(TCPIP_NET_HANDLE netH);


/*********************************************************************
   Function:
    bool TCPIP_STACK_NetMACStatisticsGet(TCPIP_NET_HANDLE netH, TCPIP_MAC_RX_STATISTICS* pRxStatistics, TCPIP_MAC_TX_STATISTICS* pTxStatistics)
  
   Summary:
    Get the MAC statistics data.

   Description:
    This function returns the statistics data of the MAC that's attached to the
    specified network interface.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:
       - netH          - handle of the interface to use
       - pRxStatistics - pointer to a TCPIP_MAC_RX_STATISTICS that will receive the current 
                         RX statistics counters. Can be NULL if not needed.

       - pTxStatistics - pointer to a TCPIP_MAC_TX_STATISTICS that will receive the current 
                         TX statistics counters. Can be NULL if not needed.
  
   Returns:
    - true if the call succeeded
    - false if the call failed (the corresponding MAC does not implement statistics counters).
                    
  Example:
  <code>
   TCPIP_MAC_RX_STATISTICS rxStatistics;
   TCPIP_MAC_TX_STATISTICS txStatistics;
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   if(TCPIP_STACK_NetMACStatisticsGet(netH, &rxStatistics, &txStatistics))
   {
        // display the statistics for the internal PIC32 MAC attached to this interface
   }
  </code>

  
   Remarks:            None
 */

bool  TCPIP_STACK_NetMACStatisticsGet(TCPIP_NET_HANDLE netH, TCPIP_MAC_RX_STATISTICS* pRxStatistics, TCPIP_MAC_TX_STATISTICS* pTxStatistics);

/*********************************************************************
   Function:
    bool  TCPIP_STACK_NetMACRegisterStatisticsGet(TCPIP_NET_HANDLE netH, TCPIP_MAC_STATISTICS_REG_ENTRY* pRegEntries, int nEntries, int* pHwEntries);
  
   Summary:
    Get the MAC statistics register data.

   Description:
    This function returns the hardware statistics register data of the MAC that's attached to the
    specified network interface.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:
    - netH           - handle of the interface to use

    - pRegStatistics - pointer to a pRegEntries that will receive the current 
                       hardware statistics registers values.
                       Can be 0, if only the number of supported hardware registers is requested
    - nEntries       - provides the number of TCPIP_MAC_STATISTICS_REG_ENTRY structures present in pRegEntries
                       Can be 0, if only the number of supported hardware registers is requested
                       The register entries structures will be filled by the driver, up to the
                       supported hardware registers.

    - pHwEntries    - pointer to an address to store the number of the statistics registers that the hardware supports
                      It is updated by the driver.
                      Can be 0 if not needed

  
   Returns:
    - true if the call succeeded
    - false if the call failed (the corresponding MAC does not implement hardware statistics counters).
                    
  Example:
  <code>
   TCPIP_MAC_REG_STATISTICS regStatistics;
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   if(TCPIP_STACK_NetMACRegisterStatisticsGet(netH, &regStatistics))
   {
        // display the hardware statistics registers for the internal PIC32 MAC attached to this interface
   }
  </code>

  
   Remarks:            None
 */

bool  TCPIP_STACK_NetMACRegisterStatisticsGet(TCPIP_NET_HANDLE netH, TCPIP_MAC_STATISTICS_REG_ENTRY* pRegEntries, int nEntries, int* pHwEntries);

/*********************************************************************
   Function:        size_t  TCPIP_STACK_ModuleConfigGet(TCPIP_STACK_MODULE modId, void* configBuff, size_t buffSize, size_t* pNeededSize)
  
   Summary:
    Get stack module configuration data.
  
   Description:     This function returns the current configuration data of the stack module
                    specified by the corresponding module ID.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - modId       - the ID that identifies the requested module
                    - configBuff  - pointer to a buffer that will receive the configuration data
                                    If this pointer is 0, just the pNeededSize will be updated
                    - buffSize    - size of the provided buffer
                    - pNeededSize - pointer to an address to store the number of bytes needed to store this module configuration data
                                    Can be NULL if not needed.
  
  
   Returns:          number of bytes copied to the user buffer:
                     -   -1 if the module ID is invalid
                     -    0 if the configBuff is NULL or buffSize is less than required
                     -   >0 if the call succeeded and the configuration was copied 
  
                    
  Example:
  <code>
   uint8_t configBuffer[200];
   size_t configSize;
   size_t copiedSize;
   copiedSize = TCPIP_STACK_ModuleConfigGet(TCPIP_MODULE_MAC_MRF24W, configBuffer, sizeof(configBuffer), &configSize);
  </code>
  
  
   Remarks:         Currently only the MAC modules implement this function.
 */
size_t              TCPIP_STACK_ModuleConfigGet(TCPIP_STACK_MODULE modId, void* configBuff, size_t buffSize, size_t* pNeededSize);

/*********************************************************************
   Function:        size_t TCPIP_STACK_NetConfigGet(TCPIP_NET_HANDLE netH, void* configStoreBuff, size_t configStoreSize, size_t* pNeededSize)
  
   Summary:
    Get stack network interface configuration data.

   Description:     This function dumps the current configuration data of the network interface
                    specified by the corresponding network handle into the supplied buffer.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH              - the handle that identifies the requested interface
                    - configStoreBuff   - pointer to a buffer that will receive the current configuration data.
                                          All the data that's needed to restore a TCPIP_NETWORK_CONFIG structure
                                          is stored in this buffer.
                                          Can be NULL if only the storage size is needed.
                    - configStoreSize   - size of the supplied buffer
                    - pNeededSize       - pointer to store the size needed for storage;
                                          Can be NULL if not needed
  
   Returns:         - -1 if the interface is invalid or the stack is not initialized
                    -  0 if no data is copied (no supplied buffer of buffer too small)
                    - >0 for success, indicating the amount of data copied. 
                    
  Example:
  <code>
   uint8_t currConfig[100];
   size_t neededSize, result;
   TCPIP_NET_HANDLE hNet = TCPIP_STACK_NetHandleGet("PIC32INT");
   result = TCPIP_STACK_NetConfigGet(hNet, currConfig, sizeof(currConfig), &neededSize);
   if(result > 0)
   {   
        // store the currConfig to some external storage
   }
  </code>
  
  
   Remarks:         - The function is a helper for retrieving the network configuration data.
                      Its companion function, TCPIP_STACK_NetConfigSet, restores
                      the TCPIP_NETWORK_CONFIG from the dump buffer. 
  
                    - Currently the data is saved in plain binary format 
                      into the supplied buffer.
                      However the application must not make use of this assumption
                      as it may change in the future releases
                      (some compression scheme may be implemented).
 */
size_t              TCPIP_STACK_NetConfigGet(TCPIP_NET_HANDLE netH, void* configStoreBuff, size_t configStoreSize, size_t* pNeededSize);

/*********************************************************************
   Function:        TCPIP_NETWORK_CONFIG* TCPIP_STACK_NetConfigSet(void* configStoreBuff, void* netConfigBuff, size_t buffSize, size_t* pNeededSize);
  
   Summary:
    Restores stack network interface configuration data.

   Description:     This function restores data from a previously dump buffer and updates the supplied interface configuration.
                    All the data is recovered and constructed into the netConfigBuff (supposing this buffer is large enough).
                    If this operation succeeded, the netConfigBuff can be safely cast to a (TCPIP_NETWORK_CONFIG*).
  
                    The structure of the netConfigBuff is as follows:
                    - a TCPIP_NETWORK_CONFIG structure is created at the very beginning of the buffer.
                    - all the needed fields that are part of the TCPIP_NETWORK_CONFIG will be placed in the buffer itself.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - configStoreBuff  - pointer to a buffer that received configuration data from a TCPIP_STACK_NetConfigGet call.
  
                    - netConfigBuff    - pointer to a buffer that will receive the TCPIP_NETWORK_CONFIG data
  
                    - buffSize         - size of the supplied netConfigBuff buffer
  
                    - pNeededSize      - pointer to store the size needed for storage;
                                         Can be NULL if not needed
                                         If supplied, the pNeededSize will be updated with
                                         the actual size that's needed for the netConfigBuff.
  
   Returns:         - valid TCPIP_NETWORK_CONFIG pointer (netConfigBuff) if the netConfigBuff is successfully updated
                    - 0 if the netConfigBuff is not supplied or not big enough
  
                    
  Example:
  <code>
   uint8_t currConfig[100];
   uint8_t restoreBuff[100];
   size_t neededSize, result;
   TCPIP_NET_HANDLE hNet = TCPIP_STACK_NetHandleGet("PIC32INT");
   result = TCPIP_STACK_NetConfigGet(hNet, currConfig, sizeof(currConfig), &neededSize);
   if(result > 0)
   {   
        // store the currConfig buffer to some external storage (neededSize bytes needed)

        // later on restore the configuration
        TCPIP_NETWORK_CONFIG* netConfig;
        // extract the network configuration from the previously saved buffer 
        netConfig = TCPIP_STACK_NetConfigSet(currConfig, restoreBuff, sizeof(restoreBuff), neededSize);
        if(netConfig)
        {
            // use this netConfig to initialize a network interface
            TCPIP_STACK_NetUp(hNet, netConfig);
        }
   }
  </code>
  
  
   Remarks:         The function is a helper for being able to restore the configuration data.
                    Its companion function, TCPIP_STACK_NetConfigGet, saves
                    the TCPIP_NETWORK_CONFIG to a dump buffer. 
  
 */
TCPIP_NETWORK_CONFIG*   TCPIP_STACK_NetConfigSet(void* configStoreBuff, void* netConfigBuff, size_t buffSize, size_t* pNeededSize);

/*********** IPv6 interface address functions ******************/

/*********************************************************************
   Function:        IPV6_ADDR_HANDLE TCPIP_STACK_NetIPv6AddressGet(TCPIP_NET_HANDLE netH, IPV6_ADDR_TYPE addType, IPV6_ADDR_STRUCT* pAddStruct, IPV6_ADDR_HANDLE addHandle);
  
   Summary:
    Gets network interface IPv6 address.

   Description:     This function allows the listing of the IPv6 addresses associated with an interface.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH        - handle of the interface to retrieve the addresses for
  
                    - addType     - type of address to request:
                                    IPV6_ADDR_TYPE_UNICAST and IPV6_ADDR_TYPE_MULTICAST supported for now
  
                    - pAddStruct  - structure provided by the user that will be filled
                                    with corresponding IPV6_ADDR_STRUCT data
  
                    - addHandle - an address handle that allows iteration across multiple IPv6 addresses.
                                  On the first call: has to be 0; it will begin the listing of the IPv6 addresses
                                  On subsequent calls: has to be a handle previously returned by a call to this function
  
   Returns:          - a non NULL IPV6_ADDR_HANDLE if an valid IPv6 address was found and the pAddStruct structure was filled with data
                    - 0 if no other IPv6 exists or if the supplied IPV6_ADDR_HANDLE is invalid
  
                    
  Example:
  <code>
   IPV6_ADDR_STRUCT currAddr;
   IPV6_ADDR_HANDLE currHandle;
   TCPIP_NET_HANDLE hNet = TCPIP_STACK_NetHandleGet("PIC32INT");
   char    ipv6AddBuff[44];

   currHandle = 0;
   do
   {
        currHandle = TCPIP_STACK_NetIPv6AddressGet(netH, IPV6_ADDR_TYPE_UNICAST, &currAddr, currHandle);
        if(currHandle)
        {   // have a valid address; display it
            TCPIP_HELPER_IPv6AddressToString(&currAddr.address, ipv6AddBuff, sizeof(ipv6AddBuff));
        }
   }while(currHandle != 0);
  </code>
                        
  
  
   Remarks:            None
  
 */
IPV6_ADDR_HANDLE        TCPIP_STACK_NetIPv6AddressGet(TCPIP_NET_HANDLE netH, IPV6_ADDR_TYPE addType, IPV6_ADDR_STRUCT* pAddStruct, IPV6_ADDR_HANDLE addHandle);



/************************************************************************************************************************
  Function:
    bool TCPIP_STACK_NetAddressSet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress, IPV4_ADDR* mask, bool setDefault)
  
   Summary:
    Sets network interface IPv4 address.

   Description:
    This function sets the associated network IP address and/or mask. If
    you're changing the network then it's preferred that you set both these
    values simultaneously to avoid having the stack running with a mismatch
    between its IP address and mask.   
    
  Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()

  Parameters:      - netH       - Interface handle to set address of.
                   - ipAddress  - IP address to set (could be NULL to set only the mask)
                   - mask       - corresponding network mask to set (could be NULL to set
                                  only the IP address)
                   - setDefault -  if true, the interface default address/mask is also set
  
  Return:          - true if success
                   - false if not such interface or interface is not enabled

  Example:
  <code>
    TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
    TCPIP_STACK_NetAddressSet(netH, &myIPAddress, &myIPMask, false);
  </code>

  Remarks:
    - One should use extreme caution when using these functions to change the settings
      of a running network interface.
      Changing these parameters at runtime can lead to unexpected behavior
      or loss of network connectivity.
      The preferred way to change the parameters for a running interface is to do so 
      as part of the network configuration passed at the stack initialization.
    
  ************************************************************************************************************************/
bool            TCPIP_STACK_NetAddressSet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress, IPV4_ADDR* mask, bool setDefault);

/*********************************************************************
   Function:        bool TCPIP_STACK_NetAddressGatewaySet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress)
  
   Summary:
    Sets network interface IPv4 gateway address.

   Description:
    This function sets the network interface IPv4 gateway address.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH - Interface handle to set the gateway address of.
                    - ipAddress - IP address to set
  
   Returns:         - true if success
                    - false if not such interface or interface is not enabled
                 
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   TCPIP_STACK_NetAddressGatewaySet(netH, &myIPAddress);
  </code>
  
   Remarks:  
    - One should use extreme caution when using these functions to change the settings
      of a running network interface.
      Changing these parameters at runtime can lead to unexpected behavior
      or loss of network connectivity.
      The preferred way to change the parameters for a running interface is to do so 
      as part of the network configuration passed at the stack initialization.
    
 */
bool            TCPIP_STACK_NetAddressGatewaySet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress);

/*********************************************************************
   Function:        bool TCPIP_STACK_NetAddressDnsPrimarySet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress)
  
   Summary:
    Sets network interface IPv4 DNS address.

   Description:
    This function sets the network interface primary IPv4 DNS address.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH - Interface handle to set the DNS address of.
                    - ipAddress - IP address to set
  
   Returns:         - true if success
                    - false if not such interface or interface is not enabled
                 
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   TCPIP_STACK_NetAddressDnsPrimarySet(netH, &myIPAddress);
  </code>
  
   Remarks:  
    - One should use extreme caution when using these functions to change the settings
      of a running network interface.
      Changing these parameters at runtime can lead to unexpected behavior
      or loss of network connectivity.
      The preferred way to change the parameters for a running interface is to do so 
      as part of the network configuration passed at the stack initialization.
    
 */
bool            TCPIP_STACK_NetAddressDnsPrimarySet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress);

/*********************************************************************
   Function:        bool TCPIP_STACK_NetAddressDnsSecondSet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress)
  
   Summary:
    Sets network interface IPv4 secondary DNS address.

   Description:
    This function sets the network interface secondary IPv4 DNS address.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH - Interface handle to set the secondary DNS address of.
                    - ipAddress - IP address to set
  
   Returns:         - true if success
                    - false if not such interface or interface is not enabled
                 
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   TCPIP_STACK_NetAddressDnsSecondSet(netH, &myIPAddress);
  </code>
  
   Remarks:
    - One should use extreme caution when using these functions to change the settings
      of a running network interface.
      Changing these parameters at runtime can lead to unexpected behavior
      or loss of network connectivity.
      The preferred way to change the parameters for a running interface is to do so 
      as part of the network configuration passed at the stack initialization.
    
 */
bool            TCPIP_STACK_NetAddressDnsSecondSet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress);

/*********************************************************************
   Function:        bool TCPIP_STACK_NetBiosNameSet(TCPIP_NET_HANDLE netH, const char* biosName);
  
   Summary:
    Sets network interface NetBIOS name.

   Description:
    This function sets the network interface NetBIOS name.
  
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH - Interface handle to set name of.
  
   Returns:         - true if the NetBIOS name of the interface is set 
                    - false if not such interface or interface is not enabled
  
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   TCPIP_STACK_NetBiosNameSet(netH, myBiosName);
  </code>
  
   Remarks:            See Important Remark above!
    - One should use extreme caution when using these functions to change the settings
      of a running network interface.
      Changing these parameters at runtime can lead to unexpected behavior
      or loss of network connectivity.
      The preferred way to change the parameters for a running interface is to do so 
      as part of the network configuration passed at the stack initialization.
    
 */
bool                TCPIP_STACK_NetBiosNameSet(TCPIP_NET_HANDLE netH, const char* biosName);

/*********************************************************************
   Function:        bool TCPIP_STACK_NetAddressMacSet(TCPIP_NET_HANDLE netH, const TCPIP_MAC_ADDR* pAddr)
  
   Summary:
    Sets network interface MAC address.

   Description:
    This function sets the network interface physical MAC address.
  
   Precondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
  
   Parameters:      - netH  - Interface handle to set the address of.
                    - pAddr - pointer to a valid physical (MAC) address. 
  
   Returns:         - true if the MAC address of the interface is set
                    - false if not such interface or interface is not enabled
  
  Example:
  <code>
   TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT");
   TCPIP_STACK_NetAddressMacSet(netH, &myMacAddress);
  </code>
  
   Remarks:
    - One should use extreme caution when using these functions to change the settings
      of a running network interface.
      Changing these parameters at runtime can lead to unexpected behavior
      or loss of network connectivity.
      The preferred way to change the parameters for a running interface is to do so 
      as part of the network configuration passed at the stack initialization.

    - This function updates the MAC address in the stack data structures.
      It does not re-program the MAC with the new address.
      The MAC programming is done only at MAC initialization for now.
    
 */
bool                TCPIP_STACK_NetAddressMacSet(TCPIP_NET_HANDLE netH, const TCPIP_MAC_ADDR* pAddr);



/*******************************************************************************
  Function:
    TCPIP_EVENT TCPIP_STACK_EventsPendingGet(TCPIP_NET_HANDLE hNet)

  Summary:
    Returns the currently pending events.

  Description:
    This function returns the currently pending events
    Multiple events can be ORed together as they accumulate.

  Precondition:
    None

  Parameters:
    - hNet    - network handle

  Returns:
    The currently TCPIP pending events.

  Example:
    <code>
    TCPIP_EVENT currEvents = TCPIP_STACK_EventsPendingGet( hNet);
    </code>

  Remarks:
    - This is the preferred method to get the current pending stack events.

    - Even with a notification handler in place it's better to use this function to get the current pending events

    - The returned value is just a momentary value. The pending events can change any time.
  */
TCPIP_EVENT TCPIP_STACK_EventsPendingGet(TCPIP_NET_HANDLE hNet);


/*******************************************************************************
  Function:
    TCPIP_EVENT_HANDLE    TCPIP_STACK_HandlerRegister(TCPIP_NET_HANDLE hNet, TCPIP_EVENT evMask, TCPIP_STACK_EVENT_HANDLER notifyHandler, const void* notifyfParam)

  Summary:
    Sets a new event notification handler.

  Description:
    This function sets a new event notification handler.
    The caller can use the handler to be notified of stack events.

  Precondition:
    None

  Parameters:
    - hNet            - network handle
    - evMask          - mask of events to be notified of
    - notifyHandler   - the event notification handler
    - notifyfParam    - notification handler parameter

  Returns:
    - a valid TCPIP_EVENT_HANDLE  if operation succeeded,
    - NULL otherwise

  Example:
    <code>
    TCPIP_EVENT_HANDLE myHandle = TCPIP_STACK_HandlerRegister( hNet, TCPIP_EV_CONN_ALL, myEventHandler, myParam );
    </code>

  Remarks:
    - The notification handler may be called from the ISR which detects the corresponding event.
      The event notification handler has to be kept as short as possible and non-blocking.

    - Without a notification handler the stack user can still call TCPIP_STACK_GetPending()
      to see if processing by the stack needed.

  */
TCPIP_EVENT_HANDLE    TCPIP_STACK_HandlerRegister(TCPIP_NET_HANDLE hNet, TCPIP_EVENT evMask, TCPIP_STACK_EVENT_HANDLER notifyHandler, const void* notifyfParam);

/*******************************************************************************
  Function:
    bool    TCPIP_STACK_HandlerDeregister(TCPIP_EVENT_HANDLE hEvent);

  Summary:
    De-registers an event notification handler.

  Description:
    This function removes an event notification handler.

  Precondition:
    None

  Parameters:
    - hEvent            - TCPIP event handle obtained by a call to
                          TCPIP_STACK_HandlerRegister()


  Returns:
    - true if operation succeeded,
    - false otherwise

  Example:
    <code>
    TCPIP_EVENT_HANDLE myHandle = TCPIP_STACK_HandlerRegister( hNet, TCPIP_EV_CONN_ALL, myEventHandler, myParam );
    // do something else
    // now we're done with it
    TCPIP_STACK_HandlerDeregister(myHandle);
    </code>

  Remarks:
    None

  */
bool    TCPIP_STACK_HandlerDeregister(TCPIP_EVENT_HANDLE hEvent);


/*******************************************************************************
  Function:
    TCPIP_MODULE_SIGNAL_HANDLE    TCPIP_MODULE_SignalFunctionRegister(TCPIP_STACK_MODULE moduleId, TCPIP_MODULE_SIGNAL_FUNC signalF);

  Summary:
    Registers a new signal function for a stack module.

  Description:
    This function registers a new signal function.
    This function will be called and the user notified when a stack internal signal occurs.

  Precondition:
    None

  Parameters:
    - moduleId        - module ID
    - signalF         - signal function to be called when an internal signal occurs

  Returns:
    - valid handle if operation succeeded,
    - 0/invalid handle otherwise (no such module, signal already set, for example)

  Example:
    <code>
    void appSignalFunc(TCPIP_STACK_MODULE moduleId, TCPIP_MODULE_SIGNAL signal)
    {
        // process incoming signal for the incoming module
    }

    TCPIP_MODULE_SIGNAL_HANDLE signalH = TCPIP_MODULE_SignalFunctionRegister( TCPIP_MODULE_HTTP_SERVER, appSignalFunc);
    </code>

  Remarks:
      There is no suport for multiple signal functions now.
      Each module supports just one signal function.
      A call to register a new module signal function will fail if a function is already registered.
      TCPIP_MODULE_SignalFunctionDeregister() needs to be called first

      By default all stack modules, including the stack manager (TCPIP_MODULE_MANAGER) 
      are initialized with a null signal function.
      Explicit call is needed for seting a module signal function.

      A signal handler can be registered for the stack manager itself (TCPIP_MODULE_MANAGER).
      This will report RX and TMO signals.

      The stack internal signaling mechanism is always active and cannot be disbled.
      This function is called on top of the normal stack signaling mechanism
      for a module that has a registered signal function.
  */
TCPIP_MODULE_SIGNAL_HANDLE    TCPIP_MODULE_SignalFunctionRegister(TCPIP_STACK_MODULE moduleId, TCPIP_MODULE_SIGNAL_FUNC signalF);

/*******************************************************************************
  Function:
    bool    TCPIP_MODULE_SignalFunctionDeregister(TCPIP_MODULE_SIGNAL_HANDLE signalHandle);

  Summary:
    De-registers a signal function for a stack module.

  Description:
    This function de-registers a previous signal function.
    The caller will no longer be notified of stack internal signals.

  Precondition:
    signalHandle - a valid signal handle

  Parameters:
    - signalHandle    - signal handle obtained from a registration call

  Returns:
    - true if operation succeeded,
    - false otherwise (no such module, invalid handle, etc.)

  Example:
    <code>
    void appSignalFunc(TCPIP_STACK_MODULE moduleId, TCPIP_MODULE_SIGNAL signal)
    {
        // process incoming signal for the incoming module
    }

    TCPIP_MODULE_SIGNAL_HANDLE signalH = TCPIP_MODULE_SignalFunctionRegister( TCPIP_MODULE_HTTP_SERVER, appSignalFunc);

    // when done with this signal notification
    TCPIP_MODULE_SignalFunctionDeregister(signalH);
    </code>

  Remarks:
      See the remarks for TCPIP_MODULE_SignalFunctionRegister()

  */
bool    TCPIP_MODULE_SignalFunctionDeregister(TCPIP_MODULE_SIGNAL_HANDLE signalHandle);

/*******************************************************************************
  Function:
    TCPIP_MODULE_SIGNAL    TCPIP_MODULE_SignalGet(TCPIP_STACK_MODULE moduleId);

  Summary:
    Returns the current signals for a TCP/IP module.

  Description:
    This function provides a read only value of the current signals for a stack module.

  Precondition:
    None

  Parameters:
    - moduleId        - module ID

  Returns:
    - current value of the module's signals

  Example:
    <code>
    TCPIP_MODULE_SIGNAL currSIgnal = TCPIP_MODULE_SignalGet(TCPIP_MODULE_HTTP_SERVER);
    </code>

  Remarks:
    The signals are processed and cleared inside the stack.
    It is not possible to clear a signal that's pending using this API.

  */
TCPIP_MODULE_SIGNAL    TCPIP_MODULE_SignalGet(TCPIP_STACK_MODULE moduleId);

// *****************************************************************************
// *****************************************************************************
// Section: Version Information
// *****************************************************************************
// *****************************************************************************
/* These functions return the version information of the TCP/IP stack */

// *****************************************************************************
/* Function:
    unsigned int TCPIP_STACK_VersionGet ( const SYS_MODULE_INDEX index )

  Summary:
    Gets the TCP/IP stack version in numerical format.

  Description:
    This function gets the TCP/IP stack version. The version is encoded as
    major * 10000 + minor * 100 + patch. The stringed version can be obtained
    using TCPIP_STACK_VersionStrGet()

  Precondition:
    None.

  Parameters:
    - index    - Identifier for the object instance to get the version for

  Returns:
    Current driver version in numerical format.

  Example:
    <code>
    unsigned int version;

    version = TCPIP_STACK_VersionGet( TCPIP_STACK_INDEX_1 );

    if(version < 110200)
    {
        // Do Something
    }
    </code>

  Remarks:
    None.
*/

unsigned int TCPIP_STACK_VersionGet ( const SYS_MODULE_INDEX index );


// *****************************************************************************
/* Function:
    char * TCPIP_STACK_VersionStrGet ( const SYS_MODULE_INDEX index )

  Summary:
    Gets the TCP/IP stack version in string format.

  Description:
    This function gets the TCP/IP stack version. The version is returned as
    major.minor.path[type], where type is optional. The numerical version can
    be obtained using TCPIP_STACK_VersionGet()

  Precondition:
    None.

  Parameters:
    index    - Identifier for the object instance to get the version for.

  Returns:
    Current TCP/IP stack version in the string format.

  Example:
    <code>
    </code>

  Remarks:
    None.
*/

char * TCPIP_STACK_VersionStrGet ( const SYS_MODULE_INDEX index );



//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


#endif  // __TCPIP_MANAGER_H_



