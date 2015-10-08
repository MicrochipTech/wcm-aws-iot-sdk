/*******************************************************************************
  Microchip TCP/IP Stack Include File

  Company:
    Microchip Technology Inc.
    
  File Name:
    tcpip_common_ports.h

  Summary:
    List of the common UDP and TCP port numbers that are used
    in the stack implementation.


  Description:
    This is the list of the well-known IANA allocated ports
    that are used in the current stack implementation.

    See RFC 6335 and https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml
    for details.

    Also some private port numbers (and ranges) used by the stack modules
    are listed in here.
    Update the values as needed.

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

#ifndef __TCPIP_COMMON_PORTS_H__
#define __TCPIP_COMMON_PORTS_H__

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
// *****************************************************************************
// UDP dynamic port numbers
// *****************************************************************************
// *****************************************************************************

// The dynamic port range defined by IANA consists of the 49152-65535 range
// and is meant for the selection of ephemeral ports (RFC 6056).
// Adjust to your needs but stay within the IANA range 
// First port number for randomized local port number selection
#define TCPIP_UDP_LOCAL_PORT_START_NUMBER         (49152)
// Last port number for randomized local port number selection
#define TCPIP_UDP_LOCAL_PORT_END_NUMBER           (65535)



// *****************************************************************************
// *****************************************************************************
// TCP dynamic port numbers
// *****************************************************************************
// *****************************************************************************


// First port number for randomized local port number selection
// Use the dynamic port range defined by IANA consists of the 49152-65535 range
// and is meant for the selection of ephemeral ports (RFC 6056).
// Adjust to your needs but stay within the IANA range 
#define TCPIP_TCP_LOCAL_PORT_START_NUMBER         (49152)

// Last port number for randomized local port number selection
#define TCPIP_TCP_LOCAL_PORT_END_NUMBER           (65535)


// *****************************************************************************
// *****************************************************************************
// Stack modules port numbers
// *****************************************************************************
// *****************************************************************************


// ************************************************************
// HTTP server listening TCP port
#define TCPIP_HTTP_SERVER_PORT                    (80)

// HTTPS server listening TCP port
#define TCPIP_HTTP_SERVER_SECURE_PORT             (443)


// ************************************************************
// Unsecured Telnet server TCP port
#define TCPIP_TELNET_SERVER_PORT                 (23)

// Secured Telnet server TCP port
#define TCPIP_TELNET_SERVER_SECURE_PORT         (992)	


// ************************************************************
// DHCP Client transactions UDP local port
#define TCPIP_DHCP_CLIENT_PORT                    (68)

// DHCP Server UDP remote port
#define TCPIP_DHCP_SERVER_PORT                    (67)



// ************************************************************
// DNS client UDP port
#define TCPIP_DNS_CLIENT_PORT                     (53)

// DNS server UDP listening port
#define TCPIP_DNS_SERVER_PORT                     (53)


// ************************************************************
// Dynamic DNS TCP remote port
#define TCPIP_DDNS_SERVER_REMOTE_PORT             (80)



// ************************************************************
// NetBIOS Name UDP Service (server) port
#define TCPIP_NBNS_SERVER_PORT                    (137)

// ************************************************************
// SMTP server remote TCP port 
#define TCPIP_SMTP_SERVER_REMOTE_PORT              (25)

// ************************************************************
// NTP server remote TCP port
#define TCPIP_NTP_SERVER_REMOTE_PORT	            (123)


// ************************************************************
// Reboot server listening UDP port
#define TCPIP_REBOOT_SERVER_PORT                    (69)


// ************************************************************
// MCHP Announce service UDP port (server and client)
#define TCPIP_ANNOUNCE_PORT	                    (30303)


// ************************************************************
// Iperf server listening port (TCP and UDP)
#define TCPIP_IPERF_SERVER_PORT                   (5001)

// Iperf client local TCP port
#define TCPIP_IPERF_TCP_LOCAL_PORT_START_NUMBER   (1024)

// ************************************************************
// TFTP Client local UDP port
#define TCPIP_TFTP_CLIENT_PORT                    (65352)

// TFTP Server remote UDP Port
#define TCPIP_TFTP_SERVER_PORT                    (69)



//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


#endif  // __TCPIP_COMMON_PORTS_H__

