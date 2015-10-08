/*******************************************************************************
  Domain Name System (DNS) server Header file

  Company:
    Microchip Technology Inc.
  
  File Name:
    dnss.h

  Summary:
    DNS server definitions and interface file

  Description:
    This source file contains the DNS Server module API
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
File Name:  dnss.h 
Copyright © 2012 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
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
#ifndef __DNSS_H
#define __DNSS_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
/* 
  Structure:
    TCPIP_DNSS_MODULE_CONFIG

  Summary:
    Provides a place holder for DNS server configuration.

  Description:
    DNS Server run-time configuration and initialization parameter.

  Remarks:
    None.
*/
typedef struct
{
    bool    deleteOldLease;  		// Delete old cache if still in place,
    bool    replyBoardAddr;  		// Reply with board address
    // specific DNS server parameters
    size_t  IPv4EntriesPerDNSName;  // Number of IPv4 entries per DNS name. Default value is 1.
    size_t  IPv6EntriesPerDNSName;  // Number of IPv6 address per DNS Name. Default value is 1
                                    // Used only when IPv6 is enabled
                                
}TCPIP_DNSS_MODULE_CONFIG;


// *****************************************************************************
// *****************************************************************************
// Section: DNS server Routines
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/*
  Function:
    TCPIP_DNS_RESULT TCPIP_DNSS_CacheEntryRemove(const char* name, IP_ADDRESS_TYPE type, IP_MULTI_ADDRESS* pAdd)

  Summary:
    Remove DNS server cache entry by Hostname and IP Type and IP(v4 or v6) address

  Description:
    This API is used for command prompt dnsDelSrv and to delete a entry IPv4 or IPv6 from
    hostName.Every time this is used to delete either one IPv4 or IPv6 address
    with respect to DNS hostname. There will be a expire time for a Host entry.

  Precondition:
    DNS server initialized.

  Parameters:
     name- Hostname
     type -  IP_ADDRESS_TYPE_IPV4 or IP_ADDRESS_TYPE_IPV6
     pAdd - v4 or v6 address

  Returns:
    - DNS_RES_OK - If entry remove is successful
    - DNS_RES_NO_ENTRY - No such entry is present
    - DNS_RES_MEMORY_FAIL - No Memory is present for IPv4 or IPv6 address type
*/
TCPIP_DNS_RESULT TCPIP_DNSS_CacheEntryRemove(const char* name, IP_ADDRESS_TYPE type, IP_MULTI_ADDRESS* pAdd);


//*****************************************************************************
/*
  Function:
    bool TCPIP_DNSS_IsEnabled(CPIP_NET_HANDLE hNet)

  Summary:
    Determine if the DNS Server is enabled on the specified interface.

  Description:
    Returns the current state of the DNS Server on the specified interface.

  Precondition:
    DNS server initialized.

  Parameters:
     hNet- Interface to query.

  Returns:
    - true	- if the DNS Server is enabled on the specified interface
    - false	- if the DNS Server is not enabled on the specified interface
*/
bool TCPIP_DNSS_IsEnabled(TCPIP_NET_HANDLE hNet);

//*****************************************************************************
/*
  Function:
    bool TCPIP_DNSS_Enable(TCPIP_NET_HANDLE hNet)

  Summary:
    Enables the DNS server for the specified interface.

  Description:
    Enables the DNS Server for the specified interface, if it is disabled.
    If it is already enabled, nothing is done.

  Precondition:
    DNS server initialized.

  Parameters:
     hNet - Interface to enable the DNS Server on.

  Returns:
    - true - if success
    - false - if unsuccessful
*/
bool TCPIP_DNSS_Enable(TCPIP_NET_HANDLE hNet);

//*****************************************************************************
/*
  Function:
    bool TCPIP_DNSS_Disable(TCPIP_NET_HANDLE hNet)

  Summary:
    Disables the DNS Server for the specified interface.

  Description:
    Disables the DNS Server for the specified interface.
    If it is already disabled, no action is taken.

  Precondition:
    DNS server initialized.

  Parameters:
    hNet - Interface to disable the DNS Server off.

  Returns:
    - true - if successful
    - false - if unsuccessful

  Remarks:
    none
*/
bool TCPIP_DNSS_Disable(TCPIP_NET_HANDLE hNet);

/*****************************************************************************
  Function:
    TCPIP_DNS_RESULT TCPIP_DNSS_EntryAdd(const char* name, IP_ADDRESS_TYPE type, IP_MULTI_ADDRESS* pAdd,uint32_t validStartTime)

  Summary:
    Add a IPv4 or IPv6 entry to the DNS server table.

  Description:
    This API is used to add a IPv4 or IPv6 entry.
    Every time this is used to add either one IPv4 or IPv6 address
    with respect to DNS hostname. There will be a expire time for a Host entry.

  Precondition:
    DNS Server Initialized.

  Parameters:
    name- Hostname
    type -	IP_ADDRESS_TYPE_IPV4 or IP_ADDRESS_TYPE_IPV6
    pAdd - IPv4 or IPv6 address
    validStartTime - Timeout value for the entry
	
  Returns:
    - DNS_RES_OK - If entry remove is successful
    - DNS_RES_NO_ENTRY - If Hostname is NULL value and Invalid IP type
    - DNS_RES_MEMORY_FAIL - No Memory is present for IPv4 or IPv6 address type
    - DNS_RES_CACHE_FULL - If there is no space for the new entry

  Remarks:
    none
***************************************************************************/

TCPIP_DNS_RESULT TCPIP_DNSS_EntryAdd(const char* name, IP_ADDRESS_TYPE type, IP_MULTI_ADDRESS* pAdd,uint32_t validStartTime);


/*****************************************************************************
  Function:
    TCPIP_DNS_RESULT TCPIP_DNSS_EntryGet(uint8_t * hostName,IP_ADDRESS_TYPE type,int index,IP_MULTI_ADDRESS* pGetAdd,uint32_t *ttltime);

  Summary:
    Get DNS server  IP address details from resolver pool entry

  Description:
    This API is used to get the DNS server IPv4 or IPv6 address from Resolver pool entry as per hostname and IP type and
    from index value. This IP type can be a IPv4 and IPv6 type. This is used for DNS record type.
	
  Precondition:
    DNSServerInit should be called

  Parameters:
    hostName - DNS host name
    type -  DNS IP type ( it will be ipv4 or ipv6 type and it is mapped to DNS record type)
    this can be used for DNS record type
    index - get the next entry after this index value
    pGetAdd - get the zero th IPv4 or IPv6 address
    ttltime - timeout value
  Returns:
    TCPIP_DNS_RESULT
  	
  Remarks:
    none
  ***************************************************************************/
TCPIP_DNS_RESULT TCPIP_DNSS_EntryGet(uint8_t * hostName,IP_ADDRESS_TYPE type,int index,IP_MULTI_ADDRESS* pGetAdd,uint32_t *ttltime);

/*****************************************************************************
  Function:
    TCPIP_DNS_RESULT TCPIP_DNSS_AddressCntGet(uint8_t * hostName,uint8_t * ipCount,int index)

  Summary:
    Get DNS Server IPv4 or IPv4 Address count details for the Input value of Index

  Description:
    This API is used to get the number of IPv4 and IPv6 address entry details
    from hash entry table. Here input parameter is index value. Output will be the hostname and IP address count.
    IP address count is the summation of both IPv4 and IPv6 address.
 
  Precondition:
    DNSServerInit should be called

  Parameters:
    hostName - get DNS host name
    ipCount - get the number of IPv4 or IPv6 Server address
    index -  Server count details per index value
	
  Returns:
    TCPIP_DNS_RESULT
  	
  Remarks:
	none
  ***************************************************************************/
TCPIP_DNS_RESULT TCPIP_DNSS_AddressCntGet(int index,uint8_t * hostName,uint8_t * ipCount);


// *****************************************************************************
/*
  Function:
    void  TCPIP_DNSS_Task(void)

  Summary:
    Standard TCP/IP stack module task function.

  Description:
    Performs DNSS module tasks in the TCP/IP stack.

  Precondition:
    DNSS module should have been initialized

  Parameters:
    None.

  Returns:
    None.

  Example:
    None

  Remarks:
    None.
*/
void  TCPIP_DNSS_Task(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif  // __DNSS_H

