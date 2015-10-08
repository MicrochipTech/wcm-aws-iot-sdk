/*******************************************************************************
  Zero Configuration (Zeroconf) Multicast DNS and 
  Service Discovery Module for Microchip TCP/IP Stack

  Company:
    Microchip Technology Inc.
    
  File Name:
    zero_conf_multicast_dns.h

  Summary:
    Zero configuration (Zeroconf), provides a mechanism to ease the configuration
    of a device on a network.

  Description:
    Zero configuration (Zeroconf), provides a mechanism to ease the configuration 
    of a device on a network. It also provides for a more human-like naming 
    convention, instead of relying on IP addresses alone. Zeroconf also goes by 
    the names Bonjour (Apple) and Avahi (Linux), and is an IETF standard.
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

#ifndef __ZEROCONF_MULTICAST_DNS_H
#define __ZEROCONF_MULTICAST_DNS_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  


// --------------------------------------------------------------
// Function prototypes and data structures
//


//void DisplayHostName(uint8_t *HostName);

typedef enum {
    MDNSD_SUCCESS =  0,
    MDNSD_ERR_BUSY =     1, /* Already Being used for another Service */
    MDNSD_ERR_CONFLICT = 2, /* Name Conflict */
    MDNSD_ERR_INVAL =    3, /* Invalid Parameter */
} MDNSD_ERR_CODE;

/* DNS-Service Discovery APIs */
/***************************************************************
  Function:
    MDNSD_ERR_CODE TCPIP_MDNS_ServiceRegister( ...)

  Summary:
   DNS-Service Discovery API for end-users to register a
   service-advertisement. The service is associated with all interfaces.

  Description:
   This API is used by end-user application to announce its
    service on local network. All peer machines that are compliant
    with Multicast-DNS & DNS-Service Discovery protocol can detect
    the announcement and lists out an entry in Service-Browser list.
    End-User selects an entry to connect to this service. So
    ultimately this is an aid to end-user to discover any service
    of interest on a local network.

    This is the first API that needs to be invoked by end-user
    application. Presently  Multicast-DNS & Service-discovery
    stack supports only single service-advertisement. Once the
    application wants to terminate the service it has to invoke
    TCPIP_MDNS_ServiceDeregister() API to free-up the DNS-SD stack for
    some other application.

  Precondition:
   None

  Parameters:
    netH      - handle of the network to be registered
    srv_name  - Service Name, which is being advertised
    srv_type  - For a HTTP-Service its "_http._tcp.local"
                _http: is application protocol proceeded with
                     under-score
                _tcp: is lower-layer protocol on which service runs
                local: is to represent service is on local-network

                For a iTunes Music Sharing "_daap._tcp.local"
                For a Printing Service "_ipp._tcp.local"
                Refer to  http://www.dns-sd.org/ServiceTypes.html
                for more service types

    port      - Port number on which service is running

    txt_len   - For additional information about service like
                default page (e.g., "index.htm") for HTTP-service.
                Length of such additional information
   txt_record - String of additional information (e.g., "index.htm")
                for HTTP-service.

   auto_rename- A flag to indicate DNS-SD stack, whether to rename
                the service automatically or not.
                If this is set to '0' Callback parameter will be used
                to indicate the conflict error and user has to select
                different name and re-register with this API.
                If this is set to '1' service-name will be automatically
                renamed with numerical suffix.

    callback  - Callback function, which is user-application defined.
                This callback gets invoked on completion of service-
                advertisement. If an service name-conflict error is
                detected and auto_rename is set to '0' callback gets
                invoked with MDNSD_ERR_CONFLICT as error-code.

    context   - Opaque context (pointer to opaque data), which needs
                to be used in callback function.

  Returns:
     MDNSD_ERR_CODE - Returns Error-code to indicate registration is
                     success or not.
      1) MDNSD_SUCCESS  - returns on success of call
      2) MDNSD_ERR_BUSY - When already some other service is being
                          advertised using this DNS-SD stack
      3) MDNSD_ERR_INVAL - Invalid Parameter
  **************************************************************/
MDNSD_ERR_CODE
TCPIP_MDNS_ServiceRegister( TCPIP_NET_HANDLE netH
                    ,const char *srv_name
                    ,const char *srv_type
                    ,uint16_t port
                    ,const uint8_t *txt_record
                    ,uint8_t auto_rename
                    ,void (*call_back)(char *name, MDNSD_ERR_CODE err, void *context)
                    ,void *context);

/***************************************************************
  Function:
    MDNSD_ERR_CODE TCPIP_MDNS_ServiceDeregister()

  Summary:
   DNS-Service Discovery API for end-user to De-register a
    service-advertisement, which was previously registered with
    TCPIP_MDNS_ServiceRegister API.

  Description:
   This API is used by end-user application to deregister
    DNS-Service Discovery on a local network. When this gets invoked
    by end-user DNS-SD stack sends out Good-Bye packets to update
    all peer machines that service will no longer be present. All
    peer machines remove the corresponding entry from Browser list.

    This is the last API that needs to be invoked by end-user
    application to free-up the DNS-SD stack for some other app.

  Precondition:
   TCPIP_MDNS_ServiceRegister must be invoked before this call.

  Parameters:
    netH - handle of the network to be deregistered

  Returns:
     MDNSD_ERR_CODE - Returns Error-code to indicate registration is
                     success or not.
      1) MDNSD_SUCCESS  - returns on success of call
      2) MDNSD_ERR_INVAL - When the input parameters are invalid or
                           if the API is invoked in invalid state
  **************************************************************/
MDNSD_ERR_CODE TCPIP_MDNS_ServiceDeregister(TCPIP_NET_HANDLE netH);


/***************************************************************
  Function:
    MDNSD_ERR_CODE TCPIP_MDNS_ServiceUpdate(
        TCPIP_NET_HANDLE netH,
        uint16_t port,
        const uint8_t *txt_record)

  Summary:
   DNS-Service Discovery API for end-user to update the service
    -advertisement, which was previously registered with
    TCPIP_MDNS_ServiceRegister

  Description:
   This API is used by end-user application to update its service
    which was previously registered. With this API end-user app
    update the port number on which the service is running. It can
    update the additional information of service. For example: the
    default page can be updated to new page and corresponding page
    name can be input to this API to update all peer machines. The
    modified service will be announced with new contents on local
    network.

    This is an optional API and should be invoked only if it is
    necessary.

  Precondition:
   TCPIP_MDNS_ServiceRegister must be invoked before this call.

  Parameters:
    netH      - handle of the network to perform the service update
    port      - Port number on which service is running
    txt_record - String of additional information (e.g., "index.htm")
                for HTTP-service.

  Returns:
     MDNSD_ERR_CODE - Returns Error-code to indicate registration is
                     success or not.
      1) MDNSD_SUCCESS  - returns on success of call
      2) MDNSD_ERR_INVAL - When the input parameters are invalid or
                           if the API is invoked in invalid state
  **************************************************************/
MDNSD_ERR_CODE TCPIP_MDNS_ServiceUpdate(
        TCPIP_NET_HANDLE netH,
        uint16_t port,
        const uint8_t *txt_record);


// *****************************************************************************
/*
  Function:
    void  TCPIP_MDNS_Task(void)

  Summary:
    Standard TCP/IP stack module task function.

  Description:
    Performs MDNS module tasks in the TCP/IP stack.

  Precondition:
    MDNS module should have been initialized

  Parameters:
    None.

  Returns:
    None.

  Example:
    None

  Remarks:
    None.
*/
void  TCPIP_MDNS_Task(void);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif  // __ZEROCONF_MULTICAST_DNS_H


