/*******************************************************************************
Microchip TCP/IP Stack Demo Application Configuration Header

  Company:
    Microchip Technology Inc.
    
  File Name:
    tcpip_config.h

  Summary:
    TCP/IP Commands configuration file
    
  Description:
    This file contains the TCP/IP commands configuration options

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

SOFTWARE AND DOCUMENTATION ARE PROVIDED ?AS IS?WITHOUT WARRANTY OF ANY KIND,
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
#ifndef __TCPIP_CONFIG_H_
#define __TCPIP_CONFIG_H_

// =======================================================================
//   Stack Configuration/Build Options
// =======================================================================

// TCPIP Stack Module Selection
//   Uncomment or comment the following lines to enable or
//   disabled the following high-level application modules.

#define TCPIP_STACK_USE_IPV4                // enable IPv4 functionality
#define TCPIP_STACK_USE_ICMP_SERVER         // Ping query and response capability
#define TCPIP_STACK_USE_HTTP_SERVER        // HTTP server with POST, Cookies, Authentication, etc.
#define TCPIP_STACK_USE_DHCP_CLIENT         // Dynamic Host Configuration Protocol client for obtaining IP address and other parameters
//#define TCPIP_STACK_USE_SMTP_CLIENT       // Simple Mail Transfer Protocol for sending email
#define TCPIP_STACK_USE_TELNET_SERVER       // Telnet server
#define TCPIP_STACK_USE_ANNOUNCE            // Microchip Embedded Ethernet Device Discoverer server/client
#define TCPIP_STACK_USE_DNS                 // Domain Name Service Client for resolving hostname strings to IP addresses
#define TCPIP_STACK_USE_DNS_SERVER          // Domain Name Service Server for redirection to the local device
#define TCPIP_STACK_USE_NBNS                // NetBIOS Name Service Server for responding to NBNS hostname broadcast queries
//#define TCPIP_STACK_USE_REBOOT_SERVER     // Module for resetting this PIC remotely.  Primarily useful for a Bootloader.
#define TCPIP_STACK_USE_SNTP_CLIENT         // Simple Network Time Protocol for obtaining current date/time from Internet
//#define TCPIP_STACK_USE_DYNAMICDNS_CLIENT  // Dynamic DNS client updater module
//#define TCPIP_STACK_USE_BERKELEY_API       // Berkeley Sockets APIs are available
#define TCPIP_STACK_USE_IPV6                // enable IPv6 functionality
#define TCPIP_STACK_USE_TCP                 // Enable the TCP module
#define TCPIP_STACK_USE_UDP                 // Enable the UDP module
#define TCPIP_STACK_USE_ZEROCONF_LINK_LOCAL // Zeroconf IPv4 Link-Local Addressing;
#define TCPIP_STACK_USE_ZEROCONF_MDNS_SD    // Zeroconf mDNS and mDNS service discovery

#define TCPIP_STACK_COMMAND_ENABLE          // TCPIP_COMMANDS for network configuration or debug
#define TCPIP_STACK_USE_IPERF               // Enable the Iperf module for standard network benchmarking
//#define TCPIP_STACK_USE_SNMP_SERVER        // Simple Network Management Protocol v2C Community Agent
//#define TCPIP_STACK_USE_SNMPV3_SERVER      // SNMP v3 agent
#define TCPIP_STACK_USE_FTP_SERVER          // File Transfer Protocol
#define TCPIP_STACK_USE_DHCP_SERVER         // DHCP server

// =======================================================================
//   Dynamic memory allocation Options
// =======================================================================
// This is the total amount of dynamic memory heap that the TCPIP stack will create
// at start up.
// This is a private heap that will be used only by internal stack modules that need dynamic memory
// (and most of them do): MAC buffers/packets, TCP and UDP buffers, etc.
// Note that the settings in other modules configuration parameters
// (especially the MAC driver, TCP, UDP, HTTP, etc.) should be consistent
// with the total amount of heap that's allocated to the stack.
// Note that this parameter should also be consistent with the project linker
// heap setting ( TCPIP_STACK_DRAM_SIZE < project heap)
#define TCPIP_STACK_DRAM_SIZE             (39250)

// The minimum amount of dynamic memory left for run time allocation by the stack (IP, UDP, etc)
// This is just a warning threshold.
// If after all the modules are initialized the amount of memory available in the TCPIP heap
// is less then TCPIP_STACK_DRAM_RUN_LIMIT then a warning will be displayed
// (if the debug channel is enabled)
// For proper operation there should be always a heap reserve of at least few KB.
#define TCPIP_STACK_DRAM_RUN_LIMIT          (2048)

// Enable debugging of an allocation call that failed.
// If the system debug is enabled (SYS_DEBUG_ENABLE) the stack will issue a
// warning message at the system debug channel.
#define TCPIP_STACK_DRAM_DEBUG_ENABLE

    // Enable tracing of the allocated memory by each module.
    // The stack will trace all the memory allocated by a module
    // and various statistics.
    //#define TCPIP_STACK_DRAM_TRACE_ENABLE
    // Number of trace slots to be used.
    // There is on slot needed per module that allocates memory from the heap.
    #define TCPIP_STACK_DRAM_TRACE_SLOTS    12

// Stack allocation function, malloc style
// This is the function the stack will call to allocate memory
// needed for its own heap: TCPIP_STACK_DRAM_SIZE.
// Use standard C library 'malloc' as a default
#define TCPIP_STACK_MALLOC_FUNC         malloc

// Stack allocation function, calloc style
// This is the function the stack will call to allocate memory
// needed for its own heap: TCPIP_STACK_DRAM_SIZE.
// Use standard C library 'calloc' as a default
#define TCPIP_STACK_CALLOC_FUNC         calloc

// Stack deallocation function, free style
// This is the function the stack will call for freeing the allocated memory
// when the stack is deinitialized.
// Use standard C library 'free' as a default
#define TCPIP_STACK_FREE_FUNC         free



// =======================================================================
//   Event Notifications Options
// =======================================================================
// This setting enables the stack event notification.
// It allows the MAC drivers to report their TX/RX related events to the stack manager
// but also allows users of the stack to register notification handler so that
// they are notified of specific events.
// The operation of the stack is more efficient when event notification is turned on
// and this is how the stack is designed to run.
// Always leave this setting defined.
// The choice for selecting this parameter will eventually be removed.
// Maintained for backward compatibility. 
#define TCPIP_STACK_USE_EVENT_NOTIFICATION


// =======================================================================
//   Stack Modules Execution Options
// =======================================================================
// This setting enables the selection of how the stack module task functions get executed.
//
// If the symbol is not defined then the stack manager calls itself all the
//  modules Task functions as required when executing the TCPIP_STACK_Task() function.
// This is the default model for now.
//
// If this symbol is defined, then the application is responsible for calling the Task
// functions for all the TCP/IP stack modules.
// This is the preferred model especially when running under an RTOS environment
// and the app can adjust the priority levels for the execution of each task.
//
// Note: the TCPIP_STACK_Task is always called at the application level.
//
//#define TCPIP_STACK_APP_EXECUTE_MODULE_TASKS

#endif  // __TCPIP_CONFIG_H_


