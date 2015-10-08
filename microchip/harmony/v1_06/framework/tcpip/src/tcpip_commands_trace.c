/*******************************************************************************
  TCP/IP commands implementation

  Summary:
    Module for Microchip TCP/IP Stack
    
  Description:
    TCPIP stack commands entities. 
    Note, this module is based on system command parser
*******************************************************************************/

/*******************************************************************************
File Name:  tcpip_commands.c
Copyright ©2012 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS?WITHOUT WARRANTY OF ANY KIND,
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
#define TCPIP_THIS_MODULE_ID    TCPIP_MODULE_COMMAND

#include "tcpip/src/tcpip_private.h"
#include "tcpip/tcpip_manager.h"

#if defined(TCPIP_STACK_COMMAND_ENABLE)


#if defined(TCPIP_STACK_USE_ICMP_CLIENT) || defined(TCPIP_STACK_USE_DNS)
#define _TCPIP_STACK_COMMAND_TASK
#endif  // defined(TCPIP_STACK_USE_ICMP_CLIENT) || defined(TCPIP_STACK_USE_DNS)



static int  initialNetIfs = 0;    // Backup interfaces number for stack restart
static TCPIP_HEAP_HANDLE       initialHeapH = 0;

static TCPIP_STACK_INIT        cmdTcpipInitData;        // data that's used for the StackInit
static TCPIP_STACK_INIT*       pCmdTcpipInitData = 0;   // pointer to this data

#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)

typedef struct
{
    size_t                  stgSize;        // size  + valid flag
    TCPIP_STACK_NET_IF_DCPT netDcptStg;     // configuration save
#if defined(TCPIP_STACK_USE_IPV6)
    uint8_t                 restoreBuff[sizeof(TCPIP_NETWORK_CONFIG) + 170]; // buffer to restore the configuration
#else
    uint8_t                 restoreBuff[sizeof(TCPIP_NETWORK_CONFIG) + 120]; // buffer to restore the configuration
#endif  // defined(TCPIP_STACK_USE_IPV6)
}TCPIP_COMMAND_STG_DCPT;

static TCPIP_COMMAND_STG_DCPT*   pCmdStgDcpt = 0;   // store current interface configuration
static TCPIP_NETWORK_CONFIG*     pCmdNetConf = 0;   // create the array of configurations needed for stack initialization

static bool                     tcpipCmdPreserveSavedInfo = false; // do not discard the saved data

#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)

typedef enum 
{
    DNS_SERVICE_COMD_ADD=0,
    DNS_SERVICE_COMD_DEL,
    DNS_SERVICE_COMD_INFO,
    DNS_SERVICE_COMD_SWITCH_INTERFACE,
    DNS_SERVICE_COMD_ENABLE_INTF,
    DNS_SERVICE_COMD_LOOKUP,
    DNS_SERVICE_COMD_NONE,
}DNS_SERVICE_COMD_TYPE;
typedef struct 
{
	char *command;
	DNS_SERVICE_COMD_TYPE  val;
}DNSS_COMMAND_MAP;


static int _Command_AddressService(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv, TCPIP_STACK_ADDRESS_SERVICE_TYPE svcType);


static int _Command_NetInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_DefaultInterfaceSet (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _CommandDhcpOptions(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_DHCPSOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_ZcllOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_IPAddressSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_GatewayAddressSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_PrimaryDNSAddressSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_BIOSNameSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_MACAddressSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_NetworkOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_StackOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_HeapInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _CommandArp(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_DNSOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_MacInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
#if defined(TCPIP_STACK_USE_TFTP_CLIENT)
static int _Command_TFTPC_Service(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
#endif

#if defined(TCPIP_STACK_USE_DHCP_SERVER)
static int _Command_DHCPLeaseInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
#endif  //  defined(TCPIP_STACK_USE_DHCP_SERVER)
#if defined(TCPIP_STACK_USE_DNS)
static int _Command_DNS_Service(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_ShowDNSResolvedInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_ConfigDNSServer(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
#endif
#if defined(TCPIP_STACK_USE_DNS_SERVER)
static int _Command_DNSSOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_AddDelDNSSrvAddress(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv,DNS_SERVICE_COMD_TYPE dnsCommand);
static int _Command_ShowDNDServInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_DnsServService(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
#endif

#if defined(TCPIP_UDP_DEBUG)
static int _Command_UdpInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
#endif  // defined(TCPIP_UDP_DEBUG)

#if defined(TCPIP_TCP_DEBUG)
static int _Command_TcpInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
#endif  // defined(TCPIP_TCP_DEBUG)

#if defined(TCPIP_PACKET_TRACE_ENABLE)
static int _Command_PktInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
#endif  // defined(TCPIP_PACKET_TRACE_ENABLE)

#if defined(TCPIP_PACKET_FLIGHT_LOG_ENABLE)
static int _Command_PktFlightFlag(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_PktFlightType(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
#endif  // defined(TCPIP_PACKET_FLIGHT_LOG_ENABLE)

#if defined(TCPIP_STACK_TIME_MEASUREMENT)
static int _Command_StackExecTime(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
void TCPIP_Commands_ExecTimeUpdate(void);
#endif // defined(TCPIP_STACK_TIME_MEASUREMENT)

#if defined(TCPIP_STACK_USE_TFTP_CLIENT)
static char tftpServerHost[TCPIP_TFTPC_HOSTNAME_LEN];     // current target server
static char tftpcFileName[TCPIP_TFTPC_FILENAME_LEN]; // TFTP file name that will be for PUT and GET command
#endif

#if defined(_TCPIP_STACK_COMMAND_TASK)
// command task status
typedef enum
{
    TCPIP_CMD_STAT_IDLE = 0,        // command task is idle

    // ping related status
    TCPIP_CMD_STAT_PING_START,      // starting ping commands

    TCPIP_PING_CMD_DNS_GET = TCPIP_CMD_STAT_PING_START,     // get DNS
    TCPIP_PING_CMD_DNS_WAIT,        // wait for DNS
    TCPIP_PING_CMD_DO_PING,         // send pings
    TCPIP_PING6_CMD_DNS_GET,        // send pings
    TCPIP_PING6_CMD_DNS_WAIT,       // wait for DNS    
    TCPIP_SEND_ECHO_REQUEST_IPV6,   // send IPv6 ping request

    TCPIP_CMD_STAT_PING_STOP = TCPIP_SEND_ECHO_REQUEST_IPV6,       // stop ping commands

    // DNS related status
    TCPIP_CMD_STAT_DNS_START,                               // starting DNS commands

    TCPIP_DNS_LOOKUP_CMD_GET = TCPIP_CMD_STAT_DNS_START,    // get DNS
    TCPIP_DNS_LOOKUP_CMD_WAIT,                              // wait for DNS

    TCPIP_CMD_STAT_DNS_STOP = TCPIP_DNS_LOOKUP_CMD_WAIT,    // stop DNS commands

}TCPIP_COMMANDS_STAT;

static SYS_CMD_DEVICE_NODE* pTcpipCmdDevice = 0;
static tcpipSignalHandle     tcpipCmdSignalHandle = 0;      // tick handle


static TCPIP_COMMANDS_STAT  tcpipCmdStat = TCPIP_CMD_STAT_IDLE;




#endif  // defined(_TCPIP_STACK_COMMAND_TASK)


#if defined(TCPIP_STACK_USE_DNS)
static char                 dnslookupTargetHost[31];     // current target host name
static DNS_RESOLVE_TYPE     dnsType=DNS_TYPE_A;
static const void*          dnsLookupCmdIoParam = 0;
static IP_MULTI_ADDRESS     dnsLookUpAddr;         // current target address
static uint32_t             dnsLookUpStartTick;

static int                  _Command_DNSLookUP(SYS_CMD_DEVICE_NODE* pCmdIO, char** argv);

static void                 TCPIPCmdDnsTask(void);
#endif

#if defined(TCPIP_STACK_USE_ICMP_CLIENT)

#define TCPIP_COMMAND_ICMP_ECHO_REQUEST_MIN_DELAY 5  // minimum delay between successive echo requests


static int                  _CommandPing(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void                 CommandPingHandler(TCPIP_NET_HANDLE hNetIf, IPV4_ADDR * remoteIP, void * data);

static void                 TCPIPCmdPingTask(void);

#if defined(TCPIP_STACK_USE_IPV6)
static int                  _Command_IPv6_Ping(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void                 CommandPing6Handler(TCPIP_NET_HANDLE hNetIf,uint8_t type, const IPV6_ADDR * localIP,
                                                                    const IPV6_ADDR * remoteIP, void * data);
static char                 icmpv6TargetAddrStr[42];
static uint32_t             pingPktSize=0;
static IPV6_ADDR            icmpv6TargetAddr;
static ICMPV6_HANDLE        hIcmpv6 = 0;
#endif
static void                 _PingStop(SYS_CMD_DEVICE_NODE* pCmdIO, const void* cmdIoParam);

static char                 icmpTargetHost[31];     // current target host name
static char                 icmpTargetAddrStr[17];  // current target address string
static IPV4_ADDR            icmpTargetAddr;         // current target address
static uint16_t             icmpSequenceNo;         // current sequence number
static uint16_t             icmpIdentifier;         // current ID number

static int                  icmpReqNo;              // number of requests to send
static int                  icmpReqCount;           // current request counter
static int                  icmpAckRecv;            // number of acks
static int                  icmpReqDelay;

static ICMP_HANDLE          hIcmp = 0;

static const void*          icmpCmdIoParam = 0;
uint32_t                    icmpStartTick;
static TCPIP_NET_HANDLE     icmpNetH = 0;

#endif  // defined(TCPIP_STACK_USE_ICMP_CLIENT)

#if defined(TCPIP_STACK_TIME_MEASUREMENT)
static uint32_t             xTimeStartCount = 0;
static uint64_t             xTimeTotal = 0;
static bool                 xTimeEnabled = false;
#endif // defined(TCPIP_STACK_TIME_MEASUREMENT)


// TCPIP stack command table
static const SYS_CMD_DESCRIPTOR    tcpipCmdTbl[]=
{
    {"netinfo",     _Command_NetInfo,              ": Get network information"},
    {"defnet",      _Command_DefaultInterfaceSet,  ": Set default network interface"},
    {"dhcp",        _CommandDhcpOptions,           ": DHCP client commands"},
    {"dhcps",       _Command_DHCPSOnOff,           ": Turn DHCP server on/off"},
    {"zcll",        _Command_ZcllOnOff,            ": Turn ZCLL on/off"},
    {"setip",       _Command_IPAddressSet,         ": Set IP address and mask"},
    {"setgw",       _Command_GatewayAddressSet,    ": Set Gateway address"},
    {"setdns",      _Command_PrimaryDNSAddressSet, ": Set DNS address"},
    {"setbios",     _Command_BIOSNameSet,          ": Set host's NetBIOS name"},
    {"setmac",      _Command_MACAddressSet,        ": Set MAC address"},
    {"if",          _Command_NetworkOnOff,         ": Bring an interface up/down"},
    {"stack",       _Command_StackOnOff,           ": Stack turn on/off"},
    {"heapinfo",    _Command_HeapInfo,             ": Check heap status"},
#if defined(TCPIP_STACK_USE_DHCP_SERVER)
    {"dhcpsinfo",   _Command_DHCPLeaseInfo,        ": Display DHCP Server Lease Details" },
#endif  //  defined(TCPIP_STACK_USE_DHCP_SERVER)
#if defined(TCPIP_STACK_USE_ICMP_CLIENT)
    {"ping",        _CommandPing,                  ": Ping an IP address"},
#if defined(TCPIP_STACK_USE_IPV6)
    {"ping6",       _Command_IPv6_Ping,            ": Ping an IPV6 address"},
#endif // defined  TCPIP_STACK_USE_IPV6
#endif  // defined(TCPIP_STACK_USE_ICMP_CLIENT)
    {"arp",         _CommandArp,                   ": ARP commands"},
#if defined(TCPIP_STACK_USE_DNS_SERVER)
    {"dnss",        _Command_DnsServService,       ": Dns Command Service"},
#endif
#if defined(TCPIP_STACK_USE_DNS)
    {"dnsc",        _Command_DNS_Service,          ": Remove DNS resolved entry"},
#endif
    {"macinfo",     _Command_MacInfo,              ": Check MAC statistics"},
#if defined(TCPIP_STACK_USE_TFTP_CLIENT)
    {"tftpc",   _Command_TFTPC_Service,            ": TFTP client Service"},
#endif
    
#if defined(TCPIP_UDP_DEBUG)
    {"udpinfo",   _Command_UdpInfo,                ": Check UDP statistics"},
#endif  // defined(TCPIP_UDP_DEBUG)
#if defined(TCPIP_TCP_DEBUG)
    {"tcpinfo",   _Command_TcpInfo,                ": Check TCP statistics"},
#endif  // defined(TCPIP_TCP_DEBUG)
#if defined(TCPIP_PACKET_TRACE_ENABLE)
    {"pktinfo",   _Command_PktInfo,                ": Check PKT statistics"},
#endif  // defined(TCPIP_PACKET_TRACE_ENABLE)
#if defined(TCPIP_PACKET_FLIGHT_LOG_ENABLE)
    {"pktf",      _Command_PktFlightFlag,          ": Control PKT flight flags"},
    {"pktt",      _Command_PktFlightType,          ": Control PKT flight type"},
#endif  // defined(TCPIP_PACKET_FLIGHT_LOG_ENABLE)
#if defined(TCPIP_STACK_TIME_MEASUREMENT)
    {"xtime",     _Command_StackExecTime,         ": Enable TCP/IP stack time measurement"},
#endif // defined(TCPIP_STACK_TIME_MEASUREMENT)

};

bool TCPIP_Commands_Initialize(const TCPIP_STACK_MODULE_CTRL* const stackCtrl, const TCPIP_COMMAND_MODULE_CONFIG* const pCmdInit)
{
    if(stackCtrl->stackAction == TCPIP_STACK_ACTION_IF_UP)
    {   // interface restart
        return true;
    }

    // stack init
    initialNetIfs = stackCtrl->nIfs;
    initialHeapH = stackCtrl->memH;

    // create command group
    if (!SYS_CMD_ADDGRP(tcpipCmdTbl, sizeof(tcpipCmdTbl)/sizeof(*tcpipCmdTbl), "tcpip", ": stack commands"))
    {
        SYS_ERROR(SYS_ERROR_ERROR, "Failed to create TCPIP Commands\r\n");
        return false;
    }

#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
    // get storage for interfaces configuration
    // cannot be taken from the TCPIP-HEAP because we need it persistent after
    // TCPIP_STACK_Deinit() is called!
    if(pCmdStgDcpt == 0 && pCmdNetConf == 0)
    {
        pCmdStgDcpt = (TCPIP_COMMAND_STG_DCPT*)TCPIP_STACK_CALLOC_FUNC(initialNetIfs, sizeof(*pCmdStgDcpt));
        pCmdNetConf = (TCPIP_NETWORK_CONFIG*)TCPIP_STACK_CALLOC_FUNC(initialNetIfs, sizeof(*pCmdNetConf));
        if(pCmdStgDcpt == 0 || pCmdNetConf == 0)
        {   // failure is not considered to be catastrophic
            SYS_ERROR(SYS_ERROR_WARNING, "Failed to create TCPIP Commands Storage/Config\r\n");
        }
    }
#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)

#if defined(TCPIP_STACK_USE_ICMP_CLIENT)
    hIcmp = 0;
#if defined(TCPIP_STACK_USE_IPV6)
    hIcmpv6 = 0;
#endif
    icmpAckRecv = 0;
#endif  // defined(TCPIP_STACK_USE_ICMP_CLIENT)

#if defined(_TCPIP_STACK_COMMAND_TASK)
    tcpipCmdSignalHandle =_TCPIPStackSignalHandlerRegister(TCPIP_THIS_MODULE_ID, TCPIP_COMMAND_Task, 0);
    if(tcpipCmdSignalHandle == 0)
    {   // timer is not active now
        SYS_ERROR(SYS_ERROR_ERROR, "TCPIP commands task registration failed\r\n");
        return false;
    }
    // else the timer will start when we send a query
    tcpipCmdStat = TCPIP_CMD_STAT_IDLE;
#endif  // defined(_TCPIP_STACK_COMMAND_TASK)

    return true;
}

void TCPIP_Commands_Deinitialize(const TCPIP_STACK_MODULE_CTRL* const stackCtrl)
{
    // if(stackCtrl->stackAction == TCPIP_STACK_ACTION_DEINIT) // stack shut down
    // if(stackCtrl->stackAction == TCPIP_STACK_ACTION_IF_DOWN) // interface down

    if(stackCtrl->stackAction == TCPIP_STACK_ACTION_DEINIT)
    {   // whole stack is going down
#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
        if(tcpipCmdPreserveSavedInfo == false)
        {
            TCPIP_STACK_FREE_FUNC(pCmdStgDcpt);
            TCPIP_STACK_FREE_FUNC(pCmdNetConf);
            pCmdStgDcpt = 0;
            pCmdNetConf = 0;
        }
#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)

#if defined(_TCPIP_STACK_COMMAND_TASK)
        if(tcpipCmdSignalHandle != 0)
        {
            _TCPIPStackSignalHandlerDeregister(tcpipCmdSignalHandle);
            tcpipCmdSignalHandle = 0;
        }
#endif  // defined(_TCPIP_STACK_COMMAND_TASK)

#if defined(TCPIP_STACK_USE_ICMP_CLIENT)
#if defined(TCPIP_STACK_USE_IPV6)
        if(hIcmpv6)
        {
            TCPIP_ICMPV6_CallbackDeregister(hIcmpv6);
        }
#endif
        if(hIcmp)
        {
            TCPIP_ICMP_CallbackDeregister(hIcmp);
        }
#endif  // defined(TCPIP_STACK_USE_ICMP_CLIENT)
        
    }
}

static int _Command_NetInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int i;
    TCPIP_NET_HANDLE netH;
    IPV4_ADDR ipAddr;
    const TCPIP_MAC_ADDR* pMac;
    const char  *hostName, *msgAdd;
    bool         dhcpActive;
    const void* cmdIoParam = pCmdIO->cmdIoParam;
#if defined(TCPIP_STACK_USE_IPV6)
    IPV6_ADDR_STRUCT currIpv6Add;
    IPV6_ADDR_HANDLE prevHandle, nextHandle;
    char   addrBuff[44];
#else
    char   addrBuff[20];
#endif  // defined(TCPIP_STACK_USE_IPV6)

    if (argc > 2)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: netinfo\r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: netinfo\r\n");
        return false;
    }

    for (i=0; i<initialNetIfs; i++)
    {
        netH = TCPIP_STACK_IndexToNet(i);
        TCPIP_STACK_NetAliasNameGet(netH, addrBuff, sizeof(addrBuff));
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "---------- Interface <%s/%s> ---------- \r\n", addrBuff, TCPIP_STACK_NetNameGet(netH));
        if(!TCPIP_STACK_NetIsUp(netH))
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Interface is down\r\n");
            continue;
        }
        hostName = TCPIP_STACK_NetBIOSName(netH); 
#if defined(TCPIP_STACK_USE_NBNS)
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Host Name: %s - NBNS enabled\r\n", hostName);
#else
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Host Name: %s - NBNS disabled \r\n", hostName);
#endif  // defined(TCPIP_STACK_USE_NBNS)
        ipAddr.Val = TCPIP_STACK_NetAddress(netH);
        TCPIP_Helper_IPAddressToString(&ipAddr, addrBuff, sizeof(addrBuff));
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "IPv4 Address: %s\r\n", addrBuff);

        ipAddr.Val = TCPIP_STACK_NetMask(netH);
        TCPIP_Helper_IPAddressToString(&ipAddr, addrBuff, sizeof(addrBuff));
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Mask: %s\r\n", addrBuff);

        ipAddr.Val = TCPIP_STACK_NetAddressGateway(netH);
        TCPIP_Helper_IPAddressToString(&ipAddr, addrBuff, sizeof(addrBuff));
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Gateway: %s\r\n", addrBuff);

        ipAddr.Val = TCPIP_STACK_NetAddressDnsPrimary(netH);
        TCPIP_Helper_IPAddressToString(&ipAddr, addrBuff, sizeof(addrBuff));
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "DNS: %s\r\n", addrBuff);

        pMac = (const TCPIP_MAC_ADDR*)TCPIP_STACK_NetAddressMac(netH);
        TCPIP_Helper_MACAddressToString(pMac, addrBuff, sizeof(addrBuff));
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "MAC Address: %s\r\n", addrBuff);

        // display IPv6 addresses
#if defined(TCPIP_STACK_USE_IPV6)
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "IPv6 Unicast addresses:\r\n");

        prevHandle = 0;
        do
        {
            nextHandle = TCPIP_STACK_NetIPv6AddressGet(netH, IPV6_ADDR_TYPE_UNICAST, &currIpv6Add, prevHandle);
            if(nextHandle)
            {   // have a valid address; display it
                TCPIP_Helper_IPv6AddressToString(&currIpv6Add.address, addrBuff, sizeof(addrBuff));
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "    %s\r\n", addrBuff);
                prevHandle = nextHandle;
            }
        }while(nextHandle != 0);

        if(prevHandle == 0)
        {   // no valid address
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "    Unknown\r\n");
        }
        
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "IPv6 Multicast addresses:\r\n");
        prevHandle = 0;
        do
        {
            nextHandle = TCPIP_STACK_NetIPv6AddressGet(netH, IPV6_ADDR_TYPE_MULTICAST, &currIpv6Add, prevHandle);
            if(nextHandle)
            {   // have a valid address; display it
                TCPIP_Helper_IPv6AddressToString(&currIpv6Add.address, addrBuff, sizeof(addrBuff));
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "    %s\r\n", addrBuff);
                prevHandle = nextHandle;
            }
        }while(nextHandle != 0);

        if(prevHandle == 0)
        {   // no valid address
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "    Unknown\r\n");
        }

#endif  // defined(TCPIP_STACK_USE_IPV6)

        dhcpActive = false;
        if(TCPIP_DHCP_IsActive(netH))
        {
            msgAdd = "dhcp";
            dhcpActive = true;
        }
#if defined(TCPIP_STACK_USE_ZEROCONF_LINK_LOCAL)
        else if(TCPIP_ZCLL_IsEnabled(netH))
        {
            msgAdd = "zcll";
        }
#endif
#if defined(TCPIP_STACK_USE_DHCP_SERVER)
        else if(TCPIP_DHCPS_IsEnabled(netH))
        {
            msgAdd = "dhcps";
        }
#endif  // defined(TCPIP_STACK_USE_DHCP_SERVER)
        else
        {
            msgAdd = "default IP address";
        }

        (*pCmdIO->pCmdApi->print)(cmdIoParam, "%s is ON\r\n", msgAdd);

        if(!dhcpActive)
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "dhcp is %s\r\n", TCPIP_DHCP_IsEnabled(netH) ? "enabled" : "disabled");
        }
        
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Link is %s\r\n", TCPIP_STACK_NetIsLinked(netH) ? "UP" : "DOWN");

    }
    return true;
}

#if defined(TCPIP_STACK_USE_DHCP_SERVER)
static int _Command_DHCPLeaseInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    TCPIP_DHCPS_LEASE_HANDLE  prevLease, nextLease;
    TCPIP_DHCPS_LEASE_ENTRY leaseEntry;
    char   addrBuff[20];
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 2)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dhcpsinfo <interface> \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: dhcpsinfo PIC32INT \r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface specified \r\n");
        return false;
    }

    (*pCmdIO->pCmdApi->print)(cmdIoParam,"MAC Address		IPAddress		RemainingLeaseTime \r\n",0);

    prevLease = 0;
    do
    {
        memset((void*)&leaseEntry,0,sizeof(TCPIP_DHCPS_LEASE_ENTRY));
        nextLease = TCPIP_DHCPS_LeaseEntryGet(netH, &leaseEntry, prevLease);
        if(!nextLease)
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, " \n\r No more entry present \r\n", 0);
        }
        if(nextLease)
        {   // valid info
            // display info
            TCPIP_Helper_MACAddressToString(&leaseEntry.hwAdd, addrBuff, sizeof(addrBuff));
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "%s", addrBuff);
            TCPIP_Helper_IPAddressToString(&leaseEntry.ipAddress, addrBuff, sizeof(addrBuff));
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "	%s ", addrBuff);
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "	%d Secs\r\n", leaseEntry.leaseTime/SYS_TMR_TickCounterFrequencyGet());

            prevLease = nextLease;
        }
    }while(nextLease != 0);


    return true;

}
#endif  //  defined(TCPIP_STACK_USE_DHCP_SERVER)

static int _Command_DefaultInterfaceSet (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 2)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: defnet <interface>\r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: defnet PIC32INT\r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface specified \r\n");
        return false;
    }

    if(TCPIP_STACK_NetDefaultSet(netH))
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Default interface set successful\r\n");
    }
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Operation not accepted\r\n");
    }

    return true;
}

static int _CommandDhcpOptions(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    IPV4_ADDR       reqIpAddr;
    bool            dhcpRes;
    int             opCode = 0;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc < 3)
    {
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Usage: %s <interface> <on/off/renew/request/info> \r\n", argv[0]);
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Ex: %s PIC32INT on \r\n", argv[0]);
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface\r\n");
        return false;
    }

    if (strcmp(argv[2], "on") == 0)
    {   // turning on a service
        opCode = 1;
    }
    else if (strcmp(argv[2], "off") == 0)
    {   // turning off a service
        opCode = 2;
    }
    else if (strcmp(argv[2], "renew") == 0)
    {   // DHCP renew
        opCode = 3;
    }
    else if (strcmp(argv[2], "request") == 0)
    {   // DHCP request
        opCode = 4;
        if(argc < 4)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Request needs an IP address\r\n");
            return false;
        }

        if (!TCPIP_Helper_StringToIPAddress(argv[3], &reqIpAddr))
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Invalid IP address string \r\n");
            return false;
        }
    }
    else if (strcmp(argv[2], "info") == 0)
    {   // DHCP info
        TCPIP_DHCP_INFO dhcpInfo;
        char addBuff[20];

        if(TCPIP_DHCP_InfoGet(netH, &dhcpInfo))
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "DHCP status: %d ( %d == Bound), time: %d\r\n", dhcpInfo.status, TCPIP_DHCP_BOUND, dhcpInfo.dhcpTime);
            if(dhcpInfo.status >= TCPIP_DHCP_BOUND)
            {
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "DHCP lease start: %d, duration: %ds\r\n", dhcpInfo.leaseStartTime, dhcpInfo.leaseDuration);
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "DHCP renew time: %d, rebind time: %d\r\n", dhcpInfo.renewTime, dhcpInfo.rebindTime);

                TCPIP_Helper_IPAddressToString(&dhcpInfo.dhcpAddress, addBuff, sizeof(addBuff));
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "DHCP address: %s\r\n", addBuff);
                TCPIP_Helper_IPAddressToString(&dhcpInfo.serverAddress, addBuff, sizeof(addBuff));
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "DHCP server: %s\r\n", addBuff);
            }
        }
        else
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "DHCP: failed to get info\r\n");
        }
        return false;
    }
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown option\r\n");
        return false;
    }


    switch(opCode)
    {
        case 1:
            dhcpRes = TCPIP_DHCP_Enable(netH);
            break;

        case 2:
            dhcpRes = TCPIP_DHCP_Disable(netH);
            break;

        case 3:
            dhcpRes = TCPIP_DHCP_Renew(netH);
            break;

        case 4:
            dhcpRes = TCPIP_DHCP_Request(netH, reqIpAddr);
            break;

        default:
            dhcpRes = false;
            break;
    }

    (*pCmdIO->pCmdApi->print)(cmdIoParam, "%s %s %s\r\n", argv[0], argv[2], dhcpRes ? "success" : "fail");

    return true;
}

static int _Command_DHCPSOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    return _Command_AddressService(pCmdIO, argc, argv, TCPIP_STACK_ADDRESS_SERVICE_DHCPS);
}

static int _Command_ZcllOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    return _Command_AddressService(pCmdIO, argc, argv, TCPIP_STACK_ADDRESS_SERVICE_ZCLL);
}

static int _Command_AddressService(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv, TCPIP_STACK_ADDRESS_SERVICE_TYPE svcType)
{ 
    typedef bool(*addSvcFnc)(TCPIP_NET_HANDLE hNet);

    TCPIP_NET_HANDLE netH;
    addSvcFnc        addFnc;
    bool             addRes, svcEnable;
    const char       *msgOK, *msgFail;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 3)
    {
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Usage: %s <interface> <on/off> \r\n", argv[0]);
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Ex: %s PIC32INT on \r\n", argv[0]);
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface\r\n");
        return false;
    }

    if (memcmp(argv[2], "on", 2) == 0)
    {   // turning on a service
        svcEnable = true;
    }
    else if (memcmp(argv[2], "off", 2) == 0)
    {   // turning off a service
        svcEnable = false;
    }
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown option\r\n");
        return false;
    }

    switch(svcType)
    {
        case TCPIP_STACK_ADDRESS_SERVICE_DHCPC:
            addFnc = svcEnable?TCPIP_DHCP_Enable:TCPIP_DHCP_Disable;
            break;

#if defined(TCPIP_STACK_USE_DHCP_SERVER)
        case TCPIP_STACK_ADDRESS_SERVICE_DHCPS:
            addFnc = svcEnable?TCPIP_DHCPS_Enable:TCPIP_DHCPS_Disable;
            break;
#endif  // defined(TCPIP_STACK_USE_DHCP_SERVER)

#if defined(TCPIP_STACK_USE_ZEROCONF_LINK_LOCAL)
        case TCPIP_STACK_ADDRESS_SERVICE_ZCLL:
            addFnc = svcEnable?TCPIP_ZCLL_Enable:TCPIP_ZCLL_Disable;
            break;
#endif
        default:
            addFnc = 0;     // unknown service; shouldn't happen
            break;
    }

    if(addFnc)
    {
        msgOK   = svcEnable?"enabled":"disabled";
        msgFail = svcEnable?"enable":"disable";

        addRes = (*addFnc)(netH);
        
        if(addRes)
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "%s %s\r\n", argv[0], msgOK);
        }
        else
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "Failed to %s %s\r\n", msgFail, argv[0]);
        }
    }

    return true;
}


static int _Command_DNSOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
	typedef bool(*addSvcFnc)(TCPIP_NET_HANDLE hNet);

    TCPIP_NET_HANDLE netH;
    addSvcFnc        addFnc;
    bool             addRes, svcEnable;
    const char       *msgOK, *msgFail;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 4)
    {
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Usage: %s <service> <interface> <on/off> \r\n", argv[0]);
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Ex: %s PIC32INT on \r\n", argv[0]);
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[2]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface\r\n");
        return false;
    }

    if (memcmp(argv[3], "on", 2) == 0)
    {   // turning on a service
        svcEnable = true;
    }
    else if (memcmp(argv[3], "off", 2) == 0)
    {   // turning off a service
        svcEnable = false;
    }
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown option\r\n");
        return false;
    }

    addFnc = svcEnable?TCPIP_DNS_Enable:TCPIP_DNS_Disable;

    msgOK   = svcEnable?"enabled":"disabled";
    msgFail = svcEnable?"enable":"disable";

    addRes = (*addFnc)(netH);

    if(addRes)
    {
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "%s %s\r\n", argv[0], msgOK);
    }
    else
    {
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Failed to %s %s\r\n", msgFail, argv[0]);
    }
    return true;
}

static int _Command_IPAddressSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    TCPIP_NET_IF*   pNetIf;
    IP_ADDRESS_TYPE addType;

#if defined(TCPIP_STACK_USE_IPV4)
    IPV4_ADDR ipAddr, ipMask;
    IPV4_ADDR*  pMask;
#endif  // defined(TCPIP_STACK_USE_IPV4)
#if defined(TCPIP_STACK_USE_IPV6)
    IPV6_ADDR  ipv6Addr;
    int     prefixLen;
#endif  // defined(TCPIP_STACK_USE_IPV6)
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    bool     success = false;

    if (argc < 3)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: setip <interface> <ipv4/6 address> <ipv4mask/ipv6 prefix len>\r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: setip PIC32INT 192.168.0.8 255.255.255.0 \r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface specified \r\n");
        return false;
    }

    pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "No such interface is up\r\n");
        return false;
    }


    addType = IP_ADDRESS_TYPE_ANY;

#if defined(TCPIP_STACK_USE_IPV4)
    if (TCPIP_Helper_StringToIPAddress(argv[2], &ipAddr))
    {
        addType = IP_ADDRESS_TYPE_IPV4;
    }
#endif  // defined(TCPIP_STACK_USE_IPV4)
#if defined(TCPIP_STACK_USE_IPV6)
    if(TCPIP_Helper_StringToIPv6Address (argv[2], &ipv6Addr))
    {
        addType = IP_ADDRESS_TYPE_IPV6;
    }
#endif  // defined(TCPIP_STACK_USE_IPV6)

    if(addType == IP_ADDRESS_TYPE_ANY)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Invalid IP address string \r\n");
        return false;
    }
    

#if defined(TCPIP_STACK_USE_IPV4)
    if(addType == IP_ADDRESS_TYPE_IPV4)
    {
        if(_TCPIPStackAddressServiceIsRunning(pNetIf) != TCPIP_STACK_ADDRESS_SERVICE_NONE)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "An address service is already running. Stop DHCP, ZCLL, etc. first\r\n");
            return false;
        }

        if(argc > 3)
        {   // we have net mask too
            if (!TCPIP_Helper_StringToIPAddress(argv[3], &ipMask))
            {
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Invalid IP mask string \r\n");
                return false;
            }
            pMask = &ipMask;
        }
        else
        {
            pMask = 0;
        }

        if(TCPIP_STACK_NetAddressSet(netH, &ipAddr, pMask, true))
        {
            success = true;
        }

    }
#endif  // defined(TCPIP_STACK_USE_IPV4)

#if defined(TCPIP_STACK_USE_IPV6)

    if(addType == IP_ADDRESS_TYPE_IPV6)
    {
        if(argc > 3)
        {   // we have prefix length
            prefixLen = atoi(argv[3]);
        }
        else
        {
            prefixLen = 0;
        }

        if(TCPIP_IPV6_UnicastAddressAdd (netH, &ipv6Addr, prefixLen, false) != 0)
        {
            success = true;
        }
    }

#endif  // defined(TCPIP_STACK_USE_IPV6)


    (*pCmdIO->pCmdApi->msg)(cmdIoParam, success ? "Set ip address OK\r\n" : "Set ip address failed\r\n");
    return false;
}

static int _Command_GatewayAddressSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    IP_ADDRESS_TYPE addType;
#if defined(TCPIP_STACK_USE_IPV4)
    IPV4_ADDR ipGateway;
#endif  // defined(TCPIP_STACK_USE_IPV4)
#if defined(TCPIP_STACK_USE_IPV6)
    IPV6_ADDR  ipv6Gateway;
    unsigned long validTime;
#endif  // defined(TCPIP_STACK_USE_IPV6)
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    bool     success = false;

    if (argc != 3)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: setgw <interface> <ipv4/6 address> <validTime> \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: setgw PIC32INT 192.168.0.1 \r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface specified \r\n");
        return false;
    }

    addType = IP_ADDRESS_TYPE_ANY;

#if defined(TCPIP_STACK_USE_IPV4)
    if (TCPIP_Helper_StringToIPAddress(argv[2], &ipGateway))
    {
        addType = IP_ADDRESS_TYPE_IPV4;
    }
#endif  // defined(TCPIP_STACK_USE_IPV4)
#if defined(TCPIP_STACK_USE_IPV6)
    if(TCPIP_Helper_StringToIPv6Address (argv[2], &ipv6Gateway))
    {
        addType = IP_ADDRESS_TYPE_IPV6;
    }
#endif  // defined(TCPIP_STACK_USE_IPV6)

    if(addType == IP_ADDRESS_TYPE_ANY)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Invalid IP address string \r\n");
        return false;
    }


#if defined(TCPIP_STACK_USE_IPV4)
    if(addType == IP_ADDRESS_TYPE_IPV4)
    {
        success = TCPIP_STACK_NetAddressGatewaySet(netH, &ipGateway);
    }
#endif  // defined(TCPIP_STACK_USE_IPV4)

#if defined(TCPIP_STACK_USE_IPV6)
    if(addType == IP_ADDRESS_TYPE_IPV6)
    {
        if(argc > 3)
        {   // we have validity time
            validTime = (unsigned long)atoi(argv[3]);
        }
        else
        {
            validTime = 0;
        }
        success = TCPIP_IPV6_RouterAddressAdd(netH, &ipv6Gateway, validTime, 0);
    }
#endif  // defined(TCPIP_STACK_USE_IPV6)


    (*pCmdIO->pCmdApi->msg)(cmdIoParam, success ? "Set gateway address OK\r\n" : "Set gateway address failed\r\n");
    return false;

}

static int _Command_PrimaryDNSAddressSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    IPV4_ADDR ipDNS;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 3) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: setdns <interface> <x.x.x.x> \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: setdns PIC32INT 255.255.255.0 \r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface specified \r\n");
        return false;
    }

    if (!TCPIP_Helper_StringToIPAddress(argv[2], &ipDNS)) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Invalid IP address string \r\n");
        return false;
    }

    if(!TCPIP_STACK_NetAddressDnsPrimarySet(netH, &ipDNS)) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Set DNS address failed\r\n");
        return false;
    }

    return true;
}

#if defined (TCPIP_STACK_USE_TFTP_CLIENT)
static int _Command_TFTPC_Service(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    TCPIP_TFTP_CMD_TYPE cmdType=TFTP_CMD_NONE;
    int  hostNameLen =0;
    int  fileNameLen=0;
    if (argc != 4) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: tftpc <hostname> <command> <filename>\r\n");
        return false;
    }
    hostNameLen = strlen(argv[1]);
    if(hostNameLen >= sizeof(tftpServerHost))
    {
        (*pCmdIO->pCmdApi->msg)(pCmdIO->cmdIoParam, "TFTPC: Server name is too long. Retry.\r\n");
        return true;
    }
    strcpy(tftpServerHost, argv[1]);
    if(strcmp("put",argv[2])==0)
    {
        cmdType = TFTP_CMD_PUT_TYPE;
    }
    else if(strcmp("get",argv[2])==0)
    {
        cmdType = TFTP_CMD_GET_TYPE;
    }
    else
    {
        (*pCmdIO->pCmdApi->msg)(pCmdIO->cmdIoParam, "TFTPC:Command not found.\r\n");
        return true;
    }

    // Process file name
    fileNameLen = strlen(argv[3]);
    if(fileNameLen < sizeof(tftpcFileName))
    {
        strcpy(tftpcFileName, argv[3]);
    }
    else
    {
        (*pCmdIO->pCmdApi->print)(pCmdIO->cmdIoParam, "TFTPC:File size should be less than [ %d ] .\r\n",sizeof(tftpcFileName)-1);
        return true;
    }

    if(TCPIP_TFTPC_SetCommandOption(tftpServerHost,cmdType,tftpcFileName) != TFTPC_ERROR_NONE)
	{
		(*pCmdIO->pCmdApi->msg)(pCmdIO->cmdIoParam, "TFTPC:Command processing error.\r\n");
        return true;
	}
    return false;
}
#endif
#if defined(TCPIP_STACK_USE_DNS)
static int _Command_DNS_Service(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t             *hostName;
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    TCPIP_DNS_RESULT res;
    DNS_SERVICE_COMD_TYPE val=DNS_SERVICE_COMD_NONE;
    DNSS_COMMAND_MAP dnssComnd[6]=
            {
                {"add", DNS_SERVICE_COMD_ADD},
                {"del",DNS_SERVICE_COMD_DEL},
                {"info",DNS_SERVICE_COMD_INFO},
                {"configintf",DNS_SERVICE_COMD_SWITCH_INTERFACE},
                {"service",DNS_SERVICE_COMD_ENABLE_INTF},
                {"lookup",DNS_SERVICE_COMD_LOOKUP},
            };
    int i=0;

    if (argc < 2) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dnsc <service/del/info/configintf/lookup> \r\n");
        return false;
    }
    for(i=0;i<(sizeof(dnssComnd)/sizeof(DNSS_COMMAND_MAP));i++)
    {
        if(strcmp(argv[1],dnssComnd[i].command) ==0)
        {
            val = dnssComnd[i].val;
            break;
        }
    }
    switch(val)
    {
        case DNS_SERVICE_COMD_ENABLE_INTF:
            _Command_DNSOnOff(pCmdIO,argc,argv);
            break;
        case DNS_SERVICE_COMD_LOOKUP:
            if (argc != 4) {
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dnsc lookup <type> <hostName> \r\n");
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: <hostName>(URL) - look up for hostname\r\n");
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: <type> : a or A for IPv4 address lookup\r\n");
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: <type> : aaaa or AAAA for IPv6 address lookup\r\n");
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: <type> : any for both IPv4 and IPv6  address lookup\r\n");
                return false;
            }
            _Command_DNSLookUP(pCmdIO,argv);
            break;
        case DNS_SERVICE_COMD_DEL:
            if (argc != 3) {
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dnsc del <hostName>|all \r\n");
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: <hostName>(URL) - Remove the entry if exists \r\n");
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: all - Remove all the resolved entry \r\n");
                return false;
            }

            hostName = (uint8_t*)argv[2];
            if (hostName == 0)
            {
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown option\r\n");
                return false;
            }
            if(strcmp((char*)hostName,(char*)"all")==0)
            {
                TCPIP_DNS_RemoveAll();
                res = DNS_RES_OK;
            }
            else
            {
                res = TCPIP_DNS_RemoveEntry((const char*)hostName);
            }
            switch(res)
            {
                case DNS_RES_NO_ENTRY:
                    (*pCmdIO->pCmdApi->print)(cmdIoParam, "[%s] not part of the DNS Cache entry \r\n",hostName);
                    return false;
                case DNS_RES_NO_SERVICE:
                    (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Incompleate command \r\n");
                    return false;
                case DNS_RES_OK:
                    return false;
                default:
                    return false;
            }
            break;
        case DNS_SERVICE_COMD_INFO:
            _Command_ShowDNSResolvedInfo(pCmdIO,argc,argv);
            break;
         case DNS_SERVICE_COMD_SWITCH_INTERFACE:
            _Command_ConfigDNSServer(pCmdIO,argc,argv);
            break;
        default:
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "Invalid Input Command :[ %s ] \r\n", argv[1]);
            return false;
    }
    return true;
}

static int _Command_ConfigDNSServer(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    TCPIP_NET_HANDLE netH;
    int servIndex;
    if (argc != 4) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dnsclient configintf <interface> <server Ip index<0|1>>\r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: Configure DNS Server details dynamically \r\n");
        return false;
    }
    netH = TCPIP_STACK_NetHandleGet(argv[2]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface\r\n");
        return false;
    }
    servIndex = atoi(argv[3]);
    if(servIndex >= TCPIP_DNS_MAX_SERVERS_SUPPORTED)
        return false;

    if(!TCPIP_DNS_SwitchDNSServerInterface(netH,servIndex))
        return false;

    return true;
}

static int _Command_DNSLookUP(SYS_CMD_DEVICE_NODE* pCmdIO, char** argv)
{
    if(tcpipCmdStat != TCPIP_CMD_STAT_IDLE)
    {
        (*pCmdIO->pCmdApi->msg)(pCmdIO->cmdIoParam, "dnsc lookup: command in progress. Retry later.\r\n");
        return true;
    }

    if((strcmp(argv[2], "A") == 0) || (strcmp(argv[2], "a") == 0))
    {
        dnsType=DNS_TYPE_A;
    }
    else if((strcmp(argv[2], "AAAA") == 0) || (strcmp(argv[2], "aaaa") == 0))
    {
        dnsType=DNS_TYPE_AAAA;
    }
    else if((strcmp(argv[2], "ANY") == 0) || (strcmp(argv[2], "any") == 0))
    {
        dnsType=DNS_TYPE_ANY;
    }
    else
    {
        (*pCmdIO->pCmdApi->print)(pCmdIO->cmdIoParam, "dnsc lookup: [%s] Lookup Type not supported.\r\n",argv[2]);
        return true;
    }

    if(strlen(argv[3]) > sizeof(dnslookupTargetHost) - 1)
    {
        (*pCmdIO->pCmdApi->msg)(pCmdIO->cmdIoParam, "dnsc lookup: Host name too long. Retry.\r\n");
        return true;
    }
    strcpy(dnslookupTargetHost, argv[3]);

    dnsLookupCmdIoParam = pCmdIO->cmdIoParam;
    (*pCmdIO->pCmdApi->print)(pCmdIO, "dnsc lookup: resolving host: %s for type:%s \r\n", dnslookupTargetHost,argv[2]);
    tcpipCmdStat = TCPIP_DNS_LOOKUP_CMD_GET;
    pTcpipCmdDevice = pCmdIO;
    _TCPIPStackSignalHandlerSetParams(TCPIP_THIS_MODULE_ID, tcpipCmdSignalHandle, TCPIP_DNS_CLIENT_TASK_PROCESS_RATE);

    return false;
}

static int _Command_ShowDNSResolvedInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_DNS_RESOLVED_ADDRESS ipAddress;
    IPV4_ADDR       primaryDns1,primaryDns2;
    int             intfIndex=0;

    IPV4_ADDR       ipv4Addr[TCPIP_DNS_CLIENT_CACHE_PER_IPV4_ADDRESS];
    uint8_t         hostName[TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN+1];
    int             index=0,i=0;
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    TCPIP_DNS_RESULT res;
    uint32_t    ttlTime=0;
    bool entryPresent= false,module=false;
    char *isEnabled[9]={"Disabled\0","Enabled\0"};
    IPV6_ADDR   ipv6Addr[TCPIP_DNS_CLIENT_CACHE_PER_IPV6_ADDRESS];
    uint8_t     addrBuf[44];

    if (argc != 2) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dnsclient info \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: display the DNS cache entry details \r\n");
        return false;
    }

    index = 0;
   // memset(dnsHostName,0,sizeof(dnsHostName));
    //(*pCmdIO->pCmdApi->msg)(cmdIoParam,"HostName            Timeout       IP-Address\r\n");

    ipAddress.ipv4Address = ipv4Addr;
    ipAddress.nIPv4Entries = 0;
    ipAddress.ipv6Address = ipv6Addr;
    ipAddress.nIPv6Entries = 0;
    primaryDns2.Val = primaryDns1.Val = 0;
    res = TCPIP_DNS_GetDNSClientConnectionInfo(&primaryDns1,&primaryDns2,&intfIndex,&module);
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "DNS configuration\r\nModule : %s\r\nPrimary DNS = %d.%d.%d.%d \r\n",isEnabled[module],primaryDns1.v[0],primaryDns1.v[1],primaryDns1.v[2],primaryDns1.v[3]);
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "Secondary DNS = %d.%d.%d.%d \r\n",primaryDns2.v[0],primaryDns2.v[1],primaryDns2.v[2],primaryDns2.v[3]);
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "Interface Index : %d \r\n",intfIndex);
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "===================================\r\n",0);
    while(1)
    {
        memset(hostName,0,TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN+1);
        memset(ipv4Addr,0,sizeof(IPV4_ADDR)*TCPIP_DNS_CLIENT_CACHE_PER_IPV4_ADDRESS);
        memset(ipv6Addr,0,sizeof(IPV6_ADDR)*TCPIP_DNS_CLIENT_CACHE_PER_IPV6_ADDRESS);
        ipAddress.nIPv4Entries = 0;
        ipAddress.nIPv6Entries = 0;
        res = TCPIP_DNS_GetResolverDetails((char*)hostName,&ipAddress,&ttlTime,TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN,&index);
        if(res == DNS_RES_OK)
        {
            entryPresent = true;
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "Hostname = %s \r\nTimeout = %d \r\n",hostName,ttlTime);
            if(ipAddress.nIPv4Entries > 0)
            {
                for(i=0;i<ipAddress.nIPv4Entries;i++)
                {                    
                    (*pCmdIO->pCmdApi->print)(cmdIoParam, "IPv4 = %d.%d.%d.%d\r\n",ipAddress.ipv4Address[i].v[0],
                        ipAddress.ipv4Address[i].v[1],ipAddress.ipv4Address[i].v[2],ipAddress.ipv4Address[i].v[3]);                    
                }
            }
            if(ipAddress.nIPv6Entries > 0)
            {
                for(i=0;i<ipAddress.nIPv6Entries;i++)
                {
                    memset(addrBuf,0,sizeof(addrBuf));
                    TCPIP_Helper_IPv6AddressToString(ipAddress.ipv6Address,(char*)addrBuf,sizeof(addrBuf));                   
                    (*pCmdIO->pCmdApi->print)(cmdIoParam, "IPv6 = %s\r\n",addrBuf);
                }
            }
            (*pCmdIO->pCmdApi->print)(cmdIoParam,"----------------------------------------------------\r\n",0);
        }
        if(res == DNS_RES_NO_SERVICE)
        {
            if(entryPresent == false)
            {
               (*pCmdIO->pCmdApi->msg)(cmdIoParam, "No DNS Server Cache entry \r\n");
            }
            entryPresent = false;
            break;
        }
        index++;
    }
    return false;
}
#endif

#if defined(TCPIP_STACK_USE_DNS_SERVER)
static int _Command_DNSSOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    typedef bool(*addSvcFnc)(TCPIP_NET_HANDLE hNet);

    TCPIP_NET_HANDLE netH;
    addSvcFnc        addFnc;
    bool             addRes, svcEnable;
    const char       *msgOK, *msgFail;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 4)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage:dnss service <interface> <on/off> \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: dnss service PIC32INT on \r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[2]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface\r\n");
        return false;
    }

    if (memcmp(argv[3], "on", 2) == 0)
    {   // turning on a service
        svcEnable = true;
    }
    else if (memcmp(argv[3], "off", 2) == 0)
    {   // turning off a service
        svcEnable = false;
    }
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown option\r\n");
        return false;
    }
	addFnc = svcEnable?TCPIP_DNSS_Enable:TCPIP_DNSS_Disable;

        msgOK   = svcEnable?"enabled":"disabled";
        msgFail = svcEnable?"enable":"disable";

    addRes = (*addFnc)(netH);

    if(addRes)
    {
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "%s %s  for interface [%s] \r\n", argv[0], msgOK,argv[2]);
    }
    else
    {
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Failed to %s %s for interface [%s]\r\n", msgFail, argv[0],argv[2]);
    }
    return true;
}

static int _Command_AddDelDNSSrvAddress(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv,DNS_SERVICE_COMD_TYPE dnsCommand)
{
    IP_ADDRESS_TYPE     addrType;
    uint8_t             *hostName;
    IP_MULTI_ADDRESS    ipDNS;
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    uint32_t		validStarttime=0;
#if defined(TCPIP_STACK_USE_IPV6)
    uint8_t     addrBuf[44];
#endif
    TCPIP_DNS_RESULT res = DNS_RES_NO_SERVICE;

    if(dnsCommand == DNS_SERVICE_COMD_DEL)
    {
         if (argc != 5) {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dnss del <hostName> <IPType> <x.x.x.x>  \r\n");
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: hostName - URL , IPType - 4 for Ipv4 address and 6 for Ipv6 address \r\n");
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: One IP address per URL at a time will be deleted \r\n");
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: dnss del www.xyz.com 4 10.20.30.40  \r\n");
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: dnss del www.abc.com 6 2001::101  \r\n");
            return false;
        }
    }
    else if(dnsCommand == DNS_SERVICE_COMD_ADD)
    {
        if (argc < 6) {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dnss add <hostName> <IPType> <x.x.x.x> <lifeTime> \r\n");
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: hostName - URL , IPType - 4 for Ipv4 address and 6 for Ipv6 address \r\n");
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: LifeTime - The life time in Second for each entry to be used \r\n");
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: One IP address per URL at a time will be added \r\n");
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: dnss add www.xyz.com 4 10.20.30.40 120 \r\n");
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: dnss add www.abc.com 6 2001::101 120 \r\n");
            return false;
        }
    }
    else
    {
        return false;
    }

    if(strlen(argv[2])>TCPIP_DNSS_HOST_NAME_LEN)
    {
        (*pCmdIO->pCmdApi->print)(cmdIoParam, " Hostname length should not be more than [%d]\r\n",TCPIP_DNSS_HOST_NAME_LEN);
        return false;
    }
    hostName = (uint8_t*)argv[2];

    if (memcmp(argv[3], "4", 1) == 0)
    {   // turning on a service
        addrType = IP_ADDRESS_TYPE_IPV4;
    }
#if defined(TCPIP_STACK_USE_IPV6)
    else if (memcmp(argv[3], "6", 1) == 0)
    {   // turning off a service
        addrType = IP_ADDRESS_TYPE_IPV6;
    }
#endif
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown option\r\n");
        return false;
    }
    if(addrType == IP_ADDRESS_TYPE_IPV4)
    {
        if (!TCPIP_Helper_StringToIPAddress(argv[4], &ipDNS.v4Add)) {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Invalid IPv4 address string \r\n");
            return false;
        }
    }
#if defined(TCPIP_STACK_USE_IPV6)
    if(addrType == IP_ADDRESS_TYPE_IPV6)
    {
        strncpy((char*)addrBuf,argv[4],strlen(argv[4]));
        if (!TCPIP_Helper_StringToIPv6Address((char*)addrBuf, &ipDNS.v6Add)) {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Invalid IPv6 address string \r\n");
            return false;
        }
    }
#endif

    if(dnsCommand == DNS_SERVICE_COMD_DEL)
    {
        res = TCPIP_DNSS_CacheEntryRemove((const char*)hostName,addrType,&ipDNS);
    }
    else if(dnsCommand == DNS_SERVICE_COMD_ADD)
    {
        validStarttime= (unsigned long)atoi((char*)argv[5]);
        res = TCPIP_DNSS_EntryAdd((const char*)hostName,addrType,&ipDNS,validStarttime);
    }

    switch(res)
    {
        case DNS_RES_NO_ENTRY:
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "The Address is not part of the DNS Cache entry \r\n");
            return false;
        case DNS_RES_MEMORY_FAIL:
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "No memory available \r\n");
            return false;
        case DNS_RES_CACHE_FULL:
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "No space to add [%s] entry \r\n",hostName);
            return false;
        case DNS_RES_OK:
            return true;
        default:
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "Failed to add [%s] entry \r\n",hostName);
            return false;
    }
    return true;
}

static int _Command_DnsServService(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int i=0;
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    DNS_SERVICE_COMD_TYPE val=DNS_SERVICE_COMD_NONE;
    DNSS_COMMAND_MAP dnssComnd[4]=
            {
                {"service",DNS_SERVICE_COMD_ENABLE_INTF},
                {"add", DNS_SERVICE_COMD_ADD,},
                {"del",DNS_SERVICE_COMD_DEL,},
                {"info",DNS_SERVICE_COMD_INFO,},
            }; 
	
	
    if (argc < 2) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dnss <service/add/del/info> \r\n");
         return false;
    }
    
    for(i=0;i<(sizeof(dnssComnd)/sizeof(DNSS_COMMAND_MAP));i++)
    {
        if(strcmp(argv[1],dnssComnd[i].command) ==0)
        {
            val = dnssComnd[i].val;
            break;
        }
    }
    
    switch(val)
    {
        case DNS_SERVICE_COMD_ENABLE_INTF:
            _Command_DNSSOnOff(pCmdIO,argc,argv);
            break;
        case DNS_SERVICE_COMD_ADD:
            _Command_AddDelDNSSrvAddress(pCmdIO,argc,argv,val);
            break;
        case DNS_SERVICE_COMD_DEL:
            _Command_AddDelDNSSrvAddress(pCmdIO,argc,argv,val);
            break;
        case DNS_SERVICE_COMD_INFO:
            _Command_ShowDNDServInfo(pCmdIO,argc,argv);
            break;
        default:
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "Invalid Input Command :[ %s ] \r\n", argv[1]);
            return false;
    }
    return true;
}

static int _Command_ShowDNDServInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    IP_MULTI_ADDRESS ipDNS;
    IP_ADDRESS_TYPE addrType;
    uint8_t         *hostName;
    uint8_t         ipcount=0;
    int             index=0;
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    TCPIP_DNS_RESULT res;
    uint32_t    ttlTime=0;
    bool        entryPresent=false;
#if defined(TCPIP_STACK_USE_IPV6)
    uint8_t     addrBuf[44];
#endif    

    if (argc != 3) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: dnsserv info <hostname> | <all>\r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Help: display the DNS cache entry details \r\n");
        return false;
    }
    hostName = (uint8_t*)argv[2];
    if(strcmp((char*)argv[2],"all")==0)
    {
    	index = 0;
       // memset(dnsHostName,0,sizeof(dnsHostName));		
        (*pCmdIO->pCmdApi->msg)(cmdIoParam,"HostName        IPv4/IPv6Count\r\n");

        while(1)
        {
            res = TCPIP_DNSS_AddressCntGet(index,(uint8_t*)hostName,&ipcount);
            if(res == DNS_RES_OK)
            {
                entryPresent = true;
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "%s       %d\r\n",hostName,ipcount);
            }
            else if(res == DNS_RES_NO_SERVICE)
            {
                if(entryPresent == false)
                {
                   (*pCmdIO->pCmdApi->msg)(cmdIoParam, "No DNS Server Cache entry \r\n");
                }
                entryPresent = false;
                break;
            }
            else
            {
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "No Memory is available \r\n");
                break;
            }
            index++;
        }
        return true;
    }
    addrType = IP_ADDRESS_TYPE_IPV4;
    index = 0;
    (*pCmdIO->pCmdApi->msg)(cmdIoParam,"HostName        IPv4Address     TTLTime \r\n");
    while(1)
    {
        res = TCPIP_DNSS_EntryGet((uint8_t*)hostName,addrType,index,&ipDNS,&ttlTime);
        if(res == DNS_RES_OK)
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "%s   %d.%d.%d.%d     %d\r\n",hostName,ipDNS.v4Add.v[0],ipDNS.v4Add.v[1],
                ipDNS.v4Add.v[2],ipDNS.v4Add.v[3],ttlTime);
            entryPresent = true;
        }
        else if((res == DNS_RES_NO_SERVICE)|| (res == DNS_RES_NO_ENTRY))
        {
            if(entryPresent == false)
            {
               (*pCmdIO->pCmdApi->print)(cmdIoParam, "[%s] No Ipv4 Address with in DNS Cache entry \r\n",hostName);
            }
            entryPresent = false;
            break;
        }
        else
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "No Memory is available \r\n");
            break;
        }
        index++;
    }
    
    (*pCmdIO->pCmdApi->msg)(cmdIoParam, "\r\n");

#if defined(TCPIP_STACK_USE_IPV6)
    addrType = IP_ADDRESS_TYPE_IPV6;
    index = 0;
    (*pCmdIO->pCmdApi->msg)(cmdIoParam,"HostName        IPv6Address             TTLTime \r\n");
    while(1)
    {
        res = TCPIP_DNSS_EntryGet((uint8_t*)hostName,addrType,index,&ipDNS,&ttlTime);
        if(res == DNS_RES_OK)
        {
            TCPIP_Helper_IPv6AddressToString(&ipDNS.v6Add,(char*)addrBuf,sizeof(addrBuf));
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "%s       %s      %d\r\n",hostName,addrBuf,ttlTime);
            entryPresent = true;
        }
        else if((res == DNS_RES_NO_SERVICE)|| (res == DNS_RES_NO_ENTRY))
        {
            if(entryPresent == false)
            {
               (*pCmdIO->pCmdApi->print)(cmdIoParam, "[%s] No Ipv6 Address DNS Cache entry \r\n",hostName);
            }
            entryPresent = false;
            break;
        }
        else
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "No Memory is available \r\n");
            break;
        }
       
        index++;
    }
#endif
    return true;
}
#endif

static int _Command_BIOSNameSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    const char* msg;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 3)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: setbios <interface> <x.x.x.x> \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: setbios PIC32INT MCHPBOARD_29 \r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface specified \r\n");
        return false;
    }

    if(TCPIP_STACK_NetBiosNameSet(netH, argv[2]))
    {
        msg = "Set BIOS Name OK\r\n";
    }
    else
    {
        msg = "Set BIOS Name failed\r\n";
    }

    (*pCmdIO->pCmdApi->msg)(cmdIoParam, msg);
    return true;
}

static int _Command_MACAddressSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    TCPIP_MAC_ADDR macAddr;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 3) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: setmac <interface> <x:x:x:x:x:x> \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: setmac PIC32INT aa:bb:cc:dd:ee:ff \r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface specified \r\n");
        return false;
    }

    (*pCmdIO->pCmdApi->print)(cmdIoParam, "argv[2]: %s\r\n", argv[2]);

    if (!TCPIP_Helper_StringToMACAddress(argv[2], macAddr.v)) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Invalid MAC address string \r\n");
        return false;
    }

    if(!TCPIP_STACK_NetAddressMacSet(netH, &macAddr)) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Set MAC address failed\r\n");
        return false;
    }

    return true;
}


static int _Command_NetworkOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    bool res = false;
    TCPIP_NET_HANDLE netH;
#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
    TCPIP_COMMAND_STG_DCPT*   pDcpt;
    TCPIP_NETWORK_CONFIG*     pNetConf;
#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
    const TCPIP_NETWORK_CONFIG*     pIfConf;
    SYS_MODULE_OBJ      tcpipStackObj;
    TCPIP_STACK_INIT    tcpip_init_data;
    uint16_t net_ix = 0;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 3)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: if <interface> <down/up> \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: if PIC32INT down \r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);

    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface specified \r\n");
        return false;
    }

    net_ix = TCPIP_STACK_NetIndexGet(netH);

    if (memcmp(argv[2], "up", 2) == 0)
    {
        if(TCPIP_STACK_NetIsUp(netH))
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "This interface already up\r\n");
            return true;
        }

        // get the data passed at initialization
        tcpipStackObj = TCPIP_STACK_Initialize(0, 0);
        TCPIP_STACK_InitializeDataGet(tcpipStackObj, &tcpip_init_data);
        pIfConf = tcpip_init_data.pNetConf + net_ix;

#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
        if(pCmdStgDcpt) 
        {
            // get the saved network configuration
            pDcpt = pCmdStgDcpt + net_ix;
            if(pDcpt->stgSize)
            {   // saved config is valid; restore
                pNetConf = TCPIP_STACK_NetConfigSet(&pDcpt->netDcptStg, pDcpt->restoreBuff, sizeof(pDcpt->restoreBuff), 0);
                if(pNetConf)
                {   // use the saved data
                    pIfConf = pNetConf;
                }
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Interface up: configuration " );
                (*pCmdIO->pCmdApi->msg)(cmdIoParam, pNetConf ? "restored\r\n" : "restore failed!\r\n");
            }
        }
#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)

        res = TCPIP_STACK_NetUp(netH, pIfConf);
    }
    else if (memcmp(argv[2], "down", 4) == 0)
    {
        if(TCPIP_STACK_NetIsUp(netH) == 0)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "This interface already down\r\n");
            return true;
        }

#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
        if(pCmdStgDcpt) 
        {
            // get the last network configuration so we use it when
            // restart the stack/interface 
            pDcpt = pCmdStgDcpt + net_ix;
            pDcpt->stgSize = TCPIP_STACK_NetConfigGet(netH, &pDcpt->netDcptStg, sizeof(pDcpt->netDcptStg), 0);

            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Interface down: configuration saved\r\n");
        }
#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)

        res = TCPIP_STACK_NetDown(netH);
    } 
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Wrong parameter specified \r\n");
        return false;
    }

    if (res == true)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Operation successful!\r\n");
    }
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Operation failed!\r\n");
    }

    return true;
}

static int _Command_StackOnOff(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
    TCPIP_NET_HANDLE netH;
    int              netIx;
    TCPIP_COMMAND_STG_DCPT  *pDcpt;
    TCPIP_NETWORK_CONFIG    *pCurrConf, *pDstConf;
#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
    TCPIP_STACK_INIT        tcpipInit;
    SYS_MODULE_OBJ          tcpipStackObj;     // stack handle
    const char              *msg;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc < 2)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: stack <up/down> <preserve>\r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: stack down preserve\r\n");
        return false;
    }


    if (memcmp(argv[1], "up", 2) == 0)
    {
        // try to get a stack handle
        tcpipStackObj = TCPIP_STACK_Initialize(0, 0);
        if ( tcpipStackObj != SYS_MODULE_OBJ_INVALID)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Stack already up!\r\n");
            return true;
        }
        // check the saved init data when the stack went down
        if(pCmdTcpipInitData == 0)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Turn Stack down and then up!\r\n");
            return true;
        }

        // copy of the init data; use as default
        tcpipInit = *pCmdTcpipInitData;

#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
        if(pCmdStgDcpt != 0 && pCmdNetConf != 0) 
        {
            // get the saved network configuration
            pDcpt = pCmdStgDcpt + 0;
            pDstConf = pCmdNetConf + 0; 
            pCurrConf = 0;
            for (netIx = 0; netIx < initialNetIfs; netIx++)
            {
                if(pDcpt->stgSize)
                {   // saved config is valid; restore
                    pCurrConf = TCPIP_STACK_NetConfigSet(&pDcpt->netDcptStg, pDcpt->restoreBuff, sizeof(pDcpt->restoreBuff), 0);
                }
                else
                {   // don't have a config to restore
                    pCurrConf = 0;
                }

                if(pCurrConf == 0)
                {   // restore failed
                    break;
                }
                else
                {   // save into array for the stack initialization
                    // force the interface start with power up
                    pCurrConf->powerMode = TCPIP_STACK_IF_POWER_FULL;
                    memcpy(pDstConf, pCurrConf, sizeof(*pDstConf));
                }

                pDcpt++;
                pDstConf++;
            }

            if(pCurrConf)
            {   // success
                tcpipInit.pNetConf = pCmdNetConf;
                tcpipInit.nNets = initialNetIfs;
                msg = "Stack up: configuration restored\r\n";
            }
            else
            {
                msg = "Stack up: configuration restore failed\r\n";
            }

            (*pCmdIO->pCmdApi->msg)(cmdIoParam, msg);
        }
#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)

        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Restarting the stack with %d interface(s)\r\n", tcpipInit.nNets);

        tcpipStackObj = TCPIP_STACK_Initialize(0, &tcpipInit.moduleInit);     // init the stack
        if ( tcpipStackObj == SYS_MODULE_OBJ_INVALID)
        {
            msg = "Stack up failed\r\n";
        }
        else
        {
            msg = "Stack up succeeded\r\n";
        }
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, msg);
    }
    else if (memcmp(argv[1], "down", 4) == 0)
    {
        // try to get a handle
        tcpipStackObj = TCPIP_STACK_Initialize(0, 0);
        if ( tcpipStackObj == SYS_MODULE_OBJ_INVALID)
        {
            msg = "Stack down: cannot get a stack handle\r\n";
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, msg);
        }
        else
        {
            // store the data passed at initialization
            TCPIP_STACK_InitializeDataGet(tcpipStackObj, &cmdTcpipInitData);
            pCmdTcpipInitData = &cmdTcpipInitData;

#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
            tcpipCmdPreserveSavedInfo = false;
            if(argc == 3 && memcmp(argv[2], "preserve", strlen("preserve")) == 0)
            {
                if(pCmdStgDcpt) 
                {
                    // get the last network configuration so we use it when
                    // restart the stack/interface 
                    pDcpt = pCmdStgDcpt + 0;
                    for (netIx = 0; netIx < initialNetIfs; netIx++)
                    {
                        netH = TCPIP_STACK_IndexToNet(netIx);
                        pDcpt->stgSize = TCPIP_STACK_NetConfigGet(netH, &pDcpt->netDcptStg, sizeof(pDcpt->netDcptStg), 0);
                        pDcpt++;
                    }

                    (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Stack down: configuration saved\r\n");
                    tcpipCmdPreserveSavedInfo = true;
                }
            }
#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)

            TCPIP_STACK_Deinitialize(tcpipStackObj);
#if defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
            tcpipCmdPreserveSavedInfo = false;          // make sure it doesn't work the next time
#endif  // defined(TCPIP_STACK_COMMANDS_STORAGE_ENABLE)
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Stack down succeeded\r\n");
        }
    }

    return true;
}

static int _Command_HeapInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int     ix, nTraces, nEntries;
    TCPIP_HEAP_TRACE_ENTRY    tEntry;
    size_t  heapSize;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 1) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: heapinfo \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: heapinfo \r\n");
        return false;
    }

    heapSize = TCPIP_HEAP_Size(initialHeapH);
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "Initial created heap size: %d Bytes\r\n", heapSize);
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "Allocable block heap size: %d Bytes\r\n", TCPIP_HEAP_MaxSize(initialHeapH));
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "All available heap size: %d Bytes\r\n", TCPIP_HEAP_FreeSize(initialHeapH));
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "Last heap error: 0x%x\r\n", TCPIP_HEAP_LastError(initialHeapH));

    nTraces = TCPIP_HEAP_TraceGetEntriesNo(initialHeapH, true);
    if(nTraces)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Trace info: \r\n");
        nEntries = TCPIP_HEAP_TraceGetEntriesNo(initialHeapH, false);
        for(ix = 0; ix < nEntries; ix++)
        {
            if(TCPIP_HEAP_TraceGetEntry(initialHeapH, ix, &tEntry))
            {
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "Module: %4d, totAllocated: %5d, currAllocated: %5d, totFailed: %5d, maxFailed: %5d \r\n", tEntry.moduleId, tEntry.totAllocated, tEntry.currAllocated, tEntry.totFailed, tEntry.maxFailed);
            }
                    
        }
    }
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "No Trace info exists.\r\n");
    }

    nEntries = TCPIP_HEAP_DistGetEntriesNo(initialHeapH);
    if(nEntries)
    {
        int     modIx;
        TCPIP_HEAP_DIST_ENTRY distEntry;
        int currLowHitMem = 0;
        int currHiHitMem = 0;

        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "TCPIP Heap distribution: \n\r");
        
        for(ix = 0; ix < nEntries; ix++)
        {
            TCPIP_HEAP_DistGetEntry(initialHeapH, ix, &distEntry);
            
            int entryPrint = 0;
            struct moduleDist* pMDist = distEntry.modDist;
            for(modIx = 0; modIx < sizeof(distEntry.modDist)/sizeof(*distEntry.modDist); modIx++, pMDist++)
            {
                if(pMDist->modHits)
                {
                    if(entryPrint == 0)
                    {
                        (*pCmdIO->pCmdApi->print)(cmdIoParam, "[%4d,    %5d]:\n\r", distEntry.lowLimit, distEntry.highLimit);
                        (*pCmdIO->pCmdApi->print)(cmdIoParam, "\tcurr hits: %d, \n\r", distEntry.currHits);
                        currLowHitMem += distEntry.currHits * distEntry.lowLimit;
                        currHiHitMem += distEntry.currHits * distEntry.highLimit;
                        entryPrint = 1;
                    }
                    (*pCmdIO->pCmdApi->print)(cmdIoParam, "\t mod: %d, \thits: %d, \n\r", pMDist->modId, pMDist->modHits);
                }
            }
            if(distEntry.gHits)
            {
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "\t mod: xx \thits: %d, \n\r", distEntry.gHits);
            }
        }

        (*pCmdIO->pCmdApi->print)(cmdIoParam, "curr Low Lim: %d, curr Hi Lim: %d, max Free: %d, min Free: %d\n\r", currLowHitMem, currHiHitMem, heapSize - currLowHitMem, heapSize - currHiHitMem);
    }

    return true;
}

static int _Command_MacInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int                     netNo, netIx;
    TCPIP_NET_HANDLE        netH;
    TCPIP_MAC_RX_STATISTICS rxStatistics;
    TCPIP_MAC_TX_STATISTICS txStatistics;
    TCPIP_MAC_STATISTICS_REG_ENTRY  regEntries[8];
    TCPIP_MAC_STATISTICS_REG_ENTRY* pRegEntry;
    int                     jx, hwEntries;
    char                    entryName[sizeof(pRegEntry->registerName) + 1];
    const char*             netName;

    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc != 1) {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: macinfo \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: macinfo \r\n");
        return false;
    }


    netNo = TCPIP_STACK_NumberOfNetworksGet();
    for(netIx = 0; netIx < netNo; netIx++)
    {
        netH = TCPIP_STACK_IndexToNet(netIx);
        netName = TCPIP_STACK_NetNameGet(netH);
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Interface: %s driver statistics\r\n", netName);
        if(TCPIP_STACK_NetMACStatisticsGet(netH, &rxStatistics, &txStatistics))
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "\tnRxOkPackets: %d, nRxPendBuffers: %d, nRxSchedBuffers: %d, ",
                    rxStatistics.nRxOkPackets, rxStatistics.nRxPendBuffers, rxStatistics.nRxSchedBuffers);
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "nRxErrorPackets: %d, nRxFragmentErrors: %d\r\n", rxStatistics.nRxErrorPackets, rxStatistics.nRxFragmentErrors);
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "\tnTxOkPackets: %d, nTxPendBuffers: %d, nTxErrorPackets: %d, nTxQueueFull: %d\r\n",
                    txStatistics.nTxOkPackets, txStatistics.nTxPendBuffers, txStatistics.nTxErrorPackets, txStatistics.nTxQueueFull);
        }
        else
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "\tnot supported\r\n");
        }

        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Interface: %s hardware statistics\r\n", netName);
        if(TCPIP_STACK_NetMACRegisterStatisticsGet(netH, regEntries, sizeof(regEntries)/sizeof(*regEntries), &hwEntries))
        {
            entryName[sizeof(entryName) - 1] = 0;
            for(jx = 0, pRegEntry = regEntries; jx < hwEntries && jx < sizeof(regEntries)/sizeof(*regEntries); jx++, pRegEntry++)
            {
                strncpy(entryName, pRegEntry->registerName, sizeof(entryName) - 1);
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "\t%s: 0x%8x\r\n", entryName, pRegEntry->registerValue);
            }
        }
        else
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "\tnot supported\r\n");
        }

    }

    return true;
}

#if defined(TCPIP_STACK_USE_DNS)
void TCPIPCmdDnsTask(void)
{
    TCPIP_DNS_RESULT  dnsRes;
    char ipv4Index=0,ipv6Index=0;
    TCPIP_DNS_RESOLVED_SERVER_ADDRESS serverIpAddress;
    uint8_t     addrBuf[44];
    uint32_t    timeout=0;

    switch(tcpipCmdStat)
    {
        case TCPIP_DNS_LOOKUP_CMD_GET:
            dnsRes = TCPIP_DNS_Resolve(dnslookupTargetHost, dnsType);
            if(dnsRes != DNS_RES_OK)
            {   // some other error
                (*pTcpipCmdDevice->pCmdApi->print)(dnsLookupCmdIoParam, "DnsLookUp: DNS failure for %s\r\n", dnslookupTargetHost);
                tcpipCmdStat = TCPIP_CMD_STAT_IDLE;
                break;
            }
            tcpipCmdStat = TCPIP_DNS_LOOKUP_CMD_WAIT;
            dnsLookUpStartTick = SYS_TMR_TickCountGet();
            // else wait some more
            break;
        case TCPIP_DNS_LOOKUP_CMD_WAIT:
            if(dnsType == DNS_TYPE_AAAA)
            {
                dnsRes = TCPIP_DNS_IsResolved(dnslookupTargetHost, &dnsLookUpAddr.v6Add);
            }
            else
            {
                dnsRes = TCPIP_DNS_IsResolved(dnslookupTargetHost, &dnsLookUpAddr.v4Add);
            }
            timeout = (SYS_TMR_TickCountGet() - dnsLookUpStartTick)/SYS_TMR_TickCounterFrequencyGet();
            if(timeout >= (TCPIP_DNS_CLIENT_SERVER_TMO/2))
            {   // timeout
                (*pTcpipCmdDevice->pCmdApi->print)(dnsLookupCmdIoParam, "DNS LookUp: request timeout.\r\n");
                tcpipCmdStat = TCPIP_CMD_STAT_IDLE;
                break;
            }
            if(dnsRes == DNS_RES_PENDING)
            {   // operation in progress
                break;
            }
            else if(dnsRes < 0 )
            {   // timeout or some other DNS error
                (*pTcpipCmdDevice->pCmdApi->print)(dnsLookupCmdIoParam, "DnsLookUp: DNS Lookup failure for %s\r\n", dnslookupTargetHost);
                tcpipCmdStat = TCPIP_CMD_STAT_IDLE;
                break;
            }
            _TCPIPStackSignalHandlerSetParams(TCPIP_THIS_MODULE_ID, tcpipCmdSignalHandle, 0);
            tcpipCmdStat = TCPIP_CMD_STAT_IDLE;
            // success
            (*pTcpipCmdDevice->pCmdApi->msg)(dnsLookupCmdIoParam, "Look UP Answer:\r\n----------------------\r\n");
            serverIpAddress.nIPv4Entries = TCPIP_DNS_GetNumberOfIPAddresses(dnslookupTargetHost,IP_ADDRESS_TYPE_IPV4);
            serverIpAddress.nIPv6Entries = TCPIP_DNS_GetNumberOfIPAddresses(dnslookupTargetHost,IP_ADDRESS_TYPE_IPV6);
            if((serverIpAddress.nIPv4Entries == 0) && (serverIpAddress.nIPv6Entries == 0))
            {
                (*pTcpipCmdDevice->pCmdApi->print)(dnsLookupCmdIoParam, "No LookUP entry for [%s]\r\n",dnslookupTargetHost);
                break;
            }
            while(1)
            {
                if(ipv4Index<serverIpAddress.nIPv4Entries)
                {
                    TCPIP_DNS_GetIPv4Address(dnslookupTargetHost,ipv4Index,&serverIpAddress.ip4Address);
                    (*pTcpipCmdDevice->pCmdApi->print)(dnsLookupCmdIoParam, "[%s] A IPv4 Address : %d.%d.%d.%d\r\n",dnslookupTargetHost,serverIpAddress.ip4Address.v[0],
                            serverIpAddress.ip4Address.v[1],serverIpAddress.ip4Address.v[2],serverIpAddress.ip4Address.v[3]);
                    ipv4Index++;
                }
                else if(ipv6Index<serverIpAddress.nIPv6Entries)
                {
                    TCPIP_DNS_GetIPv6Address(dnslookupTargetHost,ipv6Index,&serverIpAddress.ip6Address);
                    memset(addrBuf,0,sizeof(addrBuf));
                    TCPIP_Helper_IPv6AddressToString(&serverIpAddress.ip6Address,(char*)addrBuf,sizeof(addrBuf));
                    (*pTcpipCmdDevice->pCmdApi->print)(dnsLookupCmdIoParam, "[%s] AAAA IPv6 Address :%s\r\n",dnslookupTargetHost,addrBuf);
                    ipv6Index++;
                }
                else
                {
                    break;
                }
            }

        default:
            break;
    }
}
#endif

#if defined(TCPIP_STACK_USE_ICMP_CLIENT)
static int _CommandPing(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    int     argIx;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc < 2)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ping Usage: ping <stop> <if> <name/address> <n> <msDelay>\r\n");
        return true;
    }

    if(strcmp(argv[1], "stop") == 0)
    {
        if(tcpipCmdStat != TCPIP_CMD_STAT_IDLE)
        {
            _PingStop(pCmdIO, cmdIoParam);
        }
        return true;
    }

    if(tcpipCmdStat != TCPIP_CMD_STAT_IDLE)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ping: command in progress. Retry later.\r\n");
        return true;
    }

    // check the 1st parameter type
    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if(netH == 0)
    {   // use default interface
        icmpNetH = 0;
        argIx = 1; 
    }
    else
    {
        icmpNetH = netH;
        argIx = 2; 
        if (argc < 3)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ping Usage: ping <stop> <if> <name/address> <n> <msDelay>\r\n");
            return true;
        }
    }

    if(TCPIP_Helper_StringToIPAddress(argv[argIx], &icmpTargetAddr))
    {
        strncpy(icmpTargetAddrStr, argv[argIx], sizeof(icmpTargetAddrStr));
        icmpTargetHost[0] = '\0';
        tcpipCmdStat = TCPIP_PING_CMD_DO_PING;
    }
    else
    {   // assume host address
        if(strlen(argv[argIx]) > sizeof(icmpTargetHost) - 1)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ping: Host name too long. Retry.\r\n");
            return true;
        }
        strcpy(icmpTargetHost, argv[argIx]);
        tcpipCmdStat = TCPIP_PING_CMD_DNS_GET;
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Ping: resolving host: %s\r\n", icmpTargetHost);
    }


    if(hIcmp == 0)
    {
        if((hIcmp = TCPIP_ICMP_CallbackRegister(CommandPingHandler)) == 0)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ping: Failed to register ICMP handler\r\n");
            return true;
        }
    }

    icmpSequenceNo = SYS_RANDOM_PseudoGet();
    icmpIdentifier = SYS_RANDOM_PseudoGet();

    icmpReqNo = 0;
    icmpReqDelay = 0;
    argIx ++; 
    if(argc > argIx)
    {
        icmpReqNo = atoi(argv[argIx]);
        argIx++;
        if(argc > argIx)
        {
            icmpReqDelay = atoi(argv[argIx]);
        }
    }

    if(icmpReqNo == 0)
    {
        icmpReqNo = TCPIP_STACK_COMMANDS_ICMP_ECHO_REQUESTS;
    }
    if(icmpReqDelay == 0)
    {
        icmpReqDelay = TCPIP_STACK_COMMANDS_ICMP_ECHO_REQUEST_DELAY;
    }

    // convert to ticks
    if(icmpReqDelay < TCPIP_COMMAND_ICMP_ECHO_REQUEST_MIN_DELAY)
    {
        icmpReqDelay = TCPIP_COMMAND_ICMP_ECHO_REQUEST_MIN_DELAY;
    }

    pTcpipCmdDevice = pCmdIO;
    icmpCmdIoParam = cmdIoParam; 
    icmpAckRecv = 0;
    icmpReqCount = 0;

    _TCPIPStackSignalHandlerSetParams(TCPIP_THIS_MODULE_ID, tcpipCmdSignalHandle, icmpReqDelay);

    return true;

}

static void CommandPingHandler(TCPIP_NET_HANDLE hNetIf, IPV4_ADDR * remoteIP, void * data)
{
    char addBuff[20];

    if(tcpipCmdStat == TCPIP_CMD_STAT_IDLE)
    {
        return; // not our reply?
    }

    uint16_t* pReply = (uint16_t*)data;
    uint16_t myRecvId = *pReply;
    uint16_t myRecvSequenceNumber = *(pReply + 1);


    if (myRecvSequenceNumber != icmpSequenceNo || myRecvId != icmpIdentifier)
    {
        (*pTcpipCmdDevice->pCmdApi->msg)(icmpCmdIoParam, "Ping: wrong reply received\r\n");
    }
    else
    {
        uint32_t pingTicks = SYS_TMR_TickCountGet() - icmpStartTick;
        int pingMs = (pingTicks * 1000) / SYS_TMR_TickCounterFrequencyGet();
        if(pingMs == 0)
        {
            pingMs = 1;
        }

        TCPIP_Helper_IPAddressToString(remoteIP, addBuff, sizeof(addBuff));

        (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "Ping: reply from %s: time = %dms\r\n", addBuff, pingMs);
        icmpAckRecv++;
    }

}

#if defined (TCPIP_STACK_USE_IPV6)
static int _Command_IPv6_Ping(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    uint32_t size =0;
    TCPIP_NET_HANDLE netH;
    int     argIx;

    if(tcpipCmdStat != TCPIP_CMD_STAT_IDLE)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "ping6: command in progress. Retry later.\r\n");
        return true;
    }

    if((argc < 2) ||(argc > 4))
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: ping6 <net> <x::x:x:x:x> <size> \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: ping6 fe80::23:2222:3333:1234 1500\r\n");
        return true;
    }
 // check the 1st parameter type
    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if(netH == 0)
    {   // use default interface
        icmpNetH = TCPIP_STACK_NetDefaultGet();
        argIx = 1;
    }
    else
    {
        icmpNetH = netH;
        argIx = 2;
        if (argc < 3)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: ping6 <net> <x::x:x:x:x> <size> \r\n");
            return true;
        }
    }

    if(TCPIP_Helper_StringToIPv6Address(argv[argIx], &icmpv6TargetAddr))
    {
        strncpy(icmpTargetAddrStr, argv[argIx], sizeof(icmpTargetAddrStr));
        icmpTargetHost[0] = '\0';
        tcpipCmdStat = TCPIP_SEND_ECHO_REQUEST_IPV6;
        memset(icmpv6TargetAddrStr,0,sizeof(icmpv6TargetAddrStr));
        if(strlen(argv[argIx]) <= sizeof(icmpv6TargetAddrStr))
        {
            strcpy(icmpv6TargetAddrStr,argv[argIx]);
        }
    }
     else
    {   // assume host address
        if(strlen(argv[argIx]) > sizeof(icmpTargetHost) - 1)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "ping6: Host name too long. Retry.\r\n");
            return true;
        }
        strcpy(icmpTargetHost, argv[argIx]);
        tcpipCmdStat = TCPIP_PING6_CMD_DNS_GET;
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "ping6: resolving host: %s\r\n", icmpTargetHost);
    }
    
    if(argv[argIx+1] == NULL)
    {
        size  = 0;
    }
    else
    {
        size  = atoi((char *)argv[argIx+1]);
    }
    
     pingPktSize = size;

    if(hIcmpv6 == 0)
    {
        if((hIcmpv6 = TCPIP_ICMPV6_CallbackRegister(CommandPing6Handler)) == 0)
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "ping6: Failed to register ICMP handler\r\n");
            return true;
        }
    }

    icmpSequenceNo = SYS_RANDOM_PseudoGet();
    icmpIdentifier = SYS_RANDOM_PseudoGet();
    icmpReqNo = 0;
    icmpReqDelay = 0;
    if(icmpReqNo == 0)
    {
        icmpReqNo = TCPIP_STACK_COMMANDS_ICMP_ECHO_REQUESTS;
    }
    if(icmpReqDelay == 0)
    {
        icmpReqDelay = TCPIP_STACK_COMMANDS_ICMP_ECHO_REQUEST_DELAY;
    }

    // convert to ticks
    if(icmpReqDelay < TCPIP_COMMAND_ICMP_ECHO_REQUEST_MIN_DELAY)
    {
        icmpReqDelay = TCPIP_COMMAND_ICMP_ECHO_REQUEST_MIN_DELAY;
    }

    pTcpipCmdDevice = pCmdIO;
    icmpCmdIoParam = cmdIoParam;
    icmpAckRecv = 0;
    icmpReqCount = 0;

    _TCPIPStackSignalHandlerSetParams(TCPIP_THIS_MODULE_ID, tcpipCmdSignalHandle, icmpReqDelay);
    return true;

}

static void CommandPing6Handler(TCPIP_NET_HANDLE hNetIf,uint8_t type, const IPV6_ADDR * localIP, const IPV6_ADDR * remoteIP, void * data)
{
    char addBuff[42];

    if(tcpipCmdStat == TCPIP_CMD_STAT_IDLE)
    {
        return; // not our reply?
    }

    if(type != ICMPV6_INFO_ECHO_REPLY)
    {
        return;
    }
    ICMPV6_HEADER_ECHO* pReply = (ICMPV6_HEADER_ECHO*)data;
    uint16_t myRecvId = pReply->identifier;
    uint16_t myRecvSequenceNumber = pReply->sequenceNumber;


    if (myRecvSequenceNumber != icmpSequenceNo || myRecvId != icmpIdentifier)
    {
        (*pTcpipCmdDevice->pCmdApi->msg)(icmpCmdIoParam, "ping6: wrong reply received\r\n");
    }
    else
    {
        uint32_t pingTicks = SYS_TMR_TickCountGet() - icmpStartTick;
        int pingMs = (pingTicks * 1000) / SYS_TMR_TickCounterFrequencyGet();
        if(pingMs == 0)
        {
            pingMs = 1;
        }
        memset(addBuff,0,sizeof(addBuff));
        TCPIP_Helper_IPv6AddressToString(remoteIP, addBuff, sizeof(addBuff));

        (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "ping6: reply from [%s] time = %dms\r\n", addBuff, pingMs);
        icmpAckRecv++;
    }

}
#endif


static void _PingStop(SYS_CMD_DEVICE_NODE* pCmdIO, const void* cmdIoParam)
{
    _TCPIPStackSignalHandlerSetParams(TCPIP_THIS_MODULE_ID, tcpipCmdSignalHandle, 0);
    tcpipCmdStat = TCPIP_CMD_STAT_IDLE;
    if(pCmdIO)
    {
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "Ping: done. Sent %d requests, received %d replies.\r\n", icmpReqCount, icmpAckRecv);
    }
    pTcpipCmdDevice = 0;
}


static void TCPIPCmdPingTask(void)
{
    ICMP_ECHO_RESULT echoRes;
#if defined(TCPIP_STACK_USE_IPV6)   
    bool ipv6EchoRes=false;
#endif
    TCPIP_DNS_RESULT  dnsRes;
    bool killIcmp = false;
       
    switch(tcpipCmdStat)
    {
        case TCPIP_PING_CMD_DNS_GET:          
            dnsRes = TCPIP_DNS_Resolve(icmpTargetHost, DNS_TYPE_A);
            if(dnsRes != DNS_RES_OK)
            {   // some other error
                (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "Ping: DNS failure for %s\r\n", icmpTargetHost);
                killIcmp = true;
                break;
            }
            icmpStartTick = SYS_TMR_TickCountGet();
            tcpipCmdStat = TCPIP_PING_CMD_DNS_WAIT;
            // else wait some more
            break;

        case TCPIP_PING_CMD_DNS_WAIT:
            dnsRes = TCPIP_DNS_IsResolved(icmpTargetHost, &icmpTargetAddr);
            if(dnsRes == DNS_RES_PENDING)
            {   // operation in progress
                break;
            }
            else if(dnsRes < 0 )
            {   // timeout or some other DNS error
                (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "Ping: DNS failure for %s\r\n", icmpTargetHost);
                killIcmp = true;
                break;
            }
            // success
 
            TCPIP_Helper_IPAddressToString(&icmpTargetAddr, icmpTargetAddrStr, sizeof(icmpTargetAddrStr));
            tcpipCmdStat = TCPIP_PING_CMD_DO_PING;            
            break;
        case TCPIP_PING_CMD_DO_PING:
            if(icmpReqCount != 0 && icmpAckRecv == 0)
            {   // no reply received; 
                if(SYS_TMR_TickCountGet() - icmpStartTick > (SYS_TMR_TickCounterFrequencyGet() * TCPIP_STACK_COMMANDS_ICMP_ECHO_TIMEOUT) / 1000)
                {   // timeout
                    (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "Ping: request timeout.\r\n");
                    killIcmp = true;
                    break;
                }
                // else wait some more
            }

            if(icmpReqCount == icmpReqNo)
            {   // no more requests to send
                killIcmp = true;
                break;
            }

            // send another request
            echoRes = TCPIP_ICMP_EchoRequestSend (icmpNetH, &icmpTargetAddr, ++icmpSequenceNo, icmpIdentifier);

            if(echoRes >= 0 )
            {
                icmpStartTick = SYS_TMR_TickCountGet();
                if(icmpReqCount++ == 0)
                {
                    (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "Ping: request sent to: %s [%s]\r\n", icmpTargetHost, icmpTargetAddrStr);
                }
            }
            else
            {
                (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "Ping: failed to send request to: %s\r\n", icmpTargetAddrStr);
                killIcmp = true;
            }

            break;
#if defined (TCPIP_STACK_USE_IPV6)
        case TCPIP_PING6_CMD_DNS_GET:
            dnsRes = TCPIP_DNS_Resolve(icmpTargetHost, DNS_TYPE_AAAA);
            if(dnsRes != DNS_RES_OK)
            {   // some other error
                (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "ping6: DNS failure for %s\r\n", icmpTargetHost);
                killIcmp = true;
                break;
            }
            icmpStartTick = SYS_TMR_TickCountGet();
            tcpipCmdStat = TCPIP_PING6_CMD_DNS_WAIT;
            // else wait some more
            break;

        case TCPIP_PING6_CMD_DNS_WAIT:
            dnsRes = TCPIP_DNS_IsResolved(icmpTargetHost, &icmpv6TargetAddr);
            if(dnsRes == DNS_RES_PENDING)
            {   // operation in progress
                break;
            }
            else if(dnsRes < 0 )
            {   // timeout or some other DNS error
                (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "Ping: DNS failure for %s\r\n", icmpTargetHost);
                killIcmp = true;
                break;
            }
            // success

            TCPIP_Helper_IPv6AddressToString(&icmpv6TargetAddr, icmpv6TargetAddrStr, sizeof(icmpv6TargetAddrStr));
            tcpipCmdStat = TCPIP_SEND_ECHO_REQUEST_IPV6;
            break;
        case TCPIP_SEND_ECHO_REQUEST_IPV6:
            if(icmpReqCount != 0 && icmpAckRecv == 0)
            {   // no reply received;
                if(SYS_TMR_TickCountGet() - icmpStartTick > (SYS_TMR_TickCounterFrequencyGet() * TCPIP_STACK_COMMANDS_ICMP_ECHO_TIMEOUT) / 1000)
                {   // timeout
                    (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "ping6: request timeout.\r\n");
                    killIcmp = true;
                    break;
                }
                // else wait some more
            }
            if(icmpReqCount == icmpReqNo)
            {   // no more requests to send
                killIcmp = true;
                break;
            }

            // send another request
            ipv6EchoRes = TCPIP_ICMPV6_EchoRequestSend (icmpNetH, &icmpv6TargetAddr, ++icmpSequenceNo, icmpIdentifier,pingPktSize);

            if(ipv6EchoRes != 0 )
            {
                icmpStartTick = SYS_TMR_TickCountGet();
                if(icmpReqCount++ == 0)
                {
                    (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "ping6: request sent to: %s \r\n", icmpv6TargetAddrStr);
                }
            }
            else
            {
                (*pTcpipCmdDevice->pCmdApi->print)(icmpCmdIoParam, "ping6: failed to send request to: %s\r\n", icmpv6TargetAddrStr);
                killIcmp = true;
            }

            break;
#endif /* TCPIP_STACK_USE_IPV6 */

        default:
            killIcmp = true;
            break;

    }

    if(killIcmp)
    {
        _PingStop(pTcpipCmdDevice, icmpCmdIoParam);
    }

}

#endif  // defined(TCPIP_STACK_USE_ICMP_CLIENT)

void TCPIP_COMMAND_Task(void)
{
    TCPIP_MODULE_SIGNAL sigPend;

    sigPend = _TCPIPStackModuleSignalGet(TCPIP_THIS_MODULE_ID, TCPIP_MODULE_SIGNAL_MASK_ALL);

    if((sigPend & TCPIP_MODULE_SIGNAL_TMO) != 0)
    { // regular TMO occurred

#if defined(TCPIP_STACK_USE_ICMP_CLIENT)
        if(TCPIP_CMD_STAT_PING_START <= tcpipCmdStat && tcpipCmdStat <= TCPIP_CMD_STAT_PING_STOP)
        {
            TCPIPCmdPingTask();
        }
#endif  // defined(TCPIP_STACK_USE_ICMP_CLIENT)

#if defined(TCPIP_STACK_USE_DNS)
        if(TCPIP_CMD_STAT_DNS_START <= tcpipCmdStat && tcpipCmdStat <= TCPIP_CMD_STAT_DNS_STOP)
        {
            TCPIPCmdDnsTask();
        }
#endif  // defined(TCPIP_STACK_USE_DNS)
    }
}




static int _CommandArp(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    TCPIP_NET_HANDLE netH;
    IPV4_ADDR ipAddr;
    TCPIP_ARP_RESULT  arpRes;
    TCPIP_MAC_ADDR    macAddr;
    char*       message;
    char        addrBuff[20];
    size_t      arpEntries, ix;
    TCPIP_ARP_ENTRY_QUERY arpQuery;

    
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc < 3)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: arp <interface> <r/q/d/l> <ipAddr> \r\n");
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Ex: arp PIC32INT r 192.168.1.105 \r\n");
        return false;
    }

    netH = TCPIP_STACK_NetHandleGet(argv[1]);
    if (netH == 0)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Unknown interface\r\n");
        return false;
    }


    if (strcmp(argv[2], "l") == 0)
    {   // list the cache contents
        arpEntries = TCPIP_ARP_CacheEntriesNoGet(netH, ARP_ENTRY_TYPE_TOTAL);
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "arp: %d slots in the cache\r\n", arpEntries);
        for(ix = 0; ix < arpEntries; ix++)
        {
            TCPIP_ARP_EntryQuery(netH, ix, &arpQuery);
            if(arpQuery.entryType == ARP_ENTRY_TYPE_PERMANENT || arpQuery.entryType == ARP_ENTRY_TYPE_COMPLETE)
            {
                TCPIP_Helper_IPAddressToString(&arpQuery.entryIpAdd, addrBuff, sizeof(addrBuff));
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "arp: IPv4 address: %s", addrBuff);
                TCPIP_Helper_MACAddressToString(&arpQuery.entryHwAdd, addrBuff, sizeof(addrBuff));
                (*pCmdIO->pCmdApi->print)(cmdIoParam, ", MAC Address: %s", addrBuff);
                if(arpQuery.entryType == ARP_ENTRY_TYPE_COMPLETE)
                {
                    (*pCmdIO->pCmdApi->msg)(cmdIoParam, ", complete\r\n");
                }
                else
                {
                    (*pCmdIO->pCmdApi->msg)(cmdIoParam, ", permanent\r\n");
                }
            }
            else if(arpQuery.entryType == ARP_ENTRY_TYPE_INCOMPLETE)
            {
                TCPIP_Helper_IPAddressToString(&arpQuery.entryIpAdd, addrBuff, sizeof(addrBuff));
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "arp: IPv4 address: %s, queued\r\n", addrBuff);
            }
        }

        return false;
    }

    if (argc < 4 || !TCPIP_Helper_StringToIPAddress(argv[3], &ipAddr))
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Invalid IP address string \r\n");
        return false;
    }

    if (strcmp(argv[2], "r") == 0)
    {   // request an address
        arpRes = TCPIP_ARP_EntryGet(netH, &ipAddr, &macAddr, true);
        switch(arpRes)
        {
            case ARP_RES_ENTRY_SOLVED:

                TCPIP_Helper_MACAddressToString(&macAddr, addrBuff, sizeof(addrBuff));
                (*pCmdIO->pCmdApi->print)(cmdIoParam, "arp: resolved - IPv4 address: %s, MAC Address: %s\r\n", argv[3], addrBuff);
                return false;

            case ARP_RES_ENTRY_QUEUED:
                message = "arp: address already queued\r\n";
                break;

            case ARP_RES_ENTRY_NEW:
                message = "arp: address newly queued\r\n";
                break;

            default:    // ARP_RES_CACHE_FULL  
                message = "arp: queue full/error\r\n";
                break;
        }
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, message);
    }
    else if (strcmp(argv[2], "q") == 0)
    {   // query for an address
        arpRes = TCPIP_ARP_EntryGet(netH, &ipAddr, &macAddr, false);
        if(arpRes == ARP_RES_OK)
        {
            TCPIP_Helper_MACAddressToString(&macAddr, addrBuff, sizeof(addrBuff));
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "arp: IPv4 address: %s, MAC Address: %s\r\n", argv[3], addrBuff);
        }
        else
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "arp: no such entry\r\n");
        }
    }
    else if (strcmp(argv[2], "d") == 0)
    {   // delete an address
        arpRes = TCPIP_ARP_EntryRemove(netH, &ipAddr);
        if(arpRes == ARP_RES_OK)
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "arp: removed %s\r\n", argv[3]);
        }
        else
        {
            (*pCmdIO->pCmdApi->msg)(cmdIoParam, "arp: no such entry\r\n");
        }
    }
    else
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "arp: Unknown option\r\n");
    }


    return false;
}

// debug/trace suppport functions

#if defined(TCPIP_UDP_DEBUG)
static int _Command_UdpInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int  sktNo, ix;
    TCPIP_UDP_SKT_DEBUG_INFO sktInfo;


    const void* cmdIoParam = pCmdIO->cmdIoParam;
 
    sktNo = TCPIP_UDP_DebugSktNo();
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "UDP sockets: %d \r\n", sktNo);
    for(ix = 0; ix < sktNo; ix++)
    {
        if(TCPIP_UDP_DebugSktInfo(ix, &sktInfo))
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "\tsktIx: %d, addType: %d, remotePort: %d, localPort: %d, rxQueueSize: %d, txSize: %d\r\n",
                    ix, sktInfo.addType, sktInfo.remotePort, sktInfo.localPort, sktInfo.rxQueueSize, sktInfo.txSize);
        }
    }

    return true;
}
#endif  // defined(TCPIP_UDP_DEBUG)

#if defined(TCPIP_TCP_DEBUG)
static int _Command_TcpInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int  sktNo, ix;
    TCPIP_TCP_SKT_DEBUG_INFO sktInfo;


    const void* cmdIoParam = pCmdIO->cmdIoParam;

    sktNo = TCPIP_TCP_DebugSktNo();
    (*pCmdIO->pCmdApi->print)(cmdIoParam, "TCP sockets: %d \r\n", sktNo);
    for(ix = 0; ix < sktNo; ix++)
    {
        if(TCPIP_TCP_DebugSktInfo(ix, &sktInfo))
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "\tsktIx: %d, addType: %d, remotePort: %d, localPort: %d, rxSize: %d, txSize: %d, state: %d\r\n",
                    ix, sktInfo.addType, sktInfo.remotePort, sktInfo.localPort, sktInfo.rxSize, sktInfo.txSize, sktInfo.state);
        }
    }

    return true;
}
#endif  // defined(TCPIP_TCP_DEBUG)

#if defined(TCPIP_PACKET_TRACE_ENABLE)
static int _Command_PktInfo(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int  ix;
    TCPIP_PKT_TRACE_ENTRY tEntry;
    TCPIP_PKT_TRACE_INFO  tInfo;

    const void* cmdIoParam = pCmdIO->cmdIoParam;


    if(!TCPIP_PKT_TraceGetEntriesNo(&tInfo))
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "No packet info available\r\n");
        return true;
    }


    (*pCmdIO->pCmdApi->print)(cmdIoParam, "PKT trace slots: %d, used: %d, fails: %d, ackErr: %d, ownerFail: %d\r\n", tInfo.nEntries, tInfo.nUsed, tInfo.traceFails, tInfo.traceAckErrors, tInfo.traceAckOwnerFails);

    for(ix = 0; ix < tInfo.nEntries; ix++)
    {
        if(TCPIP_PKT_TraceGetEntry(ix, &tEntry))
        {
            (*pCmdIO->pCmdApi->print)(cmdIoParam, "\tmodId: %4d, totAlloc: %5d, currAlloc: %4d, currSize: %4d, totFailed: %4d, nAcks: %4d\r\n",
                    tEntry.moduleId, tEntry.totAllocated, tEntry.currAllocated, tEntry.currSize, tEntry.totFailed, tEntry.nAcks);
        }
    }

    return true;
}
#endif  // defined(TCPIP_PACKET_TRACE_ENABLE)

#if defined(TCPIP_PACKET_FLIGHT_LOG_ENABLE)
static int _Command_PktFlightFlag(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    bool paramAll;
    int flags;
    int enable;
    int modId = TCPIP_MODULE_NONE;

    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc < 4)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: pktf all/mod flags 0/1 \r\n");
        return true;
    }

    if(strcmp("all", argv[1]) == 0)
    {
        paramAll = true;
    }
    else
    {
        paramAll = false;
        modId = atoi(argv[1]);
    }

    flags = atoi(argv[2]);
    enable = atoi(argv[3]);

    if(paramAll)
    {
        TCPIP_PKT_FlightFlagsUpdateAll(flags, enable);
    }
    else
    {
        TCPIP_PKT_FlightFlagsUpdate(modId, flags, enable);
    }

    (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Set the packet flight flags\r\n");


    return true;
}


static int _Command_PktFlightType(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int enable;
    bool paramAll;
    int modType = TCPIP_MODULE_NONE;

    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc < 3)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: pktt all/modType 0/1 \r\n");
        return true;
    }

    if(strcmp("all", argv[1]) == 0)
    {
        paramAll = true;
    }
    else
    {
        paramAll = false;
        modType = atoi(argv[1]);
    }

    enable = atoi(argv[2]);

    if(paramAll)
    {
        TCPIP_PKT_FlightTypeUpdateAll(enable);
    }
    else
    {
        TCPIP_PKT_FlightTypeUpdate(modType, enable);
    }


    (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Set the packet flight modType\r\n");

    return true;
}
#endif  // defined(TCPIP_PACKET_FLIGHT_LOG_ENABLE)

#if defined(TCPIP_STACK_TIME_MEASUREMENT)
#include <cp0defs.h>
#include "system/clk/sys_clk.h"

static int _Command_StackExecTime(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    int reset = 0;
    int stop = 0;
    const void* cmdIoParam = pCmdIO->cmdIoParam;

    if (argc < 2)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Usage: xtime en(able)/get <1/0: reset/stop> \r\n");
    }
    else if(strcmp("en", argv[1]) == 0)
    {
        if(argc > 2)
        {
            reset = atoi(argv[2]);
        }

        if(reset || xTimeStartCount == 0)
        {
            xTimeTotal = 0;
            xTimeStartCount = _CP0_GET_COUNT();
        } 
        TCPIP_STACK_TimeMeasureStart(reset);    
        xTimeEnabled = true;

        (*pCmdIO->pCmdApi->msg)(cmdIoParam, "xtime: Started Stack Timing\r\n");
    }
    else if(xTimeEnabled && strcmp("get", argv[1]) == 0)
    {
        if(argc > 2)
        {
            stop = atoi(argv[2]);
        }
        uint64_t tStack = TCPIP_STACK_TimeMeasureGet(stop);    
        TCPIP_Commands_ExecTimeUpdate();
        uint64_t tTot = xTimeTotal;
        if(stop)
        {
            xTimeEnabled = false;
        }

        (*pCmdIO->pCmdApi->print)(cmdIoParam, "xtime Total counts: %.0f, Stack: %.0f\r\n", (float)tTot, (float)tStack);
        float msConv = SYS_CLK_SystemFrequencyGet() / 2000.0;
        float tTotMs = tTot / msConv;
        float tStackMs = tStack / msConv;
        (*pCmdIO->pCmdApi->print)(cmdIoParam, "xtime Total ms: %.2f, Stack: %.2f, Perc: %.2f%%\r\n", tTotMs, tStackMs, (tStackMs / tTotMs) * 100.0 );
    }

    return true;
}


void TCPIP_Commands_ExecTimeUpdate(void)
{
    if(xTimeEnabled)
    {
        uint32_t tCurr = _CP0_GET_COUNT();
        xTimeTotal += tCurr - xTimeStartCount;
        xTimeStartCount = tCurr;
    }
}

#endif // defined(TCPIP_STACK_TIME_MEASUREMENT)


#endif    // defined(TCPIP_STACK_COMMAND_ENABLE)


