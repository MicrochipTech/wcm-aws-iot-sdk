/*******************************************************************************
  MRF24WN Wireless Driver

  File Name:  
    wdrv_mrf24wn_main.c  
  
  Summary:
    Module for Microchip TCP/IP Stack PIC32 implementation
    for multiple MAC support
    
  Description:
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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

#include "tcpip/src/tcpip_private.h"
#include "wdrv_mrf24wn_common.h"
#include "wdrv_mrf24wn_main.h"

// Compile only for PIC32MX or PIC32MZ MRF24WN MAC interface
#if (defined(__PIC32MZ__) && defined(TCPIP_IF_MRF24WN)) || (defined(__PIC32MX__) && defined(TCPIP_IF_MRF24WN))

#define ETH_HEADER_SIZE 14
#define MAX_IP_PACKET_SIZE 1564 // including header
#define NUM_PREALLOCATED_RX_PACKETS 4
#define TCPIP_THIS_MODULE_ID TCPIP_MODULE_MAC_MRF24WN
#define MAX_RX_PACKET_SIZE 1518
#define MAX_TX_PACKET_SIZE 1518

// used to keep track of rx packets queued for stack
typedef struct fifo_struct
{
    int front;
    int rear;
    TCPIP_MAC_PACKET *items[NUM_PREALLOCATED_RX_PACKETS + 1];
} t_fifo;

// MRF24WN wireless driver descriptor
typedef struct
{
    const TCPIP_MAC_OBJECT *pObj;    // safe cast to TCPIP_MAC_DCPT
    TCPIP_NET_IF *pNetIf;            // interface we belong to
    bool isOpen;                     // simple open status flag                                 
    SYS_STATUS sysStat;              // driver status
} WDRV_MRF24WN_DCPT;

bool g_module_event_print = false;
static _TCPIP_PKT_ALLOC_PTR s_pktAllocF = 0;
static _TCPIP_PKT_FREE_PTR s_pktFreeF = 0;
static _TCPIP_PKT_ACK_PTR s_pktAckF = 0;
static t_fifo s_rxFifo; 
static SINGLE_LIST s_dataRxQueue; // queue of data rx packets waiting to be processed by stack from host
static TCPIP_MAC_PACKET *s_rxpacket_buffer[NUM_PREALLOCATED_RX_PACKETS];
WDRV_MRF24WN_PRIV g_wdrv_priv;
static TCPIP_MAC_MODULE_CTRL s_stackData;
WDRV_CONFIG g_wdrv_config;
uint32_t g_wdrv_init_task_priority; 
uint32_t g_wdrv_main_task_priority;
uint32_t g_wdrv_init_task_stack_size; 
uint32_t g_wdrv_main_task_stack_size;

static SYS_MODULE_OBJ WDRV_MRF24WN_Initialize(const SYS_MODULE_INDEX index, const SYS_MODULE_INIT *const init);
static void WDRV_MRF24WN_Tasks(SYS_MODULE_OBJ object);
static void WDRV_MRF24WN_Reinitialize(SYS_MODULE_OBJ object, const SYS_MODULE_INIT *const init);
static void WDRV_MRF24WN_Deinitialize(SYS_MODULE_OBJ object);
static SYS_STATUS WDRV_MRF24WN_Status (SYS_MODULE_OBJ object);
static DRV_HANDLE WDRV_MRF24WN_Open(const SYS_MODULE_INDEX drvIndex, const DRV_IO_INTENT intent);
static TCPIP_MAC_RES WDRV_MRF24WN_RegisterStatisticsGet(DRV_HANDLE hMac, TCPIP_MAC_STATISTICS_REG_ENTRY *pRegEntries, 
        int nEntries, int *pHwEntries);
static void WDRV_MRF24WN_Close(TCPIP_MAC_HANDLE hMac);
static bool WDRV_MRF24WN_CheckLink(TCPIP_MAC_HANDLE hMac);
static TCPIP_MAC_RES WDRV_MRF24WN_MulticastFilterSet(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_ADDR *DestMACAddr);
static bool WDRV_MRF24WN_PowerMode(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_POWER_MODE pwrMode);
static TCPIP_MAC_RES WDRV_MRF24WN_TxPacket(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_PACKET *ptrPacket);
static TCPIP_MAC_PACKET *WDRV_MRF24WN_RxPacket (TCPIP_MAC_HANDLE hMac, TCPIP_MAC_RES *pRes, 
        const TCPIP_MAC_PACKET_RX_STAT **ppPktStat);
static TCPIP_MAC_RES WDRV_MRF24WN_Process(TCPIP_MAC_HANDLE hMac);
static TCPIP_MAC_RES WDRV_MRF24WN_StatisticsGet(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_RX_STATISTICS *pRxStatistics, 
        TCPIP_MAC_TX_STATISTICS *pTxStatistics);
static TCPIP_MAC_RES WDRV_MRF24WN_ParametersGet(DRV_HANDLE hMac, TCPIP_MAC_PARAMETERS *pMacParams);
static bool WDRV_MRF24WN_EventSetMask(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents, bool enable);
static bool WDRV_MRF24WN_EventAcknowledge(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents);
static TCPIP_MAC_EVENT WDRV_MRF24WN_EventGet(TCPIP_MAC_HANDLE hMac);
static size_t WDRV_MRF24WN_GetConfig(TCPIP_MODULE_MAC_ID modId, void *configBuff, size_t buffSize, size_t *pConfigSize);
static void PowerDown(void);
static void PowerUp(void);
static void FifoInit(t_fifo *const p_fifo);
static bool isFifoEmpty(t_fifo *const p_fifo);
static void FifoInsert(t_fifo *const p_fifo, TCPIP_MAC_PACKET *p_packet);
static TCPIP_MAC_PACKET *FifoRemove(t_fifo *const p_fifo);
static TCPIP_MAC_PACKET *GetRxPacket(void);
static bool RxDataCallback(TCPIP_MAC_PACKET *pktHandle, const void *ackParam);
static void InitRxBuffer(void);
static void DeInitRxBuffer(void);
static void CopyFrameToStackPacketBufferCB(uint32_t len, uint8_t const *const frame);

const TCPIP_MAC_OBJECT WDRV_MRF24WN_MACObject = 
{
    TCPIP_MODULE_MAC_MRF24WN,
    "MRF24WN",
    WDRV_MRF24WN_Initialize,
    WDRV_MRF24WN_Deinitialize,
    WDRV_MRF24WN_Reinitialize,
    WDRV_MRF24WN_Status,
    WDRV_MRF24WN_Tasks,
    WDRV_MRF24WN_Open,
    WDRV_MRF24WN_Close,
    WDRV_MRF24WN_CheckLink,
    WDRV_MRF24WN_MulticastFilterSet,
    WDRV_MRF24WN_PowerMode,
    WDRV_MRF24WN_TxPacket,
    WDRV_MRF24WN_RxPacket,
    WDRV_MRF24WN_Process,
    WDRV_MRF24WN_StatisticsGet,
    WDRV_MRF24WN_ParametersGet,
    WDRV_MRF24WN_RegisterStatisticsGet,
    WDRV_MRF24WN_GetConfig,
    WDRV_MRF24WN_EventSetMask,
    WDRV_MRF24WN_EventAcknowledge,
    WDRV_MRF24WN_EventGet,
};

// only one hardware instance for now!
static WDRV_MRF24WN_DCPT wdrv_mrf24wn_dcpt = 
{
    &WDRV_MRF24WN_MACObject,     // specific PIC32 MAC data 
    0,                           // isOpen
    SYS_STATUS_UNINITIALIZED,    // sysStat
};

static void RFReadyCB(uint8_t const *const addr)
{
    g_wdrv_priv.updateMacAddressRequired = 1;
    memcpy(g_wdrv_priv.macAddr, addr, 6);
}

static void ScanDoneCB(int32_t status)
{
    if (status == 0) { // 0 means success
        WDRV_EXT_ScanDoneSet();
    } else {
        WDRV_DBG_INFORM_PRINT(("Scan failed, status = %d\r\n", status));
    }
}
static void InitDoneCB(void)
{
    g_wdrv_priv.initDriverDone = true;
}

static void DeinitDoneCB(void)
{
    g_wdrv_priv.deinitDriverDone = true;
}

static TCPIP_MAC_RES WDRV_MRF24WN_RegisterStatisticsGet(DRV_HANDLE hMac, TCPIP_MAC_STATISTICS_REG_ENTRY *pRegEntries,
    int nEntries, int *pHwEntries)
{
    // TODO :Unsupported for now
    return TCPIP_MAC_RES_OP_ERR;
}

static void WDRV_MRF24WN_Close(TCPIP_MAC_HANDLE hMac)
{
    WDRV_MRF24WN_DCPT *pMacD;
    
    pMacD = &wdrv_mrf24wn_dcpt;
    if (pMacD->isOpen == 1) {
        if (WDRV_EXT_CmdDisconnect() == TCPIP_MAC_RES_OK) {
            pMacD->isOpen = 0;
        }
    }
}

static bool WDRV_MRF24WN_CheckLink(TCPIP_MAC_HANDLE hMac)
{
    return NetworkConnected();
}

static TCPIP_MAC_RES WDRV_MRF24WN_MulticastFilterSet(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_ADDR *DestMACAddr)
{
    // 11n module does not require setting multicast filter. All multicast packets will be received.
    return TCPIP_MAC_RES_OK;
}

static bool WDRV_MRF24WN_PowerMode(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_POWER_MODE pwrMode)
{
    if (pwrMode == TCPIP_MAC_POWER_FULL)
        PowerUp();
    else if (pwrMode == TCPIP_MAC_POWER_DOWN)
        PowerDown();

    return true;
}

static TCPIP_MAC_RES WDRV_MRF24WN_TxPacket(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_PACKET * ptrPacket)
{
    TCPIP_MAC_RES res = TCPIP_MAC_RES_OK;
    TCPIP_MAC_PACKET const *p_packet = ptrPacket;
    TCPIP_MAC_DATA_SEGMENT *p_seg;
    uint8_t *p_segData;
    uint32_t sendResult;
    uint8_t *buffer = NULL;
    uint16_t curIndex = 0;

    if (NetworkConnected() == false) {
        WDRV_DBG_INFORM_MESSAGE(("Dropped the Tx packet. Stack sent a packet in unconnected state.\r\n"));
        res = TCPIP_MAC_RES_PACKET_ERR;
        goto out;
    }

    buffer = WDRV_MALLOC(MAX_IP_PACKET_SIZE);
    if (buffer == NULL) {
        WDRV_DBG_ERROR_MESSAGE(("Dropped the Tx packet. Could not alloc tx buffer.\r\n"));
        res = TCPIP_MAC_RES_ALLOC_ERR;
        goto out;
    }

    // write out the first segment to MRF, including prepended internal header
    p_seg = p_packet->pDSeg; // point to first segment
    p_segData = p_seg->segLoad;

    if (p_seg->segLen > MAX_TX_PACKET_SIZE) {
        WDRV_DBG_ERROR_PRINT(("Dropped the Tx packet. Invalid packet length %d.\r\n", p_seg->segLen));
        res = TCPIP_MAC_RES_PACKET_ERR;
        goto out;
    }
    
    memcpy(buffer, p_segData, p_seg->segLen);
    curIndex += p_seg->segLen;

    while (p_seg->next != NULL) {
        
        if (curIndex > MAX_TX_PACKET_SIZE) {
            WDRV_DBG_ERROR_PRINT(("Dropped the Tx packet. Invalid packet length %d.\r\n", curIndex));
            res = TCPIP_MAC_RES_PACKET_ERR;
            goto out;
        }
        
        p_seg = p_seg->next;
        p_segData = p_seg->segLoad;
        memcpy(buffer + curIndex, p_segData, p_seg->segLen);
        curIndex += p_seg->segLen;
    }
   
    sendResult = WDRV_EXT_DataSend(curIndex, buffer);
    if (sendResult != 0) {
        res = TCPIP_MAC_RES_PACKET_ERR;
        WDRV_DBG_INFORM_MESSAGE(("Dropped the Tx packet. No tx buffer is available now.\r\n"));
    } else {
        WDRV_DBG_TRACE_MESSAGE(("sent the packet to the module.\r\n"));
    }

out:
    if (buffer) 
        WDRV_FREE(buffer);

    // call stack ack function to let it know packet was transmitted
    if (s_pktAckF) { 
        _TCPIP_PKT_ACK_BY_PTR(s_pktAckF, ptrPacket, TCPIP_MAC_PKT_ACK_TX_OK);
    } else {
        WDRV_ASSERT(false, "Should never happen");
    }
    
    return res;
}

static TCPIP_MAC_PACKET* WDRV_MRF24WN_RxPacket (TCPIP_MAC_HANDLE hMac, TCPIP_MAC_RES* pRes, 
    const TCPIP_MAC_PACKET_RX_STAT** ppPktStat)
{
    WDRV_DBG_TRACE_MESSAGE(("recieved packet from the module.\r\n"));
    return GetRxPacket();
}

static TCPIP_MAC_RES WDRV_MRF24WN_Process(TCPIP_MAC_HANDLE hMac)
{
    // Do nothing. Just place holder. 
    return TCPIP_MAC_RES_OK;
}

static TCPIP_MAC_RES WDRV_MRF24WN_StatisticsGet(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_RX_STATISTICS* pRxStatistics, 
    TCPIP_MAC_TX_STATISTICS* pTxStatistics)
{
    // TODO : unsupported for now.
    return TCPIP_MAC_RES_OP_ERR;
}

static TCPIP_MAC_RES WDRV_MRF24WN_ParametersGet(DRV_HANDLE hMac, TCPIP_MAC_PARAMETERS *pMacParams)
{
    WDRV_MRF24WN_DCPT *pDcpt = (WDRV_MRF24WN_DCPT *)hMac;

    if(pDcpt->sysStat == SYS_STATUS_READY) { 
        if(pMacParams) {
            memcpy(pMacParams->ifPhyAddress.v, s_stackData.ifPhyAddress.v, sizeof(pMacParams->ifPhyAddress));
            pMacParams->processFlags = (TCPIP_MAC_PROCESS_FLAG_RX | TCPIP_MAC_PROCESS_FLAG_TX); 
            pMacParams->macType = TCPIP_MAC_TYPE_WLAN;
        }

        return TCPIP_MAC_RES_OK;
    }

    return TCPIP_MAC_RES_IS_BUSY;
}

static bool WDRV_MRF24WN_EventSetMask(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents, bool enable)
{
    return TrafficEventMask(hMac, macEvents, enable);
}

static bool WDRV_MRF24WN_EventAcknowledge(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents)
{
    return TrafficEventAcknowledge(hMac, macEvents);
}
    
static TCPIP_MAC_EVENT WDRV_MRF24WN_EventGet(TCPIP_MAC_HANDLE hMac)
{
    return  TrafficEventGet(hMac);
}
 
static void PowerDown(void)
{
    WDRV_EXT_PowerUpDown(false);
}

static void PowerUp(void)
{
    WDRV_EXT_PowerUpDown(true);
}

static void FifoInit(t_fifo *const p_fifo)
{
    memset(p_fifo, 0x00, sizeof(t_fifo));
    p_fifo->front = p_fifo->rear = NUM_PREALLOCATED_RX_PACKETS - 1;
}

static bool isFifoEmpty(t_fifo *const p_fifo)
{
    return p_fifo->front == p_fifo->rear;
}

static void FifoInsert(t_fifo * const p_fifo, TCPIP_MAC_PACKET *p_packet)
{
    if (p_fifo->rear == NUM_PREALLOCATED_RX_PACKETS - 1)
        p_fifo->rear = 0;
    else
        ++p_fifo->rear;

    p_fifo->items[p_fifo->rear] = p_packet;
}

static TCPIP_MAC_PACKET *FifoRemove(t_fifo *const p_fifo)
{
    if (p_fifo->front == NUM_PREALLOCATED_RX_PACKETS - 1)
        p_fifo->front = 0;
    else
        ++p_fifo->front;

    return p_fifo->items[p_fifo->front];
}

// retrieve the oldest of the queued Rx packets to deliver to the stack
static TCPIP_MAC_PACKET *GetRxPacket(void)
{
    if (!isFifoEmpty(&s_rxFifo))
        return FifoRemove(&s_rxFifo);
    else
        return NULL; // signals no rx packet to process
}

static bool RxDataCallback(TCPIP_MAC_PACKET *pktHandle, const void *ackParam)
{
    ackParam = ackParam; // ignore warning

    if(pktHandle){
        // if this is packet allocated at init and is going to be reused
        if ((pktHandle->pDSeg->segFlags & TCPIP_MAC_SEG_FLAG_RX_STICKY) == TCPIP_MAC_SEG_FLAG_RX_STICKY) {
            pktHandle->pktFlags &= ~TCPIP_MAC_PKT_FLAG_QUEUED;
            TCPIP_Helper_SingleListTailAdd(&s_dataRxQueue, (SGL_LIST_NODE *)pktHandle); // add packet back to free list
        }
    } else {
        WDRV_ASSERT(false, "pktHandle must not be null");
    }

    return false;
}

static void InitRxBuffer(void)
{
    int32_t i;
    // create a queue to hold pointers to preallocated Rx packets
    TCPIP_Helper_SingleListInitialize(&s_dataRxQueue);

    for (i = 0; i < NUM_PREALLOCATED_RX_PACKETS; ++i) {
        // preallocate Rx buffers to store Rx packets as they come in (1500 bytes data plus header and checksum)
        s_rxpacket_buffer[i] = s_pktAllocF ? _TCPIP_PKT_ALLOC_BY_PTR(s_pktAllocF, sizeof(TCPIP_MAC_PACKET), MAX_RX_PACKET_SIZE, 0) : 0; //_TCPIP_PKT_PacketAllocDebug

        if (s_rxpacket_buffer[i] != NULL) {
            s_rxpacket_buffer[i]->next = NULL;
            s_rxpacket_buffer[i]->ackFunc = RxDataCallback;
            s_rxpacket_buffer[i]->ackParam = NULL;
            s_rxpacket_buffer[i]->pktFlags = 0;
            s_rxpacket_buffer[i]->pDSeg->segFlags |= TCPIP_MAC_SEG_FLAG_RX_STICKY;
            TCPIP_Helper_SingleListTailAdd(&s_dataRxQueue, (SGL_LIST_NODE *)s_rxpacket_buffer[i]);
        } else {
            WDRV_ASSERT(false, "");
        }
    }

    FifoInit(&s_rxFifo);
}

static void DeInitRxBuffer(void)
{
    int i;
    
    if (s_pktFreeF != NULL) {
        for (i = 0; i < NUM_PREALLOCATED_RX_PACKETS; ++i) {
            if (s_rxpacket_buffer[i] != NULL) {
                _TCPIP_PKT_FREE_BY_PTR(s_pktFreeF, s_rxpacket_buffer[i]);
                s_rxpacket_buffer[i] = NULL;
            }
        }
    }
}

// finds an available Rx packet structure from the list that was allocated and
// queued up at init.
static TCPIP_MAC_PACKET *GetAvailRxBuf(void)
{
    TCPIP_MAC_PACKET *p_packet = NULL;

    // if free list has an available Rx packet buffer
    if (s_dataRxQueue.nNodes > 0) {
        p_packet = (TCPIP_MAC_PACKET *)TCPIP_Helper_SingleListHeadRemove(&s_dataRxQueue);
        WDRV_ASSERT(p_packet != NULL, "Should never happen");
    }

    return p_packet;
}

static void CopyFrameToStackPacketBufferCB(uint32_t len, uint8_t const *const frame)
{
    TCPIP_MAC_PACKET *p_packet;

    WDRV_DBG_TRACE_MESSAGE(("received data\r\n"));

    if (NetworkConnected() == false) {
        WDRV_DBG_INFORM_MESSAGE(("Receivd a packet in unconnected state. The packet will be discarded.\r\n"));
        return;
    }

    p_packet = GetAvailRxBuf();

    if(p_packet == NULL) { 
        WDRV_DBG_INFORM_MESSAGE(("Dropped the Rx packet. No Rx buffer is available now.\r\n"));
		return;
    }
    
    // mark packet as queued and stuff in timestamp
    p_packet->pDSeg->segLen = len - ETH_HEADER_SIZE;
    p_packet->pktFlags |= TCPIP_MAC_PKT_FLAG_QUEUED;
    p_packet->tStamp = SYS_TMR_TickCountGet();

    // Note: re-set pMacLayer and pNetLayer; IPv6 changes these pointers inside the packet, so
    //       when Rx packets are reused this is needed.
    memcpy(p_packet->pDSeg->segLoad, frame, len);
    p_packet->pMacLayer = p_packet->pDSeg->segLoad;
    p_packet->pNetLayer = p_packet->pMacLayer + sizeof(TCPIP_MAC_ETHERNET_HEADER);

    // store packet pointer in FIFO and signal stack that rx packet ready to process
    FifoInsert(&s_rxFifo, p_packet);

    // notify stack of Rx packet has arrived. 
    TrafficEventRequest(TCPIP_EV_RX_DONE, 0);
}

static SYS_MODULE_OBJ WDRV_MRF24WN_Initialize(const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init)
{
    WDRV_MRF24WN_DCPT* pDcpt;
    const TCPIP_MAC_MODULE_CTRL* const stackData = ((TCPIP_MAC_INIT*)init)->macControl;  
    CallBacks CB;

    if(stackData == NULL) { 
        WDRV_ASSERT(false, "stackData is null");
    }

    pDcpt = &wdrv_mrf24wn_dcpt; // no other instance supported
    if(pDcpt->isOpen != 0)
        return (SYS_MODULE_OBJ)pDcpt; // already initialized, have a client connected

    memset((void *)&g_wdrv_priv, 0, sizeof(WDRV_MRF24WN_PRIV));

    WDRV_MUTEX_CREATE(&g_wdrv_priv.debugMutex);

    memset((void *)&g_wdrv_config, 0, sizeof(g_wdrv_config));

#if WDRV_DEFAULT_POWER_SAVE == WDRV_FUNC_ENABLED  
    g_wdrv_config.powerSaveEnabled = true;
#endif
#if defined(TCPIP_STACK_USE_DHCP_CLIENT)
    g_wdrv_config.dhcpcUsed = true;
#endif
#if defined(TCPIP_STACK_USE_DHCP_SERVER)
    g_wdrv_config.dhcpsUsed = true;
#endif
    g_wdrv_config.securityMode = WDRV_DEFAULT_WIFI_SECURITY_MODE;
#if defined(WF_ENABLE_STATIC_IP)
    g_wdrv_config.staticIPEnabled = true;
#endif
#if MODULE_EVENT_PRINT == 1
    g_module_event_print = true;
#endif

    s_stackData = *stackData;
    s_pktAllocF = (_TCPIP_PKT_ALLOC_PTR)stackData->pktAllocF;
    s_pktFreeF = (_TCPIP_PKT_FREE_PTR)stackData->pktFreeF;
    s_pktAckF = (_TCPIP_PKT_ACK_PTR)stackData->pktAckF;

    if(stackData->moduleId != TCPIP_MODULE_MAC_MRF24WN)
        return SYS_MODULE_OBJ_INVALID;      // no other type supported

    TrafficEventInit(stackData->eventF, stackData->eventParam);

    InitRxBuffer();
    AsyncEventClearAll();

#if defined(TCPIP_STACK_COMMANDS_WIFI_ENABLE)
    WDRV_CLI_Init();
#endif

    g_wdrv_init_task_priority = WDRV_EXT_INIT_TASK_PRIO; 
    g_wdrv_main_task_priority = WDRV_EXT_MAIN_TASK_PRIO;
    g_wdrv_init_task_stack_size = WDRV_EXT_INIT_TASK_STACK_SIZE; 
    g_wdrv_main_task_stack_size = WDRV_EXT_MAIN_TASK_STACK_SIZE;

    CB.CopyFrameToStackPacketBuffer_CB = CopyFrameToStackPacketBufferCB;
    CB.ProceedConnectEvent_CB = ProceedConnectEventCB;
    CB.RFReady_CB = RFReadyCB;
    CB.ScanDone_CB = ScanDoneCB;
    CB.InitDone_CB = InitDoneCB;
    CB.DeinitDone_CB = DeinitDoneCB;
    CB.WPSDone_CB = WPSDoneCB;
    WDRV_EXT_Initialize(&CB);

    if (!WDRV_CONFIG_DataLoad())
        return SYS_MODULE_OBJ_INVALID;

    pDcpt->sysStat = SYS_STATUS_BUSY;

    return (SYS_MODULE_OBJ)pDcpt;
}

static void WDRV_MRF24WN_Tasks(SYS_MODULE_OBJ object)
{
    TCPIP_MAC_RES res = TCPIP_MAC_RES_PENDING;
    WDRV_MRF24WN_DCPT *pDcpt = (WDRV_MRF24WN_DCPT *)object;
    WDRV_MRF24WN_PRIV *priv = &g_wdrv_priv;
    static uint32_t startTick = 0;
    static bool connect_start_wait = false;
    int i;

    switch(pDcpt->sysStat) {
    case SYS_STATUS_UNINITIALIZED:
        break;
    case SYS_STATUS_BUSY:
        if (priv->updateMacAddressRequired) {
            memcpy((uint8_t *)(s_stackData.ifPhyAddress.v), priv->macAddr, 6);
            WDRV_DBG_INFORM_MESSAGE(("## mac address: "));
            for (i = 0; i < 6; i++) {
                WDRV_DBG_INFORM_PRINT(("%x ", s_stackData.ifPhyAddress.v[i]));
            }
            WDRV_DBG_INFORM_MESSAGE(("\r\n"));
            priv->updateMacAddressRequired = 0;
        }

        if (priv->initDriverDone) {
            if (wdrv_mrf24wn_dcpt.isOpen == 0) {
                /* In WPS mode, before connection, wait for 3 seconds */
                if (g_wdrv_config.securityMode == WDRV_SECURITY_WPS_PUSH_BUTTON || 
                    g_wdrv_config.securityMode == WDRV_SECURITY_WPS_PIN) {
                    if (connect_start_wait == false) {
                        connect_start_wait = true;
                        startTick = SYS_TMR_TickCountGet();
                    } else {
                        if (SYS_TMR_TickCountGet() - startTick >= SYS_TMR_TickCounterFrequencyGet()* 3ul) {
                            res = ConnectToNetwork();
                        }
                    } 
                } else {
                    res = ConnectToNetwork();  
                }
                if (res == TCPIP_MAC_RES_OK)
                    wdrv_mrf24wn_dcpt.isOpen = 1;
            } else
                res = TCPIP_MAC_RES_OK;
        }
        
        if(res == TCPIP_MAC_RES_OK) {
            pDcpt->sysStat = SYS_STATUS_READY;
            WDRV_DBG_TRACE_MESSAGE(("mac tasks ready\r\n"));
        } else if (res != TCPIP_MAC_RES_PENDING) {
            WDRV_CONFIG_DataSave();
            AsyncEventClearAll();
            TrafficEventDeInit();
            pDcpt->sysStat = SYS_STATUS_ERROR;
        }
        break;
    case SYS_STATUS_READY:
    case SYS_STATUS_READY_EXTENDED:
        if (AsyncEventPended())
            AsyncEventHandle();
        break;
    default: // SYS_STATUS_ERROR
        WDRV_ASSERT(false, "Should never come here.");
        break;
    }
}

static void WDRV_MRF24WN_Reinitialize(SYS_MODULE_OBJ object, const SYS_MODULE_INIT *const init)
{
    // TODO:
}

static void WDRV_MRF24WN_Deinitialize(SYS_MODULE_OBJ object)
{
    WDRV_MRF24WN_PRIV *priv = &g_wdrv_priv;

    WDRV_DBG_INFORM_MESSAGE(("wdrv deinitializing..\r\n"));
    WDRV_CONFIG_DataSave();
    AsyncEventClearAll();
    TrafficEventDeInit();
    DeInitRxBuffer();
    WDRV_EXT_Deinitialize();

    WDRV_MUTEX_DELETE(&priv->debugMutex);
}

static SYS_STATUS WDRV_MRF24WN_Status(SYS_MODULE_OBJ object)
{
    WDRV_MRF24WN_DCPT *pDcpt = (WDRV_MRF24WN_DCPT *)object;
    
    return pDcpt->sysStat;
}

static size_t WDRV_MRF24WN_GetConfig(TCPIP_MODULE_MAC_ID modId, void *configBuff, size_t buffSize, size_t *pConfigSize)
{
    if (pConfigSize)
        *pConfigSize = (modId == TCPIP_MODULE_MAC_MRF24WN) ? sizeof(TCPIP_MODULE_MAC_MRF24WN_CONFIG) : 0;

    if (modId == TCPIP_MODULE_MAC_MRF24WN) {
        if (configBuff && buffSize >= sizeof(TCPIP_MODULE_MAC_MRF24WN_CONFIG)) { // can copy the data
            //TCPIP_MODULE_MAC_MRF24WN_CONFIG *pWiFiConfig = (TCPIP_MODULE_MAC_MRF24WN_CONFIG *)configBuff;  // TODO
            // TODO aa: fill in the pWiFiConfig fields from pMacD

            return sizeof(TCPIP_MODULE_MAC_MRF24WN_CONFIG);
        }

        return 0;
    }

    return -1; // unknown module
}

static DRV_HANDLE WDRV_MRF24WN_Open(const SYS_MODULE_INDEX drvIndex, const DRV_IO_INTENT intent)
{
    WDRV_MRF24WN_DCPT *pMacD;
    DRV_HANDLE hMac = DRV_HANDLE_INVALID;
    TCPIP_MAC_RES res = TCPIP_MAC_RES_PENDING;

    if (drvIndex == TCPIP_MODULE_MAC_MRF24WN) {
        pMacD = &wdrv_mrf24wn_dcpt;
        if (pMacD->isOpen == 0) {
            if (g_wdrv_priv.initDriverDone == true) {
                res = ConnectToNetwork();
                if (res == TCPIP_MAC_RES_OK)
                    pMacD->isOpen = 1;
            }
            hMac = (DRV_HANDLE)pMacD;
        }
    }

    return hMac;
}

#endif // (defined(__PIC32MZ__) && defined(TCPIP_IF_MRF24WN)) || (defined(__PIC32MX__) && defined(TCPIP_IF_MRF24WN))

//DOM-IGNORE-END
