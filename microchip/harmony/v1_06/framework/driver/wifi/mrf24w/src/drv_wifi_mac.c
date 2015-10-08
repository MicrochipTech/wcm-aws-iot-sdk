/*******************************************************************************
  MRF24W Driver Medium Access Control (MAC) Layer

  File Name:
    drv_wifi_mac.c

  Summary:
    Module for Microchip TCP/IP Stack

  Description:
    -Provides access to MRF24W WiFi controller
    -Reference: MRF24W Data sheet, IEEE 802.11 Standard
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

/*
*********************************************************************************************************
*                                           INCLUDES
*********************************************************************************************************
*/
#include "tcpip/src/tcpip_private.h"
#include "system/tmr/sys_tmr.h"

#if defined(WIFI_USE_RTOS)
#include "FreeRTOS.h"
#include "task.h"
#endif

#if defined(TCPIP_IF_MRF24W)
#include "driver/wifi/mrf24w/src/drv_wifi_easy_config.h"

//#define DBG_TX_PRINT(s) SYS_CONSOLE_MESSAGE(s)
#define DBG_TX_PRINT(s)

//#define DBG_RX_PRINT(s) SYS_CONSOLE_MESSAGE(s)
#define DBG_RX_PRINT(s)

#define TCPIP_THIS_MODULE_ID TCPIP_MODULE_MAC_MRF24W

#define MAX_RX_PACKET_SIZE (1518)

/*
*********************************************************************************************************
*                                           DEFINES
*********************************************************************************************************
*/
#define NUM_PREALLOCATED_RX_PACKETS (4)

#define SNAP_HDR_LENGTH (6)
#define SNAP_VAL        (0xaa)
#define SNAP_CTRL_VAL   (0x03)
#define SNAP_TYPE_VAL   (0x00)

#define SNAP_HEADER_OFFSET      (10) // offset in t_RxHeader where SNAP header starts
#define ETH_HEADER_START_OFFSET (16) // offset in Rx packet where Ethernet header starts
#define WF_RX_PREAMBLE_SIZE     (sizeof(t_wfRxPreamble))
#define WF_TX_PREAMBLE_SIZE     (sizeof(t_wfTxPreamble))
#define ETH_HEADER_SIZE         (14)

#define DRV_WIFI_WPS_SAVING_FLAG 0x5a5a

//--------------
// RAW States
//--------------

// RAW Init states
enum
{
    R_INIT_BEGIN               = 0,
    R_WAIT_FOR_SCRATCH_UNMOUNT = 1,
    R_WAIT_FOR_SCRATCH_MOUNT   = 2
} t_rawInitStates;

//---------------
// Rx Data states
//---------------
typedef enum
{
    DATA_RX_IDLE             = 0,
    WAIT_FOR_TX_IDLE         = 1,
    WAIT_FOR_DATA_RX_MOUNT   = 2,
    WAIT_FOR_DATA_RX_UNMOUNT = 3
} t_RxStates;

//---------------
// Tx data states
//---------------
typedef enum
{
    DATA_TX_IDLE            = 0,
    WAIT_FOR_TX_MEMORY      = 1,
    WAIT_FOR_DATA_TX_MOUNT  = 2,
    WAIT_FOR_DATA_TX_SIGNAL = 3,
    WAIT_FOR_RX_IDLE        = 4
} t_TxStates;

/*
*********************************************************************************************************
*                                           LOCAL DATA TYPES
*********************************************************************************************************
*/

// this structure is present in all Rx packets received from the MRF24WG, at the
// start of the Rx RAW buffer, starting at index 0.  Payload data immediately follows
// this structure
typedef struct wfRxHeaderStruct
{
    uint8_t  type;             // always WF_DATA_RX_INDICATE_TYPE (3)
    uint8_t  subtype;
    uint16_t rssi;             // not used
    uint32_t arrivalTime;      // not used
    uint16_t dataLength;       // number of bytes of payload which immediately follow this structure
    uint8_t  snapHeader[6];    // SNAP header
    uint8_t  destAddress[6];   // destination MAC address (start of Ethernet header)
    uint8_t  srcMacAddress[6]; // source MAC address
    uint16_t ethType;          // Ethernet type code
} t_RxHeader;

typedef struct
{
    uint8_t type;
    uint8_t subType;
} t_RxPreamble;

typedef struct
{
    uint8_t          snap[SNAP_HDR_LENGTH];
    TCPIP_MAC_ADDR   DestMACAddr; // ethernet header presented to stack starts here
    TCPIP_MAC_ADDR   SourceMACAddr;
    TCPIP_UINT16_VAL Type;
} t_wfRxPreamble;

// used to keep track of rx packets queued for stack
typedef struct fifo_struct
{
    int front;
    int rear;
    TCPIP_MAC_PACKET * items[NUM_PREALLOCATED_RX_PACKETS + 1];
} t_fifo;

typedef struct
{
    uint8_t  reserved[4];
} t_wfTxPreamble;

/*
*********************************************************************************************************
*                                           LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
static uint8_t s_RawInitState;                     // state for Raw Init state machine
static uint8_t s_TxDataState;                      // state for Data Tx state machine
static uint8_t s_RxDataState;                      // state for Data Rx state machine
static SYS_TMR_HANDLE s_DataTxRawMoveTimer   = 0;  // timer callback for Data Tx raw move complete
static SYS_TMR_HANDLE s_DataRxRawMoveTimer   = 0;  // timer callback for Data Rx raw move complete
static SYS_TMR_HANDLE s_ScratchRawMoveTimer  = 0;  // timer callback for Scratch raw move complete
static SYS_TMR_HANDLE s_RxTxUnlockSleepTimer = 0;  // timer, when resume, delay 0.2 second before handle RX packet
static SINGLE_LIST s_DataTxQueue = { 0 };          // queue of data Tx packets waiting to be transmitted from host
static SINGLE_LIST s_DataRxQueue = { 0 };          // queue of data Rx packets waiting to be processed by stack from host
static t_fifo s_RxFifo;                            // FIFO to hold pointers to Rx packets
static bool s_TxWaitingForRxIdle;
static bool s_RxPacketPending;
bool g_RxDataLock = false;
bool g_TxDataLock = false;

static TCPIP_MAC_PACKET *RxBuffer_packet[NUM_PREALLOCATED_RX_PACKETS]={NULL};

// packet allocation functions as passed by the stack
static _TCPIP_PKT_ALLOC_PTR s_pktAllocF = 0;
static _TCPIP_PKT_FREE_PTR  s_pktFreeF  = 0;
static _TCPIP_PKT_ACK_PTR   s_pktAckF   = 0;

/*
*********************************************************************************************************
*                                           LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void RawInit(void);
static void RawMoveDataRxTimeoutHandler(uintptr_t context, uint32_t currTick);
static void RawMoveDataTxTimeoutHandler(uintptr_t context, uint32_t currTick);
static void RawMoveScratchTimeoutHandler(uintptr_t context, uint32_t currTick);
static bool RxDataCallback(TCPIP_MAC_PACKET * pktHandle, const void * ackParam);
static bool isRxPacketHeaderValid(void);
static TCPIP_MAC_PACKET * GetAvailRxBuf(void);
static void FifoInit(t_fifo *p_fifo);
static bool isFifoEmpty(t_fifo *p_fifo);
static void FifoInsert(t_fifo *p_fifo, TCPIP_MAC_PACKET *p_packet);
static TCPIP_MAC_PACKET * FifoRemove(t_fifo *p_fifo);
static bool isTxStateMachineIdle(void);

#if (WF_SAVE_WPS_CREDENTIALS == DRV_WIFI_ENABLED)
enum
{
    WEP_SHORT_KEY_SIZE = 5,
    WEP_LONG_KEY_SIZE = 13
};

enum
{
    SECURITY_NONE,
    SECURITY_OPEN,
    SECURITY_SHARED_KEY40,
    SECURITY_SHARED_KEY104,
    SECURITY_OPEN_KEY40,
    SECURITY_OPEN_KEY104,
    SECURITY_WPA1_PSK_KEY,
    SECURITY_WPA1_PSK_PASS,
    SECURITY_WPA2_PSK_KEY,
    SECURITY_WPA2_PSK_PASS,
    SECURITY_WPAUTO_PSK_KEY,
    SECURITY_WPAUTO_PSK_PASS,
    SECURITY_WPA_ENTERPRISE,
    SECURITY_WPS_PIN,
    SECURITY_WPS_PSB,
};

enum
{
    WEP_KEYIDX_MAX = 4,
    MSK_MAX = 64,
    PIN_MAX = 8,
};

typedef struct
{
    uint8_t key_idx;
    uint8_t key[WEP_KEYIDX_MAX][5];
}sec_wep40;

typedef struct
{
    uint8_t key_idx;
    uint8_t key[WEP_KEYIDX_MAX][13];
}sec_wep104;

typedef struct
{
    uint8_t key_len;
    uint8_t key[MSK_MAX];
}sec_wpa_psk;

typedef struct
{
    uint8_t pin[PIN_MAX];
}sec_wps;

typedef union
{
    sec_wep40 wep40;
    sec_wep104 wep104;
    sec_wpa_psk wpa_psk;
    sec_wps wps;
}sec_key;

static uint8_t ConvAscii2Hex(uint8_t a)
{
    if (a >= '0' && a <= '9')
        return (uint8_t)(a - 48);
    if (a >= 'a' && a <= 'f')
        return (uint8_t)(a - 97 + 10);
    if (a >= 'A' && a <= 'F')
        return (uint8_t)(a - 65 + 10);

    return '?';
}

static void ConvAsciiKey2Hex(uint8_t *key, uint8_t keyLen, uint8_t *hexKey)
{
    uint8_t i;

    for (i = 0; i < keyLen; i += 2)
    {
        hexKey[i / 2] = ConvAscii2Hex(key[i]) << 4;
        hexKey[i / 2] |= ConvAscii2Hex(key[i + 1]);
    }
}

static void ConfigWep(DRV_WIFI_WPS_CREDENTIAL *cred, uint8_t *secType, sec_key *key)
{
    uint8_t i;
    uint8_t wep_key[WEP_LONG_KEY_SIZE];
    sec_wep40 *wep_ctx = (sec_wep40 *)key;
    uint8_t *keys = (uint8_t *)wep_ctx + 1;
    uint8_t key_len = 0;

    if (cred->keyLen == WEP_SHORT_KEY_SIZE * 2)
    {
        *secType = DRV_WIFI_SECURITY_WEP_40;
        ConvAsciiKey2Hex(cred->netKey, cred->keyLen, wep_key);
        key_len = cred->keyLen / 2;
    }
    else if (cred->keyLen == WEP_SHORT_KEY_SIZE)
    {
        *secType = DRV_WIFI_SECURITY_WEP_40;
        memcpy(wep_key, cred->netKey, cred->keyLen);
        key_len = cred->keyLen;
    }
    else if (cred->keyLen == WEP_LONG_KEY_SIZE * 2)
    {
        *secType = DRV_WIFI_SECURITY_WEP_104;
        ConvAsciiKey2Hex(cred->netKey, cred->keyLen, wep_key);
        key_len = cred->keyLen / 2;
    }
    else if (cred->keyLen == WEP_LONG_KEY_SIZE)
    {
        *secType = DRV_WIFI_SECURITY_WEP_104;
        memcpy(wep_key, cred->netKey, cred->keyLen);
        key_len = cred->keyLen;
    }
    else
    {
        //DRV_WIFI_ASSERT(false, "");
    }

    for (i = 0; i < 4; i++)
    {
        memcpy(keys + i * key_len, wep_key, key_len);
    }

    wep_ctx->key_idx = cred->keyIdx - 1;
}

void WF_SaveWPSCredentials(void)
{
    DRV_WIFI_WPS_CREDENTIAL cred;
    sec_key key;
    uint8_t *psk;
    static bool once = false;

    if (p_wifi_ConfigData->flag_WpsSaving == DRV_WIFI_WPS_SAVING_FLAG) {
        once = true;
    }
    if (!once)
    {
        DRV_WIFI_WPSCredentialsGet(&cred);
        memcpy((void *)(p_wifi_ConfigData->netSSID), (void *)cred.ssid, cred.ssidLen);
        if (cred.ssidLen < DRV_WIFI_MAX_SSID_LENGTH) {
            p_wifi_ConfigData->netSSID[cred.ssidLen]=0;
        }
        p_wifi_ConfigData->SsidLength  = cred.ssidLen;

        switch (cred.authType) {
        case DRV_WIFI_WPS_AUTH_OPEN:
            if (cred.encType == DRV_WIFI_WPS_ENC_NONE)
            {
                p_wifi_ConfigData->SecurityMode = DRV_WIFI_SECURITY_OPEN;
            }
            else if (cred.encType == DRV_WIFI_WPS_ENC_WEP)
            {
                ConfigWep(&cred, &(p_wifi_ConfigData->SecurityMode), &key);
                if (p_wifi_ConfigData->SecurityMode == DRV_WIFI_SECURITY_WEP_40)
                {
                    memcpy((void *)p_wifi_ConfigData->SecurityKey, (void *)key.wep40.key, WEP_SHORT_KEY_SIZE * 4);
                    p_wifi_ConfigData->SecurityKeyLength = WEP_SHORT_KEY_SIZE * 4;
                }
                else if (p_wifi_ConfigData->SecurityMode == DRV_WIFI_SECURITY_WEP_104)
                {
                    memcpy((void *)p_wifi_ConfigData->SecurityKey, (void *)key.wep104.key, WEP_LONG_KEY_SIZE * 4);
                    p_wifi_ConfigData->SecurityKeyLength = WEP_LONG_KEY_SIZE * 4;
                }
                else
                {
                    //DRV_WIFI_ASSERT(false, "");
                }
            }
            break;

        case DRV_WIFI_WPS_AUTH_SHARED:
            ConfigWep(&cred, &p_wifi_ConfigData->SecurityMode, &key);
            if (p_wifi_ConfigData->SecurityMode == DRV_WIFI_SECURITY_WEP_40)
            {
                memcpy((void *)p_wifi_ConfigData->SecurityKey, (void *)key.wep40.key, WEP_SHORT_KEY_SIZE * 4);
                p_wifi_ConfigData->SecurityKeyLength = WEP_SHORT_KEY_SIZE * 4;
            }
            else if (p_wifi_ConfigData->SecurityMode == DRV_WIFI_SECURITY_WEP_104)
            {
                memcpy((void *)p_wifi_ConfigData->SecurityKey, (void *)key.wep104.key, WEP_LONG_KEY_SIZE * 4);
                p_wifi_ConfigData->SecurityKeyLength = WEP_LONG_KEY_SIZE * 4;
            }
            else
            {
                //DRV_WIFI_ASSERT(false, "");
            }
            break;

        case DRV_WIFI_WPS_AUTH_WPA_PSK:
        case DRV_WIFI_WPS_AUTH_WPA2_PSK:
            psk = (uint8_t *)p_wifi_ConfigData->SecurityKey;;
            memset((void *)psk, 0x00, 64);
            if (cred.keyLen == 64)
            {
                p_wifi_ConfigData->SecurityMode = DRV_WIFI_SECURITY_WPA_AUTO_WITH_KEY;
                p_wifi_ConfigData->SecurityKeyLength = 32;
                ConvAsciiKey2Hex(cred.netKey, cred.keyLen, psk);
            }
            else if (cred.keyLen >= 8 && cred.keyLen < 64)
            {
                p_wifi_ConfigData->SecurityMode = DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE; 
                p_wifi_ConfigData->SecurityKeyLength = cred.keyLen;
                if (p_wifi_ConfigData->SecurityKeyLength > 8 && cred.netKey[p_wifi_ConfigData->SecurityKeyLength - 1] == '\0')
                {
                    --p_wifi_ConfigData->SecurityKeyLength;
                }
                memcpy(psk, cred.netKey, p_wifi_ConfigData->SecurityKeyLength);
            }
            break;

        default:
            //DRV_WIFI_ASSERT(false, "");
            break;
        } // end switch

        p_wifi_ConfigData->flag_WpsSaving = DRV_WIFI_WPS_SAVING_FLAG;
        DRV_WIFI_ConfigDataSave();
        once = true;
    }
}
#endif /* WF_SAVE_WPS_CREDENTIALS == DRV_WIFI_ENABLED */

#if defined(WF_READ_RSSI_FROM_DATA)
// Make sure to use power of 2 for the size. Otherwise pseudo mod operation will not work.
// To adjust rssi index, we use bit-and operation instead of expensive % operation.
#define gs_rssiMaxCacheSize 16
static uint16_t gs_rssiCache[gs_rssiMaxCacheSize];
static uint32_t gs_rssiIndex = 0;
static uint16_t gs_adjustedIndex = 0;
static uint32_t gs_rssiSum = 0;
static uint16_t gs_rssiShiftDivisor = 0;
static uint16_t gs_rssiIndexWrapped = 0;

/*******************************************************************************
  Function:
    void DRV_WIFI_RSSI_Cache_FromRxDataRead(uint16_t rssi)

  Summary:
    Caches RSSI value read from receive data packet

  Description:


  Precondition:
    MACInit must be called first.

  Parameters:
    rssi - RSSI value read from receive data packet 

  Returns:
    None.

 *******************************************************************************/
void DRV_WIFI_RSSI_Cache_FromRxDataRead(uint16_t rssi)
{
    static int32_t initialized = 0;

    if (initialized == 0) {
        int32_t i;
        uint16_t cacheSize;

        for (i = 0; i < gs_rssiMaxCacheSize; i++)
            gs_rssiCache[i] = 0;
        cacheSize = gs_rssiMaxCacheSize;
        while (cacheSize > 1) {
            ++gs_rssiShiftDivisor;
            cacheSize /= 2;
        }
        initialized = 1;
    }

    gs_adjustedIndex = gs_rssiIndex & (gs_rssiMaxCacheSize - 1);
    gs_rssiSum -= gs_rssiCache[gs_adjustedIndex]; // delete old data
    gs_rssiSum += rssi; // add new data
    gs_rssiCache[gs_adjustedIndex] = rssi;
    if (gs_rssiIndex == 0xffffffff)
        gs_rssiIndexWrapped = 1;

    ++gs_rssiIndex;
}

/*******************************************************************************
  Function:
    void DRV_WIFI_RSSI_Get_FromRxDataRead(uint16_t *mean, uint16_t *last)

  Summary:


  Description:


  Precondition:
    MACInit must be called first.

  Parameters:
    mean - Pointer to where the average mean RSSI value to be stored.
    last - Pointer to where the total count of RSSI values to be stored.

  Returns:
    mean - the calculated mean RSSI.
    last - the total count of RSSI values.

 *******************************************************************************/
void DRV_WIFI_RSSI_Get_FromRxDataRead(uint16_t *mean, uint16_t *last)
{
    uint16_t divisor;

    if (gs_rssiIndexWrapped == 0)
        divisor = gs_rssiIndex < gs_rssiMaxCacheSize ? gs_rssiIndex : gs_rssiMaxCacheSize;
    else
        divisor = gs_rssiMaxCacheSize;

    if (divisor == gs_rssiMaxCacheSize) {
        *mean = gs_rssiSum >> gs_rssiShiftDivisor; // to avoid expensive division operation
    } else {
        if (divisor != 0)
            *mean = gs_rssiSum / divisor;
        else
            *mean = 0;
    }

    if (gs_adjustedIndex == 0)
        *last = gs_rssiCache[gs_rssiMaxCacheSize - 1];
    else
        *last = gs_rssiCache[gs_adjustedIndex - 1];
}
#endif /* defined(WF_READ_RSSI_FROM_DATA) */

/*******************************************************************************
 * Function:        TCPIP_MAC_RES MRF24W_MACInit(const TCPIP_MAC_MODULE_CTRL * const stackData, const TCPIP_MODULE_MAC_MRF24W_CONFIG * initData)
 *
 * PreCondition:    None
 *
 * Input:           pNetIf - network interface
 *
 * Output:          TCPIP_MAC_RES_OK if initialization succeeded,
 *                  error code otherwise
 *
 * Side Effects:    None
 *
 * Overview:        MRF24W_MACInit sets up the PIC's SPI module and all the
 *                  registers in the MRF24W so that normal operation can
 *                  begin. This function is called repeatedly until init is completed.
 *
 * Note:            When the system is using dynamically registered interrupts,
 *                  the WiFi interrupt handler should be registered here because
 *                  the DRV_WIFI_Init() enables the interrupts!
 *******************************************************************************/
TCPIP_MAC_RES MRF24W_MACInit(const TCPIP_MAC_MODULE_CTRL * const stackData, const TCPIP_MODULE_MAC_MRF24W_CONFIG * initData)
{
    int retCode;

    // called repeatedly during init, DO NOT add any code here
#if defined(WIFI_USE_RTOS)
    WF_Init_SemGive();
    retCode = WF_InitTaskState_Get();
#else
    retCode = WF_InitStateMachine();
#endif
    if (stackData)
    { // first time we're called
        s_pktAllocF = (_TCPIP_PKT_ALLOC_PTR)stackData->pktAllocF;
        s_pktFreeF = (_TCPIP_PKT_FREE_PTR)stackData->pktFreeF;
        s_pktAckF = (_TCPIP_PKT_ACK_PTR)stackData->pktAckF;
    }

    return retCode;
}

void RawResetInitStateMachine(void)
{
    s_RawInitState = R_INIT_BEGIN;
}

int RawInitStateMachine(void)
{
    int retCode = TCPIP_MAC_RES_PENDING;
    int status;
    uint16_t byteCount;

    switch (s_RawInitState) {
    //-------------------------------------
    case R_INIT_BEGIN:
    //-------------------------------------
        // create raw move timers
        CreateRawMoveTimers();

        // By default the firmware mounts Scratch to RAW 1 after reset.  If desired,
        // we can read the SysInfo data block from the Scratch.  We are not using this
        // data, so unmount the scratch from this RAW window.
        ScratchUnmount(RAW_ID_1);
        s_RawInitState = R_WAIT_FOR_SCRATCH_UNMOUNT;
        break;

    //-------------------------------------
    case R_WAIT_FOR_SCRATCH_UNMOUNT:
    //-------------------------------------
        // if raw move completed
        if (isRawMoveComplete(RAW_ID_1, &status, &byteCount))
        {
            /* Mount scratch memory, index defaults to 0.  This will stay permanently mounted.    */
            /* If one needs to know, this function returns the number of bytes in scratch memory. */
            ScratchMount(RAW_SCRATCH_ID);
            s_RawInitState = R_WAIT_FOR_SCRATCH_MOUNT;
        }
        // else if raw move not completed and it timed out
        else if (status == RM_TIMEOUT)
        {
            retCode = RAW_INIT_SCRATCH_UNMOUNT_FAIL;
        }
        break;

    //-------------------------------------
    case R_WAIT_FOR_SCRATCH_MOUNT:
    //-------------------------------------
        if (isRawMoveComplete(RAW_SCRATCH_ID, &status, &byteCount))
        {
            RawInit(); // complete raw init
            retCode = RAW_INIT_COMPLETE;
        }
        // else if raw move not completed and it timed out
        else if (status == RM_TIMEOUT)
        {
            retCode = RAW_INIT_SCRATCH_MOUNT_FAIL;
        }
        break;
    }

    return retCode;
}

/******************************************************************************
 * Function:        void RawInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Initializes the RAW window states.
 *
 * Note:            None
 ******************************************************************************/
static void RawInit(void)
{
    ClearAllIndexOutofBoundsFlags(); /* no raw indexes have been set past end of raw window */
}

/******************************************************************************
 * Function:        bool _MRF24W_MACCheckLink(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          true: If the PHY reports that a link partner is present
 *                        and the link has been up continuously since the last
 *                        call to _MRF24W_MACCheckLink()
 *                  false: If the PHY reports no link partner, or the link went
 *                         down momentarily since the last call to _MRF24W_MACCheckLink()
 *
 * Side Effects:    None
 *
 * Overview:        Returns the PHSTAT1.LLSTAT bit.
 *
 * Note:            None
 ******************************************************************************/
bool _MRF24W_MACCheckLink(void)
{
    return ( WFisConnected() );
}

/******************************************************************************
 * Function:        void MRF24W_MACPowerDown(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Placehholder function, not needed for actual operation, but
 *                  may be called by the MAC.
 *
 * Note:           None
 ******************************************************************************/
void MRF24W_MACPowerDown(void)
{
} // end MRF24W_MACPowerDown

/******************************************************************************
 * Function:        void MRF24W_MACPowerUp(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Placehholder function, not needed for actual operation, but
 *                  may be called by the MAC.
 *
 * Note:            None
 ******************************************************************************/
void MRF24W_MACPowerUp(void)
{
} // end MRF24W_MACPowerUp

// creates timers used to check for timeouts on data tx/rx raw moves
void CreateRawMoveTimers(void)
{

}


static void RxTxUnLockSleepTimeoutHandler(uintptr_t context, uint32_t currTick)
{
    g_RxDataLock = false;
    g_TxDataLock = false;
}

void MRF24W_RxTxUnLockTimerStart(void)
{
    uint16_t timeout;
    
    timeout = SYS_TMR_TickCounterFrequencyGet() / 5;
    s_RxTxUnlockSleepTimer = SYS_TMR_CallbackSingle(timeout, 0, RxTxUnLockSleepTimeoutHandler);
}
// called by RawMove()
void StartRawMoveTimer(uint16_t rawId)
{
    uint16_t timeout;

    timeout = SYS_TMR_TickCounterFrequencyGet() * 4;

    if (rawId == RAW_DATA_RX_ID)
    {
        s_DataRxRawMoveTimer = SYS_TMR_CallbackSingle(timeout, 0, RawMoveDataRxTimeoutHandler);
    }
    else if (rawId == RAW_DATA_TX_ID)
    {
        s_DataTxRawMoveTimer = SYS_TMR_CallbackSingle(timeout, 0, RawMoveDataTxTimeoutHandler);
    }
    else // must be RAW_SCRATCH_ID (only at init)
    {
        s_ScratchRawMoveTimer = SYS_TMR_CallbackSingle(timeout, 0, RawMoveScratchTimeoutHandler);
    }
}

void StopRawMoveTimer(uint16_t rawId)
{
    if (rawId == RAW_DATA_RX_ID)
    {
       SYS_TMR_CallbackStop (s_DataRxRawMoveTimer);
    }
    else if (rawId == RAW_DATA_TX_ID)
    {
        SYS_TMR_CallbackStop (s_DataTxRawMoveTimer);
    }
    else // must be RAW_SCRATCH_ID (only at init)
    {
        SYS_TMR_CallbackStop(s_ScratchRawMoveTimer);
    }
}

// called from timer interrupt when timeout occurs
static void RawMoveDataRxTimeoutHandler(uintptr_t context, uint32_t currTick)
{
    DRV_WIFI_ASSERT(false, "Rx Timeout");
}

// called from timer interrupt when timeout occurs
static void RawMoveDataTxTimeoutHandler(uintptr_t context, uint32_t currTick)
{
    DRV_WIFI_ASSERT(false, "Tx Timeout");
}

static void RawMoveScratchTimeoutHandler(uintptr_t context, uint32_t currTick)
{
    DRV_WIFI_ASSERT(false, "Scratch Timeout");
}

void InitDataTxStateMachine(void)
{

    s_TxDataState = DATA_TX_IDLE;
    s_TxWaitingForRxIdle = false;

    // create a queue to hold data tx messages that need to be sent to MRF
    TCPIP_Helper_SingleListInitialize(&s_DataTxQueue);
}

int InitDataRxStateMachine(void)
{
    int i;

    s_RxDataState = DATA_RX_IDLE;
    s_RxPacketPending = false;

    // create a queue to hold pointers to preallocated Rx packets
    TCPIP_Helper_SingleListInitialize(&s_DataRxQueue);

    for (i = 0; i < NUM_PREALLOCATED_RX_PACKETS; ++i)
    {
        // preallocate Rx buffers to store Rx packets as they come in (1500 bytes data plus header and checksum)
        RxBuffer_packet[i] = s_pktAllocF ? _TCPIP_PKT_ALLOC_BY_PTR(s_pktAllocF, sizeof(TCPIP_MAC_PACKET), MAX_RX_PACKET_SIZE, 0) : 0;
        if (RxBuffer_packet[i] != NULL)
        {
            RxBuffer_packet[i]->next = NULL;
            RxBuffer_packet[i]->ackFunc = RxDataCallback;
            RxBuffer_packet[i]->ackParam = NULL;
            RxBuffer_packet[i]->pktFlags = 0;
            RxBuffer_packet[i]->pDSeg->segFlags |= TCPIP_MAC_SEG_FLAG_RX_STICKY; // reuse; don't deallocate
            TCPIP_Helper_SingleListTailAdd(&s_DataRxQueue, (SGL_LIST_NODE *)RxBuffer_packet[i]);
        }
        // else allocation failed
        else
        {
            SYS_ERROR_PRINT(SYS_ERROR_FATAL, "\r\n%s, line %u\r\n", __FILE__, __LINE__);
            return TCPIP_MAC_RES_INIT_FAIL;
        }
    }

    // create a another FIFO to store pointers
    FifoInit(&s_RxFifo);

    return TCPIP_MAC_RES_OK;
}

void DeInitDataRxStateMachine(void)
{
    int i;
    if (s_pktFreeF != NULL)
    {
        for (i = 0; i < NUM_PREALLOCATED_RX_PACKETS; ++i)
        {
            if (RxBuffer_packet[i] != NULL)
            {
                _TCPIP_PKT_FREE_BY_PTR(s_pktFreeF, RxBuffer_packet[i]);
                RxBuffer_packet[i] = NULL;
            }
        }
    }
}

static void FifoInit(t_fifo *p_fifo)
{
    memset(p_fifo, 0x00, sizeof(t_fifo));
    p_fifo->front = p_fifo->rear = NUM_PREALLOCATED_RX_PACKETS - 1;
}

static bool isFifoEmpty(t_fifo *p_fifo)
{
    return p_fifo->front == p_fifo->rear;
}

static void FifoInsert(t_fifo *p_fifo, TCPIP_MAC_PACKET *p_packet)
{
    if (p_fifo->rear == NUM_PREALLOCATED_RX_PACKETS - 1)
    {
        p_fifo->rear = 0;
    }
    else
    {
        ++p_fifo->rear;
    }

    p_fifo->items[p_fifo->rear] = p_packet;
}

static TCPIP_MAC_PACKET * FifoRemove(t_fifo *p_fifo)
{
    if (p_fifo->front == NUM_PREALLOCATED_RX_PACKETS - 1)
    {
        p_fifo->front = 0;
    }
    else
    {
        ++p_fifo->front;
    }

    return p_fifo->items[p_fifo->front];
}

// called by stack when done with Rx packet
static bool RxDataCallback(TCPIP_MAC_PACKET * pktHandle, const void* ackParam)
{
    ackParam = ackParam; // ignore warning

    if (pktHandle)
    {
        // if this is packet allocated at init and is going to be reused
        if ((pktHandle->pDSeg->segFlags & TCPIP_MAC_SEG_FLAG_RX_STICKY) == TCPIP_MAC_SEG_FLAG_RX_STICKY)
        {
            pktHandle->pktFlags &= ~TCPIP_MAC_PKT_FLAG_QUEUED;
            TCPIP_Helper_SingleListTailAdd(&s_DataRxQueue, (SGL_LIST_NODE *)pktHandle); // add packet back to free list
        }
        // else this is a packet that we dynamically allocated because the 'permanent' packets were all in use
        else
        {
            // deallocate the Rx packet
            _TCPIP_PKT_FREE_BY_PTR(s_pktFreeF, pktHandle);
            pktHandle = NULL;
        }
    }
    else
    {
        SYS_CONSOLE_MESSAGE("!!!\r\n");
    }
    return false;
}

// called when Raw move complete event occurs
void WiFi_DataRxTask(void)
{
    int status;
    uint16_t byteCount;
    uint16_t ethPacketLength; // includes header
    TCPIP_MAC_PACKET *p_packet;
    const TCPIP_MAC_MODULE_CTRL *p_ctrl = GetStackData();
    const uint8_t* p_Mac = TCPIP_STACK_NetAddressMac(TCPIP_STACK_IndexToNet(p_ctrl->netIx));

    DecrementRxPendingCount();
    SYS_INT_SourceDisable(MRF_INT_SOURCE);

    DBG_RX_PRINT("RX: ");
    switch (s_RxDataState) {
    //======================================================================
    case DATA_RX_IDLE:
        DBG_RX_PRINT("0 ");
        if (isTxStateMachineIdle())
        {
            DBG_RX_PRINT("1 ");
            RawMountRxDataBuffer();   // start the raw Rx data mount
            s_RxDataState = WAIT_FOR_DATA_RX_MOUNT;
        }
        else
        {
            DBG_RX_PRINT("2 ");
            // schedule state machine to run again
            SignalRxStateMachine();
            IncrementRxPendingCount();
            s_RxDataState = WAIT_FOR_TX_IDLE;
        }
        break;

    //======================================================================
    case WAIT_FOR_TX_IDLE:
        // if tx state machine is idle, or tx state machine is waiting for Rx state machine
        if (isTxStateMachineIdle())
        {
            DBG_RX_PRINT("3 ");
            // then run this (rx) state machine and let tx state machine wait
            RawMountRxDataBuffer();   // start the raw Rx data mount
            s_RxDataState = WAIT_FOR_DATA_RX_MOUNT;
        }
        else
        {
            DBG_RX_PRINT("4 ");
            // schedule this state machine to run again (and recheck if tx is idle)
            SignalRxStateMachine();
            IncrementRxPendingCount();
        }
        break;

    //======================================================================
    case WAIT_FOR_DATA_RX_MOUNT:
        // this should always be true as the event is only triggered, and this
        // function should only be called, after the Raw Move has completed.
        if (isRawMoveComplete(RAW_DATA_RX_ID, &status, &byteCount))
        {
            DBG_RX_PRINT("5 ");
            { // for debugging
                char buf_t[20];

                if ( !((byteCount > 0) && (byteCount <= 1530)) )
                {
                    //SYS_ERROR_PRINT(SYS_ERROR_WARNING, "\r\nRx Packet is 0 byte or\r\nRx Packet is larger than 1528 bytes for UDP or 1530 bytes for TCP\r\n");
                    sprintf(buf_t,"%d\r\n",byteCount);
                    if (byteCount == 0)
                    {
                        SYS_CONSOLE_MESSAGE("Rx Packet byteCount=");
                        SYS_CONSOLE_MESSAGE(buf_t);
                    }
                    else
                    {
                        SYS_CONSOLE_MESSAGE("Rx Packet is larger than 1528 bytes for UDP or 1530 bytes for TCP, byteCount=");
                        SYS_CONSOLE_MESSAGE(buf_t);
                    }
                    DRV_WIFI_ASSERT(false, "");
                }
            }

            // if locked, receive packet then throw it away
            if (g_RxDataLock) 
            {
                if (byteCount > ETH_HEADER_START_OFFSET)
                {
                    uint8_t dump_buffer[512];
                    uint32_t count_read = 0, tmp2=0;
                    
                    SYS_CONSOLE_PRINT("byteCount=%d\r\n",byteCount);
                    ethPacketLength = byteCount - ETH_HEADER_START_OFFSET;
                    while (ethPacketLength > 0)
                    {
                        if (ethPacketLength > 512)
                            tmp2 = 512;
                        else
                            tmp2 = ethPacketLength;

                        RawRead(RAW_DATA_RX_ID, ETH_HEADER_START_OFFSET+count_read, tmp2, dump_buffer);
                        ethPacketLength -= tmp2;
                        count_read += tmp2;
                    }
                }
            }
            // if raw move successfully completed and the Rx packet header is valid
            else if ((status == RM_COMPLETE) && (isRxPacketHeaderValid()))
            {
                DBG_RX_PRINT("6 ");
                

                // get an Rx buffer structure from queue to copy rx packet to (from MRF to Rx buffer struct)
                p_packet = GetAvailRxBuf();

                // if packet structure available
                if (p_packet != NULL)
                {
                    DBG_RX_PRINT("7 ");
                    // read Ethernet packet into host buffer
                    // set raw pointer to start of 802.11 payload (start of Ethernet packet)
                    ethPacketLength = byteCount - ETH_HEADER_START_OFFSET;
                    RawRead(RAW_DATA_RX_ID, ETH_HEADER_START_OFFSET, ethPacketLength, p_packet->pDSeg->segLoad);

                    // if we received our own broadcast then throw it away
                    if ( memcmp(&p_packet->pDSeg->segLoad[6], p_Mac, 6) == 0 )
                    {
                        DBG_RX_PRINT("8 ");
                        // put buffer back in free list
                        TCPIP_Helper_SingleListTailAdd(&s_DataRxQueue, (SGL_LIST_NODE *)p_packet);
                    }
                    // else flag packet as queued and signal stack to process it
                    else
                    {
                        DBG_RX_PRINT("9 ");
                        // mark packet as queued and stuff in timestamp
                        p_packet->pDSeg->segLen = ethPacketLength - ETH_HEADER_SIZE;
                        p_packet->pktFlags |= TCPIP_MAC_PKT_FLAG_QUEUED;
                        p_packet->tStamp = SYS_TMR_TickCountGet();

                        // Note: re-set pMacLayer and pNetLayer; IPv6 changes these pointers inside the packet, so
                        //       when Rx packets are reused this is needed.
                        p_packet->pMacLayer = p_packet->pDSeg->segLoad;
                        p_packet->pNetLayer = p_packet->pMacLayer + sizeof(TCPIP_MAC_ETHERNET_HEADER);

                        // store packet pointer in FIFO and signal stack that rx packet ready to process
                        FifoInsert(&s_RxFifo, p_packet);

                        // notify stack of Rx packet
                        // SYS_CONSOLE_MESSAGE(" NS\r\n");
                        WF_UserEventsSet(TCPIP_MAC_EV_RX_DONE, 0, false);
                    }
                }
                // else out of pre-queued Rx packets, or, could not allocate a new one
                else
                {
                    SYS_CONSOLE_MESSAGE("Out of Rx -- throw away packet\r\n");
                }
            }
            // else if raw move timed out (note: invalid, proprietary packet is OK, so does not cause assert)
            else if (status == RM_TIMEOUT)
            {
                DRV_WIFI_ASSERT(false, "");
            }
            else
            {
                // We were hitting this case when a proprietary packet was received, so we should
                // not assert, but simply throw packet away, and not assert as we had been doing.
                ;
            }

            // begin raw unmount of data rx packet (it's either been buffered for stack or being thrown away)
            DeallocateDataRxBuffer();
            s_RxDataState = WAIT_FOR_DATA_RX_UNMOUNT;
        }
        // else Raw Move not yet complete (still waiting)
        else
        {
            DRV_WIFI_ASSERT(false, "");
        }
        break;

    //======================================================================
    case WAIT_FOR_DATA_RX_UNMOUNT:
        if (isRawMoveComplete(RAW_DATA_RX_ID, &status, &byteCount))
        {
            DBG_RX_PRINT("10 ");
            // if timed out waiting for raw move to complete

            s_RxDataState = DATA_RX_IDLE;

            // if notified of new rx packet while waiting for Raw move complete
            if (s_RxPacketPending == true)
            {
                DBG_RX_PRINT("12 ");
                // kick-start state machine again
                s_RxPacketPending = false;
                SignalRxStateMachine();
                IncrementRxPendingCount();
            }
            // else if tx state machine waiting for rx state machine to go idle
            else if (s_TxWaitingForRxIdle == true)
            {
                DBG_RX_PRINT("11 ");
                // kick-start Tx state machine so it can finish
                s_TxWaitingForRxIdle = false;
                SignalTxStateMachine();
            }
        }
        else
        {
            DRV_WIFI_ASSERT(false, "");
        }
        break;
    } // end switch

    DBG_RX_PRINT("\r\n");
    DRV_WIFI_INT_SourceEnable();
}

static bool isRxPacketHeaderValid(void)
{
    t_RxPreamble wfPreamble;
    uint8_t      snapHdr[6];

#ifdef WF_READ_RSSI_FROM_DATA
    //uint16_t t_mean;
    //uint16_t t_last;

    typedef struct
    {
        uint16_t rxPreamble;
        uint16_t rssi; /* the value of the MRF24WG RSSI when the data frame was received */
        uint16_t dummy1;
        uint16_t dummy2;
        uint16_t dummy3;
        uint8_t snapHeader[6]; /* Wi-Fi specific Snap header */
        uint8_t dstAddr[WF_MAC_ADDRESS_LENGTH]; /* MAC Address to which the data frame was directed. */
        uint8_t srcAddr[WF_MAC_ADDRESS_LENGTH]; /* MAC Address of the Station that sent the Data frame. */
        uint32_t type;
    } tWFRxDataIndicate; /* 28 bytes */
    tWFRxDataIndicate rawHeader;

    RawRead(RAW_DATA_RX_ID, 0, WF_RX_PREAMBLE_SIZE + 10, (uint8_t *)&rawHeader);

    // You got RSSI field, rawHeader.rssi.
    // Valid Range : RSSI_MAX (128), RSSI_MIN (43). The larger the stronger
    DRV_WIFI_RSSI_Cache_FromRxDataRead(rawHeader.rssi >> 8);
    //DRV_WIFI_RSSI_Get_FromRxDataRead(&t_mean, &t_last);
    //SYS_CONSOLE_PRINT("\r\nRSSI=%d, t_mean=%d, t_last=%d",rawHeader.rssi >> 8,t_mean,t_last);

    memcpy((void *)&wfPreamble, (void *)&rawHeader.rxPreamble, sizeof(t_RxPreamble));
    DRV_WIFI_ASSERT(wfPreamble.type == WF_DATA_RX_INDICATE_TYPE, "");

    memcpy((void *)&snapHdr, (void *)rawHeader.snapHeader, sizeof(rawHeader.snapHeader));
#else
    // read the data frame internal preamble (type and subtype bytes) to verify that we did, in
    // fact, mount an Rx data packet.  This read auto-increments the raw index to the first
    // actual data byte in the frame.
    //RawGetByte(RAW_DATA_RX_ID, (uint8_t*)&wfPreamble, sizeof(t_RxPreamble));
    RawRead(RAW_DATA_RX_ID, 0, sizeof(t_RxPreamble), (uint8_t*)&wfPreamble);
    DRV_WIFI_ASSERT(wfPreamble.type == WF_DATA_RX_INDICATE_TYPE, "");

    // read snap header in Rx packet and verify it; if snap header is not valid
    // this is a proprietary packet that will be thrown away by Rx state machine.
    RawRead(RAW_DATA_RX_ID, SNAP_HEADER_OFFSET, 6, (uint8_t *)&snapHdr);
#endif // WF_READ_RSSI_FROM_DATA

    // verify that the expected bytes contain the SNAP header; if not, this is a
    // proprietary packet that will be thrown away by Rx state machine.
    if (!(snapHdr[0] == SNAP_VAL        &&
          snapHdr[1] == SNAP_VAL        &&
          snapHdr[2] == SNAP_CTRL_VAL   &&
          snapHdr[3] == SNAP_TYPE_VAL   &&
          snapHdr[4] == SNAP_TYPE_VAL   &&
          snapHdr[5] == SNAP_TYPE_VAL) )
    {
        return false;
    }

    return true;
}

// finds an available Rx packet structure from the list that was allocated and
// queued up at init.
static TCPIP_MAC_PACKET * GetAvailRxBuf(void)
{
    TCPIP_MAC_PACKET *p_packet = NULL;

    // if free list has an available Rx packet buffer
    if (s_DataRxQueue.nNodes > 0)
    {
        p_packet = (TCPIP_MAC_PACKET *)TCPIP_Helper_SingleListHeadRemove(&s_DataRxQueue);
        DRV_WIFI_ASSERT(p_packet != NULL, "");
    }
    return p_packet;
}

bool isDataTxTaskInactive(void)
{
    return (s_TxDataState == DATA_TX_IDLE);
}

// retrieve the oldest of the queued Rx packets to deliver to the stack
TCPIP_MAC_PACKET * MRF24W_GetRxPacket(void)
{
    if (!isFifoEmpty(&s_RxFifo))
    {

        return FifoRemove(&s_RxFifo);
    }
    else
    {
        return NULL; // signals no rx packet to process
    }
}

static uint16_t GetTxPacketLength(TCPIP_MAC_PACKET *p_packet)
{
    uint16_t packetLength;
    TCPIP_MAC_DATA_SEGMENT *p_seg = p_packet->pDSeg; // point to first segment

    // number of bytes in first segment
    packetLength = p_seg->segLen;

    // loop thru any other segments and add their data length
    while (p_seg->next != NULL)
    {
        p_seg = p_seg->next;
        packetLength += p_seg->segLen;
    }

    // return the packet length plus extra 4 bytes needed for internal header
    return packetLength + WF_TX_DATA_MSG_PREAMBLE_LENGTH;
}

// called by mrf24w_MACTxPacket when stack wants to send a packet
TCPIP_MAC_RES MRF24W_TxPacket(TCPIP_MAC_PACKET *p_packet)
{
    if (DRV_WIFI_isHibernateEnable())
    {
        SYS_CONSOLE_MESSAGE("Dropped the Tx packet. Tx is not allowed in Hibernate mode.\r\n");
        return TCPIP_MAC_RES_QUEUE_TX_FULL;
    }

    // if locked, don't send packet
    //if (g_TxDataLock)
    //{
    //    return TCPIP_MAC_RES_QUEUE_TX_FULL;
    //}
    
    // if we get too many tx packets queued up, start throwing them away
    if (s_DataTxQueue.nNodes >= MAX_TX_QUEUE_COUNT)
    {
        SYS_CONSOLE_PRINT("Dropped the Tx packet. Tx queue is now full (%d).\r\n", s_DataTxQueue.nNodes);
        return TCPIP_MAC_RES_QUEUE_TX_FULL;
    }

    // if a packet size is too huge, we drop the packet.
    if (p_packet->pDSeg->segLen > 1518)
    {    
        SYS_CONSOLE_PRINT("Dropped the Tx packet. Invalid packet length. Too large: %d\r\n",p_packet->pDSeg->segLen);
        if (s_pktAckF)
        {
            _TCPIP_PKT_ACK_BY_PTR(s_pktAckF, p_packet, TCPIP_MAC_PKT_ACK_TX_OK);
        }
        return TCPIP_MAC_RES_OK;
    }

    // queue the Tx Data packet pointer
    TCPIP_Helper_SingleListTailAdd(&s_DataTxQueue, (SGL_LIST_NODE *)p_packet);

    // signal list manager that this packet cannot be reused until the
    // ack function is called.
    p_packet->pktFlags |= TCPIP_MAC_PKT_FLAG_QUEUED;

    // if Tx Data Task is idle and there is only a single message in the queue then
    // kick-started the Tx Data Task.  If there is more than one message in the queue
    // then we've had at least two calls to this function before the Tx Data Task had
    // a chance to run, but we only want to call it once, not twice.  The task
    // will check for additional tx packets when it finishes the previous one.
    if ((s_TxDataState == DATA_TX_IDLE) && (s_DataTxQueue.nNodes == 1)) // node check debug
    {
        WiFi_DataTxTask();
    }

    return TCPIP_MAC_RES_OK;
}

static void PrepTxPacketHeaders(TCPIP_MAC_PACKET *p_packet)
{
    uint8_t  *p_load;
    TCPIP_MAC_ETHERNET_HEADER *p_etherHeader;
    uint16_t type;

    p_etherHeader = (TCPIP_MAC_ETHERNET_HEADER *)(p_packet->pMacLayer);
    type = p_etherHeader->Type;

    // overwrite source address in Ethernet header with SNAP header.  The Ethernet
    // source address starts at index 6 in the Ethernet packet
    p_load = p_packet->pDSeg->segLoad; // start of packet

    // overwrite bytes [6] thru [11] in ethernet header to SNAP header
    p_load[6]  = SNAP_VAL;
    p_load[7]  = SNAP_VAL;
    p_load[8]  = SNAP_CTRL_VAL;
    p_load[9]  = SNAP_TYPE_VAL;
    p_load[10] = SNAP_TYPE_VAL;
    p_load[11] = SNAP_TYPE_VAL;

    // overwrite indexes [12], [13] with Ethernet type for protocol being used (already in network order)
    p_load[12]   = (uint8_t)type;
    p_load[13] = (uint8_t)(type >> 8);

    // now that the packet has been modified for WiFi mac, prepend the internal msg
    // header used by SPI interface.  These four bytes are immediately prior to the
    // p_DSeg pointer (the actual 'secret' buffer is the size of segLoadOffset).
    p_load = p_packet->pDSeg->segLoad - WF_TX_PREAMBLE_SIZE; // have p_load point to 4 bytes before pDSeg->segLoad

    // write out internal preamble
    p_load[0] = WF_DATA_REQUEST_TYPE;
    p_load[1] = WF_STD_DATA_MSG_SUBTYPE;
    p_load[2] = 1;
    p_load[3] = 0;
}

static bool isTxStateMachineIdle(void)
{
    // tx state machine is logically idle if its state is idle or it is waiting for the rx state machine to go idle
    return (s_TxDataState == DATA_TX_IDLE) || (s_TxDataState == WAIT_FOR_RX_IDLE);
}

bool isRxStateMachineIdle(void)
{
    return (s_RxDataState == DATA_RX_IDLE);
}

void SetRxPacketPending(void)
{
    s_RxPacketPending = true;
}

void WiFi_DataTxTask(void)
{
    int status;
    uint16_t byteCount = 0;
    uint8_t *p_segData;
    TCPIP_MAC_DATA_SEGMENT *p_seg;
    static TCPIP_MAC_PACKET *p_packet = NULL; // packet currently being sent
    static uint16_t packetLength = 0; // total length of buffer needed on MRF
    bool txStateMachineActive = true;

    SYS_INT_SourceDisable(MRF_INT_SOURCE);
    DBG_TX_PRINT("TX: ");

    while (txStateMachineActive)
    {
        txStateMachineActive = false;

        switch (s_TxDataState) {
        //======================================================================
        case DATA_TX_IDLE:
            DBG_TX_PRINT("1 ");
            // if any tx packets in queue
            if (s_DataTxQueue.nNodes > 0)
            {
                DBG_TX_PRINT("2 ");
                // point to Tx packet, but do not yet remove it from the queue
                p_packet = (TCPIP_MAC_PACKET *)s_DataTxQueue.head;
                DRV_WIFI_ASSERT(p_packet != NULL, "");

                // prepend internal WiFi header and modify Ethernet header to SNAP; get packet length
                PrepTxPacketHeaders(p_packet);
                packetLength = GetTxPacketLength(p_packet);

                if (isRxStateMachineIdle())
                {
                    DBG_TX_PRINT("3 ");
                    // start raw mount if bytes available
                    if (AllocateDataTxBuffer(packetLength))
                    {
                        DBG_TX_PRINT("4 ");
                        s_TxDataState = WAIT_FOR_DATA_TX_MOUNT;
                    }
                    // else not enough bytes available on MRF right now
                    else
                    {
                        DBG_TX_PRINT("5 ");
                        // schedule state machine to run again and check if it can proceed
                        SignalTxStateMachine();
                        s_TxDataState = WAIT_FOR_TX_MEMORY;
                    }
                }
                // else need to wait for Rx state machine to finish
                else
                {
                    DBG_TX_PRINT("6 ");
                    s_TxDataState = WAIT_FOR_RX_IDLE;
                    s_TxWaitingForRxIdle = true; // let rx state machine know that tx state machine is waiting for it
                }
            }
            else
            {
                DRV_WIFI_ASSERT(p_packet != NULL, "");
            }
            break;

        //======================================================================
        case WAIT_FOR_RX_IDLE:
            DBG_TX_PRINT("7 ");
            if (isRxStateMachineIdle())  // should always be true
            {
                // start raw mount if bytes available
                if (AllocateDataTxBuffer(packetLength))
                {
                    DBG_TX_PRINT("8 ");
                    s_TxDataState = WAIT_FOR_DATA_TX_MOUNT;
                }
                // else not enough bytes available on MRF right now
                else
                {
                    DBG_TX_PRINT("9 ");
                    // schedule state machine to run again
                    SignalTxStateMachine();
                    s_TxDataState = WAIT_FOR_TX_MEMORY;
                }
            }
            else
            {
                DRV_WIFI_ASSERT(false, "");
            }
            break;

        //======================================================================
        case WAIT_FOR_TX_MEMORY:
            DBG_TX_PRINT("10 ");
            if (isRxStateMachineIdle())
            {
                DBG_TX_PRINT("11 ");
                // attempt to allocate data buffer on MRF (if bytes available then Raw Move has been initiated)
                if (AllocateDataTxBuffer(packetLength))
                {
                    DBG_TX_PRINT("12 ");
                    s_TxDataState = WAIT_FOR_DATA_TX_MOUNT;
                }
                // else not enough bytes available on MRF right now
                else
                {
                    DBG_TX_PRINT("13 ");
                    // schedule state machine to run again
                    SignalTxStateMachine(); // should notmake a diffeerence
                }
            }
            else
            {
                DBG_TX_PRINT("14 ");
                s_TxDataState = WAIT_FOR_RX_IDLE;
                s_TxWaitingForRxIdle = true; // let rx state machine know that tx state machine is waiting for it
            }
            break;

        //======================================================================
        case WAIT_FOR_DATA_TX_MOUNT:
            DBG_TX_PRINT("15 ");
            // if tx data buffer successfully mounted (or timed out trying)
            if (isRawMoveComplete(RAW_DATA_TX_ID, &status, &byteCount))
            {
                DBG_TX_PRINT("16 ");
                if ((status == RM_COMPLETE) && (byteCount != 0))
                {
                    DBG_TX_PRINT("17 ");
                    // if, after the raw mount, we didn't get all the bytes we needed
                    if (byteCount < packetLength)
                    {
                        SYS_CONSOLE_MESSAGE("IVB\r\n");
                    }

                    uint16_t curIndex = 0;

                    // write out the first segment to MRF, including prepended internal header
                    p_seg = p_packet->pDSeg;  // point to first segment
                    p_segData = p_seg->segLoad - WF_TX_PREAMBLE_SIZE;
                    RawWrite(RAW_DATA_TX_ID, 0, p_seg->segLen + WF_TX_PREAMBLE_SIZE, p_segData);
                    curIndex += p_seg->segLen + WF_TX_PREAMBLE_SIZE;

                    // write out any other segments to MRF
                    while (p_seg->next != NULL)
                    {
                        DBG_TX_PRINT("18 ");
                        p_seg = p_seg->next;
                        p_segData = p_seg->segLoad;
                        RawWrite(RAW_DATA_TX_ID, curIndex, p_seg->segLen, p_segData);
                        curIndex += p_seg->segLen;
                    }

                    // Now that full packet copied to MRF24WG, signal it that packet ready to transmit (start raw move)
                    SendRAWDataFrame(packetLength);

                    s_TxDataState = WAIT_FOR_DATA_TX_SIGNAL;

                }
                else if (byteCount == 0)
                {
                    DRV_WIFI_ASSERT(false, "");
                }
            }
            else
            {
                DRV_WIFI_ASSERT(false, "");
            }
            break;

        //======================================================================
        case WAIT_FOR_DATA_TX_SIGNAL:
            DBG_TX_PRINT("19 ");
            // if raw move complete that signals MRF24WG that there is a tx data
            // packet to transmit
            if (isRawMoveComplete(RAW_DATA_TX_ID, &status, &byteCount))
            {
                DBG_TX_PRINT("20 ");
                if (status == RM_COMPLETE)
                {
                    DBG_TX_PRINT("21 ");


                    // remove tx packet from the list
                    p_packet = (TCPIP_MAC_PACKET *)TCPIP_Helper_SingleListHeadRemove(&s_DataTxQueue);
                    //SYS_CONSOLE_PRINT("R=%d\r\n", s_DataTxQueue.nNodes);

                    // call stack ack function to let it know packet was transmitted
                    if (s_pktAckF)
                    {
                        _TCPIP_PKT_ACK_BY_PTR(s_pktAckF, p_packet, TCPIP_MAC_PKT_ACK_TX_OK);
                    }
                    else
                    {
                        DRV_WIFI_ASSERT(false, "");
                    }

                    // if any pending tx packets
                    if (s_DataTxQueue.nNodes > 0)
                    {
                        DBG_TX_PRINT("22 ");
                        txStateMachineActive = true;
                    }

                    s_TxDataState = DATA_TX_IDLE; // ready for next tx data
                }
                else
                {
                    DRV_WIFI_ASSERT(false, "");
                }
            }
            else
            {
                DRV_WIFI_ASSERT(false, "");
            }
            break;
        } // end switch

    } // end while

    DRV_WIFI_INT_SourceEnable();

    DBG_TX_PRINT("\r\n");
}

void WiFi_ReleaseTxHeapInRedirection(void)
{
    TCPIP_MAC_PACKET *p_packet = NULL;
    // remove tx packet from the list
    while (s_DataTxQueue.nNodes > 0)
    {
        p_packet = (TCPIP_MAC_PACKET *)TCPIP_Helper_SingleListHeadRemove(&s_DataTxQueue);
        // call stack ack function to let it know packet was transmitted
        if (s_pktAckF)
        {
            _TCPIP_PKT_ACK_BY_PTR(s_pktAckF, p_packet, TCPIP_MAC_PKT_ACK_TX_OK);
        }
        else
        {
            DRV_WIFI_ASSERT(false, "");
        }
    }
}

#endif /* TCPIP_IF_MRF24W*/

//DOM-IGNORE-END
