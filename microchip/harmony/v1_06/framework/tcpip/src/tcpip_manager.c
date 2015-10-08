/*******************************************************************************
  TCP/IP Stack Manager

  Summary:
    Module for Microchip TCP/IP Stack

  Description:
    -Handles internal RX packet pre-processing prior to dispatching
     to upper application layers.
    -Reference: AN833
*******************************************************************************/

/*******************************************************************************
File Name:  tcpip_manager.c
Copyright �2012 released Microchip Technology Inc.  All rights
reserved.

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
#define TCPIP_THIS_MODULE_ID    TCPIP_MODULE_MANAGER

#include "tcpip/src/tcpip_private.h"


#include "tcpip/tcpip_mac_object.h"

#include "tcpip_module_manager.h"

#if defined(TCPIP_STACK_TIME_MEASUREMENT)
#include <cp0defs.h>
#endif // defined(TCPIP_STACK_TIME_MEASUREMENT)

#define TCPIP_STACK_INIT_MESSAGE   "TCP/IP Stack Initialization "

// MAC events enabled by the stack manager
#define TCPIP_STACK_MAC_ALL_EVENTS          (TCPIP_MAC_EV_RX_DONE | TCPIP_MAC_EV_TX_DONE | TCPIP_MAC_EV_RXTX_ERRORS)

// MAC events used by the stack manager to detect that
// there are active RX events that need processing
// always subset of TCPIP_STACK_MAC_ALL_EVENTS
#define TCPIP_STACK_MAC_ACTIVE_RX_EVENTS    (TCPIP_MAC_EV_RX_DONE | TCPIP_MAC_EV_RX_OVFLOW | TCPIP_MAC_EV_RX_BUFNA)


// constant data
//

// table with TCPIP interfaces alias names per MAC type
static const char* TCPIP_STACK_IF_ALIAS_NAME_TBL[TCPIP_MAC_TYPES] = 
{
    TCPIP_STACK_IF_NAME_ALIAS_UNK,
    TCPIP_STACK_IF_NAME_ALIAS_ETH,  
    TCPIP_STACK_IF_NAME_ALIAS_WLAN,
};


// notification modules
// Since the modules that need connection notification is
// known to the stack manager no dynamic approach is taken.
// But simply a call table is maintained.
static const tcpipModuleConnHandler  TCPIP_STACK_CONN_EVENT_TBL [] =
{
#if defined(TCPIP_STACK_USE_DHCP_CLIENT)
    TCPIP_DHCP_ConnectionHandler,
#endif // defined(TCPIP_STACK_USE_DHCP_CLIENT)

    // add other needed handlers here
};


// variables

static TCPIP_NET_IF* tcpipNetIf = 0;       // dynamically allocated

// Main default interfaces
typedef struct
{
    TCPIP_NET_IF* defaultNet;     // default network interface
}TCPIPDefaultIF;


static TCPIPDefaultIF tcpipDefIf = { 0 };

static volatile int    totTcpipEventsCnt = 0;
static volatile int    newTcpipTickAvlbl = 0;

static volatile int    newTcpipErrorEventCnt = 0;
static volatile int    newTcpipStackEventCnt = 0;


#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
static void    _TCPIP_MacEventCB(TCPIP_MAC_EVENT event, const void* hParam);
#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)

static uint8_t*    tcpip_heap = 0;          // the actual TCPIP heap

static TCPIP_STACK_MODULE_CTRL  tcpip_stack_ctrl_data = {0};

// last data structure passed for the stack initialization 
static TCPIP_STACK_INIT     tcpip_init_data = { {0} };

//
static SYS_TMR_HANDLE       tcpip_stack_tickH = SYS_TMR_HANDLE_INVALID;      // tick handle

static uint32_t             stackTaskRate;   // actual task running rate

static uint32_t             stackAsyncSignalCount;   // global counter of the number of times the modules requested a TCPIP_MODULE_SIGNAL_ASYNC
                                                    // whenever !=0, it means that async signal requests are active!
// a quick, constant time dispatch, approach taken here
// at the expense of some extra RAM used!
// Note: TCPIP_MODULE_NONE is usd as a manager entry for TMO signals!
static TCPIP_MODULE_SIGNAL_ENTRY  TCPIP_STACK_MODULE_SIGNAL_TBL [TCPIP_MODULES_NUMBER] = { {0} };

static SINGLE_LIST      TCPIP_MODULES_QUEUE_TBL [TCPIP_MODULE_LAYER3] = 
{
    // 0 layer handling
    { 0 }, // TCPIP_MODULE_NONE; dummy entry; not used
    { 0 }, // TCPIP_MODULE_MANAGER; manager special entry; consistent handling of all rxQueue's

    // 1st layer handling
    { 0 }, // TCPIP_MODULE_ARP
    { 0 }, // TCPIP_MODULE_IPV4
    { 0 }, // TCPIP_MODULE_IPV6
    { 0 }, // TCPIP_MODULE_LLDP

    // 2nd layer handling
    { 0 }, // TCPIP_MODULE_ICMP
    { 0 }, // TCPIP_MODULE_ICMPV6
    { 0 }, // TCPIP_MODULE_NDP
    { 0 }, // TCPIP_MODULE_UDP
    { 0 }, // TCPIP_MODULE_TCP

    // add other types of supported frames here

};


// table containing the layer 1 frames processed by this stack
static const TCPIP_FRAME_PROCESS_ENTRY TCPIP_FRAME_PROCESS_TBL [] = 
{
    // frameType                            // pktTypeFlags             // moduleId 
    // 1st layer handling
    {TCPIP_ETHER_TYPE_ARP,                  TCPIP_MAC_PKT_FLAG_ARP,     TCPIP_MODULE_ARP},  // ARP entry

#if defined(TCPIP_STACK_USE_IPV4)
    {TCPIP_ETHER_TYPE_IPV4,                 TCPIP_MAC_PKT_FLAG_IPV4,    TCPIP_MODULE_IPV4},  // IPv4 entry
#else
    {0,                                     0,                          TCPIP_MODULE_IPV4},  // IPv4 not processed
#endif  // defined(TCPIP_STACK_USE_IPV4)

#if defined(TCPIP_STACK_USE_IPV6)
    {TCPIP_ETHER_TYPE_IPV6,                 TCPIP_MAC_PKT_FLAG_IPV6,    TCPIP_MODULE_IPV6},  // IPv6 entry
#else
    {0,                                     0,                          0},  // IPv6 not processed
#endif // defined(TCPIP_STACK_USE_IPV6)

#if defined(TCPIP_STACK_USE_LLDP)
    {TCPIP_ETHER_TYPE_LLDP,                 TCPIP_MAC_PKT_FLAG_LLDP,    TCPIP_MODULE_LLDP},  // LLDP entry
#else
    {0,                                     0,                          0},  // LLDP not processed
#endif  // defined(TCPIP_STACK_USE_LLDP)

    // add other types of supported frames here

};


static SYS_STATUS       tcpip_stack_status = SYS_STATUS_UNINITIALIZED;



#if defined(TCPIP_STACK_TIME_MEASUREMENT)
static uint64_t         tcpip_stack_time = 0;
static bool             tcpip_stack_timeEnable = 0;
extern void             TCPIP_Commands_ExecTimeUpdate(void);
#endif // defined(TCPIP_STACK_TIME_MEASUREMENT)

static bool _TCPIPStackIsRunState(void);

static void _TCPIP_STACK_TickHandler(uintptr_t context, uint32_t currTick);        // stack tick handler

static void _TCPIP_ProcessTickEvent(void);
static int  _TCPIPExtractMacRxPackets(TCPIP_NET_IF* pNetIf);

static uint32_t _TCPIPProcessMacPackets(bool signal);

static void _TCPIP_ProcessMACErrorEvents(TCPIP_NET_IF* pNetIf, TCPIP_MAC_EVENT activeEvent);

static bool _InitNetConfig(const TCPIP_NETWORK_CONFIG* pUsrConfig, int nNets);
static bool _LoadNetworkConfig(const TCPIP_NETWORK_CONFIG* pUsrConfig, TCPIP_NET_IF* pNetIf, bool restartIf);

static const TCPIP_STACK_MODULE_CONFIG* _TCPIP_STACK_FindModuleData(TCPIP_STACK_MODULE moduleId, const TCPIP_STACK_MODULE_CONFIG* pModConfig, int nModules);

static void  TCPIP_STACK_BringNetDown(TCPIP_STACK_MODULE_CTRL* stackCtrlData);
static bool  TCPIP_STACK_BringNetUp(TCPIP_STACK_MODULE_CTRL* stackCtrlData, const TCPIP_NETWORK_CONFIG* pNetConf, const TCPIP_STACK_MODULE_CONFIG* pModConfig, int nModules);

static void* _NetConfigStringToBuffer(void** ppDstBuff, void* pSrcBuff, size_t* pDstSize, size_t* pNeedLen, size_t* pActLen);

static TCPIP_MAC_ACTION TCPIP_STACK_StackToMacAction(TCPIP_STACK_ACTION action);

static void TCPIP_STACK_StacktoMacCtrl(TCPIP_MAC_MODULE_CTRL* pMacCtrl, TCPIP_STACK_MODULE_CTRL* stackCtrlData);

static bool TCPIP_STACK_CheckEventsPending(void);

static void _TCPIP_NetIfEvent(TCPIP_NET_IF* pNetIf, TCPIP_MAC_EVENT event, bool isrProtect);

static TCPIP_MODULE_SIGNAL  _TCPIPStackManagerSignalClear(TCPIP_MODULE_SIGNAL clrMask);

#if !defined(TCPIP_STACK_APP_EXECUTE_MODULE_TASKS)
static void _TCPIPStackExecuteModules(void);
#endif  // !defined(TCPIP_STACK_APP_EXECUTE_MODULE_TASKS)

static void _TCPIPStackSignalTmo(void);

static bool _TCPIPStackCreateTimer(void);

static __inline__ void __attribute__((always_inline)) _TCPIPInsertMacRxPacket(TCPIP_NET_IF* pNetIf, TCPIP_MAC_PACKET* pRxPkt)
{
    pRxPkt->pktIf = pNetIf;
    _TCPIPStackModuleRxInsert(TCPIP_MODULE_MANAGER, pRxPkt, 0);
}

// protection against MAC ISR
static __inline__ uint32_t __attribute__((always_inline)) _TCPIPMacIsrSuspend(TCPIP_NET_IF* pNetIf)
{
#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
    return OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
#else
    return 0;
#endif // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
}

static __inline__ void __attribute__((always_inline)) _TCPIPMacIsrResume(TCPIP_NET_IF* pNetIf, uint32_t suspLevel)
{
#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, suspLevel);
#endif // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
}

static void _TCPIPSignalEntrySetNotify(TCPIP_MODULE_SIGNAL_ENTRY* pSigEntry, TCPIP_MODULE_SIGNAL signals);

static  void* _TCPIPSignalEntrySet(TCPIP_MODULE_SIGNAL_ENTRY* pSigEntry, TCPIP_MODULE_SIGNAL signal);

static __inline__ TCPIP_MODULE_SIGNAL_ENTRY* __attribute__((always_inline)) _TCPIPModuleToSignalEntry(TCPIP_STACK_MODULE modId)
{
    return TCPIP_STACK_MODULE_SIGNAL_TBL + modId;
}

// sets some signals for the module
static __inline__ void* __attribute__((always_inline)) _TCPIPModuleSignalSet(TCPIP_STACK_MODULE modId, TCPIP_MODULE_SIGNAL signal)
{
    return _TCPIPSignalEntrySet(_TCPIPModuleToSignalEntry(modId), signal);
}

static void     _TCPIPSignalEntryNotify(TCPIP_MODULE_SIGNAL_ENTRY* pSigEntry, TCPIP_MODULE_SIGNAL signal);

// sets module signals and calls user notification
static __inline__ void __attribute__((always_inline)) _TCPIPModuleSignalSetNotify(TCPIP_STACK_MODULE modId, TCPIP_MODULE_SIGNAL signals)
{
    _TCPIPSignalEntrySetNotify(_TCPIPModuleToSignalEntry(modId), signals);
}

static __inline__ void __attribute__((always_inline)) _TCPIP_ClearMacEvent(TCPIP_NET_IF* pNetIf, TCPIP_MAC_EVENT event)
{
    uint32_t suspLvl = _TCPIPMacIsrSuspend(pNetIf);

    pNetIf->activeEvents &= ~event;

    _TCPIPMacIsrResume(pNetIf, suspLvl);
}

static __inline__ void __attribute__((always_inline)) _TCPIPAsyncSignalInc(void)
{
    OSAL_CRITSECT_DATA_TYPE critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
    stackAsyncSignalCount += 1; 
    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, critSect);
}

// Note the TCPIP_EVENT enum is aligned with the TCPIP_MAC_EVENT!
static __inline__ TCPIP_EVENT __attribute__((always_inline)) TCPIP_STACK_Mac2TcpipEvent(TCPIP_MAC_EVENT macEvent)
{
    return (TCPIP_EVENT)macEvent;
}

static bool    _TCPIP_StackSyncFunction(void* synchHandle, TCPIP_MAC_SYNCH_REQUEST req);

// table with stack's modules
static const TCPIP_STACK_MODULE_ENTRY  TCPIP_STACK_MODULE_ENTRY_TBL [] =
{
    //ModuleID                  //InitFunc                                          // DeInitFunc                
    {TCPIP_MODULE_ARP,           (tcpipModuleInitFunc)TCPIP_ARP_Initialize,         TCPIP_ARP_Deinitialize},            // TCPIP_MODULE_ARP,
#if defined(TCPIP_STACK_USE_IPV4)
    {TCPIP_MODULE_IPV4,          (tcpipModuleInitFunc)TCPIP_IPV4_Initialize,        TCPIP_IPV4_DeInitialize},           // TCPIP_MODULE_IPV4,
#endif
#if defined(TCPIP_STACK_USE_IPV6)
    {TCPIP_MODULE_IPV6,          (tcpipModuleInitFunc)TCPIP_IPV6_Initialize,        TCPIP_IPV6_Deinitialize},           // TCPIP_MODULE_IPV6
    {TCPIP_MODULE_ICMPV6,        (tcpipModuleInitFunc)TCPIP_ICMPV6_Initialize,      TCPIP_ICMPV6_Deinitialize},         // TCPIP_MODULE_ICMPV6
    {TCPIP_MODULE_NDP,           (tcpipModuleInitFunc)TCPIP_NDP_Initialize,         TCPIP_NDP_Deinitialize},            // TCPIP_MODULE_NDP
#endif
#if defined(TCPIP_STACK_USE_LLDP)
    {TCPIP_MODULE_LLDP,         (tcpipModuleInitFunc)TCPIP_LLDP_Initialize,         TCPIP_LLDP_Deinitialize},           //    TCPIP_MODULE_LLDP
#endif    
#if defined(TCPIP_STACK_USE_ICMP_CLIENT) || defined(TCPIP_STACK_USE_ICMP_SERVER)
    {TCPIP_MODULE_ICMP,          (tcpipModuleInitFunc)TCPIP_ICMP_Initialize,        TCPIP_ICMP_Deinitialize},           // TCPIP_MODULE_ICMP,
#endif
#if defined(TCPIP_STACK_USE_UDP)
    {TCPIP_MODULE_UDP,           (tcpipModuleInitFunc)TCPIP_UDP_Initialize,         TCPIP_UDP_Deinitialize},            // TCPIP_MODULE_UDP,
#endif
#if defined(TCPIP_STACK_USE_TCP)
    {TCPIP_MODULE_TCP,           (tcpipModuleInitFunc)TCPIP_TCP_Initialize,         TCPIP_TCP_Deinitialize},            //  TCPIP_MODULE_TCP,
#endif    
#if defined(TCPIP_STACK_USE_DHCP_CLIENT)
    {TCPIP_MODULE_DHCP_CLIENT,   (tcpipModuleInitFunc)TCPIP_DHCP_Initialize,        TCPIP_DHCP_Deinitialize},           // TCPIP_MODULE_DHCP_CLIENT,
#endif
#if defined(TCPIP_STACK_USE_DHCP_SERVER)
    {TCPIP_MODULE_DHCP_SERVER,   (tcpipModuleInitFunc)TCPIP_DHCPS_Initialize,       TCPIP_DHCPS_Deinitialize},          // TCPIP_MODULE_DHCP_SERVER,
#endif
#if defined(TCPIP_STACK_USE_ANNOUNCE)
    {TCPIP_MODULE_ANNOUNCE,      (tcpipModuleInitFunc)TCPIP_ANNOUNCE_Init,          TCPIP_ANNOUNCE_DeInit},             // TCPIP_MODULE_ANNOUNCE,
#endif
#if defined(TCPIP_STACK_USE_DNS)
    {TCPIP_MODULE_DNS_CLIENT,    (tcpipModuleInitFunc)TCPIP_DNS_ClientInitialize,   TCPIP_DNS_ClientDeinitialize},      // TCPIP_MODULE_DNS_CLIENT,
#endif
#if defined(TCPIP_STACK_USE_NBNS)
    {TCPIP_MODULE_NBNS,          (tcpipModuleInitFunc)TCPIP_NBNS_Initialize,        TCPIP_NBNS_Deinitialize},           // TCPIP_MODULE_NBNS
#endif
#if defined(TCPIP_STACK_USE_SMTP_CLIENT)
    {TCPIP_MODULE_SMTP_CLIENT,   (tcpipModuleInitFunc)TCPIP_SMTP_ClientInitialize,  TCPIP_SMTP_ClientDeinitialize},     // TCPIP_MODULE_SMTP_CLIENT,
#endif
#if defined(TCPIP_STACK_USE_SNTP_CLIENT)
    {TCPIP_MODULE_SNTP,          (tcpipModuleInitFunc)TCPIP_SNTP_Initialize,        TCPIP_SNTP_Deinitialize},           // TCPIP_MODULE_SNTP,
#endif
#if defined(TCPIP_STACK_USE_BERKELEY_API)
    {TCPIP_MODULE_BERKELEY,      (tcpipModuleInitFunc)BerkeleySocketInit,           BerkeleySocketDeInit},              // TCPIP_MODULE_BERKELEY,
#endif
#if defined(TCPIP_STACK_USE_HTTP_SERVER)
    {TCPIP_MODULE_HTTP_SERVER,   (tcpipModuleInitFunc)TCPIP_HTTP_Initialize,        TCPIP_HTTP_Deinitialize},           // TCPIP_MODULE_HTTP_SERVER,
#endif
#if defined(TCPIP_STACK_USE_TELNET_SERVER)
    {TCPIP_MODULE_TELNET_SERVER, (tcpipModuleInitFunc)TCPIP_TELNET_Initialize,      TCPIP_TELNET_Deinitialize},         // TCPIP_MODULE_TELNET_SERVER,
#endif
#if defined(TCPIP_STACK_USE_FTP_SERVER)
    {TCPIP_MODULE_FTP_SERVER,    (tcpipModuleInitFunc)TCPIP_FTP_ServerInitialize,   TCPIP_FTP_ServerDeinitialize},      // TCPIP_MODULE_FTP_SERVER,
#endif
#if defined(TCPIP_STACK_USE_SNMP_SERVER)
    {TCPIP_MODULE_SNMP_SERVER,   (tcpipModuleInitFunc)TCPIP_SNMP_Initialize,        TCPIP_SNMP_Deinitialize},           // TCPIP_MODULE_SNMP_SERVER,
#endif
#if defined(TCPIP_STACK_USE_DNS_SERVER)
    {TCPIP_MODULE_DNS_SERVER,    (tcpipModuleInitFunc)TCPIP_DNSS_Initialize,        TCPIP_DNSS_Deinitialize},           // TCPIP_MODULE_DNS_SERVER,
#endif
#if defined(TCPIP_STACK_USE_DYNAMICDNS_CLIENT)
    {TCPIP_MODULE_DYNDNS_CLIENT, (tcpipModuleInitFunc)TCPIP_DDNS_Initialize,        TCPIP_DDNS_Deinitialize},           // TCPIP_MODULE_DYNDNS_CLIENT,
#endif
#if defined(TCPIP_STACK_USE_REBOOT_SERVER)
    {TCPIP_MODULE_REBOOT_SERVER, (tcpipModuleInitFunc)TCPIP_REBOOT_Initialize,      TCPIP_REBOOT_Deinitialize},         // TCPIP_MODULE_REBOOT_SERVER,
#endif
#if defined(TCPIP_STACK_USE_ZEROCONF_LINK_LOCAL)
    {TCPIP_MODULE_ZCLL,          (tcpipModuleInitFunc)TCPIP_ZCLL_Initialize,        TCPIP_ZCLL_Deinitialize},           // TCPIP_MODULE_ZCLL,
#if defined(TCPIP_STACK_USE_ZEROCONF_MDNS_SD)
    {TCPIP_MODULE_MDNS,          (tcpipModuleInitFunc)TCPIP_MDNS_Initialize,        TCPIP_MDNS_Deinitialize},           // TCPIP_MODULE_MDNS,
#endif
#endif
#if defined(TCPIP_STACK_COMMAND_ENABLE)
    {TCPIP_MODULE_COMMAND,      (tcpipModuleInitFunc)TCPIP_Commands_Initialize,    TCPIP_Commands_Deinitialize},       //    TCPIP_MODULE_COMMAND
#endif
#if defined(TCPIP_STACK_USE_IPERF)
    {TCPIP_MODULE_IPERF,         (tcpipModuleInitFunc)TCPIP_IPERF_Initialize,       TCPIP_IPERF_Deinitialize},          //    TCPIP_MODULE_IPERF
#endif

#if defined(TCPIP_STACK_USE_TFTP_CLIENT)
    {TCPIP_MODULE_TFTP_CLIENT,         (tcpipModuleInitFunc)TCPIP_TFTPC_Initialize,       TCPIP_TFTPC_Deinitialize},          //    TCPIP_MODULE_LLDP
#endif
    // Add other stack modules here
     
};

SYS_MODULE_OBJ TCPIP_STACK_Initialize(const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init)
{
    int                     netIx, ix;
    int                     initFail;
    TCPIP_HEAP_HANDLE       heapH;
    TCPIP_NET_IF*           pIf;
    TCPIP_MAC_POWER_MODE    powerMode;
    int                     nNets, nModules;
    const TCPIP_NETWORK_CONFIG* pUsrConfig;
    const TCPIP_STACK_MODULE_CONFIG* pModConfig;


    if(tcpipNetIf != 0)
    {   // already up and running
        return (SYS_MODULE_OBJ)&tcpip_stack_ctrl_data;
    }

    if(init == 0)
    {   // no initialization data passed
        return SYS_MODULE_OBJ_INVALID;
    }

    pUsrConfig = ((TCPIP_STACK_INIT*)init)->pNetConf;
    nNets = ((TCPIP_STACK_INIT*)init)->nNets;
    pModConfig = ((TCPIP_STACK_INIT*)init)->pModConfig;
    nModules = ((TCPIP_STACK_INIT*)init)->nModules;

    // minimum sanity check
    if(nNets == 0 || pUsrConfig == 0 || pUsrConfig->pMacObject == 0 || pModConfig == 0 || nModules == 0)
    {   // cannot run with no interface/init data
        return SYS_MODULE_OBJ_INVALID;
    }

    // snapshot of the initialization data
    tcpip_init_data = *((TCPIP_STACK_INIT*)init);
    
    SYS_CONSOLE_MESSAGE(TCPIP_STACK_INIT_MESSAGE "Started \n\r");


    while(true)
    {
        initFail = 0;

        totTcpipEventsCnt = 0;

        newTcpipErrorEventCnt = 0;
        newTcpipStackEventCnt = 0;
        newTcpipTickAvlbl = 0;

        // start stack initialization

        memset(&tcpip_stack_ctrl_data, 0, sizeof(tcpip_stack_ctrl_data));

        tcpip_heap = (uint8_t*)TCPIP_STACK_MALLOC_FUNC(TCPIP_STACK_DRAM_SIZE);

        heapH = TCPIP_HEAP_Create(tcpip_heap, TCPIP_STACK_DRAM_SIZE, 0, 0);     // get handle to the heap memory

        if((tcpip_stack_ctrl_data.memH = heapH) == 0)
        {
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "Heap creation failed, size: %d\r\n", TCPIP_STACK_DRAM_SIZE);
            initFail = 1;
            break;
        }

        tcpipNetIf = (TCPIP_NET_IF*)TCPIP_HEAP_Calloc(heapH, nNets, sizeof(TCPIP_NET_IF)); // allocate for each network interface
        if(tcpipNetIf == 0)
        {   // failed
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "Network configuration allocation failed: %d\r\n", nNets * sizeof(TCPIP_NET_IF));
            initFail = 2;
            break;
        }

        if(TCPIP_PKT_Initialize(heapH) == false)
        {
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "TCPIP Packet initialization failed: 0x%x\r\n", heapH);
            initFail = 3;
            break;
        }

        tcpip_stack_ctrl_data.nIfs = nNets;
        tcpip_stack_ctrl_data.nModules = nModules;

        if(!_InitNetConfig(pUsrConfig, nNets))
        {
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "Network configuration initialization failed: %d\r\n", nNets);
            initFail = 4;   // failed the initialization
            break;
        }

        tcpipDefIf.defaultNet = 0;          // delete the old default
        // initialize the signal handlers
        memset(TCPIP_STACK_MODULE_SIGNAL_TBL, 0x0, sizeof(TCPIP_STACK_MODULE_SIGNAL_TBL));
        stackAsyncSignalCount = 0;

        // initialize the processed frames table
        for(ix = 0; ix < sizeof(TCPIP_MODULES_QUEUE_TBL)/sizeof(*TCPIP_MODULES_QUEUE_TBL); ix++)
        {
            TCPIP_Helper_SingleListInitialize(TCPIP_MODULES_QUEUE_TBL + ix);
        }

        // start per interface initializing
        tcpip_stack_ctrl_data.stackAction = TCPIP_STACK_ACTION_INIT;

        for(netIx = 0, pIf = tcpipNetIf; netIx < nNets; netIx++, pIf++, pUsrConfig++)
        {
            // get the power mode
            powerMode = TCPIP_Helper_StringToPowerMode(pUsrConfig->powerMode);
            if(powerMode == TCPIP_MAC_POWER_NONE || powerMode != TCPIP_MAC_POWER_FULL)
            {  
                SYS_ERROR_PRINT(SYS_ERROR_ERROR, "Stack Power Mode initialization fail: %d\r\n", powerMode);
                initFail = 5;
                break;
            }

            // set transient data
            tcpip_stack_ctrl_data.powerMode = powerMode;
            tcpip_stack_ctrl_data.pNetIf = pIf;
            tcpip_stack_ctrl_data.netIx = netIx;
            if(!TCPIP_STACK_BringNetUp(&tcpip_stack_ctrl_data, pUsrConfig, pModConfig, nModules))
            {
                initFail = 6;
                break;
            }

            // interface success
            // set the default interfaces
            if(tcpipDefIf.defaultNet == 0)
            {
                tcpipDefIf.defaultNet = pIf;    // set as the 1st valid interface
            }
        }


        break;
    }

    // initialization done
    if(!initFail)
    {
        size_t heapLeft;
        // check the amount of heap left
        heapLeft = TCPIP_HEAP_FreeSize(heapH);
        if(heapLeft < TCPIP_STACK_DRAM_RUN_LIMIT)
        {
            SYS_ERROR_PRINT(SYS_ERROR_WARNING, "Dynamic memory is low: %d\r\n", heapLeft);
        }
        // continue initialization in TCPIP_STACK_Task()
        tcpip_stack_status = SYS_STATUS_BUSY;
        return (SYS_MODULE_OBJ)&tcpip_stack_ctrl_data;
    }


    SYS_ERROR_PRINT(SYS_ERROR_ERROR, TCPIP_STACK_INIT_MESSAGE "failed %d - Aborting! \n\r", initFail);
    TCPIP_STACK_Deinitialize((SYS_MODULE_OBJ)&tcpip_stack_ctrl_data);
    return SYS_MODULE_OBJ_INVALID;

}

/*********************************************************************
 * Function:        bool TCPIP_STACK_BringNetUp(TCPIP_NET_IF* pNetIf, const TCPIP_NETWORK_CONFIG* pNetConf, const TCPIP_STACK_MODULE_CONFIG* pModConfig, int nModules)
 *
 * PreCondition:    None
 *
 * Input:           net interface to bring up
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function performs the initialization of a net interface
 *
 * Note:            None
 ********************************************************************/
static bool TCPIP_STACK_BringNetUp(TCPIP_STACK_MODULE_CTRL* stackCtrlData, const TCPIP_NETWORK_CONFIG* pNetConf, const TCPIP_STACK_MODULE_CONFIG* pModConfig, int nModules)
{
    TCPIP_NET_IF*           pNetIf;
    bool                    netUpFail;
    const void*             configData;
    const TCPIP_STACK_MODULE_CONFIG* pConfig;
    TCPIP_MAC_MODULE_CTRL   macCtrl;
    SYS_MODULE_INIT         moduleInit;
    const TCPIP_MAC_OBJECT*  pMacObj;

    netUpFail = false;
    pNetIf = stackCtrlData->pNetIf;
    // restore the dynamic interface data
    pNetIf->netIfIx = stackCtrlData->netIx;
    pNetIf->Flags.powerMode = stackCtrlData->powerMode;
    pMacObj = pNetIf->pMacObj;

    if(pNetIf->Flags.powerMode == TCPIP_MAC_POWER_FULL)
    {
        moduleInit.sys.powerState = SYS_MODULE_POWER_RUN_FULL;
    }
    else if(pNetIf->Flags.powerMode == TCPIP_MAC_POWER_LOW)
    {
        moduleInit.sys.powerState = SYS_MODULE_POWER_IDLE_RUN;
    }
    else
    {
        moduleInit.sys.powerState = SYS_MODULE_POWER_IDLE_STOP;
    }


    while(true)
    {
#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
        if(!TCPIP_Notification_Initialize(&pNetIf->registeredClients))
        {
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "%s notification initialization failed\r\n", pMacObj->macName);
            netUpFail = 1;
            break;
        }
#endif
        // start stack MAC modules initialization

        // find MAC initialization data; use old if no new one
        configData = pNetIf->pMacConfig;
        if (pModConfig != 0)
        {
            pConfig = _TCPIP_STACK_FindModuleData(pMacObj->macId, pModConfig, nModules);
            if(pConfig != 0)
            {   // there's new MAC config data
                pNetIf->pMacConfig = configData = pConfig->configData;
            }
        }
        // init the MAC
        {
            TCPIP_STACK_StacktoMacCtrl(&macCtrl, stackCtrlData);
            TCPIP_MAC_INIT macInit =
            {
                { moduleInit.value },
                &macCtrl,
                configData,
            };

            pNetIf->macObjHandle = (*pMacObj->TCPIP_MAC_Initialize)(pMacObj->macId, &macInit.moduleInit);

            if( pNetIf->macObjHandle == SYS_MODULE_OBJ_INVALID)
            {
                pNetIf->macObjHandle = 0;
                SYS_ERROR_PRINT(SYS_ERROR_ERROR, "%s MAC initialization failed\r\n", pMacObj->macName);
                netUpFail = 1;
                break;
            }
        }

        // open the MAC
        pNetIf->hIfMac = (*pMacObj->TCPIP_MAC_Open)(pMacObj->macId, DRV_IO_INTENT_READWRITE);
        if(pNetIf->hIfMac == DRV_HANDLE_INVALID)
        {
            pNetIf->hIfMac = 0;
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "%s MAC Open failed\r\n", pMacObj->macName);
            netUpFail = 1;
            break;
        }

        // start stack initialization per module
        int modIx;
        const TCPIP_STACK_MODULE_ENTRY*  pEntry = TCPIP_STACK_MODULE_ENTRY_TBL + 0;

        for(modIx = 0; modIx < sizeof(TCPIP_STACK_MODULE_ENTRY_TBL)/sizeof(*TCPIP_STACK_MODULE_ENTRY_TBL); modIx++)
        {
            configData = 0;
            if (pModConfig != 0)
            {
                pConfig = _TCPIP_STACK_FindModuleData(pEntry->moduleId, pModConfig, nModules);
                if(pConfig != 0)
                {
                    configData = pConfig->configData;
                }
            }

            if(!pEntry->initFunc(stackCtrlData, configData))
            {
                SYS_ERROR_PRINT(SYS_ERROR_ERROR, "Module no: %d Initialization failed\r\n", pEntry->moduleId);
                netUpFail = 1;
                break;
            }
            pEntry++;
        }

        if(!netUpFail)
        {
#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)

            if(!(*pNetIf->pMacObj->TCPIP_MAC_EventMaskSet)(pNetIf->hIfMac, TCPIP_STACK_MAC_ALL_EVENTS, true))
            {
                SYS_ERROR_PRINT(SYS_ERROR_ERROR, "%s MAC event notification setting failed\r\n", pNetIf->pMacObj->macName);
                netUpFail = 1;
                break;
            }
#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)

            // completed the MAC initialization
        }

        break;
    }

    if(netUpFail)
    {
        return false;
    }

    pNetIf->Flags.bMacInitialize = true;
    pNetIf->Flags.bMacInitDone = false;
    pNetIf->Flags.powerMode = stackCtrlData->powerMode;

    return true;

}

/*********************************************************************
 * Function:        void TCPIP_STACK_NetUp(TCPIP_NET_HANDLE netH, const TCPIP_NETWORK_CONFIG* pUsrConfig)
 *
 * PreCondition:    None
 *
 * Input:           net interface to bring up
 *
 * Output:          true if success
 *                  false if no such network or an error occurred
 *
 * Side Effects:    None
 *
 * Overview:        This function performs the de-initialization of a net interface
 *
 * Note:            None
 ********************************************************************/
bool TCPIP_STACK_NetUp(TCPIP_NET_HANDLE netH, const TCPIP_NETWORK_CONFIG* pUsrConfig)
{
    bool    success;
    TCPIP_MAC_POWER_MODE  powerMode;

    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNet(netH);

    if(pNetIf)
    {
        if(pNetIf->Flags.bInterfaceEnabled || pNetIf->Flags.bMacInitialize)
        {   // already up
            return true;
        }

        if(pUsrConfig == 0)
        {   // no configuration present
            return false;
        }

        // Before we load the default config, we should save what used to be the netIfIx
        // set transient data
        tcpip_stack_ctrl_data.pNetIf = pNetIf;
        tcpip_stack_ctrl_data.netIx = pNetIf->netIfIx;
        tcpip_stack_ctrl_data.stackAction = TCPIP_STACK_ACTION_IF_UP;

        if(!_LoadNetworkConfig(pUsrConfig, pNetIf, true))
        {
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "Network configuration load failed: %d\r\n", pNetIf->netIfIx);
            return false;
        }

        powerMode = TCPIP_Helper_StringToPowerMode(pUsrConfig->powerMode);
        if(powerMode == TCPIP_MAC_POWER_NONE || powerMode != TCPIP_MAC_POWER_FULL)
        {  
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "Stack Power Mode initialization fail: %d\r\n", powerMode);
            return false;
        }

        tcpip_stack_ctrl_data.powerMode = powerMode;

        success = TCPIP_STACK_BringNetUp(&tcpip_stack_ctrl_data, pUsrConfig, 0, 0);
        if(!success)
        {   // don't let the MAC hanging because of a module failure
            tcpip_stack_ctrl_data.stackAction = TCPIP_STACK_ACTION_IF_DOWN;
            tcpip_stack_ctrl_data.powerMode = TCPIP_MAC_POWER_DOWN;
            TCPIP_STACK_BringNetDown(&tcpip_stack_ctrl_data);
        }
        return success;
    }

    return false;

}

void TCPIP_STACK_Deinitialize(SYS_MODULE_OBJ object)
{
    int         netIx;
    TCPIP_NET_IF* pIf;

    if(object != (SYS_MODULE_OBJ)&tcpip_stack_ctrl_data || tcpipNetIf == 0)
    {   // invalid handle/already dead
        return;
    }

    if (tcpip_stack_tickH != SYS_TMR_HANDLE_INVALID)
    {
        SYS_TMR_CallbackStop(tcpip_stack_tickH);
        tcpip_stack_tickH = SYS_TMR_HANDLE_INVALID;
    }

    // set transient data
    tcpip_stack_ctrl_data.stackAction = TCPIP_STACK_ACTION_DEINIT;
    tcpip_stack_ctrl_data.powerMode = TCPIP_MAC_POWER_DOWN;

    for(netIx = 0, pIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pIf++)
    {
        tcpip_stack_ctrl_data.pNetIf = pIf;
        tcpip_stack_ctrl_data.netIx = pIf->netIfIx;
        TCPIP_STACK_BringNetDown(&tcpip_stack_ctrl_data);
    }

    TCPIP_PKT_Deinitialize();
    TCPIP_HEAP_Free(tcpip_stack_ctrl_data.memH, tcpipNetIf);
    TCPIP_HEAP_Delete(tcpip_stack_ctrl_data.memH);     // destroy the heap
    tcpip_stack_ctrl_data.memH = 0;
    tcpipNetIf = 0;

    TCPIP_STACK_FREE_FUNC(tcpip_heap);     // free the allocated memory
    tcpip_heap = 0;

    tcpip_stack_ctrl_data.nIfs = 0;
    tcpip_stack_ctrl_data.nModules = 0;
    tcpip_stack_status = SYS_STATUS_UNINITIALIZED;
    stackAsyncSignalCount = 0;
}

SYS_STATUS TCPIP_STACK_Status ( SYS_MODULE_OBJ object )
{
    if(object != (SYS_MODULE_OBJ)&tcpip_stack_ctrl_data)
    {   // invalid handle
        return SYS_STATUS_ERROR;
    }

    return tcpip_stack_status;
}

bool TCPIP_STACK_InitializeDataGet(SYS_MODULE_OBJ object, TCPIP_STACK_INIT* pStackInit)
{
    if(object != (SYS_MODULE_OBJ)&tcpip_stack_ctrl_data)
    {   // invalid handle
        return false;
    }

    if(pStackInit)
    {
        *pStackInit = tcpip_init_data;
    }

    return true;
}


/*********************************************************************
 * Function:        void TCPIP_STACK_BringNetDown(TCPIP_STACK_MODULE_CTRL* stackCtrlData)
 *
 * PreCondition:    None
 *
 * Input:           net interface to bring down
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function performs the de-initialization of a net interface
 *
 * Note:            None
 ********************************************************************/
static void TCPIP_STACK_BringNetDown(TCPIP_STACK_MODULE_CTRL* stackCtrlData)
{
    const TCPIP_STACK_MODULE_ENTRY*  pEntry;
    TCPIP_NET_IF* pNetIf;


    // Go to the last entry in the table
    pEntry = TCPIP_STACK_MODULE_ENTRY_TBL + sizeof(TCPIP_STACK_MODULE_ENTRY_TBL)/sizeof(*TCPIP_STACK_MODULE_ENTRY_TBL);
    do
    {
        pEntry--;
        pEntry->deInitFunc(stackCtrlData);
    }
    while (pEntry != TCPIP_STACK_MODULE_ENTRY_TBL);

    pNetIf = stackCtrlData->pNetIf;
    _TCPIPStackModuleRxPurge(TCPIP_MODULE_MANAGER, pNetIf);
    if(pNetIf->hIfMac != 0)
    {
        (*pNetIf->pMacObj->TCPIP_MAC_Close)(pNetIf->hIfMac);
        pNetIf->hIfMac = 0;
    }

    if(pNetIf->macObjHandle != 0)
    {   // kill the MAC
        (*pNetIf->pMacObj->TCPIP_MAC_Deinitialize)(pNetIf->macObjHandle);
        pNetIf->macObjHandle = 0;
    }

    pNetIf->Flags.bInterfaceEnabled = pNetIf->Flags.bMacInitialize = false;
    pNetIf->Flags.powerMode = stackCtrlData->powerMode;

#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
        TCPIP_Notification_Deinitialize(&pNetIf->registeredClients, tcpip_stack_ctrl_data.memH);
#endif
}

/*********************************************************************
 * Function:        void TCPIP_STACK_NetDown(TCPIP_NET_HANDLE netH)
 *
 * PreCondition:    None
 *
 * Input:           net interface to bring down
 *
 * Output:          true if success
 *                  false if no such network
 *
 * Side Effects:    None
 *
 * Overview:        This function performs the de-initialization of a net interface
 *
 * Note:            None
 ********************************************************************/
bool TCPIP_STACK_NetDown(TCPIP_NET_HANDLE netH)
{
    int netIx;
    TCPIP_NET_IF *pIf, *pNewIf;
    TCPIP_NET_IF* pDownIf = _TCPIPStackHandleToNet(netH);

    if(pDownIf)
    {
        if(pDownIf->Flags.bInterfaceEnabled || pDownIf->Flags.bMacInitialize)
        {
            // set transient data
            tcpip_stack_ctrl_data.stackAction = TCPIP_STACK_ACTION_IF_DOWN;
            tcpip_stack_ctrl_data.powerMode = TCPIP_MAC_POWER_DOWN;
            tcpip_stack_ctrl_data.pNetIf = pDownIf;
            tcpip_stack_ctrl_data.netIx = pDownIf->netIfIx;

            if(tcpipDefIf.defaultNet == pDownIf)
            {   // since this interface is going down change the default interface
                pNewIf = 0;
                for(netIx = 0, pIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pIf++)
                {
                    if(pIf != pDownIf && pIf->Flags.bInterfaceEnabled)
                    {   // select this one
                        pNewIf = pIf;
                        break;
                    }
                }
                tcpipDefIf.defaultNet = pNewIf;
            }

            TCPIP_STACK_BringNetDown(&tcpip_stack_ctrl_data);
        }
        return true;
    }

    return false;

}




// create the stack tick timer
static bool _TCPIPStackCreateTimer(void)
{

    tcpip_stack_tickH = SYS_TMR_CallbackPeriodic(TCPIP_STACK_TICK_RATE, 0, _TCPIP_STACK_TickHandler);
    if(tcpip_stack_tickH != SYS_TMR_HANDLE_INVALID)
    {
        uint32_t sysRes = SYS_TMR_TickCounterFrequencyGet();
        uint32_t rateMs = ((sysRes * TCPIP_STACK_TICK_RATE) + 999 )/1000;    // round up
        stackTaskRate = (rateMs * 1000) / sysRes;
        // SYS_TMR_CallbackPeriodicSetRate(tcpip_stack_tickH, rateMs);
        return true;
    }


    SYS_ERROR_PRINT(SYS_ERROR_ERROR, "Stack tick registration failed: %d\r\n", TCPIP_STACK_TICK_RATE);
    return false;

}

void TCPIP_STACK_Task(SYS_MODULE_OBJ object)
{
    int                 netIx, modIx;
    TCPIP_NET_IF*       pNetIf;
    TCPIP_MAC_EVENT     activeEvents;
    bool                eventPending;
    bool                wasTickEvent;
#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
    TCPIP_EVENT         tcpipEvent;
    TCPIP_EVENT_LIST_NODE* tNode;
#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)    

    if(object != (SYS_MODULE_OBJ)&tcpip_stack_ctrl_data || tcpipNetIf == 0)
    {   // invalid handle/nothing to do
        return;
    }

    if(tcpip_stack_status != SYS_STATUS_BUSY && tcpip_stack_status != SYS_STATUS_READY)
    {   // some error state
        return;
    }

    if(!_TCPIPStackIsRunState())
    {   // not properly initialized yet
        return;
    }

#if defined(TCPIP_STACK_TIME_MEASUREMENT)
    uint32_t    tTaskStart;
    if(tcpip_stack_timeEnable)
    {
        tTaskStart = _CP0_GET_COUNT();
    }
    TCPIP_Commands_ExecTimeUpdate();
#endif // defined(TCPIP_STACK_TIME_MEASUREMENT)

    // check stack signals
    eventPending = TCPIP_STACK_CheckEventsPending();
    if(eventPending == 0 && stackAsyncSignalCount == 0)
    {   // process only when events are pending or modules need async attention
        return;
    }

    if(newTcpipTickAvlbl != 0)
    {
        wasTickEvent = true;
        _TCPIP_ProcessTickEvent();
    }
    else
    {
        wasTickEvent = false;
    }

    if( totTcpipEventsCnt)
    {   // there are MAC events pending
        totTcpipEventsCnt = 0;

        for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
        {
            if (!pNetIf->Flags.bInterfaceEnabled)
            {
                continue;
            }
            activeEvents =  pNetIf->activeEvents;

#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
            // get a fresh copy
            activeEvents |= (*pNetIf->pMacObj->TCPIP_MAC_EventPendingGet)(pNetIf->hIfMac);
#else
            activeEvents |= TCPIP_MAC_EV_RX_DONE|TCPIP_MAC_EV_TX_DONE;    // just fake pending events
#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)

            // clear processed events
            _TCPIP_ClearMacEvent(pNetIf, activeEvents);
            pNetIf->currEvents |= activeEvents;     // store all the processed events

            // acknowledge MAC events
#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
            (*pNetIf->pMacObj->TCPIP_MAC_EventAcknowledge)(pNetIf->hIfMac, activeEvents);
#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
            if((activeEvents & (TCPIP_STACK_MAC_ACTIVE_RX_EVENTS)) != 0)
            {
                _TCPIPExtractMacRxPackets(pNetIf);
                newTcpipStackEventCnt++;
            }

            if(pNetIf->Flags.bMacProcessOnEvent != 0)
            {   // normal MAC internal processing
                (*pNetIf->pMacObj->TCPIP_MAC_Process)(pNetIf->hIfMac);
            }
        }

        _TCPIPProcessMacPackets(true);

        // clear the pending RX signal so it's not reported
        _TCPIPStackManagerSignalClear(TCPIP_MODULE_SIGNAL_RX_PENDING);
    }

    // process connection related and error events    
    for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
    {
        if (!pNetIf->Flags.bInterfaceEnabled)
        {
            continue;
        }

        activeEvents = pNetIf->currEvents;
        if((activeEvents & TCPIP_MAC_EV_RXTX_ERRORS) != 0)
        {    // some error has occurred
            _TCPIP_ProcessMACErrorEvents(pNetIf, activeEvents);
        }

        if(pNetIf->exFlags.connEvent != 0)
        {   // connection related event
            activeEvents |= pNetIf->exFlags.connEventType ? TCPIP_MAC_EV_CONN_ESTABLISHED : TCPIP_MAC_EV_CONN_LOST;
            pNetIf->exFlags.connEvent = 0;

            for(modIx = 0; modIx < sizeof(TCPIP_STACK_CONN_EVENT_TBL)/sizeof(*TCPIP_STACK_CONN_EVENT_TBL); modIx++)
            {
                (*TCPIP_STACK_CONN_EVENT_TBL[modIx])(pNetIf, activeEvents);
            }
        }

#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
        // notify stack clients 
        if((tcpipEvent = TCPIP_STACK_Mac2TcpipEvent(activeEvents)) != TCPIP_EV_NONE)
        {
            TCPIP_Notification_Lock(&pNetIf->registeredClients);
            for(tNode = (TCPIP_EVENT_LIST_NODE*)pNetIf->registeredClients.list.head; tNode != 0; tNode = tNode->next)
            {
                if((tNode->evMask & tcpipEvent) != 0 )
                {   // trigger event
                    (*tNode->handler)(pNetIf, tcpipEvent, tNode->hParam);
                }
            }
            TCPIP_Notification_Unlock(&pNetIf->registeredClients);
        }
#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)

        pNetIf->currEvents = 0;
    }

    if(wasTickEvent)
    {   // send the timeout signals
        _TCPIPStackSignalTmo();
        // clear the TMO signal so it's not reported anymore
        _TCPIPStackManagerSignalClear(TCPIP_MODULE_SIGNAL_TMO);
}

#if !defined(TCPIP_STACK_APP_EXECUTE_MODULE_TASKS)
    // execute the signal tasks here, instead of letting the app handle that
    _TCPIPStackExecuteModules();
    if(stackAsyncSignalCount != 0)
    {   // when executing the tasks internally
        // signal that attention is required
        _TCPIPSignalEntryNotify(_TCPIPModuleToSignalEntry(TCPIP_MODULE_MANAGER), TCPIP_MODULE_SIGNAL_ASYNC);
    }
#endif  // !defined(TCPIP_STACK_APP_EXECUTE_MODULE_TASKS)

#if defined(TCPIP_STACK_TIME_MEASUREMENT)
    if(tcpip_stack_timeEnable)
    {
        tcpip_stack_time += _CP0_GET_COUNT() - tTaskStart;
    }
#endif // defined(TCPIP_STACK_TIME_MEASUREMENT)

}

static bool _TCPIPStackIsRunState(void)
{
    int             netIx;
    TCPIP_NET_IF*   pNetIf;

    // check that we've created the needed timer
    if(tcpip_stack_status == SYS_STATUS_BUSY && tcpip_stack_tickH == SYS_TMR_HANDLE_INVALID)
    {   // check that we can create a timer
        SYS_STATUS tmrStat = SYS_TMR_ModuleStatusGet(0);
        if(tmrStat == SYS_STATUS_BUSY)
        {   // not ready yet
            return false;
        }

        if(tmrStat == SYS_STATUS_READY)
        {   // try to create the timer
            if(_TCPIPStackCreateTimer() == false)
            {   // failed
                tmrStat = SYS_STATUS_ERROR;
            }
        }
        
        if(tmrStat <= 0)
        {   // something went wrong...
            TCPIP_STACK_Deinitialize((SYS_MODULE_OBJ)&tcpip_stack_ctrl_data);
            tcpip_stack_status = SYS_STATUS_ERROR;
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, TCPIP_STACK_INIT_MESSAGE "SYS TMR failed: %d - Aborting! \n\r", tmrStat);
            return false;
        }
    }

    for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
    {
        if(pNetIf->macObjHandle != 0)
        {
            // process the underlying MAC module tasks
            (*pNetIf->pMacObj->TCPIP_MAC_Tasks)(pNetIf->macObjHandle);

            // check if we're just starting up some interfaces
            if(pNetIf->Flags.bMacInitialize)
            {   // initializing
                SYS_STATUS macStat = (*pNetIf->pMacObj->TCPIP_MAC_Status)(pNetIf->macObjHandle);
                if(macStat < 0)
                {   // failed
                    TCPIP_STACK_NetDown(pNetIf);
                    pNetIf->Flags.bMacInitDone = true;
                }
                else if(macStat == SYS_STATUS_READY)
                {   // get the MAC address and MAC processing flags
                    TCPIP_MAC_PARAMETERS macParams;
                    (*pNetIf->pMacObj->TCPIP_MAC_ParametersGet)(pNetIf->hIfMac, &macParams);
                    memcpy(pNetIf->netMACAddr.v, macParams.ifPhyAddress.v, sizeof(pNetIf->netMACAddr));
                    pNetIf->Flags.bMacProcessOnEvent = macParams.processFlags != TCPIP_MAC_PROCESS_FLAG_NONE;
                    pNetIf->macType = macParams.macType;
                    // enable this interface
                    pNetIf->Flags.bInterfaceEnabled = true;
                    pNetIf->Flags.bMacInitialize = false;
                    pNetIf->Flags.bMacInitDone = true;
                }
            }
        }
    }

    // check we're getting out of Initialize procedure
    if(tcpip_stack_status == SYS_STATUS_BUSY)
    {
        uint32_t    ifUpMask = 0;   // masks of interfaces that are up;
        // max 32 interfaces are assumed for now!
        for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
        {
            if(pNetIf->Flags.bMacInitDone == 0)
            {
                return false;  // not done
            }
            else if(pNetIf->Flags.bInterfaceEnabled != 0)
            {
                ifUpMask |= (1 << netIx);
            }
        }

        // passed through all interfaces
        if(ifUpMask == ((1 << netIx) - 1))
        {   // all interfaces up
            int ifNumber[TCPIP_MAC_TYPES] = { 0 };
            for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
            {
                // set the interfaces name
                strncpy(pNetIf->ifName, TCPIP_STACK_IF_ALIAS_NAME_TBL[pNetIf->macType], sizeof(pNetIf->ifName));
                sprintf(pNetIf->ifName + strlen(pNetIf->ifName), "%d", ifNumber[pNetIf->macType]++);
                pNetIf->ifName[sizeof(pNetIf->ifName) - 1] = 0;
            }
            tcpip_stack_status = SYS_STATUS_READY;
            SYS_CONSOLE_MESSAGE(TCPIP_STACK_INIT_MESSAGE "Ended - success \n\r");
        }
        else
        {   // failed initializing all interfaces;
            for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
            {
                if(pNetIf->Flags.bInterfaceEnabled)
                {
                    TCPIP_STACK_NetDown(pNetIf);
                }
            }
            tcpip_stack_status = SYS_STATUS_ERROR;
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, TCPIP_STACK_INIT_MESSAGE "failed: 0x%x - Aborting! \n\r", ifUpMask);
            return false;
        }
    }

    return true;
}

#if !defined(TCPIP_STACK_APP_EXECUTE_MODULE_TASKS)
static void _TCPIPStackExecuteModules(void)
{
    int     modIx;
    TCPIP_MODULE_SIGNAL_ENTRY*  pSigEntry;
    tcpipModuleSignalHandler    signalHandler;
    uint16_t                    signalVal;
    OSAL_CRITSECT_DATA_TYPE     critSect;

    pSigEntry = TCPIP_STACK_MODULE_SIGNAL_TBL + TCPIP_MODULE_LAYER1;
    for(modIx = TCPIP_MODULE_LAYER1; modIx < sizeof(TCPIP_STACK_MODULE_SIGNAL_TBL)/sizeof(*TCPIP_STACK_MODULE_SIGNAL_TBL); modIx++, pSigEntry++)
    {
        critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
        signalHandler = pSigEntry->signalHandler;
        signalVal = pSigEntry->signalVal;
        OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, critSect);

        if(signalHandler == 0 || signalVal == 0)
        {   // unused slot or no pending signals
            continue;
        }
        // pending signals; either TMO or RX related or ASYNC
        // execute the handler-> module Task function
        // Note: this can set signals for sibling modules!
        (*signalHandler)();
    }
}
#endif  // !defined(TCPIP_STACK_APP_EXECUTE_MODULE_TASKS)

static void _TCPIP_ProcessTickEvent(void)
{
    int     netIx;
    TCPIP_NET_IF* pNetIf;
    bool    linkCurr;

    newTcpipTickAvlbl = 0;

    for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
    {
        if(pNetIf->Flags.bInterfaceEnabled)
        {
            linkCurr = (*pNetIf->pMacObj->TCPIP_MAC_LinkCheck)(pNetIf->hIfMac);     // check link status
            if(pNetIf->exFlags.linkPrev != linkCurr)
            {   // link status changed
                // just set directly the events, and do not involve the MAC notification mechanism
                pNetIf->exFlags.connEvent = 1;
                pNetIf->exFlags.connEventType = linkCurr ? 1 : 0 ;
                pNetIf->exFlags.linkPrev = linkCurr;
            }
        }
    }

}

static int _TCPIPExtractMacRxPackets(TCPIP_NET_IF* pNetIf)
{
    TCPIP_MAC_PACKET*       pRxPkt;
    int     nPackets = 0;

    // get all the new MAC packets
    while((pRxPkt = (*pNetIf->pMacObj->TCPIP_MAC_PacketRx)(pNetIf->hIfMac, 0, 0)) != 0)
    {
        _TCPIPInsertMacRxPacket(pNetIf, pRxPkt);
        nPackets++;
    }

    return nPackets;
}

// Process the queued RX packets
// returns the mask of 1st layer frames that have been processed
// signals the 1st layer modules if needed
static uint32_t _TCPIPProcessMacPackets(bool signal)
{
    int                         frameIx;
    bool                        frameFound;
    uint16_t                    frameType;
    TCPIP_MAC_PACKET*           pRxPkt;
    TCPIP_MAC_ETHERNET_HEADER*  pMacHdr;
    const TCPIP_FRAME_PROCESS_ENTRY*  pFrameEntry;
    uint32_t                    procFrameMask = 0;

    SINGLE_LIST*                pPktQueue = (TCPIP_MODULES_QUEUE_TBL + TCPIP_MODULE_MANAGER);


    while((pRxPkt = (TCPIP_MAC_PACKET*)TCPIP_Helper_SingleListHeadRemove(pPktQueue)))
    {
        pMacHdr = (TCPIP_MAC_ETHERNET_HEADER*)pRxPkt->pMacLayer;
        // get the packet type
        frameType = TCPIP_Helper_ntohs(pMacHdr->Type);

        frameFound = false;
        pFrameEntry = TCPIP_FRAME_PROCESS_TBL;
        for(frameIx = 0; frameIx < sizeof(TCPIP_FRAME_PROCESS_TBL) / sizeof(*TCPIP_FRAME_PROCESS_TBL); frameIx++, pFrameEntry++)
        {
            if(pFrameEntry->frameType == frameType)
            {   // found proper frame handler
                pRxPkt->pktFlags &= ~TCPIP_MAC_PKT_FLAG_TYPE_MASK;
                pRxPkt->pktFlags |= pFrameEntry->pktTypeFlags;

                _TCPIPStackModuleRxInsert(pFrameEntry->moduleId, pRxPkt, 0);
                if(signal)
                {   // signal to the module that RX is pending; if not already done so
                    if((procFrameMask & (1 << frameIx)) == 0)
                    {   // set the frame mask so we don't signal again
                        procFrameMask |= 1 << frameIx;
                        _TCPIPModuleSignalSetNotify(pFrameEntry->moduleId, TCPIP_MODULE_SIGNAL_RX_PENDING);
                    }
                }
                TCPIP_PKT_FlightLog(pRxPkt, pFrameEntry->moduleId, TCPIP_MAC_PKT_ACK_RX_OK, 0);
                frameFound = true;
                break;
            }
        }
        if(!frameFound)
        {   // unknown packet type; discard
            TCPIP_PKT_FlightLog(pRxPkt, TCPIP_MODULE_NONE, TCPIP_MAC_PKT_ACK_TYPE_ERR, 0);
            TCPIP_PKT_PacketAcknowledge(pRxPkt, TCPIP_MAC_PKT_ACK_TYPE_ERR); 
        }
    }

    return procFrameMask;
}

#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
// MAC ISR call
static void    _TCPIP_MacEventCB(TCPIP_MAC_EVENT event, const void* hParam)
{
    _TCPIP_NetIfEvent((TCPIP_NET_IF*)hParam, event, false);
}
#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)

static void _TCPIP_NetIfEvent(TCPIP_NET_IF* pNetIf, TCPIP_MAC_EVENT event, bool isrProtect)
{
    uint32_t isrSuspLvl;

    TCPIP_MODULE_SIGNAL_ENTRY* pSigEntry;
    pSigEntry = ((event & (TCPIP_STACK_MAC_ACTIVE_RX_EVENTS)) != 0) ? _TCPIPModuleToSignalEntry(TCPIP_MODULE_MANAGER) : 0;
   

    if(isrProtect)
    {
        isrSuspLvl = _TCPIPMacIsrSuspend(pNetIf);
    }

    pNetIf->activeEvents |= event;
    totTcpipEventsCnt++;

    if(pSigEntry)
    {
        pSigEntry->signalVal |= TCPIP_MODULE_SIGNAL_RX_PENDING;
    }

    if(isrProtect)
    {
        _TCPIPMacIsrResume(pNetIf, isrSuspLvl);
    }

    if(pSigEntry)
    {
        _TCPIPSignalEntryNotify(pSigEntry, TCPIP_MODULE_SIGNAL_RX_PENDING);
    }
}


/*******************************************************************************
  Function:
    void    _TCPIP_STACK_TickHandler(uintptr_t context, uint32_t currTick)

  Summary:
    Stack tick handler.

  Description:
    This function is called from within the System Tick ISR.
    It provides the Stack tick processing.
    It will call the notification handler registered with SYS_TMR_CallbackPeriodic


  Precondition:
   System Tick should have been initialized
   and the Stack tick handler should have been registered with the SYS_TMR_CallbackPeriodic.

  Parameters:
    currSysTick   - current system tick value at the time of call

  Returns:
    None

  Remarks:
    To avoid synchronization issues between the TCP/IP threads and TMR thread
    or MAC ISR and TMR ISR
    the TMO signal is used on a different signal entry: TCPIP_MODULE_NONE!
*****************************************************************************/
static void _TCPIP_STACK_TickHandler(uintptr_t context, uint32_t currTick)
{
    newTcpipTickAvlbl++;

    TCPIP_MODULE_SIGNAL_ENTRY* pTmoEntry = _TCPIPModuleToSignalEntry(TCPIP_MODULE_NONE);
    TCPIP_MODULE_SIGNAL_ENTRY* pMgrEntry = _TCPIPModuleToSignalEntry(TCPIP_MODULE_MANAGER);

    pTmoEntry->signalVal |= TCPIP_MODULE_SIGNAL_TMO;

    _TCPIPSignalEntryNotify(pMgrEntry, TCPIP_MODULE_SIGNAL_TMO);

}

static void _TCPIP_ProcessMACErrorEvents(TCPIP_NET_IF* pNetIf, TCPIP_MAC_EVENT activeEvent)
{
    newTcpipErrorEventCnt++;
}





/*********************************************************************
 * Function:        TCPIP_STACK_NetDefaultGet(void)
 *
 * PreCondition:    TCPIP stack should have been initialized by
 *                    TCPIP_STACK_Initialize()
 *
 * Input:           None
 *
 * Output:          The default net interface for multi-homed hosts
 *
 * Side Effects:    None
 *
 * Note:            Function to dynamically change the default interface
 *                  will be added.
 ********************************************************************/
TCPIP_NET_HANDLE TCPIP_STACK_NetDefaultGet(void)
{
    return tcpipDefIf.defaultNet;
}

// sets the default interface
// returns true if success,
// false if failed (the old interface does not change)
bool TCPIP_STACK_NetDefaultSet(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNewIf = _TCPIPStackHandleToNetUp(netH);
    if(pNewIf)
    {
        tcpipDefIf.defaultNet = pNewIf;
        return true;
    }

    return false;
}

/*********************************************************************
 * Function:        TCPIP_STACK_IPAddToNet(IPV4_ADDR* pIpAddress, bool useDefault)
 *
 * PreCondition:    TCPIP stack should have been initialized by
 *                    TCPIP_STACK_Initialize()
 *
 * Input:           pIpAddress - pointer to an IP address
 *
 *                  useDefault - when no interface is found,
 *                               if true: return the default interface
 *                               else return 0;
 *
 * Output:          Resolves a local IP address to a network interface.
 *
 *
 * Side Effects:    None
 *
 * Note:            None
 ********************************************************************/
TCPIP_NET_IF* TCPIP_STACK_IPAddToNet(IPV4_ADDR* pIpAddress, bool useDefault)
{
    TCPIP_NET_IF* pNetIf = 0;

    if(pIpAddress && pIpAddress->Val != 0)
    {
        pNetIf = TCPIP_STACK_NetByAddress(pIpAddress);
    }

    if(pNetIf == 0 && useDefault)
    {
        pNetIf = tcpipDefIf.defaultNet;
    }

    return pNetIf;
}

/*********************************************************************
 * Function:        _TCPIPStackIpAddFromAnyNet(TCPIP_NET_IF* pNetIf, IPV4_ADDR* pIpAddress)
 *
 * PreCondition:    TCPIP stack should have been initialized by
 *                    TCPIP_STACK_Initialize()
 *
 * Input:           pNetIf  - network interface to check for
 *                            if 0 all interfaces are checked
 *                  pIpAddress - pointer to an IP address
 *
 * Output:          Resolves a local IP address to a network interface
 *                  to which it belongs
 *
 * Side Effects:    None
 *
 * Note:            None
 ********************************************************************/
TCPIP_NET_IF* _TCPIPStackIpAddFromAnyNet(TCPIP_NET_IF* pNetIf, IPV4_ADDR* pIpAddress)
{
    int netIx;
    TCPIP_NET_IF* pIf;

    if(pIpAddress && pIpAddress->Val != 0)
    {
        for(netIx = 0, pIf = tcpipNetIf ; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pIf++)
        {
            if(pNetIf == 0 || pIf == pNetIf)
            {
                if(pIf->Flags.bInterfaceEnabled)
                {
                    if(_TCPIPStackIpAddFromLAN(pIf, pIpAddress))
                    {
                        return pIf;
                    }
                }
            }
        }
    }

    return 0;
}

/*********************************************************************
 * Function:        TCPIP_STACK_MacToNet(TCPIP_MAC_HANDLE hMac)
 *
 * PreCondition:    TCPIP stack should have been initialized by
 *                    TCPIP_STACK_Initialize()
 *
 * Input:           None
 *
 * Output:          Resolves a MAC Id to an tcpipNetIf entry.
 *
 *
 * Side Effects:    None
 *
 * Note:            The tcpipNetIf entries match 1 to 1 the network interfaces
 *                  A more efficient algorithm to find MAC<->tcpipNetIf entry correspondence
 *                  will be eventually added.
 ********************************************************************/
TCPIP_NET_IF* TCPIP_STACK_MacToNet(TCPIP_MAC_HANDLE hMac)
{
    int netIx;
    TCPIP_NET_IF* pNetIf;

    for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
    {
        if(pNetIf->hIfMac == hMac)
        {
            return pNetIf;
        }
    }


    return 0;
}

/*********************************************************************
 * Function:        TCPIP_STACK_MacToNetIndex(TCPIP_MAC_HANDLE hMac)
 *
 * PreCondition:    TCPIP stack should have been initialized by
 *                    TCPIP_STACK_Initialize()
 *
 * Input:           None
 *
 * Output:          Resolves a MAC to an tcpipNetIf entry.
 *
 *
 * Side Effects:    None
 *
 * Note:            The tcpipNetIf entries match 1 to 1 the network interfaces
 *                  A more efficient algorithm to find MAC<->tcpipNetIf entry correspondence
 *                  will be eventually added.
 ********************************************************************/
int TCPIP_STACK_MacToNetIndex(TCPIP_MAC_HANDLE hMac)
{
    int netIx;
    TCPIP_NET_IF* pNetIf;

    for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
    {
        if(pNetIf->hIfMac == hMac)
        {
            return netIx;
        }
    }


    return -1;
}

/*********************************************************************
 * Function:        int TCPIP_STACK_NumberOfNetworksGet(void)
 *
 * PreCondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
 *
 * Input:           None
 *
 * Output:          Number of network interfaces
 *
 * Side Effects:    None
 *
 * Note:            None
 ********************************************************************/
int TCPIP_STACK_NumberOfNetworksGet(void)
{
    return tcpip_stack_ctrl_data.nIfs;
}

/*********************************************************************
 * Function:        TCPIP_STACK_IndexToNet(int netIx)
 *
 * PreCondition:    TCPIP stack should have been initialized by
 *                    TCPIP_STACK_Initialize()
 *
 * Input:           None
 *
 * Output:          Resolves an index to an tcpipNetIf entry.
 *
 *
 * Side Effects:    None
 *
 * Note:            The tcpipNetIf entries match 1 to 1 the network interfaces
 ********************************************************************/
TCPIP_NET_HANDLE TCPIP_STACK_IndexToNet(int netIx)
{
    if(netIx < tcpip_stack_ctrl_data.nIfs)
    {
        return tcpipNetIf + netIx;
    }

    return 0;
}

int  TCPIP_STACK_NetIndexGet(TCPIP_NET_HANDLE hNet)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNet(hNet);
    return TCPIP_STACK_NetIxGet(pNetIf);
}

/*********************************************************************
 * Function:        TCPIP_STACK_MACIdToNet(TCPIP_STACK_MODULE macId)
 *
 * PreCondition:    TCPIP stack should have been initialized by
 *                    TCPIP_STACK_Initialize()
 *
 * Input:           None
 *
 * Output:          Resolves an MAC id to a Net entry.
 *
 *
 * Side Effects:    None
 *
 * Note:            In multi-homed hosts with multiple
 *                  interfaces of the same type,
 *                  the translation might not be unique.
 *                  The first match is returned!
 *
 ********************************************************************/
TCPIP_NET_IF* TCPIP_STACK_MACIdToNet(TCPIP_STACK_MODULE macId)
{
    TCPIP_NET_IF* pNetIf;

    if(macId != (TCPIP_STACK_MODULE)TCPIP_MODULE_MAC_NONE)
    {
        int netIx;
        for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
        {
            if(pNetIf->macId == macId)
            {
                return pNetIf;
            }
        }
    }


    return 0;

}

/*********************************************************************
 * Function:        TCPIP_NET_HANDLE TCPIP_STACK_NetHandleGet(const char* interface)
 *
 * PreCondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
 *
 * Input:           interface - The names specified in TCPIP_NETWORK_CONFIG.
 *
 * Output:          Resolves an interface name to a handle.
 *
 * Side Effects:    None
 *
 * Example:            TCPIP_NET_HANDLE hNet = TCPIP_STACK_NetHandleGet("PIC32INT\r\n");
 *
 * Note:            None
 ********************************************************************/
TCPIP_NET_HANDLE TCPIP_STACK_NetHandleGet(const char* interface)
{
    // try first the aliases
    int netIx;
    TCPIP_NET_IF* pNetIf;

    for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
    {
        if(strcmp(pNetIf->ifName, interface) == 0)
        {
            return (TCPIP_NET_HANDLE)pNetIf;
        }
    }

    // try the full interface name


    return TCPIP_STACK_MACIdToNet(TCPIP_STACK_StringToMACId(interface));
}


/*********************************************************************
 * Function:        const char* TCPIP_STACK_NetNameGet(TCPIP_NET_HANDLE netH)
 *
 * PreCondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
 *
 * Input:           netH - Interface handle to get the name of.
 *
 * Output:          it returns the name associated to that interface handle
 *                     returns 0 if no such name
 *
 * Side Effects:    None
 *
 * Example:            TCPIP_NET_HANDLE netH = TCPIP_STACK_IndexToNet(0);
 *                     const char* netName = TCPIP_STACK_NetNameGet(netH);
 *
 * Note:            None
 ********************************************************************/
const char* TCPIP_STACK_NetNameGet(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF*  pNetIf = _TCPIPStackHandleToNet(netH);

    if(pNetIf != 0 && pNetIf->pMacObj != 0)
    {
        return pNetIf->pMacObj->macName;
    }

    return 0;
}

int TCPIP_STACK_NetAliasNameGet(TCPIP_NET_HANDLE netH, char* nameBuffer, int buffSize)
{
    int aliasSize;
    TCPIP_NET_IF*  pNetIf = _TCPIPStackHandleToNet(netH);


    if(pNetIf)
    {
        aliasSize = strlen(pNetIf->ifName);
    }
    else
    {
        aliasSize = 0;
    }

    if(nameBuffer && buffSize)
    {   // valid buffer
        if(aliasSize)
        {
            strncpy(nameBuffer, pNetIf->ifName, buffSize - 1);
            nameBuffer[buffSize - 1] = 0;
        }
        else
        {
            nameBuffer[0] = 0;
        }
    }
    return aliasSize;
}


const TCPIP_MAC_OBJECT* TCPIP_STACK_MACObjectGet(TCPIP_NET_HANDLE netH)
{

    TCPIP_NET_IF*  pNetIf = _TCPIPStackHandleToNet(netH);

    if(pNetIf)
    {
        return pNetIf->pMacObj;
    }

    return 0;
}



/*********************************************************************
 * Function:        TCPIP_STACK_NetByAddress(const IPV4_ADDR* pIpAddress)
 *
 * PreCondition:    TCPIP stack should have been initialized by
 *                    TCPIP_STACK_Initialize()
 *
 * Input:           pointer to an IP address
 *
 * Output:          The network interface pointer to which this ip
 *                  address belongs to.
 *                  NULL if not one of our addresses.
 *
 * Side Effects:    None
 *
 * Note:            A single network interface can support multiple IP addresses.
 *                  For now this feature is not implemented/supported.
 *
 ********************************************************************/
TCPIP_NET_IF* TCPIP_STACK_NetByAddress(const IPV4_ADDR* pIpAddress)
{
    int netIx;
    TCPIP_NET_IF* pIf;

    for(netIx = 0, pIf = tcpipNetIf ; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pIf++)
    {
        if(pIf->Flags.bInterfaceEnabled && pIf->netIPAddr.Val == pIpAddress->Val)
        {
            return pIf;
        }
    }


    return 0;
}

/*********************************************************************
 * Function:        uint32_t TCPIP_STACK_NetAddress(TCPIP_NET_HANDLE netH)
 *
 * PreCondition:    TCPIP stack should have been initialized by TCPIP_STACK_Initialize()
 *
 * Input:           netH - Interface handle to get address of.
 *
 * Output:          The IP address of an interface.
 *
 * Side Effects:    None
 *
 * Example:            TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT\r\n");
 *                     uint32_t ipAdd = TCPIP_STACK_NetAddress(netH);
 *
 * Note:            None
 ********************************************************************/
uint32_t TCPIP_STACK_NetAddress(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    return TCPIP_STACK_NetAddressGet(pNetIf);
}


#if defined(TCPIP_STACK_USE_IPV6)
IPV6_ADDR_HANDLE TCPIP_STACK_NetIPv6AddressGet(TCPIP_NET_HANDLE netH, IPV6_ADDR_TYPE addType, IPV6_ADDR_STRUCT* pAddStruct, IPV6_ADDR_HANDLE addHandle)
{
    IPV6_ADDR_STRUCT * addrNode;
    IPV6_INTERFACE_CONFIG*  pIpv6Config;

    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf == 0)
    {
        return 0;
    }


    // Note: for both unicast and multicast addresses we start from unicast list
    // that's because we need to construct the solicited node multicas address
    // which is not currently stored in its own list!

    pIpv6Config = TCPIP_IPV6_InterfaceConfigGet(pNetIf);
    if(addHandle == 0)
    {   // start iteration through the list
        addrNode = (IPV6_ADDR_STRUCT *)pIpv6Config->listIpv6UnicastAddresses.head;
    }
    else
    {
        addrNode = ((IPV6_ADDR_STRUCT*)addHandle)->next;
    }

    if(addType == IPV6_ADDR_TYPE_UNICAST)
    {
        if(addrNode && pAddStruct)
        {
            memcpy(pAddStruct, addrNode, sizeof(*addrNode));
            pAddStruct->next = pAddStruct->prev = 0;
        }
        return addrNode;
    }

    if(addType == IPV6_ADDR_TYPE_MULTICAST)
    {
        if(addrNode == 0)
        {
            if(addHandle == 0 || ((IPV6_ADDR_STRUCT*)addHandle)->flags.type == IPV6_ADDR_TYPE_UNICAST)
            {   // either the unicast list is empty or finished the unicast list
                addrNode = (IPV6_ADDR_STRUCT *)pIpv6Config->listIpv6MulticastAddresses.head;
            }
        }

        if(addrNode != 0 && addrNode->flags.type == IPV6_ADDR_TYPE_UNICAST)
        {   // do not report the same solicited node address multiple times
            IPV6_ADDR_STRUCT * unicastHead = (IPV6_ADDR_STRUCT *)pIpv6Config->listIpv6UnicastAddresses.head;
            IPV6_ADDR_STRUCT * currAddress = unicastHead;
            while(currAddress != addrNode)
            {
                if(memcmp(addrNode->address.v + sizeof (IPV6_ADDR) - 3, currAddress->address.v + sizeof (IPV6_ADDR) - 3, 3) == 0)
                {   // address match; skip this one
                    addrNode = addrNode->next;
                    if(addrNode == 0)
                    {   // end of list
                        addrNode = (IPV6_ADDR_STRUCT *)pIpv6Config->listIpv6MulticastAddresses.head;
                        break;
                    }
                    else
                    {   // restart traversal
                        currAddress = unicastHead; 
                    }
                }
                else
                {
                    currAddress = currAddress->next;
                }
            }
        }


        if(addrNode && pAddStruct)
        {
            memcpy(pAddStruct, addrNode, sizeof(*addrNode));
            pAddStruct->next = pAddStruct->prev = 0;
            if(addrNode->flags.type == IPV6_ADDR_TYPE_UNICAST)
            {   // construct the solicited node multicast address
                memcpy(pAddStruct->address.v, IPV6_SOLICITED_NODE_MULTICAST.v, sizeof (IPV6_ADDR) - 3);
                pAddStruct->flags.type = IPV6_ADDR_TYPE_MULTICAST;
            }
        }
        return addrNode;
    }


    // no other address type supported
    return 0;
}

// finds an interface that has the IPv6 address
TCPIP_NET_IF* _TCPIPStackIPv6AddToNet(IPV6_ADDR* pIPv6Address, IPV6_ADDR_TYPE addType, bool useDefault)
{
    TCPIP_NET_IF* pNetIf;
    int           netIx;
    TCPIP_NET_IF* pSrchIf = 0;

    if(pIPv6Address != 0)
    {
        for(netIx = 0; netIx < tcpip_stack_ctrl_data.nIfs; netIx++)
        {
            pNetIf = tcpipNetIf + netIx;
            if(TCPIP_IPV6_AddressFind(pNetIf, pIPv6Address, addType) != 0)
            {    // found interface
                pSrchIf = pNetIf;
                break;
            }
        }
    }

    if(pSrchIf == 0 && useDefault)
    {
        pSrchIf = tcpipDefIf.defaultNet;
    }

    return pSrchIf;
}



#endif  // defined(TCPIP_STACK_USE_IPV6)




bool TCPIP_STACK_NetAddressSet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress, IPV4_ADDR* mask, bool setDefault)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        TCPIP_STACK_NetworkAddressSet(pNetIf, ipAddress, mask, setDefault);
        return true;
    }

    return false;
}
bool TCPIP_STACK_NetAddressGatewaySet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        TCPIP_STACK_GatewayAddressSet(pNetIf, ipAddress);
        return true;
    }

    return false;
}

uint32_t TCPIP_STACK_NetAddressGateway(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);

    if(pNetIf)
    {
        return pNetIf->netGateway.Val;
    }

    return 0;
}

uint32_t TCPIP_STACK_NetAddressDnsPrimary(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);

    if(pNetIf)
    {
        return pNetIf->PrimaryDNSServer.Val;
    }

    return 0;
}

uint32_t TCPIP_STACK_NetAddressDnsSecond(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);

    if(pNetIf)
    {
        return pNetIf->SecondaryDNSServer.Val;
    }

    return 0;
}

bool TCPIP_STACK_NetAddressDnsPrimarySet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    TCPIP_STACK_PrimaryDNSAddressSet(pNetIf, ipAddress);

    return pNetIf != 0;
}

bool TCPIP_STACK_NetAddressDnsSecondSet(TCPIP_NET_HANDLE netH, IPV4_ADDR* ipAddress)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        TCPIP_STACK_SecondaryDNSAddressSet(pNetIf, ipAddress);
        return true;
    }

    return false;
}

/*********************************************************************
 * Function:        uint32_t TCPIP_STACK_NetMask(TCPIP_NET_HANDLE netH)
 *
 * PreCondition:    None
 *
 * Input:           netH - Interface handle to get address of.
 *
 * Output:          if interface enabled then Value of subnet mask
 *                     else 0
 *
 * Example:         TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("PIC32INT\r\n");
 *                     uint32_t subMask = TCPIP_STACK_NetMask(netH);
 *
 * Note:            None
 ********************************************************************/
uint32_t TCPIP_STACK_NetMask(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        return TCPIP_STACK_NetMaskGet(pNetIf);
    }

    return 0;
}


bool TCPIP_STACK_NetAddressMacSet(TCPIP_NET_HANDLE netH, const TCPIP_MAC_ADDR* pAddr)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        memcpy(pNetIf->netMACAddr.v, pAddr->v, sizeof(pNetIf->netMACAddr));
        return true;
    }

    return false;
}

const char* TCPIP_STACK_NetBIOSName(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNet(netH);
    if(pNetIf)
    {
        return (const char*)pNetIf->NetBIOSName;
    }

    return 0;

}

bool TCPIP_STACK_NetBiosNameSet(TCPIP_NET_HANDLE netH, const char* biosName)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        memcpy(pNetIf->NetBIOSName, biosName, sizeof(pNetIf->NetBIOSName));
        TCPIP_Helper_FormatNetBIOSName(pNetIf->NetBIOSName);
        return true;
    }

    return false;
}

const uint8_t* TCPIP_STACK_NetAddressMac(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        return TCPIP_STACK_NetMACAddressGet(pNetIf);
    }

    return 0;
}

bool TCPIP_STACK_SetLocalMasksType(TCPIP_NET_HANDLE netH, TCPIP_LOCAL_MASK_TYPE andType, TCPIP_LOCAL_MASK_TYPE orType)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);

    if(pNetIf)
    {
        pNetIf->exFlags.localAndType = andType;
        pNetIf->exFlags.localOrType = orType;
        return true;
    }

    return false;
}

bool TCPIP_STACK_SetLocalMasks(TCPIP_NET_HANDLE netH, uint32_t andMask, uint32_t orMask)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);

    if(pNetIf)
    {
        pNetIf->localAndMask.Val = andMask;
        pNetIf->localOrMask.Val = orMask;
        return true;
    }

    return false;
}

TCPIP_STACK_MODULE  TCPIP_STACK_NetMACIdGet(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        return TCPIP_STACK_NetMACId(pNetIf);
    }

    return TCPIP_MODULE_NONE;
}

bool TCPIP_STACK_NetMACStatisticsGet(TCPIP_NET_HANDLE netH, TCPIP_MAC_RX_STATISTICS* pRxStatistics, TCPIP_MAC_TX_STATISTICS* pTxStatistics)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        TCPIP_MAC_RES res = (*pNetIf->pMacObj->TCPIP_MAC_StatisticsGet)(pNetIf->hIfMac, pRxStatistics, pTxStatistics);
        if(res == TCPIP_MAC_RES_OK)
        {
            return true;
        }
    }

    return false;
}

bool  TCPIP_STACK_NetMACRegisterStatisticsGet(TCPIP_NET_HANDLE netH, TCPIP_MAC_STATISTICS_REG_ENTRY* pRegEntries, int nEntries, int* pHwEntries)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        TCPIP_MAC_RES res = (*pNetIf->pMacObj->TCPIP_MAC_RegisterStatisticsGet)(pNetIf->hIfMac, pRegEntries, nEntries, pHwEntries);
        if(res == TCPIP_MAC_RES_OK)
        {
            return true;
        }
    }

    return false;
}


// only MAC support for now!
size_t TCPIP_STACK_ModuleConfigGet(TCPIP_STACK_MODULE modId, void* configBuff, size_t buffSize, size_t* pNeededSize)
{
    if(tcpipNetIf != 0)
    {   // we should be up and running for this

        int netIx;
        TCPIP_NET_IF* pNetIf;

        for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
        {
            if(pNetIf->macId == modId)
            {
                if(pNetIf->pMacObj != 0)
                {   // found MAC module
                    return (*pNetIf->pMacObj->TCPIP_MAC_ConfigGet)(pNetIf->hIfMac, configBuff, buffSize, pNeededSize);
                }
            }
        }
    }


    // not found
    return -1;
}

// all the parameters are returned without checking
// that the interface is enabled or not!
size_t TCPIP_STACK_NetConfigGet(TCPIP_NET_HANDLE netH, void* configStoreBuff, size_t configStoreSize, size_t* pNeededSize)
{
    TCPIP_NET_IF* pNetIf;
    TCPIP_STACK_NET_IF_DCPT* pNetStg;

    if(tcpipNetIf == 0 || (pNetIf = _TCPIPStackHandleToNet(netH))== 0)
    {   // we should be up and running and have a valid IF
        return -1;
    }

    if(pNeededSize == 0 && (configStoreBuff == 0 || configStoreSize == 0 ))
    {   // nothing to do
        return 0;
    }
    // store needed size
    if(pNeededSize)
    {
        *pNeededSize = sizeof(*pNetStg); 
    }

    if(configStoreBuff && configStoreSize >= sizeof(*pNetStg))
    {   // copy all the fields
        pNetStg = (TCPIP_STACK_NET_IF_DCPT*)configStoreBuff;

        // the TCPIP_STACK_NET_IF_DCPT has to be at the very beginning of the pNetIf !!!
        memcpy(pNetStg, &pNetIf->size, sizeof(*pNetStg));
        // update the size field
        pNetStg->size = sizeof(*pNetStg);

        return sizeof(*pNetStg);
    }

    return 0;
}

static void* _NetConfigStringToBuffer(void** ppDstBuff, void* pSrcBuff, size_t* pDstSize, size_t* pNeedLen, size_t* pActLen)
{
    size_t  currLen;

    currLen = strlen(pSrcBuff) + 1;

    *pNeedLen += currLen;
    if(currLen && currLen <= *pDstSize)
    {
        void* pCopy = *ppDstBuff;
        memcpy(*ppDstBuff, pSrcBuff, currLen);
        *ppDstBuff += currLen;
        *pDstSize -= currLen;
        *pActLen += currLen;
        return pCopy;
    }
    else
    {   // stop copying
        *pDstSize = 0;
        return 0;
    }

}

// restores pNetConfig from configBuff
TCPIP_NETWORK_CONFIG*   TCPIP_STACK_NetConfigSet(void* configStoreBuff, void* netConfigBuff, size_t buffSize, size_t* pNeededSize)
{
    TCPIP_NETWORK_CONFIG* pNetConf;            
    TCPIP_STACK_NET_IF_DCPT* pNetStg = (TCPIP_STACK_NET_IF_DCPT*)configStoreBuff;

    if(configStoreBuff == 0 || (pNeededSize == 0 && netConfigBuff == 0 ))
    {   // nothing to do
        return 0;
    }

    // minimum sanity check
    if(pNetStg->size != sizeof(*pNetStg))
    {   // not valid config save?
        return 0;
    }

    if(buffSize < sizeof(*pNetConf))
    {   // not even enough room to start
        return 0;
    }

    char    tempBuff[50 + 1];   // buffer large enough to hold any string in a TCPIP_NETWORK_CONFIG!
    void*   pDstBuff;
    size_t  dstSize;
    size_t  needLen, actualLen;
    
    // create at the very beginning of the buffer
    pNetConf = (TCPIP_NETWORK_CONFIG*)netConfigBuff;
    pDstBuff = pNetConf + 1;    // write area
    dstSize = buffSize - sizeof(*pNetConf);
    needLen = actualLen = 0;
    tempBuff[sizeof(tempBuff) - 1] = '\0';   // always end properly
    
    // get each field
    if(pNetStg->pMacObj != 0 && pNetStg->pMacObj->macName != 0)
    {
        strncpy(tempBuff, pNetStg->pMacObj->macName, sizeof(tempBuff) - 1);
        pNetConf->interface = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);
    }
    else
    {
        pNetConf->interface = 0;
    }

    strncpy(tempBuff, (char*)pNetStg->NetBIOSName, sizeof(tempBuff) - 1);
    pNetConf->hostName = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);

    TCPIP_Helper_MACAddressToString(&pNetStg->netMACAddr, tempBuff, sizeof(tempBuff) - 1);
    pNetConf->macAddr = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);

    TCPIP_Helper_IPAddressToString(&pNetStg->netIPAddr, tempBuff, sizeof(tempBuff) - 1);
    pNetConf->ipAddr = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);

    TCPIP_Helper_IPAddressToString(&pNetStg->netMask, tempBuff, sizeof(tempBuff) - 1);
    pNetConf->ipMask = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);

    TCPIP_Helper_IPAddressToString(&pNetStg->netGateway, tempBuff, sizeof(tempBuff) - 1);
    pNetConf->gateway = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);

    TCPIP_Helper_IPAddressToString(&pNetStg->PrimaryDNSServer, tempBuff, sizeof(tempBuff) - 1);
    pNetConf->priDNS = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);

    TCPIP_Helper_IPAddressToString(&pNetStg->SecondaryDNSServer, tempBuff, sizeof(tempBuff) - 1);
    pNetConf->secondDNS = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);

    strncpy(tempBuff, TCPIP_Helper_PowerModeToString(pNetStg->Flags.powerMode), sizeof(tempBuff) - 1);
    pNetConf->powerMode = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);

    // set the flags
    pNetConf->startFlags = 0; 
    if(pNetStg->Flags.bIsDHCPEnabled)
    {
        pNetConf->startFlags |= TCPIP_NETWORK_CONFIG_DHCP_CLIENT_ON; 
    }
    if(pNetStg->Flags.bIsDHCPSrvEnabled)
    {
        pNetConf->startFlags |= TCPIP_NETWORK_CONFIG_DHCP_SERVER_ON; 
    }
    if(pNetStg->Flags.bIsZcllEnabled)
    {
        pNetConf->startFlags |= TCPIP_NETWORK_CONFIG_ZCLL_ON; 
    }
    // set the MAC driver object
    pNetConf->pMacObject = pNetStg->pMacObj;
#if defined(TCPIP_STACK_USE_IPV6)
    if((pNetStg->startFlags & TCPIP_NETWORK_CONFIG_IPV6_ADDRESS) != 0)
    {
        pNetConf->startFlags |= TCPIP_NETWORK_CONFIG_IPV6_ADDRESS;
        pNetConf->ipv6PrefixLen = pNetStg->ipv6PrefixLen;

        TCPIP_Helper_IPv6AddressToString(&pNetStg->netIPv6Addr, tempBuff, sizeof(tempBuff) - 1);
        pNetConf->ipv6Addr = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);

        TCPIP_Helper_IPv6AddressToString(&pNetStg->netIPv6Gateway, tempBuff, sizeof(tempBuff) - 1);
        pNetConf->ipv6Gateway = _NetConfigStringToBuffer(&pDstBuff, tempBuff, &dstSize, &needLen, &actualLen);
    }

#endif  // defined(TCPIP_STACK_USE_IPV6)

    if(pNeededSize)
    {
        *pNeededSize = needLen + sizeof(*pNetConf);
    }

    if(actualLen == needLen)
    {   // succeeded
        return pNetConf;
    }

    return 0;

}

TCPIP_STACK_MODULE TCPIP_STACK_StringToMACId(const char* str)
{
    if(str)
    {
        int netIx;
        TCPIP_NET_IF* pNetIf;
        const TCPIP_MAC_OBJECT  *pObj;

        for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
        {
            if((pObj = pNetIf->pMacObj) != 0)
            {
                if(strcmp(str, pObj->macName) == 0)
                {
                    return pObj->macId;
                }
            }
        }
    }


    return (TCPIP_STACK_MODULE)TCPIP_MODULE_MAC_NONE;
}



const char* TCPIP_STACK_MACIdToString(TCPIP_STACK_MODULE moduleId)
{
    int netIx;
    TCPIP_NET_IF* pNetIf;
    const TCPIP_MAC_OBJECT  *pObj;

    for(netIx = 0, pNetIf = tcpipNetIf; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pNetIf++)
    {
        if((pObj = pNetIf->pMacObj) != 0)
        {
            if(pObj->macId == moduleId)
            {
                return pObj->macName;
            }
        }
    }
    return 0;
}

// detects if an IP address is a local network
bool TCPIP_STACK_IPAddressIsLocalNetwork(TCPIP_NET_IF* pNetIf, IPV4_ADDR destIpAdd)
{
    uint32_t currNetVal, destNetVal;
    uint32_t andMask, orMask;

    if(pNetIf == 0)
    {   // unknown
        return false;
    }

    switch(pNetIf->exFlags.localAndType)
    {
        case TCPIP_LOCAL_MASK_ZERO:
            andMask = 0;
            break;

        case TCPIP_LOCAL_MASK_ONE:
            andMask = 0xffffffff;
            break;

        case TCPIP_LOCAL_MASK_NET:
            andMask = pNetIf->netMask.Val;
            break;

        default:    // TCPIP_LOCAL_MASK_SET
            andMask = pNetIf->localAndMask.Val;
            break;
    }

    switch(pNetIf->exFlags.localOrType)
    {
        case TCPIP_LOCAL_MASK_ZERO:
            orMask = 0;
            break;

        case TCPIP_LOCAL_MASK_ONE:
            orMask = 0xffffffff;
            break;

        case TCPIP_LOCAL_MASK_NET:
            orMask = pNetIf->netMask.Val;
            break;

        default:    // TCPIP_LOCAL_MASK_SET
            orMask = pNetIf->localOrMask.Val;
            break;
    }

    currNetVal  = (pNetIf->netIPAddr.Val & andMask) | orMask;
    destNetVal = (destIpAdd.Val & andMask) | orMask;

    return destNetVal == currNetVal; 
}

/*********************************************************************
 * Function:        TCPIP_STACK_NetAddressBcast(TCPIP_NET_HANDLE netH)
 *
 * PreCondition:    TCPIP stack should have been initialized by
 *                    TCPIP_STACK_Initialize()
 *
 * Input:           interface handle to get address of
 *
 * Output:          The broadcast IP address of an interface.
 *
 *
 * Side Effects:    None
 *
 * Note:           None
 *                 
 ********************************************************************/
uint32_t TCPIP_STACK_NetAddressBcast(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        return (pNetIf->netIPAddr.Val | ~pNetIf->netMask.Val);
    }

    return 0;


}

bool TCPIP_STACK_NetIsUp(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    return pNetIf != 0;
}

bool TCPIP_STACK_NetIsLinked(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(netH);
    if(pNetIf)
    {
        return TCPIP_STACK_NetworkIsLinked(pNetIf);
    }
    return false;
}

bool TCPIP_STACK_NetIsReady(TCPIP_NET_HANDLE netH)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetLinked(netH);
    if(pNetIf)
    {
        return _TCPIPStackIsConfig(pNetIf) == 0;
    }
    return false;
}

TCPIP_STACK_DNS_SERVICE_TYPE TCPIP_STACK_DNSServiceSelect(TCPIP_NET_IF* pNetIf, TCPIP_NETWORK_CONFIG_FLAGS configFlags)
{
    // clear all the existing DNS address service bits
    pNetIf->Flags.v &= ~TCPIP_STACK_SERVICE_DNS_MASK;

    
#if defined(TCPIP_STACK_USE_DNS)
        if((configFlags & TCPIP_NETWORK_CONFIG_DNS_CLIENT_ON) != 0 )
        { 
            pNetIf->Flags.bIsDnsClientEnabled = 1;
            return TCPIP_STACK_SERVICE_DNSC;
        }
#endif  // defined(TCPIP_STACK_USE_DNS)
    
#if defined(TCPIP_STACK_USE_DNS_SERVER)
        if((configFlags & TCPIP_NETWORK_CONFIG_DNS_SERVER_ON) != 0 )
        { 
            pNetIf->Flags.bIsDnsServerEnabled = 1;
            return TCPIP_STACK_SERVICE_DNSS;
        }
#endif  // defined(TCPIP_STACK_USE_DNS_SERVER)
    // couldn't select an address service
    // use default/static
    return TCPIP_STACK_DNS_SERVICE_NONE;
}


TCPIP_STACK_ADDRESS_SERVICE_TYPE TCPIP_STACK_AddressServiceSelect(TCPIP_NET_IF* pNetIf, TCPIP_NETWORK_CONFIG_FLAGS configFlags)
{
    // clear all the existing address service bits
    pNetIf->Flags.v &= ~TCPIP_STACK_ADDRESS_SERVICE_MASK;

    // Set up the address service on this interface
    // Priority (high to low): DHCPc, ZCLL, DHCPS, static IP address
#if defined(TCPIP_STACK_USE_DHCP_CLIENT)
    if((configFlags & TCPIP_NETWORK_CONFIG_DHCP_CLIENT_ON) != 0 )
    { 
        pNetIf->Flags.bIsDHCPEnabled = 1;
        return TCPIP_STACK_ADDRESS_SERVICE_DHCPC;
    }
#endif  // defined(TCPIP_STACK_USE_DHCP_CLIENT)

#if defined(TCPIP_STACK_USE_ZEROCONF_LINK_LOCAL)
    if((configFlags & TCPIP_NETWORK_CONFIG_ZCLL_ON) != 0 )
    { 
        pNetIf->Flags.bIsZcllEnabled = 1;
        return TCPIP_STACK_ADDRESS_SERVICE_ZCLL;
    }
#endif  // defined(TCPIP_STACK_USE_ZEROCONF_LINK_LOCAL)

#if defined(TCPIP_STACK_USE_DHCP_SERVER)
    if((configFlags & TCPIP_NETWORK_CONFIG_DHCP_SERVER_ON) != 0 )
    { 
        pNetIf->Flags.bIsDHCPSrvEnabled = 1;
        return TCPIP_STACK_ADDRESS_SERVICE_DHCPS;
    }
#endif  // defined(TCPIP_STACK_USE_DHCP_SERVER)

    // couldn't select an address service
    // use default/static
    return TCPIP_STACK_ADDRESS_SERVICE_NONE;

}


bool TCPIP_STACK_AddressServiceCanStart(TCPIP_NET_IF* pNetIf, TCPIP_STACK_ADDRESS_SERVICE_TYPE adSvcType)
{
    if(pNetIf)
    {   // enable a different address service only if there's not another one running
        // client has to stop a previos service (DHCP, ZCLL, etc.) in order to start another one
        return (pNetIf->Flags.v & TCPIP_STACK_ADDRESS_SERVICE_MASK) == TCPIP_STACK_ADDRESS_SERVICE_NONE;
    }

    return false;
}

bool TCPIP_STACK_DNSServiceCanStart(TCPIP_NET_IF* pNetIf, TCPIP_STACK_DNS_SERVICE_TYPE adSvcType)
{
    if(pNetIf)
    {   // enable a different address service only if there's not another one running
        // client has to stop a previos service (DHCP, ZCLL, etc.) in order to start another one
        return (pNetIf->Flags.v & TCPIP_STACK_SERVICE_DNS_MASK) != TCPIP_STACK_SERVICE_DNSS;
    }

    return false;
}

TCPIP_STACK_ADDRESS_SERVICE_TYPE _TCPIPStackAddressServiceIsRunning(TCPIP_NET_IF* pNetIf)
{
    return (TCPIP_STACK_ADDRESS_SERVICE_TYPE)(pNetIf->Flags.v & TCPIP_STACK_ADDRESS_SERVICE_MASK);
}

void TCPIP_STACK_AddressServiceEvent(TCPIP_NET_IF* pNetIf, TCPIP_STACK_ADDRESS_SERVICE_TYPE adSvcType,
                                    TCPIP_STACK_ADDRESS_SERVICE_EVENT evType)
{
    typedef bool(*addSvcFnc)(TCPIP_NET_HANDLE hNet);
    addSvcFnc   addFnc;

    if(evType == TCPIP_STACK_ADDRESS_SERVICE_EVENT_RUN_RESTORE)
    {   // run time connection restore; it should be the DHCPc
        if(adSvcType == TCPIP_STACK_ADDRESS_SERVICE_DHCPC)
        {
            pNetIf->Flags.bIsDHCPEnabled = 1;
            return;
        }
    }
    else if(evType == TCPIP_STACK_ADDRESS_SERVICE_EVENT_CONN_LOST)
    {   // connection loss is considered a temporary event;
        // no need to disable a service
        // since we don't have network connectivity anyway
        return;
    }
    else if(adSvcType == TCPIP_STACK_ADDRESS_SERVICE_DHCPS)
    {   // if DHCP server was stopped/failed
        // we won't start another address service
        // the user will have to take a decision
        return;
    }

    // the DHCPc/ZCLL address service failed/stopped:
    // TCPIP_STACK_ADDRESS_SERVICE_EVENT_RUN_FAIL, TCPIP_STACK_ADDRESS_SERVICE_EVENT_USER_STOP
    //
    // make sure any running service is cleared
    pNetIf->Flags.v &= ~TCPIP_STACK_ADDRESS_SERVICE_MASK;
    _TCPIPStackSetConfig(pNetIf, true);
    addFnc = 0;
    if(adSvcType == TCPIP_STACK_ADDRESS_SERVICE_DHCPC)
    {   // the DHCP client has been stopped or failed
        // if possible we'll select ZCLL
#if defined(TCPIP_STACK_USE_ZEROCONF_LINK_LOCAL)
        if((pNetIf->startFlags & TCPIP_NETWORK_CONFIG_ZCLL_ON) != 0)
        {   // OK, we can use ZCLL
            addFnc = TCPIP_ZCLL_Enable;
        }
#endif
    }
    // else if (adSvcType == TCPIP_STACK_ADDRESS_SERVICE_ZCLL)
    // we'll select the default IP address


    if(addFnc)
    {
        if((*addFnc)(pNetIf) == true)
        {   // success
            return;
        }
    }

    // no other address service or it couldn't be started
    // select the default/static addresses
    TCPIP_STACK_AddressServiceDefaultSet(pNetIf);
}

/*********************************************************************
 * Function:        bool _InitNetConfig(const TCPIP_NETWORK_CONFIG* pUsrConfig, int nNets)
 *
 * PreCondition:    MPFSInit() is already called.
 *
 * Input:           pUsrConfig  - pointer to user configurations
 *                  nNets       - number of networks configurations provided
 *
 * Output:          Write/Read non-volatile config variables.
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
static bool _InitNetConfig(const TCPIP_NETWORK_CONFIG* pUsrConfig, int nNets)
{
    int     ix;
    TCPIP_NET_IF* pNetConfig;


    for(ix =0, pNetConfig = tcpipNetIf; ix < nNets; ix++, pNetConfig++, pUsrConfig++)
    {
        if(!_LoadNetworkConfig(pUsrConfig, pNetConfig, false))
        {
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "Default Flash Network configuration load failed %d\r\n", ix);
            return false;
        }
    }



    return true;
}


/*********************************************************************
 * Function:        bool _LoadNetworkConfig(const TCPIP_NETWORK_CONFIG* pUsrConfig, TCPIP_NET_IF* pNetIf, bool restartIf)
 *
 * PreCondition:    None
 *
 * Input:           pUsrConfig  - pointer to configurations to use
 *                  pNetIf      - network interface to default configure
 *                  restartIf   - if true this is a restart of the interface
 *
 * Output:          true if the network configuration sucessfully loaded,
 *                  false otherwise
 *
 * Side Effects:    None
 *
 * Overview:        Loads the network configuration values
 *
 * Note:            None
 ********************************************************************/
static bool _LoadNetworkConfig(const TCPIP_NETWORK_CONFIG* pUsrConfig, TCPIP_NET_IF* pNetIf, bool restartIf)
{
    TCPIP_STACK_ADDRESS_SERVICE_TYPE startAddService;
	TCPIP_STACK_DNS_SERVICE_TYPE	 addDynamicNameService;
    char    oldIfName[sizeof(pNetIf->ifName) + 1];
    const void*  pMacConfig = 0;             // MAC configuration save

    if(restartIf)
    {   // save old data that's still useful
        strncpy(oldIfName, pNetIf->ifName, sizeof(pNetIf->ifName)); 
        pMacConfig = pNetIf->pMacConfig;
    }

    memset(pNetIf, 0, sizeof(*pNetIf));

    if(pUsrConfig->macAddr != 0)
    {
        TCPIP_Helper_StringToMACAddress(pUsrConfig->macAddr, pNetIf->netMACAddr.v);
    }
    else
    {
        memset(pNetIf->netMACAddr.v, 0, sizeof(pNetIf->netMACAddr.v));
    }

    // store the default addresses
    TCPIP_Helper_StringToIPAddress(pUsrConfig->ipAddr, &pNetIf->DefaultIPAddr);
    TCPIP_Helper_StringToIPAddress(pUsrConfig->ipMask, &pNetIf->DefaultMask);

    TCPIP_Helper_StringToIPAddress(pUsrConfig->gateway, &pNetIf->DefaultGateway);
    TCPIP_Helper_StringToIPAddress(pUsrConfig->priDNS, &pNetIf->DefaultDNSServer);
    TCPIP_Helper_StringToIPAddress(pUsrConfig->secondDNS, &pNetIf->DefaultDNSServer2);

    if((pNetIf->pMacObj = pUsrConfig->pMacObject) == 0)
    {
        return false;   // no such MAC interface
    }
    pNetIf->macId = pNetIf->pMacObj->macId;

    // Load the NetBIOS Host Name
    memcpy(pNetIf->NetBIOSName, pUsrConfig->hostName, sizeof(tcpipNetIf[0].NetBIOSName));
    TCPIP_Helper_FormatNetBIOSName(pNetIf->NetBIOSName);

    if(restartIf)
    {   
        // restore the if name
        strncpy(pNetIf->ifName, oldIfName, sizeof(pNetIf->ifName)); 
        // restore MAC config data
        pNetIf->pMacConfig = pMacConfig;
    }

    // store start up flags
    pNetIf->startFlags = pUsrConfig->startFlags;
#if defined(TCPIP_STACK_USE_IPV6)
    if((pNetIf->startFlags & TCPIP_NETWORK_CONFIG_IPV6_ADDRESS) != 0)
    {
        pNetIf->ipv6PrefixLen = (uint16_t)pUsrConfig->ipv6PrefixLen;
        if(pUsrConfig->ipv6Addr == 0 || !TCPIP_Helper_StringToIPv6Address (pUsrConfig->ipv6Addr, &pNetIf->netIPv6Addr))
        {   // ignore the static IPv6 address if incorrect
            pNetIf->startFlags &= ~TCPIP_NETWORK_CONFIG_IPV6_ADDRESS;
        }
        else if(pUsrConfig->ipv6Gateway == 0 || !TCPIP_Helper_StringToIPv6Address (pUsrConfig->ipv6Gateway, &pNetIf->netIPv6Gateway))
        {   // ignore the IPv6 gateway if incorrect
            pNetIf->startFlags &= ~TCPIP_NETWORK_CONFIG_IPV6_ADDRESS;
        }
            
    }
#endif  // defined(TCPIP_STACK_USE_IPV6)

    // Set up the address service on this interface
    _TCPIPStackSetConfig(pNetIf, true);
    startAddService = TCPIP_STACK_AddressServiceSelect(pNetIf, pUsrConfig->startFlags);

    if(startAddService == TCPIP_STACK_ADDRESS_SERVICE_NONE)
    {   // couldn't start an address service; use the static values supplied
        TCPIP_STACK_AddressServiceDefaultSet(pNetIf);
    }
    
    if( (startAddService != TCPIP_STACK_ADDRESS_SERVICE_DHCPC))
    {   // will use the default DNS server
        pNetIf->PrimaryDNSServer.Val = pNetIf->DefaultDNSServer.Val;
        pNetIf->SecondaryDNSServer.Val = pNetIf->DefaultDNSServer2.Val;
    }
    else
    {   // The DHCPc will update these
        pNetIf->Flags.bIsDNSServerAuto = 1;
    }

    addDynamicNameService =  TCPIP_STACK_DNSServiceSelect(pNetIf, pUsrConfig->startFlags);

    if(addDynamicNameService == TCPIP_STACK_DNS_SERVICE_NONE)
    {
         pNetIf->Flags.bIsDnsClientEnabled = 1;
    }

    return true;
}

void TCPIP_STACK_AddressServiceDefaultSet(TCPIP_NET_IF* pNetIf)
{
    _TCPIPStackSetConfigAddress(pNetIf, &pNetIf->DefaultIPAddr, &pNetIf->DefaultMask, false);
    pNetIf->netGateway.Val = pNetIf->DefaultGateway.Val;
}


#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)

// Stack external event notification support

TCPIP_EVENT TCPIP_STACK_EventsPendingGet(const void* h)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(h);
    if(pNetIf)
    {
        return TCPIP_STACK_Mac2TcpipEvent((*pNetIf->pMacObj->TCPIP_MAC_EventPendingGet)(pNetIf->hIfMac));
    }
    return TCPIP_EV_NONE;
}

TCPIP_EVENT_HANDLE    TCPIP_STACK_HandlerRegister(TCPIP_NET_HANDLE hNet, TCPIP_EVENT evMask, TCPIP_STACK_EVENT_HANDLER handler, const void* hParam)
{
    TCPIP_EVENT_LIST_NODE* newNode;
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(hNet);

    if(pNetIf && handler)
    {
        newNode = (TCPIP_EVENT_LIST_NODE*)TCPIP_Notification_Add(&pNetIf->registeredClients, tcpip_stack_ctrl_data.memH, sizeof(*newNode));
        if(newNode)
        {
            newNode->handler = handler;
            newNode->hParam = hParam;
            newNode->evMask = evMask;
            newNode->pNetIf = pNetIf;
        }
        return newNode;
    }

    return 0;
}

bool TCPIP_STACK_HandlerDeregister(TCPIP_EVENT_HANDLE hStack)
{
    TCPIP_EVENT_LIST_NODE* pNode = (TCPIP_EVENT_LIST_NODE*)hStack;
    if(pNode)
    {
        if(TCPIP_Notification_Remove((SGL_LIST_NODE*)pNode, &pNode->pNetIf->registeredClients,tcpip_stack_ctrl_data.memH))
        {
            return true;
        }
    }

    return false;

}


#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)

static const TCPIP_STACK_MODULE_CONFIG* _TCPIP_STACK_FindModuleData(TCPIP_STACK_MODULE moduleId, const TCPIP_STACK_MODULE_CONFIG* pModConfig, int nModules)
{
    while(nModules--)
    {
        if(pModConfig->moduleId == moduleId)
        {
            return pModConfig;
        }
        pModConfig++;
    }

    return 0;
}


int  TCPIP_STACK_NetIxGet(TCPIP_NET_IF* pNetIf)
{
    if(pNetIf)
    {
        return pNetIf->netIfIx;
    }
    return -1;
}


uint32_t  TCPIP_STACK_NetAddressGet(TCPIP_NET_IF* pNetIf)
{
    if(TCPIP_STACK_NetworkIsUp(pNetIf))
    {
        return _TCPIPStackNetAddress(pNetIf);
    }
    return 0;
}


const IPV6_ADDR* TCPIP_STACK_NetStaticIPv6AddressGet(TCPIP_NET_IF* pNetIf, int* pPrefixLen)
{
#if defined(TCPIP_STACK_USE_IPV6)
    if((pNetIf->startFlags & TCPIP_NETWORK_CONFIG_IPV6_ADDRESS) != 0)
    {
        if(pPrefixLen)
        {
            *pPrefixLen = (int)pNetIf->ipv6PrefixLen;
        }
        return &pNetIf->netIPv6Addr;
    }
#endif  // defined(TCPIP_STACK_USE_IPV6)

    return 0;
}

const IPV6_ADDR* TCPIP_STACK_NetDefaultIPv6GatewayGet(TCPIP_NET_IF* pNetIf)
{
#if defined(TCPIP_STACK_USE_IPV6)
    if((pNetIf->startFlags & TCPIP_NETWORK_CONFIG_IPV6_ADDRESS) != 0)
    {
        return &pNetIf->netIPv6Gateway;
    }
#endif  // defined(TCPIP_STACK_USE_IPV6)

    return 0;
}


void  TCPIP_STACK_NetworkAddressSet(TCPIP_NET_IF* pNetIf, IPV4_ADDR* ipAddress, IPV4_ADDR* mask, bool setDefault)
{
    if(pNetIf)
    {
        if(ipAddress)
        {
            pNetIf->netIPAddr.Val = ipAddress->Val;
            if(setDefault)
            {
                pNetIf->DefaultIPAddr.Val = ipAddress->Val;
            }
        }

        if(mask)
        {
            pNetIf->netMask.Val = mask->Val;
            if(setDefault)
            {
                pNetIf->DefaultMask.Val = mask->Val;
            }
        }
    }
}

void  _TCPIPStackSetConfigAddress(TCPIP_NET_IF* pNetIf, IPV4_ADDR* ipAddress, IPV4_ADDR* mask, bool config)
{
    if(pNetIf)
    {
        if(ipAddress)
        {
            pNetIf->netIPAddr.Val = ipAddress->Val;
        }

        if(mask)
        {
            pNetIf->netMask.Val = mask->Val;
        }
        _TCPIPStackSetConfig(pNetIf, config);
    }
}

uint32_t  TCPIP_STACK_NetMaskGet(TCPIP_NET_IF* pNetIf)
{
    if(TCPIP_STACK_NetworkIsUp(pNetIf))
    {
        return pNetIf->netMask.Val;
    }
    return 0;
}

void  TCPIP_STACK_GatewayAddressSet(TCPIP_NET_IF* pNetIf, IPV4_ADDR* ipAddress)
{
    if(pNetIf)
    {
        pNetIf->netGateway.Val = ipAddress->Val;
    }
}
void  TCPIP_STACK_PrimaryDNSAddressSet(TCPIP_NET_IF* pNetIf, IPV4_ADDR* ipAddress)
{
    if(pNetIf)
    {
        pNetIf->PrimaryDNSServer.Val = ipAddress->Val;
    }
}

void  TCPIP_STACK_SecondaryDNSAddressSet(TCPIP_NET_IF* pNetIf, IPV4_ADDR* ipAddress)
{
    if(pNetIf)
    {
        pNetIf->SecondaryDNSServer.Val = ipAddress->Val;
    }
}


bool  TCPIP_STACK_AddressIsOfNetUp( TCPIP_NET_IF* pNetIf, const IPV4_ADDR* pIpAdd)
{
    if(TCPIP_STACK_NetworkIsUp(pNetIf))
    {
        return TCPIP_STACK_AddressIsOfNet(pNetIf, pIpAdd);
    }
    return false;
}

// detects net-directed bcast
bool  TCPIP_STACK_NetIsBcastAddress(TCPIP_NET_IF* pNetIf, IPV4_ADDR* pIpAdd)
{
    if(TCPIP_STACK_NetworkIsUp(pNetIf))
    {
       return (pIpAdd->Val == (pNetIf->netIPAddr.Val | ~pNetIf->netMask.Val));
    }
    return false;
}

// detects limited or net-directed bcast
bool  TCPIP_STACK_IsBcastAddress(TCPIP_NET_IF* pNetIf, IPV4_ADDR* pIpAdd)
{
    if(TCPIP_STACK_NetworkIsUp(pNetIf))
    {
       return (TCPIP_Helper_IsBcastAddress(pIpAdd) ||  TCPIP_STACK_NetIsBcastAddress(pNetIf, pIpAdd));
    }
    return false;
}

bool TCPIP_STACK_NetworkIsLinked(TCPIP_NET_IF* pNetIf)
{
    if(TCPIP_STACK_NetworkIsUp(pNetIf))
    {
        return pNetIf->exFlags.linkPrev;
    }

    return false;
}

// checks for valid up and linked interface
TCPIP_NET_IF* _TCPIPStackHandleToNetLinked(TCPIP_NET_HANDLE hNet)
{
    TCPIP_NET_IF* pNetIf = _TCPIPStackHandleToNetUp(hNet);
    if(pNetIf != 0 && pNetIf->exFlags.linkPrev != 0)
    {
        return pNetIf;
    }
    
    return 0;
}

TCPIP_NET_IF* _TCPIPStackAnyNetLinked(bool useDefault)
{
    int netIx;
    TCPIP_NET_IF* pNetIf = 0;

    if(useDefault)
    {
        pNetIf = _TCPIPStackHandleToNetLinked(tcpipDefIf.defaultNet);
    }

    if(pNetIf == 0)
    {
        TCPIP_NET_IF* pIf;

        for(netIx = 0, pIf = tcpipNetIf ; netIx < tcpip_stack_ctrl_data.nIfs; netIx++, pIf++)
        {
            if(pIf->Flags.bInterfaceEnabled && pIf->exFlags.linkPrev != 0)
            {   // found linked interface
                pNetIf = pIf;
                break;
            }
        }
    }

    return pNetIf;
}






TCPIP_STACK_MODULE  TCPIP_STACK_NetMACId(TCPIP_NET_IF* pNetIf)
{
    return pNetIf ? pNetIf->macId:(TCPIP_STACK_MODULE)TCPIP_MODULE_MAC_NONE;
}


 TCPIP_MAC_HANDLE  TCPIP_STACK_NetToMAC(TCPIP_NET_IF* pNetIf)
{
    return pNetIf ? pNetIf->hIfMac : 0;
}



const uint8_t*  TCPIP_STACK_NetMACAddressGet(TCPIP_NET_IF* pNetIf)
{
    if(TCPIP_STACK_NetworkIsUp(pNetIf))
    {
        return pNetIf->netMACAddr.v;
    }

    return 0;
}

static TCPIP_MAC_ACTION TCPIP_STACK_StackToMacAction(TCPIP_STACK_ACTION action)
{   // TCPIP_MAC_ACTION and TCPIP_STACK_ACTION should be kept in sync!
    return (TCPIP_MAC_ACTION)action;
}

static void TCPIP_STACK_StacktoMacCtrl(TCPIP_MAC_MODULE_CTRL* pMacCtrl, TCPIP_STACK_MODULE_CTRL* stackCtrlData)
{
    TCPIP_NET_IF* pNetIf = stackCtrlData->pNetIf;


    pMacCtrl->nIfs = stackCtrlData->nIfs;

    pMacCtrl->mallocF = (TCPIP_MAC_HEAP_MallocF)_TCPIP_HEAP_MALLOC_FNC;
    pMacCtrl->callocF = (TCPIP_MAC_HEAP_CallocF)_TCPIP_HEAP_CALLOC_FNC;
    pMacCtrl->freeF = (TCPIP_MAC_HEAP_FreeF)_TCPIP_HEAP_FREE_FNC;
    pMacCtrl->memH = stackCtrlData->memH;


    pMacCtrl->pktAllocF = (TCPIP_MAC_PKT_AllocF)_TCPIP_PKT_ALLOC_FNC;
    pMacCtrl->pktFreeF = (TCPIP_MAC_PKT_FreeF)_TCPIP_PKT_FREE_FNC;
    pMacCtrl->pktAckF = (TCPIP_MAC_PKT_AckF)_TCPIP_PKT_ACK_FNC;

    pMacCtrl->synchF = _TCPIP_StackSyncFunction;

#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
    // Stack can use one handler for all network interfaces, like in this case
    // Each time a notification is received, all interfaces are checked
    // Or, more efficient, use a handler per interface
    pMacCtrl->eventF = _TCPIP_MacEventCB;
    pMacCtrl->eventParam = pNetIf;
#else
    pMacCtrl->eventF = 0;
    pMacCtrl->eventParam = 0;
#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)

    pMacCtrl->moduleId = pNetIf->macId;
    pMacCtrl->netIx = stackCtrlData->netIx;
    pMacCtrl->macAction = TCPIP_STACK_StackToMacAction(stackCtrlData->stackAction);
    pMacCtrl->powerMode = stackCtrlData->powerMode;

    memcpy(pMacCtrl->ifPhyAddress.v, pNetIf->netMACAddr.v, sizeof(pMacCtrl->ifPhyAddress));
}

// returns true if the stack needs processing time because of events
static bool TCPIP_STACK_CheckEventsPending(void)
{
#if defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
    return (newTcpipTickAvlbl != 0 || totTcpipEventsCnt != 0);
#else
    // fake pending events
    totTcpipEventsCnt++;
    return true;
#endif  // defined(TCPIP_STACK_USE_EVENT_NOTIFICATION)
}


tcpipSignalHandle _TCPIPStackSignalHandlerRegister(TCPIP_STACK_MODULE modId, tcpipModuleSignalHandler signalHandler, int16_t asyncTmoMs)
{

    if(signalHandler != 0 )
    {
        if(TCPIP_MODULE_LAYER1 <= modId && modId < sizeof(TCPIP_STACK_MODULE_SIGNAL_TBL)/sizeof(*TCPIP_STACK_MODULE_SIGNAL_TBL))
        {
            TCPIP_MODULE_SIGNAL_ENTRY* pSignalEntry = TCPIP_STACK_MODULE_SIGNAL_TBL + modId;
            if(pSignalEntry->signalHandler == 0 || pSignalEntry->signalHandler == signalHandler)
            {   // found module slot
                pSignalEntry->signalHandler = signalHandler;
                if ((asyncTmoMs != 0) && (asyncTmoMs < stackTaskRate))
                {
                    asyncTmoMs = stackTaskRate;
                }
                pSignalEntry->asyncTmo = pSignalEntry->currTmo = asyncTmoMs;
                return pSignalEntry;
            }
        }
    }

    return 0;
}


bool _TCPIPStackSignalHandlerSetParams(TCPIP_STACK_MODULE modId, tcpipSignalHandle handle, int16_t asyncTmoMs)
{
    TCPIP_MODULE_SIGNAL_ENTRY* pSignalEntry = (TCPIP_MODULE_SIGNAL_ENTRY*)handle;
    if((pSignalEntry = (TCPIP_MODULE_SIGNAL_ENTRY*)handle) != 0 && pSignalEntry->signalHandler != 0)
    {   // minimim sanity check
		if ((asyncTmoMs != 0) && (asyncTmoMs < stackTaskRate))
		{
            asyncTmoMs = stackTaskRate;
		}
        pSignalEntry->asyncTmo = pSignalEntry->currTmo = asyncTmoMs;
        return true;
    }

    return false;
}

// de-registers a previous registered timeout handler
void _TCPIPStackSignalHandlerDeregister(tcpipSignalHandle handle)
{
    TCPIP_MODULE_SIGNAL_ENTRY* pSignalEntry = (TCPIP_MODULE_SIGNAL_ENTRY*)handle;
    if((pSignalEntry = (TCPIP_MODULE_SIGNAL_ENTRY*)handle) != 0)
    {
        memset(pSignalEntry, 0x0, sizeof(*pSignalEntry));
    }
}

// used by stack modules
TCPIP_MODULE_SIGNAL  _TCPIPStackModuleSignalGet(TCPIP_STACK_MODULE modId, TCPIP_MODULE_SIGNAL clrMask)
{

    TCPIP_MODULE_SIGNAL_ENTRY* pSignalEntry = TCPIP_STACK_MODULE_SIGNAL_TBL + modId;
    if(clrMask)
    {
        OSAL_CRITSECT_DATA_TYPE critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
        TCPIP_MODULE_SIGNAL modSignal = pSignalEntry->signalVal;
        pSignalEntry->signalVal &= (uint16_t)(~clrMask);
        if((clrMask & TCPIP_MODULE_SIGNAL_ASYNC) != 0)
        {
            stackAsyncSignalCount -= 1; 
        }
        OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, critSect);
        return modSignal;
    }

    return pSignalEntry->signalVal;
}

// used by manager!
// TMO and RX_PENDING signals only!
static TCPIP_MODULE_SIGNAL  _TCPIPStackManagerSignalClear(TCPIP_MODULE_SIGNAL clrMask)
{
    TCPIP_MODULE_SIGNAL tmoMask;
    TCPIP_MODULE_SIGNAL rxMask;
    TCPIP_MODULE_SIGNAL mgrSignal;
    TCPIP_MODULE_SIGNAL_ENTRY* pMgrEntry = TCPIP_STACK_MODULE_SIGNAL_TBL + TCPIP_MODULE_MANAGER;
    TCPIP_MODULE_SIGNAL_ENTRY* pTmoEntry = TCPIP_STACK_MODULE_SIGNAL_TBL + TCPIP_MODULE_NONE;


    if(clrMask == 0)
    {   // read only needed
        return pMgrEntry->signalVal | pTmoEntry->signalVal;
    }


    tmoMask = clrMask & TCPIP_MODULE_SIGNAL_TMO;
    rxMask = clrMask & TCPIP_MODULE_SIGNAL_RX_PENDING;


    // protect against ISRs (MAC + TMR) and other threads too!
    OSAL_CRITSECT_DATA_TYPE critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
    mgrSignal = pMgrEntry->signalVal;
    mgrSignal |= pTmoEntry->signalVal;
    if(tmoMask)
    {
        pTmoEntry->signalVal &= (uint16_t)(~TCPIP_MODULE_SIGNAL_TMO);
    }
    if(rxMask)
    {
        pMgrEntry->signalVal &= (uint16_t)(~TCPIP_MODULE_SIGNAL_RX_PENDING);
    }
    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, critSect);

    return mgrSignal;
}

// coming from user threads if the module tasks run in user space
static  void* _TCPIPSignalEntrySet(TCPIP_MODULE_SIGNAL_ENTRY* pSigEntry, TCPIP_MODULE_SIGNAL signal)
{
    OSAL_CRITSECT_DATA_TYPE critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
    pSigEntry->signalVal |= (uint16_t)signal;
    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, critSect);
    return pSigEntry;
}

// a stack module requests setting a signal
// allows setting a message to the manager itself, if needed
// coming from user threads if the module tasks run in user space
//
// a stack module requests setting a signal
// This function is mainly intended for requiring attention
// and the signal to be set is TCPIP_MODULE_SIGNAL_ASYNC!
// There is no need for a module to set other signals if there's really
// no pending RX packets or timeouts.
//
// NOTE:
//  a TCPIP_MODULE_SIGNAL_ASYNC is monitored by the stack manager so that as long as it is active
//  the module gets special attention.
//  This is done to allow a module to set the TCPIP_MODULE_SIGNAL_ASYNC signal just once
//  instead of continuously request it.
//  When done with the critical section the module should clear the TCPIP_MODULE_SIGNAL_ASYNC signal.
//  However, the stack manager keeps a global count of how many times the modules required TCPIP_MODULE_SIGNAL_ASYNC
//  That means that a module should always call
//  _TCPIPStackModuleSignalRequest/_TCPIPStackModuleSignalGet for TCPIP_MODULE_SIGNAL_ASYNC in pairs!
//
// NOTE:
//  A signal request sends a signal to the module task itself
//  A signal request will also send a signal to the stack task itself, when the TCP/IP modules are executed internally
//  - unless noMgrAlert is specified
//
bool _TCPIPStackModuleSignalRequest(TCPIP_STACK_MODULE modId, TCPIP_MODULE_SIGNAL signal, bool noMgrAlert)
{
    if(modId <= TCPIP_MODULE_MANAGER)
    {   // cannot request signals to manager itself
        return false;
    }

    _TCPIPModuleSignalSetNotify(modId, signal);

    if((signal & TCPIP_MODULE_SIGNAL_ASYNC) != 0)
    {
        _TCPIPAsyncSignalInc();
    }

#if !defined(TCPIP_STACK_APP_EXECUTE_MODULE_TASKS)
    if(noMgrAlert == 0)
    {
        _TCPIPSignalEntryNotify(_TCPIPModuleToSignalEntry(TCPIP_MODULE_MANAGER), signal);
    }
#endif  //  !defined(TCPIP_STACK_APP_EXECUTE_MODULE_TASKS)

    return true;
}

static void _TCPIPSignalEntryNotify(TCPIP_MODULE_SIGNAL_ENTRY* pSigEntry, TCPIP_MODULE_SIGNAL signal)
{
    TCPIP_MODULE_SIGNAL_FUNC userF;

    if((userF = pSigEntry->userSignalF) != 0)
    {
        (*userF)(pSigEntry, (pSigEntry - TCPIP_STACK_MODULE_SIGNAL_TBL) / sizeof(*pSigEntry), signal);
    }
}

static void _TCPIPSignalEntrySetNotify(TCPIP_MODULE_SIGNAL_ENTRY* pSigEntry, TCPIP_MODULE_SIGNAL signals)
{
    _TCPIPSignalEntrySet(pSigEntry, signals);
    _TCPIPSignalEntryNotify(pSigEntry, signals);
}

// signal the stack manager maintained timeout
static void _TCPIPStackSignalTmo(void)
{
    int     ix;
    TCPIP_MODULE_SIGNAL_ENTRY*  pSigEntry;

    pSigEntry = TCPIP_STACK_MODULE_SIGNAL_TBL + TCPIP_MODULE_LAYER1;
    for(ix = TCPIP_MODULE_LAYER1; ix < sizeof(TCPIP_STACK_MODULE_SIGNAL_TBL)/sizeof(*TCPIP_STACK_MODULE_SIGNAL_TBL); ix++, pSigEntry++)
    {
        if(pSigEntry->signalHandler == 0 || pSigEntry->asyncTmo == 0)
        {   // unused slot
            continue;
        }

        if((pSigEntry->currTmo -= stackTaskRate) <= 0)
        {   // timeout: send a signal to this module
            pSigEntry->currTmo += pSigEntry->asyncTmo;
            _TCPIPSignalEntrySetNotify(pSigEntry, TCPIP_MODULE_SIGNAL_TMO); 
        }
    }
}

// insert a packet into a module RX queue
// signal should be false when modId == TCPIP_MODULE_MANAGER !
void _TCPIPStackModuleRxInsert(TCPIP_STACK_MODULE modId, TCPIP_MAC_PACKET* pRxPkt, bool signal)
{
    SINGLE_LIST* pQueue = TCPIP_MODULES_QUEUE_TBL + modId;
    OSAL_CRITSECT_DATA_TYPE critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
    TCPIP_Helper_SingleListTailAdd(pQueue, (SGL_LIST_NODE*)pRxPkt);
    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, critSect);

    if(signal)
    {
        _TCPIPModuleSignalSetNotify(modId, TCPIP_MODULE_SIGNAL_RX_PENDING);
    }
}

//
// extracts a packet from a module RX queue
// returns 0 if queue is empty
TCPIP_MAC_PACKET* _TCPIPStackModuleRxExtract(TCPIP_STACK_MODULE modId)
{
    SINGLE_LIST* pQueue = TCPIP_MODULES_QUEUE_TBL + modId;
    OSAL_CRITSECT_DATA_TYPE critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
    TCPIP_MAC_PACKET* pRxPkt = (TCPIP_MAC_PACKET*)TCPIP_Helper_SingleListHeadRemove(pQueue);
    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, critSect);
    return pRxPkt;
}

// purges the packets from a module RX queue
// if pNetIf == 0, purges everything
void _TCPIPStackModuleRxPurge(TCPIP_STACK_MODULE modId, TCPIP_NET_IF* pNetIf)
{
    TCPIP_MAC_PACKET* pRxPkt;
    SINGLE_LIST       remList = { 0 };  // list of packets to remove/ack
    SINGLE_LIST       keepList = { 0 };  // list of packets to keep

    SINGLE_LIST*      pRxQueue = (TCPIP_MODULES_QUEUE_TBL + modId); // list to extract from
    
    // kill the list
    while(true)
    {
        OSAL_CRITSECT_DATA_TYPE critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
        pRxPkt = (TCPIP_MAC_PACKET*)TCPIP_Helper_SingleListHeadRemove(pRxQueue);
        OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, critSect);
        if(pRxPkt == 0)
        {
            break;
        }

        if(pRxPkt->pktIf == pNetIf)
        {   // need to remove
            TCPIP_Helper_SingleListTailAdd(&remList, (SGL_LIST_NODE*)pRxPkt);
        }
        else
        {   // need to keep
            TCPIP_Helper_SingleListTailAdd(&keepList, (SGL_LIST_NODE*)pRxPkt);
        }
    }

    // acknowledge the removed packets
    while((pRxPkt = (TCPIP_MAC_PACKET*)TCPIP_Helper_SingleListHeadRemove(&remList)) != 0)
    {
        TCPIP_PKT_PacketAcknowledge(pRxPkt, TCPIP_MAC_PKT_ACK_SOURCE_ERR);
    }

    if(!TCPIP_Helper_SingleListIsEmpty(&keepList))
    {   // replace the remaining packets
        OSAL_CRITSECT_DATA_TYPE critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
        TCPIP_Helper_SingleListAppend(pRxQueue, &keepList);
        OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, critSect);
    }

}


// inserts a RX packet into the manager RX queue
// this has to be a fully formatted TCPIP_MAC_PACKET
void _TCPIPStackInsertRxPacket(TCPIP_NET_IF* pNetIf, TCPIP_MAC_PACKET* pRxPkt, bool signal)
{
    pRxPkt->pktFlags |= TCPIP_MAC_PKT_FLAG_QUEUED;
    // update the frame length
    pRxPkt->pDSeg->segLen -= sizeof(TCPIP_MAC_ETHERNET_HEADER);
    _TCPIPInsertMacRxPacket(pNetIf, pRxPkt);

    if(signal)
    {
        _TCPIP_NetIfEvent(pNetIf, TCPIP_MAC_EV_RX_DONE, true);
    }
}

static bool _TCPIP_StackSyncFunction(void* synchHandle, TCPIP_MAC_SYNCH_REQUEST req)
{
    switch(req)
    {
        case TCPIP_MAC_SYNCH_REQUEST_OBJ_CREATE: 
            return (OSAL_SEM_Create((OSAL_SEM_HANDLE_TYPE*)synchHandle, OSAL_SEM_TYPE_BINARY, 1, 1) == OSAL_RESULT_TRUE) ? true : false;

        case TCPIP_MAC_SYNCH_REQUEST_OBJ_DELETE: 
            return (OSAL_SEM_Delete((OSAL_SEM_HANDLE_TYPE*)synchHandle) == OSAL_RESULT_TRUE) ? true : false;

        case TCPIP_MAC_SYNCH_REQUEST_OBJ_LOCK: 
            return (OSAL_SEM_Pend((OSAL_SEM_HANDLE_TYPE*)synchHandle, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE) ? true: false;

        case TCPIP_MAC_SYNCH_REQUEST_OBJ_UNLOCK: 
            return (OSAL_SEM_Post((OSAL_SEM_HANDLE_TYPE*)synchHandle) == OSAL_RESULT_TRUE) ? true: false;
            
        case TCPIP_MAC_SYNCH_REQUEST_CRIT_ENTER:
            *(OSAL_CRITSECT_DATA_TYPE*)synchHandle =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
            return true; 

        case TCPIP_MAC_SYNCH_REQUEST_CRIT_LEAVE:
            OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, *(OSAL_CRITSECT_DATA_TYPE*)synchHandle);
            return true; 

        default:
            return false;
    }

}

unsigned int TCPIP_STACK_VersionGet ( const SYS_MODULE_INDEX index )
{
    return( ( TCPIP_STACK_VERSION_MAJOR * 10000 ) +
            ( TCPIP_STACK_VERSION_MINOR * 100 ) +
            ( TCPIP_STACK_VERSION_PATCH ) );

}


char * TCPIP_STACK_VersionStrGet ( const SYS_MODULE_INDEX index )
{
    return TCPIP_STACK_VERSION_STR;
}



// this happens in user space
TCPIP_MODULE_SIGNAL_HANDLE TCPIP_MODULE_SignalFunctionRegister(TCPIP_STACK_MODULE modId, TCPIP_MODULE_SIGNAL_FUNC signalF)
{
    TCPIP_MODULE_SIGNAL_ENTRY* pEntry = 0;

    if(signalF != 0 )
    {
        if(0 < modId && modId < sizeof(TCPIP_STACK_MODULE_SIGNAL_TBL)/sizeof(*TCPIP_STACK_MODULE_SIGNAL_TBL))
        {
            TCPIP_MODULE_SIGNAL_ENTRY* pSignalEntry = TCPIP_STACK_MODULE_SIGNAL_TBL + modId;
            OSAL_CRITSECT_DATA_TYPE critSect =  OSAL_CRIT_Enter(OSAL_CRIT_TYPE_LOW);
            if(pSignalEntry->userSignalF == 0)
            {   // found module slot
                pSignalEntry->userSignalF = signalF;
                pEntry = pSignalEntry;
            }
            OSAL_CRIT_Leave(OSAL_CRIT_TYPE_LOW, critSect);
        }
    }

    return pEntry;
}

// this happens in user space
// however it should be an atomic access
bool TCPIP_MODULE_SignalFunctionDeregister(TCPIP_MODULE_SIGNAL_HANDLE signalHandle)
{
    TCPIP_MODULE_SIGNAL_ENTRY* pSignalEntry = (TCPIP_MODULE_SIGNAL_ENTRY*)signalHandle;
    if(pSignalEntry)
    {
        pSignalEntry->userSignalF = 0;
        return true;
    }

    return false;
}


// from user space
// read only atomic access
TCPIP_MODULE_SIGNAL TCPIP_MODULE_SignalGet(TCPIP_STACK_MODULE modId)
{
    if(modId == TCPIP_MODULE_MANAGER)
    {
        return _TCPIPStackManagerSignalClear(0);
    }
    else if(0 < modId && modId < sizeof(TCPIP_STACK_MODULE_SIGNAL_TBL)/sizeof(*TCPIP_STACK_MODULE_SIGNAL_TBL))
    {
        TCPIP_MODULE_SIGNAL_ENTRY* pSignalEntry = TCPIP_STACK_MODULE_SIGNAL_TBL + modId;
        return pSignalEntry->signalVal;
    }

    return TCPIP_MODULE_SIGNAL_NONE; 
}

// debugging features
//
#if defined(TCPIP_STACK_TIME_MEASUREMENT)
#include <cp0defs.h>

void TCPIP_STACK_TimeMeasureStart(bool reset)
{
    if(reset)
    {
        tcpip_stack_time = 0;
    }

    tcpip_stack_timeEnable = true;
}

uint64_t TCPIP_STACK_TimeMeasureGet(bool stop)
{
    if(stop)
    {
        tcpip_stack_timeEnable = false;
    }

    return tcpip_stack_time;
}

#endif // defined(TCPIP_STACK_TIME_MEASUREMENT)

