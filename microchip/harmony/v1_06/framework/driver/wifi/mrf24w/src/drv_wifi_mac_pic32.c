/*******************************************************************************
  MRF24W Driver Medium Access Control (MAC) Layer

  File Name:
    drv_wifi_mac_pic32.c

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

// Compile only for PIC32MX MRF24W MAC interface
#if (defined(__PIC32MZ__) && defined(TCPIP_IF_MRF24W)) || (defined(__PIC32MX__) && defined(TCPIP_IF_MRF24W))

// function prototypes
static SYS_MODULE_OBJ         MRF24W_MACInitialize(const SYS_MODULE_INDEX index, const SYS_MODULE_INIT *const init);
static void                   MRF24W_MACDeinitialize(SYS_MODULE_OBJ object);
static void                   MRF24W_MACReinitialize(SYS_MODULE_OBJ object, const SYS_MODULE_INIT *const init);
static SYS_STATUS             MRF24W_MACStatus(SYS_MODULE_OBJ object);
static DRV_HANDLE             MRF24W_MACOpen(const SYS_MODULE_INDEX drvIndex, const DRV_IO_INTENT intent);
static void                   MRF24W_MACTasks(SYS_MODULE_OBJ object);

static void                   MRF24W_MACClose(TCPIP_MAC_HANDLE hMac);
static bool                   MRF24W_MACCheckLink(TCPIP_MAC_HANDLE hMac);
static TCPIP_MAC_RES          MRF24W_MACRxFilterSetHashTableEntry(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_ADDR *DestMACAddr);
static bool                   MRF24W_MACPowerMode(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_POWER_MODE pwrMode);
static TCPIP_MAC_RES          MRF24W_MACTxPacket(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_PACKET *ptrPacket);
static TCPIP_MAC_PACKET      *MRF24W_MACRxPacket(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_RES *pRes, const TCPIP_MAC_PACKET_RX_STAT **ppPktStat);
static TCPIP_MAC_RES          MRF24W_MACProcess(TCPIP_MAC_HANDLE hMac);
static TCPIP_MAC_RES          MRF24W_MACStatisticsGet(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_RX_STATISTICS *pRxStatistics, TCPIP_MAC_TX_STATISTICS *pTxStatistics);
static TCPIP_MAC_RES          MRF24W_MACParametersGet(DRV_HANDLE hMac, TCPIP_MAC_PARAMETERS *pMacParams);
static TCPIP_MAC_RES          MRF24W_MACRegisterStatisticsGet(DRV_HANDLE hMac, TCPIP_MAC_STATISTICS_REG_ENTRY *pRegEntries, int nEntries, int* pHwEntries);
static size_t                 MRF24W_MACGetConfig(DRV_HANDLE hMac, void *configBuff, size_t buffSize, size_t *pConfigSize);

static bool                   MRF24W_MACEventSetMask(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents, bool enable);
static bool                   MRF24W_MACEventAcknowledge(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents);
static TCPIP_MAC_EVENT        MRF24W_MACEventGetPending(TCPIP_MAC_HANDLE hMac);
static void                   _MRF24W_MACDeinitialize(MRF24W_MAC_DCPT *pDcpt );

// the PIC32 MRF24W MAC descriptor
// no support for multiple instances
/*static*/ const TCPIP_MAC_OBJECT DRV_MRF24W_MACObject =
{
    TCPIP_MODULE_MAC_MRF24W,
    "MRF24W",
    MRF24W_MACInitialize,
    MRF24W_MACDeinitialize,
    MRF24W_MACReinitialize,
    MRF24W_MACStatus,
    MRF24W_MACTasks,
    MRF24W_MACOpen,
    MRF24W_MACClose,
    MRF24W_MACCheckLink,
    MRF24W_MACRxFilterSetHashTableEntry,
    MRF24W_MACPowerMode,
    MRF24W_MACTxPacket,
    MRF24W_MACRxPacket,
    MRF24W_MACProcess,
    MRF24W_MACStatisticsGet,
    MRF24W_MACParametersGet,
    MRF24W_MACRegisterStatisticsGet,
    MRF24W_MACGetConfig,
    MRF24W_MACEventSetMask,
    MRF24W_MACEventAcknowledge,
    MRF24W_MACEventGetPending,
};

// only one hardware instance for now!
static MRF24W_MAC_DCPT _pic32_mrf24w_mac_dcpt[1] =
{
    {
        &DRV_MRF24W_MACObject,
        // specific PIC32 MAC data
        0,                        // pNetIf
        0,                        // isInit
        0,                        // isOpen
        SYS_STATUS_UNINITIALIZED, // sysStat
    }
};


static bool s_runTxStateMachine = false;
static bool s_runRxStateMachine = false;
static TCPIP_MAC_MODULE_CTRL s_content_stackData;

/*
 * interface functions
 */
static SYS_MODULE_OBJ MRF24W_MACInitialize(const SYS_MODULE_INDEX index, const SYS_MODULE_INIT *const init)
{
    MRF24W_MAC_DCPT *pDcpt;
    const TCPIP_MAC_MODULE_CTRL *const stackData = ((TCPIP_MAC_INIT *)init)->macControl;
    const TCPIP_MODULE_MAC_MRF24W_CONFIG *initData = (const TCPIP_MODULE_MAC_MRF24W_CONFIG *)((TCPIP_MAC_INIT *)init)->moduleData;
    TCPIP_MAC_RES    res;

    pDcpt = _pic32_mrf24w_mac_dcpt + 0; // no other instance supported
    if (pDcpt->isInit != 0)
    {
        return (SYS_MODULE_OBJ)pDcpt; // already initialized, have a client connected
    }

    // save pointer to stack data
    s_content_stackData = *stackData;

    if (stackData->moduleId != TCPIP_MODULE_MAC_MRF24W)
    {
        return SYS_MODULE_OBJ_INVALID; // no other type supported
    }

    if (MRF24W_MACEventInit(pDcpt, stackData->eventF, stackData->eventParam, WF_EVENT_IPL, WF_EVENT_SIPL) != TCPIP_MAC_RES_OK)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    pDcpt->isInit = true;
    res = MRF24W_MACInit(stackData, initData);

    if ((res != TCPIP_MAC_RES_OK) && (res != TCPIP_MAC_RES_PENDING))
    {
        _MRF24W_MACDeinitialize(pDcpt);
        return SYS_MODULE_OBJ_INVALID;
    }

    pDcpt->sysStat = SYS_STATUS_BUSY;
    return (SYS_MODULE_OBJ)pDcpt;
}

static void MRF24W_MACTasks(SYS_MODULE_OBJ object)
{
    TCPIP_MAC_RES res;
    MRF24W_MAC_DCPT *pDcpt = (MRF24W_MAC_DCPT *)object;

    switch (pDcpt->sysStat) {
    case SYS_STATUS_UNINITIALIZED:
        break;

    case SYS_STATUS_BUSY:
        // perform the Wi-Fi initialization
        res = MRF24W_MACInit(0, 0);
        if (res == TCPIP_MAC_RES_PENDING)
        {   // not ready yet
            break;
        }

        if (res != TCPIP_MAC_RES_OK)
        {   // some error occurred
            _MRF24W_MACDeinitialize(pDcpt);
            pDcpt->sysStat = SYS_STATUS_ERROR;
        }
        else
        {
            pDcpt->sysStat = SYS_STATUS_READY;
        }
        break;

    case SYS_STATUS_READY:
    case SYS_STATUS_READY_EXTENDED:
        if (WiFiAsyncGetEventPending())
        {
            WiFiAsyncTask();
        }
        break;

    default: // SYS_STATUS_ERROR, SYS_STATUS_ERROR_EXTENDED
        SYS_CONSOLE_MESSAGE("Wi-Fi Driver reports SYS_STATUS_ERROR!\r\n");
        break;
    }
}

static void MRF24W_MACDeinitialize(SYS_MODULE_OBJ object )
{
    MRF24W_MAC_DCPT *pDcpt = (MRF24W_MAC_DCPT *)object;

    _MRF24W_MACDeinitialize(pDcpt);
}

static void MRF24W_MACReinitialize(SYS_MODULE_OBJ object, const SYS_MODULE_INIT * const init)
{
    // not supported
}

static SYS_STATUS MRF24W_MACStatus ( SYS_MODULE_OBJ object )
{
    MRF24W_MAC_DCPT* pDcpt = (MRF24W_MAC_DCPT*)object;

    return pDcpt->sysStat;
}

static size_t MRF24W_MACGetConfig(DRV_HANDLE hMac, void *configBuff, size_t buffSize, size_t *pConfigSize)
{
    if (pConfigSize)
    {
        *pConfigSize =  sizeof(TCPIP_MODULE_MAC_MRF24W_CONFIG);
    }

    if (configBuff && buffSize >= sizeof(TCPIP_MODULE_MAC_MRF24W_CONFIG))
    {   // can copy the data
#if defined(NOT_COMPLETED_YET)
        avoid warnings until fixed
            MRF24W_MAC_DCPT* pDcpt = (MRF24W_MAC_DCPT*)hMac;
            TCPIP_MODULE_MAC_MRF24W_CONFIG* pWiFiConfig = (TCPIP_MODULE_MAC_MRF24W_CONFIG*)configBuff;
#endif
        // TODO aa: fill in the pWiFiConfig fields from pDcpt

        return sizeof(TCPIP_MODULE_MAC_MRF24W_CONFIG);
    }

    return 0;
}

static DRV_HANDLE MRF24W_MACOpen(const SYS_MODULE_INDEX drvIndex, const DRV_IO_INTENT intent)
{
    MRF24W_MAC_DCPT * pDcpt;
    DRV_HANDLE hMac = DRV_HANDLE_INVALID;

    if (drvIndex == TCPIP_MODULE_MAC_MRF24W)
    {
        pDcpt = _pic32_mrf24w_mac_dcpt + 0; // no other instance supported
        if (!(pDcpt->isOpen))
        { // only one client for now
            pDcpt->isOpen = true;
            hMac = (DRV_HANDLE)pDcpt;
        }
    }

    return hMac;
}

static void MRF24W_MACClose(TCPIP_MAC_HANDLE hMac)
{
    MRF24W_MAC_DCPT *pDcpt = (MRF24W_MAC_DCPT *)hMac;
    pDcpt->isOpen = false;
}

/****************************************************************************
 * Function:        MRF24W_MACCheckLink
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          true if the link is up
 *                  false otherwise
 *
 * Side Effects:    None
 *
 * Overview:        This function periodically checks the link status
 *                  performing the MAC reconfiguration if the link went up
 *                  after being down.
 *
 * Note:            If auto negotiation is enabled the MAC we may have to be reconfigured.
 *****************************************************************************/
static bool MRF24W_MACCheckLink(TCPIP_MAC_HANDLE hMac)
{
    // TODO aa: update!
    return _MRF24W_MACCheckLink();
    //return true;
}

static TCPIP_MAC_RES MRF24W_MACRxFilterSetHashTableEntry(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_ADDR *DestMACAddr)
{
#if defined(TCPIP_STACK_USE_IPV6)
    static unsigned int usedFilters = 2;
#else
    static unsigned int usedFilters = 0;
#endif

#if (WF_SOFTWARE_MULTICAST_FILTER == DRV_WIFI_ENABLED)
    if (++usedFilters > 16) {
        SYS_ERROR_PRINT(SYS_ERROR_ERROR, "all 16 filters are already consumed!\r\n%s, line %u\r\n", __FILE__, __LINE__);
        return TCPIP_MAC_RES_OP_ERR;
    }
    DRV_WIFI_SWMultiCastFilterEnable();
    do {
       DRV_WIFI_SWMULTICAST_CONFIG p_config;

       p_config.filterId = usedFilters + 3;
       p_config.action = DRV_WIFI_MULTICAST_USE_FILTERS;
       memcpy((void *)p_config.macBytes, (uint8_t *)DestMACAddr, WF_MAC_ADDRESS_LENGTH);
       p_config.macBitMask = 0x3F;
       DRV_WIFI_SWMulticastFilterSet(&p_config);
   } while (0);
#else // using HW multicast filter
     if (++usedFilters > 2) {
        SYS_ERROR_PRINT(SYS_ERROR_ERROR, "all 2 filters are already consumed!\r\n%s, line %u\r\n", __FILE__, __LINE__);
        return TCPIP_MAC_RES_OP_ERR;
    }
    DRV_WIFI_HWMulticastFilterSet(DRV_WIFI_MULTICAST_FILTER_1 + usedFilters - 1, (uint8_t *) DestMACAddr);
#endif
    return TCPIP_MAC_RES_OK;
}

static bool MRF24W_MACPowerMode(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_POWER_MODE pwrMode)
{
    switch (pwrMode) {
    case TCPIP_MAC_POWER_FULL:
        MRF24W_MACPowerUp();
        break;

    case TCPIP_MAC_POWER_DOWN:
        MRF24W_MACPowerDown();
        break;

    default:
        return false;
    }

    return true;
}

static TCPIP_MAC_RES MRF24W_MACTxPacket(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_PACKET *ptrPacket)
{
    TCPIP_MAC_RES res;

    // if in hibernate mode then signal error to application
    if (DRV_WIFI_isHibernateEnable())
    {
        WF_UserEventsSet(DRV_WIFI_EVENT_ERROR, DRV_WIFI_ERROR_IN_HIBERNATE_MODE, true);
        return TCPIP_MAC_RES_QUEUE_TX_FULL;
    }

    res = MRF24W_TxPacket(ptrPacket);
    return res;
}

static TCPIP_MAC_PACKET* MRF24W_MACRxPacket (TCPIP_MAC_HANDLE hMac, TCPIP_MAC_RES* pRes, const TCPIP_MAC_PACKET_RX_STAT** ppPktStat)
{
    // get the oldest Rx packet
    return MRF24W_GetRxPacket();
}

static TCPIP_MAC_RES _MRF24W_MACProcess(TCPIP_MAC_HANDLE hMac)
{
    MRF24W_USR_EV_DCPT eventInfo;

    // if any events occurred
    if (WF_MgmtEventsGet(&eventInfo))
    {
        // process the WiFi event
        DRV_WIFI_ProcessEvent(eventInfo.mgmtEvents, eventInfo.mgmtEventInfo);
    }

    if (s_runRxStateMachine)
    {
        s_runRxStateMachine = false;
        WiFi_DataRxTask();
    }

    if (s_runTxStateMachine)
    {
        s_runTxStateMachine = false;
        WiFi_DataTxTask();
    }

#if defined(WF_UPDATE_FIRMWARE_XMODEM)
    WF_FirmwareUpdate_XMODEM();
#endif
#if defined(WF_UPDATE_FIRMWARE_OTA)
    WF_FirmwareUpdate_OTA();
#endif


    return TCPIP_MAC_RES_OK;
}

static TCPIP_MAC_RES MRF24W_MACProcess(TCPIP_MAC_HANDLE hMac)
{
#if defined(WIFI_USE_RTOS)
    WF_MACTask_SemGive();
    return TCPIP_MAC_RES_OK;
#else
    return _MRF24W_MACProcess(hMac);
#endif
}

#if defined(WIFI_USE_RTOS)
void DRV_WIFI_MACTask(void *p_arg)
{
    while (1)
    {
        WF_MACTask_SemTake();
        _MRF24W_MACProcess(0);
    }
}
#endif /* defined(WIFI_USE_RTOS) */

static TCPIP_MAC_RES MRF24W_MACStatisticsGet(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_RX_STATISTICS *pRxStatistics, TCPIP_MAC_TX_STATISTICS* pTxStatistics)
{
    // not supported for now
    return TCPIP_MAC_RES_OP_ERR;
}

static TCPIP_MAC_RES MRF24W_MACParametersGet(DRV_HANDLE hMac, TCPIP_MAC_PARAMETERS *pMacParams)
{
    MRF24W_MAC_DCPT* pDcpt = (MRF24W_MAC_DCPT*)hMac;

    if (pDcpt->sysStat == SYS_STATUS_READY)
    {
        if (pMacParams)
        {
            memcpy(pMacParams->ifPhyAddress.v, s_content_stackData.ifPhyAddress.v, sizeof(pMacParams->ifPhyAddress));
            pMacParams->processFlags = (TCPIP_MAC_PROCESS_FLAG_RX | TCPIP_MAC_PROCESS_FLAG_TX);
            pMacParams->macType = TCPIP_MAC_TYPE_WLAN;
        }

        return TCPIP_MAC_RES_OK;
    }

    return TCPIP_MAC_RES_IS_BUSY;
}

static TCPIP_MAC_RES MRF24W_MACRegisterStatisticsGet(DRV_HANDLE hMac, TCPIP_MAC_STATISTICS_REG_ENTRY* pRegEntries, int nEntries, int* pHwEntries)
{
    // not supported for now
    return TCPIP_MAC_RES_OP_ERR;
}

static bool MRF24W_MACEventSetMask(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents, bool enable)
{
    return _MRF24W_MACEventSetMask(hMac, macEvents, enable);
}

static bool MRF24W_MACEventAcknowledge(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents)
{
    return _MRF24W_MACEventAcknowledge(hMac, macEvents);
}

static TCPIP_MAC_EVENT MRF24W_MACEventGetPending(TCPIP_MAC_HANDLE hMac)
{
    return _MRF24W_MACEventGetPending(hMac);
}

static void _MRF24W_MACDeinitialize(MRF24W_MAC_DCPT *pDcpt)
{
    if (pDcpt->isInit)
    {
        if (WFisConnected())
        {
            DRV_WIFI_Disconnect();
        }
        DRV_WIFI_Deinitialize();
        MRF24W_ChipReset(); // will stop the MR24W from transmitting
        DRV_WIFI_SpiClose();
        WiFiAsyncClearAllEvents();
        MRF24W_MACEventDeInit(pDcpt);
        DRV_WIFI_HibernateEnable();
        pDcpt->isInit = false;
    }
}

const TCPIP_MAC_MODULE_CTRL * GetStackData(void)
{
    return &s_content_stackData;
}

bool MRF24W_IsMACInit(void)
{
    MRF24W_MAC_DCPT *pDcpt;

    pDcpt = _pic32_mrf24w_mac_dcpt + 0; // no other instance supported
    return pDcpt->isInit;
}

void SignalTxStateMachine(void)
{
    s_runTxStateMachine = true;
    WF_UserEventsSet(TCPIP_MAC_EV_TX_DONE, 0, false); // will result in MRF24W_MACProcess being called
}

void SignalRxStateMachine(void)
{
    s_runRxStateMachine = true;
    WF_UserEventsSet(TCPIP_MAC_EV_RX_DONE, 0, false); // will result in MRF24W_MACProcess being called
}

#endif // (defined(__PIC32MZ__) && defined(TCPIP_IF_MRF24W)) || (defined(__PIC32MX__) && defined(TCPIP_IF_MRF24W))

//DOM-IGNORE-END
