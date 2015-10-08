/*******************************************************************************
  DNS module private header

  Summary:
    Header definitions file for DNS module
    
  Description:
    This file contains the private definitions for the DNS module
*******************************************************************************/

/*******************************************************************************
File Name:   dns_private.h
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

#ifndef _DNS_PRIVATE_H_ 
#define _DNS_PRIVATE_H_


#define DNS_HOSTNAME_MAX_SIZE  255
#define     DNS_HASH_PROBE_STEP      1    // step to advance for hash collision

// This one is uesd for UDPGET and UDPPUT
typedef struct
{
    uint8_t *head;      // probably this is the allocated structure
    uint8_t* wrPtr;     // current write pointer, initialized to head when buffer allocated
    uint8_t* endPtr;    // end pointer, initialized to head + len when buffer allocated
}TCPIP_DNS_DATA;

// Semaphore flags for the DNS module
typedef union
{
	uint8_t Val;
	struct
	{
		unsigned char DNSInUse 		: 1;	// Indicates the DNS module is in use
		unsigned char AddressValid	: 1;	// Indicates that the address resolution is valid and complete
		unsigned char AddressType 	: 2;    // IP_ADDRESS_TYPE_IPV6 or IP_ADDRESS_TYPE_IPV4
        unsigned char filler        : 4;
	} bits;
} DNS_CLIENT_FLAGS;


// DNS ENTRY flags used in hEntry->flags
// note that only the hEntry->flags.user fields
// should be used! 
typedef enum
{
    DNS_FLAG_ENTRY_BUSY         = 0x0001,          // this is used by the hash itself!
    // user flags
    DNS_FLAG_ENTRY_INCOMPLETE   = 0x0040,          // entry is not completed yet
    DNS_FLAG_ENTRY_COMPLETE     = 0x0080,          // regular entry, complete
                                                   // else it's incomplete
                                                   //
    DNS_FLAG_ENTRY_VALID_MASK   = (DNS_FLAG_ENTRY_INCOMPLETE | DNS_FLAG_ENTRY_COMPLETE )                                                     
                                                  
}DNS_HASH_ENTRY_FLAGS;

// State machine for a DNS query
typedef enum
{
	DNS_IDLE = 0, 				// Initial state to reset client state variables
    // running, transient state
	DNS_START, 				    // resolution process started
	DNS_START_SERVICE,			// Start DNS service
	DNS_QUERY,					// Send DNS query to DNS server
	DNS_GET_RESULT,				// Wait for response from DNS server
	DNS_FAIL_SERVER,			// DNS server not responding
    // success state
	DNS_DONE,					// DNS query is finished OK

	DNS_FAIL_SERVER_TMO,		// DNS server TMO
	DNS_FAIL_HASH_NO_ENTRY,      // DNS fail due to no empty entry within hash table
	DNS_FAIL_NO_MEM,  // DNS fail with malloc fail
	DNS_FAIL_WRONG_TRANSACTIONID, 
}DNS_STATE;


// DNS unaligned key
// the IP address field is unaligned
// in DNS 
typedef struct __attribute__((packed))
{
    uint32_t    v;
}DNS_UNALIGNED_KEY;

// This is useful for Hash Key . Hostname is dynamic parameter and size is required for hash bucket calculation
//typedef struct
//{
//    char *hostname;
//    int size;
//}DNS_HOSTNAME;

    
// DNS Resolver cache entry
typedef struct	_TAG_DNS_HASH_ENTRY 
{
    OA_HASH_ENTRY		hEntry; 		// hash header;
    uint8_t*                    memblk;         // memory block for IPv4 and IPv6 and TTL block of memory. Hostname is not part of this block
    uint32_t                    tInsert;        // one time per hash entry
    uint32_t                    tRetry;         // retry time per hash entry
    int                         nIPv4Entries;     // how many entries in the ip4Address[] array; if IPv4 is defined
    IPV4_ADDR*                  pip4Address;  // pointer to an array of IPv4 entries 
    TCPIP_UINT32_VAL            ipTTL;      // Minimum TTL per IPv4 and Ipv6 addresses
    int                         nIPv6Entries;     // how many entries in the ip6Address[] array; if IPv6 is defined
    IPV6_ADDR*                  pip6Address;  // pointer to an array of IPv6 entries
    //TCPIP_UINT32_VAL*           pipv6TTL; // TTL per IPv6 address
    TCPIP_UINT16_VAL            SentTransactionID;
    DNS_RESOLVE_TYPE		resolve_type;
    uint8_t                     *pHostName;
    uint8_t			recordType;  // record can be  IPv6 or IPv4 
    uint8_t                     netIfIdx;
    
}DNS_HASH_ENTRY;

// each DNS Lease entry consists of
typedef struct
{
    OA_HASH_DCPT*       hashDcpt;       // contiguous space for a hash descriptor  and hash table entries    
}DNS_HASH_DCPT;

typedef struct
{
    DNS_HASH_DCPT	dnsCacheDcpt;
    UDP_SOCKET      dnsSocket;               // Socket used by DHCP Server
    DNS_STATE		smState;           // server state machine status
    uint32_t 		stateStartTime;
    TCPIP_NET_IF*	  pDNSNet;						  // current interface the DNS is working on
    IP_MULTI_ADDRESS  DNSServers[TCPIP_DNS_MAX_SERVERS_SUPPORTED];				  // servers associated with the current interface
    int				vDNSServerIx;					// current server used
    uint32_t            dnsTimeSeconds;         // coarse DNS time keeping, seconds
    tcpipSignalHandle dnsSignalHandle;
    const void 		 *memH;
    bool	enabled;			// Whether or not the DHCP server is enabled
 //   DNS_CLIENT_FLAGS    flags;
    int                 cacheEntryTMO;
    int                dnscacheEntries; 
    IP_ADDRESS_TYPE     dnsIpAddressType;
    int                 IPv4EntriesPerDNSName;
    int                 IPv6EntriesPerDNSName;
   
}DNS_DCPT;    // DNS descriptor



// Structure for the DNS header
typedef struct
{
    TCPIP_UINT16_VAL TransactionID;
    TCPIP_UINT16_VAL Flags;
    TCPIP_UINT16_VAL Questions;
    TCPIP_UINT16_VAL Answers;
    TCPIP_UINT16_VAL AuthoritativeRecords;
    TCPIP_UINT16_VAL AdditionalRecords;
} DNS_HEADER;

// DNS response header for ANSWER, Authorative and Additional packet response header
typedef struct __attribute__((packed))
{
    // Response name is first, but it is variable length and must be retrieved
    // using the DNSDiscardName() function
    TCPIP_UINT16_VAL	ResponseType; // Response Type
    TCPIP_UINT16_VAL	ResponseClass; // Response Class
    TCPIP_UINT32_VAL	ResponseTTL; //Time to live
    TCPIP_UINT16_VAL	ResponseLen; // Response length
} DNS_ANSWER_HEADER;

// DNS event registration

typedef struct  _TAG_DNS_LIST_NODE
{
    struct _TAG_DNS_LIST_NODE*      next;		// next node in list
                                                // makes it valid SGL_LIST_NODE node
    TCPIP_DNS_EVENT_HANDLER         handler;    // handler to be called for event
    const void*                     hParam;     // handler parameter
    TCPIP_NET_HANDLE                hNet;       // interface that's registered for
                                                // 0 if all    
}DNS_LIST_NODE;



static size_t TCPIP_DNS_OAHASH_KeyHash(OA_HASH_DCPT* pOH, void* key);
static OA_HASH_ENTRY* TCPIP_DNS_OAHASH_DeleteEntry(OA_HASH_DCPT* pOH);
static int TCPIP_DNS_OAHASH_KeyCompare(OA_HASH_DCPT* pOH, OA_HASH_ENTRY* hEntry, void* key);
static void TCPIP_DNS_OAHASH_KeyCopy(OA_HASH_DCPT* pOH, OA_HASH_ENTRY* dstEntry, void* key);
#if defined(OA_DOUBLE_HASH_PROBING)
static size_t TCPIP_DNS_OAHASH_ProbeHash(OA_HASH_DCPT* pOH, void* key);
#endif  // defined(OA_DOUBLE_HASH_PROBING)




#endif  /* _DNS_PRIVATE_H_ */

