/*******************************************************************************
  Domain Name System (DNS) Client 
  Module for Microchip TCP/IP Stack

  Summary:
    DNS client implementation file
    
  Description:
    This source file contains the functions of the 
    DNS client routines
    
    Provides  hostname to IP address translation
    Reference: RFC 1035
*******************************************************************************/

/*******************************************************************************
File Name:  DNS.c
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
#define TCPIP_THIS_MODULE_ID    TCPIP_MODULE_DNS_CLIENT

#include "tcpip/src/tcpip_private.h"

#if defined(TCPIP_STACK_USE_DNS)
#include "tcpip/src/dns_private.h"

	
/****************************************************************************
  Section:
	Constants and Global Variables
  ***************************************************************************/

static const TCPIP_DNS_CLIENT_MODULE_CONFIG dnsDefaultConfigData = 
{
    true,
    TCPIP_DNS_CLIENT_CACHE_ENTRIES,
    TCPIP_DNS_CLIENT_CACHE_ENTRY_TMO,
    TCPIP_DNS_CLIENT_CACHE_PER_IPV4_ADDRESS,
    TCPIP_DNS_CLIENT_OPEN_ADDRESS_TYPE,
#ifdef TCPIP_STACK_USE_IPV6
    TCPIP_DNS_CLIENT_CACHE_PER_IPV6_ADDRESS,
#endif
};

static PROTECTED_SINGLE_LIST      dnsRegisteredUsers = { {0} };

#if defined (TCPIP_STACK_USE_DHCP_CLIENT)
static TCPIP_DHCP_HANDLE dnsDHCPHandler = NULL;
#endif
static DNS_DCPT     gDnsDcpt;
static DNS_DCPT*    pgDnsDcpt = 0;

static int          dnsInitCount = 0;       // module initialization count
/****************************************************************************
  Section:
	Function Prototypes
  ***************************************************************************/
#if defined (TCPIP_STACK_USE_DHCP_CLIENT)
static void DNS_DhcpNotify(TCPIP_NET_HANDLE hNet, uint8_t evType, const void * param);
#endif
static void _DNSNotifyClients(TCPIP_NET_IF* pNetIf, TCPIP_DNS_EVENT_TYPE evType,void *param);
static void _DNSPutString(uint8_t **putbuf, const char* String);
static void _DNSDiscardName(TCPIP_DNS_DATA *getBuf);
static DNS_STATE _DNSRetry(DNS_STATE currState);
static int TCPIP_DNS_GetArrayOfDataFromUDPBuff(TCPIP_DNS_DATA *getbuf,int bytes,uint8_t *buf);
static uint8_t TCPIP_DNS_GetDataFromUDPBuff(TCPIP_DNS_DATA *getbuf);
static bool TCPIP_DNS_SelectDNSIntf(bool startWithDefault);
static bool _DNS_Enable(TCPIP_NET_HANDLE hNet, bool checkIfUp);
static void _DNS_RemoveHashAll(void);
static bool TCPIP_DNS_ValidateServerAddress(TCPIP_NET_IF* pIf);
TCPIP_DNS_RESULT TCPIP_DNS_Send_Query(const char* hostName,DNS_RESOLVE_TYPE type);
static OA_HASH_ENTRY* _DNS_Entry_LookupAndInsert(const char* hostName,DNS_RESOLVE_TYPE type);

static void TCPIP_DNS_ClientProcess(void);
static void     _DNSSocketRxSignalHandler(UDP_SOCKET hUDP, TCPIP_NET_HANDLE hNet, TCPIP_UDP_SIGNAL_TYPE sigType, const void* param);

/*****************************************************************************
 swap DNS Header content packet . This API is used when we recive
 the DNS response for a query from the server.
  ***************************************************************************/
static void _SwapDNSPacket(DNS_HEADER * p)
{
    p->TransactionID.Val = TCPIP_Helper_htons(p->TransactionID.Val);
    p->Flags.Val = TCPIP_Helper_htons(p->Flags.Val);
    p->AdditionalRecords.Val = TCPIP_Helper_htons(p->AdditionalRecords.Val);
    p->Answers.Val = TCPIP_Helper_htons(p->Answers.Val);
    p->AuthoritativeRecords.Val = TCPIP_Helper_htons(p->AuthoritativeRecords.Val);
    p->Questions.Val = TCPIP_Helper_htons(p->Questions.Val);
}

static void _SwapDNSAnswerPacket(DNS_ANSWER_HEADER * p)
{
    p->ResponseClass.Val = TCPIP_Helper_htons(p->ResponseClass.Val);
    p->ResponseLen.Val = TCPIP_Helper_htons(p->ResponseLen.Val);
    p->ResponseTTL.Val = TCPIP_Helper_htonl(p->ResponseTTL.Val);
    p->ResponseType.Val = TCPIP_Helper_htons(p->ResponseType.Val);
}
static void _DNS_UpdateExpiredHashEntry_Notify(OA_HASH_DCPT    *pOH,OA_HASH_ENTRY   *hE,DNS_HASH_ENTRY* pDnsHE)
{
    pDnsHE->tInsert = 0;
    pDnsHE->tRetry = 0;
    pDnsHE->ipTTL.Val = 0;
    pDnsHE->nIPv4Entries = 0;
    pDnsHE->nIPv6Entries = 0;

    memset(pDnsHE->pHostName,0,strlen((char*)pDnsHE->pHostName));
    // Notify to to the specific Host name that it is expired.
    _DNSNotifyClients(0,TCPIP_DNS_EVENT_CACHE_EXPIRED,pDnsHE->pHostName);
    TCPIP_OAHASH_EntryRemove(pOH,hE);
}

static TCPIP_DNS_RESULT _DNS_RESPONSE_HashEntryUpdate(TCPIP_DNS_DATA *udpGetBufferData,DNS_DCPT*  pDnsDcpt,DNS_HASH_ENTRY* dnshE)
{
    DNS_ANSWER_HEADER	DNSAnswerHeader;    
    IP_MULTI_ADDRESS    ipAddr;

    if(dnshE == NULL)
    {
        return DNS_RES_MEMORY_FAIL;
    }
    _DNSDiscardName(udpGetBufferData);	// Throw away response name

    TCPIP_DNS_GetArrayOfDataFromUDPBuff(udpGetBufferData,sizeof(DNS_ANSWER_HEADER),(uint8_t *)&DNSAnswerHeader);
    _SwapDNSAnswerPacket(&DNSAnswerHeader);
// Make sure that this is a 4 byte IP address, response type A or MX, class 1
// Check if this is Type A, MX, or AAAA
    if( DNSAnswerHeader.ResponseClass.Val == 0x0001u) // Internet class
    {
        if (DNSAnswerHeader.ResponseType.Val    == DNS_TYPE_A &&
                    DNSAnswerHeader.ResponseLen.Val == 0x0004u)
        {            
            // Read the buffer
            TCPIP_DNS_GetArrayOfDataFromUDPBuff(udpGetBufferData,sizeof(IPV4_ADDR),(uint8_t *)&ipAddr.v4Add);
            if(dnshE->nIPv4Entries >= pDnsDcpt->IPv4EntriesPerDNSName)
            {
                return DNS_RES_CACHE_FULL;
            }
            if(dnshE->pip4Address == 0)
            {
                return DNS_RES_MEMORY_FAIL;
            }
            // update the Hash entry for IPv4 address

            dnshE->pip4Address[dnshE->nIPv4Entries].Val = ipAddr.v4Add.Val;
            if((DNSAnswerHeader.ResponseTTL.Val < dnshE->ipTTL.Val)
                    ||(dnshE->ipTTL.Val == 0))
            {
                dnshE->ipTTL.Val = DNSAnswerHeader.ResponseTTL.Val;
            }
            dnshE->nIPv4Entries++;
        }
        else if (DNSAnswerHeader.ResponseType.Val == DNS_TYPE_AAAA &&
                        DNSAnswerHeader.ResponseLen.Val == 0x0010u)
        {
            if ((dnshE->resolve_type != DNS_TYPE_AAAA) && (dnshE->resolve_type != DNS_TYPE_ANY))
            {
                TCPIP_DNS_GetArrayOfDataFromUDPBuff(udpGetBufferData,DNSAnswerHeader.ResponseLen.Val,(uint8_t *)0);
                return DNS_RES_OK;
            }           

            // Read the buffer which will complete the buufeer read for one interation
            TCPIP_DNS_GetArrayOfDataFromUDPBuff(udpGetBufferData,sizeof (IPV6_ADDR),
                                (uint8_t*)ipAddr.v6Add.v);
            if(dnshE->nIPv6Entries >= pDnsDcpt->IPv6EntriesPerDNSName)
            {
                return DNS_RES_CACHE_FULL;
            }
            if(dnshE->pip6Address == 0)
            {
                return DNS_RES_MEMORY_FAIL;
            }
            // Update the Hash entry for IPv6 address
           
            memcpy( &dnshE->pip6Address[dnshE->nIPv6Entries],ipAddr.v6Add.v,sizeof(IPV6_ADDR));
            if((DNSAnswerHeader.ResponseTTL.Val < dnshE->ipTTL.Val)
                    ||(dnshE->ipTTL.Val == 0))
            {
                dnshE->ipTTL.Val = DNSAnswerHeader.ResponseTTL.Val;
            }
            dnshE->nIPv6Entries++;
        }
        else
        {
            TCPIP_DNS_GetArrayOfDataFromUDPBuff(udpGetBufferData,DNSAnswerHeader.ResponseLen.Val,(uint8_t *)0);
        }
    }
    else
    {
        TCPIP_DNS_GetArrayOfDataFromUDPBuff(udpGetBufferData,DNSAnswerHeader.ResponseLen.Val,(uint8_t *)0);
    }
    return DNS_RES_OK;
}

static  TCPIP_DNS_RESULT  _DNSSetHashEntry(DNS_HASH_ENTRY* dnsHE, DNS_HASH_ENTRY_FLAGS newFlags)
{
     
    dnsHE->hEntry.flags.value &= ~DNS_FLAG_ENTRY_VALID_MASK;
    dnsHE->hEntry.flags.value |= newFlags;

    if((dnsHE->nIPv4Entries != 0) && (dnsHE->nIPv6Entries!=0))
    {
        dnsHE->recordType = IP_ADDRESS_TYPE_IPV4 | IP_ADDRESS_TYPE_IPV6;
    }
    else if(dnsHE->nIPv4Entries != 0)
    {
        dnsHE->recordType = IP_ADDRESS_TYPE_IPV4;
    }
    else if(dnsHE->nIPv6Entries != 0)
    {
        dnsHE->recordType = IP_ADDRESS_TYPE_IPV6;
    }

    if(dnsHE->ipTTL.Val == 0)
        dnsHE->ipTTL.Val = TCPIP_DNS_CLIENT_CACHE_DEFAULT_TTL_VAL;
    dnsHE->tInsert = SYS_TMR_TickCountGet()/SYS_TMR_TickCounterFrequencyGet(); // make it to seconds
    dnsHE->tRetry = dnsHE->tInsert;

    return DNS_RES_OK;
}

static  void _DNSRemoveCacheEntries(DNS_HASH_DCPT* pDNSHashDcpt)
{
    OA_HASH_ENTRY*  pBkt;
    size_t      bktIx;
    DNS_HASH_ENTRY  *pE;
    DNS_DCPT        *pDnsDcpt;
    
    pDnsDcpt = pgDnsDcpt;
    if(pDNSHashDcpt->hashDcpt)
    {
        for(bktIx = 0; bktIx < pDNSHashDcpt->hashDcpt->hEntries; bktIx++)
        {
            pBkt = TCPIP_OAHASH_EntryGet(pDNSHashDcpt->hashDcpt, bktIx);
           
            pE = (DNS_HASH_ENTRY*)pBkt;
            TCPIP_HEAP_Free(pDnsDcpt->memH,pE->memblk);
            pE->nIPv4Entries = 0;
            pE->pip4Address = 0;
            pE->ipTTL.Val= 0;
            pE->nIPv6Entries = 0;
            pE->pip6Address = 0;
            pE->pHostName = 0;
            TCPIP_OAHASH_EntryRemove(pDNSHashDcpt->hashDcpt,pBkt);
        }
    }
}

/*static __inline__*/static  void /*__attribute__((always_inline))*/ _DNSReleaseSocket(DNS_DCPT *dnsDcpt)
{
    if(dnsDcpt->dnsSocket != INVALID_UDP_SOCKET)
    {
        TCPIP_UDP_Close(dnsDcpt->dnsSocket);
        dnsDcpt->dnsSocket = INVALID_UDP_SOCKET;
    }
}

static void _DNS_RemoveHashAll(void)
{
    DNS_DCPT* pDnsDcpt=NULL;
    DNS_HASH_DCPT *pHashDcpt=NULL;
    OA_HASH_DCPT *pOhDcpt=NULL;

    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == NULL) return;
    pHashDcpt = &pDnsDcpt->dnsCacheDcpt;
    if(pHashDcpt == NULL) return;
    pOhDcpt = pHashDcpt->hashDcpt;
    if(pOhDcpt == NULL) return;
    
    _DNSRemoveCacheEntries(pHashDcpt);
    TCPIP_HEAP_Free(pDnsDcpt->memH,pHashDcpt->hashDcpt);
    pOhDcpt = 0;
}

/****************************************************************************
  Section:
	Implementation
  ***************************************************************************/

bool TCPIP_DNS_ClientInitialize(const TCPIP_STACK_MODULE_CTRL* const stackData,
                       const TCPIP_DNS_CLIENT_MODULE_CONFIG* dnsData)
{
    OA_HASH_DCPT    *hashDcpt;
    size_t          hashMemSize;
    uint32_t        memoryBlockSize=0;
    uint8_t         hashCnt=0;
    uint8_t         *pMemoryBlock=NULL;
    OA_HASH_ENTRY*  pBkt;
    DNS_HASH_ENTRY  *pE=NULL;
	
    if(stackData->stackAction == TCPIP_STACK_ACTION_IF_UP)
    {   // interface restart
        if(pgDnsDcpt->dnsSocket == INVALID_UDP_SOCKET)
        {   // enable DNS client service
            _DNS_Enable(stackData->pNetIf, false);
        }
        return true;
    }

    if(dnsInitCount == 0)
    {   // stack start up; initialize just once
        DNS_DCPT* pDnsDcpt = &gDnsDcpt;
        memset(pDnsDcpt, 0, sizeof(*pDnsDcpt));

        if(dnsData == 0)
        {
            dnsData =  &dnsDefaultConfigData;
        }

        pDnsDcpt->memH = stackData->memH;
        if(pDnsDcpt->dnsCacheDcpt.hashDcpt == 0)
        {
            hashMemSize = sizeof(OA_HASH_DCPT) + dnsData->cacheEntries * sizeof(DNS_HASH_ENTRY);
            hashDcpt = (OA_HASH_DCPT*)TCPIP_HEAP_Malloc(pDnsDcpt->memH,hashMemSize);
            if(hashDcpt == 0)
            {	// failed
                return false;
            }  
            // populate the entries
            hashDcpt->memBlk = hashDcpt + 1;
            hashDcpt->hParam = hashDcpt;	// store the descriptor it belongs to
            hashDcpt->hEntrySize = sizeof(DNS_HASH_ENTRY);
            hashDcpt->hEntries = dnsData->cacheEntries;
            hashDcpt->probeStep = DNS_HASH_PROBE_STEP;
            
            hashDcpt->hashF = TCPIP_DNS_OAHASH_KeyHash;
            hashDcpt->delF = TCPIP_DNS_OAHASH_DeleteEntry;
            hashDcpt->cmpF = TCPIP_DNS_OAHASH_KeyCompare;
            hashDcpt->cpyF = TCPIP_DNS_OAHASH_KeyCopy;
#if defined(OA_DOUBLE_HASH_PROBING)
            hashDcpt->probeHash = TCPIP_DNS_OAHASH_ProbeHash;
#endif  // defined(OA_DOUBLE_HASH_PROBING)

            TCPIP_OAHASH_Initialize(hashDcpt);
            pDnsDcpt->dnsCacheDcpt.hashDcpt = hashDcpt;
            pDnsDcpt->smState = DNS_START;
            pDnsDcpt->dnsSocket =  INVALID_UDP_SOCKET;
            pDnsDcpt->cacheEntryTMO = dnsData->entrySolvedTmo;
            pDnsDcpt->dnscacheEntries = dnsData->cacheEntries;
            pDnsDcpt->IPv4EntriesPerDNSName= dnsData->IPv4EntriesPerDNSName;
            pDnsDcpt->IPv6EntriesPerDNSName = dnsData->IPv6EntriesPerDNSName;
            pDnsDcpt->dnsIpAddressType = dnsData->dnsIpAddressType;
            pDnsDcpt->vDNSServerIx = 0;
            pDnsDcpt->DNSServers[0].v4Add.Val = 0;
            pDnsDcpt->DNSServers[1].v4Add.Val = 0;
            pDnsDcpt->pDNSNet=0;

            // allocate memory for each DNS hostname , IPv4 address and IPv6 address
            // and the allocation will be done per Hash descriptor
            memoryBlockSize = pDnsDcpt->IPv4EntriesPerDNSName*sizeof(IPV4_ADDR)
                    + pDnsDcpt->IPv6EntriesPerDNSName *sizeof(IPV6_ADDR)
                    + TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN ;
            for(hashCnt=0;hashCnt < dnsData->cacheEntries;hashCnt++)
            {
                pBkt = TCPIP_OAHASH_EntryGet(hashDcpt, hashCnt);
               
                pE = (DNS_HASH_ENTRY*)pBkt;
                pE->pHostName = NULL;
                pE->pip4Address = NULL;
                pE->pip6Address = NULL;
                pMemoryBlock = (uint8_t *)TCPIP_HEAP_Malloc(pDnsDcpt->memH,memoryBlockSize);
                if(pMemoryBlock == 0)
                {
                    if(hashDcpt != 0)
                    {
                        // if there is any pMemoryBlock already made for other
                        // hash entries , we need to remove those also
                       _DNS_RemoveHashAll();
                    }
                    return false;
                }
                
                pE = (DNS_HASH_ENTRY*)pBkt;
                pE->memblk = pMemoryBlock;
                // if IPv4EntriesPerDNSName != 0, then allocate memory for IPv4 entries
                if(pDnsDcpt->IPv4EntriesPerDNSName)
                {
                    pE->pip4Address = (IPV4_ADDR *)pMemoryBlock;
                }
                if(pDnsDcpt->IPv6EntriesPerDNSName)
                {
                    pE->pip6Address = (IPV6_ADDR *)(pMemoryBlock+pDnsDcpt->IPv4EntriesPerDNSName*(sizeof(IPV4_ADDR)));
                }
                // allocate Hostname
                if(TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN !=0)
                {
                    pE->pHostName = (uint8_t*)(pMemoryBlock+(pDnsDcpt->IPv4EntriesPerDNSName*(sizeof(IPV4_ADDR)))
                            + (pDnsDcpt->IPv6EntriesPerDNSName * (sizeof(IPV6_ADDR)))
                            );
                }
            }
        }
        
        TCPIP_Notification_Initialize(&dnsRegisteredUsers);
        
#if defined (TCPIP_STACK_USE_DHCP_CLIENT)
        dnsDHCPHandler = TCPIP_DHCP_HandlerRegister(0, (TCPIP_DHCP_EVENT_HANDLER)DNS_DhcpNotify, NULL);
        if (dnsDHCPHandler == NULL)
        {
            _DNS_RemoveHashAll();
            return false;
        }
#endif
        pDnsDcpt->dnsSignalHandle =_TCPIPStackSignalHandlerRegister(TCPIP_THIS_MODULE_ID, TCPIP_DNS_ClientTask, TCPIP_DNS_CLIENT_TASK_PROCESS_RATE);
        if(pDnsDcpt->dnsSignalHandle == 0)
        {
            _DNS_RemoveHashAll();
            return false;
        }        

        // module is initialized and pgDnsDcpt is valid!
        pgDnsDcpt = &gDnsDcpt;
    }

    if(stackData->pNetIf->Flags.bIsDnsClientEnabled != 0)
    {   // enable DNS client service
        _DNS_Enable(stackData->pNetIf, false);
    }
    dnsInitCount++;
    return true;
}

static void _DNSClientDeleteCache(DNS_DCPT* pDnsDcpt)
{
    DNS_HASH_DCPT *pDnsHashDcpt;
    pDnsHashDcpt = &pDnsDcpt->dnsCacheDcpt;

    // Remove dns Timer Handle
    if( pDnsDcpt->dnsSignalHandle)
    {
       _TCPIPStackSignalHandlerDeregister( pDnsDcpt->dnsSignalHandle);
        pDnsDcpt->dnsSignalHandle = 0;
    }
    // Remove DNS register users
    TCPIP_Notification_Deinitialize(&dnsRegisteredUsers, pDnsDcpt->memH);

    // Remove dns DHCP handler
#if defined   ( TCPIP_STACK_USE_DHCP_CLIENT)
    if (dnsDHCPHandler != NULL)
    {
        TCPIP_DHCP_HandlerDeRegister(dnsDHCPHandler);
        dnsDHCPHandler = NULL;
    }
#endif
    // Remove Hash Entries
    if(pDnsHashDcpt)
    {
        _DNS_RemoveHashAll();
    }   
}

void TCPIP_DNS_ClientDeinitialize(const TCPIP_STACK_MODULE_CTRL* const stackData)
{
    // interface going down

    if(dnsInitCount > 0)
    {   // we're up and running
        DNS_DCPT* pDnsDcpt = pgDnsDcpt;

        if(pDnsDcpt->dnsSocket != INVALID_UDP_SOCKET)
        {
            TCPIP_NET_HANDLE hNet = TCPIP_UDP_SocketNetGet(pDnsDcpt->dnsSocket);
            if(stackData->pNetIf == hNet)
            {   // this interface is going down
                _DNSReleaseSocket(pDnsDcpt);
            }
        }

        if(stackData->stackAction == TCPIP_STACK_ACTION_DEINIT)
        {   // stack shut down
            if(--dnsInitCount == 0)
            {   // all closed and Release DNS client Hash resources
                _DNSClientDeleteCache(pDnsDcpt);
                // module is de-initialized and pgDnsDcpt is invalid!
                pgDnsDcpt = 0;
            }
        }
    }
}

TCPIP_DNS_RESULT TCPIP_DNS_Resolve(const char* hostName, DNS_RESOLVE_TYPE type)
{
    DNS_DCPT            *pDnsDcpt;
    DNS_HASH_DCPT       *pDnsHashDcpt;
    DNS_HASH_ENTRY      *dnsHashEntry;
    OA_HASH_ENTRY       *hE;
    IP_MULTI_ADDRESS    ipAddr;
    TCPIP_DNS_RESULT    res;

    pDnsDcpt = pgDnsDcpt;

    if(pDnsDcpt == 0)
    {
        return DNS_RES_NO_SERVICE;
    }

    pDnsHashDcpt = &pDnsDcpt->dnsCacheDcpt;
  
    hE = TCPIP_OAHASH_EntryLookup(pDnsHashDcpt->hashDcpt,(void*) hostName);
    if(hE != 0)
    {
        dnsHashEntry = (DNS_HASH_ENTRY*)hE;

        if(dnsHashEntry->hEntry.flags.value & DNS_FLAG_ENTRY_COMPLETE)
        {
            // if the type is not matching to the existing resolve type
            // then send the query with new DNS resolve type
            if(dnsHashEntry->resolve_type == type)
            {
                return DNS_RES_OK;
            }
            else
            {
                res = TCPIP_DNS_Send_Query(hostName,type);
                if(res!= DNS_RES_OK)
                {
                    return res;
                }
            }
        }
        else
        {
            pDnsDcpt->smState = DNS_GET_RESULT;
        }
    }
    else
    {
        /*
        - check hostname , if this one is not part of HASH table. then
        - add this entry as DNS_FLAG_ENTRY_INCOMPLETE and send a query
        */
        if(TCPIP_Helper_StringToIPAddress(hostName, &ipAddr.v4Add))
        {
            return  DNS_RES_NAME_IS_IPADDRESS; // DNS request is a valid IPv4 or IPv6 address;
        }
        else if (TCPIP_Helper_StringToIPv6Address (hostName, &ipAddr.v6Add))
        {
            return  DNS_RES_NAME_IS_IPADDRESS; // DNS request is a valid IPv4 or IPv6 address;
        }
        res = TCPIP_DNS_Send_Query(hostName,type);
        if(res!= DNS_RES_OK) 
        {
            return res;
        }
    }		
    return DNS_RES_OK;
}

TCPIP_DNS_RESULT TCPIP_DNS_GetIPv4Address(const char* hostName,int index,IPV4_ADDR* ipv4Addr)
{
    DNS_DCPT*		pDnsDcpt;
    DNS_HASH_DCPT*   pDnsHashDcpt;
    DNS_HASH_ENTRY*	 dnsHashEntry;
    OA_HASH_ENTRY*   hE;
    
    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0 || ipv4Addr == NULL)
    {
        return DNS_RES_NO_SERVICE;
    }

    pDnsHashDcpt = &pDnsDcpt->dnsCacheDcpt;
    hE = TCPIP_OAHASH_EntryLookup(pDnsHashDcpt->hashDcpt,(void*) hostName);
    if(hE != 0)
    {
        if(hE->flags.value & DNS_FLAG_ENTRY_COMPLETE)
        {
            dnsHashEntry = (DNS_HASH_ENTRY*)hE;
            if((dnsHashEntry->recordType & IP_ADDRESS_TYPE_IPV4) == IP_ADDRESS_TYPE_IPV4)
            {
                if(index > dnsHashEntry->nIPv4Entries)
                {
                    return DNS_RES_NO_SERVICE;
                }
                ipv4Addr->Val = dnsHashEntry->pip4Address[index].Val;
            }
        }
    }
    return DNS_RES_OK;
}

TCPIP_DNS_RESULT TCPIP_DNS_GetIPv6Address(const char* hostName,int index,IPV6_ADDR* ipv6Addr)
{
    DNS_DCPT*		pDnsDcpt;
    DNS_HASH_DCPT*   pDnsHashDcpt;
    DNS_HASH_ENTRY*	 dnsHashEntry;
    OA_HASH_ENTRY*   hE;
    

    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0 || ipv6Addr == NULL)
    {
        return DNS_RES_NO_SERVICE;
    }

    pDnsHashDcpt = &pDnsDcpt->dnsCacheDcpt;
    hE = TCPIP_OAHASH_EntryLookup(pDnsHashDcpt->hashDcpt,(void*) hostName);
    if(hE != 0)
    {
        if(hE->flags.value & DNS_FLAG_ENTRY_COMPLETE)
        {
            dnsHashEntry = (DNS_HASH_ENTRY*)hE;
            if((dnsHashEntry->recordType & IP_ADDRESS_TYPE_IPV6) == IP_ADDRESS_TYPE_IPV6)
            {
                if(index > dnsHashEntry->nIPv6Entries)
                {
                    return DNS_RES_NO_SERVICE;
                }
                memcpy (ipv6Addr, dnsHashEntry->pip6Address+index, sizeof (IPV6_ADDR));
            }
        }
    }
    return DNS_RES_OK;
}

int TCPIP_DNS_GetNumberOfIPAddresses(const char* hostName, IP_ADDRESS_TYPE type)
{
    DNS_DCPT*		pDnsDcpt;
    DNS_HASH_DCPT*   pDnsHashDcpt;
    DNS_HASH_ENTRY*	 dnsHashEntry;
    OA_HASH_ENTRY*   hE;
    
    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0 || hostName == 0)
    {
        return 0;
    }

    pDnsHashDcpt = &pDnsDcpt->dnsCacheDcpt;

    hE = TCPIP_OAHASH_EntryLookup(pDnsHashDcpt->hashDcpt,(void*) hostName);
    if(hE != 0)
    {
        if(hE->flags.value & DNS_FLAG_ENTRY_COMPLETE)
        {
            dnsHashEntry = (DNS_HASH_ENTRY*)hE;
            if((dnsHashEntry->recordType & type) == IP_ADDRESS_TYPE_IPV4)
            {
                return dnsHashEntry->nIPv4Entries;
            }
            else if((dnsHashEntry->recordType & type) == IP_ADDRESS_TYPE_IPV6)
            {
                 return dnsHashEntry->nIPv6Entries;
            }
        }
    }
    return 0;
}

TCPIP_DNS_RESULT TCPIP_DNS_IsResolved(const char* hostName, void * ipAddr)
{    
    DNS_DCPT*		pDnsDcpt;
    OA_HASH_ENTRY*	hE;
    DNS_HASH_ENTRY* pDnsHE;
    TCPIP_DNS_RESOLVED_SERVER_ADDRESS serverIpAddress;

    pDnsDcpt = pgDnsDcpt;

    if(pDnsDcpt == 0 || ipAddr == NULL)
    {
        return DNS_RES_NO_SERVICE;
    }
    
    if(TCPIP_Helper_StringToIPAddress(hostName, &serverIpAddress.ip4Address))
    {
        ((IPV4_ADDR*)ipAddr)->Val = serverIpAddress.ip4Address.Val;
        return  DNS_RES_OK; // DNS request is a valid IPv4 address;
    }
    else if (TCPIP_Helper_StringToIPv6Address (hostName, &serverIpAddress.ip6Address))
    {
        memcpy (ipAddr, &serverIpAddress.ip6Address, sizeof (IPV6_ADDR));
        return  DNS_RES_OK; // DNS request is a valid IPv6 address;
    }
	
    hE = TCPIP_OAHASH_EntryLookup(pDnsDcpt->dnsCacheDcpt.hashDcpt,(void*)hostName);
    if(hE == 0)
    {
        return DNS_RES_NO_ENTRY;
    }
    if((hE->flags.value & DNS_FLAG_ENTRY_INCOMPLETE) == DNS_FLAG_ENTRY_INCOMPLETE)
    {
        if(pDnsDcpt->smState < DNS_DONE)
        {
            return DNS_RES_PENDING;
        }
        else
        {
            return DNS_RES_SERVER_TMO; 
        }
    }

    if((hE->flags.value & DNS_FLAG_ENTRY_COMPLETE) == DNS_FLAG_ENTRY_COMPLETE)
    {
    	pDnsHE = (DNS_HASH_ENTRY*)hE;

        if (pDnsHE->resolve_type == DNS_TYPE_AAAA)
        {
            serverIpAddress.nIPv6Entries = TCPIP_DNS_GetNumberOfIPAddresses(hostName,IP_ADDRESS_TYPE_IPV6);
            if(serverIpAddress.nIPv6Entries == 0)
            {
                return DNS_RES_NO_ENTRY;
            }
            TCPIP_DNS_GetIPv6Address(hostName,serverIpAddress.nIPv6Entries-1,&serverIpAddress.ip6Address);
            memcpy (ipAddr, &serverIpAddress.ip6Address, sizeof (IPV6_ADDR));
        }
        else if(pDnsHE->resolve_type == DNS_TYPE_A)
//  Get the valid Ipv4 address.
        {
            serverIpAddress.nIPv4Entries = TCPIP_DNS_GetNumberOfIPAddresses(hostName,IP_ADDRESS_TYPE_IPV4);
            serverIpAddress.ip4Address.Val = 0;
            if(serverIpAddress.nIPv4Entries != 0)
            {
                // get the  0th location of the address
                TCPIP_DNS_GetIPv4Address(hostName,0,&serverIpAddress.ip4Address);
                ((IPV4_ADDR*)ipAddr)->Val = serverIpAddress.ip4Address.Val;
            }
            else
            {
                return DNS_RES_NO_ENTRY;
            }
        }
        else if(pDnsHE->resolve_type == DNS_TYPE_ANY)
        {
            serverIpAddress.nIPv4Entries = TCPIP_DNS_GetNumberOfIPAddresses(hostName,IP_ADDRESS_TYPE_IPV4);
            serverIpAddress.nIPv6Entries = TCPIP_DNS_GetNumberOfIPAddresses(hostName,IP_ADDRESS_TYPE_IPV6);
            if((serverIpAddress.nIPv4Entries == 0) && (serverIpAddress.nIPv6Entries == 0))
            {
                return DNS_RES_NO_ENTRY;
            }
            if(serverIpAddress.nIPv4Entries != 0)
            {
                serverIpAddress.ip4Address.Val = 0;
                 TCPIP_DNS_GetIPv4Address(hostName,0,&serverIpAddress.ip4Address);
                ((IPV4_ADDR*)ipAddr)->Val = serverIpAddress.ip4Address.Val;
            }
            else
            {
                TCPIP_DNS_GetIPv6Address(hostName,serverIpAddress.nIPv6Entries-1,&serverIpAddress.ip6Address);
                memcpy (ipAddr, &serverIpAddress.ip6Address, sizeof (IPV6_ADDR));
            }
        }
    }
    return DNS_RES_OK;
}

TCPIP_DNS_RESULT TCPIP_DNS_GetDNSClientConnectionInfo(IPV4_ADDR *Addr1,IPV4_ADDR *Addr2,int *intfIndex,bool *moduleOnOff)
{
    DNS_DCPT* pDnsDcpt = pgDnsDcpt;

    if(pDnsDcpt==NULL)
    {
         return DNS_RES_NO_SERVICE;
    }
    if(pDnsDcpt->pDNSNet == NULL)
    {
        *intfIndex = 0xFF;
    }
    else
    {
        *intfIndex = pDnsDcpt->pDNSNet->netIfIx;
    }
    Addr1->Val = pDnsDcpt->DNSServers[0].v4Add.Val;
    Addr2->Val = pDnsDcpt->DNSServers[1].v4Add.Val;
    *moduleOnOff =  pDnsDcpt->enabled;
    return DNS_RES_OK;
}

TCPIP_DNS_RESULT TCPIP_DNS_GetResolverDetails(char *pDnsHostName,TCPIP_DNS_RESOLVED_ADDRESS *ipAddr,uint32_t *ttlTime,uint32_t len,int *index)
{
    OA_HASH_ENTRY*  pBkt;
    DNS_HASH_ENTRY  *pE;
    DNS_DCPT        *pDnsDcpt;
    DNS_HASH_DCPT   *pDNSHashDcpt;
    uint32_t        tempLen=0;
    uint8_t         ipInstance=0;
    int             bktIndex=0;
    uint32_t        getStartTime = 0;

    pDnsDcpt = pgDnsDcpt;

    if(pDnsDcpt == 0 || pDnsHostName == 0 || len == 0)
    {
        return DNS_RES_NO_SERVICE;
    }

    pDNSHashDcpt = &pDnsDcpt->dnsCacheDcpt;
    if(*index>= pDNSHashDcpt->hashDcpt->hEntries)
    {
         return DNS_RES_NO_SERVICE;
    }
    if(pDNSHashDcpt->hashDcpt)
    {
        bktIndex = *index;
        while(1)
        {
            pBkt = TCPIP_OAHASH_EntryGet(pDNSHashDcpt->hashDcpt, bktIndex);
            if(pBkt != 0)
            {
                if((pBkt->flags.busy != 0) && (pBkt->flags.value & DNS_FLAG_ENTRY_COMPLETE))
                {
                    pE = (DNS_HASH_ENTRY*)pBkt;
                    tempLen = strlen((char*)pE->pHostName);
                    // collect the string upto the "len" size.
                    if(len < tempLen)
                    {
                        tempLen = len;
                    }
                    strncpy((char*)pDnsHostName,(char*)pE->pHostName,tempLen);
                    getStartTime = SYS_TMR_TickCountGet()/SYS_TMR_TickCounterFrequencyGet();
                    //*ttlTime = pE->ipTTL.Val;
                    if(pDnsDcpt->cacheEntryTMO > 0)
                    {
                        *ttlTime = pDnsDcpt->cacheEntryTMO - (getStartTime-pE->tInsert);
                    }
                    else
                    {
                        *ttlTime = pE->ipTTL.Val - (getStartTime-pE->tInsert);
                    }
                    
                    if(pE->nIPv4Entries != 0)
                    {
                        for(ipInstance=0;ipInstance <pE->nIPv4Entries;ipInstance++)
                        {
                            ipAddr->ipv4Address[ipInstance].Val = pE->pip4Address[ipInstance].Val;
                        }
                        ipAddr->nIPv4Entries = pE->nIPv4Entries;
                    }
                    if(pE->nIPv6Entries != 0)
                    {
                        for(ipInstance=0;ipInstance <pE->nIPv6Entries;ipInstance++)
                        {
                            memcpy(ipAddr->ipv6Address[ipInstance].v,pE->pip6Address[ipInstance].v,sizeof(IPV6_ADDR));
                        }
                        ipAddr->nIPv6Entries = pE->nIPv6Entries;
                    }
                    *index = bktIndex;
                    return DNS_RES_OK;
                }
                else
                {
                    pE = (DNS_HASH_ENTRY*)pBkt;
                    bktIndex++;
                    if(bktIndex>= pDNSHashDcpt->hashDcpt->hEntries)
                    {
                         break;
                    }
                }
            }       
        }
    }
    return DNS_RES_NO_SERVICE;
}

TCPIP_DNS_RESULT TCPIP_DNS_RemoveEntry(const char *pDnsHostName)
{
    OA_HASH_ENTRY*  pBkt;
    DNS_HASH_ENTRY  *pE;
    DNS_DCPT        *pDnsDcpt;
    DNS_HASH_DCPT   *pDNSHashDcpt;
    
    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0 || pDnsHostName == NULL)
    {
        return DNS_RES_NO_SERVICE;
    }

    pDNSHashDcpt = &pDnsDcpt->dnsCacheDcpt;

    if(pDNSHashDcpt->hashDcpt)
    {
        pBkt = TCPIP_OAHASH_EntryLookup(pDNSHashDcpt->hashDcpt,(void*) pDnsHostName);
        if(pBkt != 0)
        {
            if(pBkt->flags.busy != 0)
            {
                pE = (DNS_HASH_ENTRY*)pBkt;
                _DNS_UpdateExpiredHashEntry_Notify(pDNSHashDcpt->hashDcpt,pBkt,pE);
      		return DNS_RES_OK;
            }
        }
        else
        {
            return DNS_RES_NO_ENTRY;
        }
    }
    return DNS_RES_NO_SERVICE;
}

TCPIP_DNS_RESULT TCPIP_DNS_RemoveAll(void)
{
    OA_HASH_ENTRY*  pBkt;
    size_t  bktIx;
    DNS_HASH_ENTRY  *pE;
    DNS_DCPT        *pDnsDcpt;
    DNS_HASH_DCPT   *pDNSHashDcpt;

    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0)
    {
        return DNS_RES_NO_SERVICE;
    }

    pDNSHashDcpt = &pDnsDcpt->dnsCacheDcpt;

    if(pDNSHashDcpt->hashDcpt)
    {
        for(bktIx = 0; bktIx < pDNSHashDcpt->hashDcpt->hEntries; bktIx++)
        {
            pBkt = TCPIP_OAHASH_EntryGet(pDNSHashDcpt->hashDcpt, bktIx);
            if(pBkt != 0)
            {
                if(pBkt->flags.busy != 0)
                {
                    pE = (DNS_HASH_ENTRY*)pBkt;
                    _DNS_UpdateExpiredHashEntry_Notify(pDNSHashDcpt->hashDcpt,pBkt,pE);
                }
            }
        }
    }

    return DNS_RES_OK;
}

DNS_HASH_ENTRY *_DNSHashEntryFromTransactionId(TCPIP_UINT16_VAL transactionId)
{
    DNS_HASH_ENTRY* pDnsHE;
    DNS_HASH_DCPT  *pDnsHashDcpt;
    OA_HASH_ENTRY   *hE;
    int             bktIx=0;
    OA_HASH_DCPT    *pOH;
    DNS_DCPT       *pDnsDcpt;
    
    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0) 
    {
        return 0;
    }
    
    pDnsHashDcpt = &pDnsDcpt->dnsCacheDcpt;
    pOH = pDnsHashDcpt->hashDcpt;

    for(bktIx = 0; bktIx < pOH->hEntries; bktIx++)
    {
        hE = TCPIP_OAHASH_EntryGet(pOH, bktIx);
        if(hE->flags.busy != 0)
        {
            pDnsHE = (DNS_HASH_ENTRY*)hE;
            if(pDnsHE->SentTransactionID.Val == transactionId.Val)
            {
                return pDnsHE;
            }
        }
    }
    return 0;
}

void TCPIP_DNS_CacheTimeout(void)
{
    DNS_HASH_ENTRY  *pDnsHE;
    DNS_HASH_DCPT   *pDnsHashDcpt;
    OA_HASH_ENTRY   *hE;
    int             bktIx=0;
    OA_HASH_DCPT    *pOH;
    DNS_DCPT        *pDnsDcpt;
    uint32_t        getTime=0;
    
    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0) 
    {
        return;
    }
    pDnsHashDcpt = &pDnsDcpt->dnsCacheDcpt;
    pOH = pDnsHashDcpt->hashDcpt;
    if(pDnsDcpt->enabled != true)
    {
        return;
    }
    // make it to seconds
    getTime = SYS_TMR_TickCountGet()/SYS_TMR_TickCounterFrequencyGet();

// check the lease values and if there is any entry whose lease value exceeds the lease duration remove the lease entries from the HASH.

    for(bktIx = 0; bktIx < pOH->hEntries; bktIx++)
    {
        hE = TCPIP_OAHASH_EntryGet(pOH, bktIx);
    	if((hE->flags.busy != 0) && (hE->flags.value & DNS_FLAG_ENTRY_COMPLETE))
    	{
            pDnsHE = (DNS_HASH_ENTRY*)hE;
            // if cacheEntryTMO is equal to zero, then TTL time is the timeout period. 
            // TTL time may be a  higher value , so it i sbetter to use a cacheEntryTMO
            if(pDnsDcpt->cacheEntryTMO > 0)
            {
                if((getTime-pDnsHE->tInsert) > pDnsDcpt->cacheEntryTMO)
                {
                    pDnsHE->tInsert = 0;
                    pDnsHE->tRetry = 0;
                    _DNS_UpdateExpiredHashEntry_Notify(pOH,hE,pDnsHE);
                 }
            }
            else
            {
                if((getTime-pDnsHE->tInsert) > pDnsHE->ipTTL.Val)
                {
                  _DNS_UpdateExpiredHashEntry_Notify(pOH,hE,pDnsHE);
                }               
            }    			
    	}
        else if((hE->flags.busy != 0) && (hE->flags.value & DNS_FLAG_ENTRY_INCOMPLETE))
        {
            pDnsHE = (DNS_HASH_ENTRY*)hE;
            if((getTime - pDnsHE->tInsert)>TCPIP_DNS_CLIENT_CACHE_UNSOLVED_ENTRY_TMO)
            {
              _DNS_UpdateExpiredHashEntry_Notify(pOH,hE,pDnsHE);
            }
            else if(pDnsHE->pHostName)
            {
                if(strlen((char*)pDnsHE->pHostName)>0)
                { 
                    if((getTime - pDnsHE->tRetry) > TCPIP_DNS_CLIENT_LOOKUP_RETRY_TMO)  
                    {
                        TCPIP_DNS_Send_Query((char*)pDnsHE->pHostName,pDnsHE->resolve_type);
                        pDnsHE->tRetry = getTime;
                    }
                }
            }
        }
    } 
}

static uint8_t TCPIP_DNS_GetDataFromUDPBuff(TCPIP_DNS_DATA *getbuf)
{
    uint8_t val = getbuf->wrPtr[0];
    getbuf->wrPtr = getbuf->wrPtr+1;
    return val;
}

static int TCPIP_DNS_GetArrayOfDataFromUDPBuff(TCPIP_DNS_DATA *getbuf,int bytes,uint8_t *buf)
{
    int nBytes = getbuf->endPtr - getbuf->wrPtr;
    
    if(bytes < nBytes)
    {
        nBytes =  bytes;
    }

    if(buf == NULL)
    {
        getbuf->wrPtr = getbuf->wrPtr+nBytes;
        return 0;
    }
    memcpy(buf,getbuf->wrPtr,nBytes);
    getbuf->wrPtr = getbuf->wrPtr+nBytes;
    return nBytes;
}

static OA_HASH_ENTRY* _DNS_Entry_LookupAndInsert(const char* hostName,DNS_RESOLVE_TYPE type)
{
    TCPIP_UINT16_VAL    transactionId;
    DNS_HASH_ENTRY      *dnshE;
    OA_HASH_ENTRY       *hE;
    DNS_HASH_DCPT       *pDnsHashDcpt;
    DNS_DCPT*		pDnsDcpt;

    pDnsDcpt = pgDnsDcpt;
    pDnsHashDcpt = &pDnsDcpt->dnsCacheDcpt;

    hE = TCPIP_OAHASH_EntryLookup(pDnsHashDcpt->hashDcpt,(void*) hostName);
    if(hE != 0)
    {
        dnshE = (DNS_HASH_ENTRY*)hE;
        // if entry is already COMPLETE state,There will be no transmission of DNS query again.
        // It means that the entry is alreaady resolved and part of the HASH entry with
        // IPv4 or IPv6 address
        if(dnshE->hEntry.flags.value & DNS_FLAG_ENTRY_COMPLETE)
        {
            // if the type is different then send the query again with new type
            if(dnshE->resolve_type != type)
            {   // calculate DNS query transaction ID
                transactionId.Val = (uint16_t)SYS_RANDOM_PseudoGet();
                transactionId.Val = TCPIP_Helper_htons(transactionId.Val);
                dnshE = (DNS_HASH_ENTRY*)hE;
                dnshE->tInsert = SYS_TMR_TickCountGet()/SYS_TMR_TickCounterFrequencyGet();
                dnshE->tRetry = dnshE->tInsert;
                dnshE->SentTransactionID.Val = transactionId.Val;
            }
            else
                return NULL;
        }
        // else part is ready to send the query
    }
    else
    {
        hE = TCPIP_OAHASH_EntryLookupOrInsert(pDnsHashDcpt->hashDcpt,(void*)hostName);
        if(hE == 0)
        {
            return NULL;
        }
        // calculate DNS query transaction ID
        transactionId.Val = (uint16_t)SYS_RANDOM_PseudoGet();
        transactionId.Val = TCPIP_Helper_htons(transactionId.Val);
        dnshE = (DNS_HASH_ENTRY*)hE;
        dnshE->tInsert = SYS_TMR_TickCountGet()/SYS_TMR_TickCounterFrequencyGet();
        dnshE->tRetry = dnshE->tInsert;
        dnshE->SentTransactionID.Val = transactionId.Val;
    }

    return hE;
}

TCPIP_DNS_RESULT TCPIP_DNS_Send_Query(const char* hostName,DNS_RESOLVE_TYPE type)
{
    DNS_HEADER		DNSPutHeader;
    DNS_DCPT*		pDnsDcpt;
    TCPIP_UINT16_VAL    dnsBufferTempVal;
    DNS_HASH_ENTRY      *dnshE;
    OA_HASH_ENTRY       *hE;
    uint32_t            hostNameSize=0,bufferSize=0;
    uint8_t             *wrPtr=NULL,*startPtr=NULL;

    pDnsDcpt = pgDnsDcpt;

    // check the available interface it need to be sent
    // get the Interface details
    // if the UDP socket is not created and DNS module is not enabled, then don't process the DNS query
    if(pDnsDcpt == 0 || !pDnsDcpt->enabled)
    {
        return DNS_RES_NO_SERVICE;
    }

    if(!TCPIP_STACK_NetIsReady(pDnsDcpt->pDNSNet))
    {
        return DNS_RES_NO_SERVICE;
    }
 
    if(hostName == NULL)
    {
        return DNS_RES_INVALID_HOSTNAME;
    }

    hostNameSize = strlen((const char*)hostName);
    if(hostNameSize >= TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN)
    {
        return DNS_RES_INVALID_HOSTNAME;
    }

    hE = _DNS_Entry_LookupAndInsert((void*) hostName,type);
    if(hE == NULL)
    {
        return DNS_RES_LOOKUP_FAIL;
    }
    
    hE->flags.value &= ~DNS_FLAG_ENTRY_VALID_MASK;
    hE->flags.value |= DNS_FLAG_ENTRY_INCOMPLETE;
    dnshE = (DNS_HASH_ENTRY*)hE;
    dnshE->netIfIdx = pDnsDcpt->pDNSNet->netIfIx;
    dnshE->resolve_type = type;
    if(dnshE->pHostName == NULL)
    {
        return DNS_RES_NO_ENTRY;
    }

    bufferSize = TCPIP_UDP_TxPutIsReady(pDnsDcpt->dnsSocket, 18+TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN+1);
    if(bufferSize < (18+TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN+1))
    {   // failed to allocate another TX buffer
         TCPIP_UDP_Flush(pDnsDcpt->dnsSocket);  // discard old buffer data
         return DNS_RES_SOCKET_NOT_READY;
    }

    //this will put the start pointer at the beginning of the TX buffer
    TCPIP_UDP_TxOffsetSet(pDnsDcpt->dnsSocket,0,false);    

    //Get the write pointer:
    wrPtr = TCPIP_UDP_TxPointerGet(pDnsDcpt->dnsSocket);
    if(wrPtr == 0)
    {
         return DNS_RES_SOCKET_NOT_READY;
    }
    startPtr = wrPtr;
// Put DNS query here
    // Set Transaction ID
    DNSPutHeader.TransactionID.v[1] = dnshE->SentTransactionID.v[0];
    DNSPutHeader.TransactionID.v[0] = dnshE->SentTransactionID.v[1];
    // Flag -- Standard query with recursion
    DNSPutHeader.Flags.v[1] = 0x00; // 0x0100 - Standard query with recursion
    DNSPutHeader.Flags.v[0] = 0x01;
    // Question -- only one question at this time
    DNSPutHeader.Questions.v[1] = 0x01; // 0x0001 questions
    DNSPutHeader.Questions.v[0] = 0x00;
    // Answers set to zero
    // Name server resource address also set to zero
    // Additional records also set to zero
    DNSPutHeader.Answers.Val = 
        DNSPutHeader.AuthoritativeRecords.Val =
            DNSPutHeader.AdditionalRecords.Val = 0;
   
    // copy the DNS header to the UDP buffer
    memcpy(wrPtr,&DNSPutHeader, sizeof(DNS_HEADER));
    wrPtr += sizeof(DNS_HEADER);

    // Put hostname string to resolve
    _DNSPutString(&wrPtr, (const char*)dnshE->pHostName);

    // Type: DNS_TYPE_A A (host address) or DNS_TYPE_MX for mail exchange
    dnsBufferTempVal.v[1] = dnshE->resolve_type;
    dnsBufferTempVal.v[0] = 0x00;
    memcpy(wrPtr,dnsBufferTempVal.v,sizeof(TCPIP_UINT16_VAL));
    wrPtr += sizeof(TCPIP_UINT16_VAL);
    

    // Class: IN (Internet)
    dnsBufferTempVal.v[1] = 0x01; // value is 0x0001
    dnsBufferTempVal.v[0] = 0x00;
    memcpy(wrPtr,dnsBufferTempVal.v,sizeof(TCPIP_UINT16_VAL));
    wrPtr += sizeof(TCPIP_UINT16_VAL);
    
    // Put complete DNS query packet buffer to the UDP buffer
    
    // Once it is completed writing into the buffer, you need to update the Tx offset again,
    // because the socket flush function calculates how many bytes are in the buffer using the current write pointer:
    TCPIP_UDP_TxOffsetSet(pDnsDcpt->dnsSocket,(uint16_t)(wrPtr - startPtr), false);
    // that?s why we needed the startPtr, to know how many bytes we wrote to the buffer

    TCPIP_UDP_Flush(pDnsDcpt->dnsSocket);    
    // change the state to DNS_GET_RESULT
    pDnsDcpt->smState = DNS_GET_RESULT;
    // Send a DNS notification after sending a DNS query.
    _DNSNotifyClients(0,TCPIP_DNS_EVENT_QUERY,dnshE->pHostName);

    return DNS_RES_OK;
}

static void TCPIP_DNS_UpdateDNSServerAddressDesc(TCPIP_NET_IF *pIf)
{
    DNS_DCPT    *pDnsDcpt;

    pDnsDcpt = pgDnsDcpt;
    if((pIf == NULL) || (pDnsDcpt == NULL)) 
    {
        return;
    }

    pDnsDcpt->DNSServers[0].v4Add.Val = pIf->PrimaryDNSServer.Val;
    pDnsDcpt->DNSServers[1].v4Add.Val = pIf->SecondaryDNSServer.Val;
    
    if(pDnsDcpt->DNSServers[0].v4Add.Val == 0)
    {
        pDnsDcpt->DNSServers[0].v4Add.Val = pIf->DefaultDNSServer.Val;
    }
    if(pDnsDcpt->DNSServers[1].v4Add.Val == 0)
    {
        pDnsDcpt->DNSServers[1].v4Add.Val = pIf->DefaultDNSServer2.Val;
    }
    if(pDnsDcpt->DNSServers[1].v4Add.Val == pDnsDcpt->DNSServers[0].v4Add.Val)
    {
        pDnsDcpt->DNSServers[1].v4Add.Val = 0;
    }
}

// This API does these following things -
// 1. TCPIP_UDP_Bind() will bind the network IP address to this interface. This helps when we have more than one interface and
// when each interface in different subnet.
// 2 TCPIP_UDP_DestinationIPAddressSet() set the Destination IP address( DNS server IP address)
// DNS server address is selcted as per the argument value of serverIndex.
static void TCPIP_DNS_ActiveDNSServerIntfSet(TCPIP_NET_HANDLE hNet,int serverIndex)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(hNet);
    DNS_DCPT    *pDnsDcpt;

    pDnsDcpt = pgDnsDcpt;
    pDnsDcpt->pDNSNet = pNetIf;
    if(pDnsDcpt->pDNSNet == NULL) 
    {
        return;
    }
    if(serverIndex>=TCPIP_DNS_MAX_SERVERS_SUPPORTED) 
    {
        return;
    }
    
    TCPIP_DNS_UpdateDNSServerAddressDesc(pNetIf);
    // get the valid address or one of the valid server address
    pDnsDcpt->vDNSServerIx = serverIndex;

    // set the interface destination server address
    TCPIP_UDP_Bind(pDnsDcpt->dnsSocket, IP_ADDRESS_TYPE_IPV4, TCPIP_DNS_CLIENT_PORT,(IP_MULTI_ADDRESS*)&pDnsDcpt->pDNSNet->netIPAddr);
    TCPIP_UDP_DestinationIPAddressSet(pDnsDcpt->dnsSocket, pDnsDcpt->dnsIpAddressType,pDnsDcpt->DNSServers + pDnsDcpt->vDNSServerIx);
}

// returns true if the pIf can be selected for DNS traffic
// false otherwise
static bool TCPIP_DNS_ValidateIf(TCPIP_NET_IF* pIf)
{
    // check that DNS is enabled
    if(TCPIP_DNS_IsEnabled(pIf))
    {
        // check that interface is up and linked
        if(_TCPIPStackHandleToNetLinked(pIf) != 0)
        {
            // check for a valid address
            if(!_TCPIPStackIsConfig(pIf) && _TCPIPStackNetAddress(pIf) != 0)
            {
                if(!TCPIP_DNS_ValidateServerAddress(pIf)) return false;
                return true;
            }
        }
    }
    return false;
}

static bool TCPIP_DNS_ValidateServerAddress(TCPIP_NET_IF* pIf)
{
  // check if the DNS server address is not zero
    if((pIf->PrimaryDNSServer.Val == 0) && (pIf->SecondaryDNSServer.Val == 0)
            &&(pIf->DefaultDNSServer.Val == 0) &&(pIf->DefaultDNSServer2.Val == 0))
        return false;
    return true;
}

static bool TCPIP_DNS_SelectDNSIntf(bool startWithDefault)
{
    int             ix;
    DNS_DCPT*       pDnsDcpt = pgDnsDcpt;
    TCPIP_NET_IF*   pDnsIf = 0;
    int             nIfs = TCPIP_STACK_NumberOfNetworksGet();

    while(true)
    {
        // start with the already known interface
        if(TCPIP_DNS_ValidateIf(pDnsDcpt->pDNSNet))
        {   // done, this is a valid one
            pDnsIf = pDnsDcpt->pDNSNet;
            break;
        }

        if(startWithDefault == true)
        {
            TCPIP_NET_IF* pDefIf = (TCPIP_NET_IF *)TCPIP_STACK_NetDefaultGet();
            if(TCPIP_DNS_ValidateIf(pDefIf))
            {   // done, this is a valid one
                pDnsIf = pDefIf;
                break;
            }
        }

        // start iterating over the interfaces
        for(ix = 0; ix < nIfs; ix++)
        {
            TCPIP_NET_IF* pNetIf = (TCPIP_NET_IF*)TCPIP_STACK_IndexToNet(ix);
            if(TCPIP_DNS_ValidateIf(pNetIf))
            {
                pDnsIf = pNetIf;
                break;
            }
        }

        break;
    }


    if(pDnsIf != 0)
    {   // update the pDnsDcpt->DNSServers[] and store the used interface
        TCPIP_DNS_ActiveDNSServerIntfSet((TCPIP_NET_HANDLE)pDnsIf, 0);
        return true;
    }

    // couldn't find a valid interface
    return false;
}

static bool _DNS_GetResultProcessing(void)
{
    DNS_HEADER		DNSHeader;
    DNS_DCPT*		pDnsDcpt;
    DNS_HASH_ENTRY* 	dnshE;
    int 		replyPktSize=0,getBufferPktSize=0;
    uint32_t 		tempTimeOut=0;
    uint8_t             getBuffer[512];
    TCPIP_DNS_DATA      udpGetBufferData;
    
    pDnsDcpt = pgDnsDcpt;
    while(true)
    {
        replyPktSize = TCPIP_UDP_GetIsReady(pDnsDcpt->dnsSocket);
        if(!replyPktSize)
        {
            tempTimeOut = (SYS_TMR_TickCountGet() - pDnsDcpt->stateStartTime)/SYS_TMR_TickCounterFrequencyGet();
            if(tempTimeOut >= TCPIP_DNS_CLIENT_SERVER_TMO)
            {
                pDnsDcpt->smState = DNS_FAIL_SERVER;
            }
            return false;
        }
        pDnsDcpt->stateStartTime = SYS_TMR_TickCountGet();
        memset(getBuffer,0,sizeof(getBuffer));
        if(replyPktSize > sizeof(getBuffer))
        {
            replyPktSize = sizeof(getBuffer);
        }
        udpGetBufferData.head = getBuffer;
        udpGetBufferData.wrPtr = udpGetBufferData.head;
        udpGetBufferData.endPtr = udpGetBufferData.head+replyPktSize;
        // Get Compleate Array of DNS Reply bytes
        getBufferPktSize = TCPIP_UDP_ArrayGet(pDnsDcpt->dnsSocket,udpGetBufferData.head,replyPktSize);
        if(getBufferPktSize < replyPktSize)
        {
             TCPIP_UDP_Discard(pDnsDcpt->dnsSocket);
             return false;
        }
// Retrieve the DNS header and de-big-endian it
        memcpy((void*)&DNSHeader,udpGetBufferData.wrPtr,sizeof(DNSHeader));
// Shift get pointer to the next poniter to get the new value
        udpGetBufferData.wrPtr = udpGetBufferData.wrPtr + sizeof(DNSHeader);
// Swap DNS Header received packet
        _SwapDNSPacket(&DNSHeader);

// find DNS HASH entry from transaction ID
        dnshE = _DNSHashEntryFromTransactionId(DNSHeader.TransactionID);

// Throw this packet away if it isn't in response to our last query
// If the Entry is already exist , No new Resolved address is required to be added
// No Such Name Return
        if((dnshE == 0)||
           (dnshE->hEntry.flags.value & DNS_FLAG_ENTRY_COMPLETE))
        {
            TCPIP_UDP_Discard(pDnsDcpt->dnsSocket);
            break;
        }
        // No such name
        if(DNSHeader.Flags.v[0]&0x03)
        {// Remove from the Hash table if response has "No Such name"
            TCPIP_DNS_RemoveEntry((char*)dnshE->pHostName);
            TCPIP_UDP_Discard(pDnsDcpt->dnsSocket);
            break;
        }
// Remove all questions (queries)
        while(DNSHeader.Questions.Val--)
        {
            _DNSDiscardName(&udpGetBufferData);
            // Ignore Question Type and Question class
            TCPIP_DNS_GetArrayOfDataFromUDPBuff(&udpGetBufferData,4,0); // Question type class
        }
// Scan through answers
        while(DNSHeader.Answers.Val--)
        {
            if(_DNS_RESPONSE_HashEntryUpdate(&udpGetBufferData,pDnsDcpt,dnshE)!= DNS_RES_OK)
                break;
        }
        // Remove all Authoritative Records
        while(DNSHeader.AuthoritativeRecords.Val--)
        {
            if(_DNS_RESPONSE_HashEntryUpdate(&udpGetBufferData,pDnsDcpt,dnshE)!= DNS_RES_OK)
                break;
        }
        // Remove all Additional Records
        while(DNSHeader.AdditionalRecords.Val--)
        {
            if(_DNS_RESPONSE_HashEntryUpdate(&udpGetBufferData,pDnsDcpt,dnshE)!= DNS_RES_OK)
                break;
        }
        if((dnshE->nIPv4Entries>0)||(dnshE->nIPv6Entries>0))
        {
            _DNSSetHashEntry(dnshE,DNS_FLAG_ENTRY_COMPLETE);
            _DNSNotifyClients(0,TCPIP_DNS_EVENT_NAME_RESOLVED,dnshE->pHostName);
        }           
        TCPIP_UDP_Discard(pDnsDcpt->dnsSocket);
    }
    return false;
}

void TCPIP_DNS_ClientTask(void)
{
    TCPIP_MODULE_SIGNAL sigPend;

    sigPend = _TCPIPStackModuleSignalGet(TCPIP_THIS_MODULE_ID, TCPIP_MODULE_SIGNAL_MASK_ALL);


    if(sigPend != 0)
    { // signal: TMO/RX occurred
        TCPIP_DNS_ClientProcess();
    }

}

// send a signal to the DNS module that data is available
// no manager alert needed since this normally results as a higher layer (UDP) signal
static void _DNSSocketRxSignalHandler(UDP_SOCKET hUDP, TCPIP_NET_HANDLE hNet, TCPIP_UDP_SIGNAL_TYPE sigType, const void* param)
{
    if(sigType == TCPIP_UDP_SIGNAL_RX_DATA)
    {
        _TCPIPStackModuleSignalRequest(TCPIP_THIS_MODULE_ID, TCPIP_MODULE_SIGNAL_RX_PENDING, true); 
    }
}



static void TCPIP_DNS_ClientProcess(void)
{
    DNS_DCPT*		pDnsDcpt;
  
    pDnsDcpt = pgDnsDcpt;
 // if the UDP socket is not created and DNS module is not enabled, then don't process the DNS task
    if(!pDnsDcpt->enabled)
    {
        return;
    }
// DNS client Hash Entry Timout is done for this API.
    TCPIP_DNS_CacheTimeout();

    switch(pDnsDcpt->smState)
    {
        case DNS_IDLE:            
            break;	// nothing to do
        case DNS_START_SERVICE:
            // 1. Open a new UDP socket if the descriptor socket is invalid. Descriptor socket is invalid if and onlyif there is a unavailable of buffer or
            // available of socket foe that interface.
            // 2. Interface is calculated from the DNS_START and inetrface address is saved for further use
            // 3. There are different itterations possible. One is interface selection dn each interface may have miltiple DNS servers
            // so select a valid and NDS enabled Network interface and configure pDnsDcpt->DNSServers[]. At present we have only two DNS servers.
            // user can use more than one dns servers.
            // 4. DNS_START is used to select the Network interface and configure pDnsDcpt->DNSServers[]. it is a for loop. Tries to get first valid interface.
            // and stores that interface to pDnsDcpt->pDNSNet. if there are more than one inetrface, then for the next valid interface, netIx starts from the
            // (pDnsDcpt->pDNSNet interface index) +1.
            // 5. DNS_START_SERVICE will open the socket. if the socket opening fails for first DNS server Address , then try with next DNS server address
            // and next server index is updated in _DNSRetry().
            // if the Interface got changed( i,e pDnsDcpt->pDNSNet is assigned with new active interface) , then no need to create the socket again, instead of , do a TCPIP_UDP_SocketNetSet().
            // if _DNSRetry() fails, then state changed to DNS_FAIL_SERVER_TMO. DNS_FAIL_SERVER_TMO state chaneges state to DNS_START.
// and bind to the DHCP local port
            if(!TCPIP_DNS_SelectDNSIntf(false))
            {
                return;
            }

            pDnsDcpt->stateStartTime = SYS_TMR_TickCountGet();
            pDnsDcpt->smState = DNS_GET_RESULT;
        // no break, waiting the response of the query;
            break;
        case DNS_GET_RESULT:
            // process a DNS Query response packet
            _DNS_GetResultProcessing();
            break;	// done	
        case DNS_FAIL_SERVER:
            // Change the destination address to the new DNS server address.that is use secondary DNS server address
            pDnsDcpt->smState = _DNSRetry(DNS_FAIL_SERVER);
            if(pDnsDcpt->smState != DNS_FAIL_SERVER_TMO)
            {
                TCPIP_UDP_DestinationIPAddressSet(pDnsDcpt->dnsSocket, pDnsDcpt->dnsIpAddressType,pDnsDcpt->DNSServers + pDnsDcpt->vDNSServerIx);
                pDnsDcpt->stateStartTime = SYS_TMR_TickCountGet();
                pDnsDcpt->smState = DNS_GET_RESULT;
            }
            break;
        case DNS_FAIL_SERVER_TMO:		// DNS server TMO
            // if their is no valid address for this present interface , then use a next valid interface
            pDnsDcpt->smState = DNS_START_SERVICE;
            break;
        default:	// DNS_DONE
            // either done or some error state
            break;
    }    
    return ;

}

// see if we can perform a retry
static DNS_STATE _DNSRetry(DNS_STATE currState)
{
    DNS_DCPT    *pDnsDcpt=pgDnsDcpt;
    
    if(pDnsDcpt == NULL) 
    {
        return DNS_RES_NO_SERVICE;
    }

    if(currState == DNS_START)
    {
        pDnsDcpt->vDNSServerIx = 0;
    }
    else
    {
        pDnsDcpt->vDNSServerIx++;
    }
    
    if(pDnsDcpt->dnsIpAddressType == IP_ADDRESS_TYPE_IPV4)
    {
        for( ; pDnsDcpt->vDNSServerIx < TCPIP_DNS_MAX_SERVERS_SUPPORTED; pDnsDcpt->vDNSServerIx++)
        {   // can try another server if valid address
            if(pDnsDcpt->DNSServers[pDnsDcpt->vDNSServerIx].v4Add.Val != 0)
            {
                return DNS_START_SERVICE;   // new state
            }
        }
    }
    else
    {
    }

    // default: DNS_FAIL_SERVER
    return DNS_FAIL_SERVER_TMO;
}

/*****************************************************************************
  Function:
	static void _DNSPutString(TCPIP_DNS_DATA **putbuf, const char* String)

  Summary:
	Writes a string to the DNS dynamic allocated buffer which will be used while pouplating UDP 
	Buffer.
	
  Description:
	This function writes a string to the DNS dynamic allocated buffer, ensuring that it is
	properly formatted.

  Precondition:
	DNS dynamic allocated buffer .

  Parameters:
	String - the string to write to the DNS dynamic allocated buffer.

  Returns:
  	None
  ***************************************************************************/
static void _DNSPutString(uint8_t **wrPtr, const char* String)
{
    const char *RightPtr;
    uint8_t i;
    uint8_t Len;
    uint8_t nullTeminator;
    uint8_t *pPutDnsStr = *wrPtr;

    RightPtr = String;

    while(1)
    {
        do
        {
            i = *RightPtr++;
        } while((i != 0x00u) && (i != '.') && (i != '/') && (i != ',') && (i != '>'));

        // Put the length and data
        // Also, skip over the '.' in the input string
        Len = (uint8_t)(RightPtr-String-1);
        memcpy(pPutDnsStr,&Len,1);
        pPutDnsStr = pPutDnsStr+1;

        memcpy(pPutDnsStr,String,Len);
        pPutDnsStr = pPutDnsStr+Len;

        String += Len + 1;

        if(i == 0x00u || i == '/' || i == ',' || i == '>')
                break;
    }

    // Put the string null terminator character (zero length label)
    nullTeminator = 0x00;
    memcpy(pPutDnsStr,&nullTeminator,1);
    pPutDnsStr = pPutDnsStr+1;
    *wrPtr = pPutDnsStr;
}


/*****************************************************************************
  Function:
	static void DNSDiscardName(DNS_DCPT *pDnsDcpt)

  Summary:
	Reads a name string or string pointer from the DNS socket and discards it.
	
  Description:
	This function reads a name string from the DNS socket.  Each string 
	consists of a series of labels.  Each label consists of a length prefix 
	byte, followed by the label bytes.  At the end of the string, a zero length 
	label is found as termination.  If name compression is used, this function 
	will automatically detect the pointer and discard it.

  Precondition:
	UDP socket is obtained and ready for reading a DNS name

  Parameters:
	None

  Returns:
  	None
  ***************************************************************************/
static void _DNSDiscardName(TCPIP_DNS_DATA *getBuf)
{
    uint32_t len=0;

    while(1)
    {
        // Get first byte which will tell us if this is a 16-bit pointer or the
        // length of the first of a series of labels
        len = TCPIP_DNS_GetDataFromUDPBuff(getBuf);
        if(!len)
        {
            return;
        }
		
        // Check if this is a pointer, if so, get the remaining 8 bits and return
        if((len & 0xC0u) == 0xC0u)
        {
            TCPIP_DNS_GetDataFromUDPBuff(getBuf);
            return;
        }

        // Exit once we reach a zero length label
        if(len == 0u)
        {
            return;
        }

        // Ignore these bytes
        TCPIP_DNS_GetArrayOfDataFromUDPBuff(getBuf,len,0);
    }
}

static size_t TCPIP_DNS_OAHASH_KeyHash(OA_HASH_DCPT* pOH, void* key)
{
    uint8_t    *dnsHostNameKey;
    size_t      hostnameLen=0;

    dnsHostNameKey = (uint8_t *)key;
    hostnameLen = strlen((const char*)dnsHostNameKey);
    return fnv_32_hash(dnsHostNameKey, hostnameLen) % (pOH->hEntries);
}


static OA_HASH_ENTRY* TCPIP_DNS_OAHASH_DeleteEntry(OA_HASH_DCPT* pOH)
{
    OA_HASH_ENTRY*  pBkt;
    size_t      bktIx;
    DNS_HASH_ENTRY  *pE;
    DNS_DCPT        *pDnsDcpt;
    uint32_t        getTime=0;
    
    pDnsDcpt = pgDnsDcpt;
    // make it to seconds
    getTime = SYS_TMR_TickCountGet()/SYS_TMR_TickCounterFrequencyGet();
    for(bktIx = 0; bktIx < pOH->hEntries; bktIx++)
    {
        pBkt = TCPIP_OAHASH_EntryGet(pOH, bktIx);		
        if(pBkt->flags.busy != 0)
        {
            pE = (DNS_HASH_ENTRY*)pBkt;
            if(pDnsDcpt->cacheEntryTMO > 0)
            {
                if((getTime-pE->tInsert) > pDnsDcpt->cacheEntryTMO)
                {
                    _DNSNotifyClients(0,TCPIP_DNS_EVENT_CACHE_EXPIRED,pE->pHostName);
                    return pBkt;
                }
            }
            else if((getTime-pE->tInsert) > pE->ipTTL.Val)
            {
                _DNSNotifyClients(0,TCPIP_DNS_EVENT_CACHE_EXPIRED,pE->pHostName);
                return pBkt;
            }
        }
    }
    return 0;
}


static int TCPIP_DNS_OAHASH_KeyCompare(OA_HASH_DCPT* pOH, OA_HASH_ENTRY* hEntry, void* key)
{
    DNS_HASH_ENTRY  *pDnsHE;
    uint8_t         *dnsHostNameKey;

  
    pDnsHE =(DNS_HASH_ENTRY  *)hEntry;
    dnsHostNameKey = (uint8_t *)key;    
    
    return strcmp((const char*)pDnsHE->pHostName,(const char*)dnsHostNameKey);
}

static void TCPIP_DNS_OAHASH_KeyCopy(OA_HASH_DCPT* pOH, OA_HASH_ENTRY* dstEntry, void* key)
{
    uint8_t    *dnsHostNameKey;
    DNS_HASH_ENTRY  *pDnsHE;
    size_t          hostnameLen=0;

    if(key==NULL) 
    {
        return;
    }
    
    pDnsHE =(DNS_HASH_ENTRY  *)dstEntry;
    dnsHostNameKey = (uint8_t *)key;
    hostnameLen = strlen((const char*)dnsHostNameKey);
    if(hostnameLen>TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN) 
    {
        return;
    }

    if(dnsHostNameKey)
    {
        if(pDnsHE->pHostName == NULL) return;
        memset(pDnsHE->pHostName,0,TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN);
        memcpy(pDnsHE->pHostName,dnsHostNameKey,hostnameLen);
        pDnsHE->pHostName[hostnameLen]='\0';
    }
}

#if defined(OA_DOUBLE_HASH_PROBING)
static size_t TCPIP_DNS_OAHASH_ProbeHash(OA_HASH_DCPT* pOH, void* key)
{
    uint8_t    *dnsHostNameKey;
    size_t      hostnameLen=0;
    
    dnsHostNameKey = (uint8_t  *)key;
    hostnameLen = strlen(dnsHostNameKey);
    return fnv_32a_hash(dnsHostNameKey, hostnameLen) % (pOH->hEntries);
}
#endif  // defined(OA_DOUBLE_HASH_PROBING)

// Register an DNS event handler
// Use hNet == 0 to register on all interfaces available
// Returns a valid handle if the call succeeds,
// or a null handle if the call failed.
// Function has to be called after the DNS is initialized
// The hParam is passed by the client and will be used by the DNS when the notification is made.
// It is used for per-thread content or if more modules, for example, share the same handler
// and need a way to differentiate the callback.
TCPIP_DNS_HANDLE TCPIP_DNS_HandlerRegister(TCPIP_NET_HANDLE hNet, TCPIP_DNS_EVENT_HANDLER handler, const void* hParam)
{
    DNS_DCPT* pDnsDcpt = pgDnsDcpt;

    if(pDnsDcpt && handler && pDnsDcpt->memH)
    {
        DNS_LIST_NODE* newNode = (DNS_LIST_NODE*)TCPIP_Notification_Add(&dnsRegisteredUsers, pDnsDcpt->memH, sizeof(*newNode));
        if(newNode)
        {
            newNode->handler = handler;
            newNode->hParam = hParam;
            newNode->hNet = hNet;
            return newNode;
        }
    }
    return 0;
}

// deregister the event handler
bool TCPIP_DNS_HandlerDeRegister(TCPIP_DNS_HANDLE hDns)
{
    DNS_DCPT* pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt && pDnsDcpt->memH && hDns)
    {
        if(TCPIP_Notification_Remove((SGL_LIST_NODE*)hDns, &dnsRegisteredUsers, pDnsDcpt->memH))
        {
            return true;
        }
    }
    return false;
}

static void _DNSNotifyClients(TCPIP_NET_IF* pNetIf, TCPIP_DNS_EVENT_TYPE evType,void *param)
{
    DNS_LIST_NODE* dNode;

    TCPIP_Notification_Lock(&dnsRegisteredUsers);
    for(dNode = (DNS_LIST_NODE*)dnsRegisteredUsers.list.head; dNode != 0; dNode = dNode->next)
    {
        if(dNode->hNet == 0 || dNode->hNet == pNetIf)
        {   // trigger event
            if(param != 0)
            {
                if(strcmp((char*)param,dNode->hParam)==0)
                {
                    (*dNode->handler)(pNetIf, evType, dNode->hParam);
                }
            }
            else
            {
                (*dNode->handler)(pNetIf, evType, dNode->hParam);
            }
        }
    }    
    TCPIP_Notification_Unlock(&dnsRegisteredUsers);
}

bool TCPIP_DNS_IsEnabled(TCPIP_NET_HANDLE hNet)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(hNet);
    if(pNetIf)
    {
        return pNetIf->Flags.bIsDnsClientEnabled!= 0;
    }
    return false;
}

bool TCPIP_DNS_Enable(TCPIP_NET_HANDLE hNet)
{
    return _DNS_Enable(hNet, true);
}

static bool _DNS_Enable(TCPIP_NET_HANDLE hNet, bool checkIfUp)
{
    DNS_DCPT        *pDnsDcpt;
    uint16_t        bufferSize;
    TCPIP_NET_IF    *pNetIf;

    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0)
    {
        return false;
    }

    if(checkIfUp)
    {
        pNetIf = _TCPIPStackHandleToNetUp(hNet);
    }
    else
    {
        pNetIf = _TCPIPStackHandleToNet(hNet);
    }

    if(pNetIf == 0 || TCPIP_STACK_DNSServiceCanStart(pNetIf, TCPIP_STACK_SERVICE_DNSC) == false)
    {
        return false;
    }

    pNetIf->Flags.bIsDnsClientEnabled = true;      
    // open Client socket
    if(pDnsDcpt->dnsSocket == INVALID_UDP_SOCKET)
    {
        pDnsDcpt->dnsSocket = TCPIP_UDP_ClientOpen(pDnsDcpt->dnsIpAddressType,
                    TCPIP_DNS_CLIENT_PORT, pDnsDcpt->DNSServers + pDnsDcpt->vDNSServerIx);
        bufferSize = TCPIP_UDP_TxPutIsReady(pDnsDcpt->dnsSocket, 18+TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN+1);
        if(bufferSize < (18+TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN+1))
        {
            TCPIP_UDP_OptionsSet(pDnsDcpt->dnsSocket, UDP_OPTION_TX_BUFF, (void*)(unsigned int)(18+TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN+1));
        }
        TCPIP_UDP_OptionsSet(pDnsDcpt->dnsSocket, UDP_OPTION_STRICT_NET, (void*)false);
        TCPIP_UDP_OptionsSet(pDnsDcpt->dnsSocket, UDP_OPTION_STRICT_ADDRESS, (void*)false);
        TCPIP_UDP_SignalHandlerRegister(pDnsDcpt->dnsSocket, TCPIP_UDP_SIGNAL_RX_DATA, _DNSSocketRxSignalHandler, 0);
        pDnsDcpt->enabled = true;
        pDnsDcpt->pDNSNet =  pNetIf;
        pDnsDcpt->smState = DNS_START_SERVICE;
    }
    return true;
}

bool TCPIP_DNS_Disable(TCPIP_NET_HANDLE hNet)
{
    TCPIP_NET_IF* pNetIf = (TCPIP_NET_IF*)_TCPIPStackHandleToNetUp(hNet);
    DNS_DCPT *pDnsDcpt;
    int       netIx=0;
    bool ifDnsServiceIsEnabled=false;

    pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0 || pNetIf == 0)
    {
        return false;
    }

    pNetIf->Flags.bIsDnsClientEnabled = false;
    for(netIx = 0;netIx < TCPIP_STACK_NumberOfNetworksGet(); netIx++)
    {
        pNetIf = (TCPIP_NET_IF*)TCPIP_STACK_IndexToNet(netIx);
        if(pNetIf->Flags.bIsDnsClientEnabled)
        {
            ifDnsServiceIsEnabled=true;
            break;
        }
    }
    // if there is any interface , where DNS service is enabled, then don't release socket
    if(!ifDnsServiceIsEnabled)
    {
        pDnsDcpt->smState = DNS_IDLE;
        _DNSReleaseSocket(pDnsDcpt);
        pDnsDcpt->enabled = false;
        return true;
    }

    // change the DNS service interface
    TCPIP_DNS_SelectDNSIntf(false);
    return true;	
}

bool TCPIP_DNS_SwitchDNSServerInterface(TCPIP_NET_HANDLE hNet,int ServerIpIndex)
{
    DNS_DCPT* pDnsDcpt = pgDnsDcpt;
    if(pDnsDcpt == 0)
    {
        return false;
    }

    TCPIP_NET_IF* pNetIf = (TCPIP_NET_IF*)_TCPIPStackHandleToNetUp(hNet);
    if(!TCPIP_DNS_ValidateIf(pNetIf))
    {
        return false;
    }

    TCPIP_DNS_ActiveDNSServerIntfSet((TCPIP_NET_HANDLE)pNetIf,ServerIpIndex);

    return true;
}

#if defined (TCPIP_STACK_USE_DHCP_CLIENT)
static void DNS_DhcpNotify(TCPIP_NET_HANDLE hNet, uint8_t evType, const void * param)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(hNet);
    DNS_DCPT* pDnsDcpt = pgDnsDcpt;

    if(pDnsDcpt == NULL || pNetIf == NULL)
    {
       return;
    }

    if((pDnsDcpt->memH) && (pDnsDcpt->enabled)&& (pDnsDcpt->pDNSNet != NULL))
    {       
        if((pDnsDcpt->pDNSNet == pNetIf) && (evType == DHCP_EVENT_BOUND ))
        {
           TCPIP_DNS_ActiveDNSServerIntfSet(pNetIf,0);
        }
    }   
}
#endif
#else
bool TCPIP_DNS_IsEnabled(TCPIP_NET_HANDLE hNet){return false;}
bool TCPIP_DNS_Enable(TCPIP_NET_HANDLE hNet){return false;}
bool TCPIP_DNS_Disable(TCPIP_NET_HANDLE hNet){return false;}

TCPIP_DNS_RESULT  TCPIP_DNS_Resolve(const char* HostName, DNS_RESOLVE_TYPE Type)
{
    return DNS_RES_NO_SERVICE; 
}

TCPIP_DNS_RESULT  TCPIP_DNS_IsResolved(const char* HostName, void* HostIP)
{
    return DNS_RES_NO_SERVICE; 
}
TCPIP_DNS_RESULT TCPIP_DNS_RemoveEntry(const char *pDnsHostName)
{
    return DNS_RES_NO_SERVICE;
}
TCPIP_DNS_RESULT TCPIP_DNS_RemoveAll(void)
{
    return DNS_RES_NO_SERVICE;
}
TCPIP_DNS_RESULT TCPIP_DNS_GetDNSClientConnectionInfo(IPV4_ADDR *Addr1,IPV4_ADDR *Addr2,int *intfIndex,bool *moduleOnOff)
{
    return DNS_RES_NO_SERVICE;
}
TCPIP_DNS_RESULT TCPIP_DNS_GetResolverDetails(char *pDnsHostName,TCPIP_DNS_RESOLVED_ADDRESS *ipAddr,uint32_t *ttlTime,uint32_t len,int *index)
{
    return DNS_RES_NO_SERVICE;
}

#endif	//#if defined(TCPIP_STACK_USE_DNS)

